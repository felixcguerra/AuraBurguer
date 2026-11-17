#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "historico.hpp"

#define CAPACIDADE_INICIAL 16

void iniciarLista(ListaContigua *l) {
    l->dados = (Registro *)malloc(CAPACIDADE_INICIAL * sizeof(Registro));
    l->tam = 0;
    l->cap = CAPACIDADE_INICIAL;
}

void inserirLista(ListaContigua *l, const char *nome, int pontuacao) {
    // Vetor cheio: dobra a capacidade (o custo médio por inserção continua O(1))
    if (l->tam == l->cap) {
        l->cap = l->cap * 2;
        l->dados = (Registro *)realloc(l->dados, l->cap * sizeof(Registro));
    }

    strncpy(l->dados[l->tam].nome, nome, TAM_NOME - 1);
    l->dados[l->tam].nome[TAM_NOME - 1] = '\0';
    l->dados[l->tam].pontuacao = pontuacao;
    l->tam++;
}

void copiarLista(const ListaContigua *origem, ListaContigua *copia) {
    copia->cap = origem->cap;
    copia->tam = origem->tam;
    copia->dados = (Registro *)malloc(copia->cap * sizeof(Registro));
    memcpy(copia->dados, origem->dados, origem->tam * sizeof(Registro));
}

void liberarLista(ListaContigua *l) {
    free(l->dados);
    l->dados = NULL;
    l->tam = 0;
    l->cap = 0;
}

int carregarHistorico(ListaContigua *l, const char *caminho) {
    FILE *arq = abrirArquivoLeitura(caminho);
    if (arq == NULL) // primeira execução: o arquivo ainda não existe
        return 1;

    char nome[TAM_NOME];
    int pontuacao;
    int status;

    // lerProximoRegistroHistorico: 1 = leu, 0 = fim do arquivo, -1 = formato inválido
    while ((status = lerProximoRegistroHistorico(arq, nome, &pontuacao)) == 1)
        inserirLista(l, nome, pontuacao);

    fclose(arq);
    return status == 0;
}

int salvarRegistro(const char *caminho, const char *nome, int pontuacao) {
    // Se o arquivo já existe e não termina em '\n', o novo nome ficaria colado na
    // última pontuação. Confere o último caractere antes de gravar.
    int precisaQuebra = 0;
    FILE *leitura = fopen(caminho, "rb");
    if (leitura != NULL) {
        if (fseek(leitura, -1, SEEK_END) == 0 && fgetc(leitura) != '\n')
            precisaQuebra = 1;
        fclose(leitura);
    }

    // "a" = acrescenta no final (cria o arquivo se não existir, nunca apaga nada)
    FILE *arq = fopen(caminho, "a");
    if (arq == NULL)
        return 0;

    if (precisaQuebra)
        fputc('\n', arq);
    fprintf(arq, "%s\n%d\n", nome, pontuacao);
    fclose(arq);
    return 1;
}

int registrarPartida(ListaContigua *l, const char *caminho, const char *nome, int pontuacao) {
    inserirLista(l, nome, pontuacao);
    return salvarRegistro(caminho, nome, pontuacao);
}