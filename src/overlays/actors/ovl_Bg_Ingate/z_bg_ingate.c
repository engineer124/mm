/*
 * File: z_bg_ingate.c
 * Overlay: ovl_Bg_Ingate
 * Description: Swamp Tour Boat
 */

#include "z_bg_ingate.h"
#include "objects/object_sichitai_obj/object_sichitai_obj.h"

#define FLAGS (ACTOR_FLAG_10 | ACTOR_FLAG_20)

#define THIS ((BgIngate*)thisx)

void BgIngate_Init(Actor* thisx, PlayState* play2);
void BgIngate_Destroy(Actor* thisx, PlayState* play);
void BgIngate_Update(Actor* thisx, PlayState* play);
void BgIngate_Draw(Actor* thisx, PlayState* play);

void BgIngate_Action2(BgIngate* this, PlayState* play);
void BgIngate_ReturnToTouristInfo(BgIngate* this, PlayState* play);
void BgIngate_Action3(BgIngate* this, PlayState* play);
void BgIngate_ProcessText(BgIngate* this, PlayState* play);

ActorInit Bg_Ingate_InitVars = {
    ACTOR_BG_INGATE,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SICHITAI_OBJ,
    sizeof(BgIngate),
    (ActorFunc)BgIngate_Init,
    (ActorFunc)BgIngate_Destroy,
    (ActorFunc)BgIngate_Update,
    (ActorFunc)BgIngate_Draw,
};

/**
 * @brief Searches for an actor based on the parameters given to the function. Returns Actor* of actor found or NULL
 *
 * @param this
 * @param play
 * @param actorCat - Category of Actor
 * @param actorId - ID of actor to search for
 * @return Actor*
 */
Actor* BgIngate_FindActor(BgIngate* this, PlayState* play, u8 actorCat, s16 actorId) {
    Actor* foundActor = NULL;
    Actor* tempActor;

    while (true) {
        foundActor = SubS_FindActor(play, foundActor, actorCat, actorId);

        if ((foundActor == NULL) || (((this != (BgIngate*)foundActor)) && (foundActor->update != NULL))) {
            break;
        }

        tempActor = foundActor->next;
        if (tempActor == NULL) {
            foundActor = NULL;
            break;
        }
        foundActor = tempActor;
    }

    return foundActor;
}

void BgIngate_InitTimePath(BgIngate* this) {
    s32 numWaypoints;

    if (!CHECK_EVENTINF(EVENTINF_35)) {
        this->timePathTotalTime = 4 * 1000;
        this->timePathTimeSpeed = 4;
    } else {
        this->timePathTotalTime = 1 * 2000;
        this->timePathTimeSpeed = 1;
    }

    numWaypoints = this->timePath->count - (SUBS_TIME_PATHING_ORDER - 1);
    this->timePathWaypointTime = this->timePathTotalTime / numWaypoints;

    this->timePathWaypoint = SUBS_TIME_PATHING_ORDER - 1;
    this->timePathElapsedTime = 0;

    this->flags &= ~BGINGATE_FLAGS_TIME_PATH_IS_SETUP;
    this->flags &= ~BGINGATE_FLAGS_TIME_PATH_HAS_REACHED_END;
}

s32 BgIngate_FollowTimePath(BgIngate* this) {
    f32 knots[265];
    Vec3f worldPos;
    Vec3f timePathPointNew;
    Vec3f timePathTargetPos;
    s16 yaw;

    SubS_TimePathing_FillKnots(knots, SUBS_TIME_PATHING_ORDER, this->timePath->count + SUBS_TIME_PATHING_ORDER);
    if (!(this->flags & BGINGATE_FLAGS_TIME_PATH_IS_SETUP)) {
        timePathTargetPos = gZeroVec3f;
        SubS_TimePathing_Update(this->timePath, &this->timePathProgress, &this->timePathElapsedTime,
                                this->timePathWaypointTime, this->timePathTotalTime, &this->timePathWaypoint, knots,
                                &timePathTargetPos, this->timePathTimeSpeed);
        this->flags |= BGINGATE_FLAGS_TIME_PATH_IS_SETUP;
    } else {
        timePathTargetPos = this->timePathTargetPos;
    }

    this->dyna.actor.world.pos.x = timePathTargetPos.x;
    this->dyna.actor.world.pos.z = timePathTargetPos.z;

    this->timePathTargetPos = gZeroVec3f;
    if (SubS_TimePathing_Update(this->timePath, &this->timePathProgress, &this->timePathElapsedTime,
                                this->timePathWaypointTime, this->timePathTotalTime, &this->timePathWaypoint, knots,
                                &this->timePathTargetPos, this->timePathTimeSpeed)) {
        this->flags |= BGINGATE_FLAGS_TIME_PATH_HAS_REACHED_END;
    } else {
        worldPos = this->dyna.actor.world.pos;
        timePathPointNew = this->timePathTargetPos;
        yaw = Math_Vec3f_Yaw(&worldPos, &timePathPointNew);
        this->dyna.actor.world.rot.y = yaw;
        this->dyna.actor.shape.rot.y = yaw;
    }

    return false;
}

s32 func_80953DA8(BgIngate* this, PlayState* play) {
    Camera* mainCam = Play_GetCamera(play, CAM_ID_MAIN);

    if (CHECK_EVENTINF(EVENTINF_35)) {
        func_800B7298(play, &this->dyna.actor, PLAYER_CSMODE_7);
    } else {
        SET_EVENTINF(EVENTINF_41);
    }
    Camera_ChangeSetting(mainCam, CAM_SET_BOAT_CRUISE);
    play->unk_1887C = 99;

    return false;
}

void func_80953E38(PlayState* play) {
    Camera_ChangeSetting(Play_GetCamera(play, CAM_ID_MAIN), CAM_SET_NORMAL0);

    if (!CHECK_EVENTINF(EVENTINF_35)) {
        CLEAR_EVENTINF(EVENTINF_41);
    }

    play->unk_1887C = -1;
}

void BgIngate_SetupAction3(BgIngate* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    func_800B7298(play, &this->dyna.actor, PLAYER_CSMODE_58);
    player->unk_3A0.x = this->dyna.actor.world.pos.x;
    player->unk_3A0.z = this->dyna.actor.world.pos.z;
    this->flags &= ~BGINGATE_FLAGS_4;
    this->timer = 30;
    this->actionFunc = BgIngate_Action3;
}

void BgIngate_SetupAction2(BgIngate* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    player->actor.shape.rot.y = this->dyna.actor.shape.rot.y;
    player->actor.world.rot.y = player->actor.shape.rot.y;
    player->currentYaw = player->actor.shape.rot.y;
    player->actor.focus.rot.y = player->actor.shape.rot.y;
    this->flags |= BGINGATE_FLAGS_10;
    func_80953DA8(this, play);
    if (this->timePath != NULL) {
        BgIngate_InitTimePath(this);
    }
    this->actorCsId = -1;
    this->actionFunc = BgIngate_Action2;
}

void BgIngate_DoNothing(BgIngate* this, PlayState* play) {
}

void BgIngate_Action2(BgIngate* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Camera* mainCam = Play_GetCamera(play, CAM_ID_MAIN);

    if (!CHECK_EVENTINF(EVENTINF_BOAT_CRUISE_MINIGAME_FINISHED)) {

        if (!CHECK_EVENTINF(EVENTINF_35) && (this->flags & BGINGATE_FLAGS_10) && (this->unk16C == 0)) {
            this->dyna.actor.textId = 0x9E3;
            Message_StartTextbox(play, this->dyna.actor.textId, NULL);
            this->flags &= ~BGINGATE_FLAGS_10;
        }

        if (this->flags & BGINGATE_FLAGS_TIME_PATH_HAS_REACHED_END) {
            if (this->timePath->unk1 != 0xFF) {
                func_80953E38(play);
                func_800B7298(play, &this->dyna.actor, PLAYER_CSMODE_7);
                this->dyna.actor.textId = 0x9E4;
                Message_StartTextbox(play, this->dyna.actor.textId, NULL);
                this->unk16C++;
                SET_WEEKEVENTREG(WEEKEVENTREG_90_40);
                this->actionFunc = BgIngate_ProcessText;
            } else {
                if (!CHECK_EVENTINF(EVENTINF_35)) {
                    CLEAR_EVENTINF(EVENTINF_41);
                } else {
                    SET_EVENTINF(EVENTINF_BOAT_CRUISE_MINIGAME_FINISHED);
                }
                this->actionFunc = BgIngate_ReturnToTouristInfo;
            }
        } else if ((ActorCutscene_GetCurrentIndex() == -1) && (this->timePath != NULL)) {
            Actor_PlaySfxAtPos(&this->dyna.actor, NA_SE_EV_CRUISER - SFX_FLAG);
            BgIngate_FollowTimePath(this);
        }
    }

    if (ActorCutscene_GetCurrentIndex() != this->actorCsId) {
        if (ActorCutscene_GetCurrentIndex() != -1) {
            Camera_ChangeSetting(mainCam, CAM_SET_NORMAL0);
            player->stateFlags1 |= PLAYER_STATE1_20;
            play->actorCtx.flags &= ~ACTORCTX_FLAG_2;
        } else {
            Camera_ChangeSetting(mainCam, CAM_SET_BOAT_CRUISE);
            player->stateFlags1 &= ~PLAYER_STATE1_20;
        }
    }

    this->actorCsId = ActorCutscene_GetCurrentIndex();
}

void BgIngate_Action0(BgIngate* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (this->flags & BGINGATE_FLAGS_4) {
        if ((player->transformation == PLAYER_FORM_HUMAN) && (player->actor.bgCheckFlags & 1) &&
            (this->dyna.actor.xzDistToPlayer < 40.0f)) {
            if (this->dyna.actor.playerHeightRel > 15.0f) {
                func_800B7298(play, &this->dyna.actor, PLAYER_CSMODE_7);
                this->dyna.actor.textId = 0x9E6;
                Message_StartTextbox(play, this->dyna.actor.textId, NULL);
                this->actionFunc = BgIngate_ProcessText;
            }
        }
    } else if (!DynaPolyActor_IsInRidingMovingState(&this->dyna)) {
        this->flags |= BGINGATE_FLAGS_4;
    }
}

void BgIngate_ReturnToTouristInfo(BgIngate* this, PlayState* play) {
    if (CHECK_EVENTINF(EVENTINF_50)) {
        play->nextEntrance = ENTRANCE(TOURIST_INFORMATION, 2);
        CLEAR_EVENTINF(EVENTINF_50);
    } else {
        play->nextEntrance = ENTRANCE(TOURIST_INFORMATION, 1);
    }
    gSaveContext.nextCutsceneIndex = 0;
    play->transitionTrigger = TRANS_TRIGGER_START;
    play->transitionType = TRANS_TYPE_03;
    gSaveContext.nextTransitionType = TRANS_TYPE_03;
    this->actionFunc = BgIngate_DoNothing;
    CLEAR_WEEKEVENTREG(WEEKEVENTREG_90_40);
    func_800FE498();
}

void BgIngate_Action3(BgIngate* this, PlayState* play) {
    if (!DECR(this->timer)) {
        if (this->timePath != NULL) {
            func_800B7298(play, &this->dyna.actor, PLAYER_CSMODE_6);
            this->timePath = &play->setupPathList[this->timePath->unk1];
            BgIngate_SetupAction2(this, play);
            func_800FE484();
        }
    }
}

void BgIngate_ProcessText(BgIngate* this, PlayState* play) {
    u8 talkState = Message_GetState(&play->msgCtx);

    if (((talkState == TEXT_STATE_CHOICE) || (talkState == TEXT_STATE_5)) && Message_ShouldAdvance(play)) {
        switch (this->dyna.actor.textId) {
            case 0x9E4:
                this->dyna.actor.textId = 0x9E5;
                func_80151938(play, this->dyna.actor.textId);
                break;
            case 0x9E5:
                if (play->msgCtx.choiceIndex == 0) {
                    func_800B7298(play, &this->dyna.actor, PLAYER_CSMODE_6);
                    this->flags &= ~BGINGATE_FLAGS_4;
                    this->actionFunc = BgIngate_Action0;
                    func_800FE498();
                    func_8019F208();
                } else {
                    if (this->timePath != NULL) {
                        this->timePath = &play->setupPathList[this->timePath->unk1];
                    }
                    BgIngate_SetupAction2(this, play);
                    CLEAR_WEEKEVENTREG(WEEKEVENTREG_90_40);
                    func_8019F230();
                }
                func_801477B4(play);
                break;
            case 0x9E6:
                if (play->msgCtx.choiceIndex == 0) {
                    BgIngate_SetupAction3(this, play);
                    CLEAR_WEEKEVENTREG(WEEKEVENTREG_90_40);
                    func_8019F208();
                } else {
                    func_800B7298(play, &this->dyna.actor, PLAYER_CSMODE_6);
                    this->flags &= ~BGINGATE_FLAGS_4;
                    this->actionFunc = BgIngate_Action0;
                    func_800FE498();
                    func_8019F230();
                }
                func_801477B4(play);
                break;
        }
    }
}

void BgIngate_Init(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    BgIngate* this = THIS;
    s32 searchTimePathLimit;
    Vec3s* timePathPoints;
    Vec3f sp2C;
    Vec3f sp20;

    if (BgIngate_FindActor(this, play, ACTORCAT_BG, ACTOR_BG_INGATE) == NULL) {
        DynaPolyActor_Init(&this->dyna, 3);
        DynaPolyActor_LoadMesh(play, &this->dyna, &gSwampBoatCol);
        this->flags = 0;
        this->flags |= BGINGATE_FLAGS_8;
        this->flags |= BGINGATE_FLAGS_10;
        Actor_SetScale(&this->dyna.actor, 1.0f);
        this->timePath = SubS_GetAdditionalPath(play, BGINGATE_GET_FF(&this->dyna.actor), 0);
        this->dyna.actor.room = -1;
        if (CHECK_WEEKEVENTREG(WEEKEVENTREG_20_02)) {
            CLEAR_WEEKEVENTREG(WEEKEVENTREG_90_40);
        }
        if (!CHECK_EVENTINF(EVENTINF_35) && CHECK_WEEKEVENTREG(WEEKEVENTREG_90_40)) {
            searchTimePathLimit = 1;
            this->unk16C = 1;
            this->actionFunc = BgIngate_Action0;
        } else {
            searchTimePathLimit = 0;
            if (play->curSpawn == 6) {
                BgIngate_SetupAction2(this, play);
                if (CHECK_EVENTINF(EVENTINF_35)) {
                    func_80112AFC(play);
                } else {
                    SET_EVENTINF(EVENTINF_41);
                }
            } else {
                this->actionFunc = BgIngate_DoNothing;
            }
        }

        this->timePath = SubS_GetAdditionalPath(play, BGINGATE_GET_FF(&this->dyna.actor), searchTimePathLimit);
        if (this->timePath != NULL) {
            timePathPoints = Lib_SegmentedToVirtual(this->timePath->points);
            Math_Vec3s_ToVec3f(&sp2C, &timePathPoints[0]);
            Math_Vec3s_ToVec3f(&sp20, &timePathPoints[1]);
            this->dyna.actor.world.rot.y = Math_Vec3f_Yaw(&sp2C, &sp20);
            this->dyna.actor.shape.rot.y = this->dyna.actor.world.rot.y;
            this->dyna.actor.world.pos.x = sp2C.x;
            this->dyna.actor.world.pos.y = -15.0f;
            this->dyna.actor.world.pos.z = sp2C.z;
        }
        this->timePath = SubS_GetAdditionalPath(play, BGINGATE_GET_FF(&this->dyna.actor), 0);
    } else {
        Actor_Kill(&this->dyna.actor);
    }
}

void BgIngate_Destroy(Actor* thisx, PlayState* play) {
    BgIngate* this = THIS;

    if (this->flags & BGINGATE_FLAGS_8) {
        DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
    }
}

void BgIngate_Update(Actor* thisx, PlayState* play) {
    BgIngate* this = THIS;

    this->actionFunc(this, play);
}

void BgIngate_Draw(Actor* thisx, PlayState* play) {
    OPEN_DISPS(play->state.gfxCtx);

    func_8012C28C(play->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, gSwampBoatDL);

    CLOSE_DISPS(play->state.gfxCtx);
}
