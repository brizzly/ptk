
// https://github.com/brackeen/glfm

#import <UIKit/UIKit.h>
#import "TestPortrait-Bridging-Header.h"

#import "MainController.h"
#import "KShader.h"
#import "KGraphic.h"
#import "KFont.h"
#import "KSound.h"
#import "KMusic.h"
#import "KInput.h"
#import "KPTK.h"
#import "KMiscTools.h"
#import "glfm.h"

@class GLFMViewController;

@implementation MainController


KGraphic * testGraphic;     // box 1
KGraphic * testGraphic2;    // box 2
KGraphic * testGraphic3;    // background
KGraphic * shapeGraphic;
KFont * fonte;
KSound * sound1;
KSound * sound2;
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
    KPTK::_screenOrientation = K_960x1440;
        
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
    testGraphic3->loadPicture("960_1440.png");
    
    shapeGraphic = new KGraphic(gameW, gameH, screenW, screenH);
    
	
	const char * fontname = "neue.ttf";
	fonte = new KFont(KMiscTools::makeFilePath(fontname), 48, gameW, gameH);
	
	
    // Ensure OpenALManager is initialized
    OpenALManager& alManager = OpenALManager::getInstance();

    
    sound1 = new KSound;
    sound1->loadSample(KMiscTools::makeFilePath("boing2.caf"));
    sound1->setVolume(1.0);
    
    sound2 = new KSound;
    sound2->loadSample(KMiscTools::makeFilePath("jump1.caf"));
    sound2->setVolume(1.0);
    
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
	

    r += (frameTime / 1000.0f) * 0.01f;
    a += (frameTime / 1000.0f) * 0.01f;
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

    
    // LINES
    
    float line_R = 0.0f;
    float line_G = 222.0f/255.0f;
    float line_B = 1;
    float line_A = 0.50f;
    
    float pX = 20;
    float pY = 20;
    float w1 = screenWidth/2;
    float h1 = screenHeight/2;
    shapeGraphic->drawLine(pX, pY, pX+w1, pY, line_R, line_G, line_B, line_A, 1.0f);
    shapeGraphic->drawLine(pX+w1, pY, pX+w1, pY+h1, line_R, line_G, line_B, line_A, 1.0f);
    shapeGraphic->drawLine(pX+w1, pY+h1, pX, pY+h1, line_R, line_G, line_B, line_A, 1.0f);
    shapeGraphic->drawLine(pX, pY+h1, pX, pY, line_R, line_G, line_B, line_A, 1.0f);

    
	// TEXT
	
	fonte->RenderText(L"HELLO WORLD HOW ARE YOU!!!!!!", 10, 20, 1.0f);


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
    glfmSetSupportedInterfaceOrientation(display, GLFMInterfaceOrientationPortrait);

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
        case GLFMTouchPhaseBegan:
            printf("Touch %d started at (%.2f, %.2f)\n", touch, x, y);
            KInput::setScreenPressed(0, x, y);
            KInput::setFingerPosition(touch, x, y, true);
            if(rand() % 100 < 50) {
                sound1->playSample();
            }
            else {
                sound2->playSample();
            }
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
