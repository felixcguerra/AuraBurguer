#include "filaprioridade.hpp"
#include <cstring>

// Heap mínimo guardado em um vetor: para o elemento no índice i,
//   pai    = (i - 1) / 2
//   filho esquerdo = 2*i + 1
//   filho direito  = 2*i + 2
// O menor valor (turno de vencimento mais próximo) fica sempre na posição 0 (a raiz).

// Zera o vetor de dados e deixa o heap vazio (tam = 0).
void inicializa(FilaPrioridade *fp) {
    if (fp != nullptr) {
        std::memset(fp->dados, 0, sizeof(fp->dados));
        fp->tam = 0;
    }
}

// "Sobe" o elemento do índice i enquanto ele for menor que o pai,
// trocando os dois de lugar. Usado depois de inserir no final do vetor
// para restaurar a propriedade do heap mínimo (custo O(log n)).
void siftUp(int a[], int i) {
    while (i > 0) {
        int pai = (i - 1) / 2;
        if (a[pai] <= a[i]) break; // o pai já é menor ou igual: heap correto

        int tmp = a[i];
        a[i] = a[pai];
        a[pai] = tmp;
        i = pai; // continua verificando a partir da nova posição
    }
}

// "Desce" o elemento do índice i (em um heap de n elementos) enquanto algum
// filho for menor que ele, trocando-o com o menor dos filhos. Usado depois
// de remover a raiz para restaurar a propriedade do heap (custo O(log n)).
void heapify(int a[], int n, int i) {
    int menor = i;
    int esq   = 2 * i + 1;
    int dir   = 2 * i + 2;

    // Descobre qual dos três (pai, filho esquerdo, filho direito) é o menor
    if (esq < n && a[esq] < a[menor]) menor = esq;
    if (dir < n && a[dir] < a[menor]) menor = dir;

    // Se o menor não é o próprio nodo, troca e repete a verificação mais abaixo
    if (menor != i) {
        int tmp = a[i];
        a[i] = a[menor];
        a[menor] = tmp;
        heapify(a, n, menor);
    }
}

// Constrói um heap mínimo a partir de um vetor desordenado.
void buildHeap(int a[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(a, n, i);
    }
}

// Insere um valor: coloca no final do vetor e o faz subir até a posição certa.
// O valor inserido representa o turno de vencimento do ingrediente perecível.
void fpInserir(FilaPrioridade *fp, int valor) {
    if (fp == nullptr || fp->tam >= CAP) return;
    
    fp->dados[fp->tam] = valor;
    siftUp(fp->dados, fp->tam);
    fp->tam++;
}

// Consulta o menor valor (o ingrediente mais próximo de vencer) sem removê-lo.
int fpMinimo(FilaPrioridade *fp) {
    if (fp == nullptr || fp->tam <= 0) return -1;
    return fp->dados[0];
}

// Remove e retorna o menor valor: guarda a raiz, move o último elemento para
// a raiz e o faz descer até a posição certa.
int fpRemoverMinimo(FilaPrioridade *fp) {
    if (fp == nullptr || fp->tam <= 0) return -1;

    int min = fp->dados[0];
    fp->tam--;
    fp->dados[0] = fp->dados[fp->tam]; // o último elemento ocupa o lugar da raiz
    heapify(fp->dados, fp->tam, 0);
    return min;
}

void verificarfp(FilaPrioridade *fp, int turno){
    int min=fpMinimo(fp);
    if(min+6<turno)
        fpRemoverMinimo(fp);

}