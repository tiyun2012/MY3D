#include <algorithm>
#include <iostream>
#include "Camera.h"
#include "../TiMath/Quaternion.h" // <-- Add this line if not present

namespace Ti3D {
Camera::Camera()
    : target(0.0f, 0.0f, 0.0f), distance(10.0f), aspectRatio(4.0f / 3.0f),
      zNear(0.1f), zFar(100.0f), viewMode(ViewMode::Far),
      projectionMode(ProjectionMode::Perspective), fovYDegrees(60.0f),
      yawDegrees(60.0f), pitchDegrees(40.0f) {}

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

void Camera::updateArcball(const TiMath::Vector3& point, double mouseX, double mouseY, double width, double height) {
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