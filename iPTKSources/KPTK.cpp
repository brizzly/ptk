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

#include "KPTK.h"
#include "KGraphic.h"
#include "KLogFile.h"
#include "KWindow.h"
#include "KMiscTools.h"


screenOrientation KPTK::_screenOrientation = K_960x1440;

float KPTK::_screenW, KPTK::_screenH;



/*
int KPTK::_H(int value)
{
    return value;
}

float KPTK::_X(float value)
{
	return value;
}
float KPTK::inv_X(float value)
{
	return value;
}

float KPTK::_Y(float value)
{
	return value;
}
float KPTK::inv_Y(float value)
{
	return value;
}
*/



int KPTK::getScreenW()
{
    return _screenW;
}

int KPTK::getScreenH()
{
    return _screenH;
}

int KPTK::getGameW()
{
	if ( _screenOrientation == K_320x480 ) {
		return 320;
	}
	if ( _screenOrientation == K_320x568 ) {
		return 320;
	}
	else if ( _screenOrientation == K_375x667 ) {
		return 375;
	}
	else if ( _screenOrientation == K_480x320 ) {
		return 480;
	}
    else if ( _screenOrientation == K_568x320 ) {
        return 568;
    }
    else if ( _screenOrientation == K_640x480 ) {
        return 640;
    }
    else if ( _screenOrientation == K_640x1136 ) {
        return 640;
    }
    else if ( _screenOrientation == K_960x1440 ) {
        return 960;
    }
	else if ( _screenOrientation == K_621x1104 ) {
		return 621;//414;
	}
	else if ( _screenOrientation == K_768x1024 ) {
		return 768;
	}
	else if ( _screenOrientation == K_1024x768 ) {
		return 1024;
	}
    else if ( _screenOrientation == K_480x852 ) {
        return 480;
    }
	return 0;
}

int KPTK::getGameH()
{
	if ( _screenOrientation == K_320x480 ) {
		return 480;
	}
	if ( _screenOrientation == K_320x568 ) {
		return 568;
	}
	else if ( _screenOrientation == K_375x667 ) {
		return 667;
	}
	else if ( _screenOrientation == K_480x320 ) {
		return 320;
	}
    else if ( _screenOrientation == K_568x320 ) {
        return 320;
    }
    else if ( _screenOrientation == K_640x480 ) {
        return 480;
    }
    else if ( _screenOrientation == K_640x1136 ) {
        return 1136;
    }
    else if ( _screenOrientation == K_960x1440 ) {
        return 1440;
    }
	else if ( _screenOrientation == K_621x1104 ) {
		return 1104;//736*2;
	}
	else if ( _screenOrientation == K_768x1024 ) {
		return 1024;
	}
	else if ( _screenOrientation == K_1024x768 ) {
		return 768;
	}
    else if ( _screenOrientation == K_480x852 ) {
        return 852;
    }
	return 0;
}

KWindow * KPTK::createKWindow(screenOrientation orientation)
{
	int debug = (int)orientation;
	//printf("d=%d\n", debug);
	
	_screenOrientation = orientation;
	KPTK::_screenOrientation = orientation; // 4 avril 2016	
	
	KWindow * ptk_Window = new KWindow();
	ptk_Window->createGameWindow(KPTK::getGameW(), KPTK::getGameH(), 32, false, (char*)"XEON");
	ptk_Window->setClearColor(0,0,0,0);
	return ptk_Window;
}

#ifdef __ANDROID__

#else
KGraphic * KPTK::createKGraphic()
{
    // Create an instance of KGraphic
    int gameW = KPTK::getGameW();
    int gameH = KPTK::getGameH();
    KGraphic * graphic = new KGraphic(gameW, gameH, _screenW, _screenH);
   return graphic;
}
#endif
