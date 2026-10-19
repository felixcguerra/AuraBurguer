#include <new>
#include <cstring>
#include "ListaEnc.hpp"

void inicializar(ListaEnc *lista) {
    lista->ini = nullptr;
}
int tamanho(const ListaEnc *lista) {
    Nodo *aux = lista->ini;
    int contador = 0;

    while (aux != nullptr) {
        contador++;
        aux = aux->prox;
    }

    return contador;
}
int obterNaPosicao(const ListaEnc *lista, int posicao, Ingrediente *ingrediente) {
    if (lista == nullptr || ingrediente == nullptr || posicao < 0)
        return 0;

    Nodo *aux = lista->ini;
    while (aux != nullptr && posicao > 0) {
        aux = aux->prox;
        posicao--;
    }

    if (aux == nullptr)
        return 0;

    *ingrediente = aux->ingrediente;
    return 1;
}
int inserirInicio(ListaEnc *lista, Ingrediente prod) {
    Nodo *novo = new (std::nothrow) Nodo;
    if (novo == nullptr)
        return 0; // falha na alocação

    novo->ingrediente = prod;
    novo->prox = lista->ini; // o novo nodo aponta para o antigo início
    lista->ini = novo;        // o novo nodo passa a ser o início

    return 1;
}
int inserirFim(ListaEnc *lista, Ingrediente prod) {
    Nodo *novo;
    Nodo *aux;

    novo = new (std::nothrow) Nodo;
    if (novo == nullptr)
        return 0;

    novo->ingrediente = prod;
    novo->prox = nullptr;

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
Ingrediente buscar(const ListaEnc *lista, const char *tipo) {
    Nodo *aux;
    Ingrediente prod = {"", 0};
    aux = lista->ini;

    while (aux != nullptr) {
        if (strcmp(aux->ingrediente.tipo, tipo) == 0)
            return aux->ingrediente;
        aux = aux->prox;
    }

    return prod; // não encontrado: Ingrediente "vazio"
}
int removerPorTipo(ListaEnc *lista, const char *tipo) {
    Nodo *ant;
    Nodo *aux;

    ant = nullptr;
    aux = lista->ini;

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
void destruir(ListaEnc *lista) {
    Nodo *ant;
    Nodo *aux;
    aux = lista->ini;

    while (aux != nullptr) {
        ant = aux;
        aux = aux->prox;
        delete ant;
    }

    lista->ini = nullptr;
}