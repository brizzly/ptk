#include "glfm.h"
#include "MainController.h"
#include <jni.h>
#include <android/log.h>


#define LOG_TAG "NativeCode"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Render function called each frame
void onRender(GLFMDisplay *display) {
    MainController::getInstance()->update(0);
    MainController::getInstance()->draw();
}

// Function called during app initialization
void onAppInit(GLFMDisplay *display) {
    // Set render function
    glfmSetRenderFunc(display, onRender);
    // Initialize the main controller
    MainController::getInstance()->initialize(display);
}



// Main GLFM function
extern "C" void glfmMain(GLFMDisplay *display) {
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


extern "C" JNIEXPORT void JNICALL
Java_com_jmapp_testandroid_MainActivity_nativeFunction(JNIEnv *env, jobject thiz) {
    LOGE("nativeFunction called");
    // Your native logic here, such as invoking OpenGL functionality or other native processes
}




extern "C" {

JNIEXPORT void JNICALL
Java_com_jmapp_testandroid_GLFMActivity_nativeOnSurfaceCreated(JNIEnv *env, jobject thiz) {
    //LOGE("nativeOnSurfaceCreated called");
    MainController::getInstance()->initialize(nullptr);
}

/*
// JNI function for handling changes in surface size
JNIEXPORT void JNICALL
Java_com_jmapp_testandroid_GLFMRenderer_nativeOnSurfaceChanged(JNIEnv *env, jobject obj, jint width, jint height) {
    LOGE("nativeOnSurfaceChanged called with width: %d, height: %d", width, height);

    // Set the viewport to the new dimensions
    glViewport(0, 0, width, height);
}
*/

JNIEXPORT void JNICALL
Java_com_jmapp_testandroid_GLFMActivity_nativeOnSurfaceChanged(JNIEnv *env, jobject thiz, jint width, jint height) {
    LOGE("nativeOnSurfaceChanged called with width: %d, height: %d", width, height);

    // Update the OpenGL viewport
    glViewport(0, 0, width, height);
}

// JNI function for rendering each frame
JNIEXPORT void JNICALL
Java_com_jmapp_testandroid_GLFMActivity_nativeOnDrawFrame(JNIEnv *env, jobject obj) {
    //LOGE("nativeOnDrawFrame called");
    MainController::getInstance()->draw();
}

/*
// JNI function for rendering each frame
JNIEXPORT void JNICALL
Java_com_jmapp_testandroid_GLFMRenderer_nativeOnDrawFrame(JNIEnv *env, jobject obj) {
    LOGE("nativeOnDrawFrame called");

    // Clear the screen
    //glClearColor(1.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // Call MainController to draw the scene
    MainController::getInstance()->draw();
}
*/

}