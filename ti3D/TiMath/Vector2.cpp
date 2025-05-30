#include "Vector2.h"

namespace TiMath {

float Vector2::length() const {
    return std::sqrt(x * x + y * y);
}

Vector2 Vector2::normalized() const {
    float len = length();
    if (len < TiMath::EPSILON) {
        throw std::runtime_error("Cannot normalize zero vector");
    }
    return Vector2(x / len, y / len);
}

Vector2 Vector2::reflectOver(const Vector2& normal) const {
    Vector2 unitNormal = normal.normalized();
    float dotProd = dot(unitNormal);
    return *this - unitNormal * (2.0f * dotProd);
}

Vector2 Vector2::projectOnto(const Vector2& target) const {
    float lenSq = target.lengthSquared();
    if (lenSq < TiMath::EPSILON) {
        throw std::runtime_error("Cannot project onto zero vector");
    }
    float scalar = dot(target) / lenSq;
    return target * scalar;
}

std::ostream& operator<<(std::ostream& os, const Vector2& v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

} // namespace TiMath