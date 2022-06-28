/*
 * File: z_obj_hgdoor.c
 * Overlay: ovl_Obj_Hgdoor
 * Description: Music Box House - Closet Door
 */

#include "z_obj_hgdoor.h"
#include "objects/object_hgdoor/object_hgdoor.h"

#define FLAGS (ACTOR_FLAG_100000)

#define THIS ((ObjHgdoor*)thisx)

void ObjHgdoor_Init(Actor* thisx, PlayState* play);
void ObjHgdoor_Destroy(Actor* thisx, PlayState* play);
void ObjHgdoor_Update(Actor* thisx, PlayState* play);
void ObjHgdoor_Draw(Actor* thisx, PlayState* play);

void ObjHgdoor_SetupCheckShouldOpen(ObjHgdoor* this);
void ObjHgdoor_CheckShouldOpen(ObjHgdoor* this, PlayState* play);
void func_80BD42AC(ObjHgdoor* this);
void func_80BD42C0(ObjHgdoor* this, PlayState* play);
void func_80BD433C(ObjHgdoor* this);
void func_80BD4358(ObjHgdoor* this, PlayState* play);
void func_80BD4460(ObjHgdoor* this);
void func_80BD4478(ObjHgdoor* this, PlayState* play);
s32 func_80BD44D0(ObjHgdoor* this, PlayState* play);

const ActorInit Obj_Hgdoor_InitVars = {
    ACTOR_OBJ_HGDOOR,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_HGDOOR,
    sizeof(ObjHgdoor),
    (ActorFunc)ObjHgdoor_Init,
    (ActorFunc)ObjHgdoor_Destroy,
    (ActorFunc)ObjHgdoor_Update,
    (ActorFunc)ObjHgdoor_Draw,
};

static s16 D_80BD4690 = 0;
static s32 unused = 0;
static s32 unused2 = 0;

void ObjHgdoor_SetChild(ObjHgdoor* this, PlayState* play) {
    Actor* actorIterator = play->actorCtx.actorLists[ACTORCAT_PROP].first;

    while (actorIterator) {
        if ((actorIterator->id == ACTOR_OBJ_HGDOOR) && (&this->dyna.actor != actorIterator)) {
            this->dyna.actor.child = actorIterator;
            break;
        }
        actorIterator = actorIterator->next;
    }
}

void ObjHgdoor_SetParent(ObjHgdoor* this, PlayState* play) {
    Actor* actorIterator = play->actorCtx.actorLists[ACTORCAT_PROP].first;

    while (actorIterator) {
        if (actorIterator->id == ACTOR_EN_HG) {
            this->dyna.actor.parent = actorIterator;
            break;
        }
        actorIterator = actorIterator->next;
    }
}

void ObjHgdoor_Init(Actor* thisx, PlayState* play) {
    ObjHgdoor* this = THIS;
    s32 pad;
    CollisionHeader* header = NULL;

    Actor_SetScale(&this->dyna.actor, 0.1f);
    DynaPolyActor_Init(&this->dyna, 1);
    if (OBJHGDOOR_IS_RIGHT_DOOR(&this->dyna.actor)) {
        CollisionHeader_GetVirtual(&object_hgdoor_Colheader_001D10, &header);
    } else {
        CollisionHeader_GetVirtual(&object_hgdoor_Colheader_0018C0, &header);
    }
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &this->dyna.actor, header);
    this->rotation = 0;
    this->timer = 0;
    this->cutscene = this->dyna.actor.cutscene;
    ObjHgdoor_SetupCheckShouldOpen(this);
}

void ObjHgdoor_Destroy(Actor* thisx, PlayState* play) {
    ObjHgdoor* this = THIS;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void ObjHgdoor_SetupCheckShouldOpen(ObjHgdoor* this) {
    this->actionFunc = ObjHgdoor_CheckShouldOpen;
}

void ObjHgdoor_CheckShouldOpen(ObjHgdoor* this, PlayState* play) {
    if (!(gSaveContext.save.weekEventReg[75] & 0x20) && !(gSaveContext.save.weekEventReg[52] & 0x20) &&
        (this->dyna.actor.xzDistToPlayer < 100.0f) && (this->dyna.actor.playerHeightRel < 40.0f) &&
        OBJHGDOOR_IS_RIGHT_DOOR(&this->dyna.actor)) {
        ObjHgdoor_SetChild(this, play);
        ObjHgdoor_SetParent(this, play);
        func_80BD42AC(this);
    }
}

void func_80BD42AC(ObjHgdoor* this) {
    this->actionFunc = func_80BD42C0;
}

void func_80BD42C0(ObjHgdoor* this, PlayState* play) {
    if (ActorCutscene_GetCanPlayNext(this->cutscene)) {
        ActorCutscene_Start(this->cutscene, &this->dyna.actor);
        func_80BD433C(this);
        func_80BD433C((ObjHgdoor*)this->dyna.actor.child);
    } else {
        if (ActorCutscene_GetCurrentIndex() == 0x7C) {
            ActorCutscene_Stop(0x7C);
        }
        ActorCutscene_SetIntentToPlay(this->cutscene);
    }
}

void func_80BD433C(ObjHgdoor* this) {
    this->unk166 = 0x63;
    this->actionFunc = func_80BD4358;
}

void func_80BD4358(ObjHgdoor* this, PlayState* play) {
    s32 actionIndex;

    if (Cutscene_CheckActorAction(play, 483)) {
        actionIndex = Cutscene_GetActorActionIndex(play, 483);
        if (this->unk166 != play->csCtx.actorActions[actionIndex]->action) {
            this->unk166 = play->csCtx.actorActions[actionIndex]->action;
            switch (play->csCtx.actorActions[actionIndex]->action) {
                case 1:
                    Actor_PlaySfxAtPos(&this->dyna.actor, NA_SE_EV_WOOD_DOOR_OPEN_SPEEDY);
                    if ((this->dyna.actor.parent != NULL) && (this->dyna.actor.parent->id == ACTOR_EN_HG)) {
                        this->dyna.actor.parent->colChkInfo.health = 1;
                    }
                    D_80BD4690 = 1;
                    break;
                case 2:
                    break;
            }
        }
        if ((D_80BD4690 == 1) && (func_80BD44D0(this, play))) {
            func_80BD4460(this);
        }
    } else {
        this->unk166 = 0x63;
    }
}

void func_80BD4460(ObjHgdoor* this) {
    this->timer = 0;
    this->actionFunc = func_80BD4478;
}

void func_80BD4478(ObjHgdoor* this, PlayState* play) {
    if (this->timer++ > 80 && !ActorCutscene_GetCanPlayNext(this->cutscene)) {
        ActorCutscene_Stop(this->cutscene);
    }
}

s32 func_80BD44D0(ObjHgdoor* this, PlayState* play) {
    if (this->rotation < 0x5555) {
        this->rotation += 0x1388;
    } else {
        return true;
    }

    return false;
}

void func_80BD4500(ObjHgdoor* this) {
    this->dyna.actor.shape.rot.y = this->dyna.actor.home.rot.y;
    if (OBJHGDOOR_IS_RIGHT_DOOR(&this->dyna.actor)) {
        this->dyna.actor.shape.rot.y += this->rotation;
    } else {
        this->dyna.actor.shape.rot.y -= this->rotation;
    }
}

void ObjHgdoor_Update(Actor* thisx, PlayState* play) {
    ObjHgdoor* this = THIS;

    this->actionFunc(this, play);
    func_80BD4500(this);
}

void ObjHgdoor_Draw(Actor* thisx, PlayState* play) {
    OPEN_DISPS(play->state.gfxCtx);
    func_8012C28C(play->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    if (OBJHGDOOR_IS_RIGHT_DOOR(thisx)) {
        gSPDisplayList(POLY_OPA_DISP++, object_hgdoor_DL_001AB0);
        gSPDisplayList(POLY_OPA_DISP++, object_hgdoor_DL_001BA8);
    } else {
        gSPDisplayList(POLY_OPA_DISP++, object_hgdoor_DL_001670);
        gSPDisplayList(POLY_OPA_DISP++, object_hgdoor_DL_001768);
    }

    CLOSE_DISPS(play->state.gfxCtx);
}
