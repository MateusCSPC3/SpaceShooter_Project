#pragma once
#include <raylib.h>
#include <string>
#include <map>

// Enumeração de tipos de power-ups
enum class PowerUpType {
    DOUBLE_SHOT,
    TRIPLE_SHOT,
    EXPLOSIVE_SHOT,
    HOMING_MISSILE,
    AUTO_FIRE,
    DASH,
    SPEED_BOOST,
    DAMAGE_BOOST,
    FIRE_RATE,
    EXTRA_LIFE,
    REGENERATION,
    SHIELD,
    SCORE_BOOST,
    CLONE
};

class PowerUp {
public:
    PowerUp(float x, float y, PowerUpType type);
    void Update();
    void Draw() const;
    
    Vector2 GetPosition() const { return position; }
    float GetRadius() const { return radius; }
    PowerUpType GetType() const { return type; }
    bool IsActive() const { return active; }
    void Collect() { active = false; }
    
    static const char* GetName(PowerUpType type);
    static Color GetColor(PowerUpType type);
    
private:
    Vector2 position;
    PowerUpType type;
    float radius;
    bool active;
    float animationTime;
};