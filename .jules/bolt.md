## 2024-06-22 - [Double-buffering DMA]
**Learning:** When attempting to use DMA (e.g., `pushPixelsDMA`) for display transfers, using a single shared buffer provides zero parallelization benefit. True DMA performance gains in this architecture require double-buffering and allocating the buffers with `heap_caps_malloc(..., MALLOC_CAP_DMA)` to ensure they are accessible by the DMA controller.
**Action:** When implementing DMA, always ensure double-buffering is used and buffers are correctly allocated with `MALLOC_CAP_DMA`.
