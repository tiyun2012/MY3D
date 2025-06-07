#include <glad/glad.h>
#include "Renderer.h"
#include <iostream>
#include <vector>

namespace Ti3D {

Renderer::Renderer(float axisLength, float gridSize, int gridLines, float gridSpacing)
    : axisLength(axisLength), gridSize(gridSize), gridLines(gridLines), gridSpacing(gridSpacing),
      renderAxes(true), renderGrid(true), targetCamAim(TiMath::Vector3(0.0f, 0.0f, 0.0f), 0.5f) {
    initialize();
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &axisVAO);
    glDeleteBuffers(1, &axisVBO);
    glDeleteVertexArrays(1, &gridVAO);
    glDeleteBuffers(1, &gridVBO);
    glDeleteProgram(axisShaderProgram);
    glDeleteProgram(gridShaderProgram);
}

void Renderer::initialize() {
    initAxis();
    initGrid();
    targetCamAim.initialize();
}

void Renderer::initAxis() {
    // Vertex data: 3 axes (X, Y, Z) with positions and colors
    std::vector<float> vertices = {
        // X-axis (red)
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        axisLength, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        // Y-axis (green)
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, axisLength, 0.0f, 0.0f, 1.0f, 0.0f,
        // Z-axis (blue)
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, axisLength, 0.0f, 0.0f, 1.0f
    };

    glGenVertexArrays(1, &axisVAO);
    glGenBuffers(1, &axisVBO);

    glBindVertexArray(axisVAO);
    glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Shaders
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;
        out vec3 vertexColor;
        uniform mat4 view;
        uniform mat4 projection;
        void main() {
            gl_Position = projection * view * vec4(aPos, 1.0);
            vertexColor = aColor;
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        in vec3 vertexColor;
        out vec4 FragColor;
        void main() {
            FragColor = vec4(vertexColor, 1.0);
        }
    )";

    // Compile shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Axis Vertex Shader Compilation Failed: " << infoLog << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Axis Fragment Shader Compilation Failed: " << infoLog << std::endl;
    }

    axisShaderProgram = glCreateProgram();
    glAttachShader(axisShaderProgram, vertexShader);
    glAttachShader(axisShaderProgram, fragmentShader);
    glLinkProgram(axisShaderProgram);
    glGetProgramiv(axisShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(axisShaderProgram, 512, nullptr, infoLog);
        std::cerr << "Axis Shader Program Linking Failed: " << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Renderer::initGrid() {
    setGridParameters(gridLines, gridSpacing, gridSize);
}

void Renderer::setGridParameters(int lines, float spacing, float size) {
    gridLines = lines;
    gridSpacing = spacing;
    gridSize = size;

    // Generate grid vertices (lines in XZ plane)
    std::vector<float> vertices;
    int numLines = gridLines * 2 + 1; // Lines on both sides + center
    float halfSize = gridSize * 0.5f;

    // Vertical lines (along X)
    for (int i = -gridLines; i <= gridLines; ++i) {
        float x = i * gridSpacing;
        if (std::abs(x) <= halfSize) {
            vertices.push_back(x); vertices.push_back(0.0f); vertices.push_back(-halfSize);
            vertices.push_back(1.0f); vertices.push_back(1.0f); vertices.push_back(1.0f);
            vertices.push_back(x); vertices.push_back(0.0f); vertices.push_back(halfSize);
            vertices.push_back(1.0f); vertices.push_back(1.0f); vertices.push_back(1.0f);
        }
    }

    // Horizontal lines (along Z)
    for (int i = -gridLines; i <= gridLines; ++i) {
        float z = i * gridSpacing;
        if (std::abs(z) <= halfSize) {
            vertices.push_back(-halfSize); vertices.push_back(0.0f); vertices.push_back(z);
            vertices.push_back(1.0f); vertices.push_back(1.0f); vertices.push_back(1.0f);
            vertices.push_back(halfSize); vertices.push_back(0.0f); vertices.push_back(z);
            vertices.push_back(1.0f); vertices.push_back(1.0f); vertices.push_back(1.0f);
        }
    }

    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);

    glBindVertexArray(gridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Shaders (same as axis for simplicity)
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;
        out vec3 vertexColor;
        uniform mat4 view;
        uniform mat4 projection;
        void main() {
            gl_Position = projection * view * vec4(aPos, 1.0);
            vertexColor = aColor;
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        in vec3 vertexColor;
        out vec4 FragColor;
        void main() {
            FragColor = vec4(vertexColor, 1.0);
        }
    )";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Grid Vertex Shader Compilation Failed: " << infoLog << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Grid Fragment Shader Compilation Failed: " << infoLog << std::endl;
    }

    gridShaderProgram = glCreateProgram();
    glAttachShader(gridShaderProgram, vertexShader);
    glAttachShader(gridShaderProgram, fragmentShader);
    glLinkProgram(gridShaderProgram);
    glGetProgramiv(gridShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(gridShaderProgram, 512, nullptr, infoLog);
        std::cerr << "Grid Shader Program Linking Failed: " << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Renderer::draw(const Camera& camera, const StateManager& stateManager) {
    if (renderAxes) drawAxes(camera, stateManager);
    if (renderGrid) drawGrid(camera, stateManager);
    TiMath::Matrix4 view = camera.getViewMatrix();
    TiMath::Matrix4 proj = camera.getProjectionMatrix();
    drawTargetCamAim(camera);
}

void Renderer::drawAxes(const Camera& camera, const StateManager& stateManager) {
    glUseProgram(axisShaderProgram);
    glBindVertexArray(axisVAO);

    GLint viewLoc = glGetUniformLocation(axisShaderProgram, "view");
    GLint projLoc = glGetUniformLocation(axisShaderProgram, "projection");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, camera.getViewMatrix().data());
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, camera.getProjectionMatrix().data());

    glDrawArrays(GL_LINES, 0, 6);

    glBindVertexArray(0);
}

void Renderer::drawGrid(const Camera& camera, const StateManager& stateManager) {
    glUseProgram(gridShaderProgram);
    glBindVertexArray(gridVAO);

    GLint viewLoc = glGetUniformLocation(gridShaderProgram, "view");
    GLint projLoc = glGetUniformLocation(gridShaderProgram, "projection");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, camera.getViewMatrix().data());
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, camera.getProjectionMatrix().data());

    glDrawArrays(GL_LINES, 0, (gridLines * 2 + 1) * 4);

    glBindVertexArray(0);
}

void Renderer::drawTargetCamAim(const Camera& camera) const {
    targetCamAim.render(camera.getViewMatrix(), camera.getProjectionMatrix());
}

void Renderer::setRenderFlags(bool axes, bool grid) {
    renderAxes = axes;
    renderGrid = grid;
}

void Renderer::increaseGridSize(float delta) {
    gridSize += delta;
    setGridParameters(gridLines, gridSpacing, gridSize);
}

void Renderer::decreaseGridSize(float delta) {
    gridSize = std::max(0.0f, gridSize - delta);
    setGridParameters(gridLines, gridSpacing, gridSize);
}

void Renderer::increaseGridLines(int delta) {
    gridLines += delta;
    setGridParameters(gridLines, gridSpacing, gridSize);
}

void Renderer::decreaseGridLines(int delta) {
    gridLines = std::max(0, gridLines - delta);
    setGridParameters(gridLines, gridSpacing, gridSize);
}

} // namespace Ti3D