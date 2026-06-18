## 2024-10-24 - Double-buffering is required for true DMA parallelization
**Learning:** Single-buffer DMA transfers require `dmaWait` to be called before modifying the buffer for the next transfer to prevent memory corruption. This blocks the CPU, negating parallelization benefits.
**Action:** Always implement double-buffering when performing sequence-based asynchronous DMA operations, allocating with `heap_caps_malloc(..., MALLOC_CAP_DMA)` and alternating the active buffer.
