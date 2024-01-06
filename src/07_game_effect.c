#include "header.h"

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

void screen_effect_proc(uint8_t a) __z88dk_fastcall
{
    uint8_t i, j;
    for (i = 0; i < 16; i++) {
        // 水しぶき & けむり
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
        // ゴミ
        if (GV->dust[i].flag) {
            GV->dust[i].flag++;
            GV->dust[i].flag &= 0x1F;
            GV->dust[i].x.value += GV->dust[i].vx;
            GV->dust[i].y.value += GV->dust[i].vy;
            j = i;
            j += SP_DUST;
            if (0 == GV->dust[i].flag || 248 <= GV->dust[i].x.raw[1] || 192 <= GV->dust[i].y.raw[1]) {
                GV->dust[i].flag = 0;
                VGS0_ADDR_OAM[j].attr = 0x00;
            } else {
                GV->dust[i].vx += GV->dust[i].sx;
                GV->dust[i].vy += GV->dust[i].sy;
                VGS0_ADDR_OAM[j].x = GV->dust[i].x.raw[1];
                VGS0_ADDR_OAM[j].y = GV->dust[i].y.raw[1];
            }
        }
        // 星
        if (GV->star[i].flag) {
            GV->star[i].flag++;
            if (0 == (GV->star[i].flag & 0x03)) {
                GV->star[i].ptn += 1;
                if (GV->star[i].ptn < 0x78) {
                    VGS0_ADDR_BG->ptn[GV->star[i].y][GV->star[i].x] = GV->star[i].ptn;
                } else {
                    GV->star[i].flag = 0;
                    VGS0_ADDR_BG->ptn[GV->star[i].y][GV->star[i].x] = 0x00;
                }
            }
        }
        // 泡
        if (GV->bubble[i].flag) {
            GV->bubble[i].flag++;
            GV->bubble[i].flag &= 0x1F;
            if (0 == GV->bubble[i].flag) {
                VGS0_ADDR_BG->ptn[GV->bubble[i].y][GV->bubble[i].x] = 0x10;
                VGS0_ADDR_BG->attr[GV->bubble[i].y][GV->bubble[i].x] = 0x00;
            } else  if (0 == (GV->bubble[i].flag & 0x03)) {
                VGS0_ADDR_BG->ptn[GV->bubble[i].y][GV->bubble[i].x] += 1;
            }
        }
    }

    // 波のアニメーション
    a &= 0x1F;
    a >>= 2;
    a |= 0xA0;
    for (i = 0; i < 32; i++) {
        VGS0_ADDR_FG->ptn[9][i] = a;
    }
}