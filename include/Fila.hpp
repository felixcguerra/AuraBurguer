#ifndef FILA_HPP
#define FILA_HPP

#include "ListaEnc.hpp"

struct FilaNodo;

typedef struct {
    int cod;
    ListaEnc ingredientes;
} Pedido;

typedef struct {
    FilaNodo *frente;
    FilaNodo *final;
} FilaEnc;

void inicializa(FilaEnc *f);
void limpar(FilaEnc *f);
int estaVazia(const FilaEnc *f);
int enqueue(FilaEnc *f, Pedido valor);
int dequeue(FilaEnc *f, Pedido *valorRemovido);
int frente(const FilaEnc *f, Pedido *valor);
int tamanho(const FilaEnc *f);

#endif