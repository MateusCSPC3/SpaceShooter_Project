🚀 Space Shooter - Documentação do Projeto
📋 Índice
Visão Geral

Funcionalidades

Controles

Sistema de Jogo

Estrutura do Projeto

Compilação

Assets

🎯 Visão Geral
Space Shooter é um jogo de nave espacial no estilo shoot 'em up desenvolvido em C++ utilizando a biblioteca Raylib. O jogador controla uma nave em combate contra hordas de inimigos com diferentes padrões de ataque, coletando power-ups para obter vantagens temporárias.

Características Principais
Gênero: Shoot 'em up / Bullet Hell

Motor: Raylib

Linguagem: C++

Plataforma: Multiplataforma (Windows, Linux, macOS)

Perspectiva: 2D Top-down

⚡ Funcionalidades
🎮 Mecânicas de Jogo
Sistema de Movimento: 8 direções com física suave

Sistema de Tiros: Múltiplos tipos (normal, duplo, triplo, homing)

Sistema de Dash: Movimento rápido com cooldown

Sistema de Vidas: 3 vidas iniciais com indicador visual

Power-ups: 14 tipos com efeitos temporários

Clones: Duplicatas temporárias da nave do jogador

👾 Inimigos
Tipo	Características	Dificuldade
Básico	Tiro simples, movimento linear	⭐☆☆☆☆
Duplo	2 tiros paralelos, velocidade média	⭐⭐☆☆☆
Triplo	3 tiros em leque, vida média	⭐⭐⭐☆☆
Espalhado	5 tiros em leque amplo, tanque	⭐⭐⭐⭐☆

✨ Power-ups
Categoria	Power-ups	Efeito
Ofensivos	Double Shot, Triple Shot, Explosive Shot, Homing Missile	Melhoram armamento
Defensivos	Shield, Extra Life, Regeneration	Proteção e cura
Movimento	Dash, Speed Boost	Mobilidade
Estatísticas	Damage Boost, Fire Rate, Score Boost	Bônus temporários
Especiais	Auto Fire, Clone	Habilidades únicas

🎮 Controles
Teclado
Ação	Tecla Principal	Tecla Alternativa
Movimento para Cima	W	↑
Movimento para Baixo	S	↓
Movimento para Esquerda	A	←
Movimento para Direita	D	→
Atirar	SPACE	-
Pausar	P	-
Reiniciar (Game Over)	R	-
Menu Principal	ENTER	-
Seleção de Skin	← →	-
Voltar ao Menu	BACKSPACE	-
Navegação no Menu
Seleção de Skin: Use as setas ← → para escolher entre as naves disponíveis

Confirmação: ENTER para confirmar seleção e iniciar jogo

Voltar: BACKSPACE para retornar à seleção de skins

🏗️ Sistema de Jogo
Progressão de Dificuldade
Sistema de Waves: A dificuldade aumenta automaticamente a cada 30 segundos

Spawn Adaptativo: Quantidade de inimigos ajustada conforme o tempo de jogo

Inimigos Progressivos: Tipos mais difíceis aparecem conforme a wave avança

Sistema de Pontuação
Ação	Pontuação Base	Bônus
Inimigo Básico	100 pontos	× Multiplicador
Inimigo Duplo	150 pontos	× Multiplicador
Inimigo Triplo	200 pontos	× Multiplicador
Inimigo Espalhado	250 pontos	× Multiplicador
Estados do Jogo
MENU - Tela inicial com seleção de skins

PLAYING - Jogo ativo

PAUSE - Jogo pausado

GAME_OVER - Fim de jogo com opções

📁 Estrutura do Projeto
text
space-shooter/
├── main.cpp                 # Ponto de entrada do programa
├── gamemanager.h/cpp        # Gerenciador principal do jogo
├── player.h/cpp             # Sistema do jogador e clones
├── enemy.h/cpp              # Inimigos e comportamentos
├── bullet.h/cpp             # Sistema de projéteis
├── powerup.h/cpp            # Power-ups e efeitos
└── assets/                  # Recursos do jogo
    ├── background.png       # Fundo do jogo
    ├── player.png           # Nave do jogador (Skin 1)
    ├── player2.png          # Nave do jogador (Skin 2)
    ├── inimigo1.png         # Inimigo básico
    ├── inimigo2.png         # Inimigo duplo
    ├── inimigo3.png         # Inimigo triplo
    ├── inimigo4.png         # Inimigo espalhado
    └── background_music.mp3 # Trilha sonora

🔨 Compilação
Pré-requisitos
Compiler: GCC, Clang ou MSVC

Raylib: Biblioteca instalada e configurada

Sistema: Windows, Linux ou macOS

Comando de Compilação
bash
# Linux/macOS
g++ -o space_shooter main.cpp gamemanager.cpp player.cpp enemy.cpp bullet.cpp powerup.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Windows (MinGW)
g++ -o space_shooter main.cpp gamemanager.cpp player.cpp enemy.cpp bullet.cpp powerup.cpp -lraylib -lgdi32 -lwinmm

# Windows (Visual Studio)
# Configurar projeto com link para raylib.lib
Compilação com Makefile (Opcional)
makefile
CXX = g++
CXXFLAGS = -Wall -std=c++11
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
TARGET = space_shooter
SOURCES = main.cpp gamemanager.cpp player.cpp enemy.cpp bullet.cpp powerup.cpp

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES) $(LIBS)

clean:
	rm -f $(TARGET)

🎨 Assets
Especificações Técnicas
Sprites: 64×64 pixels, formato PNG com transparência

Background: 1000×700 pixels, formato PNG

Áudio: MP3, OGG ou WAV para trilha sonora

Estrutura de Arquivos
Certifique-se de que a pasta assets/ contenha:

Todos os sprites nas dimensões corretas

Arquivo de áudio nomeado como background_music.mp3

Background com o nome background.png

🐛 Solução de Problemas
Problemas Comuns

Texturas não carregam: Confirme caminhos na pasta assets/

Compilação falha: Verifique instalação da Raylib

Verifique o console para mensagens de erro

👥 Desenvolvimento
Práticas Implementadas
✅ Código modular e orientado a objetos

✅ Gerenciamento adequado de memória

✅ Separação de responsabilidades

✅ Sistema de estados robusto

✅ Colisões otimizadas

Possíveis Expansões
Sistema de save/load

Leaderboards online

Novos tipos de inimigos

Fases com chefes

Efeitos visuais avançados

 2025 - Desenvolvido com C++ e Raylib 