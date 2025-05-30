#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>
#include <iostream>
#include <stdexcept>

namespace TiMath {

/**
 * @class Vector2
 * @brief A 2D vector class for graphics and physics calculations.
 */
class Vector2 {
public:
    float x, y;

    // Static constant vectors
    static const Vector2 zero;
    static const Vector2 unitX;
    static const Vector2 unitY;
    static const Vector2 one;

    // Constructors
    constexpr Vector2() : x(0.0f), y(0.0f) {}
    constexpr Vector2(float x, float y) : x(x), y(y) {}

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
        if (std::fabs(scalar) < 1e-6f) {
            throw std::runtime_error("Division by zero in Vector2::operator/");
        }
        return Vector2(x / scalar, y / scalar);
    }

    [[nodiscard]] inline Vector2 operator-() const {
        return Vector2(-x, -y);
    }

    // Compound assignment operators
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
        if (std::fabs(scalar) < 1e-6f) {
            throw std::runtime_error("Division by zero in Vector2::operator/=");
        }
        x /= scalar; y /= scalar;
        return *this;
    }

    // Vector operations
    [[nodiscard]] inline float length() const {
        return std::sqrt(x * x + y * y);
    }

    [[nodiscard]] inline float lengthSquared() const {
        return x * x + y * y;
    }

    [[nodiscard]] inline Vector2 normalized() const {
        float lenSq = lengthSquared();
        if (lenSq < 1e-6f) {
            throw std::runtime_error("Cannot normalize zero-length vector");
        }
        float len = std::sqrt(lenSq);
        return *this / len;
    }

    [[nodiscard]] inline Vector2 safeNormalized(const Vector2& defaultValue = Vector2::unitX) const {
        float lenSq = lengthSquared();
        if (lenSq < 1e-6f) {
            return defaultValue;
        }
        float len = std::sqrt(lenSq);
        return *this / len;
    }

    [[nodiscard]] inline float dot(const Vector2& other) const {
        return x * other.x + y * other.y;
    }

    [[nodiscard]] inline float cross(const Vector2& other) const {
        return x * other.y - y * other.x;
    }

    // Geometric operations
    [[nodiscard]] float angleBetween(const Vector2& other) const;
    [[nodiscard]] Vector2 perpendicular() const { return Vector2(-y, x); }
    [[nodiscard]] Vector2 reflect(const Vector2& normal) const;

    // Component-wise operations
    [[nodiscard]] inline Vector2 componentMultiply(const Vector2& other) const {
        return Vector2(x * other.x, y * other.y);
    }

    [[nodiscard]] inline Vector2 componentDivide(const Vector2& other) const {
        if (std::fabs(other.x) < 1e-6f || std::fabs(other.y) < 1e-6f) {
            throw std::runtime_error("Division by zero in Vector2::componentDivide");
        }
        return Vector2(x / other.x, y / other.y);
    }

    // Utility functions
    [[nodiscard]] inline float distance(const Vector2& other) const {
        return (*this - other).length();
    }

    [[nodiscard]] inline float distanceSquared(const Vector2& other) const {
        return (*this - other).lengthSquared();
    }

    [[nodiscard]] inline bool isFinite() const {
        return std::isfinite(x) && std::isfinite(y);
    }

    // Static methods
    [[nodiscard]] static inline Vector2 min(const Vector2& a, const Vector2& b) {
        return Vector2(std::min(a.x, b.x), std::min(a.y, b.y));
    }

    [[nodiscard]] static inline Vector2 max(const Vector2& a, const Vector2& b) {
        return Vector2(std::max(a.x, b.x), std::max(a.y, b.y));
    }

    [[nodiscard]] static inline Vector2 lerp(const Vector2& v1, const Vector2& v2, float t) {
        return Vector2(
            v1.x + t * (v2.x - v1.x),
            v1.y + t * (v2.y - v1.y)
        );
    }

    // Comparison operators
    [[nodiscard]] inline bool isZero() const {
        const float epsilon = 1e-6f;
        return std::fabs(x) < epsilon && std::fabs(y) < epsilon;
    }

    [[nodiscard]] inline bool operator==(const Vector2& other) const {
        const float epsilon = 1e-6f;
        return std::fabs(x - other.x) < epsilon &&
               std::fabs(y - other.y) < epsilon;
    }

    [[nodiscard]] inline bool operator!=(const Vector2& other) const {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector2& v);
};

// Non-member operator for scalar multiplication
[[nodiscard]] inline Vector2 operator*(float scalar, const Vector2& v) {
    return v * scalar;
}

} // namespace TiMath

#endif // VECTOR2_H