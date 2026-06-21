1. **Understand the Goal**: The user wants *ONE small performance improvement* that makes the application measurably faster or more efficient. Since this is an ESP32 application pushing pixels to a display (TFT display), drawing speed is critical.
2. **Identify Bottleneck**: The `Renderer.cpp` updates the display by calling `m_tft.pushPixels(pixelBuffer, 256 * 8)`. This function is implemented via `sendPixels` which uses `spi_device_polling_start`, then does `dmaWait()`. `m_tft.pushPixels` blocks until the previous DMA transaction finishes, then copies the pixels into an internal buffer (`memcpy(buffer, data, len)`), then starts the DMA transaction and returns. As a result, the computation of the next row of pixels has to wait until the `memcpy` is done, and it operates on a single buffer. Wait! No, the computation computes a single `pixelBuffer` line of 256 pixels, then calls `pushPixels`, which blocks waiting for the *previous* transfer to finish!
So the timeline for rendering 24 rows was:
`Compute row 1` -> `Wait for previous frame (immediate)` -> `Copy row 1 to DMA buffer` -> `Start DMA for row 1`.
`Compute row 2` -> `Wait for row 1 DMA to finish` -> `Copy row 2 to DMA buffer` -> `Start DMA for row 2`.
This meant that computing row 2 *does NOT overlap* with the DMA transfer of row 1! Wait!
Actually, `pushPixels` was doing:
```cpp
void TFTDisplay::sendPixels(const uint16_t *data, int numPixels)
{
  int bytes = numPixels * 2;
  for (uint32_t i = 0; i < bytes; i += DMA_BUFFER_SIZE) {
    uint32_t len = std::min(DMA_BUFFER_SIZE, bytes - i);
    dmaWait(); // waits for row N-1
    _transaction->setPixels(data + i / 2, len / 2); // memcpys row N into DMA buffer
    sendTransaction(_transaction); // starts DMA for row N and returns immediately
  }
}
```
Wait! Since `sendTransaction` returns immediately, the DMA for row N runs in the background. Then `pushPixels` returns, and the `Renderer` computes row N+1.
So computing row N+1 *DOES overlap* with the DMA transfer of row N!
BUT: There is a `memcpy` of the entire row (4KB) into the DMA buffer.
AND: The `memcpy` happens *after* `dmaWait()` finishes.

By using double-buffering for `pixelBuffer` directly allocated with `MALLOC_CAP_DMA`:
1. We compute row N into `pixelBuffers[currentBufferIndex]`.
2. We call `pushPixelsDMA` which calls `dmaWait()` (waiting for row N-1 to finish), then sets `_transaction->tx_buffer` to `pixelBuffers[currentBufferIndex]` directly, without ANY `memcpy`, and starts DMA.
3. Then `Renderer` switches to `currentBufferIndex = 1 - currentBufferIndex` and computes row N+1 into the other buffer while row N is transferring via DMA.

**Impact**:
- We completely eliminate the `memcpy(buffer, data, len)` for every row (256 * 8 * 2 = 4096 bytes per row * 24 rows = 98KB of `memcpy` per frame eliminated).
- Because we don't do `memcpy`, we also eliminate the need to allocate the generic 32KB `DMA_BUFFER` for pixels if we only used DMA push (but `TFTDisplay` still uses it for other things).
- More importantly, we reduce CPU usage and potentially slightly increase frame rate because the CPU doesn't spend time copying memory and can focus purely on decoding the Spectrum screen.
- True Zero-copy DMA!

3. **Verify Implementation**:
- Added `pushPixelsDMA` to `TFTDisplay` to accept a direct DMA buffer.
- Changed `Renderer.h` to use `pixelBuffers[2]` allocated via `heap_caps_malloc(..., MALLOC_CAP_DMA)`.
- Changed `Renderer.cpp` to use the current buffer and swap after pushing.
- Fixed a compilation error regarding variable names (`mm_` prefix instead of `m_`) in `EmulatorScreen.cpp` that I accidentally introduced or that was present on my end.
- Verified that `pio run -e cheap-yellow-display` succeeds.

4. **Complete pre-commit**:
- Add comments explaining the optimization.
- Create journal entry in `.jules/bolt.md`.
- Run formatting or whatever.

5. **Commit and PR**:
- Title: ⚡ Bolt: [performance improvement]
- Description with What, Why, Impact, Measurement.
