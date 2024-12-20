package com.jmapp.testandroid.landscape;

import android.app.Activity;
import android.os.Bundle;
import android.view.WindowManager;
import android.content.res.AssetManager;
import android.view.MotionEvent;

public class MainActivity extends Activity {

    static {
        System.loadLibrary("openal");
        System.loadLibrary("android_landscape_game_lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);

        // Disable the notification bar for full screen
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

        // Load native library
        System.loadLibrary("android_landscape_game_lib");

        // Obtenez l'AssetManager et transmettez-le au code natif
        AssetManager assetManager = getAssets();
        nativeSetAssetManager(assetManager);

        // Initialize GLFM
        GLFMActivity glfm = new GLFMActivity(this);

        // Set the content view
        setContentView(glfm.getView());
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        int action = event.getActionMasked();
        float x = event.getX();
        float y = event.getY();

        // Pass touch event data to native code
        handleTouchEvent(action, x, y);

        return super.onTouchEvent(event);
    }

    // Native methods declarations
    private native void nativeSetAssetManager(AssetManager assetManager);
    private native void handleTouchEvent(int action, float x, float y);

}

