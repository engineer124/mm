/*
 * File: z_en_invisible_ruppe.c
 * Overlay: ovl_En_Invisible_Ruppe
 * Description: Invisible Rupee
 */

#include "z_en_invisible_ruppe.h"

#define FLAGS 0x00000000

#define THIS ((EnInvisibleRuppe*)thisx)

void EnInvisibleRuppe_Init(Actor* thisx, GlobalContext* globalCtx);
void EnInvisibleRuppe_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnInvisibleRuppe_Update(Actor* thisx, GlobalContext* globalCtx);

void func_80C2590C(EnInvisibleRuppe* this, GlobalContext* globalCtx);
void func_80C259E8(EnInvisibleRuppe* this, GlobalContext* globalCtx);

#if 0
const ActorInit En_Invisible_Ruppe_InitVars = {
    ACTOR_EN_INVISIBLE_RUPPE,
    ACTORCAT_NPC,
    FLAGS,
    GAMEPLAY_KEEP,
    sizeof(EnInvisibleRuppe),
    (ActorFunc)EnInvisibleRuppe_Init,
    (ActorFunc)EnInvisibleRuppe_Destroy,
    (ActorFunc)EnInvisibleRuppe_Update,
    (ActorFunc)NULL,
};

// static ColliderCylinderInit sCylinderInit = {
static ColliderCylinderInit D_80C25B50 = {
    { COLTYPE_NONE, AT_NONE, AC_NONE, OC1_ON | OC1_NO_PUSH | OC1_TYPE_PLAYER, OC2_TYPE_2, COLSHAPE_CYLINDER, },
    { ELEMTYPE_UNK4, { 0x00000000, 0x00, 0x00 }, { 0x00000000, 0x00, 0x00 }, TOUCH_NONE | TOUCH_SFX_NORMAL, BUMP_NONE, OCELEM_ON, },
    { 10, 30, 0, { 0, 0, 0 } },
};

#endif

extern ColliderCylinderInit D_80C25B50;

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Invisible_Ruppe/func_80C258A0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Invisible_Ruppe/func_80C2590C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Invisible_Ruppe/func_80C259E8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Invisible_Ruppe/EnInvisibleRuppe_Init.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Invisible_Ruppe/EnInvisibleRuppe_Destroy.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Invisible_Ruppe/EnInvisibleRuppe_Update.s")
