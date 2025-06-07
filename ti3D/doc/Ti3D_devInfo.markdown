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

- 


## Application Hierarchy

- **app/**
  - `main.cpp`: Application entry point, main loop, and mode/context switching.
  - `StateManager`: Handles global state and mode (DCC/Engine).
- **camera/**
  - `Camera`: Handles view/projection, input, and camera modes.
- **renderer/**
  - `Renderer`: Draws axes, grid, and debug points.
  - `DebugPoint`: Renders target/aim points for camera focus.
- **TiMath/**
  - Math utilities: `Vector2/3/4`, `Matrix4`, `Quaternion`.


## Features

-


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

