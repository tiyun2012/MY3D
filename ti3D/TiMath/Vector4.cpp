#include "Vector4.h"
#include <cmath>

namespace TiMath {

std::ostream& operator<<(std::ostream& os, const Vector4& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
    return os;
}

} // namespace TiMath