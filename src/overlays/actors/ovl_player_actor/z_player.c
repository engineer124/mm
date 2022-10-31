/*
 * File: z_player.c
 * Overlay: ovl_player_actor
 * Description: Player
 */

#include "global.h"
#include "z64quake.h"
#include "z64rumble.h"
#include "z64shrink_window.h"

#include "overlays/actors/ovl_Arms_Hook/z_arms_hook.h"
#include "overlays/actors/ovl_Door_Spiral/z_door_spiral.h"
#include "overlays/actors/ovl_Door_Shutter/z_door_shutter.h"
#include "overlays/actors/ovl_En_Arrow/z_en_arrow.h"
#include "overlays/actors/ovl_En_Bom/z_en_bom.h"
#include "overlays/actors/ovl_En_Boom/z_en_boom.h"
#include "overlays/actors/ovl_En_Box/z_en_box.h"
#include "overlays/actors/ovl_En_Door/z_en_door.h"
#include "overlays/actors/ovl_En_Horse/z_en_horse.h"
#include "overlays/actors/ovl_En_Ishi/z_en_ishi.h"
#include "overlays/actors/ovl_En_Test3/z_en_test3.h"
#include "overlays/actors/ovl_En_Test7/z_en_test7.h"
#include "overlays/actors/ovl_En_Torch2/z_en_torch2.h"

#include "overlays/effects/ovl_Effect_Ss_G_Splash/z_eff_ss_g_splash.h"

#include "objects/gameplay_keep/gameplay_keep.h"

#include "objects/object_link_boy/object_link_boy.h"
#include "objects/object_link_goron/object_link_goron.h"
#include "objects/object_link_zora/object_link_zora.h"
#include "objects/object_link_nuts/object_link_nuts.h"
#include "objects/object_link_child/object_link_child.h"

#define THIS ((Player*)thisx)

void Player_Init(Actor* thisx, PlayState* play);
void Player_Destroy(Actor* thisx, PlayState* play);
void Player_Update(Actor* thisx, PlayState* play);
void Player_Draw(Actor* thisx, PlayState* play);

s32 Player_GrabPlayer(PlayState* play, Player* player);
s32 Player_SetupPlayerCutscene(PlayState* play, Player* this, PlayerCsMode csMode);
void Player_SetupStandingStillMorph(Player* this, PlayState* play);
s32 Player_InflictDamage(PlayState* play, s32 damage);
void Player_TalkWithPlayer(PlayState* play, Actor* actor);
void func_8085B74C(PlayState* play);
void func_8085B820(PlayState* play, s16 arg1);
PlayerActionParam func_8085B854(PlayState* play, Player* player, ItemId itemId);
s32 func_8085B930(PlayState* play, LinkAnimationHeader* talkAnim, s32 animMode);

void Player_UpdateCommon(Player* this, PlayState* play, Input* input);
s32 Player_StartFishing(PlayState* play);
void Player_SetupRestrainedByEnemy(PlayState* play, Player* this);
s32 func_8083A658(PlayState* play, Player* this);
void Player_ChangeItem(PlayState* play, Player* this, PlayerActionParam actionParam);

void Player_UseItem(PlayState* play, Player* this, ItemId item);

s32 func_80848BF4(Player* this, PlayState* play);

void Player_SetupStandingStillType(Player* this, PlayState* play);

void func_808484F0(Player* this);

void func_80838A20(PlayState* play, Player* this);
void Player_StartBremenMarch(PlayState* play, Player* this);
void Player_StartKamaroDance(PlayState* play, Player* this);

typedef enum AnimSfxType {
    /*  1 */ ANIMSFX_TYPE_GENERAL = 1,
    /*  2 */ ANIMSFX_TYPE_FLOOR,
    /*  3 */ ANIMSFX_TYPE_FLOOR_BY_AGE,
    /*  4 */ ANIMSFX_TYPE_VOICE,
    /*  5 */ ANIMSFX_TYPE_FLOOR_LAND, // does not use sfxId
    /*  6 */ ANIMSFX_TYPE_6,          // FLOOR_WALK_Something // does not use sfxId
    /*  7 */ ANIMSFX_TYPE_FLOOR_JUMP, // does not use sfxId
    /*  8 */ ANIMSFX_TYPE_8,          // FLOOR_WALK_Something2 // does not use sfxId
    /*  9 */ ANIMSFX_TYPE_9,          // Uses NA_SE_PL_WALK_LADDER // does not use sfxId, unused
    /* 10 */ ANIMSFX_TYPE_SURFACE,
} AnimSfxType;

#define ANIMSFX_SHIFT_TYPE(type) ((type) << 11)

#define ANIMSFX_CONTINUE (1)
#define ANIMSFX_STOP (0)

#define ANIMSFX_FLAGS(type, frame, cont) \
    (((ANIMSFX_##cont) == ANIMSFX_STOP ? -1 : 1) * (ANIMSFX_SHIFT_TYPE(type) | ((frame)&0x7FF)))

#define ANIMSFX(type, frame, sfxId, cont) \
    { (sfxId), ANIMSFX_FLAGS(type, frame, cont) }

#define ANIMSFX_GET_TYPE(data) ((data)&0x7800)
#define ANIMSFX_GET_FRAME(data) ((data)&0x7FF)

typedef struct AnimSfxEntry {
    /* 0x0 */ u16 sfxId;
    /* 0x2 */ s16 flags; // negative marks the end
} AnimSfxEntry;          // size = 0x4

/* action funcs */
void func_808496AC(Player* this, PlayState* play);
void func_808497A0(Player* this, PlayState* play);
void Player_UnfriendlyZTargetStandingStill(Player* this, PlayState* play);
void Player_FriendlyZTargetStandingStill(Player* this, PlayState* play);
void Player_StandingStill(Player* this, PlayState* play);
void Player_EndSidewalk(Player* this, PlayState* play);
void Player_FriendlyBackwalk(Player* this, PlayState* play);
void func_8084A794(Player* this, PlayState* play);
void func_8084A884(Player* this, PlayState* play);
void Player_Sidewalk(Player* this, PlayState* play);
void Player_Turn(Player* this, PlayState* play);
void func_8084AC84(Player* this, PlayState* play);
void func_8084AEEC(Player* this, PlayState* play);
void func_8084AF9C(Player* this, PlayState* play);
void func_8084B0EC(Player* this, PlayState* play);
void Player_UnfriendlyBackwalk(Player* this, PlayState* play);
void Player_EndUnfriendlyBackwalk(Player* this, PlayState* play);
void func_8084B4A8(Player* this, PlayState* play);
void func_8084B5C0(Player* this, PlayState* play);
void func_8084BAA4(Player* this, PlayState* play);
void func_8084BBF0(Player* this, PlayState* play);
void func_8084BC64(Player* this, PlayState* play);
void func_8084BE40(Player* this, PlayState* play);
void func_8084BF28(Player* this, PlayState* play);
void Player_Die(Player* this, PlayState* play);
void Player_UpdateMidair(Player* this, PlayState* play);
void Player_Rolling(Player* this, PlayState* play);
void func_8084C94C(Player* this, PlayState* play);
void func_8084CA24(Player* this, PlayState* play);
void Player_JumpSlash(Player* this, PlayState* play);
void Player_ChargeSpinAttack(Player* this, PlayState* play);
void Player_WalkChargingSpinAttack(Player* this, PlayState* play);
void Player_SidewalkChargingSpinAttack(Player* this, PlayState* play);
void Player_JumpUpToLedge(Player* this, PlayState* play);
void Player_RunMiniCutsceneFunc(Player* this, PlayState* play);
void Player_MiniCsMovement(Player* this, PlayState* play);
void Player_Door_Open(Player* this, PlayState* play);
void Player_LiftActor(Player* this, PlayState* play);
void Player_LiftSilverBoulder(Player* this, PlayState* play);
void Player_ThrowSilverBoulder(Player* this, PlayState* play);
void Player_FailToLiftActor(Player* this, PlayState* play);
void func_8084E58C(Player* this, PlayState* play);
void Player_StartThrowActor(Player* this, PlayState* play);
void func_8084E724(Player* this, PlayState* play);
void Player_TalkWithActor(Player* this, PlayState* play);
void Player_GrabPushPullWall(Player* this, PlayState* play);
void func_8084EE50(Player* this, PlayState* play);
void func_8084EF9C(Player* this, PlayState* play);
void func_8084F1B8(Player* this, PlayState* play);
void func_8084F3DC(Player* this, PlayState* play);
void func_8084F4E8(Player* this, PlayState* play);
void Player_EndClimb(Player* this, PlayState* play);
void Player_RideHorse(Player* this, PlayState* play);
void func_808505D0(Player* this, PlayState* play);
void Player_UpdateSwimIdle(Player* this, PlayState* play);
void func_80850B18(Player* this, PlayState* play);
void func_80850D68(Player* this, PlayState* play);
void Player_Swim(Player* this, PlayState* play);
void Player_ZTargetSwimming(Player* this, PlayState* play);
void func_808516B4(Player* this, PlayState* play);
void func_808519FC(Player* this, PlayState* play);
void func_80851B58(Player* this, PlayState* play);
void func_80851BD4(Player* this, PlayState* play);
void Player_PlayOcarina(Player* this, PlayState* play);
void Player_ThrowDekuNut(Player* this, PlayState* play);
void Player_GetItem(Player* this, PlayState* play);
void func_80852FD4(Player* this, PlayState* play);
void func_80853194(Player* this, PlayState* play);
void Player_SwingBottle(Player* this, PlayState* play);
void Player_HealWithFairy(Player* this, PlayState* play);
void Player_DropItemFromBottle(Player* this, PlayState* play);
void Player_PresentExchangeItem(Player* this, PlayState* play);
void Player_RestrainedByEnemy(Player* this, PlayState* play);
void Player_SlipOnSlope(Player* this, PlayState* play);
void func_80854010(Player* this, PlayState* play);
void func_808540A0(Player* this, PlayState* play);
void Player_SpawnFromBlueWarp(Player* this, PlayState* play);
void Player_EnterGrotto(Player* this, PlayState* play);
void Player_SetupOpenDoorFromSpawn(Player* this, PlayState* play);
void Player_JumpFromGrotto(Player* this, PlayState* play);
void func_80854430(Player* this, PlayState* play);
void Player_ShootingGalleryPlay(Player* this, PlayState* play);
void func_808546D0(Player* this, PlayState* play);
void func_80854800(Player* this, PlayState* play);
void Player_MeleeWeaponAttack(Player* this, PlayState* play);
void func_80854C70(Player* this, PlayState* play);
void func_808553F4(Player* this, PlayState* play);
void func_80855818(Player* this, PlayState* play);
void Player_SetupElegyShell(Player* this, PlayState* play);
void func_80855AF4(Player* this, PlayState* play);
void func_80855B9C(Player* this, PlayState* play);
void func_80855C28(Player* this, PlayState* play);
void func_80855E08(Player* this, PlayState* play);
void func_808561B0(Player* this, PlayState* play);
void func_80856918(Player* this, PlayState* play);
void func_808573A4(Player* this, PlayState* play);
void func_80857BE8(Player* this, PlayState* play);
void Player_StartCutscene(Player* this, PlayState* play);

s32 Player_SetupCUpBehavior(Player* this, PlayState* play);
s32 Player_SetupOpenDoor(Player* this, PlayState* play);
s32 Player_SetupGetItemOrHoldBehavior(Player* this, PlayState* play);
s32 Player_SetupMountHorse(Player* this, PlayState* play);
s32 Player_SetupSpeakOrCheck(Player* this, PlayState* play);
s32 func_8083DFC4(Player* this, PlayState* play);
s32 func_8083A114(Player* this, PlayState* play);
s32 func_80848570(Player* this, PlayState* play);
s32 func_8083A580(Player* this, PlayState* play);
s32 Player_SetupPutDownOrThrowActor(Player* this, PlayState* play);
s32 func_80839B18(Player* this, PlayState* play);
s32 func_8083A274(Player* this, PlayState* play);
s32 Player_SetupWallJumpBehavior(Player* this, PlayState* play);
s32 Player_SetupItemCutsceneOrFirstPerson(Player* this, PlayState* play);
s32 func_8083A0CC(Player* this, PlayState* play);

s32 Player_SetupStartZTargetDefend(Player* this, PlayState* play);
s32 func_808487B8(Player* this, PlayState* play);
s32 func_80848B6C(Player* this, PlayState* play);
s32 Player_HoldActor(Player* this, PlayState* play);
s32 func_808491B4(Player* this, PlayState* play);
s32 Player_StartChangeItem(Player* this, PlayState* play);
s32 Player_StandingDefend(Player* this, PlayState* play);
s32 func_80848A0C(Player* this, PlayState* play);
s32 func_80848AB0(Player* this, PlayState* play);
s32 func_80848E4C(Player* this, PlayState* play);
s32 func_80849054(Player* this, PlayState* play);
s32 func_8084923C(Player* this, PlayState* play);
s32 func_808492C4(Player* this, PlayState* play);
s32 func_8084933C(Player* this, PlayState* play);
s32 Player_WaitForThrownZoraFins(Player* this, PlayState* play);
s32 Player_CatchZoraFins(Player* this, PlayState* play);

/* Init Mode functions */
void Player_InitMode_NoUpdateOrDraw(PlayState* play, Player* this);
void Player_InitMode_1(PlayState* play, Player* this);
void Player_InitMode_BlueWarp(PlayState* play, Player* this);
void Player_InitMode_3(PlayState* play, Player* this);
void Player_InitMode_4(PlayState* play, Player* this);
void Player_InitMode_5(PlayState* play, Player* this);
void Player_InitMode_6(PlayState* play, Player* this);
void Player_InitMode_7(PlayState* play, Player* this);
void func_80841744(PlayState* play, Player* this);
void func_80841744(PlayState* play, Player* this);
void Player_InitMode_WalkingSlow(PlayState* play, Player* this);
void Player_InitMode_B(PlayState* play, Player* this);
void Player_InitMode_Telescope(PlayState* play, Player* this);
void Player_InitMode_NoMomentum(PlayState* play, Player* this);
void Player_InitMode_WalkingSlow(PlayState* play, Player* this);
void Player_InitMode_WalkingPreserveMomentum(PlayState* play, Player* this);

void func_80859F4C(PlayState* play, Player* this, UNK_TYPE arg2);
void func_80859FCC(PlayState* play, Player* this, UNK_TYPE arg2);
void func_80859FF4(PlayState* play, Player* this, UNK_TYPE arg2);
void func_8085A120(PlayState* play, Player* this, UNK_TYPE arg2);
void func_8085A144(PlayState* play, Player* this, UNK_TYPE arg2);
void func_8085A19C(PlayState* play, Player* this, UNK_TYPE arg2);
void func_8085A1D4(PlayState* play, Player* this, UNK_TYPE arg2);
void func_8085A24C(PlayState* play, Player* this, UNK_TYPE arg2);
void func_8085A2AC(PlayState* play, Player* this, UNK_TYPE arg2);
void func_8085A330(PlayState* play, Player* this, UNK_TYPE arg2);

void func_80859414(PlayState* play, Player* this, UNK_TYPE arg2);
void func_80859AD0(PlayState* play, Player* this, void* arg2);
void func_80859B28(PlayState* play, Player* this, void* arg2);
void func_8085ADA0(PlayState* play, Player* this, UNK_TYPE arg2);
void Player_Cutscene_End(PlayState* play, Player* this, CsCmdActorAction* playerActionCsCmd);
void func_808595B8(PlayState* play, Player* this, CsCmdActorAction* playerActionCsCmd);
void func_80859890(PlayState* play, Player* this, UNK_TYPE arg2);
void func_80859C60(PlayState* play, Player* this, void* arg2);
void func_8085A710(PlayState* play, Player* this, void* anim);
void func_808595B8(PlayState* play, Player* this, CsCmdActorAction* playerActionCsCmd);
void func_80859D70(PlayState* play, Player* this, CsCmdActorAction* playerActionCsCmd);
void func_8085A6C0(PlayState* play, Player* this, void* anim);
void func_80859FF4(PlayState* play, Player* this, UNK_TYPE arg2);
void Player_Cutscene_TranslateReverse(PlayState* play, Player* this, void* arg);
void func_8085A144(PlayState* play, Player* this, UNK_TYPE arg2);
void func_8085A1D4(PlayState* play, Player* this, UNK_TYPE arg2);
void func_8085A364(PlayState* play, Player* this, void* arg2);
void func_808599DC(PlayState* play, Player* this, UNK_TYPE arg2);
void func_8085A40C(PlayState* play, Player* this, void* arg2);
void func_80859708(PlayState* play, Player* this, UNK_TYPE arg2);
void func_8085A2AC(PlayState* play, Player* this, UNK_TYPE arg2);
void func_8085A940(PlayState* play, Player* this, UNK_TYPE arg2);
void func_8085A4A4(PlayState* play, Player* this, void* arg2);
void func_8085A5DC(PlayState* play, Player* this, void* arg2);
void func_8085AA84(PlayState* play, Player* this, UNK_TYPE arg2);
void func_8085A7C0(PlayState* play, Player* this, UNK_TYPE arg2);
void func_8085AA10(PlayState* play, Player* this, UNK_TYPE arg2);
void func_808599DC(PlayState* play, Player* this, UNK_TYPE arg2);
void func_80859AF8(PlayState* play, Player* this, void* arg2);
void func_80859A44(PlayState* play, Player* this, UNK_TYPE arg2);
void func_80859D44(PlayState* play, Player* this, UNK_TYPE arg2);
void func_80859EBC(PlayState* play, Player* this, CsCmdActorAction* arg2);
void func_80859F4C(PlayState* play, Player* this, UNK_TYPE arg2);
void func_80859F4C(PlayState* play, Player* this, UNK_TYPE arg2);

void func_8085A768(PlayState* play, Player* this, void* arg2);
void func_8085AA60(PlayState* play, Player* this, UNK_TYPE arg2);

void func_8085978C(PlayState* play, Player* this, UNK_TYPE arg2);
void func_80859B54(PlayState* play, Player* this, UNK_TYPE arg2);
void func_80859CA0(PlayState* play, Player* this, UNK_TYPE arg2);
void func_808594D0(PlayState* play, Player* this, CsCmdActorAction* playerActionCsCmd);
void func_80859CFC(PlayState* play, Player* this, UNK_TYPE arg2);
void func_8085A66C(PlayState* play, Player* this, void* arg2);
void func_80859FCC(PlayState* play, Player* this, UNK_TYPE arg2);
void func_8085A120(PlayState* play, Player* this, UNK_TYPE arg2);
void func_8085A19C(PlayState* play, Player* this, UNK_TYPE arg2);
void func_8085968C(PlayState* play, Player* this, UNK_TYPE arg2);
void func_8085A24C(PlayState* play, Player* this, UNK_TYPE arg2);
void func_8085A330(PlayState* play, Player* this, UNK_TYPE arg2);
void func_8085A8C4(PlayState* play, Player* this, UNK_TYPE arg2);
void func_8085A530(PlayState* play, Player* this, void* arg2);
void func_8085A768(PlayState* play, Player* this, void* arg2);
void func_8085AA60(PlayState* play, Player* this, UNK_TYPE arg2);
void func_80859990(PlayState* play, Player* this, UNK_TYPE arg2);
void func_80859A10(PlayState* play, Player* this, UNK_TYPE arg2);

// ActionParams
void func_8082F594(PlayState* play, Player* this);
void func_8082F5A4(PlayState* play, Player* this);
void Player_SetupZoraFins(PlayState* play, Player* this);
void func_8082F5C0(PlayState* play, Player* this);
void func_8082F7F4(PlayState* play, Player* this);
void Player_SpawnExplosive(PlayState* play, Player* this);

void func_80858DB4(PlayState* play, Player* this, void* anim);
void func_80858DDC(PlayState* play, Player* this, void* anim);
void func_80858E40(PlayState* play, Player* this, void* anim);
void func_80858E60(PlayState* play, Player* this, void* anim);
void func_80858E80(PlayState* play, Player* this, void* anim);
void func_80858EA0(PlayState* play, Player* this, void* anim);
void func_80858EFC(PlayState* play, Player* this, void* anim);
void func_80858F1C(PlayState* play, Player* this, void* anim);
void func_80858F3C(PlayState* play, Player* this, void* anim);
void func_80858F5C(PlayState* play, Player* this, void* anim);
void func_80858FBC(PlayState* play, Player* this, void* anim);
void func_80859028(PlayState* play, Player* this, void* anim);
void func_808591BC(PlayState* play, Player* this, void* anim);
void func_80858DFC(PlayState* play, Player* this, void* anim);
void func_80858EC0(PlayState* play, Player* this, void* anim);
void func_80858F7C(PlayState* play, Player* this, void* anim);
void func_80858F9C(PlayState* play, Player* this, void* anim);
void func_80859168(PlayState* play, Player* this, void* anim);
void func_80859210(PlayState* play, Player* this, void* arg2);

typedef struct struct_8085C2A4 {
    /* 0x0 */ LinkAnimationHeader* unk_0;
    /* 0x4 */ LinkAnimationHeader* unk_4;
    /* 0x8 */ LinkAnimationHeader* unk_8;
} struct_8085C2A4; // size = 0xC

typedef struct BlureColors {
    u8 p1StartColor[4];
    u8 p2StartColor[4];
    u8 p1EndColor[4];
    u8 p2EndColor[4];
} BlureColors;

typedef void (*PlayerCueInterpretor)(PlayState*, Player*, CsCmdActorAction*);

typedef struct struct_8085DA94 {
    /* 0x0 */ s8 type;
    /* 0x4 */ union {
        void* ptr; // Do not use, required in the absence of designated initialisors
        LinkAnimationHeader* anim;
        PlayerCueInterpretor func;
        AnimSfxEntry* entry;
    };
} struct_8085DA94; // size = 0x8

typedef struct struct_8085E368 {
    /* 0x0 */ Vec3s base;
    /* 0x6 */ Vec3s range;
} struct_8085E368; // size = 0xC

typedef struct struct_8085D910 {
    /* 0x0 */ u8 unk_0;
    /* 0x1 */ u8 unk_1;
    /* 0x2 */ u8 unk_2;
    /* 0x3 */ u8 unk_3;
} struct_8085D910; // size = 0x4

typedef struct struct_8085D848_unk_00 {
    /* 0x00 */ s16 fogNear;
    /* 0x02 */ u8 fogColor[3];
    /* 0x05 */ u8 ambientColor[3];
} struct_8085D848_unk_00; // size = 0x08

typedef struct struct_8085D848_unk_18 {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ u8 color[3];
    /* 0x10 */ s16 radius;
} struct_8085D848_unk_18; // size = 0x14

typedef struct struct_8085D848 {
    /* 0x00 */ struct_8085D848_unk_00 unk_00[3];
    /* 0x18 */ struct_8085D848_unk_18 light[3];
} struct_8085D848; // size = 0x54

typedef struct BottleDropInfo {
    /* 0x0 */ s16 actorId;
    /* 0x2 */ s16 params;
} BottleDropInfo; // size = 0x4

typedef struct BottleCatchInfo {
    /* 0x0 */ s16 actorId;
    /* 0x2 */ s8 actorParams;
    /* 0x3 */ u8 itemId;
    /* 0x4 */ u8 actionParam;
    /* 0x5 */ u8 textId;
} BottleCatchInfo; // size = 0x6

typedef struct GoronPoundDrum {
    /* 0x0 */ u8 armIndex;
    /* 0x4 */ LinkAnimationHeader* anim;
} GoronPoundDrum; // size = 0x8

typedef struct HorseMountAnimInfo {
    /* 0x0 */ LinkAnimationHeader* anim;
    /* 0x4 */ f32 riderOffsetX;
    /* 0x8 */ f32 riderOffsetZ;
} HorseMountAnimInfo; // size = 0xC

typedef struct FallImpactInfo {
    /* 0x0 */ s8 damage;
    /* 0x1 */ u8 sourceIntensity;
    /* 0x2 */ u8 decayTimer;
    /* 0x3 */ u8 decayStep;
    /* 0x4 */ u16 sfxId;
} FallImpactInfo; // size = 0x6

typedef struct AttackAnimInfo {
    /* 0x00 */ LinkAnimationHeader* unk_0;
    /* 0x04 */ LinkAnimationHeader* unk_4;
    /* 0x08 */ LinkAnimationHeader* unk_8;
    /* 0x0C */ u8 unk_C;
    /* 0x0D */ u8 unk_D;
} AttackAnimInfo; // size = 0x10

typedef struct MeleeWeaponDamageInfo {
    /* 0x0 */ s32 dmgFlags;
    // Presumably these two fields are intended for Fierce Deity, but will also work for Deku if it can equip a sword
    /* 0x4 */ u8 dmgTransformedNormal;
    /* 0x5 */ u8 dmgTransformedStrong;
    /* 0x6 */ u8 dmgHumanNormal;
    /* 0x7 */ u8 dmgHumanStrong;
} MeleeWeaponDamageInfo; // size = 0x8

typedef struct ItemChangeAnimInfo {
    /* 0x0 */ LinkAnimationHeader* anim;
    /* 0x4 */ u8 frame;
} ItemChangeAnimInfo; // size = 0x8

typedef struct {
    /* 0x0 */ u8 itemId;
    /* 0x2 */ s16 actorId;
} ExplosiveInfo; // size = 0x4

typedef struct {
    /* 0x0 */ Color_RGB8 ambientColor;
    /* 0x3 */ Color_RGB8 diffuseColor;
    /* 0x6 */ Color_RGB8 fogColor;
    /* 0xA */ s16 fogNear;
    /* 0xC */ s16 fogFar;
} ZoraBarrierLighting; // size = 0xE

typedef struct GetItemEntry {
    /* 0x0 */ u8 itemId;
    /* 0x1 */ u8 field; // various bit-packed data
    /* 0x2 */ s8 gid;   // defines the draw id and chest opening animation
    /* 0x3 */ u8 textId;
    /* 0x4 */ u16 objectId;
} GetItemEntry; // size = 0x6

typedef struct BottleSwingAnimInfo {
    /* 0x0 */ LinkAnimationHeader* bottleSwingAnim;
    /* 0x4 */ LinkAnimationHeader* bottleCatchAnim;
    /* 0x8 */ u8 unk_8;
    /* 0x9 */ u8 unk_9;
} BottleSwingAnimInfo; // size = 0xC

// bss
#if 1
// Vec3f sDogSpawnPos; // sDogSpawnPos // In-function static
f32 sAnalogStickDistance;            // distance of the analog stick to its center
s16 sAnalogStickAngle;               // analog stick angle/yaw
s16 sCameraOffsetAnalogStickAngle;   // analog stick yaw + camera yaw
s32 D_80862B04;                      // boolean, set to the return value of Player_SetupCurrentUpperAction
BgFloorType sPlayerCurrentFloorType; // set to the return value of SurfaceType_GetFloorType
u32 sPlayerCurrentWallFlags;         // SurfaceType wall flags, set to the return value of SurfaceType_GetWallFlags
BgConveyorSpeed sPlayerConveyorSpeedIndex; // SurfaceType_GetConveyorSpeed
s16 sPlayerIsOnFloorConveyor;              // SurfaceType_IsFloorConveyor
s16 D_80862B16;                            // SurfaceType_GetConveyorDirection << 0xA
f32 sPlayerYDistToFloor;                   // sPlayerYDistToFloor = this->actor.world.pos.y - this->actor.floorHeight;
BgFloorProperty sPlayerPrevFloorProperty;  // sPlayerPrevFloorProperty = this->floorProperty; //
                                           // SurfaceType_GetFloorProperty // SurfaceType Get Floor Property
s32 sYawToTouchedWall;                     // ABS_ALT(this->actor.shape.rot.y - BINANG_ADD(this->actor.wallYaw, 0x8000))
s32 D_80862B24; // ABS_ALT(BINANG_SUB(this->currentYaw, BINANG_ADD(this->actor.wallYaw, 0x8000)))
s16 D_80862B28;
s32 sCurrentMask; // sCurrentMask = player->currentMask;
Vec3f D_80862B30;
f32 D_80862B3C;
u32 D_80862B40;             // SurfaceType_GetSlope
Input* sPlayerControlInput; // sPlayerControlInput
s32 sUsingItemAlreadyInHand;
s32 D_80862B4C;
EnvLightSettings D_80862B50; // backup of play->envCtx.lightSettings
s32 D_80862B6C;              // this->skelAnime.moveFlags // sPlayerSkelMoveFlags?
#else
extern Vec3f sDogSpawnPos;
extern f32 sAnalogStickDistance;
extern s16 sAnalogStickAngle;
extern s16 sCameraOffsetAnalogStickAngle;
extern s32 D_80862B04;
extern BgFloorType sPlayerCurrentFloorType;
extern u32 sPlayerCurrentWallFlags;
extern BgConveyorSpeed sPlayerConveyorSpeedIndex;
extern s16 sPlayerIsOnFloorConveyor;
extern s16 D_80862B16;
extern f32 sPlayerYDistToFloor;
extern BgFloorProperty sPlayerPrevFloorProperty;
extern s32 sYawToTouchedWall;
extern s32 D_80862B24;
extern s16 D_80862B28;
extern s32 sCurrentMask;
extern Vec3f D_80862B30;
extern f32 D_80862B3C;
extern u32 D_80862B40;
extern Input* sPlayerControlInput;
extern s32 sUsingItemAlreadyInHand;
extern s32 D_80862B4C;
extern EnvLightSettings D_80862B50;
extern s32 D_80862B6C;
#endif

s32 func_8082DA90(PlayState* play) {
    return (play->transitionTrigger != TRANS_TRIGGER_OFF) || (play->transitionMode != TRANS_MODE_OFF);
}

void Player_StopHorizontalMovement(Player* this) {
    this->linearVelocity = 0.0f;
    this->actor.speedXZ = 0.0f;
}

void Player_ClearAttentionModeAndStopMoving(Player* this) {
    Player_StopHorizontalMovement(this);
    this->attentionMode = PLAYER_ATTENTIONMODE_NONE;
}

s32 func_8082DAFC(PlayState* play) {
    Player* player = GET_PLAYER(play);

    return CHECK_FLAG_ALL(player->actor.flags, ACTOR_FLAG_100);
}

void Player_AnimationPlayOnce(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    LinkAnimation_PlayOnce(play, &this->skelAnime, anim);
}

void Player_AnimationPlayLoop(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    LinkAnimation_PlayLoop(play, &this->skelAnime, anim);
}

void func_8082DB60(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    LinkAnimation_PlayLoopSetSpeed(play, &this->skelAnime, anim, 2.0f / 3.0f);
}

void Player_PlayAnimOnceSlowed(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    LinkAnimation_PlayOnceSetSpeed(play, &this->skelAnime, anim, 2.0f / 3.0f);
}

void Player_AnimationPlayOnceReverse(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    LinkAnimation_Change(play, &this->skelAnime, anim, -2.0f / 3.0f, Animation_GetLastFrame(anim), 0.0f, ANIMMODE_ONCE,
                         0.0f);
}

// Player_AddRootYawToShapeYaw
void func_8082DC28(Player* this) {
    this->skelAnime.jointTable[PLAYER_LIMB_WAIST - 1].y = 0;
}

void Player_DeactivateMeleeWeapon(Player* this) {
    this->stateFlags2 &= ~PLAYER_STATE2_RELEASING_SPIN_ATTACK;
    this->meleeWeaponState = 0;
    this->meleeWeaponInfo[2].active = false;
    this->meleeWeaponInfo[1].active = false;
    this->meleeWeaponInfo[0].active = false;
}

void Player_ResetSubCam(PlayState* play, Player* this) {
    if ((this->subCamId != CAM_ID_NONE) && (play->cameraPtrs[this->subCamId] != NULL)) {
        this->subCamId = CAM_ID_NONE;
    }

    this->stateFlags2 &= ~(PLAYER_STATE2_DIVING | PLAYER_STATE2_800);
}

void Player_DetatchHeldActor(PlayState* play, Player* this) {
    Actor* heldActor = this->heldActor;

    if ((heldActor != NULL) && !Player_IsHoldingHookshot(this)) {
        this->actor.child = NULL;
        this->heldActor = NULL;
        this->interactRangeActor = NULL;
        heldActor->parent = NULL;
        this->stateFlags1 &= ~PLAYER_STATE1_HOLDING_ACTOR;
    }

    if (Player_GetExplosiveHeld(this) >= 0) {
        Player_ChangeItem(play, this, PLAYER_AP_NONE);
        this->heldItemId = ITEM_FE;
    }
}

void Player_ResetAttributes(PlayState* play, Player* this) {
    if ((this->stateFlags1 & PLAYER_STATE1_HOLDING_ACTOR) && (this->heldActor == NULL)) {
        if (this->interactRangeActor != NULL) {
            if (this->getItemId == GI_NONE) {
                this->stateFlags1 &= ~PLAYER_STATE1_HOLDING_ACTOR;
                this->interactRangeActor = NULL;
            }
        } else {
            this->stateFlags1 &= ~PLAYER_STATE1_HOLDING_ACTOR;
        }
    }

    Player_DeactivateMeleeWeapon(this);
    this->attentionMode = PLAYER_ATTENTIONMODE_NONE;
    Player_ResetSubCam(play, this);
    func_800E0238(Play_GetCamera(play, CAM_ID_MAIN));
    this->stateFlags1 &=
        ~(PLAYER_STATE1_4 | PLAYER_STATE1_2000 | PLAYER_STATE1_4000 | PLAYER_STATE1_100000 | PLAYER_STATE1_200000);
    this->stateFlags2 &= ~(PLAYER_STATE2_10 | PLAYER_STATE2_80);
    this->slashCounter = 0;
    this->unk_ADC = 0;
    this->actor.shape.rot.x = 0;
    this->actor.shape.rot.z = 0;
    this->unk_ABC = 0.0f;
    this->unk_AC0 = 0.0f;
}

s32 Player_UnequipItem(PlayState* play, Player* this) {
    if (this->itemActionParam > PLAYER_AP_LAST_USED) {
        Player_UseItem(play, this, ITEM_NONE);
        return true;
    } else {
        return false;
    }
}

void Player_ResetAttributesAndHeldActor(PlayState* play, Player* this) {
    Player_ResetAttributes(play, this);
    Player_DetatchHeldActor(play, this);
}

s32 Player_MashTimerThresholdExceeded(Player* this, s32 timerStep, s32 timerThreshold) {
    s16 temp_v0 = this->analogStickAngle - sAnalogStickAngle;

    this->genericTimer += timerStep + (s16)(ABS_ALT(temp_v0) * fabsf(sAnalogStickDistance) * (1.0f / 0x600F0));

    if (CHECK_BTN_ANY(sPlayerControlInput->press.button, BTN_B | BTN_A)) {
        this->genericTimer += 5;
    }

    return this->genericTimer >= timerThreshold;
}

void Player_SetFreezeFlashTimer(PlayState* play) {
    if (play->actorCtx.freezeFlashTimer == 0) {
        play->actorCtx.freezeFlashTimer = 1;
    }
}

u8 D_8085B9F0[PLAYER_LIMB_MAX] = {
    false, // PLAYER_LIMB_NONE
    false, // PLAYER_LIMB_ROOT
    false, // PLAYER_LIMB_WAIST
    false, // PLAYER_LIMB_LOWER_ROOT
    false, // PLAYER_LIMB_R_THIGH
    false, // PLAYER_LIMB_R_SHIN
    false, // PLAYER_LIMB_R_FOOT
    false, // PLAYER_LIMB_L_THIGH
    false, // PLAYER_LIMB_L_SHIN
    false, // PLAYER_LIMB_L_FOOT
    true,  // PLAYER_LIMB_UPPER_ROOT
    true,  // PLAYER_LIMB_HEAD
    true,  // PLAYER_LIMB_HAT
    true,  // PLAYER_LIMB_COLLAR
    true,  // PLAYER_LIMB_L_SHOULDER
    true,  // PLAYER_LIMB_L_FOREARM
    true,  // PLAYER_LIMB_L_HAND
    true,  // PLAYER_LIMB_R_SHOULDER
    true,  // PLAYER_LIMB_R_FOREARM
    true,  // PLAYER_LIMB_R_HAND
    true,  // PLAYER_LIMB_SHEATH
    true,  // PLAYER_LIMB_TORSO
};
u8 sLeftArmJointCopyFlags[PLAYER_LIMB_MAX] = {
    false, // PLAYER_LIMB_NONE
    false, // PLAYER_LIMB_ROOT
    false, // PLAYER_LIMB_WAIST
    false, // PLAYER_LIMB_LOWER_ROOT
    false, // PLAYER_LIMB_R_THIGH
    false, // PLAYER_LIMB_R_SHIN
    false, // PLAYER_LIMB_R_FOOT
    false, // PLAYER_LIMB_L_THIGH
    false, // PLAYER_LIMB_L_SHIN
    false, // PLAYER_LIMB_L_FOOT
    false, // PLAYER_LIMB_UPPER_ROOT
    false, // PLAYER_LIMB_HEAD
    false, // PLAYER_LIMB_HAT
    false, // PLAYER_LIMB_COLLAR
    true,  // PLAYER_LIMB_L_SHOULDER
    true,  // PLAYER_LIMB_L_FOREARM
    true,  // PLAYER_LIMB_L_HAND
    false, // PLAYER_LIMB_R_SHOULDER
    false, // PLAYER_LIMB_R_FOREARM
    false, // PLAYER_LIMB_R_HAND
    false, // PLAYER_LIMB_SHEATH
    false, // PLAYER_LIMB_TORSO
};
u8 sRightArmJointCopyFlags[PLAYER_LIMB_MAX] = {
    false, // PLAYER_LIMB_NONE
    false, // PLAYER_LIMB_ROOT
    false, // PLAYER_LIMB_WAIST
    false, // PLAYER_LIMB_LOWER_ROOT
    false, // PLAYER_LIMB_R_THIGH
    false, // PLAYER_LIMB_R_SHIN
    false, // PLAYER_LIMB_R_FOOT
    false, // PLAYER_LIMB_L_THIGH
    false, // PLAYER_LIMB_L_SHIN
    false, // PLAYER_LIMB_L_FOOT
    false, // PLAYER_LIMB_UPPER_ROOT
    false, // PLAYER_LIMB_HEAD
    false, // PLAYER_LIMB_HAT
    false, // PLAYER_LIMB_COLLAR
    false, // PLAYER_LIMB_L_SHOULDER
    false, // PLAYER_LIMB_L_FOREARM
    false, // PLAYER_LIMB_L_HAND
    true,  // PLAYER_LIMB_R_SHOULDER
    true,  // PLAYER_LIMB_R_FOREARM
    true,  // PLAYER_LIMB_R_HAND
    false, // PLAYER_LIMB_SHEATH
    false, // PLAYER_LIMB_TORSO
};

void Player_RequestRumble(PlayState* play, Player* this, s32 sourceIntensity, s32 decayTimer, s32 decayStep,
                          s32 distSq) {
    if (this == GET_PLAYER(play)) {
        Rumble_Request(distSq, sourceIntensity, decayTimer, decayStep);
    }
}

// TODO: less dumb name
#define SFX_VOICE_BANK_SIZE 0x20

// sAgeProperties
PlayerAgeProperties sPlayerAgeProperties[PLAYER_FORM_MAX] = {
    {
        // unk_00;
        84.0f,
        // shadowScale;
        90.0f,
        // unk_08;
        1.5f,
        // unk_0C;
        166.5f,
        // unk_10;
        105.0f,
        // unk_14;
        119.100006f,
        // unk_18;
        88.5f,
        // unk_1C;
        61.5f,
        // unk_20;
        28.5f,
        // unk_24;
        54.0f,
        // unk_28;
        75.0f,
        // unk_2C;
        84.0f,
        // unk_30;
        102.0f,
        // unk_34; // height?
        70.0f,
        // unk_38;
        27.0f,
        // unk_3C;
        24.75f,
        // unk_40;
        105.0f,
        // unk_44
        { 9, 0x123F, 0x167 },
        {
            { 8, 0x1256, 0x17C },
            { 9, 0x17EA, 0x167 },
            { 8, 0x1256, 0x17C },
            { 9, 0x17EA, 0x167 },
        },
        {
            { 9, 0x17EA, 0x167 },
            { 9, 0x1E0D, 0x17C },
            { 9, 0x17EA, 0x167 },
            { 9, 0x1E0D, 0x17C },
        },
        {
            { 8, 0x1256, 0x17C },
            { 9, 0x17EA, 0x167 },
            { -0x638, 0x1256, 0x17C },
            { -0x637, 0x17EA, 0x167 },
        },
        // u16 voiceSfxOffset;
        SFX_VOICE_BANK_SIZE * 0,
        // u16 surfaceSfxOffset;
        0x80,
        // f32 unk_98;
        33.0f,
        // f32 unk_9C;
        44.15145f,
        // openChestAnim
        &gPlayerAnim_link_demo_Tbox_open,
        // unk_A4
        &gPlayerAnim_link_demo_back_to_past,
        // unk_A8
        &gPlayerAnim_link_demo_return_to_past,
        // unk_AC
        &gPlayerAnim_link_normal_climb_startA,
        // unk_B0
        &gPlayerAnim_link_normal_climb_startB,
        // unk_B4
        {
            &gPlayerAnim_link_normal_climb_upL,
            &gPlayerAnim_link_normal_climb_upR,
            &gPlayerAnim_link_normal_Fclimb_upL,
            &gPlayerAnim_link_normal_Fclimb_upR,
        },
        // unk_C4
        {
            &gPlayerAnim_link_normal_Fclimb_sideL,
            &gPlayerAnim_link_normal_Fclimb_sideR,
        },
        // unk_CC
        {
            &gPlayerAnim_link_normal_climb_endAL,
            &gPlayerAnim_link_normal_climb_endAR,
        },
        // unk_D4
        {
            &gPlayerAnim_link_normal_climb_endBR,
            &gPlayerAnim_link_normal_climb_endBL,
        },
    },
    {
        // unk_00;
        70.0f,
        // shadowScale;
        90.0f,
        // unk_08;
        0.74f,
        // unk_0C;
        111.0f,
        // unk_10;
        70.0f,
        // unk_14;
        79.4f,
        // unk_18;
        59.0f,
        // unk_1C;
        41.0f,
        // unk_20;
        19.0f,
        // unk_24;
        36.0f,
        // unk_28;
        50.0f,
        // unk_2C;
        56.0f,
        // unk_30;
        68.0f,
        // unk_34; // height?
        70.0f,
        // unk_38;
        19.5f,
        // unk_3C;
        18.2f,
        // unk_40;
        80.0f,
        // unk_44
        { 0x17, 0xF3B, 0xDF },
        {
            { 0x18, 0xF3B, 0xDF },
            { 0x17, 0x14CF, 0xDF },
            { 0x18, 0xF3B, 0xDF },
            { 0x17, 0x14CF, 0xDF },
        },
        {
            { 0x17, 0x14CF, 0xDF },
            { 0x18, 0x1AF2, 0xDF },
            { 0x17, 0x14CF, 0xDF },
            { 0x18, 0x1AF2, 0xDF },
        },
        {
            { 8, 0x1256, 0x17C },
            { 9, 0x17EA, 0x167 },
            { -0x638, 0x1256, 0x17C },
            { -0x637, 0x17EA, 0x167 },
        },
        // u16 voiceSfxOffset;
        SFX_VOICE_BANK_SIZE * 6,
        // u16 surfaceSfxOffset;
        0x150,
        // f32 unk_98;
        -25.0f,
        // f32 unk_9C;
        42.0f,
        // openChestAnim
        &gPlayerAnim_pg_Tbox_open,
        // unk_A4
        &gPlayerAnim_link_demo_back_to_past,
        // unk_A8
        &gPlayerAnim_link_demo_return_to_past,
        // unk_AC
        &gPlayerAnim_pg_climb_startA,
        // unk_B0
        &gPlayerAnim_pg_climb_startB,
        // unk_B4
        {
            &gPlayerAnim_pg_climb_upL,
            &gPlayerAnim_pg_climb_upR,
            &gPlayerAnim_pg_climb_upL,
            &gPlayerAnim_pg_climb_upR,
        },
        // unk_C4
        {
            &gPlayerAnim_link_normal_Fclimb_sideL,
            &gPlayerAnim_link_normal_Fclimb_sideR,
        },
        // unk_CC
        {
            &gPlayerAnim_pg_climb_endAL,
            &gPlayerAnim_pg_climb_endAR,
        },
        // unk_D4
        {
            &gPlayerAnim_pg_climb_endBR,
            &gPlayerAnim_pg_climb_endBL,
        },
    },
    {
        // unk_00;
        56.0f,
        // shadowScale;
        90.0f,
        // unk_08;
        1.0f,
        // unk_0C;
        111.0f,
        // unk_10;
        70.0f,
        // unk_14;
        79.4f,
        // unk_18;
        59.0f,
        // unk_1C;
        41.0f,
        // unk_20;
        19.0f,
        // unk_24;
        36.0f,
        // unk_28;
        50.0f,
        // unk_2C;
        56.0f,
        // unk_30;
        68.0f,
        // unk_34; // height?
        70.0f,
        // unk_38;
        18.0f,
        // unk_3C;
        23.0f,
        // unk_40;
        70.0f,
        // unk_44
        { 0x17, 0x1323, -0x6D },
        {
            { 0x17, 0x1323, -0x58 },
            { 0x17, 0x18B7, -0x6D },
            { 0x17, 0x1323, -0x58 },
            { 0x17, 0x18B7, -0x6D },
        },
        {
            { 0x17, 0x18B7, -0x6D },
            { 0x18, 0x1EDA, -0x58 },
            { 0x17, 0x18B7, -0x6D },
            { 0x18, 0x1EDA, -0x58 },
        },
        {
            { 8, 0x1256, 0x17C },
            { 9, 0x17EA, 0x167 },
            { -0x638, 0x1256, 0x17C },
            { -0x637, 0x17EA, 0x167 },
        },
        // u16 voiceSfxOffset;
        SFX_VOICE_BANK_SIZE * 5,
        // u16 surfaceSfxOffset;
        0x120,
        // f32 unk_98;
        22.0f,
        // f32 unk_9C;
        36.0f,
        // openChestAnim
        &gPlayerAnim_pz_Tbox_open,
        // unk_A4
        &gPlayerAnim_link_demo_back_to_past,
        // unk_A8
        &gPlayerAnim_link_demo_return_to_past,
        // unk_AC
        &gPlayerAnim_pz_climb_startA,
        // unk_B0
        &gPlayerAnim_pz_climb_startB,
        // unk_B4
        {
            &gPlayerAnim_pz_climb_upL,
            &gPlayerAnim_pz_climb_upR,
            &gPlayerAnim_link_normal_Fclimb_upL,
            &gPlayerAnim_link_normal_Fclimb_upR,
        },
        // unk_C4
        {
            &gPlayerAnim_link_normal_Fclimb_sideL,
            &gPlayerAnim_link_normal_Fclimb_sideR,
        },
        // unk_CC
        {
            &gPlayerAnim_pz_climb_endAL,
            &gPlayerAnim_pz_climb_endAR,
        },
        // unk_D4
        {
            &gPlayerAnim_pz_climb_endBR,
            &gPlayerAnim_pz_climb_endBL,
        },
    },
    {
        // unk_00;
        35.0f,
        // shadowScale;
        50.0f,
        // unk_08;
        0.3f,
        // unk_0C;
        71.0f,
        // unk_10;
        50.0f,
        // unk_14;
        49.0f,
        // unk_18;
        39.0f,
        // unk_1C;
        27.0f,
        // unk_20;
        19.0f,
        // unk_24;
        8.0f,
        // unk_28;
        13.6f,
        // unk_2C;
        24.0f,
        // unk_30;
        24.0f,
        // unk_34; // height?
        70.0f,
        // unk_38;
        14.0f,
        // unk_3C;
        12.0f,
        // unk_40;
        55.0f,
        // unk_44
        { -0x18, 0xDED, 0x36C },
        {
            { -0x18, 0xD92, 0x35E },
            { -0x18, 0x1371, 0x3A9 },
            { 8, 0x1256, 0x17C },
            { 9, 0x17EA, 0x167 },
        },
        {
            { -0x18, 0x1371, 0x3A9 },
            { -0x18, 0x195F, 0x3A9 },
            { 9, 0x17EA, 0x167 },
            { 9, 0x1E0D, 0x17C },
        },
        {
            { 8, 0x1256, 0x17C },
            { 9, 0x17EA, 0x167 },
            { -0x638, 0x1256, 0x17C },
            { -0x637, 0x17EA, 0x167 },
        },
        // u16 voiceSfxOffset;
        SFX_VOICE_BANK_SIZE * 4,
        // u16 surfaceSfxOffset;
        0xF0,
        // f32 unk_98;
        -21.0f,
        // f32 unk_9C;
        33.0f,
        // openChestAnim
        &gPlayerAnim_pn_Tbox_open,
        // unk_A4
        &gPlayerAnim_link_demo_back_to_past,
        // unk_A8
        &gPlayerAnim_link_demo_return_to_past,
        // unk_AC
        &gPlayerAnim_clink_normal_climb_startA,
        // unk_B0
        &gPlayerAnim_clink_normal_climb_startB,
        // unk_B4
        {
            &gPlayerAnim_clink_normal_climb_upL,
            &gPlayerAnim_clink_normal_climb_upR,
            &gPlayerAnim_link_normal_Fclimb_upL,
            &gPlayerAnim_link_normal_Fclimb_upR,
        },
        // unk_C4
        {
            &gPlayerAnim_link_normal_Fclimb_sideL,
            &gPlayerAnim_link_normal_Fclimb_sideR,
        },
        // unk_CC
        {
            &gPlayerAnim_clink_normal_climb_endAL,
            &gPlayerAnim_clink_normal_climb_endAR,
        },
        // unk_D4
        {
            &gPlayerAnim_clink_normal_climb_endBR,
            &gPlayerAnim_clink_normal_climb_endBL,
        },
    },
    {
        // unk_00;
        40.0f,
        // shadowScale;
        60.0f,
        // unk_08;
        11.0f / 17.0f,
        // unk_0C;
        71.0f,
        // unk_10;
        50.0f,
        // unk_14;
        49.0f,
        // unk_18;
        39.0f,
        // unk_1C;
        27.0f,
        // unk_20;
        19.0f,
        // unk_24;
        22.0f,
        // unk_28;
        32.4f,
        // unk_2C;
        32.0f,
        // unk_30;
        48.0f,
        // unk_34; // height?
        11.0f / 17.0f * 70.0f,
        // unk_38;
        14.0f,
        // unk_3C;
        12.0f,
        // unk_40;
        55.0f,
        // unk_44
        { -0x18, 0xDED, 0x36C },
        {
            { -0x18, 0xD92, 0x35E },
            { -0x18, 0x1371, 0x3A9 },
            { 8, 0x1256, 0x17C },
            { 9, 0x17EA, 0x167 },
        },
        {
            { -0x18, 0x1371, 0x3A9 },
            { -0x18, 0x195F, 0x3A9 },
            { 9, 0x17EA, 0x167 },
            { 9, 0x1E0D, 0x17C },
        },
        {
            { 8, 0x1256, 0x17C },
            { 9, 0x17EA, 0x167 },
            { -0x638, 0x1256, 0x17C },
            { -0x637, 0x17EA, 0x167 },
        },
        // u16 voiceSfxOffset;
        SFX_VOICE_BANK_SIZE * 1,
        // u16 surfaceSfxOffset;
        0,
        // f32 unk_98;
        22.0f,
        // f32 unk_9C;
        29.4343f,
        // openChestAnim
        &gPlayerAnim_clink_demo_Tbox_open,
        // unk_A4
        &gPlayerAnim_clink_demo_goto_future,
        // unk_A8
        &gPlayerAnim_clink_demo_return_to_future,
        // unk_AC
        &gPlayerAnim_clink_normal_climb_startA,
        // unk_B0
        &gPlayerAnim_clink_normal_climb_startB,
        // unk_B4
        {
            &gPlayerAnim_clink_normal_climb_upL,
            &gPlayerAnim_clink_normal_climb_upR,
            &gPlayerAnim_link_normal_Fclimb_upL,
            &gPlayerAnim_link_normal_Fclimb_upR,
        },
        // unk_C4
        {
            &gPlayerAnim_link_normal_Fclimb_sideL,
            &gPlayerAnim_link_normal_Fclimb_sideR,
        },
        // unk_CC
        {
            &gPlayerAnim_clink_normal_climb_endAL,
            &gPlayerAnim_clink_normal_climb_endAR,
        },
        // unk_D4
        {
            &gPlayerAnim_clink_normal_climb_endBR,
            &gPlayerAnim_clink_normal_climb_endBL,
        },
    },
};

LinkAnimationHeader* D_8085BE84[PLAYER_ANIMGROUP_MAX][PLAYER_ANIMTYPE_MAX] = {
    // PLAYER_ANIMGROUP_0
    {
        &gPlayerAnim_link_normal_wait_free,  // Default idle standing, looking forward
        &gPlayerAnim_link_normal_wait,       // Default idle standing, looking forward, sword and shield
        &gPlayerAnim_link_normal_wait,       // Default idle standing, looking forward, sword and shield
        &gPlayerAnim_link_fighter_wait_long, // Default idle standing, looking forward, two hand weapon
        &gPlayerAnim_link_normal_wait_free,  // Default idle standing, looking forward
        &gPlayerAnim_link_normal_wait_free,  // Default idle standing, looking forward
    },
    // PLAYER_ANIMGROUP_1
    {
        &gPlayerAnim_link_normal_walk_free,
        &gPlayerAnim_link_normal_walk,
        &gPlayerAnim_link_normal_walk,
        &gPlayerAnim_link_fighter_walk_long,
        &gPlayerAnim_link_normal_walk_free,
        &gPlayerAnim_link_normal_walk_free,
    },
    // PLAYER_ANIMGROUP_2
    {
        &gPlayerAnim_link_normal_run_free, // Running with empty hands
        &gPlayerAnim_link_fighter_run,     // Running with Sword and Shield in hands
        &gPlayerAnim_link_normal_run,
        &gPlayerAnim_link_fighter_run_long, // Running with Two handed weapon
        &gPlayerAnim_link_normal_run_free,  // Running with empty hands
        &gPlayerAnim_link_normal_run_free,  // Running with empty hands
    },
    // PLAYER_ANIMGROUP_3
    {
        &gPlayerAnim_link_normal_damage_run_free,
        &gPlayerAnim_link_fighter_damage_run,
        &gPlayerAnim_link_normal_damage_run_free,
        &gPlayerAnim_link_fighter_damage_run_long,
        &gPlayerAnim_link_normal_damage_run_free,
        &gPlayerAnim_link_normal_damage_run_free,
    },
    // PLAYER_ANIMGROUP_4
    {
        &gPlayerAnim_link_normal_waitL_free,
        &gPlayerAnim_link_anchor_waitL,
        &gPlayerAnim_link_anchor_waitL,
        &gPlayerAnim_link_fighter_waitL_long,
        &gPlayerAnim_link_normal_waitL_free,
        &gPlayerAnim_link_normal_waitL_free,
    },
    // PLAYER_ANIMGROUP_5
    {
        &gPlayerAnim_link_normal_waitR_free,
        &gPlayerAnim_link_anchor_waitR,
        &gPlayerAnim_link_anchor_waitR,
        &gPlayerAnim_link_fighter_waitR_long,
        &gPlayerAnim_link_normal_waitR_free,
        &gPlayerAnim_link_normal_waitR_free,
    },
    // PLAYER_ANIMGROUP_6
    {
        &gPlayerAnim_link_fighter_wait2waitR_long,
        &gPlayerAnim_link_normal_wait2waitR,
        &gPlayerAnim_link_normal_wait2waitR,
        &gPlayerAnim_link_fighter_wait2waitR_long,
        &gPlayerAnim_link_fighter_wait2waitR_long,
        &gPlayerAnim_link_fighter_wait2waitR_long,
    },
    // PLAYER_ANIMGROUP_7
    {
        &gPlayerAnim_link_normal_normal2fighter_free,
        &gPlayerAnim_link_fighter_normal2fighter,
        &gPlayerAnim_link_fighter_normal2fighter,
        &gPlayerAnim_link_normal_normal2fighter_free,
        &gPlayerAnim_link_normal_normal2fighter_free,
        &gPlayerAnim_link_normal_normal2fighter_free,
    },
    // PLAYER_ANIMGROUP_8
    {
        &gPlayerAnim_link_demo_doorA_link_free,
        &gPlayerAnim_link_demo_doorA_link,
        &gPlayerAnim_link_demo_doorA_link,
        &gPlayerAnim_link_demo_doorA_link_free,
        &gPlayerAnim_link_demo_doorA_link_free,
        &gPlayerAnim_link_demo_doorA_link_free,
    },
    // PLAYER_ANIMGROUP_9
    {
        &gPlayerAnim_clink_demo_doorA_link,
        &gPlayerAnim_clink_demo_doorA_link,
        &gPlayerAnim_clink_demo_doorA_link,
        &gPlayerAnim_clink_demo_doorA_link,
        &gPlayerAnim_clink_demo_doorA_link,
        &gPlayerAnim_clink_demo_doorA_link,
    },
    // PLAYER_ANIMGROUP_10
    {
        &gPlayerAnim_link_demo_doorB_link_free,
        &gPlayerAnim_link_demo_doorB_link,
        &gPlayerAnim_link_demo_doorB_link,
        &gPlayerAnim_link_demo_doorB_link_free,
        &gPlayerAnim_link_demo_doorB_link_free,
        &gPlayerAnim_link_demo_doorB_link_free,
    },
    // PLAYER_ANIMGROUP_11
    {
        &gPlayerAnim_clink_demo_doorB_link,
        &gPlayerAnim_clink_demo_doorB_link,
        &gPlayerAnim_clink_demo_doorB_link,
        &gPlayerAnim_clink_demo_doorB_link,
        &gPlayerAnim_clink_demo_doorB_link,
        &gPlayerAnim_clink_demo_doorB_link,
    },
    // PLAYER_ANIMGROUP_12
    {
        &gPlayerAnim_link_normal_carryB_free, // Grabbing something from the floor
        &gPlayerAnim_link_normal_carryB, &gPlayerAnim_link_normal_carryB,
        &gPlayerAnim_link_normal_carryB_free, // Grabbing something from the floor
        &gPlayerAnim_link_normal_carryB_free, // Grabbing something from the floor
        &gPlayerAnim_link_normal_carryB_free, // Grabbing something from the floor
    },
    // PLAYER_ANIMGROUP_13
    {
        &gPlayerAnim_link_normal_landing_free,
        &gPlayerAnim_link_normal_landing,
        &gPlayerAnim_link_normal_landing,
        &gPlayerAnim_link_normal_landing_free,
        &gPlayerAnim_link_normal_landing_free,
        &gPlayerAnim_link_normal_landing_free,
    },
    // PLAYER_ANIMGROUP_14
    {
        &gPlayerAnim_link_normal_short_landing_free,
        &gPlayerAnim_link_normal_short_landing,
        &gPlayerAnim_link_normal_short_landing,
        &gPlayerAnim_link_normal_short_landing_free,
        &gPlayerAnim_link_normal_short_landing_free,
        &gPlayerAnim_link_normal_short_landing_free,
    },
    // PLAYER_ANIMGROUP_15
    {
        &gPlayerAnim_link_normal_landing_roll_free,  // Rolling with nothing in hands
        &gPlayerAnim_link_normal_landing_roll,       // Rolling with sword and shield
        &gPlayerAnim_link_normal_landing_roll,       // Rolling with sword and shield
        &gPlayerAnim_link_fighter_landing_roll_long, // Rolling with two hand weapon
        &gPlayerAnim_link_normal_landing_roll_free,  // Rolling with nothing in hands
        &gPlayerAnim_link_normal_landing_roll_free,  // Rolling with nothing in hands
    },
    // PLAYER_ANIMGROUP_16
    {
        &gPlayerAnim_link_normal_hip_down_free, // Rolling bonk
        &gPlayerAnim_link_normal_hip_down,      // Rolling bonk swrod and shield
        &gPlayerAnim_link_normal_hip_down,      // Rolling bonk swrod and shield
        &gPlayerAnim_link_normal_hip_down_long, // Rolling bonk two hand weapon
        &gPlayerAnim_link_normal_hip_down_free, // Rolling bonk
        &gPlayerAnim_link_normal_hip_down_free, // Rolling bonk
    },
    // PLAYER_ANIMGROUP_17
    {
        &gPlayerAnim_link_normal_walk_endL_free,
        &gPlayerAnim_link_normal_walk_endL,
        &gPlayerAnim_link_normal_walk_endL,
        &gPlayerAnim_link_fighter_walk_endL_long,
        &gPlayerAnim_link_normal_walk_endL_free,
        &gPlayerAnim_link_normal_walk_endL_free,
    },
    // PLAYER_ANIMGROUP_18
    {
        &gPlayerAnim_link_normal_walk_endR_free,
        &gPlayerAnim_link_normal_walk_endR,
        &gPlayerAnim_link_normal_walk_endR,
        &gPlayerAnim_link_fighter_walk_endR_long,
        &gPlayerAnim_link_normal_walk_endR_free,
        &gPlayerAnim_link_normal_walk_endR_free,
    },
    // PLAYER_ANIMGROUP_19
    {
        &gPlayerAnim_link_normal_defense_free,
        &gPlayerAnim_link_normal_defense,
        &gPlayerAnim_link_normal_defense,
        &gPlayerAnim_link_normal_defense_free,
        &gPlayerAnim_link_bow_defense,
        &gPlayerAnim_link_normal_defense_free,
    },
    // PLAYER_ANIMGROUP_20
    {
        &gPlayerAnim_link_normal_defense_wait_free,
        &gPlayerAnim_link_normal_defense_wait,
        &gPlayerAnim_link_normal_defense_wait,
        &gPlayerAnim_link_normal_defense_wait_free,
        &gPlayerAnim_link_bow_defense_wait,
        &gPlayerAnim_link_normal_defense_wait_free,
    },
    // PLAYER_ANIMGROUP_21
    {
        &gPlayerAnim_link_normal_defense_end_free,
        &gPlayerAnim_link_normal_defense_end,
        &gPlayerAnim_link_normal_defense_end,
        &gPlayerAnim_link_normal_defense_end_free,
        &gPlayerAnim_link_normal_defense_end_free,
        &gPlayerAnim_link_normal_defense_end_free,
    },
    // PLAYER_ANIMGROUP_22
    {
        &gPlayerAnim_link_normal_side_walk_free,
        &gPlayerAnim_link_normal_side_walk,
        &gPlayerAnim_link_normal_side_walk,
        &gPlayerAnim_link_fighter_side_walk_long,
        &gPlayerAnim_link_normal_side_walk_free,
        &gPlayerAnim_link_normal_side_walk_free,
    },
    // PLAYER_ANIMGROUP_23
    {
        &gPlayerAnim_link_normal_side_walkL_free, // Side walking
        &gPlayerAnim_link_anchor_side_walkL,      // Side walking with sword and shield in hands
        &gPlayerAnim_link_anchor_side_walkL,      // Side walking with sword and shield in hands
        &gPlayerAnim_link_fighter_side_walkL_long,
        &gPlayerAnim_link_normal_side_walkL_free, // Side walking
        &gPlayerAnim_link_normal_side_walkL_free, // Side walking
    },
    // PLAYER_ANIMGROUP_24
    {
        &gPlayerAnim_link_normal_side_walkR_free,
        &gPlayerAnim_link_anchor_side_walkR,
        &gPlayerAnim_link_anchor_side_walkR,
        &gPlayerAnim_link_fighter_side_walkR_long,
        &gPlayerAnim_link_normal_side_walkR_free,
        &gPlayerAnim_link_normal_side_walkR_free,
    },
    // PLAYER_ANIMGROUP_25
    {
        &gPlayerAnim_link_normal_45_turn_free,
        &gPlayerAnim_link_normal_45_turn,
        &gPlayerAnim_link_normal_45_turn,
        &gPlayerAnim_link_normal_45_turn_free,
        &gPlayerAnim_link_normal_45_turn_free,
        &gPlayerAnim_link_normal_45_turn_free,
    },
    // PLAYER_ANIMGROUP_26
    {
        &gPlayerAnim_link_normal_waitL2wait,
        &gPlayerAnim_link_normal_waitL2wait,
        &gPlayerAnim_link_normal_waitL2wait,
        &gPlayerAnim_link_fighter_waitL2wait_long,
        &gPlayerAnim_link_fighter_waitL2wait_long,
        &gPlayerAnim_link_fighter_waitL2wait_long,
    },
    // PLAYER_ANIMGROUP_27
    {
        &gPlayerAnim_link_normal_waitR2wait,
        &gPlayerAnim_link_normal_waitR2wait,
        &gPlayerAnim_link_normal_waitR2wait,
        &gPlayerAnim_link_fighter_waitR2wait_long,
        &gPlayerAnim_link_fighter_waitR2wait_long,
        &gPlayerAnim_link_fighter_waitR2wait_long,
    },
    // PLAYER_ANIMGROUP_28
    {
        &gPlayerAnim_link_normal_throw_free,
        &gPlayerAnim_link_normal_throw,
        &gPlayerAnim_link_normal_throw,
        &gPlayerAnim_link_normal_throw_free,
        &gPlayerAnim_link_normal_throw_free,
        &gPlayerAnim_link_normal_throw_free,
    },
    // PLAYER_ANIMGROUP_29
    {
        &gPlayerAnim_link_normal_put_free,
        &gPlayerAnim_link_normal_put,
        &gPlayerAnim_link_normal_put,
        &gPlayerAnim_link_normal_put_free,
        &gPlayerAnim_link_normal_put_free,
        &gPlayerAnim_link_normal_put_free,
    },
    // PLAYER_ANIMGROUP_30
    {
        &gPlayerAnim_link_normal_back_walk,
        &gPlayerAnim_link_normal_back_walk,
        &gPlayerAnim_link_normal_back_walk,
        &gPlayerAnim_link_normal_back_walk,
        &gPlayerAnim_link_normal_back_walk,
        &gPlayerAnim_link_normal_back_walk,
    },
    // PLAYER_ANIMGROUP_31
    {
        &gPlayerAnim_link_normal_check_free,
        &gPlayerAnim_link_normal_check,
        &gPlayerAnim_link_normal_check,
        &gPlayerAnim_link_normal_check_free,
        &gPlayerAnim_link_normal_check_free,
        &gPlayerAnim_link_normal_check_free,
    },
    // PLAYER_ANIMGROUP_32
    {
        &gPlayerAnim_link_normal_check_wait_free,
        &gPlayerAnim_link_normal_check_wait,
        &gPlayerAnim_link_normal_check_wait,
        &gPlayerAnim_link_normal_check_wait_free,
        &gPlayerAnim_link_normal_check_wait_free,
        &gPlayerAnim_link_normal_check_wait_free,
    },
    // PLAYER_ANIMGROUP_33
    {
        &gPlayerAnim_link_normal_check_end_free,
        &gPlayerAnim_link_normal_check_end,
        &gPlayerAnim_link_normal_check_end,
        &gPlayerAnim_link_normal_check_end_free,
        &gPlayerAnim_link_normal_check_end_free,
        &gPlayerAnim_link_normal_check_end_free,
    },
    // PLAYER_ANIMGROUP_34
    {
        &gPlayerAnim_link_normal_pull_start_free,
        &gPlayerAnim_link_normal_pull_start,
        &gPlayerAnim_link_normal_pull_start,
        &gPlayerAnim_link_normal_pull_start_free,
        &gPlayerAnim_link_normal_pull_start_free,
        &gPlayerAnim_link_normal_pull_start_free,
    },
    // PLAYER_ANIMGROUP_35
    {
        &gPlayerAnim_link_normal_pulling_free,
        &gPlayerAnim_link_normal_pulling,
        &gPlayerAnim_link_normal_pulling,
        &gPlayerAnim_link_normal_pulling_free,
        &gPlayerAnim_link_normal_pulling_free,
        &gPlayerAnim_link_normal_pulling_free,
    },
    // PLAYER_ANIMGROUP_36
    {
        &gPlayerAnim_link_normal_pull_end_free,
        &gPlayerAnim_link_normal_pull_end,
        &gPlayerAnim_link_normal_pull_end,
        &gPlayerAnim_link_normal_pull_end_free,
        &gPlayerAnim_link_normal_pull_end_free,
        &gPlayerAnim_link_normal_pull_end_free,
    },
    // PLAYER_ANIMGROUP_37
    {
        &gPlayerAnim_link_normal_fall_up_free,
        &gPlayerAnim_link_normal_fall_up,
        &gPlayerAnim_link_normal_fall_up,
        &gPlayerAnim_link_normal_fall_up_free,
        &gPlayerAnim_link_normal_fall_up_free,
        &gPlayerAnim_link_normal_fall_up_free,
    },
    // PLAYER_ANIMGROUP_38
    {
        &gPlayerAnim_link_normal_jump_climb_hold_free,
        &gPlayerAnim_link_normal_jump_climb_hold,
        &gPlayerAnim_link_normal_jump_climb_hold,
        &gPlayerAnim_link_normal_jump_climb_hold_free,
        &gPlayerAnim_link_normal_jump_climb_hold_free,
        &gPlayerAnim_link_normal_jump_climb_hold_free,
    },
    // PLAYER_ANIMGROUP_39
    {
        &gPlayerAnim_link_normal_jump_climb_wait_free,
        &gPlayerAnim_link_normal_jump_climb_wait,
        &gPlayerAnim_link_normal_jump_climb_wait,
        &gPlayerAnim_link_normal_jump_climb_wait_free,
        &gPlayerAnim_link_normal_jump_climb_wait_free,
        &gPlayerAnim_link_normal_jump_climb_wait_free,
    },
    // PLAYER_ANIMGROUP_40
    {
        &gPlayerAnim_link_normal_jump_climb_up_free,
        &gPlayerAnim_link_normal_jump_climb_up,
        &gPlayerAnim_link_normal_jump_climb_up,
        &gPlayerAnim_link_normal_jump_climb_up_free,
        &gPlayerAnim_link_normal_jump_climb_up_free,
        &gPlayerAnim_link_normal_jump_climb_up_free,
    },
    // PLAYER_ANIMGROUP_41
    {
        &gPlayerAnim_link_normal_down_slope_slip_end_free,
        &gPlayerAnim_link_normal_down_slope_slip_end,
        &gPlayerAnim_link_normal_down_slope_slip_end,
        &gPlayerAnim_link_normal_down_slope_slip_end_long,
        &gPlayerAnim_link_normal_down_slope_slip_end_free,
        &gPlayerAnim_link_normal_down_slope_slip_end_free,
    },
    // PLAYER_ANIMGROUP_42
    {
        &gPlayerAnim_link_normal_up_slope_slip_end_free,
        &gPlayerAnim_link_normal_up_slope_slip_end,
        &gPlayerAnim_link_normal_up_slope_slip_end,
        &gPlayerAnim_link_normal_up_slope_slip_end_long,
        &gPlayerAnim_link_normal_up_slope_slip_end_free,
        &gPlayerAnim_link_normal_up_slope_slip_end_free,
    },
    // PLAYER_ANIMGROUP_43
    {
        &gPlayerAnim_sude_nwait,
        &gPlayerAnim_lkt_nwait,
        &gPlayerAnim_lkt_nwait,
        &gPlayerAnim_sude_nwait,
        &gPlayerAnim_sude_nwait,
        &gPlayerAnim_sude_nwait,
    }
};

struct_8085C2A4 D_8085C2A4[] = {
    /* 0 / Forward */
    {
        &gPlayerAnim_link_fighter_front_jump,
        &gPlayerAnim_link_fighter_front_jump_end,
        &gPlayerAnim_link_fighter_front_jump_endR,
    },
    /* 1 / Left */
    {
        &gPlayerAnim_link_fighter_Lside_jump,
        &gPlayerAnim_link_fighter_Lside_jump_end,
        &gPlayerAnim_link_fighter_Lside_jump_endL,
    },
    /* 2 / Back */
    {
        &gPlayerAnim_link_fighter_backturn_jump,
        &gPlayerAnim_link_fighter_backturn_jump_end,
        &gPlayerAnim_link_fighter_backturn_jump_endR,
    },
    /* 3 / Right */
    {
        &gPlayerAnim_link_fighter_Rside_jump,
        &gPlayerAnim_link_fighter_Rside_jump_end,
        &gPlayerAnim_link_fighter_Rside_jump_endR,
    },
    /* 4 /  */
    {
        &gPlayerAnim_link_normal_newroll_jump_20f,
        &gPlayerAnim_link_normal_newroll_jump_end_20f,
        &gPlayerAnim_link_normal_newroll_jump_end_20f,
    },
    /* 5 /  */
    {
        &gPlayerAnim_link_normal_newside_jump_20f,
        &gPlayerAnim_link_normal_newside_jump_end_20f,
        &gPlayerAnim_link_normal_newside_jump_end_20f,
    },
};

// sCylinderInit
ColliderCylinderInit D_8085C2EC = {
    {
        COLTYPE_HIT5,
        AT_NONE,
        AC_ON | AC_TYPE_ENEMY,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_PLAYER,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK1,
        { 0x00000000, 0x00, 0x00 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        TOUCH_NONE | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_ON,
    },
    { 12, 60, 0, { 0, 0, 0 } },
};

// sShieldCylinderInit
ColliderCylinderInit D_8085C318 = {
    {
        COLTYPE_METAL,
        AT_ON | AT_TYPE_PLAYER,
        AC_ON | AC_HARD | AC_TYPE_ENEMY,
        OC1_NONE,
        OC2_TYPE_PLAYER,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK2,
        { 0x00100000, 0x00, 0x02 },
        { 0xD7CFFFFF, 0x00, 0x00 },
        TOUCH_NONE | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_ON,
    },
    { 25, 60, 0, { 0, 0, 0 } },
};

// sMeleeWeaponQuadInit
ColliderQuadInit D_8085C344 = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_PLAYER,
        AC_NONE,
        OC1_NONE,
        OC2_TYPE_PLAYER,
        COLSHAPE_QUAD,
    },
    {
        ELEMTYPE_UNK2,
        { 0x00000000, 0x00, 0x01 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_NONE,
        OCELEM_NONE,
    },
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

// sShieldQuadInit
ColliderQuadInit D_8085C394 = {
    {
        COLTYPE_METAL,
        AT_ON | AT_TYPE_PLAYER,
        AC_ON | AC_HARD | AC_TYPE_ENEMY,
        OC1_NONE,
        OC2_TYPE_PLAYER,
        COLSHAPE_QUAD,
    },
    {
        ELEMTYPE_UNK2,
        { 0x00100000, 0x00, 0x00 },
        { 0xD7CFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_NONE,
    },
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

f32 sWaterSpeedScale = 1.0f;
f32 D_8085C3E8 = 1.0f;

// ANIMSFX_TYPE_VOICE
void Player_AnimSfx_PlayVoice(Player* this, u16 sfxId) {
    u16 sfxOffset;

    if (this->currentMask == PLAYER_MASK_GIANT) {
        func_8019F830(&this->actor.projectedPos, sfxId);
    } else if (this->actor.id == ACTOR_PLAYER) {
        if (this->currentMask == PLAYER_MASK_SCENTS) {
            sfxOffset = SFX_VOICE_BANK_SIZE * 7;
        } else {
            sfxOffset = this->ageProperties->voiceSfxOffset;
        }

        Player_PlaySfx(this, sfxOffset + sfxId);
    }
}

u16 sInterruptableSfxIds[] = {
    NA_SE_VO_LI_SWEAT,
    NA_SE_VO_LI_SNEEZE,
    NA_SE_VO_LI_RELAX,
    NA_SE_VO_LI_FALL_L,
};

void Player_StopInterruptableSfx(Player* this) {
    s32 i;
    u16* sfxIdPtr = sInterruptableSfxIds;

    for (i = 0; i < ARRAY_COUNT(sInterruptableSfxIds); i++) {
        AudioSfx_StopById((u16)(*sfxIdPtr + this->ageProperties->voiceSfxOffset));
        sfxIdPtr++;
    }
}

u16 Player_GetFloorSfx(Player* this, u16 sfxId) {
    return sfxId + this->floorSfxOffset;
}

// ANIMSFX_TYPE_FLOOR
void Player_AnimSfx_PlayFloor(Player* this, u16 sfxId) {
    Player_PlaySfx(this, Player_GetFloorSfx(this, sfxId));
}

u16 Player_GetFloorSfxByAge(Player* this, u16 sfxId) {
    return sfxId + this->floorSfxOffset + this->ageProperties->surfaceSfxOffset;
}

// ANIMSFX_TYPE_FLOOR_BY_AGE
void Player_AnimSfx_PlayFloorByAge(Player* this, u16 sfxId) {
    Player_PlaySfx(this, Player_GetFloorSfxByAge(this, sfxId));
}

// ANIMSFX_TYPE_6 and ANIMSFX_TYPE_8
void Player_AnimSfx_PlayFloorWalk(Player* this, f32 freqVolumeLerp) {
    s32 sfxId;

    if (this->currentMask == PLAYER_MASK_GIANT) {
        sfxId = NA_SE_PL_GIANT_WALK;
    } else {
        sfxId = Player_GetFloorSfxByAge(this, NA_SE_PL_WALK_GROUND);
    }

    // Audio_PlaySfx_AtPosForMetalEffectsWithSyncedFreqAndVolume
    func_8019F638(&this->actor.projectedPos, sfxId, freqVolumeLerp);
}

// ANIMSFX_TYPE_FLOOR_JUMP
void Player_AnimSfx_PlayFloorJump(Player* this) {
    Player_PlaySfx(this, Player_GetFloorSfxByAge(this, NA_SE_PL_JUMP_GROUND));
}

// ANIMSFX_TYPE_FLOOR_LAND
void Player_AnimSfx_PlayFloorLand(Player* this) {
    Player_PlaySfx(this, Player_GetFloorSfxByAge(this, NA_SE_PL_LAND_GROUND));
}

void Player_PlayReactableSfx(Player* this, u16 sfxId) {
    Player_PlaySfx(this, sfxId);
    this->stateFlags2 |= PLAYER_STATE2_8;
}

void Player_PlayAnimSfx(Player* this, AnimSfxEntry* entry) {
    s32 cond;

    do {
        s32 data = ABS_ALT(entry->flags);
        s32 type = ANIMSFX_GET_TYPE(data);

        if (LinkAnimation_OnFrame(&this->skelAnime, fabsf(ANIMSFX_GET_FRAME(data)))) {
            if (type == ANIMSFX_SHIFT_TYPE(ANIMSFX_TYPE_GENERAL)) {
                Player_PlaySfx(this, entry->sfxId);
            } else if (type == ANIMSFX_SHIFT_TYPE(ANIMSFX_TYPE_FLOOR)) {
                Player_AnimSfx_PlayFloor(this, entry->sfxId);
            } else if (type == ANIMSFX_SHIFT_TYPE(ANIMSFX_TYPE_FLOOR_BY_AGE)) {
                Player_AnimSfx_PlayFloorByAge(this, entry->sfxId);
            } else if (type == ANIMSFX_SHIFT_TYPE(ANIMSFX_TYPE_VOICE)) {
                Player_AnimSfx_PlayVoice(this, entry->sfxId);
            } else if (type == ANIMSFX_SHIFT_TYPE(ANIMSFX_TYPE_FLOOR_LAND)) {
                Player_AnimSfx_PlayFloorLand(this);
            } else if (type == ANIMSFX_SHIFT_TYPE(ANIMSFX_TYPE_6)) {
                Player_AnimSfx_PlayFloorWalk(this, 6.0f);
            } else if (type == ANIMSFX_SHIFT_TYPE(ANIMSFX_TYPE_FLOOR_JUMP)) {
                Player_AnimSfx_PlayFloorJump(this);
            } else if (type == ANIMSFX_SHIFT_TYPE(ANIMSFX_TYPE_8)) {
                Player_AnimSfx_PlayFloorWalk(this, 0.0f);
            } else if (type == ANIMSFX_SHIFT_TYPE(ANIMSFX_TYPE_9)) {
                // Audio_PlaySfx_AtPosForMetalEffectsWithSyncedFreqAndVolume
                func_8019F638(&this->actor.projectedPos, this->ageProperties->surfaceSfxOffset + NA_SE_PL_WALK_LADDER,
                              0.0f);
            } else if (type == ANIMSFX_SHIFT_TYPE(ANIMSFX_TYPE_SURFACE)) {
                Player_PlaySfx(this, entry->sfxId + this->ageProperties->surfaceSfxOffset);
            }
        }

        cond = entry->flags >= 0;
        entry++;
    } while (cond);
}

void Player_ChangeAnimMorphToLastFrame(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    LinkAnimation_Change(play, &this->skelAnime, anim, 3.0f / 3.0f, 0.0f, Animation_GetLastFrame(anim), ANIMMODE_ONCE,
                         -6.0f);
}

void Player_ChangeAnimSlowedMorphToLastFrame(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    LinkAnimation_Change(play, &this->skelAnime, anim, 2.0f / 3.0f, 0.0f, Animation_GetLastFrame(anim), ANIMMODE_ONCE,
                         -6.0f);
}

void Player_ChangeAnimShortMorphLoop(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    LinkAnimation_Change(play, &this->skelAnime, anim, 3.0f / 3.0f, 0.0f, 0.0f, ANIMMODE_LOOP, -6.0f);
}

void Player_ChangeAnimShortSlowedMorphLoop(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    LinkAnimation_Change(play, &this->skelAnime, anim, 2.0f / 3.0f, 0.0f, 0.0f, ANIMMODE_LOOP, -6.0f);
}

void Player_ChangeAnimOnce(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    LinkAnimation_Change(play, &this->skelAnime, anim, 3.0f / 3.0f, 0.0f, 0.0f, ANIMMODE_ONCE, 0.0f);
}

void Player_ChangeAnimSlowedOnce(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    LinkAnimation_Change(play, &this->skelAnime, anim, 2.0f / 3.0f, 0.0f, 0.0f, ANIMMODE_ONCE, 0.0f);
}

void Player_ChangeAnimLongMorphLoop(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    LinkAnimation_Change(play, &this->skelAnime, anim, 3.0f / 3.0f, 0.0f, 0.0f, ANIMMODE_LOOP, -16.0f);
}

// Player_AnimationPlayLoopIfCurrentFinished
s32 Player_LoopAnimContinuously(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    if (LinkAnimation_Update(play, &this->skelAnime)) {
        Player_AnimationPlayLoop(play, this, anim);
        return true;
    } else {
        return false;
    }
}

void Player_AnimUpdatePrevTranslRot(Player* this) {
    this->skelAnime.prevTransl = this->skelAnime.baseTransl;
    this->skelAnime.prevRot = this->actor.shape.rot.y;
}

void Player_AnimUpdatePrevTranslRotApplyAgeScale(Player* this) {
    Player_AnimUpdatePrevTranslRot(this);
    this->skelAnime.prevTransl.x *= this->ageProperties->unk_08;
    this->skelAnime.prevTransl.y *= this->ageProperties->unk_08;
    this->skelAnime.prevTransl.z *= this->ageProperties->unk_08;
}

void Player_ClearRootLimbPosY(Player* this) {
    this->skelAnime.jointTable[PLAYER_LIMB_WAIST - 1].y = 0;
}

void Player_EndAnimMovement(Player* this) {
    if (this->skelAnime.moveFlags) {
        func_8082DC28(this);
        this->skelAnime.jointTable[PLAYER_LIMB_ROOT - 1].x = this->skelAnime.baseTransl.x;
        this->skelAnime.jointTable[PLAYER_LIMB_ROOT - 1].z = this->skelAnime.baseTransl.z;

        if (this->skelAnime.moveFlags & 8) {
            if (this->skelAnime.moveFlags & ANIM_FLAG_UPDATE_Y) {
                this->skelAnime.jointTable[PLAYER_LIMB_ROOT - 1].y = this->skelAnime.prevTransl.y;
            }
        } else {
            this->skelAnime.jointTable[PLAYER_LIMB_ROOT - 1].y = this->skelAnime.baseTransl.y;
        }
        Player_AnimUpdatePrevTranslRot(this);
        this->skelAnime.moveFlags = 0;
    }
}

void Player_UpdateAnimMovement(Player* this, s32 moveFlags) {
    Vec3f pos;

    this->skelAnime.moveFlags = moveFlags;
    this->skelAnime.prevTransl = this->skelAnime.baseTransl;
    SkelAnime_UpdateTranslation(&this->skelAnime, &pos, this->actor.shape.rot.y);

    if (moveFlags & 1) {
        pos.x *= this->ageProperties->unk_08;
        pos.z *= this->ageProperties->unk_08;
        this->actor.world.pos.x += pos.x * this->actor.scale.x;
        this->actor.world.pos.z += pos.z * this->actor.scale.z;
    }

    if (moveFlags & ANIM_FLAG_UPDATE_Y) {
        if (!(moveFlags & 4)) {
            pos.y *= this->ageProperties->unk_08;
        }
        this->actor.world.pos.y += pos.y * this->actor.scale.y;
    }

    func_8082DC28(this);
}

void Player_SetupAnimMovement(PlayState* play, Player* this, s32 moveFlags) {
    if (moveFlags & 0x200) {
        Player_AnimUpdatePrevTranslRotApplyAgeScale(this);
    } else if ((moveFlags & 0x100) || this->skelAnime.moveFlags) {
        Player_AnimUpdatePrevTranslRot(this);
    } else {
        this->skelAnime.prevTransl = this->skelAnime.jointTable[PLAYER_LIMB_ROOT - 1];
        this->skelAnime.prevRot = this->actor.shape.rot.y;
    }

    this->skelAnime.moveFlags = moveFlags;
    Player_StopHorizontalMovement(this);
    AnimationContext_DisableQueue(play);
}

void Player_PlayAnimOnceWithMovementSetSpeed(PlayState* play, Player* this, LinkAnimationHeader* anim, s32 moveFlags,
                                             f32 playSpeed) {
    LinkAnimation_PlayOnceSetSpeed(play, &this->skelAnime, anim, playSpeed);
    Player_SetupAnimMovement(play, this, moveFlags);
}

void Player_PlayAnimOnceWithMovement(PlayState* play, Player* this, LinkAnimationHeader* anim, s32 moveFlags) {
    Player_PlayAnimOnceWithMovementSetSpeed(play, this, anim, moveFlags, 3.0f / 3.0f);
}

void Player_PlayAnimOnceWithMovementSlowed(PlayState* play, Player* this, LinkAnimationHeader* anim, s32 moveFlags) {
    Player_PlayAnimOnceWithMovementSetSpeed(play, this, anim, moveFlags, 2.0f / 3.0f);
}

void Player_PlayAnimOnceWithMovementPresetFlagsSlowed(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    Player_PlayAnimOnceWithMovementSlowed(play, this, anim, 0x20C);
}

void Player_PlayAnimLoopWithMovementSetSpeed(PlayState* play, Player* this, LinkAnimationHeader* anim, s32 moveFlags,
                                             f32 playSpeed) {
    LinkAnimation_PlayLoopSetSpeed(play, &this->skelAnime, anim, playSpeed);
    Player_SetupAnimMovement(play, this, moveFlags);
}

void Player_PlayAnimLoopWithMovement(PlayState* play, Player* this, LinkAnimationHeader* anim, s32 moveFlags) {
    Player_PlayAnimLoopWithMovementSetSpeed(play, this, anim, moveFlags, 3.0f / 3.0f);
}

void Player_PlayAnimLoopWithMovementSlowed(PlayState* play, Player* this, LinkAnimationHeader* anim, s32 arg3) {
    Player_PlayAnimLoopWithMovementSetSpeed(play, this, anim, arg3, 2.0f / 3.0f);
}

void Player_PlayAnimLoopWithMovementPresetFlagsSlowed(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    Player_PlayAnimLoopWithMovementSlowed(play, this, anim, 0x1C);
}

// Stores four consecutive frames of analog stick input data into two buffers, one offset by cam angle and the other not
void Player_StoreAnalogStickInput(PlayState* play, Player* this) {
    s8 scaledCamOffsetStickAngle;
    s8 scaledStickAngle;

    this->analogStickDistance = sAnalogStickDistance;
    this->analogStickAngle = sAnalogStickAngle;

    // Get analog stick dist and angle, stick dist ranges from -60.0f to 60.0f on each axis
    func_800FF3A0(&sAnalogStickDistance, &sAnalogStickAngle, sPlayerControlInput);

    if (sAnalogStickDistance < 8.0f) {
        sAnalogStickDistance = 0.0f;
    }

    sCameraOffsetAnalogStickAngle = Camera_GetInputDirYaw(play->cameraPtrs[play->activeCamId]) + sAnalogStickAngle;

    // Loops from 0 to 3 over and over
    this->inputFrameCounter = (this->inputFrameCounter + 1) % ARRAY_COUNT(this->analogStickInputs);

    if (sAnalogStickDistance < 55.0f) {
        scaledCamOffsetStickAngle = -1;
        scaledStickAngle = -1;
    } else {
        scaledStickAngle = ((u16)(sAnalogStickAngle + 0x2000)) >> 9;
        scaledCamOffsetStickAngle =
            ((u16)(BINANG_SUB(sCameraOffsetAnalogStickAngle, this->actor.shape.rot.y) + 0x2000)) >> 14;
    }

    this->analogStickInputs[this->inputFrameCounter] = scaledStickAngle;
    this->relativeAnalogStickInputs[this->inputFrameCounter] = scaledCamOffsetStickAngle;
}

// If the player is underwater, sWaterSpeedScale will be 0.5f, making the animation play half as fast
void Player_PlayAnimOnceWithWaterInfluence(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    LinkAnimation_PlayOnceSetSpeed(play, &this->skelAnime, anim, sWaterSpeedScale);
}

s32 Player_IsAimingZoraFins(Player* this) {
    return this->stateFlags1 & PLAYER_STATE1_AIMING_ZORAFINS;
}

// TODO: consider what to do with the NONEs: cannot use a zero-argument macro like OoT since the text id is involved.
#define GET_ITEM(itemId, objectId, drawId, textId, field, CHEST_ANIM) \
    { itemId, field, (CHEST_ANIM != CHEST_ANIM_SHORT ? 1 : -1) * (drawId + 1), textId, objectId }

#define CHEST_ANIM_SHORT 0
#define CHEST_ANIM_LONG 1

#define GIFIELD_GET_DROP_TYPE(field) ((field)&0x1F)
#define GIFIELD_20 (1 << 5)
#define GIFIELD_40 (1 << 6)
#define GIFIELD_NO_COLLECTIBLE (1 << 7)
/**
 * `flags` must be 0, GIFIELD_20, GIFIELD_40 or GIFIELD_NO_COLLECTIBLE (which can be or'ed together)
 * `dropType` must be either a value from the `Item00Type` enum or 0 if the `GIFIELD_NO_COLLECTIBLE` flag was used
 */
#define GIFIELD(flags, dropType) ((flags) | (dropType))

GetItemEntry sGetItemTable[GI_MAX - 1] = {
    /* 0x01 */ GET_ITEM(ITEM_RUPEE_GREEN, OBJECT_GI_RUPY, GID_RUPEE_GREEN, 0xC4, GIFIELD(0, ITEM00_RUPEE_GREEN),
                        CHEST_ANIM_SHORT),
    /* 0x02 */
    GET_ITEM(ITEM_RUPEE_BLUE, OBJECT_GI_RUPY, GID_RUPEE_BLUE, 0x2, GIFIELD(0, ITEM00_RUPEE_BLUE), CHEST_ANIM_SHORT),
    /* 0x03 */
    GET_ITEM(ITEM_RUPEE_10, OBJECT_GI_RUPY, GID_RUPEE_RED, 0x3, GIFIELD(0, ITEM00_RUPEE_RED), CHEST_ANIM_SHORT),
    /* 0x04 */
    GET_ITEM(ITEM_RUPEE_RED, OBJECT_GI_RUPY, GID_RUPEE_RED, 0x4, GIFIELD(0, ITEM00_RUPEE_RED), CHEST_ANIM_SHORT),
    /* 0x05 */
    GET_ITEM(ITEM_RUPEE_PURPLE, OBJECT_GI_RUPY, GID_RUPEE_PURPLE, 0x5, GIFIELD(0, ITEM00_RUPEE_PURPLE),
             CHEST_ANIM_SHORT),
    /* 0x06 */
    GET_ITEM(ITEM_RUPEE_SILVER, OBJECT_GI_RUPY, GID_RUPEE_SILVER, 0x6, GIFIELD(0, ITEM00_RUPEE_PURPLE),
             CHEST_ANIM_SHORT),
    /* 0x07 */
    GET_ITEM(ITEM_RUPEE_HUGE, OBJECT_GI_RUPY, GID_RUPEE_HUGE, 0x7, GIFIELD(0, ITEM00_RUPEE_HUGE), CHEST_ANIM_SHORT),
    /* 0x08 */
    GET_ITEM(ITEM_WALLET_ADULT, OBJECT_GI_PURSE, GID_WALLET_ADULT, 0x8, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x09 */
    GET_ITEM(ITEM_WALLET_GIANT, OBJECT_GI_PURSE, GID_WALLET_GIANT, 0x9, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x0A */
    GET_ITEM(ITEM_RECOVERY_HEART, OBJECT_GI_HEART, GID_RECOVERY_HEART, 0xA, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x0B */
    GET_ITEM(ITEM_RECOVERY_HEART, OBJECT_GI_HEART, GID_RECOVERY_HEART, 0xB,
             GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x0C */
    GET_ITEM(ITEM_HEART_PIECE_2, OBJECT_GI_HEARTS, GID_HEART_PIECE, 0xC,
             GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x0D */
    GET_ITEM(ITEM_HEART_CONTAINER, OBJECT_GI_HEARTS, GID_HEART_CONTAINER, 0xD,
             GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x0E */
    GET_ITEM(ITEM_MAGIC_SMALL, OBJECT_GI_MAGICPOT, GID_MAGIC_SMALL, 0xE,
             GIFIELD(GIFIELD_20 | GIFIELD_40, ITEM00_MAGIC_SMALL), CHEST_ANIM_SHORT),
    /* 0x0F */
    GET_ITEM(ITEM_MAGIC_LARGE, OBJECT_GI_MAGICPOT, GID_MAGIC_LARGE, 0xF,
             GIFIELD(GIFIELD_20 | GIFIELD_40, ITEM00_MAGIC_LARGE), CHEST_ANIM_SHORT),
    /* 0x10 */
    GET_ITEM(ITEM_RECOVERY_HEART, OBJECT_GI_HEART, GID_RECOVERY_HEART, 0x10, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x11 */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0x11, 0, 0),
    /* 0x12 */
    GET_ITEM(ITEM_RECOVERY_HEART, OBJECT_GI_HEART, GID_RECOVERY_HEART, 0x12, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x13 */
    GET_ITEM(ITEM_RECOVERY_HEART, OBJECT_GI_HEART, GID_RECOVERY_HEART, 0x13, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x14 */
    GET_ITEM(ITEM_BOMB, OBJECT_GI_BOMB_1, GID_BOMB, 0x14, GIFIELD(GIFIELD_40, ITEM00_BOMBS_0), CHEST_ANIM_SHORT),
    /* 0x15 */
    GET_ITEM(ITEM_BOMBS_5, OBJECT_GI_BOMB_1, GID_BOMB, 0x15, GIFIELD(GIFIELD_40, ITEM00_BOMBS_0), CHEST_ANIM_SHORT),
    /* 0x16 */
    GET_ITEM(ITEM_BOMBS_10, OBJECT_GI_BOMB_1, GID_BOMB, 0x16, GIFIELD(GIFIELD_40, ITEM00_BOMBS_0), CHEST_ANIM_SHORT),
    /* 0x17 */
    GET_ITEM(ITEM_BOMBS_20, OBJECT_GI_BOMB_1, GID_BOMB, 0x17, GIFIELD(GIFIELD_40, ITEM00_BOMBS_0), CHEST_ANIM_SHORT),
    /* 0x18 */
    GET_ITEM(ITEM_BOMBS_30, OBJECT_GI_BOMB_1, GID_BOMB, 0x18, GIFIELD(GIFIELD_40, ITEM00_BOMBS_0), CHEST_ANIM_SHORT),
    /* 0x19 */ GET_ITEM(ITEM_STICK, OBJECT_GI_STICK, GID_STICK, 0x19, GIFIELD(0, ITEM00_STICK), CHEST_ANIM_SHORT),
    /* 0x1A */
    GET_ITEM(ITEM_BOMBCHUS_10, OBJECT_GI_BOMB_2, GID_BOMBCHU, 0x1A, GIFIELD(GIFIELD_40 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_SHORT),
    /* 0x1B */
    GET_ITEM(ITEM_BOMB_BAG_20, OBJECT_GI_BOMBPOUCH, GID_BOMB_BAG_20, 0x1B,
             GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x1C */
    GET_ITEM(ITEM_BOMB_BAG_30, OBJECT_GI_BOMBPOUCH, GID_BOMB_BAG_30, 0x1C,
             GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x1D */
    GET_ITEM(ITEM_BOMB_BAG_40, OBJECT_GI_BOMBPOUCH, GID_BOMB_BAG_40, 0x1D,
             GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x1E */
    GET_ITEM(ITEM_ARROWS_10, OBJECT_GI_ARROW, GID_ARROWS_SMALL, 0x1E, GIFIELD(GIFIELD_40, ITEM00_ARROWS_30),
             CHEST_ANIM_SHORT),
    /* 0x1F */
    GET_ITEM(ITEM_ARROWS_30, OBJECT_GI_ARROW, GID_ARROWS_MEDIUM, 0x1F, GIFIELD(GIFIELD_40, ITEM00_ARROWS_40),
             CHEST_ANIM_SHORT),
    /* 0x20 */
    GET_ITEM(ITEM_ARROWS_40, OBJECT_GI_ARROW, GID_ARROWS_LARGE, 0x20, GIFIELD(GIFIELD_40, ITEM00_ARROWS_50),
             CHEST_ANIM_SHORT),
    /* 0x21 */
    GET_ITEM(ITEM_ARROWS_40, OBJECT_GI_ARROW, GID_ARROWS_LARGE, 0x21, GIFIELD(GIFIELD_40, ITEM00_ARROWS_50),
             CHEST_ANIM_SHORT),
    /* 0x22 */
    GET_ITEM(ITEM_BOW, OBJECT_GI_BOW, GID_BOW, 0x22, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x23 */
    GET_ITEM(ITEM_QUIVER_40, OBJECT_GI_ARROWCASE, GID_QUIVER_40, 0x23, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x24 */
    GET_ITEM(ITEM_QUIVER_50, OBJECT_GI_ARROWCASE, GID_QUIVER_50, 0x24, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x25 */
    GET_ITEM(ITEM_ARROW_FIRE, OBJECT_GI_M_ARROW, GID_ARROW_FIRE, 0x25, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x26 */
    GET_ITEM(ITEM_ARROW_ICE, OBJECT_GI_M_ARROW, GID_ARROW_ICE, 0x26, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x27 */
    GET_ITEM(ITEM_ARROW_LIGHT, OBJECT_GI_M_ARROW, GID_ARROW_LIGHT, 0x27,
             GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x28 */ GET_ITEM(ITEM_NUT, OBJECT_GI_NUTS, GID_NUTS, 0x28, GIFIELD(0, ITEM00_NUTS_1), CHEST_ANIM_SHORT),
    /* 0x29 */ GET_ITEM(ITEM_NUTS_5, OBJECT_GI_NUTS, GID_NUTS, 0x29, GIFIELD(0, ITEM00_NUTS_1), CHEST_ANIM_SHORT),
    /* 0x2A */ GET_ITEM(ITEM_NUTS_10, OBJECT_GI_NUTS, GID_NUTS, 0x2A, GIFIELD(0, ITEM00_NUTS_1), CHEST_ANIM_SHORT),
    /* 0x2B */
    GET_ITEM(ITEM_NUT_UPGRADE_30, OBJECT_GI_NUTS, GID_NUTS, 0x2B, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_SHORT),
    /* 0x2C */
    GET_ITEM(ITEM_NUT_UPGRADE_30, OBJECT_GI_NUTS, GID_NUTS, 0x2C, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_SHORT),
    /* 0x2D */
    GET_ITEM(ITEM_NUT_UPGRADE_40, OBJECT_GI_NUTS, GID_NUTS, 0x2D, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_SHORT),
    /* 0x2E */
    GET_ITEM(ITEM_BOMBCHUS_20, OBJECT_GI_BOMB_2, GID_BOMBCHU, 0x2E, GIFIELD(GIFIELD_40 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_SHORT),
    /* 0x2F */
    GET_ITEM(ITEM_STICK_UPGRADE_20, OBJECT_GI_STICK, GID_STICK, 0x2F, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_SHORT),
    /* 0x30 */
    GET_ITEM(ITEM_STICK_UPGRADE_20, OBJECT_GI_STICK, GID_STICK, 0x30, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_SHORT),
    /* 0x31 */
    GET_ITEM(ITEM_STICK_UPGRADE_30, OBJECT_GI_STICK, GID_STICK, 0x31, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_SHORT),
    /* 0x32 */
    GET_ITEM(ITEM_SHIELD_HERO, OBJECT_GI_SHIELD_2, GID_SHIELD_HERO, 0x32,
             GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_SHORT),
    /* 0x33 */
    GET_ITEM(ITEM_SHIELD_MIRROR, OBJECT_GI_SHIELD_3, GID_SHIELD_MIRROR, 0x33,
             GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x34 */
    GET_ITEM(ITEM_POWDER_KEG, OBJECT_GI_BIGBOMB, GID_POWDER_KEG, 0x34, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x35 */
    GET_ITEM(ITEM_MAGIC_BEANS, OBJECT_GI_BEAN, GID_MAGIC_BEANS, 0x35, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_SHORT),
    /* 0x36 */
    GET_ITEM(ITEM_BOMBCHUS_1, OBJECT_GI_BOMB_2, GID_BOMBCHU, 0x36, GIFIELD(GIFIELD_40 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_SHORT),
    /* 0x37 */
    GET_ITEM(ITEM_SWORD_KOKIRI, OBJECT_GI_SWORD_1, GID_SWORD_KOKIRI, 0x37,
             GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x38 */
    GET_ITEM(ITEM_SWORD_RAZOR, OBJECT_GI_SWORD_2, GID_SWORD_RAZOR, 0x38,
             GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x39 */
    GET_ITEM(ITEM_SWORD_GILDED, OBJECT_GI_SWORD_3, GID_SWORD_GILDED, 0x39,
             GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x3A */
    GET_ITEM(ITEM_BOMBCHUS_5, OBJECT_GI_BOMB_2, GID_BOMBCHU, 0x3A, GIFIELD(GIFIELD_40 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_SHORT),
    /* 0x3B */
    GET_ITEM(ITEM_SWORD_GREAT_FAIRY, OBJECT_GI_SWORD_4, GID_SWORD_GREAT_FAIRY, 0x3B,
             GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x3C */
    GET_ITEM(ITEM_KEY_SMALL, OBJECT_GI_KEY, GID_KEY_SMALL, 0x3C, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_SHORT),
    /* 0x3D */
    GET_ITEM(ITEM_KEY_BOSS, OBJECT_GI_BOSSKEY, GID_KEY_BOSS, 0x3D, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x3E */
    GET_ITEM(ITEM_DUNGEON_MAP, OBJECT_GI_MAP, GID_DUNGEON_MAP, 0x3E, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x3F */
    GET_ITEM(ITEM_COMPASS, OBJECT_GI_COMPASS, GID_COMPASS, 0x3F, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x40 */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0x40, 0, 0),
    /* 0x41 */
    GET_ITEM(ITEM_HOOKSHOT, OBJECT_GI_HOOKSHOT, GID_HOOKSHOT, 0x41, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x42 */
    GET_ITEM(ITEM_LENS, OBJECT_GI_GLASSES, GID_LENS, 0x42, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x43 */
    GET_ITEM(ITEM_PICTO_BOX, OBJECT_GI_CAMERA, GID_PICTO_BOX, 0x43, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x44 */ GET_ITEM(ITEM_PICTO_BOX, OBJECT_UNSET_0, GID_NONE, 0x44, GIFIELD(0, ITEM00_RUPEE_GREEN), 0),
    /* 0x45 */
    GET_ITEM(ITEM_RECOVERY_HEART, OBJECT_GI_HEART, GID_RECOVERY_HEART, 0x45, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x46 */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0x46, 0, 0),
    /* 0x47 */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0x47, 0, 0),
    /* 0x48 */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0x48, 0, 0),
    /* 0x49 */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0x49, 0, 0),
    /* 0x4A */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0x4A, 0, 0),
    /* 0x4B */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0x4B, 0, 0),
    /* 0x4C */
    GET_ITEM(ITEM_OCARINA, OBJECT_GI_OCARINA, GID_OCARINA, 0x4C, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x4D */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0x4D, 0, 0),
    /* 0x4E */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0x4E, 0, 0),
    /* 0x4F */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0x4F, 0, 0),
    /* 0x50 */
    GET_ITEM(ITEM_BOMBERS_NOTEBOOK, OBJECT_GI_SCHEDULE, GID_BOMBERS_NOTEBOOK, 0x50, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x51 */
    GET_ITEM(ITEM_NONE, OBJECT_GI_MAP, GID_STONE_OF_AGONY, 0x51, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x52 */
    GET_ITEM(ITEM_SKULL_TOKEN, OBJECT_GI_SUTARU, GID_SKULL_TOKEN, 0x52, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_SHORT),
    /* 0x53 */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0x53, 0, 0),
    /* 0x54 */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0x54, 0, 0),
    /* 0x55 */
    GET_ITEM(ITEM_REMAINS_ODOLWA, OBJECT_UNSET_0, GID_REMAINS_ODOLWA, 0x55, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x56 */
    GET_ITEM(ITEM_REMAINS_GOHT, OBJECT_UNSET_0, GID_REMAINS_GOHT, 0x56, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x57 */
    GET_ITEM(ITEM_REMAINS_GYORG, OBJECT_UNSET_0, GID_REMAINS_GYORG, 0x57, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x58 */
    GET_ITEM(ITEM_REMAINS_TWINMOLD, OBJECT_UNSET_0, GID_REMAINS_TWINMOLD, 0x58, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x59 */
    GET_ITEM(ITEM_LONGSHOT, OBJECT_GI_BOTTLE_RED, GID_57, GIFIELD(GIFIELD_40, ITEM00_BOMBS_0),
             GIFIELD(GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x5A */
    GET_ITEM(ITEM_BOTTLE, OBJECT_GI_BOTTLE, GID_BOTTLE, 0x5A, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x5B */
    GET_ITEM(ITEM_POTION_RED, OBJECT_GI_LIQUID, GID_POTION_RED, 0x5B, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x5C */
    GET_ITEM(ITEM_POTION_GREEN, OBJECT_GI_LIQUID, GID_POTION_GREEN, 0x5C, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x5D */
    GET_ITEM(ITEM_POTION_BLUE, OBJECT_GI_LIQUID, GID_POTION_BLUE, 0x5D, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x5E */
    GET_ITEM(ITEM_FAIRY, OBJECT_GI_BOTTLE_04, GID_FAIRY, 0x5E, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x5F */
    GET_ITEM(ITEM_FAIRY, OBJECT_GI_BOTTLE, GID_BOTTLE, 0x5F, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x60 */
    GET_ITEM(ITEM_MILK_BOTTLE, OBJECT_GI_MILK, GID_MILK, 0x60, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x61 */
    GET_ITEM(ITEM_MILK_HALF, OBJECT_GI_MILK, GID_MILK, 0x61, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x62 */ GET_ITEM(ITEM_FISH, OBJECT_GI_FISH, GID_FISH, 0x62, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x63 */ GET_ITEM(ITEM_BUG, OBJECT_GI_INSECT, GID_BUG, 0x63, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x64 */ GET_ITEM(ITEM_BLUE_FIRE, OBJECT_UNSET_0, GID_NONE, 0x64, GIFIELD(0, ITEM00_RUPEE_GREEN), 0),
    /* 0x65 */
    GET_ITEM(ITEM_BOTTLE, OBJECT_GI_BOTTLE, GID_BOTTLE, 0x65, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x66 */
    GET_ITEM(ITEM_BIG_POE, OBJECT_GI_GHOST, GID_BIG_POE, 0x66, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x67 */ GET_ITEM(ITEM_SPRING_WATER, OBJECT_UNSET_0, GID_NONE, 0x67, GIFIELD(0, ITEM00_RUPEE_GREEN), 0),
    /* 0x68 */ GET_ITEM(ITEM_HOT_SPRING_WATER, OBJECT_UNSET_0, GID_NONE, 0x68, GIFIELD(0, ITEM00_RUPEE_GREEN), 0),
    /* 0x69 */
    GET_ITEM(ITEM_ZORA_EGG, OBJECT_GI_BOTTLE_15, GID_ZORA_EGG, 0x69, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x6A */
    GET_ITEM(ITEM_GOLD_DUST, OBJECT_GI_BOTTLE_16, GID_SEAHORSE, 0x6A, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x6B */
    GET_ITEM(ITEM_MUSHROOM, OBJECT_GI_MAGICMUSHROOM, GID_MUSHROOM, 0x6B, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x6C */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0x6C, GIFIELD(0, ITEM00_RUPEE_GREEN), 0),
    /* 0x6D */
    GET_ITEM(ITEM_BOTTLE, OBJECT_GI_BOTTLE, GID_BOTTLE, 0x6D, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x6E */
    GET_ITEM(ITEM_SEAHORSE, OBJECT_GI_BOTTLE_16, GID_SEAHORSE, 0x6E, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x6F */
    GET_ITEM(ITEM_CHATEAU, OBJECT_GI_BOTTLE_21, GID_CHATEAU, 0x6F, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x70 */
    GET_ITEM(ITEM_BOTTLE, OBJECT_GI_BOTTLE, GID_BOTTLE, 0x70, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x71 */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0x71, 0, 0),
    /* 0x72 */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0x72, 0, 0),
    /* 0x73 */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0x73, 0, 0),
    /* 0x74 */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0x74, 0, 0),
    /* 0x75 */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0x75, 0, 0),
    /* 0x76 */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0x76, 0, 0),
    /* 0x77 */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0x77, 0, 0),
    /* 0x78 */
    GET_ITEM(ITEM_MASK_DEKU, OBJECT_GI_NUTSMASK, GID_MASK_DEKU, 0x78, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x79 */
    GET_ITEM(ITEM_MASK_GORON, OBJECT_GI_GOLONMASK, GID_MASK_GORON, 0x79,
             GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x7A */
    GET_ITEM(ITEM_MASK_ZORA, OBJECT_GI_ZORAMASK, GID_MASK_ZORA, 0x7A, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x7B */
    GET_ITEM(ITEM_MASK_FIERCE_DEITY, OBJECT_GI_MASK03, GID_MASK_FIERCE_DEITY, 0x7B,
             GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x7C */
    GET_ITEM(ITEM_MASK_CAPTAIN, OBJECT_GI_MASK18, GID_MASK_CAPTAIN, 0x7C,
             GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x7D */
    GET_ITEM(ITEM_MASK_GIANT, OBJECT_GI_MASK23, GID_MASK_GIANT, 0x7D, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x7E */
    GET_ITEM(ITEM_MASK_ALL_NIGHT, OBJECT_GI_MASK06, GID_MASK_ALL_NIGHT, 0x7E,
             GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x7F */
    GET_ITEM(ITEM_MASK_BUNNY, OBJECT_GI_RABIT_MASK, GID_MASK_BUNNY, 0x7F,
             GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x80 */
    GET_ITEM(ITEM_MASK_KEATON, OBJECT_GI_KI_TAN_MASK, GID_MASK_KEATON, 0x80,
             GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x81 */
    GET_ITEM(ITEM_MASK_GARO, OBJECT_GI_MASK09, GID_MASK_GARO, 0x81, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x82 */
    GET_ITEM(ITEM_MASK_ROMANI, OBJECT_GI_MASK10, GID_MASK_ROMANI, 0x82, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x83 */
    GET_ITEM(ITEM_MASK_CIRCUS_LEADER, OBJECT_GI_MASK11, GID_MASK_CIRCUS_LEADER, 0x83,
             GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x84 */
    GET_ITEM(ITEM_MASK_POSTMAN, OBJECT_GI_MASK12, GID_MASK_POSTMAN, 0x84,
             GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x85 */
    GET_ITEM(ITEM_MASK_COUPLE, OBJECT_GI_MASK13, GID_MASK_COUPLE, 0x85, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x86 */
    GET_ITEM(ITEM_MASK_GREAT_FAIRY, OBJECT_GI_MASK14, GID_MASK_GREAT_FAIRY, 0x86,
             GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x87 */
    GET_ITEM(ITEM_MASK_GIBDO, OBJECT_GI_MASK15, GID_MASK_GIBDO, 0x87, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x88 */
    GET_ITEM(ITEM_MASK_DON_GERO, OBJECT_GI_MASK16, GID_MASK_DON_GERO, 0x88,
             GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x89 */
    GET_ITEM(ITEM_MASK_KAMARO, OBJECT_GI_MASK17, GID_MASK_KAMARO, 0x89, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x8A */
    GET_ITEM(ITEM_MASK_TRUTH, OBJECT_GI_TRUTH_MASK, GID_MASK_TRUTH, 0x8A,
             GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x8B */
    GET_ITEM(ITEM_MASK_STONE, OBJECT_GI_STONEMASK, GID_MASK_STONE, 0x8B,
             GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x8C */
    GET_ITEM(ITEM_MASK_BREMEN, OBJECT_GI_MASK20, GID_MASK_BREMEN, 0x8C, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x8D */
    GET_ITEM(ITEM_MASK_BLAST, OBJECT_GI_MASK21, GID_MASK_BLAST, 0x8D, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x8E */
    GET_ITEM(ITEM_MASK_SCENTS, OBJECT_GI_MASK22, GID_MASK_SCENTS, 0x8E, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x8F */
    GET_ITEM(ITEM_MASK_KAFEIS_MASK, OBJECT_GI_MASK05, GID_MASK_KAFEIS_MASK, 0x8F,
             GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x90 */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0x90, 0, 0),
    /* 0x91 */
    GET_ITEM(ITEM_CHATEAU_2, OBJECT_GI_BOTTLE_21, GID_CHATEAU, 0x91, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x92 */ GET_ITEM(ITEM_MILK, OBJECT_GI_MILK, GID_MILK, 0x92, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x93 */
    GET_ITEM(ITEM_GOLD_DUST_2, OBJECT_GI_GOLD_DUST, GID_GOLD_DUST, 0x93, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x94 */
    GET_ITEM(ITEM_HYLIAN_LOACH_2, OBJECT_GI_LOACH, GID_HYLIAN_LOACH, 0x94, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x95 */
    GET_ITEM(ITEM_SEAHORSE_CAUGHT, OBJECT_GI_SEAHORSE, GID_SEAHORSE_CAUGHT, 0x95, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x96 */
    GET_ITEM(ITEM_MOON_TEAR, OBJECT_GI_RESERVE00, GID_MOON_TEAR, 0x96, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x97 */
    GET_ITEM(ITEM_DEED_LAND, OBJECT_GI_RESERVE01, GID_DEED_LAND, 0x97, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x98 */
    GET_ITEM(ITEM_DEED_SWAMP, OBJECT_GI_RESERVE01, GID_DEED_SWAMP, 0x98, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x99 */
    GET_ITEM(ITEM_DEED_MOUNTAIN, OBJECT_GI_RESERVE01, GID_DEED_MOUNTAIN, 0x99, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x9A */
    GET_ITEM(ITEM_DEED_OCEAN, OBJECT_GI_RESERVE01, GID_DEED_OCEAN, 0x9A, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0x9B */
    GET_ITEM(ITEM_SWORD_GREAT_FAIRY, OBJECT_GI_SWORD_4, GID_SWORD_GREAT_FAIRY, 0x9B,
             GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x9C */
    GET_ITEM(ITEM_SWORD_KOKIRI, OBJECT_GI_SWORD_1, GID_SWORD_KOKIRI, 0x9C,
             GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x9D */
    GET_ITEM(ITEM_SWORD_RAZOR, OBJECT_GI_SWORD_2, GID_SWORD_RAZOR, 0x9D,
             GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x9E */
    GET_ITEM(ITEM_SWORD_GILDED, OBJECT_GI_SWORD_3, GID_SWORD_GILDED, 0x9E,
             GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0x9F */
    GET_ITEM(ITEM_SHIELD_HERO, OBJECT_GI_SHIELD_2, GID_SHIELD_HERO, 0x9F,
             GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_SHORT),
    /* 0xA0 */
    GET_ITEM(ITEM_ROOM_KEY, OBJECT_GI_RESERVE_B_00, GID_ROOM_KEY, 0xA0, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0xA1 */
    GET_ITEM(ITEM_LETTER_MAMA, OBJECT_GI_RESERVE_B_01, GID_LETTER_MAMA, 0xA1, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0xA2 */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0xA2, 0, 0),
    /* 0xA3 */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0xA3, 0, 0),
    /* 0xA4 */
    GET_ITEM(ITEM_NONE, OBJECT_GI_KI_TAN_MASK, GID_MASK_KEATON, 0xA4, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0xA5 */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0xA5, 0, 0),
    /* 0xA6 */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0xA6, 0, 0),
    /* 0xA7 */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0xA7, 0, 0),
    /* 0xA8 */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0xA8, 0, 0),
    /* 0xA9 */
    GET_ITEM(ITEM_BOTTLE, OBJECT_GI_BOTTLE, GID_BOTTLE, 0xA9, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0xAA */
    GET_ITEM(ITEM_LETTER_TO_KAFEI, OBJECT_GI_RESERVE_C_00, GID_LETTER_TO_KAFEI, 0xAA,
             GIFIELD(GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0xAB */
    GET_ITEM(ITEM_PENDANT_OF_MEMORIES, OBJECT_GI_RESERVE_C_01, GID_PENDANT_OF_MEMORIES, 0xAB,
             GIFIELD(GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0xAC */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0xAC, 0, 0),
    /* 0xAD */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0xAD, 0, 0),
    /* 0xAE */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0xAE, 0, 0),
    /* 0xAF */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0xAF, 0, 0),
    /* 0xB0 */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0xB0, 0, 0),
    /* 0xB1 */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0xB1, 0, 0),
    /* 0xB2 */ GET_ITEM(ITEM_NONE, OBJECT_UNSET_0, GID_NONE, 0xB2, 0, 0),
    /* 0xB3 */
    GET_ITEM(ITEM_NONE, OBJECT_GI_MSSA, GID_MASK_SUN, 0xB3, GIFIELD(GIFIELD_NO_COLLECTIBLE, 0), CHEST_ANIM_LONG),
    /* 0xB4 */
    GET_ITEM(ITEM_TINGLE_MAP, OBJECT_GI_FIELDMAP, GID_TINGLE_MAP, 0xB4, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0xB5 */
    GET_ITEM(ITEM_TINGLE_MAP, OBJECT_GI_FIELDMAP, GID_TINGLE_MAP, 0xB5, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0xB6 */
    GET_ITEM(ITEM_TINGLE_MAP, OBJECT_GI_FIELDMAP, GID_TINGLE_MAP, 0xB6, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0xB7 */
    GET_ITEM(ITEM_TINGLE_MAP, OBJECT_GI_FIELDMAP, GID_TINGLE_MAP, 0xB7, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0xB8 */
    GET_ITEM(ITEM_TINGLE_MAP, OBJECT_GI_FIELDMAP, GID_TINGLE_MAP, 0xB8, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
    /* 0xB9 */
    GET_ITEM(ITEM_TINGLE_MAP, OBJECT_GI_FIELDMAP, GID_TINGLE_MAP, 0xB9, GIFIELD(GIFIELD_20 | GIFIELD_NO_COLLECTIBLE, 0),
             CHEST_ANIM_LONG),
};

// Player_UpdateCurrentGetItemDrawId?
void func_8082ECE0(Player* this) {
    GetItemEntry* giEntry = &sGetItemTable[this->getItemId - 1];

    this->getItemDrawId = ABS_ALT(giEntry->gid);
}

LinkAnimationHeader* Player_GetStandingStillAnim(Player* this) {
    if ((this->transformation == PLAYER_FORM_ZORA) || (this->actor.id != ACTOR_PLAYER)) {
        return &gPlayerAnim_pz_wait;
    }
    if (this->transformation == PLAYER_FORM_GORON) {
        return &gPlayerAnim_pg_wait;
    }
    if (this->currentMask == PLAYER_MASK_SCENTS) {
        return &gPlayerAnim_cl_msbowait;
    }
    return D_8085BE84[PLAYER_ANIMGROUP_0][this->modelAnimType];
}

LinkAnimationHeader* sPlayerIdleAnimations[][2] = {
    { &gPlayerAnim_link_normal_wait_typeA_20f, &gPlayerAnim_link_normal_waitF_typeA_20f },
    { &gPlayerAnim_link_normal_wait_typeC_20f, &gPlayerAnim_link_normal_waitF_typeC_20f },
    { &gPlayerAnim_link_normal_wait_typeB_20f, &gPlayerAnim_link_normal_waitF_typeB_20f },
    { &gPlayerAnim_link_normal_wait_typeB_20f, &gPlayerAnim_link_normal_waitF_typeB_20f },
    { &gPlayerAnim_link_wait_typeD_20f, &gPlayerAnim_link_waitF_typeD_20f },
    { &gPlayerAnim_link_wait_typeD_20f, &gPlayerAnim_link_waitF_typeD_20f },
    { &gPlayerAnim_link_wait_typeD_20f, &gPlayerAnim_link_waitF_typeD_20f },
    { &gPlayerAnim_link_wait_heat1_20f, &gPlayerAnim_link_waitF_heat1_20f },
    { &gPlayerAnim_link_wait_heat2_20f, &gPlayerAnim_link_waitF_heat2_20f },
    { &gPlayerAnim_link_wait_itemD1_20f, &gPlayerAnim_link_wait_itemD1_20f },
    { &gPlayerAnim_link_wait_itemA_20f, &gPlayerAnim_link_waitF_itemA_20f },
    { &gPlayerAnim_link_wait_itemB_20f, &gPlayerAnim_link_waitF_itemB_20f },
    { &gPlayerAnim_link_wait_itemC_20f, &gPlayerAnim_link_wait_itemC_20f },
    { &gPlayerAnim_link_wait_itemD2_20f, &gPlayerAnim_link_wait_itemD2_20f },
    { &gPlayerAnim_cl_msbowait, &gPlayerAnim_cl_msbowait },
};

AnimSfxEntry D_8085C8C4[] = {
    ANIMSFX(ANIMSFX_TYPE_VOICE, 8, NA_SE_VO_LI_SNEEZE, STOP),
};
AnimSfxEntry D_8085C8C8[] = {
    ANIMSFX(ANIMSFX_TYPE_VOICE, 18, NA_SE_VO_LI_SWEAT, STOP),
};
AnimSfxEntry D_8085C8CC[] = {
    ANIMSFX(ANIMSFX_TYPE_VOICE, 13, NA_SE_VO_LI_BREATH_REST, STOP),
};
AnimSfxEntry D_8085C8D0[] = {
    ANIMSFX(ANIMSFX_TYPE_VOICE, 10, NA_SE_VO_LI_BREATH_REST, STOP),
};

AnimSfxEntry D_8085C8D4[] = {
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 44, NA_SE_PL_CALM_HIT, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 48, NA_SE_PL_CALM_HIT, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 52, NA_SE_PL_CALM_HIT, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 56, NA_SE_PL_CALM_HIT, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 60, NA_SE_PL_CALM_HIT, STOP),
};

AnimSfxEntry D_8085C8E8[] = {
    ANIMSFX(ANIMSFX_TYPE_8, 25, 0, CONTINUE), ANIMSFX(ANIMSFX_TYPE_8, 30, 0, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_8, 44, 0, CONTINUE), ANIMSFX(ANIMSFX_TYPE_8, 48, 0, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_8, 52, 0, CONTINUE), ANIMSFX(ANIMSFX_TYPE_8, 56, 0, STOP),
};

AnimSfxEntry D_8085C900[] = {
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 16, NA_SE_IT_SHIELD_SWING, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 20, NA_SE_IT_SHIELD_SWING, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 70, NA_SE_IT_SHIELD_SWING, STOP),
};

AnimSfxEntry D_8085C90C[] = {
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 10, NA_SE_IT_HAMMER_SWING, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_VOICE, 10, NA_SE_VO_LI_AUTO_JUMP, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 22, NA_SE_IT_SWORD_SWING, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_VOICE, 22, NA_SE_VO_LI_SWORD_N, STOP),
};

AnimSfxEntry D_8085C91C[] = {
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 39, NA_SE_IT_SWORD_SWING, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_VOICE, 39, NA_SE_VO_LI_SWORD_N, STOP),
};
AnimSfxEntry D_8085C924[] = {
    ANIMSFX(ANIMSFX_TYPE_VOICE, 20, NA_SE_VO_LI_RELAX, STOP),
};

AnimSfxEntry D_8085C928[] = {
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 4, NA_SE_VO_LI_POO_WAIT, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 12, NA_SE_VO_LI_POO_WAIT, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 30, NA_SE_VO_LI_POO_WAIT, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 61, NA_SE_VO_LI_POO_WAIT, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 68, NA_SE_VO_LI_POO_WAIT, STOP),
};

AnimSfxEntry* D_8085C93C[] = {
    D_8085C8C4, D_8085C8C8, D_8085C8CC, D_8085C8D0, D_8085C8D4, D_8085C8E8,
    D_8085C900, D_8085C90C, D_8085C91C, D_8085C924, D_8085C928, NULL,
};

// Used to index into D_8085C93C
u8 D_8085C96C[] = {
    0, 0, 1, 1, 2, 2, 2, 2, 10, 10, 10, 10, 10, 10, 3, 3, 4, 4, 8, 8, 5, 5, 6, 6, 7, 7, 9, 9, 11, 11,
};

s32 func_8082ED94(Player* this) {
    LinkAnimationHeader** entry;
    s32 i;

    if ((this->skelAnime.animation != &gPlayerAnim_link_normal_newroll_jump_end_20f) &&
        (this->skelAnime.animation != &gPlayerAnim_link_normal_newside_jump_end_20f) &&
        (((this->skelAnime.animation != Player_GetStandingStillAnim(this))) ||
         (this->skelAnime.animation == &gPlayerAnim_cl_msbowait))) {
        for (i = 0, entry = &sPlayerIdleAnimations[0][0]; i < ARRAY_COUNT_2D(sPlayerIdleAnimations); i++) {
            if (this->skelAnime.animation == *entry) {
                return i + 1;
            }
            entry++;
        }

        return 0;
    }

    return -1;
}

void func_8082EEA4(Player* this, s32 arg1) {
    u8 temp_v0 = D_8085C96C[arg1];

    if (temp_v0 != 0) {
        Player_PlayAnimSfx(this, D_8085C93C[temp_v0 - 1]);
    }
}

LinkAnimationHeader* func_8082EEE0(Player* this) {
    if (this->unk_B64 != 0) {
        return D_8085BE84[PLAYER_ANIMGROUP_3][this->modelAnimType];
    } else {
        return D_8085BE84[PLAYER_ANIMGROUP_2][this->modelAnimType];
    }
}

s32 Player_IsAimingReadyZoraFins(Player* this) {
    return Player_IsAimingZoraFins(this) && (this->unk_ACC != 0);
}

LinkAnimationHeader* Player_GetFightingRightAnim(Player* this) {
    if (Player_IsAimingReadyZoraFins(this)) {
        return &gPlayerAnim_link_boom_throw_waitR;
    } else {
        return D_8085BE84[PLAYER_ANIMGROUP_5][this->modelAnimType];
    }
}

LinkAnimationHeader* Player_GetFightingLeftAnim(Player* this) {
    if (Player_IsAimingReadyZoraFins(this)) {
        return &gPlayerAnim_link_boom_throw_waitL;
    } else {
        return D_8085BE84[PLAYER_ANIMGROUP_4][this->modelAnimType];
    }
}

LinkAnimationHeader* func_8082EFE4(Player* this) {
    if (Actor_PlayerIsAimingReadyFpsItem(this)) {
        return &gPlayerAnim_link_bow_side_walk;
    } else {
        return D_8085BE84[PLAYER_ANIMGROUP_22][this->modelAnimType];
    }
}

void Player_LerpZoraBarrierLighting(PlayState* play, ZoraBarrierLighting* lighting, f32 lerp) {
    Environment_LerpAmbientColor(play, &lighting->ambientColor, lerp);
    Environment_LerpDiffuseColor(play, &lighting->diffuseColor, lerp);
    Environment_LerpFogColor(play, &lighting->fogColor, lerp);
    Environment_LerpFog(play, lighting->fogNear, lighting->fogFar, lerp);
}

/**
 * Revert cylinder to normal properties
 */
void Player_ResetCylinder(Player* this) {
    this->cylinder.base.colType = COLTYPE_HIT5;
    this->cylinder.base.atFlags = AT_NONE;
    this->cylinder.base.acFlags = AC_ON | AC_TYPE_ENEMY;
    this->cylinder.base.ocFlags1 = OC1_ON | OC1_TYPE_ALL;
    this->cylinder.info.elemType = ELEMTYPE_UNK1;
    this->cylinder.info.toucher.dmgFlags = 0;
    this->cylinder.info.bumper.dmgFlags = 0xF7CFFFFF;
    this->cylinder.info.toucherFlags = TOUCH_NONE | TOUCH_SFX_NORMAL;
    this->cylinder.dim.radius = 12;
}

/**
 * Give cylinder special properties for attacks, uses include
 * - Normal roll
 * - Deku spin
 * - Deku launch
 * - Goron pound
 * - Goron spike roll
 * - Zora barrier
 *
 * and possibly more.
 *
 * @param dmgFlags Damage flags (DMGFLAG defines)
 * @param damage to do
 * @param radius of cylinder
 */
void Player_SetCylinderForAttack(Player* this, u32 dmgFlags, s32 damage, s32 radius) {
    this->cylinder.base.atFlags = AT_ON | AT_TYPE_PLAYER;
    if (radius > 30) {
        this->cylinder.base.ocFlags1 = OC1_NONE;
    } else {
        this->cylinder.base.ocFlags1 = OC1_ON | OC1_TYPE_ALL;
    }

    this->cylinder.info.elemType = ELEMTYPE_UNK2;
    this->cylinder.info.toucherFlags = TOUCH_ON | TOUCH_NEAREST | TOUCH_SFX_NORMAL;
    this->cylinder.dim.radius = radius;
    this->cylinder.info.toucher.dmgFlags = dmgFlags;
    this->cylinder.info.toucher.damage = damage;

    if (dmgFlags & DMG_GORON_POUND) {
        this->cylinder.base.acFlags = AC_NONE;
    } else {
        this->cylinder.base.colType = COLTYPE_NONE;
        this->cylinder.info.bumper.dmgFlags = 0xF7CFFFFF;

        if (dmgFlags & DMG_ZORA_BARRIER) {
            this->cylinder.base.acFlags = AC_NONE;
        } else {
            this->cylinder.base.acFlags = AC_ON | AC_TYPE_ENEMY;
        }
    }
}

// Check for starting Zora barrier
void Player_SetupZoraBarrier(Player* this, u16 button) {
    if ((this->transformation == PLAYER_FORM_ZORA) && CHECK_BTN_ALL(sPlayerControlInput->cur.button, button)) {
        this->stateFlags1 |= PLAYER_STATE1_ZORA_BARRIER;
    }
}

ZoraBarrierLighting sZoraBarrierLighting = {
    { 0, 0, 0 },       // ambientColor
    { 255, 255, 155 }, // diffuseColor
    { 20, 20, 50 },    // fogColor
    940,               // fogNear
    5000,              // fogFar
};

// Run Zora Barrier
void Player_UpdateZoraBarrier(PlayState* play, Player* this) {
    s32 sp4C = this->unk_B62;
    f32 temp;
    s16 sp46;
    s16 sp44;
    f32 sp40;
    f32 sp3C;
    s32 var_v0;

    if ((gSaveContext.save.playerData.magic != 0) && (this->stateFlags1 & PLAYER_STATE1_ZORA_BARRIER)) {
        if (gSaveContext.magicState == 0) {
            Magic_Consume(play, 0, MAGIC_CONSUME_GORON_ZORA);
        }

        temp = 16.0f;
        if (gSaveContext.save.playerData.magic >= 16) {
            var_v0 = 255;
        } else {
            var_v0 = (gSaveContext.save.playerData.magic / temp) * 255.0f;
        }
        Math_StepToS(&this->unk_B62, var_v0, 50);
    } else if ((Math_StepToS(&this->unk_B62, 0, 50) != 0) && (gSaveContext.magicState != 0)) {
        Magic_Reset(play);
    }

    if ((this->unk_B62 != 0) || (sp4C != 0)) {
        f32 sp34;
        f32 new_var;

        sp46 = play->gameplayFrames * 7000;
        sp44 = play->gameplayFrames * 14000;
        Player_LerpZoraBarrierLighting(play, &sZoraBarrierLighting, this->unk_B62 / 255.0f);

        sp34 = Math_SinS(sp44) * 40.0f;
        sp40 = Math_CosS(sp44) * 40.0f;
        sp3C = Math_SinS(sp46) * sp34;
        new_var = Math_CosS(sp46) * sp34;

        Lights_PointNoGlowSetInfo(&this->lightInfo, this->actor.world.pos.x + sp40, this->actor.world.pos.y + sp3C,
                                  this->actor.world.pos.z + new_var, 100, 200, 255, 600);

        Player_PlaySfx(this, NA_SE_PL_ZORA_SPARK_BARRIER - SFX_FLAG);
        func_800B648C(play, 1, 2, 100.0f, &this->actor.world.pos);
    }
}

void Player_SetUpperActionFunc(PlayState* play, Player* this, PlayerFuncAC4 arg2) {
    this->upperActionFunc = arg2;
    this->unk_ACE = 0;
    this->unk_AC8 = 0.0f;
    Player_StopInterruptableSfx(this);
}

#define GET_PLAYER_ANIM(group, type) ((LinkAnimationHeader**)D_8085BE84)[group * PLAYER_ANIMTYPE_MAX + type]

void func_8082F470(PlayState* play, Player* this, PlayerActionParam actionParam) {
    LinkAnimationHeader* current = this->skelAnime.animation;
    LinkAnimationHeader*(*iter)[PLAYER_ANIMTYPE_MAX] = (void*)&D_8085BE84[0][this->modelAnimType];
    s32 i;

    this->stateFlags1 &= ~(PLAYER_STATE1_8 | PLAYER_STATE1_AIMING_ZORAFINS);

    for (i = 0; i < PLAYER_ANIMGROUP_MAX; i++) {
        if (current == **iter) {
            break;
        }
        iter++;
    }

    Player_ChangeItem(play, this, actionParam);

    if (i < PLAYER_ANIMGROUP_MAX) {
        this->skelAnime.animation = GET_PLAYER_ANIM(i, this->modelAnimType);
    }
}

s8 sItemActionParams[] = {
    PLAYER_AP_OCARINA,                 // ITEM_OCARINA,
    PLAYER_AP_BOW,                     // ITEM_BOW,
    PLAYER_AP_BOW_FIRE,                // ITEM_ARROW_FIRE,
    PLAYER_AP_BOW_ICE,                 // ITEM_ARROW_ICE,
    PLAYER_AP_BOW_LIGHT,               // ITEM_ARROW_LIGHT,
    PLAYER_AP_PICTO_BOX,               // ITEM_OCARINA_FAIRY,
    PLAYER_AP_BOMB,                    // ITEM_BOMB,
    PLAYER_AP_BOMBCHU,                 // ITEM_BOMBCHU,
    PLAYER_AP_STICK,                   // ITEM_STICK,
    PLAYER_AP_NUT,                     // ITEM_NUT,
    PLAYER_AP_MAGIC_BEANS,             // ITEM_MAGIC_BEANS,
    PLAYER_AP_PICTO_BOX,               // ITEM_SLINGSHOT,
    PLAYER_AP_POWDER_KEG,              // ITEM_POWDER_KEG,
    PLAYER_AP_PICTO_BOX,               // ITEM_PICTO_BOX,
    PLAYER_AP_LENS,                    // ITEM_LENS,
    PLAYER_AP_HOOKSHOT,                // ITEM_HOOKSHOT,
    PLAYER_AP_SWORD_GREAT_FAIRY,       // ITEM_SWORD_GREAT_FAIRY,
    PLAYER_AP_PICTO_BOX,               // ITEM_LONGSHOT, // OoT Leftover
    PLAYER_AP_BOTTLE,                  // ITEM_BOTTLE,
    PLAYER_AP_BOTTLE_POTION_RED,       // ITEM_POTION_RED,
    PLAYER_AP_BOTTLE_POTION_GREEN,     // ITEM_POTION_GREEN,
    PLAYER_AP_BOTTLE_POTION_BLUE,      // ITEM_POTION_BLUE,
    PLAYER_AP_BOTTLE_FAIRY,            // ITEM_FAIRY,
    PLAYER_AP_BOTTLE_DEKU_PRINCESS,    // ITEM_DEKU_PRINCESS,
    PLAYER_AP_BOTTLE_MILK,             // ITEM_MILK_BOTTLE,
    PLAYER_AP_BOTTLE_MILK_HALF,        // ITEM_MILK_HALF,
    PLAYER_AP_BOTTLE_FISH,             // ITEM_FISH,
    PLAYER_AP_BOTTLE_BUG,              // ITEM_BUG,
    PLAYER_AP_BOTTLE_BUG,              // ITEM_BLUE_FIRE, // !
    PLAYER_AP_BOTTLE_POE,              // ITEM_POE,
    PLAYER_AP_BOTTLE_BIG_POE,          // ITEM_BIG_POE,
    PLAYER_AP_BOTTLE_SPRING_WATER,     // ITEM_SPRING_WATER,
    PLAYER_AP_BOTTLE_HOT_SPRING_WATER, // ITEM_HOT_SPRING_WATER,
    PLAYER_AP_BOTTLE_ZORA_EGG,         // ITEM_ZORA_EGG,
    PLAYER_AP_BOTTLE_GOLD_DUST,        // ITEM_GOLD_DUST,
    PLAYER_AP_BOTTLE_MUSHROOM,         // ITEM_MUSHROOM,
    PLAYER_AP_BOTTLE_SEAHORSE,         // ITEM_SEA_HORSE,
    PLAYER_AP_BOTTLE_CHATEAU,          // ITEM_CHATEAU,
    PLAYER_AP_BOTTLE_HYLIAN_LOACH,     // ITEM_HYLIAN_LOACH,
    PLAYER_AP_BOTTLE_POE,              // ITEM_OBABA_DRINK, // !
    PLAYER_AP_MOON_TEAR,               // ITEM_MOON_TEAR,
    PLAYER_AP_DEED_LAND,               // ITEM_DEED_LAND,
    PLAYER_AP_DEED_SWAMP,              // ITEM_DEED_SWAMP,
    PLAYER_AP_DEED_MOUNTAIN,           // ITEM_DEED_MOUNTAIN,
    PLAYER_AP_DEED_OCEAN,              // ITEM_DEED_OCEAN,
    PLAYER_AP_ROOM_KEY,                // ITEM_ROOM_KEY,
    PLAYER_AP_LETTER_MAMA,             // ITEM_LETTER_MAMA,
    PLAYER_AP_LETTER_TO_KAFEI,         // ITEM_LETTER_TO_KAFEI,
    PLAYER_AP_PENDANT_OF_MEMORIES,     // ITEM_PENDANT_MEMORIES,
    PLAYER_AP_38,                      // ITEM_TINGLE_MAP, // !
    PLAYER_AP_MASK_DEKU,               // ITEM_MASK_DEKU,
    PLAYER_AP_MASK_GORON,              // ITEM_MASK_GORON,
    PLAYER_AP_MASK_ZORA,               // ITEM_MASK_ZORA,
    PLAYER_AP_MASK_FIERCE_DEITY,       // ITEM_MASK_FIERCE_DEITY,
    PLAYER_AP_MASK_TRUTH,              // ITEM_MASK_TRUTH,
    PLAYER_AP_MASK_KAFEIS_MASK,        // ITEM_MASK_KAFEIS_MASK,
    PLAYER_AP_MASK_ALL_NIGHT,          // ITEM_MASK_ALL_NIGHT,
    PLAYER_AP_MASK_BUNNY,              // ITEM_MASK_BUNNY,
    PLAYER_AP_MASK_KEATON,             // ITEM_MASK_KEATON,
    PLAYER_AP_MASK_GARO,               // ITEM_MASK_GARO,
    PLAYER_AP_MASK_ROMANI,             // ITEM_MASK_ROMANI,
    PLAYER_AP_MASK_CIRCUS_LEADER,      // ITEM_MASK_CIRCUS_LEADER,
    PLAYER_AP_MASK_POSTMAN,            // ITEM_MASK_POSTMAN,
    PLAYER_AP_MASK_COUPLE,             // ITEM_MASK_COUPLE,
    PLAYER_AP_MASK_GREAT_FAIRY,        // ITEM_MASK_GREAT_FAIRY,
    PLAYER_AP_MASK_GIBDO,              // ITEM_MASK_GIBDO,
    PLAYER_AP_MASK_DON_GERO,           // ITEM_MASK_DON_GERO,
    PLAYER_AP_MASK_KAMARO,             // ITEM_MASK_KAMARO,
    PLAYER_AP_MASK_CAPTAIN,            // ITEM_MASK_CAPTAIN,
    PLAYER_AP_MASK_STONE,              // ITEM_MASK_STONE,
    PLAYER_AP_MASK_BREMEN,             // ITEM_MASK_BREMEN,
    PLAYER_AP_MASK_BLAST,              // ITEM_MASK_BLAST,
    PLAYER_AP_MASK_SCENTS,             // ITEM_MASK_SCENTS,
    PLAYER_AP_MASK_GIANT,              // ITEM_MASK_GIANT,
    PLAYER_AP_BOW_FIRE,                // ITEM_BOW_ARROW_FIRE,
    PLAYER_AP_BOW_ICE,                 // ITEM_BOW_ARROW_ICE,
    PLAYER_AP_BOW_LIGHT,               // ITEM_BOW_ARROW_LIGHT,
    PLAYER_AP_SWORD_KOKIRI,            // ITEM_SWORD_KOKIRI,
    PLAYER_AP_SWORD_RAZOR,             // ITEM_SWORD_RAZOR,
    PLAYER_AP_SWORD_GILDED,            // ITEM_SWORD_GILDED,
    PLAYER_AP_SWORD_GREAT_FAIRY,       // ITEM_SWORD_DEITY,
};

PlayerActionParam Player_ItemToActionParam(Player* this, ItemId item) {
    if (item >= ITEM_FD) {
        return PLAYER_AP_NONE;
    } else if (item == ITEM_FC) {
        return PLAYER_AP_LAST_USED;
    } else if (item == ITEM_FISHING_ROD) {
        return PLAYER_AP_FISHING_ROD;
    } else if ((item == ITEM_SWORD_KOKIRI) && (this->transformation == PLAYER_FORM_ZORA)) {
        return PLAYER_AP_ZORA_FINS;
    } else {
        return sItemActionParams[item];
    }
}

void func_8082F594(PlayState* play, Player* this) {
}

void func_8082F5A4(PlayState* play, Player* this) {
    this->stickFlameTimer = 0;
    this->unk_B08[1] = 1.0f;
}

void func_8082F5C0(PlayState* play, Player* this) {
    this->stateFlags1 |= PLAYER_STATE1_8;

    if (this->itemActionParam == PLAYER_AP_NUT) {
        this->stickFlameTimer = -2;
    } else {
        this->stickFlameTimer = -1;
    }
    this->unk_ACC = 0;
}

PlayerFuncAC4 D_8085C9F0[PLAYER_AP_MAX] = {
    Player_SetupStartZTargetDefend, // PLAYER_AP_NONE
    Player_SetupStartZTargetDefend, // PLAYER_AP_LAST_USED
    Player_SetupStartZTargetDefend, // PLAYER_AP_FISHING_ROD
    func_808487B8,                  // PLAYER_AP_SWORD_KOKIRI
    func_808487B8,                  // PLAYER_AP_SWORD_RAZOR
    func_808487B8,                  // PLAYER_AP_SWORD_GILDED
    func_808487B8,                  // PLAYER_AP_SWORD_GREAT_FAIRY
    Player_SetupStartZTargetDefend, // PLAYER_AP_STICK
    Player_SetupStartZTargetDefend, // PLAYER_AP_ZORA_FINS
    func_80848B6C,                  // PLAYER_AP_BOW
    func_80848B6C,                  // PLAYER_AP_BOW_FIRE
    func_80848B6C,                  // PLAYER_AP_BOW_ICE
    func_80848B6C,                  // PLAYER_AP_BOW_LIGHT
    func_80848B6C,                  // PLAYER_AP_HOOKSHOT
    Player_HoldActor,               // PLAYER_AP_BOMB
    Player_HoldActor,               // PLAYER_AP_POWDER_KEG
    Player_HoldActor,               // PLAYER_AP_BOMBCHU
    func_808491B4,                  // PLAYER_AP_11
    func_80848B6C,                  // PLAYER_AP_NUT
    Player_SetupStartZTargetDefend, // PLAYER_AP_PICTO_BOX
    Player_SetupStartZTargetDefend, // PLAYER_AP_OCARINA
    Player_SetupStartZTargetDefend, // PLAYER_AP_BOTTLE
    Player_SetupStartZTargetDefend, // PLAYER_AP_BOTTLE_FISH
    Player_SetupStartZTargetDefend, // PLAYER_AP_BOTTLE_SPRING_WATER
    Player_SetupStartZTargetDefend, // PLAYER_AP_BOTTLE_HOT_SPRING_WATER
    Player_SetupStartZTargetDefend, // PLAYER_AP_BOTTLE_ZORA_EGG
    Player_SetupStartZTargetDefend, // PLAYER_AP_BOTTLE_DEKU_PRINCESS
    Player_SetupStartZTargetDefend, // PLAYER_AP_BOTTLE_GOLD_DUST
    Player_SetupStartZTargetDefend, // PLAYER_AP_BOTTLE_1C
    Player_SetupStartZTargetDefend, // PLAYER_AP_BOTTLE_SEA_HORSE
    Player_SetupStartZTargetDefend, // PLAYER_AP_BOTTLE_MUSHROOM
    Player_SetupStartZTargetDefend, // PLAYER_AP_BOTTLE_HYLIAN_LOACH
    Player_SetupStartZTargetDefend, // PLAYER_AP_BOTTLE_BUG
    Player_SetupStartZTargetDefend, // PLAYER_AP_BOTTLE_POE
    Player_SetupStartZTargetDefend, // PLAYER_AP_BOTTLE_BIG_POE
    Player_SetupStartZTargetDefend, // PLAYER_AP_BOTTLE_POTION_RED
    Player_SetupStartZTargetDefend, // PLAYER_AP_BOTTLE_POTION_BLUE
    Player_SetupStartZTargetDefend, // PLAYER_AP_BOTTLE_POTION_GREEN
    Player_SetupStartZTargetDefend, // PLAYER_AP_BOTTLE_MILK
    Player_SetupStartZTargetDefend, // PLAYER_AP_BOTTLE_MILK_HALF
    Player_SetupStartZTargetDefend, // PLAYER_AP_BOTTLE_CHATEAU
    Player_SetupStartZTargetDefend, // PLAYER_AP_BOTTLE_FAIRY
    Player_SetupStartZTargetDefend, // PLAYER_AP_MOON_TEAR
    Player_SetupStartZTargetDefend, // PLAYER_AP_DEED_LAND
    Player_SetupStartZTargetDefend, // PLAYER_AP_ROOM_KEY
    Player_SetupStartZTargetDefend, // PLAYER_AP_LETTER_TO_KAFEI
    Player_SetupStartZTargetDefend, // PLAYER_AP_MAGIC_BEANS
    Player_SetupStartZTargetDefend, // PLAYER_AP_DEED_SWAMP
    Player_SetupStartZTargetDefend, // PLAYER_AP_DEED_MOUNTAIN
    Player_SetupStartZTargetDefend, // PLAYER_AP_DEED_OCEAN
    Player_SetupStartZTargetDefend, // PLAYER_AP_32
    Player_SetupStartZTargetDefend, // PLAYER_AP_LETTER_MAMA
    Player_SetupStartZTargetDefend, // PLAYER_AP_34
    Player_SetupStartZTargetDefend, // PLAYER_AP_35
    Player_SetupStartZTargetDefend, // PLAYER_AP_PENDANT_MEMORIES
    Player_SetupStartZTargetDefend, // PLAYER_AP_37
    Player_SetupStartZTargetDefend, // PLAYER_AP_38
    Player_SetupStartZTargetDefend, // PLAYER_AP_39
    Player_SetupStartZTargetDefend, // PLAYER_AP_MASK_TRUTH
    Player_SetupStartZTargetDefend, // PLAYER_AP_MASK_KAFEIS_MASK
    Player_SetupStartZTargetDefend, // PLAYER_AP_MASK_ALL_NIGHT
    Player_SetupStartZTargetDefend, // PLAYER_AP_MASK_BUNNY
    Player_SetupStartZTargetDefend, // PLAYER_AP_MASK_KEATON
    Player_SetupStartZTargetDefend, // PLAYER_AP_MASK_GARO
    Player_SetupStartZTargetDefend, // PLAYER_AP_MASK_ROMANI
    Player_SetupStartZTargetDefend, // PLAYER_AP_MASK_CIRCUS_LEADER
    Player_SetupStartZTargetDefend, // PLAYER_AP_MASK_POSTMAN
    Player_SetupStartZTargetDefend, // PLAYER_AP_MASK_COUPLE
    Player_SetupStartZTargetDefend, // PLAYER_AP_MASK_GREAT_FAIRY
    Player_SetupStartZTargetDefend, // PLAYER_AP_MASK_GIBDO
    Player_SetupStartZTargetDefend, // PLAYER_AP_MASK_DON_GERO
    Player_SetupStartZTargetDefend, // PLAYER_AP_MASK_KAMARO
    Player_SetupStartZTargetDefend, // PLAYER_AP_MASK_CAPTAIN
    Player_SetupStartZTargetDefend, // PLAYER_AP_MASK_STONE
    Player_SetupStartZTargetDefend, // PLAYER_AP_MASK_BREMEN
    Player_SetupStartZTargetDefend, // PLAYER_AP_MASK_BLAST
    Player_SetupStartZTargetDefend, // PLAYER_AP_MASK_SCENTS
    Player_SetupStartZTargetDefend, // PLAYER_AP_MASK_GIANT
    Player_SetupStartZTargetDefend, // PLAYER_AP_MASK_FIERCE_DEITY
    Player_SetupStartZTargetDefend, // PLAYER_AP_MASK_GORON
    Player_SetupStartZTargetDefend, // PLAYER_AP_MASK_ZORA
    Player_SetupStartZTargetDefend, // PLAYER_AP_MASK_DEKU
    Player_SetupStartZTargetDefend, // PLAYER_AP_LENS
};

// sPlayerActionParamInits?
void (*sItemChangeFuncs[PLAYER_AP_MAX])(PlayState*, Player*) = {
    func_8082F594,         // PLAYER_AP_NONE
    func_8082F594,         // PLAYER_AP_LAST_USED
    func_8082F594,         // PLAYER_AP_FISHING_ROD
    func_8082F594,         // PLAYER_AP_SWORD_KOKIRI
    func_8082F594,         // PLAYER_AP_SWORD_RAZOR
    func_8082F594,         // PLAYER_AP_SWORD_GILDED
    func_8082F594,         // PLAYER_AP_SWORD_GREAT_FAIRY
    func_8082F5A4,         // PLAYER_AP_STICK
    Player_SetupZoraFins,  // PLAYER_AP_ZORA_FINS
    func_8082F5C0,         // PLAYER_AP_BOW
    func_8082F5C0,         // PLAYER_AP_BOW_FIRE
    func_8082F5C0,         // PLAYER_AP_BOW_ICE
    func_8082F5C0,         // PLAYER_AP_BOW_LIGHT
    func_8082F7F4,         // PLAYER_AP_HOOKSHOT
    Player_SpawnExplosive, // PLAYER_AP_BOMB
    Player_SpawnExplosive, // PLAYER_AP_POWDER_KEG
    Player_SpawnExplosive, // PLAYER_AP_BOMBCHU
    Player_SetupZoraFins,  // PLAYER_AP_11
    func_8082F5C0,         // PLAYER_AP_NUT
    func_8082F594,         // PLAYER_AP_PICTO_BOX
    func_8082F594,         // PLAYER_AP_OCARINA
    func_8082F594,         // PLAYER_AP_BOTTLE
    func_8082F594,         // PLAYER_AP_BOTTLE_FISH
    func_8082F594,         // PLAYER_AP_BOTTLE_SPRING_WATER
    func_8082F594,         // PLAYER_AP_BOTTLE_HOT_SPRING_WATER
    func_8082F594,         // PLAYER_AP_BOTTLE_ZORA_EGG
    func_8082F594,         // PLAYER_AP_BOTTLE_DEKU_PRINCESS
    func_8082F594,         // PLAYER_AP_BOTTLE_GOLD_DUST
    func_8082F594,         // PLAYER_AP_BOTTLE_1C
    func_8082F594,         // PLAYER_AP_BOTTLE_SEA_HORSE
    func_8082F594,         // PLAYER_AP_BOTTLE_MUSHROOM
    func_8082F594,         // PLAYER_AP_BOTTLE_HYLIAN_LOACH
    func_8082F594,         // PLAYER_AP_BOTTLE_BUG
    func_8082F594,         // PLAYER_AP_BOTTLE_POE
    func_8082F594,         // PLAYER_AP_BOTTLE_BIG_POE
    func_8082F594,         // PLAYER_AP_BOTTLE_POTION_RED
    func_8082F594,         // PLAYER_AP_BOTTLE_POTION_BLUE
    func_8082F594,         // PLAYER_AP_BOTTLE_POTION_GREEN
    func_8082F594,         // PLAYER_AP_BOTTLE_MILK
    func_8082F594,         // PLAYER_AP_BOTTLE_MILK_HALF
    func_8082F594,         // PLAYER_AP_BOTTLE_CHATEAU
    func_8082F594,         // PLAYER_AP_BOTTLE_FAIRY
    func_8082F594,         // PLAYER_AP_MOON_TEAR
    func_8082F594,         // PLAYER_AP_DEED_LAND
    func_8082F594,         // PLAYER_AP_ROOM_KEY
    func_8082F594,         // PLAYER_AP_LETTER_TO_KAFEI
    func_8082F594,         // PLAYER_AP_MAGIC_BEANS
    func_8082F594,         // PLAYER_AP_DEED_SWAMP
    func_8082F594,         // PLAYER_AP_DEED_MOUNTAIN
    func_8082F594,         // PLAYER_AP_DEED_OCEAN
    func_8082F594,         // PLAYER_AP_32
    func_8082F594,         // PLAYER_AP_LETTER_MAMA
    func_8082F594,         // PLAYER_AP_34
    func_8082F594,         // PLAYER_AP_35
    func_8082F594,         // PLAYER_AP_PENDANT_MEMORIES
    func_8082F594,         // PLAYER_AP_37
    func_8082F594,         // PLAYER_AP_38
    func_8082F594,         // PLAYER_AP_39
    func_8082F594,         // PLAYER_AP_MASK_TRUTH
    func_8082F594,         // PLAYER_AP_MASK_KAFEIS_MASK
    func_8082F594,         // PLAYER_AP_MASK_ALL_NIGHT
    func_8082F594,         // PLAYER_AP_MASK_BUNNY
    func_8082F594,         // PLAYER_AP_MASK_KEATON
    func_8082F594,         // PLAYER_AP_MASK_GARO
    func_8082F594,         // PLAYER_AP_MASK_ROMANI
    func_8082F594,         // PLAYER_AP_MASK_CIRCUS_LEADER
    func_8082F594,         // PLAYER_AP_MASK_POSTMAN
    func_8082F594,         // PLAYER_AP_MASK_COUPLE
    func_8082F594,         // PLAYER_AP_MASK_GREAT_FAIRY
    func_8082F594,         // PLAYER_AP_MASK_GIBDO
    func_8082F594,         // PLAYER_AP_MASK_DON_GERO
    func_8082F594,         // PLAYER_AP_MASK_KAMARO
    func_8082F594,         // PLAYER_AP_MASK_CAPTAIN
    func_8082F594,         // PLAYER_AP_MASK_STONE
    func_8082F594,         // PLAYER_AP_MASK_BREMEN
    func_8082F594,         // PLAYER_AP_MASK_BLAST
    func_8082F594,         // PLAYER_AP_MASK_SCENTS
    func_8082F594,         // PLAYER_AP_MASK_GIANT
    func_8082F594,         // PLAYER_AP_MASK_FIERCE_DEITY
    func_8082F594,         // PLAYER_AP_MASK_GORON
    func_8082F594,         // PLAYER_AP_MASK_ZORA
    func_8082F594,         // PLAYER_AP_MASK_DEKU
    func_8082F594,         // PLAYER_AP_LENS
};

void func_8082F5FC(Player* this, Actor* actor) {
    this->heldActor = actor;
    this->interactRangeActor = actor;
    this->getItemId = GI_NONE;
    this->leftHandWorld.rot.y = actor->shape.rot.y - this->actor.shape.rot.y;
    this->stateFlags1 |= PLAYER_STATE1_HOLDING_ACTOR;
}

ItemChangeAnimInfo sItemChangeAnimsInfo[] = {
    { &gPlayerAnim_link_normal_free2free, 12 },
    { &gPlayerAnim_link_normal_normal2fighter, 6 },
    { &gPlayerAnim_link_hammer_normal2long, 8 },
    { &gPlayerAnim_link_normal_normal2free, 8 },
    { &gPlayerAnim_link_fighter_fighter2long, 8 },
    { &gPlayerAnim_link_normal_fighter2free, 10 },
    { &gPlayerAnim_link_hammer_long2free, 7 },
    { &gPlayerAnim_link_hammer_long2long, 11 },
    { &gPlayerAnim_link_normal_free2free, 12 },
    { &gPlayerAnim_link_normal_normal2bom, 4 },
    { &gPlayerAnim_link_normal_long2bom, 4 },
    { &gPlayerAnim_link_normal_free2bom, 4 },
    { &gPlayerAnim_link_anchor_anchor2fighter, 5 },
    { &gPlayerAnim_link_normal_free2freeB, 13 },
    { &gPlayerAnim_pz_bladeon, 4 },
};

s8 D_8085CD00[PLAYER_ANIMTYPE_MAX][PLAYER_ANIMTYPE_MAX] = {
    { 8, -5, -3, -6, 8, 0xB }, { 5, 0, -1, 4, 5, 9 },     { 3, 1, 0, 2, 3, 9 },
    { 6, -4, -2, 7, 6, 0xA },  { 8, -5, -3, -6, 8, 0xB }, { 8, -5, -3, -6, 8, 0xB },
};

ExplosiveInfo sPlayerExplosiveInfo[] = {
    { ITEM_BOMB, ACTOR_EN_BOM },        // PLAYER_AP_BOMB
    { ITEM_POWDER_KEG, ACTOR_EN_BOM },  // PLAYER_AP_POWDER_KEG
    { ITEM_BOMBCHU, ACTOR_EN_BOM_CHU }, // PLAYER_AP_BOMBCHU
};

void Player_SpawnExplosive(PlayState* play, Player* this) {
    s32 explosiveType;
    ExplosiveInfo* explosiveInfo;
    Actor* explosiveActor;

    if (this->stateFlags1 & PLAYER_STATE1_HOLDING_ACTOR) {
        Player_UnequipItem(play, this);
        return;
    }

    explosiveType = Player_GetExplosiveHeld(this);
    explosiveInfo = &sPlayerExplosiveInfo[explosiveType];
    if ((explosiveType == 1) && (gSaveContext.powderKegTimer == 0)) {
        gSaveContext.powderKegTimer = 200;
    }

    explosiveActor = Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, explosiveInfo->actorId,
                                        this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z,
                                        explosiveType == 1 ? BOMB_CAT_POWDER_KEG : BOMB_CAT_BOMB,
                                        this->actor.shape.rot.y, 0, BOMB_BODY);
    if (explosiveActor != NULL) {
        if ((explosiveType == 0) && (play->unk_1887E != 0)) {
            play->unk_1887E--;
            if (play->unk_1887E == 0) {
                play->unk_1887E = -1;
            }
        } else if ((explosiveType == 2) && (play->unk_1887D != 0)) {
            play->unk_1887D--;
            if (play->unk_1887D == 0) {
                play->unk_1887D = -1;
            }
        } else {
            Inventory_ChangeAmmo(explosiveInfo->itemId, -1);
        }
        func_8082F5FC(this, explosiveActor);
    } else if (explosiveType == 1) {
        gSaveContext.powderKegTimer = 0;
    }
}

void func_8082F7F4(PlayState* play, Player* this) {
    ArmsHook* armsHook;

    this->stateFlags1 |= PLAYER_STATE1_8;
    this->stickFlameTimer = -3;
    this->unk_B48 = 0.0f;

    this->heldActor =
        Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACTOR_ARMS_HOOK, this->actor.world.pos.x,
                           this->actor.world.pos.y, this->actor.world.pos.z, 0, this->actor.shape.rot.y, 0, 0);

    if (this->heldActor == NULL) {
        Player_UseItem(play, this, ITEM_NONE);
        return;
    }
    armsHook = (ArmsHook*)this->heldActor;
    armsHook->actor.objBankIndex = this->actor.objBankIndex;
    armsHook->unk_208 = this->transformation;
}

void Player_SetupZoraFins(PlayState* play, Player* this) {
    this->stateFlags1 |= PLAYER_STATE1_AIMING_ZORAFINS;
}

void Player_ChangeItem(PlayState* play, Player* this, PlayerActionParam actionParam) {
    this->heldItemActionParam = this->itemActionParam = actionParam;
    this->modelGroup = this->nextModelGroup;

    this->stateFlags1 &= ~(PLAYER_STATE1_AIMING_ZORAFINS | PLAYER_STATE1_8);

    this->unk_B08[0] = 0.0f;
    this->unk_B08[1] = 0.0f;
    this->stickFlameTimer = 0;

    sItemChangeFuncs[actionParam](play, this);

    Player_SetModelGroup(this, this->modelGroup);
}

// AttackAnimInfo sMeleeAttackAnimInfo
AttackAnimInfo sMeleeAttackAnimInfo[PLAYER_MWA_MAX] = {
    // PLAYER_MWA_FORWARD_SLASH_1H
    { &gPlayerAnim_link_fighter_normal_kiru, &gPlayerAnim_link_fighter_normal_kiru_end,
      &gPlayerAnim_link_fighter_normal_kiru_endR, 1, 4 },
    // PLAYER_MWA_FORWARD_SLASH_2H
    { &gPlayerAnim_link_fighter_Lnormal_kiru, &gPlayerAnim_link_fighter_Lnormal_kiru_end,
      &gPlayerAnim_link_anchor_Lnormal_kiru_endR, 1, 4 },
    // PLAYER_MWA_FORWARD_COMBO_1H
    { &gPlayerAnim_link_fighter_normal_kiru_finsh, &gPlayerAnim_link_fighter_normal_kiru_finsh_end,
      &gPlayerAnim_link_anchor_normal_kiru_finsh_endR, 0, 5 },
    // PLAYER_MWA_FORWARD_COMBO_2H
    { &gPlayerAnim_link_fighter_Lnormal_kiru_finsh, &gPlayerAnim_link_fighter_Lnormal_kiru_finsh_end,
      &gPlayerAnim_link_anchor_Lnormal_kiru_finsh_endR, 1, 7 },
    // PLAYER_MWA_RIGHT_SLASH_1H
    { &gPlayerAnim_link_fighter_Lside_kiru, &gPlayerAnim_link_fighter_Lside_kiru_end,
      &gPlayerAnim_link_anchor_Lside_kiru_endR, 1, 4 },
    // PLAYER_MWA_RIGHT_SLASH_2H
    { &gPlayerAnim_link_fighter_LLside_kiru, &gPlayerAnim_link_fighter_LLside_kiru_end,
      &gPlayerAnim_link_anchor_LLside_kiru_endL, 0, 5 },
    // PLAYER_MWA_RIGHT_COMBO_1H
    { &gPlayerAnim_link_fighter_Lside_kiru_finsh, &gPlayerAnim_link_fighter_Lside_kiru_finsh_end,
      &gPlayerAnim_link_anchor_Lside_kiru_finsh_endR, 2, 8 },
    // PLAYER_MWA_RIGHT_COMBO_2H
    { &gPlayerAnim_link_fighter_LLside_kiru_finsh, &gPlayerAnim_link_fighter_LLside_kiru_finsh_end,
      &gPlayerAnim_link_anchor_LLside_kiru_finsh_endR, 3, 8 },
    // PLAYER_MWA_LEFT_SLASH_1H
    { &gPlayerAnim_link_fighter_Rside_kiru, &gPlayerAnim_link_fighter_Rside_kiru_end,
      &gPlayerAnim_link_anchor_Rside_kiru_endR, 0, 4 },
    // PLAYER_MWA_LEFT_SLASH_2H
    { &gPlayerAnim_link_fighter_LRside_kiru, &gPlayerAnim_link_fighter_LRside_kiru_end,
      &gPlayerAnim_link_anchor_LRside_kiru_endR, 0, 5 },
    // PLAYER_MWA_LEFT_COMBO_1H
    { &gPlayerAnim_link_fighter_Rside_kiru_finsh, &gPlayerAnim_link_fighter_Rside_kiru_finsh_end,
      &gPlayerAnim_link_anchor_Rside_kiru_finsh_endR, 0, 6 },
    // PLAYER_MWA_LEFT_COMBO_2H
    { &gPlayerAnim_link_fighter_LRside_kiru_finsh, &gPlayerAnim_link_fighter_LRside_kiru_finsh_end,
      &gPlayerAnim_link_anchor_LRside_kiru_finsh_endL, 1, 5 },
    // PLAYER_MWA_STAB_1H
    { &gPlayerAnim_link_fighter_pierce_kiru, &gPlayerAnim_link_fighter_pierce_kiru_end,
      &gPlayerAnim_link_anchor_pierce_kiru_endR, 0, 3 },
    // PLAYER_MWA_STAB_2H
    { &gPlayerAnim_link_fighter_Lpierce_kiru, &gPlayerAnim_link_fighter_Lpierce_kiru_end,
      &gPlayerAnim_link_anchor_Lpierce_kiru_endL, 0, 3 },
    // PLAYER_MWA_STAB_COMBO_1H
    { &gPlayerAnim_link_fighter_pierce_kiru_finsh, &gPlayerAnim_link_fighter_pierce_kiru_finsh_end,
      &gPlayerAnim_link_anchor_pierce_kiru_finsh_endR, 1, 9 },
    // PLAYER_MWA_STAB_COMBO_2H
    { &gPlayerAnim_link_fighter_Lpierce_kiru_finsh, &gPlayerAnim_link_fighter_Lpierce_kiru_finsh_end,
      &gPlayerAnim_link_anchor_Lpierce_kiru_finsh_endR, 1, 8 },
    // PLAYER_MWA_FLIPSLASH_START
    { &gPlayerAnim_link_fighter_jump_rollkiru, &gPlayerAnim_link_fighter_jump_kiru_finsh,
      &gPlayerAnim_link_fighter_jump_kiru_finsh, 7, 99 },
    // PLAYER_MWA_JUMPSLASH_START
    { &gPlayerAnim_link_fighter_Lpower_jump_kiru, &gPlayerAnim_link_fighter_Lpower_jump_kiru_hit,
      &gPlayerAnim_link_fighter_Lpower_jump_kiru_hit, 7, 99 },
    // PLAYER_MWA_ZORA_JUMPKICK_START
    { &gPlayerAnim_pz_jumpAT, &gPlayerAnim_pz_jumpATend, &gPlayerAnim_pz_jumpATend, 8, 99 },
    // PLAYER_MWA_FLIPSLASH_FINISH
    { &gPlayerAnim_link_fighter_jump_kiru_finsh, &gPlayerAnim_link_fighter_jump_kiru_finsh_end,
      &gPlayerAnim_link_fighter_jump_kiru_finsh_end, 1, 2 },
    // PLAYER_MWA_JUMPSLASH_FINISH
    { &gPlayerAnim_link_fighter_Lpower_jump_kiru_hit, &gPlayerAnim_link_fighter_Lpower_jump_kiru_end,
      &gPlayerAnim_link_fighter_Lpower_jump_kiru_end, 1, 2 },
    // PLAYER_MWA_ZORA_JUMPKICK_FINISH
    { &gPlayerAnim_pz_jumpATend, &gPlayerAnim_pz_wait, &gPlayerAnim_link_normal_waitR_free, 1, 2 },
    // PLAYER_MWA_BACKSLASH_RIGHT
    { &gPlayerAnim_link_fighter_turn_kiruR, &gPlayerAnim_link_fighter_turn_kiruR_end,
      &gPlayerAnim_link_fighter_turn_kiruR_end, 1, 5 },
    // PLAYER_MWA_BACKSLASH_LEFT
    { &gPlayerAnim_link_fighter_turn_kiruL, &gPlayerAnim_link_fighter_turn_kiruL_end,
      &gPlayerAnim_link_fighter_turn_kiruL_end, 1, 4 },
    // PLAYER_MWA_GORON_PUNCH_LEFT
    { &gPlayerAnim_pg_punchA, &gPlayerAnim_pg_punchAend, &gPlayerAnim_pg_punchAendR, 6, 8 },
    // PLAYER_MWA_GORON_PUNCH_RIGHT
    { &gPlayerAnim_pg_punchB, &gPlayerAnim_pg_punchBend, &gPlayerAnim_pg_punchBendR, 12, 18 },
    // PLAYER_MWA_GORON_PUNCH_BUTT
    { &gPlayerAnim_pg_punchC, &gPlayerAnim_pg_punchCend, &gPlayerAnim_pg_punchCendR, 8, 14 },
    // PLAYER_MWA_ZORA_PUNCH_LEFT
    { &gPlayerAnim_pz_attackA, &gPlayerAnim_pz_attackAend, &gPlayerAnim_pz_attackAendR, 2, 5 },
    // PLAYER_MWA_ZORA_PUNCH_COMBO
    { &gPlayerAnim_pz_attackB, &gPlayerAnim_pz_attackBend, &gPlayerAnim_pz_attackBendR, 3, 8 },
    // PLAYER_MWA_ZORA_PUNCH_KICK
    { &gPlayerAnim_pz_attackC, &gPlayerAnim_pz_attackCend, &gPlayerAnim_pz_attackCendR, 3, 10 },
    // PLAYER_MWA_SPIN_ATTACK_1H
    { &gPlayerAnim_link_fighter_rolling_kiru, &gPlayerAnim_link_fighter_rolling_kiru_end,
      &gPlayerAnim_link_anchor_rolling_kiru_endR, 0, 12 },
    // PLAYER_MWA_SPIN_ATTACK_2H
    { &gPlayerAnim_link_fighter_Lrolling_kiru, &gPlayerAnim_link_fighter_Lrolling_kiru_end,
      &gPlayerAnim_link_anchor_Lrolling_kiru_endR, 0, 15 },
    // PLAYER_MWA_BIG_SPIN_1H
    { &gPlayerAnim_link_fighter_Wrolling_kiru, &gPlayerAnim_link_fighter_Wrolling_kiru_end,
      &gPlayerAnim_link_anchor_rolling_kiru_endR, 0, 16 },
    // PLAYER_MWA_BIG_SPIN_2H
    { &gPlayerAnim_link_fighter_Wrolling_kiru, &gPlayerAnim_link_fighter_Wrolling_kiru_end,
      &gPlayerAnim_link_anchor_Lrolling_kiru_endR, 0, 16 },
};

LinkAnimationHeader* sSpinAttackAnims2[] = {
    &gPlayerAnim_link_fighter_power_kiru_start,
    &gPlayerAnim_link_fighter_Lpower_kiru_start,
};
LinkAnimationHeader* sSpinAttackAnims1[] = {
    &gPlayerAnim_link_fighter_power_kiru_startL,
    &gPlayerAnim_link_fighter_Lpower_kiru_start,
};
LinkAnimationHeader* sSpinAttackChargeAnims[] = {
    &gPlayerAnim_link_fighter_power_kiru_wait,
    &gPlayerAnim_link_fighter_Lpower_kiru_wait,
};
LinkAnimationHeader* sCancelSpinAttackChargeAnims[] = {
    &gPlayerAnim_link_fighter_power_kiru_wait_end,
    &gPlayerAnim_link_fighter_Lpower_kiru_wait_end,
};
LinkAnimationHeader* sSpinAttackChargeWalkAnims[] = {
    &gPlayerAnim_link_fighter_power_kiru_walk,
    &gPlayerAnim_link_fighter_Lpower_kiru_walk,
};
LinkAnimationHeader* sSpinAttackChargeSidewalkAnims[] = {
    &gPlayerAnim_link_fighter_power_kiru_side_walk,
    &gPlayerAnim_link_fighter_Lpower_kiru_side_walk,
};

u8 sSmallSpinAttackMWAs[] = {
    PLAYER_MWA_SPIN_ATTACK_1H,
    PLAYER_MWA_SPIN_ATTACK_2H,
};
u8 sBigSpinAttackMWAs[] = {
    PLAYER_MWA_BIG_SPIN_1H,
    PLAYER_MWA_BIG_SPIN_2H,
};

// sBlureColors
BlureColors D_8085CF88[] = {
    { { 255, 255, 255, 255 }, { 255, 255, 255, 64 }, { 255, 255, 255, 0 }, { 255, 255, 255, 0 } },
    { { 165, 185, 255, 185 }, { 205, 225, 255, 50 }, { 255, 255, 255, 0 }, { 255, 255, 255, 0 } },
};

void Player_OverrideBlureColors(PlayState* play, Player* this, s32 colorType, s32 elemDuration) {
    EffectBlure* blure0 = Effect_GetByIndex(this->meleeWeaponEffectIndex[0]);
    EffectBlure* blure1 = Effect_GetByIndex(this->meleeWeaponEffectIndex[1]);
    s32 i;

    for (i = 0; i < 4; i++) {
        blure0->p1StartColor[i] = D_8085CF88[colorType].p1StartColor[i];
        blure0->p2StartColor[i] = D_8085CF88[colorType].p2StartColor[i];
        blure0->p1EndColor[i] = D_8085CF88[colorType].p1EndColor[i];
        blure0->p2EndColor[i] = D_8085CF88[colorType].p2EndColor[i];
        blure1->p1StartColor[i] = D_8085CF88[colorType].p1StartColor[i];
        blure1->p2StartColor[i] = D_8085CF88[colorType].p2StartColor[i];
        blure1->p1EndColor[i] = D_8085CF88[colorType].p1EndColor[i];
        blure1->p2EndColor[i] = D_8085CF88[colorType].p2EndColor[i];
    }

    if (this->transformation == PLAYER_FORM_DEKU) {
        elemDuration = 8;
    }
    blure0->elemDuration = elemDuration;
    blure1->elemDuration = elemDuration;
}

void Player_MeleeAttack(PlayState* play, Player* this, s32 meleeWeaponState) {
    u16 voiceSfx;
    u16 itemSfx;

    if (this->meleeWeaponState == 0) {
        voiceSfx = NA_SE_VO_LI_SWORD_N;
        if (this->transformation == PLAYER_FORM_GORON) {
            itemSfx = NA_SE_IT_GORON_PUNCH_SWING;
        } else {
            itemSfx = 0;
            if (this->meleeWeaponAnimation >= PLAYER_MWA_SPIN_ATTACK_1H) {
                voiceSfx = NA_SE_VO_LI_SWORD_L;
            } else if (this->meleeWeaponAnimation == PLAYER_MWA_ZORA_PUNCH_KICK) {
                itemSfx = NA_SE_IT_GORON_PUNCH_SWING;
            } else {
                itemSfx = NA_SE_IT_SWORD_SWING_HARD;
                if (this->slashCounter >= 3) {
                    voiceSfx = NA_SE_VO_LI_SWORD_L;
                } else {
                    itemSfx = (this->itemActionParam == PLAYER_AP_SWORD_GREAT_FAIRY) ? NA_SE_IT_HAMMER_SWING
                                                                                     : NA_SE_IT_SWORD_SWING;
                }
            }
        }

        if (itemSfx != 0) {
            Player_PlayReactableSfx(this, itemSfx);
        }

        if (!((this->meleeWeaponAnimation >= PLAYER_MWA_FLIPSLASH_START) &&
              (this->meleeWeaponAnimation <= PLAYER_MWA_ZORA_JUMPKICK_FINISH))) {
            Player_AnimSfx_PlayVoice(this, voiceSfx);
        }

        Player_OverrideBlureColors(play, this, 0, 4);
    }

    this->meleeWeaponState = meleeWeaponState;
}

s32 Player_SetupStartUnfriendlyZTargeting(Player* this) {
    if ((this->targetedActor != NULL) && CHECK_FLAG_ALL(this->targetedActor->flags, ACTOR_FLAG_1 | ACTOR_FLAG_4)) {
        this->stateFlags3 |= PLAYER_STATE3_80000000;
        return true;
    }

    if (this->stateFlags3 & PLAYER_STATE3_80000000) {
        this->stateFlags3 &= ~PLAYER_STATE3_80000000;
        if (this->linearVelocity == 0.0f) {
            this->currentYaw = this->actor.shape.rot.y;
        }
    }

    return false;
}

s32 Player_IsZTargeting(Player* this) {
    return Player_IsUnfriendlyZTargeting(this) || Player_IsFriendlyZTargeting(this);
}

s32 Player_IsZTargetingSetupStartUnfriendly(Player* this) {
    return Player_SetupStartUnfriendlyZTargeting(this) || Player_IsFriendlyZTargeting(this);
}

void func_8082FC60(Player* this) {
    this->leftRightBlendWeightTarget = 0.0f;
    this->leftRightBlendWeight = 0.0f;
}

s32 func_8082FC78(Player* this, ItemId item) {
    if ((item < ITEM_FD) && (Player_ItemToActionParam(this, item) == this->heldItemActionParam)) {
        return true;
    } else {
        return false;
    }
}

s32 func_8082FCC4(Player* this, ItemId item, PlayerActionParam actionParam) {
    if ((item < ITEM_FD) && (Player_ItemToActionParam(this, item) == actionParam)) {
        return true;
    } else {
        return false;
    }
}

s32 func_8082FD0C(Player* this, PlayerActionParam actionParam) {
    s32 btn;

    for (btn = EQUIP_SLOT_C_LEFT; btn <= EQUIP_SLOT_C_RIGHT; btn++) {
        if (func_8082FCC4(this, GET_CUR_FORM_BTN_ITEM(btn), actionParam)) {
            return btn;
        }
    }
    return -1;
}

u16 D_8085CFA8[] = {
    BTN_B,
    BTN_CLEFT,
    BTN_CDOWN,
    BTN_CRIGHT,
};

// Return currently-pressed button, in order of priority B, CLEFT, CDOWN, CRIGHT.
EquipSlot Player_GetButtonPressed(void) {
    EquipSlot i;

    for (i = 0; i < ARRAY_COUNT(D_8085CFA8); i++) {
        if (CHECK_BTN_ALL(sPlayerControlInput->press.button, D_8085CFA8[i])) {
            break;
        }
    }

    return i;
}

void Player_SetupUseItem(Player* this, PlayState* play) {
    if (this->stateFlags1 & (PLAYER_STATE1_HOLDING_ACTOR | PLAYER_STATE1_IN_CUTSCENE)) {
        return;
    }
    if (this->stateFlags2 & PLAYER_STATE2_KAMARO_DANCE) {
        return;
    }
    if (this->stateFlags3 & PLAYER_STATE3_BREMEN_MARCH) {
        return;
    }
    if (Player_IsShootingHookshot(this)) {
        return;
    }

    if (this->transformation == PLAYER_FORM_HUMAN) {
        if (this->currentMask != PLAYER_MASK_NONE) {
            PlayerActionParam maskActionParam = this->currentMask + PLAYER_AP_MASK_TRUTH - 1;
            s32 btn;

            btn = func_8082FD0C(this, maskActionParam);

            if (btn < 0) {
                s32 maskIdMinusOne =
                    Player_ItemToActionParam(this, GET_CUR_FORM_BTN_ITEM(this->unk_154)) - PLAYER_AP_39 - 1;

                if ((maskIdMinusOne < PLAYER_MASK_TRUTH - 1) || (maskIdMinusOne >= PLAYER_MASK_MAX - 1)) {
                    maskIdMinusOne = this->currentMask - 1;
                }
                Player_UseItem(play, this, Player_MaskIdToItemId(maskIdMinusOne));
                return;
            } else {
                if ((this->currentMask == PLAYER_MASK_GIANT) && (gSaveContext.save.playerData.magic == 0)) {
                    func_80838A20(play, this);
                }

                this->unk_154 = btn;
            }
        }
    }

    if (((this->actor.id == ACTOR_PLAYER) && (this->heldItemActionParam >= PLAYER_AP_FISHING_ROD)) &&
        !(((func_80124148(this) == 0) || (gSaveContext.jinxTimer == 0)) &&
          (func_8082FC78(this, (IREG(1) != 0) ? ITEM_FISHING_ROD : Inventory_GetBtnBItem(play)) ||
           func_8082FC78(this, C_BTN_ITEM(EQUIP_SLOT_C_LEFT)) || func_8082FC78(this, C_BTN_ITEM(EQUIP_SLOT_C_DOWN)) ||
           func_8082FC78(this, C_BTN_ITEM(EQUIP_SLOT_C_RIGHT))))) {
        Player_UseItem(play, this, ITEM_NONE);
    } else {
        s32 pad;
        ItemId item;
        s32 i = Player_GetButtonPressed();

        i = ((i >= 4) && (this->transformation == PLAYER_FORM_FIERCE_DEITY) &&
             (this->itemActionParam != PLAYER_AP_SWORD_GREAT_FAIRY))
                ? 0
                : i;

        item = func_8012364C(play, this, i);
        if (item >= ITEM_FD) {
            for (i = 0; i < ARRAY_COUNT(D_8085CFA8); i++) {
                if (CHECK_BTN_ALL(sPlayerControlInput->cur.button, D_8085CFA8[i])) {
                    break;
                }
            }

            item = func_8012364C(play, this, i);
            if ((item < ITEM_FD) && (Player_ItemToActionParam(this, item) == this->itemActionParam)) {
                D_80862B4C = 1;
            }
        } else if (item == ITEM_F0) {
            if (this->blastMaskTimer == 0) {
                EnBom* bomb = (EnBom*)Actor_Spawn(&play->actorCtx, play, ACTOR_EN_BOM, this->actor.focus.pos.x,
                                                  this->actor.focus.pos.y, this->actor.focus.pos.z, BOMB_CAT_BOMB, 0, 0,
                                                  BOMB_BODY);

                if (bomb != NULL) {
                    bomb->timer = 0;
                    this->blastMaskTimer = 310;
                }
            }
        } else if (item == ITEM_F1) {
            Player_StartBremenMarch(play, this);
        } else if (item == ITEM_F2) {
            Player_StartKamaroDance(play, this);
        } else if ((func_80124110(this, Player_ItemToActionParam(this, item)) != 0) && (gSaveContext.jinxTimer != 0)) {
            if (Message_GetState(&play->msgCtx) == TEXT_STATE_NONE) {
                Message_StartTextbox(play, 0xF7, NULL);
            }
        } else {
            this->heldItemButton = i;
            Player_UseItem(play, this, item);
        }
    }
}

void Player_SetupStartChangeItem(Player* this, PlayState* play) {
    LinkAnimationHeader* anim;
    s32 pad[3];
    u8 nextModelAnimType;
    s32 itemChangeAnim;
    s8 actionParam;
    s32 pad3;
    f32 startFrame;
    f32 endFrame;
    f32 frameSpeed;

    actionParam = Player_ItemToActionParam(this, this->heldItemId);
    Player_SetUpperActionFunc(play, this, Player_StartChangeItem);

    nextModelAnimType = gPlayerModelTypes[this->nextModelGroup].modelAnimType;
    itemChangeAnim = D_8085CD00[gPlayerModelTypes[this->modelGroup].modelAnimType][nextModelAnimType];

    if ((actionParam == PLAYER_AP_ZORA_FINS) || (this->itemActionParam == PLAYER_AP_ZORA_FINS)) {
        itemChangeAnim = (actionParam == PLAYER_AP_NONE) ? -14 : 14;
    } else if ((actionParam == PLAYER_AP_BOTTLE) || (actionParam == PLAYER_AP_11) ||
               ((actionParam == PLAYER_AP_NONE) &&
                ((this->itemActionParam == PLAYER_AP_BOTTLE) || (this->itemActionParam == PLAYER_AP_11)))) {
        itemChangeAnim = (actionParam == PLAYER_AP_NONE) ? -13 : 13;
    }

    this->itemChangeAnim = ABS_ALT(itemChangeAnim);

    anim = sItemChangeAnimsInfo[this->itemChangeAnim].anim;
    if ((anim == &gPlayerAnim_link_normal_fighter2free) && (this->currentShield == PLAYER_SHIELD_NONE)) {
        anim = &gPlayerAnim_link_normal_free2fighter_free;
    }

    endFrame = Animation_GetLastFrame(anim);

    if (itemChangeAnim >= 0) {
        frameSpeed = 1.2f;
        startFrame = 0.0f;
    } else {
        frameSpeed = -1.2f;
        startFrame = endFrame;
        endFrame = 0.0f;
    }

    if (actionParam != PLAYER_AP_NONE) {
        frameSpeed *= 2.0f;
    }

    LinkAnimation_Change(play, &this->skelAnimeUpper, anim, frameSpeed, startFrame, endFrame, ANIMMODE_ONCE, 0.0f);
    this->stateFlags3 &= ~PLAYER_STATE3_START_CHANGE_ITEM;
}

void func_808304BC(Player* this, PlayState* play) {
    if ((this->actor.id == ACTOR_PLAYER) && !(this->stateFlags3 & PLAYER_STATE3_START_CHANGE_ITEM)) {
        if ((this->itemActionParam == this->heldItemActionParam) || (this->stateFlags1 & PLAYER_STATE1_400000)) {
            if ((gSaveContext.save.playerData.health != 0) && (play->csCtx.state == CS_STATE_IDLE)) {
                if ((this->csMode == PLAYER_CSMODE_0) && (play->shootingGalleryStatus == 0) &&
                    (play->activeCamId == 0)) {
                    if (!func_8082DA90(play) && (gSaveContext.timerStates[4] != 5)) {
                        Player_SetupUseItem(this, play);
                    }
                }
            }
        }
    }

    if (this->stateFlags3 & PLAYER_STATE3_START_CHANGE_ITEM) {
        Player_SetupStartChangeItem(this, play);
    }
}

// EN_ARROW ammo related?
s32 func_808305BC(PlayState* play, Player* this, ItemId* item, s32* typeParam) {
    if (this->itemActionParam == PLAYER_AP_NUT) {
        *item = ITEM_NUT;
        *typeParam = (this->transformation == PLAYER_FORM_DEKU) ? 7 : 6;
    } else {
        *item = ITEM_BOW;
        *typeParam = (this->stateFlags1 & PLAYER_STATE1_RIDING_HORSE)
                         ? ENARROW_1
                         : this->itemActionParam - PLAYER_AP_BOW + ENARROW_2;
    }

    if (this->transformation == PLAYER_FORM_DEKU) {
        return ((gSaveContext.save.playerData.magic >= 2) ||
                ((gSaveContext.save.weekEventReg[8] & 1) && (play->sceneId == SCENE_BOWLING)))
                   ? 1
                   : 0;
    }
    if (this->stateFlags3 & PLAYER_STATE3_400) {
        return 1;
    }
    if (gSaveContext.minigameState == 1) {
        return play->interfaceCtx.hbaAmmo;
    }
    if (play->shootingGalleryStatus != 0) {
        return play->shootingGalleryStatus;
    }

    return AMMO(*item);
}

u16 D_8085CFB0[] = {
    NA_SE_PL_BOW_DRAW,
    0,
    NA_SE_IT_HOOKSHOT_READY,
};

u8 sMagicArrowCosts[] = {
    4, // ENARROW_3
    4, // ENARROW_4
    8, // ENARROW_5
    2, // ENARROW_6
};

// Draw bow or hookshot / first person items?
s32 func_808306F8(Player* this, PlayState* play) {
    if ((this->itemActionParam >= PLAYER_AP_BOW_FIRE) && (this->itemActionParam <= PLAYER_AP_BOW_LIGHT) &&
        (gSaveContext.magicState != 0)) {
        play_sound(NA_SE_SY_ERROR);
    } else {
        Player_SetUpperActionFunc(play, this, func_80848BF4);

        this->stateFlags3 |= PLAYER_STATE3_40;
        this->unk_ACC = 14;

        if (this->stickFlameTimer >= 0) {
            s32 var_v1 = ABS_ALT(this->stickFlameTimer);
            ItemId item;
            s32 arrowType;
            s32 magicArrowType;

            if (var_v1 != 2) {
                Player_PlaySfx(this, D_8085CFB0[var_v1 - 1]);
            }

            if (!Player_IsHoldingHookshot(this) && (func_808305BC(play, this, &item, &arrowType) > 0)) {
                if (this->stickFlameTimer >= 0) {
                    magicArrowType = arrowType - ENARROW_3;

                    if ((arrowType - ENARROW_3 >= ENARROW_3 - ENARROW_3) &&
                        (arrowType - ENARROW_3 <= ENARROW_5 - ENARROW_3)) {
                        if (((void)0, gSaveContext.save.playerData.magic) < sMagicArrowCosts[magicArrowType]) {
                            arrowType = ENARROW_2;
                            magicArrowType = -1;
                        }
                    } else if ((arrowType == ENARROW_7) &&
                               (!(gSaveContext.save.weekEventReg[8] & 1) || (play->sceneId != SCENE_BOWLING))) {
                        magicArrowType = ENARROW_3;
                    } else {
                        magicArrowType = -1;
                    }

                    this->heldActor = Actor_SpawnAsChild(
                        &play->actorCtx, &this->actor, play, ACTOR_EN_ARROW, this->actor.world.pos.x,
                        this->actor.world.pos.y, this->actor.world.pos.z, 0, this->actor.shape.rot.y, 0, arrowType);

                    if ((this->heldActor != NULL) && (magicArrowType >= 0)) {
                        Magic_Consume(play, sMagicArrowCosts[magicArrowType], MAGIC_CONSUME_NOW);
                    }
                }
            }
        }

        return true;
    }

    return false;
}

// Take out / put away sounds?
void Player_ChangeItemWithSfx(PlayState* play, Player* this) {
    s32 isGoronOrDeku = (this->transformation == PLAYER_FORM_GORON) || (this->transformation == PLAYER_FORM_DEKU);

    if ((this->itemActionParam != PLAYER_AP_NONE) && !isGoronOrDeku) {
        if (Player_ActionToSword(&this->actor, this->itemActionParam) >= 0) {
            Player_PlayReactableSfx(this, NA_SE_IT_SWORD_PUTAWAY);
        } else {
            Player_PlayReactableSfx(this, NA_SE_PL_CHANGE_ARMS);
        }
    }

    Player_UseItem(play, this, this->heldItemId);

    if (!isGoronOrDeku) {
        if (Player_ActionToSword(&this->actor, this->itemActionParam) >= 0) {
            Player_PlayReactableSfx(this, NA_SE_IT_SWORD_PICKOUT);
        } else if (this->itemActionParam != PLAYER_AP_NONE) {
            Player_PlayReactableSfx(this, NA_SE_PL_CHANGE_ARMS);
        }
    }
}

void Player_SetupHeldItemUpperActionFunc(PlayState* play, Player* this) {
    if (Player_StartChangeItem == this->upperActionFunc) {
        Player_ChangeItemWithSfx(play, this);
    }

    Player_SetUpperActionFunc(play, this, D_8085C9F0[this->itemActionParam]);
    this->unk_ACC = 0;
    this->unk_AA4 = 0;
    Player_DetatchHeldActor(play, this);
    this->stateFlags3 &= ~PLAYER_STATE3_START_CHANGE_ITEM;
}

LinkAnimationHeader* sRightDefendStandingAnims[2] = {
    &gPlayerAnim_link_anchor_waitR2defense,
    &gPlayerAnim_link_anchor_waitR2defense_long,
};
LinkAnimationHeader* sLeftDefendStandingAnims[2] = {
    &gPlayerAnim_link_anchor_waitL2defense,
    &gPlayerAnim_link_anchor_waitL2defense_long,
};
LinkAnimationHeader* D_8085CFCC[2] = {
    &gPlayerAnim_link_anchor_defense_hit,
    &gPlayerAnim_link_anchor_defense_long_hitL,
};
LinkAnimationHeader* D_8085CFD4[2] = {
    &gPlayerAnim_link_anchor_defense_hit,
    &gPlayerAnim_link_anchor_defense_long_hitR,
};
LinkAnimationHeader* D_8085CFDC[2] = {
    &gPlayerAnim_link_normal_defense_hit,
    &gPlayerAnim_link_fighter_defense_long_hit,
};

LinkAnimationHeader* Player_GetStandingDefendAnim(PlayState* play, Player* this) {
    Player_SetUpperActionFunc(play, this, Player_StandingDefend);
    Player_DetatchHeldActor(play, this);

    if (this->leftRightBlendWeight < 0.5f) {
        return sRightDefendStandingAnims[Player_IsHoldingTwoHandedWeapon(this)];
    } else {
        return sLeftDefendStandingAnims[Player_IsHoldingTwoHandedWeapon(this)];
    }
}

void func_80830AE8(Player* this) {
    s32 sfxId;

    if (this->transformation == PLAYER_FORM_GORON) {
        sfxId = NA_SE_PL_GORON_SQUAT;
    } else {
        s32 var_v1;

        if (this->transformation == PLAYER_FORM_DEKU) {
            var_v1 = NA_SE_PL_CHANGE_ARMS;
        } else {
            var_v1 = NA_SE_IT_SHIELD_SWING;
        }
        sfxId = var_v1;
    }

    Player_PlaySfx(this, sfxId);
}

void func_80830B38(Player* this) {
    s32 sfxId;

    if (this->transformation == PLAYER_FORM_GORON) {
        sfxId = NA_SE_PL_BALL_TO_GORON;
    } else {
        s32 var_v1;

        if (this->transformation == PLAYER_FORM_DEKU) {
            var_v1 = NA_SE_PL_TAKE_OUT_SHIELD;
        } else {
            var_v1 = NA_SE_IT_SHIELD_REMOVE;
        }
        sfxId = var_v1;
    }

    Player_PlaySfx(this, sfxId);
}

s32 Player_StartZTargetDefend(PlayState* play, Player* this) {
    if (CHECK_BTN_ALL(sPlayerControlInput->cur.button, BTN_R)) {
        if (!(this->stateFlags1 & (PLAYER_STATE1_400000 | PLAYER_STATE1_RIDING_HORSE | PLAYER_STATE1_IN_CUTSCENE))) {
            if (!(this->stateFlags1 & PLAYER_STATE1_SWIMMING) ||
                ((this->currentBoots >= PLAYER_BOOTS_ZORA_UNDERWATER) && (this->actor.bgCheckFlags & 1))) {
                if ((play->shootingGalleryStatus == 0) && (this->itemActionParam == this->heldItemActionParam)) {
                    if ((this->transformation == PLAYER_FORM_FIERCE_DEITY) ||
                        (!Player_IsGoronOrDeku(this) &&
                         ((((this->transformation == PLAYER_FORM_ZORA)) &&
                           !(this->stateFlags1 & PLAYER_STATE1_2000000)) ||
                          ((this->transformation == PLAYER_FORM_HUMAN) &&
                           (this->currentShield != PLAYER_SHIELD_NONE))) &&
                         Player_IsZTargeting(this))) {
                        LinkAnimationHeader* anim = Player_GetStandingDefendAnim(play, this);
                        f32 lastFrame;

                        lastFrame = Animation_GetLastFrame(anim);
                        LinkAnimation_Change(play, &this->skelAnimeUpper, anim, 3.0f / 3.0f, lastFrame, lastFrame,
                                             ANIMMODE_ONCE, 0.0f);
                        func_80830AE8(this);
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

void func_80830CE8(PlayState* play, Player* this) {
    Player_SetUpperActionFunc(play, this, func_80848AB0);

    if (this->heldItemActionParam < 0) {
        func_80123C58(this);
    }

    Animation_Reverse(&this->skelAnimeUpper);
    func_80830B38(this);
}

void Player_SetupChangeItem(PlayState* play, Player* this) {
    ItemChangeAnimInfo* entry = &sItemChangeAnimsInfo[this->itemChangeAnim];
    f32 itemChangeFrame;

    itemChangeFrame = entry->frame;
    if (this->skelAnimeUpper.playSpeed < 0.0f) {
        itemChangeFrame -= 1.0f;
    }
    if (LinkAnimation_OnFrame(&this->skelAnimeUpper, itemChangeFrame)) {
        Player_ChangeItemWithSfx(play, this);
    }
    Player_SetupStartUnfriendlyZTargeting(this);
}

s32 func_80830DF0(Player* this, PlayState* play) {
    if (this->stateFlags3 & PLAYER_STATE3_START_CHANGE_ITEM) {
        Player_SetupStartChangeItem(this, play);
    } else {
        return false;
    }
    return true;
}

s32 func_80830E30(Player* this, PlayState* play) {
    if ((this->itemActionParam == PLAYER_AP_11) || (this->transformation == PLAYER_FORM_ZORA)) {
        Player_SetUpperActionFunc(play, this, func_8084923C);

        LinkAnimation_PlayOnce(play, &this->skelAnimeUpper,
                               (this->meleeWeaponAnimation == PLAYER_MWA_ZORA_PUNCH_LEFT)
                                   ? &gPlayerAnim_pz_cutterwaitA
                                   : ((this->meleeWeaponAnimation == PLAYER_MWA_ZORA_PUNCH_COMBO)
                                          ? &gPlayerAnim_pz_cutterwaitB
                                          : &gPlayerAnim_pz_cutterwaitC));
        this->unk_ACC = 0xA;
    } else {
        if (!func_808306F8(this, play)) {
            return false;
        }

        LinkAnimation_PlayOnce(play, &this->skelAnimeUpper,
                               (Player_IsHoldingHookshot(this))
                                   ? &gPlayerAnim_link_hook_shot_ready
                                   : ((this->transformation == PLAYER_FORM_DEKU) ? &gPlayerAnim_pn_tamahakidf
                                                                                 : &gPlayerAnim_link_bow_bow_ready));
    }

    if (this->stateFlags1 & PLAYER_STATE1_RIDING_HORSE) {
        Player_AnimationPlayLoop(play, this, &gPlayerAnim_link_uma_anim_walk);
    } else if ((this->actor.bgCheckFlags & 1) && (this->transformation != PLAYER_FORM_ZORA)) {
        Player_AnimationPlayLoop(play, this, Player_GetStandingStillAnim(this));
    }

    return true;
}

s32 Player_CheckShootingGalleryShootInput(PlayState* play) {
    return (play->shootingGalleryStatus > 0) && CHECK_BTN_ALL(sPlayerControlInput->press.button, BTN_B);
}

s32 func_80830FD4(PlayState* play) {
    return (play->shootingGalleryStatus != 0) &&
           ((play->shootingGalleryStatus < 0) ||
            CHECK_BTN_ANY(sPlayerControlInput->cur.button,
                          BTN_CRIGHT | BTN_CLEFT | BTN_CDOWN | BTN_CUP | BTN_B | BTN_A));
}

s32 Player_SetupAimAttention(Player* this, PlayState* play) {
    if ((this->attentionMode == PLAYER_ATTENTIONMODE_NONE) || (this->attentionMode == PLAYER_ATTENTIONMODE_AIMING)) {
        if (Player_IsZTargeting(this) || (this->targetedActor != NULL) ||
            (func_800DF86C(Play_GetCamera(play, CAM_ID_MAIN), 0xD) == 0)) {
            return true;
        }
        this->attentionMode = PLAYER_ATTENTIONMODE_AIMING;
    }
    return false;
}

s32 Player_CanUseFpsItem(Player* this, PlayState* play) {
    if ((this->doorType == PLAYER_DOORTYPE_NONE) && !(this->stateFlags1 & PLAYER_STATE1_2000000)) {
        if ((sUsingItemAlreadyInHand != 0) || Player_CheckShootingGalleryShootInput(play)) {
            if (func_80830E30(this, play)) {
                return Player_SetupAimAttention(this, play);
            }
        }
    }
    return false;
}

s32 Player_EndHookshotMove(PlayState* play, Player* this) {
    if (this->actor.child != NULL) {
        if (this->heldActor == NULL) {
            this->heldActor = this->actor.child;
            Player_RequestRumble(play, this, 255, 10, 250, SQ(0));
            Player_PlaySfx(this, NA_SE_IT_HOOKSHOT_RECEIVE);
        }
        return true;
    }
    return false;
}

s32 func_80831194(PlayState* play, Player* this) {
    if (this->heldActor != NULL) {
        if (!Player_IsHoldingHookshot(this)) {
            ItemId item;
            s32 sp30;

            func_808305BC(play, this, &item, &sp30);
            if ((this->transformation != PLAYER_FORM_DEKU) && !(this->stateFlags3 & PLAYER_STATE3_400)) {
                if (gSaveContext.minigameState == 1) {
                    if ((play->sceneId != SCENE_SYATEKI_MIZU) && (play->sceneId != SCENE_F01) &&
                        (play->sceneId != SCENE_SYATEKI_MORI)) {
                        play->interfaceCtx.hbaAmmo -= 1;
                    }
                } else if (play->shootingGalleryStatus != 0) {
                    play->shootingGalleryStatus--;
                } else {
                    Inventory_ChangeAmmo(item, -1);
                }
            }

            if (play->shootingGalleryStatus == 1) {
                play->shootingGalleryStatus = -0xA;
            }

            Player_RequestRumble(play, this, 150, 10, 150, SQ(0));
        } else {
            Player_RequestRumble(play, this, 255, 20, 150, SQ(0));
            this->unk_B48 = 0.0f;
        }

        this->fpsItemShotTimer = (this->transformation == PLAYER_FORM_DEKU) ? 20 : 4;

        this->heldActor->parent = NULL;
        this->actor.child = NULL;
        this->heldActor = NULL;
        return 1;
    }

    return 0;
}

void Player_SetZTargetFriendlyYaw(Player* this) {
    this->stateFlags1 |= PLAYER_STATE1_Z_TARGETING_FRIENDLY;

    if (!(this->skelAnime.moveFlags & 0x80) && (this->actor.bgCheckFlags & BGCHECKFLAG_PLAYER_WALL_INTERACT) &&
        (sYawToTouchedWall < 0x2000)) {
        this->currentYaw = this->actor.shape.rot.y = this->actor.wallYaw + 0x8000;
    }

    this->targetYaw = this->actor.shape.rot.y;
}

s32 Player_InterruptHoldingActor(PlayState* play, Player* this, Actor* actor) {
    if (actor == NULL) {
        Player_ResetAttributesAndHeldActor(play, this);
        Player_SetupStandingStillType(this, play);
        return true;
    }

    return false;
}

void Player_SetupHoldActorUpperAction(Player* this, PlayState* play) {
    if (!Player_InterruptHoldingActor(play, this, this->heldActor)) {
        Player_SetUpperActionFunc(play, this, Player_HoldActor);
        LinkAnimation_PlayLoop(play, &this->skelAnimeUpper, &gPlayerAnim_link_normal_carryB_wait);
    }
}

// Stops the current fanfare if a stateflag is set; these two are Kamaro Dancing and Bremen Marching.
void func_80831454(Player* this) {
    if ((this->stateFlags3 & PLAYER_STATE3_BREMEN_MARCH) || (this->stateFlags2 & PLAYER_STATE2_KAMARO_DANCE)) {
        Audio_QueueSeqCmd(0x110000FF);
    }
}

s32 Player_SetAction(PlayState* play, Player* this, PlayerActionFunc actionFunc, s32 arg3) {
    s32 i;
    f32* ptr;

    if (actionFunc == this->actionFunc) {
        return false;
    }

    play->actorCtx.flags &= ~ACTORCTX_FLAG_PICTOGRAPH_ON;

    if (this->actor.flags & ACTOR_FLAG_OCARINA_READY) {
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_OFF);
        this->actor.flags &= ~ACTOR_FLAG_OCARINA_READY;
    } else if ((func_80857BE8 == this->actionFunc) || (func_808561B0 == this->actionFunc)) {
        this->actor.shape.shadowDraw = ActorShadow_DrawFeet;
        this->actor.shape.shadowScale = this->ageProperties->shadowScale;
        this->unk_ABC = 0.0f;
        if (func_80857BE8 == this->actionFunc) {
            if (this->stateFlags3 & PLAYER_STATE3_80000) {
                Magic_Reset(play);
            }
            Player_ResetAttributes(play, this);
            this->actor.shape.rot.x = 0;
            this->actor.shape.rot.z = 0;
            this->actor.bgCheckFlags &= ~0x0800;
        } else {
            Actor_SetScale(&this->actor, 0.01f);
        }
    } else if ((this->transformation == PLAYER_FORM_GORON) && (Player_GetMeleeWeaponHeld(this) != 0)) {
        Player_UseItem(play, this, ITEM_NONE);
    }

    func_800AEF44(Effect_GetByIndex(this->meleeWeaponEffectIndex[2]));
    this->actionFunc = actionFunc;

    if ((this->heldItemActionParam != this->itemActionParam) &&
        (!(arg3 & 1) || !(this->stateFlags1 & PLAYER_STATE1_400000))) {
        func_80123C58(this);
    }

    if (!(arg3 & 1) && !(this->stateFlags1 & PLAYER_STATE1_HOLDING_ACTOR)) {
        Player_SetupHeldItemUpperActionFunc(play, this);
        LinkAnimation_PlayLoop(play, &this->skelAnimeUpper, Player_GetStandingStillAnim(this));
        this->stateFlags1 &= ~PLAYER_STATE1_400000;
    }

    func_80831454(this);
    Player_EndAnimMovement(this);

    this->stateFlags1 &= ~(PLAYER_STATE1_40 | PLAYER_STATE1_4000000 | PLAYER_STATE1_SKIP_OTHER_ACTORS_UPDATE |
                           PLAYER_STATE1_IN_CUTSCENE | PLAYER_STATE1_FALLING_INTO_GROTTO_OR_VOID);
    this->stateFlags2 &= ~(PLAYER_STATE2_80000 | PLAYER_STATE2_800000 | PLAYER_STATE2_KAMARO_DANCE |
                           PLAYER_STATE2_OCARINA_ON | PLAYER_STATE2_10000000);
    this->stateFlags3 &=
        ~(PLAYER_STATE3_MIDAIR | PLAYER_STATE3_8 | PLAYER_STATE3_80 | PLAYER_STATE3_200 | PLAYER_STATE3_2000 |
          PLAYER_STATE3_8000 | PLAYER_STATE3_10000 | PLAYER_STATE3_20000 | PLAYER_STATE3_40000 | PLAYER_STATE3_80000 |
          PLAYER_STATE3_200000 | PLAYER_STATE3_1000000 | PLAYER_STATE3_BREMEN_MARCH);

    this->genericVar = 0;
    this->genericTimer = 0;
    this->unk_AA4 = 0;
    this->unk_B86[0] = 0;
    this->unk_B86[1] = 0;
    this->unk_B8A = 0;
    this->unk_B8C = 0;
    this->unk_B8E = 0;

    // TODO: Is there no other way to write this that works?
    i = 0;
    ptr = this->unk_B10;
    do {
        *ptr = 0.0f;
        ptr++;
        i++;
    } while (i < ARRAY_COUNT(this->unk_B10));

    this->actor.shape.rot.z = 0;

    Player_ResetCylinder(this);
    Player_StopInterruptableSfx(this);

    return true;
}

void Player_SetActionFuncPreserveMoveFlags(PlayState* play, Player* this, PlayerActionFunc actionFunc, s32 arg3) {
    s32 moveFlags = this->skelAnime.moveFlags;

    this->skelAnime.moveFlags = 0;
    Player_SetAction(play, this, actionFunc, arg3);
    this->skelAnime.moveFlags = moveFlags;
}

void func_80831760(PlayState* play, Player* this, PlayerActionFunc actionFunc, s32 arg3) {
    if (this->heldItemActionParam >= 0) {
        s32 heldItemActionParam = this->heldItemActionParam;

        this->heldItemActionParam = this->itemActionParam;
        Player_SetAction(play, this, actionFunc, arg3);
        this->heldItemActionParam = heldItemActionParam;

        Player_SetModels(this, Player_ActionToModelGroup(this, this->heldItemActionParam));
    }
}

void Player_PutAwayHookshot(Player* this) {
    if (Player_IsHoldingHookshot(this)) {
        if (this->heldActor != NULL) {
            Actor_MarkForDeath(this->heldActor);
            this->actor.child = NULL;
            this->heldActor = NULL;
        }
    }
}

s32 func_80831814(Player* this, PlayState* play, PlayerUnkAA5 arg2) {
    if (!(this->stateFlags1 &
          (PLAYER_STATE1_4 | PLAYER_STATE1_HOLDING_ACTOR | PLAYER_STATE1_2000 | PLAYER_STATE1_4000))) {
        if (func_800DF86C(Play_GetCamera(play, CAM_ID_MAIN), 6) != 0) {
            if ((this->actor.bgCheckFlags & 1) ||
                (Player_IsSwimming(this) && (this->actor.depthInWater < this->ageProperties->unk_2C))) {
                this->attentionMode = arg2;
                return true;
            }
        }
    }
    return false;
}

// Toggle Lens
void func_808318C0(PlayState* play) {
    if (Magic_Consume(play, 0, MAGIC_CONSUME_LENS)) {
        if (play->actorCtx.lensActive) {
            Actor_DeactivateLens(play);
        } else {
            play->actorCtx.lensActive = true;
        }

        play_sound(play->actorCtx.lensActive ? NA_SE_SY_GLASSMODE_ON : NA_SE_SY_GLASSMODE_OFF);
    } else {
        play_sound(NA_SE_SY_ERROR);
    }
}

// Toggle Lens from a button press
void func_80831944(PlayState* play, Player* this) {
    if (func_8012364C(play, this, Player_GetButtonPressed()) == ITEM_LENS) {
        func_808318C0(play);
    }
}

// Proposed name: Player_UseItem
void Player_UseItem(PlayState* play, Player* this, ItemId item) {
    PlayerActionParam actionParam = Player_ItemToActionParam(this, item);

    if ((((this->itemActionParam == this->heldItemActionParam) &&
          (!(this->stateFlags1 & PLAYER_STATE1_400000) || (Player_ActionToMeleeWeapon(actionParam) != 0) ||
           (actionParam == PLAYER_AP_NONE))) ||
         ((this->heldItemActionParam < PLAYER_AP_NONE) &&
          ((Player_ActionToMeleeWeapon(actionParam) != 0) || (actionParam == PLAYER_AP_NONE)))) &&
        ((actionParam == PLAYER_AP_NONE) || !(this->stateFlags1 & PLAYER_STATE1_SWIMMING) ||
         (actionParam == PLAYER_AP_MASK_ZORA) ||
         ((this->currentBoots >= PLAYER_BOOTS_ZORA_UNDERWATER) && (this->actor.bgCheckFlags & 1)))) {
        s32 var_v1 = ((actionParam >= PLAYER_AP_MASK_TRUTH) && (actionParam <= PLAYER_AP_MASK_DEKU) &&
                      ((this->transformation != PLAYER_FORM_HUMAN) || (actionParam >= PLAYER_AP_MASK_GIANT)));
        CollisionPoly* sp5C;
        s32 sp58;
        f32 sp54;
        s32 explAction;

        if (var_v1 || (CHECK_FLAG_ALL(this->actor.flags, ACTOR_FLAG_100) && (actionParam != PLAYER_AP_NONE)) ||
            (actionParam == PLAYER_AP_OCARINA) ||
            ((actionParam > PLAYER_AP_BOTTLE) && actionParam < PLAYER_AP_MASK_TRUTH) ||
            ((actionParam == PLAYER_AP_PICTO_BOX) && (this->talkActor != NULL) &&
             (this->exchangeItemId > PLAYER_AP_NONE))) {
            if (var_v1) {
                PlayerTransformation playerForm = (actionParam < PLAYER_AP_MASK_FIERCE_DEITY)
                                                      ? PLAYER_FORM_HUMAN
                                                      : actionParam - PLAYER_AP_MASK_FIERCE_DEITY;

                if (((this->currentMask != PLAYER_MASK_GIANT) && (actionParam == PLAYER_AP_MASK_GIANT) &&
                     ((gSaveContext.magicState != 0) || (gSaveContext.save.playerData.magic == 0))) ||
                    (!(this->stateFlags1 & PLAYER_STATE1_SWIMMING) &&
                     BgCheck_EntityCheckCeiling(&play->colCtx, &sp54, &this->actor.world.pos,
                                                sPlayerAgeProperties[playerForm].unk_00, &sp5C, &sp58, &this->actor))) {
                    play_sound(NA_SE_SY_ERROR);
                    return;
                }
            }
            if ((actionParam == PLAYER_AP_MAGIC_BEANS) && (AMMO(ITEM_MAGIC_BEANS) == 0)) {
                play_sound(NA_SE_SY_ERROR);
            } else {
                this->heldItemActionParam = actionParam;
                this->attentionMode = PLAYER_UNKAA5_5;
            }
        } else if (((actionParam == PLAYER_AP_STICK) && (AMMO(ITEM_STICK) == 0)) ||
                   (((play->unk_1887D != 0) || (play->unk_1887E != 0)) &&
                    (play->actorCtx.actorLists[ACTORCAT_EXPLOSIVES].length >= 5)) ||
                   ((play->unk_1887D == 0) && (play->unk_1887E == 0) &&
                    ((explAction = Player_ActionToExplosive(this, actionParam)) >= 0) &&
                    ((AMMO(sPlayerExplosiveInfo[explAction].itemId) == 0) ||
                     (play->actorCtx.actorLists[ACTORCAT_EXPLOSIVES].length >= 3)))) {
            play_sound(NA_SE_SY_ERROR);
        } else if (actionParam == PLAYER_AP_LENS) {
            func_808318C0(play);
        } else if (actionParam == PLAYER_AP_PICTO_BOX) {
            if (!func_80831814(this, play, PLAYER_UNKAA5_2)) {
                play_sound(NA_SE_SY_ERROR);
            }
        } else if ((actionParam == PLAYER_AP_NUT) &&
                   ((this->transformation != PLAYER_FORM_DEKU) || (this->heldItemButton != 0))) {
            if (AMMO(ITEM_NUT) != 0) {
                func_8083A658(play, this);
            } else {
                play_sound(NA_SE_SY_ERROR);
            }
        } else if ((this->transformation == PLAYER_FORM_HUMAN) && (actionParam >= PLAYER_AP_MASK_TRUTH) &&
                   (actionParam < PLAYER_AP_MASK_GIANT)) {
            s32 maskId = actionParam - PLAYER_AP_39;

            this->prevMask = this->currentMask;
            if (maskId == this->currentMask) {
                this->currentMask = PLAYER_MASK_NONE;
                Player_PlayReactableSfx(this, NA_SE_PL_TAKE_OUT_SHIELD);
            } else {
                this->currentMask = maskId;
                Player_PlayReactableSfx(this, NA_SE_PL_CHANGE_ARMS);
            }
            gSaveContext.save.equippedMask = this->currentMask;
        } else if ((actionParam != this->itemActionParam) ||
                   ((this->heldActor == NULL) && (Player_ActionToExplosive(this, actionParam) >= 0))) {
            u8 nextAnimType;

            this->nextModelGroup = Player_ActionToModelGroup(this, actionParam);
            nextAnimType = gPlayerModelTypes[this->nextModelGroup].modelAnimType;
            var_v1 = ((this->transformation != PLAYER_FORM_GORON) || (actionParam == PLAYER_AP_POWDER_KEG));

            if (var_v1 && (this->itemActionParam >= 0) && (item != this->heldItemId) &&
                (D_8085CD00[gPlayerModelTypes[this->modelGroup].modelAnimType][nextAnimType] != 0)) {
                this->heldItemId = item;
                this->stateFlags3 |= PLAYER_STATE3_START_CHANGE_ITEM;
            } else {
                Player_PutAwayHookshot(this);
                Player_DetatchHeldActor(play, this);
                func_8082F470(play, this, actionParam);
                if (!var_v1) {
                    sUsingItemAlreadyInHand = 1;
                    D_80862B4C = 1;
                }
            }
        } else {
            sUsingItemAlreadyInHand = 1;
            D_80862B4C = 1;
        }
    }
}

void func_80831F34(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    s32 sp24 = Player_IsSwimming(this);

    Player_ResetAttributesAndHeldActor(play, this);
    Player_SetAction(play, this, sp24 ? func_80851BD4 : Player_Die, 0);
    Player_AnimationPlayOnce(play, this, anim);

    if (anim == &gPlayerAnim_link_derth_rebirth) {
        this->skelAnime.endFrame = 84.0f;
    }

    this->stateFlags1 |= PLAYER_STATE1_IN_DEATH_CUTSCENE;

    Player_ClearAttentionModeAndStopMoving(this);
    Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_DOWN);

    if (this == GET_PLAYER(play)) {
        this->unk_A86 = play->playerActorCsIds[6];
        func_801A0184();
        gSaveContext.powderKegTimer = 0;
        gSaveContext.unk_1014 = 0;
        gSaveContext.jinxTimer = 0;

        if (Inventory_ConsumeFairy(play)) {
            play->gameOverCtx.state = GAMEOVER_REVIVE_START;
            this->genericVar = 1;
        } else {
            play->gameOverCtx.state = GAMEOVER_DEATH_START;
            func_801A41F8(0);
            Audio_PlayFanfare(NA_BGM_GAME_OVER);
            gSaveContext.seqId = (u8)NA_BGM_DISABLED;
            gSaveContext.ambienceId = 0xFF;
        }

        ShrinkWindow_Letterbox_SetSizeTarget(32);
    }
}

s32 func_80832090(Player* this) {
    return (!(Player_RunMiniCutsceneFunc == this->actionFunc) ||
            (((this->stateFlags3 & PLAYER_STATE3_START_CHANGE_ITEM)) &&
             ((this->heldItemId == ITEM_FC) || (this->heldItemId == ITEM_NONE)))) &&
           (!(Player_StartChangeItem == this->upperActionFunc) ||
            Player_ItemToActionParam(this, this->heldItemId) == this->itemActionParam);
}

// Whether action is Bremen marching or Kamaro dancing
s32 func_8083213C(Player* this) {
    return func_8084AC84 == this->actionFunc || func_8084AEEC == this->actionFunc;
}

s32 Player_SetupCurrentUpperAction(Player* this, PlayState* play) {
    if (!(this->stateFlags1 & PLAYER_STATE1_RIDING_HORSE) && (this->actor.parent != NULL) &&
        Player_IsHoldingHookshot(this)) {
        Player_SetAction(play, this, func_80855E08, 1);
        this->stateFlags3 |= PLAYER_STATE3_80;
        Player_AnimationPlayOnce(play, this, &gPlayerAnim_link_hook_fly_start);
        Player_SetupAnimMovement(play, this, (1 | ANIM_FLAG_UPDATE_Y | 8 | ANIM_FLAG_NOMOVE | 0x80));
        Player_ClearAttentionModeAndStopMoving(this);
        this->currentYaw = this->actor.shape.rot.y;
        this->actor.bgCheckFlags &= ~1;
        this->lookFlags |= 0x43;
        Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_LASH);
        return true;
    }

    if (func_80832090(this)) {
        func_808304BC(this, play);
        if (Player_ThrowDekuNut == this->actionFunc) {
            return true;
        }
    }

    if (!this->upperActionFunc(this, play)) {
        return false;
    }

    if (this->unk_AC8 != 0.0f) {
        if ((func_8082ED94(this) == 0) || (this->linearVelocity != 0.0f)) {
            AnimationContext_SetCopyFalse(play, this->skelAnime.limbCount, this->skelAnimeUpper.jointTable,
                                          this->skelAnime.jointTable, D_8085B9F0);
        }
        if ((this->actor.bgCheckFlags & 1) && !(this->skelAnime.moveFlags & 8)) {
            Math_StepToF(&this->unk_AC8, 0.0f, 0.25f);
            AnimationContext_SetInterp(play, this->skelAnime.limbCount, this->skelAnime.jointTable,
                                       this->skelAnimeUpper.jointTable, 1.0f - this->unk_AC8);
        }
    } else if ((func_8082ED94(this) == 0) || (this->linearVelocity != 0.0f) || (this->skelAnime.moveFlags & 8)) {
        AnimationContext_SetCopyTrue(play, this->skelAnime.limbCount, this->skelAnime.jointTable,
                                     this->skelAnimeUpper.jointTable, D_8085B9F0);
    } else {
        AnimationContext_SetCopyAll(play, this->skelAnime.limbCount, this->skelAnime.jointTable,
                                    this->skelAnimeUpper.jointTable);
    }

    return true;
}

s32 func_808323C0(Player* this, s16 csIndex) {
    if ((csIndex >= 0) && (ActorCutscene_GetCurrentIndex() != csIndex)) {
        if (!ActorCutscene_GetCanPlayNext(csIndex)) {
            ActorCutscene_SetIntentToPlay(csIndex);

            return false;
        }
        ActorCutscene_Start(csIndex, &this->actor);
    }
    this->unk_A86 = csIndex;

    return true;
}

s32 func_80832444(Player* this) {
    if (this->unk_A86 >= 0) {
        if (!ActorCutscene_GetCanPlayNext(this->unk_A86)) {
            ActorCutscene_SetIntentToPlay(this->unk_A86);
            return false;
        }

        ActorCutscene_Start(this->unk_A86, &this->actor);
    }
    return true;
}

s32 func_8083249C(Player* this) {
    if ((this->unk_A86 >= 0) && (ActorCutscene_GetCurrentIndex() != this->unk_A86)) {
        return func_80832444(this);
    }
    return true;
}

s32 Player_SetupMiniCsFunc(PlayState* play, Player* this, PlayerMiniCsFunc func, s32 arg3) {
    this->miniCsFunc = func;
    this->unk_A86 = arg3;
    Player_SetAction(play, this, Player_RunMiniCutsceneFunc, 0);
    func_8083249C(this);
    this->stateFlags2 |= PLAYER_STATE2_ALWAYS_DISABLE_MOVE_ROTATION;
    return Player_UnequipItem(play, this);
}

s32 func_80832558(PlayState* play, Player* this, PlayerMiniCsFunc func) {
    return Player_SetupMiniCsFunc(play, this, func, -1);
}

// To do with turning, related to targeting
void Player_UpdateYaw(Player* this, PlayState* play) {
    s16 previousYaw = this->actor.shape.rot.y;

    if (!(this->stateFlags2 &
          (PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING | PLAYER_STATE2_ALWAYS_DISABLE_MOVE_ROTATION))) {
        Actor* targetedActor = this->targetedActor;

        if ((targetedActor != NULL) && ((play->actorCtx.targetContext.unk4B != 0) || (this != GET_PLAYER(play))) &&
            (targetedActor->id != ACTOR_OBJ_NOZOKI)) {
            Math_ScaledStepToS(&this->actor.shape.rot.y,
                               Math_Vec3f_Yaw(&this->actor.world.pos, &targetedActor->focus.pos), 0xFA0);
        } else if ((this->stateFlags1 & PLAYER_STATE1_Z_TARGETING_FRIENDLY) &&
                   !(this->stateFlags2 & (PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING |
                                          PLAYER_STATE2_ALWAYS_DISABLE_MOVE_ROTATION))) {
            Math_ScaledStepToS(&this->actor.shape.rot.y, this->targetYaw, 0xFA0);
        }
    } else if (!(this->stateFlags2 & PLAYER_STATE2_ALWAYS_DISABLE_MOVE_ROTATION)) {
        Math_ScaledStepToS(&this->actor.shape.rot.y, this->currentYaw, 0x7D0);
    }

    this->unk_B4C = this->actor.shape.rot.y - previousYaw;
}

// Steps angle based on offset from referenceAngle, then returns any excess angle difference beyond angleMinMax
s16 Player_StepAngleWithOffset(s16* angle, s16 target, s16 step, s16 angleMinMax, s16 referenceAngle,
                               s16 angleDiffMinMax) {
    s16 angleDiff;
    s16 clampedAngleDiff;
    s16 originalAngle;

    angleDiff = clampedAngleDiff = referenceAngle - *angle;
    clampedAngleDiff = CLAMP(clampedAngleDiff, -angleDiffMinMax, angleDiffMinMax);
    *angle += BINANG_SUB(angleDiff, clampedAngleDiff);

    Math_ScaledStepToS(angle, target, step);

    originalAngle = *angle;
    if (*angle < -angleMinMax) {
        *angle = -angleMinMax;
    } else if (angleMinMax < *angle) {
        *angle = angleMinMax;
    }
    return originalAngle - *angle;
}

s16 Player_UpdateLookAngles(Player* this, s32 syncUpperRotToFocusRot) {
    s16 yawDiff;
    s16 lookYaw = this->actor.shape.rot.y;

    if (syncUpperRotToFocusRot) {
        this->upperBodyRot.x = this->actor.focus.rot.x;
        lookYaw = this->actor.focus.rot.y;
        this->lookFlags |= (PLAYER_LOOKFLAGS_OVERRIDE_UPPERBODY_ROT_X | PLAYER_LOOKFLAGS_OVERRIDE_FOCUS_ROT_X);
    } else {
        s16 temp = Player_StepAngleWithOffset(&this->headRot.x, this->actor.focus.rot.x, 0x258, 0x2710,
                                              this->actor.focus.rot.x, 0);

        Player_StepAngleWithOffset(&this->upperBodyRot.x, temp, 0xC8, 0xFA0, this->headRot.x, 0x2710);

        yawDiff = this->actor.focus.rot.y - lookYaw;
        Player_StepAngleWithOffset(&yawDiff, 0, 0xC8, 0x5DC0, this->upperBodyRot.y, 0x1F40);
        lookYaw = this->actor.focus.rot.y - yawDiff;
        Player_StepAngleWithOffset(&this->headRot.y, (yawDiff - this->upperBodyRot.y), 0xC8, 0x1F40, yawDiff, 0x1F40);
        Player_StepAngleWithOffset(&this->upperBodyRot.y, yawDiff, 0xC8, 0x1F40, this->headRot.y, 0x1F40);
        this->lookFlags |= PLAYER_LOOKFLAGS_OVERRIDE_UPPERBODY_ROT_Y | PLAYER_LOOKFLAGS_OVERRIDE_UPPERBODY_ROT_X |
                           PLAYER_LOOKFLAGS_OVERRIDE_HEAD_ROT_Y | PLAYER_LOOKFLAGS_OVERRIDE_HEAD_ROT_X |
                           PLAYER_LOOKFLAGS_OVERRIDE_FOCUS_ROT_X;
    }

    return lookYaw;
}

void func_80832888(Player* this, PlayState* play) {
    s32 var_v1 = 0;
    Actor* var_v1_2;
    s32 pressedZ = CHECK_BTN_ALL(sPlayerControlInput->cur.button, BTN_Z);
    s32 temp_v0_3;
    s32 var_a1;

    if (!pressedZ) {
        this->stateFlags1 &= ~PLAYER_STATE1_40000000;
    }

    if ((play->csCtx.state != CS_STATE_IDLE) || (this->csMode != PLAYER_CSMODE_0) ||
        (this->stateFlags1 & (PLAYER_STATE1_IN_DEATH_CUTSCENE | PLAYER_STATE1_IN_CUTSCENE)) ||
        (this->stateFlags3 & PLAYER_STATE3_80)) {
        this->unk_738 = 0;
    } else if (pressedZ || (this->stateFlags2 & PLAYER_STATE2_2000) || (this->unk_A78 != NULL)) {
        if (this->unk_738 <= 5) {
            this->unk_738 = 5;
        } else {
            this->unk_738--;
        }
    } else if (this->stateFlags1 & PLAYER_STATE1_Z_TARGETING_FRIENDLY) {
        this->unk_738 = 0;
    } else if (this->unk_738 != 0) {
        this->unk_738--;
    }

    if (this->unk_738 > 5) {
        var_v1 = 1;
    }

    temp_v0_3 = func_8082DAFC(play);
    if (temp_v0_3 || (this->unk_738 != 0) ||
        (this->stateFlags1 & (PLAYER_STATE1_CHARGING_SPIN_ATTACK | PLAYER_STATE1_2000000))) {
        if (!temp_v0_3) {
            if (!(this->stateFlags1 & PLAYER_STATE1_2000000) &&
                ((this->itemActionParam != PLAYER_AP_FISHING_ROD) || (this->stickFlameTimer == 0)) &&
                CHECK_BTN_ALL(sPlayerControlInput->press.button, BTN_Z)) {
                var_v1_2 = (this == GET_PLAYER(play)) ? play->actorCtx.targetContext.arrowPointedActor
                                                      : &GET_PLAYER(play)->actor;
                var_a1 = gSaveContext.options.zTargetSetting != 0 || this != GET_PLAYER(play);
                this->stateFlags1 |= PLAYER_STATE1_8000;
                if ((this->currentMask != PLAYER_MASK_GIANT) && (var_v1_2 != NULL) &&
                    !(var_v1_2->flags & ACTOR_FLAG_8000000) &&
                    !(this->stateFlags3 & (PLAYER_STATE3_200 | PLAYER_STATE3_2000))) {
                    if ((var_v1_2 == this->targetedActor) && (this == GET_PLAYER(play))) {
                        var_v1_2 = play->actorCtx.targetContext.unk_94;
                    }

                    if ((var_v1_2 != NULL) &&
                        (((var_v1_2 != this->targetedActor)) || (var_v1_2->flags & ACTOR_FLAG_80000))) {
                        var_v1_2->flags &= ~ACTOR_FLAG_80000;
                        if (var_a1 == 0) {
                            this->stateFlags2 |= PLAYER_STATE2_2000;
                        }
                        this->targetedActor = var_v1_2;
                        this->unk_738 = 0xF;
                        this->stateFlags2 &= ~(PLAYER_STATE2_CAN_SPEAK_OR_CHECK | PLAYER_STATE2_TATL_REQUESTING_TALK);
                    } else if (var_a1 == 0) {
                        func_80123DA4(this);
                    }
                    this->stateFlags1 &= ~PLAYER_STATE1_40000000;
                } else if (!(this->stateFlags1 & (PLAYER_STATE1_Z_TARGETING_FRIENDLY | PLAYER_STATE1_40000000)) &&
                           (func_808573A4 != this->actionFunc)) {
                    Player_SetZTargetFriendlyYaw(this);
                }
            }

            if (this->targetedActor != NULL) {
                if ((this == GET_PLAYER(play)) && (this->targetedActor != this->unk_A78) &&
                    func_800B83F8(this->targetedActor, this, var_v1)) {
                    func_80123DA4(this);
                    this->stateFlags1 |= PLAYER_STATE1_40000000;
                } else if (this->targetedActor != NULL) {
                    this->targetedActor->targetPriority = 0x28;
                }
            } else if (this->unk_A78 != NULL) {
                this->targetedActor = this->unk_A78;
            }
        }

        if ((this->targetedActor != NULL) && !(this->stateFlags3 & (PLAYER_STATE3_200 | PLAYER_STATE3_2000))) {
            this->stateFlags1 &= ~(PLAYER_STATE1_10000 | PLAYER_STATE1_Z_TARGETING_FRIENDLY);
            if ((this->stateFlags1 & PLAYER_STATE1_HOLDING_ACTOR) ||
                !CHECK_FLAG_ALL(this->targetedActor->flags, ACTOR_FLAG_1 | ACTOR_FLAG_4)) {
                this->stateFlags1 |= PLAYER_STATE1_10000;
            }
        } else if (this->stateFlags1 & PLAYER_STATE1_Z_TARGETING_FRIENDLY) {
            this->stateFlags2 &= ~PLAYER_STATE2_2000;
        } else {
            func_80123DC0(this);
        }
    } else {
        func_80123DC0(this);
    }
}

s32 Player_CalculateTargetVelocityAndYaw(PlayState* play, Player* this, f32* targetVelocity, s16* targetYaw, f32 arg4) {
    f32 baseSpeedScale;

    if ((this->attentionMode != PLAYER_ATTENTIONMODE_NONE) || func_8082DA90(play) ||
        (this->stateFlags1 & PLAYER_STATE1_1)) {
        *targetVelocity = 0.0f;
        *targetYaw = this->actor.shape.rot.y;
    } else {
        *targetVelocity = sAnalogStickDistance;
        *targetYaw = sAnalogStickAngle;
        if (arg4 != 0.0f) {
            *targetVelocity -= 20.0f;
            if (*targetVelocity < 0.0f) {
                *targetVelocity = 0.0f;
            } else {
                baseSpeedScale = 1.0f - Math_CosS(*targetVelocity * 450.0f);
                *targetVelocity = (SQ(baseSpeedScale) * 30.0f) + 7.0f;
            }
        } else {
            *targetVelocity *= 0.8f;
        }

        if (this->transformation == PLAYER_FORM_FIERCE_DEITY) {
            *targetVelocity *= 1.5f;
        }

        if (sAnalogStickDistance != 0.0f) {
            f32 slope = Math_SinS(this->angleToFloorX);
            f32 speedLimit = this->speedLimit;
            f32 var_fa1;

            if (this->shapeOffsetY != 0.0f) {
                var_fa1 = (this->targetedActor != NULL) ? 0.002f : 0.008f;

                speedLimit -= this->shapeOffsetY * var_fa1;
                speedLimit = CLAMP_MIN(speedLimit, 2.0f);
                // if (var_fa0 < 2.0f) {
                //     var_fa0 = 2.0f;
                // }
            }
            *targetVelocity = (*targetVelocity * 0.14f) - (8.0f * slope * slope);
            *targetVelocity = CLAMP(*targetVelocity, 0.0f, speedLimit);
            if (slope) {} //! FAKE
            return true;
        }
    }

    return false;
}

s32 Player_StepLinearVelocityToZero(Player* this) {
    return Math_StepToF(&this->linearVelocity, 0.0f, REG(43) / 100.0f);
}

s32 Player_GetTargetVelocityAndYaw(Player* this, f32* targetVelocity, s16* targetYaw, f32 arg3, PlayState* play) {
    if (!Player_CalculateTargetVelocityAndYaw(play, this, targetVelocity, targetYaw, arg3)) {
        *targetYaw = this->actor.shape.rot.y;

        if (this->targetedActor != NULL) {
            if ((play->actorCtx.targetContext.unk4B != 0) &&
                !(this->stateFlags2 & PLAYER_STATE2_ALWAYS_DISABLE_MOVE_ROTATION)) {
                *targetYaw = Math_Vec3f_Yaw(&this->actor.world.pos, &this->targetedActor->focus.pos);
            }
        } else if (Player_IsFriendlyZTargeting(this)) {
            *targetYaw = this->targetYaw;
        }

        return false;
    }

    *targetYaw += Camera_GetInputDirYaw(play->cameraPtrs[play->activeCamId]);
    return true;
}

/**
 * The values of following arrays are used as indices for the `sSubActions` array.
 * Each index correspond to a function which will be called sequentially until any of them return `true`.
 * Negative marks the end of the array.
 */
s8 sTargetEnemyStandStillSubActions[] = { 13, 2, 4, 9, 10, 11, 8, -7 };
s8 sFriendlyTargetingStandStillSubActions[] = { 13, 1, 2, 5, 3, 4, 9, 10, 11, 7, 8, -6 };
s8 sEndSidewalkSubActions[] = { 13, 1, 2, 3, 4, 9, 10, 11, 8, 7, -6 };
s8 sFriendlyBackwalkSubActions[] = { 13, 2, 4, 9, 10, 11, 8, -7 };
s8 sSidewalkSubActions[] = { 13, 2, 4, 9, 10, 11, 12, 8, -7 };
s8 sTurnSubActions[] = { -7 };
s8 sStandStillSubActions[] = { 0, 11, 1, 2, 3, 5, 4, 9, 8, 7, -6 };
s8 sRunSubActions[] = { 0, 11, 1, 2, 3, 12, 5, 4, 9, 8, 7, -6 };
s8 sTargetRunSubActions[] = { 13, 1, 2, 3, 12, 5, 4, 9, 10, 11, 8, 7, -6 };
s8 sEndBackwalkSubActions[] = { 10, 8, -7 };
s8 D_8085D048[] = { 0, 12, 5, 4, -14 };
s8 D_8085D050[] = { 13, 2, -4 };

s32 (*sSubActions[])(Player*, PlayState*) = {
    Player_SetupCUpBehavior,
    Player_SetupOpenDoor,
    Player_SetupGetItemOrHoldBehavior,
    Player_SetupMountHorse,
    Player_SetupSpeakOrCheck,
    func_8083DFC4,
    func_8083A114,
    func_80848570,
    func_8083A580,
    Player_SetupPutDownOrThrowActor,
    func_80839B18,
    func_8083A274,
    Player_SetupWallJumpBehavior,
    Player_SetupItemCutsceneOrFirstPerson,
    func_8083A0CC,
};

// Checks if you're allowed to perform a sub-action based on a provided array of indices into sSubActions
s32 Player_SetupSubAction(PlayState* play, Player* this, s8* subActionIndex, s32 flag) {
    if (!(this->stateFlags1 & (PLAYER_STATE1_1 | PLAYER_STATE1_IN_DEATH_CUTSCENE | PLAYER_STATE1_IN_CUTSCENE)) &&
        !func_8082DA90(play)) {
        if (flag) {
            D_80862B04 = Player_SetupCurrentUpperAction(this, play);
            if (Player_ThrowDekuNut == this->actionFunc) {
                return true;
            }
        }

        if (Player_IsShootingHookshot(this)) {
            this->lookFlags |= 0x41;
            return true;
        }

        if (!(this->stateFlags3 & PLAYER_STATE3_START_CHANGE_ITEM) &&
            (Player_StartChangeItem != this->upperActionFunc)) {
            while (*subActionIndex >= 0) {
                if (sSubActions[*subActionIndex](this, play)) {
                    return true;
                }
                subActionIndex++;
            }

            if (sSubActions[-(*subActionIndex)](this, play)) {
                return true;
            }
        }

        if (func_8083213C(this)) {
            return true;
        }
    } else if (this->stateFlags1 & PLAYER_STATE1_HOLDING_ACTOR) {
        Player_SetupCurrentUpperAction(this, play);
    }

    return false;
}

/**
 * Checks if action is interrupted within a certain number of frames from the end of the current animation
 * Returns -1 is action is not interrupted at all, 0 if interrupted by a sub-action, 1 if interrupted by the player
 * moving
 */
s32 Player_IsActionInterrupted(PlayState* play, Player* this, SkelAnime* skelAnime, f32 frame) {
    if ((skelAnime->endFrame - frame) <= skelAnime->curFrame) {
        f32 targetVelocity;
        s16 targetYaw;

        if (Player_SetupSubAction(play, this, sStandStillSubActions, 1)) {
            return PLAYER_ACTIONINTERRUPT_BY_SUB_ACTION;
        }

        if (D_80862B04 || Player_GetTargetVelocityAndYaw(this, &targetVelocity, &targetYaw, 0.018f, play)) {
            return PLAYER_ACTIONINTERRUPT_BY_MOVEMENT;
        }
    }
    return PLAYER_ACTIONINTERRUPT_NONE;
}

void Player_SetupSpinAttackActor(PlayState* play, Player* this, s32 magicCost, s32 isSwordBeam) {
    if (magicCost != 0) {
        this->unk_B08[0] = 0.0f; // spinAttackTimer
    } else {
        this->unk_B08[0] = 0.5f; // spinAttackTimer
    }

    this->stateFlags1 |= PLAYER_STATE1_CHARGING_SPIN_ATTACK;

    if ((this->actor.id == ACTOR_PLAYER) && (isSwordBeam || (this->transformation == PLAYER_FORM_HUMAN))) {
        s16 pitch = 0;
        Actor* thunder;

        if (isSwordBeam) {
            if (this->targetedActor != NULL) {
                pitch = Math_Vec3f_Pitch(&this->bodyPartsPos[PLAYER_BODYPART_WAIST], &this->targetedActor->focus.pos);
            }
            if (gSaveContext.save.playerData.magic == 0) {
                return;
            }
        }

        thunder = Actor_Spawn(&play->actorCtx, play, ACTOR_EN_M_THUNDER, this->bodyPartsPos[PLAYER_BODYPART_WAIST].x,
                              this->bodyPartsPos[PLAYER_BODYPART_WAIST].y, this->bodyPartsPos[PLAYER_BODYPART_WAIST].z,
                              pitch, 0, 0, (this->itemActionParam - PLAYER_AP_SWORD_KOKIRI) | magicCost);

        if ((thunder != NULL) && isSwordBeam) {
            Magic_Consume(play, 1, MAGIC_CONSUME_DEITY_BEAM);
            this->fpsItemShotTimer = 4;
        }
    }
}

// Check for inputs for quickspin
s32 Player_CanQuickspin(Player* this) {
    s8 stickInputsArray[4];
    s8* analogStickInput;
    s8* stickInput;
    s8 inputDiff1;
    s8 inputDiff2;
    s32 i;

    if (this->itemActionParam == PLAYER_AP_STICK) {
        return false;
    }

    analogStickInput = &this->analogStickInputs[0];
    stickInput = &stickInputsArray[0];
    // Check all four stored frames of input to see if stick distance traveled is ever less than 55.0f from the center
    for (i = 0; i < 4; i++, analogStickInput++, stickInput++) {
        if ((*stickInput = *analogStickInput) < 0) {
            return false;
        }
        // Multiply each stored stickInput by 2
        *stickInput *= 2;
    }

    // Get diff between first two frames of stick input
    inputDiff1 = stickInputsArray[0] - stickInputsArray[1];
    // Return false if the difference is too small (< ~28 degrees) for the player to be spinning the analog stick
    if (ABS_ALT(inputDiff1) < 10) {
        return false;
    }

    stickInput = &stickInputsArray[1];
    // *stickInput will be the second, then third frame of stick input in this loop
    for (i = 1; i < 3; i++, stickInput++) {
        // Get diff between current input frame and next input frame
        inputDiff2 = *stickInput - *(stickInput + 1);
        // If the difference too small, or stick has changed directions, return false
        if ((ABS_ALT(inputDiff2) < 10) || (inputDiff2 * inputDiff1 < 0)) {
            return false;
        }
    }

    return true;
}

void Player_SetupSpinAttackAnims(PlayState* play, Player* this) {
    LinkAnimationHeader* anim;

    if ((this->meleeWeaponAnimation >= PLAYER_MWA_RIGHT_SLASH_1H) &&
        (this->meleeWeaponAnimation <= PLAYER_MWA_RIGHT_COMBO_2H)) {
        anim = sSpinAttackAnims1[Player_IsHoldingTwoHandedWeapon(this)];
    } else {
        anim = sSpinAttackAnims2[Player_IsHoldingTwoHandedWeapon(this)];
    }

    Player_DeactivateMeleeWeapon(this);
    LinkAnimation_Change(play, &this->skelAnime, anim, 1.0f, 8.0f, Animation_GetLastFrame(anim), ANIMMODE_ONCE, -9.0f);
    Player_SetupSpinAttackActor(play, this, 2 << 8, false);
}

void Player_StartChargeSpinAttack(PlayState* play, Player* this) {
    Player_SetAction(play, this, Player_ChargeSpinAttack, 1);
    Player_SetupSpinAttackAnims(play, this);
}

s8 sMeleeWeaponAttackDirections[] = {
    PLAYER_MWA_STAB_1H,
    PLAYER_MWA_RIGHT_SLASH_1H,
    PLAYER_MWA_RIGHT_SLASH_1H,
    PLAYER_MWA_LEFT_SLASH_1H,
};

s8 D_8085D094[][3] = {
    { PLAYER_MWA_ZORA_PUNCH_LEFT, PLAYER_MWA_ZORA_PUNCH_COMBO, PLAYER_MWA_ZORA_PUNCH_KICK },
    { PLAYER_MWA_GORON_PUNCH_LEFT, PLAYER_MWA_GORON_PUNCH_RIGHT, PLAYER_MWA_GORON_PUNCH_BUTT },
};

PlayerMeleeWeaponAnimation func_808335F4(Player* this) {
    s32 temp_a1;
    PlayerMeleeWeaponAnimation meleeWeaponAnimation;

    temp_a1 = this->relativeAnalogStickInputs[this->inputFrameCounter];
    if ((this->transformation == PLAYER_FORM_ZORA) || (this->transformation == PLAYER_FORM_GORON)) {
        s32 requiredScopeTemp;

        // yikes
        meleeWeaponAnimation =
            ((this->transformation == PLAYER_FORM_ZORA) ? D_8085D094[0] : D_8085D094[1])[this->slashCounter];

        if (this->slashCounter != 0) {
            this->meleeWeaponAnimation = meleeWeaponAnimation;
            if (this->slashCounter >= 2) {
                this->slashCounter = -1;
            }
        }
    } else {
        if (Player_CanQuickspin(this)) {
            meleeWeaponAnimation = PLAYER_MWA_SPIN_ATTACK_1H;
        } else {
            if (temp_a1 < 0) {
                meleeWeaponAnimation =
                    Player_IsZTargeting(this) ? PLAYER_MWA_FORWARD_SLASH_1H : PLAYER_MWA_RIGHT_SLASH_1H;
            } else {
                meleeWeaponAnimation = sMeleeWeaponAttackDirections[temp_a1];
                if (meleeWeaponAnimation == PLAYER_MWA_STAB_1H) {
                    this->stateFlags2 |= PLAYER_STATE2_ENABLE_FORWARD_SLIDE_FROM_ATTACK;
                    if (!Player_IsZTargeting(this)) {
                        meleeWeaponAnimation = PLAYER_MWA_FORWARD_SLASH_1H;
                    }
                }
            }

            if (this->itemActionParam == PLAYER_AP_STICK) {
                meleeWeaponAnimation = PLAYER_MWA_FORWARD_SLASH_1H;
            }
        }

        if (Player_IsHoldingTwoHandedWeapon(this)) {
            meleeWeaponAnimation++;
        }
    }
    return meleeWeaponAnimation;
}

void Player_SetupMeleeWeaponToucherFlags(Player* this, s32 index, u32 dmgFlags, s32 damage) {
    this->meleeWeaponQuads[index].info.toucher.dmgFlags = dmgFlags;
    this->meleeWeaponQuads[index].info.toucher.damage = damage;

    if (dmgFlags == DMG_DEKU_STICK) {
        this->meleeWeaponQuads[index].info.toucherFlags = (TOUCH_ON | TOUCH_NEAREST | TOUCH_SFX_WOOD);
    } else {
        this->meleeWeaponQuads[index].info.toucherFlags = (TOUCH_ON | TOUCH_NEAREST);
    }
}

MeleeWeaponDamageInfo sMeleeWeaponDmgInfo[] = {
    /* Goron, non-sword AP         */ { DMG_GORON_PUNCH, 2, 2, 0, 0 },
    /* PLAYER_AP_SWORD_KOKIRI      */ { DMG_SWORD, 4, 8, 1, 2 },
    /* PLAYER_AP_SWORD_RAZOR       */ { DMG_SWORD, 4, 8, 2, 4 },
    /* PLAYER_AP_SWORD_GILDED      */ { DMG_SWORD, 4, 8, 3, 6 },
    /* PLAYER_AP_SWORD_GREAT_FAIRY */ { DMG_SWORD, 4, 8, 4, 8 },
    /* PLAYER_AP_STICK             */ { DMG_DEKU_STICK, 0, 0, 2, 4 },
    /* PLAYER_AP_ZORA_FINS         */ { DMG_ZORA_PUNCH, 1, 2, 0, 0 },
};

// New function in NE0: split out of Player_StartMeleeWeaponAttack to be able to call it to patch Power Crouch Stab.
void func_8083375C(Player* this, PlayerMeleeWeaponAnimation meleeWeaponAnimation) {
    MeleeWeaponDamageInfo* dmgInfo = &sMeleeWeaponDmgInfo[0];
    s32 damage;

    if (this->actor.id == ACTOR_EN_TEST3) {
        // Was Kafei originally intended to be able to punch?
        meleeWeaponAnimation = PLAYER_MWA_GORON_PUNCH_LEFT;
        this->meleeWeaponAnimation = -1;
    } else {
        //! @bug Quick Put Away Damage: Since 0 is also the "no weapon" value, producing a weapon quad without a weapon
        //! in hand, such as during Quick Put Away, produced a quad with the Goron punch properties, which does 0 damage
        //! as human.
        dmgInfo =
            &sMeleeWeaponDmgInfo[(this->transformation == PLAYER_FORM_GORON) ? 0 : Player_GetMeleeWeaponHeld(this)];
    }

    //! @bug Great Deku Sword: Presumably the dmgTransformed fields are intended for Fierce Deity, but also work for
    //! Deku if it is able to equip a sword (such as with the "0th day" glitch), giving Great Fairy's Sword damage.
    damage =
        ((meleeWeaponAnimation >= PLAYER_MWA_FLIPSLASH_START) &&
         (meleeWeaponAnimation <= PLAYER_MWA_ZORA_JUMPKICK_FINISH))
            ? ((this->transformation == PLAYER_FORM_HUMAN) ? dmgInfo->dmgHumanStrong : dmgInfo->dmgTransformedStrong)
            : ((this->transformation == PLAYER_FORM_HUMAN) ? dmgInfo->dmgHumanNormal : dmgInfo->dmgTransformedNormal);

    Player_SetupMeleeWeaponToucherFlags(this, 0, dmgInfo->dmgFlags, damage);
    Player_SetupMeleeWeaponToucherFlags(this, 1, dmgInfo->dmgFlags, damage);
}

void Player_StartMeleeWeaponAttack(PlayState* play, Player* this, PlayerMeleeWeaponAnimation meleeWeaponAnimation) {
    func_8083375C(this, meleeWeaponAnimation);
    Player_SetAction(play, this, Player_MeleeWeaponAttack, 0);
    this->genericTimer = 0;

    if ((meleeWeaponAnimation < PLAYER_MWA_FLIPSLASH_FINISH) ||
        (meleeWeaponAnimation > PLAYER_MWA_ZORA_JUMPKICK_FINISH)) {
        Player_DeactivateMeleeWeapon(this);
    }

    // Accumulate consecutive slashes to do the "third slash" types
    if ((meleeWeaponAnimation != this->meleeWeaponAnimation) || (this->slashCounter >= 3)) {
        this->slashCounter = 0;
    }

    this->slashCounter++;
    if (this->slashCounter >= 3) {
        meleeWeaponAnimation += 2;
    }

    this->meleeWeaponAnimation = meleeWeaponAnimation;
    Player_PlayAnimOnceSlowed(play, this, sMeleeAttackAnimInfo[meleeWeaponAnimation].unk_0);
    this->unk_ADC = this->skelAnime.animLength + 4.0f;

    if ((meleeWeaponAnimation < PLAYER_MWA_FLIPSLASH_START) ||
        (meleeWeaponAnimation > PLAYER_MWA_ZORA_JUMPKICK_START)) {
        Player_SetupAnimMovement(play, this, (1 | 8 | ANIM_FLAG_NOMOVE));
    }
    this->currentYaw = this->actor.shape.rot.y;
}

void Player_SetupInvincibility(Player* this, s32 invincibilityTimer) {
    if (this->invincibilityTimer >= 0) {
        this->invincibilityTimer = invincibilityTimer;
        this->damageFlashTimer = 0;
    }
}

void Player_SetupInvincibilityNoDamageFlash(Player* this, s32 invincibilityTimer) {
    if (this->invincibilityTimer > invincibilityTimer) {
        this->invincibilityTimer = invincibilityTimer;
    }
    this->damageFlashTimer = 0;
}

// Player_InflictDamageImpl?
s32 func_808339D4(PlayState* play, Player* this, s32 damage) {
    if ((this->invincibilityTimer != 0) || (this->stateFlags3 & PLAYER_STATE3_400000) ||
        (this->actor.id != ACTOR_PLAYER)) {
        return 1;
    }

    if (this->actor.category != ACTORCAT_PLAYER) {
        this->actor.colChkInfo.damage = -damage;
        return Actor_ApplyDamage(&this->actor);
    }

    if (this->currentMask == PLAYER_MASK_GIANT) {
        damage >>= 2;
    }

    return Health_ChangeBy(play, damage);
}

void func_80833A64(Player* this) {
    this->skelAnime.prevTransl = this->skelAnime.jointTable[PLAYER_LIMB_ROOT - 1];
    Player_UpdateAnimMovement(this, 1 | ANIM_FLAG_UPDATE_Y);
}

void func_80833AA0(Player* this, PlayState* play) {
    if (Player_SetAction(play, this, Player_UpdateMidair, 0)) {
        Player_AnimationPlayLoop(play, this, &gPlayerAnim_link_normal_landing_wait);
        this->genericTimer = 1;
    }
    if (this->attentionMode != PLAYER_ATTENTIONMODE_CUTSCENE) {
        this->attentionMode = PLAYER_ATTENTIONMODE_NONE;
    }
}

// TODO: this can be one array, but should it be?
LinkAnimationHeader* D_8085D0D4[] = {
    &gPlayerAnim_link_normal_front_shit,
    &gPlayerAnim_link_normal_front_shitR,
    &gPlayerAnim_link_normal_back_shit,
    &gPlayerAnim_link_normal_back_shitR,
    // };
    // LinkAnimationHeader* D_8085D0E4[] = {
    &gPlayerAnim_link_normal_front_hit,
    &gPlayerAnim_link_anchor_front_hitR,
    &gPlayerAnim_link_normal_back_hit,
    &gPlayerAnim_link_anchor_back_hitR,
};

void func_80833B18(PlayState* play, Player* this, s32 arg2, f32 speedXZ, f32 velocityY, s16 arg5,
                   s32 invincibilityTimer) {
    LinkAnimationHeader* anim = NULL;

    if (this->stateFlags1 & PLAYER_STATE1_2000) {
        func_80833A64(this);
    }

    this->unk_B64 = 0;

    Player_PlaySfx(this, NA_SE_PL_DAMAGE);

    if (func_808339D4(play, this, -this->actor.colChkInfo.damage) == 0) {
        this->stateFlags2 &= ~PLAYER_STATE2_80;
        if ((this->actor.bgCheckFlags & 1) || (this->stateFlags1 & PLAYER_STATE1_SWIMMING)) {
            return;
        }
    }

    if (this->actor.colChkInfo.damage != 0) {
        Player_SetupInvincibility(this, invincibilityTimer);
    }

    if (this->stateFlags2 & PLAYER_STATE2_10) {
        return;
    }

    if (arg2 == 3) {
        Player_SetAction(play, this, func_808546D0, 0);
        anim = &gPlayerAnim_link_normal_ice_down;
        Player_ClearAttentionModeAndStopMoving(this);
        this->actor.velocity.y = 0.0f;

        Player_RequestRumble(play, this, 255, 10, 40, SQ(0));

        Player_PlaySfx(this, NA_SE_PL_FREEZE_S);
        Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_FREEZE);
    } else if (arg2 == 4) {
        Player_SetAction(play, this, func_80854800, 0);
        func_8082DB60(play, this, &gPlayerAnim_link_normal_electric_shock);
        Player_ClearAttentionModeAndStopMoving(this);

        this->genericTimer = 20;
        this->actor.velocity.y = 0.0f;

        Player_RequestRumble(play, this, 255, 80, 150, SQ(0));
    } else {
        arg5 -= this->actor.shape.rot.y;

        if (this->stateFlags1 & PLAYER_STATE1_SWIMMING) {
            Player_SetAction(play, this, func_80851B58, 0);
            Player_RequestRumble(play, this, 180, 20, 50, SQ(0));

            if (arg2 == 1) {
                this->linearVelocity = speedXZ * 1.5f;
                this->actor.velocity.y = velocityY * 0.7f;
            } else {
                this->linearVelocity = 4.0f;
                this->actor.velocity.y = 0.0f;
            }

            Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_DAMAGE_S);
            anim = &gPlayerAnim_link_swimer_swim_hit;
        } else if ((arg2 == 1) || (arg2 == 2) || !(this->actor.bgCheckFlags & 1) ||
                   (this->stateFlags1 &
                    (PLAYER_STATE1_4 | PLAYER_STATE1_2000 | PLAYER_STATE1_4000 | PLAYER_STATE1_200000))) {
            Player_SetAction(play, this, func_8084BC64, 0);

            this->stateFlags3 |= PLAYER_STATE3_MIDAIR;

            Player_RequestRumble(play, this, 255, 20, 150, SQ(0));
            Player_ClearAttentionModeAndStopMoving(this);

            if (arg2 == 2) {
                this->genericTimer = 4;

                this->actor.speedXZ = 3.0f;
                this->linearVelocity = 3.0f;
                this->actor.velocity.y = 6.0f;

                Player_ChangeAnimOnce(play, this, D_8085BE84[PLAYER_ANIMGROUP_3][this->modelAnimType]);
                Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_DAMAGE_S);
            } else {
                this->actor.speedXZ = speedXZ;
                this->linearVelocity = speedXZ;
                this->actor.velocity.y = velocityY;

                if (ABS_ALT(arg5) > 0x4000) {
                    anim = &gPlayerAnim_link_normal_front_downA;
                } else {
                    anim = &gPlayerAnim_link_normal_back_downA;
                }
                Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_FALL_L);
            }
            this->actor.bgCheckFlags &= ~1;
        } else if ((this->linearVelocity > 4.0f) && !Player_IsUnfriendlyZTargeting(this)) {
            this->unk_B64 = 20;

            Player_RequestRumble(play, this, 120, 20, 10, SQ(0));
            Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_DAMAGE_S);

            return;
        } else {
            LinkAnimationHeader** animPtr = D_8085D0D4;

            Player_SetAction(play, this, func_8084BBF0, 0);
            func_8082FC60(this);

            if (this->actor.colChkInfo.damage < 5) {
                Player_RequestRumble(play, this, 120, 20, 10, SQ(0));
            } else {
                Player_RequestRumble(play, this, 180, 20, 100, SQ(0));
                this->linearVelocity = 23.0f;

                // animPtr = D_8085D0E4;
                animPtr += 4;
            }

            if (ABS_ALT(arg5) <= 0x4000) {
                animPtr += 2;
            }

            if (Player_IsUnfriendlyZTargeting(this)) {
                animPtr += 1;
            }

            anim = *animPtr;
            Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_DAMAGE_S);
        }

        this->actor.shape.rot.y += arg5;
        this->currentYaw = this->actor.shape.rot.y;
        this->actor.world.rot.y = this->actor.shape.rot.y;

        if (ABS_ALT(arg5) > 0x4000) {
            this->actor.shape.rot.y += 0x8000;
        }
    }

    Player_ResetAttributesAndHeldActor(play, this);

    this->stateFlags1 |= PLAYER_STATE1_4000000;

    if (anim != NULL) {
        Player_PlayAnimOnceSlowed(play, this, anim);
    }
}

s32 Player_GetHurtFloorType(BgFloorType floorType) {
    s32 hurtFloorType = floorType - 2;

    if ((hurtFloorType >= BG_FLOOR_TYPE_2 - BG_FLOOR_TYPE_2) && (hurtFloorType <= BG_FLOOR_TYPE_3 - BG_FLOOR_TYPE_2)) {
        return hurtFloorType;
    }
    return -1;
}

s32 Player_IsFloorSinkingSand(BgFloorType floorType) {
    return (floorType == BG_FLOOR_TYPE_4) || (floorType == BG_FLOOR_TYPE_7) || (floorType == BG_FLOOR_TYPE_12);
}

void Player_SetupEnterGrotto(PlayState* play, Player* this) {
    Player_SetAction(play, this, Player_EnterGrotto, 0);
    this->stateFlags1 |= PLAYER_STATE1_IN_CUTSCENE | PLAYER_STATE1_FALLING_INTO_GROTTO_OR_VOID;
}

void func_80834140(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    if (!(this->stateFlags1 & PLAYER_STATE1_IN_DEATH_CUTSCENE)) {
        Player_SetupEnterGrotto(play, this);
        if (func_8082DA90(play)) {
            this->genericTimer = -30;
        }
        this->stateFlags1 |= PLAYER_STATE1_IN_DEATH_CUTSCENE;
        LinkAnimation_Change(play, &this->skelAnime, anim, 1.0f, 0.0f, 84.0f, ANIMMODE_ONCE, -6.0f);
        this->genericVar = 1;
        this->linearVelocity = 0.0f;
    }
}

s32 Player_Burning(PlayState* play, Player* this) {
    f32 temp_fv0;
    f32 flameScale;
    f32 flameIntensity;
    s32 i;
    s32 timerStep;
    s32 spawnedFlame = false;
    s32 var_v0;
    s32 var_v1;
    u8* timerPtr = this->flameTimers;

    if ((this->transformation == PLAYER_FORM_ZORA) || (this->transformation == PLAYER_FORM_DEKU)) {
        timerStep = 0;
        if (this->actor.bgCheckFlags & 1) {
            if (this->cylinder.base.ocFlags1 & 2) {
                Math_Vec3f_Copy(&this->actor.world.pos, &this->actor.prevPos);
                this->linearVelocity = 0.0f;
            }
            func_80834140(play, this, &gPlayerAnim_link_derth_rebirth);
        }
    } else {
        if (this->transformation == PLAYER_FORM_GORON) {
            var_v1 = 20;
        } else {
            var_v1 = (s32)(this->linearVelocity * 0.4f) + 1;
        }

        if (this->stateFlags2 & PLAYER_STATE2_8) {
            var_v0 = 100;
        } else {
            var_v0 = 0;
        }

        timerStep = var_v0 + var_v1;
    }

    for (i = 0; i < PLAYER_BODYPART_MAX; i++, timerPtr++) {
        if (*timerPtr <= timerStep) {
            *timerPtr = 0;
        } else {
            spawnedFlame = true;
            *timerPtr -= timerStep;
            if (*timerPtr > 20.0f) {
                temp_fv0 = (*timerPtr - 20.0f) * 0.01f;
                flameScale = CLAMP(temp_fv0, 0.19999999f, 0.2f);
            } else {
                flameScale = *timerPtr * 0.01f;
            }

            flameIntensity = (*timerPtr - 25.0f) * 0.02f;
            flameIntensity = CLAMP(flameIntensity, 0.0f, 1.0f);
            EffectSsFireTail_SpawnFlameOnPlayer(play, flameScale, i, flameIntensity);
        }
    }

    if (spawnedFlame) {
        Player_PlaySfx(this, NA_SE_EV_TORCH - SFX_FLAG);
        if ((play->gameplayFrames % 4) == 0) {
            Player_InflictDamage(play, -1);
        }
    } else {
        this->isBurning = false;
    }

    return this->stateFlags1 & PLAYER_STATE1_IN_DEATH_CUTSCENE;
}

s32 Player_StartBurning(PlayState* play, Player* this) {
    s32 i = 0;

    while (i < ARRAY_COUNT(this->flameTimers)) {
        this->flameTimers[i] = Rand_S16Offset(0, 200);
        i++;
    }
    // for (i = 0; i < ARRAY_COUNT(this->flameTimers); i++) { this->flameTimers[i] = Rand_S16Offset(0, 200); }
    // for (i = 0; i < ARRAY_COUNT(this->flameTimers);) {
    //     this->flameTimers[i++] = Rand_S16Offset(0, 200);
    // }
    // for (i = 0; i < ARRAY_COUNT(this->flameTimers);) {
    //     this->flameTimers[i] = Rand_S16Offset(0, 200);
    //     i++;
    // }

    this->isBurning = true;
    return Player_Burning(play, this);
}

s32 func_80834534(PlayState* play, Player* this) {
    Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_FALL_L);
    return Player_StartBurning(play, this);
}

s32 func_8083456C(PlayState* play, Player* this) {
    if (this->actor.colChkInfo.acHitEffect == 1) {
        return func_80834534(play, this);
    }
    return 0;
}

void func_808345A8(Player* this) {
    if ((this->invincibilityTimer > 0) && (this->invincibilityTimer < 20)) {
        this->invincibilityTimer = 20;
    }
}

void func_808345C8(void) {
    if (INV_CONTENT(ITEM_MASK_DEKU) == ITEM_MASK_DEKU) {
        gSaveContext.save.playerForm = PLAYER_FORM_HUMAN;
        gSaveContext.save.equippedMask = PLAYER_MASK_NONE;
    }
}

s32 func_80834600(Player* this, PlayState* play) {
    s32 pad74;
    s32 var_v0;

    if (this->voidRespawnCounter != 0) {
        if (!Player_InBlockingCsMode(play, this)) {
            Player_InflictDamage(play, -16);
            this->voidRespawnCounter = 0;
        }
    } else if ((var_v0 = ((Player_GetHeight(this) - 8.0f) < (this->shapeOffsetY * this->actor.scale.y))) ||
               (this->actor.bgCheckFlags & 0x100) || (sPlayerCurrentFloorType == BG_FLOOR_TYPE_9) ||
               (this->stateFlags2 & PLAYER_STATE2_80000000)) {
        Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_DAMAGE_S);
        if (var_v0 != 0) {
            Play_TriggerRespawn(&play->state);
            func_808345C8();
            Scene_SetExitFade(play);
        } else {
            Play_TriggerVoidOut(&play->state);
            func_808345C8();
        }
        Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_TAKEN_AWAY);
        play->unk_18845 = 1;
        play_sound(NA_SE_OC_ABYSS);
    } else if ((this->unk_B75 != 0) && ((this->unk_B75 >= 3) || (this->invincibilityTimer == 0))) {
        u8 sp6C[] = { 0, 2, 1, 1 };

        if (!func_8083456C(play, this)) {
            if (this->unk_B75 == 4) {
                this->shockTimer = 40;
            }

            this->actor.colChkInfo.damage += this->unk_B74;
            func_80833B18(play, this, sp6C[this->unk_B75 - 1], this->unk_B78, this->unk_B7C, this->unk_B76, 20);
        }
    } else if ((this->shieldQuad.base.acFlags & 0x80) || (this->shieldCylinder.base.acFlags & 0x80) ||
               ((this->invincibilityTimer < 0) && (this->cylinder.base.acFlags & 2) &&
                (this->cylinder.info.acHitInfo != NULL) &&
                (this->cylinder.info.acHitInfo->toucher.dmgFlags != DMG_UNBLOCKABLE))) {
        LinkAnimationHeader* var_a2;
        s32 sp64;

        Player_RequestRumble(play, this, 180, 20, 100, SQ(0));
        if ((this->invincibilityTimer >= 0) && !Player_IsGoronOrDeku(this)) {
            sp64 = (func_8084B5C0 == this->actionFunc);
            if (!Player_IsSwimming(this)) {
                Player_SetAction(play, this, func_8084BAA4, 0);
            }

            this->genericVar = sp64;
            if ((s8)sp64 == 0) {
                Player_SetUpperActionFunc(play, this, func_80848A0C);
                var_a2 = (this->leftRightBlendWeight < 0.5f) ? D_8085CFD4[Player_IsHoldingTwoHandedWeapon(this)]
                                                             : D_8085CFCC[Player_IsHoldingTwoHandedWeapon(this)];
                LinkAnimation_PlayOnce(play, &this->skelAnimeUpper, var_a2);
            } else {
                Player_AnimationPlayOnce(play, this, D_8085CFDC[Player_IsHoldingTwoHandedWeapon(this)]);
            }
        }
        if (!(this->stateFlags1 & (PLAYER_STATE1_4 | PLAYER_STATE1_2000 | PLAYER_STATE1_4000 | PLAYER_STATE1_200000))) {
            this->linearVelocity = -18.0f;
            this->currentYaw = this->actor.shape.rot.y;
        }
        return 0;
    } else if ((this->unk_D6B != 0) || (this->invincibilityTimer > 0) || (this->stateFlags1 & PLAYER_STATE1_4000000) ||
               (this->csMode != PLAYER_CSMODE_0) || (this->meleeWeaponQuads[0].base.atFlags & 2) ||
               (this->meleeWeaponQuads[1].base.atFlags & 2) || (this->cylinder.base.atFlags & 2) ||
               (this->shieldCylinder.base.atFlags & 2)) {
        return 0;
    } else if (this->cylinder.base.acFlags & 2) {
        Actor* sp60 = this->cylinder.base.ac;
        s32 var_a2_2;

        if (sp60->flags & ACTOR_FLAG_1000000) {
            Player_PlaySfx(this, NA_SE_PL_BODY_HIT);
        }

        if (this->actor.colChkInfo.acHitEffect == 2) {
            var_a2_2 = 3;
        } else if (this->actor.colChkInfo.acHitEffect == 3) {
            var_a2_2 = 4;
        } else if (this->actor.colChkInfo.acHitEffect == 7) {
            var_a2_2 = 1;
            this->shockTimer = 40;
        } else if (this->actor.colChkInfo.acHitEffect == 9) {
            var_a2_2 = 1;
            if (func_80834534(play, this)) {
                return 1;
            }

        } else if (((this->actor.colChkInfo.acHitEffect == 4) && (this->currentMask != PLAYER_MASK_GIANT)) ||
                   (this->stateFlags3 & PLAYER_STATE3_1000)) {
            var_a2_2 = 1;
        } else {
            var_a2_2 = 0;
            if (func_8083456C(play, this)) {
                return 1;
            }
        }
        func_80833B18(play, this, var_a2_2, 4.0f, 5.0f, Actor_YawBetweenActors(sp60, &this->actor), 20);
    } else if (this->invincibilityTimer != 0) {
        return 0;
    } else {
        s32 sp58 = Player_GetHurtFloorType(sPlayerCurrentFloorType);
        u32 sp54 = SurfaceType_IsWallDamage(&play->colCtx, this->actor.floorPoly, this->actor.floorBgId);
        s32 var_a1 = 0;
        s32 var_v1_2;
        s32 pad48;

        if ((sp58 < 0) ||
            ((sp54 == 0) && (this->transformation == PLAYER_FORM_GORON) && !(this->actor.depthInWater > 0.0f))) {
            var_a1 = (this->actor.wallPoly != NULL) &&
                     SurfaceType_IsWallDamage(&play->colCtx, this->actor.wallPoly, this->actor.wallBgId);
            if (var_a1 == 0) {
                //! FAKE?
                goto label;
            }
        }
        var_v1_2 = (var_a1 != 0) ? this->actor.wallBgId : this->actor.floorBgId;
        if (((this->transformation == PLAYER_FORM_DEKU) || (this->transformation == PLAYER_FORM_ZORA)) &&
            ((sp58 >= 0) && (sp54 == 0) && !(this->stateFlags1 & PLAYER_STATE1_SWIMMING) &&
             (this->actor.bgCheckFlags & 1) && (this->actor.depthInWater < -30.0f))) {
            func_80834534(play, this);
        } else {
            this->actor.colChkInfo.damage = 4;
            func_80833B18(play, this, (var_v1_2 == 0x32) ? 0 : 1, 4.0f, 5.0f,
                          (var_a1 != 0) ? this->actor.wallYaw : this->actor.shape.rot.y, 20);
            return 1;
        label:
            return 0;
        }
    }
    return 1;
}

void Player_SetupJumpWithSfx(Player* this, f32 arg1, u16 sfxId) {
    this->actor.velocity.y = arg1 * sWaterSpeedScale;
    this->actor.bgCheckFlags &= ~1;

    if (sfxId != 0) {
        Player_AnimSfx_PlayFloorJump(this);
        Player_AnimSfx_PlayVoice(this, sfxId);
    }

    this->stateFlags1 |= PLAYER_STATE1_JUMPING;
    this->fallStartHeight = this->actor.world.pos.y;
}

void Player_SetupJumpAction(PlayState* play, Player* this, LinkAnimationHeader* anim, f32 speed, u16 sfxId) {
    Player_SetAction(play, this, Player_UpdateMidair, 1);
    if (anim != NULL) {
        Player_PlayAnimOnceSlowed(play, this, anim);
    }
    Player_SetupJumpWithSfx(this, speed, sfxId);
}

void Player_SetupJump(Player* this, LinkAnimationHeader* anim, f32 speed, PlayState* play) {
    Player_SetupJumpAction(play, this, anim, speed, NA_SE_VO_LI_SWORD_N);
}

s32 Player_SetupWallJumpBehavior(Player* this, PlayState* play) {
    if ((this->transformation != PLAYER_FORM_GORON) &&
        ((this->transformation != PLAYER_FORM_DEKU) || Player_IsSwimming(this) || (this->unk_B5C < 4)) &&
        !(this->stateFlags1 & PLAYER_STATE1_HOLDING_ACTOR) && (this->unk_B5C >= 2) &&
        (!(this->stateFlags1 & PLAYER_STATE1_SWIMMING) || (this->wallHeight < this->ageProperties->unk_14))) {
        s32 var_v1 = false;
        LinkAnimationHeader* anim;
        f32 wallHeight;

        if (Player_IsSwimming(this)) {
            f32 depth = (this->transformation == PLAYER_FORM_FIERCE_DEITY) ? 80.0f : 50.0f;

            if (this->actor.depthInWater < depth) {
                if ((this->unk_B5C < 2) || (this->ageProperties->unk_10 < this->wallHeight)) {
                    return false;
                }
            } else if ((this->currentBoots < PLAYER_BOOTS_ZORA_UNDERWATER) || (this->unk_B5C >= 3)) {
                return false;
            }
        } else if (!(this->actor.bgCheckFlags & 1) || ((this->ageProperties->unk_14 <= this->wallHeight) &&
                                                       (this->stateFlags1 & PLAYER_STATE1_SWIMMING))) {
            return false;
        }

        if ((this->actor.wallBgId != BGCHECK_SCENE) && (sPlayerCurrentWallFlags & WALL_FLAG_6)) {
            if (this->unk_B5D >= 6) {
                this->stateFlags2 |= PLAYER_STATE2_4;
                if (CHECK_BTN_ALL(sPlayerControlInput->press.button, BTN_A)) {
                    var_v1 = true;
                }
            }
        } else if ((this->unk_B5D >= 6) || CHECK_BTN_ALL(sPlayerControlInput->press.button, BTN_A)) {
            var_v1 = true;
        }

        if (var_v1) {
            Player_SetAction(play, this, Player_JumpUpToLedge, 0);
            wallHeight = this->wallHeight;

            if (this->ageProperties->unk_14 <= wallHeight) {
                anim = &gPlayerAnim_link_normal_250jump_start;
                this->linearVelocity = 1.0f;
            } else {
                CollisionPoly* poly;
                s32 bgId;
                f32 wallPolyNormalX = COLPOLY_GET_NORMAL(this->actor.wallPoly->normal.x);
                f32 wallPolyNormalZ = COLPOLY_GET_NORMAL(this->actor.wallPoly->normal.z);
                f32 var_fv1 = this->wallDistance + 0.5f; // wallDistance
                f32 yIntersect;
                s32 pad;

                this->stateFlags1 |= PLAYER_STATE1_4;

                if (Player_IsSwimming(this)) {
                    wallHeight -= 60.0f * this->ageProperties->unk_08;
                    anim = &gPlayerAnim_link_swimer_swim_15step_up;
                    this->stateFlags1 &= ~PLAYER_STATE1_SWIMMING;
                } else if (this->ageProperties->unk_18 <= wallHeight) {
                    wallHeight -= 59.0f * this->ageProperties->unk_08;
                    anim = &gPlayerAnim_link_normal_150step_up;
                } else {
                    wallHeight -= 41.0f * this->ageProperties->unk_08;
                    anim = &gPlayerAnim_link_normal_100step_up;
                }

                this->unk_ABC -= wallHeight * 100.0f;

                this->actor.world.pos.x -= var_fv1 * wallPolyNormalX;
                this->actor.world.pos.y += this->wallHeight + 10.0f;
                this->actor.world.pos.z -= var_fv1 * wallPolyNormalZ;

                yIntersect =
                    BgCheck_EntityRaycastFloor5(&play->colCtx, &poly, &bgId, &this->actor, &this->actor.world.pos);
                if ((this->actor.world.pos.y - yIntersect) <= 20.0f) {
                    this->actor.world.pos.y = yIntersect;
                    if (bgId != BGCHECK_SCENE) {
                        DynaPolyActor_SetRidingMovingStateByIndex(&play->colCtx, bgId);
                    }
                }

                Player_ClearAttentionModeAndStopMoving(this);
                this->actor.velocity.y = 0.0f;
            }

            this->actor.bgCheckFlags |= 1;
            LinkAnimation_PlayOnceSetSpeed(play, &this->skelAnime, anim, 1.3f);
            AnimationContext_DisableQueue(play);
            this->actor.shape.rot.y = this->currentYaw = this->actor.wallYaw + 0x8000;
            return true;
        }
    } else if ((this->actor.bgCheckFlags & 1) && (this->unk_B5C == 1) && (this->unk_B5D >= 3)) {
        f32 yVel = (this->wallHeight * 0.08f) + 5.5f;

        Player_SetupJump(this, &gPlayerAnim_link_normal_jump, yVel, play);
        this->linearVelocity = 2.5f;
        return true;
    }

    return false;
}

void Player_SetupMiniCsMovement(PlayState* play, Player* this, f32 arg2, s16 arg3) {
    Player_SetAction(play, this, Player_MiniCsMovement, 0);
    Player_ResetAttributes(play, this);

    this->unk_A86 = -1;
    this->genericVar = 1;
    this->genericTimer = 1;

    this->unk_3A0.x = Math_SinS(arg3) * arg2 + this->actor.world.pos.x;
    this->unk_3A0.z = Math_CosS(arg3) * arg2 + this->actor.world.pos.z;

    Player_AnimationPlayOnce(play, this, Player_GetStandingStillAnim(this));
}

void Player_SetupSwimIdle(PlayState* play, Player* this) {
    Player_SetAction(play, this, Player_UpdateSwimIdle, 0);
    Player_ChangeAnimLongMorphLoop(play, this, &gPlayerAnim_link_swimer_swim_wait);
}

s32 Player_TryEnteringGrotto(PlayState* play, Player* this) {
    if (!func_8082DA90(play) && (this->stateFlags1 & PLAYER_STATE1_FALLING_INTO_GROTTO_OR_VOID)) {
        Player_SetupEnterGrotto(play, this);
        Player_AnimationPlayLoop(play, this, &gPlayerAnim_link_normal_landing_wait);
        Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_FALL_S);
        func_8019F128(NA_SE_OC_SECRET_WARP_IN);
        return true;
    }
    return false;
}

/**
 * The actual entrances each "return entrance" value can map to.
 * This is used by scenes that are shared between locations.
 *
 * This 1D array is split into groups of entrances.
 * The start of each group is indexed by `sReturnEntranceGroupIndices` values.
 * The resulting groups are then indexed by the spawn value.
 *
 * The spawn value (`PlayState.curSpawn`) is set to a different value depending on the entrance used to enter the
 * scene, which allows these dynamic "return entrances" to link back to the previous scene.
 *
 * Seems unused in MM
 */
u16 sReturnEntranceGroupData[] = {
    // 0xFE00
    /* 0 */ 0x1000,
};

/**
 * The values are indices into `sReturnEntranceGroupData` marking the start of each group
 */
u8 sReturnEntranceGroupIndices[] = {
    0, // 0xFE00
};

// subfunction of OoT's func_80839034
void func_808354A4(PlayState* play, s32 arg1, s32 arg2) {
    play->nextEntrance = play->setupExitList[arg1];

    if (play->nextEntrance == 0xFFFF) {
        gSaveContext.respawnFlag = 4;
        play->nextEntrance = gSaveContext.respawn[RESPAWN_MODE_UNK_3].entrance;
        play->transitionType = TRANS_TYPE_03;
        gSaveContext.nextTransitionType = TRANS_TYPE_03;
    } else if (play->nextEntrance >= 0xFE00) {
        play->nextEntrance =
            sReturnEntranceGroupData[sReturnEntranceGroupIndices[play->nextEntrance - 0xFE00] + play->curSpawn];

        Scene_SetExitFade(play);
    } else {
        if (arg2) {
            gSaveContext.respawn[RESPAWN_MODE_DOWN].entrance = play->nextEntrance;
            Play_TriggerVoidOut(&play->state);
            gSaveContext.respawnFlag = -2;
        }

        gSaveContext.unk_3DBB = 1;
        Scene_SetExitFade(play);
    }

    play->transitionTrigger = TRANS_TRIGGER_START;
}

void func_808355D8(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    func_80833AA0(this, play);
    this->genericTimer = -2;
    Player_ChangeAnimSlowedOnce(play, this, anim);
    Player_PlayReactableSfx(this, NA_SE_IT_DEKUNUTS_FLOWER_CLOSE);
}

// related to grottos (?)
s32 Player_SetupExit(PlayState* play, Player* this, CollisionPoly* poly, s32 bgId) {
    u32 exitIndex;
    BgFloorType floorType;
    s32 yDistToExit;
    s32 sp30;

    if ((this == GET_PLAYER(play)) && !(this->stateFlags1 & PLAYER_STATE1_IN_DEATH_CUTSCENE) && !func_8082DA90(play) &&
        (this->csMode == PLAYER_CSMODE_0) && !(this->stateFlags1 & PLAYER_STATE1_1)) {
        exitIndex = 0;
        if (((poly != NULL) &&
             (exitIndex = SurfaceType_GetSceneExitIndex(&play->colCtx, poly, bgId), (exitIndex != 0)) &&
             (((play->sceneId != SCENE_GORONRACE) && (play->sceneId != SCENE_DEKU_KING)) || ((s32)exitIndex < 3)) &&
             (((play->sceneId != SCENE_20SICHITAI) && (play->sceneId != SCENE_20SICHITAI2)) ||
              ((s32)exitIndex < 0x15)) &&
             ((play->sceneId != SCENE_11GORONNOSATO) || ((s32)exitIndex < 6))) ||
            (Player_IsFloorSinkingSand(sPlayerCurrentFloorType) && (this->floorProperty == BG_FLOOR_PROPERTY_12))) {

            yDistToExit = this->sceneExitPosY - (s32)this->actor.world.pos.y;

            if (!(this->stateFlags1 &
                  (PLAYER_STATE1_RIDING_HORSE | PLAYER_STATE1_SWIMMING | PLAYER_STATE1_IN_CUTSCENE)) &&
                !(this->actor.bgCheckFlags & 1) && (yDistToExit < 400) && (sPlayerYDistToFloor > 100.0f)) {
                if ((this->floorProperty != BG_FLOOR_PROPERTY_5) && (this->floorProperty != BG_FLOOR_PROPERTY_12)) {
                    this->linearVelocity = 0.0f;
                }

                return false;
            } else {
                if (this->stateFlags3 & PLAYER_STATE3_1000000) {
                    func_808355D8(play, this, &gPlayerAnim_pn_kakkufinish);
                }

                if (exitIndex == 0) {
                    Play_TriggerVoidOut(&play->state);
                    Scene_SetExitFade(play);
                } else {
                    func_808354A4(play, exitIndex - 1, SurfaceType_GetSlope(&play->colCtx, poly, bgId) == 2);
                    if ((this->stateFlags1 & PLAYER_STATE1_SWIMMING) && (this->floorProperty == BG_FLOOR_PROPERTY_5)) {
                        func_8019F128(NA_SE_OC_TUNAMI);
                        func_801A4058(5);
                        gSaveContext.seqId = (u8)NA_BGM_DISABLED;
                        gSaveContext.ambienceId = 0xFF;
                    } else if (!(this->actor.bgCheckFlags & 1) && (this->floorProperty == BG_FLOOR_PROPERTY_12)) {
                        func_8019F128(NA_SE_OC_SECRET_WARP_IN);
                    }

                    if (this->stateFlags1 & PLAYER_STATE1_RIDING_HORSE) {
                        if (D_801BDAA0) {
                            D_801BDAA0 = false;
                        } else {
                            D_801BDA9C = true;
                        }
                    }
                }

                if (!(this->stateFlags1 &
                      (PLAYER_STATE1_RIDING_HORSE | PLAYER_STATE1_SWIMMING | PLAYER_STATE1_IN_CUTSCENE)) &&
                    ((floorType = SurfaceType_GetFloorType(&play->colCtx, poly, bgId)) != BG_FLOOR_TYPE_10) &&
                    ((yDistToExit < 100) || (this->actor.bgCheckFlags & 1))) {
                    if (floorType == BG_FLOOR_TYPE_11) {
                        func_8019F128(NA_SE_OC_SECRET_HOLE_OUT);
                        func_801A4058(5);
                        gSaveContext.seqId = (u8)NA_BGM_DISABLED;
                        gSaveContext.ambienceId = 0xFF;
                    } else {
                        func_8085B74C(play);
                    }
                } else if (!(this->actor.bgCheckFlags & 1)) {
                    Player_StopHorizontalMovement(this);
                }

                Camera_ChangeSetting(Play_GetCamera(play, CAM_ID_MAIN), 0x50);
                this->stateFlags1 |= PLAYER_STATE1_1 | PLAYER_STATE1_IN_CUTSCENE;
                return true;
            }
        } else {
            if ((this->stateFlags1 & PLAYER_STATE1_SWIMMING) && (this->actor.floorPoly == NULL)) {
                BgCheck_EntityRaycastFloor7(&play->colCtx, &this->actor.floorPoly, &sp30, &this->actor,
                                            &this->actor.world.pos);
                if (this->actor.floorPoly == NULL) {
                    Play_TriggerVoidOut(&play->state);
                    return false;
                }
                //! FAKE
                if (0) {}
            }

            if (!(this->stateFlags1 & PLAYER_STATE1_FALLING_INTO_GROTTO_OR_VOID)) {
                if (((this->actor.world.pos.y < -4000.0f) ||
                     (((this->floorProperty == BG_FLOOR_PROPERTY_5) || (this->floorProperty == BG_FLOOR_PROPERTY_12) ||
                       (this->floorProperty == BG_FLOOR_PROPERTY_13)) &&
                      ((sPlayerYDistToFloor < 100.0f) || (this->fallDistance > 400))))) {
                    if (this->actor.bgCheckFlags & 1) {
                        if (this->floorProperty == BG_FLOOR_PROPERTY_5) {
                            Play_TriggerRespawn(&play->state);
                            func_808345C8();
                        } else {
                            Play_TriggerVoidOut(&play->state);
                        }
                        if (!SurfaceType_IsWallDamage(&play->colCtx, this->actor.floorPoly, this->actor.floorBgId)) {
                            gSaveContext.respawnFlag = -5;
                        }

                        play->transitionType = TRANS_TYPE_04;
                        play_sound(NA_SE_OC_ABYSS);
                    } else {
                        if (this->stateFlags3 & PLAYER_STATE3_1000000) {
                            func_808355D8(play, this, &gPlayerAnim_pn_kakkufinish);
                        }

                        if (this->floorProperty == BG_FLOOR_PROPERTY_13) {
                            Player_SetAction(play, this, func_808497A0, 0);
                            this->stateFlags1 |= PLAYER_STATE1_IN_CUTSCENE;
                        } else {
                            Player_SetupEnterGrotto(play, this);
                            this->genericTimer = 9999;
                            if (this->floorProperty == BG_FLOOR_PROPERTY_5) {
                                this->genericVar = -1;
                            } else {
                                this->genericVar = 1;
                            }
                        }
                    }
                }
            }

            this->sceneExitPosY = this->actor.world.pos.y;
        }
    }

    return false;
}

void Player_TranslateAndRotateY(Player* this, Vec3f* translation, Vec3f* src, Vec3f* dst) {
    Lib_Vec3f_TranslateAndRotateY(translation, this->actor.shape.rot.y, src, dst);
}

// Player_GetPosInACertainDirectionFromARadiusAway
void Player_GetWorldPosRelativeToPlayer(Vec3f* srcPos, s16 rotY, f32 radius, Vec3f* dstPos) {
    dstPos->x = Math_SinS(rotY) * radius + srcPos->x;
    dstPos->z = Math_CosS(rotY) * radius + srcPos->z;
}

Actor* Player_SpawnFairy(PlayState* play, Player* this, Vec3f* translation, Vec3f* arg3, s32 elfParams) {
    Vec3f pos;

    Player_TranslateAndRotateY(this, translation, arg3, &pos);

    return Actor_Spawn(&play->actorCtx, play, ACTOR_EN_ELF, pos.x, pos.y, pos.z, 0, 0, 0, elfParams);
}

f32 func_80835CD8(PlayState* play, Player* this, Vec3f* arg2, Vec3f* pos, CollisionPoly** outPoly, s32* outBgId) {
    Player_TranslateAndRotateY(this, &this->actor.world.pos, arg2, pos);
    return BgCheck_EntityRaycastFloor5(&play->colCtx, outPoly, outBgId, &this->actor, pos);
}

f32 func_80835D2C(PlayState* play, Player* this, Vec3f* arg2, Vec3f* pos) {
    CollisionPoly* poly;
    s32 bgId;

    return func_80835CD8(play, this, arg2, pos, &poly, &bgId);
}

s32 Player_WallLineTestWithOffset(PlayState* play, Player* this, Vec3f* arg2, CollisionPoly** outPoly, s32* bgId,
                                  Vec3f* posResult) {
    Vec3f posA;
    Vec3f posB;

    posA.x = this->actor.world.pos.x;
    posA.y = this->actor.world.pos.y + arg2->y;
    posA.z = this->actor.world.pos.z;

    Player_TranslateAndRotateY(this, &this->actor.world.pos, arg2, &posB);

    return BgCheck_EntityLineTest2(&play->colCtx, &posA, &posB, posResult, outPoly, true, false, false, true, bgId,
                                   &this->actor);
}

Vec3f D_8085D100 = { 0.0f, 50.0f, 0.0f };

s32 func_80835DF8(PlayState* play, Player* this, CollisionPoly** outPoly, s32* outBgId) {
    Vec3f pos;
    f32 yIntersect = func_80835CD8(play, this, &D_8085D100, &pos, outPoly, outBgId);

    if ((*outBgId == BGCHECK_SCENE) && (fabsf(this->actor.world.pos.y - yIntersect) < 10.0f)) {
        func_800FAAB4(play, SurfaceType_GetLightSettingIndex(&play->colCtx, *outPoly, *outBgId));
        return true;
    }
    return false;
}

/**
 * PLAYER_DOORTYPE_STAIRCASE: DoorSpiral
 */
void Player_Door_Staircase(PlayState* play, Player* this, Actor* door) {
    static Vec3f D_8085D10C = { 20.0f, 0.0f, 20.0f };
    DoorSpiral* doorStaircase = (DoorSpiral*)door;

    this->currentYaw = doorStaircase->actor.home.rot.y + 0x8000;
    this->actor.shape.rot.y = this->currentYaw;
    if (this->linearVelocity <= 0.0f) {
        this->linearVelocity = 0.1f;
    }
    Player_SetupMiniCsMovement(play, this, 50.0f, this->actor.shape.rot.y);

    this->unk_397 = this->doorType;
    this->genericVar = 0;
    this->stateFlags1 |= PLAYER_STATE1_IN_CUTSCENE;
    Player_GetWorldPosRelativeToPlayer(&doorStaircase->actor.world.pos, doorStaircase->actor.shape.rot.y, -140.0f,
                                       &this->unk_3A0);

    D_8085D10C.x = (this->doorDirection != 0) ? -400.0f : 400.0f;
    D_8085D10C.z = 200.0f;
    Player_TranslateAndRotateY(this, &this->unk_3A0, &D_8085D10C, &this->unk_3AC);

    doorStaircase->shouldClimb = true;
    Player_ClearAttentionModeAndStopMoving(this);

    if (this->doorTimer != 0) {
        this->genericTimer = 0;
        Player_ChangeAnimMorphToLastFrame(play, this, Player_GetStandingStillAnim(this));
        this->skelAnime.endFrame = 0.0f;
    } else {
        this->linearVelocity = 0.1f;
    }

    Camera_ChangeSetting(Play_GetCamera(play, CAM_ID_MAIN), 0x50);
    this->doorBgCamIndex =
        play->doorCtx.transitionActorList[DOOR_GET_TRANSITION_ID(&doorStaircase->actor)].sides[0].bgCamDataId;
    Actor_DeactivateLens(play);
    this->floorSfxOffset = NA_SE_PL_WALK_CONCRETE - SFX_FLAG;
}

/**
 * PLAYER_DOORTYPE_SLIDING: DoorShutter, BgOpenShutter
 */
void Player_Door_Sliding(PlayState* play, Player* this, Actor* door) {
    DoorSlidingActor* doorSliding = (DoorSlidingActor*)door;
    Vec3f sp38;

    this->currentYaw = doorSliding->dyna.actor.home.rot.y;
    if (this->doorDirection > 0) {
        this->currentYaw -= 0x8000;
    }
    this->actor.shape.rot.y = this->currentYaw;
    if (this->linearVelocity <= 0.0f) {
        this->linearVelocity = 0.1f;
    }

    Player_SetupMiniCsMovement(play, this, 50.0f, this->actor.shape.rot.y);
    this->genericVar = 0;
    this->unk_397 = this->doorType;
    this->stateFlags1 |= PLAYER_STATE1_IN_CUTSCENE;
    Actor_OffsetOfPointInActorCoords(&doorSliding->dyna.actor, &sp38, &this->actor.world.pos);

    Player_GetWorldPosRelativeToPlayer(&this->actor.world.pos, doorSliding->dyna.actor.shape.rot.y,
                                       (42.0f - fabsf(sp38.z)) * this->doorDirection, &this->actor.world.pos);
    Player_GetWorldPosRelativeToPlayer(&this->actor.world.pos, doorSliding->dyna.actor.shape.rot.y,
                                       this->doorDirection * 20.0f, &this->unk_3A0);
    Player_GetWorldPosRelativeToPlayer(&this->actor.world.pos, doorSliding->dyna.actor.shape.rot.y,
                                       this->doorDirection * -120.0f, &this->unk_3AC);

    doorSliding->unk_15C = 1;
    Player_ClearAttentionModeAndStopMoving(this);

    if (this->doorTimer != 0) {
        this->genericTimer = 0;
        Player_ChangeAnimMorphToLastFrame(play, this, Player_GetStandingStillAnim(this));
        this->skelAnime.endFrame = 0.0f;
    } else {
        this->linearVelocity = 0.1f;
    }

    if (doorSliding->dyna.actor.category == ACTORCAT_DOOR) {
        this->doorBgCamIndex = play->doorCtx.transitionActorList[DOOR_GET_TRANSITION_ID(&doorSliding->dyna.actor)]
                                   .sides[this->doorDirection > 0 ? 0 : 1]
                                   .bgCamDataId;
        Actor_DeactivateLens(play);
    }
}

// sPlayerOpenDoorLeftAnimPerForm
LinkAnimationHeader* D_8085D118[] = {
    &gPlayerAnim_pg_doorA_open, // PLAYER_FORM_GORON
    &gPlayerAnim_pz_doorA_open, // PLAYER_FORM_ZORA
    &gPlayerAnim_pn_doorA_open, // PLAYER_FORM_DEKU
};
// sPlayerOpenDoorRightAnimPerForm
LinkAnimationHeader* D_8085D124[] = {
    &gPlayerAnim_pg_doorB_open, // PLAYER_FORM_GORON
    &gPlayerAnim_pz_doorB_open, // PLAYER_FORM_ZORA
    &gPlayerAnim_pn_doorB_open, // PLAYER_FORM_DEKU
};

/**
 * PLAYER_DOORTYPE_HANDLE: EnDoor
 * PLAYER_DOORTYPE_FAKE:
 * PLAYER_DOORTYPE_PROXIMITY: EnDoor
 */
void Player_Door_Default(PlayState* play, Player* this, Actor* door) {
    s32 temp = this->transformation - 1;
    LinkAnimationHeader* anim;
    f32 temp_fv0; // sp5C
    DoorHandleActor* doorHandle = (DoorHandleActor*)door;

    doorHandle->animIndex = this->transformation;

    if (this->doorDirection < 0) {
        if (this->transformation == PLAYER_FORM_FIERCE_DEITY) {
            anim = D_8085BE84[PLAYER_ANIMGROUP_8][this->modelAnimType];
        } else if (this->transformation == PLAYER_FORM_HUMAN) {
            anim = D_8085BE84[PLAYER_ANIMGROUP_9][this->modelAnimType];
        } else {
            anim = D_8085D118[temp];
        }
    } else {
        doorHandle->animIndex += 5;

        if (this->transformation == PLAYER_FORM_FIERCE_DEITY) {
            anim = D_8085BE84[PLAYER_ANIMGROUP_10][this->modelAnimType];
        } else if (this->transformation == PLAYER_FORM_HUMAN) {
            anim = D_8085BE84[PLAYER_ANIMGROUP_11][this->modelAnimType];
        } else {
            anim = D_8085D124[temp];
        }
    }

    Player_SetAction(play, this, Player_Door_Open, 0);
    this->stateFlags2 |= PLAYER_STATE2_800000;
    Player_UnequipItem(play, this);
    if (this->doorDirection < 0) {
        this->actor.shape.rot.y = doorHandle->dyna.actor.shape.rot.y;
    } else {
        this->actor.shape.rot.y = doorHandle->dyna.actor.shape.rot.y - 0x8000;
    }

    this->currentYaw = this->actor.shape.rot.y;
    temp_fv0 = this->doorDirection * 22.0f;
    Player_GetWorldPosRelativeToPlayer(&doorHandle->dyna.actor.world.pos, doorHandle->dyna.actor.shape.rot.y, temp_fv0,
                                       &this->actor.world.pos);
    Player_PlayAnimOnceWithWaterInfluence(play, this, anim);

    if (this->doorTimer != 0) {
        this->skelAnime.endFrame = 0.0f;
    }

    Player_ClearAttentionModeAndStopMoving(this);
    Player_SetupAnimMovement(play, this, 1 | ANIM_FLAG_UPDATE_Y | 4 | 8 | 0x80 | 0x200);
    doorHandle->playOpenAnim = true;
    if (this->doorType != PLAYER_DOORTYPE_FAKE) {
        CollisionPoly* poly;
        s32 bgId;
        Vec3f pos;
        s32 enDoorType = ENDOOR_GET_TYPE(&doorHandle->dyna.actor);

        this->stateFlags1 |= PLAYER_STATE1_IN_CUTSCENE;

        if (this->actor.category == ACTORCAT_PLAYER) {
            Actor_DeactivateLens(play);
            Player_GetWorldPosRelativeToPlayer(&doorHandle->dyna.actor.world.pos, doorHandle->dyna.actor.shape.rot.y,
                                               -temp_fv0, &pos);
            pos.y = doorHandle->dyna.actor.world.pos.y + 10.0f;
            BgCheck_EntityRaycastFloor5(&play->colCtx, &poly, &bgId, &this->actor, &pos);

            if (Player_SetupExit(play, this, poly, BGCHECK_SCENE)) {
                gSaveContext.entranceSpeed = 2.0f;
            } else if (enDoorType != ENDOOR_TYPE_7) {
                Camera* cam;

                this->genericVar = 38.0f * D_8085C3E8;
                cam = Play_GetCamera(play, CAM_ID_MAIN);

                Camera_ChangeDoorCam(cam, &doorHandle->dyna.actor,
                                     play->doorCtx.transitionActorList[DOOR_GET_TRANSITION_ID(&doorHandle->dyna.actor)]
                                         .sides[(this->doorDirection > 0) ? 0 : 1]
                                         .bgCamDataId,
                                     0.0f, this->genericVar, 26.0f * D_8085C3E8, 10.0f * D_8085C3E8);
            }
        }
    }
}

// door stuff
s32 Player_SetupOpenDoor(Player* this, PlayState* play) {
    if ((gSaveContext.save.playerData.health != 0) && (this->doorType != PLAYER_DOORTYPE_NONE)) {
        if ((this->actor.category != ACTORCAT_PLAYER) ||
            ((((this->doorType <= PLAYER_DOORTYPE_TALK) && ActorCutscene_GetCanPlayNext(0x7C)) ||
              ((this->doorType >= PLAYER_DOORTYPE_HANDLE) && ActorCutscene_GetCanPlayNext(0x7D))) &&
             (!(this->stateFlags1 & PLAYER_STATE1_HOLDING_ACTOR) &&
              (CHECK_BTN_ALL(sPlayerControlInput->press.button, BTN_A) ||
               (Player_SetupOpenDoorFromSpawn == this->actionFunc) || (this->doorType == PLAYER_DOORTYPE_STAIRCASE) ||
               (this->doorType == PLAYER_DOORTYPE_PROXIMITY))))) {
            Actor* doorActor = this->doorActor;
            Actor* var_v0_3;

            if (this->doorType <= PLAYER_DOORTYPE_TALK) {
                Player_TalkWithPlayer(play, doorActor);
                if (doorActor->textId == 0x1821) {
                    doorActor->flags |= ACTOR_FLAG_100;
                }
                return true;
            }

            gSaveContext.respawn[RESPAWN_MODE_DOWN].data = 0;

            if (this->doorType == PLAYER_DOORTYPE_STAIRCASE) {
                Player_Door_Staircase(play, this, doorActor);
            } else if (this->doorType == PLAYER_DOORTYPE_SLIDING) {
                Player_Door_Sliding(play, this, doorActor);
            } else {
                Player_Door_Default(play, this, doorActor);
            }

            if (this->actor.category == ACTORCAT_PLAYER) {
                this->unk_A86 = 0x7D;
                ActorCutscene_Start(this->unk_A86, &this->actor);
            }

            if (this->actor.category == ACTORCAT_PLAYER) {
                if ((this->doorType < PLAYER_DOORTYPE_FAKE) && (doorActor->category == ACTORCAT_DOOR) &&
                    ((this->doorType != PLAYER_DOORTYPE_HANDLE) || (ENDOOR_GET_TYPE(doorActor) != ENDOOR_TYPE_7))) {
                    s8 roomNum = play->doorCtx.transitionActorList[DOOR_GET_TRANSITION_ID(doorActor)]
                                     .sides[(this->doorDirection > 0) ? 0 : 1]
                                     .room;

                    if ((roomNum >= 0) && (roomNum != play->roomCtx.curRoom.num)) {
                        Room_StartRoomTransition(play, &play->roomCtx, roomNum);
                    }
                }
            }

            doorActor->room = play->roomCtx.curRoom.num;
            if (((var_v0_3 = doorActor->child) != NULL) || ((var_v0_3 = doorActor->parent) != NULL)) {
                var_v0_3->room = play->roomCtx.curRoom.num;
            }
            return true;
        }
    }

    return false;
}

void Player_SetupUnfriendlyZTargetStandStill(Player* this, PlayState* play) {
    LinkAnimationHeader* anim;

    Player_SetAction(play, this, Player_UnfriendlyZTargetStandingStill, 1);

    if (this->leftRightBlendWeight < 0.5f) {
        anim = Player_GetFightingRightAnim(this);
        this->leftRightBlendWeight = 0.0f;
    } else {
        anim = Player_GetFightingLeftAnim(this);
        this->leftRightBlendWeight = 1.0f;
    }

    this->leftRightBlendWeightTarget = this->leftRightBlendWeight;
    Player_AnimationPlayLoop(play, this, anim);
    this->currentYaw = this->actor.shape.rot.y;
}

void Player_SetupFriendlyZTargetingStandStill(Player* this, PlayState* play) {
    Player_SetAction(play, this, Player_FriendlyZTargetStandingStill, 1);
    Player_ChangeAnimMorphToLastFrame(play, this, Player_GetStandingStillAnim(this));
    this->currentYaw = this->actor.shape.rot.y;
}

void Player_SetupStandingStillType(Player* this, PlayState* play) {
    if (Player_IsUnfriendlyZTargeting(this)) {
        Player_SetupUnfriendlyZTargetStandStill(this, play);
    } else if (Player_IsFriendlyZTargeting(this)) {
        Player_SetupFriendlyZTargetingStandStill(this, play);
    } else {
        Player_SetupStandingStillMorph(this, play);
    }
}

void Player_ReturnToStandStill(Player* this, PlayState* play) {
    PlayerActionFunc actionFunc;

    if (Player_IsUnfriendlyZTargeting(this)) {
        actionFunc = Player_UnfriendlyZTargetStandingStill;
    } else if (Player_IsFriendlyZTargeting(this)) {
        actionFunc = Player_FriendlyZTargetStandingStill;
    } else {
        actionFunc = Player_StandingStill;
    }
    Player_SetAction(play, this, actionFunc, 1);
}

void Player_SetupReturnToStandStill(Player* this, PlayState* play) {
    Player_ReturnToStandStill(this, play);
    if (Player_IsUnfriendlyZTargeting(this)) {
        this->genericTimer = 1;
    }
}

void Player_SetupReturnToStandStillSetAnim(Player* this, LinkAnimationHeader* anim, PlayState* play) {
    Player_SetupReturnToStandStill(this, play);
    Player_PlayAnimOnceWithWaterInfluence(play, this, anim);
}

void func_80836AD8(PlayState* play, Player* this) {
    Player_SetAction(play, this, func_80857BE8, 0);
    this->stickFlameTimer = 0;
    this->unk_B86[1] = 0;
    this->unk_AF0[0].x = 0.0f;
    this->unk_AF0[0].y = 0.0f;
    this->unk_AF0[0].z = 0.0f;
    this->unk_B08[0] = 0.0f;
    this->unk_B08[1] = 0.0f;
    Player_PlaySfx(this, NA_SE_PL_GORON_TO_BALL);
}

void func_80836B3C(PlayState* play, Player* this, f32 arg2) {
    this->currentYaw = this->actor.shape.rot.y;
    this->actor.world.rot.y = this->actor.shape.rot.y;

    if (this->transformation == PLAYER_FORM_GORON) {
        func_80836AD8(play, this);
        LinkAnimation_Change(play, &this->skelAnime, D_8085BE84[PLAYER_ANIMGROUP_15][this->modelAnimType],
                             1.5f * sWaterSpeedScale, 0.0f, 6.0f, ANIMMODE_ONCE, 0.0f);
    } else {
        LinkAnimationHeader* anim;

        anim = D_8085BE84[PLAYER_ANIMGROUP_15][this->modelAnimType];
        Player_SetAction(play, this, Player_Rolling, 0);
        LinkAnimation_Change(play, &this->skelAnime, anim, 1.25f * sWaterSpeedScale, arg2, Animation_GetLastFrame(anim),
                             ANIMMODE_ONCE, 0.0f);
    }
}

void func_80836C70(PlayState* play, Player* this, PlayerBodyPart bodyPartIndex) {
    static Vec3f D_8085D130 = { 0, 0, 0 };
    s32 i;

    for (i = 0; i < 4; i++) {
        Vec3f velocity;

        velocity.x = randPlusMinusPoint5Scaled(4.0f);
        velocity.y = Rand_ZeroFloat(2.0f);
        velocity.z = randPlusMinusPoint5Scaled(4.0f);
        D_8085D130.y = -0.2f;
        EffectSsHahen_Spawn(play, &this->bodyPartsPos[bodyPartIndex], &velocity, &D_8085D130, 0, 10, OBJECT_LINK_NUTS,
                            16, object_link_nuts_DL_008860);
    }
}

void Player_ResetLookAngles(Player* this) {
    this->actor.focus.rot.x = 0;
    this->actor.focus.rot.z = 0;
    this->headRot.x = 0;
    this->headRot.y = 0;
    this->headRot.z = 0;
    this->upperBodyRot.x = 0;
    this->upperBodyRot.y = 0;
    this->upperBodyRot.z = 0;
    this->actor.shape.rot.y = this->actor.focus.rot.y;
    this->currentYaw = this->actor.focus.rot.y;
}

s32 func_80836DC0(PlayState* play, Player* this) {
    if ((MREG(48) != 0) || func_800C9DDC(&play->colCtx, this->actor.floorPoly, this->actor.floorBgId)) {
        Player_SetAction(play, this, func_808561B0, 0);
        this->stateFlags1 &= ~(PLAYER_STATE1_Z_TARGETING_FRIENDLY | PLAYER_STATE1_40000000);
        Player_ChangeAnimMorphToLastFrame(play, this, &gPlayerAnim_pn_attack);
        Player_StopHorizontalMovement(this);
        Player_ResetLookAngles(this);
        this->actor.shape.shadowDraw = ActorShadow_DrawCircle;
        this->unk_B48 = -2000.0f;
        this->actor.shape.shadowScale = 13.0f;
        Player_PlayReactableSfx(this, NA_SE_PL_DEKUNUTS_IN_GRD);
        return true;
    }

    return false;
}

void Player_AddQuake(PlayState* play, u16 quakeSpeed, s16 verticalMag, s16 countdown) {
    s16 quakeIndex = Quake_Add(Play_GetCamera(play, CAM_ID_MAIN), QUAKE_TYPE_3);

    Quake_SetSpeed(quakeIndex, quakeSpeed);
    Quake_SetQuakeValues(quakeIndex, verticalMag, 0, 0, 0);
    Quake_SetCountdown(quakeIndex, countdown);
}

FallImpactInfo sFallImpactInfos[] = {
    { -8, 180, 40, 100, NA_SE_VO_LI_LAND_DAMAGE_S },
    { -16, 255, 140, 150, NA_SE_VO_LI_LAND_DAMAGE_S },
};

// Player_FallAgainstTheFloor, Player_LetTheBodiesHitTheFloor, Player_ImpactFloor, Player_ProcessFallDamage,
// Player_DamageOnFloorImpact, Player_CalculateFallDamage
s32 Player_SetupFallLanding(PlayState* play, Player* this) {
    s32 fallDistance;

    if ((sPlayerCurrentFloorType == BG_FLOOR_TYPE_6) || (sPlayerCurrentFloorType == BG_FLOOR_TYPE_9) ||
        (this->csMode != PLAYER_CSMODE_0)) {
        fallDistance = 0;
    } else {
        fallDistance = this->fallDistance;
    }

    Math_StepToF(&this->linearVelocity, 0.0f, 1.0f);

    this->stateFlags1 &= ~(PLAYER_STATE1_JUMPING | PLAYER_STATE1_FREEFALLING);

    // Height enough for fall damage
    if (fallDistance >= 400) {
        s32 index;
        FallImpactInfo* entry;

        if (this->fallDistance < 800) {
            // small fall
            index = 0;
        } else {
            // big fall
            index = 1;
        }

        Player_PlaySfx(this, NA_SE_PL_BODY_HIT);

        entry = &sFallImpactInfos[index];
        Player_AnimSfx_PlayVoice(this, entry->sfxId);

        if (Player_InflictDamage(play, entry->damage)) {
            // Player's dead
            return -1;
        }

        Player_SetupInvincibility(this, 40);
        Player_AddQuake(play, 32967, 2, 30);
        Player_RequestRumble(play, this, entry->sourceIntensity, entry->decayTimer, entry->decayStep, SQ(0));

        return index + 1;
    }

    // Tiny fall, won't damage player
    if (fallDistance > 200) {
        fallDistance = fallDistance * 2;
        fallDistance = CLAMP_MAX(fallDistance, 255);

        Player_RequestRumble(play, this, fallDistance, fallDistance * 0.1f, fallDistance, SQ(0));
        if (sPlayerCurrentFloorType == BG_FLOOR_TYPE_6) {
            //! bug unreachable code: When sPlayerCurrentFloorType is equal to BG_FLOOR_TYPE_6 then fallDistance is
            //! ignored (set to zero), so the previous check based on said variable will always fail, producing this
            //! current check to always be false.
            Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_CLIMB_END);
        }
    }

    Player_AnimSfx_PlayFloorLand(this);
    return 0;
}

s32 func_808370D4(PlayState* play, Player* this) {
    if ((this->fallDistance < 800) && (this->relativeAnalogStickInputs[this->inputFrameCounter] == 0) &&
        !(this->stateFlags1 & PLAYER_STATE1_HOLDING_ACTOR)) {
        func_80836B3C(play, this, 0.0f);

        return true;
    }
    return false;
}

void func_80837134(PlayState* play, Player* this) {
    LinkAnimationHeader* anim = D_8085BE84[PLAYER_ANIMGROUP_13][this->modelAnimType];
    s32 temp_v0_2; // sp28

    this->stateFlags1 &= ~(PLAYER_STATE1_JUMPING | PLAYER_STATE1_FREEFALLING);

    if (this->transformation == PLAYER_FORM_DEKU) {
        s32 var_v1 = false;

        if ((this->skelAnime.animation == &gPlayerAnim_pn_rakkafinish) ||
            (this->skelAnime.animation == &gPlayerAnim_pn_kakkufinish)) {
            func_80836C70(play, this, PLAYER_BODYPART_L_HAND);
            func_80836C70(play, this, PLAYER_BODYPART_R_HAND);
            var_v1 = true;
        }

        if (CHECK_BTN_ALL(sPlayerControlInput->cur.button, BTN_A) && func_80836DC0(play, this)) {
            return;
        }

        if (var_v1) {
            Player_SetupReturnToStandStillSetAnim(this, anim, play);
            Player_AnimSfx_PlayFloorLand(this);
            return;
        }
    } else if (this->stateFlags2 & PLAYER_STATE2_80000) {
        if (Player_IsUnfriendlyZTargeting(this)) {
            anim = D_8085C2A4[this->genericVar].unk_8;
        } else {
            anim = D_8085C2A4[this->genericVar].unk_4;
        }
    } else if (this->skelAnime.animation == &gPlayerAnim_link_normal_run_jump) {
        anim = &gPlayerAnim_link_normal_run_jump_end;
    } else if (Player_IsUnfriendlyZTargeting(this)) {
        anim = &gPlayerAnim_link_anchor_landingR;
        func_8082FC60(this);
    } else if (this->fallDistance <= 80) {
        anim = D_8085BE84[PLAYER_ANIMGROUP_14][this->modelAnimType];
    } else if (func_808370D4(play, this)) {
        return;
    }

    temp_v0_2 = Player_SetupFallLanding(play, this);
    if (temp_v0_2 > 0) { // Player suffered damage because of this fall
        Player_SetupReturnToStandStillSetAnim(this, D_8085BE84[PLAYER_ANIMGROUP_13][this->modelAnimType], play);
        this->skelAnime.endFrame = 8.0f;

        if (temp_v0_2 == 1) {
            this->genericTimer = 0xA;
        } else {
            this->genericTimer = 0x14;
        }
    } else if (temp_v0_2 == 0) {
        Player_SetupReturnToStandStillSetAnim(this, anim, play);
    }
}

void func_808373A4(PlayState* play, Player* this) {
    Player_ChangeAnimMorphToLastFrame(play, this, &gPlayerAnim_pn_attack);
    this->unk_B10[0] = 20000.0f;
    this->unk_B10[1] = 0x30000;
    Player_PlaySfx(this, NA_SE_PL_DEKUNUTS_ATTACK);
}

s32 func_808373F8(PlayState* play, Player* this, u16 sfxId) {
    LinkAnimationHeader* anim;
    f32 speed;
    s16 yawDiff = this->currentYaw - this->actor.shape.rot.y;

    if ((IREG(66) / 100.0f) < this->linearVelocity) {
        speed = IREG(67) / 100.0f;
    } else {
        speed = (IREG(68) / 100.0f + (IREG(69) * this->linearVelocity) / 1000.0f);

        if ((this->transformation == PLAYER_FORM_DEKU) && (speed < 8.0f)) {
            speed = 8.0f;
        } else if (speed < 5.0f) {
            speed = 5.0f;
        }
    }

    if ((ABS_ALT(yawDiff) >= 0x1000) || (this->linearVelocity <= 4.0f)) {
        anim = &gPlayerAnim_link_normal_jump;
    } else {
        s32 var_v1;

        if ((this->transformation != PLAYER_FORM_DEKU) && (((sPlayerPrevFloorProperty == BG_FLOOR_PROPERTY_1)) ||
                                                           (sPlayerPrevFloorProperty == BG_FLOOR_PROPERTY_2))) {
            if (sPlayerPrevFloorProperty == BG_FLOOR_PROPERTY_1) {
                var_v1 = 4;
            } else {
                var_v1 = 5;
            }

            Player_SetupJumpAction(play, this, D_8085C2A4[var_v1].unk_0, speed,
                                   ((var_v1 == 4) ? NA_SE_VO_LI_SWORD_N : sfxId));
            this->genericTimer = -1;
            this->stateFlags2 |= PLAYER_STATE2_80000;
            this->genericVar = var_v1;
            return true;
        }
        anim = &gPlayerAnim_link_normal_run_jump;
    }

    // Deku hopping
    if (this->transformation == PLAYER_FORM_DEKU) {
        speed *= 0.3f + ((5 - this->remainingHopsCounter) * 0.18f);
        if (speed < 4.0f) {
            speed = 4.0f;
        }

        if ((this->actor.depthInWater > 0.0f) && (this->remainingHopsCounter != 0)) {
            this->actor.world.pos.y += this->actor.depthInWater;
            Player_SetupJumpAction(play, this, anim, speed, 0);
            this->genericTimer = 1;
            this->stateFlags3 |= PLAYER_STATE3_200000;
            Player_PlaySfx(this, (NA_SE_PL_DEKUNUTS_JUMP5 + 1 - this->remainingHopsCounter));
            Player_AnimSfx_PlayVoice(this, sfxId);
            this->remainingHopsCounter--;
            if (this->remainingHopsCounter == 0) {
                this->stateFlags2 |= PLAYER_STATE2_80000;
                func_808373A4(play, this);
            }

            return true;
        }

        if (this->actor.velocity.y > 0.0f) {
            sfxId = 0;
        }
    }

    Player_SetupJumpAction(play, this, anim, speed, sfxId);
    this->genericTimer = 1;

    return true;
}

s32 Player_SetupSpawnSplash(PlayState* play, Player* this, f32 arg2, s32 scale) {
    f32 yVelEnteringWater = fabsf(arg2);

    if (yVelEnteringWater > 2.0f) {
        WaterBox* waterBox;
        f32 sp34;
        Vec3f splashPos;

        Math_Vec3f_Copy(&splashPos, &this->bodyPartsPos[PLAYER_BODYPART_WAIST]);
        splashPos.y += 20.0f;
        if (WaterBox_GetSurface1(play, &play->colCtx, splashPos.x, splashPos.z, &splashPos.y, &waterBox)) {
            sp34 = splashPos.y - this->bodyPartsPos[PLAYER_BODYPART_L_FOOT].y;
            if ((sp34 > -2.0f) && (sp34 < 100.0f)) {
                EffectSsGSplash_Spawn(play, &splashPos, NULL, NULL,
                                      (yVelEnteringWater <= 10.0f) ? EFFSSGSPLASH_TYPE_0 : EFFSSGSPLASH_TYPE_1, scale);
                return true;
            }
        }
    }

    return false;
}

s32 func_8083784C(Player* this) {
    if (this->actor.velocity.y < 0.0f) {
        if ((this->actor.depthInWater > 0.0f) &&
            ((this->ageProperties->unk_2C - this->actor.depthInWater) < sPlayerYDistToFloor)) {
            if ((this->remainingHopsCounter != 0) && (gSaveContext.save.playerData.health != 0) &&
                !(this->stateFlags1 & PLAYER_STATE1_4000000)) {
                if (((this->talkActor == NULL) || !(this->talkActor->flags & ACTOR_FLAG_10000))) {
                    return true;
                }
            }
        }
    }

    return false;
}

void func_808378FC(PlayState* play, Player* this) {
    if (!Player_IsZTargetingSetupStartUnfriendly(this)) {
        this->stateFlags2 |= PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING;
    }

    if (func_8083784C(this) && func_808373F8(play, this, NA_SE_VO_LI_AUTO_JUMP)) {
        Player_SetupSpawnSplash(play, this, 20.0f, this->actor.velocity.y * 50.0f);
    }
}

s32 Player_CanHoldActor(Player* this) {
    return (this->interactRangeActor != NULL) && (this->heldActor == NULL) &&
           (this->transformation != PLAYER_FORM_DEKU);
}

void Player_SetupHoldActor(PlayState* play, Player* this) {
    if (Player_CanHoldActor(this)) {
        Actor* interactRangeActor = this->interactRangeActor;
        LinkAnimationHeader* anim;

        if ((interactRangeActor->id == ACTOR_EN_ISHI) && (ENISHI_GET_1(interactRangeActor) != 0)) {
            Player_SetAction(play, this, Player_LiftSilverBoulder, 0);
            anim = &gPlayerAnim_link_silver_carry;
        } else if (((interactRangeActor->id == ACTOR_EN_BOMBF) || (interactRangeActor->id == ACTOR_EN_KUSA) ||
                    (interactRangeActor->id == ACTOR_EN_KUSA2) || (interactRangeActor->id == ACTOR_OBJ_GRASS_CARRY)) &&
                   (Player_GetStrength() <= PLAYER_STRENGTH_DEKU)) {
            Player_SetAction(play, this, Player_FailToLiftActor, 0);
            anim = &gPlayerAnim_link_normal_nocarry_free;

            this->actor.world.pos.x =
                (Math_SinS(interactRangeActor->yawTowardsPlayer) * 20.0f) + interactRangeActor->world.pos.x;
            this->actor.world.pos.z =
                (Math_CosS(interactRangeActor->yawTowardsPlayer) * 20.0f) + interactRangeActor->world.pos.z;

            this->currentYaw = this->actor.shape.rot.y = interactRangeActor->yawTowardsPlayer + 0x8000;
        } else {
            Player_SetAction(play, this, Player_LiftActor, 0);
            anim = D_8085BE84[PLAYER_ANIMGROUP_12][this->modelAnimType];
        }

        Player_AnimationPlayOnce(play, this, anim);
    } else {
        Player_SetupStandingStillType(this, play);
        this->stateFlags1 &= ~PLAYER_STATE1_HOLDING_ACTOR;
    }
}

void Player_SetupTalkWithActor(PlayState* play, Player* this) {
    Player_SetActionFuncPreserveMoveFlags(play, this, Player_TalkWithActor, 0);

    this->exchangeItemId = PLAYER_AP_NONE;
    this->stateFlags1 |= (PLAYER_STATE1_40 | PLAYER_STATE1_IN_CUTSCENE);
    if (this->actor.textId != 0) {
        Message_StartTextbox(play, this->actor.textId, this->talkActor);
    }
    this->targetedActor = this->talkActor;
}

void Player_SetupRideHorse(PlayState* play, Player* this) {
    Player_SetActionFuncPreserveMoveFlags(play, this, Player_RideHorse, 0);
}

void func_80837BF8(PlayState* play, Player* this) {
    Player_SetAction(play, this, Player_GrabPushPullWall, 0);
}

void func_80837C20(PlayState* play, Player* this) {
    s32 sp1C = this->genericTimer;
    s32 sp18 = this->genericVar;

    Player_SetActionFuncPreserveMoveFlags(play, this, func_8084F4E8, 0);
    this->actor.velocity.y = 0.0f;

    this->genericTimer = sp1C;
    this->genericVar = sp18;
}

void Player_SetupGetItem(PlayState* play, Player* this) {
    Player_SetActionFuncPreserveMoveFlags(play, this, Player_GetItem, 0);
    this->stateFlags1 |= (PLAYER_STATE1_GETTING_ITEM | PLAYER_STATE1_IN_CUTSCENE);

    if (this->getItemId == GI_HEART_CONTAINER) {
        this->genericTimer = 20;
    } else if (this->getItemId >= GI_NONE) {
        this->genericTimer = 1;
    } else {
        this->getItemId = -this->getItemId;
    }
}

void func_80837CEC(PlayState* play, Player* this, CollisionPoly* arg2, f32 arg3, LinkAnimationHeader* anim) {
    f32 nx = COLPOLY_GET_NORMAL(arg2->normal.x);
    f32 nz = COLPOLY_GET_NORMAL(arg2->normal.z);

    Player_SetAction(play, this, func_8084F1B8, 0);
    Player_ResetAttributesAndHeldActor(play, this);
    Player_AnimationPlayOnce(play, this, anim);

    this->actor.world.pos.x -= (arg3 + 1.0f) * nx;
    this->actor.world.pos.z -= (arg3 + 1.0f) * nz;
    this->actor.shape.rot.y = Math_FAtan2F(nz, nx);
    this->currentYaw = this->actor.shape.rot.y;

    Player_ClearAttentionModeAndStopMoving(this);
    this->actor.velocity.y = 0.0f;
    Player_AnimUpdatePrevTranslRot(this);
}

s32 func_80837DEC(Player* this, PlayState* play) {
    if ((this->transformation != PLAYER_FORM_GORON) && (this->transformation != PLAYER_FORM_DEKU) &&
        (this->actor.depthInWater < -80.0f)) {
        if ((ABS_ALT(this->angleToFloorX)) < 0xAAA && (ABS_ALT(this->unk_B6E) < 0xAAA)) {
            CollisionPoly* entityPoly;
            CollisionPoly* sp90;
            s32 entityBgId;
            s32 sp88;
            Vec3f sp7C;
            Vec3f sp70;
            f32 temp_fv1_2;
            f32 entityNormalX;
            f32 entityNormalY;
            f32 entityNormalZ;
            f32 temp_fv0_2;
            f32 var_fv1;

            sp7C.x = this->actor.prevPos.x - this->actor.world.pos.x;
            sp7C.z = this->actor.prevPos.z - this->actor.world.pos.z;

            var_fv1 = sqrtf(SQ(sp7C.x) + SQ(sp7C.z));
            if (var_fv1 != 0.0f) {
                var_fv1 = 5.0f / var_fv1;
            } else {
                var_fv1 = 0.0f;
            }

            sp7C.x = this->actor.prevPos.x + (sp7C.x * var_fv1);
            sp7C.y = this->actor.world.pos.y;
            sp7C.z = this->actor.prevPos.z + (sp7C.z * var_fv1);

            if (BgCheck_EntityLineTest2(&play->colCtx, &this->actor.world.pos, &sp7C, &sp70, &entityPoly, true, false,
                                        false, true, &entityBgId, &this->actor)) {
                if (ABS_ALT(entityPoly->normal.y) < 0x258) {
                    s32 var_v1_2; // sp54

                    entityNormalX = COLPOLY_GET_NORMAL(entityPoly->normal.x);
                    entityNormalY = COLPOLY_GET_NORMAL(entityPoly->normal.y);
                    entityNormalZ = COLPOLY_GET_NORMAL(entityPoly->normal.z);

                    temp_fv0_2 = Math3D_UDistPlaneToPos(entityNormalX, entityNormalY, entityNormalZ, entityPoly->dist,
                                                        &this->actor.world.pos);

                    sp70.x = this->actor.world.pos.x - ((temp_fv0_2 + 1.0f) * entityNormalX);
                    sp70.z = this->actor.world.pos.z - ((temp_fv0_2 + 1.0f) * entityNormalZ);
                    sp70.y = this->actor.world.pos.y + 26.800001f;

                    temp_fv1_2 = this->actor.world.pos.y -
                                 BgCheck_EntityRaycastFloor5(&play->colCtx, &sp90, &sp88, &this->actor, &sp70);
                    if ((temp_fv1_2 >= -11.0f) && (temp_fv1_2 <= 0.0f)) {
                        var_v1_2 = sPlayerPrevFloorProperty == BG_FLOOR_PROPERTY_6;
                        if (!var_v1_2) {
                            if (SurfaceType_GetWallFlags(&play->colCtx, entityPoly, entityBgId) & WALL_FLAG_3) {
                                var_v1_2 = true;
                            }
                        }

                        func_80837CEC(play, this, entityPoly, temp_fv0_2,
                                      (var_v1_2) ? &gPlayerAnim_link_normal_Fclimb_startB
                                                 : &gPlayerAnim_link_normal_fall);
                        if (var_v1_2) {
                            func_80832558(play, this, func_80837C20);

                            this->actor.shape.rot.y = this->currentYaw += 0x8000;
                            this->stateFlags1 |= PLAYER_STATE1_200000;
                            Player_SetupAnimMovement(play, this,
                                                     1 | ANIM_FLAG_UPDATE_Y | 4 | 8 | ANIM_FLAG_NOMOVE | 0x80);
                            this->genericTimer = -1;
                            this->genericVar = var_v1_2;
                        } else {
                            this->stateFlags1 |= PLAYER_STATE1_2000;
                            this->stateFlags1 &= ~PLAYER_STATE1_Z_TARGETING_FRIENDLY;
                        }

                        Player_PlaySfx(this, NA_SE_PL_SLIPDOWN);
                        Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_HANG);
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

void func_808381A0(Player* this, LinkAnimationHeader* anim, PlayState* play) {
    Player_SetAction(play, this, func_8084F3DC, 0);
    LinkAnimation_PlayOnceSetSpeed(play, &this->skelAnime, anim, 1.3f);
}

Vec3f D_8085D148 = { 0.0f, 50.0f, 0.0f };

s32 func_808381F8(PlayState* play, Player* this) {
    CollisionPoly* poly;
    s32 bgId;
    Vec3f pos;
    f32 yIntersect;

    Player_TranslateAndRotateY(this, &this->actor.prevPos, &D_8085D148, &pos);

    yIntersect = BgCheck_EntityRaycastFloor5(&play->colCtx, &poly, &bgId, &this->actor, &pos);

    return fabsf(yIntersect - this->actor.world.pos.y) < 10.0f;
}

Vec3f D_8085D154 = { 0.0f, 0.0f, 100.0f };

void Player_SetupMidairBehavior(Player* this, PlayState* play) {
    s32 temp_t0; // sp64
    CollisionPoly* sp60;
    s32 sp5C;
    WaterBox* sp58;
    Vec3f sp4C;
    f32 sp48;
    f32 sp44;

    this->fallDistance = this->fallStartHeight - (s32)this->actor.world.pos.y;
    if (!(this->stateFlags1 & (PLAYER_STATE1_SWIMMING | PLAYER_STATE1_IN_CUTSCENE)) &&
        ((this->stateFlags1 & PLAYER_STATE1_FALLING_INTO_GROTTO_OR_VOID) ||
         !(this->stateFlags3 & (PLAYER_STATE3_200 | PLAYER_STATE3_2000))) &&
        !(this->actor.bgCheckFlags & 1)) {
        if (Player_TryEnteringGrotto(play, this)) {
            return;
        }

        if (sPlayerPrevFloorProperty == BG_FLOOR_PROPERTY_8) {
            this->actor.world.pos.x = this->actor.prevPos.x;
            this->actor.world.pos.z = this->actor.prevPos.z;
            return;
        }

        if ((this->stateFlags3 & PLAYER_STATE3_MIDAIR) || (this->skelAnime.moveFlags & 0x80)) {
            return;
        }

        if ((Player_UpdateMidair == this->actionFunc) || (func_8084C94C == this->actionFunc) ||
            (func_8084CA24 == this->actionFunc) || (func_80857BE8 == this->actionFunc) ||
            (func_808546D0 == this->actionFunc) || (func_80854800 == this->actionFunc)) {
            return;
        }

        if ((sPlayerPrevFloorProperty == BG_FLOOR_PROPERTY_7) || (this->meleeWeaponState != 0) ||
            ((this->skelAnime.moveFlags & 8) && func_808381F8(play, this))) {
            Math_Vec3f_Copy(&this->actor.world.pos, &this->actor.prevPos);
            if (this->linearVelocity > 0.0f) {
                Player_StopHorizontalMovement(this);
            }
            this->actor.bgCheckFlags |= 2;
            return;
        }

        temp_t0 = BINANG_SUB(this->currentYaw, this->actor.shape.rot.y);
        Player_SetAction(play, this, Player_UpdateMidair, 1);
        Player_ResetAttributes(play, this);

        this->floorSfxOffset = this->unk_D66;
        if ((this->transformation != PLAYER_FORM_GORON) &&
            ((this->transformation != PLAYER_FORM_DEKU) || (this->remainingHopsCounter != 0)) &&
            (this->actor.bgCheckFlags & 4)) {
            if (!(this->stateFlags1 & PLAYER_STATE1_SWIMMING)) {
                if ((sPlayerPrevFloorProperty != BG_FLOOR_PROPERTY_6) &&
                    (sPlayerPrevFloorProperty != BG_FLOOR_PROPERTY_9) && (sPlayerYDistToFloor > 20.0f) &&
                    (this->meleeWeaponState == 0)) {
                    if ((ABS_ALT(temp_t0) < 0x2000) && (this->linearVelocity > 3.0f)) {
                        if (!(this->stateFlags1 & PLAYER_STATE1_HOLDING_ACTOR)) {
                            if (((this->transformation == PLAYER_FORM_ZORA) &&
                                 CHECK_BTN_ALL(sPlayerControlInput->cur.button, BTN_A)) ||
                                ((sPlayerPrevFloorProperty == BG_FLOOR_PROPERTY_11) &&
                                 (this->transformation != PLAYER_FORM_GORON) &&
                                 (this->transformation != PLAYER_FORM_DEKU))) {

                                sp48 = func_80835CD8(play, this, &D_8085D154, &sp4C, &sp60, &sp5C);
                                sp44 = this->actor.world.pos.y;

                                if (WaterBox_GetSurface1(play, &play->colCtx, sp4C.x, sp4C.z, &sp44, &sp58) &&
                                    ((sp44 - sp48) > 50.0f)) {
                                    Player_SetupJump(this, &gPlayerAnim_link_normal_run_jump_water_fall, 6.0f, play);
                                    Player_SetAction(play, this, func_8084C94C, 0);
                                    return;
                                }
                            }
                        }
                        func_808373F8(play, this, NA_SE_VO_LI_AUTO_JUMP);
                        return;
                    }
                }
            }
        }

        // Checking if the ledge is tall enough for Player to hang from
        if ((sPlayerPrevFloorProperty == BG_FLOOR_PROPERTY_9) || (sPlayerYDistToFloor <= this->ageProperties->unk_34) ||
            !func_80837DEC(this, play)) {
            Player_AnimationPlayLoop(play, this, &gPlayerAnim_link_normal_landing_wait);
        }
    } else {
        this->fallStartHeight = this->actor.world.pos.y;
        this->remainingHopsCounter = 5;
    }
}

s32 Player_SetupCameraMode(PlayState* play, Player* this) {
    s32 camMode;
    Camera* camera;

    if (this->attentionMode == PLAYER_ATTENTIONMODE_AIMING) {
        if (Actor_PlayerIsAimingFpsItem(this)) {
            if (this->transformation == PLAYER_FORM_HUMAN) {
                camMode = CAM_MODE_SLINGSHOT;
            } else if (this->transformation == PLAYER_FORM_DEKU) {
                camMode = CAM_MODE_DEKUSHOOT;
            } else {
                camMode = CAM_MODE_BOWARROW;
            }
        } else {
            camMode = CAM_MODE_ZORAFIN;
        }
    } else {
        camMode = CAM_MODE_FIRSTPERSON;
    }

    camera = (this->actor.id == ACTOR_PLAYER) ? Play_GetCamera(play, CAM_ID_MAIN)
                                              : Play_GetCamera(play, ((EnTest3*)this)->unk_D8E);

    return Camera_ChangeMode(camera, camMode);
}

void func_80838760(Player* this) {
    if (this->unk_A86 >= 0) {
        ActorCutscene_Stop(this->unk_A86);
        this->unk_A86 = -1;
    }
}

s32 Player_SetupCutscene(PlayState* play, Player* this) {
    if (this->attentionMode == PLAYER_ATTENTIONMODE_CUTSCENE) {
        func_80838760(this);
        this->actor.flags &= ~ACTOR_FLAG_100;
        Player_SetAction(play, this, Player_StartCutscene, 0);
        if (this->doorBgCamIndex != 0) {
            this->stateFlags1 |= PLAYER_STATE1_IN_CUTSCENE;
        }
        Player_DeactivateMeleeWeapon(this);

        return true;
    }
    return false;
}

void Player_LoadGetItemObject(Player* this, s16 objectId) {
    s32 pad[2];

    if (objectId != OBJECT_UNSET_0) {
        this->giObjectLoading = true;
        osCreateMesgQueue(&this->giObjectLoadQueue, &this->giObjectLoadMsg, 1);
        DmaMgr_SendRequestImpl(&this->giObjectDmaRequest, this->giObjectSegment, gObjectTable[objectId].vromStart,
                               gObjectTable[objectId].vromEnd - gObjectTable[objectId].vromStart, 0,
                               &this->giObjectLoadQueue, NULL);
    }
}

LinkAnimationHeader* D_8085D160[PLAYER_FORM_MAX] = {
    &gPlayerAnim_pz_maskoffstart, &gPlayerAnim_pg_maskoffstart, &gPlayerAnim_pz_maskoffstart,
    &gPlayerAnim_pn_maskoffstart, &gPlayerAnim_cl_setmask,
};

void func_808388B8(PlayState* play, Player* this, PlayerTransformation playerForm) {
    Player_ResetAttributesAndHeldActor(play, this);
    func_80831760(play, this, func_808553F4, 0);
    Player_ChangeAnimSlowedMorphToLastFrame(play, this, D_8085D160[this->transformation]);
    gSaveContext.save.playerForm = playerForm;
    this->stateFlags1 |= PLAYER_STATE1_2;

    D_80862B50 = play->envCtx.lightSettings;
    this->actor.velocity.y = 0.0f;
    Actor_DeactivateLens(play);
}

void func_808389BC(PlayState* play, Player* this) {
    func_80831760(play, this, func_80855AF4, 0);
    Player_ChangeAnimSlowedMorphToLastFrame(play, this, &gPlayerAnim_cl_setmask);
    this->stateFlags1 |= (PLAYER_STATE1_100 | PLAYER_STATE1_IN_CUTSCENE);
    Player_ClearAttentionModeAndStopMoving(this);
}

void func_80838A20(PlayState* play, Player* this) {
    func_80831760(play, this, func_80855B9C, 0);
    Player_PlayAnimOnceSlowed(play, this, &gPlayerAnim_cl_maskoff);
    this->currentMask = PLAYER_MASK_NONE;
    this->stateFlags1 |= (PLAYER_STATE1_100 | PLAYER_STATE1_IN_CUTSCENE);
    Player_ClearAttentionModeAndStopMoving(this);
    Magic_Reset(play);
}

u8 sPlayerMass[PLAYER_FORM_MAX] = {
    100, // PLAYER_FORM_FIERCE_DEITY
    200, // PLAYER_FORM_GORON
    80,  // PLAYER_FORM_ZORA
    20,  // PLAYER_FORM_DEKU
    50,  // PLAYER_FORM_HUMAN
};

LinkAnimationHeader* sPlayerOcarinaStartAnimations[PLAYER_FORM_MAX] = {
    &gPlayerAnim_link_normal_okarina_start,
    &gPlayerAnim_pg_gakkistart,
    &gPlayerAnim_pz_gakkistart,
    &gPlayerAnim_pn_gakkistart,
    &gPlayerAnim_link_normal_okarina_start,
};
LinkAnimationHeader* sPlayerOcarinaPlayAnimations[PLAYER_FORM_MAX] = {
    &gPlayerAnim_link_normal_okarina_swing,
    &gPlayerAnim_pg_gakkiplay,
    &gPlayerAnim_pz_gakkiplay,
    &gPlayerAnim_pn_gakkiplay,
    &gPlayerAnim_link_normal_okarina_swing,
};

u8 D_8085D1A4[PLAYER_AP_MAX] = {
    GI_NONE,                // PLAYER_AP_NONE
    GI_NONE,                // PLAYER_AP_LAST_USED
    GI_NONE,                // PLAYER_AP_FISHING_ROD
    GI_SWORD_KOKIRI,        // PLAYER_AP_SWORD_KOKIRI
    GI_SWORD_RAZOR,         // PLAYER_AP_SWORD_RAZOR
    GI_SWORD_GILDED,        // PLAYER_AP_SWORD_GILDED
    GI_SWORD_GREAT_FAIRY,   // PLAYER_AP_SWORD_GREAT_FAIRY
    GI_STICKS_1,            // PLAYER_AP_STICK
    GI_SWORD_KOKIRI,        // PLAYER_AP_ZORA_FINS
    GI_QUIVER_30,           // PLAYER_AP_BOW
    GI_ARROW_FIRE,          // PLAYER_AP_BOW_FIRE
    GI_ARROW_ICE,           // PLAYER_AP_BOW_ICE
    GI_ARROW_LIGHT,         // PLAYER_AP_BOW_LIGHT
    GI_HOOKSHOT,            // PLAYER_AP_HOOKSHOT
    GI_BOMBS_1,             // PLAYER_AP_BOMB
    GI_POWDER_KEG,          // PLAYER_AP_POWDER_KEG
    GI_BOMBCHUS_10,         // PLAYER_AP_BOMBCHU
    GI_40,                  // PLAYER_AP_11
    GI_NUTS_1,              // PLAYER_AP_NUT
    GI_PICTO_BOX,           // PLAYER_AP_PICTO_BOX
    GI_OCARINA,             // PLAYER_AP_OCARINA
    GI_BOTTLE,              // PLAYER_AP_BOTTLE
    GI_FISH,                // PLAYER_AP_BOTTLE_FISH
    GI_75,                  // PLAYER_AP_BOTTLE_SPRING_WATER
    GI_ICE_TRAP,            // PLAYER_AP_BOTTLE_HOT_SPRING_WATER
    GI_ZORA_EGG,            // PLAYER_AP_BOTTLE_ZORA_EGG
    GI_GOLD_DUST,           // PLAYER_AP_BOTTLE_DEKU_PRINCESS
    GI_6C,                  // PLAYER_AP_BOTTLE_GOLD_DUST
    GI_SEAHORSE,            // PLAYER_AP_BOTTLE_1C
    GI_MUSHROOM,            // PLAYER_AP_BOTTLE_SEAHORSE
    GI_70,                  // PLAYER_AP_BOTTLE_MUSHROOM
    GI_DEKU_PRINCESS,       // PLAYER_AP_BOTTLE_HYLIAN_LOACH
    GI_BUG,                 // PLAYER_AP_BOTTLE_BUG
    GI_POE,                 // PLAYER_AP_BOTTLE_POE
    GI_BIG_POE,             // PLAYER_AP_BOTTLE_BIG_POE
    GI_POTION_RED,          // PLAYER_AP_BOTTLE_POTION_RED
    GI_POTION_BLUE,         // PLAYER_AP_BOTTLE_POTION_BLUE
    GI_POTION_GREEN,        // PLAYER_AP_BOTTLE_POTION_GREEN
    GI_MILK_HALF,           // PLAYER_AP_BOTTLE_MILK
    GI_MILK_HALF,           // PLAYER_AP_BOTTLE_MILK_HALF
    GI_CHATEAU,             // PLAYER_AP_BOTTLE_CHATEAU
    GI_FAIRY,               // PLAYER_AP_BOTTLE_FAIRY
    GI_MOON_TEAR,           // PLAYER_AP_MOON_TEAR
    GI_DEED_LAND,           // PLAYER_AP_DEED_LAND
    GI_ROOM_KEY,            // PLAYER_AP_ROOM_KEY
    GI_LETTER_TO_KAFEI,     // PLAYER_AP_LETTER_TO_KAFEI
    GI_MAGIC_BEANS,         // PLAYER_AP_MAGIC_BEANS
    GI_DEED_SWAMP,          // PLAYER_AP_DEED_SWAMP
    GI_DEED_MOUNTAIN,       // PLAYER_AP_DEED_MOUNTAIN
    GI_DEED_OCEAN,          // PLAYER_AP_DEED_OCEAN
    GI_MOON_TEAR,           // PLAYER_AP_32
    GI_LETTER_TO_MAMA,      // PLAYER_AP_LETTER_MAMA
    GI_A7,                  // PLAYER_AP_34
    GI_A8,                  // PLAYER_AP_35
    GI_PENDANT_OF_MEMORIES, // PLAYER_AP_PENDANT_OF_MEMORIES
    GI_PENDANT_OF_MEMORIES, // PLAYER_AP_37
    GI_PENDANT_OF_MEMORIES, // PLAYER_AP_38
    GI_PENDANT_OF_MEMORIES, // PLAYER_AP_39
    GI_MASK_TRUTH,          // PLAYER_AP_MASK_TRUTH
    GI_MASK_KAFEIS_MASK,    // PLAYER_AP_MASK_KAFEIS_MASK
    GI_MASK_ALL_NIGHT,      // PLAYER_AP_MASK_ALL_NIGHT
    GI_MASK_BUNNY,          // PLAYER_AP_MASK_BUNNY
    GI_MASK_KEATON,         // PLAYER_AP_MASK_KEATON
    GI_MASK_GARO,           // PLAYER_AP_MASK_GARO
    GI_MASK_ROMANI,         // PLAYER_AP_MASK_ROMANI
    GI_MASK_CIRCUS_LEADER,  // PLAYER_AP_MASK_CIRCUS_LEADER
    GI_MASK_POSTMAN,        // PLAYER_AP_MASK_POSTMAN
    GI_MASK_COUPLE,         // PLAYER_AP_MASK_COUPLE
    GI_MASK_GREAT_FAIRY,    // PLAYER_AP_MASK_GREAT_FAIRY
    GI_MASK_GIBDO,          // PLAYER_AP_MASK_GIBDO
    GI_MASK_DON_GERO,       // PLAYER_AP_MASK_DON_GERO
    GI_MASK_KAMARO,         // PLAYER_AP_MASK_KAMARO
    GI_MASK_CAPTAIN,        // PLAYER_AP_MASK_CAPTAIN
    GI_MASK_STONE,          // PLAYER_AP_MASK_STONE
    GI_MASK_BREMEN,         // PLAYER_AP_MASK_BREMEN
    GI_MASK_BLAST,          // PLAYER_AP_MASK_BLAST
    GI_MASK_SCENTS,         // PLAYER_AP_MASK_SCENTS
    GI_MASK_GIANT,          // PLAYER_AP_MASK_GIANT
    GI_MASK_FIERCE_DEITY,   // PLAYER_AP_MASK_FIERCE_DEITY
    GI_MASK_GORON,          // PLAYER_AP_MASK_GORON
    GI_MASK_ZORA,           // PLAYER_AP_MASK_ZORA
    GI_MASK_DEKU,           // PLAYER_AP_MASK_DEKU
    GI_LENS,                // PLAYER_AP_LENS
};

LinkAnimationHeader* D_8085D1F8[] = {
    &gPlayerAnim_link_normal_give_other,
    &gPlayerAnim_link_normal_take_out, // Hold up cutscene item; "this item doesn't work here"
};

// TODO: standarize
#define BOTTLE_ACTION(item) ((item)-PLAYER_AP_BOTTLE)

s32 Player_SetupItemCutsceneOrFirstPerson(Player* this, PlayState* play) {
    s32 bottleAction;

    if (this->attentionMode != PLAYER_ATTENTIONMODE_NONE) {
        if (!(this->actor.bgCheckFlags & 3) && !(this->stateFlags1 & PLAYER_STATE1_SWIMMING) &&
            !(this->stateFlags1 & PLAYER_STATE1_RIDING_HORSE) && !(this->stateFlags3 & PLAYER_STATE3_8) &&
            !(this->skelAnime.moveFlags & 8)) {
            func_80838760(this);
            func_80833AA0(this, play);
            return true;
        }
        if (!Player_SetupCutscene(play, this)) {
            if (this->attentionMode == PLAYER_UNKAA5_5) {
                if ((this->heldItemActionParam > PLAYER_AP_39) && (this->heldItemActionParam <= PLAYER_AP_MASK_DEKU)) {
                    s32 maskId = this->heldItemActionParam - PLAYER_AP_39;

                    this->prevMask = this->currentMask;
                    if ((!!(maskId == this->currentMask) != 0) || (this->heldItemActionParam < PLAYER_AP_MASK_GIANT) ||
                        ((this->heldItemActionParam == PLAYER_AP_MASK_GIANT) &&
                         (this->transformation != PLAYER_FORM_HUMAN))) {
                        if (maskId == this->currentMask) {
                            this->currentMask = PLAYER_MASK_NONE;
                        } else {
                            this->currentMask = maskId;
                        }
                        if (this->transformation == PLAYER_FORM_HUMAN) {
                            func_80838A20(play, this);
                            return true;
                        }
                        func_808388B8(play, this, PLAYER_FORM_HUMAN);
                    } else {
                        this->currentMask = maskId;
                        if ((u8)(maskId) == PLAYER_MASK_GIANT) { // u8 temp?
                            func_808389BC(play, this);
                            return true;
                        }
                        func_808388B8(play, this, this->heldItemActionParam - PLAYER_AP_MASK_FIERCE_DEITY);
                    }
                    gSaveContext.save.equippedMask = this->currentMask;
                } else if (((this->actor.flags & ACTOR_FLAG_100) == ACTOR_FLAG_100) ||
                           (this->heldItemActionParam == PLAYER_AP_PICTO_BOX) ||
                           ((this->heldItemActionParam != this->unk_B2B) &&
                            ((this->heldItemActionParam == PLAYER_AP_BOTTLE_BIG_POE) ||
                             ((this->heldItemActionParam >= PLAYER_AP_BOTTLE_ZORA_EGG) &&
                              (this->heldItemActionParam <= PLAYER_AP_BOTTLE_HYLIAN_LOACH)) ||
                             (this->heldItemActionParam > PLAYER_AP_BOTTLE_FAIRY) ||
                             ((this->talkActor != NULL) && (this->exchangeItemId > 0) &&
                              (((this->exchangeItemId == PLAYER_AP_MAGIC_BEANS) &&
                                (this->heldItemActionParam == PLAYER_AP_MAGIC_BEANS)) ||
                               ((this->exchangeItemId != PLAYER_AP_MAGIC_BEANS) &&
                                (Player_ActionToBottle(this, this->heldItemActionParam) > -1))))))) {
                    Actor* talkActor;
                    s32 heldItemTemp = this->heldItemActionParam;

                    func_80838760(this);
                    this->heldItemActionParam = PLAYER_AP_NONE;
                    func_80831760(play, this, Player_PresentExchangeItem, 0);
                    talkActor = this->talkActor;
                    this->heldItemActionParam = heldItemTemp;
                    this->unk_A86 = -1;

                    if ((talkActor != NULL) && (((this->exchangeItemId == PLAYER_AP_MAGIC_BEANS) &&
                                                 (this->heldItemActionParam == PLAYER_AP_MAGIC_BEANS)) ||
                                                ((this->exchangeItemId != PLAYER_AP_MAGIC_BEANS) &&
                                                 (this->exchangeItemId > PLAYER_AP_NONE)))) {
                        this->stateFlags1 |= (PLAYER_STATE1_IN_CUTSCENE | PLAYER_STATE1_40);
                        if (this->exchangeItemId == PLAYER_AP_MAGIC_BEANS) {
                            Inventory_ChangeAmmo(ITEM_MAGIC_BEANS, -1);
                            func_80831760(play, this, func_8084B4A8, 0);
                            this->currentYaw = talkActor->yawTowardsPlayer + 0x8000;
                            this->actor.shape.rot.y = this->currentYaw;
                            if (talkActor->xzDistToPlayer < 40.0f) {
                                Player_PlayAnimOnceSlowed(play, this, &gPlayerAnim_link_normal_backspace);
                                Player_SetupAnimMovement(play, this, 1 | 8 | ANIM_FLAG_NOMOVE);
                            } else {
                                Player_ChangeAnimMorphToLastFrame(play, this, D_8085BE84[31][this->modelAnimType]);
                            }
                            this->stateFlags1 |= PLAYER_STATE1_IN_CUTSCENE;
                            this->genericTimer = 80;
                            this->genericVar = -1;
                            this->targetedActor = this->talkActor;
                        } else {
                            this->unk_A86 = 0x7C;
                        }

                        talkActor->flags |= ACTOR_FLAG_100;
                        this->actor.textId = 0;
                        this->targetedActor = this->talkActor;
                    } else {
                        this->stateFlags1 |=
                            (PLAYER_STATE1_IN_CUTSCENE | PLAYER_STATE1_SKIP_OTHER_ACTORS_UPDATE | PLAYER_STATE1_40);
                        this->unk_A86 = play->playerActorCsIds[3];
                        this->genericVar = 1;
                        this->actor.textId = 0xFE;
                    }
                    this->actor.flags |= ACTOR_FLAG_100;
                    this->exchangeItemId = this->heldItemActionParam;
                    if (this->genericVar >= 0) {
                        Player_AnimationPlayOnce(play, this, D_8085D1F8[this->genericVar]);
                    }
                    Player_ClearAttentionModeAndStopMoving(this);
                    return true;
                } else {
                    bottleAction = Player_ActionToBottle(this, this->heldItemActionParam);

                    if (bottleAction > -1) {
                        func_80838760(this);
                        if (bottleAction >= BOTTLE_ACTION(PLAYER_AP_BOTTLE_FAIRY)) {
                            func_80831760(play, this, Player_HealWithFairy, 0);
                            Player_PlayAnimOnceSlowed(play, this, &gPlayerAnim_link_bottle_bug_out);
                        } else if ((bottleAction > BOTTLE_ACTION(PLAYER_AP_BOTTLE)) &&
                                   (bottleAction < BOTTLE_ACTION(PLAYER_AP_BOTTLE_POE))) {
                            func_80831760(play, this, Player_DropItemFromBottle, 0);
                            Player_PlayAnimOnceSlowed(play, this, &gPlayerAnim_link_bottle_fish_out);
                            this->unk_A86 = play->playerActorCsIds[2];
                        } else {
                            func_80831760(play, this, func_80853194, 0);
                            Player_ChangeAnimSlowedMorphToLastFrame(play, this,
                                                                    (this->transformation == PLAYER_FORM_DEKU)
                                                                        ? &gPlayerAnim_pn_drinkstart
                                                                        : &gPlayerAnim_link_bottle_drink_demo_start);
                        }
                    } else {
                        Actor* ocarinaActor = this->ocarinaActor;

                        if ((ocarinaActor == NULL) || (ocarinaActor->id == ACTOR_EN_ZOT) ||
                            (ocarinaActor->cutscene == -1)) {
                            if (!func_808323C0(this, play->playerActorCsIds[0])) {
                                return false;
                            }
                        } else {
                            this->unk_A86 = -1;
                        }
                        func_80831760(play, this, Player_PlayOcarina, 0);
                        if ((this->skelAnime.playSpeed < 0.0f) ||
                            ((this->skelAnime.animation != sPlayerOcarinaStartAnimations[this->transformation]) &&
                             (this->skelAnime.animation != sPlayerOcarinaPlayAnimations[this->transformation]))) {
                            Player_PlayAnimOnceSlowed(play, this, sPlayerOcarinaStartAnimations[this->transformation]);
                        }
                        this->stateFlags2 |= PLAYER_STATE2_OCARINA_ON;
                        if (ocarinaActor != NULL) {
                            this->actor.flags |= ACTOR_FLAG_OCARINA_READY;
                            if (ocarinaActor->id == ACTOR_EN_ZOT) {
                                this->ocarinaActorXZDist = -1.0f;
                            } else {
                                ocarinaActor->flags |= ACTOR_FLAG_OCARINA_READY;
                            }
                        }
                    }
                }
            } else {
                if (Player_SetupCameraMode(play, this) != CAM_MODE_NORMAL) {
                    func_80838760(this);
                    if (!(this->stateFlags1 & PLAYER_STATE1_RIDING_HORSE)) {
                        Player_SetAction(play, this, func_8084E724, 1);
                        this->genericTimer = 13;
                        Player_ResetLookAngles(this);
                        if (this->attentionMode == PLAYER_UNKAA5_2) {
                            play->actorCtx.flags |= ACTORCTX_FLAG_PICTOGRAPH_ON;
                        }
                    }
                    this->stateFlags1 |= PLAYER_STATE1_100000;
                    play_sound(NA_SE_SY_CAMERA_ZOOM_UP);
                    Player_StopHorizontalMovement(this);
                    return true;
                }
                this->attentionMode = PLAYER_ATTENTIONMODE_NONE;
                play_sound(NA_SE_SY_ERROR);
                return false;
            }
            this->stateFlags1 |= (PLAYER_STATE1_IN_CUTSCENE | PLAYER_STATE1_SKIP_OTHER_ACTORS_UPDATE);
            Player_ClearAttentionModeAndStopMoving(this);
        }
        return true;
    }
    return false;
}

s32 Player_SetupSpeakOrCheck(Player* this, PlayState* play) {
    if (gSaveContext.save.playerData.health != 0) {
        Actor* talkActor = this->talkActor;
        Actor* targetedActor = this->targetedActor;
        Actor* tatlActor = NULL;
        s32 tatlHasText = false;
        s32 targetActorHasText = false;

        if (this->tatlActor != NULL) {
            targetActorHasText =
                (targetedActor != NULL) && (CHECK_FLAG_ALL(targetedActor->flags, ACTOR_FLAG_1 | ACTOR_FLAG_40000) ||
                                            (targetedActor->hintId != 0xFF));

            if (targetActorHasText || (this->tatlTextId != 0)) {
                //! @bug ?
                tatlHasText = (this->tatlTextId < 0) && ((ABS_ALT(this->tatlTextId) & 0xFF00) != 0x10000);

                if (tatlHasText || !targetActorHasText) {
                    tatlActor = this->tatlActor;
                    if (tatlHasText) {
                        targetedActor = NULL;
                        talkActor = NULL;
                    }
                } else {
                    tatlActor = targetedActor;
                }
            }
        }

        if ((talkActor != NULL) || (tatlActor != NULL)) {
            if ((targetedActor == NULL) || (targetedActor == talkActor) || (targetedActor == tatlActor)) {
                if (!(this->stateFlags1 & PLAYER_STATE1_HOLDING_ACTOR) ||
                    ((this->heldActor != NULL) &&
                     (tatlHasText || (talkActor == this->heldActor) || (tatlActor == this->heldActor) ||
                      ((talkActor != NULL) && (talkActor->flags & ACTOR_FLAG_10000))))) {
                    if (((this->actor.bgCheckFlags & 1) || (this->stateFlags1 & PLAYER_STATE1_RIDING_HORSE) ||
                         Player_IsSwimming(this))) {
                        if (talkActor != NULL) {
                            if ((targetedActor == NULL) || (targetedActor == talkActor)) {
                                this->stateFlags2 |= PLAYER_STATE2_CAN_SPEAK_OR_CHECK;
                            }

                            if (!ActorCutscene_GetCanPlayNext(0x7C)) {
                                return false;
                            }

                            if (CHECK_BTN_ALL(sPlayerControlInput->press.button, BTN_A) ||
                                (talkActor->flags & ACTOR_FLAG_10000)) {
                                tatlActor = NULL;
                            } else if (tatlActor == NULL) {
                                return false;
                            }
                        }

                        if (tatlActor != NULL) {
                            if (!tatlHasText) {
                                this->stateFlags2 |= PLAYER_STATE2_TATL_REQUESTING_TALK;
                                if (!ActorCutscene_GetCanPlayNext(0x7C) ||
                                    !CHECK_BTN_ALL(sPlayerControlInput->press.button, BTN_CUP)) {
                                    return false;
                                }
                            }

                            talkActor = tatlActor;
                            this->talkActor = NULL;

                            if (tatlHasText || !targetActorHasText) {
                                tatlActor->textId = ABS_ALT(this->tatlTextId);
                            } else if (tatlActor->hintId != 0xFF) {
                                tatlActor->textId = tatlActor->hintId + 0x1900;
                            }
                        }

                        this->currentMask = sCurrentMask;
                        gSaveContext.save.equippedMask = this->currentMask;
                        Player_TalkWithPlayer(play, talkActor);
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

s32 Player_SetupCUpBehavior(Player* this, PlayState* play) {
    if (this->attentionMode != PLAYER_ATTENTIONMODE_NONE) {
        Player_SetupItemCutsceneOrFirstPerson(this, play);
        return true;
    }

    if ((this->targetedActor != NULL) && (CHECK_FLAG_ALL(this->targetedActor->flags, ACTOR_FLAG_1 | ACTOR_FLAG_40000) ||
                                          (this->targetedActor->hintId != 0xFF))) {
        this->stateFlags2 |= PLAYER_STATE2_TATL_REQUESTING_TALK;
    } else if ((this->tatlTextId == 0) && !Player_IsUnfriendlyZTargeting(this) &&
               CHECK_BTN_ALL(sPlayerControlInput->press.button, BTN_CUP) &&
               !func_80831814(this, play, PLAYER_ATTENTIONMODE_C_UP)) {
        play_sound(NA_SE_SY_ERROR);
    }

    return false;
}

// Jumpslash/Jumpkick start
void Player_SetupJumpSlash(PlayState* play, Player* this, PlayerMeleeWeaponAnimation meleeWeaponAnimation,
                           f32 linearVelocity, f32 yVelocity) {
    if (this->transformation == PLAYER_FORM_ZORA) {
        linearVelocity *= 1.1f;
        meleeWeaponAnimation = PLAYER_MWA_ZORA_JUMPKICK_START;
        yVelocity *= 0.9f;
    }

    Player_StartMeleeWeaponAttack(play, this, meleeWeaponAnimation);
    Player_SetAction(play, this, Player_JumpSlash, 0);
    this->stateFlags3 |= PLAYER_STATE3_MIDAIR;
    this->linearVelocity = linearVelocity;
    this->currentYaw = this->actor.shape.rot.y;
    this->actor.velocity.y = yVelocity;
    this->actor.bgCheckFlags &= ~1;
    Player_AnimSfx_PlayFloorJump(this);
    Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_SWORD_L);
}

s32 func_808396B8(PlayState* play, Player* this) {
    if (!(this->stateFlags1 & PLAYER_STATE1_400000) &&
        (((this->actor.id != ACTOR_PLAYER) && CHECK_BTN_ALL(sPlayerControlInput->press.button, BTN_B)) ||
         ((Player_GetMeleeWeaponHeld(this) != 0) &&
          ((this->transformation != PLAYER_FORM_GORON) || (this->actor.bgCheckFlags & 1)) &&
          ((this->transformation != PLAYER_FORM_ZORA) || !(this->stateFlags1 & PLAYER_STATE1_2000000)) &&
          (sUsingItemAlreadyInHand != 0)))) {
        return true;
    }
    return false;
}

s32 func_80839770(Player* this, PlayState* play) {
    if (func_808396B8(play, this)) {
        if ((this->transformation != PLAYER_FORM_GORON) && (sPlayerCurrentFloorType != BG_FLOOR_TYPE_7)) {
            Player_SetupJumpSlash(play, this,
                                  (this->transformation == PLAYER_FORM_ZORA) ? PLAYER_MWA_ZORA_JUMPKICK_START
                                                                             : PLAYER_MWA_JUMPSLASH_START,
                                  3.0f, 4.5f);
            return true;
        }
    }
    return false;
}

s32 func_80839800(Player* this, PlayState* play) {
    if ((this->relativeAnalogStickInputs[this->inputFrameCounter] == 0) &&
        (sPlayerCurrentFloorType != BG_FLOOR_TYPE_7)) {
        func_80836B3C(play, this, 0.0f);
        return true;
    }
    return false;
}

void func_80839860(Player* this, PlayState* play, s32 arg2) {
    s32 pad;
    f32 speed = (!(arg2 & 1) ? 5.8f : 3.5f);

    if (this->currentBoots == PLAYER_BOOTS_GIANT) {
        speed /= 2.0f;
    }

    //! FAKE
    if (arg2 == 2) {}

    Player_SetupJumpAction(play, this, D_8085C2A4[arg2].unk_0, speed, NA_SE_VO_LI_SWORD_N);

    this->genericTimer = 1;
    this->genericVar = arg2;

    this->currentYaw = this->actor.shape.rot.y + (arg2 << 0xE);
    this->linearVelocity = !(arg2 & 1) ? 6.0f : 8.5f;

    this->stateFlags2 |= PLAYER_STATE2_80000;
    Player_PlaySfx(this, ((arg2 << 0xE) == 0x8000) ? NA_SE_PL_ROLL : NA_SE_PL_SKIP);
}

void Player_StartBremenMarch(PlayState* play, Player* this) {
    if (this->actor.bgCheckFlags & 1) {
        this->heldItemActionParam = PLAYER_AP_OCARINA;
        func_80831760(play, this, func_8084AC84, 0);
        func_8082DB60(play, this, &gPlayerAnim_clink_normal_okarina_walk);
        Player_SetupAnimMovement(play, this, (4 | 0x200));
        this->stateFlags3 |= PLAYER_STATE3_BREMEN_MARCH;
        this->unk_B48 = this->linearVelocity;
        Audio_PlayFanfare(NA_BGM_BREMEN_MARCH);
    }
}

void Player_StartKamaroDance(PlayState* play, Player* this) {
    if (this->actor.bgCheckFlags & 1) {
        this->heldItemActionParam = PLAYER_AP_NONE;
        func_80831760(play, this, func_8084AEEC, 0);
        func_8082DB60(play, this, &gPlayerAnim_alink_dance_loop);
        this->stateFlags2 |= PLAYER_STATE2_KAMARO_DANCE;
        Audio_PlayFanfare(NA_BGM_KAMARO_DANCE);
    }
}

s32 func_80839A84(PlayState* play, Player* this) {
    if (this->transformation == PLAYER_FORM_DEKU) {
        if (func_80836DC0(play, this)) {
            return 1;
        }
    } else {
        return 0;
    }

    Player_SetAction(play, this, func_808573A4, 0);
    this->stateFlags1 &= ~(PLAYER_STATE1_Z_TARGETING_FRIENDLY | PLAYER_STATE1_40000000);
    this->unk_ADC = 4;
    func_808373A4(play, this);
    return 1;
}

s32 func_80839B18(Player* this, PlayState* play) {
    if (CHECK_BTN_ALL(sPlayerControlInput->press.button, BTN_A) && (play->roomCtx.curRoom.unk3 != 2) &&
        (sPlayerCurrentFloorType != BG_FLOOR_TYPE_7) && (D_80862B40 != 1)) {
        s32 temp_a2 = this->relativeAnalogStickInputs[this->inputFrameCounter];

        if (temp_a2 <= 0) {
            if (Player_IsZTargeting(this)) {
                if (this->actor.category != ACTORCAT_PLAYER) {
                    if (temp_a2 < 0) {
                        Player_SetupJump(this, &gPlayerAnim_link_normal_jump, REG(69) / 100.0f, play);
                    } else {
                        func_80836B3C(play, this, 0.0f);
                    }
                } else if (!(this->stateFlags1 & PLAYER_STATE1_SWIMMING) && (Player_GetMeleeWeaponHeld(this) != 0) &&
                           func_80832090(this) && (this->transformation != PLAYER_FORM_GORON)) {
                    Player_SetupJumpSlash(play, this, PLAYER_MWA_JUMPSLASH_START, 5.0f, 5.0f);
                } else if (func_80839A84(play, this) == 0) {
                    func_80836B3C(play, this, 0.0f);
                }

                return true;
            }
        } else {
            func_80839860(this, play, temp_a2);
            return true;
        }
    }

    return false;
}

void func_80839CD8(Player* this, PlayState* play) {
    LinkAnimationHeader* anim;
    f32 var_fv0 = this->walkFrame - 3.0f;

    if (var_fv0 < 0.0f) {
        var_fv0 += 29.0f;
    }

    if (var_fv0 < 14.0f) {
        anim = D_8085BE84[PLAYER_ANIMGROUP_17][this->modelAnimType];
        var_fv0 = 11.0f - var_fv0;
        if (var_fv0 < 0.0f) {
            var_fv0 = -var_fv0 * 1.375f;
        }
        var_fv0 = var_fv0 / 11.0f;
    } else {
        anim = D_8085BE84[PLAYER_ANIMGROUP_18][this->modelAnimType];
        var_fv0 = 26.0f - var_fv0;
        if (var_fv0 < 0.0f) {
            var_fv0 = -var_fv0 * 2;
        }
        var_fv0 = var_fv0 / 12.0f;
    }

    LinkAnimation_Change(play, &this->skelAnime, anim, 1.0f, 0.0f, Animation_GetLastFrame(anim), ANIMMODE_ONCE,
                         4.0f * var_fv0);
    this->currentYaw = this->actor.shape.rot.y;
}

void func_80839E3C(Player* this, PlayState* play) {
    Player_ReturnToStandStill(this, play);
    func_80839CD8(this, play);
}

void Player_SetupStandingStillNoMorph(Player* this, PlayState* play) {
    Player_SetAction(play, this, Player_StandingStill, 1);
    Player_AnimationPlayOnce(play, this, Player_GetStandingStillAnim(this));
    this->currentYaw = this->actor.shape.rot.y;
}

void func_80839ED0(Player* this, PlayState* play) {
    if (!(this->stateFlags3 & PLAYER_STATE3_80) && (Player_ThrowDekuNut != this->actionFunc) && !func_8083213C(this)) {
        Player_ResetLookAngles(this);
        if (!(this->stateFlags1 & PLAYER_STATE1_40)) {
            if (Player_IsSwimming(this)) {
                Player_SetupSwimIdle(play, this);
            } else {
                Player_SetupStandingStillType(this, play);
            }
        }
        if (this->attentionMode < PLAYER_UNKAA5_5) {
            this->attentionMode = PLAYER_ATTENTIONMODE_NONE;
        }
    }
    this->stateFlags1 &= ~(PLAYER_STATE1_4 | PLAYER_STATE1_2000 | PLAYER_STATE1_4000 | PLAYER_STATE1_100000);
}

s32 func_80839F98(PlayState* play, Player* this) {
    if (!(this->stateFlags1 & PLAYER_STATE1_SWIMMING)) {
        if (this->linearVelocity != 0.0f) {
            func_80836B3C(play, this, 0.0f);
            return true;
        }
        func_80836AD8(play, this);
        LinkAnimation_Change(play, &this->skelAnime, &gPlayerAnim_pg_maru_change, 2.0f / 3.0f, 0.0f, 7.0f,
                             ANIMMODE_ONCE, 0.0f);
        return true;
    }
    return false;
}

// Toggles swimming/walking underwater as Zora
void func_8083A04C(Player* this) {
    if (this->currentBoots == PLAYER_BOOTS_ZORA_UNDERWATER) {
        if (CHECK_BTN_ALL(sPlayerControlInput->press.button, BTN_A)) {
            this->currentBoots = PLAYER_BOOTS_ZORA_LAND;
        }
        if (Player_UpdateSwimIdle == this->actionFunc) {
            this->genericTimer = 20;
        }
    } else {
        if (CHECK_BTN_ALL(sPlayerControlInput->press.button, BTN_B)) {
            this->currentBoots = PLAYER_BOOTS_ZORA_UNDERWATER;
        }
    }
}

s32 func_8083A0CC(Player* this, PlayState* play) {
    if (!D_80862B04 && (this->transformation == PLAYER_FORM_ZORA)) {
        func_8083A04C(this);
    }
    return false;
}

s32 func_8083A114(Player* this, PlayState* play) {
    if (!D_80862B04 && !(this->stateFlags1 & PLAYER_STATE1_RIDING_HORSE) &&
        !Player_SetupStartUnfriendlyZTargeting(this)) {
        if ((this->transformation == PLAYER_FORM_ZORA) && (this->stateFlags1 & PLAYER_STATE1_SWIMMING)) {
            func_8083A04C(this);
        } else if (CHECK_BTN_ALL(sPlayerControlInput->press.button, BTN_A) &&
                   !Player_SetupStartUnfriendlyZTargeting(this)) {
            if (this->transformation == PLAYER_FORM_GORON) {
                if (func_80839F98(play, this)) {
                    return true;
                }
            } else if (func_80839A84(play, this) || func_80839800(this, play)) {
                return true;
            }

            if ((this->putAwayCountdown == 0) && (this->itemActionParam >= PLAYER_AP_SWORD_KOKIRI) &&
                (this->transformation != PLAYER_FORM_FIERCE_DEITY)) {
                Player_UseItem(play, this, ITEM_NONE);
            } else {
                this->stateFlags2 ^= PLAYER_STATE2_100000;
            }
        }
    }

    return false;
}

s32 func_8083A274(Player* this, PlayState* play) {
    if (CHECK_BTN_ALL(sPlayerControlInput->cur.button, BTN_R) && (this->attentionMode == PLAYER_ATTENTIONMODE_NONE) &&
        (play->shootingGalleryStatus == 0)) {
        if (Player_IsGoronOrDeku(this) ||
            ((((this->transformation == PLAYER_FORM_ZORA) && !(this->stateFlags1 & PLAYER_STATE1_2000000)) ||
              ((this->transformation == PLAYER_FORM_HUMAN) && (this->currentShield != PLAYER_SHIELD_NONE))) &&
             !Player_IsFriendlyZTargeting(this) && (this->targetedActor == NULL))) {
            Player_DeactivateMeleeWeapon(this);
            Player_DetatchHeldActor(play, this);
            if (Player_SetAction(play, this, func_8084B5C0, 0)) {
                this->stateFlags1 |= PLAYER_STATE1_400000;
                if (this->transformation != PLAYER_FORM_GORON) {
                    LinkAnimationHeader* anim;
                    f32 endFrame;

                    if (!Player_IsGoronOrDeku(this)) {
                        Player_SetModelsForHoldingShield(this);
                        anim = D_8085BE84[PLAYER_ANIMGROUP_19][this->modelAnimType];
                    } else {
                        anim = (this->transformation == PLAYER_FORM_DEKU) ? &gPlayerAnim_pn_gurd
                                                                          : &gPlayerAnim_clink_normal_defense_ALL;
                    }

                    if (anim != this->skelAnime.animation) {
                        if (Player_IsUnfriendlyZTargeting(this)) {
                            this->unk_B3C = 1.0f;
                        } else {
                            this->unk_B3C = 0.0f;
                            func_8082FC60(this);
                        }
                        this->upperBodyRot.x = 0;
                        this->upperBodyRot.y = 0;
                        this->upperBodyRot.z = 0;
                    }

                    endFrame = Animation_GetLastFrame(anim);
                    LinkAnimation_Change(play, &this->skelAnime, anim, 2.0f / 3.0f,
                                         (anim == &gPlayerAnim_pn_gurd) ? 0.0f : endFrame, endFrame, ANIMMODE_ONCE,
                                         0.0f);
                }
                func_80830AE8(this);
            }

            return true;
        }
    }

    return false;
}

s32 func_8083A4A4(Player* this, f32* arg1, s16* arg2, f32 arg3) {
    s16 yaw = this->currentYaw - *arg2;

    if (ABS_ALT(yaw) > 0x6000) {
        if (Math_StepToF(&this->linearVelocity, 0.0f, arg3)) {
            *arg1 = 0.0f;
            *arg2 = this->currentYaw;
        } else {
            return true;
        }
    }
    return false;
}

void func_8083A548(Player* this) {
    if ((this->unk_ADC > 0) && !CHECK_BTN_ALL(sPlayerControlInput->cur.button, BTN_B)) {
        this->unk_ADC = -this->unk_ADC;
    }
}

s32 func_8083A580(Player* this, PlayState* play) {
    if (CHECK_BTN_ALL(sPlayerControlInput->cur.button, BTN_B)) {
        if (!(this->stateFlags1 & PLAYER_STATE1_400000) && (Player_GetMeleeWeaponHeld(this) != 0)) {
            if ((this->unk_ADC > 0) && (((this->transformation == PLAYER_FORM_ZORA)) ||
                                        ((this->unk_ADC == 1) && (this->itemActionParam != PLAYER_AP_STICK)))) {
                if (this->transformation == PLAYER_FORM_ZORA) {
                    func_80830E30(this, play);
                } else {
                    Player_StartChargeSpinAttack(play, this);
                }
                return true;
            }
        }
    } else {
        func_8083A548(this);
    }
    return false;
}

s32 func_8083A658(PlayState* play, Player* this) {
    if (this->actor.bgCheckFlags & 1) {
        Player_SetAction(play, this, Player_ThrowDekuNut, 0);
        Player_AnimationPlayOnce(play, this, &gPlayerAnim_link_normal_light_bom);
        this->attentionMode = PLAYER_ATTENTIONMODE_NONE;
        return true;
    }
    return false;
}

BottleSwingAnimInfo sBottleSwingAnims[] = {
    { &gPlayerAnim_link_bottle_bug_miss, &gPlayerAnim_link_bottle_bug_in, 2, 3 },
    { &gPlayerAnim_link_bottle_fish_miss, &gPlayerAnim_link_bottle_fish_in, 5, 3 },
};

s32 Player_TrySwingingBottle(PlayState* play, Player* this) {
    if (sUsingItemAlreadyInHand != 0) {
        if (Player_GetBottleHeld(this) >= 0) {
            Player_SetAction(play, this, Player_SwingBottle, 0);
            if (this->actor.depthInWater > 12.0f) {
                this->genericTimer = 1;
            }
            Player_PlayAnimOnceSlowed(play, this, sBottleSwingAnims[this->genericTimer].bottleSwingAnim);
            Player_PlaySfx(this, NA_SE_IT_SWORD_SWING);
            Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_AUTO_JUMP);
            return true;
        }
        return false;
    }
    return false;
}

void Player_SetupRun(Player* this, PlayState* play) {
    if ((func_8084AF9C != this->actionFunc) && (func_8084B0EC != this->actionFunc)) {
        this->unk_B70 = 0;
        this->unk_B34 = 0.0f;
        this->walkFrame = 0.0f;
        Player_ChangeAnimShortMorphLoop(play, this, D_8085BE84[PLAYER_ANIMGROUP_2][this->modelAnimType]);
    }

    Player_SetAction(play, this, Player_IsZTargeting(this) ? func_8084B0EC : func_8084AF9C, 1);
}

void Player_SetupZTargetRunning(Player* this, PlayState* play, s16 currentYaw) {
    this->currentYaw = currentYaw;
    this->actor.shape.rot.y = this->currentYaw;
    Player_SetupRun(this, play);
}

s32 Player_SetupDefaultSpawnBehavior(PlayState* play, Player* this, f32 arg2) {
    WaterBox* waterBox;
    f32 ySurface = this->actor.world.pos.y;

    if (WaterBox_GetSurface1(play, &play->colCtx, this->actor.world.pos.x, this->actor.world.pos.z, &ySurface,
                             &waterBox)) {
        ySurface -= this->actor.world.pos.y;
        if (this->ageProperties->unk_24 <= ySurface) {
            Player_SetAction(play, this, func_80850B18, 0);
            Player_ChangeAnimLongMorphLoop(play, this, &gPlayerAnim_link_swimer_swim);
            this->stateFlags1 |= (PLAYER_STATE1_SWIMMING | PLAYER_STATE1_IN_CUTSCENE);
            this->genericTimer = 20;
            this->linearVelocity = 2.0f;
            Player_SetBootData(play, this);
            return false;
        }
    }

    Player_SetupMiniCsMovement(play, this, arg2, this->actor.shape.rot.y);
    this->stateFlags1 |= PLAYER_STATE1_IN_CUTSCENE;
    return true;
}

/**
 * Update for using telescopes. SCENE_AYASHIISHOP acts quite differently: it has a different camera mode and cannot use
 * zooming.
 *
 * - Stick inputs move the view; shape.rot.y is used as a base position which cannot be looked too far away from. (This
 * is not necessarily the same as the original angle of the spawn.)
 * - A can be used to zoom (except in SCENE_AYASHIISHOP)
 * - B exits, using the RESPAWN_MODE_DOWN entrance
 */
void func_8083A98C(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    Player* this = THIS;
    s32 camMode;

    if (play->csCtx.state != CS_STATE_IDLE) {
        return;
    }

    if (DECR(this->genericTimer) != 0) {
        camMode = (play->sceneId != SCENE_AYASHIISHOP) ? CAM_MODE_FIRSTPERSON : CAM_MODE_DEKUHIDE;

        // Show controls overlay. SCENE_AYASHIISHOP does not have Zoom, so has a different one.
        if (this->genericTimer == 1) {
            Message_StartTextbox(play, (play->sceneId == SCENE_AYASHIISHOP) ? 0x2A00 : 0x5E6, NULL);
        }
    } else {
        sPlayerControlInput = play->state.input;
        if (play->view.fovy >= 25.0f) {
            s16 prevFocusX = thisx->focus.rot.x;
            s16 prevFocusY = thisx->focus.rot.y;
            s16 inputY;
            s16 inputX;
            s16 newYaw; // from base position shape.rot.y

            // Pitch:
            inputY = sPlayerControlInput->rel.stick_y * 4;
            // Add input, clamped to prevent turning too fast
            thisx->focus.rot.x += CLAMP(inputY, -0x12C, 0x12C);
            // Prevent looking too far up or down
            thisx->focus.rot.x = CLAMP(thisx->focus.rot.x, -0x2EE0, 0x2EE0);

            // Yaw: shape.rot.y is used as a fixed starting position
            inputX = sPlayerControlInput->rel.stick_x * -4;
            // Start from current position: no input -> no change
            newYaw = thisx->focus.rot.y - thisx->shape.rot.y;
            // Add input, clamped to prevent turning too fast
            newYaw += CLAMP(inputX, -0x12C, 0x12C);
            // Prevent looking too far left or right of base position
            newYaw = CLAMP(newYaw, -0x3E80, 0x3E80);
            thisx->focus.rot.y = thisx->shape.rot.y + newYaw;

            if (play->sceneId == SCENE_00KEIKOKU) {
                f32 focusDeltaX = (s16)(thisx->focus.rot.x - prevFocusX);
                f32 focusDeltaY = (s16)(thisx->focus.rot.y - prevFocusY);

                func_8019FAD8(&gSfxDefaultPos, NA_SE_PL_TELESCOPE_MOVEMENT - SFX_FLAG,
                              sqrtf(SQ(focusDeltaX) + SQ(focusDeltaY)) / 300.0f);
            }
        }

        if (play->sceneId == SCENE_AYASHIISHOP) {
            camMode = CAM_MODE_DEKUHIDE;
        } else if (CHECK_BTN_ALL(sPlayerControlInput->cur.button, BTN_A)) { // Zoom
            camMode = CAM_MODE_TARGET;
        } else {
            camMode = CAM_MODE_NORMAL;
        }

        // Exit
        if (CHECK_BTN_ALL(sPlayerControlInput->press.button, BTN_B)) {
            // Message_CloseTextbox
            func_801477B4(play);

            if (play->sceneId == SCENE_00KEIKOKU) {
                gSaveContext.respawn[RESPAWN_MODE_DOWN].entrance = ENTRANCE(ASTRAL_OBSERVATORY, 2);
            } else {
                u16 entrance;

                if (play->sceneId == SCENE_AYASHIISHOP) {
                    entrance = ENTRANCE(CURIOSITY_SHOP, 3);
                } else {
                    entrance = ENTRANCE(PIRATES_FORTRESS_INTERIOR, 8);
                }
                gSaveContext.respawn[RESPAWN_MODE_DOWN].entrance = entrance;
            }

            Play_TriggerVoidOut(&play->state);
            gSaveContext.respawnFlag = -2;
            play->transitionType = TRANS_TYPE_21;
        }
    }

    Camera_ChangeSetting(Play_GetCamera(play, CAM_ID_MAIN), CAM_SET_TELESCOPE);
    Camera_ChangeMode(Play_GetCamera(play, CAM_ID_MAIN), camMode);
}

// Set up using a telescope
void Player_InitMode_Telescope(PlayState* play, Player* this) {
    this->actor.update = func_8083A98C;
    this->actor.draw = NULL;
    if (play->sceneId == SCENE_00KEIKOKU) {
        this->actor.focus.rot.x = 0xBD8;
        this->actor.focus.rot.y = -0x4D74;
        this->genericTimer = 20;
    } else if (play->sceneId == SCENE_AYASHIISHOP) {
        this->actor.focus.rot.x = 0x9A6;
        this->actor.focus.rot.y = 0x2102;
        this->genericTimer = 2;
    } else {
        this->actor.focus.rot.x = 0x9A6;
        this->actor.focus.rot.y = 0x2102;
        this->genericTimer = 20;
    }
    play->actorCtx.flags |= ACTORCTX_FLAG_TELESCOPE_ON;
}

void Player_InitMode_B(PlayState* play, Player* this) {
    Player_SetupStandingStillMorph(this, play);
}

void Player_InitMode_NoMomentum(PlayState* play, Player* this) {
    if (Player_SetupDefaultSpawnBehavior(play, this, 180.0f)) {
        this->genericTimer = -20;
    }
}

// InitModes 0xA and 0xE
void Player_InitMode_WalkingSlow(PlayState* play, Player* this) {
    this->linearVelocity = 2.0f;
    gSaveContext.entranceSpeed = 2.0f;

    if (Player_SetupDefaultSpawnBehavior(play, this, 120.0f)) {
        this->genericTimer = -15;
    }
}

void Player_InitMode_WalkingPreserveMomentum(PlayState* play, Player* this) {
    if (gSaveContext.entranceSpeed < 0.1f) {
        gSaveContext.entranceSpeed = 0.1f;
    }

    this->linearVelocity = gSaveContext.entranceSpeed;
    if (Player_SetupDefaultSpawnBehavior(play, this, 800.0f)) {
        this->genericTimer = -80.0f / this->linearVelocity;
        if (this->genericTimer < -20) {
            this->genericTimer = -20;
        }
    }
}

void Player_SetupFriendlyBackwalk(Player* this, s16 currentYaw, PlayState* play) {
    Player_SetAction(play, this, Player_FriendlyBackwalk, 1);
    LinkAnimation_CopyJointToMorph(play, &this->skelAnime);
    this->walkFrame = 0.0f;
    this->unk_B34 = 0.0f;
    this->currentYaw = currentYaw;
}

void Player_SetupFriendlySidewalk(Player* this, PlayState* play) {
    Player_SetAction(play, this, Player_EndSidewalk, 1);
    Player_ChangeAnimShortMorphLoop(play, this, D_8085BE84[PLAYER_ANIMGROUP_1][this->modelAnimType]);
}

void Player_SetupUnfriendlyBackwalk(Player* this, s16 currentYaw, PlayState* play) {
    Player_SetAction(play, this, Player_UnfriendlyBackwalk, 1);
    LinkAnimation_Change(play, &this->skelAnime, &gPlayerAnim_link_anchor_back_walk, 1.0f, 0.0f,
                         Animation_GetLastFrame(&gPlayerAnim_link_anchor_back_walk), ANIMMODE_ONCE, -6.0f);
    this->linearVelocity = 8.0f;
    this->currentYaw = currentYaw;
}

void Player_SetupSidewalk(Player* this, PlayState* play) {
    Player_SetAction(play, this, Player_Sidewalk, 1);
    Player_ChangeAnimShortMorphLoop(play, this, D_8085BE84[PLAYER_ANIMGROUP_24][this->modelAnimType]);
    this->walkFrame = 0.0f;
}

void Player_SetupEndUnfriendlyBackwalk(Player* this, PlayState* play) {
    Player_SetAction(play, this, Player_EndUnfriendlyBackwalk, 1);
    LinkAnimation_PlayOnceSetSpeed(play, &this->skelAnime, &gPlayerAnim_link_anchor_back_brake, 6.0f / 3.0f);
}

void Player_SetupTurn(PlayState* play, Player* this, s16 currentYaw) {
    this->currentYaw = currentYaw;
    Player_SetAction(play, this, Player_Turn, 1);
    this->unk_B4E = 1200;
    this->unk_B4E *= sWaterSpeedScale;
    LinkAnimation_Change(play, &this->skelAnime, D_8085BE84[PLAYER_ANIMGROUP_25][this->modelAnimType], 1.0f, 0.0f, 0.0f,
                         ANIMMODE_LOOP, -6.0f);
}

void Player_EndUnfriendlyZTarget(Player* this, PlayState* play) {
    LinkAnimationHeader* anim;

    Player_SetAction(play, this, Player_StandingStill, 1);
    if (this->leftRightBlendWeight < 0.5f) {
        anim = D_8085BE84[PLAYER_ANIMGROUP_27][this->modelAnimType];
    } else {
        anim = D_8085BE84[PLAYER_ANIMGROUP_26][this->modelAnimType];
    }
    Player_AnimationPlayOnce(play, this, anim);
    this->currentYaw = this->actor.shape.rot.y;
}

void Player_SetupUnfriendlyZTarget(Player* this, PlayState* play) {
    Player_SetAction(play, this, Player_UnfriendlyZTargetStandingStill, 1);
    Player_ChangeAnimMorphToLastFrame(play, this, D_8085BE84[PLAYER_ANIMGROUP_6][this->modelAnimType]);
    this->genericTimer = 1;
}

void Player_SetupEndUnfriendlyZTarget(Player* this, PlayState* play) {
    if (this->linearVelocity != 0.0f) {
        Player_SetupRun(this, play);
    } else {
        Player_EndUnfriendlyZTarget(this, play);
    }
}

void Player_EndMiniCsMovement(Player* this, PlayState* play) {
    if (this->linearVelocity != 0.0f) {
        Player_SetupRun(this, play);
    } else {
        Player_SetupStandingStillType(this, play);
    }
}

void Player_StartJumpOutOfWater(PlayState* play, Player* this, f32 yVelocity) {
    this->stateFlags1 |= PLAYER_STATE1_JUMPING;
    this->stateFlags1 &= ~PLAYER_STATE1_SWIMMING;
    Player_ResetSubCam(play, this);

    if (Player_SetupSpawnSplash(play, this, yVelocity, 500)) {
        Player_PlaySfx(this, NA_SE_EV_JUMP_OUT_WATER);
    }
    Player_SetBootData(play, this);
}

s32 Player_SetupDive(PlayState* play, Player* this, Input* input) {
    if ((!(this->stateFlags1 & PLAYER_STATE1_GETTING_ITEM) && !(this->stateFlags2 & PLAYER_STATE2_DIVING) &&
         (this->transformation != PLAYER_FORM_ZORA)) &&
        ((input == NULL) ||
         ((((this->interactRangeActor == NULL) || (this->interactRangeActor->id != ACTOR_EN_ZOG)) &&
           CHECK_BTN_ALL(input->press.button, BTN_A)) &&
          ((ABS_ALT(this->unk_AAA) < 0x2EE0) && (this->currentBoots < PLAYER_BOOTS_ZORA_UNDERWATER) &&
           ((s32)SurfaceType_GetConveyorSpeed(&play->colCtx, this->actor.floorPoly, this->actor.floorBgId) < 2))))) {
        if (Player_StartCutscene != this->actionFunc) {
            Player_SetAction(play, this, func_808516B4, 0);
        }

        Player_AnimationPlayOnce(play, this, &gPlayerAnim_link_swimer_swim_deep_start);
        this->unk_AAA = 0;
        this->stateFlags2 |= PLAYER_STATE2_DIVING;
        this->actor.velocity.y = 0.0f;
        if (input != NULL) {
            this->stateFlags2 |= PLAYER_STATE2_800;
            Player_PlaySfx(this, NA_SE_PL_DIVE_BUBBLE);
        }

        return true;
    }

    if ((this->transformation != PLAYER_FORM_DEKU) &&
        ((this->stateFlags1 & PLAYER_STATE1_GETTING_ITEM) ||
         ((this->stateFlags2 & PLAYER_STATE2_DIVING) &&
          (((func_80850D68 != this->actionFunc) && !(this->stateFlags3 & PLAYER_STATE3_8000)) ||
           (this->unk_AAA < -0x1555)))) &&
        ((this->actor.depthInWater - this->actor.velocity.y) < this->ageProperties->unk_30)) {
        s32 temp_v0_3;
        s16 sp2A;
        f32 sp24;

        this->stateFlags2 &= ~PLAYER_STATE2_DIVING;
        Player_ResetSubCam(play, this);
        temp_v0_3 = Player_SetupSpawnSplash(play, this, this->actor.velocity.y, 0x1F4);
        if (this->stateFlags3 & PLAYER_STATE3_8000) {
            sp2A = this->unk_B86[1];
            sp24 = this->unk_B48 * 1.5f;
            Player_SetAction(play, this, func_8084CA24, 1);
            this->stateFlags3 |= PLAYER_STATE3_8000;
            this->stateFlags1 &= ~PLAYER_STATE1_SWIMMING;
            sp24 = CLAMP_MAX(sp24, 13.5f);
            this->linearVelocity = Math_CosS(this->unk_AAA) * sp24;
            this->actor.velocity.y = -Math_SinS(this->unk_AAA) * sp24;
            this->unk_B86[1] = sp2A;
            Player_PlaySfx(this, NA_SE_EV_JUMP_OUT_WATER);
            return true;
        }

        if (temp_v0_3) {
            Player_PlaySfx(this, NA_SE_PL_FACE_UP);
        } else {
            Player_PlaySfx(this, NA_SE_PL_FACE_UP);
        }

        if (input != NULL) {
            Player_SetAction(play, this, func_808519FC, 1);
            if (this->stateFlags1 & PLAYER_STATE1_GETTING_ITEM) {
                this->stateFlags1 |=
                    (PLAYER_STATE1_GETTING_ITEM | PLAYER_STATE1_HOLDING_ACTOR | PLAYER_STATE1_IN_CUTSCENE);
            }
            this->genericTimer = 2;
        }

        Player_ChangeAnimMorphToLastFrame(play, this,
                                          (this->stateFlags1 & PLAYER_STATE1_HOLDING_ACTOR)
                                              ? &gPlayerAnim_link_swimer_swim_get
                                              : &gPlayerAnim_link_swimer_swim_deep_end);
        return true;
    }

    return false;
}

void Player_SetupSwim(PlayState* play, Player* this, s16 swimYaw) {
    Player_SetAction(play, this, Player_Swim, 0);
    Player_ChangeAnimLongMorphLoop(play, this, &gPlayerAnim_link_swimer_swim);
    this->actor.shape.rot.y = swimYaw;
    this->currentYaw = swimYaw;
}

void func_8083B798(PlayState* play, Player* this) {
    if (this->transformation == PLAYER_FORM_ZORA) {
        Player_SetAction(play, this, Player_Swim, 0);
        LinkAnimation_Change(play, &this->skelAnime, &gPlayerAnim_link_swimer_swim, 1.0f,
                             Animation_GetLastFrame(&gPlayerAnim_link_swimer_swim), 0.0f, ANIMMODE_LOOP, 0.0f);
        this->unk_B48 = 2.0f;
    } else {
        Player_AnimationPlayLoop(play, this, &gPlayerAnim_link_swimer_swim);
        this->genericTimer = 1;
    }

    this->unk_AAA = 0x3E80;
}

void func_8083B850(PlayState* play, Player* this) {
    this->currentBoots = PLAYER_BOOTS_ZORA_LAND;
    this->prevBoots = PLAYER_BOOTS_ZORA_LAND;
    Player_SetAction(play, this, func_80850D68, 0);
    this->unk_B48 = sqrtf(SQ(this->linearVelocity) + SQ(this->actor.velocity.y));
    Player_OverrideBlureColors(play, this, 1, 8);
    this->currentBoots = PLAYER_BOOTS_ZORA_LAND;
    this->prevBoots = PLAYER_BOOTS_ZORA_LAND;
}

void func_8083B8D0(PlayState* play, Player* this) {
    if (Player_SetupSpawnSplash(play, this, this->actor.velocity.y, 500)) {
        Player_PlaySfx(this, NA_SE_EV_DIVE_INTO_WATER);
        if (this->fallDistance > 800) {
            Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_CLIMB_END);
        }
    }
}

void func_8083B930(PlayState* play, Player* this) {
    LinkAnimationHeader* var_a2;

    if ((this->currentBoots < PLAYER_BOOTS_ZORA_UNDERWATER) || !(this->actor.bgCheckFlags & 1) ||
        (func_80857BE8 == this->actionFunc)) {
        Player_ResetAttributesAndHeldActor(play, this);

        if (func_8084CA24 == this->actionFunc) {
            func_8083B850(play, this);
            this->stateFlags3 |= PLAYER_STATE3_8000;
        } else if ((this->transformation == PLAYER_FORM_ZORA) && (func_8084C94C == this->actionFunc)) {
            func_8083B850(play, this);
            this->stateFlags3 |= PLAYER_STATE3_8000;
            func_8082DB60(play, this, &gPlayerAnim_pz_fishswim);
        } else if ((this->currentBoots < PLAYER_BOOTS_ZORA_UNDERWATER) && (this->stateFlags2 & PLAYER_STATE2_DIVING)) {
            this->stateFlags2 &= ~PLAYER_STATE2_DIVING;
            Player_SetupDive(play, this, NULL);
            this->genericVar = 1;
        } else if (func_8084C94C == this->actionFunc) {
            Player_SetAction(play, this, func_808516B4, 0);
            func_8083B798(play, this);
        } else {
            Player_SetAction(play, this, Player_UpdateSwimIdle, 1);
            Player_ChangeAnimMorphToLastFrame(play, this,
                                              (this->actor.bgCheckFlags & 1) ? &gPlayerAnim_link_swimer_wait2swim_wait
                                                                             : &gPlayerAnim_link_swimer_land2swim_wait);
        }
    }
    if (!(this->stateFlags1 & PLAYER_STATE1_SWIMMING) || (this->actor.depthInWater < this->ageProperties->unk_2C)) {
        func_8083B8D0(play, this);
    }

    this->stateFlags1 |= PLAYER_STATE1_SWIMMING;
    this->stateFlags2 |= PLAYER_STATE2_DIVING;
    this->stateFlags1 &= ~(PLAYER_STATE1_JUMPING | PLAYER_STATE1_FREEFALLING);

    this->unk_AEC = 0.0f;
    Player_SetBootData(play, this);
}

void func_8083BB4C(PlayState* play, Player* this) {
    f32 sp1C = this->actor.depthInWater - this->ageProperties->unk_2C;

    if (sp1C < 0.0f) {
        this->underwaterTimer = 0;
        if ((this->transformation == PLAYER_FORM_ZORA) && (this->actor.bgCheckFlags & 1)) {
            this->currentBoots = 4;
        }
        func_801A3E38(0);
    } else {
        func_801A3E38(0x20);
        if ((this->transformation == PLAYER_FORM_ZORA) || (sp1C < 10.0f)) {
            this->underwaterTimer = 0;
        } else if (this->underwaterTimer < 300) {
            this->underwaterTimer++;
        }
    }

    if ((this->actor.parent == NULL) && (Player_JumpUpToLedge != this->actionFunc) &&
        (func_8084F3DC != this->actionFunc) &&
        ((func_8084CA24 != this->actionFunc) || (this->actor.velocity.y < -2.0f))) {
        if (this->ageProperties->unk_2C < this->actor.depthInWater) {
            if (this->transformation == PLAYER_FORM_GORON) {
                func_80834140(play, this, &gPlayerAnim_link_swimer_swim_down);
                func_808345C8();
                func_8083B8D0(play, this);
            } else if (this->transformation == PLAYER_FORM_DEKU) {
                if (this->remainingHopsCounter != 0) {
                    func_808373F8(play, this, NA_SE_VO_LI_AUTO_JUMP);
                } else {
                    if ((play->sceneId == SCENE_20SICHITAI) && (this->unk_3CF == 0)) {
                        if (gSaveContext.eventInf[5] & 1) {
                            play->nextEntrance = ENTRANCE(TOURIST_INFORMATION, 2);
                        } else {
                            play->nextEntrance = ENTRANCE(TOURIST_INFORMATION, 1);
                        }
                        play->transitionTrigger = TRANS_TRIGGER_START;
                        play->transitionType = TRANS_TYPE_04;
                        this->stateFlags1 |= PLAYER_STATE1_200;
                        play_sound(NA_SE_SY_DEKUNUTS_JUMP_FAILED);
                    } else if ((this->unk_3CF == 0) &&
                               ((play->sceneId == SCENE_30GYOSON) || (play->sceneId == SCENE_31MISAKI) ||
                                (play->sceneId == SCENE_TORIDE))) {
                        Play_TriggerVoidOut(&play->state);
                        func_808345C8();
                    } else {
                        Player_SetAction(play, this, func_808497A0, 0);
                        this->stateFlags1 |= PLAYER_STATE1_IN_CUTSCENE;
                    }
                    func_8083B8D0(play, this);
                }
            } else if (!(this->stateFlags1 & PLAYER_STATE1_SWIMMING) ||
                       (((this->currentBoots < PLAYER_BOOTS_ZORA_UNDERWATER) || !(this->actor.bgCheckFlags & 1)) &&
                        (func_8084E724 != this->actionFunc) && (func_80851B58 != this->actionFunc) &&
                        (func_80851BD4 != this->actionFunc) && (Player_UpdateSwimIdle != this->actionFunc) &&
                        (Player_Swim != this->actionFunc) && (Player_ZTargetSwimming != this->actionFunc) &&
                        (func_808516B4 != this->actionFunc) && (func_808519FC != this->actionFunc) &&
                        (func_80850B18 != this->actionFunc) && (func_80850D68 != this->actionFunc))) {
                func_8083B930(play, this);
            }
        } else if ((this->stateFlags1 & PLAYER_STATE1_SWIMMING) &&
                   (this->actor.depthInWater < this->ageProperties->unk_24) &&
                   (((func_80850D68 != this->actionFunc) && !(this->stateFlags3 & PLAYER_STATE3_8000)) ||
                    (this->actor.bgCheckFlags & 1))) {
            if (this->skelAnime.moveFlags == 0) {
                Player_SetupTurn(play, this, this->actor.shape.rot.y);
            }
            Player_StartJumpOutOfWater(play, this, this->actor.velocity.y);
        }
    }
}

void func_8083BF54(PlayState* play, Player* this) {
    Vec3f sp84;
    s32 temp_v0;
    s32 var_a2;

    this->actor.terminalVelocity = -20.0f;
    this->actor.gravity = REG(68) / 100.0f;

    var_a2 = false;
    temp_v0 = Player_IsFloorSinkingSand(sPlayerCurrentFloorType);

    if (temp_v0 ||
        (((var_a2 = (sPlayerCurrentFloorType == BG_FLOOR_TYPE_14) || (sPlayerCurrentFloorType == BG_FLOOR_TYPE_15))) ||
         (sPlayerCurrentFloorType == BG_FLOOR_TYPE_13))) {
        f32 temp_fv1_2;
        f32 var_fa1;
        f32 var_ft4;
        f32 var_fv0;
        u16 sfxId;

        var_ft4 = fabsf(this->linearVelocity + D_80862B3C) * 20.0f;
        if (temp_v0) {
            if (sPlayerCurrentFloorType == BG_FLOOR_TYPE_4) {
                var_fa1 = 1300.0f;
            } else if (sPlayerCurrentFloorType == BG_FLOOR_TYPE_7) {
                var_fa1 = 20000.0f;
                var_ft4 = 0.0f;
            } else {
                var_fa1 = 10000.0f;
                var_ft4 *= 1.6f;
            }
            sfxId = NA_SE_PL_SINK_ON_SAND - SFX_FLAG;
        } else if (var_a2) {
            if (sPlayerCurrentFloorType == BG_FLOOR_TYPE_14) {
                var_fa1 = 400.0f;
                var_ft4 *= 10.0f;
            } else {
                var_fa1 = 1300.0f;
                var_ft4 = 0.0f;
            }
            sfxId = NA_SE_PL_SINK_ON_SNOW - SFX_FLAG;
        } else {
            var_fa1 = (this->transformation == PLAYER_FORM_GORON) ? 10000.0f : 1000.0f;
            var_ft4 = 0.0f;
            sfxId = NA_SE_PL_SINK_ON_SAND - SFX_FLAG;
        }

        var_fa1 = CLAMP_MIN(var_fa1, this->shapeOffsetY);

        var_fv0 = (sPlayerCurrentFloorType == BG_FLOOR_TYPE_14) ? 200.0f : (var_fa1 - this->shapeOffsetY) * 0.02f;
        var_fv0 = CLAMP(var_fv0, 0.0f, 300.0f);

        temp_fv1_2 = this->shapeOffsetY;
        this->shapeOffsetY += var_fv0 - var_ft4;
        this->shapeOffsetY = CLAMP(this->shapeOffsetY, 0.0f, var_fa1);

        if ((this->linearVelocity == 0.0f) && (fabsf(this->shapeOffsetY - temp_fv1_2) > 2.0f)) {
            func_800B8F98(&this->actor, sfxId);
        }

        this->actor.gravity -= this->shapeOffsetY * 0.004f;
    } else {
        this->shapeOffsetY = 0.0f;
    }

    if ((this->stateFlags3 & PLAYER_STATE3_10) && (this->actor.bgCheckFlags & 0x20)) {
        if (this->actor.depthInWater < 50.0f) {
            f32 temp_fv1_5;
            Vec3f* bodyPartsPos;
            f32 var_fa0_3;
            f32 var_ft4_2;

            var_ft4_2 = fabsf(this->bodyPartsPos[PLAYER_BODYPART_WAIST].x - this->unk_D6C.x) +
                        fabsf(this->bodyPartsPos[PLAYER_BODYPART_WAIST].y - this->unk_D6C.y) +
                        fabsf(this->bodyPartsPos[PLAYER_BODYPART_WAIST].z - this->unk_D6C.z);
            var_ft4_2 = CLAMP_MAX(var_ft4_2, 4.0f);

            this->unk_AEC += var_ft4_2;
            if (this->unk_AEC > 15.0f) {
                this->unk_AEC = 0.0f;
                sp84.x = (Rand_ZeroOne() * 10.0f) + this->actor.world.pos.x;
                sp84.y = this->actor.world.pos.y + this->actor.depthInWater;
                sp84.z = (Rand_ZeroOne() * 10.0f) + this->actor.world.pos.z;

                EffectSsGRipple_Spawn(play, &sp84, 100, 500, 0);

                if ((this->linearVelocity > 4.0f) && !Player_IsSwimming(this) &&
                    ((this->actor.world.pos.y + this->actor.depthInWater) <
                     this->bodyPartsPos[PLAYER_BODYPART_WAIST].y)) {
                    Player_SetupSpawnSplash(play, this, 20.0f,
                                            (fabsf(this->linearVelocity) * 50.0f) + (this->actor.depthInWater * 5.0f));
                } else if (this->stateFlags3 & PLAYER_STATE3_8000) {
                    s32 i;

                    var_fa0_3 = (this->actor.world.pos.y + this->actor.depthInWater) - 5.0f;
                    bodyPartsPos = this->bodyPartsPos;

                    for (i = 0; i < PLAYER_BODYPART_MAX; i++, bodyPartsPos++) {
                        temp_fv1_5 = bodyPartsPos->y - var_fa0_3;

                        if (temp_fv1_5 > 0.0f) {
                            Player_SetupSpawnSplash(play, this, 20.0f,
                                                    fabsf(this->linearVelocity) * 20.0f + (temp_fv1_5 * 10.0f));
                        }
                    }
                }
            }
        }

        if (this->ageProperties->unk_2C < this->actor.depthInWater) {
            s32 numBubbles = 0;
            s32 i;
            f32 var_fv1;

            var_fv1 = (this->stateFlags1 & PLAYER_STATE1_4000000)
                          ? -fabsf(this->linearVelocity)
                          : ((func_80850D68 == this->actionFunc)
                                 ? (ABS_ALT(this->unk_B8A) * -0.004f) + (this->unk_B48 * -0.38f)
                                 : this->actor.velocity.y);

            if ((var_fv1 > -1.0f) || ((this->currentBoots == 5) && (this->actor.bgCheckFlags & 1))) {
                if (Rand_ZeroOne() < 0.2f) {
                    numBubbles = 1;
                }
            } else {
                numBubbles = var_fv1 * -0.3f;
                if (numBubbles >= 9) {
                    numBubbles = 8;
                }
            }

            for (i = 0; i < numBubbles; i++) {
                EffectSsBubble_Spawn(play, &this->actor.world.pos, 20.0f, 10.0f, 20.0f, 0.13f);
            }
        }
    }
}

s32 Player_LookAtTargetActor(Player* this, s32 arg1) {
    Actor* targetedActor = this->targetedActor;
    Vec3f headPos;
    s16 pitchTarget;
    s16 yawTarget;

    headPos.x = this->actor.world.pos.x;
    headPos.y = this->bodyPartsPos[PLAYER_BODYPART_HEAD].y + 3.0f;
    headPos.z = this->actor.world.pos.z;

    pitchTarget = Math_Vec3f_Pitch(&headPos, &targetedActor->focus.pos);
    yawTarget = Math_Vec3f_Yaw(&headPos, &targetedActor->focus.pos);

    Math_SmoothStepToS(&this->actor.focus.rot.y, yawTarget, 4, 0x2710, 0);
    Math_SmoothStepToS(&this->actor.focus.rot.x, pitchTarget, 4, 0x2710, 0);

    this->lookFlags |= PLAYER_LOOKFLAGS_OVERRIDE_FOCUS_ROT_Y;

    return Player_UpdateLookAngles(this, arg1);
}

Vec3f D_8085D218 = { 0.0f, 100.0f, 40.0f };

void Player_SetLookAngle(Player* this, PlayState* play) {
    if (this->targetedActor != NULL) {
        if (Actor_PlayerIsAimingReadyFpsItem(this) || Player_IsAimingReadyZoraFins(this)) {
            Player_LookAtTargetActor(this, true);
        } else {
            Player_LookAtTargetActor(this, false);
        }
        return;
    }

    if (sPlayerCurrentFloorType == BG_FLOOR_TYPE_11) {
        Math_SmoothStepToS(&this->actor.focus.rot.x, -20000, 10, 4000, 800);
    } else {
        s16 focusPitchTarget = 0;
        f32 yIntersect;
        Vec3f pos;
        s16 focusPitchAngle;

        yIntersect = func_80835D2C(play, this, &D_8085D218, &pos);
        if (yIntersect > BGCHECK_Y_MIN) {
            focusPitchAngle = Math_FAtan2F(40.0f, this->actor.world.pos.y - yIntersect);
            focusPitchTarget = CLAMP(focusPitchAngle, -4000, 4000);
        }
        this->actor.focus.rot.y = this->actor.shape.rot.y;
        Math_SmoothStepToS(&this->actor.focus.rot.x, focusPitchTarget, 14, 4000, 30);
    }

    Player_UpdateLookAngles(this, Actor_PlayerIsAimingReadyFpsItem(this) || Player_IsAimingReadyZoraFins(this));
}

void func_8083C85C(Player* this) {
    Math_ScaledStepToS(&this->upperBodyRot.x, D_80862B3C * -500.0f, 900);
    this->headRot.x = (-(f32)this->upperBodyRot.x * 0.5f);
    this->lookFlags |= 0x48;
}

void func_8083C8E8(Player* this, PlayState* play) {
    if ((Actor_PlayerIsAimingReadyFpsItem(this) == 0) && (Player_IsAimingReadyZoraFins(this) == 0) &&
        ((this->linearVelocity > 5.0f) || (D_80862B3C != 0.0f))) {
        s16 temp1;
        s16 temp2;

        temp1 = this->linearVelocity * 200.0f;
        temp2 = BINANG_SUB(this->currentYaw, this->actor.shape.rot.y) * this->linearVelocity * 0.1f;

        temp1 = CLAMP(temp1, -0xFA0, 0xFA0);

        temp1 += (s16)(s32)(D_80862B3C * -500.0f);

        temp1 = CLAMP(temp1, -0x2EE0, 0x2EE0);

        temp2 = CLAMP(-temp2, -0xFA0, 0xFA0);

        Math_ScaledStepToS(&this->upperBodyRot.x, temp1, 0x384);
        this->headRot.x = -(f32)this->upperBodyRot.x * 0.5f;
        Math_ScaledStepToS(&this->headRot.z, temp2, 0x12C);
        Math_ScaledStepToS(&this->upperBodyRot.z, temp2, 0xC8);
        this->lookFlags |= 0x168;
    } else {
        Player_SetLookAngle(this, play);
    }
}

void Player_SetRunVelocityAndYawImpl(Player* this, f32 targetVelocity, s16 targetYaw, f32 arg3, f32 arg4, s16 arg5) {
    Math_AsymStepToF(&this->linearVelocity, targetVelocity, arg3, arg4);
    Math_ScaledStepToS(&this->currentYaw, targetYaw, arg5);
}

void Player_SetRunVelocityAndYaw(Player* this, f32 targetVelocity, s16 targetYaw) {
    Player_SetRunVelocityAndYawImpl(this, targetVelocity, targetYaw, REG(19) / 100.0f, 1.5f, REG(27));
}

s32 func_8083CBC4(Player* this, f32 arg1, s16 arg2, f32 arg3, f32 arg4, f32 arg5, s16 arg6) {
    s16 temp_v0 = this->currentYaw - arg2;

    if ((this->speedLimit * 1.5f) < fabsf(this->linearVelocity)) {
        arg5 *= 4.0f;
        arg3 *= 4.0f;
    }

    if (ABS_ALT(temp_v0) > 0x6000) {
        if (Math_StepToF(&this->linearVelocity, 0.0f, arg3) == 0) {
            return false;
        }

        this->currentYaw = arg2;
    } else {
        Math_AsymStepToF(&this->linearVelocity, arg1, arg4, arg5);
        Math_ScaledStepToS(&this->currentYaw, arg2, arg6);
    }

    return true;
}

HorseMountAnimInfo sMountHorseAnims[][2] = {
    {
        { &gPlayerAnim_link_uma_left_up, 35.17f, 6.6099997f },
        { &gPlayerAnim_link_uma_right_up, -34.16f, 7.91f },
    },
    {
        { &gPlayerAnim_cl_uma_leftup, 22.718237f, 2.3294117f },
        { &gPlayerAnim_cl_uma_rightup, -22.0f, 1.9800001f },
    },
};

u16 D_8085D254[] = {
    0x1804, // PLAYER_FORM_GORON
    0x1805, // PLAYER_FORM_ZORA
    0x1806, // PLAYER_FORM_DEKU
    0x1806, // PLAYER_FORM_HUMAN
};

u16 D_8085D25C[] = {
    0x1804, // PLAYER_FORM_FIERCE_DEITY
    0x1804, // PLAYER_FORM_GORON
    0x1805, // PLAYER_FORM_ZORA
    0x1806, // PLAYER_FORM_DEKU
};

// Player_MountHorse
s32 Player_SetupMountHorse(Player* this, PlayState* play) {
    EnHorse* rideActor = (EnHorse*)this->rideActor;

    if (rideActor != NULL) {
        if ((rideActor->type != HORSE_TYPE_2) && (this->transformation != PLAYER_FORM_FIERCE_DEITY)) {
            if (CHECK_BTN_ALL(sPlayerControlInput->press.button, BTN_A)) {
                if (ActorCutscene_GetCanPlayNext(0x7C)) {
                    rideActor->actor.textId = D_8085D254[this->transformation - 1];
                    Player_TalkWithPlayer(play, &rideActor->actor);
                    return true;
                }
            }

            ActorCutscene_SetIntentToPlay(0x7C);
        } else if ((rideActor->type == HORSE_TYPE_2) && (this->transformation != PLAYER_FORM_HUMAN)) {
            if (CHECK_BTN_ALL(sPlayerControlInput->press.button, BTN_A)) {
                if (ActorCutscene_GetCanPlayNext(0x7C)) {
                    rideActor->actor.textId = D_8085D25C[this->transformation];
                    Player_TalkWithPlayer(play, &rideActor->actor);
                    return true;
                }
            }

            ActorCutscene_SetIntentToPlay(0x7C);
        } else {
            if (CHECK_BTN_ALL(sPlayerControlInput->press.button, BTN_A)) {
                s32 pad[2];
                f32 sp28 = Math_CosS(rideActor->actor.shape.rot.y);
                f32 sp24 = Math_SinS(rideActor->actor.shape.rot.y);
                HorseMountAnimInfo* entry;
                f32 riderOffsetX;
                f32 riderOffsetZ;

                func_80832558(play, this, Player_SetupRideHorse);

                this->stateFlags1 |= PLAYER_STATE1_RIDING_HORSE;
                this->actor.bgCheckFlags &= ~0x20;
                this->isBurning = false;

                if (this->transformation == PLAYER_FORM_FIERCE_DEITY) {
                    entry = sMountHorseAnims[0];
                } else {
                    entry = sMountHorseAnims[1];
                }
                if (this->mountSide >= 0) {
                    entry++;
                }

                riderOffsetX = entry->riderOffsetX;
                riderOffsetZ = entry->riderOffsetZ;

                this->actor.world.pos.x = rideActor->actor.world.pos.x + rideActor->riderPos.x +
                                          ((riderOffsetX * sp28) + (riderOffsetZ * sp24));
                this->actor.world.pos.z = rideActor->actor.world.pos.z + rideActor->riderPos.z +
                                          ((riderOffsetZ * sp28) - (riderOffsetX * sp24));

                this->unk_B48 = rideActor->actor.world.pos.y - this->actor.world.pos.y;

                this->currentYaw = this->actor.shape.rot.y = rideActor->actor.shape.rot.y;

                Actor_MountHorse(play, this, &rideActor->actor);
                Player_AnimationPlayOnce(play, this, entry->anim);
                Player_SetupAnimMovement(play, this, 1 | ANIM_FLAG_UPDATE_Y | 8 | ANIM_FLAG_NOMOVE | 0x80);
                this->actor.parent = this->rideActor;
                Player_ClearAttentionModeAndStopMoving(this);
                Actor_DeactivateLens(play);

                return true;
            }
        }
    }

    return false;
}

LinkAnimationHeader* D_8085D264[] = {
    &gPlayerAnim_link_normal_down_slope_slip,
    &gPlayerAnim_link_normal_up_slope_slip,
};

s32 func_8083CF68(PlayState* play, Player* this) {
    if (!Player_InBlockingCsMode(play, this) && !(this->cylinder.base.ocFlags1 & 2)) {
        if ((Player_SlipOnSlope != this->actionFunc) && (func_80857BE8 != this->actionFunc) && (D_80862B40 == 1)) {
            s16 playerVelYaw = Math_FAtan2F(this->actor.velocity.z, this->actor.velocity.x);
            Vec3f slopeNormal;
            s16 downwardSlopeYaw;
            s16 velYawToDownwardSlope;
            f32 slopeSlowdownSpeed;
            f32 temp_fv1;
            f32 var_fa1;
            f32 slopeSlowdownSpeedStep;

            Actor_GetSlopeDirection(this->actor.floorPoly, &slopeNormal, &downwardSlopeYaw);
            velYawToDownwardSlope = downwardSlopeYaw - playerVelYaw;

            if (ABS_ALT(velYawToDownwardSlope) > 0x3E80) { // 87.9 degrees
                var_fa1 = (func_80857BE8 == this->actionFunc) ? Math_CosS(this->angleToFloorX) : slopeNormal.y;
                slopeSlowdownSpeed = (1.0f - var_fa1) * 40.0f;
                temp_fv1 = fabsf(this->actor.speedXZ) + slopeSlowdownSpeed;
                slopeSlowdownSpeedStep = SQ(temp_fv1) * 0.011f;
                slopeSlowdownSpeedStep = CLAMP_MIN(slopeSlowdownSpeedStep, 2.2f);

                // slows down speed as player is climbing a slope
                this->pushedYaw = downwardSlopeYaw;
                Math_StepToF(&this->pushedSpeed, slopeSlowdownSpeed, slopeSlowdownSpeedStep);
            } else {
                Player_SetAction(play, this, Player_SlipOnSlope, 0);
                Player_ResetAttributesAndHeldActor(play, this);
                Player_ChangeAnimShortMorphLoop(play, this, D_8085D264[this->genericVar]);
                this->linearVelocity = sqrtf(SQXZ(this->actor.velocity));
                this->currentYaw = downwardSlopeYaw;
                if (D_80862B28 >= 0) {
                    this->genericVar = 1;
                    Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_HANG);
                }

                return true;
            }
        }
    }

    return false;
}

void Player_PickupItemDrop(PlayState* play, Player* this, GetItemEntry* giEntry) {
    Item00Type dropType = GIFIELD_GET_DROP_TYPE(giEntry->field);

    if (!(giEntry->field & GIFIELD_NO_COLLECTIBLE)) {
        Item_DropCollectible(play, &this->actor.world.pos, dropType | 0x8000);

        if ((dropType == ITEM00_BOMBS_A) || (dropType == ITEM00_ARROWS_30) || (dropType == ITEM00_ARROWS_40) ||
            (dropType == ITEM00_ARROWS_50) || (dropType == ITEM00_RUPEE_GREEN) || (dropType == ITEM00_RUPEE_BLUE) ||
            (dropType == ITEM00_RUPEE_RED) || (dropType == ITEM00_RUPEE_PURPLE) || (dropType == ITEM00_RUPEE_HUGE)) {
            return;
        }
    }

    Item_Give(play, giEntry->itemId);
    play_sound((this->getItemId < 0) ? NA_SE_SY_GET_BOXITEM : NA_SE_SY_GET_ITEM);
}

s32 Player_SetupGetItemOrHoldBehavior(Player* this, PlayState* play) {
    if (gSaveContext.save.playerData.health != 0) {
        Actor* interactRangeActor = this->interactRangeActor;

        if (interactRangeActor != NULL) {
            if (this->getItemId > GI_NONE) {
                if (this->getItemId < GI_MAX) {
                    GetItemEntry* giEntry = &sGetItemTable[this->getItemId - 1];

                    interactRangeActor->parent = &this->actor;
                    if ((Item_CheckObtainability(giEntry->itemId) == ITEM_NONE) ||
                        ((s16)giEntry->objectId == OBJECT_GI_BOMB_2)) {
                        Player_DetatchHeldActor(play, this);
                        Player_LoadGetItemObject(this, giEntry->objectId);

                        if (!(this->stateFlags2 & PLAYER_STATE2_DIVING) ||
                            (this->currentBoots == PLAYER_BOOTS_ZORA_UNDERWATER)) {
                            func_80838760(this);
                            Player_SetupMiniCsFunc(play, this, Player_SetupGetItem, play->playerActorCsIds[1]);
                            Player_PlayAnimOnceSlowed(play, this,
                                                      (this->transformation == PLAYER_FORM_DEKU)
                                                          ? &gPlayerAnim_pn_getB
                                                          : &gPlayerAnim_link_demo_get_itemB);
                        }

                        this->stateFlags1 |=
                            (PLAYER_STATE1_GETTING_ITEM | PLAYER_STATE1_HOLDING_ACTOR | PLAYER_STATE1_IN_CUTSCENE);
                        Player_ClearAttentionModeAndStopMoving(this);

                        return true;
                    }

                    Player_PickupItemDrop(play, this, giEntry);
                    this->getItemId = GI_NONE;
                }
            } else if (this->csMode == PLAYER_CSMODE_0) {
                if (!(this->stateFlags1 & PLAYER_STATE1_HOLDING_ACTOR)) {
                    if (this->getItemId != GI_NONE) {
                        if (CHECK_BTN_ALL(sPlayerControlInput->press.button, BTN_A)) {
                            GetItemEntry* giEntry = &sGetItemTable[-this->getItemId - 1];
                            EnBox* chest = (EnBox*)interactRangeActor;

                            if ((giEntry->itemId != ITEM_NONE) &&
                                (((Item_CheckObtainability(giEntry->itemId) == ITEM_NONE) &&
                                  (giEntry->field & GIFIELD_40)) ||
                                 (((Item_CheckObtainability(giEntry->itemId) != ITEM_NONE)) &&
                                  (giEntry->field & GIFIELD_20)))) {
                                this->getItemId =
                                    (giEntry->itemId == ITEM_MASK_CAPTAIN) ? -GI_RECOVERY_HEART : -GI_RUPEE_BLUE;
                                giEntry = &sGetItemTable[-this->getItemId - 1];
                            }

                            func_80832558(play, this, Player_SetupGetItem);
                            this->stateFlags1 |=
                                (PLAYER_STATE1_GETTING_ITEM | PLAYER_STATE1_HOLDING_ACTOR | PLAYER_STATE1_IN_CUTSCENE);
                            Player_LoadGetItemObject(this, giEntry->objectId);

                            this->actor.world.pos.x =
                                interactRangeActor->world.pos.x -
                                (Math_SinS(interactRangeActor->shape.rot.y) * this->ageProperties->unk_9C);
                            this->actor.world.pos.z =
                                interactRangeActor->world.pos.z -
                                (Math_CosS(interactRangeActor->shape.rot.y) * this->ageProperties->unk_9C);
                            this->actor.world.pos.y = interactRangeActor->world.pos.y;
                            this->currentYaw = this->actor.shape.rot.y = interactRangeActor->shape.rot.y;

                            Player_ClearAttentionModeAndStopMoving(this);
                            if ((giEntry->itemId != ITEM_NONE) && (giEntry->gid >= 0) &&
                                (Item_CheckObtainability(giEntry->itemId) == ITEM_NONE)) {
                                this->unk_A86 = chest->cutsceneIdxB;
                                Player_PlayAnimOnceSlowed(play, this, this->ageProperties->openChestAnim);
                                Player_SetupAnimMovement(play, this,
                                                         1 | ANIM_FLAG_UPDATE_Y | 4 | 8 | ANIM_FLAG_NOMOVE | 0x80);
                                this->actor.bgCheckFlags &= ~0x20;
                                chest->unk_1EC = 1;
                            } else {
                                Player_AnimationPlayOnce(play, this, &gPlayerAnim_link_normal_box_kick);
                                chest->unk_1EC = -1;
                            }

                            return true;
                        }
                    } else if (!(this->stateFlags1 & PLAYER_STATE1_SWIMMING) &&
                               (this->transformation != PLAYER_FORM_DEKU)) {
                        if ((this->heldActor == NULL) || Player_IsHoldingHookshot(this)) {
                            EnBom* bomb = (EnBom*)interactRangeActor;

                            if (((this->transformation != PLAYER_FORM_GORON) &&
                                 (((bomb->actor.id == ACTOR_EN_BOM) && bomb->isPowderKeg) ||
                                  ((interactRangeActor->id == ACTOR_EN_ISHI) && (interactRangeActor->params & 1)) ||
                                  (interactRangeActor->id == ACTOR_EN_MM)))) {
                                return false;
                            }

                            this->stateFlags2 |= PLAYER_STATE2_10000;

                            if (CHECK_BTN_ALL(sPlayerControlInput->press.button, BTN_A)) {
                                func_80832558(play, this, Player_SetupHoldActor);
                                Player_ClearAttentionModeAndStopMoving(this);
                                this->stateFlags1 |= PLAYER_STATE1_HOLDING_ACTOR;

                                return true;
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}

// Player_SetAction_Throwing
void Player_SetupStartThrowActor(Player* this, PlayState* play) {
    Player_SetAction(play, this, Player_StartThrowActor, 1);
    Player_AnimationPlayOnce(play, this, D_8085BE84[PLAYER_ANIMGROUP_28][this->modelAnimType]);
}

// Determines whether a held actor should be dropped or thrown: false implies droppable.
s32 Player_CanThrowActor(Player* this, Actor* heldActor) {
    if ((heldActor != NULL) && !(heldActor->flags & ACTOR_FLAG_800000) &&
        ((this->linearVelocity < 1.1f) || (heldActor->id == ACTOR_EN_BOM_CHU))) {
        return false;
    }
    return true;
}

s32 Player_SetupPutDownOrThrowActor(Player* this, PlayState* play) {
    if (this->stateFlags1 & PLAYER_STATE1_HOLDING_ACTOR) {
        if ((this->heldActor != NULL) &&
            CHECK_BTN_ANY(sPlayerControlInput->press.button, BTN_CRIGHT | BTN_CLEFT | BTN_CDOWN | BTN_B | BTN_A)) {
            if (!Player_InterruptHoldingActor(play, this, this->heldActor)) {
                if (!Player_CanThrowActor(this, this->heldActor)) {
                    Player_SetAction(play, this, func_8084E58C, 1);
                    Player_AnimationPlayOnce(play, this, D_8085BE84[PLAYER_ANIMGROUP_29][this->modelAnimType]);
                    return true;
                }
                Player_SetupStartThrowActor(this, play);
            }

            return true;
        }
    }
    return false;
}

s32 func_8083D860(Player* this, PlayState* play) {
    if ((this->wallHeight >= 79.0f) &&
        (!(this->stateFlags1 & PLAYER_STATE1_SWIMMING) || (this->currentBoots == PLAYER_BOOTS_ZORA_UNDERWATER) ||
         (this->actor.depthInWater < this->ageProperties->unk_2C))) {
        s32 var_t0 = (sPlayerCurrentWallFlags & WALL_FLAG_3) ? 2 : 0; // sp8C
        s32 temp_t2 = sPlayerCurrentWallFlags & WALL_FLAG_1;          // sp80

        if ((var_t0 != 0) || temp_t2 ||
            SurfaceType_CheckWallFlag2(&play->colCtx, this->actor.wallPoly, this->actor.wallBgId)) {
            CollisionPoly* wallPoly = this->actor.wallPoly;
            f32 sp78; // var_fa1_2; // sp78
            f32 sp74; // var_ft4_2 // sp74
            f32 zOut; // var_fa0;
            f32 yOut; // var_ft5_2 // sp6C
            Vec3f sp48[3];
            s32 i;
            f32 sp40;
            Vec3f* sp3C;
            f32 xOut;

            yOut = xOut = 0.0f;
            if (var_t0 != 0) {
                sp78 = this->actor.world.pos.x;
                sp74 = this->actor.world.pos.z;
            } else {
                // s32 pad;

                sp3C = sp48;
                CollisionPoly_GetVerticesByBgId(wallPoly, this->actor.wallBgId, &play->colCtx, sp48);
                sp78 = xOut = sp48[0].x;
                sp74 = zOut = sp48[0].z;
                yOut = sp48[0].y;

                for (i = 1; i < ARRAY_COUNT(sp48); i++) {
                    sp3C++;

                    if (sp78 > sp3C->x) {
                        sp78 = sp3C->x;
                    } else if (xOut < sp3C->x) {
                        xOut = sp3C->x;
                    }

                    if (sp74 > sp3C->z) {
                        sp74 = sp3C->z;
                    } else if (zOut < sp3C->z) {
                        zOut = sp3C->z;
                    }

                    if (yOut > sp3C->y) {
                        yOut = sp3C->y;
                    }
                }

                sp78 = (sp78 + xOut) * 0.5f;
                sp74 = (sp74 + zOut) * 0.5f;

                xOut = ((this->actor.world.pos.x - sp78) * COLPOLY_GET_NORMAL(wallPoly->normal.z)) -
                       ((this->actor.world.pos.z - sp74) * COLPOLY_GET_NORMAL(wallPoly->normal.x));

                sp40 = this->actor.world.pos.y - yOut;
                yOut = ((s32)((sp40 / 15.0f) + 0.5f) * 15.0f) - sp40;
                xOut = fabsf(xOut);
            }

            if (xOut < 8.0f) {
                f32 wallPolyNormalX = COLPOLY_GET_NORMAL(wallPoly->normal.x);
                f32 wallPolyNormalZ = COLPOLY_GET_NORMAL(wallPoly->normal.z);
                f32 wallDistance = this->wallDistance;
                LinkAnimationHeader* anim;

                func_80832558(play, this, func_80837C20);

                this->stateFlags1 |= PLAYER_STATE1_200000;
                this->stateFlags1 &= ~PLAYER_STATE1_SWIMMING;

                if ((var_t0 != 0) || temp_t2) {
                    if ((this->genericVar = var_t0) != 0) {
                        if (this->actor.bgCheckFlags & 1) {
                            anim = &gPlayerAnim_link_normal_Fclimb_startA;

                        } else {
                            anim = &gPlayerAnim_link_normal_Fclimb_hold2upL;
                        }
                        wallDistance = (this->ageProperties->unk_3C + 4.0f) - wallDistance;
                    } else {
                        anim = this->ageProperties->unk_AC;
                        wallDistance = 20.5f;
                    }

                    this->genericTimer = -2;
                    this->actor.world.pos.y += yOut;

                    this->actor.shape.rot.y = this->currentYaw = this->actor.wallYaw + 0x8000;
                } else {
                    anim = this->ageProperties->unk_B0;
                    wallDistance = (this->ageProperties->unk_38 - this->ageProperties->unk_3C) + 17.0f;
                    this->genericTimer = -4;

                    this->actor.shape.rot.y = this->currentYaw = i = this->actor.wallYaw; //! FAKE
                }

                this->actor.world.pos.x = (wallDistance * wallPolyNormalX) + sp78;
                this->actor.world.pos.z = (wallDistance * wallPolyNormalZ) + sp74;
                Player_ClearAttentionModeAndStopMoving(this);
                Math_Vec3f_Copy(&this->actor.prevPos, &this->actor.world.pos);
                Player_AnimationPlayOnce(play, this, anim);
                Player_SetupAnimMovement(play, this, 1 | ANIM_FLAG_UPDATE_Y | 4 | 8 | ANIM_FLAG_NOMOVE | 0x80);
                return true;
            }
        }
    }

    return false;
}

void Player_SetupEndClimb(Player* this, LinkAnimationHeader* anim, PlayState* play) {
    Player_SetActionFuncPreserveMoveFlags(play, this, Player_EndClimb, 0);
    LinkAnimation_PlayOnceSetSpeed(play, &this->skelAnime, anim, 4.0f / 3.0f);
}

s32 Player_SetPositionAndYawOnClimbWall(PlayState* play, Player* this, f32 yOffset, f32 xzDistToWall, f32 xzCheckBScale,
                                        f32 xzCheckAScale) {
    CollisionPoly* wallPoly;
    s32 bgId;
    Vec3f checkPosA;
    Vec3f checkPosB;
    Vec3f posResult;
    f32 yawCos = Math_CosS(this->actor.shape.rot.y);
    f32 yawSin = Math_SinS(this->actor.shape.rot.y);

    checkPosA.x = this->actor.world.pos.x + (xzCheckAScale * yawSin);
    checkPosA.z = this->actor.world.pos.z + (xzCheckAScale * yawCos);
    checkPosB.x = this->actor.world.pos.x + (xzCheckBScale * yawSin);
    checkPosB.z = this->actor.world.pos.z + (xzCheckBScale * yawCos);
    checkPosA.y = checkPosB.y = this->actor.world.pos.y + yOffset;

    if (BgCheck_EntityLineTest2(&play->colCtx, &checkPosA, &checkPosB, &posResult, &this->actor.wallPoly, true, false,
                                false, true, &bgId, &this->actor)) {
        f32 wallPolyNormalX;
        f32 wallPolyNormalZ;

        wallPoly = this->actor.wallPoly;
        this->actor.bgCheckFlags |= BGCHECKFLAG_PLAYER_WALL_INTERACT;
        this->actor.wallBgId = bgId;
        sPlayerCurrentWallFlags = SurfaceType_GetWallFlags(&play->colCtx, wallPoly, bgId);

        wallPolyNormalX = COLPOLY_GET_NORMAL(wallPoly->normal.x);
        wallPolyNormalZ = COLPOLY_GET_NORMAL(wallPoly->normal.z);

        Math_ScaledStepToS(&this->actor.shape.rot.y, Math_FAtan2F(-wallPolyNormalZ, -wallPolyNormalX), 0x320);

        this->currentYaw = this->actor.shape.rot.y;
        this->actor.world.pos.x = posResult.x - (Math_SinS(this->actor.shape.rot.y) * xzDistToWall);
        this->actor.world.pos.z = posResult.z - (Math_CosS(this->actor.shape.rot.y) * xzDistToWall);

        return true;
    }

    this->actor.bgCheckFlags &= ~BGCHECKFLAG_PLAYER_WALL_INTERACT;
    return false;
}

void Player_PushPullSetPositionAndYaw(PlayState* play, Player* this) {
    f32 temp_fv0 = this->ageProperties->unk_38;

    Player_SetPositionAndYawOnClimbWall(play, this, 26.800001f, temp_fv0 + 5.0f, temp_fv0 + 15.0f, 0.0f);
}

void func_8083DF38(Player* this, LinkAnimationHeader* anim, PlayState* play) {
    if (!func_80832558(play, this, func_80837BF8)) {
        Player_SetAction(play, this, Player_GrabPushPullWall, 0);
    }

    Player_AnimationPlayOnce(play, this, anim);
    Player_ClearAttentionModeAndStopMoving(this);

    this->actor.shape.rot.y = this->currentYaw = this->actor.wallYaw + 0x8000;
}

s32 func_8083DFC4(Player* this, PlayState* play) {
    if (!(this->stateFlags1 & PLAYER_STATE1_HOLDING_ACTOR) && (this->actor.bgCheckFlags & 0x200) &&
        (sYawToTouchedWall < 0x3000)) {
        if ((this->linearVelocity > 0.0f) && func_8083D860(this, play)) {
            return true;
        }

        if (!Player_IsSwimming(this) && ((this->linearVelocity == 0.0f) || !(this->stateFlags2 & PLAYER_STATE2_4)) &&
            (sPlayerCurrentWallFlags & WALL_FLAG_6) && (this->actor.bgCheckFlags & 1) && (this->wallHeight >= 39.0f)) {
            this->stateFlags2 |= PLAYER_STATE2_1;

            if (CHECK_BTN_ALL(sPlayerControlInput->cur.button, BTN_A)) {
                DynaPolyActor* dyna;

                if ((this->actor.wallBgId != BGCHECK_SCENE) &&
                    ((dyna = DynaPoly_GetActor(&play->colCtx, this->actor.wallBgId)) != NULL)) {
                    this->rightHandActor = &dyna->actor;
                } else {
                    this->rightHandActor = NULL;
                }

                func_8083DF38(this, &gPlayerAnim_link_normal_push_wait, play);
                return true;
            }
        }
    }

    return false;
}

s32 Player_SetupPushPullWallIdle(PlayState* play, Player* this) {
    if ((this->actor.bgCheckFlags & 0x200) &&
        ((this->stateFlags2 & PLAYER_STATE2_10) || CHECK_BTN_ALL(sPlayerControlInput->cur.button, BTN_A))) {
        DynaPolyActor* var_v1 = NULL;

        if (this->actor.wallBgId != BGCHECK_SCENE) {
            var_v1 = DynaPoly_GetActor(&play->colCtx, this->actor.wallBgId);
        }

        if (&var_v1->actor == this->rightHandActor) {
            if (this->stateFlags2 & PLAYER_STATE2_10) {
                return true;
            }
            return false;
        }
    }

    Player_ReturnToStandStill(this, play);
    Player_AnimationPlayOnce(play, this, &gPlayerAnim_link_normal_push_wait_end);
    this->stateFlags2 &= ~PLAYER_STATE2_10;
    return true;
}

void Player_SetupPushWall(Player* this, PlayState* play) {
    Player_SetAction(play, this, func_8084EE50, 0);
    Player_AnimationPlayOnce(play, this, &gPlayerAnim_link_normal_push_start);
    this->stateFlags2 |= PLAYER_STATE2_10;
}

void Player_SetupPullWall(Player* this, PlayState* play) {
    Player_SetAction(play, this, func_8084EF9C, 0);
    Player_AnimationPlayOnce(play, this, D_8085BE84[PLAYER_ANIMGROUP_34][this->modelAnimType]);
    this->stateFlags2 |= PLAYER_STATE2_10;
}

void func_8083E2F4(Player* this, PlayState* play) {
    this->stateFlags1 &= ~(PLAYER_STATE1_200000 | PLAYER_STATE1_SWIMMING);
    func_80833AA0(this, play);

    if (this->transformation == PLAYER_FORM_DEKU) {
        this->linearVelocity = -1.7f;
    } else {
        this->linearVelocity = -0.4f;
    }
}

s32 func_8083E354(Player* this, PlayState* play) {
    if (!CHECK_BTN_ALL(sPlayerControlInput->press.button, BTN_A) && (this->actor.bgCheckFlags & 0x200)) {
        if ((sPlayerCurrentWallFlags & WALL_FLAG_3) || (sPlayerCurrentWallFlags & WALL_FLAG_1) ||
            SurfaceType_CheckWallFlag2(&play->colCtx, this->actor.wallPoly, this->actor.wallBgId)) {
            return false;
        }
    }

    func_8083E2F4(this, play);
    Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_AUTO_JUMP);
    return true;
}

s32 func_8083E404(Player* this, f32 arg1, s16 arg2) {
    f32 sp1C = BINANG_SUB(arg2, this->actor.shape.rot.y);
    f32 temp_fv1;

    if (this->targetedActor != NULL) {
        Player_LookAtTargetActor(this, Actor_PlayerIsAimingReadyFpsItem(this) || Player_IsAimingReadyZoraFins(this));
    }

    temp_fv1 = fabsf(sp1C) / 0x8000;
    if (((SQ(temp_fv1) * 50.0f) + 6.0f) < arg1) {
        return 1;
    }

    if ((((1.0f - temp_fv1) * 10.0f) + 6.8f) < arg1) {
        return -1;
    }
    return 0;
}

s32 func_8083E514(Player* this, f32* arg2, s16* arg3, PlayState* play) {
    s16 temp_v1 = *arg3 - this->targetYaw;
    u16 var_a2;

    var_a2 = ABS_ALT(temp_v1);

    if ((Actor_PlayerIsAimingReadyFpsItem(this) || Player_IsAimingReadyZoraFins(this)) &&
        (this->targetedActor == NULL)) {
        *arg2 *= Math_SinS(var_a2);

        if (*arg2 != 0.0f) {
            *arg3 = (((temp_v1 >= 0) ? 1 : -1) * 0x4000) + this->actor.shape.rot.y;
        } else {
            *arg3 = this->actor.shape.rot.y;
        }

        if (this->targetedActor != NULL) {
            Player_LookAtTargetActor(this, true);
        } else {
            Math_SmoothStepToS(&this->actor.focus.rot.x, (sPlayerControlInput->rel.stick_y * 240.0f), 0xE, 0xFA0, 0x1E);
            Player_UpdateLookAngles(this, true);
        }
    } else {
        if (this->targetedActor != NULL) {
            return func_8083E404(this, *arg2, *arg3);
        }

        Player_SetLookAngle(this, play);
        if ((*arg2 != 0.0f) && (var_a2 < 0x1770)) {
            return 1;
        }

        if ((Math_SinS(0x4000 - (var_a2 >> 1)) * 200.0f) < *arg2) {
            return -1;
        }
    }
    return 0;
}

s32 Player_GetPushPullDirection(Player* this, f32* arg1, s16* arg2) {
    f32 temp_fv0;
    u16 temp_v0;
    s16 var_v1;

    var_v1 = *arg2 - this->actor.shape.rot.y;
    temp_v0 = ABS_ALT(var_v1);
    temp_fv0 = Math_CosS(temp_v0);
    *arg1 *= temp_fv0;

    // Can't be (*arg1 != 0.0f)
    if (*arg1 != 0) {
        if (temp_fv0 > 0.0f) {
            return 1;
        }
        return -1;
    }
    return 0;
}

s32 Player_GetSpinAttackMoveDirection(Player* this, f32* arg1, s16* arg2, PlayState* play) {
    Player_SetLookAngle(this, play);

    if ((*arg1 != 0.0f) || (ABS_ALT(this->unk_B4C) > 0x190)) {
        s16 temp_a0 = *arg2 - (u16)Camera_GetInputDirYaw(play->cameraPtrs[play->activeCamId]);
        u16 temp;

        temp = (ABS_ALT(temp_a0) - 0x2000);
        if ((temp < 0x4000) || (this->unk_B4C != 0)) {
            return -1;
        }
        return 1;
    }

    return 0;
}

void func_8083E8E0(Player* this, f32 arg1, s16 arg2) {
    s16 temp = arg2 - this->actor.shape.rot.y;

    if (arg1 > 0.0f) {
        if (temp < 0) {
            this->leftRightBlendWeightTarget = 0.0f;
        } else {
            this->leftRightBlendWeightTarget = 1.0f;
        }
    }

    Math_StepToF(&this->leftRightBlendWeight, this->leftRightBlendWeightTarget, 0.3f);
}

void func_8083E958(PlayState* play, Player* this) {
    LinkAnimation_BlendToJoint(play, &this->skelAnime, Player_GetFightingRightAnim(this), this->walkFrame,
                               Player_GetFightingLeftAnim(this), this->walkFrame, this->leftRightBlendWeight,
                               this->blendTableBuffer);
}

s32 func_8083E9C4(f32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    f32 temp_fv0;

    if ((arg3 == 0.0f) && (arg1 > 0.0f)) {
        arg3 = arg2;
    }
    temp_fv0 = (arg0 + arg1) - arg3;
    if (((temp_fv0 * arg1) >= 0.0f) && (((temp_fv0 - arg1) * arg1) < 0.0f)) {
        return true;
    }
    return false;
}

void Player_SetupWalkSfx(Player* this, f32 arg1) {
    s32 sp24;
    f32 updateScale = R_UPDATE_RATE / 2.0f;

    arg1 *= updateScale;
    if (arg1 < -7.25f) {
        arg1 = -7.25f;
    } else if (arg1 > 7.25f) {
        arg1 = 7.25f;
    }

    sp24 = func_8083E9C4(this->walkFrame, arg1, 29.0f, 10.0f);

    if (sp24 || func_8083E9C4(this->walkFrame, arg1, 29.0f, 24.0f)) {
        Player_AnimSfx_PlayFloorWalk(this, this->linearVelocity);
        if (this->linearVelocity > 4.0f) {
            this->stateFlags2 |= PLAYER_STATE2_8;
        }
        this->actor.shape.unk_17 = sp24 ? 1 : 2;
    }

    this->walkFrame += arg1;
    if (this->walkFrame < 0.0f) {
        this->walkFrame += 29.0f;
    } else if (this->walkFrame >= 29.0f) {
        this->walkFrame -= 29.0f;
    }
}

void Player_ChooseIdleAnim(PlayState* play, Player* this) {
    LinkAnimationHeader* anim;
    u32 healthIsCritical;
    LinkAnimationHeader** animPtr;
    s32 animIndex;
    s32 rand;
    f32 morphFrames;
    s16 lastFrame;

    if (((this->actor.id != ACTOR_PLAYER) && !(healthIsCritical = (this->actor.colChkInfo.health < 0x64))) ||
        ((this->actor.id == ACTOR_PLAYER) &&
         (((this->targetedActor != NULL) ||
           ((this->transformation != PLAYER_FORM_FIERCE_DEITY) && (this->transformation != PLAYER_FORM_HUMAN)) ||
           (this->currentMask == PLAYER_MASK_SCENTS)) ||
          (!(healthIsCritical = LifeMeter_IsCritical()) && (this->unk_AA4 = ((this->unk_AA4 + 1) & 1)))))) {
        this->stateFlags2 &= ~PLAYER_STATE2_10000000;
        anim = Player_GetStandingStillAnim(this);
    } else {
        this->stateFlags2 |= PLAYER_STATE2_10000000;

        if (this->stateFlags1 & PLAYER_STATE1_HOLDING_ACTOR) {
            anim = Player_GetStandingStillAnim(this);
        } else {
            animIndex = play->roomCtx.curRoom.unk2;

            if (healthIsCritical) {
                if (this->unk_AA4 >= 0) {
                    animIndex = 7;
                    this->unk_AA4 = -1;
                } else {
                    animIndex = 8;
                }
            } else {
                rand = Rand_ZeroOne() * 5.0f;
                if (rand < 4) {
                    if (((rand != 0) && (rand != 3)) || ((this->rightHandType == PLAYER_MODELTYPE_RH_SHIELD) &&
                                                         ((rand == 3) || (Player_GetMeleeWeaponHeld(this) != 0)))) {
                        if ((rand == 0) && Player_IsHoldingTwoHandedWeapon(this)) {
                            rand = 4;
                        }
                        animIndex = rand + 9;
                    }
                }
            }

            animPtr = &sPlayerIdleAnimations[animIndex][0];
            if (this->modelAnimType != PLAYER_ANIMTYPE_1) {
                animPtr = &sPlayerIdleAnimations[animIndex][1];
            }
            anim = *animPtr;
        }
    }

    lastFrame = Animation_GetLastFrame(anim);
    if ((this->skelAnime.animation == anim) || (this->skelAnime.animation == &gPlayerAnim_pz_attackAend) ||
        (this->skelAnime.animation == &gPlayerAnim_pz_attackBend) ||
        (this->skelAnime.animation == &gPlayerAnim_pz_attackCend)) {
        morphFrames = 0.0f;
    } else {
        morphFrames = -6.0f;
    }

    LinkAnimation_Change(play, &this->skelAnime, anim, (2.0f / 3.0f) * sWaterSpeedScale, 0.0f, lastFrame, ANIMMODE_ONCE,
                         morphFrames);
}

void func_8083EE60(Player* this, PlayState* play) {
    f32 temp_fv0;
    f32 var_fs0;

    if (this->unk_B34 < 1.0f) {
        f32 temp_fs0 = R_UPDATE_RATE / 2.0f;

        Player_SetupWalkSfx(this, REG(35) / 1000.0f);
        LinkAnimation_LoadToJoint(play, &this->skelAnime, D_8085BE84[PLAYER_ANIMGROUP_30][this->modelAnimType],
                                  this->walkFrame);
        this->unk_B34 += (1.0f * 1.0f) * temp_fs0;
        if (this->unk_B34 >= 1.0f) {
            this->unk_B34 = 1.0f;
        }
        var_fs0 = this->unk_B34;
    } else {
        temp_fv0 = this->linearVelocity - (REG(48) / 100.0f);

        if (temp_fv0 < 0.0f) {
            var_fs0 = 1.0f;
            Player_SetupWalkSfx(this, ((REG(35)) / 1000.0f) + (((REG(36)) / 1000.0f) * this->linearVelocity));

            LinkAnimation_LoadToJoint(play, &this->skelAnime, D_8085BE84[PLAYER_ANIMGROUP_30][this->modelAnimType],
                                      this->walkFrame);
        } else {
            var_fs0 = (REG(37) / 1000.0f) * temp_fv0;
            if (var_fs0 < 1.0f) {
                Player_SetupWalkSfx(this, (REG(35) / 1000.0f) + ((REG(36) / 1000.0f) * this->linearVelocity));
            } else {
                var_fs0 = 1.0f;
                Player_SetupWalkSfx(this, (REG(39) / 100.0f) + ((REG(38) / 1000.0f) * temp_fv0));
            }

            LinkAnimation_LoadToMorph(play, &this->skelAnime, D_8085BE84[PLAYER_ANIMGROUP_30][this->modelAnimType],
                                      this->walkFrame);
            LinkAnimation_LoadToJoint(play, &this->skelAnime, &gPlayerAnim_link_normal_back_run,
                                      this->walkFrame * (16.0f / 29.0f));
        }
    }
    if (var_fs0 < 1.0f) {
        LinkAnimation_InterpJointMorph(play, &this->skelAnime, 1.0f - var_fs0);
    }
}

void func_8083F144(Player* this, PlayState* play) {
    Player_SetAction(play, this, func_8084A794, 1);
    Player_ChangeAnimMorphToLastFrame(play, this, &gPlayerAnim_link_normal_back_brake);
}

s32 func_8083F190(Player* this, f32* arg1, s16* arg2, PlayState* play) {
    if (this->linearVelocity > 6.0f) {
        func_8083F144(this, play);
        return true;
    }

    if (*arg1 != 0.0f) {
        if (Player_StepLinearVelocityToZero(this)) {
            *arg1 = 0.0f;
            *arg2 = this->currentYaw;
        } else {
            return true;
        }
    }
    return false;
}

void func_8083F230(Player* this, PlayState* play) {
    Player_SetAction(play, this, func_8084A884, 1);
    Player_AnimationPlayOnce(play, this, &gPlayerAnim_link_normal_back_brake_end);
}

void func_8083F27C(PlayState* play, Player* this) {
    f32 temp_fv0;
    LinkAnimationHeader* sp38;
    LinkAnimationHeader* sp34;

    sp38 = D_8085BE84[PLAYER_ANIMGROUP_23][this->modelAnimType];
    sp34 = D_8085BE84[PLAYER_ANIMGROUP_24][this->modelAnimType];

    this->skelAnime.animation = sp38;

    Player_SetupWalkSfx(this, (REG(30) / 1000.0f) + ((REG(32) / 1000.0f) * this->linearVelocity));

    temp_fv0 = this->walkFrame * (16.0f / 29.0f);
    LinkAnimation_BlendToJoint(play, &this->skelAnime, sp34, temp_fv0, sp38, temp_fv0, this->leftRightBlendWeight,
                               this->blendTableBuffer);
}

void func_8083F358(Player* this, s32 arg1, PlayState* play) {
    LinkAnimationHeader* var_v0;
    f32 var_fv1;
    s16 var_a1;

    if (ABS_ALT(D_80862B28) < 0xE38) {
        var_a1 = 0;
    } else {
        var_a1 = CLAMP(D_80862B28, -0x2AAA, 0x2AAA);
    }

    Math_ScaledStepToS(&this->unk_B70, var_a1, 0x190);
    if ((this->modelAnimType == PLAYER_ANIMTYPE_3) || ((this->unk_B70 == 0) && (this->shapeOffsetY <= 0.0f))) {
        if (arg1 == 0) {
            LinkAnimation_LoadToJoint(play, &this->skelAnime, D_8085BE84[PLAYER_ANIMGROUP_1][this->modelAnimType],
                                      this->walkFrame);
        } else {
            LinkAnimation_LoadToMorph(play, &this->skelAnime, D_8085BE84[PLAYER_ANIMGROUP_1][this->modelAnimType],
                                      this->walkFrame);
        }
        return;
    }

    if (this->unk_B70 != 0) {
        var_fv1 = this->unk_B70 / (f32)0x2AAA;
    } else {
        var_fv1 = this->shapeOffsetY * 0.0006f;
    }

    var_fv1 *= fabsf(this->linearVelocity) * 0.5f;
    if (var_fv1 > 1.0f) {
        var_fv1 = 1.0f;
    }

    if (var_fv1 < 0.0f) {
        var_v0 = &gPlayerAnim_link_normal_climb_down;
        var_fv1 = -var_fv1;
    } else {
        var_v0 = &gPlayerAnim_link_normal_climb_up;
    }

    if (arg1 == 0) {
        LinkAnimation_BlendToJoint(play, &this->skelAnime, D_8085BE84[PLAYER_ANIMGROUP_1][this->modelAnimType],
                                   this->walkFrame, var_v0, this->walkFrame, var_fv1, this->blendTableBuffer);
    } else {
        LinkAnimation_BlendToMorph(play, &this->skelAnime, D_8085BE84[PLAYER_ANIMGROUP_1][this->modelAnimType],
                                   this->walkFrame, var_v0, this->walkFrame, var_fv1, this->blendTableBuffer);
    }
}

void func_8083F57C(Player* this, PlayState* play) {
    f32 temp_fv0;
    f32 var_fs0;

    if (this->unk_B34 < 1.0f) {
        f32 temp_fs0;

        temp_fs0 = R_UPDATE_RATE / 2.0f;
        Player_SetupWalkSfx(this, REG(35) / 1000.0f);
        LinkAnimation_LoadToJoint(play, &this->skelAnime, D_8085BE84[PLAYER_ANIMGROUP_1][this->modelAnimType],
                                  this->walkFrame);

        // required
        this->unk_B34 += 1 * temp_fs0;
        if (this->unk_B34 >= 1.0f) {
            this->unk_B34 = 1.0f;
        }
        var_fs0 = this->unk_B34;
    } else {
        temp_fv0 = (this->linearVelocity - (REG(48) / 100.0f));
        if (temp_fv0 < 0.0f) {
            var_fs0 = 1.0f;
            Player_SetupWalkSfx(this, (REG(35) / 1000.0f) + ((REG(36) / 1000.0f) * this->linearVelocity));
            func_8083F358(this, 0, play);
        } else {
            var_fs0 = (REG(37) / 1000.0f) * temp_fv0;
            if (var_fs0 < 1.0f) {
                Player_SetupWalkSfx(this, (REG(35) / 1000.0f) + ((REG(36) / 1000.0f) * this->linearVelocity));
            } else {
                var_fs0 = 1.0f;
                Player_SetupWalkSfx(this, (REG(39) / 100.0f) + ((REG(38) / 1000.0f) * temp_fv0));
            }
            func_8083F358(this, 1, play);
            LinkAnimation_LoadToJoint(play, &this->skelAnime, func_8082EEE0(this), this->walkFrame * (20.0f / 29.0f));
        }
    }

    if (var_fs0 < 1.0f) {
        LinkAnimation_InterpJointMorph(play, &this->skelAnime, 1.0f - var_fs0);
    }
}

void func_8083F828(Vec3f* arg0, Vec3f* arg1, f32 arg2, f32 arg3, f32 arg4) {
    arg1->x = randPlusMinusPoint5Scaled(arg3) + arg0->x;
    arg1->y = randPlusMinusPoint5Scaled(arg4) + (arg0->y + arg2);
    arg1->z = randPlusMinusPoint5Scaled(arg3) + arg0->z;
}

Color_RGBA8 D_8085D26C = { 255, 255, 255, 255 };
Vec3f D_8085D270 = { 0.0f, 0.04f, 0.0f };

s32 func_8083F8A8(PlayState* play, Player* this, f32 radius, s32 countMax, f32 randAccelWeight, s32 scale,
                  s32 scaleStep, s32 useLighting) {
    static Vec3f D_8085D27C = { 0.0f, 0.0f, 0.0f };
    static Vec3f D_8085D288 = { 0.0f, 0.0f, 0.0f };

    if ((countMax < 0) || (this->floorSfxOffset == NA_SE_PL_WALK_SNOW - SFX_FLAG)) {
        s32 count = func_80173B48(&play->state) / 20000000;
        Vec3f pos;
        s32 i;

        count = (count >= ABS_ALT(countMax)) ? ABS_ALT(countMax) : count;
        for (i = 0; i < count; i++) {
            func_8083F828(&this->actor.world.pos, &pos, 0.0f, 40.0f, 10.0f);
            D_8085D27C.x = randPlusMinusPoint5Scaled(3.0f);
            D_8085D27C.z = randPlusMinusPoint5Scaled(3.0f);
            EffectSsDust_Spawn(play, 0, &pos, &D_8085D27C, &D_8085D270, &D_8085D26C, &D_8085D26C, scale, scaleStep, 42,
                               0);
        }

        return true;
    } else if ((this->floorSfxOffset == NA_SE_PL_WALK_GROUND - SFX_FLAG) ||
               (this->floorSfxOffset == NA_SE_PL_WALK_SAND - SFX_FLAG)) {
        s32 count = func_80173B48(&play->state) / 12000000;

        if (count > 0) {
            Actor_SpawnFloorDustRing(play, &this->actor, &this->actor.world.pos, radius,
                                     (count < countMax) ? count : countMax, randAccelWeight, scale, scaleStep,
                                     useLighting);

            return true;
        }
    } else if (this->floorSfxOffset == NA_SE_PL_WALK_GRASS - SFX_FLAG) {
        s32 count = func_80173B48(&play->state) / 12000000;
        Vec3f velocity;
        Vec3f pos;
        s32 i;

        count = (count >= countMax) ? countMax : count;
        for (i = 0; i < count; i++) {
            func_8083F828(&this->actor.world.pos, &pos, 0.0f, 20.0f, 20.0f);
            velocity.x = randPlusMinusPoint5Scaled(3.0f);
            velocity.y = Rand_ZeroFloat(2.0f);
            velocity.z = randPlusMinusPoint5Scaled(3.0f);
            D_8085D288.y = -0.1f;
            EffectSsHahen_Spawn(play, &pos, &velocity, &D_8085D288, 0, 0x96, 1, 0x10, gKakeraLeafTip);
        }
    }

    return false;
}

s32 Player_SetupSpawnDustAtFeet(PlayState* play, Player* this) {
    if ((this->floorSfxOffset == NA_SE_PL_WALK_GROUND - SFX_FLAG) ||
        (this->floorSfxOffset == NA_SE_PL_WALK_SAND - SFX_FLAG)) {
        Vec3f* feetPos = this->actor.shape.feetPos;
        s32 i;

        for (i = 0; i < ARRAY_COUNT(this->actor.shape.feetPos); i++) {
            func_800B1210(play, feetPos, &gZeroVec3f, &gZeroVec3f, 50, 30);
            feetPos++;
        }

        return true;
    }

    if (this->floorSfxOffset == NA_SE_PL_WALK_SNOW - SFX_FLAG) {
        Vec3f* feetPos = this->actor.shape.feetPos;
        s32 i;

        for (i = 0; i < ARRAY_COUNT(this->actor.shape.feetPos); i++) {
            EffectSsDust_Spawn(play, 0, feetPos, &gZeroVec3f, &D_8085D270, &D_8085D26C, &D_8085D26C, 100, 40, 17, 0);
            feetPos++;
        }

        return true;
    }

    return false;
}

s32 Player_SetupMeleeAttack(PlayState* play, Player* this, f32 arg2, f32 arg3, f32 arg4) {
    if (arg4 < this->skelAnime.curFrame) {
        Player_DeactivateMeleeWeapon(this);
    } else if (arg2 <= this->skelAnime.curFrame) {
        this->stateFlags3 |= PLAYER_STATE3_2000000;
        Player_MeleeAttack(play, this, (arg3 <= this->skelAnime.curFrame) ? 1 : -1);
        return true;
    }
    return false;
}

// Crouch-stabbing
s32 func_8083FD80(Player* this, PlayState* play) {
    if (!Player_IsGoronOrDeku(this) && (Player_GetMeleeWeaponHeld(this) != 0) &&
        (this->transformation != PLAYER_FORM_ZORA) && (sUsingItemAlreadyInHand != 0)) {
        //! Calling this function sets the meleeWeaponQuads' damage properties correctly, patching "Power Crouch Stab".
        func_8083375C(this, PLAYER_MWA_STAB_1H);
        Player_AnimationPlayOnce(play, this, &gPlayerAnim_link_normal_defense_kiru);
        this->genericVar = 1;
        this->meleeWeaponAnimation = PLAYER_MWA_STAB_1H;
        this->currentYaw = this->actor.shape.rot.y + this->upperBodyRot.y;
        this->slashCounter = 0;
        return true;
    }
    return false;
}

// Probably want to rename this one..
s32 Player_IsBusy(Player* this, PlayState* play) {
    return Player_SetupItemCutsceneOrFirstPerson(this, play) || Player_SetupSpeakOrCheck(this, play) ||
           Player_SetupGetItemOrHoldBehavior(this, play);
}

// Player_RumbleAndPlaySfx?
void Player_RequestQuakeAndRumble(PlayState* play, Player* this, u16 sfxId) {
    Player_AddQuake(play, 27767, 7, 20);
    Player_RequestRumble(play, this, 255, 20, 150, SQ(0));
    Player_PlaySfx(this, sfxId);
}

void func_8083FEF4(PlayState* play, Player* this) {
    Inventory_ChangeAmmo(ITEM_STICK, -1);
    Player_UseItem(play, this, ITEM_NONE);
}

s32 func_8083FF30(PlayState* play, Player* this) {
    if ((this->itemActionParam == PLAYER_AP_STICK) && (this->unk_B08[1] > 0.5f)) {
        if (AMMO(ITEM_STICK) != 0) {
            EffectSsStick_Spawn(play, &this->bodyPartsPos[PLAYER_BODYPART_R_HAND],
                                BINANG_ADD(this->actor.shape.rot.y, 0x8000));
            this->unk_B08[1] = 0.5f;
            func_8083FEF4(play, this);
            Player_PlaySfx(this, NA_SE_IT_WOODSTICK_BROKEN);
        }
        return true;
    }

    return false;
}

// handles razor sword health and breaking
s32 func_8083FFEC(PlayState* play, Player* this) {
    if (this->itemActionParam == PLAYER_AP_SWORD_RAZOR) {
        if (gSaveContext.save.playerData.swordHealth > 0) {
            gSaveContext.save.playerData.swordHealth--;
            if (gSaveContext.save.playerData.swordHealth <= 0) {
                Item_Give(play, ITEM_SWORD_KOKIRI);
                Player_UseItem(play, this, ITEM_SWORD_KOKIRI);
                Player_PlaySfx(this, NA_SE_IT_MAJIN_SWORD_BROKEN);
                if (Message_GetState(&play->msgCtx) == TEXT_STATE_NONE) {
                    Message_StartTextbox(play, 0xF9, NULL);
                }
            }
        }
        return true;
    }
    return false;
}

// Could return the last function, but never used as such
void func_80840094(PlayState* play, Player* this) {
    func_8083FF30(play, this);
    func_8083FFEC(play, this);
}

LinkAnimationHeader* D_8085D294[] = {
    &gPlayerAnim_link_fighter_rebound,
    &gPlayerAnim_link_fighter_rebound_long,
    &gPlayerAnim_link_fighter_reboundR,
    &gPlayerAnim_link_fighter_rebound_longR,
};

void Player_SetupMeleeWeaponRebound(PlayState* play, Player* this) {
    if (func_8084B5C0 != this->actionFunc) {
        Player_ResetAttributes(play, this);
        if ((this->transformation != PLAYER_FORM_HUMAN) && (this->transformation != PLAYER_FORM_FIERCE_DEITY)) {
            u8 moveFlags = this->skelAnime.moveFlags;
            s32 pad;

            this->skelAnime.moveFlags = 0;
            Player_SetAction(play, this, func_80854C70, 0);
            this->skelAnime.moveFlags = moveFlags;
        } else {
            s32 var_v1;
            s32 pad;

            Player_SetAction(play, this, func_80854C70, 0);
            if (Player_IsUnfriendlyZTargeting(this)) {
                var_v1 = 2;
            } else {
                var_v1 = 0;
            }
            Player_PlayAnimOnceSlowed(play, this, D_8085D294[Player_IsHoldingTwoHandedWeapon(this) + var_v1]);
        }
    }

    Player_RequestRumble(play, this, 180, 20, 100, SQ(0));
    this->linearVelocity = -18.0f;
    func_80840094(play, this);
}

s32 func_808401F4(PlayState* play, Player* this) {
    if (this->meleeWeaponState > 0) {
        s32 temp_v0_3;

        if (this->meleeWeaponAnimation < PLAYER_MWA_SPIN_ATTACK_1H) {
            if (!(this->meleeWeaponQuads[0].base.atFlags & 4) && !(this->meleeWeaponQuads[1].base.atFlags & 4)) {
                if (this->skelAnime.curFrame >= 2.0f) {
                    CollisionPoly* poly;
                    s32 bgId;
                    Vec3f spC8;
                    Vec3f pos;
                    Vec3f spB0;
                    Vec3f* var_a1;
                    Vec3f* temp_a0 = &this->meleeWeaponInfo[0].tip;
                    f32 var_fv1;

                    if (this->linearVelocity >= 0.0f) {
                        var_a1 = &this->meleeWeaponInfo[0].base;
                        if ((this->transformation == PLAYER_FORM_GORON) || (this->actor.id == ACTOR_EN_TEST3)) {
                            var_a1 = &this->unk_AF0[1];
                        }

                        var_fv1 = Math_Vec3f_DistXYZAndStoreDiff(temp_a0, var_a1, &spB0);
                        if (var_fv1 != 0.0f) {
                            var_fv1 = (var_fv1 + 10.0f) / var_fv1;
                        }

                        spC8.x = temp_a0->x + (spB0.x * var_fv1);
                        spC8.y = temp_a0->y + (spB0.y * var_fv1);
                        spC8.z = temp_a0->z + (spB0.z * var_fv1);
                        if (BgCheck_EntityLineTest2(&play->colCtx, &spC8, temp_a0, &pos, &poly, 1, 0, 0, 1, &bgId,
                                                    &this->actor)) {
                            if (!SurfaceType_IsIgnoredByEntities(&play->colCtx, poly, bgId) &&
                                (SurfaceType_GetFloorType(&play->colCtx, poly, bgId) != BG_FLOOR_TYPE_6) &&
                                !func_800B90AC(play, &this->actor, poly, bgId, &pos)) {
                                if (this->transformation == PLAYER_FORM_GORON) {
                                    MtxF sp64;
                                    Vec3s actorRot;
                                    DynaPolyActor* temp_v0;

                                    Player_SetFreezeFlashTimer(play);
                                    Player_RequestQuakeAndRumble(play, this, NA_SE_IT_HAMMER_HIT);
                                    if (this->transformation == PLAYER_FORM_GORON) {
                                        func_800B648C(play, 2, 2, 100.0f, &this->actor.world.pos);
                                        func_800C0094(poly, pos.x, pos.y, pos.z, &sp64);
                                        Matrix_MtxFToYXZRot(&sp64, &actorRot, true);
                                        Actor_Spawn(&play->actorCtx, play, ACTOR_EN_TEST, pos.x, pos.y, pos.z,
                                                    actorRot.x, actorRot.y, actorRot.z, 500);
                                    }

                                    if (bgId != BGCHECK_SCENE) {
                                        temp_v0 = DynaPoly_GetActor(&play->colCtx, bgId);

                                        if ((((this->meleeWeaponQuads[0].base.atFlags & 2) != 0) &&
                                             (&temp_v0->actor == this->meleeWeaponQuads[0].base.at)) ||
                                            ((this->meleeWeaponQuads[1].base.atFlags & 2) &&
                                             (&temp_v0->actor == this->meleeWeaponQuads[1].base.at))) {
                                            return false;
                                        }
                                    }

                                    Player_SetupMeleeWeaponRebound(play, this);
                                    if (this->transformation != PLAYER_FORM_GORON) {
                                        return true;
                                    }
                                    return false;
                                }

                                if (this->linearVelocity >= 0.0f) {
                                    BgSurfaceSfxType sfxType = SurfaceType_GetSfxType(&play->colCtx, poly, bgId);

                                    if (sfxType == BG_SURFACE_SFX_TYPE_10) {
                                        CollisionCheck_SpawnShieldParticlesWood(play, &pos, &this->actor.projectedPos);
                                    } else {
                                        pos.x += 8.0f * COLPOLY_GET_NORMAL(poly->normal.x);
                                        pos.y += 8.0f * COLPOLY_GET_NORMAL(poly->normal.y);
                                        pos.x += 8.0f * COLPOLY_GET_NORMAL(poly->normal.z);
                                        CollisionCheck_SpawnShieldParticles(play, &pos);

                                        if (sfxType == BG_SURFACE_SFX_TYPE_11) {
                                            Player_PlaySfx(this, NA_SE_IT_WALL_HIT_SOFT);
                                        } else {
                                            Player_PlaySfx(this, NA_SE_IT_WALL_HIT_HARD);
                                        }
                                    }

                                    func_80840094(play, this);
                                    Player_RequestRumble(play, this, 180, 20, 100, SQ(0));
                                    this->linearVelocity = -14.0f;
                                }
                            }
                        }
                    }
                }
            } else {
                Player_SetupMeleeWeaponRebound(play, this);
                Player_SetFreezeFlashTimer(play);
                return true;
            }
        }

        temp_v0_3 = (this->meleeWeaponQuads[0].base.atFlags & 2) != 0;
        if (temp_v0_3 || (this->meleeWeaponQuads[1].base.atFlags & 2)) {
            if ((this->meleeWeaponAnimation < PLAYER_MWA_SPIN_ATTACK_1H) &&
                (this->transformation != PLAYER_FORM_GORON)) {
                Actor* temp_v1 = this->meleeWeaponQuads[temp_v0_3 ? 0 : 1].base.at;

                if ((temp_v1 != NULL) && (temp_v1->id != ACTOR_EN_KANBAN)) {
                    Player_SetFreezeFlashTimer(play);
                }
            }

            if (!func_8083FF30(play, this)) {
                func_8083FFEC(play, this);
                if (this->actor.colChkInfo.atHitEffect == 1) {
                    this->actor.colChkInfo.damage = 8;
                    func_80833B18(play, this, 4, 0.0f, 0.0f, this->actor.shape.rot.y, 20);
                    return true;
                }
            }
        }
    }

    return false;
}

Vec3f D_8085D2A4 = { 0.0f, 0.0f, 5.0f };

void func_80840770(PlayState* play, Player* this) {
    if (this->genericTimer != 0) {
        if (this->genericTimer > 0) {
            this->genericTimer--;
            if (this->genericTimer == 0) {
                if (this->stateFlags1 & PLAYER_STATE1_SWIMMING) {
                    LinkAnimation_Change(play, &this->skelAnime, &gPlayerAnim_link_swimer_swim_wait, 1.0f, 0.0f,
                                         Animation_GetLastFrame(&gPlayerAnim_link_swimer_swim_wait), ANIMMODE_ONCE,
                                         -16.0f);
                } else {
                    LinkAnimation_Change(play, &this->skelAnime, &gPlayerAnim_link_derth_rebirth, 1.0f, 99.0f,
                                         Animation_GetLastFrame(&gPlayerAnim_link_derth_rebirth), ANIMMODE_ONCE, 0.0f);
                }
                gSaveContext.healthAccumulator = 0xA0;
                this->genericTimer = -1;
            }
        } else if (gSaveContext.healthAccumulator == 0) {
            func_80838760(this);

            this->stateFlags1 &= ~PLAYER_STATE1_IN_DEATH_CUTSCENE;
            if (this->stateFlags1 & PLAYER_STATE1_SWIMMING) {
                Player_SetupSwimIdle(play, this);
            } else {
                Player_SetupStandingStillMorph(this, play);
            }

            this->unk_D6B = 20;
            Player_SetupInvincibilityNoDamageFlash(this, -20);
            func_801A01C4();
        }
    } else if (this->genericVar != 0) {
        func_80838760(this);
        this->unk_A86 = play->playerActorCsIds[7];
        this->genericTimer = 60;
        Player_SpawnFairy(play, this, &this->actor.world.pos, &D_8085D2A4, 5);
        Player_PlaySfx(this, NA_SE_EV_FIATY_HEAL - SFX_FLAG);
    } else if (play->gameOverCtx.state == 2) {
        play->gameOverCtx.state = 3;
    }
}

void func_80840980(Player* this, u16 sfxId) {
    Player_AnimSfx_PlayVoice(this, sfxId);
}

void Player_ThrowActor(PlayState* play, Player* this, f32 speedXZ, f32 yVelocity) {
    Actor* heldActor = this->heldActor;

    if (!Player_InterruptHoldingActor(play, this, heldActor)) {
        heldActor->world.rot.y = this->actor.shape.rot.y;
        heldActor->speedXZ = speedXZ;
        heldActor->velocity.y = yVelocity;
        Player_SetupHeldItemUpperActionFunc(play, this);
        Player_PlaySfx(this, NA_SE_PL_THROW);
        Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_SWORD_N);
    }
}

// Check if bonked and if so, rumble, play sound, etc.
s32 Player_TryBonkning(PlayState* play, Player* this, f32* arg2, f32 arg3) {
    Actor* cylinderOc = NULL;

    if (arg3 <= *arg2) {
        // If interacting with a wall and close to facing it
        if (((this->actor.bgCheckFlags & 0x200) && (D_80862B24 < 0x1C00)) ||
            // or, impacting something's cylinder
            (((this->cylinder.base.ocFlags1 & OC1_HIT) && (cylinderOc = this->cylinder.base.oc) != NULL) &&
             // and that something is a Beaver Race ring,
             ((cylinderOc->id == ACTOR_EN_TWIG) ||
              // or something is a tree and `this` is close to facing it (note the this actor's facing direction would
              // be antiparallel to the cylinder's actor's yaw if this was directly facing it)
              (((cylinderOc->id == ACTOR_EN_WOOD02) || (cylinderOc->id == ACTOR_EN_SNOWWD) ||
                (cylinderOc->id == ACTOR_OBJ_TREE)) &&
               (ABS_ALT(BINANG_SUB(this->actor.world.rot.y, cylinderOc->yawTowardsPlayer)) > 0x6000))))) {

            if (!func_8082DA90(play)) {
                if (this->doorType == PLAYER_DOORTYPE_STAIRCASE) {
                    Player_SetupStandingStillMorph(this, play);
                    return true;
                }

                if (cylinderOc != NULL) {
                    cylinderOc->home.rot.y = 1;
                } else if (this->actor.wallBgId != BGCHECK_SCENE) { // i.e. was an actor
                    DynaPolyActor* wallPolyActor = DynaPoly_GetActor(&play->colCtx, this->actor.wallBgId);

                    // Large crates, barrels and palm trees
                    if ((wallPolyActor != NULL) &&
                        ((wallPolyActor->actor.id == ACTOR_OBJ_KIBAKO2) ||
                         (wallPolyActor->actor.id == ACTOR_OBJ_TARU) || (wallPolyActor->actor.id == ACTOR_OBJ_YASI))) {
                        wallPolyActor->actor.home.rot.z = 1;
                    }
                }

                if (!(this->stateFlags3 & PLAYER_STATE3_1000)) {
                    if ((this->stateFlags3 & PLAYER_STATE3_8000) && (func_8084CA24 != this->actionFunc)) {
                        Player_SetAction(play, this, func_80851B58, 0);
                        Player_PlayAnimOnceSlowed(play, this, &gPlayerAnim_link_swimer_swim_hit);
                        Player_ResetAttributes(play, this);
                        this->linearVelocity *= 0.2f;
                    } else {
                        Player_SetAction(play, this, Player_Rolling, 0);
                        Player_AnimationPlayOnce(play, this, D_8085BE84[PLAYER_ANIMGROUP_16][this->modelAnimType]);
                        this->genericTimer = 1;
                    }
                }

                this->linearVelocity = -this->linearVelocity;
                Player_AddQuake(play, 33267, 3, 12);
                Player_RequestRumble(play, this, 255, 20, 150, SQ(0));
                func_800B648C(play, 2, 2, 100.0f, &this->actor.world.pos);
                Player_PlaySfx(this, NA_SE_PL_BODY_HIT);
                Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_CLIMB_END);
                return true;
            }
        }
    }
    return false;
}

s32 Player_SetupReleaseSpinAttack(Player* this, PlayState* play) {
    if (Player_SetupCutscene(play, this)) {
        this->stateFlags2 |= PLAYER_STATE2_RELEASING_SPIN_ATTACK;
    } else if (!CHECK_BTN_ALL(sPlayerControlInput->cur.button, BTN_B)) {
        PlayerMeleeWeaponAnimation meleeWeaponAnimation;

        if ((this->unk_B08[0] >= 0.85f) || Player_CanQuickspin(this)) {
            meleeWeaponAnimation = sBigSpinAttackMWAs[Player_IsHoldingTwoHandedWeapon(this)];
        } else {
            meleeWeaponAnimation = sSmallSpinAttackMWAs[Player_IsHoldingTwoHandedWeapon(this)];
        }

        Player_StartMeleeWeaponAttack(play, this, meleeWeaponAnimation);
        Player_SetupInvincibilityNoDamageFlash(this, -8);

        this->stateFlags2 |= PLAYER_STATE2_RELEASING_SPIN_ATTACK;
        if (this->relativeAnalogStickInputs[this->inputFrameCounter] == 0) {
            this->stateFlags2 |= PLAYER_STATE2_ENABLE_FORWARD_SLIDE_FROM_ATTACK;
        }
    } else {
        return false;
    }

    return true;
}

void Player_SetupWalkChargingSpinAttack(Player* this, PlayState* play) {
    Player_SetAction(play, this, Player_WalkChargingSpinAttack, 1);
}

void Player_SetupSidewalkChargingSpinAttack(Player* this, PlayState* play) {
    Player_SetAction(play, this, Player_SidewalkChargingSpinAttack, 1);
}

void Player_CancelSpinAttackCharge(Player* this, PlayState* play) {
    Player_ReturnToStandStill(this, play);
    Player_DeactivateMeleeWeapon(this);
    Player_ChangeAnimMorphToLastFrame(play, this, sCancelSpinAttackChargeAnims[Player_IsHoldingTwoHandedWeapon(this)]);
    this->currentYaw = this->actor.shape.rot.y;
}

void Player_SetupChargeSpinAttack(Player* this, PlayState* play) {
    Player_SetAction(play, this, Player_ChargeSpinAttack, 1);
    Player_AnimationPlayLoop(play, this, sSpinAttackChargeAnims[Player_IsHoldingTwoHandedWeapon(this)]);
    this->genericTimer = 1;
    this->walkFrame = 0.0f;
}

// Spin attack size
void Player_UpdateSpinAttackTimer(Player* this) {
    Math_StepToF(&this->unk_B08[0], (gSaveContext.save.weekEventReg[23] & 2) ? 1.0f : 0.5f, 0.02f);
}

s32 func_80840F90(PlayState* play, Player* this, CsCmdActorAction* playerAction, f32 arg3, s16 arg4, s32 arg5) {
    if ((arg5 != 0) && (this->linearVelocity == 0.0f)) {
        return LinkAnimation_Update(play, &this->skelAnime);
    }

    if (arg5 != 2) {
        f32 halfUpdateRate = R_UPDATE_RATE / 2.0f;
        f32 curDiffX = playerAction->endPos.x - this->actor.world.pos.x;
        f32 curDiffZ = playerAction->endPos.z - this->actor.world.pos.z;
        f32 scaledCurDist = sqrtf(SQ(curDiffX) + SQ(curDiffZ)) / halfUpdateRate;
        s32 framesLeft = (playerAction->endFrame - play->csCtx.frames) + 1;

        arg4 = Math_FAtan2F(curDiffZ, curDiffX);

        if (arg5 == 1) {
            f32 distX = playerAction->endPos.x - playerAction->startPos.x;
            f32 distZ = playerAction->endPos.z - playerAction->startPos.z;
            s32 temp = (((sqrtf(SQ(distX) + SQ(distZ)) / halfUpdateRate) /
                         (playerAction->endFrame - playerAction->startFrame)) /
                        1.5f) *
                       4.0f;
            if (temp >= framesLeft) {
                arg3 = 0.0f;
                arg4 = this->actor.shape.rot.y;
            } else {
                arg3 = scaledCurDist / ((framesLeft - temp) + 1);
            }
        } else {
            arg3 = scaledCurDist / framesLeft;
        }
    }

    this->stateFlags2 |= PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING;
    func_8083F57C(this, play);
    Player_SetRunVelocityAndYaw(this, arg3, arg4);
    if ((arg3 == 0.0f) && (this->linearVelocity == 0.0f)) {
        func_80839CD8(this, play);
    }

    return false;
}

s32 func_808411D4(PlayState* play, Player* this, f32* arg2, s32 arg3) {
    f32 xDiff = this->unk_3A0.x - this->actor.world.pos.x;
    f32 yDiff = this->unk_3A0.z - this->actor.world.pos.z;
    s32 sp2C;
    s32 pad2;
    s16 var_v1;

    sp2C = sqrtf(SQ(xDiff) + SQ(yDiff));
    var_v1 = Math_Vec3f_Yaw(&this->actor.world.pos, &this->unk_3A0);
    if (sp2C < arg3) {
        *arg2 = 0.0f;
        var_v1 = this->actor.shape.rot.y;
    }
    if (func_80840F90(play, this, NULL, *arg2, var_v1, 2)) {
        return 0;
    }
    return sp2C;
}

void Player_InitMode_NoUpdateOrDraw(PlayState* play, Player* this) {
    this->actor.update = Player_DoNothing3;
    this->actor.draw = NULL;
}

void Player_InitMode_BlueWarp(PlayState* play, Player* this) {
    Player_SetAction(play, this, Player_SpawnFromBlueWarp, 0);
    this->stateFlags1 |= PLAYER_STATE1_IN_CUTSCENE;
    LinkAnimation_Change(play, &this->skelAnime, &gPlayerAnim_link_okarina_warp_goal, 2.0f / 3.0f, 0.0f, 24.0f,
                         ANIMMODE_ONCE, 0.0f);
    this->actor.world.pos.y += 800.0f;
}

u8 sSwordItemsByAge[] = {
    ITEM_SWORD_RAZOR,
    ITEM_SWORD_KOKIRI,
};

// OoT leftover?
void Player_CutsceneDrawSword(PlayState* play, Player* this, s32 arg2) {
    ItemId item;
    PlayerActionParam actionParam;

    //! @bug OoB read if player is goron, deku or human
    item = sSwordItemsByAge[this->transformation];
    actionParam = sItemActionParams[item];
    Player_PutAwayHookshot(this);
    Player_DetatchHeldActor(play, this);
    this->heldItemId = item;
    this->nextModelGroup = Player_ActionToModelGroup(this, actionParam);
    Player_ChangeItem(play, this, actionParam);
    Player_SetupHeldItemUpperActionFunc(play, this);
    if (arg2) {
        Player_PlaySfx(this, NA_SE_IT_SWORD_PICKOUT);
    }
}

Vec3f D_8085D2B4 = { -1.0f, 69.0f, 20.0f };

void Player_InitMode_1(PlayState* play, Player* this) {
    Player_SetAction(play, this, func_80852FD4, 0);
    this->stateFlags1 |= PLAYER_STATE1_IN_CUTSCENE;
    Math_Vec3f_Copy(&this->actor.world.pos, &D_8085D2B4);
    this->currentYaw = this->actor.shape.rot.y = -0x8000;
    LinkAnimation_Change(play, &this->skelAnime, this->ageProperties->unk_A8, 2.0f / 3.0f, 0.0f, 0.0f, ANIMMODE_ONCE,
                         0.0f);
    Player_SetupAnimMovement(play, this, (1 | ANIM_FLAG_UPDATE_Y | 4 | 8 | 0x80 | 0x200));
    if (this->transformation == PLAYER_FORM_FIERCE_DEITY) {
        Player_CutsceneDrawSword(play, this, false);
    }
    this->genericTimer = 20;
}

void Player_InitMode_3(PlayState* play, Player* this) {
    Player_SetAction(play, this, Player_SetupOpenDoorFromSpawn, 0);
    Player_SetupAnimMovement(play, this, (1 | ANIM_FLAG_UPDATE_Y | 8 | ANIM_FLAG_NOMOVE | 0x80));
}

void Player_InitMode_4(PlayState* play, Player* this) {
    Player_SetupJump(this, &gPlayerAnim_link_normal_jump, 12.0f, play);
    Player_SetAction(play, this, Player_JumpFromGrotto, 0);
    this->stateFlags1 |= PLAYER_STATE1_IN_CUTSCENE;
    this->fallStartHeight = this->actor.world.pos.y;
}

void Player_InitMode_7(PlayState* play, Player* this) {
    func_80833B18(play, this, 1, 2.0f, 2.0f, this->actor.shape.rot.y + 0x8000, 0);
}

void Player_InitMode_5(PlayState* play, Player* this) {
    Player_SetAction(play, this, func_808540A0, 0);
    this->actor.draw = NULL;
    this->stateFlags1 |= PLAYER_STATE1_IN_CUTSCENE;
}

void Player_InitMode_6(PlayState* play, Player* this) {
    if (gSaveContext.save.isOwlSave) {
        Player_SetAction(play, this, func_808496AC, 0);
        Player_ChangeAnimShortMorphLoop(play, this, D_8085BE84[PLAYER_ANIMGROUP_43][this->modelAnimType]);
        this->stateFlags1 |= PLAYER_STATE1_IN_CUTSCENE;
        this->genericTimer = 0x28;
        gSaveContext.save.isOwlSave = false;
    } else {
        Player_SetAction(play, this, Player_StandingStill, 0);
        Player_ChangeAnimShortMorphLoop(play, this, D_8085BE84[PLAYER_ANIMGROUP_43][this->modelAnimType]);
        this->stateFlags1 |= PLAYER_STATE1_IN_CUTSCENE;
        this->stateFlags2 |= PLAYER_STATE2_20000000;
        Actor_Spawn(&play->actorCtx, play, ACTOR_EN_TEST7, this->actor.world.pos.x, this->actor.world.pos.y,
                    this->actor.world.pos.z, 0, 0, 0, ENTEST7_MINUS1);
    }
}

// InitModes 0x8 and 0x9
void func_80841744(PlayState* play, Player* this) {
    Player_SetAction(play, this, func_80855C28, 0);
    if (PLAYER_GET_INITMODE(&this->actor) == PLAYER_INITMODE_8) {
        Player_AnimationPlayOnceReverse(play, this, sPlayerOcarinaStartAnimations[this->transformation]);
        this->heldItemActionParam = PLAYER_AP_OCARINA;
        Player_SetModels(this, Player_ActionToModelGroup(this, this->heldItemActionParam));
    } else {
        func_8082DB60(play, this, D_8085BE84[PLAYER_ANIMGROUP_43][this->modelAnimType]);
    }
    this->stateFlags1 |= PLAYER_STATE1_IN_CUTSCENE;
    this->unk_ABC = -10000.0f;
    this->genericTimer = 0x2710;
    this->unk_B10[5] = 8.0f;
}

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(targetArrowOffset, 500, ICHAIN_STOP),
};

Vec3s D_8085D2C4 = { -57, 3377, 0 };

void Player_InitCommon(Player* this, PlayState* play, FlexSkeletonHeader* skelHeader) {
    Actor_ProcessInitChain(&this->actor, sInitChain);
    this->currentYaw = this->actor.world.rot.y;

    if ((PLAYER_GET_INITMODE(&this->actor) != PLAYER_INITMODE_TELESCOPE) &&
        ((gSaveContext.respawnFlag != 2) ||
         (gSaveContext.respawn[RESPAWN_MODE_RETURN].playerParams != PLAYER_PARAMS(0xFF, PLAYER_INITMODE_TELESCOPE)))) {
        Player_SetupHeldItemUpperActionFunc(play, this);
        SkelAnime_InitLink(play, &this->skelAnime, skelHeader, D_8085BE84[PLAYER_ANIMGROUP_0][this->modelAnimType],
                           1 | 8, this->jointTableBuffer, this->morphTableBuffer, PLAYER_LIMB_MAX);
        this->skelAnime.baseTransl = D_8085D2C4;

        SkelAnime_InitLink(play, &this->skelAnimeUpper, skelHeader, Player_GetStandingStillAnim(this), 1 | 8,
                           this->unk_929, this->unk_9C8, PLAYER_LIMB_MAX);
        this->skelAnimeUpper.baseTransl = D_8085D2C4;

        if (this->transformation == PLAYER_FORM_GORON) {
            SkelAnime_InitFlex(play, &this->skelAnimeGoron, &object_link_goron_Skel_0177B8,
                               &object_link_goron_Anim_0178D0, this->jointTable, this->morphTable,
                               OBJECT_LINK_GORON_1_LIMB_MAX);
        }

        ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawFeet, this->ageProperties->shadowScale);
    }

    this->subCamId = CAM_ID_NONE;
    Collider_InitAndSetCylinder(play, &this->cylinder, &this->actor, &D_8085C2EC);
    Collider_InitAndSetCylinder(play, &this->shieldCylinder, &this->actor, &D_8085C318);
    Collider_InitAndSetQuad(play, &this->meleeWeaponQuads[0], &this->actor, &D_8085C344);
    Collider_InitAndSetQuad(play, &this->meleeWeaponQuads[1], &this->actor, &D_8085C344);
    Collider_InitAndSetQuad(play, &this->shieldQuad, &this->actor, &D_8085C394);
}

void func_80841A50(PlayState* play, Player* this) {
    if ((play->roomCtx.curRoom.num >= 0) && (play->roomCtx.prevRoom.num < 0)) {
        Math_Vec3f_Copy(&this->unk_3C0, &this->actor.world.pos);
        this->unk_3CC = this->actor.shape.rot.y;
        this->unk_3CE = play->roomCtx.curRoom.num;
        this->unk_3CF = 1;
    }
}

typedef void (*PlayerInitModeFunc)(PlayState*, Player*);

// Initialisation functions for various gameplay modes depending on spawn params. There may be at most 0x10 due to it
// using a single nybble.
// sInitModeFuncs
PlayerInitModeFunc D_8085D2CC[0x10] = {
    /* 0x0 */ Player_InitMode_NoUpdateOrDraw,
    /* 0x1 */ Player_InitMode_1,
    /* 0x2 */ Player_InitMode_BlueWarp,
    /* 0x3 */ Player_InitMode_3,
    /* 0x4 */ Player_InitMode_4,
    /* 0x5 */ Player_InitMode_5,
    /* 0x6 */ Player_InitMode_6,
    /* 0x7 */ Player_InitMode_7,
    /* 0x8 */ func_80841744,
    /* 0x9 */ func_80841744,
    /* 0xA */ Player_InitMode_WalkingSlow,
    /* 0xB */ Player_InitMode_B,
    /* 0xC */ Player_InitMode_Telescope, // Telescope
    /* 0xD */ Player_InitMode_NoMomentum,
    /* 0xE */ Player_InitMode_WalkingSlow,
    /* 0xF */ Player_InitMode_WalkingPreserveMomentum,
};

// sBlureInit
EffectBlureInit2 D_8085D30C = {
    0, 8, 0, { 255, 255, 255, 255 }, { 255, 255, 255, 64 }, { 255, 255, 255, 0 }, { 255, 255, 255, 0 }, 4,
    0, 2, 0, { 0, 0, 0, 0 },         { 0, 0, 0, 0 },
};

// sTireMarkInit ?
EffectTireMarkInit D_8085D330 = { 0, 63, { 0, 0, 15, 100 } };

// sTireMarkGoronColor ?
Color_RGBA8 D_8085D338 = { 0, 0, 15, 100 };
// sTireMarkOtherColor ?
Color_RGBA8 D_8085D33C = { 0, 0, 0, 150 };
Vec3f D_8085D340 = { 0.0f, 50.0f, 0.0f };

void Player_Init(Actor* thisx, PlayState* play) {
    s32 pad;
    Player* this = THIS;
    s8 objBankIndex;
    s32 respawnFlag;
    s32 var_a1;
    PlayerInitMode initMode;

    play->playerInit = Player_InitCommon;
    play->playerUpdate = Player_UpdateCommon;
    play->unk_18770 = Player_SetupRestrainedByEnemy;
    play->startPlayerFishing = Player_StartFishing;
    play->grabPlayer = Player_GrabPlayer;
    play->startPlayerCutscene = Player_SetupPlayerCutscene;
    play->func_18780 = Player_SetupStandingStillMorph;
    play->damagePlayer = Player_InflictDamage;
    play->talkWithPlayer = Player_TalkWithPlayer;
    play->unk_1878C = func_8085B74C;
    play->unk_18790 = func_8085B820;
    play->unk_18794 = func_8085B854;
    play->setPlayerTalkAnim = func_8085B930;

    gActorOverlayTable->initInfo->objectId = 1;

    this->actor.room = -1;
    this->unk_A86 = -1;

    if (this->actor.shape.rot.x != 0) {
        this->transformation = this->actor.shape.rot.x - 1;

        objBankIndex = Object_GetIndex(&play->objectCtx, gPlayerFormObjectIndices[this->transformation]);
        this->actor.objBankIndex = objBankIndex;
        if (objBankIndex < 0) {
            Actor_MarkForDeath(&this->actor);
            return;
        }

        Actor_SetObjectDependency(play, &this->actor);
    } else {
        this->transformation = gSaveContext.save.playerForm;
        if (this->transformation == PLAYER_FORM_HUMAN) {
            if (gSaveContext.save.equippedMask == PLAYER_MASK_GIANT) {
                gSaveContext.save.equippedMask = PLAYER_MASK_NONE;
            }
            this->currentMask = gSaveContext.save.equippedMask;
        } else {
            this->currentMask = this->transformation + PLAYER_MASK_FIERCE_DEITY;
            gSaveContext.save.equippedMask = PLAYER_MASK_NONE;
        }

        Inventory_UpdateDeitySwordEquip(play);

        this->stickFlameTimer = 0;
        this->unk_B90 = 0;
        this->unk_B92 = 0;
        this->unk_B94 = 0;
        this->unk_B96 = 0;
        this->stateFlags1 &= ~(PLAYER_STATE1_8 | PLAYER_STATE1_CHARGING_SPIN_ATTACK | PLAYER_STATE1_AIMING_ZORAFINS |
                               PLAYER_STATE1_2000000);
        this->stateFlags2 &= ~(PLAYER_STATE2_RELEASING_SPIN_ATTACK | PLAYER_STATE2_1000000 |
                               PLAYER_STATE2_ENABLE_FORWARD_SLIDE_FROM_ATTACK);
        this->stateFlags3 &=
            ~(PLAYER_STATE3_8 | PLAYER_STATE3_40 | PLAYER_STATE3_80 | PLAYER_STATE3_100 | PLAYER_STATE3_200 |
              PLAYER_STATE3_SWINGING_BOTTLE | PLAYER_STATE3_1000 | PLAYER_STATE3_2000 | PLAYER_STATE3_8000 |
              PLAYER_STATE3_10000 | PLAYER_STATE3_40000 | PLAYER_STATE3_80000 | PLAYER_STATE3_100000 |
              PLAYER_STATE3_200000 | PLAYER_STATE3_800000 | PLAYER_STATE3_1000000 | PLAYER_STATE3_2000000);
        this->unk_B08[0] = 0.0f;
        this->unk_B08[1] = 0.0f;
    }

    if (this->transformation == PLAYER_FORM_ZORA) {
        if (this->stateFlags1 & PLAYER_STATE1_SWIMMING) {
            this->unk_B10[0] = 1.0f;
        } else {
            this->unk_B10[0] = 0.0f;
        }
    }

    this->actor.flags &= ~(ACTOR_FLAG_20000 | ACTOR_FLAG_4000000);
    if (this->transformation != PLAYER_FORM_DEKU) {
        this->actor.flags |= ACTOR_FLAG_4000000;
        if (this->transformation == PLAYER_FORM_GORON) {
            this->actor.flags |= ACTOR_FLAG_20000;
        }
    }

    this->ageProperties = &sPlayerAgeProperties[this->transformation];

    this->heldItemActionParam = PLAYER_AP_NONE;
    this->itemActionParam = PLAYER_AP_NONE;
    this->heldItemId = ITEM_NONE;

    Player_UseItem(play, this, ITEM_NONE);
    Player_SetEquipmentData(play, this);
    this->prevBoots = this->currentBoots;
    Player_InitCommon(this, play, gPlayerSkeletons[this->transformation]);

    if (this->actor.shape.rot.z != 0) {
        EffectTireMark* tireMark;

        this->actor.shape.rot.z = 0;
        Player_OverrideBlureColors(play, this, 0, 4);

        tireMark = Effect_GetByIndex(this->meleeWeaponEffectIndex[2]);
        if (this->transformation == PLAYER_FORM_GORON) {
            tireMark->color = D_8085D338;
        } else {
            tireMark->color = D_8085D33C;
        }

        if ((this->csMode == PLAYER_CSMODE_9) || (this->csMode == PLAYER_CSMODE_93)) {
            Player_SetAction(play, this, Player_StartCutscene, 0);
            this->stateFlags1 |= PLAYER_STATE1_IN_CUTSCENE;
        } else {
            Player_SetAction(play, this, func_80855818, 0);
            this->actor.shape.rot.y = this->currentYaw;

            if (this->prevMask != PLAYER_MASK_NONE) {
                Player_PlayAnimOnceSlowed(play, this, &gPlayerAnim_cl_maskoff);
            } else if (this->transformation == PLAYER_FORM_HUMAN) {
                LinkAnimation_Change(play, &this->skelAnime, D_8085D160[this->transformation], -2.0f / 3.0f, 9.0f, 0.0f,
                                     ANIMMODE_ONCE, 0.0f);
            } else {
                func_8082DB60(play, this, &gPlayerAnim_cl_setmaskend);
            }

            this->stateFlags1 |= (PLAYER_STATE1_SKIP_OTHER_ACTORS_UPDATE | PLAYER_STATE1_IN_CUTSCENE);
            this->stateFlags3 |= PLAYER_STATE3_20000;
            this->unk_B10[5] = 3.0f;
        }
        return;
    }

    this->prevMask = this->currentMask;

    Effect_Add(play, &this->meleeWeaponEffectIndex[0], EFFECT_BLURE2, 0, 0, &D_8085D30C);
    Effect_Add(play, &this->meleeWeaponEffectIndex[1], EFFECT_BLURE2, 0, 0, &D_8085D30C);

    Player_OverrideBlureColors(play, this, 0, 4);
    if (this->transformation == PLAYER_FORM_GORON) {
        D_8085D330.color = D_8085D338;
    } else {
        D_8085D330.color = D_8085D33C;
    }
    Effect_Add(play, &this->meleeWeaponEffectIndex[2], EFFECT_TIRE_MARK, 0, 0, &D_8085D330);

    if (this->actor.shape.rot.x != 0) {
        this->actor.shape.rot.x = 0;
        this->csMode = PLAYER_CSMODE_68;
        Player_SetAction(play, this, Player_StartCutscene, 0);
        this->stateFlags1 |= PLAYER_STATE1_IN_CUTSCENE;
        return;
    }

    play->shootingGalleryStatus = 0;
    play->unk_1887D = 0;
    play->unk_1887E = 0;
    this->giObjectSegment = ZeldaArena_Malloc(0x2000);
    this->maskObjectSegment = ZeldaArena_Malloc(0x3800);

    Lights_PointNoGlowSetInfo(&this->lightInfo, this->actor.world.pos.x, this->actor.world.pos.y,
                              this->actor.world.pos.z, 0xFF, 0x80, 0, -1);
    this->lightNode = LightContext_InsertLight(play, &play->lightCtx, &this->lightInfo);
    Play_AssignPlayerActorCsIdsFromScene(&play->state, this->actor.cutscene);

    respawnFlag = gSaveContext.respawnFlag;
    if (respawnFlag != 0) {
        if (respawnFlag == -3) {
            this->actor.params = gSaveContext.respawn[RESPAWN_MODE_UNK_3].playerParams;
        } else {
            if ((respawnFlag == 1) || (respawnFlag == -1)) {
                this->voidRespawnCounter = -2;
            }

            if (respawnFlag != -7) {
                s32 respawnIndex;

                if ((respawnFlag == -8) || (respawnFlag == -5) || (respawnFlag == -4)) {
                    respawnFlag = 1;
                }

                if ((respawnFlag < 0) && (respawnFlag != -1) && (respawnFlag != -6)) {
                    respawnIndex = RESPAWN_MODE_DOWN;
                } else {
                    respawnIndex = (respawnFlag < 0) ? RESPAWN_MODE_TOP : respawnFlag - 1;

                    Math_Vec3f_Copy(&this->actor.world.pos, &gSaveContext.respawn[respawnIndex].pos);
                    Math_Vec3f_Copy(&this->actor.home.pos, &this->actor.world.pos);
                    Math_Vec3f_Copy(&this->actor.prevPos, &this->actor.world.pos);
                    Math_Vec3f_Copy(&this->actor.focus.pos, &this->actor.world.pos);

                    this->fallStartHeight = this->actor.world.pos.y;

                    this->currentYaw = this->actor.shape.rot.y = gSaveContext.respawn[respawnIndex].yaw;
                    this->actor.params = gSaveContext.respawn[respawnIndex].playerParams;
                }

                play->actorCtx.sceneFlags.switches[2] = gSaveContext.respawn[respawnIndex].tempSwitchFlags;
                play->actorCtx.sceneFlags.collectible[1] = gSaveContext.respawn[respawnIndex].unk_18;
                play->actorCtx.sceneFlags.collectible[2] = gSaveContext.respawn[respawnIndex].tempCollectFlags;
            }
        }
    }

    var_a1 = ((respawnFlag == 4) || (gSaveContext.respawnFlag == -4)) ? 1 : 0;
    if (func_801226E0(play, var_a1) == 0) {
        gSaveContext.respawn[RESPAWN_MODE_DOWN].playerParams = PLAYER_PARAMS(thisx->params, PLAYER_INITMODE_D);
    }

    gSaveContext.respawn[RESPAWN_MODE_DOWN].data = 1;
    if (respawnFlag == 0) {
        gSaveContext.respawn[RESPAWN_MODE_TOP] = gSaveContext.respawn[RESPAWN_MODE_DOWN];
    }
    gSaveContext.respawn[RESPAWN_MODE_TOP].playerParams =
        PLAYER_PARAMS(gSaveContext.respawn[RESPAWN_MODE_TOP].playerParams, PLAYER_INITMODE_D);

    initMode = PLAYER_GET_INITMODE(&this->actor);
    if (((initMode == PLAYER_INITMODE_5) || (initMode == PLAYER_INITMODE_6)) &&
        (gSaveContext.save.cutscene >= 0xFFF0)) {
        initMode = PLAYER_INITMODE_D;
    }

    D_8085D2CC[initMode](play, this);

    if ((this->actor.draw != NULL) && gSaveContext.save.hasTatl &&
        ((gSaveContext.gameMode == 0) || (gSaveContext.gameMode == 3)) && (play->sceneId != SCENE_SPOT00)) {
        this->tatlActor = Player_SpawnFairy(play, this, &this->actor.world.pos, &D_8085D340, 0);

        if (gSaveContext.dogParams != 0) {
            gSaveContext.dogParams |= 0x8000;
        }

        if (gSaveContext.powderKegTimer != 0) {
            this->nextModelGroup = Player_ActionToModelGroup(this, PLAYER_AP_POWDER_KEG);
            this->heldItemId = ITEM_POWDER_KEG;
            Player_ChangeItem(play, this, PLAYER_AP_POWDER_KEG);
            Player_SetupHoldActorUpperAction(this, play);
        } else if (gSaveContext.unk_1014 != 0) {
            func_8082F5FC(this, Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACTOR_EN_MM,
                                                   this->actor.world.pos.x, this->actor.world.pos.y,
                                                   this->actor.world.pos.z, 0, this->actor.shape.rot.y, 0, 0x8000));
            Player_SetupHoldActorUpperAction(this, play);
        }
    }

    Minimap_SavePlayerRoomInitInfo(play);
    func_80841A50(play, this);
    this->unk_3CF = 0;
    MREG(64) = 0;
}

// Restores rot to 0
void func_80842510(s16* rot) {
    s16 rotSpeed;

    rotSpeed = (ABS_ALT(*rot) * 100.0f) / 1000.0f;
    rotSpeed = CLAMP(rotSpeed, 0x190, 0xFA0);

    Math_ScaledStepToS(rot, 0, rotSpeed);
}

// Restores a collection of rot's to 0 unless `lookFlags` flags are set
void func_808425B4(Player* this) {
    if (!(this->lookFlags & 2)) {
        s16 sp26 = this->actor.focus.rot.y - this->actor.shape.rot.y;

        func_80842510(&sp26);
        this->actor.focus.rot.y = this->actor.shape.rot.y + sp26;
    }
    if (!(this->lookFlags & 1)) {
        func_80842510(&this->actor.focus.rot.x);
    }
    if (!(this->lookFlags & 8)) {
        func_80842510(&this->headRot.x);
    }
    if (!(this->lookFlags & 0x40)) {
        func_80842510(&this->upperBodyRot.x);
    }
    if (!(this->lookFlags & 4)) {
        func_80842510(&this->actor.focus.rot.z);
    }
    if (!(this->lookFlags & 0x10)) {
        func_80842510(&this->headRot.y);
    }
    if (!(this->lookFlags & 0x20)) {
        func_80842510(&this->headRot.z);
    }
    if (!(this->lookFlags & 0x80)) {
        if (this->unk_AA8 != 0) {
            func_80842510(&this->unk_AA8);
        } else {
            func_80842510(&this->upperBodyRot.y);
        }
    }
    if (!(this->lookFlags & 0x100)) {
        func_80842510(&this->upperBodyRot.z);
    }

    this->lookFlags = 0;
}

/**
 * Sets the DoAction for the interface A/B buttons, depending on a significant number of things
 */
void Player_SetDoAction(PlayState* play, Player* this) {
    DoAction doActionB;
    s32 sp38;

    if (this != GET_PLAYER(play)) {
        return;
    }

    doActionB = -1;
    sp38 = Player_IsSwimming(this) || (func_8084CA24 == this->actionFunc);

    // Set B do action
    if (this->transformation == PLAYER_FORM_GORON) {
        if (this->stateFlags3 & PLAYER_STATE3_80000) {
            doActionB = DO_ACTION_NONE;
        } else if (this->stateFlags3 & PLAYER_STATE3_1000) {
            doActionB = DO_ACTION_POUND;
        } else {
            doActionB = DO_ACTION_PUNCH;
        }
    } else if (this->transformation == PLAYER_FORM_ZORA) {
        if ((!(this->stateFlags1 & PLAYER_STATE1_SWIMMING)) || (!sp38 && (this->actor.bgCheckFlags & 1))) {
            doActionB = DO_ACTION_PUNCH;
        } else {
            doActionB = DO_ACTION_DIVE;
        }
    } else if (this->transformation == PLAYER_FORM_DEKU) {
        doActionB = DO_ACTION_SHOOT;
    } else { // PLAYER_FORM_HUMAN
        if (this->currentMask == PLAYER_MASK_BLAST) {
            doActionB = DO_ACTION_EXPLODE;
        } else if (this->currentMask == PLAYER_MASK_BREMEN) {
            doActionB = DO_ACTION_MARCH;
        } else if (this->currentMask == PLAYER_MASK_KAMARO) {
            doActionB = DO_ACTION_DANCE;
        }
    }

    if (doActionB > -1) {
        func_801155B4(play, doActionB);
    } else if (play->interfaceCtx.unk_21C != 0) {
        play->interfaceCtx.unk_21C = 0;
        play->interfaceCtx.bButtonDoAction = 0;
    }

    // Set A do action
    if ((Message_GetState(&play->msgCtx) == TEXT_STATE_NONE) ||
        ((play->msgCtx.currentTextId >= 0x100) && (play->msgCtx.currentTextId <= 0x200)) ||
        ((play->msgCtx.currentTextId >= 0x1BB2) && (play->msgCtx.currentTextId < 0x1BB7))) {
        Actor* heldActor = this->heldActor;                   // sp34
        Actor* interactRangeActor = this->interactRangeActor; // sp30
        s32 pad;
        s32 sp28 = this->relativeAnalogStickInputs[this->inputFrameCounter];
        s32 sp24;
        DoAction doActionA =
            ((this->transformation == PLAYER_FORM_GORON) && !(this->stateFlags1 & PLAYER_STATE1_400000))
                ? DO_ACTION_CURL
                : DO_ACTION_NONE;

        if (play->actorCtx.flags & ACTORCTX_FLAG_PICTOGRAPH_ON) {
            doActionA = DO_ACTION_SNAP;
        } else if (Player_InBlockingCsMode(play, this) || (this->actor.flags & ACTOR_FLAG_OCARINA_READY) ||
                   (this->stateFlags1 & PLAYER_STATE1_CHARGING_SPIN_ATTACK) ||
                   (this->stateFlags3 & PLAYER_STATE3_80000) || (func_80854430 == this->actionFunc)) {
            doActionA = DO_ACTION_NONE;
        } else if (this->stateFlags1 & PLAYER_STATE1_100000) {
            doActionA = DO_ACTION_RETURN;
        } else if ((this->itemActionParam == PLAYER_AP_FISHING_ROD) && (this->stickFlameTimer != 0)) {
            doActionA = (this->stickFlameTimer == 2) ? DO_ACTION_REEL : DO_ACTION_NONE;
            doActionA =
                (this->stickFlameTimer == 2) ? DO_ACTION_REEL : DO_ACTION_NONE; // required to match, maybe FAKE?
        } else if (this->stateFlags3 & PLAYER_STATE3_2000) {
            doActionA = DO_ACTION_DOWN;
        } else if ((this->doorType != PLAYER_DOORTYPE_NONE) && (this->doorType != PLAYER_DOORTYPE_STAIRCASE) &&
                   !(this->stateFlags1 & PLAYER_STATE1_HOLDING_ACTOR)) {
            doActionA = DO_ACTION_OPEN;
        } else if (this->stateFlags3 & PLAYER_STATE3_200000) {
            static u8 D_8085D34C[] = {
                DO_ACTION_1, DO_ACTION_2, DO_ACTION_3, DO_ACTION_4, DO_ACTION_5, DO_ACTION_6, DO_ACTION_7, DO_ACTION_8,
            };

            doActionA = D_8085D34C[this->remainingHopsCounter];
        } else if ((!(this->stateFlags1 & PLAYER_STATE1_HOLDING_ACTOR) || (heldActor == NULL)) &&
                   (interactRangeActor != NULL) && (this->getItemId < GI_NONE)) {
            doActionA = DO_ACTION_OPEN;
        } else if (!sp38 && (this->stateFlags2 & PLAYER_STATE2_1)) {
            doActionA = DO_ACTION_GRAB;
        } else if ((this->stateFlags2 & PLAYER_STATE2_4) ||
                   (!(this->stateFlags1 & PLAYER_STATE1_RIDING_HORSE) && (this->rideActor != NULL))) {
            doActionA = DO_ACTION_CLIMB;
        } else if ((this->stateFlags1 & PLAYER_STATE1_RIDING_HORSE) &&
                   (!EN_HORSE_CHECK_4((EnHorse*)this->rideActor) && (func_808505D0 != this->actionFunc))) {
            if ((this->stateFlags2 & PLAYER_STATE2_CAN_SPEAK_OR_CHECK) && (this->talkActor != NULL)) {
                if ((this->talkActor->category == ACTORCAT_NPC) || (this->talkActor->id == ACTOR_DM_CHAR08)) {
                    doActionA = DO_ACTION_SPEAK;
                } else {
                    doActionA = DO_ACTION_CHECK;
                }
            } else if (!func_8082DA90(play) && !Actor_PlayerIsAimingReadyFpsItem(this) &&
                       !(this->stateFlags1 & PLAYER_STATE1_100000)) {
                doActionA = DO_ACTION_FASTER;
            } else {
                doActionA = DO_ACTION_NONE;
            }
        } else if ((this->stateFlags2 & PLAYER_STATE2_CAN_SPEAK_OR_CHECK) && (this->talkActor != NULL)) {
            if ((this->talkActor->category == ACTORCAT_NPC) || (this->talkActor->category == ACTORCAT_ENEMY) ||
                (this->talkActor->id == ACTOR_DM_CHAR08)) {
                doActionA = DO_ACTION_SPEAK;
            } else {
                doActionA = DO_ACTION_CHECK;
            }
        } else if ((this->stateFlags1 & (PLAYER_STATE1_2000 | PLAYER_STATE1_200000)) ||
                   ((this->stateFlags1 & PLAYER_STATE1_RIDING_HORSE) && (this->stateFlags2 & PLAYER_STATE2_400000))) {
            doActionA = DO_ACTION_DOWN;
        } else if ((this->stateFlags1 & PLAYER_STATE1_HOLDING_ACTOR) && (this->getItemId == GI_NONE) &&
                   (heldActor != NULL)) {
            if ((this->actor.bgCheckFlags & 1) || (heldActor->id == ACTOR_EN_NIW)) {
                if (!Player_CanThrowActor(this, heldActor)) {
                    doActionA = DO_ACTION_DROP;
                } else {
                    doActionA = DO_ACTION_THROW;
                }
            } else {
                doActionA = DO_ACTION_NONE;
            }
        } else if (this->stateFlags2 & PLAYER_STATE2_10000) {
            doActionA = DO_ACTION_GRAB;
        } else if (this->stateFlags2 & PLAYER_STATE2_800) {
            static u8 D_8085D354[] = { DO_ACTION_1, DO_ACTION_2 };
            s32 var_v0;

            var_v0 = ((120.0f - this->actor.depthInWater) / 40.0f);
            var_v0 = CLAMP(var_v0, 0, ARRAY_COUNT(D_8085D354) - 1);

            doActionA = D_8085D354[var_v0];
        } else if (this->stateFlags3 & PLAYER_STATE3_100) {
            doActionA = DO_ACTION_JUMP;
        } else if (this->stateFlags3 & PLAYER_STATE3_1000) {
            doActionA = DO_ACTION_RETURN;
        } else if (!Player_IsZTargeting(this) && (this->stateFlags1 & PLAYER_STATE1_SWIMMING) && !sp38) {
            doActionA = DO_ACTION_SURFACE;
        } else if (((this->transformation != PLAYER_FORM_DEKU) &&
                    (sp38 || ((this->stateFlags1 & PLAYER_STATE1_SWIMMING) && !(this->actor.bgCheckFlags & 1)))) ||
                   ((this->transformation == PLAYER_FORM_DEKU) && (this->actor.bgCheckFlags & 1) &&
                    func_800C9DDC(&play->colCtx, this->actor.floorPoly, this->actor.floorBgId))) {
            doActionA = (this->transformation == PLAYER_FORM_ZORA) ? DO_ACTION_SWIM
                        : ((this->stateFlags1 & PLAYER_STATE1_SWIMMING) && (interactRangeActor != NULL) &&
                           (interactRangeActor->id == ACTOR_EN_ZOG))
                            ? DO_ACTION_GRAB
                            : DO_ACTION_DIVE;
        } else {
            sp24 = Player_IsZTargeting(this);
            if ((sp24 && (this->transformation != PLAYER_FORM_DEKU)) || !(this->stateFlags1 & PLAYER_STATE1_400000) ||
                !Player_IsGoronOrDeku(this)) {
                if ((this->transformation != PLAYER_FORM_GORON) &&
                    !(this->stateFlags1 & (PLAYER_STATE1_4 | PLAYER_STATE1_4000)) && (sp28 <= 0) &&
                    ((Player_IsUnfriendlyZTargeting(this)) ||
                     ((sPlayerCurrentFloorType != BG_FLOOR_TYPE_7) &&
                      ((Player_IsFriendlyZTargeting(this)) ||
                       ((play->roomCtx.curRoom.unk3 != 2) && !(this->stateFlags1 & PLAYER_STATE1_400000) &&
                        (sp28 == 0)))))) {
                    doActionA = DO_ACTION_ATTACK;
                } else if ((play->roomCtx.curRoom.unk3 != 2) && sp24 && (sp28 > 0)) {
                    doActionA = DO_ACTION_JUMP;
                } else if ((this->transformation == PLAYER_FORM_DEKU) &&
                           !(this->stateFlags1 & PLAYER_STATE1_SWIMMING) && (this->actor.bgCheckFlags & 1)) {
                    doActionA = DO_ACTION_ATTACK;
                } else if (((this->transformation == PLAYER_FORM_HUMAN) ||
                            (this->transformation == PLAYER_FORM_ZORA)) &&
                           ((this->itemActionParam >= PLAYER_AP_SWORD_KOKIRI) ||
                            ((this->stateFlags2 & PLAYER_STATE2_100000) &&
                             (play->actorCtx.targetContext.arrowPointedActor == NULL)))) {
                    doActionA = DO_ACTION_PUT_AWAY;

                    if (!play->msgCtx.currentTextId) {} //! FAKE
                }
            }
        }

        if (doActionA != DO_ACTION_PUT_AWAY) {
            this->putAwayCountdown = 20;
        } else if (this->putAwayCountdown != 0) {
            doActionA = DO_ACTION_NONE;
            this->putAwayCountdown--;
        }

        func_8011552C(play, doActionA);

        // Set Tatl state
        if (!Play_InCsMode(play) && (this->stateFlags2 & PLAYER_STATE2_TATL_REQUESTING_TALK) &&
            !(this->stateFlags3 & PLAYER_STATE3_100)) {
            if (this->targetedActor != NULL) {
                func_80115764(play, 0x2B);
            } else {
                func_80115764(play, 0x2A);
            }
            ActorCutscene_SetIntentToPlay(0x7C);
        } else {
            func_80115764(play, 0x2C);
        }
    }
}

s32 func_808430E0(Player* this) {
    if ((this->transformation == PLAYER_FORM_DEKU) && (this->actor.bgCheckFlags & 1) && func_8083784C(this)) {
        this->actor.bgCheckFlags &= ~1;
    }
    if (this->actor.bgCheckFlags & 1) {
        return false;
    }

    if (!(this->stateFlags1 & PLAYER_STATE1_SWIMMING)) {
        sPlayerCurrentFloorType = BG_FLOOR_TYPE_0;
    }
    this->angleToFloorX = 0;
    this->unk_B6E = 0;
    D_80862B28 = 0;
    return true;
}

void func_80843178(PlayState* play, Player* this) {
    u8 spC7;
    CollisionPoly* floorPoly;
    f32 temp_fv0; // spBC
    f32 temp_fv0_3;
    f32 temp_fv1; // spB4
    u32 var_v1;   // spB0
    s32 spAC;

    spC7 = 0;
    spAC = (Player_MiniCsMovement == this->actionFunc) && (this->unk_397 == 4);
    sPlayerPrevFloorProperty = this->floorProperty;

    temp_fv0 = this->ageProperties->unk_38;
    temp_fv1 = this->ageProperties->unk_00;

    if (this->stateFlags1 & (PLAYER_STATE1_IN_CUTSCENE | PLAYER_STATE1_FALLING_INTO_GROTTO_OR_VOID)) {
        if ((!(this->stateFlags1 & PLAYER_STATE1_IN_DEATH_CUTSCENE) && !(this->stateFlags2 & PLAYER_STATE2_4000) &&
             (this->stateFlags1 & PLAYER_STATE1_FALLING_INTO_GROTTO_OR_VOID)) ||
            spAC) {
            var_v1 = 0x38;
            this->actor.bgCheckFlags &= ~1;
        } else {
            if ((this->stateFlags1 & 1) && (play->roomCtx.curRoom.unk3 != 1) &&
                ((this->sceneExitPosY - (s32)this->actor.world.pos.y) >= 0x64)) {
                var_v1 = 0x39;
            } else if (!(this->stateFlags1 & PLAYER_STATE1_1) &&
                       ((Player_Door_Open == this->actionFunc) || (Player_MiniCsMovement == this->actionFunc))) {
                var_v1 = 0x3C;
                this->actor.bgCheckFlags &= ~(8 | 0x200);
            } else {
                var_v1 = 0x3F;
            }
        }
    } else {
        if (func_808561B0 == this->actionFunc) {
            var_v1 = 0x814;
        } else if ((this->stateFlags3 & (PLAYER_STATE3_1000 | PLAYER_STATE3_80000)) && (this->linearVelocity >= 8.0f)) {
            var_v1 = 0x337;
        } else {
            var_v1 = 0x3F;
        }
    }

    if (this->stateFlags3 & PLAYER_STATE3_1) {
        var_v1 &= ~6;
    }
    if (var_v1 & 4) {
        this->stateFlags3 |= PLAYER_STATE3_10;
    }

    if (Player_IsSwimming(this)) {
        var_v1 &= ~0x18;
    }

    Actor_UpdateBgCheckInfo(play, &this->actor, 26.800001f, temp_fv0, temp_fv1, var_v1);

    this->unk_AC0 -= (this->actor.world.pos.y - this->actor.prevPos.y) / this->actor.scale.y;
    this->unk_AC0 = CLAMP(this->unk_AC0, -1000.0f, 1000.0f);

    if (this->actor.bgCheckFlags & 0x10) {
        this->actor.velocity.y = 0.0f;
    }

    sPlayerYDistToFloor = this->actor.world.pos.y - this->actor.floorHeight;
    sPlayerConveyorSpeedIndex = BG_CONVEYOR_SPEED_DISABLED;
    floorPoly = this->actor.floorPoly;

    if ((floorPoly != NULL) && (var_v1 & 4)) {
        this->floorProperty = SurfaceType_GetFloorProperty(&play->colCtx, floorPoly, this->actor.floorBgId);
        if (this == GET_PLAYER(play)) {
            func_801A3CF4(SurfaceType_GetEcho(&play->colCtx, floorPoly, this->actor.floorBgId));
            if (this->actor.floorBgId == BGCHECK_SCENE) {
                func_800FAAB4(play, SurfaceType_GetLightSettingIndex(&play->colCtx, floorPoly, this->actor.floorBgId));
            } else {
                DynaPolyActor_SetRidingRotatingStateByIndex(&play->colCtx, this->actor.floorBgId);
            }
        }

        sPlayerConveyorSpeedIndex = SurfaceType_GetConveyorSpeed(&play->colCtx, floorPoly, this->actor.floorBgId);
        if (sPlayerConveyorSpeedIndex != BG_CONVEYOR_SPEED_DISABLED) {
            sPlayerIsOnFloorConveyor = SurfaceType_IsFloorConveyor(&play->colCtx, floorPoly, this->actor.floorBgId);
            if ((!sPlayerIsOnFloorConveyor && (this->actor.depthInWater > 20.0f)) ||
                (sPlayerIsOnFloorConveyor && (this->actor.bgCheckFlags & 1))) {
                D_80862B16 = SurfaceType_GetConveyorDirection(&play->colCtx, floorPoly, this->actor.floorBgId) << 0xA;
            } else {
                sPlayerConveyorSpeedIndex = BG_CONVEYOR_SPEED_DISABLED;
            }
        }
    }

    this->actor.bgCheckFlags &= ~0x200;
    if (this->actor.bgCheckFlags & 8) {
        static Vec3f D_8085D358 = { 0.0f, 0.0f, 0.0f };
        CollisionPoly* spA8;
        s32 spA4;
        s16 temp_v1_3; // spA2
        f32 sp9C;

        D_8085D358.y = 17.800001f;
        D_8085D358.z = this->ageProperties->unk_38 + 10.0f;
        if (Player_WallLineTestWithOffset(play, this, &D_8085D358, &spA8, &spA4, &D_80862B30)) {
            this->actor.bgCheckFlags |= 0x200;
            if (spA8 != this->actor.wallPoly) {
                this->actor.wallPoly = spA8;
                this->actor.wallBgId = spA4;
                this->actor.wallYaw = Math_FAtan2F(spA8->normal.z, spA8->normal.x);
            }
        }

        temp_v1_3 = this->actor.shape.rot.y - BINANG_ADD(this->actor.wallYaw, 0x8000);
        sPlayerCurrentWallFlags = SurfaceType_GetWallFlags(&play->colCtx, this->actor.wallPoly, this->actor.wallBgId);
        sYawToTouchedWall = ABS_ALT(temp_v1_3);
        temp_v1_3 = BINANG_SUB(this->currentYaw, BINANG_ADD(this->actor.wallYaw, 0x8000));
        D_80862B24 = ABS_ALT(temp_v1_3);

        temp_fv0_3 = D_80862B24 * 0.00008f;
        if (!(this->actor.bgCheckFlags & 1) || (temp_fv0_3 >= 1.0f)) {
            this->speedLimit = R_RUN_SPEED_LIMIT / 100.0f;
        } else {
            temp_fv1 = (R_RUN_SPEED_LIMIT / 100.0f) * temp_fv0_3;

            this->speedLimit = temp_fv1;
            if (temp_fv1 < 0.1f) {
                this->speedLimit = 0.1f;
            }
        }

        if ((this->actor.bgCheckFlags & 0x200) && (sYawToTouchedWall < 0x3000)) {
            CollisionPoly* wallPoly = this->actor.wallPoly;

            if (ABS_ALT(wallPoly->normal.y) < 0x258) {
                f32 wallPolyNormalX;
                f32 wallPolyNormalY;
                f32 wallPolyNormalZ;
                f32 temp_fv1_3;
                CollisionPoly* sp84;
                CollisionPoly* sp80;
                s32 sp7C;
                Vec3f sp70;
                f32 temp_fv0_5; // sp6C
                f32 sp68;
                s32 temp_v1_6;

                wallPolyNormalX = COLPOLY_GET_NORMAL(wallPoly->normal.x);
                wallPolyNormalY = COLPOLY_GET_NORMAL(wallPoly->normal.y);
                wallPolyNormalZ = COLPOLY_GET_NORMAL(wallPoly->normal.z);

                this->wallDistance = Math3D_UDistPlaneToPos(wallPolyNormalX, wallPolyNormalY, wallPolyNormalZ,
                                                            wallPoly->dist, &this->actor.world.pos);
                temp_fv1_3 = this->wallDistance + 10.0f;
                sp70.x = this->actor.world.pos.x - (temp_fv1_3 * wallPolyNormalX);
                sp70.z = this->actor.world.pos.z - (temp_fv1_3 * wallPolyNormalZ);
                sp70.y = this->actor.world.pos.y + this->ageProperties->unk_0C;
                temp_fv0_5 = BgCheck_EntityRaycastFloor5(&play->colCtx, &sp84, &sp7C, &this->actor, &sp70);

                this->wallHeight = temp_fv0_5 - this->actor.world.pos.y;
                if ((this->wallHeight < 17.800001f) ||
                    ((BgCheck_EntityCheckCeiling(&play->colCtx, &sp68, &this->actor.world.pos,
                                                 (temp_fv0_5 - this->actor.world.pos.y) + 20.0f, &sp80, &sp7C,
                                                 &this->actor) != 0))) {
                    this->wallHeight = 399.96002f;
                } else {
                    D_8085D358.y = (temp_fv0_5 + 5.0f) - this->actor.world.pos.y;

                    if ((Player_WallLineTestWithOffset(play, this, &D_8085D358, &sp80, &sp7C, &D_80862B30)) &&
                        (temp_v1_6 = this->actor.wallYaw - Math_FAtan2F(sp80->normal.z, sp80->normal.x),
                         ABS_ALT(temp_v1_6) < 0x4000) &&
                        !(SurfaceType_CheckWallFlag1(&play->colCtx, sp80, sp7C))) {
                        this->wallHeight = 399.96002f;
                    } else if (SurfaceType_CheckWallFlag0(&play->colCtx, wallPoly, this->actor.wallBgId) == 0) {
                        if (this->ageProperties->unk_1C <= this->wallHeight) {
                            if (ABS_ALT(sp84->normal.y) > 0x5DC0) {
                                if ((this->ageProperties->unk_14 <= this->wallHeight) || Player_IsSwimming(this)) {
                                    spC7 = 4;
                                } else if (this->ageProperties->unk_18 <= this->wallHeight) {
                                    spC7 = 3;
                                } else {
                                    spC7 = 2;
                                }
                            }
                        } else {
                            spC7 = 1;
                        }
                    }
                }
            }
        }
    } else {
        this->speedLimit = R_RUN_SPEED_LIMIT / 100.0f;
        this->wallHeight = 0.0f;
        this->unk_B5D = 0;
    }

    if (spC7 == this->unk_B5C) {
        if (this->linearVelocity != 0.0f) {
            if (this->unk_B5D < 0x64) {
                this->unk_B5D++;
            }
        }
    } else {
        this->unk_B5C = spC7;
        this->unk_B5D = 0;
    }

    sPlayerCurrentFloorType = SurfaceType_GetFloorType(&play->colCtx, floorPoly, this->actor.floorBgId);
    if (this->actor.bgCheckFlags & 1) {
        f32 floorPolyNormalX;
        f32 floorPolyNormalY;
        f32 floorPolyNormalZ;
        f32 sp54;
        s32 pad;
        f32 sp4C;

        D_80862B40 = SurfaceType_GetSlope(&play->colCtx, this->actor.floorPoly, this->actor.floorBgId);
        if (func_808430E0(this) == 0) {
            floorPolyNormalY = COLPOLY_GET_NORMAL(floorPoly->normal.y);

            if (this->actor.floorBgId != BGCHECK_SCENE) {
                DynaPolyActor_SetRidingMovingStateByIndex(&play->colCtx, this->actor.floorBgId);
            } else if (!(this->actor.bgCheckFlags & 2) && (this->actor.depthInWater <= 24.0f) && (D_80862B40 != 1) &&
                       (sPlayerConveyorSpeedIndex == BG_CONVEYOR_SPEED_DISABLED) && (floorPolyNormalY > 0.5f)) {
                if (ActorCutscene_GetCurrentIndex() != play->playerActorCsIds[8]) {
                    func_80841A50(play, this);
                }
            }

            floorPolyNormalX = COLPOLY_GET_NORMAL(floorPoly->normal.x);
            floorPolyNormalY = 1.0f / floorPolyNormalY;
            floorPolyNormalZ = COLPOLY_GET_NORMAL(floorPoly->normal.z);

            sp54 = Math_SinS(this->currentYaw);
            sp4C = Math_CosS(this->currentYaw);
            this->angleToFloorX =
                Math_FAtan2F(1.0f, (-(floorPolyNormalX * sp54) - (floorPolyNormalZ * sp4C)) * floorPolyNormalY);
            this->unk_B6E =
                Math_FAtan2F(1.0f, (-(floorPolyNormalX * sp4C) - (floorPolyNormalZ * sp54)) * floorPolyNormalY);
            sp54 = Math_SinS(this->actor.shape.rot.y);
            sp4C = Math_CosS(this->actor.shape.rot.y);
            D_80862B28 =
                Math_FAtan2F(1.0f, (-(floorPolyNormalX * sp54) - (floorPolyNormalZ * sp4C)) * floorPolyNormalY);
            func_8083CF68(play, this);
        }
    } else {
        func_808430E0(this);
        D_80862B40 = 0;
    }

    if (floorPoly != NULL) {
        this->unk_D66 = this->floorSfxOffset;
        if (spAC != 0) {
            this->floorSfxOffset = NA_SE_PL_WALK_CONCRETE - SFX_FLAG;
            return;
        }

        if (this->actor.bgCheckFlags & 0x20) {
            if (this->actor.depthInWater < 50.0f) {
                if (this->actor.depthInWater < 20.0f) {
                    this->floorSfxOffset = (sPlayerCurrentFloorType == BG_FLOOR_TYPE_13)
                                               ? NA_SE_PL_WALK_DIRT - SFX_FLAG
                                               : NA_SE_PL_WALK_WATER0 - SFX_FLAG;
                } else {
                    this->floorSfxOffset = (sPlayerCurrentFloorType == BG_FLOOR_TYPE_13)
                                               ? NA_CODE_DIRT_DEEP - SFX_FLAG
                                               : NA_SE_PL_WALK_WATER1 - SFX_FLAG;
                }

                return;
            }
        }

        if (this->stateFlags2 & PLAYER_STATE2_200) {
            this->floorSfxOffset = NA_SE_PL_WALK_SAND - SFX_FLAG;
        } else if (COLPOLY_GET_NORMAL(floorPoly->normal.y) > 0.5f) {
            this->floorSfxOffset = SurfaceType_GetSfxIdOffset(&play->colCtx, floorPoly, this->actor.floorBgId);
        }
    }
}

void Player_UpdateCamAndSeqModes(PlayState* play, Player* this) {
    u8 seqMode;
    s32 pad[2];
    Camera* camera;
    s32 camMode;

    if (this == GET_PLAYER(play)) {
        seqMode = SEQ_MODE_DEFAULT;
        if (this->stateFlags1 & PLAYER_STATE1_100000) {
            seqMode = SEQ_MODE_STILL;
        } else if (this->csMode != PLAYER_CSMODE_0) {
            Camera_ChangeMode(Play_GetCamera(play, CAM_ID_MAIN), CAM_MODE_NORMAL);
        } else {
            camera = (this->actor.id == ACTOR_PLAYER) ? Play_GetCamera(play, CAM_ID_MAIN)
                                                      : Play_GetCamera(play, ((EnTest3*)this)->unk_D8E);
            if ((this->actor.parent != NULL) && (this->stateFlags3 & PLAYER_STATE3_80)) {
                camMode = CAM_MODE_HOOKSHOT;
                Camera_SetViewParam(camera, CAM_VIEW_TARGET, this->actor.parent);
            } else if (func_8084BC64 == this->actionFunc) {
                camMode = CAM_MODE_STILL;
            } else if (this->stateFlags3 & PLAYER_STATE3_8000) {
                if (this->stateFlags1 & PLAYER_STATE1_SWIMMING) {
                    camMode = CAM_MODE_GORONDASH;
                } else {
                    camMode = CAM_MODE_FREEFALL;
                }
            } else if (this->stateFlags3 & PLAYER_STATE3_80000) {
                if (this->actor.bgCheckFlags & 1) {
                    camMode = CAM_MODE_GORONDASH;
                } else {
                    camMode = CAM_MODE_GORONJUMP;
                }
            } else if (this->stateFlags2 & PLAYER_STATE2_100) {
                camMode = CAM_MODE_PUSHPULL;
            } else if (this->targetedActor != NULL) {
                if ((this->actor.flags & ACTOR_FLAG_100) == ACTOR_FLAG_100) {
                    camMode = CAM_MODE_TALK;
                } else if (this->stateFlags1 & PLAYER_STATE1_10000) {
                    if (this->stateFlags1 & PLAYER_STATE1_2000000) {
                        camMode = CAM_MODE_FOLLOWBOOMERANG;
                    } else {
                        camMode = CAM_MODE_FOLLOWTARGET;
                    }
                } else {
                    camMode = CAM_MODE_BATTLE;
                }
                Camera_SetViewParam(camera, CAM_VIEW_TARGET, this->targetedActor);
            } else if (this->stateFlags1 & PLAYER_STATE1_CHARGING_SPIN_ATTACK) {
                camMode = CAM_MODE_CHARGE;
            } else if (this->stateFlags3 & PLAYER_STATE3_100) {
                camMode = CAM_MODE_DEKUHIDE;
            } else if (this->stateFlags1 & PLAYER_STATE1_2000000) {
                camMode = CAM_MODE_FOLLOWBOOMERANG;
                Camera_SetViewParam(camera, CAM_VIEW_TARGET, this->boomerangActor);
            } else if (this->stateFlags1 & (PLAYER_STATE1_4 | PLAYER_STATE1_2000 | PLAYER_STATE1_4000)) {
                if (Player_IsFriendlyZTargeting(this)) {
                    camMode = CAM_MODE_HANGZ;
                } else {
                    camMode = CAM_MODE_HANG;
                }
            } else if ((this->stateFlags3 & PLAYER_STATE3_2000) && (this->actor.velocity.y < 0.0f)) {
                if (this->stateFlags1 & (PLAYER_STATE1_Z_TARGETING_FRIENDLY | PLAYER_STATE1_40000000)) {
                    camMode = CAM_MODE_DEKUFLYZ;
                } else {
                    camMode = CAM_MODE_DEKUFLY;
                }
            } else if (this->stateFlags1 & (PLAYER_STATE1_Z_TARGETING_FRIENDLY | PLAYER_STATE1_40000000)) {
                if (Actor_PlayerIsAimingReadyFpsItem(this) || Player_IsAimingReadyZoraFins(this)) {
                    camMode = CAM_MODE_BOWARROWZ;
                } else if (this->stateFlags1 & PLAYER_STATE1_200000) {
                    camMode = CAM_MODE_CLIMBZ;
                } else {
                    camMode = CAM_MODE_TARGET;
                }
            } else if ((this->stateFlags1 & PLAYER_STATE1_400000) && (this->transformation != 0)) {
                camMode = CAM_MODE_STILL;
            } else if (this->stateFlags1 & PLAYER_STATE1_JUMPING) {
                camMode = CAM_MODE_JUMP;
            } else if (this->stateFlags1 & PLAYER_STATE1_200000) {
                camMode = CAM_MODE_CLIMB;
            } else if (this->stateFlags1 & PLAYER_STATE1_FREEFALLING) {
                camMode = CAM_MODE_FREEFALL;
            } else if (((Player_MeleeWeaponAttack == this->actionFunc) &&
                        (this->meleeWeaponAnimation >= PLAYER_MWA_FORWARD_SLASH_1H) &&
                        (this->meleeWeaponAnimation <= PLAYER_MWA_ZORA_PUNCH_KICK)) ||
                       (this->stateFlags3 & PLAYER_STATE3_8) ||
                       ((Player_RideHorse == this->actionFunc) && (this->genericTimer == 0)) ||
                       (func_808505D0 == this->actionFunc)) {
                camMode = CAM_MODE_STILL;
            } else {
                camMode = CAM_MODE_NORMAL;
                if ((this->linearVelocity == 0.0f) &&
                    (!(this->stateFlags1 & PLAYER_STATE1_RIDING_HORSE) || (this->rideActor->speedXZ == 0.0f))) {
                    seqMode = SEQ_MODE_STILL;
                }
            }

            Camera_ChangeMode(camera, camMode);
        }

        if (play->actorCtx.targetContext.bgmEnemy != NULL) {
            seqMode = SEQ_MODE_ENEMY;
            Audio_UpdateEnemyBgmVolume(sqrtf(play->actorCtx.targetContext.bgmEnemy->xyzDistToPlayerSq));
        }

        Audio_SetSequenceMode(seqMode);
    }
}

Vec3f sStickFlameVelocity = { 0.0f, 0.5f, 0.0f };
Vec3f sStickFlameAccel = { 0.0f, 0.5f, 0.0f };
Color_RGBA8 sStickFlamePrimColor = { 255, 255, 100, 255 };
Color_RGBA8 sStickFlameEnvColor = { 255, 50, 0, 0 };

void Player_UpdateDekuStick(PlayState* play, Player* this) {
    f32 newDekuStickLength;
    f32 color;

    if (this->unk_B08[1] == 0.0f) {
        Player_UseItem(play, this, ITEM_NONE);
        return;
    }

    newDekuStickLength = 1.0f;
    if (DECR(this->stickFlameTimer) == 0) {
        Inventory_ChangeAmmo(ITEM_STICK, -1);
        this->stickFlameTimer = 1;
        this->unk_B08[1] = 0.0f;
        newDekuStickLength = 0.0f;
    } else if (this->stickFlameTimer > 200) {
        newDekuStickLength = (210 - this->stickFlameTimer) / 10.0f;
    } else if (this->stickFlameTimer < 0x14) {
        newDekuStickLength = this->stickFlameTimer / 20.0f;
        this->unk_B08[1] = newDekuStickLength;
    }

    if (newDekuStickLength > 0.0f) {
        // Spawn flame effect
        func_800B0EB0(play, &this->meleeWeaponInfo[0].tip, &sStickFlameVelocity, &sStickFlameAccel,
                      &sStickFlamePrimColor, &sStickFlameEnvColor, (newDekuStickLength * 200.0f), 0, 8);
        if (((play->roomCtx.curRoom.enablePosLights != 0)) || (MREG(93) != 0)) {
            color = (Rand_ZeroOne() * 30.0f) + 225.0f;
            Lights_PointSetColorAndRadius(&this->lightInfo, color, color * 0.7f, 0, newDekuStickLength * 300.0f);
        }
    }
}

void Player_ElectricShock(PlayState* play, Player* this) {
    this->shockTimer--;
    this->unk_B66 += this->shockTimer;

    if (this->unk_B66 > 20) {
        Vec3f pos;
        Vec3f* bodyPartsPos;
        s32 scale;
        s32 randIndex;

        this->unk_B66 -= 20;
        scale = this->shockTimer * 2;
        if (scale > 40) {
            scale = 40;
        }

        randIndex = Rand_ZeroFloat(PLAYER_BODYPART_MAX - 0.1f);
        bodyPartsPos = randIndex + this->bodyPartsPos;

        pos.x = (randPlusMinusPoint5Scaled(5.0f) + bodyPartsPos->x) - this->actor.world.pos.x;
        pos.y = (randPlusMinusPoint5Scaled(5.0f) + bodyPartsPos->y) - this->actor.world.pos.y;
        pos.z = (randPlusMinusPoint5Scaled(5.0f) + bodyPartsPos->z) - this->actor.world.pos.z;

        EffectSsFhgFlash_SpawnShock(play, &this->actor, &pos, scale, 1);
        func_800B8F98(&this->actor, NA_SE_PL_SPARK - SFX_FLAG);
    }
}

/**
 * Rumbles the controller when close to a secret.
 */
void Player_DetectSecrets(PlayState* play, Player* this) {
    f32 step = (SQ(200.0f) * 5.0f) - (this->closestSecretDistSq * 5.0f);

    if (step < 0.0f) {
        step = 0.0f;
    }

    this->secretRumbleCharge += step;
    if (this->secretRumbleCharge > SQ(2000.0f)) {
        this->secretRumbleCharge = 0.0f;
        Player_RequestRumble(play, this, 120, 20, 10, SQ(0));
    }
}

s8 D_8085D384[92] = {
    /*  0 */ PLAYER_CSMODE_0,
    /*  1 */ PLAYER_CSMODE_2,
    /*  2 */ PLAYER_CSMODE_2,
    /*  3 */ PLAYER_CSMODE_4,
    /*  4 */ PLAYER_CSMODE_3,
    /*  5 */ PLAYER_CSMODE_56,
    /*  6 */ PLAYER_CSMODE_8,
    /*  7 */ PLAYER_CSMODE_0,
    /*  8 */ PLAYER_CSMODE_0,
    /*  9 */ PLAYER_CSMODE_135,
    /* 10 */ PLAYER_CSMODE_21,
    /* 11 */ PLAYER_CSMODE_61,
    /* 12 */ PLAYER_CSMODE_62,
    /* 13 */ PLAYER_CSMODE_60,
    /* 14 */ PLAYER_CSMODE_63,
    /* 15 */ PLAYER_CSMODE_64,
    /* 16 */ PLAYER_CSMODE_65,
    /* 17 */ PLAYER_CSMODE_66,
    /* 18 */ PLAYER_CSMODE_70,
    /* 19 */ PLAYER_CSMODE_19,
    /* 20 */ PLAYER_CSMODE_71,
    /* 21 */ PLAYER_CSMODE_72,
    /* 22 */ PLAYER_CSMODE_67,
    /* 23 */ PLAYER_CSMODE_73,
    /* 24 */ PLAYER_CSMODE_74,
    /* 25 */ PLAYER_CSMODE_75,
    /* 26 */ PLAYER_CSMODE_68,
    /* 27 */ PLAYER_CSMODE_69,
    /* 28 */ PLAYER_CSMODE_76,
    /* 29 */ PLAYER_CSMODE_116,
    /* 30 */ PLAYER_CSMODE_0,
    /* 31 */ PLAYER_CSMODE_40,
    /* 32 */ PLAYER_CSMODE_0,
    /* 33 */ -PLAYER_CSMODE_52, // the only negative one
    /* 34 */ PLAYER_CSMODE_42,
    /* 35 */ PLAYER_CSMODE_43,
    /* 36 */ PLAYER_CSMODE_57,
    /* 37 */ PLAYER_CSMODE_81,
    /* 38 */ PLAYER_CSMODE_41,
    /* 39 */ PLAYER_CSMODE_53,
    /* 40 */ PLAYER_CSMODE_54,
    /* 41 */ PLAYER_CSMODE_44,
    /* 42 */ PLAYER_CSMODE_55,
    /* 43 */ PLAYER_CSMODE_45,
    /* 44 */ PLAYER_CSMODE_46,
    /* 45 */ PLAYER_CSMODE_47,
    /* 46 */ PLAYER_CSMODE_48,
    /* 47 */ PLAYER_CSMODE_49,
    /* 48 */ PLAYER_CSMODE_50,
    /* 49 */ PLAYER_CSMODE_51,
    /* 50 */ PLAYER_CSMODE_77,
    /* 51 */ PLAYER_CSMODE_78,
    /* 52 */ PLAYER_CSMODE_79,
    /* 53 */ PLAYER_CSMODE_80,
    /* 54 */ PLAYER_CSMODE_81,
    /* 55 */ PLAYER_CSMODE_82,
    /* 56 */ PLAYER_CSMODE_83,
    /* 57 */ PLAYER_CSMODE_84,
    /* 58 */ PLAYER_CSMODE_85,
    /* 59 */ PLAYER_CSMODE_86,
    /* 60 */ PLAYER_CSMODE_87,
    /* 61 */ PLAYER_CSMODE_88,
    /* 62 */ PLAYER_CSMODE_89,
    /* 63 */ PLAYER_CSMODE_90,
    /* 64 */ PLAYER_CSMODE_91,
    /* 65 */ PLAYER_CSMODE_92,
    /* 66 */ PLAYER_CSMODE_94,
    /* 67 */ PLAYER_CSMODE_95,
    /* 68 */ PLAYER_CSMODE_100,
    /* 69 */ PLAYER_CSMODE_101,
    /* 70 */ PLAYER_CSMODE_98,
    /* 71 */ PLAYER_CSMODE_99,
    /* 72 */ PLAYER_CSMODE_102,
    /* 73 */ PLAYER_CSMODE_103,
    /* 74 */ PLAYER_CSMODE_104,
    /* 75 */ PLAYER_CSMODE_112,
    /* 76 */ PLAYER_CSMODE_113,
    /* 77 */ PLAYER_CSMODE_117,
    /* 78 */ PLAYER_CSMODE_104,
    /* 79 */ PLAYER_CSMODE_104,
    /* 80 */ PLAYER_CSMODE_105,
    /* 81 */ PLAYER_CSMODE_106,
    /* 82 */ PLAYER_CSMODE_107,
    /* 83 */ PLAYER_CSMODE_108,
    /* 84 */ PLAYER_CSMODE_109,
    /* 85 */ PLAYER_CSMODE_110,
    /* 86 */ PLAYER_CSMODE_118,
    /* 87 */ PLAYER_CSMODE_119,
    /* 88 */ PLAYER_CSMODE_120,
    /* 89 */ PLAYER_CSMODE_114,
    /* 90 */ PLAYER_CSMODE_111,
    /* 91 */ PLAYER_CSMODE_122,
};

f32 D_8085D3E0[PLAYER_FORM_MAX] = {
    0.8f, 0.6f, 0.8f, 1.5f, 1.0f,
};

void func_80844784(PlayState* play, Player* this) {
    f32 var_fv0;
    s16 var_a3;
    f32 temp_ft4;
    s32 temp_ft2;
    f32 temp_fv1_2;
    f32 sp58;
    f32 sp54;
    f32 sp50;
    f32 sp4C;
    f32 sp48;
    f32 sp44;
    f32 temp_fa0;
    f32 temp_fa1;
    s16 temp_v0;
    f32 temp_fv0_2;

    if ((this->actor.bgCheckFlags & 1) && (sPlayerCurrentFloorType == BG_FLOOR_TYPE_5) &&
        (this->currentBoots < PLAYER_BOOTS_ZORA_UNDERWATER)) {
        var_a3 = this->currentYaw;
        var_fv0 = this->linearVelocity;
        temp_v0 = this->actor.world.rot.y - var_a3;

        if ((ABS_ALT(temp_v0) > 0x6000) && (this->actor.speedXZ != 0.0f)) {
            var_fv0 = 0.0f;
            var_a3 += 0x8000;
        }

        if ((Math_StepToF(&this->actor.speedXZ, var_fv0, 0.35f) != 0) && (var_fv0 == 0.0f)) {
            this->actor.world.rot.y = this->currentYaw;
        }

        if (this->linearVelocity != 0.0f) {
            temp_ft2 = (fabsf(this->linearVelocity) * 700.0f) - (fabsf(this->actor.speedXZ) * 100.0f);
            temp_ft2 = CLAMP(temp_ft2, 0, 0x546);

            Math_ScaledStepToS(&this->actor.world.rot.y, var_a3, temp_ft2);
        }
        if ((this->linearVelocity == 0.0f) && (this->actor.speedXZ != 0.0f)) {
            func_8019F780(&this->actor.projectedPos, Player_GetFloorSfx(this, NA_SE_PL_SLIP_LEVEL - SFX_FLAG),
                          this->actor.speedXZ);
        }
    } else {
        this->actor.speedXZ = this->linearVelocity;
        this->actor.world.rot.y = this->currentYaw;
    }

    Actor_UpdateVelocityWithGravity(&this->actor);
    D_80862B3C = 0.0f;
    if ((gSaveContext.save.playerData.health != 0) &&
        ((this->pushedSpeed != 0.0f) || (this->windSpeed != 0.0f) || (play->envCtx.windSpeed >= 50.0f)) &&
        (!Player_InCsMode(play)) &&
        !(this->stateFlags1 & (PLAYER_STATE1_4 | PLAYER_STATE1_2000 | PLAYER_STATE1_4000 | PLAYER_STATE1_200000)) &&
        !(this->stateFlags3 & PLAYER_STATE3_100) && (Player_JumpUpToLedge != this->actionFunc) &&
        (this->actor.id == ACTOR_PLAYER)) {
        this->actor.velocity.x += this->pushedSpeed * Math_SinS(this->pushedYaw);
        this->actor.velocity.z += this->pushedSpeed * Math_CosS(this->pushedYaw);
        temp_fv1_2 = 10.0f - this->actor.velocity.y;
        if (temp_fv1_2 > 0.0f) {
            sp58 = D_8085D3E0[this->transformation];
            sp54 = this->windSpeed * sp58;
            sp50 = Math_SinS(this->windAngleX) * sp54;
            sp4C = Math_CosS(this->windAngleX) * sp54;
            sp48 = Math_SinS(this->windAngleY) * sp4C;
            sp44 = Math_CosS(this->windAngleY) * sp4C;

            if ((sp50 > 0.0f) && (this->transformation == PLAYER_FORM_DEKU) && !(this->actor.bgCheckFlags & 1)) {
                if (Player_SetAction(play, this, func_80856918, 1)) {
                    this->stateFlags3 |= PLAYER_STATE3_2000 | PLAYER_STATE3_1000000;
                    Player_PlayReactableSfx(this, NA_SE_IT_DEKUNUTS_FLOWER_OPEN);
                    func_8019FD90(4, 2);
                }

                this->genericTimer = 0x270F;
                Math_Vec3f_Copy(this->unk_AF0, &this->actor.world.pos);
            }

            if (play->envCtx.windSpeed >= 50.0f) {
                temp_fa0 = play->envCtx.windDir.x;
                temp_fa1 = play->envCtx.windDir.y;
                temp_ft4 = play->envCtx.windDir.z;

                temp_fv0_2 = sqrtf(SQ(temp_fa0) + SQ(temp_fa1) + SQ(temp_ft4));
                if (temp_fv0_2 != 0.0f) {
                    temp_fv0_2 = ((play->envCtx.windSpeed - 50.0f) * 0.1f * sp58) / temp_fv0_2;

                    sp48 -= temp_fa0 * temp_fv0_2;
                    sp50 -= temp_fa1 * temp_fv0_2;
                    sp44 -= temp_ft4 * temp_fv0_2;
                }
            }

            if (temp_fv1_2 < sp50) {
                temp_fv1_2 /= sp50;

                sp48 *= temp_fv1_2;
                sp50 *= temp_fv1_2;
                sp44 *= temp_fv1_2;
            }

            if (this->actor.bgCheckFlags & 1) {
                D_80862B3C = (sp44 * Math_CosS(this->currentYaw)) + (Math_SinS(this->currentYaw) * sp48);
                if (fabsf(D_80862B3C) > 4.0f) {
                    Player_SetupSpawnDustAtFeet(play, this);
                }

                func_8019F780(&this->actor.projectedPos, Player_GetFloorSfx(this, NA_SE_PL_SLIP_LEVEL - SFX_FLAG),
                              fabsf(D_80862B3C));
            }

            this->actor.velocity.x += sp48;
            this->actor.velocity.z += sp44;
            this->actor.velocity.y += sp50;
        }
    }

    Actor_UpdatePos(&this->actor);
}

Color_RGBA8 D_8085D3F4 = { 100, 255, 255, 0 };
Color_RGBA8 D_8085D3F8 = { 0, 100, 100, 0 };

void func_80844D80(PlayState* play, Player* this) {
    Vec3f pos;
    Vec3f spA0;
    Vec3f velocity;
    Vec3f accel;
    Vec3f sp7C;
    s32 i;

    Math_Vec3f_Diff(&this->meleeWeaponInfo[0].tip, &this->meleeWeaponInfo[0].base, &sp7C);
    Math_Vec3f_SumScaled(&this->meleeWeaponInfo[0].base, &sp7C, 0.3f, &spA0);

    for (i = 0; i < 2; i++) {
        Math_Vec3f_SumScaled(&this->meleeWeaponInfo[0].base, &sp7C, Rand_ZeroOne(), &pos);
        Math_Vec3f_AddRand(&pos, 15.0f, &pos);
        Math_Vec3f_DistXYZAndStoreNormDiff(&spA0, &pos, 1.7f, &velocity);
        Math_Vec3f_ScaleAndStore(&velocity, 0.01f, &accel);
        EffectSsKirakira_SpawnDispersed(play, &pos, &velocity, &accel, &D_8085D3F4, &D_8085D3F8,
                                        Rand_S16Offset(-20, -120), 15);
    }
}

f32 D_8085D3FC[] = { 0.005f, 0.05f };

f32 sWaterConveyorSpeeds[BG_CONVEYOR_SPEED_MAX - 1] = {
    2.0f,  // BG_CONVEYOR_SPEED_SLOW
    4.0f,  // BG_CONVEYOR_SPEED_MEDIUM
    11.0f, // BG_CONVEYOR_SPEED_FAST
};
f32 sFloorConveyorSpeeds[BG_CONVEYOR_SPEED_MAX - 1] = {
    0.5f, // BG_CONVEYOR_SPEED_SLOW
    1.0f, // BG_CONVEYOR_SPEED_MEDIUM
    3.0f, // BG_CONVEYOR_SPEED_FAST
};

void Player_UpdateCommon(Player* this, PlayState* play, Input* input) {
    f32 temp_fv0;
    f32 temp_fv1;

    sPlayerControlInput = input;

    if (this->voidRespawnCounter < 0) {
        this->voidRespawnCounter++;
        if (this->voidRespawnCounter == 0) {
            this->voidRespawnCounter = 1;
            play_sound(NA_SE_OC_REVENGE);
        }
    }

    Math_Vec3f_Copy(&this->actor.prevPos, &this->actor.home.pos);

    temp_fv1 = fabsf(this->linearVelocity) * (fabsf(Math_SinS(this->angleToFloorX) * 800.0f) + 100.0f);

    Math_StepToF(&this->unk_AC0, 0.0f, CLAMP_MIN(temp_fv1, 300.0f));

    if (this->fpsItemShotTimer != 0) {
        this->fpsItemShotTimer--;
    }

    if (this->unk_B5E != 0) {
        this->unk_B5E--;
    }

    if (this->unk_D6B != 0) {
        this->unk_D6B--;
    }

    if (this->invincibilityTimer < 0) {
        this->invincibilityTimer++;
    } else if (this->invincibilityTimer > 0) {
        this->invincibilityTimer--;
    }

    if (this->unk_B64 != 0) {
        this->unk_B64--;
    }

    if (this->blastMaskTimer != 0) {
        this->blastMaskTimer--;
    }

    if (gSaveContext.jinxTimer != 0) {
        gSaveContext.jinxTimer--;
    }

    func_80122C20(play, &this->unk_3D0);
    if ((this->transformation == PLAYER_FORM_FIERCE_DEITY) && (Player_IsZTargeting(this) != 0)) {
        func_80844D80(play, this);
    }
    if (this->transformation == PLAYER_FORM_ZORA) {
        s32 var_v0 = (this->stateFlags1 & PLAYER_STATE1_SWIMMING) ? 1 : 0;

        Math_StepToF(&this->unk_B10[0], var_v0, D_8085D3FC[var_v0]);
    }
    func_80832888(this, play);
    if (play->roomCtx.curRoom.enablePosLights != 0) {
        Lights_PointSetColorAndRadius(&this->lightInfo, 255, 255, 255, 60);
    } else {
        this->lightInfo.params.point.radius = -1;
    }

    if ((this->itemActionParam == PLAYER_AP_STICK) && (this->stickFlameTimer != 0)) {
        Player_UpdateDekuStick(play, this);
    } else if (this->itemActionParam == PLAYER_AP_FISHING_ROD) {
        if (this->stickFlameTimer < 0) {
            this->stickFlameTimer++;
        }
    }

    if (this->shockTimer != 0) {
        Player_ElectricShock(play, this);
    }

    if (this->isBurning) {
        Player_Burning(play, this);
    }

    if (this->stateFlags2 & PLAYER_STATE2_8000) {
        if (!(this->actor.bgCheckFlags & 1)) {
            Player_StopHorizontalMovement(this);
            Actor_MoveWithGravity(&this->actor);
        }
        func_80843178(play, this);
    } else {
        f32 temp_fa0;
        f32 var_fv1_2;
        s32 var_v1; // sp58
        s32 pad;

        if (this->currentBoots != this->prevBoots) {
            if (this->currentBoots == PLAYER_BOOTS_ZORA_UNDERWATER) {
                if (this->stateFlags1 & PLAYER_STATE1_SWIMMING) {
                    Player_ResetSubCam(play, this);
                    if (this->ageProperties->unk_2C < this->actor.depthInWater) {
                        this->stateFlags2 |= PLAYER_STATE2_DIVING;
                    }
                }
            } else if ((this->stateFlags1 & PLAYER_STATE1_SWIMMING) &&
                       ((this->prevBoots == PLAYER_BOOTS_ZORA_UNDERWATER) || (this->actor.bgCheckFlags & 1))) {
                func_8083B930(play, this);
                this->stateFlags2 &= ~PLAYER_STATE2_DIVING;
                if (Player_UpdateSwimIdle == this->actionFunc) {
                    this->genericTimer = 20;
                }
            }
            this->prevBoots = this->currentBoots;
        }
        if ((this->actor.parent == NULL) && (this->stateFlags1 & PLAYER_STATE1_RIDING_HORSE)) {
            this->actor.parent = this->rideActor;
            Player_SetupRideHorse(play, this);
            this->genericTimer = -1;
            Player_AnimationPlayOnce(play, this, &gPlayerAnim_link_uma_wait_1);
            Player_SetupAnimMovement(play, this, 1 | ANIM_FLAG_UPDATE_Y | 8 | ANIM_FLAG_NOMOVE | 0x80);
        }

        if (this->unk_ADC == 0) {
            this->slashCounter = 0;
        } else if (this->unk_ADC < 0) {
            this->unk_ADC++;
        } else {
            this->unk_ADC--;
        }

        if (!(this->stateFlags3 & PLAYER_STATE3_2000)) {
            Math_ScaledStepToS(&this->unk_AAA, 0, 0x190);
        }

        if ((this->transformation >= PLAYER_FORM_GORON) && (this->transformation <= PLAYER_FORM_DEKU)) {
            func_800BBB74(&this->blinkInfo, 20, 80, 3);
        } else {
            func_800BBAC0(&this->blinkInfo, 20, 80, 6);
        }

        this->actor.shape.face = ((play->gameplayFrames & 0x20) ? 0 : 3) + this->blinkInfo.eyeTexIndex;

        if (this->currentMask == PLAYER_MASK_BUNNY) {
            func_80124420(this);
        }

        if (Actor_PlayerIsAimingFpsItem(this)) {
            func_808484F0(this);
        }

        if ((play->unk_18844 == 0) && !(this->skelAnime.moveFlags & 0x80)) {
            if (!(this->stateFlags1 & PLAYER_STATE1_2) && (this->actor.parent == NULL)) {
                func_80844784(play, this);
            }
            func_80843178(play, this);
        } else {
            sPlayerCurrentFloorType = BG_FLOOR_TYPE_0;
            this->floorProperty = BG_FLOOR_PROPERTY_0;
            if (this->stateFlags1 & PLAYER_STATE1_RIDING_HORSE) {
                this->actor.floorPoly = this->rideActor->floorPoly;
                this->actor.floorBgId = this->rideActor->floorBgId;
            }
            sPlayerConveyorSpeedIndex = BG_CONVEYOR_SPEED_DISABLED;
            this->pushedSpeed = 0.0f;
        }

        Player_SetupExit(play, this, this->actor.floorPoly, this->actor.floorBgId);
        if (sPlayerConveyorSpeedIndex != BG_CONVEYOR_SPEED_DISABLED) {
            f32 conveyorSpeed;
            s32 pad2;

            sPlayerConveyorSpeedIndex--;
            if (!sPlayerIsOnFloorConveyor) {
                conveyorSpeed = sWaterConveyorSpeeds[sPlayerConveyorSpeedIndex];
                if (!(this->stateFlags1 & PLAYER_STATE1_SWIMMING)) {
                    conveyorSpeed /= 4.0f;
                }
            } else {
                conveyorSpeed = sFloorConveyorSpeeds[sPlayerConveyorSpeedIndex];
            }

            Math_StepToF(&this->pushedSpeed, conveyorSpeed, conveyorSpeed * 0.1f);
            Math_ScaledStepToS(&this->pushedYaw, D_80862B16,
                               ((this->stateFlags1 & PLAYER_STATE1_SWIMMING) ? 400.0f : 800.0f) * conveyorSpeed);
        } else if (this->pushedSpeed != 0.0f) {
            Math_StepToF(&this->pushedSpeed, 0.0f, (this->stateFlags1 & PLAYER_STATE1_SWIMMING) ? 0.5f : 2.0f);
        }
        if (!(this->stateFlags1 & (PLAYER_STATE1_IN_DEATH_CUTSCENE | PLAYER_STATE1_IN_CUTSCENE)) &&
            !(this->stateFlags3 & PLAYER_STATE3_80) && (func_80854430 != this->actionFunc)) {
            func_8083BB4C(play, this);
            if (!Play_InCsMode(play)) {
                if ((this->actor.id == ACTOR_PLAYER) &&
                    !(this->stateFlags1 & PLAYER_STATE1_FALLING_INTO_GROTTO_OR_VOID) &&
                    (gSaveContext.save.playerData.health == 0) && func_808323C0(this, play->playerActorCsIds[6])) {
                    if (this->stateFlags3 & PLAYER_STATE3_1000000) {
                        func_808355D8(play, this, &gPlayerAnim_pn_kakkufinish);
                    } else if (this->stateFlags1 &
                               (PLAYER_STATE1_4 | PLAYER_STATE1_2000 | PLAYER_STATE1_4000 | PLAYER_STATE1_200000)) {
                        Player_ResetAttributes(play, this);
                        func_80833AA0(this, play);
                    } else if ((this->actor.bgCheckFlags & 1) || (this->stateFlags1 & PLAYER_STATE1_SWIMMING)) {
                        func_80831F34(play, this,
                                      Player_IsSwimming(this)
                                          ? &gPlayerAnim_link_swimer_swim_down
                                          : ((this->shockTimer != 0) ? &gPlayerAnim_link_normal_electric_shock_end
                                                                     : &gPlayerAnim_link_derth_rebirth));
                    }
                } else {
                    if ((this->actor.parent == NULL) &&
                        (func_8082DA90(play) || (this->unk_D6B != 0) || !func_80834600(this, play))) {
                        Player_SetupMidairBehavior(this, play);
                    } else {
                        this->fallStartHeight = this->actor.world.pos.y;
                    }

                    Player_DetectSecrets(play, this);
                }
            }
        } else if (!(this->actor.bgCheckFlags & 1) && (Player_MiniCsMovement == this->actionFunc) &&
                   (this->unk_397 == 4)) {
            this->actor.world.pos.y = this->actor.prevPos.y;
        }

        if (play->csCtx.state != CS_STATE_IDLE) {
            if ((this->csMode != PLAYER_CSMODE_5) && !(this->stateFlags1 & PLAYER_STATE1_RIDING_HORSE)) {
                if (!(this->stateFlags2 & PLAYER_STATE2_80) && (this->actor.id == ACTOR_PLAYER)) {
                    if ((play->csCtx.playerAction != NULL) && (D_8085D384[play->csCtx.playerAction->action] != 0)) {
                        func_800B7298(play, NULL, PLAYER_CSMODE_5);
                        Player_StopHorizontalMovement(this);
                        // Can't be this->csMode == PLAYER_CSMODE_0
                    } else if ((!this->csMode) &&
                               !(this->stateFlags2 & (PLAYER_STATE2_DIVING | PLAYER_STATE2_OCARINA_ON)) &&
                               (play->csCtx.state != CS_STATE_3)) {
                        func_800B7298(play, NULL, PLAYER_CSMODE_20);
                        Player_StopHorizontalMovement(this);
                    }
                }
            }
        }

        // Can't be != PLAYER_CSMODE_0
        if (this->csMode) {
            if ((this->csMode != PLAYER_CSMODE_6) ||
                !(this->stateFlags1 & (PLAYER_STATE1_4 | PLAYER_STATE1_2000 | PLAYER_STATE1_4000 |
                                       PLAYER_STATE1_200000 | PLAYER_STATE1_4000000))) {
                if (Player_StartCutscene != this->actionFunc) {
                    this->attentionMode = PLAYER_ATTENTIONMODE_CUTSCENE;
                    if (this->csMode == PLAYER_CSMODE_5) {
                        Player_SetupCutscene(play, this);
                        Player_ClearAttentionModeAndStopMoving(this);
                    }
                }
            } else if (Player_StartCutscene != this->actionFunc) {
                Player_Cutscene_End(play, this, 0);
            }
        } else {
            this->prevCsMode = PLAYER_CSMODE_0;
        }

        func_8083BF54(play, this);
        Lights_PointSetPosition(&this->lightInfo, this->actor.world.pos.x, this->actor.world.pos.y + 40.0f,
                                this->actor.world.pos.z);

        if (((this->targetedActor == NULL) || (this->targetedActor == this->talkActor) ||
             (this->targetedActor->hintId == 0xFF)) &&
            (this->tatlTextId == 0)) {
            this->stateFlags2 &= ~(PLAYER_STATE2_CAN_SPEAK_OR_CHECK | PLAYER_STATE2_TATL_REQUESTING_TALK);
        }

        this->stateFlags1 &= ~(PLAYER_STATE1_ZORA_BARRIER | PLAYER_STATE1_CHARGING_SPIN_ATTACK | PLAYER_STATE1_400000);
        this->stateFlags2 &=
            ~(PLAYER_STATE2_1 | PLAYER_STATE2_4 | PLAYER_STATE2_8 |
              PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING | PLAYER_STATE2_ALWAYS_DISABLE_MOVE_ROTATION |
              PLAYER_STATE2_100 | PLAYER_STATE2_200 | PLAYER_STATE2_1000 | PLAYER_STATE2_4000 | PLAYER_STATE2_10000 |
              PLAYER_STATE2_400000 | PLAYER_STATE2_4000000);
        this->stateFlags3 &= ~(PLAYER_STATE3_10 | PLAYER_STATE3_40 | PLAYER_STATE3_100 | PLAYER_STATE3_SWINGING_BOTTLE |
                               PLAYER_STATE3_1000 | PLAYER_STATE3_100000 | PLAYER_STATE3_2000000 |
                               PLAYER_STATE3_4000000 | PLAYER_STATE3_8000000 | PLAYER_STATE3_10000000);
        func_808425B4(this);
        Player_StoreAnalogStickInput(play, this);

        sWaterSpeedScale = (this->stateFlags1 & PLAYER_STATE1_SWIMMING) ? 0.5f : 1.0f;
        D_8085C3E8 = 1.0f / sWaterSpeedScale;

        sUsingItemAlreadyInHand = D_80862B4C = 0;

        var_v1 = Play_InCsMode(play);
        sCurrentMask = this->currentMask;
        if (!(this->stateFlags3 & PLAYER_STATE3_4)) {
            this->actionFunc(this, play);
        }

        if (!var_v1) {
            Player_SetDoAction(play, this);
        }

        Player_UpdateCamAndSeqModes(play, this);

        if (this->skelAnime.moveFlags & 8) {
            AnimationContext_SetMoveActor(play, &this->actor, &this->skelAnime,
                                          (this->skelAnime.moveFlags & 4) ? 1.0f : this->ageProperties->unk_08);
        }

        Player_UpdateYaw(this, play);
        if (this->actor.flags & ACTOR_FLAG_100) {
            this->talkActorDistance = 0.0f;
        } else {
            this->talkActor = NULL;
            this->exchangeItemId = PLAYER_AP_NONE;
            this->talkActorDistance = FLT_MAX;
        }
        if (!(this->actor.flags & ACTOR_FLAG_OCARINA_READY) && (this->attentionMode != PLAYER_UNKAA5_5)) {
            this->ocarinaActor = NULL;
            this->ocarinaActorXZDist = FLT_MAX;
        }
        if (!(this->stateFlags1 & PLAYER_STATE1_HOLDING_ACTOR)) {
            this->interactRangeActor = NULL;
            this->getItemDirection = 0x6000;
        }
        if (this->actor.parent == NULL) {
            this->rideActor = NULL;
        }

        this->tatlTextId = 0;
        this->unk_B2B = -1;
        this->closestSecretDistSq = FLT_MAX;
        this->doorType = PLAYER_DOORTYPE_NONE;
        this->unk_B75 = 0;
        this->unk_A78 = NULL;

        Math_StepToF(&this->windSpeed, 0.0f, 0.5f);
        if ((this->unk_B62 != 0) || ((gSaveContext.magicState == 0) && (gSaveContext.save.playerData.magic != 0) &&
                                     (this->stateFlags1 & PLAYER_STATE1_ZORA_BARRIER))) {
            Player_UpdateZoraBarrier(play, this);
        }

        temp_fv0 = this->actor.world.pos.y - this->actor.prevPos.y;
        var_fv1_2 =
            temp_fv0 +
            ((this->bodyPartsPos[PLAYER_BODYPART_L_FOOT].y + this->bodyPartsPos[PLAYER_BODYPART_R_FOOT].y) * 0.5f);
        temp_fv0 += this->bodyPartsPos[PLAYER_BODYPART_HEAD].y + 10.0f;

        if (this->cylinder.info.toucher.dmgFlags == 0x80000) {
            this->cylinder.dim.height = 80;
            var_fv1_2 = ((temp_fv0 + var_fv1_2) * 0.5f) - 40.0f;
        } else {
            this->cylinder.dim.height = temp_fv0 - var_fv1_2;

            if (this->cylinder.dim.height < 0) {
                temp_fa0 = temp_fv0;
                temp_fv0 = var_fv1_2;
                var_fv1_2 = temp_fa0;
                this->cylinder.dim.height = -this->cylinder.dim.height;
            }
        }

        this->cylinder.dim.yShift = var_fv1_2 - this->actor.world.pos.y;

        if (this->unk_B62 != 0) {
            this->shieldCylinder.base.acFlags = 0;
            this->shieldCylinder.info.toucher.dmgFlags = 0x80000;
            this->shieldCylinder.info.toucherFlags = 1;
            this->shieldCylinder.info.bumperFlags = 0;
            this->shieldCylinder.dim.height = 80;
            this->shieldCylinder.dim.radius = 50;
            this->shieldCylinder.dim.yShift = ((temp_fv0 + var_fv1_2) * 0.5f - 40.0f) - this->actor.world.pos.y;

            Collider_UpdateCylinder(&this->actor, &this->shieldCylinder);
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->shieldCylinder.base);
        } else if (this->stateFlags1 & PLAYER_STATE1_400000) {
            if ((this->transformation == PLAYER_FORM_GORON) || (this->transformation == PLAYER_FORM_DEKU)) {
                this->shieldCylinder.base.acFlags = 0x15;
                this->shieldCylinder.info.toucher.dmgFlags = 0x100000;
                this->shieldCylinder.info.toucherFlags = 0;
                this->shieldCylinder.info.bumperFlags = 1;

                if (this->transformation == PLAYER_FORM_GORON) {
                    this->shieldCylinder.dim.height = 35;
                } else {
                    this->shieldCylinder.dim.height = 30;
                }

                if (this->transformation == PLAYER_FORM_GORON) {
                    this->shieldCylinder.dim.radius = 30;
                } else {
                    this->shieldCylinder.dim.radius = 20;
                }

                this->shieldCylinder.dim.yShift = 0;
                Collider_UpdateCylinder(&this->actor, &this->shieldCylinder);
                CollisionCheck_SetAC(play, &play->colChkCtx, &this->shieldCylinder.base);
                this->cylinder.dim.yShift = 0;
                this->cylinder.dim.height = this->shieldCylinder.dim.height;
            } else {
                this->cylinder.dim.height = this->cylinder.dim.height * 0.8f;
            }
        }

        Collider_UpdateCylinder(&this->actor, &this->cylinder);
        if (!(this->stateFlags2 & PLAYER_STATE2_4000)) {
            if (!(this->stateFlags1 & (PLAYER_STATE1_4 | PLAYER_STATE1_IN_DEATH_CUTSCENE | PLAYER_STATE1_2000 |
                                       PLAYER_STATE1_4000 | PLAYER_STATE1_RIDING_HORSE)) &&
                !(this->stateFlags3 & PLAYER_STATE3_10000000)) {
                if ((func_808561B0 != this->actionFunc) && (Player_SlipOnSlope != this->actionFunc) &&
                    (this->actor.draw != NULL)) {
                    if ((this->actor.id != ACTOR_PLAYER) && (this->csMode == PLAYER_CSMODE_110)) {
                        this->cylinder.dim.radius = 8;
                    }
                    CollisionCheck_SetOC(play, &play->colChkCtx, &this->cylinder.base);
                }
            }
            if (!(this->stateFlags1 & (PLAYER_STATE1_IN_DEATH_CUTSCENE | PLAYER_STATE1_4000000)) &&
                (this->invincibilityTimer <= 0)) {
                if ((func_808561B0 != this->actionFunc) &&
                    ((func_80857BE8 != this->actionFunc) || (this->genericVar != 1))) {
                    if (this->cylinder.base.atFlags != 0) {
                        CollisionCheck_SetAT(play, &play->colChkCtx, &this->cylinder.base);
                    }
                    CollisionCheck_SetAC(play, &play->colChkCtx, &this->cylinder.base);
                }
            }
        }

        AnimationContext_SetNextQueue(play);
    }

    func_801229FC(this);
    Math_Vec3f_Copy(&this->actor.home.pos, &this->actor.world.pos);

    if ((this->stateFlags1 &
         (PLAYER_STATE1_IN_DEATH_CUTSCENE | PLAYER_STATE1_SKIP_OTHER_ACTORS_UPDATE | PLAYER_STATE1_IN_CUTSCENE)) ||
        (this != GET_PLAYER(play))) {
        this->actor.colChkInfo.mass = MASS_IMMOVABLE;
    } else {
        this->actor.colChkInfo.mass = sPlayerMass[this->transformation];
    }

    this->stateFlags3 &= ~(PLAYER_STATE3_4 | PLAYER_STATE3_400);
    Collider_ResetCylinderAC(play, &this->cylinder.base);
    Collider_ResetCylinderAC(play, &this->shieldCylinder.base);
    Collider_ResetCylinderAT(play, &this->shieldCylinder.base);
    Collider_ResetQuadAT(play, &this->meleeWeaponQuads[0].base);
    Collider_ResetQuadAT(play, &this->meleeWeaponQuads[1].base);
    Collider_ResetQuadAC(play, &this->shieldQuad.base);

    if (!(this->actor.bgCheckFlags & 1) || (this->actor.bgCheckFlags & 2) || (this->actor.floorBgId != BGCHECK_SCENE)) {
        this->unk_AC0 = 0.0f;
    }
    this->actor.shape.yOffset = this->unk_ABC + this->unk_AC0;
}

Vec3f D_8085D41C = { 0.0f, 0.0f, -30.0f };

void Player_Update(Actor* thisx, PlayState* play) {
    static Vec3f sDogSpawnPos;
    Player* this = (Player*)thisx;
    s32 dogParams;
    s32 pad;
    Input input;
    s32 pad2;

    this->stateFlags3 &= ~PLAYER_STATE3_10;
    if (gSaveContext.dogParams < 0) {
        if (Object_GetIndex(&play->objectCtx, OBJECT_DOG) < 0) {
            gSaveContext.dogParams = 0;
        } else {
            Actor* dog;

            gSaveContext.dogParams &= (u16)~0x8000;
            Player_TranslateAndRotateY(this, &this->actor.world.pos, &D_8085D41C, &sDogSpawnPos);

            dogParams = gSaveContext.dogParams;

            dog = Actor_Spawn(&play->actorCtx, play, ACTOR_EN_DG, sDogSpawnPos.x, sDogSpawnPos.y, sDogSpawnPos.z, 0,
                              this->actor.shape.rot.y, 0, dogParams | 0x8000);
            if (dog != NULL) {
                dog->room = -1;
            }
        }
    }

    if ((this->interactRangeActor != NULL) && (this->interactRangeActor->update == NULL)) {
        this->interactRangeActor = NULL;
    }

    if ((this->heldActor != NULL) && (this->heldActor->update == NULL)) {
        Player_DetatchHeldActor(play, this);
    }

    if ((play->actorCtx.unk268 != 0) && (this == GET_PLAYER(play))) {
        input = play->actorCtx.unk_26C;
    } else if ((this->csMode == PLAYER_CSMODE_5) ||
               (this->stateFlags1 & (PLAYER_STATE1_20 | PLAYER_STATE1_IN_CUTSCENE)) || (this != GET_PLAYER(play)) ||
               func_8082DA90(play) || (gSaveContext.save.playerData.health == 0)) {
        bzero(&input, sizeof(Input));
        this->fallStartHeight = this->actor.world.pos.y;
    } else {
        input = play->state.input[0];
        if (this->unk_B5E != 0) {
            input.cur.button &= ~0xC008;
            input.press.button &= ~0xC008;
        }
    }

    Player_UpdateCommon(this, play, &input);
    play->actorCtx.unk268 = 0;
    bzero(&play->actorCtx.unk_26C, sizeof(Input));

    MREG(52) = this->actor.world.pos.x;
    MREG(53) = this->actor.world.pos.y;
    MREG(54) = this->actor.world.pos.z;
    MREG(55) = this->actor.world.rot.y;
}

void Player_DrawGameplay(PlayState* play, Player* this, s32 lod, Gfx* cullDList,
                         OverrideLimbDrawFlex overrideLimbDraw) {
    OPEN_DISPS(play->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x0C, cullDList);
    gSPSegment(POLY_XLU_DISP++, 0x0C, cullDList);

    Player_DrawImpl(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount, lod,
                    this->transformation, 0, this->actor.shape.face, overrideLimbDraw, func_80128BD0, &this->actor);

    CLOSE_DISPS(play->state.gfxCtx);
}

void func_80846460(Player* this) {
    Vec3f* pos;
    Vec3f* bodyPartsPos;
    s32 i;

    this->actor.focus.pos.x = this->actor.world.pos.x;
    this->actor.focus.pos.z = this->actor.world.pos.z;
    this->actor.focus.pos.y = this->actor.world.pos.y + 24.0f;

    pos = &this->actor.world.pos;
    bodyPartsPos = this->bodyPartsPos;
    for (i = 0; i < ARRAY_COUNT(this->bodyPartsPos); i++) {
        Math_Vec3f_Copy(bodyPartsPos, pos);
        bodyPartsPos++;
    }

    this->bodyPartsPos[PLAYER_BODYPART_HEAD].y = this->actor.world.pos.y + 24.0f;
    this->bodyPartsPos[PLAYER_BODYPART_WAIST].y = this->actor.world.pos.y + 60.0f;
    Math_Vec3f_Copy(this->actor.shape.feetPos, pos);
    Math_Vec3f_Copy(&this->actor.shape.feetPos[1], pos);
}

struct_80124618 D_8085D428[] = {
    { 0, { 0, 0, 0 } },          { 1, { 0x50, 0xAA, 0x50 } }, { 3, { 0x64, 0x50, 0x64 } },
    { 7, { 0x64, 0x64, 0x64 } }, { 8, { 0x64, 0x64, 0x64 } },
};
struct_80124618 D_8085D450[] = {
    { 0, { 0, 0, 0 } },          { 1, { 0x50, 0xAA, 0x50 } }, { 3, { 0x64, 0x50, 0x64 } },
    { 7, { 0x64, 0x64, 0x64 } }, { 8, { 0x64, 0x64, 0x64 } },
};
struct_80124618 D_8085D478[] = {
    { 0, { 0, 0, 0 } },
    { 8, { 0, 0, 0 } },
};
struct_80124618 D_8085D488[] = {
    { 0, { 0x64, 0x64, 0x64 } }, { 1, { 0x64, 0x3C, 0x64 } }, { 3, { 0x64, 0x8C, 0x64 } },
    { 7, { 0x64, 0x50, 0x64 } }, { 9, { 0x64, 0x64, 0x64 } },
};
struct_80124618 D_8085D4B0[] = {
    { 0, { 0x64, 0x64, 0x64 } }, { 1, { 0x64, 0x46, 0x64 } }, { 3, { 0x64, 0x78, 0x64 } },
    { 6, { 0x64, 0x50, 0x64 } }, { 8, { 0x64, 0x64, 0x64 } }, { 9, { 0x64, 0x64, 0x64 } },
};
struct_80124618 D_8085D4E0[] = {
    { 0, { 0, 0, 0 } },          { 1, { 0, 0, 0 } },          { 3, { 0x64, 0x82, 0x64 } },
    { 5, { 0x82, 0x82, 0x82 } }, { 7, { 0x50, 0x5A, 0x50 } }, { 9, { 0x64, 0x64, 0x64 } },
};
struct_80124618 D_8085D510[] = {
    { 0, { 0, 0x32, 0 } },
    { 1, { 0, 0x32, 0 } },
};
struct_80124618 D_8085D520[] = {
    { 0, { 0x64, 0x78, 0x64 } },
    { 1, { 0x64, 0x78, 0x64 } },
};
struct_80124618 D_8085D530[] = {
    { 0, { 0xA0, 0x78, 0xA0 } },
    { 1, { 0xA0, 0x78, 0xA0 } },
};
struct_80124618 D_8085D540[] = {
    { 0, { 0, 0, 0 } },
    { 2, { 0x64, 0x64, 0x64 } },
};

struct_80124618* D_8085D550[3] = {
    D_8085D488,
    D_8085D4B0,
    D_8085D4E0,
};
struct_80124618* D_8085D55C[3] = {
    D_8085D428,
    D_8085D450,
    D_8085D478,
};
struct_80124618* D_8085D568[3] = {
    D_8085D510,
    D_8085D520,
    D_8085D530,
};

Gfx* D_8085D574[] = {
    object_link_nuts_DL_009C48,
    object_link_nuts_DL_009AB8,
    object_link_nuts_DL_009DB8,
};

Color_RGB8 D_8085D580 = { 255, 255, 255 };
Color_RGB8 D_8085D584 = { 80, 80, 200 };

void Player_Draw(Actor* thisx, PlayState* play) {
    Player* this = THIS;
    f32 one = 1.0f;
    s32 spEC = false;

    Math_Vec3f_Copy(&this->unk_D6C, &this->bodyPartsPos[PLAYER_BODYPART_WAIST]);
    if (this->stateFlags3 & (PLAYER_STATE3_100 | PLAYER_STATE3_40000)) {
        struct_80124618** spE8 = D_8085D550;
        struct_80124618** spE4;
        f32 spE0;
        Gfx** spDC;
        s32 i;

        OPEN_DISPS(play->state.gfxCtx);

        Matrix_Push();
        func_8012C268(play);
        spEC = true;
        if (this->stateFlags3 & PLAYER_STATE3_40000) {
            Matrix_SetTranslateRotateYXZ(this->unk_AF0[0].x, this->unk_AF0[0].y, this->unk_AF0[0].z, &gZeroVec3s);
            Matrix_Scale(this->actor.scale.x, this->actor.scale.y, this->actor.scale.z, MTXMODE_APPLY);
            spE8 = D_8085D568;
            spE0 = 0.0f;
        } else {
            Matrix_Translate(0.0f, -this->unk_ABC, 0.0f, MTXMODE_APPLY);
            spE0 = this->genericTimer - 6;
            if (spE0 < 0.0f) {
                spE8 = D_8085D55C;
                spE0 = this->unk_B86[0];
            }
        }

        spE4 = spE8;
        spDC = D_8085D574;

        for (i = 0; i < 3; i++, spE4++, spDC++) {
            Matrix_Push();
            func_80124618(*spE4, spE0, &this->unk_AF0[1]);
            Matrix_Scale(this->unk_AF0[1].x, this->unk_AF0[1].y, this->unk_AF0[1].z, MTXMODE_APPLY);
            gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_OPA_DISP++, *spDC);

            Matrix_Pop();
        }
        Matrix_Pop();

        CLOSE_DISPS(play->state.gfxCtx);
    }

    if (!(this->stateFlags2 & PLAYER_STATE2_20000000) && (this->unk_ABC > -3900.0f)) {
        OPEN_DISPS(play->state.gfxCtx);

        if (!spEC) {
            func_8012C268(play);
        }

        func_8012C2DC(play->state.gfxCtx);

        func_800B8050(&this->actor, play, 0);
        func_800B8118(&this->actor, play, 0);
        func_80122868(play, this);

        if (this->stateFlags3 & PLAYER_STATE3_1000) {
            Color_RGB8 spBC;
            f32 spB8 = this->unk_ABC + 1.0f;
            f32 spB4 = 1.0f - (this->unk_ABC * 0.5f);

            func_80846460(this);
            Matrix_Translate(this->actor.world.pos.x, this->actor.world.pos.y + (1200.0f * this->actor.scale.y * spB8),
                             this->actor.world.pos.z, MTXMODE_NEW);

            if (this->unk_B86[0] != 0) {
                Matrix_RotateYS(this->stickFlameTimer, MTXMODE_APPLY);
                Matrix_RotateXS(this->unk_B86[0], MTXMODE_APPLY);
                Matrix_RotateYS(-this->stickFlameTimer, MTXMODE_APPLY);
            }

            Matrix_RotateYS(this->actor.shape.rot.y, MTXMODE_APPLY);
            Matrix_RotateZS(this->actor.shape.rot.z, MTXMODE_APPLY);

            Matrix_Scale(this->actor.scale.x * spB4 * 1.15f, this->actor.scale.y * spB8 * 1.15f,
                         CLAMP_MIN(spB8, spB4) * this->actor.scale.z * 1.15f, MTXMODE_APPLY);
            Matrix_RotateXS(this->actor.shape.rot.x, MTXMODE_APPLY);
            Scene_SetRenderModeXlu(play, 0, 1);
            Lib_LerpRGB(&D_8085D580, &D_8085D584, this->unk_B10[0], &spBC);

            gDPSetEnvColor(POLY_OPA_DISP++, spBC.r, spBC.g, spBC.b, 255);

            gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_OPA_DISP++, gLinkGoronCurledDL);

            if (this->unk_B86[1] != 0) {
                if (this->unk_B86[1] < 3) {
                    func_80124618(D_8085D540, this->unk_B86[1], this->unk_AF0);
                    Matrix_Scale(this->unk_AF0[0].x, this->unk_AF0[0].y, this->unk_AF0[0].z, MTXMODE_APPLY);
                    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx),
                              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                }

                gSPDisplayList(POLY_OPA_DISP++, object_link_goron_DL_00C540);
            }

            func_80122BA4(play, &this->unk_3D0, 1, 255);
            func_80122BA4(play, &this->unk_3D0, 2, 255);

            if (this->unk_B86[1] < 3) {
                if (this->genericVar >= 5) {
                    f32 var_fa1;
                    u8 sp9B;

                    var_fa1 = (this->genericVar - 4) * 0.02f;

                    if (this->unk_B86[1] != 0) {
                        sp9B = (-this->unk_B86[1] * 0x55) + 0xFF;
                    } else {
                        sp9B = (200.0f * var_fa1);
                    }

                    if (this->unk_B86[1] != 0) {
                        var_fa1 = 0.65f;
                    } else {
                        var_fa1 *= one;
                    }

                    Matrix_Scale(1.0f, var_fa1, var_fa1, MTXMODE_APPLY);

                    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx),
                              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                    AnimatedMat_DrawXlu(play, Lib_SegmentedToVirtual(&object_link_goron_Matanimheader_013138));
                    gDPSetEnvColor(POLY_XLU_DISP++, 155, 0, 0, sp9B);
                    gSPDisplayList(POLY_XLU_DISP++, object_link_goron_DL_0127B0);
                    AnimatedMat_DrawXlu(play, Lib_SegmentedToVirtual(&object_link_goron_Matanimheader_014684));
                    gSPDisplayList(POLY_XLU_DISP++, object_link_goron_DL_0134D0);
                }
            }
        } else if ((this->transformation == PLAYER_FORM_GORON) && (this->stateFlags1 & PLAYER_STATE1_400000)) {
            func_80846460(this);
            SkelAnime_DrawFlexOpa(play, this->skelAnimeGoron.skeleton, this->skelAnimeGoron.jointTable,
                                  this->skelAnimeGoron.dListCount, NULL, NULL, NULL);
        } else {
            OverrideLimbDrawFlex sp84 = func_80125D4C;
            s32 lod = ((this->csMode != PLAYER_CSMODE_0) || (this->actor.projectedPos.z < 320.0f)) ? 0 : 1;
            Vec3f sp74;

            //! FAKE
            if (this->transformation == PLAYER_FORM_FIERCE_DEITY) {}
            if (this->stateFlags1 & PLAYER_STATE1_100000) {
                SkinMatrix_Vec3fMtxFMultXYZ(&play->viewProjectionMtxF, &this->actor.focus.pos, &sp74);
                if (sp74.z < -4.0f) {
                    sp84 = func_801262C8;
                }
            }

            if (this->stateFlags2 & PLAYER_STATE2_4000000) {
                s16 temp_s0_2 = play->gameplayFrames * 600;
                s16 sp70 = (play->gameplayFrames * 1000) & 0xFFFF;

                Matrix_Push();

                this->actor.scale.y = -this->actor.scale.y;
                Matrix_SetTranslateRotateYXZ(this->actor.world.pos.x,
                                             this->actor.world.pos.y + (2.0f * this->actor.depthInWater) +
                                                 (this->unk_ABC * this->actor.scale.y),
                                             this->actor.world.pos.z, &this->actor.shape.rot);
                Matrix_Scale(this->actor.scale.x, this->actor.scale.y, this->actor.scale.z, MTXMODE_APPLY);
                Matrix_RotateXS(temp_s0_2, MTXMODE_APPLY);
                Matrix_RotateYS(sp70, MTXMODE_APPLY);
                Matrix_Scale(1.1f, 0.95f, 1.05f, MTXMODE_APPLY);
                Matrix_RotateYS(-sp70, MTXMODE_APPLY);
                Matrix_RotateXS(-temp_s0_2, MTXMODE_APPLY);
                Player_DrawGameplay(play, this, lod, gCullFrontDList, sp84);
                this->actor.scale.y = -this->actor.scale.y;

                Matrix_Pop();
            }

            gSPClearGeometryMode(POLY_OPA_DISP++, G_CULL_BOTH);

            gSPClearGeometryMode(POLY_XLU_DISP++, G_CULL_BOTH);

            if ((this->transformation == PLAYER_FORM_ZORA) && (this->unk_B62 != 0) &&
                !(this->stateFlags3 & PLAYER_STATE3_8000)) {
                Matrix_Push();
                Matrix_RotateXS(-0x4000, MTXMODE_APPLY);
                Matrix_Translate(0.0f, 0.0f, -1800.0f, MTXMODE_APPLY);
                Player_DrawZoraShield(play, this);
                Matrix_Pop();
            }

            Player_DrawGameplay(play, this, lod, gCullBackDList, sp84);
        }

        func_801229A0(play, this);
        if (this->stateFlags2 & PLAYER_STATE2_4000) {
            f32 temp_fa0 = this->unk_B48;

            gSPSegment(POLY_XLU_DISP++, 0x08,
                       Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0U, -(s32)play->gameplayFrames & 0x7F, 0x20, 0x20, 1, 0,
                                        ((s32)play->gameplayFrames * -2) & 0x7F, 0x20, 0x20));

            Matrix_Scale(temp_fa0, temp_fa0, temp_fa0, MTXMODE_APPLY);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

            gDPSetEnvColor(POLY_XLU_DISP++, 0, 50, 100, 255);

            gSPDisplayList(POLY_XLU_DISP++, gEffIceFragment3DL);
        }

        if (this->getItemDrawId > 0) {
            Player_DrawGetItem(play, this);
        }

        func_80122D44(play, &this->unk_3D0);

        CLOSE_DISPS(play->state.gfxCtx);
    }

    play->actorCtx.flags &= ~ACTORCTX_FLAG_FAIRY_MASK_PARTICLES_ON;
}

void Player_Destroy(Actor* thisx, PlayState* play) {
    Player* this = (Player*)thisx;

    Effect_Destroy(play, this->meleeWeaponEffectIndex[0]);
    Effect_Destroy(play, this->meleeWeaponEffectIndex[1]);
    Effect_Destroy(play, this->meleeWeaponEffectIndex[2]);
    LightContext_RemoveLight(play, &play->lightCtx, this->lightNode);
    Collider_DestroyCylinder(play, &this->cylinder);
    Collider_DestroyCylinder(play, &this->shieldCylinder);
    Collider_DestroyQuad(play, &this->meleeWeaponQuads[0]);
    Collider_DestroyQuad(play, &this->meleeWeaponQuads[1]);
    Collider_DestroyQuad(play, &this->shieldQuad);
    ZeldaArena_Free(this->giObjectSegment);
    ZeldaArena_Free(this->maskObjectSegment);
    Magic_Reset(play);
    func_80831454(this);
}

s32 Player_SetFirstPersonAimLookAngles(PlayState* play, Player* this, s32 arg2) {
    s32 pad;
    s16 aimAngleY;

    if (!Actor_PlayerIsAimingReadyFpsItem(this) && !Player_IsAimingReadyZoraFins(this) && !arg2) {
        aimAngleY = (s16)(sPlayerControlInput->rel.stick_y * 240);
        Math_SmoothStepToS(&this->actor.focus.rot.x, aimAngleY, 0xE, 0xFA0, 0x1E);

        aimAngleY = (s16)(sPlayerControlInput->rel.stick_x * -0x10);
        aimAngleY = CLAMP(aimAngleY, -0xBB8, 0xBB8);
        this->actor.focus.rot.y += (s16)aimAngleY;
    } else {
        s16 temp3;

        temp3 = ((sPlayerControlInput->rel.stick_y >= 0) ? 1 : -1) *
                (s32)((1.0f - Math_CosS(sPlayerControlInput->rel.stick_y * 0xC8)) * 1500.0f);
        this->actor.focus.rot.x += temp3;

        if (this->stateFlags1 & PLAYER_STATE1_RIDING_HORSE) {
            this->actor.focus.rot.x = CLAMP(this->actor.focus.rot.x, -0x1F40, 0xFA0);
        } else {
            this->actor.focus.rot.x = CLAMP(this->actor.focus.rot.x, -0x36B0, 0x36B0);
        }

        aimAngleY = this->actor.focus.rot.y - this->actor.shape.rot.y;
        temp3 = ((sPlayerControlInput->rel.stick_x >= 0) ? 1 : -1) *
                (s32)((1.0f - Math_CosS(sPlayerControlInput->rel.stick_x * 0xC8)) * -1500.0f);
        aimAngleY += temp3;

        this->actor.focus.rot.y = CLAMP(aimAngleY, -0x4AAA, 0x4AAA) + this->actor.shape.rot.y;
    }

    this->lookFlags |= PLAYER_LOOKFLAGS_OVERRIDE_FOCUS_ROT_Y;

    return Player_UpdateLookAngles(this, (play->shootingGalleryStatus != 0) || Actor_PlayerIsAimingReadyFpsItem(this) ||
                                             Player_IsAimingReadyZoraFins(this));
}

void Player_UpdateSwimMovement(Player* this, f32* speed, f32 speedTarget, s16 yawTarget) {
    f32 incrStep = this->skelAnime.curFrame - 10.0f;
    f32 maxSpeed = (R_RUN_SPEED_LIMIT / 100.0f) * 0.8f;

    if (*speed > maxSpeed) {
        *speed = maxSpeed;
    }

    if ((0.0f < incrStep) && (incrStep < 16.0f)) {
        incrStep = fabsf(incrStep) * 0.5f;
    } else {
        speedTarget = 0.0f;
        incrStep = 0.0f;
    }

    Math_AsymStepToF(speed, speedTarget * 0.8f, incrStep, (fabsf(*speed) * 0.02f) + 0.05f);
    Math_ScaledStepToS(&this->currentYaw, yawTarget, 1600); // 1 ESS turn, also one frame of first-person rotation
}

void Player_SetVerticalWaterVelocity(Player* this) {
    f32 accel;
    f32 temp_fa1;
    f32 temp_fv0;
    f32 targetVelocity = -5.0f;
    f32 waterSurface = this->ageProperties->unk_28;
    f32 var_ft5_4;

    temp_fv0 = this->actor.depthInWater - waterSurface;
    if (this->actor.velocity.y < 0.0f) {
        waterSurface += 1.0f;
    }

    if (this->actor.depthInWater < waterSurface) {
        temp_fv0 = CLAMP(temp_fv0, -0.4f, -0.1f);
        accel = temp_fv0 - ((this->actor.velocity.y <= 0.0f) ? 0.0f : this->actor.velocity.y * 0.5f);
    } else {
        if (!(this->stateFlags1 & PLAYER_STATE1_IN_DEATH_CUTSCENE) &&
            (this->currentBoots >= PLAYER_BOOTS_ZORA_UNDERWATER) && (this->actor.velocity.y >= -5.0f)) {
            accel = -0.3f;
        } else if ((this->transformation == PLAYER_FORM_DEKU) && (this->actor.velocity.y < 0.0f)) {
            targetVelocity = 0.0f;
            accel = -this->actor.velocity.y;
        } else {
            targetVelocity = 2.0f;
            var_ft5_4 = CLAMP(temp_fv0, 0.1f, 0.4f);
            accel = ((this->actor.velocity.y >= 0.0f) ? 0.0f : this->actor.velocity.y * -0.3f) + var_ft5_4;
        }

        if (this->actor.depthInWater > 100.0f) {
            this->stateFlags2 |= PLAYER_STATE2_DIVING;
        }
    }

    this->actor.velocity.y += accel;
    if (((this->actor.velocity.y - targetVelocity) * accel) > 0.0f) {
        this->actor.velocity.y = targetVelocity;
    }
    this->actor.gravity = 0.0f;
}

void Player_PlaySwimAnim(PlayState* play, Player* this, Input* input, f32 linearVelocity) {
    f32 playSpeed;

    if ((input != NULL) && CHECK_BTN_ANY(input->press.button, BTN_B | BTN_A)) {
        playSpeed = 1.0f;
    } else {
        playSpeed = 0.5f;
    }

    playSpeed = playSpeed * linearVelocity;
    playSpeed = CLAMP(playSpeed, 1.0f, 2.5f);
    this->skelAnime.playSpeed = playSpeed;

    LinkAnimation_Update(play, &this->skelAnime);
}

s32 Player_SetupShootingGalleryPlay(PlayState* play, Player* this) {
    if (play->shootingGalleryStatus != 0) {
        if (play->sceneId == SCENE_20SICHITAI) {
            Player_SetAction(play, this, func_80854430, 0);
            play->shootingGalleryStatus = 0;
            this->csMode = PLAYER_CSMODE_0;
            return true;
        }

        Player_ResetAttributesAndHeldActor(play, this);
        Player_SetAction(play, this, Player_ShootingGalleryPlay, 0);
        if (!Actor_PlayerIsAimingFpsItem(this) || Player_IsHoldingHookshot(this)) {
            Player_UseItem(play, this, ITEM_BOW);
        }
        Player_AnimationPlayOnce(play, this, Player_GetStandingStillAnim(this));
        this->csMode = PLAYER_CSMODE_0;
        this->stateFlags1 |= PLAYER_STATE1_100000;
        Player_StopHorizontalMovement(this);
        Player_ResetLookAngles(this);

        return true;
    }
    return false;
}

s32 Player_ForceOcarina(PlayState* play, Player* this) {
    if (this->stateFlags3 & PLAYER_STATE3_OCARINA_AFTER_TEXTBOX) {
        this->stateFlags3 &= ~PLAYER_STATE3_OCARINA_AFTER_TEXTBOX;
        this->heldItemActionParam = PLAYER_AP_OCARINA;
        this->attentionMode = PLAYER_UNKAA5_5;
        Player_SetupItemCutsceneOrFirstPerson(this, play);
        return true;
    }
    return false;
}

void func_808479F4(PlayState* play, Player* this, f32 arg2) {
    if (this->actor.wallBgId != BGCHECK_SCENE) {
        DynaPolyActor* actor = DynaPoly_GetActor(&play->colCtx, this->actor.wallBgId);

        if (actor != NULL) {
            func_800B72F8(actor, arg2, this->actor.world.rot.y);
        }
    }
}

void func_80847A50(Player* this) {
    Player_PlaySfx(this, ((this->genericVar != 0) ? NA_SE_PL_WALK_METAL1 : NA_SE_PL_WALK_LADDER) +
                             this->ageProperties->surfaceSfxOffset);
}

Vec3f D_8085D588[] = {
    { 30.0f, 0.0f, 0.0f },
    { -30.0f, 0.0f, 0.0f },
};
Vec3f D_8085D5A0[] = {
    { 60.0f, 20.0f, 0.0f },
    { -60.0f, 20.0f, 0.0f },
};
Vec3f D_8085D5B8[] = {
    { 60.0f, -20.0f, 0.0f },
    { -60.0f, -20.0f, 0.0f },
};
Vec3f D_8085D5D0 = { 0.0f, 0.0f, -30.0f };

// related to mounting/unmounting the horse
s32 func_80847A94(PlayState* play, Player* this, s32 arg2, f32* arg3) {
    Actor* rideActor = this->rideActor;
    f32 sp60;
    f32 sp5C;
    Vec3f sp50;
    Vec3f sp44;
    CollisionPoly* sp40;
    CollisionPoly* sp3C;
    s32 sp38;
    s32 sp34;

    sp60 = rideActor->world.pos.y + 20.0f;
    sp5C = rideActor->world.pos.y - 20.0f;

    *arg3 = func_80835CD8(play, this, &D_8085D588[arg2], &sp50, &sp3C, &sp34);
    if ((sp5C < *arg3) && (*arg3 < sp60)) {
        if (!Player_WallLineTestWithOffset(play, this, &D_8085D5A0[arg2], &sp40, &sp38, &sp44)) {
            if (!Player_WallLineTestWithOffset(play, this, &D_8085D5B8[arg2], &sp40, &sp38, &sp44)) {
                this->actor.floorPoly = sp3C;
                this->actor.floorBgId = sp38;
                this->floorSfxOffset = SurfaceType_GetSfxIdOffset(&play->colCtx, sp3C, sp34);
                return true;
            }
        }
    }
    return false;
}

s32 func_80847BF0(Player* this, PlayState* play) {
    EnHorse* rideActor = (EnHorse*)this->rideActor;
    s32 var_a2; // sp38
    f32 sp34;

    if (this->genericTimer < 0) {
        this->genericTimer = 0x63;
    } else {
        var_a2 = (this->mountSide < 0) ? 0 : 1;

        if (!func_80847A94(play, this, var_a2, &sp34)) {
            var_a2 ^= 1;
            if (!func_80847A94(play, this, var_a2, &sp34)) {
                return false;
            }

            this->mountSide = -this->mountSide;
        }

        if (play->csCtx.state == CS_STATE_IDLE) {
            if (!func_8082DA90(play)) {
                if (EN_HORSE_CHECK_1(rideActor) || EN_HORSE_CHECK_4(rideActor)) {
                    this->stateFlags2 |= PLAYER_STATE2_400000;

                    if (EN_HORSE_CHECK_1(rideActor) ||
                        (EN_HORSE_CHECK_4(rideActor) && CHECK_BTN_ALL(sPlayerControlInput->press.button, BTN_A))) {
                        rideActor->actor.child = NULL;

                        Player_SetActionFuncPreserveMoveFlags(play, this, func_808505D0, 0);
                        this->unk_B48 = sp34 - rideActor->actor.world.pos.y;

                        Player_AnimationPlayOnce(play, this,
                                                 (this->mountSide < 0) ? &gPlayerAnim_link_uma_left_down
                                                                       : &gPlayerAnim_link_uma_right_down);

                        return true;
                    }
                }
            }
        }
    }

    return false;
}

// Used in 2 horse-related functions
void func_80847E2C(Player* this, f32 arg1, f32 minFrame) {
    f32 addend;
    f32 dir;

    if ((this->unk_B48 != 0.0f) && (minFrame <= this->skelAnime.curFrame)) {
        if (arg1 < fabsf(this->unk_B48)) {
            dir = (this->unk_B48 >= 0.0f) ? 1 : -1;
            addend = dir * arg1;
        } else {
            addend = this->unk_B48;
        }
        this->actor.world.pos.y += addend;
        this->unk_B48 -= addend;
    }
}

s32 func_80847ED4(Player* this) {
    return (this->interactRangeActor != NULL) && (this->interactRangeActor->id == ACTOR_EN_ZOG) &&
           CHECK_BTN_ALL(sPlayerControlInput->cur.button, BTN_A);
}

void func_80847F1C(Player* this) {
    s32 pad;
    f32 yPos;
    s16 yaw;
    Actor* interactRangeActor = this->interactRangeActor;

    if (func_80847ED4(this)) {
        yPos = this->actor.world.pos.y;
        yaw = this->currentYaw - interactRangeActor->shape.rot.y;
        Lib_Vec3f_TranslateAndRotateY(&interactRangeActor->world.pos, interactRangeActor->shape.rot.y, &D_8085D5D0,
                                      &this->actor.world.pos);
        this->actor.world.pos.y = yPos;
        this->actor.shape.rot.y = interactRangeActor->shape.rot.y;

        interactRangeActor->speedXZ = Math_CosS(ABS_ALT(yaw)) * this->linearVelocity * 0.5f;
        if (interactRangeActor->speedXZ < 0.0f) {
            interactRangeActor->speedXZ = 0.0f;
        }
        Player_SetZTargetFriendlyYaw(this);
    }
}

AnimSfxEntry sSwimAnimSfx[] = {
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 0, NA_SE_PL_SWIM, STOP),
};

void Player_SetupSwimMovement(Player* this, f32* arg1, f32 arg2, s16 arg3) {
    Player_UpdateSwimMovement(this, arg1, arg2, arg3);
    Player_PlayAnimSfx(this, sSwimAnimSfx);
    func_80847F1C(this);
}

void Player_SetupZTargetSwimming(PlayState* play, Player* this) {
    Player_SetAction(play, this, Player_ZTargetSwimming, 0);
    Player_ChangeAnimLongMorphLoop(play, this, &gPlayerAnim_link_swimer_swim);
}

s32 func_80848094(PlayState* play, Player* this, f32* arg2, s16* arg3) {
    LinkAnimationHeader* anim;
    s16 temp_v0 = this->currentYaw - *arg3;
    s32 temp_v0_2;

    if (ABS_ALT(temp_v0) > 0x6000) {
        anim = &gPlayerAnim_link_swimer_swim_wait;
        if (Math_StepToF(&this->linearVelocity, 0.0f, 1.0f)) {
            this->currentYaw = *arg3;
        } else {
            *arg2 = 0.0f;
            *arg3 = this->currentYaw;
        }
    } else {
        temp_v0_2 = func_8083E514(this, arg2, arg3, play);
        if (temp_v0_2 > 0) {
            anim = &gPlayerAnim_link_swimer_swim;
        } else if (temp_v0_2 < 0) {
            anim = &gPlayerAnim_link_swimer_back_swim;
        } else {
            s16 diff = BINANG_SUB(this->actor.shape.rot.y, *arg3);

            if (diff > 0) {
                anim = &gPlayerAnim_link_swimer_Rside_swim;
            } else {
                anim = &gPlayerAnim_link_swimer_Lside_swim;
            }
        }
    }

    if (anim != this->skelAnime.animation) {
        Player_ChangeAnimLongMorphLoop(play, this, anim);
        return true;
    }
    return false;
}

void func_808481CC(PlayState* play, Player* this, f32 arg2) {
    f32 sp2C;
    s16 sp2A;

    Player_GetTargetVelocityAndYaw(this, &sp2C, &sp2A, 0.0f, play);
    Player_UpdateSwimMovement(this, &this->linearVelocity, sp2C / 2.0f, sp2A);
    Player_UpdateSwimMovement(this, &this->actor.velocity.y, arg2, this->currentYaw);
}

void Player_EndGetItem(PlayState* play, Player* this) {
    this->getItemDrawId = 0;
    this->stateFlags1 &= ~(PLAYER_STATE1_GETTING_ITEM | PLAYER_STATE1_HOLDING_ACTOR);
    this->getItemId = GI_NONE;
    func_800E0238(Play_GetCamera(play, CAM_ID_MAIN));
}

void Player_SetupEndGetItem(PlayState* play, Player* this) {
    Player_EndGetItem(play, this);
    func_8082DC28(this);
    Player_SetupStandingStillNoMorph(this, play);
    this->currentYaw = this->actor.shape.rot.y;
}

// Player_GetItem?
s32 Player_SetupGetItemText(PlayState* play, Player* this) {
    if (this->getItemId == GI_NONE) {
        return true;
    }

    if (this->genericVar == 0) {
        GetItemEntry* giEntry = &sGetItemTable[this->getItemId - 1];

        this->genericVar = 1;
        Message_StartTextbox(play, giEntry->textId, &this->actor);
        Item_Give(play, giEntry->itemId);

        if ((this->getItemId >= GI_MASK_DEKU) && (this->getItemId <= GI_MASK_KAFEIS_MASK)) {
            Audio_PlayFanfare(NA_BGM_GET_NEW_MASK);
        } else if (((this->getItemId >= GI_RUPEE_GREEN) && (this->getItemId <= GI_RUPEE_10)) ||
                   (this->getItemId == GI_RECOVERY_HEART)) {
            play_sound(NA_SE_SY_GET_BOXITEM);
        } else {
            s32 seqId;

            if ((this->getItemId == GI_HEART_CONTAINER) ||
                ((this->getItemId == GI_HEART_PIECE) && EQ_MAX_QUEST_HEART_PIECE_COUNT)) {
                seqId = NA_BGM_GET_HEART | 0x900;
            } else {
                s32 var_v1;

                if ((this->getItemId == GI_HEART_PIECE) ||
                    ((this->getItemId >= GI_RUPEE_PURPLE) && (this->getItemId <= GI_RUPEE_HUGE))) {
                    var_v1 = NA_BGM_GET_SMALL_ITEM;
                } else {
                    var_v1 = NA_BGM_GET_ITEM | 0x900;
                }
                seqId = var_v1;
            }

            Audio_PlayFanfare(seqId);
        }
    } else if (Message_GetState(&play->msgCtx) == TEXT_STATE_CLOSING) {
        if (this->getItemId == GI_OCARINA) {
            // zelda teaching song of time cs?
            play->nextEntrance = ENTRANCE(CUTSCENE, 0);
            gSaveContext.nextCutsceneIndex = 0xFFF2;
            play->transitionTrigger = TRANS_TRIGGER_START;
            play->transitionType = TRANS_TYPE_03;
            gSaveContext.nextTransitionType = TRANS_TYPE_03;
            this->stateFlags1 &= ~PLAYER_STATE1_IN_CUTSCENE;
            Player_SetupPlayerCutscene(play, NULL, PLAYER_CSMODE_7);
        }
        this->getItemId = GI_NONE;
    }

    return false;
}

AnimSfxEntry D_8085D5E0[] = {
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 60, NA_SE_IT_MASTER_SWORD_SWING, STOP),
};

void func_808484CC(Player* this) {
    Player_PlayAnimSfx(this, D_8085D5E0);
}

void func_808484F0(Player* this) {
    this->unk_B08[0] += this->unk_B08[1];
    this->unk_B08[1] -= this->unk_B08[0] * 5.0f;
    this->unk_B08[1] *= 0.3f;

    if (fabsf(this->unk_B08[1]) < 0.00001f) {
        this->unk_B08[1] = 0.0f;
        if (fabsf(this->unk_B08[0]) < 0.00001f) {
            this->unk_B08[0] = 0.0f;
        }
    }
}

s32 func_80848570(Player* this, PlayState* play) {
    if (!Player_TrySwingingBottle(play, this)) {
        if (func_808396B8(play, this)) {
            PlayerMeleeWeaponAnimation meleeWeaponAnimation = func_808335F4(this);

            Player_StartMeleeWeaponAttack(play, this, meleeWeaponAnimation);
            if ((meleeWeaponAnimation >= PLAYER_MWA_SPIN_ATTACK_1H) ||
                ((this->transformation == PLAYER_FORM_FIERCE_DEITY) && Player_IsZTargeting(this))) {
                this->stateFlags2 |= PLAYER_STATE2_RELEASING_SPIN_ATTACK;
                Player_SetupSpinAttackActor(play, this, 0, meleeWeaponAnimation < PLAYER_MWA_SPIN_ATTACK_1H);
            }
        } else {
            return false;
        }
    }
    return true;
}

void Player_SpawnElegyShell(PlayState* play, Player* this) {
    EnTorch2* elegyShell = (EnTorch2*)play->actorCtx.elegyShells[this->transformation];
    Actor* effChange;

    if (elegyShell != NULL) {
        // Relocate existing elegy shell
        Math_Vec3f_Copy(&elegyShell->actor.home.pos, &this->actor.world.pos);
        elegyShell->actor.home.rot.y = this->actor.shape.rot.y;
        elegyShell->state = 0;
        elegyShell->framesUntilNextState = 20;
    } else {
        // Spawn a new elegy shell for this player-form
        elegyShell = (EnTorch2*)Actor_Spawn(&play->actorCtx, play, ACTOR_EN_TORCH2, this->actor.world.pos.x,
                                            this->actor.world.pos.y, this->actor.world.pos.z, 0,
                                            this->actor.shape.rot.y, 0, this->transformation);
    }

    if (elegyShell != NULL) {
        play->actorCtx.elegyShells[this->transformation] = (Actor*)elegyShell;
        Play_SetupRespawnPoint(&play->state, this->transformation + 3, PLAYER_PARAMS(0xFF, PLAYER_INITMODE_B));
    }

    effChange = Actor_Spawn(&play->actorCtx, play, ACTOR_EFF_CHANGE, this->actor.world.pos.x, this->actor.world.pos.y,
                            this->actor.world.pos.z, 0, this->actor.shape.rot.y, 0,
                            (GET_PLAYER_FORM << 3) | this->transformation);
    if (effChange != NULL) {
        //! @bug: This function should only pass Player*: it uses *(this + 0x153), which is meant to be
        //! player->currentMask, but in this case is garbage in the skelAnime
        Player_PlaySfx((Player*)effChange, NA_SE_PL_TRANSFORM);
    }
}

s32 Player_SetupStartZTargetDefend(Player* this, PlayState* play) {
    if (Player_StartZTargetDefend(play, this)) {
        return true;
    }
    return false;
}

s32 func_808487B8(Player* this, PlayState* play) {
    if (Player_StartZTargetDefend(play, this) || func_80830DF0(this, play)) {
        return true;
    }
    return false;
}

s32 Player_StartChangeItem(Player* this, PlayState* play) {
    if (LinkAnimation_Update(play, &this->skelAnimeUpper) ||
        ((Player_ItemToActionParam(this, this->heldItemId) == this->itemActionParam) &&
         (sUsingItemAlreadyInHand = sUsingItemAlreadyInHand || ((this->modelAnimType != PLAYER_ANIMTYPE_3) &&
                                                                (this->itemActionParam != PLAYER_AP_STICK) &&
                                                                (play->shootingGalleryStatus == 0))))) {
        Player_SetUpperActionFunc(play, this, D_8085C9F0[this->itemActionParam]);
        this->unk_ACC = 0;
        this->unk_AA4 = 0;
        D_80862B4C = sUsingItemAlreadyInHand;
        return this->upperActionFunc(this, play);
    }

    if (func_8082ED94(this) != 0) {
        Player_SetupChangeItem(play, this);
        Player_AnimationPlayOnce(play, this, Player_GetStandingStillAnim(this));
        this->unk_AA4 = 0;
    } else {
        Player_SetupChangeItem(play, this);
    }
    return true;
}

s32 Player_StandingDefend(Player* this, PlayState* play) {
    LinkAnimation_Update(play, &this->skelAnimeUpper);
    if (!CHECK_BTN_ALL(sPlayerControlInput->cur.button, BTN_R)) {
        func_80830CE8(play, this);
    } else {
        this->stateFlags1 |= PLAYER_STATE1_400000;
        Player_SetModelsForHoldingShield(this);
        if ((this->transformation == PLAYER_FORM_ZORA) && CHECK_BTN_ALL(sPlayerControlInput->cur.button, BTN_B)) {
            Player_SetupZoraBarrier(this, BTN_R | BTN_B);
        }
    }
    return true;
}

s32 func_80848A0C(Player* this, PlayState* play) {
    if (LinkAnimation_Update(play, &this->skelAnimeUpper)) {
        LinkAnimationHeader* anim;
        f32 lastFrame;

        anim = Player_GetStandingDefendAnim(play, this);
        lastFrame = Animation_GetLastFrame(anim);
        LinkAnimation_Change(play, &this->skelAnimeUpper, anim, 1.0f, lastFrame, lastFrame, ANIMMODE_ONCE, 0.0f);
    }

    this->stateFlags1 |= PLAYER_STATE1_400000;
    Player_SetModelsForHoldingShield(this);
    return true;
}

s32 func_80848AB0(Player* this, PlayState* play) {
    // possibly a case of if ((sUsingItemAlreadyInHand = D_80862B4C) || LinkAnimation_Update(play,
    // &this->skelAnimeUpper))
    sUsingItemAlreadyInHand = D_80862B4C;
    if ((sUsingItemAlreadyInHand != 0) || LinkAnimation_Update(play, &this->skelAnimeUpper)) {
        Player_SetUpperActionFunc(play, this, D_8085C9F0[this->itemActionParam]);
        LinkAnimation_PlayLoop(play, &this->skelAnimeUpper, D_8085BE84[PLAYER_ANIMGROUP_0][this->modelAnimType]);
        this->unk_AA4 = 0;
        this->upperActionFunc(this, play);
        return false;
    }
    return true;
}

s32 func_80848B6C(Player* this, PlayState* play) {
    if (this->stickFlameTimer >= 0) {
        this->stickFlameTimer = -this->stickFlameTimer;
    }

    if (!Player_IsHoldingHookshot(this) || Player_EndHookshotMove(play, this)) {
        if (!Player_StartZTargetDefend(play, this) && !Player_CanUseFpsItem(this, play)) {
            return false;
        }
    }
    return true;
}

LinkAnimationHeader* D_8085D5E4[] = {
    &gPlayerAnim_link_hook_walk2ready,
    &gPlayerAnim_link_bow_walk2ready,
    &gPlayerAnim_pn_tamahakidf,
};

LinkAnimationHeader* D_8085D5F0[] = {
    &gPlayerAnim_link_hook_wait,
    &gPlayerAnim_link_bow_bow_wait,
    &gPlayerAnim_pn_tamahakidf,
};

u16 D_8085D5FC[] = { NA_SE_IT_BOW_FLICK, NA_SE_PL_DEKUNUTS_MISS_FIRE, 0, 0 };

s32 func_80848BF4(Player* this, PlayState* play) {
    s32 index;
    s32 temp;

    if (Player_IsHoldingHookshot(this)) {
        index = 0;
    } else {
        temp = (this->transformation != PLAYER_FORM_DEKU) ? 1 : 2;
        index = temp;
    }

    if (this->transformation != PLAYER_FORM_DEKU) {
        Math_ScaledStepToS(&this->upperBodyRot.z, 0x4B0, 0x190);
        this->lookFlags |= 0x100;
    }

    if ((this->unk_ACE == 0) && (func_8082ED94(this) == 0) &&
        (this->skelAnime.animation == &gPlayerAnim_link_bow_side_walk)) {
        LinkAnimation_PlayOnce(play, &this->skelAnimeUpper, D_8085D5E4[index]);
        this->unk_ACE = -1;
    } else if (LinkAnimation_Update(play, &this->skelAnimeUpper)) {
        LinkAnimation_PlayLoop(play, &this->skelAnimeUpper, D_8085D5F0[index]);
        this->unk_ACE = 1;
    } else if (this->unk_ACE == 1) {
        this->unk_ACE = 2;
    }

    if (this->unk_ACC >= 0xB) {
        this->unk_ACC--;
    }

    Player_SetupAimAttention(this, play);
    if ((this->unk_ACE > 0) && ((this->stickFlameTimer < 0) || ((D_80862B4C == 0) && !func_80830FD4(play)))) {
        Player_SetUpperActionFunc(play, this, func_80848E4C);
        if (this->stickFlameTimer >= 0) {
            if (index != 0) {
                if (!func_80831194(play, this)) {
                    Player_PlaySfx(this, D_8085D5FC[this->stickFlameTimer - 1]);
                }

                if (this->transformation == PLAYER_FORM_DEKU) {
                    LinkAnimation_PlayOnceSetSpeed(play, &this->skelAnimeUpper, &gPlayerAnim_pn_tamahaki, 2.0f / 3.0f);
                }
            } else if (this->actor.bgCheckFlags & 1) {
                func_80831194(play, this);
            }
        }
        this->unk_ACC = 0xA;
        Player_StopHorizontalMovement(this);
    } else {
        this->stateFlags3 |= PLAYER_STATE3_40;
    }

    return 1;
}

s32 func_80848E4C(Player* this, PlayState* play) {
    s32 animFinished = LinkAnimation_Update(play, &this->skelAnimeUpper);

    if (Player_IsHoldingHookshot(this) && !Player_EndHookshotMove(play, this)) {
        return 1;
    }

    if (!Player_StartZTargetDefend(play, this) &&
        ((((this->stickFlameTimer < 0) && (D_80862B4C != 0)) ||
          ((animFinished || (this->transformation != PLAYER_FORM_DEKU)) && (sUsingItemAlreadyInHand != 0))) ||
         Player_CheckShootingGalleryShootInput(play))) {

        this->stickFlameTimer = ABS_ALT(this->stickFlameTimer);
        if (func_808306F8(this, play)) {
            if (Player_IsHoldingHookshot(this)) {
                this->unk_ACE = 1;
            } else {
                LinkAnimation_PlayOnce(play, &this->skelAnimeUpper,
                                       (this->transformation == PLAYER_FORM_DEKU)
                                           ? &gPlayerAnim_pn_tamahakidf
                                           : &gPlayerAnim_link_bow_bow_shoot_next);
            }
        }
    } else {
        if (this->unk_ACC != 0) {
            this->unk_ACC--;
        }

        if ((Player_IsZTargeting(this)) || (this->attentionMode != PLAYER_ATTENTIONMODE_NONE) ||
            (this->stateFlags1 & PLAYER_STATE1_100000)) {
            if (this->unk_ACC == 0) {
                this->unk_ACC++;
            }
            return 1;
        }

        if (Player_IsHoldingHookshot(this)) {
            Player_SetUpperActionFunc(play, this, func_80848B6C);
        } else {
            Player_SetUpperActionFunc(play, this, func_80849054);
            LinkAnimation_PlayOnce(play, &this->skelAnimeUpper,
                                   (this->transformation == PLAYER_FORM_DEKU) ? &gPlayerAnim_pn_tamahakidf
                                                                              : &gPlayerAnim_link_bow_bow_shoot_end);
        }
        this->unk_ACC = 0;
    }

    return 1;
}

s32 func_80849054(Player* this, PlayState* play) {
    if (!(this->actor.bgCheckFlags & 1) || LinkAnimation_Update(play, &this->skelAnimeUpper)) {
        Player_SetUpperActionFunc(play, this, func_80848B6C);
    }
    return true;
}

s32 Player_HoldActor(Player* this, PlayState* play) {
    Actor* heldActor = this->heldActor;

    if (heldActor == NULL) {
        Player_SetupHeldItemUpperActionFunc(play, this);
    }

    if (Player_StartZTargetDefend(play, this)) {
        return true;
    }

    if (this->stateFlags1 & PLAYER_STATE1_HOLDING_ACTOR) {
        if (LinkAnimation_Update(play, &this->skelAnimeUpper)) {
            LinkAnimation_PlayLoop(play, &this->skelAnimeUpper, &gPlayerAnim_link_normal_carryB_wait);
        }

        if ((heldActor->id == ACTOR_EN_NIW) && (this->actor.velocity.y <= 0.0f)) {
            this->actor.terminalVelocity = -2.0f;
            this->actor.gravity = -0.5f;
            this->fallStartHeight = this->actor.world.pos.y;
        }
        return true;
    }
    return Player_SetupStartZTargetDefend(this, play);
}

s32 func_808491B4(Player* this, PlayState* play) {
    if (Player_StartZTargetDefend(play, this)) {
        return true;
    }

    if (this->stateFlags1 & PLAYER_STATE1_2000000) {
        Player_SetUpperActionFunc(play, this, Player_WaitForThrownZoraFins);
    } else if (Player_CanUseFpsItem(this, play)) {
        return true;
    }

    return false;
}

s32 func_8084923C(Player* this, PlayState* play) {
    if (LinkAnimation_Update(play, &this->skelAnimeUpper)) {
        Player_SetUpperActionFunc(play, this, func_808492C4);
        LinkAnimation_PlayLoop(play, &this->skelAnimeUpper, &gPlayerAnim_pz_cutterwaitanim);
    }
    if (this->skelAnimeUpper.animation == &gPlayerAnim_pz_cutterwaitanim) {
        Player_SetupAimAttention(this, play);
    }
    return true;
}

s32 func_808492C4(Player* this, PlayState* play) {
    LinkAnimation_Update(play, &this->skelAnimeUpper);
    Player_SetupAimAttention(this, play);
    if (D_80862B4C == 0) {
        Player_SetUpperActionFunc(play, this, func_8084933C);
        LinkAnimation_PlayOnce(play, &this->skelAnimeUpper, &gPlayerAnim_pz_cutterattack);
    }
    return true;
}

s32 func_8084933C(Player* this, PlayState* play) {
    if (LinkAnimation_Update(play, &this->skelAnimeUpper)) {
        Player_SetUpperActionFunc(play, this, Player_WaitForThrownZoraFins);
        this->unk_ACC = 0;
    } else if (LinkAnimation_OnFrame(&this->skelAnimeUpper, 6.0f)) {
        Vec3f pos;
        s16 untargetedRotY;

        Player_GetWorldPosRelativeToPlayer(&this->bodyPartsPos[PLAYER_BODYPART_L_HAND], this->actor.shape.rot.y, 0.0f,
                                           &pos);
        pos.y = this->actor.world.pos.y + 50.0f;

        untargetedRotY = this->actor.shape.rot.y - 0x190;
        this->boomerangActor = Actor_Spawn(
            &play->actorCtx, play, ACTOR_EN_BOOM, pos.x, pos.y, pos.z, this->actor.focus.rot.x,
            (this->targetedActor != NULL) ? this->actor.shape.rot.y + 0x36B0 : untargetedRotY, 0, ZORA_BOOMERANG_LEFT);

        if (this->boomerangActor != NULL) {
            EnBoom* leftBoomerang = (EnBoom*)this->boomerangActor;
            EnBoom* rightBoomerang;

            leftBoomerang->moveTo = this->targetedActor;
            if (leftBoomerang->moveTo != NULL) {
                leftBoomerang->unk_1CF = 0x10;
            }
            leftBoomerang->unk_1CC = leftBoomerang->unk_1CF + 0x24;

            Player_GetWorldPosRelativeToPlayer(&this->bodyPartsPos[PLAYER_BODYPART_R_HAND], this->actor.shape.rot.y,
                                               0.0f, &pos);

            untargetedRotY = (this->actor.shape.rot.y + 0x190);
            rightBoomerang =
                (EnBoom*)Actor_Spawn(&play->actorCtx, play, ACTOR_EN_BOOM, pos.x, pos.y, pos.z, this->actor.focus.rot.x,
                                     (this->targetedActor != NULL) ? this->actor.shape.rot.y - 0x36B0 : untargetedRotY,
                                     0, ZORA_BOOMERANG_RIGHT);

            if (rightBoomerang != NULL) {
                rightBoomerang->moveTo = this->targetedActor;
                if (rightBoomerang->moveTo != NULL) {
                    rightBoomerang->unk_1CF = 0x10;
                }

                rightBoomerang->unk_1CC = rightBoomerang->unk_1CF + 0x24;
                leftBoomerang->actor.child = &rightBoomerang->actor;
                rightBoomerang->actor.parent = &leftBoomerang->actor;
            }

            this->stateFlags1 |= PLAYER_STATE1_2000000;
            this->stateFlags3 &= ~PLAYER_STATE3_800000;
            if (!Player_IsUnfriendlyZTargeting(this)) {
                Player_SetZTargetFriendlyYaw(this);
            }

            this->fpsItemShotTimer = 20;

            Player_PlaySfx(this, NA_SE_IT_BOOMERANG_THROW);
            Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_SWORD_N);
        }
    }

    return true;
}

s32 Player_WaitForThrownZoraFins(Player* this, PlayState* play) {
    if (Player_StartZTargetDefend(play, this)) {
        return true;
    }

    if (this->stateFlags3 & PLAYER_STATE3_800000) {
        Player_SetUpperActionFunc(play, this, Player_CatchZoraFins);
        LinkAnimation_PlayOnce(play, &this->skelAnimeUpper, &gPlayerAnim_pz_cuttercatch);
        this->stateFlags3 &= ~PLAYER_STATE3_800000;
        Player_PlaySfx(this, NA_SE_PL_CATCH_BOOMERANG);
        Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_SWORD_N);
        return true;
    }

    return false;
}

s32 Player_CatchZoraFins(Player* this, PlayState* play) {
    if (!func_808491B4(this, play) && LinkAnimation_Update(play, &this->skelAnimeUpper)) {
        if (this->stateFlags1 & PLAYER_STATE1_2000000) {
            Player_SetUpperActionFunc(play, this, Player_WaitForThrownZoraFins);
            this->unk_ACC = 0;
        } else {
            Player_SetUpperActionFunc(play, this, Player_SetupStartZTargetDefend);
        }
    }
    return true;
}

void func_808496AC(Player* this, PlayState* play) {
    LinkAnimation_Update(play, &this->skelAnime);
    func_808323C0(this, play->playerActorCsIds[2]);

    if (DECR(this->genericTimer) == 0) {
        if (Message_GetState(&play->msgCtx) == TEXT_STATE_NONE) {
            func_80838760(this);
            Player_SetAction(play, this, Player_StandingStill, 0);
            this->stateFlags1 &= ~PLAYER_STATE1_IN_CUTSCENE;
        }
    } else if (this->genericTimer == 30) {
        if (Message_GetState(&play->msgCtx) != TEXT_STATE_NONE) {
            this->genericTimer += 1;
        } else {
            Message_StartTextbox(play, 0xC03, NULL);
        }
    }
}

void func_808497A0(Player* this, PlayState* play) {
    this->stateFlags3 |= PLAYER_STATE3_10000000;
    LinkAnimation_Update(play, &this->skelAnime);
    Player_SetupCurrentUpperAction(this, play);

    if (MREG(64) == 0) {
        MREG(64) = 0x14;
        MREG(68) = 0;
        MREG(65) = MREG(66) = MREG(67) = MREG(68);
        play_sound(NA_SE_SY_DEKUNUTS_JUMP_FAILED);
    } else if (MREG(64) > 0) {
        MREG(68) += MREG(64);
        if (MREG(68) > 0xFF) {
            MREG(68) = 0xFF;
            if (this->unk_B86[0] == 0) {
                this->unk_B86[0] = 1;
                Player_ResetAttributesAndHeldActor(play, this);
            } else {
                MREG(64) = -0x14;
                this->stateFlags1 &= ~PLAYER_STATE1_SWIMMING;
                this->actor.bgCheckFlags &= ~1;
                Player_SetEquipmentData(play, this);
                this->prevBoots = this->currentBoots;

                if (this->unk_3CF != 0) {
                    Math_Vec3f_Copy(&this->actor.world.pos, &this->unk_3C0);
                    this->actor.shape.rot.y = this->unk_3CC;
                } else {
                    Math_Vec3f_Copy(&this->actor.world.pos, &gSaveContext.respawn[RESPAWN_MODE_DOWN].pos);
                    this->actor.shape.rot.y = gSaveContext.respawn[RESPAWN_MODE_DOWN].yaw;
                }

                Math_Vec3f_Copy(&this->actor.prevPos, &this->actor.world.pos);
                this->linearVelocity = 0.0f;
                this->currentYaw = this->actor.shape.rot.y;
                this->actor.velocity.y = 0.0f;
                Player_AnimationPlayOnce(play, this, Player_GetStandingStillAnim(this));

                if ((play->roomCtx.curRoom.num == this->unk_3CE) && (play->roomCtx.prevRoom.num < 0)) {
                    this->genericTimer = 5;
                } else {
                    play->roomCtx.curRoom.num = -1;
                    play->roomCtx.prevRoom.num = -1;
                    play->roomCtx.curRoom.segment = NULL;
                    play->roomCtx.prevRoom.segment = NULL;

                    func_8012EBF8(play, &play->roomCtx);
                    this->genericTimer = -1;
                    this->genericVar = this->unk_3CE;
                }
            }
        }
    } else if (this->genericTimer < 0) {
        if (Room_StartRoomTransition(play, &play->roomCtx, this->genericVar)) {
            Map_InitRoomData(play, play->roomCtx.curRoom.num);
            Minimap_SavePlayerRoomInitInfo(play);
            this->genericTimer = 5;
        }
    } else if (this->genericTimer > 0) {
        this->genericTimer--;
    } else {
        MREG(68) += MREG(64);
        if (MREG(68) < 0) {
            MREG(68) = 0;
            MREG(64) = 0;
            Player_SetupInvincibilityNoDamageFlash(this, -40);
            Player_SetupStandingStillMorph(this, play);
            this->actor.bgCheckFlags |= 1;
        }
    }
}

void Player_UnfriendlyZTargetStandingStill(Player* this, PlayState* play) {
    f32 sp44;
    s16 sp42;
    s32 temp_v0;

    if (this->genericTimer != 0) {
        if (LinkAnimation_Update(play, &this->skelAnime)) {
            Player_EndAnimMovement(this);
            Player_AnimationPlayLoop(play, this, Player_GetFightingRightAnim(this));
            this->genericTimer = 0;
            this->stateFlags3 &= ~PLAYER_STATE3_8;
        }
        func_8082FC60(this);
    } else {
        func_8083E958(play, this);
    }

    Player_StepLinearVelocityToZero(this);
    if (Player_SetupSubAction(play, this, sTargetEnemyStandStillSubActions, 1)) {
        return;
    }

    if (!Player_SetupStartUnfriendlyZTargeting(this) &&
        (!Player_IsFriendlyZTargeting(this) || (Player_StandingDefend != this->upperActionFunc))) {
        Player_SetupEndUnfriendlyZTarget(this, play);
        return;
    }

    Player_GetTargetVelocityAndYaw(this, &sp44, &sp42, 0.0f, play);

    temp_v0 = func_8083E404(this, sp44, sp42);
    if (temp_v0 > 0) {
        Player_SetupZTargetRunning(this, play, sp42);
    } else if (temp_v0 < 0) {
        Player_SetupUnfriendlyBackwalk(this, sp42, play);
    } else if (sp44 > 4.0f) {
        Player_SetupSidewalk(this, play);
    } else {
        u32 temp_v0_2;

        Player_SetupWalkSfx(this, this->linearVelocity * 0.3f + 1.0f);
        func_8083E8E0(this, sp44, sp42);

        temp_v0_2 = this->walkFrame;
        if ((temp_v0_2 < 6) || ((temp_v0_2 - 0xE) < 6)) {
            Math_StepToF(&this->linearVelocity, 0.0f, 1.5f);
        } else {
            s16 temp_v0_3 = sp42 - this->currentYaw;
            s32 var_v1; // sp30

            var_v1 = ABS_ALT(temp_v0_3);
            if (var_v1 > 0x4000) {
                if (Math_StepToF(&this->linearVelocity, 0.0f, 1.5f) != 0) {
                    this->currentYaw = sp42;
                }
            } else {
                Math_AsymStepToF(&this->linearVelocity, sp44 * 0.3f, 2.0f, 1.5f);
                Math_ScaledStepToS(&this->currentYaw, sp42, var_v1 * 0.1f);
            }
        }
    }
}

void Player_FriendlyZTargetStandingStill(Player* this, PlayState* play) {
    f32 sp3C;
    s16 sp3A;
    s32 temp_v0;

    if (LinkAnimation_Update(play, &this->skelAnime)) {
        Player_EndAnimMovement(this);
        Player_AnimationPlayOnce(play, this, Player_GetStandingStillAnim(this));
        this->stateFlags3 &= ~PLAYER_STATE3_8;
    }

    Player_StepLinearVelocityToZero(this);
    if (Player_SetupSubAction(play, this, sFriendlyTargetingStandStillSubActions, 1)) {
        return;
    }

    if (Player_SetupStartUnfriendlyZTargeting(this)) {
        Player_SetupUnfriendlyZTarget(this, play);
        return;
    }
    if (!Player_IsFriendlyZTargeting(this)) {
        Player_SetActionFuncPreserveMoveFlags(play, this, Player_StandingStill, 1);
        this->currentYaw = this->actor.shape.rot.y;
        return;
    }
    if (Player_StandingDefend == this->upperActionFunc) {
        Player_SetupUnfriendlyZTarget(this, play);
        return;
    }

    Player_GetTargetVelocityAndYaw(this, &sp3C, &sp3A, 0.0f, play);

    temp_v0 = func_8083E514(this, &sp3C, &sp3A, play);
    if (temp_v0 > 0) {
        Player_SetupZTargetRunning(this, play, sp3A);
    } else if (temp_v0 < 0) {
        Player_SetupFriendlyBackwalk(this, sp3A, play);
    } else if (sp3C > 4.9f) {
        Player_SetupSidewalk(this, play);
        func_8082FC60(this);
    } else if (sp3C != 0.0f) {
        Player_SetupFriendlySidewalk(this, play);
    } else {
        s16 temp_v0_2 = sp3A - this->actor.shape.rot.y;

        if (ABS_ALT(temp_v0_2) > 0x320) {
            Player_SetupTurn(play, this, sp3A);
        }
    }
}

void Player_StandingStill(Player* this, PlayState* play) {
    s32 temp_v0 = func_8082ED94(this);
    s32 sp40 = LinkAnimation_Update(play, &this->skelAnime);
    f32 sp3C;
    s16 sp3A;
    s16 temp_v1_2;

    func_8083C85C(this);
    if (temp_v0 > 0) {
        func_8082EEA4(this, temp_v0 - 1);
    }

    if (sp40 ||
        ((this->currentMask == PLAYER_MASK_SCENTS) && (this->skelAnime.animation != &gPlayerAnim_cl_msbowait)) ||
        ((this->currentMask != PLAYER_MASK_SCENTS) && (this->skelAnime.animation == &gPlayerAnim_cl_msbowait))) {
        if (this->genericTimer != 0) {
            if (DECR(this->genericTimer) == 0) {
                this->skelAnime.endFrame = this->skelAnime.animLength - 1.0f;
            }

            this->skelAnime.jointTable[PLAYER_LIMB_ROOT - 1].y =
                (this->skelAnime.jointTable[PLAYER_LIMB_ROOT - 1].y + ((this->genericTimer & 1) * 0x50)) - 0x28;
        } else {
            Player_EndAnimMovement(this);
            Player_ChooseIdleAnim(play, this);
        }
        this->stateFlags3 &= ~PLAYER_STATE3_8;
    }

    Player_StepLinearVelocityToZero(this);
    if ((this->genericTimer == 0) && !Player_SetupShootingGalleryPlay(play, this) &&
        !Player_SetupSubAction(play, this, sStandStillSubActions, 1)) {
        if (Player_SetupStartUnfriendlyZTargeting(this)) {
            Player_SetupUnfriendlyZTarget(this, play);
            return;
        }
        if (Player_IsFriendlyZTargeting(this)) {
            Player_SetupFriendlyZTargetingStandStill(this, play);
            return;
        }
        Player_GetTargetVelocityAndYaw(this, &sp3C, &sp3A, 0.018f, play);
        if (sp3C != 0.0f) {
            Player_SetupZTargetRunning(this, play, sp3A);
            return;
        }
        temp_v1_2 = sp3A - this->actor.shape.rot.y;

        if (ABS_ALT(temp_v1_2) > 0x320) {
            Player_SetupTurn(play, this, sp3A);
        } else {
            Math_ScaledStepToS(&this->actor.shape.rot.y, sp3A, 0x4B0);
            this->currentYaw = this->actor.shape.rot.y;
            if (Player_GetStandingStillAnim(this) == this->skelAnime.animation) {
                Player_SetLookAngle(this, play);
            }
        }
    }
}

void Player_EndSidewalk(Player* this, PlayState* play) {
    f32 var_fv0;
    s16 temp_v0_3;
    f32 sp44;
    s16 sp42;
    s32 var_v0;
    s32 temp_v0_2;
    s32 var_v1;
    f32 var_fv1;

    this->skelAnime.mode = 0;
    LinkAnimation_SetUpdateFunction(&this->skelAnime);

    this->skelAnime.animation = func_8082EFE4(this);
    if (this->skelAnime.animation == &gPlayerAnim_link_bow_side_walk) {
        var_fv0 = 24.0f;
        var_fv1 = -(MREG(95) / 100.0f);
    } else {
        var_fv0 = 29.0f;
        var_fv1 = MREG(95) / 100.0f;
    }

    this->skelAnime.animLength = var_fv0;
    this->skelAnime.endFrame = var_fv0 - 1.0f;
    if (BINANG_SUB(this->currentYaw, this->actor.shape.rot.y) >= 0) {
        var_v0 = 1;
    } else {
        var_v0 = -1;
    }

    this->skelAnime.playSpeed = var_v0 * (this->linearVelocity * var_fv1);

    LinkAnimation_Update(play, &this->skelAnime);
    if (LinkAnimation_OnFrame(&this->skelAnime, 0.0f) || LinkAnimation_OnFrame(&this->skelAnime, var_fv0 / 2.0f)) {
        Player_AnimSfx_PlayFloorWalk(this, this->linearVelocity);
    }

    if (Player_SetupSubAction(play, this, sEndSidewalkSubActions, 1)) {
        return;
    }

    if (Player_SetupStartUnfriendlyZTargeting(this)) {
        Player_SetupUnfriendlyZTarget(this, play);
        return;
    }
    if (!Player_IsFriendlyZTargeting(this)) {
        Player_SetupStandingStillMorph(this, play);
        return;
    }
    Player_GetTargetVelocityAndYaw(this, &sp44, &sp42, 0.0f, play);

    temp_v0_2 = func_8083E514(this, &sp44, &sp42, play);
    if (temp_v0_2 > 0) {
        Player_SetupZTargetRunning(this, play, sp42);
        return;
    }
    if (temp_v0_2 < 0) {
        Player_SetupFriendlyBackwalk(this, sp42, play);
        return;
    }
    if (sp44 > 4.9f) {
        Player_SetupSidewalk(this, play);
        func_8082FC60(this);
        return;
    }
    if ((sp44 == 0.0f) && (this->linearVelocity == 0.0f)) {
        Player_SetupFriendlyZTargetingStandStill(this, play);
        return;
    }

    temp_v0_3 = sp42 - this->currentYaw;
    var_v1 = ABS_ALT(temp_v0_3);
    if (var_v1 > 0x4000) {
        if (Math_StepToF(&this->linearVelocity, 0.0f, 1.5f)) {
            this->currentYaw = sp42;
        }
    } else {
        Math_AsymStepToF(&this->linearVelocity, sp44 * 0.4f, 1.5f, 1.5f);
        Math_ScaledStepToS(&this->currentYaw, sp42, var_v1 * 0.1f);
    }
}

void Player_FriendlyBackwalk(Player* this, PlayState* play) {
    f32 sp34;
    s16 sp32;
    s32 sp2C;

    func_8083EE60(this, play);
    if (Player_SetupSubAction(play, this, sFriendlyBackwalkSubActions, 1)) {
        return;
    }

    if (Player_IsZTargetingSetupStartUnfriendly(this) == 0) {
        Player_SetupZTargetRunning(this, play, this->currentYaw);
        return;
    }

    Player_GetTargetVelocityAndYaw(this, &sp34, &sp32, 0.0f, play);

    sp2C = func_8083E514(this, &sp34, &sp32, play);
    if (sp2C >= 0) {
        if (!func_8083F190(this, &sp34, &sp32, play)) {
            if (sp2C != 0) {
                Player_SetupRun(this, play);
            } else if (sp34 > 4.9f) {
                Player_SetupSidewalk(this, play);
            } else {
                Player_SetupFriendlySidewalk(this, play);
            }
        }
    } else {
        s16 sp2A = sp32 - this->currentYaw;

        Math_AsymStepToF(&this->linearVelocity, sp34 * 1.5f, 1.5f, 2.0f);
        Math_ScaledStepToS(&this->currentYaw, sp32, sp2A * 0.1f);
        if ((sp34 == 0.0f) && (this->linearVelocity == 0.0f)) {
            Player_SetupFriendlyZTargetingStandStill(this, play);
        }
    }
}

void func_8084A794(Player* this, PlayState* play) {
    s32 sp34 = LinkAnimation_Update(play, &this->skelAnime);
    f32 sp30;
    s16 sp2E;

    Player_StepLinearVelocityToZero(this);

    if (Player_SetupSubAction(play, this, sFriendlyBackwalkSubActions, 1)) {
        return;
    }

    Player_GetTargetVelocityAndYaw(this, &sp30, &sp2E, 0.0f, play);
    if (this->linearVelocity != 0.0f) {
        return;
    }

    this->currentYaw = this->actor.shape.rot.y;
    if (func_8083E514(this, &sp30, &sp2E, play) > 0) {
        Player_SetupRun(this, play);
    } else if ((sp30 != 0.0f) || sp34) {
        func_8083F230(this, play);
    }
}

void func_8084A884(Player* this, PlayState* play) {
    s32 sp1C = LinkAnimation_Update(play, &this->skelAnime);

    if (Player_SetupSubAction(play, this, sFriendlyBackwalkSubActions, 1)) {
        return;
    }

    if (sp1C) {
        Player_SetupFriendlyZTargetingStandStill(this, play);
    }
}

void Player_Sidewalk(Player* this, PlayState* play) {
    f32 sp3C;
    s16 sp3A;
    s32 var_v0;

    func_8083F27C(play, this);
    if (Player_SetupSubAction(play, this, sSidewalkSubActions, 1)) {
        return;
    }

    if (!Player_IsZTargetingSetupStartUnfriendly(this)) {
        Player_SetupRun(this, play);
        return;
    }

    Player_GetTargetVelocityAndYaw(this, &sp3C, &sp3A, 0.0f, play);
    if (Player_IsFriendlyZTargeting(this)) {
        var_v0 = func_8083E514(this, &sp3C, &sp3A, play);
    } else {
        var_v0 = func_8083E404(this, sp3C, sp3A);
    }

    if (var_v0 > 0) {
        Player_SetupRun(this, play);
    } else if (var_v0 < 0) {
        if (Player_IsFriendlyZTargeting(this)) {
            Player_SetupFriendlyBackwalk(this, sp3A, play);
        } else {
            Player_SetupUnfriendlyBackwalk(this, sp3A, play);
        }
    } else if ((this->linearVelocity < 3.6f) && (sp3C < 4.0f)) {
        if ((Player_IsUnfriendlyZTargeting(this) == 0) && (Player_IsFriendlyZTargeting(this))) {
            Player_SetupFriendlySidewalk(this, play);
        } else {
            Player_SetupStandingStillType(this, play);
        }
    } else {
        s16 temp_v0;
        s32 var_v1;
        s32 pad;

        func_8083E8E0(this, sp3C, sp3A);

        temp_v0 = sp3A - this->currentYaw;
        var_v1 = ABS_ALT(temp_v0);
        if (var_v1 > 0x4000) {
            if (Math_StepToF(&this->linearVelocity, 0.0f, 3.0f)) {
                this->currentYaw = sp3A;
            }
        } else {
            sp3C = sp3C * 0.9f;
            Math_AsymStepToF(&this->linearVelocity, sp3C, 2.0f, 3.0f);
            Math_ScaledStepToS(&this->currentYaw, sp3A, var_v1 * 0.1f);
        }
    }
}

void Player_Turn(Player* this, PlayState* play) {
    f32 sp34;
    s16 sp32;

    LinkAnimation_Update(play, &this->skelAnime);
    if (Player_IsHoldingTwoHandedWeapon(this)) {
        AnimationContext_SetLoadFrame(play, Player_GetStandingStillAnim(this), 0, this->skelAnime.limbCount,
                                      this->skelAnime.morphTable);
        AnimationContext_SetCopyTrue(play, this->skelAnime.limbCount, this->skelAnime.jointTable,
                                     this->skelAnime.morphTable, D_8085B9F0);
    }

    Player_GetTargetVelocityAndYaw(this, &sp34, &sp32, 0.018f, play);
    if ((this != GET_PLAYER(play)) && (this->targetedActor == NULL)) {
        sp32 = this->actor.home.rot.y;
    }

    if (Player_SetupSubAction(play, this, sTurnSubActions, 1)) {
        return;
    }

    if (sp34 != 0.0f) {
        this->actor.shape.rot.y = sp32;
        Player_SetupRun(this, play);
    } else if (Math_ScaledStepToS(&this->actor.shape.rot.y, sp32, this->unk_B4E)) {
        Player_SetupStandingStillNoMorph(this, play);
    }
    this->currentYaw = this->actor.shape.rot.y;
}

void func_8084AC84(Player* this, PlayState* play) {
    this->stateFlags2 |= PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING;

    if (this->linearVelocity < 1.0f) {
        this->skelAnime.animation = &gPlayerAnim_clink_normal_okarina_walk;
    } else {
        this->skelAnime.animation = &gPlayerAnim_clink_normal_okarina_walkB;
    }
    LinkAnimation_Update(play, &this->skelAnime);

    if (!Player_SetupShootingGalleryPlay(play, this) &&
        (!Player_SetupSubAction(play, this, sStandStillSubActions, 1) || (func_8084AC84 == this->actionFunc))) {
        f32 sp3C;
        f32 temp_fv0;
        f32 temp_fv1;
        s16 sp32;
        s16 sp30;

        if (!CHECK_BTN_ALL(sPlayerControlInput->cur.button, BTN_B)) {
            Player_SetupStandingStillNoMorph(this, play);
            return;
        }

        this->linearVelocity = this->unk_B48;
        Player_GetTargetVelocityAndYaw(this, &sp3C, &sp32, 0.018f, play);
        sp30 = sp32;

        if (!func_8083A4A4(this, &sp3C, &sp32, REG(43) / 100.0f)) {
            Player_SetRunVelocityAndYawImpl(this, sp3C, sp32, REG(19) / 100.0f, 1.5f, 0x3E8);
            func_8083C8E8(this, play);
            if ((this->linearVelocity == 0.0f) && (sp3C == 0.0f)) {
                this->currentYaw = sp30;
                this->actor.shape.rot.y = this->currentYaw;
            }
        }

        this->unk_B48 = this->linearVelocity;
        temp_fv0 = this->skelAnime.curFrame + 5.0f;
        temp_fv1 = this->skelAnime.animLength / 2.0f;

        // effectively an fmodf
        temp_fv0 -= temp_fv1 * (s32)(temp_fv0 / temp_fv1);
        this->linearVelocity *= Math_CosS(temp_fv0 * 1000.0f) * 0.4f;
    }
}

void func_8084AEEC(Player* this, PlayState* play) {
    this->stateFlags2 |= PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING;
    LinkAnimation_Update(play, &this->skelAnime);
    Player_StepLinearVelocityToZero(this);
    if (!Player_SetupShootingGalleryPlay(play, this)) {
        if (!Player_SetupSubAction(play, this, sStandStillSubActions, 0) || (func_8084AEEC == this->actionFunc)) {
            if (!CHECK_BTN_ALL(sPlayerControlInput->cur.button, BTN_B)) {
                Player_SetupStandingStillNoMorph(this, play);
            }
        }
    }
}

void func_8084AF9C(Player* this, PlayState* play) {
    f32 sp2C; // player speed?
    s16 sp2A;

    this->stateFlags2 |= PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING;
    func_8083F57C(this, play);
    if (Player_SetupSubAction(play, this, sRunSubActions, 1)) {
        return;
    }

    if (Player_IsZTargetingSetupStartUnfriendly(this)) {
        Player_SetupRun(this, play);
        return;
    }

    Player_GetTargetVelocityAndYaw(this, &sp2C, &sp2A, 0.018f, play);
    if (this->currentMask == PLAYER_MASK_BUNNY) {
        sp2C *= 1.5f;
    }

    if (!func_8083A4A4(this, &sp2C, &sp2A, REG(43) / 100.0f)) {
        Player_SetRunVelocityAndYaw(this, sp2C, sp2A);
        func_8083C8E8(this, play);
        if ((this->linearVelocity == 0.0f) && (sp2C == 0.0f)) {
            func_80839E3C(this, play);
        }
    }
}

void func_8084B0EC(Player* this, PlayState* play) {
    f32 sp2C;
    s16 sp2A;

    this->stateFlags2 |= PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING;

    func_8083F57C(this, play);
    if (Player_SetupSubAction(play, this, sTargetRunSubActions, 1)) {
        return;
    }

    if (!Player_IsZTargetingSetupStartUnfriendly(this)) {
        Player_SetupRun(this, play);
        return;
    }

    Player_GetTargetVelocityAndYaw(this, &sp2C, &sp2A, 0.0f, play);
    if (!func_8083A4A4(this, &sp2C, &sp2A, REG(43) / 100.0f)) {
        if (((Player_IsFriendlyZTargeting(this) != 0) && (sp2C != 0) &&
             (func_8083E514(this, &sp2C, &sp2A, play) <= 0)) ||
            (!Player_IsFriendlyZTargeting(this) && (func_8083E404(this, sp2C, sp2A) <= 0))) {
            Player_SetupStandingStillType(this, play);
        } else {
            Player_SetRunVelocityAndYaw(this, sp2C, sp2A);
            func_8083C8E8(this, play);
            if ((this->linearVelocity == 0.0f) && (sp2C == 0.0f)) {
                Player_SetupStandingStillType(this, play);
            }
        }
    }
}

void Player_UnfriendlyBackwalk(Player* this, PlayState* play) {
    s32 sp34 = LinkAnimation_Update(play, &this->skelAnime);
    f32 sp30;
    s16 sp2E;

    if (Player_SetupSubAction(play, this, sSidewalkSubActions, 1)) {
        return;
    }

    if (!Player_IsZTargetingSetupStartUnfriendly(this)) {
        Player_SetupRun(this, play);
        return;
    }

    Player_GetTargetVelocityAndYaw(this, &sp30, &sp2E, 0.0f, play);
    if ((this->skelAnime.morphWeight == 0.0f) && (this->skelAnime.curFrame > 5.0f)) {
        Player_StepLinearVelocityToZero(this);
        if ((this->skelAnime.curFrame > 10.0f) && (func_8083E404(this, sp30, sp2E) < 0)) {
            Player_SetupUnfriendlyBackwalk(this, sp2E, play);
        } else if (sp34) {
            Player_SetupEndUnfriendlyBackwalk(this, play);
        }
    }
}

void Player_EndUnfriendlyBackwalk(Player* this, PlayState* play) {
    s32 sp34 = LinkAnimation_Update(play, &this->skelAnime);
    f32 sp30;
    s16 sp2E;

    Player_StepLinearVelocityToZero(this);
    if (Player_SetupSubAction(play, this, sEndBackwalkSubActions, 1)) {
        return;
    }

    Player_GetTargetVelocityAndYaw(this, &sp30, &sp2E, 0.0f, play);
    if (this->linearVelocity == 0.0f) {
        this->currentYaw = this->actor.shape.rot.y;
        if (func_8083E404(this, sp30, sp2E) > 0) {
            Player_SetupRun(this, play);
        } else if ((sp30 != 0.0f) || sp34) {
            Player_SetupStandingStillType(this, play);
        }
    }
}

void func_8084B4A8(Player* this, PlayState* play) {
    if (this->skelAnime.animation == &gPlayerAnim_link_normal_backspace) {
        if (LinkAnimation_Update(play, &this->skelAnime)) {
            Player_EndAnimMovement(this);
            Player_ChangeAnimMorphToLastFrame(play, this, D_8085BE84[PLAYER_ANIMGROUP_31][this->modelAnimType]);
        }
    } else {
        Player_LoopAnimContinuously(play, this, D_8085BE84[PLAYER_ANIMGROUP_32][this->modelAnimType]);
    }

    if (DECR(this->genericTimer) == 0) {
        if (!Player_SetupItemCutsceneOrFirstPerson(this, play)) {
            Player_SetupReturnToStandStillSetAnim(this, D_8085BE84[PLAYER_ANIMGROUP_33][this->modelAnimType], play);
        }
        this->actor.flags &= ~ACTOR_FLAG_100;
        func_800E0238(Play_GetCamera(play, CAM_ID_MAIN));
    }
}

// Player_Action_Shielding
void func_8084B5C0(Player* this, PlayState* play) {
    Player_StepLinearVelocityToZero(this);

    if (this->transformation == PLAYER_FORM_GORON) {
        SkelAnime_Update(&this->skelAnimeGoron);

        if (!Player_IsBusy(this, play)) {
            if (!func_8083A274(this, play)) {
                this->stateFlags1 &= ~PLAYER_STATE1_400000;

                if (this->heldItemActionParam < PLAYER_AP_NONE) {
                    func_80123C58(this);
                }

                Player_SetupReturnToStandStillSetAnim(this, D_8085BE84[PLAYER_ANIMGROUP_21][this->modelAnimType], play);
                func_80830B38(this);
            } else {
                this->stateFlags1 |= PLAYER_STATE1_400000;
            }
        }

        return;
    }

    if (LinkAnimation_Update(play, &this->skelAnime)) {
        if (!Player_IsGoronOrDeku(this)) {
            Player_AnimationPlayLoop(play, this, D_8085BE84[PLAYER_ANIMGROUP_20][this->modelAnimType]);
        }

        this->genericTimer = 1;
        this->genericVar = 0;
    }

    if (!Player_IsGoronOrDeku(this)) {
        this->stateFlags1 |= PLAYER_STATE1_400000;
        Player_SetupCurrentUpperAction(this, play);
        this->stateFlags1 &= ~PLAYER_STATE1_400000;
        if (this->transformation == PLAYER_FORM_ZORA) {
            Player_SetupZoraBarrier(this, BTN_R | BTN_B);
        }
    }

    if (this->genericTimer != 0) {
        f32 yStick = sPlayerControlInput->rel.stick_y * 180;
        f32 xStick = sPlayerControlInput->rel.stick_x * -120;
        s16 temp_a0 = this->actor.shape.rot.y - Camera_GetInputDirYaw(play->cameraPtrs[play->activeCamId]);
        s16 var_a1;
        s16 temp_ft5;
        s16 var_a2;
        s16 var_a3;

        var_a1 = (yStick * Math_CosS(temp_a0)) + (Math_SinS(temp_a0) * xStick);
        temp_ft5 = (xStick * Math_CosS(temp_a0)) - (Math_SinS(temp_a0) * yStick);

        var_a1 = CLAMP_MAX(var_a1, 0xDAC);
        var_a2 = ABS_ALT(var_a1 - this->actor.focus.rot.x) * 0.25f;
        var_a2 = CLAMP_MIN(var_a2, 0x64);

        var_a3 = ABS_ALT(temp_ft5 - this->upperBodyRot.y) * 0.25f;
        var_a3 = CLAMP_MIN(var_a3, 0x32);
        Math_ScaledStepToS(&this->actor.focus.rot.x, var_a1, var_a2);

        this->upperBodyRot.x = this->actor.focus.rot.x;
        Math_ScaledStepToS(&this->upperBodyRot.y, temp_ft5, var_a3);

        if (this->genericVar != 0) {
            if (!func_808401F4(play, this)) {
                if (this->skelAnime.curFrame < 2.0f) {
                    Player_MeleeAttack(play, this, 1);
                }
            } else {
                this->genericTimer = 1;
                this->genericVar = 0;
            }
        } else if (!Player_IsBusy(this, play)) {
            if (func_8083A274(this, play) != 0) {
                func_8083FD80(this, play);
            } else {
                this->stateFlags1 &= ~PLAYER_STATE1_400000;
                Player_DeactivateMeleeWeapon(this);

                if (Player_IsGoronOrDeku(this)) {
                    Player_SetupReturnToStandStill(this, play);
                    LinkAnimation_Change(play, &this->skelAnime, this->skelAnime.animation, 1.0f,
                                         Animation_GetLastFrame(this->skelAnime.animation), 0.0f, 2, 0.0f);
                } else {
                    if (this->heldItemActionParam < PLAYER_AP_NONE) {
                        func_80123C58(this);
                    }

                    Player_SetupReturnToStandStillSetAnim(this, D_8085BE84[PLAYER_ANIMGROUP_21][this->modelAnimType],
                                                          play);
                }

                Player_PlaySfx(this, NA_SE_IT_SHIELD_REMOVE);
                return;
            }
        } else {
            return;
        }
    }

    this->stateFlags1 |= PLAYER_STATE1_400000;
    Player_SetModelsForHoldingShield(this);
    this->lookFlags |= 0xC1;
}

void func_8084BAA4(Player* this, PlayState* play) {
    Player_StepLinearVelocityToZero(this);
    if (this->genericVar == 0) {
        D_80862B04 = Player_SetupCurrentUpperAction(this, play);
        if ((Player_StandingDefend == this->upperActionFunc) ||
            (Player_IsActionInterrupted(play, this, &this->skelAnimeUpper, 4.0f) > 0)) {
            Player_SetAction(play, this, Player_UnfriendlyZTargetStandingStill, 1);
        }
    } else {
        s32 temp_v0;

        this->stateFlags1 |= PLAYER_STATE1_400000;
        temp_v0 = Player_IsActionInterrupted(play, this, &this->skelAnime, 4.0f);
        if ((temp_v0 != 0) && ((temp_v0 > 0) || LinkAnimation_Update(play, &this->skelAnime))) {
            LinkAnimationHeader* anim;
            f32 lastFrame;

            Player_SetAction(play, this, func_8084B5C0, 1);
            Player_SetModelsForHoldingShield(this);
            anim = D_8085BE84[PLAYER_ANIMGROUP_19][this->modelAnimType];
            lastFrame = Animation_GetLastFrame(anim);
            LinkAnimation_Change(play, &this->skelAnime, anim, 1.0f, lastFrame, lastFrame, ANIMMODE_ONCE, 0.0f);
        }
    }
}

void func_8084BBF0(Player* this, PlayState* play) {
    s32 temp_v0;

    Player_StepLinearVelocityToZero(this);
    temp_v0 = Player_IsActionInterrupted(play, this, &this->skelAnime, 16.0f);
    if (temp_v0 != 0) {
        if (LinkAnimation_Update(play, &this->skelAnime) || (temp_v0 > 0)) {
            Player_SetupStandingStillType(this, play);
        }
    }
}

void func_8084BC64(Player* this, PlayState* play) {
    this->stateFlags2 |=
        PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING | PLAYER_STATE2_ALWAYS_DISABLE_MOVE_ROTATION;
    func_808345A8(this);

    if (!(this->stateFlags1 & PLAYER_STATE1_IN_CUTSCENE) && (this->genericTimer == 0) && (this->unk_B75 != 0)) {
        s16 temp_v0 = this->unk_B76;
        s16 temp_v1 = this->actor.shape.rot.y - temp_v0;

        this->actor.shape.rot.y = temp_v0;
        this->currentYaw = temp_v0;
        this->linearVelocity = this->unk_B78;

        if (ABS_ALT(temp_v1) > 0x4000) {
            this->actor.shape.rot.y = temp_v0 + 0x8000;
        }

        if (this->actor.velocity.y < 0.0f) {
            this->actor.gravity = 0.0f;
            this->actor.velocity.y = 0.0f;
        }
    }

    if (LinkAnimation_Update(play, &this->skelAnime) && (this->actor.bgCheckFlags & 1)) {
        if (this->genericTimer != 0) {
            this->genericTimer--;
            if (this->genericTimer == 0) {
                Player_SetupStandingStillMorph(this, play);
            }
        } else if ((this->stateFlags1 & PLAYER_STATE1_IN_CUTSCENE) ||
                   (!(this->cylinder.base.acFlags & 2) && (this->unk_B75 == 0))) {
            if (this->stateFlags1 & PLAYER_STATE1_IN_CUTSCENE) {
                this->genericTimer++;
            } else {
                Player_SetAction(play, this, func_8084BE40, 0);
                this->stateFlags1 |= PLAYER_STATE1_4000000;
            }

            Player_AnimationPlayOnce(play, this,
                                     (this->currentYaw != this->actor.shape.rot.y)
                                         ? &gPlayerAnim_link_normal_front_downB
                                         : &gPlayerAnim_link_normal_back_downB);
            Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_FREEZE);
        }
    }

    if (this->actor.bgCheckFlags & 2) {
        Player_AnimSfx_PlayFloor(this, NA_SE_PL_BOUND);
    }
}

void func_8084BE40(Player* this, PlayState* play) {
    this->stateFlags2 |=
        (PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING | PLAYER_STATE2_ALWAYS_DISABLE_MOVE_ROTATION);
    func_808345A8(this);
    Player_StepLinearVelocityToZero(this);
    if (LinkAnimation_Update(play, &this->skelAnime) && (this->linearVelocity == 0.0f)) {
        if (this->stateFlags1 & PLAYER_STATE1_IN_CUTSCENE) {
            this->genericTimer += 1;
        } else {
            Player_SetAction(play, this, func_8084BF28, 0);
            this->stateFlags1 |= PLAYER_STATE1_4000000;
        }

        Player_PlayAnimOnceSlowed(play, this,
                                  (this->currentYaw != this->actor.shape.rot.y)
                                      ? &gPlayerAnim_link_normal_front_down_wake
                                      : &gPlayerAnim_link_normal_back_down_wake);
        this->currentYaw = this->actor.shape.rot.y;
    }
}

AnimSfxEntry D_8085D604[] = {
    ANIMSFX(ANIMSFX_TYPE_8, 20, 0, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_8, 30, 0, STOP),
};

void func_8084BF28(Player* this, PlayState* play) {
    this->stateFlags2 |= PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING;

    func_808345A8(this);
    if (this->stateFlags1 & PLAYER_STATE1_IN_CUTSCENE) {
        LinkAnimation_Update(play, &this->skelAnime);
    } else {
        s32 temp_v0 = Player_IsActionInterrupted(play, this, &this->skelAnime, 16.0f);

        if (temp_v0 != 0) {
            if (LinkAnimation_Update(play, &this->skelAnime) || (temp_v0 > 0)) {
                Player_SetupStandingStillType(this, play);
            }
        }
    }

    Player_PlayAnimSfx(this, D_8085D604);
}

AnimSfxEntry D_8085D60C[] = {
    ANIMSFX(ANIMSFX_TYPE_FLOOR, 60, NA_SE_PL_BOUND, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_8, 140, 0, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_8, 164, 0, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_8, 170, 0, STOP),
};

void Player_Die(Player* this, PlayState* play) {
    if ((this->transformation != PLAYER_FORM_GORON) && (this->actor.depthInWater <= 0.0f)) {
        if ((play->roomCtx.curRoom.unk2 == 3) || (sPlayerCurrentFloorType == BG_FLOOR_TYPE_9) ||
            ((Player_GetHurtFloorType(sPlayerCurrentFloorType) >= 0) &&
             !SurfaceType_IsWallDamage(&play->colCtx, this->actor.floorPoly, this->actor.floorBgId))) {
            Player_StartBurning(play, this);
        }
    }

    Player_StepLinearVelocityToZero(this);

    if (LinkAnimation_Update(play, &this->skelAnime)) {
        if (this == GET_PLAYER(play)) {
            func_80840770(play, this);
        }
    } else if (this->skelAnime.animation == &gPlayerAnim_link_derth_rebirth) {
        Player_PlayAnimSfx(this, D_8085D60C);
    } else if ((this->skelAnime.animation == &gPlayerAnim_link_normal_electric_shock_end) &&
               LinkAnimation_OnFrame(&this->skelAnime, 88.0f)) {
        Player_AnimSfx_PlayFloor(this, NA_SE_PL_BOUND);
    }
}

s32 func_8084C124(PlayState* play, Player* this) {
    if (Player_SetupSpawnSplash(play, this, 3.0f, 500)) {
        Player_PlaySfx(this, NA_SE_EV_DIVE_INTO_WATER);
        return true;
    }
    return false;
}

void Player_UpdateMidair(Player* this, PlayState* play) {
    f32 sp44;
    s16 sp42;
    Actor* heldActor;

    if (Player_IsUnfriendlyZTargeting(this)) {
        this->actor.gravity = -1.2f;
    }

    if (!(this->actor.bgCheckFlags & 1)) {
        Player_GetTargetVelocityAndYaw(this, &sp44, &sp42, 0.0f, play);
        if (this->stateFlags1 & PLAYER_STATE1_HOLDING_ACTOR) {
            heldActor = this->heldActor;
            if (!Player_InterruptHoldingActor(play, this, heldActor) && (heldActor->id == ACTOR_EN_NIW) &&
                CHECK_BTN_ANY(sPlayerControlInput->press.button, BTN_CRIGHT | BTN_CLEFT | BTN_CDOWN | BTN_B | BTN_A)) {
                Player_ThrowActor(play, this, this->linearVelocity + 2.0f, this->actor.velocity.y + 2.0f);
            }
        }

        LinkAnimation_Update(play, &this->skelAnime);
        if ((this->skelAnime.animation == &gPlayerAnim_link_normal_newroll_jump_20f) &&
            LinkAnimation_OnFrame(&this->skelAnime, 4.0f)) {
            Player_PlaySfx(this, NA_SE_PL_ROLL);
        }

        if (this->transformation == PLAYER_FORM_DEKU) {
            s16 prevYaw = this->currentYaw;

            func_808378FC(play, this);
            func_8083CBC4(this, sp44 * 0.5f, sp42, 2.0f, 0.2f, 0.1f, 0x190);

            if (this->skelAnime.animation == &gPlayerAnim_pn_attack) {
                this->stateFlags2 |= (PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING |
                                      PLAYER_STATE2_ALWAYS_DISABLE_MOVE_ROTATION);

                this->unk_B10[0] += -800.0f;
                this->actor.shape.rot.y += BINANG_ADD((s16)this->unk_B10[0], BINANG_SUB(this->currentYaw, prevYaw));
                Math_StepToF(&this->unk_B10[1], 0.0f, this->unk_B10[0]);
            }
        } else {
            func_8083CBC4(this, sp44, sp42, 1.0f, 0.05f, 0.1f, 0xC8);
        }

        Player_SetupCurrentUpperAction(this, play);
        if ((((this->stateFlags2 & PLAYER_STATE2_80000) && ((this->genericVar == 2) || (this->genericVar >= 4))) ||
             !func_80839770(this, play)) &&
            (this->actor.velocity.y < 0.0f)) {
            if (this->genericTimer >= 0) {
                if ((this->actor.bgCheckFlags & 8) || (this->genericTimer == 0) || (this->fallDistance > 0)) {
                    if ((sPlayerYDistToFloor > 800.0f) || (this->stateFlags3 & PLAYER_STATE3_10000)) {
                        func_80840980(this, NA_SE_VO_LI_FALL_S);
                        this->stateFlags3 &= ~PLAYER_STATE3_10000;
                    }
                    LinkAnimation_Change(play, &this->skelAnime, &gPlayerAnim_link_normal_landing, 1.0f, 0.0f, 0.0f,
                                         ANIMMODE_ONCE, 8.0f);
                    this->genericTimer = -1;
                }
            } else {
                if ((this->genericTimer == -1) && (this->fallDistance > 120) && (sPlayerYDistToFloor > 280.0f)) {
                    this->genericTimer = -2;
                    func_80840980(this, NA_SE_VO_LI_FALL_L);
                }

                if ((this->actor.bgCheckFlags & 0x200) &&
                    !(this->stateFlags1 & (PLAYER_STATE1_HOLDING_ACTOR | PLAYER_STATE1_SWIMMING)) &&
                    (this->linearVelocity > 0.0f)) {
                    if ((this->transformation != PLAYER_FORM_GORON) &&
                        ((this->transformation != PLAYER_FORM_DEKU) || (this->remainingHopsCounter != 0))) {
                        if ((this->wallHeight >= 150.0f) &&
                            (this->relativeAnalogStickInputs[this->inputFrameCounter] == 0)) {
                            if (func_8083D860(this, play)) {
                                func_8084C124(play, this);
                            }
                        } else if ((this->unk_B5C >= 2) &&
                                   ((this->wallHeight < (150.0f * this->ageProperties->unk_08)) &&
                                    (((this->actor.world.pos.y - this->actor.floorHeight) + this->wallHeight)) >
                                        (70.0f * this->ageProperties->unk_08))) {
                            AnimationContext_DisableQueue(play);
                            if (this->stateFlags3 & PLAYER_STATE3_10000) {
                                Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_HOOKSHOT_HANG);
                            } else {
                                Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_HANG);
                            }

                            this->actor.world.pos.y += this->wallHeight;
                            func_80837CEC(play, this, this->actor.wallPoly, this->wallDistance,
                                          GET_PLAYER_ANIM(PLAYER_ANIMGROUP_38, this->modelAnimType));
                            this->currentYaw += 0x8000;
                            this->actor.shape.rot.y = this->currentYaw;
                            this->stateFlags1 |= PLAYER_STATE1_2000;

                            func_8084C124(play, this);
                        }
                    }
                }
            }
        }
    } else {
        func_80837134(play, this);
        Player_SetupCurrentUpperAction(this, play);
    }

    Player_SetupItemCutsceneOrFirstPerson(this, play);
}

// sPlayerRollingAnimSfx
AnimSfxEntry D_8085D61C[] = {
    ANIMSFX(ANIMSFX_TYPE_VOICE, 1, NA_SE_VO_LI_SWORD_N, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_FLOOR_BY_AGE, 6, NA_SE_PL_WALK_GROUND, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 6, NA_SE_PL_ROLL, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_FLOOR_LAND, 18, 0, STOP),
};

// Player_Action_Rolling // Handles bonking too?
void Player_Rolling(Player* this, PlayState* play) {
    s32 animFinished;

    this->stateFlags2 |= PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING;
    this->stateFlags3 |= PLAYER_STATE3_8000000;

    animFinished = LinkAnimation_Update(play, &this->skelAnime);
    if (LinkAnimation_OnFrame(&this->skelAnime, 8.0f)) {
        Player_SetupInvincibilityNoDamageFlash(this, -10);
    }

    if (this->skelAnime.curFrame >= 8.0f) {
        if (this->skelAnime.curFrame < 18.0f) {
            Player_SetCylinderForAttack(this, DMG_NORMAL_ROLL, 1, 12);
        } else {
            Player_ResetCylinder(this);
        }
    }

    if (Player_IsBusy(this, play)) {
        return;
    }

    if (this->genericTimer != 0) {
        s32 actionInterruptResult;

        Math_StepToF(&this->linearVelocity, 0.0f, 2.0f);

        actionInterruptResult = Player_IsActionInterrupted(play, this, &this->skelAnime, 5.0f);
        if (actionInterruptResult != 0) {
            if ((actionInterruptResult > 0) || animFinished) {
                Player_SetupReturnToStandStill(this, play);
            }
        }
    } else if (!Player_TryBonkning(play, this, &this->linearVelocity, 6.0f)) {
        if ((this->skelAnime.curFrame < 15.0f) || !func_80848570(this, play)) {
            f32 targetVelocity;
            s16 targetYaw;

            if (this->skelAnime.curFrame >= 20.0f) {
                Player_SetupReturnToStandStill(this, play);
                return;
            }

            Player_GetTargetVelocityAndYaw(this, &targetVelocity, &targetYaw, 0.018f, play);

            targetVelocity *= 1.5f;
            if ((targetVelocity < 3.0f) || (this->relativeAnalogStickInputs[this->inputFrameCounter] != 0)) {
                targetVelocity = 3.0f;
            }

            Player_SetRunVelocityAndYaw(this, targetVelocity, this->actor.shape.rot.y);

            if (Player_SetupSpawnDustAtFeet(play, this)) {
                func_800B8F98(&this->actor, (this->floorSfxOffset == NA_SE_PL_WALK_SNOW - SFX_FLAG)
                                                ? NA_SE_PL_ROLL_SNOW_DUST - SFX_FLAG
                                                : NA_SE_PL_ROLL_DUST - SFX_FLAG);
            }

            Player_PlayAnimSfx(this, D_8085D61C);
        }
    }
}

void func_8084C94C(Player* this, PlayState* play) {
    this->stateFlags2 |= PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING;

    if (LinkAnimation_Update(play, &this->skelAnime)) {
        Player_AnimationPlayLoop(play, this, &gPlayerAnim_link_normal_run_jump_water_fall_wait);
    }

    Math_StepToF(&this->linearVelocity, 0.0f, 0.05f);
    if (this->actor.bgCheckFlags & 1) {
        if (this->fallDistance >= 400) {
            this->actor.colChkInfo.damage = 0x10;
            func_80833B18(play, this, 1, 4.0f, 5.0f, this->actor.shape.rot.y, 20);
        } else {
            func_80836B3C(play, this, 4.0f);
        }
    }
}

void func_8084CA24(Player* this, PlayState* play) {
    this->stateFlags2 |= PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING;

    if (LinkAnimation_Update(play, &this->skelAnime)) {
        func_8082DB60(play, this, &gPlayerAnim_pz_fishswim);
    }

    Math_SmoothStepToS(&this->unk_B86[1], 0, 6, 0x7D0, 0x190);

    if (!Player_TryBonkning(play, this, &this->linearVelocity, 0.0f)) {
        if (this->actor.bgCheckFlags & 1) {
            if (this->unk_AAA > 0x36B0) {
                this->actor.colChkInfo.damage = 0x10;
                func_80833B18(play, this, 1, 4.0f, 5.0f, this->actor.shape.rot.y, 20);
            } else {
                func_80836B3C(play, this, 4.0f);
            }
        } else {
            this->actor.gravity = -1.0f;
            this->unk_AAA = Math_FAtan2F(this->actor.speedXZ, -this->actor.velocity.y);
            Player_SetupZoraBarrier(this, BTN_R);
        }
    }
}

void Player_JumpSlash(Player* this, PlayState* play) {
    AttackAnimInfo* attackInfoEntry = &sMeleeAttackAnimInfo[this->meleeWeaponAnimation];
    f32 targetVelocity;
    s16 targetYaw;

    this->stateFlags2 |= PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING;

    if (this->transformation == PLAYER_FORM_ZORA) {
        this->actor.gravity = -0.8f;
    } else {
        this->actor.gravity = -1.2f;
    }

    LinkAnimation_Update(play, &this->skelAnime);

    if (!func_808401F4(play, this)) {
        Player_SetupMeleeAttack(play, this, 6.0f, attackInfoEntry->unk_C, attackInfoEntry->unk_D);
        if (!(this->actor.bgCheckFlags & 1)) {
            Player_GetTargetVelocityAndYaw(this, &targetVelocity, &targetYaw, 0.0f, play);
            func_8083CBC4(this, targetVelocity, this->currentYaw, 1.0f, 0.05f, 0.1f, 200);
        } else if (Player_SetupFallLanding(play, this) >= 0) { // Player didn't die because of this fall
            this->meleeWeaponAnimation += 3;
            Player_StartMeleeWeaponAttack(play, this, this->meleeWeaponAnimation);
            this->slashCounter = 3;
            this->meleeWeaponState = 0;
            Player_AnimSfx_PlayFloorLand(this);
        }
    }
}

void Player_ChargeSpinAttack(Player* this, PlayState* play) {
    f32 targetVelocity;
    s16 targetYaw;
    s32 moveDir;

    this->stateFlags1 |= PLAYER_STATE1_CHARGING_SPIN_ATTACK;

    if (LinkAnimation_Update(play, &this->skelAnime)) {
        Player_EndAnimMovement(this);
        Player_SetZTargetFriendlyYaw(this);
        this->stateFlags1 &= ~PLAYER_STATE1_Z_TARGETING_FRIENDLY;
        Player_AnimationPlayLoop(play, this, sSpinAttackChargeAnims[Player_IsHoldingTwoHandedWeapon(this)]);
        this->genericTimer = -1;
    }

    Player_StepLinearVelocityToZero(this);

    if (!Player_IsBusy(this, play) && (this->genericTimer != 0)) {
        Player_UpdateSpinAttackTimer(this);
        if (this->genericTimer < 0) {
            if (this->unk_B08[0] >= 0.1f) {
                this->slashCounter = 0;
                this->genericTimer = 1;
            } else if (!CHECK_BTN_ALL(sPlayerControlInput->cur.button, BTN_B)) {
                Player_CancelSpinAttackCharge(this, play);
            }
        } else if (!Player_SetupReleaseSpinAttack(this, play)) {
            Player_GetTargetVelocityAndYaw(this, &targetVelocity, &targetYaw, 0.0f, play);

            moveDir = Player_GetSpinAttackMoveDirection(this, &targetVelocity, &targetYaw, play);
            if (moveDir > 0) {
                Player_SetupWalkChargingSpinAttack(this, play);
            } else if (moveDir < 0) {
                Player_SetupSidewalkChargingSpinAttack(this, play);
            }
        }
    }
}

void Player_WalkChargingSpinAttack(Player* this, PlayState* play) {
    s32 absCurYawDiff;
    s32 moveDir;
    f32 absLinearVelocity;
    f32 blendWeight;
    f32 targetVelocity;
    s16 targetYaw;
    s16 curYawDiff;
    s32 pad[2];
    s32 absTargetYawDiff;

    curYawDiff = this->currentYaw - this->actor.shape.rot.y;
    absCurYawDiff = ABS_ALT(curYawDiff);

    absLinearVelocity = fabsf(this->linearVelocity);

    this->stateFlags1 |= PLAYER_STATE1_CHARGING_SPIN_ATTACK;

    blendWeight = absLinearVelocity * 1.5f;
    blendWeight = CLAMP_MIN(blendWeight, 1.5f);

    blendWeight = ((absCurYawDiff < 0x4000) ? -1.0f : 1.0f) * blendWeight;

    Player_SetupWalkSfx(this, blendWeight);

    blendWeight = CLAMP(absLinearVelocity * 0.5f, 0.5f, 1.0f);

    LinkAnimation_BlendToJoint(play, &this->skelAnime, sSpinAttackChargeAnims[Player_IsHoldingTwoHandedWeapon(this)],
                               0.0f, sSpinAttackChargeWalkAnims[Player_IsHoldingTwoHandedWeapon(this)],
                               this->walkFrame * (21.0f / 29.0f), blendWeight, this->blendTableBuffer);
    if (!Player_IsBusy(this, play) && !Player_SetupReleaseSpinAttack(this, play)) {
        Player_UpdateSpinAttackTimer(this);
        Player_GetTargetVelocityAndYaw(this, &targetVelocity, &targetYaw, 0.0f, play);

        moveDir = Player_GetSpinAttackMoveDirection(this, &targetVelocity, &targetYaw, play);

        if (moveDir < 0) {
            Player_SetupSidewalkChargingSpinAttack(this, play);
            return;
        }

        if (moveDir == 0) {
            targetVelocity = 0.0f;
            targetYaw = this->currentYaw;
        }

        absTargetYawDiff = ABS_ALT(BINANG_SUB(targetYaw, this->currentYaw));

        if (absTargetYawDiff > 0x4000) {
            if (Math_StepToF(&this->linearVelocity, 0.0f, 1.0f)) {
                this->currentYaw = targetYaw;
            }
        } else {
            Math_AsymStepToF(&this->linearVelocity, targetVelocity * 0.2f, 1.0f, 0.5f);
            Math_ScaledStepToS(&this->currentYaw, targetYaw, absTargetYawDiff * 0.1f);
            if ((targetVelocity == 0.0f) && (this->linearVelocity == 0.0f)) {
                Player_SetupChargeSpinAttack(this, play);
            }
        }
    }
}

void Player_SidewalkChargingSpinAttack(Player* this, PlayState* play) {
    f32 absLinearVelocity;
    f32 blendWeight;

    absLinearVelocity = fabsf(this->linearVelocity);

    this->stateFlags1 |= PLAYER_STATE1_CHARGING_SPIN_ATTACK;

    if (absLinearVelocity == 0.0f) {
        absLinearVelocity = ABS_ALT(this->unk_B4C) * 0.0015f;
        if (absLinearVelocity < 400.0f) {
            absLinearVelocity = 0.0f;
        }

        Player_SetupWalkSfx(this, ((this->unk_B4C >= 0) ? 1 : -1) * absLinearVelocity);
    } else {
        blendWeight = absLinearVelocity * 1.5f;
        blendWeight = CLAMP_MIN(blendWeight, 1.5f);
        Player_SetupWalkSfx(this, blendWeight);
    }

    blendWeight = CLAMP(absLinearVelocity * 0.5f, 0.5f, 1.0f);

    LinkAnimation_BlendToJoint(play, &this->skelAnime, sSpinAttackChargeAnims[Player_IsHoldingTwoHandedWeapon(this)],
                               0.0f, sSpinAttackChargeSidewalkAnims[Player_IsHoldingTwoHandedWeapon(this)],
                               this->walkFrame * (21.0f / 29.0f), blendWeight, this->blendTableBuffer);
    if (!Player_IsBusy(this, play) && !Player_SetupReleaseSpinAttack(this, play)) {
        f32 targetVelocity;
        s16 targetYaw;
        s32 moveDir;
        s32 pad;
        s32 absTargetYawDiff;

        Player_UpdateSpinAttackTimer(this);
        Player_GetTargetVelocityAndYaw(this, &targetVelocity, &targetYaw, 0.0f, play);
        moveDir = Player_GetSpinAttackMoveDirection(this, &targetVelocity, &targetYaw, play);

        if (moveDir > 0) {
            Player_SetupWalkChargingSpinAttack(this, play);
            return;
        }

        if (moveDir == 0) {
            targetVelocity = 0.0f;
            targetYaw = this->currentYaw;
        }

        absTargetYawDiff = ABS_ALT(BINANG_SUB(targetYaw, this->currentYaw));

        if (absTargetYawDiff > 0x4000) {
            if (Math_StepToF(&this->linearVelocity, 0.0f, 1.0f)) {
                this->currentYaw = targetYaw;
            }
        } else {
            Math_AsymStepToF(&this->linearVelocity, targetVelocity * 0.2f, 1.0f, 0.5f);
            Math_ScaledStepToS(&this->currentYaw, targetYaw, absTargetYawDiff * 0.1f);
            if ((targetVelocity == 0.0f) && (this->linearVelocity == 0.0f) && (absLinearVelocity == 0.0f)) {
                Player_SetupChargeSpinAttack(this, play);
            }
        }
    }
}

void Player_JumpUpToLedge(Player* this, PlayState* play) {
    s32 animFinished;
    f32 frame;
    s32 temp_v0;

    this->stateFlags2 |= PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING;
    animFinished = LinkAnimation_Update(play, &this->skelAnime);

    if (this->skelAnime.animation == &gPlayerAnim_link_normal_250jump_start) {
        this->linearVelocity = 1.0f;
        if (LinkAnimation_OnFrame(&this->skelAnime, 8.0f)) {
            f32 jumpVelocityY = this->wallHeight;

            jumpVelocityY = CLAMP_MAX(jumpVelocityY, this->ageProperties->unk_0C);
            if (this->stateFlags1 & PLAYER_STATE1_SWIMMING) {
                jumpVelocityY *= 0.085f;
            } else {
                jumpVelocityY *= 0.072f;
            }

            if (this->transformation == PLAYER_FORM_HUMAN) {
                jumpVelocityY += 1.0f;
            }

            Player_SetupJumpAction(play, this, NULL, jumpVelocityY, NA_SE_VO_LI_AUTO_JUMP);
            this->genericTimer = -1;
        }
    } else {
        temp_v0 = Player_IsActionInterrupted(play, this, &this->skelAnime, 4.0f);
        if (temp_v0 == 0) {
            this->stateFlags1 &= ~(PLAYER_STATE1_4 | PLAYER_STATE1_4000 | PLAYER_STATE1_JUMPING);
            return;
        }
        if (animFinished || (temp_v0 > 0)) {
            Player_SetupStandingStillNoMorph(this, play);
            this->stateFlags1 &= ~(PLAYER_STATE1_4 | PLAYER_STATE1_4000 | PLAYER_STATE1_JUMPING);
            this->unk_ABC = 0.0f;
            return;
        }

        frame = 0.0f;
        if (this->skelAnime.animation == &gPlayerAnim_link_swimer_swim_15step_up) {
            if (LinkAnimation_OnFrame(&this->skelAnime, 30.0f)) {
                Player_StartJumpOutOfWater(play, this, 10.0f);
            }
            frame = 50.0f;
        } else if (this->skelAnime.animation == &gPlayerAnim_link_normal_150step_up) {
            frame = 30.0f;
        } else if (this->skelAnime.animation == &gPlayerAnim_link_normal_100step_up) {
            frame = 16.0f;
        }

        if (LinkAnimation_OnFrame(&this->skelAnime, frame)) {
            Player_AnimSfx_PlayFloorLand(this);
            Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_CLIMB_END);
        }

        if ((this->skelAnime.animation == &gPlayerAnim_link_normal_100step_up) || (this->skelAnime.curFrame > 5.0f)) {
            if (this->genericTimer == 0) {
                Player_AnimSfx_PlayFloorJump(this);
                this->genericTimer = 1;
            }
            Math_SmoothStepToF(&this->unk_ABC, 0.0f, 0.1f, 400.0f, 150.0f);
        }
    }
}

void Player_RunMiniCutsceneFunc(Player* this, PlayState* play) {
    s32 temp_v0;

    this->stateFlags2 |=
        (PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING | PLAYER_STATE2_ALWAYS_DISABLE_MOVE_ROTATION);
    if (this->miniCsFunc == func_80837BF8) {
        this->stateFlags2 |= PLAYER_STATE2_1;
    }

    LinkAnimation_Update(play, &this->skelAnime);
    func_8083249C(this);

    temp_v0 = Player_SetupCurrentUpperAction(this, play);
    if (((this->stateFlags1 & PLAYER_STATE1_HOLDING_ACTOR) && (this->heldActor != NULL) &&
         (this->getItemId == GI_NONE)) ||
        (temp_v0 == 0)) {
        this->miniCsFunc(play, this);
    }
}

void Player_MiniCsMovement(Player* this, PlayState* play) {
    if (Player_SetupItemCutsceneOrFirstPerson(this, play) == 0) {
        if ((this->stateFlags3 & PLAYER_STATE3_10) && !(this->actor.bgCheckFlags & 1)) {
            func_80833AA0(this, play);
            this->stateFlags1 |= PLAYER_STATE1_IN_CUTSCENE;
        } else if (this->genericTimer == 0) {
            LinkAnimation_Update(play, &this->skelAnime);
            if (DECR(this->doorTimer) == 0) {
                this->linearVelocity = 0.1f;
                this->genericTimer = 1;
            }
        } else if (this->genericVar == 0) {
            f32 sp6C;
            s32 var_t0; // sp68

            sp6C = 5.0f * sWaterSpeedScale;
            var_t0 = func_808411D4(play, this, &sp6C, -1);
            if (this->unk_397 == 4) {
                if (MREG(64) < 0) {
                    if (play->roomCtx.unk31 != 1) {
                        MREG(68) += MREG(64);
                        if (MREG(68) < 0) {
                            MREG(68) = 0;
                        }

                        this->actor.world.pos.y += (this->doorDirection != 0) ? 3.0f : -3.0f;
                        this->actor.prevPos.y = this->actor.world.pos.y;
                    }
                } else if (MREG(64) == 0) {
                    CollisionPoly* sp64;
                    s32 sp60;

                    if (func_80835DF8(play, this, &sp64, &sp60)) {
                        this->actor.floorPoly = sp64;
                        this->actor.floorBgId = sp60;
                    }
                }
            }

            if (var_t0 < 0x1E) {
                this->genericVar = 1;
                this->stateFlags1 |= PLAYER_STATE1_IN_CUTSCENE;
                this->unk_3A0.x = this->unk_3AC.x;
                this->unk_3A0.z = this->unk_3AC.z;
            }
        } else {
            f32 sp5C;
            s32 sp58;
            s32 temp_v0_8;

            sp58 = 0x14;
            sp5C = 5.0f;
            if (this->stateFlags1 & PLAYER_STATE1_1) {
                sp5C = gSaveContext.entranceSpeed;
                if (sPlayerConveyorSpeedIndex != BG_CONVEYOR_SPEED_DISABLED) {
                    this->unk_3A0.x = (Math_SinS(D_80862B16) * 400.0f) + this->actor.world.pos.x;
                    this->unk_3A0.z = (Math_CosS(D_80862B16) * 400.0f) + this->actor.world.pos.z;
                }
            } else {
                if (this->genericTimer < 0) {
                    this->genericTimer++;
                    sp5C = gSaveContext.entranceSpeed;
                    sp58 = -1;
                } else if (this->unk_397 == 4) {
                    if (MREG(64) == 0) {
                        MREG(64) = 0x10;
                        MREG(68) = 0;

                        MREG(65) = MREG(66) = MREG(67) = MREG(68);
                    } else if (MREG(64) >= 0) {
                        MREG(68) += MREG(64);
                        if (MREG(68) >= 0x100) {
                            TransitionActorEntry* temp_v1_4; // sp50
                            s32 roomNum;

                            temp_v1_4 = &play->doorCtx.transitionActorList[this->doorNext];
                            roomNum = temp_v1_4->sides[0].room;
                            MREG(68) = 0xFF;

                            if ((roomNum != play->roomCtx.curRoom.num) && (play->roomCtx.curRoom.num >= 0)) {
                                play->roomCtx.prevRoom = play->roomCtx.curRoom;

                                play->roomCtx.curRoom.num = -1;
                                play->roomCtx.curRoom.segment = NULL;
                                func_8012EBF8(play, &play->roomCtx);
                            } else {
                                static Vec3f D_8085D62C = { 0.0f, 0.0f, 0.0f };
                                static Vec3f D_8085D638 = { 0.0f, 0.0f, 0.0f };
                                static Vec3f D_8085D644 = { 0.0f, 0.0f, 0.0f };

                                MREG(64) = -0x10;
                                if (play->roomCtx.curRoom.num < 0) {
                                    Room_StartRoomTransition(play, &play->roomCtx, temp_v1_4->sides[0].room);
                                    play->roomCtx.prevRoom.num = -1;
                                    play->roomCtx.prevRoom.segment = NULL;
                                }

                                this->actor.world.pos.x = temp_v1_4->pos.x;
                                this->actor.world.pos.y = temp_v1_4->pos.y;
                                this->actor.world.pos.z = temp_v1_4->pos.z;

                                this->actor.shape.rot.y = ((((temp_v1_4->rotY >> 7) & 0x1FF) / 180.0f) * 0x8000);

                                D_8085D62C.x = (this->doorDirection != 0) ? -120.0f : 120.0f;
                                D_8085D62C.y = (this->doorDirection != 0) ? -75.0f : 75.0f;
                                D_8085D62C.z = -240.0f;
                                if (this->doorDirection != 0) {
                                    Camera_ChangeDoorCam(play->cameraPtrs[0], &this->actor, -2, 0.0f,
                                                         temp_v1_4->pos.x + 0x32, temp_v1_4->pos.y + 0x5F,
                                                         temp_v1_4->pos.z - 0x32);
                                } else {
                                    Camera_ChangeDoorCam(play->cameraPtrs[0], &this->actor, -2, 0.0f,
                                                         temp_v1_4->pos.x - 0x32, temp_v1_4->pos.y + 5,
                                                         temp_v1_4->pos.z - 0x32);
                                }

                                Player_TranslateAndRotateY(this, &this->actor.world.pos, &D_8085D62C,
                                                           &this->actor.world.pos);

                                D_8085D638.x = (this->doorDirection != 0) ? 130.0f : -130.0f;
                                D_8085D638.z = 160.0f;
                                Player_TranslateAndRotateY(this, &this->actor.world.pos, &D_8085D638, &this->unk_3A0);
                                D_8085D644.z = 160.0f;
                                Player_TranslateAndRotateY(this, &this->unk_3A0, &D_8085D644, &this->unk_3AC);

                                this->actor.shape.rot.y += (this->doorDirection != 0) ? 0x4000 : -0x4000;
                                this->genericVar = 0;

                                this->actor.world.rot.y = this->currentYaw = this->actor.shape.rot.y;
                            }
                        }

                        this->actor.world.pos.y += (this->doorDirection != 0) ? 3.0f : -3.0f;
                        this->actor.prevPos.y = this->actor.world.pos.y;
                    }
                }
            }

            temp_v0_8 = func_808411D4(play, this, &sp5C, sp58);
            if ((this->genericTimer == 0) ||
                ((temp_v0_8 == 0) && (this->linearVelocity == 0.0f) && (Play_GetCamera(play, 0)->stateFlags & 0x10))) {
                if (this->unk_397 == 4) {
                    Map_InitRoomData(play, (s16)play->roomCtx.curRoom.num);
                    Minimap_SavePlayerRoomInitInfo(play);
                }

                MREG(64) = 0;
                func_800E0238(Play_GetCamera(play, 0));
                func_80838760(this);
                if (!(this->stateFlags3 & PLAYER_STATE3_20000)) {
                    func_801226E0(play, ((void)0, gSaveContext.respawn[RESPAWN_MODE_DOWN].data));
                }

                if (play->shootingGalleryStatus != 0) {
                    play->func_18780(this, play);
                    Player_SetAction(play, this, func_80854430, 0);
                    if (play->sceneId == 0x45) {
                        play->shootingGalleryStatus = 0;
                    }
                } else if (!Player_SetupSpeakOrCheck(this, play)) {
                    Player_EndMiniCsMovement(this, play);
                }
            }
        }
    }

    if (this->stateFlags1 & PLAYER_STATE1_HOLDING_ACTOR) {
        Player_SetupCurrentUpperAction(this, play);
    }
}

// door stuff
void Player_Door_Open(Player* this, PlayState* play) {
    EnDoor* doorActor = (EnDoor*)this->doorActor;
    s32 sp38;
    s32 animFinished;
    CollisionPoly* poly;
    s32 bgId;

    sp38 = (doorActor != NULL) && (doorActor->doorType == ENDOOR_TYPE_7);
    this->stateFlags2 |= PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING;

    if (DECR(this->genericVar) == 0) {
        func_80835DF8(play, this, &poly, &bgId);
    }

    animFinished = LinkAnimation_Update(play, &this->skelAnime);
    Player_SetupCurrentUpperAction(this, play);

    if (animFinished) {
        if (this->genericTimer == 0) {
            if (DECR(this->doorTimer) == 0) {
                this->genericTimer = 1;
                this->skelAnime.endFrame = this->skelAnime.animLength - 1.0f;
            }
        } else {
            func_80838760(this);
            Player_SetupStandingStillNoMorph(this, play);

            if ((this->actor.category == ACTORCAT_PLAYER) && !sp38) {
                if (play->roomCtx.prevRoom.num >= 0) {
                    func_8012EBF8(play, &play->roomCtx);
                }

                func_800E0238(Play_GetCamera(play, CAM_ID_MAIN));
                Play_SetupRespawnPoint(&play->state, RESPAWN_MODE_DOWN, PLAYER_PARAMS(0xFF, PLAYER_INITMODE_B));
            }
        }
    } else if (!(this->stateFlags1 & PLAYER_STATE1_IN_CUTSCENE) && LinkAnimation_OnFrame(&this->skelAnime, 15.0f)) {
        func_80838760(this);
        play->func_18780(this, play);
    } else if (sp38 && LinkAnimation_OnFrame(&this->skelAnime, 15.0f)) {
        s16 doorRot =
            (this->doorDirection < 0) ? doorActor->door.dyna.actor.world.rot.x : doorActor->door.dyna.actor.world.rot.z;

        if (doorRot != 0) {
            func_808354A4(play, doorRot - 1, false);
        }
    }
}

// grab/hold an actor (?)
void Player_LiftActor(Player* this, PlayState* play) {
    Player_StepLinearVelocityToZero(this);

    if (LinkAnimation_Update(play, &this->skelAnime)) {
        Player_SetupStandingStillType(this, play);
        Player_SetupHoldActorUpperAction(this, play);
    } else if (LinkAnimation_OnFrame(&this->skelAnime, 4.0f)) {
        Actor* interactRangeActor;

        interactRangeActor = this->interactRangeActor;
        if (!Player_InterruptHoldingActor(play, this, interactRangeActor)) {
            this->actor.child = interactRangeActor;
            this->heldActor = interactRangeActor;
            interactRangeActor->parent = &this->actor;
            interactRangeActor->bgCheckFlags &= ~0xFF;
            this->leftHandWorld.rot.y = interactRangeActor->shape.rot.y - this->actor.shape.rot.y;
        }
    } else {
        Math_ScaledStepToS(&this->leftHandWorld.rot.y, 0, 0xFA0);
    }
}

// grab/hold an actor (?)
void Player_LiftSilverBoulder(Player* this, PlayState* play) {
    Player_StepLinearVelocityToZero(this);

    if (LinkAnimation_Update(play, &this->skelAnime)) {
        Player_AnimationPlayLoop(play, this, &gPlayerAnim_link_silver_wait);
        this->genericTimer = 1;
    } else if (this->genericTimer == 0) {
        if (LinkAnimation_OnFrame(&this->skelAnime, 27.0f)) {
            Actor* interactRangeActor = this->interactRangeActor;

            this->heldActor = interactRangeActor;
            this->actor.child = interactRangeActor;
            interactRangeActor->parent = &this->actor;
        } else if (LinkAnimation_OnFrame(&this->skelAnime, 25.0f)) {
            Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_SWORD_L);
        }
    } else if (CHECK_BTN_ANY(sPlayerControlInput->press.button, BTN_CRIGHT | BTN_CLEFT | BTN_CDOWN | BTN_B | BTN_A)) {
        Player_SetAction(play, this, Player_ThrowSilverBoulder, 1);
        Player_AnimationPlayOnce(play, this, &gPlayerAnim_link_silver_throw);
    }
}

// throw held actor (?)
void Player_ThrowSilverBoulder(Player* this, PlayState* play) {
    if (LinkAnimation_Update(play, &this->skelAnime)) {
        Player_SetupStandingStillType(this, play);
    } else if (LinkAnimation_OnFrame(&this->skelAnime, 6.0f)) {
        Actor* heldActor = this->heldActor;

        heldActor->world.rot.y = this->actor.shape.rot.y;
        heldActor->speedXZ = 10.0f;
        heldActor->velocity.y = 20.0f;
        Player_SetupHeldItemUpperActionFunc(play, this);
        Player_PlaySfx(this, NA_SE_PL_THROW);
        Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_SWORD_N);
    }
}

void Player_FailToLiftActor(Player* this, PlayState* play) {
    if (LinkAnimation_Update(play, &this->skelAnime)) {
        Player_AnimationPlayLoop(play, this, &gPlayerAnim_link_normal_nocarry_free_wait);
        this->genericTimer = 0xF;
    } else if (this->genericTimer != 0) {
        this->genericTimer--;
        if (this->genericTimer == 0) {
            Player_SetupReturnToStandStillSetAnim(this, &gPlayerAnim_link_normal_nocarry_free_end, play);
            this->stateFlags1 &= ~PLAYER_STATE1_HOLDING_ACTOR;
            Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_DAMAGE_S);
        }
    }
}

// Player_Action_PutDownObject?
void func_8084E58C(Player* this, PlayState* play) {
    Player_StepLinearVelocityToZero(this);

    if (LinkAnimation_Update(play, &this->skelAnime)) {
        Player_SetupStandingStillType(this, play);
    } else if (LinkAnimation_OnFrame(&this->skelAnime, 4.0f)) {
        Actor* heldActor = this->heldActor;

        if (!Player_InterruptHoldingActor(play, this, heldActor)) {
            heldActor->velocity.y = 0.0f;
            heldActor->speedXZ = 0.0f;
            Player_SetupHeldItemUpperActionFunc(play, this);
            if (heldActor->id == ACTOR_EN_BOM_CHU) {
                func_80831814(this, play, PLAYER_ATTENTIONMODE_NONE);
            }
        }
    }
}

// Player_Action_Throwing
void Player_StartThrowActor(Player* this, PlayState* play) {
    f32 targetVelocity;
    s16 targetYaw;

    Player_StepLinearVelocityToZero(this);

    if (LinkAnimation_Update(play, &this->skelAnime) ||
        ((this->skelAnime.curFrame >= 8.0f) &&
         Player_GetTargetVelocityAndYaw(this, &targetVelocity, &targetYaw, 0.018f, play))) {
        Player_SetupStandingStillType(this, play);
    } else if (LinkAnimation_OnFrame(&this->skelAnime, 3.0f)) {
        Player_ThrowActor(play, this, this->linearVelocity + 8.0f, 12.0f);
    }
}

void func_8084E724(Player* this, PlayState* play) {
    u8 temp_v1;
    unsigned int new_var;

    if (this->stateFlags1 & PLAYER_STATE1_SWIMMING) {
        Player_SetVerticalWaterVelocity(this);
        Player_UpdateSwimMovement(this, &this->linearVelocity, 0.0f, this->actor.shape.rot.y);
    } else {
        Player_StepLinearVelocityToZero(this);
    }

    if (this->attentionMode == PLAYER_ATTENTIONMODE_AIMING) {
        if (Actor_PlayerIsAimingFpsItem(this) || Player_IsAimingZoraFins(this)) {
            Player_SetupCurrentUpperAction(this, play);
        }
    }

    temp_v1 = this->attentionMode;
    new_var = temp_v1;

    if (((temp_v1 == PLAYER_UNKAA5_2) && !(play->actorCtx.flags & ACTORCTX_FLAG_PICTOGRAPH_ON)) ||
        ((temp_v1 != PLAYER_UNKAA5_2) &&
         (((((((((this->csMode != PLAYER_CSMODE_0) || (new_var == 0)) || ((s32)temp_v1 >= PLAYER_UNKAA5_5)) ||
               (Player_SetupStartUnfriendlyZTargeting(this) != 0)) ||
              (this->targetedActor != NULL)) ||
             (Player_SetupCameraMode(play, this) == 0)) ||
            ((this->attentionMode == PLAYER_ATTENTIONMODE_AIMING) &&
             (((((Player_ItemToActionParam(this, Inventory_GetBtnBItem(play)) != this->itemActionParam) &&
                 CHECK_BTN_ANY(sPlayerControlInput->press.button, BTN_B)) ||
                CHECK_BTN_ANY(sPlayerControlInput->press.button, BTN_R | BTN_A)) ||
               (Player_IsFriendlyZTargeting(this) != 0)) ||
              (!Actor_PlayerIsAimingReadyFpsItem(this) && (Player_IsAimingReadyZoraFins(this) == 0))))) ||
           ((this->attentionMode == PLAYER_ATTENTIONMODE_C_UP) &&
            CHECK_BTN_ANY(sPlayerControlInput->press.button,
                          BTN_CRIGHT | BTN_CLEFT | BTN_CDOWN | BTN_CUP | BTN_R | BTN_B | BTN_A))) ||
          Player_SetupSpeakOrCheck(this, play)))) {
        func_80839ED0(this, play);
        play_sound(NA_SE_SY_CAMERA_ZOOM_UP);
    } else if ((DECR(this->genericTimer) == 0) || (this->attentionMode != PLAYER_ATTENTIONMODE_AIMING)) {
        if (Player_IsShootingHookshot(this)) {
            this->lookFlags |= 0x43;
        } else {
            this->actor.shape.rot.y = Player_SetFirstPersonAimLookAngles(play, this, 0);
        }
    }

    this->currentYaw = this->actor.shape.rot.y;
}

void Player_TalkWithActor(Player* this, PlayState* play) {
    this->stateFlags2 |= PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING;

    func_8083249C(this);
    Player_SetupCurrentUpperAction(this, play);
    if (Message_GetState(&play->msgCtx) == TEXT_STATE_CLOSING) {
        this->actor.flags &= ~ACTOR_FLAG_100;
        if (!CHECK_FLAG_ALL(this->talkActor->flags, ACTOR_FLAG_1 | ACTOR_FLAG_4)) {
            this->stateFlags2 &= ~PLAYER_STATE2_2000;
        }

        func_800E0238(Play_GetCamera(play, CAM_ID_MAIN));
        ActorCutscene_Stop(0x7C);
        if (this->stateFlags1 & PLAYER_STATE1_RIDING_HORSE) {
            s32 sp44 = this->genericTimer;

            Player_SetupRideHorse(play, this);
            this->genericTimer = sp44;
        } else if (!Player_ForceOcarina(play, this) && !Player_SetupShootingGalleryPlay(play, this) &&
                   !Player_SetupCutscene(play, this) &&
                   ((this->talkActor != this->interactRangeActor) || !Player_SetupGetItemOrHoldBehavior(this, play))) {
            if (Player_IsSwimming(this)) {
                Player_SetupSwimIdle(play, this);
            } else {
                Player_SetupStandingStillMorph(this, play);
            }
        }

        this->unk_B5E = 0xA;
        return;
    }

    if (this->stateFlags1 & PLAYER_STATE1_RIDING_HORSE) {
        Player_RideHorse(this, play);
    } else if (Player_IsSwimming(this)) {
        Player_UpdateSwimIdle(this, play);
        if (this->actor.depthInWater > 100.0f) {
            this->actor.velocity.y = 0.0f;
            this->actor.gravity = 0.0f;
        }
    } else if (!Player_IsUnfriendlyZTargeting(this) && LinkAnimation_Update(play, &this->skelAnime)) {
        if (this->skelAnime.moveFlags != 0) {
            Player_EndAnimMovement(this);
            if ((this->talkActor->category == ACTORCAT_NPC) && (this->itemActionParam != PLAYER_AP_FISHING_ROD)) {
                Player_PlayAnimOnceSlowed(play, this, &gPlayerAnim_link_normal_talk_free);
            } else {
                Player_AnimationPlayLoop(play, this, Player_GetStandingStillAnim(this));
            }
        } else {
            func_8082DB60(play, this, &gPlayerAnim_link_normal_talk_free_wait);
        }
    }

    if (this->targetedActor != NULL) {
        this->currentYaw = Player_LookAtTargetActor(this, false);
        this->actor.shape.rot.y = this->currentYaw;
        if (this->genericVar != 0) {
            if (!(this->stateFlags1 & PLAYER_STATE1_HOLDING_ACTOR)) {
                if (LinkAnimation_Update(play, &this->skelAnimeUpper)) {
                    this->genericVar -= 1;
                    if (this->genericVar != 0) {
                        LinkAnimation_Change(play, &this->skelAnimeUpper, &gPlayerAnim_link_normal_talk_free, 1.0f,
                                             0.0f, Animation_GetLastFrame(&gPlayerAnim_link_normal_talk_free),
                                             ANIMMODE_ONCE, -6.0f);
                    }
                }
            }
            AnimationContext_SetCopyFalse(play, this->skelAnime.limbCount, this->skelAnime.jointTable,
                                          this->skelAnimeUpper.jointTable, D_8085B9F0);
        } else if (!(this->stateFlags1 & PLAYER_STATE1_HOLDING_ACTOR) &&
                   (this->skelAnime.animation == &gPlayerAnim_link_normal_talk_free_wait)) {
            s32 temp_v0 = this->actor.focus.rot.y - this->actor.shape.rot.y;

            if (ABS_ALT(temp_v0) > 0xFA0) {
                LinkAnimation_Change(
                    play, &this->skelAnimeUpper, D_8085BE84[PLAYER_ANIMGROUP_25][this->modelAnimType], 0.4f, 0.0f,
                    Animation_GetLastFrame(D_8085BE84[PLAYER_ANIMGROUP_25][this->modelAnimType]), ANIMMODE_ONCE, -6.0f);
                this->genericVar = 2;
            }
        }
    }
}

void Player_GrabPushPullWall(Player* this, PlayState* play) {
    f32 targetVelocity;
    s16 targetYaw;
    s32 pushPullDir;

    this->stateFlags2 |= (PLAYER_STATE2_1 | PLAYER_STATE2_ALWAYS_DISABLE_MOVE_ROTATION | PLAYER_STATE2_100);
    Player_PushPullSetPositionAndYaw(play, this);

    if (LinkAnimation_Update(play, &this->skelAnime) && !Player_SetupPushPullWallIdle(play, this)) {
        Player_GetTargetVelocityAndYaw(this, &targetVelocity, &targetYaw, 0.0f, play);
        pushPullDir = Player_GetPushPullDirection(this, &targetVelocity, &targetYaw);
        if (pushPullDir > 0) {
            Player_SetupPushWall(this, play);
        } else if (pushPullDir < 0) {
            Player_SetupPullWall(this, play);
        }
    }
}

AnimSfxEntry D_8085D650[] = {
    ANIMSFX(ANIMSFX_TYPE_FLOOR, 3, NA_SE_PL_SLIP, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_FLOOR, 21, NA_SE_PL_SLIP, STOP),
};

void func_8084EE50(Player* this, PlayState* play) {
    this->stateFlags2 |= (PLAYER_STATE2_1 | PLAYER_STATE2_ALWAYS_DISABLE_MOVE_ROTATION | PLAYER_STATE2_100);

    if (Player_LoopAnimContinuously(play, this, &gPlayerAnim_link_normal_pushing)) {
        this->genericTimer = 1;
    } else if ((this->genericTimer == 0) && LinkAnimation_OnFrame(&this->skelAnime, 11.0f)) {
        Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_PUSH);
    }

    Player_PlayAnimSfx(this, D_8085D650);
    Player_PushPullSetPositionAndYaw(play, this);

    if (!Player_SetupPushPullWallIdle(play, this)) {
        f32 sp34;
        s16 sp32;
        s32 temp_v0;

        Player_GetTargetVelocityAndYaw(this, &sp34, &sp32, 0.0f, play);
        temp_v0 = Player_GetPushPullDirection(this, &sp34, &sp32);
        if (temp_v0 < 0) {
            Player_SetupPullWall(this, play);
        } else if (temp_v0 == 0) {
            func_8083DF38(this, &gPlayerAnim_link_normal_push_end, play);
        } else {
            this->stateFlags2 |= PLAYER_STATE2_10;
        }
    }

    if (this->stateFlags2 & PLAYER_STATE2_10) {
        func_808479F4(play, this, 2.0f);
        this->linearVelocity = 2.0f;
    }
}

AnimSfxEntry D_8085D658[] = {
    ANIMSFX(ANIMSFX_TYPE_FLOOR, 4, NA_SE_PL_SLIP, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_FLOOR, 24, NA_SE_PL_SLIP, STOP),
};

Vec3f D_8085D660 = { 0.0f, 26.800001f, -60.0f };

void func_8084EF9C(Player* this, PlayState* play) {
    LinkAnimationHeader* anim = D_8085BE84[PLAYER_ANIMGROUP_35][this->modelAnimType];

    this->stateFlags2 |= (PLAYER_STATE2_1 | PLAYER_STATE2_ALWAYS_DISABLE_MOVE_ROTATION | PLAYER_STATE2_100);

    if (Player_LoopAnimContinuously(play, this, anim)) {
        this->genericTimer = 1;
    } else if (this->genericTimer == 0) {
        if (LinkAnimation_OnFrame(&this->skelAnime, 11.0f)) {
            Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_PUSH);
        }
        if (0) {}
    } else {
        Player_PlayAnimSfx(this, D_8085D658);
    }

    Player_PushPullSetPositionAndYaw(play, this);
    if (!Player_SetupPushPullWallIdle(play, this)) {
        f32 sp78;
        s16 sp76;
        s32 temp_v0;

        Player_GetTargetVelocityAndYaw(this, &sp78, &sp76, 0.0f, play);

        temp_v0 = Player_GetPushPullDirection(this, &sp78, &sp76);
        if (temp_v0 > 0) {
            Player_SetupPushWall(this, play);
        } else if (temp_v0 == 0) {
            func_8083DF38(this, D_8085BE84[PLAYER_ANIMGROUP_36][this->modelAnimType], play);
        } else {
            this->stateFlags2 |= PLAYER_STATE2_10;
        }
    }

    if (this->stateFlags2 & PLAYER_STATE2_10) {
        Vec3f sp64;
        f32 yIntersect;
        CollisionPoly* poly;
        s32 bgId;
        Vec3f sp4C;
        Vec3f sp40;

        yIntersect = func_80835D2C(play, this, &D_8085D660, &sp64) - this->actor.world.pos.y;
        if (fabsf(yIntersect) < 26.800001f) {
            sp64.y -= 7.0f;
            sp4C.x = this->actor.world.pos.x;
            sp4C.z = this->actor.world.pos.z;
            sp4C.y = sp64.y;
            if (!BgCheck_EntityLineTest2(&play->colCtx, &sp4C, &sp64, &sp40, &poly, 1, 0, 0, 1, &bgId, &this->actor)) {
                func_808479F4(play, this, -2.0f);
                return;
            }
        }
        this->stateFlags2 &= ~PLAYER_STATE2_10;
    }
}

void func_8084F1B8(Player* this, PlayState* play) {
    f32 sp3C;
    s16 sp3A;

    this->stateFlags2 |= PLAYER_STATE2_ALWAYS_DISABLE_MOVE_ROTATION;

    if (LinkAnimation_Update(play, &this->skelAnime)) {
        Player_AnimationPlayLoop(play, this,
                                 (this->genericVar > 0) ? &gPlayerAnim_link_normal_fall_wait
                                                        : D_8085BE84[PLAYER_ANIMGROUP_39][this->modelAnimType]);
    } else if (this->genericVar == 0) {
        f32 frame;

        if (this->skelAnime.animation == &gPlayerAnim_link_normal_fall) {
            frame = 11.0f;
        } else {
            frame = 1.0f;
        }

        if (LinkAnimation_OnFrame(&this->skelAnime, frame)) {
            Player_AnimSfx_PlayFloor(this, NA_SE_PL_WALK_GROUND);
            if (this->skelAnime.animation == &gPlayerAnim_link_normal_fall) {
                this->genericVar = 1;
            } else {
                this->genericVar = -1;
            }
        }
    }

    Math_ScaledStepToS(&this->actor.shape.rot.y, this->currentYaw, 0x800);
    if (this->genericVar != 0) {
        Player_GetTargetVelocityAndYaw(this, &sp3C, &sp3A, 0.0f, play);
        if (this->analogStickInputs[this->inputFrameCounter] >= 0) {
            func_808381A0(this,
                          (this->genericVar > 0) ? D_8085BE84[PLAYER_ANIMGROUP_37][this->modelAnimType]
                                                 : D_8085BE84[PLAYER_ANIMGROUP_40][this->modelAnimType],
                          play);
        } else if (CHECK_BTN_ALL(sPlayerControlInput->cur.button, BTN_A) || (this->actor.shape.feetFloorFlags != 0)) {
            func_80833A64(this);

            if (this->genericVar < 0) {
                this->linearVelocity = -0.8f;
            } else {
                this->linearVelocity = 0.8f;
            }

            func_80833AA0(this, play);
            this->stateFlags1 &= ~(PLAYER_STATE1_4 | PLAYER_STATE1_2000 | PLAYER_STATE1_4000);
            this->actor.bgCheckFlags &= ~1;
        }
    }
}

void func_8084F3DC(Player* this, PlayState* play) {
    this->stateFlags2 |= PLAYER_STATE2_ALWAYS_DISABLE_MOVE_ROTATION;

    if (LinkAnimation_Update(play, &this->skelAnime)) {
        this->currentYaw = this->skelAnime.jointTable[PLAYER_LIMB_WAIST - 1].y + this->actor.shape.rot.y;
        Player_UpdateAnimMovement(this, 1);
        this->actor.shape.rot.y = this->currentYaw;
        Player_SetupStandingStillNoMorph(this, play);
        this->stateFlags1 &= ~(PLAYER_STATE1_4 | PLAYER_STATE1_2000 | PLAYER_STATE1_4000);
    } else if (LinkAnimation_OnFrame(&this->skelAnime, this->skelAnime.endFrame - 6.0f)) {
        Player_AnimSfx_PlayFloorLand(this);
    } else if (LinkAnimation_OnFrame(&this->skelAnime, this->skelAnime.endFrame - 34.0f)) {
        Player_PlaySfx(this, NA_SE_PL_CLIMB_CLIFF);
        Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_CLIMB_END);
        func_8084C124(play, this);
    }
}

void func_8084F4E8(Player* this, PlayState* play) {
    s32 yStick = sPlayerControlInput->rel.stick_y;
    s32 xStick = sPlayerControlInput->rel.stick_x;
    f32 var_fv0;
    f32 var_fv1;
    Vec3f sp7C;
    s32 sp78;
    Vec3f sp6C;
    Vec3f sp60;
    DynaPolyActor* dyna;
    LinkAnimationHeader* anim1;
    LinkAnimationHeader* anim2;

    this->fallStartHeight = this->actor.world.pos.y;

    this->stateFlags2 |= PLAYER_STATE2_ALWAYS_DISABLE_MOVE_ROTATION;

    if ((this->genericVar != 0) && (ABS_ALT(yStick) < ABS_ALT(xStick))) {
        var_fv0 = ABS_ALT(xStick) * 0.0325f;
        yStick = 0;
    } else {
        var_fv0 = ABS_ALT(yStick) * 0.05f;
        xStick = 0;
    }

    if (var_fv0 < 1.0f) {
        var_fv0 = 1.0f;
    } else if (var_fv0 > 3.35f) {
        var_fv0 = 3.35f;
    }

    if (this->skelAnime.playSpeed >= 0.0f) {
        var_fv1 = 1.0f;
    } else {
        var_fv1 = -1.0f;
    }

    this->skelAnime.playSpeed = var_fv1 * var_fv0;

    if (this->genericTimer >= 0) {
        if ((this->actor.wallPoly != NULL) && (this->actor.wallBgId != BGCHECK_SCENE)) {
            dyna = DynaPoly_GetActor(&play->colCtx, this->actor.wallBgId);

            if (dyna != NULL) {
                Math_Vec3f_Diff(&dyna->actor.world.pos, &dyna->actor.prevPos, &sp7C);
                Math_Vec3f_Sum(&this->actor.world.pos, &sp7C, &this->actor.world.pos);
            }
        }

        Actor_UpdateBgCheckInfo(play, &this->actor, 26.800001f, 6.0f, this->ageProperties->unk_00 + 15.0f, 7);
        Player_SetPositionAndYawOnClimbWall(play, this, 26.800001f, this->ageProperties->unk_3C, 50.0f, -20.0f);
    }

    func_80831944(play, this);

    if ((this->genericTimer < 0) || !func_8083E354(this, play)) {
        if (LinkAnimation_Update(play, &this->skelAnime)) {
            if (this->genericTimer < 0) {
                this->genericTimer = ABS_ALT(this->genericTimer) & 1;
            } else if (yStick != 0) {
                f32 yIntersect;

                sp78 = this->genericVar + this->genericTimer;

                if (yStick > 0) {
                    sp6C.x = 0.0f;
                    sp6C.y = this->ageProperties->unk_40;
                    sp6C.z = this->ageProperties->unk_3C + 10.0f;

                    yIntersect = func_80835D2C(play, this, &sp6C, &sp60);

                    if (this->actor.world.pos.y < yIntersect) {
                        if (this->genericVar != 0) {
                            this->actor.world.pos.y = yIntersect;
                            this->stateFlags1 &= ~PLAYER_STATE1_200000;
                            func_80837CEC(play, this, this->actor.wallPoly, this->ageProperties->unk_3C,
                                          &gPlayerAnim_link_normal_jump_climb_up_free);
                            this->currentYaw += 0x8000;
                            this->actor.shape.rot.y = this->currentYaw;
                            func_808381A0(this, &gPlayerAnim_link_normal_jump_climb_up_free, play);
                            this->stateFlags1 |= PLAYER_STATE1_4000;
                        } else {
                            Player_SetupEndClimb(this, this->ageProperties->unk_D4[this->genericTimer], play);
                        }
                    } else {
                        this->skelAnime.prevTransl = this->ageProperties->unk_4A[sp78];
                        Player_AnimationPlayOnce(play, this, this->ageProperties->unk_B4[sp78]);
                    }
                } else if ((this->actor.world.pos.y - this->actor.floorHeight) < 15.0f) {
                    if (this->genericVar != 0) {
                        func_8083E2F4(this, play);
                    } else {
                        if (this->genericTimer != 0) {
                            this->skelAnime.prevTransl = this->ageProperties->unk_44;
                        }

                        Player_SetupEndClimb(this, this->ageProperties->unk_CC[this->genericTimer], play);
                        this->genericTimer = 1;
                    }
                } else {
                    sp78 ^= 1;
                    this->skelAnime.prevTransl = this->ageProperties->unk_62[sp78];
                    anim1 = this->ageProperties->unk_B4[sp78];
                    LinkAnimation_Change(play, &this->skelAnime, anim1, -1.0f, Animation_GetLastFrame(anim1), 0.0f, 2,
                                         0.0f);
                }

                this->genericTimer ^= 1;
            } else if ((this->genericVar != 0) && (xStick != 0)) {
                anim2 = this->ageProperties->unk_C4[this->genericTimer];

                if (xStick > 0) {
                    this->skelAnime.prevTransl = this->ageProperties->unk_7A[this->genericTimer];
                    Player_AnimationPlayOnce(play, this, anim2);
                } else {
                    this->skelAnime.prevTransl = this->ageProperties->unk_7A[this->genericTimer + 2];
                    LinkAnimation_Change(play, &this->skelAnime, anim2, -1.0f, Animation_GetLastFrame(anim2), 0.0f, 2,
                                         0.0f);
                }
            } else {
                this->stateFlags2 |= PLAYER_STATE2_1000;
            }

            return;
        }
    }

    if (this->genericTimer < 0) {
        if (((this->genericTimer == -2) &&
             (LinkAnimation_OnFrame(&this->skelAnime, 14.0f) || LinkAnimation_OnFrame(&this->skelAnime, 29.0f))) ||
            ((this->genericTimer == -4) &&
             (LinkAnimation_OnFrame(&this->skelAnime, 22.0f) || LinkAnimation_OnFrame(&this->skelAnime, 35.0f) ||
              LinkAnimation_OnFrame(&this->skelAnime, 49.0f) || LinkAnimation_OnFrame(&this->skelAnime, 55.0f)))) {
            func_80847A50(this);
        }
    } else if (LinkAnimation_OnFrame(&this->skelAnime, (this->skelAnime.playSpeed > 0.0f) ? 20.0f : 0.0f)) {
        func_80847A50(this);
    }
}

f32 D_8085D66C[] = { 11.0f, 21.0f };
f32 D_8085D674[] = { 40.0f, 50.0f };

AnimSfxEntry D_8085D67C[] = {
    ANIMSFX(ANIMSFX_TYPE_SURFACE, 10, NA_SE_PL_WALK_LADDER, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_SURFACE, 20, NA_SE_PL_WALK_LADDER, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_SURFACE, 30, NA_SE_PL_WALK_LADDER, STOP),
};

void Player_EndClimb(Player* this, PlayState* play) {
    s32 actionInterruptResult;
    f32* var_v1;

    this->stateFlags2 |= PLAYER_STATE2_ALWAYS_DISABLE_MOVE_ROTATION;

    actionInterruptResult = Player_IsActionInterrupted(play, this, &this->skelAnime, 4.0f);

    if (actionInterruptResult == 0) {
        this->stateFlags1 &= ~PLAYER_STATE1_200000;
        return;
    }

    if ((actionInterruptResult > 0) || LinkAnimation_Update(play, &this->skelAnime)) {
        Player_SetupStandingStillNoMorph(this, play);
        this->stateFlags1 &= ~PLAYER_STATE1_200000;
        return;
    }

    var_v1 = D_8085D66C;

    if (this->genericTimer != 0) {
        Player_PlayAnimSfx(this, D_8085D67C);
        var_v1 = D_8085D674;
    }

    if (LinkAnimation_OnFrame(&this->skelAnime, var_v1[0]) || LinkAnimation_OnFrame(&this->skelAnime, var_v1[1])) {
        CollisionPoly* poly;
        s32 bgId;
        Vec3f pos;

        pos.x = this->actor.world.pos.x;
        pos.y = this->actor.world.pos.y + 20.0f;
        pos.z = this->actor.world.pos.z;
        if (BgCheck_EntityRaycastFloor5(&play->colCtx, &poly, &bgId, &this->actor, &pos) != 0.0f) {
            this->floorSfxOffset = SurfaceType_GetSfxIdOffset(&play->colCtx, poly, bgId);
            Player_AnimSfx_PlayFloorLand(this);
        }
    }
}

void func_8084FD7C(PlayState* play, Player* this, Actor* actor) {
    s16 var_a3;

    if (this->unk_B86[0] != 0) {
        this->unk_B86[0]--;
        return;
    }

    this->upperBodyRot.y = Player_SetFirstPersonAimLookAngles(play, this, 1) - this->actor.shape.rot.y;

    var_a3 = ABS_ALT(this->upperBodyRot.y) - 0x4000;
    if (var_a3 > 0) {
        var_a3 = CLAMP_MAX(var_a3, 0x15E);
        actor->shape.rot.y += var_a3 * ((this->upperBodyRot.y >= 0) ? 1 : -1);
        actor->world.rot.y = actor->shape.rot.y;
    }

    this->upperBodyRot.y += 0x2710;
    this->unk_AA8 = -0x1388;
}

s32 func_8084FE48(Player* this) {
    return this->targetedActor == NULL && !Player_IsZTargetingSetupStartUnfriendly(this);
}

LinkAnimationHeader* D_8085D688[] = {
    &gPlayerAnim_link_uma_anim_stop,    &gPlayerAnim_link_uma_anim_stand,   &gPlayerAnim_link_uma_anim_walk,
    &gPlayerAnim_link_uma_anim_slowrun, &gPlayerAnim_link_uma_anim_fastrun, &gPlayerAnim_link_uma_anim_jump100,
    &gPlayerAnim_link_uma_anim_jump200,
};

LinkAnimationHeader* D_8085D6A4[] = {
    NULL,
    NULL,
    &gPlayerAnim_link_uma_anim_walk_muti,
    &gPlayerAnim_link_uma_anim_walk_muti,
    &gPlayerAnim_link_uma_anim_walk_muti,
    &gPlayerAnim_link_uma_anim_slowrun_muti,
    &gPlayerAnim_link_uma_anim_fastrun_muti,
    &gPlayerAnim_link_uma_anim_fastrun_muti,
    &gPlayerAnim_link_uma_anim_fastrun_muti,
    NULL,
    NULL,
};

LinkAnimationHeader* D_8085D6D0[] = {
    &gPlayerAnim_link_uma_wait_3,
    &gPlayerAnim_link_uma_wait_1,
    &gPlayerAnim_link_uma_wait_2,
};

u8 D_8085D6DC[][2] = {
    { 0x20, 0x3A },
    { 0x19, 0x2A },
};

Vec3s D_8085D6E0 = { -0x45, 0x1BEA, -0x10A };

AnimSfxEntry D_8085D6E8[] = {
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 48, NA_SE_PL_CALM_HIT, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 58, NA_SE_PL_CALM_HIT, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 68, NA_SE_PL_CALM_HIT, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 92, NA_SE_PL_CALM_PAT, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 110, NA_SE_PL_CALM_PAT, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 126, NA_SE_PL_CALM_PAT, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 132, NA_SE_PL_CALM_PAT, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 136, NA_SE_PL_CALM_PAT, STOP),
};

void Player_RideHorse(Player* this, PlayState* play) {
    EnHorse* rideActor = (EnHorse*)this->rideActor;

    this->stateFlags2 |= PLAYER_STATE2_ALWAYS_DISABLE_MOVE_ROTATION;

    func_80847E2C(this, 1.0f, 10.0f);

    if (this->genericTimer == 0) {
        if (LinkAnimation_Update(play, &this->skelAnime)) {
            this->skelAnime.animation = &gPlayerAnim_link_uma_wait_1;
            this->genericTimer = 0x63;
        } else {
            s32 var_v0 = (this->mountSide < 0) ? 0 : 1;

            if (LinkAnimation_OnFrame(&this->skelAnime, D_8085D6DC[var_v0][0])) {
                Actor_SetCameraHorseSetting(play, this);
                Player_PlaySfx(this, NA_SE_PL_CLIMB_CLIFF);
            } else if (LinkAnimation_OnFrame(&this->skelAnime, D_8085D6DC[var_v0][1])) {
                Player_PlaySfx(this, NA_SE_PL_SIT_ON_HORSE);
            }
        }
    } else {
        if (rideActor->actor.bgCheckFlags & 1) {
            func_80841A50(play, this);
        }

        Actor_SetCameraHorseSetting(play, this);

        this->skelAnime.prevTransl = D_8085D6E0;

        if ((this->genericTimer < 0) ||
            ((rideActor->animIndex != (this->genericTimer & 0xFFFF)) &&
             ((rideActor->animIndex >= ENHORSE_ANIM_STOPPING) || (this->genericTimer >= 2)))) {
            s32 animationIdx = rideActor->animIndex;

            if (animationIdx < ENHORSE_ANIM_STOPPING) {
                f32 temp_fv0 = Rand_ZeroOne();
                s32 index = 0;

                animationIdx = ENHORSE_ANIM_WHINNY;
                if (temp_fv0 < 0.1f) {
                    index = 2;
                } else if (temp_fv0 < 0.2f) {
                    index = 1;
                }

                Player_AnimationPlayOnce(play, this, D_8085D6D0[index]);
            } else {
                this->skelAnime.animation = D_8085D688[animationIdx - 2];
                if (this->genericTimer >= 0) {
                    Animation_SetMorph(play, &this->skelAnime, 8.0f);
                }

                if (animationIdx < ENHORSE_ANIM_WALK) {
                    Player_SetupHeldItemUpperActionFunc(play, this);
                    this->genericVar = 0;
                }
            }

            this->genericTimer = animationIdx;
        }

        if (this->genericTimer == 1) {
            if (D_80862B04 || func_8082DAFC(play)) {
                Player_AnimationPlayOnce(play, this, &gPlayerAnim_link_uma_wait_3);
            } else if (LinkAnimation_Update(play, &this->skelAnime)) {
                this->genericTimer = 0x63;
            } else if (this->skelAnime.animation == &gPlayerAnim_link_uma_wait_1) {
                Player_PlayAnimSfx(this, D_8085D6E8);
            }
        } else {
            this->skelAnime.curFrame = rideActor->curFrame;
            LinkAnimation_AnimateFrame(play, &this->skelAnime);
        }

        AnimationContext_SetCopyAll(play, this->skelAnime.limbCount, this->skelAnime.morphTable,
                                    this->skelAnime.jointTable);

        if ((play->csCtx.state != CS_STATE_IDLE) || (this->csMode != PLAYER_CSMODE_0)) {
            this->attentionMode = PLAYER_ATTENTIONMODE_NONE;
            this->genericVar = 0;
        } else if ((this->genericTimer < 2) || (this->genericTimer >= 4)) {
            D_80862B04 = Player_SetupCurrentUpperAction(this, play);
            if (D_80862B04) {
                this->genericVar = 0;
            }
        }

        this->actor.world.pos.x = rideActor->actor.world.pos.x + rideActor->riderPos.x;
        this->actor.world.pos.y = rideActor->actor.world.pos.y + rideActor->riderPos.y - 27.0f;
        this->actor.world.pos.z = rideActor->actor.world.pos.z + rideActor->riderPos.z;

        this->currentYaw = this->actor.shape.rot.y = rideActor->actor.shape.rot.y;

        if (!D_80862B04) {
            if (this->genericVar != 0) {
                if (LinkAnimation_Update(play, &this->skelAnimeUpper)) {
                    rideActor->stateFlags &= ~ENHORSE_FLAG_8;
                    this->genericVar = 0;
                }

                if (this->skelAnimeUpper.animation == &gPlayerAnim_link_uma_stop_muti) {
                    if (LinkAnimation_OnFrame(&this->skelAnimeUpper, 23.0f)) {
                        Player_PlaySfx(this, NA_SE_IT_LASH);
                        Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_LASH);
                    }

                    AnimationContext_SetCopyAll(play, this->skelAnime.limbCount, this->skelAnime.jointTable,
                                                this->skelAnimeUpper.jointTable);
                } else {
                    if (LinkAnimation_OnFrame(&this->skelAnimeUpper, 10.0f)) {
                        Player_PlaySfx(this, NA_SE_IT_LASH);
                        Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_LASH);
                    }

                    AnimationContext_SetCopyTrue(play, this->skelAnime.limbCount, this->skelAnime.jointTable,
                                                 this->skelAnimeUpper.jointTable, D_8085B9F0);
                }
            } else if (!CHECK_FLAG_ALL(this->actor.flags, 0x100)) {
                LinkAnimationHeader* anim = NULL;

                if (EN_HORSE_CHECK_3(rideActor)) {
                    anim = &gPlayerAnim_link_uma_stop_muti;
                } else if (EN_HORSE_CHECK_2(rideActor)) {
                    if ((this->genericTimer >= 2) && (this->genericTimer != 0x63)) {
                        anim = D_8085D6A4[this->genericTimer];
                    }
                }

                if (anim != NULL) {
                    LinkAnimation_PlayOnce(play, &this->skelAnimeUpper, anim);
                    this->genericVar = 1;
                }
            }
        }

        if (this->stateFlags1 & PLAYER_STATE1_100000) {
            if (CHECK_BTN_ANY(sPlayerControlInput->press.button, BTN_A) || !func_8084FE48(this)) {
                this->attentionMode = PLAYER_ATTENTIONMODE_NONE;
                this->stateFlags1 &= ~PLAYER_STATE1_100000;
            } else {
                func_8084FD7C(play, this, &rideActor->actor);
            }
        } else if ((this->csMode != PLAYER_CSMODE_0) ||
                   (!func_8082DAFC(play) &&
                    ((rideActor->actor.speedXZ != 0.0f) || !Player_SetupSpeakOrCheck(this, play)) &&
                    !func_80847BF0(this, play) && !Player_SetupItemCutsceneOrFirstPerson(this, play))) {
            if (this->targetedActor != NULL) {
                if (Actor_PlayerIsAimingReadyFpsItem(this)) {
                    this->upperBodyRot.y = Player_LookAtTargetActor(this, true) - this->actor.shape.rot.y;
                    this->upperBodyRot.y = CLAMP(this->upperBodyRot.y, -0x4AAA, 0x4AAA);
                    this->actor.focus.rot.y = this->actor.shape.rot.y + this->upperBodyRot.y;
                    this->upperBodyRot.y += 0xFA0;
                    this->lookFlags |= 0x80;
                } else {
                    Player_LookAtTargetActor(this, false);
                }

                this->unk_AA8 = 0;
            } else if (func_8084FE48(this)) {
                if (Actor_PlayerIsAimingReadyFpsItem(this)) {
                    Player_SetupAimAttention(this, play);
                }

                this->unk_B86[0] = 0xC;
            } else if (Actor_PlayerIsAimingReadyFpsItem(this)) {
                func_8084FD7C(play, this, &rideActor->actor);
            }
        }
    }

    if (this->csMode == PLAYER_CSMODE_6) {
        this->csMode = PLAYER_CSMODE_0;
    }
}

AnimSfxEntry D_8085D708[] = {
    ANIMSFX(ANIMSFX_TYPE_FLOOR_LAND, 0, 0, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 10, NA_SE_PL_GET_OFF_HORSE, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 25, NA_SE_PL_SLIPDOWN, STOP),
};

void func_808505D0(Player* this, PlayState* play) {
    this->stateFlags2 |= PLAYER_STATE2_ALWAYS_DISABLE_MOVE_ROTATION;
    func_80847E2C(this, 1.0f, 10.0f);

    if (LinkAnimation_Update(play, &this->skelAnime)) {
        Actor* rideActor = this->rideActor;

        Camera_ChangeSetting(Play_GetCamera(play, CAM_ID_MAIN), 1);
        Player_SetupStandingStillNoMorph(this, play);

        this->stateFlags1 &= ~PLAYER_STATE1_RIDING_HORSE;
        this->actor.parent = NULL;
        D_801BDA9C = 0;

        if (CHECK_QUEST_ITEM(QUEST_SONG_EPONA) || (DREG(1) != 0)) {
            gSaveContext.save.horseData.sceneId = play->sceneId;
            gSaveContext.save.horseData.pos.x = rideActor->world.pos.x;
            gSaveContext.save.horseData.pos.y = rideActor->world.pos.y;
            gSaveContext.save.horseData.pos.z = rideActor->world.pos.z;
            gSaveContext.save.horseData.yaw = rideActor->shape.rot.y;
        }
    } else {
        if (this->mountSide < 0) {
            D_8085D708[0].flags = ANIMSFX_FLAGS(ANIMSFX_TYPE_FLOOR_LAND, 40, CONTINUE);
        } else {
            D_8085D708[0].flags = ANIMSFX_FLAGS(ANIMSFX_TYPE_FLOOR_LAND, 29, CONTINUE);
        }

        Player_PlayAnimSfx(this, D_8085D708);
    }
}

s32 func_80850734(PlayState* play, Player* this) {
    if ((this->transformation == PLAYER_STRENGTH_ZORA) && (this->windSpeed == 0.0f) &&
        (this->currentBoots < PLAYER_BOOTS_ZORA_UNDERWATER) && CHECK_BTN_ALL(sPlayerControlInput->cur.button, BTN_A)) {
        func_8083B850(play, this);
        this->stateFlags2 |= PLAYER_STATE2_DIVING;
        LinkAnimation_Change(play, &this->skelAnime, &gPlayerAnim_pz_waterroll, 2.0f / 3.0f, 4.0f,
                             Animation_GetLastFrame(&gPlayerAnim_pz_waterroll), ANIMMODE_ONCE, -6.0f);
        this->genericTimer = 5;
        this->unk_B86[0] = 0;
        this->unk_B48 = this->linearVelocity;
        this->actor.velocity.y = 0.0f;
        Player_PlaySfx(this, NA_SE_PL_ZORA_SWIM_DASH);
        return true;
    }
    return false;
}

s32 func_80850854(PlayState* play, Player* this) {
    if ((this->transformation == PLAYER_FORM_DEKU) && (this->remainingHopsCounter != 0) &&
        (gSaveContext.save.playerData.health != 0) && (sAnalogStickDistance != 0.0f)) {
        func_808373F8(play, this, 0);
        return true;
    }
    return false;
}

void Player_UpdateSwimIdle(Player* this, PlayState* play) {
    f32 swimVelocity;
    s16 swimYaw;

    this->stateFlags2 |= PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING;

    Player_LoopAnimContinuously(play, this, &gPlayerAnim_link_swimer_swim_wait);
    Player_SetVerticalWaterVelocity(this);

    if (this->genericTimer != 0) {
        this->genericTimer--;
    }

    Player_SetupZoraBarrier(this, BTN_R);

    if (CHECK_BTN_ALL(sPlayerControlInput->press.button, BTN_A)) {
        this->genericTimer = 0;
    }

    if (!func_8082DAFC(play) && !Player_SetupSubAction(play, this, D_8085D048, 1) &&
        !Player_SetupDive(play, this, sPlayerControlInput) &&
        ((this->genericTimer != 0) || !func_80850734(play, this))) {
        swimVelocity = 0.0f;
        swimYaw = this->actor.shape.rot.y;

        if (this->attentionMode > PLAYER_UNKAA5_2) {
            this->attentionMode = PLAYER_ATTENTIONMODE_NONE;
        }

        if (this->currentBoots >= PLAYER_BOOTS_ZORA_UNDERWATER) {
            if (this->actor.bgCheckFlags & 1) {
                Player_SetupReturnToStandStillSetAnim(this, D_8085BE84[PLAYER_ANIMGROUP_14][this->modelAnimType], play);
                Player_AnimSfx_PlayFloorLand(this);
            }
        } else if ((func_80850854(play, this) == 0) &&
                   (Player_GetTargetVelocityAndYaw(this, &swimVelocity, &swimYaw, 0.0f, play),
                    (swimVelocity != 0.0f))) {
            if ((ABS_ALT(BINANG_SUB(this->actor.shape.rot.y, swimYaw)) > 0x6000) &&
                !Math_StepToF(&this->linearVelocity, 0.0f, 1.0f)) {
                return;
            }

            if (Player_IsZTargetingSetupStartUnfriendly(this) || func_80847ED4(this)) {
                Player_SetupZTargetSwimming(play, this);
            } else {
                Player_SetupSwim(play, this, swimYaw);
            }
        }

        Player_UpdateSwimMovement(this, &this->linearVelocity, swimVelocity, swimYaw);
        func_80847F1C(this);
    }
}

void func_80850B18(Player* this, PlayState* play) {
    if (!Player_SetupItemCutsceneOrFirstPerson(this, play)) {
        this->stateFlags2 |= PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING;
        Player_PlaySwimAnim(play, this, NULL, this->linearVelocity);
        Player_SetVerticalWaterVelocity(this);

        if (DECR(this->genericTimer) == 0) {
            Player_SetupSwimIdle(play, this);
        }
    }
}

void func_80850BA8(Player* this) {
    this->linearVelocity = Math_CosS(this->unk_AAA) * this->unk_B48;
    this->actor.velocity.y = -Math_SinS(this->unk_AAA) * this->unk_B48;
}

void func_80850BF8(Player* this, f32 arg1) {
    f32 temp_fv0;
    s16 temp_ft0;

    Math_AsymStepToF(&this->unk_B48, arg1, 1.0f, (fabsf(this->unk_B48) * 0.01f) + 0.4f);
    temp_fv0 = Math_CosS(sPlayerControlInput->rel.stick_x * 0x10E);

    temp_ft0 = (((sPlayerControlInput->rel.stick_x >= 0) ? 1 : -1) * (1.0f - temp_fv0) * -1100.0f);
    temp_ft0 = CLAMP(temp_ft0, -0x1F40, 0x1F40);

    this->currentYaw += temp_ft0;
}

void func_80850D20(PlayState* play, Player* this) {
    func_8083F8A8(play, this, 12.0f, -1, 1.0f, 160, 20, true);
}

void func_80850D68(Player* this, PlayState* play) {
    f32 sp44;
    s16 sp42;
    s16 sp40;
    s16 sp3E;
    s16 sp3C;
    s16 sp3A;
    // s16 temp_v1_3;

    this->stateFlags2 |= PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING;

    Player_SetVerticalWaterVelocity(this);
    Player_SetupZoraBarrier(this, BTN_R);

    if (Player_SetupSubAction(play, this, D_8085D048, false)) {
        return;
    }

    if (Player_SetupDive(play, this, sPlayerControlInput)) {
        return;
    }

    if (Player_TryBonkning(play, this, &this->linearVelocity, 0.0f)) {
        return;
    }

    sp44 = 0.0f;

    if (this->genericTimer != 0) {
        if ((!func_8082DA90(play) && !CHECK_BTN_ALL(sPlayerControlInput->cur.button, BTN_A)) ||
            (this->currentBoots != PLAYER_BOOTS_ZORA_LAND)) {
            this->unk_B86[0] = 1;
        }

        if (LinkAnimation_Update(play, &this->skelAnime) && (DECR(this->genericTimer) == 0)) {
            if (this->unk_B86[0] != 0) {
                this->stateFlags3 &= ~PLAYER_STATE3_8000;
                Player_PlayAnimOnceSlowed(play, this, &gPlayerAnim_pz_swimtowait);
            } else {
                func_8082DB60(play, this, &gPlayerAnim_pz_fishswim);
            }
        } else {
            Player_GetTargetVelocityAndYaw(this, &sp44, &sp40, 0.0f, play);
            Math_ScaledStepToS(&this->currentYaw, sp40, 0x640);

            if (this->skelAnime.curFrame >= 13.0f) {
                sp44 = 12.0f;

                if (LinkAnimation_OnFrame(&this->skelAnime, 13.0f)) {
                    this->unk_B48 = 16.0f;
                }
                this->stateFlags3 |= PLAYER_STATE3_8000;
            } else {
                sp44 = 0.0f;
            }
        }

        Math_SmoothStepToS(&this->unk_B86[1], sPlayerControlInput->rel.stick_x * 0xC8, 0xA, 0x3E8, 0x64);
        Math_SmoothStepToS(&this->unk_B8E, this->unk_B86[1], IREG(40) + 1, IREG(41), IREG(42));
    } else if (this->unk_B86[0] == 0) {
        LinkAnimation_Update(play, &this->skelAnime);

        if ((!func_8082DA90(play) && !CHECK_BTN_ALL(sPlayerControlInput->cur.button, BTN_A)) ||
            (this->currentBoots != PLAYER_BOOTS_ZORA_LAND) || (this->windSpeed > 9.0f)) {
            this->stateFlags3 &= ~PLAYER_STATE3_8000;
            Player_PlayAnimOnceSlowed(play, this, &gPlayerAnim_pz_swimtowait);
            this->unk_B86[0] = 1;
        } else {
            sp44 = 9.0f;
            func_800B8F98(&this->actor, NA_SE_PL_ZORA_SWIM_LV - SFX_FLAG);
        }

        // Y
        sp3E = sPlayerControlInput->rel.stick_y * 0xC8;
        if (this->unk_B8C != 0) {
            this->unk_B8C--;
            sp3E = CLAMP_MAX(sp3E, (s16)(this->angleToFloorX - 0xFA0));
            // temp_v1_3 = this->angleToFloorX - 0xFA0;
            // if (temp_v1_3 < sp3E) {
            //     sp3E = temp_v1_3;
            // }
        }

        if ((this->unk_AAA >= -0x1555) && (this->actor.depthInWater < (this->ageProperties->unk_24 + 10.0f))) {
            // if (sp3E < 0x7D0) {
            //     sp3E = 0x7D0;
            // }
            sp3E = CLAMP_MIN(sp3E, 0x7D0);
        }
        Math_SmoothStepToS(&this->unk_AAA, sp3E, 4, 0xFA0, 0x190);

        // X
        sp42 = sPlayerControlInput->rel.stick_x * 0x64;
        if (Math_ScaledStepToS(&this->unk_B8A, sp42, 0x384) && (sp42 == 0)) {
            Math_SmoothStepToS(&this->unk_B86[1], 0, 4, 0x5DC, 0x64);
            Math_SmoothStepToS(&this->unk_B8E, this->unk_B86[1], IREG(44) + 1, IREG(45), IREG(46));
        } else {
            sp3C = this->unk_B86[1];
            sp3A = (this->unk_B8A < 0) ? -0x3A98 : 0x3A98;
            this->unk_B86[1] += this->unk_B8A;
            Math_SmoothStepToS(&this->unk_B8E, this->unk_B86[1], IREG(47) + 1, IREG(48), IREG(49));

            if ((ABS_ALT(this->unk_B8A) > 0xFA0) && ((((sp3C + this->unk_B8A) - sp3A) * (sp3C - sp3A)) <= 0)) {
                Player_PlaySfx(this, NA_SE_PL_ZORA_SWIM_ROLL);
            }
        }

        if (sPlayerYDistToFloor < 20.0f) {
            func_80850D20(play, this);
        }
    } else {
        Math_SmoothStepToS(&this->unk_B86[1], 0, 4, 0xFA0, 0x190);
        if ((this->skelAnime.curFrame <= 5.0f) || !func_80850734(play, this)) {
            if (LinkAnimation_Update(play, &this->skelAnime)) {
                Player_SetupSwimIdle(play, this);
            }
        }

        Player_ResetCylinder(this);
    }

    if ((this->unk_B8C < 8) && (this->actor.bgCheckFlags & 1)) {
        DynaPolyActor* dynaActor;

        if ((this->actor.floorBgId == BGCHECK_SCENE) ||
            ((dynaActor = DynaPoly_GetActor(&play->colCtx, this->actor.floorBgId)) == NULL) ||
            (dynaActor->actor.id != ACTOR_EN_TWIG)) {
            this->unk_AAA += (s16)((-this->angleToFloorX - this->unk_AAA) * 2);
            this->unk_B8C = 0xF;
        }

        func_80850D20(play, this);
        Player_PlaySfx(this, NA_SE_PL_BODY_BOUND);
    }

    func_80850BF8(this, sp44);
    func_80850BA8(this);
}

void Player_Swim(Player* this, PlayState* play) {
    f32 sp34;
    s16 sp32;
    s16 sp30;
    s16 var_v0;

    this->stateFlags2 |= PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING;

    Player_SetVerticalWaterVelocity(this);
    Player_SetupZoraBarrier(this, BTN_R);

    if (!Player_SetupSubAction(play, this, D_8085D048, 1) && !Player_SetupDive(play, this, sPlayerControlInput) &&
        !func_80850854(play, this)) {
        Player_PlaySwimAnim(play, this, sPlayerControlInput, this->linearVelocity);
        if (func_8082DA90(play)) {
            sp34 = this->linearVelocity;
            sp32 = this->actor.shape.rot.y;
        } else {
            Player_GetTargetVelocityAndYaw(this, &sp34, &sp32, 0.0f, play);
        }
        sp30 = this->actor.shape.rot.y - sp32;
        if (!func_80850734(play, this)) {
            if (Player_IsZTargetingSetupStartUnfriendly(this) || func_80847ED4(this)) {
                Player_SetupZTargetSwimming(play, this);
            } else {
                if ((sp34 == 0.0f) || (ABS_ALT(sp30) > 0x6000) ||
                    (this->currentBoots >= PLAYER_BOOTS_ZORA_UNDERWATER)) {
                    Player_SetupSwimIdle(play, this);
                }
            }
            Player_SetupSwimMovement(this, &this->linearVelocity, sp34, sp32);
        }
    }
}

void Player_ZTargetSwimming(Player* this, PlayState* play) {
    f32 sp2C;
    s16 sp2A;

    Player_PlaySwimAnim(play, this, sPlayerControlInput, this->linearVelocity);
    Player_SetVerticalWaterVelocity(this);
    Player_SetupZoraBarrier(this, BTN_R);

    if (!Player_SetupSubAction(play, this, D_8085D048, 1) && !Player_SetupDive(play, this, sPlayerControlInput)) {
        Player_GetTargetVelocityAndYaw(this, &sp2C, &sp2A, 0.0f, play);

        if (sp2C == 0.0f) {
            Player_SetupSwimIdle(play, this);
        } else if (!Player_IsZTargetingSetupStartUnfriendly(this) && !func_80847ED4(this)) {
            Player_SetupSwim(play, this, sp2A);
        } else {
            func_80848094(play, this, &sp2C, &sp2A);
        }

        Player_SetupSwimMovement(this, &this->linearVelocity, sp2C, sp2A);
    }
}

void func_808516B4(Player* this, PlayState* play) {
    this->stateFlags2 |= PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING;

    this->actor.gravity = 0.0f;
    Player_SetupCurrentUpperAction(this, play);
    Player_SetupZoraBarrier(this, BTN_R);

    if (Player_SetupItemCutsceneOrFirstPerson(this, play)) {
        return;
    }

    if (this->currentBoots >= PLAYER_BOOTS_ZORA_UNDERWATER) {
        Player_SetupSwimIdle(play, this);
    } else if (this->genericVar == 0) {
        f32 temp_fv0;

        if (this->genericTimer == 0) {
            if (LinkAnimation_Update(play, &this->skelAnime) ||
                ((this->skelAnime.curFrame >= 22.0f) && !CHECK_BTN_ALL(sPlayerControlInput->cur.button, BTN_A))) {
                func_8083B798(play, this);
            } else if (LinkAnimation_OnFrame(&this->skelAnime, 20.0f)) {
                this->actor.velocity.y = -2.0f;
            }
            Player_StepLinearVelocityToZero(this);
        } else {
            Player_PlaySwimAnim(play, this, sPlayerControlInput, this->actor.velocity.y);
            this->unk_AAA = 0x3E80;

            if (CHECK_BTN_ALL(sPlayerControlInput->cur.button, BTN_A) &&
                !Player_SetupGetItemOrHoldBehavior(this, play) && !(this->actor.bgCheckFlags & 1) &&
                (this->actor.depthInWater < 120.0f)) {
                func_808481CC(play, this, -2.0f);
            } else {
                this->genericVar += 1;
                Player_ChangeAnimLongMorphLoop(play, this, &gPlayerAnim_link_swimer_swim_wait);
            }
        }

        temp_fv0 = (this->actor.depthInWater - this->ageProperties->unk_30) * 0.04f;
        if (temp_fv0 < this->actor.velocity.y) {
            this->actor.velocity.y = temp_fv0;
        }
    } else if (this->genericVar == 1) {
        LinkAnimation_Update(play, &this->skelAnime);
        Player_SetVerticalWaterVelocity(this);
        if (this->unk_AAA < 0x2710) {
            this->genericVar++;
            this->genericTimer = this->actor.depthInWater;
            Player_ChangeAnimLongMorphLoop(play, this, &gPlayerAnim_link_swimer_swim);
        }
    } else if (!Player_SetupDive(play, this, sPlayerControlInput)) {
        f32 var_fv1 = (this->genericTimer * 0.018f) + 4.0f;

        if (this->stateFlags1 & PLAYER_STATE1_HOLDING_ACTOR) {
            sPlayerControlInput = NULL;
        }

        Player_PlaySwimAnim(play, this, sPlayerControlInput, fabsf(this->actor.velocity.y));
        Math_ScaledStepToS(&this->unk_AAA, -0x2710, 0x320);

        var_fv1 = CLAMP_MAX(var_fv1, 8.0f);
        func_808481CC(play, this, var_fv1);
    }
}

void func_808519FC(Player* this, PlayState* play) {
    this->stateFlags2 |= PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING;

    Player_SetupZoraBarrier(this, BTN_R);

    if (((this->stateFlags1 & PLAYER_STATE1_GETTING_ITEM) || (this->skelAnime.curFrame <= 1.0f) ||
         !func_80850734(play, this)) &&
        LinkAnimation_Update(play, &this->skelAnime)) {
        if (!(this->stateFlags1 & PLAYER_STATE1_GETTING_ITEM) || Player_SetupGetItemText(play, this)) {
            Player_EndGetItem(play, this);
            Player_SetupSwimIdle(play, this);
            Player_ResetSubCam(play, this);
        }
    } else {
        if ((this->stateFlags1 & PLAYER_STATE1_GETTING_ITEM) && LinkAnimation_OnFrame(&this->skelAnime, 10.0f)) {
            func_8082ECE0(this);
            Player_ResetSubCam(play, this);
        } else if (LinkAnimation_OnFrame(&this->skelAnime, 5.0f)) {
            Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_BREATH_DRINK);
        }
    }

    Player_SetVerticalWaterVelocity(this);
    Player_UpdateSwimMovement(this, &this->linearVelocity, 0.0f, this->actor.shape.rot.y);
}

void func_80851B58(Player* this, PlayState* play) {
    Player_SetVerticalWaterVelocity(this);
    Math_StepToF(&this->linearVelocity, 0.0f, 0.4f);
    if (LinkAnimation_Update(play, &this->skelAnime) && (this->linearVelocity < 10.0f)) {
        Player_SetupSwimIdle(play, this);
    }
}

void func_80851BD4(Player* this, PlayState* play) {
    Player_SetVerticalWaterVelocity(this);
    if (LinkAnimation_Update(play, &this->skelAnime) && (this == GET_PLAYER(play))) {
        func_80840770(play, this);
    }
    Player_UpdateSwimMovement(this, &this->linearVelocity, 0.0f, this->actor.shape.rot.y);
}

// ======== OCARINA CODE START ======== //

s32 Player_HasSongPlayed(PlayState* play, Player* this) {
    return ((play->sceneId == SCENE_MILK_BAR) && Audio_IsSequencePlaying(NA_BGM_BALLAD_OF_THE_WIND_FISH)) ||
           (((play->sceneId != SCENE_MILK_BAR) && (this->csMode == PLAYER_CSMODE_68)) ||
            ((play->msgCtx.msgMode == MSGMODE_SONG_PLAYED) ||
             (play->msgCtx.msgMode == MSGMODE_SETUP_DISPLAY_SONG_PLAYED) ||
             (play->msgCtx.msgMode == MSGMODE_DISPLAY_SONG_PLAYED) ||
             ((play->msgCtx.ocarinaMode != 1) &&
              ((this->csMode == PLAYER_CSMODE_5) || (play->msgCtx.ocarinaMode == 3) ||
               play->msgCtx.ocarinaAction == OCARINA_ACTION_FREE_PLAY_DONE))));
}

/**
 * unk_AF0[0].y -> OCARINA_BTN_A -> far right deku pipe scale
 * unk_AF0[0].z -> OCARINA_BTN_C_DOWN -> middle deku pipe scale
 * unk_AF0[1].x -> OCARINA_BTN_C_RIGHT -> far left deku pipe scale
 * unk_AF0[1].y -> OCARINA_BTN_C_LEFT -> top right deku pipe scale
 * unk_AF0[1].z -> OCARINA_BTN_C_UP -> top left deku pipe scale
 */
void Player_UpdateOcarinaDekuPipesAnims(PlayState* play, Player* this) {
    f32* dekuPipeScales = &this->unk_AF0[0].y;
    Vec3f scale;

    if (Player_HasSongPlayed(play, this)) {
        s32 i;

        // scale all deku pipes animations
        if (this->skelAnime.mode != ANIMMODE_LOOP) {
            func_8082DB60(play, this, sPlayerOcarinaPlayAnimations[this->transformation]);
        }
        func_80124618(D_801C03A0, this->skelAnime.curFrame, &scale);

        for (i = 0; i < OCARINA_BTN_MAX; i++) {
            *dekuPipeScales = scale.x;
            dekuPipeScales++;
        }
    } else if (play->msgCtx.ocarinaMode == 1) {
        if (play->msgCtx.lastOcarinaButtonIndex != OCARINA_BTN_INVALID) {
            // Scale up the deku pipe that is playing a note
            dekuPipeScales[play->msgCtx.lastOcarinaButtonIndex] = 1.2f;
            Player_PlayAnimOnceSlowed(play, this, sPlayerOcarinaPlayAnimations[this->transformation]);
        } else {
            s32 i;

            // Restore the deku pipes to normal size
            for (i = 0; i < OCARINA_BTN_MAX; i++) {
                Math_StepToF(dekuPipeScales++, 1.0f, 0.04000001f);
            }
        }
    }
}

typedef enum {
    /* 0 */ PLAYER_ARM_LEFT,
    /* 1 */ PLAYER_ARM_RIGHT,
    /* 2 */ PLAYER_ARM_MAX
} PlayerHandIndex;

void Player_InitOcarinaGoronDrumsAnims(Player* this) {
    this->unk_B86[PLAYER_ARM_LEFT] = OCARINA_BTN_NONE;
    this->unk_B86[PLAYER_ARM_RIGHT] = OCARINA_BTN_NONE;
    this->unk_B10[OCARINA_BTN_A] = 0.0f; // frame zero
}

GoronPoundDrum sPlayerGoronDrumPoundAnimations[] = {
    { PLAYER_ARM_RIGHT, &gPlayerAnim_pg_gakkiplayA }, // OCARINA_BTN_A
    { PLAYER_ARM_RIGHT, &gPlayerAnim_pg_gakkiplayL }, // OCARINA_BTN_C_DOWN
    { PLAYER_ARM_RIGHT, &gPlayerAnim_pg_gakkiplayD }, // OCARINA_BTN_C_RIGHT
    { PLAYER_ARM_LEFT, &gPlayerAnim_pg_gakkiplayU },  // OCARINA_BTN_C_LEFT
    { PLAYER_ARM_LEFT, &gPlayerAnim_pg_gakkiplayR },  // OCARINA_BTN_C_UP
};

void Player_GoronPoundDrumSetup(PlayState* play, Player* this) {
    GoronPoundDrum* poundDrum = &sPlayerGoronDrumPoundAnimations[play->msgCtx.lastOcarinaButtonIndex];
    f32* frame = &this->unk_B10[play->msgCtx.lastOcarinaButtonIndex];
    s16* lastOcarinaButtonIndex = &this->unk_B86[poundDrum->armIndex];

    *lastOcarinaButtonIndex = play->msgCtx.lastOcarinaButtonIndex;
    *frame = 3.0f;
}

void Player_GoronPoundDrumAnim(PlayState* play, Player* this) {
    GoronPoundDrum* poundDrum;
    f32* frame;
    s32 i;

    i = this->unk_B86[PLAYER_ARM_LEFT];
    if (i > OCARINA_BTN_NONE) {
        // Pound drum with left hand
        poundDrum = &sPlayerGoronDrumPoundAnimations[i];
        i = PLAYER_ARM_LEFT;
        frame = &this->unk_B10[this->unk_B86[i]];

        AnimationContext_SetLoadFrame(play, poundDrum->anim, *frame, this->skelAnime.limbCount,
                                      this->skelAnime.morphTable);
        AnimationContext_SetCopyTrue(play, this->skelAnime.limbCount, this->skelAnime.jointTable,
                                     this->skelAnime.morphTable, sLeftArmJointCopyFlags);
    }
    i = this->unk_B86[PLAYER_ARM_RIGHT];
    if (i > OCARINA_BTN_NONE) {
        // Pound drum with right hand
        poundDrum = &sPlayerGoronDrumPoundAnimations[i];
        i = PLAYER_ARM_RIGHT;
        frame = &this->unk_B10[this->unk_B86[i]];

        AnimationContext_SetLoadFrame(play, poundDrum->anim, *frame, this->skelAnime.limbCount,
                                      ALIGN16((uintptr_t)this->blendTableBuffer));
        AnimationContext_SetCopyTrue(play, this->skelAnime.limbCount, this->skelAnime.jointTable,
                                     ALIGN16((uintptr_t)this->blendTableBuffer), sRightArmJointCopyFlags);
    }

    frame = &this->unk_B10[0];

    // Loop over all drums
    for (i = 0; i < OCARINA_BTN_MAX; i++) {
        // Next frame of animation
        *frame += 1.0f;
        if (*frame >= 9.0f) {
            // End of animation
            *frame = 8.0f;
            if (this->unk_B86[PLAYER_ARM_LEFT] == i) {
                this->unk_B86[PLAYER_ARM_LEFT] = OCARINA_BTN_NONE;
            } else if (this->unk_B86[PLAYER_ARM_RIGHT] == i) {
                this->unk_B86[PLAYER_ARM_RIGHT] = OCARINA_BTN_NONE;
            }
        }
        // next drum
        frame++;
    }
}

// Goron playing the drums?
void Player_UpdateOcarinaGoronDrumsAnims(PlayState* play, Player* this) {
    if (Player_HasSongPlayed(play, this)) {
        if (this->skelAnime.animation != &gPlayerAnim_pg_gakkiplay) {
            func_8082DB60(play, this, &gPlayerAnim_pg_gakkiplay);
        }

        func_80124618(D_801C0490, this->skelAnime.curFrame, &this->unk_AF0[1]);
    } else if (play->msgCtx.ocarinaMode == 1) {
        if (play->msgCtx.lastOcarinaButtonIndex != OCARINA_BTN_INVALID) {
            Player_GoronPoundDrumSetup(play, this);
        }

        Player_GoronPoundDrumAnim(play, this);
    }
}

// Zora playing the guitar?
void Player_UpdateOcarinaZoraGuitarAnims(PlayState* play, Player* this) {
    if (Player_HasSongPlayed(play, this)) {
        if (this->skelAnime.mode != ANIMMODE_LOOP) {
            func_8082DB60(play, this, sPlayerOcarinaPlayAnimations[this->transformation]);
        }

        // Left hand jiggle timer
        this->unk_B8A = 8;
    } else {
        f32 inputDist;
        s16 var_a1_3;
        s16 inputAngle;

        if ((play->msgCtx.ocarinaMode == 1) && (play->msgCtx.lastOcarinaButtonIndex != OCARINA_BTN_INVALID)) {
            if ((this->ocarinaActor != NULL) && (this->ocarinaActorXZDist < 0.0f)) {
                this->ocarinaActor->flags |= ACTOR_FLAG_OCARINA_READY;
                this->ocarinaActorXZDist = 0.0f;
            }

            // Apply a strum
            Player_PlayAnimOnceSlowed(play, this, sPlayerOcarinaPlayAnimations[this->transformation]);
            // Left hand jiggle timer
            this->unk_B8A = 8;
        }

        sPlayerControlInput = play->state.input;
        func_800FF3A0(&inputDist, &inputAngle, sPlayerControlInput);

        // Adjust input values
        if (BINANG_ADD(inputAngle, 0x4000) < 0) {
            inputAngle -= 0x8000;
            inputDist = -inputDist;
        }

        // Clamp input angle
        if (inputAngle < -0x1F40) {
            inputAngle = -0x1F40;
        } else if (inputAngle > 0x2EE0) {
            inputAngle = 0x2EE0;
        }

        // Rotate upper body based on input
        var_a1_3 = (inputDist * -100.0f);
        var_a1_3 = CLAMP_MAX(var_a1_3, 0xFA0);
        Math_SmoothStepToS(&this->upperBodyRot.x, var_a1_3, 4, 0x7D0, 0);
        Math_SmoothStepToS(&this->upperBodyRot.y, inputAngle, 4, 0x7D0, 0);

        // Move chin up/down
        this->headRot.x = -this->upperBodyRot.x;

        // Flags to ensure rot adjustments above are not 0'd out
        this->lookFlags |= (0x8 | 0x40 | 0x80);

        // Change face expression
        var_a1_3 = ABS_ALT(this->upperBodyRot.x);
        if (var_a1_3 < 0x7D0) {
            this->actor.shape.face = 0;
        } else if (var_a1_3 < 0xFA0) {
            this->actor.shape.face = 13;
        } else {
            this->actor.shape.face = 8;
        }
    }

    if (DECR(this->unk_B8A) != 0) {
        // Jiggle Zora's left hand
        this->unk_B86[0] += (s16)(this->upperBodyRot.x * 2.5f);
        this->unk_B86[1] += (s16)(this->upperBodyRot.y * 3.0f);
    } else {
        // Stop the jiggling
        this->unk_B86[0] = 0;
        this->unk_B86[1] = 0;
    }
}

void Player_UpdateOcarinaAnims(PlayState* play, Player* this) {
    if (this->transformation == PLAYER_FORM_DEKU) {
        Player_UpdateOcarinaDekuPipesAnims(play, this);
    } else if (this->transformation == PLAYER_FORM_GORON) {
        Player_UpdateOcarinaGoronDrumsAnims(play, this);
    } else if (this->transformation == PLAYER_FORM_ZORA) {
        Player_UpdateOcarinaZoraGuitarAnims(play, this);
    }
}

void Player_InitOcarinaAnims(PlayState* play, Player* this) {
    if (this->genericTimer++ >= 3) {
        if ((this->transformation == PLAYER_FORM_ZORA) || (this->transformation == PLAYER_FORM_DEKU)) {
            Player_ChangeAnimOnce(play, this, sPlayerOcarinaPlayAnimations[this->transformation]);
        } else if (this->transformation == PLAYER_FORM_GORON) {
            Player_InitOcarinaGoronDrumsAnims(this);
            func_8082DB60(play, this, &gPlayerAnim_pg_gakkiwait);
        } else {
            func_8082DB60(play, this, sPlayerOcarinaPlayAnimations[this->transformation]);
        }

        this->unk_B48 = 1.0f;
    }
}

void Player_PlayOcarina(Player* this, PlayState* play) {
    if ((this->attentionMode != PLAYER_ATTENTIONMODE_CUTSCENE) &&
        ((LinkAnimation_Update(play, &this->skelAnime) &&
          (this->skelAnime.animation == sPlayerOcarinaStartAnimations[this->transformation])) ||
         ((this->skelAnime.mode == 0) && (this->genericTimer == 0)))) {
        Player_InitOcarinaAnims(play, this);
        if (!(this->actor.flags & ACTOR_FLAG_OCARINA_READY) || (this->ocarinaActor->id == ACTOR_EN_ZOT)) {
            // Message_DisplayOcarinaStaff
            func_80152434(play, OCARINA_ACTION_FREE_PLAY);
        }
    } else if (this->genericTimer != 0) {
        if (play->msgCtx.ocarinaMode == 4) {
            play->interfaceCtx.unk_222 = 0;
            ActorCutscene_Stop(play->playerActorCsIds[0]);
            this->actor.flags &= ~ACTOR_FLAG_OCARINA_READY;

            if ((this->talkActor != NULL) && (this->talkActor == this->ocarinaActor) &&
                (this->ocarinaActorXZDist >= 0.0f)) {
                Player_TalkWithPlayer(play, this->talkActor);
            } else if (this->tatlTextId < 0) {
                this->talkActor = this->tatlActor;
                this->tatlActor->textId = -this->tatlTextId;
                Player_TalkWithPlayer(play, this->talkActor);
            } else if (!Player_SetupItemCutsceneOrFirstPerson(this, play)) {
                Player_SetupReturnToStandStill(this, play);
                Player_AnimationPlayOnceReverse(play, this, sPlayerOcarinaStartAnimations[this->transformation]);
            }
        } else {
            s32 isWarping = (play->msgCtx.ocarinaMode >= OCARINA_MODE_WARP_TO_GREAT_BAY_COAST) &&
                            (play->msgCtx.ocarinaMode <= OCARINA_MODE_WARP_TO_ENTRANCE);
            s32 pad[2];

            if (isWarping || (play->msgCtx.ocarinaMode == OCARINA_MODE_APPLY_SOT) ||
                (play->msgCtx.ocarinaMode == OCARINA_MODE_APPLY_DOUBLE_SOT) ||
                (play->msgCtx.ocarinaMode == OCARINA_MODE_APPLY_INV_SOT_FAST) ||
                (play->msgCtx.ocarinaMode == OCARINA_MODE_APPLY_INV_SOT_SLOW)) {
                if (play->msgCtx.ocarinaMode == OCARINA_MODE_APPLY_SOT) {
                    if (!func_8082DA90(play)) {
                        // Dawn of a new day
                        if (gSaveContext.save.playerData.deaths == 1) {
                            // First SoT Cutscene, replay snippets of first cycle
                            play->nextEntrance = ENTRANCE(CUTSCENE, 1);
                        } else {
                            // Repeated SoT cutscenes, jumpt straight to Dawn Of...
                            play->nextEntrance = ENTRANCE(CUTSCENE, 0);
                        }

                        gSaveContext.nextCutsceneIndex = 0xFFF7;
                        play->transitionTrigger = TRANS_TRIGGER_START;
                    }
                } else {
                    // Song of soaring or time song effects
                    Actor* actor;

                    play->interfaceCtx.unk_222 = 0;
                    ActorCutscene_Stop(play->playerActorCsIds[0]);
                    this->actor.flags &= ~ACTOR_FLAG_OCARINA_READY;

                    actor = Actor_Spawn(&play->actorCtx, play, isWarping ? ACTOR_EN_TEST7 : ACTOR_EN_TEST6,
                                        this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z, 0, 0,
                                        0, play->msgCtx.ocarinaMode);
                    if (actor != NULL) {
                        this->stateFlags1 &= ~PLAYER_STATE1_IN_CUTSCENE;
                        this->csMode = PLAYER_CSMODE_0;
                        Player_SetupPlayerCutscene(play, NULL, PLAYER_CSMODE_19);
                        this->stateFlags1 |= PLAYER_STATE1_SKIP_OTHER_ACTORS_UPDATE | PLAYER_STATE1_IN_CUTSCENE;
                    } else {
                        Player_SetupReturnToStandStill(this, play);
                        Player_AnimationPlayOnceReverse(play, this,
                                                        sPlayerOcarinaStartAnimations[this->transformation]);
                    }
                }
            } else if ((play->msgCtx.ocarinaMode == 3) && (play->msgCtx.lastPlayedSong == OCARINA_SONG_ELEGY)) {
                // Elegy of emptiness
                play->interfaceCtx.unk_222 = 0;
                ActorCutscene_Stop(play->playerActorCsIds[0]);

                this->actor.flags &= ~ACTOR_FLAG_OCARINA_READY;
                func_80831760(play, this, Player_SetupElegyShell, 0);
                this->stateFlags1 |= PLAYER_STATE1_SKIP_OTHER_ACTORS_UPDATE | PLAYER_STATE1_IN_CUTSCENE;
            } else if (this->attentionMode == PLAYER_ATTENTIONMODE_CUTSCENE) {
                f32 temp_fa0 = this->skelAnime.jointTable[PLAYER_LIMB_ROOT - 1].x;
                f32 temp_fa1 = this->skelAnime.jointTable[PLAYER_LIMB_ROOT - 1].z;
                f32 var_fv1;

                var_fv1 = sqrtf(SQ(temp_fa0) + SQ(temp_fa1));
                if (var_fv1 != 0.0f) {
                    var_fv1 = (var_fv1 - 100.0f) / var_fv1;
                    var_fv1 = CLAMP_MIN(var_fv1, 0.0f);
                }

                this->skelAnime.jointTable[PLAYER_LIMB_ROOT - 1].x = temp_fa0 * var_fv1;
                this->skelAnime.jointTable[PLAYER_LIMB_ROOT - 1].z = temp_fa1 * var_fv1;
            } else {
                Player_UpdateOcarinaAnims(play, this);
            }
        }
    }
}

// ======== OCARINA CODE END ======== //

void Player_ThrowDekuNut(Player* this, PlayState* play) {
    Player_StepLinearVelocityToZero(this);

    if (LinkAnimation_Update(play, &this->skelAnime)) {
        Player_SetupReturnToStandStillSetAnim(this, &gPlayerAnim_link_normal_light_bom_end, play);
    } else if (LinkAnimation_OnFrame(&this->skelAnime, 3.0f)) {
        if (Actor_Spawn(&play->actorCtx, play, ACTOR_EN_ARROW, this->bodyPartsPos[PLAYER_BODYPART_R_HAND].x,
                        this->bodyPartsPos[PLAYER_BODYPART_R_HAND].y, this->bodyPartsPos[PLAYER_BODYPART_R_HAND].z,
                        4000, this->actor.shape.rot.y, 0, 8) != NULL) {
            Inventory_ChangeAmmo(ITEM_NUT, -1);
            this->fpsItemShotTimer = 4;
        }

        Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_SWORD_N);
    }
}

AnimSfxEntry D_8085D73C[] = {
    ANIMSFX(ANIMSFX_TYPE_FLOOR_JUMP, 87, 0, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_VOICE, 87, NA_SE_VO_LI_CLIMB_END, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_VOICE, 69, NA_SE_VO_LI_AUTO_JUMP, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_FLOOR_LAND, 123, 0, STOP),
};

AnimSfxEntry D_8085D74C[] = {
    ANIMSFX(ANIMSFX_TYPE_VOICE, 13, NA_SE_VO_LI_AUTO_JUMP, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_FLOOR_JUMP, 13, 0, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_FLOOR_LAND, 73, 0, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_FLOOR_LAND, 120, 0, STOP),
};

void Player_GetItem(Player* this, PlayState* play) {
    func_8083249C(this);

    if (LinkAnimation_Update(play, &this->skelAnime)) {
        if (this->genericTimer != 0) {
            if (this->genericTimer > 1) {
                this->genericTimer--;
            }

            if (Player_SetupGetItemText(play, this) && (this->genericTimer == 1)) {
                Player_SetModels(this, Player_ActionToModelGroup(this, this->heldItemActionParam));

                if ((this->getItemDrawId == GID_POWDER_KEG) || (this->getItemDrawId == GID_REMAINS_GYORG) ||
                    (this->getItemDrawId == GID_REMAINS_TWINMOLD) || (this->getItemDrawId == GID_SWORD_RAZOR)) {
                    func_80838760(this);
                    Player_EndGetItem(play, this);
                    this->stateFlags1 &= ~PLAYER_STATE1_IN_CUTSCENE;
                    Player_SetupPlayerCutscene(play, NULL, PLAYER_CSMODE_93);
                } else {
                    s32 var_a2 = ((this->talkActor != NULL) && (this->exchangeItemId < PLAYER_AP_NONE)) ||
                                 (this->stateFlags3 & PLAYER_STATE3_OCARINA_AFTER_TEXTBOX);

                    if (var_a2 || (gSaveContext.healthAccumulator == 0)) {
                        func_80838760(this);
                        if (var_a2) {
                            Player_EndGetItem(play, this);
                            this->exchangeItemId = PLAYER_AP_NONE;
                            if (!Player_ForceOcarina(play, this)) {
                                Player_TalkWithPlayer(play, this->talkActor);
                            }
                        } else {
                            Player_SetupEndGetItem(play, this);
                        }
                    }
                }
            }
        } else {
            Player_EndAnimMovement(this);

            if ((this->getItemId == GI_STRAY_FAIRY) || (this->getItemId == GI_SKULL_TOKEN) ||
                (this->getItemId == GI_ICE_TRAP)) {
                func_80838760(this);
                this->stateFlags1 &= ~(PLAYER_STATE1_GETTING_ITEM | PLAYER_STATE1_HOLDING_ACTOR);
                if (this->getItemId == GI_STRAY_FAIRY) {
                    Player_SetupStandingStillNoMorph(this, play);
                } else {
                    this->actor.colChkInfo.damage = 0;
                    func_80833B18(play, this, 3, 0.0f, 0.0f, 0, 20);
                }
            } else {
                if (this->skelAnime.animation == &gPlayerAnim_link_normal_box_kick) {
                    Player_PlayAnimOnceSlowed(play, this,
                                              (this->transformation == PLAYER_FORM_DEKU)
                                                  ? &gPlayerAnim_pn_getB
                                                  : &gPlayerAnim_link_demo_get_itemB);
                } else {
                    Player_PlayAnimOnceSlowed(play, this,
                                              (this->transformation == PLAYER_FORM_DEKU)
                                                  ? &gPlayerAnim_pn_getA
                                                  : &gPlayerAnim_link_demo_get_itemA);
                }

                Player_SetupAnimMovement(play, this, (1 | ANIM_FLAG_UPDATE_Y | 4 | 8 | ANIM_FLAG_NOMOVE | 0x80));
                func_80838760(this);
                this->unk_A86 = play->playerActorCsIds[1];
                this->genericTimer = 2;
            }
        }
    } else if (this->genericTimer == 0) {
        if (this->transformation == PLAYER_FORM_HUMAN) {
            Player_PlayAnimSfx(this, D_8085D73C);
        } else if (this->transformation == PLAYER_FORM_DEKU) {
            Player_PlayAnimSfx(this, D_8085D74C);
        }
    } else {
        if ((this->skelAnime.animation == &gPlayerAnim_link_demo_get_itemB) ||
            (this->skelAnime.animation == &gPlayerAnim_pn_getB)) {
            Math_ScaledStepToS(&this->actor.shape.rot.y,
                               BINANG_ADD(Camera_GetCamDirYaw(play->cameraPtrs[play->activeCamId]), 0x8000), 0xFA0);
        } else if ((this->skelAnime.animation == &gPlayerAnim_pn_getA) &&
                   LinkAnimation_OnFrame(&this->skelAnime, 10.0f)) {
            Player_AnimSfx_PlayFloorLand(this);
        }

        if (LinkAnimation_OnFrame(&this->skelAnime, 21.0f)) {
            func_8082ECE0(this);
        }
    }
}

AnimSfxEntry D_8085D75C[] = {
    ANIMSFX(ANIMSFX_TYPE_VOICE, 5, NA_SE_VO_LI_AUTO_JUMP, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_FLOOR_LAND, 15, 0, STOP),
};

void func_80852FD4(Player* this, PlayState* play) {
    if (LinkAnimation_Update(play, &this->skelAnime)) {
        if (this->genericVar == 0) {
            if (DECR(this->genericTimer) == 0) {
                this->genericVar = 1;
                this->skelAnime.endFrame = this->skelAnime.animLength - 1.0f;
            }
        } else {
            Player_SetupStandingStillNoMorph(this, play);
        }
    } else if ((this->transformation == PLAYER_FORM_FIERCE_DEITY) && LinkAnimation_OnFrame(&this->skelAnime, 158.0f)) {
        Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_SWORD_N);
    } else if (this->transformation != PLAYER_FORM_FIERCE_DEITY) {
        Player_PlayAnimSfx(this, D_8085D75C);
    } else {
        func_808484CC(this);
    }
}

Vec3f D_8085D764 = { 0.0f, 24.0f, 19.0f };
Vec3f D_8085D770 = { 0.0f, 0.0f, 2.0f };
Vec3f D_8085D77C = { 0.0f, 0.0f, -0.2f };

Color_RGBA8 D_8085D788 = { 255, 255, 255, 255 };
Color_RGBA8 D_8085D78C = { 255, 255, 255, 255 };

void func_808530E0(PlayState* play, Player* this) {
    Vec3f pos;
    Vec3f velocity;
    Vec3f accel;

    Player_TranslateAndRotateY(this, &this->actor.world.pos, &D_8085D764, &pos);
    Player_TranslateAndRotateY(this, &gZeroVec3f, &D_8085D770, &velocity);
    Player_TranslateAndRotateY(this, &gZeroVec3f, &D_8085D77C, &accel);
    func_800B0EB0(play, &pos, &velocity, &accel, &D_8085D788, &D_8085D78C, 40, 10, 10);
}

u8 D_8085D790[] = {
    1,     // PLAYER_AP_BOTTLE_POTION_RED
    1 | 2, // PLAYER_AP_BOTTLE_POTION_BLUE
    2,     // PLAYER_AP_BOTTLE_POTION_GREEN
    4,     // PLAYER_AP_BOTTLE_MILK
    4,     // PLAYER_AP_BOTTLE_MILK_HALF
    1 | 2, // PLAYER_AP_BOTTLE_CHATEAU
};

void func_80853194(Player* this, PlayState* play) {
    func_808323C0(this, play->playerActorCsIds[2]);

    if (LinkAnimation_Update(play, &this->skelAnime)) {
        if (this->genericTimer == 0) {
            if (this->heldItemActionParam == PLAYER_AP_BOTTLE_POE) {
                s32 health = Rand_S16Offset(-1, 3);

                if (health == 0) {
                    health = 3;
                }
                if ((health < 0) && (gSaveContext.save.playerData.health <= 0x10)) {
                    health = 3;
                }

                if (health < 0) {
                    Health_ChangeBy(play, -0x10);
                } else {
                    gSaveContext.healthAccumulator = health * 0x10;
                }
            } else {
                s32 temp_v1 = D_8085D790[this->heldItemActionParam - PLAYER_AP_BOTTLE_POTION_RED];

                if (temp_v1 & 1) {
                    gSaveContext.healthAccumulator = 0x140;
                }
                if (temp_v1 & 2) {
                    Magic_Add(play, MAGIC_FILL_TO_CAPACITY);
                }
                if (temp_v1 & 4) {
                    gSaveContext.healthAccumulator = 0x50;
                }

                if (this->heldItemActionParam == PLAYER_AP_BOTTLE_CHATEAU) {
                    gSaveContext.save.weekEventReg[0xE] |= 8;
                }

                gSaveContext.jinxTimer = 0;
            }

            func_8082DB60(play, this,
                          (this->transformation == PLAYER_FORM_DEKU) ? &gPlayerAnim_pn_drink
                                                                     : &gPlayerAnim_link_bottle_drink_demo_wait);
            this->genericTimer = 1;

        //! FAKE
        dummy_label_235515:;
        } else if (this->genericTimer < 0) {
            this->genericTimer++;
            if (this->genericTimer == 0) {
                this->genericTimer = 3;
                this->skelAnime.endFrame = this->skelAnime.animLength - 1.0f;
            } else if (this->genericTimer == -6) {
                func_808530E0(play, this);
            }
        } else {
            func_80838760(this);
            Player_SetupStandingStillNoMorph(this, play);
        }
    } else if (this->genericTimer == 1) {
        if ((gSaveContext.healthAccumulator == 0) && (gSaveContext.magicState != 9)) {
            if (this->transformation == PLAYER_FORM_DEKU) {
                LinkAnimation_Change(play, &this->skelAnime, &gPlayerAnim_pn_drinkend, 2.0f / 3.0f, 0.0f, 5.0f, 2,
                                     -6.0f);
                this->genericTimer = -7;
            } else {
                Player_ChangeAnimSlowedMorphToLastFrame(play, this, &gPlayerAnim_link_bottle_drink_demo_end);
                this->genericTimer = 2;
            }

            Player_UpdateBottleHeld(play, this,
                                    (this->heldItemActionParam == PLAYER_AP_BOTTLE_MILK) ? ITEM_MILK_HALF : ITEM_BOTTLE,
                                    PLAYER_AP_BOTTLE);
        }

        Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_DRINK - SFX_FLAG);
    } else if ((this->genericTimer == 2) && LinkAnimation_OnFrame(&this->skelAnime, 29.0f)) {
        Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_BREATH_DRINK);
    }
}

BottleCatchInfo sBottleCatchInfos[] = {
    { ACTOR_EN_ELF, 2, ITEM_FAIRY, PLAYER_AP_BOTTLE_FAIRY, 0x5E },
    { ACTOR_EN_FISH, -1, ITEM_FISH, PLAYER_AP_BOTTLE_FISH, 0x62 },
    { ACTOR_EN_INSECT, -1, ITEM_BUG, PLAYER_AP_BOTTLE_BUG, 0x63 },
    { ACTOR_EN_MUSHI2, -1, ITEM_BUG, PLAYER_AP_BOTTLE_BUG, 0x63 },
    { ACTOR_EN_TEST5, 0, ITEM_SPRING_WATER, PLAYER_AP_BOTTLE_SPRING_WATER, 0x67 },
    { ACTOR_EN_TEST5, 1, ITEM_HOT_SPRING_WATER, PLAYER_AP_BOTTLE_HOT_SPRING_WATER, 0x68 },
    { ACTOR_BG_GORON_OYU, -1, ITEM_HOT_SPRING_WATER, PLAYER_AP_BOTTLE_HOT_SPRING_WATER, 0x68 },
    { ACTOR_EN_ZORAEGG, -1, ITEM_ZORA_EGG, PLAYER_AP_BOTTLE_ZORA_EGG, 0x69 },
    { ACTOR_EN_DNP, -1, ITEM_DEKU_PRINCESS, PLAYER_AP_BOTTLE_DEKU_PRINCESS, 0x5F },
    { ACTOR_EN_OT, -1, ITEM_SEAHORSE, PLAYER_AP_BOTTLE_SEAHORSE, 0x6E },
    { ACTOR_OBJ_KINOKO, -1, ITEM_MUSHROOM, PLAYER_AP_BOTTLE_SEAHORSE, 0x6B },
    { ACTOR_EN_POH, -1, ITEM_POE, PLAYER_AP_BOTTLE_POE, 0x65 },
    { ACTOR_EN_BIGPO, -1, ITEM_BIG_POE, PLAYER_AP_BOTTLE_BIG_POE, 0x66 },
    { ACTOR_EN_ELF, 6, ITEM_FAIRY, PLAYER_AP_BOTTLE_FAIRY, 0x5E },
};

void Player_SwingBottle(Player* this, PlayState* play) {
    BottleSwingAnimInfo* bottleSwingAnims = &sBottleSwingAnims[this->genericTimer];

    Player_StepLinearVelocityToZero(this);

    if (LinkAnimation_Update(play, &this->skelAnime)) {
        if (this->genericVar != 0) {
            func_808323C0(this, play->playerActorCsIds[3]);

            if (this->genericTimer == 0) {
                Message_StartTextbox(play, sBottleCatchInfos[this->genericVar - 1].textId, &this->actor);

                Audio_PlayFanfare(NA_BGM_GET_ITEM | 0x900);
                this->genericTimer = 1;
            } else if (Message_GetState(&play->msgCtx) == TEXT_STATE_CLOSING) {
                Actor* talkActor;

                this->genericVar = 0;
                func_80838760(this);
                func_800E0238(Play_GetCamera(play, CAM_ID_MAIN));

                talkActor = this->talkActor;
                if ((talkActor != NULL) && (this->exchangeItemId < PLAYER_AP_NONE)) {
                    Player_TalkWithPlayer(play, talkActor);
                }
            }
        } else {
            Player_SetupStandingStillNoMorph(this, play);
        }
    } else {
        if (this->genericVar == 0) {
            s32 temp_ft5 = this->skelAnime.curFrame - bottleSwingAnims->unk_8;

            if ((temp_ft5 >= 0) && ((s32)bottleSwingAnims->unk_9 >= temp_ft5)) {
                if ((this->genericTimer != 0) && (temp_ft5 == 0)) {
                    Player_PlaySfx(this, NA_SE_IT_SCOOP_UP_WATER);
                }

                if (func_8012364C(play, this, this->heldItemButton) == ITEM_BOTTLE) {
                    Actor* interactRangeActor = this->interactRangeActor;

                    if (interactRangeActor != NULL) {
                        BottleCatchInfo* catchInfo = sBottleCatchInfos;
                        s32 i;

                        for (i = 0; i < ARRAY_COUNT(sBottleCatchInfos); i++) {
                            if (((interactRangeActor->id == catchInfo->actorId) &&
                                 ((catchInfo->actorParams < 0) ||
                                  (interactRangeActor->params == catchInfo->actorParams)))) {
                                break;
                            }
                            catchInfo++;
                        }

                        if (i < ARRAY_COUNT(sBottleCatchInfos)) {
                            this->genericVar = i + 1;
                            this->genericTimer = 0;
                            this->stateFlags1 |= PLAYER_STATE1_SKIP_OTHER_ACTORS_UPDATE | PLAYER_STATE1_IN_CUTSCENE;
                            interactRangeActor->parent = &this->actor;
                            Player_UpdateBottleHeld(play, this, catchInfo->itemId, catchInfo->actionParam);
                            Player_PlayAnimOnceSlowed(play, this, bottleSwingAnims->bottleCatchAnim);
                        }
                    }
                }
            }
        }

        if (this->skelAnime.curFrame <= 7.0f) {
            this->stateFlags3 |= PLAYER_STATE3_SWINGING_BOTTLE;
        }
    }
}

Vec3f sBottleFairyPosOffset = { 0.0f, 0.0f, 5.0f };

void Player_HealWithFairy(Player* this, PlayState* play) {
    func_808323C0(this, play->playerActorCsIds[2]);

    if (LinkAnimation_Update(play, &this->skelAnime)) {
        func_80838760(this);
        Player_SetupStandingStillNoMorph(this, play);
    } else if (LinkAnimation_OnFrame(&this->skelAnime, 37.0f)) {
        s32 sp2C = 8;

        Player_PlaySfx(this, NA_SE_EV_BOTTLE_CAP_OPEN);
        Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_AUTO_JUMP);
        if (this->heldItemActionParam == PLAYER_AP_BOTTLE_FAIRY) {
            Player_UpdateBottleHeld(play, this, ITEM_BOTTLE, PLAYER_AP_BOTTLE);
            Player_PlaySfx(this, NA_SE_EV_FIATY_HEAL - SFX_FLAG);
            sp2C = 1;
        }

        Player_SpawnFairy(play, this, &this->leftHandWorld.pos, &sBottleFairyPosOffset, sp2C);
    }
}

void Player_DropItemFromBottle(Player* this, PlayState* play) {
    static Vec3f D_8085D7F8 = { 10.0f, 26.800001f, 30.0f };
    static s8 D_8085D804[PLAYER_FORM_MAX] = { 0x2D, 0x4B, 0x37, 0x23, 0x28 };
    static BottleDropInfo sBottleDropInfo[] = {
        { ACTOR_EN_FISH, 0 },       // PLAYER_AP_BOTTLE_FISH
        { ACTOR_OBJ_AQUA, 0 },      // PLAYER_AP_BOTTLE_SPRING_WATER
        { ACTOR_OBJ_AQUA, 1 },      // PLAYER_AP_BOTTLE_HOT_SPRING_WATER
        { ACTOR_EN_ZORAEGG, 0x11 }, // PLAYER_AP_BOTTLE_ZORA_EGG
        { ACTOR_EN_DNP, 1 },        // PLAYER_AP_BOTTLE_DEKU_PRINCESS
        { ACTOR_EN_MUSHI2, 0 },     // PLAYER_AP_BOTTLE_GOLD_DUST
        { ACTOR_EN_MUSHI2, 0 },     // PLAYER_AP_BOTTLE_1C
        { ACTOR_EN_OT, 0x8000 },    // PLAYER_AP_BOTTLE_SEAHORSE
        { ACTOR_EN_MUSHI2, 0 },     // PLAYER_AP_BOTTLE_MUSHROOM
        { ACTOR_EN_MUSHI2, 0 },     // PLAYER_AP_BOTTLE_HYLIAN_LOACH
        { ACTOR_EN_MUSHI2, 0 },     // PLAYER_AP_BOTTLE_BUG
    };
    static AnimSfxEntry sBottleDropAnimSfx[] = {
        ANIMSFX(ANIMSFX_TYPE_VOICE, 38, NA_SE_VO_LI_AUTO_JUMP, CONTINUE),
        ANIMSFX(ANIMSFX_TYPE_GENERAL, 40, NA_SE_EV_BOTTLE_CAP_OPEN, STOP),
    };

    CollisionPoly* sp6C;
    s32 sp68;
    Vec3f sp5C;
    f32 temp_fa0;
    f32 temp_fv0;
    f32 temp_fv1;
    BottleDropInfo* dropInfo;

    D_8085D7F8.z = D_8085D804[this->transformation];
    if (Player_WallLineTestWithOffset(play, this, &D_8085D7F8, &sp6C, &sp68, &sp5C)) {
        temp_fv1 = this->actor.world.pos.x - sp5C.x;
        temp_fa0 = this->actor.world.pos.z - sp5C.z;
        temp_fv0 = sqrtf(SQ(temp_fv1) + SQ(temp_fa0));

        if (temp_fv0 != 0.0f) {
            temp_fv0 = 3.0f / temp_fv0;

            this->actor.world.pos.x = this->actor.world.pos.x + (temp_fv1 * temp_fv0);
            this->actor.world.pos.z = this->actor.world.pos.z + (temp_fa0 * temp_fv0);
        }
    }

    Player_StepLinearVelocityToZero(this);
    func_8083249C(this);

    if (LinkAnimation_Update(play, &this->skelAnime)) {
        func_80838760(this);
        if (!Player_SetupItemCutsceneOrFirstPerson(this, play)) {
            Player_SetupStandingStillNoMorph(this, play);
        }
    } else if (LinkAnimation_OnFrame(&this->skelAnime, 76.0f)) {
        dropInfo = &sBottleDropInfo[this->heldItemActionParam - PLAYER_AP_BOTTLE - 1];

        Actor_Spawn(&play->actorCtx, play, dropInfo->actorId,
                    (Math_SinS(this->actor.shape.rot.y) * 5.0f) + this->leftHandWorld.pos.x, this->leftHandWorld.pos.y,
                    (Math_CosS(this->actor.shape.rot.y) * 5.0f) + this->leftHandWorld.pos.z, 0x4000,
                    this->actor.shape.rot.y, 0, dropInfo->params);
        Player_UpdateBottleHeld(play, this, ITEM_BOTTLE, PLAYER_AP_BOTTLE);
    } else {
        Player_PlayAnimSfx(this, sBottleDropAnimSfx);
    }
}

AnimSfxEntry D_8085D840[] = {
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 30, NA_SE_PL_PUT_OUT_ITEM, STOP),
};

void Player_PresentExchangeItem(Player* this, PlayState* play) {
    this->stateFlags2 |= PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING;
    this->stateFlags3 |= PLAYER_STATE3_4000000;

    func_8083249C(this);

    if (LinkAnimation_Update(play, &this->skelAnime)) {
        if (this->exchangeItemId == PLAYER_AP_NONE) {
            Actor* talkActor = this->talkActor;

            func_80838760(this);
            this->getItemDrawId = GID_BOTTLE;

            if ((talkActor->textId != 0) && (talkActor->textId != 0xFFFF)) {
                this->actor.flags |= ACTOR_FLAG_100;
            }
            Player_TalkWithPlayer(play, talkActor);
        } else {
            GetItemEntry* giEntry = &sGetItemTable[D_8085D1A4[this->exchangeItemId] - 1];

            if (Player_ActionToBottle(this, this->heldItemActionParam) < 0) {
                this->getItemDrawId = ABS_ALT(giEntry->gid);
            }

            if (this->genericTimer == 0) {
                if ((this->actor.textId != 0) && (this->actor.textId != 0xFFFF)) {
                    Message_StartTextbox(play, this->actor.textId, &this->actor);
                }

                this->genericTimer = 1;
            } else if (Message_GetState(&play->msgCtx) == TEXT_STATE_CLOSING) {
                func_80838760(this);
                this->getItemDrawId = GID_BOTTLE;
                this->actor.flags &= ~ACTOR_FLAG_100;
                Player_SetupStandingStillNoMorph(this, play);
                this->unk_B5E = 0xA;
            }
        }
    } else if (this->genericTimer >= 0) {
        if ((Player_ActionToBottle(this, this->heldItemActionParam) >= 0) &&
            LinkAnimation_OnFrame(&this->skelAnime, 36.0f)) {
            Player_SetModels(this, PLAYER_MODELGROUP_BOTTLE);
        } else if (LinkAnimation_OnFrame(&this->skelAnime, 2.0f)) {
            GetItemEntry* giEntry = &sGetItemTable[D_8085D1A4[this->heldItemActionParam] - 1];

            Player_LoadGetItemObject(this, giEntry->objectId);
        }
        Player_PlayAnimSfx(this, D_8085D840);
    }

    if ((this->genericVar == 0) && (this->targetedActor != NULL)) {
        this->currentYaw = Player_LookAtTargetActor(this, 0);
        this->actor.shape.rot.y = this->currentYaw;
    }
}

void Player_RestrainedByEnemy(Player* this, PlayState* play) {
    this->stateFlags2 |=
        (PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING | PLAYER_STATE2_ALWAYS_DISABLE_MOVE_ROTATION);

    if (LinkAnimation_Update(play, &this->skelAnime)) {
        Player_AnimationPlayLoop(play, this, &gPlayerAnim_link_normal_re_dead_attack_wait);
    }

    if (play->sceneId != SCENE_SEA_BS) {
        Player_SetupZoraBarrier(this, BTN_R);
    }

    if (Player_MashTimerThresholdExceeded(this, 0, 100)) {
        Player_SetupStandingStillType(this, play);
        this->stateFlags2 &= ~PLAYER_STATE2_80;
    }
}

void Player_SlipOnSlope(Player* this, PlayState* play) {
    CollisionPoly* floorPoly;
    f32 var_fv0;
    f32 temp_fv1;
    f32 var_fa0;
    s16 downwardSlopeYaw;
    s16 var_v0;
    Vec3f slopeNormal;

    this->stateFlags2 |=
        (PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING | PLAYER_STATE2_ALWAYS_DISABLE_MOVE_ROTATION);
    LinkAnimation_Update(play, &this->skelAnime);
    Player_SetupSpawnDustAtFeet(play, this);
    func_8019F780(&this->actor.projectedPos, Player_GetFloorSfx(this, NA_SE_PL_SLIP_LEVEL - SFX_FLAG),
                  this->actor.speedXZ);

    if (Player_SetupItemCutsceneOrFirstPerson(this, play)) {
        return;
    }

    if ((this->transformation == PLAYER_FORM_GORON) && func_8083A114(this, play)) {
        return;
    }

    floorPoly = this->actor.floorPoly;
    if (floorPoly == NULL) {
        func_80833AA0(this, play);
        return;
    }

    Actor_GetSlopeDirection(floorPoly, &slopeNormal, &downwardSlopeYaw);
    var_v0 = downwardSlopeYaw;
    if (this->genericVar != 0) {
        var_v0 = downwardSlopeYaw + 0x8000;
    }
    if (this->linearVelocity < 0.0f) {
        downwardSlopeYaw += 0x8000;
    }

    var_fv0 = (1.0f - slopeNormal.y) * 40.0f;
    var_fv0 = CLAMP(var_fv0, 0.0f, 10.0f);

    temp_fv1 = var_fv0 * var_fv0 * 0.015f;
    var_fa0 = slopeNormal.y * 0.01f;
    if (SurfaceType_GetSlope(&play->colCtx, floorPoly, this->actor.floorBgId) != 1) {
        var_fv0 = 0.0f;
        var_fa0 = slopeNormal.y * 10.0f;
    }
    temp_fv1 = CLAMP_MIN(temp_fv1, 1.0f);

    if (Math_AsymStepToF(&this->linearVelocity, var_fv0, temp_fv1, var_fa0) && (var_fv0 == 0.0f)) {
        Player_SetupReturnToStandStillSetAnim(this,
                                              (this->genericVar == 0)
                                                  ? D_8085BE84[PLAYER_ANIMGROUP_41][this->modelAnimType]
                                                  : D_8085BE84[PLAYER_ANIMGROUP_42][this->modelAnimType],
                                              play);
    }

    Math_SmoothStepToS(&this->currentYaw, downwardSlopeYaw, 0xA, 0xFA0, 0x320);
    Math_ScaledStepToS(&this->actor.shape.rot.y, var_v0, 0x7D0);
}

void func_80859CE0(PlayState* play, Player* this, s32 arg2);

void func_80854010(Player* this, PlayState* play) {
    if ((DECR(this->genericTimer) == 0) && Player_SetupCutscene(play, this)) {
        func_80859CE0(play, this, 0);
        Player_SetAction(play, this, Player_StartCutscene, 0);
        Player_StartCutscene(this, play);
    }
}

void func_808540A0(Player* this, PlayState* play) {
    Player_SetAction(play, this, func_80854010, 0);
    this->genericTimer = 0x28;
    Actor_Spawn(&play->actorCtx, play, ACTOR_DEMO_KANKYO, 0.0f, 0.0f, 0.0f, 0, 0, 0, 0x10);
}

void Player_Cutscene_SetPosAndYawToStart(Player* this, CsCmdActorAction* playerAction);

void Player_SpawnFromBlueWarp(Player* this, PlayState* play) {
    if (sPlayerYDistToFloor < 150.0f) {
        if (LinkAnimation_Update(play, &this->skelAnime)) {
            if (this->genericTimer == 0) {
                if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
                    this->skelAnime.endFrame = this->skelAnime.animLength - 1.0f;
                    Player_AnimSfx_PlayFloorLand(this);
                    this->genericTimer = 1;
                }
            } else {
                Player_SetupStandingStillMorph(this, play);
            }
        }

        Math_SmoothStepToF(&this->actor.velocity.y, 2.0f, 0.3f, 8.0f, 0.5f);
    }

    if (play->csCtx.state != CS_STATE_IDLE) {
        if (play->csCtx.playerAction != NULL) {
            s32 pad;
            f32 sp28 = this->actor.world.pos.y;

            Player_Cutscene_SetPosAndYawToStart(this, play->csCtx.playerAction);
            this->actor.world.pos.y = sp28;
        }
    }
}

void Player_EnterGrotto(Player* this, PlayState* play) {
    if (this->skelAnime.animation == NULL) {
        this->stateFlags2 |= PLAYER_STATE2_4000;
    } else {
        LinkAnimation_Update(play, &this->skelAnime);
        if ((this->skelAnime.animation == &gPlayerAnim_link_derth_rebirth) &&
            LinkAnimation_OnFrame(&this->skelAnime, 60.0f)) {
            Player_AnimSfx_PlayFloor(this, NA_SE_PL_BOUND);
            Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_DAMAGE_S);
        }
    }

    if ((this->genericTimer++ > 8) && !func_8082DA90(play)) {
        if (this->genericVar != 0) {
            if (this->genericVar < 0) {
                Play_TriggerRespawn(&play->state);
            } else {
                Play_TriggerVoidOut(&play->state);
            }
            if (!SurfaceType_IsWallDamage(&play->colCtx, this->actor.floorPoly, this->actor.floorBgId)) {
                gSaveContext.respawnFlag = -5;
            }

            play->transitionType = TRANS_TYPE_04;
            play_sound(NA_SE_OC_ABYSS);
        } else {
            play->transitionType = TRANS_TYPE_02;
            gSaveContext.nextTransitionType = TRANS_TYPE_02;
            gSaveContext.seqId = 0xFF;
            gSaveContext.ambienceId = 0xFF;
        }

        play->transitionTrigger = TRANS_TRIGGER_START;
    }
}

void Player_SetupOpenDoorFromSpawn(Player* this, PlayState* play) {
    Player_SetupOpenDoor(this, play);
}

void Player_JumpFromGrotto(Player* this, PlayState* play) {
    this->actor.gravity = -1.0f;
    LinkAnimation_Update(play, &this->skelAnime);

    if (this->actor.velocity.y < 0.0f) {
        func_80833AA0(this, play);
    } else if (this->actor.velocity.y < 6.0f) {
        Math_StepToF(&this->linearVelocity, 3.0f, 0.5f);
    }
}

void func_80854430(Player* this, PlayState* play) {
    if (play->shootingGalleryStatus < 0) {
        play->shootingGalleryStatus = 0;
        func_80839ED0(this, play);
    } else if (this->genericVar == 0) {
        if ((play->sceneId != SCENE_20SICHITAI) && CHECK_BTN_ALL(sPlayerControlInput->press.button, BTN_B)) {
            play->shootingGalleryStatus = 0xA;
            Player_SetupShootingGalleryPlay(play, this);
            Player_SetAction(play, this, func_80854430, 1);
            this->genericVar = 1;
        } else {
            play->shootingGalleryStatus = 0;
            Player_SetFirstPersonAimLookAngles(play, this, 0);

            if (play->actorCtx.flags & ACTORCTX_FLAG_PICTOGRAPH_ON) {
                this->stateFlags1 |= PLAYER_STATE1_100000;
                Player_SetupCameraMode(play, this);
            } else {
                this->stateFlags1 &= ~PLAYER_STATE1_100000;
                if ((play->sceneId == SCENE_20SICHITAI) &&
                    (func_8012364C(play, this, Player_GetButtonPressed()) == ITEM_PICTO_BOX)) {
                    s32 requiredScopeTemp;

                    play->actorCtx.flags |= ACTORCTX_FLAG_PICTOGRAPH_ON;
                }
            }
        }
    } else if (CHECK_BTN_ANY(sPlayerControlInput->press.button,
                             BTN_CRIGHT | BTN_CLEFT | BTN_CDOWN | BTN_CUP | BTN_R | BTN_A)) {
        play->shootingGalleryStatus = -1;
        Player_ShootingGalleryPlay(this, play);
        Player_SetAction(play, this, func_80854430, 0);
        this->genericVar = 0;
    } else {
        play->shootingGalleryStatus = 0xA;
        Player_ShootingGalleryPlay(this, play);
    }
}

void Player_ShootingGalleryPlay(Player* this, PlayState* play) {
    this->attentionMode = PLAYER_ATTENTIONMODE_AIMING;
    Player_SetupCameraMode(play, this);
    LinkAnimation_Update(play, &this->skelAnime);
    Player_SetupCurrentUpperAction(this, play);
    this->upperBodyRot.y = Player_SetFirstPersonAimLookAngles(play, this, 1) - this->actor.shape.rot.y;
    this->lookFlags |= 0x80;

    if (play->shootingGalleryStatus < 0) {
        play->shootingGalleryStatus++;
        if (play->shootingGalleryStatus == 0) {
            func_80839ED0(this, play);
        }
    }
}

void func_808546D0(Player* this, PlayState* play) {
    if (this->genericVar >= 0) {
        if (this->genericVar < 6) {
            this->genericVar++;
        } else {
            this->unk_B48 = (this->genericVar >> 1) * 22.0f;
            if (Player_MashTimerThresholdExceeded(this, 1, 0x64)) {
                this->genericVar = -1;
                EffectSsIcePiece_SpawnBurst(play, &this->actor.world.pos, this->actor.scale.x);
                Player_PlaySfx(this, NA_SE_PL_ICE_BROKEN);
            }

            if (this->transformation == PLAYER_FORM_ZORA) {
                Player_SetupEnterGrotto(play, this);
                this->skelAnime.animation = NULL;
                this->genericTimer = -0x28;
                this->genericVar = 1;
                this->linearVelocity = 0.0f;
            } else if (play->gameplayFrames % 4 == 0) {
                Player_InflictDamage(play, -1);
            }
        }

        this->stateFlags2 |= PLAYER_STATE2_4000;
    } else if (LinkAnimation_Update(play, &this->skelAnime)) {
        Player_SetupStandingStillType(this, play);
        Player_SetupInvincibilityNoDamageFlash(this, 20);
    }
}

void func_80854800(Player* this, PlayState* play) {
    LinkAnimation_Update(play, &this->skelAnime);
    func_808345A8(this);

    if (((this->genericTimer % 25) != 0) || (func_808339D4(play, this, -1) != 0)) {
        if (DECR(this->genericTimer) == 0) {
            Player_SetupStandingStillType(this, play);
        }
    }

    this->shockTimer = 40;
    func_800B8F98(&this->actor, this->ageProperties->voiceSfxOffset + (NA_SE_VO_LI_TAKEN_AWAY - SFX_FLAG));
}

void Player_MeleeWeaponAttack(Player* this, PlayState* play) {
    AttackAnimInfo* attackInfoEntry = &sMeleeAttackAnimInfo[this->meleeWeaponAnimation];

    if (this->skelAnime.curFrame < (this->skelAnime.endFrame - 6.0f)) {
        this->stateFlags2 |= PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING;
    }

    if (func_808401F4(play, this)) {
        return;
    }

    if (this->linearVelocity >= 0.0f) {
        Player_SetupMeleeAttack(play, this, (this->transformation == PLAYER_FORM_GORON) ? 5.0f : 0.0f,
                                attackInfoEntry->unk_C, attackInfoEntry->unk_D);
    }

    if ((this->meleeWeaponAnimation == PLAYER_MWA_GORON_PUNCH_LEFT) ||
        (this->meleeWeaponAnimation == PLAYER_MWA_GORON_PUNCH_RIGHT)) {
        this->unk_3D0.unk_00 = 3;
    }

    //! @bug Lunge Storage: If this block is prevented from running at the end of an animation that produces a lunge,
    //! the prepared lunge will be retained until next time execution passes through here, which usually means the next
    //! sword slash.
    if ((this->stateFlags2 & PLAYER_STATE2_ENABLE_FORWARD_SLIDE_FROM_ATTACK) &&
        LinkAnimation_OnFrame(&this->skelAnime, 0.0f)) {
        this->linearVelocity = 15.0f;
        this->stateFlags2 &= ~PLAYER_STATE2_ENABLE_FORWARD_SLIDE_FROM_ATTACK;
    }

    if (this->linearVelocity > 12.0f) {
        Player_SetupSpawnDustAtFeet(play, this);
    }

    Math_StepToF(&this->linearVelocity, 0.0f, 5.0f);
    func_8083A548(this);

    if (LinkAnimation_Update(play, &this->skelAnime) ||
        ((this->meleeWeaponAnimation >= PLAYER_MWA_FLIPSLASH_FINISH) &&
         (this->meleeWeaponAnimation <= PLAYER_MWA_ZORA_JUMPKICK_FINISH) && (this->skelAnime.curFrame > 2.0f) &&
         Player_CanQuickspin(this))) {
        sUsingItemAlreadyInHand = this->genericTimer;

        if (!func_80848570(this, play)) {
            LinkAnimationHeader* anim =
                Player_IsUnfriendlyZTargeting(this) ? attackInfoEntry->unk_8 : attackInfoEntry->unk_4;

            Player_DeactivateMeleeWeapon(this);

            if (anim == NULL) {
                this->skelAnime.moveFlags &= ~8;
                Player_SetupStandingStillMorph(this, play);
            } else {
                u8 moveFlags = this->skelAnime.moveFlags;

                if (this->transformation == PLAYER_FORM_ZORA) {
                    if (func_8083A580(this, play)) {
                        anim = this->skelAnimeUpper.animation;
                    }
                    this->unk_ADC = 0;
                } else if ((anim == &gPlayerAnim_link_fighter_Lpower_jump_kiru_end) &&
                           (this->modelAnimType != PLAYER_ANIMTYPE_3)) {
                    anim = &gPlayerAnim_link_fighter_power_jump_kiru_end;
                }

                this->skelAnime.moveFlags = 0;
                Player_SetAction(play, this, Player_StandingStill, 1);
                Player_PlayAnimOnceWithWaterInfluence(play, this, anim);
                this->currentYaw = this->actor.shape.rot.y;
                this->skelAnime.moveFlags = moveFlags;
            }
            this->stateFlags3 |= PLAYER_STATE3_8;
        }
    } else if (((this->transformation == PLAYER_FORM_ZORA) &&
                (this->meleeWeaponAnimation != PLAYER_MWA_ZORA_PUNCH_KICK) &&
                (this->meleeWeaponAnimation != PLAYER_MWA_ZORA_JUMPKICK_FINISH)) ||
               ((this->transformation == PLAYER_FORM_GORON) &&
                (this->meleeWeaponAnimation != PLAYER_MWA_GORON_PUNCH_BUTT))) {
        this->genericTimer |= CHECK_BTN_ALL(sPlayerControlInput->press.button, BTN_B) ? 1 : 0;
    }
}

void func_80854C70(Player* this, PlayState* play) {
    LinkAnimation_Update(play, &this->skelAnime);
    Player_StepLinearVelocityToZero(this);

    if (this->skelAnime.curFrame >= 6.0f) {
        Player_SetupStandingStillType(this, play);
    }
}

// Array colour interpolation
// arg0 is the interpolation parameter
// arg1,5,9 are out colours
// arg2,6,0xA are first values
// arg3,7,0xB are second values
// arg4,8,0xC are subtracted after interpolation
void func_80854CD0(f32 arg0, s16* arg1, u8* arg2, u8* arg3, u8* arg4, s16* arg5, u8* arg6, u8* arg7, u8* arg8,
                   s16* arg9, u8* argA, u8* argB, u8* argC) {
    s32 i;

    for (i = 0; i < 3; i++) {
        *arg1 = ((s32)((*arg2 - *arg3) * arg0) + *arg3) - *arg4;
        *arg5 = ((s32)((*arg6 - *arg7) * arg0) + *arg7) - *arg8;
        *arg9 = ((s32)((*argA - *argB) * arg0) + *argB) - *argC;

        arg1++;
        arg2++;
        arg3++;
        arg4++;
        arg5++;
        arg6++;
        arg7++;
        arg8++;
        arg9++;
        argA++;
        argB++;
        argC++;
    }
}

// Black, probably in-function static
u8 D_8085D844[] = { 0, 0, 0 };

// arg1 is the colour interpolation parameter
void func_80854EFC(PlayState* play, f32 arg1, struct_8085D848_unk_00* arg2) {
    struct_8085D848_unk_00 sp70;
    struct_8085D848_unk_00* var_t0;
    struct_8085D848_unk_00* var_v1;
    u8* var_t3;
    u8* var_t4;
    u8* new_var;
    s32 pad[4];

    new_var = play->envCtx.unk_C4.diffuseColor1;
    sp70.fogNear = play->envCtx.unk_C4.fogNear;
    sp70.fogColor[0] = play->envCtx.unk_C4.fogColor[0];
    sp70.fogColor[1] = play->envCtx.unk_C4.fogColor[1];
    sp70.fogColor[2] = play->envCtx.unk_C4.fogColor[2];
    sp70.ambientColor[0] = play->envCtx.unk_C4.ambientColor[0];
    sp70.ambientColor[1] = play->envCtx.unk_C4.ambientColor[1];
    sp70.ambientColor[2] = play->envCtx.unk_C4.ambientColor[2];

    if (arg1 <= 1.0f) {
        arg1 -= 0.0f;

        var_v1 = &arg2[0];
        var_t0 = &sp70;
        var_t3 = D_8085D844;
        var_t4 = new_var;
    } else if (arg1 <= 2.0f) {
        arg1 -= 1.0f;
        var_v1 = &arg2[1];
        var_t0 = &arg2[0];
        var_t3 = D_8085D844;
        var_t4 = D_8085D844;

    } else if (arg1 <= 3.0f) {
        arg1 -= 2.0f;
        var_v1 = &arg2[2];
        var_t0 = &arg2[1];
        var_t3 = D_8085D844;
        var_t4 = D_8085D844;

    } else {
        arg1 -= 3.0f;
        var_v1 = &sp70;
        var_t0 = &arg2[2];
        var_t3 = new_var;
        var_t4 = D_8085D844;
    }

    play->envCtx.lightSettings.fogNear =
        ((s16)((var_v1->fogNear - var_t0->fogNear) * arg1) + var_t0->fogNear) - play->envCtx.unk_C4.fogNear;

    func_80854CD0(arg1, play->envCtx.lightSettings.fogColor, var_v1->fogColor, var_t0->fogColor,
                  play->envCtx.unk_C4.fogColor, play->envCtx.lightSettings.ambientColor, var_v1->ambientColor,
                  var_t0->ambientColor, play->envCtx.unk_C4.ambientColor, play->envCtx.lightSettings.diffuseColor1,
                  var_t3, var_t4, new_var);
}

struct_8085D848 D_8085D848[] = {
    {
        {
            { 650, { 0, 0, 0 }, { 10, 0, 30 } },
            { 300, { 200, 200, 255 }, { 0, 0, 0 } },
            { 600, { 0, 0, 0 }, { 0, 0, 200 } },
        },
        {
            { { -40.0f, 20.0f, -10.0f }, { 120, 200, 255 }, 1000 },
            { { 0.0f, -10.0f, 0.0f }, { 255, 255, 255 }, 5000 },
            { { -10.0f, 4.0f, 3.0f }, { 200, 200, 255 }, 5000 },
        },
    },
    {
        {
            { 650, { 0, 0, 0 }, { 10, 0, 30 } },
            { 300, { 200, 200, 255 }, { 0, 0, 0 } },
            { 600, { 0, 0, 0 }, { 0, 0, 200 } },
        },
        {
            { { 0.0f, 0.0f, 5.0f }, { 155, 255, 255 }, 100 },
            { { 0.0f, 0.0f, 5.0f }, { 155, 255, 255 }, 100 },
            { { 0.0f, 0.0f, 5.0f }, { 155, 255, 255 }, 100 },
        },
    },
};

// arg2 is the colour interpolation parameter
// arg3 both selects the light to use and scales the radius
// arg4 selects the env fog/colour info
void func_808550D0(PlayState* play, Player* this, f32 arg2, f32 arg3, s32 arg4) {
    struct_8085D848* temp_a2 = &D_8085D848[arg4];
    struct_8085D848_unk_18* lightInit = temp_a2->light;
    Vec3f pos;

    func_80854EFC(play, arg2, temp_a2->unk_00);

    if (arg3 > 2.0f) {
        arg3 -= 2.0f;
        lightInit += 2;
    } else if (arg3 > 1.0f) {
        arg3 -= 1.0f;
        lightInit += 1;
    }

    Player_TranslateAndRotateY(this, &this->actor.world.pos, &lightInit->pos, &pos);
    Lights_PointNoGlowSetInfo(&this->lightInfo, pos.x, pos.y, pos.z, lightInit->color[0], lightInit->color[1],
                              lightInit->color[2], lightInit->radius * arg3);
}

AnimSfxEntry D_8085D8F0[] = {
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 2, NA_SE_PL_PUT_OUT_ITEM, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 4, NA_SE_IT_SET_TRANSFORM_MASK, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 11, NA_SE_PL_FREEZE_S, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 30, NA_SE_PL_TRANSFORM_VOICE, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 20, NA_SE_IT_TRANSFORM_MASK_BROKEN, STOP),
};

AnimSfxEntry D_8085D904[] = {
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 8, NA_SE_IT_SET_TRANSFORM_MASK, STOP),
};

void func_80855218(PlayState* play, Player* this, UNK_PTR arg2) {
    if (LinkAnimation_Update(play, &this->skelAnime) && (this->skelAnime.animation == &gPlayerAnim_cl_setmask)) {
        func_8082DB60(play, this, &gPlayerAnim_cl_setmaskend);
    } else if ((this->skelAnime.animation == &gPlayerAnim_cl_setmask) ||
               (this->skelAnime.animation == &gPlayerAnim_cl_setmaskend)) {
        if (this->genericVar >= 58) {
            Math_StepToS(&this->genericTimer, 255, 50);
        }

        if (this->genericVar >= 64) {
            Math_StepToF(&this->unk_B10[2], 0.0f, 0.015f);
        } else if (this->genericVar >= 0xE) {
            Math_StepToF(&this->unk_B10[2], 0.3f, 0.3f);
        }

        if (this->genericVar > 65) {
            Math_StepToF(&this->unk_B10[3], 0.0f, 0.02f);
        } else if (this->genericVar >= 0x10) {
            Math_StepToF(&this->unk_B10[3], -0.1f, 0.1f);
        }

        if ((MREG(64) == 0) && (this->skelAnime.animation == &gPlayerAnim_cl_setmask)) {
            Player_PlayAnimSfx(this, D_8085D8F0);
        }
    } else {
        if (this->genericVar >= 20) {
            Math_StepToS(&this->genericTimer, 255, 20);
        }

        if (MREG(64) == 0) {
            Player_PlayAnimSfx(this, D_8085D904);
            if (this->genericVar == 15) {
                Player_PlaySfx(this, NA_SE_PL_FACE_CHANGE);
            }
        }
    }
}

u16 D_8085D908[] = {
    WEEKEVENTREG_30_80, // PLAYER_FORM_FIERCE_DEITY
    WEEKEVENTREG_30_20, // PLAYER_FORM_GORON
    WEEKEVENTREG_30_40, // PLAYER_FORM_ZORA
    WEEKEVENTREG_30_10, // PLAYER_FORM_DEKU
};
struct_8085D910 D_8085D910[] = {
    { 0x10, 0xA, 0x3B, 0x3F },
    { 9, 0x32, 0xA, 0xD },
};

void func_808553F4(Player* this, PlayState* play) {
    struct_8085D910* sp4C = D_8085D910;
    s32 sp48 = false;

    func_808323C0(this, play->playerActorCsIds[5]);
    sPlayerControlInput = play->state.input;

    Camera_ChangeMode(play->cameraPtrs[play->activeCamId],
                      (this->transformation == PLAYER_FORM_HUMAN) ? CAM_MODE_NORMAL : CAM_MODE_JUMP);
    this->stateFlags2 |= PLAYER_STATE2_ALWAYS_DISABLE_MOVE_ROTATION;
    this->actor.shape.rot.y = Camera_GetCamDirYaw(play->cameraPtrs[play->activeCamId]) + 0x8000;

    func_80855218(play, this, &sp4C);

    if (this->genericVar == 0x14) {
        func_80165DCC(0x64);
    }

    if (MREG(64) != 0) {
        MREG(68) += MREG(64);
        if (MREG(68) > 0xFF) {
            MREG(68) = 0xFF;
            this->actor.update = func_8012301C;
            this->actor.draw = NULL;
            this->genericVar = 0;
            func_80165DF0();
            SET_WEEKEVENTREG(D_8085D908[gSaveContext.save.playerForm]);
        }
    } else if ((this->genericVar++ > ((this->transformation == PLAYER_FORM_HUMAN) ? 0x53 : 0x37)) ||
               ((this->genericVar >= 5) &&
                (sp48 = ((this->transformation != PLAYER_FORM_HUMAN) ||
                         CHECK_WEEKEVENTREG(D_8085D908[gSaveContext.save.playerForm])) &&
                        CHECK_BTN_ANY(sPlayerControlInput->press.button,
                                      BTN_CRIGHT | BTN_CLEFT | BTN_CDOWN | BTN_CUP | BTN_B | BTN_A)))) {
        MREG(64) = 0x2D;
        MREG(65) = 0xDC;
        MREG(66) = 0xDC;
        MREG(67) = 0xDC;
        MREG(68) = 0;

        if (sp48) {
            if (ActorCutscene_GetCurrentIndex() == this->unk_A86) {
                func_800E0348(Play_GetCamera(play, ActorCutscene_GetCurrentSubCamId(this->unk_A86)));
            }

            if (this->transformation == PLAYER_FORM_HUMAN) {
                AudioSfx_StopById(NA_SE_PL_TRANSFORM_VOICE);
                AudioSfx_StopById(NA_SE_IT_TRANSFORM_MASK_BROKEN);
            } else {
                AudioSfx_StopById(NA_SE_PL_FACE_CHANGE);
            }
        }

        Player_PlaySfx(this, NA_SE_SY_TRANSFORM_MASK_FLASH);
    }

    if (this->genericVar >= sp4C->unk_0) {
        if (this->genericVar < sp4C->unk_2) {
            Math_StepToF(&this->unk_B10[4], 1.0f, sp4C->unk_1 / 100.0f);
        } else if (this->genericVar < sp4C->unk_3) {
            if (this->genericVar == sp4C->unk_2) {
                func_801000CC(NA_SE_EV_LIGHTNING_HARD);
            }

            Math_StepToF(&this->unk_B10[4], 2.0f, 0.5f);
        } else {
            Math_StepToF(&this->unk_B10[4], 3.0f, 0.2f);
        }
    }

    if (this->genericVar >= 0x10) {
        if (this->genericVar < 0x40) {
            Math_StepToF(&this->unk_B10[5], 1.0f, 0.2f);
        } else if (this->genericVar < 0x37) {
            Math_StepToF(&this->unk_B10[5], 2.0f, 1.0f);
        } else {
            Math_StepToF(&this->unk_B10[5], 3.0f, 0.55f);
        }
    }

    func_808550D0(play, this, this->unk_B10[4], this->unk_B10[5], (this->transformation == PLAYER_FORM_HUMAN) ? 0 : 1);
}

void func_80855818(Player* this, PlayState* play) {
    Camera_ChangeMode(play->cameraPtrs[play->activeCamId],
                      (this->prevMask == PLAYER_MASK_NONE) ? CAM_MODE_NORMAL : CAM_MODE_JUMP);

    if (MREG(64) != 0) {
        MREG(68) -= MREG(64);
        if (MREG(68) < 0) {
            MREG(64) = 0;
            MREG(68) = 0;
        }
    }

    if (this->genericVar++ < 4) {
        if ((this->prevMask == PLAYER_MASK_NONE) && (this->genericVar == 4)) {
            LinkAnimation_Change(play, &this->skelAnime, Player_GetStandingStillAnim(this), 1.0f, 0.0f, 20.0f,
                                 ANIMMODE_ONCE, 20.0f);
        }
    } else {
        s32 pad;
        f32 dist;
        s16 angle;

        func_800FF3A0(&dist, &angle, play->state.input);
        if (LinkAnimation_Update(play, &this->skelAnime) || ((this->genericVar > 10) && (dist != 0.0f))) {
            if (MREG(64) == 0) {
                this->stateFlags1 &= ~PLAYER_STATE1_2;
                this->prevMask = this->currentMask;
                this->unk_A86 = play->playerActorCsIds[5];
                func_80838760(this);
                play->envCtx.lightSettings = D_80862B50;
                Player_SetupStandingStillMorph(this, play);
                return;
            }
        }

        Math_StepToF(&this->unk_B10[5], 4.0f, 0.2f);
    }

    func_808550D0(play, this, 0, this->unk_B10[5], (this->prevMask == PLAYER_MASK_NONE) ? 0 : 1);
}

void Player_SetupElegyShell(Player* this, PlayState* play) {
    if (this->genericTimer++ > 90) {
        play->msgCtx.ocarinaMode = 4;
        Player_SetupStandingStillMorph(this, play);
    } else if (this->genericTimer == 10) {
        Player_SpawnElegyShell(play, this);
    }
}

// Giant's Mask
void func_80855AF4(Player* this, PlayState* play) {
    this->stateFlags2 |= PLAYER_STATE2_ALWAYS_DISABLE_MOVE_ROTATION;

    func_80855218(play, this, 0);
    this->genericVar++;

    if (!(this->stateFlags1 & PLAYER_STATE1_100)) {
        this->prevMask = this->currentMask;
        gSaveContext.save.equippedMask = this->currentMask = PLAYER_MASK_GIANT;
        Magic_Consume(play, 0, MAGIC_CONSUME_GIANTS_MASK);
        this->currentBoots = PLAYER_BOOTS_GIANT;
        this->prevBoots = PLAYER_BOOTS_GIANT;
        Player_SetBootData(play, this);
        Player_SetupStandingStillMorph(this, play);
    }
}

void func_80855B9C(Player* this, PlayState* play) {
    this->stateFlags2 |= PLAYER_STATE2_ALWAYS_DISABLE_MOVE_ROTATION;

    LinkAnimation_Update(play, &this->skelAnime);

    if (!(this->stateFlags1 & PLAYER_STATE1_100)) {
        this->prevMask = this->currentMask;

        gSaveContext.save.equippedMask = this->currentMask = PLAYER_MASK_NONE;

        this->currentBoots = PLAYER_BOOTS_HYLIAN;
        this->prevBoots = PLAYER_BOOTS_HYLIAN;
        Player_SetBootData(play, this);
        Player_SetupStandingStillMorph(this, play);
    }
}

void func_80855C28(Player* this, PlayState* play) {
    s16 sp3E;
    s32 pad;
    LinkAnimationHeader* sp34;
    s32 var_a0;

    func_808323C0(this, play->playerActorCsIds[4]);
    sp3E = BINANG_SUB(this->actor.shape.rot.y, this->actor.world.rot.y);

    var_a0 = 0;
    if ((this->actor.floorHeight - this->actor.world.pos.y) < 60.0f) {
        Math_StepToF(&this->unk_B10[5], 200.0f, 150.0f);
        var_a0 = Math_StepToS(&this->genericTimer, 0xFA0, 0x15E);
    }

    this->actor.shape.rot.y += this->genericTimer;
    this->skelAnime.jointTable[PLAYER_LIMB_ROOT - 1].x = 0;
    this->skelAnime.jointTable[PLAYER_LIMB_ROOT - 1].z = 0;
    this->unk_ABC += this->unk_B10[5];

    if (this->unk_ABC >= 0.0f) {
        this->unk_ABC = 0.0f;
        if ((var_a0 != 0) && (sp3E < 0)) {
            if (BINANG_SUB(this->actor.shape.rot.y, this->actor.world.rot.y) >= 0) {
                this->actor.shape.rot.y = this->actor.world.rot.y;
                func_80838760(this);
                if (PLAYER_GET_INITMODE(&this->actor) == PLAYER_INITMODE_8) {
                    sp34 = sPlayerOcarinaStartAnimations[this->transformation];
                    Player_SetupReturnToStandStill(this, play);
                    LinkAnimation_Change(play, &this->skelAnime, sp34, -2.0f / 3.0f, Animation_GetLastFrame(sp34), 0.0f,
                                         ANIMMODE_ONCE, -6.0f);
                } else {
                    Player_SetupStandingStillNoMorph(this, play);
                }
            }
        }
    } else if (this->genericVar == 0) {
        Player_PlaySfx(this, NA_SE_PL_WARP_PLATE_OUT);
        this->genericVar = 1;
    }
}

void func_80855E08(Player* this, PlayState* play) {
    this->stateFlags2 |= PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING;

    if (LinkAnimation_Update(play, &this->skelAnime)) {
        Player_AnimationPlayLoop(play, this, &gPlayerAnim_link_hook_fly_wait);
    }

    Math_Vec3f_Sum(&this->actor.world.pos, &this->actor.velocity, &this->actor.world.pos);
    if (Player_EndHookshotMove(play, this)) {
        f32 var_fv0;

        Math_Vec3f_Copy(&this->actor.prevPos, &this->actor.world.pos);
        func_80843178(play, this);

        var_fv0 = this->actor.world.pos.y - this->actor.floorHeight;
        var_fv0 = CLAMP_MAX(var_fv0, 20.0f);

        this->actor.world.pos.y = this->actor.world.pos.y - var_fv0;
        this->actor.shape.rot.x = 0;
        this->linearVelocity = 1.0f;
        this->actor.velocity.y = 0.0f;
        this->actor.world.rot.x = this->actor.shape.rot.x;
        func_80833AA0(this, play);
        this->stateFlags2 &= ~PLAYER_STATE2_DIVING;
        this->actor.bgCheckFlags |= 1;
        this->stateFlags3 |= PLAYER_STATE3_10000;
    } else if ((this->skelAnime.animation != &gPlayerAnim_link_hook_fly_start) || (this->skelAnime.curFrame >= 4.0f)) {
        this->actor.gravity = 0.0f;
        Math_ScaledStepToS(&this->actor.shape.rot.x, this->actor.world.rot.x, 0x800);
        Player_RequestRumble(play, this, 100, 2, 100, SQ(0));
    }
}

void func_80855F9C(PlayState* play, Player* this) {
    f32 sp24;
    s16 sp22;

    this->stateFlags2 |= PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING;
    Player_GetTargetVelocityAndYaw(this, &sp24, &sp22, 0.018f, play);
    Math_ScaledStepToS(&this->currentYaw, sp22, 0x258);
}

s32 func_80856000(PlayState* play, Player* this) {
    CollisionPoly* poly;
    s32 bgId;
    Vec3f pos;
    f32 sp28;

    pos.x = this->actor.world.pos.x;
    pos.y = this->actor.world.pos.y - 20.0f;
    pos.z = this->actor.world.pos.z;
    return BgCheck_EntityCheckCeiling(&play->colCtx, &sp28, &pos, 30.0f, &poly, &bgId, &this->actor);
}

void func_80856074(PlayState* play, Player* this) {
    if (func_8083F8A8(play, this, 12.0f, 4, 0.0f, 10, 50, true)) {
        EffectSsHahen_SpawnBurst(play, &this->actor.world.pos, 3.0f, 0, 4, 8, 2, -1, 10, NULL);
    }
}

void func_80856110(PlayState* play, Player* this, f32 arg2, f32 arg3, f32 arg4, s16 scale, s16 scaleStep, s16 life) {
    static Vec3f D_8085D918 = { 0.0f, 0.5f, 0.0f };        // velocity
    static Vec3f D_8085D924 = { 0.0f, 0.5f, 0.0f };        // accel
    static Color_RGBA8 D_8085D930 = { 255, 255, 55, 255 }; // primColor
    static Color_RGBA8 D_8085D934 = { 100, 50, 0, 0 };     // envColor
    Vec3f pos;

    pos.x = this->actor.world.pos.x;
    pos.y = this->actor.world.pos.y + arg2;
    pos.z = this->actor.world.pos.z;

    D_8085D918.y = arg3;
    D_8085D924.y = arg4;

    func_800B0EB0(play, &pos, &D_8085D918, &D_8085D924, &D_8085D930, &D_8085D934, scale, scaleStep, life);
}

// Deku Flower related
void func_808561B0(Player* this, PlayState* play) {
    DynaPolyActor* dyna;
    s32 aux = 0xAE;
    f32 temp_fv0_2; // sp3C
    s32 sp38;
    s32 var_v1; // sp34

    LinkAnimation_Update(play, &this->skelAnime);

    if (Player_SetupItemCutsceneOrFirstPerson(this, play)) {
        return;
    }

    if (this->genericVar == 0) {
        this->unk_ABC += this->unk_B48;
        if (this->unk_ABC < -1000.0f) {
            this->unk_ABC = -1000.0f;
            this->genericVar = 1;
            this->unk_B48 = 0.0f;
        }
        func_80856074(play, this);
    } else if (this->genericVar == 1) {
        this->unk_B48 += -22.0f;
        if (this->unk_B48 < -170.0f) {
            this->unk_B48 = -170.0f;
        }
        this->unk_ABC += this->unk_B48;
        if (this->unk_ABC < -3900.0f) {
            this->unk_ABC = -3900.0f;
            this->genericVar = 2;
            this->actor.shape.rot.y = Camera_GetInputDirYaw(play->cameraPtrs[play->activeCamId]);
            this->actor.scale.y = 0.01f;
            this->currentYaw = this->actor.world.rot.y = this->actor.shape.rot.y;
        } else {
            temp_fv0_2 = Math_SinS((1000.0f + this->unk_ABC) * (-30.0f)) * 0.004f;
            this->actor.scale.y = 0.01f + temp_fv0_2;
            this->actor.scale.z = this->actor.scale.x = 0.01f - (this->unk_B48 * -0.000015f);

            this->actor.shape.rot.y += (s16)(this->unk_B48 * 130.0f);
            if (this->actor.floorBgId != BGCHECK_SCENE) {
                dyna = DynaPoly_GetActor(&play->colCtx, this->actor.floorBgId);

                if (dyna != NULL) {
                    Math_Vec3f_StepToXZ(&this->actor.world.pos, &dyna->actor.world.pos, 1.0f);
                }
            }
        }

        func_80856074(play, this);
    } else if (this->genericVar == 2) {
        if (!CHECK_BTN_ALL(sPlayerControlInput->cur.button, BTN_A)) {
            if (func_80856000(play, this)) {
                this->genericTimer = 0;
            } else {
                this->genericVar = 3;
                if (this->genericTimer >= 10) {
                    this->unk_B48 = 2700.0f;
                } else {
                    this->unk_B48 = 1450.0f;
                }
                Player_PlayReactableSfx(this, NA_SE_PL_DEKUNUTS_OUT_GRD);
            }
        } else if (this->genericTimer < 15) {
            this->genericTimer++;
            if (this->genericTimer == 10) {
                func_80856110(play, this, 20.0f, 3.8f, -0.1f, 140, 23, 15);
            }
        }
        func_80855F9C(play, this);
    } else {
        this->unk_ABC += this->unk_B48;

        temp_fv0_2 = this->unk_ABC;
        if (temp_fv0_2 >= 0.0f) {
            f32 speed; // sp30

            sp38 = (this->genericTimer >= 10);
            var_v1 = -1;
            speed = this->unk_B48 * this->actor.scale.y;
            if (this->actor.floorBgId != BGCHECK_SCENE) {
                dyna = DynaPoly_GetActor(&play->colCtx, this->actor.floorBgId);
                var_v1 = 0;
                if ((dyna != NULL) && (dyna->actor.id == ACTOR_OBJ_ETCETERA) && (dyna->actor.params & 0x100)) {
                    var_v1 = 1;
                    speed *= (f32)aux / 100.0f;
                }
            }

            Math_Vec3f_Copy(this->unk_AF0, &this->actor.world.pos);
            this->unk_ABC = 0.0f;
            this->actor.world.pos.y += temp_fv0_2 * this->actor.scale.y;
            Player_SetupJump(this, &gPlayerAnim_pn_kakku, speed, play);
            Player_SetAction(play, this, func_80856918, 1);
            this->boomerangActor = NULL;

            this->stateFlags3 |= PLAYER_STATE3_200;
            if (sp38 != 0) {
                this->stateFlags3 |= PLAYER_STATE3_2000;
            }
            if (var_v1 < 0) {
                this->stateFlags3 |= PLAYER_STATE3_40000;
            }

            this->genericVar = var_v1;
            this->genericTimer = 9999;
            Player_SetCylinderForAttack(this, DMG_DEKU_LAUNCH, 2, 20);
        } else if (this->unk_ABC < 0.0f) {
            func_80856074(play, this);
        }
    }

    if (this->unk_ABC < -1500.0f) {
        this->stateFlags3 |= PLAYER_STATE3_100;
        if (this->unk_B86[0] < 8) {
            this->unk_B86[0]++;
            if (this->unk_B86[0] == 8) {
                Player_PlayReactableSfx(this, NA_SE_PL_DEKUNUTS_BUD);
            }
        }
    }
}

void func_808566C0(PlayState* play, Player* this, PlayerBodyPart bodyPartIndex, f32 arg3, f32 arg4, f32 arg5,
                   s32 life) {
    Color_RGBA8 primColor = { 255, 200, 200, 0 };
    Color_RGBA8 envColor = { 255, 255, 0, 0 };
    static Vec3f D_8085D940 = { 0.0f, 0.3f, 0.0f };
    static Vec3f D_8085D94C = { 0.0f, -0.025f, 0.0f };
    Vec3f pos;
    s32 scale;
    f32 sp34;
    Vec3f* temp_v0;

    if (Rand_ZeroOne() < 0.5f) {
        sp34 = -1.0f;
    } else {
        sp34 = 1.0f;
    }

    D_8085D940.x = (Rand_ZeroFloat(arg4) + arg3) * sp34;
    D_8085D94C.x = arg5 * sp34;
    if (Rand_ZeroOne() < 0.5f) {
        sp34 = -1.0f;
    } else {
        sp34 = 1.0f;
    }

    temp_v0 = &this->bodyPartsPos[bodyPartIndex];
    D_8085D940.z = (Rand_ZeroFloat(arg4) + arg3) * sp34;
    D_8085D94C.z = arg5 * sp34;
    pos.x = temp_v0->x;
    pos.y = Rand_ZeroFloat(15.0f) + temp_v0->y;
    pos.z = temp_v0->z;
    if (Rand_ZeroOne() < 0.5f) {
        scale = 2000;
    } else {
        scale = -150;
    }

    EffectSsKirakira_SpawnDispersed(play, &pos, &D_8085D940, &D_8085D94C, &primColor, &envColor, scale, life);
}

void func_8085687C(Player* this) {
}

s32 func_80856888(f32* arg0, f32 arg1, f32 arg2) {
    if (arg2 != 0.0f) {
        if (arg1 < *arg0) {
            arg2 = -arg2;
        }

        *arg0 += arg2;
        if (((*arg0 - arg1) * arg2) >= 0.0f) {
            *arg0 = arg1;
            return true;
        }
    } else if (arg1 == *arg0) {
        return true;
    }

    return false;
}

f32 D_8085D958[] = { 600.0f, 960.0f };
Vec3f D_8085D960 = { -30.0f, 50.0f, 0.0f };
Vec3f D_8085D96C = { 30.0f, 50.0f, 0.0f };

// Flying as Deku?
void func_80856918(Player* this, PlayState* play) {
    if ((this->boomerangActor != NULL) && (this->boomerangActor->update == NULL)) {
        this->boomerangActor = NULL;
    }

    if (Player_SetupItemCutsceneOrFirstPerson(this, play)) {
        return;
    }

    if (this->actor.bgCheckFlags & 1) {
        func_80837134(play, this);
        return;
    }

    if ((this->actor.velocity.y > 0.0f) && (this->stateFlags3 & PLAYER_STATE3_200)) {
        this->actor.terminalVelocity = -20.0f;
        this->actor.gravity = -5.5f;
        Player_SetCylinderForAttack(this, DMG_DEKU_LAUNCH, 2, 20);
        func_80856110(play, this, 0.0f, 0.0f, -1.0f, 500, 0, 8);

        if (this->actor.bgCheckFlags & 0x10) {
            func_80833AA0(this, play);
        }
    } else if (!(this->stateFlags3 & PLAYER_STATE3_2000)) {
        func_80833AA0(this, play);
    } else if (this->stateFlags3 & PLAYER_STATE3_200) {
        if (this->actor.velocity.y < 0.0f) {
            if (this->genericVar < 0) {
                func_80833AA0(this, play);
            } else {
                LinkAnimation_Update(play, &this->skelAnime);
                if (this->skelAnime.curFrame > 6.0f) {
                    this->actor.velocity.y = 6.0f;
                    this->stateFlags3 &= ~PLAYER_STATE3_200;
                    this->stateFlags3 |= PLAYER_STATE3_1000000;
                    Player_PlayReactableSfx(this, NA_SE_IT_DEKUNUTS_FLOWER_OPEN);
                    func_8019FD90(4, 2);
                }
            }
        }

        this->actor.terminalVelocity = -10.0f;
        this->actor.gravity = -0.5f;
        Player_ResetCylinder(this);
    } else if (CHECK_BTN_ALL(sPlayerControlInput->press.button, BTN_A)) {
        func_808355D8(play, this, &gPlayerAnim_pn_kakkufinish);
    } else {
        s16 temp_a0;
        f32 temp_fv1; // sp78
        s16 sp76;
        s16 var_v1;
        s16 var_a1;
        f32 sp6C;
        f32 sp68;
        s16 sp66;
        s16 temp_ft0;
        s32 temp;
        s16 var_v1_4;

        this->linearVelocity = sqrtf(SQXZ(this->actor.velocity));
        if (this->linearVelocity != 0.0f) {
            var_a1 = Math_FAtan2F(this->actor.velocity.z, this->actor.velocity.x);

            temp_a0 = this->actor.shape.rot.y - var_a1;
            if (ABS_ALT(temp_a0) > 0x4000) {
                this->linearVelocity = -this->linearVelocity;
                var_a1 = var_a1 + 0x8000;
            }
            this->currentYaw = var_a1;
        }

        if (this->windSpeed != 0.0f) {
            Math_SmoothStepToS(&this->unk_B8C, this->windAngleX, 3, 0x1F40, 0x190);
        }

        func_8085687C(this);

        if (this->genericTimer != 0) {
            this->genericTimer--;
        }

        temp_fv1 = D_8085D958[this->genericVar] - Math_Vec3f_DistXZ(&this->actor.world.pos, this->unk_AF0);
        LinkAnimation_Update(play, &this->skelAnime);

        if ((this->genericTimer != 0) && (temp_fv1 > 300.0f)) {
            sp76 = 0x1770;
            if (this->skelAnime.animation != &gPlayerAnim_pn_kakku) {
                Player_ChangeAnimSlowedOnce(play, this, &gPlayerAnim_pn_kakkufinish);
            } else if (LinkAnimation_OnFrame(&this->skelAnime, 8.0f)) {
                s32 i;

                Player_TranslateAndRotateY(this, &this->actor.world.pos, &D_8085D960,
                                           &this->bodyPartsPos[PLAYER_BODYPART_L_HAND]);
                Player_TranslateAndRotateY(this, &this->actor.world.pos, &D_8085D96C,
                                           &this->bodyPartsPos[PLAYER_BODYPART_R_HAND]);

                for (i = 0; i < 13; i++) {
                    func_808566C0(play, this, PLAYER_BODYPART_L_HAND, 0.6f, 1.0f, 0.8f, 17);
                    func_808566C0(play, this, PLAYER_BODYPART_R_HAND, 0.6f, 1.0f, 0.8f, 17);
                }
            }
        } else if ((this->genericTimer == 0) || (temp_fv1 < 0.0f)) {
            sp76 = 0;
            func_808355D8(play, this, &gPlayerAnim_pn_rakkafinish);
        } else {
            sp76 = 0x1770 - (s32)((300.0f - temp_fv1) * 10.0f);

            if (this->skelAnime.animation != &gPlayerAnim_pn_batabata) {
                Player_ChangeAnimShortSlowedMorphLoop(play, this, &gPlayerAnim_pn_batabata);
            } else if (LinkAnimation_OnFrame(&this->skelAnime, 6.0f)) {
                Player_PlaySfx(this, NA_SE_PL_DEKUNUTS_STRUGGLE);
            }
        }

        Math_AsymStepToS(&this->unk_B86[1], sp76, 0x190, 0x190);

        this->unk_B8A += this->unk_B86[1];
        temp = ABS_ALT(this->unk_B86[1]);
        if (temp > 0xFA0) {
            this->unk_B66 += (u8)(ABS_ALT(this->unk_B86[1]) * 0.01f);
        }

        if (this->unk_B66 > 200) {
            this->unk_B66 -= 200;
            func_808566C0(play, this, PLAYER_BODYPART_L_HAND, 0.0f, 1.0f, 0.0f, 32);
            func_808566C0(play, this, PLAYER_BODYPART_R_HAND, 0.0f, 1.0f, 0.0f, 32);
        }

        func_8019FCB8(&this->actor.projectedPos, 0x1851, 2.0f * (this->unk_B86[1] * (1.0f / 6000.0f)));
        if ((this->boomerangActor == NULL) && CHECK_BTN_ALL(sPlayerControlInput->press.button, BTN_B)) {
            if (AMMO(ITEM_NUT) == 0) {
                play_sound(NA_SE_SY_ERROR);
            } else {
                this->boomerangActor =
                    Actor_Spawn(&play->actorCtx, play, ACTOR_EN_ARROW, this->bodyPartsPos[PLAYER_BODYPART_WAIST].x,
                                this->bodyPartsPos[PLAYER_BODYPART_WAIST].y,
                                this->bodyPartsPos[PLAYER_BODYPART_WAIST].z, -1, 0, 0, 8);
                if (this->boomerangActor != NULL) {
                    this->boomerangActor->velocity.x = this->actor.velocity.x * 1.5f;
                    this->boomerangActor->velocity.z = this->actor.velocity.z * 1.5f;
                    Inventory_ChangeAmmo(ITEM_NUT, -1);
                    Actor_PlaySfxAtPos(this->boomerangActor, NA_SE_PL_DEKUNUTS_DROP_BOMB);
                }
            }
        }

        if (this->actor.velocity.y < 0.0f) {
            if (sp76 != 0) {
                this->actor.terminalVelocity = -0.38f;
                this->actor.gravity = -0.2f;
            } else {
                this->actor.terminalVelocity = (this->unk_B86[1] * 0.0033f) + -20.0f;
                this->actor.gravity = (this->unk_B86[1] * 0.00004f) + (REG(68) / 100.0f);
            }
        }

        this->fallStartHeight = this->actor.world.pos.y;
        Player_GetTargetVelocityAndYaw(this, &sp6C, &sp66, 0.0f, play);
        if (sp6C == 0.0f) {
            sp68 = 0.1f;
        } else {
            s16 temp_v0_6 = this->currentYaw - sp66;

            if (ABS_ALT(temp_v0_6) > 0x4000) {
                sp6C = -sp6C;
                sp66 += 0x8000;
            }
            sp68 = 0.25f;
        }

        Math_SmoothStepToS(&this->unk_B8C, sp6C * 600.0f, 8, 0xFA0, 0x64);
        Math_ScaledStepToS(&this->currentYaw, sp66, 0xFA);

        temp_ft0 = BINANG_SUB(sp66, this->currentYaw) * -2.0f;
        temp_ft0 = CLAMP(temp_ft0, -0x1F40, 0x1F40);
        Math_SmoothStepToS(&this->unk_B8E, temp_ft0, 0x14, 0x320, 0x14);
        sp6C = (sp6C * (this->unk_B86[1] * 0.0004f)) * fabsf(Math_SinS(this->unk_B8C));
        func_80856888(&this->linearVelocity, sp6C, sp68);

        sp6C = sqrtf(SQ(this->linearVelocity) + SQ(this->actor.velocity.y));
        if (sp6C > 8.0f) {
            sp6C = 8.0f / sp6C;
            this->linearVelocity *= sp6C;
            this->actor.velocity.y *= sp6C;
        }
    }

    func_808378FC(play, this);
}

// Deku spinning related
void func_808573A4(Player* this, PlayState* play) {
    this->stateFlags2 |=
        PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING | PLAYER_STATE2_ALWAYS_DISABLE_MOVE_ROTATION;

    LinkAnimation_Update(play, &this->skelAnime);
    Player_SetCylinderForAttack(this, DMG_DEKU_SPIN, 1, 30);

    if (!Player_SetupItemCutsceneOrFirstPerson(this, play)) {
        s16 prevYaw = this->currentYaw;
        f32 sp40;
        s16 sp3E;

        Player_GetTargetVelocityAndYaw(this, &sp40, &sp3E, 0.018f, play);
        sp40 *= 1.0f - (0.9f * ((11100.0f - this->unk_B10[0]) / 11100.0f));
        if (!func_8083A4A4(this, &sp40, &sp3E, REG(43) / 100.0f)) {
            Player_SetRunVelocityAndYaw(this, sp40, sp3E);
        }

        this->unk_B10[0] += -800.0f;
        this->actor.shape.rot.y += BINANG_ADD((s16)this->unk_B10[0], BINANG_SUB(this->currentYaw, prevYaw));

        if (Math_StepToF(&this->unk_B10[1], 0.0f, this->unk_B10[0])) {
            this->actor.shape.rot.y = this->currentYaw;
            Player_EndMiniCsMovement(this, play);
        } else if (this->skelAnime.animation == &gPlayerAnim_pn_attack) {
            this->stateFlags3 |= PLAYER_STATE3_100000;

            if (this->unk_B10[1] < 0.0f) {
                Player_ChangeAnimMorphToLastFrame(play, this, Player_GetStandingStillAnim(this));
            }
        }

        func_808566C0(play, this, PLAYER_BODYPART_WAIST, 1.0f, 0.5f, 0.0f, 32);

        if (this->unk_B10[0] > 9500.0f) {
            func_8083F8A8(play, this, 2.0f, 1, 2.5f, 10, 18, true);
        }

        func_800AE930(&play->colCtx, Effect_GetByIndex(this->meleeWeaponEffectIndex[2]), &this->actor.world.pos, 2.0f,
                      this->currentYaw, this->actor.floorPoly, this->actor.floorBgId);
        func_800B8F98(&this->actor, Player_GetFloorSfx(this, NA_SE_PL_SLIP_LEVEL - SFX_FLAG));
    }
}

void func_80857640(Player* this, f32 arg1, s32 arg2) {
    Player_SetupJumpWithSfx(this, arg1, NA_SE_VO_LI_SWORD_N);
    Player_PlaySfx(this, NA_SE_PL_GORON_BALLJUMP);
    Player_StopHorizontalMovement(this);
    if (this->genericTimer < arg2) {
        this->genericTimer = arg2;
    }
    this->genericVar = 1;
    this->unk_B48 = 1.0f;
}

void func_808576BC(PlayState* play, Player* this) {
    s32 var_v0 = ((this->actor.velocity.z * Math_CosS(this->currentYaw)) +
                  (this->actor.velocity.x * Math_SinS(this->currentYaw))) *
                 800.0f;

    var_v0 -= this->genericTimer;
    var_v0 = ABS_ALT(var_v0);

    if (var_v0 <= 0x7D0) {
        return;
    }

    if (var_v0 > 0x1770) {
        func_800B8F98(&this->actor, NA_SE_PL_GORON_SLIP - SFX_FLAG);
    }

    if (func_8083F8A8(play, this, 12.0f, -1 - (var_v0 >> 0xC), (var_v0 >> 0xA) + 1.0f, (var_v0 >> 7) + 160, 20, true)) {
        Player_PlaySfx(this, (this->floorSfxOffset == NA_SE_PL_WALK_SNOW - SFX_FLAG)
                                 ? NA_SE_PL_ROLL_SNOW_DUST - SFX_FLAG
                                 : NA_SE_PL_ROLL_DUST - SFX_FLAG);
    }
}

void func_808577E0(Player* this) {
    f32 temp_fa1 = ABS_ALT(this->genericTimer) * 0.00004f;

    if (this->unk_ABC < temp_fa1) {
        this->unk_B48 += 0.08f;
    } else {
        this->unk_B48 += -0.07f;
    }

    this->unk_B48 = CLAMP(this->unk_B48, -0.2f, 0.14f);
    if (fabsf(this->unk_B48) < 0.12f) {
        if (Math_StepUntilF(&this->unk_ABC, temp_fa1, this->unk_B48)) {
            this->unk_B48 = 0.0f;
        }
    } else {
        this->unk_ABC += this->unk_B48;
        this->unk_ABC = CLAMP(this->unk_ABC, -0.7f, 0.3f);
    }
}

s32 func_80857950(PlayState* play, Player* this) {
    if (((this->unk_B86[1] == 0) && !CHECK_BTN_ALL(sPlayerControlInput->cur.button, BTN_A)) ||
        ((this->genericVar == 3) && (this->actor.velocity.y < 0.0f))) {
        Player_SetAction(play, this, Player_StandingStill, 1);
        Math_Vec3f_Copy(&this->actor.world.pos, &this->actor.prevPos);
        LinkAnimation_Change(play, &this->skelAnime, &gPlayerAnim_pg_maru_change, -2.0f / 3.0f, 7.0f, 0.0f,
                             ANIMMODE_ONCE, 0.0f);
        Player_PlaySfx(this, NA_SE_PL_BALL_TO_GORON);
        return true;
    }

    return false;
}

s32 func_80857A44(PlayState* play, Player* this) {
    if (LinkAnimation_Update(play, &this->skelAnime)) {
        Player_EndAnimMovement(this);

        this->actor.shape.shadowDraw = ActorShadow_DrawCircle;
        this->actor.bgCheckFlags |= 0x800;
        this->genericVar = 4;
        this->actor.shape.shadowScale = 30.0f;
        this->genericTimer = this->linearVelocity * 500.0f;
        this->unk_B08[0] = this->linearVelocity;
        this->unk_B08[1] = 0.0f;
        this->actor.home.rot.y = this->currentYaw;

        return true;
    }

    return false;
}

void func_80857AEC(PlayState* play, Player* this) {
    if (this->actor.bgCheckFlags & 2) {
        this->unk_B08[1] += this->unk_B08[0] * 0.05f;

        if (this->unk_B86[1] == 0) {
            if (this->genericVar == 1) {
                this->genericVar = 2;
                Player_RequestQuakeAndRumble(play, this, NA_SE_PL_GORON_PUNCH);
                play->actorCtx.unk2 = 4;
                EffectSsBlast_SpawnWhiteShockwave(play, &this->actor.world.pos, &gZeroVec3f, &gZeroVec3f);
                this->genericTimer = 0;
                this->unk_B08[0] = 0.0f;
                Actor_Spawn(&play->actorCtx, play, ACTOR_EN_TEST, this->actor.world.pos.x, this->actor.world.pos.y,
                            this->actor.world.pos.z, 0, 0, 0, 0);
            } else {
                this->genericVar = 4;
            }
        }

        Player_AnimSfx_PlayFloorLand(this);
    }
}

// At least Goron rolling
void func_80857BE8(Player* this, PlayState* play) {
    if (Player_SetupSubAction(play, this, D_8085D050, 0)) {
        return;
    }

    if ((this->genericVar == 0) && !func_80857A44(play, this)) {
        return;
    }

    this->stateFlags3 |= PLAYER_STATE3_1000;
    func_808577E0(this);

    if (!func_80857950(play, this)) {
        f32 spE4 = 0.0f;
        s16 spE2 = this->currentYaw;
        u16 spE0;
        s32 spDC;
        s32 spD8;

        if (Player_TryBonkning(play, this, this->unk_B08,
                               (this->doorType == PLAYER_DOORTYPE_STAIRCASE) ? 0.0f : 12.0f)) {
            if (func_80857BE8 == this->actionFunc) {
                this->linearVelocity *= 0.1f;
                Player_SetupJumpWithSfx(this, 10.0f, 0);
                if (this->unk_B86[1] != 0) {
                    this->unk_B86[1] = 0;
                    this->genericVar = 3;
                }
            } else {
                return;
            }
        } else if ((this->actor.bgCheckFlags & 8) && (this->unk_B08[0] >= 12.0f)) {
            s16 temp_v0 = this->currentYaw - BINANG_ADD(this->actor.wallYaw, 0x8000);
            s16 temp_v2;
            s32 var_a2 = ABS_ALT(temp_v0);

            this->unk_B08[1] += this->unk_B08[0] * 0.05f;
            temp_v2 = ((temp_v0 >= 0) ? 1 : -1) * ((var_a2 + 0x100) & ~0x1FF);
            this->currentYaw += BINANG_SUB(0x8000, (s16)(temp_v2 * 2));
            this->actor.home.rot.y = this->currentYaw;
            this->actor.shape.rot.y = this->currentYaw;

            this->unk_B8C = 4;
            Player_PlaySfx(this, NA_SE_IT_GORON_ROLLING_REFLECTION);
        }

        this->stateFlags2 |=
            (PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING | PLAYER_STATE2_ALWAYS_DISABLE_MOVE_ROTATION);

        if (this->unk_B8E != 0) {
            this->unk_B8E--;
        } else {
            Player_GetTargetVelocityAndYaw(this, &spE4, &spE2, 0.0f, play);
            spE4 *= 2.6f;
        }

        if (this->unk_B8C != 0) {
            this->unk_B8C--;
            spE2 = this->currentYaw;
        }

        if (this->unk_B86[1] != 0) {
            spE4 = 18.0f;
            Math_StepToC(&this->genericVar, 4, 1);

            if ((this->stateFlags3 & PLAYER_STATE3_80000) &&
                (!CHECK_BTN_ALL(sPlayerControlInput->cur.button, BTN_A) || (gSaveContext.save.playerData.magic == 0) ||
                 ((this->genericVar == 4) && (this->unk_B08[0] < 12.0f)))) {
                if (Math_StepToS(&this->unk_B86[1], 0, 1) != 0) {
                    this->stateFlags3 &= ~PLAYER_STATE3_80000;
                    Magic_Reset(play);
                    Player_PlaySfx(this, NA_SE_PL_GORON_BALL_CHARGE_FAILED);
                }
                this->genericVar = 4;
            } else if (this->unk_B86[1] < 7) {
                if (!(this->stateFlags3 & PLAYER_STATE3_80000)) {
                    this->unk_3D0.unk_00 = 2;
                }
                this->unk_B86[1]++;
            }
        }

        spDC = spE4 * 900.0f;

        Math_AsymStepToF(&this->unk_B10[0], (this->unk_B8A != 0) ? 1.0f : 0.0f, 0.8f, 0.05f);
        if (this->actor.bgCheckFlags & 1) {
            func_80857AEC(play, this);
            if (this->genericVar == 2) {
                if (this->unk_B8A == 0) {
                    this->genericVar = 4;
                } else {
                    this->unk_B8A--;
                    this->unk_ABC = 0.0f;
                    this->unk_B48 = 0.14f;
                }
            } else if ((this->unk_B86[1] == 0) && CHECK_BTN_ALL(sPlayerControlInput->press.button, BTN_B) &&
                       (Inventory_GetBtnBItem(play) < ITEM_FD)) {
                func_80857640(this, 14.0f, 0x1F40);
            } else {
                f32 spCC;
                s16 spCA;
                s16 spC8;
                s32 var_a0;
                s32 spC0;
                f32 spBC;
                f32 spB8;
                f32 spB4;
                f32 spB0;
                f32 spAC;
                f32 spA8;
                f32 spA4;
                f32 spA0;
                Vec3f slopeNormal;
                s16 downwardSlopeYaw;
                s16 sp90 = this->currentYaw;
                s16 sp8E = this->currentYaw - this->actor.home.rot.y;
                f32 sp88 = Math_CosS(sp8E);

                if (this->unk_B86[1] == 0) {
                    this->unk_B08[1] = 0.0f;
                    if (this->genericVar >= 0x36) {
                        Magic_Consume(play, 2, MAGIC_CONSUME_GORON_ZORA);
                        this->unk_B08[0] = 18.0f;
                        this->unk_B86[1] = 1;
                        this->stateFlags3 |= PLAYER_STATE3_80000;
                        Player_PlayReactableSfx(this, NA_SE_PL_GORON_BALL_CHARGE_DASH);
                    }
                } else {
                    this->unk_B08[1] = CLAMP(this->unk_B08[1], 0.0f, 0.9f);
                }

                spBC = this->unk_B08[0] * (1.0f - this->unk_B08[1]) * sp88;
                if ((spBC < 0.0f) || ((spE4 == 0.0f) && (ABS_ALT(sp8E) > 0xFA0))) {
                    spBC = 0.0f;
                }

                Math_StepToF(&this->unk_B08[1], 0.0f, fabsf(sp88) * 20.0f);
                var_a0 = spBC * 500.0f;
                var_a0 = CLAMP_MIN(var_a0, 0);

                spC0 = (s32)(spE4 * 400.0f) - var_a0;
                spC0 = CLAMP_MIN(spC0, 0);

                spDC = CLAMP_MIN(spDC, var_a0);

                spAC = spBC * Math_SinS(this->actor.home.rot.y);
                spA8 = spBC * Math_CosS(this->actor.home.rot.y);
                spB4 = this->unk_B08[0] * Math_SinS(this->currentYaw);
                spB0 = this->unk_B08[0] * Math_CosS(this->currentYaw);

                spA4 = spB4 - spAC;
                spA0 = spB0 - spA8;
                this->linearVelocity = spBC;
                this->currentYaw = this->actor.home.rot.y;
                spCC = spE4;
                spCA = spE2;

                if (func_8083A4A4(this, &spCC, &spCA, (this->genericVar >= 5) ? 0.0f : 1.0f)) {
                    if (this->unk_B86[1] == 0) {
                        this->genericVar = 4;
                    }

                    if (this->genericVar == 4) {
                        spDC = -0xFA0;
                    }
                } else {
                    static Vec3f D_8085D978 = { -30.0f, 60.0f, 0.0f };
                    static Vec3f D_8085D984 = { 30.0f, 60.0f, 0.0f };
                    f32 sp84 = (((this->floorSfxOffset == NA_SE_PL_WALK_SNOW - SFX_FLAG) ||
                                 (this->floorSfxOffset == NA_SE_PL_WALK_ICE - SFX_FLAG) ||
                                 (this->floorSfxOffset == NA_SE_PL_WALK_SAND - SFX_FLAG) ||
                                 (sPlayerCurrentFloorType == BG_FLOOR_TYPE_5)) &&
                                (spC0 >= 0x7D0))
                                   ? 0.08f
                                   : this->genericTimer * 0.0003f;
                    f32 sp80 = (Math_SinS(this->angleToFloorX) * 8.0f) + 0.6f;
                    s16 var_a3;
                    s16 sp7C;
                    Vec3f sp70;
                    f32 sp6C;
                    f32 var_fa1;

                    if (this->unk_B86[1] == 0) {
                        if ((gSaveContext.magicState == 0) && (gSaveContext.save.playerData.magic >= 2) &&
                            (this->genericTimer >= 0x36B0)) {
                            this->genericVar += 1;
                            func_800B8F98(&this->actor, NA_SE_PL_GORON_BALL_CHARGE - SFX_FLAG);
                        } else {
                            this->genericVar = 4;
                        }
                    }

                    if (spE4 != spCC) {
                        this->currentYaw = spE2;
                    }

                    sp84 = CLAMP_MIN(sp84, 0.0f);
                    sp80 = CLAMP_MIN(sp80, 0.0f);

                    Math_AsymStepToF(&this->linearVelocity, spE4, sp84, sp80);
                    spC8 = (s16)(fabsf(this->actor.speedXZ) * 20.0f) + 300;
                    spC8 = CLAMP_MIN(spC8, 100);

                    sp7C = (s32)(BINANG_SUB(spE2, this->currentYaw) * -0.5f);
                    this->unk_B08[1] += (f32)(SQ(sp7C)) * 8e-9f;
                    Math_ScaledStepToS(&this->currentYaw, spE2, spC8);
                    sp6C = func_80835D2C(play, this, &D_8085D978, &sp70);

                    var_fa1 = func_80835D2C(play, this, &D_8085D984, &sp70) - sp6C;
                    if (fabsf(var_fa1) > 100.0f) {
                        var_fa1 = 0.0f;
                    }

                    var_a3 = Math_FAtan2F(60.0f, var_fa1);
                    if (ABS_ALT(var_a3) > 0x2AAA) {
                        var_a3 = 0;
                    }

                    Math_ScaledStepToS(&this->actor.shape.rot.z, var_a3 + sp7C, spC8);
                }

                spBC = this->linearVelocity;
                this->actor.home.rot.y = this->currentYaw;
                this->currentYaw = sp90;
                Actor_GetSlopeDirection(this->actor.floorPoly, &slopeNormal, &downwardSlopeYaw);

                spB8 = sqrtf(SQ(spA4) + SQ(spA0));
                if (this->unk_B86[1] != 0) {
                    if ((ABS_ALT(sp8E) + ABS_ALT(this->angleToFloorX)) > 15000) {
                        this->unk_B86[1] = 0;
                        this->genericVar = 4;
                        this->unk_B8E = 0x14;
                        this->genericTimer = 0;
                        this->stateFlags3 &= ~PLAYER_STATE3_80000;
                        Magic_Reset(play);
                    }
                } else {
                    f32 temp_ft4_2 = (0.6f * slopeNormal.x) + spA4;
                    f32 temp_ft5 = (0.6f * slopeNormal.z) + spA0;
                    f32 temp_fv0_3 = sqrtf(SQ(temp_ft4_2) + SQ(temp_ft5));

                    if ((temp_fv0_3 < spB8) || (temp_fv0_3 < 6.0f)) {
                        spA4 = temp_ft4_2;
                        spA0 = temp_ft5;
                        spB8 = temp_fv0_3;
                    }
                }

                if (spB8 != 0.0f) {
                    f32 pad;
                    f32 sp54 = spB8 - 0.3f;

                    sp54 = CLAMP_MIN(sp54, 0.0f);

                    spB8 = sp54 / spB8;

                    spA4 *= spB8;
                    spA0 *= spB8;

                    if (sp54 != 0.0f) {
                        this->stickFlameTimer = Math_FAtan2F(spA0, spA4);
                    }

                    if (this->genericTimer == 0) {
                        s32 temp_v0_10 = this->unk_B86[0];
                        s32 temp_ft3_2 = sp54 * 800.0f;

                        this->unk_B86[0] += (s16)temp_ft3_2;
                        if ((this->actor.bgCheckFlags & 1) && (temp_ft3_2 != 0) &&
                            (((temp_v0_10 + temp_ft3_2) * temp_v0_10) <= 0)) {
                            spE0 = Player_GetFloorSfx(this, NA_SE_PL_GORON_ROLL);
                            func_8019F780(&this->actor.projectedPos, spE0, sp54);
                        }
                    }
                }

                spAC = Math_SinS(this->actor.home.rot.y) * spBC;
                spA8 = Math_CosS(this->actor.home.rot.y) * spBC;

                spB4 = spAC + spA4;
                spB0 = spA8 + spA0;

                this->unk_B08[0] = sqrtf(SQ(spB4) + SQ(spB0));
                this->unk_B08[0] = CLAMP_MAX(this->unk_B08[0], 18.0f);

                this->currentYaw = Math_FAtan2F(spB0, spB4);
            }

            func_808576BC(play, this);

            if (ABS_ALT(this->genericTimer) > 0xFA0) {
                this->stateFlags2 |= PLAYER_STATE2_8;
            }

            if (this->actor.bgCheckFlags & 1) {
                this->linearVelocity = this->unk_B08[0] * Math_CosS(this->angleToFloorX);
                this->actor.velocity.y = this->unk_B08[0] * Math_SinS(this->angleToFloorX);
            }

            if ((this->unk_B86[1] != 0) ||
                func_800C9C24(&play->colCtx, this->actor.floorPoly, this->actor.floorBgId, 1)) {
                func_800AE930(&play->colCtx, Effect_GetByIndex(this->meleeWeaponEffectIndex[2]), &this->actor.world.pos,
                              15.0f, this->actor.shape.rot.y, this->actor.floorPoly, this->actor.floorBgId);
            } else {
                func_800AEF44(Effect_GetByIndex(this->meleeWeaponEffectIndex[2]));
            }
        } else {
            Math_ScaledStepToS(&this->actor.shape.rot.z, 0, 0x190);

            this->unk_B86[0] = 0;
            if (this->unk_B86[1] != 0) {
                this->actor.gravity = -1.0f;
                Math_ScaledStepToS(&this->actor.home.rot.y, spE2, 0x190);

                this->unk_B08[0] = sqrtf(SQ(this->linearVelocity) + SQ(this->actor.velocity.y)) *
                                   ((this->linearVelocity >= 0.0f) ? 1.0f : -1.0f);
                this->unk_B08[0] = CLAMP_MAX(this->unk_B08[0], 18.0f);
            } else {
                this->unk_B48 += this->actor.velocity.y * 0.005f;
                if (this->genericVar == 1) {
                    if (this->actor.velocity.y > 0.0f) {
                        if ((this->actor.velocity.y + this->actor.gravity) < 0.0f) {
                            this->actor.velocity.y = -this->actor.gravity;
                        }
                    } else {
                        this->unk_B8A = 0xA;
                        if (this->actor.velocity.y > -1.0f) {
                            this->actor.gravity = -0.2f;
                        } else {
                            this->unk_3D0.unk_00 = 1;
                            this->actor.gravity = -10.0f;
                        }
                    }
                }
                this->unk_B08[0] = this->linearVelocity;
            }

            func_800AEF44(Effect_GetByIndex(this->meleeWeaponEffectIndex[2]));
        }

        Math_ScaledStepToS(&this->actor.shape.rot.y, this->actor.home.rot.y, 0x7D0);

        Math_AsymStepToS(&this->genericTimer, spDC, (spDC >= 0) ? 0x7D0 : 0x3E8, 0x4B0);

        if (this->genericTimer != 0) {
            spD8 = this->actor.shape.rot.x;
            this->actor.shape.rot.x += this->genericTimer;

            Math_ScaledStepToS(&this->unk_B86[0], 0, ABS_ALT(this->genericTimer));
            if ((this->actor.bgCheckFlags & 1) && (((this->genericTimer + spD8) * spD8) <= 0)) {
                spE0 =
                    Player_GetFloorSfx(this, (this->unk_B86[1] != 0) ? NA_SE_PL_GORON_CHG_ROLL : NA_SE_PL_GORON_ROLL);
                func_8019F780(&this->actor.projectedPos, spE0, this->unk_B08[0]);
            }
        }

        if (this->genericVar == 2) {
            Player_SetCylinderForAttack(this, DMG_GORON_POUND, 4, 60);
            func_800B648C(play, 0, 2, 100.0f, &this->actor.world.pos);
        } else if (this->unk_B86[1] != 0) {
            Player_SetCylinderForAttack(this, DMG_GORON_SPIKES, 1, 25);
        } else {
            Player_SetCylinderForAttack(this, DMG_NORMAL_ROLL, 1, 25);
        }
    }
}

void func_80858C84(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    Player_ClearRootLimbPosY(this);
    Player_ChangeAnimMorphToLastFrame(play, this, anim);
    Player_StopHorizontalMovement(this);
}

void func_80858CC8(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    Player_ClearRootLimbPosY(this);
    LinkAnimation_Change(play, &this->skelAnime, anim, 2.0f / 3.0f, 0.0f, Animation_GetLastFrame(anim), ANIMMODE_ONCE,
                         -8.0f);
    Player_StopHorizontalMovement(this);
}

void func_80858D48(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    Player_ClearRootLimbPosY(this);
    LinkAnimation_Change(play, &this->skelAnime, anim, 2.0f / 3.0f, 0.0f, 0.0f, ANIMMODE_LOOP, -8.0f);
    Player_StopHorizontalMovement(this);
}

void func_80858DB4(PlayState* play, Player* this, void* anim) {
    Player_StopHorizontalMovement(this);
}

void func_80858DDC(PlayState* play, Player* this, void* anim) {
    func_80858C84(play, this, anim);
}

void func_80858DFC(PlayState* play, Player* this, void* anim) {
    Player_ClearRootLimbPosY(this);
    Player_ChangeAnimOnce(play, this, anim);
    Player_StopHorizontalMovement(this);
}

void func_80858E40(PlayState* play, Player* this, void* anim) {
    func_80858CC8(play, this, anim);
}

void func_80858E60(PlayState* play, Player* this, void* anim) {
    func_80858D48(play, this, anim);
}

void func_80858E80(PlayState* play, Player* this, void* anim) {
    Player_PlayAnimOnceWithMovementPresetFlagsSlowed(play, this, anim);
}

void func_80858EA0(PlayState* play, Player* this, void* anim) {
    Player_PlayAnimOnceWithMovement(play, this, anim, 4 | 8 | ANIM_FLAG_NOMOVE | 0x80);
}

void func_80858EC0(PlayState* play, Player* this, void* anim) {
    Player_AnimationPlayOnceReverse(play, this, anim);
    Player_SetupAnimMovement(play, this, 4 | 8 | ANIM_FLAG_NOMOVE | 0x80);
}

void func_80858EFC(PlayState* play, Player* this, void* anim) {
    Player_PlayAnimLoopWithMovementPresetFlagsSlowed(play, this, anim);
}

void func_80858F1C(PlayState* play, Player* this, void* anim) {
    Player_PlayAnimLoopWithMovement(play, this, anim, 4 | 8 | ANIM_FLAG_NOMOVE | 0x80);
}

void func_80858F3C(PlayState* play, Player* this, void* anim) {
    Player_AnimationPlayOnce(play, this, anim);
}

void func_80858F5C(PlayState* play, Player* this, void* anim) {
    Player_AnimationPlayLoop(play, this, anim);
}

void func_80858F7C(PlayState* play, Player* this, void* anim) {
    Player_PlayAnimOnceSlowed(play, this, anim);
}

void func_80858F9C(PlayState* play, Player* this, void* anim) {
    func_8082DB60(play, this, anim);
}

void func_80858FBC(PlayState* play, Player* this, void* anim) {
    LinkAnimation_Update(play, &this->skelAnime);
}

void func_80858FE8(Player* this) {
    if (this->skelAnime.animation == &gPlayerAnim_lost_horse_wait) {
        Player_AnimSfx_PlayFloor(this, NA_SE_PL_SLIP_LEVEL - SFX_FLAG);
        Player_PlaySfx(this, NA_SE_VO_LK_DRAGGED_DAMAGE - SFX_FLAG);
    }
}

void (*D_8085D990[])(PlayState*, Player*, void*) = {
    /*  0 */ NULL,
    /*  1 */ func_80858DB4,
    /*  2 */ func_80858DDC,
    /*  3 */ func_80858E40,
    /*  4 */ func_80858E60,
    /*  5 */ func_80858E80,
    /*  6 */ func_80858EA0,
    /*  7 */ func_80858EFC,
    /*  8 */ func_80858F1C,
    /*  9 */ func_80858F3C,
    /* 10 */ func_80858F5C,
    /* 11 */ func_80858FBC,
    /* 12 */ func_80859028,
    /* 13 */ func_808591BC,
    /* 14 */ func_80858DFC,
    /* 15 */ func_80858F7C,
    /* 16 */ func_80858F9C,
    /* 17 */ func_80859168,
    /* 18 */ func_80859210,
    /* 19 */ func_80858EC0,
};

AnimSfxEntry D_8085D9E0[] = {
    ANIMSFX(ANIMSFX_TYPE_FLOOR_LAND, 34, 0, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 45, NA_SE_PL_CALM_HIT, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 51, NA_SE_PL_CALM_HIT, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 64, NA_SE_PL_CALM_HIT, STOP),
};
AnimSfxEntry D_8085D9F0[] = {
    ANIMSFX(ANIMSFX_TYPE_VOICE, 7, NA_SE_VO_LI_DEMO_DAMAGE, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_FLOOR, 18, NA_SE_PL_BOUND, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_VOICE, 18, NA_SE_VO_LI_FREEZE, STOP),
};
AnimSfxEntry D_8085D9FC[] = {
    ANIMSFX(ANIMSFX_TYPE_FLOOR_BY_AGE, 14, NA_SE_PL_LAND_GROUND, STOP),
};
AnimSfxEntry D_8085DA00[] = {
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 6, NA_SE_PL_GET_UP, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 18, NA_SE_VO_LK_WAKE_UP, STOP),
};
AnimSfxEntry D_8085DA08[] = {
    ANIMSFX(ANIMSFX_TYPE_FLOOR_BY_AGE, 26, NA_SE_PL_LAND_GROUND, STOP),
};
AnimSfxEntry D_8085DA0C[] = {
    ANIMSFX(ANIMSFX_TYPE_8, 16, 0, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_FLOOR_JUMP, 36, 0, STOP),
};
AnimSfxEntry D_8085DA14[] = {
    ANIMSFX(ANIMSFX_TYPE_FLOOR_JUMP, 55, 0, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 55, NA_SE_VO_LK_CATCH_DEMO, STOP),
};
AnimSfxEntry D_8085DA1C[] = {
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 4, NA_SE_VO_LK_USING_UP_ENERGY, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_FLOOR, 16, NA_SE_PL_BOUND, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_VOICE, 16, NA_SE_VO_LI_DAMAGE_S, STOP),
};
AnimSfxEntry D_8085DA28[] = {
    ANIMSFX(ANIMSFX_TYPE_FLOOR_BY_AGE, 28, NA_SE_PL_LAND_GROUND, STOP),
};
AnimSfxEntry D_8085DA2C[] = {
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 1, NA_SE_VO_LK_USING_UP_ENERGY, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_FLOOR_JUMP, 42, 0, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_VOICE, 44, NA_SE_VO_LI_FALL_L, STOP),
};
AnimSfxEntry D_8085DA38[] = {
    ANIMSFX(ANIMSFX_TYPE_FLOOR, 1, NA_SE_PL_BOUND, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_VOICE, 1, NA_SE_VO_LI_DAMAGE_S, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_FLOOR_BY_AGE, 39, NA_SE_PL_LAND_GROUND, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_FLOOR_LAND, 49, 0, STOP),
};

// gPlayerAnim_cl_nigeru
AnimSfxEntry D_8085DA48[] = {
    ANIMSFX(ANIMSFX_TYPE_6, 1, 0, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_6, 5, 0, STOP),
};
AnimSfxEntry D_8085DA50[] = {
    ANIMSFX(ANIMSFX_TYPE_6, 10, 0, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_6, 13, 0, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_6, 16, 0, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_6, 19, 0, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_6, 22, 0, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_FLOOR, 22, NA_SE_PL_SLIP, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_VOICE, 55, NA_SE_VO_LI_DAMAGE_S, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_FLOOR_LAND, 62, 0, STOP),
};

AnimSfxEntry D_8085DA70[] = {
    ANIMSFX(ANIMSFX_TYPE_6, 42, 0, CONTINUE),
    ANIMSFX(ANIMSFX_TYPE_6, 48, 0, STOP),
};
AnimSfxEntry D_8085DA78[] = {
    ANIMSFX(ANIMSFX_TYPE_FLOOR, 2, NA_SE_PL_BOUND, STOP),
};
AnimSfxEntry D_8085DA7C[] = {
    ANIMSFX(ANIMSFX_TYPE_VOICE, 5, NA_SE_VO_LI_FREEZE, STOP),
};
AnimSfxEntry D_8085DA80[] = {
    ANIMSFX(ANIMSFX_TYPE_VOICE, 1, NA_SE_VO_LI_FALL_L, STOP),
};
AnimSfxEntry D_8085DA84[] = {
    ANIMSFX(ANIMSFX_TYPE_VOICE, 13, NA_SE_VO_LI_HANG, STOP),
};
AnimSfxEntry D_8085DA88[] = {
    ANIMSFX(ANIMSFX_TYPE_FLOOR_LAND, 26, 0, STOP),
};
AnimSfxEntry D_8085DA8C[] = {
    ANIMSFX(ANIMSFX_TYPE_VOICE, 4, NA_SE_VO_LI_SURPRISE, STOP),
};
AnimSfxEntry D_8085DA90[] = {
    ANIMSFX(ANIMSFX_TYPE_GENERAL, 18, NA_SE_PL_SIT_ON_HORSE, STOP),
};

void func_80859028(PlayState* play, Player* this, void* anim) {
    if (LinkAnimation_Update(play, &this->skelAnime)) {
        func_80858D48(play, this, anim);
        this->genericTimer = 1;
    }

    if (this->skelAnime.animation == &gPlayerAnim_okiagaru_tatu) {
        Player_PlayAnimSfx(this, D_8085DA08);
    } else if (this->skelAnime.animation == &gPlayerAnim_lost_horse) {
        Player_PlayAnimSfx(this, D_8085DA14);
    } else if (this->skelAnime.animation == &gPlayerAnim_sirimochi) {
        Player_PlayAnimSfx(this, D_8085DA38);
    } else if (this->skelAnime.animation == &gPlayerAnim_alink_somukeru) {
        Player_PlayAnimSfx(this, D_8085DA7C);
    } else if (this->skelAnime.animation == &gPlayerAnim_al_fuwafuwa) {
        Player_PlayAnimSfx(this, D_8085DA84);
    } else if (this->skelAnime.animation == &gPlayerAnim_cl_umanoru) {
        Player_PlayAnimSfx(this, D_8085DA90);
    } else {
        func_80858FE8(this);
    }
}

void func_80859168(PlayState* play, Player* this, void* anim) {
    if (LinkAnimation_Update(play, &this->skelAnime)) {
        Player_EndAnimMovement(this);
        func_8082DB60(play, this, anim);
    }
}

void func_808591BC(PlayState* play, Player* this, void* anim) {
    if (LinkAnimation_Update(play, &this->skelAnime)) {
        Player_PlayAnimLoopWithMovementPresetFlagsSlowed(play, this, anim);
        this->genericTimer = 1;
    }
}

void func_80859210(PlayState* play, Player* this, void* arg2) {
    LinkAnimation_Update(play, &this->skelAnime);
    Player_PlayAnimSfx(this, arg2);
}

void func_80859248(Player* this) {
    if ((this->unk_398 == NULL) || (this->unk_398->update == NULL)) {
        this->unk_398 = NULL;
    }
    this->targetedActor = this->unk_398;
    if (this->unk_398 != NULL) {
        this->actor.shape.rot.y = Player_LookAtTargetActor(this, 0);
    }
}

void func_8085929C(PlayState* play, Player* this, UNK_TYPE arg2) {
    this->stateFlags1 |= PLAYER_STATE1_SWIMMING;
    this->stateFlags2 |= PLAYER_STATE2_DIVING;
    this->stateFlags1 &= ~(PLAYER_STATE1_JUMPING | PLAYER_STATE1_FREEFALLING);
    Player_AnimationPlayLoop(play, this, &gPlayerAnim_link_swimer_swim);
    this->linearVelocity = 0.0f;
}

void func_80859300(PlayState* play, Player* this, UNK_TYPE arg2) {
    this->actor.gravity = 0.0f;

    if (this->genericVar == 0) {
        if ((this->transformation == PLAYER_FORM_DEKU) || Player_SetupDive(play, this, NULL)) {
            this->genericVar = 1;
        } else {
            Player_PlaySwimAnim(play, this, NULL, fabsf(this->actor.velocity.y));
            Math_ScaledStepToS(&this->unk_AAA, -0x2710, 0x320);
            Player_UpdateSwimMovement(this, &this->actor.velocity.y, 4.0f, this->currentYaw);
        }
    } else {
        if (LinkAnimation_Update(play, &this->skelAnime)) {
            if (this->genericVar == 1) {
                Player_ChangeAnimLongMorphLoop(play, this, &gPlayerAnim_link_swimer_swim_wait);
            } else {
                Player_AnimationPlayLoop(play, this, &gPlayerAnim_link_swimer_swim_wait);
            }
        }
        Player_SetVerticalWaterVelocity(this);
        Player_UpdateSwimMovement(this, &this->linearVelocity, 0.0f, this->actor.shape.rot.y);
    }
}

void func_80859414(PlayState* play, Player* this, UNK_TYPE arg2) {
    func_80859248(this);

    if (Player_IsSwimming(this)) {
        func_80859300(play, this, 0);
    } else {
        LinkAnimation_Update(play, &this->skelAnime);
        if (Player_IsShootingHookshot(this) || (this->stateFlags1 & PLAYER_STATE1_HOLDING_ACTOR)) {
            Player_SetupCurrentUpperAction(this, play);
        } else if ((this->interactRangeActor != NULL) && (this->interactRangeActor->textId == 0xFFFF)) {
            Player_SetupGetItemOrHoldBehavior(this, play);
        }
    }
}

void func_808594D0(PlayState* play, Player* this, CsCmdActorAction* playerActionCsCmd) {
    if (this->stateFlags1 & PLAYER_STATE1_SWIMMING) {
        func_8085929C(play, this, 0);
    } else {
        LinkAnimationHeader* anim = D_8085BE84[PLAYER_ANIMGROUP_43][this->modelAnimType];

        if ((this->unk_396 == 6) || (this->unk_396 == 0x2E)) {
            Player_AnimationPlayOnce(play, this, anim);
        } else {
            Player_ClearRootLimbPosY(this);
            LinkAnimation_Change(play, &this->skelAnime, anim, 2.0f / 3.0f, 0.0f, Animation_GetLastFrame(anim),
                                 ANIMMODE_LOOP, -4.0f);
        }
        Player_StopHorizontalMovement(this);
    }
}

void func_808595B8(PlayState* play, Player* this, CsCmdActorAction* playerActionCsCmd) {
    if (Player_SetupShootingGalleryPlay(play, this)) {
        return;
    }

    if ((this->csMode == PLAYER_CSMODE_20) && (play->csCtx.state == CS_STATE_IDLE)) {
        func_800B7298(play, NULL, PLAYER_CSMODE_6);
    } else if (this->stateFlags1 & PLAYER_STATE1_SWIMMING) {
        func_80859300(play, this, 0);
        this->actor.velocity.y = 0.0f;
    } else {
        LinkAnimation_Update(play, &this->skelAnime);
        if (Player_IsShootingHookshot(this) || (this->stateFlags1 & PLAYER_STATE1_HOLDING_ACTOR)) {
            Player_SetupCurrentUpperAction(this, play);
        }
    }
}

void func_8085968C(PlayState* play, Player* this, UNK_TYPE arg2) {
    if (this->actor.id == ACTOR_EN_TEST3) {
        Player_LoadGetItemObject(this, OBJECT_GI_MSSA);
        this->stateFlags1 |= PLAYER_STATE1_GETTING_ITEM;
    }

    Player_PlayAnimOnceSlowed(play, this,
                              (this->transformation == PLAYER_FORM_DEKU) ? &gPlayerAnim_pn_getA
                                                                         : &gPlayerAnim_link_demo_get_itemA);
}

void func_80859708(PlayState* play, Player* this, UNK_TYPE arg2) {
    LinkAnimation_Update(play, &this->skelAnime);
    if ((this->actor.id == ACTOR_EN_TEST3) && Animation_OnFrame(&this->skelAnime, 20.0f)) {
        this->getItemDrawId = GID_MASK_BLAST;
        func_80151BB4(play, 0x1B);
        Audio_PlayFanfare(0x37);
    }
}

void func_8085978C(PlayState* play, Player* this, UNK_TYPE arg2) {
    f32 linearVelocity;
    s16 yaw;

    this->stateFlags1 &= ~PLAYER_STATE1_2000000;

    yaw = Math_Vec3f_Yaw(&this->actor.world.pos, &this->unk_3A0);
    linearVelocity = this->linearVelocity;
    this->actor.world.rot.y = yaw;
    this->actor.shape.rot.y = yaw;
    this->currentYaw = yaw;
    if (linearVelocity <= 0.0f) {
        this->linearVelocity = 0.1f;
    } else if (linearVelocity > 2.5f) {
        this->linearVelocity = 2.5f;
    }

    if ((this->transformation != PLAYER_FORM_HUMAN) && (play->roomCtx.curRoom.unk3 == 5)) {
        MREG(64) = 0x2D;
        MREG(65) = 0xFF;
        MREG(66) = 0xFF;
        MREG(67) = 0xFF;
        MREG(68) = 0;
        play_sound(NA_SE_SY_WHITE_OUT_T);
    }
}

void func_80859890(PlayState* play, Player* this, UNK_TYPE arg2) {
    f32 sp24;
    s16 temp_v1 = MREG(64);

    if (temp_v1 > 0) {
        MREG(68) += temp_v1;
        if (MREG(68) >= 0x100) {
            MREG(64) = -0x40;
            MREG(68) = 0xFF;
            gSaveContext.save.playerForm = PLAYER_FORM_HUMAN;
            this->actor.update = func_8012301C;
            this->actor.draw = NULL;
            this->genericVar = 0;
        }
    } else if (temp_v1 < 0) {
        MREG(68) += temp_v1;
        if (MREG(68) < 0) {
            MREG(64) = 0;
            MREG(68) = 0;
        }
    } else {
        sp24 = 2.5f;
        func_808411D4(play, this, &sp24, 0xA);
        this->genericTimer++;
        if (this->genericTimer >= 0x15) {
            this->csMode = PLAYER_CSMODE_10;
        }
    }
}

void func_80859990(PlayState* play, Player* this, UNK_TYPE arg2) {
    this->linearVelocity = 2.5f;
    Player_GetWorldPosRelativeToPlayer(&this->actor.world.pos, this->actor.shape.rot.y, 180.0f, &this->unk_3A0);
}

void func_808599DC(PlayState* play, Player* this, UNK_TYPE arg2) {
    f32 sp1C;

    sp1C = 2.5f;
    func_808411D4(play, this, &sp1C, 0xA);
}

void func_80859A10(PlayState* play, Player* this, UNK_TYPE arg2) {
    Player_SetupUnfriendlyZTarget(this, play);
}

void func_80859A44(PlayState* play, Player* this, UNK_TYPE arg2) {
    func_80859248(this);
    if (this->genericTimer != 0) {
        if (LinkAnimation_Update(play, &this->skelAnime)) {
            Player_AnimationPlayLoop(play, this, Player_GetFightingRightAnim(this));
            this->genericTimer = 0;
        }
        func_8082FC60(this);
    } else {
        func_8083E958(play, this);
    }
}

void func_80859AD0(PlayState* play, Player* this, UNK_PTR arg2) {
    func_80840F90(play, this, arg2, 0.0f, 0, 0);
}

void func_80859AF8(PlayState* play, Player* this, UNK_PTR arg2) {
    this->actor.shape.face = 0xF;
    func_80840F90(play, this, arg2, 0.0f, 0, 0);
}

void func_80859B28(PlayState* play, Player* this, UNK_PTR arg2) {
    func_80840F90(play, this, arg2, 0.0f, 0, 1);
}

void func_80859B54(PlayState* play, Player* this, UNK_TYPE arg2) {
    func_80858CC8(play, this, &gPlayerAnim_link_normal_okarina_start);
    this->heldItemActionParam = PLAYER_AP_OCARINA;
    Player_SetModels(this, Player_ActionToModelGroup(this, this->heldItemActionParam));
}

void Player_Cutscene_Translate(PlayState* play, Player* this, CsCmdActorAction* playerAction) {
    f32 startX = playerAction->startPos.x;
    f32 startY = playerAction->startPos.y;
    f32 startZ = playerAction->startPos.z;
    f32 diffX = playerAction->endPos.x - startX;
    f32 diffY = playerAction->endPos.y - startY;
    f32 diffZ = playerAction->endPos.z - startZ;
    f32 progress = (((f32)(play->csCtx.frames - playerAction->startFrame)) /
                    ((f32)(playerAction->endFrame - playerAction->startFrame)));

    this->actor.world.pos.x = (diffX * progress) + startX;
    this->actor.world.pos.y = (diffY * progress) + startY;
    this->actor.world.pos.z = (diffZ * progress) + startZ;
}

void func_80859C60(PlayState* play, Player* this, void* arg2) {
    if (arg2 != NULL) {
        Player_Cutscene_Translate(play, this, arg2);
    }

    LinkAnimation_Update(play, &this->skelAnime);
}

void func_80859CA0(PlayState* play, Player* this, s32 arg2) {
    Player_ChangeAnimShortMorphLoop(play, this, D_8085BE84[PLAYER_ANIMGROUP_43][this->modelAnimType]);
    Player_StopHorizontalMovement(this);
}

void func_80859CE0(PlayState* play, Player* this, s32 arg2) {
    this->actor.draw = Player_Draw;
}

void func_80859CFC(PlayState* play, Player* this, UNK_TYPE arg2) {
    Player_CutsceneDrawSword(play, this, false);
    Player_PlayAnimOnceSlowed(play, this, &gPlayerAnim_link_demo_return_to_past);
}

void func_80859D44(PlayState* play, Player* this, UNK_TYPE arg2) {
    LinkAnimation_Update(play, &this->skelAnime);
}

struct_8085DA94 sCutsceneModeInitFuncs[PLAYER_CSMODE_MAX] = {
    /* PLAYER_CSMODE_0   */ { 0, { NULL } },
    /* PLAYER_CSMODE_1   */ { -1, { func_808594D0 } },
    /* PLAYER_CSMODE_2   */ { 0, { NULL } },
    /* PLAYER_CSMODE_3   */ { 0, { NULL } },
    /* PLAYER_CSMODE_4   */ { 3, { &gPlayerAnim_link_demo_bikkuri } },
    /* PLAYER_CSMODE_5   */ { 0, { NULL } },
    /* PLAYER_CSMODE_6   */ { 0, { NULL } },
    /* PLAYER_CSMODE_7   */ { -1, { func_808594D0 } },
    /* PLAYER_CSMODE_8   */ { 2, { &gPlayerAnim_link_demo_furimuki } },
    /* PLAYER_CSMODE_9   */ { -1, { func_8085978C } },
    /* PLAYER_CSMODE_10  */ { 3, { &gPlayerAnim_link_demo_warp } },
    /* PLAYER_CSMODE_11  */ { 5, { &gPlayerAnim_clink_demo_standup } },
    /* PLAYER_CSMODE_12  */ { 7, { &gPlayerAnim_clink_demo_standup_wait } },
    /* PLAYER_CSMODE_13  */ { 2, { &gPlayerAnim_link_demo_baru_op3 } },
    /* PLAYER_CSMODE_14  */ { 0, { NULL } },
    /* PLAYER_CSMODE_15  */ { 3, { &gPlayerAnim_link_demo_jibunmiru } },
    /* PLAYER_CSMODE_16  */ { -1, { func_80859B54 } },
    /* PLAYER_CSMODE_17  */ { 2, { &gPlayerAnim_link_normal_okarina_end } },
    /* PLAYER_CSMODE_18  */ { 16, { &gPlayerAnim_link_normal_hang_up_down } },
    /* PLAYER_CSMODE_19  */ { -1, { func_80859CA0 } },
    /* PLAYER_CSMODE_20  */ { -1, { func_808594D0 } },
    /* PLAYER_CSMODE_21  */ { 3, { &gPlayerAnim_clink_demo_mimawasi } },
    /* PLAYER_CSMODE_22  */ { 6, { &gPlayerAnim_om_get_mae } },
    /* PLAYER_CSMODE_23  */ { 3, { &gPlayerAnim_link_demo_look_hand } },
    /* PLAYER_CSMODE_24  */ { 3, { &gPlayerAnim_link_normal_wait_typeB_20f } },
    /* PLAYER_CSMODE_25  */ { -1, { func_80859CFC } },
    /* PLAYER_CSMODE_26  */ { -1, { func_8085A66C } },
    /* PLAYER_CSMODE_27  */ { 3, { &gPlayerAnim_link_demo_zeldamiru } },
    /* PLAYER_CSMODE_28  */ { 3, { &gPlayerAnim_link_demo_kenmiru1 } },
    /* PLAYER_CSMODE_29  */ { 3, { &gPlayerAnim_link_demo_kenmiru2 } },
    /* PLAYER_CSMODE_30  */ { 3, { &gPlayerAnim_link_demo_kenmiru2_modori } },
    /* PLAYER_CSMODE_31  */ { 6, { &gameplay_keep_Linkanim_00D310 } },
    /* PLAYER_CSMODE_32  */ { -1, { func_80859FCC } },
    /* PLAYER_CSMODE_33  */ { 3, { &gPlayerAnim_demo_rakka } },
    /* PLAYER_CSMODE_34  */ { 4, { &gPlayerAnim_demo_pikupiku } },
    /* PLAYER_CSMODE_35  */ { 3, { &gameplay_keep_Linkanim_00D2B8 } },
    /* PLAYER_CSMODE_36  */ { -1, { func_8085A120 } },
    /* PLAYER_CSMODE_37  */ { -1, { func_8085A19C } },
    /* PLAYER_CSMODE_38  */ { 6, { &gameplay_keep_Linkanim_00D278 } },
    /* PLAYER_CSMODE_39  */ { 6, { &gameplay_keep_Linkanim_00D288 } },
    /* PLAYER_CSMODE_40  */ { 5, { &gPlayerAnim_rakuba } },
    /* PLAYER_CSMODE_41  */ { 5, { &gPlayerAnim_bajyo_furikaeru } },
    /* PLAYER_CSMODE_42  */ { 5, { &gPlayerAnim_okiagaru } },
    /* PLAYER_CSMODE_43  */ { 5, { &gPlayerAnim_okiagaru_tatu } },
    /* PLAYER_CSMODE_44  */ { 7, { &gPlayerAnim_bajyo_walk } },
    /* PLAYER_CSMODE_45  */ { 5, { &gPlayerAnim_rakka } },
    /* PLAYER_CSMODE_46  */ { 5, { &gPlayerAnim_sirimochi } },
    /* PLAYER_CSMODE_47  */ { 5, { &gPlayerAnim_spotlight } },
    /* PLAYER_CSMODE_48  */ { 3, { &gPlayerAnim_al_hensin } },
    /* PLAYER_CSMODE_49  */ { 5, { &gPlayerAnim_dl_jibunmiru } },
    /* PLAYER_CSMODE_50  */ { 5, { &gPlayerAnim_vs_yousei } },
    /* PLAYER_CSMODE_51  */ { 5, { &gPlayerAnim_urusai } },
    /* PLAYER_CSMODE_52  */ { 5, { &gPlayerAnim_okarinatori } },
    /* PLAYER_CSMODE_53  */ { 5, { &gPlayerAnim_lost_horse } },
    /* PLAYER_CSMODE_54  */ { 4, { &gPlayerAnim_lost_horse_wait } },
    /* PLAYER_CSMODE_55  */ { 5, { &gPlayerAnim_lost_horse2 } },
    /* PLAYER_CSMODE_56  */ { 14, { &gPlayerAnim_okarinatori } },
    /* PLAYER_CSMODE_57  */ { 5, { &gPlayerAnim_cl_tobikakaru } },
    /* PLAYER_CSMODE_58  */ { -1, { func_8085978C } },
    /* PLAYER_CSMODE_59  */ { 5, { &gameplay_keep_Linkanim_00D0A0 } },
    /* PLAYER_CSMODE_60  */ { 2, { &gPlayerAnim_cl_furafura } },
    /* PLAYER_CSMODE_61  */ { 7, { &gPlayerAnim_cl_nigeru } },
    /* PLAYER_CSMODE_62  */ { 5, { &gPlayerAnim_cl_ononoki } },
    /* PLAYER_CSMODE_63  */ { 3, { &gPlayerAnim_al_gaku } },
    /* PLAYER_CSMODE_64  */ { 3, { &gPlayerAnim_al_fuwafuwa } },
    /* PLAYER_CSMODE_65  */ { 3, { &gPlayerAnim_al_fuwafuwa_modori } },
    /* PLAYER_CSMODE_66  */ { 3, { &gPlayerAnim_al_elf_tobidasi } },
    /* PLAYER_CSMODE_67  */ { -1, { func_8085968C } },
    /* PLAYER_CSMODE_68  */ { -1, { func_8085A24C } },
    /* PLAYER_CSMODE_69  */ { -1, { func_8085A330 } },
    /* PLAYER_CSMODE_70  */ { 7, { &gPlayerAnim_cl_tewofuru } },
    /* PLAYER_CSMODE_71  */ { 5, { &gPlayerAnim_cl_jibun_miru } },
    /* PLAYER_CSMODE_72  */ { 5, { &gPlayerAnim_cl_hoo } },
    /* PLAYER_CSMODE_73  */ { 3, { &gPlayerAnim_al_yareyare } },
    /* PLAYER_CSMODE_74  */ { 3, { &gPlayerAnim_al_yes } },
    /* PLAYER_CSMODE_75  */ { 3, { &gPlayerAnim_al_no } },
    /* PLAYER_CSMODE_76  */ { 3, { &gPlayerAnim_al_unun } },
    /* PLAYER_CSMODE_77  */ { 7, { &gPlayerAnim_dl_yusaburu } },
    /* PLAYER_CSMODE_78  */ { 5, { &gPlayerAnim_dl_kokeru } },
    /* PLAYER_CSMODE_79  */ { 3, { &gPlayerAnim_alink_powerup } },
    /* PLAYER_CSMODE_80  */ { 4, { &gPlayerAnim_alink_rakkatyu } },
    /* PLAYER_CSMODE_81  */ { 3, { &gPlayerAnim_alink_kyoro } },
    /* PLAYER_CSMODE_82  */ { 4, { &gPlayerAnim_alink_yurayura } },
    /* PLAYER_CSMODE_83  */ { 3, { &gPlayerAnim_alink_somukeru } },
    /* PLAYER_CSMODE_84  */ { 5, { &gPlayerAnim_alink_fukitobu } },
    /* PLAYER_CSMODE_85  */ { 3, { &gameplay_keep_Linkanim_00CFC8 } },
    /* PLAYER_CSMODE_86  */ { 4, { &gPlayerAnim_alink_tereru } },
    /* PLAYER_CSMODE_87  */ { 5, { &gameplay_keep_Linkanim_00D1D0 } },
    /* PLAYER_CSMODE_88  */ { 3, { &gPlayerAnim_alink_kaitenmiss } },
    /* PLAYER_CSMODE_89  */ { 4, { &gameplay_keep_Linkanim_00CFC0 } },
    /* PLAYER_CSMODE_90  */ { 4, { &gameplay_keep_Linkanim_00CFB8 } },
    /* PLAYER_CSMODE_91  */ { 4, { &gameplay_keep_Linkanim_00D050 } },
    /* PLAYER_CSMODE_92  */ { 4, { &gameplay_keep_Linkanim_00D048 } },
    /* PLAYER_CSMODE_93  */ { -1, { func_8085A8C4 } },
    /* PLAYER_CSMODE_94  */ { 3, { &gPlayerAnim_alink_ozigi } },
    /* PLAYER_CSMODE_95  */ { 3, { &gPlayerAnim_alink_ozigi_modori } },
    /* PLAYER_CSMODE_96  */ { 9, { &gPlayerAnim_link_normal_back_downA } },
    /* PLAYER_CSMODE_97  */ { -1, { func_8085A530 } },
    /* PLAYER_CSMODE_98  */ { 15, { &gPlayerAnim_cl_maskoff } },
    /* PLAYER_CSMODE_99  */ { 7, { &gPlayerAnim_cl_kubisime } },
    /* PLAYER_CSMODE_100 */ { 3, { &gPlayerAnim_alink_ee } },
    /* PLAYER_CSMODE_101 */ { 3, { &gameplay_keep_Linkanim_00CFF0 } },
    /* PLAYER_CSMODE_102 */ { -1, { func_8085A768 } },
    /* PLAYER_CSMODE_103 */ { -1, { func_8085AA60 } },
    /* PLAYER_CSMODE_104 */ { 5, { &gPlayerAnim_cl_dakisime } },
    /* PLAYER_CSMODE_105 */ { 5, { &gPlayerAnim_kf_omen } },
    /* PLAYER_CSMODE_106 */ { 5, { &gPlayerAnim_kf_dakiau } },
    /* PLAYER_CSMODE_107 */ { 5, { &gPlayerAnim_kf_hanare } },
    /* PLAYER_CSMODE_108 */ { 5, { &gPlayerAnim_kf_miseau } },
    /* PLAYER_CSMODE_109 */ { 5, { &gPlayerAnim_kf_awase } },
    /* PLAYER_CSMODE_110 */ { 7, { &gPlayerAnim_kf_tetunagu_loop } },
    /* PLAYER_CSMODE_111 */ { 3, { &gPlayerAnim_link_keirei } },
    /* PLAYER_CSMODE_112 */ { 5, { &gPlayerAnim_cl_umanoru } },
    /* PLAYER_CSMODE_113 */ { 5, { &gPlayerAnim_cl_wakare } },
    /* PLAYER_CSMODE_114 */ { 4, { &gPlayerAnim_alink_dance_loop } },
    /* PLAYER_CSMODE_115 */ { 2, { &gPlayerAnim_link_demo_goma_furimuki } },
    /* PLAYER_CSMODE_116 */ { 7, { &gPlayerAnim_link_uma_anim_fastrun } },
    /* PLAYER_CSMODE_117 */ { 5, { &gPlayerAnim_cl_umamiage } },
    /* PLAYER_CSMODE_118 */ { 7, { &gPlayerAnim_demo_suwari1 } },
    /* PLAYER_CSMODE_119 */ { 7, { &gPlayerAnim_demo_suwari2 } },
    /* PLAYER_CSMODE_120 */ { 7, { &gPlayerAnim_demo_suwari3 } },
    /* PLAYER_CSMODE_121 */ { -1, { func_80859990 } },
    /* PLAYER_CSMODE_122 */ { 0, { NULL } },
    /* PLAYER_CSMODE_123 */ { -1, { func_80859A10 } },
    /* PLAYER_CSMODE_124 */ { 7, { &gPlayerAnim_clink_demo_get1 } },
    /* PLAYER_CSMODE_125 */ { 5, { &gPlayerAnim_clink_demo_get2 } },
    /* PLAYER_CSMODE_126 */ { 5, { &gPlayerAnim_clink_demo_get3 } },
    /* PLAYER_CSMODE_127 */ { 3, { &gPlayerAnim_link_demo_gurad } },
    /* PLAYER_CSMODE_128 */ { 4, { &gPlayerAnim_link_demo_sita_wait } },
    /* PLAYER_CSMODE_129 */ { 3, { &gPlayerAnim_L_1kyoro } },
    /* PLAYER_CSMODE_130 */ { 3, { &gPlayerAnim_L_2kyoro } },
    /* PLAYER_CSMODE_131 */ { 3, { &gPlayerAnim_L_sagaru } },
    /* PLAYER_CSMODE_132 */ { 3, { &gPlayerAnim_L_bouzen } },
    /* PLAYER_CSMODE_133 */ { 3, { &gPlayerAnim_L_kamaeru } },
    /* PLAYER_CSMODE_134 */ { 3, { &gPlayerAnim_L_hajikareru } },
    /* PLAYER_CSMODE_135 */ { 3, { &gPlayerAnim_L_ken_miru } },
    /* PLAYER_CSMODE_136 */ { 3, { &gPlayerAnim_L_mukinaoru } },
    /* PLAYER_CSMODE_137 */ { 3, { &gPlayerAnim_link_demo_return_to_past } },
    /* PLAYER_CSMODE_138 */ { 3, { &gPlayerAnim_link_last_hit_motion1 } },
    /* PLAYER_CSMODE_139 */ { 3, { &gPlayerAnim_link_last_hit_motion2 } },
};

struct_8085DA94 sCutsceneModeUpdateFuncs[PLAYER_CSMODE_MAX] = {
    /* PLAYER_CSMODE_0   */ { 0, { NULL } },
    /* PLAYER_CSMODE_1   */ { -1, { func_80859414 } },
    /* PLAYER_CSMODE_2   */ { -1, { func_80859AD0 } },
    /* PLAYER_CSMODE_3   */ { -1, { func_80859B28 } },
    /* PLAYER_CSMODE_4   */ { 11, { NULL } },
    /* PLAYER_CSMODE_5   */ { -1, { func_8085ADA0 } },
    /* PLAYER_CSMODE_6   */ { -1, { Player_Cutscene_End } },
    /* PLAYER_CSMODE_7   */ { -1, { func_808595B8 } },
    /* PLAYER_CSMODE_8   */ { 18, { D_8085DA70 } },
    /* PLAYER_CSMODE_9   */ { -1, { func_80859890 } },
    /* PLAYER_CSMODE_10  */ { -1, { func_80859C60 } },
    /* PLAYER_CSMODE_11  */ { 18, { D_8085D9E0 } },
    /* PLAYER_CSMODE_12  */ { 11, { NULL } },
    /* PLAYER_CSMODE_13  */ { 11, { NULL } },
    /* PLAYER_CSMODE_14  */ { 0, { NULL } },
    /* PLAYER_CSMODE_15  */ { 11, { NULL } },
    /* PLAYER_CSMODE_16  */ { 17, { &gPlayerAnim_link_normal_okarina_swing } },
    /* PLAYER_CSMODE_17  */ { 11, { NULL } },
    /* PLAYER_CSMODE_18  */ { 11, { NULL } },
    /* PLAYER_CSMODE_19  */ { -1, { func_8085A710 } },
    /* PLAYER_CSMODE_20  */ { -1, { func_808595B8 } },
    /* PLAYER_CSMODE_21  */ { 12, { &gPlayerAnim_clink_demo_mimawasi_wait } },
    /* PLAYER_CSMODE_22  */ { -1, { func_80859D70 } },
    /* PLAYER_CSMODE_23  */ { 12, { &gPlayerAnim_link_demo_look_hand_wait } },
    /* PLAYER_CSMODE_24  */ { 11, { NULL } },
    /* PLAYER_CSMODE_25  */ { 11, { NULL } },
    /* PLAYER_CSMODE_26  */ { -1, { func_8085A6C0 } },
    /* PLAYER_CSMODE_27  */ { 12, { &gPlayerAnim_link_demo_zeldamiru_wait } },
    /* PLAYER_CSMODE_28  */ { 12, { &gPlayerAnim_link_demo_kenmiru1_wait } },
    /* PLAYER_CSMODE_29  */ { 12, { &gPlayerAnim_link_demo_kenmiru2_wait } },
    /* PLAYER_CSMODE_30  */ { 12, { &gPlayerAnim_demo_link_nwait } },
    /* PLAYER_CSMODE_31  */ { 12, { &gameplay_keep_Linkanim_00D318 } },
    /* PLAYER_CSMODE_32  */ { -1, { func_80859FF4 } },
    /* PLAYER_CSMODE_33  */ { -1, { Player_Cutscene_TranslateReverse } },
    /* PLAYER_CSMODE_34  */ { 11, { NULL } },
    /* PLAYER_CSMODE_35  */ { 12, { &gameplay_keep_Linkanim_00D2C0 } },
    /* PLAYER_CSMODE_36  */ { -1, { func_8085A144 } },
    /* PLAYER_CSMODE_37  */ { -1, { func_8085A1D4 } },
    /* PLAYER_CSMODE_38  */ { 12, { &gameplay_keep_Linkanim_00D280 } },
    /* PLAYER_CSMODE_39  */ { 12, { &gameplay_keep_Linkanim_00D290 } },
    /* PLAYER_CSMODE_40  */ { 18, { D_8085D9F0 } },
    /* PLAYER_CSMODE_41  */ { 11, { NULL } },
    /* PLAYER_CSMODE_42  */ { 18, { D_8085DA00 } },
    /* PLAYER_CSMODE_43  */ { 13, { &gPlayerAnim_okiagaru_wait } },
    /* PLAYER_CSMODE_44  */ { -1, { func_8085A364 } },
    /* PLAYER_CSMODE_45  */ { 18, { D_8085DA2C } },
    /* PLAYER_CSMODE_46  */ { 12, { &gPlayerAnim_sirimochi_wait } },
    /* PLAYER_CSMODE_47  */ { 12, { &gPlayerAnim_spotlight_wait } },
    /* PLAYER_CSMODE_48  */ { 12, { &gPlayerAnim_al_hensin_loop } },
    /* PLAYER_CSMODE_49  */ { 12, { &gPlayerAnim_dl_jibunmiru_wait } },
    /* PLAYER_CSMODE_50  */ { 18, { D_8085DA50 } },
    /* PLAYER_CSMODE_51  */ { 11, { NULL } },
    /* PLAYER_CSMODE_52  */ { 18, { D_8085D9FC } },
    /* PLAYER_CSMODE_53  */ { 12, { &gPlayerAnim_lost_horse_wait } },
    /* PLAYER_CSMODE_54  */ { -1, { func_8085A364 } },
    /* PLAYER_CSMODE_55  */ { 18, { D_8085DA1C } },
    /* PLAYER_CSMODE_56  */ { 11, { NULL } },
    /* PLAYER_CSMODE_57  */ { 18, { D_8085DA0C } },
    /* PLAYER_CSMODE_58  */ { -1, { func_808599DC } },
    /* PLAYER_CSMODE_59  */ { 11, { NULL } },
    /* PLAYER_CSMODE_60  */ { 18, { D_8085DA28 } },
    /* PLAYER_CSMODE_61  */ { -1, { func_8085A364 } },
    /* PLAYER_CSMODE_62  */ { 11, { NULL } },
    /* PLAYER_CSMODE_63  */ { 11, { NULL } },
    /* PLAYER_CSMODE_64  */ { 12, { &gPlayerAnim_al_fuwafuwa_loop } },
    /* PLAYER_CSMODE_65  */ { -1, { func_8085A40C } },
    /* PLAYER_CSMODE_66  */ { 11, { NULL } },
    /* PLAYER_CSMODE_67  */ { -1, { func_80859708 } },
    /* PLAYER_CSMODE_68  */ { -1, { func_8085A2AC } },
    /* PLAYER_CSMODE_69  */ { 11, { NULL } },
    /* PLAYER_CSMODE_70  */ { 11, { NULL } },
    /* PLAYER_CSMODE_71  */ { -1, { func_8085A40C } },
    /* PLAYER_CSMODE_72  */ { 11, { NULL } },
    /* PLAYER_CSMODE_73  */ { 11, { NULL } },
    /* PLAYER_CSMODE_74  */ { 11, { NULL } },
    /* PLAYER_CSMODE_75  */ { 11, { NULL } },
    /* PLAYER_CSMODE_76  */ { 11, { NULL } },
    /* PLAYER_CSMODE_77  */ { 11, { NULL } },
    /* PLAYER_CSMODE_78  */ { 18, { D_8085DA78 } },
    /* PLAYER_CSMODE_79  */ { 12, { &gPlayerAnim_alink_powerup_loop } },
    /* PLAYER_CSMODE_80  */ { -1, { func_8085A364 } },
    /* PLAYER_CSMODE_81  */ { 12, { &gPlayerAnim_alink_kyoro_loop } },
    /* PLAYER_CSMODE_82  */ { 11, { NULL } },
    /* PLAYER_CSMODE_83  */ { 12, { &gPlayerAnim_alink_somukeru_loop } },
    /* PLAYER_CSMODE_84  */ { 18, { D_8085DA80 } },
    /* PLAYER_CSMODE_85  */ { 12, { &gameplay_keep_Linkanim_00CFD0 } },
    /* PLAYER_CSMODE_86  */ { 11, { NULL } },
    /* PLAYER_CSMODE_87  */ { 11, { NULL } },
    /* PLAYER_CSMODE_88  */ { 11, { NULL } },
    /* PLAYER_CSMODE_89  */ { 11, { NULL } },
    /* PLAYER_CSMODE_90  */ { 11, { NULL } },
    /* PLAYER_CSMODE_91  */ { 11, { NULL } },
    /* PLAYER_CSMODE_92  */ { 11, { NULL } },
    /* PLAYER_CSMODE_93  */ { -1, { func_8085A940 } },
    /* PLAYER_CSMODE_94  */ { 12, { &gPlayerAnim_alink_ozigi_loop } },
    /* PLAYER_CSMODE_95  */ { 11, { NULL } },
    /* PLAYER_CSMODE_96  */ { -1, { func_8085A4A4 } },
    /* PLAYER_CSMODE_97  */ { -1, { func_8085A5DC } },
    /* PLAYER_CSMODE_98  */ { -1, { func_8085AA84 } },
    /* PLAYER_CSMODE_99  */ { 11, { NULL } },
    /* PLAYER_CSMODE_100 */ { 12, { &gPlayerAnim_alink_ee_loop } },
    /* PLAYER_CSMODE_101 */ { 12, { &gameplay_keep_Linkanim_00CFF8 } },
    /* PLAYER_CSMODE_102 */ { -1, { func_8085A7C0 } },
    /* PLAYER_CSMODE_103 */ { 11, { NULL } },
    /* PLAYER_CSMODE_104 */ { 13, { &gPlayerAnim_cl_dakisime_loop } },
    /* PLAYER_CSMODE_105 */ { 13, { &gPlayerAnim_kf_omen_loop } },
    /* PLAYER_CSMODE_106 */ { 13, { &gPlayerAnim_kf_dakiau_loop } },
    /* PLAYER_CSMODE_107 */ { 13, { &gPlayerAnim_kf_hanare_loop } },
    /* PLAYER_CSMODE_108 */ { 11, { NULL } },
    /* PLAYER_CSMODE_109 */ { 11, { NULL } },
    /* PLAYER_CSMODE_110 */ { 11, { NULL } },
    /* PLAYER_CSMODE_111 */ { 12, { &gPlayerAnim_link_kei_wait } },
    /* PLAYER_CSMODE_112 */ { 13, { &gPlayerAnim_cl_umanoru_loop } },
    /* PLAYER_CSMODE_113 */ { 13, { &gPlayerAnim_cl_wakare_loop } },
    /* PLAYER_CSMODE_114 */ { 11, { NULL } },
    /* PLAYER_CSMODE_115 */ { -1, { func_8085AA10 } },
    /* PLAYER_CSMODE_116 */ { 11, { NULL } },
    /* PLAYER_CSMODE_117 */ { -1, { func_8085A364 } },
    /* PLAYER_CSMODE_118 */ { 11, { NULL } },
    /* PLAYER_CSMODE_119 */ { 11, { NULL } },
    /* PLAYER_CSMODE_120 */ { 11, { NULL } },
    /* PLAYER_CSMODE_121 */ { -1, { func_808599DC } },
    /* PLAYER_CSMODE_122 */ { -1, { func_80859AF8 } },
    /* PLAYER_CSMODE_123 */ { -1, { func_80859A44 } },
    /* PLAYER_CSMODE_124 */ { 11, { NULL } },
    /* PLAYER_CSMODE_125 */ { 11, { NULL } },
    /* PLAYER_CSMODE_126 */ { 11, { NULL } },
    /* PLAYER_CSMODE_127 */ { 12, { &gPlayerAnim_link_demo_gurad_wait } },
    /* PLAYER_CSMODE_128 */ { 11, { NULL } },
    /* PLAYER_CSMODE_129 */ { 12, { &gPlayerAnim_L_kw } },
    /* PLAYER_CSMODE_130 */ { 11, { NULL } },
    /* PLAYER_CSMODE_131 */ { 11, { NULL } },
    /* PLAYER_CSMODE_132 */ { 11, { NULL } },
    /* PLAYER_CSMODE_133 */ { 11, { NULL } },
    /* PLAYER_CSMODE_134 */ { -1, { func_80859D44 } },
    /* PLAYER_CSMODE_135 */ { 11, { NULL } },
    /* PLAYER_CSMODE_136 */ { 12, { &gPlayerAnim_L_kennasi_w } },
    /* PLAYER_CSMODE_137 */ { -1, { func_80859EBC } },
    /* PLAYER_CSMODE_138 */ { -1, { func_80859F4C } },
    /* PLAYER_CSMODE_139 */ { -1, { func_80859F4C } },
};

LinkAnimationHeader* D_8085E354[PLAYER_FORM_MAX] = {
    &gPlayerAnim_L_okarina_get, &gPlayerAnim_L_okarina_get, &gPlayerAnim_L_okarina_get,
    &gPlayerAnim_L_okarina_get, &gPlayerAnim_om_get,
};

struct_8085E368 D_8085E368[PLAYER_FORM_MAX] = {
    /* PLAYER_FORM_FIERCE_DEITY */ { { -200, 700, 100 }, { 800, 600, 800 } },
    /* PLAYER_FORM_GORON        */ { { -200, 700, 100 }, { 800, 600, 800 } },
    /* PLAYER_FORM_ZORA         */ { { -200, 700, 100 }, { 800, 600, 800 } },
    /* PLAYER_FORM_DEKU         */ { { -200, 700, 100 }, { 800, 600, 800 } },
    /* PLAYER_FORM_HUMAN        */ { { -200, 500, 0 }, { 600, 400, 600 } },
};

Color_RGBA8 D_8085E3A4 = { 255, 255, 255, 0 };
Color_RGBA8 D_8085E3A8 = { 0, 128, 128, 0 };

void func_80859D70(PlayState* play, Player* this, CsCmdActorAction* arg2) {
    struct_8085E368* posInfo;
    Vec3f effectPos;
    Vec3f randPos;

    func_808591BC(play, this, D_8085E354[this->transformation]);

    if (this->rightHandType != 0xFF) {
        this->rightHandType = 0xFF;
    } else {
        posInfo = &D_8085E368[this->transformation];
        randPos.x = randPlusMinusPoint5Scaled(posInfo->range.x) + posInfo->base.x;
        randPos.y = randPlusMinusPoint5Scaled(posInfo->range.y) + posInfo->base.y;
        randPos.z = randPlusMinusPoint5Scaled(posInfo->range.z) + posInfo->base.z;
        SkinMatrix_Vec3fMtxFMultXYZ(&this->shieldMf, &randPos, &effectPos);
        EffectSsKirakira_SpawnDispersed(play, &effectPos, &gZeroVec3f, &gZeroVec3f, &D_8085E3A4, &D_8085E3A8, 600, -10);
    }
}

void func_80859EBC(PlayState* play, Player* this, CsCmdActorAction* playerActionCsCmd) {
    if (LinkAnimation_Update(play, &this->skelAnime)) {
        Player_Cutscene_End(play, this, playerActionCsCmd);
    } else if (this->genericTimer == 0) {
        Item_Give(play, ITEM_SWORD_RAZOR);
        Player_CutsceneDrawSword(play, this, false);
    } else {
        func_808484CC(this);
    }
}

void func_80859F4C(PlayState* play, Player* this, UNK_TYPE arg2) {
    if (LinkAnimation_Update(play, &this->skelAnime)) {
        Player_SetupMeleeAttack(play, this, 0.0f, 99.0f, this->skelAnime.endFrame - 8.0f);
    }
    if (this->itemActionParam != PLAYER_AP_SWORD_GILDED) {
        Player_CutsceneDrawSword(play, this, true);
    }
}

void func_80859FCC(PlayState* play, Player* this, UNK_TYPE arg2) {
    if (this->transformation != PLAYER_FORM_DEKU) {
        gSaveContext.save.playerForm = PLAYER_FORM_DEKU;
    }
}

void func_80859FF4(PlayState* play, Player* this, UNK_TYPE arg2) {
    LinkAnimation_Update(play, &this->skelAnime);
    if (gSaveContext.save.playerForm != this->transformation) {
        this->actor.update = func_8012301C;
        this->actor.draw = NULL;
    }
}

void Player_Cutscene_TranslateReverse(PlayState* play, Player* this, void* arg) {
    CsCmdActorAction* playerAction = arg;
    f32 xEnd = playerAction->endPos.x;
    f32 yEnd = playerAction->endPos.y;
    f32 zEnd = playerAction->endPos.z;
    f32 xDiff;
    f32 yDiff;
    f32 zDiff;
    f32 progress;

    xDiff = playerAction->startPos.x - xEnd;
    yDiff = playerAction->startPos.y - yEnd;
    zDiff = playerAction->startPos.z - zEnd;

    progress = ((f32)(playerAction->endFrame - play->csCtx.frames)) /
               ((f32)(playerAction->endFrame - playerAction->startFrame));

    this->actor.world.pos.x = (xDiff * progress) + xEnd;
    this->actor.world.pos.y = (yDiff * progress) + yEnd;
    this->actor.world.pos.z = (zDiff * progress) + zEnd;
    LinkAnimation_Update(play, &this->skelAnime);
}

void func_8085A120(PlayState* play, Player* this, UNK_TYPE arg2) {
    if (this->transformation != PLAYER_FORM_FIERCE_DEITY) {
        gSaveContext.save.playerForm = PLAYER_FORM_FIERCE_DEITY;
    }
}

void func_8085A144(PlayState* play, Player* this, UNK_TYPE arg2) {
    LinkAnimation_Update(play, &this->skelAnime);
    if (gSaveContext.save.playerForm != this->transformation) {
        this->actor.update = func_8012301C;
        this->actor.draw = NULL;
    }
}

void func_8085A19C(PlayState* play, Player* this, UNK_TYPE arg2) {
    Player_AnimationPlayOnce(play, this, &gPlayerAnim_demo_rakka);
    this->unk_AAA = -0x8000;
}

void func_8085A1D4(PlayState* play, Player* this, UNK_TYPE arg2) {
    LinkAnimation_Update(play, &this->skelAnime);
    this->actor.gravity = 0.0f;
    Math_StepToF(&this->actor.velocity.y, -this->actor.terminalVelocity, -((f32)REG(68) / 100.0f));
}

void func_8085A24C(PlayState* play, Player* this, UNK_TYPE arg2) {
    Player_PlayAnimOnceSlowed(play, this, sPlayerOcarinaStartAnimations[this->transformation]);
    this->heldItemActionParam = PLAYER_AP_OCARINA;
    Player_SetModels(this, Player_ActionToModelGroup(this, this->heldItemActionParam));
}

void func_8085A2AC(PlayState* play, Player* this, UNK_TYPE arg2) {
    if ((LinkAnimation_Update(play, &this->skelAnime)) &&
        (this->skelAnime.animation == sPlayerOcarinaStartAnimations[this->transformation])) {
        Player_InitOcarinaAnims(play, this);
        return;
    }
    if (this->genericTimer != 0) {
        Player_UpdateOcarinaAnims(play, this);
    }
}

void func_8085A330(PlayState* play, Player* this, UNK_TYPE arg2) {
    Player_AnimationPlayOnceReverse(play, this, sPlayerOcarinaStartAnimations[this->transformation]);
}

void func_8085A364(PlayState* play, Player* this, void* arg2) {
    Player_Cutscene_Translate(play, this, arg2);
    if (LinkAnimation_Update(play, &this->skelAnime)) {
        Player_PlayAnimLoopWithMovementPresetFlagsSlowed(play, this, &gPlayerAnim_cl_umamiage_loop);
    }

    if (this->skelAnime.animation == &gPlayerAnim_cl_nigeru) {
        Player_PlayAnimSfx(this, D_8085DA48);
    } else if (this->skelAnime.animation == &gPlayerAnim_alink_rakkatyu) {
        func_800B8F98(&this->actor, NA_SE_PL_FLYING_AIR - SFX_FLAG);
    } else {
        func_80858FE8(this);
    }
}

void func_8085A40C(PlayState* play, Player* this, void* arg2) {
    if (LinkAnimation_Update(play, &this->skelAnime)) {
        func_80859CA0(play, this, arg2);
    } else if (this->skelAnime.animation == &gPlayerAnim_al_fuwafuwa_modori) {
        Player_PlayAnimSfx(this, D_8085DA88);
    } else if (this->skelAnime.animation == &gPlayerAnim_cl_jibun_miru) {
        Player_PlayAnimSfx(this, D_8085DA8C);
    }
}

void func_8085A4A4(PlayState* play, Player* this, void* arg2) {
    if (LinkAnimation_Update(play, &this->skelAnime) && (this->genericTimer == 0) && (this->actor.bgCheckFlags & 1)) {
        Player_AnimationPlayOnce(play, this, &gPlayerAnim_link_normal_back_downB);
        this->genericTimer = 1;
    }
    if (this->genericTimer != 0) {
        Player_StepLinearVelocityToZero(this);
    }
}

void func_8085A530(PlayState* play, Player* this, void* arg2) {
    LinkAnimation_Change(play, &this->skelAnime, &gPlayerAnim_link_normal_give_other, 1.0f,
                         (play->sceneId == SCENE_ALLEY) ? IREG(56) : 0.0f,
                         Animation_GetLastFrame(&gPlayerAnim_link_normal_give_other), ANIMMODE_ONCE, -8.0f);
}

void func_8085A5DC(PlayState* play, Player* this, void* arg2) {
    if (LinkAnimation_Update(play, &this->skelAnime)) {
        if (this->genericTimer++ >= 0x15) {
            LinkAnimation_Change(play, &this->skelAnime, &gPlayerAnim_pz_wait, 1.0f, 0.0f, 0.0f, ANIMMODE_LOOP, -16.0f);
        }
    }
}

void func_8085A66C(PlayState* play, Player* this, void* arg2) {
    if (Player_IsSwimming(this)) {
        func_8085929C(play, this, 0);
    } else {
        func_80858E40(play, this, &gPlayerAnim_link_demo_kousan);
    }
}

void func_8085A6C0(PlayState* play, Player* this, void* anim) {
    if (Player_IsSwimming(this)) {
        func_80859300(play, this, 0);
    } else {
        func_80858FBC(play, this, anim);
    }
}

void func_8085A710(PlayState* play, Player* this, void* anim) {
    func_80858FBC(play, this, anim);
    if (Player_SetupGetItemOrHoldBehavior(this, play)) {
        play->csCtx.state = CS_STATE_3;
        ActorCutscene_Stop(ActorCutscene_GetCurrentIndex());
    }
}

void func_8085A768(PlayState* play, Player* this, void* arg2) {
    Player_LoadGetItemObject(this, OBJECT_GI_RESERVE_C_01);
    func_80858E40(play, this, &gPlayerAnim_link_normal_give_other);
    this->stateFlags2 &= ~PLAYER_STATE2_1000000;
}

void func_8085A7C0(PlayState* play, Player* this, UNK_TYPE arg2) {
    if (LinkAnimation_Update(play, &this->skelAnime)) {
        if (this->genericTimer == 0) {
            if ((Message_GetState(&play->msgCtx) == TEXT_STATE_CLOSING) ||
                (Message_GetState(&play->msgCtx) == TEXT_STATE_NONE)) {
                this->getItemDrawId = GID_BOTTLE;
                this->genericTimer = -1;
            } else {
                this->getItemDrawId = GID_MUSHROOM;
            }
        } else if (this->genericTimer < 0) {
            if (Actor_HasParent(&this->actor, play)) {
                this->actor.parent = NULL;
                this->genericTimer = 1;
            } else {
                Actor_PickUp(&this->actor, play, GI_PENDANT_OF_MEMORIES, 9999.9f, 9999.9f);
            }
        }
    } else if (LinkAnimation_OnFrame(&this->skelAnime, 4.0f)) {
        gSaveContext.save.weekEventReg[50] |= 0x80;
    }
}

void func_8085A8C4(PlayState* play, Player* this, UNK_TYPE arg2) {
    if ((this->transformation != PLAYER_FORM_HUMAN) && (play->roomCtx.curRoom.unk3 == 5)) {
        MREG(64) = 0x2D;
        MREG(65) = 0xFF;
        MREG(66) = 0xFF;
        MREG(67) = 0xFF;
        MREG(68) = 0;
        play_sound(NA_SE_SY_WHITE_OUT_T);
    }
}

void func_8085A940(PlayState* play, Player* this, UNK_TYPE arg2) {
    s16 temp_v1 = MREG(64);

    if (temp_v1 > 0) {
        MREG(68) += temp_v1;
        if (MREG(68) > 0xFF) {
            MREG(64) = -0x40;
            MREG(68) = 0xFF;
            gSaveContext.save.playerForm = PLAYER_FORM_HUMAN;
            this->actor.update = func_8012301C;
            this->actor.draw = NULL;
            this->genericVar = 0;
        }
    } else if (temp_v1 < 0) {
        MREG(68) += temp_v1;
        if (MREG(68) < 0) {
            MREG(64) = 0;
            MREG(68) = 0;
        }
    } else {
        LinkAnimation_Update(play, &this->skelAnime);
    }
}

void func_8085AA10(PlayState* play, Player* this, UNK_TYPE arg2) {
    if (LinkAnimation_Update(play, &this->skelAnime) && (ActorCutscene_GetCurrentIndex() == 0x7D)) {
        ActorCutscene_Stop(0x7D);
    }
}

void func_8085AA60(PlayState* play, Player* this, UNK_TYPE arg2) {
    Player_SpawnElegyShell(play, this);
}

void func_8085AA84(PlayState* play, Player* this, UNK_TYPE arg2) {
    if (LinkAnimation_Update(play, &this->skelAnime)) {
        this->stateFlags2 |= PLAYER_STATE2_1000000;
    }
}

void Player_Cutscene_End(PlayState* play, Player* this, CsCmdActorAction* arg2) {
    if (Player_IsSwimming(this)) {
        Player_SetupSwimIdle(play, this);
        Player_ResetSubCam(play, this);
    } else {
        func_80839ED0(this, play);
        if (!Player_SetupSpeakOrCheck(this, play)) {
            Player_SetupGetItemOrHoldBehavior(this, play);
        }
    }

    this->csMode = PLAYER_CSMODE_0;
    this->attentionMode = PLAYER_ATTENTIONMODE_NONE;
}

void Player_Cutscene_SetPosAndYawToStart(Player* this, CsCmdActorAction* playerAction) {
    this->actor.world.pos.x = playerAction->startPos.x;
    this->actor.world.pos.y = playerAction->startPos.y;
    this->actor.world.pos.z = playerAction->startPos.z;

    this->currentYaw = this->actor.shape.rot.y = playerAction->rot.y;
}

void Player_Cutscene_8085ABA8(Player* this, CsCmdActorAction* playerAction) {
    f32 xDiff = playerAction->startPos.x - (s32)this->actor.world.pos.x;
    f32 yDiff = playerAction->startPos.y - (s32)this->actor.world.pos.y;
    f32 zDiff = playerAction->startPos.z - (s32)this->actor.world.pos.z;
    f32 dist;
    s16 temp_v0;

    temp_v0 = playerAction->rot.y - this->actor.shape.rot.y;
    dist = sqrtf(SQ(xDiff) + SQ(yDiff) + SQ(zDiff));
    if (this->linearVelocity == 0.0f) {
        if ((dist > 50.0f) || (ABS_ALT(temp_v0) > 0x4000)) {
            Player_Cutscene_SetPosAndYawToStart(this, playerAction);
        }
    }

    this->skelAnime.moveFlags = 0;
    Player_ClearRootLimbPosY(this);
}

void Player_CsModePlayback(PlayState* play, Player* this, CsCmdActorAction* actorAction, struct_8085DA94* arg3) {
    if (arg3->type > 0) {
        D_8085D990[arg3->type](play, this, arg3->anim);
    } else if (arg3->type < 0) {
        arg3->func(play, this, actorAction);
    }

    if ((D_80862B6C & 4) && !(this->skelAnime.moveFlags & 4)) {
        this->skelAnime.morphTable[PLAYER_LIMB_ROOT - 1].y /= this->ageProperties->unk_08;
        D_80862B6C = 0;
    }
}

void Player_CutsceneDetatchHeldActor(PlayState* play, Player* this, PlayerCsMode csMode) {
    if ((csMode != PLAYER_CSMODE_1) && (csMode != PLAYER_CSMODE_7) && (csMode != PLAYER_CSMODE_20) &&
        (csMode != PLAYER_CSMODE_6)) {
        Player_DetatchHeldActor(play, this);
    }
}

// Player_CutsceneUnk6Update?
void func_8085ADA0(PlayState* play, Player* this, s32 arg2) {
    CsCmdActorAction* actorAction = (this->actor.id == ACTOR_EN_TEST3)
                                        ? play->csCtx.actorActions[Cutscene_GetActorActionIndex(play, 0x1FA)]
                                        : play->csCtx.playerAction;
    s32 var_a0 = false;
    s32 pad;
    s32 csMode;

    if ((play->csCtx.state == CS_STATE_IDLE) || (play->csCtx.state == CS_STATE_3) ||
        (play->csCtx.state == CS_STATE_4)) {
        if ((D_8085D384[this->unk_396] == PLAYER_CSMODE_68) && (play->sceneId == SCENE_OKUJOU)) {
            this->attentionMode = PLAYER_UNKAA5_5;

            if (Player_SetupItemCutsceneOrFirstPerson(this, play)) {
                this->csMode = PLAYER_CSMODE_0;
            }
            return;
        } else {
            var_a0 = true;

            if (D_8085D384[this->unk_396] != PLAYER_CSMODE_16) {
                this->csMode = PLAYER_CSMODE_6;
                func_800B7298(play, NULL, PLAYER_CSMODE_6);
                this->unk_396 = 0;
                Player_StopHorizontalMovement(this);
                return;
            }
        }
    }

    if (!var_a0 && (actorAction == NULL)) {
        this->actor.flags &= ~ACTOR_FLAG_40;
        return;
    }

    if (!var_a0 && (this->unk_396 != actorAction->action)) {
        csMode = D_8085D384[actorAction->action];

        if ((csMode >= PLAYER_CSMODE_0) && (D_801F4DE0 == 0)) {
            if ((csMode == PLAYER_CSMODE_2) || (csMode == PLAYER_CSMODE_3)) {
                Player_Cutscene_8085ABA8(this, actorAction);
            } else {
                Player_Cutscene_SetPosAndYawToStart(this, actorAction);
            }
        }

        if (csMode == PLAYER_CSMODE_108) {
            this->stateFlags3 |= PLAYER_STATE3_BREMEN_MARCH;
        } else if (csMode == PLAYER_CSMODE_110) {
            this->stateFlags3 &= ~PLAYER_STATE3_BREMEN_MARCH;
        }

        D_80862B6C = this->skelAnime.moveFlags;

        Player_EndAnimMovement(this);
        Player_CutsceneDetatchHeldActor(play, this, ABS_ALT(csMode));
        Player_CsModePlayback(play, this, actorAction, &sCutsceneModeInitFuncs[ABS_ALT(csMode)]);

        this->genericTimer = 0;
        this->genericVar = 0;
        this->unk_396 = actorAction->action;
    }

    csMode = D_8085D384[this->unk_396];
    Player_CsModePlayback(play, this, actorAction, &sCutsceneModeUpdateFuncs[ABS_ALT(csMode)]);

    if ((u16)actorAction->rot.x != 0) {
        Math_SmoothStepToS(&this->actor.focus.rot.x, (u16)actorAction->rot.x, 4, 0x2710, 0);
        Player_UpdateLookAngles(this, false);
    }
}

void Player_StartCutscene(Player* this, PlayState* play) {
    if (this->csMode != this->prevCsMode) {
        D_80862B6C = this->skelAnime.moveFlags;
        Player_EndAnimMovement(this);

        this->prevCsMode = this->csMode;
        Player_CutsceneDetatchHeldActor(play, this, this->csMode);
        Player_CsModePlayback(play, this, NULL, &sCutsceneModeInitFuncs[this->csMode]);
    }

    Player_CsModePlayback(play, this, NULL, &sCutsceneModeUpdateFuncs[this->csMode]);
}

s32 Player_StartFishing(PlayState* play) {
    Player* player = GET_PLAYER(play);

    Player_ResetAttributesAndHeldActor(play, player);
    Player_UseItem(play, player, ITEM_FISHING_ROD);
    return 1;
}

// Player_GrabPlayerImpl? Player_GrabPlayerNoChecks?
void Player_SetupRestrainedByEnemy(PlayState* play, Player* this) {
    Player_ResetAttributesAndHeldActor(play, this);
    Player_SetAction(play, this, Player_RestrainedByEnemy, 0);
    Player_AnimationPlayOnce(play, this, &gPlayerAnim_link_normal_re_dead_attack);
    this->stateFlags2 |= PLAYER_STATE2_80;
    Player_ClearAttentionModeAndStopMoving(this);
    Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_HELD);
}

s32 Player_GrabPlayer(PlayState* play, Player* player) {
    if (!Player_InBlockingCsMode(play, player) && (player->invincibilityTimer >= 0) &&
        !Player_IsShootingHookshot(player)) {
        if (!(player->stateFlags1 & (PLAYER_STATE1_IN_DEATH_CUTSCENE | PLAYER_STATE1_2000 | PLAYER_STATE1_4000 |
                                     PLAYER_STATE1_100000 | PLAYER_STATE1_200000 | PLAYER_STATE1_RIDING_HORSE))) {
            if (!(player->stateFlags2 & PLAYER_STATE2_80) && !(player->stateFlags3 & PLAYER_STATE3_80)) {
                Player_SetupRestrainedByEnemy(play, player);
                return true;
            }
        }
    }

    return false;
}

s32 Player_SetupPlayerCutscene(PlayState* play, Player* this, PlayerCsMode csMode) {
    Player* player = GET_PLAYER(play);

    if (this != NULL) {
        if (csMode == PLAYER_CSMODE_0) {
            return Player_Door_Open == this->actionFunc;
        }

        if ((this->actor.id == ACTOR_EN_TEST3) && (csMode < 0)) {
            Player_SetupTurn(play, this, this->actor.home.rot.y);
            return false;
        }

        if (this->actor.id == ACTOR_EN_TEST3) {
            player = this;
        }
    }

    if ((player->actor.id == ACTOR_EN_TEST3) || !Player_InBlockingCsMode(play, player)) {
        Player_ResetAttributesAndHeldActor(play, player);
        Player_SetAction(play, player, Player_StartCutscene, 0);
        player->csMode = csMode;
        player->unk_398 = &this->actor;
        Player_ClearAttentionModeAndStopMoving(player);

        return true;
    }

    return false;
}

void Player_SetupStandingStillMorph(Player* this, PlayState* play) {
    Player_SetAction(play, this, Player_StandingStill, 1);
    Player_ChangeAnimMorphToLastFrame(play, this, Player_GetStandingStillAnim(this));
    this->currentYaw = this->actor.shape.rot.y;
}

/**
 * Returns true if Player's health reaches zero
 */
s32 Player_InflictDamage(PlayState* play, s32 damage) {
    Player* player = GET_PLAYER(play);

    if ((player->stateFlags2 & PLAYER_STATE2_80) || !Player_InBlockingCsMode(play, player)) {
        if (func_808339D4(play, player, damage) == 0) {
            player->stateFlags2 &= ~PLAYER_STATE2_80;
            return true;
        }
    }

    return false;
}

// Start talking with the given actor
void Player_TalkWithPlayer(PlayState* play, Actor* actor) {
    s32 pad;
    Player* player = GET_PLAYER(play);

    func_808323C0(player, 0x7C);
    if ((player->talkActor != NULL) || (actor == player->tatlActor) ||
        CHECK_FLAG_ALL(actor->flags, ACTOR_FLAG_1 | ACTOR_FLAG_40000)) {
        actor->flags |= ACTOR_FLAG_100;
    }

    player->talkActor = actor;
    player->exchangeItemId = PLAYER_AP_NONE;
    player->targetedActor = actor;

    if (actor->textId == 0xFFFF) {
        func_800B7298(play, actor, PLAYER_CSMODE_1);
        actor->flags |= ACTOR_FLAG_100;
        Player_UnequipItem(play, player);
    } else {
        if (player->actor.flags & ACTOR_FLAG_100) {
            player->actor.textId = 0;
        } else {
            player->actor.flags |= ACTOR_FLAG_100;
            player->actor.textId = actor->textId;
        }

        if (player->stateFlags1 & PLAYER_STATE1_RIDING_HORSE) {
            s32 sp24 = player->genericTimer;

            Player_UnequipItem(play, player);
            Player_SetupTalkWithActor(play, player);
            player->genericTimer = sp24;
        } else {
            if (Player_IsSwimming(player)) {
                func_80832558(play, player, Player_SetupTalkWithActor);
                Player_ChangeAnimLongMorphLoop(play, player, &gPlayerAnim_link_swimer_swim_wait);
            } else if ((actor->category != ACTORCAT_NPC) || (player->itemActionParam == PLAYER_AP_FISHING_ROD)) {
                Player_SetupTalkWithActor(play, player);

                if (!Player_IsUnfriendlyZTargeting(player)) {
                    if ((actor != player->tatlActor) && (actor->xzDistToPlayer < (actor->colChkInfo.cylRadius + 40))) {
                        Player_PlayAnimOnceSlowed(play, player, &gPlayerAnim_link_normal_backspace);
                    } else {
                        Player_AnimationPlayLoop(play, player, Player_GetStandingStillAnim(player));
                    }
                }
            } else {
                func_80832558(play, player, Player_SetupTalkWithActor);
                Player_PlayAnimOnceSlowed(play, player,
                                          (actor->xzDistToPlayer < (actor->colChkInfo.cylRadius + 40))
                                              ? &gPlayerAnim_link_normal_backspace
                                              : &gPlayerAnim_link_normal_talk_free);
            }

            if (player->skelAnime.animation == &gPlayerAnim_link_normal_backspace) {
                Player_SetupAnimMovement(play, player, 1 | 8 | ANIM_FLAG_NOMOVE);
            }
            Player_ClearAttentionModeAndStopMoving(player);
        }

        player->stateFlags1 |= PLAYER_STATE1_40 | PLAYER_STATE1_IN_CUTSCENE;
    }

    if ((player->tatlActor == player->talkActor) && ((player->talkActor->textId & 0xFF00) != 0x200)) {
        player->tatlActor->flags |= ACTOR_FLAG_100;
    }
}

void func_8085B74C(PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 temp_fv1;
    f32 linearVelocity = player->linearVelocity;

    if (linearVelocity < 0.0f) {
        linearVelocity = -linearVelocity;
        player->actor.world.rot.y += 0x8000;
    }

    temp_fv1 = R_RUN_SPEED_LIMIT / 100.0f;

    if (temp_fv1 < linearVelocity) {
        gSaveContext.entranceSpeed = temp_fv1;
    } else {
        gSaveContext.entranceSpeed = linearVelocity;
    }

    Player_SetupMiniCsMovement(play, player, 400.0f,
                               (sPlayerConveyorSpeedIndex != BG_CONVEYOR_SPEED_DISABLED) ? D_80862B16
                                                                                         : player->actor.world.rot.y);
    player->stateFlags1 |= (PLAYER_STATE1_1 | PLAYER_STATE1_IN_CUTSCENE);
}

void func_8085B820(PlayState* play, s16 arg1) {
    Player* player = GET_PLAYER(play);

    player->actor.focus.rot.y = arg1;
    Player_ResetLookAngles(player);
}

PlayerActionParam func_8085B854(PlayState* play, Player* player, ItemId itemId) {
    PlayerActionParam actionParam = Player_ItemToActionParam(player, itemId);

    if ((actionParam >= PLAYER_AP_MASK_TRUTH) && (actionParam <= PLAYER_AP_MASK_DEKU) &&
        (actionParam == (player->currentMask + PLAYER_AP_MASK_TRUTH - 1))) {
        actionParam = PLAYER_AP_NONE;
    }

    if ((actionParam <= PLAYER_AP_NONE) || (actionParam >= PLAYER_AP_MAX)) {
        return -1;
    }

    player->heldItemActionParam = PLAYER_AP_NONE;
    player->actionFunc = NULL;
    func_80831760(play, player, Player_PresentExchangeItem, 0);
    player->unk_A86 = 0x7C;
    player->heldItemActionParam = actionParam;
    Player_AnimationPlayOnce(play, player, &gPlayerAnim_link_normal_give_other);
    player->stateFlags1 |= (PLAYER_STATE1_40 | PLAYER_STATE1_IN_CUTSCENE);
    player->getItemDrawId = GID_BOTTLE;
    player->exchangeItemId = actionParam;

    return actionParam;
}

s32 func_8085B930(PlayState* play, LinkAnimationHeader* talkAnim, s32 animMode) {
    Player* player = GET_PLAYER(play);

    if (!(player->actor.flags & ACTOR_FLAG_100)) {
        return false;
    }

    LinkAnimation_Change(play, &player->skelAnime, (talkAnim == NULL) ? Player_GetStandingStillAnim(player) : talkAnim,
                         2.0f / 3.0f, 0.0f, Animation_GetLastFrame(talkAnim), animMode, -6.0f);
    return true;
}
