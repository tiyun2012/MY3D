#include "Vector2.h"
#include <cmath>

namespace TiMath {

std::ostream& operator<<(std::ostream& os, const Vector2& v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

} // namespace TiMath