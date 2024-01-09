#include "header.h"

// 敵の種別コード
#define ET_BOMBER 0    // 爆発
#define ET_MARINE_LR 1 // 潜水艦
#define ET_MARINE_RL 2 // 潜水艦

// 敵種別毎の使用スプライト数
const uint8_t tbl_init_sn[9] = {
    1, // 0: 爆発
    3, // 1: 潜水艦 (左から右)
    3, // 2: 潜水艦 (右から左)
};

// 初期パターン定義
static const uint8_t ptn_bomb[1] = {0xA0};
static const uint8_t ptn_marineLR[3] = {0x18, 0x15, 0x16};
static const uint8_t ptn_marineRL[3] = {0x16, 0x15, 0x18};

static uint8_t* get_init_ptn(uint8_t type)
{
    switch (type) {
        case 0: return ptn_bomb;     // 0: 爆発
        case 1: return ptn_marineLR; // 1: 潜水艦 (左から右)
        case 2: return ptn_marineRL;
        default: return (uint8_t*)0;
    }
}

// 属性定義
static const uint8_t attr_bomb[1] = {0x85};
static const uint8_t attr_marineLR[3] = {0x00, 0x00, 0x00};
static const uint8_t attr_marineRL[3] = {0xC0, 0xC0, 0xC0};

static uint8_t* get_init_attr(uint8_t type)
{
    switch (type) {
        case 0: return attr_bomb;     // 0: 爆発
        case 1: return attr_marineLR; // 1: 潜水艦 (左から右)
        case 2: return attr_marineRL;
        default: return (uint8_t*)0;
    }
}

// スプライトの初期座法設定テーブル
static const int8_t ofxy_zero[1] = {0x00};
static const int8_t wh_size2[1] = {2};
static const int8_t ofx_marineLR[3] = {-28, -24, -16};
static const int8_t ofx_marineRL[3] = {0, 16, 22};
static const int8_t ofy_marine[3] = {0, 0, 0};
static const int8_t w_marineLR[3] = {0, 0, 1};
static const int8_t w_marineRL[3] = {1, 0, 0};
static const int8_t h_marine[3] = {1, 1, 1};

static uint8_t* get_init_ofx(uint8_t type)
{
    switch (type) {
        case 0: return ofxy_zero;    // 0: 爆発
        case 1: return ofx_marineLR; // 1: 潜水艦 (左から右)
        case 2: return ofx_marineRL;
        default: return (uint8_t*)0;
    }
}

static uint8_t* get_init_ofy(uint8_t type)
{
    switch (type) {
        case 0: return ofxy_zero;  // 0: 爆発
        case 1: return ofy_marine; // 1: 潜水艦 (左から右)
        case 2: return ofy_marine;
        default: return (uint8_t*)0;
    }
}

static uint8_t* get_init_width(uint8_t type)
{
    switch (type) {
        case 0: return wh_size2;   // 0: 爆発
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
static const rect_t hittbl[3] = {
    {8, 8, 8, 8},     // 0: 爆発
    {-24, 0, 24, 16}, // 1: 潜水艦 (左から右)
    {0, 0, 24, 16}    // 2: 潜水艦 (右から左)
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
    enemy->n8[2] = 0;
    enemy->n8[3] = 0;
    enemy->n16.value = 0;
    enemy->check = 0;
    enemy->type = type;
    enemy->x.raw[1] = x;
    enemy->y.raw[1] = y;
    enemy->vx.value = 0;
    enemy->vy.value = 0;
    enemy->si = GV->espIndex;
    enemy->sn = tbl_init_sn[type];

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

static void erase_enemy(uint8_t index) __z88dk_fastcall
{
    GV->enemy[index].flag = 0;
    uint8_t si = GV->enemy[index].si;
    uint8_t i, n;
    for (i = 0; i < GV->enemy[index].sn; i++) {
        n = SP_ENEMY + si;
        VGS0_ADDR_OAM[n].ptn = 0x00;
        VGS0_ADDR_OAM[n].attr = 0x00;
        si += 1;
        si &= 0x7F;
    }
}

void move_enemy(void) __z88dk_fastcall
{
    uint8_t i, j, k, si;
    uint8_t dx, dy;
    for (i = 0; i < 32; i++) {
        if (GV->enemy[i].flag) {
            switch (GV->enemy[i].type) {
                case ET_BOMBER: move_bomber(&GV->enemy[i]); break;
                case ET_MARINE_LR: move_marineLR(&GV->enemy[i]); break;
            }
            // 自機ショットとの当たり判定チェック
            if (GV->enemy[i].flag && GV->enemy[i].check) {
                for (j = 0; j < 8; j++) {
                    if (GV->shot[j].flag) {
                        // Y座標が範囲内かチェック
                        uint8_t et = GV->enemy[i].y.raw[1];
                        et += hittbl[GV->enemy[i].type].y;
                        uint8_t eb = et;
                        eb += hittbl[GV->enemy[i].type].height;
                        if (GV->shot[j].y.raw[1] < eb && et < GV->shot[j].y.raw[1] + 8) {
                            // X座標が範囲内ならヒット
                            uint8_t el = GV->enemy[i].x.raw[1];
                            el += hittbl[GV->enemy[i].type].x;
                            uint8_t er = el;
                            er += hittbl[GV->enemy[i].type].width;
                            if (GV->shot[j].x < er && el < GV->shot[j].x + 8) {
                                if (0 != GV->enemy[i].type) {
                                    GV->enemy[i].flag = 0;
                                }
                                GV->shot[j].flag = 0;
                                VGS0_ADDR_OAM[SP_SHOT + j].attr = 0x00;
                                add_enemy(ET_BOMBER, GV->shot[j].x - 8, GV->shot[j].y.raw[1] - 8);
                                add_enemy(ET_BOMBER, el + (er - el - 24) / 2, et + (eb - et - 24) / 2);
                            }
                        }
                    }
                }
            }
            // 爆発の当たり判定チェック（爆発以外のものを誘爆）
            if (0 != GV->enemy[i].flag && ET_BOMBER == GV->enemy[i].type) {
                for (j = 0; j < 32; j++) {
                    if (0 != GV->enemy[j].flag && ET_BOMBER != GV->enemy[j].type) {
                        // Y座標が範囲内かチェック
                        uint8_t et = GV->enemy[j].y.raw[1];
                        et += hittbl[GV->enemy[j].type].y;
                        uint8_t eb = et;
                        eb += hittbl[GV->enemy[j].type].height;
                        if (GV->enemy[i].y.raw[1] + 4 < eb && et < GV->enemy[i].y.raw[1] + 20) {
                            // X座標が範囲内ならヒット
                            uint8_t el = GV->enemy[j].x.raw[1];
                            el += hittbl[GV->enemy[j].type].x;
                            uint8_t er = el;
                            er += hittbl[GV->enemy[j].type].width;
                            if (GV->enemy[i].x.raw[1] + 4 < er && el < GV->enemy[i].x.raw[1] + 20) {
                                erase_enemy(j);
                                add_enemy(ET_BOMBER, el + (er - el - 24) / 2, et + (eb - et - 24) / 2);
                            }
                        }
                    }
                }
            }
            if (0 != GV->enemy[i].flag) {
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
                // 削除
                erase_enemy(i);
            }
        }
    }
}
