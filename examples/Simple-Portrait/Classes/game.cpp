
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
}

void game::draw(float frameTime)
{
	static float r = 0.0;
	static float z = 0.0;
	static float a = 0.0;
	

	r += (frameTime / 1000.0f) * 0.001f;
	a += (frameTime / 1000.0f) * 0.001f;
	z = 1.0 + cos(a * M_PI / 180.0f); // (frameTime / 1000.0f)

	static float debugval = 0.0;
	debugval += 0.4f;
	
	
	int screenWidth = KPTK::_screenW;
	int screenHeight = KPTK::_screenH;

   
	// FULL SCREEN IMAGE : 960x1440
	testGraphic3->blit(0, 0, 960, 1440, 0, 0);
	
   
	// SQUARE 256x256
	testGraphic->blit(0, 0, 256, 256, 140, 80, a, z);
	 
	 
	// SQUARE 512x512
	testGraphic4->blit(0, 0, 512, 512, 362, 353, 10, 1.0f, 0.5f);
	
	
	// LINES
	
	float line_R = 1.0f;
	float line_G = 0;
	float line_B = 0;
	float line_A = 0.75f;
	float line_W = 5.0;
	
	float pX = 20;
	float pY = 20;
	float w1 = screenWidth/2;
	float h1 = screenHeight/2;
	shapeGraphic->drawLine(pX, pY, pX+w1, pY, line_R, line_G, line_B, line_A, line_W);
	shapeGraphic->drawLine(pX+w1, pY, pX+w1, pY+h1, line_R, line_G, line_B, line_A, line_W);
	shapeGraphic->drawLine(pX+w1, pY+h1, pX, pY+h1, line_R, line_G, line_B, line_A, line_W);
	shapeGraphic->drawLine(pX, pY+h1, pX, pY, line_R, line_G, line_B, line_A, line_W);

	shapeGraphic->drawLine(0, 80, screenWidth, 80, 1, 1, 1, 1, 1.0);
	shapeGraphic->drawLine(0, 100, screenWidth, 100, 0, 1, 0, 0.5, 10.0);
	
	
	// BUTTON
	
	buttonLeft->blit(0, 800, 20, 49, 0, 0);
	buttonMiddle->blit(0, 800, 160, 49, 20, 0);
	buttonRight->blit(0, 800, 20, 49, 20+160, 0);
	

	
	// SQUARE 256x256
   testGraphic2->blit(0, 0, 256, 256, 420, 850, 360-a, z);

	
	
	
	wchar_t * txt = L"This is so cool. COOL Bis";
	float yfontpos = 0;
	
//    fonte->RenderText(txt, 0, +100+42, 100);
	fonte->RenderText(txt, 0, 0, 100);
	fonte->RenderText(txt, 0, 100, 50);
	fonte->RenderText(txt, 0, 200, 100);
	fonte->RenderText(txt, 0, 300, 50);
	
	
/*
	fonte->RenderText(txt, 10, yfontpos, 6);   yfontpos += 40;
	fonte->RenderText(txt, 10, yfontpos, 12);    yfontpos += 60;
	fonte->RenderText(txt, 10, yfontpos, 24);    yfontpos += 100;
	fonte->RenderText(txt, 10, yfontpos, 48);    yfontpos += 130;
	fonte->RenderText(txt, 10, yfontpos, 72);    yfontpos += 110;

	fonte2->RenderText(txt, 10, yfontpos, 6);   yfontpos += 40;
	fonte2->RenderText(txt, 10, yfontpos, 12);    yfontpos += 60;
	fonte2->RenderText(txt, 10, yfontpos, 24);    yfontpos += 100;
	fonte2->RenderText(txt, 10, yfontpos, 48);    yfontpos += 130;
	fonte2->RenderText(txt, 10, yfontpos, 72);    yfontpos += 110;
	
	fonte3->RenderText(txt, 10, yfontpos, 6);   yfontpos += 40;
	fonte3->RenderText(txt, 10, yfontpos, 12);    yfontpos += 60;
	fonte3->RenderText(txt, 10, yfontpos, 24);    yfontpos += 100;
	fonte3->RenderText(txt, 10, yfontpos, 48);    yfontpos += 130;
	fonte3->RenderText(txt, 10, yfontpos, 72);    yfontpos += 110;
*/
	
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
