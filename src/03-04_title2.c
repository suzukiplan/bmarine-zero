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

#ifdef STEAM
    vgs0_fg_putstr(9, 19, 0x80, "  GAME START");
    vgs0_fg_putstr(9, 20, SR->extra ? 0x80 : 0x81, "  EXTRA START");
    vgs0_fg_putstr(9, 21, 0x80, "  SCORE RANKING");
    vgs0_fg_putstr(9, 22, 0x80, "  EXIT");
#else
    vgs0_fg_putstr(9, 19, 0x80, "  GAME START");
    vgs0_fg_putstr(9, 21, SR->extra ? 0x80 : 0x81, "  EXTRA START");
    vgs0_fg_putstr(9, 23, 0x80, "  SCORE RANKING");
#endif
    VGS0_ADDR_FG->ptn[19 + (GV->menuCursor << 1)][9] = '>';

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
    uint8_t scrollX = 0;
    uint8_t page = 0;
    NameTable* scr = (NameTable*)0xA000;
    print_score_ranking(&scr[1]);
    print_rank_history(&scr[2]);
    vgs0_putstr(&scr[1], 6, 22, 0x80, "PUSH BUTTON TO NEXT");
    vgs0_putstr(&scr[2], 6, 22, 0x80, "PUSH BUTTON TO NEXT");
    while (1) {
        a++;
        if (0 == start && 0 == scrollX) {
            // ボタンが押されたかチェック
            pad = vgs0_joypad_get();
            if (0 != (pad & ANY_BUTTON) && 0 == (prevPad & ANY_BUTTON)) {
                if (2 == GV->menuCursor) {
                    vgs0_se_play(23);
                    scrollX = 1;
                    if (0 == page) {
                        page++;
                        vgs0_memcpy((uint16_t)&scr[0], (uint16_t)VGS0_ADDR_FG, sizeof(NameTable));
                    } else {
                        page++;
                        if (3 == page) {
                            page = 0;
                        }
                    }
                } else {
                    vgs0_bgm_fadeout();
                    vgs0_se_play(0);
                    start = 1;
                    for (i = 0; i < 32; i++) {
                        VGS0_ADDR_FG->attr[19][i] = 0x80;
                    }
                }
            } else if (0 == page) {
                uint8_t prevMenuCursor = GV->menuCursor;
                if (0 != (pad & VGS0_JOYPAD_UP) && 0 == (prevPad & VGS0_JOYPAD_UP)) {
                    vgs0_se_play(21);
                    if (GV->menuCursor) {
                        GV->menuCursor--;
                        if (!SR->extra && 1 == GV->menuCursor) {
                            GV->menuCursor--;
                        }
                    } else {
#ifdef STEAM
                        GV->menuCursor = 3;
#else
                        GV->menuCursor = 2;
#endif
                    }
                }
                if (0 != (pad & VGS0_JOYPAD_DW) && 0 == (prevPad & VGS0_JOYPAD_DW)) {
                    vgs0_se_play(21);
#ifdef STEAM
                    if (3 != GV->menuCursor) {
#else
                    if (2 != GV->menuCursor) {
#endif
                        GV->menuCursor++;
                        if (!SR->extra && 1 == GV->menuCursor) {
                            GV->menuCursor++;
                        }
                    } else {
                        GV->menuCursor = 0;
                    }
                }
                if (GV->menuCursor != prevMenuCursor) {
#ifdef STEAM
                    VGS0_ADDR_FG->attr[19 + prevMenuCursor][9] = 0x00;
                    VGS0_ADDR_FG->attr[19 + GV->menuCursor][9] = 0x80;
                    VGS0_ADDR_FG->ptn[19 + GV->menuCursor][9] = '>';
#else
                    VGS0_ADDR_FG->attr[19 + (prevMenuCursor << 1)][9] = 0x00;
                    VGS0_ADDR_FG->attr[19 + (GV->menuCursor << 1)][9] = 0x80;
                    VGS0_ADDR_FG->ptn[19 + (GV->menuCursor << 1)][9] = '>';
#endif
                }
            }
            prevPad = pad;
        } else if (0 < scrollX) {
            for (i = 3; i < 30; i++) {
                vgs0_memcpy((uint16_t)&VGS0_ADDR_FG->ptn[i][0], (uint16_t)&VGS0_ADDR_FG->ptn[i][1], 31);
                vgs0_memcpy((uint16_t)&VGS0_ADDR_FG->attr[i][0], (uint16_t)&VGS0_ADDR_FG->attr[i][1], 31);
                VGS0_ADDR_FG->ptn[i][31] = scr[page].ptn[i][scrollX];
                VGS0_ADDR_FG->attr[i][31] = scr[page].attr[i][scrollX];
            }
            scrollX++;
            scrollX &= 0x1F;
        } else {
            start += 1;
            if (56 == start) {
                GV->extra = GV->menuCursor;
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
            if (3 == GV->menuCursor) {
                vgs0_exit();
            }
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
