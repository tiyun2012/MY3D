#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <GLFW/glfw3.h>

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace Ti3D
{

enum class AppMode
{
    DCC,
    Engine
};

enum class ContextType
{
    None = 0,      // No context
    Viewport = 1,  // Viewport context
    Timeline = 2   // Timeline context (placeholder for future)
};

// Forward declaration of Renderer
class Renderer;
class Camera;
class StateManager
{
   public:
    StateManager(GLFWwindow* window, Renderer& renderer, Camera& camera);
    static Renderer* currentRenderer;  // Added Renderer reference
    static Camera* currentCamera;      // Added Camera reference
    double currentCursorX = 0.0, currentCursorY = 0.0;  // mouse state tracking
    void processHotkeys(GLFWwindow* window, Renderer& renderer,
                        float deltaTime);

    void updateMouseClickState(GLFWwindow* window);

    // Add reference to Renderer for TargetCamAim access
    void setRenderer(Renderer* rdr) { currentRenderer = rdr; }
    void setCamera(Camera* cam) { currentCamera = cam; }

   private:
    float hotkeyCooldown;
    float modeCooldown;
    bool FKeyPressed;  // Tracks if F key is pressed
    ContextType currentContext;
    bool cameraUpdate;  // Tracks if camera state changed
    int windowWidth = 800, windowHeight = 600;  // Default window size
};

}  // namespace Ti3D

#endif  // STATEMANAGER_H