#ifndef PILHA_HPP
#define PILHA_HPP

#include "ListaEnc.hpp"

// Declaração antecipada: o nodo da pilha é definido só em Pilha.cpp,
// escondendo os detalhes internos de quem usa a pilha.
struct PilhaNodo;

// Pilha encadeada (LIFO) de ingredientes: guarda apenas o ponteiro para o topo.
// É usada para montar o hambúrguer do jogador.
typedef struct {
    PilhaNodo *topo;
} PilhaEnc;

// Deixa a pilha vazia.
void inicializa(PilhaEnc *p);
// Esvazia a pilha, liberando todos os nodos.
void limpar(PilhaEnc *p);
// Retorna 1 se a pilha está vazia, 0 caso contrário.
int estaVazia(const PilhaEnc *p);
// Empilha um ingrediente no topo. Retorna 1 em sucesso, 0 em falha.
int push(PilhaEnc *p, Ingrediente valor);
// Desempilha o ingrediente do topo e o devolve em *valorRemovido. Retorna 0 se a pilha estiver vazia.
int pop(PilhaEnc *p, Ingrediente *valorRemovido);
// Consulta o ingrediente do topo sem removê-lo. Retorna 0 se a pilha estiver vazia.
int topo(const PilhaEnc *p, Ingrediente *valor);
// Retorna o número de elementos da pilha.
int tamanho(const PilhaEnc *p);
// Lê o elemento de uma posição sem desempilhar (0 = topo). Usada para comparar
// o hambúrguer com a receita sem destruir a pilha.
int obterNaPosicao(const PilhaEnc *p, int posicao, Ingrediente *valor);

#endif