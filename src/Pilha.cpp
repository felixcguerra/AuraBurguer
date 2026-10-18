#include <new>

typedef struct {
    int cod;
    char nome[50];
    float preco;
} Produto;

typedef struct str_Nodo Nodo;

struct str_Nodo {
    Nodo *prox;
    Produto dado;
};

typedef struct {
    Nodo *topo;
} PilhaEnc;
void inicializa(PilhaEnc *p) {
    p->topo = nullptr;
}

int estaVazia(const PilhaEnc *p) {
    return p->topo == nullptr;
}

int push(PilhaEnc *p, Produto valor) {
    Nodo *novo = new (std::nothrow) Nodo;
    if (novo == nullptr)
        return 0;

    novo->dado = valor;
    novo->prox = p->topo; // aponta para o antigo topo
    p->topo = novo;        // o novo nodo passa a ser o topo
    return 1;
}

int pop(PilhaEnc *p, Produto *valorRemovido) {
    Nodo *removido;

    if (estaVazia(p)) // underflow
        return 0;

    removido = p->topo;
    *valorRemovido = removido->dado;
    p->topo = removido->prox;
    delete removido;
    return 1;
}

int topo(const PilhaEnc *p, Produto *valor) {
    if (estaVazia(p))
        return 0;

    *valor = p->topo->dado;
    return 1;
}

int tamanho(const PilhaEnc *p) {
    Nodo *aux = p->topo;
    int contador = 0;

    while (aux != nullptr) {
        contador++;
        aux = aux->prox;
    }

    return contador;
}