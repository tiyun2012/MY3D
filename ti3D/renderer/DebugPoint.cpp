#include <glad/glad.h> // Added for OpenGL functions and constants
#include "DebugPoint.h"
#include <vector>
#include <cmath>
#include <iostream> // Added for std::cerr
#include "../TiMath/TiMathConfig.h"

namespace Ti3D {

void DebugPoint::initialize() {
    // Vertex data for a unit circle in XZ plane
    std::vector<float> vertices;
    std::vector<GLuint> indices;
    vertices.reserve(circleSegments * 3);
    indices.reserve(circleSegments * 3);

    // Generate circle vertices
    for (int i = 0; i < circleSegments; ++i) {
        float theta = 2.0f * TiMath::PI * static_cast<float>(i) / circleSegments;
        vertices.push_back(std::cos(theta)); // x
        vertices.push_back(0.0f);           // y
        vertices.push_back(std::sin(theta)); // z
    }

    // Generate indices for triangle fan
    for (int i = 1; i < circleSegments - 1; ++i) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    // Create VAO, VBO, EBO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // Simple vertex shader
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
        }
    )";

    // Simple fragment shader (red circle)
    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red color
        }
    )";

    // Compile shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    // Check compilation status
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex Shader Compilation Failed: " << infoLog << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    // Check compilation status
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment Shader Compilation Failed: " << infoLog << std::endl;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check linking status
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader Program Linking Failed: " << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void DebugPoint::render(const TiMath::Matrix4& viewMatrix, const TiMath::Matrix4& projectionMatrix) const {
    if (!visible) return;

    glUseProgram(shaderProgram);

    // Compute model matrix to orient the circle toward the camera
    TiMath::Vector3 cameraPos;
    TiMath::Matrix4 viewInv = viewMatrix.inverse();
    cameraPos = TiMath::Vector3(viewInv.m[12], viewInv.m[13], viewInv.m[14]);

    TiMath::Vector3 dir = (cameraPos - position).normalized();
    if (dir.isZero()) dir = TiMath::Vector3(0.0f, 0.0f, 1.0f); // Fallback
    TiMath::Vector3 up(0.0f, 1.0f, 0.0f);
    TiMath::Vector3 right = dir.cross(up).normalized();
    if (right.isZero()) right = TiMath::Vector3(1.0f, 0.0f, 0.0f);
    up = right.cross(dir).normalized();

    // Create model matrix
    TiMath::Matrix4 model;
    model.m[0] = right.x * size; model.m[4] = right.y * size; model.m[8] = right.z * size;
    model.m[1] = up.x * size;    model.m[5] = up.y * size;    model.m[9] = up.z * size;
    model.m[2] = -dir.x * size;  model.m[6] = -dir.y * size;  model.m[10] = -dir.z * size;
    model.m[12] = position.x;    model.m[13] = position.y;    model.m[14] = position.z;
    model.m[15] = 1.0f;

    // Set uniforms
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.data());
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, viewMatrix.data());
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, projectionMatrix.data());

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, circleSegments * 3 - 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

} // namespace Ti3D