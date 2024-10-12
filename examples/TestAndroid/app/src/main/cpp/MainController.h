#pragma once

#include "glfm.h"

class MainController {
public:
    static MainController* getInstance();
    void initialize(GLFMDisplay *display);
    void update(double frameTime);
    void draw();

private:
    MainController() = default;
    static MainController* instance;
};
