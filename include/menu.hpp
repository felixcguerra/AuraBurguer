#ifndef MENU_HPP
#define MENU_HPP

#include "raylib.h"

typedef enum {
    MENU_ACAO_NENHUMA = 0,
    MENU_ACAO_NOVO_JOGO,
    MENU_ACAO_COMO_JOGAR,
    MENU_ACAO_HISTORICO,
    MENU_ACAO_SAIR
} MenuPrincipalAcao;

typedef enum {
    PAUSE_ACAO_NENHUMA = 0,
    PAUSE_ACAO_CONTINUAR,
    PAUSE_ACAO_INVENTARIO,
    PAUSE_ACAO_SALVAR,
    PAUSE_ACAO_SAIR_MENU,
    PAUSE_ACAO_COMO_JOGAR
} MenuPauseAcao;

/**
 * Desenha e processa o menu principal.
 */
MenuPrincipalAcao atualizarDesenharMenuPrincipal(void);

/**
 * Desenha e processa o menu de pause.
 */
MenuPauseAcao atualizarDesenharMenuPause(void);

/**
 * Desenha tela de ajuda.
 */
void desenharTelaAjuda(void);

/**
 * Desenha tela de inventário.
 */
void desenharTelaInventario(bool podeEquiparNaTelaAtual);

#endif
