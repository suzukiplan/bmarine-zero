#include "header.h"

void update_player_position(void)  __z88dk_fastcall
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

void add_spray(uint8_t x, uint8_t y, uint8_t sn, uint8_t attr)
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

void add_dust_ground(uint8_t x, uint8_t y)
{
    GV->dustIndex &= 0x0F;
    GV->dust[GV->dustIndex].flag = 1;
    GV->dust[GV->dustIndex].x.raw[1] = x;
    GV->dust[GV->dustIndex].y.raw[1] = y;
    GV->dust[GV->dustIndex].vx = 0;

    uint8_t r = get_random(&GV->ridx) & 0x1F;
    GV->dust[GV->dustIndex].sx = r;

    if (GV->dustIndex & 0x01) {
        GV->dust[GV->dustIndex].sx = -GV->dust[GV->dustIndex].sx;
    }

    r = get_random(&GV->ridx) & 0x1F;
    GV->dust[GV->dustIndex].sy = r;

    r = get_random(&GV->ridx);
    if (r < 0xC0) {
        GV->dust[GV->dustIndex].sy = -GV->dust[GV->dustIndex].sy;
    }

    r = get_random(&GV->ridx);
    GV->dust[GV->dustIndex].vy = r;
    GV->dust[GV->dustIndex].vy <<= 1;
    GV->dust[GV->dustIndex].vy = -GV->dust[GV->dustIndex].vy;

    uint8_t s = GV->dustIndex;
    s += SP_DUST;
    vgs0_oam_set(s, x, y, 0x83, 0x14);
    GV->dustIndex++;
}

void add_star(void) __z88dk_fastcall
{
    if (GV->star[GV->starIndex].flag) {
        return;
    }
    uint8_t x = get_random(&GV->ridx) & 0x1F;
    uint8_t y = get_random(&GV->ridx) & 0x07;
    if (VGS0_ADDR_BG->ptn[y][x] != 0) {
        return;
    }
    VGS0_ADDR_BG->ptn[y][x] = 0x70;
    VGS0_ADDR_BG->attr[y][x] = 0x84;
    GV->star[GV->starIndex].flag = 1;
    GV->star[GV->starIndex].x = x;
    GV->star[GV->starIndex].y = y;
    GV->star[GV->starIndex].ptn = 0x70;
    GV->starIndex += 1;
    GV->starIndex &= 0x0F;
}

void add_bubble(void)  __z88dk_fastcall
{
    if (GV->bubble[GV->bubbleIndex].flag) {
        return;
    }
    uint8_t y = get_random(&GV->ridx) & 0x1F;
    if (y < 9 || 22 < y) {
        return;
    }

    uint8_t x = get_random(&GV->ridx) & 0x1F;
    if (0 == x || 31 == x) {
        return;
    }

    if (VGS0_ADDR_BG->ptn[y][x] != 0x10) {
        return;
    }
    if (GV->bubbleIndex == 0x0F || GV->bubbleIndex == 0x07) {
        VGS0_ADDR_BG->ptn[y][x] = 0xA0;
    } else {
        VGS0_ADDR_BG->ptn[y][x] = 0x90;
    }
    VGS0_ADDR_BG->attr[y][x] = 0x84;
    GV->bubble[GV->bubbleIndex].flag = 1;
    GV->bubble[GV->bubbleIndex].x = x;
    GV->bubble[GV->bubbleIndex].y = y;
    GV->bubbleIndex += 1;
    GV->bubbleIndex &= 0x0F;
}

void add_shot(uint8_t x, uint16_t y)
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
