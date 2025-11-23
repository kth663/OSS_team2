#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include <fcntl.h>
#include <io.h>

#define PASSWORD_LENGTH 5
#define MAX_INPUT_LENGTH (PASSWORD_LENGTH + 1)
#define DATA_FILE_NAME "password.txt"

void set_utf8_encoding() {
    if (SetConsoleOutputCP(65001) == 0) {
        fprintf(stderr, "SetConsoleOutputCP failed. Error: %lu\n", GetLastError());
    }

    if (SetConsoleCP(65001) == 0) {
        fprintf(stderr, "SetConsoleCP failed. Error: %lu\n", GetLastError());
    }

    SetConsoleOutputCP(65001);
    
}    

void generateRandomPassword(char password[]) {
    const char CHAR_SET[] = "abcdefghijklmnopqrstuvwxyz";
    int charSetSize = strlen(CHAR_SET);

    for (int i = 0; i < PASSWORD_LENGTH; i++) {
        int randomIndex = rand() % charSetSize;
        password[i] = CHAR_SET[randomIndex];
    }
    password[PASSWORD_LENGTH] = '\0';
};

void savePasswordToFile(const char password[]) {
    FILE *fp = NULL;
    errno_t err;

    err = fopen_s(&fp, DATA_FILE_NAME, "w");

    if (err != 0) {
        printf("⚠️ 오류: 파일 (%s)을 열 수 없습니다. 비밀번호를 저장하지 못했습니다.\n", DATA_FILE_NAME);
        return;
    }

    fprintf(fp, "%s", password);
    
    fclose(fp);

}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


int main() {
    set_utf8_encoding();

    srand(time(NULL));

    char secretPassword[MAX_INPUT_LENGTH];
    char userGuess[MAX_INPUT_LENGTH];
    int scanResult;


    generateRandomPassword(secretPassword);
    savePasswordToFile(secretPassword);

    printf("==========================================\n");
    printf("    탈출하려면 최종 비밀번호를 입력하세요!   \n");
    printf("==========================================\n");
    printf("  (비밀번호는 영문 소문자 a-z로 구성됩니다.) \n");
    printf("------------------------------------------\n");

    while (true) {
        printf("\n   5글자 비밀번호를 입력하세요: ");
        
        scanResult = scanf_s("%s", userGuess, MAX_INPUT_LENGTH);
        
        clearInputBuffer(); 

        if (scanResult != 1) { 
            printf("\n입력 오류가 발생했습니다. 다시 시도해주세요.\n");
            continue;
        }

        size_t len = strlen(userGuess);

        if (len != PASSWORD_LENGTH) {
            printf(" 입력은 반드시 5글자여야 합니다. 다시 입력해주세요.\n", PASSWORD_LENGTH);
            continue;
        }

        if (strcmp(secretPassword, userGuess) == 0) {
            printf("\n 정답입니다! \n");
            break; 
        } else {
            printf("\n 틀렸습니다. 다시 시도해보세요.\n"); 
        }
    }

    printf("\n[창을 닫으려면 엔터(Enter) 키를 누르세요...]\n");

    clearInputBuffer();

    getchar();


    return 0;
}