#ifndef VECTOR2_H
#define VECTOR2_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm> // For std::clamp
#include <stdexcept>
#include <ostream>
#include "TiMathConfig.h"



namespace TiMath {

class Vector2 {
public:
    float x, y;

    // Static constants (declarations only)
    static const Vector2 zero;
    static const Vector2 unitX;
    static const Vector2 unitY;

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
        if (std::fabs(scalar) < TiMath::EPSILON) {
            return Vector2{0.0f, 0.0f}; // Safe default
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
            x = y = 0.0f; // Safe default
            return *this;
        }
        x /= scalar; y /= scalar;
        return *this;
    }

    // Vector operations
    [[nodiscard]] constexpr float dot(const Vector2& other) const {
        return x * other.x + y * other.y;
    }

    [[nodiscard]] inline float length() const {
        return std::sqrt(lengthSquared());
    }

    [[nodiscard]] constexpr float lengthSquared() const {
        return x * x + y * y;
    }

    [[nodiscard]] inline Vector2 normalized() const {
        float len = length();
        return len < TiMath::EPSILON ? Vector2{0.0f, 0.0f} : Vector2{x / len, y / len};
    }

    [[nodiscard]] inline Vector2 normalizedUnsafe() const {
        float len = length();
        return Vector2{x / len, y / len}; // Assumes len != 0
    }

    [[nodiscard]] inline Vector2 reflectOver(const Vector2& normal) const {
        Vector2 unitNormal = normal.normalized();
        if (unitNormal.isZero()) {
            return Vector2{0.0f, 0.0f};
        }
        float dotProd = dot(unitNormal);
        return *this - unitNormal * (2.0f * dotProd);
    }

    [[nodiscard]] inline Vector2 projectOnto(const Vector2& target) const {
        float lenSq = target.lengthSquared();
        if (lenSq < TiMath::EPSILON) {
            return Vector2{0.0f, 0.0f};
        }
        float scalar = dot(target) / lenSq;
        return target * scalar;
    }

    [[nodiscard]] inline float cross(const Vector2& other) const {
        return x * other.y - y * other.x;
    }

    [[nodiscard]] inline Vector2 perpendicular() const {
        return Vector2(-y, x); // Counterclockwise 90-degree rotation
    }

    [[nodiscard]] inline Vector2 rotate(float angleDegrees) const {
        float angleRad = angleDegrees * static_cast<float>(TiMath::PI) / 180.0f;
        float cosA = std::cos(angleRad);
        float sinA = std::sin(angleRad);
        return Vector2(x * cosA - y * sinA, x * sinA + y * cosA);
    }

    [[nodiscard]] inline float angleBetween(const Vector2& other) const {
        float dotProd = dot(other);
        float lenSq1 = lengthSquared();
        float lenSq2 = other.lengthSquared();
        if (lenSq1 < TiMath::EPSILON || lenSq2 < TiMath::EPSILON) {
            return 0.0f;
        }
        float cosTheta = dotProd / std::sqrt(lenSq1 * lenSq2);
        cosTheta = std::max(-1.0f, std::min(1.0f, cosTheta));
        return std::acos(cosTheta);
    }

    [[nodiscard]] inline bool isZero(float epsilon = TiMath::EPSILON) const {
        return std::fabs(x) < epsilon && std::fabs(y) < epsilon;
    }

    [[nodiscard]] inline bool isFinite() const {
        return std::isfinite(x) && std::isfinite(y);
    }

    [[nodiscard]] inline bool operator==(const Vector2& other) const {
        return std::fabs(x - other.x) < TiMath::EPSILON &&
               std::fabs(y - other.y) < TiMath::EPSILON;
    }

    [[nodiscard]] inline bool operator!=(const Vector2& other) const {
        return !(*this == other);
    }

    [[nodiscard]] static inline Vector2 lerp(const Vector2& v1, const Vector2& v2, float t) {
        return Vector2(v1.x + t * (v2.x - v1.x), v1.y + t * (v2.y - v1.y));
    }

    [[nodiscard]] static inline Vector2 min(const Vector2& a, const Vector2& b) {
        return Vector2(std::min(a.x, b.x), std::min(a.y, b.y));
    }

    [[nodiscard]] static inline Vector2 max(const Vector2& a, const Vector2& b) {
        return Vector2(std::max(a.x, b.x), std::max(a.y, b.y));
    }

    [[nodiscard]] inline Vector2 clamp(float minVal, float maxVal) const {
        return Vector2(std::clamp(x, minVal, maxVal), std::clamp(y, minVal, maxVal));
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector2& v);
};

// Static constant definitions
constexpr Vector2 Vector2::zero{0.0f, 0.0f};
constexpr Vector2 Vector2::unitX{1.0f, 0.0f};
constexpr Vector2 Vector2::unitY{0.0f, 1.0f};

[[nodiscard]] inline Vector2 operator*(float scalar, const Vector2& v) {
    return v * scalar;
}

} // namespace TiMath

#endif // VECTOR2_H