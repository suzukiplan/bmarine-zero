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

void add_spray(uint8_t x, uint8_t y, uint8_t sn, uint8_t attr)
{
    GV->spray[GV->sprayIndex].sn = sn;
    GV->spray[GV->sprayIndex].t = 0;
    vgs0_oam_set(SP_SPRAY + GV->sprayIndex, x, y, attr, sn, 0, 0);
    GV->sprayIndex += 1;
    GV->sprayIndex &= 0x0F;
}

void add_spray2(uint8_t x, uint8_t y, uint8_t sn, uint8_t attr)
{
    GV->spray2[GV->spray2Index].sn = sn;
    GV->spray2[GV->spray2Index].t = 0;
    vgs0_oam_set(SP_SPRAY2 + GV->spray2Index, x, y, attr, sn, 0, 0);
    GV->spray2Index += 1;
    GV->spray2Index &= 0x0F;
}

void add_dust_ground(uint8_t x, uint8_t y)
{
    GV->dust[GV->dustIndex].flag = 1;
    GV->dust[GV->dustIndex].x.raw[1] = x;
    GV->dust[GV->dustIndex].y.raw[1] = y;
    GV->dust[GV->dustIndex].vx = 0;
    GV->dust[GV->dustIndex].sx = get_random(&GV->ridx) & 0x1F;
    if (GV->dustIndex & 0x01) {
        GV->dust[GV->dustIndex].sx = -GV->dust[GV->dustIndex].sx;
    }
    GV->dust[GV->dustIndex].sy = get_random(&GV->ridx) & 0x1F;
    if (get_random(&GV->ridx) < 0xC0) {
        GV->dust[GV->dustIndex].sy = -GV->dust[GV->dustIndex].sy;
    }
    GV->dust[GV->dustIndex].vy = get_random(&GV->ridx);
    GV->dust[GV->dustIndex].vy <<= 1;
    GV->dust[GV->dustIndex].vy = -GV->dust[GV->dustIndex].vy;

    vgs0_oam_set(SP_DUST + GV->dustIndex, x, y, 0x83, 0x14, 0, 0);
    GV->dustIndex++;
    GV->dustIndex &= 0x0F;
}

void add_dust_air(uint8_t x, uint8_t y)
{
    GV->dust[GV->dustIndex].flag = 1;
    GV->dust[GV->dustIndex].x.raw[1] = x;
    GV->dust[GV->dustIndex].y.raw[1] = y;
    GV->dust[GV->dustIndex].vx = 0;

    GV->dust[GV->dustIndex].sx = get_random(&GV->ridx) & 0x1F;
    if (get_random(&GV->ridx) < 0x80) {
        GV->dust[GV->dustIndex].sx = -GV->dust[GV->dustIndex].sx;
    }
    GV->dust[GV->dustIndex].sy = get_random(&GV->ridx) & 0x1F;
    if (get_random(&GV->ridx) < 0x80) {
        GV->dust[GV->dustIndex].sy = -GV->dust[GV->dustIndex].sy;
    }
    GV->dust[GV->dustIndex].vx = get_random(&GV->ridx);
    GV->dust[GV->dustIndex].vy = get_random(&GV->ridx);
    switch (GV->dustIndex & 3) {
        case 0: vgs0_oam_set(SP_DUST + GV->dustIndex, x, y, 0x83, 0x14, 0, 0); break;
        case 1: vgs0_oam_set(SP_DUST + GV->dustIndex, x, y, 0x80, 0x40, 0, 0); break;
        case 2: vgs0_oam_set(SP_DUST + GV->dustIndex, x, y, 0x80, 0x50, 0, 0); break;
        case 3: vgs0_oam_set(SP_DUST + GV->dustIndex, x, y, 0x84, 0x60, 0, 0); break;
    }
    GV->dustIndex++;
    GV->dustIndex &= 0x0F;
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

void add_bubble(void) __z88dk_fastcall
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

void add_medal(uint8_t type, uint8_t x, uint8_t y)
{
    if (GV->medal[GV->medalIndex].flag) return;
    if (GV->st.medal[type] != 0xFFFF) {
        GV->st.medal[type]++;
    }
    vgs0_memset((uint16_t)&GV->medal[GV->medalIndex], 0, sizeof(Medal));
    GV->medal[GV->medalIndex].flag = 1;
    GV->medal[GV->medalIndex].type = type;
    GV->medal[GV->medalIndex].x.raw[1] = x;
    GV->medal[GV->medalIndex].y.raw[1] = y;
    if (0 == type) {
        vgs0_oam_set(SP_MEDAL + GV->medalIndex, x, y, 0x86, 0x00, 1, 1);
    } else {
        vgs0_oam_set(SP_MEDAL + GV->medalIndex, x, y, 0x89, 0x00, 1, 1);
    }
    VGS0_ADDR_OAM[SP_MEDAL + GV->medalIndex].bank = BANK_MEDAL_SP;
    GV->medalIndex += 1;
    GV->medalIndex &= 0x0F;
}

void add_catk(uint8_t x, uint8_t y)
{
    if (GV->catk[GV->catkIndex].flag) {
        return;
    }
    GV->catk[GV->catkIndex].flag = 1;
    GV->catk[GV->catkIndex].x.raw[1] = x;
    GV->catk[GV->catkIndex].y.raw[1] = y;
    uint8_t angle = vgs0_angle(x + 4, y + 4, GV->player.x.raw[1] + 12, GV->player.y.raw[1] + 8);
    GV->catk[GV->catkIndex].rx = vgs0_sin(angle);
    GV->catk[GV->catkIndex].ry = vgs0_cos(angle);
    angle >>= 3;
    vgs0_oam_set(SP_CATK + GV->catkIndex, x, y, 0x80, angle, 0, 0);
    VGS0_ADDR_OAM[SP_CATK + GV->catkIndex].bank = BANK_CATK;
    GV->catkIndex += 1;
    GV->catkIndex &= 0x0F;
}

void screen_effect_proc(void) __z88dk_fastcall
{
    uint8_t i;
    for (i = 0; i < 16; i++) {
        // 水しぶき & けむり
        if (GV->spray[i].sn) {
            GV->spray[i].t += 1;
            if (0 == (GV->spray[i].t & 1)) {
                GV->spray[i].sn += 1;
            }
            if (16 == GV->spray[i].t) {
                GV->spray[i].sn = 0;
                VGS0_ADDR_OAM[SP_SPRAY + i].attr = 0x00;
            } else {
                VGS0_ADDR_OAM[SP_SPRAY + i].ptn = GV->spray[i].sn;
            }
        }
        // うんこの軌道
        if (GV->spray2[i].sn) {
            GV->spray2[i].t += 1;
            if (0 == (GV->spray2[i].t & 7)) {
                GV->spray2[i].sn += 1;
            }
            if (64 == GV->spray2[i].t) {
                GV->spray2[i].sn = 0;
                VGS0_ADDR_OAM[SP_SPRAY2 + i].attr = 0x00;
            } else {
                VGS0_ADDR_OAM[SP_SPRAY2 + i].ptn = GV->spray2[i].sn;
            }
        }
        // ゴミ
        if (GV->dust[i].flag) {
            GV->dust[i].flag++;
            GV->dust[i].flag &= 0x1F;
            GV->dust[i].x.value += GV->dust[i].vx;
            GV->dust[i].y.value += GV->dust[i].vy;
            if (0 == GV->dust[i].flag || 248 <= GV->dust[i].x.raw[1] || 192 <= GV->dust[i].y.raw[1]) {
                GV->dust[i].flag = 0;
                VGS0_ADDR_OAM[SP_DUST + i].attr = 0x00;
            } else {
                GV->dust[i].vx += GV->dust[i].sx;
                GV->dust[i].vy += GV->dust[i].sy;
                VGS0_ADDR_OAM[SP_DUST + i].x = GV->dust[i].x.raw[1];
                VGS0_ADDR_OAM[SP_DUST + i].y = GV->dust[i].y.raw[1];
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
            } else if (0 == (GV->bubble[i].flag & 0x03)) {
                VGS0_ADDR_BG->ptn[GV->bubble[i].y][GV->bubble[i].x] += 1;
            }
        }
        // 打ち返し弾
        if (GV->catk[i].flag) {
            GV->catk[i].vx.value += GV->catk[i].rx;
            GV->catk[i].vy.value += GV->catk[i].ry;
            GV->catk[i].x.value += vgs0_sdiv16((int16_t)GV->catk[i].vx.value, 13);
            GV->catk[i].y.value += vgs0_sdiv16((int16_t)GV->catk[i].vy.value, 13);
            if (248 < GV->catk[i].x.raw[1] || 192 < GV->catk[i].y.raw[1]) {
                GV->catk[i].flag = 0;
                GV->catk[i].vx.value = 0;
                GV->catk[i].vy.value = 0;
                VGS0_ADDR_OAM[SP_CATK + i].attr = 0x00;
            } else {
                VGS0_ADDR_OAM[SP_CATK + i].x = GV->catk[i].x.raw[1];
                VGS0_ADDR_OAM[SP_CATK + i].y = GV->catk[i].y.raw[1];
                if (0 == GV->player.dmg && 0 == GV->player.muteki) {
                    GV->hbuf[0].x = GV->catk[i].x.raw[1] + 3;
                    GV->hbuf[0].y = GV->catk[i].y.raw[1] + 3;
                    GV->hbuf[0].width = 2;
                    GV->hbuf[0].height = 2;
                    GV->hbuf[1].x = GV->player.x.raw[1] + 10;
                    GV->hbuf[1].y = GV->player.y.raw[1] + 8;
                    GV->hbuf[1].width = 4;
                    GV->hbuf[1].height = 4;
                    if (vgs0_collision_check((uint16_t)GV->hbuf)) {
                        GV->hit = 0;
                        GV->player.dmg = 60;
                    }
                }
            }
        }
        // メダル
        if (GV->medal[i].flag) {
            if (0x20 != GV->medal[i].flag) {
                if (1 == GV->medal[i].flag) {
                    if (0 == GV->player.dead && (100 < GV->hit || GV->player.jmp || GV->player.mode)) {
                        GV->medal[i].vx.value = 0;
                        GV->medal[i].vy.value = 0;
                        GV->medal[i].flag = 0x10; // 自動収集
                    } else {
                        GV->medal[i].vx.value = 0;
                        if (((int8_t)GV->medal[i].vy.raw[1]) < 1) {
                            GV->medal[i].vy.value -= 3;
                        }
                        if (0 == (GV->frame & 0x03)) {
                            GV->medal[i].an += 2;
                            GV->medal[i].an &= 0x07;
                            VGS0_ADDR_OAM[SP_MEDAL + i].ptn = GV->medal[i].an;
                        }
                    }
                } else if (0x10 == GV->medal[i].flag) {
                    if (GV->player.dead) {
                        GV->medal[i].vx.value = 0;
                        GV->medal[i].vy.value = 0;
                        GV->medal[i].flag = 0x01; // 自動収集解除
                    } else {
                        uint8_t r = vgs0_angle(GV->medal[i].x.raw[1] + 8, GV->medal[i].y.raw[1] + 8, GV->player.x.raw[1] + 12, GV->player.y.raw[1] + 8);
                        GV->medal[i].vx.value = (uint16_t)((int8_t)vgs0_sin(r));
                        GV->medal[i].vy.value = (uint16_t)((int8_t)vgs0_cos(r));
                        GV->medal[i].vx.value += GV->medal[i].vx.value;
                        GV->medal[i].vy.value += GV->medal[i].vy.value;
                        GV->medal[i].vx.value += GV->medal[i].vx.value;
                        GV->medal[i].vy.value += GV->medal[i].vy.value;
                        if (0 == (GV->frame & 0x02)) {
                            GV->medal[i].an += 2;
                            GV->medal[i].an &= 0x1F;
                            if (GV->medal[i].an < 0x10) {
                                VGS0_ADDR_OAM[SP_MEDAL + i].ptn = GV->medal[i].an + 0x20;
                            } else {
                                VGS0_ADDR_OAM[SP_MEDAL + i].ptn = (GV->medal[i].an & 0x0F) + 0x40;
                            }
                        }
                    }
                }
                GV->medal[i].x.value += GV->medal[i].vx.value;
                GV->medal[i].y.value += GV->medal[i].vy.value;
                if (192 < GV->medal[i].y.raw[1] && GV->medal[i].y.raw[1] < 248) {
                    if (GV->st.lost[GV->medal[i].type] != 0xFFFF) {
                        GV->st.lost[GV->medal[i].type]++;
                    }
                    GV->medal[i].flag = 0;
                    VGS0_ADDR_OAM[SP_MEDAL + i].attr = 0x00;
                } else {
                    GV->hbuf[0].x = GV->player.x.raw[1];
                    GV->hbuf[0].y = GV->player.y.raw[1];
                    GV->hbuf[0].width = 24;
                    GV->hbuf[0].height = 16;
                    GV->hbuf[1].x = GV->medal[i].x.raw[1];
                    GV->hbuf[1].y = GV->medal[i].y.raw[1];
                    GV->hbuf[1].width = 16;
                    GV->hbuf[1].height = 16;
                    if (0 == GV->player.dead && vgs0_collision_check((uint16_t)GV->hbuf)) {
                        vgs0_se_play(10);
                        GV->hkt = HIT_KEEP_TIME;
                        GV->medal[i].flag = 0x20;
                        GV->medal[i].an = 0;
                        VGS0_ADDR_OAM[SP_MEDAL + i].ptn = 0x60;
                        if (0 == GV->medal[i].type) {
                            if (GV->player.hp < 80) {
                                GV->player.hp++;
                                if (GV->st.cure != 0xFFFF) {
                                    GV->st.cure++;
                                }
                            } else {
                                // メダル基礎得点上昇
                                GV->smc.value++;
                                if (0 == GV->smc.value) {
                                    GV->smc.value = 0xFFFF;
                                }
                                if (GV->st.sup != 0xFFFF) {
                                    GV->st.sup++;
                                }
                            }
                        } else {
                            GV->sadd[1] += 5 + (GV->smc.raw[0] & 0x0F);
                            GV->sadd[2] += ((GV->smc.raw[0] & 0xF0) >> 4);
                            if (GV->smc.raw[1]) {
                                GV->sadd[3] += GV->smc.raw[1] & 0x0F;
                                GV->sadd[4] += ((GV->smc.raw[1] & 0xF0) >> 4);
                            }
                        }
                    } else {
                        VGS0_ADDR_OAM[SP_MEDAL + i].x = GV->medal[i].x.raw[1];
                        VGS0_ADDR_OAM[SP_MEDAL + i].y = GV->medal[i].y.raw[1];
                    }
                }
            } else {
                // 取得アニメーション
                if (0 == (GV->frame & 0x01)) {
                    GV->medal[i].an += 2;
                    if (GV->medal[i].an < 32) {
                        if (GV->medal[i].an < 0x10) {
                            VGS0_ADDR_OAM[SP_MEDAL + i].ptn = GV->medal[i].an + 0x60;
                        } else {
                            VGS0_ADDR_OAM[SP_MEDAL + i].ptn = (GV->medal[i].an & 0x0F) + 0x80;
                        }
                    } else if (48 == GV->medal[i].an) {
                        GV->medal[i].flag = 0;
                        VGS0_ADDR_OAM[SP_MEDAL + i].attr = 0x00;
                    }
                }
            }
        }
    }

    // 波のアニメーション
    uint8_t a = GV->frame;
    a &= 0x1F;
    a >>= 2;
    a |= 0xA0;
    for (i = 0; i < 32; i++) {
        VGS0_ADDR_FG->ptn[9][i] = a;
    }
}