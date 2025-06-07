#ifndef CAMERA_H
#define CAMERA_H

#include "../TiMath/Vector3.h"
#include "../TiMath/Matrix4.h"
#include "../TiMath/Quaternion.h" // Add this include for Quaternion
#include <GLFW/glfw3.h>

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
    double CameraWidth = 800.0; // Default width
    double CameraHeight = 600.0; // Default height
    double CameraAspectRatio = CameraWidth / CameraHeight;
    Camera()
        : target(0.0f, 0.0f, 0.0f), distance(10.0f), aspectRatio(4.0f / 3.0f),
          zNear(0.1f), zFar(100.0f), viewMode(ViewMode::Far),
          projectionMode(ProjectionMode::Perspective), fovYDegrees(60.0f),
          yawDegrees(60.0f), pitchDegrees(40.0f) {}

    /**
     * @brief Processes keyboard and mouse input for zooming, panning, and view/projection mode switching.
     * @param window GLFW window for input polling.
     * @param deltaTime Time since last frame (in seconds) for smooth movement.
     * @param dccMode True if in DCC mode, to suppress movement during Spacebar.
     * @param spacebarPressed True if Spacebar is pressed, to zero movement in DCC mode.
     */

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
    void setAspectRatio(float aspectRatio) {
        this->aspectRatio = aspectRatio;
    }

    /**
     * @brief Focuses the camera on a specific point with a given distance.
     * @param point The target point to focus on.
     * @param distance The distance from the point to the camera.
     */
    void focusOnPoint(const TiMath::Vector3& point, float distance);

    /**
     * @brief Orbits the camera around a specific point based on mouse movement.
     * @param point The point to orbit around.
     * @param deltaX The change in mouse X position.
     * @param deltaY The change in mouse Y position.
     * @param sensitivity The sensitivity of the orbiting motion.
     */
    void orbitAroundPoint(const TiMath::Vector3& point, float deltaX, float deltaY, float sensitivity = 0.3f);

    // Arcball orbit API
    void startArcball(const TiMath::Vector3& point, float mouseX, float mouseY, float width, float height);
    void updateArcball(const TiMath::Vector3& point, double mouseX, double mouseY, double width, double height);
    void endArcball();
    TiMath::Vector3 projectToArcball(float mouseX, float mouseY, float width, float height, const TiMath::Vector3& center, float radius);

    TiMath::Vector3 getPosition() const;
    void setPosition(const TiMath::Vector3& pos);

private:
    bool arcballActive = false;
    TiMath::Vector3 arcballStartVec;
    TiMath::Vector3 arcballStartCamPos;
    float arcballRadius = 1.0f; // You can adjust this as needed
};

} // namespace Ti3D

#endif // CAMERA_H