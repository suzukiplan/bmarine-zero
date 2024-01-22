#include "header.h"

void print_score_ranking(void) __z88dk_fastcall
{
    vgs0_fg_putstr(9, 5, 0x80, "SCORE RANKING");
    vgs0_fg_putstr(5, 8, 0x80, "#  NAM  LV  SCORE");
    vgs0_fg_putstr(5, 9, 0x80, "-  ---  --  ---------");

    for (uint8_t i = 0; i < 5; i++) {
        uint8_t y = 11 + (i << 1);
        if (SR->data[i].exist) {
            vgs0_fg_putstr(5, y, 0x80, "0  ***  **         00");
            VGS0_ADDR_FG->ptn[y][8] = SR->data[i].name[0];
            VGS0_ADDR_FG->ptn[y][9] = SR->data[i].name[1];
            VGS0_ADDR_FG->ptn[y][10] = SR->data[i].name[2];
            VGS0_ADDR_FG->ptn[y][13] = SR->data[i].lv[0];
            VGS0_ADDR_FG->ptn[y][14] = SR->data[i].lv[1];
            uint8_t show = 0;
            for (uint8_t j = 0; j < 8; j++) {
                if (0 == show && 0 != SR->data[i].sc[j]) {
                    show = 1;
                }
                if (show) {
                    VGS0_ADDR_FG->ptn[y][17 + j] = '0' + SR->data[i].sc[j];
                }
            }
        } else {
            vgs0_fg_putstr(5, y, 0x81, "0  ***  **   NO ENTRY");
        }
        VGS0_ADDR_FG->ptn[y][5] = '1' + i;
    }
}
