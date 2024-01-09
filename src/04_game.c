#include "header.h"

void game_main(void)
{
    uint8_t i;
    uint8_t a = 12;

    // スコアと波をBGからFGに描き直す
    vgs0_fg_putstr(2, 2, 0x80, "SC         0    HI         0");
    score_print(VGS0_ADDR_FG);
    for (i = 0; i < 32; i++) {
        VGS0_ADDR_BG->ptn[2][i] = 0x00;  // スコア(BG)をクリア
        VGS0_ADDR_BG->ptn[9][i] = 0x00;  // 波をクリア
        VGS0_ADDR_FG->ptn[9][i] = 0xA2;  // 波を描画
        VGS0_ADDR_FG->attr[9][i] = 0x80; // 波を描画
    }

    // 波に使っていたスプライトを自機スプライトに上書き
    vgs0_memcpy((uint16_t)&VGS0_ADDR_OAM[0],
                (uint16_t)&VGS0_ADDR_OAM[3],
                sizeof(OAM) * 9);

    // はイスコ以外のグローバル変数初期化
    vgs0_memset(0xC000 + 8, 0x00, sizeof(GlobalVariables) - 8);
    GV->player.x.value = 0x7400;
    GV->player.y.value = 0x4000;

    vgs0_bgm_play(1);

    // メインループ
    while (1) {
        vgs0_wait_vsync();
        a++;

        if (0 == (a & 0x1F)) {
            add_enemy(1, 0, (get_random(&GV->ridx) & 0x3F) + 0x60);
        }

        // 追加可能なら星と泡を追加
        add_star();
        add_bubble();

        // プレイヤーショットの移動
        move_pshot();

        // プレイヤーの移動
        move_player(a);

        // 敵キャラの移動
        move_enemy();

        // 画面エフェクトの処理を実行
        screen_effect_proc(a);

        // スコア再描画 & ハイスコア更新（加算時のみ）
        if (GV->scoreAdded) {
            score_print(VGS0_ADDR_FG);
        }
    }
}