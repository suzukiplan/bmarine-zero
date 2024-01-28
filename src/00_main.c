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

static void init_vram(void)
{
    vgs0_memset(0x8000, 0x00, 0x4000);
    vgs0_dma(BANK_PALETTE);
    vgs0_memcpy((uint16_t)VGS0_ADDR_PALETTE, (uint16_t)VGS0_ADDR_CHARACTER, 512);
}

void main(void)
{
    // スコアランキングをロード
    vgs0_load((uint16_t)SR, sizeof(ScoreRanking));
    if ('B' != SR->eyecatch[0] || 'M' != SR->eyecatch[1] || '#' != SR->eyecatch[2] || 'S' != SR->eyecatch[3] || 'C' != SR->eyecatch[4] || 'O' != SR->eyecatch[5] || 'R' != SR->eyecatch[6] || 'E' != SR->eyecatch[7]) {
        vgs0_memset((uint16_t)SR, 0x00, sizeof(ScoreRanking));
        SR->eyecatch[0] = 'B';
        SR->eyecatch[1] = 'M';
        SR->eyecatch[2] = '#';
        SR->eyecatch[3] = 'S';
        SR->eyecatch[4] = 'C';
        SR->eyecatch[5] = 'O';
        SR->eyecatch[6] = 'R';
        SR->eyecatch[7] = 'E';
    }

    // グローバル変数を初期化
    vgs0_memset((uint16_t)GV, 0x00, sizeof(GlobalVariables));

    while (1) {
        // タイトル画面へ遷移
        vgs0_memcpy((uint16_t)&GV->hi[0], (uint16_t)&SR->data[0].sc, 8);
        init_vram();
        vgs0_bank0_switch(BANK_PRG0_0);
        vgs0_bank1_switch(BANK_PRG0_1);
        vgs0_bank2_switch(BANK_PRG0_2);
        vgs0_bank3_switch(BANK_PRG0_3);
        if (GV->demoEnd) {
            submain(1);
        } else {
            submain(0);
        }

        // ゲームメインへ遷移
        vgs0_bank0_switch(BANK_PRG1_0);
        vgs0_bank1_switch(BANK_PRG1_1);
        vgs0_bank2_switch(BANK_PRG1_2);
        vgs0_bank3_switch(BANK_PRG1_3);
        submain(0);

        // デモでなければスコアエントリーへ遷移
        if (!GV->demoEnd) {
            init_vram();
            vgs0_bank0_switch(BANK_PRG0_0);
            vgs0_bank1_switch(BANK_PRG0_1);
            vgs0_bank2_switch(BANK_PRG0_2);
            vgs0_bank3_switch(BANK_PRG0_3);
            submain(2);
        }
    }
}