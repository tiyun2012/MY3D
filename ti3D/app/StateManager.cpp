#include "StateManager.h"
#include "Renderer.h"
#include "CameraManager.h"
#include <iostream>
#include <GLFW/glfw3.h>

namespace Ti3D {


StateManager::StateManager(GLFWwindow* window,Renderer & renderer)
    : currentMode(AppMode::DCC), spacebarPressed(false), hotkeyCooldown(0.0f), 
      modeCooldown(0.0f), currentContext(ContextType::None), cameraUpdate(false),
      FKeyPressed(false) {
        
    
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
    currentRenderer= &renderer; // Store reference to Renderer
    // glfwSetKeyCallback(window, keyCallback); can be removed if not needed
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

void StateManager::processHotkeys(GLFWwindow* window, Renderer& renderer,float deltaTime) 
{
    hotkeyCooldown -= deltaTime;
    modeCooldown -= deltaTime;

    if ((hotkeyCooldown <= 0.0f) && (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) 
                                && (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS))
    {
        if(glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)                   
        {
          
            currentRenderer->renderAxes = !currentRenderer->renderAxes;
            std::cout << "Toggled: Origin Axis \n";
            hotkeyCooldown = 0.2f;
        }
        //Grid Display 
        if(glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)                   
        {
          // Toggle grid visibility
          currentRenderer->renderGrid = !currentRenderer->renderGrid;
          std::cout << "Toggled: Grid \n";
          hotkeyCooldown = 0.2f;
        }
        if(glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)                   
        {
          
            currentRenderer->increaseGridLines(1);
            std::cout << "Increased Grid Lines \n";
            hotkeyCooldown = 0.2f;

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

Renderer* StateManager::currentRenderer = nullptr;


void StateManager::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
 
    // Check if the key is a modifier key (e.g., Shift, Control, Alt)
    if (mods & (GLFW_MOD_SHIFT | GLFW_MOD_CONTROL | GLFW_MOD_ALT)) {
        std::cout << "Modifier key pressed: " << key << "\n";
        return;
    }

    // Handle spacebar press for toggling camera update
    if (key == GLFW_KEY_SPACE) {
        if (action == GLFW_PRESS) {
            std::cout << "Spacebar pressed\n";
            currentRenderer->targetCamAim.visible = !currentRenderer->targetCamAim.visible;
        }
        return; // Skip further processing for spacebar
    }

    // Process other keys
if (action == GLFW_PRESS || action == GLFW_REPEAT) 
    {
      std::cout << "Key pressed: " <<key << "\n";
      
    } 
    else if (action == GLFW_RELEASE) 
   {
          std::cout << "Key Release: \n";
    } 
        
}

} // namespace Ti3D