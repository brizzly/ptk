#include "Projectile.h"
#include "Player.h"
#include "KGraphic.h"
#include <cmath>

Projectile::Projectile(float x, float y, float vx, float vy, float s)
    : pos(x, y, 4.0f), size(s), velX(vx), velY(vy)
{}

void Projectile::Update(float dt) {
    pos.x += velX * dt;
    pos.y += velY * dt;
}

void Projectile::Draw(KGraphic* gfx) const {
    if (!gfx) return;
    float half = size / 2.0f;
    vec2 vertice[4] = {
        {-half, -half},
        { half, -half},
        { half,  half},
        {-half,  half}
    };
    gfx->drawShape(4, vertice, (int)pos.x, (int)pos.y, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
}

bool Projectile::Collides(const Player &p) {
    float halfS = p.GetSize() / 2.0f;
    float halfMyS = size / 2.0f;
    const Vector& pPos = p.GetPosition();

    return !(pos.x + halfMyS < pPos.x - halfS ||
             pos.x - halfMyS > pPos.x + halfS ||
             pos.y + halfMyS < pPos.y - halfS ||
             pos.y - halfMyS > pPos.y + halfS);
}
