#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include "start.h"

void start(){
    int key;
  
  printf("press Enter to Start\n");
  key = getch();
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