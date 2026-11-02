#include <new>
#include <cstring>
#include "ListaEnc.hpp"

// Deixa a lista vazia (sem nenhum nodo).
void inicializar(ListaEnc *lista) {
    lista->ini = nullptr;
}

// Conta os nodos da lista percorrendo do início ao fim (custo O(n)).
int tamanho(const ListaEnc *lista) {
    Nodo *aux = lista->ini;
    int contador = 0;

    while (aux != nullptr) {
        contador++;
        aux = aux->prox;
    }

    return contador;
}

// Lê o ingrediente na posição dada (0 = primeiro nodo) e o devolve em *ingrediente.
// Retorna 1 em caso de sucesso e 0 se os parâmetros forem inválidos ou a posição não existir.
int obterNaPosicao(const ListaEnc *lista, int posicao, Ingrediente *ingrediente) {
    if (lista == nullptr || ingrediente == nullptr || posicao < 0)
        return 0;

    // Avança 'posicao' nodos a partir do início
    Nodo *aux = lista->ini;
    while (aux != nullptr && posicao > 0) {
        aux = aux->prox;
        posicao--;
    }

    if (aux == nullptr) // posição além do fim da lista
        return 0;

    *ingrediente = aux->ingrediente;
    return 1;
}

// Insere um ingrediente no início da lista (custo O(1)).
// Retorna 1 em caso de sucesso e 0 se a alocação falhar.
int inserirInicio(ListaEnc *lista, Ingrediente prod) {
    // nothrow: retorna nullptr em vez de lançar exceção se faltar memória
    Nodo *novo = new (std::nothrow) Nodo;
    if (novo == nullptr)
        return 0; // falha na alocação

    novo->ingrediente = prod;
    novo->prox = lista->ini; // o novo nodo aponta para o antigo início
    lista->ini = novo;        // o novo nodo passa a ser o início

    return 1;
}

// Insere um ingrediente no final da lista (custo O(n), pois é preciso
// percorrer até o último nodo). É assim que a receita do pedido é montada,
// preservando a ordem de baixo para cima.
// Retorna 1 em caso de sucesso e 0 se a alocação falhar.
int inserirFim(ListaEnc *lista, Ingrediente prod) {
    Nodo *novo;
    Nodo *aux;

    novo = new (std::nothrow) Nodo;
    if (novo == nullptr)
        return 0;

    novo->ingrediente = prod;
    novo->prox = nullptr; // será o último nodo

    if (lista->ini == nullptr) { // lista vazia: o novo nodo é o único (e o início)
        lista->ini = novo;
    } else {
        aux = lista->ini;
        while (aux->prox != nullptr) // percorre até o último nodo
            aux = aux->prox;
        aux->prox = novo;
    }

    return 1;
}

// Busca o primeiro ingrediente cujo tipo seja igual ao informado.
// Retorna uma cópia do ingrediente encontrado, ou um Ingrediente "vazio"
// (tipo "" e estado 0) se não existir.
Ingrediente buscar(const ListaEnc *lista, const char *tipo) {
    Nodo *aux;
    Ingrediente prod = {"", 0}; // valor devolvido caso nada seja encontrado
    aux = lista->ini;

    while (aux != nullptr) {
        if (strcmp(aux->ingrediente.tipo, tipo) == 0)
            return aux->ingrediente;
        aux = aux->prox;
    }

    return prod; // não encontrado: Ingrediente "vazio"
}

// Remove da lista o primeiro nodo cujo tipo seja igual ao informado.
// Retorna 1 se removeu e 0 se não encontrou.
int removerPorTipo(ListaEnc *lista, const char *tipo) {
    Nodo *ant;
    Nodo *aux;

    ant = nullptr; // 'ant' acompanha o nodo anterior ao 'aux'
    aux = lista->ini;

    // Procura o nodo a remover, guardando também o anterior
    while (aux != nullptr && strcmp(aux->ingrediente.tipo, tipo) != 0) {
        ant = aux;
        aux = aux->prox;
    }

    if (aux == nullptr) // não encontrado
        return 0;

    if (ant == nullptr) // removendo o primeiro nodo
        lista->ini = aux->prox;
    else             // removendo do meio ou do final
        ant->prox = aux->prox;

    delete aux;
    return 1;
}

// Libera todos os nodos da lista e a deixa vazia.
void destruir(ListaEnc *lista) {
    Nodo *ant;
    Nodo *aux;
    aux = lista->ini;

    // 'ant' guarda o nodo atual para liberá-lo depois que 'aux' já avançou
    while (aux != nullptr) {
        ant = aux;
        aux = aux->prox;
        delete ant;
    }

    lista->ini = nullptr;
}