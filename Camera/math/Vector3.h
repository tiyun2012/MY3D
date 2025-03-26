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
	// Data members
	float x, y, z;
	// static constants
	 static const Vector3 zero; 
	 static const Vector3 unitX;
	 static const Vector3 unitY;
	 static const Vector3 unitZ;
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
	Vector3& operator-=(const Vector3&v);
	Vector3& operator*=(float s);
	Vector3& operator/=(float s);
	float length() const;
	float lengthSquared() const;
	//
	Vector3 normalized() const;

	Vector3 cross(const Vector3& v) const;

	float dot(const Vector3& v) const;
	float angleBetween(const Vector3& v) const;
	Vector3 projectOnto(const Vector3& v) const;
	Vector3 reflectOver(const Vector3& normal) const;
	// Comparison operators
	bool isZero() const;
	bool operator==(const Vector3& v)const;
	bool operator!=(const Vector3& v)const;

	// Static methods (utility methods)
	static Vector3 lerp(const Vector3&v1,const Vector3&v2,float t);
	friend std::ostream& operator<<(std::ostream& os, const Vector3& v);

};

// Non-member operator overload to allow scalar multiplication from the left
inline Vector3 operator*(float s, const Vector3& v) {
    return Vector3(v.x * s, v.y * s, v.z * s);
}
inline const Vector3 Vector3::zero = Vector3(0, 0, 0);
inline const Vector3 Vector3::unitX = Vector3(1, 0, 0);
inline const Vector3 Vector3::unitY = Vector3(0, 1, 0);
inline const Vector3 Vector3::unitZ = Vector3(0, 0, 1);

#endif//VECTOR3_H