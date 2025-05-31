#include "Vector3.h"
#include <cmath>

namespace TiMath {

std::ostream& operator<<(std::ostream& os, const Vector3& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

} // namespace TiMath