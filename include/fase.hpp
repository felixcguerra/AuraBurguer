#ifndef FASE_HPP
#define FASE_HPP

#include "Fila.hpp"
#include "Pilha.hpp"
#include "filaprioridade.hpp"

typedef enum {
    STATE_MENU,
    STATE_NOVO_JOGO,
    STATE_PAUSE,
    STATE_COMO_JOGAR,
    STATE_HISTORICO,
    STATE_DERROTA,
    STATE_VITORIA
} GameState;

typedef struct {
    int cod = 0;
    int NumDePedidosIni= 0;
    int NumDePedidos = 0;
    int PacienciaDosClientes = 0;
    int PacienciaDoClienteAtual = 0;
    int TurnoAtual = 0;
    int PreparadoSuiço=0;
    int PreparadoCheddar=0;
    int PreparadoCebola=0;
    int PreparadoTomate=0;
    FilaEnc pedidos = {};
    PilhaEnc pedidoAtual = {};
    FilaPrioridade Bacon = {};
    FilaPrioridade Carne = {};
    FilaPrioridade Ovo = {};
} Fase;

int iniciarfase(Fase *fase);
GameState atualizarfase(Fase *fase);

#endif
