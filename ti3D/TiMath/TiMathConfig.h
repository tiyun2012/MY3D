#ifndef TIMATH_CONFIG_H
#define TIMATH_CONFIG_H

namespace TiMath {
constexpr float EPSILON = 1e-6f;
constexpr float PI = 3.14159265358979323846f;
#define TIMATH_COLUMN_MAJOR
// #define TIMATH_ROW_MAJOR
#define USE_SSE
} // namespace TiMath

#endif // TIMATH_CONFIG_H