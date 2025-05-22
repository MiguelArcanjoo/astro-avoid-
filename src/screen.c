
#include "screen.h"
#include <stdlib.h>  
#include <stdio.h>   
#define ALTURA_TELA 25
#define LARGURA_TELA 80





void desenharEspaco() {
    screenSetColor(BLUE, BLACK); 
    for (int y = 0; y < ALTURA_TELA; y++) {
        screenGotoxy(0, y);
        for (int x = 0; x < LARGURA_TELA; x++) {
            int tipo = rand() % 100;
            if (tipo < 2) {
                printf(".");  // estrela pequena
            } else if (tipo == 3) {
                printf("*");  // estrela brilhante
            } else {
                printf(" ");  // espaço vazio
            }
        }
    }
}

void desenharHUD(int nivel, int vidas) {
    screenSetColor(YELLOW, BLACK); 
    screenGotoxy(2, 0);
    printf("Nível: %d   Vidas: %d", nivel, vidas);
}    
    
void screenDrawBorders() 
{
    char hbc = BOX_HLINE;
    char vbc = BOX_VLINE;
    
    screenClear();
    screenBoxEnable();
    
    screenGotoxy(MINX, MINY);
    printf("%c", BOX_UPLEFT);

    for (int i=MINX+1; i<MAXX; i++)
    {
        screenGotoxy(i, MINY);
        printf("%c", hbc);
    }
    screenGotoxy(MAXX, MINY);
    printf("%c", BOX_UPRIGHT);

    for (int i=MINY+1; i<MAXY; i++)
    {
        screenGotoxy(MINX, i);
        printf("%c", vbc);
        screenGotoxy(MAXX, i);
        printf("%c", vbc);
    }

    screenGotoxy(MINX, MAXY);
    printf("%c", BOX_DWNLEFT);
    for (int i=MINX+1; i<MAXX; i++)
    {
        screenGotoxy(i, MAXY);
        printf("%c", hbc);
    }
    screenGotoxy(MAXX, MAXY);
    printf("%c", BOX_DWNRIGHT);

    screenBoxDisable();
    
}

void screenInit(int drawBorders)
{
    screenClear();
    if (drawBorders) screenDrawBorders();
    screenHomeCursor();
    screenHideCursor();
}

void screenDestroy()
{
    printf("%s[0;39;49m", ESC); // Resetar cores
    screenSetNormal();
    screenClear();
    screenHomeCursor();
    screenShowCursor();
}

void screenGotoxy(int x, int y)
{
    x = ( x<0 ? 0 : x>=MAXX ? MAXX-1 : x);
    y = ( y<0 ? 0 : y>MAXY ? MAXY : y);
    
    printf("%s[f%s[%dB%s[%dC", ESC, ESC, y, ESC, x);
}

void screenSetColor( screenColor fg, screenColor bg)
{
    char atr[] = "[0;";

    if ( fg > LIGHTGRAY )
    {
        atr[1] = '1';
		fg -= 8;
    }

    printf("%s%s%d;%dm", ESC, atr, fg+30, bg+40);
}
