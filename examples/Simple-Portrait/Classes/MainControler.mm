
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
KGraphic * testGraphic4;    // box 3
KGraphic * shapeGraphic;
KGraphic * buttonLeft;
KGraphic * buttonMiddle;
KGraphic * buttonRight;
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
    //testGraphic3->loadPicture("960_1440.png");
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
	
	const char * fontname = "neue.ttf";
	fonte = new KFont(KMiscTools::makeFilePath(fontname), 48, gameW, gameH);
	
	
    // Ensure OpenALManager is initialized
    OpenALManager& alManager = OpenALManager::getInstance();

    /*
    sound1 = new KSound;
    sound1->loadSample(KMiscTools::makeFilePath("boing2.caf"));
    sound1->setVolume(1.0);
    
    sound2 = new KSound;
    sound2->loadSample(KMiscTools::makeFilePath("jump1.caf"));
    sound2->setVolume(1.0);
    
    music1 = new KMusic();
    music1->load(KMiscTools::makeFilePath("menu.mp3"));
    music1->playMusic();
    */
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
	

    r += (frameTime / 1000.0f) * 0.001f;
    a += (frameTime / 1000.0f) * 0.001f;
    z = 1.0 + cos(a * M_PI / 180.0f); // (frameTime / 1000.0f)

    static float debugval = 0.0;
    debugval += 0.4f;

    
    // FULL SCREEN IMAGE : 960x1440
    testGraphic3->blit(0, 0, 960, 1440, 0, 0);
    
    
    // SQUARE 256x256
    testGraphic->blit(0, 0, 256, 256, 140, 80, a, z);
     
     
     // SQUARE 256x256
    testGraphic2->blit(0, 0, 256, 256, 420, 850, 360-a, z);

    
    // SQUARE 512x512
    testGraphic4->blit(0, 0, 512, 512, 362, 353);
    
    
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

    
    // BUTTON
    
    buttonLeft->blit(0, 0, 40, 98, 0, 0);
    buttonMiddle->blit(0, 0, 40, 98, 0+50, 0);
    buttonRight->blit(0, 0, 40, 98, 0+50+100, 0);
    
    
	// TEXT
	
	fonte->RenderText(L"y=20: HELLO WORLD HOW ARE YOU!!!!!!", 10, 20, 1.0f);
	fonte->RenderText(L"y=80: i'm fine thank you", 10, 80, 1.0f);
	fonte->RenderText(L"y=140: OK it's good ;p", 10, 140, 1.0f);

	fonte->RenderText(L"y=500: OK it's good ;p", 10, 500, 1.0f);
	fonte->RenderText(L"y=700: OK it's good ;p", 10, 700, 1.0f);
	fonte->RenderText(L"y=900: OK it's good ;p", 10, 900, 1.0f);
	fonte->RenderText(L"y=1100: OK it's good ;p", 10, 1100, 1.0f);
	fonte->RenderText(L"y=1300: OK it's good ;p", 10, 1300, 1.0f);
	fonte->RenderText(L"y=1400: OK it's good ;p", 10, 1400, 1.0f);


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
