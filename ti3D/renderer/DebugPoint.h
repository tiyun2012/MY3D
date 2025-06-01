#ifndef DEBUGPOINT_H
#define DEBUGPOINT_H

#include "../TiMath/Vector3.h"
#include "../TiMath/Matrix4.h"
#include <GLFW/glfw3.h>

namespace Ti3D {

class DebugPoint {
public:
    TiMath::Vector3 position;
    float size; // Radius of the circle
    bool visible;

    DebugPoint(const TiMath::Vector3& pos = TiMath::Vector3(0.0f, 0.0f, 0.0f), float radius = 0.5f)
        : position(pos), size(radius), visible(true) {}

    // Initialize OpenGL resources (VAO, VBO, shader)
    void initialize();

    // Render the point as a circle facing the camera
    void render(const TiMath::Matrix4& viewMatrix, const TiMath::Matrix4& projectionMatrix) const;

    // Set position
    void setPosition(const TiMath::Vector3& pos) { position = pos; }

private:
    GLuint vao, vbo, ebo;
    GLuint shaderProgram;
    static const int circleSegments = 32; // Number of segments for the circle
};

} // namespace Ti3D

#endif // DEBUGPOINT_H