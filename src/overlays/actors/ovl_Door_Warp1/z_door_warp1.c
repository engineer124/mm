/*
 * File: z_door_warp1.c
 * Overlay: ovl_Door_Warp1
 * Description: Blue warp portal and crystal, and the Majora's Mask-shaped boss warp platform
 */

#include "z_door_warp1.h"
#include "objects/object_warp1/object_warp1.h"

#define FLAGS 0x00000000

#define THIS ((DoorWarp1*)thisx)

void DoorWarp1_Init(Actor* thisx, PlayState* play);
void DoorWarp1_Destroy(Actor* thisx, PlayState* play);
void DoorWarp1_Update(Actor* thisx, PlayState* play);
void DoorWarp1_Draw(Actor* thisx, PlayState* play);

void DoorWarp1_SetupAction(DoorWarp1* this, DoorWarp1ActionFunc actionFunc);
void DoorWarp1_MoonWarp_Setup(DoorWarp1* this, PlayState* play);
void DoorWarp1_BossWarp_Setup(DoorWarp1* this, PlayState* play);
void DoorWarp1_EntrancePlatform_Setup(DoorWarp1* this, PlayState* play);
void DoorWarp1_EntranceWarp_Setup(DoorWarp1* this, PlayState* play);
void DoorWarp1_EntranceWarp_WaitForCue(DoorWarp1* this, PlayState* play);
void DoorWarp1_EntranceWarp_Activate(DoorWarp1* this, PlayState* play);
void DoorWarp1_EntranceWarp_Idle(DoorWarp1* this, PlayState* play);
void func_808B93A0(DoorWarp1* this, PlayState* play);
void func_808B94A4(DoorWarp1* this, PlayState* play);
void DoorWarp1_EntranceWarp_StartCutscene(DoorWarp1* this, PlayState* play);
void func_808B958C(DoorWarp1* this, PlayState* play);
void DoorWarp1_EntranceWarp_DoNothing(DoorWarp1* this, PlayState* play);
void DoorWarp1_MoonWarp_Grow(DoorWarp1* this, PlayState* play);
void DoorWarp1_MoonWarp_Idle(DoorWarp1* this, PlayState* play);
void DoorWarp1_MoonWarp_StartCutscene(DoorWarp1* this, PlayState* play);
void DoorWarp1_MoonWarp_WarpOut(DoorWarp1* this, PlayState* play);
void DoorWarp1_BossWarp_WaitToGrow(DoorWarp1* this, PlayState* play);
void DoorWarp1_BossWarp_Grow(DoorWarp1* this, PlayState* play);
void DoorWarp1_BossWarp_IdleWithRemains(DoorWarp1* this, PlayState* play);
void DoorWarp1_BossWarp_Message(DoorWarp1* this, PlayState* play);
void func_808B9ED8(DoorWarp1* this, PlayState* play);
void DoorWarp1_BossWarp_IdleWithoutRemains(DoorWarp1* this, PlayState* play);
void DoorWarp1_BossWarp_StartCutscene(DoorWarp1* this, PlayState* play);
void DoorWarp1_BossWarp_WarpOut(DoorWarp1* this, PlayState* play);
void func_808BA550(DoorWarp1* this, PlayState* play);
void DoorWarp1_EntrancePlatform_StartCutscene(DoorWarp1* this, PlayState* play);
void DoorWarp1_EntrancePlatform_DoNothing(DoorWarp1* this, PlayState* play);
void func_808BB8D4(DoorWarp1* this, PlayState* play, s32 arg2);

static s16 D_808BC000;
static f32 D_808BC004;

ActorInit Door_Warp1_InitVars = {
    ACTOR_DOOR_WARP1,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_WARP1,
    sizeof(DoorWarp1),
    (ActorFunc)DoorWarp1_Init,
    (ActorFunc)DoorWarp1_Destroy,
    (ActorFunc)DoorWarp1_Update,
    (ActorFunc)DoorWarp1_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 4000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 800, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 4000, ICHAIN_STOP),
};

void DoorWarp1_SetupAction(DoorWarp1* this, DoorWarp1ActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

s32 DoorWarp1_GetRemainsIndex(DoorWarp1* this, PlayState* play) {
    s32 ret = 0;

    if ((play->sceneId == SCENE_MITURIN_BS) && !CHECK_QUEST_ITEM(QUEST_REMAINS_ODOLWA)) {
        ret = 1;
    } else if ((play->sceneId == SCENE_HAKUGIN_BS) && !CHECK_QUEST_ITEM(QUEST_REMAINS_GOHT)) {
        ret = 2;
    } else if ((play->sceneId == SCENE_SEA_BS) && !CHECK_QUEST_ITEM(QUEST_REMAINS_GYORG)) {
        ret = 3;
    } else if ((play->sceneId == SCENE_INISIE_BS) && !CHECK_QUEST_ITEM(QUEST_REMAINS_TWINMOLD)) {
        ret = 4;
    }
    return ret;
}

void DoorWarp1_InitLights(DoorWarp1* this, PlayState* play) {
    s32 pad[2];

    Lights_PointNoGlowSetInfo(&this->upperLightInfo, this->dyna.actor.world.pos.x, this->dyna.actor.world.pos.y,
                              this->dyna.actor.world.pos.z, 0, 0, 0, 0);
    this->upperLight = LightContext_InsertLight(play, &play->lightCtx, &this->upperLightInfo);
    Lights_PointNoGlowSetInfo(&this->lowerLightInfo, this->dyna.actor.world.pos.x, this->dyna.actor.world.pos.y,
                              this->dyna.actor.world.pos.z, 0, 0, 0, 0);
    this->lowerLight = LightContext_InsertLight(play, &play->lightCtx, &this->lowerLightInfo);
}

s32 DoorWarp1_PlayerInRange(DoorWarp1* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 ret = false;

    if ((fabsf(this->dyna.actor.xzDistToPlayer) < 60.0f) &&
        (((player->actor.world.pos.y - 20.0f) < this->dyna.actor.world.pos.y) &&
         (this->dyna.actor.world.pos.y < (player->actor.world.pos.y + 20.0f)))) {
        ret = true;
    }
    return ret;
}

void DoorWarp1_Init(Actor* thisx, PlayState* play) {
    DoorWarp1* this = THIS;

    this->unk_1CC = 0;
    this->unk_202 = 0;
    this->unk_203 = 0;
    this->bossRemains = NULL;
    this->unk_1C0 = 0.0f;
    Actor_ProcessInitChain(&this->dyna.actor, sInitChain);
    ActorShape_Init(&this->dyna.actor.shape, 0.0f, NULL, 0.0f);

    this->isBgActor = false;
    this->activationBeamOn = 0;
    this->unk_203 = 0;
    this->unk_204 = 1.0f;

    switch (WARP_GET_TYPE(&this->dyna.actor)) {
        case WARP_BLUE_MOON:
        case WARP_BLUE_BOSS:
        case WARP_ENTRANCE_WOODFALL_TEMPLE:
        case WARP_ENTRANCE_SNOWHEAD_TEMPLE:
        case WARP_ENTRANCE_GREAT_BAY_TEMPLE:
        case WARP_ENTRANCE_STONE_TOWER_TEMPLE:
            DoorWarp1_InitLights(this, play);
            break;

        default:
            break;
    }

    switch (WARP_GET_TYPE(&this->dyna.actor)) {
        case WARP_BLUE_MOON:
            DoorWarp1_MoonWarp_Setup(this, play);
            break;

        case WARP_BLUE_BOSS:
            DoorWarp1_BossWarp_Setup(this, play);
            break;

        case WARP_ENTRANCE_WOODFALL_TEMPLE:
        case WARP_ENTRANCE_SNOWHEAD_TEMPLE:
        case WARP_ENTRANCE_GREAT_BAY_TEMPLE:
        case WARP_ENTRANCE_STONE_TOWER_TEMPLE:
            this->isBgActor = true;
            DynaPolyActor_Init(&this->dyna, 0);
            DynaPolyActor_LoadMesh(play, &this->dyna, &gWarpBossWarpPlatformCol);
            DoorWarp1_EntrancePlatform_Setup(this, play);
            break;

        case WARP_ENTRANCE_ACTIVE:
            DoorWarp1_EntranceWarp_Setup(this, play);
            break;
    }

    if ((play->sceneId == SCENE_MITURIN_BS) || (play->sceneId == SCENE_HAKUGIN_BS) ||
        (play->sceneId == SCENE_INISIE_BS) || (play->sceneId == SCENE_SEA_BS)) {
        Environment_StopTime();
        play->interfaceCtx.restrictions.songOfTime = 1;
        play->interfaceCtx.restrictions.songOfSoaring = 1;
    }
}

void DoorWarp1_Destroy(Actor* thisx, PlayState* play) {
    s32 pad;
    DoorWarp1* this = THIS;
    s16 i;

    LightContext_RemoveLight(play, &play->lightCtx, this->upperLight);
    LightContext_RemoveLight(play, &play->lightCtx, this->lowerLight);

    for (i = 0; i < ARRAY_COUNT(play->envCtx.lightSettings.diffuseColor1); i++) {
        play->envCtx.lightSettings.diffuseColor1[i] = 0;
        play->envCtx.lightSettings.fogColor[i] = play->envCtx.lightSettings.diffuseColor1[i];
        play->envCtx.lightSettings.ambientColor[i] = play->envCtx.lightSettings.diffuseColor1[i];
    }

    if (this->isBgActor) {
        DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
    }
}

void DoorWarp1_MoonWarp_Setup(DoorWarp1* this, PlayState* play) {
    this->scale = 0;
    this->unk_1C6 = -140;
    this->unk_1C8 = -80;
    this->unk_1BC = 1.0f;
    this->blueWarpPortalAlpha2 = 0.0f;
    this->blueWarpPortalAlpha1 = 0.0f;
    this->unk_1B8 = 0.0f;
    this->unk_1A4 = 0.3f;
    this->unk_1A8 = 0.3f;
    this->blueWarpScroll = 0.0f;
    this->dyna.actor.shape.yOffset = 1.0f;
    this->unk_1D0 = 0;
    D_808BC000 = 100;
    Lights_PointNoGlowSetInfo(&this->upperLightInfo, this->dyna.actor.world.pos.x, this->dyna.actor.world.pos.y,
                              this->dyna.actor.world.pos.z, 200, 255, 255, 255);
    Lights_PointNoGlowSetInfo(&this->lowerLightInfo, this->dyna.actor.world.pos.x, this->dyna.actor.world.pos.y,
                              this->dyna.actor.world.pos.z, 200, 255, 255, 255);
    DoorWarp1_SetupAction(this, DoorWarp1_MoonWarp_Grow);
}

void DoorWarp1_BossWarp_Setup(DoorWarp1* this, PlayState* play) {
    SkelAnime_Init(play, &this->skelAnime, &gWarpCrystalSkel, &gWarpCrystalAnim, NULL, NULL, 0);
    Animation_ChangeImpl(&this->skelAnime, &gWarpCrystalAnim, 1.0f, 1.0f, 1.0f, ANIMMODE_ONCE, 40.0f, ANIMTAPER_ACCEL);
    this->scale = 0;
    this->unk_1C6 = -140;
    this->unk_1C8 = -80;
    this->unk_1D0 = 0;
    this->unk_1BC = 1.0f;
    this->unk_1A4 = 0.3f;
    this->unk_1A8 = 0.3f;
    this->blueWarpPortalAlpha2 = 0.0f;
    this->blueWarpPortalAlpha1 = 0.0f;
    this->unk_1B8 = 0.0f;
    this->blueWarpScroll = 0.0f;
    this->dyna.actor.shape.yOffset = -400.0f;
    D_808BC000 = 160;
    Lights_PointNoGlowSetInfo(&this->upperLightInfo, this->dyna.actor.world.pos.x, this->dyna.actor.world.pos.y,
                              this->dyna.actor.world.pos.z, 200, 255, 255, 255);
    Lights_PointNoGlowSetInfo(&this->lowerLightInfo, this->dyna.actor.world.pos.x, this->dyna.actor.world.pos.y,
                              this->dyna.actor.world.pos.z, 200, 255, 255, 255);
    DoorWarp1_SetupAction(this, DoorWarp1_BossWarp_WaitToGrow);
}

void DoorWarp1_EntrancePlatform_Setup(DoorWarp1* this, PlayState* play) {
    this->dyna.actor.shape.yOffset = 0.0f;
    Actor_SetScale(&this->dyna.actor, 0.1f);
    Lights_PointNoGlowSetInfo(&this->upperLightInfo, this->dyna.actor.world.pos.x, this->dyna.actor.world.pos.y,
                              this->dyna.actor.world.pos.z, 200, 255, 255, 255);
    Lights_PointNoGlowSetInfo(&this->lowerLightInfo, this->dyna.actor.world.pos.x, this->dyna.actor.world.pos.y,
                              this->dyna.actor.world.pos.z, 200, 255, 255, 255);
    if (((WARP_GET_TYPE(&this->dyna.actor) == WARP_ENTRANCE_WOODFALL_TEMPLE) &&
         CHECK_QUEST_ITEM(QUEST_REMAINS_ODOLWA)) ||
        ((WARP_GET_TYPE(&this->dyna.actor) == WARP_ENTRANCE_SNOWHEAD_TEMPLE) && CHECK_QUEST_ITEM(QUEST_REMAINS_GOHT)) ||
        ((WARP_GET_TYPE(&this->dyna.actor) == WARP_ENTRANCE_GREAT_BAY_TEMPLE) &&
         CHECK_QUEST_ITEM(QUEST_REMAINS_GYORG)) ||
        ((WARP_GET_TYPE(&this->dyna.actor) == WARP_ENTRANCE_STONE_TOWER_TEMPLE) &&
         CHECK_QUEST_ITEM(QUEST_REMAINS_TWINMOLD))) {
        s16 entranceWarpParams = WARP_GET_TYPE00_2(&this->dyna.actor);

        entranceWarpParams |= WARP_ENTRANCE_ACTIVE;
        Actor_SpawnAsChild(&play->actorCtx, &this->dyna.actor, play, ACTOR_DOOR_WARP1, this->dyna.actor.world.pos.x,
                           this->dyna.actor.world.pos.y + 10.0f, this->dyna.actor.world.pos.z,
                           this->dyna.actor.world.rot.x, this->dyna.actor.world.rot.y, this->dyna.actor.world.rot.z,
                           entranceWarpParams);
        DoorWarp1_SetupAction(this, DoorWarp1_EntrancePlatform_StartCutscene);
    } else {
        DoorWarp1_SetupAction(this, DoorWarp1_EntrancePlatform_DoNothing);
    }
}

void DoorWarp1_EntranceWarp_Setup(DoorWarp1* this, PlayState* play) {
    Actor_SetScale(&this->dyna.actor, 0.1f);
    Lights_PointNoGlowSetInfo(&this->upperLightInfo, this->dyna.actor.world.pos.x, this->dyna.actor.world.pos.y,
                              this->dyna.actor.world.pos.z, 200, 255, 255, 0);
    Lights_PointNoGlowSetInfo(&this->lowerLightInfo, this->dyna.actor.world.pos.x, this->dyna.actor.world.pos.y,
                              this->dyna.actor.world.pos.z, 200, 255, 255, 0);
    Actor_SetScale(&this->dyna.actor, 3.0f);
    this->activationBeamOn = true;
    this->unk_1D0 = 0;
    this->unk_1A8 = 0.0f;
    this->beamScaleY = 0.0f;
    this->unk_1A4 = 700.0f;
    if (play->sceneId == SCENE_INISIE_N) {
        DoorWarp1_SetupAction(this, DoorWarp1_EntranceWarp_DoNothing);
    } else if (CHECK_WEEKEVENTREG(WEEKEVENTREG_WATCHED_ENTRANCE_WARP_CS)) {
        this->activationBeamOn = false;
        DoorWarp1_SetupAction(this, DoorWarp1_EntranceWarp_Idle);
    } else {
        DoorWarp1_SetupAction(this, DoorWarp1_EntranceWarp_WaitForCue);
    }
}

s32 DoorWarp1_EntranceWarp_CheckForCue(DoorWarp1* this, PlayState* play) {
    s32 cueChannel;
    u8 ret = false;

    if (Cutscene_CheckActorAction(play, 569)) {
        cueChannel = Cutscene_GetActorActionIndex(play, 569);

        if (this->unk_208 != play->csCtx.actorActions[cueChannel]->action) {
            this->unk_208 = play->csCtx.actorActions[cueChannel]->action;
            if (play->csCtx.actorActions[cueChannel]->action == 2) {
                ret = true;
            }
        }
    }

    return ret;
}

void DoorWarp1_EntranceWarp_WaitForCue(DoorWarp1* this, PlayState* play) {
    if (DoorWarp1_EntranceWarp_CheckForCue(this, play)) {
        DoorWarp1_SetupAction(this, DoorWarp1_EntranceWarp_Activate);
    }
}

void DoorWarp1_EntranceWarp_Activate(DoorWarp1* this, PlayState* play) {
    s32 pad[2];
    s16 sp2E = 0;
    f32 phi_f0 = 0.0f;

    if (play->sceneId == SCENE_MITURIN) {
        sp2E = -10;
        phi_f0 = -5.0f;
    } else if (play->sceneId == SCENE_HAKUGIN) {
        sp2E = -20;
    } else if (play->sceneId == SCENE_SEA) {
        sp2E = -20;
    }

    Math_SmoothStepToF(&this->beamScaleY, 40.0f, 0.5f, 1.2f + phi_f0, 0.01f);
    Math_SmoothStepToF(&this->unk_1A4, 0.0f, 0.5f, 12.0f, 0.1f);
    if ((this->unk_1A4 < 460.0f) && (this->unk_1A4 > 100.0f)) {
        func_808BB8D4(this, play, 6);
    }

    if (this->unk_1A4 < 0.1f) {
        this->unk_1D0 = sp2E + 30;
        this->activationBeamOn = false;
        DoorWarp1_SetupAction(this, DoorWarp1_EntranceWarp_Idle);
    }

    Actor_PlaySfxAtPos(&this->dyna.actor, NA_SE_EV_WARP_HOLE_ENERGY - SFX_FLAG);
}

void DoorWarp1_EntranceWarp_Idle(DoorWarp1* this, PlayState* play) {
    if (this->unk_1D0 != 0) {
        this->unk_1D0--;
        return;
    }

    Math_SmoothStepToF(&this->unk_1A8, 1.0f, 0.2f, 0.05f, 0.01f);
    this->unk_203 = (u8)(this->unk_1A8 * 200.0f);
    if (this->unk_1A8 > 0.8f) {
        func_808BB8D4(this, play, 1);
    }

    if (DoorWarp1_PlayerInRange(this, play) && !Play_InCsMode(play)) {
        func_800B7298(play, &this->dyna.actor, PLAYER_CSMODE_7);
        Message_StartTextbox(play, 0xF2, &this->dyna.actor);
        DoorWarp1_SetupAction(this, func_808B93A0);
    }

    Actor_PlaySfxAtPos(&this->dyna.actor, NA_SE_EV_BOSS_WARP_HOLE - SFX_FLAG);
}

void func_808B93A0(DoorWarp1* this, PlayState* play) {
    s32 pad;
    Player* player = GET_PLAYER(play);

    if ((Message_GetState(&play->msgCtx) == TEXT_STATE_CHOICE) && Message_ShouldAdvance(play)) {
        func_801477B4(play);
        if (play->msgCtx.choiceIndex == 0) {
            func_8019F208();
            func_800B7298(play, &this->dyna.actor, PLAYER_CSMODE_9);
            player->unk_3A0.x = this->dyna.actor.world.pos.x;
            player->unk_3A0.z = this->dyna.actor.world.pos.z;
            this->unk_1CA = 1;
            DoorWarp1_SetupAction(this, DoorWarp1_EntranceWarp_StartCutscene);
        } else {
            func_8019F230();
            func_800B7298(play, &this->dyna.actor, PLAYER_CSMODE_6);
            DoorWarp1_SetupAction(this, func_808B94A4);
        }
    }
    func_808BB8D4(this, play, 1);
    Actor_PlaySfxAtPos(&this->dyna.actor, NA_SE_EV_BOSS_WARP_HOLE - SFX_FLAG);
}

void func_808B94A4(DoorWarp1* this, PlayState* play) {
    if (!DoorWarp1_PlayerInRange(this, play) && (ActorCutscene_GetCurrentIndex() != play->playerActorCsIds[8])) {
        DoorWarp1_SetupAction(this, DoorWarp1_EntranceWarp_Idle);
    }
    func_808BB8D4(this, play, 1);
    Actor_PlaySfxAtPos(&this->dyna.actor, NA_SE_EV_BOSS_WARP_HOLE - SFX_FLAG);
}

void DoorWarp1_EntranceWarp_StartCutscene(DoorWarp1* this, PlayState* play) {
    if (!ActorCutscene_GetCanPlayNext(play->playerActorCsIds[9])) {
        ActorCutscene_SetIntentToPlay(play->playerActorCsIds[9]);
    } else {
        ActorCutscene_Start(play->playerActorCsIds[9], NULL);
        DoorWarp1_SetupAction(this, func_808B958C);
    }
}

void func_808B958C(DoorWarp1* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    func_808BB8D4(this, play, 1);

    if (this->unk_1CA > 100) {
        if (player->actor.velocity.y < 10.0f) {
            player->actor.gravity = 0.1f;
        } else {
            player->actor.gravity = 0.0f;
        }
    } else {
        this->unk_1CA++;
    }

    this->unk_1D0++;
    if ((this->unk_1D0 > 120) && (gSaveContext.nextCutsceneIndex == 0xFFEF)) {
        DoorWarp1_BossWarp_WarpOut(this, play);
        play->transitionType = TRANS_TYPE_FADE_WHITE;
        gSaveContext.nextTransitionType = TRANS_TYPE_FADE_WHITE;
    }

    Math_SmoothStepToF(&this->unk_1A8, 6.0f, 0.2f, 0.02f, 0.01f);
    Actor_PlaySfxAtPos(&this->dyna.actor, NA_SE_EV_LINK_WARP - SFX_FLAG);
}

void DoorWarp1_EntranceWarp_DoNothing(DoorWarp1* this, PlayState* play) {
}

void DoorWarp1_MoonWarp_Grow(DoorWarp1* this, PlayState* play) {
    Actor_PlaySfxAtPos(&this->dyna.actor, NA_SE_EV_WARP_HOLE - SFX_FLAG);
    Math_SmoothStepToF(&this->blueWarpPortalAlpha2, 255.0f, 0.4f, 10.0f, 0.01f);
    Math_SmoothStepToF(&this->blueWarpPortalAlpha1, 255.0f, 0.4f, 10.0f, 0.01f);

    if (this->scale < 100) {
        this->scale += 2;
    }

    if (this->unk_1C6 < 120) {
        this->unk_1C6 += 4;
    }

    if (this->unk_1C8 < 230) {
        this->unk_1C8 += 4;
    } else {
        DoorWarp1_SetupAction(this, DoorWarp1_MoonWarp_Idle);
    }
}

void DoorWarp1_MoonWarp_Idle(DoorWarp1* this, PlayState* play) {
    Actor_PlaySfxAtPos(&this->dyna.actor, NA_SE_EV_WARP_HOLE - SFX_FLAG);
    if (DoorWarp1_PlayerInRange(this, play) && !Play_InCsMode(play)) {
        Player* player = GET_PLAYER(play);

        AudioSfx_PlaySfx(NA_SE_EV_LINK_WARP, &player->actor.projectedPos, 4, &gSfxDefaultFreqAndVolScale,
                         &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
        func_800B7298(play, &this->dyna.actor, PLAYER_CSMODE_9);
        player->unk_3A0.x = this->dyna.actor.world.pos.x;
        player->unk_3A0.z = this->dyna.actor.world.pos.z;
        this->unk_1CA = 1;
        DoorWarp1_SetupAction(this, DoorWarp1_MoonWarp_StartCutscene);
    }
}

void DoorWarp1_MoonWarp_StartCutscene(DoorWarp1* this, PlayState* play) {
    if (!ActorCutscene_GetCanPlayNext(play->playerActorCsIds[9])) {
        ActorCutscene_SetIntentToPlay(play->playerActorCsIds[9]);
    } else {
        ActorCutscene_Start(play->playerActorCsIds[9], NULL);
        DoorWarp1_SetupAction(this, DoorWarp1_MoonWarp_WarpOut);
    }
}

void DoorWarp1_MoonWarp_WarpOut(DoorWarp1* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (this->unk_1CA > 100) {
        if (player->actor.velocity.y < 10.0f) {
            player->actor.gravity = 0.1f;
        } else {
            player->actor.gravity = 0.0f;
        }
    } else {
        this->unk_1CA++;
    }

    Math_SmoothStepToF(&this->blueWarpPortalAlpha2, 0.0f, 0.2f, 6.0f, 0.01f);

    this->unk_1D0++;
    if (D_808BC000 < this->unk_1D0) {
        if ((gSaveContext.nextCutsceneIndex == 0xFFEF) && (D_808BC000 < this->unk_1D0) &&
            (gSaveContext.nextCutsceneIndex == 0xFFEF)) {
            if (WARP_GET_TYPE00_1(&this->dyna.actor) != 0xFF) {
                play->nextEntrance = play->setupExitList[WARP_GET_TYPE00_3(&this->dyna.actor)];
                Scene_SetExitFade(play);
                play->transitionTrigger = TRANS_TRIGGER_START;
            } else {
                func_80169FDC(&play->state);
            }
        }
    }

    Math_StepToF(&this->unk_1A4, 2.0f, 0.01f);
    Math_StepToF(&this->unk_1A8, 10.0f, 0.02f);
    Lights_PointNoGlowSetInfo(&this->upperLightInfo, player->actor.world.pos.x + 10, player->actor.world.pos.y + 10,
                              player->actor.world.pos.z + 10, 235, 255, 255, 255);
    Lights_PointNoGlowSetInfo(&this->lowerLightInfo, player->actor.world.pos.x - 10, player->actor.world.pos.y - 10,
                              player->actor.world.pos.z - 10, 235, 255, 255, 255);
    Math_SmoothStepToF(&this->dyna.actor.shape.yOffset, 0.0f, 0.5f, 2.0f, 0.1f);
}

void DoorWarp1_BossWarp_WaitToGrow(DoorWarp1* this, PlayState* play) {
    if (fabsf(this->dyna.actor.xzDistToPlayer) >= 60.0f) {
        if (DoorWarp1_GetRemainsIndex(this, play) != 0) {
            this->bossRemains = (DmHina*)Actor_SpawnAsChild(&play->actorCtx, &this->dyna.actor, play, ACTOR_DM_HINA,
                                                            this->dyna.actor.world.pos.x, this->dyna.actor.world.pos.y,
                                                            this->dyna.actor.world.pos.z, 0, 0, 0,
                                                            DoorWarp1_GetRemainsIndex(this, play) - 1);
        }
        DoorWarp1_SetupAction(this, DoorWarp1_BossWarp_Grow);
    }
}

void DoorWarp1_BossWarp_Grow(DoorWarp1* this, PlayState* play) {
    Actor_PlaySfxAtPos(&this->dyna.actor, NA_SE_EV_WARP_HOLE - SFX_FLAG);
    Math_SmoothStepToF(&this->blueWarpPortalAlpha2, 255.0f, 0.2f, 2.0f, 0.1f);
    Math_SmoothStepToF(&this->blueWarpPortalAlpha1, 255.0f, 0.2f, 2.0f, 0.1f);
    if (this->scale < 10) {
        this->scale += 2;
    }

    if (this->unk_1C6 < 120) {
        this->unk_1C6 += 4;
    }

    if (this->unk_1C8 < 230) {
        this->unk_1C8 += 4;
        return;
    }

    this->dyna.actor.parent = NULL;
    if (DoorWarp1_GetRemainsIndex(this, play) != 0) {
        this->unk_202 = 1;
        DoorWarp1_SetupAction(this, DoorWarp1_BossWarp_IdleWithRemains);
    } else {
        DoorWarp1_SetupAction(this, DoorWarp1_BossWarp_IdleWithoutRemains);
    }
}

void DoorWarp1_BossWarp_IdleWithRemains(DoorWarp1* this, PlayState* play) {
    if (this->unk_203 != 0) {
        if (1) {}
        return;
    }

    if (!Actor_HasParent(&this->dyna.actor, play)) {
        Actor_PickUp(&this->dyna.actor, play, (GI_REMAINS_ODOLWA - 1) + DoorWarp1_GetRemainsIndex(this, play), 30.0f,
                     80.0f);
        return;
    }

    switch (play->sceneId) {
        case SCENE_MITURIN_BS:
            gSaveContext.save.unk_ECC[0] =
                (((void)0, gSaveContext.save.unk_ECC[0]) & 0xFFFFFF00) | (((u8)gSaveContext.save.unk_ECC[1]) & 0xFF);
            break;

        case SCENE_HAKUGIN_BS:
            gSaveContext.save.unk_ECC[0] = (((void)0, gSaveContext.save.unk_ECC[0]) & 0xFFFF00FF) |
                                           ((((u8)gSaveContext.save.unk_ECC[1]) & 0xFF) << 8);
            break;

        case SCENE_INISIE_BS:
            gSaveContext.save.unk_ECC[0] = (((void)0, gSaveContext.save.unk_ECC[0]) & 0xFF00FFFF) |
                                           ((((u8)gSaveContext.save.unk_ECC[1]) & 0xFF) << 0x10);
            break;

        case SCENE_SEA_BS:
            gSaveContext.save.unk_ECC[0] = (((void)0, gSaveContext.save.unk_ECC[0]) & 0x00FFFFFF) |
                                           ((((u8)gSaveContext.save.unk_ECC[1]) & 0xFF) << 0x18);
            break;
    }

    gSaveContext.save.unk_ECC[1] =
        (gSaveContext.save.unk_ECC[1] & 0xFFFFFF00) | ((((u8)gSaveContext.save.unk_ECC[1]) + 1) & 0xFF);
    Item_Give(play, (ITEM_REMAINS_ODOLWA - 1) + DoorWarp1_GetRemainsIndex(this, play));
    DoorWarp1_SetupAction(this, DoorWarp1_BossWarp_Message);
}

void DoorWarp1_BossWarp_Message(DoorWarp1* this, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == TEXT_STATE_CLOSING) {
        this->unk_1CE = 110;
        DoorWarp1_SetupAction(this, func_808B9ED8);
    }
}

void func_808B9ED8(DoorWarp1* this, PlayState* play) {
    this->unk_1CE--;
    if (this->unk_1CE <= 0) {
        DoorWarp1_BossWarp_WarpOut(this, play);
    }
}

void DoorWarp1_BossWarp_IdleWithoutRemains(DoorWarp1* this, PlayState* play) {
    Actor_PlaySfxAtPos(&this->dyna.actor, NA_SE_EV_WARP_HOLE - SFX_FLAG);
    if ((this->unk_203 == 0) && DoorWarp1_PlayerInRange(this, play) && !Play_InCsMode(play) && (this->unk_203 == 0)) {
        Player* player = GET_PLAYER(play);

        Interface_SetHudVisibility(HUD_VISIBILITY_NONE);
        func_800B7298(play, &this->dyna.actor, PLAYER_CSMODE_9);
        player->unk_3A0.x = this->dyna.actor.world.pos.x;
        player->unk_3A0.z = this->dyna.actor.world.pos.z;
        this->unk_1CA = 20;
        DoorWarp1_SetupAction(this, DoorWarp1_BossWarp_StartCutscene);
    }
}

void DoorWarp1_BossWarp_StartCutscene(DoorWarp1* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Player* player2 = GET_PLAYER(play);

    if (this->unk_1CA != 0) {
        this->unk_1CA--;
        return;
    }

    if (!ActorCutscene_GetCanPlayNext(play->playerActorCsIds[9])) {
        ActorCutscene_SetIntentToPlay(play->playerActorCsIds[9]);
    } else {
        ActorCutscene_Start(play->playerActorCsIds[9], NULL);
        AudioSfx_PlaySfx(NA_SE_EV_LINK_WARP, &player->actor.projectedPos, 4, &gSfxDefaultFreqAndVolScale,
                         &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
        Animation_ChangeImpl(&this->skelAnime, &gWarpCrystalAnim, 1.0f, Animation_GetLastFrame(&gWarpCrystalAnim),
                             Animation_GetLastFrame(&gWarpCrystalAnim), ANIMMODE_ONCE, 40.0f, ANIMTAPER_ACCEL);
        this->unk_1CA = 50;
        D_808BC004 = player2->actor.world.pos.y;
        DoorWarp1_SetupAction(this, func_808BA550);
    }
}

void DoorWarp1_BossWarp_WarpOut(DoorWarp1* this, PlayState* play) {
    s32 phi_v0_2;
    u8 phi_a0;
    s32 phi_v0_3;

    if ((play->sceneId == SCENE_MITURIN_BS) || (play->sceneId == SCENE_HAKUGIN_BS) ||
        (play->sceneId == SCENE_INISIE_BS) || (play->sceneId == SCENE_SEA_BS)) {
        D_801F4DE2 = play->sceneId;
        if (play->sceneId == SCENE_MITURIN_BS) {
            phi_v0_2 = 0;
        } else if (play->sceneId == SCENE_HAKUGIN_BS) {
            phi_v0_2 = 1;
        } else if (play->sceneId == SCENE_INISIE_BS) {
            phi_v0_2 = 2;
        } else if (play->sceneId == SCENE_SEA_BS) {
            phi_v0_2 = 3;
        } else {
            phi_v0_2 = 0;
        }

        if (this->unk_202 != 0) {
            if (phi_v0_2 > 0) {
                SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_WOODFALL_TEMPLE_PRISON);
            }

            switch (phi_v0_2) {
                case 0:
                    phi_a0 = gSaveContext.save.unk_ECC[0] & 0xFF;
                    break;

                case 1:
                    phi_a0 = (gSaveContext.save.unk_ECC[0] & 0xFF00) >> 8;
                    break;

                case 2:
                    phi_a0 = (gSaveContext.save.unk_ECC[0] & 0xFF0000) >> 0x10;
                    break;

                case 3:
                    phi_a0 = (gSaveContext.save.unk_ECC[0] & 0xFF000000) >> 0x18;
                    break;

                default:
                    phi_a0 = 0;
                    break;
            }

            switch (phi_a0) {
                case 0:
                    phi_v0_3 = 10;
                    break;

                case 1:
                    phi_v0_3 = 0;
                    break;

                case 2:
                    phi_v0_3 = 1;
                    break;

                case 3:
                    phi_v0_3 = 2;
                    break;

                default:
                    phi_v0_3 = 10;
                    break;
            }

            play->nextEntrance = ENTRANCE(GIANTS_CHAMBER, 0);
            if (phi_v0_3 < 10) {
                gSaveContext.nextCutsceneIndex = phi_v0_3 + 0xFFF0;
            }

            play->transitionTrigger = TRANS_TRIGGER_START;
            play->transitionType = TRANS_TYPE_FADE_WHITE;
            gSaveContext.nextTransitionType = TRANS_TYPE_FADE_WHITE;
        } else {
            switch (phi_v0_2) {
                case 0:
                    if (CHECK_WEEKEVENTREG(WEEKEVENTREG_CLEARED_WOODFALL_TEMPLE)) {
                        SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_WOODFALL_TEMPLE_PRISON);
                        play->nextEntrance = ENTRANCE(WOODFALL_TEMPLE, 1);
                        play->transitionTrigger = TRANS_TRIGGER_START;
                        play->transitionType = TRANS_TYPE_FADE_WHITE;
                        gSaveContext.nextTransitionType = TRANS_TYPE_FADE_WHITE;
                    } else {
                        play->nextEntrance = ENTRANCE(WOODFALL, 0);
                        gSaveContext.nextCutsceneIndex = 0xFFF0;
                        play->transitionTrigger = TRANS_TRIGGER_START;
                        play->transitionType = TRANS_TYPE_FADE_WHITE;
                        gSaveContext.nextTransitionType = TRANS_TYPE_FADE_WHITE;
                    }
                    break;

                case 1:
                    SET_WEEKEVENTREG(WEEKEVENTREG_CLEARED_SNOWHEAD_TEMPLE);
                    play->nextEntrance = ENTRANCE(MOUNTAIN_VILLAGE_SPRING, 7);
                    play->transitionTrigger = TRANS_TRIGGER_START;
                    play->transitionType = TRANS_TYPE_FADE_WHITE;
                    gSaveContext.nextTransitionType = TRANS_TYPE_FADE_WHITE;
                    break;

                case 3:
                    if (CHECK_WEEKEVENTREG(WEEKEVENTREG_CLEARED_GREAT_BAY_TEMPLE)) {
                        play->nextEntrance = ENTRANCE(ZORA_CAPE, 9);
                        gSaveContext.nextCutsceneIndex = 0xFFF0;
                        play->transitionTrigger = TRANS_TRIGGER_START;
                        play->transitionType = TRANS_TYPE_FADE_WHITE;
                        gSaveContext.nextTransitionType = TRANS_TYPE_FADE_WHITE;
                    } else {
                        SET_WEEKEVENTREG(WEEKEVENTREG_CLEARED_GREAT_BAY_TEMPLE);
                        play->nextEntrance = ENTRANCE(ZORA_CAPE, 8);
                        gSaveContext.nextCutsceneIndex = 0xFFF0;
                        play->transitionTrigger = TRANS_TRIGGER_START;
                        play->transitionType = TRANS_TYPE_FADE_WHITE;
                        gSaveContext.nextTransitionType = TRANS_TYPE_FADE_WHITE;
                    }
                    break;

                case 2:
                    SET_WEEKEVENTREG(WEEKEVENTREG_CLEARED_STONE_TOWER_TEMPLE);
                    play->nextEntrance = ENTRANCE(IKANA_CANYON, 15);
                    gSaveContext.nextCutsceneIndex = 0xFFF2;
                    play->transitionTrigger = TRANS_TRIGGER_START;
                    play->transitionType = TRANS_TYPE_FADE_WHITE;
                    gSaveContext.nextTransitionType = TRANS_TYPE_FADE_WHITE;
                    break;
            }
        }
    } else if (WARP_GET_TYPE00_1(&this->dyna.actor) != 0xFF) {
        if (WARP_GET_TYPE(&this->dyna.actor) == WARP_ENTRANCE_ACTIVE) {
            gSaveContext.respawnFlag = -2;
        }
        play->nextEntrance = play->setupExitList[WARP_GET_TYPE00_3(&this->dyna.actor)];
        Scene_SetExitFade(play);
        play->transitionTrigger = TRANS_TRIGGER_START;
    } else {
        func_80169FDC(&play->state);
    }
}

void func_808BA550(DoorWarp1* this, PlayState* play) {
    Player* player2 = GET_PLAYER(play);
    Player* player = GET_PLAYER(play);
    s16 i;
    s32 temp_f16;
    f32 temp_f0;
    f32 temp_f2;
    s32 temp;
    s32 tempS;

    if (this->unk_1CA != 0) {
        this->unk_1CA--;
    }

    if (this->unk_1CA < 0x1F) {
        temp_f0 = 0.0f;
        switch (player2->transformation) {
            case PLAYER_FORM_FIERCE_DEITY:
                temp_f0 = -37.0f;
                break;

            case PLAYER_FORM_GORON:
                temp_f0 = -37.0f;
                break;

            case PLAYER_FORM_ZORA:
                temp_f0 = -37.0f;
                break;

            case PLAYER_FORM_DEKU:
                temp_f0 = -20.0f;
                break;

            case PLAYER_FORM_HUMAN:
                temp_f0 = -25.0f;
                break;
        }

        temp_f2 = this->dyna.actor.world.pos.y + (this->dyna.actor.shape.yOffset * 0.1f) + temp_f0;

        if (D_808BC004 < temp_f2) {
            player->actor.world.pos.y = temp_f2;
        }

        if (this->unk_1CA <= 0) {
            if (this->unk_1CC < 4000) {
                this->unk_1CC += 40;
            }
            player->actor.world.rot.y -= this->unk_1CC;
            player->actor.shape.rot.y -= this->unk_1CC;
        }
        Math_SmoothStepToF(&player->actor.world.pos.x, this->dyna.actor.world.pos.x, 0.5f, 0.1f, 0.01f);
        Math_SmoothStepToF(&player->actor.world.pos.z, this->dyna.actor.world.pos.z, 0.5f, 0.1f, 0.01f);
    }

    this->unk_1D0++;
    if ((D_808BC000 < this->unk_1D0) && (gSaveContext.nextCutsceneIndex == 0xFFEF)) {
        DoorWarp1_BossWarp_WarpOut(this, play);
    }

    if (this->unk_1D0 > 140) {
        play->envCtx.fillScreen = true;
        temp_f0 = (this->unk_1D0 - 140) / 20.0f;
        if (temp_f0 > 1.0f) {
            temp_f0 = 1.0f;
        }
        play->envCtx.screenFillColor[0] = 160;
        play->envCtx.screenFillColor[1] = 160;
        play->envCtx.screenFillColor[2] = 160;
        play->envCtx.screenFillColor[3] = 255.0f * temp_f0;
    }

    Lights_PointNoGlowSetInfo(&this->upperLightInfo, player->actor.world.pos.x + 10.0f,
                              player->actor.world.pos.y + 10.0f, player->actor.world.pos.z + 10.0f, 235, 255, 255, 255);
    Lights_PointNoGlowSetInfo(&this->lowerLightInfo, player->actor.world.pos.x - 10.0f,
                              player->actor.world.pos.y - 10.0f, player->actor.world.pos.z - 10.0f, 235, 255, 255, 255);
    Math_SmoothStepToF(&this->dyna.actor.shape.yOffset, 800.0f, 0.2f, 15.0f, 0.01f);
    this->dyna.actor.shape.rot.y += 0x320;
    Math_SmoothStepToF(&this->unk_1BC, 1.13f, 0.2f, 0.1f, 0.01f);
    Math_StepToF(&this->unk_1A4, 2.0f, 0.003f);
    Math_StepToF(&this->unk_1A8, 10.0f, 0.006f);
    Math_SmoothStepToF(&this->blueWarpPortalAlpha2, 0.0f, 0.2f, 3.0f, 0.01f);
    Math_SmoothStepToF(&this->blueWarpPortalAlpha1, 0.0f, 0.2f, 2.0f, 0.01f);
    Math_SmoothStepToF(&this->unk_1B8, 255.0f, 0.1f, 1.0f, 0.01f);

    tempS = D_808BC000;
    if (1) {}
    temp_f0 = 1.0f - ((f32)(D_808BC000 - this->unk_1D0) / ((D_808BC000 - tempS) + 100));
    if (temp_f0 > 0.0f) {
        temp_f16 = -255.0f * temp_f0;

        for (i = 0; i < 3; i++) {
            play->envCtx.lightSettings.diffuseColor1[i] = temp_f16;
            play->envCtx.lightSettings.fogColor[i] = temp_f16;
            play->envCtx.lightSettings.ambientColor[i] = temp_f16;
        }

        play->envCtx.lightSettings.fogNear = -500.0f * temp_f0;
        if (play->envCtx.lightSettings.fogNear < -300) {
            play->roomCtx.curRoom.segment = NULL;
        }
    }
}

void DoorWarp1_EntrancePlatform_StartCutscene(DoorWarp1* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 cutscene;
    f32 xzRange;

    xzRange = 200.0f;
    if (play->sceneId == SCENE_SEA) {
        xzRange = 85.0f;
    }

    if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_WATCHED_ENTRANCE_WARP_CS) &&
        (fabsf(this->dyna.actor.xzDistToPlayer) < xzRange) &&
        ((player->actor.world.pos.y - 20.0f) < this->dyna.actor.world.pos.y) &&
        (this->dyna.actor.world.pos.y < (player->actor.world.pos.y + 20.0f))) {
        cutscene = this->dyna.actor.cutscene;

        if (ActorCutscene_GetCanPlayNext(cutscene)) {
            ActorCutscene_Start(cutscene, &this->dyna.actor);
            SET_WEEKEVENTREG(WEEKEVENTREG_WATCHED_ENTRANCE_WARP_CS);
            DoorWarp1_SetupAction(this, DoorWarp1_EntrancePlatform_DoNothing);
        } else {
            ActorCutscene_SetIntentToPlay(cutscene);
        }
    }
}

void DoorWarp1_EntrancePlatform_DoNothing(DoorWarp1* this, PlayState* play) {
}

void DoorWarp1_Update(Actor* thisx, PlayState* play) {
    DoorWarp1* this = THIS;

    if (this->unk_203 == 0) {
        this->unk_204 = 1.0f;
    }

    if ((this->bossRemains != NULL) && (this->bossRemains->unk_15C != this->unk_204)) {
        this->bossRemains->actor.world.pos.y = this->dyna.actor.world.pos.y;
        this->bossRemains->unk_158 = this->dyna.actor.world.pos.y;
        this->bossRemains->unk_15C = this->unk_204;
    }

    this->actionFunc(this, play);

    if (WARP_GET_TYPE(&this->dyna.actor) < WARP_ENTRANCE_WOODFALL_TEMPLE) {
        Actor_SetScale(&this->dyna.actor, this->scale / 100.0f);
    }
}

void func_808BACCC(DoorWarp1* this, PlayState* play) {
    s32 pad;

    OPEN_DISPS(play->state.gfxCtx);

    func_8012C2DC(play->state.gfxCtx);

    gDPSetPrimColor(POLY_XLU_DISP++, 0xFF, 0xFF, 200, 255, 255, (u8)this->unk_1B8);
    gDPSetEnvColor(POLY_XLU_DISP++, 0, 100, 255, (u8)this->unk_1B8);

    POLY_XLU_DISP = SkelAnime_Draw(play, this->skelAnime.skeleton, this->skelAnime.jointTable, NULL, NULL,
                                   &this->dyna.actor, POLY_XLU_DISP);

    CLOSE_DISPS(play->state.gfxCtx);

    SkelAnime_Update(&this->skelAnime);
}

void DoorWarp1_DrawBlueWarp(DoorWarp1* this, PlayState* play) {
    s32 pad;
    s32 pad2;
    s32 sp94 = play->state.frames * 10;
    f32 sp90;
    f32 sp8C;
    f32 phi_f12;
    f32 sp84;

    if (this->unk_1A4 >= 1.0f) {
        sp90 = 0.0f;
    } else {
        sp90 = 1.0f - this->unk_1A4;
    }

    if (this->unk_1A8 >= 1.0f) {
        sp8C = 0.0f;
    } else {
        sp8C = 1.0f - this->unk_1A8;
    }

    OPEN_DISPS(play->state.gfxCtx);

    sp84 = 1.0f - ((2.0f - this->unk_1A4) / 1.7f);
    this->blueWarpScroll += sp84 * 15.0f;
    func_8012C2DC(play->state.gfxCtx);

    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x80, 255.0f * sp84, 255, 255, (u8)this->blueWarpPortalAlpha1);
    gDPSetEnvColor(POLY_XLU_DISP++, 0, 255.0f * sp84, 255, 255);

    Matrix_Translate(this->dyna.actor.world.pos.x, this->dyna.actor.world.pos.y + 1.0f, this->dyna.actor.world.pos.z,
                     MTXMODE_NEW);
    phi_f12 = 1.0f;
    if (this->unk_203 != 0) {
        phi_f12 = this->unk_204 * phi_f12;
    }
    Matrix_Scale(phi_f12, phi_f12, phi_f12, MTXMODE_APPLY);

    gSPSegment(POLY_XLU_DISP++, 0x0A, Matrix_NewMtx(play->state.gfxCtx));
    Matrix_Push();
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, sp94 & 0xFF, -((s16)(2.0f * this->blueWarpScroll) & 0x1FF), 0x100,
                                0x100, 1, sp94 & 0xFF, -((s16)(2.0f * this->blueWarpScroll) & 0x1FF), 0x100, 0x100));

    Matrix_Translate(0.0f, this->unk_1A4 * 230.0f, 0.0f, MTXMODE_APPLY);
    Matrix_Scale(((this->unk_1C6 * sp90) / 100.0f) + 1.0f, 1.0f, ((this->unk_1C6 * sp90) / 100.0f) + 1.0f,
                 MTXMODE_APPLY);

    gSPSegment(POLY_XLU_DISP++, 0x09, Matrix_NewMtx(play->state.gfxCtx));
    gSPDisplayList(POLY_XLU_DISP++, gWarpPortalDL);

    Matrix_Pop();

    if (this->blueWarpPortalAlpha2 > 0.0f) {
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x80, 255.0f * sp84, 255, 255, (u8)this->blueWarpPortalAlpha2);
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 255.0f * sp84, 255, 255);

        sp94 *= 2;
        gSPSegment(POLY_XLU_DISP++, 0x08,
                   Gfx_TwoTexScroll(play->state.gfxCtx, 0, sp94 & 0xFF, -((s16)this->blueWarpScroll & 0x1FF), 0x100, 0x100, 1,
                                    sp94 & 0xFF, -((s16)this->blueWarpScroll & 0x1FF), 0x100, 0x100));

        Matrix_Translate(0.0f, this->unk_1A8 * 60.0f, 0.0f, MTXMODE_APPLY);
        Matrix_Scale(((this->unk_1C8 * sp8C) / 100.0f) + 1.0f, 1.0f, ((this->unk_1C8 * sp8C) / 100.0f) + 1.0f,
                     MTXMODE_APPLY);

        gSPSegment(POLY_XLU_DISP++, 0x09, Matrix_NewMtx(play->state.gfxCtx));
        gSPDisplayList(POLY_XLU_DISP++, gWarpPortalDL);
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

void DoorWarp1_DrawEntrancePlatform(DoorWarp1* this, PlayState* play) {
    Gfx_DrawDListOpa(play, gWarpBossWarpPlatformDL);
}

void DoorWarp1_DrawEntranceWarp(DoorWarp1* this, PlayState* play2) {
    PlayState* play = play2;
    s32 pad;
    Color_RGB8 sp64[] = {
        { 50, 150, 0 },
        { 100, 150, 200 },
        { 0, 50, 200 },
        { 120, 150, 0 },
    };
    s32 sp60 = 0;

    if (this->activationBeamOn) {
        Matrix_Translate(this->dyna.actor.world.pos.x, this->dyna.actor.world.pos.y + this->unk_1A4,
                         this->dyna.actor.world.pos.z, MTXMODE_NEW);
        Matrix_Scale(4.0f, this->beamScaleY, 4.0f, MTXMODE_APPLY);
        AnimatedMat_Draw(play, Lib_SegmentedToVirtual(gWarpBossWarpActivationBeamTexAnim));
        Gfx_DrawDListXlu(play, gWarpBossWarpActivationBeamDL);
        return;
    }

    if (play->sceneId != SCENE_MITURIN) {
        if (play->sceneId == SCENE_HAKUGIN) {
            sp60 = 1;
        } else if (play->sceneId == SCENE_SEA) {
            sp60 = 2;
        } else if (play->sceneId == SCENE_INISIE_R) {
            sp60 = 3;
        }
    }

    Matrix_Translate(this->dyna.actor.world.pos.x, this->dyna.actor.world.pos.y, this->dyna.actor.world.pos.z,
                     MTXMODE_NEW);
    Matrix_RotateYS(this->dyna.actor.world.rot.y, MTXMODE_APPLY);
    Matrix_Scale(1.0f, this->unk_1A8, 1.0f, MTXMODE_APPLY);
    AnimatedMat_Draw(play, Lib_SegmentedToVirtual(gWarpBossWarpLightShaftsTexAnim));

    OPEN_DISPS(play->state.gfxCtx);

    func_8012C2DC(play->state.gfxCtx);

    gDPSetEnvColor(POLY_XLU_DISP++, sp64[sp60].r, sp64[sp60].g, sp64[sp60].b, 255);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x80, 255, 255, 255, 255);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, gWarpBossWarpLightShaftsDL);

    CLOSE_DISPS(play->state.gfxCtx);

    OPEN_DISPS(play->state.gfxCtx);

    AnimatedMat_Draw(play, Lib_SegmentedToVirtual(gWarpBossWarpGlowTexAnim));
    Matrix_Translate(this->dyna.actor.world.pos.x, this->dyna.actor.world.pos.y, this->dyna.actor.world.pos.z,
                     MTXMODE_NEW);
    Matrix_RotateYS(this->dyna.actor.world.rot.y, MTXMODE_APPLY);
    Matrix_Scale(1.0f, 0.0f, 1.0f, MTXMODE_APPLY);

    func_8012C2DC(play->state.gfxCtx);

    gDPSetEnvColor(POLY_XLU_DISP++, sp64[sp60].r, sp64[sp60].g, sp64[sp60].b, 255);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x80, 255, 255, 255, this->unk_203);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, gWarpBossWarpGlowDL);

    CLOSE_DISPS(play->state.gfxCtx);
}

void DoorWarp1_Draw(Actor* thisx, PlayState* play) {
    DoorWarp1* this = THIS;

    switch (WARP_GET_TYPE(&this->dyna.actor)) {
        case WARP_BLUE_MOON:
            DoorWarp1_DrawBlueWarp(this, play);
            break;

        case WARP_BLUE_BOSS:
            func_808BACCC(this, play);
            DoorWarp1_DrawBlueWarp(this, play);
            break;

        case WARP_ENTRANCE_WOODFALL_TEMPLE:
        case WARP_ENTRANCE_SNOWHEAD_TEMPLE:
        case WARP_ENTRANCE_GREAT_BAY_TEMPLE:
        case WARP_ENTRANCE_STONE_TOWER_TEMPLE:
            DoorWarp1_DrawEntrancePlatform(this, play);
            break;

        case WARP_ENTRANCE_ACTIVE:
            DoorWarp1_DrawEntranceWarp(this, play);
            break;
    }
}

void func_808BB8D4(DoorWarp1* this, PlayState* play, s32 arg2) {
    static Vec3f sVelocity = { 0.0f, 0.05f, 0.0f };
    static Vec3f sAccel = { 0.0f, 0.25f, 0.0f };
    Vec3f pos;
    Color_RGBA8 primColor;
    Color_RGBA8 envColor;
    s32 i;
    s32 rand;
    s16 scale;
    s32 life;

    for (i = 0; i < arg2; i++) {
        rand = (Rand_ZeroOne() * 0x10000);

        pos.x = (-Math_SinS(rand - 0x8000) * 35.0f) + this->dyna.actor.world.pos.x;
        pos.y = this->dyna.actor.world.pos.y;
        pos.z = (Math_CosS(rand - 0x8000) * 35.0f) + this->dyna.actor.world.pos.z;

        primColor.r = 255;
        primColor.g = 255;
        primColor.b = 255;
        envColor.r = 200;
        envColor.g = 200;
        envColor.b = 255;

        sAccel.y = 0.25f;
        sVelocity.x = 0.0f;
        sVelocity.z = 0.0f;

        scale = (Rand_ZeroOne() * 1000.0f) + 800.0f;

        life = 27;
        if (arg2 >= 2) {
            life = 8;
            sAccel.y = 1.8f;
            sVelocity.x = -Math_SinS(rand - 0x8000) * 5.0f;
            sVelocity.z = Math_CosS(rand - 0x8000) * 5.0f;
        }
        EffectSsKirakira_SpawnDispersed(play, &pos, &sVelocity, &sAccel, &primColor, &envColor, scale, life);
    }
}
