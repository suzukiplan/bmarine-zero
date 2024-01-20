#include "header.h"

static const uint8_t ptn[10] = {0x00, 0x03, 0x06, 0x09, 0x0C, 0x30, 0x33, 0x36, 0x39, 0x3C};

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
        if (0 == enemy->check) {
            enemy->check = 1;
            // 画面上部の爆発の当たり判定は小さくする
            if (enemy->y.raw[1] < 100) {
                enemy->hit.x = 10;
                enemy->hit.y = 10;
                enemy->hit.width = 4;
                enemy->hit.height = 4;
            }
        }
        enemy->n8[0]++;
        if (enemy->n8[0] < 10) {
            VGS0_ADDR_OAM[enemy->si[0]].ptn = ptn[enemy->n8[0]];
            // 特定パターン以降の当たり判定を削除
            if (0x38 < ptn[enemy->n8[0]]) {
                enemy->check = 0;
            }
        } else {
            enemy->flag = 0;
            // レベル8以降は打ち返し弾を発射
            if (8 <= GV->level) {
                add_catk(enemy->x.raw[1] + 8, enemy->y.raw[1] + 8);
            }
        }
    }
}