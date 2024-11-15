//
//  ball.cpp
//  Pong
//
//  Created by Julien on 13/11/2024.
//  Copyright © 2024 Jadeware Games. All rights reserved.
//

#include "ball.h"


ball::ball(int posx, int posy, float vx, float vy, int w, int h, int game_w, int game_h, int screen_w, int screen_h)
{
    screenWidth = screen_w;
    screenHeight = screen_h;
    
    pos = new Vector((float)posx, (float)posy);
    vel = new Vector(vx, vy);
    siz = new Vector(w, h);
	
	kg = new KGraphic(game_w, game_h, screen_w, screen_h);
	kg->loadPicture("box.png");

}

ball::~ball()
{
}

void ball::update(float frametime)
{
    float ballVel = 0.005f;

    Vector v = (*vel) * (ballVel * frametime * 0.001f);

    *pos += v;
    
    // Handle horizontal screen boundaries
    if (vel->x >= 0) {
        if (pos->x + siz->x > screenWidth) {
            vel->x *= -1; // Reverse direction
        }
    } else {
        if (pos->x <= 0) {
            vel->x *= -1; // Reverse direction
        }
    }

    if (vel->y >= 0) {
        if (pos->y + siz->y > screenHeight) {
            vel->y *= -1; // Reverse direction
        }
    } else {
        if (pos->y <= 0) {
            vel->y *= -1; // Reverse direction
        }
    }
}
