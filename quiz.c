#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

#define MAX_QUESTIONS 50 
#define MAX_STRLEN 256

extern int score;

char questions[MAX_QUESTIONS][MAX_STRLEN];
char answers[MAX_QUESTIONS][MAX_STRLEN];
int questionCount = 0;

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
            printf("\n [ERROR] 문제 데이터를 찾을 수 없습니다.\n");
            if (questionFile) fclose(questionFile);
            if (answerFile) fclose(answerFile);
            printf(" (엔터를 누르면 메뉴로 복귀합니다)");
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
    printf("            (  -  -  )     < \"반갑습니다. 진행을 맡았습니다.\"\n");
    printf("            (    L   )     < \"준비되셨으면 문제를 드리겠습니다.\"\n");
    printf("           /|   __   |\\    \n");
    printf("          ( |  |  |  | )   \n");
    printf("           \\|__|__|__/     \n");
    printf("\n");
    printf("   ================================================================\n");
    printf("                        🔔  넌센스 퀴즈  🔔   \n");
    printf("   ================================================================\n");
    printf("\n");


    printf("   +--------------------------------------------------------------+\n");
    printf("   |                                                              |\n");
    printf("   |  Q. %-56s |\n", questions[randomIndex]); 
    printf("   |                                                              |\n");
    printf("   +--------------------------------------------------------------+\n");
    printf("\n");
    printf("     ✍️  정답을 칠판에 적어주세요 : ");

    char userAnswer[MAX_STRLEN];
    if (fgets(userAnswer, MAX_STRLEN, stdin) != NULL) {
        removeNewline(userAnswer);
    }

    printf("\n     결과를 확인하고 있습니다. 잠시만 기다려주세요...\n\n");
    

    if (strcmp(userAnswer, answers[randomIndex]) == 0) {
        printf("   ****************************************************************\n");
        printf("   * *\n");
        printf("   * ⭕   정   답   입   니   다   ! !   ⭕                  *\n");
        printf("   * *\n");
        printf("   ****************************************************************\n");
        printf("\n");
        printf("            ✨  축하합니다! 1000점을 획득하셨습니다.  ✨\n");
        
        score += 1000; 
    } else {
        printf("   ################################################################\n");
        printf("   #                                                              #\n");
        printf("   #              ❌   오   답   입   니   다   ❌                #\n");
        printf("   #                                                              #\n");
        printf("   ################################################################\n");
        printf("\n");
        printf("            아쉽네요. 정답은 [ %s ] 입니다.\n", answers[randomIndex]);
    }

    printf("\n   ================================================================\n");
    printf("     (계속하려면 아무 키나 누르세요...)\n");
    _getch();
}