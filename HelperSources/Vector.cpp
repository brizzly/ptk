#include "Vector.h"

// Constructor
Vector::Vector(float _x, float _y, float _s) : x(_x), y(_y), s(_s) {}

// Scalar multiplication
Vector Vector::operator*(float scalar) const
{
    return Vector(x * scalar, y * scalar, s * scalar);
}

// Addition assignment
Vector& Vector::operator+=(const Vector& other)
{
    x += other.x;
    y += other.y;
    s += other.s;
    return *this;
}

// Utility method (optional)
float Vector::magnitude() const
{
    return std::sqrt(x * x + y * y);
}
