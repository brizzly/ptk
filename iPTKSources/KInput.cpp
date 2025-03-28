// Released under MIT License
/*
 Copyright (c) 2010 by Julien Meyer. Web: http://www.jmapp.com
 
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


//#include "KLogFile.h"
#include "KInput.h"
#include	"KPTK.h"

bool	KInput::_screenPressed = false;
int		KInput::_xScreenHit = 0;
int		KInput::_yScreenHit = 0;
int		KInput::_tapeCount = 0;
int		KInput::midX = 0;
int		KInput::midY = 0;
float	KInput::_xAcc = 0;
float	KInput::_yAcc = 0;
float	KInput::_zAcc = 0;
float	KInput::_rotation = 0;
bool	KInput::_rotation_begin = 0;
bool	KInput::_rotation_end = 0;
float	KInput::_pinch = 0;
bool	KInput::_pinch_begin = 0;
bool	KInput::_pinch_end = 0;
float   KInput::_pinch_centerx = 0;
float   KInput::_pinch_centery = 0;
float	KInput::_force = -1;
bool	KInput::_force_available = false;
bool	KInput::_needUpdate_Unpressed = false;

bool	KInput::_btPressed = ISUP;
long	KInput::_mx=0,KInput::_my =0;

sFinger	KInput::finger[2] = { 0,0,false ,0,0,false} ;

void KInput::initInput(int windowHandle)
{
	_screenPressed = false;
}

void KInput::hidePointer()
{
}

sFinger	*KInput::getFinger( short fingerid )
{
	if ( fingerid >1 ) return 0 ;
	return &finger[fingerid] ;
	
}

void KInput::setFingerPosition( int fingerid , short	x , short y , bool pressed )
{
	if( fingerid > 1 ) return;
	
	finger[fingerid].pressed = pressed ;
    
    finger[fingerid].x = x;
    finger[fingerid].y = y;
	
    /*
	if( KPTK::_screenOrientation == K_320x480 || KPTK::_screenOrientation == K_375x667 || KPTK::_screenOrientation == K_768x1024 || KPTK::_screenOrientation == K_621x1104 || KPTK::_screenOrientation == K_480x852 ) {
		finger[fingerid].x = x ;
	}
	else if(KPTK::_screenOrientation == K_480x320) {
		finger[fingerid].x = 480-y;
	}
	else if(KPTK::_screenOrientation == K_1024x768) {
		finger[fingerid].x = 1024-y;
	}
	
	if( KPTK::_screenOrientation == K_320x480 || KPTK::_screenOrientation == K_375x667 || KPTK::_screenOrientation == K_768x1024 || KPTK::_screenOrientation == K_621x1104 || KPTK::_screenOrientation == K_480x852) {
		finger[fingerid].y = y;
	}
	else {
		finger[fingerid].y = x;
	}*/
}

void KInput::swapFingerPosition(int fingerid1, int fingerid2)
{
	if( fingerid1 > 1 ) return;
	if( fingerid2 > 1 ) return;
	
	short x_ = finger[fingerid1].x;
	finger[fingerid1].x = finger[fingerid2].x;
	finger[fingerid2].x = x_;
	
	short y_ = finger[fingerid1].y;
	finger[fingerid1].y = finger[fingerid2].y;
	finger[fingerid2].y = y_;
	
	bool p_ = finger[fingerid1].pressed;
	finger[fingerid1].pressed = finger[fingerid2].pressed;
	finger[fingerid2].pressed = p_;
}

bool KInput::isPressed(int keyCode)
{
	return _btPressed;
	/*
	if(keyCode == K_VK_ESCAPE) {
		return ISUP;
	}
	return _screenPressed; //ISUP;
	*/
}

void KInput::mousePointerTo(int x, int y)
{
//	KLogFile::logDebug("mousePointerTo: %d %d", x, y);
	midX = x;
	midY = y;
}

int KInput::getMouseX()
{
    //printf("getMouseX _xScreenHit: %d\n", (int)_xScreenHit);
#ifdef __ANDROID__
    //__android_log_print(ANDROID_LOG_DEBUG, "PTK", "getMouseX _xScreenHit: %d\n", (int)_xScreenHit);
#endif

    return (int) _xScreenHit;
                  
	if( KPTK::_screenOrientation == K_621x1104 )
	{
        float r3ratio = 1104.0f / 736.0f; //621.0f / 1104.0f; //414.0f;
		return (int) (_xScreenHit * r3ratio);
	}
    else if( KPTK::_screenOrientation == K_480x852 )
    {
        float r3ratio = 480.0f;
        return (int) (_xScreenHit * r3ratio);
    }
	else if( KPTK::_screenOrientation == K_320x480 || KPTK::_screenOrientation == K_320x568 || KPTK::_screenOrientation == K_375x667 || KPTK::_screenOrientation == K_768x1024 )
	{
		return (int)_xScreenHit;
	}
	else if( KPTK::_screenOrientation == K_480x320) {
		return (int)480-_yScreenHit;
	}
	else if( KPTK::_screenOrientation == K_1024x768) {
		return (int)1024-_yScreenHit;
	}
	return 0;
}

int KInput::getMouseY()
{
    return (int) _yScreenHit;
    
	if( KPTK::_screenOrientation == K_621x1104 )
	{
		float r3ratio = 1104.0f / 736.0f;
		return (int) (_yScreenHit * r3ratio);
	}
    else if( KPTK::_screenOrientation == K_480x852 )
    {
        float r3ratio = 480.0f / 852.0f;
        return (int) (_yScreenHit * r3ratio);
    }
	else if( KPTK::_screenOrientation == K_320x480 || KPTK::_screenOrientation == K_320x568 || KPTK::_screenOrientation == K_375x667 || KPTK::_screenOrientation == K_768x1024 )
	{
		return (int)_yScreenHit;
	}
	return (int)_xScreenHit;
}

bool KInput::getLeftButtonState()
{
	return _btPressed;
}

void KInput::showPointer()
{
}


// iphone specific events calls

float KInput::getAccelerationX()
{
	return _xAcc;
}

float KInput::getAccelerationY()
{
	return _yAcc;
}

float KInput::getAccelerationZ()
{
	return _zAcc;
}

void KInput::setAccelerometer(float x, float y, float z)
{
	//printf("%f %f %f\n", x, y, z);
	_xAcc = x;
	_yAcc = y;
	_zAcc = z;
}

float KInput::getRotation() {
	return _rotation;
}
void KInput::setRotation(float r) {
	_rotation = r;
	//_rotation_begin = false;
	_rotation_end = false;
}
bool KInput::getRotationBegin() {
	return _rotation_begin;
}
void KInput::setRotationBegin(bool value) {
	_rotation_begin = value;
	if(value == true) {
		_rotation_end = false;
	}
}
bool KInput::getRotationEnd() {
	return _rotation_end;
}
void KInput::setRotationEnd(bool value) {
	_rotation_end = value;
	if(value == true) {
		_rotation_begin = false;
	}
}

float KInput::getPinch() {
	return _pinch;
}
void KInput::setPinch(float r) {
	_pinch = r;
	_pinch_end = false;
    //printf("setPinch %f\n",r);
}
float KInput::getPinchCenterX() {
    return _pinch_centerx;
}
float KInput::getPinchCenterY() {
    return _pinch_centery;
}
void KInput::setPinchCenterX(float value) {
    _pinch_centerx = value;
}
void KInput::setPinchCenterY(float value) {
    _pinch_centery = value;
}
bool KInput::getPinchBegin() {
	return _pinch_begin;
}
void KInput::setPinchBegin(bool value) {
	_pinch_begin = value;
    //printf("setPinchBegin %d\n", value ? 1 : 0);
}

bool KInput::getPinchEnd() {
	return _pinch_end;
}
void KInput::setPinchEnd(bool value) {
	_pinch_end = value;
    //printf("setPinchEnd %d\n", value ? 1 : 0);
    
	if(_pinch_end == true) {
		_pinch_begin = false;
        _pinch = 0;
	}
}


bool KInput::getForceAvailable() {
	return _force_available;
}
void KInput::setForceAvailable(bool value) {
	_force_available = value;
}

float KInput::getForce() {
	return _force;
}
void KInput::setForce(float value) {
	_force = value;
	if(_force < 0) {
		_force = 0;
	}
	else if(_force > 1) {
		_force = 1;
	}
}



int KInput::getScreenTap()
{
	return _tapeCount;
}

bool KInput::isScreenPressed(int *hitX, int *hitY)
{
	if(_screenPressed == false) {
		_btPressed = ISUP;
		return false;
	}
	
	_btPressed = ISDOWN;
	
    if(hitX) {
		*hitX = KInput::getMouseX();
	}
	if(hitY) {
		*hitY = KInput::getMouseY();
	}
	//KLogFile::logDebug("setScreenMoving: %d %d", *hitX, *hitY);
	return true;
}

bool KInput::isScreenReleased(int *hitX, int *hitY)
{
	if(_screenPressed == true) {
		return false;
	}
    if(hitX) {
		*hitX = KInput::getMouseX();
	}
	if(hitY) {
		*hitY = KInput::getMouseY();
	}	
	return true;
}

void KInput::setScreenPressed(int tapCount, int x, int y)
{
	//printf("setScreenPressed: %d %d\n", x, y);
	_btPressed = ISDOWN ;
	_screenPressed = true;
	_xScreenHit = x;
	_yScreenHit = y;
	_tapeCount = tapCount;
    
    //setFingerPosition( int fingerid , short    x , short y , bool pressed )
}

void KInput::setScreenMoving(int x, int y)
{
	//printf("setScreenMoving: %d %d\n", x, y);
	_xScreenHit = x;
	_yScreenHit = y;
}

void KInput::setScreenReleased()
{
	//printf("setScreenReleased\n");
	_screenPressed = false;
	_needUpdate_Unpressed = true;
	_btPressed = ISUP ;
	//_xScreenHit = -1;
	//_yScreenHit = -1;
	_tapeCount = 0;
}

void KInput::setFrameEnded()
{
	if(_needUpdate_Unpressed == true) {
		_needUpdate_Unpressed = false;
		_screenPressed = false;
	}
}









