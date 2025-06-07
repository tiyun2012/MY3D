#include "StateManager.h"
#include "Renderer.h"
#include <iostream>
#include <GLFW/glfw3.h>

namespace Ti3D {
Renderer* StateManager::currentRenderer = nullptr;
Camera* StateManager::currentCamera = nullptr;

StateManager::StateManager(GLFWwindow* window,Renderer & renderer,Camera &camera)
    :hotkeyCooldown(0.0f), modeCooldown(0.0f), currentContext(ContextType::None)
    {
        currentRenderer = &renderer;
        this->currentCamera = &camera;
        //camera initialization
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        currentCamera->setAspectRatio(static_cast<float>(windowWidth) / static_cast<float>(windowHeight)); // Set default aspect ratio
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
    else 
    {
        currentContext = ContextType::None;
    } 


    
}



} // namespace Ti3D