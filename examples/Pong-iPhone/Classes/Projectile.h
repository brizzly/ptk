#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Vector.h"

class KGraphic;
class Player;

class Projectile {
public:
    Vector pos;
    Vector vel;
    float size;

    Projectile(float x, float y, float dx, float dy, float s);

    void Update(float dt);
    void Draw(KGraphic* gfx);
    bool Collides(const Player &p);
};

#endif // PROJECTILE_H
