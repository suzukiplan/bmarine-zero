#include "header.h"

void game_main(void)
{
    uint8_t i, j;
    uint8_t a = 12;

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

    // メインループ
    while (1) {
        vgs0_wait_vsync();
        a++;

        // 波のアニメーション
        j = a;
        j &= 0x1F;
        j >>= 2;
        j |= 0xA0;
        for (i = 0; i < 32; i++) {
            VGS0_ADDR_FG->ptn[9][i] =j;
        }
    }
}