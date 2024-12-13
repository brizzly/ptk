#ifndef GAME2_H
#define GAME2_H

#include <vector>
#include "Player.h"
#include "Projectile.h"

class KWindow;
class KGraphic;

class Game2 {
public:
    KWindow* window;
    KGraphic* gfx;
    Player player;
    std::vector<Projectile> projectiles;

    bool running;
    float spawnTimer;
    float spawnInterval;
    float timeSurvived;
    unsigned long lastTime;

    Game2();
    void Run(double frameTime);

private:
    void Update(float dt);
    void Draw();
    void SpawnProjectile();

    // Fonction utilitaire pour supprimer les projectiles hors écran
    static bool IsOffScreen(const Projectile &p);
};

#endif // GAME2_H
