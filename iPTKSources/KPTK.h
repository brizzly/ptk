// Released under MIT License
/*
 Copyright (c) 2010 by Julien Meyer. Web: http://www.jadegame.com
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

#ifndef KPTK_H
#define KPTK_H

#include "KGraphic.h"
#include "KWindow.h"

#ifdef __ANDROID__
#include <android/asset_manager_jni.h>
#endif

//class KWindow;


enum screenOrientation
{
	// ANDROID
	
	K_320x568,
	K_568x320,
	
	// IPHONE
	
	K_320x480,
	K_480x320,
    
    K_640x480,
    K_640x1136,
    K_960x1440,
	
	// IPHONE6
	
	K_375x667,
	K_667x375,
	
	// IPHONE 6P,7P,8P 1242x2208 -> 621x1104
    
	K_621x1104,
	K_1104x621,
    
    // IPHONE 12Pro
    
    K_480x852,

	// IPAD
	
	K_768x1024,
	K_1024x768
};


class KPTK
{
public:

    static int _screenW;
    static int _screenH;
    static screenOrientation _screenOrientation;

    static void setScreenSize(int width, int height);
    static void setScreenOrientation(screenOrientation orientation);
    
public:

	static	KWindow			*	createKWindow(screenOrientation orientation);

#ifdef __ANDROID__
    static  KGraphic        *   createKGraphic(AAssetManager * assetManager);
#else
    static	KGraphic		*	createKGraphic();
#endif

	static	int					getGameW();
	static	int					getGameH();
    static  int                 getScreenW();
    static  int                 getScreenH();
};

#endif // KPTK_H

