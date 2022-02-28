/*
 * File: z_en_pr.c
 * Overlay: ovl_En_Pr
 * Description: Desbreko
 */

#include "z_en_pr.h"
#include "overlays/actors/ovl_En_Prz/z_en_prz.h"
#include "objects/object_pr/object_pr.h"

#define FLAGS (ACTOR_FLAG_1 | ACTOR_FLAG_4 | ACTOR_FLAG_10)

#define THIS ((EnPr*)thisx)

void EnPr_Init(Actor* thisx, GlobalContext* globalCtx);
void EnPr_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnPr_Update(Actor* thisx, GlobalContext* globalCtx);
void EnPr_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80A326F0(EnPr* this);
void func_80A32740(EnPr* this, GlobalContext* globalCtx);
void func_80A32854(EnPr* this);
void func_80A3289C(EnPr* this, GlobalContext* globalCtx);
void func_80A3295C(EnPr* this);
void func_80A32984(EnPr* this, GlobalContext* globalCtx);
void func_80A32A40(EnPr* this, GlobalContext* globalCtx);
void func_80A32AF8(EnPr* this);
void func_80A32B20(EnPr* this, GlobalContext* globalCtx);
void func_80A32CDC(EnPr* this);
void func_80A32D28(EnPr* this, GlobalContext* globalCtx);
void func_80A32EA4(EnPr* this, GlobalContext* globalCtx);
void func_80A32F48(EnPr* this, GlobalContext* globalCtx);

static DamageTable sDamageTable = {
    /* Deku Nut       */ DMG_ENTRY(0, 0x0),
    /* Deku Stick     */ DMG_ENTRY(0, 0x0),
    /* Horse trample  */ DMG_ENTRY(0, 0x0),
    /* Explosives     */ DMG_ENTRY(1, 0xF),
    /* Zora boomerang */ DMG_ENTRY(1, 0xF),
    /* Normal arrow   */ DMG_ENTRY(1, 0xF),
    /* UNK_DMG_0x06   */ DMG_ENTRY(0, 0x0),
    /* Hookshot       */ DMG_ENTRY(1, 0xF),
    /* Goron punch    */ DMG_ENTRY(0, 0x0),
    /* Sword          */ DMG_ENTRY(0, 0x0),
    /* Goron pound    */ DMG_ENTRY(0, 0x0),
    /* Fire arrow     */ DMG_ENTRY(0, 0x0),
    /* Ice arrow      */ DMG_ENTRY(0, 0x0),
    /* Light arrow    */ DMG_ENTRY(2, 0x4),
    /* Goron spikes   */ DMG_ENTRY(0, 0x0),
    /* Deku spin      */ DMG_ENTRY(0, 0x0),
    /* Deku bubble    */ DMG_ENTRY(1, 0xF),
    /* Deku launch    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x12   */ DMG_ENTRY(0, 0x0),
    /* Zora barrier   */ DMG_ENTRY(1, 0x5),
    /* Normal shield  */ DMG_ENTRY(0, 0x0),
    /* Light ray      */ DMG_ENTRY(0, 0x0),
    /* Thrown object  */ DMG_ENTRY(1, 0xF),
    /* Zora punch     */ DMG_ENTRY(1, 0xF),
    /* Spin attack    */ DMG_ENTRY(0, 0x0),
    /* Sword beam     */ DMG_ENTRY(0, 0x0),
    /* Normal Roll    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1B   */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1C   */ DMG_ENTRY(0, 0x0),
    /* Unblockable    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1E   */ DMG_ENTRY(0, 0x0),
    /* Powder Keg     */ DMG_ENTRY(1, 0xF),
};

f32 D_80A338C0[PLAYER_FORM_MAX] = { 30.0f, 30.0f, 30.0f, 15.0f, 15.0f };

const ActorInit En_Pr_InitVars = {
    ACTOR_EN_PR,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_PR,
    sizeof(EnPr),
    (ActorFunc)EnPr_Init,
    (ActorFunc)EnPr_Destroy,
    (ActorFunc)EnPr_Update,
    (ActorFunc)EnPr_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK4,
        { 0x20000000, 0x00, 0x04 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_NONE,
    },
    { 18, 20, 5, { 0, 0, 0 } },
};

static AnimationHeader* sAnimations[] = {
    &object_pr_Anim_0021E8, &object_pr_Anim_001E10, &object_pr_Anim_0021E8,
    &object_pr_Anim_000740, &object_pr_Anim_000268,
};

u8 D_80A33934[] = {
    0, 2, 0, 0, 2,
};

void EnPr_Init(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    EnPr* this = THIS;
    EnPrz* prz;
    s32 i;
    s32 temp_s4;

    this->actor.colChkInfo.health = 3;
    this->actor.colChkInfo.mass = 50;
    this->actor.hintId = 0x5C;

    Collider_InitAndSetCylinder(globalCtx, &this->collider, &this->actor, &sCylinderInit);

    this->unk_208 = 255;
    this->unk_2CC = 0.01f;

    Actor_SetScale(&this->actor, 0.01f);

    this->actor.targetMode = 3;
    this->unk_2B8 = this->actor.world.pos.y;
    this->actor.shape.yOffset = 1500.0f;
    this->actor.colChkInfo.damageTable = &sDamageTable;

    SkelAnime_InitFlex(globalCtx, &this->skelAnime, &object_pr_Skel_0038B8, &object_pr_Anim_0021E8, this->jointTable,
                       this->morphTable, 10);
    this->unk_2C8 = this->actor.world.rot.z * 20.0f;

    if (this->unk_2C8 < 80.0f) {
        this->unk_2C8 = 80.0f;
    }
    this->actor.world.rot.z = 0;

    temp_s4 = ENPR_GET_FF00(&this->actor);
    if (temp_s4 > ENPR_FF00_MAX) {
        temp_s4 = ENPR_FF00_MAX;
    } else if (temp_s4 < ENPR_FF00_MIN) {
        temp_s4 = ENPR_FF00_MIN;
    }

    this->unk_2F8 = &this->actor;

    for (i = 0; i < temp_s4; i++) {
        prz = (EnPrz*)Actor_SpawnAsChild(&globalCtx->actorCtx, &this->actor, globalCtx, ACTOR_EN_PRZ,
                                         this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z, 0,
                                         this->actor.world.rot.y, 0, i + 1);
        if (prz != NULL) {
            prz->unk_220 = this->unk_2F8;
            this->unk_2F8 = &prz->actor;
        }
    }

    this->unk_2D0 = 255;
    this->unk_20E = 30;
    this->unk_216 = this->actor.world.rot.y;
    this->unk_214 = this->actor.world.rot.y;
    this->unk_2B4 = this->actor.world.pos.y;

    func_80A326F0(this);
}

void EnPr_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnPr* this = THIS;

    Collider_DestroyCylinder(globalCtx, &this->collider);
}

void func_80A3242C(EnPr* this, s32 arg0) {
    f32 sp34;

    this->unk_21C = arg0;
    sp34 = 1.0f;
    this->unk_2BC = Animation_GetLastFrame(sAnimations[arg0]);

    if (this->unk_21C == 2) {
        sp34 = 2.0f;
    }

    Animation_Change(&this->skelAnime, sAnimations[this->unk_21C], sp34, 0.0f, this->unk_2BC, D_80A33934[this->unk_21C],
                     -2.0f);
}

s32 func_80A324E0(EnPr* this, GlobalContext* globalCtx) {
    CollisionPoly* sp54;
    Vec3f sp48;
    s32 sp44;
    WaterBox* sp40;

    if (BgCheck_EntityLineTest1(&globalCtx->colCtx, &this->actor.world.pos, &this->unk_2E0, &sp48, &sp54, 1, 0, 0, 1,
                                &sp44)) {
        return 1;
    }

    if (WaterBox_GetSurface1(globalCtx, &globalCtx->colCtx, this->actor.world.pos.x, this->actor.world.pos.z,
                             &this->unk_2B4, &sp40)) {
        if ((this->unk_2B4 - 30.0f) < this->actor.world.pos.y) {
            this->unk_2B8 = this->unk_2B4 - 30.0f;
            return 2;
        }
    }

    if (this->actor.world.pos.y < (this->actor.floorHeight + 20.0f)) {
        this->unk_2B8 = this->actor.floorHeight + 20.0f;
        return 2;
    }
    return 0;
}

s32 func_80A325E4(EnPr* this) {
    s16 temp = this->unk_214 * 0.2f;
    s32 abs = ABS_ALT(temp) & 0xFFFF;

    Math_SmoothStepToS(&this->actor.world.rot.y, this->unk_22C, 1, abs + 1, 0);
    this->unk_216 = this->actor.world.rot.y - this->unk_22C;

    if (this->unk_216 > 10000) {
        this->unk_216 = 10000;
    } else if (this->unk_216 < -10000) {
        this->unk_216 = -10000;
    }

    if (ABS_ALT(BINANG_SUB(this->actor.world.rot.y, this->unk_22C)) > 0x100) {
        return true;
    }
    this->unk_216 = 0;
    return false;
}

void func_80A326F0(EnPr* this) {
    if (this->unk_21C != 0) {
        func_80A3242C(this, 0);
    }
    this->unk_206 = 0;
    this->actor.speedXZ = 1.0f;
    this->actionFunc = func_80A32740;
}

void func_80A32740(EnPr* this, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    switch (func_80A324E0(this, globalCtx)) {
        case 1:
            func_80A3295C(this);
            return;

        case 2:
            this->unk_206 = 3;
            this->actionFunc = func_80A32A40;
            break;
    }

    if (this->unk_2C8 < sqrtf(SQ(this->actor.world.pos.x - this->actor.home.pos.x) +
                              SQ(this->actor.world.pos.z - this->actor.home.pos.z))) {
        func_80A32854(this);
    } else if ((this->unk_20E == 0) && (player->stateFlags1 & 0x8000000)) {
        if (sqrtf(SQ(player->actor.world.pos.x - this->actor.home.pos.x) +
                  SQ(player->actor.world.pos.z - this->actor.home.pos.z)) < this->unk_2C8) {
            func_80A32AF8(this);
        }
    }
}

void func_80A32854(EnPr* this) {
    if (this->unk_21C != 0) {
        func_80A3242C(this, 0);
    }
    this->unk_206 = 1;
    this->actionFunc = func_80A3289C;
}

void func_80A3289C(EnPr* this, GlobalContext* globalCtx) {
    this->unk_22C = Math_Vec3f_Yaw(&this->actor.world.pos, &this->actor.home.pos);
    func_80A325E4(this);

    if (sqrtf(SQ(this->actor.world.pos.x - this->actor.home.pos.x) +
              SQ(this->actor.world.pos.z - this->actor.home.pos.z)) < (((BREG(53) * 0.1f) + 0.5f) * this->unk_2C8)) {
        this->unk_206 = 3;
        this->actionFunc = func_80A32A40;
    }
}

void func_80A3295C(EnPr* this) {
    this->unk_22C = BINANG_ADD(this->actor.world.rot.y, 0x4000);
    this->unk_206 = 2;
    this->actionFunc = func_80A32984;
}

void func_80A32984(EnPr* this, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if ((player->stateFlags1 & 0x8000000) && (this->unk_20E == 0)) {
        this->unk_22C = this->actor.world.rot.y;
        func_80A32AF8(this);
    } else if (!func_80A325E4(this)) {
        if (func_80A324E0(this, globalCtx)) {
            this->unk_22C += 0x1000;
        } else {
            if (this->unk_21C != 0) {
                func_80A3242C(this, 0);
            }
            this->unk_206 = 3;
            this->actionFunc = func_80A32A40;
        }
    }
}

void func_80A32A40(EnPr* this, GlobalContext* globalCtx) {
    Vec3f sp34;
    WaterBox* sp30;

    Math_Vec3f_Copy(&sp34, &this->actor.world.pos);
    sp34.y = randPlusMinusPoint5Scaled(50.0f) + this->actor.home.pos.y;

    if (WaterBox_GetSurface1(globalCtx, &globalCtx->colCtx, this->actor.world.pos.x, this->actor.world.pos.z,
                             &this->unk_2B4, &sp30)) {
        if (sp34.y < (this->unk_2B4 - 30.0f)) {
            this->unk_2B8 = sp34.y;
        } else {
            this->unk_2B8 = this->unk_2B4 - 30.0f;
        }
    }

    this->unk_206 = 0;
    this->actionFunc = func_80A32740;
}

void func_80A32AF8(EnPr* this) {
    this->unk_206 = 4;
    this->skelAnime.playSpeed = 2.0f;
    this->actionFunc = func_80A32B20;
}

void func_80A32B20(EnPr* this, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    this->actor.speedXZ = BREG(57) + 3.0f;
    Math_SmoothStepToS(&this->unk_22C, this->actor.yawTowardsPlayer, BREG(49) + 1, BREG(50) + 1000, BREG(51));
    this->unk_2B8 = D_80A338C0[(void)0, gSaveContext.playerForm] + player->actor.world.pos.y;
    func_80A324E0(this, globalCtx);

    if (!(player->stateFlags1 & 0x8000000)) {
        this->skelAnime.playSpeed = 1.0f;
        this->actor.speedXZ = 1.0f;
        func_80A32854(this);
    } else if (this->unk_2C8 < sqrtf(SQ(player->actor.world.pos.x - this->actor.home.pos.x) +
                                     SQ(player->actor.world.pos.z - this->actor.home.pos.z))) {
        this->skelAnime.playSpeed = 1.0f;
        this->actor.speedXZ = 1.0f;
        func_80A32854(this);
    } else if (!func_80A325E4(this) && (this->actor.xzDistToPlayer < 200.0f) &&
               (fabsf(player->actor.world.pos.y - this->actor.world.pos.y) < 80.0f)) {
        this->skelAnime.playSpeed = 1.0f;
        func_80A32CDC(this);
    }
}

void func_80A32CDC(EnPr* this) {
    func_80A3242C(this, 3);
    this->unk_206 = 5;
    this->unk_20A = 400;
    this->unk_2C0 = 0.0f;
    this->actionFunc = func_80A32D28;
}

void func_80A32D28(EnPr* this, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (!(player->stateFlags1 & 0x8000000)) {
        this->skelAnime.playSpeed = 1.0f;
        this->actor.speedXZ = 1.0f;
        func_80A32854(this);
    } else {
        Math_SmoothStepToS(&this->unk_22C, this->actor.yawTowardsPlayer, BREG(49) + 1, BREG(50) + 1000, BREG(51));
        func_80A325E4(this);
        this->unk_2B8 = D_80A338C0[(void)0, gSaveContext.playerForm] + player->actor.world.pos.y;
        func_80A324E0(this, globalCtx);
        if (this->unk_2C8 < sqrtf(SQ(player->actor.world.pos.x - this->actor.home.pos.x) +
                                  SQ(player->actor.world.pos.z - this->actor.home.pos.z))) {
            this->skelAnime.playSpeed = 1.0f;
            this->actor.speedXZ = 1.0f;
            func_80A32854(this);
        }
    }
}

void func_80A32E60(EnPr* this) {
    func_80A3242C(this, 4);
    this->unk_206 = 6;
    this->actor.speedXZ = 0.0f;
    this->actionFunc = func_80A32EA4;
}

void func_80A32EA4(EnPr* this, GlobalContext* globalCtx) {
    f32 curFrame = this->skelAnime.curFrame;

    if (this->unk_2BC <= curFrame) {
        if (this->actor.colChkInfo.health <= 0) {
            this->unk_206 = 7;
            this->unk_20A = 50;
            this->actor.flags |= ACTOR_FLAG_8000000;
            this->unk_2C4 = 0.0f;
            Enemy_StartFinishingBlow(globalCtx, &this->actor);
            Actor_PlaySfxAtPos(&this->actor, NA_SE_EN_BUBLEWALK_DEAD);
            this->unk_216 = 0;
            this->actionFunc = func_80A32F48;
        } else {
            func_80A326F0(this);
        }
    }
}

void func_80A32F48(EnPr* this, GlobalContext* globalCtx) {
    WaterBox* sp2C;

    if (this->unk_208 > 0) {
        this->unk_208 -= 2;
    } else {
        this->unk_208 = 0;
    }

    if (WaterBox_GetSurface1(globalCtx, &globalCtx->colCtx, this->actor.world.pos.x, this->actor.world.pos.z,
                             &this->unk_2B4, &sp2C)) {
        if ((this->unk_2B4 - 100.0f) < this->actor.world.pos.y) {
            this->unk_212 += 0xBB8;
            this->unk_2C4 = 2.0f * Math_SinS(this->unk_212);
        }
        Math_ApproachF(&this->actor.world.pos.y, (this->unk_2B4 - 16.0f) + this->unk_2C4, 0.5f, 2.0f);
        Math_SmoothStepToS(&this->actor.world.rot.z, 0x7700, 0x12C, 0x3E8, 0x3E8);
    }

    if ((this->unk_20A == 0) && (this->unk_208 == 0)) {
        this->unk_2D2 = 1;
    }

    if (this->unk_2D2 != 0) {
        Math_SmoothStepToS(&this->unk_2D0, 0, 1, 15, 50);
        if (this->unk_2D0 < 2) {
            Actor_MarkForDeath(&this->actor);
        }
    }
}

void func_80A33098(EnPr* this, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if ((this->unk_206 != 7) && (this->unk_206 != 6)) {
        if (this->collider.base.acFlags & AC_HIT) {
            this->collider.base.acFlags &= ~AC_HIT;
            if (this->actor.colChkInfo.damageEffect == 4) {
                this->unk_22E = 40;
                this->unk_230 = 20;
                Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_CLEAR_TAG, this->actor.focus.pos.x,
                            this->actor.focus.pos.y, this->actor.focus.pos.z, 0, 0, 0, CLEAR_TAG_LARGE_LIGHT_RAYS);
            }

            if ((player->stateFlags1 & 0x8000000) && (this->actor.colChkInfo.damageEffect == 5)) {
                this->unk_22E = 40;
                this->unk_230 = 31;
            }

            if ((this->unk_206 != 6) && (this->unk_206 != 7)) {
                Actor_ApplyDamage(&this->actor);
                Actor_SetColorFilter(&this->actor, 0x4000, 255, 0, 8);
                Actor_PlaySfxAtPos(&this->actor, NA_SE_EN_BUBLEWALK_DAMAGE);
                func_80A32E60(this);
            }
        }
    }
}

void EnPr_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnPr* this = THIS;
    s32 i;

    SkelAnime_Update(&this->skelAnime);

    if (this->unk_20A != 0) {
        this->unk_20A--;
    }

    if (this->unk_20E != 0) {
        this->unk_20E--;
    }

    if (this->unk_20C != 0) {
        this->unk_20C--;
    }

    if (this->unk_210 != 0) {
        this->unk_210--;
    }

    this->actionFunc(this, globalCtx);

    Actor_SetFocus(&this->actor, 20.0f);
    func_80A33098(this, globalCtx);
    Math_Vec3f_Copy(&this->unk_2E0, &this->actor.world.pos);

    this->unk_2E0.x += (Math_SinS(this->actor.world.rot.y) * 70.0f);
    this->unk_2E0.y = this->unk_2D4.y - 10.0f;
    this->unk_2E0.z += Math_CosS(this->actor.world.rot.y) * 70.0f;

    Math_SmoothStepToS(&this->unk_214, this->unk_216, 1, 0x7D0, 0);

    if (this->unk_210 == 0) {
        this->unk_210 = Rand_S16Offset(10, 30);
        this->unk_204 = 1;
    }

    Actor_MoveWithGravity(&this->actor);

    if (this->unk_206 != 7) {
        Vec3f sp50;
        s16 pitch;

        Math_ApproachF(&this->actor.world.pos.y, this->unk_2B8, 0.3f, BREG(11) + 1.0f);
        Math_Vec3f_Copy(&sp50, &this->actor.world.pos);
        sp50.y = this->unk_2B8;
        pitch = Math_Vec3f_Pitch(&this->actor.world.pos, &sp50) * 0.3f;

        if (fabsf(this->actor.world.pos.y - this->unk_2B8) > 8.0f) {
            Math_SmoothStepToS(&this->actor.world.rot.x, pitch, 1, BREG(48) + 500, 0);
        } else {
            Math_SmoothStepToS(&this->actor.world.rot.x, 0, 1, BREG(52) + 500, 0);
        }
    }

    Actor_UpdateBgCheckInfo(globalCtx, &this->actor, 0.0f, 30.0f, 20.0f, 0x1D);
    Math_Vec3s_Copy(&this->actor.shape.rot, &this->actor.world.rot);

    if (this->unk_206 != 7) {
        Collider_UpdateCylinder(&this->actor, &this->collider);
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &this->collider.base);
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &this->collider.base);

        if (this->unk_204 != 0) {
            Vec3f sp40;

            Math_Vec3f_Copy(&sp40, &this->unk_2D4);
            this->unk_204 = 0;
            sp40.x += randPlusMinusPoint5Scaled(20.0f);
            sp40.y += randPlusMinusPoint5Scaled(5.0f);
            sp40.z += randPlusMinusPoint5Scaled(20.0f);

            for (i = 0; i < (s32)Rand_ZeroFloat(5.0f) + 5; i++) {
                EffectSsBubble_Spawn(globalCtx, &sp40, 0.0f, 5.0f, 5.0f, Rand_ZeroFloat(0.03f) + 0.07f);
            }
        }
    }
}

s32 EnPr_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    EnPr* this = THIS;

    if (limbIndex == 2) {
        rot->y += this->unk_214;
    }
    return false;
}

void EnPr_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
    Vec3f sp24 = { 0.0f, 0.0f, 0.0f };
    EnPr* this = THIS;

    if (limbIndex == 2) {
        Matrix_InsertTranslation(0.0f, 0.0f, 0.0f, MTXMODE_APPLY);
        Matrix_MultiplyVector3fByState(&sp24, &this->unk_2D4);
    }

    if ((limbIndex == 0) || (limbIndex == 1) || (limbIndex == 2) || (limbIndex == 3) || (limbIndex == 4) ||
        (limbIndex == 5) || (limbIndex == 6) || (limbIndex == 7) || (limbIndex == 8) || (limbIndex == 9)) {
        Matrix_GetStateTranslation(&this->unk_23C[this->unk_228]);
        this->unk_228++;
        if (this->unk_228 >= ARRAY_COUNT(this->unk_23C)) {
            this->unk_228 = 0;
        }
    }
}

void EnPr_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnPr* this = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx);

    func_8012C28C(globalCtx->state.gfxCtx);
    func_8012C2DC(globalCtx->state.gfxCtx);

    if (this->unk_2D2 == 0) {
        gDPPipeSync(POLY_OPA_DISP++);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, this->unk_208, this->unk_208, this->unk_208, this->unk_2D0);
        gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, this->unk_2D0);

        Scene_SetRenderModeXlu(globalCtx, 0, 1);
        SkelAnime_DrawFlexOpa(globalCtx, this->skelAnime.skeleton, this->skelAnime.jointTable,
                              this->skelAnime.dListCount, EnPr_OverrideLimbDraw, EnPr_PostLimbDraw, &this->actor);
    } else {
        gDPPipeSync(POLY_XLU_DISP++);
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, this->unk_2D0);

        Scene_SetRenderModeXlu(globalCtx, 1, 2);
        POLY_XLU_DISP = SkelAnime_DrawFlex(globalCtx, this->skelAnime.skeleton, this->skelAnime.jointTable,
                                           this->skelAnime.dListCount, NULL, NULL, &this->actor, POLY_XLU_DISP);
    }

    if (this->unk_22E != 0) {
        f32 temp = this->unk_22E * 0.05f;

        this->unk_238 = 0.8f;
        this->unk_234 = 0.8f;
        func_800BE680(globalCtx, &this->actor, this->unk_23C, 10, 0.8f, 0.8f, temp, this->unk_230);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx);
}
