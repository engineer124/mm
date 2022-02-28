/*
 * File: z_en_warp_tag.c
 * Overlay: ovl_En_Warp_tag
 * Description: Warp to Trial Entrance
 */

#include "z_en_warp_tag.h"

#define FLAGS (ACTOR_FLAG_1 | ACTOR_FLAG_10 | ACTOR_FLAG_2000000 | ACTOR_FLAG_8000000)

#define THIS ((EnWarptag*)thisx)

void EnWarptag_Init(Actor* thisx, GlobalContext* globalCtx);
void EnWarptag_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnWarptag_Update(Actor* thisx, GlobalContext* globalCtx);

void func_809C085C(EnWarptag* this, GlobalContext* globalCtx);
void func_809C08E0(EnWarptag* this, GlobalContext* globalCtx);
void func_809C09A0(EnWarptag* this, GlobalContext* globalCtx);
void func_809C0A20(EnWarptag* this, GlobalContext* globalCtx);
void func_809C0AB4(EnWarptag* this, GlobalContext* globalCtx);
void func_809C0E30(EnWarptag* this, GlobalContext* globalCtx);

#if 0
const ActorInit En_Warp_tag_InitVars = {
    ACTOR_EN_WARP_TAG,
    ACTORCAT_ITEMACTION,
    FLAGS,
    GAMEPLAY_KEEP,
    sizeof(EnWarptag),
    (ActorFunc)EnWarptag_Init,
    (ActorFunc)EnWarptag_Destroy,
    (ActorFunc)EnWarptag_Update,
    (ActorFunc)NULL,
};

// static InitChainEntry sInitChain[] = {
static InitChainEntry D_809C1008[] = {
    ICHAIN_VEC3F(scale, 1, ICHAIN_CONTINUE),
    ICHAIN_VEC3S(shape.rot, 0, ICHAIN_STOP),
};

#endif

extern InitChainEntry D_809C1008[];

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Warp_tag/EnWarptag_Init.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Warp_tag/EnWarptag_Destroy.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Warp_tag/func_809C085C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Warp_tag/func_809C08E0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Warp_tag/func_809C09A0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Warp_tag/func_809C0A20.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Warp_tag/func_809C0AB4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Warp_tag/func_809C0E30.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Warp_tag/EnWarptag_Update.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Warp_tag/func_809C0F3C.s")
