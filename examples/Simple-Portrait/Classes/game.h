
#import "KShader.h"
#import "KGraphic.h"
#import "KFont.h"
#import "KSound.h"
#import "KMusic.h"
#import "KInput.h"
#import "KPTK.h"
#import "KMiscTools.h"

class game
{
private:
	KGraphic * testGraphic;     // box 1
	KGraphic * testGraphic2;    // box 2
	KGraphic * testGraphic3;    // background
	KGraphic * testGraphic4;    // box 3
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
	
	void init(int width, int height);
	void draw(float frameTime);
    void draw_scene0(float frameTime);
    void draw_scene1(float frameTime);
    void draw_scene2(float frameTime);
    void draw_scene3(float frameTime);
	void playSfx();
};
