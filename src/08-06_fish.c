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

#define nAnimationCounter enemy->n8[0]
#define nPatternIndex enemy->n8[1]
#define nPreviousX enemy->n8[2]
#define nRiseX enemy->n8[3]
#define nRiseFlag enemy->n8[4]
#define nLandingY enemy->n8[5]
#define nSprayFlag enemy->n8[6]

// 6: 魚
void move_fish(Enemy* enemy) __z88dk_fastcall
{
    if (1 == enemy->flag) {
        enemy->check = 1;
        enemy->vx.raw[0] = get_random(&GV->ridx);
        enemy->vx.raw[0] += get_random(&GV->ridx) & 0x7F;
        enemy->vx.raw[0] += get_random(&GV->ridx) & 0x3F;
        enemy->vx.value += 64;
        nRiseX = get_random(&GV->ridx);
        nLandingY = get_random(&GV->ridx) & 0x3F;
        nLandingY += 72;
        if (get_random(&GV->ridx) & 0x80) {
            // 左 -> 右
            enemy->flag = 0x02;
        } else {
            // 右 -> 左
            enemy->flag = 0x82;
            VGS0_ADDR_OAM[enemy->si[0]].attr |= 0x40;
            enemy->vx.value = -((int16_t)enemy->vx.value);
        }
        nPreviousX = enemy->x.raw[1];
        // 当たり判定を大きくしておく
        enemy->hit.x = -4;
        enemy->hit.y = -4;
        enemy->hit.width = 24;
        enemy->hit.height = 24;
    }

    if (enemy->flag == 0x02) {
        if (enemy->x.raw[1] < nPreviousX) {
            enemy->flag = 0x03;
        }
    } else if (enemy->flag == 0x03) {
        if (248 < enemy->x.raw[1]) {
            enemy->flag = 0;
            return;
        } else if (0 == nRiseFlag && nRiseX < enemy->x.raw[1]) {
            nRiseFlag = 1;
        }
    } else {
        if (nPreviousX < enemy->x.raw[1]) {
            enemy->flag = 0;
            return;
        } else if (0 == nRiseFlag && enemy->x.raw[1] < nRiseX) {
            nRiseFlag = 1;
            // 当たり判定を小さくする
            enemy->hit.x = 6;
            enemy->hit.y = 6;
            enemy->hit.width = 4;
            enemy->hit.height = 4;
        }
    }
    nPreviousX = enemy->x.raw[1];

    if (nRiseFlag) {
        if (1 == nRiseFlag) {
            if (enemy->y.raw[1] < 64) {
                nRiseFlag++;
            } else if (0xFB != enemy->vy.raw[1]) {
                enemy->vy.value -= 22;
                if (0xFF <= enemy->vy.raw[1]) {
                    VGS0_ADDR_OAM[enemy->si[0]].ptn = 3 - (enemy->vy.raw[1] - 0xFF);
                    VGS0_ADDR_OAM[enemy->si[0]].ptn <<= 1;
                    VGS0_ADDR_OAM[enemy->si[0]].ptn |= 0x80;
                }
            }
            if (0 == nSprayFlag && enemy->y.raw[1] < 72) {
                nSprayFlag = 1;
                vgs0_se_play(5);
                add_spray(enemy->x.raw[1], 69, 0x30, 0xC3);
                add_spray(enemy->x.raw[1] + 8, 69, 0x30, 0x83);
            }
        } else if (2 == nRiseFlag) {
            if (1 == nSprayFlag && 72 < enemy->y.raw[1]) {
                nSprayFlag = 2;
                vgs0_se_play(5);
                add_spray(enemy->x.raw[1], 69, 0x30, 0xC3);
                add_spray(enemy->x.raw[1] + 8, 69, 0x30, 0x83);
            }
            if (nLandingY < enemy->y.raw[1]) {
                VGS0_ADDR_OAM[enemy->si[0]].ptn = 0x8E;
                nRiseFlag++;
                // 当たり判定を普通にする
                enemy->hit.x = 0;
                enemy->hit.y = 0;
                enemy->hit.width = 16;
                enemy->hit.height = 16;
            } else if (3 != enemy->vy.raw[1]) {
                if (0 == enemy->vy.raw[1]) {
                    VGS0_ADDR_OAM[enemy->si[0]].ptn = 0x88;
                } else if (1 == enemy->vy.raw[1]) {
                    VGS0_ADDR_OAM[enemy->si[0]].ptn = 0x8A;
                } else if (2 == enemy->vy.raw[1]) {
                    VGS0_ADDR_OAM[enemy->si[0]].ptn = 0x8C;
                }
                enemy->vy.value += 44;
            }
        } else {
            if (168 < enemy->y.raw[1]) {
                VGS0_ADDR_OAM[enemy->si[0]].ptn = 0x80;
                enemy->vy.value = 0;
            } else if (0 != enemy->vy.raw[1]) {
                enemy->vy.value -= 44;
            } else {
                VGS0_ADDR_OAM[enemy->si[0]].ptn = 0x80;
            }
        }
    }
}