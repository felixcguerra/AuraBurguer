#include <cstring>
#include "raylib.h"
#include "telaHistorico.hpp"

#define LINHAS_VISIVEIS 14

// Tela de histórico

static const ListaContigua *original = nullptr; 
static ListaContigua exibida = { nullptr, 0, 0 }; 
static OrdemHistorico ordemAtual = ORDEM_PONTUACAO;
static int primeiraLinha = 0; 

static void atualizarExibida(void) {
    ordenarHistorico(&exibida, ordemAtual);
    primeiraLinha = 0;
}

void abrirTelaHistorico(const ListaContigua *historico) {
    original = historico;
    ordemAtual = ORDEM_PONTUACAO;
    liberarLista(&exibida);
    copiarLista(original, &exibida);
    ordenarHistorico(&exibida, ORDEM_ALFABETICA);
    ordenarHistorico(&exibida, ordemAtual);
}

void fecharTelaHistorico(void) {
    liberarLista(&exibida);
}

int atualizarDesenharTelaHistorico(void) {
    // Escolha do critério de ordenação (só reordena se o critério realmente mudou)
    if (IsKeyPressed(KEY_P) && ordemAtual != ORDEM_PONTUACAO) {
        ordemAtual = ORDEM_PONTUACAO;
        atualizarExibida();
    }
    if (IsKeyPressed(KEY_A) && ordemAtual != ORDEM_ALFABETICA) {
        ordemAtual = ORDEM_ALFABETICA;
        atualizarExibida();
    }

    // Rolagem pela lista (setas, PageUp/PageDown e roda do mouse)
    int ultimaPossivel = exibida.tam - LINHAS_VISIVEIS;
    if (ultimaPossivel < 0) ultimaPossivel = 0;

    float roda = GetMouseWheelMove();
    if (IsKeyPressed(KEY_DOWN) || roda < 0) primeiraLinha += 1;
    if (IsKeyPressed(KEY_UP) || roda > 0) primeiraLinha -= 1;
    if (IsKeyPressed(KEY_PAGE_DOWN)) primeiraLinha += LINHAS_VISIVEIS;
    if (IsKeyPressed(KEY_PAGE_UP)) primeiraLinha -= LINHAS_VISIVEIS;
    if (primeiraLinha > ultimaPossivel) primeiraLinha = ultimaPossivel;
    if (primeiraLinha < 0) primeiraLinha = 0;

    // Fundo centralizado da tela de histórico.
    const int larguraFundo = 900;
    const int alturaFundo = 720;
    const int fundoEsquerda = (GetScreenWidth() - larguraFundo) / 2;
    const int fundoSuperior = 0;
    const int fundoInferior = (GetScreenHeight() + alturaFundo) / 2;
    DrawRectangle(fundoEsquerda, fundoSuperior, larguraFundo,
                  fundoInferior - fundoSuperior, DARKGRAY);

    // Mantém o histórico centralizado horizontalmente em telas de tamanhos diferentes.
    const int larguraPainel = 800;
    const int painelEsquerda = (GetScreenWidth() - larguraPainel) / 2;
    const int painelDireita = painelEsquerda + larguraPainel;

    const char *titulo = "HISTORICO";
    const int tituloTamanho = 42;
    DrawText(titulo,
             GetScreenWidth() / 2 - MeasureText(titulo, tituloTamanho) / 2,
             24, tituloTamanho, WHITE);

    // Opções de ordenação (o critério atual fica destacado)
    DrawText("Ordenar por:", painelEsquerda, 84, 22, LIGHTGRAY);
    DrawText("Pontuacao decrescente (P)", painelEsquerda + 170, 84, 22,
             ordemAtual == ORDEM_PONTUACAO ? GOLD : LIGHTGRAY);
    DrawText("Alfabetica (A)", painelDireita - MeasureText("Alfabetica (A)", 22), 84, 22,
             ordemAtual == ORDEM_ALFABETICA ? GOLD : LIGHTGRAY);

    // Cabeçalho da tabela
    DrawText("#", painelEsquerda, 130, 22, SKYBLUE);
    DrawText("Nome", painelEsquerda + 80, 130, 22, SKYBLUE);
    DrawText("Pontuacao", painelDireita - MeasureText("Pontuacao", 22), 130, 22, SKYBLUE);

    if (exibida.tam == 0) {
        const char *mensagemVazia = "Nenhuma partida registrada ainda.";
        DrawText(mensagemVazia,
                 GetScreenWidth() / 2 - MeasureText(mensagemVazia, 24) / 2,
                 200, 24, LIGHTGRAY);
    }

    // Linhas visíveis
    int y = 170;
    for (int i = primeiraLinha; i < exibida.tam && i < primeiraLinha + LINHAS_VISIVEIS; i++) {
        DrawText(TextFormat("%d", i + 1), painelEsquerda, y, 22, WHITE);
        DrawText(exibida.dados[i].nome, painelEsquerda + 80, y, 22, WHITE);
        DrawText(TextFormat("%d", exibida.dados[i].pontuacao),
                 painelDireita - MeasureText(TextFormat("%d", exibida.dados[i].pontuacao), 22),
                 y, 22, WHITE);
        y += 32;
    }

    if (exibida.tam > 0) {
        int ultimaVisivel = primeiraLinha + LINHAS_VISIVEIS;
        if (ultimaVisivel > exibida.tam) ultimaVisivel = exibida.tam;
        const char *contagem = TextFormat("Mostrando %d-%d de %d",
                                          primeiraLinha + 1, ultimaVisivel, exibida.tam);
        DrawText(contagem, painelEsquerda, 640, 18, LIGHTGRAY);
    }
    const char *ajuda = "Setas/PageUp/PageDown: rolar    ESC: voltar ao menu";
    DrawText(ajuda,
             GetScreenWidth() / 2 - MeasureText(ajuda, 20) / 2,
             676, 20, LIGHTGRAY);

    if (IsKeyPressed(KEY_ESCAPE)) {
        fecharTelaHistorico();
        return 1;
    }
    return 0;
}

// Tela de fim de jogo

static char nomeDigitado[TAM_NOME] = "";
static bool partidaSalva = false;
static bool falhaAoSalvar = false;

static void desenharTextoFimCentralizado(const char *texto, int y, int tamanho, Color cor) {
    DrawText(texto, (GetScreenWidth() - MeasureText(texto, tamanho)) / 2, y, tamanho, cor);
}

void iniciarTelaFim(void) {
    nomeDigitado[0] = '\0';
    partidaSalva = false;
    falhaAoSalvar = false;
}

int atualizarDesenharTelaFim(bool vitoria, int pontuacao, ListaContigua *historico, const char *arqHistorico) {
    const int larguraPainel = 800;
    const int alturaPainel = 480;
    const int painelEsquerda = (GetScreenWidth() - larguraPainel) / 2;
    const int painelSuperior = (GetScreenHeight() - alturaPainel) / 2;
    DrawRectangle(painelEsquerda, painelSuperior, larguraPainel, alturaPainel, DARKGRAY);

    desenharTextoFimCentralizado(vitoria ? "VITORIA" : "DERROTA",
                                 painelSuperior + 40, 42, vitoria ? GREEN : RED);
    desenharTextoFimCentralizado(vitoria ? "Voce atendeu todos os clientes." : "A paciencia do cliente acabou.",
                                 painelSuperior + 105, 24, LIGHTGRAY);
    desenharTextoFimCentralizado(TextFormat("Pontuacao: %d", pontuacao),
                                 painelSuperior + 150, 28, GOLD);

    if (partidaSalva) {
        if (falhaAoSalvar)
            desenharTextoFimCentralizado("Nao foi possivel gravar o arquivo de historico.",
                                         painelSuperior + 245, 24, RED);
        else
            desenharTextoFimCentralizado("Partida registrada no historico!",
                                         painelSuperior + 245, 24, GREEN);
        desenharTextoFimCentralizado("Pressione ESC para voltar ao menu",
                                     painelSuperior + 315, 24, LIGHTGRAY);
        return IsKeyPressed(KEY_ESCAPE) ? 1 : 0;
    }

    int tamanho = (int)strlen(nomeDigitado);
    int tecla = GetCharPressed();
    while (tecla > 0) {
        if (tecla >= 32 && tecla <= 126 && tamanho < TAM_NOME - 1) {
            nomeDigitado[tamanho++] = (char)tecla;
            nomeDigitado[tamanho] = '\0';
        }
        tecla = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE) && tamanho > 0)
        nomeDigitado[tamanho - 1] = '\0';

    desenharTextoFimCentralizado("Digite seu nome e pressione ENTER:",
                                 painelSuperior + 215, 24, WHITE);
    const int larguraCampo = 420;
    const int campoEsquerda = (GetScreenWidth() - larguraCampo) / 2;
    DrawRectangle(campoEsquerda, painelSuperior + 260, larguraCampo, 44, GRAY);
    DrawText(nomeDigitado, campoEsquerda + 10, painelSuperior + 270, 26, WHITE);
    if (((int)(GetTime() * 2)) % 2 == 0) 
        DrawText("_", campoEsquerda + 10 + MeasureText(nomeDigitado, 26),
                 painelSuperior + 270, 26, WHITE);
    desenharTextoFimCentralizado("ESC: registrar como Anonimo e voltar ao menu",
                                 painelSuperior + 330, 20, LIGHTGRAY);

    if (IsKeyPressed(KEY_ENTER) && nomeDigitado[0] != '\0') {
        falhaAoSalvar = !registrarPartida(historico, arqHistorico, nomeDigitado, pontuacao);
        partidaSalva = true;
    }
    else if (IsKeyPressed(KEY_ESCAPE)) {
        registrarPartida(historico, arqHistorico, "Anonimo", pontuacao);
        return 1;
    }
    return 0;
}