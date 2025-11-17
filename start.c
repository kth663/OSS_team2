#include <stdio.h>
#include <windows.h>
#include "start.h"

void start(){
    int key;
  
  printf("press Enter to start\n");
  key = getchar();
  if (key == 27){
    return;
  }
  printf("Loading");
  fflush(stdout);

  for (int i = 0; i < 5; i++){
    Sleep(300);
    printf(".");
    fflush(stdout);
  }

  Sleep(500);
  system("cls");
}