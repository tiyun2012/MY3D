#include "Camera.h"
#include <algorithm>
#include <iostream>

namespace Ti3D {

void Camera::processInput(GLFWwindow* window, float deltaTime, bool dccMode, bool spacebarPressed) {
    const float distanceStep = 0.1f;
    const float moveSpeed = 5.0f; // Units per second

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    // Zoom
    float distanceDelta = 0.0f;
    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
        distanceDelta -= distanceStep;
    }
    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
        distanceDelta += distanceStep;
    }
    // Suppress zoom in DCC mode when Spacebar is pressed
    if (dccMode && spacebarPressed) {
        distanceDelta = 0.0f;
        std::cout << "Camera zoom suppressed: Spacebar pressed in DCC mode\n";
    }
    distance = std::max(1.0f, std::min(1000.0f, distance + distanceDelta));

    // Pan
    TiMath::Vector3 moveDir(0.0f, 0.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        moveDir += (viewMode == ViewMode::Left || viewMode == ViewMode::Right) ?
                   TiMath::Vector3(0.0f, 1.0f, 0.0f) : // Up in Left/Right view
                   (viewMode == ViewMode::Top || viewMode == ViewMode::Bottom) ?
                   TiMath::Vector3(0.0f, 0.0f, -1.0f) : // Forward in Top/Bottom view
                   TiMath::Vector3(0.0f, 0.0f, -1.0f); // Forward in Far view
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        moveDir += (viewMode == ViewMode::Left || viewMode == ViewMode::Right) ?
                   TiMath::Vector3(0.0f, -1.0f, 0.0f) : // Down in Left/Right view
                   (viewMode == ViewMode::Top || viewMode == ViewMode::Bottom) ?
                   TiMath::Vector3(0.0f, 0.0f, 1.0f) : // Backward in Top/Bottom view
                   TiMath::Vector3(0.0f, 0.0f, 1.0f); // Backward in Far view
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        moveDir += TiMath::Vector3(-1.0f, 0.0f, 0.0f); // Left in all views
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        moveDir += TiMath::Vector3(1.0f, 0.0f, 0.0f); // Right in all views
    }
    // Suppress panning in DCC mode when Spacebar is pressed
    if (dccMode && spacebarPressed) {
        moveDir = TiMath::Vector3(0.0f, 0.0f, 0.0f);
        std::cout << "Camera panning suppressed: Spacebar pressed in DCC mode\n";
    }
    if (!moveDir.isZero()) {
        moveDir = moveDir.normalized();
        std::cout << "Camera moving: moveDir=(" << moveDir.x << ", " << moveDir.y << ", " << moveDir.z 
                  << "), target=(" << target.x << ", " << target.y << ", " << target.z << ")\n";
    }
    target += moveDir * moveSpeed * deltaTime;

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
    // Toggle projection mode
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        static bool lastPState = false;
        bool currentPState = true;
        if (currentPState && !lastPState) {
            projectionMode = (projectionMode == ProjectionMode::Orthographic) ?
                             ProjectionMode::Perspective : ProjectionMode::Orthographic;
        }
        lastPState = currentPState;
    } else {
        static bool lastPState = false;
        lastPState = false;
    }
}

void Camera::processMouseInput(GLFWwindow* window, double xpos, double ypos, float deltaTime) {
    static double lastX = 0.0, lastY = 0.0;
    float deltaX = static_cast<float>(xpos - lastX);
    float deltaY = static_cast<float>(ypos - lastY);
    lastX = xpos; lastY = ypos;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        TiMath::Vector3 moveDir(-deltaX * 0.01f, deltaY * 0.01f, 0.0f);
        target += moveDir * distance * deltaTime;
    }
}

TiMath::Matrix4 Camera::getViewMatrix() const {
    TiMath::Vector3 position;
    TiMath::Vector3 up(0.0f, 1.0f, 0.0f);
    switch (viewMode) {
        case ViewMode::Top:
            position = target + TiMath::Vector3(0.0f, distance, 0.0f);
            up = TiMath::Vector3(0.0f, 0.0f, -1.0f);
            break;
        case ViewMode::Left:
            position = target + TiMath::Vector3(-distance, 0.0f, 0.0f);
            break;
        case ViewMode::Right:
            position = target + TiMath::Vector3(distance, 0.0f, 0.0f);
            break;
        case ViewMode::Bottom:
            position = target + TiMath::Vector3(0.0f, -distance, 0.0f);
            up = TiMath::Vector3(0.0f, 0.0f, 1.0f);
            break;
        case ViewMode::Far:
            position = target + TiMath::Vector3(0.0f, 0.0f, -distance);
            up = TiMath::Vector3(0.0f, 1.0f, 0.0f);
            break;
    }
    return TiMath::Matrix4::lookAt(position, target, up);
}

TiMath::Matrix4 Camera::getProjectionMatrix() const {
    if (projectionMode == ProjectionMode::Perspective) {
        return TiMath::Matrix4::perspective(fovYDegrees, aspectRatio, zNear, zFar);
    }
    float halfHeight = distance;
    float halfWidth = halfHeight * aspectRatio;
    return TiMath::Matrix4::orthographic(-halfWidth, halfWidth, -halfHeight, halfHeight, zNear, zFar);
}

} // namespace Ti3D