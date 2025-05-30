#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>
#include <stdexcept>
#include <ostream>

namespace TiMath {

class Vector2 {
public:
    float x, y;

    // Constructor
    constexpr Vector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

    // Arithmetic operators
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
        if (std::abs(scalar) < 1e-6f) {
            throw std::runtime_error("Division by zero in Vector2");
        }
        return Vector2(x / scalar, y / scalar);
    }

    // Compound assignment operators
    inline Vector2& operator+=(const Vector2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    inline Vector2& operator-=(const Vector2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    inline Vector2& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    inline Vector2& operator/=(float scalar) {
        if (std::abs(scalar) < 1e-6f) {
            throw std::runtime_error("Division by zero in Vector2");
        }
        x /= scalar;
        y /= scalar;
        return *this;
    }

    // Dot product
    [[nodiscard]] inline float dot(const Vector2& other) const {
        return x * other.x + y * other.y;
    }

    // Length (magnitude)
    [[nodiscard]] float length() const;

    // Normalized vector
    [[nodiscard]] Vector2 normalized() const;

    // Reflect over a normal
    [[nodiscard]] Vector2 reflectOver(const Vector2& normal) const;

    // Project onto another vector
    [[nodiscard]] Vector2 projectOnto(const Vector2& target) const;

    // Perpendicular vector
    [[nodiscard]] inline Vector2 perpendicular() const {
        return Vector2(-y, x);
    }

    // Check if zero
    [[nodiscard]] inline bool isZero(float epsilon = 1e-6f) const {
        return std::abs(x) < epsilon && std::abs(y) < epsilon;
    }

    // Check if finite
    [[nodiscard]] inline bool isFinite() const {
        return std::isfinite(x) && std::isfinite(y);
    }

    // Linear interpolation (static)
    [[nodiscard]] static inline Vector2 lerp(const Vector2& v1, const Vector2& v2, float t) {
        return Vector2(
            v1.x + t * (v2.x - v1.x),
            v1.y + t * (v2.y - v1.y)
        );
    }
};

// Non-member scalar multiplication
[[nodiscard]] inline Vector2 operator*(float scalar, const Vector2& v) {
    return v * scalar;
}

// Non-member output stream operator
[[nodiscard]] inline std::ostream& operator<<(std::ostream& os, const Vector2& v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

} // namespace TiMath

#endif // VECTOR2_H