cmake_minimum_required(VERSION 3.10)

project(Ti3D VERSION 1.0 LANGUAGES CXX)
# Description: A simple 3D graphics engine
# Author: NHAT PHAN

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find GLFW
find_library(GLFW_LIBRARY NAMES glfw3 glfw PATHS ${CMAKE_SOURCE_DIR}/ThirdParty/glfw-3.4/lib-vc2022)
if(NOT GLFW_LIBRARY)
    message(FATAL_ERROR "GLFW library not found. Please ensure it is built and located in ThirdParty/glfw-3.4/lib-vc2022")
endif()

# Find OpenGL
find_package(OpenGL REQUIRED)

# Add GLAD source file
set(GLAD_SRC ${CMAKE_SOURCE_DIR}/ThirdParty/gladLib/src/glad.c)
if(NOT EXISTS ${GLAD_SRC})
    message(FATAL_ERROR "GLAD source file not found: ${GLAD_SRC}")
endif()

# Force glad.c to be compiled as C, not C++, to avoid symbol mismatch when linking with C++ code
set_source_files_properties(${GLAD_SRC} PROPERTIES LANGUAGE C)

# Source files
set(SRC_FILES
    camera/Camera.cpp
    ${GLAD_SRC}
)

# Define the executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Include directories (glad first, then glfw)
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/ThirdParty/gladLib/include
    ${CMAKE_SOURCE_DIR}/ThirdParty/glfw-3.4/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${GLFW_LIBRARY} OpenGL::GL)

# Set output name
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "Ti3D" SUFFIX ".exe")

# Enable verbose output for debugging
set(CMAKE_VERBOSE_MAKEFILE ON)