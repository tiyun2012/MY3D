#include <iostream>
#include "Vector3.h"

int main() {
    // Create some Vector3 objects
    Vector3 v1(1.0f, 2.0f, 3.0f);
    Vector3 v2(4.0f, 5.0f, 6.0f);

    // Test addition
    Vector3 v_add = v1 + v2;
    std::cout << "v1 + v2 = (" << v_add.x << ", " << v_add.y << ", " << v_add.z << ")\n";

    // Test subtraction
    Vector3 v_sub = v1 - v2;
    std::cout << "v1 - v2 = (" << v_sub.x << ", " << v_sub.y << ", " << v_sub.z << ")\n";

    // Test scalar multiplication
    Vector3 v_mul = v1 * 2.0f;
    std::cout << "v1 * 2.0 = (" << v_mul.x << ", " << v_mul.y << ", " << v_mul.z << ")\n";

    // Test scalar division
    Vector3 v_div = v1 / 2.0f;
    std::cout << "v1 / 2.0 = (" << v_div.x << ", " << v_div.y << ", " << v_div.z << ")\n";

    // Test unary minus
    Vector3 v_neg = -v1;
    std::cout << "-v1 = (" << v_neg.x << ", " << v_neg.y << ", " << v_neg.z << ")\n";

    // Test length
    float len = v1.length();
    std::cout << "Length of v1 = " << len << "\n";

    // Test normalization
    Vector3 v_norm = v1.normalized();
    std::cout << "Normalized v1 = (" << v_norm.x << ", " << v_norm.y << ", " << v_norm.z << ")\n";

    // Test cross product
    Vector3 v_cross = v1.cross(v2);
    std::cout << "v1 cross v2 = (" << v_cross.x << ", " << v_cross.y << ", " << v_cross.z << ")\n";

    // Test dot product
    float v_dot = v1.dot(v2);
    std::cout << "v1 dot v2 = " << v_dot << "\n";

    return 0;
}
