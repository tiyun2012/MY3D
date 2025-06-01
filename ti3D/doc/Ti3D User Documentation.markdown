# Ti3D Application Documentation

## Project Status (June 2025)

**Ti3D** is a modular C++ OpenGL application for 3D visualization and camera navigation, built with a custom math library (TiMath), GLAD for OpenGL loading, and GLFW for window/input management. The app features colored axes, a configurable XZ grid, and a debug point (TargetCamAim) that always faces the camera. The codebase is organized into Renderer, Camera, StateManager, and DebugPoint modules, with extensibility in mind.

### Current Features

- **Rendering:**  
  - 3D axes (X: red, Y: green, Z: blue), adjustable length.
  - XZ grid with customizable size, line count, and spacing.
  - Debug point (TargetCamAim) as a red circle, always camera-facing.
- **Camera System:**  
  - Perspective and orthographic projections.
  - Multiple view modes (Top, Left, Right, Bottom, Far).
  - Keyboard and mouse controls for zoom, pan, and view switching.
- **Input Handling:**  
  - Hotkey-driven controls (DCC mode with Spacebar modifier).
  - Context-aware input (Viewport mode via left-click).
- **Math Library:**  
  - Custom vector/matrix/quaternion operations for robust 3D math.
- **Architecture:**  
  - Modular: Renderer, Camera, StateManager, DebugPoint, TiMath.
  - Designed for extensibility and experimentation.

### Known Issues

- **GLAD/GLFW Include Order:**  
  - The project requires `<glad/glad.h>` to be included before any OpenGL or GLFW headers in all `.cpp` files.
  - Do **not** include `<glad/glad.h>` in header files; only include it in source files, and always before `<GLFW/glfw3.h>`.
  - Do **not** include `<GL/gl.h>` or `<GL/glu.h>` anywhere.
  - See the "Resolving C1189 Error" section below for details.

- **Build System:**  
  - Visual Studio project (`Ti3D.vcxproj`).
  - Requires GLAD and GLFW include/library paths set up correctly.

### Architecture Overview

- **Renderer:** Handles OpenGL setup and drawing of axes, grid, and debug point.
- **DebugPoint:** Renders a camera-facing circle for visual debugging.
- **Camera:** Manages view/projection matrices and user navigation.
- **StateManager:** Handles input context, hotkeys, and app mode.
- **TiMath:** Provides vector, matrix, and quaternion math utilities.

### Usage

- **Controls:**  
  - `Esc`: Exit
  - `P`: Toggle projection mode
  - `T/L/R/B/F`: Switch camera views
  - `=/-`: Zoom
  - `W/A/S/D`: Pan
  - `Space` + hotkeys: Toggle axes/grid, adjust grid, switch views
  - `Tab` + hotkeys: Switch app mode

- **Build:**  
  - Open `Ti3D.vcxproj` in Visual Studio.
  - Ensure GLAD and GLFW are included and linked.
  - Clean and rebuild if you encounter include order errors.

### Troubleshooting

- **C1189 Error (OpenGL header already included):**
  - Ensure `<glad/glad.h>` is included only in `.cpp` files, and always before `<GLFW/glfw3.h>`.
  - Remove any `<glad/glad.h>` from headers.
  - Remove any `<GL/gl.h>` or `<GL/glu.h>` includes.
  - Clean and rebuild the solution.

---

For more details, see the full documentation below.

---

# Ti3D Application Documentation

## Introduction

Ti3D is a 3D visualization and interaction application developed in C++ using OpenGL for rendering. It provides a lightweight environment for viewing a 3D coordinate system, including colored axes (X: red, Y: green, Z: blue), a customizable grid in the XZ plane, and a debug point (TargetCamAim) visualized as a red circle. The application supports two modes: DCC (Digital Content Creation) for scene manipulation and Engine (placeholder for future game-like functionality). Users can control the camera, toggle rendering options, and adjust the grid via hotkeys.
The primary goal of Ti3D is to serve as a foundation for 3D graphics experimentation, with a focus on camera navigation, rendering basic primitives, and extensible input handling.

## Features

- **Rendering:**  
  - Coordinate axes (X: red, Y: green, Z: blue) with configurable length (default: 2 units).
  - XZ grid with adjustable size (default: 20 units), number of lines (default: 10), and spacing (default: 2 units).
  - Debug point (TargetCamAim) rendered as a red circle, always facing the camera, with a radius of 6 units.
- **Camera System:**  
  - Supports Perspective and Orthographic projections.
  - Multiple view modes: Top, Left, Right, Bottom, and Far (default perspective view).
  - Camera controls: zoom (via +/- keys), pan (WASD or mouse drag), and focus on the debug point (F key in Viewport context).
  - Adjustable field of view, distance (1 to 1000 units), and aspect ratio.
- **Input Handling:**  
  - Hotkey-driven controls in DCC mode (with Spacebar as modifier):
    - Toggle axes (Space+A) and grid (Space+G).
    - Adjust grid size (Space+Q to increase, Space+E to decrease).
    - Adjust grid lines (Space+Z to increase, Space+Z to decrease).
    - Switch camera views (Space+1 to Space+6 for Perspective, Front, Left, Top, Back, Right).
  - Mode switching** (Tab+G for Engine, Tab+N for DCC).
  - Context-aware input (Viewport mode activated by left-click).
- **Math Library:**  
  - Custom TiMath library for vector (Vector2, Vector3, Vector4) operations, including dot/cross products, normalization, rotation, and projection.
  - Matrix operations (via Matrix4, assumed available) for transformations and camera matrices.
- **Extensibility:**  
  - Modular design with separate Renderer, Camera, and StateManager classes.
  - Placeholder for Engine mode to add game-like features in the future.

## Architecture

Ti3D is structured around the following components:

1. **Renderer (Renderer.h, Renderer.cpp)**

   - **Purpose:** Handles OpenGL rendering of axes, grid, and the debug point.
   - **Key Functions:**
     - `initialize()`: Sets up vertex arrays (VAOs), buffers (VBOs), and shaders for axes and grid.
     - `draw()`: Renders axes, grid, and TargetCamAim based on camera matrices and render flags.
     - `setRenderFlags(bool axes, bool grid)`: Toggles visibility of axes and grid.
     - `increaseGridSize(), decreaseGridSize(), increaseGridLines(), decreaseGridLines()`: Adjusts grid parameters.

   - **Details:**
     - Uses GLAD for OpenGL functions.
     - Axes are drawn as colored lines (6 vertices, 3 lines for X, Y, Z).
     - Grid is a set of XZ-plane lines, dynamically generated based on gridLines, gridSpacing, and gridSize.
     - TargetCamAim is a DebugPoint object, initialized at (0,0,0) with a radius of 0.5 units.

2. **DebugPoint (DebugPoint.h, DebugPoint.cpp)**

   - **Purpose:** Renders a 2D circle (red) that always faces the camera, used as TargetCamAim.
   - **Key Functions:**
     - `initialize()`: Creates a unit circle (32 segments) with vertex data for a triangle fan.
     - `render()`: Draws the circle using a model matrix to orient it toward the camera and position it.

   - **Details:**
     - Uses OpenGL shaders to render a red circle.
     - Camera-facing orientation computed using the inverse view matrix and cross products.

3. **Camera (Camera.h, Camera.cpp)**

   - **Purpose:** Manages the view and projection matrices for rendering.
   - **Key Functions:**
     - `processInput()`: Handles keyboard inputs for zooming, panning, and mode switching.
     - `processMouseInput()`: Supports mouse-based panning (left-click drag).
     - `getViewMatrix()`: Computes the view matrix based on view mode and target position.
     - `getProjectionMatrix()`: Returns perspective or orthographic projection matrix.
     - `focusOn()`: Sets the camera to focus on a point (e.g., TargetCamAim).

   - **Details:**
     - Supports Perspective (60° FOV) and Orthographic projections.
     - View modes adjust camera orientation (e.g., Top view looks down Y-axis).
     - Input is suppressed in DCC mode when Spacebar is pressed to prioritize hotkeys.

4. **StateManager (StateManager.h, StateManager.cpp)**

   - **Purpose:** Manages application state, input contexts, and hotkeys.
   - **Key Functions:**
     - `processHotkeys()`: Executes hotkey actions (e.g., toggle axes, switch modes) with a cooldown.
     - `updateMouseClickState()`: Sets Viewport context on left-click and handles focus (F key).
     - `updateCameraInput()`: Processes mouse and keyboard inputs for camera control.
     - `setMode()`: Switches between DCC and Engine modes.

   - **Details:**
     - DCC mode uses Spacebar-modified hotkeys for rendering and camera controls.
     - Engine mode is a placeholder with no hotkeys defined.
     - Contexts (Viewport, Timeline) determine input behavior.

5. **TiMath Library (Vector2.h, Vector2.cpp, Vector3.h, Vector3.cpp, Vector4.h, Vector4.cpp, TiMathConfig.h)**

   - **Purpose:** Provides mathematical utilities for 3D graphics.
   - **Components:**
     - `Vector2`: 2D vector with operations (dot, cross, normalize, rotate).
     - `Vector3`: 3D vector with operations (dot, cross, normalize, rotate, reflect, project).
     - `Vector4`: 4D vector with homogeneous coordinate support (e.g., homogeneousDivide()).
     - `TiMathConfig`: Defines constants (EPSILON, PI) and matrix order (TIMATH_COLUMN_MAJOR).

   - **Details:**
     - Optimized for robustness with epsilon-based checks for zero division and normalization.
     - Supports SSE (via USE_SSE define) for potential performance gains.
     - Used extensively in Camera, Renderer, and DebugPoint for transformations.

6. **Main Application (main.cpp)**

   - **Purpose:** Initializes GLFW, GLAD, and core components, and runs the main loop.
   - **Key Functions:**
     - Sets up a GLFW window (800x600, OpenGL 3.3 core profile).
     - Initializes CameraManager, Renderer, and StateManager.
     - Handles window resizing and mouse input via callbacks.
     - Runs the render loop, processing inputs and drawing the scene.

   - **Details:**
     - Enables depth testing for correct 3D rendering.
     - Clears the screen to black before each frame.

## Dependencies

- **External Libraries:**
  - GLAD: OpenGL loader (ThirdParty/gladLib/include/glad/glad.h).
  - GLFW: Window and input management (GLFW/glfw3.h).

- **Internal Libraries:**
  - TiMath: Custom math library for vectors and matrices.
  - CameraManager: Assumed header (CameraManager.h) for managing camera instances.

- **Standard C++ Libraries:**
  - `<iostream>`, `<vector>`, `<cmath>`, `<algorithm>`, `<functional>`, `<string>`, `<map>`.

## Controls

- **Keyboard**

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

## Build Instructions

- **Prerequisites**

  - **Compiler:** C++11 or later (e.g., MSVC, GCC, Clang).
  - **Libraries:**
    - GLAD: Include ThirdParty/gladLib/include in include paths.
    - GLFW: Link against GLFW library and include its headers.

  - **Build System:** Visual Studio with Ti3D.vcxproj.

- **Resolving C1189 Error**
  The C1189 error (OpenGL header already included) occurs due to include order conflicts between `<glad/glad.h>` and `<GLFW/glfw3.h>`. To resolve:

  - **Ensure Correct Include Order:**

    In .cpp files (main.cpp, Renderer.cpp, DebugPoint.cpp), include `<glad/glad.h>` before any headers that include `<GLFW/glfw3.h>` (e.g., Camera.h, StateManager.h).
    Example:
    ```cpp
    #include <glad/glad.h>
    #include "Renderer.h"
    #include <GLFW/glfw3.h>
    ```

  - **Remove `<glad/glad.h>` from Headers:**

    Renderer.h uses a type alias (using GLuint = unsigned int;) instead of including `<glad/glad.h>`.
    Avoid including `<glad/glad.h>` in other headers (Camera.h, StateManager.h, DebugPoint.h).

  - **Check Unshared Files:**

    Verify CameraManager.h does not include `<GL/gl.h>` or `<glad/glad.h>` unnecessarily.
    Example CameraManager.h:
    ```cpp
    #ifndef CAMERAMANAGER_H
    #define CAMERAMANAGER_H
    #include "Camera.h"
    class CameraManager {
    public:
        Camera& getActiveCamera();
        void switchTo(int view);
    private:
        Camera camera;
    };
    #endif
    ```

  - **Update Build Configuration:**

    In Ti3D.vcxproj, ensure include paths include ThirdParty/gladLib/include.
    Remove any forced includes of `<GL/gl.h>` (Properties > C/C++ > Advanced > Forced Include File).
    Link against GLFW and OpenGL libraries.

  - **Clean and Rebuild:**

    Delete the build/ directory.
    Rebuild the project with Ti3D.vcxproj.

- **Compilation**

  Use Visual Studio to open Ti3D.vcxproj.
  Set the configuration to Debug or Release.
  Build the solution, ensuring GLAD and GLFW are correctly linked.
  Run the executable to verify rendering and controls.

## Future Improvements

- Engine Mode: Implement hotkeys and functionality for Engine mode (e.g., game-like interactions).
- Scene Objects: Add support for rendering 3D models or additional primitives (e.g., cubes, spheres).
- UI: Integrate a graphical interface (e.g., ImGui) for settings and controls.
- Performance: Optimize rendering with instancing or batching for complex scenes.
- Error Handling: Enhance shader compilation error logs and add runtime OpenGL error checks.
- Documentation: Add inline comments and Doxygen tags for better code maintainability.
- CameraManager: Fully implement CameraManager to support multiple cameras and smooth transitions.

## Conclusion

Ti3D is a robust foundation for 3D graphics applications, with a clean separation of concerns between rendering, input handling, and math utilities. By resolving the C1189 error and expanding its features, Ti3D can serve as a versatile tool for 3D visualization and development. For further assistance, share CameraManager.h or build logs if compilation issues persist.
