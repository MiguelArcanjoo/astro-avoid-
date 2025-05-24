#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

// estruturas, structs para requisitos e facilitar o código
typedef struct{
    int x;
    int y;
    int direcao;
    char sprite[4]; // para armazenar emojis
} Asteroide;

typedef struct{
    Asteroide* dados; // lista dinâmica
    int quantidade;
    int capacidade;
} ListaAsteroides;

typedef struct{
    int x, y;
    int vidas;
    int nivel;
    int pontuacao;
} Jogador;

Jogador jogador;
ListaAsteroides asteroides;
const char* sprites[5] = {"🪨", "☄️", "🛰️", "🛸", "🪐"};

// funções para lista de asteroides
void inicializarLista(ListaAsteroides* lista, int capacidade){
    lista->dados = (Asteroide*)malloc(capacidade * sizeof(Asteroide));
    lista->quantidade = 0;
    lista->capacidade = capacidade;
}

void adicionarAsteroide(ListaAsteroides* lista, Asteroide ast){
    if (lista->quantidade < lista->capacidade) {
        lista->dados[lista->quantidade++] = ast;
    }
}

void liberarLista(ListaAsteroides* lista){
    free(lista->dados);
    lista->quantidade = lista->capacidade = 0;
}

// Função para salvar pontuação
void salvarPontuacao(int pontuacao){
    FILE *arquivo = fopen("pontuacoes.txt", "a");  // 'a' para append (adicionar ao final)
    if (arquivo) {
        fprintf(arquivo, "%d\n", pontuacao);  // salva como texto
        fclose(arquivo);
    }
}

// função para carregar a melhor pontuação
int carregarMelhorPontuacao(){
    FILE *arquivo = fopen("pontuacoes.txt", "r");
    int melhor = 0, atual;
    if (arquivo) {
        while (fscanf(arquivo, "%d", &atual) == 1){  // lê cada linha como número
            if (atual > melhor) melhor = atual;
        }
        fclose(arquivo);
    }
    return melhor;
}

void resetarPosicaoJogador(){
    jogador.x = MAXX / 2;
    jogador.y = MAXY - 2;
}

void inicializarJogo(){
    // jogador ja começa com 3 vidas
    resetarPosicaoJogador();
    jogador.vidas = 3;
    jogador.nivel = 1;
    jogador.pontuacao = 0;

    // inicializa os asteroides
    inicializarLista(&asteroides, 20);

    // add asteroides iniciais
    for (int i = 0; i < 3 + jogador.nivel; i++){
        Asteroide ast = {
            .x = rand() % (MAXX - 3) + 1,
            .y = rand() % (MAXY / 2) + 1,
            .direcao = (rand() % 2) ? 1 : -1
        };
        strcpy(ast.sprite, sprites[rand() % 5]);
        adicionarAsteroide(&asteroides, ast);
    }
}

void desenharJogo(){
    screenClear();
    desenharEspaco();
    
    // desenha asteroides
    Asteroide* ast = asteroides.dados;
    for (int i = 0; i < asteroides.quantidade; i++, ast++){
        screenGotoxy(ast->x, ast->y);
        printf("%s", ast->sprite);
    }
    
    // desenha jogador
    screenGotoxy(jogador.x, jogador.y);
    screenSetColor(GREEN, BLACK);
    printf("▲");
    
    desenharHUD(jogador.nivel, jogador.vidas);
    screenUpdate();
}

void atualizarAsteroides(){
    Asteroide* ast = asteroides.dados;
    for (int i = 0; i < asteroides.quantidade; i++, ast++){
        ast->x += ast->direcao;
        if (ast->x <= 1) ast->x = MAXX - 3;
        if (ast->x >= MAXX - 2) ast->x = 1;
    }
}

int verificarColisao(){
    Asteroide* ast = asteroides.dados;
    for (int i = 0; i < asteroides.quantidade; i++, ast++){
        if (jogador.y == ast->y && jogador.x >= ast->x && jogador.x <= ast->x + 2){
            return 1;
        }
    }
    return 0;
}

void proximoNivel(){
    jogador.nivel++;
    jogador.pontuacao += jogador.nivel * 100;
    liberarLista(&asteroides);
    inicializarLista(&asteroides, 20);
    
    // add mais asteroides
    for (int i = 0; i < 3 + jogador.nivel; i++){
        Asteroide ast = {
            .x = rand() % (MAXX - 3) + 1,
            .y = rand() % (MAXY / 2) + 1,
            .direcao = (rand() % 2) ? 1 : -1
        };
        strcpy(ast.sprite, sprites[rand() % 5]);
        adicionarAsteroide(&asteroides, ast);
    }
    
    resetarPosicaoJogador();
    
    // mostra  a mensagem
    screenClear();
    screenGotoxy(MAXX/2 - 10, MAXY/2);
    printf("NOVO NÍVEL %d!", jogador.nivel);
    screenUpdate();
    timerDelay(2);
}

void fimDeJogo() {
    salvarPontuacao(jogador.pontuacao);
    screenClear();
    screenGotoxy(MAXX/2 - 10, MAXY/2);
    printf("FIM DE JOGO! Pontuação: %d", jogador.pontuacao);
    screenGotoxy(MAXX/2 - 10, MAXY/2 + 1);
    printf("Melhor pontuação: %d", carregarMelhorPontuacao());
    screenUpdate();
    timerDelay(3);
}

int main(){
    srand(time(NULL));
    screenInit(1);
    keyboardInit();
    timerInit(100);
    inicializarJogo();
    desenharJogo();
    
    int tecla;
    while (1){
        if (keyhit()){
            tecla = readch();
            if (tecla == 27) break; //ESC para sair
            
            // movimentação do jogador
            if (tecla == 'w') jogador.y--;
            if (tecla == 's') jogador.y++;
            if (tecla == 'a') jogador.x--;
            if (tecla == 'd') jogador.x++;
            
            // limites da tela
            if (jogador.x < 1) jogador.x = 1;
            if (jogador.x > MAXX - 2) jogador.x = MAXX - 2;
            if (jogador.y < 1) jogador.y = 1;
            if (jogador.y > MAXY - 2) jogador.y = MAXY - 2;
            
            // verifica se chegou no topo
            if (jogador.y == 1){
                proximoNivel();
            }
        }
        
        if (timerTimeOver()){
            atualizarAsteroides();
            
            if (verificarColisao()){
                jogador.vidas--;
                if (jogador.vidas <= 0){
                    fimDeJogo();
                    break;
                }
                resetarPosicaoJogador();
            }
            
            desenharJogo();
        }
    }
    
    liberarLista(&asteroides);
    keyboardDestroy();
    screenDestroy();
    timerDestroy();
    return 0;
}