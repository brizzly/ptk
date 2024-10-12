// MainController.cpp (Fixed)
// Implemented minor changes for code clarity, corrected improper singleton pattern usage.

#include "MainController.h"
#include <GLES2/gl2.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

// Singleton instance initialization
MainController* MainController::instance = nullptr;

MainController* MainController::getInstance() {
    if (instance == nullptr) {
        instance = new MainController();
    }
    return instance;
}

void MainController::initialize(GLFMDisplay *display) {
    // Set up OpenGL or GLFM state
    glClearColor(1.0f, 0.5f, 0.0f, 1.0f);

    int width = 1080;
    int height = 2192;

    int gameW = 1080;
    int gameH = 2192;

    int screenW = 1080;
    int screenH = 2192;

    // Set the viewport to match the screen size
    glViewport(0, 0, width, height);
/*
    KPTK::_screenW = width;
    KPTK::_screenH = height;
    KPTK::_screenOrientation = K_960x1440;

    int gameW = KPTK::getGameW();
    int gameH = KPTK::getGameH();
    int screenW = width;
    int screenH = height;
*/
    //KMiscTools::initMiscTools();


    testGraphic = new KGraphic(gameW, gameH, screenW, screenH);
    testGraphic->loadPicture("box.png");

    testGraphic2 = new KGraphic(gameW, gameH, screenW, screenH);
    testGraphic2->loadPicture("box2.png");

    testGraphic3 = new KGraphic(gameW, gameH, screenW, screenH);
    testGraphic3->loadPicture("960_1440.png");

    /*
    sound1 = new KSound;
    sound1->loadSample(KMiscTools::makeFilePath("boing2.caf"));
    sound1->setVolume(50);

    music1 = new KMusic();
    music1->playMusic(KMiscTools::makeFilePath("menu.mp3"));
    */
}

void MainController::update(double frameTime) {
    // Update state, handle user input, etc.
}

void MainController::draw() {

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float frameTime = 16.666;

    static float r = 0.0;
    static float z = 0.0;
    static float a = 0.0;

    r += (frameTime / 1000.0f) * 0.0005f;
    a += (frameTime / 1000.0f) * 0.0005f;
    z = 1.0 + cos(a * M_PI / 180.0f); // (frameTime / 1000.0f)

    static float debugval = 0.0;
    debugval += 0.4f;


    // FULL SCREEN IMAGE : 960x1440

    testGraphic3->srcX = 0;
    testGraphic3->srcY = 0;
    testGraphic3->destX = 0;
    testGraphic3->destY = 0;
    testGraphic3->sizeW = 960;
    testGraphic3->sizeH = 1440;
    testGraphic3->angle = 0;
    testGraphic3->zoom = 1.0;
    testGraphic3->blend = 1.0;
    testGraphic3->render();


    // SQUARE 256x256

    testGraphic->srcX = 0;
    testGraphic->srcY = 0;
    testGraphic->destX = 140;
    testGraphic->destY = 80;
    testGraphic->sizeW = 256;
    testGraphic->sizeH = 256;
    testGraphic->angle = a;
    testGraphic->zoom = z;
    testGraphic->blend = 1.0;
    testGraphic->render();



    // SQUARE 256x256

    testGraphic2->srcX = 0;
    testGraphic2->srcY = 0;
    testGraphic2->destX = 420;
    testGraphic2->destY = 850;
    testGraphic2->sizeW = 256;
    testGraphic2->sizeH = 256;
    testGraphic2->angle = 360-a;
    testGraphic2->zoom = 1.0;
    testGraphic2->blend = 1.0;
    testGraphic2->render();



}
