#ifndef MEDICOES_HPP
#define MEDICOES_HPP

#include "timer.hpp"

typedef struct {
    // 5.1 Carregamento da AVL e Histórico
    int avlNosInseridos;
    int avlAltura;
    int avlRotacoes;
    double avlTempoCarga;
    int historicoEntradasCarregadas;

    // 5.2 Buscas na AVL (durante a partida)
    int avlTotalBuscas;
    int avlTotalComparacoes;
    double avlTempoTotalBuscas;

    // 5.2 Tabela Hash (Cache)
    int hashHits;
    int hashMisses;
    int hashColisoesInsercao;
    int hashElementos;
    int hashTamanhoTabela;

    // 5.2 Ordenação do Histórico
    double historicoTempoOrdenacao;
    bool historicoFoiOrdenado;
} MetricasExecucao;

extern MetricasExecucao g_metricas;

void iniciarMedicoes(void);
void incrementarNosInseridos(void);
void registrarRotacoes(int qtd);
void registrarBuscaAVL(int comparacoes, double tempo);
void registrarAcessoHash(bool hit);
void incrementarColisaoHash(void);
void registrarMetricasCarga(int altura, double tempoCarga, int entradasHistorico);
void atualizarDadosHash(int elementos, int tamanhoTabela);
void registrarMetricaOrdenacao(double tempoOrdenacao, int n, const char *criterio);
void gerarEGravarRelatorioDesempenho(void);

#endif