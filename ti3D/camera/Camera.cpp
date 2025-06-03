#include <algorithm>
#include <iostream>
#include "Camera.h"
#include "../TiMath/Quaternion.h" // <-- Add this line if not present

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

void Camera::focusOnPoint(const TiMath::Vector3& point, float distance) {
    target = point;
    this->distance = distance;
    if (viewMode == ViewMode::Far) {
        // Preserve current orientation or set default if needed
        // If yawDegrees and pitchDegrees are not set meaningfully, use defaults
        if (std::abs(yawDegrees) < TiMath::EPSILON && std::abs(pitchDegrees) < TiMath::EPSILON) {
            yawDegrees = 60.0f; // Default from constructor
            pitchDegrees = 40.0f;
        }
        // Camera position will be computed in getViewMatrix using yaw and pitch
    }
    // For other view modes, getViewMatrix will compute position based on target and distance
}

void Camera::orbitAroundPoint(const TiMath::Vector3& point, float deltaX, float deltaY, float sensitivity) {
    // Convert deltas to angles (in degrees)
    float yawDelta = deltaX * sensitivity;
    float pitchDelta = deltaY * sensitivity;

    // Update yaw/pitch (clamp pitch to avoid flipping)
    yawDegrees += yawDelta;
    pitchDegrees += pitchDelta;
    pitchDegrees = std::clamp(pitchDegrees, -89.0f, 89.0f);

    // Compute new camera position in spherical coordinates
    float radYaw = yawDegrees * TiMath::PI / 180.0f;
    float radPitch = pitchDegrees * TiMath::PI / 180.0f;

    float x = point.x + distance * std::cos(radPitch) * std::sin(radYaw);
    float y = point.y + distance * std::sin(radPitch);
    float z = point.z + distance * std::cos(radPitch) * std::cos(radYaw);

    target = point; // Always look at the aim point

    // Print debug info
    std::cout << "[Orbit] Yaw: " << yawDegrees << " Pitch: " << pitchDegrees
              << " (ΔYaw: " << yawDelta << ", ΔPitch: " << pitchDelta << ")\n";
}

// Project screen coordinates to a virtual sphere around the target
TiMath::Vector3 Camera::projectToArcball(float mouseX, float mouseY, float width, float height, const TiMath::Vector3& center, float radius) {
    // Normalize to [-1, 1]
    float x = (2.0f * mouseX - width) / width;
    float y = (height - 2.0f * mouseY) / height;
    float z2 = radius * radius - x * x - y * y;
    float z = z2 > 0.0f ? std::sqrt(z2) : 0.0f;
    return TiMath::Vector3(x, y, z).normalized();
}

void Camera::startArcball(const TiMath::Vector3& point, float mouseX, float mouseY, float width, float height) {
    arcballActive = true;
    arcballStartCamPos = getPosition(); // Implement getPosition() to get camera world pos
    arcballStartVec = projectToArcball(mouseX, mouseY, width, height, point, arcballRadius);
}

void Camera::updateArcball(const TiMath::Vector3& point, float mouseX, float mouseY, float width, float height) {
    if (!arcballActive) return;
    TiMath::Vector3 currVec = projectToArcball(mouseX, mouseY, width, height, point, arcballRadius);

    // Compute rotation quaternion from startVec to currVec
    TiMath::Vector3 axis = arcballStartVec.cross(currVec);
    float dot = std::clamp(arcballStartVec.dot(currVec), -1.0f, 1.0f);
    float angle = std::acos(dot);

    if (axis.isZero() || angle == 0.0f) return;

    TiMath::Quaternion q = TiMath::Quaternion::fromAxisAngle(axis.normalized(), angle);

    // Rotate camera position around the point
    TiMath::Vector3 camOffset = arcballStartCamPos - point;
    TiMath::Vector3 newOffset = q.rotateVector(camOffset);
    TiMath::Vector3 newCamPos = point + newOffset;

    // Update camera position and target
    setPosition(newCamPos); // Implement setPosition() to update camera position
    target = point;
}

void Camera::endArcball() {
    arcballActive = false;
}

TiMath::Vector3 Camera::getPosition() const {
    // For Far view mode (orbit), reconstruct position from yaw/pitch/distance/target
    float radYaw = yawDegrees * TiMath::PI / 180.0f;
    float radPitch = pitchDegrees * TiMath::PI / 180.0f;
    return target + TiMath::Vector3(
        distance * std::cos(radPitch) * std::sin(radYaw),
        distance * std::sin(radPitch),
        distance * std::cos(radPitch) * std::cos(radYaw)
    );
}

void Camera::setPosition(const TiMath::Vector3& pos) {
    // Update yaw/pitch/distance based on new position and target
    TiMath::Vector3 offset = pos - target;
    distance = offset.length();
    yawDegrees = std::atan2(offset.x, offset.z) * 180.0f / TiMath::PI;
    pitchDegrees = std::asin(offset.y / distance) * 180.0f / TiMath::PI;
}

} // namespace Ti3D