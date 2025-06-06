#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <functional>
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <GLFW/glfw3.h>
#include "../camera/CameraManager.h"

namespace Ti3D {

enum class AppMode { DCC, Engine };

enum class ContextType {
    None = 0,     // No context
    Viewport = 1, // Viewport context
    Timeline = 2  // Timeline context (placeholder for future)
};

// Forward declaration of Renderer
class Renderer;

class StateManager {
public:
    struct Hotkey {
        int key;
        std::string description;
        std::function<void(Renderer&)> action;
        int modifier;
    };

    StateManager(GLFWwindow* window,Renderer & renderer);
    
    void setMode(AppMode mode);
    AppMode getMode() const { return currentMode; }
    
    void updateHotkeyStates(GLFWwindow* window, float deltaTime);
    void processHotkeys(GLFWwindow* window, Renderer& renderer,float deltaTime);
    
    bool isSpacebarPressed() const { return spacebarPressed; }

    
    void updateMouseClickState(GLFWwindow* window);

    // Process camera input and set CameraUpdate

    // Getter for CameraUpdate
    bool isCameraUpdated() const { return cameraUpdate; }

    // Reset CameraUpdate after processing
    void resetCameraUpdate() { cameraUpdate = false; }

    // Add reference to CameraManager for hotkey actions
    CameraManager* cameraManager = nullptr;
    void setCameraManager(CameraManager* mgr) { cameraManager = mgr; }

    // Add reference to Renderer for TargetCamAim access
    void setRenderer(Renderer* rdr) { currentRenderer = rdr; }
    int windowWidth = 800;
    int windowHeight = 600;
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) ;
    static Renderer* currentRenderer ; // Added Renderer reference
    static Camera* currentCamera; // Added Camera reference


private:

    double currentCursorX = 0.0, currentCursorY = 0.0; // mouse state tracking
    AppMode currentMode;
    bool spacebarPressed;
    float hotkeyCooldown;
    float modeCooldown;
    bool FKeyPressed; // Tracks if F key is pressed
    ContextType currentContext;
    bool cameraUpdate; // Tracks if camera state changed
    
    
    std::vector<Hotkey> dccHotkeys;
    std::vector<Hotkey> engineHotkeys;
    std::vector<Hotkey> modeHotkeys;
    
};

} // namespace Ti3D

#endif // STATEMANAGER_H