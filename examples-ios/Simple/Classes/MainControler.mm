#import <UIKit/UIKit.h>
#import "SimpleTest-Bridging-Header.h"

#import "MainController.h"
#import "KShader.h"
#import "KGraphic.h"
#import "KSound.h"
#import "KMusic.h"
#import "KInput.h"
#import "KPTK.h"
#import "KMiscTools.h"
#import "glfm.h"

@class GLFMViewController;

@implementation MainController



KGraphic * testGraphic;     // boite 1
KGraphic * testGraphic2;    // boite 2
KGraphic * testGraphic3;    //
KGraphic * testGraphic4;    //
KGraphic * testGraphic5;    //
KGraphic * testGraphic6;    //
KGraphic * testGraphic7;    //

KSound * sound1;

KMusic * music1;


- (id) init {
    if (self = [super init]) {
        // Initialization code can go here if needed
    }
    return self;
}


// Function called when the surface is created or resized
void onSurfaceCreated(GLFMDisplay *display, int width, int height)
{
    const GLubyte* version = glGetString(GL_VERSION);
    printf("OpenGL Version: %s\n", version);
    
    // Set the viewport to match the screen size
    glViewport(0, 0, width, height);
    
    KPTK::_screenW = width;
    KPTK::_screenH = height;
    KPTK::_screenOrientation = K_640x480;
        
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
    testGraphic3->loadPicture("1704_960.png");
    
    testGraphic4 = new KGraphic(gameW, gameH, screenW, screenH);
    testGraphic4->loadPicture("852_480.png");
    
    testGraphic5 = new KGraphic(gameW, gameH, screenW, screenH);
    testGraphic5->loadPicture("world1.png");
    
    testGraphic6 = new KGraphic(gameW, gameH, screenW, screenH);
    testGraphic6->loadPicture("box.png");
    
    testGraphic7 = new KGraphic(gameW, gameH, screenW, screenH);
    testGraphic7->loadPicture("640_480.png");
    
    
    sound1 = new KSound;
    sound1->loadSample(KMiscTools::makeFilePath("boing2.caf"));
    sound1->setVolume(50);
    
    music1 = new KMusic();
    music1->load(KMiscTools::makeFilePath("menu.mp3"));
    music1->playMusic();
}

// Rendering function
void onFrame(GLFMDisplay *display, double frameTime)
{
    int screenWidth, screenHeight;
    glfmGetDisplaySize(display, &screenWidth, &screenHeight);
    
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    static float r = 0.0;
    static float z = 0.0;
    static float a = 0.0;
    
    r += (frameTime / 1000.0f) * 0.0005f;
    a += (frameTime / 1000.0f) * 0.0005f;
    z = 1.0 + cos(a * M_PI / 180.0f); // (frameTime / 1000.0f)

    static float debugval = 0.0;
    debugval += 0.4f;

    
    // FULL SCREEN IMAGE : 640x480

    testGraphic7->drawEx(0, 0, 640, 480, 0, 0, 0, 1.0, 1.0);
    
    
    // SQUARE 256x256
    
    testGraphic6->drawEx(0, 0, 256, 256, 128, 128, a, z, 1.0);
    
    
    // SQUARE 256x256
    
    testGraphic->drawEx(0, 0, 256, 256, 10, 10, 0, 1.0, 1.0);
    

     // SQUARE 256x256
     
    testGraphic2->drawEx(0, 0, 256, 256, 200, 100, 360-a, 1.0, 1.0);
}

// Function to handle surface destruction (cleanup)
void onSurfaceDestroyed(GLFMDisplay *display)
{
    //glDeleteProgram(program);
    //glDeleteBuffers(1, &vertexBuffer);
    //glDeleteVertexArraysOES(1, &vertexArray);
}

void onSurfaceResized(GLFMDisplay *display, int width, int height) {
    glViewport(0, 0, width, height);
}

void onAppStart(GLFMDisplay *display)
{
    // Enable multi-touch input
    glfmSetMultitouchEnabled(display, true);
}

// Main function for GLFM setup
void glfmMain(GLFMDisplay *display)
{
    glfmSetDisplayConfig(display, GLFMRenderingAPIOpenGLES2, GLFMColorFormatRGBA8888, GLFMDepthFormat16, GLFMStencilFormatNone, GLFMMultisampleNone);
    
    // Lock orientation if necessary, e.g., Portrait
    glfmSetSupportedInterfaceOrientation(display, GLFMInterfaceOrientationLandscapeLeft); // Change to GLFMInterfaceOrientationLandscape if needed

    glfmSetSurfaceCreatedFunc(display, onSurfaceCreated);
    glfmSetSurfaceDestroyedFunc(display, onSurfaceDestroyed);
    //glfmSetRenderFunc(display, onDraw);
    glfmSetTouchFunc(display, onTouch);
    glfmSetSurfaceResizedFunc(display, onSurfaceResized);
    glfmSetMainLoopFunc(display, onFrame);
    
    onAppStart(display);
}

bool onTouch(GLFMDisplay *display, int touch, GLFMTouchPhase phase, double x, double y)
{
    float offsetX = (KPTK::_screenW - KPTK::getGameW()*2) / 2.0f;
    x -= offsetX;
    x *= 0.5f;
    y *= 0.5f;
    
    switch (phase) {
        case GLFMTouchPhaseBegan:
            printf("Touch %d started at (%.2f, %.2f)\n", touch, x, y);
            KInput::setScreenPressed(0, x, y);
            KInput::setFingerPosition(touch, x, y, true);
            sound1->playSample();
            break;
        case GLFMTouchPhaseMoved:
            printf("Touch %d moved to (%.2f, %.2f)\n", touch, x, y);
            KInput::setScreenMoving(x, y);
            break;
        case GLFMTouchPhaseEnded:
            printf("Touch %d ended at (%.2f, %.2f)\n", touch, x, y);
            KInput::setScreenReleased();
            KInput::setFingerPosition(touch, x, y, false);
            break;
        case GLFMTouchPhaseCancelled:
            printf("Touch %d cancelled at (%.2f, %.2f)\n", touch, x, y);
            break;
        default:
            break;
    }
    return true;
}


@end
