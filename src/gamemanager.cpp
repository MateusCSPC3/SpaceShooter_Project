#include "gamemanager.h"
#include "raylib.h"
#include <algorithm>
#include <random>
#include "raymath.h"

GameManager::GameManager()
    : currentState(GameState::MENU), score(0), enemySpawnTimer(0),
      enemySpawnRate(2.0f), powerUpSpawnTimer(0), gameTime(0),
      difficultyScale(1.0f), baseSpawnRate(2.0f), waveNumber(1),
      selectedPlayerSkin(0), selectingSkin(true)
{
    LoadTextures();
    LoadAudio();  // Carrega a soundtrack
}

GameManager::~GameManager() {
    UnloadTextures();
    UnloadAudio();  // Descarrega a soundtrack
}

// NOVO: Método para carregar áudio
void GameManager::LoadAudio() {
    // Carrega a música de fundo 
    backgroundMusic = LoadMusicStream("assets/background_music.mp3");
    
    
    SetMusicVolume(backgroundMusic, 0.5f);
    
    // Toca a música em loop
    PlayMusicStream(backgroundMusic);
}


void GameManager::UnloadAudio() {
    UnloadMusicStream(backgroundMusic);
}

void GameManager::LoadTextures() {
    // Carrega o background
    background = LoadTexture("assets/background.png");
    
    // Carrega texturas do player
    playerTextures[0] = LoadTexture("assets/player.png");
    playerTextures[1] = LoadTexture("assets/player2.png");
    
    // Carrega texturas dos inimigos
    enemyTextures[0] = LoadTexture("assets/inimigo1.png");  // BASIC
    enemyTextures[1] = LoadTexture("assets/inimigo2.png");  // DOUBLE
    enemyTextures[2] = LoadTexture("assets/inimigo3.png");  // TRIPLE
    enemyTextures[3] = LoadTexture("assets/inimigo4.png");  // SPREAD
    
    // Configura o player com a skin selecionada
    player.SetSkin(selectedPlayerSkin);
}

void GameManager::UnloadTextures() {
    UnloadTexture(background);
    for (int i = 0; i < 2; i++) UnloadTexture(playerTextures[i]);
    for (int i = 0; i < 4; i++) UnloadTexture(enemyTextures[i]);
}

void GameManager::SetState(GameState newState) {
    currentState = newState;
}

void GameManager::Reset() {
    currentState = GameState::PLAYING;
    score = 0;
    enemySpawnTimer = 0;
    powerUpSpawnTimer = 0;
    gameTime = 0;
    difficultyScale = 1.0f;
    enemySpawnRate = baseSpawnRate;
    waveNumber = 1;
    player = Player();
    player.SetSkin(selectedPlayerSkin); // Mantém a skin selecionada
    enemies.clear();
    powerUps.clear();
}

void GameManager::Update() {
    
    UpdateMusicStream(backgroundMusic);
    
    switch (currentState) {
        case GameState::MENU:
            if (selectingSkin) {
                // Navegação entre skins
                if (IsKeyPressed(KEY_LEFT)) {
                    selectedPlayerSkin = (selectedPlayerSkin - 1 + 2) % 2;
                }
                if (IsKeyPressed(KEY_RIGHT)) {
                    selectedPlayerSkin = (selectedPlayerSkin + 1) % 2;
                }
                if (IsKeyPressed(KEY_ENTER)) {
                    selectingSkin = false;
                    player.SetSkin(selectedPlayerSkin);
                }
            } else {
                if (IsKeyPressed(KEY_ENTER)) {
                    SetState(GameState::PLAYING);
                }
                if (IsKeyPressed(KEY_BACKSPACE)) {
                    selectingSkin = true;
                }
            }
            break;

        case GameState::PLAYING:
            player.Update();
            gameTime += GetFrameTime();
            UpdateDifficulty();

            enemySpawnTimer += GetFrameTime();
            if (enemySpawnTimer >= enemySpawnRate) {
                int enemiesToSpawn = 1 + static_cast<int>(difficultyScale / 2.0f);
                enemiesToSpawn = std::min(enemiesToSpawn, 5);

                for (int i = 0; i < enemiesToSpawn; i++) {
                    SpawnEnemy();
                }
                enemySpawnTimer = 0;
            }

            for (auto it = enemies.begin(); it != enemies.end(); ) {
                it->Update();
                if (it->health <= 0 || it->IsOffScreen()) {
                    if (it->health <= 0) {
                        int baseScore = 100;
                        switch (it->GetType()) {
                            case EnemyType::BASIC: baseScore = 100; break;
                            case EnemyType::DOUBLE: baseScore = 150; break;
                            case EnemyType::TRIPLE: baseScore = 200; break;
                            case EnemyType::SPREAD: baseScore = 250; break;
                        }
                        score += baseScore * player.GetScoreMultiplier();

                        if (GetRandomValue(0, 100) < 25) {
                            SpawnPowerUp(it->GetX(), it->GetY());
                        }
                    }
                    it = enemies.erase(it);
                } else {
                    ++it;
                }
            }

            for (auto &powerUp : powerUps) {
                powerUp.Update();
            }

            powerUps.erase(
                std::remove_if(powerUps.begin(), powerUps.end(),
                               [](const PowerUp &p) { return !p.IsActive(); }),
                powerUps.end()
            );

            CheckCollisions();

            if (IsKeyPressed(KEY_P)) {
                SetState(GameState::PAUSE);
            }
            break;

        case GameState::PAUSE:
            if (IsKeyPressed(KEY_P)) {
                SetState(GameState::PLAYING);
            }
            break;

        case GameState::GAME_OVER:
            if (IsKeyPressed(KEY_R)) {
                Reset();
            }
            if (IsKeyPressed(KEY_ENTER)) {
                SetState(GameState::MENU);
                selectingSkin = true;
            }
            break;
    }
}

void GameManager::UpdateDifficulty() {
    difficultyScale = 1.0f + (gameTime / 30.0f);
    waveNumber = 1 + static_cast<int>(gameTime / 30.0f);
    enemySpawnRate = baseSpawnRate / (difficultyScale * 0.8f);
    enemySpawnRate = std::max(enemySpawnRate, 0.3f);
}

EnemyType GameManager::GetRandomEnemyType() {
    int rand = GetRandomValue(0, 100);
    if (difficultyScale > 2.5f && rand < 20) return EnemyType::SPREAD;
    else if (difficultyScale > 1.8f && rand < 35) return EnemyType::TRIPLE;
    else if (difficultyScale > 1.2f && rand < 50) return EnemyType::DOUBLE;
    else if (rand < 30) {
        int earlyRand = GetRandomValue(0, 100);
        if (earlyRand < 60) return EnemyType::DOUBLE;
        else if (earlyRand < 85) return EnemyType::TRIPLE;
        else return EnemyType::SPREAD;
    }
    return EnemyType::BASIC;
}

void GameManager::SpawnEnemy() {
    int x = GetRandomValue(50, GetScreenWidth() - 50);
    EnemyType type = GetRandomEnemyType();
    enemies.emplace_back(x, -50, type);
}

void GameManager::SpawnPowerUp(float x, float y) {
    int randomType = GetRandomValue(0, static_cast<int>(PowerUpType::CLONE));
    powerUps.emplace_back(x, y, static_cast<PowerUpType>(randomType));
}

void GameManager::CheckCollisions() {
    Rectangle playerRect = { (float)player.GetX(), (float)player.GetY(), (float)player.GetWidth(), (float)player.GetHeight() };

    for (auto &enemy : enemies) {
        for (auto &bullet : player.bullets) {
            if (bullet.active) {
                Rectangle bulletRect = { bullet.position.x, bullet.position.y, bullet.size.x, bullet.size.y };
                Rectangle enemyRect = { (float)enemy.GetX() - enemy.GetRadius(), (float)enemy.GetY() - enemy.GetRadius(), (float)enemy.GetRadius() * 2, (float)enemy.GetRadius() * 2 };
                if (CheckCollisionRecs(bulletRect, enemyRect)) {
                    bullet.active = false;
                    enemy.health -= player.GetDamageMultiplier();
                }
            }
        }

        for (auto &clone : player.clones) {
            for (auto &bullet : clone.bullets) {
                if (bullet.active) {
                    Rectangle bulletRect = { bullet.position.x, bullet.position.y, bullet.size.x, bullet.size.y };
                    Rectangle enemyRect = { (float)enemy.GetX() - enemy.GetRadius(), (float)enemy.GetY() - enemy.GetRadius(), (float)enemy.GetRadius() * 2, (float)enemy.GetRadius() * 2 };
                    if (CheckCollisionRecs(bulletRect, enemyRect)) {
                        bullet.active = false;
                        enemy.health--;
                    }
                }
            }
        }

        Rectangle enemyRect = { (float)enemy.GetX() - enemy.GetRadius(), (float)enemy.GetY() - enemy.GetRadius(), (float)enemy.GetRadius() * 2, (float)enemy.GetRadius() * 2 };
        if (CheckCollisionRecs(playerRect, enemyRect)) {
            player.TakeDamage();
            if (player.GetLives() <= 0) {
                SetState(GameState::GAME_OVER);
            }
        }

        for (auto &bullet : enemy.bullets) {
            if (bullet.active) {
                Rectangle bulletRect = { bullet.position.x, bullet.position.y, bullet.size.x, bullet.size.y };
                if (CheckCollisionRecs(bulletRect, playerRect)) {
                    bullet.active = false;
                    player.TakeDamage();
                    if (player.GetLives() <= 0) {
                        SetState(GameState::GAME_OVER);
                    }
                }
            }
        }
    }

    for (auto &powerUp : powerUps) {
        if (powerUp.IsActive()) {
            Vector2 playerCenter = { playerRect.x + playerRect.width / 2, playerRect.y + playerRect.height / 2 };
            Vector2 powerUpPos = powerUp.GetPosition();
            float distance = Vector2Distance(playerCenter, powerUpPos);
            if (distance < playerRect.width / 2 + powerUp.GetRadius()) {
                player.ApplyPowerUp(static_cast<int>(powerUp.GetType()));
                powerUp.Collect();
            }
        }
    }

    player.bullets.erase(
        std::remove_if(player.bullets.begin(), player.bullets.end(),
                       [](const Bullet &b) { return !b.active; }),
        player.bullets.end()
    );
}

void GameManager::DrawHUD() const {
    const int screenWidth = GetScreenWidth();
    const int heartSize = 30;
    const int heartSpacing = 10;
    const int margin = 20;
    int x = screenWidth - margin - heartSize;
    int y = margin;

    for (int i = 0; i < player.GetLives(); i++) {
        Color heartColor = RED;
        DrawCircle(x - heartSize/4, y + heartSize/4, heartSize/3, heartColor);
        DrawCircle(x + heartSize/4, y + heartSize/4, heartSize/3, heartColor);
        DrawTriangle(
            { (float)x, (float)y + heartSize/2 },
            { (float)x - heartSize/2, (float)y - heartSize/4 },
            { (float)x + heartSize/2, (float)y - heartSize/4 },
            heartColor
        );
        x -= (heartSize + heartSpacing);
    }

    DrawText(TextFormat("Score: %d", score), 10, 10, 20, WHITE);
    DrawText(TextFormat("Wave: %d", waveNumber), 10, 35, 20, YELLOW);
    DrawPowerUpIndicator();
}

void GameManager::DrawPowerUpIndicator() const {
    int yPos = 60;
    if (player.HasShield()) { DrawText("Shield Active", 10, yPos, 20, BLUE); yPos += 25; }
    if (player.GetSpeedMultiplier() > 1.0f) { DrawText("Speed Boost Active", 10, yPos, 20, GREEN); yPos += 25; }
    if (player.GetDamageMultiplier() > 1) { DrawText("Damage Boost Active", 10, yPos, 20, RED); yPos += 25; }
    if (player.GetFireRateMultiplier() > 1.0f) { DrawText("Fire Rate Boost Active", 10, yPos, 20, YELLOW); yPos += 25; }
    if (player.GetScoreMultiplier() > 1) { DrawText("Score Boost Active", 10, yPos, 20, PURPLE); yPos += 25; }
}

void GameManager::DrawSkinSelection() const {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    
    DrawText("SELECT YOUR SHIP", screenWidth/2 - 120, screenHeight/2 - 150, 30, YELLOW);
    
    // Desenha as opções de skin
    for (int i = 0; i < 2; i++) {
        int xPos = screenWidth/2 - 100 + (i * 250);
        int yPos = screenHeight/2 - 50;
        
        // Destaque para a skin selecionada
        if (i == selectedPlayerSkin) {
            DrawRectangle(xPos - 10, yPos - 10, 84, 84, YELLOW);
        }
        
        DrawTexture(playerTextures[i], xPos, yPos, WHITE);
        DrawText(TextFormat("SHIP %d", i + 1), xPos + 10, yPos + 70, 20, WHITE);
    }
    
    DrawText("USE <- -> TO SELECT", screenWidth/2 - 120, screenHeight/2 + 100, 25, GREEN);
    DrawText("PRESS ENTER TO CONFIRM", screenWidth/2 - 140, screenHeight/2 + 130, 25, GREEN);
}

void GameManager::DrawMenu() const {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    
    if (selectingSkin) {
        DrawSkinSelection();
    } else {
        DrawText("SPACE SHOOTER", screenWidth/2 - 150, screenHeight/2 - 100, 50, WHITE);
        DrawText("PRESS ENTER TO START", screenWidth/2 - 150, screenHeight/2, 30, GREEN);
        DrawText("PRESS BACKSPACE TO CHANGE SHIP", screenWidth/2 - 200, screenHeight/2 + 40, 20, YELLOW);
        DrawText("CONTROLS:", screenWidth/2 - 100, screenHeight/2 + 80, 25, YELLOW);
        DrawText("WASD/ARROWS - Move", screenWidth/2 - 120, screenHeight/2 + 110, 20, WHITE);
        DrawText("SPACE - Shoot", screenWidth/2 - 120, screenHeight/2 + 135, 20, WHITE);
        DrawText("P - Pause", screenWidth/2 - 120, screenHeight/2 + 160, 20, WHITE);
    }
}

void GameManager::DrawPause() const {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.7f));
    DrawText("GAME PAUSED", screenWidth/2 - 120, screenHeight/2 - 50, 40, YELLOW);
    DrawText("PRESS P TO CONTINUE", screenWidth/2 - 140, screenHeight/2 + 20, 25, WHITE);
}

// MÉTODO DRAW() 
void GameManager::Draw() const {
    // Desenha o background antes de tudo
    DrawTexture(background, 0, 0, WHITE);

    // DESENHA O PLAYER COM SPRITE
    DrawTexture(playerTextures[player.GetCurrentSkin()], player.GetX(), player.GetY(), WHITE);
    
    // Desenha elementos do player (bullets, efeitos)
    player.Draw();

    // DESENHA INIMIGOS COM SPRITES
    for (const auto &enemy : enemies) {
        
        int enemyX = enemy.GetX() - 32; 
        int enemyY = enemy.GetY() - 32;
        
        // Seleciona a textura baseada no tipo
        Texture2D enemyTexture;
        switch(enemy.GetType()) {
            case EnemyType::BASIC: enemyTexture = enemyTextures[0]; break;
            case EnemyType::DOUBLE: enemyTexture = enemyTextures[1]; break;
            case EnemyType::TRIPLE: enemyTexture = enemyTextures[2]; break;
            case EnemyType::SPREAD: enemyTexture = enemyTextures[3]; break;
        }
        
        DrawTexture(enemyTexture, enemyX, enemyY, WHITE);
        // Desenha elementos do enemy (bullets, barra de vida)
        enemy.Draw();
    }

    // Desenha power-ups
    for (const auto &powerUp : powerUps) powerUp.Draw();
    
    // DESENHA CLONES DO PLAYER
    for (const auto &clone : player.clones) {
        DrawTexture(playerTextures[player.GetCurrentSkin()], clone.x, clone.y, Fade(WHITE, 0.5f));
    }

    DrawHUD();

    // Estados do jogo
    switch (currentState) {
        case GameState::MENU:
            DrawMenu();
            break;
        case GameState::PAUSE:
            DrawPause();
            break;
        case GameState::GAME_OVER:
            DrawText("GAME OVER", GetScreenWidth()/2 - 100, GetScreenHeight()/2 - 50, 40, RED);
            DrawText("Press R to restart", GetScreenWidth()/2 - 120, GetScreenHeight()/2, 20, WHITE);
            DrawText("Press ENTER for menu", GetScreenWidth()/2 - 140, GetScreenHeight()/2 + 30, 20, WHITE);
            break;
        case GameState::PLAYING:
            break;
    }
}