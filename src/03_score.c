#include "header.h"


void score_print(NameTable* nam)
{
    uint8_t detectS = 0;
    uint8_t detectH = 0;
    uint8_t updateH = 0;
    for (uint8_t i = 0; i < 8; i++) {
        // score
        if (!updateH) {
            if (GV->hi[7 - i] < GV->sc[7 - i]) {
                updateH = 1;
            }
        }
        if (updateH) {
            GV->hi[7 - i] = GV->sc[7 - i];
        }
        if (7 == i || 0 != detectS || 0 != GV->sc[7 - i]) {
            nam->ptn[2][5 + i] = GV->sc[7 - i] + '0';
            detectS = 1;
        } else {
            nam->ptn[2][5 + i] = ' ';
        }
        // high score
        if (7 == i || 0 != detectH || 0 != GV->hi[7 - i]) {
            nam->ptn[2][21 + i] = GV->hi[7 - i] + '0';
            detectH = 1;
        } else {
            nam->ptn[2][21 + i] = ' ';
        }
    }
}