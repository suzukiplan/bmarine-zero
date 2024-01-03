#include "../vgszero/lib/sdcc/vgs0lib.h"

typedef union {
    uint16_t value;
    uint8_t raw[2];
} Var16;

// グローバル変数
typedef struct {
    uint8_t sc[8];  // スコア
    uint8_t hi[8];  // ハイスコア
    uint8_t ridx;   // 乱数インデクス

    // プレイヤー
    struct Player {
        Var16 x;
        Var16 y;
    } player;
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
