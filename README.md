# Tetris Clone
A Tetris implementation built from scratch in C++ and OpenGL — no engine.
Every system — piece logic, collision, rendering, text — is written manually against
raw OpenGL. The renderer maintains a CPU-side framebuffer that is uploaded to
a texture each frame and drawn via custom GLSL shaders onto a fullscreen quad.

## Technical details
- **Language:** C++20
- **Graphics:** OpenGL (no engine, no framework)
- **Shaders:** Custom GLSL vertex and fragment shaders
- **Windowing / Input:** GLFW
- **Math:** GLM
- **Font rendering:** stb_truetype (CPU-side bitmap baking)
- **Build system:** CMake + vcpkg

## What's implemented
- All 7 tetrominoes (I, O, T, S, Z, J, L) with distinct colours
- Gravity-based falling with delta-time integration
- Piece locking on floor or stack contact
- Clockwise and counter-clockwise rotation with collision undo
- Horizontal movement with wall and stack collision
- DAS (Delayed Auto Shift) — hold to move with configurable delay and repeat rate
- Line clearing with gravity shift and multi-line scoring
- Next piece preview
- Score tracking with Tetris bonus (1/2/3/4 lines = 100/300/500/800)
- Level system — speed increases every 30 seconds
- Game over detection with restart (R)
- CPU framebuffer renderer — `SetPixel` / `DrawRect` uploaded via `glTexSubImage2D`
- Random piece spawning using a seeded Mersenne Twister

## Controls
| Key | Action |
|---|---|
| `←` / `A` | Move left |
| `→` / `D` | Move right |
| `↑` / `W` | Rotate clockwise |
| `↓` / `S` | Rotate counter-clockwise |
| `Space` | Fast drop |
| `R` | Restart (on game over) |
| `Escape` | Quit |

## Building
Requires OpenGL and CMake. Clone the repo and open `CMakeLists.txt` in Visual Studio
or build via CMake from the command line. Dependencies managed via vcpkg (`vcpkg.json` included).

## About
Built by [Austin Espinosa](https://foxglowgames.com) — indie developer and
Information Science student at ICU Tokyo.
