#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <math.h>

#include "timing.h"
#include "maze.h"
#include "data.h"

#define SCREEN_H 25

// 스타듀식 대화창 설정
#define DLG_H 7
#define DLG_MARGIN 3
#define PORTRAIT_W 18   // 초상화 박스 폭
#define PORTRAIT_H 5    // 초상화 내부 높이

static const char* BUNNY_LINES[] = {
    "지금이다 싶을 때 ENTER!",
    "10초를 몸으로 느껴봐요...",
    "너무 서두르지는 말기!",
    "숫자 사라져도 괜찮아!",
    "딱 10초면 800점이에요!"
};
#define BUNNY_LINES_COUNT (sizeof(BUNNY_LINES)/sizeof(BUNNY_LINES[0]))

// --------------------------------------------------
// 콘솔 유틸
// --------------------------------------------------
static int cw(void){
    CONSOLE_SCREEN_BUFFER_INFO c;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &c);
    return c.srWindow.Right - c.srWindow.Left + 1;
}
static void go(int x,int y){
    COORD p={(SHORT)x,(SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),p);
}
static void clear_area(int x,int y,int w,int h){
    for(int r=0;r<h;r++){
        go(x,y+r);
        for(int i=0;i<w;i++) putchar(' ');
    }
}

// 한글 폭(2칸 문자로 가정) + 안전 출력용(1줄)
static void put_fit_1line(const char* msg, int inner_cols){
    int used=0;
    for(int i=0; msg[i]; i++){
        unsigned char c=(unsigned char)msg[i];
        int cw2 = (c & 0x80) ? 2 : 1;
        if(used + cw2 > inner_cols) break;

        putchar(msg[i]);
        if(c & 0x80){
            i++;
            if(msg[i]) putchar(msg[i]);
        }
        used += cw2;
    }
    for(int i=used;i<inner_cols;i++) putchar(' ');
}

// 아주 단순 2줄 래핑(1줄 넘치면 나머지 2줄로)
static void wrap_2lines(const char* msg, int inner_cols, char* out1, int out1sz, char* out2, int out2sz){
    out1[0]=0; out2[0]=0;

    int used=0, bi=0;
    int i=0;

    // out1 채우기
    for(i=0; msg[i] && bi<out1sz-1; i++){
        unsigned char c=(unsigned char)msg[i];
        int cw2 = (c & 0x80) ? 2 : 1;
        if(used + cw2 > inner_cols) break;

        out1[bi++] = msg[i];
        if(c & 0x80 && msg[i+1] && bi<out1sz-1){
            out1[bi++] = msg[i+1];
            i++;
        }
        used += cw2;
    }
    out1[bi]=0;

    // rest 시작점(바이트 인덱스 기준)
    const char* rest = msg + bi;
    while(*rest==' ') rest++;

    // out2 채우기
    used=0; bi=0;
    for(i=0; rest[i] && bi<out2sz-1; i++){
        unsigned char c=(unsigned char)rest[i];
        int cw2 = (c & 0x80) ? 2 : 1;
        if(used + cw2 > inner_cols) break;

        out2[bi++] = rest[i];
        if(c & 0x80 && rest[i+1] && bi<out2sz-1){
            out2[bi++] = rest[i+1];
            i++;
        }
        used += cw2;
    }
    out2[bi]=0;
}

// --------------------------------------------------
// 프레임 / 박스
// --------------------------------------------------
static void frame(const char* title){
    int w=cw();
    go(1,1); printf("╔"); for(int i=0;i<w-4;i++) printf("═"); printf("╗");
    for(int y=2;y<SCREEN_H-1;y++){
        go(1,y); printf("║");
        go(w-2,y); printf("║");
    }
    go(1,SCREEN_H-1); printf("╚"); for(int i=0;i<w-4;i++) printf("═"); printf("╝");

    go(3,2); printf("%s", title);
    go(w-20,2); printf("Score: %d", score);

    go(1,3); printf("╟"); for(int i=0;i<w-4;i++) printf("─"); printf("╢");
}

static void box(int x,int y,int w,int h,const char* t){
    go(x,y); printf("┌"); for(int i=0;i<w-2;i++) printf("─"); printf("┐");
    for(int r=1;r<h-1;r++){
        go(x,y+r); printf("│");
        go(x+w-1,y+r); printf("│");
    }
    go(x,y+h-1); printf("└"); for(int i=0;i<w-2;i++) printf("─"); printf("┘");
    if(t){ go(x+2,y); printf("%s", t); }
}

static void print_center_in_box(int x,int y,int w,const char* s){
    int len=(int)strlen(s);
    int px = x + (w - len)/2;
    if(px < x+1) px = x+1;
    go(px,y); printf("%s", s);
}

// --------------------------------------------------
// 스타듀식 대화창(초상화 + 이름 + 대사)
//  - 프레임은 1번만 그림
//  - 갱신은 "텍스트 영역만" clear 후 출력 (테두리 안 지워짐)
// --------------------------------------------------
static void draw_bunny_portrait(int x,int y){
    const char* p[PORTRAIT_H] = {
        "   (\\_/)        ",
        "  ( o.o )       ",
        "   > ^ <        ",
        "   (___)        ",
        "    \" \"         "
    };
    for(int i=0;i<PORTRAIT_H;i++){
        go(x, y+i);
        printf("%s", p[i]);
    }
}

static void dialogue_draw_once(int x,int y,int w,int h){
    // 바깥 대화창
    box(x,y,w,h,NULL);

    // 좌측 초상화 박스
    int px = x+2;
    int py = y+1;
    int pw = PORTRAIT_W;
    int ph = h-2;

    box(px, py, pw, ph, NULL);

    // 초상화 출력(테두리 안쪽)
    draw_bunny_portrait(px+1, py+1);
}

static void dialogue_say_update(int x,int y,int w,int h, const char* name, const char* msg){
    // 초상화 박스 우측 텍스트 영역
    int px = x+2;
    int pw = PORTRAIT_W;

    int tx = px + pw + 2;             // 텍스트 시작 x
    int tw = (x+w-2) - tx;            // 텍스트 영역 폭(테두리 제외)
    if(tw < 10) return;

    int inner_cols = tw - 1;          // 안전폭
    int nameY = y+1;
    int lineY1 = y+2;
    int lineY2 = y+3;

    // ✅ 텍스트 영역만 지우기(테두리/초상화 건드리지 않음)
    // y+1 ~ y+h-2 내부 줄들 중, tx ~ x+w-3 구간만 clear
    for(int r=1; r<=h-2; r++){
        go(tx, y+r);
        for(int i=0;i<tw;i++) putchar(' ');
    }

    // 이름
    go(tx, nameY);
    printf("%s", name);

    // 메시지 2줄 래핑
    char line1[256], line2[256];
    wrap_2lines(msg, inner_cols, line1, sizeof(line1), line2, sizeof(line2));

    go(tx, lineY1);
    put_fit_1line(line1, inner_cols);

    go(tx, lineY2);
    put_fit_1line(line2, inner_cols);
}

// --------------------------------------------------
// 메뉴 / 설명
// --------------------------------------------------
static int timing_menu(void){
    system("cls");
    frame("Timing Game");

    int w=cw();
    box(6,7,w-12,8,"MENU");
    go(10,10); printf("1. 게임 시작");
    go(10,11); printf("2. 게임 설명");
    go(10,12); printf("ESC. 미로로 돌아가기");

    while(1){
        int ch=_getch();
        if(ch=='1') return 1;
        if(ch=='2') return 2;
        if(ch==27)  return 0;
    }
}

static void description(void){
    system("cls");
    frame("Timing Game - HOW TO PLAY");

    const char* lines[]={
        "이 게임은 10초에 최대한 가깝게 시간을 맞추는 게임입니다.",
        "처음에는 시간이 보이다가 잠시 후 사라집니다.",
        "10초쯤 되었다고 느껴질 때 ENTER를 누르세요.",
        "정확히 맞히면 800점을 획득합니다.",
        "",
        "ENTER를 누르면 메뉴로 돌아갑니다."
    };

    int y=6;
    for(int i=0;i<6;i++){
        go(6,y+i);
        for(int j=0; lines[i][j]; j++){
            putchar(lines[i][j]);
            Sleep(18);
        }
    }
    while(_getch()!=13);
}

// --------------------------------------------------
// 실제 게임 (초상화 1개 + 대화창 이름 '버니' + 테두리 고정)
// --------------------------------------------------
void timing_game(void){
    const double target=10.0;
    srand((unsigned)time(NULL));

    while(1){
        int sel = timing_menu();
        if(sel==0){ maze(1); return; }
        if(sel==2){ description(); continue; }

        system("cls");
        frame("Timing Game");

        int w = cw();
        int margin = DLG_MARGIN;

        // 대화창(하단 고정)
        int dlgX = margin;
        int dlgW = w - margin*2;
        int dlgY = SCREEN_H - DLG_H;
        int dlgH = DLG_H;

        // 위쪽 게임 영역
        int topY = 5;
        int usableH = dlgY - topY - 1;

        // 왼쪽 게임 영역은 "전체 폭"으로 사용(토끼 박스 삭제)
        int gameX = margin;
        int gameW = w - margin*2;

        // Info/Timer 높이
        int infoH  = 5;
        int timerH = usableH - infoH - 1;
        if(timerH < 7) timerH = 7;

        int yInfo  = topY;
        int yTimer = yInfo + infoH + 1;

        // 박스 그리기
        box(gameX, yInfo,  gameW, infoH,  "Info");
        box(gameX, yTimer, gameW, timerH, "Timer");

        // 대화창 프레임/초상화는 1번만
        dialogue_draw_once(dlgX, dlgY, dlgW, dlgH);

        // Info 내용
        double visible = item[1] ? 5.4 : 2.7;
        go(gameX+2, yInfo+2); printf("ITEM   : Timing Watch");
        go(gameX+2, yInfo+3); printf("Status : %s | Visible: %.1fs", item[1] ? "ON" : "OFF", visible);

        // 시작 대사
        dialogue_say_update(dlgX, dlgY, dlgW, dlgH, "버니", "준비됐나요? 딱 10초에 ENTER!");

        // READY / GO (시간 늘림 유지)
        clear_area(gameX+1, yTimer+1, gameW-2, timerH-2);
        print_center_in_box(gameX, yTimer + timerH/2, gameW, "READY?");
        Sleep(1200);

        clear_area(gameX+1, yTimer+1, gameW-2, timerH-2);
        print_center_in_box(gameX, yTimer + timerH/2, gameW, "GO!");
        Sleep(800);
        Sleep(300);
        clear_area(gameX+1, yTimer+1, gameW-2, timerH-2);

        // 게임 루프
        clock_t st=clock(), et;
        int faded=0;
        int tick=0;

        while(1){
            double t = (double)(clock()-st)/CLOCKS_PER_SEC;

            // Timer 중앙 표시
            if(t <= visible){
                char buf[32];
                sprintf(buf, "%.2f sec", t);
                go(gameX + (gameW/2) - 8, yTimer + timerH/2);
                printf("%-16s", buf);
            }else{
                if(!faded){
                    dialogue_say_update(dlgX, dlgY, dlgW, dlgH, "버니",
                        "이제 숫자는 안 보여요. 감각으로 10초를 느껴봐요!");
                    faded=1;
                }
                go(gameX + (gameW/2) - 8, yTimer + timerH/2);
                printf("%-16s", "ENTER!");
            }

            // NPC 주기 멘트(너무 잦지 않게)
            if(tick % 80 == 0){
                const char* m = BUNNY_LINES[rand()%BUNNY_LINES_COUNT];
                dialogue_say_update(dlgX, dlgY, dlgW, dlgH, "버니", m);
            }
            tick++;

            if(_kbhit()){
                int c=_getch();
                if(c==13){ et=clock(); break; }
                if(c==27){ maze(1); return; }
            }
            Sleep(60);
        }

        // 결과 화면
        double t = (double)(et-st)/CLOCKS_PER_SEC;
        double gap = fabs(t-target);

        system("cls");
        frame("Result");

        box(6,7,w-12,8,"RESULT");
        go(10,10); printf("TIME : %.2f sec", t);
        go(10,11); printf("GAP  : %.2f sec", gap);

        if(gap<=0.2){
            go(10,13); printf("PERFECT! +800");
            score += 800;
        }else if(t>target){
            go(10,13); printf("TOO LATE");
        }else{
            go(10,13); printf("TOO EARLY");
        }

        go(10,15); printf("ENTER : retry   ESC : back");

        while(1){
            int ch=_getch();
            if(ch==13) break;
            if(ch==27){ maze(1); return; }
        }
    }
}
