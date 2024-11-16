# Set the minimum required version of CMake to build this project
cmake_minimum_required(VERSION 3.12)

# Store the project name in a variable
set(PROJECT_NAME "MY3D")

# Define the project using the stored project name
project(${PROJECT_NAME})

# Set the C++ standard to C++20 to use modern C++ features
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Collect all source files from specific directories
file(GLOB_RECURSE SRC_FILES
    src/*.cpp
    myMath/*.cpp
    utils/*.cpp
)

# List of files to exclude
set(EXCLUDED_FILES
    # math/math1.cpp
    # math/mathB.cpp
    utils/utilstest.cpp
)

# Remove the excluded files from the list of source files
foreach(file ${EXCLUDED_FILES})
    list(REMOVE_ITEM SRC_FILES ${file})
endforeach()



# Create the executable using the stored project name and collected source files
add_executable(${PROJECT_NAME} main.cpp ${SRC_FILES})
# Add include directories ,this should be config after add_executable
target_include_directories(${PROJECT_NAME} PRIVATE
    include
    math
    utils
)
# Find and link the necessary libraries (OpenGL, GLFW, Glad)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glad REQUIRED)

# Link the project with the necessary libraries
target_link_libraries(${PROJECT_NAME} glfw glad OpenGL::GL)

# Enable warnings during compilation to help catch potential issues
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)

# Optional: Enable optimization and debug info based on build type
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(${PROJECT_NAME} PRIVATE -O3)
elseif (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${PROJECT_NAME} PRIVATE -g)
endif()
