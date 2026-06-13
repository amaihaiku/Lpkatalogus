## 2023-10-27 - [Optimize Pixel Rendering Loop]
**Learning:** [The ZX Spectrum emulator screen rendering loops are highly CPU bound. The original code in `Renderer::drawSpectrumScreen` recalculated character block base offsets for every single one of the 8 pixel rows. Pre-calculating this outside the loop saves thousands of bitwise operations per frame.]
**Action:** [Look for loop invariants and complex math inside the deepest tight loops of rendering and emulation paths, hoisting them where possible.]
