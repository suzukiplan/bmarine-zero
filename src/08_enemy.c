#include "header.h"

// 敵の種別コード
#define ET_BOMBER 0     // 爆発
#define ET_MARINE_LR 1  // 潜水艦
#define ET_MARINE_RL 2  // 潜水艦

// 敵種別毎の使用スプライト数
const uint8_t tbl_init_sn[9] = {
    9,  // 0: 爆発
    8,  // 1: 潜水艦 (左から右)
    8,  // 2: 潜水艦 (右から左)
};

// パターン定義
const uint8_t ptn_bomb[10][9] = {
    { 0xA0, 0xA1, 0xA2, 0xB0, 0xB1, 0xB2, 0xC0, 0xC1, 0xC2 },
    { 0xA3, 0xA4, 0xA5, 0xB3, 0xB4, 0xB5, 0xC3, 0xC4, 0xC5 },
    { 0xA6, 0xA7, 0xA8, 0xB6, 0xB7, 0xB8, 0xC6, 0xC7, 0xC8 },
    { 0xA9, 0xAA, 0xAB, 0xB9, 0xBA, 0xBB, 0xC9, 0xCA, 0xCB },
    { 0xAC, 0xAD, 0xAE, 0xBC, 0xBD, 0xBE, 0xCC, 0xCD, 0xCE },
    { 0xD0, 0xD1, 0xD2, 0xE0, 0xE1, 0xE2, 0xF0, 0xF1, 0xF2 },
    { 0xD3, 0xD4, 0xD5, 0xE3, 0xE4, 0xE5, 0xF3, 0xF4, 0xF5 },
    { 0xD6, 0xD7, 0xD8, 0xE6, 0xE7, 0xE8, 0xF6, 0xF7, 0xF8 },
    { 0xD9, 0xDA, 0xDB, 0xE9, 0xEA, 0xEB, 0xF9, 0xFA, 0xFB },
    { 0xDC, 0xDD, 0xDE, 0xEC, 0xED, 0xEE, 0xFC, 0xFD, 0xFE }
};
const uint8_t ptn_marineLR[8] = { 0x18, 0x28, 0x15, 0x25, 0x16, 0x26, 0x17, 0x27 };
const uint8_t ptn_marineRL[8] = { 0x17, 0x27, 0x16, 0x26, 0x15, 0x25, 0x18, 0x28 };

static uint8_t* get_init_ptn(uint8_t type)
{
    switch (type) {
        case 0: return ptn_bomb[0];
        case 1: return ptn_marineLR;
        case 2: return ptn_marineRL;
        default: return (uint8_t*)0;
    }
}

// 属性定義
const uint8_t attr_bomb[9] = { 0x85, 0x85, 0x85, 0x85, 0x85, 0x85, 0x85, 0x85, 0x85 };
const uint8_t attr_marineLR[8] = { 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80 };
const uint8_t attr_marineRL[8] = { 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0 };

static uint8_t* get_init_attr(uint8_t type)
{
    switch (type) {
        case 0: return attr_bomb;
        case 1: return attr_marineLR;
        case 2: return attr_marineRL;
        default: return (uint8_t*)0;
    }
}

// スプライトの初期座法設定テーブル
const uint8_t ofx_bomb[9] = { 0x00, 0x08, 0x10, 0x00, 0x08, 0x10, 0x00, 0x08, 0x10 };
const uint8_t ofy_bomb[9] = { 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x10, 0x10, 0x10 };
const uint8_t ofx_marineLR[8] = { 0, 0, 6, 6, 14, 14, 22, 22 };
const uint8_t ofx_marineRL[8] = { 0x00, 0x00, 0x08, 0x08, 0x10, 0x10, 0x18, 0x18 };
const uint8_t ofy_marine[8] = { 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08 };

static uint8_t* get_init_ofx(uint8_t type)
{
    switch (type) {
        case 0: return ofx_bomb;
        case 1: return ofx_marineLR;
        case 2: return ofx_marineRL;
        default: return (uint8_t*)0;
    }
}

static uint8_t* get_init_ofy(uint8_t type)
{
    switch (type) {
        case 0: return ofy_bomb;
        case 1: return ofy_marine;
        case 2: return ofy_marine;
        default: return (uint8_t*)0;
    }
}

// 当たり判定定義テーブル
const Rect tbl_init_hit[3] = {
    { 8, 8, 8, 8 },     // 0: 爆発
    { 8, 0, 24, 16 },   // 1: 潜水艦 (左から右)
    { 0, 0, 24, 16 }    // 2: 潜水艦 (右から左)
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
    vgs0_memcpy((uint16_t)&enemy->hit, (uint16_t)&tbl_init_hit[type], sizeof(Rect));

    // OAMに初期値を設定
    uint8_t* ptn = get_init_ptn(type);
    uint8_t* attr = get_init_attr(type);
    uint8_t* ofx = get_init_ofx(type);
    uint8_t* ofy = get_init_ofy(type);
    for (i = 0; i < tbl_init_sn[type]; i++) {
        vgs0_oam_set(GV->espIndex + SP_ENEMY, x + ofx[i], y + ofy[i], attr[i], ptn[i]);
        GV->espIndex += 1;
        GV->espIndex &= 0x7F;
    }
    score_increment(0);
    GV->enemyIndex += 1;
    GV->enemyIndex &= 0x1F;
}

void move_marineLR(Enemy* enemy) __z88dk_fastcall
{
    enemy->vx.value = 0x00C0;

    // プロペラを回転
    enemy->n8[0]++;
    enemy->n8[0] &= 3;
    if (0 == enemy->n8[0]) {
        enemy->n8[1]++;
        enemy->n8[1] &= 0x03;
        uint8_t sn = enemy->si;
        VGS0_ADDR_OAM[SP_ENEMY + sn].ptn = 0x18 + enemy->n8[1];
        sn++;
        sn &= 0x7F;
        VGS0_ADDR_OAM[SP_ENEMY + sn].ptn = 0x28 + enemy->n8[1];
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
