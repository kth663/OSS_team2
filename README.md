#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

void spinAnimation(int* s1, int* s2, int* s3);

int main(void) {
    int money = 100;
    int bet;
    int s1, s2, s3;

    srand((unsigned int)time(NULL));

    printf("============================================\n");
    printf("||           SLOT MACHINE GAME            ||\n");
    printf("============================================\n");
    printf("시작 금액: %d 코인\n\n", money);

    while (money > 0) {
        printf("베팅 금액을 입력하세요 (0 입력 시 종료): ");
        scanf_s("%d", &bet);
        if (bet == 0) break;
        if (bet > money) {
            printf(">> 돈이 부족합니다!\n\n");
            continue;
        }

        money -= bet;
        printf("\n슬롯을 돌립니다...\n");

        // spinAnimation에서 마지막 프레임을 바로 화면에 출력 + 결과 반환
        spinAnimation(&s1, &s2, &s3);

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

        printf("현재 잔액: %d 코인\n\n", money);
        Sleep(1500);
    }

    printf("\n게임 종료! 남은 코인: %d\n", money);
    printf("============================================\n");
    printf("||       THANK YOU FOR PLAYING!           ||\n");
    printf("============================================\n");
    return 0;
}

// spinAnimation에서 글자 바뀌는 효과 + 마지막 프레임 출력
void spinAnimation(int* s1, int* s2, int* s3) {
    const char* symbols[] = { "CHERRY", "LEMON", "BAR", "7", "BELL" };
    int temp1, temp2, temp3;

    for (int i = 0; i < 12; i++) {
        temp1 = rand() % 5;
        temp2 = rand() % 5;
        temp3 = rand() % 5;

        // 돌아가는 슬롯 화면 출력
        printf("\r|  %-6s|  %-6s|  %-6s|", symbols[temp1], symbols[temp2], symbols[temp3]);
        fflush(stdout);
        Sleep(100 + i * 25);
    }

    // 마지막 프레임 그대로 결과로 전달
    *s1 = temp1;
    *s2 = temp2;
    *s3 = temp3;
    printf("\n"); // 마지막 줄 내려주기
}

