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

    // Static constants
    static const Vector4 zero;
    static const Vector4 unitX;
    static const Vector4 unitY;
    static const Vector4 unitZ;
    static const Vector4 unitW;

    // Constructors
    constexpr Vector4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f)
        : x(x), y(y), z(z), w(w) {}
    explicit Vector4(const Vector3& v, float w = 1.0f) : x(v.x), y(v.y), z(v.z), w(w) {}

    // Conversion to Vector3 (perspective divide if w != 0)
    Vector3 toVector3() const {
        if (std::fabs(w) < 1e-6f) {
            return Vector3(x, y, z);
        }
        return Vector3(x / w, y / w, z / w);
    }

    // Arithmetic operations
    Vector4 operator+(const Vector4& other) const {
        return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    Vector4 operator-(const Vector4& other) const {
        return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    Vector4 operator*(float scalar) const {
        return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    Vector4 operator/(float scalar) const {
        if (std::fabs(scalar) < 1e-6f) {
            throw std::runtime_error("Division by zero in Vector4::operator/");
        }
        return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
    }

    // Compound assignment operators
    Vector4& operator+=(const Vector4& other) {
        x += other.x; y += other.y; z += other.z; w += other.w;
        return *this;
    }

    Vector4& operator-=(const Vector4& other) {
        x -= other.x; y -= other.y; z -= other.z; w -= other.w;
        return *this;
    }

    Vector4& operator*=(float scalar) {
        x *= scalar; y *= scalar; z *= scalar; w *= scalar;
        return *this;
    }

    Vector4& operator/=(float scalar) {
        if (std::fabs(scalar) < 1e-6f) {
            throw std::runtime_error("Division by zero in Vector4::operator/=");
        }
        x /= scalar; y /= scalar; z /= scalar; w /= scalar;
        return *this;
    }

    // Dot product
    float dot(const Vector4& other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    // Normalize
    Vector4 normalized() const {
        float magSq = x * x + y * y + z * z + w * w;
        if (magSq < 1e-6f) {
            return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
        }
        float mag = std::sqrt(magSq);
        return Vector4(x / mag, y / mag, z / mag, w / mag);
    }

    // Check if zero
    bool isZero() const {
        const float epsilon = 1e-6f;
        return std::fabs(x) < epsilon && std::fabs(y) < epsilon &&
               std::fabs(z) < epsilon && std::fabs(w) < epsilon;
    }

    // Comparison operators
    bool operator==(const Vector4& other) const {
        const float epsilon = 1e-6f;
        return std::fabs(x - other.x) < epsilon &&
               std::fabs(y - other.y) < epsilon &&
               std::fabs(z - other.z) < epsilon &&
               std::fabs(w - other.w) < epsilon;
    }

    bool operator!=(const Vector4& other) const {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector4& v);
};

// Non-member operators
inline Vector4 operator*(float scalar, const Vector4& v) {
    return v * scalar;
}

Vector4 operator*(const Matrix4& m, const Vector4& v);

} // namespace TiMath

#endif // VECTOR4_H