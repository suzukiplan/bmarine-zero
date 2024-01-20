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

// n8 変数名の定義
#define nFrameCounter enemy->n8[0] /* フレームカウンタ */
#define nPropeller enemy->n8[1]    /* プロペラのアニメーション用 */
#define nThunderWait enemy->n8[2]  /* 雷撃の発射ウェイト */
#define nMarineAnime enemy->n8[3]  /* 艦体のアニメーション用 */
#define nPersonality enemy->n8[4]  /* 性格フラグ */
#define nSurmount enemy->n8[5]     /* 潜水フラグ */

static const uint8_t swim_ptn[8] = {0x15, 0xB0, 0xB3, 0xB6, 0xB9, 0xB9, 0xBC, 0xB3};
static const uint8_t bullet_ptn[8] = {0x90, 0x90, 0x93, 0x96, 0x96, 0x99, 0x99, 0x9C};

// 2: 潜水艦 (右から左)
void move_marineRL(Enemy* enemy) __z88dk_fastcall
{
    if (1 == enemy->flag) {
        enemy->flag = 2;
        enemy->vx.value -= get_random(&GV->ridx);
        enemy->vx.value -= 0x0040;
        nPersonality = get_random(&GV->ridx) & 0x03;
    }
    nFrameCounter++;

    // 性格別の行動変化
    if (1 == nPersonality || 2 == nPersonality) {
        // 片思いタイプ（自機が好き）
        if (GV->player.x.raw[1] + 24 < enemy->x.raw[1]) {
            if (-5 < (int8_t)enemy->vx.raw[1]) {
                enemy->vx.value -= 7;
            }
        } else {
            if (enemy->vx.raw[1] < 0xFF) {
                enemy->vx.value += 133;
            }
        }
    }
    if (3 == nPersonality || 2 == nPersonality) {
        // ひきこもりタイプ（底が好き）
        if (enemy->y.raw[1] < 158 && 0 == enemy->vy.raw[1]) {
            if (0 == nSurmount && 0 == nThunderWait) {
                nSurmount = 1;
                VGS0_ADDR_OAM[enemy->si[1]].ptn = 0x51;
                VGS0_ADDR_OAM[enemy->si[0]].ptn = 0x52;
            }
            enemy->vy.value++;
        } else if (163 < enemy->y.raw[1]) {
            if (0 != nSurmount && 0 == nThunderWait) {
                nSurmount = 0;
                nMarineAnime = 0;
                VGS0_ADDR_OAM[enemy->si[1]].ptn = 0x15;
                VGS0_ADDR_OAM[enemy->si[0]].ptn = 0x16;
            }
            enemy->vy.value = 0;
        } else {
            if (0 != nSurmount && 0 == nThunderWait) {
                nSurmount = 0;
                nMarineAnime = 0;
                VGS0_ADDR_OAM[enemy->si[1]].ptn = 0x15;
                VGS0_ADDR_OAM[enemy->si[0]].ptn = 0x16;
            }
            if (16 < enemy->vy.value) {
                enemy->vy.value -= 13;
            }
        }
    }

    // 雷撃
    if (nThunderWait) {
        nThunderWait--;
        if (0 == (nFrameCounter & 0x01)) {
            nMarineAnime++;
            nMarineAnime &= 0x07;
            if (nMarineAnime) {
                VGS0_ADDR_OAM[enemy->si[1]].ptn = bullet_ptn[nMarineAnime];
                VGS0_ADDR_OAM[enemy->si[0]].ptn = bullet_ptn[nMarineAnime] + 1;
            } else {
                if (nSurmount) {
                    VGS0_ADDR_OAM[enemy->si[1]].ptn = 0x51;
                    VGS0_ADDR_OAM[enemy->si[0]].ptn = 0x52;
                } else {
                    VGS0_ADDR_OAM[enemy->si[1]].ptn = 0x15;
                    VGS0_ADDR_OAM[enemy->si[0]].ptn = 0x16;
                }
            }
        }
    } else {
        if (enemy->x.raw[1] - 16 < GV->player.x.raw[1] + 24 && GV->player.x.raw[1] < enemy->x.raw[1] - 8) {
            add_enemy(ET_THUNDER, enemy->x.raw[1], enemy->y.raw[1] - 4);
            nThunderWait = 16;
            nMarineAnime = 0;
        } else if (0 == nSurmount) {
            // 平泳ぎ
            if (0 == (nFrameCounter & 0x03)) {
                nMarineAnime++;
                nMarineAnime &= 0x07;
                VGS0_ADDR_OAM[enemy->si[1]].ptn = swim_ptn[nMarineAnime];
                VGS0_ADDR_OAM[enemy->si[0]].ptn = swim_ptn[nMarineAnime] + 1;
            }
        }
    }

    // クリッピング
    if (2 == enemy->flag) {
        if (enemy->x.raw[1] < 248) {
            VGS0_ADDR_OAM[enemy->si[0]].attr = 0xC0;
            enemy->flag++;
        }
    } else if (3 == enemy->flag) {
        if (enemy->x.raw[1] < 232) {
            VGS0_ADDR_OAM[enemy->si[1]].attr = 0xC0;
            enemy->check = 1;
            enemy->flag++;
        }
    } else if (4 == enemy->flag) {
        if (enemy->x.raw[1] < 228) {
            VGS0_ADDR_OAM[enemy->si[2]].attr = 0xC0;
            enemy->flag++;
        }
    } else if (5 == enemy->flag) {
        if (248 < enemy->x.raw[1]) {
            enemy->flag++;
        } else {
            if (0 == (nFrameCounter & 0x03)) {
                add_spray(enemy->x.raw[1] + 24, enemy->y.raw[1] + 4, 0x38, 0xC3);
            }
        }
    } else if (6 == enemy->flag) {
        if (enemy->x.raw[1] < 248) {
            VGS0_ADDR_OAM[enemy->si[0]].attr = 0x00;
            enemy->flag++;
        }
    } else {
        if (enemy->x.raw[1] < 232) {
            enemy->flag = 0; // 画面アウト
            return;
        }
    }

    // プロペラを回転
    if (0 == (nFrameCounter & 3)) {
        nPropeller++;
        nPropeller &= 0x03;
        if (nSurmount) {
            VGS0_ADDR_OAM[enemy->si[2]].ptn = 0x54 + nPropeller;
        } else {
            VGS0_ADDR_OAM[enemy->si[2]].ptn = 0x18 + nPropeller;
        }
    }
}
