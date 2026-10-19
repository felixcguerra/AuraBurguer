#ifndef PILHA_HPP
#define PILHA_HPP

#include "ListaEnc.hpp"

struct PilhaNodo;

typedef struct {
    PilhaNodo *topo;
} PilhaEnc;

void inicializa(PilhaEnc *p);
void limpar(PilhaEnc *p);
int estaVazia(const PilhaEnc *p);
int push(PilhaEnc *p, Ingrediente valor);
int pop(PilhaEnc *p, Ingrediente *valorRemovido);
int topo(const PilhaEnc *p, Ingrediente *valor);
int tamanho(const PilhaEnc *p);
int obterNaPosicao(const PilhaEnc *p, int posicao, Ingrediente *valor);

#endif