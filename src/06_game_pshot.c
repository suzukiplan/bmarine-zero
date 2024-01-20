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

void add_pshot(uint8_t x, uint16_t y)
{
    if (GV->shot[GV->shotIndex].flag) {
        return;
    }
    vgs0_se_play(1);
    GV->shot[GV->shotIndex].flag = 1;
    GV->shot[GV->shotIndex].x = x;
    GV->shot[GV->shotIndex].y.value = y;
    GV->shot[GV->shotIndex].spd = 0;
    if (y < 0x4800) {
        GV->shot[GV->shotIndex].onair = 1;
    } else {
        GV->shot[GV->shotIndex].onair = 0;
    }
    uint8_t sn = GV->shotIndex;
    sn += SP_SHOT;
    vgs0_oam_set(sn, x, GV->shot[GV->shotIndex].y.raw[1], 0x80, 0x13, 0, 1);
    GV->shotIndex++;
    GV->shotIndex &= 0x07;
}

void move_pshot(void) __z88dk_fastcall
{
    uint8_t i, j;
    for (i = 0; i < 8; i++) {
        if (GV->shot[i].flag) {
            GV->shot[i].spd += 44;
            GV->shot[i].y.value += GV->shot[i].spd;
            j = i;
            j += SP_SHOT;
            if (176 < GV->shot[i].y.raw[1]) {
                vgs0_se_play(3);
                GV->hit = 0;
                GV->hkt = 0;
                GV->shot[i].flag = 0;
                VGS0_ADDR_OAM[j].attr = 0x00;
                add_dust_ground(GV->shot[i].x, GV->shot[i].y.raw[1] + 7);
                add_dust_ground(GV->shot[i].x, GV->shot[i].y.raw[1] + 7);
                add_dust_ground(GV->shot[i].x, GV->shot[i].y.raw[1] + 7);
                add_dust_ground(GV->shot[i].x, GV->shot[i].y.raw[1] + 7);
            } else {
                GV->shot[i].flag++;
                VGS0_ADDR_OAM[j].y = GV->shot[i].y.raw[1];
                if (0 == (GV->shot[i].flag & 0x03)) {
                    add_spray(GV->shot[i].x, GV->shot[i].y.raw[1], 0x40, 0x80);
                }
                // 空中ショットの場合は着水判定
                if (GV->shot[i].onair) {
                    GV->shot[i].onair += 1;
                    if (0x44 < GV->shot[i].y.raw[1]) {
                        vgs0_se_play(6);
                        // 対空時間に比例して衝撃の水しぶきの間隔を広げる
                        j = GV->shot[i].onair >> 1;
                        add_spray(GV->shot[i].x - 5 - j, 0x46, 0x30, 0xC3);
                        add_spray(GV->shot[i].x + 5 + j, 0x46, 0x30, 0x83);
                        // 滞空時間に比例してバウンド（水の抵抗値）を大きくする
                        GV->shot[i].spd = GV->shot[i].onair;
                        GV->shot[i].spd <<= 4;
                        GV->shot[i].spd = -GV->shot[i].spd;
                        GV->shot[i].onair = 0;
                    }
                }
            }
        }
    }
}