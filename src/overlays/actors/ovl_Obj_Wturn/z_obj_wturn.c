/*
 * File: z_obj_wturn.c
 * Overlay: ovl_Obj_Wturn
 * Description: Stone Tower Temple Inverter
 */

#include "z_obj_wturn.h"

#define FLAGS (ACTOR_FLAG_10 | ACTOR_FLAG_100000 | ACTOR_FLAG_2000000)

#define THIS ((ObjWturn*)thisx)

void ObjWturn_Init(Actor* thisx, GlobalContext* globalCtx);
void ObjWturn_Update(Actor* thisx, GlobalContext* globalCtx);

void func_808A7968(ObjWturn* this, GlobalContext* globalCtx);
void func_808A7A5C(ObjWturn* this, GlobalContext* globalCtx);
void func_808A7BA0(ObjWturn* this, GlobalContext* globalCtx);
void func_808A7C78(ObjWturn* this, GlobalContext* globalCtx);

#if 0
const ActorInit Obj_Wturn_InitVars = {
    ACTOR_OBJ_WTURN,
    ACTORCAT_ITEMACTION,
    FLAGS,
    GAMEPLAY_KEEP,
    sizeof(ObjWturn),
    (ActorFunc)ObjWturn_Init,
    (ActorFunc)Actor_Noop,
    (ActorFunc)ObjWturn_Update,
    (ActorFunc)NULL,
};

#endif

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Wturn/ObjWturn_Init.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Wturn/func_808A7954.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Wturn/func_808A7968.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Wturn/func_808A7A24.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Wturn/func_808A7A5C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Wturn/func_808A7AAC.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Wturn/func_808A7BA0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Wturn/func_808A7C04.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Wturn/func_808A7C78.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Wturn/ObjWturn_Update.s")
