#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <vector>
#include <iostream>
#include "../TiMath/Matrix4.h"
#include "../TiMath/Vector4.h"
#include "Camera.h"

namespace Ti3D {

/**
 * @class Renderer
 * @brief Manages OpenGL rendering of coordinate axes and a grid plane.
 */
class Renderer {
private:
    unsigned int shaderProgram;
    unsigned int axesVAO, axesVBO;
    unsigned int gridVAO, gridVBO;

    /**
     * @brief Creates and compiles the shader program.
     * @return The shader program ID, or 0 if compilation fails.
     */
    unsigned int createShaderProgram();

    /**
     * @brief Initializes the grid geometry based on camera distance.
     * @param cameraDistance The camera's distance for scaling the grid.
     */
    void initGrid(float cameraDistance);

    /**
     * @brief Checks for OpenGL errors and logs them.
     * @param operation The operation being checked (for error reporting).
     */
    void checkGLError(const char* operation);

public:
    Renderer();
    ~Renderer();

    /**
     * @brief Reinitializes OpenGL resources (e.g., after context change).
     */
    void reinitialize();

    /**
     * @brief Draws coordinate axes and grid using the provided camera.
     * @param camera The camera providing view and projection matrices.
     */
    void drawAxes(const Camera& camera);
};

} // namespace Ti3D

#endif // RENDERER_H