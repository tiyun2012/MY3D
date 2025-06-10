cmake_minimum_required(VERSION 3.10)
project(Ti3D LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define paths
set(GLFW3_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/glfw-3.4")
set(GLAD_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/gladLib")
set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/imgui")

# Verify ImGui source files
foreach(file imgui.cpp imgui_draw.cpp imgui_widgets.cpp imgui_tables.cpp backends/imgui_impl_glfw.cpp backends/imgui_impl_opengl3.cpp)
    if (NOT EXISTS "${IMGUI_DIR}/${file}")
        message(FATAL_ERROR "Missing ImGui file: ${IMGUI_DIR}/${file}")
    endif()
endforeach()

# Verify GLFW library
if (NOT EXISTS "${GLFW3_DIR}/lib-vc2022/glfw3.lib")
    message(FATAL_ERROR "GLFW library not found: ${GLFW3_DIR}/lib-vc2022/glfw3.lib")
endif()

# Define GLFW as an interface library
add_library(glfw INTERFACE)
if (MSVC)
    target_link_libraries(glfw INTERFACE "${GLFW3_DIR}/lib-vc2022/glfw3.lib")
else()
    target_link_libraries(glfw INTERFACE "${GLFW3_DIR}/lib/libglfw3.a")
endif()
target_include_directories(glfw INTERFACE "${GLFW3_DIR}/include")

# Define GLAD as a static library
add_library(glad STATIC
    "${GLAD_DIR}/src/glad.c"
)
set_target_properties(glad PROPERTIES
    LINKER_LANGUAGE C
)
target_include_directories(glad PUBLIC
    "${GLAD_DIR}/include"
)
if (MSVC)
    target_compile_options(glad PRIVATE /MD$<$<CONFIG:Debug>:d>)
endif()

# Define Dear ImGui as a static library
add_library(imgui STATIC
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
)
target_include_directories(imgui PUBLIC
    "${IMGUI_DIR}"
    "${IMGUI_DIR}/backends"
)
target_link_libraries(imgui PUBLIC glfw glad)
if (MSVC)
    target_compile_options(imgui PRIVATE /MD$<$<CONFIG:Debug>:d>)
endif()

# Include directories
include_directories(
    "${GLFW3_DIR}/include"
    "${GLAD_DIR}/include"
    "${IMGUI_DIR}"
    "${IMGUI_DIR}/backends"
    "${CMAKE_SOURCE_DIR}/TiMath"
    "${CMAKE_SOURCE_DIR}/camera"
    "${CMAKE_SOURCE_DIR}/renderer"
    "${CMAKE_SOURCE_DIR}/app"
)

# Source files
set(SOURCES
    # app/main.cpp
    UI/main.cpp
    
    camera/Camera.cpp
    renderer/Renderer.cpp
    renderer/DebugPoint.cpp
    TiMath/Vector2.cpp
    TiMath/Vector3.cpp
    TiMath/Vector4.cpp
    TiMath/Quaternion.cpp
    TiMath/Matrix4.cpp
    app/StateManager.cpp
)

# Create executable
add_executable(Ti3D ${SOURCES})
if (MSVC)
    target_compile_options(Ti3D PRIVATE /MD$<$<CONFIG:Debug>:d>)
    target_link_libraries(Ti3D PRIVATE glfw glad imgui opengl32)
else()
    target_link_libraries(Ti3D PRIVATE glfw glad imgui)
    if (UNIX AND NOT APPLE)
        target_link_libraries(Ti3D PRIVATE GL dl)
    elseif(APPLE)
        target_link_libraries(Ti3D PRIVATE "-framework OpenGL")
    endif()
endif()

# Force MSVC to use dynamic runtime (/MD, /MDd)
if(MSVC)
    foreach(flag_var
        CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_CXX_FLAGS_RELWITHDEBINFO)
        if(${flag_var} MATCHES "/MT")
            string(REGEX REPLACE "/MT" "/MD" ${flag_var} "${${flag_var}}")
        endif()
    endforeach()
endif()

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
foreach(CONFIG Debug Release)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG} "${CMAKE_BINARY_DIR}/bin/${CONFIG}")
endforeach()

# Diagnostics
message(STATUS "Ti3D Configuration:")
message(STATUS "  GLFW3_DIR: ${GLFW3_DIR}")
message(STATUS "  GLAD_DIR: ${GLAD_DIR}")
message(STATUS "  IMGUI_DIR: ${IMGUI_DIR}")
message(STATUS "  Output Directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")