#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include <array>
#include <optional>
#include "Camera.h"

namespace Ti3D {

// Camera view types for quick switching
enum class CameraViewType {
    Perspective = 0,
    Front = 1,
    Left = 2,
    Top = 3,
    Back = 4,
    Right = 5,
    Count
};

class CameraManager {
public:
    CameraManager();
    Camera& getActiveCamera();
    void switchTo(CameraViewType type);
    void switchToPrevious();
    CameraViewType getActiveType() const { return activeType; }
private:
    std::array<Camera, static_cast<size_t>(CameraViewType::Count)> cameras;
    CameraViewType activeType;
    std::optional<CameraViewType> previousType;
};

} // namespace Ti3D

#endif // CAMERAMANAGER_H