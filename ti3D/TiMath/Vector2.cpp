#include "Vector2.h"

namespace TiMath {

float Vector2::length() const {
    return std::sqrt(x * x + y * y);
}

Vector2 Vector2::normalized() const {
    float len = length();
    if (len < 1e-6f) {
        throw std::runtime_error("Cannot normalize zero vector");
    }
    return Vector2(x / len, y / len);
}

Vector2 Vector2::reflectOver(const Vector2& normal) const {
    Vector2 unitNormal = normal.normalized();
    if (unitNormal.isZero()) {
        throw std::runtime_error("Zero-length normal in Vector2::reflectOver");
    }
    float dotProd = dot(unitNormal);
    return *this - unitNormal * (2.0f * dotProd);
}

Vector2 Vector2::projectOnto(const Vector2& target) const {
    if (target.isZero()) {
        throw std::runtime_error("Cannot project onto zero vector");
    }
    float dotProd = dot(target);
    float targetLenSq = target.dot(target);
    return target * (dotProd / targetLenSq);
}

} // namespace TiMath