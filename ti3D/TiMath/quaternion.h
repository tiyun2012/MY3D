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
    /**
     * @brief Constructs a quaternion (default identity).
     */
    constexpr Quaternion() : w(1.0f), x(0.0f), y(0.0f), z(0.0f) {}

    /**
     * @brief Constructs a quaternion with given components.
     * @param w Scalar component.
     * @param x X component of vector part.
     * @param y Y component of vector part.
     * @param z Z component of vector part.
     */
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

    /**
     * @brief Creates a quaternion from Euler angles (in degrees, ZYX order).
     * @param yaw Degrees around Z axis.
     * @param pitch Degrees around Y axis.
     * @param roll Degrees around X axis.
     * @return A quaternion representing the rotation.
     */
    [[nodiscard]] static Quaternion fromEulerAngles(float yaw, float pitch, float roll) {
        float cy = std::cos(yaw * static_cast<float>(M_PI) / 360.0f);
        float sy = std::sin(yaw * static_cast<float>(M_PI) / 360.0f);
        float cp = std::cos(pitch * static_cast<float>(M_PI) / 360.0f);
        float sp = std::sin(pitch * static_cast<float>(M_PI) / 360.0f);
        float cr = std::cos(roll * static_cast<float>(M_PI) / 360.0f);
        float sr = std::sin(roll * static_cast<float>(M_PI) / 360.0f);

        return Quaternion(
            cr * cp * cy + sr * sp * sy,
            sr * cp * cy - cr * sp * sy,
            cr * sp * cy + sr * cp * sy,
            cr * cp * sy - sr * sp * cy
        ).normalized();
    }

    /**
     * @brief Spherical linear interpolation between two quaternions.
     * @param q1 The starting quaternion.
     * @param q2 The ending quaternion.
     * @param t Interpolation factor (0.0 to 1.0).
     * @return The interpolated quaternion.
     */
    [[nodiscard]] static Quaternion slerp(const Quaternion& q1, const Quaternion& q2, float t) {
        float dot = q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
        Quaternion q2Adjusted = (dot < 0.0f) ? Quaternion(-q2.w, -q2.x, -q2.y, -q2.z) : q2;
        dot = std::max(-1.0f, std::min(1.0f, std::abs(dot)));
        float theta = std::acos(dot);
        float sinTheta = std::sin(theta);
        if (sinTheta < 1e-6f) return q1.normalized();
        float s1 = std::sin((1.0f - t) * theta) / sinTheta;
        float s2 = std::sin(t * theta) / sinTheta;
        return Quaternion(
            s1 * q1.w + s2 * q2Adjusted.w,
            s1 * q1.x + s2 * q2Adjusted.x,
            s1 * q1.y + s2 * q2Adjusted.y,
            s1 * q1.z + s2 * q2Adjusted.z
        ).normalized();
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

    // Compound multiplication
    Quaternion& operator*=(const Quaternion& q) {
        *this = *this * q;
        return *this;
    }

    // Scalar division
    [[nodiscard]] Quaternion operator/(float scalar) const {
        if (std::fabs(scalar) < 1e-6f) {
            throw std::runtime_error("Division by zero in Quaternion::operator/");
        }
        return Quaternion(w / scalar, x / scalar, y / scalar, z / scalar);
    }

    /**
     * @brief Normalizes the quaternion.
     * @return A normalized copy of the quaternion.
     * @throws std::runtime_error If the quaternion is zero-length.
     */
    [[nodiscard]] Quaternion normalized() const {
        float lenSq = w * w + x * x + y * y + z * z;
        if (lenSq < 1e-6f) {
            throw std::runtime_error("Cannot normalize zero-length quaternion");
        }
        float len = std::sqrt(lenSq);
        return Quaternion(w / len, x / len, y / len, z / len);
    }

    /**
     * @brief Returns the conjugate of the quaternion.
     * @return The conjugate quaternion.
     */
    [[nodiscard]] Quaternion conjugate() const {
        return Quaternion(w, -x, -y, -z);
    }

    /**
     * @brief Returns the inverse of the quaternion.
     * @return The inverse quaternion.
     * @throws std::runtime_error If the quaternion is zero-length.
     */
    [[nodiscard]] Quaternion inverse() const {
        float lenSq = w * w + x * x + y * y + z * z;
        if (lenSq < 1e-6f) {
            throw std::runtime_error("Cannot invert zero-length quaternion");
        }
        return conjugate() / lenSq;
    }

    /**
     * @brief Converts the quaternion to a rotation matrix.
     * @return A 4x4 rotation matrix.
     */
    [[nodiscard]] Matrix4 toMatrix() const;

    friend std::ostream& operator<<(std::ostream& os, const Quaternion& q);
};

} // namespace TiMath

#endif // QUATERNION_H