#include <new>
#include "Pilha.hpp"

// Nodo interno da pilha encadeada: guarda um Ingrediente e o ponteiro para o
// nodo que está logo abaixo dele na pilha.
struct PilhaNodo {
    PilhaNodo *prox;
    Ingrediente dado;
};

// Deixa a pilha vazia (sem topo).
void inicializa(PilhaEnc *p) {
    p->topo = nullptr;
}

// Esvazia a pilha, desempilhando todos os elementos (liberando a memória de cada nodo).
void limpar(PilhaEnc *p) {
    Ingrediente descartado;
    while (pop(p, &descartado)) {
    }
}

// Retorna 1 se a pilha não tem elementos, 0 caso contrário.
int estaVazia(const PilhaEnc *p) {
    return p->topo == nullptr;
}

// Empilha um ingrediente no topo (política LIFO: o último a entrar é o primeiro a sair).
// Retorna 1 em caso de sucesso e 0 se a alocação de memória falhar.
int push(PilhaEnc *p, Ingrediente valor) {
    // nothrow: retorna nullptr em vez de lançar exceção se faltar memória
    PilhaNodo *novo = new (std::nothrow) PilhaNodo;
    if (novo == nullptr)
        return 0;

    novo->dado = valor;
    novo->prox = p->topo; // aponta para o antigo topo
    p->topo = novo;        // o novo nodo passa a ser o topo
    return 1;
}

// Desempilha o ingrediente do topo e o devolve em *valorRemovido.
// Retorna 1 em caso de sucesso e 0 se a pilha estiver vazia (underflow).
int pop(PilhaEnc *p, Ingrediente *valorRemovido) {
    PilhaNodo *removido;

    if (estaVazia(p)) // underflow
        return 0;

    removido = p->topo;
    *valorRemovido = removido->dado;
    p->topo = removido->prox; // o elemento de baixo vira o novo topo
    delete removido;
    return 1;
}

// Consulta o ingrediente do topo sem removê-lo.
// Retorna 1 em caso de sucesso e 0 se a pilha estiver vazia.
int topo(const PilhaEnc *p, Ingrediente *valor) {
    if (estaVazia(p))
        return 0;

    *valor = p->topo->dado;
    return 1;
}

// Conta os elementos da pilha percorrendo do topo até a base (custo O(n)).
int tamanho(const PilhaEnc *p) {
    PilhaNodo *aux = p->topo;
    int contador = 0;

    while (aux != nullptr) {
        contador++;
        aux = aux->prox;
    }

    return contador;
}

// Operação de travessia adicionada ao TAD (o enunciado permite essa adaptação):
// lê o elemento na posição dada SEM desempilhar, para poder comparar o
// hambúrguer com a receita sem destruir a pilha.
// A posição 0 é o TOPO; a última posição (tamanho - 1) é a base.
// Retorna 1 em caso de sucesso e 0 se os parâmetros forem inválidos ou a posição não existir.
int obterNaPosicao(const PilhaEnc *p, int posicao, Ingrediente *valor) {
    if (p == nullptr || valor == nullptr || posicao < 0)
        return 0;

    // Desce pela pilha a partir do topo, 'posicao' vezes
    PilhaNodo *aux = p->topo;
    while (aux != nullptr && posicao > 0) {
        aux = aux->prox;
        posicao--;
    }

    if (aux == nullptr) // a posição pedida é maior que o tamanho da pilha
        return 0;

    *valor = aux->dado;
    return 1;
}