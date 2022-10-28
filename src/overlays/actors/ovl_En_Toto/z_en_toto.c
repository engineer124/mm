/*
 * File: z_en_toto.c
 * Overlay: ovl_En_Toto
 * Description: Toto
 */

#include "z_en_toto.h"
#include "objects/object_zm/object_zm.h"

#define FLAGS (ACTOR_FLAG_1 | ACTOR_FLAG_8)

#define THIS ((EnToto*)thisx)

#define ENTOTO_WEEK_EVENT_FLAGS (gSaveContext.save.weekEventReg[50] & 1 || gSaveContext.save.weekEventReg[51] & 0x80)

void EnToto_Init(Actor* thisx, PlayState* play);
void EnToto_Destroy(Actor* thisx, PlayState* play);
void EnToto_Update(Actor* thisx, PlayState* play);
void EnToto_Draw(Actor* thisx, PlayState* play);

void func_80BA3930(EnToto* this, PlayState* play);
void func_80BA39C8(EnToto* this, PlayState* play);
void func_80BA3DBC(EnToto* this, PlayState* play);
void func_80BA3BFC(EnToto* this, PlayState* play);
void func_80BA3CC4(EnToto* this, PlayState* play);
void func_80BA3D38(EnToto* this, PlayState* play);
s32 EnToto_SubCs_True(EnToto* this, PlayState* play);
s32 EnToto_SubCs_False(EnToto* this, PlayState* play);
s32 EnToto_Cs_3_4(EnToto* this, PlayState* play);
s32 EnToto_SubCs_0_2_3_4_16(EnToto* this, PlayState* play);
s32 EnToto_SubCs_9_14(EnToto* this, PlayState* play);
s32 EnToto_Cs_14(EnToto* this, PlayState* play);
s32 EnToto_SubCs_10_15(EnToto* this, PlayState* play);
s32 EnToto_Cs_5_10_12_15(EnToto* this, PlayState* play);
s32 EnToto_Cs_0(EnToto* this, PlayState* play);
s32 EnToto_Cs_1(EnToto* this, PlayState* play);
s32 EnToto_Cs_2(EnToto* this, PlayState* play);
s32 EnToto_Cs_9(EnToto* this, PlayState* play);
s32 EnToto_SubCs_6(EnToto* this, PlayState* play);
s32 EnToto_Cs_6(EnToto* this, PlayState* play);
s32 EnToto_Cs_7(EnToto* this, PlayState* play);
s32 EnToto_SubCs_8(EnToto* this, PlayState* play);
s32 EnToto_Cs_8(EnToto* this, PlayState* play);
s32 EnToto_SubCs_StartOcarina(EnToto* this, PlayState* play);
s32 EnToto_Cs_EndOcarina(EnToto* this, PlayState* play);
s32 EnToto_SubCs_12(EnToto* this, PlayState* play);
s32 EnToto_SubCs_13(EnToto* this, PlayState* play);
s32 EnToto_Cs_13(EnToto* this, PlayState* play);
s32 EnToto_Cs_16(EnToto* this, PlayState* play);
s32 func_80BA4C0C(EnToto* this, PlayState* play);
s32 func_80BA4C44(EnToto* this, PlayState* play);

typedef enum {
    /* 0x00 */ TOTO_SUBCSMODE_0,
    /* 0x01 */ TOTO_SUBCSMODE_1,
    /* 0x02 */ TOTO_SUBCSMODE_2,
    /* 0x03 */ TOTO_SUBCSMODE_3,
    /* 0x04 */ TOTO_SUBCSMODE_4,
    /* 0x05 */ TOTO_SUBCSMODE_5,
    /* 0x06 */ TOTO_SUBCSMODE_6,
    /* 0x07 */ TOTO_SUBCSMODE_7,
    /* 0x08 */ TOTO_SUBCSMODE_8,
    /* 0x09 */ TOTO_SUBCSMODE_9,
    /* 0x0A */ TOTO_SUBCSMODE_10,
    /* 0x0B */ TOTO_SUBCSMODE_11,
    /* 0x0C */ TOTO_SUBCSMODE_12,
    /* 0x0D */ TOTO_SUBCSMODE_13,
    /* 0x0E */ TOTO_SUBCSMODE_14,
    /* 0x0F */ TOTO_SUBCSMODE_15,
    /* 0x10 */ TOTO_SUBCSMODE_16,
    /* 0x11 */ TOTO_SUBCSMODE_17
} TotoSubCsMode;

typedef enum {
    /* 0x00 */ TOTO_CSMODE_0,
    /* 0x01 */ TOTO_CSMODE_1,
    /* 0x02 */ TOTO_CSMODE_2,
    /* 0x03 */ TOTO_CSMODE_3,
    /* 0x04 */ TOTO_CSMODE_4,
    /* 0x05 */ TOTO_CSMODE_5,
    /* 0x06 */ TOTO_CSMODE_6,
    /* 0x07 */ TOTO_CSMODE_7,
    /* 0x08 */ TOTO_CSMODE_8,
    /* 0x09 */ TOTO_CSMODE_9,
    /* 0x0A */ TOTO_CSMODE_10,
    /* 0x0B */ TOTO_CSMODE_11,
    /* 0x0C */ TOTO_CSMODE_12,
    /* 0x0D */ TOTO_CSMODE_13,
    /* 0x0E */ TOTO_CSMODE_14,
    /* 0x0F */ TOTO_CSMODE_15,
    /* 0x10 */ TOTO_CSMODE_16
} TotoCsMode;

ActorInit En_Toto_InitVars = {
    ACTOR_EN_TOTO,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_ZM,
    sizeof(EnToto),
    (ActorFunc)EnToto_Init,
    (ActorFunc)EnToto_Destroy,
    (ActorFunc)EnToto_Update,
    (ActorFunc)EnToto_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_METAL,
        AT_NONE,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK2,
        { 0x00100000, 0x00, 0x00 },
        { 0x01000202, 0x00, 0x00 },
        TOUCH_NONE | TOUCH_SFX_NORMAL,
        BUMP_ON | BUMP_HOOKABLE,
        OCELEM_ON,
    },
    { 20, 60, 0, { 0, 0, 0 } },
};

static EnTotoActionFunc D_80BA501C[] = {
    func_80BA3930,
    func_80BA3BFC,
    func_80BA3D38,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_U8(targetMode, 1, ICHAIN_STOP),
};

static EnTotoText D_80BA502C[] = {
    { 0, 0, 0x2B21 },
    { 3, 2, 0 },
};

static EnTotoText D_80BA5034[] = {
    { 0, 0, 0x2B23 },
};

static EnTotoText D_80BA5038[] = {
    { 2, 1, 0x2B24 },
    { 4, 0, 0x2B25 },
    { 3, 2, 0 },
};

static EnTotoText D_80BA5044[] = {
    { 4, 0, 0x2B25 },
};

static EnTotoText D_80BA5048[] = {
    { 16, 0, 0x2A94 }, { 0, 0, 0x2A95 }, { 4, 0, 0x2A96 }, { 4, 0, 0x2A97 },
    { 0, 0, 0x2A98 },  { 0, 0, 0x2A99 }, { 4, 0, 0x2A9A }, { 4, 0, 0x2A9B },
};

static EnTotoText D_80BA5068[] = {
    { 0, 0, 0x2AE1 },
    { 0, 0, 0x2AE2 },
    { 4, 0, 0x2AE3 },
};

static EnTotoText D_80BA5074[] = {
    { 4, 0, 0x2AE4 },
};

static AnimationHeader* D_80BA5078[] = {
    &object_zm_Anim_0028B8,
    &object_zm_Anim_00B894,
    &object_zm_Anim_002F20,
    &object_zm_Anim_00BC08,
};

static EnTotoText D_80BA5088[] = {
    { 5, 0, 0 },  { 6, 20, 0 }, { 7, 0, 0 },  { 8, 9, 0 },  { 9, 10, 0 }, { 1, 0, 0 },  { 10, 0, 0 },
    { 11, 0, 0 }, { 12, 0, 0 }, { 13, 0, 0 }, { 15, 0, 0 }, { 17, 1, 0 }, { 17, 0, 0 },
};

static EnTotoText D_80BA50BC[] = {
    { 5, 0, 0 }, { 6, 20, 0 }, { 8, 5, 0 }, { 12, 0, 0 }, { 13, 0, 0 }, { 14, 20, 0x2B22 }, { 1, 0, 0 }, { 17, 0, 0 },
};

static EnTotoUnkStruct2 D_80BA50DC[] = {
    { 0x2B2F, 0x2B30, 0x2B31, { 0xFF96, 22, 0xFE16 } }, // PLAYER_FORM_GORON
    { 0x2B26, 0x2B27, 0x2B28, { 0x0072, 22, 0xFE3C } }, // PLAYER_FORM_ZORA
    { 0x2B29, 0x2B2A, 0x2B2B, { 0xFF67, 22, 0xFE6E } }, // PLAYER_FORM_DEKU
    { 0x2B2C, 0x2B2D, 0x2B2E, { 0xFFF1, 22, 0xFE74 } }, // PLAYER_FORM_HUMAN
};

static Vec3s D_80BA510C[] = {
    { 0xFF46, 0xFFF8, 0xFF40 },
    { 0xFF21, 0xFFFD, 0xFF04 },
    { 0xFF64, 0x0016, 0xFE7E },
};

static u16 sOcarinaActionWIndFishPrompts[] = {
    OCARINA_ACTION_PROMPT_WIND_FISH_HUMAN,
    OCARINA_ACTION_PROMPT_WIND_FISH_GORON,
    OCARINA_ACTION_PROMPT_WIND_FISH_ZORA,
    OCARINA_ACTION_PROMPT_WIND_FISH_DEKU,
};

static u8 D_80BA5128[] = { 8, 4, 2, 1 };

static EnTotoUnkFunc D_80BA512C[] = {
    EnToto_SubCs_0_2_3_4_16,   // TOTO_SUBCSMODE_0
    EnToto_SubCs_False,        // TOTO_SUBCSMODE_1
    EnToto_SubCs_0_2_3_4_16,   // TOTO_SUBCSMODE_2
    EnToto_SubCs_0_2_3_4_16,   // TOTO_SUBCSMODE_3
    EnToto_SubCs_0_2_3_4_16,   // TOTO_SUBCSMODE_4
    EnToto_SubCs_False,        // TOTO_SUBCSMODE_5
    EnToto_SubCs_6,            // TOTO_SUBCSMODE_6
    EnToto_SubCs_False,        // TOTO_SUBCSMODE_7
    EnToto_SubCs_8,            // TOTO_SUBCSMODE_8
    EnToto_SubCs_9_14,         // TOTO_SUBCSMODE_9
    EnToto_SubCs_10_15,        // TOTO_SUBCSMODE_10
    EnToto_SubCs_StartOcarina, // TOTO_SUBCSMODE_11
    EnToto_SubCs_12,           // TOTO_SUBCSMODE_12
    EnToto_SubCs_13,           // TOTO_SUBCSMODE_13
    EnToto_SubCs_9_14,         // TOTO_SUBCSMODE_14
    EnToto_SubCs_10_15,        // TOTO_SUBCSMODE_15
    EnToto_SubCs_0_2_3_4_16,   // TOTO_SUBCSMODE_16
    EnToto_SubCs_True,         // TOTO_SUBCSMODE_17
};

static EnTotoUnkFunc D_80BA5174[] = {
    EnToto_Cs_0,          // TOTO_CSMODE_0
    EnToto_Cs_1,          // TOTO_CSMODE_1
    EnToto_Cs_2,          // TOTO_CSMODE_2
    EnToto_Cs_3_4,        // TOTO_CSMODE_3
    EnToto_Cs_3_4,        // TOTO_CSMODE_4
    EnToto_Cs_5_10_12_15, // TOTO_CSMODE_5
    EnToto_Cs_6,          // TOTO_CSMODE_6
    EnToto_Cs_7,          // TOTO_CSMODE_7
    EnToto_Cs_8,          // TOTO_CSMODE_8
    EnToto_Cs_9,          // TOTO_CSMODE_9
    EnToto_Cs_5_10_12_15, // TOTO_CSMODE_10
    EnToto_Cs_EndOcarina, // TOTO_CSMODE_11
    EnToto_Cs_5_10_12_15, // TOTO_CSMODE_12
    EnToto_Cs_13,         // TOTO_CSMODE_13
    EnToto_Cs_14,         // TOTO_CSMODE_14
    EnToto_Cs_5_10_12_15, // TOTO_CSMODE_15
    EnToto_Cs_16,         // TOTO_CSMODE_16
};

static EnTotoActionFunc D_80BA51B8[] = {
    func_80BA39C8,
    func_80BA3CC4,
    func_80BA3DBC,
};

void func_80BA36C0(EnToto* this, PlayState* play, s32 index) {
    this->unk2B7 = 0;
    this->actionFuncIndex = index;
    D_80BA501C[this->actionFuncIndex](this, play);
}

void EnToto_Init(Actor* thisx, PlayState* play) {
    EnToto* this = THIS;

    Actor_ProcessInitChain(&this->actor, sInitChain);
    Collider_InitAndSetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    if ((play->sceneId == SCENE_MILK_BAR) && (gSaveContext.save.time >= CLOCK_TIME(6, 0)) &&
        (gSaveContext.save.time < CLOCK_TIME(21, 30))) {
        Actor_Kill(&this->actor);
        return;
    }
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    this->actor.bgCheckFlags |= 0x400;
    SkelAnime_InitFlex(play, &this->skelAnime, &object_zm_Skel_00A978,
                       ((play->sceneId == SCENE_SONCHONOIE) ? &object_zm_Anim_003AA8 : &object_zm_Anim_00C880),
                       this->jointTable, this->morphTable, 18);
    func_80BA36C0(this, play, 0);
    this->actor.shape.rot.x = 0;
}

void EnToto_Destroy(Actor* thisx, PlayState* play) {
    EnToto* this = THIS;

    Collider_DestroyCylinder(play, &this->collider);
}

void func_80BA383C(EnToto* this, PlayState* play) {
    if (SkelAnime_Update(&this->skelAnime) && this->actionFuncIndex == 1 &&
        this->skelAnime.animation != &object_zm_Anim_000C80) {
        if (play->msgCtx.currentTextId != 0x2A98 && play->msgCtx.currentTextId != 0x2A99) {
            if (this->unk2B4 & 1 || Rand_ZeroOne() > 0.5f) {
                this->unk2B4 = (this->unk2B4 + 1) & 3;
            }
        }
        Animation_PlayOnce(&this->skelAnime, D_80BA5078[this->unk2B4]);
    }
    func_800BBB74(this->unk260, 0x14, 0x50, 3);
}

void func_80BA3930(EnToto* this, PlayState* play) {
    AnimationHeader* animationHeader = &object_zm_Anim_00C880;

    if (play->sceneId == SCENE_SONCHONOIE) {
        animationHeader = &object_zm_Anim_003AA8;
    }
    Animation_MorphToLoop(&this->skelAnime, animationHeader, -4.0f);
}

s32 func_80BA397C(EnToto* this, s16 arg1) {
    s16 diff;

    diff = this->actor.yawTowardsPlayer - this->actor.home.rot.y;
    if (ABS_ALT(diff) < arg1) {
        return 1;
    }
    return 0;
}

void func_80BA39C8(EnToto* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    func_80BA383C(this, play);
    if (Actor_ProcessTalkRequest(&this->actor, &play->state)) {
        func_80BA36C0(this, play, 1);
        if (play->sceneId != SCENE_SONCHONOIE) {
            Flags_SetSwitch(play, this->actor.params & 0x7F);
        } else if (player->transformation == PLAYER_FORM_DEKU) {
            Flags_SetSwitch(play, this->actor.home.rot.x);
        }
        this->unk2B6 = 0;
        return;
    }

    //! @TODO: 0xED02 nor 0xED01 match CLOCK_TIME macro
    if (((play->sceneId == SCENE_MILK_BAR) &&
         !((gSaveContext.save.time >= CLOCK_TIME(6, 0)) && (gSaveContext.save.time < 0xED02))) ||
        ((play->sceneId != SCENE_MILK_BAR) && func_80BA397C(this, 0x2000))) {
        if (this->unk2B6 != 0) {
            this->text = D_80BA5044;
            this->actor.flags |= ACTOR_FLAG_10000;
            func_800B8500(&this->actor, play, 9999.9f, 9999.9f, PLAYER_AP_NONE);
        } else {
            this->actor.flags &= ~ACTOR_FLAG_10000;
            func_800B8614(&this->actor, play, 50.0f);
            if (play->sceneId == SCENE_SONCHONOIE) {
                if (player->transformation == PLAYER_FORM_DEKU) {
                    if (!Flags_GetSwitch(play, this->actor.home.rot.x)) {
                        this->text = D_80BA5068;
                    } else {
                        this->text = D_80BA5074;
                    }
                } else {
                    this->text = D_80BA5048;
                }
            } else if (ENTOTO_WEEK_EVENT_FLAGS) {
                this->text = D_80BA502C;
            } else if (!Flags_GetSwitch(play, this->actor.params & 0x7F)) {
                this->text = D_80BA5034;
            } else {
                this->text = D_80BA5038;
            }
        }

        this->actor.textId = this->text->textId;
    }
}

void func_80BA3BFC(EnToto* this, PlayState* play) {
    if (play->sceneId == SCENE_SONCHONOIE) {
        Animation_MorphToPlayOnce(&this->skelAnime, &object_zm_Anim_000C80, -4.0f);
        this->unk2B4 = 0;
    } else {
        if (this->text->unk0 == 4) {
            func_80151BB4(play, 9);
        }
        Animation_MorphToLoop(&this->skelAnime, &object_zm_Anim_00B3E0, -4.0f);
    }
}

void func_80BA3C88(EnToto* this) {
    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 4, 4000, 800);
}

void func_80BA3CC4(EnToto* this, PlayState* play) {
    func_80BA383C(this, play);
    func_80BA3C88(this);
    if (Actor_TextboxIsClosing(&this->actor, play)) {
        func_80BA36C0(this, play, this->text->unk1);
    } else {
        func_80BA4C44(this, play);
    }
}

void func_80BA3D38(EnToto* this, PlayState* play) {
    this->cutscene = this->actor.cutscene;
    this->text = ENTOTO_WEEK_EVENT_FLAGS ? D_80BA50BC : D_80BA5088;
    func_80BA4C0C(this, play);
    play->actorCtx.flags |= ACTORCTX_FLAG_5;
    this->unk260[0] = 0;
}

void func_80BA3DBC(EnToto* this, PlayState* play) {
    Player* player;

    func_80BA383C(this, play);
    if (this->unk2B7 == 0) {
        if (!func_80BA4C44(this, play)) {
            return;
        }
        if (this->text->unk1 != 0 && ENTOTO_WEEK_EVENT_FLAGS) {
            this->unk2B7 = 1;
            return;
        }
    } else {
        player = GET_PLAYER(play);
        if ((player->stateFlags1 & PLAYER_STATE1_400) && player->unk_AE7 != 0) {
            func_80151BB4(play, 48);
            func_80151BB4(play, 9);
            func_80151BB4(play, 10);
        } else {
            return;
        }
    }

    func_80BA36C0(this, play, 0);
    ActorCutscene_Stop(this->cutscene);
    play->actorCtx.flags &= ~ACTORCTX_FLAG_5;
}

s32 EnToto_SubCs_True(EnToto* this, PlayState* play) {
    return true;
}

s32 EnToto_SubCs_False(EnToto* this, PlayState* play) {
    return false;
}

s32 EnToto_Cs_3_4(EnToto* this, PlayState* play) {
    if (this->text->unk1 == 2) {
        func_800B7298(play, NULL, 7);
    }
    return 0;
}

s32 EnToto_SubCs_0_2_3_4_16(EnToto* this, PlayState* play) {
    if (this->text->textId != 0) {
        Message_ContinueTextbox(play, this->text->textId);
    } else {
        Message_CloseTextbox(play);
        EnToto_Cs_3_4(this, play);
    }
    if (this->text->unk0 == 4) {
        func_80151BB4(play, 9);
    }
    return 0;
}

s32 EnToto_SubCs_9_14(EnToto* this, PlayState* play) {
    this->unk2B1 = this->text->unk1;
    return 0;
}

s32 EnToto_Cs_14(EnToto* this, PlayState* play) {
    if (DECR(this->unk2B1) == 0) {
        Message_StartTextbox(play, this->text->textId, NULL);
        return 1;
    }
    return 0;
}

s32 EnToto_SubCs_10_15(EnToto* this, PlayState* play) {
    s16 prevCutscene = this->cutscene;

    this->cutscene = ActorCutscene_GetAdditionalCutscene(this->cutscene);
    ActorCutscene_SetIntentToPlay(this->cutscene);
    ActorCutscene_Stop(prevCutscene);
    return 0;
}

s32 EnToto_Cs_5_10_12_15(EnToto* this, PlayState* play) {
    if (ActorCutscene_GetCanPlayNext(this->cutscene)) {
        ActorCutscene_StartAndSetUnkLinkFields(this->cutscene, &GET_PLAYER(play)->actor);
        return 1;
    }

    ActorCutscene_SetIntentToPlay(this->cutscene);
    return 0;
}

s32 EnToto_Cs_0(EnToto* this, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == TEXT_STATE_5) && Message_ShouldAdvance(play)) {
        return 1;
    }
    return 0;
}

s32 EnToto_Cs_1(EnToto* this, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == TEXT_STATE_CLOSING) {
        return 1;
    }
    return 0;
}

s32 EnToto_Cs_2(EnToto* this, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == TEXT_STATE_CHOICE) && Message_ShouldAdvance(play)) {
        if (play->msgCtx.choiceIndex != 0) {
            func_8019F230();
        } else {
            func_8019F208();
        }
        return (play->msgCtx.choiceIndex != 0 ? 0 : this->text->unk1) + 1; // Possible msg MACRO
    }
    return 0;
}

s32 EnToto_Cs_9(EnToto* this, PlayState* play) {
    EnTotoUnkStruct2* temp_v1_2;

    if (DECR(this->unk2B1) == 0) {
        if (!ENTOTO_WEEK_EVENT_FLAGS) {
            temp_v1_2 = &D_80BA50DC[gSaveContext.save.playerForm - 1];
            Message_StartTextbox(play, (this->text->unk0 == 6) ? temp_v1_2->unk0 : temp_v1_2->unk4, NULL);
        }
        return 1;
    }
    return 0;
}

s32 EnToto_SubCs_6(EnToto* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    u32 phi_s0 = 0;
    Vec3s* end = &D_80BA510C[3];

    EnToto_SubCs_9_14(this, play);
    func_800B7298(play, NULL, 6);
    if (player->actor.world.pos.z > -310.0f) {
        if ((player->actor.world.pos.x > -150.0f) || (player->actor.world.pos.z > -172.0f)) {
            phi_s0 = 3;
        } else {
            if (player->actor.world.pos.z > -232.0f) {
                phi_s0 = 2;
            } else {
                phi_s0 = 1;
            }
        }
    }
    func_80122744(play, &this->unk_2BC, phi_s0, end - phi_s0);
    this->spotlights = Actor_Spawn(&play->actorCtx, play, ACTOR_DM_CHAR07, 0.0f, 0.0f, 0.0f, 0, 0, 0, 0xF02);
    return 0;
}

s32 EnToto_Cs_6(EnToto* this, PlayState* play) {
    func_80BA3C88(this);
    if (func_80122760(play, &this->unk_2BC, 60.0f)) {
        func_800B7298(play, NULL, 0x13);
        return EnToto_Cs_9(this, play);
    }
    return 0;
}

s32 EnToto_Cs_7(EnToto* this, PlayState* play) {
    if (EnToto_Cs_1(this, play)) {
        func_800B7298(play, NULL, 6);
        return 1;
    }
    return 0;
}

s32 EnToto_SubCs_8(EnToto* this, PlayState* play) {
    ActorCutscene_Stop(this->cutscene);
    this->unk2B1 = 0;
    return 0;
}

s32 func_80BA44D4(EnTotoUnkStruct2* arg0, Player* player) {
    Vec3f unk6;

    Math_Vec3s_ToVec3f(&unk6, &arg0->unk6);
    if (Math_Vec3f_DistXZ(&player->actor.world.pos, &unk6) < 10.0f) {
        return 1;
    }
    return 0;
}

s32 EnToto_Cs_8(EnToto* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    EnTotoUnkStruct2* temp_s0;
    s32 i;

    func_80BA3C88(this);
    if (player->actor.world.pos.z > -270.0f) {
        if (this->spotlights != NULL) {
            Actor_Kill(this->spotlights);
        }
        this->unk2B6 = 1;
        return this->text->unk1;
    }
    if (player->actor.bgCheckFlags & 1) {
        temp_s0 = &D_80BA50DC[gSaveContext.save.playerForm - 1];
        if (func_80BA44D4(temp_s0, player)) {
            Math_Vec3s_ToVec3f(&player->actor.world.pos, &temp_s0->unk6);
            player->actor.shape.rot.y = 0;
            player->currentYaw = 0;
            return EnToto_Cs_5_10_12_15(this, play);
        }
        if (!ENTOTO_WEEK_EVENT_FLAGS) {
            for (i = 0; i < 4; i++) {
                if (func_80BA44D4(&D_80BA50DC[i], player)) {
                    if (this->unk2B1 < 10) {
                        this->unk2B1++;
                        if (this->unk2B1 >= 10) {
                            Message_StartTextbox(play, D_80BA50DC[((void)0, gSaveContext.save.playerForm) - 1].unk2,
                                                 NULL);
                        }
                    }
                    return 0;
                }
            }
            this->unk2B1 = 0;
        }
    }
    return 0;
}

s32 EnToto_SubCs_StartOcarina(EnToto* this, PlayState* play) {
    func_800B7298(play, NULL, 0x44);
    Message_StartOcarina(play, sOcarinaActionWIndFishPrompts[CUR_FORM]);
    return 0;
}

s32 EnToto_Cs_EndOcarina(EnToto* this, PlayState* play) {
    if (play->msgCtx.ocarinaMode == OCARINA_MODE_END) {
        if (gSaveContext.save.playerForm == PLAYER_FORM_HUMAN) {
            gSaveContext.save.weekEventReg[56] |= 0x10;
        }
        if (gSaveContext.save.playerForm == PLAYER_FORM_DEKU) {
            gSaveContext.save.weekEventReg[56] |= 0x20;
        }
        if (gSaveContext.save.playerForm == PLAYER_FORM_ZORA) {
            gSaveContext.save.weekEventReg[56] |= 0x40;
        }
        if (gSaveContext.save.playerForm == PLAYER_FORM_GORON) {
            gSaveContext.save.weekEventReg[56] |= 0x80;
        }
        return true;
    }
    return false;
}

s32 EnToto_SubCs_12(EnToto* this, PlayState* play) {
    Vec3f spawnPos;
    s32 i;

    this->unk2B3 = 0;
    if (gSaveContext.save.weekEventReg[56] & 0x10) {
        this->unk2B3 += 1;
    }
    if (gSaveContext.save.weekEventReg[56] & 0x20) {
        this->unk2B3 += 2;
    }
    if (gSaveContext.save.weekEventReg[56] & 0x40) {
        this->unk2B3 += 4;
    }
    if (gSaveContext.save.weekEventReg[56] & 0x80) {
        this->unk2B3 += 8;
    }
    for (i = 0; i < 4; i++) {
        if (gSaveContext.save.playerForm != (i + 1) && (D_80BA5128[i] & this->unk2B3)) {
            Math_Vec3s_ToVec3f(&spawnPos, &D_80BA50DC[i].unk6);
            Actor_Spawn(&play->actorCtx, play, ACTOR_PLAYER, spawnPos.x, spawnPos.y, spawnPos.z, i + 2, 0, 0, -1);
        }
    }
    EnToto_SubCs_10_15(this, play);
    if (ENTOTO_WEEK_EVENT_FLAGS) {
        EnToto_SubCs_10_15(this, play);
    }
    return 0;
}

s32 EnToto_SubCs_13(EnToto* this, PlayState* play) {
    // Player CsCmd to start Ocarina
    func_800B7298(play, NULL, 0x44);
    // Audio_PlayFanfareWithPlayerIOCustomPort
    func_801A31EC(NA_BGM_BALLAD_OF_THE_WIND_FISH, 4, this->unk2B3 ^ 0xF);
    this->unk2B1 = 4;
    return 0;
}

s32 EnToto_Cs_13(EnToto* this, PlayState* play) {
    Actor* actor;

    if (DECR(this->unk2B1) == 0) {
        if (!Audio_IsSequencePlaying(NA_BGM_BALLAD_OF_THE_WIND_FISH)) {
            actor = &GET_PLAYER(play)->actor;
            actor = actor->next;
            while (actor != NULL) {
                Actor_Kill(actor);
                actor = actor->next;
            }
            if (this->spotlights != NULL) {
                Actor_Kill(this->spotlights);
            }
            func_800B7298(play, NULL, 0x45);
            if (this->unk2B3 == 0xF) {
                if (CURRENT_DAY == 1) {
                    gSaveContext.save.weekEventReg[50] |= 1;
                } else {
                    gSaveContext.save.weekEventReg[51] |= 0x80;
                }
            } else {
                EnToto_SubCs_10_15(this, play);
            }
            return 1;
        }
    }
    return 0;
}

s32 EnToto_Cs_16(EnToto* this, PlayState* play) {
    Player* player;

    if (EnToto_Cs_0(this, play)) {
        player = GET_PLAYER(play);
        Animation_MorphToPlayOnce(&this->skelAnime, &object_zm_Anim_0028B8, -4.0f);
        if (player->transformation == PLAYER_FORM_ZORA) {
            if (!Flags_GetSwitch(play, this->actor.params & 0x7F)) {
                Flags_SetSwitch(play, this->actor.params & 0x7F);
                return 1;
            } else {
                return 3;
            }
        } else {
            if (!Flags_GetSwitch(play, (this->actor.params >> 7) & 0x7F)) {
                Flags_SetSwitch(play, (this->actor.params >> 7) & 0x7F);
                return 4;
            } else {
                return 7;
            }
        }
    }
    return 0;
}

s32 func_80BA4C0C(EnToto* this, PlayState* play) {
    return D_80BA512C[this->text->unk0](this, play);
}

s32 func_80BA4C44(EnToto* this, PlayState* play) {
    s32 ret;

    ret = D_80BA5174[this->text->unk0](this, play);
    if (ret != 0) {
        this->text += ret;
        return func_80BA4C0C(this, play);
    }
    return 0;
}

void func_80BA4CB4(EnToto* this, PlayState* play) {
    CsCmdActorAction* action = play->csCtx.actorActions[Cutscene_GetActorActionIndex(play, 525)];

    if (this->unk2B5 != action->action) {
        this->unk2B5 = action->action;
        if (this->unk2B5 != 4) {
            if (this->unk2B5 == 3) {
                Animation_MorphToPlayOnce(&this->skelAnime, &object_zm_Anim_001DF0, -4.0f);
            } else {
                Animation_PlayOnce(&this->skelAnime,
                                   this->unk2B5 == 1 ? &object_zm_Anim_0016A4 : &object_zm_Anim_0022C8);
                if (this->unk2B5 == 2 && this->unk2B3 != 0xF) {
                    func_80151BB4(play, 9);
                    func_80151BB4(play, 10);
                }
            }
        }
    }
    Math_ScaledStepToS(&this->actor.shape.rot.y, this->actor.home.rot.y, 0x320);
    if (SkelAnime_Update(&this->skelAnime)) {
        if (this->unk2B5 != 3) {
            Animation_PlayLoop(&this->skelAnime, this->unk2B5 == 1 ? &object_zm_Anim_00C880 : &object_zm_Anim_001324);
        }
    }
    if (this->unk2B5 == 4 && !Actor_HasParent(&this->actor, play)) {
        Actor_PickUp(&this->actor, play, GI_MASK_CIRCUS_LEADER, 9999.9f, 9999.9f);
    }
}

void EnToto_Update(Actor* thisx, PlayState* play) {
    EnToto* this = THIS;
    s32 pad;

    if (Cutscene_CheckActorAction(play, 0x20D)) {
        func_80BA4CB4(this, play);
    } else {
        D_80BA51B8[this->actionFuncIndex](this, play);
    }

    Collider_ResetCylinderAC(play, &this->collider.base);
    Collider_UpdateCylinder(&this->actor, &this->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    Actor_SetFocus(&this->actor, 40.0f);
}

void EnToto_Draw(Actor* thisx, PlayState* play) {
    TexturePtr sp4C[] = { object_zm_Tex_008AE8, object_zm_Tex_00A068, object_zm_Tex_00A468 };
    EnToto* this = THIS;
    s32 pad;

    OPEN_DISPS(play->state.gfxCtx);

    func_8012C28C(play->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, Lib_SegmentedToVirtual(sp4C[this->unk260[0]]));
    Scene_SetRenderModeXlu(play, 0, 1);
    SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount, NULL,
                          NULL, &this->actor);

    CLOSE_DISPS(play->state.gfxCtx);
}
