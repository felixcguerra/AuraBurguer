#ifndef FILAPRIORIDADE_HPP
#define FILAPRIORIDADE_HPP

#define CAP 1024

typedef struct {
    int dados[CAP];
    int tam;
} FilaPrioridade;

void inicializa(FilaPrioridade *fp);
void siftUp(int a[], int i);
void heapify(int a[], int n, int i);
void fpInserir(FilaPrioridade *fp, int valor);
int fpMinimo(FilaPrioridade *fp);
int fpRemoverMinimo(FilaPrioridade *fp);

#endif
