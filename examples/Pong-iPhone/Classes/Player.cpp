#include "Player.h"
#include "KInput.h"
#include "KPTK.h"

Player::Player(float x, float y, float s)
    : pos(x, y, 1.0f), // on fixe s à 1.0 pour la position
      size(s)
{}

void Player::Update(float dt) {
    bool up    = KInput::isPressed(K_VK_W) || KInput::isPressed(K_VK_UP);
    bool down  = KInput::isPressed(K_VK_S) || KInput::isPressed(K_VK_DOWN);
    bool left  = KInput::isPressed(K_VK_A) || KInput::isPressed(K_VK_LEFT);
    bool right = KInput::isPressed(K_VK_D) || KInput::isPressed(K_VK_RIGHT);

    float speed = 100.0f;

    // On modifie directement pos.x et pos.y
    if (up)    pos.y -= speed * dt;
    if (down)  pos.y += speed * dt;
    if (left)  pos.x -= speed * dt;
    if (right) pos.x += speed * dt;
}

void Player::Draw(KGraphic* gfx) {
    float half = size / 2.0f;
    vec2 vertice[4] = {
        {-half, -half},
        { half, -half},
        { half,  half},
        {-half,  half}
    };
    gfx->drawShape(4, vertice, (int)pos.x, (int)pos.y, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
}
