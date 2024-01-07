#include "header.h"

// 敵の種別コード
#define ET_BOMBER 0     // 爆発
#define ET_MARINE_LR 1  // 潜水艦
#define ET_MARINE_RL 2  // 潜水艦

// 敵種別毎の使用スプライト数
const uint8_t tbl_init_sn[9] = {
    1,  // 0: 爆発
    3,  // 1: 潜水艦 (左から右)
    3,  // 2: 潜水艦 (右から左)
};

// パターン定義
const uint8_t ptn_bomb[10]= {
    0xA0, 0xA3, 0xA6, 0xA9, 0xAC,
    0xD0, 0xD3, 0xD6, 0xD9, 0xDC
};
const uint8_t ptn_marineLR[3] = { 0x18, 0x15, 0x16 };
const uint8_t ptn_marineRL[3] = { 0x16, 0x15, 0x18 };

static uint8_t* get_init_ptn(uint8_t type)
{
    switch (type) {
        case 0: return ptn_bomb; // 0: 爆発
        case 1: return ptn_marineLR; // 1: 潜水艦 (左から右)
        case 2: return ptn_marineRL;
        default: return (uint8_t*)0;
    }
}

// 属性定義
const uint8_t attr_bomb[1] = { 0x85 };
const uint8_t attr_marineLR[3] = { 0x80, 0x80, 0x80 };
const uint8_t attr_marineRL[3] = { 0xC0, 0xC0, 0xC0 };

static uint8_t* get_init_attr(uint8_t type)
{
    switch (type) {
        case 0: return attr_bomb; // 0: 爆発
        case 1: return attr_marineLR; // 1: 潜水艦 (左から右)
        case 2: return attr_marineRL;
        default: return (uint8_t*)0;
    }
}

// スプライトの初期座法設定テーブル
const int8_t ofxy_zero[1] = { 0x00 };
const int8_t wh_size2[1] = { 2 };
const int8_t ofx_marineLR[3] = { -28, -24, -16 };
const int8_t ofx_marineRL[3] = { 0, 16, 22 };
const int8_t ofy_marine[3] = { 0, 0, 0 };
const int8_t w_marineLR[3] = { 0, 0, 1 };
const int8_t w_marineRL[3] = { 1, 0, 0 };
const int8_t h_marine[3] = { 1, 1, 1 };

static uint8_t* get_init_ofx(uint8_t type)
{
    switch (type) {
        case 0: return ofxy_zero; // 0: 爆発
        case 1: return ofx_marineLR; // 1: 潜水艦 (左から右)
        case 2: return ofx_marineRL;
        default: return (uint8_t*)0;
    }
}

static uint8_t* get_init_ofy(uint8_t type)
{
    switch (type) {
        case 0: return ofxy_zero; // 0: 爆発
        case 1: return ofy_marine; // 1: 潜水艦 (左から右)
        case 2: return ofy_marine;
        default: return (uint8_t*)0;
    }
}

static uint8_t* get_init_width(uint8_t type)
{
    switch (type) {
        case 0: return wh_size2; // 0: 爆発
        case 1: return w_marineLR; // 1: 潜水艦 (左から右)
        case 2: return w_marineRL;
        default: return (uint8_t*)0;
    }
}

static uint8_t* get_init_height(uint8_t type)
{
    switch (type) {
        case 0: return wh_size2; // 0: 爆発
        case 1: return h_marine; // 1: 潜水艦 (左から右)
        case 2: return h_marine;
        default: return (uint8_t*)0;
    }
}

// 当たり判定定義テーブル
const rect_t tbl_init_hit[3] = {
    { 8, 8, 8, 8 },     // 0: 爆発
    { 0, 0, 28, 16 },   // 1: 潜水艦 (左から右)
    { 0, 0, 28, 16 }    // 2: 潜水艦 (右から左)
};

void add_enemy(uint8_t type, uint8_t x, uint8_t y)
{
    uint8_t i, j;
    Enemy* enemy = &GV->enemy[GV->enemyIndex];

    // 追加可能なテーブルがあるかチェック
    if (enemy->flag) {
        return;
    }

    // スプライトに空きがあるかチェック
    j = GV->espIndex;
    for (i = 0; i < tbl_init_sn[type]; i++, j++) {
        j &= 0x7F;
        if (VGS0_ADDR_OAM[SP_ENEMY + j].ptn != 0x00) {
            return; // 空きなし
        }
    }

    // テーブルに初期値を設定
    enemy->flag = 1;
    enemy->n8[0] = 0;
    enemy->n8[1] = 0;
    enemy->n16.value = 0;
    enemy->type = type;
    enemy->x.raw[1] = x;
    enemy->y.raw[1] = y;
    enemy->si = GV->espIndex;
    enemy->sn = tbl_init_sn[type];
    vgs0_memcpy((uint16_t)&enemy->hit, (uint16_t)&tbl_init_hit[type], sizeof(rect_t));

    // OAMに初期値を設定
    uint8_t* ptn = get_init_ptn(type);
    uint8_t* attr = get_init_attr(type);
    int8_t* ofx = get_init_ofx(type);
    int8_t* ofy = get_init_ofy(type);
    int8_t* w = get_init_width(type);
    int8_t* h = get_init_height(type);
    for (i = 0; i < tbl_init_sn[type]; i++) {
        vgs0_oam_set(GV->espIndex + SP_ENEMY, x + ofx[i], y + ofy[i], attr[i], ptn[i], w[i], h[i]);
        GV->espIndex += 1;
        GV->espIndex &= 0x7F;
    }
    score_increment(0);
    GV->enemyIndex += 1;
    GV->enemyIndex &= 0x1F;
}

// 1: 潜水艦 (左から右)
void move_marineLR(Enemy* enemy) __z88dk_fastcall
{
    enemy->vx.value = 0x00C0;
    enemy->n8[0]++;

    // プロペラを回転
    if (0 == (enemy->n8[0] & 3)) {
        enemy->n8[1]++;
        enemy->n8[1] &= 0x03;
        VGS0_ADDR_OAM[SP_ENEMY + enemy->si].ptn = 0x18 + enemy->n8[1];
    }
}

void move_enemy(void) __z88dk_fastcall
{
    uint8_t i, j, k, si;
    uint8_t dx, dy;
    for (i = 0; i < 32; i++) {
        if (GV->enemy[i].flag) {
            switch (GV->enemy[i].type) {
                case ET_MARINE_LR: move_marineLR(&GV->enemy[i]); break;
            }
            if (GV->enemy[i].flag) {
                // 座標更新
                dx = GV->enemy[i].x.raw[1];
                dy = GV->enemy[i].y.raw[1];
                GV->enemy[i].x.value += GV->enemy[i].vx.value;
                GV->enemy[i].y.value += GV->enemy[i].vy.value;
                dx = GV->enemy[i].x.raw[1] - dx;
                dy = GV->enemy[i].y.raw[1] - dy;
                // 表示座標に変化があればスプライトを動かす
                if (0 != dx || 0 != dy) {
                    si = GV->enemy[i].si;
                    for (j = 0; j < GV->enemy[i].sn; j++) {
                        k = SP_ENEMY + si;
                        VGS0_ADDR_OAM[k].x += dx;
                        VGS0_ADDR_OAM[k].y += dy;
                        si += 1;
                        si &= 0x7F;
                    }
                }
            } else {
                si = GV->enemy[i].si;
                for (j = 0; j < GV->enemy[i].sn; j++) {
                    k = SP_ENEMY + si;
                    VGS0_ADDR_OAM[k].ptn = 0x00;
                    VGS0_ADDR_OAM[k].attr = 0x00;
                    si += 1;
                    si &= 0x7F;
                }
            }
        }
    }
}
