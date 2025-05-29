#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>
#include <iostream>
#include <limits>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace TiMath {

/**
 * @class Vector3
 * @brief A 3D vector class for graphics and physics calculations.
 */
class Vector3 {
public:
    float x, y, z;

    // Static constant vectors (declarations only)
    static const Vector3 zero;
    static const Vector3 unitX;
    static const Vector3 unitY;
    static const Vector3 unitZ;

    // Constructors
    constexpr Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    constexpr Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    // Arithmetic operators
    [[nodiscard]] inline Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    [[nodiscard]] inline Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    [[nodiscard]] inline Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    [[nodiscard]] inline Vector3 operator/(float scalar) const {
        if (std::fabs(scalar) < 1e-6f) {
            throw std::runtime_error("Division by zero in Vector3::operator/");
        }
        return Vector3(x / scalar, y / scalar, z / scalar);
    }

    [[nodiscard]] inline Vector3 operator-() const {
        return Vector3(-x, -y, -z);
    }

    // Compound assignment operators
    inline Vector3& operator+=(const Vector3& other) {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }

    inline Vector3& operator-=(const Vector3& other) {
        x -= other.x; y -= other.y; z -= other.z;
        return *this;
    }

    inline Vector3& operator*=(float scalar) {
        x *= scalar; y *= scalar; z *= scalar;
        return *this;
    }

    inline Vector3& operator/=(float scalar) {
        if (std::fabs(scalar) < 1e-6f) {
            throw std::runtime_error("Division by zero in Vector3::operator/=");
        }
        x /= scalar; y /= scalar; z /= scalar;
        return *this;
    }

    // Vector operations
    /** @brief Returns the length (magnitude) of the vector. */
    [[nodiscard]] inline float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    /** @brief Returns the squared length of the vector (faster than length()). */
    [[nodiscard]] inline float lengthSquared() const {
        return x * x + y * y + z * z;
    }

    /** @brief Returns a normalized copy of the vector, or zero if length is zero. */
    [[nodiscard]] inline Vector3 normalized() const {
        float lenSq = lengthSquared();
        if (lenSq < 1e-6f) return Vector3(0.0f, 0.0f, 0.0f);
        float len = std::sqrt(lenSq);
        return *this / len;
    }

    /** @brief Returns the cross product of this vector and another. */
    [[nodiscard]] inline Vector3 cross(const Vector3& other) const {
        return Vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    /** @brief Returns the dot product of this vector and another. */
    [[nodiscard]] inline float dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    /** @brief Returns the angle (in radians) between this vector and another. */
    [[nodiscard]] float angleBetween(const Vector3& other) const;

    /** @brief Projects this vector onto another vector. */
    [[nodiscard]] Vector3 projectOnto(const Vector3& other) const;

    /** @brief Reflects this vector over a normal vector. */
    [[nodiscard]] Vector3 reflectOver(const Vector3& normal) const;

    // Comparison operators
    /** @brief Checks if the vector is approximately zero. */
    [[nodiscard]] inline bool isZero() const {
        const float epsilon = 1e-6f;
        return std::fabs(x) < epsilon && std::fabs(y) < epsilon && std::fabs(z) < epsilon;
    }

    /** @brief Checks if two vectors are approximately equal. */
    [[nodiscard]] inline bool operator==(const Vector3& other) const {
        const float epsilon = 1e-6f;
        return std::fabs(x - other.x) < epsilon &&
               std::fabs(y - other.y) < epsilon &&
               std::fabs(z - other.z) < epsilon;
    }

    /** @brief Checks if two vectors are not approximately equal. */
    [[nodiscard]] inline bool operator!=(const Vector3& other) const {
        return !(*this == other);
    }

    // Static utility methods
    /** @brief Linearly interpolates between two vectors. */
    [[nodiscard]] static inline Vector3 lerp(const Vector3& v1, const Vector3& v2, float t) {
        return Vector3(
            v1.x + t * (v2.x - v1.x),
            v1.y + t * (v2.y - v1.y),
            v1.z + t * (v2.z - v1.z)
        );
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector3& v);
};

// Non-member operator for scalar multiplication
[[nodiscard]] inline Vector3 operator*(float scalar, const Vector3& v) {
    return v * scalar;
}

} // namespace TiMath

#endif // VECTOR3_H