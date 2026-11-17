#include <cstdlib>
#include <cstring>
#include "hash.hpp"
#include "medicoes.hpp"

void iniciarHash(TabelaHash *t, int m) {
    t->tabela = (No **)calloc(m, sizeof(No *));
    t->m = m;
    t->n = 0;
}

void inserirHash(TabelaHash *t, int chave, const char *receita) {
    int i = chave % t->m;
    if (t->tabela[i] != NULL) {
        incrementarColisaoHash(); // Conta colisão por encadeamento
    }
    No *novo = (No *)malloc(sizeof(No));
    novo->chave = chave;
    strcpy(novo->receita, receita);
    novo->prox  = t->tabela[i];
    t->tabela[i] = novo;
    t->n++;
}

static const char *buscarHashAux(TabelaHash *t, int chave, bool *encontrou) {
    int i = chave % t->m;
    No *p = t->tabela[i];
    while (p != NULL) {
        if (p->chave == chave) {
            if (encontrou) *encontrou = true;
            return p->receita;
        }
        p = p->prox;
    }
    if (encontrou) *encontrou = false;
    return NULL;
}

const char *buscarHash(TabelaHash *t, int chave) {
    return buscarHashAux(t, chave, NULL);
}

/* Remoção com encadeamento: O(1 + comprimento da lista) */
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
    bool hit = false;
    const char *receita = buscarHashAux(t, codigo, &hit);
    if (hit) {
        registrarAcessoHash(true); // Hit no cache
        return receita;
    }
    registrarAcessoHash(false); // Miss no cache

    // Cronometra e conta comparações da busca na AVL
    Timer tBusca;
    timer_iniciar(&tBusca);
    int comparacoes = 0;
    
    // Buscamos na AVL contando comparações
    NodoAVL *nodo = nullptr;
    NodoAVL *atual = raiz;
    while (atual != nullptr) {
        comparacoes++;
        if (codigo == atual->codigo) {
            nodo = atual;
            break;
        }
        if (codigo < atual->codigo)
            atual = atual->esq;
        else
            atual = atual->dir;
    }
    double tempoBusca = timer_parar(&tBusca);
    registrarBuscaAVL(comparacoes, tempoBusca);

    if (nodo == NULL)
        return NULL;

    inserirHash(t, codigo, nodo->receita);
    return buscarHashAux(t, codigo, NULL);
}