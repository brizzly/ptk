
#include "MainController.h"
#include "game.h"

#define LOG_TAG "NativeCode"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "TAG", __VA_ARGS__))
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)


game gameInstance;


// Singleton instance initialization
MainController* MainController::instance = nullptr;

MainController* MainController::getInstance() {
    if (instance == nullptr) {
        instance = new MainController();
        instance->width = 0;
        instance->height = 0;
        instance->_framtime = 0;
    }
    return instance;
}

void MainController::setAssetManager(AAssetManager* assetManager) {
    this->assetManager = assetManager; // Assuming you have a member variable AAssetManager* assetManager;
}

void MainController::initialize(int w, int h)
{
    if (width != 0 && height != 0) {
        return;
    }

    width = w;
    height = h;
    KLogFile::logDebug("Initialize with screen: %d %d", width, height);


    // Set the viewport to match the screen size
    glViewport(0, 0, width, height);


    KPTK::setScreenSize(width, height);
    KPTK::setScreenOrientation(K_640x480);


    int gameW = KPTK::getGameW();
    int gameH = KPTK::getGameH();
    KLogFile::logDebug("Initialize with game: %d %d", gameW, gameH);


    gameInstance.init(width, height, this->assetManager);
}

void MainController::update(double frameTime) {
}

void MainController::draw() {

    // Set up OpenGL or GLFM state
    //glClearColor(1.0f, 0.5f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    static auto lastTime = std::chrono::high_resolution_clock::now();

    // Calculate frame time
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsedTime = currentTime - lastTime;
    this->_framtime = elapsedTime.count();
    lastTime = currentTime;
    //float frameTime = 16.666;

    gameInstance.draw(16.666 * 10000 /*this->_framtime*/);
}

void MainController::touchEvent(int touch, int phase, double x, double y)
{
    float ratio = KPTK::_screenW / KPTK::_screenH;
    if(ratio > 1.0)
    {
        float offsetX = (KPTK::_screenW - KPTK::getGameW()*2) / 2.0f;
        x -= offsetX;
        x *= 0.5f;
        y *= 0.5f;
    }
    else
    {
        x = x * (KPTK::getGameW() / KPTK::_screenW);
        y = y * (KPTK::getGameH() / KPTK::_screenH);
    }

    switch (phase) {
        case 0://AMOTION_EVENT_ACTION_DOWN:
            //printf("Touch %d started at (%.2f, %.2f)\n", touch, x, y);
            LOGI("Touch game (%f, %f)", x, y);

            KInput::setScreenPressed(0, x, y);
            KInput::setFingerPosition(touch, x, y, true);
            gameInstance.playSfx();
            break;
        case 1://GLFMTouchPhaseMoved:
            //printf("Touch %d moved to (%.2f, %.2f)\n", touch, x, y);
            LOGI("Touch moved game (%f, %f)", x, y);

            KInput::setScreenMoving(x, y);
            break;
        case 2://GLFMTouchPhaseEnded:
            //printf("Touch %d ended at (%.2f, %.2f)\n", touch, x, y);
            LOGI("Touch ended game (%f, %f)", x, y);

            KInput::setScreenReleased();
            KInput::setFingerPosition(touch, x, y, false);
            break;
        case 3://GLFMTouchPhaseCancelled:
            //printf("Touch %d cancelled at (%.2f, %.2f)\n", touch, x, y);
            LOGI("Touch cancelled game (%f, %f)", x, y);

            break;
        default:
            break;
    }
}
