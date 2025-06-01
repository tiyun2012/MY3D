#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <functional>
#include <vector>
#include <string>
#include <map>
#include <GLFW/glfw3.h>
#include "../camera/CameraManager.h"
#include "../renderer/TargetCamAim.h"

namespace Ti3D {

enum class AppMode { DCC, Engine };

enum class ContextType {
    None = 0,     // No context
    Viewport = 1, // Viewport context
    Timeline = 2  // Timeline context (placeholder for future)
};

class Renderer;

class StateManager {
public:
    struct Hotkey {
        int key;
        std::string description;
        std::function<void(Renderer&)> action;
        int modifier;
    };

    StateManager();
    
    void setMode(AppMode mode);
    AppMode getMode() const { return currentMode; }
    
    void updateHotkeyStates(GLFWwindow* window, float deltaTime);
    void processHotkeys(GLFWwindow* window, Renderer& renderer);
    
    bool isSpacebarPressed() const { return spacebarPressed; }
    
    void printControls() const;

    void updateMouseClickState(GLFWwindow* window);
    void updateCameraInput(GLFWwindow* window, double xpos, double ypos);
    bool isCameraUpdated() const { return cameraUpdate; }
    void resetCameraUpdate() { cameraUpdate = false; }

    CameraManager* cameraManager;
    void setCameraManager(CameraManager* mgr) { cameraManager = mgr; }

    // TargetCamAim management
    TargetCamAim& getTargetCamAim() { return targetCamAim; }
    void focusCameraOnTarget(float distance = 50.0f);

private:
    AppMode currentMode;
    bool spacebarPressed;
    float hotkeyCooldown;
    float modeCooldown;
    ContextType currentContext;
    bool cameraUpdate;
    
    std::vector<Hotkey> dccHotkeys;
    std::vector<Hotkey> engineHotkeys;
    std::vector<Hotkey> modeHotkeys;
    
    TargetCamAim targetCamAim; // Instance to manage aim point
    
    static const std::map<int, std::string> specialKeyNames;
};

} // namespace Ti3D

#endif // STATEMANAGER_H