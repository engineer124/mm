/*
 * File: z_obj_kzsaku.c
 * Overlay: ovl_Obj_Kzsaku
 * Description: Underwater grate
 */

#include "z_obj_kzsaku.h"
#include "objects/object_kzsaku/object_kzsaku.h"

#define FLAGS (ACTOR_FLAG_10 | ACTOR_FLAG_20)

#define THIS ((ObjKzsaku*)thisx)

void ObjKzsaku_Init(Actor* thisx, PlayState* play);
void ObjKzsaku_Destroy(Actor* thisx, PlayState* play);
void ObjKzsaku_Update(Actor* thisx, PlayState* play);
void ObjKzsaku_Draw(Actor* thisx, PlayState* play);

void ObjKzsaku_SetupIdle(ObjKzsaku* this);
void func_80C08BBC(ObjKzsaku* this);
void func_80C08C84(ObjKzsaku* this);
void ObjKzsaku_Idle(ObjKzsaku* this, PlayState* play);
void ObjKzsaku_Rise(ObjKzsaku* this, PlayState* play);
void func_80C08CB0(ObjKzsaku* this, PlayState* play);

ActorInit Obj_Kzsaku_InitVars = {
    ACTOR_OBJ_KZSAKU,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_KZSAKU,
    sizeof(ObjKzsaku),
    (ActorFunc)ObjKzsaku_Init,
    (ActorFunc)ObjKzsaku_Destroy,
    (ActorFunc)ObjKzsaku_Update,
    (ActorFunc)ObjKzsaku_Draw,
};

void ObjKzsaku_Init(Actor* thisx, PlayState* play) {
    s32 pad;
    ObjKzsaku* this = THIS;
    CollisionHeader* col = NULL;

    Actor_SetScale(&this->dyna.actor, 1.0f);
    DynaPolyActor_Init(&this->dyna, 1);
    CollisionHeader_GetVirtual(&object_kzsaku_Colheader_001118, &col);

    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, col);

    this->switchFlag = KZSAKU_GET_SWITCHFLAG(thisx);
    this->raisedAmount = 0.0f;

    if (Flags_GetSwitch(play, this->switchFlag)) {
        func_80C08C84(this);
    } else {
        ObjKzsaku_SetupIdle(this);
    }
}

void ObjKzsaku_Destroy(Actor* thisx, PlayState* play) {
    ObjKzsaku* this = THIS;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void ObjKzsaku_SetupIdle(ObjKzsaku* this) {
    this->dyna.actor.world.pos.y = this->dyna.actor.home.pos.y;
    this->actionFunc = ObjKzsaku_Idle;
}

void ObjKzsaku_Idle(ObjKzsaku* this, PlayState* play) {
    if (Flags_GetSwitch(play, this->switchFlag)) {
        func_80C08BBC(this);
    }
}

void func_80C08BBC(ObjKzsaku* this) {
    this->actionFunc = ObjKzsaku_Rise;
}

void ObjKzsaku_Rise(ObjKzsaku* this, PlayState* play) {
    if (this->dyna.actor.cutscene != -1) {
        if (ActorCutscene_GetCanPlayNext(this->dyna.actor.cutscene)) {
            ActorCutscene_StartAndSetUnkLinkFields(this->dyna.actor.cutscene, &this->dyna.actor);
        } else {
            ActorCutscene_SetIntentToPlay(this->dyna.actor.cutscene);
        }
    }
    if (this->raisedAmount < 450.0f) {
        func_800B9010(&this->dyna.actor, NA_SE_EV_METALDOOR_SLIDE - SFX_FLAG);
        this->raisedAmount += 15.0f;
    } else {
        func_80C08C84(this);
    }
    this->dyna.actor.world.pos.y = this->dyna.actor.home.pos.y + this->raisedAmount;
}

void func_80C08C84(ObjKzsaku* this) {
    this->timer = 0;
    this->dyna.actor.world.pos.y = this->dyna.actor.home.pos.y + 450.0f;
    this->actionFunc = func_80C08CB0;
}

void func_80C08CB0(ObjKzsaku* this, PlayState* play) {
    if (this->timer <= 20) {
        if (this->timer == 20) {
            if (ActorCutscene_GetCurrentIndex() == this->dyna.actor.cutscene) {
                ActorCutscene_Stop(this->dyna.actor.cutscene);
            }
            this->timer = 21;
        } else {
            this->timer++;
        }
    }
}

void ObjKzsaku_Update(Actor* thisx, PlayState* play) {
    ObjKzsaku* this = THIS;

    this->actionFunc(this, play);
}

void ObjKzsaku_Draw(Actor* thisx, PlayState* play) {
    OPEN_DISPS(play->state.gfxCtx);

    func_8012C28C(play->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, object_kzsaku_DL_000040);

    CLOSE_DISPS(play->state.gfxCtx);
}
