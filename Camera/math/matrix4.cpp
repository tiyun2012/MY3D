#include "matrix4.h"
#include <algorithm> // For std::fill
#include <iomanip>   // For std::setw and std::fixed

// Loads the identity matrix
void Matrix4::loadIdentity() {
    std::fill(m.begin(), m.end(), 0.0f);
    m[0] = m[5] = m[10] = m[15] = 1.0f;
}

// Matrix multiplication
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

// Translation matrix
Matrix4 Matrix4::translation(const Vector3& v) {
    Matrix4 result;
    result.m[12] = v.x;
    result.m[13] = v.y;
    result.m[14] = v.z;
    return result;
}

// Rotation matrix around an arbitrary axis using Rodrigues' rotation formula
Matrix4 Matrix4::rotationAxis(const Vector3& axis, float angleDegrees) {
    Matrix4 result;

    float angleRadians = angleDegrees * static_cast<float>(M_PI) / 180.0f;
    float c = std::cos(angleRadians);
    float s = std::sin(angleRadians);
    float t = 1 - c;

    Vector3 a = axis.normalized();
    if (a.x == 0.0f && a.y == 0.0f && a.z == 0.0f) {
        return result; // Zero axis: return identity
    }
    float x = a.x, y = a.y, z = a.z;

    result.m[0] = t * x * x + c;
    result.m[1] = t * x * y + s * z;
    result.m[2] = t * x * z - s * y;
    result.m[3] = 0;

    result.m[4] = t * x * y - s * z;
    result.m[5] = t * y * y + c;
    result.m[6] = t * y * z + s * x;
    result.m[7] = 0;

    result.m[8] = t * x * z + s * y;
    result.m[9] = t * y * z - s * x;
    result.m[10] = t * z * z + c;
    result.m[11] = 0;

    result.m[12] = 0;
    result.m[13] = 0;
    result.m[14] = 0;
    result.m[15] = 1;

    return result;
}

// Perspective projection matrix
Matrix4 Matrix4::perspective(float fovYDegrees, float aspect, float zNear, float zFar) {
    Matrix4 result;
    result.loadIdentity();

    float f = 1.0f / std::tan(fovYDegrees * static_cast<float>(M_PI) / 360.0f);

    result.m[0] = f / aspect;
    result.m[5] = f;
    result.m[10] = (zFar + zNear) / (zNear - zFar);
    result.m[11] = -1;
    result.m[14] = (2 * zFar * zNear) / (zNear - zFar);
    result.m[15] = 0;

    return result;
}

// Output stream operator for Matrix4
std::ostream& operator<<(std::ostream& os, const Matrix4& mat) {
    os << std::fixed << std::setprecision(2);
    os << "| " << std::setw(6) << mat.m[0] << " " << std::setw(6) << mat.m[4] << " "
       << std::setw(6) << mat.m[8] << " " << std::setw(6) << mat.m[12] << " |" << std::endl;
    os << "| " << std::setw(6) << mat.m[1] << " " << std::setw(6) << mat.m[5] << " "
       << std::setw(6) << mat.m[9] << " " << std::setw(6) << mat.m[13] << " |" << std::endl;
    os << "| " << std::setw(6) << mat.m[2] << " " << std::setw(6) << mat.m[6] << " "
       << std::setw(6) << mat.m[10] << " " << std::setw(6) << mat.m[14] << " |" << std::endl;
    os << "| " << std::setw(6) << mat.m[3] << " " << std::setw(6) << mat.m[7] << " "
       << std::setw(6) << mat.m[11] << " " << std::setw(6) << mat.m[15] << " |";
    return os;
}