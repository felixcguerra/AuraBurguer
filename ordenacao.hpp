#ifndef ORDENACAO_HPP
#define ORDENACAO_HPP

#include "historico.hpp"

// Critérios para exibir o histórico.
typedef enum {
    ORDEM_PONTUACAO,   // pontuação decrescente
    ORDEM_ALFABETICA   // nome de A a Z (sem diferenciar maiúsculas de minúsculas)
} OrdemHistorico;

// Ordena a lista (em memória) com MergeSort pelo critério escolhido.
// O algoritmo é estável: registros empatados mantêm a ordem em que estavam.
// Não mexe no arquivo; use sobre uma cópia (copiarLista) para preservar a original.
void ordenarHistorico(ListaContigua *l, OrdemHistorico ordem);

#endif