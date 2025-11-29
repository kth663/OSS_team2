#ifndef QUIZ_H
#define QUIZ_H

#define MAX_QUESTIONS 50
#define MAX_STRLEN 256

// 외부 변수 선언 (quiz.c에서 실제 정의됨)
extern char questions[MAX_QUESTIONS][MAX_STRLEN];
extern char answers[MAX_QUESTIONS][MAX_STRLEN];
extern int questionCount;

// 함수 선언
void removeNewline(char* str);
void quiz(void);

#endif