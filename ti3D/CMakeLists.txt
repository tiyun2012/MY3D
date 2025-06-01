cmake_minimum_required(VERSION 3.10)
project(Ti3D LANGUAGES CXX C)  # Added C for glad.c

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set paths for third-party libraries
set(GLFW3_DIR "C:/Users/pc/Desktop/Dev/ti3D/ThirdParty/glfw-3.4")
set(GLAD_DIR "C:/Users/pc/Desktop/Dev/ti3D/ThirdParty/gladLib")
set(IMGUI_DIR "C:/Users/pc/Desktop/Dev/ti3D/ThirdParty/imgui")

# Define GLAD as a static library
add_library(glad STATIC
    ${GLAD_DIR}/src/glad.c
)
set_target_properties(glad PROPERTIES
    LINKER_LANGUAGE C  # Explicitly set language to C
)
target_include_directories(glad PUBLIC
    ${GLAD_DIR}/include
)

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
    camera/CameraManager.cpp
    renderer/Renderer.cpp
    TiMath/Vector2.cpp
    TiMath/Vector3.cpp
    TiMath/Vector4.cpp  # Added Vector4.cpp
    TiMath/Quaternion.cpp
    TiMath/Matrix4.cpp
    app/StateManager.cpp
)

# Create executable
add_executable(Ti3D ${SOURCES})

# Link libraries
if (MSVC)
    target_link_libraries(Ti3D PRIVATE ${GLFW3_DIR}/lib-vc2022/glfw3_mt.lib glad opengl32)
else()
    target_link_libraries(Ti3D PRIVATE ${GLFW3_DIR}/lib/libglfw3.a glad)
    if (UNIX AND NOT APPLE)
        target_link_libraries(Ti3D PRIVATE GL dl)
    elseif(APPLE)
        target_link_libraries(Ti3D PRIVATE "-framework OpenGL")
    endif()
endif()

# Force MSVC to use the dynamic runtime (/MD, /MDd) to avoid CRT conflicts
if(MSVC)
    foreach(flag_var
        CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        if(${flag_var} MATCHES "/MT")
            string(REGEX REPLACE "/MT" "/MD" ${flag_var} "${${flag_var}}")
        endif()
    endforeach()
endif()

# Set output directories for multi-config and single-config generators
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
foreach(CONFIG Debug Release)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG} ${CMAKE_BINARY_DIR}/bin/${CONFIG})
endforeach()

# Print configuration summary
message(STATUS "Ti3D Configuration:")
message(STATUS "  GLFW3_DIR: ${GLFW3_DIR}")
message(STATUS "  GLAD_DIR: ${GLAD_DIR}")
message(STATUS "  Output Directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")