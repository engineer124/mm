/*
 * File: z_en_elf.c
 * Overlay: ovl_En_Elf
 * Description: Fairies
 */

#include "z_en_elf.h"

#include "z64elf_message.h"

#define FLAGS (ACTOR_FLAG_10 | ACTOR_FLAG_20 | ACTOR_FLAG_UPDATE_DURING_OCARINA)

#define THIS ((EnElf*)thisx)

void EnElf_Init(Actor* thisx, PlayState* play2);
void EnElf_Destroy(Actor* thisx, PlayState* play);
void EnElf_Update(Actor* thisx, PlayState* play);
void EnElf_Draw(Actor* thisx, PlayState* play);

void EnElf_SetupAction(EnElf* this, EnElfActionFunc actionFunc);

void EnElf_UnkFunc_3(EnElf* this, PlayState* play);
void EnElf_UnkFunc_0(EnElf* this, PlayState* play);
void EnElf_UnkFunc_1(EnElf* this, PlayState* play);
void EnElf_UnkFunc_2(EnElf* this, PlayState* play);

void EnElf_Action_0(EnElf* this, PlayState* play);
void EnElf_Action_4(EnElf* this, PlayState* play);
void EnElf_Spawner_DoNothing(EnElf* this, PlayState* play);
void EnElf_Action_2(EnElf* this, PlayState* play);
void EnElf_Action_3(EnElf* this, PlayState* play);
void EnElf_Tatl_Action(EnElf* this, PlayState* play);

void EnElf_Tatl_UpdateMisc2Tatl(EnElf* this, PlayState* play);
void EnElf_Tatl_UpdateMisc1(EnElf* this, PlayState* play);
void EnElf_SpawnSparkles(EnElf* this, PlayState* play, s32 sparkleLife);
void EnElf_Tatl_Update(Actor* thisx, PlayState* play);
void EnElf_CutsceneTranslate(Vec3f* dest, PlayState* play, s32 cueChannel);

ActorProfile En_Elf_Profile = {
    /**/ ACTOR_EN_ELF,
    /**/ ACTORCAT_ITEMACTION,
    /**/ FLAGS,
    /**/ GAMEPLAY_KEEP,
    /**/ sizeof(EnElf),
    /**/ EnElf_Init,
    /**/ EnElf_Destroy,
    /**/ EnElf_Update,
    /**/ EnElf_Draw,
};

void EnElf_SetupAction(EnElf* this, EnElfActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void EnElf_ChangeState(EnElf* this, s32 fairyState) {
    this->fairyState = fairyState;

    switch (this->fairyState) {
        case FAIRY_STATE_0:
            this->unkYawAngPitch = 0x400;
            this->unkYawAngVel = 0x200;
            this->unkFunc = EnElf_UnkFunc_0;
            this->unkTimer = 100;
            this->skelAnime.playSpeed = 1.0f;
            this->unk_250 = 5.0f;
            this->unk_254 = 20.0f;
            break;

        case FAIRY_STATE_10:
            this->unkYawAngPitch = 0x400;
            this->unkYawAngVel = 0x200;
            this->unkFunc = EnElf_UnkFunc_0;
            this->unkTimer = 100;
            this->unk_250 = 1.0f;
            this->skelAnime.playSpeed = 1.0f;
            this->unk_254 = 5.0f;
            break;

        case FAIRY_STATE_8:
            this->unkYawAngPitch = 0x400;
            this->unkYawAngVel = 0;
            this->unkFunc = EnElf_UnkFunc_0;
            this->skelAnime.playSpeed = 1.0f;
            this->unk_254 = 0.0f;
            this->unk_250 = 5.0f;
            break;

        case FAIRY_STATE_7:
            this->unkYawAngPitch = 0x1000;
            this->unkYawAngVel = 0x200;
            this->unkFunc = EnElf_UnkFunc_0;
            this->skelAnime.playSpeed = 1.0f;
            this->unk_250 = 3.0f;
            this->unk_254 = 10.0f;
            break;

        case FAIRY_STATE_5:
            this->unkFunc = EnElf_UnkFunc_0;
            this->unkYawAngPitch = 0x1E;
            this->unkTimer = 1;
            this->unk_250 = 0.0f;
            this->unk_254 = 0.0f;
            this->skelAnime.playSpeed = 1.0f;
            break;

        case FAIRY_STATE_6:
            this->unkYawAngPitch = 0x1000;
            this->unkYawAngVel = 0x200;
            this->unkFunc = EnElf_UnkFunc_0;
            this->unk_250 = 0.0f;
            this->unk_254 = 0.0f;
            this->skelAnime.playSpeed = 1.0f;
            break;

        case FAIRY_STATE_1:
            this->unkYawAngPitch = 0x1000;
            this->unkYawAngVel = 0x800;
            this->unkFunc = EnElf_UnkFunc_0;
            this->unk_250 = 5.0f;
            this->unk_254 = 7.5f;
            this->skelAnime.playSpeed = 2.0f;
            break;

        case FAIRY_STATE_2:
            this->unkYawAngPitch = 0x400;
            this->unkYawAngVel = 0x1000;
            this->unkFunc = EnElf_UnkFunc_1;
            this->skelAnime.playSpeed = 1.0f;
            this->unk_250 = 10.0f;
            this->unk_254 = 20.0f;
            break;

        case FAIRY_STATE_3:
            this->unkYawAngVel = 0x600;
            this->unkFunc = EnElf_UnkFunc_2;
            this->unk_254 = 1.0f;
            this->unk_250 = 1.0f;
            this->skelAnime.playSpeed = 1.0f;
            break;

        case FAIRY_STATE_4:
            this->unkYawAngPitch = 0x1000;
            this->unkYawAngVel = 0x800;
            this->unkFunc = EnElf_UnkFunc_0;
            this->unk_250 = 60.0f;
            this->unk_254 = 20.0f;
            this->skelAnime.playSpeed = 2.0f;
            break;

        case FAIRY_STATE_9:
            this->unkYawAngPitch = 0x400;
            this->unkYawAngVel = 0x2000;
            this->unkFunc = EnElf_UnkFunc_0;
            this->unkTimer = 42;
            this->unk_254 = 1.0f;
            this->skelAnime.playSpeed = 1.0f;
            this->unk_250 = 5.0f;
            break;

        default:
            break;
    }
}

s32 EnElf_IsOutsideRangeXZ(Vec3f* posA, Vec3f* posB, f32 dist) {
    return SQ(posB->x - posA->x) + SQ(posB->z - posA->z) > SQ(dist);
}

void EnElf_UnkFunc_5(EnElf* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (!EnElf_IsOutsideRangeXZ(&this->actor.world.pos, &player->actor.world.pos, 30.0f)) {
        this->unk_254 = 0.5f;
    } else {
        this->unk_254 = 2.0f;
    }
    if (this->unkTimer > 0) {
        this->unkTimer--;
    } else {
        this->fairyState = FAIRY_STATE_1;
        this->unkYaw = 128;
        this->unk_254 = Rand_ZeroFloat(1.0f) + 0.5f;
        this->unkYawAngVel = Rand_CenteredFloat(0x7FFF);
        this->unkFunc = EnElf_UnkFunc_3;
    }
}

void EnElf_UnkFunc_4(EnElf* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (EnElf_IsOutsideRangeXZ(&this->actor.world.pos, &player->actor.world.pos, 50.0f)) {
        if (this->unkTimer > 0) {
            this->unkTimer--;
        } else {
            this->fairyState = FAIRY_STATE_1;
            this->unkYaw = 128;
            this->unk_254 = Rand_ZeroFloat(1.0f) + 0.5f;
            this->unkYawAngVel = Rand_CenteredFloat(0x7FFF);
            this->unkFunc = EnElf_UnkFunc_3;
        }
    }
}

void EnElf_UnkFunc_3(EnElf* this, PlayState* play) {
    f32 xzDistToPlayer;

    if (this->unkTimer > 0) {
        this->unkTimer--;
    } else {
        xzDistToPlayer = this->actor.xzDistToPlayer;

        if (xzDistToPlayer < 50.0f) {
            if (Rand_ZeroOne() < 0.2f) {
                this->fairyState = FAIRY_STATE_2;
                this->unkYaw = 0x400;
                this->unk_254 = 2.0f;
                this->actor.speed = 1.5f;
                this->unkFunc = EnElf_UnkFunc_4;
                this->unkTimer = (s32)Rand_ZeroFloat(8.0f) + 4;
            } else {
                this->unkTimer = 10;
            }
        } else {
            if (xzDistToPlayer > 150.0f) {
                xzDistToPlayer = 150.0f;
            }

            xzDistToPlayer = ((xzDistToPlayer - 50.0f) * 0.95f) + 0.05f;

            if (Rand_ZeroOne() < xzDistToPlayer) {
                this->fairyState = FAIRY_STATE_3;
                this->unkYaw = 0x200;
                this->unk_254 = (2.0f * xzDistToPlayer) + 1.0f;
                this->unkFunc = EnElf_UnkFunc_5;
                this->unkTimer = (s32)Rand_ZeroFloat(16.0f) + 16;
            } else {
                this->unkTimer = 10;
            }
        }
    }

    if (Rand_ZeroOne() < 0.1f) {
        this->fairyState = FAIRY_STATE_1;
        this->unkYaw = 128;
        this->unk_254 = Rand_ZeroFloat(0.5f) + 0.5f;
        this->unkYawAngVel = Rand_CenteredFloat(0x7FFF);
    }
}

void func_8088CBAC(EnElf* this, PlayState* play) {
    f32 phi_f0;

    if (this->fairyFlags & FAIRY_FLAG_14) {
        phi_f0 = 0.0f;
    } else {
        phi_f0 = 100.0f;
    }

    if (EnElf_IsOutsideRangeXZ(&this->unkPos, &this->actor.world.pos, phi_f0)) {
        this->fairyState = FAIRY_STATE_0;
        this->unkYaw = 0x200;
        this->unk_254 = 1.5f;
        this->unkFunc = EnElf_UnkFunc_3;
    } else {
        this->unkFunc(this, play);
    }
}

void EnElf_SetupAction_0(EnElf* this, PlayState* play) {
    EnElf_SetupAction(this, EnElf_Action_0);
    this->unk_250 = Rand_ZeroFloat(10.0f) + 10.0f;
    this->unkPitch = 0;
    this->unkYawAngPitch = (s32)Rand_ZeroFloat(1048.0f) + 0x200;
    this->unkPos = this->actor.world.pos;
    this->yaw = Rand_CenteredFloat(0x7FFF);
    this->unkFunc = EnElf_UnkFunc_3;
    func_8088CBAC(this, play);
    this->unkTimer = 0;
    this->disappearTimer = 240;
    if ((this->fairyFlags & FAIRY_FLAG_10) && Flags_GetCollectible(play, this->collectibleFlag)) {
        Actor_Kill(&this->actor);
    }
}

typedef enum {
    /* 0 */ FAIRY_COLOR_ZERO,
    /* 1 */ FAIRY_COLOR_RAND_ABOVE_200,
    /* 2 */ FAIRY_COLOR_RAND_FULL
} FairyColorFlag;

f32 EnElf_GetColor(s32 colorFlag) {
    switch (colorFlag) {
        case FAIRY_COLOR_RAND_ABOVE_200:
            return Rand_ZeroFloat(55.0f) + 200.0f;

        case FAIRY_COLOR_RAND_FULL:
            return Rand_ZeroFloat(255.0f);

        default: // FAIRY_COLOR_ZERO
            return 0.0f;
    }
}

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 8, ICHAIN_STOP),
};

static Color_RGBAf sInnerColors[] = {
    { 255.0f, 255.0f, 230.0f, 255.0f }, // FAIRY_COLOR_CONFIG_0
    { 255.0f, 220.0f, 220.0f, 255.0f }, // FAIRY_COLOR_CONFIG_N1
    { 255.0f, 235.0f, 220.0f, 255.0f }, // FAIRY_COLOR_CONFIG_N2
};

static Color_RGBAf sOuterColors[] = {
    { 220.0f, 160.0f, 80.0f, 255.0f }, // FAIRY_COLOR_CONFIG_0
    { 255.0f, 50.0f, 100.0f, 255.0f }, // FAIRY_COLOR_CONFIG_N1
    { 255.0f, 150.0f, 0.0f, 255.0f },  // FAIRY_COLOR_CONFIG_N2
};

typedef enum {
    /* -2 */ FAIRY_COLOR_CONFIG_N2 = -2,
    /* -1 */ FAIRY_COLOR_CONFIG_N1,
    /*  0 */ FAIRY_COLOR_CONFIG_0,
    /*  1 */ FAIRY_COLOR_CONFIG_1,
    /*  2 */ FAIRY_COLOR_CONFIG_2,
    /*  3 */ FAIRY_COLOR_CONFIG_3,
    /*  4 */ FAIRY_COLOR_CONFIG_4,
    /*  5 */ FAIRY_COLOR_CONFIG_5,
    /*  6 */ FAIRY_COLOR_CONFIG_6,
    /*  7 */ FAIRY_COLOR_CONFIG_7,
    /*  8 */ FAIRY_COLOR_CONFIG_8,
    /*  9 */ FAIRY_COLOR_CONFIG_9,
    /* 10 */ FAIRY_COLOR_CONFIG_10,
    /* 11 */ FAIRY_COLOR_CONFIG_11,
    /* 12 */ FAIRY_COLOR_CONFIG_12,
    /* 13 */ FAIRY_COLOR_CONFIG_MAX
} FairyColorConfig;

typedef struct {
    /* 0x0 */ u8 r;
    /* 0x1 */ u8 g;
    /* 0x2 */ u8 b;
} FairyColorFlags; // size = 0x3

static FairyColorFlags sColorFlags[FAIRY_COLOR_CONFIG_MAX] = {
    { FAIRY_COLOR_ZERO, FAIRY_COLOR_ZERO, FAIRY_COLOR_ZERO },                     //
    { FAIRY_COLOR_RAND_ABOVE_200, FAIRY_COLOR_ZERO, FAIRY_COLOR_ZERO },           // FAIRY_COLOR_CONFIG_1
    { FAIRY_COLOR_RAND_ABOVE_200, FAIRY_COLOR_RAND_FULL, FAIRY_COLOR_ZERO },      // FAIRY_COLOR_CONFIG_2
    { FAIRY_COLOR_RAND_ABOVE_200, FAIRY_COLOR_ZERO, FAIRY_COLOR_RAND_FULL },      // FAIRY_COLOR_CONFIG_3
    { FAIRY_COLOR_ZERO, FAIRY_COLOR_RAND_ABOVE_200, FAIRY_COLOR_ZERO },           // FAIRY_COLOR_CONFIG_4
    { FAIRY_COLOR_RAND_FULL, FAIRY_COLOR_RAND_ABOVE_200, FAIRY_COLOR_ZERO },      // FAIRY_COLOR_CONFIG_5
    { FAIRY_COLOR_ZERO, FAIRY_COLOR_RAND_ABOVE_200, FAIRY_COLOR_RAND_FULL },      // FAIRY_COLOR_CONFIG_6
    { FAIRY_COLOR_ZERO, FAIRY_COLOR_ZERO, FAIRY_COLOR_RAND_ABOVE_200 },           // FAIRY_COLOR_CONFIG_7
    { FAIRY_COLOR_RAND_FULL, FAIRY_COLOR_ZERO, FAIRY_COLOR_RAND_ABOVE_200 },      // FAIRY_COLOR_CONFIG_8
    { FAIRY_COLOR_ZERO, FAIRY_COLOR_RAND_FULL, FAIRY_COLOR_RAND_ABOVE_200 },      // FAIRY_COLOR_CONFIG_9
    { FAIRY_COLOR_RAND_ABOVE_200, FAIRY_COLOR_RAND_ABOVE_200, FAIRY_COLOR_ZERO }, // FAIRY_COLOR_CONFIG_10
    { FAIRY_COLOR_RAND_ABOVE_200, FAIRY_COLOR_ZERO, FAIRY_COLOR_RAND_ABOVE_200 }, // FAIRY_COLOR_CONFIG_11
    { FAIRY_COLOR_ZERO, FAIRY_COLOR_RAND_ABOVE_200, FAIRY_COLOR_RAND_ABOVE_200 }, // FAIRY_COLOR_CONFIG_12
};

void EnElf_Init(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EnElf* this = THIS;
    Player* player = GET_PLAYER(play);
    s32 colorConfig;
    s32 fairyType;

    Actor_ProcessInitChain(thisx, sInitChain);
    SkelAnime_Init(play, &this->skelAnime, &gameplay_keep_Skel_02AF58.sh, &gameplay_keep_Anim_029140, this->jointTable,
                   this->morphTable, FAIRY_LIMB_MAX);
    ActorShape_Init(&thisx->shape, 0.0f, NULL, 15.0f);
    thisx->shape.shadowAlpha = 255;

    Lights_PointGlowSetInfo(&this->lightInfoGlow, thisx->world.pos.x, thisx->world.pos.y, thisx->world.pos.z, 255, 255,
                            255, 0);
    this->lightNodeGlow = LightContext_InsertLight(play, &play->lightCtx, &this->lightInfoGlow);
    Lights_PointNoGlowSetInfo(&this->lightInfoNoGlow, thisx->world.pos.x, thisx->world.pos.y, thisx->world.pos.z, 255,
                              255, 255, 0);
    this->lightNodeNoGlow = LightContext_InsertLight(play, &play->lightCtx, &this->lightInfoNoGlow);

    this->fairyFlags = 0;
    this->disappearTimer = 600;
    this->unk_240 = 0.0f;
    colorConfig = FAIRY_COLOR_CONFIG_0;

    this->collectibleFlag = FAIRY_GET_COLLECTIBLE_FLAG(&this->actor);
    fairyType = FAIRY_GET_TYPE(&this->actor);
    if (FAIRY_GET_BOOL_PARAM(thisx)) {
        this->fairyFlags |= FAIRY_FLAG_10;
    }

    this->actor.params = fairyType;
    switch (fairyType) {
        case FAIRY_TYPE_TATL:
            thisx->room = -1;
            EnElf_SetupAction(this, EnElf_Tatl_Action);
            EnElf_ChangeState(this, FAIRY_STATE_0);
            this->fairyFlags |= FAIRY_FLAG_TATL;
            thisx->update = EnElf_Tatl_Update;
            this->tatlHintActor = NULL;
            this->unk_234 = NULL;
            this->unk_269 = 20;
            if ((gSaveContext.save.saveInfo.playerData.tatlTimer >= 25800) ||
                (gSaveContext.save.saveInfo.playerData.tatlTimer < 3000)) {
                gSaveContext.save.saveInfo.playerData.tatlTimer = 0;
            }
            this->hintTextId = QuestHint_GetTatlTextId(play);
            break;

        case FAIRY_TYPE_BOTTLE_RELEASE_1:
            colorConfig = FAIRY_COLOR_CONFIG_N1;
            gSaveContext.jinxTimer = 0;
            EnElf_SetupAction(this, EnElf_Action_2);
            this->unk_254 = Math_Vec3f_DistXZ(&thisx->world.pos, &player->actor.world.pos);
            this->unkYaw = player->actor.shape.rot.y;
            this->unkYawAngVel = -0x1000;
            this->unkPos.y = thisx->world.pos.y - player->actor.world.pos.y;
            this->unkPitch = 0;
            this->unk_250 = 0.0f;
            break;

        case FAIRY_TYPE_REVIVE_DEATH:
            colorConfig = FAIRY_COLOR_CONFIG_N1;
            EnElf_SetupAction(this, EnElf_Action_3);
            this->unk_254 = 0.0f;
            this->unkYaw = player->actor.shape.rot.y;
            this->unkYawAngVel = 0;
            this->unkPos.y = thisx->world.pos.y - player->actor.world.pos.y;
            this->unkPitch = 0;
            this->unk_250 = 7.0f;
            break;

        case FAIRY_TYPE_HEAL_BIG:
            this->fairyFlags |= FAIRY_FLAG_BIG;
            thisx->shape.shadowDraw = ActorShadow_DrawWhiteCircle;
            this->fairyFlags |= FAIRY_FLAG_TIMED;
            colorConfig = FAIRY_COLOR_CONFIG_N1;
            this->fairyFlags |= FAIRY_FLAG_11;
            this->fairyFlags |= FAIRY_FLAG_12;
            EnElf_SetupAction_0(this, play);
            break;

        case FAIRY_TYPE_HEAL_TIMED:
            this->fairyFlags |= FAIRY_FLAG_TIMED;
            colorConfig = FAIRY_COLOR_CONFIG_N1;
            this->fairyFlags |= FAIRY_FLAG_11;
            this->fairyFlags |= FAIRY_FLAG_13;
            this->fairyFlags |= FAIRY_FLAG_12;
            EnElf_SetupAction_0(this, play);
            break;

        case FAIRY_TYPE_HEAL:
            colorConfig = FAIRY_COLOR_CONFIG_N1;
            this->fairyFlags |= FAIRY_FLAG_11;
            this->fairyFlags |= FAIRY_FLAG_13;
            this->fairyFlags |= FAIRY_FLAG_12;
            EnElf_SetupAction_0(this, play);
            break;

        case FAIRY_TYPE_9:
            this->fairyFlags |= FAIRY_FLAG_12;
            // fallthrough
        case FAIRY_TYPE_10:
            colorConfig = FAIRY_COLOR_CONFIG_N2;
            EnElf_SetupAction_0(this, play);
            break;

        case FAIRY_TYPE_BOTTLE_RELEASE_0:
            Actor_Kill(thisx);
            return;

        case FAIRY_TYPE_KOKIRI:
            colorConfig = Rand_ZeroFloat(FAIRY_COLOR_CONFIG_12 - 0.01f) + 1.0f;
            EnElf_SetupAction(this, EnElf_Action_4);
            EnElf_ChangeState(this, FAIRY_STATE_0);
            break;

        case FAIRY_TYPE_SPAWNER:
            EnElf_SetupAction(this, EnElf_Spawner_DoNothing);
            EnElf_ChangeState(this, FAIRY_STATE_6);
            this->fairyFlags |= FAIRY_FLAG_NO_DRAW;
            {
                s32 i;

                for (i = 0; i < 8; i++) {
                    Actor_Spawn(&play->actorCtx, play, ACTOR_EN_ELF, thisx->world.pos.x, thisx->world.pos.y - 30.0f,
                                thisx->world.pos.z, 0, 0, 0, FAIRY_PARAMS(FAIRY_TYPE_HEAL, false, 0));
                }
            }
            break;

        default:
            break;
    }

    this->unk_23C = 3.0f;

    this->innerColor = sInnerColors[FAIRY_COLOR_CONFIG_0];
    if (colorConfig > 0) {
        this->outerColor.r = EnElf_GetColor(sColorFlags[colorConfig].r);
        this->outerColor.g = EnElf_GetColor(sColorFlags[colorConfig].g);
        this->outerColor.b = EnElf_GetColor(sColorFlags[colorConfig].b);
        this->outerColor.a = 0;
    } else {
        this->innerColor = sInnerColors[-colorConfig];
        this->outerColor = sOuterColors[-colorConfig];
    }
}

void EnElf_Destroy(Actor* thisx, PlayState* play) {
    s32 pad;
    EnElf* this = THIS;

    LightContext_RemoveLight(play, &play->lightCtx, this->lightNodeGlow);
    LightContext_RemoveLight(play, &play->lightCtx, this->lightNodeNoGlow);
}

void EnElf_UnkFunc_0(EnElf* this, PlayState* play) {
    this->unkPos.x = Math_SinS(this->unkYaw) * this->unk_254;
    this->unkPos.y = Math_SinS(this->unkPitch) * this->unk_250;
    this->unkPos.z = Math_CosS(this->unkYaw) * this->unk_254;
    this->unkYaw += this->unkYawAngVel;
    this->unkPitch += this->unkYawAngPitch;
}

void EnElf_UnkFunc_1(EnElf* this, PlayState* play) {
    f32 xzScale = (Math_CosS(this->unkPitch) * this->unk_250) + this->unk_254;

    this->unkPos.x = Math_SinS(this->unkYaw) * xzScale;
    this->unkPos.y = 0.0f;
    this->unkPos.z = Math_CosS(this->unkYaw) * xzScale;

    this->unkYaw += this->unkYawAngVel;
    this->unkPitch += this->unkYawAngPitch;
}

void EnElf_UnkFunc_2(EnElf* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    this->unkPitch = (this->unkYaw * 2) & 0xFFFF;
    this->unkPos.x = Math_SinS(this->unkYaw) * this->unk_254;
    this->unkPos.y = Math_SinS(this->unkPitch) * this->unk_250;
    this->unkPos.z = -Math_SinS(player->actor.shape.rot.y) * this->unkPos.x;
    this->unkPos.x = Math_CosS(player->actor.shape.rot.y) * this->unkPos.x;
    this->unkYaw += this->unkYawAngVel;
}

void EnElf_StepVerticalSpeed1(EnElf* this, Vec3f* arg1, f32 arg2) {
    f32 velocityYTarget = ((arg1->y + this->unkPos.y) - this->actor.world.pos.y) * arg2;
    f32 speedYTarget = fabsf(velocityYTarget);
    f32 speedYDir = (velocityYTarget >= 0.0f) ? 1.0f : -1.0f;

    speedYTarget = CLAMP(speedYTarget, 0.0f, 30.0f);
    Math_StepToF(&this->actor.velocity.y, speedYTarget * speedYDir, 32.0f);
}

void func_8088D660(EnElf* this, Vec3f* targetPos, f32 arg2) {
    f32 xVelTarget = ((targetPos->x + this->unkPos.x) - this->actor.world.pos.x) * arg2;
    f32 zVelTarget = ((targetPos->z + this->unkPos.z) - this->actor.world.pos.z) * arg2;
    f32 xVelDirection = (xVelTarget >= 0.0f) ? 1.0f : -1.0f;
    f32 zVelDirection = (zVelTarget >= 0.0f) ? 1.0f : -1.0f;

    xVelTarget = fabsf(xVelTarget);
    zVelTarget = fabsf(zVelTarget);

    xVelTarget = CLAMP(xVelTarget, 0.0f, 30.0f) * xVelDirection;
    zVelTarget = CLAMP(zVelTarget, 0.0f, 30.0f) * zVelDirection;

    EnElf_StepVerticalSpeed1(this, targetPos, arg2);
    Math_StepToF(&this->actor.velocity.x, xVelTarget, 1.5f);
    Math_StepToF(&this->actor.velocity.z, zVelTarget, 1.5f);
    Actor_UpdatePos(&this->actor);
}

void func_8088D7F8(EnElf* this, Vec3f* targetPos) {
    EnElf_StepVerticalSpeed1(this, targetPos, 0.2f);
    this->actor.velocity.x = 0.0f;
    this->actor.velocity.z = 0.0f;
    Actor_UpdatePos(&this->actor);
    this->actor.world.pos.x = targetPos->x + this->unkPos.x;
    this->actor.world.pos.z = targetPos->z + this->unkPos.z;
}

void func_8088D864(EnElf* this, Vec3f* targetPos) {
    EnElf_StepVerticalSpeed1(this, targetPos, 0.2f);
    this->actor.velocity.z = 0.0f;
    this->actor.velocity.x = 0.0f;
    Actor_UpdatePos(&this->actor);
    this->actor.world.pos.x = targetPos->x + this->unkPos.x;
    this->actor.world.pos.z = targetPos->z + this->unkPos.z;
}

void EnElf_StepVerticalSpeed2(EnElf* this, Vec3f* arg1) {
    f32 velocityYTarget = (((Math_SinS(this->unkPitch) * this->unk_250) + arg1->y) - this->actor.world.pos.y) * 0.2f;
    f32 speedYTarget = fabsf(velocityYTarget);
    f32 speedYDir = (velocityYTarget >= 0.0f) ? 1.0f : -1.0f;

    this->unkPitch += this->unkYawAngPitch;
    speedYTarget = CLAMP(speedYTarget, 0.0f, 30.0f);

    Math_StepToF(&this->actor.velocity.y, speedYTarget * speedYDir, 1.5f);
}

void func_8088D9BC(EnElf* this, PlayState* play) {
    s32 pad[2];
    Player* player = GET_PLAYER(play);
    s16 targetYaw;
    Vec3f* vec = &this->unkPos;

    if (this->fairyFlags & FAIRY_FLAG_14) {
        Math_SmoothStepToF(&this->actor.speed, 5.0f, 0.5f, 1.0f, 0.01f);
    } else {
        Math_SmoothStepToF(&this->actor.speed, this->unk_254, 0.2f, 0.5f, 0.01f);
    }

    switch (this->fairyState) {
        case FAIRY_STATE_0:
            targetYaw = Math_Atan2S_XY(-(this->actor.world.pos.z - vec->z), -(this->actor.world.pos.x - vec->x));
            break;

        case FAIRY_STATE_3:
            targetYaw = Math_Atan2S_XY(-(this->actor.world.pos.z - player->actor.world.pos.z),
                                       -(this->actor.world.pos.x - player->actor.world.pos.x));
            break;

        case FAIRY_STATE_2:
            targetYaw = Math_Atan2S_XY(this->actor.world.pos.z - player->actor.world.pos.z,
                                       this->actor.world.pos.x - player->actor.world.pos.x);
            break;

        default:
            targetYaw = this->unkYawAngVel;
            break;
    }

    if (this->fairyFlags & FAIRY_FLAG_14) {
        Math_SmoothStepToS(&this->yaw, targetYaw, 10, this->unkYaw, 0x1000);
    } else {
        Math_SmoothStepToS(&this->yaw, targetYaw, 10, this->unkYaw, 0x20);
    }

    this->actor.world.rot.y = this->yaw;
    Actor_MoveWithGravity(&this->actor);
}

void EnElf_UpdateMovement(EnElf* this, Vec3f* arg1, f32 arg2, f32 arg3, f32 arg4) {
    f32 xVelTarget = ((arg1->x + this->unkPos.x) - this->actor.world.pos.x) * arg4;
    f32 zVelTarget = ((arg1->z + this->unkPos.z) - this->actor.world.pos.z) * arg4;
    f32 xzVelocity;
    f32 clampedXZ;

    arg4 += 0.3f;
    arg3 += 30.0f;

    EnElf_StepVerticalSpeed1(this, arg1, arg4);

    xzVelocity = sqrtf(SQ(xVelTarget) + SQ(zVelTarget));
    clampedXZ = CLAMP(xzVelocity, arg2, arg3);
    this->actor.speed = clampedXZ;

    if ((xzVelocity != clampedXZ) && (xzVelocity != 0.0f)) {
        xzVelocity = clampedXZ / xzVelocity;
        xVelTarget *= xzVelocity;
        zVelTarget *= xzVelocity;
    }

    Math_StepToF(&this->actor.velocity.x, xVelTarget, 5.0f);
    Math_StepToF(&this->actor.velocity.z, zVelTarget, 5.0f);
    Actor_UpdatePos(&this->actor);
}

s32 EnElf_TryDisappearing(EnElf* this) {
    if (this->disappearTimer > 0) {
        this->disappearTimer--;
    } else {
        this->disappearTimer--;
        if (this->disappearTimer > -10) {
            Actor_SetScale(&this->actor, (this->disappearTimer + 10) * 0.008f * 0.1f);
        } else {
            Actor_Kill(&this->actor);
            return true;
        }
    }
    return false;
}

void EnElf_Action_0(EnElf* this, PlayState* play) {
    Actor* playerActor = &GET_PLAYER(play)->actor;
    s32 pad;
    Player* player = GET_PLAYER(play);
    f32 heightDiff;

    SkelAnime_Update(&this->skelAnime);

    if (Rand_ZeroOne() < 0.05f) {
        this->unk_250 = Rand_ZeroFloat(10.0f) + 10.0f;
        this->unkYawAngPitch = (s32)Rand_ZeroFloat(0x400) + 0x200;
    }

    func_8088CBAC(this, play);
    if (this->fairyFlags & FAIRY_FLAG_11) {
        this->unkPos.y = player->bodyPartsPos[PLAYER_BODYPART_WAIST].y;
    }

    EnElf_StepVerticalSpeed2(this, &this->unkPos);
    func_8088D9BC(this, play);

    if (Actor_HasParent(&this->actor, play)) {
        if (this->fairyFlags & FAIRY_FLAG_10) {
            Flags_SetCollectible(play, this->collectibleFlag);
        }
        Actor_Kill(&this->actor);
        return;
    }

    if (Player_InCsMode(play)) {
        if ((this->fairyFlags & FAIRY_FLAG_14) && (this->fairyFlags & FAIRY_FLAG_TIMED)) {
            if (EnElf_TryDisappearing(this)) {}
        }
        return;
    }

    heightDiff = this->actor.world.pos.y - playerActor->world.pos.y;

    if ((this->fairyFlags & FAIRY_FLAG_12) && (heightDiff > 0.0f) && (heightDiff < 60.0f) &&
        !EnElf_IsOutsideRangeXZ(&this->actor.world.pos, &playerActor->world.pos, 10.0f)) {
        Health_ChangeBy(play, 0x80);
        if (this->fairyFlags & FAIRY_FLAG_BIG) {
            Magic_Add(play, MAGIC_FILL_TO_CAPACITY);
        }
        gSaveContext.jinxTimer = 0;
        this->unk_254 = 50.0f;
        this->unkYaw = playerActor->shape.rot.y;
        this->unkYawAngVel = -0x1000;
        this->unkPos.y = 30.0f;
        this->unk_250 = 0.0f;
        this->unkPitch = 0;
        EnElf_SetupAction(this, EnElf_Action_2);
        if (this->fairyFlags & FAIRY_FLAG_10) {
            Flags_SetCollectible(play, this->collectibleFlag);
        }
        return;
    }

    if (this->fairyFlags & FAIRY_FLAG_TIMED) {
        if (EnElf_TryDisappearing(this)) {
            return;
        }
    } else {
        Actor_SetScale(&this->actor, 0.008f);
    }

    if (this->fairyFlags & FAIRY_FLAG_13) {
        Actor_OfferGetItem(&this->actor, play, GI_MAX, 80.0f, 60.0f);
    }
}

void EnElf_Action_4(EnElf* this, PlayState* play) {
    Vec3f parentPos;
    Actor* parent;

    SkelAnime_Update(&this->skelAnime);
    EnElf_UnkFunc_0(this, play);
    parent = this->actor.parent;

    if ((parent != NULL) && (parent->update != NULL)) {
        parentPos = this->actor.parent->world.pos;
        parentPos.y += (1500.0f * this->actor.scale.y) + 40.0f;
        func_8088D660(this, &parentPos, 0.2f);
    } else {
        Actor_Kill(&this->actor);
    }
    this->yaw = Math_Atan2S_XY(this->actor.velocity.z, this->actor.velocity.x);
}

void EnElf_Spawner_DoNothing(EnElf* this, PlayState* play) {
}

void EnElf_Action_2(EnElf* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    SkelAnime_Update(&this->skelAnime);
    Math_SmoothStepToF(&this->unk_254, 30.0f, 0.1f, 4.0f, 1.0f);

    this->unkPos.x = Math_CosS(this->unkYaw) * this->unk_254;
    this->unkPos.y += this->unk_250;

    switch (this->unkPitch) {
        case 0:
            if (this->unk_250 < 2.0f) {
                this->unk_250 += 0.1f;
            } else {
                gSaveContext.healthAccumulator = 0xA0;
                this->unkPitch++;
            }
            break;

        case 1:
            if (this->unk_250 > -1.0f) {
                this->unk_250 -= 0.2f;
            }
            break;

        default:
            break;
    }

    this->unkPos.z = Math_SinS(this->unkYaw) * -this->unk_254;
    this->unkYaw += this->unkYawAngVel;
    func_8088D660(this, &player->actor.world.pos, 0.2f);

    if (this->unk_250 < 0.0f) {
        if ((this->unkPos.y < 20.0f) && (this->unkPos.y > 0.0f)) {
            Actor_SetScale(&this->actor, this->unkPos.y * 0.008f * 0.05f);
        }
    }

    if (this->unkPos.y < -10.0f) {
        Actor_Kill(&this->actor);
        return;
    }

    this->yaw = Math_Atan2S_XY(this->actor.velocity.z, this->actor.velocity.x);
    EnElf_SpawnSparkles(this, play, 32);
    Actor_PlaySfx(&this->actor, NA_SE_EV_FIATY_HEAL - SFX_FLAG);
}

void EnElf_Action_6(EnElf* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    SkelAnime_Update(&this->skelAnime);
    if (this->unkPos.y > 200.0f) {
        Actor_Kill(&this->actor);
        return;
    }

    if (this->unkYawAngPitch >= 0x7E) {
        this->unk_254 += 0.1f;
        this->unk_250 += 0.5f;
        this->unkPos.y += this->unk_250;
    } else {
        this->unkYawAngPitch++;
        if (this->unk_254 < 30.0f) {
            this->unk_254 += 0.5f;
        }

        if (this->unkPos.y > 0.0f) {
            this->unkPos.y -= 0.7f;
        }
    }

    this->unkPos.x = Math_CosS(this->unkYaw) * this->unk_254;
    this->unkPos.z = Math_SinS(this->unkYaw) * -this->unk_254;
    this->unkYaw += this->unkYawAngVel;

    func_8088D7F8(this, &player->bodyPartsPos[PLAYER_BODYPART_WAIST]);
    this->yaw = Math_Atan2S_XY(this->actor.velocity.z, this->actor.velocity.x);
    EnElf_SpawnSparkles(this, play, 32);
    Actor_PlaySfx(&this->actor, NA_SE_EV_FIATY_HEAL - SFX_FLAG);
}

void EnElf_Action_3(EnElf* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    SkelAnime_Update(&this->skelAnime);

    this->unkPos.z = 0.0f;
    this->unkPos.x = 0.0f;
    this->unkPos.y += this->unk_250;
    this->unk_250 -= 0.35f;

    if (this->unk_250 <= 0.0f) {
        EnElf_SetupAction(this, EnElf_Action_6);
        this->unkYawAngVel = 0x800;
        this->unkYawAngPitch = 0;
        this->unk_250 = 0.0f;
        this->unk_254 = 1.0f;
    }

    func_8088D7F8(this, &player->bodyPartsPos[PLAYER_BODYPART_WAIST]);
    Actor_SetScale(&this->actor, (1.0f - (SQ(this->unk_250) * SQ(1.0f / 9.0f))) * 0.008f);
    this->yaw = Math_Atan2S_XY(this->actor.velocity.z, this->actor.velocity.x);
    EnElf_SpawnSparkles(this, play, 32);
    Actor_PlaySfx(&this->actor, NA_SE_EV_FIATY_HEAL - SFX_FLAG);
}

void EnElf_Tatl_UpdateMisc2(EnElf* this, PlayState* play) {
    if (this->fairyFlags & FAIRY_FLAG_TATL) {
        EnElf_Tatl_UpdateMisc2Tatl(this, play);
    }

    SkelAnime_Update(&this->skelAnime);

    this->unkFunc(this, play);
}

void func_8088E60C(EnElf* this, PlayState* play) {
    s16 glowLightRadius;
    Player* player = GET_PLAYER(play);

    if (player->stateFlags1 & PLAYER_STATE1_GETTING_ITEM) {
        glowLightRadius = 200;
    } else {
        glowLightRadius = 100;
    }

    if ((this->fairyState == FAIRY_STATE_6) || (player->stateFlags1 & PLAYER_STATE1_IS_CHANGING_PLAYER_FORM) ||
        (this->fairyFlags & FAIRY_FLAG_15)) {
        glowLightRadius = 0;
    }

    if (Cutscene_IsCueInChannel(play, CS_CMD_ACTOR_CUE_201)) {
        if (play->csCtx.actorCues[Cutscene_GetCueChannel(play, CS_CMD_ACTOR_CUE_201)]->id == 6) {
            glowLightRadius = 0;
        }
    }

    if (this->fairyFlags & FAIRY_FLAG_5) {
        Player* player = GET_PLAYER(play);

        Lights_PointNoGlowSetInfo(&this->lightInfoNoGlow, player->actor.world.pos.x, player->actor.world.pos.y + 60.0f,
                                  player->actor.world.pos.z, 255, 255, 255, 200);
    } else {
        Lights_PointNoGlowSetInfo(&this->lightInfoNoGlow, this->actor.world.pos.x, this->actor.world.pos.y,
                                  this->actor.world.pos.z, 255, 255, 255, -1);
    }

    Lights_PointGlowSetInfo(&this->lightInfoGlow, this->actor.world.pos.x, this->actor.world.pos.y,
                            this->actor.world.pos.z, 255, 255, 255, glowLightRadius);
    this->yaw = Math_Atan2S_XY(this->actor.velocity.z, this->actor.velocity.x);
    Actor_SetScale(&this->actor, this->actor.scale.x);
}

void EnElf_Tatl_Action(EnElf* this, PlayState* play) {
    Vec3f nextPos;
    Player* player = GET_PLAYER(play);
    Actor* targetFairyActor;
    f32 xScale;
    f32 distFromLinksHead;
    s32 cueChannel;

    EnElf_Tatl_UpdateMisc1(this, play);
    EnElf_Tatl_UpdateMisc2(this, play);

    xScale = 0.0f;

    if (Cutscene_IsCueInChannel(play, CS_CMD_ACTOR_CUE_201)) {
        cueChannel = Cutscene_GetCueChannel(play, CS_CMD_ACTOR_CUE_201);
        EnElf_CutsceneTranslate(&nextPos, play, cueChannel);
        this->actor.shape.rot.y = play->csCtx.actorCues[cueChannel]->rot.y;
        this->actor.shape.rot.x = play->csCtx.actorCues[cueChannel]->rot.x;
        if (play->csCtx.actorCues[cueChannel]->id == 5) {
            EnElf_SpawnSparkles(this, play, 16);
        }

        if (this->fairyState == FAIRY_STATE_8) {
            func_8088D864(this, &nextPos);
        } else {
            func_8088D660(this, &nextPos, 0.2f);
        }

        if ((play->sceneId == SCENE_CLOCKTOWER) && (gSaveContext.sceneLayer == 0) && (play->csCtx.scriptIndex == 0) &&
            ((play->csCtx.curFrame == 149) || (play->csCtx.curFrame == 381) || (play->csCtx.curFrame == 591))) {
            Actor_PlaySfx(&this->actor, NA_SE_EV_WHITE_FAIRY_DASH);
        }

        if ((play->sceneId == SCENE_SECOM) && (gSaveContext.sceneLayer == 0) && (play->csCtx.scriptIndex == 4) &&
            (play->csCtx.curFrame == 95)) {
            Actor_PlaySfx(&this->actor, NA_SE_EV_WHITE_FAIRY_DASH);
        }
    } else {
        this->actor.shape.rot.x = 0;
        distFromLinksHead = Math_Vec3f_DistXYZ(&player->bodyPartsPos[PLAYER_BODYPART_HAT], &this->actor.world.pos);

        switch (this->fairyState) {
            case FAIRY_STATE_5:
                func_8088D660(this, &player->bodyPartsPos[PLAYER_BODYPART_HAT],
                              1.0f - (this->unkYawAngPitch * 0.033333335f));
                xScale = Math_Vec3f_DistXYZ(&player->bodyPartsPos[PLAYER_BODYPART_HAT], &this->actor.world.pos);

                if (distFromLinksHead < 7.0f) {
                    this->unkTimer = 0;
                    xScale = 0;
                } else if (distFromLinksHead < 25.0f) {
                    xScale = (xScale - 5.0f) * 0.05f;
                    xScale = 1.0f - xScale;
                    xScale = (1.0f - SQ(xScale)) * 0.008f;
                } else {
                    xScale = 0.008f;
                }
                EnElf_SpawnSparkles(this, play, 16);
                break;

            case FAIRY_STATE_6:
                func_8088D660(this, &player->bodyPartsPos[PLAYER_BODYPART_HAT], 0.2f);
                this->actor.world.pos = player->bodyPartsPos[PLAYER_BODYPART_HAT];
                break;

            case FAIRY_STATE_9:
                nextPos = player->bodyPartsPos[PLAYER_BODYPART_HAT];
                nextPos.y += 1500.0f * this->actor.scale.y;

                func_8088D7F8(this, &nextPos);
                EnElf_SpawnSparkles(this, play, 16);

                if (this->unk_254 <= 19.0f) {
                    this->unk_254 += 1.0f;
                }

                if (this->unk_254 >= 21.0f) {
                    this->unk_254 -= 1.0f;
                }

                if (this->unkTimer < 0x20) {
                    this->unkYawAngVel = (this->unkTimer * 240) + 0x200;
                }
                break;

            case FAIRY_STATE_10:
                nextPos = GET_ACTIVE_CAM(play)->eye;
                nextPos.y += -2000.0f * this->actor.scale.y;
                EnElf_UpdateMovement(this, &nextPos, 0.0f, 30.0f, 0.2f);
                break;

            default:
                targetFairyActor = play->actorCtx.attention.fairyActor;
                if ((player->stateFlags1 & PLAYER_STATE1_TALKING) && (player->talkActor != NULL)) {
                    Math_Vec3f_Copy(&nextPos, &player->talkActor->focus.pos);
                } else {
                    Math_Vec3f_Copy(&nextPos, &play->actorCtx.attention.fairyPos);
                }
                nextPos.y += 1500.0f * this->actor.scale.y;

                if (targetFairyActor != NULL) {
                    EnElf_UpdateMovement(this, &nextPos, 0.0f, 30.0f, 0.2f);
                    if (this->actor.speed >= 5.0f) {
                        EnElf_SpawnSparkles(this, play, 16);
                    }
                } else {
                    if ((this->timer % 32) == 0) {
                        this->unk_23C = Rand_ZeroFloat(7.0f) + 3.0f;
                    }

                    if (this->fairyFlags & FAIRY_FLAG_1) {
                        if (distFromLinksHead < 30.0f) {
                            this->fairyFlags ^= FAIRY_FLAG_1;
                        }
                        EnElf_UpdateMovement(this, &nextPos, 0.0f, 30.0f, 0.2f);
                        EnElf_SpawnSparkles(this, play, 16);
                    } else {
                        if (distFromLinksHead > 100.0f) {
                            this->fairyFlags |= FAIRY_FLAG_1;
                            if (this->unk_269 == 0) {
                                Actor_PlaySfx(&this->actor, NA_SE_EV_BELL_DASH_NORMAL);
                            }
                            this->unkTimer = 100;
                        }
                        EnElf_UpdateMovement(this, &nextPos, 0.0f, this->unk_23C, 0.2f);
                    }
                }
                break;
        }
    }

    if (this->fairyState == FAIRY_STATE_5) {
        this->actor.scale.x = xScale;
    } else if (this->fairyState == FAIRY_STATE_6) {
        this->actor.scale.x = 0.0f;
    } else {
        Math_SmoothStepToF(&this->actor.scale.x, 0.008f, 0.3f, 0.00080000004f, 0.000080000005f);
    }

    func_8088E60C(this, play);

    if (!Cutscene_IsCueInChannel(play, CS_CMD_ACTOR_CUE_201)) {
        this->actor.shape.rot.y = this->yaw;
    }
}

void EnElf_LerpColor(Color_RGBAf* dest, Color_RGBAf* newColor, Color_RGBAf* curColor, f32 rate) {
    Color_RGBAf rgbaDiff;

    rgbaDiff.r = newColor->r - curColor->r;
    rgbaDiff.g = newColor->g - curColor->g;
    rgbaDiff.b = newColor->b - curColor->b;
    rgbaDiff.a = newColor->a - curColor->a;

    dest->r += rgbaDiff.r * rate;
    dest->g += rgbaDiff.g * rate;
    dest->b += rgbaDiff.b * rate;
    dest->a += rgbaDiff.a * rate;
}

void EnElf_Tatl_UpdateMisc2Tatl(EnElf* this, PlayState* play) {
    Actor* targetFairyActor = play->actorCtx.attention.fairyActor;
    Player* player = GET_PLAYER(play);
    f32 transitionRate;

    if (!(this->fairyCsFlags & FAIRY_CS_FLAG_3)) {
        this->fairyCsFlags &= ~FAIRY_CS_FLAG_4;
    }

    if (this->fairyCsFlags & FAIRY_CS_FLAG_3) {
        if (this->fairyCsFlags & FAIRY_CS_FLAG_4) {
            this->unk_238 = 1.0f;
            this->fairyCsFlags &= ~FAIRY_CS_FLAG_3;
            this->unk_268 = 0;
            this->fairyCsFlags &= ~FAIRY_CS_FLAG_4;
        } else {
            this->innerColor.r = 255.0f;
            this->innerColor.g = 0.0f;
            this->innerColor.b = 0.0f;
            this->innerColor.a = 255.0f;

            this->outerColor.r = 200.0f;
            this->outerColor.g = 80.0f;
            this->outerColor.b = 80.0f;
            this->outerColor.a = 0.0f;
        }
    } else if (play->actorCtx.attention.fairyMoveProgressFactor != 0.0f) {
        this->unk_268 = 0;
        this->unk_238 = 1.0f;
        if (this->unk_269 == 0) {
            Actor_PlaySfx(&this->actor, NA_SE_EV_BELL_DASH_NORMAL);
        }
    } else if (this->unk_268 == 0) {
        if ((targetFairyActor == NULL) ||
            (Math_Vec3f_DistXYZ(&this->actor.world.pos, &play->actorCtx.attention.fairyPos) < 50.0f)) {
            this->unk_268 = 1;
        }
    } else if (this->unk_238 != 0.0f) {
        if (Math_StepToF(&this->unk_238, 0.0f, 0.25f)) {
            this->innerColor = play->actorCtx.attention.fairyInnerColor;
            this->outerColor = play->actorCtx.attention.fairyOuterColor;
        } else {
            transitionRate = 0.25f / this->unk_238;
            EnElf_LerpColor(&this->innerColor, &play->actorCtx.attention.fairyInnerColor, &this->innerColor,
                            transitionRate);
            EnElf_LerpColor(&this->outerColor, &play->actorCtx.attention.fairyOuterColor, &this->outerColor,
                            transitionRate);
        }
    }

    if (this->fairyFlags & FAIRY_FLAG_0) {
        if ((targetFairyActor == NULL) || (player->focusActor == NULL)) {
            this->fairyFlags ^= FAIRY_FLAG_0;
        }
    } else if ((targetFairyActor != NULL) && (player->focusActor != NULL)) {
        u8 temp = this->unk_269;
        u16 targetSfxId = (this->unk_269 == 0) ? NA_SE_NONE : NA_SE_NONE;

        if (temp == 0) {
            Actor_PlaySfx(&this->actor, targetSfxId);
        }
        this->fairyFlags |= FAIRY_FLAG_0;
    }
}

void EnElf_Tatl_UpdateMisc1(EnElf* this, PlayState* play) {
    s32 fairyState;
    Actor* targetFairyActor;
    Player* player = GET_PLAYER(play);
    s32 pad;

    if (play->csCtx.state != CS_STATE_IDLE) {
        if (Cutscene_IsCueInChannel(play, CS_CMD_ACTOR_CUE_201)) {
            switch (play->csCtx.actorCues[Cutscene_GetCueChannel(play, CS_CMD_ACTOR_CUE_201)]->id) {
                case 4:
                    fairyState = FAIRY_STATE_7;
                    break;

                case 3:
                    fairyState = FAIRY_STATE_4;
                    break;

                case 1:
                    fairyState = FAIRY_STATE_8;
                    break;

                default:
                    fairyState = FAIRY_STATE_0;
                    break;
            }
        } else {
            fairyState = FAIRY_STATE_0;
            this->unkTimer = 100;
        }
    } else if (this->fairyCsFlags & FAIRY_CS_FLAG_3) {
        fairyState = FAIRY_STATE_1;
        Actor_PlaySfx_Flagged(&this->actor, NA_SE_EV_BELL_ANGER - SFX_FLAG);
    } else {
        targetFairyActor = play->actorCtx.attention.fairyActor;
        if (player->stateFlags1 & PLAYER_STATE1_GETTING_ITEM) {
            fairyState = FAIRY_STATE_10;
            this->unkTimer = 100;
        } else if ((targetFairyActor == NULL) || (targetFairyActor->category == ACTORCAT_NPC)) {
            if (targetFairyActor != NULL) {
                this->unkTimer = 100;
                player->stateFlags2 |= PLAYER_STATE2_TATL_ACTIVE;
                fairyState = FAIRY_STATE_0;
            } else {
                switch (this->fairyState) {
                    case FAIRY_STATE_0:
                        if (this->unkTimer != 0) {
                            this->unkTimer--;
                            fairyState = FAIRY_STATE_0;
                        } else if (!(player->stateFlags1 & PLAYER_STATE1_TALKING)) {
                            if (this->unk_269 == 0) {
                                Actor_PlaySfx(&this->actor, NA_SE_EV_NAVY_VANISH);
                            }
                            fairyState = FAIRY_STATE_5;
                        } else {
                            fairyState = FAIRY_STATE_0;
                        }
                        break;

                    case FAIRY_STATE_5:
                        if (this->unkTimer != 0) {
                            if (this->unkYawAngPitch > 0) {
                                this->unkYawAngPitch--;
                                fairyState = FAIRY_STATE_5;
                            } else {
                                player->stateFlags2 |= PLAYER_STATE2_TATL_ACTIVE;
                                fairyState = FAIRY_STATE_0;
                            }
                        } else {
                            fairyState = FAIRY_STATE_6;
                        }
                        break;

                    case FAIRY_STATE_6:
                        fairyState = FAIRY_STATE_6;
                        break;

                    case FAIRY_STATE_9:
                        fairyState = this->fairyState;
                        if (this->unkTimer > 0) {
                            this->unkTimer--;
                        } else {
                            fairyState = FAIRY_STATE_0;
                        }
                        break;

                    default:
                        fairyState = FAIRY_STATE_0;
                        break;
                }
            }
        } else {
            fairyState = FAIRY_STATE_1;
        }

        switch (fairyState) {
            case FAIRY_STATE_0:
                if (!(player->stateFlags2 & PLAYER_STATE2_TATL_ACTIVE)) {
                    fairyState = FAIRY_STATE_5;
                    if (this->unk_269 == 0) {
                        Actor_PlaySfx(&this->actor, NA_SE_EV_NAVY_VANISH);
                    }
                }
                break;

            case FAIRY_STATE_6:
                if (player->stateFlags2 & PLAYER_STATE2_TATL_ACTIVE) {
                    fairyState = FAIRY_STATE_9;
                    this->unkTimer = 42;
                    if (this->unk_269 == 0) {
                        Actor_PlaySfx(&this->actor, NA_SE_EV_BELL_DASH_NORMAL);
                    }
                } else if (player->stateFlags1 & PLAYER_STATE1_TALKING) {
                    player->stateFlags2 |= PLAYER_STATE2_TATL_ACTIVE;
                    fairyState = FAIRY_STATE_0;
                    this->unkTimer = 0;
                }
                break;

            case FAIRY_STATE_5:
                player->stateFlags2 &= ~PLAYER_STATE2_TATL_ACTIVE;
                break;

            default:
                player->stateFlags2 |= PLAYER_STATE2_TATL_ACTIVE;
                break;
        }
    }

    if (this->fairyState != fairyState) {
        EnElf_ChangeState(this, fairyState);
        if (fairyState == FAIRY_STATE_9) {
            this->unk_254 = Math_Vec3f_DistXZ(&player->bodyPartsPos[PLAYER_BODYPART_HAT], &this->actor.world.pos);
            this->unkYaw = Math_Vec3f_Yaw(&this->actor.world.pos, &player->bodyPartsPos[PLAYER_BODYPART_HAT]);
        }
    }
}

void EnElf_SpawnSparkles(EnElf* this, PlayState* play, s32 sparkleLife) {
    static Vec3f sSparkleVelocity = { 0.0f, -0.05f, 0.0f };
    static Vec3f sSparkleAccel = { 0.0f, -0.025f, 0.0f };
    s32 pad;
    Vec3f sparklePos;
    Color_RGBA8 primColor;
    Color_RGBA8 envColor;

    if (this->fairyFlags & FAIRY_FLAG_NO_DRAW) {
        return;
    }

    sparklePos.x = Rand_CenteredFloat(6.0f) + this->actor.world.pos.x;
    sparklePos.y = (Rand_ZeroOne() * 6.0f) + this->actor.world.pos.y;
    sparklePos.z = Rand_CenteredFloat(6.0f) + this->actor.world.pos.z;

    primColor.r = this->innerColor.r;
    primColor.g = this->innerColor.g;
    primColor.b = this->innerColor.b;

    envColor.r = this->outerColor.r;
    envColor.g = this->outerColor.g;
    envColor.b = this->outerColor.b;

    EffectSsKirakira_SpawnDispersed(play, &sparklePos, &sSparkleVelocity, &sSparkleAccel, &primColor, &envColor, 1000,
                                    sparkleLife);
}

void func_8088F9E4(Actor* thisx, PlayState* play) {
    EnElf* this = THIS;
    s32 bgId;

    thisx->floorHeight =
        BgCheck_EntityRaycastFloor5_2(play, &play->colCtx, &thisx->floorPoly, &bgId, &this->actor, &thisx->world.pos);
    thisx->shape.shadowAlpha = 50;
}

void EnElf_Tatl_UpdateMisc3(EnElf* this, PlayState* play) {
    Vec3f refPos;
    Player* player = GET_PLAYER(play);

    if (this->fairyFlags & FAIRY_FLAG_MOVE_TO_PLAYER) {
        refPos = play->actorCtx.attention.fairyPos;

        if (this->unk_234 != NULL) {
            refPos = this->unk_234->world.pos;
        } else if ((player->focusActor == NULL) || (&player->actor == player->focusActor) ||
                   (&this->actor == player->focusActor) || (this->fairyCsFlags & FAIRY_CS_FLAG_2)) {
            refPos.x = player->bodyPartsPos[PLAYER_BODYPART_HEAD].x + (Math_SinS(player->actor.shape.rot.y) * 20.0f);
            refPos.y = player->bodyPartsPos[PLAYER_BODYPART_HEAD].y + 5.0f;
            refPos.z = player->bodyPartsPos[PLAYER_BODYPART_HEAD].z + (Math_CosS(player->actor.shape.rot.y) * 20.0f);
            this->fairyCsFlags &= ~FAIRY_CS_FLAG_2;
        }
        this->actor.focus.pos = refPos;
        this->fairyFlags &= ~FAIRY_FLAG_MOVE_TO_PLAYER;
    }

    EnElf_Tatl_UpdateMisc2(this, play);
    refPos = this->actor.focus.pos;
    EnElf_UpdateMovement(this, &refPos, 0, 30.0f, 0.2f);

    if (this->actor.speed >= 5.0f) {
        EnElf_SpawnSparkles(this, play, 16);
    }

    Math_SmoothStepToF(&this->actor.scale.x, 0.008f, 0.3f, 0.00080000004f, 0.000080000005f);
    func_8088E60C(this, play);
}

void func_8088FC34(EnElf* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    Math_SmoothStepToS(&this->actor.shape.rot.y, this->yaw, 5, 0x1000, 0x400);
    this->timer++;

    if (this->unk_234 == NULL) {
        if (this->fairyCsFlags & FAIRY_CS_FLAG_5) {
            this->unk_240 = 0.0f;
        } else {
            Math_StepToF(&this->unk_240, 1.0f, 0.05f);
        }
        Environment_AdjustLights(play, SQ(this->unk_240), player->actor.projectedPos.z + 780.0f, 0.2f, 0.5f);
    }
}

void func_8088FD04(EnElf* this) {
    if (CutsceneManager_GetCurrentCsId() == this->actor.csId) {
        this->fairyCsFlags &= ~FAIRY_CS_FLAG_0;
        this->fairyCsFlags |= FAIRY_CS_FLAG_1;
    } else if (CutsceneManager_GetCurrentCsId() == CS_ID_GLOBAL_TALK) {
        CutsceneManager_Stop(CS_ID_GLOBAL_TALK);
        CutsceneManager_Queue(this->actor.csId);
        this->fairyCsFlags |= FAIRY_CS_FLAG_0;
    } else if (CutsceneManager_IsNext(this->actor.csId)) {
        CutsceneManager_Start(this->actor.csId, &this->actor);
        this->fairyCsFlags &= ~FAIRY_CS_FLAG_0;
        this->fairyCsFlags |= FAIRY_CS_FLAG_1;
    } else {
        CutsceneManager_Queue(this->actor.csId);
        this->fairyCsFlags |= FAIRY_CS_FLAG_0;
    }
}

void func_8088FDCC(EnElf* this) {
    this->actor.update = EnElf_Tatl_Update;
    EnElf_ChangeState(this, FAIRY_STATE_0);
    this->fairyFlags &= ~FAIRY_FLAG_5;
    this->actor.focus.pos = this->actor.world.pos;
    this->unk_234 = NULL;
    if ((this->fairyCsFlags & FAIRY_CS_FLAG_1) && (this->actor.csId != CS_ID_GLOBAL_TALK)) {
        CutsceneManager_Stop(this->actor.csId);
    }
    this->fairyCsFlags &= ~FAIRY_CS_FLAG_5;
}

/** Update Funtions **/

void EnElf_Tatl_UpdateTalk(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EnElf* this = THIS;

    EnElf_Tatl_UpdateMisc3(this, play);

    switch (Message_GetState(&play->msgCtx)) {
        case TEXT_STATE_CHOICE:
            if (Message_ShouldAdvance(play)) {
                if (play->msgCtx.currentTextId == 0x202) {
                    switch (play->msgCtx.choiceIndex) {
                        case 0:
                            Audio_PlaySfx_MessageCancel();
                            break;

                        case 1:
                            Audio_PlaySfx_MessageDecide();
                            break;

                        default:
                            break;
                    }
                }

                switch (play->msgCtx.choiceIndex) {
                    case 0:
                        Message_ContinueTextbox(play, play->msgCtx.currentTextId - 1);
                        break;

                    case 1:
                        Message_ContinueTextbox(play, play->msgCtx.currentTextId + 1);
                        break;

                    default:
                        break;
                }
            }
            break;

        case TEXT_STATE_EVENT:
            if (Message_ShouldAdvance(play)) {
                switch (play->msgCtx.currentTextId) {
                    case 0x240:
                        Message_ContinueTextbox(play, 0x245);
                        break;

                    case 0x21D:
                    case 0x21E:
                    case 0x21F:
                    case 0x220:
                    case 0x221:
                    case 0x222:
                    case 0x223:
                    case 0x23F:
                    case 0x241:
                    case 0x242:
                    case 0x243:
                    case 0x244:
                        switch (CURRENT_DAY) {
                            case 1:
                                Message_ContinueTextbox(play, 0x246);
                                break;

                            case 2:
                                Message_ContinueTextbox(play, 0x247);
                                break;

                            case 3:
                                if (!gSaveContext.save.isNight) {
                                    Message_ContinueTextbox(play, 0x248);
                                } else if ((CURRENT_TIME < CLOCK_TIME(6, 0)) &&
                                           CHECK_WEEKEVENTREG(WEEKEVENTREG_74_20)) {
                                    Message_ContinueTextbox(play, 0x225);
                                } else {
                                    Message_ContinueTextbox(play, 0x249);
                                }
                                break;
                        }
                        break;

                    default:
                        Message_CloseTextbox(play);
                        func_8088FDCC(this);
                        break;
                }
            }
            break;

        case TEXT_STATE_CLOSING:
            func_8088FDCC(this);
            break;
    }

    func_8088FC34(this, play);

    if (this->fairyCsFlags & FAIRY_CS_FLAG_0) {
        func_8088FD04(this);
    }
}

void EnElf_Tatl_Update(Actor* thisx, PlayState* play) {
    s32 pad;
    EnElf* this = THIS;
    Player* player = GET_PLAYER(play);
    u16 hintTextId = QuestHint_GetTatlTextId(play);

    if (hintTextId != this->hintTextId) {
        this->hintTextId = hintTextId;
        gSaveContext.save.saveInfo.playerData.tatlTimer = 0;
    }

    if ((player->tatlTextId == 0) && (player->focusActor == NULL)) {
        if ((gSaveContext.save.saveInfo.playerData.tatlTimer >= 600) &&
            (gSaveContext.save.saveInfo.playerData.tatlTimer <= 3000)) {
            player->tatlTextId = QuestHint_GetTatlTextId(play);
        }
    }

    if (player->tatlTextId < 0) {
        thisx->flags |= ACTOR_FLAG_TALK_OFFER_AUTO_ACCEPTED;
    }

    if (Actor_TalkOfferAccepted(thisx, &play->state)) {
        Audio_PlaySfx_AtPosWithReverb(&gSfxDefaultPos, NA_SE_VO_NA_LISTEN, 0x20);
        thisx->focus.pos = thisx->world.pos;

        if (thisx->textId == QuestHint_GetTatlTextId(play)) {
            this->fairyFlags |= FAIRY_FLAG_7;
            gSaveContext.save.saveInfo.playerData.tatlTimer = 3000 + 1;
        }

        this->fairyFlags |= FAIRY_FLAG_MOVE_TO_PLAYER;
        this->fairyFlags |= FAIRY_FLAG_5;
        thisx->update = EnElf_Tatl_UpdateTalk;
        EnElf_ChangeState(this, FAIRY_STATE_3);
        if (this->tatlHintActor != NULL) {
            this->tatlHintActor->flags |= ACTOR_FLAG_TALK;
            thisx->csId = this->tatlHintActor->csId;
            if (thisx->csId != CS_ID_NONE) {
                func_8088FD04(this);
            }
            if (this->tatlHintActor->home.rot.x == -0x961) {
                this->unk_234 = this->tatlHintActor;
                Actor_ChangeFocus(thisx, play, this->tatlHintActor);
            }
        } else {
            thisx->csId = CS_ID_NONE;
        }
        thisx->flags &= ~ACTOR_FLAG_TALK_OFFER_AUTO_ACCEPTED;
    } else if (this->fairyCsFlags & FAIRY_CS_FLAG_2) {
        thisx->focus.pos = thisx->world.pos;
        this->fairyFlags |= FAIRY_FLAG_MOVE_TO_PLAYER;
        this->fairyFlags |= FAIRY_FLAG_5;
        thisx->update = EnElf_Tatl_UpdateTalk;
        EnElf_ChangeState(this, FAIRY_STATE_3);
    } else {
        this->actionFunc(this, play);

        if (!Play_InCsMode(play)) {
            if (gSaveContext.save.saveInfo.playerData.tatlTimer < 25800) {
                gSaveContext.save.saveInfo.playerData.tatlTimer++;
            } else if (!(this->fairyFlags & FAIRY_FLAG_7)) {
                gSaveContext.save.saveInfo.playerData.tatlTimer = 0;
            }
        }
    }

    this->tatlHintActor = NULL;
    this->timer++;

    if ((this->unk_240 >= 0.0f) &&
        Environment_AdjustLights(play, CB(this->unk_240), player->actor.projectedPos.z + 780.0f, 0.2f, 0.5f)) {
        Math_StepToF(&this->unk_240, -0.05f, 0.05f);
    }

    if (this->unk_269 > 0) {
        this->unk_269--;
    }

    if (((u32)this->unk_269 == 0) && (play->csCtx.state != CS_STATE_IDLE)) {
        this->unk_269 = 1;
    }
}

void EnElf_Update(Actor* thisx, PlayState* play) {
    EnElf* this = THIS;

    this->actionFunc(this, play);

    thisx->shape.rot.y = this->yaw;
    this->timer++;

    if (this->fairyFlags & FAIRY_FLAG_BIG) {
        func_8088F9E4(thisx, play);
    }
}

s32 EnElf_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx,
                           Gfx** gfx) {
    static Vec3f sZeroVec = { 0.0f, 0.0f, 0.0f };
    s32 pad;
    EnElf* this = THIS;
    Vec3f limb6Pos;
    f32 scale;

    if (limbIndex == FAIRY_LIMB_6) {
        scale = ((Math_SinS(this->timer * 4096) * 0.1f) + 1.0f) * 0.012f;
        if (this->fairyFlags & FAIRY_FLAG_BIG) {
            scale *= 2.0f;
        }
        scale *= this->actor.scale.x * (1.0f / 0.008f);

        Matrix_MultVec3f(&sZeroVec, &limb6Pos);
        Matrix_Translate(limb6Pos.x, limb6Pos.y, limb6Pos.z, MTXMODE_NEW);
        Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    }

    //! @note: `limbIndex` extends past `FAIRY_LIMB_MAX`.
    //! This code was copied from OoT despite the number of limbs decreasing.
    //! In OoT, this was intented to not draw wings for big fairies
    if ((this->fairyFlags & FAIRY_FLAG_BIG) &&
        ((limbIndex == FAIRY_LIMB_4) || (limbIndex == 7) || (limbIndex == 11) || (limbIndex == 14))) {
        *dList = NULL;
    }
    return false;
}

void EnElf_Draw(Actor* thisx, PlayState* play) {
    EnElf* this = THIS;
    Player* player = GET_PLAYER(play);
    s32 pad;
    s32 pad2;
    Gfx* gfx;
    f32 alphaScale;
    s32 envAlpha;

    if (player->currentMask == PLAYER_MASK_GIANT) {
        return;
    }

    if (this->fairyFlags & FAIRY_FLAG_NO_DRAW) {
        return;
    }

    if (Cutscene_IsCueInChannel(play, CS_CMD_ACTOR_CUE_201)) {
        if (play->csCtx.actorCues[Cutscene_GetCueChannel(play, CS_CMD_ACTOR_CUE_201)]->id == 6) {
            return;
        }
    }

    if (player->stateFlags1 & PLAYER_STATE1_IN_FIRST_PERSON_MODE) {
        if (!(kREG(90) < this->actor.projectedPos.z)) {
            return;
        }
    }

    gfx = GRAPH_ALLOC(play->state.gfxCtx, 4 * sizeof(Gfx));
    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL27_Xlu(play->state.gfxCtx);

    envAlpha = (this->timer * 50) & 0x1FF;
    envAlpha = (envAlpha >= 0x100) ? 511 - envAlpha : envAlpha;

    alphaScale = (this->disappearTimer < 0) ? (this->disappearTimer * 0.0011666666f) + 1.0f : 1.0f;

    gSPSegment(POLY_XLU_DISP++, 0x08, gfx);

    gDPPipeSync(gfx++);
    gDPSetPrimColor(gfx++, 0, 0x01, (u8)(s8)this->innerColor.r, (u8)(s8)this->innerColor.g, (u8)(s8)this->innerColor.b,
                    (u8)(s8)(this->innerColor.a * alphaScale));

    if (this->fairyFlags & FAIRY_FLAG_TATL) {
        gDPSetRenderMode(gfx++, G_RM_PASS, G_RM_CLD_SURF2);
    } else {
        gDPSetRenderMode(gfx++, G_RM_PASS, G_RM_ZB_CLD_SURF2);
    }

    gSPEndDisplayList(gfx);

    gDPSetEnvColor(POLY_XLU_DISP++, (u8)(s8)this->outerColor.r, (u8)(s8)this->outerColor.g, (u8)(s8)this->outerColor.b,
                   (u8)(s8)(envAlpha * alphaScale));

    POLY_XLU_DISP = SkelAnime_Draw(play, this->skelAnime.skeleton, this->skelAnime.jointTable, EnElf_OverrideLimbDraw,
                                   NULL, &this->actor, POLY_XLU_DISP);

    CLOSE_DISPS(play->state.gfxCtx);
}

/**
 * Interpolates the actor's position based on the corresponding actor action's position
 * and the current cutscene frame
 */
void EnElf_CutsceneTranslate(Vec3f* dest, PlayState* play, s32 cueChannel) {
    Vec3f startPos;
    Vec3f endPos;
    CsCmdActorCue* cue = play->csCtx.actorCues[cueChannel];
    f32 lerp;

    startPos.x = cue->startPos.x;
    startPos.y = cue->startPos.y;
    startPos.z = cue->startPos.z;

    endPos.x = cue->endPos.x;
    endPos.y = cue->endPos.y;
    endPos.z = cue->endPos.z;

    lerp = Environment_LerpWeight(cue->endFrame, cue->startFrame, play->csCtx.curFrame);

    VEC3F_LERPIMPDST(dest, &startPos, &endPos, lerp);
}
