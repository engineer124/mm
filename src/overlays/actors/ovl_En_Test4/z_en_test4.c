/*
 * File: z_en_test4.c
 * Overlay: ovl_En_Test4
 * Description: Three-Day Clock
 */

#include "z_en_test4.h"
#include "overlays/gamestates/ovl_daytelop/z_daytelop.h"
#include "overlays/actors/ovl_En_Horse/z_en_horse.h"

#define FLAGS (ACTOR_FLAG_10 | ACTOR_FLAG_20 | ACTOR_FLAG_100000)

#define THIS ((EnTest4*)thisx)

void EnTest4_Init(Actor* thisx, PlayState* play);
void EnTest4_Destroy(Actor* thisx, PlayState* play);
void EnTest4_Update(Actor* thisx, PlayState* play);

void EnTest4_Action(EnTest4* this, PlayState* play);
void EnTest4_Cutscene(EnTest4* this, PlayState* play);

ActorInit En_Test4_InitVars = {
    ACTOR_EN_TEST4,
    ACTORCAT_SWITCH,
    FLAGS,
    GAMEPLAY_KEEP,
    sizeof(EnTest4),
    (ActorFunc)EnTest4_Init,
    (ActorFunc)EnTest4_Destroy,
    (ActorFunc)EnTest4_Update,
    (ActorFunc)NULL,
};

static s32 sIsLoaded = false;
// "Night of ..."
static s16 sNightOfTextIds1[] = { 0x1BB4, 0x1BB5, 0x1BB6 };
// "Dawn of ..." (Note: first two message are the same)
static s16 sDawnOfTextIds1[] = { 0x1BB2, 0x1BB2, 0x1BB3 };
// "Night of ..."
static s16 sNightOfTextIds2[] = { 0x1BB4, 0x1BB5, 0x1BB6 };
// "Dawn of ..." (Note: first two message are the same)
static s16 sDawnOfTextIds2[] = { 0x1BB2, 0x1BB2, 0x1BB3 };
static u16 D_80A43364[] = { CLOCK_TIME(6, 0), CLOCK_TIME(18, 0) };

static s16 sCutscenes[2];
static s16 sCurrentCs;

void EnTest4_DayNightTransitionFromInit(EnTest4* this, PlayState* play) {
    if (this->dayNightIndex != NIGHT_INDEX) {
        func_80151A68(play, sNightOfTextIds1[CURRENT_DAY - 1]);
    } else if ((sCutscenes[this->dayNightIndex] < 0) || (play->actorCtx.flags & ACTORCTX_FLAG_TELESCOPE_ON)) {
        // Increment day without a cutscene
        if (play->actorCtx.flags & ACTORCTX_FLAG_TELESCOPE_ON) {
            Sram_IncrementDay();
            gSaveContext.save.time = CLOCK_TIME(6, 0);
            func_80151A68(play, sDawnOfTextIds1[CURRENT_DAY - 1]);
        } else {
            this->dayNightIndex = NIGHT_INDEX;
            this->unk_146 = gSaveContext.save.time += CLOCK_TIME_MINUTE;
        }

        Interface_NewDay(play, CURRENT_DAY);
        sSceneSeqState = SCENESEQ_MORNING;
        Environment_PlaySceneSequence(play);
        func_800FEAF4(&play->envCtx);
        this->actionFunc = EnTest4_Action;
    }

    if (gSaveContext.cutsceneTrigger == 0) {
        if ((sCutscenes[this->dayNightIndex] >= 0) && !(play->actorCtx.flags & ACTORCTX_FLAG_TELESCOPE_ON)) {
            // Start cutscene
            this->actionFunc = EnTest4_Cutscene;
            sCurrentCs = sCutscenes[this->dayNightIndex];
            this->transitionCsTimer = 0;
            gSaveContext.eventInf[1] |= 0x80;
        } else if (this->dayNightIndex == NIGHT_INDEX) {
            // Previously night, turning day
            play_sound(NA_SE_EV_CHICKEN_CRY_M);
        } else {
            // Previously day, turning Night
            func_8019F128(NA_SE_EV_DOG_CRY_EVENING);
        }
    } else {
        this->actionFunc = EnTest4_Action;
        // Update to the new dayNightIndex
        if (this->dayNightIndex == NIGHT_INDEX) {
            this->dayNightIndex = DAY_INDEX;
        } else {
            this->dayNightIndex = NIGHT_INDEX;
        }

        this->unk_146 = gSaveContext.save.time += CLOCK_TIME_MINUTE;
    }
}

void EnTest4_DayNightTransitionFromUpdate(EnTest4* this, PlayState* play) {
    if (this->dayNightIndex != NIGHT_INDEX) {
        func_80151A68(play, sNightOfTextIds2[CURRENT_DAY - 1]);
    } else if ((sCutscenes[this->dayNightIndex] < 0) || (play->actorCtx.flags & ACTORCTX_FLAG_TELESCOPE_ON)) {
        // Increment day without a cutscene
        Sram_IncrementDay();
        gSaveContext.save.time = CLOCK_TIME(6, 0);
        Interface_NewDay(play, CURRENT_DAY);
        func_80151A68(play, sDawnOfTextIds2[CURRENT_DAY - 1]);
        sSceneSeqState = SCENESEQ_MORNING;
        Environment_PlaySceneSequence(play);
        func_800FEAF4(&play->envCtx);
        this->actionFunc = EnTest4_Action;
    }

    if (gSaveContext.cutsceneTrigger == 0) {
        if ((sCutscenes[this->dayNightIndex] >= 0) && !(play->actorCtx.flags & ACTORCTX_FLAG_TELESCOPE_ON)) {
            // Start cutscene
            this->actionFunc = EnTest4_Cutscene;
            sCurrentCs = sCutscenes[this->dayNightIndex];
            this->transitionCsTimer = 0;
            gSaveContext.eventInf[1] |= 0x80;
        } else if (this->dayNightIndex == NIGHT_INDEX) {
            // Previously night, turning day
            play_sound(NA_SE_EV_CHICKEN_CRY_M);
        } else {
            // Previously day, turning Night
            func_8019F128(NA_SE_EV_DOG_CRY_EVENING);
        }
    } else {
        this->actionFunc = EnTest4_Action;
        // Update to the new dayNightIndex
        if (this->dayNightIndex == NIGHT_INDEX) {
            this->dayNightIndex = DAY_INDEX;
        } else {
            this->dayNightIndex = NIGHT_INDEX;
        }

        this->unk_146 = gSaveContext.save.time += CLOCK_TIME_MINUTE;
    }
}

void EnTest4_Day3Bells(EnTest4* this) {
    if ((gSaveContext.save.time >= CLOCK_TIME(6, 0)) && (gSaveContext.save.time <= CLOCK_TIME(18, 0))) {
        if (gSaveContext.save.time < CLOCK_TIME(17, 30)) {
            this->nextBellTime = CLOCK_TIME(17, 30);
        } else if (gSaveContext.save.time < CLOCK_TIME(17, 36)) {
            this->nextBellTime = CLOCK_TIME(17, 36);
        } else if (gSaveContext.save.time < CLOCK_TIME(17, 42)) {
            this->nextBellTime = CLOCK_TIME(17, 42);
        } else if (gSaveContext.save.time < CLOCK_TIME(17, 48)) {
            this->nextBellTime = CLOCK_TIME(17, 48);
        } else if (gSaveContext.save.time < CLOCK_TIME(17, 54)) {
            this->nextBellTime = CLOCK_TIME(17, 54);
        } else {
            this->nextBellTime = CLOCK_TIME(0, 0);
        }
    } else if (gSaveContext.save.time > CLOCK_TIME(6, 0)) {
        this->nextBellTime = CLOCK_TIME(0, 0);
    } else if (gSaveContext.save.time < CLOCK_TIME(0, 10)) {
        this->nextBellTime = CLOCK_TIME(0, 10);
    } else if (gSaveContext.save.time < CLOCK_TIME(0, 20)) {
        this->nextBellTime = CLOCK_TIME(0, 20);
    } else if (gSaveContext.save.time < CLOCK_TIME(0, 30)) {
        this->nextBellTime = CLOCK_TIME(0, 30);
    } else if (gSaveContext.save.time < CLOCK_TIME(0, 40)) {
        this->nextBellTime = CLOCK_TIME(0, 40);
    } else if (gSaveContext.save.time < CLOCK_TIME(0, 50)) {
        this->nextBellTime = CLOCK_TIME(0, 50);
    } else if (gSaveContext.save.time < CLOCK_TIME(1, 0)) {
        this->nextBellTime = CLOCK_TIME(1, 0);
    } else if (gSaveContext.save.time < CLOCK_TIME(1, 10)) {
        this->nextBellTime = CLOCK_TIME(1, 10);
    } else if (gSaveContext.save.time < CLOCK_TIME(1, 20)) {
        this->nextBellTime = CLOCK_TIME(1, 20);
    } else if (gSaveContext.save.time < CLOCK_TIME(1, 30) - 1) {
        this->nextBellTime = CLOCK_TIME(1, 30) - 1;
    } else if (gSaveContext.save.time < CLOCK_TIME(1, 40) - 1) {
        this->nextBellTime = CLOCK_TIME(1, 40) - 1;
    } else if (gSaveContext.save.time < CLOCK_TIME(1, 50) - 1) {
        this->nextBellTime = CLOCK_TIME(1, 50) - 1;
    } else if (gSaveContext.save.time < CLOCK_TIME(2, 0)) {
        this->nextBellTime = CLOCK_TIME(2, 0);
    } else if (gSaveContext.save.time < CLOCK_TIME(2, 10)) {
        this->nextBellTime = CLOCK_TIME(2, 10);
    } else if (gSaveContext.save.time < CLOCK_TIME(2, 20)) {
        this->nextBellTime = CLOCK_TIME(2, 20);
    } else if (gSaveContext.save.time < CLOCK_TIME(2, 30)) {
        this->nextBellTime = CLOCK_TIME(2, 30);
    } else if (gSaveContext.save.time < CLOCK_TIME(2, 40)) {
        this->nextBellTime = CLOCK_TIME(2, 40);
    } else if (gSaveContext.save.time < CLOCK_TIME(2, 50)) {
        this->nextBellTime = CLOCK_TIME(2, 50);
    } else if (gSaveContext.save.time < CLOCK_TIME(3, 0)) {
        this->nextBellTime = CLOCK_TIME(3, 0);
    } else if (gSaveContext.save.time < CLOCK_TIME(3, 10)) {
        this->nextBellTime = CLOCK_TIME(3, 10);
    } else if (gSaveContext.save.time < CLOCK_TIME(3, 20)) {
        this->nextBellTime = CLOCK_TIME(3, 20);
    } else if (gSaveContext.save.time < CLOCK_TIME(3, 30)) {
        this->nextBellTime = CLOCK_TIME(3, 30);
    } else if (gSaveContext.save.time < CLOCK_TIME(3, 40)) {
        this->nextBellTime = CLOCK_TIME(3, 40);
    } else if (gSaveContext.save.time < CLOCK_TIME(3, 50)) {
        this->nextBellTime = CLOCK_TIME(3, 50);
    } else if (gSaveContext.save.time < CLOCK_TIME(4, 0)) {
        this->nextBellTime = CLOCK_TIME(4, 0);
    } else if (gSaveContext.save.time < CLOCK_TIME(4, 10)) {
        this->nextBellTime = CLOCK_TIME(4, 10);
    } else if (gSaveContext.save.time < CLOCK_TIME(4, 20)) {
        this->nextBellTime = CLOCK_TIME(4, 20);
    } else if (gSaveContext.save.time < CLOCK_TIME(4, 30) - 1) {
        this->nextBellTime = CLOCK_TIME(4, 30) - 1;
    } else if (gSaveContext.save.time < CLOCK_TIME(4, 40) - 1) {
        this->nextBellTime = CLOCK_TIME(4, 40) - 1;
    } else if (gSaveContext.save.time < CLOCK_TIME(4, 50) - 1) {
        this->nextBellTime = CLOCK_TIME(4, 50) - 1;
    } else if (gSaveContext.save.time < CLOCK_TIME(5, 0)) {
        this->nextBellTime = CLOCK_TIME(5, 0);
    } else if (gSaveContext.save.time < CLOCK_TIME(5, 5)) {
        this->nextBellTime = CLOCK_TIME(5, 5);
    } else if (gSaveContext.save.time < CLOCK_TIME(5, 10)) {
        this->nextBellTime = CLOCK_TIME(5, 10);
    } else if (gSaveContext.save.time < CLOCK_TIME(5, 15) - 1) {
        this->nextBellTime = CLOCK_TIME(5, 15) - 1;
    } else if (gSaveContext.save.time < CLOCK_TIME(5, 20)) {
        this->nextBellTime = CLOCK_TIME(5, 20);
    } else if (gSaveContext.save.time < CLOCK_TIME(5, 25) - 1) {
        this->nextBellTime = CLOCK_TIME(5, 25) - 1;
    } else if (gSaveContext.save.time < CLOCK_TIME(5, 30)) {
        this->nextBellTime = CLOCK_TIME(5, 30);
    } else if (gSaveContext.save.time < CLOCK_TIME(5, 33) - 1) {
        this->nextBellTime = CLOCK_TIME(5, 33) - 1;
    } else if (gSaveContext.save.time < CLOCK_TIME(5, 36)) {
        this->nextBellTime = CLOCK_TIME(5, 36);
    } else if (gSaveContext.save.time < CLOCK_TIME(5, 39) - 1) {
        this->nextBellTime = CLOCK_TIME(5, 39) - 1;
    } else if (gSaveContext.save.time < CLOCK_TIME(5, 42)) {
        this->nextBellTime = CLOCK_TIME(5, 42);
    } else if (gSaveContext.save.time < CLOCK_TIME(5, 45)) {
        this->nextBellTime = CLOCK_TIME(5, 45);
    } else if (gSaveContext.save.time < CLOCK_TIME(5, 48)) {
        this->nextBellTime = CLOCK_TIME(5, 48);
    } else if (gSaveContext.save.time < CLOCK_TIME(5, 51)) {
        this->nextBellTime = CLOCK_TIME(5, 51);
    } else if (gSaveContext.save.time < CLOCK_TIME(5, 54)) {
        this->nextBellTime = CLOCK_TIME(5, 54);
    } else if (gSaveContext.save.time < CLOCK_TIME(5, 57)) {
        this->nextBellTime = CLOCK_TIME(5, 57);
    } else if (gSaveContext.save.time < CLOCK_TIME(6, 0)) {
        this->nextBellTime = CLOCK_TIME(6, 0);
    }
}

/**
 * Gets next bell time and shrinks screen near the end of the day
 */
void EnTest4_Day1Day2Bells(EnTest4* this, PlayState* play) {
    gSaveContext.screenScale = 1000.0f;

    if ((gSaveContext.save.time >= CLOCK_TIME(6, 0)) && (gSaveContext.save.time < CLOCK_TIME(18, 0))) {
        if (gSaveContext.save.time < CLOCK_TIME(17, 30)) {
            this->nextBellTime = CLOCK_TIME(17, 30);
        } else if (gSaveContext.save.time < CLOCK_TIME(17, 36)) {
            this->nextBellTime = CLOCK_TIME(17, 36);
        } else if (gSaveContext.save.time < CLOCK_TIME(17, 42)) {
            this->nextBellTime = CLOCK_TIME(17, 42);
        } else if (gSaveContext.save.time < CLOCK_TIME(17, 48)) {
            this->nextBellTime = CLOCK_TIME(17, 48);
        } else if (gSaveContext.save.time < CLOCK_TIME(17, 54)) {
            this->nextBellTime = CLOCK_TIME(17, 54);
        } else {
            this->nextBellTime = CLOCK_TIME(5, 30);
        }
    } else {
        if (gSaveContext.save.time < CLOCK_TIME(5, 30)) {
            this->nextBellTime = CLOCK_TIME(5, 30);
        } else if (gSaveContext.save.time < CLOCK_TIME(5, 36)) {
            this->nextBellTime = CLOCK_TIME(5, 36);
        } else if (gSaveContext.save.time < CLOCK_TIME(5, 42)) {
            this->nextBellTime = CLOCK_TIME(5, 42);
            gSaveContext.screenScale -= 50.0f;
        } else if (gSaveContext.save.time < CLOCK_TIME(5, 48)) {
            this->nextBellTime = CLOCK_TIME(5, 48);
            gSaveContext.screenScale -= 100.0f;
        } else if (gSaveContext.save.time < CLOCK_TIME(5, 54)) {
            this->nextBellTime = CLOCK_TIME(5, 54);
            gSaveContext.screenScale -= 150.0f;
        } else if (gSaveContext.save.time < CLOCK_TIME(6, 0)) {
            this->nextBellTime = CLOCK_TIME(17, 30);
            gSaveContext.screenScale -= 200.0f;
        } else {
            this->nextBellTime = CLOCK_TIME(17, 30);
        }

        if ((sCutscenes[this->dayNightIndex] < 0) || (play->actorCtx.flags & ACTORCTX_FLAG_TELESCOPE_ON) ||
            (CURRENT_DAY == 3) || (gSaveContext.save.time >= CLOCK_TIME(17, 0))) {
            gSaveContext.screenScale = 1000.0f;
        }
        if (gSaveContext.screenScale != 1000.0f) {
            gSaveContext.screenScaleFlag = true;
        }
    }
}

void EnTest4_Init(Actor* thisx, PlayState* play) {
    s32 dayTemp;
    EnTest4* this = THIS;
    Player* player = GET_PLAYER(play);
    s8 actorCsId = this->actor.cutscene;

    sCutscenes[0] = actorCsId;
    if (actorCsId >= 0) {
        ActorCutscene* actorCutscene = ActorCutscene_GetCutscene(sCutscenes[0]);

        gSaveContext.eventInf[5] |= 0x4;
        sCutscenes[1] = actorCutscene->additionalCutscene;
    } else {
        gSaveContext.eventInf[5] &= (u8)~0x4;
        sCutscenes[1] = sCutscenes[0];
    }

    if (sIsLoaded || (gSaveContext.eventInf[2] & 0x80)) {
        Actor_Kill(&this->actor);
    } else {
        sIsLoaded = true;
        this->actor.room = -1;
        gSaveContext.screenScaleFlag = false;
        gSaveContext.screenScale = 1000.0f;

        if (CURRENT_DAY == 0) {
            if (gSaveContext.save.time < CLOCK_TIME(6, 1)) {
                gSaveContext.save.time = CLOCK_TIME(6, 0);
                gSaveContext.gameMode = 0;
                STOP_GAMESTATE(&play->state);
                SET_NEXT_GAMESTATE(&play->state, DayTelop_Init, sizeof(DayTelopState));
                this->dayNightIndex = DAY_INDEX;
                gSaveContext.save.time = CLOCK_TIME(6, 0);
                Actor_Kill(&this->actor);
            } else {
                gSaveContext.save.day = 1;
                dayTemp = gSaveContext.save.day;
                gSaveContext.save.daysElapsed = dayTemp;
                this->dayNightIndex = DAY_INDEX;
                this->unk_146 = gSaveContext.save.time;
                this->actionFunc = EnTest4_Action;
            }
        } else if (gSaveContext.save.time == CLOCK_TIME(6, 0)) {
            this->dayNightIndex = NIGHT_INDEX;
            EnTest4_DayNightTransitionFromInit(this, play);
            if ((gSaveContext.cutsceneTrigger == 0) && (sCutscenes[this->dayNightIndex] >= 0) &&
                !(play->actorCtx.flags & ACTORCTX_FLAG_TELESCOPE_ON)) {
                player->stateFlags1 |= PLAYER_STATE1_200;
            }
        } else {
            if ((gSaveContext.save.time > CLOCK_TIME(18, 0)) || (gSaveContext.save.time < CLOCK_TIME(6, 0))) {
                this->dayNightIndex = NIGHT_INDEX;
            } else {
                this->dayNightIndex = DAY_INDEX;
            }
            this->unk_146 = gSaveContext.save.time;
            this->actionFunc = EnTest4_Action;
        }
    }

    if (CURRENT_DAY == 3) {
        EnTest4_Day3Bells(this);
    } else {
        EnTest4_Day1Day2Bells(this, play);
    }

    this->lastBellTime = gSaveContext.save.time;
    if ((sCutscenes[this->dayNightIndex] < 0) || (play->actorCtx.flags & ACTORCTX_FLAG_TELESCOPE_ON)) {
        gSaveContext.screenScaleFlag = false;
        gSaveContext.screenScale = 1000.0f;
    }
}

void EnTest4_Destroy(Actor* thisx, PlayState* play) {
}

void EnTest4_Action(EnTest4* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if ((play->transitionMode == TRANS_MODE_OFF) && !Play_InCsMode(play) && (play->numSetupActors <= 0) &&
        (play->roomCtx.unk31 == 0) && !Play_IsDebugCamEnabled()) {
        s16 temp_a2;
        u16 temp_a0 = D_80A43364[this->dayNightIndex];
        s16 temp_a3;
        s16 bellDiff;
        s16 new_var;

        temp_a3 = gSaveContext.save.time - temp_a0;
        temp_a2 = this->unk_146 - temp_a0;

        bellDiff = this->lastBellTime - this->nextBellTime;
        new_var = gSaveContext.save.time - this->nextBellTime;

        if ((temp_a3 * temp_a2) <= 0) {
            gSaveContext.unk_3CA7 = 1;
            if (play->actorCtx.flags & ACTORCTX_FLAG_PICTOGRAPH_ON) {
                play->actorCtx.flags &= ~ACTORCTX_FLAG_PICTOGRAPH_ON;
            }

            if (temp_a0 != CLOCK_TIME(6, 0)) {
                EnTest4_DayNightTransitionFromUpdate(this, play);
            } else if (temp_a0 == CLOCK_TIME(6, 0)) {
                if (CURRENT_DAY == 3) {
                    Interface_StartMoonCrash(play);
                    Actor_Kill(&this->actor);
                    gSaveContext.eventInf[1] |= 0x80;
                } else if (((sCutscenes[this->dayNightIndex] < 0) ||
                            (play->actorCtx.flags & ACTORCTX_FLAG_TELESCOPE_ON)) &&
                           (CURRENT_DAY != 3)) {
                    EnTest4_DayNightTransitionFromUpdate(this, play);
                } else {
                    gSaveContext.screenScale = 0.0f;
                    Play_SetRespawnData(&play->state, RESPAWN_MODE_DOWN, Entrance_CreateFromSpawn(0), player->unk_3CE,
                                        0xBFF, &player->unk_3C0, player->unk_3CC);
                    func_80169EFC(&play->state);
                    if (player->stateFlags1 & PLAYER_STATE1_800000) {
                        EnHorse* rideActor = (EnHorse*)player->rideActor;

                        if ((rideActor->type == HORSE_TYPE_EPONA) || (rideActor->type == HORSE_TYPE_2)) {
                            if (CURRENT_DAY < 3) {
                                D_801BDA9C = 1;
                            } else {
                                D_801BDA9C = 0;
                            }
                        }
                    }

                    gSaveContext.respawnFlag = -4;
                    gSaveContext.eventInf[2] |= 0x80;
                    Actor_Kill(&this->actor);
                }
            }

            if ((sCutscenes[this->dayNightIndex] >= 0) && !(play->actorCtx.flags & ACTORCTX_FLAG_TELESCOPE_ON)) {
                player->stateFlags1 |= PLAYER_STATE1_200;
                this->unk_146 = gSaveContext.save.time;
            } else {
                if (this->dayNightIndex == NIGHT_INDEX) {
                    this->dayNightIndex = DAY_INDEX;
                } else {
                    this->dayNightIndex = NIGHT_INDEX;
                }

                this->unk_146 = gSaveContext.save.time += CLOCK_TIME_MINUTE;
            }
        } else if ((new_var * bellDiff) <= 0) {
            Audio_PlaySfx_BigBells(&this->actor.projectedPos, THREE_DAY_CLOCK_GET_BELL_VOLUME_INDEX(&this->actor));
            this->lastBellTime = gSaveContext.save.time;

            if (CURRENT_DAY == 3) {
                if ((this->nextBellTime == CLOCK_TIME(0, 0)) &&
                    ((gSaveContext.save.inventory.items[SLOT_OCARINA] == ITEM_NONE) ||
                     (play->sceneId == SCENE_CLOCKTOWER))) {
                    s32 playerParams;
                    u32 entrance = gSaveContext.save.entrance;

                    if ((play->actorCtx.flags & ACTORCTX_FLAG_TELESCOPE_ON)) {
                        playerParams = 0xCFF;
                    } else {
                        playerParams = 0xBFF;
                    }
                    Play_SetRespawnData(&play->state, RESPAWN_MODE_RETURN, entrance, player->unk_3CE, playerParams,
                                        &player->unk_3C0, player->unk_3CC);

                    if ((play->sceneId == SCENE_TENMON_DAI) || (play->sceneId == SCENE_00KEIKOKU)) {
                        play->nextEntrance = ENTRANCE(TERMINA_FIELD, 0);
                    } else {
                        play->nextEntrance = ENTRANCE(SOUTH_CLOCK_TOWN, 0);
                    }
                    gSaveContext.nextCutsceneIndex = 0xFFF1;
                    play->transitionTrigger = TRANS_TRIGGER_START;
                    play->transitionType = TRANS_TYPE_02;
                    player->stateFlags1 |= PLAYER_STATE1_200;
                    Actor_Kill(&this->actor);
                }
                EnTest4_Day3Bells(this);
            } else {
                EnTest4_Day1Day2Bells(this, play);
            }
        }
    }
}

void EnTest4_Cutscene(EnTest4* this, PlayState* play) {
    if (!this->transitionCsTimer) {
        if (sCurrentCs >= 0) {
            if (ActorCutscene_GetCanPlayNext(sCurrentCs) == 0) {
                ActorCutscene_SetIntentToPlay(sCurrentCs);
            } else {
                ActorCutscene_Start(sCurrentCs, &this->actor);
                this->transitionCsTimer = 1;
            }
        } else {
            this->transitionCsTimer = 1;
        }
    } else if (this->transitionCsTimer < 60) {
        this->transitionCsTimer++;
        if (this->transitionCsTimer == 10) {
            if (this->dayNightIndex == NIGHT_INDEX) {
                play_sound(NA_SE_EV_CHICKEN_CRY_M);
            } else {
                func_8019F128(NA_SE_EV_DOG_CRY_EVENING);
            }
        }
        if (this->transitionCsTimer == 60) {
            Player* player = GET_PLAYER(play);

            gSaveContext.save.time += CLOCK_TIME_MINUTE;
            this->unk_146 = gSaveContext.save.time;
            play->numSetupActors = -play->numSetupActors;
            player->stateFlags1 &= ~PLAYER_STATE1_200;
        }
    } else {
        this->actionFunc = EnTest4_Action;
        if (this->dayNightIndex == NIGHT_INDEX) {
            this->dayNightIndex = DAY_INDEX;
        } else {
            this->dayNightIndex = NIGHT_INDEX;
        }

        if (sCurrentCs >= 0) {
            ActorCutscene_Stop(sCurrentCs);
        }
        gSaveContext.hudVisibility = HUD_VISIBILITY_IDLE;
        gSaveContext.eventInf[1] &= (u8)~0x80;
        Interface_SetHudVisibility(HUD_VISIBILITY_ALL);
    }
}

void EnTest4_UpdateClearSkies(EnTest4* this, PlayState* play) {
    if ((CURRENT_DAY == 2) && (gSaveContext.save.time >= CLOCK_TIME(7, 0)) &&
        (gSaveContext.save.time < CLOCK_TIME(17, 30)) && (play->envCtx.precipitation[2] == 0)) {
        // Raining
        gWeatherMode = 1;
        Environment_PlayStormNatureAmbience(play);
        play->envCtx.lightningState = LIGHTNING_ON;
        play->envCtx.precipitation[0] = 60;
    } else if ((play->envCtx.precipitation[0] != 0) && ((play->state.frames % 4) == 0)) {
        play->envCtx.precipitation[0]--;
        if ((play->envCtx.precipitation[0]) == 8) {
            Environment_StopStormNatureAmbience(play);
        }
    }

    if (gWeatherMode == 1) {
        this->state = TEST4_STATE_RAIN;
    }
}

void EnTest4_UpdateRainySkies(EnTest4* this, PlayState* play) {
    if (((gSaveContext.save.time >= CLOCK_TIME(17, 30)) && (gSaveContext.save.time < CLOCK_TIME(23, 0)) &&
         (play->envCtx.precipitation[0] != 0)) ||
        (play->envCtx.precipitation[2] != 0)) {
        gWeatherMode = 0;
        play->envCtx.lightningState = LIGHTNING_LAST;
    }

    if (gWeatherMode == 0) {
        this->state = TEST4_STATE_CLEAR;
    }
}

void EnTest4_SetSkyboxNumStars(EnTest4* this, PlayState* play) {
    s32 skyboxNumStars = THREE_DAY_CLOCK_GET_SKYBOX_STAR_COUNT(&this->actor) * 100;

    if (skyboxNumStars > 0) {
        gSkyboxNumStars = skyboxNumStars;
    }
}

void EnTest4_Update(Actor* thisx, PlayState* play) {
    EnTest4* this = THIS;
    Player* player = GET_PLAYER(play);

    if (!(player->stateFlags1 & PLAYER_STATE1_2)) {
        this->actionFunc(this, play);

        if (func_800FE4B8(play)) {
            switch (this->state) {
                case TEST4_STATE_CLEAR:
                    EnTest4_UpdateClearSkies(this, play);
                    break;

                case TEST4_STATE_RAIN:
                    EnTest4_UpdateRainySkies(this, play);
                    break;
            }
        }
        EnTest4_SetSkyboxNumStars(this, play);
    }
}
