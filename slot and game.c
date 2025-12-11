#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>   // _getch()
#include <stdlib.h>  // system("cls"), rand, srand
#include <windows.h> // Sleep
#include <time.h>    // time
#include "data.h"
#include "maze.h"
#include "slot and game.h"
#include "data.h"
#include "luckcharm.h"


void openInventory(void) {
    system("cls");
    printf("===== 인벤토리 =====\n");
    printf("점수: %d\n", score);
    printf("행운의 부적: %d 개\n", getLuckyCharmCount);
    printf("퀴즈북: %d 개\n",item[1]);
    printf("뱀게임 부적: %d개\n",item[2]);
    printf("초시계: %d개\n",item[3]);
    printf("\n===== 비밀번호 단서 =====\n");

    // password = 실제 정답 (예: abcde)
    // getPassword[i] = 1이면 단서 구매한 것
    for (int i = 0; i < 5; i++) {
        if (getPassword[i] == 1) {
            printf("%d번 글자: %c\n", i + 1, password[i]);
        } else {
            printf("%d번 글자: ?\n", i + 1);
        }
    }

    printf("\n아무 키나 누르면 돌아갑니다.\n");
    _getch();
    maze(0);
}

