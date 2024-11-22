#pragma region rbf_quat_lerp
// ===========
// RBF update
// ===========
#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm> // For std::min and std::max
#include <fstream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLFW
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include <Eigen/Dense>
#include <Eigen/Geometry>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Custom clamp function for pre-C++17 compilers
template <typename T>
T clamp(const T& v, const T& lo, const T& hi) {
    return (v < lo) ? lo : (v > hi) ? hi : v;
}

// ===
// RBF
// ===
// Function to compute Gaussian RBF
double gaussianRBF(double distance, double sigma) {
    return std::exp(-(distance * distance) / (2 * sigma * sigma));
}

// Function to interpolate quaternions using RBF
Eigen::Quaterniond rbfInterpolate(const std::vector<Eigen::Quaterniond>& quaternions,
                                  const std::vector<Eigen::Vector3d>& positions,
                                  const Eigen::Vector3d& targetPosition,
                                  double sigma) {
    size_t n = quaternions.size();
    std::vector<double> weights(n);

    // Compute weights using Gaussian RBF
    for (size_t i = 0; i < n; ++i) {
        double distance = (positions[i] - targetPosition).norm();
        weights[i] = gaussianRBF(distance, sigma);
    }

    // Normalize weights
    double weightSum = std::accumulate(weights.begin(), weights.end(), 0.0);
    for (double& weight : weights) {
        weight /= weightSum;
    }

    // Interpolate quaternions
    Eigen::Quaterniond result = Eigen::Quaterniond::Identity();
    for (size_t i = 0; i < n; ++i) {
        result = result.slerp(weights[i], quaternions[i]);
    }

    return result;
}

int RBF_lerp_quat(glm::vec3 pos) {
    // Example quaternions and positions
    std::vector<Eigen::Quaterniond> quaternions = {
        Eigen::Quaterniond(1, 0, 0, 0),
        Eigen::Quaterniond(0, 1, 0, 0),
        Eigen::Quaterniond(0, 0, 1, 0)
    };
    std::vector<Eigen::Vector3d> positions = {
        Eigen::Vector3d(0, 0, 0),
        Eigen::Vector3d(1, 0, 0),
        Eigen::Vector3d(0, 1, 0)
    };
    Eigen::Vector3d targetPosition(pos[0],pos[1],pos[2]);

    // Perform RBF interpolation
    Eigen::Quaterniond interpolatedQuat = rbfInterpolate(quaternions, positions, targetPosition, 1.0);

    std::cout << "Interpolated Quaternion: " << interpolatedQuat.coeffs().transpose() << std::endl;

    return 0;
}

// =====
// ASSET
// =====
static std::vector<GLfloat> generateSphere(int stacks, int slices, double radius) {
  // std::vector<Eigen::Vector3d> vertices;
  std::vector<GLfloat> P;
  for (int i = 0; i <= stacks; ++i) {
      double lat = M_PI * (-0.5 + static_cast<double>(i) / stacks);
      double sinLat = sin(lat);
      double cosLat = cos(lat);

      for (int j = 0; j <= slices; ++j) {
          double lon = 2 * M_PI * static_cast<double>(j) / slices;
          double sinLon = sin(lon);
          double cosLon = cos(lon);

          double x = cosLon * cosLat;
          double y = sinLat;
          double z = sinLon * cosLat;
          // vertices.emplace_back(radius * x, radius * y, radius * z);
          P.push_back(radius * x);
          P.push_back(radius * y);
          P.push_back(radius * z);
      }
  }

  // Eigen::MatrixXd sphereVertices(vertices.size(), 3);
  // for (size_t i = 0; i < vertices.size(); ++i) {
  //     sphereVertices.row(static_cast<Eigen::Index>(i)) = vertices[i];
  // }

  return P;
}

class TriangleMesh {
public:
TriangleMesh();
void draw();
~TriangleMesh();

private:
unsigned int EBO, VAO, vertex_count, attrNum;
std::vector<GLuint> VBOs;
};

TriangleMesh::TriangleMesh() {

    std::vector<GLfloat> P =
    {
        -1.f, -1.f, 0.f,
         1.f, -1.f, 0.f,
         1.f,  1.f, 0.f,
        -1.f,  1.f, 0.f
    };

    std::vector<GLfloat> uv =
    {
        0.f , 0.f,
        1.f , 0.f,
        1.f , 1.f,
        0.f , 1.f
    };

    std::vector<GLfloat> Cd =
    {
        1.f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 0.f, 1.f
    };

    vertex_count = 4;
    attrNum = 3;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //IBO data
    std::vector<GLuint> indexData = { 0, 1, 2, 3};

    VBOs.resize(attrNum);
    glGenBuffers( attrNum, VBOs.data() );

    //Create VBO
    glBindBuffer( GL_ARRAY_BUFFER, VBOs[0] );
    glBufferData( GL_ARRAY_BUFFER, P.size() * sizeof(GLfloat), P.data(), GL_STATIC_DRAW);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * 4, (void*)0 );
    glEnableVertexAttribArray( 0 );

    glBindBuffer( GL_ARRAY_BUFFER, VBOs[1] );
    glBufferData( GL_ARRAY_BUFFER, uv.size() * sizeof(GLfloat), uv.data(), GL_STATIC_DRAW);
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 2 * 4, (void*)0 );
    glEnableVertexAttribArray( 1 );

    glBindBuffer( GL_ARRAY_BUFFER, VBOs[2] );
    glBufferData( GL_ARRAY_BUFFER, Cd.size() * sizeof(GLfloat), Cd.data(), GL_STATIC_DRAW);
    glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, 3 * 4, (void*)0 );
    glEnableVertexAttribArray( 2 );

    //Create IBO
    glGenBuffers( 1, &EBO );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(GLuint), indexData.data(), GL_STATIC_DRAW );
}

void TriangleMesh::draw() {
    glBindVertexArray(0);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLE_FAN, vertex_count, GL_UNSIGNED_INT, NULL);
}

TriangleMesh::~TriangleMesh() {
    for(int i = 0; i<attrNum; i++)
        glDisableVertexAttribArray(i);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(vertex_count, VBOs.data());
}

class thuyPoint {
public:
    thuyPoint();
    void draw();
    ~thuyPoint();
    void updateBuffer(); //todo

private:
    unsigned int EBO, VAO, vertex_count, attrNum;
    std::vector<GLuint> VBOs;
};

thuyPoint::thuyPoint() {
  updateBuffer();
}

void thuyPoint::updateBuffer()
{
  std::vector<GLfloat> P =
  {
      -1.f, -1.f, 0.f,
      1.f, -1.f, 0.f,
      1.f,  1.f, 0.f,
      -1.f,  1.f, 0.f
  };

  std::vector<GLfloat> uv =
  {
      0.f , 0.f,
      1.f , 0.f,
      1.f , 1.f,
      0.f , 1.f
  };

  std::vector<GLfloat> Cd =
  {
      1.f, 0.f, 0.f,
      0.f, 1.f, 0.f,
      0.f, 1.f, 0.f,
      0.f, 0.f, 1.f
  };

  // Generate a sphere
  P = generateSphere(20, 20, 1.0f);

  vertex_count = P.size()/3;
  std::cout<< vertex_count << std::endl;
  attrNum = 3;

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  //IBO data
  std::vector<GLuint> indexData = { 0, 1, 2, 3};

  uv.clear();
  Cd.clear();
  indexData.clear();
  for (size_t i = 0; i < vertex_count; i++)
  {
    uv.push_back(1.0f);
    uv.push_back(1.0f);
    Cd.push_back(P[i*3+0]);
    Cd.push_back(P[i*3+1]);
    Cd.push_back(P[i*3+2]);
    indexData.push_back((GLuint)i);
  }

  VBOs.resize(attrNum);
  glGenBuffers( attrNum, VBOs.data() );

  //Create VBO
  glBindBuffer( GL_ARRAY_BUFFER, VBOs[0] );
  glBufferData( GL_ARRAY_BUFFER, P.size() * sizeof(GLfloat), P.data(), GL_STATIC_DRAW);
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * 4, (void*)0 );
  glEnableVertexAttribArray( 0 );

  glBindBuffer( GL_ARRAY_BUFFER, VBOs[1] );
  glBufferData( GL_ARRAY_BUFFER, uv.size() * sizeof(GLfloat), uv.data(), GL_STATIC_DRAW);
  glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 2 * 4, (void*)0 );
  glEnableVertexAttribArray( 1 );

  glBindBuffer( GL_ARRAY_BUFFER, VBOs[2] );
  glBufferData( GL_ARRAY_BUFFER, Cd.size() * sizeof(GLfloat), Cd.data(), GL_STATIC_DRAW);
  glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, 3 * 4, (void*)0 );
  glEnableVertexAttribArray( 2 );

  //Create IBO
  glGenBuffers( 1, &EBO );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(GLuint), indexData.data(), GL_STATIC_DRAW );
}

void thuyPoint::draw() {
  glBindVertexArray(0);
  glBindVertexArray(VAO);
  glPointSize(15.0f);
  glDrawElements(GL_POINTS, vertex_count, GL_UNSIGNED_INT, NULL);
}

thuyPoint::~thuyPoint() {
  for(int i = 0; i<attrNum; i++)
    glDisableVertexAttribArray(i);
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(vertex_count, VBOs.data());
}

class thuyXfo {
public:
    thuyXfo();
    void draw();
    void drawI();
    ~thuyXfo();

private:
    unsigned int EBO, VAO, vertex_count, attrNum;
    std::vector<GLuint> VBOs;
};

thuyXfo::thuyXfo() {

  std::vector<GLfloat> P =
      {
        // X axis (red)
        0.0f, 0.0f, 0.0f, // Origin to (1, 0, 0)
        1.0f, 0.0f, 0.0f,
        // Y axis (green)
        0.0f, 0.0f, 0.0f, // Origin to (0, 1, 0)
        0.0f, 1.0f, 0.0f,
        // Z axis (blue)
        0.0f, 0.0f, 0.0f, // Origin to (0, 0, 1)
        0.0f, 0.0f, 1.0f};

  std::vector<GLfloat> uv =
      {
        0.f, 0.f,
        1.f, 0.f,
        1.f, 1.f,
        0.f, 1.f,
        0.f, 1.f,
        0.f, 1.f};

  std::vector<GLfloat> Cd =
      {
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f};

  vertex_count = 6;
  attrNum = 3;

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // IBO data
  std::vector<GLuint> indexData = {0, 1, 2, 3, 4, 5};

  VBOs.resize(attrNum);
  glGenBuffers(attrNum, VBOs.data());

  // Create VBO
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
  glBufferData(GL_ARRAY_BUFFER, P.size() * sizeof(GLfloat), P.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * 4, (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
  glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(GLfloat), uv.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * 4, (void *)0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
  glBufferData(GL_ARRAY_BUFFER, Cd.size() * sizeof(GLfloat), Cd.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * 4, (void *)0);
  glEnableVertexAttribArray(2);

  // Create IBO
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(GLuint), indexData.data(), GL_STATIC_DRAW);


  glm::vec3 instancePositions[] = {
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(2.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 2.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 2.0f)
  };

  GLuint instanceVBO;
  glGenBuffers(1, &instanceVBO);
  glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(instancePositions), instancePositions, GL_STATIC_DRAW);

  glBindVertexArray(VAO);
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glVertexAttribDivisor(3, 1); // Tell OpenGL this is an instanced vertex attribute
  glBindVertexArray(0);
}

void thuyXfo::draw()
{
  glBindVertexArray(0);
  glBindVertexArray(VAO);
  glDrawElements(GL_LINES, vertex_count, GL_UNSIGNED_INT, NULL);
}

void thuyXfo::drawI()
{
  glBindVertexArray(0);
  glBindVertexArray(VAO);
  glDrawArraysInstanced(GL_LINES, 0, 6, 4); // Draw 4 instances of the axes
}

thuyXfo::~thuyXfo()
{
  for (int i = 0; i < attrNum; i++)
    glDisableVertexAttribArray(i);
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(vertex_count, VBOs.data());
}

// ======
// OPENGL
// ======
TriangleMesh *triangle;
thuyXfo *gnomon;
thuyPoint *testSphere;
GLuint renderProg;
GLuint testProg;
GLuint bufWIDTH, bufHEIGHT;
GLFWwindow *glfw_window = nullptr;
const GLuint WIDTH = 800, HEIGHT = 800;
GLuint FBO;
GLuint renderTex_id;
GLuint tempTex_id;
GLuint paintTex_id;

// Camera control variables
float cameraDistance = 5.0f;
float cameraAngleX = 0.0f;
float cameraAngleY = 0.0f;
float lastX = 400, lastY = 300;
bool firstMouse = true;
float panX = 0.0f, panY = 0.0f;
bool leftMouseButtonPressed = false;
bool rightMouseButtonPressed = false;

glm::mat4 u_prj;
glm::mat4 u_viw;
glm::mat4 u_prj2D;
glm::mat4 u_viw2D;
Eigen::MatrixXd verticesSphere;

// Mouse callback function
static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
  if (ImGui::GetIO().WantCaptureMouse) {
      firstMouse = true; // Reset firstMouse to ignore sudden jumps
      return;
  }
  if (firstMouse) {
      lastX = static_cast<float>(xpos);
      lastY = static_cast<float>(ypos);
      firstMouse = false;
  }

  float xoffset = static_cast<float>(xpos) - lastX;
  float yoffset = lastY - static_cast<float>(ypos); // Reversed since y-coordinates go from bottom to top

  lastX = static_cast<float>(xpos);
  lastY = static_cast<float>(ypos);

  float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  if (leftMouseButtonPressed) {
      cameraAngleY += xoffset;
      cameraAngleX += yoffset;

      cameraAngleX = clamp(cameraAngleX, -89.0f, 89.0f);
  }

  if (rightMouseButtonPressed) {
      panX += xoffset * 0.1f;
      panY += yoffset * 0.1f;
  }
}

// Mouse button callback function
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
  if (ImGui::GetIO().WantCaptureMouse)
      return;
  if (button == GLFW_MOUSE_BUTTON_LEFT) {
      leftMouseButtonPressed = (action == GLFW_PRESS);
  }

  if (button == GLFW_MOUSE_BUTTON_RIGHT) {
      rightMouseButtonPressed = (action == GLFW_PRESS);
  }
}

// Scroll callback function
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  if (ImGui::GetIO().WantCaptureMouse)
      return;
  cameraDistance -= static_cast<float>(yoffset);
  cameraDistance = clamp(cameraDistance, 1.0f, 100.0f);
}

void printProgramLog(GLuint program)
{
  // Make sure name is shader
  if (glIsProgram(program))
  {
    // Program log length
    int infoLogLength = 0;
    int maxLength = infoLogLength;

    // Get info string length
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

    // Allocate string
    char *infoLog = new char[maxLength];

    // Get info log
    glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
    if (infoLogLength > 0)
    {
      // Print Log
      printf("%s\n", infoLog);
    }

    // Deallocate string
    delete[] infoLog;
  }
  else
  {
    printf("Name %d is not a program\n", program);
  }
}

void printShaderLog(GLuint shader)
{
  // Make sure name is shader
  if (glIsShader(shader))
  {
    // Shader log length
    int infoLogLength = 0;
    int maxLength = infoLogLength;

    // Get info string length
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

    // Allocate string
    char *infoLog = new char[maxLength];

    // Get info log
    glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
    if (infoLogLength > 0)
    {
      // Print Log
      printf("%s\n", infoLog);
    }

    // Deallocate string
    delete[] infoLog;
  }
  else
  {
    printf("Name %d is not a shader\n", shader);
  }
}

GLuint loadShaderFromFile(std::string path, GLenum shaderType)
{
  GLuint shaderID = 0;
  std::string shaderString;
  std::ifstream sourceFile(path.c_str());
  if (sourceFile)
  {
    shaderString.assign((std::istreambuf_iterator<char>(sourceFile)), std::istreambuf_iterator<char>());
    shaderID = glCreateShader(shaderType);
    const GLchar *shaderSource = shaderString.c_str();
    glShaderSource(shaderID, 1, (const GLchar **)&shaderSource, NULL);
    glCompileShader(shaderID);
    GLint shaderCompiled = GL_FALSE;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompiled);
    if (shaderCompiled != GL_TRUE)
    {
      printf("Unable to compile shader %d!\n\nSource:\n%s\n", shaderID, shaderSource);
      printf("Source is \n %s\n", shaderString.c_str());
      printShaderLog(shaderID);
      glDeleteShader(shaderID);
      shaderID = 0;
    }
  }
  else
  {
    printf("Unable to open file %s\n", path.c_str());
  }
  return shaderID;
}

GLuint add_shader(GLuint program, std::string path, GLenum type)
{

  // load shader from file
  GLuint current_shader = loadShaderFromFile(path, type);
  glAttachShader(program, current_shader);

  return current_shader;
}

GLuint add_shader(GLuint program, const char *shader_code, GLenum type)
{
  GLuint current_shader = glCreateShader(type);

  const GLchar *code[1];
  code[0] = shader_code;

  GLint code_length[1];
  code_length[0] = strlen(shader_code);

  glShaderSource(current_shader, 1, code, code_length);
  glCompileShader(current_shader);

  GLint result = 0;
  GLchar log[1024] = {0};

  glGetShaderiv(current_shader, GL_COMPILE_STATUS, &result);
  if (!result)
  {
    glGetShaderInfoLog(current_shader, sizeof(log), NULL, log);
    printShaderLog(current_shader);
    std::cout << "Error compiling " << type << " shader: " << log << "\n";
    return 0;
  }
  glAttachShader(program, current_shader);
  return current_shader;
}

bool create_shaders(GLuint &_shProg, const char *V, const char *F, bool isFile = true)
{
  _shProg = glCreateProgram();
  if (!_shProg)
  {
    std::cout << "Error creating _shProg program!\n";
    exit(1);
    return false;
  }

  GLuint vertexShader = 0;
  GLuint fragmentShader = 0;
  if (isFile)
  {
    vertexShader = add_shader(_shProg, std::string(V), GL_VERTEX_SHADER);     // don't work :(
    fragmentShader = add_shader(_shProg, std::string(F), GL_FRAGMENT_SHADER); // don't work :(
  }
  else
  {
    vertexShader = add_shader(_shProg, V, GL_VERTEX_SHADER);     // don't work :(
    fragmentShader = add_shader(_shProg, F, GL_FRAGMENT_SHADER); // don't work :(
  }

  GLint result = 0;
  GLchar log[1024] = {0};

  glLinkProgram(_shProg);
  glGetProgramiv(_shProg, GL_LINK_STATUS, &result);
  if (!result)
  {
    glGetProgramInfoLog(_shProg, sizeof(log), NULL, log);
    printProgramLog(_shProg);
    std::cout << "Error linking program:\n"
              << log << '\n';
    return false;
  }

  glValidateProgram(_shProg);
  glGetProgramiv(_shProg, GL_VALIDATE_STATUS, &result);
  if (!result)
  {
    glGetProgramInfoLog(_shProg, sizeof(log), NULL, log);
    printProgramLog(_shProg);
    std::cout << "Error validating program:\n"
              << log << '\n';
    return false;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return true;
}

bool resizeTexture(GLuint W, GLuint H)
{
  glBindTexture(GL_TEXTURE_2D, renderTex_id);
#ifdef __EMSCRIPTEN__
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
#else
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
#endif
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glBindTexture(GL_TEXTURE_2D, paintTex_id);
#ifdef __EMSCRIPTEN__
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
#else
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
#endif
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  bufWIDTH = W;
  bufHEIGHT = H;

  std::cout << "Resize texture: " << bufWIDTH << " - " << bufHEIGHT << std::endl;
  return true;
}

bool create_framebuffer(GLuint W, GLuint H)
{
  glGenFramebuffers(1, &FBO);

  glGenTextures(1, &paintTex_id);
  glGenTextures(1, &renderTex_id);

  return resizeTexture(W, H);
}

void cleanShader()
{
  // glDeleteTextures(1, &renderTex_id);
  // glDeleteTextures(1, &paintTex_id);
  // glDeleteFramebuffers(1, &FBO);
  // glDeleteFramebuffers(1, &FBO1);
  // glDeleteRenderbuffers(1, &RBO);
  glDeleteProgram(renderProg);
  glDeleteProgram(testProg);
}

void updateShader()
{
  cleanShader();
  // create_shaders();
  // create_framebuffer();
}

glm::vec3 cp;
void addImGuiFrame()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGuiWindowFlags window_flags = 0;
  window_flags |= ImGuiWindowFlags_NoBackground;
  window_flags |= ImGuiWindowFlags_NoDecoration;
  // window_flags |= ImGuiWindowFlags_NoResize;
  // window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
  window_flags |= ImGuiWindowFlags_NoTitleBar;
  bool *open_ptr;

  ImGui::Begin("Commander", open_ptr, window_flags);
  ImGui::SliderFloat3("Control Point thuy", (float*)&cp, -10.0f, 10.0f);
  // ImGui::Checkbox("bucket", &doFloodFill);
  // ImGui::Checkbox("useTex", &isUseTex);
  // ImGui::SliderFloat("Brush size", &f, 0.001f, .2f); // Edit 1 float using a slider from 0.0f to 1.0f
  // ImGui::ColorEdit4("BG color", (float *)&bg_color);
  // ImGui::ColorEdit4("FG color", (float *)&clear_color); // Edit 3 floats representing a color
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  ImGui::End();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

static void glfw_error_callback(int error, const char *description)
{
  fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

float aspect = 1.f;
void initCam()
{
  int vpSize[2]{0, 0};
  int _w, _h;
  glfwGetFramebufferSize(glfw_window, &_w, &_h);
  if (_w != vpSize[0] || _h != vpSize[1])
  {
    vpSize[0] = _w;
    vpSize[1] = _h;
    glViewport(0, 0, vpSize[0], vpSize[1]);
    aspect = (float)_w/(float)_h;
    u_prj2D = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -10.0f, 10.0f);
    // glUniform2f(loc_res, (float)_w, (float)_h);
    // std::cout << "size: " << _w << " - " << _h << std::endl;
  }
  // 2D
  u_viw2D = glm::mat4(1.f);
  u_viw2D = glm::translate(u_viw2D, glm::vec3(0.f, 0.f, 0.f));
  u_viw2D = glm::scale(u_viw2D, glm::vec3(1.f, 1.f, 1.f));
}

int init()
{
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
    return EXIT_FAILURE;

  // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
  // GL ES 2.0 + GLSL 100
  const char *glsl_version = "#version 100";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
  // GL 3.2 + GLSL 150
  const char *glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
  // GL 3.0 + GLSL 130
  const char *glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

  // Set all the required options for GLFW
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  // glfwWindowHint(GLFW_SCALE_TO_MONITOR, 1);

  // glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  // glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);

  // Create a GLFWwindow object that we can use for GLFW's functions
  glfw_window = glfwCreateWindow(WIDTH, HEIGHT, "ThuyTest", nullptr, nullptr);

  // glfwSetWindowUserPointer(glfw_window, (void *)this);

  int screenWidth, screenHeight;
  glfwGetFramebufferSize(glfw_window, &screenWidth, &screenHeight);

  if (nullptr == glfw_window)
  {
    std::cout << "Failed to create GLFW glfw_window" << std::endl;
    glfwTerminate();

    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(glfw_window);

  // Set mouse and scroll callbacks
  glfwSetCursorPosCallback(glfw_window, mouse_callback);
  glfwSetMouseButtonCallback(glfw_window, mouse_button_callback);
  glfwSetScrollCallback(glfw_window, scroll_callback);

  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigWindowsMoveFromTitleBarOnly = true;
  // io.DisplaySize = ImVec2((float)WIDTH / .1, (float)HEIGHT / .1);

  io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
  io.DisplaySize = ImVec2((float)WIDTH, (float)HEIGHT);

  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(glfw_window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
  bool show_demo_window = true;
  bool show_another_window = false;

  // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
  glewExperimental = GL_TRUE;
  // Initialize GLEW to setup the OpenGL Function pointers
  if (GLEW_OK != glewInit())
  {
    std::cout << "Failed to initialize GLEW" << std::endl;
    return EXIT_FAILURE;
  }

  // Define the viewport dimensions
  glViewport(0, 0, screenWidth, screenHeight);

  // shader
  const char *__v;
  const char *__f;
  __v = "resources/shaders/win32/v.glsl";
  __f = "resources/shaders/win32/f.glsl";

  const char *v2 = "resources/shaders/win32/v2.glsl";
  const char *f2 = "resources/shaders/win32/f2.glsl";

  __v = "#version 300 es\n"
        "uniform mat4 u_mvp;\n"
        "uniform mat4 u_prj; // uniforms[0]\n"
        "uniform mat4 u_viw; // uniforms[0]\n"

        "in vec3 P;\n"
        "in vec2 uv;\n"
        "in vec3 Cd;\n"

        "out vec2 frag_uv;\n"
        "out vec3 frag_cd;\n"

        "void main()\n"
        "{\n"
            "gl_Position = u_prj * u_viw * vec4(P, 1.); // 0\n"
            "frag_uv = uv; // 1\n"
            "frag_cd = Cd; // 2\n"
        "}\0";

  __f = "#version 300 es\n"
        "precision highp float;\n"
        "uniform sampler2D samplerA;\n"
        "uniform float u_time;\n"
        "in vec2 frag_uv;\n"
        "out vec4 frag_col;\n"
        "void main()\n"
        "{\n"
            "frag_col = texture(samplerA, frag_uv);\n"
        "}\0";

  v2 =  "#version 300 es\n"
        "uniform mat4 u_mvp;\n"
        "uniform mat4 u_prj; // uniforms[0]\n"
        "uniform mat4 u_viw; // uniforms[0]\n"
        "uniform mat4 u_mod; // uniforms[0]\n"

        "in vec3 P;\n"
        "in vec2 uv;\n"
        "in vec3 Cd;\n"
        "in vec3 instancePos;\n"

        "out vec2 frag_uv;\n"
        "out vec3 frag_cd;\n"

        "void main()\n"
        "{\n"
            "vec3 pos = P + instancePos;\n"
            "gl_Position = u_prj * u_viw * u_mod * vec4(pos, 1.); // 0\n"
            "frag_uv = uv; // 1\n"
            "frag_cd = Cd; // 2\n"
        "}\0";

  f2 =  "#version 300 es\n"
        "precision highp float;\n"
        "uniform sampler2D samplerA;\n"
        "uniform float u_time;\n"
        "in vec2 frag_uv;\n"
        "in vec3 frag_cd;\n"
        "out vec4 frag_col;\n"
        "void main()\n"
        "{\n"
            "frag_col = vec4(frag_cd,1.0);\n"
        "}\0";

  if (!create_shaders(renderProg, __v, __f, false))
    return false;
  if (!create_shaders(testProg, v2, f2, false))
    return false;
  if (!create_framebuffer(WIDTH, HEIGHT))
    return false;
  triangle = new TriangleMesh();
  gnomon = new thuyXfo();
  testSphere = new thuyPoint();

  initCam();

  return EXIT_SUCCESS;
}

void draw()
{
  glUseProgram(testProg);

  glBindFramebuffer(GL_FRAMEBUFFER, FBO);
  glClearColor(.2,.2,.2,0);
  glClear(GL_COLOR_BUFFER_BIT);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTex_id, 0);

  glUniformMatrix4fv(glGetUniformLocation(testProg, "u_prj"), 1, GL_FALSE, glm::value_ptr(u_prj));
  glUniformMatrix4fv(glGetUniformLocation(testProg, "u_viw"), 1, GL_FALSE, glm::value_ptr(u_viw));

  glm::mat4 u_mod = glm::mat4(1.0f);
  // u_mod = glm::translate(u_mod, {0,0,0});
  // u_mod = glm::rotate(u_mod, (float)glfwGetTime(), {0,1,0});
  glUniformMatrix4fv(glGetUniformLocation(testProg, "u_mod"), 1, GL_FALSE, glm::value_ptr(u_mod));

  // triangle->draw();
  gnomon->drawI();
  testSphere->draw();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // render to view
  double curTime = glfwGetTime();
  glUseProgram(renderProg);
  glUniform1f(glGetUniformLocation(renderProg, "uTime"), curTime);
  glUniformMatrix4fv(glGetUniformLocation(renderProg, "u_prj"), 1, GL_FALSE, glm::value_ptr(u_prj2D));
  glUniformMatrix4fv(glGetUniformLocation(renderProg, "u_viw"), 1, GL_FALSE, glm::value_ptr(u_viw2D));
  glActiveTexture(GL_TEXTURE0 + 0);
  glBindTexture(GL_TEXTURE_2D, renderTex_id);
  triangle->draw();
}

void updateCam()
{
  glm::vec3 camera_pos = {-.5,0,3};
  // camera_pos*=cameraDistance;
  glm::vec3 camera_target = {0,0,0};
  glm::vec3 up = {0,1,0};

  camera_pos = glm::vec3(cameraDistance * sin(glm::radians(cameraAngleY)) * cos(glm::radians(cameraAngleX)),
                         cameraDistance * sin(glm::radians(cameraAngleX)),
                         cameraDistance * cos(glm::radians(cameraAngleY)) * cos(glm::radians(cameraAngleX))),
  camera_target = glm::vec3(panX, panY, 0.0f);

  u_prj = glm::perspective(glm::radians(45.0f), (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);
  u_viw = glm::lookAt(camera_pos,camera_target,up);
}

void update()
{
  while (!glfwWindowShouldClose(glfw_window))
  {
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);

    RBF_lerp_quat(cp);

    updateCam();
    draw();
    addImGuiFrame();

    glfwPollEvents();
    glfwSwapBuffers(glfw_window);
  }
}

void close()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwTerminate();
}

int main()
{
    init();
    update();
    close();
    return EXIT_SUCCESS;
}

#pragma endregion