/*
 * File: z_en_go.c
 * Overlay: ovl_En_Go
 * Description: Goron
 */

#include "z_en_go.h"

#define FLAGS 0x02000019

#define THIS ((EnGo*)thisx)

void EnGo_Init(Actor* thisx, GlobalContext* globalCtx);
void EnGo_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnGo_Update(Actor* thisx, GlobalContext* globalCtx);

void func_80A14798(EnGo* this, GlobalContext* globalCtx);
void func_80A149B0(EnGo* this, GlobalContext* globalCtx);
void func_80A14B30(EnGo* this, GlobalContext* globalCtx);
void func_80A14E14(EnGo* this, GlobalContext* globalCtx);
void func_80A14E74(EnGo* this, GlobalContext* globalCtx);
void func_80A14EB0(EnGo* this, GlobalContext* globalCtx);
void func_80A14FC8(EnGo* this, GlobalContext* globalCtx);
void func_80A153FC(EnGo* this, GlobalContext* globalCtx);
void func_80A157C4(EnGo* this, GlobalContext* globalCtx);

#if 0
const ActorInit En_Go_InitVars = {
    ACTOR_EN_GO,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_OF1D_MAP,
    sizeof(EnGo),
    (ActorFunc)EnGo_Init,
    (ActorFunc)EnGo_Destroy,
    (ActorFunc)EnGo_Update,
    (ActorFunc)NULL,
};

// static ColliderSphereInit sSphereInit = {
static ColliderSphereInit D_80A16434 = {
    { COLTYPE_NONE, AT_ON | AT_TYPE_ENEMY, AC_ON | AC_TYPE_PLAYER, OC1_ON | OC1_TYPE_ALL, OC2_TYPE_1, COLSHAPE_SPHERE, },
    { ELEMTYPE_UNK0, { 0x20000000, 0x00, 0x04 }, { 0xF7CFFFFF, 0x00, 0x00 }, TOUCH_ON | TOUCH_SFX_NORMAL, BUMP_ON, OCELEM_ON, },
    { 0, { { 0, 0, 0 }, 0 }, 100 },
};

// static ColliderCylinderInit sCylinderInit = {
static ColliderCylinderInit D_80A16460 = {
    { COLTYPE_METAL, AT_ON | AT_TYPE_ENEMY, AC_ON | AC_TYPE_PLAYER | AC_TYPE_OTHER, OC1_ON | OC1_TYPE_ALL, OC2_TYPE_1, COLSHAPE_CYLINDER, },
    { ELEMTYPE_UNK2, { 0xF7CFFFFF, 0x02, 0x00 }, { 0xF7CFFFFF, 0x00, 0x00 }, TOUCH_ON | TOUCH_SFX_NORMAL, BUMP_ON, OCELEM_ON, },
    { 0, 0, 0, { 0, 0, 0 } },
};

// static ColliderCylinderInit sCylinderInit = {
static ColliderCylinderInit D_80A1648C = {
    { COLTYPE_HIT1, AT_NONE, AC_NONE, OC1_ON | OC1_TYPE_ALL, OC2_TYPE_1, COLSHAPE_CYLINDER, },
    { ELEMTYPE_UNK1, { 0x00000000, 0x00, 0x00 }, { 0x00000000, 0x00, 0x00 }, TOUCH_NONE | TOUCH_SFX_NORMAL, BUMP_NONE, OCELEM_ON, },
    { 0, 0, 0, { 0, 0, 0 } },
};

// sColChkInfoInit
static CollisionCheckInfoInit2 D_80A164B8 = { 0, 0, 0, 0, MASS_IMMOVABLE };

// static DamageTable sDamageTable = {
static DamageTable D_80A164C4 = {
    /* Deku Nut       */ DMG_ENTRY(1, 0x0),
    /* Deku Stick     */ DMG_ENTRY(1, 0x0),
    /* Horse trample  */ DMG_ENTRY(1, 0x0),
    /* Explosives     */ DMG_ENTRY(1, 0x0),
    /* Zora boomerang */ DMG_ENTRY(1, 0x0),
    /* Normal arrow   */ DMG_ENTRY(1, 0x0),
    /* UNK_DMG_0x06   */ DMG_ENTRY(1, 0x0),
    /* Hookshot       */ DMG_ENTRY(1, 0x0),
    /* Goron punch    */ DMG_ENTRY(1, 0xF),
    /* Sword          */ DMG_ENTRY(1, 0x0),
    /* Goron pound    */ DMG_ENTRY(1, 0xF),
    /* Fire arrow     */ DMG_ENTRY(1, 0x2),
    /* Ice arrow      */ DMG_ENTRY(1, 0x0),
    /* Light arrow    */ DMG_ENTRY(1, 0x0),
    /* Goron spikes   */ DMG_ENTRY(1, 0xF),
    /* Deku spin      */ DMG_ENTRY(1, 0x0),
    /* Deku bubble    */ DMG_ENTRY(1, 0x0),
    /* Deku launch    */ DMG_ENTRY(1, 0x0),
    /* UNK_DMG_0x12   */ DMG_ENTRY(1, 0x0),
    /* Zora barrier   */ DMG_ENTRY(1, 0x0),
    /* Normal shield  */ DMG_ENTRY(1, 0x0),
    /* Light ray      */ DMG_ENTRY(1, 0x0),
    /* Thrown object  */ DMG_ENTRY(1, 0x0),
    /* Zora punch     */ DMG_ENTRY(1, 0x0),
    /* Spin attack    */ DMG_ENTRY(1, 0x0),
    /* Sword beam     */ DMG_ENTRY(1, 0x0),
    /* Normal Roll    */ DMG_ENTRY(1, 0x0),
    /* UNK_DMG_0x1B   */ DMG_ENTRY(1, 0x0),
    /* UNK_DMG_0x1C   */ DMG_ENTRY(1, 0x0),
    /* Unblockable    */ DMG_ENTRY(1, 0x0),
    /* UNK_DMG_0x1E   */ DMG_ENTRY(1, 0x0),
    /* Powder Keg     */ DMG_ENTRY(1, 0x0),
};

#endif

extern ColliderSphereInit D_80A16434;
extern ColliderCylinderInit D_80A16460;
extern ColliderCylinderInit D_80A1648C;
extern CollisionCheckInfoInit2 D_80A164B8;
extern DamageTable D_80A164C4;

extern UNK_TYPE D_06000458;
extern UNK_TYPE D_06003258;
extern UNK_TYPE D_060091A8;
extern UNK_TYPE D_06011AC8;
extern UNK_TYPE D_06014D00;

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A10FD0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A11144.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A1143C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A115B4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A118F8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A11BF8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A11EC0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A1203C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A1213C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A121F4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A1222C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A122EC.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A123A0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A124A0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A124FC.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A125BC.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A12660.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A126BC.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A12774.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A12868.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A12954.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A12A64.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A12B78.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A12C48.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A12D6C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A12DF4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A12E80.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A12FE8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A131F8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A132C8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A133A8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A13400.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A134B0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A134F4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A13564.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A136B8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A13728.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A137C0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A139E4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A13B1C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A13E80.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A14018.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A14104.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A141D4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A1428C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A14324.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A143A8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A14430.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A1449C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A144F4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A145AC.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A14668.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A146CC.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A14798.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A149B0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A14B30.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A14E14.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A14E74.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A14EB0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A14FC8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A153FC.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A15684.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A157C4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/EnGo_Init.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/EnGo_Destroy.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/EnGo_Update.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A15B80.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A15D04.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A15E38.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Go/func_80A15FEC.s")
