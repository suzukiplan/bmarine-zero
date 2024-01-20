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