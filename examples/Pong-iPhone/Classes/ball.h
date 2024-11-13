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

class ball : Node
{
private:
    
    int screenWidth;
    int screenHeight;
    
public:
    
    Vector * pos;
    Vector * vel;
    Vector * siz;
    
public:
    ball(int posx, int posy, float vx, float vy, int w, int h, int screen_w, int screen_h);
    ~ball();
    
    void update(float frametime);
};

#endif /* ball_h */
