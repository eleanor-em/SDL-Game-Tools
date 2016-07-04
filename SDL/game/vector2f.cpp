#include "vector2f.h"

Vector2f operator *(float lhs, const Vector2f &rhs) { return Vector2f(rhs) * lhs; }
