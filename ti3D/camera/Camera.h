#ifndef CAMERA_H
#define CAMERA_H

#define GLFW_INCLUDE_NONE // Prevent GLFW from including OpenGL headers
#include <GLFW/glfw3.h>
#include "../TiMath/Vector3.h"
#include "../TiMath/Matrix4.h"

namespace Ti3D {

class Camera {
public:
    enum class ViewMode { Top, Left, Right, Bottom, Far };
    enum class ProjectionMode { Orthographic, Perspective };

    TiMath::Vector3 target;
    float distance;
    float aspectRatio;
    float zNear, zFar;
    ViewMode viewMode;
    ProjectionMode projectionMode;
    float fovYDegrees;
    float yawDegrees;
    float pitchDegrees;

    Camera()
        : target(0.0f, 0.0f, 0.0f), distance(10.0f), aspectRatio(4.0f / 3.0f),
          zNear(0.1f), zFar(100.0f), viewMode(ViewMode::Far),
          projectionMode(ProjectionMode::Perspective), fovYDegrees(60.0f),
          yawDegrees(60.0f), pitchDegrees(40.0f) {}

    void processInput(GLFWwindow* window, float deltaTime, bool dccMode = false, bool spacebarPressed = false);
    void processMouseInput(GLFWwindow* window, double xpos, double ypos, float deltaTime);
    [[nodiscard]] TiMath::Matrix4 getViewMatrix() const;
    [[nodiscard]] TiMath::Matrix4 getProjectionMatrix() const;
    void setAspectRatio(float width, float height) {
        aspectRatio = width / height;
    }
};

} // namespace Ti3D

#endif // CAMERA_H