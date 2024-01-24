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

static void put_number(uint8_t x, uint8_t y, uint16_t num)
{
    uint8_t n[5];
    uint8_t i;
    for (i = 4; i != 0xFF; i--) {
        n[i] = (uint8_t)vgs0_mod16(num, 10);
        num = vgs0_div16(num, 10);
    }
    uint8_t detectNotZero = 0;
    for (i = 0; i < 5; i++, x++) {
        if (detectNotZero || 4 == i || 0 != n[i]) {
            detectNotZero = 1;
            VGS0_ADDR_BG->ptn[y][x] = '0' + n[i];
        } else {
            VGS0_ADDR_BG->ptn[y][x] = ' ';
        }
    }
}

static uint8_t put_percent(uint8_t x, uint8_t y, uint16_t n1, uint16_t n2)
{
    uint8_t i;
    uint32_t v32 = 0;
    for (i = 0; i < 100; i++) {
        v32 += n1;
    }
    uint8_t n8 = 0;
    while (n2 <= v32) {
        n8++;
        v32 -= n2;
    }
    uint8_t result = n8;

    uint8_t n[3];
    for (i = 2; i != 0xFF; i--) {
        n[i] = (uint8_t)vgs0_mod(n8, 10);
        n8 = (uint8_t)vgs0_div(n8, 10);
    }
    uint8_t detectNotZero = 0;
    for (i = 0; i < 3; i++) {
        if (detectNotZero || 2 == i || 0 != n[i]) {
            detectNotZero = 1;
            VGS0_ADDR_BG->ptn[y][x] = '0' + n[i];
            x++;
        }
    }
    VGS0_ADDR_BG->ptn[y][x] = '%';
    return result;
}

static void rank_up(int8_t* rank, uint8_t up, uint8_t limit)
{
    while (up && (*rank) < 26 && limit) {
        (*rank)++;
        up--;
        limit--;
    }
}

static void rank_down(int8_t* rank, uint8_t down)
{
    while (down && 0 < (*rank)) {
        (*rank)--;
        down--;
    }
}

void show_result(void) __z88dk_fastcall
{
    uint8_t y = 1;
    uint8_t per, i;
    uint32_t v32;
    int8_t rank = (int8_t)GV->level - 1;
    const char* ranks[] = {
        "E1",
        "       SEAMAN RECRUIT       ",
        "E2",
        "      SEAMAN APPRENTICE     ",
        "E3",
        "           SEAMAN           ",
        "E4",
        " PETTY OFFICER THIRD CLASS  ",
        "E5",
        " PETTY OFFICER SECOND CLASS ",
        "E6",
        " PETTY OFFICER FIRST CLASS  ",
        "E7",
        "    CHIEF PETTY OFFICER     ",
        "E8",
        " SENIOR CHIEF PETTY OFFICER ",
        "E9",
        " MASTER CHIEF PETTY OFFICER ",
        "W1",
        "    WARRANT OFFICER ONE     ",
        "W2",
        " CHIEF WARRANT OFFICER TWO  ",
        "W3",
        "CHIEF WARRANT OFFICER THREE ",
        "W4",
        " CHIEF WARRANT OFFICER FOUR ",
        "W5",
        " CHIEF WARRANT OFFICER FIVE ",
        "O1",
        "           ENSIGN           ",
        "O2",
        "  LIEUTENANT JUNIOR GRADE   ",
        "O2",
        "  LIEUTENANT JUNIOR GRADE+  ",
        "O3",
        "         LIEUTENANT         ",
        "O4",
        "    LIEUTENANT COMMANDER    ",
        "O5",
        "         COMMANDER          ",
        "O6",
        "          CAPTAIN           ",
        "O7",
        "  REAR ADMIRAL, LOWER HALF  ",
        "O8",
        "  REAR ADMIRAL, UPPER HALF  ",
        "O9",
        "        VICE ADMIRAL        ",
        "OA",
        "          ADMIRAL           ",
        "OB",
        "       FLEET ADMIRAL        ",
    };

    vgs0_memset(0x8000, 0x00, 0x1800);
    *VGS0_ADDR_BG_DPM = BANK_MAIN_BG;
    *VGS0_ADDR_FG_DPM = BANK_MAIN_FG;
    *VGS0_ADDR_SPRITE_DPM = BANK_BRAND1_SP;
    *VGS0_ADDR_FG_SCROLL_X = 0;
    *VGS0_ADDR_FG_SCROLL_Y = 0;
    *VGS0_ADDR_BG_SCROLL_X = 0;
    *VGS0_ADDR_BG_SCROLL_Y = 0;

    vgs0_bg_putstr(2, y, 0x80, "-YOUR BATTLE MARINE RESULTS-");
    vgs0_wait_vsync();

    y += 2;
    vgs0_bg_putstr(2, y, 0x80, "     SHOTS FIRED:");
    put_number(20, y, GV->st.shot);
    rank_up(&rank, GV->st.shot / 1024, 10);
    vgs0_wait_vsync();

    y++;
    vgs0_bg_putstr(2, y, 0x80, "    MISSED SHOTS:");
    put_number(20, y, GV->st.miss);
    per = put_percent(26, y, GV->st.miss, GV->st.shot);
    rank_down(&rank, per / 5);
    vgs0_wait_vsync();

    y++;
    vgs0_bg_putstr(2, y, 0x80, "     LASER FIRED:");
    put_number(20, y, GV->st.laser);
    vgs0_wait_vsync();

    y++;
    vgs0_bg_putstr(2, y, 0x80, "   MAXIMUM COMBO:");
    put_number(20, y, GV->st.maxhit);
    rank_up(&rank, GV->st.maxhit / 100, 16);
    vgs0_wait_vsync();

    y += 2;
    vgs0_bg_putstr(2, y, 0x80, "APPEARED ENEMIES:");
    v32 = GV->st.e[ET_MARINE_LR];
    v32 += GV->st.e[ET_MARINE_RL];
    v32 += GV->st.e[ET_FISH];
    v32 += GV->st.e[ET_KANI];
    if (65536 <= v32) {
        v32 = 65535;
    }
    uint16_t appearedEnemies = (uint16_t)v32;
    rank_up(&rank, appearedEnemies / 512, 3);
    put_number(20, y, appearedEnemies);
    vgs0_wait_vsync();

    y++;
    vgs0_bg_putstr(2, y, 0x80, "DEFEATED ENEMIES:");
    v32 = GV->st.d[ET_MARINE_LR];
    v32 += GV->st.d[ET_MARINE_RL];
    v32 += GV->st.d[ET_FISH];
    v32 += GV->st.d[ET_KANI];
    if (65536 <= v32) {
        v32 = 65535;
    }
    put_number(20, y, (uint16_t)v32);
    per = put_percent(26, y, (uint16_t)v32, appearedEnemies);
    rank_up(&rank, per / 20, 5);
    vgs0_wait_vsync();

    y++;
    vgs0_bg_putstr(2, y, 0x80, "    DAMAGE COUNT:");
    put_number(20, y, GV->st.dmg);
    rank_down(&rank, GV->st.dmg / 4);
    vgs0_wait_vsync();

    y += 2;
    vgs0_bg_putstr(2, y, 0x80, " APPEARED MEDALS:");
    v32 = GV->st.medal[0];
    v32 += GV->st.medal[1];
    if (65536 <= v32) {
        v32 = 65535;
    }
    uint16_t appearedMedals = (uint16_t)v32;
    put_number(20, y, appearedMedals);
    rank_up(&rank, appearedMedals / 512, 5);
    vgs0_wait_vsync();

    y++;
    vgs0_bg_putstr(2, y, 0x80, "     LOST MEDALS:");
    v32 = GV->st.lost[0];
    v32 += GV->st.lost[1];
    if (65536 <= v32) {
        v32 = 65535;
    }
    put_number(20, y, (uint16_t)v32);
    per = put_percent(26, y, (uint16_t)v32, appearedMedals);
    rank_down(&rank, per / 2);
    vgs0_wait_vsync();

    y++;
    vgs0_bg_putstr(2, y, 0x80, "      CURE TIMES:");
    put_number(20, y, GV->st.cure);
    rank_down(&rank, GV->st.cure / 256);
    vgs0_wait_vsync();

    y++;
    vgs0_bg_putstr(2, y, 0x80, "   BASE POINT UP:");
    rank_up(&rank, GV->st.sup / 256, 16);
    put_number(20, y, GV->st.sup);

    // ランクをレベル未満にはしない
    if (rank < (int8_t)GV->level - 1) {
        rank = (int8_t)GV->level - 1;
    }
    for (i = 0; i < 44; i++) {
        vgs0_wait_vsync();
    }

    y += 2;
    vgs0_bg_putstr(8, y, 0x80, "YOUR RANK IS XX!");
    uint8_t bank = BANK_RANK1_SP;
    if (0x10 <= rank) {
        bank++;
    }
    uint8_t ptn = (rank & 0x08) ? 0x08 : 0x00;
    ptn += (rank & 0x07) << 5;
    vgs0_oam_set(0, 96, (y << 3) + 12, 0x87, ptn, 7, 1);
    VGS0_ADDR_OAM[0].bank = bank;
    rank <<= 1;
    vgs0_bg_putstr(21, y, 0x80, ranks[rank]);
    vgs0_bg_putstr(2, y + 4, 0x80, ranks[rank | 1]);
    GV->player.rank[0] = ranks[rank][0];
    GV->player.rank[1] = ranks[rank][1];
    vgs0_wait_vsync();

    y += 6;
    vgs0_fg_putstr(7, y, 0x80, "PRESS START BUTTON");

    uint8_t aa = 0;
    uint8_t pad = 0;
    while (1) {
        pad = vgs0_joypad_get();
        if (0 != (pad & VGS0_JOYPAD_ST)) {
            break;
        }
        aa++;
        vgs0_wait_vsync();
        switch (aa & 0x3F) {
            case 0:
                for (i = 0; i < 32; i++) {
                    VGS0_ADDR_FG->attr[y][i] = 0x80;
                }
                break;
            case 24:
                for (i = 0; i < 32; i++) {
                    VGS0_ADDR_FG->attr[y][i] = 0x81;
                }
                break;
            case 32:
                for (i = 0; i < 32; i++) {
                    VGS0_ADDR_FG->attr[y][i] = 0x00;
                }
                break;
            case 56:
                for (i = 0; i < 32; i++) {
                    VGS0_ADDR_FG->attr[y][i] = 0x81;
                }
                break;
        }
    }
    while (0 != (vgs0_joypad_get() & VGS0_JOYPAD_ST)) {
        ;
    }
    vgs0_se_play(23);
    for (aa = 0; aa < 16; aa++) {
        ptn = aa | 0xE0;
        for (i = 0; i < 32; i++) {
            for (uint8_t j = 0; j < 32; j++) {
                VGS0_ADDR_FG->ptn[j][i] = ptn;
                VGS0_ADDR_FG->attr[j][i] = 0x82;
            }
        }
        vgs0_wait_vsync();
        vgs0_wait_vsync();
    }
    vgs0_memset(0x8000, 0x00, 0x1800);
}
