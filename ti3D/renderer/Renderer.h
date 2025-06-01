#ifndef RENDERER_H
#define RENDERER_H

#include "../camera/Camera.h"

namespace Ti3D {

// Forward declaration of StateManager
class StateManager;

class Renderer {
public:
    Renderer(float axisLength, float gridSize, int gridLines, float gridSpacing);
    ~Renderer();

    void drawAxes(const Camera& camera, const StateManager& stateManager);
    void drawGrid(const Camera& camera, const StateManager& stateManager);
    void setRenderFlags(bool renderAxes, bool renderGrid);
    bool getRenderAxes() const { return renderAxes; }
    bool getRenderGrid() const { return renderGrid; }
    void setGridParameters(float gridSize, int gridLines);
    void increaseGridSize(float increment);
    void decreaseGridSize(float decrement);
    void increaseGridLines(int increment);
    void decreaseGridLines(int decrement);

private:
    void initAxis();
    void initGrid();

    bool renderAxes;
    bool renderGrid;
    float axisLength;
    float gridSize;
    int gridLines;
    float gridSpacing;

    GLuint axisVAO, axisVBO;
    GLuint gridVAO, gridVBO;
    GLuint axisShaderProgram;
    GLuint gridShaderProgram;
};

} // namespace Ti3D

#endif // RENDERER_H