#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>
#include <stdexcept>
#include <ostream>
#include "TiMathConfig.h"

namespace TiMath {

class Vector2 {
public:
    float x, y;

    constexpr Vector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

    [[nodiscard]] inline Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }

    [[nodiscard]] inline Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }

    [[nodiscard]] inline Vector2 operator*(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    [[nodiscard]] inline Vector2 operator/(float scalar) const {
        if (std::fabs(scalar) < TiMath::EPSILON) {
            throw std::runtime_error("Division by zero in Vector2");
        }
        return Vector2(x / scalar, y / scalar);
    }

    inline Vector2& operator+=(const Vector2& other) {
        x += other.x; y += other.y;
        return *this;
    }

    inline Vector2& operator-=(const Vector2& other) {
        x -= other.x; y -= other.y;
        return *this;
    }

    inline Vector2& operator*=(float scalar) {
        x *= scalar; y *= scalar;
        return *this;
    }

    inline Vector2& operator/=(float scalar) {
        if (std::fabs(scalar) < TiMath::EPSILON) {
            throw std::runtime_error("Division by zero in Vector2");
        }
        x /= scalar; y /= scalar;
        return *this;
    }

    [[nodiscard]] inline float dot(const Vector2& other) const {
        return x * other.x + y * other.y;
    }

    [[nodiscard]] float length() const;

    [[nodiscard]] inline float lengthSquared() const {
        return x * x + y * y;
    }

    [[nodiscard]] Vector2 normalized() const;

    [[nodiscard]] Vector2 reflectOver(const Vector2& normal) const;

    [[nodiscard]] Vector2 projectOnto(const Vector2& target) const;

    [[nodiscard]] inline Vector2 perpendicular() const {
        return Vector2(-y, x);
    }

    [[nodiscard]] inline bool isZero(float epsilon = TiMath::EPSILON) const {
        return std::fabs(x) < epsilon && std::fabs(y) < epsilon;
    }

    [[nodiscard]] inline bool isFinite() const {
        return std::isfinite(x) && std::isfinite(y);
    }

    [[nodiscard]] static inline Vector2 lerp(const Vector2& v1, const Vector2& v2, float t) {
        return Vector2(v1.x + t * (v2.x - v1.x), v1.y + t * (v2.y - v1.y));
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector2& v);
};

[[nodiscard]] inline Vector2 operator*(float scalar, const Vector2& v) {
    return v * scalar;
}

std::ostream& operator<<(std::ostream& os, const Vector2& v);

} // namespace TiMath

#endif // VECTOR2_H