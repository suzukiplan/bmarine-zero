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

void title2(void) __z88dk_fastcall
{
    vgs0_wait_vsync();

    // DPM を設定
    *VGS0_ADDR_BG_DPM = BANK_MAIN_BG;
    *VGS0_ADDR_FG_DPM = BANK_MAIN_FG;
    *VGS0_ADDR_SPRITE_DPM = BANK_MAIN_SP;
    *VGS0_ADDR_FG_SCROLL_X = 0;
    *VGS0_ADDR_FG_SCROLL_Y = 0;

    // ネームテーブル + OAMをクリア
    vgs0_memset(0x8000, 0x00, 0x1800);

    // FGにタイトル描画
    uint8_t i, j;
    uint8_t n;
    n = 0;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 32; j++) {
            VGS0_ADDR_FG->ptn[i + 5][j] = titleBattle[n];
            VGS0_ADDR_FG->attr[i + 5][j] = 0x80;
            n++;
        }
    }
    n = 0;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 32; j++) {
            VGS0_ADDR_FG->ptn[i + 10][j] = titleMarine[n];
            VGS0_ADDR_FG->attr[i + 10][j] = 0x81;
            n++;
        }
    }

    vgs0_fg_putstr(2, 2, 0x80, "SC         0    HI         0");
    score_print(VGS0_ADDR_FG);
    vgs0_fg_putstr(11, 16, 0x80, "- KAISEN -");

    vgs0_fg_putstr(9, 19, 0x80, "> GAME START");
    vgs0_fg_putstr(9, 21, SR->extra ? 0x80 : 0x81, "  EXTRA START");
    vgs0_fg_putstr(9, 23, 0x80, "  SCORE RANKING");

    // BGを表示
    n = 0;
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
    uint8_t sidx = 0;
    uint8_t start = 0;
    uint8_t pad = 0;
    uint8_t prevPad = 0xFF;
    uint8_t menuCursor = 0;
    while (1) {
        a++;
        if (0 == start) {
            // STARTボタンが押されたかチェック
            pad = vgs0_joypad_get();
            if (0 != (pad & VGS0_JOYPAD_ST) && 0 == (prevPad & VGS0_JOYPAD_ST)) {
                vgs0_bgm_fadeout();
                vgs0_se_play(0);
                start = 1;
                for (i = 0; i < 32; i++) {
                    VGS0_ADDR_FG->attr[19][i] = 0x80;
                }
            } else {
                uint8_t prevMenuCursor = menuCursor;
                if (0 != (pad & VGS0_JOYPAD_UP) && 0 == (prevPad & VGS0_JOYPAD_UP)) {
                    vgs0_se_play(21);
                    if (menuCursor) {
                        menuCursor--;
                        if (!SR->extra && 1 == menuCursor) {
                            menuCursor--;
                        }
                    } else {
                        menuCursor = 2;
                    }
                }
                if (0 != (pad & VGS0_JOYPAD_DW) && 0 == (prevPad & VGS0_JOYPAD_DW)) {
                    vgs0_se_play(21);
                    if (2 != menuCursor) {
                        menuCursor++;
                        if (!SR->extra && 1 == menuCursor) {
                            menuCursor++;
                        }
                    } else {
                        menuCursor = 0;
                    }
                }
                if (menuCursor != prevMenuCursor) {
                    VGS0_ADDR_FG->attr[19 + (prevMenuCursor << 1)][9] = 0x00;
                    VGS0_ADDR_FG->attr[19 + (menuCursor << 1)][9] = 0x80;
                    VGS0_ADDR_FG->ptn[19 + (menuCursor << 1)][9] = '>';
                }
            }
            prevPad = pad;
        } else {
            start += 1;
            if (56 == start) {
                GV->extra = menuCursor;
                break;
            }
            if (start < 16) {
                for (i = 0; i < 32; i++) {
                    VGS0_ADDR_FG->ptn[i][start] = 0x00;
                    VGS0_ADDR_FG->ptn[i][31 - start] = 0x00;
                }
            }
        }

        // スプライトの泡を動かす
        if (start < 40) {
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
        }

        // V-SYNC待ち
        vgs0_wait_vsync();

        // BGを縦スクロール
        if (start < 40) {
            if (a & 1) {
                *VGS0_ADDR_BG_SCROLL_Y += 1;
            }
        } else if (40 == start) {
            *VGS0_ADDR_BG_SCROLL_Y = 0;
        }

        // BGをラスタースクロール
        if (start < 40) {
            for (i = 0; i < 192; i++) {
                while (i != *VGS0_ADDR_COUNT_V) {
                    ;
                }
                *VGS0_ADDR_BG_SCROLL_X = rasterTable[(sidx + i) & 0x1F];
                if (start && start < 16) {
                    if (i & 1) {
                        *VGS0_ADDR_FG_SCROLL_X = start << 3;
                    } else {
                        *VGS0_ADDR_FG_SCROLL_X = 0 - (start << 3);
                    }
                }
            }
            sidx++;
        }

        // BGフェードアウト
        if (24 == start) {
            *VGS0_ADDR_FG_SCROLL_X = 0;
            *VGS0_ADDR_FG_SCROLL_Y = 0;
            for (i = 1; i < 25; i++) {
                for (j = 0; j < 32; j++) {
                    VGS0_ADDR_FG->ptn[i][j] = 0x60;
                    if (i & 1) {
                        if (j & 1) {
                            VGS0_ADDR_FG->attr[i][j] = 0x80 | 0x40;
                        } else {
                            VGS0_ADDR_FG->attr[i][j] = 0x80 | 0x40 | 0x20;
                        }
                    } else {
                        if (j & 1) {
                            VGS0_ADDR_FG->attr[i][j] = 0x80;
                        } else {
                            VGS0_ADDR_FG->attr[i][j] = 0x80 | 0x20;
                        }
                    }
                }
            }
        } else if (24 < start && start < 24 + 32) {
            for (i = 1; i < 25; i++) {
                for (j = 0; j < 32; j++) {
                    VGS0_ADDR_FG->ptn[i][j] += 1;
                }
            }
        }
        if (40 == start) {
            *VGS0_ADDR_BG_SCROLL_X = 0;
            *VGS0_ADDR_BG_SCROLL_Y = 0;
            for (i = 0; i < 32; i++) {
                if (23 == i) {
                    for (j = 0; j < 32; j++) {
                        VGS0_ADDR_BG->ptn[i][j] = 0x02 + (j & 1);
                        VGS0_ADDR_BG->attr[i][j] = 0x83;
                    }
                } else if (24 == i) {
                    for (j = 0; j < 32; j++) {
                        VGS0_ADDR_BG->ptn[i][j] = 0x12 + (j & 1);
                        VGS0_ADDR_BG->attr[i][j] = 0x83;
                    }
                } else if (8 < i) {
                    for (j = 0; j < 32; j++) {
                        VGS0_ADDR_BG->ptn[i][j] = 0x10;
                        VGS0_ADDR_BG->attr[i][j] = 0x80;
                    }
                } else {
                    for (j = 0; j < 32; j++) {
                        VGS0_ADDR_BG->ptn[i][j] = 0x00;
                    }
                }
                VGS0_ADDR_OAM[i].attr = 0x00;
            }
            vgs0_bg_putstr(2, 2, 0x80, "SC         0    HI         0");
            vgs0_memset((uint16_t)GV->sc, 0, 8);
            score_print(VGS0_ADDR_BG);
            GV->player.x.value = 0x7400;
            GV->player.y.value = 0x4000;
            vgs0_oam_set(0x00, GV->player.x.raw[1], GV->player.y.raw[1] + 8, 0x80, 0x80, 0, 0);
            vgs0_oam_set(0x01, GV->player.x.raw[1] + 8, GV->player.y.raw[1] + 8, 0x80, 0x80, 0, 0);
            vgs0_oam_set(0x02, GV->player.x.raw[1] + 16, GV->player.y.raw[1] + 8, 0x80, 0x80, 0, 0);
            vgs0_oam_set(0x03, GV->player.x.raw[1], GV->player.y.raw[1], 0x80, 0x10, 0, 0);
            vgs0_oam_set(0x04, GV->player.x.raw[1] + 8, GV->player.y.raw[1], 0x80, 0x11, 0, 0);
            vgs0_oam_set(0x05, GV->player.x.raw[1] + 16, GV->player.y.raw[1], 0x80, 0x12, 0, 0);
            vgs0_oam_set(0x06, GV->player.x.raw[1], GV->player.y.raw[1] + 8, 0x80, 0x20, 0, 0);
            vgs0_oam_set(0x07, GV->player.x.raw[1] + 8, GV->player.y.raw[1] + 8, 0x80, 0x21, 0, 0);
            vgs0_oam_set(0x08, GV->player.x.raw[1] + 16, GV->player.y.raw[1] + 8, 0x80, 0x22, 0, 0);
        }
        if (40 <= start) {
            j = a;
            j &= 0x1F;
            j >>= 2;
            j |= 0x80;
            for (i = 1; i < 31; i++) {
                VGS0_ADDR_BG->ptn[9][i] = j;
            }
            VGS0_ADDR_OAM[0].ptn = j;
            VGS0_ADDR_OAM[1].ptn = j;
            VGS0_ADDR_OAM[2].ptn = j;
        }
    }
}
