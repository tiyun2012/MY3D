#ifndef RENDERER_H
#define RENDERER_H

// #include <glad/glad.h> // For OpenGL types like GLuint
#include "../camera/Camera.h"
#include "../app/StateManager.h"
#include "DebugPoint.h"

namespace Ti3D {

class Renderer {
public:
    float axisLength;
    float gridSize;
    int gridLines;
    float gridSpacing;
    bool renderAxes;
    bool renderGrid;
    DebugPoint targetCamAim;

    Renderer(float axisLength, float gridSize, int gridLines, float gridSpacing);
    ~Renderer();

    void initialize();
    void draw(const Camera& camera, const StateManager& stateManager);
    void drawAxes(const Camera& camera, const StateManager& stateManager);
    void drawGrid(const Camera& camera, const StateManager& stateManager);
    void drawTargetCamAim(const Camera& camera) const;
    void setRenderFlags(bool axes, bool grid);
    bool getRenderAxes() const { return renderAxes; }
    bool getRenderGrid() const { return renderGrid; }
    void increaseGridSize(float delta);
    void decreaseGridSize(float delta);
    void increaseGridLines(int delta);
    void decreaseGridLines(int delta);

private:
    GLuint axisVAO, axisVBO;
    GLuint gridVAO, gridVBO;
    GLuint axisShaderProgram, gridShaderProgram;

    void initAxis();
    void initGrid();
    void setGridParameters(int lines, float spacing, float size);
};

} // namespace Ti3D

#endif // RENDERER_H