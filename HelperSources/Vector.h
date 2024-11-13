#ifndef VECTOR_INC__
#define VECTOR_INC__

#include <cmath> // Standard math functions

class Vector
{
public:
    // Members
    float x, y, s;

    // Constructors
    Vector(float _x = 0, float _y = 0, float _s = 1);

    // Operators
    Vector operator*(float scalar) const;       // Scalar multiplication
    Vector& operator+=(const Vector& other);   // Addition assignment

    // Utility methods (optional, based on your needs)
    float magnitude() const;
};

#endif // VECTOR_INC__
