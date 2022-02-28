/*
 * File: z_obj_hakaisi.c
 * Overlay: ovl_Obj_Hakaisi
 * Description: Gravestone
 */

#include "z_obj_hakaisi.h"

#define FLAGS (ACTOR_FLAG_1 | ACTOR_FLAG_8 | ACTOR_FLAG_20)

#define THIS ((ObjHakaisi*)thisx)

void ObjHakaisi_Init(Actor* thisx, GlobalContext* globalCtx);
void ObjHakaisi_Destroy(Actor* thisx, GlobalContext* globalCtx);
void ObjHakaisi_Update(Actor* thisx, GlobalContext* globalCtx);
void ObjHakaisi_Draw(Actor* thisx, GlobalContext* globalCtx);

#if 0
const ActorInit Obj_Hakaisi_InitVars = {
    ACTOR_OBJ_HAKAISI,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_HAKAISI,
    sizeof(ObjHakaisi),
    (ActorFunc)ObjHakaisi_Init,
    (ActorFunc)ObjHakaisi_Destroy,
    (ActorFunc)ObjHakaisi_Update,
    (ActorFunc)ObjHakaisi_Draw,
};

#endif

extern UNK_TYPE D_06001F10;
extern UNK_TYPE D_060021B0;
extern UNK_TYPE D_06002650;
extern UNK_TYPE D_06002FC4;

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hakaisi/ObjHakaisi_Init.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hakaisi/ObjHakaisi_Destroy.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hakaisi/func_80B1444C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hakaisi/func_80B14460.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hakaisi/func_80B14510.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hakaisi/func_80B14524.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hakaisi/func_80B14558.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hakaisi/func_80B1456C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hakaisi/func_80B145F4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hakaisi/func_80B14648.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hakaisi/func_80B149A8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hakaisi/func_80B149C0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hakaisi/func_80B14A24.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hakaisi/func_80B14B6C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hakaisi/func_80B14CF8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hakaisi/func_80B14F4C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hakaisi/ObjHakaisi_Update.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hakaisi/ObjHakaisi_Draw.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hakaisi/func_80B151E0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hakaisi/func_80B15254.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hakaisi/func_80B15264.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hakaisi/func_80B15330.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hakaisi/func_80B1544C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Hakaisi/func_80B154A0.s")
