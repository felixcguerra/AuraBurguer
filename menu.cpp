#include "menu.hpp"
#include "hud.hpp"

static const float LARGURA_LAYOUT_MENU = 1200.0f;
static const float ALTURA_LAYOUT_MENU = 720.0f;

typedef struct {
    float escala;
    float deslocamentoX;
    float deslocamentoY;
} LayoutMenu;

static LayoutMenu obterLayoutMenu(void) {
    const float escalaHorizontal = GetScreenWidth() / LARGURA_LAYOUT_MENU;
    const float escalaVertical = GetScreenHeight() / ALTURA_LAYOUT_MENU;
    const float escala = escalaHorizontal < escalaVertical ? escalaHorizontal : escalaVertical;

    LayoutMenu layout = {
        escala,
        (GetScreenWidth() - LARGURA_LAYOUT_MENU * escala) / 2.0f,
        (GetScreenHeight() - ALTURA_LAYOUT_MENU * escala) / 2.0f
    };
    return layout;
}

static Rectangle transformarRetanguloMenu(LayoutMenu layout, float x, float y, float largura, float altura) {
    return (Rectangle){
        layout.deslocamentoX + x * layout.escala,
        layout.deslocamentoY + y * layout.escala,
        largura * layout.escala,
        altura * layout.escala
    };
}

static void desenharTextoMenu(LayoutMenu layout, const char *texto, int x, int y, int tamanho, Color cor) {
    DrawText(
        texto,
        (int)(layout.deslocamentoX + x * layout.escala),
        (int)(layout.deslocamentoY + y * layout.escala),
        (int)(tamanho * layout.escala),
        cor
    );
}

static int centralizarTextoMenu(const char *texto, int tamanho) {
    return (int)((LARGURA_LAYOUT_MENU - MeasureText(texto, tamanho)) / 2);
}

// Desenha o menu principal e trata a entrada do jogador (mouse e teclado).
// Retorna a ação escolhida, ou MENU_ACAO_NENHUMA se nada foi selecionado neste quadro.
MenuPrincipalAcao atualizarDesenharMenuPrincipal(void) {
    const char *opcoes[] = { "Novo jogo (N)", "Como jogar (A)", "Historico (H)", "Sair (S)" };
    Rectangle botoes[4];
    static int selecionado = 0; // 'static': lembra o botão selecionado entre um quadro e outro
    Vector2 mouse = GetMousePosition();
    LayoutMenu layout = obterLayoutMenu();

    const int tamanhoTitulo = 54;
    desenharTextoMenu(layout, "D     do", centralizarTextoMenu("D     do", tamanhoTitulo), 60, tamanhoTitulo, WHITE);
    desenharTextoMenu(layout, "    urguer", centralizarTextoMenu("    urguer", tamanhoTitulo), 120, tamanhoTitulo, WHITE);

    const int xLinhaDado = centralizarTextoMenu("D     do", tamanhoTitulo);
    const int xLinhaBurguer = centralizarTextoMenu("    urguer", tamanhoTitulo);
    PrintarIngrediente("a", {
        layout.deslocamentoX + (xLinhaDado + MeasureText("D   ", tamanhoTitulo)) * layout.escala - 5.0f,
        layout.deslocamentoY + 87.0f * layout.escala
    }, 0.5f * layout.escala, WHITE);
    PrintarIngrediente("b", {
        layout.deslocamentoX + (xLinhaBurguer + MeasureText("  ", tamanhoTitulo)) * layout.escala,
        layout.deslocamentoY + 147.0f * layout.escala
    }, 0.5f * layout.escala, WHITE);

    // Decoração lateral em zigue-zague.
    PrintarIngrediente("z", {
        layout.deslocamentoX + 130.0f * layout.escala,
        layout.deslocamentoY + 170.0f * layout.escala
    }, 0.8f * layout.escala, WHITE);
    PrintarIngrediente("n", {
        layout.deslocamentoX + 220.0f * layout.escala,
        layout.deslocamentoY + 315.0f * layout.escala
    }, 0.8f * layout.escala, WHITE);
    PrintarIngrediente("p", {
        layout.deslocamentoX + 130.0f * layout.escala,
        layout.deslocamentoY + 460.0f * layout.escala
    }, 0.8f * layout.escala, WHITE);

    PrintarIngrediente("t", {
        layout.deslocamentoX + 1070.0f * layout.escala,
        layout.deslocamentoY + 170.0f * layout.escala
    }, 0.8f * layout.escala, WHITE);
    PrintarIngrediente("q", {
        layout.deslocamentoX + 980.0f * layout.escala,
        layout.deslocamentoY + 315.0f * layout.escala
    }, 0.8f * layout.escala, WHITE);
    PrintarIngrediente("o", {
        layout.deslocamentoX + 1070.0f * layout.escala,
        layout.deslocamentoY + 460.0f * layout.escala
    }, 0.8f * layout.escala, WHITE);

    // Calcula a posição de cada botão e o desenha (destacado se estiver selecionado)
    for (int i = 0; i < 4; i++) {
        botoes[i] = transformarRetanguloMenu(layout, 460.0f, 225.0f + i * 70.0f, 280.0f, 54.0f);
        bool emCima = CheckCollisionPointRec(mouse, botoes[i]);
        if (emCima) selecionado = i; // passar o mouse por cima seleciona o botão
        DrawRectangleRec(botoes[i], (selecionado == i) ? DARKBLUE : DARKGRAY);
        desenharTextoMenu(layout, opcoes[i], 478, 240 + i * 70, 24, WHITE);
    }

    // Navegação pelas setas; o módulo faz a seleção "dar a volta" nas pontas
    if (IsKeyPressed(KEY_DOWN)) selecionado = (selecionado + 1) % 4;
    if (IsKeyPressed(KEY_UP)) selecionado = (selecionado + 3) % 4;

    // Atalhos de teclado para cada opção
    if (IsKeyPressed(KEY_N)) return MENU_ACAO_NOVO_JOGO;
    if (IsKeyPressed(KEY_A)) return MENU_ACAO_COMO_JOGAR;
    if (IsKeyPressed(KEY_H)) return MENU_ACAO_HISTORICO;
    if (IsKeyPressed(KEY_S)) return MENU_ACAO_SAIR;

    // Clique do mouse: retorna a ação do botão clicado
    // (o +1 existe porque o valor 0 do enum é MENU_ACAO_NENHUMA)
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        for (int i = 0; i < 4; i++) {
            if (!CheckCollisionPointRec(mouse, botoes[i])) continue;
            return (MenuPrincipalAcao)(i + 1);
        }
    }

    // ENTER confirma o botão atualmente selecionado
    if (IsKeyPressed(KEY_ENTER)) {
        return (MenuPrincipalAcao)(selecionado + 1);
    }

    return MENU_ACAO_NENHUMA;
}

// Desenha o menu de pausa sobre o jogo e trata a entrada do jogador.
// Funciona como o menu principal, mas com três opções.
MenuPauseAcao atualizarDesenharMenuPause(void) {
    const char *opcoes[] = { "Continuar (C)", "Como jogar (A)", "Sair (S)" };
    // Ações na mesma ordem dos botões, para acessar por índice
    const MenuPauseAcao acoes[] = {
        PAUSE_ACAO_CONTINUAR,
        PAUSE_ACAO_COMO_JOGAR,
        PAUSE_ACAO_SAIR_MENU
    };
    Rectangle botoes[3];
    static int selecionado = 0; // lembra a seleção entre os quadros
    Vector2 mouse = GetMousePosition();
    LayoutMenu layout = obterLayoutMenu();

    // Fundo semitransparente para escurecer o jogo por trás
    DrawRectangleRec(transformarRetanguloMenu(layout, 350, 140, 500, 450), Fade(BLACK, 0.8f));
    desenharTextoMenu(layout, "Pause", centralizarTextoMenu("Pause", 40), 180, 40, WHITE);

    for (int i = 0; i < 3; i++) {
        botoes[i] = transformarRetanguloMenu(layout, 430.0f, 260.0f + i * 70.0f, 340.0f, 54.0f);
        bool emCima = CheckCollisionPointRec(mouse, botoes[i]);
        if (emCima) selecionado = i;
        DrawRectangleRec(botoes[i], (selecionado == i) ? DARKGREEN : DARKGRAY);
        desenharTextoMenu(layout, opcoes[i], 446, 275 + i * 70, 24, WHITE);
    }

    // Navegação circular pelas setas
    if (IsKeyPressed(KEY_DOWN)) selecionado = (selecionado + 1) % 3;
    if (IsKeyPressed(KEY_UP)) selecionado = (selecionado + 2) % 3;

    // Atalhos de teclado
    if (IsKeyPressed(KEY_C)) return PAUSE_ACAO_CONTINUAR;
    if (IsKeyPressed(KEY_A)) return PAUSE_ACAO_COMO_JOGAR;
    if (IsKeyPressed(KEY_S)) return PAUSE_ACAO_SAIR_MENU;

    // Clique do mouse
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        for (int i = 0; i < 3; i++) {
            if (!CheckCollisionPointRec(mouse, botoes[i])) continue;
            return acoes[i];
        }
    }

    // ENTER confirma a opção selecionada
    if (IsKeyPressed(KEY_ENTER)) {
        return acoes[selecionado];
    }

    return PAUSE_ACAO_NENHUMA;
}

// Desenha a tela estática "Como jogar": ingredientes e símbolos, objetivo,
// regras de fila e paciência, e as ações disponíveis. Não tem interação própria;
// quem chama decide quando sair (ESC).
void desenharTelaAjuda(void) {
    LayoutMenu layout = obterLayoutMenu();
    desenharTextoMenu(layout, "AJUDA - DADO BURGUER", centralizarTextoMenu("AJUDA - DADOBURGUER", 38), 24, 38, WHITE);

    // Dois painéis: ingredientes à esquerda, regras à direita
    DrawRectangleRec(transformarRetanguloMenu(layout, 32, 86, 435, 566), Fade(DARKGRAY, 0.75f));
    DrawRectangleRec(transformarRetanguloMenu(layout, 485, 86, 683, 566), Fade(DARKGRAY, 0.75f));

    // Painel esquerdo: lista de ingredientes e seus símbolos (Tabela 1 do enunciado)
    desenharTextoMenu(layout, "INGREDIENTES E SIMBOLOS", 55, 108, 22, GOLD);
    desenharTextoMenu(layout, "Pao (de cima): z", 55, 150, 18, WHITE);
    desenharTextoMenu(layout, "Ketchup: k", 55, 182, 18, WHITE);
    desenharTextoMenu(layout, "Mostarda: m", 55, 214, 18, WHITE);
    desenharTextoMenu(layout, "Picles: p", 55, 246, 18, WHITE);
    desenharTextoMenu(layout, "Alface: a", 55, 278, 18, WHITE);
    desenharTextoMenu(layout, "Queijo suico: s", 55, 310, 18, WHITE);
    desenharTextoMenu(layout, "Queijo cheddar: q", 55, 342, 18, WHITE);
    desenharTextoMenu(layout, "Cebola: c", 270, 150, 18, WHITE);
    desenharTextoMenu(layout, "Tomate: t", 270, 182, 18, WHITE);
    desenharTextoMenu(layout, "Carne: n", 270, 214, 18, WHITE);
    desenharTextoMenu(layout, "Ovo frito: o", 270, 246, 18, WHITE);
    desenharTextoMenu(layout, "Bacon: b", 270, 278, 18, WHITE);

    // Painel direito: objetivo do jogo
    desenharTextoMenu(layout, "OBJETIVO", 510, 108, 22, GOLD);
    desenharTextoMenu(layout, "Prepare o pedido correto de cada cliente.", 510, 137, 18, WHITE);

    // Painel direito: como funcionam pedidos e fila
    desenharTextoMenu(layout, "PEDIDOS E FILA", 510, 173, 22, GOLD);
    desenharTextoMenu(layout, "Todos os clientes entram na fila no inicio, por ordem de chegada.", 510, 202, 17, WHITE);
    desenharTextoMenu(layout, "Cada cliente recebe um pedido aleatorio entre os cadastrados na AVL;", 510, 226, 17, WHITE);
    desenharTextoMenu(layout, "o pedido e localizado de forma eficiente. Atenda sempre quem esta", 510, 250, 17, WHITE);
    desenharTextoMenu(layout, "na frente; ao concluir, esse cliente sai da fila.", 510, 274, 17, WHITE);

    // Painel direito: paciência, vitória e derrota
    desenharTextoMenu(layout, "PACIENCIA E RESULTADO", 510, 310, 22, GOLD);
    desenharTextoMenu(layout, "So o cliente da frente perde 1 de paciencia por turno; os demais", 510, 339, 17, WHITE);
    desenharTextoMenu(layout, "aguardam sem perder paciencia. O limite maximo e definido pelo grupo.", 510, 363, 17, WHITE);
    desenharTextoMenu(layout, "Derrota: a paciencia chega a zero antes de concluir o pedido.", 510, 387, 17, RED);
    desenharTextoMenu(layout, "Vitoria: todos os clientes sao atendidos.", 510, 411, 17, GREEN);
    desenharTextoMenu(layout, "A pontuacao e o numero de clientes atendidos.", 510, 435, 17, WHITE);

    // Painel direito: ações que consomem turno
    desenharTextoMenu(layout, "ACOES - CADA UMA CONSOME 1 TURNO", 510, 471, 22, GOLD);
    desenharTextoMenu(layout, "Preparar um ingrediente", 510, 500, 17, WHITE);
    desenharTextoMenu(layout, "Adicionar um ingrediente ao hamburguer", 510, 524, 17, WHITE);
    desenharTextoMenu(layout, "Remover o ingrediente do topo do hamburguer", 510, 548, 17, WHITE);

    // Consultas não gastam turno
    desenharTextoMenu(layout, "CONSULTAS (estoque, pedido etc.) NAO CONSOMEM TURNO.", 510, 584, 17, SKYBLUE);
    desenharTextoMenu(layout, "Pressione ESC para voltar ao menu", centralizarTextoMenu("Pressione ESC para voltar ao menu", 22), 674, 22, LIGHTGRAY);
}

// Tela de seleção de dificuldade (extra opcional do enunciado).
// Por enquanto só desenha as opções; a leitura das teclas F/M/D ainda não foi
// implementada, então a função sempre retorna 0.
int SelecionarDificuldade(void) {//retorna um inteiro para a dificuldade que o jogador escolheu apertando o botão respectivo.
    LayoutMenu layout = obterLayoutMenu();
    desenharTextoMenu(layout, "SELECIONE A DIFICULDADE", centralizarTextoMenu("SELECIONE A DIFICULDADE", 38), 140, 38, WHITE);
    desenharTextoMenu(layout, "Facil (F)", centralizarTextoMenu("Facil (F)", 24), 220, 24, WHITE);
    desenharTextoMenu(layout, "Medio (M)", centralizarTextoMenu("Medio (M)", 24), 270, 24, WHITE);
    desenharTextoMenu(layout, "Dificil (D)", centralizarTextoMenu("Dificil (D)", 24), 320, 24, WHITE);
    return 0;
}