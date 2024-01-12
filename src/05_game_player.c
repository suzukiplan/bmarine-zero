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

void move_player() __z88dk_fastcall
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
    if (pad & VGS0_JOYPAD_T1) {
        if (0 == GV->player.jmpKeep && 0 == GV->player.jmp) {
            vgs0_se_play(4);
            GV->player.jmp = -543;
            GV->player.jmpKeep = 1;
            add_spray(GV->player.x.raw[1] + 12, GV->player.y.raw[1] + 5, 0x30, 0x83);
            add_spray(GV->player.x.raw[1] + 4, GV->player.y.raw[1] + 5, 0x30, 0xC3);
        }
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
    } else {
        GV->player.shot = 0;
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