//
//  ball.hpp
//  Pong
//
//  Created by Julien on 13/11/2024.
//  Copyright © 2024 Jadeware Games. All rights reserved.
//

#ifndef ball_h
#define ball_h

#include <stdio.h>
#import "List.h"
#import "vector.h"
#import "KGraphic.h"

#ifdef __ANDROID__
#include "Box2D/Box2D.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#else
#include <Box2D/Box2D.h>
#endif

#define PTM_RATIO        32.0 // box2d pixel to meter
#define EDGE_IS_GROUND    (char*)"10002"

#define CATEGORY_WALL        0x0001
#define CATEGORY_GROUND        0x0002
#define CATEGORY_BLOCK        0x0004
#define CATEGORY_BLOCKER    0x0008


class ball : Node
{
private:

    int gameWidth;
    int gameHeight;
    int screenWidth;
    int screenHeight;
    
    b2Body * box2d_body;
    b2BodyDef ballBodyDef;
    b2Fixture * fixture;
    b2World * _world;
    b2ContactListener * contactListener;
    float density=1.0f;
    float * vertices;
    int numberOfVertices;
    
public:
    
    //Vector * pos;
    Vector * vel;
    Vector * siz;
	
	KGraphic * kg;
    
public:
    ball(KGraphic * graphic, int posx, int posy, float vx, float vy, int w, int h, int game_w, int game_h, int screen_w, int screen_h);
    ~ball();
    
    Vector getPos();
    void editFixture(float density, float friction, float restitution);
    void update(float frametime);
};

#endif /* ball_h */
