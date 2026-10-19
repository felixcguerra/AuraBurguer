#include <cstring>
#include "../include/filaprioridade.hpp"

void inicializa(FilaPrioridade *fp) {
    std::memset(fp->dados, 0, sizeof(fp->dados));
    fp->tam = 0;
}

void siftUp(int a[], int i) {
    while (i > 0) {
        int pai = (i - 1) / 2;
        if (a[pai] <= a[i]) return;

        int tmp = a[i];
        a[i] = a[pai];
        a[pai] = tmp;
        i = pai;
    }
}

void heapify(int a[], int n, int i) {
    int menor = i;
    int esq   = 2*i + 1;
    int dir   = 2*i + 2;

    if (esq < n && a[esq] < a[menor]) menor = esq;
    if (dir < n && a[dir] < a[menor]) menor = dir;

    if (menor != i) {
        int tmp = a[i]; a[i] = a[menor]; a[menor] = tmp;
        heapify(a, n, menor);
    }
}

void fpInserir(FilaPrioridade *fp, int valor) {
    fp->dados[fp->tam] = valor;
    siftUp(fp->dados, fp->tam);
    fp->tam++;
}

int fpMinimo(FilaPrioridade *fp) {
    return fp->dados[0];
}

int fpRemoverMinimo(FilaPrioridade *fp) {
    int min = fp->dados[0];
    fp->tam--;
    fp->dados[0] = fp->dados[fp->tam];
    heapify(fp->dados, fp->tam, 0);
    return min;
}