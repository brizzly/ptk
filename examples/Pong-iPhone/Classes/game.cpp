
#include "game.h"

game::game()
{
}

void game::init(int width, int height)
{
	KPTK::_screenW = width;
	KPTK::_screenH = height;
	//KPTK::_screenOrientation = K_320x568;
	KPTK::_screenOrientation = K_960x1440;
		
	int gameW = KPTK::getGameW();
	int gameH = KPTK::getGameH();
	int screenW = width;
	int screenH = height;
	
	KMiscTools::initMiscTools();
	
	
	testGraphic = new KGraphic(gameW, gameH, screenW, screenH);
	testGraphic->loadPicture("box.png");
	
	testGraphic2 = new KGraphic(gameW, gameH, screenW, screenH);
	testGraphic2->loadPicture("box2.png");
	
	testGraphic3 = new KGraphic(gameW, gameH, screenW, screenH);
	//testGraphic3->loadPicture("960_1440.png");
	testGraphic3->loadPicture("960_1440_Retina.png");

	testGraphic4 = new KGraphic(gameW, gameH, screenW, screenH);
	testGraphic4->loadPicture("box512.png");
	
	shapeGraphic = new KGraphic(gameW, gameH, screenW, screenH);
	
	buttonLeft = new KGraphic(gameW, gameH, screenW, screenH);
	buttonLeft->loadPicture("border2_left.png");

	buttonMiddle = new KGraphic(gameW, gameH, screenW, screenH);
	buttonMiddle->loadPicture("border2_middle.png");
	
	buttonRight = new KGraphic(gameW, gameH, screenW, screenH);
	buttonRight->loadPicture("border2_right.png");
	

	fonte = new KFont(KMiscTools::makeFilePath("neue.ttf"), gameW, gameH);
	fonte->SetTextColor(0, 0, 1.0);
	fonte->SetBackgroundColor(1, 0, 0, 0.8f);
	
	fonte2 = new KFont(KMiscTools::makeFilePath("pixel.ttf"), gameW, gameH);
	fonte2->SetBackgroundColor(1, 0, 0, 0.8f);
	
	
	fonte3 = new KFont(KMiscTools::makeFilePath("chinese.ttf"), gameW, gameH);
	fonte3->SetTextColor(1, 0, 0);
	
	// Ensure OpenALManager is initialized
	OpenALManager& alManager = OpenALManager::getInstance();

	sound1 = new KSound;
	sound1->loadSample(KMiscTools::makeFilePath("boing2.caf"));
	sound1->setVolume(1.0);
	
	sound2 = new KSound;
	sound2->loadSample(KMiscTools::makeFilePath("jump1.caf"));
	sound2->setVolume(1.0);
	
	music1 = new KMusic();
	music1->load(KMiscTools::makeFilePath("menu.mp3"));
 //   music1->playMusic();
    
    
    for(int i=0 ; i<1 ; i++) {
        
        int px = rand() % (KPTK::_screenW - 256);
        int py = rand() % (KPTK::_screenH - 256);
        addNewBall(px, py);
    }
    
    gameinstance = new Game2();
    
}

void game::addNewBall(int px, int py)
{
    if(rand() % 2 == 0) {
        addBall("box.png", px, py);
    }
    else {
        addBall("box2.png", px, py);
    }
}

void game::addBall(char * imagename, int px, int py)
{
    float vx = 2 * (float) ( ((rand() % 200) - 100) / 100.0f );
    float vy = 2 * (float) ( ((rand() % 200) - 100) / 100.0f );
    
    vx = vy = 0;

    int game_w = KPTK::getGameW();
    int game_h = KPTK::getGameH();
    int screen_w = KPTK::_screenW;
    int screen_h = KPTK::_screenH;

    KGraphic * kg = new KGraphic(game_w, game_h, screen_w, screen_h);
    kg->loadPicture(imagename);
    
    float tw = kg->getTextureWidth(); // 256 pixels
    float th = kg->getTextureHeight();
    float tsw = kg->getTextureSizeW(); // 128 retina pixels
    float tsh = kg->getTextureSizeH();
    printf("tw=%f, th=%f, tsw=%f, tsh=%f\n", tw, th, tsw, tsh);

    ball * b = new ball(kg, px, py, vx, vy, tsw, tsh, game_w, game_h, screen_w, screen_h);
    ballsList.Append(b);
}

void game::draw(float frameTime)
{
    int screenWidth = KPTK::_screenW;
    int screenHeight = KPTK::_screenH;
    
    
	static float r = 0.0;
	static float z = 0.0;
	static float a = 0.0;
	

	r += (frameTime / 1000.0f) * 0.001f;
	a += (frameTime / 1000.0f) * 0.001f;
	z = 1.0 + cos(a * M_PI / 180.0f); // (frameTime / 1000.0f)

	static float debugval = 0.0;
	debugval += 0.4f;
	

    // Ball logic
	
    for(int i=0 ; i<ballsList.count() ; i++) {
     
        ball * b = (ball*) ballsList.get(i);
        b->update(frameTime);
    }

   
	// FULL SCREEN IMAGE : 960x1440
	//testGraphic3->blit(0, 0, 960, 1440, 0, 0);
	
   
	// SQUARE 256x256
    
    for(int i=0 ; i<ballsList.count() ; i++) {
        ball * b = (ball*) ballsList.get(i);
		b->kg->setDrawBounds(true);
        b->kg->draw(b->getPos().x, b->getPos().y, (i%2) ? 2*a : -a*10, 1, 1);
    }
    
    gameinstance->Run(frameTime);
}

void game::playSfx()
{
	if(rand() % 100 < 50) {
		sound1->playSample();
	}
	else {
		sound2->playSample();
	}
}

game::~game()
{
	
}
