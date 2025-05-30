#include "Vector2.h"

namespace TiMath {

// Static constant definitions
const Vector2 Vector2::zero(0.0f, 0.0f);
const Vector2 Vector2::unitX(1.0f, 0.0f);
const Vector2 Vector2::unitY(0.0f, 1.0f);
const Vector2 Vector2::one(1.0f, 1.0f);

float Vector2::angleBetween(const Vector2& other) const {
    float lenSq1 = lengthSquared();
    float lenSq2 = other.lengthSquared();
    if (lenSq1 < 1e-6f || lenSq2 < 1e-6f) {
        return 0.0f;
    }
    
    float cosTheta = dot(other) / std::sqrt(lenSq1 * lenSq2);
    cosTheta = std::clamp(cosTheta, -1.0f, 1.0f);
    return std::acos(cosTheta);
}

Vector2 Vector2::reflect(const Vector2& normal) const {
    Vector2 n = normal.normalized();
    if (n.isZero()) {
        return Vector2(0.0f, 0.0f);
    }
    float dotProd = dot(n);
    return *this - n * (2.0f * dotProd);
}

std::ostream& operator<<(std::ostream& os, const Vector2& v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

} // namespace TiMath