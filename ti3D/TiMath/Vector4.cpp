#include "Vector4.h"
#include "Matrix4.h"

namespace TiMath {

// Static constant definitions
const Vector4 Vector4::zero(0.0f, 0.0f, 0.0f, 0.0f);
const Vector4 Vector4::unitX(1.0f, 0.0f, 0.0f, 0.0f);
const Vector4 Vector4::unitY(0.0f, 1.0f, 0.0f, 0.0f);
const Vector4 Vector4::unitZ(0.0f, 0.0f, 1.0f, 0.0f);
const Vector4 Vector4::unitW(0.0f, 0.0f, 0.0f, 1.0f);

std::ostream& operator<<(std::ostream& os, const Vector4& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
    return os;
}

Vector4 operator*(const Matrix4& m, const Vector4& v) {
    return Vector4(
        m.m[0] * v.x + m.m[4] * v.y + m.m[8] * v.z + m.m[12] * v.w,
        m.m[1] * v.x + m.m[5] * v.y + m.m[9] * v.z + m.m[13] * v.w,
        m.m[2] * v.x + m.m[6] * v.y + m.m[10] * v.z + m.m[14] * v.w,
        m.m[3] * v.x + m.m[7] * v.y + m.m[11] * v.z + m.m[15] * v.w
    );
}

} // namespace TiMath