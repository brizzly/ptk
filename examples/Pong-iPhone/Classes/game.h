
#import "KShader.h"
#import "KGraphic.h"
#import "KFont.h"
#import "KSound.h"
#import "KMusic.h"
#import "KInput.h"
#import "KPTK.h"
#import "KMiscTools.h"
#import "Vector.h"
#import "List.h"
#import "ball.h"
#import "Game2.h"

class game
{
private:
	KGraphic * testGraphic;     // box 1
	KGraphic * testGraphic2;    // box 2
	KGraphic * testGraphic3;    // background
	KGraphic * testGraphic4;    // box 3
	KGraphic * shapeGraphic;
	KGraphic * buttonLeft;
	KGraphic * buttonMiddle;
	KGraphic * buttonRight;
	KFont * fonte;
	KFont * fonte2;
	KFont * fonte3;
	KSound * sound1;
	KSound * sound2;
	KMusic * music1;
    
    List ballsList;
    
    Game2 * gameinstance;
	
public:
	game();
	~game();
	
	void init(int width, int height);
    void addNewBall(int px, int py);
    void addBall(char * imagename, int px, int py);
	void draw(float frameTime);
	void playSfx();
};
