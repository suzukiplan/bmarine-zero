#include "header.h"

#define SP_DUST 208
#define SP_SHOT 224
#define SP_SPRAY 240

inline void update_player_position(void)
{
    VGS0_ADDR_OAM[0].x = GV->player.x.raw[1];
    VGS0_ADDR_OAM[0].y = GV->player.y.raw[1];
    VGS0_ADDR_OAM[1].x = GV->player.x.raw[1] + 8;
    VGS0_ADDR_OAM[1].y = GV->player.y.raw[1];
    VGS0_ADDR_OAM[2].x = GV->player.x.raw[1] + 16;
    VGS0_ADDR_OAM[2].y = GV->player.y.raw[1];
    VGS0_ADDR_OAM[3].x = GV->player.x.raw[1];
    VGS0_ADDR_OAM[3].y = GV->player.y.raw[1] + 8;
    VGS0_ADDR_OAM[4].x = GV->player.x.raw[1] + 8;
    VGS0_ADDR_OAM[4].y = GV->player.y.raw[1] + 8;
    VGS0_ADDR_OAM[5].x = GV->player.x.raw[1] + 16;
    VGS0_ADDR_OAM[5].y = GV->player.y.raw[1] + 8;
}

inline void add_spray(uint8_t x, uint8_t y, uint8_t sn, uint8_t attr)
{
    GV->sprayIndex &= 0x0F;
    GV->spray[GV->sprayIndex].sn = sn;
    GV->spray[GV->sprayIndex].t = 0;
    uint8_t i = SP_SPRAY + GV->sprayIndex;
    VGS0_ADDR_OAM[i].x = x;
    VGS0_ADDR_OAM[i].y = y;
    VGS0_ADDR_OAM[i].ptn = sn;
    VGS0_ADDR_OAM[i].attr = attr;
    GV->sprayIndex += 1;
}

inline void add_shot(uint8_t x, uint16_t y)
{
    if (GV->shot[GV->shotIndex].flag) {
        return;
    }
    GV->shot[GV->shotIndex].flag = 1;
    GV->shot[GV->shotIndex].x = x;
    GV->shot[GV->shotIndex].y.value = y;
    GV->shot[GV->shotIndex].spd = 0;
    uint8_t sn = GV->shotIndex;
    sn <<= 1;
    sn += SP_SHOT;
    vgs0_oam_set(sn, x, GV->shot[GV->shotIndex].y.raw[1], 0x80, 0x13);
    vgs0_oam_set(sn + 1, x, GV->shot[GV->shotIndex].y.raw[1] + 8, 0x80, 0x23);
    GV->shotIndex++;
    GV->shotIndex &= 0x07;
}

void game_main(void)
{
    uint8_t i, j;
    uint8_t a = 12;
    uint8_t pad;

    // スコアと波をBGからFGに描き直す
    vgs0_fg_putstr(2, 2, 0x80, "SC         0    HI         0");
    score_print(VGS0_ADDR_FG);
    for (i = 0; i < 32; i++) {
        VGS0_ADDR_BG->ptn[2][i] = 0x00; // スコア(BG)をクリア
        VGS0_ADDR_BG->ptn[9][i] = 0x00; // 波をクリア
        VGS0_ADDR_FG->ptn[9][i] = 0xA2; // 波を描画
        VGS0_ADDR_FG->attr[9][i] = 0x80; // 波を描画
    }

    // 波に使っていたスプライトを自機スプライトに上書き
    vgs0_memcpy((uint16_t)&VGS0_ADDR_OAM[0],
                (uint16_t)&VGS0_ADDR_OAM[3],
                sizeof(OAM) * 9);

    // グローバル変数初期化
    GV->player.spd = 0;
    GV->player.jmp = 0;
    GV->player.flight = 0;
    GV->player.snock = 0;
    GV->player.shot = 0;
    GV->sprayIndex = 0;
    GV->shotIndex = 0;
    for (i = 0; i < 16; i++) {
        GV->spray[i].sn = 0;
    }
    for (i = 0; i < 8; i++) {
        GV->shot[i].flag = 0;
    }

    // メインループ
    while (1) {
        vgs0_wait_vsync();
        a++;

        // プレイヤーショットの移動
        for (i = 0; i < 8; i++) {
            if (GV->shot[i].flag) {
                GV->shot[i].spd += 44;
                GV->shot[i].y.value += GV->shot[i].spd;
                j = i;
                j <<= 1;
                j += SP_SHOT;
                if (176 < GV->shot[i].y.raw[1]) {
                    GV->shot[i].flag = 0;
                    VGS0_ADDR_OAM[j].attr = 0x00;
                    VGS0_ADDR_OAM[j + 1].attr = 0x00;
                } else {
                    GV->shot[i].flag++;
                    VGS0_ADDR_OAM[j].y = GV->shot[i].y.raw[1];
                    VGS0_ADDR_OAM[j + 1].y = GV->shot[i].y.raw[1] + 8;
                    if (0 == (GV->shot[i].flag & 0x03)) {
                        add_spray(GV->shot[i].x, GV->shot[i].y.raw[1], 0x40, 0x80);
                    }
                }
            }
        }

        // プレイヤー: 左右移動速度
        pad = vgs0_joypad_get();
        if (pad & VGS0_JOYPAD_LE) {
            if (-640 < GV->player.spd) {
                GV->player.spd -= 64;
            }
            VGS0_ADDR_OAM[1].attr |= 0b01000000; 
        } else if (pad & VGS0_JOYPAD_RI) {
            if (GV->player.spd < 640) {
                GV->player.spd += 64;
            }
            VGS0_ADDR_OAM[1].attr &= 0b10111111; 
        } else if (0 < GV->player.spd) {
            GV->player.spd -= 64;
        } else if (GV->player.spd < 0) {
            GV->player.spd += 64;
        }

        // プレイヤー: ジャンプ
        if (pad & VGS0_JOYPAD_T1) {
            if (0 == GV->player.jmpKeep && 0 == GV->player.jmp) {
                GV->player.jmp = -777;
                GV->player.jmpKeep = 1;
                add_spray(GV->player.x.raw[1] + 12, GV->player.y.raw[1] + 5, 0x30, 0x83);
                add_spray(GV->player.x.raw[1] + 4, GV->player.y.raw[1] + 5, 0x30, 0xC3);
            }
        } else {
            GV->player.jmpKeep = 0;
        }

        // プレイヤー: ショット
        if (pad & VGS0_JOYPAD_T2) {
            if (0 == GV->player.shot) {
                add_shot(GV->player.x.raw[1] + 8, GV->player.y.value + 0x0B00);
                GV->player.shot = 1;
            }
        } else {
            GV->player.shot = 0;
        }

        // プレイヤー: 座標更新
        if (0 != GV->player.spd || 0 != GV->player.jmp) {
            if (0 != GV->player.jmp) {
                GV->player.x.value += GV->player.spd;
                GV->player.x.value += GV->player.spd / 2;
                GV->player.y.value += GV->player.jmp;
                GV->player.flight++;
                score_increment(0);
                GV->player.y.value += GV->player.jmp;
                if (GV->player.jmpKeep) {
                    GV->player.jmp += 55;
                } else {
                    GV->player.jmp += 122;
                }
                if (0x41 < GV->player.y.raw[1]) {
                    GV->player.y.raw[1] = 0x41;
                    GV->player.jmp = 0;
                    GV->player.snock = GV->player.flight;
                    GV->player.snock >>= 3;
                    GV->player.slx = GV->player.x.raw[1] - 4;
                    GV->player.srx = GV->player.x.raw[1] + 20;
                    GV->player.flight = 0;
                }
            } else {
                GV->player.x.value += GV->player.spd;
                GV->player.y.raw[1] = 0x41;
                if (a & 0x01) {
                    j = GV->player.y.raw[1] + 5;
                    j += random[GV->ridx] & 0x01;
                    GV->ridx++;
                    j += random[GV->ridx] & 0x01;
                    GV->ridx++;
                    if (GV->player.spd < 0) {
                        add_spray(GV->player.x.raw[1] + 16, j, 0x30, 0x83);
                    } else {
                        add_spray(GV->player.x.raw[1], j, 0x30, 0xC3);
                    }
                }
            }
            update_player_position();
        } else if (GV->player.y.raw[1] != 0x40) {
            GV->player.y.raw[1] = 0x40;
            update_player_position();
        }

        // 着水時の衝撃波
        if (GV->player.snock) {
            GV->player.snock--;
            add_spray(GV->player.slx, GV->player.y.raw[1] + 5, 0x30, 0xC3);
            add_spray(GV->player.srx, GV->player.y.raw[1] + 5, 0x30, 0x83);
            GV->player.slx -= 6;
            GV->player.srx += 6;
        }

        // 水しぶき & けむり
        for (i = 0; i < 16; i++) {
            if (GV->spray[i].sn) {
                GV->spray[i].t += 1;
                if (GV->spray[i].t & 1) {
                    GV->spray[i].sn += 1;
                }
                if (16 == GV->spray[i].t) {
                    GV->spray[i].sn = 0;
                    VGS0_ADDR_OAM[SP_SPRAY + i].attr = 0x00;
                } else {
                    VGS0_ADDR_OAM[SP_SPRAY + i].ptn = GV->spray[i].sn;
                }
            }
        }

        // 波のアニメーション
        j = a;
        j &= 0x1F;
        j >>= 2;
        j |= 0xA0;
        for (i = 0; i < 32; i++) {
            VGS0_ADDR_FG->ptn[9][i] =j;
        }

        // スコア加算がされた場合は再描画
        if (GV->scoreAdded) {
            score_print(VGS0_ADDR_FG);
        }
    }
}