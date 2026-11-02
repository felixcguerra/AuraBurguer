#include "fase.hpp"
#include <cstring>
#include <ctime>
#include <random>

// Função auxiliar (visível só neste arquivo): cria um Pedido a partir do código
// e da string de receita (ex.: "skmpaz"), monta a lista encadeada de ingredientes
// (um nodo por caractere, na ordem da receita) e coloca o pedido na fila.
// Retorna 1 em caso de sucesso e 0 se alguma alocação falhar (liberando o que
// já tinha sido alocado para o pedido).
static int adicionarPedido(FilaEnc *pedidos, int codigo, const char *combinacao) {
    Pedido pedido = {};
    pedido.cod = codigo;
    inicializar(&pedido.ingredientes);

    // Cada caractere da string vira um Ingrediente com tipo de 1 letra
    for (const char *caractere = combinacao; *caractere != '\0'; ++caractere) {
        Ingrediente ingrediente = {};
        ingrediente.tipo[0] = *caractere;
        ingrediente.tipo[1] = '\0'; // termina a string do tipo
        ingrediente.estado = 0;
        if (!inserirFim(&pedido.ingredientes, ingrediente)) {
            destruir(&pedido.ingredientes);
            return 0;
        }
    }

    // Se não conseguir enfileirar, libera a lista de ingredientes recém-criada
    if (!enqueue(pedidos, pedido)) {
        destruir(&pedido.ingredientes);
        return 0;
    }
    return 1;
}

// Menor e maior código cadastrado na AVL (extremos esquerdo e direito da árvore).
static int menorCodigo(NodoAVL *raiz) {
    while (raiz->esq != nullptr)
        raiz = raiz->esq;
    return raiz->codigo;
}

static int maiorCodigo(NodoAVL *raiz) {
    while (raiz->dir != nullptr)
        raiz = raiz->dir;
    return raiz->codigo;
}

// Sorteia um número entre min e max (inclusive). Usa <random> em vez de rand(),
// porque no Windows RAND_MAX é 32767 e os códigos passam de 800 mil.
static int sortear(int min, int max) {
    static std::mt19937 gerador((unsigned)std::time(nullptr));
    std::uniform_int_distribution<int> distribuicao(min, max);
    return distribuicao(gerador);
}

// Sorteia um código de pedido e obtém a receita: primeiro consulta a hash (hit);
// se não estiver lá (miss), busca na AVL e guarda o resultado na hash.
// Se o código sorteado não existir na AVL, sorteia outro.
// Retorna o código sorteado e coloca a receita em *receita.
static int sortearPedido(NodoAVL *raiz, TabelaHash *cache, const char **receita) {
    int menor = menorCodigo(raiz);
    int maior = maiorCodigo(raiz);
    int codigo;
    do {
        codigo = sortear(menor, maior);
        *receita = buscarPedidoHash(cache, raiz, codigo);
    } while (*receita == nullptr);
    return codigo;
}

// Prepara uma fase nova: zera contadores, define a paciência dos clientes,
// cria os pedidos iniciais (sorteados, via hash/AVL) e inicializa o hambúrguer
// atual e os estoques de perecíveis. Retorna 1 em caso de sucesso e 0 em falha.
int iniciarfase(Fase *fase, NodoAVL *raiz, TabelaHash *cache) {
    if (fase == nullptr || raiz == nullptr || cache == nullptr)
        return 0;

    // Limpa os resultados de uma fase anterior antes de reinicializar
    limpar(&fase->pedidos);
    limpar(&fase->pedidoAtual);
    inicializa(&fase->pedidos);
    fase->cod = 0;
    fase->TurnoAtual = 0;
    fase->NumDePedidosIni = 3;

    // Paciência máxima de cada cliente (em turnos)
    fase->PacienciaDosClientes = 15;
    fase->PacienciaDoClienteAtual = fase->PacienciaDosClientes;
    // Estoque dos ingredientes preparados que não vencem
    fase->PreparadoSuiço = 0;
    fase->PreparadoCheddar = 0;
    fase->PreparadoCebola = 0;
    fase->PreparadoTomate = 0;
    fase->NumDePedidos = 0;

    // Cada cliente recebe um pedido sorteado; a receita vem da hash ou da AVL
    for (int i = 0; i < fase->NumDePedidosIni; i++) {
        const char *receita = nullptr;
        int codigo = sortearPedido(raiz, cache, &receita);
        if (!adicionarPedido(&fase->pedidos, codigo, receita)) {
            limpar(&fase->pedidos);
            return 0;
        }
    }
    fase->NumDePedidos = fase->NumDePedidosIni;

    // Hambúrguer em montagem e estoques de perecíveis começam vazios
    inicializa(&fase->pedidoAtual);
    inicializa(&fase->Bacon);
    inicializa(&fase->Carne);
    inicializa(&fase->Ovo);
    return 1;
}

// Compara o hambúrguer montado (pilha) com a receita do pedido (lista encadeada).
// Retorna 1 se forem idênticos e 0 caso contrário. Não destrói a pilha: usa
// obterNaPosicao para ler os elementos sem desempilhar.
int CompararHamburguerComPedido(const PilhaEnc *hamburguer, const Pedido *pedido) {
    if (hamburguer == nullptr || pedido == nullptr)
        return 0;

    int tamanhoHamburguer = tamanho(hamburguer);
    int tamanhoPedido = tamanho(&pedido->ingredientes);

    // Tamanhos diferentes: já não podem ser iguais
    if (tamanhoHamburguer != tamanhoPedido)
        return 0;

    // A lista da receita está na ordem de baixo para cima, enquanto a posição 0
    // da pilha é o topo. Por isso a pilha é lida do fim (base) para o início (topo),
    // enquanto a lista é percorrida do início ao fim.
    Nodo *nodo = pedido->ingredientes.ini;
    for (int i = tamanhoHamburguer - 1; i >= 0; i--) {
        Ingrediente ingrediente = {};
        if (!obterNaPosicao(hamburguer, i, &ingrediente)) {
            return 0;
        }
        // Basta um ingrediente diferente para o hambúrguer estar errado
        if (nodo == nullptr ||
            std::strcmp(ingrediente.tipo, nodo->ingrediente.tipo) != 0) {
            return 0;
        }
        nodo = nodo->prox;
    }

    return 1;
}

// Atualiza o estado da fase após uma ação do jogador que consome turno.
// Passos: avança o turno, reduz a paciência do cliente da frente, verifica se o
// hambúrguer atual completa o pedido e decide se o jogo continua, foi vencido
// ou perdido.
// Retorna STATE_VITORIA, STATE_DERROTA ou STATE_NOVO_JOGO (jogo em andamento).
GameState atualizarfase(Fase *fase) {
    if (fase == nullptr)
        return STATE_DERROTA;
    
    // Cada ação consome um turno e gasta 1 de paciência do cliente da frente
    fase->TurnoAtual++;
    if (fase->PacienciaDoClienteAtual > 0)
        fase->PacienciaDoClienteAtual--;
    Pedido pedidoAtual = {};
    // Se o hambúrguer montado bate com o pedido da frente da fila, o cliente é atendido
    if (frente(&fase->pedidos, &pedidoAtual) &&
        CompararHamburguerComPedido(&fase->pedidoAtual, &pedidoAtual)) {
        fase->NumDePedidos--;
        limpar(&fase->pedidoAtual); // prepara um prato vazio para o próximo cliente
        // Remove o cliente atendido da fila e libera a lista de ingredientes dele
        // (o dequeue só libera o nodo da fila; sem o destruir havia vazamento).
        Pedido descartado = {};
        if (dequeue(&fase->pedidos, &descartado))
            destruir(&descartado.ingredientes);
        // Era o último cliente: vitória
        if (fase->NumDePedidos <= 0)
            return STATE_VITORIA;
        // O próximo cliente começa com a paciência máxima
        fase->PacienciaDoClienteAtual = fase->PacienciaDosClientes;
    }

    // Paciência esgotada antes de concluir o pedido: derrota
    if (fase->PacienciaDoClienteAtual <= 0)
        return STATE_DERROTA;

    return STATE_NOVO_JOGO;
}