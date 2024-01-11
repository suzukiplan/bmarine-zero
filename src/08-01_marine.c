#include "header.h"

// n8 変数名の定義
#define nFrameCounter enemy->n8[0] /* フレームカウンタ */
#define nPropeller enemy->n8[1]    /* プロペラのアニメーション用 */
#define nThunderWait enemy->n8[2]  /* 雷撃の発射ウェイト */
#define nMarineAnime enemy->n8[3]  /* 艦体のアニメーション用 */
#define nPersonality enemy->n8[4]  /* 性格フラグ */

// 1: 潜水艦 (左から右)
void move_marineLR(Enemy* enemy) __z88dk_fastcall
{
    if (1 == enemy->flag) {
        enemy->flag = 2;
        enemy->vx.value = get_random(&GV->ridx);
        enemy->vx.value += 0x0040;
        nPersonality = get_random(&GV->ridx) & 0x03;
    }
    nFrameCounter++;

    // 性格別の行動変化
    if (1 == nPersonality) {
        // 片思いタイプ（自機が好き）
        if (enemy->x.raw[1] < GV->player.x.raw[1]) {
            if (enemy->vx.raw[1] < 5) {
                enemy->vx.value += 7;
            }
        } else {
            if (0 < enemy->vx.raw[1]) {
                enemy->vx.value -= 133;
            }
        }
    } else if (3 == nPersonality) {
        // ひきこもりタイプ（底が好き）
        if (enemy->y.raw[1] < 158 && 0 == enemy->vy.raw[1]) {
            enemy->vy.value++;
        } else if (163 < enemy->y.raw[1]) {
            enemy->vy.value = 0;
        } else {
            if (16 < enemy->vy.value) {
                enemy->vy.value -= 13;
            }
        }
    }

    // 雷撃
    if (nThunderWait) {
        nThunderWait--;
        nMarineAnime++;
        switch (nMarineAnime) {
            case 1:
                VGS0_ADDR_OAM[enemy->si[1]].ptn = 0x90;
                VGS0_ADDR_OAM[enemy->si[2]].ptn = 0x91;
                break;
            case 3:
                VGS0_ADDR_OAM[enemy->si[1]].ptn = 0x93;
                VGS0_ADDR_OAM[enemy->si[2]].ptn = 0x94;
                break;
            case 5:
                VGS0_ADDR_OAM[enemy->si[1]].ptn = 0x96;
                VGS0_ADDR_OAM[enemy->si[2]].ptn = 0x97;
                break;
            case 9:
                VGS0_ADDR_OAM[enemy->si[1]].ptn = 0x99;
                VGS0_ADDR_OAM[enemy->si[2]].ptn = 0x9A;
                break;
            case 12:
                VGS0_ADDR_OAM[enemy->si[1]].ptn = 0x9C;
                VGS0_ADDR_OAM[enemy->si[2]].ptn = 0x9D;
                break;
            case 15:
                VGS0_ADDR_OAM[enemy->si[1]].ptn = 0x15;
                VGS0_ADDR_OAM[enemy->si[2]].ptn = 0x16;
                break;
        }
    } else {
        if (enemy->x.raw[1] - 16 < GV->player.x.raw[1] + 24 && GV->player.x.raw[1] < enemy->x.raw[1] - 8) {
            add_enemy(ET_THUNDER, enemy->x.raw[1] - 12, enemy->y.raw[1] - 4);
            nThunderWait = 16;
            nMarineAnime = 0;
        }
    }

    // クリッピング
    switch (enemy->flag) {
        case 2:
            if (8 <= enemy->x.raw[1]) {
                VGS0_ADDR_OAM[enemy->si[2]].attr = 0x80;
                enemy->flag++;
            }
            break;
        case 3:
            if (24 <= enemy->x.raw[1]) {
                VGS0_ADDR_OAM[enemy->si[1]].attr = 0x80;
                enemy->check = 1;
                enemy->flag++;
            }
            break;
        case 4:
            if (28 <= enemy->x.raw[1]) {
                VGS0_ADDR_OAM[enemy->si[0]].attr = 0x80;
                enemy->flag++;
            }
            break;
        case 5:
            if (enemy->x.raw[1] < 28) {
                enemy->flag++;
            } else {
                if (0 == (nFrameCounter & 0x03)) {
                    add_spray(enemy->x.raw[1] - 28, enemy->y.raw[1] + 4, 0x38, 0x83);
                }
            }
            break;
        case 6:
            if (8 <= enemy->x.raw[1]) {
                VGS0_ADDR_OAM[enemy->si[2]].attr = 0x00;
                enemy->flag++;
            }
            break;
        case 7:
            if (16 <= enemy->x.raw[1]) {
                enemy->flag = 0; // 画面アウト
            }
            break;
    }

    // プロペラを回転
    if (0 == (nFrameCounter & 3)) {
        nPropeller++;
        nPropeller &= 0x03;
        VGS0_ADDR_OAM[enemy->si[0]].ptn = 0x18 + nPropeller;
    }
}
