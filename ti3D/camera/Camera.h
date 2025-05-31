#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../TiMath/Vector3.h"
#include "../TiMath/Matrix4.h"

namespace Ti3D {

/**
 * @class Camera
 * @brief A 3D camera class supporting orthographic and perspective projections with multiple views, zoom, and pan.
 */
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
    float yawDegrees;   // Rotation around Y-axis
    float pitchDegrees; // Rotation around X-axis

    Camera()
        : target(0.0f, 0.0f, 0.0f), distance(10.0f), aspectRatio(4.0f / 3.0f),
          zNear(0.1f), zFar(100.0f), viewMode(ViewMode::Far),
          projectionMode(ProjectionMode::Perspective), fovYDegrees(60.0f),
          yawDegrees(60.0f), pitchDegrees(40.0f) {}

    /**
     * @brief Processes keyboard and mouse input for zooming, panning, and view/projection mode switching.
     * @param window GLFW window for input polling.
     * @param deltaTime Time since last frame (in seconds) for smooth movement.
     */
    void processInput(GLFWwindow* window, float deltaTime);

    /**
     * @brief Processes mouse input for panning.
     * @param window GLFW window for input polling.
     * @param xpos Current mouse X position.
     * @param ypos Current mouse Y position.
     * @param deltaTime Time since last frame (in seconds).
     */
    void processMouseInput(GLFWwindow* window, double xpos, double ypos, float deltaTime);

    /**
     * @brief Returns the view matrix based on the current view mode and rotations.
     * @return A 4x4 view matrix.
     */
    [[nodiscard]] TiMath::Matrix4 getViewMatrix() const;

    /**
     * @brief Returns the projection matrix (orthographic or perspective).
     * @return A 4x4 projection matrix.
     */
    [[nodiscard]] TiMath::Matrix4 getProjectionMatrix() const;

    /**
     * @brief Sets the aspect ratio based on window dimensions.
     * @param width Window width.
     * @param height Window height.
     */
    void setAspectRatio(float width, float height) {
        aspectRatio = width / height;
    }
};

} // namespace Ti3D

#endif // CAMERA_H