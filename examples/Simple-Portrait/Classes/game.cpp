
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

void game::draw_scene0(float frameTime)
{
    static float r = 0.0;
    static float z = 1.0;
    static float a = 0.0;
    
    int sw = KPTK::_screenW;
    int sh = KPTK::_screenH;

    // (int x1, int y1, int x2, int y2, int destX, int destY, float angle, float zoom, float blend , bool flipx , bool flipy )
    
    // FULL SCREEN IMAGE : 960x1440
    testGraphic3->drawEx(0, 0, sw, sh, 0, 0);
}

void game::draw_scene1(float frameTime)
{
//    static float r = 0.0;
//    static float z = 1.0;
    static float a = 0.0;
    
    //r += (frameTime / 1000.0f) * 0.001f;
    a += (frameTime / 1000.0f) * 0.001f;
    //z = 1;//1.0 + cos(a * M_PI / 180.0f); // (frameTime / 1000.0f)
    
    float x = 0;
    float y = 0;
    //float a = 0;
    float z = 1;
    float b = 0.5f;
    

    // FULL SCREEN IMAGE : 960x1440
    testGraphic3->draw(0, 0, 0, 1, 1);
     
    // SQUARE 512x512
    testGraphic4->setDrawBounds(true);
    testGraphic4->draw(362, 353, 360-a, 1.0f, 0.9f);
    
    // SQUARE 512x512
  //  testGraphic4->setDrawBounds(true);
  //  testGraphic4->blit(0, 0, 512, 512, 362, 353, 0/*10*/, 1.0f, 0.5f);
    
    //testGraphic->setDrawBounds(true);
    //testGraphic->blit(0, 0, 512, 512, 362, 353, 0/*10*/, 1.0f, 0.5f);
    //testGraphic->blit(0, 0, testGraphic->getTextureSizeW(), testGraphic->getTextureSizeH(), 0, 0, 0/*10*/, 1.0f, 0.5f);
    
    // square 256x256
    testGraphic->setDrawBounds(true);
    testGraphic->draw(x, y, a, z, b);
}

void game::draw_scene2(float frameTime)
{
    //    static float r = 0.0;
    static float z = 1.0;
    static float a = 0.0;
    
    //r += (frameTime / 1000.0f) * 0.001f;
    a += (frameTime / 1000.0f) * 0.001f;
    z = 1.0 + cos(a * M_PI / 180.0f); // (frameTime / 1000.0f)
    
    float x = 0;
    float y = 0;
    //float a = 0;
    //float z = 1;
    float b = 0.5f;
    

    // FULL SCREEN IMAGE : 960x1440
    testGraphic3->draw(0, 0, 0, 1, 1);
     
    // SQUARE 512x512
    testGraphic4->setDrawBounds(true);
    testGraphic4->draw(362, 353, 360-a, z, 0.9f);
    
    // SQUARE 512x512
  //  testGraphic4->setDrawBounds(true);
  //  testGraphic4->blit(0, 0, 512, 512, 362, 353, 0/*10*/, 1.0f, 0.5f);
    
    //testGraphic->setDrawBounds(true);
    //testGraphic->blit(0, 0, 512, 512, 362, 353, 0/*10*/, 1.0f, 0.5f);
    //testGraphic->blit(0, 0, testGraphic->getTextureSizeW(), testGraphic->getTextureSizeH(), 0, 0, 0/*10*/, 1.0f, 0.5f);
    
    // square 256x256
    testGraphic->setDrawBounds(true);
    testGraphic->draw(x, y, a, z, b);
}

void game::draw(float frameTime)
{
	static float r = 0.0;
	static float z = 0.0;
	static float a = 0.0;
	

	r += (frameTime / 1000.0f) * 0.001f;
	a += (frameTime / 1000.0f) * 0.001f;
    z = 1;//1.0 + cos(a * M_PI / 180.0f); // (frameTime / 1000.0f)

	static float debugval = 0.0;
	debugval += 0.4f;
	
	
	int screenWidth = KPTK::_screenW;
	int screenHeight = KPTK::_screenH;
    
    //draw_scene0(frameTime);
    //draw_scene1(frameTime);
    draw_scene2(frameTime);
    return;

   
	// FULL SCREEN IMAGE : 960x1440
	testGraphic3->drawEx(0, 0, 960, 1440, 0, 0);
	
   
	// SQUARE 256x256
    testGraphic->setDrawBounds(true);
	testGraphic->drawEx(0, 0, 256, 256, 140, 80, a, z);
	 
	 
	// SQUARE 512x512
	testGraphic4->drawEx(0, 0, 512, 512, 362, 353, 10, 1.0f, 0.5f);
	
	
	// LINES
	
	float line_R = 0.0f;
	float line_G = 1.0;
	float line_B = 0.0;
	float line_A = 1.0f;
	float line_W = 6.0;

    float pX = 0;
    float pY = 0;
    float w1 = 512; //screenWidth/2;
    float h1 = 512; //screenHeight/2;
    
    /*
    float square_x1[4] = {pX, pX+w1, pX+w1, pX};
    float square_y1[4] = {pY, pY, pY+h1, pY+h1};
    
    float square_x2[4] = {pX+w1, pX+w1, pX, pX};
    float square_y2[4] = {pY, pY+h1, pY+h1, pY};

    shapeGraphic->drawLine(square_x1[0], square_y1[0], square_x2[0], square_y2[0], line_R, line_G, line_B, line_A, line_W);
    shapeGraphic->drawLine(square_x1[1], square_y1[1], square_x2[1], square_y2[1], line_R, line_G, line_B, line_A, line_W);
    shapeGraphic->drawLine(square_x1[2], square_y1[2], square_x2[2], square_y2[2], line_R, line_G, line_B, line_A, line_W);
    shapeGraphic->drawLine(square_x1[3], square_y1[3], square_x2[3], square_y2[3], line_R, line_G, line_B, line_A, line_W);
*/
    
    int cube_x = screenWidth/2; //10;//362;
    int cube_y = screenHeight/2; //10;//353;
    
    vec2 vertice[] = {
        {pX, pY},
        {pX+w1, pY},
        {pX+w1, pY+h1},
        {pX, pY+h1}
    };
    
//    vec2 center;
//    center[0] = 256;
//    center[1] = 256;
//    for(int i=0 ; i<4 ; i++) {
//        center[0] += vertice[i][0];
//        center[1] += vertice[i][1];
//    }

    
    //testGraphic4->shape_centerX = center[0];
    //testGraphic4->shape_centerY = center[1];
    //testGraphic4->angle = a;
    testGraphic4->blitShape(4, vertice, cube_x, cube_y, line_W, line_R, line_G, line_B, line_A);
    
    
    /*

    testGraphic4->blit(0, 0, 512, 512, cube_x, cube_y, 10, 1.0f, 0.5f);
    for(int i=0 ; i<4 ; i++)
    {
        float x1 = square_x1[i];
        float y1 = square_y1[i];
        float x2 = square_x2[i];
        float y2 = square_y2[i];
        
        x1 += cube_x;
        y1 += cube_y;
        
        x2 += cube_x;
        y2 += cube_y;
        
        testGraphic4->drawLine(x1, y1, x2, y2, line_R, line_G, line_B, line_A, line_W);
    }
    */
    
    

	shapeGraphic->drawLine(0, 80, screenWidth, 80, 1, 1, 1, 1, 1.0);
	shapeGraphic->drawLine(0, 100, screenWidth, 100, 0, 1, 0, 0.5, 10.0);
    
    shapeGraphic->angle = a;
	
	
	// BUTTON
	
	buttonLeft->drawEx(0, 800, 20, 49, 0, 0);
	buttonMiddle->drawEx(0, 800, 160, 49, 20, 0);
	buttonRight->drawEx(0, 800, 20, 49, 20+160, 0);
	

	
	// SQUARE 256x256
    testGraphic2->setDrawBounds(true);
    testGraphic2->drawEx(0, 0, 256, 256, 420, 850, 360-a, z);

	
	
	
	wchar_t * txt = L"This is so cool. COOL Bis";
	float yfontpos = 0;
	
//    fonte->RenderText(txt, 0, +100+42, 100);
	fonte->RenderText(txt, 0, 0, 100);
	fonte->RenderText(txt, 0, 100, 50);
	fonte->RenderText(txt, 0, 200, 100);
	fonte->RenderText(txt, 0, 300, 50);
	
	

	
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
