#include "player.h"
#include <raylib.h>
#include <algorithm>
#include <cmath>
#include "powerup.h"

Player::Player()
    : x(500), y(600), baseSpeed(5), speedMultiplier(1.0f), 
      width(64), height(64), lives(3), maxLives(3),  
      shieldActive(false), doubleShotActive(false), tripleShotActive(false),
      explosiveShotActive(false), homingMissileActive(false), autoFireActive(false),
      damageMultiplier(1), fireRateMultiplier(1.0f), scoreMultiplier(1),
      dashCooldown(0), homingMissileCooldown(0), fireCooldown(0), regenerationTimer(0),
      currentSkin(0)  
{}

void Player::Update() {
    // Movimento com multiplicador de velocidade
    float currentSpeed = baseSpeed * speedMultiplier;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) x += currentSpeed;
    if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) x -= currentSpeed;
    if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) y -= currentSpeed;
    if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) y += currentSpeed;

    // Limitar dentro da tela
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x + width > GetScreenWidth()) x = GetScreenWidth() - width;
    if (y + height > GetScreenHeight()) y = GetScreenHeight() - height;

    // Sistema de tiro automático
    if (autoFireActive && fireCooldown <= 0) {
        FireBullet();
        fireCooldown = 15 / fireRateMultiplier; // Taxa de tiro 
    }
    
    // Tiro manual
    if (IsKeyPressed(KEY_SPACE)) {
        FireBullet();
    }

    // Atualizar projeteis
    for (auto &bullet : bullets) {
        bullet.Update();
    }

    // Atualizar mísseis teleguiados
    if (homingMissileActive) {
        UpdateHomingMissiles();
    }

    // Remover projeteis que saíram da tela
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
                       [](const Bullet &b) { return b.IsOffScreen(); }),
        bullets.end()
    );
    
    // Atualizar clones
    for (auto &clone : clones) {
        clone.timer--;
        // Clones também atiram
        if (clone.timer % 30 == 0) {
            clone.bullets.push_back(Bullet(clone.x + width/2, clone.y));
        }
        
        // Atualizar projéteis dos clones
        for (auto &bullet : clone.bullets) {
            bullet.Update();
        }
        
        // Remover projéteis dos clones que saíram da tela
        clone.bullets.erase(
            std::remove_if(clone.bullets.begin(), clone.bullets.end(),
                           [](const Bullet &b) { return b.IsOffScreen(); }),
            clone.bullets.end()
        );
    }
    
    // Remover clones expirados
    clones.erase(
        std::remove_if(clones.begin(), clones.end(),
                       [](const Clone &c) { return c.timer <= 0; }),
        clones.end()
    );
    
    // Atualizar cooldowns e temporizadores
    UpdateCooldowns();
    UpdateRegeneration();
    UpdatePowerUpDurations();
}

void Player::UpdateCooldowns() {
    if (dashCooldown > 0) dashCooldown--;
    if (homingMissileCooldown > 0) homingMissileCooldown--;
    if (fireCooldown > 0) fireCooldown--;
}

void Player::UpdateRegeneration() {
    if (regenerationTimer > 0) {
        regenerationTimer--;
        if (regenerationTimer == 0 && lives < maxLives) {
            lives++;
            regenerationTimer = 300; 
        }
    }
}

void Player::UpdatePowerUpDurations() {
    for (auto it = powerUpDurations.begin(); it != powerUpDurations.end(); ) {
        it->second--;
        if (it->second <= 0) {
            // Desativar power-up quando o tempo acabar
            switch(static_cast<PowerUpType>(it->first)) {
                case PowerUpType::DOUBLE_SHOT: doubleShotActive = false; break;
                case PowerUpType::TRIPLE_SHOT: tripleShotActive = false; break;
                case PowerUpType::EXPLOSIVE_SHOT: explosiveShotActive = false; break;
                case PowerUpType::HOMING_MISSILE: homingMissileActive = false; break;
                case PowerUpType::AUTO_FIRE: autoFireActive = false; break;
                case PowerUpType::SPEED_BOOST: speedMultiplier = 1.0f; break;
                case PowerUpType::DAMAGE_BOOST: damageMultiplier = 1; break;
                case PowerUpType::FIRE_RATE: fireRateMultiplier = 1.0f; break;
                case PowerUpType::SCORE_BOOST: scoreMultiplier = 1; break;
                default: break;
            }
            it = powerUpDurations.erase(it);
        } else {
            ++it;
        }
    }
}

void Player::TakeDamage() {
    if (shieldActive) {
        shieldActive = false;
        return;
    }
    
    if (lives > 0) {
        lives--;
    }
}

void Player::Dash() {
    if (CanDash()) {
        Vector2 direction = {0, 0};
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) direction.x = 1;
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) direction.x = -1;
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) direction.y = -1;
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) direction.y = 1;
        
        
        float length = sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length > 0) {
            direction.x /= length;
            direction.y /= length;
        } else {
            direction.y = -1; 
        }
        
        // Aplicar dash
        x += direction.x * 100;
        y += direction.y * 100;
        
        // Limitar dentro da tela após o dash
        if (x < 0) x = 0;
        if (y < 0) y = 0;
        if (x + width > GetScreenWidth()) x = GetScreenWidth() - width;
        if (y + height > GetScreenHeight()) y = GetScreenHeight() - height;
        
        // Configurar cooldown
        dashCooldown = 120; // 2 segundos a 60 FPS
    }
}

void Player::FireBullet() {
    if (fireCooldown > 0) return;
    
    // Tiro normal
    bullets.push_back(Bullet(x + width/2, y));
    
    // Tiro duplo
    if (doubleShotActive) {
        bullets.push_back(Bullet(x + width/4, y));
        bullets.push_back(Bullet(x + 3*width/4, y));
    }
    
    // Tiro triplo
    if (tripleShotActive) {
        bullets.push_back(Bullet(x + width/2, y, {0, -1}, 8.0f));
        bullets.push_back(Bullet(x + width/2, y, {-0.2f, -1}, 8.0f));
        bullets.push_back(Bullet(x + width/2, y, {0.2f, -1}, 8.0f));
    }
    
    // Mísseis teleguiados
    if (homingMissileActive && homingMissileCooldown <= 0) {
        Bullet missile(x + width/2, y, {0, -1}, 4.0f);
        
        bullets.push_back(missile);
        homingMissileCooldown = 90; // 1.5 segundos a 60 FPS
    }
    
    // Configurar cooldown do tiro
    fireCooldown = 10 / fireRateMultiplier;
}

void Player::UpdateHomingMissiles() {
    
}

void Player::ApplyPowerUp(int powerUpType) {
    PowerUpType type = static_cast<PowerUpType>(powerUpType);
    
    switch(type) {
        case PowerUpType::DOUBLE_SHOT:
            doubleShotActive = true;
            powerUpDurations[powerUpType] = 600; // 10 segundos
            break;
            
        case PowerUpType::TRIPLE_SHOT:
            tripleShotActive = true;
            powerUpDurations[powerUpType] = 600; // 10 segundos
            break;
            
        case PowerUpType::EXPLOSIVE_SHOT:
            explosiveShotActive = true;
            powerUpDurations[powerUpType] = 600; // 10 segundos
            break;
            
        case PowerUpType::HOMING_MISSILE:
            homingMissileActive = true;
            powerUpDurations[powerUpType] = 600; // 10 segundos
            break;
            
        case PowerUpType::AUTO_FIRE:
            autoFireActive = true;
            powerUpDurations[powerUpType] = 600; // 10 segundos
            break;
            
        case PowerUpType::DASH:
            // Habilidade permanente, sem duração
            break;
            
        case PowerUpType::SPEED_BOOST:
            speedMultiplier = 1.5f;
            powerUpDurations[powerUpType] = 600; // 10 segundos
            break;
            
        case PowerUpType::DAMAGE_BOOST:
            damageMultiplier = 2;
            powerUpDurations[powerUpType] = 600; // 10 segundos
            break;
            
        case PowerUpType::FIRE_RATE:
            fireRateMultiplier = 1.5f;
            powerUpDurations[powerUpType] = 600; // 10 segundos
            break;
            
        case PowerUpType::EXTRA_LIFE:
            lives = std::min(lives + 1, maxLives + 1);
            maxLives = std::max(maxLives, lives);
            break;
            
        case PowerUpType::REGENERATION:
            regenerationTimer = 300; // 5 segundos
            break;
            
        case PowerUpType::SHIELD:
            shieldActive = true;
            break;
            
        case PowerUpType::SCORE_BOOST:
            scoreMultiplier = 2;
            powerUpDurations[powerUpType] = 600; // 10 segundos
            break;
            
        case PowerUpType::CLONE:
            clones.push_back({x - 50, y, 300, {}}); // Clone por 5 segundos
            break;
    }
}

void Player::Draw() const {
   
    
    // Desenhar projeteis
    for (const auto &bullet : bullets) {
        bullet.Draw();
    }
    
    // Desenhar clones 
    for (const auto &clone : clones) {
        
        for (const auto &bullet : clone.bullets) {
            bullet.Draw();
        }
    }
    
    // Indicador de dash pronto
    if (CanDash()) {
        DrawCircleLines(x + width/2, y + height + 10, 5, GREEN);
    }
    
    // Escudo (se ativo)
    if (shieldActive) {
        DrawCircleLines(x + width/2, y + height/2, width/2, BLUE);
    }
}