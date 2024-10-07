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

#ifndef API_KMISCTOOLS__
#define API_KMISCTOOLS__

#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>

//#ifdef MAX_PATH
//#undef MAX_PATH
#define	MAX_PATH		512
//#endif


static struct utsname systemInfo;

class KMiscTools
{
private:
	
	static	char	currentAppPath[MAX_PATH];
	static	char	currentFilePath[MAX_PATH];
	static	char	currentTempPath[MAX_PATH];
	static	char	currentDocHomePath[MAX_PATH];
	
	static	bool	_inPackage;
	
	static char		currentPath[MAX_PATH*2];


	
public:
	
	static	void initMiscTools();
	static	char * makeHomeDocFilePath(const char *filename);
	static	char * makeTempFilePath(const char *filename);
	static	char * makeFilePath(const char *filename);
	static	void messageBox(char *msg, char *title);
	static	short flipShort(short value);
	static	long flipLong(long value);
	static	long getSeed();
	static	unsigned long getMilliseconds();
	static  void  getSerial( char *s );
	static unsigned short pathSeparator(void) ;
	static	int getDocHome(char * path, unsigned int maxLen) ;
	
	static	int getUserHome(char * path, unsigned int maxLen) ;
	static int	getTempHome(char * path, unsigned int maxLen)	;
	static int  getCurrentDir(char * path, unsigned int maxLen) ;
	static int  getAppHome(char * path, unsigned int maxLen) ;
	static	void appendPath(char * path, char * component) ;
	static  char *concatePath(char * path1, char * path2) ;

	static bool isIphone6();
	static bool iPad();
	static bool iPadPro();
	static bool iPadMini();
	static float getScale();
	static int screenDPI();
	static bool isInternetReachable();
	static void getUserLanguage(char * lang);
	
	static int getUserID(char * path);
	static int createUserID(char * uid);
	
};


#endif // API_KMISCTOOLS__
