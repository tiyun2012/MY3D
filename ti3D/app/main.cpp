#include <glad/glad.h>
#include "../camera/CameraManager.h"
#include "../renderer/Renderer.h"
#include "StateManager.h"
#include <iostream>
namespace Ti3D {
static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (!cam) {
        std::cerr << "Error: Camera pointer is null in framebufferSizeCallback" << std::endl;
        return;
    }
    float aspectRatio = (height > 0) ? static_cast<float>(width) / static_cast<float>(height) : 1.0f;
    cam->setAspectRatio(aspectRatio); // Use single aspect ratio parameter if applicable
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

    Ti3D::CameraManager cameraManager;
    float initialAspectRatio = static_cast<float>(width) / static_cast<float>(height);
    cameraManager.getActiveCamera().setAspectRatio(initialAspectRatio);
    Ti3D::Renderer renderer(2.0f, 20.0f, 10, 2.0f);
    Ti3D::StateManager stateManager;
    stateManager.setCameraManager(&cameraManager);
    stateManager.setRenderer(&renderer);
    glEnable(GL_DEPTH_TEST);

    glfwSetFramebufferSizeCallback(window, Ti3D::framebufferSizeCallback);
    glfwSetCursorPosCallback(window, Ti3D::mouseCallback);
    glfwSetWindowUserPointer(window, &cameraManager.getActiveCamera());

    float lastFrame = static_cast<float>(glfwGetTime());
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        stateManager.updateHotkeyStates(window, deltaTime);
        stateManager.processHotkeys(window, renderer);
        stateManager.stateActions(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer.draw(cameraManager.getActiveCamera(), stateManager);

        stateManager.resetCameraUpdate();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}