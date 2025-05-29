#include "quaternion.h"

std::ostream &operator<<(std::ostream &os, const Quaternion &q)
{
    {
        os << "Quaternion(" << q.w << ", " << q.x << ", " << q.y << ", " << q.z << ")";
        return os;
    };
}