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

void main(void)
{
    // パレット初期化
    vgs0_dma(BANK_PALETTE);
    vgs0_memcpy((uint16_t)VGS0_ADDR_PALETTE, (uint16_t)VGS0_ADDR_CHARACTER, 512);

    for (uint8_t i = 0; i < 8; i++) {
        GV->hi[i] = 0;
        GV->sc[i] = 0;
    }
    GV->hi[0] = 3;
    GV->hi[1] = 7;
    GV->hi[2] = 5;

    while (1) {
        // タイトル画面へ遷移
        vgs0_bank0_switch(BANK_PRG0_0);
        vgs0_bank1_switch(BANK_PRG0_1);
        vgs0_bank2_switch(BANK_PRG0_2);
        vgs0_bank3_switch(BANK_PRG0_3);
        submain(0);

        // ゲームメインへ遷移
        vgs0_bank0_switch(BANK_PRG1_0);
        vgs0_bank1_switch(BANK_PRG1_1);
        vgs0_bank2_switch(BANK_PRG1_2);
        vgs0_bank3_switch(BANK_PRG1_3);
        submain(0);

        // VRAM クリア
        vgs0_memset(0x8000, 0x00, 0x4000);
        vgs0_dma(BANK_PALETTE);
        vgs0_memcpy((uint16_t)VGS0_ADDR_PALETTE, (uint16_t)VGS0_ADDR_CHARACTER, 512);
    }
}