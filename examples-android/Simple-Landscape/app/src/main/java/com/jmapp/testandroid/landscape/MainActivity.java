package com.jmapp.testandroid.landscape;

import android.app.Activity;
import android.os.Bundle;
import android.os.PowerManager;
import android.view.WindowManager;
import android.content.res.AssetManager;
import android.view.MotionEvent;
import android.content.pm.ActivityInfo;

public class MainActivity extends Activity {

    static {
        System.loadLibrary("openal");
        System.loadLibrary("android_landscape_game_lib");
    }

    private PowerManager.WakeLock wakeLock;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);

        //setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);

        // Disable the notification bar for full screen
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

        // Acquire a wake lock to prevent screen dimming
        PowerManager powerManager = (PowerManager) getSystemService(POWER_SERVICE);
        wakeLock = powerManager.newWakeLock(PowerManager.SCREEN_BRIGHT_WAKE_LOCK | PowerManager.ON_AFTER_RELEASE, "MyApp::MyWakeLock");
        wakeLock.acquire();

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
    protected void onPause()
    {
/*
        if (adView != null) {
            adView.pause();
        }
*/
        super.onPause();
        if (wakeLock != null && wakeLock.isHeld()) {
            wakeLock.release();
        }
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

    @Override
    protected void onResume() {
        /*if (adView != null) {
            adView.resume();
        }*/
        super.onResume();
    }

    @Override
    protected void onDestroy() {
        /*if (adView != null) {
            adView.destroy();
        }*/
        super.onDestroy();
    }



    // Native methods declarations
    private native void nativeSetAssetManager(AssetManager assetManager);
    private native void handleTouchEvent(int action, float x, float y);

}

