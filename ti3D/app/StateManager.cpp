#include "StateManager.h"
#include "Renderer.h"
#include "CameraManager.h"
#include <iostream>
#include <GLFW/glfw3.h>

namespace Ti3D {


StateManager::StateManager(GLFWwindow* window)
    : currentMode(AppMode::DCC), spacebarPressed(false), hotkeyCooldown(0.0f), 
      modeCooldown(0.0f), currentContext(ContextType::None), cameraUpdate(false),
      FKeyPressed(false){
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
    glfwSetKeyCallback(window, keyCallback);
}

void StateManager::setMode(AppMode mode) {
    if (currentMode != mode) {
        currentMode = mode;
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
    
    glfwGetCursorPos(window, &currentCursorX, &currentCursorY); // mouse state tracking
    static bool lastLeftClickState = false;
    // bool currentLeftClickState = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);

    if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)) 
    {
        // Left click pressed: set to Viewport context
        // currentContext = ContextType::Viewport;
        std::cout << "GLFW_PRESS\n";
    } 


    
}

void StateManager::stateActions(GLFWwindow *window)
{
    focusOnTarget(window);
}

void StateManager::focusOnTarget(GLFWwindow* window)
{
   bool isCurrentFPressed = (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS);
   if (isCurrentFPressed && !FKeyPressed)
   {
       std::cout << "Focusing on .... \n";
       
       FKeyPressed = true;
   }
   else if (!isCurrentFPressed)
   {
       FKeyPressed = false;
   }

}

} // namespace Ti3D