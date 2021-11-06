/*
 * File: z_en_mm2.c
 * Overlay: ovl_En_Mm2
 * Description: Postman's Letter to Himself
 */

#include "z_en_mm2.h"

#define FLAGS 0x00000010

#define THIS ((EnMm2*)thisx)

void EnMm2_Init(Actor* thisx, GlobalContext* globalCtx);
void EnMm2_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnMm2_Update(Actor* thisx, GlobalContext* globalCtx);
void EnMm2_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_809A2080(EnMm2* this, GlobalContext* globalCtx);
void func_809A20FC(EnMm2* this, GlobalContext* globalCtx);

#if 0
const ActorInit En_Mm2_InitVars = {
    ACTOR_EN_MM2,
    ACTORCAT_ITEMACTION,
    FLAGS,
    GAMEPLAY_KEEP,
    sizeof(EnMm2),
    (ActorFunc)EnMm2_Init,
    (ActorFunc)EnMm2_Destroy,
    (ActorFunc)EnMm2_Update,
    (ActorFunc)EnMm2_Draw,
};

#endif

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Mm2/EnMm2_Init.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Mm2/EnMm2_Destroy.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Mm2/func_809A2080.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Mm2/func_809A20FC.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Mm2/EnMm2_Update.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Mm2/EnMm2_Draw.s")
