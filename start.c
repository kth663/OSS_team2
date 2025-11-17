#include <stdio.h>
#include "start.h"

void start(){
    int key;
  
  printf("press Enter to start\n");
  key = getchar();
  if (key == 27){
    return;
  }
}