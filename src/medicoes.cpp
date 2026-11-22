#include <cstdio>
#include "medicoes.hpp"

MetricasExecucao g_metricas;

void iniciarMedicoes(void) {
    g_metricas = {};
    
    // Cria ou sobrescreve o arquivo medicoes.log a cada execução
    FILE *arq = fopen("medicoes.log", "w");
    if (arq != NULL) {
        fprintf(arq, "========================================\n");
        fprintf(arq, "      LOG DE MEDICOES E DESEMPENHO      \n");
        fprintf(arq, "========================================\n\n");
        fclose(arq);
    }
}

void incrementarNosInseridos(void) {
    g_metricas.avlNosInseridos++;
}

void registrarRotacoes(int qtd) {
    g_metricas.avlRotacoes += qtd; // Contabiliza 1 para simples ou 2 para dupla
}

void registrarBuscaAVL(int comparacoes, double tempo) {
    g_metricas.avlTotalBuscas++;
    g_metricas.avlTotalComparacoes += comparacoes;
    g_metricas.avlTempoTotalBuscas += tempo;
}

void registrarAcessoHash(bool hit) {
    if (hit) g_metricas.hashHits++;
    else g_metricas.hashMisses++;
}

void incrementarColisaoHash(void) {
    g_metricas.hashColisoesInsercao++;
}

void registrarMetricasCarga(int altura, double tempoCarga, int entradasHistorico) {
    g_metricas.avlAltura = altura;
    g_metricas.avlTempoCarga = tempoCarga;
    g_metricas.historicoEntradasCarregadas = entradasHistorico;

    FILE *arq = fopen("medicoes.log", "a");
    if (arq != NULL) {
        fprintf(arq, "--- AVL:Carregamento ---\n");
        fprintf(arq, "Nos Inseridos: %d\n", g_metricas.avlNosInseridos);
        fprintf(arq, "Altura da Arvore: %d\n", g_metricas.avlAltura);
        fprintf(arq, "Numero de Rotacoes: %d\n", g_metricas.avlRotacoes);
        fprintf(arq, "Tempo de Carregamento: %.6f s\n", g_metricas.avlTempoCarga);
        fprintf(arq, "--Historico:carregamento--\n", g_metricas.avlTempoCarga);
        fprintf(arq, "Historico - Entradas Carregadas: %d\n\n", g_metricas.historicoEntradasCarregadas);
        fclose(arq);
    }
}

void atualizarDadosHash(int elementos, int tamanhoTabela) {
    g_metricas.hashElementos = elementos;
    g_metricas.hashTamanhoTabela = tamanhoTabela;
}

void registrarMetricaOrdenacao(double tempoOrdenacao, int n, const char *criterio) {
    g_metricas.historicoTempoOrdenacao = tempoOrdenacao;
    g_metricas.historicoFoiOrdenado = true;

    FILE *arq = fopen("medicoes.log", "a");
    if (arq != NULL) {
        fprintf(arq, "--- ORDENACAO DO HISTORICO ---\n");
        fprintf(arq, "Criterio: %s | Registros ordenados: %d\n", criterio, n);
        fprintf(arq, "Tempo de Execucao (MergeSort): %.6f s\n\n", tempoOrdenacao);
        fclose(arq);
    }
}

void gerarEGravarRelatorioDesempenho(void) {
    double mediaComparacoes = 0.0;
    if (g_metricas.avlTotalBuscas > 0) {
        mediaComparacoes = (double)g_metricas.avlTotalComparacoes / g_metricas.avlTotalBuscas;
    }

    int totalAcessosHash = g_metricas.hashHits + g_metricas.hashMisses;
    double taxaAcertoHash = 0.0;
    if (totalAcessosHash > 0) {
        taxaAcertoHash = ((double)g_metricas.hashHits / totalAcessosHash) * 100.0;
    }

    double fatorCargaHash = 0.0;
    if (g_metricas.hashTamanhoTabela > 0) {
        fatorCargaHash = (double)g_metricas.hashElementos / g_metricas.hashTamanhoTabela;
    }

    FILE *arq = fopen("medicoes.log", "a");
    if (arq != NULL) {
        fprintf(arq, "========================================\n");
        fprintf(arq, "       RELATORIO DE DESEMPENHO          \n");
        fprintf(arq, "========================================\n");
        fprintf(arq, "1. AVL - BUSCAS DURANTE A SESSAO:\n");
        fprintf(arq, "   - Total de buscas: %d\n", g_metricas.avlTotalBuscas);
        fprintf(arq, "   - Total de comparacoes: %d\n", g_metricas.avlTotalComparacoes);
        fprintf(arq, "   - Media de comparacoes por busca: %.2f\n", mediaComparacoes);
        fprintf(arq, "   - Tempo acumulado de buscas: %.6f s\n\n", g_metricas.avlTempoTotalBuscas);

        fprintf(arq, "2. TABELA HASH - ESTATISTICAS DE CACHE:\n");
        fprintf(arq, "   - Hits: %d | Misses: %d\n", g_metricas.hashHits, g_metricas.hashMisses);
        fprintf(arq, "   - Taxa de acerto: %.2f%%\n", taxaAcertoHash);
        fprintf(arq, "   - Fator de carga (lambda): %.4f\n", fatorCargaHash);
        fprintf(arq, "   - Total de colisoes em insercoes: %d\n\n", g_metricas.hashColisoesInsercao);

        fprintf(arq, "3. ORDENACAO DO HISTORICO:\n");
        if (g_metricas.historicoFoiOrdenado) {
            fprintf(arq, "   - Tempo da ultima ordenacao: %.6f s\n", g_metricas.historicoTempoOrdenacao);
        } else {
            fprintf(arq, "   - Opcao Historico nao foi acessada durante esta sessao.\n");
        }
        fprintf(arq, "========================================\n\n");
        fclose(arq);
    }
}