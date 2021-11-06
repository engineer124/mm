/*
 * File: z_en_mm.c
 * Overlay: ovl_En_Mm
 * Description: Rock Sirloin
 */

#include "z_en_mm.h"

#define FLAGS 0x00000010

#define THIS ((EnMm*)thisx)

void EnMm_Init(Actor* thisx, GlobalContext* globalCtx);
void EnMm_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnMm_Update(Actor* thisx, GlobalContext* globalCtx);
void EnMm_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80965D3C(EnMm* this, GlobalContext* globalCtx);
void func_80965DB4(EnMm* this, GlobalContext* globalCtx);
void func_8096611C(EnMm* this, GlobalContext* globalCtx);

void EnMm_SetupAction(EnMm* this, EnMmActionFunc actionFunc);

#if 0
const ActorInit En_Mm_InitVars = {
    ACTOR_EN_MM,
    ACTORCAT_ITEMACTION,
    FLAGS,
    GAMEPLAY_KEEP,
    sizeof(EnMm),
    (ActorFunc)EnMm_Init,
    (ActorFunc)EnMm_Destroy,
    (ActorFunc)EnMm_Update,
    (ActorFunc)EnMm_Draw,
};

// static ColliderCylinderInit sCylinderInit = {
static ColliderCylinderInit D_80966340 = {
    { COLTYPE_METAL, AT_NONE, AC_NONE, OC1_ON | OC1_TYPE_ALL, OC2_TYPE_2, COLSHAPE_CYLINDER, },
    { ELEMTYPE_UNK2, { 0x00100000, 0x00, 0x00 }, { 0x01000202, 0x00, 0x00 }, TOUCH_NONE | TOUCH_SFX_NORMAL, BUMP_NONE, OCELEM_ON, },
    { 6, 30, 0, { 0, 0, 0 } },
};

// static InitChainEntry sInitChain[] = {
static InitChainEntry D_8096636C[] = {
    ICHAIN_F32_DIV1000(gravity, -1200, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 270, ICHAIN_STOP),
};

#endif

extern ColliderCylinderInit D_80966340;
extern InitChainEntry D_8096636C[];

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Mm/EnMm_SetupAction.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Mm/func_80965BBC.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Mm/EnMm_Init.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Mm/EnMm_Destroy.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Mm/func_80965D3C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Mm/func_80965DB4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Mm/func_8096611C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Mm/EnMm_Update.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Mm/EnMm_Draw.s")
