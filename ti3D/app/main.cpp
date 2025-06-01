#include <glad/glad.h>
#include "../camera/Camera.h"
#include "../renderer/Renderer.h"
#include "StateManager.h"
#include <iostream>

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
    Ti3D::StateManager stateManager;
    glEnable(GL_DEPTH_TEST);

    glfwSetFramebufferSizeCallback(window, Ti3D::framebufferSizeCallback);
    glfwSetCursorPosCallback(window, Ti3D::mouseCallback);
    glfwSetWindowUserPointer(window, &camera);

    double lastX = 0.0, lastY = 0.0;
    glfwGetCursorPos(window, &lastX, &lastY);

    float lastFrame = static_cast<float>(glfwGetTime());
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Update state, process hotkeys, and check mouse click state
        stateManager.updateHotkeyStates(window, deltaTime);
        stateManager.processHotkeys(window, renderer);
        stateManager.updateMouseClickState(window);

        // Update camera input
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        stateManager.updateCameraInput(window, xpos, ypos);

        // Process camera movement, suppressing in DCC mode when Spacebar is pressed
        camera.processInput(window, deltaTime, stateManager.getMode() == Ti3D::AppMode::DCC, stateManager.isSpacebarPressed());

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer.drawAxes(camera, stateManager);
        renderer.drawGrid(camera, stateManager);

        // Reset CameraUpdate after rendering
        stateManager.resetCameraUpdate();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}