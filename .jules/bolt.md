## 2026-04-14 - Optimize pixel dirty checking by tracking lines via memcmp
**Learning:** Comparing entire memory blocks with memcmp on the ESP32 is much faster than pixel-by-pixel comparisons, especially for high frame rate renderings. Bypassing pixel level tracking entirely if line contents haven't changed produces massive performance gains on this architecture.
**Action:** When iterating over predictable size pixel chunks, precalculate offsets and try chunk-level byte comparison with memcmp before committing to loop-heavy checks on single pixels.
