#include "../vgszero/lib/sdcc/vgs0lib.h"

typedef union {
    uint16_t value;
    uint8_t raw[2];
} Var16;

// グローバル変数
typedef struct {
    uint8_t sc[8];      // スコア
    uint8_t hi[8];      // ハイスコア
    uint8_t ridx;       // 乱数インデクス
    uint8_t scoreAdded; // スコア加算フラグ（再描画判定用）

    // プレイヤー
    struct Player {
        Var16 x;        // X座標
        Var16 y;        // Y座標
        int16_t spd;    // 移動スピード
        int16_t jmp;    // ジャンプスピード
        int8_t jmpKeep; // ジャンプ中のボタン押しっぱなしフラグ
        uint8_t flight; // 対空時間（フレーム数）
    } player;

    // 水しぶき
    struct Spray {
        uint8_t sn;     // 使用スプライトパターン兼activeフラグ（0 == not active）
        uint8_t t;      // 1フレーム表示で+1（8でディアクティベート）
    } spray[16];
    uint8_t sprayIndex;

} GlobalVariables;
#define GV ((GlobalVariables*)0xC000)

// table宣言変数
extern const uint16_t random[256];

// サブルーチン
void init_palette(void);
void title(void);
void score_print(NameTable* nam);
void score_increment(uint8_t keta);
void game_main(void);
