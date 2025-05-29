#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../TiMath/Vector3.h"
#include "../TiMath/Quaternion.h"
#include "../TiMath/Matrix4.h"

namespace Ti3D {

/**
 * @class Camera
 * @brief A 3D camera class using quaternions for rotation.
 */
class Camera {
public:
    TiMath::Vector3 target;
    TiMath::Quaternion rotation;
    float distance;
    float fovYDegrees;
    float aspectRatio;
    float zNear, zFar;

    Camera()
        : target(0.0f, 0.0f, 0.0f), rotation(1.0f, 0.0f, 0.0f, 0.0f),
          distance(5.0f), fovYDegrees(45.0f), aspectRatio(4.0f / 3.0f),
          zNear(0.1f), zFar(100.0f) {}

    /** @brief Updates camera rotation based on elapsed time. */
    void update(float deltaTime, float rotationSpeed = 20.0f) {
        TiMath::Quaternion q = TiMath::Quaternion::fromAxisAngle(TiMath::Vector3(0.0f, 1.0f, 0.0f), rotationSpeed * deltaTime);
        rotation = (q * rotation).normalized();
    }

    /** @brief Processes keyboard input for zooming. */
    void processInput(GLFWwindow* window, float deltaTime) {
        const float distanceStep = 0.1f;
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
        if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
            distance = std::max(1.0f, distance - distanceStep);
        }
        if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
            distance += distanceStep;
        }
    }

    /** @brief Processes mouse input for rotation. */
    void processMouseInput(GLFWwindow* window, double xpos, double ypos) {
        static double lastX = 400.0, lastY = 300.0;
        float dx = static_cast<float>(xpos - lastX) * 0.1f;
        float dy = static_cast<float>(ypos - lastY) * 0.1f;
        lastX = xpos; lastY = ypos;

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            TiMath::Quaternion qx = TiMath::Quaternion::fromAxisAngle(TiMath::Vector3(0.0f, 1.0f, 0.0f), -dx);
            TiMath::Quaternion qy = TiMath::Quaternion::fromAxisAngle(TiMath::Vector3(1.0f, 0.0f, 0.0f), -dy);
            rotation = (qx * qy * rotation).normalized();
        }
    }

    /** @brief Returns the view matrix. */
    [[nodiscard]] TiMath::Matrix4 getViewMatrix() const {
        TiMath::Vector3 initialDirection(0.0f, 0.0f, -1.0f);
        TiMath::Matrix4 rotationMatrix = rotation.toMatrix();
        TiMath::Vector3 direction(
            rotationMatrix.m[0] * initialDirection.x + rotationMatrix.m[4] * initialDirection.y + rotationMatrix.m[8] * initialDirection.z,
            rotationMatrix.m[1] * initialDirection.x + rotationMatrix.m[5] * initialDirection.y + rotationMatrix.m[9] * initialDirection.z,
            rotationMatrix.m[2] * initialDirection.x + rotationMatrix.m[6] * initialDirection.y + rotationMatrix.m[10] * initialDirection.z
        );
        TiMath::Vector3 position = target + direction * distance;
        TiMath::Vector3 f = (target - position).normalized();
        TiMath::Vector3 up(0.0f, 1.0f, 0.0f);
        TiMath::Vector3 s = f.cross(up).normalized();
        TiMath::Vector3 u = s.cross(f);

        TiMath::Matrix4 view;
        view.m[0] = s.x; view.m[4] = s.y; view.m[8] = s.z; view.m[12] = 0.0f;
        view.m[1] = u.x; view.m[5] = u.y; view.m[9] = u.z; view.m[13] = 0.0f;
        view.m[2] = -f.x; view.m[6] = -f.y; view.m[10] = -f.z; view.m[14] = 0.0f;
        view.m[3] = 0.0f; view.m[7] = 0.0f; view.m[11] = 0.0f; view.m[15] = 1.0f;

        TiMath::Matrix4 translation = TiMath::Matrix4::translation(-position);
        return view * translation;
    }

    /** @brief Returns the projection matrix. */
    [[nodiscard]] TiMath::Matrix4 getProjectionMatrix() const {
        return TiMath::Matrix4::perspective(fovYDegrees, aspectRatio, zNear, zFar);
    }

    /** @brief Sets the aspect ratio based on window dimensions. */
    void setAspectRatio(float width, float height) {
        aspectRatio = width / height;
    }
};

/**
 * @class Renderer
 * @brief Manages OpenGL rendering of coordinate axes.
 */
class Renderer {
private:
    unsigned int shaderProgram;
    unsigned int VAO, VBO;

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

public:
    Renderer() : shaderProgram(0), VAO(0), VBO(0) {
        shaderProgram = createShaderProgram();
        float axisVertices[] = {
            0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f
        };
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertices), axisVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }

    ~Renderer() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(shaderProgram);
    }

    void drawAxes(const Camera& camera) {
        glUseProgram(shaderProgram);
        TiMath::Matrix4 mvp = camera.getProjectionMatrix() * camera.getViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uMVP"), 1, GL_FALSE, mvp.m.data());
        glBindVertexArray(VAO);
        glUniform3f(glGetUniformLocation(shaderProgram, "uColor"), 1.0f, 0.0f, 0.0f);
        glDrawArrays(GL_LINES, 0, 2);
        glUniform3f(glGetUniformLocation(shaderProgram, "uColor"), 0.0f, 1.0f, 0.0f);
        glDrawArrays(GL_LINES, 2, 2);
        glUniform3f(glGetUniformLocation(shaderProgram, "uColor"), 0.0f, 0.0f, 1.0f);
        glDrawArrays(GL_LINES, 4, 2);
        glBindVertexArray(0);
    }
};

} // namespace Ti3D

#endif // CAMERA_H