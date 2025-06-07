#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <functional>
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <GLFW/glfw3.h>

namespace Ti3D {

enum class AppMode { DCC, Engine };

enum class ContextType {
    None = 0,     // No context
    Viewport = 1, // Viewport context
    Timeline = 2  // Timeline context (placeholder for future)
};

// Forward declaration of Renderer
class Renderer;

class StateManager 
{
public:
    
    StateManager(GLFWwindow* window,Renderer & renderer);
    void processHotkeys(GLFWwindow* window, Renderer& renderer, float deltaTime);

    void updateMouseClickState(GLFWwindow* window);


    // Add reference to Renderer for TargetCamAim access
    void setRenderer(Renderer* rdr) { currentRenderer = rdr; }

    static Renderer* currentRenderer; // Added Renderer reference

private:

    double currentCursorX = 0.0, currentCursorY = 0.0; // mouse state tracking
    float hotkeyCooldown;
    float modeCooldown;
    bool FKeyPressed; // Tracks if F key is pressed
    ContextType currentContext;
    bool cameraUpdate; // Tracks if camera state changed



};

} // namespace Ti3D

#endif // STATEMANAGER_H