#include<stdio.h>
#include "start.h"
#include "data.h"
#include "password_game.h"
#include "time.h"

int score = 0;
int x;
int y;
int getPassword[5] = {0,0,0,0,0};
int item[5] = {0,0,0,0,0}; // 0: 뱀게임, 1: 타이밍, 2: 퀴즈, 3: 슬룻머신, 4: 미로
char password[100];
int startTime = 0;
int endTime = 0;

int main(void){
    
    startGame();

    return 0;
}

void startGame(){
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);  

    //전역변수 초기화
    score = 0;
    x = 0;
    y = 0;
    for(int i = 0;i<5;i++){
        getPassword[i] = 0;
        item[i] = 0;
    }
    startTime = time(0);

    executeGame();

    start_screen();
}