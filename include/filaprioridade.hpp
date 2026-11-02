#ifndef FILAPRIORIDADE_HPP
#define FILAPRIORIDADE_HPP


// Capacidade máxima do heap (tamanho fixo do vetor)
#define CAP 1024



// Heap mínimo em vetor: guarda os turnos de vencimento dos ingredientes
// perecíveis, com o mais próximo de vencer na posição 0.
typedef struct {
    int dados[CAP]; // elementos do heap
    int tam;        // quantidade de elementos atualmente no heap
} FilaPrioridade;

// Deixa o heap vazio.
void inicializa(FilaPrioridade *fp);

// Faz o elemento do índice i subir até a posição correta (usado na inserção).
void siftUp(int a[], int i);

// Faz o elemento do índice i descer até a posição correta (usado na remoção).
void heapify(int a[], int n, int i);

// Constrói um heap a partir de um vetor desordenado.
void buildHeap(int a[], int n);

// Insere um valor no heap.
void fpInserir(FilaPrioridade *fp, int valor);

// Retorna o menor valor sem removê-lo (O(1)).
int fpMinimo(FilaPrioridade *fp);

// Remove e retorna o menor valor.
int fpRemoverMinimo(FilaPrioridade *fp);

void verificarfp(FilaPrioridade *fp, int turno);

#endif