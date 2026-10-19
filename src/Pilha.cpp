#include <new>
#include "Pilha.hpp"

struct PilhaNodo {
    PilhaNodo *prox;
    Ingrediente dado;
};

void inicializa(PilhaEnc *p) {
    p->topo = nullptr;
}

void limpar(PilhaEnc *p) {
    Ingrediente descartado;
    while (pop(p, &descartado)) {
    }
}

int estaVazia(const PilhaEnc *p) {
    return p->topo == nullptr;
}

int push(PilhaEnc *p, Ingrediente valor) {
    PilhaNodo *novo = new (std::nothrow) PilhaNodo;
    if (novo == nullptr)
        return 0;

    novo->dado = valor;
    novo->prox = p->topo; // aponta para o antigo topo
    p->topo = novo;        // o novo nodo passa a ser o topo
    return 1;
}

int pop(PilhaEnc *p, Ingrediente *valorRemovido) {
    PilhaNodo *removido;

    if (estaVazia(p)) // underflow
        return 0;

    removido = p->topo;
    *valorRemovido = removido->dado;
    p->topo = removido->prox;
    delete removido;
    return 1;
}

int topo(const PilhaEnc *p, Ingrediente *valor) {
    if (estaVazia(p))
        return 0;

    *valor = p->topo->dado;
    return 1;
}

int tamanho(const PilhaEnc *p) {
    PilhaNodo *aux = p->topo;
    int contador = 0;

    while (aux != nullptr) {
        contador++;
        aux = aux->prox;
    }

    return contador;
}

int obterNaPosicao(const PilhaEnc *p, int posicao, Ingrediente *valor) {
    if (p == nullptr || valor == nullptr || posicao < 0)
        return 0;

    PilhaNodo *aux = p->topo;
    while (aux != nullptr && posicao > 0) {
        aux = aux->prox;
        posicao--;
    }

    if (aux == nullptr)
        return 0;

    *valor = aux->dado;
    return 1;
}
