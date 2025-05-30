#ifndef VECTOR4_H
#define VECTOR4_H

#include <cmath>

namespace TiMath {

class Vector4 {
public:
    float x, y, z, w;

    // Constructors
    constexpr Vector4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f)
        : x(x), y(y), z(z), w(w) {}

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
        if (std::abs(scalar) < 1e-6f) {
            return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
        }
        return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
    }

    // Dot product
    float dot(const Vector4& other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    // Normalize (useful for shaders)
    Vector4 normalized() const {
        float mag = std::sqrt(x * x + y * y + z * z + w * w);
        if (mag < 1e-6f) {
            return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
        }
        return Vector4(x / mag, y / mag, z / mag, w / mag);
    }

    // Check if zero
    bool isZero() const {
        return std::abs(x) < 1e-6f && std::abs(y) < 1e-6f && std::abs(z) < 1e-6f && std::abs(w) < 1e-6f;
    }
};

// Matrix-Vector multiplication (for Camera.h and shaders)
class Matrix4; // Forward declaration
Vector4 operator*(const Matrix4& m, const Vector4& v);

} // namespace TiMath

#endif // VECTOR4_H