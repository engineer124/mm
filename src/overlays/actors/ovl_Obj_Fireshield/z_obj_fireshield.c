/*
 * File: z_obj_fireshield.c
 * Overlay: ovl_Obj_Fireshield
 * Description: Ring of fire
 */

#include "z_obj_fireshield.h"

#define FLAGS (ACTOR_FLAG_10)

#define THIS ((ObjFireshield*)thisx)

void ObjFireshield_Init(Actor* thisx, GlobalContext* globalCtx);
void ObjFireshield_Destroy(Actor* thisx, GlobalContext* globalCtx);
void ObjFireshield_Update(Actor* thisx, GlobalContext* globalCtx);
void ObjFireshield_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80A4CABC(ObjFireshield* this, GlobalContext* globalCtx);
void func_80A4CB7C(ObjFireshield* this, GlobalContext* globalCtx);
void func_80A4CCBC(ObjFireshield* this, GlobalContext* globalCtx);
void func_80A4CD28(ObjFireshield* this, GlobalContext* globalCtx);

#if 0
const ActorInit Obj_Fireshield_InitVars = {
    ACTOR_OBJ_FIRESHIELD,
    ACTORCAT_PROP,
    FLAGS,
    GAMEPLAY_KEEP,
    sizeof(ObjFireshield),
    (ActorFunc)ObjFireshield_Init,
    (ActorFunc)ObjFireshield_Destroy,
    (ActorFunc)ObjFireshield_Update,
    (ActorFunc)ObjFireshield_Draw,
};

// static ColliderCylinderInit sCylinderInit = {
static ColliderCylinderInit D_80A4D820 = {
    { COLTYPE_NONE, AT_ON | AT_TYPE_ENEMY, AC_NONE, OC1_ON | OC1_TYPE_PLAYER, OC2_TYPE_2, COLSHAPE_CYLINDER, },
    { ELEMTYPE_UNK0, { 0x20000000, 0x01, 0x04 }, { 0x00000000, 0x00, 0x00 }, TOUCH_ON | TOUCH_SFX_NONE, BUMP_NONE, OCELEM_ON, },
    { 28, 144, 0, { 0, 0, 0 } },
};

// static InitChainEntry sInitChain[] = {
static InitChainEntry D_80A4D8A4[] = {
    ICHAIN_F32(uncullZoneForward, 4000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 400, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

#endif

extern ColliderCylinderInit D_80A4D820;
extern InitChainEntry D_80A4D8A4[];

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Fireshield/func_80A4CA90.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Fireshield/func_80A4CABC.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Fireshield/func_80A4CB7C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Fireshield/func_80A4CC54.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Fireshield/func_80A4CCBC.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Fireshield/func_80A4CD28.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Fireshield/func_80A4CD34.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Fireshield/func_80A4CE28.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Fireshield/func_80A4D174.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Fireshield/func_80A4D1CC.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Fireshield/ObjFireshield_Init.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Fireshield/ObjFireshield_Destroy.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Fireshield/ObjFireshield_Update.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_Obj_Fireshield/ObjFireshield_Draw.s")
