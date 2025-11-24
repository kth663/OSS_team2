#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<locale.h>
#include<conio.h>
#include<time.h>


int score = 0;

//뱀의 몸 구조체
typedef struct snake{
    int x;
    int y;

    struct snake* next; 
    struct snake* pre;
}snake;

void setCursorPos(int,int);
void pushPos(int [], int [],int,int,int);
void popPos(int [], int [],int,int,int);
void spawnApple(int*, int*, int*, int [], int [], int);


void snakeGame(){
    
    // ■와 ●가 ??로 출력되는 문제 때문에 설정
    setlocale(LC_CTYPE, "ko_KR.UTF-8");
    //명령어 실행후 메시지가 나오지 않게 함
    system("chcp 65001 > nul");


    system("cls");
    printf("Level 1 : 6 * 6\n");
    printf("Level 2 : 8 * 8\n");
    printf("Level 3 : 10 * 10\n");
    printf("Enter level -> ");
    int level = 1;
    int mapSize = 5;
    int speed = 500;
    //레벨 입력후 조정
    scanf_s("%d", &level);
    if(level == 1){
        mapSize = 6;
        speed = 500;
    }
    else if(level == 2){
        mapSize = 8;
        speed = 400;
    }
    else if(level == 3){
        mapSize = 10;
        speed = 300;
    }
    system("cls");
    


    srand(time(0));
    
    int applePosX[200];
    int applePosY[200];
    int size = 0;
    

    int checkMap[12][12];
    mapSize += 2;

    int startTime = time(0);

    //맵 생성
    for(int i = 0;i<mapSize;i++){
        for(int j = 0;j<mapSize;j++){
            if(i == 0 || j == 0 || i == mapSize-1 || j == mapSize-1){
                checkMap[j][i] = 1;
                printf("■ ");
            }
            else{
                checkMap[j][i] = 0;
                printf("  ");
                applePosX[size] = j;
                applePosY[size] = i;
                size++;
            }
        }
        printf("\n");
    }
    
    //뱀 생성(몸통 3개)
    int snakeX[3] = {mapSize/2, mapSize/2 + 1, mapSize/2 + 1};
    int snakeY[3] = {mapSize/2, mapSize/2, mapSize/2 + 1};
    snake* head;
    snake* tail;
    snake* newBody;
    snake* pre = NULL;

    for(int i = 0;i<3;i++){
        newBody = (snake*)malloc(sizeof(snake));
        newBody->x = snakeX[i];
        newBody->y = snakeY[i];
        newBody->next = NULL;
        newBody->pre = pre;
        if(i == 0){
            head = newBody;
        }
        else if(i == 2){
            pre->next = newBody;
            tail = newBody;
        }
        else{
            pre->next = newBody;
        }
        pre = newBody;
        popPos(applePosX, applePosY, newBody->x, newBody->y, size);
        size--;
        checkMap[newBody->x][newBody->y] = 1;
        setCursorPos(newBody->x, newBody->y);
        printf("● ");

    }
    


    
    int appleIdx;
    int appleX;
    int appleY;

    //가장 초기에 사과 하나 생성
    spawnApple(&appleIdx, &appleX, &appleY, applePosX, applePosY, size);

    int dirX[4] = {0,0,-1,1};
    int dirY[4] = {-1,1,0,0};

    int nx = dirX[0];
    int ny = dirY[0];

    int isClear = 0;

    while(1){
        //입력 받았을 때 방향 전환
        if(kbhit()){
            char input = _getch();
            int i = -1;
            if(input == 'w'){
                i = 0;
            }
            else if(input == 's'){
                i = 1;
            }
            else if(input == 'a'){
                i = 2;
            }
            else if(input == 'd'){
                i = 3;
            }

            if(i != -1){
                nx = dirX[i];
                ny = dirY[i];
            }
            
        }
        //외곽 부분 충돌시 종료
        if(head->x + nx == 0 || head->x + nx == mapSize-1 || head->y + ny == 0 || head->y + ny == mapSize-1){
            break;
        }
        //자기 몸통과 충돌시 종료
        if(checkMap[head->x + nx][head->y + ny] == 1){
            break;
        }
        //사과 먹었을 때
        if(head->x + nx == appleX && head->y + ny == appleY){
            //머리 추가
            newBody = (snake*)malloc(sizeof(snake));
            newBody->x = head->x + nx;
            newBody->y = head->y + ny;
            newBody->next = head;
            head->pre = newBody;
            head = newBody;
            
            setCursorPos(head->x, head->y);
            printf("● ");

            popPos(applePosX, applePosY, head->x, head->y, size);
            size--;
            checkMap[head->x][head->y] = 1;

            //모든 칸이 채워지면 클리어
            if(size == 0){
                isClear = 1;
                break;
            }

            spawnApple(&appleIdx, &appleX, &appleY, applePosX, applePosY, size);
        }
        else{
            
            
            setCursorPos(tail->x, tail->y);
            printf("  ");
            pushPos(applePosX, applePosY, tail->x, tail->y, size);
            size++;
            checkMap[tail->x][tail->y] = 0;

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
            popPos(applePosX, applePosY, head->x, head->y, size);
            size--;
            checkMap[head->x][head->y] = 1;
        }

        //speed에 따라 속도 조절
        Sleep(speed);

    }

    int endTime = time(0);
    system("cls");
    

    //클리어시 추가 스코어 부여
    int clearScore = 0;
    if(isClear == 1){
        if (level == 1)
        {
            clearScore= 500;
        }
        else if(level == 2){
            clearScore= 1200;
        }
        else if(level == 3){
            clearScore= 2000;
        }
    }
    //시간에 따라서 스코어 부여
    int timeScore = endTime - startTime;
    //이번 게임에서 얻은 총 스코어
    int totalScore = clearScore + timeScore;
    
    //스코어 출력
    if(isClear == 1){
        printf("Game Clear!\n");
        printf("Clear Score : %d\n",clearScore);
        printf("Time Score : %d\n",timeScore);
        printf("GetScore : %d + %d = %d\n",clearScore,timeScore,totalScore);

    }
    else{
        printf("Game Over\n");
        printf("GetScore : %d\n",totalScore);

    }
    
    //게임에서 얻은 스코어를 전체 스코어에 반영
    score += totalScore;
    printf("Score : %d\n",score);

    printf("\n\nr : restart\nq : quit");

    //입력에 따라 재시작 또는 종료
    while(1){
        char c = _getch();
        if(c == 'q')
            break;
        if(c == 'r'){
            snakeGame();
            break;
        }
    }

    



}


//커서의 위치를 조절하는 함수
void setCursorPos(int x,int y){
    COORD pos;
    //문자열의 가로가 2칸이기 때문에 2를 곱함
    pos.X = x * 2;
    pos.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

//사과가 생길 수 있는 위치를 저장하는 배열에 추가
void pushPos(int ax[], int ay[],int x,int y,int size){
    ax[size] = x;
    ay[size] = y;
}

//사과가 생길 수 있는 위치를 저장하는 배열에서 제거
void popPos(int ax[], int ay[],int x,int y,int size){
    for(int i = 0;i<size;i++){
        if(ax[i] == x && ay[i] == y){
            for(int j = i+1;j<=size;j++){
                ax[j-1] = ax[j];
                ay[j-1] = ay[j];
            }
            break;
        }
    }
}

//사과 생성 함수
void spawnApple(int* appleIdx, int* appleX, int* appleY, int applePosX[], int applePosY[], int size){
    *appleIdx = rand() % size;
    *appleX = applePosX[*appleIdx];
    *appleY = applePosY[*appleIdx];
    setCursorPos(*appleX, *appleY);
    printf("○ ");
}
