#ifndef HUD_HPP
#define HUD_HPP

#include "fase.hpp"

/**
 * Desenha os parametros atuais da fase.
 */
void desenharHud(const Fase *fase);
// Desenha o hambúrguer em montagem (pilha) a partir da posição (posX, posY).
void DesenharHamburger(const PilhaEnc *pilha, int posX, int posY);
// Desenha o pedido do cliente da frente da fila a partir da posição (posX, posY).
void DesenharPedido(const FilaEnc *fila, int posX, int posY);

#endif