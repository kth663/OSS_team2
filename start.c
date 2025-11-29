#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include "start.h"
#include "maze.h"
#include "data.h"
#include "data.h"

void gotoxy2(int x, int y) {
    COORD pos = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void show_loading_bar() {
    int barLength = 30;

    for (int i = 0; i <= barLength; i++) {
        gotoxy2((SCREEN_WIDTH - (barLength + 10)) / 2, SCREEN_HEIGHT / 2 + 2);

        printf("[");
        for (int j = 0; j < i; j++) printf("■");
        for (int j = i; j < barLength; j++) printf("□");
        printf("] %d%%", (i * 100) / barLength);

        Sleep(100);
    }
}

void blink_press_enter() {
char *msg = "Press ENTER to Start";
  int msgX = (SCREEN_WIDTH - strlen(msg)) / 2;

  while (1) {
      gotoxy2(msgX, SCREEN_HEIGHT / 2 + 5);
      printf("%s", msg);
      Sleep(500);

      gotoxy2(msgX, SCREEN_HEIGHT / 2 + 5);
      printf("%*s", (int)strlen(msg), "");  
      Sleep(300);
        if (kbhit()) {              
            int key = getch();

            
            if (key == 27) {
                return;
            }

            
            if (key == 13) {

                system("cls");
                printf("Loading");
                fflush(stdout);

                for (int i = 0; i < 5; i++) {
                    Sleep(300);
                    printf(".");
                    fflush(stdout);
                }

                Sleep(500);
                system("cls");
                break;
            }
        }
    }
}

void start_screen() {
    system("cls");
    system("color 0E");

    char *title = "★ The Loop★";
    int titleX = (SCREEN_WIDTH - strlen(title)) / 2;
    int titleY = SCREEN_HEIGHT / 2 - 3;

    gotoxy2(titleX, titleY);
    printf("%s", title);

    show_loading_bar();
    blink_press_enter();

    system("color 07");

    x = 1;
    y = 1; 
    maze();
}