#include "fase.hpp"
#include <cstring>

static int adicionarPedido(FilaEnc *pedidos, int codigo, const char *combinacao) {
    Pedido pedido = {};
    pedido.cod = codigo;
    inicializar(&pedido.ingredientes);

    for (const char *caractere = combinacao; *caractere != '\0'; ++caractere) {
        Ingrediente ingrediente = {};
        ingrediente.tipo[0] = *caractere;
        ingrediente.tipo[1] = '\0';
        ingrediente.estado = 0;
        if (!inserirFim(&pedido.ingredientes, ingrediente)) {
            destruir(&pedido.ingredientes);
            return 0;
        }
    }

    if (!enqueue(pedidos, pedido)) {
        destruir(&pedido.ingredientes);
        return 0;
    }
    return 1;
}

int iniciarfase(Fase *fase) {
    if (fase == nullptr)
        return 0;

    limpar(&fase->pedidos);
    limpar(&fase->pedidoAtual);
    inicializa(&fase->pedidos);
    fase->cod = 0;
    fase->TurnoAtual = 0;
    fase->NumDePedidosIni = 3;

    fase->PacienciaDosClientes = 15;
    fase->PacienciaDoClienteAtual = fase->PacienciaDosClientes;
    fase->PreparadoSuiço = 0;
    fase->PreparadoCheddar = 0;
    fase->PreparadoCebola = 0;
    fase->PreparadoTomate = 0;
    fase->NumDePedidos = 0;

    if (!adicionarPedido(&fase->pedidos, 1, "skmpaz") ||
        !adicionarPedido(&fase->pedidos, 2, "qctmmz") ||
        !adicionarPedido(&fase->pedidos, 3, "kcsqmkz")) {
        limpar(&fase->pedidos);
        return 0;
    }
    fase->NumDePedidos = fase->NumDePedidosIni;

    inicializa(&fase->pedidoAtual);
    inicializa(&fase->Bacon);
    inicializa(&fase->Carne);
    inicializa(&fase->Ovo);
    return 1;
}
int CompararHamburguerComPedido(const PilhaEnc *hamburguer, const Pedido *pedido) {
    if (hamburguer == nullptr || pedido == nullptr)
        return 0;

    int tamanhoHamburguer = tamanho(hamburguer);
    int tamanhoPedido = tamanho(&pedido->ingredientes);

    if (tamanhoHamburguer != tamanhoPedido)
        return 0;

    Nodo *nodo = pedido->ingredientes.ini;
       for (int i = tamanhoHamburguer-1; i >=0; i--) {
        Ingrediente ingrediente = {};
        if (!obterNaPosicao(hamburguer, i, &ingrediente)) {
            return 0;
        }
        if (std::strcmp(ingrediente.tipo, nodo->ingrediente.tipo) != 0) {
            return 0;
        }
        nodo = nodo->prox;
    }

    return 1;
}

GameState atualizarfase(Fase *fase) {
    if (fase == nullptr)
        return STATE_DERROTA;
    
    fase->TurnoAtual++;
    if (fase->PacienciaDoClienteAtual > 0)
        fase->PacienciaDoClienteAtual--;
    Pedido pedidoAtual = {};
    if (frente(&fase->pedidos, &pedidoAtual) &&
        CompararHamburguerComPedido(&fase->pedidoAtual, &pedidoAtual)) {
        fase->NumDePedidos--;
        limpar(&fase->pedidoAtual);
        Pedido descartado;
        if (fase->NumDePedidos <= 0) return STATE_VITORIA;
        else dequeue(&fase->pedidos, &descartado);
        fase->PacienciaDoClienteAtual = fase->PacienciaDosClientes;
    }
    
    if (fase->PacienciaDoClienteAtual <= 0)
        return STATE_DERROTA;

    return STATE_NOVO_JOGO;
}
