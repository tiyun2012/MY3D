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

Vector3 &Vector3::operator-=(const Vector3 &v)
{
    return *this = *this - v;
}

Vector3 &Vector3::operator*=(float s)
{
    return *this = *this * s;
}

Vector3 &Vector3::operator/=(float s)
{
    return *this = *this / s;
}

// Vector length
float Vector3::length() const {
    return sqrtf(x * x + y * y + z * z);
}

float Vector3::lengthSquared() const
{
    return x * x + y * y + z * z;
}

// Normalize vector
Vector3 Vector3::normalized() const {
    float len = length();
    return (len>0)?*this/len:Vector3(0,0,0);
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

float Vector3::angleBetween(const Vector3 &v) const
{
    float len1=this->length();
    float len2=v.length();
    if(len1==0 || len2==0)
        return 0;
    return acosf((*this*v)/(len1*len2));
}

Vector3 Vector3::projectOnto(const Vector3 &v) const
{
    float secondVectorLength = v.length();
    if(secondVectorLength == 0)
        return Vector3(0, 0, 0);
    return ((*this * v) / secondVectorLength)*v.normalized();
}

bool Vector3::isZero() const
{
    return (x == 0 && y == 0 && z == 0);
}

bool Vector3::operator==(const Vector3 &v) const
{
    return (x == v.x && y == v.y && z == v.z);
}

bool Vector3::operator!=(const Vector3 &v) const
{
    return (x!=v.x || y!=v.y || z!=v.z);
}
