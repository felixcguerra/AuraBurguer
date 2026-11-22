#ifndef HASH_HPP
#define HASH_HPP

#include "AVL.hpp" // NodoAVL, TAM_RECEITA

typedef struct No {
    int chave;                // código do pedido
    char receita[TAM_RECEITA]; // valor guardado (no lugar do int da base)
    struct No *prox;
} No;

typedef struct {
    No **tabela;   /* vetor de ponteiros para listas */
    int m;         /* número de slots */
    int n;         /* número de elementos */
} TabelaHash;

/* Cria a tabela com m slots vazios */
void iniciarHash(TabelaHash *t, int m);

/* Inserção com encadeamento — O(1) */
void inserirHash(TabelaHash *t, int chave, const char *receita);

/* Busca com encadeamento — retorna a receita ou NULL se não encontrado */
const char *buscarHash(TabelaHash *t, int chave);

/* Remoção com encadeamento — retorna 1 se removeu, 0 se não encontrou */
int removerHash(TabelaHash *t, int chave);

/* Libera a tabela inteira */
void liberarHash(TabelaHash *t);

/* Cache: consulta a hash; se não achar (miss), busca na AVL e insere na hash */
const char *buscarPedidoHash(TabelaHash *t, NodoAVL *raiz, int codigo);

#endif