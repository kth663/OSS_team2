
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

#define T_SCREEN_HEIGHT 25
#define T_BOX_WIDTH     60

    static const char* BUNNY_LINES[] = {
    "토끼: 지금이다 싶을 때 ENTER!",
    "토끼: 10초를 몸으로 느껴봐요...",
    "토끼: 너무 서두르지는 말기!"
};

#define BUNNY_LINES_COUNT (sizeof(BUNNY_LINES)/ sizeof(BUNNY_LINES[0]))

// --------------------------------------------------
//  기본 헬퍼 (이 파일 안에서만 사용)
// --------------------------------------------------
static int t_get_console_width(void) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int width = 80;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    return width;
}

static void t_gotoxy(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

static void t_set_color(WORD attr) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attr);
}

static void t_draw_center_text(int y, const char* text) {
    int width = t_get_console_width();
    int len   = (int)strlen(text);
    int x     = (width - len) / 2;
    if (x < 0) x = 0;

    t_gotoxy(x, y);
    printf("%s", text);
}
static void t_draw_center_text_kor(int y, const char* text) {
    int width = t_get_console_width();

    int cols = 0;
    for (int i = 0; text[i] != '\0'; i++) {
        unsigned char c = (unsigned char)text[i];

        // CP949 기준: 0x80 이상이면 한글 등 2바이트 문자일 가능성 높음
        if (c & 0x80) {
            cols += 2;   // 화면에서 2칸 차지
            i++;         // 다음 바이트는 건너뛰기
        } else {
            cols += 1;   // 영문/숫자/기호는 1칸
        }
    }

    int x = (width - cols) / 2;
    if (x < 0) x = 0;

    t_gotoxy(x, y);
    printf("%s", text);
}

static void t_clear_line(int y) {
    int width = t_get_console_width();
    t_gotoxy(0, y);
    for (int i = 0; i < width; i++) putchar(' ');
}

// --------------------------------------------------
//  타이밍 게임 전용 박스 (폭 고정)
// --------------------------------------------------
static void t_draw_fixed_box(int top, int height) {
    int width = t_get_console_width();
    int left = (width - T_BOX_WIDTH) / 2;
    if (left < 0) left = 0;

    // 위
    t_gotoxy(left, top);
    putchar('+');
    for (int i = 0; i < T_BOX_WIDTH - 2; i++) putchar('-');
    putchar('+');

    // 중간
    for (int r = 1; r < height - 1; r++) {
        t_gotoxy(left, top + r);
        putchar('|');
        for (int i = 0; i < T_BOX_WIDTH - 2; i++) putchar(' ');
        putchar('|');
    }

    // 아래
    t_gotoxy(left, top + height - 1);
    putchar('+');
    for (int i = 0; i < T_BOX_WIDTH - 2; i++) putchar('-');
    putchar('+');
}

// lines[]를 박스 안에 왼쪽 정렬로 출력
static void t_draw_text_box(int top, const char* lines[], int n_lines) {
    int height = n_lines + 2;
    int width  = t_get_console_width();
    int left   = (width - T_BOX_WIDTH) / 2;
    if (left < 0) left = 0;

    t_draw_fixed_box(top, height);

    for (int i = 0; i < n_lines; i++) {
        t_gotoxy(left + 2, top + 1 + i);
        printf("%s", lines[i]);
    }
}

// --------------------------------------------------
//  점수 + 구분선
// --------------------------------------------------
static void t_draw_score_and_line(void) {
    char buf[64];
    int width = t_get_console_width();

    sprintf(buf, "SCORE: %d", score);
    int x = width - (int)strlen(buf) - 2;
    if (x < 0) x = 0;
    t_gotoxy(x, 0);
    printf("%s", buf);

    for (int i = 0; i < width; i++) {
        t_gotoxy(i, 1);
        putchar('-');
    }
}

// --------------------------------------------------
//  토끼 캐릭터 (프레임마다 깨끗이 지우고, 살짝만 점프)
// --------------------------------------------------
// ---------------------------------------------
//  토끼 + 말풍선 영역 지우기
// ---------------------------------------------
static void t_clear_bunny_area(int base_y) {
    int width = t_get_console_width();
    for (int r = -1; r <= 4; r++) {
        int y = base_y + r;
        if (y < 0) continue;
        t_gotoxy(0, y);
        for (int i = 0; i < width; i++) putchar(' ');
    }
}

// ---------------------------------------------
//  귀여운 말풍선 (꼬리 달린 구름 모양)
//  bunny_x : 토끼가 시작되는 x 좌표
//  base_y  : 토끼 위쪽 줄 기준
// ---------------------------------------------
static void t_draw_speech_bubble_cute(int bunny_x, int base_y, const char* msg) {
    int len = (int)strlen(msg);
    int inner = len + 2;          // 양쪽 여백
    int bubble_width = inner + 2; // 양쪽 따옴표/점

    int bubble_x = bunny_x - bubble_width - 6; // 토끼 왼쪽으로 약간 띄우기
    if (bubble_x < 0) bubble_x = 0;
    int bubble_y = base_y - 1;                // 토끼보다 한 줄 위에서 시작

    // 색 저장
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    WORD old_attr = csbi.wAttributes;

    // 말풍선 색 (밝은 하늘색 느낌)
    t_set_color(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    // 윗줄:  .---------
    t_gotoxy(bubble_x, bubble_y);
    putchar(' ');
    putchar('.');
    for (int i = 0; i < inner; i++) putchar('-');
    putchar('.');

    // 본문: (  메시지  )
    t_gotoxy(bubble_x, bubble_y + 1);
    putchar('(');
    putchar(' ');
    printf("%s", msg);
    putchar(' ');
    putchar(')');

    // 아랫줄:  '---------
    t_gotoxy(bubble_x, bubble_y + 2);
    putchar(' ');
    putchar('\'');
    for (int i = 0; i < inner; i++) putchar('-');
    putchar('\'');

    // 꼬리 (토끼 쪽으로 내려가는 작은 꼬리)
    int tail_x = bunny_x - 3;
    int tail_y = bubble_y + 3;
    t_gotoxy(tail_x, tail_y);
    printf("\\");
    t_gotoxy(tail_x + 1, tail_y + 1);
    printf("\\");

    // 색 복원
    t_set_color(old_attr);
}

// ---------------------------------------------
//  토끼 + 말풍선 통합 출력
// ---------------------------------------------
static void t_draw_bunny(int base_y, int frame) {
    const char* bunny[] = {
        " (\\_/)",
        " ( •_•)",
        " / >o "
    };

    int width   = t_get_console_width();
    int bunny_x = width - 18;   // 화면 오른쪽 근처

    // 이 줄들 전체를 싹 지움 (버니 + 말풍선 영역)
    t_clear_bunny_area(base_y);

    // 어떤 대사 쓸지 고르기 (프레임에 따라)
    int msg_index = (frame / 25) % BUNNY_LINES_COUNT; // 숫자 바꾸면 말 바뀌는 속도 조절
    const char* msg = BUNNY_LINES[msg_index];

    // 1) 말풍선 먼저 그리기
    t_draw_speech_bubble_cute(bunny_x, base_y, msg);

    // 2) 색 저장
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    WORD old_attr = csbi.wAttributes;

    // 토끼 색 (밝은 분홍 느낌)
    t_set_color(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    // 3) 토끼 그리기
    for (int r = 0; r < 3; r++) {
        t_gotoxy(bunny_x, base_y + r);
        printf("%s", bunny[r]);
    }

    // 색 복원
    t_set_color(old_attr);
}
// --------------------------------------------------
//  화면들
// --------------------------------------------------

// 1) 시작 메뉴
static int t_show_menu(void) {
    while (1) {
        system("cls");

        // 타이틀 강조 (색만 바꿈)
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        WORD old_attr = csbi.wAttributes;

        t_set_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        t_draw_center_text(4, "10초 맞추기 게임");
        t_set_color(old_attr);

        const char* opt1[] = { "[1] 게임 시작하기" };
        const char* opt2[] = { "[2] 게임 설명 보기" };
        const char* opt3[] = { "[ESC] 미로로 돌아가기" };

        t_draw_text_box(7,  opt1, 1);
        Sleep(120);
        t_draw_text_box(11, opt2, 1);
        Sleep(120);
        t_draw_text_box(15, opt3, 1);

        int ch = getch();
        if (ch == '1') return 1;
        if (ch == '2') return 2;
        if (ch == 27)  return 3;
    }
}

// 2) 게임 설명 (한 글자씩 출력, ESC 없음)
static void t_show_description(void) {
    system("cls");

    const char* lines[] = {
        "이 게임은 10초에 최대한 가깝게 시간을 맞추는 게임입니다.",
        "처음에는 시간이 화면에 보이다가,",
        "잠시 후 숫자가 사라집니다.",
        "10초쯤 되었다고 느껴질 때 ENTER를 누르세요.",
        "",
        "[ENTER] 게임 시작하기"
    };
    int n = sizeof(lines) / sizeof(lines[0]);

    const char* empty[6] = { "", "", "", "", "", "" };
    int top = 6;
    t_draw_text_box(top, empty, n);   // 틀 먼저

    int width = t_get_console_width();
    int left  = (width - T_BOX_WIDTH) / 2;
    if (left < 0) left = 0;

    // 한 글자씩 출력
    for (int i = 0; i < n; i++) {
        const char* s = lines[i];
        int y = top + 1 + i;
        t_gotoxy(left + 2, y);
        for (int j = 0; s[j] != '\0'; j++) {
            putchar(s[j]);
            Sleep(15);
        }
    }

    while (1) {
        int ch = getch();
        if (ch == 13) break;  // ENTER → 로딩으로
    }
}

// 3) 로딩 화면 (길이 고정 "Loading...")
static void t_show_loading(void) {
    system("cls");

    int hasItem = (item[1] != 0);
    const char* tip_no_item  = "TIP: 상점에서 아이템을 구매하면 숫자가 더 오래 보입니다.";
    const char* tip_has_item = "아이템 효과로 숫자가 더 오래 표시됩니다! (+200%)";
    const char* tip = hasItem ? tip_has_item : tip_no_item;

    int y_msg = T_SCREEN_HEIGHT / 2;     // Loading... 줄
    int y_tip = y_msg + 2;               // TIP 줄

    // 먼저 두 줄 비우고 TIP 가운데 정렬(한글 기준)
    t_clear_line(y_msg);
    t_clear_line(y_tip);
    t_draw_center_text_kor(y_tip, tip);

    // "Loading..." 애니메이션 (길이는 항상 같게 유지)
    for (int i = 0; i < 24; i++) {
        char buf[] = "Loading...   ";  // 뒤에 공백 넣어서 길이 고정
        int dotCount = i % 4;          // 0~3

        for (int d = 0; d < 3; d++) {
            buf[7 + d] = (d < dotCount) ? '.' : ' ';
        }

        t_clear_line(y_msg);
        // 여기서도 한글 버전 센터 함수 쓰지만,
        // buf는 전부 ASCII라 그냥 길이 기준으로 딱 맞게 들어감
        t_draw_center_text_kor(y_msg, buf);

        Sleep(150);
    }
}
// --------------------------------------------------
//  실제 게임
// --------------------------------------------------
void timing_game(void) {
    const double target = 10.0;
    srand((unsigned int)time(NULL));

    while (1) {
        // 메뉴
        int menu = t_show_menu();
        if (menu == 3) {   // ESC → 미로로
            system("cls");
            maze(1);
            return;
        }
        if (menu == 2) {   // 설명 보고 → 로딩
            t_show_description();
            t_show_loading();
        } else {           // 바로 시작
            t_show_loading();
        }

        // ----------------- 게임 진행 -----------------
        double visible_limit = (item[1] != 0) ? 5.4 : 2.7;

        system("cls");
        t_draw_score_and_line();
        t_draw_center_text(8, "READY?");
        Sleep(800);

        system("cls");
        t_draw_score_and_line();
        t_draw_center_text(8, "GO!");
        Sleep(500);

        system("cls");
        t_draw_score_and_line();

        clock_t start_time = clock();
        clock_t end_time;
        double  t;
        int     faded = 0;
        int     frame = 0;

        while (1) {
            clock_t now = clock();
            t = (double)(now - start_time) / CLOCKS_PER_SEC;

            if (t <= visible_limit) {
                char buf[64];
                sprintf(buf, "%.2f초", t);
                t_clear_line(10);
                t_draw_center_text(10, buf);
            } else if (!faded) {
                t_clear_line(10);
                t_draw_center_text(10, "멈추고 싶을 때 ENTER를 누르세요!");
                faded = 1;
            }

            t_draw_bunny(14, frame);
            

            frame++;

            if (_kbhit()) {
                int c = _getch();
                if (c == 13) {  // ENTER
                    end_time = clock();
                    break;
                } else if (c == 27) { // ESC → 미로
                    system("cls");
                    maze(1);
                    return;
                }
            }

            Sleep(50);
        }

        t = (double)(end_time - start_time) / CLOCKS_PER_SEC;

        // STOP 연출
        system("cls");
        t_draw_score_and_line();
        t_draw_center_text(8, "STOP!");
        t_draw_bunny(14, 0);
        Sleep(900);

        // ----------------- 결과 화면 -----------------
        system("cls");
        t_draw_score_and_line();

        char line_result[128];
        char line_detail[128];
        char line_score[128];

        sprintf(line_result, "결과: %.2f초", t);

        double gap = fabs(t - target);
        if (gap <= 0.2) {
            sprintf(line_detail, "대성공! 800점을 획득했습니다!");
            score += 800;
        } else if (t > target) {
            sprintf(line_detail, "%.2f초 느렸습니다.", gap);
        } else {
            sprintf(line_detail, "%.2f초 빨랐습니다.", gap);
        }
        sprintf(line_score, "현재 스코어: %d점", score);

        const char* box_lines[] = {
            "결과",
            line_result,
            line_detail,
            line_score,
            "",
            "[ENTER] 다시 하기   [ESC] 미로로 돌아가기"
        };
        int n_box = sizeof(box_lines) / sizeof(box_lines[0]);

        t_draw_text_box(6, box_lines, n_box);

        while (1) {
            int ch = _getch();
            if (ch == 27) {
                system("cls");
                maze(1);
                return;
            }
            if (ch == 13) break;
        }
    }
}