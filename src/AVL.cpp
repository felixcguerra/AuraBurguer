#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "AVL.hpp"

int maiorEntre(int a, int b) {
    return (a > b) ? a : b;
}

int altura(NodoAVL *nodo) {
    if (nodo == nullptr)
        return 0;

    return nodo->altura;
}

int fatorBalanceamento(NodoAVL *nodo) {
    if (nodo == nullptr)
        return 0;

    return altura(nodo->esq) - altura(nodo->dir);
}

NodoAVL *rotacaoDireita(NodoAVL *y) {
    NodoAVL *x = y->esq;
    NodoAVL *T2 = x->dir;

    x->dir = y;
    y->esq = T2;

    y->altura = 1 + maiorEntre(altura(y->esq), altura(y->dir));
    x->altura = 1 + maiorEntre(altura(x->esq), altura(x->dir));

    return x;
}

NodoAVL *rotacaoEsquerda(NodoAVL *x) {
    NodoAVL *y = x->dir;
    NodoAVL *T2 = y->esq;

    y->esq = x;
    x->dir = T2;

    x->altura = 1 + maiorEntre(altura(x->esq), altura(x->dir));
    y->altura = 1 + maiorEntre(altura(y->esq), altura(y->dir));

    return y;
}

NodoAVL *inserirAVL(NodoAVL *raiz, int codigo, const char *receita, int *ok) {

    if (raiz == nullptr) {
        // Em C++ o retorno de malloc precisa de cast explícito
        NodoAVL *novo = static_cast<NodoAVL *>(malloc(sizeof(NodoAVL)));

        if (novo == nullptr) {
            if (ok != nullptr)
                *ok = 0;
            return nullptr;
        }

        novo->codigo = codigo;
        // strncpy + terminador: nunca escreve além do buffer da receita
        strncpy(novo->receita, receita, TAM_RECEITA - 1);
        novo->receita[TAM_RECEITA - 1] = '\0';

        novo->altura = 1;
        novo->esq = nullptr;
        novo->dir = nullptr;

        return novo;
    }

    if (codigo < raiz->codigo)
        raiz->esq = inserirAVL(raiz->esq, codigo, receita, ok);

    else if (codigo > raiz->codigo)
        raiz->dir = inserirAVL(raiz->dir, codigo, receita, ok);

    else
        return raiz; // código repetido: ignora

    raiz->altura = 1 + maiorEntre(altura(raiz->esq), altura(raiz->dir));

    int fb = fatorBalanceamento(raiz);

    // Caso LL
    if (fb > 1 && codigo < raiz->esq->codigo)
        return rotacaoDireita(raiz);

    // Caso RR
    if (fb < -1 && codigo > raiz->dir->codigo)
        return rotacaoEsquerda(raiz);

    // Caso LR
    if (fb > 1 && codigo > raiz->esq->codigo) {
        raiz->esq = rotacaoEsquerda(raiz->esq);
        return rotacaoDireita(raiz);
    }

    // Caso RL
    if (fb < -1 && codigo < raiz->dir->codigo) {
        raiz->dir = rotacaoDireita(raiz->dir);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

NodoAVL *buscarAVL(NodoAVL *raiz, int codigo) {

    if (raiz == nullptr)
        return nullptr;

    if (codigo == raiz->codigo)
        return raiz;

    if (codigo < raiz->codigo)
        return buscarAVL(raiz->esq, codigo);

    return buscarAVL(raiz->dir, codigo);
}

// Usa lerProximoPedido (leitura.cpp), que já valida código, tamanho da receita
// e símbolos dos ingredientes, e trata "\r\n" e linhas em branco.
int carregarArquivo(FILE *arq, NodoAVL **raiz) {
    if (arq == nullptr || raiz == nullptr)
        return 0;

    int codigo;
    char receita[TAM_RECEITA];
    int status;

    while ((status = lerProximoPedido(arq, &codigo, receita)) == 1) {
        int ok = 1;
        *raiz = inserirAVL(*raiz, codigo, receita, &ok);
        if (!ok)
            return 0;
    }

    return status == 0; // 0 = fim normal do arquivo; -1 = formato inválido
}

void liberarAVL(NodoAVL *raiz) {

    if (raiz == nullptr)
        return;

    liberarAVL(raiz->esq);
    liberarAVL(raiz->dir);

    free(raiz);
}