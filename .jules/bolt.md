## 2025-01-28 - Fast Screen Row Rendering
**Learning:** ZX Spectrum emulators on ESP32 often do unnecessary pixel buffer computation for clean rows. Since the 8 scanlines of a character row are continuous across the X-axis (32 bytes per scanline), we can use highly optimized `memcmp` over those scanlines to perform a lightning-fast dirty check.
**Action:** By short-circuiting the rendering using `memcmp`, we bypass the expensive memory reads, palette lookups, and 32-bit pixel generations for ~95% of the rows (which are unchanged between frames).
