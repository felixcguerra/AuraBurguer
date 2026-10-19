#include <new>
#include "Fila.hpp"

struct FilaNodo {
    FilaNodo *prox;
    Pedido dado;
};

void inicializa(FilaEnc *f) {
    f->frente = nullptr;
    f->final = nullptr;
}

void limpar(FilaEnc *f) {
    Pedido descartado;
    while (dequeue(f, &descartado)) {
        destruir(&descartado.ingredientes);
    }
}

int estaVazia(const FilaEnc *f) {
    return f->frente == nullptr;
}

int enqueue(FilaEnc *f, Pedido valor) {
    FilaNodo *novo = new (std::nothrow) FilaNodo;
    if (novo == nullptr)
        return 0;

    novo->dado = valor;
    novo->prox = nullptr;

    if (f->final != nullptr)
        f->final->prox = novo; // o antigo final passa a apontar para o novo nodo
    else
        f->frente = novo;      // fila estava vazia: novo nodo também é a frente

    f->final = novo;
    return 1;
}

int dequeue(FilaEnc *f, Pedido *valorRemovido) {
    FilaNodo *removido;

    if (estaVazia(f)) // underflow
        return 0;

    removido = f->frente;
    *valorRemovido = removido->dado;
    f->frente = removido->prox;

    if (f->frente == nullptr) // a fila ficou vazia
        f->final = nullptr;

    delete(removido);
    return 1;
}

int frente(const FilaEnc *f, Pedido *valor) {
    if (estaVazia(f))
        return 0;

    *valor = f->frente->dado; // copia o dado do primeiro nodo sem removê-lo
    return 1;
}

int tamanho(const FilaEnc *f) {
    FilaNodo *aux = f->frente;
    int contador = 0;

    while (aux != nullptr) {
        contador++;
        aux = aux->prox;
    }

    return contador;
}