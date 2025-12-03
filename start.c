#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include "start.h"
#include "maze.h"
#include "data.h"

void gotoxy2(int x, int y) {
    COORD pos = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// --------------------------
//       THE LOOP 로고
//   (위에서 천천히 내려오기)
// --------------------------
void print_logo() {
    const char* logo[] = {
        "  █████████  ███   ███  ███████          ███        █████      █████    ██████",
        "     ███     ███   ███  ███              ███      ███   ███  ███   ███  ███  ███",
        "     ███     █████████  ██████           ███      ███   ███  ███   ███  ██████",
        "     ███     ███   ███  ███              ███      ███   ███  ███   ███  ███",
        "     ███     ███   ███  ███████          ████████   █████      █████    ███"
    };

    int lines = 5;
    int finalY = 3;   // 로고가 멈출 최종 Y 위치
    int startY = -5;  // 화면 위 밖에서부터 떨어지는 느낌

    for (int y = startY; y <= finalY; y++) {
        system("cls");  // 이전 프레임 지우기

        // 현재 y 위치에 로고 출력
        for (int i = 0; i < lines; i++) {
            int drawY = y + i;
            if (drawY >= 0) {  // 화면 위를 벗어나는 줄은 출력 안 함
                gotoxy2(2, drawY); // 왼쪽에서 약간 들여쓰기
                printf("%s", logo[i]);
            }
        }

        Sleep(80);  // 떨어지는 속도 (원하면 조절 가능)
    }
}
// ---------------------------------


void show_loading_bar() {
    int barLength = 30;

    for (int i = 0; i <= barLength; i++) {
        gotoxy2((SCREEN_WIDTH - (barLength + 10)) / 2, SCREEN_HEIGHT / 2 + 7);

        printf("[");
        for (int j = 0; j < i; j++) printf("■");
        for (int j = i; j < barLength; j++) printf("□");
        printf("] %d%%", (i * 100) / barLength);

        Sleep(50);
    }
}

void blink_press_enter() {
    char *msg = "Press ENTER to Start";
    int msgX = (SCREEN_WIDTH - strlen(msg)) / 2;

    while (1) {
        gotoxy2(msgX, SCREEN_HEIGHT / 2 + 10);
        printf("%s", msg);
        Sleep(500);

        gotoxy2(msgX, SCREEN_HEIGHT / 2 + 10);
        printf("%*s", (int)strlen(msg), "");
        Sleep(300);

        if (kbhit()) {
            int key = getch();
            if (key == 27) return; 
            if (key == 13) {
                system("cls");
                return;
            }
        }
    }
}

void start_screen() {
    system("cls");
    system("color 0E");   // 노란색 유지

    print_logo();         // ← 이제 떨어지는 애니메이션
    show_loading_bar();   // 로고 아래에 그대로 이어서 나옴
    blink_press_enter();

    system("color 07");   // 원래 색으로 복귀

    x = 1;
    y = 1;
    maze();
}