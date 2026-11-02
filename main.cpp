#include "menu.hpp"
#include "fase.hpp"
#include "hud.hpp"
#include "leitura.hpp"
#include "AVL.hpp"
#include "hash.hpp"
#include "filaprioridade.hpp"
#include "raylib.h"
#include <cstdio>

// Empilha um ingrediente no hambúrguer que o jogador está montando.
// Retorna 1 se o ingrediente foi adicionado e 0 se a ação não pôde ser feita
// (fase inválida, pão de cima já colocado ou falha de alocação).
static int adicionarIngredienteAoPedido(Fase *fase, char tipo) {
    if (fase == nullptr)
        return 0;

    // Regra do jogo: não é possível adicionar nada por cima do pão de cima ('z')
    Ingrediente ingredienteTopo = {};
    if (topo(&fase->pedidoAtual, &ingredienteTopo) &&
        ingredienteTopo.tipo[0] == 'z') {
        return 0;
    }

    // Monta o ingrediente com tipo de 1 letra
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

// Para ingredientes que exigem preparo: a primeira vez que a tecla é apertada
// o ingrediente é preparado (*preparado passa a 1); na vez seguinte ele é
// adicionado ao hambúrguer (e *preparado volta a 0).
// Retorna 1 se a ação consumiu um turno e 0 caso contrário.
static int prepararOuAdicionarIngrediente(Fase *fase, char tipo, int *preparado) {
    // Ainda não preparado: esta ação é o preparo
    if (*preparado == 0) {
        *preparado = 1;

        return 1;
    }

    // Já preparado: esta ação é adicioná-lo ao hambúrguer
    if (adicionarIngredienteAoPedido(fase, tipo)) {
        *preparado = 0;
        return 1;
    }
    return 0;
}

static int prepararOuAdicionarIngredienteHeap(Fase *fase, char tipo, FilaPrioridade *alimento) {//TODO INCREMENTAR para tirar comida
    
    int min=fpMinimo(alimento);

    if(min==-1){
        fpInserir(alimento,fase->TurnoAtual);
        TraceLog(LOG_ERROR, "vazio");
        return 1;
        
    }
    if(min+3 > fase->TurnoAtual){
        fpInserir(alimento,fase->TurnoAtual);
        TraceLog(LOG_ERROR, "pronto");
        return 1; 
    }
    if (adicionarIngredienteAoPedido(fase, tipo)) {
        fpRemoverMinimo(alimento);
        TraceLog(LOG_ERROR, "sla");
        return 1;
    }

    return 0;
}

// Função principal: abre a janela do raylib e roda o loop do jogo.
// O jogo funciona como uma máquina de estados (variável estadoAtual); a cada
// quadro, o 'switch' desenha e trata a tela correspondente ao estado atual.
int main(int argc, char *argv[]) {
    // O programa recebe os nomes dos arquivos pela linha de comando:
    // argv[1] = arquivo de pedidos, argv[2] = arquivo de histórico
    if (argc < 3) {
        fprintf(stderr, "Uso: %s <pedidos> <historico>\n", argv[0]);
        return 1;
    }

    // Carregamento dos pedidos (antes de abrir a janela e exibir o menu):
    // lê o arquivo e monta a AVL indexada pelo código do pedido.
    FILE *arqPedidos = abrirArquivoLeitura(argv[1]);
    if (arqPedidos == nullptr) {
        fprintf(stderr, "Erro: nao foi possivel abrir %s\n", argv[1]);
        return 1;
    }

    NodoAVL *raiz = nullptr;
    int pedidosOk = carregarArquivo(arqPedidos, &raiz);
    fclose(arqPedidos); // o arquivo não é mais necessário depois da carga
    if (!pedidosOk) {
        fprintf(stderr, "Erro: arquivo de pedidos invalido (%s)\n", argv[1]);
        liberarAVL(raiz);
        return 1;
    }

    // Cache de pedidos: tabela hash consultada antes da AVL (ver sortearPedido em fase.cpp)
    TabelaHash cache;
    iniciarHash(&cache, 1009);

    // Carregamento do histórico. Na primeira execução o arquivo pode não
    // existir: nesse caso o histórico simplesmente começa vazio.
    // TODO (seu): abrir argv[2] com abrirArquivoLeitura, ler com
    // lerProximoRegistroHistorico e acrescentar (nome, pontuacao) na lista contígua.

    Fase fase;
    const int larguraJanela = 1200;
    const int alturaJanela = 720;
    GameState estadoAtual = STATE_MENU;
    GameState estadoRetornoAjuda = STATE_MENU; // para onde voltar ao sair da tela de ajuda
    bool executando = true;

    InitWindow(larguraJanela, alturaJanela, "Burguer");
    SetTargetFPS(60);
    SetExitKey(0); // desativa o ESC como tecla de saída padrão do raylib (ESC é usado no jogo)

    // Loop principal: roda até o jogador sair ou fechar a janela
    while (executando && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        switch (estadoAtual) {
            // Menu principal: inicia o jogo, abre a ajuda ou o histórico, ou sai
            case STATE_MENU: {
                MenuPrincipalAcao acao = atualizarDesenharMenuPrincipal();
                if (acao == MENU_ACAO_NOVO_JOGO) {
                    // Só entra no jogo se a fase for inicializada com sucesso
                    if (iniciarfase(&fase, raiz, &cache)) {
                        estadoAtual = STATE_NOVO_JOGO;
                    } else {
                        TraceLog(LOG_ERROR, "Falha ao inicializar a fase.");
                    }
                } else if (acao == MENU_ACAO_COMO_JOGAR) {
                    estadoRetornoAjuda = STATE_MENU; // a ajuda deve voltar ao menu, não à pausa
                    estadoAtual = STATE_COMO_JOGAR;
                } else if (acao == MENU_ACAO_HISTORICO) {
                    estadoAtual = STATE_HISTORICO;
                } else if (acao == MENU_ACAO_SAIR) {
                    executando = false;
                }
                break;
            }

            // Partida em andamento: lê as teclas, atualiza a fase e desenha a cozinha
            case STATE_NOVO_JOGO: {
                DrawText("Cozinha", 480, 100, 42, WHITE);

                // ESC abre o menu de pausa
                if (IsKeyPressed(KEY_ESCAPE)) {
                    estadoAtual = STATE_PAUSE;
                }

                // Indica se o jogador fez uma ação que consome turno neste quadro
                bool turnoConcluido = false;
                // Cada tecla corresponde a um ingrediente (símbolos da Tabela 1)
                switch (GetKeyPressed()) {
                    // Ingredientes sem preparo: são adicionados direto ao hambúrguer
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
                    // Ingredientes com preparo (perenes): preparar primeiro, adicionar depois
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


                    case KEY_N:
                        turnoConcluido = prepararOuAdicionarIngredienteHeap(&fase, 'n',&fase.Carne);
                        break;
                    case KEY_O:
                        turnoConcluido = prepararOuAdicionarIngredienteHeap(&fase, 'o', &fase.Ovo);
                        break;
                    case KEY_B:
                        turnoConcluido = prepararOuAdicionarIngredienteHeap(&fase, 'b', &fase.Bacon);
                        break;




                    // Backspace: remove o ingrediente do topo (consome turno se havia algo para remover)
                    case KEY_BACKSPACE:
                        if (!estaVazia(&fase.pedidoAtual)){
                            turnoConcluido = true;
                            Ingrediente ingredienteRemovido;
                            pop(&fase.pedidoAtual, &ingredienteRemovido);
                        }
                        break;
                    default:
                        break;
                }


                verificarfp(&fase.Carne,fase.TurnoAtual);
                verificarfp(&fase.Ovo,fase.TurnoAtual);
                verificarfp(&fase.Bacon,fase.TurnoAtual);



                // Se o jogador gastou um turno, atualiza a fase; o retorno pode
                // levar a vitória, derrota ou continuar a partida
                if (turnoConcluido)
                    estadoAtual=atualizarfase(&fase);

                // Desenha informações da fase, o hambúrguer montado e o pedido da frente da fila
                desenharHud(&fase);
                DesenharHamburger(&fase.pedidoAtual, 600, 440);
                DesenharPedido(&fase.pedidos, 900, 440);
                break;
            }

            // Menu de pausa: continuar, ver a ajuda (voltando depois ao pause) ou sair para o menu
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

            // Tela de ajuda: ESC volta para a tela de onde o jogador veio
            case STATE_COMO_JOGAR:
                desenharTelaAjuda();
                if (IsKeyPressed(KEY_ESCAPE)) {
                    estadoAtual = estadoRetornoAjuda;
                }
                break;

            // Tela de histórico: ainda é apenas um aviso (a implementação virá depois)
            case STATE_HISTORICO:
                DrawText("HISTORICO", 480, 245, 42, WHITE);
                DrawText("Esta tela sera implementada futuramente.", 340, 320, 24, LIGHTGRAY);
                DrawText("Pressione ESC para voltar ao menu", 370, 390, 24, LIGHTGRAY);
                if (IsKeyPressed(KEY_ESCAPE)) {
                    estadoAtual = STATE_MENU;
                }
                break;
            // Tela de derrota: a paciência do cliente da frente chegou a zero
            case STATE_DERROTA:
                DrawText("DERROTA", 480, 245, 42, WHITE);
                DrawText("Voce perdeu o jogo.", 340, 320, 24, LIGHTGRAY);
                DrawText("Pressione ESC para voltar ao menu", 370, 390, 24, LIGHTGRAY);
                if (IsKeyPressed(KEY_ESCAPE)) {
                    estadoAtual = STATE_MENU;
                }
                break;
            // Tela de vitória: todos os clientes foram atendidos
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

    // Ao sair: libera a fila de pedidos e o hambúrguer atual, e fecha a janela
    limpar(&fase.pedidos);
    limpar(&fase.pedidoAtual);
    liberarAVL(raiz);
    liberarHash(&cache);

    CloseWindow();
    return 0;
}