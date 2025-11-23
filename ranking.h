#ifndef RANKING_MANAGER_H
#define RANKING_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 
#include <limits.h> 
typedef struct {
    char initials[4];   
    char padding[4];    
    int score;         
} RankEntry;


#define FILENAME "ranking.txt"
#define MAX_RECORDS 100 


void set_utf8_encoding();
void clearInputBuffer();
int compareRanks(const void *a, const void *b);
int getUserInput(RankEntry *newEntry);
int loadRanks(RankEntry ranks[]);
void saveRanks(const RankEntry ranks[], int count);
void printRanks(const RankEntry ranks[], int count);

int executeRankingManager();

void displayExitRankingMessage(const char *filename);

#endif