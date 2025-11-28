#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "maze.h"  

#define MAX_QUESTIONS 50 
#define MAX_STRLEN 256

char questions[MAX_QUESTIONS][MAX_STRLEN];
char answers[MAX_QUESTIONS][MAX_STRLEN];
int questionCount = 0;

// fgets 같은거 쓰다보면 \n 들어오니 그거 제거해줘야됨
void removeNewline(char* str) {
    str[strcspn(str, "\n")] = 0;
}

// 넌센스 퀴즈 함수
void quiz() {
    FILE* questionFile;
    FILE* answerFile;
    
    // 파일 읽기
    if (questionCount == 0) {
        // 파일 열기
        questionFile = fopen("questions.txt", "r");
        answerFile = fopen("answers.txt", "r");

        if (questionFile == NULL || answerFile == NULL) {
            printf("오류: 데이터 파일(questions.txt, answers.txt)을 찾을 수 없습니다.\n");
            if (questionFile) fclose(questionFile);
            if (answerFile) fclose(answerFile);
            return;
        }

        //읽는 부분
        while (questionCount < MAX_QUESTIONS && 
               fgets(questions[questionCount], MAX_STRLEN, questionFile) != NULL &&
               fgets(answers[questionCount], MAX_STRLEN, answerFile) != NULL) {
            
            removeNewline(questions[questionCount]);
            removeNewline(answers[questionCount]);
            
            questionCount++;
        }
        
        // 파일 닫기
        fclose(questionFile);
        fclose(answerFile);
        
        printf("[시스템] 퀴즈 데이터 %d개를 로드했습니다.\n", questionCount);
    }

    // 오류 생길 때
    if (questionCount == 0) {
        printf("오류: 출제할 퀴즈가 없습니다.\n");
        return;
    }

    // 랜덤 문제 출제
    srand((unsigned int)time(NULL)); // 랜덤 시드 초기화
    int randomIndex = rand() % questionCount;

    printf("\n========== [넌센스 퀴즈] ==========\n");
    printf("문제: %s\n", questions[randomIndex]);
    printf("===================================\n");
    printf("정답 입력: ");

    // 입력 받기
    char userAnswer[MAX_STRLEN];
    if (fgets(userAnswer, MAX_STRLEN, stdin) != NULL) {
        removeNewline(userAnswer); // 입력값의 \n 제거
    }

    // 정답 비교
    if (strcmp(userAnswer, answers[randomIndex]) == 0) {
        printf("\n>>> 정답입니다!! (스코어 +1000) <<<\n");
        // 스코어 변수 받아와서 스코어 = 스코어 + 1000 해줘야됨 
    } else {
        printf("\n>>> 땡! 틀렸습니다. <<<\n");
        printf("정답은 ['%s'] 입니다.\n", answers[randomIndex]);
    }

    //system("pause");

    maze();
}