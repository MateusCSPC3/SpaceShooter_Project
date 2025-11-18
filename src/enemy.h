#pragma once
#include <vector>
#include "bullet.h"

// Enum para tipos de inimigos
enum class EnemyType {
    BASIC,      // Tiro simples
    DOUBLE,     // Tiro duplo  
    TRIPLE,     // Tiro triplo
    SPREAD      // Tiro espalhado
};

class Enemy {
public:
    Enemy(int startX, int startY, EnemyType type = EnemyType::BASIC);
    void Update();
    void Draw() const;
    bool IsOffScreen() const;

    int health;
    std::vector<Bullet> bullets;
    EnemyType GetType() const { return type; }

    int GetX() const { return x; }
    int GetY() const { return y; }
    int GetRadius() const { return radius; }
    int GetWidth() const { return width; }   
    int GetHeight() const { return height; } 

private:
    void Shoot();
    void ShootBasic();
    void ShootDouble();
    void ShootTriple();
    void ShootSpread();

    int x, y;
    int speed;
    int radius;
    int shootCooldown;
    EnemyType type;
    int initialHealth;
    
    
    int width;
    int height;
};

