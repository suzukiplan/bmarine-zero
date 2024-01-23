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

void submain(uint8_t arg) __z88dk_fastcall
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
    vgs0_memset(0xC000 + 8, 0x00, sizeof(GlobalVariables) - 8);
    GV->player.x.value = 0x7400;
    GV->player.y.value = 0x4000;
    GV->player.hp = 80;
    GV->player.chp = 0;
    GV->demo = demo;
    GV->extra = extra;

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
        vgs0_fg_putstr(7, 17, 0x80, "PRESS START BUTTON");
    }

    // メインループ
    uint8_t paused = 0;
    while (255 != GV->player.dead) {
        vgs0_wait_vsync();
        GV->frame++;

        // ポーズ
        if (0 == demo && 0 == paused && 0 != (vgs0_joypad_get() & VGS0_JOYPAD_ST)) {
            vgs0_fg_putstr(11, 16, 0x80, "  PAUSE  ");
            vgs0_se_play(26);
            paused = 1;
        } else if (1 == paused) {
            if (0 == (vgs0_joypad_get() & VGS0_JOYPAD_ST)) {
                paused = 2;
            }
        } else if (2 == paused) {
            if (0 != (vgs0_joypad_get() & VGS0_JOYPAD_ST)) {
                vgs0_fg_putstr(11, 16, 0x80, "         ");
                paused = 3;
            }
        } else if (3 == paused) {
            if (0 == (vgs0_joypad_get() & VGS0_JOYPAD_ST)) {
                vgs0_se_play(24);
                paused = 0;
            }
        }
        if (paused) {
            GV->frame--;
            continue;
        }

        // デモプレイ中
        if (GV->demo) {
            if (vgs0_joypad_get() & VGS0_JOYPAD_ST) {
                GV->demoEnd = 1;
                return;
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
                return;
            }
        }

        // リプレイデータを記録（デモプレイの記録用）
#if 1
        *((uint8_t*)(0xA000 + GV->replay)) = GV->pad;
        GV->replay++;
        GV->replay &= 0x1FFF;
#endif
    }
}