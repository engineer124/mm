/*
 * File: z_obj_hsblock.c
 * Overlay: ovl_Obj_Hsblock
 * Description: Hookshot Block
 */

#include "z_obj_hsblock.h"

#define FLAGS 0x00000000

#define THIS ((ObjHsblock*)thisx)

void ObjHsblock_Init(Actor* thisx, PlayState* play);
void ObjHsblock_Destroy(Actor* thisx, PlayState* play);
void ObjHsblock_Update(Actor* thisx, PlayState* play);
void ObjHsblock_Draw(Actor* thisx, PlayState* play);

void func_8093E0A0(ObjHsblock* this, PlayState* play);

void ObjHsblock_SetupAction(ObjHsblock* this, ObjHsblockActionFunc actionFunc);

#if 0
const ActorInit Obj_Hsblock_InitVars = {
    ACTOR_OBJ_HSBLOCK,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_D_HSBLOCK,
    sizeof(ObjHsblock),
    (ActorFunc)ObjHsblock_Init,
    (ActorFunc)ObjHsblock_Destroy,
    (ActorFunc)ObjHsblock_Update,
    (ActorFunc)ObjHsblock_Draw,
};

// static InitChainEntry sInitChain[] = {
static InitChainEntry D_8093E33C[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 4000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 200, ICHAIN_STOP),
};

#endif

extern InitChainEntry D_8093E33C[];

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hsblock/ObjHsblock_SetupAction.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hsblock/func_8093DEAC.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hsblock/ObjHsblock_Init.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hsblock/ObjHsblock_Destroy.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hsblock/func_8093E03C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hsblock/func_8093E05C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hsblock/func_8093E0A0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hsblock/func_8093E0E8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hsblock/func_8093E10C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hsblock/ObjHsblock_Update.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hsblock/ObjHsblock_Draw.s")
