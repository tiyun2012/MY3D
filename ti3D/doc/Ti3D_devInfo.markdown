# Ti3D Application Documentation
<!-- notice for debugging  -->
- **GLAD/GLFW Include Order:**  
  - The project requires `<glad/glad.h>` to be included before any OpenGL or GLFW headers in all `.cpp` files.
  - Do **not** include `<glad/glad.h>` in header files; only include it in source files, and always before `<GLFW/glfw3.h>`.
  - Do **not** include `<GL/gl.h>` or `<GL/glu.h>` anywhere.
  - See the "Resolving C1189 Error" section below for details.


## Overview

**Ti3D** is a modular 3D engine and application framework for interactive visualization, editing, and debugging of 3D scenes. It features a flexible camera system, customizable grid/axes rendering, and supports both DCC (Digital Content Creation) and Engine modes for different workflows.

### Usage

- **Controls:**  
  - `Esc`: Exit
  - `P`: Toggle projection mode
  - `T/L/R/B/F`: Switch camera views
  - `=/-`: Zoom
  - `W/A/S/D`: Pan
  - `Space` + hotkeys: Toggle axes/grid, adjust grid, switch views
  - `Tab` + hotkeys: Switch app mode
  - **General:**
    - `Esc`: Close the application.
    - `P`: Toggle between Perspective and Orthographic projections.
    - `T`: Switch to Top view.
    - `L`: Switch to Left view.
    - `R`: Switch to Right view.
    - `B`: Switch to Bottom view.
    - `F`: Switch to Far view or focus on TargetCamAim (in Viewport context).
    - `=`, `+`: Zoom in.
    - `-`, `_`: Zoom out.
    - `W`, `A`, `S`, `D`: Pan camera (direction depends on view mode).

  - **DCC Mode Hotkeys (with Space held):**
    - `A`: Toggle axes visibility.
    - `G`: Toggle grid visibility.
    - `Q`: Increase grid size by 1 unit.
    - `E`: Decrease grid size by 1 unit.
    - `Z`: Increase grid lines by 1.
    - `X`: Decrease grid lines by 1.
    - `1` to `6`: Switch camera to Perspective, Front, Left, Top, Back, or Right view.

  - **Mode Switching (with Tab held):**
    - `G`: Switch to Engine mode.
    - `N`: Switch to DCC mode.

- **Mouse**

  - `Left-click`: Set Viewport context.
  - `Left-click + drag`: Pan the camera (scaled by distance).


## Application Hierarchy

- **app/**
  - `main.cpp`: Application entry point, main loop, and mode/context switching.
  - `StateManager`: Handles global state and mode (DCC/Engine).
- **camera/**
  - `Camera`: Handles view/projection, input, and camera modes.
  - `CameraManager`: Manages multiple camera presets/views.
- **renderer/**
  - `Renderer`: Draws axes, grid, and debug points.
  - `DebugPoint`: Renders target/aim points for camera focus.
- **TiMath/**
  - Math utilities: `Vector2/3/4`, `Matrix4`, `Quaternion`.


## Features

- Multiple camera views (Perspective, Top, Left, Right, Bottom, Far)
- Toggle between Perspective and Orthographic projections
- Camera orbit, pan, and zoom (keyboard and mouse)
- DCC and Engine modes with context-sensitive controls
- Customizable grid and axes (visibility, size, lines)
- Debug point rendering for camera target/aim
- Modular math library for 3D operations
- OpenGL rendering with GLAD/GLFW backend


## How to Use

1. **Build and Run**
   - Use CMake to configure and build the project.
   - Run `Ti3D.exe` from the build output directory.

2. **Controls**
   - See the "Controls" section below for all hotkeys and mouse actions.
   - Switch between DCC and Engine modes using `Tab + G` or `Tab + N`.
   - Use the mouse and keyboard to navigate and manipulate the camera and scene.

3. **Typical Workflow**
   - Start in DCC mode for scene editing (precise camera, grid/axes toggling).
   - Switch to Engine mode for runtime/interactive testing.
   - Use debug points to focus or aim the camera.

## Debugging Tips & Common Issues

- **GLAD/GLFW Include Order**
  - Always include `<glad/glad.h>` before any OpenGL/GLFW headers in `.cpp` files.
  - Never include `<glad/glad.h>` in header files.
  - Do not include `<GL/gl.h>` or `<GL/glu.h>` anywhere.
  - See the "Resolving C1189 Error" section for details.

- **Shader Compilation**
  - Check the console for shader compilation/linking errors.
  - Use the debug output in `Renderer.cpp` and `DebugPoint.cpp` for details.

- **Camera Controls**
  - If camera movement is unresponsive, check if DCC mode and Spacebar are suppressing input.
  - Ensure the correct view mode is active for expected controls.

- **Grid/Axes Rendering**
  - If grid or axes are not visible, verify the render flags and OpenGL state.
  - Use DCC hotkeys (`Space + A/G`) to toggle visibility.

- **Math Errors**
  - Watch for warnings about zero-length vectors or singular matrices in the console.
  - Use debug output in math classes to trace invalid operations.

- **General**
  - Use verbose console output for debugging input, camera, and rendering state.
  - Check for OpenGL errors if rendering artifacts appear.

