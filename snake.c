#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<locale.h>
#include<conio.h>


typedef struct snake{
    int x;
    int y;

    struct snake* next; 
    struct snake* pre;
}snake;

void setCursorPos(int,int);



void snackGame(){

    // ■와 ●가 ??로 출력되는 문제 때문에 설정
    setlocale(LC_CTYPE, "ko_KR.UTF-8");
    //명령어 실행후 메시지가 나오지 않게 함
    system("chcp 65001 > nul");

    int mapSize = 5;
    mapSize += 2;

    for(int i = 0;i<mapSize;i++){
        for(int j = 0;j<mapSize;j++){
            if(i == 0 || j == 0 || i == mapSize-1 || j == mapSize-1){
                printf("■ ");
            }
            else{
                printf("  ");
            }
        }
        printf("\n");
    }
    

    snake* head;
    snake* tail;
    snake* newBody;
    newBody = (snake*)malloc(sizeof(snake));
    newBody->x = mapSize/2;
    newBody->y = mapSize/2;
    newBody->next = NULL;
    newBody->pre = NULL;
    head = newBody;

    newBody = (snake*)malloc(sizeof(snake));
    newBody->x = mapSize/2 + 1;
    newBody->y = mapSize/2;
    newBody->next = NULL;    
    newBody->pre = head;
    head->next = newBody;

    newBody = (snake*)malloc(sizeof(snake));
    newBody->x = mapSize/2 + 1;
    newBody->y = mapSize/2 + 1;
    newBody->next = NULL;
    newBody->pre = head->next;
    head->next->next = newBody;
    tail = newBody;

    snake* cur = head;;
    while(1){
        setCursorPos(cur->x,cur->y);
        printf("● ");
        cur = cur->next;
        if(cur == NULL)
            break;
        
    }

    while(1){
        char input = _getch();
        int nx = 0;
        int ny = 0;
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

        setCursorPos(tail->x, tail->y);
        printf("  ");

        //꼬리의 위치를 이동할 칸으로 변경
        tail->x = head->x + nx;
        tail->y = head->y + ny;
        //꼬리와 머리를 연결
        tail->next = head;
        head->pre = tail;
        //꼬리를 머리로 설정
        head = tail;
        tail = tail->pre;
        //전 꼬리와 현재 꼬리 연결 끊기
        head->pre = NULL;
        tail->next = NULL;
    
        setCursorPos(head->x, head->y);
        printf("● ");
        

    }


}


void setCursorPos(int x,int y){
    COORD pos;
    //문자열의 가로가 2칸이기 때문에 2를 곱함
    pos.X = x * 2;
    pos.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}