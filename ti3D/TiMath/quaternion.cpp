#include "Quaternion.h"
#include <cmath>

namespace TiMath {

std::ostream& operator<<(std::ostream& os, const Quaternion& q) {
    os << "(" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << ")";
    return os;
}

} // namespace TiMath