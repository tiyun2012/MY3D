#ifndef MATRIX4_H
#define MATRIX4_H

#include <array>
#include <cmath>
#include <ostream>
#include "Vector3.h"

namespace TiMath {

class Vector4; // Forward declaration
class Quaternion; // Forward declaration

/**
 * @class Matrix4
 * @brief A 4x4 matrix class for 3D transformations (column-major order).
 */
class Matrix4 {
public:
    std::array<float, 16> m; // Column-major order

    // Default constructor: initializes to identity matrix
    constexpr Matrix4() : m{1.0f, 0.0f, 0.0f, 0.0f,
                            0.0f, 1.0f, 0.0f, 0.0f,
                            0.0f, 0.0f, 1.0f, 0.0f,
                            0.0f, 0.0f, 0.0f, 1.0f} {}

    /**
     * @brief Loads the identity matrix.
     */
    void loadIdentity();

    /**
     * @brief Returns the identity matrix.
     * @return A 4x4 identity matrix.
     */
    [[nodiscard]] static constexpr Matrix4 getIdentity() {
        return Matrix4();
    }

    /**
     * @brief Multiplies this matrix by another.
     * @param other The matrix to multiply with.
     * @return The resulting matrix.
     */
    [[nodiscard]] Matrix4 operator*(const Matrix4& other) const;

    /**
     * @brief Creates a translation matrix.
     * @param v The translation vector.
     * @return A 4x4 translation matrix.
     */
    [[nodiscard]] static Matrix4 translation(const Vector3& v);

    /**
     * @brief Creates a rotation matrix from an axis and angle.
     * @param axis The rotation axis (must be non-zero).
     * @param angleDegrees The rotation angle in degrees.
     * @return A 4x4 rotation matrix.
     */
    [[nodiscard]] static Matrix4 rotationAxis(const Vector3& axis, float angleDegrees);

    /**
     * @brief Creates a scaling matrix.
     * @param s The scaling factors for x, y, z.
     * @return A 4x4 scaling matrix.
     */
    [[nodiscard]] static Matrix4 scaling(const Vector3& s);

    /**
     * @brief Creates a perspective projection matrix.
     * @param fovYDegrees Vertical field of view in degrees.
     * @param aspect Aspect ratio (width/height).
     * @param zNear Near clipping plane.
     * @param zFar Far clipping plane.
     * @return A 4x4 perspective projection matrix.
     */
    [[nodiscard]] static Matrix4 perspective(float fovYDegrees, float aspect, float zNear, float zFar);

    /**
     * @brief Creates an orthographic projection matrix.
     * @param left Left plane coordinate.
     * @param right Right plane coordinate.
     * @param bottom Bottom plane coordinate.
     * @param top Top plane coordinate.
     * @param near Near clipping plane.
     * @param far Far clipping plane.
     * @return A 4x4 orthographic projection matrix.
     */
    [[nodiscard]] static Matrix4 orthographic(float left, float right, float bottom, float top, float near, float far);

    /**
     * @brief Creates a frustum projection matrix.
     * @param left Left plane coordinate.
     * @param right Right plane coordinate.
     * @param bottom Bottom plane coordinate.
     * @param top Top plane coordinate.
     * @param zNear Near clipping plane.
     * @param zFar Far clipping plane.
     * @return A 4x4 frustum projection matrix.
     */
    [[nodiscard]] static Matrix4 frustum(float left, float right, float bottom, float top, float zNear, float zFar);

    /**
     * @brief Creates a viewport transformation matrix.
     * @param x Leftmost pixel of the viewport.
     * @param y Bottommost pixel of the viewport.
     * @param width Width of the viewport in pixels.
     * @param height Height of the viewport in pixels.
     * @param zNear Near depth value (typically 0.0).
     * @param zFar Far depth value (typically 1.0).
     * @return A 4x4 viewport transformation matrix.
     */
    [[nodiscard]] static Matrix4 viewport(float x, float y, float width, float height, float zNear, float zFar);

    /**
     * @brief Creates a look-at view matrix.
     * @param eye The camera position.
     * @param target The target point to look at.
     * @param up The up vector (must be non-zero).
     * @return A 4x4 view matrix.
     */
    [[nodiscard]] static Matrix4 lookAt(const Vector3& eye, const Vector3& target, const Vector3& up);

    /**
     * @brief Computes the inverse of the matrix.
     * @return The inverse matrix.
     */
    [[nodiscard]] Matrix4 inverse() const;

    /**
     * @brief Converts the matrix to a quaternion representing its rotation component.
     * @return The rotation as a quaternion.
     */
    [[nodiscard]] Quaternion toQuaternion() const;

    /**
     * @brief Decomposes the matrix into translation, rotation (as a quaternion), and scaling components.
     * @return A tuple containing the translation (Vector3), rotation (Quaternion), and scaling (Vector3).
     */
    [[nodiscard]] std::tuple<Vector3, Quaternion, Vector3> decompose() const;

    /**
     * @brief Returns the matrix as an array for graphics APIs.
     * @return A pointer to the column-major array.
     */
    [[nodiscard]] const float* toArray() const {
        return m.data();
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix4& mat);
    friend Vector4 operator*(const Matrix4& m, const Vector4& v);
};

} // namespace TiMath

#endif // MATRIX4_H