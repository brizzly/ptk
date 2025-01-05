#ifndef GAME_H
#define GAME_H

#include "KPTK.h"
#include "KMiscTools.h"
#include "KInput.h"
#include "KGraphic.h"
#include "KFont.h"
#include "KSound.h"
#include "KMusic.h"
#include "ball.h"
#include "Player.h"
#include "Projectile.h"
#include <vector>

#ifdef __ANDROID__
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#endif

enum class GameState {
    RUNNING,
    GAME_OVER
};

class game
{
private:
#ifdef __ANDROID__
    AAssetManager   * assetManager_;
#endif

public:
    game();
    ~game();

#ifdef __ANDROID__
    void                setAssetManager(AAssetManager * value);
#endif

    void init(int width, int height);
    void draw(float frameTime);
    void playSfx();
    void addNewBall(int px, int py);
    void addBall(char * imagename, int px, int py);

private:
    // Ressources graphiques
    KGraphic * testGraphic;
    KGraphic * testGraphic2;
    KGraphic * testGraphic3;
    KGraphic * testGraphic4;
    KGraphic * shapeGraphic;
    KGraphic * buttonLeft;
    KGraphic * buttonMiddle;
    KGraphic * buttonRight;

    KFont * fonte;
    KFont * fonte2;
    KFont * fonte3;

    KSound * sound1;
    KSound * sound2;
    KMusic * music1;

    List ballsList;

    // Variables du jeu (fusion avec Game2)
    Player player;
    std::vector<Projectile> projectiles;

    GameState gameState;
    double gameStartTime;
    double timeSurvived;
    double finalSurvivalTime;
    double gameOverTime;

    float spawnTimer;
    float spawnInterval;
    unsigned long lastTime;

    void Run(double frameTime);
    void Update(float dt);
    void DrawProjectilesAndPlayer();
    void SpawnProjectile();
    static bool IsOffScreen(const Projectile &p);
    void ResetGame();
};

#endif // GAME_H
