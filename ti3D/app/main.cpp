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

    // Map for special key names
    static const std::map<int, std::string> specialKeyNames = {
        { GLFW_KEY_UP, "Up" },
        { GLFW_KEY_DOWN, "Down" },
        { GLFW_KEY_LEFT, "Left" },
        { GLFW_KEY_RIGHT, "Right" }
    };

    // Hotkey table
    struct Hotkey {
        int key; // GLFW key code
        std::string description; // Action description
        std::function<void(Ti3D::Renderer&)> action; // Callback function
    };
    std::vector<Hotkey> hotkeys = {
        { GLFW_KEY_1, "Toggle axes visibility",
          [&renderer](Ti3D::Renderer& r) { r.setRenderFlags(!r.getRenderAxes(), r.getRenderGrid()); } },
        { GLFW_KEY_2, "Toggle grid visibility",
          [&renderer](Ti3D::Renderer& r) { r.setRenderFlags(r.getRenderAxes(), !r.getRenderGrid()); } },
        { GLFW_KEY_UP, "Increase axis length",
          [&renderer](Ti3D::Renderer& r) { r.setAxisLength(r.getAxisLength() + 0.1f); } },
        { GLFW_KEY_DOWN, "Decrease axis length",
          [&renderer](Ti3D::Renderer& r) { r.setAxisLength(r.getAxisLength() - 0.1f); } },
        { GLFW_KEY_LEFT, "Decrease grid lines",
          [&renderer](Ti3D::Renderer& r) { r.setGridParameters(r.getGridSize(), r.getGridLines() - 1, r.getGridSpacing()); } },
        { GLFW_KEY_RIGHT, "Increase grid lines",
          [&renderer](Ti3D::Renderer& r) { r.setGridParameters(r.getGridSize(), r.getGridLines() + 1, r.getGridSpacing()); } }
    };

    // Print hotkey table for user reference
    std::cout << "Hotkeys:\n";
    for (const auto& hotkey : hotkeys) {
        const char* keyName = glfwGetKeyName(hotkey.key, 0);
        std::string displayName;
        if (keyName) {
            displayName = keyName;
        } else {
            auto it = specialKeyNames.find(hotkey.key);
            displayName = (it != specialKeyNames.end()) ? it->second : "Unknown";
        }
        std::cout << "Key " << displayName << ": " << hotkey.description << "\n";
    }
    std::cout << std::endl;

    float lastFrame = static_cast<float>(glfwGetTime());
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.processInput(window, deltaTime);

        // Process hotkeys
        static float toggleCooldown = 0.0f;
        toggleCooldown -= deltaTime;
        if (toggleCooldown <= 0.0f) {
            for (const auto& hotkey : hotkeys) {
                if (glfwGetKey(window, hotkey.key) == GLFW_PRESS) {
                    hotkey.action(renderer);
                    toggleCooldown = 0.2f;
                    break; // Prevent multiple actions per frame
                }
            }
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer.drawAxes(camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}