#include "header.h"

void print_score_ranking(void) __z88dk_fastcall
{
    vgs0_fg_putstr(9, 5, 0x80, "SCORE RANKING");
    vgs0_fg_putstr(5, 8, 0x80, "#  NAM  LV  SCORE");
    vgs0_fg_putstr(5, 9, 0x80, "-  ---  --  ---------");
    vgs0_fg_putstr(5, 11, 0x80, "1  X.S  M+  999999990");
    vgs0_fg_putstr(5, 13, 0x80, "2  Y.S  MX  114514190");
    vgs0_fg_putstr(5, 15, 0x80, "3  Z.S  NM    9999990");
    vgs0_fg_putstr(5, 17, 0x80, "4  HOG   4     888880");
    vgs0_fg_putstr(5, 19, 0x81, "5  ***  **   NO ENTRY");
    vgs0_fg_putstr(4, 22, 0x80, "   PRESS START BUTTON   ");
}
