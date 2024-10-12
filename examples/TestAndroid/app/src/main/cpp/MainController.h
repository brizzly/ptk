#pragma once

#include "glfm.h"
#include "KGraphic.h"
#include "KMiscTools.h"
#include "KPTK.h"
//#include "KSound.h"
//#include "KMusic.h"

class MainController {

public:

    KGraphic * testGraphic;     // box 1
    KGraphic * testGraphic2;    // box 2
    KGraphic * testGraphic3;    // background
    //KSound * sound1;
    //KMusic * music1;

public:
    static MainController* getInstance();
    void initialize(GLFMDisplay *display);
    void update(double frameTime);
    void draw();

private:
    MainController() = default;
    static MainController* instance;
};
