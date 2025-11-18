#include "bullet.h"
#include <raylib.h>
#include <cmath>

Bullet::Bullet(float x, float y, Vector2 dir, float spd, bool explosive, bool homing, int dmg)
    : position({x, y}), direction(dir), speed(spd), active(true), 
      isExplosive(explosive), isHoming(homing), damage(dmg) {
    size = {5, 10};
}

void Bullet::Update() {
    if (active) {
        position.x += direction.x * speed;
        position.y += direction.y * speed;

        // Verificar se saiu da tela
        if (position.y < -size.y || position.y > GetScreenHeight() ||
            position.x < -size.x || position.x > GetScreenWidth()) {
            active = false;
        }
    }
}

void Bullet::Draw() const {
    if (active) {
        Color color = YELLOW;
        if (isExplosive) color = ORANGE;
        if (isHoming) color = PURPLE;
        
        DrawRectangleV(position, size, color);
        
        
        if (isHoming) {
            DrawCircleLines(position.x + size.x/2, position.y + size.y/2, size.x, Fade(PURPLE, 0.5f));
        }
    }
}

void Bullet::SetDirection(Vector2 dir) {
    direction = dir;
}

bool Bullet::IsOffScreen() const {
    return !active;
}