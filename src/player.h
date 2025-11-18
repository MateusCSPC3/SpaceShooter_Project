#pragma once
#include <vector>
#include <map>
#include "bullet.h"

class Player {
public:
    Player();
    void Update();
    void Draw() const;
    std::vector<Bullet> bullets; 

    // Getters públicos para acesso do GameManager
    int GetX() const { return x; }
    int GetY() const { return y; }
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    int GetLives() const { return lives; }
    bool HasShield() const { return shieldActive; }
    float GetSpeedMultiplier() const { return speedMultiplier; }
    int GetDamageMultiplier() const { return damageMultiplier; }
    float GetFireRateMultiplier() const { return fireRateMultiplier; }
    int GetScoreMultiplier() const { return scoreMultiplier; }

    // Métodos para controle de habilidades
    void ApplyPowerUp(int powerUpType);
    void TakeDamage();
    void Dash();
    bool CanDash() const { return dashCooldown <= 0; }
    void FireBullet();
    void UpdateHomingMissiles();
    
    // Sistema de clones
    struct Clone {
        int x, y;
        int timer;
        std::vector<Bullet> bullets;
    };
    std::vector<Clone> clones;

    // Sistema de skins
    void SetSkin(int skinIndex) { currentSkin = skinIndex; }
    int GetCurrentSkin() const { return currentSkin; }

private:
    int x, y;
    int baseSpeed;
    float speedMultiplier;
    int width, height;
    int lives;
    int maxLives;
    
    // Sistema de habilidades
    bool shieldActive;
    bool doubleShotActive;
    bool tripleShotActive;
    bool explosiveShotActive;
    bool homingMissileActive;
    bool autoFireActive;
    int damageMultiplier;
    float fireRateMultiplier;
    int scoreMultiplier;
    
    // Cooldowns e temporizadores
    int dashCooldown;
    int homingMissileCooldown;
    int fireCooldown;
    int regenerationTimer;
    
   
    std::map<int, int> powerUpDurations;
    
    
    int currentSkin;
    
    void UpdateCooldowns();
    void UpdateRegeneration();
    void UpdatePowerUpDurations();
};