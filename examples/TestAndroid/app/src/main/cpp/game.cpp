
#include "game.h"

game::game()
{
}

void game::init(int width, int height, AAssetManager* assetManager)
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
	
	
	testGraphic = new KGraphic(gameW, gameH, screenW, screenH, assetManager);
	testGraphic->loadPicture("box.png");
	
	testGraphic2 = new KGraphic(gameW, gameH, screenW, screenH, assetManager);
	testGraphic2->loadPicture("box2.png");
	
	testGraphic3 = new KGraphic(gameW, gameH, screenW, screenH, assetManager);
	//testGraphic3->loadPicture("960_1440.png");
	testGraphic3->loadPicture("960_1440_Retina.png");

	testGraphic4 = new KGraphic(gameW, gameH, screenW, screenH, assetManager);
	testGraphic4->loadPicture("box512.png");

    testGraphic5 = new KGraphic(gameW, gameH, screenW, screenH, assetManager);
    testGraphic5->loadPicture("title_1@2x.png");
	
	shapeGraphic = new KGraphic(gameW, gameH, screenW, screenH, assetManager);
    shapeGraphic2 = new KGraphic(gameW, gameH, screenW, screenH, assetManager);
    shapeGraphic3 = new KGraphic(gameW, gameH, screenW, screenH, assetManager);
	
	buttonLeft = new KGraphic(gameW, gameH, screenW, screenH, assetManager);
	buttonLeft->loadPicture("border2_left.png");

	buttonMiddle = new KGraphic(gameW, gameH, screenW, screenH, assetManager);
	buttonMiddle->loadPicture("border2_middle.png");
	
	buttonRight = new KGraphic(gameW, gameH, screenW, screenH, assetManager);
	buttonRight->loadPicture("border2_right.png");

#ifdef __ANDROID__
    fonte = new KFont(KMiscTools::makeFilePath("neue.ttf"), gameW, gameH, assetManager);
    fonte->SetTextColor(0, 0, 1.0);
    fonte->SetBackgroundColor(1, 0, 0, 0.8f);

    fonte2 = new KFont(KMiscTools::makeFilePath("pixel.ttf"), gameW, gameH, assetManager);
    fonte2->SetBackgroundColor(1, 0, 0, 0.8f);


    fonte3 = new KFont(KMiscTools::makeFilePath("chinese.ttf"), gameW, gameH, assetManager);
    fonte3->SetTextColor(1, 0, 0);
#else
	fonte = new KFont(KMiscTools::makeFilePath("neue.ttf"), gameW, gameH);
	fonte->SetTextColor(0, 0, 1.0);
	fonte->SetBackgroundColor(1, 0, 0, 0.8f);
	
	fonte2 = new KFont(KMiscTools::makeFilePath("pixel.ttf"), gameW, gameH);
	fonte2->SetBackgroundColor(1, 0, 0, 0.8f);
	
	
	fonte3 = new KFont(KMiscTools::makeFilePath("chinese.ttf"), gameW, gameH);
	fonte3->SetTextColor(1, 0, 0);
#endif

	// Ensure OpenALManager is initialized
	OpenALManager& alManager = OpenALManager::getInstance();

	sound1 = new KSound;
	sound1->loadSample(KMiscTools::makeFilePath("boing2.caf"), assetManager);
	sound1->setVolume(1.0);
	
	sound2 = new KSound;
	sound2->loadSample(KMiscTools::makeFilePath("jump1.caf"), assetManager);
	sound2->setVolume(1.0);
	
	music1 = new KMusic();
	music1->load(KMiscTools::makeFilePath("menu.mp3"), assetManager);
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
    float b = 0.85f;
    

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

void game::draw_scene3(float frameTime)
{
    static float z = 1.0;
    static float a = 0.0;
    
    //r += (frameTime / 1000.0f) * 0.001f;
    a += (frameTime / 1000.0f) * 0.001f;
    z = 1.0 + cos(a * M_PI / 180.0f); // (frameTime / 1000.0f)
    
    
    int screenWidth = KPTK::_screenW;
    int screenHeight = KPTK::_screenH;
    
    // LINES
    
    float line_R = 0.0f;
    float line_G = 1.0;
    float line_B = 0.0;
    float line_A = 1.0f;
    float line_W = 6.0;

    float pX = 0 + 50;
    float pY = 0 + 50;
    float w1 = 512;
    float h1 = 512;
    
    
    float square_x1[4] = {pX, pX+w1, pX+w1, pX};
    float square_y1[4] = {pY, pY, pY+h1, pY+h1};
    
    float square_x2[4] = {pX+w1, pX+w1, pX, pX};
    float square_y2[4] = {pY, pY+h1, pY+h1, pY};
    
    shapeGraphic->angle = a;
    shapeGraphic->line_centerX = w1/2 + pX;
    shapeGraphic->line_centerY = h1/2 + pY;

    shapeGraphic->drawLine(square_x1[0], square_y1[0], square_x2[0], square_y2[0], 1, 1, 1, line_A, line_W);
    shapeGraphic->drawLine(square_x1[1], square_y1[1], square_x2[1], square_y2[1], 1, 1, 1, line_A, line_W);
    shapeGraphic->drawLine(square_x1[2], square_y1[2], square_x2[2], square_y2[2], 1, 1, 1, line_A, line_W);
    shapeGraphic->drawLine(square_x1[3], square_y1[3], square_x2[3], square_y2[3], 1, 1, 1, line_A, line_W);

    
    int cube_x = 200;
    int cube_y = 320;
    
    vec2 vertice[] = {
        {pX, pY},
        {pX+w1, pY},
        {pX+w1, pY+h1},
        {pX, pY+h1}
    };
    
    shapeGraphic2->shape_centerX = w1/2 + pX;
    shapeGraphic2->shape_centerY = h1/2 + pY;
    shapeGraphic2->angle = a;
    shapeGraphic2->drawShape(4, vertice, cube_x, cube_y, line_W, line_R, line_G, line_B, line_A);
    
    
    
    // Simple lines
    float ylimit = KPTK::getGameH()/2-100;
    shapeGraphic3->drawLine(40, 100+z*ylimit, screenWidth-40, 100+z*ylimit, 1, 0, 0, 1.0f, z*10.0f);
    shapeGraphic3->drawLine(40, 100+z*ylimit, screenWidth-40, 200, 0, 0, 1, 1.0f, 6.0);
    shapeGraphic3->drawLine(screenWidth-40, 200, screenWidth-40, 100+z*ylimit, 1, 0.5f, 0, 1.0f, z*10.0f);
  
    
    // BUTTON
    
    buttonLeft->drawEx(0, 100, 20, 49, 0, 0);
    buttonMiddle->drawEx(0, 100, 160, 49, 20, 0);
    buttonRight->drawEx(0, 100, 20, 49, 20+160, 0);
    

    
    // SQUARE 256x256
    testGraphic2->setDrawBounds(true);
    testGraphic2->drawEx(0, 0, 256, 256, 420, 850, 360-a, z);

    
    
    
    wchar_t * txt = L"This is so cool. COOL VERY COOL and yes this is a very long text actually.";
    float yfontpos = 0;
    
//    fonte->RenderText(txt, 0, +100+42, 100);

    fonte->RenderText(txt, 0, 0, 100);
    fonte->RenderText(txt, 0, 100, 50);
    fonte->RenderText(txt, 0, 200+z*ylimit, 100);
    fonte->RenderText(txt, 0, 300+z*ylimit, 50);
}

void game::draw_scene4(float frameTime)
{
	static float z = 1.0;
	static float a = 0.0;
	
	//r += (frameTime / 1000.0f) * 0.001f;
//	a += (frameTime / 1000.0f) * 0.001f;
	a += (frameTime / 1000.0f) * 0.02f;
	z = 1.0 + cos(a * M_PI / 180.0f); // (frameTime / 1000.0f)
	
	
	//wchar_t * txt = L"Le Lorem Ipsum est simplement du faux texte employé dans la composition et la mise en page avant impression. Le Lorem Ipsum est le faux texte standard de l'imprimerie depuis les années 1500, quand un imprimeur anonyme assembla ensemble des morceaux de texte pour réaliser un livre spécimen de polices de texte. Il n'a pas fait que survivre cinq siècles, mais s'est aussi adapté à la bureautique informatique, sans que son contenu n'en soit modifié.";
	wchar_t * txt = L"Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";
	float yfontpos = 0;
	
	fonte->SetTextColor(1, 1, 1);
	fonte->SetBackgroundColor(0, 0, 0, 0);
	fonte->SetMaxCharBeforeLine(960-10);
	fonte->RenderText(txt, 0, 0, 24 + z*20);

    testGraphic5->draw(0, 0, 0, 1, 1);
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
    

    draw_scene0(frameTime);
    //draw_scene1(frameTime);
    draw_scene2(frameTime);
    draw_scene3(frameTime);
	draw_scene4(frameTime);


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
