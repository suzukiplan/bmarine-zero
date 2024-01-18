#include "header.h"

#define nJumpWait enemy->n8[0]
#define nJumpCounter enemy->n8[1]
#define nJumpPower enemy->n8[2]
#define nKanitama enemy->n8[3]
#define nMoveDirection enemy->n16

// 7: カニ
void move_kani(Enemy* enemy) __z88dk_fastcall
{
    if (1 == enemy->flag) {
        enemy->check = 1;
        enemy->flag = 2;
        nJumpWait = get_random(&GV->ridx);
        nMoveDirection.value = 128;
        if (get_random(&GV->ridx) < 0x80) {
            nMoveDirection.value = -nMoveDirection.value;
        }
    }

    if (nJumpWait) {
        nJumpWait--;
        if (0 == nJumpWait) {
            vgs0_se_play(17);
            nJumpCounter = 30;
            nJumpPower = get_random(&GV->ridx) & 0x03;
            nJumpPower += get_random(&GV->ridx) & 0x01;
            nJumpPower += 3;
            nKanitama = 0;
            VGS0_ADDR_OAM[enemy->si[0]].ptn = 0x6E;
        }
        return;
    }

    if (nJumpCounter) {
        enemy->vx.value = nMoveDirection.value;
        enemy->vy.value -= nJumpPower;
        nJumpCounter--;
        return;
    }

    if (0 == nKanitama && 0 < (int16_t)enemy->vy.value) {
        nKanitama = 1;
        add_enemy(ET_KANITAMA, enemy->x.raw[1] + 4, enemy->y.raw[1] - 5);
    }

    if (enemy->y.raw[1] < 168) {
        enemy->vy.value += 3;
    } else {
        enemy->y.raw[1] = 168;
        enemy->y.raw[0] = 0;
        enemy->vy.value = 0;
        enemy->vx.value = 0;
        nJumpWait = 0x80 | get_random(&GV->ridx);
        VGS0_ADDR_OAM[enemy->si[0]].ptn = 0x6C;
    }
}
