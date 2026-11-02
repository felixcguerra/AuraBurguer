#include "menu.hpp"

// Desenha o menu principal e trata a entrada do jogador (mouse e teclado).
// Retorna a ação escolhida, ou MENU_ACAO_NENHUMA se nada foi selecionado neste quadro.
MenuPrincipalAcao atualizarDesenharMenuPrincipal(void) {
    const char *opcoes[] = { "Novo jogo (N)", "Como jogar (A)", "Historico (H)", "Sair (S)" };
    Rectangle botoes[4];
    static int selecionado = 0; // 'static': lembra o botão selecionado entre um quadro e outro
    Vector2 mouse = GetMousePosition();

    DrawText("Burguer", 420, 140, 54, WHITE);

    // Calcula a posição de cada botão e o desenha (destacado se estiver selecionado)
    for (int i = 0; i < 4; i++) {
        botoes[i] = (Rectangle){ 460.0f, 225.0f + i * 70.0f, 280.0f, 54.0f };
        bool emCima = CheckCollisionPointRec(mouse, botoes[i]);
        if (emCima) selecionado = i; // passar o mouse por cima seleciona o botão
        DrawRectangleRec(botoes[i], (selecionado == i) ? DARKBLUE : DARKGRAY);
        DrawText(opcoes[i], (int)botoes[i].x + 18, (int)botoes[i].y + 15, 24, WHITE);
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

    // Fundo semitransparente para escurecer o jogo por trás
    DrawRectangle(350, 140, 500, 450, Fade(BLACK, 0.8f));
    DrawText("Pause", 560, 180, 40, WHITE);

    for (int i = 0; i < 3; i++) {
        botoes[i] = (Rectangle){ 430.0f, 260.0f + i * 70.0f, 340.0f, 54.0f };
        bool emCima = CheckCollisionPointRec(mouse, botoes[i]);
        if (emCima) selecionado = i;
        DrawRectangleRec(botoes[i], (selecionado == i) ? DARKGREEN : DARKGRAY);
        DrawText(opcoes[i], (int)botoes[i].x + 16, (int)botoes[i].y + 15, 24, WHITE);
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
    DrawText("AJUDA - BURGUER", 425, 24, 38, WHITE);

    // Dois painéis: ingredientes à esquerda, regras à direita
    DrawRectangle(32, 86, 435, 566, Fade(DARKGRAY, 0.75f));
    DrawRectangle(485, 86, 683, 566, Fade(DARKGRAY, 0.75f));

    // Painel esquerdo: lista de ingredientes e seus símbolos (Tabela 1 do enunciado)
    DrawText("INGREDIENTES E SIMBOLOS", 55, 108, 22, GOLD);
    DrawText("Pao (de cima): z", 55, 150, 18, WHITE);
    DrawText("Ketchup: k", 55, 182, 18, WHITE);
    DrawText("Mostarda: m", 55, 214, 18, WHITE);
    DrawText("Picles: p", 55, 246, 18, WHITE);
    DrawText("Alface: a", 55, 278, 18, WHITE);
    DrawText("Queijo suico: s", 55, 310, 18, WHITE);
    DrawText("Queijo cheddar: q", 55, 342, 18, WHITE);
    DrawText("Cebola: c", 270, 150, 18, WHITE);
    DrawText("Tomate: t", 270, 182, 18, WHITE);
    DrawText("Carne: n", 270, 214, 18, WHITE);
    DrawText("Ovo frito: o", 270, 246, 18, WHITE);
    DrawText("Bacon: b", 270, 278, 18, WHITE);

    // Painel direito: objetivo do jogo
    DrawText("OBJETIVO", 510, 108, 22, GOLD);
    DrawText("Prepare o pedido correto de cada cliente.", 510, 137, 18, WHITE);

    // Painel direito: como funcionam pedidos e fila
    DrawText("PEDIDOS E FILA", 510, 173, 22, GOLD);
    DrawText("Todos os clientes entram na fila no inicio, por ordem de chegada.", 510, 202, 17, WHITE);
    DrawText("Cada cliente recebe um pedido aleatorio entre os cadastrados na AVL;", 510, 226, 17, WHITE);
    DrawText("o pedido e localizado de forma eficiente. Atenda sempre quem esta", 510, 250, 17, WHITE);
    DrawText("na frente; ao concluir, esse cliente sai da fila.", 510, 274, 17, WHITE);

    // Painel direito: paciência, vitória e derrota
    DrawText("PACIENCIA E RESULTADO", 510, 310, 22, GOLD);
    DrawText("So o cliente da frente perde 1 de paciencia por turno; os demais", 510, 339, 17, WHITE);
    DrawText("aguardam sem perder paciencia. O limite maximo e definido pelo grupo.", 510, 363, 17, WHITE);
    DrawText("Derrota: a paciencia chega a zero antes de concluir o pedido.", 510, 387, 17, RED);
    DrawText("Vitoria: todos os clientes sao atendidos.", 510, 411, 17, GREEN);
    DrawText("A pontuacao e o numero de clientes atendidos.", 510, 435, 17, WHITE);

    // Painel direito: ações que consomem turno
    DrawText("ACOES - CADA UMA CONSOME 1 TURNO", 510, 471, 22, GOLD);
    DrawText("Preparar um ingrediente", 510, 500, 17, WHITE);
    DrawText("Adicionar um ingrediente ao hamburguer", 510, 524, 17, WHITE);
    DrawText("Remover o ingrediente do topo do hamburguer", 510, 548, 17, WHITE);

    // Consultas não gastam turno
    DrawText("CONSULTAS (estoque, pedido etc.) NAO CONSOMEM TURNO.", 510, 584, 17, SKYBLUE);
    DrawText("Pressione ESC para voltar ao menu", 410, 674, 22, LIGHTGRAY);
}

// Tela de seleção de dificuldade (extra opcional do enunciado).
// Por enquanto só desenha as opções; a leitura das teclas F/M/D ainda não foi
// implementada, então a função sempre retorna 0.
int SelecionarDificuldade(void) {//retorna um inteiro para a dificuldade que o jogador escolheu apertando o botão respectivo.
    DrawText("SELECIONE A DIFICULDADE", 400, 140, 38, WHITE);
    DrawText("Facil (F)", 480, 220, 24, WHITE);
    DrawText("Medio (M)", 480, 270, 24, WHITE);
    DrawText("Dificil (D)", 480, 320, 24, WHITE);
    return 0;
}