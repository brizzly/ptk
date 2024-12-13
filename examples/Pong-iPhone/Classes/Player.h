#ifndef PLAYER_H
#define PLAYER_H

#include "Vector.h"

class KGraphic;

class Player {
public:
    Vector pos; // Utilisation de Vector au lieu de Vec2
    float size;

    Player(float x, float y, float s);

    void Update(float dt);
    void Draw(KGraphic* gfx);
};

#endif // PLAYER_H
