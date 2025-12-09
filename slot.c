#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "maze.h"
#include "luckcharm.h"
#include "data.h"

// 색상 출력 함수
void printColored(const char* text, int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
    printf("%-6s", text);
    SetConsoleTextAttribute(hConsole, 7); // 기본색으로 복원
}

// 슬롯 애니메이션
void spinAnimation(int* s1, int* s2, int* s3, int useCharm) {
    const char* allSymbols[] = { "CHERRY", "LEMON", "BAR", "7", "BELL" };
    int colors[] = { 12, 14, 8, 10, 11 }; // 빨강, 노랑, 회색, 초록, 파랑
    const char* symbols[5];
    int symbolCount = 5;

    // 부적 사용 시 심볼 하나 제거
    if (useCharm) {
        int removeIndex = rand() % 5;
        int j = 0;
        for (int i = 0; i < 5; i++) {
            if (i != removeIndex) symbols[j++] = allSymbols[i];
        }
        symbolCount = 4;
    } else {
        for (int i = 0; i < 5; i++) symbols[i] = allSymbols[i];
    }

    int temp1, temp2, temp3;

    // 슬롯 박스 고정 출력
    printf("┌────────┬────────┬────────┐\n");
    printf("│        │        │        │\n");
    printf("└────────┴────────┴────────┘\n");

    for (int i = 0; i < 15; i++) {
        temp1 = rand() % symbolCount;
        temp2 = rand() % symbolCount;
        temp3 = rand() % symbolCount;

        // 심볼만 갱신 (박스 고정)
        COORD pos = { 2, 1 }; // 좌표: x=2, y=1 첫 심볼 위치
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        SetConsoleCursorPosition(hConsole, pos);
        printColored(symbols[temp1], colors[temp1]);

        pos.X = 11; // 두 번째 슬롯
        SetConsoleCursorPosition(hConsole, pos);
        printColored(symbols[temp2], colors[temp2]);

        pos.X = 20; // 세 번째 슬롯
        SetConsoleCursorPosition(hConsole, pos);
        printColored(symbols[temp3], colors[temp3]);

        fflush(stdout);
        Sleep(80 + i * 20); // 점점 느려지면서 정지
    }

    *s1 = temp1;
    *s2 = temp2;
    *s3 = temp3;

    // 최종 심볼 출력 (박스 그대로)
    COORD pos = { 2, 1 };
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsole, pos);
    printColored(symbols[*s1], colors[*s1]);

    pos.X = 11;
    SetConsoleCursorPosition(hConsole, pos);
    printColored(symbols[*s2], colors[*s2]);

    pos.X = 20;
    SetConsoleCursorPosition(hConsole, pos);
    printColored(symbols[*s3], colors[*s3]);

    printf("\n");
}

// 슬롯머신 게임 루프
void runSlotMachine(void) {
    system("cls");
    printf("\n");

    int bet;
    int s1, s2, s3;

    srand((unsigned int)time(NULL));

    printf("╔════════════════════════════════════════╗\n");
    printf("║               SLOT MACHINE             ║\n");
    printf("╠════════════════════════════════════════╣\n");
    printf("║   시작 점수 : %-6d 코인                   ║\n", score);
    printf("║   행운의 부적 : %-3d 개                      ║\n", getLuckyCharmCount());
    printf("╚════════════════════════════════════════╝\n\n");
    
    
    if (score <= 0) {
    printf("스코어 없음! 슬롯머신을 이용할 수 없습니다.\n");
    return;
    }
    
    
    while (score > 0) {
        printf("베팅 금액을 입력하세요 (0 입력 시 종료): ");
        if (scanf_s("%d", &bet) != 1) {
            while (getchar() != '\n');
            continue;
        }
        if (bet == 0) break;
        if (bet > score) {
            printf(">> 점수가 부족합니다!\n\n");
            continue;
        }

        score -= bet;
        printf("\n슬롯을 돌리는 중....\n");
    


        int useCharm = 0;
        if (getLuckyCharmCount() > 0) {
            printf("행운의 부적을 사용하시겠습니까? (%d개 남음, 1: 사용, 0: 사용 안 함): ", getLuckyCharmCount());
            if (scanf_s("%d", &useCharm) != 1) useCharm = 0;
        }

        if (useCharm && !useLuckyCharm()) useCharm = 0;

        spinAnimation(&s1, &s2, &s3, useCharm);

        if (s1 == s2 && s2 == s3) {
            printf("\n🎉🎉🎉 JACKPOT!! 🎉🎉🎉\n");
            score += bet * 5;
        }
        else if (s1 == s2 || s2 == s3 || s1 == s3) {
            printf("\n✨ 두 개 일치 성공! ✨\n");
            score += bet * 2;
        }
        else {
            printf("\n😢 꽝! 다음 기회에...\n");
        }

        printf("현재 점수: %d 코인\n", score);
        printf("남은 행운의 부적: %d\n\n", getLuckyCharmCount());
        Sleep(1500);
    }

    printf("\n게임 종료! 남은 코인: %d\n", score);
   
    return;
}
