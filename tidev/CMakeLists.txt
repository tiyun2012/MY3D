cmake_minimum_required(VERSION 3.12)
project(MY3D)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ensure that vcpkg toolchain is being used
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

# Set the target triplet for static linkage
set(VCPKG_TARGET_TRIPLET "x64-windows-static")

# Add source files
file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Add GLAD as a static library
add_library(glad STATIC thirdParty/glad/src/glad.c)

# Include directories for GLAD
target_include_directories(glad PRIVATE thirdParty/glad/include)
target_include_directories(${PROJECT_NAME} PRIVATE thirdParty/glad/include)

# Find and link GLFW and OpenGL
find_package(glfw3 REQUIRED CONFIG)
find_package(OpenGL REQUIRED)

# Link GLAD, GLFW, and OpenGL to the main executable
target_link_libraries(${PROJECT_NAME} PRIVATE glad glfw3 OpenGL::GL)

# Explicitly add the directory containing glfw3.lib
target_link_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/vcpkg/installed/x64-windows-static/lib")

# Set output directories
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)
