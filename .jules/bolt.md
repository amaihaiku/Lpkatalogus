## 2024-03-08 - Rendering Loop Optimization
**Learning:** ESP32 performance can be heavily impacted by repetitive calculations and array indexing within tight inner loops like video frame rendering.
**Action:** When working on tight loops, use pointer arithmetic and hoist invariant calculations (such as `scan` bases and `pixelAddress` calculations) out of the inner loops to minimize ALU cycles per pixel block. Ensure the changes don't break logic and always document them with comments as requested.
