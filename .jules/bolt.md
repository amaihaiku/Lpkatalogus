## 2026-07-02 - DMA Single Buffering Anti-Pattern
**Learning:** Implementing SPI DMA with only a single shared buffer provides zero parallelization benefit. The CPU is forced to wait for the DMA transfer to complete before it can prepare the next chunk of data, completely negating the asynchronous advantage of DMA.
**Action:** Always implement DMA display transfers using a double-buffering scheme (allocating with `MALLOC_CAP_DMA`). Ensure the CPU prepares the inactive buffer while the DMA controller asynchronously transfers the active buffer.
