
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
    KGraphic * testGraphic;     // boite 1
    KGraphic * testGraphic2;    // boite 2
    KGraphic * testGraphic3;    //
    KGraphic * testGraphic4;    //
    KGraphic * testGraphic5;    //
    KGraphic * testGraphic6;    //
    KGraphic * testGraphic7;    //
	KFont * fonte;
	KFont * fonte2;
	KFont * fonte3;
    KSound * sound1;
    KMusic * music1;
	
public:
	game();
	~game();
	
	void init(int width, int height);
	void draw(float frameTime);
    void draw_scene0(float frameTime);
	void playSfx();
};
