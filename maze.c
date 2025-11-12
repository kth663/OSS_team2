#include<stdio.h>
#include<stdlib.h>
#include<windows.h>

int maze(){
    
    FILE* mazeFile;
    fopen_s(&mazeFile, "maze.txt", "r");

    char map[27][27];

    for(int i = 0;i<27;i++){
        for(int j = 0;j<27;j++){
            fscanf_s(mazeFile, "%c", &map[i][j], 1);
        }
        // 엔터를 저장하지 않기 위해 엔터 읽기
        char temp;
        fscanf_s(mazeFile,"%c", &temp, 1); 
    }

    
}