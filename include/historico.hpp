#ifndef HISTORICO_HPP
#define HISTORICO_HPP

#include "leitura.hpp" // TAM_NOME

// Uma partida do histórico: nome do jogador e pontuação obtida.
typedef struct {
    char nome[TAM_NOME];
    int pontuacao;
} Registro;

// Lista contígua alocada dinamicamente: os registros ficam lado a lado em um
// vetor, o que dá acesso por índice em O(1) (necessário para os algoritmos de
// ordenação). Quando o vetor enche, a capacidade é dobrada.
typedef struct {
    Registro *dados; // vetor de registros
    int tam;         // quantidade de registros guardados
    int cap;         // capacidade atual do vetor
} ListaContigua;

// Cria a lista vazia (com uma capacidade inicial).
void iniciarLista(ListaContigua *l);

// Acrescenta um registro ao final da lista.
void inserirLista(ListaContigua *l, const char *nome, int pontuacao);

// Cria em 'copia' uma cópia independente de 'origem' (usada para ordenar sem
// mexer na lista original).
void copiarLista(const ListaContigua *origem, ListaContigua *copia);

// Libera o vetor da lista.
void liberarLista(ListaContigua *l);

// Lê o arquivo de histórico (duas linhas por partida: nome e pontuação) e
// acrescenta os registros na lista. Se o arquivo ainda não existe, a lista
// fica como está (histórico vazio). Retorna 1 em sucesso e 0 se o arquivo
// tiver formato inválido (os registros lidos até o erro ficam na lista).
int carregarHistorico(ListaContigua *l, const char *caminho);

// Acrescenta uma partida ao FINAL do arquivo, sem apagar as anteriores.
// Retorna 1 em sucesso e 0 se não foi possível gravar.
int salvarRegistro(const char *caminho, const char *nome, int pontuacao);

// Registra uma partida encerrada: acrescenta na lista e grava no arquivo.
// Retorna 1 em sucesso e 0 se a gravação falhou.
int registrarPartida(ListaContigua *l, const char *caminho, const char *nome, int pontuacao);

#endif