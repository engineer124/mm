/*
 * File: z_obj_nozoki.c
 * Overlay: ovl_Obj_Nozoki
 * Description: Sakon's Hideout Objects (Sun's Mask, doors, etc)
 */

#include "z_obj_nozoki.h"
#include "objects/object_secom_obj/object_secom_obj.h"

#define FLAGS (ACTOR_FLAG_10)

#define THIS ((ObjNozoki*)thisx)

void ObjNozoki_Init(Actor* thisx, PlayState* play);
void ObjNozoki_Destroy(Actor* thisx, PlayState* play);
void ObjNozoki_Update(Actor* thisx, PlayState* play);

void ObjNozoki_SetupAction(ObjNozoki* this, ObjNozokiActionFunc actionFunc);
void ObjNozoki_SakonActionSetup(ObjNozoki* this, PlayState* play);
void ObjNozoki_Door_Wait(ObjNozoki* this, PlayState* play);
void ObjNozoki_Door_Open(ObjNozoki* this, PlayState* play);
void ObjNozoki_Door_Close(ObjNozoki* this, PlayState* play);
void ObjNozoki_SunMask_Wait(ObjNozoki* this, PlayState* play);
void ObjNozoki_SunMask_Conveyor(ObjNozoki* this, PlayState* play);
void ObjNozoki_BackShutter_Action(ObjNozoki* this, PlayState* play);
void ObjNozoki_FrontShutter_Action(ObjNozoki* this, PlayState* play);
void ObjNozoki_Curiosity_Action1(ObjNozoki* this, PlayState* play);
void ObjNozoki_Curiosity_Action2(ObjNozoki* this, PlayState* play);
void ObjNozoki_Draw(Actor* thisx, PlayState* play);

typedef enum {
    /* 0 */ SUNMASK_CONVEYOR_SPEED_DEFAULT,
    /* 1 */ SUNMASK_CONVEYOR_SPEED_SLOW,
    /* 2 */ SUNMASK_CONVEYOR_SPEED_FAST,
} SunMaskConveyorSpeed;

s32 sHasSakonTrialBegun;
s32 sActorCsId;
f32 D_80BA36B8;

ActorInit Obj_Nozoki_InitVars = {
    ACTOR_OBJ_NOZOKI,
    ACTORCAT_ITEMACTION,
    FLAGS,
    GAMEPLAY_KEEP,
    sizeof(ObjNozoki),
    (ActorFunc)ObjNozoki_Init,
    (ActorFunc)ObjNozoki_Destroy,
    (ActorFunc)ObjNozoki_Update,
    (ActorFunc)NULL, // SAKON_HIDEOUT_CURIOSITY doesn't draw
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F(scale, 1, ICHAIN_CONTINUE),
    ICHAIN_U8(targetMode, 0, ICHAIN_STOP),
};

s16 D_80BA34B8[] = { OBJECT_SECOM_OBJ, OBJECT_GI_MSSA, OBJECT_SECOM_OBJ, OBJECT_SECOM_OBJ };

Vec3f D_80BA34C0 = { 0.0f, 0.0f, -1110.0f };

Vec3f D_80BA34CC = { 0.0f, 0.0f, 38.0f };

f32 D_80BA34D8[] = { 100.0f, 9999.0f, 9999.0f };

f32 sSunMaskConveyorSpeeds[] = { 0.5f, 0.25f, 1.0f };

void ObjNozoki_SetupAction(ObjNozoki* this, ObjNozokiActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void ObjNozoki_Init(Actor* thisx, PlayState* play) {
    ObjNozoki* this = THIS;

    Actor_ProcessInitChain(&this->dyna.actor, sInitChain);
    this->dyna.actor.shape.rot.x = 0;
    this->dyna.actor.shape.rot.z = 0;
    this->actorCsId = this->dyna.actor.cutscene;

    if (play->sceneId == SCENE_AYASHIISHOP) {
        // Curiosity Shop
        this->type = SAKON_HIDEOUT_CURIOSITY;
        ObjNozoki_SetupAction(this, ObjNozoki_Curiosity_Action1);
        this->dyna.actor.colChkInfo.cylRadius = -40;
    } else {
        // Sakon's Hideout
        this->type = OBJNOZOKI_GET_TYPE(&this->dyna.actor);
        if (this->type == SAKON_HIDEOUT_DOOR) {
            DynaPolyActor_Init(&this->dyna, 0);
        }
        ObjNozoki_SetupAction(this, ObjNozoki_SakonActionSetup);
    }
}

void ObjNozoki_Destroy(Actor* thisx, PlayState* play) {
    ObjNozoki* this = THIS;

    if (this->type == SAKON_HIDEOUT_DOOR) {
        DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
    }
}

void ObjNozoki_SakonActionSetup(ObjNozoki* this, PlayState* play) {
    s32 objBankIndex = Object_GetIndex(&play->objectCtx, D_80BA34B8[this->type]);

    if (objBankIndex < 0) {
        Actor_Kill(&this->dyna.actor);
        return;
    }

    if (Object_IsLoaded(&play->objectCtx, objBankIndex)) {
        this->dyna.actor.objBankIndex = objBankIndex;
        this->dyna.actor.draw = ObjNozoki_Draw;

        if (this->type == SAKON_HIDEOUT_DOOR) {
            Actor_SetObjectDependency(play, &this->dyna.actor);
            DynaPolyActor_LoadMesh(play, &this->dyna, &object_secom_obj_Colheader_0001C0);
            if (ActorCutscene_GetAdditionalCutscene(this->actorCsId) >= 0) {
                this->dyna.actor.params |= OBJNOZOKI_400;
            }
            ObjNozoki_SetupAction(this, ObjNozoki_Door_Wait);
        } else if (this->type == SAKON_HIDEOUT_MASK) {
            Lib_Vec3f_TranslateAndRotateY(&this->dyna.actor.world.pos, this->dyna.actor.shape.rot.y, &D_80BA34C0,
                                          &this->dyna.actor.home.pos);
            Actor_SetScale(&this->dyna.actor, 0.6f);
            this->dyna.actor.flags |= ACTOR_FLAG_20;
            ObjNozoki_SetupAction(this, ObjNozoki_SunMask_Wait);
        } else if (this->type == SAKON_HIDEOUT_BACK_SHUTTER) {
            Lib_Vec3f_TranslateAndRotateY(&this->dyna.actor.home.pos, this->dyna.actor.shape.rot.y, &D_80BA34CC,
                                          &this->dyna.actor.focus.pos);
            Math_Vec3f_Copy(&this->dyna.actor.world.pos, &this->dyna.actor.focus.pos);
            ObjNozoki_SetupAction(this, ObjNozoki_BackShutter_Action);
        } else { // SAKON_HIDEOUT_FRONT_SHUTTER
            ObjNozoki_SetupAction(this, ObjNozoki_FrontShutter_Action);
        }
    }
}

/* DOOR */

s32 ObjNozoki_Door_TryCutscene(ObjNozoki* this) {
    if (this->actorCsId < 0) {
        return true;
    }

    if (ActorCutscene_GetCanPlayNext(this->actorCsId)) {
        ActorCutscene_StartAndSetUnkLinkFields(this->actorCsId, &this->dyna.actor);
        return true;
    }

    ActorCutscene_SetIntentToPlay(this->actorCsId);
    return false;
}

s32 ObjNozoki_Door_AreEnemiesCleared(ObjNozoki* this, PlayState* play) {
    Actor* enemy = play->actorCtx.actorLists[ACTORCAT_ENEMY].first;
    Vec3f sp30;

    while (enemy != NULL) {
        Actor_OffsetOfPointInActorCoords(&this->dyna.actor, &sp30, &enemy->world.pos);
        if (sp30.z >= 0.0f) {
            return false;
        }
        enemy = enemy->next;
    }

    return true;
}

void ObjNozoki_Door_SetupOpen(ObjNozoki* this) {
    ObjNozoki_SetupAction(this, ObjNozoki_Door_Open);
    this->dyna.actor.velocity.y = 0.0f;
    this->timer = 0;
}

void ObjNozoki_Door_Wait(ObjNozoki* this, PlayState* play) {
    if (!(play->actorCtx.flags & ACTORCTX_FLAG_5)) {
        if (OBJNOZOKI_GET_200(&this->dyna.actor)) {
            // Player puzzle: defeat the enemies
            if (!ObjNozoki_Door_AreEnemiesCleared(this, play)) {
                // Enemies not defeated yet
                return;
            }
        } else {
            // Kafei puzzle: step on the switch
            if (sHasSakonTrialBegun) {
                play->actorCtx.flags |= ACTORCTX_FLAG_7;
            }

            if (!Flags_GetSwitch(play, OBJNOZOKI_GET_SWITCHFLAG1(&this->dyna.actor))) {
                // Switch not stepped on yet
                return;
            }
        }

        // Puzzle was a success!
        if (!sHasSakonTrialBegun || ObjNozoki_Door_TryCutscene(this)) {
            ObjNozoki_Door_SetupOpen(this);
            if (!sHasSakonTrialBegun) {
                this->timer = 25;
                play_sound(NA_SE_SY_SECOM_WARNING);
            } else {
                this->timer = ActorCutscene_GetLength(this->actorCsId);
                if (this->timer < 0) {
                    this->timer = 50;
                }
                play->actorCtx.flags |= ACTORCTX_FLAG_4;
            }
        }
        GET_PLAYER(play)->linearVelocity = 0.0f;
    }
}

void ObjNozoki_Door_Open(ObjNozoki* this, PlayState* play) {
    if (this->timer != 0) {
        if (DECR(this->timer) == 0) {
            Actor_PlaySfxAtPos(&this->dyna.actor, NA_SE_EV_SLIDE_DOOR_OPEN);
        }
        return;
    }

    if (!(play->actorCtx.flags & ACTORCTX_FLAG_5)) {
        Math_StepToF(&this->dyna.actor.velocity.y, 15.0f, 3.0f);
        Math_StepToF(&this->dyna.actor.world.pos.y, this->dyna.actor.home.pos.y + 200.0f, this->dyna.actor.velocity.y);

        if (OBJNOZOKI_GET_200(&this->dyna.actor)) {
            if (!Flags_GetSwitch(play, OBJNOZOKI_GET_SWITCHFLAG1(&this->dyna.actor))) {
                return;
            }
        } else if (Flags_GetSwitch(play, OBJNOZOKI_GET_SWITCHFLAG1(&this->dyna.actor))) {
            s32 actorCsId = this->dyna.actor.cutscene;

            if (actorCsId == this->actorCsId) {
                if (OBJNOZOKI_GET_400(&this->dyna.actor)) {
                    Vec3f offset;

                    Actor_OffsetOfPointInActorCoords(&this->dyna.actor, &offset, &GET_PLAYER(play)->actor.world.pos);
                    if (offset.z < -20.0f) {
                        this->actorCsId = ActorCutscene_GetAdditionalCutscene(this->actorCsId);
                    }
                }
            } else if (sActorCsId == 0) {
                if (ObjNozoki_Door_TryCutscene(this)) {
                    sActorCsId = 1;
                }
            } else if (ActorCutscene_GetCurrentIndex() != this->actorCsId) {
                this->actorCsId = actorCsId;
                this->dyna.actor.params &= ~OBJNOZOKI_400;
                Audio_QueueSeqCmd(NA_BGM_ENEMY | 0x8800);
            }
            return;
        }
    }

    ObjNozoki_SetupAction(this, ObjNozoki_Door_Close);
    this->dyna.actor.velocity.y = 0.0f;
    Actor_PlaySfxAtPos(&this->dyna.actor, NA_SE_EV_SLIDE_DOOR_CLOSE);
}

void ObjNozoki_Door_Close(ObjNozoki* this, PlayState* play) {
    this->dyna.actor.velocity.y = 30.0f;

    if (Math_StepToF(&this->dyna.actor.world.pos.y, this->dyna.actor.home.pos.y, 30.0f) && !sHasSakonTrialBegun) {
        ObjNozoki_SetupAction(this, ObjNozoki_Door_Wait);
        sHasSakonTrialBegun = true;
    }

    if (!(play->actorCtx.flags & ACTORCTX_FLAG_5)) {
        if (!(OBJNOZOKI_GET_200(&this->dyna.actor)) &&
            Flags_GetSwitch(play, OBJNOZOKI_GET_SWITCHFLAG1(&this->dyna.actor))) {
            ObjNozoki_Door_SetupOpen(this);
        }
    }
}

/* SUNS MASK */

void ObjNozoki_SunMask_Set(ObjNozoki* this, PlayState* play, s32 sunMaskConveyorSpeed, s32 arg3) {
    this->sunMaskConveyorSpeed = sunMaskConveyorSpeed;
    this->timer = 80;
    Flags_UnsetSwitch(play, arg3);
}

/**
 * Wait for Kafei to step on the first switch to trigger Sakon's Trial
 */
void ObjNozoki_SunMask_Wait(ObjNozoki* this, PlayState* play) {
    if (!Flags_GetSwitch(play, OBJNOZOKI_GET_SWITCHFLAG1(&this->dyna.actor))) {
        // Waiting for the switch
        this->dyna.actor.shape.rot.x = -0x1F40;
        this->timer = 80;
    } else if (DECR(this->timer) == 0) {
        ObjNozoki_SetupAction(this, ObjNozoki_SunMask_Conveyor);
    }
}

s32 ObjNozoki_SunMask_GetCutscene(ObjNozoki* this) {
    s32 i;
    s16 actorCsId = this->actorCsId;

    for (i = 0; i < 3; i++) {
        if (ActorCutscene_GetCurrentIndex() == actorCsId) {
            return i;
        }
        actorCsId = ActorCutscene_GetAdditionalCutscene(actorCsId);
    }

    return -1;
}

void ObjNozoki_SunMask_Conveyor(ObjNozoki* this, PlayState* play) {
    s32 actorCsId = ObjNozoki_SunMask_GetCutscene(this);
    f32 sp38;
    f32 sp34;
    f32 speedXZ;

    if (actorCsId >= 0) {
        sActorCsId = actorCsId;
    }

    if ((actorCsId < 0) && Play_InCsMode(play)) {
        return;
    }

    if (D_80BA36B8 < D_80BA34D8[sActorCsId]) {
        sp38 = this->dyna.actor.home.pos.y - this->dyna.actor.world.pos.y;

        if ((this->sunMaskConveyorSpeed != SUNMASK_CONVEYOR_SPEED_SLOW) &&
            Flags_GetSwitch(play, this->dyna.actor.world.rot.z)) {
            ObjNozoki_SunMask_Set(this, play, SUNMASK_CONVEYOR_SPEED_SLOW, this->dyna.actor.world.rot.x);
        } else if ((this->sunMaskConveyorSpeed != SUNMASK_CONVEYOR_SPEED_FAST) &&
                   Flags_GetSwitch(play, this->dyna.actor.world.rot.x)) {
            ObjNozoki_SunMask_Set(this, play, SUNMASK_CONVEYOR_SPEED_FAST, this->dyna.actor.world.rot.z);
        } else if (DECR(this->timer) == 0) {
            this->sunMaskConveyorSpeed = SUNMASK_CONVEYOR_SPEED_DEFAULT;
            Flags_UnsetSwitch(play, this->dyna.actor.world.rot.z);
            Flags_UnsetSwitch(play, this->dyna.actor.world.rot.x);
        }

        Math_StepToF(&this->dyna.actor.speedXZ, sSunMaskConveyorSpeeds[this->sunMaskConveyorSpeed], 0.1f);

        if ((play->actorCtx.flags & ACTORCTX_FLAG_6) || (play->actorCtx.flags & ACTORCTX_FLAG_5)) {
            speedXZ = 0.5f;
        } else {
            speedXZ = this->dyna.actor.speedXZ;
        }

        sp34 = Math_Vec3f_StepToXZ(&this->dyna.actor.world.pos, &this->dyna.actor.home.pos, speedXZ);

        D_80BA36B8 += this->dyna.actor.speedXZ;

        if (play->actorCtx.flags & ACTORCTX_FLAG_6) {
            if (sp34 <= 5.0f) {
                Actor_Kill(&this->dyna.actor);
            }
        } else if (!(play->actorCtx.flags & ACTORCTX_FLAG_5) && (GET_PLAYER(play)->actor.id == ACTOR_PLAYER) &&
                   Flags_GetSwitch(play, OBJNOZOKI_GET_SWITCHFLAG2(&this->dyna.actor)) && (sp38 < 20.0f)) {
            static Vec3f sZeroVec = { 0.0f, 0.0f, 50.0f };

            play->actorCtx.flags |= ACTORCTX_FLAG_6;
            Lib_Vec3f_TranslateAndRotateY(&this->dyna.actor.home.pos, this->dyna.actor.shape.rot.y, &sZeroVec,
                                          &this->dyna.actor.world.pos);
            this->dyna.actor.shape.rot.x = -0x1F40;
        } else if (sp34 < 50.0f) {
            play->actorCtx.flags |= ACTORCTX_FLAG_5;

            if (sp34 < 20.0f) {
                this->dyna.actor.velocity.y -= 0.4f;
                this->dyna.actor.world.pos.y += this->dyna.actor.velocity.y;

                sp38 = this->dyna.actor.home.pos.y - this->dyna.actor.world.pos.y;
                if (sp38 >= 100.0f) {
                    Actor_Kill(&this->dyna.actor);
                }

                this->dyna.actor.shape.rot.x = -0x1F40 - (s16)(sp38 * 400.0f);
            }
        }
    }

    this->dyna.actor.velocity.x += this->dyna.actor.speedXZ * 0.66f;
    if (this->dyna.actor.velocity.x >= 0x10000) {
        this->dyna.actor.velocity.x -= 0x10000;
    }

    // Apply the mask speed to the underlying conveyer?
    play->roomCtx.unk7A[0] = this->dyna.actor.velocity.x;

    func_8019FAD8(&gSfxDefaultPos, NA_SE_EV_SECOM_CONVEYOR - SFX_FLAG, this->dyna.actor.speedXZ);
}

/* BACK SHUTTER */

void ObjNozoki_BackShutter_Action(ObjNozoki* this, PlayState* play) {
    Vec3f* pos = &this->dyna.actor.focus.pos;

    if (this->sunMaskConveyorSpeed == SUNMASK_CONVEYOR_SPEED_DEFAULT) {
        if (play->actorCtx.flags & ACTORCTX_FLAG_6) {
            this->sunMaskConveyorSpeed = SUNMASK_CONVEYOR_SPEED_SLOW;
            this->timer = 20;
            Math_Vec3f_Copy(&this->dyna.actor.world.pos, pos);
        } else if (!(play->actorCtx.flags & ACTORCTX_FLAG_5) &&
                   Flags_GetSwitch(play, OBJNOZOKI_GET_SWITCHFLAG1(&this->dyna.actor))) {
            pos = &this->dyna.actor.home.pos;
        }
    } else if (DECR(this->timer) == 0) {
        pos = &this->dyna.actor.home.pos;
    }

    Math_Vec3f_StepToXZ(&this->dyna.actor.world.pos, pos, 8.0f);
}

/* FRONT SHUTTER */

void ObjNozoki_FrontShutter_Action(ObjNozoki* this, PlayState* play) {
    if (this->sunMaskConveyorSpeed == SUNMASK_CONVEYOR_SPEED_DEFAULT) {
        if (Flags_GetSwitch(play, OBJNOZOKI_GET_SWITCHFLAG1(&this->dyna.actor))) {
            this->sunMaskConveyorSpeed = SUNMASK_CONVEYOR_SPEED_SLOW;
            this->timer = 70;
        }
    } else if (this->sunMaskConveyorSpeed == SUNMASK_CONVEYOR_SPEED_SLOW) {
        if (D_80BA36B8 > 40.0f) {
            this->sunMaskConveyorSpeed = SUNMASK_CONVEYOR_SPEED_FAST;
            Actor_PlaySfxAtPos(&this->dyna.actor, NA_SE_PL_SIT_ON_HORSE);
        } else if (this->timer != 0) {
            if (DECR(this->timer) == 0) {
                Actor_PlaySfxAtPos(&this->dyna.actor, NA_SE_EV_CONVEYOR_SHUTTER_OPEN);
            }
        } else {
            Math_StepToF(&this->dyna.actor.world.pos.y, this->dyna.actor.home.pos.y + 50.0f, 4.0f);
        }
    } else {
        Math_StepToF(&this->dyna.actor.world.pos.y, this->dyna.actor.home.pos.y, 4.0f);
    }
}

/* CURIOSITY SHOP */

void ObjNozoki_Curiosity_Action1(ObjNozoki* this, PlayState* play) {
    if (gSaveContext.save.weekEventReg[64] & 0x20) {
        Actor* npc = play->actorCtx.actorLists[ACTORCAT_NPC].first;
        Actor* test3 = SubS_FindActor(play, npc, ACTORCAT_NPC, ACTOR_EN_TEST3);

        if ((test3 != NULL) && (test3->draw != NULL)) {
            if ((play->curSpawn == 3) && !(gSaveContext.save.weekEventReg[64] & 0x40)) {
                this->dyna.actor.flags |= (ACTOR_FLAG_TARGETABLE | ACTOR_FLAG_8 | ACTOR_FLAG_10000);
                this->dyna.actor.textId = 0x297A;
            } else {
                this->dyna.actor.flags |= (ACTOR_FLAG_TARGETABLE | ACTOR_FLAG_8);
                if (gSaveContext.save.weekEventReg[64] & 0x40) {
                    this->dyna.actor.textId = 0;
                } else {
                    this->dyna.actor.textId = 0x2979;
                }
            }

            if (Actor_ProcessTalkRequest(&this->dyna.actor, &play->state)) {
                ObjNozoki_SetupAction(this, ObjNozoki_Curiosity_Action2);
            } else {
                func_800B8614(&this->dyna.actor, play, 50.0f);
            }
        }
    }
}

void ObjNozoki_Curiosity_Action2(ObjNozoki* this, PlayState* play) {
    if ((play->curSpawn == 3) && !(gSaveContext.save.weekEventReg[64] & 0x40)) {
        if (Actor_TextboxIsClosing(&this->dyna.actor, play)) {
            gSaveContext.save.weekEventReg[64] |= 0x40;
            this->dyna.actor.flags &= ~ACTOR_FLAG_10000;
            ObjNozoki_SetupAction(this, ObjNozoki_Curiosity_Action1);
        }
    } else if ((this->dyna.actor.textId == 0) || Actor_TextboxIsClosing(&this->dyna.actor, play)) {
        play->nextEntrance = ENTRANCE(CURIOSITY_SHOP, 2);
        play->transitionTrigger = TRANS_TRIGGER_START;
    }
}

/* MAIN */

void ObjNozoki_Update(Actor* thisx, PlayState* play) {
    ObjNozoki* this = THIS;

    this->actionFunc(this, play);
}

Gfx* D_80BA34FC[] = {
    object_secom_obj_DL_000080, // SAKON_HIDEOUT_DOOR
    (Gfx*)0x0A0001A0, //! @bug This dlist should point to a dlist in OBJECT_GI_MSSA, but the segment and the offset are
                      //! wrong. This doesn't have side effects because of the extra check in the Draw function
    object_secom_obj_DL_001230, // SAKON_HIDEOUT_BACK_SHUTTER
    object_secom_obj_DL_001300, // SAKON_HIDEOUT_FRONT_SHUTTER
};

void ObjNozoki_Draw(Actor* thisx, PlayState* play) {
    ObjNozoki* this = THIS;

    if (this->type == SAKON_HIDEOUT_MASK) {
        GetItem_Draw(play, GID_MASK_SUN);
    } else {
        Gfx_DrawDListOpa(play, D_80BA34FC[this->type]);
    }
}
