// グローバル変数

typedef struct {
    uint8_t sc[8];  // スコア
    uint8_t hi[8];  // ハイスコア
    uint8_t ridx;   // 乱数インデクス
} GlobalVariables;
#define GV ((GlobalVariables*)0xC000)
