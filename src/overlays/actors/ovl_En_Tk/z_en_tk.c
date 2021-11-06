/*
 * File: z_en_tk.c
 * Overlay: ovl_En_Tk
 * Description: Dampé
 */

#include "z_en_tk.h"

#define FLAGS 0x00000009

#define THIS ((EnTk*)thisx)

void EnTk_Init(Actor* thisx, GlobalContext* globalCtx);
void EnTk_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnTk_Update(Actor* thisx, GlobalContext* globalCtx);
void EnTk_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80AECA90(EnTk* this, GlobalContext* globalCtx);
void func_80AECB6C(EnTk* this, GlobalContext* globalCtx);
void func_80AED610(EnTk* this, GlobalContext* globalCtx);
void func_80AED940(EnTk* this, GlobalContext* globalCtx);
void func_80AEDC4C(EnTk* this, GlobalContext* globalCtx);
void func_80AEDD4C(EnTk* this, GlobalContext* globalCtx);
void func_80AEDF5C(EnTk* this, GlobalContext* globalCtx);
void func_80AEE2C0(EnTk* this, GlobalContext* globalCtx);
void func_80AEE414(EnTk* this, GlobalContext* globalCtx);
void func_80AEE4D0(EnTk* this, GlobalContext* globalCtx);
void func_80AEE6B8(EnTk* this, GlobalContext* globalCtx);
void func_80AEE9B0(EnTk* this, GlobalContext* globalCtx);

#if 0
const ActorInit En_Tk_InitVars = {
    ACTOR_EN_TK,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_TK,
    sizeof(EnTk),
    (ActorFunc)EnTk_Init,
    (ActorFunc)EnTk_Destroy,
    (ActorFunc)EnTk_Update,
    (ActorFunc)EnTk_Draw,
};

// static ColliderCylinderInit sCylinderInit = {
static ColliderCylinderInit D_80AEF830 = {
    { COLTYPE_NONE, AT_NONE, AC_NONE, OC1_ON | OC1_TYPE_ALL, OC2_TYPE_2, COLSHAPE_CYLINDER, },
    { ELEMTYPE_UNK0, { 0x00000000, 0x00, 0x00 }, { 0x00000000, 0x00, 0x00 }, TOUCH_NONE | TOUCH_SFX_NORMAL, BUMP_NONE, OCELEM_ON, },
    { 30, 52, 0, { 0, 0, 0 } },
};

#endif

extern ColliderCylinderInit D_80AEF830;

extern UNK_TYPE D_0600B530;
extern UNK_TYPE D_0600B9E8;

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEC460.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEC524.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEC658.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/EnTk_Init.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/EnTk_Destroy.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AECA3C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AECA90.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AECB0C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AECB6C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AECE0C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AECE60.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AED354.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AED38C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AED4F8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AED544.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AED610.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AED898.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AED940.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEDBEC.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEDC4C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEDCBC.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEDD4C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEDDA0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEDE10.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEDF5C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEE2A8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEE2C0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEE300.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEE374.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEE414.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEE478.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEE4D0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEE650.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEE6B8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEE784.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEE7E0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEE86C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEE9B0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEEA4C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEEAD4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEEB88.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEED38.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEF048.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEF094.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEF15C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEF1B4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEF1C4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEF210.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEF220.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEF278.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEF2C8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEF2D8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/EnTk_Update.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEF5F4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEF65C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/func_80AEF6A4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tk/EnTk_Draw.s")
