#include "header.h"

void main(void)
{
    // パレット初期化
    vgs0_dma(BANK_PALETTE);
    vgs0_memcpy((uint16_t)VGS0_ADDR_PALETTE, (uint16_t)VGS0_ADDR_CHARACTER, 512);

    for (uint8_t i = 0; i < 8; i++) {
        GV->hi[i] = 0;
        GV->sc[i] = 0;
    }
    GV->hi[0] = 3;
    GV->hi[1] = 7;
    GV->hi[2] = 5;

    // タイトル画面へ遷移
    vgs0_bank0_switch(BANK_PRG0_0);
    vgs0_bank1_switch(BANK_PRG0_1);
    vgs0_bank2_switch(BANK_PRG0_2);
    vgs0_bank3_switch(BANK_PRG0_3);
    submain(0);

    // ゲームメインへ遷移
    vgs0_bank0_switch(BANK_PRG1_0);
    vgs0_bank1_switch(BANK_PRG1_1);
    vgs0_bank2_switch(BANK_PRG1_2);
    vgs0_bank3_switch(BANK_PRG1_3);
    submain(0);
}