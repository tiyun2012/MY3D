#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <functional>
#include <vector>
#include <string>
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

    StateManager();
    
    void setMode(AppMode mode);
    AppMode getMode() const { return currentMode; }
    
    void updateHotkeyStates(GLFWwindow* window, float deltaTime);
    void processHotkeys(GLFWwindow* window, Renderer& renderer);
    
    bool isSpacebarPressed() const { return spacebarPressed; }
    

    // Update mouse click state and process movement keys based on context
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
    void setRenderer(Renderer* rdr) { renderer = rdr; }

    int windowWidth = 800;
    int windowHeight = 600;

private:
    AppMode currentMode;
    bool spacebarPressed;
    float hotkeyCooldown;
    float modeCooldown;
    ContextType currentContext;
    bool cameraUpdate; // Tracks if camera state changed
    Renderer* renderer = nullptr; // Added Renderer reference
    
    std::vector<Hotkey> dccHotkeys;
    std::vector<Hotkey> engineHotkeys;
    std::vector<Hotkey> modeHotkeys;
    
    static const std::map<int, std::string> specialKeyNames;
};

} // namespace Ti3D

#endif // STATEMANAGER_H