#ifndef MATRIX4_H
#define MATRIX4_H

#include <array>
#include <cmath>
#include <ostream>
#include "vector3.h" // Assuming Vector3 is defined in vector3.h

class Matrix4 {
public:
    std::array<float, 16> m; // Column-major order

    // Default constructor: initializes to identity matrix at compile time
    constexpr Matrix4() : m{1.0f, 0.0f, 0.0f, 0.0f,  // Column 0
                            0.0f, 1.0f, 0.0f, 0.0f,  // Column 1
                            0.0f, 0.0f, 1.0f, 0.0f,  // Column 2
                            0.0f, 0.0f, 0.0f, 1.0f}  // Column 3
    {}

    // Loads the identity matrix (non-constexpr due to runtime modification)
    void loadIdentity() ;
    [[nodiscard]] static const Matrix4 getIdentity() {
        static constexpr Matrix4 identity;

        return identity;
    }

    // Matrix multiplication
    [[nodiscard]] Matrix4 operator*(const Matrix4& other) const;

    // Static methods to create specific matrices
    [[nodiscard]] static Matrix4 translation(const Vector3& v);
    [[nodiscard]] static Matrix4 rotationAxis(const Vector3& axis, float angleDegrees);
    [[nodiscard]] static Matrix4 perspective(float fovYDegrees, float aspect, float zNear, float zFar);

    // Friend function for output streaming
    friend std::ostream& operator<<(std::ostream& os, const Matrix4& mat);
};

#endif // MATRIX4_H