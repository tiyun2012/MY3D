cmake_minimum_required(VERSION 3.10)
project(Ti3D)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set paths for third-party libraries
set(GLFW3_DIR "C:/Users/pc/Desktop/Dev/ti3D/ThirdParty/glfw-3.4")
set(GLAD_DIR "C:/Users/pc/Desktop/Dev/ti3D/ThirdParty/gladLib")

# Add GLFW3 to CMAKE_PREFIX_PATH
list(APPEND CMAKE_PREFIX_PATH "${GLFW3_DIR}")

# Include directories
include_directories(
    ${GLFW3_DIR}/include
    ${GLAD_DIR}/include
    ${CMAKE_SOURCE_DIR}/TiMath
    ${CMAKE_SOURCE_DIR}/camera
    ${CMAKE_SOURCE_DIR}/renderer
    ${CMAKE_SOURCE_DIR}/app
)

# Source files
set(SOURCES
    app/main.cpp
    camera/Camera.cpp
    renderer/Renderer.cpp
    TiMath/Vector3.cpp
    TiMath/Quaternion.cpp
    TiMath/Matrix4.cpp
    ${GLAD_DIR}/src/glad.c
)

# Create executable
add_executable(Ti3D ${SOURCES})

# Link libraries
if (MSVC)
    target_link_libraries(Ti3D PRIVATE ${GLFW3_DIR}/lib-vc2022/glfw3_mt.lib)
else()
    target_link_libraries(Ti3D PRIVATE glfw)
endif()

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)