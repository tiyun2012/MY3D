#pragma once

#include "../camera/Camera.h"

namespace Ti3D {

class DebugDrawable {
public:
    virtual ~DebugDrawable() = default;
    virtual void draw(const Camera& camera) = 0;
};

} // namespace Ti3D