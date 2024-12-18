package com.jmapp.testandroid;

import android.opengl.GLSurfaceView;
import android.opengl.GLES20;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;


public class GLFMRenderer implements GLSurfaceView.Renderer {

    private GLFMActivity glfmActivity;

    public GLFMRenderer(GLFMActivity glfmActivity) {
        this.glfmActivity = glfmActivity;
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        // Called when the surface is created or recreated.
        //GLES20.glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set the background frame color
        glfmActivity.nativeOnSurfaceCreated();
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        // Called to draw the current frame.
        //GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);
        glfmActivity.nativeOnDrawFrame();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        // Called if the geometry of the view changes, for example when the device's screen orientation changes.
        //GLES20.glViewport(0, 0, width, height);
        glfmActivity.nativeOnSurfaceChanged(width, height);
    }
}
