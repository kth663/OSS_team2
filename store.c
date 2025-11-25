#include <stdio.h>

extern int score;
extern int clues[5];

int store() {
    int choice;
    
    while(1) {
        // 1. 화면을 깨끗하게 지웁니다.
        system("cls"); 

        // 2. [NEW] 고양이 아스키 아트 및 환영 메시지 출력
        printf("\n");
        printf("      /\\_/\\ \n");
        printf("     ( o_o )  < \"어서오라냥!\"\n");
        printf("      > ^ <   < \"안경 쓴 점장 고양이다냥. 천천히 둘러보라냥~\"\n");
        printf("\n");
        printf(" ========================================\n");
        printf("       [ 🐾 냐옹이의 비밀 상점 🐾 ]      \n");
        printf(" ========================================\n");
        
        // 3. 현재 상태 표시 (이모지 추가)
        printf("  💰 내 지갑 : %d 점\n", score);
        printf("  🏷️ 단서 가격 : 2000 점\n");
        printf(" ----------------------------------------\n");

        // 4. 판매 목록 출력
        for (int i = 0; i < 5; i++) {
            // 조금 더 깔끔한 정렬을 위해 공백 조정
            printf("   %d. 📜 비밀번호 %d번째 단서 ", i + 1, i + 1);
            if (clues[i] == 1) {
                printf("[ ✅ 보유중 ]\n");
            } else {
                printf("[ ⬜ 구매가능 ]\n");
            }
        }

        printf(" ----------------------------------------\n");
        printf("   0. 🚪 상점 나가기\n");
        printf(" ========================================\n");
        printf("  구매할 번호를 입력하라냥 (엔터) >> ");

        // 5. 사용자 입력 및 처리 (기존 로직과 동일)
        scanf("%d", &choice);

        // 입력 버퍼 비우기 (엔터키 찌꺼기 제거용 - quiz 등 다른 입력과 충돌 방지)
        while(getchar() != '\n');

        if (choice == 0) {
            printf("\n  \"다음에 또 오라냥~\"\n");
            // 잠시 메시지를 보여주기 위한 대기 (선택사항)
            // system("pause"); 
            break; 
        }

        if (choice >= 1 && choice <= 5) {
            int index = choice - 1;

            if (clues[index] == 1) {
                printf("\n  >>> ⚠️ \"그건 이미 가지고 있다냥!\"\n");
            }
            else if (score < 2000) {
                printf("\n  >>> 💸 \"돈이 부족하다냥! 열심히 모아오라냥.\"\n");
            }
            else {
                score = score - 2000;
                clues[index] = 1;              
                printf("\n  >>> 🎉 \"구매 고맙다냥! 여기 단서 받으라냥.\"\n");
            }
        } 
        else {
            printf("\n  >>> ❓ \"그런 번호는 없다냥. 다시 말해달라냥.\"\n");
        }

        // 결과 메시지를 사용자가 읽을 시간을 줍니다.
        printf("\n  (계속하려면 엔터를 누르라냥...)");
        getchar(); // 엔터키 입력 대기
    }

    return 0; // 정상 종료
}