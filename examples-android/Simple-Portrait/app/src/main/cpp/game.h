
#include "KShader.h"
#include "KGraphic.h"
#include "KFont.h"
#include "KSound.h"
#include "KMusic.h"
#include "KInput.h"
#include "KPTK.h"
#include "KMiscTools.h"

#ifdef __ANDROID__
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#endif



class game
{
private:
#ifdef __ANDROID__
    AAssetManager   * assetManager_;
#endif

	KGraphic * testGraphic;     // box 1
	KGraphic * testGraphic2;    // box 2
	KGraphic * testGraphic3;    // background
	KGraphic * testGraphic4;    // box 3
    KGraphic * testGraphic5;    // retina image
    KGraphic * testGraphic6;
	KGraphic * shapeGraphic;
    KGraphic * shapeGraphic2;
    KGraphic * shapeGraphic3;
	KGraphic * buttonLeft;
	KGraphic * buttonMiddle;
	KGraphic * buttonRight;
	KFont * fonte;
	KFont * fonte2;
	KFont * fonte3;
	KSound * sound1;
	KSound * sound2;
	KMusic * music1;
	
public:
	game();
	~game();

#ifdef __ANDROID__
    void                setAssetManager(AAssetManager * value);
#endif

	void init(int width, int height, AAssetManager* assetManager);
	void draw(float frameTime);
    void draw_scene0(float frameTime);
    void draw_scene1(float frameTime);
    void draw_scene2(float frameTime);
    void draw_scene3(float frameTime);
	void draw_scene4(float frameTime);
	void playSfx();
};
