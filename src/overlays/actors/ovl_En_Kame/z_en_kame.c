/*
 * File: z_en_kame.c
 * Overlay: ovl_En_Kame
 * Description: Snapper Turtle
 */

#include "z_en_kame.h"

#define FLAGS 0x00000405

#define THIS ((EnKame*)thisx)

void EnKame_Init(Actor* thisx, GlobalContext* globalCtx);
void EnKame_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnKame_Update(Actor* thisx, GlobalContext* globalCtx);
void EnKame_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80AD70EC(EnKame* this, GlobalContext* globalCtx);
void func_80AD7254(EnKame* this, GlobalContext* globalCtx);
void func_80AD7424(EnKame* this, GlobalContext* globalCtx);
void func_80AD7798(EnKame* this, GlobalContext* globalCtx);
void func_80AD7948(EnKame* this, GlobalContext* globalCtx);
void func_80AD7B90(EnKame* this, GlobalContext* globalCtx);
void func_80AD7D40(EnKame* this, GlobalContext* globalCtx);
void func_80AD7E0C(EnKame* this, GlobalContext* globalCtx);
void func_80AD7F10(EnKame* this, GlobalContext* globalCtx);
void func_80AD7FF8(EnKame* this, GlobalContext* globalCtx);
void func_80AD810C(EnKame* this, GlobalContext* globalCtx);
void func_80AD825C(EnKame* this, GlobalContext* globalCtx);
void func_80AD8388(EnKame* this, GlobalContext* globalCtx);

#if 0
const ActorInit En_Kame_InitVars = {
    ACTOR_EN_KAME,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_TL,
    sizeof(EnKame),
    (ActorFunc)EnKame_Init,
    (ActorFunc)EnKame_Destroy,
    (ActorFunc)EnKame_Update,
    (ActorFunc)EnKame_Draw,
};

// static ColliderCylinderInit sCylinderInit = {
static ColliderCylinderInit D_80AD8DE0 = {
    { COLTYPE_HARD, AT_NONE | AT_TYPE_ENEMY, AC_ON | AC_HARD | AC_TYPE_PLAYER, OC1_ON | OC1_TYPE_ALL, OC2_TYPE_1, COLSHAPE_CYLINDER, },
    { ELEMTYPE_UNK0, { 0xF7CFFFFF, 0x00, 0x04 }, { 0xF7CF7FFF, 0x00, 0x00 }, TOUCH_ON | TOUCH_SFX_NORMAL, BUMP_ON | BUMP_HOOKABLE, OCELEM_ON, },
    { 35, 40, 0, { 0, 0, 0 } },
};

// sColChkInfoInit
static CollisionCheckInfoInit D_80AD8E0C = { 3, 15, 30, 80 };

// static DamageTable sDamageTable = {
static DamageTable D_80AD8E14 = {
    /* Deku Nut       */ DMG_ENTRY(0, 0x1),
    /* Deku Stick     */ DMG_ENTRY(1, 0x0),
    /* Horse trample  */ DMG_ENTRY(1, 0x0),
    /* Explosives     */ DMG_ENTRY(1, 0xF),
    /* Zora boomerang */ DMG_ENTRY(1, 0x0),
    /* Normal arrow   */ DMG_ENTRY(1, 0x0),
    /* UNK_DMG_0x06   */ DMG_ENTRY(0, 0x0),
    /* Hookshot       */ DMG_ENTRY(0, 0xD),
    /* Goron punch    */ DMG_ENTRY(1, 0x0),
    /* Sword          */ DMG_ENTRY(1, 0x0),
    /* Goron pound    */ DMG_ENTRY(0, 0xF),
    /* Fire arrow     */ DMG_ENTRY(2, 0x2),
    /* Ice arrow      */ DMG_ENTRY(2, 0x3),
    /* Light arrow    */ DMG_ENTRY(2, 0x4),
    /* Goron spikes   */ DMG_ENTRY(1, 0x0),
    /* Deku spin      */ DMG_ENTRY(0, 0x1),
    /* Deku bubble    */ DMG_ENTRY(1, 0x0),
    /* Deku launch    */ DMG_ENTRY(2, 0xE),
    /* UNK_DMG_0x12   */ DMG_ENTRY(0, 0x1),
    /* Zora barrier   */ DMG_ENTRY(0, 0x5),
    /* Normal shield  */ DMG_ENTRY(0, 0x0),
    /* Light ray      */ DMG_ENTRY(0, 0x0),
    /* Thrown object  */ DMG_ENTRY(1, 0x0),
    /* Zora punch     */ DMG_ENTRY(1, 0x0),
    /* Spin attack    */ DMG_ENTRY(1, 0x0),
    /* Sword beam     */ DMG_ENTRY(0, 0x0),
    /* Normal Roll    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1B   */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1C   */ DMG_ENTRY(0, 0x0),
    /* Unblockable    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1E   */ DMG_ENTRY(0, 0x0),
    /* Powder Keg     */ DMG_ENTRY(1, 0xF),
};

// static InitChainEntry sInitChain[] = {
static InitChainEntry D_80AD8E44[] = {
    ICHAIN_S8(hintId, 1, ICHAIN_CONTINUE),
    ICHAIN_F32(gravity, -1, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 3500, ICHAIN_STOP),
};

#endif

extern ColliderCylinderInit D_80AD8DE0;
extern CollisionCheckInfoInit D_80AD8E0C;
extern DamageTable D_80AD8E14;
extern InitChainEntry D_80AD8E44[];

extern UNK_TYPE D_060008B4;
extern UNK_TYPE D_06000AF4;
extern UNK_TYPE D_06001C68;
extern UNK_TYPE D_06002510;
extern UNK_TYPE D_060027D8;
extern UNK_TYPE D_06002F88;
extern UNK_TYPE D_060031DC;
extern UNK_TYPE D_060035EC;
extern UNK_TYPE D_06004210;
extern UNK_TYPE D_0600823C;

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/EnKame_Init.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/EnKame_Destroy.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD6F34.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD6F9C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD7018.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD70A0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD70EC.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD71B4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD7254.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD73A8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD7424.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD7568.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD75A8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD76CC.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD7798.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD792C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD7948.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD7B18.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD7B90.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD7C54.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD7D40.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD7DA4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD7E0C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD7EC0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD7F10.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD7FA4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD7FF8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD8060.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD810C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD8148.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD825C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD8364.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD8388.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD84C0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/EnKame_Update.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD8A48.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD8AF8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/EnKame_Draw.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD8CEC.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Kame/func_80AD8D64.s")
