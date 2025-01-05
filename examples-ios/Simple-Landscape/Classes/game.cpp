
#include "game.h"

game::game()
{
}

void game::init(int width, int height)
{
	KPTK::_screenW = width;
	KPTK::_screenH = height;
	KPTK::_screenOrientation = K_640x480;
		
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
    testGraphic3->loadPicture("1704_960.png");
    
    testGraphic4 = new KGraphic(gameW, gameH, screenW, screenH);
    testGraphic4->loadPicture("852_480.png");
    
    testGraphic5 = new KGraphic(gameW, gameH, screenW, screenH);
    testGraphic5->loadPicture("world1.png");
    
    testGraphic6 = new KGraphic(gameW, gameH, screenW, screenH);
    testGraphic6->loadPicture("box.png");
    
    testGraphic7 = new KGraphic(gameW, gameH, screenW, screenH);
    testGraphic7->loadPicture("640_480.png");
    
    
    sound1 = new KSound;
    sound1->loadSample(KMiscTools::makeFilePath("boing2.caf"));
    sound1->setVolume(50);
    
    music1 = new KMusic();
    music1->load(KMiscTools::makeFilePath("menu.mp3"));
    music1->playMusic();
	
	// Ensure OpenALManager is initialized
	OpenALManager& alManager = OpenALManager::getInstance();
}

void game::draw_scene0(float frameTime)
{
    static float r = 0.0;
    static float z = 0.0;
    static float a = 0.0;
    
    r += (frameTime / 1000.0f) * 0.0005f;
    a += (frameTime / 1000.0f) * 0.0005f;
    z = 1.0 + cos(a * M_PI / 180.0f); // (frameTime / 1000.0f)

    static float debugval = 0.0;
    debugval += 0.4f;

    
    // FULL SCREEN IMAGE : 640x480

    testGraphic7->drawEx(0, 0, 640, 480, 0, 0, 0, 1.0, 1.0);
    
    
    // SQUARE 256x256
    
    testGraphic6->drawEx(0, 0, 256, 256, 128, 128, a, z, 1.0);
    
    
    // SQUARE 256x256
    
    testGraphic->drawEx(0, 0, 256, 256, 10, 10, 0, 1.0, 1.0);
    

     // SQUARE 256x256
     
    testGraphic2->drawEx(0, 0, 256, 256, 200, 100, 360-a, 1.0, 1.0);
}

void game::draw(float frameTime)
{
    draw_scene0(frameTime);
}

void game::playSfx()
{
    sound1->playSample();
}

game::~game()
{
	
}
