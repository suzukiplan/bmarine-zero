#include "header.h"

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
    GV->sprayIndex = 0;
    for (i = 0; i < 16; i++) {
        GV->spray[i].sn = 0;
    }

    // メインループ
    while (1) {
        vgs0_wait_vsync();
        a++;

        // プレイヤーの移動
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

        if (pad & VGS0_JOYPAD_T1) {
            if (0 == GV->player.jmpKeep && 0 == GV->player.jmp) {
                GV->player.jmp = -777;
                GV->player.jmpKeep = 1;
            }
        } else {
            GV->player.jmpKeep = 0;
        }

        if (0 != GV->player.spd || 0 != GV->player.jmp) {
            if (0 != GV->player.jmp) {
                GV->player.x.value += GV->player.spd;
                GV->player.x.value += GV->player.spd / 2;
                GV->player.y.value += GV->player.jmp;
            } else {
                GV->player.x.value += GV->player.spd;
            }
            if (0 == GV->player.jmp) {
                GV->player.y.raw[1] = 0x41;
            } else {
                GV->player.y.value += GV->player.jmp;
                if (GV->player.jmpKeep) {
                    GV->player.jmp += 55;
                } else {
                    GV->player.jmp += 122;
                }
                if (0x41 < GV->player.y.raw[1]) {
                    GV->player.y.raw[1] = 0x41;
                    GV->player.jmp = 0;
                }
            }
            update_player_position();
            if (0 == GV->player.jmp && (a & 0x01)) {
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
        } else if (GV->player.y.raw[1] != 0x40 && 0 == GV->player.jmp) {
            GV->player.y.raw[1] = 0x40;
            update_player_position();
        }

        // 水しぶき
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
    }
}