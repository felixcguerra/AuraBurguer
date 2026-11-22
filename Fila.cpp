#include <new>
#include "Fila.hpp"

// Nodo interno da fila encadeada: guarda um Pedido e o ponteiro para o próximo nodo.
// Fica definido só neste .cpp, então quem usa a fila não enxerga os detalhes internos.
struct FilaNodo {
    FilaNodo *prox;
    Pedido dado;
};

// Deixa a fila vazia: sem nodo na frente e sem nodo no final.
void inicializa(FilaEnc *f) {
    f->frente = nullptr;
    f->final = nullptr;
}

// Remove todos os pedidos da fila e libera a lista de ingredientes de cada um.
// Cada Pedido tem uma ListaEnc própria, por isso ela precisa ser destruída
// além de o nodo da fila ser removido.
void limpar(FilaEnc *f) {
    Pedido descartado;
    while (dequeue(f, &descartado)) {
        destruir(&descartado.ingredientes);
    }
}

// Retorna 1 se a fila não tem elementos, 0 caso contrário.
int estaVazia(const FilaEnc *f) {
    return f->frente == nullptr;
}

// Insere um pedido no final da fila (política FIFO: quem entra primeiro sai primeiro).
// Retorna 1 em caso de sucesso e 0 se a alocação de memória falhar.
int enqueue(FilaEnc *f, Pedido valor) {
    // nothrow: em vez de lançar exceção, retorna nullptr se faltar memória
    FilaNodo *novo = new (std::nothrow) FilaNodo;
    if (novo == nullptr)
        return 0;

    novo->dado = valor;
    novo->prox = nullptr; // o novo nodo será o último, então não aponta para ninguém

    if (f->final != nullptr)
        f->final->prox = novo; // o antigo final passa a apontar para o novo nodo
    else
        f->frente = novo;      // fila estava vazia: novo nodo também é a frente

    f->final = novo;
    return 1;
}

// Remove o pedido da frente da fila e o devolve em *valorRemovido.
// Retorna 1 em caso de sucesso e 0 se a fila estiver vazia (underflow).
// Atenção: só o nodo é liberado; a lista de ingredientes do pedido continua
// existindo e é responsabilidade de quem chamou destruí-la (se necessário).
int dequeue(FilaEnc *f, Pedido *valorRemovido) {
    FilaNodo *removido;

    if (estaVazia(f)) // underflow
        return 0;

    removido = f->frente;
    *valorRemovido = removido->dado;
    f->frente = removido->prox; // a frente passa a ser o segundo nodo

    if (f->frente == nullptr) // a fila ficou vazia
        f->final = nullptr;   // então o final também precisa ser atualizado

    delete(removido);
    return 1;
}

// Consulta o pedido da frente sem removê-lo da fila.
// Retorna 1 em caso de sucesso e 0 se a fila estiver vazia.
int frente(const FilaEnc *f, Pedido *valor) {
    if (estaVazia(f))
        return 0;

    *valor = f->frente->dado; // copia o dado do primeiro nodo sem removê-lo
    return 1;
}

// Conta quantos pedidos existem na fila percorrendo todos os nodos (custo O(n)).
int tamanho(const FilaEnc *f) {
    FilaNodo *aux = f->frente;
    int contador = 0;

    while (aux != nullptr) {
        contador++;
        aux = aux->prox;
    }

    return contador;
}