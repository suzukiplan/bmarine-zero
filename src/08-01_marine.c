#include "header.h"

// 1: 潜水艦 (左から右)
void move_marineLR(Enemy* enemy) __z88dk_fastcall
{
    if (1 == enemy->flag) {
        enemy->flag = 2;
        enemy->vx.value = get_random(&GV->ridx);
        enemy->vx.value += 0x0040;
    }
    enemy->n8[0]++;

    // 雷撃
    if (enemy->n8[2]) {
        enemy->n8[2]--;
        enemy->n8[3]++;
        switch (enemy->n8[3]) {
            case 1:
                VGS0_ADDR_OAM[enemy->si[1]].ptn = 0x90;
                VGS0_ADDR_OAM[enemy->si[2]].ptn = 0x91;
                break;
            case 3:
                VGS0_ADDR_OAM[enemy->si[1]].ptn = 0x93;
                VGS0_ADDR_OAM[enemy->si[2]].ptn = 0x94;
                break;
            case 5:
                VGS0_ADDR_OAM[enemy->si[1]].ptn = 0x96;
                VGS0_ADDR_OAM[enemy->si[2]].ptn = 0x97;
                break;
            case 9:
                VGS0_ADDR_OAM[enemy->si[1]].ptn = 0x99;
                VGS0_ADDR_OAM[enemy->si[2]].ptn = 0x9A;
                break;
            case 12:
                VGS0_ADDR_OAM[enemy->si[1]].ptn = 0x9C;
                VGS0_ADDR_OAM[enemy->si[2]].ptn = 0x9D;
                break;
            case 15:
                VGS0_ADDR_OAM[enemy->si[1]].ptn = 0x15;
                VGS0_ADDR_OAM[enemy->si[2]].ptn = 0x16;
                break;
        }
    } else {
        if (enemy->x.raw[1] - 16 < GV->player.x.raw[1] + 24 && GV->player.x.raw[1] < enemy->x.raw[1] - 8) {
            add_enemy(ET_THUNDER, enemy->x.raw[1] - 12, enemy->y.raw[1] - 4);
            enemy->n8[2] = 16;
            enemy->n8[3] = 0;
        }
    }

    // クリッピング
    switch (enemy->flag) {
        case 2:
            if (8 <= enemy->x.raw[1]) {
                VGS0_ADDR_OAM[enemy->si[2]].attr = 0x80;
                enemy->flag++;
            }
            break;
        case 3:
            if (24 <= enemy->x.raw[1]) {
                VGS0_ADDR_OAM[enemy->si[1]].attr = 0x80;
                enemy->check = 1;
                enemy->flag++;
            }
            break;
        case 4:
            if (28 <= enemy->x.raw[1]) {
                VGS0_ADDR_OAM[enemy->si[0]].attr = 0x80;
                enemy->flag++;
            }
            break;
        case 5:
            if (enemy->x.raw[1] < 28) {
                enemy->flag++;
            } else {
                if (0 == (enemy->n8[0] & 0x03)) {
                    add_spray(enemy->x.raw[1] - 28, enemy->y.raw[1] + 4, 0x38, 0x83);
                }
            }
            break;
        case 6:
            if (8 <= enemy->x.raw[1]) {
                VGS0_ADDR_OAM[enemy->si[2]].attr = 0x00;
                enemy->flag++;
            }
            break;
        case 7:
            if (16 <= enemy->x.raw[1]) {
                enemy->flag = 0; // 画面アウト
            }
            break;
    }

    // プロペラを回転
    if (0 == (enemy->n8[0] & 3)) {
        enemy->n8[1]++;
        enemy->n8[1] &= 0x03;
        VGS0_ADDR_OAM[enemy->si[0]].ptn = 0x18 + enemy->n8[1];
    }
}
