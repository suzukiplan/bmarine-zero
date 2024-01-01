// グローバル変数
typedef struct {
    uint8_t ridx; // 乱数インデクス
} GlobalVariables;
#define GV ((GlobalVariables*)0xC000)
