#include "../vgszero/lib/sdcc/vgs0lib.h"

// バンク定義
#define BANK_PRG0_0 0    // プログラム0 page 0
#define BANK_PRG0_1 1    // プログラム0 page 1
#define BANK_PRG0_2 2    // プログラム0 page 2
#define BANK_PRG0_3 3    // プログラム0 page 3
#define BANK_PRG1_0 4    // プログラム1 page 0
#define BANK_PRG1_1 5    // プログラム1 page 1
#define BANK_PRG1_2 6    // プログラム1 page 2
#define BANK_PRG1_3 7    // プログラム1 page 3
#define BANK_MAIN_BG 8   // メインBG
#define BANK_MAIN_FG 9   // メインFG
#define BANK_MAIN_SP 10  // メインスプライト
#define BANK_BOMB_SP 11  // 爆破パターン
#define BANK_HIT_SP 12   // コンボ・カウンタ
#define BANK_MEDAL_SP 13 // メダル

// スプライトの初期位置
#define SP_MEDAL 66    // メダル (16)
#define SP_HIT 82       // コンボ・カウンタ (6)
#define SP_ENEMY 88     // 敵 (128)
#define SP_DUST 216     // 土埃 (16)
#define SP_SHOT 232     // 自機ショット (8)
#define SP_SPRAY 240    // 水しぶき & ショットの煙 (16)

// 敵の種別コード
#define ET_BOMBER 0    // 爆発
#define ET_MARINE_LR 1 // 潜水艦
#define ET_MARINE_RL 2 // 潜水艦
#define ET_THUNDER 3   // 雷撃

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
    uint8_t sa;     // ショット発射アニメーション
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

// 勲章
typedef struct {
    uint8_t flag;   // 存在フラグ & 状態
    uint8_t an;     // アニメーションパターン
    var16_t x;      // X座標
    var16_t y;      // Y座標
    var16_t vx;     // フレームごとの座標移動量（X）
    var16_t vy;     // フレームごとの座標移動量（Y）
} Medal;

// 敵
typedef struct {
    uint8_t flag;   // 存在フラグ
    uint8_t type;   // 種別フラグ
    uint8_t n8[8];  // 汎用変数（8bit）
    var16_t n16;    // 汎用変数 (16bit)
    uint8_t si[8];  // スプライト・インデクス
    uint8_t sn;     // スプライト数
    var16_t x;      // X座標
    var16_t y;      // Y座標
    var16_t vx;     // 移動速度(X)
    var16_t vy;     // 移動速度(Y)
    uint8_t check;  // 当たり判定有効化フラグ
} Enemy;

// グローバル変数
typedef struct {
    uint8_t hi[8];          // ハイスコア
    uint8_t sc[8];          // スコア
    uint8_t sadd[8];        // 桁毎のスコア加算値
    uint8_t ridx;           // 乱数インデクス
    uint8_t frame;          // フレームカウンタ
    uint8_t scoreAdded;     // スコア加算フラグ（再描画判定用）
    urect_t hbuf[2];        // 当たり判定用バッファ
    uint16_t hit;           // コンボ数
    uint16_t maxhit;        // 最大コンボ数
    uint16_t hitlog;        // 直前フレームのコンボ数
    uint8_t hkt;            // コンボ数のキープフレーム数
    uint8_t hstat;          // コンボ数表示ステータス (0: 非表示, 1: 登場中, 2: 表示中, 3: 退避中)
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
    Medal medal[16];        // 勲章
    uint8_t medalIndex;     // 勲章 index
    Enemy enemy[32];        // 敵
    uint8_t enemyIndex;     // 敵 index
    uint8_t espIndex;       // 敵スプライト index
} GlobalVariables;
#define GV ((GlobalVariables*)0xC000)

// table宣言変数
extern const uint16_t random[256];

// サブルーチン
void init_palette(void);
void submain(uint8_t arg) __z88dk_fastcall;

void score_calc(void) __z88dk_fastcall;
void score_print(NameTable* nam) __z88dk_fastcall;
void score_increment(uint8_t keta) __z88dk_fastcall;
void hit_print() __z88dk_fastcall;
uint8_t get_random(uint8_t* seed) __z88dk_fastcall;

void move_player() __z88dk_fastcall;

void move_pshot(void) __z88dk_fastcall;
void add_pshot(uint8_t x, uint16_t y);

void add_spray(uint8_t x, uint8_t y, uint8_t sn, uint8_t attr);
void add_dust_ground(uint8_t x, uint8_t y);
void add_dust_air(uint8_t x, uint8_t y);
void add_star(void) __z88dk_fastcall;
void add_bubble(void)  __z88dk_fastcall;
void add_medal(uint8_t x, uint8_t y);
void screen_effect_proc() __z88dk_fastcall;

void add_enemy(uint8_t type, uint8_t x, uint8_t y);
void move_enemy(void) __z88dk_fastcall;
void move_bomber(Enemy* enemy) __z88dk_fastcall;
void move_marineLR(Enemy* enemy) __z88dk_fastcall;
void move_marineRL(Enemy* enemy) __z88dk_fastcall;
void move_thunder(Enemy* enemy) __z88dk_fastcall;
