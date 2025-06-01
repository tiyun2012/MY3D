#include "TargetCamAim.h"
#include <glad/glad.h>
#include <vector>
#include <cmath>
#include <iostream>

namespace Ti3D {

static const char* vertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)glsl";

static const char* fragmentShaderSource = R"glsl(
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0, 1.0, 0.0, 1.0); // Yellow for visibility
}
)glsl";

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

TargetCamAim::TargetCamAim(float radius, const TiMath::Vector3& pos)
    : position(pos), radius(radius), vao(0), vbo(0), shaderProgram(0) {
    initShader();
    initCircleMesh();
}

void TargetCamAim::setPosition(const TiMath::Vector3& pos) {
    position = pos;
}

const TiMath::Vector3& TargetCamAim::getPosition() const {
    return position;
}

void TargetCamAim::setRadius(float r) {
    radius = r;
    // Reinitialize mesh with new radius
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    initCircleMesh();
}

float TargetCamAim::getRadius() const {
    return radius;
}

void TargetCamAim::initShader() {
    shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
    if (!shaderProgram) {
        std::cerr << "Failed to create TargetCamAim shader program" << std::endl;
    }
}

void TargetCamAim::initCircleMesh() {
    std::vector<float> vertices;
    const int segments = 32;
    for (int i = 0; i <= segments; ++i) {
        float theta = 2.0f * float(M_PI) * float(i) / float(segments);
        vertices.push_back(radius * std::cos(theta));
        vertices.push_back(radius * std::sin(theta));
        vertices.push_back(0.0f);
    }
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void TargetCamAim::draw(const Camera& camera) {
    if (!shaderProgram) return;

    glUseProgram(shaderProgram);

    // Compute camera position based on Camera::getViewMatrix logic
    TiMath::Vector3 camPos;
    switch (camera.viewMode) {
        case Camera::ViewMode::Top:
            camPos = camera.target + TiMath::Vector3(0.0f, camera.distance, 0.0f);
            break;
        case Camera::ViewMode::Left:
            camPos = camera.target + TiMath::Vector3(-camera.distance, 0.0f, 0.0f);
            break;
        case Camera::ViewMode::Right:
            camPos = camera.target + TiMath::Vector3(camera.distance, 0.0f, 0.0f);
            break;
        case Camera::ViewMode::Bottom:
            camPos = camera.target + TiMath::Vector3(0.0f, -camera.distance, 0.0f);
            break;
        case Camera::ViewMode::Far:
            camPos = camera.target + TiMath::Vector3(0.0f, 0.0f, -camera.distance);
            break;
    }

    // Compute model matrix to orient circle to face camera
    TiMath::Vector3 toCam = (camPos - position).normalized();
    if (toCam.isZero()) {
        std::cerr << "Warning: Camera at target position, using default orientation" << std::endl;
        toCam = TiMath::Vector3(0.0f, 0.0f, 1.0f);
    }
    TiMath::Vector3 up = TiMath::Vector3(0.0f, 1.0f, 0.0f);
    TiMath::Vector3 right = toCam.cross(up).normalized();
    if (right.isZero()) {
        // If toCam is parallel to up, choose a different up vector
        up = TiMath::Vector3(0.0f, 0.0f, 1.0f);
        right = toCam.cross(up).normalized();
    }
    TiMath::Vector3 newUp = right.cross(toCam).normalized();

    // Create rotation matrix
    TiMath::Matrix4 rotation;
    rotation.m[0] = right.x;  rotation.m[4] = right.y;  rotation.m[8] = right.z;
    rotation.m[1] = newUp.x;  rotation.m[5] = newUp.y;  rotation.m[9] = newUp.z;
    rotation.m[2] = toCam.x;  rotation.m[6] = toCam.y;  rotation.m[10] = toCam.z;
    rotation.m[3] = 0.0f;     rotation.m[7] = 0.0f;     rotation.m[11] = 0.0f;    rotation.m[15] = 1.0f;

    // Combine with translation
    TiMath::Matrix4 model = TiMath::Matrix4::translation(position) * rotation;

    // Set uniforms
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
    if (modelLoc == -1 || viewLoc == -1 || projLoc == -1) {
        std::cerr << "Failed to get uniform location in TargetCamAim" << std::endl;
        return;
    }
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.data());
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, camera.getViewMatrix().data());
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, camera.getProjectionMatrix().data());

    glBindVertexArray(vao);
    glDrawArrays(GL_LINE_LOOP, 0, 33); // 32 segments + 1
    glBindVertexArray(0);
}

} // namespace Ti3D