#include "../vgszero/lib/sdcc/vgs0lib.h"

// スプライトの初期位置
#define SP_ENEMY 88     // 敵 (128)
#define SP_DUST 216     // 土埃 (16)
#define SP_SHOT 232     // 自機ショット (8)
#define SP_SPRAY 240    // 水しぶき & ショットの煙 (16)

// 固定小数点数
typedef union {
    uint16_t value;
    uint8_t raw[2];
} var16_t;

// 矩形 (signed)
typedef struct {
    int8_t x;
    int8_t y;
    uint8_t width;
    uint8_t height;
} rect_t;

// 矩形 (unsigned)
typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t width;
    uint8_t height;
} urect_t;

// プレイヤー
typedef struct {
    var16_t x;      // X座標
    var16_t y;      // Y座標
    int16_t spd;    // 移動スピード
    int16_t jmp;    // ジャンプスピード
    int8_t jmpKeep; // ジャンプ中のボタン押しっぱなしフラグ
    uint8_t flight; // 対空時間（フレーム数）
    uint8_t snock;  // 着水時の衝撃波カウンター
    uint8_t slx;    // 着水座標の左端
    uint8_t srx;    // 着水座標の右端
    uint8_t shot;   // ショット発射フラグ
} Player;

// プレイヤーショット
typedef struct {
    uint8_t flag;   // 存在フラグ
    uint8_t x;      // X座標
    var16_t y;      // Y座標
    int16_t spd;    // 落下スピード
    uint8_t onair;  // 空中フラグ
} Shot;

// 水しぶき & 煙
typedef struct {
    uint8_t sn;     // 使用スプライトパターン兼activeフラグ（0 == not active）
    uint8_t t;      // 1フレーム表示で+1（8でディアクティベート）
} Spray;

// ゴミ
typedef struct {
    uint8_t flag;   // 存在フラグ
    var16_t x;      // X座標
    var16_t y;      // Y座標
    int16_t vx;     // フレームごとの座標移動量（X）
    int16_t vy;     // フレームごとの座標移動量（Y）
    int16_t sx;     // フレーム毎のvx加算値
    int16_t sy;     // フレーム毎のvy加算値
} Dust;

// 星
typedef struct {
    uint8_t flag;   // 存在フラグ
    uint8_t x;      // X座標（nametable）
    uint8_t y;      // X座標（nametable）
    uint8_t ptn;    // パターン
} Star;

// 泡
typedef struct {
    uint8_t flag;   // 存在フラグ
    uint8_t x;      // X座標（nametable）
    uint8_t y;      // X座標（nametable）
} Bubble;

// 敵
typedef struct {
    uint8_t flag;   // 存在フラグ
    uint8_t type;   // 種別フラグ
    uint8_t n8[2];  // 汎用変数（8bit）
    var16_t n16;    // 汎用変数 (16bit)
    uint8_t si;     // スプライト・インデクス
    uint8_t sn;     // スプライト数
    var16_t x;      // X座標
    var16_t y;      // Y座標
    var16_t vx;     // 移動速度(X)
    var16_t vy;     // 移動速度(Y)
    rect_t hit;     // 当たり判定
} Enemy;

// グローバル変数
typedef struct {
    uint8_t hi[8];          // ハイスコア
    uint8_t sc[8];          // スコア
    uint8_t ridx;           // 乱数インデクス
    uint8_t scoreAdded;     // スコア加算フラグ（再描画判定用）
    Player player;          // プレイヤー
    Shot shot[8];           // プレイヤーショット
    uint8_t shotIndex;      // プレイヤーショット index
    Spray spray[16];        // 水しぶき
    uint8_t sprayIndex;     // 水しぶき index
    Dust dust[16];          // ゴミ
    uint8_t dustIndex;      // ゴミ index
    Star star[16];          // 星
    uint8_t starIndex;      // 星 index
    Bubble bubble[16];      // 泡
    uint8_t bubbleIndex;    // 泡 index
    Enemy enemy[32];        // 敵
    uint8_t enemyIndex;     // 敵 index
    uint8_t espIndex;       // 敵スプライト index
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

void add_enemy(uint8_t type, uint8_t x, uint8_t y);
void move_enemy(void) __z88dk_fastcall;
