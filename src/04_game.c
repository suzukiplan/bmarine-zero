#include "header.h"

#define SP_DUST 208
#define SP_SHOT 224
#define SP_SPRAY 240

void update_player_position(void)
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

    uint8_t r;

    r = random[GV->ridx] & 0x1F;
    GV->ridx++;
    GV->dust[GV->dustIndex].sx = r;

    r = random[GV->ridx];
    GV->ridx++;
    if (GV->dustIndex & 0x01) {
        GV->dust[GV->dustIndex].sx = -GV->dust[GV->dustIndex].sx;
    }

    r = random[GV->ridx] & 0x1F;
    GV->ridx++;
    GV->dust[GV->dustIndex].sy = r;

    r = random[GV->ridx];
    GV->ridx++;
    if (r < 0xC0) {
        GV->dust[GV->dustIndex].sy = -GV->dust[GV->dustIndex].sy;
    }

    r = random[GV->ridx];
    GV->ridx++;
    GV->dust[GV->dustIndex].vy = r;
    GV->dust[GV->dustIndex].vy <<= 1;
    GV->dust[GV->dustIndex].vy = -GV->dust[GV->dustIndex].vy;

    uint8_t s = GV->dustIndex;
    s += SP_DUST;
    vgs0_oam_set(s, x, y, 0x83, 0x14);
    GV->dustIndex++;
}

void add_star()
{
    if (GV->star[GV->starIndex].flag) {
        return;
    }
    uint8_t r = random[GV->ridx];
    GV->ridx += 1;
    uint8_t x = r & 0x1F;
    r = random[GV->ridx];
    GV->ridx += 1;
    uint8_t y = r & 0x07;
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

void add_bubble()
{
    if (GV->bubble[GV->bubbleIndex].flag) {
        return;
    }
    uint8_t r;

    r = random[GV->ridx];
    GV->ridx += 1;
    uint8_t y = r & 0x1F;
    if (y < 9 || 22 < y) {
        return;
    }

    r = random[GV->ridx];
    GV->ridx += 1;
    uint8_t x = r & 0x1F;
    if (0 == x || 31 == x) {
        return;
    }

    if (VGS0_ADDR_BG->ptn[y][x] != 0x10) {
        return;
    }
    VGS0_ADDR_BG->ptn[y][x] = 0x90;
    VGS0_ADDR_BG->attr[y][x] = 0x84;
    GV->bubble[GV->bubbleIndex].flag = 1;
    GV->bubble[GV->bubbleIndex].x = x;
    GV->bubble[GV->bubbleIndex].y = y;
    GV->bubble[GV->bubbleIndex].ptn = 0x90;
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
    GV->dustIndex = 0;
    GV->starIndex = 0;
    GV->bubbleIndex = 0;
    for (i = 0; i < 16; i++) {
        GV->spray[i].sn = 0;
        GV->dust[i].flag = 0;
        GV->star[i].flag = 0;
        GV->bubble[i].flag = 0;
    }
    for (i = 0; i < 8; i++) {
        GV->shot[i].flag = 0;
    }

    // メインループ
    while (1) {
        vgs0_wait_vsync();
        a++;

        // 追加可能なら星と泡を追加
        add_star();
        add_bubble();

        // プレイヤーショットの移動
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
                            vgs0_se_play(5);
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
                vgs0_se_play(4);
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
                } else if (0 == (a & 3)) {
                    vgs0_se_play(2);
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
                    GV->bubble[i].ptn += 1;
                    VGS0_ADDR_BG->ptn[GV->bubble[i].y][GV->bubble[i].x] = GV->bubble[i].ptn;
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