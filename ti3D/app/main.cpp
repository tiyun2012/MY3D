#include "../camera/Camera.h"
#include "../renderer/Renderer.h"
#include <iostream>
#include <functional>
#include <vector>
#include <map>

namespace Ti3D {
static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (cam) {
        cam->setAspectRatio(static_cast<float>(width), static_cast<float>(height));
    }
}

static void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (cam) {
        static float lastFrame = static_cast<float>(glfwGetTime());
        float currentFrame = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        cam->processMouseInput(window, xpos, ypos, deltaTime);
    }
}
} // namespace Ti3D

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    int width = 800, height = 600;
    GLFWwindow* window = glfwCreateWindow(width, height, "Ti3D", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    Ti3D::Camera camera;
    camera.setAspectRatio(static_cast<float>(width), static_cast<float>(height));
    Ti3D::Renderer renderer(2.0f, 20.0f, 10, 2.0f); // Initial axis length, grid size, lines, spacing
    glEnable(GL_DEPTH_TEST);

    glfwSetFramebufferSizeCallback(window, Ti3D::framebufferSizeCallback);
    glfwSetCursorPosCallback(window, Ti3D::mouseCallback);
    glfwSetWindowUserPointer(window, &camera);

    // Application modes
    enum class AppMode { DCC, Engine };
    AppMode currentMode = AppMode::DCC;

    // Map for special key names
    static const std::map<int, std::string> specialKeyNames = {
        { GLFW_KEY_SPACE, "Space" },
        { GLFW_KEY_TAB, "Tab" },
        { GLFW_KEY_UP, "Up" },
        { GLFW_KEY_DOWN, "Down" },
        { GLFW_KEY_LEFT, "Left" },
        { GLFW_KEY_RIGHT, "Right" },
        { GLFW_KEY_W, "W" },
        { GLFW_KEY_A, "A" },
        { GLFW_KEY_S, "S" },
        { GLFW_KEY_D, "D" },
        { GLFW_KEY_G, "G" },
        { GLFW_KEY_N, "N" }
    };

    // Hotkey table
    struct Hotkey {
        int key; // GLFW key code (requires modifier)
        std::string description; // Action description
        std::function<void(Ti3D::Renderer&)> action; // Callback function
        int modifier; // Modifier key (e.g., GLFW_KEY_SPACE, GLFW_KEY_TAB)
    };
    // DCC Mode hotkeys
    std::vector<Hotkey> dccHotkeys = {
        { GLFW_KEY_A, "Space + A: Toggle axes visibility",
          [&renderer](Ti3D::Renderer& r) { r.setRenderFlags(!r.getRenderAxes(), r.getRenderGrid()); },
          GLFW_KEY_SPACE },
        { GLFW_KEY_G, "Space + G: Toggle grid visibility",
          [&renderer](Ti3D::Renderer& r) { r.setRenderFlags(r.getRenderAxes(), !r.getRenderGrid()); },
          GLFW_KEY_SPACE }
    };
    // Engine Mode hotkeys (placeholder for future design)
    std::vector<Hotkey> engineHotkeys = {
        // Add Engine Mode hotkeys here later
    };
    // Mode switching hotkeys
    std::vector<Hotkey> modeHotkeys = {
        { GLFW_KEY_G, "Tab + G: Switch to Engine Mode",
          [&currentMode](Ti3D::Renderer&) { currentMode = AppMode::Engine; },
          GLFW_KEY_TAB },
        { GLFW_KEY_N, "Tab + N: Switch to DCC Mode",
          [&currentMode](Ti3D::Renderer&) { currentMode = AppMode::DCC; },
          GLFW_KEY_TAB }
    };

    // Function to print controls based on mode
    auto printControls = [&](AppMode mode) {
        std::cout << "Current Mode: " << (mode == AppMode::DCC ? "DCC" : "Engine") << "\n";
        std::cout << "Controls:\n";
        const auto& hotkeys = (mode == AppMode::DCC) ? dccHotkeys : engineHotkeys;
        for (const auto& hotkey : hotkeys) {
            std::cout << hotkey.description << "\n";
        }
        std::cout << "W: Move camera forward\n";
        std::cout << "S: Move camera backward\n";
        std::cout << "A: Move camera left\n";
        std::cout << "D: Move camera right\n";
        std::cout << "Up: Camera control (context-dependent)\n";
        std::cout << "Down: Camera control (context-dependent)\n";
        std::cout << "Left: Camera control (context-dependent)\n";
        std::cout << "Right: Camera control (context-dependent)\n";
        std::cout << "Tab + G: Switch to Engine Mode\n";
        std::cout << "Tab + N: Switch to DCC Mode\n";
        std::cout << std::endl;
    };

    // Print initial controls
    printControls(currentMode);

    float lastFrame = static_cast<float>(glfwGetTime());
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Process mode switching
        static float modeCooldown = 0.0f;
        modeCooldown -= deltaTime;
        bool modeSwitched = false;
        if (modeCooldown <= 0.0f && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
            for (const auto& hotkey : modeHotkeys) {
                if (glfwGetKey(window, hotkey.key) == GLFW_PRESS) {
                    hotkey.action(renderer);
                    modeCooldown = 0.2f;
                    modeSwitched = true;
                    printControls(currentMode);
                    break;
                }
            }
        }

        // Process mode-specific hotkeys
        static float hotkeyCooldown = 0.0f;
        hotkeyCooldown -= deltaTime;
        bool spacebarPressed = (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS);
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

        // Process camera movement, suppressing in DCC mode when Spacebar is pressed
        camera.processInput(window, deltaTime, currentMode == AppMode::DCC, spacebarPressed);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer.drawAxes(camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}