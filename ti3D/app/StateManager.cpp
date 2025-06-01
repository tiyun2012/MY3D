#include "StateManager.h"
#include "Renderer.h"
#include <GLFW/glfw3.h>
#include <iostream>

namespace Ti3D {

const std::map<int, std::string> StateManager::specialKeyNames = {
    {GLFW_KEY_SPACE, "Space"},
    {GLFW_KEY_LEFT_CONTROL, "Ctrl"},
    {GLFW_KEY_LEFT_SHIFT, "Shift"},
    {GLFW_KEY_LEFT_ALT, "Alt"},
    {GLFW_KEY_F, "F"},
};

StateManager::StateManager()
    : currentMode(AppMode::DCC), spacebarPressed(false),
      hotkeyCooldown(0.0f), modeCooldown(0.0f),
      currentContext(ContextType::Viewport), cameraUpdate(false),
      cameraManager(nullptr),
      targetCamAim(0.5f, TiMath::Vector3(0.0f, 0.0f, 0.0f)) {
    dccHotkeys = {
        {GLFW_KEY_A, "Toggle Axes", [this](Renderer& renderer) {
            renderer.setRenderFlags(!renderer.getRenderAxes(), renderer.getRenderGrid());
        }, 0},
        {GLFW_KEY_G, "Toggle Grid", [this](Renderer& renderer) {
            renderer.setRenderFlags(renderer.getRenderAxes(), !renderer.getRenderGrid());
        }, 0},
        {GLFW_KEY_F, "Focus Camera on Target", [this](Renderer& renderer) {
            if (currentContext == ContextType::Viewport) {
                focusCameraOnTarget(50.0f);
            }
        }, 0},
    };

    engineHotkeys = {
        {GLFW_KEY_A, "Toggle Axes", [this](Renderer& renderer) {
            renderer.setRenderFlags(!renderer.getRenderAxes(), renderer.getRenderGrid());
        }, 0},
        {GLFW_KEY_G, "Toggle Grid", [this](Renderer& renderer) {
            renderer.setRenderFlags(renderer.getRenderAxes(), !renderer.getRenderGrid());
        }, 0},
    };

    modeHotkeys = {
        {GLFW_KEY_M, "Switch to DCC Mode", [this](Renderer& renderer) {
            if (modeCooldown <= 0.0f) {
                setMode(AppMode::DCC);
                modeCooldown = 0.2f;
            }
        }, GLFW_MOD_CONTROL},
        {GLFW_KEY_M, "Switch to Engine Mode", [this](Renderer& renderer) {
            if (modeCooldown <= 0.0f) {
                setMode(AppMode::Engine);
                modeCooldown = 0.2f;
            }
        }, GLFW_MOD_ALT},
    };
}

void StateManager::setMode(AppMode mode) {
    currentMode = mode;
    std::cout << "Switched to " << (mode == AppMode::DCC ? "DCC" : "Engine") << " mode\n";
    printControls();
}

void StateManager::updateHotkeyStates(GLFWwindow* window, float deltaTime) {
    spacebarPressed = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
    hotkeyCooldown -= deltaTime;
    if (hotkeyCooldown < 0.0f) hotkeyCooldown = 0.0f;
    modeCooldown -= deltaTime;
    if (modeCooldown < 0.0f) modeCooldown = 0.0f;
}

void StateManager::processHotkeys(GLFWwindow* window, Renderer& renderer) {
    if (hotkeyCooldown > 0.0f) return;

    for (const auto& hotkey : modeHotkeys) {
        if (glfwGetKey(window, hotkey.key) == GLFW_PRESS &&
            glfwGetKey(window, hotkey.modifier) == GLFW_PRESS) {
            hotkey.action(renderer);
            hotkeyCooldown = 0.2f;
            break;
        }
    }

    const auto& activeHotkeys = (currentMode == AppMode::DCC) ? dccHotkeys : engineHotkeys;
    for (const auto& hotkey : activeHotkeys) {
        if (hotkey.modifier == 0) {
            if (glfwGetKey(window, hotkey.key) == GLFW_PRESS) {
                hotkey.action(renderer);
                hotkeyCooldown = 0.2f;
                break;
            }
        } else {
            if (glfwGetKey(window, hotkey.key) == GLFW_PRESS &&
                glfwGetKey(window, hotkey.modifier) == GLFW_PRESS) {
                hotkey.action(renderer);
                hotkeyCooldown = 0.2f;
                break;
            }
        }
    }
}

void StateManager::printControls() const {
    std::cout << "Current Mode: " << (currentMode == AppMode::DCC ? "DCC" : "Engine") << "\n";
    std::cout << "Available Hotkeys:\n";
    const auto& activeHotkeys = (currentMode == AppMode::DCC) ? dccHotkeys : engineHotkeys;
    for (const auto& hotkey : activeHotkeys) {
        std::string keyName = specialKeyNames.count(hotkey.key) ? specialKeyNames.at(hotkey.key) : std::string(1, (char)hotkey.key);
        std::string modifier = (hotkey.modifier == GLFW_MOD_CONTROL) ? "Ctrl+" :
                              (hotkey.modifier == GLFW_MOD_ALT) ? "Alt+" :
                              (hotkey.modifier == GLFW_MOD_SHIFT) ? "Shift+" : "";
        std::cout << modifier << keyName << ": " << hotkey.description << "\n";
    }
    for (const auto& hotkey : modeHotkeys) {
        std::string keyName = specialKeyNames.count(hotkey.key) ? specialKeyNames.at(hotkey.key) : std::string(1, (char)hotkey.key);
        std::string modifier = (hotkey.modifier == GLFW_MOD_CONTROL) ? "Ctrl+" :
                              (hotkey.modifier == GLFW_MOD_ALT) ? "Alt+" :
                              (hotkey.modifier == GLFW_MOD_SHIFT) ? "Shift+" : "";
        std::cout << modifier << keyName << ": " << hotkey.description << "\n";
    }
}

void StateManager::updateMouseClickState(GLFWwindow* window) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        currentContext = ContextType::Viewport;
    }
}

void StateManager::updateCameraInput(GLFWwindow* window, double xpos, double ypos) {
    if (cameraManager) {
        Camera& camera = cameraManager->getActiveCamera();
        camera.processMouseInput(window, xpos, ypos, 0.016f);
        cameraUpdate = true;
    }
}

void StateManager::focusCameraOnTarget(float distance) {
    if (!cameraManager) {
        std::cerr << "Error: cameraManager is not set in StateManager\n";
        return;
    }
    Camera& camera = cameraManager->getActiveCamera();
    TiMath::Vector3 targetPos = targetCamAim.getPosition();

    TiMath::Vector3 camPos;
    switch (camera.viewMode) {
        case Camera::ViewMode::Top:
            camPos = camera.target + TiMath::Vector3(0.0f, camera.distance, 0.0f);
            break;
        case Camera::ViewMode::Left:
            camPos = camera.target + TiMath::Vector3(-camera.distance, 0.0f, 0.0f);
            break;
        case Camera::ViewMode::Right:
            camPos = camera.target + TiMath::Vector3(camera.distance, 0.0f, 0.0f);
            break;
        case Camera::ViewMode::Bottom:
            camPos = camera.target + TiMath::Vector3(0.0f, -camera.distance, 0.0f);
            break;
        case Camera::ViewMode::Far:
            camPos = camera.target + TiMath::Vector3(0.0f, 0.0f, -camera.distance);
            break;
    }

    TiMath::Vector3 direction = (camPos - targetPos).normalized();
    if (direction.isZero()) {
        direction = TiMath::Vector3(0.0f, 0.0f, -1.0f);
    }

    camera.target = targetPos;
    camera.distance = distance;
    cameraUpdate = true;
}

} // namespace Ti3D