#include "Vector3.h"
#include "Matrix4.h"
#include "Quaternion.h"

int main() {
    Matrix4 matrix1=Matrix4::getIdentity(); // Initialize to identity matrix
    
    // Print the resulting matrix
    std::cout << "---------Resulting Matrix:\n" << &matrix1 << std::endl;
    const Matrix4 &matrix2=Matrix4::getIdentity(); // Initialize to identity matrix
    std::cout << "---------Resulting Matrix:\n" << &matrix2 << std::endl;
    Matrix4 matrix3 ;// Initialize to identity matrix
    std::cout << "---------Resulting Matrix:\n" << &matrix3 << std::endl;
    Matrix4 matrix4=Matrix4::translation(Vector3(1.0,3.0,4)); 
    std::cout << "---------Resulting Matrix:\n" << matrix4 << std::endl;
    Matrix4 matrix5=Matrix4::rotationAxis(Vector3(0,0,1), 45);
    std::cout << "---------Resulting Matrix:\n" << matrix5 << std::endl;
    // quaternion
    Quaternion q(1,0.1,0,0);//Example quaternion
    std::cout << "---------Resulting Quaternion:\n" << q << std::endl;
    Quaternion q2;
    std::cout << "---------Resulting Quaternion:\n" << q2 << std::endl;
    return 0;
}