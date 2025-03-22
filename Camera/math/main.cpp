#include <iostream>
#include "Vector3.h"

int main() {
    // Test using scalar multiplication with the scalar on the left
    Vector3 v1 =Vector3(1.3f, 3.0f, 5.0f);
    Vector3 v2=-v1;
    std::cout << "3.0 * Vector3(1.3, 3, 5) = ("
              << v2.x << ", " << v2.y << ", " << v2.z << ")\n";
    
    // Other tests...
    // ddddddddddd
    return 0;
}
