#ifndef MATRIX4_H
#define MATRIX4_H

#include <array>
#include <cmath>
#include <ostream>
#include "Vector3.h"

namespace TiMath {

/**
 * @class Matrix4
 * @brief A 4x4 matrix class for 3D transformations.
 */
class Matrix4 {
public:
    std::array<float, 16> m; // Column-major order

    // Default constructor: initializes to identity matrix
    constexpr Matrix4() : m{1.0f, 0.0f, 0.0f, 0.0f,
                            0.0f, 1.0f, 0.0f, 0.0f,
                            0.0f, 0.0f, 1.0f, 0.0f,
                            0.0f, 0.0f, 0.0f, 1.0f} {}

    // Loads the identity matrix
    void loadIdentity();

    // Get identity matrix
    [[nodiscard]] static constexpr Matrix4 getIdentity() {
        return Matrix4();
    }

    // Matrix multiplication
    [[nodiscard]] Matrix4 operator*(const Matrix4& other) const;

    // Static transformation methods
    [[nodiscard]] static Matrix4 translation(const Vector3& v);
    [[nodiscard]] static Matrix4 rotationAxis(const Vector3& axis, float angleDegrees);
    [[nodiscard]] static Matrix4 perspective(float fovYDegrees, float aspect, float zNear, float zFar);

    // Inverse matrix
    [[nodiscard]] Matrix4 inverse() const;

    friend std::ostream& operator<<(std::ostream& os, const Matrix4& mat);
};

} // namespace TiMath

#endif // MATRIX4_H