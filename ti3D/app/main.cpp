#include "../camera/Camera.h"
#include "../renderer/Renderer.h"
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
    glEnable(GL_DEPTH_TEST);

    glfwSetFramebufferSizeCallback(window, Ti3D::framebufferSizeCallback);
    glfwSetCursorPosCallback(window, Ti3D::mouseCallback);
    glfwSetWindowUserPointer(window, &camera);

    float lastFrame = static_cast<float>(glfwGetTime());
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.processInput(window, deltaTime);

        // Toggle rendering flags and adjust parameters
        static float toggleCooldown = 0.0f;
        toggleCooldown -= deltaTime;
        if (toggleCooldown <= 0.0f) {
            if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
                renderer.setRenderFlags(!renderer.getRenderAxes(), renderer.getRenderGrid());
                toggleCooldown = 0.2f;
            }
            if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
                renderer.setRenderFlags(renderer.getRenderAxes(), !renderer.getRenderGrid());
                toggleCooldown = 0.2f;
            }
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                renderer.setAxisLength(renderer.getAxisLength() + 0.1f);
                toggleCooldown = 0.2f;
            }
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                renderer.setAxisLength(renderer.getAxisLength() - 0.1f);
                toggleCooldown = 0.2f;
            }
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                renderer.setGridParameters(renderer.getGridSize(), renderer.getGridLines() - 1, renderer.getGridSpacing());
                toggleCooldown = 0.2f;
            }
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                renderer.setGridParameters(renderer.getGridSize(), renderer.getGridLines() + 1, renderer.getGridSpacing());
                toggleCooldown = 0.2f;
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