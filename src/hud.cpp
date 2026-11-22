#include "hud.hpp"
#include "raylib.h"
#include <cctype>

static Texture2D spritesIngredientes = {};
static bool spritesIngredientesCarregados = false;

void carregarSpritesIngredientes() {
    if (spritesIngredientesCarregados)
        return;

    spritesIngredientes = LoadTexture("assets/ingredientsprites.png");
    spritesIngredientesCarregados = IsTextureValid(spritesIngredientes);
    if (!spritesIngredientesCarregados) {
        TraceLog(LOG_ERROR, "Nao foi possivel carregar assets/ingredientsprites.png");
    }
}

void descarregarSpritesIngredientes() {
    if (spritesIngredientesCarregados) {
        UnloadTexture(spritesIngredientes);
        spritesIngredientes = {};
        spritesIngredientesCarregados = false;
    }
}

// Recebe um texto e desenha um recorte da spritesheet para cada caractere valido.
void PrintarIngrediente(const char *texto, Vector2 posicao, float escala, Color cor) {
    if (texto == nullptr)
        return;

    carregarSpritesIngredientes();
    if (!spritesIngredientesCarregados)
        return;

    for (const unsigned char *caractere = (const unsigned char *)texto;
         *caractere != '\0'; ++caractere) {
        char ingrediente = *caractere;
        Rectangle recorte = {};
        float larguraDesenho = 160.0f * escala;
        float alturaDesenho = 100.0f * escala;

        switch (ingrediente) {
            case 'x': recorte = { 110.0f, 100.0f, 110.0f, 60.0f }; break; // pao base
            case 'z': recorte = { 220.0f, 100.0f, 110.0f, 60.0f }; break; // pao
            case 'n': recorte = { 107.0f,   0.0f, 106.0f, 60.0f }; break; // carne pronta
            case 'N': recorte = { 212.0f,   0.0f, 100.0f, 60.0f }; break; // carne crua
            case '9': recorte = { 317.0f,   0.0f, 106.0f, 60.0f }; break; // carne ponto menos
            case 'o': recorte = {   40.0f, 545.0f, 120.0f, 80.0f }; break; // ovo
            case 'b': recorte = { 210.0f, 535.0f, 110.0f, 100.0f }; break; // bacon
            case 'B': recorte = { 330.0f, 535.0f, 110.0f, 100.0f }; break; // bacon
            
            
            case 'p': recorte = { 565.0f, 90.0f, 90.0f, 70.0f }; break; // picles
            case 'a': recorte = { 436.0f, 110.0f, 115.0f, 50.0f }; break; // alface
            case 's': recorte = {   03.0f, 185.0f, 100.0f, 50.0f }; break; // suico
            case 'q': recorte = { 135.0f, 185.0f, 100.0f, 50.0f }; break; // cheddar
            case 'c': recorte = { 330.0f, 110.0f, 100.0f, 50.0f }; break; // cebola
            case 't': recorte = { 665.0f, 111.0f, 100.0f, 50.0f }; break; // tomate
            case 'm': recorte = {   40.0f, 407.0f, 96.0f, 50.0f }; break; // mostarda
            case 'M': recorte = { 0.0f, 345.0f, 40.0f, 110.0f }; // mostarda bisnaga
                      larguraDesenho = 40.0f * escala;
                      alturaDesenho = 110.0f * escala;
                      break;
            case 'k': recorte = {   40.0f, 295.0f, 96.0f, 50.0f }; break; // ketchup
            case 'K': recorte = { 0.0f, 233.0f, 40.0f, 110.0f }; // ketchup bisnaga
                      larguraDesenho = 40.0f * escala;
                      alturaDesenho = 110.0f * escala;
                      break;
            default: continue;
        }

        Rectangle destino = {
            posicao.x-larguraDesenho/2.0f,
            posicao.y-alturaDesenho/2.0f,
            larguraDesenho,
            alturaDesenho
        };
        Vector2 origem = { 0.0f, 0.0f };
        DrawTexturePro(spritesIngredientes, recorte, destino, origem, 0.0f, cor);
        posicao.x += larguraDesenho + 10.0f;
    }
}

// Desenha na tela o hambúrguer que o jogador está montando.
// (posX, posY) é o ponto de referência onde a pilha é desenhada;
// os ingredientes são empilhados visualmente de baixo para cima.
void DesenharHamburger(const PilhaEnc *pilha, int posX, int posY) {
    if (pilha == nullptr)
        return;

    const int fonte = 20;        // tamanho da fonte dos símbolos
    const int espacamento = 20;  // distância vertical entre ingredientes
    const int quantidade = tamanho(pilha);
    // Ponto inicial do desenho, subindo conforme a quantidade de ingredientes
    int y = posY;
    PrintarIngrediente("x",{ (float)posX, (float)y },1.5f);
    y -= espacamento;
    for (int i = quantidade - 1; i >= 0; i--) {
        Ingrediente ingrediente = {};
        // Se a leitura falhar, registra o erro no log do raylib e interrompe o desenho
        if (!obterNaPosicao(pilha, i, &ingrediente)) {
            TraceLog(LOG_ERROR, "Falha ao ler a pilha do hamburger para desenhar.");
            return;
        }
        PrintarIngrediente(ingrediente.tipo,{ (float)posX, (float)y},1.5f);
        y -= espacamento;
        

    }
       
    }
    

// Desenha o pedido do cliente que está na frente da fila,
// mostrando os ingredientes exigidos na ordem da receita.
void DesenharPedido(const FilaEnc *fila, int posX, int posY) {
    if (fila == nullptr)
        return;

    Pedido pedido = {};
    // Se a fila está vazia, não há pedido para mostrar
    if (!frente(fila, &pedido))
        return;


    const int espacamento = 60;
    int y = posY - espacamento;

    // O pão de baixo fica na base do desenho
    PrintarIngrediente("x", { (float)posX, (float)y },0.8f);
    // Percorre a receita e desenha cada ingrediente uma linha acima do anterior;
    // cada ingrediente usa um recorte proprio da spritesheet
    Nodo *nodo = pedido.ingredientes.ini;
    while (nodo != nullptr) {
        y -= espacamento;
        PrintarIngrediente(nodo->ingrediente.tipo,
                           { (float)posX, (float)y },0.8f);
        nodo = nodo->prox;
    }
}
void DesenharHeap(const FilaPrioridade *heap, int turnoAtual, int posX, int posY, const char *nomeIngrediente) {
    if (heap == nullptr)
        return;

    const int espacamento = 120;
    int y = posY-espacamento;


    // Desenha cada ingrediente no heap ao lado do anterior
    for (int i = 0; i < heap->tam; i++) {
        y += espacamento;
        if (turnoAtual > heap->dados[i]+6) {//ingrediente queimou
             PrintarIngrediente( nomeIngrediente, { (float)posX, (float)y }, 0.8f, BLACK);
             continue;
        }
        if(heap->dados[i]+3 >turnoAtual){ //ingrediente não está pronto
            if (nomeIngrediente=="n"){
                if(heap->dados[i]+3-turnoAtual==1)
                    PrintarIngrediente( "N", { (float)posX, (float)y }, 0.8f);
                else PrintarIngrediente( "9", { (float)posX, (float)y }, 0.8f);
             }
             else if(nomeIngrediente=="b"){
                PrintarIngrediente( "B", { (float)posX, (float)y }, 0.8f);
             }
             else PrintarIngrediente( nomeIngrediente, { (float)posX, (float)y }, 0.8f, Fade(WHITE, 0.5f));
             continue;
        }
        PrintarIngrediente( nomeIngrediente, { (float)posX, (float)y }, 0.8f);//nesse caso está tudo certo, ingrediente pronto
        
    }
}
// Desenha o HUD (painel de informações) com os dados atuais da fase:
// fase, turno, pedidos restantes, paciência do cliente e estoque de ingredientes.
void desenharHud(const Fase *fase) {
    DrawRectangle(0, GetScreenHeight() - 480, (float)GetScreenWidth(), 480.0f, GRAY);
    DrawRectangle(50, GetScreenHeight() - 440 , 520.0f, 380.0f, DARKGRAY);
    DrawRectangle(GetScreenWidth()/2-250, GetScreenHeight()-300, 500.0f, 260.0f, BROWN);
    Rectangle rec = { (float)(GetScreenWidth()/2-230), (float)(GetScreenHeight()-270), 460.0f, 200.0f };
    DrawRectangleLinesEx(rec, 5, DARKBROWN);
    DrawRectangle(GetScreenWidth()/2+270, GetScreenHeight()-370, 300.0f, 330.0f, BLACK);
    Rectangle rec2 = { (float)(GetScreenWidth()-300), (float)(GetScreenHeight()-300), 280.0f, 280.0f };
    DrawRectangleRounded(rec2, 0.4f, 10, BEIGE);
    DrawRectangle(0, 50, GetScreenWidth(), 10, YELLOW);
    const int larguraPlaca = 320;
    const int alturaPlaca = 90;
    const int placaX = GetScreenWidth() - 330 - 100 - larguraPlaca;
    const int placaY = 50 + 10 + 100;
    DrawRectangle(placaX, placaY, larguraPlaca, alturaPlaca, BROWN);
    DrawText("enviar: enter",
             placaX + (larguraPlaca - MeasureText("enviar: enter", 24)) / 2,
             placaY + 26, 24, DARKBROWN);
    DrawText("lixeira: backspace",
             placaX + (larguraPlaca - MeasureText("lixeira: backspace", 24)) / 2,
             placaY + 48, 24, DARKBROWN);
    Rectangle rec3 = {(float)(GetScreenWidth() - 330), 0, 330.0f, 650.0f};
    DrawRectangleRounded(rec3, 0.5f, 10, WHITE);
    DrawCircle(GetScreenWidth() - 165, 50, 30, GRAY);
    if (fase->PreparadoSuiço > 0) {
        PrintarIngrediente("s", { (float)((GetScreenWidth()-480)), (float)(GetScreenHeight()-290) }, 1.5f);
    }
    if (fase->PreparadoCheddar > 0) {
        PrintarIngrediente("q", { (float)((GetScreenWidth()-480)), (float)(GetScreenHeight()-130) }, 1.5f);
    }
    if (fase->PreparadoCebola > 0) {
        PrintarIngrediente("c", { (float)(GetScreenWidth()/2-100), (float)(GetScreenHeight()-170) }, 1.5f);
    }
    if (fase->PreparadoTomate > 0) {
        PrintarIngrediente("t", { (float)(GetScreenWidth()/2+100), (float)(GetScreenHeight()-170)  }, 1.5f);
    }
    PrintarIngrediente("K", { (float)(GetScreenWidth()-520), (float)(GetScreenHeight()/2) }, 2.0f);
    PrintarIngrediente("M", { (float)(GetScreenWidth() -390), (float)(GetScreenHeight() / 2) }, 2.0f);
    PrintarIngrediente("a", { (float)((GetScreenWidth()-140)), (float)(GetScreenHeight()-210) }, 1.2f);
    PrintarIngrediente("p", { (float)((GetScreenWidth()-140)), (float)(GetScreenHeight()-100) }, 1.2f);
   

    // Informações da partida 
    DrawText(TextFormat("Fase: %d", fase->cod), 10, 90, 40, WHITE);
    DrawText(TextFormat("Turno: %d", fase->TurnoAtual), 10, 160, 40, WHITE);

    // Silhueta de pessoa no lugar de "Pedidos:".
    DrawCircle(34, 235, 15, WHITE);
    DrawRectangle(18, 252, 32, 18, WHITE);
    DrawTriangle((Vector2){18, 270}, (Vector2){50, 270}, (Vector2){34, 252}, WHITE);
    DrawText(TextFormat("%d", fase->NumDePedidos), 62, 230, 40, WHITE);

    // Barra de paciência: verde representa paciência cheia e vermelho representa paciência baixa.
    const int barraX = 10;
    const int barraY = 310;
    const int barraLargura = 300;
    const int barraAltura = 40;
    float razao = 0.0f;
    if (fase->PacienciaDosClientes > 0) {
        razao = (float)fase->PacienciaDoClienteAtual / (float)fase->PacienciaDosClientes;
    }
    if (razao < 0.0f)
        razao = 0.0f;
    if (razao > 1.0f)
        razao = 1.0f;

    DrawRectangleGradientH(barraX, barraY, barraLargura, barraAltura, GREEN, RED);
    DrawRectangleLines(barraX, barraY, barraLargura, barraAltura, DARKGRAY);

    const int marcadorPosX = barraX + (int)((1.0f - razao) * (barraLargura - 12));
    DrawRectangle(marcadorPosX, barraY - 8, 12, barraAltura + 16, WHITE);
    DrawRectangleLines(marcadorPosX, barraY - 8, 12, barraAltura + 16, BLACK);


    const int posHamburgerX = (int)(GetScreenWidth() * 0.5f);
    const int posHamburgerY = (int)(GetScreenHeight() * 0.6f);
    const int posPedidoX = (int)(GetScreenWidth() - 165.0f);
    const int posPedidoY = (int)(610.0f);
    DesenharHamburger(&(fase->pedidoAtual), posHamburgerX, posHamburgerY);
    DesenharPedido(&(fase->pedidos), posPedidoX, posPedidoY);
    DesenharHeap(&(fase->Bacon), fase->TurnoAtual, 150,  GetScreenHeight() - 380, "b");
    DesenharHeap(&(fase->Carne), fase->TurnoAtual, 320,  GetScreenHeight() - 380, "n");
    DesenharHeap(&(fase->Ovo), fase->TurnoAtual, 490,  GetScreenHeight() - 380, "o");
}