#include "CameraManager.h"

namespace Ti3D {

CameraManager::CameraManager() : activeType(CameraViewType::Perspective), previousType(std::nullopt) {
    // Set up default camera views
    // Perspective
    cameras[static_cast<size_t>(CameraViewType::Perspective)] = Camera();
    cameras[static_cast<size_t>(CameraViewType::Perspective)].viewMode = Camera::ViewMode::Far;
    cameras[static_cast<size_t>(CameraViewType::Perspective)].projectionMode = Camera::ProjectionMode::Perspective;
    // Front
    cameras[static_cast<size_t>(CameraViewType::Front)] = Camera();
    cameras[static_cast<size_t>(CameraViewType::Front)].viewMode = Camera::ViewMode::Far;
    cameras[static_cast<size_t>(CameraViewType::Front)].projectionMode = Camera::ProjectionMode::Orthographic;
    // Left
    cameras[static_cast<size_t>(CameraViewType::Left)] = Camera();
    cameras[static_cast<size_t>(CameraViewType::Left)].viewMode = Camera::ViewMode::Left;
    cameras[static_cast<size_t>(CameraViewType::Left)].projectionMode = Camera::ProjectionMode::Orthographic;
    // Top
    cameras[static_cast<size_t>(CameraViewType::Top)] = Camera();
    cameras[static_cast<size_t>(CameraViewType::Top)].viewMode = Camera::ViewMode::Top;
    cameras[static_cast<size_t>(CameraViewType::Top)].projectionMode = Camera::ProjectionMode::Orthographic;
    // Back
    cameras[static_cast<size_t>(CameraViewType::Back)] = Camera();
    cameras[static_cast<size_t>(CameraViewType::Back)].viewMode = Camera::ViewMode::Bottom;
    cameras[static_cast<size_t>(CameraViewType::Back)].projectionMode = Camera::ProjectionMode::Orthographic;
    // Right
    cameras[static_cast<size_t>(CameraViewType::Right)] = Camera();
    cameras[static_cast<size_t>(CameraViewType::Right)].viewMode = Camera::ViewMode::Right;
    cameras[static_cast<size_t>(CameraViewType::Right)].projectionMode = Camera::ProjectionMode::Orthographic;
}

Camera& CameraManager::getActiveCamera() {
    return cameras[static_cast<size_t>(activeType)];
}

void CameraManager::switchTo(CameraViewType type) {
    if (type != activeType) {
        previousType = activeType;
        activeType = type;
    }
}

void CameraManager::switchToPrevious() {
    if (previousType) {
        CameraViewType tmp = activeType;
        activeType = *previousType;
        previousType = tmp;
    }
}

} // namespace Ti3D
