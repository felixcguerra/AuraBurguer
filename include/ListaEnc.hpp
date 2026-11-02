#ifndef LISTA_ENC_HPP
#define LISTA_ENC_HPP

// Representa um ingrediente do jogo.
struct Ingrediente {
    char tipo[10];   // símbolo do ingrediente (ex.: "k", "n"), como string
    int estado=0;    // campo reservado para o estado do ingrediente
};

// Nodo da lista encadeada: um ingrediente e o ponteiro para o próximo nodo.
struct str_Nodo {
    str_Nodo *prox;
    Ingrediente ingrediente;
};

// Apelido para facilitar a escrita.
using Nodo = str_Nodo;

// Lista encadeada simples: guarda apenas o ponteiro para o primeiro nodo.
// É usada para armazenar a receita de cada pedido, na ordem de montagem.
struct ListaEnc {
    Nodo *ini;
};

// Deixa a lista vazia.
void inicializar(ListaEnc *lista);
// Retorna o número de nodos da lista.
int tamanho(const ListaEnc *lista);
// Lê o ingrediente de uma posição (0 = primeiro). Retorna 0 se a posição for inválida.
int obterNaPosicao(const ListaEnc *lista, int posicao, Ingrediente *ingrediente);
// Insere um ingrediente no início da lista.
int inserirInicio(ListaEnc *lista, Ingrediente ingrediente);
// Insere um ingrediente no final da lista.
int inserirFim(ListaEnc *lista, Ingrediente ingrediente);
// Busca um ingrediente pelo tipo; retorna um ingrediente "vazio" se não encontrar.
Ingrediente buscar(const ListaEnc *lista, const char *tipo);
// Remove o primeiro nodo com o tipo informado. Retorna 1 se removeu, 0 se não encontrou.
int removerPorTipo(ListaEnc *lista, const char *tipo);
// Libera todos os nodos da lista.
void destruir(ListaEnc *lista);

#endif