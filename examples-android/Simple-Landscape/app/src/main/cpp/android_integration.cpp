#include "glfm.h"
#include "MainController.h"
#include <jni.h>
#include <cmath>
//#include <android/asset_manager.h>
#include <android/native_window_jni.h> // Required for native window integration
#include <android/asset_manager_jni.h> // Include for AAssetManager_fromJava
#include <android/log.h>

#define LOG_TAG "NativeCode"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "TAG", __VA_ARGS__))
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)



// Render function called each frame
void onRender(GLFMDisplay *display) {
    MainController::getInstance()->update(0);
    MainController::getInstance()->draw();
}

void onFrame(GLFMDisplay *display, double frameTime)
{
    MainController::getInstance()->update(frameTime);
    MainController::getInstance()->draw();
}

// Function called during app initialization
void onAppInit(GLFMDisplay *display) {
    // Set render function
    //glfmSetRenderFunc(display, onRender);
    glfmSetMainLoopFunc(display, onFrame);
    // Initialize the main controller
    //MainController::getInstance()->initialize(display);
}

extern "C" {

    JNIEXPORT void JNICALL
    Java_com_jmapp_testandroid_MainActivity_handleTouchEvent(JNIEnv* env, jobject /* this */, jint action, jfloat x, jfloat y) {
        switch (action) {
            case AMOTION_EVENT_ACTION_DOWN:
                LOGI("Touch down at (%f, %f)", x, y);
                MainController::getInstance()->touchEvent(0, action, x, y);
                break;
            case AMOTION_EVENT_ACTION_MOVE:
                LOGI("Touch move to (%f, %f)", x, y);
                MainController::getInstance()->touchEvent(0, action, x, y);
                break;
            case AMOTION_EVENT_ACTION_UP:
                LOGI("Touch up at (%f, %f)", x, y);
                MainController::getInstance()->touchEvent(0, action, x, y);
                break;
            case AMOTION_EVENT_ACTION_CANCEL:
                LOGI("Touch canceled at (%f, %f)", x, y);
                MainController::getInstance()->touchEvent(0, action, x, y);
                break;
            default:
                LOGI("Other touch event (%d) at (%f, %f)", action, x, y);
                MainController::getInstance()->touchEvent(1, action, x, y);
                break;
        }

    }


    JNIEXPORT void JNICALL
    Java_com_jmapp_testandroid_MainActivity_nativeSetAssetManager(JNIEnv* env, jobject obj, jobject assetManager) {
        AAssetManager * g_assetManager = AAssetManager_fromJava(env, assetManager);

        MainController::getInstance()->setAssetManager(g_assetManager);
    }

    // Main GLFM function
    void glfmMain(GLFMDisplay *display) {

        if (!display) {
            //KLogFile::logDebug(("GLFMDisplay is null");
            return;
        }

        glfmSetUserInterfaceOrientation(display, GLFMUserInterfaceOrientationLandscape);

        // Configure display preferences
        glfmSetDisplayConfig(display,
                             GLFMRenderingAPIOpenGLES2,
                             GLFMColorFormatRGBA8888,
                             GLFMDepthFormatNone,
                             GLFMStencilFormatNone,
                             GLFMMultisampleNone);

        // Call the app initialization function
        onAppInit(display);
    }


    JNIEXPORT void JNICALL
    Java_com_jmapp_testandroid_GLFMActivity_nativeOnSurfaceCreated(JNIEnv *env, jobject thiz) {
        LOGE("nativeOnSurfaceCreated called");
        //MainController::getInstance()->initialize(thiz);
    }

    JNIEXPORT void JNICALL
    Java_com_jmapp_testandroid_GLFMActivity_nativeOnSurfaceChanged(JNIEnv *env, jobject thiz, jint width, jint height) {
        LOGE("nativeOnSurfaceChanged called with width: %d, height: %d", width, height);

        MainController::getInstance()->initialize(width, height);

        // Update the OpenGL viewport
        //glViewport(0, 0, width, height);

    }

    // JNI function for rendering each frame
    JNIEXPORT void JNICALL
    Java_com_jmapp_testandroid_GLFMActivity_nativeOnDrawFrame(JNIEnv *env, jobject obj) {
        //LOGE("nativeOnDrawFrame called");
        MainController::getInstance()->draw();
    }

}