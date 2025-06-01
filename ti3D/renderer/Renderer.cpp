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
    uMVPLocation = glGetUniformLocation(program, "uMVP");
    uColorLocation = glGetUniformLocation(program, "uColor");
    return program;
}

void Renderer::initGrid(Camera::ViewMode viewMode) {
    std::vector<float> gridVertices;
    float halfSize = gridSize / 2.0f;
    int numLines = gridLines;

    if (viewMode == Camera::ViewMode::Left || viewMode == Camera::ViewMode::Right) {
        // YZ plane (normal along X)
        for (int i = -numLines; i <= numLines; ++i) {
            float y = i * gridSpacing;
            // Line along Z
            gridVertices.push_back(0.0f); gridVertices.push_back(y); gridVertices.push_back(-halfSize); gridVertices.push_back(1.0f);
            gridVertices.push_back(0.0f); gridVertices.push_back(y); gridVertices.push_back(halfSize); gridVertices.push_back(1.0f);
            // Line along Y
            float z = i * gridSpacing;
            gridVertices.push_back(0.0f); gridVertices.push_back(-halfSize); gridVertices.push_back(z); gridVertices.push_back(1.0f);
            gridVertices.push_back(0.0f); gridVertices.push_back(halfSize); gridVertices.push_back(z); gridVertices.push_back(1.0f);
        }
    } else if (viewMode == Camera::ViewMode::Far) {
        // XZ plane (normal along Y)
        for (int i = -numLines; i <= numLines; ++i) {
            float x = i * gridSpacing;
            // Line along Z
            gridVertices.push_back(x); gridVertices.push_back(0.0f); gridVertices.push_back(-halfSize); gridVertices.push_back(1.0f);
            gridVertices.push_back(x); gridVertices.push_back(0.0f); gridVertices.push_back(halfSize); gridVertices.push_back(1.0f);
            // Line along X
            float z = i * gridSpacing;
            gridVertices.push_back(-halfSize); gridVertices.push_back(0.0f); gridVertices.push_back(z); gridVertices.push_back(1.0f);
            gridVertices.push_back(halfSize); gridVertices.push_back(0.0f); gridVertices.push_back(z); gridVertices.push_back(1.0f);
        }
    } else {
        // XY plane (normal along Z) for Top/Bottom/Perspective
        for (int i = -numLines; i <= numLines; ++i) {
            float x = i * gridSpacing;
            // Line along Y
            gridVertices.push_back(x); gridVertices.push_back(-halfSize); gridVertices.push_back(0.0f); gridVertices.push_back(1.0f);
            gridVertices.push_back(x); gridVertices.push_back(halfSize); gridVertices.push_back(0.0f); gridVertices.push_back(1.0f);
            // Line along X
            float y = i * gridSpacing;
            gridVertices.push_back(-halfSize); gridVertices.push_back(y); gridVertices.push_back(0.0f); gridVertices.push_back(1.0f);
            gridVertices.push_back(halfSize); gridVertices.push_back(y); gridVertices.push_back(0.0f); gridVertices.push_back(1.0f);
        }
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

Renderer::Renderer(float axisLength, float gridSize, int gridLines, float gridSpacing)
    : shaderProgram(0), axesVAO(0), axesVBO(0), gridVAO(0), gridVBO(0),
      axisLength(std::max(0.1f, axisLength)), gridSize(std::max(1.0f, gridSize)),
      gridLines(std::max(1, gridLines)), gridSpacing(std::max(0.1f, gridSpacing)),
      renderAxes(true), renderGrid(true) {
    shaderProgram = createShaderProgram();
    if (shaderProgram == 0) {
        std::cerr << "Failed to create shader program. Rendering disabled." << std::endl;
        return;
    }
    // Initialize axes
    float axisVertices[] = {
        0.0f, 0.0f, 0.0f, 1.0f,  axisLength, 0.0f, 0.0f, 1.0f, // X-axis
        0.0f, 0.0f, 0.0f, 1.0f,  0.0f, axisLength, 0.0f, 1.0f, // Y-axis
        0.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f, axisLength, 1.0f  // Z-axis
    };
    glGenVertexArrays(1, &axesVAO);
    glGenBuffers(1, &axesVBO);
    glBindVertexArray(axesVAO);
    glBindBuffer(GL_ARRAY_BUFFER, axesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertices), axisVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // Initialize grid (default to Far view for initialization)
    initGrid(Camera::ViewMode::Far);
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
        0.0f, 0.0f, 0.0f, 1.0f,  axisLength, 0.0f, 0.0f, 1.0f, // X-axis
        0.0f, 0.0f, 0.0f, 1.0f,  0.0f, axisLength, 0.0f, 1.0f, // Y-axis
        0.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f, axisLength, 1.0f  // Z-axis
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
    initGrid(Camera::ViewMode::Far); // Default for reinitialization
    checkGLError("Renderer reinitialization");
}

void Renderer::drawAxes(const Camera& camera) {
    if (shaderProgram == 0) return;

    glUseProgram(shaderProgram);
    TiMath::Matrix4 mvp = camera.getProjectionMatrix() * camera.getViewMatrix();
    TiMath::Matrix4 model = TiMath::Matrix4::getIdentity(); // Centered at origin

    // Reinitialize grid if view mode changed
    static Camera::ViewMode lastViewMode = Camera::ViewMode::Far;
    if (lastViewMode != camera.viewMode) {
        glDeleteVertexArrays(1, &gridVAO);
        glDeleteBuffers(1, &gridVBO);
        initGrid(camera.viewMode);
        lastViewMode = camera.viewMode;
    }

    if (renderAxes) {
        glBindVertexArray(axesVAO);
        glUniformMatrix4fv(uMVPLocation, 1, GL_FALSE, (mvp * model).toArray());
        struct { float r, g, b; } colors[] = {
            {1.0f, 0.0f, 0.0f}, // Red X
            {0.0f, 1.0f, 0.0f}, // Green Y
            {0.0f, 0.0f, 1.0f}  // Blue Z
        };
        for (int i = 0; i < 3; ++i) {
            glUniform3f(uColorLocation, colors[i].r, colors[i].g, colors[i].b);
            glDrawArrays(GL_LINES, i * 2, 2);
        }
    }

    if (renderGrid) {
        glBindVertexArray(gridVAO);
        glUniformMatrix4fv(uMVPLocation, 1, GL_FALSE, (mvp * model).toArray());
        glUniform3f(uColorLocation, 0.5f, 0.5f, 0.5f); // Gray grid
        glDrawArrays(GL_LINES, 0, 2 * (2 * gridLines + 1) * 2);
    }

    glBindVertexArray(0);
    checkGLError("drawAxes");
}

} // namespace Ti3D