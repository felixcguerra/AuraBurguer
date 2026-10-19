#ifndef HUD_HPP
#define HUD_HPP

#include "fase.hpp"

/**
 * Desenha os parametros atuais da fase.
 */
void desenharHud(const Fase *fase);
void DesenharHamburger(const PilhaEnc *pilha, int posX, int posY);
void DesenharPedido(const FilaEnc *fila, int posX, int posY);

#endif
