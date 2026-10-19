#ifndef LISTA_ENC_HPP
#define LISTA_ENC_HPP

struct Ingrediente {
    char tipo[10];
    int estado=0;
};

struct str_Nodo {
    str_Nodo *prox;
    Ingrediente ingrediente;
};

using Nodo = str_Nodo;

struct ListaEnc {
    Nodo *ini;
};

void inicializar(ListaEnc *lista);
int tamanho(const ListaEnc *lista);
int obterNaPosicao(const ListaEnc *lista, int posicao, Ingrediente *ingrediente);
int inserirInicio(ListaEnc *lista, Ingrediente ingrediente);
int inserirFim(ListaEnc *lista, Ingrediente ingrediente);
Ingrediente buscar(const ListaEnc *lista, const char *tipo);
int removerPorTipo(ListaEnc *lista, const char *tipo);
void destruir(ListaEnc *lista);

#endif