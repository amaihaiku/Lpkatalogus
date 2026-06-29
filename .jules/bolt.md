## 2024-05-20 - True DMA usage requires double buffering and proper memory allocation
**Learning:** Using `pushPixelsDMA` with a single buffer causes visual tearing and provides no parallelization benefits. True DMA requires double-buffering. Also, the DMA buffer must be allocated with `heap_caps_malloc(..., MALLOC_CAP_DMA)`.
**Action:** When migrating from `pushPixels` to `pushPixelsDMA` for SPI TFT displays, always convert to a double-buffered approach and allocate buffers with `MALLOC_CAP_DMA`.
