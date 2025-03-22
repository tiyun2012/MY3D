//#pragma once
#ifndef VECTOR3_H
#define VECTOR3_H
#include <iostream>
#include <cmath>

// Define M_PI if not defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
class Vector3 {
public:
	float x, y, z;
	// constructor
	//making a vector object default
	Vector3();
	//making vector object from given value
	Vector3(float xi, float yi, float zi);
	// Arithmetic operators
	Vector3 operator+(const Vector3& v) const;

	Vector3 operator-(const Vector3& v) const;

	Vector3 operator*(float s) const;

	float operator*(const Vector3& v) const;// dot product

	Vector3 operator/(float s) const;

	Vector3 operator-() const;
	// compound assigment operators
	Vector3& operator+=(const Vector3&v);
	float length() const;

	Vector3 normalized() const;

	Vector3 cross(const Vector3& v) const;

	float dot(const Vector3& v) const;

};

// Non-member operator overload to allow scalar multiplication from the left
inline Vector3 operator*(float s, const Vector3& v) {
    return Vector3(v.x * s, v.y * s, v.z * s);
}


#endif//VECTOR3_H