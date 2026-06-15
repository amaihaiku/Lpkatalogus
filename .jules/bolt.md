## 2024-05-24 - Single Buffer DMA Stall
**Learning:** In the `Renderer::drawSpectrumScreen()` function, attempting to optimize display transfers with `pushPixelsDMA()` using a single `pixelBuffer` provides zero parallelization benefit. Because the buffer is reused on the very next loop iteration, `dmaWait()` must be called immediately, stalling the CPU while the DMA transfer finishes. The CPU and DMA remain strictly serialized.
**Action:** Do not use `pushPixelsDMA` unless double-buffering is implemented, as single-buffered DMA adds setup overhead without any throughput gain.
