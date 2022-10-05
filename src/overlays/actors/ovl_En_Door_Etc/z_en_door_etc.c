/*
 * File: z_en_door_etc.c
 * Overlay: ovl_En_Door_Etc
 * Description: Wooden Door
 */

#include "z_en_door_etc.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define FLAGS (ACTOR_FLAG_10)

#define THIS ((EnDoorEtc*)thisx)

void EnDoorEtc_Init(Actor* thisx, PlayState* play2);
void EnDoorEtc_Destroy(Actor* thisx, PlayState* play);
void EnDoorEtc_Update(Actor* thisx, PlayState* play);

void EnDoorEtc_WaitForObject(EnDoorEtc* this, PlayState* play);
void func_80AC21A0(EnDoorEtc* this, PlayState* play);
void func_80AC2354(EnDoorEtc* this, PlayState* play);
void EnDoorEtc_Draw(Actor* thisx, PlayState* play);

const ActorInit En_Door_Etc_InitVars = {
    ACTOR_EN_DOOR_ETC,
    ACTORCAT_DOOR,
    FLAGS,
    GAMEPLAY_KEEP,
    sizeof(EnDoorEtc),
    (ActorFunc)EnDoorEtc_Init,
    (ActorFunc)EnDoorEtc_Destroy,
    (ActorFunc)EnDoorEtc_Update,
    (ActorFunc)NULL,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_NONE | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_ON,
    },
    { 100, 40, 0, { 0, 0, 0 } },
};

typedef struct {
    /* 0x0 */ s16 sceneId;
    /* 0x2 */ u8 dListIndex;
    /* 0x4 */ s16 objectId;
} EnDoorEtcInfo; // size = 0x6

EnDoorEtcInfo sObjInfo[] = {
    { SCENE_MITURIN, 1, OBJECT_NUMA_OBJ },
    { -1, 0, GAMEPLAY_KEEP },
    { -1, 13, GAMEPLAY_FIELD_KEEP },
    { 0, 0, OBJECT_UNSET_0 },
    { 0, 0, OBJECT_UNSET_0 },
    { 0, 0, OBJECT_UNSET_0 },
    { 0, 0, OBJECT_UNSET_0 },
    { 0, 0, OBJECT_UNSET_0 },
    { 0, 0, OBJECT_UNSET_0 },
    { 0, 0, OBJECT_UNSET_0 },
    { 0, 0, OBJECT_UNSET_0 },
    { 0, 0, OBJECT_UNSET_0 },
    { 0, 0, OBJECT_UNSET_0 },
    { 0, 0, OBJECT_UNSET_0 },
    { 0, 0, OBJECT_UNSET_0 },
    { 0, 0, OBJECT_UNSET_0 },
    { 0, 0, OBJECT_UNSET_0 },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_U8(targetMode, 0, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 4000, ICHAIN_CONTINUE),
    ICHAIN_U16(shape.rot.x, 0, ICHAIN_CONTINUE),
    ICHAIN_U16(shape.rot.z, 0, ICHAIN_STOP),
};

void EnDoorEtc_Init(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    s32 objectIndex;
    EnDoorEtcInfo* objectInfo = sObjInfo;
    s32 i;
    EnDoorEtc* this = THIS;

    Actor_ProcessInitChain(&this->door.dyna.actor, sInitChain);
    Actor_SetScale(&this->door.dyna.actor, 0.01f);
    this->door.dyna.actor.shape.rot.x = -0x4000;
    this->angle = 0;
    for (i = 0; i < 15; i++, objectInfo++) {
        if (play->sceneId == objectInfo->sceneId) {
            break;
        }
    }
    if ((i >= 15) && (Object_GetIndex(&play->objectCtx, GAMEPLAY_FIELD_KEEP) >= 0)) {
        objectInfo++;
    }
    objectIndex = Object_GetIndex(&play->objectCtx, objectInfo->objectId);
    if (objectIndex < 0) {
        Actor_MarkForDeath(&this->door.dyna.actor);
    } else {
        this->door.requiredObjBankIndex = objectIndex;
        this->door.dlIndex = objectInfo->dListIndex;
        if (this->door.dyna.actor.objBankIndex == this->door.requiredObjBankIndex) {
            EnDoorEtc_WaitForObject(this, play);
        } else {
            this->actionFunc = EnDoorEtc_WaitForObject;
        }
    }
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->door.dyna.actor, &sCylinderInit);
    this->door.dyna.actor.colChkInfo.mass = MASS_IMMOVABLE;
}

void EnDoorEtc_Destroy(Actor* thisx, PlayState* play) {
    EnDoorEtc* this = THIS;

    Collider_DestroyCylinder(play, &this->collider);
}

/**
 * Calculates if the distance between `a` and `b` is greater than distance `c`
 */
s32 EnDoorEtc_IsDistanceGreater(Vec3f* a, Vec3f* b, f32 c) {
    f32 dx = b->x - a->x;
    f32 dy = b->y - a->y;
    f32 dz = b->z - a->z;

    return ((SQ(dx) + SQ(dy) + SQ(dz)) < SQ(c));
}

void EnDoorEtc_WaitForObject(EnDoorEtc* this, PlayState* play) {
    if (Object_IsLoaded(&play->objectCtx, this->door.requiredObjBankIndex)) {
        this->door.dyna.actor.flags &= ~ACTOR_FLAG_10;
        this->door.dyna.actor.objBankIndex = this->door.requiredObjBankIndex;
        this->actionFunc = func_80AC2354;
        this->door.dyna.actor.draw = EnDoorEtc_Draw;
    }
}

void func_80AC2118(EnDoorEtc* this, PlayState* play) {
    if (this->angle < 0) {
        this->angle += 0x100;
    } else {
        this->angle = 0;
        this->actionFunc = func_80AC21A0;
        this->unk_1F4 &= ~1;
    }
}

void func_80AC2154(EnDoorEtc* this, PlayState* play) {
    if (this->timer > 0) {
        this->timer--;
    } else if (this->angle > -0x4000) {
        this->angle -= 0x800;
    } else {
        this->angle = -0x4000;
        this->actionFunc = func_80AC2118;
    }
}

void func_80AC21A0(EnDoorEtc* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Vec3f playerOffsetFromDoor;
    s32 pad;
    s16 yawDiff;
    s32 yawDiffAbs;

    Actor_OffsetOfPointInActorCoords(&this->door.dyna.actor, &playerOffsetFromDoor, &player->actor.world.pos);
    if (this->door.playOpenAnim == 0) {
        if ((!Player_InCsMode(play)) &&
            ((fabsf(playerOffsetFromDoor.y) < 20.0f) && fabsf(playerOffsetFromDoor.x) < 20.0f) &&
            (fabsf(playerOffsetFromDoor.z) < 50.0f)) {
            yawDiff = player->actor.shape.rot.y - this->door.dyna.actor.shape.rot.y;
            if (playerOffsetFromDoor.z > 0.0f) {
                yawDiff = 0x8000 - yawDiff;
            }
            yawDiffAbs = ABS_ALT(yawDiff);
            if (yawDiffAbs < 0x3000) {
                player->doorDirection = (playerOffsetFromDoor.z >= 0.0f) ? 1.0f : -1.0f;
                player->doorActor = &this->door.dyna.actor;
                player->doorType = -1;
            }
        }
    }
    if ((this->door.dyna.actor.textId == 0x239B) &&
        Flags_GetSwitch(play, ENDOORETC_GET_SWITCHFLAG(&this->door.dyna.actor))) {
        Flags_UnsetSwitch(play, ENDOORETC_GET_SWITCHFLAG(&this->door.dyna.actor));
        this->actionFunc = func_80AC2154;
        this->door.dyna.actor.textId = 0x1800; // "It won't budge!"
        this->unk_1F4 |= 1;
        this->timer = 0x5A;
    }
}

void func_80AC2354(EnDoorEtc* this, PlayState* play) {
    Actor* door = play->actorCtx.actorLists[ACTORCAT_DOOR].first;

    while (door != NULL) {
        if ((door->id != ACTOR_EN_DOOR) ||
            !EnDoorEtc_IsDistanceGreater(&door->world.pos, &this->door.dyna.actor.world.pos, 10.0f)) {
            door = door->next;
        } else {
            this->door.dyna.actor.world.pos.x = door->world.pos.x;
            this->door.dyna.actor.world.pos.y = door->world.pos.y;
            this->door.dyna.actor.world.pos.z = door->world.pos.z;
            this->door.dyna.actor.shape.rot.y = door->shape.rot.y;
            this->door.dyna.actor.world.rot.y = door->world.rot.y;
            Actor_MarkForDeath(door);
            this->actionFunc = func_80AC21A0;
            this->door.dyna.actor.textId = 0x239B;
            Actor_SetFocus(&this->door.dyna.actor, 70.0f);
            break;
        }
    }
}

void EnDoorEtc_Update(Actor* thisx, PlayState* play) {
    s32 pad;
    EnDoorEtc* this = THIS;

    this->actionFunc(this, play);
    if (this->unk_1F4 & 1) {
        Collider_UpdateCylinder(&this->door.dyna.actor, &this->collider);
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    }
}

void EnDoorEtc_Draw(Actor* thisx, PlayState* play) {
    EnDoorEtc* this = THIS;

    OPEN_DISPS(play->state.gfxCtx);

    func_8012C28C(play->state.gfxCtx);
    Matrix_Translate(-2900.0f, 0.0f, 0.0f, MTXMODE_APPLY);
    Matrix_RotateZS(this->angle, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, gDoorLeftDL);
    gSPDisplayList(POLY_OPA_DISP++, gDoorRightDL);

    CLOSE_DISPS(play->state.gfxCtx);
}
