
// https://github.com/brackeen/glfm

#import <UIKit/UIKit.h>
#import <OpenGLES/ES2/gl.h>
#import "TestPortrait-Bridging-Header.h"
#import "game.h"
#import "MainController.h"
#import "glfm.h"

@class GLFMViewController;

@implementation MainController


game gameInstance;


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
    
	gameInstance.init(width, height);
}

// Rendering function
void onFrame(GLFMDisplay *display, double frameTime)
{
    int screenWidth, screenHeight;
    glfmGetDisplaySize(display, &screenWidth, &screenHeight);
    
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	gameInstance.draw(frameTime);
}

// Function to handle surface destruction (cleanup)
void onSurfaceDestroyed(GLFMDisplay *display)
{
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
			gameInstance.playSfx();
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
