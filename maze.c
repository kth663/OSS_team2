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
        printf("┃ ");
    }
    else if(ch == 'O'){
        printf("  ");
    }
    else{
        printf("%c ", ch);
    }
}

void resetVisited(int v[27][27]){
    for(int i = 0;i<27;i++){
        for(int j = 0;j<27;j++){
            v[i][j] = 0;
        }
    }
}

int dirX[4] = {0,1,0,-1};
int dirY[4] = {1,0,-1,0};
int findPath(char map[27][27],int v[27][27],int cx,int cy,int dx,int dy){
    if(cx == dx && cy == dy){
        return 1;
    }
    v[cy][cx] = 1;
    int res = 0;
    for(int i = 0;i<4;i++){
        int nx = cx + dirX[i];
        int ny = cy + dirY[i];

        if(map[ny][nx] == 'X')
            continue;
        if (v[ny][nx] == 1)
            continue;               
        setCursorPos(nx, ny);
        
        res = findPath(map,v, nx, ny, dx, dy);
        if(res == 1){
            setCursorPos(cx, cy);
            if(i == 0){
                printf("▽ ");
            }
            else if(i == 1){
                printf("▷ ");
            }
            else if(i == 2){
                printf("△ ");
            }
            else if(i == 3){
                printf("◁ ");
            }
            
            break;
        }
    }
    return res;
}

void resetMaze(char map[27][27]){
    for(int i = 0;i<27;i++){
        for(int j= 0;j<27;j++){
            char ch = map[i][j];
            if(ch == 'X')
                continue;
            setCursorPos(j, i);
            printShape(ch);
        }
        printf("\n");
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

void maze(int do_Animation){

    system("cls");

    // item[4] = 1;//테스트를 위해 아이템 지급 << 2025-12-05 PM 5:29 이거 주석처리함
    
    // ■와 ●가 ??로 출력되는 문제 때문에 설정
    setlocale(LC_CTYPE, "ko_KR.UTF-8");
    //명령어 실행후 메시지가 나오지 않게 함
    system("chcp 65001 > nul");

    FILE* mazeFile;
    fopen_s(&mazeFile, "maze.txt", "r");

    char map[27][27];
    int visited[27][27];
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
            if(ch == 'X'){
                if(i != 0 && map[i-1][j] == 'X' && j != 26 && map[i][j+1] == 'X' && i != 26 && map[i+1][j] == 'X'){
                    printf("┣━");
                }
                else if(j != 0 && map[i][j-1] == 'X' && j != 26 && map[i][j+1] == 'X' && i != 26 && map[i+1][j] == 'X'){
                    printf("┳━");
                }
                else if(j != 0 && map[i][j-1] == 'X' && i != 0 && map[i-1][j] == 'X' && i != 26 && map[i+1][j] == 'X'){
                    printf("┫ ");
                }
                else if(j != 0 && map[i][j-1] == 'X' && i != 0 && map[i-1][j] == 'X' && j != 26 && map[i][j+1] == 'X'){
                    printf("┻━");
                }
                else if(i != 0 && map[i-1][j] == 'X' && i != 26 && map[i+1][j] == 'X'){
                    printf("┃ ");
                }
                else if(j != 0 && map[i][j-1] == 'X' && j != 26 && map[i][j+1] == 'X'){
                    printf("━━");
                }
                else if(i != 26 && map[i+1][j] == 'X' && j != 26 && map[i][j+1] == 'X'){
                    printf("┏━");
                }
                else if(i != 0 && map[i-1][j] == 'X' && j != 26 && map[i][j+1] == 'X'){
                    printf("┗━");
                }
                else if(i != 0 && map[i-1][j] == 'X' && j != 0 && map[i][j-1] == 'X'){
                    printf("┛ ");
                }
                else if(i != 26 && map[i+1][j] == 'X' && j != 0 && map[i][j-1] == 'X'){
                    printf("┓ ");
                }
                else{
                    printf("  ");
                }
                continue;
            }
            printShape(ch);
        }
        printf("\n");
        if(do_Animation)
            Sleep(50);
    }


    setCursorPos(30,1);
    printf("┌──────────────────────────────────────┐");

    setCursorPos(30,2);
    printf("│G: 뱀게임(1)                          │");
    setCursorPos(30,3);
    printf("│T: 타이밍 게임(2)                     │");
    setCursorPos(30,4);
    printf("│R: 넌센스 퀴즈(3)                     │");
    setCursorPos(30,5);
    printf("│L: 슬롯머신(4)                        │");
    setCursorPos(30,6);
    printf("│E: 출구(5)                            │");

    setCursorPos(30,7);
    printf("└──────────────────────────────────────┘");

    setCursorPos(30,9);
    printf("┌────────────────────────────┐");
    setCursorPos(30,10);
    printf("│e: 인벤토리 열기            │");
    setCursorPos(30,11);
    printf("│r: 상점 열기                │");
    setCursorPos(30,12);
    printf("└────────────────────────────┘");


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
                break;
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
        if(item[4] == 1){
            if(input == '1'){
                resetVisited(visited);
                resetMaze(map);
                findPath(map, visited, x, y, 13, 3);
            }
            else if(input == '2'){
                resetVisited(visited);
                resetMaze(map);
                findPath(map, visited, x, y, 19, 13);
            }
            else if(input == '3'){
                resetVisited(visited);
                resetMaze(map);
                findPath(map, visited, x, y, 13, 19);
            }
            else if(input == '4'){
                resetVisited(visited);
                resetMaze(map);
                findPath(map,visited, x, y, 17, 21);
            }
            else if(input == '5'){
                resetVisited(visited);
                resetMaze(map);
                findPath(map,visited, x, y, 25, 25);
            }
        }
        
        if(input == 'r'){
            //closeMaze();
            //상점 함수 불러오는 위치
            store();
            break;
        }
        else if(input == 'e'){
            //closeMaze();
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