// Define a 3D vector class
#include"Vector3.h"

// Constructors
Vector3::Vector3() : x(0), y(0), z(0) {};
Vector3::Vector3(float xi, float yi, float zi) : x(xi), y(yi), z(zi) {}
// ++++++++ ARITHMETIC OPERATORS ++++++++
    // Vector addition, not change argument and object
 Vector3 Vector3::operator+(const Vector3 & v) const {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }

// Vector subtraction
Vector3 Vector3::operator-(const Vector3& v) const {
    return Vector3(x - v.x, y - v.y, z - v.z);
}

// Scalar multiplication
Vector3 Vector3::operator*(float s) const {
    return Vector3(x * s, y * s, z * s);
}
// Scalar multiplication
float Vector3::operator*(const Vector3& v) const {
    return (x * v.x+ y * v.y+ z * v.z);
}


    // Scalar division
Vector3 Vector3::operator/(float s) const {
    return Vector3(x / s, y / s, z / s);
}

// Unary minus operator
Vector3 Vector3::operator-() const {
    return Vector3(-x, -y, -z);
}
// compound assignment operators
Vector3 &Vector3::operator+=(const Vector3 &v)
{
    return *this = *this + v;
}

// Vector length
float Vector3::length() const {
    return sqrtf(x * x + y * y + z * z);
}

// Normalize vector
Vector3 Vector3::normalized() const {
    float len = length();
    if (len > 0)
        return (*this) / len;
    else
        return Vector3(0, 0, 0);
}

// Cross product
/*

*/
Vector3 Vector3::cross(const Vector3& v) const {
    return Vector3(y * v.z - z * v.y,
        z * v.x - x * v.z,
        x * v.y - y * v.x);
}

// Dot product
float Vector3::dot(const Vector3& v) const {
    return x * v.x + y * v.y + z * v.z;
}
