#include "../vgszero/lib/sdcc/vgs0lib.h"

// スプライトの初期位置
#define SP_ENEMY 80     // 敵 (128)
#define SP_DUST 208     // 土埃 (16)
#define SP_SHOT 224     // 自機ショット (16)
#define SP_SPRAY 240    // 水しぶき & ショットの煙 (16)

// 敵の種別コード
#define ET_BOMBER 0     // 爆発
#define ET_SUBMARINE 1  // 潜水艦
#define ET_THUNDER 2    // サンダー（潜水艦のショット）
#define ET_FISH 3       // 魚
#define ET_BIRD 4       // 鳥
#define ET_BIRDDROP 5   // 鳥の糞
#define ET_CRAB 6       // カニ
#define ET_CRABFIRE 7   // カニタマ

typedef union {
    uint16_t value;
    uint8_t raw[2];
} Var16;

// グローバル変数
typedef struct {
    uint8_t hi[8];      // ハイスコア
    uint8_t sc[8];      // スコア
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
        uint8_t snock;  // 着水時の衝撃波カウンター
        uint8_t slx;    // 着水座標の左端
        uint8_t srx;    // 着水座標の右端
        uint8_t shot;   // ショット発射フラグ
    } player;

    // プレイヤーショット
    struct Shot {
        uint8_t flag;   // 存在フラグ
        uint8_t x;      // X座標
        Var16 y;        // Y座標
        int16_t spd;    // 落下スピード
        uint8_t onair;  // 空中フラグ
    } shot[8];
    uint8_t shotIndex;

    // 水しぶき
    struct Spray {
        uint8_t sn;     // 使用スプライトパターン兼activeフラグ（0 == not active）
        uint8_t t;      // 1フレーム表示で+1（8でディアクティベート）
    } spray[16];
    uint8_t sprayIndex;

    // ゴミ
    struct Dust {
        uint8_t flag;   // 存在フラグ
        Var16 x;        // X座標
        Var16 y;        // Y座標
        int16_t vx;     // フレームごとの座標移動量（X）
        int16_t vy;     // フレームごとの座標移動量（Y）
        int16_t sx;     // フレーム毎のvx加算値
        int16_t sy;     // フレーム毎のvy加算値
    } dust[16];
    uint8_t dustIndex;

    // 星
    struct Star {
        uint8_t flag;   // 存在フラグ
        uint8_t x;      // X座標（nametable）
        uint8_t y;      // X座標（nametable）
        uint8_t ptn;    // パターン
    } star[16];
    uint8_t starIndex;

    // 泡
    struct Bubble {
        uint8_t flag;   // 存在フラグ
        uint8_t x;      // X座標（nametable）
        uint8_t y;      // X座標（nametable）
    } bubble[16];
    uint8_t bubbleIndex;

    // 敵
    struct Enemy {
        uint8_t flag;   // 存在フラグ
        uint8_t type;   // 種別フラグ
        uint8_t n8[4];  // 汎用変数（8bit）
        Var16 n16[4];   // 汎用変数 (16bit)
        uint8_t si;     // スプライト・インデクス
        uint8_t sn;     // スプライト数
        Var16 x;        // X座標
        Var16 y;        // Y座標
        Var16 vx;       // 移動速度(X)
        Var16 vy;       // 移動速度(Y)
    } enemy[16];
    uint8_t enemyIndex;
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
uint8_t get_random(uint8_t* seed) __z88dk_fastcall;

void move_player(uint8_t a) __z88dk_fastcall;

void move_pshot(void) __z88dk_fastcall;
void add_pshot(uint8_t x, uint16_t y);

void add_spray(uint8_t x, uint8_t y, uint8_t sn, uint8_t attr);
void add_dust_ground(uint8_t x, uint8_t y);
void add_star(void) __z88dk_fastcall;
void add_bubble(void)  __z88dk_fastcall;
void screen_effect_proc(uint8_t a) __z88dk_fastcall;
