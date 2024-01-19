#include "../vgszero/lib/sdcc/vgs0lib.h"

// ヒットの維持フレーム数
#define HIT_KEEP_TIME 120

// バンク定義
#define BANK_PRG0_0 0     // プログラム0 page 0
#define BANK_PRG0_1 1     // プログラム0 page 1
#define BANK_PRG0_2 2     // プログラム0 page 2
#define BANK_PRG0_3 3     // プログラム0 page 3
#define BANK_PRG1_0 4     // プログラム1 page 0
#define BANK_PRG1_1 5     // プログラム1 page 1
#define BANK_PRG1_2 6     // プログラム1 page 2
#define BANK_PRG1_3 7     // プログラム1 page 3
#define BANK_PALETTE 8    // パレット
#define BANK_MAIN_BG 9    // メインBG
#define BANK_MAIN_FG 10   // メインFG
#define BANK_MAIN_SP 11   // メインスプライト
#define BANK_BOMB_SP 12   // 爆破パターン
#define BANK_HIT_SP 13    // コンボ・カウンタ
#define BANK_MEDAL_SP 14  // メダル
#define BANK_LASER_SP 15  // レーザー
#define BANK_LASER2_SP 16 // レーザー装飾
#define BANK_NABURA_SP 17 // ナブラステージ

// スプライトの初期位置
#define SP_JIKIDMG 0    // 自機ダメージ (1)
#define SP_JIKI 1       // 自機 (2)
#define SP_SHOTEFF 3    // ショット発射エフェクト (1)
#define SP_HP 4         // 体力ゲージ (5)
#define SP_SPRAY2 10    // うんこの軌道 (16)
#define SP_LTOP 57      // レーザー発射口 (1)
#define SP_LBOTTOM 58   // レーザー地面 (1)
#define SP_LASER 59     // レーザー (1)
#define SP_LMETER 60    // レーザーのパワーメーター (1)
#define SP_ZANZO 61     // レーザー充填 & 発射時の残像 (4)
#define SP_TAIRYO 65    // 大漁旗 (1)
#define SP_MEDAL 66     // メダル (16)
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
#define ET_BIRD 4      // 鳥
#define ET_UNK 5       // 鳥の落とし物
#define ET_FISH 6      // 魚
#define ET_KANI 7      // カニ
#define ET_KANITAMA 8  // カニタマ

// プレイヤー
typedef struct {
    var16_t x;          // X座標
    var16_t y;          // Y座標
    int16_t spd;        // 移動スピード
    int16_t jmp;        // ジャンプスピード
    int8_t jmpKeep;     // ジャンプ中のボタン押しっぱなしフラグ
    uint8_t flight;     // 対空時間（フレーム数）
    uint8_t snock;      // 着水時の衝撃波カウンター
    uint8_t slx;        // 着水座標の左端
    uint8_t srx;        // 着水座標の右端
    uint8_t shot;       // ショット発射フラグ
    uint8_t sa;         // ショット発射アニメーション
    uint8_t charge;     // レーザーチャージ
    uint8_t laser;      // レーザー
    uint8_t lcnt;       // レーザーカウンタ
    uint8_t lhit;       // レーザー当たり判定
    uint8_t zflag[4];   // 残像フラグ
    uint8_t zindex;     // 残像インデクス
    uint8_t dmg;        // ダメージカウンタ
    uint8_t muteki;     // 無敵カウンタ
    uint8_t hp;         // 体力
    uint8_t chp;        // 現在の表示体力
    uint8_t nabura;     // ナブラ演出フラグ
    uint8_t mode;       // 0: 通常モード, 1: ナブラモード
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
    uint8_t type;   // 種別 (0: 回復, 1: スコア)
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
    rect_t hit;     // 当たり判定
} Enemy;

// グローバル変数
typedef struct {
    uint8_t hi[8];          // ハイスコア
    uint8_t sc[8];          // スコア
    uint8_t sadd[8];        // 桁毎のスコア加算値
    var16_t smc;            // スコアに反映するメダル総数（最大体力で type 0 を取得すると上昇 & damage でリセット）
    uint8_t ridx;           // 乱数インデクス
    uint8_t frame;          // フレームカウンタ
    uint8_t level;          // レベル
    uint8_t levelFrame;     // レベル表示フレーム
    uint16_t enemies;       // 敵出現総数カウンタ
    uint8_t waitclear;      // 画面から敵が履けるのを待つ
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
    Spray spray2[16];       // うんこの軌道
    uint8_t spray2Index;    // うんこの軌道 index
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
void submain(uint8_t arg) __z88dk_fastcall;

void score_calc(void) __z88dk_fastcall;
void score_print(NameTable* nam) __z88dk_fastcall;
void score_increment(uint8_t keta) __z88dk_fastcall;
void hit_print(void) __z88dk_fastcall;
void render_hp(void) __z88dk_fastcall;
uint8_t get_random(uint8_t* seed) __z88dk_fastcall;

void move_player(void) __z88dk_fastcall;

void move_pshot(void) __z88dk_fastcall;
void add_pshot(uint8_t x, uint16_t y);

void add_spray(uint8_t x, uint8_t y, uint8_t sn, uint8_t attr);
void add_spray2(uint8_t x, uint8_t y, uint8_t sn, uint8_t attr);
void add_dust_ground(uint8_t x, uint8_t y);
void add_dust_air(uint8_t x, uint8_t y);
void add_star(void) __z88dk_fastcall;
void add_bubble(void)  __z88dk_fastcall;
void add_medal(uint8_t type, uint8_t x, uint8_t y);
void screen_effect_proc(void) __z88dk_fastcall;

void add_enemy(uint8_t type, uint8_t x, uint8_t y);
void erase_enemy(Enemy* enemy) __z88dk_fastcall;
void move_enemy(void) __z88dk_fastcall;
void move_bomber(Enemy* enemy) __z88dk_fastcall;
void move_marineLR(Enemy* enemy) __z88dk_fastcall;
void move_marineRL(Enemy* enemy) __z88dk_fastcall;
void move_thunder(Enemy* enemy) __z88dk_fastcall;
void move_bird(Enemy* enemy) __z88dk_fastcall;
void move_unk(Enemy* enemy) __z88dk_fastcall;
void move_fish(Enemy* enemy) __z88dk_fastcall;
void move_kani(Enemy* enemy) __z88dk_fastcall;
void move_kanitama(Enemy* enemy) __z88dk_fastcall;

void level_proc(void) __z88dk_fastcall;
uint8_t level_medal_laser(uint8_t type) __z88dk_fastcall;
uint8_t level_medal_shot(uint8_t type) __z88dk_fastcall;
uint8_t level_medal_bomb(uint8_t type) __z88dk_fastcall;
