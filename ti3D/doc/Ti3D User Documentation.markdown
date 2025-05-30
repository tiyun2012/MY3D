# Ti3D User Documentation

## Overview
Ti3D is a 3D visualization application built with OpenGL, GLFW, and GLAD. It features a camera system for viewing a 10x10 unit grid plane (in the XZ plane at Y=0) and colored coordinate axes (red X, green Y, blue Z) from multiple perspectives. The camera supports top, left, right, bottom, and far views, with zoom and pan functionality, centered on a movable target point.

This document explains how to build, run, and use the Ti3D application, including system requirements, controls, and troubleshooting steps.

## System Requirements
- **Operating System**: Windows, Linux, or macOS
- **Compiler**: C++17 compatible (e.g., MSVC, GCC, Clang)
- **Dependencies**:
  - CMake 3.10 or higher
  - GLFW 3.4 (included in `ThirdParty/glfw-3.4/`)
  - GLAD (included in `ThirdParty/gladLib/`)
- **Development Environment**: Visual Studio Code (recommended) with CMake Tools extension
- **Hardware**: OpenGL 3.3 compatible GPU

## Project Structure
The Ti3D project is organized as follows:
```
Ti3D/
├── .vscode/
│   ├── tasks.json        # VS Code build and run tasks
├── app/
│   ├── main.cpp         # Application entry point
├── camera/
│   ├── Camera.h         # Camera class definition
├── renderer/
│   ├── Renderer.h       # Renderer class for grid and axes
│   ├── Renderer.cpp     # Renderer implementation (empty)
├── TiMath/
│   ├── Vector3.h/cpp    # 3D vector math
│   ├── Quaternion.h/cpp # Quaternion math
│   ├── Matrix4.h/cpp    # 4x4 matrix math
├── ThirdParty/
│   ├── glfw-3.4/        # GLFW library
│   ├── gladLib/         # GLAD library
├── CMakeLists.txt       # CMake build configuration
```

## Building the Application
Follow these steps to build Ti3D using Visual Studio Code:

1. **Clone or Extract the Project**:
   - Ensure the project is in a directory (e.g., `C:/Users/pc/Desktop/Dev/ti3D/`).
   - Verify that `ThirdParty/glfw-3.4/` and `ThirdParty/gladLib/` are present.

2. **Open in VS Code**:
   - Launch VS Code and open the `Ti3D/` folder (`File > Open Folder`).

3. **Install CMake Tools Extension**:
   - Go to the Extensions view (`Ctrl+Shift+X`) and install "CMake Tools" by Microsoft.

4. **Configure the Build**:
   - Open the Command Palette (`Ctrl+Shift+P`).
   - Run `Tasks: Run Task` and select `CMake: Configure`.
   - This creates a `build/` directory and configures the project using `CMakeLists.txt`.

5. **Build the Project**:
   - Run `Tasks: Run Task` and select `CMake: Build` (or press `Ctrl+Shift+B`).
   - The build output is placed in `build/bin/` (e.g., `build/bin/Ti3D.exe` on Windows).

6. **Troubleshooting Build Issues**:
   - Ensure GLFW and GLAD paths in `CMakeLists.txt` match your system:
     ```cmake
     set(GLFW3_DIR "C:/Users/pc/Desktop/Dev/ti3D/ThirdParty/glfw-3.4")
     set(GLAD_DIR "C:/Users/pc/Desktop/Dev/ti3D/ThirdParty/gladLib")
     ```
   - Check the Terminal panel for errors (e.g., missing libraries).
   - If using a non-MSVC compiler, ensure `glfw` is installed (e.g., `sudo apt-get install libglfw3-dev` on Ubuntu).

## Running the Application
1. **Run via VS Code**:
   - Run `Tasks: Run Task` and select `Run Ti3D`.
   - Alternatively, try `Run Ti3D (Debug Fallback)` if the executable is in `build/Debug/`.

2. **Run Manually**:
   - Navigate to `build/bin/` in a terminal.
   - Run `./Ti3D.exe` (Windows) or `./Ti3D` (Linux/macOS).

3. **Troubleshooting Run Issues**:
   - **Executable Not Found**:
     - Run `Tasks: Run Task` > `Check Executable Path` to list `build/bin/` and `build/Debug/` contents.
     - If `Ti3D.exe` is in `build/Debug/`, use the `Run Ti3D (Debug Fallback)` task.
     - If elsewhere, update `.vscode/tasks.json`:
       ```json
       "command": "${workspaceFolder}/build/[correct_path]/Ti3D${command:cmake.buildProgramExtension}",
       "options": { "cwd": "${workspaceFolder}/build/[correct_path]" }
       ```
   - **Permissions** (Linux/macOS):
     - Run `chmod +x build/bin/Ti3D` or `chmod +x build/Debug/Ti3D`.
   - **Dependencies**:
     - Windows: Ensure GLFW DLLs (e.g., `glfw3.dll`) are in `build/bin/` or PATH.
     - Linux/macOS: Run `ldd build/bin/Ti3D` to check for missing libraries.
   - Check the Terminal for errors (e.g., "file not found") and share them for support.

## Using the Application
Ti3D displays a 10x10 unit grid plane (gray, in the XZ plane at Y=0) and coordinate axes (red X, green Y, blue Z) centered on a movable target point. The camera can switch between five views (Top, Left, Right, Bottom, Far), zoom in/out, and pan the target.

### Controls
- **View Mode Switching**:
  - `T`: Switch to Top view (camera above, looking down Y-axis).
  - `L`: Switch to Left view (camera left, looking along X-axis).
  - `R`: Switch to Right view (camera right, looking along -X-axis).
  - `B`: Switch to Bottom view (camera below, looking up Y-axis).
  - `F`: Switch to Far view (camera far, looking along Z-axis).
- **Zoom**:
  - `+` or `Keypad +`: Zoom in (reduce distance, min 1.0 unit).
  - `-` or `Keypad -`: Zoom out (increase distance).
- **Pan**:
  - `W`: Move target forward (negative Z).
  - `S`: Move target backward (positive Z).
  - `A`: Move target left (negative X).
  - `D`: Move target right (positive X).
- **Exit**:
  - `Esc`: Close the application.

### Camera Behavior
- **Views**: The camera is positioned 10 units (default) from the target along the respective axis (e.g., Y=+10 for Top, X=-10 for Left). Each view is orthogonal, with no rotation.
- **Zoom**: Adjusts the distance from the target (affects all views).
- **Pan**: Moves the target in the XZ plane, keeping the grid and axes centered on it.
- **Default View**: Top view on startup.

### Visual Elements
- **Grid Plane**: 10x10 units, gray, in the XZ plane (Y=0), with 1-unit spacing.
- **Axes**:
  - Red: Positive X-axis.
  - Green: Positive Y-axis.
  - Blue: Positive Z-axis.
- **Background**: Black (RGB: 0, 0, 0).

## Troubleshooting
- **Application Doesn’t Start**:
  - Verify the executable exists using the `Check Executable Path` task.
  - Ensure GLFW and GLAD are correctly set up in `CMakeLists.txt`.
  - Check for missing DLLs or libraries.
- **Controls Don’t Work**:
  - Ensure the window is focused.
  - Verify key bindings in `Camera::processInput` in `camera/Camera.h`.
- **Grid/Axes Not Visible**:
  - Zoom in/out (`+/-`) to adjust the camera distance.
  - Switch views (`T`, `L`, `R`, `B`, `F`) to ensure the target is in view.
  - Check `renderer/Renderer.h` for correct rendering logic.

## Extending the Application
To add features, modify the following files:
- **New Camera Controls**: Edit `Camera::processInput` in `camera/Camera.h`.
- **Toggle Grid**: Add a boolean flag to `Renderer` and a key (e.g., `G`) in `app/main.cpp`.
- **Add Objects**: Extend `Renderer::drawAxes` in `renderer/Renderer.h` to render new geometry.
- **Debugging**: Create `.vscode/launch.json` for VS Code debugging (contact the developer for a sample).

## Contact
For issues or feature requests, contact the developer with:
- Operating system (Windows, Linux, macOS).
- Error messages from the Terminal.
- Output of the `Check Executable Path` task.
- Description of the problem or desired feature.

*Last Updated: May 30, 2025*

Using the Application (Updated)
Ti3D displays a 10x10 unit grid plane (gray, in the XZ plane at Y=0) and coordinate axes (red X, green Y, blue Z) centered on a movable target point. The camera uses an orthographic projection for top, left, right, bottom, and far views, implemented via TiMath::Matrix4::orthographic, ensuring no perspective distortion. You can switch views, zoom, and pan to explore the scene.
Controls

View Mode Switching:
T: Top view (above, looking down Y-axis).
L: Left view (left, looking along X-axis).
R: Right view (right, looking along -X-axis).
B: Bottom view (below, looking up Y-axis).
F: Far view (far, looking along Z-axis).


Zoom:
+ or Keypad +: Zoom in (reduce view size, min distance 1.0 unit).
- or Keypad -: Zoom out (increase view size).


Pan:
W: Move target forward (negative Z).
S: Move target backward (positive Z).
A: Move target left (negative X).
D: Move target right (positive X).


Exit:
Esc: Close the application.



Camera Behavior

Orthographic Views: Each view uses an orthographic projection, implemented in TiMath::Matrix4::orthographic. The view spans ±distance units vertically and ±distance * aspectRatio horizontally (default: ±10 vertically, ±13.33 horizontally at 4:3), showing the 10x10 grid plane without distortion.
Zoom: Adjusts distance. Smaller values zoom in, larger values zoom out.
Pan: Moves target in the XZ plane, keeping the grid and axes centered.
Default View: Top view, with the grid plane fully visible.

Last Updated: May 30, 2025

Ti3D User Documentation
Overview
Ti3D is a 3D visualization tool built with C++, OpenGL 3.3, GLFW, and GLAD, displaying a 10x10 unit gray grid plane (XZ plane, Y=0) and coordinate axes (red X, green Y, blue Z) centered on a movable target. It offers perspective and orthographic views with Maya-like controls.
Last Updated: May 30, 2025
Features

Scene: 10x10 unit grid plane (XZ, Y=0), gray; axes: red (X), green (Y), blue (Z).
Camera Views: Perspective (60° FOV), Orthographic (Top, Left, Right, Bottom, Far).
Controls: Tumble (Alt + LMB), track (Alt + MMB), dolly (Alt + RMB, wheel); keys: P, T, L, R, B, F, Alt + F, WASD, +/-.
Platform: Windows (MSVC 2022); macOS/Linux possible.
Build: CMake, VS Code tasks.

System Requirements

OS: Windows 10/11.
Compiler: MSVC 2022, C++17.
Dependencies: GLFW 3.4, GLAD (included).
Tools: CMake 3.10+, VS Code (optional).
Hardware: OpenGL 3.3 GPU.

Installation and Setup

Clone/Extract: C:/Users/pc/Desktop/Dev/ti3D.
Dependencies: GLFW (ThirdParty/glfw3.4-3.4), GLAD (ThirdParty/gladLib).
CMake:set(CMAKE_CXX_STANDARD 17)
set(GLFW3_DIR "C:/Users/pc/Desktop/Dev/ti3D/ThirdParty/glfw3.4-3.4")
set(GLAD_DIR "C:/Users/pc/Desktop/Dev/ti3D/ThirdParty/gladLib")



Building

VS Code:
Tasks: Run Task > CMake: Configure.
Tasks: Run Build Task (Ctrl+Shift+B).
Output: build/bin/Debug/Ti3D.exe or build/Debug/Ti3D.exe.


Manual:mkdir build
cd build
cmake -S .. -B .
cmake --build . --config Debug



Running

VS Code: Tasks: Run Task > Run Ti3D or Run Ti3D (Debug Fallback).
Manual: Run build/bin/Ti3D.exe or build/Debug/Ti3D.exe with glfw3.dll.
Initial View: Perspective, grid visible, ~45° yaw/pitch.

Controls

View Switching: P (Perspective), T (Top), L (Left), R (Right), B (Bottom), F (Far).
Mouse (with Alt):
Alt + LMB: Tumble (perspective, smooth rotation around (0,0,0)).
Alt + MMB: Track.
Alt + RMB: Dolly.
Wheel: Dolly.


Keyboard: Alt + F (Frame), WASD (Pan), +/- (Zoom), Esc (Exit).

Camera Behavior

Perspective: 60° FOV, tumbles around (0,0,0) with slerp.
Orthographic: ±distance height, ±distance*aspectRatio width; no tumbling.
Tumbling: Alt + LMB, smooth, additive, cursor-driven.

Troubleshooting

Build:
Quaternion::identity: Ensure Quaternion.h has static constexpr Quaternion identity().
Quaternion::to_mat4: Not currently implemented; placeholder in Quaternion.h for future use (e.g., 3D model rendering).
Verify CMakeLists.txt includes Quaternion.cpp, Matrix4.cpp.
Clear IntelliSense: Ctrl+Shift+P, “C/C++: Reset IntelliSense Database”.


Camera:
Tumbling: Use perspective (P), cursor on grid; adjust rotateSpeed (2.0f).
Raycasting: Debug getCursorWorldPoint with std::cout.


Runtime:
Check Ti3D.exe path: Tasks: Run Task > Check Executable Path.
Ensure glfw3.dll.


Share: OS, compiler, Quaternion.h, logs, screenshots.

Version History

May 30, 2025:
Removed: Quaternion::toMatrix (not needed currently); added placeholder for to_mat4 (GLM-style) for future use.
Fixed: Quaternion::identity() for Camera.h (IntelliSense error 980).
Fixed: Camera.h syntax in getViewMatrix, getCursorWorldPoint, slerp.
Added: Matrix4 16-float constructor, Quaternion::rotateVector.


Previous: Initial orthographic camera, WASD, +/- zoom.

Future Enhancements

Dynamic placeA.
Grid toggle.
Wireframe/shaded modes.
3D model loading (may require Quaternion::to_mat4).
Cross-platform support.

Contact
Provide: version, OS, compiler, steps, logs, screenshots.

