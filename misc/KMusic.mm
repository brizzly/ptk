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

#include "KMusic.h"

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import <MediaPlayer/MediaPlayer.h>

AVAudioPlayer * audioPlayer = NULL;

KMusic::KMusic()
{
	_started = false;
	
	/*
	UInt32 dataSize = sizeof(Float32);
	AudioSessionGetProperty (
							 kAudioSessionProperty_CurrentHardwareOutputVolume,
							 &dataSize,
							 &volume
							 );
	printf("volume=%f\n", volume);*/
	
	
	MPMusicPlayerController * iPod = [MPMusicPlayerController iPodMusicPlayer];
    _device_volume = iPod.volume;
	//printf("volume=%f\n", _device_volume);
	
	//_device_volume = [MPMusicPlayerController applicationMusicPlayer].volume;
	//printf("volume=%f\n", _device_volume);

	if(_device_volume < 0) {
		_device_volume = 0.5f;
	}
}

KMusic::~KMusic()
{
}

bool KMusic::isMusicEnded()
{
	if(audioPlayer == NULL) {
		return false;
	}
	if(_started == false) {
		return false;
	}
	//printf("audio %d %f / %f\n", (int)audioPlayer.playing, audioPlayer.currentTime, audioPlayer.duration);
	
	//if(audioPlayer.playing && audioPlayer.currentTime >= audioPlayer.duration) {
	if(audioPlayer.playing == false && audioPlayer.currentTime == 0 && audioPlayer.duration > 0)
	{
		if(audioPlayer.numberOfLoops == -1) {
			return false;
		}
		stopMusic();
		return true;
	}
	return false;
}

bool KMusic::iPodPlaying()
{
    if ([[MPMusicPlayerController systemMusicPlayer] playbackState] == MPMusicPlaybackStatePlaying) {
		return true;
	}
	return false;
}

bool KMusic::playMusic(char * filename, bool loop)
{
	if(iPodPlaying() == true) {
		return false;
	}
	if(audioPlayer != NULL) {
		return false;
	}
	NSString * fileStr = [NSString stringWithFormat:@"%s", filename];
	NSURL * url = [NSURL fileURLWithPath:fileStr];
	NSError * error;
	audioPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:url error:&error];
	if(!audioPlayer) {
		NSLog(@"%@", [error description]);
		return false;
	}
	//[audioPlayer setDelegate:this];
	audioPlayer.numberOfLoops = loop ? -1 : 0;
	//setVolume(9*_device_volume);
//	setVolume(0);
	[audioPlayer play];
	if(audioPlayer.isPlaying == true) {
		_started = true;
	}
	return true;
}

void KMusic::stopMusic()
{
	_started = false;
	if(audioPlayer) {
		if([audioPlayer isPlaying] == YES) {
			[audioPlayer stop];
		}
		audioPlayer = nil;
	}
}

void KMusic::pauseMusic()
{
	if(audioPlayer && [audioPlayer isPlaying] == YES) {
		[audioPlayer pause];
	}
}

void KMusic::resumeMusic()
{
	if(audioPlayer && [audioPlayer isPlaying] == NO) {
		[audioPlayer play];
	}
}

int KMusic::getVolume()
{
	return _device_volume * 100;
}

void KMusic::setVolume(int volume)
{
	MPMusicPlayerController * iPod = [MPMusicPlayerController iPodMusicPlayer];
    _device_volume = iPod.volume;
	
	float volume2 = (float)volume * 0.25f;

	if(audioPlayer) {
		audioPlayer.volume = (volume2 / 100.0f) * _device_volume;
	}
}



