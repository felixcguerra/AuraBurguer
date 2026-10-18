#include "menu.hpp"
#include "raylib.h"

typedef enum {
    STATE_MENU,
    STATE_NOVO_JOGO,
    STATE_COMO_JOGAR,
    STATE_HISTORICO
} GameState;

int main(void) {
    const int larguraJanela = 1200;
    const int alturaJanela = 720;
    GameState estadoAtual = STATE_MENU;
    bool executando = true;

    InitWindow(larguraJanela, alturaJanela, "Burguer");
    SetTargetFPS(60);
    SetExitKey(0);

    while (executando && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        switch (estadoAtual) {
            case STATE_MENU: {
                MenuPrincipalAcao acao = atualizarDesenharMenuPrincipal();
                if (acao == MENU_ACAO_NOVO_JOGO) {
                    estadoAtual = STATE_NOVO_JOGO;
                } else if (acao == MENU_ACAO_COMO_JOGAR) {
                    estadoAtual = STATE_COMO_JOGAR;
                } else if (acao == MENU_ACAO_HISTORICO) {
                    estadoAtual = STATE_HISTORICO;
                } else if (acao == MENU_ACAO_SAIR) {
                    executando = false;
                }
                break;
            }

            case STATE_NOVO_JOGO:
                DrawText("NOVO JOGO", 480, 245, 42, WHITE);
                DrawText("A partida ainda nao foi implementada.", 360, 320, 24, LIGHTGRAY);
                DrawText("Pressione ESC para voltar ao menu", 370, 390, 24, LIGHTGRAY);
                if (IsKeyPressed(KEY_ESCAPE)) {
                    estadoAtual = STATE_MENU;
                }
                break;

            case STATE_COMO_JOGAR:
                desenharTelaAjuda();
                if (IsKeyPressed(KEY_ESCAPE)) {
                    estadoAtual = STATE_MENU;
                }
                break;

            case STATE_HISTORICO:
                DrawText("HISTORICO", 480, 245, 42, WHITE);
                DrawText("Esta tela sera implementada futuramente.", 340, 320, 24, LIGHTGRAY);
                DrawText("Pressione ESC para voltar ao menu", 370, 390, 24, LIGHTGRAY);
                if (IsKeyPressed(KEY_ESCAPE)) {
                    estadoAtual = STATE_MENU;
                }
                break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
