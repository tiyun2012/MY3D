#include "Matrix4.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include "Vector4.h"

namespace TiMath {

void Matrix4::loadIdentity() {
    std::fill(m.begin(), m.end(), 0.0f);
    m[0] = m[5] = m[10] = m[15] = 1.0f;
}

Matrix4 Matrix4::operator*(const Matrix4& other) const {
    Matrix4 result;
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            result.m[col * 4 + row] =
                m[0 * 4 + row] * other.m[col * 4 + 0] +
                m[1 * 4 + row] * other.m[col * 4 + 1] +
                m[2 * 4 + row] * other.m[col * 4 + 2] +
                m[3 * 4 + row] * other.m[col * 4 + 3];
        }
    }
    return result;
}

Matrix4 Matrix4::translation(const Vector3& v) {
    Matrix4 result;
    result.m[12] = v.x;
    result.m[13] = v.y;
    result.m[14] = v.z;
    return result;
}

Matrix4 Matrix4::rotationAxis(const Vector3& axis, float angleDegrees) {
    Matrix4 result;
    Vector3 a = axis.normalized();
    if (a.isZero()) {
        std::cerr << "Warning: Zero-length axis in rotation, returning identity" << std::endl;
        return getIdentity();
    }
    float angle = angleDegrees * static_cast<float>(M_PI) / 180.0f;
    float c = std::cos(angle);
    float s = std::sin(angle);
    float t = 1.0f - c;

    result.m[0] = t * a.x * a.x + c;
    result.m[1] = t * a.x * a.y + s * a.z;
    result.m[2] = t * a.x * a.z - s * a.y;
    result.m[3] = 0.0f;

    result.m[4] = t * a.x * a.y - s * a.z;
    result.m[5] = t * a.y * a.y + c;
    result.m[6] = t * a.y * a.z + s * a.x;
    result.m[7] = 0.0f;

    result.m[8] = t * a.x * a.z + s * a.y;
    result.m[9] = t * a.y * a.z - s * a.x;
    result.m[10] = t * a.z * a.z + c;
    result.m[11] = 0.0f;

    result.m[12] = 0.0f;
    result.m[13] = 0.0f;
    result.m[14] = 0.0f;
    result.m[15] = 1.0f;

    return result;
}

Matrix4 Matrix4::scaling(const Vector3& s) {
    Matrix4 result;
    result.m[0] = s.x;
    result.m[5] = s.y;
    result.m[10] = s.z;
    return result;
}

Matrix4 Matrix4::perspective(float fovYDegrees, float aspect, float zNear, float zFar) {
    if (std::fabs(aspect) < EPSILON || std::fabs(zNear - zFar) < EPSILON) {
        std::cerr << "Warning: Invalid perspective parameters, returning identity" << std::endl;
        return getIdentity();
    }
    Matrix4 result;
    float f = 1.0f / std::tan(fovYDegrees * static_cast<float>(M_PI) / 360.0f);
    result.m[0] = f / aspect;
    result.m[5] = f;
    result.m[10] = (zFar + zNear) / (zNear - zFar);
    result.m[11] = -1.0f;
    result.m[14] = (2.0f * zFar * zNear) / (zNear - zFar);
    result.m[1] = result.m[2] = result.m[3] = result.m[4] =
    result.m[6] = result.m[7] = result.m[8] = result.m[9] =
    result.m[12] = result.m[13] = result.m[15] = 0.0f;
    return result;
}

Matrix4 Matrix4::orthographic(float left, float right, float bottom, float top, float near, float far) {
    Matrix4 result;
    result.m[0] = 2.0f / (right - left);
    result.m[5] = 2.0f / (top - bottom);
    result.m[10] = -2.0f / (far - near);
    result.m[12] = -(right + left) / (right - left);
    result.m[13] = -(top + bottom) / (top - bottom);
    result.m[14] = -(far + near) / (far - near);
    result.m[15] = 1.0f;
    result.m[1] = result.m[2] = result.m[3] = result.m[4] =
    result.m[6] = result.m[7] = result.m[8] = result.m[9] =
    result.m[11] = 0.0f;
    return result;
}

Matrix4 Matrix4::lookAt(const Vector3& eye, const Vector3& target, const Vector3& up) {
    Vector3 dir = (target - eye).normalized();
    if (dir.isZero()) {
        std::cerr << "Warning: Zero-length direction in lookAt, returning identity" << std::endl;
        return getIdentity();
    }
    Vector3 upNorm = up.normalized();
    if (upNorm.isZero()) {
        std::cerr << "Warning: Zero-length up vector in lookAt, returning identity" << std::endl;
        return getIdentity();
    }
    Vector3 right = dir.cross(upNorm).normalized();
    if (right.isZero()) {
        std::cerr << "Warning: Invalid up vector in lookAt, returning identity" << std::endl;
        return getIdentity();
    }
    Vector3 newUp = right.cross(dir).normalized();

    Matrix4 result;
    result.m[0] = right.x;  result.m[4] = right.y;  result.m[8] = right.z;
    result.m[1] = newUp.x;  result.m[5] = newUp.y;  result.m[9] = newUp.z;
    result.m[2] = -dir.x;   result.m[6] = -dir.y;   result.m[10] = -dir.z;
    result.m[3] = 0.0f;     result.m[7] = 0.0f;     result.m[11] = 0.0f;    result.m[15] = 1.0f;
    result.m[12] = -right.dot(eye);
    result.m[13] = -newUp.dot(eye);
    result.m[14] = dir.dot(eye);
    return result;
}

Matrix4 Matrix4::inverse() const {
    Matrix4 result;
    float a00 = m[0],  a01 = m[4],  a02 = m[8],  a03 = m[12];
    float a10 = m[1],  a11 = m[5],  a12 = m[9],  a13 = m[13];
    float a20 = m[2],  a21 = m[6],  a22 = m[10], a23 = m[14];
    float a30 = m[3],  a31 = m[7],  a32 = m[11], a33 = m[15];

    float b00 = a22 * a33 - a32 * a23;
    float b01 = a21 * a33 - a31 * a23;
    float b02 = a21 * a32 - a31 * a22;
    float b03 = a20 * a33 - a30 * a23;
    float b04 = a20 * a32 - a30 * a22;
    float b05 = a20 * a31 - a30 * a21;

    float det = a00 * (a11 * b00 - a12 * b01 + a13 * b02) +
                a01 * (-a10 * b00 + a12 * b03 - a13 * b04) +
                a02 * (a10 * b01 - a11 * b03 + a13 * b05) +
                a03 * (-a10 * b02 + a11 * b04 - a12 * b05);

    if (std::fabs(det) < EPSILON) {
        std::cerr << "Warning: Matrix is singular, returning identity" << std::endl;
        return getIdentity();
    }
    float invDet = 1.0f / det;

    result.m[0]  = (a11 * b00 - a12 * b01 + a13 * b02) * invDet;
    result.m[4]  = (-a01 * b00 + a02 * b01 - a03 * b02) * invDet;
    result.m[8]  = (a01 * b03 - a02 * b04 + a03 * b05) * invDet;
    result.m[12] = (-a01 * b02 + a02 * b04 - a03 * b05) * invDet;

    result.m[1]  = (-a10 * b00 + a12 * b03 - a13 * b04) * invDet;
    result.m[5]  = (a00 * b00 - a02 * b03 + a03 * b04) * invDet;
    result.m[9]  = (-a00 * b01 + a02 * b02 - a03 * b03) * invDet;
    result.m[13] = (a00 * b02 - a02 * b04 + a03 * b05) * invDet;

    result.m[2]  = (a10 * b01 - a11 * b03 + a13 * b05) * invDet;
    result.m[6]  = (-a00 * b01 + a01 * b03 - a03 * b05) * invDet;
    result.m[10] = (a00 * b00 - a01 * b03 + a03 * b04) * invDet;
    result.m[14] = (-a00 * b02 + a01 * b04 - a03 * b05) * invDet;

    result.m[3]  = (-a10 * b02 + a11 * b04 - a12 * b05) * invDet;
    result.m[7]  = (a00 * b02 - a01 * b04 + a02 * b05) * invDet;
    result.m[11] = (-a00 * b01 + a01 * b03 - a02 * b04) * invDet;
    result.m[15] = (a00 * b00 - a01 * b03 + a02 * b04) * invDet;

    return result;
}

std::ostream& operator<<(std::ostream& os, const Matrix4& mat) {
    os << std::fixed << std::setprecision(4);
    os << "[" << mat.m[0]  << ", " << mat.m[4]  << ", " << mat.m[8]  << ", " << mat.m[12] << "]\n";
    os << "[" << mat.m[1]  << ", " << mat.m[5]  << ", " << mat.m[9]  << ", " << mat.m[13] << "]\n";
    os << "[" << mat.m[2]  << ", " << mat.m[6]  << ", " << mat.m[10] << ", " << mat.m[14] << "]\n";
    os << "[" << mat.m[3]  << ", " << mat.m[7]  << ", " << mat.m[11] << ", " << mat.m[15] << "]";
    return os;
}

} // namespace TiMath