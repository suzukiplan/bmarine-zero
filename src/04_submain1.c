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

int submain(uint8_t arg) __z88dk_fastcall
{
    uint8_t i, j;

    // スコアと波をBGからFGに描き直す
    vgs0_fg_putstr(2, 2, 0x80, "SC         0    HI         0");
    score_print(VGS0_ADDR_FG);
    for (i = 0; i < 32; i++) {
        VGS0_ADDR_BG->ptn[2][i] = 0x00;  // スコア(BG)をクリア
        VGS0_ADDR_BG->ptn[9][i] = 0x00;  // 波をクリア
        VGS0_ADDR_FG->ptn[9][i] = 0xA2;  // 波を描画
        VGS0_ADDR_FG->attr[9][i] = 0x80; // 波を描画
    }

    // ハイスコア以外のグローバル変数を初期化
    uint8_t demo = GV->demo;
    uint8_t extra = GV->extra;
    uint8_t menuCursor = GV->menuCursor;
    vgs0_memset(0xC000 + 8, 0x00, sizeof(GlobalVariables));
    GV->player.x.value = 0x7400;
    GV->player.y.value = 0x4000;
    GV->player.hp = 80;
    GV->player.chp = 0;
    GV->demo = demo;
    GV->extra = extra;
    GV->menuCursor = menuCursor;
    GV->tail[0] = 'T';
    GV->tail[1] = 'A';
    GV->tail[2] = 'I';
    GV->tail[3] = 'L';

    // OAMを初期化
    vgs0_memset((uint16_t)VGS0_ADDR_OAM, 0x00, sizeof(OAM) * 256);
    vgs0_oam_set(SP_JIKI, GV->player.x.raw[1], GV->player.y.raw[1], 0x80, 0x10, 2, 1);
    vgs0_oam_set(SP_JIKI + 1, GV->player.x.raw[1] + 8, GV->player.y.raw[1], 0x80, 0x2F, 0, 0);
    VGS0_ADDR_OAM[SP_LASER].widthMinus1 = 1;
    VGS0_ADDR_OAM[SP_LASER].bank = BANK_LASER_SP;
    VGS0_ADDR_OAM[SP_LTOP].widthMinus1 = 3;
    VGS0_ADDR_OAM[SP_LTOP].heightMinus1 = 1;
    VGS0_ADDR_OAM[SP_LTOP].bank = BANK_LASER2_SP;
    VGS0_ADDR_OAM[SP_LBOTTOM].y = 177;
    VGS0_ADDR_OAM[SP_LBOTTOM].widthMinus1 = 3;
    VGS0_ADDR_OAM[SP_LBOTTOM].heightMinus1 = 0;
    VGS0_ADDR_OAM[SP_LBOTTOM].bank = BANK_LASER2_SP;

    // 体力ゲージの初期描画
    for (i = 0; i < 5; i++) {
        VGS0_ADDR_FG->ptn[3][2 + i] = i + 1;
        VGS0_ADDR_FG->ptn[4][2 + i] = (i + 1) | 0x10;
        VGS0_ADDR_FG->attr[3][2 + i] = 0x87;
        VGS0_ADDR_FG->attr[4][2 + i] = 0x87;
        VGS0_ADDR_OAM[SP_HP + i].x = 16 + (i << 3);
        VGS0_ADDR_OAM[SP_HP + i].y = 32;
        VGS0_ADDR_OAM[SP_HP + i].bank = BANK_MEDAL_SP;
    }
    render_hp();

    if (0 == GV->demo) {
        vgs0_bgm_play(1);
    } else {
        vgs0_fg_putstr(9, 14, 0x80, "DEMONSTRATION!");
        vgs0_fg_putstr(PUSH_MSG_X, 17, 0x80, PUSH_MSG);
    }

    // メインループ
    uint8_t paused = 0;
    uint8_t pauseCursor = 0;
    uint8_t pad = 0;
    uint8_t prevPad = 0;
    uint8_t needRestart = 0;
    while (255 != GV->player.dead) {
        vgs0_wait_vsync();
        prevPad = pad;
        pad = vgs0_joypad_get();
        GV->frame++;

        // ポーズ
        if (0 == demo && 0 == paused && 0 != (pad & VGS0_JOYPAD_ST)) {
            vgs0_fg_putstr(11, 16, 0x80, "  PAUSE  ");
            vgs0_fg_putstr(11, 18, 0x80, " >RESUME ");
            vgs0_fg_putstr(11, 19, 0x80, "  RESTART");
            VGS0_ADDR_OAM[SP_PAUSE].x = 12 * 8;
            VGS0_ADDR_OAM[SP_PAUSE].y = 15 * 8;
            VGS0_ADDR_OAM[SP_PAUSE].ptn = 0x20;
            VGS0_ADDR_OAM[SP_PAUSE].attr = 0x89;
            VGS0_ADDR_OAM[SP_PAUSE].bank = BANK_CATK;
            VGS0_ADDR_OAM[SP_PAUSE].widthMinus1 = 9;
            VGS0_ADDR_OAM[SP_PAUSE].heightMinus1 = 4;
            vgs0_se_play(26);
            paused = 1;
            pauseCursor = 0;
        } else if (1 == paused) {
            if (0 == (pad & VGS0_JOYPAD_ST)) {
                paused = 2;
            }
        } else if (2 == paused) {
            if (0 != (pad & VGS0_JOYPAD_ST) || 0 != (pad & VGS0_JOYPAD_T1) || 0 != (pad & VGS0_JOYPAD_T2)) {
                vgs0_fg_putstr(11, 16, 0x80, "         ");
                vgs0_fg_putstr(11, 18, 0x80, "         ");
                vgs0_fg_putstr(11, 19, 0x80, "         ");
                VGS0_ADDR_OAM[SP_PAUSE].attr = 0x00;
                paused = 3;
            } else if ((0 == (prevPad & VGS0_JOYPAD_UP) && 0 != (pad & VGS0_JOYPAD_UP)) || (0 == (prevPad & VGS0_JOYPAD_DW) && 0 != (pad & VGS0_JOYPAD_DW))) {
                VGS0_ADDR_FG->ptn[18 + pauseCursor][12] = ' ';
                pauseCursor = 1 - pauseCursor;
                VGS0_ADDR_FG->ptn[18 + pauseCursor][12] = '>';
                vgs0_se_play(21);
            }
        } else if (3 == paused) {
            if (0 == pad) {
                if (0 == pauseCursor) {
                    vgs0_se_play(24);
                    paused = 0;
                } else {
                    vgs0_bgm_fadeout();
                    vgs0_se_play(0);
                    needRestart = 1;
                    break;
                }
            }
        }
        if (paused) {
            GV->frame--;
            continue;
        }

        // デモプレイ中
        if (GV->demo) {
            if (pad & ANY_BUTTON) {
                GV->demoEnd = 1;
                return 0;
            }
            if (GV->demo < 16) {
                switch (GV->frame & 0x3F) {
                    case 0:
                        for (i = 7; i < 25; i++) {
                            VGS0_ADDR_FG->attr[17][i] = 0x80;
                        }
                        break;
                    case 24:
                        for (i = 7; i < 25; i++) {
                            VGS0_ADDR_FG->attr[17][i] = 0x81;
                        }
                        break;
                    case 32:
                        for (i = 7; i < 25; i++) {
                            VGS0_ADDR_FG->attr[17][i] = 0x00;
                        }
                        GV->demo++;
                        break;
                    case 56:
                        for (i = 7; i < 25; i++) {
                            VGS0_ADDR_FG->attr[17][i] = 0x81;
                        }
                        break;
                }
            }
        }

        // 敵出現制御
        level_proc();

        // 追加可能なら星と泡を追加
        add_star();
        add_bubble();

        // プレイヤーショットの移動
        move_pshot();

        // プレイヤーの移動
        move_player();

        // 敵キャラの移動
        move_enemy();

        // 画面エフェクトの処理を実行
        screen_effect_proc();

        // スコア加算値の計算処理
        score_calc();

        // スコア再描画 & ハイスコア更新（加算時のみ）
        if (GV->scoreAdded) {
            score_print(VGS0_ADDR_FG);
        }

        // ヒット数を描画
        hit_print();

        // 王冠を表示
        render_crown();

        // デモから抜ける
        if (16 <= GV->demo) {
            if (16 == GV->demo) {
                vgs0_bgm_fadeout();
            }
            GV->demo++;
            if (24 <= GV->demo) {
                uint8_t ptn = (GV->demo - 24) >> 2;
                ptn |= 0xE0;
                for (i = 0; i < 32; i++) {
                    for (j = 0; j < 32; j++) {
                        VGS0_ADDR_FG->ptn[j][i] = ptn;
                        VGS0_ADDR_FG->attr[j][i] = 0x82;
                    }
                }
            }
            if (85 == GV->demo) {
                return 0;
            }
        }

        // リプレイデータを記録（デモプレイの記録用）
#if 1
        *((uint8_t*)(0xA000 + GV->replay)) = GV->pad;
        GV->replay++;
        GV->replay &= 0x1FFF;
#endif
    }

    if (needRestart) {
        while (1) {
            vgs0_wait_vsync();
            needRestart += 1;
            if (16 <= needRestart && 0 == (needRestart & 0x03)) {
                uint8_t ptn = (needRestart - 16) >> 2;
                ptn |= 0xE0;
                for (i = 0; i < 32; i++) {
                    for (j = 0; j < 32; j++) {
                        VGS0_ADDR_FG->ptn[j][i] = ptn;
                        VGS0_ADDR_FG->attr[j][i] = 0x82;
                    }
                }
            }
            if (88 == needRestart) {
                vgs0_memcpy((uint16_t)&GV->hi[0], (uint16_t)&SR->data[0].sc, 8);
                vgs0_memset((uint16_t)GV->sc, 0, 8);
                vgs0_memset((uint16_t)VGS0_ADDR_OAM, 0, sizeof(OAM) * 256);
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
                return 1;
            }
        }
    }
    return 0;
}