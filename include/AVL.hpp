#ifndef AVL_HPP
#define AVL_HPP

#include <cstdio>
#include "leitura.hpp" // TAM_RECEITA

// Nodo da árvore AVL que guarda os pedidos lidos do arquivo.
// (Renomeado de "Pedido" para "NodoAVL": o nome "Pedido" já é usado pela
//  fila em Fila.hpp e os dois tipos conflitavam quando incluídos juntos.)
struct NodoAVL {
    int codigo;
    char receita[TAM_RECEITA];

    int altura;

    NodoAVL *esq;
    NodoAVL *dir;
};

int maiorEntre(int a, int b);
int altura(NodoAVL *nodo);
int fatorBalanceamento(NodoAVL *nodo);

NodoAVL *rotacaoDireita(NodoAVL *y);
NodoAVL *rotacaoEsquerda(NodoAVL *x);

// Insere (codigo, receita) e retorna a nova raiz. Em falta de memória
// retorna a raiz sem alteração e *ok (se não for nullptr) passa a 0.
NodoAVL *inserirAVL(NodoAVL *raiz, int codigo, const char *receita, int *ok = nullptr);
NodoAVL *buscarAVL(NodoAVL *raiz, int codigo);

// Lê todos os pedidos de um arquivo já aberto e os insere na AVL.
// Retorna 1 em sucesso e 0 se o arquivo estiver malformado ou faltar memória
// (a árvore parcial continua válida e deve ser liberada por quem chamou).
int carregarArquivo(FILE *arq, NodoAVL **raiz);

void liberarAVL(NodoAVL *raiz);

#endif