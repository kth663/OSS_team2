#ifndef START_H
#define START_H
#include <windows.h>
#include <stdio.h>
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

void gotoxy(int x, int y);
void show_loading_bar();
void blink_press_enter();

void start_screen(void);

#endif