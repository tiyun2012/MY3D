#ifndef VECTOR3_H
#define VECTOR3_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm> // For std::clamp
#include <stdexcept>
#include <ostream>
#include "TiMathConfig.h"


namespace TiMath {

class Vector3 {
public:
    float x, y, z;

    // Static constants (declarations only)
    static const Vector3 zero;
    static const Vector3 unitX;
    static const Vector3 unitY;
    static const Vector3 unitZ;

    constexpr Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}

    // Arithmetic operators
    /** @brief Adds two vectors component-wise. */
    [[nodiscard]] inline Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    /** @brief Subtracts two vectors component-wise. */
    [[nodiscard]] inline Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    /** @brief Negates all components of the vector. */
    [[nodiscard]] inline Vector3 operator-() const {
        return Vector3(-x, -y, -z);
    }

    /** @brief Scales the vector by a scalar. */
    [[nodiscard]] inline Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    /** @brief Divides the vector by a scalar, returns zero vector if scalar is near zero. */
    [[nodiscard]] inline Vector3 operator/(float scalar) const {
        if (std::fabs(scalar) < TiMath::EPSILON) {
            return Vector3{0.0f, 0.0f, 0.0f}; // Safe default
        }
        return Vector3(x / scalar, y / scalar, z / scalar);
    }

    /** @brief Adds another vector to this one. */
    inline Vector3& operator+=(const Vector3& other) {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }

    /** @brief Subtracts another vector from this one. */
    inline Vector3& operator-=(const Vector3& other) {
        x -= other.x; y -= other.y; z -= other.z;
        return *this;
    }

    /** @brief Scales this vector by a scalar. */
    inline Vector3& operator*=(float scalar) {
        x *= scalar; y *= scalar; z *= scalar;
        return *this;
    }

    /** @brief Divides this vector by a scalar, sets to zero if scalar is near zero. */
    inline Vector3& operator/=(float scalar) {
        if (std::fabs(scalar) < TiMath::EPSILON) {
            x = y = z = 0.0f; // Safe default
            return *this;
        }
        x /= scalar; y /= scalar; z /= scalar;
        return *this;
    }

    // Vector operations
    /** @brief Computes the dot product with another vector. */
    [[nodiscard]] constexpr float dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    /** @brief Computes the cross product with another vector. */
    [[nodiscard]] inline Vector3 cross(const Vector3& other) const {
        return Vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    /** @brief Returns the length (magnitude) of the vector. */
    [[nodiscard]] inline float length() const {
        return std::sqrt(lengthSquared());
    }

    /** @brief Returns the squared length of the vector. */
    [[nodiscard]] constexpr float lengthSquared() const {
        return x * x + y * y + z * z;
    }

    /** @brief Returns a normalized copy of the vector, or zero if length is near zero. */
    [[nodiscard]] inline Vector3 normalized() const {
        float len = length();
        return len < TiMath::EPSILON ? Vector3{0.0f, 0.0f, 0.0f} : Vector3{x / len, y / len, z / len};
    }

    /** @brief Returns a normalized copy, assuming non-zero length. */
    [[nodiscard]] inline Vector3 normalizedUnsafe() const {
        float len = length();
        return Vector3{x / len, y / len, z / len}; // Assumes len != 0
    }

    /** @brief Reflects the vector over a normal, returns zero if normal is invalid. */
    [[nodiscard]] inline Vector3 reflectOver(const Vector3& normal) const {
        Vector3 unitNormal = normal.normalized();
        if (unitNormal.isZero()) {
            return Vector3{0.0f, 0.0f, 0.0f};
        }
        float dotProd = dot(unitNormal);
        return *this - unitNormal * (2.0f * dotProd);
    }

    /** @brief Projects the vector onto a target, returns zero if target is near zero. */
    [[nodiscard]] inline Vector3 projectOnto(const Vector3& target) const {
        float lenSq = target.lengthSquared();
        if (lenSq < TiMath::EPSILON) {
            return Vector3{0.0f, 0.0f, 0.0f};
        }
        float scalar = dot(target) / lenSq;
        return target * scalar;
    }

    /** @brief Rotates the vector around an axis by an angle (degrees), returns self if axis is invalid. */
    [[nodiscard]] inline Vector3 rotate(float angleDegrees, const Vector3& axis) const {
        Vector3 unitAxis = axis.normalized();
        if (unitAxis.isZero()) {
            return *this; // No rotation if axis is invalid
        }
        float angleRad = angleDegrees * static_cast<float>(TiMath::PI) / 180.0f;
        float cosA = std::cos(angleRad);
        float sinA = std::sin(angleRad);
        float oneMinusCos = 1.0f - cosA;

        // Rodrigues' rotation formula
        Vector3 term1 = *this * cosA;
        Vector3 term2 = unitAxis.cross(*this) * sinA;
        Vector3 term3 = unitAxis * (unitAxis.dot(*this) * oneMinusCos);
        return term1 + term2 + term3;
    }

    /** @brief Returns a perpendicular vector, or zero if invalid. */
    [[nodiscard]] inline Vector3 perpendicular() const {
        Vector3 arbitrary = std::abs(x) > TiMath::EPSILON ? unitY : unitX;
        Vector3 perp = cross(arbitrary).normalized();
        return perp.isZero() ? Vector3{0.0f, 0.0f, 0.0f} : perp;
    }

    /** @brief Returns the angle (radians) between this and another vector. */
    [[nodiscard]] inline float angleBetween(const Vector3& other) const {
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

    /** @brief Checks if the vector is near zero within epsilon. */
    [[nodiscard]] inline bool isZero(float epsilon = TiMath::EPSILON) const {
        return std::fabs(x) < epsilon && std::fabs(y) < epsilon && std::fabs(z) < epsilon;
    }

    /** @brief Checks if all components are finite. */
    [[nodiscard]] inline bool isFinite() const {
        return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
    }

    /** @brief Compares vectors for approximate equality within epsilon. */
    [[nodiscard]] inline bool operator==(const Vector3& other) const {
        return std::fabs(x - other.x) < TiMath::EPSILON &&
               std::fabs(y - other.y) < TiMath::EPSILON &&
               std::fabs(z - other.z) < TiMath::EPSILON;
    }

    /** @brief Compares vectors for inequality. */
    [[nodiscard]] inline bool operator!=(const Vector3& other) const {
        return !(*this == other);
    }

    /** @brief Linearly interpolates between two vectors. */
    [[nodiscard]] static inline Vector3 lerp(const Vector3& v1, const Vector3& v2, float t) {
        return Vector3(
            v1.x + t * (v2.x - v1.x),
            v1.y + t * (v2.y - v1.y),
            v1.z + t * (v2.z - v1.z)
        );
    }

    /** @brief Returns a vector with component-wise minimums. */
    [[nodiscard]] static inline Vector3 min(const Vector3& a, const Vector3& b) {
        return Vector3(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
    }

    /** @brief Returns a vector with component-wise maximums. */
    [[nodiscard]] static inline Vector3 max(const Vector3& a, const Vector3& b) {
        return Vector3(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
    }

    /** @brief Clamps each component within a range. */
    [[nodiscard]] inline Vector3 clamp(float minVal, float maxVal) const {
        return Vector3(
            std::clamp(x, minVal, maxVal),
            std::clamp(y, minVal, maxVal),
            std::clamp(z, minVal, maxVal)
        );
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector3& v);
};

// Static constant definitions
inline constexpr Vector3 Vector3::zero{0.0f, 0.0f, 0.0f};
inline constexpr Vector3 Vector3::unitX{1.0f, 0.0f, 0.0f};
inline constexpr Vector3 Vector3::unitY{0.0f, 1.0f, 0.0f};
inline constexpr Vector3 Vector3::unitZ{0.0f, 0.0f, 1.0f};

[[nodiscard]] inline Vector3 operator*(float scalar, const Vector3& v) {
    return v * scalar;
}

} // namespace TiMath

#endif // VECTOR3_H