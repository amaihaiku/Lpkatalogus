## 2024-06-04 - [Cache ASCII Glyphs in TFT Display]
**Learning:** `getGlyphData` sequentially iterated through all glyphs in a VLW font for every character drawn. This is an O(N) lookup that blocks the UI rendering path.
**Action:** Implemented a fixed-size array cache (`Glyph glyphCache[128]`) for ASCII characters (0-127), dropping the lookup complexity to O(1) for common text rendering scenarios. This is a critical pattern when performing text-heavy screen updates in memory-constrained environments.
