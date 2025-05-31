#ifndef QUATERNION_H
#define QUATERNION_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm> // For std::clamp
#include <stdexcept>
#include <ostream>
#include "TiMathConfig.h"
#include "Vector3.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace TiMath {

class Quaternion {
public:
    float x, y, z, w; // Imaginary (x, y, z), real (w)

    // Static constants (declarations only)
    static const Quaternion identity;

    constexpr Quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f) : x(x), y(y), z(z), w(w) {}

    // Arithmetic operators
    /** @brief Adds two quaternions component-wise. */
    [[nodiscard]] inline Quaternion operator+(const Quaternion& other) const {
        return Quaternion(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    /** @brief Subtracts two quaternions component-wise. */
    [[nodiscard]] inline Quaternion operator-(const Quaternion& other) const {
        return Quaternion(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    /** @brief Negates all components of the quaternion. */
    [[nodiscard]] inline Quaternion operator-() const {
        return Quaternion(-x, -y, -z, -w);
    }

    /** @brief Scales the quaternion by a scalar. */
    [[nodiscard]] inline Quaternion operator*(float scalar) const {
        return Quaternion(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    /** @brief Multiplies two quaternions (Hamilton product). */
    [[nodiscard]] inline Quaternion operator*(const Quaternion& other) const {
        return Quaternion(
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w,
            w * other.w - x * other.x - y * other.y - z * other.z
        );
    }

    /** @brief Divides the quaternion by a scalar, returns identity if scalar is near zero. */
    [[nodiscard]] inline Quaternion operator/(float scalar) const {
        if (std::fabs(scalar) < TiMath::EPSILON) {
            return Quaternion(0.0f, 0.0f, 0.0f, 1.0f); // Safe default: identity
        }
        return Quaternion(x / scalar, y / scalar, z / scalar, w / scalar);
    }

    /** @brief Adds another quaternion to this one. */
    inline Quaternion& operator+=(const Quaternion& other) {
        x += other.x; y += other.y; z += other.z; w += other.w;
        return *this;
    }

    /** @brief Subtracts another quaternion from this one. */
    inline Quaternion& operator-=(const Quaternion& other) {
        x -= other.x; y -= other.y; z -= other.z; w -= other.w;
        return *this;
    }

    /** @brief Scales this quaternion by a scalar. */
    inline Quaternion& operator*=(float scalar) {
        x *= scalar; y *= scalar; z *= scalar; w *= scalar;
        return *this;
    }

    /** @brief Multiplies this quaternion by another (Hamilton product). */
    inline Quaternion& operator*=(const Quaternion& other) {
        *this = *this * other;
        return *this;
    }

    /** @brief Divides this quaternion by a scalar, sets to identity if scalar is near zero. */
    inline Quaternion& operator/=(float scalar) {
        if (std::fabs(scalar) < TiMath::EPSILON) {
            x = y = z = 0.0f; w = 1.0f; // Safe default: identity
            return *this;
        }
        x /= scalar; y /= scalar; z /= scalar; w /= scalar;
        return *this;
    }

    // Quaternion operations
    /** @brief Computes the dot product with another quaternion. */
    [[nodiscard]] constexpr float dot(const Quaternion& other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    /** @brief Returns the length (magnitude) of the quaternion. */
    [[nodiscard]] inline float length() const {
        return std::sqrt(lengthSquared());
    }

    /** @brief Returns the squared length of the quaternion. */
    [[nodiscard]] constexpr float lengthSquared() const {
        return x * x + y * y + z * z + w * w;
    }

    /** @brief Returns a normalized copy of the quaternion, or identity if length is near zero. */
    [[nodiscard]] inline Quaternion normalized() const {
        float len = length();
        return len < TiMath::EPSILON ? Quaternion(0.0f, 0.0f, 0.0f, 1.0f) : Quaternion(x / len, y / len, z / len, w / len);
    }

    /** @brief Returns a normalized copy, assuming non-zero length. */
    [[nodiscard]] inline Quaternion normalizedUnsafe() const {
        float len = length();
        return Quaternion(x / len, y / len, z / len, w / len); // Assumes len != 0
    }

    /** @brief Returns the conjugate of the quaternion. */
    [[nodiscard]] inline Quaternion conjugate() const {
        return Quaternion(-x, -y, -z, w);
    }

    /** @brief Returns the inverse of the quaternion, or identity if length is near zero. */
    [[nodiscard]] inline Quaternion inverse() const {
        float lenSq = lengthSquared();
        if (lenSq < TiMath::EPSILON) {
            return Quaternion(0.0f, 0.0f, 0.0f, 1.0f); // Safe default: identity
        }
        return conjugate() / lenSq;
    }

    /** @brief Rotates a vector by this quaternion. */
    [[nodiscard]] inline Vector3 rotateVector(const Vector3& v) const {
        Quaternion vQuat(v.x, v.y, v.z, 0.0f);
        Quaternion result = *this * vQuat * inverse();
        return Vector3(result.x, result.y, result.z);
    }

    /** @brief Creates a quaternion from an axis and angle (degrees). */
    [[nodiscard]] static inline Quaternion fromAxisAngle(const Vector3& axis, float angleDegrees) {
        Vector3 unitAxis = axis.normalized();
        if (unitAxis.isZero()) {
            return Quaternion(0.0f, 0.0f, 0.0f, 1.0f); // Safe default: identity
        }
        float angleRad = angleDegrees * static_cast<float>(M_PI) / 180.0f;
        float halfAngle = angleRad * 0.5f;
        float sinHalf = std::sin(halfAngle);
        return Quaternion(
            unitAxis.x * sinHalf,
            unitAxis.y * sinHalf,
            unitAxis.z * sinHalf,
            std::cos(halfAngle)
        );
    }

    /** @brief Extracts the rotation axis and angle (degrees), returns zero axis and zero angle if invalid. */
    [[nodiscard]] inline std::pair<Vector3, float> toAxisAngle() const {
        float len = std::sqrt(x * x + y * y + z * z);
        if (len < TiMath::EPSILON || std::fabs(w) > 1.0f - TiMath::EPSILON) {
            return {Vector3(0.0f, 0.0f, 0.0f), 0.0f}; // Safe default
        }
        float angleRad = 2.0f * std::acos(std::clamp(w, -1.0f, 1.0f));
        return {Vector3(x / len, y / len, z / len), angleRad * 180.0f / static_cast<float>(M_PI)};
    }

    /** @brief Spherically interpolates between two quaternions. */
    [[nodiscard]] static inline Quaternion slerp(const Quaternion& q1, const Quaternion& q2, float t) {
        Quaternion q1Norm = q1.normalized();
        Quaternion q2Norm = q2.normalized();
        float dot = q1Norm.dot(q2Norm);
        if (dot < 0.0f) { // Shortest path
            q2Norm = -q2Norm;
            dot = -dot;
        }
        if (dot > 1.0f - TiMath::EPSILON) { // Linear interpolation for near-identical quaternions
            return lerp(q1Norm, q2Norm, t).normalized();
        }
        float theta = std::acos(std::clamp(dot, -1.0f, 1.0f));
        float sinTheta = std::sin(theta);
        if (std::fabs(sinTheta) < TiMath::EPSILON) {
            return q1Norm; // Safe default
        }
        float coeff1 = std::sin((1.0f - t) * theta) / sinTheta;
        float coeff2 = std::sin(t * theta) / sinTheta;
        return (q1Norm * coeff1 + q2Norm * coeff2).normalized();
    }

    /** @brief Checks if the quaternion is near zero within epsilon (identity-like). */
    [[nodiscard]] inline bool isZero(float epsilon = TiMath::EPSILON) const {
        return std::fabs(x) < epsilon && std::fabs(y) < epsilon &&
               std::fabs(z) < epsilon && std::fabs(w - 1.0f) < epsilon;
    }

    /** @brief Checks if all components are finite. */
    [[nodiscard]] inline bool isFinite() const {
        return std::isfinite(x) && std::isfinite(y) && std::isfinite(z) && std::isfinite(w);
    }

    /** @brief Compares quaternions for approximate equality within epsilon. */
    [[nodiscard]] inline bool operator==(const Quaternion& other) const {
        return std::fabs(x - other.x) < TiMath::EPSILON &&
               std::fabs(y - other.y) < TiMath::EPSILON &&
               std::fabs(z - other.z) < TiMath::EPSILON &&
               std::fabs(w - other.w) < TiMath::EPSILON;
    }

    /** @brief Compares quaternions for inequality. */
    [[nodiscard]] inline bool operator!=(const Quaternion& other) const {
        return !(*this == other);
    }

    /** @brief Linearly interpolates between two quaternions (non-normalized). */
    [[nodiscard]] static inline Quaternion lerp(const Quaternion& q1, const Quaternion& q2, float t) {
        return Quaternion(
            q1.x + t * (q2.x - q1.x),
            q1.y + t * (q2.y - q1.y),
            q1.z + t * (q2.z - q1.z),
            q1.w + t * (q2.w - q1.w)
        );
    }

    /** @brief Returns a quaternion with component-wise minimums. */
    [[nodiscard]] static inline Quaternion min(const Quaternion& a, const Quaternion& b) {
        return Quaternion(
            std::min(a.x, b.x),
            std::min(a.y, b.y),
            std::min(a.z, b.z),
            std::min(a.w, b.w)
        );
    }

    /** @brief Returns a quaternion with component-wise maximums. */
    [[nodiscard]] static inline Quaternion max(const Quaternion& a, const Quaternion& b) {
        return Quaternion(
            std::max(a.x, b.x),
            std::max(a.y, b.y),
            std::max(a.z, b.z),
            std::max(a.w, b.w)
        );
    }

    /** @brief Clamps each component within a range. */
    [[nodiscard]] inline Quaternion clamp(float minVal, float maxVal) const {
        return Quaternion(
            std::clamp(x, minVal, maxVal),
            std::clamp(y, minVal, maxVal),
            std::clamp(z, minVal, maxVal),
            std::clamp(w, minVal, maxVal)
        );
    }

    friend std::ostream& operator<<(std::ostream& os, const Quaternion& q);
};

// Static constant definitions
inline constexpr Quaternion Quaternion::identity{0.0f, 0.0f, 0.0f, 1.0f};

[[nodiscard]] inline Quaternion operator*(float scalar, const Quaternion& q) {
    return q * scalar;
}

} // namespace TiMath

#endif // QUATERNION_H