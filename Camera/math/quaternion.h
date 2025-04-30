#ifndef QUATERNION_H
#define QUATERNION_H
#include <iostream>
#include "vector3.h" // Assuming Vector3 is defined in vector3.h
class Quaternion 
{
public:
float w, x, y, z; // Public data members for direct access
constexpr Quaternion() : w(1), x(0), y(0), z(0) {} ;// Default constructor initializes to identity quaternion
Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {} ;// Constructor with parameters

// static const Quaternion identity; // Identity quaternion (1, 0, 0, 0)
friend std::ostream& operator<<(std::ostream& os, const Quaternion& q); // Output stream operator for Quaternion
};
#endif // QUATERNION_H