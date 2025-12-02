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
int questionCount = 0;

void gotoxy(int x, int y) {
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
    
    if (questionCount == 0) {
        questionFile = fopen("questions.txt", "r");
        answerFile = fopen("answers.txt", "r");

        if (questionFile == NULL || answerFile == NULL) {
            printf("\n [ERROR] 데이터 파일 없음.\n");
            if (questionFile) fclose(questionFile);
            if (answerFile) fclose(answerFile);
            printf(" (엔터 복귀)");
            while(getchar() != '\n'); getchar();
            return;
        }

        while (questionCount < MAX_QUESTIONS && 
               fgets(questions[questionCount], MAX_STRLEN, questionFile) != NULL &&
               fgets(answers[questionCount], MAX_STRLEN, answerFile) != NULL) {
            removeNewline(questions[questionCount]);
            removeNewline(answers[questionCount]);
            questionCount++;
        }
        fclose(questionFile);
        fclose(answerFile);
    }

    if (questionCount == 0) return;

    srand((unsigned int)time(NULL));
    int randomIndex = rand() % questionCount;

    system("cls"); 
    
    printf("\n");
    printf("             /\\____/\\    \n");
    printf("            (  o  o  )     < \"반갑습니다.\"\n"); 
    printf("            (    L   )     < \"문제를 드리겠습니다.\"\n");
    printf("           /|   __   |\\    \n");
    printf("          ( |  |  |  | )   \n");
    printf("           \\|__|__|__/     \n");
    printf("\n");
    printf("   ================================================================\n");
    printf("       🔔   도  전  !     F A N T A S T I C     골  든  벨   🔔   \n");
    printf("   ================================================================\n");
    printf("\n");

    for(int i=0; i<3; i++) {
        Sleep(400); 
        
        gotoxy(12, 2); 
        printf("(  -  -  )"); 
        
        Sleep(200); 

        gotoxy(12, 2); 
        printf("(  o  o  )");
    }

    gotoxy(0, 13); 

    printf("   +--------------------------------------------------------------+\n");
    printf("   |                                                              |\n");
    printf("   |  Q. ");
    
    typeWriter(questions[randomIndex]);
    
    printf("\n"); 
    printf("   |                                                              |\n");
    printf("   +--------------------------------------------------------------+\n");
    printf("\n");
    printf("     ✍️  정답을 칠판에 적어주세요 : ");

    char userAnswer[MAX_STRLEN];
    if (fgets(userAnswer, MAX_STRLEN, stdin) != NULL) {
        removeNewline(userAnswer);
    }

    printf("\n     결과 확인 중...");
    typeWriter(".........."); 
    printf("\n\n");
    
    if (strcmp(userAnswer, answers[randomIndex]) == 0) {
        gotoxy(12, 2); 
        printf("(  ^  ^  )"); 
        gotoxy(0, 22); 

        printf("   ****************************************************************\n");
        printf("   * ⭕   정   답   입   니   다   ! !   ⭕                  *\n");
        printf("   ****************************************************************\n");
        printf("\n            ✨  축하합니다! 1000점을 획득하셨습니다.  ✨\n");
        
        score += 1000; 
    } else {
        gotoxy(12, 2); 
        printf("(  x  x  )"); 
        gotoxy(0, 22); 

        printf("   ################################################################\n");
        printf("   #              ❌   오   답   입   니   다   ❌                #\n");
        printf("   ################################################################\n");
        printf("\n            아쉽네요. 정답은 [ %s ] 입니다.\n", answers[randomIndex]);
    }

    printf("\n   ================================================================\n");
    printf("     (계속하려면 아무 키나 누르세요...)\n");
    _getch();

    maze();

    return 0;
}