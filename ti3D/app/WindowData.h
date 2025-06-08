// In WindowData.h
#ifndef WINDOWDATA_H
#define WINDOWDATA_H

#include <GLFW/glfw3.h>

#include "Camera.h"
#include "StateManager.h"
namespace Ti3D
{
struct WindowData
{
    Camera* camera;
    StateManager* stateManager;
};
}  // namespace Ti3D

#endif  // WINDOWDATA_H