#ifndef FILA_HPP
#define FILA_HPP

#include "ListaEnc.hpp"

// Declaração antecipada: o nodo da fila é definido só em Fila.cpp,
// escondendo os detalhes internos de quem usa a fila.
struct FilaNodo;

// Um pedido de cliente: código identificador e a receita,
// guardada como lista encadeada de ingredientes (de baixo para cima).
typedef struct {
    int cod;
    ListaEnc ingredientes;
} Pedido;

// Fila encadeada (FIFO) de pedidos: guarda ponteiros para o primeiro
// e para o último nodo.
typedef struct {
    FilaNodo *frente;
    FilaNodo *final;
} FilaEnc;

// Deixa a fila vazia.
void inicializa(FilaEnc *f);
// Esvazia a fila, liberando os nodos e as listas de ingredientes dos pedidos.
void limpar(FilaEnc *f);
// Retorna 1 se a fila está vazia, 0 caso contrário.
int estaVazia(const FilaEnc *f);
// Insere um pedido no final da fila. Retorna 1 em sucesso, 0 em falha.
int enqueue(FilaEnc *f, Pedido valor);
// Remove o pedido da frente e o devolve em *valorRemovido. Retorna 0 se a fila estiver vazia.
int dequeue(FilaEnc *f, Pedido *valorRemovido);
// Consulta o pedido da frente sem removê-lo. Retorna 0 se a fila estiver vazia.
int frente(const FilaEnc *f, Pedido *valor);
// Retorna o número de pedidos na fila.
int tamanho(const FilaEnc *f);

#endif