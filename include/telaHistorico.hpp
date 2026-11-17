#ifndef TELAHISTORICO_HPP
#define TELAHISTORICO_HPP

#include "historico.hpp"
#include "ordenacao.hpp"

// ---------- Tela de histórico (aberta pelo menu principal) ----------

// Chamar uma vez ao entrar na tela: monta a exibição ordenada por pontuação.
void abrirTelaHistorico(const ListaContigua *historico);

// Desenha e trata a tela a cada quadro. Teclas: P = pontuação decrescente,
// A = ordem alfabética, setas/PageUp/PageDown/roda do mouse = rolar.
// Retorna 1 quando o jogador pede para voltar ao menu (ESC), 0 caso contrário.
int atualizarDesenharTelaHistorico(void);

// Libera a cópia usada na exibição (pode ser chamada mais de uma vez).
void fecharTelaHistorico(void);

// ---------- Tela de fim de jogo (vitória ou derrota) ----------

// Chamar uma vez quando a partida termina: limpa o campo de nome.
void iniciarTelaFim(void);

// Mostra o resultado e a pontuação, pede o nome do jogador e registra a partida
// no histórico (lista + arquivo). ESC antes de digitar o nome registra como "Anonimo".
// Retorna 1 quando o jogador terminou e deve voltar ao menu, 0 caso contrário.
int atualizarDesenharTelaFim(bool vitoria, int pontuacao, ListaContigua *historico, const char *arqHistorico);

#endif