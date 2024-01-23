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

// スコア表示（ハイスコア更新を含む）
void score_print(NameTable* nam) __z88dk_fastcall
{
    uint8_t detectS = 0;
    uint8_t detectH = 0;
    uint8_t updateH = 0;
    uint8_t notUpdateH = 0;
    for (uint8_t i = 0; i < 8; i++) {
        // score
        if (!updateH) {
            if (GV->hi[7 - i] < GV->sc[7 - i]) {
                if (0 == notUpdateH) {
                    updateH = 1;
                }
            } else if (GV->sc[7 - i] < GV->hi[7 - i]) {
                notUpdateH = 1;
            }
        }
        if (updateH) {
            GV->hi[7 - i] = GV->sc[7 - i];
        }
        if (7 == i || 0 != detectS || 0 != GV->sc[7 - i]) {
            nam->ptn[2][5 + i] = GV->sc[7 - i] + '0';
            detectS = 1;
        } else {
            nam->ptn[2][5 + i] = ' ';
        }
        // high score
        if (7 == i || 0 != detectH || 0 != GV->hi[7 - i]) {
            nam->ptn[2][21 + i] = GV->hi[7 - i] + '0';
            detectH = 1;
        } else {
            nam->ptn[2][21 + i] = ' ';
        }
    }
    GV->scoreAdded = 0;
}

void score_calc(void) __z88dk_fastcall
{
    for (uint8_t i = 0; i < 7; i++) {
        while (10 <= GV->sadd[i]) {
            GV->sadd[i] -= 10;
            GV->sadd[i + 1] += 1;
        }
        if (0 < GV->sadd[i]) {
            score_increment(i);
            GV->sadd[i]--;
        }
    }
    if (0 < GV->sadd[7]) {
        score_increment(7);
        GV->sadd[7]--;
    }
}

// 指定ケタ（0: 10の位, 7: 億の位）のスコアを加算
void score_increment(uint8_t keta) __z88dk_fastcall
{
    if (GV->demo) {
        return;
    }
    GV->scoreAdded = 1;
    if (7 < keta) {
        for (uint8_t i = 0; i < 8; i++) {
            GV->sc[i] = 9;
        }
        return;
    }
    GV->sc[keta] += 1;
    if (9 < GV->sc[keta]) {
        GV->sc[keta] = 0;
        score_increment(keta + 1);
    }
}

static const uint8_t hitptn[10] = {0x00, 0x20, 0x40, 0x60, 0x02, 0x22, 0x42, 0x62, 0x04, 0x24};
static const uint8_t hitspdx[6] = {7, 6, 5, 4, 3, 2};
static const uint8_t hitposx[6] = {175, 185, 195, 211, 223, 231};

void hit_print() __z88dk_fastcall
{
    // コンボタイマーのチェック
    if (GV->hkt) {
        GV->hkt--;
        if (0 == GV->hkt) {
            GV->hit = 0;
        }
    }

    // ヒット数の減少を検出した場合、退避状態へ遷移
    if (GV->hstat && GV->hit < GV->hitlog) {
        GV->hitlog = GV->hit;
        GV->hstat = 100;
    }

    // コンボ数に変化があったのみ更新処理を実行
    if (GV->hitlog != GV->hit && 10 < GV->hit && GV->hstat < 100) {
        GV->hitlog = GV->hit;
        // 最大コンボを更新
        if (GV->st.maxhit < GV->hit) {
            GV->st.maxhit = GV->hit;
        }
        // カンスト対応 (表示上は 999, 内部的には 9999 まで記録)
        if (999 < GV->hit) {
            if (9999 < GV->hit) {
                GV->hit = 9999;
            }
            VGS0_ADDR_OAM[SP_HIT].attr = 0x84;
            VGS0_ADDR_OAM[SP_HIT + 1].attr = 0x84;
            VGS0_ADDR_OAM[SP_HIT + 2].attr = 0x84;
            VGS0_ADDR_OAM[SP_HIT + 3].attr = 0x84;
            VGS0_ADDR_OAM[SP_HIT + 4].attr = 0x84;
            VGS0_ADDR_OAM[SP_HIT + 5].attr = 0x84;
            VGS0_ADDR_OAM[SP_HIT].ptn = hitptn[9];
            VGS0_ADDR_OAM[SP_HIT + 1].attr = hitptn[9];
            VGS0_ADDR_OAM[SP_HIT + 2].attr = hitptn[9];
        } else {
            // 100の位のパターン更新
            uint16_t hit = GV->hit;
            if (GV->hit < 100) {
                VGS0_ADDR_OAM[SP_HIT].attr = 0x00;
            } else {
                VGS0_ADDR_OAM[SP_HIT].attr = 0x84;
                VGS0_ADDR_OAM[SP_HIT].ptn = hitptn[vgs0_div16(hit, 100)];
                hit = vgs0_mod(hit, 100);
            }
            // 10の位のパターン更新
            if (GV->hit < 10) {
                VGS0_ADDR_OAM[SP_HIT + 1].attr = 0x00;
            } else {
                VGS0_ADDR_OAM[SP_HIT + 1].attr = 0x84;
                VGS0_ADDR_OAM[SP_HIT + 1].ptn = hitptn[vgs0_div(hit, 10)];
                hit = vgs0_mod(hit, 10);
            }
            // 1の位のパターン更新
            if (GV->hit < 1) {
                VGS0_ADDR_OAM[SP_HIT + 2].attr = 0x00;
            } else {
                VGS0_ADDR_OAM[SP_HIT + 2].attr = 0x84;
                VGS0_ADDR_OAM[SP_HIT + 2].ptn = hitptn[hit];
            }
        }
    }
    // ステータス変化に応じたOAM更新
    switch (GV->hstat) {
        case 0: { // 初期状態
            if (10 < GV->hit) {
                // HITの属性設定
                VGS0_ADDR_OAM[SP_HIT + 3].ptn = 0xE0;
                VGS0_ADDR_OAM[SP_HIT + 4].ptn = 0xE2;
                VGS0_ADDR_OAM[SP_HIT + 5].ptn = 0xE4;
                VGS0_ADDR_OAM[SP_HIT + 3].attr = 0x84;
                VGS0_ADDR_OAM[SP_HIT + 4].attr = 0x84;
                VGS0_ADDR_OAM[SP_HIT + 5].attr = 0x84;
                // その他の初期値設定
                for (uint8_t i = 0; i < 6; i++) {
                    VGS0_ADDR_OAM[SP_HIT + i].bank = BANK_HIT_SP;
                    VGS0_ADDR_OAM[SP_HIT + i].x = 240;
                    VGS0_ADDR_OAM[SP_HIT + i].y = 27;
                    VGS0_ADDR_OAM[SP_HIT + i].widthMinus1 = 1;
                    VGS0_ADDR_OAM[SP_HIT + i].heightMinus1 = 1;
                }
                GV->hstat = 1; // 登場状態へ繊維
            }
            break;
        }
        case 1: { // 登場状態
            uint8_t done = 1;
            for (uint8_t i = 0; i < 6; i++) {
                VGS0_ADDR_OAM[SP_HIT + i].x -= hitspdx[i];
                if (VGS0_ADDR_OAM[SP_HIT + i].x < hitposx[i]) {
                    VGS0_ADDR_OAM[SP_HIT + i].x = hitposx[i];
                } else {
                    done = 0;
                }
            }
            if (done) {
                GV->hstat = 2; // 表示完了状態へ繊維
            }
            break;
        }
        case 2: { // 表示完了状態
            // TODO: 何かするかも
            break;
        }
        case 100: { // 退避状態
            uint8_t done = 1;
            for (uint8_t i = 0; i < 6; i++) {
                VGS0_ADDR_OAM[SP_HIT + i].ptn |= 0x08;
                if (VGS0_ADDR_OAM[SP_HIT + i].x < 240) {
                    VGS0_ADDR_OAM[SP_HIT + i].x++;
                    if (240 <= VGS0_ADDR_OAM[SP_HIT + i].x) {
                        VGS0_ADDR_OAM[SP_HIT + i].attr = 0x00;
                    } else {
                        done = 0;
                    }
                }
            }
            if (done) {
                GV->hstat = 0; // 初期状態へ戻る
            }
            break;
        }
    }
}

void render_hp(void) __z88dk_fastcall
{
    if (GV->player.hp == GV->player.chp) {
        return;
    }
    if (GV->player.hp < GV->player.chp) {
        GV->player.chp--;
    } else {
        GV->player.chp = GV->player.hp;
    }
    uint8_t i, j;
    if (80 == GV->player.chp) {
        for (i = 0; i < 5; i++) {
            VGS0_ADDR_OAM[SP_HP + i].ptn = 0xA0;
            VGS0_ADDR_OAM[SP_HP + i].attr = 0x86;
        }
        return;
    }
    uint8_t attr;
    if (GV->player.hp < 32) {
        attr = 0x8A;
    } else {
        attr = 0x86;
    }
    uint8_t chp = GV->player.chp >> 1;
    j = 0;
    for (uint8_t i = 8; i <= 40; i += 8, j += 1) {
        if (chp < i) {
            i = chp & 0x07;
            if (i) {
                VGS0_ADDR_OAM[SP_HP + j].ptn = (7 - i) | 0xA0;
                VGS0_ADDR_OAM[SP_HP + j].attr = attr;
            } else {
                VGS0_ADDR_OAM[SP_HP + j].attr = 0x00;
            }
            for (i = 0; i < j; i++) {
                VGS0_ADDR_OAM[SP_HP + i].ptn = 0xA0;
                VGS0_ADDR_OAM[SP_HP + i].attr = attr;
            }
            for (i = j + 1; i < 5; i++) {
                VGS0_ADDR_OAM[SP_HP + i].attr = 0x00;
            }
            return;
        }
    }
}

void render_crown(void) __z88dk_fastcall
{
    if (GV->smc.value == GV->smcPrev.value) {
        return;
    }
    GV->smcPrev.value = GV->smc.value;
    uint16_t n = GV->smcPrev.value >> 4;
    uint8_t nn, i;
    if (30 < n) {
        nn = 30;
    } else {
        nn = n & 0xFF;
    }
    for (i = 0; i < nn; i++) {
        VGS0_ADDR_FG->ptn[24][1 + i] = 0xB0;
        VGS0_ADDR_FG->attr[24][1 + i] = 0x86;
    }
    for (; i < 30; i++) {
        VGS0_ADDR_FG->attr[24][1 + i] = 0x00;
    }
}