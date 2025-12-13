#include "ranking.h"
#include <windows.h> 
#include <time.h>
#include "data.h"
#include "conio.h"

void set_utf8_encoding() {
    if (SetConsoleOutputCP(65001) == 0) {
        fprintf(stderr, "SetConsoleOutputCP failed. Error: %lu\n", GetLastError());
    }

    if (SetConsoleCP(65001) == 0) {
        fprintf(stderr, "SetConsoleCP failed. Error: %lu\n", GetLastError());
    }

    SetConsoleOutputCP(65001);
} 

void _clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int compareRanks(const void *a, const void *b) {
    RankEntry *rankA = (RankEntry *)a;
    RankEntry *rankB = (RankEntry *)b;

    return rankA->time - rankB->time; 
}

int getUserInput(RankEntry *newEntry) {
    char inputBuffer[100]; 
    char *endptr;
    long score_val; 

    // printf("\n>>> 게임 점수를 입력하세요: ");
    // if (fgets(inputBuffer, sizeof(inputBuffer), stdin) == NULL) {
    //     printf("오류: 입력 오류가 발생했습니다.\n");
    //     return 0;
    // }
    
    // size_t len = strlen(inputBuffer);
    // if (len > 0 && inputBuffer[len - 1] == '\n') {
    //     inputBuffer[len - 1] = '\0';
    // } else {
    //     _clearInputBuffer(); 
    // }
    
    // score_val = strtol(inputBuffer, &endptr, 10);
    
    // if (endptr == inputBuffer || *endptr != '\0' || score_val > INT_MAX || score_val < 0) {
    //     printf("오류: 유효하지 않거나 너무 큰 점수 입력입니다. (최대 %d)\n", INT_MAX);
    //     return 0;
    // }
    
    // newEntry->score = (int)score_val;
    
    while (1) {
        printf(">>> 3글자 이니셜을 입력하세요 (예: ABC): ");
        
        if (fgets(inputBuffer, sizeof(inputBuffer), stdin) == NULL) {
            printf("오류: 입력 오류가 발생했습니다.\n");
            return 0;
        }

        size_t input_len = strlen(inputBuffer);
        int hasNewline = 0;
        if (input_len > 0 && inputBuffer[input_len - 1] == '\n') {
            inputBuffer[input_len - 1] = '\0';
            input_len--;
            hasNewline = 1;
        }

        if (!hasNewline) {
            _clearInputBuffer(); 
        }

        if (input_len == 3) {
            int isValid = 1;
            for (size_t i = 0; i < input_len; i++) {
                if (!isalpha(inputBuffer[i])) {
                    isValid = 0;
                    break;
                }
                newEntry->initials[i] = (char)toupper(inputBuffer[i]);
            }

            if (isValid) {
                newEntry->initials[3] = '\0'; 
                break; 
            }
        }
        
        printf("이니셜은 정확히 3개의 알파벳으로만 구성되어야 합니다. 다시 입력해주세요.\n");
    }
    return 1;
}

int loadRanks(RankEntry ranks[]) {
    FILE *fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        return 0; 
    }

    int count = 0;
    while (count < MAX_RECORDS && fscanf(fp, "%s %d", ranks[count].initials, &ranks[count].time) == 2) {
        count++;
    }

    fclose(fp);
    return count;
}

void saveRanks(const RankEntry ranks[], int count) {
    FILE *fp = fopen(FILENAME, "w");
    if (fp == NULL) {
        printf("오류: 랭킹 파일 (%s)을 저장할 수 없습니다.\n", FILENAME);
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s %d\n", ranks[i].initials, ranks[i].time);
    }

    fclose(fp);
}

void printRanks(const RankEntry ranks[], int count) {
    printf("\n\n================================\n");
    printf("       TOP 랭킹 페이지      \n");
    printf("================================\n");
    printf("순위 | 이니셜 | 플레이타임\n");
    printf("--------------------------------\n");
    
    for (int i = 0; i < count; i++) {
        printf(" %2d. |  %s  | %d : %d \n", i + 1, ranks[i].initials, ranks[i].time/60,ranks[i].time%60);
    }

    printf("================================\n");
}

void displayExitRankingMessage(const char *filename) {
    printf("\n** 랭킹 정보가 %s 파일에 저장되었습니다. **\n", filename);
    printf("\n[다시 시작하려면 에스(S) 키를 누르세요...]\n");
    printf("\n[창을 닫으려면 엔터(Enter) 키를 누르세요...]\n");
    //_clearInputBuffer();
    while(1){
        char c = _getch();
        if(c == 's'){
            startGame();
            return;
        }
        else if(c == 13){
            return;
        }
        printf("Enter와 S중 선택해주세요\n");
    } 
    
}


int executeRankingManager() {

    set_utf8_encoding(); 

    system("cls");

    srand(time(NULL)); 

    RankEntry allRanks[MAX_RECORDS]; 
    int recordCount;             
    RankEntry newRecord;           

    recordCount = loadRanks(allRanks);
    printf(" 기존 랭킹 기록 %d개를 불러왔습니다.\n", recordCount);

    int playTime = endTime - startTime;
    newRecord.time = playTime;
    if (!getUserInput(&newRecord)) {
        printf("기록 입력 중 오류 발생. 프로그램이 종료됩니다.\n");
        return 1;
    }

    if (recordCount < MAX_RECORDS) {
        allRanks[recordCount] = newRecord;
        recordCount++;
        printf("새로운 기록 (%s, %d점)이 랭킹에 추가되었습니다.\n", newRecord.initials, newRecord.time);
    } else {
        qsort(allRanks, recordCount, sizeof(RankEntry), compareRanks); 
        
        if (newRecord.time > allRanks[MAX_RECORDS - 1].time) {
            printf("기록 저장 공간이 가득 찼지만, 새로운 기록 (%s, %d점)이 현재 랭킹에 들었습니다.\n", newRecord.initials, newRecord.time);
            printf("기존 최하위 기록 (%s, %d점)을 대체합니다.\n", allRanks[MAX_RECORDS - 1].initials, allRanks[MAX_RECORDS - 1].time);
            allRanks[MAX_RECORDS - 1] = newRecord; 
        } else {
            printf("기록 저장 공간이 가득 찼으며, 새로운 기록 (%s, %d점)은 현재 랭킹 100위 안에 들지 못했습니다.\n", newRecord.initials, newRecord.time);
        }
    }

    qsort(allRanks, recordCount, sizeof(RankEntry), compareRanks);
    printRanks(allRanks, recordCount);
    
    saveRanks(allRanks, recordCount);
    
    displayExitRankingMessage(FILENAME);

    return 0;
}