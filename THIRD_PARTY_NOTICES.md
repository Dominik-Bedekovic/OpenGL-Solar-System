# Third-party notices

## Sun and planet textures — Solar System Scope

Creator: **Solar System Scope** (INOVE).
Source: https://www.solarsystemscope.com/textures/
License: **Creative Commons Attribution 4.0 International (CC BY 4.0)**.
License terms: https://creativecommons.org/licenses/by/4.0/
Legal code: https://creativecommons.org/licenses/by/4.0/legalcode.en

These texture files may be used, adapted and redistributed, including commercially,
subject to the license. Preserve appropriate credit, the source and license links,
and indicate modifications. No endorsement by the creator is implied. The license
provides no warranties. Do not impose additional legal or technological restrictions
that prevent recipients from exercising the licensed rights.

The files below were verified on 2026-09-28 by byte-for-byte comparison with the
Solar System Scope copies on Wikimedia Commons. Image bytes are unchanged.
Earth and Venus use shorter local filenames; only those filenames differ.
The source collection uses NASA imagery/elevation data, with enhanced colors and
fictional terrain in unmapped regions, as described by its creator.

| Local file | Published source / attribution record | SHA-256 of included file |
| --- | --- | --- |
| `2k_earth.jpg` | [Source](https://commons.wikimedia.org/wiki/File:Solarsystemscope_texture_2k_earth_daymap.jpg) | `767ee1dc6eb3802699bfccf6f264880f8acd0b80de3191cd24984fe279b07b7c` |
| `2k_jupiter.jpg` | [Source](https://commons.wikimedia.org/wiki/File:Solarsystemscope_texture_2k_jupiter.jpg) | `b0f04d005350252636b0e3396fc592548cbd9e9126b269d32d5c6abd4b0e4f2b` |
| `2k_mars.jpg` | [Source](https://commons.wikimedia.org/wiki/File:Solarsystemscope_texture_2k_mars.jpg) | `2d187f3e77a98eaa8cea5f4cc722f633c122ef170b9e94ace6b5fb6cbc3f8e01` |
| `2k_mercury.jpg` | [Source](https://commons.wikimedia.org/wiki/File:Solarsystemscope_texture_2k_mercury.jpg) | `5a5c80607f643496bac9a631e71957def35ed788895f18b678ac849c2b38e48a` |
| `2k_neptune.jpg` | [Source](https://commons.wikimedia.org/wiki/File:Solarsystemscope_texture_2k_neptune.jpg) | `cb42ea82709741d28b0af44d8b283cbc6dbd0c521a7f0e1e1e010ade00977df6` |
| `2k_saturn.jpg` | [Source](https://commons.wikimedia.org/wiki/File:Solarsystemscope_texture_2k_saturn.jpg) | `54a900ca9bf7ab62e70f862852759abdf342e6d6436a95a2fe9ebdb6bcd3bbac` |
| `2k_sun.jpg` | [Source](https://commons.wikimedia.org/wiki/File:Solarsystemscope_texture_2k_sun.jpg) | `ff0f076ba65e03b5ab518451bc96699325be38e3ccbdd5869ee1c00f3a0c8816` |
| `2k_uranus.jpg` | [Source](https://commons.wikimedia.org/wiki/File:Solarsystemscope_texture_2k_uranus.jpg) | `d15239d46f82d3ea13d2b260b5b29b2a382f42f2916dae0694d0387b1204a09d` |
| `2k_venus.jpg` | [Source](https://commons.wikimedia.org/wiki/File:Solarsystemscope_texture_2k_venus_surface.jpg) | `dbe5db1c794a8ab4cbf7dd6bf193540c400fc833ce1e6cc399318aa68026278b` |

## Saturn rings

The previous `saturn_ring.jpg` had no verified provenance and has been removed.
The annulus geometry and procedural bands in `shaders/frag.glsl` are generated
by application code and do not use or derive from that image.

## Bundled software

| Component | Upstream | License notice included |
| --- | --- | --- |
| GLFW 3.3.2 | https://www.glfw.org/ | `third_party/glfw/LICENSE.md`; `licenses/GLFW.txt` (zlib/libpng) |
| GLEW | https://glew.sourceforge.net/ | `third_party/glew/LICENSE.txt`; `licenses/GLEW.txt` (all upstream notices retained) |
| GLM | https://github.com/g-truc/glm | `third_party/glm/copying.txt`; `licenses/GLM.txt` (MIT or Modified MIT) |
| stb_image | https://github.com/nothings/stb | License at the end of `third_party/stb/stb_image.h`; `licenses/stb_image.txt` (MIT or public-domain alternative) |

Retain the applicable dependency notices with source or binary distributions.
CMake copies this file and `licenses/` beside the executable and into installed
packages. Upstream source notices remain in the vendored code.

## Application code

The asset and dependency licenses above apply only to their respective materials.
No new blanket license is assigned to the project's own application code by this notice.
