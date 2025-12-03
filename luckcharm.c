// lucky_charm.c
#include "luckcharm.h"

static int luckyCharmCount = 0; // 내부에서만 관리

// 부적 개수 반환
int getLuckyCharmCount(void) {
    return luckyCharmCount;
}

// 부적 1개 사용
// 사용 가능하면 1 반환, 없으면 0 반환
int useLuckyCharm(void) {
    if (luckyCharmCount > 0) {
        luckyCharmCount--;
        return 1;
    }
    return 0;
}

// 부적 1개 추가
void addLuckyCharm(void) {
    luckyCharmCount++;
}

// 초기화
void initLuckyCharm(int count) {
    luckyCharmCount = count;
}
