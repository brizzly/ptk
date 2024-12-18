#include "Player.h"
#include "KInput.h"
#include "KGraphic.h"

Player::Player(float x, float y, float s)
    : pos(x, y, 1.0f),
      size(s)
{}

void Player::Update(float dt) {
    // Déplacement tactile
    bool fingerDown = KInput::isScreenPressed();
    float fx = KInput::getMouseX();
    float fy = KInput::getMouseY();

    // Déplacement clavier
    bool up    = KInput::isPressed(K_VK_W) || KInput::isPressed(K_VK_UP);
    bool down  = KInput::isPressed(K_VK_S) || KInput::isPressed(K_VK_DOWN);
    bool left  = KInput::isPressed(K_VK_A) || KInput::isPressed(K_VK_LEFT);
    bool right = KInput::isPressed(K_VK_D) || KInput::isPressed(K_VK_RIGHT);

    float speed = 100.0f;

    if (fingerDown) {
        // Si le doigt est sur l'écran, on place le joueur sous le doigt
        SetPosition(fx, fy);
    } else {
        // Sinon, déplacements au clavier
        if (up)    pos.y -= speed * dt;
        if (down)  pos.y += speed * dt;
        if (left)  pos.x -= speed * dt;
        if (right) pos.x += speed * dt;
    }
}

void Player::Draw(KGraphic* gfx) {
    if (!gfx) return;
    float half = size / 2.0f;
    vec2 vertice[4] = {
        {-half, -half},
        { half, -half},
        { half,  half},
        {-half,  half}
    };
    gfx->drawShape(4, vertice, (int)pos.x, (int)pos.y, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
}

void Player::SetPosition(float x, float y) {
    pos.x = x;
    pos.y = y;
}

float Player::GetSize() const {
    return size;
}

const Vector& Player::GetPosition() const {
    return pos;
}
