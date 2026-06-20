1. **Update `TFTDisplay.h`**
   - Change `SPITransactionInfo *_transaction;` (line 85) to `SPITransactionInfo *_transactions[2];`.
   - Add an integer `_transIndex` (initialized to 0) to track the active transaction buffer.

2. **Update `TFTDisplay.cpp`**
   - In the constructor (line 116), change `_transaction = new SPITransactionInfo(DMA_BUFFER_SIZE);` to initialize both `_transactions[0]` and `_transactions[1]`.
   - In `sendCmd` (lines 121-125):
     - Calculate inactive index: `int inactiveIndex = 1 - _transIndex;`
     - Prepare data on inactive buffer: `_transactions[inactiveIndex]->setCommand(cmd);`
     - Call `dmaWait();`
     - Update index: `_transIndex = inactiveIndex;`
     - Start transaction: `sendTransaction(_transactions[_transIndex]);`
   - In `sendPixels` (lines 145-153):
     - Loop through data by `DMA_BUFFER_SIZE`.
     - Calculate inactive index.
     - Prepare data: `_transactions[inactiveIndex]->setPixels(...)`.
     - Call `dmaWait()`.
     - Update index and call `sendTransaction()`.
   - Apply the same pattern to `sendData` and `sendColor`.
   - In `TFTDisplay` constructor (line 109), increase `queue_size` in `spi_device_interface_config_t` from `1` to `2` to allow queueing.

3. **Add Journal Entry**
   - Add an entry to `.jules/bolt.md` detailing the transition from single to double DMA buffering for `pushPixelsDMA` to optimize DMA parallelization in the architecture.

4. **Verify Compilation**
   - Run `pio run -e cheap-yellow-display` or `pio run -e esp32-s3-custom-joystick` to verify compilation.

5. Complete pre-commit steps to ensure proper testing, verification, review, and reflection are done.

6. **Submit PR**
   - Submit PR formatted correctly with `⚡ Bolt: [performance improvement]`.
