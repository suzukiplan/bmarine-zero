#include "../vgszero/lib/sdcc/vgs0lib.h"
#include "palette.h"

void main(void)
{
    // パレットを初期化
    init_palette();

    // Bank 4 を Character Pattern Table ($A000) に転送 (DMA)
    vgs0_dma(4);

    // 画面中央付近 (10,12) に "HELLO,WORLD!" を描画
    vgs0_bg_putstr(10, 12, 0x80, "HELLO,WORLD!");

    // BGMを再生
    vgs0_bgm_play(0);

    uint8_t prevA = 0;
    uint8_t prevB = 0;

    while (1) {
        vgs0_wait_vsync();
        uint8_t pad = vgs0_joypad_get();
        if (pad & VGS0_JOYPAD_LE) {
            *VGS0_ADDR_BG_SCROLL_X += 1;
        } else if (pad & VGS0_JOYPAD_RI) {
            *VGS0_ADDR_BG_SCROLL_X -= 1;
        }

        if (pad & VGS0_JOYPAD_UP) {
            *VGS0_ADDR_BG_SCROLL_Y += 1;
        } else if (pad & VGS0_JOYPAD_DW) {
            *VGS0_ADDR_BG_SCROLL_Y -= 1;
        }

        if (pad & VGS0_JOYPAD_T1) {
            if (0 == prevA) {
                vgs0_se_play(0);
            }
            prevA = 1;
        } else {
            prevA = 0;
        }

        if (pad & VGS0_JOYPAD_T2) {
            if (0 == prevB) {
                vgs0_se_play(1);
            }
            prevB = 1;
        } else {
            prevB = 0;
        }
    }
}
