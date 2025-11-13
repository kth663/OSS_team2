#include<stdio.h>

void snackGame(){



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