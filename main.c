#include<stdio.h>
#include "start.h"
#include "data.h"
#include "password_game.h"

int score = 0;
int x;
int y;
int getPassword[5] = {0,0,0,0,0};
int item[5] = {0,0,0,0,0}; // 0: 뱀게임, 1: 타이밍, 2: 퀴즈, 3: 슬룻머신, 4: 미로
char password[100];

int main(void){
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);  

    executeGame();

    start_screen();

    return 0;
}