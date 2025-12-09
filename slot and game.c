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
    printf("\n아무 키나 누르면 돌아갑니다.\n");
    _getch();
    maze(0);
}
