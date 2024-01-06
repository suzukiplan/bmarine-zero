#include "header.h"

void add_pshot(uint8_t x, uint16_t y)
{
    if (GV->shot[GV->shotIndex].flag) {
        return;
    }
    vgs0_se_play(1);
    GV->shot[GV->shotIndex].flag = 1;
    GV->shot[GV->shotIndex].x = x;
    GV->shot[GV->shotIndex].y.value = y;
    GV->shot[GV->shotIndex].spd = 0;
    if (y < 0x4800) {
        GV->shot[GV->shotIndex].onair = 1;
    } else {
        GV->shot[GV->shotIndex].onair = 0;
    }
    uint8_t sn = GV->shotIndex;
    sn <<= 1;
    sn += SP_SHOT;
    vgs0_oam_set(sn, x, GV->shot[GV->shotIndex].y.raw[1], 0x80, 0x13);
    vgs0_oam_set(sn + 1, x, GV->shot[GV->shotIndex].y.raw[1] + 8, 0x80, 0x23);
    GV->shotIndex++;
    GV->shotIndex &= 0x07;
}

void move_pshot(void) __z88dk_fastcall
{
    uint8_t i, j;
    for (i = 0; i < 8; i++) {
        if (GV->shot[i].flag) {
            GV->shot[i].spd += 44;
            GV->shot[i].y.value += GV->shot[i].spd;
            j = i;
            j <<= 1;
            j += SP_SHOT;
            if (176 < GV->shot[i].y.raw[1]) {
                vgs0_se_play(3);
                GV->shot[i].flag = 0;
                VGS0_ADDR_OAM[j].attr = 0x00;
                VGS0_ADDR_OAM[j + 1].attr = 0x00;
                add_dust_ground(GV->shot[i].x, GV->shot[i].y.raw[1] + 7);
                add_dust_ground(GV->shot[i].x, GV->shot[i].y.raw[1] + 7);
                add_dust_ground(GV->shot[i].x, GV->shot[i].y.raw[1] + 7);
                add_dust_ground(GV->shot[i].x, GV->shot[i].y.raw[1] + 7);
            } else {
                GV->shot[i].flag++;
                VGS0_ADDR_OAM[j].y = GV->shot[i].y.raw[1];
                VGS0_ADDR_OAM[j + 1].y = GV->shot[i].y.raw[1] + 8;
                if (0 == (GV->shot[i].flag & 0x03)) {
                    add_spray(GV->shot[i].x, GV->shot[i].y.raw[1], 0x40, 0x80);
                }
                // 空中ショットの場合は着水判定
                if (GV->shot[i].onair) {
                    GV->shot[i].onair += 1;
                    if (0x44 < GV->shot[i].y.raw[1]) {
                        vgs0_se_play(6);
                        // 対空時間に比例して衝撃の水しぶきの間隔を広げる
                        j = GV->shot[i].onair >> 1;
                        add_spray(GV->shot[i].x - 5 - j, 0x46, 0x30, 0xC3);
                        add_spray(GV->shot[i].x + 5 + j, 0x46, 0x30, 0x83);
                        // 滞空時間に比例してバウンド（水の抵抗値）を大きくする
                        GV->shot[i].spd = GV->shot[i].onair;
                        GV->shot[i].spd <<= 4;
                        GV->shot[i].spd = -GV->shot[i].spd;
                        GV->shot[i].onair = 0;
                    }
                }
            }
        }
    }
}