
// https://github.com/brackeen/glfm

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
    music1->playMusic(KMiscTools::makeFilePath("menu.mp3"));
    
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
    
    testGraphic7->srcX = 0;
    testGraphic7->srcY = 0;
    testGraphic7->destX = 0;
    testGraphic7->destY = 0;
    testGraphic7->sizeW = 640;
    testGraphic7->sizeH = 480;
    testGraphic7->angle = 0;
    testGraphic7->zoom = 1.0;
    testGraphic7->blend = 1.0;
    testGraphic7->render();
    
    // SQUARE 256x256
    
    testGraphic6->srcX = 0;
    testGraphic6->srcY = 0;
    testGraphic6->destX = 128;
    testGraphic6->destY = 128;
    testGraphic6->sizeW = 256;
    testGraphic6->sizeH = 256;
    testGraphic6->angle = a;
    testGraphic6->zoom = z;
    testGraphic6->blend = 1.0;
    testGraphic6->render();
    
    /*
     // IMAGE DE 1024x1024 QUI CONTIENT UN BACKGROUND DE 640x480
     
     testGraphic5->srcX = 0;
     testGraphic5->srcY = 0;
     testGraphic5->destX = 0;
     testGraphic5->destY = 0;
     testGraphic5->sizeW = 640;
     testGraphic5->sizeH = 480;
     testGraphic5->angle = 0;
     testGraphic5->zoom = 1.0;
     testGraphic5->blend = 1.0;
     testGraphic5->render();
     
    
    
     // FULL SCREEN IMAGE : 1704x960
     
     testGraphic3->srcX = 0;
     testGraphic3->srcY = 0;//debugval;
     testGraphic3->destX = 0;//+ debugval;  //34;//debugval;
     testGraphic3->destY = 0;//+ debugval; // 0.0; //28;//debugval;
     testGraphic3->sizeW = 1704; //640;// - debugval;//640;
     testGraphic3->sizeH = 960; //480;//480;
     testGraphic3->angle = 0;//a;    // Rotation angle in degrees
     testGraphic3->zoom = 1.0;//z; //0.5f; //  z; //1.0;// + debugval;      // Scale (1.0 = no scaling)
     testGraphic3->blend = 1.0;     // Alpha blending value (0.0 - 1.0)
     testGraphic3->render();
     
     
     
     
     // HALF FULL SCREEN IMAGE : 852x480
     
     testGraphic4->srcX = 0;
     testGraphic4->srcY = 0;//debugval;
     testGraphic4->destX = 0;//+ debugval;  //34;//debugval;
     testGraphic4->destY = 0;//+ debugval; // 0.0; //28;//debugval;
     testGraphic4->sizeW = 852; //640;// - debugval;//640;
     testGraphic4->sizeH = 480; //480;//480;
     testGraphic4->angle = 0;//a;    // Rotation angle in degrees
     testGraphic4->zoom = 2-z; //0.5f; //  z; //1.0;// + debugval;      // Scale (1.0 = no scaling)
     testGraphic4->blend = 1.0;     // Alpha blending value (0.0 - 1.0)
     testGraphic4->render();
     */
     
    
    
    // SQUARE 256x256
    
    testGraphic->srcX = 0;
    testGraphic->srcY = 0;
    testGraphic->destX = 10;
    testGraphic->destY = 10;
    testGraphic->sizeW = 256;
    testGraphic->sizeH = 256;
    testGraphic->angle = 0;//a;
    testGraphic->zoom = 1.0; //z;
    testGraphic->blend = 1.0;
    testGraphic->render();
    
    

     // SQUARE 256x256
     
     testGraphic2->srcX = 0;
     testGraphic2->srcY = 0;
     testGraphic2->destX = 200;
     testGraphic2->destY = 100;
     testGraphic2->sizeW = 256;
     testGraphic2->sizeH = 256;
     testGraphic2->angle = 360-a;
     testGraphic2->zoom = 1.0;
     testGraphic2->blend = 1.0;
     testGraphic2->render();

    
    
    // Swap the buffers to display the rendered content
    //glfmSwapBuffers(display);
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
