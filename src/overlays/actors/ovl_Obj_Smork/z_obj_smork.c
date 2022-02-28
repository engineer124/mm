/*
 * File: z_obj_smork.c
 * Overlay: ovl_Obj_Smork
 * Description: Romani Ranch Chimney Smoke
 */

#include "z_obj_smork.h"

#define FLAGS (ACTOR_FLAG_10 | ACTOR_FLAG_20)

#define THIS ((ObjSmork*)thisx)

void ObjSmork_Init(Actor* thisx, GlobalContext* globalCtx);
void ObjSmork_Destroy(Actor* thisx, GlobalContext* globalCtx);
void ObjSmork_Update(Actor* thisx, GlobalContext* globalCtx);
void ObjSmork_Draw(Actor* thisx, GlobalContext* globalCtx);

#if 0
const ActorInit Obj_Smork_InitVars = {
    ACTOR_OBJ_SMORK,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_F53_OBJ,
    sizeof(ObjSmork),
    (ActorFunc)ObjSmork_Init,
    (ActorFunc)ObjSmork_Destroy,
    (ActorFunc)ObjSmork_Update,
    (ActorFunc)ObjSmork_Draw,
};

#endif

extern UNK_TYPE D_06001C00;

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Smork/func_80A3D680.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Smork/func_80A3D940.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Smork/func_80A3D9C4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Smork/ObjSmork_Init.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Smork/ObjSmork_Destroy.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Smork/ObjSmork_Update.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Smork/ObjSmork_Draw.s")
