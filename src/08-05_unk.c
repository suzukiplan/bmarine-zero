#include "header.h"

// 5: 鳥の落とし物
void move_unk(Enemy* enemy) __z88dk_fastcall
{
    if (1 == enemy->flag) {
        vgs0_se_play(15);
        enemy->flag = 2;
        enemy->vy.value = 188;
        enemy->check = 1;
    }
    if (72 < enemy->y.raw[1]) {
        enemy->flag = 0;
        vgs0_se_play(5);
        add_spray(enemy->x.raw[1] - 2, 69, 0x30, 0xC3);
        add_spray(enemy->x.raw[1] + 2, 69, 0x30, 0x83);
    } else {
        enemy->n8[0]++;
        enemy->n8[0] &= 0x07;
        if (0 == enemy->n8[0]) {
            add_spray2(enemy->x.raw[1], enemy->y.raw[1], 0x70, 0x89);
        }
    }
}