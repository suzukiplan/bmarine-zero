#include "header.h"

static const uint8_t ptn[10] = {0xA0, 0xA3, 0xA6, 0xA9, 0xAC, 0xD0, 0xD3, 0xD6, 0xD9, 0xDC};

// 0: 爆発
void move_bomber(Enemy* enemy) __z88dk_fastcall
{
    if (1 == enemy->flag) {
        vgs0_se_play(7);
        int8_t vx = (int8_t)get_random(&GV->ridx);
        int8_t vy = (int8_t)get_random(&GV->ridx);
        enemy->vx.value = vx;
        enemy->vy.value = vy;
    }
    if (0 == (enemy->flag & 3)) {
        add_dust_air(enemy->x.raw[1] + 8, enemy->y.raw[1] + 8);
    }
    enemy->flag++;
    if (0 == (enemy->flag & 3)) {
        enemy->check = 1;
        enemy->n8[0]++;
        if (enemy->n8[0] < 10) {
            VGS0_ADDR_OAM[SP_ENEMY + enemy->si].ptn = ptn[enemy->n8[0]];
        } else {
            enemy->flag = 0;
        }
    }
}