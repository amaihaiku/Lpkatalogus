## 2024-05-18 - [TFT Display Row Rendering Optimization]
**Learning:** ESP32 rendering for a retro emulator can bottleneck on unnecessary character block repaints when iterating attribute and pixel bytes individually per frame, even when caching previous states.
**Action:** Always implement a fast-path chunk comparison (e.g. `memcmp` on the 32-byte row chunks) to skip the deep per-pixel decode loop for static screen areas, which minimizes SPI pushes and speeds up emulator framerates.
