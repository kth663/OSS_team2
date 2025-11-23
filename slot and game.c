#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>   // _getch()
#include <stdlib.h>  // system("cls"), rand, srand
#include <windows.h> // Sleep
#include <time.h>    // time

#define ROWS 3
#define COLS 20   // 아래 미로 문자열 길이와 맞춰둠

char maze[ROWS][COLS + 1] = {
    "####################",
    "#S              $#",
    "####################"
};

int player_r = 1;
int player_c = 2;

int money = 200;
int luckyCharm = 0;

void printMaze(void);
int  canMove(int nr, int nc);
void openInventory(void);
void openShop(void);
void playSlotMachine(void);
void spinAnimation(int* s1, int* s2, int* s3, int useLucky);

// main.c에서 호출할 함수
void runGameLoop(void);

// ================== 게임 루프 함수 ==================
void runGameLoop(void) {
    while (1) {
        printMaze();
        printf("\nWASD: 이동, E: 인벤토리, Q: 종료\n");

        int ch = _getch();
        int nr = player_r;
        int nc = player_c;

        if (ch == 'w' || ch == 'W') nr--;
        else if (ch == 's' || ch == 'S') nr++;
        else if (ch == 'a' || ch == 'A') nc--;
        else if (ch == 'd' || ch == 'D') nc++;
        else if (ch == 'e' || ch == 'E') {
            openInventory();
            continue;
        }
        else if (ch == 'q' || ch == 'Q') {
            system("cls");
            printf("게임을 종료합니다.\n");
            break;
        }
        else {
            continue;
        }

        if (canMove(nr, nc)) {
            player_r = nr;
            player_c = nc;
        }

        char tile = maze[player_r][player_c];

        if (tile == 'S') {
            system("cls");
            printMaze();
            printf("\n[슬롯머신 존에 도착했습니다]\n");
            printf("슬롯머신을 플레이하려면 아무 키나 누르세요 (취소: ESC)\n");
            int key = _getch();
            if (key != 27) {
                playSlotMachine();
            }
        }

        if (tile == '$') {
            system("cls");
            printMaze();
            printf("\n[상점 존에 도착했습니다]\n");
            printf("상점을 이용하려면 아무 키나 누르세요 (취소: ESC)\n");
            int key = _getch();
            if (key != 27) {
                openShop();
            }
        }
    }
}

// ================== 나머지 함수들 ==================

void printMaze(void) {
    system("cls");
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            if (r == player_r && c == player_c)
                putchar('*');
            else
                putchar(maze[r][c]);
        }
        putchar('\n');
    }
}

int canMove(int nr, int nc) {
    if (nr < 0 || nr >= ROWS || nc < 0 || nc >= COLS) return 0;
    if (maze[nr][nc] == '#') return 0;
    return 1;
}

void openInventory(void) {
    system("cls");
    printf("===== 인벤토리 =====\n");
    printf("코인: %d\n", money);
    printf("행운의 부적: %d 개\n", luckyCharm);
    printf("\n아무 키나 누르면 돌아갑니다.\n");
    _getch();
}

void openShop(void) {
    while (1) {
        system("cls");
        printf("===== 상점 =====\n");
        printf("1) 행운의 부적 구매 (100 코인)\n");
        printf("0) 나가기\n");
        printf("------------------------\n");
        printf("현재 코인: %d\n", money);
        printf("행운의 부적: %d 개\n", luckyCharm);
        printf("\n번호를 입력하세요: ");

        int choice;
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }

        if (choice == 0) break;
        else if (choice == 1) {
            if (money >= 100) {
                money -= 100;
                luckyCharm++;
                printf("\n행운의 부적을 구매했습니다!\n");
            }
            else {
                printf("\n코인이 부족합니다!\n");
            }
        }
        else {
            printf("\n잘못된 선택입니다.\n");
        }

        printf("\n계속하려면 아무 키나 누르세요...\n");
        _getch();
    }
    while (getchar() != '\n');
}

void playSlotMachine(void) {
    int bet;
    int s1, s2, s3;
    int useLucky = 0;

    while (1) {
        system("cls");
        printf("===== SLOT MACHINE GAME =====\n");
        printf("현재 코인: %d\n", money);
        printf("행운의 부적: %d 개\n", luckyCharm);
        printf("-----------------------------\n");
        printf("베팅 금액을 입력하세요 (0 입력 시 슬롯 종료, 미로로 복귀): ");

        if (scanf("%d", &bet) != 1) {
            while (getchar() != '\n');
            continue;
        }

        if (bet == 0) break;
        if (bet < 0) { printf("음수는 안 됩니다.\n"); Sleep(1000); continue; }
        if (bet > money) { printf("코인이 부족합니다!\n"); Sleep(1000); continue; }

        useLucky = 0;
        if (luckyCharm > 0) {
            printf("\n행운의 부적을 사용하시겠습니까? (y/n): ");
            while (getchar() != '\n');
            int ch = _getch();
            if (ch == 'y' || ch == 'Y') {
                useLucky = 1;
                luckyCharm--;
                printf("\n행운의 부적 사용! 슬롯 심볼 종류가 4개로 줄어듭니다.\n");
                Sleep(1000);
            }
            else {
                printf("\n부적을 사용하지 않습니다.\n");
                Sleep(800);
            }
        }

        money -= bet;
        system("cls");
        printf("슬롯을 돌립니다...\n");

        spinAnimation(&s1, &s2, &s3, useLucky);

        if (s1 == s2 && s2 == s3) {
            printf(">>> 잭팟!! 베팅의 5배를 획득했습니다!\n");
            money += bet * 5;
        }
        else if (s1 == s2 || s2 == s3 || s1 == s3) {
            printf(">> 두 개 일치! 베팅의 2배를 획득했습니다!\n");
            money += bet * 2;
        }
        else {
            printf("꽝! 다음 기회에...\n");
        }

        printf("현재 코인: %d\n", money);
        printf("\n아무 키나 누르면 다음 판으로...\n");
        _getch();

        if (money <= 0) {
            printf("\n코인이 모두 소진되었습니다. 슬롯머신을 종료합니다.\n");
            Sleep(1500);
            break;
        }
    }

    while (getchar() != '\n');
}

void spinAnimation(int* s1, int* s2, int* s3, int useLucky) {
    const char* symbols[] = { "CHERRY", "LEMON", "BAR", "7", "BELL" };
    int symbolCount = useLucky ? 4 : 5;
    int temp1 = 0, temp2 = 0, temp3 = 0;

    for (int i = 0; i < 12; i++) {
        temp1 = rand() % symbolCount;
        temp2 = rand() % symbolCount;
        temp3 = rand() % symbolCount;

        printf("\r|  %-6s|  %-6s|  %-6s|", symbols[temp1], symbols[temp2], symbols[temp3]);
        fflush(stdout);
        Sleep(100 + i * 25);
    }

    *s1 = temp1;
    *s2 = temp2;
    *s3 = temp3;
    printf("\n");
}
