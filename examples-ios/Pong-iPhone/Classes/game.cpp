#include "game.h"
#include <algorithm>
#include <cmath>
#include <ctime>
#include <sstream>
#include <iomanip>

game::game()
    : testGraphic(nullptr), testGraphic2(nullptr), testGraphic3(nullptr), testGraphic4(nullptr),
      shapeGraphic(nullptr), buttonLeft(nullptr), buttonMiddle(nullptr), buttonRight(nullptr),
      fonte(nullptr), fonte2(nullptr), fonte3(nullptr),
      sound1(nullptr), sound2(nullptr), music1(nullptr),
      player(160.0f, 240.0f, 10.0f),
      gameState(GameState::RUNNING),
      gameStartTime(0.0), timeSurvived(0.0), finalSurvivalTime(0.0), gameOverTime(0.0),
      spawnTimer(0.0f), spawnInterval(3.0f)
{
    lastTime = KMiscTools::getMilliseconds();
    srand((unsigned)time(nullptr));
    gameStartTime = (double)lastTime / 1000.0;
}

game::~game()
{
    delete testGraphic;
    delete testGraphic2;
    delete testGraphic3;
    delete testGraphic4;
    delete shapeGraphic;
    delete buttonLeft;
    delete buttonMiddle;
    delete buttonRight;

    delete fonte;
    delete fonte2;
    delete fonte3;

    delete sound1;
    delete sound2;
    delete music1;
}

void game::init(int width, int height)
{
    KPTK::_screenW = width;
    KPTK::_screenH = height;
    KPTK::_screenOrientation = K_320x480; // Orientation standard portrait

    int gameW = KPTK::getGameW();
    int gameH = KPTK::getGameH();
    int screenW = width;
    int screenH = height;

    KMiscTools::initMiscTools();

    testGraphic = new KGraphic(gameW, gameH, screenW, screenH);
    testGraphic->loadPicture("box.png");

    testGraphic2 = new KGraphic(gameW, gameH, screenW, screenH);
    testGraphic2->loadPicture("box2.png");

    testGraphic3 = new KGraphic(gameW, gameH, screenW, screenH);
    testGraphic3->loadPicture("960_1440_Retina.png");

    testGraphic4 = new KGraphic(gameW, gameH, screenW, screenH);
    testGraphic4->loadPicture("box512.png");

    shapeGraphic = new KGraphic(gameW, gameH, screenW, screenH);

    buttonLeft = new KGraphic(gameW, gameH, screenW, screenH);
    buttonLeft->loadPicture("border2_left.png");

    buttonMiddle = new KGraphic(gameW, gameH, screenW, screenH);
    buttonMiddle->loadPicture("border2_middle.png");

    buttonRight = new KGraphic(gameW, gameH, screenW, screenH);
    buttonRight->loadPicture("border2_right.png");

    fonte = new KFont(KMiscTools::makeFilePath("neue.ttf"), gameW, gameH);
    fonte->SetTextColor(1, 1, 1);
    fonte->SetBackgroundColor(0, 0, 0, 0.5f);

    fonte2 = new KFont(KMiscTools::makeFilePath("pixel.ttf"), gameW, gameH);
    fonte2->SetBackgroundColor(1, 0, 0, 0.8f);

    fonte3 = new KFont(KMiscTools::makeFilePath("chinese.ttf"), gameW, gameH);
    fonte3->SetTextColor(1, 0, 0);

    sound1 = new KSound;
    sound1->loadSample(KMiscTools::makeFilePath("boing2.caf"));
    sound1->setVolume(1.0f);

    sound2 = new KSound;
    sound2->loadSample(KMiscTools::makeFilePath("jump1.caf"));
    sound2->setVolume(1.0f);

    music1 = new KMusic();
    music1->load(KMiscTools::makeFilePath("menu.mp3"));
    music1->setVolume(0.7f);
    music1->setLooping(true);
    music1->playMusic(); // musique de fond en boucle

    for(int i=0; i<1; i++) {
        int px = rand() % (KPTK::_screenW - 256);
        int py = rand() % (KPTK::_screenH - 256);
        addNewBall(px, py);
    }
}

void game::ResetGame() {
    projectiles.clear();
    player.SetPosition(160.0f, 240.0f);
    gameState = GameState::RUNNING;
    timeSurvived = 0.0;
    finalSurvivalTime = 0.0;
    spawnTimer = 0.0f;
    spawnInterval = 3.0f;
    lastTime = KMiscTools::getMilliseconds();
    gameStartTime = (double)lastTime / 1000.0;
}

void game::addNewBall(int px, int py)
{
    if(rand() % 2 == 0) {
        addBall("box.png", px, py);
    } else {
        addBall("box2.png", px, py);
    }
}

void game::addBall(char * imagename, int px, int py)
{
    float vx = 0;
    float vy = 0;

    int game_w = KPTK::getGameW();
    int game_h = KPTK::getGameH();
    int screen_w = KPTK::_screenW;
    int screen_h = KPTK::_screenH;

    KGraphic * kg = new KGraphic(game_w, game_h, screen_w, screen_h);
    kg->loadPicture(imagename);

    float tw = kg->getTextureWidth();
    float th = kg->getTextureHeight();
    float tsw = kg->getTextureSizeW();
    float tsh = kg->getTextureSizeH();
    printf("tw=%f, th=%f, tsw=%f, tsh=%f\n", tw, th, tsw, tsh);

    ball * b = new ball(kg, px, py, vx, vy, tsw, tsh, game_w, game_h, screen_w, screen_h);
    ballsList.Append(b);
}

void game::draw(float frameTime)
{
    // Mise à jour des balls
    for(int i=0; i<ballsList.count(); i++) {
        ball * b = (ball*) ballsList.get(i);
        b->update(frameTime);
    }

    // Dessin des balls
    for(int i=0; i<ballsList.count(); i++) {
        ball * b = (ball*) ballsList.get(i);
        b->kg->setDrawBounds(true);
        b->kg->draw(b->getPos().x, b->getPos().y, 0.0f, 1, 1);
    }

    Run(frameTime);
}

void game::Run(double frameTime) {
    unsigned long currentTime = KMiscTools::getMilliseconds();
    float dt = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;

    Update(dt);
    DrawProjectilesAndPlayer();
}

void game::Update(float dt)
{
    if (gameState == GameState::GAME_OVER) {
        // Vérifie si plus de 5 secondes se sont écoulées depuis la fin
        double currentTimeSec = (double)KMiscTools::getMilliseconds() / 1000.0;
        if (currentTimeSec - gameOverTime >= 5.0) {
            // Si l’utilisateur touche l’écran après 5s, on réinitialise le jeu
            if (KInput::isScreenPressed()) {
                ResetGame();
            }
        }
        return;
    }

    player.Update(dt);

    spawnTimer += dt;
    if (spawnTimer >= spawnInterval && gameState == GameState::RUNNING) {
        spawnTimer = 0.0f;
        SpawnProjectile();
        if (sound1) {
            sound1->playSample();
        }
    }

    for (auto &proj : projectiles) {
        proj.Update(dt);
        if (proj.Collides(player)) {
            gameState = GameState::GAME_OVER;
            finalSurvivalTime = timeSurvived;
            gameOverTime = (double)KMiscTools::getMilliseconds() / 1000.0;
            if (sound2) {
                sound2->playSample();
            }
        }
    }

    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(), IsOffScreen), projectiles.end());

    timeSurvived += dt;
}

void game::DrawProjectilesAndPlayer() {
    player.Draw(shapeGraphic);
    for (auto &proj : projectiles) {
        proj.Draw(shapeGraphic);
    }

    if (gameState == GameState::GAME_OVER) {
        std::wstringstream woss;
        woss << L"Temps de survie : " << std::fixed << std::setprecision(1) << finalSurvivalTime << L" s";
        std::wstring message = woss.str();

        float gameW = (float)KPTK::getGameW();
        float gameH = (float)KPTK::getGameH();
        fonte->RenderTextCentered(message.c_str(), gameH / 2.0f, 0.0f, 20.0f);

        double currentTimeSec = (double)KMiscTools::getMilliseconds() / 1000.0;
        if (currentTimeSec - gameOverTime >= 5.0) {
            std::wstring restartMsg = L"Appuyez sur l'écran pour recommencer";
            fonte->RenderTextCentered(restartMsg.c_str(), (gameH / 2.0f) + 40.0f, 0.0f, 20.0f);
        }
    }
}

void game::SpawnProjectile() {
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
    projectiles.emplace_back(x, y, dx * speed, dy * speed, 16.0f);
}

bool game::IsOffScreen(const Projectile &p) {
    return (p.pos.x < 0 || p.pos.x > KPTK::getGameW() || p.pos.y < 0 || p.pos.y > KPTK::getGameH());
}

void game::playSfx() {
    if(rand() % 100 < 50) {
        sound1->playSample();
    } else {
        sound2->playSample();
    }
}
