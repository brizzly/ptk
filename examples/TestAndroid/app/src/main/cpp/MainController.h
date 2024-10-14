#pragma once

#include "glfm.h"
#include "KGraphic.h"
#include "KMiscTools.h"
#include "KLogFile.h"
#include "KPTK.h"
#include "KSound.h"
#include "KMusic.h"
#include "KInput.h"
#include <GLES2/gl2.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <android/asset_manager_jni.h>

class MainController {

private:

    int width;
    int height;
    double _framtime;
    AAssetManager* assetManager;

public:

    KGraphic * testGraphic;     // box 1
    KGraphic * testGraphic2;    // box 2
    KGraphic * testGraphic3;    // background
    KSound * sound1;
    KSound * sound2;
    KMusic * music1;

public:
    static MainController* getInstance();

    void setAssetManager(AAssetManager* assetManager);
    void initialize(int w, int h);
    void update(double frameTime);
    void draw();
    void touchEvent(int touch, int phase, double x, double y);

private:
    MainController() = default;
    static MainController* instance;
};
