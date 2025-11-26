#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h> 

#define CLUE_PRICE 2000 

extern int score;
extern int clues[5];

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

void setCatFace(const char* face) {
    gotoxy_store(13, 2); 
    printf("           "); 
    gotoxy_store(13, 2); 
    printf("%s", face);
    gotoxy_store(0, 26); 
}

void say(const char* text) {
    gotoxy_store(6, 24); 
    printf("                                                           "); 
    gotoxy_store(6, 24); 
    typeWriter_store(text, 20); 
}

void openStore() {
    int choice;
    const int LIST_Y = 16; 

    system("cls");

    gotoxy_store(0, 0); printf("             /\\____/\\    ");
    gotoxy_store(0, 1); printf("            (  o  o  )     < [ 점장 야옹이 ]");
    gotoxy_store(0, 2); printf("            (    w   )     ");
    gotoxy_store(0, 3); printf("           /|   __   |\\    ");
    gotoxy_store(0, 4); printf("          ( |  $  $  | )   ");
    gotoxy_store(0, 5); printf("           \\|__|__|__/     ");
    
    gotoxy_store(0, 8); printf("   ================================================================");
    gotoxy_store(0, 9); printf("          🪙   냥   냥   이      비   밀      상   점   🪙        ");
    gotoxy_store(0, 10);printf("   ================================================================");

    for (int i = 0; i < 5; i++) {
        gotoxy_store(7, LIST_Y + i); 
        
        printf("[%d] 비밀번호 %d번째 단서", i + 1, i + 1);
        
        gotoxy_store(35, LIST_Y + i);
        if (clues[i] == 1) printf("[ ✔️ 보유중 ]");
        else printf("[ 🏷️ 2000G ]");
    }
    
    gotoxy_store(7, LIST_Y + 6); 
    printf("[0] 나가기");
    
    gotoxy_store(0, 22); printf("   ----------------------------------------------------------------");
    gotoxy_store(0, 23); printf("    💬 "); 
    gotoxy_store(0, 24); 
    gotoxy_store(0, 25); printf("   ----------------------------------------------------------------");

    setCatFace("(  -  -  )"); 
    say("어서오시게. 무엇이 필요한가냥?");
    Sleep(300);
    setCatFace("(  o  o  )"); 

    while (1) {
        gotoxy_store(40, 2); 
        printf("💰 내 지갑: %5d G", score);

        gotoxy_store(0, 27); 
        printf("    👉 번호 선택 (0: 종료) : ");
        
        int key = _getch();
        choice = key - '0'; 

        printf("%d", choice); 
        Sleep(200);
        gotoxy_store(25, 27); printf("   "); 

        if (choice == 0) {
            setCatFace("(  ^  -  )"); 
            say("다음에 또 오시게나. 조심해서 가라냥.");
            Sleep(1000); 
            break; 
        }
        else if (choice >= 1 && choice <= 5) {
            int idx = choice - 1;

            if (clues[idx] == 1) {
                setCatFace("(  ?  ?  )"); 
                say("그건 이미 자네 주머니에 있지 않은가?");
            }
            else if (score < CLUE_PRICE) {
                setCatFace("(  -  _  -  )"); 
                say("외상은 안 된다네. 돈을 더 모아오시게.");
            }
            else {
                score -= CLUE_PRICE;
                clues[idx] = 1;

                setCatFace("(  >  ▽  <  )"); 
                say("탁월한 선택이네! 여기 물건 받으시게.");
                
                gotoxy_store(35, LIST_Y + idx); 
                printf("[ ✔️ 보유중 ]  "); 
            }
        } 
        else {
            setCatFace("(  o  .  o  )");
            say("응? 그런 물건은 안 판다네.");
        }
    }
}