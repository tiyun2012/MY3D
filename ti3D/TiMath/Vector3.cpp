#include "Vector3.h"

namespace TiMath {

// Static constant definitions
const Vector3 Vector3::zero(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::unitX(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::unitY(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::unitZ(0.0f, 0.0f, 1.0f);

float Vector3::angleBetween(const Vector3& other) const {
    float dotProd = dot(other);
    float lenSq1 = lengthSquared();
    float lenSq2 = other.lengthSquared();
    if (lenSq1 < 1e-6f || lenSq2 < 1e-6f) {
        return 0.0f;
    }
    float cosTheta = dotProd / std::sqrt(lenSq1 * lenSq2);
    cosTheta = std::max(-1.0f, std::min(1.0f, cosTheta));
    return std::acos(cosTheta);
}

Vector3 Vector3::projectOnto(const Vector3& other) const {
    float lenSq = other.lengthSquared();
    if (lenSq < 1e-6f) {
        return Vector3(0.0f, 0.0f, 0.0f);
    }
    float scalar = dot(other) / lenSq;
    return other * scalar;
}

Vector3 Vector3::reflectOver(const Vector3& normal) const {
    Vector3 unitNormal = normal.normalized();
    if (unitNormal.isZero()) {
        return Vector3(0.0f, 0.0f, 0.0f);
    }
    float dotProd = dot(unitNormal);
    return *this - unitNormal * (2.0f * dotProd);
}

std::ostream& operator<<(std::ostream& os, const Vector3& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

} // namespace TiMath