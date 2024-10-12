package com.jmapp.testandroid;

import android.app.Activity;
import android.os.Bundle;
import android.view.WindowManager;
import android.content.res.AssetManager;

public class MainActivity extends Activity {

    static {
        System.loadLibrary("android_game_lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);

        // Disable the notification bar for full screen
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

        // Load native library
        System.loadLibrary("android_game_lib");

        // Obtenez l'AssetManager et transmettez-le au code natif
        AssetManager assetManager = getAssets();
        nativeSetAssetManager(assetManager);

        // Initialize GLFM
        GLFMActivity glfm = new GLFMActivity(this);

        // Set the content view
        setContentView(glfm.getView());
    }

    private native void nativeSetAssetManager(AssetManager assetManager);
}

