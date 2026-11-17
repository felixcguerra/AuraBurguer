#include "menu.hpp"
#include "fase.hpp"
#include "hud.hpp"
#include "leitura.hpp"
#include "AVL.hpp"
#include "hash.hpp"
#include "historico.hpp"
#include "telaHistorico.hpp"
#include "filaprioridade.hpp"
#include "medicoes.hpp"
#include "timer.hpp"
#include "raylib.h"
#include <cstdio>

// Função auxiliar para calcular altura da AVL (vazia = 0, apenas raiz = 1)
static int obterAlturaAVL(NodoAVL *p) {
    if (p == nullptr) return 0;
    int altEsq = obterAlturaAVL(p->esq);
    int altDir = obterAlturaAVL(p->dir);
    return 1 + (altEsq > altDir ? altEsq : altDir);
}

static int adicionarIngredienteAoPedido(Fase *fase, char tipo) {
    if (fase == nullptr) return 0;

    Ingrediente ingredienteTopo = {};
    if (topo(&fase->pedidoAtual, &ingredienteTopo) && ingredienteTopo.tipo[0] == 'z') {
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

static int prepararOuAdicionarIngredienteHeap(Fase *fase, char tipo, FilaPrioridade *alimento) {
    int min = fpMinimo(alimento);

    if (min == -1) {
        fpInserir(alimento, fase->TurnoAtual);
        return 1;
    }
    if (min + 3 > fase->TurnoAtual) {
        fpInserir(alimento, fase->TurnoAtual);
        return 1; 
    }
    if (adicionarIngredienteAoPedido(fase, tipo)) {
        fpRemoverMinimo(alimento);
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Uso: %s <pedidos> <historico>\n", argv[0]);
        return 1;
    }

    // Inicializa o log de medições
    iniciarMedicoes();

    // Cronometra o carregamento da AVL
    Timer tCargaAVL;
    timer_iniciar(&tCargaAVL);

    FILE *arqPedidos = abrirArquivoLeitura(argv[1]);
    if (arqPedidos == nullptr) {
        fprintf(stderr, "Erro: nao foi possivel abrir %s\n", argv[1]);
        return 1;
    }

    NodoAVL *raiz = nullptr;
    int pedidosOk = carregarArquivo(arqPedidos, &raiz);
    fclose(arqPedidos);

    double tempoCarga = timer_parar(&tCargaAVL);

    if (!pedidosOk) {
        fprintf(stderr, "Erro: arquivo de pedidos invalido (%s)\n", argv[1]);
        liberarAVL(raiz);
        return 1;
    }

    // Cache de pedidos (Tabela Hash)
    TabelaHash cache;
    iniciarHash(&cache, 1009);

    // Carregamento do histórico
    const char *arqHistorico = argv[2];
    ListaContigua historico;
    iniciarLista(&historico);
    if (!carregarHistorico(&historico, arqHistorico))
        fprintf(stderr, "Aviso: formato invalido em %s.\n", arqHistorico);

    // Registra as métricas iniciais (5.1) no log
    int alturaAVL = obterAlturaAVL(raiz);
    registrarMetricasCarga(alturaAVL, tempoCarga, historico.tam);

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
                    if (iniciarfase(&fase, raiz, &cache)) {
                        estadoAtual = STATE_NOVO_JOGO;
                    } else {
                        TraceLog(LOG_ERROR, "Falha ao inicializar a fase.");
                    }
                } else if (acao == MENU_ACAO_COMO_JOGAR) {
                    estadoRetornoAjuda = STATE_MENU;
                    estadoAtual = STATE_COMO_JOGAR;
                } else if (acao == MENU_ACAO_HISTORICO) {
                    abrirTelaHistorico(&historico);
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
                    case KEY_Z: turnoConcluido = adicionarIngredienteAoPedido(&fase, 'z'); break;
                    case KEY_K: turnoConcluido = adicionarIngredienteAoPedido(&fase, 'k'); break;
                    case KEY_M: turnoConcluido = adicionarIngredienteAoPedido(&fase, 'm'); break;
                    case KEY_P: turnoConcluido = adicionarIngredienteAoPedido(&fase, 'p'); break;
                    case KEY_A: turnoConcluido = adicionarIngredienteAoPedido(&fase, 'a'); break;
                    case KEY_S: turnoConcluido = prepararOuAdicionarIngrediente(&fase, 's', &fase.PreparadoSuiço); break;
                    case KEY_Q: turnoConcluido = prepararOuAdicionarIngrediente(&fase, 'q', &fase.PreparadoCheddar); break;
                    case KEY_C: turnoConcluido = prepararOuAdicionarIngrediente(&fase, 'c', &fase.PreparadoCebola); break;
                    case KEY_T: turnoConcluido = prepararOuAdicionarIngrediente(&fase, 't', &fase.PreparadoTomate); break;
                    case KEY_N: turnoConcluido = prepararOuAdicionarIngredienteHeap(&fase, 'n', &fase.Carne); break;
                    case KEY_O: turnoConcluido = prepararOuAdicionarIngredienteHeap(&fase, 'o', &fase.Ovo); break;
                    case KEY_B: turnoConcluido = prepararOuAdicionarIngredienteHeap(&fase, 'b', &fase.Bacon); break;
                    case KEY_BACKSPACE:
                        if (!estaVazia(&fase.pedidoAtual)){
                            turnoConcluido = true;
                            Ingrediente ingredienteRemovido;
                            pop(&fase.pedidoAtual, &ingredienteRemovido);
                        }
                        break;
                    default: break;
                }

                verificarfp(&fase.Carne, fase.TurnoAtual);
                verificarfp(&fase.Ovo, fase.TurnoAtual);
                verificarfp(&fase.Bacon, fase.TurnoAtual);

                if (turnoConcluido) {
                    estadoAtual = atualizarfase(&fase);
                    if (estadoAtual == STATE_VITORIA || estadoAtual == STATE_DERROTA)
                        iniciarTelaFim();
                }

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
                if (atualizarDesenharTelaHistorico()) {
                    estadoAtual = STATE_MENU;
                }
                break;

            case STATE_DERROTA:
            case STATE_VITORIA: {
                int pontuacao = fase.NumDePedidosIni - fase.NumDePedidos;
                if (atualizarDesenharTelaFim(estadoAtual == STATE_VITORIA, pontuacao, &historico, arqHistorico)) {
                    estadoAtual = STATE_MENU;
                }
                break;
            }
        }

        EndDrawing();
    }

    // Grava o relatório da sessão ao encerrar
    atualizarDadosHash(cache.n, cache.m);
    gerarEGravarRelatorioDesempenho();

    limpar(&fase.pedidos);
    limpar(&fase.pedidoAtual);
    liberarAVL(raiz);
    liberarHash(&cache);
    fecharTelaHistorico();
    liberarLista(&historico);

    CloseWindow();
    return 0;
}