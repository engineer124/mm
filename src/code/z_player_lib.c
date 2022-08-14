#include "prevent_bss_reordering.h"
#include "global.h"

#include "objects/gameplay_keep/gameplay_keep.h"

#include "objects/object_link_boy/object_link_boy.h"
#include "objects/object_link_goron/object_link_goron.h"
#include "objects/object_link_zora/object_link_zora.h"
#include "objects/object_link_nuts/object_link_nuts.h"
#include "objects/object_link_child/object_link_child.h"

#include "objects/object_mask_truth/object_mask_truth.h"
#include "objects/object_mask_kerfay/object_mask_kerfay.h"
#include "objects/object_mask_yofukasi/object_mask_yofukasi.h"
#include "objects/object_mask_rabit/object_mask_rabit.h"
#include "objects/object_mask_ki_tan/object_mask_ki_tan.h"
#include "objects/object_mask_json/object_mask_json.h"
#include "objects/object_mask_romerny/object_mask_romerny.h"
#include "objects/object_mask_zacho/object_mask_zacho.h"
#include "objects/object_mask_posthat/object_mask_posthat.h"
#include "objects/object_mask_meoto/object_mask_meoto.h"
#include "objects/object_mask_bigelf/object_mask_bigelf.h"
#include "objects/object_mask_gibudo/object_mask_gibudo.h"
#include "objects/object_mask_gero/object_mask_gero.h"
#include "objects/object_mask_dancer/object_mask_dancer.h"
#include "objects/object_mask_skj/object_mask_skj.h"
#include "objects/object_mask_stone/object_mask_stone.h"
#include "objects/object_mask_bree/object_mask_bree.h"
#include "objects/object_mask_bakuretu/object_mask_bakuretu.h"
#include "objects/object_mask_bu_san/object_mask_bu_san.h"
#include "objects/object_mask_kyojin/object_mask_kyojin.h"
#include "objects/object_mask_boy/object_mask_boy.h"
#include "objects/object_mask_goron/object_mask_goron.h"
#include "objects/object_mask_zora/object_mask_zora.h"
#include "objects/object_mask_nuts/object_mask_nuts.h"

void PlayerCall_Init(Actor* thisx, PlayState* play);
void PlayerCall_Destroy(Actor* thisx, PlayState* play);
void PlayerCall_Update(Actor* thisx, PlayState* play);
void PlayerCall_Draw(Actor* thisx, PlayState* play);

typedef struct {
    /* 0x00 */ Vec3f unk_00;
    /* 0x0C */ Vec3f unk_0C;
    /* 0x18 */ s16 unk_18;
    /* 0x1A */ s16 unk_1A;
} struct_801F58B0; // size = 0x1C

struct_801F58B0 D_801F58B0[3][3];

Vec3f D_801F59B0[2];

s32 D_801F59C8[2];

typedef struct {
    /* 0x0 */ s16 unk_0;
    /* 0x2 */ s16 unk_2;
    /* 0x4 */ s16 unk_4;
    /* 0x6 */ s16 unk_6;
    /* 0x8 */ s16 unk_8;
} struct_801F59D0; // size = 0xA

struct_801F59D0 D_801F59D0;

Vec3f* D_801F59DC;

s32 D_801F59E0;

s32 D_801F59E4;

Vec3f D_801F59E8;

s32 D_801F59F4;
s32 D_801F59F8;

void func_80127B64(struct_801F58B0 arg0[], s32 count, Vec3f* arg2);

s32 func_801226E0(PlayState* play, s32 arg1) {
    if (arg1 == 0) {
        Play_SetupRespawnPoint(&play->state, RESPAWN_MODE_DOWN, 0xBFF);
        if (play->sceneNum == SCENE_KAKUSIANA) {
            return 1;
        }
    }

    gSaveContext.respawn[RESPAWN_MODE_DOWN].data = 0;
    return arg1;
}

s32 func_80122744(PlayState* play, struct_80122744_arg1* arg1, u32 arg2, Vec3s* arg3) {
    arg1->unk_00 = arg2;
    arg1->unk_01 = 0;
    arg1->unk_04 = arg3;
    return 1;
}

s32 func_80122760(PlayState* play, struct_80122744_arg1* arg1, f32 arg2) {
    if (arg1->unk_01 < arg1->unk_00) {
        Player* player = GET_PLAYER(play);
        Vec3f sp30;
        s32 pad;
        s16 yaw;
        f32 distXZ;

        Math_Vec3s_ToVec3f(&sp30, &arg1->unk_04[arg1->unk_01]);
        yaw = Math_Vec3f_Yaw(&player->actor.world.pos, &sp30);
        func_800B6F20(play, &play->actorCtx.unk_26C, arg2, yaw);
        play->actorCtx.unk268 = 1;
        distXZ = Math_Vec3f_DistXZ(&player->actor.world.pos, &sp30);

        if ((fabsf(player->actor.world.pos.y - sp30.y) < 50.0f) && (distXZ < arg2)) {
            arg1->unk_01++;
        }

        return false;
    }

    return true;
}

void func_80122868(PlayState* play, Player* player) {
    OPEN_DISPS(play->state.gfxCtx);

    if (player->invincibilityTimer > 0) {
        s32 phi_v0 = 50 - player->invincibilityTimer;

        phi_v0 = CLAMP(phi_v0, 8, 40);

        player->unk_B5F += phi_v0;
        POLY_OPA_DISP =
            Gfx_SetFog(POLY_OPA_DISP, 255, 0, 0, 0, 0, 4000 - (s32)(Math_CosS(player->unk_B5F << 8) * 2000.0f));
    } else if (gSaveContext.jinxTimer != 0) {
        player->unk_B5F += 10;
        POLY_OPA_DISP =
            Gfx_SetFog(POLY_OPA_DISP, 0, 0, 255, 0, 0, 4000 - (s32)(Math_CosS(player->unk_B5F << 8) * 2000.0f));
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

void func_801229A0(PlayState* play, Player* player) {
    OPEN_DISPS(play->state.gfxCtx);

    if ((gSaveContext.jinxTimer != 0) || (player->invincibilityTimer > 0)) {
        POLY_OPA_DISP = func_801660B8(play, POLY_OPA_DISP);
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

void func_801229EC(Actor* thisx, PlayState* play) {
}

s16 sMaskObjectIds[PLAYER_MASK_MAX - 1] = {
    OBJECT_MASK_TRUTH,  OBJECT_MASK_KERFAY,  OBJECT_MASK_YOFUKASI, OBJECT_MASK_RABIT,   OBJECT_MASK_KI_TAN,
    OBJECT_MASK_JSON,   OBJECT_MASK_ROMERNY, OBJECT_MASK_ZACHO,    OBJECT_MASK_POSTHAT, OBJECT_MASK_MEOTO,
    OBJECT_MASK_BIGELF, OBJECT_MASK_GIBUDO,  OBJECT_MASK_GERO,     OBJECT_MASK_DANCER,  OBJECT_MASK_SKJ,
    OBJECT_MASK_STONE,  OBJECT_MASK_BREE,    OBJECT_MASK_BAKURETU, OBJECT_MASK_BU_SAN,  OBJECT_MASK_KYOJIN,
    OBJECT_MASK_BOY,    OBJECT_MASK_GORON,   OBJECT_MASK_ZORA,     OBJECT_MASK_NUTS,
};

// Load mask object?
/**
 * Notes:
 *
 * player->maskObjectLoadState seems to be able to take 3 possible values
 *    0: The mask object is loaded.
 *    1: The mask object must be changed (and the DMA request has not been sent yet)
 *    2: Waiting for the DMA request to complete.
 */
void func_801229FC(Player* player) {
    if (player->maskObjectLoadState == 1) {
        s16 objectId = sMaskObjectIds[(u8)player->maskId - 1];

        osCreateMesgQueue(&player->maskObjectLoadQueue, &player->maskObjectLoadMsg, 1);
        DmaMgr_SendRequestImpl(&player->maskDmaRequest, player->maskObjectSegment, gObjectTable[objectId].vromStart,
                               gObjectTable[objectId].vromEnd - gObjectTable[objectId].vromStart, 0,
                               &player->maskObjectLoadQueue, NULL);
        player->maskObjectLoadState++;
    } else if (player->maskObjectLoadState == 2) {
        if (osRecvMesg(&player->maskObjectLoadQueue, NULL, OS_MESG_NOBLOCK) == 0) {
            player->maskObjectLoadState = 0;

            if (player->currentMask == PLAYER_MASK_GREAT_FAIRY) {
                s32 i;

                for (i = 0; i < ARRAY_COUNT(D_801F58B0); i++) {
                    func_80127B64(D_801F58B0[i], ARRAY_COUNT(D_801F58B0[i]), &player->bodyPartsPos[7]);
                }
            }
        }
    } else if ((player->currentMask != PLAYER_MASK_NONE) && (player->currentMask != (u8)player->maskId)) {
        player->maskObjectLoadState = 1;
        player->maskId = player->currentMask;
    } else if (player->currentMask == PLAYER_MASK_CIRCUS_LEADER) {
        s32 i;

        for (i = 0; i < ARRAY_COUNT(D_801F59C8); i++) {
            D_801F59C8[i] += Rand_S16Offset(4, 23) + (s32)(fabsf(player->linearVelocity) * 50.0f);
        }
    }
}

void func_80122BA4(PlayState* play, struct_80122D44_arg1* arg1, s32 arg2, s32 alpha) {
    if (arg2 == arg1->unk_00) {
        s32 index;

        arg1->unk_01--;
        if (arg1->unk_01 < 0) {
            arg1->unk_01 = 3;
        }

        index = arg1->unk_01;
        arg1->unk_04[index].unk_00 = arg1->unk_00;
        arg1->unk_04[index].alpha = alpha;
        Matrix_Get(&arg1->unk_04[index].mf);

        arg1->unk_00 = 0;
    }
}

void func_80122C20(PlayState* play, struct_80122D44_arg1* arg1) {
    struct_80122D44_arg1_unk_04* temp_v1 = &arg1->unk_04[0];
    s32 phi_a1;
    s32 i;

    for (i = 0; i < ARRAY_COUNT(arg1->unk_04); i++, temp_v1++) {
        // Can't be `temp_v1->alpha != 0`
        if (temp_v1->alpha) {
            phi_a1 = temp_v1->unk_00 == 3 ? (255 / 3) : (255 / 5);
            if (phi_a1 >= temp_v1->alpha) {
                temp_v1->alpha = 0;
            } else {
                temp_v1->alpha -= phi_a1;
            }
        }
    }
}

typedef struct {
    /* 0x0 */ Color_RGB8 color;
    /* 0x4 */ Gfx* dList;
} struct_801BFDD0; // size = 0x08

struct_801BFDD0 D_801BFDD0[] = {
    { { 180, 200, 255 }, gLinkGoronCurledDL },
    { { 155, 0, 0 }, gLinkGoronRollingSpikesAndEffectDL },
    { { 255, 0, 0 }, gLinkGoronGoronPunchEffectDL },
};

void func_80122D44(PlayState* play, struct_80122D44_arg1* arg1) {
    struct_801BFDD0* temp_s3;
    struct_80122D44_arg1_unk_04* phi_s2 = arg1->unk_04;
    s32 phi_s6 = false;
    s32 i;

    OPEN_DISPS(play->state.gfxCtx);

    for (i = 0; i != ARRAY_COUNT(arg1->unk_04); i++) {
        if ((phi_s2->alpha != 0) && (phi_s2->alpha != 255)) {
            temp_s3 = &D_801BFDD0[phi_s2->unk_00 - 1];
            Matrix_Put(&phi_s2->mf);

            gDPPipeSync(POLY_XLU_DISP++);

            if (!phi_s6 && phi_s2->unk_00 == 2) {
                AnimatedMat_DrawXlu(play, Lib_SegmentedToVirtual(&object_link_goron_Matanimheader_013138));
                phi_s6 = true;
            }

            Scene_SetRenderModeXlu(play, 1, 2);
            gDPSetEnvColor(POLY_XLU_DISP++, temp_s3->color.r, temp_s3->color.g, temp_s3->color.b, phi_s2->alpha);

            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

            gSPDisplayList(POLY_XLU_DISP++, temp_s3->dList);
        }

        phi_s2++;
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

u8 sMaskItemIds[PLAYER_MASK_MAX - 1] = {
    ITEM_MASK_TRUTH,         // PLAYER_MASK_TRUTH
    ITEM_MASK_KAFEIS_MASK,   // PLAYER_MASK_KAFEIS_MASK
    ITEM_MASK_ALL_NIGHT,     // PLAYER_MASK_ALL_NIGHT
    ITEM_MASK_BUNNY,         // PLAYER_MASK_BUNNY
    ITEM_MASK_KEATON,        // PLAYER_MASK_KEATON
    ITEM_MASK_GARO,          // PLAYER_MASK_GARO
    ITEM_MASK_ROMANI,        // PLAYER_MASK_ROMANI
    ITEM_MASK_CIRCUS_LEADER, // PLAYER_MASK_CIRCUS_LEADER
    ITEM_MASK_POSTMAN,       // PLAYER_MASK_POSTMAN
    ITEM_MASK_COUPLE,        // PLAYER_MASK_COUPLE
    ITEM_MASK_GREAT_FAIRY,   // PLAYER_MASK_GREAT_FAIRY
    ITEM_MASK_GIBDO,         // PLAYER_MASK_GIBDO
    ITEM_MASK_DON_GERO,      // PLAYER_MASK_DON_GERO
    ITEM_MASK_KAMARO,        // PLAYER_MASK_KAMARO
    ITEM_MASK_CAPTAIN,       // PLAYER_MASK_CAPTAIN
    ITEM_MASK_STONE,         // PLAYER_MASK_STONE
    ITEM_MASK_BREMEN,        // PLAYER_MASK_BREMEN
    ITEM_MASK_BLAST,         // PLAYER_MASK_BLAST
    ITEM_MASK_SCENTS,        // PLAYER_MASK_SCENTS
    ITEM_MASK_GIANT,         // PLAYER_MASK_GIANT
    ITEM_MASK_FIERCE_DEITY,  // PLAYER_MASK_FIERCE_DEITY
    ITEM_MASK_GORON,         // PLAYER_MASK_GORON
    ITEM_MASK_ZORA,          // PLAYER_MASK_ZORA
    ITEM_MASK_DEKU,          // PLAYER_MASK_DEKU
};

u8 Player_MaskIdToItemId(s32 maskIdMinusOne) {
    return sMaskItemIds[maskIdMinusOne];
}

u8 Player_GetCurMaskItemId(PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (player->currentMask != PLAYER_MASK_NONE) {
        return Player_MaskIdToItemId(player->currentMask - 1);
    }

    return ITEM_NONE;
}

void func_80122F28(Player* player) {
    if ((player->actor.category == ACTORCAT_PLAYER) &&
        (!(player->stateFlags1 & (PLAYER_STATE1_400 | PLAYER_STATE1_800 | PLAYER_STATE1_200000 | PLAYER_STATE1_800000 |
                                  PLAYER_STATE1_20000000))) &&
        (!(player->stateFlags2 & PLAYER_STATE2_1))) {
        if (player->doorType <= PLAYER_DOORTYPE_MINUS_1) {
            ActorCutscene_SetIntentToPlay(0x7C);
        } else {
            ActorCutscene_SetIntentToPlay(0x7D);
        }
    }
}

s32 func_80122F9C(PlayState* play) {
    Player* player = GET_PLAYER(play);

    return (player->stateFlags2 & PLAYER_STATE2_80000) && player->unk_AE7 == 2;
}

s32 func_80122FCC(PlayState* play) {
    Player* player = GET_PLAYER(play);

    return (player->stateFlags2 & PLAYER_STATE2_80000) && (player->unk_AE7 == 1 || player->unk_AE7 == 3);
}

void func_8012300C(PlayState* play, s32 arg1) {
    Player* player = GET_PLAYER(play);

    player->unk_B2B = arg1;
}

void func_8012301C(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    Player* this = (Player*)thisx;

    this->unk_AE7++;

    if (this->unk_AE7 == 2) {
        s16 objectId = gPlayerFormObjectIndices[GET_PLAYER_FORM];

        gActorOverlayTable[ACTOR_PLAYER].initInfo->objectId = objectId;
        func_8012F73C(&play->objectCtx, this->actor.objBankIndex, objectId);
        this->actor.objBankIndex = Object_GetIndex(&play->objectCtx, GAMEPLAY_KEEP);
    } else if (this->unk_AE7 >= 3) {
        s32 objBankIndex = Object_GetIndex(&play->objectCtx, gActorOverlayTable[ACTOR_PLAYER].initInfo->objectId);

        if (Object_IsLoaded(&play->objectCtx, objBankIndex)) {
            this->actor.objBankIndex = objBankIndex;
            this->actor.shape.rot.z = GET_PLAYER_FORM + 1;
            this->actor.init = PlayerCall_Init;
            this->actor.update = PlayerCall_Update;
            this->actor.draw = PlayerCall_Draw;
            gSaveContext.save.equippedMask = PLAYER_MASK_NONE;
        }
    }
}

FlexSkeletonHeader* gPlayerSkeletons[PLAYER_FORM_MAX] = {
    &gLinkFierceDeitySkel, &gLinkGoronSkel, &gLinkZoraSkel, &gLinkDekuSkel, &gLinkHumanSkel,
};

s16 D_801BFE14[][18] = {
    { 200, 666, 200, 700, 366, 200, 600, 175, 60, 800, 1000, -100, 600, 590, 800, 125, 300, 65 },
    { 200, 1000, 300, 800, 500, 400, 800, 400, 120, 800, 550, -100, 600, 540, 750, 125, 400, 200 },
    { 100, 1000, 300, 800, 250, 200, 800, 200, 90, 800, 350, -80, 600, 540, 750, 60, 200, 200 },
    { 200, 1000, 300, 700, 550, 270, 600, 1000, 120, 800, 600, -100, 600, 590, 750, 125, 200, 130 },
    { 200, 1000, 300, 700, 550, 270, 700, 300, 120, 800, 600, -100, 600, 590, 750, 125, 200, 130 },
    { 200, 1000, 300, 700, 550, 270, 700, 300, 120, 800, 600, -100, 600, 590, 750, 125, 200, 130 },
    { 200, 1000, 300, 700, 550, 270, 700, 200, 120, 800, 600, -140, 600, 590, 750, 125, 200, 130 },
    { 80, 800, 150, 700, 480, 270, 600, 50, 120, 800, 300, -40, 400, 540, 270, 25, 0, 80 },
};

// OoT's Player_SetBootData
void func_80123140(PlayState* play, Player* player) {
    s16* bootRegs;
    s32 currentBoots;
    f32 scale;

    if ((player->actor.id == ACTOR_PLAYER) && (player->transformation == PLAYER_FORM_FIERCE_DEITY)) {
        REG(27) = 1200;
    } else {
        REG(27) = 2000;
    }

    REG(48) = 370;

    currentBoots = player->currentBoots;
    if (currentBoots >= PLAYER_BOOTS_ZORA_UNDERWATER) {
        if (player->stateFlags1 & PLAYER_STATE1_8000000) {
            currentBoots++;
        }
        if (player->transformation == PLAYER_FORM_GORON) {
            REG(48) = 200;
        }
    } else if (currentBoots == PLAYER_BOOTS_GIANT) {
        REG(48) = 170;
    }

    bootRegs = D_801BFE14[currentBoots];
    REG(19) = bootRegs[0];
    REG(30) = bootRegs[1];
    REG(32) = bootRegs[2];
    REG(34) = bootRegs[3];
    REG(35) = bootRegs[4];
    REG(36) = bootRegs[5];
    REG(37) = bootRegs[6];
    REG(38) = bootRegs[7];
    REG(39) = bootRegs[8];
    REG(43) = bootRegs[9];
    R_RUN_SPEED_LIMIT = bootRegs[10];
    REG(68) = bootRegs[11];
    REG(69) = bootRegs[12];
    IREG(66) = bootRegs[13];
    IREG(67) = bootRegs[14];
    IREG(68) = bootRegs[15];
    IREG(69) = bootRegs[16];
    MREG(95) = bootRegs[17];

    if (play->roomCtx.currRoom.unk3 == 2) {
        R_RUN_SPEED_LIMIT = 500;
    }

    if ((player->actor.id == ACTOR_PLAYER) && (player->transformation == PLAYER_FORM_FIERCE_DEITY)) {
        scale = 0.015f;
    } else {
        scale = 0.01f;
    }

    Actor_SetScale(&player->actor, scale);
}

s32 Player_InBlockingCsMode(PlayState* play, Player* player) {
    return (player->stateFlags1 & (PLAYER_STATE1_80 | PLAYER_STATE1_200 | PLAYER_STATE1_20000000)) ||
           (player->csMode != PLAYER_CSMODE_0) || (play->transitionTrigger == TRANS_TRIGGER_START) ||
           (play->transitionMode != TRANS_MODE_OFF) || (player->stateFlags1 & PLAYER_STATE1_1) ||
           (player->stateFlags3 & PLAYER_STATE3_80) || (play->actorCtx.unk268 != 0);
}

s32 Player_InCsMode(PlayState* play) {
    Player* player = GET_PLAYER(play);

    return Player_InBlockingCsMode(play, player) || player->unk_AA5 == 5;
}

s32 func_80123420(Player* player) {
    return player->stateFlags3 & PLAYER_STATE3_80000000;
}

s32 func_80123434(Player* player) {
    return player->stateFlags1 & (PLAYER_STATE1_10000 | PLAYER_STATE1_20000 | PLAYER_STATE1_40000000);
}

// Unused
s32 func_80123448(PlayState* play) {
    Player* player = GET_PLAYER(play);

    return (player->stateFlags1 & PLAYER_STATE1_400000) &&
           (player->transformation != PLAYER_FORM_HUMAN || (!func_80123434(player) && player->targetedActor == NULL));
}

// TODO: Player_IsGoronOrDeku is a temporary name until we have more info on this function.
// Hypothesis: this function checks if the current form would crouch when he tries to use the shield
s32 Player_IsGoronOrDeku(Player* player) {
    return player->transformation == PLAYER_FORM_GORON || player->transformation == PLAYER_FORM_DEKU;
}

s32 func_801234D4(PlayState* play) {
    Player* player = GET_PLAYER(play);

    return (player->stateFlags2 & PLAYER_STATE2_8) || player->actor.speedXZ != 0.0f ||
           ((player->transformation != PLAYER_FORM_ZORA) && (player->stateFlags1 & PLAYER_STATE1_8000000)) ||
           ((player->transformation == PLAYER_FORM_ZORA) && (player->stateFlags1 & PLAYER_STATE1_8000000) &&
            (!(player->actor.bgCheckFlags & 1) || (player->currentBoots < PLAYER_BOOTS_ZORA_UNDERWATER)));
}

s32 func_80123590(PlayState* play, Actor* actor) {
    Player* player = GET_PLAYER(play);

    if ((player->stateFlags1 & PLAYER_STATE1_800) && (player->heldActor == actor)) {
        player->interactRangeActor = NULL;
        player->heldActor = NULL;
        player->actor.child = NULL;
        player->stateFlags1 &= ~PLAYER_STATE1_800;
        return true;
    }

    return false;
}

s32 func_801235DC(PlayState* play, f32 arg1, s16 arg2) {
    Player* player = GET_PLAYER(play);

    if (player->stateFlags3 & PLAYER_STATE3_1000) {
        player->unk_B08[0] = arg1;
        player->unk_B08[1] += arg1 * 0.05f;
        player->currentYaw = arg2;
        player->actor.home.rot.y = arg2;
        player->actor.shape.rot.y = arg2;
        player->unk_B8C = 4;
        player->invincibilityTimer = 20;

        return true;
    }

    return false;
}

ItemID func_8012364C(PlayState* play, Player* player, s32 arg2) {
    if (arg2 >= 4) {
        return ITEM_NONE;
    }

    if (arg2 == 0) {
        s32 item = Inventory_GetBtnBItem(play);

        if (item >= ITEM_FD) {
            return item;
        }

        if ((player->currentMask == PLAYER_MASK_BLAST) && (play->interfaceCtx.bButtonDoAction == 0x18)) {
            return ITEM_F0;
        }

        if ((player->currentMask == PLAYER_MASK_BREMEN) && (play->interfaceCtx.bButtonDoAction == 0x1A)) {
            return ITEM_F1;
        }

        if ((player->currentMask == PLAYER_MASK_KAMARO) && (play->interfaceCtx.bButtonDoAction == 0x19)) {
            return ITEM_F2;
        }

        return item;
    }

    if (arg2 == 1) {
        return C_BTN_ITEM(EQUIP_SLOT_C_LEFT);
    }

    if (arg2 == 2) {
        return C_BTN_ITEM(EQUIP_SLOT_C_DOWN);
    }

    return C_BTN_ITEM(EQUIP_SLOT_C_RIGHT);
}

u16 sCItemButtons[] = { BTN_CLEFT, BTN_CDOWN, BTN_CRIGHT };

PlayerActionParam func_80123810(PlayState* play) {
    Player* player = GET_PLAYER(play);
    PlayerActionParam actionParam;
    ItemID itemId;
    s32 i;

    if (gSaveContext.save.unk_06 == 0) {
        if (CHECK_BTN_ANY(CONTROLLER1(&play->state)->press.button, BTN_A | BTN_B)) {
            play->interfaceCtx.unk_222 = 0;
            play->interfaceCtx.unk_224 = 0;
            Interface_ChangeAlpha(play->msgCtx.unk_120BC);
            return -1;
        }
    } else {
        gSaveContext.save.unk_06--;
    }

    for (i = 0; i < ARRAY_COUNT(sCItemButtons); i++) {
        if (CHECK_BTN_ALL(CONTROLLER1(&play->state)->press.button, sCItemButtons[i])) {
            i++;
            itemId = func_8012364C(play, player, i);

            play->interfaceCtx.unk_222 = 0;
            play->interfaceCtx.unk_224 = 0;
            Interface_ChangeAlpha(play->msgCtx.unk_120BC);

            if ((itemId >= ITEM_FD) || ((actionParam = play->unk_18794(play, player, itemId)) < PLAYER_AP_NONE)) {
                play_sound(NA_SE_SY_ERROR);
                return -1;
            } else {
                s32 pad;

                player->heldItemButton = i;
                return actionParam;
            }
        }
    }

    return PLAYER_AP_NONE;
}

// Used to map action params to model groups
u8 sActionModelGroups[PLAYER_AP_MAX] = {
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_NONE
    PLAYER_MODELGROUP_13,             // PLAYER_AP_LAST_USED
    PLAYER_MODELGROUP_STICK,          // PLAYER_AP_FISHING_POLE
    PLAYER_MODELGROUP_ONE_HAND_SWORD, // PLAYER_AP_SWORD_KOKIRI
    PLAYER_MODELGROUP_ONE_HAND_SWORD, // PLAYER_AP_SWORD_RAZOR
    PLAYER_MODELGROUP_ONE_HAND_SWORD, // PLAYER_AP_SWORD_GILDED
    PLAYER_MODELGROUP_TWO_HAND_SWORD, // PLAYER_AP_SWORD_GREAT_FAIRY
    PLAYER_MODELGROUP_STICK,          // PLAYER_AP_STICK
    PLAYER_MODELGROUP_ZORA_FINS,      // PLAYER_AP_ZORA_FINS
    PLAYER_MODELGROUP_BOW,            // PLAYER_AP_BOW
    PLAYER_MODELGROUP_BOW,            // PLAYER_AP_BOW_FIRE
    PLAYER_MODELGROUP_BOW,            // PLAYER_AP_BOW_ICE
    PLAYER_MODELGROUP_BOW,            // PLAYER_AP_BOW_LIGHT
    PLAYER_MODELGROUP_HOOKSHOT,       // PLAYER_AP_HOOKSHOT
    PLAYER_MODELGROUP_EXPLOSIVES,     // PLAYER_AP_BOMB
    PLAYER_MODELGROUP_EXPLOSIVES,     // PLAYER_AP_POWDER_KEG
    PLAYER_MODELGROUP_EXPLOSIVES,     // PLAYER_AP_BOMBCHU
    PLAYER_MODELGROUP_8,              // PLAYER_AP_11
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_NUT
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_PICTO_BOX
    PLAYER_MODELGROUP_INSTRUMENT,     // PLAYER_AP_OCARINA
    PLAYER_MODELGROUP_BOTTLE,         // PLAYER_AP_BOTTLE
    PLAYER_MODELGROUP_BOTTLE,         // PLAYER_AP_BOTTLE_FISH
    PLAYER_MODELGROUP_BOTTLE,         // PLAYER_AP_BOTTLE_SPRING_WATER
    PLAYER_MODELGROUP_BOTTLE,         // PLAYER_AP_BOTTLE_HOT_SPRING_WATER
    PLAYER_MODELGROUP_BOTTLE,         // PLAYER_AP_BOTTLE_ZORA_EGG
    PLAYER_MODELGROUP_BOTTLE,         // PLAYER_AP_BOTTLE_DEKU_PRINCESS
    PLAYER_MODELGROUP_BOTTLE,         // PLAYER_AP_BOTTLE_GOLD_DUST
    PLAYER_MODELGROUP_BOTTLE,         // PLAYER_AP_BOTTLE_1C
    PLAYER_MODELGROUP_BOTTLE,         // PLAYER_AP_BOTTLE_SEA_HORSE
    PLAYER_MODELGROUP_BOTTLE,         // PLAYER_AP_BOTTLE_MUSHROOM
    PLAYER_MODELGROUP_BOTTLE,         // PLAYER_AP_BOTTLE_HYLIAN_LOACH
    PLAYER_MODELGROUP_BOTTLE,         // PLAYER_AP_BOTTLE_BUG
    PLAYER_MODELGROUP_BOTTLE,         // PLAYER_AP_BOTTLE_POE
    PLAYER_MODELGROUP_BOTTLE,         // PLAYER_AP_BOTTLE_BIG_POE
    PLAYER_MODELGROUP_BOTTLE,         // PLAYER_AP_BOTTLE_POTION_RED
    PLAYER_MODELGROUP_BOTTLE,         // PLAYER_AP_BOTTLE_POTION_BLUE
    PLAYER_MODELGROUP_BOTTLE,         // PLAYER_AP_BOTTLE_POTION_GREEN
    PLAYER_MODELGROUP_BOTTLE,         // PLAYER_AP_BOTTLE_MILK
    PLAYER_MODELGROUP_BOTTLE,         // PLAYER_AP_BOTTLE_MILK_HALF
    PLAYER_MODELGROUP_BOTTLE,         // PLAYER_AP_BOTTLE_CHATEAU
    PLAYER_MODELGROUP_BOTTLE,         // PLAYER_AP_BOTTLE_FAIRY
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_MOON_TEAR
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_DEED_LAND
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_ROOM_KEY
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_LETTER_TO_KAFEI
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_MAGIC_BEANS
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_DEED_SWAMP
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_DEED_MOUNTAIN
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_DEED_OCEAN
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_32
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_LETTER_MAMA
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_34
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_35
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_PENDANT_MEMORIES
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_37
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_38
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_39
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_MASK_TRUTH
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_MASK_KAFEIS_MASK
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_MASK_ALL_NIGHT
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_MASK_BUNNY
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_MASK_KEATON
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_MASK_GARO
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_MASK_ROMANI
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_MASK_CIRCUS_LEADER
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_MASK_POSTMAN
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_MASK_COUPLE
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_MASK_GREAT_FAIRY
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_MASK_GIBDO
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_MASK_DON_GERO
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_MASK_KAMARO
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_MASK_CAPTAIN
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_MASK_STONE
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_MASK_BREMEN
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_MASK_BLAST
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_MASK_SCENTS
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_MASK_GIANT
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_MASK_FIERCE_DEITY
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_MASK_GORON
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_MASK_ZORA
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_MASK_DEKU
    PLAYER_MODELGROUP_DEFAULT,        // PLAYER_AP_LENS
};

PlayerModelGroup Player_ActionToModelGroup(Player* player, PlayerActionParam actionParam) {
    PlayerModelGroup modelGroup = sActionModelGroups[actionParam];

    if ((modelGroup == PLAYER_MODELGROUP_ONE_HAND_SWORD) && Player_IsGoronOrDeku(player)) {
        return PLAYER_MODELGROUP_1;
    }
    return modelGroup;
}

u8 D_801BFF90[PLAYER_FORM_MAX] = {
    PLAYER_BOOTS_FIERCE_DEITY, // PLAYER_FORM_FIERCE_DEITY
    PLAYER_BOOTS_GORON,        // PLAYER_FORM_GORON
    PLAYER_BOOTS_ZORA_LAND,    // PLAYER_FORM_ZORA
    PLAYER_BOOTS_DEKU,         // PLAYER_FORM_DEKU
    PLAYER_BOOTS_HYLIAN,       // PLAYER_FORM_HUMAN
};

u8 sPlayerStrengths[PLAYER_FORM_MAX] = {
    PLAYER_STRENGTH_HUMAN, // PLAYER_FORM_FIERCE_DEITY
    PLAYER_STRENGTH_GORON, // PLAYER_FORM_GORON
    PLAYER_STRENGTH_ZORA,  // PLAYER_FORM_ZORA
    PLAYER_STRENGTH_DEKU,  // PLAYER_FORM_DEKU
    PLAYER_STRENGTH_HUMAN, // PLAYER_FORM_HUMAN
};

typedef struct {
    /* 0x00 */ u8 flag;
    /* 0x02 */ u16 textId;
} TextTriggerEntry; // size = 0x04

// These textIds are OoT remnants. The corresponding text entries are not present in this game, and so these don't point
// to anything relevant.
TextTriggerEntry sEnvironmentTextTriggers[] = {
    { 1, 0x26FC },
    { 2, 0x26FD },
    { 0, 0 },
    { 2, 0x26FD },
};

PlayerModelIndices gPlayerModelTypes[PLAYER_MODELGROUP_MAX] = {
    /* PLAYER_MODELGROUP_0 */
    { PLAYER_ANIMTYPE_2, PLAYER_MODELTYPE_LH_OPEN, PLAYER_MODELTYPE_RH_SHIELD, PLAYER_MODELTYPE_SHEATH_12,
      PLAYER_MODELTYPE_WAIST },
    /* PLAYER_MODELGROUP_1 */
    { PLAYER_ANIMTYPE_1, PLAYER_MODELTYPE_LH_ONE_HAND_SWORD, PLAYER_MODELTYPE_RH_CLOSED, PLAYER_MODELTYPE_SHEATH_15,
      PLAYER_MODELTYPE_WAIST },
    /* PLAYER_MODELGROUP_ONE_HAND_SWORD */
    { PLAYER_ANIMTYPE_1, PLAYER_MODELTYPE_LH_ONE_HAND_SWORD, PLAYER_MODELTYPE_RH_SHIELD, PLAYER_MODELTYPE_SHEATH_13,
      PLAYER_MODELTYPE_WAIST },
    /* PLAYER_MODELGROUP_DEFAULT */
    { PLAYER_ANIMTYPE_0, PLAYER_MODELTYPE_LH_OPEN, PLAYER_MODELTYPE_RH_OPEN, PLAYER_MODELTYPE_SHEATH_14,
      PLAYER_MODELTYPE_WAIST },
    /* PLAYER_MODELGROUP_4 */
    { PLAYER_ANIMTYPE_0, PLAYER_MODELTYPE_LH_OPEN, PLAYER_MODELTYPE_RH_OPEN, PLAYER_MODELTYPE_SHEATH_14,
      PLAYER_MODELTYPE_WAIST },
    /* PLAYER_MODELGROUP_TWO_HAND_SWORD */
    { PLAYER_ANIMTYPE_3, PLAYER_MODELTYPE_LH_TWO_HAND_SWORD, PLAYER_MODELTYPE_RH_CLOSED, PLAYER_MODELTYPE_SHEATH_14,
      PLAYER_MODELTYPE_WAIST },
    /* PLAYER_MODELGROUP_BOW */
    { PLAYER_ANIMTYPE_4, PLAYER_MODELTYPE_LH_CLOSED, PLAYER_MODELTYPE_RH_BOW, PLAYER_MODELTYPE_SHEATH_14,
      PLAYER_MODELTYPE_WAIST },
    /* PLAYER_MODELGROUP_EXPLOSIVES */
    { PLAYER_ANIMTYPE_5, PLAYER_MODELTYPE_LH_OPEN, PLAYER_MODELTYPE_RH_OPEN, PLAYER_MODELTYPE_SHEATH_14,
      PLAYER_MODELTYPE_WAIST },
    /* PLAYER_MODELGROUP_8 */
    { PLAYER_ANIMTYPE_0, PLAYER_MODELTYPE_LH_4, PLAYER_MODELTYPE_RH_OPEN, PLAYER_MODELTYPE_SHEATH_14,
      PLAYER_MODELTYPE_WAIST },
    /* PLAYER_MODELGROUP_HOOKSHOT */
    { PLAYER_ANIMTYPE_4, PLAYER_MODELTYPE_LH_OPEN, PLAYER_MODELTYPE_RH_HOOKSHOT, PLAYER_MODELTYPE_SHEATH_14,
      PLAYER_MODELTYPE_WAIST },
    /* PLAYER_MODELGROUP_STICK */
    { PLAYER_ANIMTYPE_3, PLAYER_MODELTYPE_LH_CLOSED, PLAYER_MODELTYPE_RH_CLOSED, PLAYER_MODELTYPE_SHEATH_14,
      PLAYER_MODELTYPE_WAIST },
    /* PLAYER_MODELGROUP_INSTRUMENT */
    { PLAYER_ANIMTYPE_0, PLAYER_MODELTYPE_LH_OPEN, PLAYER_MODELTYPE_RH_INSTRUMENT, PLAYER_MODELTYPE_SHEATH_14,
      PLAYER_MODELTYPE_WAIST },
    /* PLAYER_MODELGROUP_BOTTLE */
    { PLAYER_ANIMTYPE_0, PLAYER_MODELTYPE_LH_BOTTLE, PLAYER_MODELTYPE_RH_OPEN, PLAYER_MODELTYPE_SHEATH_14,
      PLAYER_MODELTYPE_WAIST },
    /* PLAYER_MODELGROUP_13 */
    { PLAYER_ANIMTYPE_0, PLAYER_MODELTYPE_LH_ONE_HAND_SWORD, PLAYER_MODELTYPE_RH_OPEN, PLAYER_MODELTYPE_SHEATH_15,
      PLAYER_MODELTYPE_WAIST },
    /* PLAYER_MODELGROUP_ZORA_FINS */
    { PLAYER_ANIMTYPE_0, PLAYER_MODELTYPE_LH_CLOSED, PLAYER_MODELTYPE_RH_CLOSED, PLAYER_MODELTYPE_SHEATH_14,
      PLAYER_MODELTYPE_WAIST },
};

/* DLists groups start */
// Note: Only the ones with size 2 * PLAYER_FORM_MAX

Gfx* gPlayerWaistDLs[2 * PLAYER_FORM_MAX] = {
    gLinkFierceDeityWaistDL, gLinkFierceDeityWaistDL, gLinkGoronWaistDL, gLinkGoronWaistDL, gLinkZoraWaistDL,
    gLinkZoraWaistDL,        gLinkDekuWaistDL,        gLinkDekuWaistDL,  gLinkHumanWaistDL, gLinkHumanWaistDL,
};

Gfx* gPlayerHandHoldingShields[PLAYER_SHIELD_MAX - 1][2] = {
    { object_link_child_DL_01DC28, object_link_child_DL_01DC28 },
    { object_link_child_DL_01DC48, object_link_child_DL_01DC48 },
};

Gfx* gPlayerSheath12DLs[2 * PLAYER_FORM_MAX] = {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    gLinkDekuSheathDL,
    gLinkDekuSheathDL,
    gLinkHumanSheathEmptyDL,
    gLinkHumanSheathEmptyDL,
};

Gfx* gPlayerSheath13DLs[2 * PLAYER_FORM_MAX] = {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    gLinkDekuSheathDL,
    gLinkDekuSheathDL,
    gLinkHumanSheathEmptyDL,
    gLinkHumanSheathEmptyDL,
};

Gfx* gPlayerSheath14DLs[2 * PLAYER_FORM_MAX] = {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    gLinkDekuSheathDL,
    gLinkDekuSheathDL,
    gLinkHumanSheathEmptyDL,
    gLinkHumanSheathEmptyDL,
};

Gfx* gPlayerShields[][2] = {
    { gLinkHumanHylianShieldWithMtxDL, gLinkHumanHylianShieldWithMtxDL },
    { gLinkHumanMirrorShieldWithMtxDL, gLinkHumanMirrorShieldWithMtxDL },
};

Gfx* gPlayerSheathedSwords[][2] = {
    { gLinkHumanSheathedKokiriSwordDL, gLinkHumanSheathedKokiriSwordDL },
    { gLinkHumanSheathedRazorSwordDL, gLinkHumanSheathedRazorSwordDL },
    { gLinkHumanSheathedGildedSwordDL, gLinkHumanSheathedGildedSwordDL },
};

Gfx* gPlayerSwordSheaths[][2] = {
    { gLinkHumanKokiriSwordSheathDL, gLinkHumanKokiriSwordSheathDL },
    { gLinkHumanRazorSwordSheathDL, gLinkHumanRazorSwordSheathDL },
    { gLinkHumanGildedSwordSheathDL, gLinkHumanGildedSwordSheathDL },
};

Gfx* gPlayerLeftHandTwoHandSwordDLs[2 * PLAYER_FORM_MAX] = {
    gLinkFierceDeityLeftHandHoldingSwordDL,
    gLinkFierceDeityLeftHandHoldingSwordDL,
    gLinkGoronLeftHandOpenDL,
    gLinkGoronLeftHandOpenDL,
    gLinkZoraLeftHandOpenDL,
    gLinkZoraLeftHandOpenDL,
    gLinkDekuLeftHandDL,
    gLinkDekuLeftHandDL,
    gLinkHumanLeftHandHoldingGreatFairysSwordDL,
    gLinkHumanLeftHandHoldingGreatFairysSwordDL,
};

Gfx* gPlayerLeftHandOpenDLs[2 * PLAYER_FORM_MAX] = {
    gLinkFierceDeityLeftHandDL, gLinkFierceDeityLeftHandDL, gLinkGoronLeftHandOpenDL, gLinkGoronLeftHandOpenDL,
    gLinkZoraLeftHandOpenDL,    gLinkZoraLeftHandOpenDL,    gLinkDekuLeftHandDL,      gLinkDekuLeftHandDL,
    gLinkHumanLeftHandOpenDL,   gLinkHumanLeftHandOpenDL,
};

Gfx* gPlayerLeftHandClosedDLs[2 * PLAYER_FORM_MAX] = {
    gLinkFierceDeityLeftHandDL, gLinkFierceDeityLeftHandDL, gLinkGoronLeftHandClosedDL, gLinkGoronLeftHandClosedDL,
    gLinkZoraLeftHandClosedDL,  gLinkZoraLeftHandClosedDL,  gLinkDekuLeftHandDL,        gLinkDekuLeftHandDL,
    gLinkHumanLeftHandClosedDL, gLinkHumanLeftHandClosedDL,
};

Gfx* gPlayerLeftHandOneHandSwordDLs[2 * PLAYER_FORM_MAX] = {
    gLinkFierceDeityLeftHandDL,
    gLinkFierceDeityLeftHandDL,
    gLinkGoronLeftHandOpenDL,
    gLinkGoronLeftHandOpenDL,
    gLinkZoraLeftHandClosedDL,
    gLinkZoraLeftHandClosedDL,
    gLinkDekuLeftHandDL,
    gLinkDekuLeftHandDL,
    gLinkHumanLeftHandHoldingKokiriSwordDL,
    gLinkHumanLeftHandHoldingKokiriSwordDL,
};

Gfx* D_801C018C[][2] = {
    { gLinkHumanLeftHandHoldingKokiriSwordDL, gLinkHumanLeftHandHoldingKokiriSwordDL },
    { gLinkHumanLeftHandHoldingRazorSwordDL, gLinkHumanLeftHandHoldingRazorSwordDL },
    { gLinkHumanLeftHandHoldingGildedSwordDL, gLinkHumanLeftHandHoldingGildedSwordDL },
};

Gfx* gPlayerRightHandOpenDLs[2 * PLAYER_FORM_MAX] = {
    gLinkFierceDeityRightHandDL, gLinkFierceDeityRightHandDL, gLinkGoronRightHandOpenDL, gLinkGoronRightHandOpenDL,
    gLinkZoraRightHandOpenDL,    gLinkZoraRightHandOpenDL,    gLinkDekuRightHandDL,      gLinkDekuRightHandDL,
    gLinkHumanRightHandOpenDL,   gLinkHumanRightHandOpenDL,
};

Gfx* gPlayerRightHandClosedDLs[2 * PLAYER_FORM_MAX] = {
    gLinkFierceDeityRightHandDL, gLinkFierceDeityRightHandDL, gLinkGoronRightHandClosedDL, gLinkGoronRightHandClosedDL,
    gLinkZoraRightHandClosedDL,  gLinkZoraRightHandClosedDL,  gLinkDekuRightHandDL,        gLinkDekuRightHandDL,
    gLinkHumanRightHandClosedDL, gLinkHumanRightHandClosedDL,
};

Gfx* gPlayerRightHandBowDLs[2 * PLAYER_FORM_MAX] = {
    gLinkFierceDeityRightHandDL,
    gLinkFierceDeityRightHandDL,
    gLinkGoronRightHandOpenDL,
    gLinkGoronRightHandOpenDL,
    gLinkZoraRightHandOpenDL,
    gLinkZoraRightHandOpenDL,
    gLinkDekuRightHandDL,
    gLinkDekuRightHandDL,
    gLinkHumanRightHandHoldingBowDL,
    gLinkHumanRightHandHoldingBowDL,
};

Gfx* gPlayerRightHandInstrumentDLs[2 * PLAYER_FORM_MAX] = {
    gLinkFierceDeityRightHandDL,
    gLinkFierceDeityRightHandDL,
    gLinkGoronRightHandOpenDL,
    gLinkGoronRightHandOpenDL,
    gLinkZoraRightHandOpenDL,
    gLinkZoraRightHandOpenDL,
    gLinkDekuRightHandDL,
    gLinkDekuRightHandDL,
    gLinkHumanRightHandHoldingOcarinaDL,
    gLinkHumanRightHandHoldingOcarinaDL,
};

Gfx* gPlayerRightHandHookshotDLs[2 * PLAYER_FORM_MAX] = {
    gLinkFierceDeityRightHandDL,
    gLinkFierceDeityRightHandDL,
    gLinkGoronRightHandOpenDL,
    gLinkGoronRightHandOpenDL,
    gLinkZoraRightHandOpenDL,
    gLinkZoraRightHandOpenDL,
    gLinkDekuRightHandDL,
    gLinkDekuRightHandDL,
    gLinkHumanRightHandHoldingHookshotDL,
    gLinkHumanRightHandHoldingHookshotDL,
};

Gfx* gPlayerLeftHandBottleDLs[2 * PLAYER_FORM_MAX] = {
    gLinkFierceDeityLeftHandDL,
    gLinkFierceDeityLeftHandDL,
    gLinkGoronLeftHandHoldBottleDL,
    gLinkGoronLeftHandHoldBottleDL,
    gLinkZoraLeftHandHoldBottleDL,
    gLinkZoraLeftHandHoldBottleDL,
    gLinkDekuLeftHandDL,
    gLinkDekuLeftHandDL,
    gLinkHumanLeftHandHoldBottleDL,
    gLinkHumanLeftHandHoldBottleDL,
};

/* DLists groups end */

Gfx* D_801C0294[PLAYER_FORM_MAX] = {
    gLinkFierceDeityLeftForearmDL, gLinkGoronLeftForearmDL, gLinkZoraLeftForearmDL,
    gLinkDekuLeftForearmDL,        gLinkHumanLeftForearmDL,
};

Gfx* D_801C02A8[PLAYER_FORM_MAX] = {
    gLinkFierceDeityLeftHandDL, gLinkGoronLeftHandOpenDL,   gLinkZoraLeftHandClosedDL,
    gLinkDekuLeftHandDL,        gLinkHumanLeftHandClosedDL,
};

Gfx* D_801C02BC[PLAYER_FORM_MAX] = {
    gLinkFierceDeityRightShoulderDL, gLinkGoronRightShoulderDL, gLinkZoraRightShoulderDL,
    gLinkDekuRightShoulderDL,        gLinkHumanRightShoulderDL,
};

Gfx* D_801C02D0[PLAYER_FORM_MAX] = {
    gLinkFierceDeityRightHandDL,
    0x060038C0, // This is in the middle of a texture in the link_goron object. It has the same offset as a link_nuts
                // dlist, maybe a typo?
    gLinkZoraRightHandOpenDL,
    gLinkDekuRightHandDL,
    object_link_child_DL_018490,
};

Gfx* D_801C02E4[PLAYER_FORM_MAX] = {
    gLinkFierceDeityRightHandDL,
    0x060038C0, // This is in the middle of a texture in the link_goron object. It has the same offset as a link_nuts
                // dlist, maybe a typo?
    gLinkZoraRightHandOpenDL,
    gLinkDekuRightHandDL,
    object_link_child_DL_017B40,
};

// Indexed by model types (left hand, right hand, sheath or waist)
Gfx** sPlayerDListGroups[PLAYER_MODELTYPE_MAX] = {
    gPlayerLeftHandOpenDLs,         // PLAYER_MODELTYPE_LH_OPEN
    gPlayerLeftHandClosedDLs,       // PLAYER_MODELTYPE_LH_CLOSED
    gPlayerLeftHandOneHandSwordDLs, // PLAYER_MODELTYPE_LH_ONE_HAND_SWORD
    gPlayerLeftHandTwoHandSwordDLs, // PLAYER_MODELTYPE_LH_TWO_HAND_SWORD
    gPlayerLeftHandOpenDLs,         // PLAYER_MODELTYPE_LH_4
    gPlayerLeftHandBottleDLs,       // PLAYER_MODELTYPE_LH_BOTTLE
    gPlayerRightHandOpenDLs,        // PLAYER_MODELTYPE_RH_OPEN
    gPlayerRightHandClosedDLs,      // PLAYER_MODELTYPE_RH_CLOSED
    gPlayerRightHandClosedDLs,      // PLAYER_MODELTYPE_RH_SHIELD
    gPlayerRightHandBowDLs,         // PLAYER_MODELTYPE_RH_BOW
    gPlayerRightHandInstrumentDLs,  // PLAYER_MODELTYPE_RH_INSTRUMENT
    gPlayerRightHandHookshotDLs,    // PLAYER_MODELTYPE_RH_HOOKSHOT
    gPlayerSheath12DLs,             // PLAYER_MODELTYPE_SHEATH_12
    gPlayerSheath13DLs,             // PLAYER_MODELTYPE_SHEATH_13
    gPlayerSheath14DLs,             // PLAYER_MODELTYPE_SHEATH_14
    gPlayerSheath14DLs,             // PLAYER_MODELTYPE_SHEATH_15
    gPlayerWaistDLs,                // PLAYER_MODELTYPE_WAIST
    NULL,                           // PLAYER_MODELTYPE_17
};

struct_80124618 D_801C0340[] = {
    { 0, { 0, 0, 0 } },       { 5, { 0, 0, 0 } },        { 7, { 100, 100, 100 } },
    { 9, { 110, 110, 110 } }, { 11, { 100, 100, 100 } },
};
struct_80124618 D_801C0368[] = {
    { 0, { 0, 0, 0 } },       { 4, { 0, 0, 0 } },      { 6, { 120, 150, 60 } },   { 8, { 130, 80, 160 } },
    { 9, { 100, 100, 100 } }, { 10, { 90, 100, 90 } }, { 11, { 100, 100, 100 } },
};
struct_80124618 D_801C03A0[] = {
    { 0, { 100, 100, 100 } },
    { 2, { 120, 120, 120 } },
    { 6, { 90, 90, 90 } },
    { 7, { 93, 93, 93 } },
};
struct_80124618 D_801C03C0[] = {
    { 0, { 200, 100, 110 } },
    { 2, { 90, 100, 100 } },
    { 3, { 100, 100, 100 } },
    { 7, { 100, 100, 100 } },
};
struct_80124618 D_801C03E0[] = {
    { 0, { 100, 100, 110 } }, { 2, { 60, 100, 80 } },    { 3, { 130, 105, 110 } },
    { 7, { 130, 105, 110 } }, { 10, { 100, 100, 100 } }, { 19, { 100, 100, 100 } },
};
struct_80124618 D_801C0410[] = {
    { 0, { 0, 0, 0 } },
    { 2, { 80, 110, 80 } },
    { 3, { 100, 100, 100 } },
};
struct_80124618 D_801C0428[] = {
    { 0, { 0, 0, 0 } },      { 6, { 0, 0, 0 } },        { 7, { 60, 60, 50 } },     { 8, { 120, 130, 100 } },
    { 9, { 100, 120, 80 } }, { 11, { 100, 100, 100 } }, { 13, { 100, 100, 100 } },
};
struct_80124618 D_801C0460[] = {
    { 0, { 100, 100, 100 } }, { 5, { 100, 100, 100 } }, { 7, { 100, 74, 80 } },
    { 8, { 100, 180, 130 } }, { 10, { 100, 80, 80 } },  { 13, { 100, 100, 100 } },
};
struct_80124618 D_801C0490[] = {
    { 0, { 100, 100, 100 } },  { 1, { 100, 100, 100 } },  { 2, { 90, 100, 105 } },  { 4, { 110, 100, 100 } },
    { 5, { 90, 100, 105 } },   { 6, { 100, 100, 100 } },  { 7, { 90, 100, 105 } },  { 8, { 100, 100, 100 } },
    { 9, { 90, 100, 105 } },   { 10, { 100, 100, 100 } }, { 11, { 90, 100, 105 } }, { 12, { 110, 100, 100 } },
    { 13, { 100, 100, 100 } }, { 14, { 90, 100, 105 } },  { 15, { 90, 100, 105 } }, { 17, { 100, 100, 100 } },
};
struct_80124618 D_801C0510[] = {
    { 0, { 100, 100, 100 } }, { 4, { 100, 100, 100 } }, { 5, { 90, 110, 100 } },
    { 6, { 110, 105, 100 } }, { 8, { 100, 100, 100 } },
};
struct_80124618 D_801C0538[] = {
    { 0, { 100, 100, 100 } }, { 5, { 100, 100, 100 } },  { 6, { 0, 0, 0 } },
    { 8, { 100, 100, 100 } }, { 14, { 100, 100, 100 } },
};
struct_80124618 D_801C0560[] = {
    { 0, { 100, 100, 100 } },
    { 2, { 95, 95, 100 } },
    { 3, { 105, 105, 100 } },
    { 5, { 102, 102, 102 } },
};
struct_80124618 D_801C0580[] = {
    { 0, { 100, 100, 100 } }, { 9, { 100, 100, 100 } }, { 10, { 150, 150, 150 } },
    { 12, { 0, 0, 0 } },      { 14, { 0, 0, 0 } },
};
struct_80124618 D_801C05A8[] = {
    { 0, { 100, 100, 100 } },
    { 6, { 100, 100, 100 } },
    { 7, { 0, 0, 0 } },
    { 17, { 0, 0, 0 } },
};
struct_80124618 D_801C05C8[] = {
    { 0, { 0, 0, 0 } },
    { 17, { 50, 50, 50 } },
};
struct_80124618 D_801C05D8[] = {
    { 0, { 0, 0, 0 } },
    { 5, { 0, 0, 0 } },
    { 9, { 100, 100, 100 } },
};
struct_80124618 D_801C05F0[] = {
    { 0, { 100, 100, 100 } },
    { 5, { 100, 100, 100 } },
    { 9, { 0, 0, 0 } },
};
struct_80124618 D_801C0608[] = {
    { 0, { 100, 100, 100 } },
    { 10, { 100, 100, 100 } },
    { 32, { 100, 115, 105 } },
    { 58, { 100, 101, 100 } },
};
struct_80124618 D_801C0628[] = {
    { 0, { 100, 100, 100 } },  { 5, { 100, 100, 100 } },  { 15, { 100, 100, 105 } }, { 25, { 100, 100, 105 } },
    { 34, { 100, 100, 100 } }, { 46, { 100, 100, 100 } }, { 57, { 100, 100, 105 } }, { 67, { 100, 100, 105 } },
    { 76, { 100, 100, 100 } }, { 78, { 100, 100, 100 } },
};
struct_80124618 D_801C0678[] = {
    { 0, { 0, 0, 0 } },
    { 6, { 0, 0, 0 } },
    { 8, { 100, 100, 100 } },
    { 11, { 100, 100, 100 } },
};
struct_80124618 D_801C0698[] = {
    { 0, { 0, 0, 0 } },
    { 7, { 1120, 112, 112 } },
    { 8, { 140, 168, 168 } },
    { 11, { 140, 140, 140 } },
};
struct_80124618 D_801C06B8[] = {
    { 0, { 100, 100, 100 } }, { 7, { 100, 100, 100 } }, { 8, { 70, 30, 70 } }, { 10, { 0, 0, 0 } }, { 14, { 0, 0, 0 } },
};
struct_80124618 D_801C06E0[] = {
    { 0, { 140, 140, 140 } },
    { 1, { 0, 0, 0 } },
    { 14, { 0, 0, 0 } },
};
struct_80124618 D_801C06F8[] = {
    { 0, { 100, 100, 100 } },
    { 5, { 100, 100, 100 } },
    { 6, { 0, 0, 0 } },
    { 10, { 0, 0, 0 } },
};
struct_80124618 D_801C0718[] = {
    { 0, { 140, 140, 140 } },
    { 1, { 0, 0, 0 } },
    { 10, { 0, 0, 0 } },
};
struct_80124618 D_801C0730[] = {
    { 0, { 100, 100, 100 } },
    { 13, { 100, 100, 100 } },
};
struct_80124618 D_801C0740[] = {
    { 0, { 140, 140, 140 } },
    { 13, { 140, 140, 140 } },
};
struct_80124618 D_801C0750[] = {
    { 0, { 100, 100, 100 } },  { 5, { 100, 100, 100 } },  { 6, { 200, 200, 200 } },
    { 10, { 200, 200, 200 } }, { 11, { 100, 100, 100 } },
};

// alpha values
u8 D_801C0778[] = {
    0, 0, 0, 0, 0, 100, 200, 255, 255, 255, 200, 100,
};

struct_80124618 D_801C0784[] = {
    { 0, { 100, 100, 100 } },  { 14, { 100, 100, 100 } }, { 15, { 200, 200, 200 } },
    { 16, { 200, 200, 200 } }, { 18, { 100, 100, 100 } },
};

u8 D_801C07AC[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 200, 255, 255, 255, 200, 100, 0,
};

void Player_SetModelsForHoldingShield(Player* player) {
    if (player->stateFlags1 & PLAYER_STATE1_400000) {
        if ((player->heldItemActionParam < 0) || (player->heldItemActionParam == player->itemActionParam)) {
            if (!Player_IsHoldingTwoHandedWeapon(player)) {
                if (!Player_IsGoronOrDeku(player)) {
                    D_801F59E0 = player->transformation * 2;
                    player->rightHandType = PLAYER_MODELTYPE_RH_SHIELD;
                    player->rightHandDLists = &sPlayerDListGroups[PLAYER_MODELTYPE_RH_SHIELD][D_801F59E0];

                    if (player->sheathType == PLAYER_MODELTYPE_SHEATH_14) {
                        player->sheathType = PLAYER_MODELTYPE_SHEATH_12;
                    } else if (player->sheathType == PLAYER_MODELTYPE_SHEATH_15) {
                        player->sheathType = PLAYER_MODELTYPE_SHEATH_13;
                    }

                    player->sheathDLists = &sPlayerDListGroups[player->sheathType][D_801F59E0];
                    player->modelAnimType = PLAYER_ANIMTYPE_2;
                    player->heldItemActionParam = -1;
                }
            }
        }
    }
}

void Player_SetModels(Player* player, PlayerModelGroup modelGroup) {
    PlayerModelIndices* playerModelTypes;

    D_801F59E0 = player->transformation * 2;
    player->leftHandType = gPlayerModelTypes[modelGroup].leftHandType;
    player->rightHandType = gPlayerModelTypes[modelGroup].rightHandType;
    player->sheathType = gPlayerModelTypes[modelGroup].sheathType;

    if (player->sheathType == PLAYER_MODELTYPE_SHEATH_14) {
        if (CUR_FORM_EQUIP(EQUIP_SLOT_B) == ITEM_NONE) {
            player->sheathType = PLAYER_MODELTYPE_SHEATH_15;
        }
    }

    playerModelTypes = &gPlayerModelTypes[modelGroup];

    player->leftHandDLists = &sPlayerDListGroups[playerModelTypes->leftHandType][D_801F59E0];
    player->rightHandDLists = &sPlayerDListGroups[playerModelTypes->rightHandType][D_801F59E0];
    player->sheathDLists = &sPlayerDListGroups[playerModelTypes->sheathType][D_801F59E0];
    player->waistDLists = &sPlayerDListGroups[playerModelTypes->waistType][D_801F59E0];

    Player_SetModelsForHoldingShield(player);
}

void Player_SetModelGroup(Player* player, PlayerModelGroup modelGroup) {
    player->modelGroup = modelGroup;

    if (modelGroup == PLAYER_MODELGROUP_1) {
        player->modelAnimType = PLAYER_ANIMTYPE_0;
    } else {
        player->modelAnimType = gPlayerModelTypes[modelGroup].modelAnimType;
    }

    if (player->modelAnimType < PLAYER_ANIMTYPE_3) {
        if (((player->transformation != PLAYER_FORM_FIERCE_DEITY) && (player->transformation != PLAYER_FORM_HUMAN)) ||
            (player->currentShield == PLAYER_SHIELD_NONE)) {
            player->modelAnimType = PLAYER_ANIMTYPE_0;
        }
    }

    Player_SetModels(player, modelGroup);
}

void func_80123C58(Player* player) {
    player->heldItemActionParam = player->itemActionParam;
    Player_SetModelGroup(player, Player_ActionToModelGroup(player, player->itemActionParam));
    player->unk_AA5 = 0;
}

void Player_SetEquipmentData(PlayState* play, Player* player) {
    if (player->csMode != PLAYER_CSMODE_134) {
        player->currentShield = GET_CUR_EQUIP_VALUE(EQUIP_TYPE_SHIELD);
        if ((player->transformation != PLAYER_FORM_ZORA) || (((player->currentBoots != PLAYER_BOOTS_ZORA_LAND)) &&
                                                             (player->currentBoots != PLAYER_BOOTS_ZORA_UNDERWATER))) {
            player->currentBoots = D_801BFF90[player->transformation];
        }
        Player_SetModelGroup(player, Player_ActionToModelGroup(player, player->itemActionParam));
        func_80123140(play, player);
        if (player->unk_B62 != 0) {
            player->unk_B62 = 1;
        }
    }
}

void Player_UpdateBottleHeld(PlayState* play, Player* player, ItemID itemId, PlayerActionParam actionParam) {
    Inventory_UpdateBottleItem(play, itemId, player->heldItemButton);

    if (itemId != ITEM_BOTTLE) {
        player->heldItemId = itemId;
        player->itemActionParam = actionParam;
    }

    player->heldItemActionParam = actionParam;
}

// Player_Untarget / Player_StopTargeting?
void func_80123DA4(Player* player) {
    player->targetedActor = NULL;
    player->stateFlags2 &= ~PLAYER_STATE2_2000;
}

void func_80123DC0(Player* player) {
    if ((player->actor.bgCheckFlags & 1) ||
        (player->stateFlags1 & (PLAYER_STATE1_200000 | PLAYER_STATE1_800000 | PLAYER_STATE1_8000000)) ||
        (!(player->stateFlags1 & (PLAYER_STATE1_40000 | PLAYER_STATE1_80000)) &&
         ((player->actor.world.pos.y - player->actor.floorHeight) < 100.0f))) {
        player->stateFlags1 &= ~(PLAYER_STATE1_8000 | PLAYER_STATE1_10000 | PLAYER_STATE1_20000 | PLAYER_STATE1_40000 |
                                 PLAYER_STATE1_80000 | PLAYER_STATE1_40000000);
    } else if (!(player->stateFlags1 & (PLAYER_STATE1_40000 | PLAYER_STATE1_80000 | PLAYER_STATE1_200000))) {
        player->stateFlags1 |= PLAYER_STATE1_80000;
    } else if ((player->stateFlags1 & PLAYER_STATE1_40000) && (player->transformation == PLAYER_FORM_DEKU)) {
        player->stateFlags1 &=
            ~(PLAYER_STATE1_8000 | PLAYER_STATE1_10000 | PLAYER_STATE1_20000 | PLAYER_STATE1_40000000);
    }

    func_80123DA4(player);
}

void func_80123E90(PlayState* play, Actor* actor) {
    Player* player = GET_PLAYER(play);

    func_80123DC0(player);
    player->targetedActor = actor;
    player->unk_A78 = actor;
    player->stateFlags1 |= PLAYER_STATE1_10000;
    Camera_SetViewParam(Play_GetCamera(play, CAM_ID_MAIN), CAM_VIEW_TARGET, actor);
    Camera_ChangeMode(Play_GetCamera(play, CAM_ID_MAIN), CAM_MODE_FOLLOWTARGET);
}

s32 func_80123F14(PlayState* play) {
    Player* player = GET_PLAYER(play);

    return player->stateFlags1 & PLAYER_STATE1_800000;
}

s32 func_80123F2C(PlayState* play, s32 ammo) {
    play->unk_1887C = ammo + 1;

    return 1;
}

s32 Player_IsBurningStickInRange(PlayState* play, Vec3f* pos, f32 xzRange, f32 yRange) {
    Player* this = GET_PLAYER(play);
    Vec3f diff;
    s32 pad;

    if ((this->itemActionParam == PLAYER_AP_STICK) && (this->unk_B28 != 0)) {
        Math_Vec3f_Diff(&this->meleeWeaponInfo[0].tip, pos, &diff);
        return (SQXZ(diff) <= SQ(xzRange)) && (0.0f <= diff.y) && (diff.y <= yRange);
    }

    return false;
}

u8 Player_GetStrength(void) {
    return sPlayerStrengths[GET_PLAYER_FORM];
}

u8 Player_GetMask(PlayState* play) {
    Player* player = GET_PLAYER(play);

    return player->currentMask;
}

// Unused
void Player_RemoveMask(PlayState* play) {
    Player* player = GET_PLAYER(play);

    player->currentMask = PLAYER_MASK_NONE;
}

s32 Player_HasMirrorShieldEquipped(PlayState* play) {
    Player* player = GET_PLAYER(play);

    return (player->transformation == PLAYER_FORM_HUMAN) && (player->currentShield == PLAYER_SHIELD_MIRROR_SHIELD);
}

s32 Player_IsHoldingMirrorShield(PlayState* play) {
    Player* player = GET_PLAYER(play);

    return (player->transformation == PLAYER_FORM_HUMAN) && (player->rightHandType == PLAYER_MODELTYPE_RH_SHIELD) &&
           (player->currentShield == PLAYER_SHIELD_MIRROR_SHIELD);
}

s32 Player_IsHoldingHookshot(Player* player) {
    return player->itemActionParam == PLAYER_AP_HOOKSHOT;
}

s32 func_801240DC(Player* player) {
    return Player_IsHoldingHookshot(player) && (player->heldActor == NULL);
}

s32 func_80124110(Player* player, PlayerActionParam actionParam) {
    s32 temp_v0 = actionParam - PLAYER_AP_FISHING_POLE;

    if (player->transformation != PLAYER_FORM_GORON) {
        if (((actionParam - PLAYER_AP_FISHING_POLE) > (PLAYER_AP_FISHING_POLE - PLAYER_AP_FISHING_POLE)) &&
            ((actionParam - PLAYER_AP_FISHING_POLE) < (PLAYER_AP_SWORD_GREAT_FAIRY - PLAYER_AP_FISHING_POLE))) {
            return temp_v0;
        }
    }

    return 0;
}

s32 func_80124148(Player* player) {
    return func_80124110(player, player->itemActionParam);
}

s32 Player_ActionToMeleeWeapon(PlayerActionParam actionParam) {
    s32 weapon = actionParam - (PLAYER_AP_SWORD_KOKIRI - 1);

    if ((weapon > 0) && (weapon <= (PLAYER_AP_ZORA_FINS - (PLAYER_AP_SWORD_KOKIRI - 1)))) {
        return weapon;
    }
    return 0;
}

s32 Player_GetMeleeWeaponHeld(Player* player) {
    return Player_ActionToMeleeWeapon(player->itemActionParam);
}

s32 Player_IsHoldingTwoHandedWeapon(Player* player) {
    // Relies on the actionParams for two-handed weapons being contiguous.
    if ((player->itemActionParam >= PLAYER_AP_SWORD_GREAT_FAIRY) && (player->itemActionParam <= PLAYER_AP_STICK)) {
        return true;
    }

    return false;
}

s32 Player_ActionToBottle(Player* player, PlayerActionParam actionParam) {
    s32 bottle = actionParam - PLAYER_AP_BOTTLE;

    // Relies on bottle-related action params to be contiguous
    if ((bottle >= (PLAYER_AP_BOTTLE - PLAYER_AP_BOTTLE)) && (bottle <= (PLAYER_AP_BOTTLE_FAIRY - PLAYER_AP_BOTTLE))) {
        return bottle;
    }

    return -1;
}

s32 Player_GetBottleHeld(Player* Player) {
    return Player_ActionToBottle(Player, Player->itemActionParam);
}

s32 Player_ActionToExplosive(Player* player, PlayerActionParam actionParam) {
    s32 explosive = actionParam - PLAYER_AP_BOMB;

    // Relies on explosive-related action params to be contiguous
    if ((explosive >= (PLAYER_AP_BOMB - PLAYER_AP_BOMB)) && (explosive <= (PLAYER_AP_BOMBCHU - PLAYER_AP_BOMB))) {
        return explosive;
    }

    return -1;
}

s32 Player_GetExplosiveHeld(Player* player) {
    return Player_ActionToExplosive(player, player->itemActionParam);
}

s32 Player_ActionToSword(Actor* actor, PlayerActionParam actionParam) {
    s32 sword = 0;

    //! FAKE:
    if ((actionParam == PLAYER_AP_LAST_USED) ||
        ((sword = actionParam - PLAYER_AP_SWORD_KOKIRI, (sword >= PLAYER_AP_SWORD_KOKIRI - PLAYER_AP_SWORD_KOKIRI)) &&
         (sword <= PLAYER_AP_SWORD_GREAT_FAIRY - PLAYER_AP_SWORD_KOKIRI))) {
        return sword;
    }

    return -1;
}

s32 func_801242B4(Player* player) {
    return (player->stateFlags1 & PLAYER_STATE1_8000000) && player->currentBoots < PLAYER_BOOTS_ZORA_UNDERWATER;
}

s32 func_801242DC(PlayState* play) {
    Player* player = GET_PLAYER(play);
    TextTriggerEntry* triggerEntry;
    s32 envIndex;

    if (play->roomCtx.currRoom.unk2 == 3) { // Room is hot
        envIndex = 0;
    } else if ((player->transformation != PLAYER_FORM_ZORA) && (player->unk_AD8 > 80)) {
        envIndex = 3;
    } else if (player->stateFlags1 & PLAYER_STATE1_8000000) {
        if ((player->transformation == PLAYER_FORM_ZORA) && (player->currentBoots >= PLAYER_BOOTS_ZORA_UNDERWATER) &&
            (player->actor.bgCheckFlags & 1)) {
            envIndex = 1;
        } else {
            envIndex = 2;
        }
    } else {
        return 0;
    }

    // Trigger general textboxes under certain conditions, like "It's so hot in here!". Unused in MM
    triggerEntry = &sEnvironmentTextTriggers[envIndex];
    if (!Player_InCsMode(play)) {
        if ((triggerEntry->flag) && !(gSaveContext.textTriggerFlags & triggerEntry->flag) && (envIndex == 0)) {
            Message_StartTextbox(play, triggerEntry->textId, NULL);
            gSaveContext.textTriggerFlags |= triggerEntry->flag;
        }
    }

    return envIndex + 1;
}

#ifdef NON_MATCHING
void func_80124420(Player* player) {
    f32 pad;
    s16 sp2C;
    s16 sp28;
    s16 sp26;

    D_801F59D0.unk_6 -= D_801F59D0.unk_6 >> 3;
    D_801F59D0.unk_8 -= D_801F59D0.unk_8 >> 3;
    D_801F59D0.unk_6 += (-D_801F59D0.unk_0) >> 2;
    D_801F59D0.unk_8 += (-D_801F59D0.unk_2) >> 2;

    sp26 = BINANG_SUB(player->actor.world.rot.y, player->actor.shape.rot.y);
    sp28 = (player->actor.speedXZ * -200.0f * Math_CosS(sp26)) * (randPlusMinusPoint5Scaled(2.0f) + 10.0f);
    sp2C = (player->actor.speedXZ * 100.0f * Math_SinS(sp26)) * (randPlusMinusPoint5Scaled(2.0f) + 10.0f);

    D_801F59D0.unk_6 += sp28 >> 2;
    D_801F59D0.unk_8 += sp2C >> 2;

    if (D_801F59D0.unk_6 > 0x1770) {
        D_801F59D0.unk_6 = 0x1770;
    } else if (D_801F59D0.unk_6 < -0x1770) {
        D_801F59D0.unk_6 = -0x1770;
    }

    if (D_801F59D0.unk_8 > 0x1770) {
        D_801F59D0.unk_8 = 0x1770;
    } else if (D_801F59D0.unk_8 < -0x1770) {
        D_801F59D0.unk_8 = -0x1770;
    }

    D_801F59D0.unk_0 += D_801F59D0.unk_6;
    D_801F59D0.unk_2 += D_801F59D0.unk_8;

    if (D_801F59D0.unk_0 < 0) {
        D_801F59D0.unk_4 = D_801F59D0.unk_0 >> 1;
    } else {
        D_801F59D0.unk_4 = 0;
    }
}
#else
void func_80124420(Player* player);
#pragma GLOBAL_ASM("asm/non_matchings/code/z_player_lib/func_80124420.s")
#endif

void func_80124618(struct_80124618 arg0[], f32 curFrame, Vec3f* arg2) {
    s32 currentFrame = curFrame;
    f32 temp_f0;
    f32 temp_f14;
    f32 progress;
    s16 temp_v1;

    do {
        temp_v1 = arg0[1].unk_0;
        arg0++;
    } while (temp_v1 < currentFrame);

    temp_f0 = arg0[-1].unk_0;
    progress = (curFrame - temp_f0) / (temp_v1 - temp_f0);

    temp_f14 = arg0[-1].unk_2.x;
    arg2->x = LERPIMP(temp_f14, arg0->unk_2.x, progress) * 0.01f;

    temp_f14 = arg0[-1].unk_2.y;
    arg2->y = LERPIMP(temp_f14, arg0->unk_2.y, progress) * 0.01f;

    temp_f14 = arg0[-1].unk_2.z;
    arg2->z = LERPIMP(temp_f14, arg0->unk_2.z, progress) * 0.01f;
}

struct_80124618 D_801C07C0[] = {
    { 0, { 100, 100, 100 } },  { 10, { 100, 100, 100 } }, { 11, { 100, 120, 120 } },
    { 12, { 100, 120, 120 } }, { 14, { 100, 100, 100 } }, { 19, { 100, 100, 100 } },
};

struct_80124618 D_801C07F0[] = {
    { 0, { 40, 60, 70 } },    { 3, { 40, 60, 70 } },    { 4, { 75, 90, 85 } },
    { 5, { 110, 120, 100 } }, { 7, { 100, 100, 100 } }, { 8, { 100, 100, 100 } },
};
struct_80124618 D_801C0820[] = {
    { 0, { 100, 100, 100 } },
    { 7, { 100, 100, 100 } },
    { 12, { 40, 60, 70 } },
};
struct_80124618 D_801C0838[] = {
    { 0, { 40, 60, 70 } },
    { 7, { 40, 60, 70 } },
    { 12, { 100, 100, 100 } },
};

Gfx gCullBackDList[] = {
    gsSPSetGeometryMode(G_CULL_BACK),
    gsSPEndDisplayList(),
};
Gfx gCullFrontDList[] = {
    gsSPSetGeometryMode(G_CULL_FRONT),
    gsSPEndDisplayList(),
};

TexturePtr sPlayerEyesTextures[] = {
    gLinkHumanEyesOpenTex,     gLinkHumanEyesHalfTex,   gLinkHumanEyesClosedTex,   gLinkHumanEyesRollRightTex,
    gLinkHumanEyesRollLeftTex, gLinkHumanEyesRollUpTex, gLinkHumanEyesRollDownTex, object_link_child_Tex_003800,
};

TexturePtr sPlayerMouthTextures[] = {
    gLinkHumanMouthClosedTex,
    gLinkHumanMouthTeethTex,
    gLinkHumanMouthAngryTex,
    gLinkHumanMouthHappyTex,
};

typedef struct PlayerFaceIndices {
    /* 0x00 */ u8 eyeIndex;
    /* 0x01 */ u8 mouthIndex;
} PlayerFaceIndices; // size = 0x02

PlayerFaceIndices sPlayerFaces[] = {
    { PLAYER_EYES_OPEN, PLAYER_MOUTH_CLOSED },       // PLAYER_FACE_0
    { PLAYER_EYES_HALF, PLAYER_MOUTH_CLOSED },       // PLAYER_FACE_1
    { PLAYER_EYES_CLOSED, PLAYER_MOUTH_CLOSED },     // PLAYER_FACE_2
    { PLAYER_EYES_OPEN, PLAYER_MOUTH_CLOSED },       // PLAYER_FACE_3
    { PLAYER_EYES_HALF, PLAYER_MOUTH_CLOSED },       // PLAYER_FACE_4
    { PLAYER_EYES_CLOSED, PLAYER_MOUTH_CLOSED },     // PLAYER_FACE_5
    { PLAYER_EYES_ROLL_LEFT, PLAYER_MOUTH_CLOSED },  // PLAYER_FACE_6
    { PLAYER_EYES_ROLL_UP, PLAYER_MOUTH_TEETH },     // PLAYER_FACE_7
    { PLAYER_EYES_7, PLAYER_MOUTH_ANGRY },           // PLAYER_FACE_8
    { PLAYER_EYES_OPEN, PLAYER_MOUTH_ANGRY },        // PLAYER_FACE_9
    { PLAYER_EYES_ROLL_RIGHT, PLAYER_MOUTH_CLOSED }, // PLAYER_FACE_10
    { PLAYER_EYES_ROLL_LEFT, PLAYER_MOUTH_CLOSED },  // PLAYER_FACE_11
    { PLAYER_EYES_CLOSED, PLAYER_MOUTH_ANGRY },      // PLAYER_FACE_12
    { PLAYER_EYES_HALF, PLAYER_MOUTH_TEETH },        // PLAYER_FACE_13
    { PLAYER_EYES_OPEN, PLAYER_MOUTH_ANGRY },        // PLAYER_FACE_14
    { PLAYER_EYES_OPEN, PLAYER_MOUTH_HAPPY },        // PLAYER_FACE_15
};

void Player_DrawImpl(PlayState* play, void** skeleton, Vec3s* jointTable, s32 dListCount, s32 lod,
                     PlayerTransformation playerForm, s32 boots, s32 face, OverrideLimbDrawFlex overrideLimbDraw,
                     PostLimbDrawFlex postLimbDraw, Actor* actor) {
    s32 eyeIndex = (jointTable[22].x & 0xF) - 1;
    s32 mouthIndex = ((jointTable[22].x >> 4) & 0xF) - 1;
    Gfx* gfx;

    OPEN_DISPS(play->state.gfxCtx);

    gfx = POLY_OPA_DISP;

    if (eyeIndex < 0) {
        eyeIndex = sPlayerFaces[face].eyeIndex;
    }

    if (playerForm == PLAYER_FORM_GORON) {
        if ((eyeIndex >= PLAYER_EYES_ROLL_RIGHT) && (eyeIndex <= PLAYER_EYES_ROLL_DOWN)) {
            eyeIndex = PLAYER_EYES_OPEN;
        } else if (eyeIndex == PLAYER_EYES_7) {
            eyeIndex = PLAYER_EYES_ROLL_RIGHT;
        }
    }

    gSPSegment(&gfx[0], 0x08, Lib_SegmentedToVirtual(sPlayerEyesTextures[eyeIndex]));

    if (mouthIndex < 0) {
        mouthIndex = sPlayerFaces[face].mouthIndex;
    }

    gSPSegment(&gfx[1], 0x09, Lib_SegmentedToVirtual(sPlayerMouthTextures[mouthIndex]));

    POLY_OPA_DISP = &gfx[2];

    D_801F59E0 = playerForm * 2;
    D_801F59E4 = lod;
    SkelAnime_DrawFlexLod(play, skeleton, jointTable, dListCount, overrideLimbDraw, postLimbDraw, actor, lod);

    CLOSE_DISPS(play->state.gfxCtx);
}

Vec3f D_801C08C0[PLAYER_FORM_MAX] = {
    { 1304.0f, 0.0f, 0.0f }, { 1156.0f, 0.0f, 0.0f }, { 1406.0f, 0.0f, 0.0f },
    { 408.0f, 0.0f, 0.0f },  { 695.0f, 0.0f, 0.0f },
};

f32 D_801C08FC[PLAYER_FORM_MAX] = {
    1265.0f, 1056.0f, 1506.0f, 359.0f, 826.0f,
};
f32 D_801C0910[PLAYER_FORM_MAX] = {
    170.0416f, 133.63359f, 197.68358f, 16.646399f, 48.302498f,
};
f32 D_801C0924[PLAYER_FORM_MAX] = {
    10.019104f, 22.120003f, -29.12001f, 3.7582989f, -19.925102f,
};
f32 D_801C0938[PLAYER_FORM_MAX] = {
    5.0f, 4.0f, 1.0f, 1.0f, 3.0f,
};

void func_80124870(PlayState* play, Player* player, SkelAnime* skelAnime, Vec3f* pos, Vec3s* rot, s32 arg5, s32 arg6,
                   s32 arg7) {
    CollisionPoly* spA4;
    s32 spA0;
    f32 sp9C;
    Vec3f sp90;
    Vec3f sp84;
    Vec3f footprintPos;
    f32 sp74;
    f32 sp70;
    f32 temp_f18;
    f32 sp68;
    f32 temp_f16;
    f32 sp60;
    f32 temp_f0;
    f32 sp58;
    f32 sp54;
    f32 temp_f20;
    f32 sp4C;
    f32 sp48;
    s32 temp_f8;
    s32 temp_v0_4;
    s16 phi_t1;

    if ((player->stateFlags3 & PLAYER_STATE3_1) || !(player->actor.scale.y >= 0.0f) ||
        (player->stateFlags1 & PLAYER_STATE1_80) || (play->unk_18844 != 0)) {
        return;
    }

    sp74 = D_801C0910[player->transformation];
    sp70 = D_801C0924[player->transformation];
    temp_f20 = D_801C0938[player->transformation] - player->unk_AB8;
    Matrix_Push();
    Matrix_TranslateRotateZYX(pos, rot);
    Matrix_MultZero(&sp90);
    Matrix_TranslateRotateZYX(&D_801C08C0[player->transformation], &skelAnime->jointTable[arg6]);
    Matrix_Translate(D_801C08FC[player->transformation], 0.0f, 0.0f, 1);
    Matrix_MultZero(&sp84);
    Matrix_MultVecY(-300.0f, &footprintPos);
    Matrix_Pop();

    footprintPos.y += 15.0f;
    sp9C = BgCheck_EntityRaycastFloor3(&play->colCtx, &spA4, &spA0, &footprintPos) + temp_f20 +
           (player->unk_ABC * player->actor.scale.y);
    if (sp84.y < sp9C) {
        sp68 = sp84.x - sp90.x;
        temp_f16 = sp84.y - sp90.y;
        sp60 = sp84.z - sp90.z;
        temp_f0 = sqrtf(SQ(sp68) + SQ(temp_f16) + SQ(sp60));

        sp58 = (SQ(temp_f0) + sp70) / (2.0f * temp_f0);

        temp_f20 = sp74 - SQ(sp58);
        if (sp74 < SQ(sp58)) {
            temp_f20 = 0.0f;
        } else {
            temp_f20 = sqrtf(temp_f20);
        }
        sp4C = func_80086B30(temp_f20, sp58);
        temp_f0 = sqrtf(SQ(sp68) + SQ(sp9C - sp90.y) + SQ(sp60));
        sp58 = (SQ(temp_f0) + sp70) / (2.0f * temp_f0);
        sp54 = temp_f0 - sp58;

        temp_f20 = sp74 - SQ(sp58);
        if (sp74 < SQ(sp58)) {
            temp_f20 = 0.0f;
        } else {
            temp_f20 = sqrtf(temp_f20);
        }

        sp48 = func_80086B30(temp_f20, sp58);
        phi_t1 = (3.1415927f - (func_80086B30(sp54, temp_f20) + (1.5707964f - sp48))) * 10430.378f;
        phi_t1 = -skelAnime->jointTable[arg6].z + phi_t1;
        temp_f8 = (s32)((sp48 - sp4C) * 10430.378f);

        if ((s16)(ABS_ALT(skelAnime->jointTable[arg6].x) + ABS_ALT(skelAnime->jointTable[arg6].y)) < 0) {
            phi_t1 = BINANG_ROT180(phi_t1);
        }

        rot->z -= (s16)temp_f8;

        skelAnime->jointTable[arg5].z -= (s16)temp_f8;
        skelAnime->jointTable[arg6].z += (s16)phi_t1;
        skelAnime->jointTable[arg7].z = (skelAnime->jointTable[arg7].z + (s16)temp_f8) - phi_t1;

        temp_v0_4 = func_800C99D4(&play->colCtx, spA4, spA0);
        if ((temp_v0_4 >= 2) && (temp_v0_4 < 4) && (SurfaceType_IsWallDamage(&play->colCtx, spA4, spA0) == 0)) {
            footprintPos.y = sp9C;
            EffectSsGFire_Spawn(play, &footprintPos);
        }
    }
}

void func_80124CC4(PlayState* play, Player* player, f32 arg2) {
    static Vec3f D_801C094C = { -500.0f, -100.0f, 0.0f };
    CollisionPoly* poly;
    s32 bgId;
    Vec3f sp7C;
    Vec3f sp70;
    Vec3f sp64;
    Vec3f sp58;
    f32 sp54;
    f32 sp50;

    D_801C094C.z = 0.0f;
    Matrix_MultVec3f(&D_801C094C, &sp7C);
    D_801C094C.z = arg2;
    Matrix_MultVec3f(&D_801C094C, &sp70);

    if (BgCheck_AnyLineTest3(&play->colCtx, &sp7C, &sp70, &sp64, &poly, 1, 1, 1, 1, &bgId)) {
        if (!func_800B90AC(play, &player->actor, poly, bgId, &sp64) ||
            BgCheck_ProjectileLineTest(&play->colCtx, &sp7C, &sp70, &sp64, &poly, 1, 1, 1, 1, &bgId)) {
            OPEN_DISPS(play->state.gfxCtx);

            OVERLAY_DISP = Gfx_CallSetupDL(OVERLAY_DISP, 7);

            SkinMatrix_Vec3fMtxFMultXYZW(&play->viewProjectionMtxF, &sp64, &sp58, &sp54);
            if (sp54 < 200.0f) {
                sp50 = 0.08f;
            } else {
                sp50 = (sp54 / 200.0f) * 0.08f;
            }
            Matrix_Translate(sp64.x, sp64.y, sp64.z, MTXMODE_NEW);
            Matrix_Scale(sp50, sp50, sp50, MTXMODE_APPLY);

            gSPMatrix(OVERLAY_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

            gSPSegment(OVERLAY_DISP++, 0x06, play->objectCtx.status[player->actor.objBankIndex].segment);
            gSPDisplayList(OVERLAY_DISP++, gameplay_keep_DL_04F250);

            CLOSE_DISPS(play->state.gfxCtx);
        }
    }
}

s32 D_801C0958 = false;

Gfx** D_801C095C[] = {
    gPlayerLeftHandClosedDLs,
    gPlayerLeftHandOpenDLs,
};
Gfx** D_801C0964[] = {
    gPlayerRightHandClosedDLs,
    gPlayerRightHandOpenDLs,
};

void func_80124F18(s16* arg0, f32* arg1, s16 arg2, f32 arg3, f32 arg4) {
    f32 phi_f12;

    if (*arg0 < arg2) {
        *arg1 += arg3;
    } else {
        *arg1 -= arg3;
    }

    *arg1 = CLAMP(*arg1, -arg4, arg4);
    *arg0 += (s16)*arg1;
    if (((arg2 - *arg0) * (s16)*arg1) < 0) {
        *arg0 = arg2;
    }
}

void func_80124FF0(f32 arg0, s16 arg1, Vec3f* arg2, s16 arg3, Vec3f* arg4, Vec3f* arg5, s16* arg6, f32* arg7, f32 arg8,
                   s16 arg9, s16* arg10, f32* arg11, s32 arg12) {
    Vec3f sp44;
    f32 sp40;
    s16 temp_v0;
    s16 sp3C;
    f32 temp_f14;
    f32 sp34 = Math_CosS(arg1) * arg0;
    f32 sp30 = Math_SinS(arg1) * -arg0;

    arg5->x = (Math_SinS(arg3) * sp30) + arg2->x;
    arg5->y = arg2->y + sp34;
    arg5->z = (Math_CosS(arg3) * sp30) + arg2->z;

    Math_Vec3f_Diff(arg5, arg4, &sp44);
    sp40 = sqrtf(SQXZ(sp44));

    sp3C = (sp40 <= 1.0f) ? arg3 : Math_FAtan2F(sp44.z, sp44.x);
    sp40 = (Math_CosS(sp3C - arg3) * sp40) + arg8;

    if (ABS_ALT(BINANG_SUB(sp3C, arg3)) > 0x4000) {
        sp3C = BINANG_ROT180(sp3C);
    }
    sp3C -= arg3;

    temp_v0 = Math_FAtan2F(sp44.y, sp40);
    temp_v0 = CLAMP(temp_v0, (s16)-arg9, arg9);
    //! FAKE:
    if (sp3C) {}

    func_80124F18(arg6, arg7, temp_v0, 20.0f, 2000.0f);
}

void func_801251C4(Player* player, Vec3f* arg1) {
    Vec3f sp4C;
    Vec3f sp40;

    sp4C.x = player->actor.world.pos.x;
    sp4C.y = player->actor.world.pos.y + 60.0f;
    sp4C.z = player->actor.world.pos.z;
    func_80124FF0(-20.0f, player->unk_B8C, &sp4C, player->actor.shape.rot.y, player->bodyPartsPos, arg1,
                  &player->unk_B90, &player->unk_B10[0], 0.0f, 0x1F40, &player->unk_B92, &player->unk_B10[1], 0);

    sp40.x = (player->bodyPartsPos[6].x + player->bodyPartsPos[3].x) * 0.5f;
    sp40.y = (player->bodyPartsPos[6].y + player->bodyPartsPos[3].y) * 0.5f;
    sp40.z = (player->bodyPartsPos[6].z + player->bodyPartsPos[3].z) * 0.5f;
    func_80124FF0(-20.0f, player->unk_B90, arg1, (player->actor.shape.rot.y + player->unk_B92), &sp40, arg1,
                  &player->unk_B94, &player->unk_B10[2], -1.9f, 0x1F40, &player->unk_B96, &player->unk_B10[3], 0);
}

void func_80125318(Vec3f* arg0, Vec3s* arg1) {
    arg0->x = 0.0f;
    arg0->y = 0.0f;
    arg0->z = 0.0f;
    arg1->x = 0;
    arg1->y = 0;
    arg1->z = 0;
}

void func_80125340(void) {
    Matrix_Push();
    D_801C0958 = true;
}

void func_8012536C(void) {
    if (D_801C0958) {
        Matrix_Pop();
        D_801C0958 = false;
    }
}

void Player_DrawZoraShield(PlayState* play, Player* player) {
    u8* phi_a0;
    Vtx* vtx;
    Gfx* dList;
    f32 scale;
    s32 i;

    scale = player->unk_B62 * (10.0f / 51.0f);

    OPEN_DISPS(play->state.gfxCtx);

    AnimatedMat_DrawXlu(play, Lib_SegmentedToVirtual(&object_link_zora_Matanimheader_012A80));
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);

    // clang-format off
    vtx = Lib_SegmentedToVirtual(&object_link_zora_Vtx_011210); phi_a0 = Lib_SegmentedToVirtual(&object_link_zora_U8_011710);
    // clang-format on

    // ARRAY_COUNT(object_link_zora_Vtx_011210)
    for (i = 0; i < 80; i++) {
        // Editing the Vtxs in object itself
        vtx->v.cn[3] = (*phi_a0 * player->unk_B62) >> 8;
        vtx++;
        phi_a0++;
    }

    dList = POLY_XLU_DISP;

    gSPMatrix(&dList[0], Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(&dList[1], object_link_zora_DL_011760);

    POLY_XLU_DISP = &dList[2];

    CLOSE_DISPS(play->state.gfxCtx);
}

void func_80125500(PlayState* play, Player* player, s32 limbIndex, Vec3f* pos, Vec3s* rot) {
    if (limbIndex == PLAYER_LIMB_L_THIGH) {
        func_80124870(play, player, &player->skelAnime, pos, rot, 7, 8, 9);
    } else if (limbIndex == PLAYER_LIMB_R_THIGH) {
        func_80124870(play, player, &player->skelAnime, pos, rot, 4, 5, 6);
    }
}

// Probably EnArrow, waiting for it to be decomped
typedef struct {
    /* 0x000 */ Actor actor;
    /* 0x144 */ f32 unk_144;
} ActorUnknown; // size >= 0x144

// Player_OverrideLimbDrawGameplayCommon
s32 func_80125580(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    s32 pad;
    Player* player = (Player*)thisx;
    Vec3f sp54;
    Actor* sp50;
    f32 temp_f6;
    Actor* phi_v0;
    struct_80124618* phi_a0;
    struct_80124618* phi_v0_2;
    struct_80124618* phi_v1_2;

    if (limbIndex == PLAYER_LIMB_ROOT) {
        D_801F59F4 = (s32)player->leftHandType;
        D_801F59F8 = (s32)player->rightHandType;
        D_801F59DC = &player->meleeWeaponInfo[2].base;
        if (player->transformation != PLAYER_FORM_FIERCE_DEITY) {
            if (!(player->skelAnime.moveFlags & 4) || (player->skelAnime.moveFlags & 1)) {
                pos->x *= player->ageProperties->unk_08;
                pos->z *= player->ageProperties->unk_08;
            }
            if (!(player->skelAnime.moveFlags & 4) || (player->skelAnime.moveFlags & 2)) {
                pos->y *= player->ageProperties->unk_08;
            }
        }

        pos->y -= player->unk_AB8;
        if ((player->transformation == PLAYER_FORM_ZORA) && (player->stateFlags3 & PLAYER_STATE3_8000)) {
            Matrix_Translate(pos->x, ((Math_CosS(player->unk_AAA) - 1.0f) * 200.0f) + pos->y, pos->z, 1);
            Matrix_RotateXS(player->unk_AAA, 1);
            if (player->unk_B62 != 0) {
                Matrix_Push();
                Matrix_RotateZS(player->unk_B8E, 1);
                Matrix_RotateZYX(rot->x, rot->y, rot->z, 1);
                Matrix_RotateXS(-0x8000, 1);
                Matrix_Translate(0.0f, 0.0f, -4000.0f, 1);
                Player_DrawZoraShield(play, player);
                Matrix_Pop();
            }
            Matrix_RotateZS(player->unk_B86[1], 1);
            Matrix_RotateZYX(rot->x, rot->y, rot->z, 1);
            func_80125318(pos, rot);
        } else if (player->stateFlags3 & PLAYER_STATE3_2000) {
            func_801251C4(player, &sp54);
            sp54.x -= player->actor.world.pos.x;
            sp54.y -= player->actor.world.pos.y;
            sp54.z -= player->actor.world.pos.z;
            Matrix_Translate(pos->x + sp54.x, pos->y + sp54.y, pos->z + sp54.z, 1);
            Matrix_RotateXS(player->unk_B94, 1);
            Matrix_RotateZS(player->unk_B8E, 1);
            player->unk_AB2.x = player->unk_B90 - player->unk_B94;
            Matrix_RotateZYX(rot->x, rot->y, rot->z, 1);
            func_80125318(pos, rot);
        } else if (player->unk_AAA != 0) {
            Matrix_Translate(pos->x, ((Math_CosS(player->unk_AAA) - 1.0f) * 200.0f) + pos->y, pos->z, 1);
            Matrix_RotateXS(player->unk_AAA, 1);
            Matrix_RotateZYX(rot->x, rot->y, rot->z, 1);
            func_80125318(pos, rot);
        }
    } else {
        if (*dList != NULL) {
            D_801F59DC++;
        }

        if (limbIndex == PLAYER_LIMB_HEAD) {
            rot->x += player->unk_AAC.z;
            rot->y -= player->unk_AAC.y;
            rot->z += player->unk_AAC.x;
            if (player->transformation == PLAYER_FORM_DEKU) {
                sp50 = NULL;

                if (((player->skelAnime.animation == &gameplay_keep_Linkanim_00E298)) ||
                    (player->unk_284.animation == &gameplay_keep_Linkanim_00E2F0) ||
                    ((player->stateFlags3 & PLAYER_STATE3_40) && (sp50 = player->heldActor, (sp50 != NULL)))) {
                    Matrix_TranslateRotateZYX(pos, rot);
                    func_80125340();
                    func_80125318(pos, rot);
                    if (sp50 != NULL) {
                        f32 temp;

                        player->unk_AF0[0].x = 1.0f - (((ActorUnknown*)sp50)->unk_144 * 0.03f);
                        player->unk_AF0[0].y = 1.0f - (((ActorUnknown*)sp50)->unk_144 * 0.01f);
                        player->unk_AF0[0].z = 1.0f - (((ActorUnknown*)sp50)->unk_144 * 0.04f);
                        rot->z = (((ActorUnknown*)sp50)->unk_144 * 320.0f);
                    } else if (player->skelAnime.animation == &gameplay_keep_Linkanim_00E298) {
                        func_80124618(D_801C03E0, player->skelAnime.curFrame, player->unk_AF0);
                    } else {
                        func_80124618(D_801C03C0, player->unk_284.curFrame, player->unk_AF0);
                    }
                    Matrix_Scale(player->unk_AF0[0].x, player->unk_AF0[0].y, player->unk_AF0[0].z, 1);
                }
            }
        } else if (limbIndex == PLAYER_LIMB_TORSO) {
            if ((player->skelAnime.animation == &gameplay_keep_Linkanim_00E1F8) ||
                (player->skelAnime.animation == &gameplay_keep_Linkanim_00E260) ||
                (player->skelAnime.animation == &gameplay_keep_Linkanim_00E248) ||
                (player->transformation == PLAYER_FORM_ZORA)) {
                Matrix_TranslateRotateZYX(pos, rot);
                if (player->transformation == PLAYER_FORM_GORON) {
                    func_80125340();
                }
                func_80125318(pos, rot);
                if ((player->transformation != PLAYER_FORM_ZORA) ||
                    (player->skelAnime.animation == &gameplay_keep_Linkanim_00E410)) {
                    if (player->skelAnime.animation == &gameplay_keep_Linkanim_00E410) {
                        phi_a0 = D_801C0608;
                    } else {
                        if (player->skelAnime.animation == &gameplay_keep_Linkanim_00E260) {
                            phi_v0_2 = D_801C0628;
                        } else {
                            if (player->skelAnime.animation == &gameplay_keep_Linkanim_00E248) {
                                phi_v1_2 = D_801C07C0;
                            } else {
                                phi_v1_2 = D_801C0460;
                            }
                            phi_v0_2 = phi_v1_2;
                        }
                        phi_a0 = phi_v0_2;
                    }
                    func_80124618(phi_a0, player->skelAnime.curFrame, player->unk_AF0);
                } else {
                    player->unk_AF0[0].y = 1.0f;
                }
                if (player->transformation == PLAYER_FORM_ZORA) {
                    player->unk_AF0[0].y *= 1.0f + (0.29999995f * player->unk_B10[0]);
                    player->unk_AF0[0].x = 1.0f;
                    player->unk_AF0[0].z = 1.0f;
                }
                Matrix_Scale(player->unk_AF0[0].x, player->unk_AF0[0].y, player->unk_AF0[0].z, 1);
            }
        } else if (limbIndex == PLAYER_LIMB_UPPER_ROOT) {
            s16 phi_a0_2;
            s16 rot_temp = 0x44C;

            if (player->unk_AA8 != 0) {
                Matrix_RotateZS(rot_temp, 1);
                Matrix_RotateYS(player->unk_AA8, 1);
            }

            if (player->unk_AB2.y != 0) {
                Matrix_RotateYS(player->unk_AB2.y, 1);
            }

            phi_a0_2 = player->unk_AB2.x;
            if ((player->transformation == PLAYER_FORM_DEKU) && (player->stateFlags3 & PLAYER_STATE3_40)) {
                if (player->heldActor != NULL) {
                    phi_a0_2 += (s16)(((ActorUnknown*)player->heldActor)->unk_144 * -470.0f);
                }
            }

            Matrix_RotateXS(phi_a0_2, 1);
            if (player->unk_AB2.z != 0) {
                Matrix_RotateZS(player->unk_AB2.z, 1);
            }
        } else {
            func_80125500(play, player, limbIndex, pos, rot);
        }
    }

    return 0;
}

void func_80125CE0(Player* player, struct_80124618* arg1, Vec3f* pos, Vec3s* rot) {
    Matrix_TranslateRotateZYX(pos, rot);
    func_80125318(pos, rot);
    func_80124618(arg1, player->skelAnime.curFrame, player->unk_AF0);
    Matrix_Scale(player->unk_AF0[0].x, player->unk_AF0[0].y, player->unk_AF0[0].z, MTXMODE_APPLY);
}

#ifdef NON_EQUIVALENT
s32 func_80125D4C(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* actor) {
    Player* player = (Player*)actor;
    s32 temp_v0_2;
    s32 temp_v0_9;
    Gfx** phi_a0;
    s32 phi_a1;
    Gfx** phi_v1;
    s32 new_var;
    struct_80124618* phi_a1_2;
    Gfx** phi_v1_2;
    s32 phi_v0;

    if (!func_80125580(play, limbIndex, dList, pos, rot, player)) {
        if (limbIndex == PLAYER_LIMB_L_HAND) {
            phi_a0 = player->leftHandDLists;
            if (player->stateFlags3 & PLAYER_STATE3_2000) {
                rot->z -= player->unk_B8C;
            } else if ((D_801F59F4 == 4) && (player->stateFlags1 & PLAYER_STATE1_2000000)) {
                phi_a0 = &gPlayerLeftHandOpenDLs[D_801F59E0];
                D_801F59F4 = 0;
            } else if ((player->leftHandType == 0) && (player->actor.speedXZ > 2.0f) &&
                       !(player->stateFlags1 & PLAYER_STATE1_8000000)) {
                phi_a0 = &gPlayerLeftHandClosedDLs[D_801F59E0];
                D_801F59F4 = 1;
            } else {
                if ((player->leftHandType == 2) && (player->transformation == PLAYER_FORM_HUMAN) &&
                    ((phi_v0 = CUR_EQUIP_VALUE(EQUIP_TYPE_SWORD), phi_v0 != 0))) {
                    phi_a0 = &D_801C018C[phi_v0 - 1];
                } else {
                    phi_v0 = player->skelAnime.jointTable[0x16].x & 0xF000;
                    if (phi_v0 != 0) {
                        phi_v0 = (phi_v0 >> 0xC) - 1;
                        if (phi_v0 >= 2) {
                            phi_v0 = 0;
                        }
                        phi_a0 = &D_801C095C[phi_v0][D_801F59E0];
                    }
                }
            }
            *dList = phi_a0[D_801F59E4];
            if (player->transformation == PLAYER_FORM_GORON) {
                if (&gameplay_keep_Linkanim_00E218 == player->skelAnime.animation) {
                    func_80125CE0(player, D_801C0750, pos, rot);
                }
            } else if (player->transformation == PLAYER_FORM_ZORA) {
                if ((player->stateFlags1 & PLAYER_STATE1_2) || (player->stateFlags1 & PLAYER_STATE1_400) ||
                    func_801242B4(player)) {
                    *dList = gLinkZoraLeftHandOpenDL;
                } else {
                    phi_a1 = player->skelAnime.animation == &gameplay_keep_Linkanim_00E3E8 &&
                             player->skelAnime.curFrame >= 6.0f;
                    if ((phi_a1 != 0) || (player->skelAnime.animation == &gameplay_keep_Linkanim_00E3E0)) {
                        *dList = object_link_zora_DL_00E2A0;
                        if (phi_a1 != 0) {
                            phi_a1 = D_801C0538;
                        } else {
                            phi_a1 = D_801C0560;
                        }
                        func_80125CE0(player, phi_a1, pos, rot);
                    }
                }
            }
        } else if (limbIndex == PLAYER_LIMB_R_HAND) {
            if ((player->transformation == PLAYER_FORM_ZORA) &&
                (((player->stateFlags1 & PLAYER_STATE1_2)) || (player->stateFlags1 & PLAYER_STATE1_400) ||
                 func_801242B4(player))) {
                *dList = gLinkZoraRightHandOpenDL;
            } else {
                phi_v1 = player->rightHandDLists;
                if (player->stateFlags3 & PLAYER_STATE3_2000) {
                    rot->z -= player->unk_B8C;
                }
                if (D_801F59F8 == 8) {
                    if (player->transformation == PLAYER_FORM_HUMAN) {
                        if (player->currentShield != 0) {
                            phi_v1 = &gPlayerHandHoldingShields[player->currentShield - 1];
                        }
                    }
                } else if ((player->rightHandType == 6) && (player->actor.speedXZ > 2.0f) &&
                           (!(player->stateFlags1 & PLAYER_STATE1_8000000))) {
                    phi_v1 = &gPlayerRightHandClosedDLs[D_801F59E0];
                    D_801F59F8 = 7;
                } else {
                    // temp_v0_9 = player->skelAnime.jointTable->unk_84 & 0xF00;
                    temp_v0_9 = player->skelAnime.jointTable[0x16].x & 0xF00;

                    if (temp_v0_9 != 0) {
                        temp_v0_9 = (temp_v0_9 >> 8) - 1;
                        phi_v1 = &D_801C0964[temp_v0_9][D_801F59E0];
                    }
                }
                *dList = phi_v1[D_801F59E4];
                if (&gameplay_keep_Linkanim_00E230 == player->skelAnime.animation) {
                    func_80125CE0(player, D_801C0784, pos, rot);
                }
            }
        } else if (limbIndex == PLAYER_LIMB_SHEATH) {
            Gfx** phi_v1_2 = player->sheathDLists;

            if (player->transformation == PLAYER_FORM_HUMAN) {
                s32 val = GET_CUR_EQUIP_VALUE(EQUIP_TYPE_SWORD);

                if (val != 0) {
                    if ((player->sheathType == 0xE) || (player->sheathType == 0xC)) {
                        phi_v1_2 = &gPlayerSheathedSwords[val - 1][0];
                    } else {
                        phi_v1_2 = &gPlayerSwordSheaths[val - 1][0];
                    }
                }
            }
            *dList = phi_v1_2[D_801F59E4];
        } else if (limbIndex == PLAYER_LIMB_WAIST) {
            *dList = player->waistDLists[D_801F59E4];
        } else if (limbIndex == PLAYER_LIMB_HAT) {
            if (player->transformation == PLAYER_FORM_ZORA) {
                // Matrix_Scale((player->unk_B10[0] * 1.0f) + 1.0f, 1.0f, 1.0f, 1);
                // super important (* 1)
                Matrix_Scale((player->unk_B10[0] * 1) + 1.0f, 1.0f, 1.0f, MTXMODE_APPLY);
            }
        }
    }
    return 0;
}
#else
// OverrideLimbDrawFlex
s32 func_80125D4C(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* actor);
#pragma GLOBAL_ASM("asm/non_matchings/code/z_player_lib/func_80125D4C.s")
#endif

// Player_OverrideLimbDrawGameplayFirstPerson
s32 func_801262C8(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* actor) {
    Player* player = (Player*)actor;

    if (func_80125580(play, limbIndex, dList, pos, rot, actor) == 0) {
        if (player->unk_AA5 != 3) {
            *dList = NULL;
        } else if (limbIndex == PLAYER_LIMB_L_FOREARM) {
            *dList = D_801C0294[player->transformation];
        } else if (limbIndex == PLAYER_LIMB_L_HAND) {
            *dList = D_801C02A8[player->transformation];
        } else if (limbIndex == PLAYER_LIMB_R_SHOULDER) {
            *dList = D_801C02BC[player->transformation];
        } else if (limbIndex == PLAYER_LIMB_R_HAND) {
            if (Player_IsHoldingHookshot(player)) {
                *dList = D_801C02E4[player->transformation];
            } else {
                *dList = D_801C02D0[player->transformation];
            }
        } else {
            *dList = NULL;
        }
    }

    return false;
}

// unused
s32 func_801263FC(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    Player* player = (Player*)thisx;

    if (!func_80125580(play, limbIndex, dList, pos, rot, thisx)) {
        *dList = NULL;
    }

    return false;
}

s32 func_80126440(PlayState* play, ColliderQuad* collider, WeaponInfo* weaponInfo, Vec3f* arg3, Vec3f* arg4) {
    if (weaponInfo->active == 0) {
        if (collider != NULL) {
            Collider_ResetQuadAT(play, &collider->base);
        }
        Math_Vec3f_Copy(&weaponInfo->tip, arg3);
        Math_Vec3f_Copy(&weaponInfo->base, arg4);
        weaponInfo->active = 1;

        return true;
    }

    if ((weaponInfo->tip.x == arg3->x) && (weaponInfo->tip.y == arg3->y) && (weaponInfo->tip.z == arg3->z) &&
        (weaponInfo->base.x == arg4->x) && (weaponInfo->base.y == arg4->y) && (weaponInfo->base.z == arg4->z)) {
        if (collider != NULL) {
            Collider_ResetQuadAT(play, &collider->base);
        }

        return false;
    }

    if (collider != NULL) {
        Collider_SetQuadVertices(collider, arg4, arg3, &weaponInfo->base, &weaponInfo->tip);
        CollisionCheck_SetAT(play, &play->colChkCtx, &collider->base);
    }
    Math_Vec3f_Copy(&weaponInfo->base, arg4);
    Math_Vec3f_Copy(&weaponInfo->tip, arg3);
    weaponInfo->active = 1;

    return true;
}

u8 D_801C096C[PLAYER_SHIELD_MAX] = {
    COLTYPE_METAL,
    COLTYPE_METAL,
    COLTYPE_METAL,
};

void func_801265C8(PlayState* play, Player* player, ColliderQuad* collider, Vec3f arg3[4]) {
    if (player->stateFlags1 & PLAYER_STATE1_400000) {
        Vec3f sp4C;
        Vec3f sp40;
        Vec3f sp34;
        Vec3f sp28;

        player->shieldQuad.base.colType = D_801C096C[player->currentShield];
        Matrix_MultVec3f(&arg3[0], &sp28);
        Matrix_MultVec3f(&arg3[1], &sp34);
        Matrix_MultVec3f(&arg3[2], &sp40);
        Matrix_MultVec3f(&arg3[3], &sp4C);
        Collider_SetQuadVertices(collider, &sp28, &sp34, &sp40, &sp4C);
        CollisionCheck_SetAC(play, &play->colChkCtx, &collider->base);
        CollisionCheck_SetAT(play, &play->colChkCtx, &collider->base);
    }
}

Vec3f D_801C0970[] = {
    { 0.0f, 400.0f, 0.0f },
    { 0.0f, 1400.0f, -1000.0f },
    { 0.0f, -400.0f, 1000.0f },
};

Vec3f D_801C0994[] = {
    { 5000.0f, 400.0f, 0.0f },
    { 5000.0f, -400.0f, 1000.0f },
    { 5000.0f, 1400.0f, -1000.0f },
};

Vec3f D_801C09B8[] = {
    { 0.0f, 750.0f, 750.0f },
    { 1500.0f, 1500.0f, 1500.0f },
    { -2500.0f, -2000.0f, -3000.0f },
};

Vec3f D_801C09DC[] = {
    { 900.0f, 300.0f, 100.0f },
    { 1300.0f, 700.0f, -300.0f },
    { 500.0f, -100.0f, 500.0f },
};

Vec3f D_801C0A00[] = {
    { -2500.0f, 1400.0f, 1100.0f },
    { -2900.0f, 1000.0f, 1500.0f },
    { -2100.0f, 1800.0f, 700.0f },
};

Vec3f D_801C0A24[] = {
    { 0.0f, 0.0f, 0.0f },
    { -800.0f, 800.0f, 800.0f },
    { -800.0f, -800.0f, -800.0f },
};

Vec3f D_801C0A48[] = {
    { 2000.0f, 0.0f, 0.0f },
    { 2800.0f, -800.0f, -800.0f },
    { 2800.0f, 800.0f, 800.0f },
};

Vec3f D_801C0A6C[] = {
    { -400.0f, 800.0f, 0.0f },
    { -5000.0f, -500.0f, -4000.0f },
    { -5000.0f, 8000.0f, 4000.0f },
};

Vec3f D_801C0A90[] = {
    { -400.0f, 1800.0f, 0.0f },
    { 5000.0f, 8000.0f, 4000.0f },
    { 5000.0f, -500.0f, -4000.0f },
};

Gfx* D_801C0AB4[] = {
    object_link_zora_DL_00CC38,
    object_link_zora_DL_00CDA0,
};

Gfx* D_801C0ABC[] = {
    object_link_zora_DL_010868,
    object_link_zora_DL_010978,
};

Vec3f D_801C0AC4[] = {
    { 5400.0f, 1700.0f, 1800.0f },
    { 5400.0f, 1700.0f, -1800.0f },
};

Vec3f D_801C0ADC[] = {
    { 5250.0f, 570.0f, 2400.0f },
    { 5250.0f, 570.0f, -2400.0f },
};

struct_80124618* D_801C0AF4[] = {
    D_801C0678,
    D_801C0698,
};

struct_80124618* D_801C0AFC[] = {
    D_801C06B8,
    D_801C06E0,
};

struct_80124618* D_801C0B04[] = {
    D_801C06F8,
    D_801C0718,
};

struct_80124618* D_801C0B0C[] = {
    D_801C0730,
    D_801C0740,
};

Gfx* D_801C0B14[] = {
    object_link_nuts_DL_008760,
    object_link_nuts_DL_008660,
};

u8 D_801C0B1C[] = {
    0x0C,
    0x0F,
};

Gfx* D_801C0B20[] = {
    object_mask_truth_DL_0001A0,
    object_mask_kerfay_DL_000D40,
    object_mask_yofukasi_DL_000490,
    object_mask_rabit_DL_000610,
    object_mask_ki_tan_DL_0004A0,
    object_mask_json_DL_0004C0,
    object_mask_romerny_DL_0007A0,
    object_mask_zacho_DL_000700,
    object_mask_posthat_DL_000290,
    object_mask_meoto_DL_0005A0,
    object_mask_bigelf_DL_0016F0,
    object_mask_gibudo_DL_000250,
    gDonGeroMaskDL,
    object_mask_dancer_DL_000EF0,
    object_mask_skj_DL_0009F0,
    object_mask_stone_DL_000820,
    object_mask_bree_DL_0003C0,
    object_mask_bakuretu_DL_0005C0,
    object_mask_bu_san_DL_000710,
    object_mask_kyojin_DL_000380,
    gameplay_keep_DL_00B260,
    gameplay_keep_DL_005A10,
    gameplay_keep_DL_005360,
    gameplay_keep_DL_0056C0,
    object_mask_boy_DL_000900,
    object_mask_goron_DL_0014A0,
    object_mask_zora_DL_000DB0,
    object_mask_nuts_DL_001D90,
};

Vec3f D_801C0B90[] = {
    { 950.0f, -800.0f, 300.0f },
    { 950.0f, -800.0f, -300.0f },
};

void func_8012669C(PlayState* play, Player* player, Vec3f* arg2, Vec3f* arg3) {
    Vec3f sp3C;
    Vec3f sp30;

    Matrix_MultVec3f(arg2, &sp3C);
    Matrix_MultVec3f(arg3, &sp30);

    if (player->meleeWeaponState != 0) {
        if (func_80126440(play, NULL, &player->meleeWeaponInfo[0], &sp3C, &sp30) &&
            (player->transformation != PLAYER_FORM_GORON) && (!(player->stateFlags1 & PLAYER_STATE1_400000))) {
            EffectBlure_AddVertex(Effect_GetByIndex(player->meleeWeaponEffectIndex[0]), &player->meleeWeaponInfo[0].tip,
                                  &player->meleeWeaponInfo[0].base);
        }
        if ((player->meleeWeaponState > 0) &&
            ((player->meleeWeaponAnimation < 0x1E) || (player->stateFlags2 & PLAYER_STATE2_20000))) {
            Matrix_MultVec3f(&arg2[1], &sp3C);
            Matrix_MultVec3f(&arg3[1], &sp30);
            func_80126440(play, &player->meleeWeaponQuads[0], &player->meleeWeaponInfo[1], &sp3C, &sp30);
            Matrix_MultVec3f(&arg2[2], &sp3C);
            Matrix_MultVec3f(&arg3[2], &sp30);
            func_80126440(play, &player->meleeWeaponQuads[1], &player->meleeWeaponInfo[2], &sp3C, &sp30);
        }
    } else {
        Math_Vec3f_Copy(&player->meleeWeaponInfo[0].tip, &sp3C);
        Math_Vec3f_Copy(&player->meleeWeaponInfo[0].base, &sp30);
    }
}

void Player_DrawGetItemImpl(PlayState* play, Player* player, Vec3f* refPos, s32 drawIdPlusOne) {
    f32 sp34;

    if (player->stateFlags3 & PLAYER_STATE3_4000000) {
        sp34 = 6.0f;
    } else {
        sp34 = 14.0f;
    }

    OPEN_DISPS(play->state.gfxCtx);

    gSegments[6] = PHYSICAL_TO_VIRTUAL(player->giObjectSegment);

    gSPSegment(POLY_OPA_DISP++, 0x06, player->giObjectSegment);
    gSPSegment(POLY_XLU_DISP++, 0x06, player->giObjectSegment);

    Matrix_Translate((Math_SinS(player->actor.shape.rot.y) * 3.3f) + refPos->x, refPos->y + sp34,
                     (Math_CosS(player->actor.shape.rot.y) * 3.3f) + refPos->z, MTXMODE_NEW);
    Matrix_RotateZYX(0, (play->gameplayFrames * 1000), 0, MTXMODE_APPLY);
    Matrix_Scale(0.2f, 0.2f, 0.2f, MTXMODE_APPLY);
    GetItem_Draw(play, drawIdPlusOne - 1);

    CLOSE_DISPS(play->state.gfxCtx);
}

void Player_DrawGetItem(PlayState* play, Player* player) {
    if (!player->giObjectLoading || (osRecvMesg(&player->giObjectLoadQueue, NULL, OS_MESG_NOBLOCK) == 0)) {
        Vec3f refPos;
        s32 drawIdPlusOne;

        player->giObjectLoading = false;
        if ((player->actor.id == ACTOR_EN_TEST3) ||
            ((player->transformation == PLAYER_FORM_DEKU) && (player->stateFlags1 & PLAYER_STATE1_400))) {
            refPos.x = player->actor.world.pos.x;
            refPos.z = player->actor.world.pos.z;
            if (player->actor.id == ACTOR_EN_TEST3) {
                if (player->stateFlags1 & PLAYER_STATE1_400) {
                    refPos.y = player->actor.world.pos.y + 30.0f;
                } else {
                    refPos.x = player->bodyPartsPos[0xC].x;
                    refPos.y = player->bodyPartsPos[0xC].y - 6.0f;
                    refPos.z = player->bodyPartsPos[0xC].z;
                }
            } else {
                refPos.y = player->actor.world.pos.y + 28.0f;
            }
        } else {
            Math_Vec3f_Copy(&refPos, &D_801F59E8);
        }

        drawIdPlusOne = ABS_ALT(player->unk_B2A);
        Player_DrawGetItemImpl(play, player, &refPos, drawIdPlusOne);
    }
}

void func_80126AB4(Player* player, Vec3f** arg1) {
    if ((player->transformation == PLAYER_FORM_GORON) || (player->actor.id == ACTOR_EN_TEST3)) {
        Math_Vec3f_Copy(&player->unk_AF0[1], &player->meleeWeaponInfo[0].base);
        *arg1 = D_801C09B8;
        return;
    }

    D_801C0994[1].x = D_801C0994[0].x;

    if (player->unk_ADD >= 3) {
        player->unk_ADD++;
        D_801C0994[1].x *= 1.0f + ((9 - player->unk_ADD) * 0.1f);
    }

    D_801C0994[1].x += 1200.0f;
    D_801C0994[2].x = D_801C0994[1].x;
    *arg1 = D_801C0994;
}

void func_80126B8C(PlayState* play, Player* player) {
    Vec3f* sp1C;

    func_80126AB4(player, &sp1C);
    func_8012669C(play, player, sp1C, D_801C0970);
}

void func_80126BD0(PlayState* play, Player* player, s32 arg2) {
    if ((player->transformation != PLAYER_FORM_ZORA) || (player->rightHandType == 0xB)) {
        return;
    }

    if ((arg2 != 0) && (player->stateFlags1 & PLAYER_STATE1_400000)) {
        OPEN_DISPS(play->state.gfxCtx);

        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, object_link_zora_DL_0110A8);

        CLOSE_DISPS(play->state.gfxCtx);
    } else {
        Actor* boomerangActor = player->boomerangActor;
        Vec3f sp58;
        Vec3f sp4C;

        if (!(player->stateFlags1 & PLAYER_STATE1_2000000) ||
            ((player->boomerangActor != NULL) && (player->boomerangActor->params != arg2) &&
             (((boomerangActor->child == NULL)) || (boomerangActor->child->params != arg2)))) {
            OPEN_DISPS(play->state.gfxCtx);

            if ((player->skelAnime.animation != &gameplay_keep_Linkanim_00E3E0) &&
                (player->skelAnime.animation != &gameplay_keep_Linkanim_00E3D8)) {
                if (player->skelAnime.animation == &gameplay_keep_Linkanim_00E3E8) {
                    func_80124618(D_801C0580, player->skelAnime.curFrame, player->unk_AF0);
                } else if (player->skelAnime.animation == &gameplay_keep_Linkanim_00E418) {
                    func_80124618(D_801C05A8, player->skelAnime.curFrame, player->unk_AF0);
                } else if (player->skelAnime.animation == &gameplay_keep_Linkanim_00E408) {
                    func_80124618(D_801C05D8, player->skelAnime.curFrame, player->unk_AF0);
                } else if (player->skelAnime.animation == &gameplay_keep_Linkanim_00E350) {
                    func_80124618(D_801C07F0, player->skelAnime.curFrame, player->unk_AF0);
                } else if (player->skelAnime.animation == &gameplay_keep_Linkanim_00E3F0) {
                    func_80124618(D_801C0820, player->skelAnime.curFrame, player->unk_AF0);
                } else if (player->skelAnime.animation == &gameplay_keep_Linkanim_00E3F8) {
                    func_80124618(D_801C0838, player->skelAnime.curFrame, player->unk_AF0);
                } else if (player->itemActionParam == 8) {
                    player->unk_AF0[0].x = 1.0f;
                    player->unk_AF0[0].y = 1.0f;
                    player->unk_AF0[0].z = 1.0f;
                } else {
                    player->unk_AF0[0].x = 0.4f;
                    player->unk_AF0[0].y = 0.6f;
                    player->unk_AF0[0].z = 0.7f;
                }
                Matrix_Push();

                Matrix_Scale(player->unk_AF0[0].x, player->unk_AF0[0].y, player->unk_AF0[0].z, 1);
                gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx),
                          G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

                gSPDisplayList(POLY_OPA_DISP++, D_801C0AB4[arg2]);

                if ((player->meleeWeaponState != 0) && ((((player->meleeWeaponAnimation == 0x1B)) && (arg2 == 0)) ||
                                                        ((player->meleeWeaponAnimation == 0x1C) && (arg2 != 0)))) {
                    func_8012669C(play, player, D_801C0A00, D_801C09DC);
                }
                Matrix_Pop();
            }

            if (player->skelAnime.animation == &gameplay_keep_Linkanim_00E418) {
                func_80124618(D_801C05C8, player->skelAnime.curFrame, player->unk_AF0);
                D_801C05F0[0].unk_2.x = 0x32;
                D_801C05F0[0].unk_2.y = 0x32;
                D_801C05F0[0].unk_2.z = 0x32;
                D_801C05F0[1].unk_2 = D_801C05F0[0].unk_2;
            } else if (player->skelAnime.animation == &gameplay_keep_Linkanim_00E408) {
                func_80124618(D_801C05F0, player->skelAnime.curFrame, player->unk_AF0);
            } else if (player->skelAnime.animation == &gameplay_keep_Linkanim_00E3D8) {
                player->unk_AF0[0].x = ((f32)ABS_ALT(player->unk_B8A) * 0.00003f) + 0.5f;
                player->unk_AF0[0].y = player->unk_AF0[0].x;
                player->unk_AF0[0].z = player->unk_AF0[0].x;

                D_801C05F0[0].unk_2.x = (player->unk_AF0[0].x * 100.0f);

                D_801C05F0[0].unk_2.y = D_801C05F0[0].unk_2.x;
                D_801C05F0[0].unk_2.z = D_801C05F0[0].unk_2.x;

                D_801C05F0[1].unk_2 = D_801C05F0[0].unk_2;
            } else {
                //! @bug Skips CLOSE_DISPS
                return;
            }

            Matrix_Push();
            Matrix_Scale(player->unk_AF0[0].x, player->unk_AF0[0].y, player->unk_AF0[0].z, MTXMODE_APPLY);

            gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_OPA_DISP++, D_801C0ABC[arg2]);

            Matrix_MultVec3f(&D_801C0AC4[arg2], &sp58);
            Matrix_MultVec3f(&D_801C0ADC[arg2], &sp4C);

            if (func_80126440(play, NULL, &player->meleeWeaponInfo[arg2], &sp58, &sp4C) &&
                (player->stateFlags1 & PLAYER_STATE1_8000000)) {
                EffectBlure_AddVertex(Effect_GetByIndex(player->meleeWeaponEffectIndex[arg2]),
                                      &player->meleeWeaponInfo[arg2].tip, &player->meleeWeaponInfo[arg2].base);
            }
            Matrix_Pop();

            CLOSE_DISPS(play->state.gfxCtx);
        }
    }
}

s32 func_801271B0(PlayState* play, Player* player, s32 arg2) {
    if (player->transformation == PLAYER_FORM_DEKU) {
        if (((player->skelAnime.animation == &gameplay_keep_Linkanim_00E2D0)) ||
            (player->skelAnime.animation == &gameplay_keep_Linkanim_00E2D8) ||
            (player->skelAnime.animation == &gameplay_keep_Linkanim_00E2E8) ||
            (player->skelAnime.animation == &gameplay_keep_Linkanim_00E278)) {
            struct_80124618** sp3C = D_801C0AF4;

            OPEN_DISPS(play->state.gfxCtx);

            if (player->skelAnime.animation == &gameplay_keep_Linkanim_00E2D8) {
                sp3C = D_801C0AFC;
            } else if (player->skelAnime.animation == &gameplay_keep_Linkanim_00E278) {
                sp3C = D_801C0B0C;
            } else if (player->skelAnime.animation == &gameplay_keep_Linkanim_00E2E8) {
                sp3C = D_801C0B04;
            }

            Matrix_Push();
            Matrix_Translate(0.0f, 150.0f, 0.0f, MTXMODE_APPLY);
            func_80124618(sp3C[0], player->skelAnime.curFrame, &player->unk_AF0[1]);
            Matrix_Scale(player->unk_AF0[1].x, player->unk_AF0[1].y, player->unk_AF0[1].z, 1);

            gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

            gSPDisplayList(POLY_OPA_DISP++, D_801C0B14[arg2]);

            Matrix_Translate(2150.0f, 0.0f, 0.0f, MTXMODE_APPLY);
            Matrix_RotateXS(player->unk_B8A, MTXMODE_APPLY);
            func_80124618(sp3C[1], player->skelAnime.curFrame, &player->unk_AF0[1]);
            Matrix_Scale(player->unk_AF0[1].x, player->unk_AF0[1].y, player->unk_AF0[1].z, MTXMODE_APPLY);

            gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

            // Close flower / Open flower
            gSPDisplayList(POLY_OPA_DISP++,
                           player->actor.velocity.y < -6.0f ? object_link_nuts_DL_008AA0 : object_link_nuts_DL_008C50);

            Matrix_MultZero(&player->bodyPartsPos[D_801C0B1C[arg2]]);
            Matrix_Pop();

            CLOSE_DISPS(play->state.gfxCtx);

            return true;
        }
    }

    return false;
}

// Player_SetMaskSegment?
s32 func_80127438(PlayState* play, Player* player, s32 currentMask) {
    if ((player->maskObjectLoadState == 0) && (currentMask == (u8)player->maskId)) {
        OPEN_DISPS(play->state.gfxCtx);

        gSPSegment(POLY_OPA_DISP++, 0x0A, player->maskObjectSegment);

        CLOSE_DISPS(play->state.gfxCtx);

        return true;
    }

    return false;
}

void func_80127488(PlayState* play, Player* player, u8 alpha) {
    OPEN_DISPS(play->state.gfxCtx);

    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, alpha);
    gSPDisplayList(POLY_XLU_DISP++, gLinkGoronGoronPunchEffectDL);

    func_80122BA4(play, &player->unk_3D0, 3, alpha);

    CLOSE_DISPS(play->state.gfxCtx);
}

void Player_DrawCouplesMask(PlayState* play, Player* player) {
    gSegments[0xA] = PHYSICAL_TO_VIRTUAL(player->maskObjectSegment);
    AnimatedMat_DrawOpa(play, Lib_SegmentedToVirtual(&object_mask_meoto_Matanimheader_001CD8));
}

void Player_DrawCircusLeadersMask(PlayState* play, Actor* actor) {
    static Vec3f D_801C0BA8 = { 0.0f, 0.0f, 0.0f };
    static Vec3f D_801C0BB4 = { 0.0f, 0.0f, 0.0f };
    Gfx* gfx;
    s32 i;

    OPEN_DISPS(play->state.gfxCtx);

    gfx = POLY_XLU_DISP;

    for (i = 0; i < ARRAY_COUNT(D_801C0B90); i++) {
        f32 temp_f22 = (D_801F59C8[i] / 400.0f) * 0.1f;

        Matrix_MultVec3f(&D_801C0B90[i], &D_801F59B0[i]);

        if (1) {}

        D_801F59B0[i].y += -10.0f * temp_f22;

        if (D_801F59C8[i] < 0x190) {
            f32 phi_f20;

            if (temp_f22 > 0.05f) {
                phi_f20 = 0.05f;
            } else {
                phi_f20 = temp_f22;
            }

            Matrix_Push();
            Matrix_Translate(D_801F59B0[i].x, D_801F59B0[i].y, D_801F59B0[i].z, 0);
            Matrix_Scale(phi_f20, temp_f22, phi_f20, 1);

            gSPMatrix(&gfx[0], Matrix_NewMtx(play->state.gfxCtx), G_MTX_PUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPSegment(&gfx[1], 0x08, gSegments[SEGMENT_NUMBER(gEffBubble1Tex)] + SEGMENT_OFFSET(gEffBubble1Tex));
            gDPSetPrimColor(&gfx[2], 0, 0, 255, 255, 255, 255);
            gDPSetEnvColor(&gfx[3], 150, 150, 150, 0);
            gSPDisplayList(&gfx[4], gEffBubbleDL);
            gSPPopMatrix(&gfx[5], G_MTX_MODELVIEW);

            Matrix_Pop();

            gfx = &gfx[6];
        } else {
            Player* player = (Player*)actor;
            f32 temp_f0 = sqrtf(SQ(player->actor.velocity.x) + SQ(player->actor.velocity.z));
            s16 phi_s0 = temp_f0 * 2000.0f;
            f32 temp_f20;

            D_801C0BA8.y = temp_f0 * 0.4f;
            D_801C0BB4.y = -0.3f;

            if (phi_s0 > 0x3E80) {
                phi_s0 = 0x3E80;
            }

            phi_s0 = player->actor.focus.rot.y + ((i != 0) ? phi_s0 : -phi_s0);
            temp_f20 = temp_f0 * 0.2f;

            if (temp_f20 > 4.0f) {
                temp_f20 = 4.0f;
            }

            D_801C0BA8.x = -Math_SinS(phi_s0) * temp_f20;
            D_801C0BA8.z = -Math_CosS(phi_s0) * temp_f20;

            EffectSsDtBubble_SpawnColorProfile(play, &D_801F59B0[i], &D_801C0BA8, &D_801C0BB4, 0x14, 0x14, 3, 0);
            D_801F59C8[i] -= 0x190;
        }
    }

    POLY_XLU_DISP = gfx;

    CLOSE_DISPS(play->state.gfxCtx);
}

void Player_DrawBlastMask(PlayState* play, Player* player) {
    static Gfx D_801C0BC0[] = {
        gsDPSetEnvColor(0, 0, 0, 255),
        gsSPEndDisplayList(),
    };
    static Gfx D_801C0BD0[] = {
        gsDPSetRenderMode(AA_EN | Z_CMP | Z_UPD | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL |
                              G_RM_FOG_SHADE_A,
                          AA_EN | Z_CMP | Z_UPD | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL |
                              GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)),
        gsSPEndDisplayList(),
    };

    OPEN_DISPS(play->state.gfxCtx);

    if (player->unk_B60 != 0) {
        s32 alpha;

        gSegments[0xA] = VIRTUAL_TO_PHYSICAL(player->maskObjectSegment);

        AnimatedMat_DrawOpa(play, Lib_SegmentedToVirtual(&object_mask_bakuretu_Matanimheader_0011F8));

        if (player->unk_B60 < 11) {
            alpha = (player->unk_B60 / 10.0f) * 255;
        } else {
            alpha = 255;
        }

        gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, (u8)alpha);
        gSPDisplayList(POLY_OPA_DISP++, object_mask_bakuretu_DL_000440);
        gSPSegment(POLY_OPA_DISP++, 0x09, D_801C0BD0);
        gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, (u8)(255 - alpha));
    } else {
        gSPSegment(POLY_OPA_DISP++, 0x09, D_801C0BC0);
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

Vec3f D_801C0BE0 = { 0.0f, 0.3f, 0.0f };
Vec3f D_801C0BEC = { 0.0f, -0.025f, 0.0f };
Color_RGBA8 D_801C0BF8 = { 250, 100, 100, 0 };
Color_RGBA8 D_801C0BFC = { 0, 0, 100, 0 };

Vec3f D_801C0C00 = { 0.0f, 20.0f, 0.0f };

Vec3f D_801C0C0C[] = {
    { 174.0f, -1269.0f, -1.0f },
    { 401.0f, -729.0f, -701.0f },
    { 401.0f, -729.0f, 699.0f },
};

Vec3f D_801C0C30[] = {
    { 74.0f, -1269.0f, -1.0f },
    { 301.0f, -729.0f, -701.0f },
    { 301.0f, -729.0f, 699.0f },
};

typedef struct {
    /* 0x00 */ f32 unk_00;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ Vec3f unk_08;
    /* 0x14 */ f32 unk_14;
    /* 0x18 */ s16 unk_18;
    /* 0x1A */ s16 unk_1A;
} struct_80128388_arg1; // size = 0x1C

struct_80128388_arg1 D_801C0C54[] = {
    { 0.0f, 0x0000, 0x8000, { 0.0f, 0.0f, 0.0f }, 0.0f, 0x0000, 0x0000 },
    { 16.8f, 0x0000, 0x0000, { 0.0f, 0.0f, 0.0f }, 20.0f, 0x1388, 0x1388 },
    { 30.0f, 0x0000, 0x0000, { 0.0f, 0.0f, 0.0f }, 20.0f, 0x1F40, 0x2EE0 },
};

Color_RGB8 D_801C0CA8[] = {
    { 255, 255, 255 }, { 80, 80, 255 },   { 136, 192, 255 }, { 136, 192, 255 }, { 184, 232, 232 }, { 248, 200, 0 },
    { 255, 180, 0 },   { 0, 128, 0 },     { 252, 238, 0 },   { 131, 0, 174 },   { 64, 64, 32 },    { 0, 0, 255 },
    { 255, 0, 255 },   { 255, 0, 255 },   { 255, 0, 0 },     { 0, 0, 255 },     { 0, 200, 0 },     { 255, 255, 255 },
    { 255, 255, 255 }, { 255, 255, 255 }, { 80, 80, 255 },
};

Vec3f D_801C0CE8[PLAYER_FORM_MAX] = {
    { 0.0f, 0.0f, 0.0f },    { 300.0f, 300.0f, -230.0f }, { 0.0f, 90.0f, -50.0f },
    { 0.0f, 20.0f, -60.0f }, { 0.0f, 0.0f, 0.0f },
};
Vec3f D_801C0D24[PLAYER_FORM_MAX] = {
    { 200.0f, 300.0f, 0.0f }, { 200.0f, 200.0f, 0.0f }, { 200.0f, 300.0f, 0.0f },
    { 200.0f, 150.0f, 0.0f }, { 200.0f, 200.0f, 0.0f },
};
Vec3f D_801C0D60 = { 398.0f, 1419.0f, 244.0f };
Vec3f D_801C0D6C = { 420.0f, 1210.0f, 380.0f };

f32 D_801C0D78[] = {
    0.0f,    // Player is not holding a melee weapon
    3000.0f, // PLAYER_AP_SWORD_KOKIRI
    3000.0f, // PLAYER_AP_SWORD_RAZOR
    4000.0f, // PLAYER_AP_SWORD_GILDED
    5500.0f, // PLAYER_AP_SWORD_GREAT_FAIRY
    -1.0f,   // PLAYER_AP_STICK
    2500.0f, // PLAYER_AP_ZORA_FINS
};

Gfx* D_801C0D94 = object_link_child_DL_017818;

f32 D_801C0D98 = -35.0f;

f32 D_801C0D9C = -395.0f;
f32 D_801C0DA0 = 0.0f;

f32 D_801C0DA4 = 0.0f;

Vec3f D_801C0DA8[4] = {
    { -4500.0f, -3000.0f, -600.0f },
    { 1500.0f, -3000.0f, -600.0f },
    { -4500.0f, 3000.0f, -600.0f },
    { 1500.0f, 3000.0f, -600.0f },
};

Vec3f D_801C0DD8 = { 50.0f, 800.0f, 0.0f };
Vec3f D_801C0DE4 = { 50.0f, 850.0f, 0.0f };
Gfx* D_801C0DF0[] = {
    object_link_goron_DL_010590, object_link_goron_DL_010368, object_link_goron_DL_010140,
    object_link_goron_DL_00FF18, object_link_goron_DL_00FCF0,
};

Vec2f D_801C0E04[PLAYER_FORM_MAX] = {
    { 140.0f, -130.0f }, { 0.0f, -200.0f }, { -160.0f, 0.0f }, { 220.0f, -200.0f }, { 0.0f, 0.0f },
};

Gfx* D_801C0E2C[] = {
    object_link_nuts_DL_007A28, object_link_nuts_DL_0077D0, object_link_nuts_DL_007548,
    object_link_nuts_DL_007900, object_link_nuts_DL_0076A0,
};
Vec3f D_801C0E40[PLAYER_FORM_MAX] = {
    { 0.0f, 0.0f, 0.0f },       { -578.3f, -1100.9f, 0.0f }, { -189.5f, -594.87f, 0.0f },
    { -570.0f, -812.0f, 0.0f }, { -230.0f, -520.0f, 0.0f },
};
Vec3f D_801C0E7C = { 1100.0f, -700.0f, 0.0f };

// unused
Vec3f D_801C0E88 = { 1600.0f, -1700.0f, -70.0f };

Vec3f D_801C0E94 = { 1800.0f, -300.0f, 0.0f };
Vec3f D_801C0EA0 = { 1300.0f, -400.0f, 0.0f };
Vec3f D_801C0EAC = { 630.0f, 100.0f, -30.0f };
Vec3s D_801C0EB8 = { 0, 0, 0x7FFF };

void Player_DrawBunnyHood(PlayState* play) {
    Mtx* temp_a1 = GRAPH_ALLOC(play->state.gfxCtx, 2 * sizeof(Mtx));
    Vec3s sp2C;

    OPEN_DISPS(play->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x0B, temp_a1);

    Matrix_Push();

    sp2C.x = D_801F59D0.unk_2 + 0x3E2;
    sp2C.y = D_801F59D0.unk_4 + 0xDBE;
    sp2C.z = D_801F59D0.unk_0 - 0x348A;
    Matrix_SetTranslateRotateYXZ(97.0f, -1203.0f, -240.0f, &sp2C);

    Matrix_ToMtx(temp_a1++);

    sp2C.x = D_801F59D0.unk_2 - 0x3E2;
    sp2C.y = -D_801F59D0.unk_4 - 0xDBE;
    sp2C.z = D_801F59D0.unk_0 - 0x348A;
    Matrix_SetTranslateRotateYXZ(97.0f, -1203.0f, 240.0f, &sp2C);

    Matrix_ToMtx(temp_a1);

    Matrix_Pop();

    CLOSE_DISPS(play->state.gfxCtx);
}

void func_80127B64(struct_801F58B0 arg0[], s32 count, Vec3f* arg2) {
    s32 i;

    for (i = 0; i < count; i++, arg0++) {
        Math_Vec3f_Copy(&arg0->unk_00, arg2);
        Math_Vec3f_Copy(&arg0->unk_0C, &gZeroVec3f);
        arg0->unk_18 = 0;
        arg0->unk_1A = 0;
    }
}

void func_80127BE8(PlayState* arg0, Vec3f* arg1) {
    Vec3f sp2C;
    f32 sp28;

    D_801C0BE0.y = Rand_ZeroFloat(0.07f) + -0.1f;
    D_801C0BEC.y = Rand_ZeroFloat(0.1f) + 0.04f;
    if (Rand_ZeroOne() < 0.5f) {
        sp28 = -1.0f;
    } else {
        sp28 = 1.0f;
    }

    D_801C0BE0.x = (Rand_ZeroFloat(0.2f) + 0.1f) * sp28;
    D_801C0BEC.x = 0.1f * sp28;
    if (Rand_ZeroOne() < 0.5f) {
        sp28 = -1.0f;
    } else {
        sp28 = 1.0f;
    }

    D_801C0BE0.z = (Rand_ZeroFloat(0.2f) + 0.1f) * sp28;
    D_801C0BEC.z = 0.1f * sp28;
    sp2C.x = arg1->x;
    sp2C.y = Rand_ZeroFloat(15.0f) + arg1->y;
    sp2C.z = arg1->z;
    EffectSsKirakira_SpawnDispersed(arg0, &sp2C, &D_801C0BE0, &D_801C0BEC, &D_801C0BF8, &D_801C0BFC, -0x32, 0xB);
}

void func_80127DA4(PlayState* play, struct_801F58B0 arg1[], struct_80128388_arg1 arg2[], s32 arg3, Vec3f* arg4,
                   Vec3f* arg5, u32* arg6) {
    struct_801F58B0* phi_s1 = &arg1[1];
    Vec3f spB0;
    Vec3f spA4;
    f32 f22;
    f32 f28;
    f32 f24;
    f32 f20;
    f32 f0;
    f32 sp8C = -1.0f;
    s32 i;
    s16 s0;
    s16 s2;

    Math_Vec3f_Copy(&arg1->unk_00, arg4);
    Math_Vec3f_Diff(arg5, arg4, &spB0);
    arg1->unk_18 = Math_FAtan2F(spB0.z, spB0.x);
    arg1->unk_1A = Math_FAtan2F(sqrtf(SQ(spB0.x) + SQ(spB0.z)), spB0.y);
    i = 1;
    arg2 += 1;

    while (i < arg3) {
        if ((play->actorCtx.unk5 & 8) != 0) {
            if ((*arg6 & 0x20) != 0) {
                sp8C = -0.2f;
            } else {
                sp8C = 0.2f;
            }

            *arg6 += 0x16;
            if ((*arg6 & 1) == 0) {
                func_80127BE8(play, &phi_s1->unk_00);
            }
        }
        Math_Vec3f_Sum(&phi_s1->unk_00, &phi_s1->unk_0C, &phi_s1->unk_00);

        f0 = Math_Vec3f_DistXYZAndStoreDiff(&arg1->unk_00, &phi_s1->unk_00, &spB0);
        f28 = f0 - arg2->unk_00;
        if (f0 == 0.0f) {
            spB0.x = 0.0f;
            spB0.y = arg2->unk_00;
            spB0.z = 0.0f;
        }
        f20 = sqrtf(SQ(spB0.x) + SQ(spB0.z));

        if (f20 > 4.0f) {
            phi_s1->unk_18 = Math_FAtan2F(spB0.z, spB0.x);
            s2 = phi_s1->unk_18 - arg1->unk_18;

            if (ABS_ALT(s2) > 0x4000) {
                phi_s1->unk_18 = (s16)(phi_s1->unk_18 + 0x8000);
                f20 = -f20;
            }
        }

        phi_s1->unk_1A = Math_FAtan2F(f20, spB0.y);

        s2 = phi_s1->unk_18 - arg1->unk_18;
        s2 = CLAMP(s2, -arg2->unk_18, arg2->unk_18);
        phi_s1->unk_18 = arg1->unk_18 + s2;

        s0 = phi_s1->unk_1A - arg1->unk_1A;
        s0 = CLAMP(s0, -arg2->unk_1A, arg2->unk_1A);
        phi_s1->unk_1A = arg1->unk_1A + s0;

        f20 = Math_CosS(phi_s1->unk_1A) * arg2->unk_00;
        spA4.x = Math_SinS(phi_s1->unk_18) * f20;
        spA4.z = Math_CosS(phi_s1->unk_18) * f20;
        spA4.y = Math_SinS(phi_s1->unk_1A) * arg2->unk_00;
        Math_Vec3f_Sum(&arg1->unk_00, &spA4, &phi_s1->unk_00);
        phi_s1->unk_0C.x *= 0.9f;
        phi_s1->unk_0C.z *= 0.9f;

        f22 = Math_CosS(s0) * f28;
        f24 = Math_SinS(s0) * f28;
        phi_s1->unk_0C.y += sp8C;

        if ((play->actorCtx.unk5 & 8) != 0) {
            phi_s1->unk_0C.y = CLAMP(phi_s1->unk_0C.y, -0.8f, 0.8f);
        } else {
            phi_s1->unk_0C.y = phi_s1->unk_0C.y;
            f20 = Math_SinS(arg1->unk_1A);
            phi_s1->unk_0C.y += (((f22 * Math_CosS(arg1->unk_1A)) + (f24 * f20)) * 0.2f);
            phi_s1->unk_0C.y = CLAMP(phi_s1->unk_0C.y, -2.0f, 4.0f);
        }

        f20 = (f24 * Math_CosS(arg1->unk_1A)) - (Math_SinS(arg1->unk_1A) * f22);
        f22 = Math_CosS(s2) * f20;
        f24 = Math_SinS(s2) * f20;

        f20 = Math_SinS(arg1->unk_18);

        phi_s1->unk_0C.x += (((f24 * Math_CosS(arg1->unk_18)) - (f22 * f20)) * 0.1f);
        phi_s1->unk_0C.x = CLAMP(phi_s1->unk_0C.x, -4.0f, 4.0f);

        f20 = Math_SinS(arg1->unk_18);

        phi_s1->unk_0C.z += (((f22 * Math_CosS(arg1->unk_18)) + (f24 * f20)) * -0.1f);
        phi_s1->unk_0C.z = CLAMP(phi_s1->unk_0C.z, -4.0f, 4.0f);

        arg1++;
        phi_s1++;
        i++;
        arg2++;
    }
}

void func_80128388(struct_801F58B0 arg0[], struct_80128388_arg1 arg1[], s32 arg2, Mtx** arg3) {
    struct_801F58B0* phi_s1 = &arg0[1];
    Vec3f sp58;
    Vec3s sp50;
    s32 i;

    sp58.y = 0.0f;
    sp58.z = 0.0f;
    sp50.x = 0;
    i = 1;

    if (arg2 >= 2) {
        do {
            sp58.x = arg1->unk_00 * 100.0f;
            sp50.z = arg1->unk_06 + (s16)(phi_s1->unk_1A - arg0->unk_1A);
            sp50.y = arg1->unk_04 + (s16)(phi_s1->unk_18 - arg0->unk_18);
            Matrix_TranslateRotateZYX(&sp58, &sp50);
            Matrix_ToMtx(*arg3);
            (*arg3)++;
            arg0++;
            phi_s1++;
            arg1++;
            i++;
        } while (i < arg2);
    }
}

void Player_DrawGreatFairysMask(PlayState* play, Player* player) {
    s32 pad;
    Mtx* sp90 = GRAPH_ALLOC(play->state.gfxCtx, 6 * sizeof(Mtx));
    Vec3f sp84;
    Vec3f sp78;
    Vec3f* iter = D_801C0C0C;
    Vec3f* iter2 = D_801C0C30;
    u32 sp6C = play->gameplayFrames;
    s32 i;

    OPEN_DISPS(play->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x0B, sp90);

    Matrix_MultVec3f(&D_801C0C00, &D_801C0C54[1].unk_08);
    Math_Vec3f_Lerp(&player->bodyPartsPos[7], &player->bodyPartsPos[0], 0.2f, &D_801C0C54[2].unk_08);

    for (i = 0; i < ARRAY_COUNT(D_801C0C0C); i++) {
        Matrix_MultVec3f(iter, &sp84);
        Matrix_MultVec3f(iter2, &sp78);

        func_80127DA4(play, D_801F58B0[i], D_801C0C54, 3, &sp84, &sp78, &sp6C);
        sp6C += 11;

        Matrix_Push();
        Matrix_Translate(iter->x, iter->y, iter->z, MTXMODE_APPLY);
        func_80128388(D_801F58B0[i], D_801C0C54, 3, &sp90);
        Matrix_Pop();
        iter++;
        iter2++;
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

s32 func_80128640(PlayState* play, Player* player, Gfx* dlist) {
    s32 temp_v1 = player->skelAnime.animation == &gameplay_keep_Linkanim_00D0A8;
    f32 temp_f0;

    if ((temp_v1) ||
        ((player->currentMask != PLAYER_MASK_NONE) && (player->skelAnime.animation == &gameplay_keep_Linkanim_00D0C8) &&
         (temp_f0 = player->skelAnime.curFrame - 8.0f, (temp_f0 >= 0.0f)) && (temp_f0 < 4.0f)) ||
        (player->stateFlags2 & PLAYER_STATE2_1000000)) {
        s32 sp6C;

        if (temp_v1) {
            sp6C = player->prevMask;
        } else {
            sp6C = player->currentMask;
        }
        if (func_80127438(play, player, sp6C)) {
            OPEN_DISPS(play->state.gfxCtx);

            Matrix_Push();
            Matrix_Translate(-323.67f, 412.15f, -969.96f, MTXMODE_APPLY);
            Matrix_RotateZYX(-0x32BE, -0x50DE, -0x7717, MTXMODE_APPLY);

            gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

            gSPDisplayList(POLY_OPA_DISP++, D_801C0B20[sp6C - 1]);

            Matrix_Pop();

            CLOSE_DISPS(play->state.gfxCtx);
        }

    } else if (player->heldItemActionParam == 7) {
        OPEN_DISPS(play->state.gfxCtx);

        Matrix_Push();
        Matrix_Translate(-428.26f, 267.2f, -33.82f, MTXMODE_APPLY);
        Matrix_RotateZYX(-0x8000, 0, 0x4000, MTXMODE_APPLY);
        Matrix_Scale(1.0f, player->unk_B08[1], 1.0f, MTXMODE_APPLY);

        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, gDekuStickDL);

        Matrix_Pop();

        CLOSE_DISPS(play->state.gfxCtx);
    } else if (player->leftHandType == 5) {
        s32 sp48 = Player_ActionToBottle(player, player->heldItemActionParam);
        Vec3f* temp_v1_2 = &D_801C0CE8[player->transformation];

        OPEN_DISPS(play->state.gfxCtx);

        Matrix_Push();
        Matrix_Translate(temp_v1_2->x, temp_v1_2->y, temp_v1_2->z, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        if (sp48 != 0) {
            Color_RGB8* temp_v1_3 = &D_801C0CA8[sp48];

            gDPSetEnvColor(POLY_XLU_DISP++, temp_v1_3->r, temp_v1_3->g, temp_v1_3->b, 0);
            gSPDisplayList(POLY_XLU_DISP++, gameplay_keep_DL_000320);
        }
        gSPDisplayList(POLY_XLU_DISP++, gameplay_keep_DL_0003E0);

        Matrix_Pop();

        CLOSE_DISPS(play->state.gfxCtx);
    } else if (dlist == object_link_zora_DL_00E2A0) { // zora guitar
        s16 sp26 = Math_SinS(player->unk_B86[0]) * (ABS_ALT(player->unk_AB2.x) * ((f32)(IREG(52) + 20)) / 100.0f);
        s16 sp24 = Math_SinS(player->unk_B86[1]) * (ABS_ALT(player->unk_AB2.y) * ((f32)(IREG(53) + 15)) / 100.0f);

        OPEN_DISPS(play->state.gfxCtx);

        Matrix_Push();
        Matrix_RotateXS(sp26, MTXMODE_APPLY);
        Matrix_RotateYS(sp24, MTXMODE_APPLY);

        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, object_link_zora_DL_00E088); // hand

        Matrix_Pop();

        CLOSE_DISPS(play->state.gfxCtx);
    } else {
        return 0;
    }

    return 1;
}

void func_80128B74(PlayState* play, Player* player, s32 limbIndex) {
    Vec3f* footPos = &D_801C0D24[player->transformation];

    Actor_SetFeetPos(&player->actor, limbIndex, 9, footPos, 6, footPos);
}

#ifdef NON_EQUIVALENT
void func_80128BD0(PlayState* play, s32 limbIndex, Gfx** dList1, Gfx** dList2, Vec3s* rot, Actor* actor) {
    Player* player = (Player*)actor;
    s32 pad[8];
    MtxF sp230;
    Actor* sp224;
    Vec3f sp20C;
    MtxF sp1BC;
    Vec3f sp178[5];
    s32 sp154;
    Vec3f spF0[5];
    Vec3f spD4;
    MtxF sp68;
    Vec3f sp5C;
    Vec3f sp50;
    Vec3s* temp_s0_11;
    Vec3s* temp_s1;
    f32 temp_f0;
    s16 temp_v0;
    Vec3f* temp_s0_10;
    Vec2f* temp_s0_6;

    if (*dList2 != NULL) {
        Matrix_MultZero(D_801F59DC);
    }

    if (limbIndex == PLAYER_LIMB_L_HAND) {
        Math_Vec3f_Copy(&player->leftHandWorld.pos, D_801F59DC);
        if ((*dList1 != 0) && !func_801271B0(play, player, 0) && !func_80128640(play, player, *dList1) &&
            (&gameplay_keep_Linkanim_00E218 == player->skelAnime.animation)) {
            func_80127488(play, player, D_801C0778[(s32)player->skelAnime.curFrame]);
        }

        if (player->actor.scale.y >= 0.0f) {
            if (!Player_IsHoldingHookshot(player) && ((player->heldActor != NULL))) {
                if ((player->stateFlags3 & PLAYER_STATE3_40) && ((player->transformation != PLAYER_FORM_DEKU))) {
                    Vec3f* phi_a0;

                    phi_a0 = &D_801C0D60;
                    if (player->transformation == PLAYER_FORM_HUMAN) {
                        phi_a0 = &D_801C0D6C;
                    }

                    Matrix_MultVec3f(phi_a0, &player->heldActor->world.pos);
                    Matrix_RotateZYX(0x69E8, -0x5708, 0x458E, MTXMODE_APPLY);
                    Matrix_Get(&sp230);
                    temp_s1 = &player->heldActor->world.rot;
                    Matrix_MtxFToYXZRot(&sp230, temp_s1, 0);
                    player->heldActor->shape.rot = *temp_s1;
                } else if (player->stateFlags1 & PLAYER_STATE1_800) {
                    temp_v0 = player->actor.shape.rot.y + player->leftHandWorld.rot.y;
                    player->heldActor->shape.rot.y = temp_v0;
                    player->heldActor->world.rot.y = temp_v0;
                }
            } else {
                if ((player->transformation == PLAYER_FORM_FIERCE_DEITY) ||
                    ((player->transformation != PLAYER_FORM_ZORA) &&
                     ((player->heldItemActionParam == 7) ||
                      ((player->meleeWeaponState != 0) && ((player->meleeWeaponAnimation != 0x19)) &&
                       (player->meleeWeaponAnimation != 0x1A))))) {
                    if (player->heldItemActionParam == 7) {
                        D_801C0994->x = player->unk_B08[1] * 5000.0f;
                    } else {
                        D_801C0994->x = D_801C0D78[Player_GetMeleeWeaponHeld(player)];
                    }
                    func_80126B8C(play, player);
                }
                Matrix_Get(&player->mf_CC4);
                Matrix_MtxFToYXZRot(&player->mf_CC4, &player->leftHandWorld.rot, 0);
            }
        }
    } else if (limbIndex == PLAYER_LIMB_R_HAND) {
        sp224 = player->heldActor;

        if (*dList1 != 0) {
            if (player->rightHandType == 9) {
                OPEN_DISPS(play->state.gfxCtx);

                Matrix_Push();
                Matrix_Translate(D_801C0D9C, D_801C0D9C, D_801C0DA0, MTXMODE_APPLY);
                if ((player->stateFlags3 & PLAYER_STATE3_40) && (player->unk_B28 >= 0) && (player->unk_ACC < 0xB)) {
                    Matrix_MultZero(&sp20C);
                    temp_f0 = Math_Vec3f_DistXYZ(D_801F59DC, &sp20C);
                    player->unk_B08[0] = temp_f0 - 3.0f;
                    if (temp_f0 < 3.0f) {
                        player->unk_B08[0] = 0.0f;
                    } else {
                        player->unk_B08[0] *= 1.6f;
                        if (player->unk_B08[0] > 1.0f) {
                            player->unk_B08[0] = 1.0f;
                        }
                    }
                    player->unk_B08[1] = -0.5f;
                }
                Matrix_Scale(1.0f, player->unk_B08[0], 1.0f, MTXMODE_APPLY);

                gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx),
                          G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

                gSPDisplayList(POLY_XLU_DISP++, D_801C0D94);

                Matrix_Pop();

                CLOSE_DISPS(play->state.gfxCtx);
            } else if (&gameplay_keep_Linkanim_00E230 == player->skelAnime.animation) {
                func_80127488(play, player, D_801C07AC[(s32)player->skelAnime.curFrame]);
            } else {
                func_801271B0(play, player, 1);
            }
        }

        if (player->actor.scale.y >= 0.0f) {
            if (player->rightHandType == 0xFF) {
                Matrix_Get(&player->shieldMf);
            } else if (player->rightHandType == 8) {
                Matrix_Get(&player->shieldMf);
                func_801265C8(play, player, &player->shieldQuad, D_801C0DA8);
            } else if (player->rightHandType == 0xB) {
                Matrix_MultVec3f(&D_801C0DD8, &player->rightHandWorld.pos);
                if (sp224 != NULL) {
                    Matrix_MultVec3f(&D_801C0DE4, &sp224->world.pos);
                    Matrix_RotateZYX(0, -0x4000, -0x4000, MTXMODE_APPLY);
                    Matrix_Get(&sp1BC);
                    Matrix_MtxFToYXZRot(&sp1BC, &sp224->world.rot, 0);
                    sp224->shape.rot = sp224->world.rot;
                    if (func_800B7128(player) != 0) {
                        Matrix_Translate(500.0f, 300.0f, 0.0f, MTXMODE_APPLY);
                        func_80124CC4(play, player, 77600.0f);
                    }
                }
            } else if ((player->meleeWeaponState != 0) && (player->meleeWeaponAnimation == 0x19)) {
                func_80126B8C(play, player);
            }
            if ((player->unk_B2A != 0) || ((func_800B7118(player) == 0) && (sp224 != NULL))) {
                if (!(player->stateFlags1 & PLAYER_STATE1_400) && (player->unk_B2A != 0) &&
                    (player->exchangeItemId != PLAYER_AP_NONE)) {
                    Math_Vec3f_Copy(&D_801F59E8, &player->leftHandWorld.pos);
                } else {
                    D_801F59E8.x = (player->bodyPartsPos[0xF].x + player->leftHandWorld.pos.x) * 0.5f;
                    D_801F59E8.y = (player->bodyPartsPos[0xF].y + player->leftHandWorld.pos.y) * 0.5f;
                    D_801F59E8.z = (player->bodyPartsPos[0xF].z + player->leftHandWorld.pos.z) * 0.5f;
                }

                if (player->unk_B2A == 0) {
                    Math_Vec3f_Copy(&sp224->world.pos, &D_801F59E8);
                }
            }
        }
    } else if (limbIndex == PLAYER_LIMB_L_FOREARM) {
        func_80126BD0(play, player, 0);
    } else if (limbIndex == PLAYER_LIMB_R_FOREARM) {
        func_80126BD0(play, player, 1);
    } else if (limbIndex == PLAYER_LIMB_TORSO) {
        if (player->transformation == PLAYER_FORM_GORON) {
            if ((&gameplay_keep_Linkanim_00E1F8 == player->skelAnime.animation) ||
                ((&gameplay_keep_Linkanim_00E200 == player->skelAnime.animation) != 0) ||
                (&gameplay_keep_Linkanim_00E1F0 == player->skelAnime.animation)) {
                s32 i;

                OPEN_DISPS(play->state.gfxCtx);

                if (&gameplay_keep_Linkanim_00E200 == player->skelAnime.animation) {
                    f32* phi_v0 = &player->unk_B10[0];

                    for (i = 0; i < ARRAY_COUNT(sp178); i++) {
                        func_80124618(D_801C0510, *phi_v0, &sp178[i]);

                        phi_v0++;
                    }
                } else {
                    if (&gameplay_keep_Linkanim_00E1F8 == player->skelAnime.animation) {
                        func_8012536C();
                        func_80124618(D_801C0428, player->skelAnime.curFrame, &player->unk_AF0[1]);
                    }

                    for (i = 0; i < ARRAY_COUNT(sp178); i++) {
                        Math_Vec3f_Copy(&sp178[i], &player->unk_AF0[1]);
                    }
                }
                Matrix_Push();
                Matrix_Scale(player->unk_AF0[1].x, player->unk_AF0[1].y, player->unk_AF0[1].z, MTXMODE_APPLY);

                gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx),
                          G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

                gSPDisplayList(POLY_XLU_DISP++, object_link_goron_DL_00FC18);

                Matrix_Pop();

                for (i = 0; i < ARRAY_COUNT(sp178); i++) {
                    Matrix_Push();
                    Matrix_Scale(sp178[i].x, sp178[i].y, sp178[i].z, MTXMODE_APPLY);

                    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx),
                              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                    gSPDisplayList(POLY_XLU_DISP++, D_801C0DF0[i]);

                    Matrix_Pop();
                }

                CLOSE_DISPS(play->state.gfxCtx);
            }
        }
    } else if (limbIndex == PLAYER_LIMB_HEAD) {
        if ((*dList1 != NULL) && ((player->currentMask != PLAYER_MASK_NONE)) &&
            ((((player->transformation == PLAYER_FORM_HUMAN)) &&
              ((&gameplay_keep_Linkanim_00D0C8 != player->skelAnime.animation) ||
               (player->skelAnime.curFrame >= 12.0f))) ||
             ((player->transformation != PLAYER_FORM_HUMAN) && ((s32)player->currentMask >= PLAYER_MASK_FIERCE_DEITY) &&
              (player->currentMask != (player->transformation + PLAYER_MASK_FIERCE_DEITY)) &&
              (player->skelAnime.curFrame >= 10.0f)))) {
            if (func_80127438(play, player, player->currentMask)) {
                sp154 = player->currentMask - 1;

                OPEN_DISPS(play->state.gfxCtx);

                if (player->currentMask == PLAYER_MASK_COUPLE) {
                    func_8012754C(play, player);
                } else if (player->currentMask == PLAYER_MASK_CIRCUS_LEADER) {
                    func_80127594(play, player);
                } else if (player->currentMask == PLAYER_MASK_BLAST) {
                    func_801278F8(play, player);
                } else if (player->currentMask == PLAYER_MASK_BUNNY) {
                    func_80127A60(play);
                } else if (player->currentMask == PLAYER_MASK_GREAT_FAIRY) {
                    func_801284A0(play, player);
                } else if ((player->currentMask >= PLAYER_MASK_FIERCE_DEITY)) {
                    temp_s0_6 = &D_801C0E04[player->transformation];

                    Matrix_Push();
                    Matrix_Translate(temp_s0_6->x, temp_s0_6->z, 0.0f, 1);
                    Matrix_Scale(1.0f, 1.0f - player->unk_B10[3], 1.0f - player->unk_B10[2], 1);

                    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx),
                              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

                    Matrix_Pop();
                    if ((&gameplay_keep_Linkanim_00D0C8 == player->skelAnime.animation &&
                         (player->skelAnime.curFrame >= 51.0f)) ||
                        (&gameplay_keep_Linkanim_00D0D0 == player->skelAnime.animation)) {
                        sp154 += 4;
                    }
                }

                gSPDisplayList(POLY_XLU_DISP++, D_801C0B20[sp154]);

                CLOSE_DISPS(play->state.gfxCtx);
            }
        } else if (player->transformation == PLAYER_FORM_DEKU) {
            if (&gameplay_keep_Linkanim_00E2C8 == player->skelAnime.animation) {
                OPEN_DISPS(play->state.gfxCtx);

                func_80124618(D_801C0410, player->skelAnime.curFrame, player->unk_AF0);
                Matrix_Push();

                Matrix_Scale(player->unk_AF0[0].x, player->unk_AF0[0].y, player->unk_AF0[0].z, MTXMODE_APPLY);
                gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx),
                          G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPDisplayList(POLY_XLU_DISP++, object_link_nuts_DL_00A348);

                Matrix_Pop();

                CLOSE_DISPS(play->state.gfxCtx);
            } else if (&gameplay_keep_Linkanim_00E2B0 == player->skelAnime.animation ||
                       (&gameplay_keep_Linkanim_00E2A8 == player->skelAnime.animation) ||
                       (&gameplay_keep_Linkanim_00D300 == player->skelAnime.animation)) {
                s32 i;

                OPEN_DISPS(play->state.gfxCtx);

                if (&gameplay_keep_Linkanim_00E2B0 == player->skelAnime.animation) {
                    s32 i;

                    func_80124618(D_801C0340, player->skelAnime.curFrame, &spD4);
                    player->unk_AF0[0].x = spD4.x;
                    func_80124618(D_801C0368, player->skelAnime.curFrame, spF0);

                    for (i = 1; i < ARRAY_COUNT(spF0); i++) {
                        Math_Vec3f_Copy(&spF0[i], spF0);
                    }

                    player->unk_AF0[0].y = spF0[0].x;
                    player->unk_AF0[0].z = spF0[0].x;
                    player->unk_AF0[1].x = spF0[0].x;
                    player->unk_AF0[1].y = spF0[0].x;
                    player->unk_AF0[1].z = spF0[0].x;
                } else {
                    // loop?
                    spF0[0].x = player->unk_AF0[0].y;
                    spF0[0].y = player->unk_AF0[0].y;
                    spF0[0].z = player->unk_AF0[0].y;
                    spF0[1].x = player->unk_AF0[0].z;
                    spF0[1].y = player->unk_AF0[0].z;
                    spF0[1].z = player->unk_AF0[0].z;
                    spF0[2].x = player->unk_AF0[1].x;
                    spF0[2].y = player->unk_AF0[1].x;
                    spF0[2].z = player->unk_AF0[1].x;
                    spF0[3].x = player->unk_AF0[1].y;
                    spF0[3].y = player->unk_AF0[1].y;
                    spF0[3].z = player->unk_AF0[1].y;
                    spF0[4].x = player->unk_AF0[1].z;
                    spF0[4].y = player->unk_AF0[1].z;
                    spF0[4].z = player->unk_AF0[1].z;
                }
                Matrix_Push();
                Matrix_Scale(player->unk_AF0[0].x, player->unk_AF0[0].x, player->unk_AF0[0].x, MTXMODE_APPLY);

                gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx),
                          G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPDisplayList(POLY_XLU_DISP++, object_link_nuts_DL_007390);
                Matrix_Pop();

                for (i = 0; i < ARRAY_COUNT(D_801C0E2C); i++) {
                    Matrix_Push();
                    Matrix_Scale(spF0[i].x, spF0[i].y, spF0[i].z, MTXMODE_APPLY);
                    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx),
                              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                    gSPDisplayList(POLY_XLU_DISP++, D_801C0E2C[i]);
                    Matrix_Pop();
                }

                CLOSE_DISPS(play->state.gfxCtx);
            }
        }
        if ((player->stateFlags1 & (PLAYER_STATE1_100 | PLAYER_STATE1_2)) && (player->unk_AE8 != 0)) {
            OPEN_DISPS(play->state.gfxCtx);

            temp_s0_10 = &D_801C0E40[player->transformation];

            Matrix_Push();
            AnimatedMat_DrawXlu(play, Lib_SegmentedToVirtual(gameplay_keep_Matanimheader_054F18));
            Matrix_Translate(temp_s0_10->x, temp_s0_10->y, 0.0f, MTXMODE_APPLY);
            if (player->transformation == PLAYER_FORM_ZORA) {
                Matrix_Scale(0.7f, 0.7f, 0.7f, 1);
            }

            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 255, (u8)player->unk_AE8);
            gSPDisplayList(POLY_XLU_DISP++, gameplay_keep_DL_054C90);

            Matrix_Pop();

            CLOSE_DISPS(play->state.gfxCtx);
        }
        if (player->actor.scale.y >= 0.0f) {
            if (player->transformation != 3) {
                Matrix_MultVec3f(&D_801C0E7C, &player->actor.focus.pos);
            } else {
                Matrix_MultVec3f(&D_801C0E7C, &player->actor.focus.pos);
                Matrix_MultVec3f(&D_801C0E94, D_801F59DC);
                if (((&gameplay_keep_Linkanim_00E298 == player->skelAnime.animation) ||
                     (&gameplay_keep_Linkanim_00E2F0 == player->unk_284.animation) ||
                     (((player->stateFlags3 & PLAYER_STATE3_40) != 0) && ((player->heldActor != 0)))) &&
                    (player->heldActor != 0)) {
                    Matrix_Push();
                    Matrix_MultVec3f(&D_801C0EA0, &player->heldActor->world.pos);
                    Matrix_RotateZYX(0, 0x4000, 0, MTXMODE_APPLY);
                    Matrix_Get(&sp68);
                    temp_s0_11 = &player->heldActor->world.rot;
                    Matrix_MtxFToYXZRot(&sp68, temp_s0_11, 0);
                    player->heldActor->shape.rot = *temp_s0_11;
                    Matrix_Pop();
                }
            }
        }
    } else if ((limbIndex == PLAYER_LIMB_HAT) && (player->stateFlags3 & PLAYER_STATE3_100000)) {
        Matrix_MultVecX(3000.0f, &sp5C);
        Matrix_MultVecX(2300.0f, &sp50);
        if (func_80126440(play, NULL, player->meleeWeaponInfo, &sp5C, &sp50) != 0) {
            EffectBlure_AddVertex(Effect_GetByIndex(player->meleeWeaponEffectIndex[0]), &player->meleeWeaponInfo[0].tip,
                                  &player->meleeWeaponInfo[0].base);
        }
    } else if (limbIndex == PLAYER_LIMB_R_SHIN) {
        if ((player->meleeWeaponState != 0) &&
            (((player->meleeWeaponAnimation == 0x1D)) || (player->meleeWeaponAnimation == 0x12) ||
             (player->meleeWeaponAnimation == 0x15))) {
            func_8012669C(play, player, D_801C0A48, D_801C0A24);
        }
    } else if (limbIndex == PLAYER_LIMB_WAIST) {
        if ((player->meleeWeaponState != 0) && (player->meleeWeaponAnimation == 0x1A)) {
            Math_Vec3f_Copy(&player->unk_AF0[1], &player->meleeWeaponInfo[0].base);
            func_8012669C(play, player, D_801C0A90, D_801C0A6C);
        }
    } else if (limbIndex == PLAYER_LIMB_SHEATH) {
        if ((*dList1 != NULL) && (player->transformation == PLAYER_FORM_HUMAN) &&
            (player->currentShield != PLAYER_SHIELD_NONE) &&
            (((player->sheathType == 0xE)) || (player->sheathType == 0xF))) {
            OPEN_DISPS(play->state.gfxCtx);

            gSPDisplayList(POLY_XLU_DISP++, *gPlayerShields[player->currentShield - 1]);

            CLOSE_DISPS(play->state.gfxCtx);
        }

        if (player->actor.scale.y >= 0.0f) {
            if ((player->rightHandType != 8) && (player->rightHandType != 0xFF)) {
                Matrix_TranslateRotateZYX(&D_801C0EAC, &D_801C0EB8);
                Matrix_Get(&player->shieldMf);
            }
        }
    } else if (player->actor.scale.y >= 0.0f) {
        func_80128B74(play, player, limbIndex);
    }
    func_8012536C();
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/z_player_lib/func_80128BD0.s")
#endif
