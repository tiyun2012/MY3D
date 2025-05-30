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
    /**
     * @brief Returns the length (magnitude) of the vector.
     * @return The Euclidean length of the vector.
     */
    [[nodiscard]] inline float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    /**
     * @brief Returns the squared length of the vector (faster than length()).
     * @return The squared magnitude of the vector.
     */
    [[nodiscard]] inline float lengthSquared() const {
        return x * x + y * y + z * z;
    }

    /**
     * @brief Returns a normalized copy of the vector, or zero if length is zero.
     * @return A unit vector in the same direction or (0,0,0) if zero-length.
     */
    [[nodiscard]] inline Vector3 normalized() const {
        float lenSq = lengthSquared();
        if (lenSq < 1e-6f) return Vector3(0.0f, 0.0f, 0.0f);
        float len = std::sqrt(lenSq);
        return *this / len;
    }

    /**
     * @brief Returns the cross product of this vector and another.
     * @param other The vector to compute the cross product with.
     * @return The cross product vector.
     */
    [[nodiscard]] inline Vector3 cross(const Vector3& other) const {
        return Vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    /**
     * @brief Returns the dot product of this vector and another.
     * @param other The vector to compute the dot product with.
     * @return The dot product.
     */
    [[nodiscard]] inline float dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    /**
     * @brief Returns the angle (in radians) between this vector and another.
     * @param other The vector to compute the angle with.
     * @return The angle in radians.
     */
    [[nodiscard]] float angleBetween(const Vector3& other) const;

    /**
     * @brief Projects this vector onto another vector.
     * @param other The vector to project onto.
     * @return The projected vector.
     */
    [[nodiscard]] Vector3 projectOnto(const Vector3& other) const;

    /**
     * @brief Reflects this vector over a normal vector.
     * @param normal The normal vector to reflect over (must be non-zero).
     * @return The reflected vector.
     */
    [[nodiscard]] Vector3 reflectOver(const Vector3& normal) const;

    /**
     * @brief Returns the distance to another vector.
     * @param other The target vector.
     * @return The Euclidean distance between this vector and the target.
     */
    [[nodiscard]] inline float distanceTo(const Vector3& other) const {
        return (*this - other).length();
    }

    // Comparison operators
    /**
     * @brief Checks if the vector is approximately zero.
     * @return True if all components are near zero.
     */
    [[nodiscard]] inline bool isZero() const {
        const float epsilon = 1e-6f;
        return std::fabs(x) < epsilon && std::fabs(y) < epsilon && std::fabs(z) < epsilon;
    }
    /**
     * @brief Checks if all components are finite (not NaN or Infinity).
     * @return True if all components are finite, false otherwise.
     */
    [[nodiscard]] inline bool isFinite() const {
        return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
    }
    /**
     * @brief Checks if two vectors are approximately equal.
     * @param other The vector to compare with.
     * @return True if the vectors are approximately equal.
     */
    [[nodiscard]] inline bool operator==(const Vector3& other) const {
        const float epsilon = 1e-6f;
        return std::fabs(x - other.x) < epsilon &&
               std::fabs(y - other.y) < epsilon &&
               std::fabs(z - other.z) < epsilon;
    }

    /**
     * @brief Checks if two vectors are not approximately equal.
     * @param other The vector to compare with.
     * @return True if the vectors are not approximately equal.
     */
    [[nodiscard]] inline bool operator!=(const Vector3& other) const {
        return !(*this == other);
    }

    // Static utility methods
    /**
     * @brief Linearly interpolates between two vectors.
     * @param v1 The starting vector.
     * @param v2 The ending vector.
     * @param t Interpolation factor (0.0 to 1.0).
     * @return The interpolated vector.
     */
    [[nodiscard]] static inline Vector3 lerp(const Vector3& v1, const Vector3& v2, float t) {
        return Vector3(
            v1.x + t * (v2.x - v1.x),
            v1.y + t * (v2.y - v1.y),
            v1.z + t * (v2.z - v1.z)
        );
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector3& v);
    friend Vector3 operator*(float scalar, const Vector3& v);
};

// Non-member operator for scalar multiplication
[[nodiscard]] inline Vector3 operator*(float scalar, const Vector3& v) {
    return v * scalar;
}

} // namespace TiMath

#endif // VECTOR3_H