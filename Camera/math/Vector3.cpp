#include "vector3.h"

// Static constant definitions (One Definition Rule compliance)
const Vector3 Vector3::zero(0, 0, 0);
const Vector3 Vector3::unitX(1, 0, 0);
const Vector3 Vector3::unitY(0, 1, 0);
const Vector3 Vector3::unitZ(0, 0, 1);

// Arithmetic operators
Vector3 Vector3::operator+(const Vector3& other) const {
    return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator-(const Vector3& other) const {
    return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::operator*(float scalar) const {
    return Vector3(x * scalar, y * scalar, z * scalar);
}

Vector3 Vector3::operator/(float scalar) const {
    return Vector3(x / scalar, y / scalar, z / scalar);
}

Vector3 Vector3::operator-() const {
    return Vector3(-x, -y, -z);
}

// Compound assignment operators
Vector3& Vector3::operator+=(const Vector3& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vector3& Vector3::operator-=(const Vector3& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

Vector3& Vector3::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Vector3& Vector3::operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}

// Vector operations
Vector3 Vector3::normalized() const {
    float len = length();
    if (len < 1e-6f) return Vector3(0, 0, 0); // Avoid division by near-zero
    return *this / len;
}

Vector3 Vector3::cross(const Vector3& other) const {
    return Vector3(y * other.z - z * other.y,
                   z * other.x - x * other.z,
                   x * other.y - y * other.x);
}

float Vector3::angleBetween(const Vector3& other) const {
    float dotProd = dot(other);
    float lenSq1 = lengthSquared();
    float lenSq2 = other.lengthSquared();
    if (lenSq1 == 0 || lenSq2 == 0) return 0; // Handle zero-length vectors
    float cosTheta = dotProd / std::sqrt(lenSq1 * lenSq2);
    cosTheta = std::max(-1.0f, std::min(1.0f, cosTheta)); // Clamp to [-1, 1]
    return std::acos(cosTheta);
}

Vector3 Vector3::projectOnto(const Vector3& other) const {
    float lenSq = other.lengthSquared();
    if (lenSq == 0) return Vector3(0, 0, 0); // Handle zero-length projection vector
    float scalar = dot(other) / lenSq;
    return other * scalar;
}

Vector3 Vector3::reflectOver(const Vector3& normal) const {
    Vector3 unitNormal = normal.normalized();
    if (unitNormal.isZero()) return Vector3(0, 0, 0); // Handle invalid normal
    return 2 * projectOnto(unitNormal) - *this;
}

// Comparison operators
bool Vector3::isZero() const {
    const float epsilon = 1e-6f;
    return std::fabs(x) < epsilon && std::fabs(y) < epsilon && std::fabs(z) < epsilon;
}

bool Vector3::operator==(const Vector3& other) const {
    const float epsilon = 1e-6f;
    return std::fabs(x - other.x) < epsilon &&
           std::fabs(y - other.y) < epsilon &&
           std::fabs(z - other.z) < epsilon;
}

bool Vector3::operator!=(const Vector3& other) const {
    return !(*this == other);
}

// Static utility methods
Vector3 Vector3::lerp(const Vector3& v1, const Vector3& v2, float t) {
    return Vector3(v1.x + t * (v2.x - v1.x),
                   v1.y + t * (v2.y - v1.y),
                   v1.z + t * (v2.z - v1.z));
}

// Friend function for output
std::ostream& operator<<(std::ostream& os, const Vector3& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}