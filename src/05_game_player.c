#include "header.h"

static void update_player_position(void) __z88dk_fastcall
{
    if (GV->player.x.raw[1] < 8) {
        GV->player.x.raw[1] = 8;
    } else if (224 < GV->player.x.raw[1]) {
        GV->player.x.raw[1] = 224;
    }
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

void move_player(void) __z88dk_fastcall
{
    uint8_t i;
    uint8_t pad = vgs0_joypad_get();

    // 左右の加速度処理
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

    // ジャンプ
    if (pad & VGS0_JOYPAD_T1 && 0 == GV->player.laser) {
        if (0 == GV->player.jmpKeep && 0 == GV->player.jmp) {
            vgs0_se_play(4);
            GV->player.jmp = -543;
            GV->player.jmpKeep = 1;
            add_spray(GV->player.x.raw[1] + 12, GV->player.y.raw[1] + 5, 0x30, 0x83);
            add_spray(GV->player.x.raw[1] + 4, GV->player.y.raw[1] + 5, 0x30, 0xC3);
        }
        GV->player.charge = 0;
    } else {
        GV->player.jmpKeep = 0;
    }

    // ショット発射
    if (pad & VGS0_JOYPAD_T2) {
        if (0 == GV->player.shot) {
            add_pshot(GV->player.x.raw[1] + 8, GV->player.y.value + 0x0B00);
            GV->player.shot = 1;
            GV->player.sa = 16;
        }
        if (0 == GV->player.jmp) {
            if (0xFF != GV->player.charge) {
                GV->player.charge++;
            }
        }
    } else {
        GV->player.shot = 0;
        if (60 < GV->player.charge && 0 == GV->player.jmp) {
            GV->player.laser = 255;
            GV->player.lcnt = 0;
        }
        GV->player.charge = 0;
    }

    // チャージ開始から10フレーム以降 & レーザー発射中は残像を描画
    if (0 == (GV->frame & 0x03)) {
        if (10 < GV->player.charge || GV->player.laser) {
            GV->player.zflag[GV->player.zindex] = 1;
            vgs0_oam_set(SP_ZANZO + GV->player.zindex, GV->player.x.raw[1], GV->player.y.raw[1], 0x87, 0x00, 2, 1);
            VGS0_ADDR_OAM[SP_ZANZO + GV->player.zindex].bank = BANK_LASER2_SP;
            GV->player.zindex++;
            GV->player.zindex &= 0x03;
        }
    }

    for (i = 0; i < 4; i++) {
        if (GV->player.zflag[i]) {
            GV->player.zflag[i]++;
            if (0 == (GV->player.zflag[i] & 0x03)) {
                VGS0_ADDR_OAM[SP_ZANZO + i].ptn += 3;
                if (12 == VGS0_ADDR_OAM[SP_ZANZO + i].ptn) {
                    GV->player.zflag[i] = 0;
                    VGS0_ADDR_OAM[SP_ZANZO + i].attr = 0x00;
                }
            }
        }
    }

    // 座標更新
    if (0 != GV->player.spd || 0 != GV->player.jmp) {
        if (0 != GV->player.jmp) {
            GV->player.x.value += GV->player.spd;
            GV->player.x.value += GV->player.spd / 2;
            GV->player.y.value += GV->player.jmp;
            GV->player.flight++;
            GV->player.y.value += GV->player.jmp;
            if (GV->player.jmpKeep) {
                GV->player.jmp += 23;
            } else {
                GV->player.jmp += 54;
            }
            if (0x41 < GV->player.y.raw[1]) {
                vgs0_se_play(5);
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

            if (8 < GV->player.x.raw[1] && GV->player.x.raw[1] < 224) {
                if (GV->frame & 0x01) {
                    i = GV->player.y.raw[1] + 5;
                    i += get_random(&GV->ridx) & 0x01;
                    i += get_random(&GV->ridx) & 0x01;
                    if (GV->player.spd < 0) {
                        add_spray(GV->player.x.raw[1] + 16, i, 0x30, 0x83);
                    } else {
                        add_spray(GV->player.x.raw[1], i, 0x30, 0xC3);
                    }
                } else if (0 == (GV->frame & 3)) {
                    vgs0_se_play(2);
                }
            }
        }
        update_player_position();
    } else if (GV->player.y.raw[1] != 0x40) {
        GV->player.y.raw[1] = 0x40;
        update_player_position();
    }

    // レーザー
    if (GV->player.laser) {
        VGS0_ADDR_OAM[SP_LASER].x = GV->player.x.raw[1] + 4;
        VGS0_ADDR_OAM[SP_LASER].y = GV->player.y.raw[1] + 16;
        if (GV->player.lcnt < 13) {
            VGS0_ADDR_OAM[SP_LASER].widthMinus1 = 1;
            VGS0_ADDR_OAM[SP_LASER].heightMinus1 = GV->player.lcnt;
            VGS0_ADDR_OAM[SP_LASER].attr = 0x87;
            VGS0_ADDR_OAM[SP_LASER].ptn = 0x00;
            VGS0_ADDR_OAM[SP_LASER].bank = BANK_LASER_SP;
        } else if (GV->player.lcnt < 16) {
            ;
        } else if (GV->player.lcnt < 32) {
            VGS0_ADDR_OAM[SP_LASER].ptn = ((GV->player.lcnt - 16) >> 2) << 1;
        } else if (GV->player.lcnt < 152) {
            GV->player.lhit = 1;
            VGS0_ADDR_OAM[SP_LASER].ptn = 8 + (GV->player.lcnt & 0x06);
        } else {
            GV->player.lhit = 0;
            GV->player.laser = 0;
            VGS0_ADDR_OAM[SP_LASER].attr = 0;
        }
        GV->player.lcnt++;
    }

    // ショット発射アニメーション
    if (GV->player.sa) {
        GV->player.sa--;
        if (GV->player.sa) {
            i = GV->frame;
            i >>= 1;
            i &= 0x03;
            i <<= 1;
            i += 0x48;
            vgs0_oam_set(6, GV->player.x.raw[1] + 3, GV->player.y.raw[1] + 7, 0x84, i, 1, 1);
        } else {
            VGS0_ADDR_OAM[6].attr = 0;
        }
    }

    // 着水時の衝撃波
    if (GV->player.snock) {
        GV->player.snock--;
        add_spray(GV->player.slx, GV->player.y.raw[1] + 5, 0x30, 0xC3);
        add_spray(GV->player.srx, GV->player.y.raw[1] + 5, 0x30, 0x83);
        GV->player.slx -= 6;
        GV->player.srx += 6;
    }
}