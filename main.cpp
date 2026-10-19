#include "menu.hpp"
#include "fase.hpp"
#include "hud.hpp"
#include "raylib.h"

static int adicionarIngredienteAoPedido(Fase *fase, char tipo) {
    if (fase == nullptr)
        return 0;

    Ingrediente ingredienteTopo = {};
    if (topo(&fase->pedidoAtual, &ingredienteTopo) &&
        ingredienteTopo.tipo[0] == 'z') {
        return 0;
    }

    Ingrediente ingrediente = {};
    ingrediente.tipo[0] = tipo;
    ingrediente.tipo[1] = '\0';
    ingrediente.estado = 0;

    if (!push(&fase->pedidoAtual, ingrediente)) {
        TraceLog(LOG_ERROR, "Falha ao adicionar ingrediente ao pedido atual.");
        return 0;
    }

    return 1;
}

static int prepararOuAdicionarIngrediente(Fase *fase, char tipo, int *preparado) {
    if (*preparado == 0) {
        *preparado = 1;
        return 1;
    }

    if (adicionarIngredienteAoPedido(fase, tipo)) {
        *preparado = 0;
        return 1;
    }
    return 0;
}

int main(void) {
    //faz a leitura da lista de pedidos
    Fase fase;
    const int larguraJanela = 1200;
    const int alturaJanela = 720;
    GameState estadoAtual = STATE_MENU;
    GameState estadoRetornoAjuda = STATE_MENU;
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
                    if (iniciarfase(&fase)) {
                        estadoAtual = STATE_NOVO_JOGO;
                    } else {
                        TraceLog(LOG_ERROR, "Falha ao inicializar a fase.");
                    }
                } else if (acao == MENU_ACAO_COMO_JOGAR) {
                    estadoAtual = STATE_COMO_JOGAR;
                } else if (acao == MENU_ACAO_HISTORICO) {
                    estadoAtual = STATE_HISTORICO;
                } else if (acao == MENU_ACAO_SAIR) {
                    executando = false;
                }
                break;
            }

            case STATE_NOVO_JOGO: {
                DrawText("Cozinha", 480, 100, 42, WHITE);

                if (IsKeyPressed(KEY_ESCAPE)) {
                    estadoAtual = STATE_PAUSE;
                }

                bool turnoConcluido = false;
                switch (GetKeyPressed()) {
                    case KEY_Z:
                        turnoConcluido = adicionarIngredienteAoPedido(&fase, 'z');
                        break;
                    case KEY_K:
                        turnoConcluido = adicionarIngredienteAoPedido(&fase, 'k');
                        break;
                    case KEY_M:
                        turnoConcluido = adicionarIngredienteAoPedido(&fase, 'm');
                        break;
                    case KEY_P:
                        turnoConcluido = adicionarIngredienteAoPedido(&fase, 'p');
                        break;
                    case KEY_A:
                        turnoConcluido = adicionarIngredienteAoPedido(&fase, 'a');
                        break;
                    case KEY_S:
                        turnoConcluido = prepararOuAdicionarIngrediente(&fase, 's', &fase.PreparadoSuiço);
                        break;
                    case KEY_Q:
                        turnoConcluido = prepararOuAdicionarIngrediente(&fase, 'q', &fase.PreparadoCheddar);
                        break;
                    case KEY_C:
                        turnoConcluido = prepararOuAdicionarIngrediente(&fase, 'c', &fase.PreparadoCebola);
                        break;
                    case KEY_T:
                        turnoConcluido = prepararOuAdicionarIngrediente(&fase, 't', &fase.PreparadoTomate);
                        break;
                    case KEY_BACKSPACE:
                        if (!estaVazia(&fase.pedidoAtual)){
                            turnoConcluido = true;
                            Ingrediente ingredienteRemovido;
                            pop(&fase.pedidoAtual, &ingredienteRemovido);
                        }
                }
                if (turnoConcluido)
                    estadoAtual=atualizarfase(&fase);

                desenharHud(&fase);
                DesenharHamburger(&fase.pedidoAtual, 600, 440);
                DesenharPedido(&fase.pedidos, 900, 440);
                break;
            }

            case STATE_PAUSE: {
                MenuPauseAcao acao = atualizarDesenharMenuPause();
                if (acao == PAUSE_ACAO_CONTINUAR) {
                    estadoAtual = STATE_NOVO_JOGO;
                } else if (acao == PAUSE_ACAO_COMO_JOGAR) {
                    estadoRetornoAjuda = STATE_PAUSE;
                    estadoAtual = STATE_COMO_JOGAR;
                } else if (acao == PAUSE_ACAO_SAIR_MENU) {
                    estadoAtual = STATE_MENU;
                }
                break;
            }

            case STATE_COMO_JOGAR:
                desenharTelaAjuda();
                if (IsKeyPressed(KEY_ESCAPE)) {
                    estadoAtual = estadoRetornoAjuda;
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
            case STATE_DERROTA:
                DrawText("DERROTA", 480, 245, 42, WHITE);
                DrawText("Voce perdeu o jogo.", 340, 320, 24, LIGHTGRAY);
                DrawText("Pressione ESC para voltar ao menu", 370, 390, 24, LIGHTGRAY);
                if (IsKeyPressed(KEY_ESCAPE)) {
                    estadoAtual = STATE_MENU;
                }
                break;
            case STATE_VITORIA:
                DrawText("VITORIA", 480, 245, 42, WHITE);
                DrawText("Voce venceu o jogo.", 340, 320, 24, LIGHTGRAY);
                DrawText("Pressione ESC para voltar ao menu", 370, 390, 24, LIGHTGRAY);
                if (IsKeyPressed(KEY_ESCAPE)) {
                    estadoAtual = STATE_MENU;
                }
                break;
        }

        EndDrawing();
    }

    limpar(&fase.pedidos);
    limpar(&fase.pedidoAtual);
    CloseWindow();
    return 0;
}
