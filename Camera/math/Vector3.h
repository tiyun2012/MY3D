#ifndef VECTOR3_H
#define VECTOR3_H

#include <iostream>
#include <cmath>
#include <limits>

// Define M_PI if not defined (used in angle calculations)
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Vector3 {
public:
    // Public data members for direct access
    float x, y, z;

    // Static constant vectors
    static const Vector3 zero;    // (0, 0, 0)
    static const Vector3 unitX;   // (1, 0, 0)
    static const Vector3 unitY;   // (0, 1, 0)
    static const Vector3 unitZ;   // (0, 0, 1)

    // Constructors
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    // Arithmetic operators
    Vector3 operator+(const Vector3& other) const; // Vector addition
    Vector3 operator-(const Vector3& other) const; // Vector subtraction
    Vector3 operator*(float scalar) const;         // Scalar multiplication
    Vector3 operator/(float scalar) const;         // Scalar division
    Vector3 operator-() const;                     // Negation

    // Compound assignment operators (in-place modification)
    Vector3& operator+=(const Vector3& other);
    Vector3& operator-=(const Vector3& other);
    Vector3& operator*=(float scalar);
    Vector3& operator/=(float scalar);

    // Vector operations
    /** Returns the length (magnitude) of the vector */
    inline float length() const { return std::sqrt(x * x + y * y + z * z); }

    /** Returns the squared length of the vector (faster than length()) */
    inline float lengthSquared() const { return x * x + y * y + z * z; }

    /** Returns a normalized copy of the vector (unit vector), or zero if length is zero */
    Vector3 normalized() const;

    /** Returns the cross product of this vector and another */
    Vector3 cross(const Vector3& other) const;

    /** Returns the dot product of this vector and another */
    inline float dot(const Vector3& other) const { return x * other.x + y * other.y + z * other.z; }

    /** Returns the angle (in radians) between this vector and another */
    float angleBetween(const Vector3& other) const;

    /** Projects this vector onto another vector */
    Vector3 projectOnto(const Vector3& other) const;

    /** Reflects this vector over a normal vector */
    Vector3 reflectOver(const Vector3& normal) const;

    // Comparison operators
    /** Checks if the vector is approximately zero (using epsilon) */
    bool isZero() const;

    /** Checks if two vectors are approximately equal (using epsilon) */
    bool operator==(const Vector3& other) const;

    /** Checks if two vectors are not approximately equal */
    bool operator!=(const Vector3& other) const;

    // Static utility methods
    /** Linearly interpolates between two vectors based on t (0 to 1) */
    static Vector3 lerp(const Vector3& v1, const Vector3& v2, float t);

    // Friend function for output streaming
    friend std::ostream& operator<<(std::ostream& os, const Vector3& v);
};

// Non-member operator for scalar multiplication from the left (e.g., 2 * vector)
inline Vector3 operator*(float scalar, const Vector3& v) {
    return Vector3(v.x * scalar, v.y * scalar, v.z * scalar);
}

#endif // VECTOR3_H