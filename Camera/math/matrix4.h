#ifndef __MATRIX4_H__
#define __MATRIX4_H__
#include <array>
#include <cmath>
#include <iostream>
#include"Vector3.h"

class Matrix4 {
    public:
    std::array<float,16>m;
    // default constructor, initializes to identity matrix
    constexpr Matrix4();
    constexpr void loadIdentity();


};
#endif // __MATRIX4_H__