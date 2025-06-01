#include "StateManager.h"
#include "Renderer.h"
#include "CameraManager.h"
#include <GLFW/glfw3.h> // Added for GLFW constants and types
#include <iostream>

namespace Ti3D {

const std::map<int, std::string> StateManager::specialKeyNames = {
    { GLFW_KEY_SPACE, "Space" },
    { GLFW_KEY_TAB, "Tab" },
    { GLFW_KEY_UP, "Up" },
    { GLFW_KEY_DOWN, "Down" },
    { GLFW_KEY_LEFT, "Left" },
    { GLFW_KEY_RIGHT, "Right" },
    { GLFW_KEY_W, "W" },
    { GLFW_KEY_A, "A" },
    { GLFW_KEY_S, "S" },
    { GLFW_KEY_D, "D" },
    { GLFW_KEY_G, "G" },
    { GLFW_KEY_N, "N" },
    { GLFW_KEY_Q, "Q" },
    { GLFW_KEY_E, "E" },
    { GLFW_KEY_Z, "Z" },
    { GLFW_KEY_X, "X" }
};

StateManager::StateManager()
    : currentMode(AppMode::DCC), spacebarPressed(false), hotkeyCooldown(0.0f), 
      modeCooldown(0.0f), currentContext(ContextType::None), cameraUpdate(false) {
    // DCC Mode hotkeys
    dccHotkeys = {
        { GLFW_KEY_A, "Space + A: Toggle axes visibility",
          [](Renderer& r) { r.setRenderFlags(!r.getRenderAxes(), r.getRenderGrid()); },
          GLFW_KEY_SPACE },
        { GLFW_KEY_G, "Space + G: Toggle grid visibility",
          [](Renderer& r) { r.setRenderFlags(r.getRenderAxes(), !r.getRenderGrid()); },
          GLFW_KEY_SPACE },
        { GLFW_KEY_Q, "Space + Q: Increase grid size",
          [](Renderer& r) { r.increaseGridSize(1.0f); },
          GLFW_KEY_SPACE },
        { GLFW_KEY_E, "Space + E: Decrease grid size",
          [](Renderer& r) { r.decreaseGridSize(1.0f); },
          GLFW_KEY_SPACE },
        { GLFW_KEY_Z, "Space + Z: Increase grid lines",
          [](Renderer& r) { r.increaseGridLines(1); },
          GLFW_KEY_SPACE },
        { GLFW_KEY_X, "Space + X: Decrease grid lines",
          [](Renderer& r) { r.decreaseGridLines(1); },
          GLFW_KEY_SPACE },
        // Camera view hotkeys
        { GLFW_KEY_1, "Space + 1: Perspective View",
          [this](Renderer&) { if (cameraManager) cameraManager->switchTo(CameraViewType::Perspective); },
          GLFW_KEY_SPACE },
        { GLFW_KEY_2, "Space + 2: Front View",
          [this](Renderer&) { if (cameraManager) cameraManager->switchTo(CameraViewType::Front); },
          GLFW_KEY_SPACE },
        { GLFW_KEY_3, "Space + 3: Left View",
          [this](Renderer&) { if (cameraManager) cameraManager->switchTo(CameraViewType::Left); },
          GLFW_KEY_SPACE },
        { GLFW_KEY_4, "Space + 4: Top View",
          [this](Renderer&) { if (cameraManager) cameraManager->switchTo(CameraViewType::Top); },
          GLFW_KEY_SPACE },
        { GLFW_KEY_5, "Space + 5: Back View",
          [this](Renderer&) { if (cameraManager) cameraManager->switchTo(CameraViewType::Back); },
          GLFW_KEY_SPACE },
        { GLFW_KEY_6, "Space + 6: Right View",
          [this](Renderer&) { if (cameraManager) cameraManager->switchTo(CameraViewType::Right); },
          GLFW_KEY_SPACE }
    };

    // Engine Mode hotkeys (placeholder)
    engineHotkeys = {};

    // Mode switching hotkeys
    modeHotkeys = {
        { GLFW_KEY_G, "Tab + G: Switch to Engine Mode",
          [this](Renderer&) { setMode(AppMode::Engine); },
          GLFW_KEY_TAB },
        { GLFW_KEY_N, "Tab + N: Switch to DCC Mode",
          [this](Renderer&) { setMode(AppMode::DCC); },
          GLFW_KEY_TAB }
    };

    printControls();
}

void StateManager::setMode(AppMode mode) {
    if (currentMode != mode) {
        currentMode = mode;
        printControls();
    }
}

void StateManager::updateHotkeyStates(GLFWwindow* window, float deltaTime) {
    spacebarPressed = (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS);
    hotkeyCooldown -= deltaTime;
    modeCooldown -= deltaTime;
}

void StateManager::processHotkeys(GLFWwindow* window, Renderer& renderer) {
    // Process mode switching
    if (modeCooldown <= 0.0f && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
        for (const auto& hotkey : modeHotkeys) {
            if (glfwGetKey(window, hotkey.key) == GLFW_PRESS) {
                hotkey.action(renderer);
                modeCooldown = 0.2f;
                std::cout << "Hotkey triggered: " << hotkey.description << "\n";
                return;
            }
        }
    }

    // Process mode-specific hotkeys
    if (hotkeyCooldown <= 0.0f && currentMode == AppMode::DCC && spacebarPressed) {
        for (const auto& hotkey : dccHotkeys) {
            if (glfwGetKey(window, hotkey.key) == GLFW_PRESS) {
                hotkey.action(renderer);
                hotkeyCooldown = 0.2f;
                std::cout << "Hotkey triggered: " << hotkey.description << "\n";
                break;
            }
        }
    }
}

void StateManager::updateMouseClickState(GLFWwindow* window) {
    static bool lastLeftClickState = false;
    bool currentLeftClickState = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);

    if (currentLeftClickState && !lastLeftClickState) {
        // Left click pressed: set to Viewport context
        currentContext = ContextType::Viewport;
        std::cout << "Context set to Viewport (1)\n";
    } else if (!currentLeftClickState && lastLeftClickState) {
        // Left click released: no change to context
    }

    lastLeftClickState = currentLeftClickState;

    // Check movement keys and print actions if in Viewport context
    if (currentContext == ContextType::Viewport) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            std::cout << "Camera moving forward\n";
            cameraUpdate = true;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            std::cout << "Camera moving backward\n";
            cameraUpdate = true;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            std::cout << "Camera moving left\n";
            cameraUpdate = true;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            std::cout << "Camera moving right\n";
            cameraUpdate = true;
        }
    }
}

void StateManager::updateCameraInput(GLFWwindow* window, double xpos, double ypos) {
    static double lastX = xpos, lastY = ypos;
    static bool lastPState = false;
    static bool lastZoomState = false;

    // View mode switches
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        cameraUpdate = true;
    }

    // Projection toggle
    bool currentPState = (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS);
    if (currentPState && !lastPState) {
        cameraUpdate = true;
    }
    lastPState = currentPState;

    // Zoom
    bool currentZoomState = (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS ||
                             glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS ||
                             glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS ||
                             glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS);
    if (currentZoomState && !lastZoomState) {
        cameraUpdate = true;
    }
    lastZoomState = currentZoomState;

    // Mouse panning
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        float deltaX = static_cast<float>(xpos - lastX);
        float deltaY = static_cast<float>(ypos - lastY);
        if (deltaX != 0.0f || deltaY != 0.0f) {
            cameraUpdate = true;
        }
    }
    lastX = xpos;
    lastY = ypos;
}

void StateManager::printControls() const {
    std::cout << "Current Mode: " << (currentMode == AppMode::DCC ? "DCC" : "Engine") << "\n";
    std::cout << "Controls:\n";
    const auto& hotkeys = (currentMode == AppMode::DCC) ? dccHotkeys : engineHotkeys;
    for (const auto& hotkey : hotkeys) {
        std::cout << hotkey.description << "\n";
    }
    std::cout << "W: Move camera forward\n";
    std::cout << "S: Move camera backward\n";
    std::cout << "A: Move camera left\n";
    std::cout << "D: Move camera right\n";
    std::cout << "Up: Camera control (context-dependent)\n";
    std::cout << "Down: Camera control (context-dependent)\n";
    std::cout << "Left: Camera control (context-dependent)\n";
    std::cout << "Right: Camera control (context-dependent)\n";
    std::cout << "Tab + G: Switch to Engine Mode\n";
    std::cout << "Tab + N: Switch to DCC Mode\n";
    std::cout << std::endl;
}

} // namespace Ti3D