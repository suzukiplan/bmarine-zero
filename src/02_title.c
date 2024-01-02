#include "header.h"

static const uint8_t titleBattle[32 * 5] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x84, 0x8D, 0x8E, 0x84, 0x8D, 0x88, 0x8c, 0x89, 0x88, 0x8c, 0x89, 0x87, 0x00, 0x00, 0x80, 0x84, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x85, 0x00, 0x85, 0x85, 0x00, 0x85, 0x00, 0x85, 0x00, 0x00, 0x85, 0x00, 0x85, 0x00, 0x00, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8A, 0x84, 0x8B, 0x8A, 0x84, 0x8B, 0x00, 0x85, 0x00, 0x00, 0x85, 0x00, 0x85, 0x00, 0x00, 0x8A, 0x84, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x85, 0x00, 0x85, 0x85, 0x00, 0x85, 0x00, 0x85, 0x00, 0x00, 0x85, 0x00, 0x85, 0x00, 0x00, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0x84, 0x83, 0x86, 0x00, 0x86, 0x00, 0x86, 0x00, 0x00, 0x86, 0x00, 0x81, 0x84, 0x89, 0x81, 0x84, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t titleMarine[32 * 5] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x8C, 0x82, 0x8E, 0x84, 0x8D, 0x80, 0x84, 0x8D, 0x88, 0x8c, 0x89, 0x80, 0x8D, 0x87, 0x80, 0x84, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x85, 0x85, 0x85, 0x85, 0x00, 0x85, 0x85, 0x00, 0x85, 0x00, 0x85, 0x00, 0x85, 0x85, 0x85, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x85, 0x85, 0x85, 0x8A, 0x84, 0x8B, 0x8A, 0x84, 0x90, 0x00, 0x85, 0x00, 0x85, 0x85, 0x85, 0x8A, 0x84, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x85, 0x85, 0x85, 0x85, 0x00, 0x85, 0x85, 0x00, 0x91, 0x00, 0x85, 0x00, 0x85, 0x85, 0x85, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x86, 0x86, 0x86, 0x86, 0x00, 0x86, 0x86, 0x00, 0x86, 0x88, 0x8F, 0x89, 0x86, 0x81, 0x83, 0x81, 0x84, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const int8_t rasterTable[32] = {
    0, 0, 1, 1, 2, 2, 3, 3,
    4, 4, 3, 3, 2, 2, 1, 1,
    0, 0, -1, -1, -2, -2, -3, -3,
    -4, -4, -3, -3, -2, -2, -1, -1
};

void title(void)
{
    // DPM を設定
    *VGS0_ADDR_BG_DPM = 4;
    *VGS0_ADDR_FG_DPM = 5;
    *VGS0_ADDR_SPRITE_DPM = 6;

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

    vgs0_fg_putstr(2, 2, 0x80, "SC        00    HI        00");
    vgs0_fg_putstr(11, 16, 0x80, "- KAISEN -");
    vgs0_fg_putstr(7, 20, 0x80, "PRESS START BUTTON");
    vgs0_fg_putstr(3, 23, 0x80, "@COPYRIGHT 2024 SUZUKIPLAN");

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
        uint8_t x = random[GV->ridx];
        GV->ridx++;
        uint8_t y = random[GV->ridx] % 192;
        GV->ridx++;
        vgs0_oam_set(i, x, y, 0x82, i);
    }

    // ループ
    uint8_t a = 0;
    uint8_t sidx = 0;
    uint8_t start = 0;
    while (1) {
        a++;
        if (0 == start) {
            // STARTボタンが押されたかチェック
            uint8_t pad = vgs0_joypad_get();
            if (pad & VGS0_JOYPAD_ST) {
                start = 1;
                for (i = 0; i < 32; i++) {
                    VGS0_ADDR_FG->attr[20][i] = 0x80;
                }
            }
        } else {
            start += 1;
            if (0 == start) {
                break;
            }
            if (start < 16) {
                for (i = 0; i < 32; i++) {
                    VGS0_ADDR_FG->ptn[i][start] = 0x00;
                    VGS0_ADDR_FG->ptn[i][31 - start] = 0x00;
                }
            }
        }

        // PRESS START BUTTON を点滅
        if (0 == start) {
            switch (a & 0x3F) {
                case 0:
                    for (i = 0; i < 32; i++) {
                        VGS0_ADDR_FG->attr[20][i] = 0x80;
                    }
                    break;
                case 24:
                    for (i = 0; i < 32; i++) {
                        VGS0_ADDR_FG->attr[20][i] = 0x81;
                    }
                    break;
                case 32:
                    for (i = 0; i < 32; i++) {
                        VGS0_ADDR_FG->attr[20][i] = 0x00;
                    }
                    break;
                case 56:
                    for (i = 0; i < 32; i++) {
                        VGS0_ADDR_FG->attr[20][i] = 0x81;
                    }
                    break;
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
                    uint8_t x = random[GV->ridx];
                    GV->ridx++;
                    uint8_t y = random[GV->ridx] % 192;
                    GV->ridx++;
                    vgs0_oam_set(i, x, y, 0x82, 0);
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
            vgs0_bg_putstr(2, 2, 0x80, "SC        00    HI        00");
            vgs0_oam_set(0x00, 116, 64 + 8, 0x80, 0x80);
            vgs0_oam_set(0x01, 124, 64 + 8, 0x80, 0x80);
            vgs0_oam_set(0x02, 132, 64 + 8, 0x80, 0x80);
            vgs0_oam_set(0x03, 116, 64, 0x80, 0x10);
            vgs0_oam_set(0x04, 124, 64, 0x80, 0x11);
            vgs0_oam_set(0x05, 132, 64, 0x80, 0x12);
            vgs0_oam_set(0x06, 116, 64 + 8, 0x80, 0x20);
            vgs0_oam_set(0x07, 124, 64 + 8, 0x80, 0x21);
            vgs0_oam_set(0x08, 132, 64 + 8, 0x80, 0x22);
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
