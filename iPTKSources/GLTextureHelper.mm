/*
 *  GLTextureHelper.cpp
 *  OpenGL
 *
 // Released under BSD License
 /* Copyright (c) 2007, Richard S. Wright Jr.
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without modification, 
 are permitted provided that the following conditions are met:
 
 Redistributions of source code must retain the above copyright notice, this list 
 of conditions and the following disclaimer.
 
 Redistributions in binary form must reproduce the above copyright notice, this list 
 of conditions and the following disclaimer in the documentation and/or other 
 materials provided with the distribution.
 
 Neither the name of Richard S. Wright Jr. nor the names of other contributors may be used 
 to endorse or promote products derived from this software without specific prior 
 written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
 OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED 
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR 
 BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "GLTextureHelper.h"
#include <QuartzCore/QuartzCore.h>
#include <Foundation/NSBundle.h>
#include <UIKit/UIKit.h>
#include <UIKit/UIImage.h>

#include <sys/mman.h>
#include <sys/fcntl.h>

#include "KMiscTools.h"


GLTextureHelper::GLTextureHelper(bool inv_y_axis)
{
	_invertYaxis = inv_y_axis;
	_pData = NULL;
	_surfaceRetina = false;
	_nWidth = 0;
	_nHeight = 0;
	_texSizeW = 0;
	_texSizeH = 0;
}

GLTextureHelper::~GLTextureHelper(void)
{
	if(_pData) {
		free(_pData);
		_pData = NULL;
	}
}

char * GLTextureHelper::loadFileDatas(const char *filename)
{
    NSString *texturePath = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:filename] ofType:nil];
    if (!texturePath) {
        printf("Failed to find texture: %s\n", filename);
        return nullptr;
    }
    
    // Copy the UTF8 string to a heap-allocated C-string to ensure it persists
    const char *utf8String = [texturePath UTF8String];
    char *copy = (char *)malloc(strlen(utf8String) + 1);
    strcpy(copy, utf8String);

    return copy;
}

void GLTextureHelper::clearSurface(char * surface, int w, int h, int bpp, int R, int G, int B, int A)
{
	int pixelSize = bpp/8;
	for(int i=0 ; i<w ; i++)
	{
		for(int j=0 ; j<h ; j++)
		{
			int pos = (i + (w * j))*pixelSize;
			if(pixelSize == 3)
			{
				surface[pos+0] = R;
				surface[pos+1] = G;
				surface[pos+2] = B;
			}
			else if(pixelSize == 4)
			{
				surface[pos+0] = R;
				surface[pos+1] = G;
				surface[pos+2] = B;
				surface[pos+3] = A;
			}
		}		
	}	
}

bool GLTextureHelper::isScreenRetina_iPhone4()
{
	UIScreen* mainscr = [UIScreen mainScreen];
	int w = mainscr.currentMode.size.width;
	int h = mainscr.currentMode.size.height;
	if (w == 640 && h == 960) // Retina 3.5" display detected (iphone4)
	{
		return true;
	}
	return false;
}

bool GLTextureHelper::isScreenRetina()
{
	UIScreen* mainscr = [UIScreen mainScreen];
	int w = mainscr.currentMode.size.width;
	int h = mainscr.currentMode.size.height;
	if (w == 640 && h == 960) // Retina 3.5" display detected
	{
		return true;
	}
	if(w == 960 && h == 1704) // Retina iPhone 6+,7+,8+
	{
		return true;
	}
	if(w == 1242 && h == 2208) // Retina iPhone 7+,8+
	{
		return true;
	}
	if(w == 1125 && h == 2001) // Retina iPhone 8+ Scale2 instead of 3
	{
		return true;
	}
	if(w == 1125 && h == 2436) // Retina iPhone X
	{
		return true;
	}
	if ((w == 640 && h == 1136) || (w == 1136 && h == 640)) // Retina 4" display detected
	{
		return true;
	}
	if ((w == 750 && h == 1334) || (w == 1080 && h == 1920) ) // Retina 4"5 and 5"5 display detected
	{
		return true;
	}
	if ((w == 1536 && h == 2048) || (w == 2048 && h == 1536)) // Retina 13" iPad display detected
	{
		return true;
	}
	if(w == 2048 && h == 2732) // ipad pro
	{
		return true;
	}
	return false;
}

bool GLTextureHelper::iPadScreen()
{
	UIScreen* mainscr = [UIScreen mainScreen];
	int w = mainscr.currentMode.size.width;
	int h = mainscr.currentMode.size.height;
	if (w == 1024 && h == 768) // iPad non retina detected
	{
		return true;
	}
	if (w == 2048 && h == 1536) // iPad retina detected
	{
		return true;
	}
	if(w == 2048 && h == 2732) // ipad pro
	{
		return true;
	}
	return false;
}

bool GLTextureHelper::LoadTexture(const char *szTextureFile)
{
	CGImageRef Image;
	CGContextRef Context;
	long   v;
	int rx = 1;
	int ry = 1;
	
	if(!szTextureFile || !strcmp(szTextureFile, "")) {
		return false;
	}
		
	const char *extension = &szTextureFile[strlen(szTextureFile)-3];
	if(!strcmp(extension, "tga")) 
	{
		JadeImage_t tgaImage;
		tgaImage.filename = (char *)szTextureFile;
		int res = LoadFileTGA(&tgaImage);
		if(res == 0) {
			return false;
		}
				
		_nWidth = tgaImage.width;
		_nHeight = tgaImage.height;
		
		if ( _nWidth > _nHeight )
		{
			v = _nWidth;
		}
		else
		{
			v = _nHeight;
		}
		_texSizeW = 1;
		do
		{
			_texSizeW *= 2;
			if ( _texSizeW >= v )
				break;
		}
		while ( _texSizeW < 1024  );
		
		rx = _texSizeW;
		ry = _texSizeW;		
		
		int pixelSize = tgaImage.bpp/8;
		GLubyte * newSurface = (GLubyte *) calloc(rx * ry * 4, sizeof(GLubyte));
		
		// recopie dans la plus grande surface puissance de 2 notre texture non puissance de 2
		unsigned char R, G, B, A;
		for(int i=0 ; i<tgaImage.width ; i++)
		{
			for(int j=0 ; j<tgaImage.height ; j++)
			{
				int pos = (i + (tgaImage.width * j))*pixelSize;
				int pos2 = (i + (rx * (tgaImage.height-j-1)))*4;
				
				R = tgaImage.pixels[(pos)+0];
				G = tgaImage.pixels[(pos)+1];
				B = tgaImage.pixels[(pos)+2];
				if(pixelSize == 3)
				{
					A = 255;
				}
				else if(pixelSize == 4)
				{
					A = tgaImage.pixels[(pos)+3];
				}
				
				newSurface[(pos2)+0] = (GLubyte)R;
				newSurface[(pos2)+1] = (GLubyte)G;
				newSurface[(pos2)+2] = (GLubyte)B;
				newSurface[(pos2)+3] = (GLubyte)A;
				
				//printf("%d %d %d %d\n", (int)R, (int)G, (int)B, (int)A);
			}		
		}
		
		_pData = newSurface;
		_nWidth = rx;
		_nHeight = ry;	
		free(tgaImage.pixels);
	}
	else
	{			
		NSString * stringFromChar = [NSString stringWithCString:szTextureFile encoding:NSUTF8StringEncoding];
		
		_surfaceRetina = false;
		
		stringFromChar = [stringFromChar stringByReplacingOccurrencesOfString:@"@2x" withString:@""];
		
		UIImage * myimage = nil;
	//	if ( [[[UIDevice currentDevice] systemVersion] intValue] >= 4 && [[UIScreen mainScreen] scale] == 2.0 )
	//	{
		NSString *file_noext = [[stringFromChar lastPathComponent] stringByDeletingPathExtension];
		NSString *ext = [stringFromChar pathExtension];
		
		NSString *path2x = [[stringFromChar stringByDeletingLastPathComponent]
							stringByAppendingPathComponent:[NSString stringWithFormat:@"%@@2x",
															file_noext
															]];
		/*
		NSString *path2x = [[stringFromChar stringByDeletingLastPathComponent]
							stringByAppendingPathComponent:[NSString stringWithFormat:@"%@@2x.%@",
															[[stringFromChar lastPathComponent] stringByDeletingPathExtension],
															ext]];*/
		
		NSString * exists = [[NSBundle mainBundle] pathForResource:path2x ofType:ext];
		if(exists)
		{
			_surfaceRetina = true;
			myimage = [UIImage imageNamed:stringFromChar];
		}
		else
		{
			myimage = [UIImage imageNamed:stringFromChar];
		}
		
		
		Image = myimage.CGImage;
		if(Image == NULL) {
			return false;
		}
		
		// Get the width and height of the image
		_nWidth = (GLint) CGImageGetWidth(Image);
		_nHeight = (GLint) CGImageGetHeight(Image);
	
		// Texture dimensions must be a power of 2. If you write an application that allows users to supply an image,
		// you'll want to add code that checks the dimensions and takes appropriate action if they are not a power of 2.
		
		const char* extensions = (char*) glGetString(GL_EXTENSIONS);
		bool npot = strstr(extensions, "GL_APPLE_texture_2D_limited_npot") != 0;
		if(npot)
		{
		}
		
		_texSizeW = 1;
		do
		{
			_texSizeW *= 2;
			if ( _texSizeW >= _nWidth )
				break;
		}
		while ( _texSizeW < 2048  );
		
		_texSizeH = 1;
		do
		{
			_texSizeH *= 2;
			if ( _texSizeH >= _nHeight )
				break;
		}
		while ( _texSizeH < 2048  );
		
		
		if(Image == NULL) {
			return false;
		}
		
		// Allocated memory needed for the bitmap context
		_pData = (GLubyte *) calloc(_texSizeW * _texSizeH * 4, sizeof(GLubyte));
		
		// Uses the bitmap creation function provided by the Core Graphics framework. 
		Context = CGBitmapContextCreate(_pData, _nWidth, _nHeight, 8, _texSizeW * 4, CGImageGetColorSpace(Image), kCGImageAlphaPremultipliedLast);

		rx = _texSizeW;
		ry = _texSizeH;
		
		if(_invertYaxis == true)
		{
			CGContextTranslateCTM(Context, 0, (CGFloat)_nHeight);
			CGContextScaleCTM(Context, 1.0, -1.0);
		}
		
		// After you create the context, you can draw the sprite image to the context.
		CGContextDrawImage(Context, CGRectMake(0.0, 0.0, (CGFloat)_nWidth, (CGFloat)_nHeight), Image);

		// You don't need the context at this point, so you need to release it to avoid memory leaks.
		CGContextRelease(Context);
	}
	
	// Specify a 2D texture image, providing the a pointer to the image data in memory
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rx, ry, 0, GL_RGBA, GL_UNSIGNED_BYTE, _pData);
	
	//glGenerateMipmap(GL_TEXTURE_2D);
	
	if(_pData) {
		free(_pData);
		//delete [] _pData ;
		_pData = NULL;
	}
	return true;
}

int GLTextureHelper::LoadFileTGA(JadeImage_t *textureObject)
{    
	unsigned char		*imageData;									// Image Data (Up To 32 Bits)
	int					bpp;										// Image Color Depth In Bits Per Pixel.
	int					width;										// Image Width
	int					height;										// Image Height
	unsigned char		TGAheader[12] =		{0,0,2,0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header
	unsigned char		cTGAheader[12] =	{0,0,10,0,0,0,0,0,0,0,0,0};	// Compressed TGA Header
	unsigned char		TGAcompare[12];								// Used To Compare TGA Header
	unsigned char		header[6];									// First 6 Useful Bytes From The Header
	unsigned int		bytesPerPixel;								// Holds Number Of Bytes Per Pixel Used In The TGA File
	unsigned int		imageSize;									// Used To Store The Image Size When Setting Aside Ram
	int					temp;										// Temporary Variable
	
	if(textureObject == 0) {
		return 0;
	}
	
	//Texture_t textureObject;
	textureObject->width = 0;
	textureObject->height = 0;
	textureObject->bpp = 0;
	textureObject->pixels = 0;
	
	char	*file_path = KMiscTools::makeFilePath(textureObject->filename);
	FILE *file = fopen(file_path, "rb");
	if(	file==NULL )
	{
		return 0;
	}
	if( fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) )	// Are There 12 Bytes To Read?
	{
		return 0;
	}
	
	bool is_compressed = false;
	
	if(memcmp(TGAheader, &TGAcompare, sizeof(TGAcompare)) == 0)				// See if header matches the predefined header of 
	{																		// an Uncompressed TGA image
		is_compressed = false;
	}
	else if(memcmp(cTGAheader, &TGAcompare, sizeof(TGAcompare)) == 0)		// See if header matches the predefined header of
	{																		// an RLE compressed TGA image
		is_compressed = true;
	}
	else																	// If header matches neither type
	{
		//logfile->logDebug("TGA file be type 2 or type 10\n");
		return 0;
	}
	
	if(is_compressed == true)
	{
		// compressed TGA loading -------------------
		
		if(	fread(header,1,sizeof(header),file)!=sizeof(header))				// If So Read Next 6 Header Bytes
		{
			if (file == NULL)									// Did The File Even Exist? *Added Jim Strong*
				return 0;									// Return False
			else
			{
				fclose(file);									// If Anything Failed, Close The File
				return 0;									// Return False
			}
		}
		
		width	= header[1] * 256 + header[0];					// Determine The TGA Width	(highbyte*256+lowbyte)
		height	= header[3] * 256 + header[2];					// Determine The TGA Height	(highbyte*256+lowbyte)
		bpp		= header[4];										// Determine Bits Per Pixel
		
		if(	width	<=0	||								// Is The Width Less Than Or Equal To Zero
		   height	<=0	||								// Is The Height Less Than Or Equal To Zero
		   (header[4]!=24 && header[4]!=32))					// Is The TGA 24 or 32 Bit?
		{
			fclose(file);										// If Anything Failed, Close The File
			return 0;										// Return False
		}
		
		bytesPerPixel	= (bpp / 8);									// Compute BYTES per pixel
		imageSize		= (bytesPerPixel * width * height);		// Compute amout of memory needed to store image
		imageData		= (unsigned char *)malloc(imageSize);					// Allocate that much memory
		
		if(imageData == NULL)											// If it wasnt allocated correctly..
		{
			//logfile->logDebug("Could not allocate memory for image\n");
			fclose(file);
			return 0;
		}
		
		unsigned int pixelcount	= height * width;							// Nuber of pixels in the image
		unsigned int currentpixel	= 0;												// Current pixel being read
		unsigned int currentbyte	= 0;												// Current byte 
		unsigned char * colorbuffer = (unsigned char *)malloc(bytesPerPixel);			// Storage for 1 pixel
		do
		{
			unsigned char chunkheader = 0;											// Storage for "chunk" header
			if(fread(&chunkheader, sizeof(unsigned char), 1, file) == 0)				// Read in the 1 byte header
			{
				//logfile->logDebug("Could not read RLE header\n");
				if(file != NULL)												// If file is open
				{
					fclose(file);												// Close file
				}
				if(imageData != NULL)									// If there is stored image data
				{
					free(imageData);									// Delete image data
				}
				return 0;													// Return failed
			}
			if(chunkheader < 128)												// If the ehader is < 128, it means the that is the number of RAW color packets minus 1
			{																	// that follow the header
				chunkheader++;													// add 1 to get number of following color values
				for(short counter = 0; counter < chunkheader; counter++)		// Read RAW color values
				{
					if(fread(colorbuffer, 1, bytesPerPixel, file) != bytesPerPixel) // Try to read 1 pixel
					{
						//logfile->logDebug("Could not read image data\n");
						if(file != NULL)													// See if file is open
						{
							fclose(file);													// If so, close file
						}
						if(colorbuffer != NULL)												// See if colorbuffer has data in it
						{
							free(colorbuffer);												// If so, delete it
						}
						if(imageData != NULL)										// See if there is stored Image data
						{
							free(imageData);										// If so, delete it too
						}
						return 0;														// Return failed
					}
					imageData[currentbyte		] = colorbuffer[2];				    // Flip R and B vcolor values around in the process 
					imageData[currentbyte + 1	] = colorbuffer[1];
					imageData[currentbyte + 2	] = colorbuffer[0];
					if(bytesPerPixel == 4)												// if its a 32 bpp image
					{
						imageData[currentbyte + 3] = colorbuffer[3];				// copy the 4th byte
					}
					currentbyte += bytesPerPixel;										// Increase thecurrent byte by the number of bytes per pixel
					currentpixel++;															// Increase current pixel by 1
					if(currentpixel > pixelcount)											// Make sure we havent read too many pixels
					{
						//logfile->logDebug("Too many pixels read\n");
						if(file != NULL)													// If there is a file open
						{
							fclose(file);													// Close file
						}	
						if(colorbuffer != NULL)												// If there is data in colorbuffer
						{
							free(colorbuffer);												// Delete it
						}
						if(imageData != NULL)										// If there is Image data
						{
							free(imageData);										// delete it
						}
						return 0;														// Return failed
					}
				}
			}
			else																			// chunkheader > 128 RLE data, next color reapeated chunkheader - 127 times
			{
				chunkheader -= 127;															// Subteact 127 to get rid of the ID bit
				if(fread(colorbuffer, 1, bytesPerPixel, file) != bytesPerPixel)		// Attempt to read following color values
				{	
					//logfile->logDebug("Could not read from file\n");
					if(file != NULL)														// If thereis a file open
					{
						fclose(file);														// Close it
					}
					if(colorbuffer != NULL)													// If there is data in the colorbuffer
					{
						free(colorbuffer);													// delete it
					}
					if(imageData != NULL)											// If thereis image data
					{
						free(imageData);											// delete it
					}
					return 0;															// return failed
				}
				
				for(short counter = 0; counter < chunkheader; counter++)					// copy the color into the image data as many times as dictated 
				{																			// by the header
					imageData[currentbyte		] = colorbuffer[2];					// switch R and B bytes areound while copying
					imageData[currentbyte + 1	] = colorbuffer[1];
					imageData[currentbyte + 2	] = colorbuffer[0];
					if(bytesPerPixel == 4)												// If TGA images is 32 bpp
					{
						imageData[currentbyte + 3] = colorbuffer[3];				// Copy 4th byte
					}
					currentbyte += bytesPerPixel;										// Increase current byte by the number of bytes per pixel
					currentpixel++;															// Increase pixel count by 1
					if(currentpixel > pixelcount)											// Make sure we havent written too many pixels
					{
						//logfile->logDebug("Too many pixels read\n");			// if there is too many... Display an error!
						if(file != NULL)													// If there is a file open
						{
							fclose(file);													// Close file
						}	
						if(colorbuffer != NULL)												// If there is data in colorbuffer
						{
							free(colorbuffer);												// Delete it
						}
						if(imageData != NULL)										// If there is Image data
						{
							free(imageData);										// delete it
						}
						return 0;														// Return failed
					}
				}
			}
		}
		while(currentpixel < pixelcount);													// Loop while there are still pixels left
	}
	else
	{
		// uncompressed TGA loading ------------------
		
		if(	fread(header,1,sizeof(header),file)!=sizeof(header))				// If So Read Next 6 Header Bytes
		{
			if (file == NULL)									// Did The File Even Exist? *Added Jim Strong*
				return 0;									// Return False
			else
			{
				fclose(file);									// If Anything Failed, Close The File
				return 0;									// Return False
			}
		}
		
		width  = header[1] * 256 + header[0];			// Determine The TGA Width	(highbyte*256+lowbyte)
		height = header[3] * 256 + header[2];			// Determine The TGA Height	(highbyte*256+lowbyte)
		
		if(	width	<=0	||								// Is The Width Less Than Or Equal To Zero
		   height	<=0	||								// Is The Height Less Than Or Equal To Zero
		   (header[4]!=24 && header[4]!=32))					// Is The TGA 24 or 32 Bit?
		{
			fclose(file);										// If Anything Failed, Close The File
			return 0;										// Return False
		}
		
		bpp	= header[4];							// Grab The TGA's Bits Per Pixel (24 or 32)
		bytesPerPixel	= bpp/8;						// Divide By 8 To Get The Bytes Per Pixel
		imageSize		= width*height*bytesPerPixel;	// Calculate The Memory Required For The TGA Data
		
		imageData=(unsigned char *)malloc(imageSize);		// Reserve Memory To Hold The TGA Data
		
		if(	imageData==NULL ||							// Does The Storage Memory Exist?
		   fread(imageData, 1, imageSize, file)!=imageSize)	// Does The Image Size Match The Memory Reserved?
		{
			if(imageData!=NULL)						// Was Image Data Loaded
				free(imageData);						// If So, Release The Image Data
			
			fclose(file);										// Close The File
			return 0;										// Return False
		}
	/*
		for(int m=0 ; m<imageSize ; m++)
		{
			unsigned char c = imageData[m];
			printf("%d ", (int)c);
		}*/
	
		for(unsigned int i=0; i<imageSize; i+=bytesPerPixel)		// Loop Through The Image Data
		{														// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
			temp=imageData[i];							// Temporarily Store The Value At Image Data 'i'
			imageData[i] = imageData[i + 2];	// Set The 1st Byte To The Value Of The 3rd Byte
			imageData[i + 2] = temp;					// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
		}
	}
	
	fclose (file);
	
	textureObject->originalWidth = width;
	textureObject->originalHeight = height;	
	textureObject->width = width;
	textureObject->height = height;
	textureObject->bpp = bpp;
	textureObject->pixels = imageData;
	
	return 1;		
}

