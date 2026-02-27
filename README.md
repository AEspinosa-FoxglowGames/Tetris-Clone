# Tetris Clone
A Tetris implementation built from scratch in C++ and OpenGL — no engine.
Every system — piece logic, collision, rendering — is written manually against
raw OpenGL. The renderer maintains a CPU-side framebuffer that is uploaded to
a texture each frame and drawn via custom GLSL shaders onto a fullscreen quad.

## Status
🚧 In development — pieces fall, lock, and stack. Rotation and line clearing not yet implemented.

## Technical details
- **Language:** C++
- **Graphics:** OpenGL (no engine, no framework)
- **Shaders:** Custom GLSL vertex and fragment shaders
- **Windowing / Input:** GLFW
- **Math:** GLM
- **Build system:** CMake + vcpkg

## What's implemented
- All 7 tetrominoes (I, O, T, S, Z, J, L) with distinct colours
- Gravity-based falling with delta-time integration
- Piece locking on floor or stack contact
- Horizontal movement with wall and stack collision
- CPU framebuffer renderer — `SetPixel` / `DrawRect` uploaded via `glTexSubImage2D`
- Random piece spawning using a seeded Mersenne Twister

## Controls
| Key | Action |
|---|---|
| `←` / `→` | Move piece left / right |
| `Space` | Fast drop |
| `Escape` | Quit |

## Building
Requires OpenGL and CMake. Clone the repo and open `CMakeLists.txt` in Visual Studio
or build via CMake from the command line. Dependencies managed via vcpkg (`vcpkg.json` included).

## About
Built by [Austin Espinosa](https://foxglowgames.com) — indie developer and
Information Science student at ICU Tokyo.
