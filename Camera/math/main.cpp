#include <iostream>
#include "Vector3.h"
#include "matrix4.h"
// #include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
int main() {
    // Test using scalar multiplication with the scalar on the left
    Vector3 v1 =Vector3(1.0f, 1.0f, 0.0f);
    Vector3 v2 =Vector3(-1.0f, 0.0f, 0.0f);
    Vector3 v3 =v1.projectOnto(v2);
    std::cout << "v1.projectOnto v2 "<<v3.x<< ","<<v3.y<<","<<v3.z<< std::endl;
    Matrix4 mat=Matrix4();
    std::cout << "Matrix4 Identity Matrix: " << std::endl << mat << std::endl;

    return 0;
}
