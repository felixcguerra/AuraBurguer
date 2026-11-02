#ifndef FASE_HPP
#define FASE_HPP

#include "Fila.hpp"
#include "Pilha.hpp"
#include "filaprioridade.hpp"
#include "AVL.hpp"
#include "hash.hpp"

// Estados possíveis do jogo (usados como máquina de estados no main).
typedef enum {
    STATE_MENU,        // menu principal
    STATE_NOVO_JOGO,   // partida em andamento
    STATE_PAUSE,       // menu de pausa
    STATE_COMO_JOGAR,  // tela de ajuda
    STATE_HISTORICO,   // tela de histórico
    STATE_DERROTA,     // fim de jogo: derrota
    STATE_VITORIA      // fim de jogo: vitória
} GameState;

// Guarda todo o estado de uma partida (fase).
struct Fase{
    int cod = 0;                      // identificador da fase
    int NumDePedidosIni= 0;           // número de pedidos (clientes) no início da fase
    int NumDePedidos = 0;             // quantos pedidos ainda faltam atender
    int PacienciaDosClientes = 0;     // paciência máxima de cada cliente (em turnos)
    int PacienciaDoClienteAtual = 0;  // paciência restante do cliente da frente da fila
    int TurnoAtual = 0;               // turno atual (cada ação consome um turno)
    // Ingredientes com preparo e sem validade: 1 = preparado, 0 = não preparado
    int PreparadoSuiço=0;
    int PreparadoCheddar=0;
    int PreparadoCebola=0;
    int PreparadoTomate=0;
    FilaEnc pedidos = {};             // fila de clientes/pedidos (FIFO)
    PilhaEnc pedidoAtual = {};        // hambúrguer sendo montado (pilha, LIFO)
    // Estoques dos ingredientes perecíveis (heaps mínimos)
    FilaPrioridade Bacon = {};
    FilaPrioridade Carne = {};
    FilaPrioridade Ovo = {};
};

// Prepara uma fase nova. Os pedidos dos clientes são sorteados e obtidos pela
// hash (cache); se não estiverem nela, são buscados na AVL.
// Retorna 1 em caso de sucesso e 0 em falha.
int iniciarfase(Fase *fase, NodoAVL *raiz, TabelaHash *cache);
// Atualiza a fase após uma ação que consome turno; retorna o próximo estado do jogo.
GameState atualizarfase(Fase *fase);

#endif