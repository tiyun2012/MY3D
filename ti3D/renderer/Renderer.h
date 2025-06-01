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
    GLint uMVPLocation, uColorLocation;
    float axisLength; // Length of each axis
    float gridSize; // Total size of grid (edge to edge)
    int gridLines; // Number of lines per axis
    float gridSpacing; // Distance between grid lines
    bool renderAxes; // Flag to render axes
    bool renderGrid; // Flag to render grid

    /**
     * @brief Creates and compiles the shader program.
     * @return The shader program ID, or 0 if compilation fails.
     */
    unsigned int createShaderProgram();

    /**
     * @brief Initializes the grid geometry based on view mode and parameters.
     * @param viewMode Camera view mode to determine grid plane.
     */
    void initGrid(Camera::ViewMode viewMode);

    /**
     * @brief Checks for OpenGL errors and logs them.
     * @param operation The operation being checked (for error reporting).
     */
    void checkGLError(const char* operation);

public:
    Renderer(float axisLength = 1.0f, float gridSize = 20.0f, int gridLines = 10, float gridSpacing = 2.0f);
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

    /**
     * @brief Sets rendering flags for axes and grid.
     * @param axes Enable/disable axes rendering.
     * @param grid Enable/disable grid rendering.
     */
    void setRenderFlags(bool axes, bool grid) {
        renderAxes = axes;
        renderGrid = grid;
    }

    /**
     * @brief Sets axis length.
     * @param length New length for each axis.
     */
    void setAxisLength(float length) {
        axisLength = std::max(0.1f, length); // Prevent zero/negative length
        reinitialize(); // Update VAO/VBO
    }

    /**
     * @brief Sets grid parameters.
     * @param size Total grid size (edge to edge).
     * @param lines Number of lines per axis.
     * @param spacing Distance between grid lines.
     */
    void setGridParameters(float size, int lines, float spacing) {
        gridSize = std::max(1.0f, size);
        gridLines = std::max(1, lines);
        gridSpacing = std::max(0.1f, spacing);
        reinitialize(); // Update grid geometry
    }

    /**
     * @brief Getters for rendering flags and parameters.
     */
    bool getRenderAxes() const { return renderAxes; }
    bool getRenderGrid() const { return renderGrid; }
    float getAxisLength() const { return axisLength; }
    float getGridSize() const { return gridSize; }
    int getGridLines() const { return gridLines; }
    float getGridSpacing() const { return gridSpacing; }
};

} // namespace Ti3D

#endif // RENDERER_H