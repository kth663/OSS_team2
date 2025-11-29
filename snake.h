#ifndef SNAKE_H
#define SNAKE_H

void snakeGame(void);
void setCursorPos(int x, int y);
void pushPos(int ax[], int ay[], int x, int y, int size);
void popPos(int ax[], int ay[], int x, int y, int size);
void spawnApple(int* appleIdx, int* appleX, int* appleY,
                int applePosX[], int applePosY[], int size);

#endif