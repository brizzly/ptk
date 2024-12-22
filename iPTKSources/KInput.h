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


#ifndef API_KINPUT__
#define API_KINPUT__

#define		ISDOWN			true
#define		ISUP			false

//#ifdef MAC_GRIZZ
#include <stdbool.h>
//#endif

#ifdef __ANDROID__
#include <android/log.h>
#endif





enum		EKeyboardLayout	{ 
	K_VK_LEFT=0  ,
	K_VK_UP ,
	K_VK_DOWN ,
	K_VK_RIGHT	,
	K_VK_SPACE , 
	K_VK_L_SHIFT , 
	K_VK_R_SHIFT ,
	K_VK_RETURN ,
	K_VK_R_CONTROL , 
	K_VK_L_CONTROL ,
	K_VK_F1 ,
	K_VK_F2 ,
	K_VK_F3 , K_VK_F4 , K_VK_F5 , K_VK_F6 , K_VK_F7 , K_VK_F8 , K_VK_F9 , K_VK_F10 ,K_VK_F11 , K_VK_F12 ,
	K_VK_BACK , K_VK_TAB ,
	K_VK_ESCAPE ,
	K_VK_A ,
	K_VK_B ,
	K_VK_C ,
	K_VK_D ,
	K_VK_E ,
	K_VK_F ,
	K_VK_G ,
	K_VK_H ,
	K_VK_I ,
	K_VK_J ,
	K_VK_K ,
	K_VK_L ,
	K_VK_M ,
	K_VK_N ,
	K_VK_O ,
	K_VK_P ,
	K_VK_Q ,
	K_VK_R ,
	K_VK_S ,
	K_VK_T ,
	K_VK_U ,
	K_VK_V ,
	K_VK_W ,
	K_VK_X ,
	K_VK_Y ,
	K_VK_Z ,
	K_VK_0,
	K_VK_1,
	K_VK_2,
	K_VK_3 ,
	K_VK_4 ,
	K_VK_5 ,
	K_VK_6 ,
	K_VK_7 ,
	K_VK_8 ,
	K_VK_9,
	K_VK_NUM0 ,K_VK_NUM1 ,K_VK_NUM2 ,K_VK_NUM3 ,K_VK_NUM4 ,K_VK_NUM5 ,K_VK_NUM6 ,K_VK_NUM7 ,K_VK_NUM8 ,K_VK_NUM9  ,
	K_VK_MULTIPLY ,
	K_VK_ADD ,
	K_VK_SUBTRACT ,
	K_VK_DECIMAL ,
	K_VK_DIVIDE ,
	K_VK_CLEAR ,
	K_VK_MENU ,
	K_VK_LWIN ,
	K_VK_RWIN , 
	K_VK_NUMLOCK ,
	K_VK_SCROLL ,
	K_VK_OEM_1,K_VK_OEM_PLUS ,      
	K_VK_OEM_COMMA ,  
	K_VK_OEM_MINUS   ,  
	K_VK_OEM_PERIOD , 
	K_VK_OEM_2     ,     
	K_VK_OEM_3     ,     
	K_VK_OEM_4      ,    
	K_VK_OEM_5     ,     
	K_VK_OEM_6    ,      
	K_VK_OEM_7  ,
	K_VK_END , 
	K_VK_HOME , K_VK_DEL , 
	K_VK_INSERT ,
	K_VK_SNAPSHOT ,
	K_VK_PRIOR ,
	K_VK_NEXT ,
	K_VK_ERROR       
};

struct	KeyStroke
{
	enum EKeyboardLayout	toucheID;
	unsigned	short		KeyCode;
};


extern	struct KeyStroke			KeyboardLayout[];


struct	sFinger	
{
	short		x,y ;
	bool		pressed ;
};

class KInput
{
private:
	static			sFinger			finger[2];
	
	static bool		_screenPressed;
	static int		_xScreenHit;
	static int		_yScreenHit;
	static int		_tapeCount;
	static float	_xAcc;
	static float	_yAcc;
	static float	_zAcc;
	static float	_rotation;
	static bool		_rotation_begin;
	static bool		_rotation_end;
	static float	_pinch;
	static bool		_pinch_begin;
	static bool		_pinch_end;
	static float	_force;
	static bool		_force_available;
	static bool		_needUpdate_Unpressed;
	
	static int		midX, midY;
	static			bool	_btPressed ;
	static			long	_mx,_my ;
public:
		
	static void		initInput(int windowHandle);
	static void		hidePointer();
	static bool		isPressed(int keyCode);
	static void		mousePointerTo(int x, int y);
	static int		getMouseX();
	static int		getMouseY();		
	static bool		getLeftButtonState();
	static void		showPointer();
	
	// iPhone specific 
	static float	getAccelerationX();
	static float	getAccelerationY();
	static float	getAccelerationZ();
	static void		setAccelerometer(float x, float y, float z);
	
	static float	getRotation();
	static void		setRotation(float r);
	static bool		getRotationBegin();
	static void		setRotationBegin(bool value);
	static bool		getRotationEnd();
	static void		setRotationEnd(bool value);
	
	static float	getPinch();
	static void		setPinch(float r);
	static bool		getPinchBegin();
	static void		setPinchBegin(bool value);
	static bool		getPinchEnd();
	static void		setPinchEnd(bool value);
	static float	getForce();
	static void		setForce(float value);
	static bool		getForceAvailable();
	static void		setForceAvailable(bool value);

	static int		getScreenTap();
	static bool		isScreenPressed(int *hitX=0, int *hitY=0);
	static bool		isScreenReleased(int *hitX, int *hitY);
	//max 2 fingers  : 0 or 1
	static	sFinger*		getFinger( short fingerid ) ;
	

	//do not use unless you know what you're doing
	static void		setFingerPosition( int fingerid , short	x , short y , bool pressed ) ;
	static void		swapFingerPosition(int fingerid1, int fingerid2);
	
	static void		setScreenPressed(int tapCount, int x, int y);
	static void		setScreenMoving(int x, int y);
	static void		setScreenReleased();
	
	static void		setFrameEnded();

	
};


#endif // API_KINPUT__
