/*
 * File: z_obj_rotlift.c
 * Overlay: ovl_Obj_Rotlift
 * Description: Deku Moon Dungeon - Spiked Rotating Platforms
 */

#include "z_obj_rotlift.h"
#include "objects/object_rotlift/object_rotlift.h"

#define FLAGS 0x00000000

#define THIS ((ObjRotlift*)thisx)

void ObjRotlift_Init(Actor* thisx, PlayState* play2);
void ObjRotlift_Destroy(Actor* thisx, PlayState* play);
void ObjRotlift_Update(Actor* thisx, PlayState* play);
void ObjRotlift_Draw(Actor* thisx, PlayState* play);

void ObjRotlift_MoveDekuFlowers(ObjRotlift* this);

typedef struct ModelInfo {
    /* 0x0 */ Gfx* dList;
    /* 0x4 */ AnimatedMaterial* animMat;
    /* 0x8 */ CollisionHeader* colHeader;
} ModelInfo; // size = 0xC

const ActorInit Obj_Rotlift_InitVars = {
    ACTOR_OBJ_ROTLIFT,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_ROTLIFT,
    sizeof(ObjRotlift),
    (ActorFunc)ObjRotlift_Init,
    (ActorFunc)ObjRotlift_Destroy,
    (ActorFunc)ObjRotlift_Update,
    (ActorFunc)ObjRotlift_Draw,
};

struct ModelInfo sModelInfo[] = {
    {
        object_rotlift_DL_000400,
        object_rotlift_Matanimheader_001F98,
        &object_rotlift_Colheader_002190,
    },
    {
        object_rotlift_DL_002CE0,
        object_rotlift_Matanimheader_004A08,
        &object_rotlift_Colheader_004DF0,
    },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 4000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 800, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 800, ICHAIN_STOP),
};

void ObjRotlift_MoveDekuFlowers(ObjRotlift* this) {
    ObjEtcetera** dekuFlower = this->dekuFlowers;
    ObjEtcetera* curDekuFlower;
    f32 posOffset = 300.0f;
    s32 i;

    for (i = 0; i < ARRAY_COUNT(this->dekuFlowers); i++, dekuFlower++) {
        curDekuFlower = *dekuFlower;
        if (curDekuFlower->dyna.actor.update == NULL) {
            *dekuFlower = NULL;
        } else {
            curDekuFlower->dyna.actor.world.pos.x =
                this->dyna.actor.world.pos.x + posOffset * Math_SinS(this->dyna.actor.shape.rot.y);
            curDekuFlower->dyna.actor.world.pos.y = this->dyna.actor.world.pos.y + 380.0f * this->dyna.actor.scale.y;
            curDekuFlower->dyna.actor.world.pos.z =
                this->dyna.actor.world.pos.z + posOffset * Math_CosS(this->dyna.actor.shape.rot.y);
            curDekuFlower->dyna.actor.shape.rot.y = this->dyna.actor.shape.rot.y;
        }
        posOffset -= 600.0f;
    }
}

void ObjRotlift_Init(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    ObjRotlift* this = THIS;
    s32 type = OBJROTLIFT_GET_TYPE(&this->dyna.actor);
    s32 dekuFlowerType;
    s32 i;
    ModelInfo* modelInfo;
    ObjEtcetera** dekuFlowers;

    Actor_ProcessInitChain(&this->dyna.actor, sInitChain);
    if (type == 0) {
        for (dekuFlowers = this->dekuFlowers, i = 0; i < ARRAY_COUNT(this->dekuFlowers); i++, dekuFlowers++) {
            if (!OBJROTLIFT_GET_4000(thisx) || (i != 0)) {
                dekuFlowerType = 0;
            } else {
                dekuFlowerType = 0x100;
            }
            *dekuFlowers = (ObjEtcetera*)Actor_SpawnAsChild(
                &play->actorCtx, &this->dyna.actor, play, ACTOR_OBJ_ETCETERA, this->dyna.actor.world.pos.x,
                this->dyna.actor.world.pos.y, this->dyna.actor.world.pos.z, this->dyna.actor.shape.rot.x,
                this->dyna.actor.shape.rot.y, this->dyna.actor.shape.rot.z, dekuFlowerType);
        }
        ObjRotlift_MoveDekuFlowers(this);
    }
    DynaPolyActor_Init(&this->dyna, 3);

    modelInfo = &sModelInfo[type];
    DynaPolyActor_LoadMesh(play, &this->dyna, modelInfo->colHeader);
    modelInfo->animMat = Lib_SegmentedToVirtual(modelInfo->animMat);
}

void ObjRotlift_Destroy(Actor* thisx, PlayState* play) {
    ObjRotlift* this = THIS;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void ObjRotlift_Update(Actor* thisx, PlayState* play) {
    ObjRotlift* this = THIS;
    s16 angShift;
    s32 angVelocity;

    if (OBJROTLIFT_GET_TYPE(&this->dyna.actor) == 0) {
        ObjRotlift_MoveDekuFlowers(this);
    }
    if (thisx->params >= 0) {
        angVelocity = -0xC8;
    } else {
        angVelocity = 0xC8;
    }
    angShift = angVelocity;
    this->dyna.actor.shape.rot.y += angShift;
}

void ObjRotlift_Draw(Actor* thisx, PlayState* play) {
    s32 pad;
    ObjRotlift* this = THIS;
    ModelInfo* modelInfo = &sModelInfo[OBJROTLIFT_GET_TYPE(&this->dyna.actor)];

    AnimatedMat_Draw(play, modelInfo->animMat);
    Gfx_DrawDListOpa(play, modelInfo->dList);
}
