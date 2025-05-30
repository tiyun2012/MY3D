#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../TiMath/Vector3.h"
#include "../TiMath/Matrix4.h"

namespace Ti3D {

/**
 * @class Camera
 * @brief A 3D camera class supporting orthographic top, left, right, bottom, and far views with zoom and pan.
 */
class Camera {
public:
    enum class ViewMode { Top, Left, Right, Bottom, Far };

    TiMath::Vector3 target;
    float distance;
    float aspectRatio;
    float zNear, zFar;
    ViewMode viewMode;

    Camera()
        : target(0.0f, 0.0f, 0.0f), distance(10.0f), aspectRatio(4.0f / 3.0f),
          zNear(0.1f), zFar(100.0f), viewMode(ViewMode::Top) {}

    /** @brief Processes keyboard input for zooming, panning, and view mode switching. */
    void processInput(GLFWwindow* window, float deltaTime) {
        const float distanceStep = 0.1f;
        const float moveSpeed = 5.0f; // Units per second
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
        // Zoom
        if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
            distance = std::max(1.0f, distance - distanceStep);
        }
        if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
            distance += distanceStep;
        }
        // Pan
        float moveDistance = moveSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            target.z -= moveDistance; // Move forward
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            target.z += moveDistance; // Move backward
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            target.x -= moveDistance; // Move left
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            target.x += moveDistance; // Move right
        }
        // Switch view mode
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
            viewMode = ViewMode::Top;
        }
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
            viewMode = ViewMode::Left;
        }
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            viewMode = ViewMode::Right;
        }
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
            viewMode = ViewMode::Bottom;
        }
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
            viewMode = ViewMode::Far;
        }
    }

    /** @brief Returns the view matrix based on the current view mode. */
    [[nodiscard]] TiMath::Matrix4 getViewMatrix() const {
        TiMath::Vector3 position;
        TiMath::Vector3 up(0.0f, 1.0f, 0.0f);

        switch (viewMode) {
            case ViewMode::Top:
                position = target + TiMath::Vector3(0.0f, distance, 0.0f);
                up = TiMath::Vector3(0.0f, 0.0f, -1.0f); // Z-up for top view
                break;
            case ViewMode::Left:
                position = target + TiMath::Vector3(-distance, 0.0f, 0.0f);
                up = TiMath::Vector3(0.0f, 1.0f, 0.0f);
                break;
            case ViewMode::Right:
                position = target + TiMath::Vector3(distance, 0.0f, 0.0f);
                up = TiMath::Vector3(0.0f, 1.0f, 0.0f);
                break;
            case ViewMode::Bottom:
                position = target + TiMath::Vector3(0.0f, -distance, 0.0f);
                up = TiMath::Vector3(0.0f, 0.0f, 1.0f); // Z-up for bottom view
                break;
            case ViewMode::Far:
                position = target + TiMath::Vector3(0.0f, 0.0f, -distance);
                up = TiMath::Vector3(0.0f, 1.0f, 0.0f);
                break;
        }

        TiMath::Vector3 f = (target - position).normalized();
        TiMath::Vector3 s = f.cross(up).normalized();
        TiMath::Vector3 u = s.cross(f);

        TiMath::Matrix4 view;
        view.m[0] = s.x; view.m[4] = s.y; view.m[8] = s.z; view.m[12] = 0.0f;
        view.m[1] = u.x; view.m[5] = u.y; view.m[9] = u.z; view.m[13] = 0.0f;
        view.m[2] = -f.x; view.m[6] = -f.y; view.m[10] = -f.z; view.m[14] = 0.0f;
        view.m[3] = 0.0f; view.m[7] = 0.0f; view.m[11] = 0.0f; view.m[15] = 1.0f;

        TiMath::Matrix4 translation = TiMath::Matrix4::translation(-position);
        return view * translation;
    }

    /** @brief Returns the orthographic projection matrix. */
    [[nodiscard]] TiMath::Matrix4 getProjectionMatrix() const {
        float halfHeight = distance; // View height scales with distance
        float halfWidth = halfHeight * aspectRatio;
        return TiMath::Matrix4::orthographic(
            -halfWidth, halfWidth, -halfHeight, halfHeight, zNear, zFar);
    }

    /** @brief Sets the aspect ratio based on window dimensions. */
    void setAspectRatio(float width, float height) {
        aspectRatio = width / height;
    }
};

} // namespace Ti3D

#endif // CAMERA_H