//
//  ball.cpp
//  Pong
//
//  Created by Julien on 13/11/2024.
//  Copyright © 2024 Jadeware Games. All rights reserved.
//

#include "ball.h"


ball::ball(KGraphic * graphic, int posx, int posy, float vx, float vy, int w, int h, int game_w, int game_h, int screen_w, int screen_h)
{
    kg = graphic;
    
    screenWidth = screen_w;
    screenHeight = screen_h;
    
    //pos = new Vector((float)posx, (float)posy);
    vel = new Vector(vx, vy);
    siz = new Vector(w, h);

    
    // setup box2D --------------------------------------------------
    
    vec2 vertice[] = {
        {0, 0},
        {0+w, 0},
        {0+w, 0+h},
        {0, 0+h}
    };
    
    numberOfVertices = 4;
    vertices = new float[numberOfVertices*3];
    for(int i=0 ; i<numberOfVertices ; i++)
    {
        vertices[(i*3)+0] = vertice[i][0];
        vertices[(i*3)+1] = vertice[i][1];
        vertices[(i*3)+2] = 0;
        
        printf("vertices[%d] = %f\n", (i*3)+0, vertices[(i*numberOfVertices)+0]);
        printf("vertices[%d] = %f\n", (i*3)+1, vertices[(i*numberOfVertices)+1]);
        printf("vertices[%d] = %f\n", (i*3)+2, vertices[(i*numberOfVertices)+2]);
    }
    
    b2Vec2 gravity = b2Vec2(0.0f, 9.0f);
    _world = new b2World(gravity);
    
    float px = posx / PTM_RATIO;
    float py = posy / PTM_RATIO;
    
    int category = 0;
    int mask = 0;
    
    //box2d_body = physicengine->createEntity(entity,CATEGORY_BLOCK, -1);
   
    ballBodyDef.type = b2_dynamicBody;
    //ballBodyDef.linearDamping = 0.5;
    //ballBodyDef.angularDamping = 0.2f;
    ballBodyDef.position.Set(px, py);
    ballBodyDef.userData = this;
    ballBodyDef.active = true;
    
    box2d_body = _world->CreateBody(&ballBodyDef);
    
    /*
    if(entity->getShapeType() == SHAPE_CIRCLE || entity->getShapeType() == SHAPE_STAR)
    {
        b2CircleShape circle;
        circle.m_radius = entity->getCircleRadius()/PTM_RATIO;
        
        b2FixtureDef ballShapeDef;
        ballShapeDef.shape = &circle;
        //ballShapeDef.isSensor = false;
        ballShapeDef.density = density;
        ballShapeDef.friction = 1;//0.10f;
        ballShapeDef.restitution = 0.5f;// 1;//0.25f;//0.75f;
        if(category > 0) {
            ballShapeDef.filter.categoryBits = category;
        }
        if(mask > 0) {
            ballShapeDef.filter.maskBits = mask;
        }
        fixture = _ph_entity->CreateFixture(&ballShapeDef);
        //    _ph_entity->SetAngularVelocity(0.0f);
    }
     */
    
    // SHAPE_POLYGON
    
    b2PolygonShape polygon;
    
    b2Vec2 * phy_vertices = new b2Vec2[numberOfVertices];
    for(int i=0 ; i<numberOfVertices ; i++)
    {
        float px = vertices[i*3];
        px /= PTM_RATIO;
        
        float py = vertices[(i*3)+1];
        py /= PTM_RATIO;
        
        phy_vertices[i].Set(px, py);
    }
    polygon.Set(phy_vertices, numberOfVertices);
    
    b2FixtureDef polygonShapeDef;
    polygonShapeDef.shape = &polygon;
    polygonShapeDef.density = 100*density;
    polygonShapeDef.friction = 1;
    polygonShapeDef.restitution = 0; //0.25f;
    if(category > 0) {
        polygonShapeDef.filter.categoryBits = category;
    }
    if(mask > 0) {
        polygonShapeDef.filter.maskBits = mask;
    }
    fixture = box2d_body->CreateFixture(&polygonShapeDef);
    //_ph_entity->SetAngularVelocity(0.3f);


    //    bool sleep_Allowed = _ph_entity->IsSleepingAllowed();
    //    sleep_Allowed;
    box2d_body->SetSleepingAllowed( false );
    //box2d_body->SetTransform(box2d_body->GetPosition(), entAngleRad);
    
    /*
    b2Vec2 vectForce = b2Vec2(px2, py2);
    vectForce.Normalize();
    vectForce.x *= exploForce;
    vectForce.y *= exploForce;
    box2d_body->ApplyForce(vectForce, body->GetPosition());
    */
    

}

ball::~ball()
{
    if(_world)
    {
        delete _world;
        _world = NULL;
    }
}

Vector ball::getPos()
{
    Vector pos;
    pos.x = 0;
    pos.y = 0;
    if(box2d_body) {
        b2Vec2 posb2d = box2d_body->GetPosition();
        pos.x = posb2d.x * PTM_RATIO;
        pos.y = posb2d.y * PTM_RATIO;
    }
    return pos;
}

void ball::editFixture(float density, float friction, float restitution)
{
    if(!box2d_body) {
        return;
    }
    b2Fixture * F = box2d_body->GetFixtureList();
    while(F != NULL)
    {
        //F->SetFilterData(<#const b2Filter &filter#>)
        F->SetDensity(density);
        F->SetFriction(friction);
        F->SetRestitution(restitution);
        F = F->GetNext();
    }
}

void ball::update(float frametime)
{
    float ballVel = 0.005f;
    
    if(frametime > 0.12) {
        //frametime = 0.12;
    }
    //_world->Step((float32)frametime/1000.0, 10, 10);
    _world->Step(1.0/60.0, 8, 3);
    
    if(!box2d_body) {
        return;
    }
    
    b2Vec2 lv = box2d_body->GetLinearVelocity();
        
    Vector pos = getPos();

    Vector v = (*vel) * (ballVel * frametime * 0.001f);

    /*
    pos += v;
    
    // Handle horizontal screen boundaries
    if (vel->x >= 0) {
        if (pos.x + siz->x > screenWidth) {
            vel->x *= -1; // Reverse direction
        }
    } else {
        if (pos.x <= 0) {
            vel->x *= -1; // Reverse direction
        }
    }

    if (vel->y >= 0) {
        if (pos.y + siz->y > screenHeight) {
            vel->y *= -1; // Reverse direction
        }
    } else {
        if (pos.y <= 0) {
            vel->y *= -1; // Reverse direction
        }
    }
    */
    
    
    
    // Update Box2D ----------------------------------------------
    


    float blocy = pos.y; //  pos->y / PTM_RATIO; //  //box2d_body->getPosY();
    
    printf("blocy %f lv.y %f\n", blocy, lv.y);
    
    
    //    printf("_current_block (%x) %f ; ylimit=%d ; %d < %d\n", (unsigned int)_current_block, blocy, ylimit, (int)timer->getDelta(), (int)timer->getDelay());
    
//    if (box2d_body->GetType() != b2_dynamicBody) {
//        box2d_body->SetType(b2_dynamicBody);
//    }
//    float mass = box2d_body->GetMass();
//    if (mass == 0) {
//        // Recalculate mass if necessary
//        b2MassData massData;
//        box2d_body->GetMassData(&massData);
//        massData.mass = 10;//desired_mass;
//        box2d_body->SetMassData(&massData);
//    }
//    if (box2d_body->IsFixedRotation()) {
//        box2d_body->SetFixedRotation(false);
//    }
//    box2d_body->SetLinearDamping(0.0f);
//    box2d_body->SetAwake(true);
    
    int ylimit = 530;
    
    if(lv.y > 0) {
        if(blocy > ylimit)
        {
            lv.y *= -1;
            box2d_body->SetLinearVelocity(lv);
            //      box2d_body->SetLinearVelocity(lv);
            //printf("_current_block VELY= %f\n", lv.y);
        }
    }
    
    b2Vec2 posb2d;
    posb2d.x = pos.x / PTM_RATIO;
    posb2d.y = pos.y / PTM_RATIO;
        
    b2Transform trans = box2d_body->GetTransform();
    b2Rot rot = trans.q;
    float entAngleRad = rot.GetAngle();
    //entAngleRad += (_yoff_tap - hity) / 2000.0f;
    box2d_body->SetTransform(posb2d, entAngleRad);
    
 
    
    
    
}
