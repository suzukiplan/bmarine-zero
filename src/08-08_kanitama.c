/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2024 Yoji Suzuki.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
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
