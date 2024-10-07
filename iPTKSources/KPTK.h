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

#ifndef API_KPTK__
#define API_KPTK__

#include <stdio.h>
#include <stdlib.h>
#include "KWindow.h"
#include "KGraphic.h"


#define	PTK_FREEPTR(x)   if(x)  { delete x ; x = NULL ; }

enum screenOrientation
{
	// ANDROID
	
	K_320x568,
	K_568x320,
	
	// IPHONE
	
	K_320x480,
	K_480x320,
    
    K_640x480,
	
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
	
	static	screenOrientation	_screenOrientation;
    
    static float _screenW, _screenH;
    

	static	float				_X(float value);
	static	float				_Y(float value);
	static	float				inv_X(float value);
	static	float				inv_Y(float value);
	static	bool				isEyeRetina();
	static	bool				isEyeRetina_4();
	static bool					isEyeRetina_45();
	static bool 				isEyeRetina_5();
	static	int					_H(int value);
	static	KWindow			*	createKWindow(screenOrientation orientation);
	static	KGraphic		*	createKGraphic();
	static	int					getGameW();
	static	int					getGameH();
    static  int                 getScreenW();
    static  int                 getScreenH();
     
};


#endif // API_KPTK__

