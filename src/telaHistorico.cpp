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
    liberarLista(&exibida);
    copiarLista(original, &exibida);
    ordenarHistorico(&exibida, ordemAtual);
    primeiraLinha = 0;
}

void abrirTelaHistorico(const ListaContigua *historico) {
    original = historico;
    ordemAtual = ORDEM_PONTUACAO;
    atualizarExibida();
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

    // Título e opções de ordenação (o critério atual fica destacado)
    DrawText("HISTORICO", 480, 24, 42, WHITE);
    DrawText("Ordenar por:", 250, 84, 22, LIGHTGRAY);
    DrawText("Pontuacao decrescente (P)", 420, 84, 22, ordemAtual == ORDEM_PONTUACAO ? GOLD : LIGHTGRAY);
    DrawText("Alfabetica (A)", 760, 84, 22, ordemAtual == ORDEM_ALFABETICA ? GOLD : LIGHTGRAY);

    // Cabeçalho da tabela
    DrawText("#", 250, 130, 22, SKYBLUE);
    DrawText("Nome", 330, 130, 22, SKYBLUE);
    DrawText("Pontuacao", 800, 130, 22, SKYBLUE);

    if (exibida.tam == 0) {
        DrawText("Nenhuma partida registrada ainda.", 330, 200, 24, LIGHTGRAY);
    }

    // Linhas visíveis
    int y = 170;
    for (int i = primeiraLinha; i < exibida.tam && i < primeiraLinha + LINHAS_VISIVEIS; i++) {
        DrawText(TextFormat("%d", i + 1), 250, y, 22, WHITE);
        DrawText(exibida.dados[i].nome, 330, y, 22, WHITE);
        DrawText(TextFormat("%d", exibida.dados[i].pontuacao), 800, y, 22, WHITE);
        y += 32;
    }

    if (exibida.tam > 0) {
        int ultimaVisivel = primeiraLinha + LINHAS_VISIVEIS;
        if (ultimaVisivel > exibida.tam) ultimaVisivel = exibida.tam;
        DrawText(TextFormat("Mostrando %d-%d de %d", primeiraLinha + 1, ultimaVisivel, exibida.tam),
                 250, 640, 18, LIGHTGRAY);
    }
    DrawText("Setas/PageUp/PageDown: rolar    ESC: voltar ao menu", 330, 676, 20, LIGHTGRAY);

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

void iniciarTelaFim(void) {
    nomeDigitado[0] = '\0';
    partidaSalva = false;
    falhaAoSalvar = false;
}

int atualizarDesenharTelaFim(bool vitoria, int pontuacao, ListaContigua *historico, const char *arqHistorico) {
    DrawText(vitoria ? "VITORIA" : "DERROTA", 480, 130, 42, WHITE);
    DrawText(vitoria ? "Voce atendeu todos os clientes." : "A paciencia do cliente acabou.",
             340, 200, 24, LIGHTGRAY);
    DrawText(TextFormat("Pontuacao: %d", pontuacao), 340, 240, 28, GOLD);

    if (partidaSalva) {
        if (falhaAoSalvar)
            DrawText("Nao foi possivel gravar o arquivo de historico.", 340, 330, 24, RED);
        else
            DrawText("Partida registrada no historico!", 340, 330, 24, GREEN);
        DrawText("Pressione ESC para voltar ao menu", 340, 390, 24, LIGHTGRAY);
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

    DrawText("Digite seu nome e pressione ENTER:", 340, 310, 24, WHITE);
    DrawRectangle(340, 350, 420, 44, DARKGRAY);
    DrawText(nomeDigitado, 350, 360, 26, WHITE);
    if (((int)(GetTime() * 2)) % 2 == 0) 
        DrawText("_", 350 + MeasureText(nomeDigitado, 26), 360, 26, WHITE);
    DrawText("ESC: registrar como Anonimo e voltar ao menu", 340, 420, 20, LIGHTGRAY);

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