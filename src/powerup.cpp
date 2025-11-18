#include "powerup.h"
#include <cmath>
#include <raylib.h>

PowerUp::PowerUp(float x, float y, PowerUpType type) 
    : position({x, y}), type(type), radius(15.0f), active(true), animationTime(0.0f) {}

void PowerUp::Update() {
    // Animação flutuante
    animationTime += 0.1f;
    position.y += sin(animationTime) * 0.5f;
}

void PowerUp::Draw() const {
    if (!active) return;
    
    Color color = GetColor(type);
    DrawCircleV(position, radius, color);
    DrawCircleV(position, radius - 5, Fade(color, 0.7f));
    DrawCircleV(position, radius - 10, Fade(color, 0.4f));
}

const char* PowerUp::GetName(PowerUpType type) {
    switch(type) {
        case PowerUpType::DOUBLE_SHOT: return "Double Shot";
        case PowerUpType::TRIPLE_SHOT: return "Triple Shot";
        case PowerUpType::EXPLOSIVE_SHOT: return "Explosive Shot";
        case PowerUpType::HOMING_MISSILE: return "Homing Missile";
        case PowerUpType::AUTO_FIRE: return "Auto Fire";
        case PowerUpType::DASH: return "Dash";
        case PowerUpType::SPEED_BOOST: return "Speed Boost";
        case PowerUpType::DAMAGE_BOOST: return "Damage Boost";
        case PowerUpType::FIRE_RATE: return "Fire Rate";
        case PowerUpType::EXTRA_LIFE: return "Extra Life";
        case PowerUpType::REGENERATION: return "Regeneration";
        case PowerUpType::SHIELD: return "Shield";
        case PowerUpType::SCORE_BOOST: return "Score Boost";
        case PowerUpType::CLONE: return "Clone";
        default: return "Unknown";
    }
}

Color PowerUp::GetColor(PowerUpType type) {
    switch(type) {
        case PowerUpType::DOUBLE_SHOT: return BLUE;
        case PowerUpType::TRIPLE_SHOT: return SKYBLUE;
        case PowerUpType::EXPLOSIVE_SHOT: return ORANGE;
        case PowerUpType::HOMING_MISSILE: return PURPLE;
        case PowerUpType::AUTO_FIRE: return YELLOW;
        case PowerUpType::DASH: return GREEN;
        case PowerUpType::SPEED_BOOST: return LIME;
        case PowerUpType::DAMAGE_BOOST: return DARKPURPLE;
        case PowerUpType::FIRE_RATE: return GOLD;
        case PowerUpType::EXTRA_LIFE: return PINK;
        case PowerUpType::REGENERATION: return MAGENTA;
        case PowerUpType::SHIELD: return DARKBLUE;
        case PowerUpType::SCORE_BOOST: return VIOLET;
        case PowerUpType::CLONE: return BEIGE;
        default: return WHITE;
    }
}