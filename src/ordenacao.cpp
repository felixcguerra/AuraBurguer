#include <cctype>
#include <cstdlib>
#include "ordenacao.hpp"
#include "medicoes.hpp"

// Compara dois nomes ignorando maiúsculas/minúsculas.
// Retorna < 0 se a vem antes de b, 0 se são iguais e > 0 se a vem depois.
static int compararNomes(const char *a, const char *b) {
    while (*a != '\0' && *b != '\0') {
        int ca = tolower((unsigned char)*a);
        int cb = tolower((unsigned char)*b);
        if (ca != cb)
            return ca - cb;
        a++;
        b++;
    }
    return tolower((unsigned char)*a) - tolower((unsigned char)*b);
}

// Critério de ordenação: retorna 1 se x pode ficar antes de y.
// O empate também retorna 1 (<= e >=), o que mantém a estabilidade do merge.
static int vemAntes(const Registro *x, const Registro *y, OrdemHistorico ordem) {
    if (ordem == ORDEM_ALFABETICA)
        return compararNomes(x->nome, y->nome) <= 0;
    return x->pontuacao >= y->pontuacao; // ORDEM_PONTUACAO: decrescente
}

/* Intercala a[ini..meio] e a[meio+1..fim], ambos já ordenados. */
static void merge(Registro a[], Registro aux[], int ini, int meio, int fim, OrdemHistorico ordem) {
    int t, i, j, k;

    for (t = ini; t <= fim; t++)      /* copia para aux */
        aux[t] = a[t];

    i = ini;  j = meio + 1;  k = ini;

    while (i <= meio && j <= fim) {
        if (vemAntes(&aux[i], &aux[j], ordem)) /* empate fica à esquerda: estável */
            a[k++] = aux[i++];
        else
            a[k++] = aux[j++];
    }
    while (i <= meio) a[k++] = aux[i++];
    while (j <= fim)  a[k++] = aux[j++];
}

static void mergeSortRec(Registro a[], Registro aux[], int ini, int fim, OrdemHistorico ordem) {
    if (ini >= fim) return;               /* conquista: 0 ou 1 elemento */
    int meio = ini + (fim - ini) / 2;     /* divisão: ao meio */
    mergeSortRec(a, aux, ini, meio, ordem);
    mergeSortRec(a, aux, meio + 1, fim, ordem);
    merge(a, aux, ini, meio, fim, ordem); /* recombinação */
}

static void mergeSort(Registro a[], int n, OrdemHistorico ordem) {
    Registro *aux = (Registro *)malloc(n * sizeof(Registro)); /* o vetor extra: O(n) de memória */
    mergeSortRec(a, aux, 0, n - 1, ordem);
    free(aux);
}

void ordenarHistorico(ListaContigua *l, OrdemHistorico ordem) {
    if (l->tam < 2)
        return;

    Timer t;
    timer_iniciar(&t);

    mergeSort(l->dados, l->tam, ordem);

    double tempo = timer_parar(&t);
    registrarMetricaOrdenacao(tempo, l->tam,
                              ordem == ORDEM_ALFABETICA ? "alfabetica" : "pontuacao decrescente");
}