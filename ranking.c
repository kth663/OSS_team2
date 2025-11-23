#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 
#include <windows.h>
#include <time.h>

typedef struct {
    char initials[4]; 
    int score;       
} RankEntry;


#define FILENAME "ranking.txt"
#define MAX_RECORDS 100 


void set_utf8_encoding() {
    if (SetConsoleOutputCP(65001) == 0) {
        fprintf(stderr, "SetConsoleOutputCP failed. Error: %lu\n", GetLastError());
    }

    if (SetConsoleCP(65001) == 0) {
        fprintf(stderr, "SetConsoleCP failed. Error: %lu\n", GetLastError());
    }

    SetConsoleOutputCP(65001);
    
}    

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


int compareRanks(const void *a, const void *b);
int getUserInput(RankEntry *newEntry);
int loadRanks(RankEntry ranks[]);
void saveRanks(const RankEntry ranks[], int count);
void printRanks(const RankEntry ranks[], int count);


int compareRanks(const void *a, const void *b) {
    RankEntry *rankA = (RankEntry *)a;
    RankEntry *rankB = (RankEntry *)b;

    return rankB->score - rankA->score; 
};

int getUserInput(RankEntry *newEntry) {
    char inputBuffer[100]; 

    printf("\n>>> 게임 점수를 입력하세요: ");
    if (fgets(inputBuffer, sizeof(inputBuffer), stdin) == NULL || sscanf(inputBuffer, "%d", &newEntry->score) != 1) {
        printf("⚠️ 오류: 유효하지 않은 점수 입력입니다.\n");
        return 0;
    }
    
    while (1) {
        printf(">>> 3글자 이니셜을 입력하세요 (예: ABC): ");
        if (fgets(inputBuffer, sizeof(inputBuffer), stdin) == NULL || sscanf(inputBuffer, "%s", newEntry->initials) != 1) {
            printf("⚠️ 오류: 유효하지 않은 이니셜 입력입니다.\n");
            return 0;
        }

        size_t len = strlen(newEntry->initials);

        if (len == 3) {
            int isValid = 1;
            for (size_t i = 0; i < len; i++) {
                if (!isalpha(newEntry->initials[i])) {
                    isValid = 0;
                    break;
                }
                newEntry->initials[i] = (char)toupper(newEntry->initials[i]);
            }

            if (isValid) {
                newEntry->initials[3] = '\0'; 
                break;
            }
        }
        
        printf("⚠️ 이니셜은 정확히 3개의 알파벳으로만 구성되어야 합니다. 다시 입력해주세요.\n");
    }
    return 1;
}

int loadRanks(RankEntry ranks[]) {
    FILE *fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        return 0; 
    }

    int count = 0;
    while (count < MAX_RECORDS && fscanf(fp, "%s %d", ranks[count].initials, &ranks[count].score) == 2) {
        count++;
    }

    fclose(fp);
    return count;
}

void saveRanks(const RankEntry ranks[], int count) {
    FILE *fp = fopen(FILENAME, "w");
    if (fp == NULL) {
        printf("⚠️ 오류: 랭킹 파일 (%s)을 저장할 수 없습니다.\n", FILENAME);
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s %d\n", ranks[i].initials, ranks[i].score);
    }

    fclose(fp);
}

void printRanks(const RankEntry ranks[], int count) {
    printf("\n\n================================\n");
    printf("        🏆 TOP 랭킹 페이지 🏆\n");
    printf("================================\n");
    printf("순위 | 이니셜 | 점수\n");
    printf("--------------------------------\n");
    
    for (int i = 0; i < count; i++) {
        printf(" %2d. |  %s  | %d\n", i + 1, ranks[i].initials, ranks[i].score);
    }

    printf("================================\n");
}

int main() {

    set_utf8_encoding();

    srand(time(NULL));

    RankEntry allRanks[MAX_RECORDS]; 
    int recordCount;                 
    RankEntry newRecord;            

    recordCount = loadRanks(allRanks);
    printf(" 기존 랭킹 기록 %d개를 불러왔습니다.\n", recordCount);

    if (!getUserInput(&newRecord)) {
        printf("프로그램을 종료합니다.\n");
        return 1;
    }

    if (recordCount < MAX_RECORDS) {
        allRanks[recordCount] = newRecord;
        recordCount++;
        printf("✅ 새로운 기록 (%s, %d점)이 랭킹에 추가되었습니다.\n", newRecord.initials, newRecord.score);
    } else {

        if (newRecord.score > allRanks[MAX_RECORDS - 1].score) {
            printf("⭐ 기록 저장 공간이 가득 찼지만, 새로운 기록 (%s, %d점)이 현재 랭킹에 들었습니다.\n", newRecord.initials, newRecord.score);
            printf("🔽 기존 최하위 기록 (%s, %d점)을 대체합니다.\n", allRanks[MAX_RECORDS - 1].initials, allRanks[MAX_RECORDS - 1].score);
            allRanks[MAX_RECORDS - 1] = newRecord;
        } else {
            printf("⚠️ 기록 저장 공간이 가득 찼으며, 새로운 기록 (%s, %d점)은 현재 랭킹 100위 안에 들지 못했습니다.\n", newRecord.initials, newRecord.score);
        }
    }

    qsort(allRanks, recordCount, sizeof(RankEntry), compareRanks);

    printRanks(allRanks, recordCount);
    
    saveRanks(allRanks, recordCount);
    
    printf("\n** 랭킹 정보가 %s 파일에 저장되었습니다. **\n", FILENAME);
    
    printf("\n[창을 닫으려면 엔터(Enter) 키를 누르세요...]\n");

    clearInputBuffer();

    getchar();

    return 0;
}