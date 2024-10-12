// GLFMActivity.java (Fixed)
// Fixed incorrect initialization and updated View handling.

package com.jmapp.testandroid;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.View;

public class GLFMActivity {

    private GLSurfaceView glSurfaceView;

    public GLFMActivity(Context context) {
        // Create the GLSurfaceView
        glSurfaceView = new GLSurfaceView(context);

        // Set the OpenGL ES version to 2
        glSurfaceView.setEGLContextClientVersion(2);

        // Set the renderer to our native renderer
        glSurfaceView.setRenderer(new GLFMRenderer(this));

        // Call the native method when the SurfaceView is created
        nativeOnSurfaceCreated();
    }

    public View getView() {
        return glSurfaceView;
    }

    // Native method for creating OpenGL surface
    public native void nativeOnSurfaceCreated();

    // Native method for handling surface changes
    public native void nativeOnSurfaceChanged(int width, int height);

    public native void nativeOnDrawFrame();

    static {
        // Load the native library
        System.loadLibrary("android_game_lib");
    }
}


