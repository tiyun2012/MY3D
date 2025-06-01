#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include "../renderer/Renderer.h"
#include <functional>
#include <vector>
#include <string>
#include <map>

namespace Ti3D {

enum class AppMode { DCC, Engine };

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

private:
    AppMode currentMode;
    bool spacebarPressed;
    float hotkeyCooldown;
    float modeCooldown;
    
    std::vector<Hotkey> dccHotkeys;
    std::vector<Hotkey> engineHotkeys;
    std::vector<Hotkey> modeHotkeys;
    
    static const std::map<int, std::string> specialKeyNames;
};

} // namespace Ti3D

#endif // STATEMANAGER_H