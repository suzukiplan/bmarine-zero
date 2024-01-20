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
    if (0 == enemy->n8[3] && enemy->y.raw[1] < 69) {
        vgs0_se_play(5);
        enemy->n8[3] = 1;
        add_spray(enemy->x.raw[1] - 3, 69, 0x30, 0xC3);
        add_spray(enemy->x.raw[1] + 3, 69, 0x30, 0x83);
    }
    enemy->check = 1;
    enemy->n8[0]++;
    enemy->n8[0] &= 3;
    if (0 == enemy->n8[0]) {
        enemy->n8[1] = 1 - enemy->n8[1];
        VGS0_ADDR_OAM[enemy->si[0]].ptn = enemy->n8[2] + enemy->n8[1];
    }
    if (enemy->n8[3]) {
        if (-1800 < (int16_t)enemy->vy.value) {
            enemy->vy.value -= 22;
        }
    } else {
        if (-900 < (int16_t)enemy->vy.value) {
            enemy->vy.value -= 7;
        }
    }
    if (192 < enemy->y.raw[1] || 248 <= enemy->x.raw[1]) {
        enemy->flag = 0;
    }
}
