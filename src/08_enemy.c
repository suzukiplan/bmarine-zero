#include "header.h"

#define HIT_KEEP_TIME 120

// 敵種別毎の使用スプライト数
const uint8_t tbl_init_sn[9] = {
    1, // 0: 爆発
    3, // 1: 潜水艦 (左から右)
    3, // 2: 潜水艦 (右から左)
    1, // 3: 雷撃
};

// 初期パターン定義
static const uint8_t ptn_bomb[1] = {0x00};
static const uint8_t ptn_marineLR[3] = {0x18, 0x15, 0x16};
static const uint8_t ptn_marineRL[3] = {0x16, 0x15, 0x18};
static const uint8_t ptn_thunder[1] = {0x1C};

static uint8_t* get_init_ptn(uint8_t type)
{
    switch (type) {
        case 0: return ptn_bomb;     // 0: 爆発
        case 1: return ptn_marineLR; // 1: 潜水艦 (左から右)
        case 2: return ptn_marineRL; // 2: 潜水艦 (右から左)
        case 3: return ptn_thunder;  // 3: 雷撃
        default: return (uint8_t*)0;
    }
}

// 属性定義
static const uint8_t attr_bomb[1] = {0x85};
static const uint8_t attr_marineLR[3] = {0x00, 0x00, 0x00};
static const uint8_t attr_marineRL[3] = {0xC0, 0xC0, 0xC0};
static const uint8_t attr_thunder[1] = {0x84};

static uint8_t* get_init_attr(uint8_t type)
{
    switch (type) {
        case 0: return attr_bomb;     // 0: 爆発
        case 1: return attr_marineLR; // 1: 潜水艦 (左から右)
        case 2: return attr_marineRL; // 2: 潜水艦 (右から左)
        case 3: return attr_thunder;  // 3: 雷撃
        default: return (uint8_t*)0;
    }
}

// スプライトの初期座法設定テーブル
static const int8_t ofxy_zero[1] = {0x00};
static const int8_t wh_size0[1] = {0};
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
        case 2: return ofx_marineRL; // 2: 潜水艦 (右から左)
        case 3: return ofxy_zero;    // 3: 雷撃
        default: return (uint8_t*)0;
    }
}

static uint8_t* get_init_ofy(uint8_t type)
{
    switch (type) {
        case 0: return ofxy_zero;  // 0: 爆発
        case 1: return ofy_marine; // 1: 潜水艦 (左から右)
        case 2: return ofy_marine; // 2: 潜水艦 (右から左)
        case 3: return ofxy_zero;  // 3: 雷撃
        default: return (uint8_t*)0;
    }
}

static uint8_t* get_init_width(uint8_t type)
{
    switch (type) {
        case 0: return wh_size2;   // 0: 爆発
        case 1: return w_marineLR; // 1: 潜水艦 (左から右)
        case 2: return w_marineRL; // 2: 潜水艦 (右から左)
        case 3: return wh_size0;   // 3: 雷撃
        default: return (uint8_t*)0;
    }
}

static uint8_t* get_init_height(uint8_t type)
{
    switch (type) {
        case 0: return wh_size2; // 0: 爆発
        case 1: return h_marine; // 1: 潜水艦 (左から右)
        case 2: return h_marine; // 2: 潜水艦 (右から左)
        case 3: return wh_size0; // 3: 雷撃
        default: return (uint8_t*)0;
    }
}

// 当たり判定定義テーブル
static const rect_t hittbl[] = {
    {8, 8, 8, 8},     // 0: 爆発
    {-24, 0, 24, 16}, // 1: 潜水艦 (左から右)
    {0, 0, 24, 16},   // 2: 潜水艦 (右から左)
    {2, 0, 6, 8},     // 3: 雷撃
};

// 敵を追加
void add_enemy(uint8_t type, uint8_t x, uint8_t y)
{
    uint8_t i, j;

    // 追加可能なレコードを探索
    i = GV->enemyIndex;
    while (GV->enemy[i].flag) {
        i++;
        i &= 0x1F;
        if (i == GV->enemyIndex) {
            return; // 空きなし
        }
    }
    GV->enemyIndex = i;
    Enemy* enemy = &GV->enemy[GV->enemyIndex];
    vgs0_memset((uint16_t)enemy, 0, sizeof(Enemy));

    // スプライトに空きがあるかチェック
    for (i = 0; i < tbl_init_sn[type]; i++) {
        j = GV->espIndex;
        while (VGS0_ADDR_OAM[SP_ENEMY + j].ptn != 0x00) {
            j++;
            j &= 0x7F;
            if (j == GV->espIndex) {
                return; // 空きなし
            }
        }
        enemy->si[i] = SP_ENEMY + j;
        GV->espIndex = j + 1;
        GV->espIndex &= 0x7F;
    }

    // テーブルに初期値を設定
    enemy->flag = 1;
    enemy->type = type;
    enemy->x.raw[1] = x;
    enemy->y.raw[1] = y;
    enemy->sn = tbl_init_sn[type];

    // OAMに初期値を設定
    uint8_t* ptn = get_init_ptn(type);
    uint8_t* attr = get_init_attr(type);
    int8_t* ofx = get_init_ofx(type);
    int8_t* ofy = get_init_ofy(type);
    int8_t* w = get_init_width(type);
    int8_t* h = get_init_height(type);
    for (i = 0; i < enemy->sn; i++) {
        vgs0_oam_set(enemy->si[i], x + ofx[i], y + ofy[i], attr[i], ptn[i], w[i], h[i]);
        GV->espIndex += 1;
        GV->espIndex &= 0x7F;
    }
    GV->enemyIndex += 1;
    GV->enemyIndex &= 0x1F;
}

// 敵を削除
static void erase_enemy(Enemy* enemy) __z88dk_fastcall
{
    enemy->flag = 0;
    for (uint8_t i = 0; i < enemy->sn; i++) {
        VGS0_ADDR_OAM[enemy->si[i]].ptn = 0x00;
        VGS0_ADDR_OAM[enemy->si[i]].attr = 0x00;
    }
}

// 座標更新
static void update_enemy_position(Enemy* enemy) __z88dk_fastcall
{
    uint8_t dx = enemy->x.raw[1];
    uint8_t dy = enemy->y.raw[1];
    enemy->x.value += enemy->vx.value;
    enemy->y.value += enemy->vy.value;
    dx = enemy->x.raw[1] - dx;
    dy = enemy->y.raw[1] - dy;

    // 表示座標に変化があればスプライトを動かす
    if (dx || dy) {
        for (uint8_t i = 0; i < enemy->sn; i++) {
            VGS0_ADDR_OAM[enemy->si[i]].x += dx;
            VGS0_ADDR_OAM[enemy->si[i]].y += dy;
        }
    }
}

static void increment_hit_count() __z88dk_fastcall
{
    GV->hit++;
    if (GV->hit < 10) {
        GV->sadd[0] += GV->hit;
    } else if (GV->hit < 100) {
        GV->sadd[1] += GV->hit >> 3;
        GV->sadd[0] += GV->hit & 0x0F;
    } else {
        GV->sadd[2] += GV->hit >> 5;
        GV->sadd[1] += GV->hit >> 4;
        GV->sadd[0] += GV->hit & 0x0F;
    }
    GV->hkt = HIT_KEEP_TIME;
}

// 自機ショットとの当たり判定チェック
static void check_hit_pshot(Enemy* enemy) __z88dk_fastcall
{
    if (0 == enemy->flag || 0 == enemy->check) {
        return;
    }
    uint8_t et = enemy->y.raw[1];
    et += hittbl[enemy->type].y;
    uint8_t eb = et;
    eb += hittbl[enemy->type].height;
    uint8_t el = enemy->x.raw[1];
    el += hittbl[enemy->type].x;
    uint8_t er = el;
    er += hittbl[enemy->type].width;
    GV->hbuf[0].x = el;
    GV->hbuf[0].y = et;
    GV->hbuf[0].width = hittbl[enemy->type].width;
    GV->hbuf[0].height = hittbl[enemy->type].height;
    GV->hbuf[1].width = 8;
    GV->hbuf[1].height = 8;
    for (uint8_t i = 0; i < 8; i++) {
        Shot* shot = &GV->shot[i];
        if (shot->flag) {
            GV->hbuf[1].x = shot->x;
            GV->hbuf[1].y = shot->y.raw[1];
            if (vgs0_collision_check((uint16_t)GV->hbuf)) {
                if (0 != enemy->type) {
                    erase_enemy(enemy);
                    add_enemy(ET_BOMBER, el + (er - el - 24) / 2, et + (eb - et - 24) / 2);
                }
                GV->shot[i].flag = 0;
                VGS0_ADDR_OAM[SP_SHOT + i].attr = 0x00;
                add_enemy(ET_BOMBER, GV->shot[i].x - 8, GV->shot[i].y.raw[1] - 8);
                increment_hit_count();
                return;
            }
        }
    }
}

// 爆発の当たり判定チェック（爆発以外のものを誘爆）
static void check_hit_bomb(Enemy* bomb) __z88dk_fastcall
{
    if (0 == bomb->check || ET_BOMBER != bomb->type) {
        return; // I'm not explosion
    }
    GV->hbuf[0].x = bomb->x.raw[1] + 8;
    GV->hbuf[0].y = bomb->y.raw[1] + 8;
    GV->hbuf[0].width = 8;
    GV->hbuf[0].height = 8;
    for (uint8_t i = 0; i < 32; i++) {
        Enemy* enemy = &GV->enemy[i];
        if (ET_BOMBER != enemy->type && enemy->flag && enemy->check) {
            GV->hbuf[1].x = enemy->x.raw[1] + hittbl[enemy->type].x;
            GV->hbuf[1].y = enemy->y.raw[1] + hittbl[enemy->type].y;
            GV->hbuf[1].width = hittbl[enemy->type].width;
            GV->hbuf[1].height = hittbl[enemy->type].height;
            if (vgs0_collision_check((uint16_t)GV->hbuf)) {
                erase_enemy(enemy);
                add_enemy(ET_BOMBER, GV->hbuf[1].x + (GV->hbuf[1].width - 24) / 2, GV->hbuf[1].y + (GV->hbuf[1].height - 24) / 2);
                increment_hit_count();
            }
        }
    }
}

void move_enemy(void) __z88dk_fastcall
{
    uint8_t i;
    for (i = 0; i < 32; i++) {
        Enemy* enemy = &GV->enemy[i];
        if (enemy->flag) {
            switch (enemy->type) {
                case ET_BOMBER: move_bomber(enemy); break;
                case ET_MARINE_LR: move_marineLR(enemy); break;
                case ET_THUNDER: move_thunder(enemy); break;
            }
            if (0 == enemy->flag) {
                erase_enemy(enemy);
            } else {
                update_enemy_position(enemy);
                if (GV->frame & 1) {
                    check_hit_pshot(enemy);
                } else if (0 != enemy->flag) {
                    check_hit_bomb(enemy);
                }
            }
        }
    }
}
