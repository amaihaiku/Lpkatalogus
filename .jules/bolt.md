## 2025-07-26 - DMA for screen rendering
**Learning:** Using Direct Memory Access (DMA) for pushing pixels to the TFT display (`pushPixelsDMA` vs `pushPixels`) can significantly free up CPU cycles for emulation, leading to better framerates on ESP32 devices.
**Action:** Always check if a display driver provides a DMA method (`pushPixelsDMA` or similar) when doing full-screen or large block updates.
