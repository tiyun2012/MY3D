#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <vector>
#include "../TiMath/Matrix4.h"
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

    unsigned int createShaderProgram() {
        const char* vertexShaderSource = R"glsl(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        uniform mat4 uMVP;
        void main() { gl_Position = uMVP * vec4(aPos, 1.0); }
        )glsl";

        const char* fragmentShaderSource = R"glsl(
        #version 330 core
        uniform vec3 uColor;
        out vec4 FragColor;
        void main() { FragColor = vec4(uColor, 1.0); }
        )glsl";

        int success;
        char infoLog[512];

        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShader);
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
            throw std::runtime_error(std::string("Vertex Shader Compilation Failed: ") + infoLog);
        }

        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            throw std::runtime_error(std::string("Fragment Shader Compilation Failed: ") + infoLog);
        }

        unsigned int program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program, 512, nullptr, infoLog);
            throw std::runtime_error(std::string("Shader Program Linking Failed: ") + infoLog);
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return program;
    }

    void initGrid(float size, float spacing) {
        std::vector<float> gridVertices;
        int numLines = static_cast<int>(size / spacing);
        float halfSize = size / 2.0f;

        // Generate grid lines along X-axis
        for (int i = -numLines; i <= numLines; ++i) {
            float z = i * spacing;
            gridVertices.push_back(-halfSize); // Start X
            gridVertices.push_back(0.0f);      // Y
            gridVertices.push_back(z);         // Z
            gridVertices.push_back(halfSize);  // End X
            gridVertices.push_back(0.0f);      // Y
            gridVertices.push_back(z);         // Z
        }

        // Generate grid lines along Z-axis
        for (int i = -numLines; i <= numLines; ++i) {
            float x = i * spacing;
            gridVertices.push_back(x);         // X
            gridVertices.push_back(0.0f);      // Y
            gridVertices.push_back(-halfSize); // Start Z
            gridVertices.push_back(x);         // X
            gridVertices.push_back(0.0f);      // Y
            gridVertices.push_back(halfSize);  // End Z
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

public:
    Renderer() : shaderProgram(0), axesVAO(0), axesVBO(0), gridVAO(0), gridVBO(0) {
        shaderProgram = createShaderProgram();
        // Initialize axes
        float axisVertices[] = {
            0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f
        };
        glGenVertexArrays(1, &axesVAO);
        glGenBuffers(1, &axesVBO);
        glBindVertexArray(axesVAO);
        glBindBuffer(GL_ARRAY_BUFFER, axesVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertices), axisVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);

        // Initialize grid
        initGrid(10.0f, 1.0f); // 10x10 grid with 1-unit spacing
    }

    ~Renderer() {
        glDeleteVertexArrays(1, &axesVAO);
        glDeleteBuffers(1, &axesVBO);
        glDeleteVertexArrays(1, &gridVAO);
        glDeleteBuffers(1, &gridVBO);
        glDeleteProgram(shaderProgram);
    }

    void drawAxes(const Camera& camera) {
        glUseProgram(shaderProgram);
        TiMath::Matrix4 mvp = camera.getProjectionMatrix() * camera.getViewMatrix();
        // Optional: Translate grid to camera's target
        // TiMath::Matrix4 gridModel = TiMath::Matrix4::translation(camera.target);
        // TiMath::Matrix4 gridMVP = mvp * gridModel;

        // Draw axes
        glBindVertexArray(axesVAO);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uMVP"), 1, GL_FALSE, mvp.m.data());
        glUniform3f(glGetUniformLocation(shaderProgram, "uColor"), 1.0f, 0.0f, 0.0f); // Red X-axis
        glDrawArrays(GL_LINES, 0, 2);
        glUniform3f(glGetUniformLocation(shaderProgram, "uColor"), 0.0f, 1.0f, 0.0f); // Green Y-axis
        glDrawArrays(GL_LINES, 2, 2);
        glUniform3f(glGetUniformLocation(shaderProgram, "uColor"), 0.0f, 0.0f, 1.0f); // Blue Z-axis
        glDrawArrays(GL_LINES, 4, 2);

        // Draw grid
        glBindVertexArray(gridVAO);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uMVP"), 1, GL_FALSE, mvp.m.data()); // Use gridMVP if translating
        glUniform3f(glGetUniformLocation(shaderProgram, "uColor"), 0.5f, 0.5f, 0.5f); // Gray grid
        glDrawArrays(GL_LINES, 0, 2 * (2 * static_cast<int>(10.0f / 1.0f) + 1) * 2);
        glBindVertexArray(0);
    }
};

} // namespace Ti3D

#endif // RENDERER_H