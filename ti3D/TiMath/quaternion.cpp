#include "Quaternion.h"

namespace TiMath {

// Static constant definition
const Quaternion Quaternion::identity(1.0f, 0.0f, 0.0f, 0.0f);

Matrix4 Quaternion::toMatrix() const {
    Matrix4 result;
    float xx = x * x, yy = y * y, zz = z * z;
    float xy = x * y, xz = x * z, yz = y * z;
    float wx = w * x, wy = w * y, wz = w * z;

    result.m[0] = 1.0f - 2.0f * (yy + zz);
    result.m[1] = 2.0f * (xy + wz);
    result.m[2] = 2.0f * (xz - wy);
    result.m[3] = 0.0f;

    result.m[4] = 2.0f * (xy - wz);
    result.m[5] = 1.0f - 2.0f * (xx + zz);
    result.m[6] = 2.0f * (yz + wx);
    result.m[7] = 0.0f;

    result.m[8] = 2.0f * (xz + wy);
    result.m[9] = 2.0f * (yz - wx);
    result.m[10] = 1.0f - 2.0f * (xx + yy);
    result.m[11] = 0.0f;

    result.m[12] = 0.0f;
    result.m[13] = 0.0f;
    result.m[14] = 0.0f;
    result.m[15] = 1.0f;

    return result;
}

std::ostream& operator<<(std::ostream& os, const Quaternion& q) {
    os << "Quaternion(" << q.w << ", " << q.x << ", " << q.y << ", " << q.z << ")";
    return os;
}

} // namespace TiMath