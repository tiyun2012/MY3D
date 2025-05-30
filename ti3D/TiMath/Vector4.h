#ifndef VECTOR4_H
#define VECTOR4_H

#include <cmath>
#include <iostream>
#include "Vector3.h"

namespace TiMath {

class Matrix4; // Forward declaration

/**
 * @class Vector4
 * @brief A 4D vector class for homogeneous coordinates in 3D graphics.
 */
class Vector4 {
public:
    float x, y, z, w;

    // Static constants (declarations only)
    static const Vector4 zero;
    static const Vector4 unitX;
    static const Vector4 unitY;
    static const Vector4 unitZ;
    static const Vector4 unitW;

    // Constructors
    /**
     * @brief Constructs a Vector4 with given components.
     * @param x X component.
     * @param y Y component.
     * @param z Z component.
     * @param w W component (default 1.0 for points).
     */
    constexpr Vector4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f)
        : x(x), y(y), z(z), w(w) {}

    /**
     * @brief Constructs a Vector4 from a Vector3 and w component.
     * @param v The 3D vector for x, y, z components.
     * @param w The w component (default 1.0 for points).
     */
    explicit Vector4(const Vector3& v, float w = 1.0f) : x(v.x), y(v.y), z(v.z), w(w) {}

    /**
     * @brief Converts to Vector3, performing perspective divide if w != 0.
     * @return A Vector3 with x/w, y/w, z/w if w is non-zero; otherwise x, y, z.
     */
    Vector3 toVector3() const {
        if (std::fabs(w) < 1e-6f) {
            return Vector3(x, y, z);
        }
        return Vector3(x / w, y / w, z / w);
    }

    // Arithmetic operations
    /**
     * @brief Adds another Vector4.
     * @param other The vector to add.
     * @return The resulting vector.
     */
    Vector4 operator+(const Vector4& other) const {
        return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    /**
     * @brief Subtracts another Vector4.
     * @param other The vector to subtract.
     * @return The resulting vector.
     */
    Vector4 operator-(const Vector4& other) const {
        return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    /**
     * @brief Scales the vector by a scalar.
     * @param scalar The scaling factor.
     * @return The scaled vector.
     */
    Vector4 operator*(float scalar) const {
        return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    /**
     * @brief Divides the vector by a scalar.
     * @param scalar The divisor.
     * @return The resulting vector.
     * @throws std::runtime_error if scalar is near zero.
     */
    Vector4 operator/(float scalar) const {
        if (std::fabs(scalar) < 1e-6f) {
            throw std::runtime_error("Division by zero in Vector4::operator/");
        }
        return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
    }

    // Compound assignment operators
    /**
     * @brief Adds another Vector4 to this one.
     * @param other The vector to add.
     * @return Reference to this vector.
     */
    Vector4& operator+=(const Vector4& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }

    /**
     * @brief Subtracts another Vector4 from this one.
     * @param other The vector to subtract.
     * @return Reference to this vector.
     */
    Vector4& operator-=(const Vector4& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
    }

    /**
     * @brief Scales this vector by a scalar.
     * @param scalar The scaling factor.
     * @return Reference to this vector.
     */
    Vector4& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }

    /**
     * @brief Divides this vector by a scalar.
     * @param scalar The divisor.
     * @return Reference to this vector.
     * @throws std::runtime_error if scalar is near zero.
     */
    Vector4& operator/=(float scalar) {
        if (std::fabs(scalar) < 1e-6f) {
            throw std::runtime_error("Division by zero in Vector4::operator/=");
        }
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
        return *this;
    }

    /**
     * @brief Computes the dot product with another Vector4.
     * @param other The other vector.
     * @return The dot product.
     */
    float dot(const Vector4& other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    /**
     * @brief Returns a normalized copy of the vector.
     * @return A unit vector or (0,0,0,0) if zero-length.
     */
    Vector4 normalized() const {
        float magSq = x * x + y * y + z * z + w * w;
        if (magSq < 1e-6f) {
            return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
        }
        float mag = std::sqrt(magSq);
        return Vector4(x / mag, y / mag, z / mag, w / mag);
    }

    /**
     * @brief Returns the distance to another vector.
     * @param other The target vector.
     * @return The Euclidean distance between this vector and the target.
     */
    [[nodiscard]] inline float distanceTo(const Vector4& other) const {
        return (*this - other).length();
    }

    /**
     * @brief Returns the length (magnitude) of the vector.
     * @return The Euclidean length of the vector.
     */
    [[nodiscard]] inline float length() const {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }

    /**
     * @brief Returns the squared length of the vector (faster than length()).
     * @return The squared magnitude of the vector.
     */
    [[nodiscard]] inline float lengthSquared() const {
        return x * x + y * y + z * z + w * w;
    }

    /**
     * @brief Checks if the vector is approximately zero.
     * @return True if all components are near zero.
     */
    bool isZero() const {
        const float epsilon = 1e-6f;
        return std::fabs(x) < epsilon && std::fabs(y) < epsilon &&
               std::fabs(z) < epsilon && std::fabs(w) < epsilon;
    }

    /**
     * @brief Checks if two vectors are approximately equal.
     * @param other The vector to compare with.
     * @return True if the vectors are approximately equal.
     */
    bool operator==(const Vector4& other) const {
        const float epsilon = 1e-6f;
        return std::fabs(x - other.x) < epsilon &&
               std::fabs(y - other.y) < epsilon &&
               std::fabs(z - other.z) < epsilon &&
               std::fabs(w - other.w) < epsilon;
    }

    /**
     * @brief Checks if two vectors are not approximately equal.
     * @param other The vector to compare with.
     * @return True if the vectors are not approximately equal.
     */
    bool operator!=(const Vector4& other) const {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector4& v);
    friend Vector4 operator*(float scalar, const Vector4& v);
    friend Vector4 operator*(const Matrix4& m, const Vector4& v);
};

// Non-member operators
inline Vector4 operator*(float scalar, const Vector4& v) {
    return v * scalar;
}

} // namespace TiMath

#endif // VECTOR4_H