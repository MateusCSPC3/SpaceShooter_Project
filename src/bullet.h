#pragma once
#include <raylib.h>

class Bullet {
public:
    Vector2 position;
    Vector2 size;
    Vector2 direction;
    float speed;
    bool active;
    bool isExplosive;
    bool isHoming;
    int damage;

    // Construtor
    Bullet(float x = 0, float y = 0, Vector2 dir = {0, -1}, float spd = 8.0f, 
           bool explosive = false, bool homing = false, int dmg = 1);

    void Update();
    void Draw() const;
    void SetDirection(Vector2 dir);
    bool IsOffScreen() const;
};