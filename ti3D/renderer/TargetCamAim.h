#pragma once

#include "DebugDrawable.h"
#include "../TiMath/Vector3.h"
#include "../TiMath/Matrix4.h"

namespace Ti3D {

class TargetCamAim : public DebugDrawable {
public:
    TargetCamAim(float radius = 0.5f, const TiMath::Vector3& pos = TiMath::Vector3(0.0f, 0.0f, 0.0f));
    void setPosition(const TiMath::Vector3& pos);
    const TiMath::Vector3& getPosition() const;
    void setRadius(float radius);
    float getRadius() const;
    void draw(const Camera& camera) override;

private:
    TiMath::Vector3 position;
    float radius;
    unsigned int vao, vbo;
    unsigned int shaderProgram;
    void initCircleMesh();
    void initShader();
};

} // namespace Ti3D