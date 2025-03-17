cmake_minimum_required(VERSION 3.15)
set(PROJECT_NAME GLRBF)
# Project name and settings
project(${PROJECT_NAME} LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configurations
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Build configurations" FORCE)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/ThirdParty/glfw-3.4/include
    ${CMAKE_SOURCE_DIR}/ThirdParty/imgui
    ${CMAKE_SOURCE_DIR}/ThirdParty/imgui/backends
    ${CMAKE_SOURCE_DIR}/ThirdParty/gladLib/include
    ${CMAKE_SOURCE_DIR}/glm-master
    ${CMAKE_SOURCE_DIR}/eigen-master
)

# Source files
set(SOURCES
    ${CMAKE_SOURCE_DIR}/ThirdParty/gladLib/src/glad.c
    ${CMAKE_SOURCE_DIR}/ThirdParty/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_SOURCE_DIR}/ThirdParty/imgui/backends/imgui_impl_opengl3.cpp
    ${CMAKE_SOURCE_DIR}/ThirdParty/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/ThirdParty/imgui/imgui_demo.cpp
    ${CMAKE_SOURCE_DIR}/ThirdParty/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/ThirdParty/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/ThirdParty/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/src/main.cpp
)

# Header files
set(HEADERS
    ${CMAKE_SOURCE_DIR}/ThirdParty/imgui/backends/imgui_impl_glfw.h
    ${CMAKE_SOURCE_DIR}/ThirdParty/imgui/backends/imgui_impl_opengl3.h
    ${CMAKE_SOURCE_DIR}/ThirdParty/imgui/backends/imgui_impl_opengl3_loader.h
    ${CMAKE_SOURCE_DIR}/ThirdParty/imgui/imconfig.h
    ${CMAKE_SOURCE_DIR}/ThirdParty/imgui/imgui.h
    ${CMAKE_SOURCE_DIR}/ThirdParty/imgui/imgui_internal.h
    ${CMAKE_SOURCE_DIR}/ThirdParty/imgui/imstb_rectpack.h
    ${CMAKE_SOURCE_DIR}/ThirdParty/imgui/imstb_textedit.h
    ${CMAKE_SOURCE_DIR}/ThirdParty/imgui/imstb_truetype.h
)

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Platform-specific settings
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CONSOLE)
endif()

# Linker settings for Debug
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug
)
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:_DEBUG>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:/W3 /SDL>)
target_link_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:/DEBUG>)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw3 opengl32)

# Linker settings for Release
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release
)
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:NDEBUG>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:/W3 /SDL /O2>)
target_link_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:/OPT:REF /OPT:ICF>)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw3 opengl32)

# Additional library paths
target_link_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/ThirdParty/gladLib/src
    ${CMAKE_SOURCE_DIR}/ThirdParty/glfw-3.4/lib-vc2022
)
