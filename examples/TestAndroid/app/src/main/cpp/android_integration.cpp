#include "glfm.h"
#include "MainController.h"
#include <jni.h>
#include <android/log.h>
//#include <android/asset_manager.h>
#include <android/native_window_jni.h> // Required for native window integration
#include <android/asset_manager_jni.h> // Include for AAssetManager_fromJava

#define LOG_TAG "NativeCode"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

//AAssetManager* g_assetManager = nullptr;


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