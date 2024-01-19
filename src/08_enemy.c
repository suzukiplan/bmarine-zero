#include "header.h"

// 敵種別毎の使用スプライト数
const uint8_t tbl_init_sn[9] = {
    1, // 0: 爆発
    3, // 1: 潜水艦 (左から右)
    3, // 2: 潜水艦 (右から左)
    1, // 3: 雷撃
    1, // 4: 鳥
    1, // 5: 鳥の落とし物
    1, // 6: 魚
    1, // 7: カニ
    1, // 8: カニタマ
};

// 初期パターン定義
static const uint8_t ptn_bomb[1] = {0x00};
static const uint8_t ptn_marineLR[3] = {0x18, 0x15, 0x16};
static const uint8_t ptn_marineRL[3] = {0x16, 0x15, 0x18};
static const uint8_t ptn_thunder[1] = {0x1C};
static const uint8_t ptn_bird[1] = {0x68};
static const uint8_t ptn_unk[1] = {0x2C};
static const uint8_t ptn_fish[1] = {0x80};
static const uint8_t ptn_kani[1] = {0x6C};
static const uint8_t ptn_kanitama[1] = {0x2D};

static const uint8_t* get_init_ptn(uint8_t type)
{
    const uint8_t* ptn[] = {
        ptn_bomb,     // 0: 爆発
        ptn_marineLR, // 1: 潜水艦 (左から右)
        ptn_marineRL, // 2: 潜水艦 (右から左)
        ptn_thunder,  // 3: 雷撃
        ptn_bird,     // 4: 鳥
        ptn_unk,      // 5: 鳥の落とし物
        ptn_fish,     // 6: 魚
        ptn_kani,     // 7: カニ
        ptn_kanitama, // 8: カニタマ
    };
    return ptn[type];
}

// 属性定義
static const uint8_t attr_bomb[1] = {0x85};
static const uint8_t attr_marineLR[3] = {0x00, 0x00, 0x00};
static const uint8_t attr_marineRL[3] = {0x00, 0x00, 0x00};
static const uint8_t attr_thunder[1] = {0x84};
static const uint8_t attr_bird[1] = {0x80};
static const uint8_t attr_unk[1] = {0x80};
static const uint8_t attr_fish[1] = {0x87};
static const uint8_t attr_kani[1] = {0x85};
static const uint8_t attr_kanitama[1] = {0x85};

static const uint8_t* get_init_attr(uint8_t type)
{
    const uint8_t* attr[] = {
        attr_bomb,     // 0: 爆発
        attr_marineLR, // 1: 潜水艦 (左から右)
        attr_marineRL, // 2: 潜水艦 (右から左)
        attr_thunder,  // 3: 雷撃
        attr_bird,     // 4: 鳥
        attr_unk,      // 5: 鳥の落とし物
        attr_fish,     // 6: 魚
        attr_kani,     // 7: カニ
        attr_kanitama, // 8: カニタマ
    };
    return attr[type];
}

// スプライトの初期座法設定テーブル
static const int8_t single0[1] = {0};
static const int8_t single1[1] = {1};
static const int8_t single2[1] = {2};
static const int8_t ofx_marineLR[3] = {-28, -24, -16};
static const int8_t ofx_marineRL[3] = {0, 16, 22};
static const int8_t ofy_marine[3] = {0, 0, 0};
static const int8_t w_marineLR[3] = {0, 0, 1};
static const int8_t w_marineRL[3] = {1, 0, 0};
static const int8_t h_marine[3] = {1, 1, 1};

static const uint8_t* get_init_ofx(uint8_t type)
{
    const uint8_t* ofx[] = {
        single0,      // 0: 爆発
        ofx_marineLR, // 1: 潜水艦 (左から右)
        ofx_marineRL, // 2: 潜水艦 (右から左)
        single0,      // 3: 雷撃
        single0,      // 4: 鳥
        single0,      // 5: 鳥の落とし物
        single0,      // 6: 魚
        single0,      // 7: カニ
        single0,      // 8: カニタマ
    };
    return ofx[type];
}

static const uint8_t* get_init_ofy(uint8_t type)
{
    const uint8_t* ofy[] = {
        single0,    // 0: 爆発
        ofy_marine, // 1: 潜水艦 (左から右)
        ofy_marine, // 2: 潜水艦 (右から左)
        single0,    // 3: 雷撃
        single0,    // 4: 鳥
        single0,    // 5: 鳥の落とし物
        single0,    // 6: 魚
        single0,    // 7: カニ
        single0,    // 8: カニタマ
    };
    return ofy[type];
}

static const uint8_t* get_init_width(uint8_t type)
{
    const uint8_t* width[] = {
        single2,    // 0: 爆発
        w_marineLR, // 1: 潜水艦 (左から右)
        w_marineRL, // 2: 潜水艦 (右から左)
        single0,    // 3: 雷撃
        single1,    // 4: 鳥
        single0,    // 5: 鳥の落とし物
        single1,    // 6: 魚
        single1,    // 7: カニ
        single0,    // 8: カニタマ
    };
    return width[type];
}

static const uint8_t* get_init_height(uint8_t type)
{
    const uint8_t* height[] = {
        single2,  // 0: 爆発
        h_marine, // 1: 潜水艦 (左から右)
        h_marine, // 2: 潜水艦 (右から左)
        single0,  // 3: 雷撃
        single1,  // 4: 鳥
        single0,  // 5: 鳥の落とし物
        single1,  // 6: 魚
        single1,  // 7: カニ
        single0,  // 8: カニタマ
    };
    return height[type];
}

// 当たり判定定義テーブル
static const rect_t hittbl[] = {
    {8, 8, 8, 8},     // 0: 爆発
    {-24, 0, 24, 16}, // 1: 潜水艦 (左から右)
    {0, 0, 24, 16},   // 2: 潜水艦 (右から左)
    {2, 0, 6, 8},     // 3: 雷撃
    {0, 0, 16, 16},   // 4: 鳥
    {0, 0, 8, 8},     // 5: 鳥の落とし物
    {0, 0, 16, 16},   // 6: 魚
    {0, 0, 16, 16},   // 7: カニ
    {0, 0, 8, 8},     // 8: カニタマ
};

// スプライトバンク
static const uint8_t spbank[] = {
    BANK_BOMB_SP,   // 0: 爆発
    0,              // 1: 潜水艦 (左から右)
    0,              // 2: 潜水艦 (右から左)
    0,              // 3: 雷撃
    0,              // 4: 鳥
    0,              // 5: 鳥の落とし物
    BANK_LASER2_SP, // 6: 魚
    0,              // 7: カニ
    0,              // 8: カニタマ
};

// ダメージ量
const uint8_t dmgtbl[] = {
    35,  // 0: 爆発
    255, // 1: 潜水艦 (左から右)
    255, // 2: 潜水艦 (右から左)
    25,  // 3: 雷撃
    28,  // 4: 鳥
    17,  // 5: 鳥の落とし物
    16,  // 6: 魚
    255, // 7: カニ
    32,  // 8: カニタマ
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

    // 敵総数をインクリメント（※ただし、爆発、雷撃、鳥の落とし物、カニタマを除く）
    if (ET_BOMBER != type && ET_THUNDER != type && ET_UNK != type && ET_KANITAMA != type) {
        GV->enemies++;
    }

    // テーブルに初期値を設定
    enemy->flag = 1;
    enemy->type = type;
    enemy->x.raw[1] = x;
    enemy->y.raw[1] = y;
    enemy->sn = tbl_init_sn[type];
    vgs0_memcpy((uint16_t)&enemy->hit, (uint16_t)&hittbl[type], sizeof(rect_t));

    // OAMに初期値を設定
    const uint8_t* ptn = get_init_ptn(type);
    const uint8_t* attr = get_init_attr(type);
    const int8_t* ofx = get_init_ofx(type);
    const int8_t* ofy = get_init_ofy(type);
    const int8_t* w = get_init_width(type);
    const int8_t* h = get_init_height(type);
    for (i = 0; i < enemy->sn; i++) {
        vgs0_oam_set(enemy->si[i], x + ofx[i], y + ofy[i], attr[i], ptn[i], w[i], h[i]);
        VGS0_ADDR_OAM[enemy->si[i]].bank = spbank[type];
        GV->espIndex += 1;
        GV->espIndex &= 0x7F;
    }
    GV->enemyIndex += 1;
    GV->enemyIndex &= 0x1F;
}

// 敵を削除
void erase_enemy(Enemy* enemy) __z88dk_fastcall
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

// 自機・自機ショットとの当たり判定チェック
static void check_hit_pshot(Enemy* enemy) __z88dk_fastcall
{
    if (0 == enemy->flag || 0 == enemy->check) {
        return;
    }
    uint8_t et = enemy->y.raw[1];
    et += enemy->hit.y;
    uint8_t eb = et;
    eb += enemy->hit.height;
    uint8_t el = enemy->x.raw[1];
    el += enemy->hit.x;
    uint8_t er = el;
    er += enemy->hit.width;
    GV->hbuf[0].x = el;
    GV->hbuf[0].y = et;
    GV->hbuf[0].width = enemy->hit.width;
    GV->hbuf[0].height = enemy->hit.height;

    if (GV->player.lhit) {
        if (0 != enemy->type) {
            GV->hbuf[1].x = GV->player.x.raw[1] + 4;
            GV->hbuf[1].y = GV->player.y.raw[1] + 16;
            GV->hbuf[1].width = 16;
            GV->hbuf[1].height = 128;
            if (vgs0_collision_check((uint16_t)GV->hbuf)) {
                add_medal(level_medal_laser(enemy->type), el + (er - el - 16) / 2, et + (eb - et - 16) / 2);
                erase_enemy(enemy);
                add_enemy(ET_BOMBER, el + (er - el - 24) / 2, et + (eb - et - 24) / 2);
                increment_hit_count();
            }
        }
        return; // レーザー中は自機 and 自機ショットの判定をスキップ（自機はレーザー中無敵）
    }

    GV->hbuf[1].width = 8;
    GV->hbuf[1].height = 8;
    for (uint8_t i = 0; i < 8; i++) {
        Shot* shot = &GV->shot[i];
        if (shot->flag) {
            GV->hbuf[1].x = shot->x;
            GV->hbuf[1].y = shot->y.raw[1];
            if (vgs0_collision_check((uint16_t)GV->hbuf)) {
                if (0 != enemy->type) {
                    add_medal(level_medal_shot(enemy->type), el + (er - el - 16) / 2, et + (eb - et - 16) / 2);
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

    if (0 == GV->player.dmg && 0 == GV->player.muteki) {
        GV->hbuf[1].x = GV->player.x.raw[1] + 10;
        GV->hbuf[1].y = GV->player.y.raw[1] + 8;
        GV->hbuf[1].width = 4;
        GV->hbuf[1].height = 4;
        if (vgs0_collision_check((uint16_t)GV->hbuf)) {
            GV->hit = 0;
            GV->player.dmg = 60;
            GV->player.dmgsrc = enemy->type;
            if (ET_BOMBER != enemy->type) {
                erase_enemy(enemy);
                add_enemy(ET_BOMBER, el + (er - el - 24) / 2, et + (eb - et - 24) / 2);
            }
            return;
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
            GV->hbuf[1].x = enemy->x.raw[1] + enemy->hit.x;
            GV->hbuf[1].y = enemy->y.raw[1] + enemy->hit.y;
            GV->hbuf[1].width = enemy->hit.width;
            GV->hbuf[1].height = enemy->hit.height;
            if (vgs0_collision_check((uint16_t)GV->hbuf)) {
                add_medal(level_medal_bomb(enemy->type), GV->hbuf[1].x + (GV->hbuf[1].width - 16) / 2, GV->hbuf[1].y + (GV->hbuf[1].height - 16) / 2);
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
    uint8_t cnt = 0;
    for (i = 0; i < 32; i++) {
        Enemy* enemy = &GV->enemy[i];
        if (enemy->flag) {
            cnt++;
            switch (enemy->type) {
                case ET_BOMBER: move_bomber(enemy); break;
                case ET_MARINE_LR: move_marineLR(enemy); break;
                case ET_MARINE_RL: move_marineRL(enemy); break;
                case ET_THUNDER: move_thunder(enemy); break;
                case ET_BIRD: move_bird(enemy); break;
                case ET_UNK: move_unk(enemy); break;
                case ET_FISH: move_fish(enemy); break;
                case ET_KANI: move_kani(enemy); break;
                case ET_KANITAMA: move_kanitama(enemy); break;
                default: erase_enemy(enemy);
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
    if (GV->waitclear) {
        if (0 == cnt) {
            GV->waitclear = 0;
        }
    }
}
