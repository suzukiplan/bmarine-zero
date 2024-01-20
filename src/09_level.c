#include "header.h"

static void level_up(void) __z88dk_fastcall
{
    GV->level++;
    GV->levelFrame = 0xA0;
    GV->waitclear = 1; // 敵が消えるのを待つ
}

void level_proc(void) __z88dk_fastcall
{
    uint8_t i, j;

    // レベルアップ処理
    if (0 != GV->levelFrame) {
        if (0xA0 == GV->levelFrame && GV->waitclear) {
            return; // 敵のクリア待ち
        }
        if (0xA0 == GV->levelFrame) {
            GV->enemies = 0;
            if (5 == GV->level) {
                vgs0_fg_putstr(6, 14, 0x80, " NABURA MODE START! ");
            } else if (6 == GV->level) {
                vgs0_fg_putstr(6, 14, 0x80, "INTO THE CRAB HOUSE!");
            } else if (7 == GV->level) {
                vgs0_fg_putstr(6, 14, 0x80, "  LEVEL MAX START!  ");
            } else if (8 == GV->level) {
                vgs0_fg_putstr(6, 14, 0x80, "  SHI-NU GA YOI...  ");
                vgs0_bgm_play(1);
                vgs0_palette_set(0, 1, 3, 7, 14);
                vgs0_palette_set(4, 8, 3, 7, 14);
            } else {
                vgs0_fg_putstr(6, 14, 0x80, "   LEVEL 0 START!   ");
                VGS0_ADDR_FG->ptn[14][15] = '0' + GV->level;
            }
            if (5 == GV->level) {
                // ナブラモードに遷移
                vgs0_bgm_play(3);
                GV->player.nabura = 1;
            } else if (6 == GV->level) {
                // ダークネスモードに遷移
                vgs0_bgm_play(2);
                GV->player.darkness = 1;
            }
        }
        GV->levelFrame--;
        j = GV->levelFrame & 0x1F;
        if (0 == j) {
            for (i = 0; i < 20; i++) {
                VGS0_ADDR_FG->attr[14][6 + i] = 0x00;
            }
        } else if (0x10 == j) {
            for (i = 0; i < 20; i++) {
                VGS0_ADDR_FG->attr[14][6 + i] = 0x80;
            }
        }
    }

    // レベル別の敵出現処理
    switch (GV->level) {
        case 0: // 即座にレベルアップ
            level_up();
            GV->level = 8; // debug (初期レベル)
            break;
        case 1: // 潜水艦が左から現れる（単方向なら簡単なので出現量多め）
            if (0 == (GV->frame & 0x1F)) {
                add_enemy(ET_MARINE_LR, 0, (get_random(&GV->ridx) & 0x3F) + 0x60);
            }
            if (80 < GV->enemies) {
                level_up();
            }
            break;
        case 2: // 潜水艦が左右から大量に現れる
            if (0 == (GV->frame & 0x3F)) {
                add_enemy(ET_MARINE_LR, 0, (get_random(&GV->ridx) & 0x3F) + 0x60);
                add_enemy(ET_MARINE_RL, 255, (get_random(&GV->ridx) & 0x3F) + 0x60);
            }
            if (100 < GV->enemies) {
                level_up();
            }
            break;
        case 3: // 鳥登場
            if (0 == (GV->frame & 0x3F)) {
                add_enemy(ET_MARINE_LR, 0, (get_random(&GV->ridx) & 0x3F) + 0x60);
                add_enemy(ET_MARINE_RL, 255, (get_random(&GV->ridx) & 0x3F) + 0x60);
            }
            if (0 == (GV->frame & 0x7F)) {
                add_enemy(ET_BIRD, 248, 8 + (get_random(&GV->ridx) & 0x0F));
            }
            if (170 < GV->enemies) {
                level_up();
            }
            break;
        case 4: // 魚登場
            if (0 == (GV->frame & 0x3F)) {
                add_enemy(ET_MARINE_LR, 0, (get_random(&GV->ridx) & 0x3F) + 0x60);
                add_enemy(ET_MARINE_RL, 255, (get_random(&GV->ridx) & 0x3F) + 0x60);
            }
            if (0 == (GV->frame & 0x7F)) {
                add_enemy(ET_BIRD, 248, 8 + (get_random(&GV->ridx) & 0x0F));
            }
            if (0x3F == (GV->frame & 0x7F)) {
                add_enemy(ET_FISH, 248, 92 + (get_random(&GV->ridx) & 0x3F));
                add_enemy(ET_FISH, 248, 92 + (get_random(&GV->ridx) & 0x3F));
            }
            if (200 < GV->enemies) {
                vgs0_bgm_fadeout();
                level_up();
            }
            break;
        case 5: // ナブラモード（遷移演出終了後から敵出現）
            if (0 == GV->player.nabura) {
                if (0 == (GV->frame & 0x7F)) {
                    add_enemy(ET_BIRD, 248, 8 + (get_random(&GV->ridx) & 0x0F));
                }
                if (0x0F == (GV->frame & 0x0F)) {
                    add_enemy(ET_FISH, 248, 92 + (get_random(&GV->ridx) & 0x3F));
                }
            }
            if (200 < GV->enemies) {
                vgs0_bgm_fadeout();
                level_up();
            }
            break;
        case 6: // 癒やしのカニ乱舞
            if (0x0F == (GV->frame & 0x0F)) {
                add_enemy(ET_KANI, 248, 168);
            }
            if (120 < GV->enemies) {
                level_up();
            }
            break;
        case 7: // 全敵キャラが登場
        case 8: // レベル8は7と同じだが打ち返し弾あり
            if (0 == (GV->frame & 0x3F)) {
                add_enemy(ET_MARINE_LR, 0, (get_random(&GV->ridx) & 0x3F) + 0x60);
                add_enemy(ET_MARINE_RL, 255, (get_random(&GV->ridx) & 0x3F) + 0x60);
            }
            if (0 == (GV->frame & 0x7F)) {
                add_enemy(ET_BIRD, 248, 8 + (get_random(&GV->ridx) & 0x0F));
            }
            if (0x3F == (GV->frame & 0x7F)) {
                add_enemy(ET_FISH, 248, 92 + (get_random(&GV->ridx) & 0x3F));
            }
            if (0x1F == (GV->frame & 0x7F)) {
                add_enemy(ET_KANI, 248, 168);
            }
            if (7 == GV->level && 100 < GV->enemies) {
                vgs0_bgm_fadeout();
                level_up();
            }
            break;
    }
}

// レーザーによる追加メダル種別の決定
uint8_t level_medal_laser(uint8_t type) __z88dk_fastcall
{
    if (8 <= GV->level) {
        return 1; // レベル8 はスコアのみ
    } else if (type == ET_KANI || type == ET_KANITAMA) {
        return 0; // カニとカニタマはレーザーを浴びる回復を出す（レベルMAX前のカニステージはレーザーで稼げるようにする為）
    } else {
        return 1; // カニ以外は常にスコア（敵種別に関係なく）
    }
}

// 誘爆による追加メダル種別の決定
uint8_t level_medal_bomb(uint8_t type) __z88dk_fastcall
{
    if (ET_MARINE_LR == type || ET_MARINE_RL == type) {
        if (8 <= GV->level) {
            return get_random(&GV->ridx) & 0x01; // レベル8の潜水艦は50%の確率で回復を出す
        } else {
            return 0; // レベル8以外の潜水艦は常に回復を出す
        }
    }
    switch (GV->level) {
        case 1: // レベル1 は回復のみ
            return 0;
        case 2: // レベル2 は 75% の確率で回復
            if (get_random(&GV->ridx) < 0xC0) {
                return 0;
            } else {
                return 1;
            }
        case 3: // レベル3 は 50% の確率で回復
            if (get_random(&GV->ridx) < 0x80) {
                return 0;
            } else {
                return 1;
            }
        case 4: // レベル4 は 25% の確率で回復
            if (get_random(&GV->ridx) < 0x40) {
                return 0;
            } else {
                return 1;
            }
        case 5: // レベル5 は 12.5% の確率で回復
            if (get_random(&GV->ridx) < 0x20) {
                return 0;
            } else {
                return 1;
            }
        case 6: // レベル6 は 6.25% の確率で回復
            if (get_random(&GV->ridx) < 0x10) {
                return 0;
            } else {
                return 1;
            }
        default: // レベル7以降はスコアのみ
            return 1;
    }
}

// ショットによる追加メダル種別の決定
uint8_t level_medal_shot(uint8_t type) __z88dk_fastcall
{
    switch (GV->level) {
        case 1: // レベル1 は回復のみ
            return 0;
        case 2: // レベル2 は 87.5% の確率で回復
            if (get_random(&GV->ridx) < 0xE0) {
                return 0;
            } else {
                return 1;
            }
        case 3: // レベル3 は 75% の確率で回復
            if (get_random(&GV->ridx) < 0xC0) {
                return 0;
            } else {
                return 1;
            }
        case 4: // レベル4 は 50% の確率で回復
            if (get_random(&GV->ridx) < 0x80) {
                return 0;
            } else {
                return 1;
            }
        case 5: // レベル5 は 25% の確率で回復
            if (get_random(&GV->ridx) < 0x40) {
                return 0;
            } else {
                return 1;
            }
        case 6: // レベル6 は 12.5% の確率で回復
            if (get_random(&GV->ridx) < 0x20) {
                return 0;
            } else {
                return 1;
            }
        case 7: // レベル7 は 6.25% の確率で回復
            if (get_random(&GV->ridx) < 0x10) {
                return 0;
            } else {
                return 1;
            }
        default: // レベル8はスコアのみ
            return 1;
    }
}
