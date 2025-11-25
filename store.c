#include <stdio.h>

extern int score;
extern int clues[5];

int store()
{
    int choice;
    
    while(1)
    {
        printf("\n========== [ 상 점 ] ==========\n");
        printf("내 지갑: %d점\n", score);
        printf("단서 가격: %d점\n", 2000);
        printf("-------------------------------\n");

        for (int i = 0; i < 5; i++) {
            printf("%d. 비밀번호 %d번째 단서 ", i + 1, i + 1);
            if (clues[i] == 1) {
                printf("[ V 보유중 ]\n");
            } else {
                printf("[   구매가능 ]\n");
            }
        }

        printf("0. 나가기 (프로그램 종료)\n");
        printf("===============================\n");
        printf("구매할 번호를 입력하고 [엔터]를 치세요 >> ");

        scanf("%d", &choice);

        if (choice == 0) {
            printf("\n상점을 나갑니다.\n");
            break; 
        }

        if (choice >= 1 && choice <= 5) {
            int index = choice - 1;

            if (clues[index] == 1) {
                printf("\n>>> [!] 이미 가지고 있는 단서입니다.\n");
            }

            else if (score < 2000) {
                printf("\n>>> [!] 돈이 부족합니다! (부족한 돈: %d점)\n", 2000 - score);
            }

            else {
                score = score - 2000;
                clues[index] = 1;              
                printf("\n>>> [성공] %d번째 단서를 구매했습니다!\n", choice);
            }
        } 
        else {
            printf("\n>>> [!] 잘못된 번호입니다. 1~5번을 입력하세요.\n");
        }
    }
}