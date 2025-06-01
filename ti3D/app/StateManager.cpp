#include "StateManager.h"
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
    : currentMode(AppMode::DCC), spacebarPressed(false), hotkeyCooldown(0.0f), modeCooldown(0.0f) {
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