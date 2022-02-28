/*
 * File: z_en_hs.c
 * Overlay: ovl_En_Hs
 * Description: Grog
 */

#include "z_en_hs.h"

#define FLAGS (ACTOR_FLAG_1 | ACTOR_FLAG_8 | ACTOR_FLAG_10)

#define THIS ((EnHs*)thisx)

void EnHs_Init(Actor* thisx, GlobalContext* globalCtx);
void EnHs_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnHs_Update(Actor* thisx, GlobalContext* globalCtx);
void EnHs_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80952FE0(EnHs* this, GlobalContext* globalCtx);
void func_80953098(EnHs* this, GlobalContext* globalCtx);
void func_80953180(EnHs* this, GlobalContext* globalCtx);
void func_809532C0(EnHs* this, GlobalContext* globalCtx);
void func_809532D0(EnHs* this, GlobalContext* globalCtx);
void func_80953354(EnHs* this, GlobalContext* globalCtx);
void func_8095345C(EnHs* this, GlobalContext* globalCtx);

#if 0
const ActorInit En_Hs_InitVars = {
    ACTOR_EN_HS,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_HS,
    sizeof(EnHs),
    (ActorFunc)EnHs_Init,
    (ActorFunc)EnHs_Destroy,
    (ActorFunc)EnHs_Update,
    (ActorFunc)EnHs_Draw,
};

// static ColliderCylinderInit sCylinderInit = {
static ColliderCylinderInit D_80953910 = {
    { COLTYPE_NONE, AT_NONE, AC_ON | AC_TYPE_ENEMY, OC1_ON | OC1_TYPE_ALL, OC2_TYPE_1, COLSHAPE_CYLINDER, },
    { ELEMTYPE_UNK0, { 0x00000000, 0x00, 0x00 }, { 0xF7CFFFFF, 0x00, 0x00 }, TOUCH_NONE | TOUCH_SFX_NORMAL, BUMP_ON, OCELEM_ON, },
    { 40, 40, 0, { 0, 0, 0 } },
};

#endif

extern ColliderCylinderInit D_80953910;

extern UNK_TYPE D_060005C0;

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Hs/func_80952C50.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Hs/EnHs_Init.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Hs/EnHs_Destroy.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Hs/func_80952DFC.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Hs/func_80952E50.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Hs/func_80952F00.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Hs/func_80952FE0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Hs/func_80953098.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Hs/func_80953180.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Hs/func_809532C0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Hs/func_809532D0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Hs/func_80953354.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Hs/func_809533A0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Hs/func_8095345C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Hs/EnHs_Update.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Hs/func_8095376C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Hs/func_80953848.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Hs/EnHs_Draw.s")
