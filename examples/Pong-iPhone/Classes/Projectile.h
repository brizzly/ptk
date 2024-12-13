#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Vector.h"
class KGraphic;
class Player;

class Projectile {
public:
    Projectile(float x, float y, float vx, float vy, float s);

    void Update(float dt);
    void Draw(KGraphic* gfx) const;
    bool Collides(const Player &p);

    Vector pos;
    float size;
    float velX;
    float velY;
};

#endif // PROJECTILE_H
