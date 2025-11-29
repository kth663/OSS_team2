#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<locale.h>
#include<conio.h>

#include "password_game.h"
#include "snake.h"
#include "timing.h"
#include "data.h"
#include "quiz.h"
#include "slot.h"
#include "store.h"
#include "slot and game.h"



//void setCursorPos(int x,int y);

//미로의 문자에 따라 모양을 출력하는 함수
void printShape(char ch){
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

//미로 종료시 천천히 사라지게하는 함수
void closeMaze(){
    for(int i = 27/3;i>=0;i--){
        for(int j = 0;j<3;j++){
            setCursorPos(0, i*3 + j);
            printf("                                                       ");
        }
        
        Sleep(50);
    }
    
}

void maze(){

    system("cls");
    
    // ■와 ●가 ??로 출력되는 문제 때문에 설정
    setlocale(LC_CTYPE, "ko_KR.UTF-8");
    //명령어 실행후 메시지가 나오지 않게 함
    system("chcp 65001 > nul");

    FILE* mazeFile;
    fopen_s(&mazeFile, "maze.txt", "r");

    char map[27][27];
    //미로 불려오기
    for(int i = 0;i<27;i++){
        for(int j = 0;j<27;j++){
            fscanf_s(mazeFile, "%c", &map[i][j], 1);
        }
        // 엔터를 저장하지 않기 위해 엔터 읽기
        char temp;
        fscanf_s(mazeFile,"%c", &temp, 1); 
    }

    fclose(mazeFile);

    //미로 출력
    for(int i = 0;i<27;i++){
        for(int j= 0;j<27;j++){
            char ch = map[i][j];
            printShape(ch);
        }
        printf("\n");
        Sleep(50);
    }



    setCursorPos(x, y);

    printf("● ");

    while(1){
        //입력시 이동 또는 스테이지 전환
        char input = _getch();
        int nx = x;
        int ny = y;
        if(input == 13){
            if(map[y][x] == 'E'){
                closeMaze();
                //출구 함수 불러오는 위치
                runGame();
                break;
            }
            else if(map[y][x] == 'G'){
                closeMaze();
                //뱀 게임 함수 불러오는 위치
                snakeGame();
                break;
            }
            else if(map[y][x] == 'T'){
                closeMaze();
                //타이밍 맞추는 함수 불러오는 위치
                timing_game();
                for (int i=0; i<27; i++){
                    for (int j=0; j<27; j++){
                        char ch = map[i][j];
                        printShape(ch);
                    }
                    printf("\n");
                }
                setCursorPos(x, y);
                printf("● ");
                continue;
            }
            else if(map[y][x] == 'R'){
                closeMaze();
                //넌센스 퀴즈 함수 불러오는 위치
                quiz();
                break;
            }
            else if(map[y][x] == 'L'){
                closeMaze();
                //슬룻 머신 함수 불러오는 위치
                runSlotMachine();
                break;
            }
            
        }
        if(input == 'r'){
            closeMaze();
            //상점 함수 불러오는 위치
            store();
            break;
        }
        else if(input == 'e'){
            closeMaze();
            //인벤토리 함수 불러오는 위치
            openInventory();
            break;
        }
        if(input == 'w'){
            ny--;
        }
        else if(input == 's'){
            ny++;
        }
        else if(input == 'a'){
            nx--;
        }
        else if(input == 'd'){
            nx++;
        }

        //벽이면 이동하지 않음
        if(map[ny][nx] == 'X'){
            continue;
        }

        //플레이어 이동
        setCursorPos(x, y);
        printShape(map[y][x]);
        setCursorPos(nx, ny);
        printf("● ");

        x = nx;
        y = ny;
    }
    

}

//커서의 위치를 조절하는 함수
// void setCursorPos(int x,int y){
//     COORD pos;
//     //문자열의 가로가 2칸이기 때문에 2를 곱함
//     pos.X = x * 2;
//     pos.Y = y;

//     SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
// }