#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <math.h>
#include "timing.h"

#define SCREEN_HEIGHT 25

int score;

int get_console_width() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int width = 80;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    return width;
}

void gotoxy(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void print_block(int y_start, const char *lines[], int n_lines) {
    int width = get_console_width();
    int max_len = 0;
    for (int i = 0; i < n_lines; i++) {
        int len = (int)strlen(lines[i]);
        if (len > max_len) max_len = len;
    }
    int x = (width - max_len) / 2;
    if (x < 0) x = 0;

    for (int i = 0; i < n_lines; i++) {
        gotoxy(x, y_start + i * 2);
        printf("%s", lines[i]);
    }
}

void draw_score_and_line() {
    char buf[64];
    int width = get_console_width();

    sprintf(buf, "SCORE: %d", score);
    int x = width - (int)strlen(buf) - 2;
    if (x < 0) x = 0;
    gotoxy(x, 0);
    printf("%s", buf);

    for (int i = 0; i < width; i++) {
        gotoxy(i, 1);
        printf("-");
    }
}

void timing_game() {
    int key;
    clock_t start_time, end_time;
    double t;
    const double target = 10.0;

    srand((unsigned int)time(NULL));

    while (1) {
        system("cls");
        const char *intro_lines[] = {
            "10초 맞추기 게임!",
            "ENTER: 게임 시작    ESC: 나가기"
        };
        print_block(8, intro_lines, 2);

        while (1) {
            key = getch();
            if (key == 27) {
                system("cls");
                return;
            }
            if (key == 13) {
                break;
            }
        }

        system("cls");
        draw_score_and_line();
        const char *ready_lines[] = { "READY?" };
        print_block(10, ready_lines, 1);
        Sleep(1000);

        system("cls");
        draw_score_and_line();
        const char *go_lines[] = { "GO!" };
        print_block(10, go_lines, 1);
        Sleep(1000);

        system("cls");
        draw_score_and_line();
        start_time = clock();

        int faded = 0;

        while (1) {
            clock_t now = clock();
            t = (double)(now - start_time) / CLOCKS_PER_SEC;

            if (t <= 2.7) {
                char buf[64];
                sprintf(buf, "%.2f초", t);
                const char *time_lines[] = { buf };
                print_block(10, time_lines, 1);
            } else {
                if (!faded) {
                    system("cls");
                    draw_score_and_line();

                    const char *msg[] = { "엔터를 눌러 멈추세요!" };
                    print_block(10, msg, 1);
                    faded = 1;
                }
            }

            if (kbhit()) {
                int c = getch();
                if (c == 13) {
                    end_time = clock();
                    break;
                } else if (c == 27) {
                    system("cls");
                    return;
                }
            }

            Sleep(50);
        }

        t = (double)(end_time - start_time) / CLOCKS_PER_SEC;

        system("cls");
        draw_score_and_line();
        const char *stop_lines[] = { "STOP!" };
        print_block(10, stop_lines, 1);
        Sleep(1000);

        system("cls");
        draw_score_and_line();

        char result_line[128];
        char detail_line[128];
        char score_line[128];

        sprintf(result_line, "결과: %.2f초", t);

        double gap = fabs(t - target);
        if (gap <= 0.2) {
            sprintf(detail_line, "성공! 800점 획득!");
            score += 800;
        } else if (t > target) {
            sprintf(detail_line, "%.2f초 느렸습니다.", gap);
        } else {
            sprintf(detail_line, "%.2f초 빨랐습니다.", gap);
        }

        sprintf(score_line, "현재 스코어: %d점", score);

        const char *result_block[] = {
            result_line,
            detail_line,
            score_line,
            "다시 하려면 ENTER, 나가려면 ESC"
        };
        print_block(8, result_block, 4);

        while (1) {
            key = getch();
            if (key == 27) {
                system("cls");
                return;
            }
            if (key == 13) {
                break;
            }
        }
    }
}