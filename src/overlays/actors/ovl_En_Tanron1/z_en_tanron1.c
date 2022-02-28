/*
 * File: z_en_tanron1.c
 * Overlay: ovl_En_Tanron1
 * Description: Swarm of Moths in Woodfall Temple
 */

#include "z_en_tanron1.h"

#define FLAGS (ACTOR_FLAG_1 | ACTOR_FLAG_4 | ACTOR_FLAG_10 | ACTOR_FLAG_20)

#define THIS ((EnTanron1*)thisx)

void EnTanron1_Init(Actor* thisx, GlobalContext* globalCtx);
void EnTanron1_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnTanron1_Update(Actor* thisx, GlobalContext* globalCtx);
void EnTanron1_Draw(Actor* thisx, GlobalContext* globalCtx);

#if 0
const ActorInit En_Tanron1_InitVars = {
    ACTOR_EN_TANRON1,
    ACTORCAT_ENEMY,
    FLAGS,
    GAMEPLAY_KEEP,
    sizeof(EnTanron1),
    (ActorFunc)EnTanron1_Init,
    (ActorFunc)EnTanron1_Destroy,
    (ActorFunc)EnTanron1_Update,
    (ActorFunc)EnTanron1_Draw,
};

#endif

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tanron1/EnTanron1_Init.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tanron1/EnTanron1_Destroy.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tanron1/func_80BB4E50.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tanron1/EnTanron1_Update.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tanron1/EnTanron1_Draw.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tanron1/func_80BB5318.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tanron1/func_80BB5AAC.s")
