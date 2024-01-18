#include "header.h"

#define nAngle enemy->n8[0]
#define nCheckWait enemy->n8[1]
#define nCheckCount enemy->n8[2]
#define nAnime enemy->n8[3]
#define nPtn enemy->n8[4]

// 8: カニタマ
void move_kanitama(Enemy* enemy) __z88dk_fastcall
{
    if (248 < enemy->x.raw[1] || 200 < enemy->y.raw[1]) {
        enemy->flag = 0;
        return;
    }

    if (1 == enemy->flag) {
        enemy->flag = 2;
        enemy->check = 1;
        nAngle = 128;
        vgs0_se_play(18);
    }

    nAnime++;
    nAnime &= 0x03;
    if (0 == nAnime) {
        nPtn = 1 - nPtn;
        VGS0_ADDR_OAM[enemy->si[0]].ptn = 0x2D + nPtn;
        if (nPtn) {
            add_spray2(enemy->x.raw[1], enemy->y.raw[1], 0xF0, 0x88);
        }
    }

    if (0 == nCheckWait) {
        nCheckWait = 3;
        nCheckCount++;
        uint8_t angle = vgs0_angle(enemy->x.raw[1] + 4, enemy->y.raw[1] + 4, GV->player.x.raw[1] + 12, GV->player.y.raw[1] + 8);
        if (nAngle < angle) {
            nAngle++;
        } else if (angle < nAngle) {
            nAngle--;
        }
    } else {
        nCheckWait--;
    }

    enemy->vx.value = vgs0_sin(nAngle);
    enemy->vy.value = vgs0_cos(nAngle);
}
