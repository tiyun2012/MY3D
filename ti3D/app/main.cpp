#include <glad/glad.h>

#include <iostream>

#include "Camera.h"  // Include Camera.h directly
#include "Renderer.h"
#include "StateManager.h"
#include "WindowData.h"  // Include WindowData.h for WindowData struct

namespace Ti3D
{
static void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    WindowData* data =
        static_cast<WindowData*>(glfwGetWindowUserPointer(window));
    if (!data || !data->camera)
    {
        std::cerr << "Error: WindowData or Camera pointer is null in "
                     "framebufferSizeCallback"
                  << std::endl;
        return;
    }
    float aspectRatio =
        (height > 0) ? static_cast<float>(width) / static_cast<float>(height)
                     : 1.0f;
    data->camera->setAspectRatio(aspectRatio);
}

static void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    WindowData* data =
        static_cast<WindowData*>(glfwGetWindowUserPointer(window));
    if (data && data->camera)
    {
        static float lastFrame = static_cast<float>(glfwGetTime());
        float currentFrame = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        data->camera->processMouseInput(window, xpos, ypos, deltaTime);
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            if (data->stateManager)
            {
                data->camera->updateArcball(TiMath::Vector3(0, 0, 0), xpos,
                                            ypos, data->camera->CameraWidth,
                                            data->camera->CameraHeight);
            }
        }
    }
}

static void mouseButtonCallback(GLFWwindow* window, int button, int action,
                                int mods)
{
    WindowData* data =
        static_cast<WindowData*>(glfwGetWindowUserPointer(window));
    if (data && data->camera && data->stateManager)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        {
            data->camera->startArcball(TiMath::Vector3(0, 0, 0), xpos, ypos,
                                       data->camera->CameraWidth,
                                       data->camera->CameraHeight);
        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        {
            data->camera->endArcball();
        }
    }
}
}  // namespace Ti3D

int main()
{
    if (!glfwInit())
    {
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
    GLFWwindow* window =
        glfwCreateWindow(width, height, "Ti3D", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Initialize Camera
    Ti3D::Camera camera;
    // camera.setAspectRatio(static_cast<float>(width) /
    // static_cast<float>(height)); camera.viewMode =
    // Ti3D::Camera::ViewMode::Far; // Ensure perspective view
    // camera.projectionMode = Ti3D::Camera::ProjectionMode::Perspective;

    // Initialize Renderer
    Ti3D::Renderer renderer(2.0f, 20.0f, 10, 2.0f);

    // Initialize StateManager
    Ti3D::StateManager stateManager(window, renderer, camera);

    // Set window user pointer to a struct containing Camera and StateManager
    struct WindowData
    {
        Ti3D::Camera* camera;
        Ti3D::StateManager* stateManager;
    };
    WindowData windowData = {&camera, &stateManager};
    glfwSetWindowUserPointer(window, &windowData);

    glEnable(GL_DEPTH_TEST);
    glfwSetFramebufferSizeCallback(window, Ti3D::framebufferSizeCallback);
    glfwSetCursorPosCallback(window, Ti3D::mouseCallback);
    glfwSetMouseButtonCallback(window, Ti3D::mouseButtonCallback);

    float lastFrame = static_cast<float>(glfwGetTime());
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        stateManager.processHotkeys(window, renderer, deltaTime);
        stateManager.updateMouseClickState(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render the scene
        renderer.draw(camera, stateManager);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}