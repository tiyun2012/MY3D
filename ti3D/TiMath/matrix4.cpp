#include "Matrix4.h"
#include <algorithm>
#include <iomanip>

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
        throw std::runtime_error("Zero-length axis in Matrix4::rotationAxis");
    }
    float angleRadians = angleDegrees * static_cast<float>(M_PI) / 180.0f;
    float c = std::cos(angleRadians);
    float s = std::sin(angleRadians);
    float t = 1.0f - c;
    float x = a.x, y = a.y, z = a.z;

    result.m[0] = t * x * x + c;
    result.m[1] = t * x * y + s * z;
    result.m[2] = t * x * z - s * y;
    result.m[3] = 0.0f;

    result.m[4] = t * x * y - s * z;
    result.m[5] = t * y * y + c;
    result.m[6] = t * y * z + s * x;
    result.m[7] = 0.0f;

    result.m[8] = t * x * z + s * y;
    result.m[9] = t * y * z - s * x;
    result.m[10] = t * z * z + c;
    result.m[11] = 0.0f;

    result.m[12] = 0.0f;
    result.m[13] = 0.0f;
    result.m[14] = 0.0f;
    result.m[15] = 1.0f;

    return result;
}

Matrix4 Matrix4::perspective(float fovYDegrees, float aspect, float zNear, float zFar) {
    if (std::fabs(aspect) < 1e-6f || std::fabs(zNear - zFar) < 1e-6f) {
        throw std::runtime_error("Invalid perspective parameters");
    }
    Matrix4 result;
    float f = 1.0f / std::tan(fovYDegrees * static_cast<float>(M_PI) / 360.0f);
    result.m[0] = f / aspect;
    result.m[1] = 0.0f; result.m[2] = 0.0f; result.m[3] = 0.0f;
    result.m[4] = 0.0f; result.m[5] = f; result.m[6] = 0.0f; result.m[7] = 0.0f;
    result.m[8] = 0.0f; result.m[9] = 0.0f;
    result.m[10] = (zFar + zNear) / (zNear - zFar);
    result.m[11] = -1.0f;
    result.m[12] = 0.0f; result.m[13] = 0.0f;
    result.m[14] = (2.0f * zFar * zNear) / (zNear - zFar);
    result.m[15] = 0.0f;
    return result;
}

Matrix4 Matrix4::inverse() const {
    Matrix4 result;
    float r00 = m[0], r01 = m[4], r02 = m[8];
    float r10 = m[1], r11 = m[5], r12 = m[9];
    float r20 = m[2], r21 = m[6], r22 = m[10];

    result.m[0] = r00; result.m[4] = r10; result.m[8] = r20;
    result.m[1] = r01; result.m[5] = r11; result.m[9] = r21;
    result.m[2] = r02; result.m[6] = r12; result.m[10] = r22;
    result.m[3] = 0.0f; result.m[7] = 0.0f; result.m[11] = 0.0f;
    result.m[15] = 1.0f;

    float tx = m[12], ty = m[13], tz = m[14];
    result.m[12] = -(r00 * tx + r10 * ty + r20 * tz);
    result.m[13] = -(r01 * tx + r11 * ty + r21 * tz);
    result.m[14] = -(r02 * tx + r12 * ty + r22 * tz);

    return result;
}

std::ostream& operator<<(std::ostream& os, const Matrix4& mat) {
    os << std::fixed << std::setprecision(2);
    os << "| " << std::setw(6) << mat.m[0] << " " << std::setw(6) << mat.m[4] << " "
       << std::setw(6) << mat.m[8] << " " << std::setw(6) << mat.m[12] << " |\n";
    os << "| " << std::setw(6) << mat.m[1] << " " << std::setw(6) << mat.m[5] << " "
       << std::setw(6) << mat.m[9] << " " << std::setw(6) << mat.m[13] << " |\n";
    os << "| " << std::setw(6) << mat.m[2] << " " << std::setw(6) << mat.m[6] << " "
       << std::setw(6) << mat.m[10] << " " << std::setw(6) << mat.m[14] << " |\n";
    os << "| " << std::setw(6) << mat.m[3] << " " << std::setw(6) << mat.m[7] << " "
       << std::setw(6) << mat.m[11] << " " << std::setw(6) << mat.m[15] << " |";
    return os;
}

} // namespace TiMath