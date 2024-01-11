#include "header.h"

// 3: 雷撃
void move_thunder(Enemy* enemy) __z88dk_fastcall
{
    if (enemy->flag == 1) {
        enemy->flag = 2;
        if (get_random(&GV->ridx) < 0x20) {
            enemy->vx.value = get_random(&GV->ridx);
            if (get_random(&GV->ridx) < 0x80) {
                enemy->vx.value = -enemy->vx.value;
            } else {
                VGS0_ADDR_OAM[enemy->si[0]].attr |= 0x40;
            }
            enemy->n8[2] = 0x1E;
            vgs0_se_play(9);
        } else {
            enemy->n8[2] = 0x1C;
            vgs0_se_play(8);
        }
    }
    enemy->check = 1;
    enemy->n8[0]++;
    enemy->n8[0] &= 3;
    if (0 == enemy->n8[0]) {
        enemy->n8[1] = 1 - enemy->n8[1];
        VGS0_ADDR_OAM[enemy->si[0]].ptn = enemy->n8[2] + enemy->n8[1];
    }
    if (-900 < (int16_t)enemy->vy.value) {
        enemy->vy.value -= 7;
    }
    if (192 < enemy->y.raw[1] || 248 <= enemy->x.raw[1]) {
        enemy->flag = 0;
    }
}
