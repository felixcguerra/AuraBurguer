#include "hud.hpp"
#include "raylib.h"

void DesenharHamburger(const PilhaEnc *pilha, int posX, int posY) {
    if (pilha == nullptr)
        return;

    const int fonte = 20;
    const int espacamento = 25;
    const int quantidade = tamanho(pilha);
    int y = posY - ((quantidade + 1) * espacamento) ;

    for (int i = 0; i < quantidade; i++) {
        Ingrediente ingrediente = {};
        if (!obterNaPosicao(pilha, i, &ingrediente)) {
            TraceLog(LOG_ERROR, "Falha ao ler a pilha do hamburger para desenhar.");
            return;
        }

        DrawText(ingrediente.tipo, posX ,
                 y, fonte, WHITE);
        y += espacamento;
    }

    DrawText("z", posX, y, fonte, WHITE);
}

void DesenharPedido(const FilaEnc *fila, int posX, int posY) {
    if (fila == nullptr)
        return;

    Pedido pedido = {};
    if (!frente(fila, &pedido))
        return;

    const int fonte = 20;
    const int espacamento = 25;
    const int quantidade = tamanho(&pedido.ingredientes);
    int y = posY - espacamento;

    int i = 0;
    DrawText("z", posX, y, fonte, WHITE);
    Nodo *nodo = pedido.ingredientes.ini;
    while (nodo != nullptr) {
        y -= espacamento;
        DrawText(nodo->ingrediente.tipo, posX - MeasureText(nodo->ingrediente.tipo, fonte),
                 y, fonte, WHITE);
        nodo = nodo->prox;
        
    }
     
}
void desenharHud(const Fase *fase) {
    DrawText(TextFormat("Fase: %d", fase->cod), 10, 90, 20, WHITE);

    DrawText(TextFormat("Turno: %d", fase->TurnoAtual), 10, 145, 20, WHITE);
    DrawText(TextFormat("Pedidos: %d", fase->NumDePedidos), 10, 170, 20, WHITE);
    DrawText(TextFormat("Paciencia do cliente atual: %d", fase->PacienciaDoClienteAtual), 10, 195, 20, WHITE);

    DrawText("Ingredientes preparados:", 10, 230, 20, WHITE);
    DrawText(TextFormat("Suico: %d", fase->PreparadoSuiço), 10, 255, 20, WHITE);
    DrawText(TextFormat("Cheddar: %d", fase->PreparadoCheddar), 10, 280, 20, WHITE);
    DrawText(TextFormat("Cebola: %d", fase->PreparadoCebola), 10, 305, 20, WHITE);
    DrawText(TextFormat("Tomate: %d", fase->PreparadoTomate), 10, 330, 20, WHITE);
    DrawText(TextFormat("Bacon: %d", fase->Bacon.tam), 10, 355, 20, WHITE);
    DrawText(TextFormat("Carne: %d", fase->Carne.tam), 10, 380, 20, WHITE);
    DrawText(TextFormat("Ovo: %d", fase->Ovo.tam), 10, 405, 20, WHITE);
}
