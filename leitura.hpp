#ifndef LEITURA_HPP
#define LEITURA_HPP

#include <cstdio>

// Tamanhos dos buffers de texto.
// Receita: de 5 a 7 caracteres no enunciado (folga para não estourar o buffer).
// Nome: no máximo 20 caracteres + 1 para o '\0'.
#define TAM_RECEITA 16
#define TAM_NOME 21

// Abre um arquivo para leitura de texto.
// Retorna o ponteiro do arquivo ou nullptr se não foi possível abrir.
FILE *abrirArquivoLeitura(const char *caminho);

// Lê o próximo pedido do arquivo de pedidos (duas linhas: código e receita).
// Retorna: 1 = leu um pedido; 0 = fim do arquivo; -1 = formato inválido.
int lerProximoPedido(FILE *arq, int *codigo, char receita[TAM_RECEITA]);

// Lê o próximo registro do arquivo de histórico (duas linhas: nome e pontuação).
// Retorna: 1 = leu um registro; 0 = fim do arquivo; -1 = formato inválido.
int lerProximoRegistroHistorico(FILE *arq, char nome[TAM_NOME], int *pontuacao);

#endif