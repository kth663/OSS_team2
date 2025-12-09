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

#define BUNNY_LINES_COUNT (sizeof(BUNNY_LINES) / sizeof(BUNNY_LINES[0]))

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

// 한글 기준 가운데 정렬
static void t_draw_center_text_kor(int y, const char* text) {
    int width = t_get_console_width();

    int cols = 0;
    for (int i = 0; text[i] != '\0'; i++) {
        unsigned char c = (unsigned char)text[i];

        if (c & 0x80) {       // CP949 기준: 0x80 이상이면 2바이트 문자라고 보고 2칸
            cols += 2;
            i++;
        } else {
            cols += 1;
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

static void draw_bunny_static(int base_y) {
    const char* bunny[] = {
        " (\\_/)",
        " ( •_•)",
        " / >o "
    };

    int width = t_get_console_width();
    int bunny_x = width - 18;

    WORD old;
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        old = csbi.wAttributes;
    }

    t_set_color(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    for (int r = 0; r < 3; r++) {
        t_gotoxy(bunny_x, base_y + r);
        printf("%s", bunny[r]);
    }

    t_set_color(old);
}

// --------------------------------------------------
// 말풍선만 변경
// --------------------------------------------------
static void draw_bubble_only(int base_y, const char* msg) {
    int width   = t_get_console_width();
    int bunny_x = width - 18;      // 토끼 x (draw_bunny_static 와 동일 기준)

    // 말풍선 위치와 크기(고정 너비로 단순하게)
    int bubble_width = 28;         // 전체 가로 길이
    int bubble_x     = bunny_x - (bubble_width + 4); // 토끼 왼쪽에 배치
    if (bubble_x < 0) bubble_x = 0;
    int bubble_y     = base_y - 1; // 토끼 바로 위에서 시작

    // 먼저 말풍선 영역만 싹 지우기 (토끼 쪽 안 건드리도록)
    for (int r = 0; r < 4; r++) {
        t_gotoxy(bubble_x, bubble_y + r);
        for (int i = 0; i < bubble_width; i++) {
            putchar(' ');
        }
    }

    // 색 저장
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    WORD old_attr = csbi.wAttributes;

    // 말풍선 색 (연한 하늘색 느낌)
    t_set_color(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    // 윗줄:  .--------------------------.
    t_gotoxy(bubble_x, bubble_y);
    putchar(' ');
    putchar('.');
    for (int i = 0; i < bubble_width - 4; i++) putchar('-');
    putchar('.');

    // 본문: (  메시지 ...             )
    t_gotoxy(bubble_x, bubble_y + 1);
    putchar('(');
    putchar(' ');

    // 메시지 출력 (너무 길면 그냥 잘려서 오른쪽으로 흘러가도 됨)
    printf("%s", msg);

    // 남은 부분 공백 채우기
    int printed_len = (int)strlen(msg);
    int inner_width = bubble_width - 4; // 양쪽 괄호/공백 제외한 내부 폭 대충 맞추기
    if (printed_len < inner_width) {
        for (int i = 0; i < inner_width - printed_len; i++) {
            putchar(' ');
        }
    }

    putchar(' ');
    putchar(')');

    // 아랫줄:  '--------------------------'
    t_gotoxy(bubble_x, bubble_y + 2);
    putchar(' ');
    putchar('\'');
    for (int i = 0; i < bubble_width - 4; i++) putchar('-');
    putchar('\'');

    // 꼬리 (토끼 쪽으로 내려가는 작은 꼬리, 토끼에 안 겹치게 살짝만)
    int tail_x = bubble_x + bubble_width - 4;
    int tail_y = bubble_y + 3;
    t_gotoxy(tail_x,     tail_y);
    printf("\\");
    t_gotoxy(tail_x + 1, tail_y + 1);
    printf("\\");

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
    t_draw_text_box(top, empty, n);

    int width = t_get_console_width();
    int left  = (width - T_BOX_WIDTH) / 2;
    if (left < 0) left = 0;

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
        if (ch == 13) break;
    }
}

// 3) 로딩 화면 (깜빡임 최소화)
static void t_show_loading(void) {
    system("cls");

    int hasItem = (item[1] != 0);
    const char* tip_no_item  = "TIP: 상점에서 아이템을 구매하면 숫자가 더 오래 보입니다.";
    const char* tip_has_item = "아이템 효과로 숫자가 더 오래 표시됩니다! (+200%)";
    const char* tip = hasItem ? tip_has_item : tip_no_item;

    int width = t_get_console_width();
    int y_msg = T_SCREEN_HEIGHT / 2;
    int y_tip = y_msg + 2;

    t_clear_line(y_msg);
    t_clear_line(y_tip);
    t_draw_center_text_kor(y_tip, tip);

    const char* base = "Loading...";
    int baseLen = (int)strlen(base);
    int x = (width - baseLen) / 2;
    if (x < 0) x = 0;

    for (int i = 0; i < 24; i++) {
        char buf[16] = "Loading...   ";
        int dotCount = i % 4;

        for (int d = 0; d < 3; d++) {
            buf[7 + d] = (d < dotCount) ? '.' : ' ';
        }

        t_gotoxy(x, y_msg);   // ← 더 이상 줄 전체 clear 안 함
        printf("%s", buf);

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
        int menu = t_show_menu();
        if (menu == 3) {
            system("cls");
            maze(1);
            return;
        }
        if (menu == 2) {
            t_show_description();
            t_show_loading();
        } else {
            t_show_loading();
        }

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
        draw_bunny_static(14);

        // 말풍선 타이머
        int bubbleTimer = 0;
        int bubbleIndex = 0;

        while (1) {
            clock_t now = clock();
            t = (double)(now - start_time) / CLOCKS_PER_SEC;

            if (t <= visible_limit) {
                char buf[64];
                sprintf(buf, "%.2f초", t);

                // ★ 숫자 라인 전체를 매번 지우지 않고, 그냥 덮어쓰기만 함
                t_draw_center_text(10, buf);
            } else if (!faded) {
                t_clear_line(10);  // 숫자 사라지는 시점에 딱 한 번만 지우기
                t_draw_center_text(10, "멈추고 싶을 때 ENTER를 누르세요!");
                faded = 1;
            }

            
            if (bubbleTimer % 35 == 0) {
                bubbleIndex = rand() % BUNNY_LINES_COUNT;
                draw_bubble_only(14, BUNNY_LINES[bubbleIndex]);
            }
            bubbleTimer++;
            

            if (_kbhit()) {
                int c = _getch();
                if (c == 13) {
                    end_time = clock();
                    break;
                } else if (c == 27) {
                    system("cls");
                    maze(1);
                    return;
                }
            }

            Sleep(60);   // 50 → 60ms로 살짝 늘려서 깜빡임 체감도 조금 줄이기
        }

        t = (double)(end_time - start_time) / CLOCKS_PER_SEC;

        system("cls");
        t_draw_score_and_line();
        t_draw_center_text(8, "STOP!");
        draw_bunny_static(14);
        Sleep(900);

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
