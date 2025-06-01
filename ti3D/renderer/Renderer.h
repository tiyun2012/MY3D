#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../camera/Camera.h"

namespace Ti3D {

class Renderer {
private:
    GLuint axisShaderProgram;
    GLuint gridShaderProgram;
    GLuint axisVAO, axisVBO;
    GLuint gridVAO, gridVBO;
    bool renderAxes;
    bool renderGrid;
    float axisLength;
    float gridSize;
    int gridLines;
    float gridSpacing;

    void initAxis();
    void initGrid();

public:
    Renderer(float axisLength, float gridSize, int gridLines, float gridSpacing);
    ~Renderer();

    void drawAxes(const Camera& camera);
    void drawGrid(const Camera& camera);
    void setRenderFlags(bool renderAxes, bool renderGrid);
    bool getRenderAxes() const { return renderAxes; }
    bool getRenderGrid() const { return renderGrid; }
    void setGridParameters(float gridSize, int gridLines);

    // New methods to adjust grid size and lines
    void increaseGridSize(float increment = 1.0f);
    void decreaseGridSize(float decrement = 1.0f);
    void increaseGridLines(int increment = 1);
    void decreaseGridLines(int decrement = 1);
};

} // namespace Ti3D

#endif // RENDERER_H