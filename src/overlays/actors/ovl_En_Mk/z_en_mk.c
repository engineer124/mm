/*
 * File: z_en_mk.c
 * Overlay: ovl_En_Mk
 * Description: Marine Researcher
 */

#include "z_en_mk.h"

#define FLAGS (ACTOR_FLAG_1 | ACTOR_FLAG_8 | ACTOR_FLAG_10)

#define THIS ((EnMk*)thisx)

void EnMk_Init(Actor* thisx, GlobalContext* globalCtx);
void EnMk_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnMk_Update(Actor* thisx, GlobalContext* globalCtx);
void EnMk_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_809596A0(EnMk* this, GlobalContext* globalCtx);
void func_80959774(EnMk* this, GlobalContext* globalCtx);
void func_80959A24(EnMk* this, GlobalContext* globalCtx);
void func_80959C94(EnMk* this, GlobalContext* globalCtx);
void func_80959D28(EnMk* this, GlobalContext* globalCtx);
void func_80959E18(EnMk* this, GlobalContext* globalCtx);

#if 0
const ActorInit En_Mk_InitVars = {
    ACTOR_EN_MK,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_MK,
    sizeof(EnMk),
    (ActorFunc)EnMk_Init,
    (ActorFunc)EnMk_Destroy,
    (ActorFunc)EnMk_Update,
    (ActorFunc)EnMk_Draw,
};

// static ColliderCylinderInit sCylinderInit = {
static ColliderCylinderInit D_8095A260 = {
    { COLTYPE_NONE, AT_NONE, AC_ON | AC_TYPE_ENEMY, OC1_ON | OC1_TYPE_ALL, OC2_TYPE_1, COLSHAPE_CYLINDER, },
    { ELEMTYPE_UNK0, { 0x00000000, 0x00, 0x00 }, { 0xF7CFFFFF, 0x00, 0x00 }, TOUCH_NONE | TOUCH_SFX_NORMAL, BUMP_ON, OCELEM_ON, },
    { 30, 40, 0, { 0, 0, 0 } },
};

#endif

extern ColliderCylinderInit D_8095A260;

extern UNK_TYPE D_06001C38;

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Mk/func_809592E0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Mk/EnMk_Init.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Mk/EnMk_Destroy.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Mk/func_80959524.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Mk/func_8095954C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Mk/func_80959624.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Mk/func_809596A0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Mk/func_80959774.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Mk/func_80959844.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Mk/func_80959A24.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Mk/func_80959C94.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Mk/func_80959D28.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Mk/func_80959E18.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Mk/EnMk_Update.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Mk/func_8095A150.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Mk/func_8095A198.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Mk/EnMk_Draw.s")
