#ifndef VECTOR4_H
#define VECTOR4_H

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

class Vector4 {
public:
    float x, y, z, w;

    // Static constant declarations
    static const Vector4 zero;
    static const Vector4 unitX;
    static const Vector4 unitY;
    static const Vector4 unitZ;
    static const Vector4 unitW;

    constexpr Vector4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) : x(x), y(y), z(z), w(w) {}
    constexpr Vector4(const Vector3& v, float w = 1.0f) : x(v.x), y(v.y), z(v.z), w(w) {}

    // Arithmetic operators
    /** @brief Adds two vectors component-wise. */
    [[nodiscard]] inline Vector4 operator+(const Vector4& other) const {
        return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    /** @brief Subtracts two vectors component-wise. */
    [[nodiscard]] inline Vector4 operator-(const Vector4& other) const {
        return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    /** @brief Negates all components of the vector. */
    [[nodiscard]] inline Vector4 operator-() const {
        return Vector4(-x, -y, -z, -w);
    }

    /** @brief Scales the vector by a scalar. */
    [[nodiscard]] inline Vector4 operator*(float scalar) const {
        return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    /** @brief Divides the vector by a scalar, returns zero vector if scalar is near zero. */
    [[nodiscard]] inline Vector4 operator/(float scalar) const {
        if (std::fabs(scalar) < TiMath::EPSILON) {
            return Vector4{0.0f, 0.0f, 0.0f, 0.0f}; // Safe default
        }
        return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
    }

    /** @brief Adds another vector to this one. */
    inline Vector4& operator+=(const Vector4& other) {
        x += other.x; y += other.y; z += other.z; w += other.w;
        return *this;
    }

    /** @brief Subtracts another vector from this one. */
    inline Vector4& operator-=(const Vector4& other) {
        x -= other.x; y -= other.y; z -= other.z; w -= other.w;
        return *this;
    }

    /** @brief Scales this vector by a scalar. */
    inline Vector4& operator*=(float scalar) {
        x *= scalar; y *= scalar; z *= scalar; w *= scalar;
        return *this;
    }

    /** @brief Divides this vector by a scalar, sets to zero if scalar is near zero. */
    inline Vector4& operator/=(float scalar) {
        if (std::fabs(scalar) < TiMath::EPSILON) {
            x = y = z = w = 0.0f; // Safe default
            return *this;
        }
        x /= scalar; y /= scalar; z /= scalar; w /= scalar;
        return *this;
    }

    // Vector operations
    /** @brief Computes the dot product with another vector. */
    [[nodiscard]] constexpr float dot(const Vector4& other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    /** @brief Returns the length (magnitude) of the vector. */
    [[nodiscard]] inline float length() const {
        return std::sqrt(lengthSquared());
    }

    /** @brief Returns the squared length of the vector. */
    [[nodiscard]] constexpr float lengthSquared() const {
        return x * x + y * y + z * z + w * w;
    }

    /** @brief Returns a normalized copy of the vector, or zero if length is near zero. */
    [[nodiscard]] inline Vector4 normalized() const {
        float len = length();
        return len < TiMath::EPSILON ? Vector4{0.0f, 0.0f, 0.0f, 0.0f} : Vector4{x / len, y / len, z / len, w / len};
    }

    /** @brief Returns a normalized copy, assuming non-zero length. */
    [[nodiscard]] inline Vector4 normalizedUnsafe() const {
        float len = length();
        return Vector4{x / len, y / len, z / len, w / len}; // Assumes len != 0
    }

    /** @brief Converts homogeneous coordinates to 3D by dividing (x, y, z) by w, returns zero if w is near zero. */
    [[nodiscard]] inline Vector3 homogeneousDivide() const {
        if (std::fabs(w) < TiMath::EPSILON) {
            return Vector3{0.0f, 0.0f, 0.0f}; // Safe default
        }
        return Vector3(x / w, y / w, z / w);
    }

    /** @brief Extracts the 3D component (x, y, z) as a Vector3. */
    [[nodiscard]] inline Vector3 toVector3() const {
        return Vector3(x, y, z);
    }

    /** @brief Checks if the vector is near zero within epsilon. */
    [[nodiscard]] inline bool isZero(float epsilon = TiMath::EPSILON) const {
        return std::fabs(x) < epsilon && std::fabs(y) < epsilon &&
               std::fabs(z) < epsilon && std::fabs(w) < epsilon;
    }

    /** @brief Checks if all components are finite. */
    [[nodiscard]] inline bool isFinite() const {
        return std::isfinite(x) && std::isfinite(y) && std::isfinite(z) && std::isfinite(w);
    }

    /** @brief Compares vectors for approximate equality within epsilon. */
    [[nodiscard]] inline bool operator==(const Vector4& other) const {
        return std::fabs(x - other.x) < TiMath::EPSILON &&
               std::fabs(y - other.y) < TiMath::EPSILON &&
               std::fabs(z - other.z) < TiMath::EPSILON &&
               std::fabs(w - other.w) < TiMath::EPSILON;
    }

    /** @brief Compares vectors for inequality. */
    [[nodiscard]] inline bool operator!=(const Vector4& other) const {
        return !(*this == other);
    }

    /** @brief Linearly interpolates between two vectors. */
    [[nodiscard]] static inline Vector4 lerp(const Vector4& v1, const Vector4& v2, float t) {
        return Vector4(
            v1.x + t * (v2.x - v1.x),
            v1.y + t * (v2.y - v1.y),
            v1.z + t * (v2.z - v1.z),
            v1.w + t * (v2.w - v1.w)
        );
    }

    /** @brief Returns a vector with component-wise minimums. */
    [[nodiscard]] static inline Vector4 min(const Vector4& a, const Vector4& b) {
        return Vector4(
            std::min(a.x, b.x),
            std::min(a.y, b.y),
            std::min(a.z, b.z),
            std::min(a.w, b.w)
        );
    }

    /** @brief Returns a vector with component-wise maximums. */
    [[nodiscard]] static inline Vector4 max(const Vector4& a, const Vector4& b) {
        return Vector4(
            std::max(a.x, b.x),
            std::max(a.y, b.y),
            std::max(a.z, b.z),
            std::max(a.w, b.w)
        );
    }

    /** @brief Clamps each component within a range. */
    [[nodiscard]] inline Vector4 clamp(float minVal, float maxVal) const {
        return Vector4(
            std::clamp(x, minVal, maxVal),
            std::clamp(y, minVal, maxVal),
            std::clamp(z, minVal, maxVal),
            std::clamp(w, minVal, maxVal)
        );
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector4& v);
};

// Static constant definitions
inline constexpr Vector4 Vector4::zero{0.0f, 0.0f, 0.0f, 0.0f};
inline constexpr Vector4 Vector4::unitX{1.0f, 0.0f, 0.0f, 0.0f};
inline constexpr Vector4 Vector4::unitY{0.0f, 1.0f, 0.0f, 0.0f};
inline constexpr Vector4 Vector4::unitZ{0.0f, 0.0f, 1.0f, 0.0f};
inline constexpr Vector4 Vector4::unitW{0.0f, 0.0f, 0.0f, 1.0f};

[[nodiscard]] inline Vector4 operator*(float scalar, const Vector4& v) {
    return v * scalar;
}

} // namespace TiMath

#endif // VECTOR4_H