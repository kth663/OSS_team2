#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<locale.h>

void setCursorPos(int x,int y);

int maze(){
    
    // ■와 ●가 ??로 출력되는 문제 때문에 설정
    setlocale(LC_CTYPE, "ko_KR.UTF-8");
    //명령어 실행후 메시지가 나오지 않게 함
    system("chcp 65001 > nul");

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

    for(int i = 0;i<27;i++){
        for(int j= 0;j<27;j++){
            char ch = map[i][j];
            if(ch == 'X'){
                printf("■ ");
            }
            else if(ch == 'O'){
                printf("  ");
            }
            else{
                printf("%c ", ch);
            }
        }
        printf("\n");
    }

    int x = 1;
    int y = 1;

    setCursorPos(x, y);

    printf("● ");
    

}

void setCursorPos(int x,int y){
    COORD pos;
    //문자열의 가로가 2칸이기 때문에 2를 곱함
    pos.X = x * 2;
    pos.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}