# Validation — 2026-09-28

- Configured and built the complete application and bundled GLFW/GLEW with CMake 4.4 and GCC 13.3 on Linux, Release configuration.
- Compiled both GLSL 330 shaders and linked their program successfully in a headless OpenGL context.
- Verified all nine included JPEGs decode as RGB at 2048 x 1024.
- Compared all nine images byte-for-byte against Solar System Scope copies published on Wikimedia Commons; all matched. The provenance and hashes are in THIRD_PARTY_NOTICES.md.
- Checked the installation package contains the executable, shaders, textures, attribution, and dependency notices.

A complete interactive window/rendering test was not possible: the environment cannot start the X11 display server. Windows and macOS builds have not been executed here.

On a desktop, build with the README commands, launch the application, resize the window, use WASD/mouse/scroll, and inspect Saturn's rings and orbit lines. Run `solar_system --smoke-test` for a three-frame startup/error check.

## Changes

- CMake 4 compatibility for bundled GLFW.
- Assets/shaders and notices packaged beside the executable; executable-relative resource lookup and an installation target.
- Framebuffer-size viewport updates and dynamic aspect ratio, including minimized-window handling.
- Removed time-driven scene drift.
- Replaced unverified ring JPEG with procedural annulus geometry and shader bands; orbits use an explicit solid-color shader mode.
- Fail-fast initialization, shader and texture errors; automatic texture, shader and sphere cleanup while the context is alive.
- Removed reliance on nonstandard M_PI and prevented Windows min/max macro conflicts.
- Correct texture row alignment and explicitly requested RGB/RGBA image channels.
