#include "Game2.h"
#include "KPTK.h"
#include "KMiscTools.h"
#include "KInput.h"
#include <algorithm>
#include <cmath>
#include <ctime>

Game2::Game2()
    : player(160.0f, 240.0f, 10.0f),
      running(true),
      spawnTimer(0.0f),
      spawnInterval(3.0f),
      timeSurvived(0.0f)
{
    KMiscTools::initMiscTools();
    KPTK::setScreenOrientation(K_320x480);
    window = KPTK::createKWindow(K_320x480);
    KPTK::setScreenSize(window->getWindowWidth(), window->getWindowHeight());
    gfx = KPTK::createKGraphic();
    lastTime = KMiscTools::getMilliseconds();
    srand((unsigned)time(nullptr));
}

void Game2::Run(double frameTime) {

    static unsigned long startTime = 0;
    unsigned long currentTime = KMiscTools::getMilliseconds();
    
    if(startTime == 0) {
        startTime = KMiscTools::getMilliseconds();
    }
    
    //float dt = (currentTime - lastTime) / 1000.0f;
    //lastTime = currentTime;

    double ft = (currentTime - startTime)/ 1000.0f;
    
    startTime = KMiscTools::getMilliseconds();
    
    
    Update(ft);
    Draw();
    //window->flipBackBuffer();
}

void Game2::Update(float dt)
{
    //KInput::setFrameEnded();
    
    if(running == false) {
        
        
        return;
    }
    
    
    player.Update(dt);
    
    //printf("dt: %f, spawnTimer: %f, spawnInterval: %f\n", dt, spawnTimer, spawnInterval);

    spawnTimer += dt;
    if (spawnTimer >= spawnInterval) {
        spawnTimer = 0.0f;
        printf("Spawn projectil\n");
        SpawnProjectile();
        //spawnInterval = std::max(0.1f, spawnInterval * 0.98f);
    }

    for (auto &proj : projectiles) {
        proj.Update(dt);
        if (proj.Collides(player)) {
            running = false;
        }
    }

    // Nettoyage des projectiles hors écran sans lambda
    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(), IsOffScreen), projectiles.end());


    timeSurvived += dt;
    
    
    printf("timeSurvived: %f\n", timeSurvived);
}

void Game2::Draw() {
    //window->setWorldView(0,0,0,1,true);
    player.Draw(gfx);
    for (auto &proj : projectiles) {
        proj.Draw(gfx);
    }
}

void Game2::SpawnProjectile() {
    float gw = (float)KPTK::getGameW();
    float gh = (float)KPTK::getGameH();
    float x, y;
    int side = rand() % 4;

    switch (side) {
        case 0: // haut
            x = (float)(rand() % (int)gw);
            y = 0.0f;
            break;
        case 1: // bas
            x = (float)(rand() % (int)gw);
            y = gh;
            break;
        case 2: // gauche
            x = 0.0f;
            y = (float)(rand() % (int)gh);
            break;
        default: // droite
            x = gw;
            y = (float)(rand() % (int)gh);
            break;
    }

    float dx = (gw / 2.0f - x);
    float dy = (gh / 2.0f - y);
    float len = std::sqrt(dx*dx + dy*dy);
    dx /= len; dy /= len;

    float speed = 50.0f + (float)timeSurvived * 2.0f;
    projectiles.emplace_back(x, y, dx * speed, dy * speed, 8.0f);
}

bool Game2::IsOffScreen(const Projectile &p) {
    return (p.pos.x < 0 || p.pos.x > KPTK::getGameW() || p.pos.y < 0 || p.pos.y > KPTK::getGameH());
}
