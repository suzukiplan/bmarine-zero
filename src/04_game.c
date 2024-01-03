#include "header.h"

inline void update_player_position(void)
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

    // メインループ
    GV->player.spd = 0;
    while (1) {
        vgs0_wait_vsync();
        a++;

        // プレイヤーの移動
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

        if (0 != GV->player.spd) {
            GV->player.x.value += GV->player.spd;
            GV->player.y.raw[1] = 0x41;
            update_player_position();
        } else if (GV->player.y.raw[1] != 0x40) {
            GV->player.y.raw[1] = 0x40;
            update_player_position();
        }

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