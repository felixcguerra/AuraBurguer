#include <cstdlib>
#include <cstring>
#include "hash.hpp"

void iniciarHash(TabelaHash *t, int m) {
    t->tabela = (No **)calloc(m, sizeof(No *)); /* calloc: todas as listas vazias */
    t->m = m;
    t->n = 0;
}

/* Inserção com encadeamento — O(1) */
void inserirHash(TabelaHash *t, int chave, const char *receita) {
    int i = chave % t->m;
    No *novo = (No *)malloc(sizeof(No));
    novo->chave = chave;
    strcpy(novo->receita, receita);
    novo->prox  = t->tabela[i];   /* insere no início */
    t->tabela[i] = novo;
    t->n++;
}

/* Busca com encadeamento — O(1 + comprimento da lista) */
const char *buscarHash(TabelaHash *t, int chave) {
    int i = chave % t->m;
    No *p = t->tabela[i];
    while (p != NULL) {
        if (p->chave == chave) return p->receita;
        p = p->prox;
    }
    return NULL;   /* não encontrado */
}

/* Remoção com encadeamento — O(1 + comprimento da lista) */
int removerHash(TabelaHash *t, int chave) {
    int i = chave % t->m;
    No *p = t->tabela[i];
    No *ant = NULL;
    while (p != NULL) {
        if (p->chave == chave) {
            if (ant == NULL)
                t->tabela[i] = p->prox;   /* era o primeiro */
            else
                ant->prox = p->prox;
            free(p);
            t->n--;
            return 1;   /* removido */
        }
        ant = p;
        p   = p->prox;
    }
    return 0;   /* não encontrado */
}

void liberarHash(TabelaHash *t) {
    for (int i = 0; i < t->m; i++) {
        No *p = t->tabela[i];
        while (p != NULL) {
            No *prox = p->prox;
            free(p);
            p = prox;
        }
    }
    free(t->tabela);
}

const char *buscarPedidoHash(TabelaHash *t, NodoAVL *raiz, int codigo) {
    const char *receita = buscarHash(t, codigo);   /* hit: não consulta a AVL */
    if (receita != NULL)
        return receita;

    NodoAVL *nodo = buscarAVL(raiz, codigo);      /* miss: busca na AVL */
    if (nodo == NULL)
        return NULL;

    inserirHash(t, codigo, nodo->receita);         /* guarda no cache */
    return buscarHash(t, codigo);
}