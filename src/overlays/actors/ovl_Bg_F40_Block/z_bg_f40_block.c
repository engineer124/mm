/*
 * File: z_bg_f40_block.c
 * Overlay: ovl_Bg_F40_Block
 * Description: Stone Tower Block
 */

#include "z_bg_f40_block.h"

#define FLAGS (ACTOR_FLAG_10)

#define THIS ((BgF40Block*)thisx)

void BgF40Block_Init(Actor* thisx, GlobalContext* globalCtx);
void BgF40Block_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgF40Block_Update(Actor* thisx, GlobalContext* globalCtx);
void BgF40Block_Draw(Actor* thisx, GlobalContext* globalCtx);

#if 0
const ActorInit Bg_F40_Block_InitVars = {
    ACTOR_BG_F40_BLOCK,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_F40_OBJ,
    sizeof(BgF40Block),
    (ActorFunc)BgF40Block_Init,
    (ActorFunc)BgF40Block_Destroy,
    (ActorFunc)BgF40Block_Update,
    (ActorFunc)BgF40Block_Draw,
};

// static InitChainEntry sInitChain[] = {
static InitChainEntry D_80BC4668[] = {
    ICHAIN_F32(uncullZoneForward, 4000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 400, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 400, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

#endif

extern InitChainEntry D_80BC4668[];

extern UNK_TYPE D_060043D0;
extern UNK_TYPE D_06004640;

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_F40_Block/func_80BC3980.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_F40_Block/func_80BC3A2C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_F40_Block/func_80BC3B00.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_F40_Block/func_80BC3CA4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_F40_Block/func_80BC3D08.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_F40_Block/func_80BC4038.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_F40_Block/BgF40Block_Init.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_F40_Block/BgF40Block_Destroy.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_F40_Block/func_80BC41AC.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_F40_Block/func_80BC4228.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_F40_Block/func_80BC4344.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_F40_Block/func_80BC4380.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_F40_Block/func_80BC43CC.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_F40_Block/func_80BC4448.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_F40_Block/func_80BC44F4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_F40_Block/func_80BC4530.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_F40_Block/func_80BC457C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_F40_Block/BgF40Block_Update.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Bg_F40_Block/BgF40Block_Draw.s")
