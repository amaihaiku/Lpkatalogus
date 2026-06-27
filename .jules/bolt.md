## 2024-10-18 - True DMA Performance requires Double Buffering
**Learning:** When attempting to use DMA for display transfers, using a single shared buffer provides zero parallelization benefit. The CPU is blocked waiting for the transfer to finish before putting the next batch of data into the buffer.
**Action:** Always use double-buffering (allocating buffers with `heap_caps_malloc(..., MALLOC_CAP_DMA)`) for DMA transfers. Prepare data in buffer B while DMA transfers from buffer A, then wait and swap.
