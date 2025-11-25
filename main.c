#include <stdio.h>
#include "quiz.c"
#include "store.c"

int score = 10000;
int clues[5] = {0, 0, 0, 0, 0};

int main(void){
    system("chcp 65001");
    store();
    return 0;
}