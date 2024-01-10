#include "header.h"

// 3: 雷撃
void move_thunder(Enemy* enemy) __z88dk_fastcall
{
    enemy->check = 1;
    enemy->n8[0]++;
    enemy->n8[0] &= 3;
    if (0 == enemy->n8[0]) {
        enemy->n8[1] = 1 - enemy->n8[1];
        VGS0_ADDR_OAM[enemy->si[0]].ptn = 0x1C + enemy->n8[1];
    }
    if (-900 < (int16_t)enemy->vy.value) {
        enemy->vy.value -= 7;
    }
    if (192 < enemy->y.raw[1]) {
        enemy->flag = 0;
    }
}
