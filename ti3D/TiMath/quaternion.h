#ifndef QUATERNION_H
#define QUATERNION_H

#include <cmath>
#include <iostream>
#include "Vector3.h"
#include "Matrix4.h"

namespace TiMath {

/**
 * @class Quaternion
 * @brief A quaternion class for 3D rotations.
 */
class Quaternion {
public:
    float w, x, y, z;

    // Static identity quaternion (declaration only)
    static const Quaternion identity;

    // Constructors
    constexpr Quaternion() : w(1.0f), x(0.0f), y(0.0f), z(0.0f) {}
    constexpr Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}

    /**
     * @brief Creates a quaternion from an axis and angle.
     * @param axis The rotation axis (must be non-zero).
     * @param angleDegrees The rotation angle in degrees.
     * @return A quaternion representing the rotation.
     * @throws std::runtime_error If the axis is zero-length.
     */
    [[nodiscard]] static Quaternion fromAxisAngle(const Vector3& axis, float angleDegrees) {
        Vector3 normAxis = axis.normalized();
        if (normAxis.isZero()) {
            throw std::runtime_error("Zero-length axis in Quaternion::fromAxisAngle");
        }
        float angleRadians = angleDegrees * static_cast<float>(M_PI) / 180.0f;
        float halfAngle = angleRadians / 2.0f;
        float sinHalfAngle = std::sin(halfAngle);
        float cosHalfAngle = std::cos(halfAngle);
        return Quaternion(
            cosHalfAngle,
            normAxis.x * sinHalfAngle,
            normAxis.y * sinHalfAngle,
            normAxis.z * sinHalfAngle
        );
    }

    // Quaternion multiplication
    [[nodiscard]] Quaternion operator*(const Quaternion& q) const {
        return Quaternion(
            w * q.w - x * q.x - y * q.y - z * q.z,
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w
        );
    }

    // Scalar division
    [[nodiscard]] Quaternion operator/(float scalar) const {
        if (std::fabs(scalar) < 1e-6f) {
            throw std::runtime_error("Division by zero in Quaternion::operator/");
        }
        return Quaternion(w / scalar, x / scalar, y / scalar, z / scalar);
    }

    // Normalize the quaternion
    [[nodiscard]] Quaternion normalized() const {
        float lenSq = w * w + x * x + y * y + z * z;
        if (lenSq < 1e-6f) {
            throw std::runtime_error("Cannot normalize zero-length quaternion");
        }
        float len = std::sqrt(lenSq);
        return *this / len;
    }

    // Conjugate
    [[nodiscard]] Quaternion conjugate() const {
        return Quaternion(w, -x, -y, -z);
    }

    // Inverse
    [[nodiscard]] Quaternion inverse() const {
        float lenSq = w * w + x * x + y * y + z * z;
        if (lenSq < 1e-6f) {
            throw std::runtime_error("Cannot invert zero-length quaternion");
        }
        return conjugate() / lenSq;
    }

    // Convert to rotation matrix
    [[nodiscard]] Matrix4 toMatrix() const;

    friend std::ostream& operator<<(std::ostream& os, const Quaternion& q);
};

} // namespace TiMath

#endif // QUATERNION_H