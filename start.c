#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include "start.h"
#include "maze.h"
#include "data.h"

#define LOGO_LINES 5

void gotoxy2(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

/* =========================
   THE LOOP 로고 내려오기
   ========================= */
void show_logo_drop(void) {
    const char* logo[LOGO_LINES] = {
        "  █████████  ███   ███  ███████          ███        █████      █████    ██████",
        "     ███     ███   ███  ███              ███      ███   ███  ███   ███  ███  ███",
        "     ███     █████████  ██████           ███      ███   ███  ███   ███  ██████",
        "     ███     ███   ███  ███              ███      ███   ███  ███   ███  ███",
        "     ███     ███   ███  ███████          ████████   █████      █████    ███"
    };

    int targetY = 4;                 // 최종 위치의 y 시작
    int startY  = -LOGO_LINES;       // 화면 밖 위에서 시작

    for (int y = startY; y <= targetY; y++) {
        system("cls");
        for (int i = 0; i < LOGO_LINES; i++) {
            int drawY = y + i;
            if (drawY < 0) continue;
            gotoxy2(2, drawY);
            printf("%s", logo[i]);
        }
        Sleep(60);
    }
}

/* ================
   로딩 바
   ================ */
void show_loading_bar(void) {
    int barLength = 30;

    for (int i = 0; i <= barLength; i++) {
        gotoxy2((SCREEN_WIDTH - (barLength + 10)) / 2, SCREEN_HEIGHT / 2 + 2);

        printf("[");
        for (int j = 0; j < i; j++) printf("■");
        for (int j = i; j < barLength; j++) printf("□");
        printf("] %d%%", (i * 100) / barLength);

        Sleep(80);
    }
}

/* ==========================
   PRESS ENTER 깜빡이기
   ========================== */
void blink_press_enter(void) {
    char *msg = "Press ENTER to Start";
    int msgX = (SCREEN_WIDTH - (int)strlen(msg)) / 2;

    while (1) {
        gotoxy2(msgX, SCREEN_HEIGHT / 2 + 5);
        printf("%s", msg);
        Sleep(500);

        gotoxy2(msgX, SCREEN_HEIGHT / 2 + 5);
        printf("%*s", (int)strlen(msg), "");
        Sleep(300);

        if (kbhit()) {
            int key = getch();
            if (key == 13) {      // ENTER
                system("cls");
                return;
            }
        }
    }
}

/* ===========
   공통 박스
   =========== */
void draw_box(int left, int top, int width, int height) {
    int right = left + width - 1;
    int bottom = top + height - 1;

    // 모서리
    gotoxy2(left, top);     printf("┌");
    gotoxy2(right, top);    printf("┐");
    gotoxy2(left, bottom);  printf("└");
    gotoxy2(right, bottom); printf("┘");

    // 가로선
    for (int x = left + 1; x < right; x++) {
        gotoxy2(x, top);    printf("─");
        gotoxy2(x, bottom); printf("─");
    }
    // 세로선
    for (int y = top + 1; y < bottom; y++) {
        gotoxy2(left, y);   printf("│");
        gotoxy2(right, y);  printf("│");
    }
}

/* =========================
   한 줄 천천히 출력 (편지용)
   ========================= */
void print_line_slow(const char* text, int x, int y, int delay_ms) {
    gotoxy2(x, y);
    for (int i = 0; text[i] != '\0'; i++) {
        putchar(text[i]);
        fflush(stdout);
        Sleep(delay_ms);
    }
}

/* =========================
   편지 + 선택지 화면
   ========================= */
int show_intro_letter_and_get_choice(void) {
    system("cls");

    int boxW = 70;
    int boxH = 16;
    int left = (SCREEN_WIDTH - boxW) / 2;
    int top  = 5;

    draw_box(left, top, boxW, boxH);

    const char* text[] = {
        "눈을 떠보니, 당신은 사방이 낯선 벽으로 둘러싸인",
        "어두운 미로에 갇혀 있었습니다.",
        "",
        "과연 이 미로에서 출구를 찾아",
        "무사히 탈출할 수 있을까요?"
    };
    int lines = 5;

    int textStartY = top + 2;
    int innerLeft  = left + 3;   // 왼쪽 정렬 + 여백 2칸 정도

    for (int i = 0; i < lines; i++) {
        print_line_slow(text[i], innerLeft, textStartY + i, 15);
        Sleep(120);
    }

    // 선택지
    const char* opt1 = "[1] 미로로 바로 출발하기";
    const char* opt2 = "[2] 게임 설명 보기";

    gotoxy2(left + 4, top + boxH - 4);
    printf("%s", opt1);
    gotoxy2(left + 4, top + boxH - 3);
    printf("%s", opt2);

    // 입력
    while (1) {
        int ch = getch();
        if (ch == '1') return 1;
        if (ch == '2') return 2;
    }
}

/* =====================
   게임 설명 (1 / 2)
   ===================== */
void show_game_description_page1(void) {
    system("cls");

    int boxW = 70;
    int boxH = 11;
    int left = (SCREEN_WIDTH - boxW) / 2;
    int top  = 6;

    draw_box(left, top, boxW, boxH);

    const char* lines[] = {
        "게임 설명 (1 / 2)",
        "",
        "- 미로는 W, A, S, D 키로 이동할 수 있습니다.",
        "- 미니게임을 클리어해서 스코어를 모아 보세요.",
        "- 모은 스코어로 상점에서 출구에 쓰일 비밀번호와",
        "  미니게임을 쉽게 클리어할 수 있는 아이템을 구매할 수 있습니다.",
        "",
        "[ENTER] 다음 페이지로"
    };
    int n = 8;

    for (int i = 0; i < n; i++) {
        // ❗ 기존 gotoxy2 + printf 대신 이걸로
        print_line_slow(lines[i], left + 3, top + 2 + i, 15);
    }

    // ENTER 대기
    while (1) {
        int ch = getch();
        if (ch == 13) break;
    }
}

/* =====================
   게임 설명 (2 / 2)
   ===================== */
void show_game_description_page2(void) {
    system("cls");

    int boxW = 70;
    int boxH = 20;
    int left = (SCREEN_WIDTH - boxW) / 2;
    int top  = 4;

    draw_box(left, top, boxW, boxH);

    const char* lines[] = {
        "게임 설명 (2 / 2) - 이벤트 칸 안내",
        "",
        "미로 속 특정 칸에는 알파벳 이니셜이 표시되어 있습니다.",
        "각 이니셜은 서로 다른 미니게임이나 상점을 의미합니다.",
        "",
        "예시)",
        " - s : 시작 지점",
        " - e : 출구",
        " - g : 벽 게임",
        " - l : 슬롯머신 게임",
        " - t : 10초 타이밍 맞추기 게임",
        " - r : 넌센스 게임",
        "",
        "[ENTER] 미로로 출발하기"
    };
    int n = 14;

    for (int i = 0; i < n; i++) {
        // 여기도 한 글자씩 출력
        print_line_slow(lines[i], left + 3, top + 2 + i, 15);
    }

    while (1) {
        int ch = getch();
        if (ch == 13) break;
    }
}

/* =====================
   메인 스타트 화면
   ===================== */
void start_screen(void) {
    system("cls");
    system("color 0E");      // 노란색

    show_logo_drop();        // 로고 내려오기
    show_loading_bar();      // 로딩바
    blink_press_enter();     // PRESS ENTER 깜빡이기

    int choice = show_intro_letter_and_get_choice();

    if (choice == 2) {
        show_game_description_page1();
        show_game_description_page2();
    }

    system("color 07");      // 미로 들어가기 전에 원래 색으로 복구

    x = 1;
    y = 1;
    maze();
}