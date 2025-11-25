#include <stdio.h>
#include <conio.h>
#include "quiz.c"
#include "store.c"

int score = 10000;
int clues[5] = {0, 0, 0, 0, 0};

int main(void){
    system("chcp 65001");
    while (1) {
        system("cls");

        printf("\n========== [ 메인 테스트 메뉴 ] ==========\n");
        printf("  💰 현재 점수 : %d 점\n", score);
        printf(" ========================================\n");
        printf("  1. 🧠 넌센스 퀴즈 하러 가기\n");
        printf("  2. 🐾 고양이 상점 하러 가기\n");
        printf("  0. 🚪 프로그램 종료\n");
        printf(" ========================================\n");
        printf("  >> 번호를 누르세요 (엔터 X): ");

        int key = _getch();

        if (key == '1') {
            quiz(); 
        }
        else if (key == '2') {
            store(); 
        }
        else if (key == '0') {
            printf("\n\n테스트를 종료합니다.\n");
            break;
        }
    }
}