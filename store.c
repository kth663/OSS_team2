#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h> 
#include "data.h" 
#include "maze.h"

#define CLUE_PRICE 2000 
#define ITEM_PRICE 200 

void gotoxy_store(int x, int y) {
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void typeWriter_store(const char* str, int speed) {
    for (int i = 0; 0 != str[i]; i++) {
        printf("%c", str[i]);
        Sleep(speed); 
    }
}

void setCatFace(const char* faceType) {
    for(int i=4; i<=9; i++) {
        gotoxy_store(35, i); printf("                         ");
    }

    if (strcmp(faceType, "normal") == 0) {
        gotoxy_store(35, 6); printf("      /\\___/\\");
        gotoxy_store(35, 7); printf("     (  o o  )");
        gotoxy_store(35, 8); printf("     (   ^   )");
        gotoxy_store(35, 9); printf("      \\  ~  /");
        gotoxy_store(35, 10); printf("      /     \\");
    }

    else if (strcmp(faceType, "surprised") == 0) {
        gotoxy_store(35, 6); printf("      /\\___/\\");
        gotoxy_store(35, 7); printf("     (  O O  ) !");
        gotoxy_store(35, 8); printf("     (   0   )");
        gotoxy_store(35, 9); printf("      \\  ~  /");
        gotoxy_store(35, 10); printf("      /     \\");
    }

    else if (strcmp(faceType, "sad") == 0) {
        gotoxy_store(35, 6); printf("      /\\___/\\");
        gotoxy_store(35, 7); printf("     (  - -  ) ...");
        gotoxy_store(35, 8); printf("     (   ^   )");
        gotoxy_store(35, 9); printf("      \\  ~  /");
        gotoxy_store(35, 10); printf("      /     \\");
    }
}

void say(const char* line1, const char* line2, const char* line3) {
    gotoxy_store(52, 14); printf("                         ");
    gotoxy_store(52, 15); printf("                         ");
    gotoxy_store(52, 16); printf("                         ");
    gotoxy_store(52, 17); printf("                         ");

    gotoxy_store(52, 14); typeWriter_store(line1, 10);
    gotoxy_store(52, 15); typeWriter_store(line2, 10);
    gotoxy_store(52, 16); typeWriter_store(line3, 10);
}

int store() {
    int choice;
    int* itemArray = getPassword; 
    int* gameItems = item;        

    system("cls");

    printf("\n");
    printf("  ==========================================================================\n");
    printf("  ||                              CBNU STORE                              ||\n");
    printf("  ==========================================================================\n");
    printf("  ||                                                                      ||\n");
    printf("  ||                                                                      ||\n");
    printf("  ||                                                                      ||\n");
    printf("  ||                                                                      ||\n");
    printf("  ||                                                                      ||\n");
    printf("  ||                                                                      ||\n");
    printf("  ||                                                                      ||\n");
    printf("  ==========================================================================\n");

    setCatFace("normal");

    gotoxy_store(2, 12); printf("┌──────────────────────────────────────────────┐┌───────────────────────────┐");
    gotoxy_store(2, 13); printf("│                                              ││                           │");
    gotoxy_store(2, 14); printf("│                                              ││                           │");
    gotoxy_store(2, 15); printf("│                                              ││                           │");
    gotoxy_store(2, 16); printf("│                                              ││                           │");
    gotoxy_store(2, 17); printf("│                                              ││                           │");
    gotoxy_store(2, 18); printf("│                                              ││                           │");
    gotoxy_store(2, 19); printf("│                                              ││          %-6d G         │", score);
    gotoxy_store(2, 20); printf("└──────────────────────────────────────────────┘└───────────────────────────┘");

    gotoxy_store(4, 13); printf("[ 단서 : 2000 G ]");
    for(int i=0; i<5; i++) {
        gotoxy_store(4, 14+i);
        if(itemArray[i] == 1) printf("%d. 비밀번호 %d단서 [품절]", i+1, i+1);
        else printf("%d. 비밀번호 %d단서", i+1, i+1);
    }

    gotoxy_store(26, 13); printf(" [ 아이템 : 200 G ] ");
    char* itemNames[5] = {"뱀게임 부적", "타이밍 시계", "퀴즈 힌트북", "슬롯 행운권", "미로 나침반"};
    for(int i=0; i<5; i++) {
        gotoxy_store(28, 14+i);
        if(gameItems[i] == 1) printf("%d. %s [품절]", i+6, itemNames[i]);
        else printf("%d. %s", i+6, itemNames[i]);
    }
    
    gotoxy_store(4, 19); printf("0. 나가기");

    say("손님, 제가", "어떻게 도와드릴", "까요?");

    while (1) {
        gotoxy_store(59, 19); printf("%-6d", score);

        gotoxy_store(15, 19); 
        printf("선택 >> ");
        
        gotoxy_store(23, 19);
        if (scanf("%d", &choice) == 0) { 
            while(getchar() != '\n'); 
            choice = -1;
        }
        gotoxy_store(23, 19); printf("   ");

        if (choice == 0) {
            setCatFace("normal");
            say("안녕히 가세요.", "또 오시든가요...", "");
            Sleep(1000); 
            break; 
        }
        else if (choice >= 1 && choice <= 5) {
            int idx = choice - 1;
            if (itemArray[idx] == 1) {
                setCatFace("normal");
                say("그건 이미", "가져가셨잖아요.", "묻지 말아 주세요.");
            }
            else if (score < CLUE_PRICE) {
                setCatFace("sad");
                say("돈이 없으시네요.", "저도 곤란합니다.", "정말요.");
            }
            else {
                score -= CLUE_PRICE;
                itemArray[idx] = 1;
                setCatFace("surprised");
                say("팔렸다!", "감사합니다 손님!", "좋은 하루 되세요.");
                
                gotoxy_store(4, 14+idx); printf("%d. 비밀번호 %d단서 [품절]", idx+1, idx+1);
            }
        } 
        else if (choice >= 6 && choice <= 10) {
            int idx = choice - 6; 
            if (gameItems[idx] == 1) {
                setCatFace("normal");
                say("욕심이 많으시네요.", "이미 샀잖아요.", "");
            }
            else if (score < ITEM_PRICE) {
                setCatFace("sad");
                say("200원도 없으신가요?", "농담이시죠?", "");
            }
            else {
                score -= ITEM_PRICE;
                gameItems[idx] = 1;
                setCatFace("surprised");
                say("오! 구매 감사합니다.", "유용할 거예요.", "(아마도)");
                
                gotoxy_store(28, 14+idx); printf("%d. %s [품절]", choice, itemNames[idx]);
            }
        }
        else {
            setCatFace("normal");
            say("메뉴판에 없는 건", "못 팝니다.", "제 권한 밖이에요.");
        }
    }
    
    maze(0); 
    return 0;
}