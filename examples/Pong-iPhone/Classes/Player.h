#ifndef PLAYER_H
#define PLAYER_H

#include "Vector.h"
class KGraphic;

class Player {
public:
    Player(float x, float y, float s);

    void Update(float dt);
    void Draw(KGraphic* gfx);

    void SetPosition(float x, float y);
    float GetSize() const;
    const Vector& GetPosition() const;

public:
    Vector pos;
    float size;
};

#endif // PLAYER_H
