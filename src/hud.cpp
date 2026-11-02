#include "hud.hpp"
#include "raylib.h"

// Desenha na tela o hambúrguer que o jogador está montando.
// (posX, posY) é o ponto de referência onde a pilha é desenhada;
// os ingredientes são empilhados visualmente de baixo para cima.
void DesenharHamburger(const PilhaEnc *pilha, int posX, int posY) {
    if (pilha == nullptr)
        return;

    const int fonte = 20;        // tamanho da fonte dos símbolos
    const int espacamento = 25;  // distância vertical entre ingredientes
    const int quantidade = tamanho(pilha);
    // Ponto inicial do desenho, subindo conforme a quantidade de ingredientes
    int y = posY - ((quantidade + 1) * espacamento) ;

    for (int i = 0; i < quantidade; i++) {
        Ingrediente ingrediente = {};
        // Se a leitura falhar, registra o erro no log do raylib e interrompe o desenho
        if (!obterNaPosicao(pilha, i, &ingrediente)) {
            TraceLog(LOG_ERROR, "Falha ao ler a pilha do hamburger para desenhar.");
            return;
        }

        DrawText(ingrediente.tipo, posX ,
                 y, fonte, WHITE);
        y += espacamento;
    }

    // O pão ("z") é desenhado por último, na posição final
    DrawText("z", posX, y, fonte, WHITE);
}

// Desenha o pedido do cliente que está na frente da fila,
// mostrando os ingredientes exigidos na ordem da receita.
void DesenharPedido(const FilaEnc *fila, int posX, int posY) {
    if (fila == nullptr)
        return;

    Pedido pedido = {};
    // Se a fila está vazia, não há pedido para mostrar
    if (!frente(fila, &pedido))
        return;

    const int fonte = 20;
    const int espacamento = 25;
    const int quantidade = tamanho(&pedido.ingredientes);
    int y = posY - espacamento;

    int i = 0;
    // O pão de baixo fica na base do desenho
    DrawText("z", posX, y, fonte, WHITE);
    // Percorre a receita e desenha cada ingrediente uma linha acima do anterior;
    // o texto é alinhado à direita de posX usando a largura medida do símbolo
    Nodo *nodo = pedido.ingredientes.ini;
    while (nodo != nullptr) {
        y -= espacamento;
        DrawText(nodo->ingrediente.tipo, posX - MeasureText(nodo->ingrediente.tipo, fonte),
                 y, fonte, WHITE);
        nodo = nodo->prox;
        
    }
     
}

// Desenha o HUD (painel de informações) com os dados atuais da fase:
// fase, turno, pedidos restantes, paciência do cliente e estoque de ingredientes.
void desenharHud(const Fase *fase) {
    DrawText(TextFormat("Fase: %d", fase->cod), 10, 90, 20, WHITE);

    // Informações da partida
    DrawText(TextFormat("Turno: %d", fase->TurnoAtual), 10, 145, 20, WHITE);
    DrawText(TextFormat("Pedidos: %d", fase->NumDePedidos), 10, 170, 20, WHITE);
    DrawText(TextFormat("Paciencia do cliente atual: %d", fase->PacienciaDoClienteAtual), 10, 195, 20, WHITE);

    // Estoque: ingredientes preparados (perenes) e quantidade de cada perecível
    DrawText("Ingredientes preparados:", 10, 230, 20, WHITE);
    DrawText(TextFormat("Suico: %d", fase->PreparadoSuiço), 10, 255, 20, WHITE);
    DrawText(TextFormat("Cheddar: %d", fase->PreparadoCheddar), 10, 280, 20, WHITE);
    DrawText(TextFormat("Cebola: %d", fase->PreparadoCebola), 10, 305, 20, WHITE);
    DrawText(TextFormat("Tomate: %d", fase->PreparadoTomate), 10, 330, 20, WHITE);

    //todo
    DrawText(TextFormat("Bacon: %d", fase->Bacon.tam), 10, 355, 20, WHITE);
    DrawText(TextFormat("Carne: %d", fase->Carne.tam), 10, 380, 20, WHITE);
    DrawText(TextFormat("Ovo: %d", fase->Ovo.tam), 10, 405, 20, WHITE);
}