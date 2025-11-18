#pragma once
#include "player.h"
#include "enemy.h"
#include "powerup.h"
#include <vector>
#include "raylib.h"  

enum class GameState {
    MENU,
    PLAYING,
    PAUSE,
    GAME_OVER
};

class GameManager {
public:
    GameManager();
    ~GameManager(); 

    void Update();
    void Draw() const;
    void Reset();

    void SetState(GameState newState);
    GameState GetState() const { return currentState; }
    bool IsGameOver() const { return currentState == GameState::GAME_OVER; }
    int GetScore() const { return score; }

private:
    void SpawnEnemy();
    void SpawnPowerUp(float x, float y);
    void CheckCollisions();
    void DrawHUD() const;
    void DrawPowerUpIndicator() const;
    void DrawMenu() const;
    void DrawPause() const;

    // Sistema de dificuldade
    void UpdateDifficulty();
    EnemyType GetRandomEnemyType();

    // Sistema de texturas e áudio
    void LoadTextures();
    void UnloadTextures();
    void LoadAudio();       //  Sistema de áudio
    void UnloadAudio();     //  Sistema de áudio
    void DrawSkinSelection() const;

    Player player;
    std::vector<Enemy> enemies;
    std::vector<PowerUp> powerUps;

    GameState currentState;
    int score;
    float enemySpawnTimer;
    float enemySpawnRate;
    float powerUpSpawnTimer;

    // Variáveis de dificuldade
    float gameTime;
    float difficultyScale;
    float baseSpawnRate;
    int waveNumber;

    // Sistema de texturas
    Texture2D background;
    Texture2D playerTextures[2];    // player.png e player2.png
    Texture2D enemyTextures[4];     // inimigo1.png à inimigo4.png
    
    // Seleção de skin
    int selectedPlayerSkin;
    bool selectingSkin;

    // Sistema de áudio
    Music backgroundMusic;
};