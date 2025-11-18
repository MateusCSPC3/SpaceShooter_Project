#include "enemy.h"
#include <raylib.h>
#include <algorithm>

Enemy::Enemy(int startX, int startY, EnemyType enemyType)
    : x(startX), y(startY), type(enemyType), shootCooldown(60) {
    
    // Configurações baseadas no tipo
    switch(type) {
        case EnemyType::BASIC:
            speed = 2;
            radius = 20;
            health = 3;
            initialHealth = 3;
            width = 64;   //  Tamanho para sprite
            height = 64;  //  Tamanho para sprite
            break;
        case EnemyType::DOUBLE:
            speed = 3;
            radius = 22;
            health = 4;
            initialHealth = 4;
            width = 64;
            height = 64;
            break;
        case EnemyType::TRIPLE:
            speed = 2;
            radius = 24;
            health = 5;
            initialHealth = 5;
            width = 64;
            height = 64;
            break;
        case EnemyType::SPREAD:
            speed = 1;
            radius = 26;
            health = 6;
            initialHealth = 6;
            width = 64;
            height = 64;
            break;
    }
}

void Enemy::Update() {
    y += speed;

    // Atira de tempos em tempos
    shootCooldown--;
    if (shootCooldown <= 0) {
        Shoot();
        
        // Define cooldown baseado no tipo
        switch(type) {
            case EnemyType::BASIC: shootCooldown = 120; break;
            case EnemyType::DOUBLE: shootCooldown = 100; break;
            case EnemyType::TRIPLE: shootCooldown = 90; break;
            case EnemyType::SPREAD: shootCooldown = 150; break;
        }
    }

    // Atualizar projeteis
    for (auto &bullet : bullets) {
        bullet.Update();
    }

    // Remover projeteis inativos
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
                       [](const Bullet &b) { return !b.active; }),
        bullets.end()
    );
}

void Enemy::Shoot() {
    switch(type) {
        case EnemyType::BASIC:
            ShootBasic();
            break;
        case EnemyType::DOUBLE:
            ShootDouble();
            break;
        case EnemyType::TRIPLE:
            ShootTriple();
            break;
        case EnemyType::SPREAD:
            ShootSpread();
            break;
    }
}

void Enemy::ShootBasic() {
    // Tiro simples para baixo (direção {0, 1})
    bullets.emplace_back((float)x + width/2, (float)y + height, Vector2{0, 1}, 5.0f);
}

void Enemy::ShootDouble() {
    // Tiro duplo - similar ao do player mas para baixo
    bullets.emplace_back((float)x + width/3, (float)y + height, Vector2{0, 1}, 5.0f);
    bullets.emplace_back((float)x + 2*width/3, (float)y + height, Vector2{0, 1}, 5.0f);
}

void Enemy::ShootTriple() {
    // Tiro triplo - similar ao do player mas para baixo
    bullets.emplace_back((float)x + width/2, (float)y + height, Vector2{0, 1}, 5.0f);
    bullets.emplace_back((float)x + width/2, (float)y + height, Vector2{-0.2f, 1}, 5.0f);
    bullets.emplace_back((float)x + width/2, (float)y + height, Vector2{0.2f, 1}, 5.0f);
}

void Enemy::ShootSpread() {
    // Tiro espalhado - 5 projéteis em leque para baixo
    bullets.emplace_back((float)x + width/2, (float)y + height, Vector2{0, 1}, 4.0f);
    bullets.emplace_back((float)x + width/2, (float)y + height, Vector2{-0.3f, 0.9f}, 4.0f);
    bullets.emplace_back((float)x + width/2, (float)y + height, Vector2{0.3f, 0.9f}, 4.0f);
    bullets.emplace_back((float)x + width/2, (float)y + height, Vector2{-0.6f, 0.8f}, 4.0f);
    bullets.emplace_back((float)x + width/2, (float)y + height, Vector2{0.6f, 0.8f}, 4.0f);
}

void Enemy::Draw() const {
    
    
    // Barra de vida
    float healthPercent = (float)health / (float)initialHealth;
    DrawRectangle(x - width/2, y - height/2 - 10, (int)(width * healthPercent), 5, GREEN);

    // Desenhar projéteis
    for (const auto &bullet : bullets) {
        bullet.Draw();
    }
}

bool Enemy::IsOffScreen() const {
    return (y - height > GetScreenHeight());
}