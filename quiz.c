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

char questions[MAX_QUESTIONS][MAX_STRLEN];
char answers[MAX_QUESTIONS][MAX_STRLEN];
char initials[MAX_QUESTIONS][MAX_STRLEN]; 
int questionCount = 0;

void _gotoxy(int x, int y) {
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void typeWriter(const char* str) {
    for (int i = 0; i < strlen(str); i++) {
        printf("%c", str[i]); 
        Sleep(35);            
    }
}

void removeNewline(char* str) {
    str[strcspn(str, "\n")] = 0;
}

void quiz() {
    FILE* questionFile;
    FILE* answerFile;
    FILE* initialFile;
    
    if (questionCount == 0) {
        questionFile = fopen("questions.txt", "r");
        answerFile = fopen("answers.txt", "r");
        initialFile = fopen("initials.txt", "r"); 

        if (questionFile == NULL || answerFile == NULL || initialFile == NULL) {
            printf("\n [ERROR] 데이터 파일(문제, 정답, 초성) 중 하나가 없습니다.\n");
            if (questionFile) fclose(questionFile);
            if (answerFile) fclose(answerFile);
            if (initialFile) fclose(initialFile);
            printf(" (엔터 복귀)");
            while(getchar() != '\n'); getchar();
            return;
        }

        while (questionCount < MAX_QUESTIONS && 
               fgets(questions[questionCount], MAX_STRLEN, questionFile) != NULL &&
               fgets(answers[questionCount], MAX_STRLEN, answerFile) != NULL &&
               fgets(initials[questionCount], MAX_STRLEN, initialFile) != NULL) {
            
            removeNewline(questions[questionCount]);
            removeNewline(answers[questionCount]);
            removeNewline(initials[questionCount]);
            questionCount++;
        }
        fclose(questionFile);
        fclose(answerFile);
        fclose(initialFile);
    }

    if (questionCount == 0) return;

    srand((unsigned int)time(NULL));
    int randomIndex = rand() % questionCount;

    system("cls"); 
    
    printf("\n");
    printf("              ,     ,  \n");
    printf("             / \\___/ \\ \n");
    printf("            (  @   @  )      < \"멍!! 멍!!\"\n");
    printf("            /   > <   \\      < \"내가 왔다멍! 퀴즈 풀라멍!\"\n");
    printf("           /  _______  \\     \n");
    printf("          /  /       \\  \\    \n");
    printf("         (_ /         \\ _)   \n");
    printf("\n");
    printf("   ================================================================\n");
    printf("       🔔                     넌센스 퀴즈 !                   🔔   \n");
    printf("   ================================================================\n");
    printf("\n");

    for(int i=0; i<3; i++) {
        Sleep(400); 
        _gotoxy(15, 2); printf("(  -   -  )"); 
        Sleep(200); 
        _gotoxy(15, 2); printf("(  @   @  )"); 
    }

    _gotoxy(0, 14); 

    printf("   +--------------------------------------------------------------+\n");
    printf("   |                                                              |\n");
    printf("   |  Q. ");
    typeWriter(questions[randomIndex]);
    printf("\n"); 
    printf("   |                                                              |\n");
    printf("   +--------------------------------------------------------------+\n");
    
    int hintUsed = 0;
    char userAnswer[MAX_STRLEN];

    while(1) {
        _gotoxy(0, 19);
        printf("   ----------------------------------------------------------------\n");
        printf("    [1] ✍️ 정답 입력하기    [2] 💡 초성 힌트 보기 (500점 차감) \n");
        printf("   ----------------------------------------------------------------\n");
        
        while(getchar() != '\n');

        if(hintUsed) {
            _gotoxy(0, 18);
            printf("    💡 힌트 : [ %s ]          ", initials[randomIndex]);
        }

        _gotoxy(0, 23);
        printf("    👉 선택 : ");
        
        int key = _getch();
        int choice = key - '0';

        if (choice == 2) {
            if (!hintUsed) {
                hintUsed = 1;
                /*힌트 사용시 여기서 뭔가 감소하게 만들기*/
            }
        }
        else if (choice == 1) {
            break;
        }
    }

    _gotoxy(0, 23); printf("    ✍️ 정답을 칠판에 적어주라멍 :                   ");
    _gotoxy(34, 23);

    if (fgets(userAnswer, MAX_STRLEN, stdin) != NULL) {
        removeNewline(userAnswer);
    }

    printf("\n     킁킁... 정답 냄새를 맡는 중이다멍...");
    typeWriter(".........."); 
    printf("\n\n");
    
    if (strcmp(userAnswer, answers[randomIndex]) == 0) {
        _gotoxy(15, 2); printf("(  ^   ^  )"); 
        _gotoxy(0, 27); 

        printf("   ****************************************************************\n");
        printf("   * ⭕   딩   동   댕   ! ! !   ⭕                  *\n");
        printf("   ****************************************************************\n");
        
        printf("\n            ✨  대 단 하 다 멍 !  (+%d점 획득)  ✨\n", 1000);
        
        score += 1000; 
    } else {
        _gotoxy(15, 2); printf("(  T   T  )"); 
        _gotoxy(0, 27); 

        printf("   ################################################################\n");
        printf("   #              ❌   땡   !   틀   렸   다   멍   ❌                #\n");
        printf("   ################################################################\n");
        printf("\n            아쉽다멍... 정답은 [ %s ] 였다멍!\n", answers[randomIndex]);
    }

    printf("\n   ================================================================\n");
    printf("     (계속하려면 아무 키나 누르라멍...)\n");
    _getch();

    maze();
}