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

uint8_t logo(void) __z88dk_fastcall
{
    // DPM を設定
    *VGS0_ADDR_BG_DPM = BANK_MAIN_BG;
    *VGS0_ADDR_FG_DPM = BANK_TITLE_FG;
    *VGS0_ADDR_SPRITE_DPM = BANK_BRAND1_SP;

    // ネームテーブル+OAMをクリア
    vgs0_memset(0x8000, 0x00, 0x1800);

    // SUZUKIPLAN ロゴをスプライトで表示
    vgs0_wait_vsync();
    vgs0_palette_set(0, 0, 0, 0, 6);
    vgs0_oam_set(250, 8, 80, 0x84, 0x00, 15, 5);
    vgs0_oam_set(251, 136, 80, 0x84, 0x60, 13, 5);

    // 光る SUZUKIPLAN ロゴのスプライトを準備
    uint8_t i = 0;
    for (i = 0; i < 16; i++) {
        vgs0_oam_set(i, 8 + (i << 3), 80, 0x0A, i, 0, 5);
        VGS0_ADDR_OAM[i].bank = BANK_BRAND2_SP;
    }
    for (i = 16; i < 30; i++) {
        vgs0_oam_set(i, 8 + (i << 3), 80, 0x0A, 0x60 + (i - 16), 0, 5);
        VGS0_ADDR_OAM[i].bank = BANK_BRAND2_SP;
    }

    uint8_t a = 0;
    uint8_t pad;
    while (a < 180) {
        // ボタンが押されたかチェック
        pad = vgs0_joypad_get();
        if (0 != (pad & ANY_BUTTON)) {
            vgs0_dma(BANK_PALETTE);
            vgs0_memcpy((uint16_t)VGS0_ADDR_PALETTE, (uint16_t)VGS0_ADDR_CHARACTER, 512);
            return 1;
        }

        vgs0_wait_vsync();

        if (60 <= a && a < 91) {
            i = a - 60;
            if (i < 30) {
                if (0 == i) {
                    vgs0_se_play(20);
                }
                VGS0_ADDR_OAM[i].attr = 0x8A;
            }
            if (0 < i) {
                VGS0_ADDR_OAM[i - 1].attr = 0x00;
            }
        }

        a++;
    }

    vgs0_dma(BANK_PALETTE);
    vgs0_memcpy((uint16_t)VGS0_ADDR_PALETTE, (uint16_t)VGS0_ADDR_CHARACTER, 512);
    return 0;
}
