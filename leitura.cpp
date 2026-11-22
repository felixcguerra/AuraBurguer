#include "leitura.hpp"
#include <cstring>
#include <cstdlib>

#define TAM_LINHA 256 // tamanho máximo de uma linha lida do arquivo

// Símbolos válidos dos ingredientes (Tabela 1 do enunciado).
#define SIMBOLOS_VALIDOS "zkmpasqctnob"

// Remove '\n' e '\r' do final da string (arquivos criados no Windows usam "\r\n").
static void removerQuebraDeLinha(char *s) {
    size_t n = strlen(s);
    while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r')) {
        s[--n] = '\0';
    }
}

// Lê a próxima linha que não esteja vazia, já sem a quebra de linha.
// Pular linhas vazias evita problemas com uma linha em branco no fim do arquivo.
// Retorna 1 se leu uma linha e 0 se o arquivo acabou.
static int lerLinhaNaoVazia(FILE *arq, char *linha, int tam) {
    while (fgets(linha, tam, arq) != nullptr) {
        removerQuebraDeLinha(linha);
        if (linha[0] != '\0')
            return 1;
    }
    return 0;
}

// Converte um texto em inteiro. Retorna 1 se o texto inteiro for um número válido
// (espaços no final são aceitos) e 0 caso contrário.
static int converterInteiro(const char *texto, int *valor) {
    char *fim;
    long v = strtol(texto, &fim, 10);
    if (fim == texto) // nenhum dígito encontrado
        return 0;
    while (*fim == ' ' || *fim == '\t')
        fim++;
    if (*fim != '\0') // sobrou lixo depois do número
        return 0;
    *valor = (int)v;
    return 1;
}

// Verifica se a receita é aceitável: não vazia, cabe no buffer (TAM_RECEITA)
// e usa apenas símbolos válidos de ingredientes.
// Retorna 1 se for válida e 0 caso contrário.
static int receitaValida(const char *receita) {
    size_t n = strlen(receita);
    if (n == 0 || n >= TAM_RECEITA)
        return 0;
    for (size_t i = 0; i < n; i++) {
        if (strchr(SIMBOLOS_VALIDOS, receita[i]) == nullptr)
            return 0;
    }
    return 1;
}

FILE *abrirArquivoLeitura(const char *caminho) {
    if (caminho == nullptr)
        return nullptr;
    return fopen(caminho, "r");
}

int lerProximoPedido(FILE *arq, int *codigo, char receita[TAM_RECEITA]) {
    char linha[TAM_LINHA];

    if (arq == nullptr || codigo == nullptr || receita == nullptr)
        return -1;

    // Linha 1: código do pedido. Se não há mais linhas, é o fim normal do arquivo.
    if (!lerLinhaNaoVazia(arq, linha, TAM_LINHA))
        return 0;
    if (!converterInteiro(linha, codigo))
        return -1;

    // Linha 2: receita. Um código sem receita indica arquivo malformado.
    if (!lerLinhaNaoVazia(arq, linha, TAM_LINHA))
        return -1;
    if (!receitaValida(linha)) // receita grande demais ou com símbolo desconhecido
        return -1;
    strcpy(receita, linha);

    return 1;
}

int lerProximoRegistroHistorico(FILE *arq, char nome[TAM_NOME], int *pontuacao) {
    char linha[TAM_LINHA];

    if (arq == nullptr || nome == nullptr || pontuacao == nullptr)
        return -1;

    // Linha 1: nome do jogador (pode ter espaços). Fim do arquivo aqui é normal.
    if (!lerLinhaNaoVazia(arq, linha, TAM_LINHA))
        return 0;
    strncpy(nome, linha, TAM_NOME - 1); // nomes maiores que 20 caracteres são cortados
    nome[TAM_NOME - 1] = '\0';

    // Linha 2: pontuação. Um nome sem pontuação indica arquivo malformado.
    if (!lerLinhaNaoVazia(arq, linha, TAM_LINHA))
        return -1;
    if (!converterInteiro(linha, pontuacao))
        return -1;

    return 1;
}