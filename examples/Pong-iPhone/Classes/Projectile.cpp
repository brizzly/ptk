#include "Projectile.h"
#include "Player.h"
#include "KPTK.h"
#include <cmath>

Projectile::Projectile(float x, float y, float dx, float dy, float s)
    : pos(x, y, 1.0f),   // On fixe s à 1.0 pour la position
      vel(dx, dy, 1.0f), // On fixe s à 1.0 pour la vitesse
      size(s)
{}

void Projectile::Update(float dt) {
    // On peut utiliser directement les champs x,y ou l'opérateur +=
    // pos.x += vel.x * dt;
    // pos.y += vel.y * dt;

    // Ou de manière plus élégante :
    pos += vel * dt;
}

void Projectile::Draw(KGraphic* gfx) {
    float half = size / 2.0f;
    vec2 vertice[4] = {
        {-half, -half},
        { half, -half},
        { half,  half},
        {-half,  half}
    };
    gfx->drawShape(4, vertice, (int)pos.x, (int)pos.y, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
}

bool Projectile::Collides(const Player &p) {
    float halfS = p.size / 2.0f;
    float halfMyS = size / 2.0f;
    return !(pos.x + halfMyS < p.pos.x - halfS ||
             pos.x - halfMyS > p.pos.x + halfS ||
             pos.y + halfMyS < p.pos.y - halfS ||
             pos.y - halfMyS > p.pos.y + halfS);
}
