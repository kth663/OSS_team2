#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include "timing.h"


int score;

void timing_game(){
  int key;
  clock_t start_time, end_time;
  double t;
  const double target = 10.0;
  
  while(1){
    
  printf("\n10초 타이밍 맞추기 게임을 시작하시겠습니까?\n");
  printf("엔터키를 눌러 계속하거나 ESC를 눌러 나가십시오.\n");
  /*기존 esc 안될 경우 q로 변경할수도?*/
  key = getch();

    if (key == 27){
      printf("게임 종료.");
      break;
    }
    else if (key != 13){
      printf("다른 키를 입력해주세요.");
      continue;
    }

  printf("\n엔터 키를 눌러 게임 시작하기\n");
  getchar();
  start_time = time(NULL);
  printf("10초 후 엔터를 누르세요.");

  getchar();
  end_time = time(NULL);

  t = difftime(end_time , start_time);
  printf("게임 결과: %.2f초\n", t);

  double gap = t - target;
  if (gap < 0)
    gap = -gap;

  if (gap <= 0.2){
    printf("성공! 800스코어 획득!\n");
    score = score + 800;
    printf("현재 스코어:%d\n", score);
  }
  else if (t > target){
    printf("%.2f초 느려요!\n", gap);
  }
  else {
    printf("%.2f초 빨라요!\n", gap);
  }
}
}

//문제점:스코어가 계속 초기화 된다 >> 나중에 전역변수 쓰면 괜찮을 듯.