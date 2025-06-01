#include "Renderer.h"
#include "../TiMath/Vector3.h"
#include <vector>
#include <string>
#include <iostream>
#include <GLFW/glfw3.h> // For GL types

namespace Ti3D {

static const char* axisVertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
uniform mat4 view;
uniform mat4 projection;
out vec3 vColor;
void main() {
    gl_Position = projection * view * vec4(aPos, 1.0);
    vColor = aColor;
}
)glsl";

static const char* axisFragmentShaderSource = R"glsl(
#version 330 core
in vec3 vColor;
out vec4 FragColor;
void main() {
    FragColor = vec4(vColor, 1.0);
}
)glsl";

static const char* gridVertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 view;
uniform mat4 projection;
void main() {
    gl_Position = projection * view * vec4(aPos, 1.0);
}
)glsl";

static const char* gridFragmentShaderSource = R"glsl(
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(0.5, 0.5, 0.5, 1.0); // Gray grid
}
)glsl";

static void checkGLError(const std::string& context) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error at " << context << ": " << err << std::endl;
    }
}

static GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed (" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "): " << infoLog << std::endl;
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

static GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource) {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    if (!vertexShader || !fragmentShader) {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return 0;
    }
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Program linking failed: " << infoLog << std::endl;
        glDeleteProgram(program);
        program = 0;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}

Renderer::Renderer(float axisLength, float gridSize, int gridLines, float gridSpacing)
    : renderAxes(true), renderGrid(true), axisLength(axisLength),
      gridSize(gridSize), gridLines(gridLines), gridSpacing(gridSpacing) {
    // Validate inputs
    if (gridSize <= 0.0f) gridSize = 1.0f;
    if (gridLines < 1) gridLines = 1;
    if (gridSpacing <= 0.0f) gridSpacing = gridSize / static_cast<float>(gridLines);

    initAxis();
    initGrid();
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &axisVAO);
    glDeleteBuffers(1, &axisVBO);
    glDeleteVertexArrays(1, &gridVAO);
    glDeleteBuffers(1, &gridVBO);
    glDeleteProgram(axisShaderProgram);
    glDeleteProgram(gridShaderProgram);
}

void Renderer::initAxis() {
    axisShaderProgram = createShaderProgram(axisVertexShaderSource, axisFragmentShaderSource);
    if (!axisShaderProgram) {
        std::cerr << "Failed to create axis shader program" << std::endl;
        return;
    }
    std::vector<float> axisVertices = {
        // X-axis (red)
        0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
        axisLength, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
        // Y-axis (green)
        0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
        0.0f, axisLength, 0.0f,  0.0f, 1.0f, 0.0f,
        // Z-axis (blue)
        0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, axisLength,  0.0f, 0.0f, 1.0f
    };
    glGenVertexArrays(1, &axisVAO);
    glGenBuffers(1, &axisVBO);
    glBindVertexArray(axisVAO);
    glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
    glBufferData(GL_ARRAY_BUFFER, axisVertices.size() * sizeof(float), axisVertices.data(), GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

void Renderer::initGrid() {
    gridShaderProgram = createShaderProgram(gridVertexShaderSource, gridFragmentShaderSource);
    if (!gridShaderProgram) {
        std::cerr << "Failed to create grid shader program" << std::endl;
        return;
    }
    std::vector<float> gridVertices;
    // X-axis lines
    for (int i = -gridLines; i <= gridLines; ++i) {
        float z = i * gridSpacing;
        gridVertices.insert(gridVertices.end(), {
            -gridSize, 0.0f, z,
            gridSize, 0.0f, z
        });
    }
    // Z-axis lines
    for (int i = -gridLines; i <= gridLines; ++i) {
        float x = i * gridSpacing;
        gridVertices.insert(gridVertices.end(), {
            x, 0.0f, -gridSize,
            x, 0.0f, gridSize
        });
    }
    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);
    glBindVertexArray(gridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(float), gridVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void Renderer::drawAxes(const Camera& camera) {
    if (!renderAxes) return;
    glUseProgram(axisShaderProgram);
    glBindVertexArray(axisVAO);
    TiMath::Matrix4 view = camera.getViewMatrix();
    TiMath::Matrix4 projection = camera.getProjectionMatrix();
    GLint viewLoc = glGetUniformLocation(axisShaderProgram, "view");
    GLint projLoc = glGetUniformLocation(axisShaderProgram, "projection");
    if (viewLoc == -1 || projLoc == -1) {
        std::cerr << "Failed to get uniform location: view=" << viewLoc << ", projection=" << projLoc << std::endl;
        glBindVertexArray(0);
        return;
    }
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.data());
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection.data());
    checkGLError("After setting axis uniforms");
    // Debug: Log matrix values
    std::cout << "Axis view matrix: ";
    for (int i = 0; i < 16; ++i) std::cout << view.data()[i] << " ";
    std::cout << "\n";
    glDrawArrays(GL_LINES, 0, 6); // X, Y, Z axes (2 vertices each)
    checkGLError("After drawing axes");
    glBindVertexArray(0);
}

void Renderer::drawGrid(const Camera& camera) {
    if (!renderGrid) return;
    glUseProgram(gridShaderProgram);
    glBindVertexArray(gridVAO);
    TiMath::Matrix4 view = camera.getViewMatrix();
    TiMath::Matrix4 projection = camera.getProjectionMatrix();
    GLint viewLoc = glGetUniformLocation(gridShaderProgram, "view");
    GLint projLoc = glGetUniformLocation(gridShaderProgram, "projection");
    if (viewLoc == -1 || projLoc == -1) {
        std::cerr << "Failed to get uniform location: view=" << viewLoc << ", projection=" << projLoc << std::endl;
        glBindVertexArray(0);
        return;
    }
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.data());
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection.data());
    checkGLError("After setting grid uniforms");
    // Debug: Log matrix values
    std::cout << "Grid view matrix: ";
    for (int i = 0; i < 16; ++i) std::cout << view.data()[i] << " ";
    std::cout << "\n";
    // Total vertices: (2 * (2 * gridLines + 1)) for X-lines + (2 * (2 * gridLines + 1)) for Z-lines
    glDrawArrays(GL_LINES, 0, 4 * (2 * gridLines + 1));
    checkGLError("After drawing grid");
    glBindVertexArray(0);
}

void Renderer::setRenderFlags(bool renderAxes, bool renderGrid) {
    this->renderAxes = renderAxes;
    this->renderGrid = renderGrid;
}

void Renderer::setGridParameters(float gridSize, int gridLines) {
    if (gridSize <= 0.0f) gridSize = 1.0f;
    if (gridLines < 1) gridLines = 1;
    this->gridSize = gridSize;
    this->gridLines = gridLines;
    this->gridSpacing = gridSize / static_cast<float>(gridLines);
    glDeleteVertexArrays(1, &gridVAO);
    glDeleteBuffers(1, &gridVBO);
    initGrid();
}

void Renderer::increaseGridSize(float increment) {
    gridSize += increment;
    if (gridSize <= 0.0f) gridSize = 1.0f;
    gridSpacing = gridSize / static_cast<float>(gridLines);
    glDeleteVertexArrays(1, &gridVAO);
    glDeleteBuffers(1, &gridVBO);
    initGrid();
    std::cout << "Grid size increased to: " << gridSize << "\n";
}

void Renderer::decreaseGridSize(float decrement) {
    gridSize -= decrement;
    if (gridSize <= 0.0f) gridSize = 1.0f;
    gridSpacing = gridSize / static_cast<float>(gridLines);
    glDeleteVertexArrays(1, &gridVAO);
    glDeleteBuffers(1, &gridVBO);
    initGrid();
    std::cout << "Grid size decreased to: " << gridSize << "\n";
}

void Renderer::increaseGridLines(int increment) {
    gridLines += increment;
    if (gridLines < 1) gridLines = 1;
    gridSpacing = gridSize / static_cast<float>(gridLines);
    glDeleteVertexArrays(1, &gridVAO);
    glDeleteBuffers(1, &gridVBO);
    initGrid();
    std::cout << "Grid lines increased to: " << gridLines << "\n";
}

void Renderer::decreaseGridLines(int decrement) {
    gridLines -= decrement;
    if (gridLines < 1) gridLines = 1;
    gridSpacing = gridSize / static_cast<float>(gridLines);
    glDeleteVertexArrays(1, &gridVAO);
    glDeleteBuffers(1, &gridVBO);
    initGrid();
    std::cout << "Grid lines decreased to: " << gridLines << "\n";
}

} // namespace Ti3D