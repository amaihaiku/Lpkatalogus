
## $(date +%Y-%m-%d) - Renderer Flash State Change Handling
**Learning:** In software renderers for platforms like ESP32 where memory is directly modified frame-by-frame, `memcmp` provides excellent fast-paths. However, attributes like 'flashing text' present edge-cases where the underlying data doesn't change, but the screen must still be updated. The ZX spectrum flash state reverses colors roughly twice a second.
**Action:** Always check the interplay of fast-paths with external state/timing flags. Ensure time-based rendering attributes force updates even if the memory chunks are unchanged from the previous frame.
