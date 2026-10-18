#define CAP 1024

typedef struct {
    int dados[CAP];
    int tam;
} FilaPrioridade;

void siftUp(int a[], int i) {
    while (i > 0) {
        int pai = (i - 1) / 2;
        if (a[pai] >= a[i]) return;

        int tmp = a[i];
        a[i] = a[pai];
        a[pai] = tmp;
        i = pai;
    }
}

void heapify(int a[], int n, int i) {
    int maior = i;
    int esq   = 2*i + 1;
    int dir   = 2*i + 2;

    if (esq < n && a[esq] > a[maior]) maior = esq;
    if (dir < n && a[dir] > a[maior]) maior = dir;

    if (maior != i) {
        int tmp = a[i]; a[i] = a[maior]; a[maior] = tmp;
        heapify(a, n, maior);
    }
}

void fpInserir(FilaPrioridade *fp, int valor) {
    fp->dados[fp->tam] = valor;
    siftUp(fp->dados, fp->tam);
    fp->tam++;
}

int fpMaximo(FilaPrioridade *fp) {
    return fp->dados[0];   /* O(1) */
}

int fpRemoverMaximo(FilaPrioridade *fp) {
    int max    = fp->dados[0];
    fp->tam--;
    fp->dados[0] = fp->dados[fp->tam];
    heapify(fp->dados, fp->tam, 0);
    return max;
}