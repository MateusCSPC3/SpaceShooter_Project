#include <raylib.h>
#include "gamemanager.h"

int main() {
    constexpr int screenWidth = 1000;
    constexpr int screenHeight = 700;

    // Inicializa a janela
    InitWindow(screenWidth, screenHeight, "Space Shooter");
    
    // INICIALIZA O ÁUDIO 
    InitAudioDevice();
    
    SetTargetFPS(60);

    GameManager game;

    // Loop principal do jogo
    while (!WindowShouldClose()) {
        // Atualização do jogo
        game.Update();

        // Renderização
        BeginDrawing();
        ClearBackground(BLACK);
        game.Draw();
        EndDrawing();
    }

    
    CloseAudioDevice();
    CloseWindow();
    
    return 0;
}