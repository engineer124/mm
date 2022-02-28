/*
 * File: z_en_maruta.c
 * Overlay: ovl_En_Maruta
 * Description: Swordsman's School - Practice Log
 */

#include "z_en_maruta.h"

#define FLAGS (ACTOR_FLAG_1 | ACTOR_FLAG_10)

#define THIS ((EnMaruta*)thisx)

void EnMaruta_Init(Actor* thisx, GlobalContext* globalCtx);
void EnMaruta_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnMaruta_Update(Actor* thisx, GlobalContext* globalCtx);
void EnMaruta_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80B372CC(EnMaruta* this, GlobalContext* globalCtx);
void func_80B3738C(EnMaruta* this, GlobalContext* globalCtx);
void func_80B37428(EnMaruta* this, GlobalContext* globalCtx);
void func_80B374FC(EnMaruta* this, GlobalContext* globalCtx);
void func_80B37950(EnMaruta* this, GlobalContext* globalCtx);
void func_80B379C0(EnMaruta* this, GlobalContext* globalCtx);
void func_80B37A64(EnMaruta* this, GlobalContext* globalCtx);
void func_80B37AA0(EnMaruta* this, GlobalContext* globalCtx);

#if 0
const ActorInit En_Maruta_InitVars = {
    ACTOR_EN_MARUTA,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_MARUTA,
    sizeof(EnMaruta),
    (ActorFunc)EnMaruta_Init,
    (ActorFunc)EnMaruta_Destroy,
    (ActorFunc)EnMaruta_Update,
    (ActorFunc)EnMaruta_Draw,
};

// static ColliderCylinderInit sCylinderInit = {
static ColliderCylinderInit D_80B38AFC = {
    { COLTYPE_NONE, AT_NONE, AC_ON | AC_TYPE_PLAYER, OC1_ON | OC1_TYPE_ALL, OC2_TYPE_1, COLSHAPE_CYLINDER, },
    { ELEMTYPE_UNK1, { 0x00000000, 0x00, 0x00 }, { 0xF7CFFFFF, 0x00, 0x00 }, TOUCH_NONE | TOUCH_SFX_NORMAL, BUMP_ON | BUMP_HOOKABLE, OCELEM_ON, },
    { 12, 65, 0, { 0, 0, 0 } },
};

// static DamageTable sDamageTable = {
static DamageTable D_80B38B28 = {
    /* Deku Nut       */ DMG_ENTRY(0, 0x0),
    /* Deku Stick     */ DMG_ENTRY(0, 0xF),
    /* Horse trample  */ DMG_ENTRY(0, 0x0),
    /* Explosives     */ DMG_ENTRY(0, 0x0),
    /* Zora boomerang */ DMG_ENTRY(0, 0x0),
    /* Normal arrow   */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x06   */ DMG_ENTRY(2, 0xF),
    /* Hookshot       */ DMG_ENTRY(0, 0x1),
    /* Goron punch    */ DMG_ENTRY(2, 0xF),
    /* Sword          */ DMG_ENTRY(1, 0xF),
    /* Goron pound    */ DMG_ENTRY(1, 0xF),
    /* Fire arrow     */ DMG_ENTRY(0, 0x0),
    /* Ice arrow      */ DMG_ENTRY(0, 0x0),
    /* Light arrow    */ DMG_ENTRY(0, 0x0),
    /* Goron spikes   */ DMG_ENTRY(1, 0xF),
    /* Deku spin      */ DMG_ENTRY(0, 0x1),
    /* Deku bubble    */ DMG_ENTRY(0, 0x0),
    /* Deku launch    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x12   */ DMG_ENTRY(0, 0x0),
    /* Zora barrier   */ DMG_ENTRY(0, 0x0),
    /* Normal shield  */ DMG_ENTRY(0, 0x0),
    /* Light ray      */ DMG_ENTRY(0, 0x0),
    /* Thrown object  */ DMG_ENTRY(0, 0xF),
    /* Zora punch     */ DMG_ENTRY(0, 0x0),
    /* Spin attack    */ DMG_ENTRY(1, 0xF),
    /* Sword beam     */ DMG_ENTRY(0, 0x0),
    /* Normal Roll    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1B   */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1C   */ DMG_ENTRY(0, 0x0),
    /* Unblockable    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1E   */ DMG_ENTRY(0, 0x0),
    /* Powder Keg     */ DMG_ENTRY(0, 0x0),
};

// sColChkInfoInit
static CollisionCheckInfoInit2 D_80B38B48 = { 8, 0, 0, 0, MASS_HEAVY };

#endif

extern ColliderCylinderInit D_80B38AFC;
extern DamageTable D_80B38B28;
extern CollisionCheckInfoInit2 D_80B38B48;

extern UNK_TYPE D_06002EC0;

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Maruta/EnMaruta_Init.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Maruta/EnMaruta_Destroy.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Maruta/func_80B372B8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Maruta/func_80B372CC.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Maruta/func_80B37364.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Maruta/func_80B3738C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Maruta/func_80B373F4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Maruta/func_80B37428.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Maruta/func_80B374B8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Maruta/func_80B374FC.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Maruta/func_80B37590.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Maruta/func_80B37950.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Maruta/func_80B37998.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Maruta/func_80B379C0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Maruta/func_80B37A14.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Maruta/func_80B37A64.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Maruta/func_80B37A8C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Maruta/func_80B37AA0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Maruta/func_80B37B78.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Maruta/func_80B37C04.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Maruta/func_80B37C60.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Maruta/func_80B37CA0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Maruta/func_80B37EC0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Maruta/func_80B38028.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Maruta/func_80B38060.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Maruta/func_80B3828C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Maruta/func_80B382E4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Maruta/EnMaruta_Update.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Maruta/EnMaruta_Draw.s")
