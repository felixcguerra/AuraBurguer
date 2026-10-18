#include "hud.hpp"
void desenharHud(const Jogador *jogador, int pontuacao, int faseAtual) {
    Color energiaExtraColor = (Color){ 60, 110, 130, 200 };  
   for (int i = 0; i < jogador->vidas; i++) {
        DrawRectangle(10 + i * 30, 10, 20, 20, RED);
    }

    DrawRectangle(10, 40, 200, 15, DARKGRAY);
   
    DrawRectangle(10, 40, (int)(200.0f * jogador->energia / 100), 15, GOLD);
    if (jogador->energia > jogador->energiaBase) {
        DrawRectangle(10 + (int)(200.0f * jogador->energiaMax / 100), 40, (int)(200.0f * (jogador->energia - jogador->energiaMax) / 100), 15, energiaExtraColor);
    }
    DrawText(TextFormat("Pontos: %d", pontuacao), 10, 65, 20, WHITE);
    DrawText(TextFormat("Fase: %d", faseAtual), 10, 90, 20, WHITE);
    DrawText(TextFormat("Dano: %d", jogador->dano), 10, 115, 20, WHITE);
}
