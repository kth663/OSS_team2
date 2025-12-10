#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <string.h>
#include <conio.h>
#include "data.h"
#include "luckcharm.h"
#include "maze.h"

// ------------------- 전역 변수 -------------------
int score = 100;

// ------------------- 콘솔 유틸 -------------------
void moveCursor(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void clearLine(int y) {
    moveCursor(0, y);
    printf("                                        ");
}

int getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

int centerX(const char* text) {
    int width = getConsoleWidth();
    int len = (int)strlen(text);
    int x = (width - len) / 2;
    if (x < 0) x = 0;
    return x;
}

// ------------------- 슬롯머신 헤더 출력 -------------------
void printSlotHeader(int topY) {
    int boxWidth = 42;
    int x;

    x = centerX("╔════════════════════════════════════════╗");
    moveCursor(x, topY);     printf("╔════════════════════════════════════════╗");
    x = centerX("║               SLOT MACHINE             ║");
    moveCursor(x, topY + 1); printf("║               SLOT MACHINE             ║");
    x = centerX("╠════════════════════════════════════════╣");
    moveCursor(x, topY + 2); printf("╠════════════════════════════════════════╣");

    char buf[50];

    sprintf(buf, "시작 점수 : %d 코인", score);
    int padLeft = (boxWidth - 2 - (int)strlen(buf)) / 2;
    int padRight = boxWidth - 2 - (int)strlen(buf) - padLeft;
    x = centerX("║                                        ║");
    moveCursor(x, topY + 3);
    printf("║%*s%s%*s║", padLeft, "", buf, padRight, "");

    sprintf(buf, "행운의 부적 : %d 개", getLuckyCharmCount());
    padLeft = (boxWidth - 2 - (int)strlen(buf)) / 2;
    padRight = boxWidth - 2 - (int)strlen(buf) - padLeft;
    x = centerX("║                                        ║");
    moveCursor(x, topY + 4);
    printf("║%*s%s%*s║", padLeft, "", buf, padRight, "");

    x = centerX("╚════════════════════════════════════════╝");
    moveCursor(x, topY + 5);
    printf("╚════════════════════════════════════════╝");
}

// ------------------- 슬롯머신 심볼 애니메이션 -------------------
void spinAnimation(int* s1, int* s2, int* s3, int useCharm, int topY) {
    const char* allSymbols[] = { "CHERRY","LEMON","BAR","7","BELL" };
    const char* symbols[5];
    int symbolCount = 5;

    if (useCharm) {
        int removeIndex = rand() % 5;
        int j = 0;
        for (int i = 0; i < 5; i++)
            if (i != removeIndex)
                symbols[j++] = allSymbols[i];
        symbolCount = 4;
    } else {
        for (int i = 0; i < 5; i++)
            symbols[i] = allSymbols[i];
    }

    int temp1 = 0, temp2 = 0, temp3 = 0;

    int x = centerX("┌────────┬────────┬────────┐");
    moveCursor(x, topY);     printf("┌────────┬────────┬────────┐");
    moveCursor(x, topY + 1); printf("│        │        │        │");
    moveCursor(x, topY + 2); printf("└────────┴────────┴────────┘");

    for (int i = 0; i < 15; i++) {
        temp1 = rand() % symbolCount;
        temp2 = rand() % symbolCount;
        temp3 = rand() % symbolCount;

        moveCursor(x + 2, topY + 1);  printf("%-6s", symbols[temp1]);
        moveCursor(x + 11, topY + 1); printf("%-6s", symbols[temp2]);
        moveCursor(x + 20, topY + 1); printf("%-6s", symbols[temp3]);

        fflush(stdout);
        Sleep(80 + i * 20);
    }

    *s1 = temp1;
    *s2 = temp2;
    *s3 = temp3;

    moveCursor(x + 2, topY + 1);  printf("%-6s", symbols[*s1]);
    moveCursor(x + 11, topY + 1); printf("%-6s", symbols[*s2]);
    moveCursor(x + 20, topY + 1); printf("%-6s", symbols[*s3]);
}

// ------------------- 슬롯머신 실행 함수 -------------------
void runSlotMachine() {
    system("cls");
    srand((unsigned int)time(NULL));

    int bet, s1, s2, s3;
    char input[20];
    int topY = 3;

    printSlotHeader(topY);

    while (1) {
        moveCursor(0, topY + 7); printf("                                      ");
        moveCursor(0, topY + 7); printf("베팅 금액 입력 (0 입력 시 종료): ");
        if (fgets(input, sizeof(input), stdin) == NULL) continue;
        if (sscanf(input, "%d", &bet) != 1) continue;
        if (bet == 0) return;
        if (bet > score) {
            moveCursor(0, topY + 8);
            printf(">> 점수가 부족합니다!");
            continue;
        }
        break;
    }

    score -= bet;

    int useCharm = 0;
    if (getLuckyCharmCount() > 0) {
        while (1) {
            moveCursor(0, topY + 8); printf("                                      ");
            moveCursor(0, topY + 8);
            printf("행운의 부적 사용? (%d개 남음, 1:사용 0:사용 안함): ",
                   getLuckyCharmCount());
            if (fgets(input, sizeof(input), stdin) == NULL) continue;
            if (sscanf(input, "%d", &useCharm) != 1) continue;
            if (useCharm != 0 && useCharm != 1) continue;
            break;
        }
    }

    if (useCharm && !useLuckyCharm()) useCharm = 0;

    spinAnimation(&s1, &s2, &s3, useCharm, topY + 10);

    moveCursor(0, topY + 14); printf("                                      ");
    if (s1 == s2 && s2 == s3) {
        printf("JACKPOT! 점수 %d 증가", bet * 5);
        score += bet * 5;
    }
    else if (s1 == s2 || s2 == s3 || s1 == s3) {
        printf("두 개 일치! 점수 %d 증가", bet * 2);
        score += bet * 2;
    }
    else {
        printf("꽝! 점수 변화 없음");
    }

    moveCursor(0, topY + 15);
    printf("현재 점수: %d 코인 | 남은 부적: %d", score, getLuckyCharmCount());

    moveCursor(0, topY + 17);
    printf("\n아무 키나 누르면 종료합니다...");
    _getch();
    maze(1);
}
