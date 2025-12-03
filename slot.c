#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "maze.h"
#include "luckcharm.h"

void spinAnimation(int* s1, int* s2, int* s3, int useCharm);

// 슬롯머신 게임 루프
void runSlotMachine(void) {
    int money = 100;
    int bet;
    int s1, s2, s3;

    srand((unsigned int)time(NULL));

    printf("============================================\n");
    printf("||           SLOT MACHINE GAME            ||\n");
    printf("============================================\n");
    printf("시작 금액: %d 코인\n\n", money);
    printf("보유 행운의 부적: %d개\n\n", getLuckyCharmCount());

    while (money > 0) {
        printf("베팅 금액을 입력하세요 (0 입력 시 종료): ");
        if (scanf_s("%d", &bet) != 1) {
            while (getchar() != '\n'); // 입력 버퍼 정리
            continue;
        }
        if (bet == 0) break;
        if (bet > money) {
            printf(">> 돈이 부족합니다!\n\n");
            continue;
        }

        money -= bet;
        printf("\n슬롯을 돌립니다...\n");

        // 부적 사용 여부 결정
        int useCharm = 0;
        if (getLuckyCharmCount() > 0) {
            printf("행운의 부적을 사용하시겠습니까? (%d개 남음, 1: 사용, 0: 사용 안 함): ", getLuckyCharmCount());
            if (scanf_s("%d", &useCharm) != 1) useCharm = 0;
        }

        if (useCharm) {
            if (useLuckyCharm()) {
                printf("🎁 행운의 부적 사용! 슬롯 확률 상승!\n");
            } else {
                useCharm = 0; // 혹시 모를 안전장치
            }
        }

        // 슬롯머신 돌리기
        spinAnimation(&s1, &s2, &s3, useCharm);

        // 결과 판정
        if (s1 == s2 && s2 == s3) {
            printf(">>> 🎉 잭팟!! 베팅의 5배를 획득했습니다!\n");
            money += bet * 5;
        }
        else if (s1 == s2 || s2 == s3 || s1 == s3) {
            printf(">> ✨ 두 개 일치! 베팅의 2배를 획득했습니다!\n");
            money += bet * 2;
        }
        else {
            printf("😢 꽝! 다음 기회에...\n");
        }

        printf("현재 잔액: %d 코인\n", money);
        printf("남은 행운의 부적: %d\n\n", getLuckyCharmCount());
        Sleep(1500);
    }

    printf("\n게임 종료! 남은 코인: %d\n", money);
    printf("============================================\n");
    printf("||       THANK YOU FOR PLAYING!           ||\n");
    printf("============================================\n");

    maze(); // 슬롯머신 종료 후 미로 게임 호출
}

// 슬롯 애니메이션
void spinAnimation(int* s1, int* s2, int* s3, int useCharm) {
    const char* allSymbols[] = { "CHERRY", "LEMON", "BAR", "7", "BELL" };
    const char* symbols[5];
    int symbolCount = 5;

    // 부적 사용 시 심볼 1개 제거
    if (useCharm) {
        int removeIndex = rand() % 5;
        int j = 0;
        for (int i = 0; i < 5; i++) {
            if (i != removeIndex) {
                symbols[j++] = allSymbols[i];
            }
        }
        symbolCount = 4;
    } else {
        for (int i = 0; i < 5; i++) symbols[i] = allSymbols[i];
    }

    int temp1, temp2, temp3;
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
    printf("test"); 
}
