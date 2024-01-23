#include "header.h"

static void put_score_data(NameTable* namtbl, uint8_t x, uint8_t y, uint8_t rank, uint8_t isMyRank, ScoreData* data)
{
    if (data->exist) {
        vgs0_putstr(namtbl, x, y, isMyRank ? 0x80 : 0x81, "0  ***  **         00");
        namtbl->ptn[y][x + 3] = data->name[0];
        namtbl->ptn[y][x + 4] = data->name[1];
        namtbl->ptn[y][x + 5] = data->name[2];
        namtbl->ptn[y][x + 8] = data->lv[0];
        namtbl->ptn[y][x + 9] = data->lv[1];
        uint8_t show = 0;
        for (uint8_t j = 0; j < 8; j++) {
            if (0 == show && 0 != data->sc[7 - j]) {
                show = 1;
            }
            if (show) {
                namtbl->ptn[y][x + 12 + j] = '0' + data->sc[7 - j];
            }
        }
    } else {
        vgs0_putstr(namtbl, x, y, 0x81, "0  ***  **   NO ENTRY");
    }
    namtbl->ptn[y][x] = '0' + rank;
}

void print_score_ranking(NameTable* nametbl) __z88dk_fastcall
{
    vgs0_putstr(nametbl, 9, 5, 0x80, "SCORE RANKING");
    vgs0_putstr(nametbl, 5, 8, 0x80, "#  NAM  LV  SCORE");
    vgs0_putstr(nametbl, 5, 9, 0x80, "-  ---  --  ---------");
    for (uint8_t i = 0; i < 5; i++) {
        put_score_data(nametbl, 5, 11 + (i << 1), i + 1, 1, &SR->data[i]);
    }
}

static int8_t compare_score(uint8_t* sc1, uint8_t* sc2)
{
    for (uint8_t i = 0; i < 8; i++) {
        if (sc1[7 - i] < sc2[7 - i]) {
            return 1;
        } else if (sc1[7 - i] > sc2[7 - i]) {
            return -1;
        }
    }
    return 0;
}

static uint8_t code_from_cursor(uint8_t cx, uint8_t cy)
{
    const uint8_t text[] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', 0x0E, 0x0F};
    return text[cy * 13 + cx];
}

void score_entry(void) __z88dk_fastcall
{
    uint8_t rank, i, j;

    // ランクインしているかチェック
    for (rank = 0; rank < 5; rank++) {
        if (0 < compare_score(SR->data[rank].sc, GV->sc)) {
            break;
        }
    }
    if (5 == rank) {
        return; // 残念、ランク外
    }

    // 下位のスコアをズラす
    if (4 != rank) {
        for (i = 3; rank <= i && i != 0xFF; i--) {
            vgs0_memcpy((uint16_t)&SR->data[i + 1], (uint16_t)&SR->data[i], sizeof(ScoreData));
        }
    }

    // スコアとレベルを設定
    vgs0_memcpy((uint16_t)SR->data[rank].sc, (uint16_t)GV->sc, 8);
    SR->data[rank].exist = 1;
    if (SR->defaultName[0]) {
        SR->data[rank].name[0] = SR->defaultName[0];
        SR->data[rank].name[1] = SR->defaultName[1];
        SR->data[rank].name[2] = SR->defaultName[2];
    } else {
        SR->data[rank].name[0] = ' ';
        SR->data[rank].name[1] = ' ';
        SR->data[rank].name[2] = ' ';
    }
    switch (GV->level) {
        case 1:
        case 2:
        case 3:
        case 4:
            SR->data[rank].lv[0] = ' ';
            SR->data[rank].lv[1] = '0' + GV->level;
            break;
        case 5:
            SR->data[rank].lv[0] = 'N';
            SR->data[rank].lv[1] = 'M';
            break;
        case 6:
            SR->data[rank].lv[0] = 'C';
            SR->data[rank].lv[1] = 'H';
            break;
        case 7:
            SR->data[rank].lv[0] = 'M';
            SR->data[rank].lv[1] = 'X';
            break;
        case 8:
            SR->data[rank].lv[0] = 'M';
            SR->data[rank].lv[1] = '+';
            SR->extra = 1;
            break;
        case 9:
            SR->data[rank].lv[0] = 'E';
            SR->data[rank].lv[1] = 'X';
            break;
        default:
            SR->data[rank].lv[0] = '?';
            SR->data[rank].lv[1] = '?';
            break;
    }

    *VGS0_ADDR_BG_DPM = BANK_MAIN_BG;
    *VGS0_ADDR_FG_DPM = BANK_MAIN_FG;
    *VGS0_ADDR_SPRITE_DPM = BANK_BRAND1_SP;
    *VGS0_ADDR_FG_SCROLL_X = 0xFC;
    *VGS0_ADDR_FG_SCROLL_Y = 0;
    *VGS0_ADDR_BG_SCROLL_X = 0;
    *VGS0_ADDR_BG_SCROLL_Y = 0;
    vgs0_bg_putstr(2, 2, 0x80, "SC         0    HI         0");
    score_print(VGS0_ADDR_BG);

    vgs0_fg_putstr(8, 5, 0x80, "CONGRATULATIONS");
    vgs0_fg_putstr(11, 6, 0x80, "RANKED #0");
    VGS0_ADDR_FG->ptn[6][19] = '1' + rank;

    vgs0_fg_putstr(5, 9, 0x80, "#  NAM  LV  SCORE");
    vgs0_fg_putstr(5, 10, 0x80, "-  ---  --  ---------");
    for (j = 0; j < 5; j++) {
        put_score_data(VGS0_ADDR_FG, 5, 11 + j, j + 1, j == rank, &SR->data[j]);
    }

    VGS0_ADDR_BG->ptn[16][2] = 0x68;
    VGS0_ADDR_BG->attr[16][2] = 0x86;
    VGS0_ADDR_BG->ptn[16][29] = 0x6A;
    VGS0_ADDR_BG->attr[16][29] = 0x86;
    VGS0_ADDR_BG->ptn[24][2] = 0x88;
    VGS0_ADDR_BG->attr[24][2] = 0x86;
    VGS0_ADDR_BG->ptn[24][29] = 0x8A;
    VGS0_ADDR_BG->attr[24][29] = 0x86;
    for (i = 3; i < 29; i++) {
        VGS0_ADDR_BG->ptn[16][i] = 0x69;
        VGS0_ADDR_BG->attr[16][i] = 0x86;
        VGS0_ADDR_BG->ptn[24][i] = 0x89;
        VGS0_ADDR_BG->attr[24][i] = 0x86;
    }
    for (i = 17; i < 24; i++) {
        VGS0_ADDR_BG->ptn[i][2] = 0x78;
        VGS0_ADDR_BG->attr[i][2] = 0x86;
        VGS0_ADDR_BG->ptn[i][29] = 0x7A;
        VGS0_ADDR_BG->attr[i][29] = 0x86;
    }
    for (i = 3; i < 29; i++) {
        for (j = 17; j < 24; j++) {
            VGS0_ADDR_BG->ptn[j][i] = 0x79;
            VGS0_ADDR_BG->attr[j][i] = 0x86;
        }
    }

    VGS0_ADDR_BG->ptn[4][8] = 0x68;
    VGS0_ADDR_BG->attr[4][8] = 0x86;
    VGS0_ADDR_BG->ptn[4][23] = 0x6A;
    VGS0_ADDR_BG->attr[4][23] = 0x86;
    VGS0_ADDR_BG->ptn[7][8] = 0x88;
    VGS0_ADDR_BG->attr[7][8] = 0x86;
    VGS0_ADDR_BG->ptn[7][23] = 0x8A;
    VGS0_ADDR_BG->attr[7][23] = 0x86;
    for (i = 9; i < 23; i++) {
        VGS0_ADDR_BG->ptn[4][i] = 0x69;
        VGS0_ADDR_BG->attr[4][i] = 0x86;
        VGS0_ADDR_BG->ptn[7][i] = 0x89;
        VGS0_ADDR_BG->attr[7][i] = 0x86;
    }
    for (i = 5; i < 7; i++) {
        VGS0_ADDR_BG->ptn[i][8] = 0x78;
        VGS0_ADDR_BG->attr[i][8] = 0x86;
        VGS0_ADDR_BG->ptn[i][23] = 0x7A;
        VGS0_ADDR_BG->attr[i][23] = 0x86;
    }
    for (i = 9; i < 23; i++) {
        for (j = 5; j < 7; j++) {
            VGS0_ADDR_BG->ptn[j][i] = 0x79;
            VGS0_ADDR_BG->attr[j][i] = 0x86;
        }
    }

    vgs0_fg_putstr(3, 18, 0x80, "A B C D E F G H I J K L M ");
    vgs0_fg_putstr(3, 20, 0x80, "N O P Q R S T U V W X Y Z ");
    vgs0_fg_putstr(3, 22, 0x80, "0 1 2 3 4 5 6 7 8 9 . @ @ ");
    VGS0_ADDR_FG->ptn[22][25] = 0x0E;
    VGS0_ADDR_FG->ptn[22][27] = 0x0F;

    uint8_t cx = 0;
    uint8_t cy = 0;
    uint8_t nx = 0;
    if (SR->defaultName[0]) {
        cx = 12;
        cy = 2;
    }
    vgs0_oam_set(0, 2 * 8 + 4 + (cx * 16), 17 * 8 - 1 + (cy * 16), 0x84, 0xD0, 2, 2); // cursor
    vgs0_oam_set(1, 8 * 8 + 4 + (nx * 8), (11 + rank) * 8 - 1, 0x84, 0xE3, 0, 0);     // name cursor

    uint8_t pad;
    uint8_t prevPad = 0x00;
    int8_t mx = 0;
    int8_t my = 0;
    uint8_t code;
    while (1) {
        vgs0_wait_vsync();
        pad = vgs0_joypad_get();
        if (0x00 == (prevPad & VGS0_JOYPAD_LE) && 0x00 != (pad & VGS0_JOYPAD_LE)) {
            mx = -1;
        } else if (0x00 == (prevPad & VGS0_JOYPAD_RI) && 0x00 != (pad & VGS0_JOYPAD_RI)) {
            mx = 1;
        } else {
            mx = 0;
        }
        if (0x00 == (prevPad & VGS0_JOYPAD_UP) && 0x00 != (pad & VGS0_JOYPAD_UP)) {
            my = -1;
        } else if (0x00 == (prevPad & VGS0_JOYPAD_DW) && 0x00 != (pad & VGS0_JOYPAD_DW)) {
            my = 1;
        } else {
            my = 0;
        }

        uint8_t backspace = 0;
        if (mx || my) {
            vgs0_se_play(21);
            uint8_t not_edge = 1;
            if (mx < 0 && 0 == cx) {
                mx = 12;
                not_edge = 0;
            } else if (0 < mx && cx == 12) {
                mx = -12;
                not_edge = 0;
            }
            if (my < 0 && 0 == cy) {
                my = 2;
                not_edge = 0;
            } else if (0 < my && 2 == cy) {
                my = -2;
                not_edge = 0;
            }
            if (not_edge) {
                for (i = 0; i < 4; i++) {
                    VGS0_ADDR_OAM[0].x += vgs0_smul(mx, 4);
                    VGS0_ADDR_OAM[0].y += vgs0_smul(my, 4);
                    vgs0_wait_vsync();
                }
            }
            cx += mx;
            cy += my;
        } else if ((0x00 == (prevPad & VGS0_JOYPAD_T1) && 0x00 != (pad & VGS0_JOYPAD_T1)) ||
                   (0x00 == (prevPad & VGS0_JOYPAD_T2) && 0x00 != (pad & VGS0_JOYPAD_T2)) ||
                   (0x00 == (prevPad & VGS0_JOYPAD_ST) && 0x00 != (pad & VGS0_JOYPAD_ST))) {
            code = code_from_cursor(cx, cy);
            if (0x10 < code) {
                vgs0_se_play(23);
                VGS0_ADDR_FG->ptn[11 + rank][8 + nx] = code;
                if (2 == nx) {
                    cx = 12;
                    cy = 2;
                } else {
                    nx++;
                }
            } else if (0x0E == code) {
                backspace = 1;
            } else if (0x0F == code) {
                vgs0_se_play(25);
                break;
            }
        } else if (0x00 == (prevPad & VGS0_JOYPAD_SE) && 0x00 != (pad & VGS0_JOYPAD_SE)) {
            if (cx != 11 || cy != 2) {
                vgs0_se_play(21);
                cx = 11;
                cy = 2;
            } else {
                backspace = 1;
            }
        }

        if (backspace) {
            if (0 == nx) {
                vgs0_se_play(24);
                VGS0_ADDR_FG->ptn[11 + rank][8 + nx] = ' ';
            } else {
                if (' ' == VGS0_ADDR_FG->ptn[11 + rank][8 + nx]) {
                    nx--;
                    vgs0_se_play(24);
                    VGS0_ADDR_FG->ptn[11 + rank][8 + nx] = ' ';
                } else {
                    vgs0_se_play(24);
                    VGS0_ADDR_FG->ptn[11 + rank][8 + nx] = ' ';
                }
            }
        }
        VGS0_ADDR_OAM[0].x = 2 * 8 + 4 + (cx * 16);
        VGS0_ADDR_OAM[0].y = 17 * 8 - 1 + (cy * 16);
        VGS0_ADDR_OAM[1].x = 8 * 8 + 4 + (nx * 8);
        prevPad = pad;
    }
    SR->data[rank].name[0] = VGS0_ADDR_FG->ptn[11 + rank][8];
    SR->data[rank].name[1] = VGS0_ADDR_FG->ptn[11 + rank][9];
    SR->data[rank].name[2] = VGS0_ADDR_FG->ptn[11 + rank][10];
    SR->defaultName[0] = VGS0_ADDR_FG->ptn[11 + rank][8];
    SR->defaultName[1] = VGS0_ADDR_FG->ptn[11 + rank][9];
    SR->defaultName[2] = VGS0_ADDR_FG->ptn[11 + rank][10];
    vgs0_save((uint16_t)SR, sizeof(ScoreRanking));
    while (0 != vgs0_joypad_get()) {
        ;
    }
}
