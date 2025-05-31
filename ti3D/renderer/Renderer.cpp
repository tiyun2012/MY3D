#include "Renderer.h"

namespace Ti3D {

unsigned int Renderer::createShaderProgram() {
    const char* vertexShaderSource = R"glsl(
    #version 330 core
    layout(location = 0) in vec4 aPos;
    uniform mat4 uMVP;
    void main() { gl_Position = uMVP * aPos; }
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
        std::cerr << "Vertex Shader Compilation Failed: " << infoLog << std::endl;
        return 0;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment Shader Compilation Failed: " << infoLog << std::endl;
        glDeleteShader(vertexShader);
        return 0;
    }

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Shader Program Linking Failed: " << infoLog << std::endl;
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}

void Renderer::initGrid(float cameraDistance) {
    float size = cameraDistance * 2.0f; // Scale grid with distance
    float spacing = cameraDistance / 10.0f; // Adjust spacing
    std::vector<float> gridVertices;
    int numLines = static_cast<int>(size / spacing);
    float halfSize = size / 2.0f;

    // Generate grid lines along X-axis
    for (int i = -numLines; i <= numLines; ++i) {
        float z = i * spacing;
        gridVertices.push_back(-halfSize); // Start X
        gridVertices.push_back(0.0f);      // Y
        gridVertices.push_back(z);         // Z
        gridVertices.push_back(1.0f);      // W
        gridVertices.push_back(halfSize);  // End X
        gridVertices.push_back(0.0f);      // Y
        gridVertices.push_back(z);         // Z
        gridVertices.push_back(1.0f);      // W
    }

    // Generate grid lines along Z-axis
    for (int i = -numLines; i <= numLines; ++i) {
        float x = i * spacing;
        gridVertices.push_back(x);         // X
        gridVertices.push_back(0.0f);      // Y
        gridVertices.push_back(-halfSize); // Start Z
        gridVertices.push_back(1.0f);      // W
        gridVertices.push_back(x);         // X
        gridVertices.push_back(0.0f);      // Y
        gridVertices.push_back(halfSize);  // End Z
        gridVertices.push_back(1.0f);      // W
    }

    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);
    glBindVertexArray(gridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(float), gridVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void Renderer::checkGLError(const char* operation) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL Error during " << operation << ": " << err << std::endl;
    }
}

Renderer::Renderer() : shaderProgram(0), axesVAO(0), axesVBO(0), gridVAO(0), gridVBO(0) {
    shaderProgram = createShaderProgram();
    if (shaderProgram == 0) {
        std::cerr << "Failed to create shader program. Rendering disabled." << std::endl;
        return;
    }
    // Initialize axes
    float axisVertices[] = {
        0.0f, 0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f, // X-axis
        0.0f, 0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f, // Y-axis
        0.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f, 1.0f  // Z-axis
    };
    glGenVertexArrays(1, &axesVAO);
    glGenBuffers(1, &axesVBO);
    glBindVertexArray(axesVAO);
    glBindBuffer(GL_ARRAY_BUFFER, axesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertices), axisVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // Initialize grid
    initGrid(10.0f); // Default distance
    checkGLError("Renderer initialization");
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &axesVAO);
    glDeleteBuffers(1, &axesVBO);
    glDeleteVertexArrays(1, &gridVAO);
    glDeleteBuffers(1, &gridVBO);
    glDeleteProgram(shaderProgram);
}

void Renderer::reinitialize() {
    glDeleteVertexArrays(1, &axesVAO);
    glDeleteBuffers(1, &axesVBO);
    glDeleteVertexArrays(1, &gridVAO);
    glDeleteBuffers(1, &gridVBO);
    glDeleteProgram(shaderProgram);

    shaderProgram = createShaderProgram();
    if (shaderProgram == 0) {
        std::cerr << "Failed to create shader program. Rendering disabled." << std::endl;
        return;
    }
    // Reinitialize axes
    float axisVertices[] = {
        0.0f, 0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f, // X-axis
        0.0f, 0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f, // Y-axis
        0.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f, 1.0f  // Z-axis
    };
    glGenVertexArrays(1, &axesVAO);
    glGenBuffers(1, &axesVBO);
    glBindVertexArray(axesVAO);
    glBindBuffer(GL_ARRAY_BUFFER, axesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertices), axisVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // Reinitialize grid
    initGrid(10.0f);
    checkGLError("Renderer reinitialization");
}

void Renderer::drawAxes(const Camera& camera) {
    if (shaderProgram == 0) return;

    glUseProgram(shaderProgram);
    TiMath::Matrix4 mvp = camera.getProjectionMatrix() * camera.getViewMatrix();
    TiMath::Matrix4 gridModel = TiMath::Matrix4::translation(camera.target);
    TiMath::Matrix4 gridMVP = mvp * gridModel;

    // Reinitialize grid if distance changed significantly
    static float lastDistance = 0.0f;
    if (std::fabs(lastDistance - camera.distance) > 0.1f) {
        glDeleteVertexArrays(1, &gridVAO);
        glDeleteBuffers(1, &gridVBO);
        initGrid(camera.distance);
        lastDistance = camera.distance;
    }

    // Draw axes
    glBindVertexArray(axesVAO);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uMVP"), 1, GL_FALSE, mvp.toArray());
    glUniform3f(glGetUniformLocation(shaderProgram, "uColor"), 1.0f, 0.0f, 0.0f); // Red X-axis
    glDrawArrays(GL_LINES, 0, 2);
    glUniform3f(glGetUniformLocation(shaderProgram, "uColor"), 0.0f, 1.0f, 0.0f); // Green Y-axis
    glDrawArrays(GL_LINES, 2, 2);
    glUniform3f(glGetUniformLocation(shaderProgram, "uColor"), 0.0f, 0.0f, 1.0f); // Blue Z-axis
    glDrawArrays(GL_LINES, 4, 2);

    // Draw grid
    glBindVertexArray(gridVAO);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uMVP"), 1, GL_FALSE, gridMVP.toArray());
    glUniform3f(glGetUniformLocation(shaderProgram, "uColor"), 0.5f, 0.5f, 0.5f); // Gray grid
    int numLines = static_cast<int>(camera.distance * 2.0f / (camera.distance / 10.0f));
    glDrawArrays(GL_LINES, 0, 2 * (2 * numLines + 1) * 2);
    glBindVertexArray(0);

    checkGLError("drawAxes");
}

} // namespace Ti3D