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

static const uint8_t titleBattle[32 * 5] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x84, 0x8D, 0x8E, 0x84, 0x8D, 0x88, 0x8c, 0x89, 0x88, 0x8c, 0x89, 0x87, 0x00, 0x00, 0x80, 0x84, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x85, 0x00, 0x85, 0x85, 0x00, 0x85, 0x00, 0x85, 0x00, 0x00, 0x85, 0x00, 0x85, 0x00, 0x00, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8A, 0x84, 0x8B, 0x8A, 0x84, 0x8B, 0x00, 0x85, 0x00, 0x00, 0x85, 0x00, 0x85, 0x00, 0x00, 0x8A, 0x84, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x85, 0x00, 0x85, 0x85, 0x00, 0x85, 0x00, 0x85, 0x00, 0x00, 0x85, 0x00, 0x85, 0x00, 0x00, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0x84, 0x83, 0x86, 0x00, 0x86, 0x00, 0x86, 0x00, 0x00, 0x86, 0x00, 0x81, 0x84, 0x89, 0x81, 0x84, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static const uint8_t titleMarine[32 * 5] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x8C, 0x82, 0x8E, 0x84, 0x8D, 0x80, 0x84, 0x8D, 0x88, 0x8c, 0x89, 0x80, 0x8D, 0x87, 0x80, 0x84, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x85, 0x85, 0x85, 0x85, 0x00, 0x85, 0x85, 0x00, 0x85, 0x00, 0x85, 0x00, 0x85, 0x85, 0x85, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x85, 0x85, 0x85, 0x8A, 0x84, 0x8B, 0x8A, 0x84, 0x90, 0x00, 0x85, 0x00, 0x85, 0x85, 0x85, 0x8A, 0x84, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x85, 0x85, 0x85, 0x85, 0x00, 0x85, 0x85, 0x00, 0x91, 0x00, 0x85, 0x00, 0x85, 0x85, 0x85, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x86, 0x86, 0x86, 0x86, 0x00, 0x86, 0x86, 0x00, 0x86, 0x88, 0x8F, 0x89, 0x86, 0x81, 0x83, 0x81, 0x84, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static const int8_t rasterTable[32] = {
    0, 0, 1, 1, 2, 2, 3, 3,
    4, 4, 3, 3, 2, 2, 1, 1,
    0, 0, -1, -1, -2, -2, -3, -3,
    -4, -4, -3, -3, -2, -2, -1, -1};

static const int8_t rasterTable2[64] = {
    0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7,
    8, 8, 7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1,
    0, 0, -1, -1, -2, -2, -3, -3, -4, -4, -5, -5, -6, -6, -7, -7,
    -8, -8, -7, -7, -6, -6, -5, -5, -4, -4, -3, -3, -2, -2, -1, -1};

static void render_logo(uint8_t color)
{
    uint8_t i, j;
    uint8_t n;
    n = 0;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 32; j++) {
            if (titleBattle[n]) {
                VGS0_ADDR_FG->ptn[i + 5][j] = (titleBattle[n] & 0x1F) | color;
                VGS0_ADDR_FG->attr[i + 5][j] = 0x82;
            } else {
                VGS0_ADDR_FG->attr[i + 5][j] = 0x00;
            }
            n++;
        }
    }
    n = 0;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 32; j++) {
            if (titleMarine[n]) {
                VGS0_ADDR_FG->ptn[i + 10][j] = (titleMarine[n] & 0x1F) | color;
                VGS0_ADDR_FG->attr[i + 10][j] = 0x82;
            } else {
                VGS0_ADDR_FG->attr[i + 10][j] = 0x00;
            }
            n++;
        }
    }
}

static void render_logo2()
{
    uint8_t i, j;
    uint8_t n;
    n = 0;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 32; j++) {
            if (titleBattle[n]) {
                VGS0_ADDR_FG->ptn[i + 5][j] = titleBattle[n];
                VGS0_ADDR_FG->attr[i + 5][j] = 0x80;
            } else {
                VGS0_ADDR_FG->attr[i + 5][j] = 0x00;
            }
            n++;
        }
    }
    n = 0;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 32; j++) {
            if (titleMarine[n]) {
                VGS0_ADDR_FG->ptn[i + 10][j] = titleMarine[n];
                VGS0_ADDR_FG->attr[i + 10][j] = 0x81;
            } else {
                VGS0_ADDR_FG->attr[i + 10][j] = 0x00;
            }
            n++;
        }
    }
}

uint8_t title1(void) __z88dk_fastcall
{
    // DPM を設定
    *VGS0_ADDR_BG_DPM = BANK_MAIN_BG;
    *VGS0_ADDR_FG_DPM = BANK_TITLE_FG;
    *VGS0_ADDR_SPRITE_DPM = BANK_MAIN_SP;

    // ネームテーブル+OAMをクリア
    vgs0_memset(0x8000, 0x00, 0x1800);

    // BGを表示
    uint8_t i, j;
    uint8_t n = 0;
    for (i = 0; i < 32; i++) {
        for (j = 0; j < 32; j++) {
            VGS0_ADDR_BG->ptn[i][j] = 1;
            VGS0_ADDR_BG->attr[i][j] = 0x82;
            n++;
            n &= 3;
        }
    }

    // 泡を初期化
    for (i = 0; i < 16; i++) {
        uint8_t x = get_random(&GV->ridx);
        uint8_t y = get_random(&GV->ridx) % 192;
        vgs0_oam_set(i, x, y, 0x82, i, 0, 0);
    }

    // ループ
    uint8_t a = 0;
    uint8_t aa = 0;
    uint8_t sidx = 0;
    uint8_t logo = 0;
    uint8_t f60 = 0;
    uint8_t sec = 0;
    *VGS0_ADDR_FG_SCROLL_Y = (uint8_t)-24;
    while (65 != logo) {
        // STARTボタンが押されたかチェック
        uint8_t pad = vgs0_joypad_get();
        if (0 != (pad & VGS0_JOYPAD_ST)) {
            vgs0_dma(BANK_PALETTE);
            vgs0_memcpy((uint16_t)VGS0_ADDR_PALETTE, (uint16_t)VGS0_ADDR_CHARACTER, 512);
            return 1;
        }

        // 秒カウンタ
        f60++;
        if (60 == f60) {
            f60 = 0;
            sec++;
            if (sec == 12) {
                logo = 12;
            } else if (sec == 24) {
                logo = 32;
            }
        }
        a++;

        // スプライトの泡を動かす
        for (i = 0; i < 16; i++) {
            VGS0_ADDR_OAM[i].y -= 1;
            if (a & 1) {
                VGS0_ADDR_OAM[i].ptn += 1;
            }
            if (16 == VGS0_ADDR_OAM[i].ptn) {
                uint8_t x = get_random(&GV->ridx);
                uint8_t y = get_random(&GV->ridx) % 192;
                vgs0_oam_set(i, x, y, 0x82, 0, 0, 0);
            }
        }

        // V-SYNC待ち
        vgs0_wait_vsync();

        if (logo < 9 && 0 == (a & 0x0F)) {
            if (logo < 8) {
                render_logo(logo * 0x20);
            }
            logo++;
        } else if (9 == logo && 0 != *VGS0_ADDR_FG_SCROLL_Y) {
            if (a & 1) {
                (*VGS0_ADDR_FG_SCROLL_Y)++;
                if (0 == *VGS0_ADDR_FG_SCROLL_Y) {
                    logo++;
                    aa = 0;
                }
            }
        } else if (10 == logo) {
            aa++;
            if (aa == 30) {
                *VGS0_ADDR_FG_DPM = BANK_MAIN_FG;
                render_logo2();
                vgs0_fg_putstr(2, 2, 0x80, "SC         0    HI         0");
                score_print(VGS0_ADDR_FG);
                vgs0_fg_putstr(11, 16, 0x80, "- KAISEN -");
                vgs0_fg_putstr(7, 19, 0x80, "PRESS START BUTTON");
                vgs0_fg_putstr(4, 23, 0x80, "@2013-2024 BY SUZUKIPLAN");
                vgs0_fg_putstr(5, 22, 0x80, "PROGRAMMED BY Y.SUZUKI");
                aa = 0;
                logo++;
            }
        } else if (11 == logo) {
            aa++;
            switch (aa & 0x3F) {
                case 0:
                    for (i = 0; i < 32; i++) {
                        VGS0_ADDR_FG->attr[19][i] = 0x80;
                    }
                    break;
                case 24:
                    for (i = 0; i < 32; i++) {
                        VGS0_ADDR_FG->attr[19][i] = 0x81;
                    }
                    break;
                case 32:
                    for (i = 0; i < 32; i++) {
                        VGS0_ADDR_FG->attr[19][i] = 0x00;
                    }
                    break;
                case 56:
                    for (i = 0; i < 32; i++) {
                        VGS0_ADDR_FG->attr[19][i] = 0x81;
                    }
                    break;
            }
        } else if (12 <= logo && logo < 29) {
            for (i = 4; i < 32; i++) {
                VGS0_ADDR_FG->attr[i][logo - 12] = 0x00;
                VGS0_ADDR_FG->attr[i][31 - (logo - 12)] = 0x00;
            }
            logo++;
        } else if (29 == logo) {
            vgs0_fg_putstr(9, 5, 0x80, "SCORE RANKING");
            vgs0_fg_putstr(5, 8, 0x80, "#  NAM  LV  SCORE");
            vgs0_fg_putstr(5, 9, 0x80, "-  ---  --  ---------");
            vgs0_fg_putstr(5, 11, 0x80, "1  ZUN   8  999999990");
            vgs0_fg_putstr(5, 13, 0x80, "2  Y.S   8  114514190");
            vgs0_fg_putstr(5, 15, 0x80, "3  OGI   8    7655730");
            vgs0_fg_putstr(5, 17, 0x80, "4  MOO   8       5730");
            vgs0_fg_putstr(5, 19, 0x81, "5  ***  **   NO ENTRY");
            vgs0_fg_putstr(4, 22, 0x80, "   PRESS START BUTTON   ");
            logo++;
            aa = 0;
        } else if (30 == logo) {
            aa++;
            switch (aa & 0x3F) {
                case 0:
                    for (i = 0; i < 32; i++) {
                        VGS0_ADDR_FG->attr[22][i] = 0x80;
                    }
                    break;
                case 24:
                    for (i = 0; i < 32; i++) {
                        VGS0_ADDR_FG->attr[22][i] = 0x81;
                    }
                    break;
                case 32:
                    for (i = 0; i < 32; i++) {
                        VGS0_ADDR_FG->attr[22][i] = 0x00;
                    }
                    break;
                case 56:
                    for (i = 0; i < 32; i++) {
                        VGS0_ADDR_FG->attr[22][i] = 0x81;
                    }
                    break;
            }
        } else if (32 <= logo && logo < 49) {
            for (i = 0; i < 32; i++) {
                VGS0_ADDR_FG->attr[i][logo - 32] = 0x00;
                VGS0_ADDR_FG->attr[i][31 - (logo - 32)] = 0x00;
            }
            logo++;
        } else if (49 <= logo) {
            if (0 == (a & 0x03)) {
                uint8_t ptn = logo - 49;
                ptn |= 0xE0;
                for (i = 0; i < 32; i++) {
                    for (j = 0; j < 32; j++) {
                        VGS0_ADDR_FG->ptn[j][i] = ptn;
                        VGS0_ADDR_FG->attr[j][i] = 0x82;
                    }
                }
                logo++;
            }
        }

        // BGを縦スクロール
        if (a & 1) {
            *VGS0_ADDR_BG_SCROLL_Y += 1;
        }

        // BGをラスタースクロール
        for (i = 0; i < 192; i++) {
            while (i != *VGS0_ADDR_COUNT_V) {
                ;
            }
            *VGS0_ADDR_BG_SCROLL_X = rasterTable[(sidx + i) & 0x1F];
            if (logo < 2) {
                *VGS0_ADDR_FG_SCROLL_X = rasterTable2[(sidx + i) & 0x3F] * 2;
            } else if (logo < 4) {
                *VGS0_ADDR_FG_SCROLL_X = rasterTable2[(sidx + i) & 0x3F];
            } else if (logo < 8) {
                *VGS0_ADDR_FG_SCROLL_X = rasterTable[(sidx + i) & 0x1F];
            } else if (32 <= i && 13 <= logo && logo < 29) {
                if (i & 1) {
                    *VGS0_ADDR_FG_SCROLL_X = (logo - 13) << 3;
                } else {
                    *VGS0_ADDR_FG_SCROLL_X = 0 - ((logo - 13) << 3);
                }
            } else if (32 <= logo && logo < 39) {
                if (i & 1) {
                    *VGS0_ADDR_FG_SCROLL_X = (logo - 13) << 3;
                } else {
                    *VGS0_ADDR_FG_SCROLL_X = 0 - ((logo - 13) << 3);
                }
            } else {
                *VGS0_ADDR_FG_SCROLL_X = 0;
            }
        }
        sidx++;
    }
    return 0;
}
