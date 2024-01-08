#include "header.h"

// 1: 潜水艦 (左から右)
void move_marineLR(Enemy* enemy) __z88dk_fastcall
{
    enemy->vx.value = 0x00C0;
    enemy->n8[0]++;

    // クリッピング
    switch (enemy->flag) {
        case 1:
            if (8 <= enemy->x.raw[1]) {
                VGS0_ADDR_OAM[SP_ENEMY + enemy->si + 2].attr = 0x80;
                enemy->flag++;
            }
            break;
        case 2:
            if (24 <= enemy->x.raw[1]) {
                VGS0_ADDR_OAM[SP_ENEMY + enemy->si + 1].attr = 0x80;
                enemy->flag++;
            }
            break;
        case 3:
            if (28 <= enemy->x.raw[1]) {
                VGS0_ADDR_OAM[SP_ENEMY + enemy->si].attr = 0x80;
                enemy->flag++;
            }
            break;
        case 4:
            if (enemy->x.raw[1] < 28) {
                enemy->flag++;
            }
            break;
        case 5:
            if (8 <= enemy->x.raw[1]) {
                VGS0_ADDR_OAM[SP_ENEMY + enemy->si + 2].attr = 0x00;
                enemy->flag++;
            }
            break;
        case 6:
            if (16 <= enemy->x.raw[1]) {
                enemy->flag = 0; // 画面アウト
            }
            break;
    }

    // プロペラを回転
    if (0 == (enemy->n8[0] & 3)) {
        enemy->n8[1]++;
        enemy->n8[1] &= 0x03;
        VGS0_ADDR_OAM[SP_ENEMY + enemy->si].ptn = 0x18 + enemy->n8[1];
    }
}
