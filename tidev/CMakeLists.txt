# Minimum CMake version
cmake_minimum_required(VERSION 3.15)

# Project name and language
project(MyProject VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the vcpkg installation root
set(VCPKG_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg")
set(VCPKG_TARGET_TRIPLET "x64-windows-static")

# Include directories for GLAD
set(GLAD_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/glad/include")
set(GLAD_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/glad/src/glad.c")

# Include GLAD headers
include_directories(${GLAD_INCLUDE_DIR})

# Include directories for GLFW
set(GLFW_INCLUDE_DIR "${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/include")
include_directories(${GLFW_INCLUDE_DIR})

# Find OpenGL
find_package(OpenGL REQUIRED)

# Add the executable and include the GLAD source file
add_executable(${PROJECT_NAME}
    src/main.cpp    # Your main application source file
    ${GLAD_SOURCE}  # GLAD source file
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src     # Your source directory
    ${GLAD_INCLUDE_DIR}                 # GLAD include directory
    ${GLFW_INCLUDE_DIR}                 # GLFW include directory
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    "${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/lib/glfw3.lib"  # GLFW library
    OpenGL::GL                                                       # OpenGL library
)

# For Windows-specific libraries (optional)
if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        opengl32.lib                    # OpenGL library on Windows
        gdi32                           # Windows Graphics Device Interface
        user32                          # Windows User API
    )
endif()

# Set the output directory for the executable
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Configure VSCode IntelliSense with the CMake Tools extension
# No need for compile_commands.json when using the Visual Studio generator
