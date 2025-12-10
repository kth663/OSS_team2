#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h> 
#include <windows.h> 
#include "data.h" 
#include "maze.h"

#define MAX_QUESTIONS 50 
#define MAX_STRLEN 256

#define COLOR_DEFAULT 7
#define COLOR_BLUE 9
#define COLOR_GREEN 10
#define COLOR_CYAN 11
#define COLOR_RED 12
#define COLOR_YELLOW 14
#define COLOR_WHITE 15
#define COLOR_GRAY 8    
#define COLOR_BRIGHT_WHITE 15

#define UI_X 4          
#define UI_Y 3          
#define UI_WIDTH 60     
#define UI_HEIGHT 19    

char questions[MAX_QUESTIONS][MAX_STRLEN];
char answers[MAX_QUESTIONS][MAX_STRLEN];
char initials[MAX_QUESTIONS][MAX_STRLEN];
int answerLengths[MAX_QUESTIONS];
int questionCount = 0;
            
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void _gotoxy(int x, int y) {
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void typeWriter(const char* str) {
    for (int i = 0; i < strlen(str); i++) {
        printf("%c", str[i]);
        fflush(stdout); 
        Sleep(25);            
    }
}

void removeNewline(char* str) {
    str[strcspn(str, "\n")] = 0;
    str[strcspn(str, "\r")] = 0; 
}

void drawBox() {
    setColor(COLOR_WHITE);
    
    _gotoxy(UI_X, UI_Y);
    printf("┌");
    for(int i=0; i<UI_WIDTH-2; i++) printf("─");
    printf("┐");

    for(int i=1; i<UI_HEIGHT-1; i++) {
        _gotoxy(UI_X, UI_Y + i);
        printf("│");
        _gotoxy(UI_X + UI_WIDTH - 1, UI_Y + i);
        printf("│");
    }

    _gotoxy(UI_X, UI_Y + UI_HEIGHT - 1);
    printf("└");
    for(int i=0; i<UI_WIDTH-2; i++) printf("─");
    printf("┘");

    _gotoxy(UI_X, UI_Y + 2);
    printf("├");
    for(int i=0; i<UI_WIDTH-2; i++) printf("─");
    printf("┤");
}

void drawResultSymbol(int isCorrect, int x, int y) {
    if (isCorrect) {
        setColor(COLOR_GREEN);
        _gotoxy(x, y);     printf("   $$$$$$\\   ");
        _gotoxy(x, y + 1); printf("  $$  __$$\\  ");
        _gotoxy(x, y + 2); printf("  $$ /  $$ | ");
        _gotoxy(x, y + 3); printf("  $$ |  $$ | ");
        _gotoxy(x, y + 4); printf("  \\$$$$$$  | ");
        _gotoxy(x, y + 5); printf("   \\______/  ");
    } else {
        setColor(COLOR_RED);
        _gotoxy(x, y);     printf("  $$\\   $$\\  ");
        _gotoxy(x, y + 1); printf("  $$ |  $$ | ");
        _gotoxy(x, y + 2); printf("  \\$$\\ $$  | ");
        _gotoxy(x, y + 3); printf("   \\$$$$  /  ");
        _gotoxy(x, y + 4); printf("   $$  $$<   ");
        _gotoxy(x, y + 5); printf("  $$  /\\$$\\  ");
    }
    setColor(COLOR_DEFAULT);
}

void quiz() {
    FILE *qF, *aF, *iF, *nF;
    
    if (questionCount == 0) {
        qF = fopen("questions.txt", "r");
        aF = fopen("answers.txt", "r");
        iF = fopen("initials.txt", "r"); 
        nF = fopen("num_char.txt", "r");

        if (!qF || !aF || !iF || !nF) {
            return;
        }

        while (questionCount < MAX_QUESTIONS && 
               fgets(questions[questionCount], MAX_STRLEN, qF) &&
               fgets(answers[questionCount], MAX_STRLEN, aF) &&
               fgets(initials[questionCount], MAX_STRLEN, iF)) {
            
            fscanf(nF, "%d", &answerLengths[questionCount]); 
            removeNewline(questions[questionCount]);
            removeNewline(answers[questionCount]);
            removeNewline(initials[questionCount]);
            questionCount++;
        }
        fclose(qF); fclose(aF); fclose(iF); fclose(nF);
    }
    if (questionCount == 0) return;

    srand((unsigned int)time(NULL));
    int idx = rand() % questionCount;
    int answerLen = answerLengths[idx]; 

    system("cls"); 
    
    drawBox();

    _gotoxy(UI_X + 2, UI_Y + 1);
    setColor(BACKGROUND_BLUE | COLOR_WHITE);
    printf("  NON-SENSE QUIZ STAGE  "); 
    setColor(COLOR_DEFAULT);

    _gotoxy(UI_X + 40, UI_Y + 1);
    setColor(COLOR_YELLOW);
    printf("Score: %d", score);

    setColor(COLOR_WHITE);
    _gotoxy(UI_X + 4, UI_Y + 4);
    printf("Q.");

    _gotoxy(UI_X + 4, UI_Y + 8);
    if (item[2] >= 1) { 
        setColor(COLOR_YELLOW);
        printf(" ★ 힌트 아이템 사용됨! ★");
        _gotoxy(UI_X + 4, UI_Y + 9);
        printf(" >> 초성 : [ %s ]", initials[idx]);
    } else {
        setColor(COLOR_GRAY); 
        printf(" (초성 아이템 미사용)");
    }

    setColor(COLOR_WHITE);
    _gotoxy(UI_X + 4, UI_Y + 11);
    printf("   글자 수 : ");
    
    setColor(COLOR_CYAN);
    for(int i=0; i<answerLen; i++) printf("▣ ");

    setColor(COLOR_WHITE);
    _gotoxy(UI_X + 4, UI_Y + 13);
    printf("   정답 입력 : ");

    fflush(stdout); 

    _gotoxy(UI_X + 4, UI_Y + 5);
    setColor(COLOR_BRIGHT_WHITE); 
    typeWriter(questions[idx]); 

    int inputX = UI_X + 19;
    int inputY = UI_Y + 13;
    char userAnswer[MAX_STRLEN];

    _gotoxy(inputX, inputY);
    printf("▶"); 
    _gotoxy(inputX + 2, inputY);

    while (_kbhit()) _getch();
    fflush(stdin);
   
    setColor(COLOR_CYAN);
    if (fgets(userAnswer, MAX_STRLEN, stdin) != NULL) {
         removeNewline(userAnswer);
    }

    Sleep(300);

    setColor(COLOR_WHITE);
    _gotoxy(UI_X + 2, UI_Y + 15);
    for(int i=0; i<UI_WIDTH-4; i++) printf("─"); 

    if (strcmp(userAnswer, answers[idx]) == 0) {
        Beep(523, 100); Beep(659, 100); Beep(784, 200);
        drawResultSymbol(1, UI_X + 40, UI_Y + 5);
        
        _gotoxy(UI_X + 4, UI_Y + 16);
        setColor(COLOR_GREEN);
        printf("   ★ 정답입니다!! (+1000점) ★");
        score += 1000;
    } else {
        Beep(200, 300); 
        drawResultSymbol(0, UI_X + 40, UI_Y + 5); 

        _gotoxy(UI_X + 4, UI_Y + 16);
        setColor(COLOR_RED);
        printf("   땡! 정답은 [ %s ] 입니다...", answers[idx]);
    }

    setColor(COLOR_DEFAULT);
    _gotoxy(UI_X + 15, UI_Y + UI_HEIGHT - 2);
    printf(">> 아무 키나 누르면 돌아갑니다 <<");
    _getch();

    maze(1); 
}