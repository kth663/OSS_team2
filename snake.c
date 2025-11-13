#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<locale.h>

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
    

}