#ifndef Z64ACTOR_H
#define Z64ACTOR_H

#include "PR/ultratypes.h"
#include "z64math.h"
#include "z64animation.h"
#include "z64collision_check.h"
#include "unk.h"

// This value is hardcoded to be the size of ovl_Arrow_Fire which currently is the biggest actor that uses the AM_FIELD.
#define AM_FIELD_SIZE SEGMENT_SIZE(ovl_Arrow_Fire)

#define MASS_IMMOVABLE 0xFF // Cannot be pushed by OC collisions
#define MASS_HEAVY 0xFE     // Can only be pushed by OC collisions with IMMOVABLE and HEAVY objects.

struct Actor;
struct PlayState;
struct Lights;
struct CollisionPoly;

struct EnBox;

typedef void(*ActorFunc)(struct Actor* this, struct PlayState* play);

typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ Vec3s rot;
} PosRot; // size = 0x14

typedef struct {
    /* 0x00 */ u8 attack[32];
} DamageTable; // size = 0x20

typedef struct {
    /* 0x0 */ u8 health;
    /* 0x2 */ s16 cylRadius;
    /* 0x4 */ s16 cylHeight;
    /* 0x6 */ u8 mass;
} CollisionCheckInfoInit; // size = 0x8

typedef struct {
    /* 0x0 */ u8 health;
    /* 0x2 */ s16 cylRadius;
    /* 0x4 */ s16 cylHeight;
    /* 0x6 */ s16 cylYShift;
    /* 0x8 */ u8 mass;
} CollisionCheckInfoInit2; // size = 0xC

typedef struct {
    /* 0x00 */ DamageTable* damageTable;
    /* 0x04 */ Vec3f displacement;
    /* 0x10 */ s16 cylRadius;
    /* 0x12 */ s16 cylHeight;
    /* 0x14 */ s16 cylYShift;
    /* 0x16 */ u8 mass;
    /* 0x17 */ u8 health;
    /* 0x18 */ u8 damage;
    /* 0x19 */ u8 damageEffect;
    /* 0x1A */ u8 atHitEffect;
    /* 0x1B */ u8 acHitEffect;
} CollisionCheckInfo; // size = 0x1C

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s16 unk34;
    /* 0x36 */ s16 unk36;
    /* 0x38 */ s16 unk38;
    /* 0x3A */ s16 unk3A;
} ActorEnTest20C; // size = 0x3C

typedef struct {
    /* 0x0 */ s16 unk_0; // frame?
    /* 0x2 */ Vec3s unk_2; // scale
} struct_80124618; // size = 0x8

typedef struct {
    /* 0x00 */ s16 id;
    /* 0x02 */ u8 type;
    /* 0x04 */ u32 flags;
    /* 0x08 */ s16 objectId;
    /* 0x0C */ u32 instanceSize;
    /* 0x10 */ ActorFunc init;
    /* 0x14 */ ActorFunc destroy;
    /* 0x18 */ ActorFunc update;
    /* 0x1C */ ActorFunc draw;
} ActorInit; // size = 0x20

typedef enum {
    ALLOCTYPE_NORMAL,
    ALLOCTYPE_ABSOLUTE,
    ALLOCTYPE_PERMANENT
} AllocType;

typedef struct {
    /* 0x00 */ uintptr_t vromStart;
    /* 0x04 */ uintptr_t vromEnd;
    /* 0x08 */ void* vramStart;
    /* 0x0C */ void* vramEnd;
    /* 0x10 */ void* loadedRamAddr; // original name: "allocp"
    /* 0x14 */ ActorInit* initInfo;
    /* 0x18 */ char* name;
    /* 0x1C */ u16 allocType; // bit 0: don't allocate memory, use actorContext->0x250? bit 1: Always keep loaded?
    /* 0x1E */ s8 numLoaded; // original name: "clients"
} ActorOverlay; // size = 0x20

typedef void(*ActorShadowFunc)(struct Actor* actor, struct Lights* mapper, struct PlayState* play);

typedef struct {
    /* 0x00 */ Vec3s rot; // Current actor shape rotation
    /* 0x06 */ s16 face; // Used to index eyebrow/eye/mouth textures. Only used by player
    /* 0x08 */ f32 yOffset; // Model y axis offset. Represents model space units
    /* 0x0C */ ActorShadowFunc shadowDraw; // Shadow draw function
    /* 0x10 */ f32 shadowScale; // Changes the size of the shadow
    /* 0x14 */ u8 shadowAlpha; // Default is 255
    /* 0x15 */ u8 feetFloorFlags; // Set if the actor's foot is clipped under the floor. & 1 is right foot, & 2 is left
    /* 0x16 */ u8 unk_16;
    /* 0x17 */ u8 unk_17;
    /* 0x18 */ Vec3f feetPos[2]; // Update by using `Actor_SetFeetPos` in PostLimbDrawOpa
} ActorShape; // size = 0x30

typedef struct Actor {
    /* 0x000 */ s16 id; // Actor ID
    /* 0x002 */ u8 category; // Actor category. Refer to the corresponding enum for values
    /* 0x003 */ s8 room; // Room number the actor is in. -1 denotes that the actor won't despawn on a room change
    /* 0x004 */ u32 flags; // Flags used for various purposes
    /* 0x008 */ PosRot home; // Initial position/rotation when spawned. Can be used for other purposes
    /* 0x01C */ s16 params; // Configurable variable set by the actor's spawn data; original name: "args_data"
    /* 0x01E */ s8 objBankIndex; // Object bank index of the actor's object dependency; original name: "bank"
    /* 0x01F */ s8 targetMode; // Controls how far the actor can be targeted from and how far it can stay locked on
    /* 0x020 */ s16 unk20;
    /* 0x024 */ PosRot world; // Position/rotation in the world
    /* 0x038 */ s8 cutscene;
    /* 0x039 */ u8 audioFlags; // Another set of flags? Seems related to sfx or bgm
    /* 0x03C */ PosRot focus; // Target reticle focuses on this position. For player this represents head pos and rot
    /* 0x050 */ u16 sfxId; // Id of sound effect to play. Plays when value is set, then is cleared the following update cycle
    /* 0x054 */ f32 targetArrowOffset; // Height offset of the target arrow relative to `focus` position
    /* 0x058 */ Vec3f scale; // Scale of the actor in each axis
    /* 0x064 */ Vec3f velocity; // Velocity of the actor in each axis
    /* 0x070 */ f32 speedXZ; // How fast the actor is traveling along the XZ plane
    /* 0x074 */ f32 gravity; // Acceleration due to gravity. Value is added to Y velocity every frame
    /* 0x078 */ f32 terminalVelocity; // Sets the lower bounds cap on velocity along the Y axis
    /* 0x07C */ struct CollisionPoly* wallPoly; // Wall polygon the actor is touching
    /* 0x080 */ struct CollisionPoly* floorPoly; // Floor polygon directly below the actor
    /* 0x084 */ u8 wallBgId; // Bg ID of the wall polygon the actor is touching
    /* 0x085 */ u8 floorBgId; // Bg ID of the floor polygon directly below the actor
    /* 0x086 */ s16 wallYaw; // Y rotation of the wall polygon the actor is touching
    /* 0x088 */ f32 floorHeight; // Y position of the floor polygon directly below the actor
    /* 0x08C */ f32 depthInWater; // Directed distance to the surface of active waterbox. Negative value means water is below.
    /* 0x090 */ u16 bgCheckFlags; // See comments below actor struct for wip docs. TODO: macros for these flags
    /* 0x092 */ s16 yawTowardsPlayer; // Y rotation difference between the actor and the player
    /* 0x094 */ f32 xyzDistToPlayerSq; // Squared distance between the actor and the player in the x,y,z axis
    /* 0x098 */ f32 xzDistToPlayer; // Distance between the actor and the player in the XZ plane
    /* 0x09C */ f32 playerHeightRel; // Directed distance is negative if the player is below.
    /* 0x0A0 */ CollisionCheckInfo colChkInfo; // Variables related to the Collision Check system
    /* 0x0BC */ ActorShape shape; // Variables related to the physical shape of the actor
    /* 0x0EC */ Vec3f projectedPos; // Position of the actor in projected space
    /* 0x0F8 */ f32 projectedW; // w component of the projected actor position
    /* 0x0FC */ f32 uncullZoneForward; // Amount to increase the uncull zone forward by (in projected space)
    /* 0x100 */ f32 uncullZoneScale; // Amount to increase the uncull zone scale by (in projected space)
    /* 0x104 */ f32 uncullZoneDownward; // Amount to increase uncull zone downward by (in projected space)
    /* 0x108 */ Vec3f prevPos; // World position from the previous update cycle
    /* 0x114 */ u8 isTargeted; // Set to true if the actor is currently being targeted by the player
    /* 0x115 */ u8 targetPriority; // Lower values have higher priority. Resets to 0 when player stops targeting
    /* 0x116 */ u16 textId; // Text ID to pass to link/display when interacting with the actor
    /* 0x118 */ u16 freezeTimer; // Actor does not update when set. Timer decrements automatically
    /* 0x11A */ u16 colorFilterParams; // Set color filter to red, blue, or white. Toggle opa or xlu
    /* 0x11C */ u8 colorFilterTimer; // A non-zero value enables the color filter. Decrements automatically
    /* 0x11D */ u8 isDrawn; // Set to true if the actor is currently being drawn. Always stays false for lens actors
    /* 0x11E */ u8 dropFlag; // Configures what item is dropped by the actor from `Item_DropCollectibleRandom`
    /* 0x11F */ u8 hintId; // Sets what 0600 dialog to display when talking to Tatl. Default 0xFF
    /* 0x120 */ struct Actor* parent; // Usage is actor specific. Set if actor is spawned via `Actor_SpawnAsChild`
    /* 0x124 */ struct Actor* child; // Usage is actor specific. Set if actor is spawned via `Actor_SpawnAsChild`
    /* 0x128 */ struct Actor* prev; // Previous actor of this category
    /* 0x12C */ struct Actor* next; // Next actor of this category
    /* 0x130 */ ActorFunc init; // Initialization Routine. Called by `Actor_InitContext` or `Actor_UpdateAll`
    /* 0x134 */ ActorFunc destroy; // Destruction Routine. Called by `Actor_Destroy`
    /* 0x138 */ ActorFunc update; // Update Routine. Called by `Actor_UpdateAll`
    /* 0x13C */ ActorFunc draw; // Draw Routine. Called by `Actor_Draw`
    /* 0x140 */ ActorOverlay* overlayEntry; // Pointer to the overlay table entry for this actor
} Actor; // size = 0x144

typedef enum {
    /* 0 */ FOOT_LEFT,
    /* 1 */ FOOT_RIGHT
} ActorFootIndex;

/**
 * BgCheckFlags WIP documentation (logical masks):
 * 0x001 : Standing on the ground
 * 0x002 : Has touched the ground (only active for 1 frame)
 * 0x004 : Has left the ground (only active for 1 frame)
 * 0x008 : Touching a wall
 * 0x010 : Touching a ceiling
 * 0x020 : On or below water surface
 * 0x040 : Has touched water (actor is responsible for unsetting this the frame it touches the water)
 * 0x080 : Similar to & 0x1 but with no velocity check and is cleared every frame
 * 0x100 : Crushed between a floor and ceiling (triggers a void for player)
 * 0x200 : Unknown (only set/used by player so far)
 */

typedef struct {
    /* 0x000 */ Actor actor;
    /* 0x144 */ s32 bgId;
    /* 0x148 */ f32 pushForce;
    /* 0x14C */ f32 unk14C;
    /* 0x150 */ s16 yRotation;
    /* 0x152 */ u16 unk152;
    /* 0x154 */ u32 flags;
    /* 0x158 */ u8 stateFlags;
    /* 0x15A */ s16 pad15A;
} DynaPolyActor; // size = 0x15C


typedef enum {
    /* 0x00 */ ITEM00_RUPEE_GREEN,
    /* 0x01 */ ITEM00_RUPEE_BLUE,
    /* 0x02 */ ITEM00_RUPEE_RED,
    /* 0x03 */ ITEM00_RECOVERY_HEART,
    /* 0x04 */ ITEM00_BOMBS_A,
    /* 0x05 */ ITEM00_ARROWS_10,
    /* 0x06 */ ITEM00_HEART_PIECE,
    /* 0x07 */ ITEM00_HEART_CONTAINER,
    /* 0x08 */ ITEM00_ARROWS_30,
    /* 0x09 */ ITEM00_ARROWS_40,
    /* 0x0A */ ITEM00_ARROWS_50,
    /* 0x0B */ ITEM00_BOMBS_B,
    /* 0x0C */ ITEM00_NUTS_1,
    /* 0x0D */ ITEM00_STICK,
    /* 0x0E */ ITEM00_MAGIC_LARGE,
    /* 0x0F */ ITEM00_MAGIC_SMALL,
    /* 0x10 */ ITEM00_MASK,
    /* 0x11 */ ITEM00_SMALL_KEY,
    /* 0x12 */ ITEM00_FLEXIBLE,
    /* 0x13 */ ITEM00_RUPEE_HUGE,
    /* 0x14 */ ITEM00_RUPEE_PURPLE,
    /* 0x15 */ ITEM00_3_HEARTS,
    /* 0x16 */ ITEM00_SHIELD_HERO,
    /* 0x17 */ ITEM00_NUTS_10,
    /* 0x18 */ ITEM00_NOTHING,
    /* 0x19 */ ITEM00_BOMBS_0,
    /* 0x1A */ ITEM00_BIG_FAIRY,
    /* 0x1B */ ITEM00_MAP,
    /* 0x1C */ ITEM00_COMPASS,
    /* 0x1D */ ITEM00_MUSHROOM_CLOUD,

    /* 0xFF */ ITEM00_NO_DROP = -1
} Item00Type;

struct EnItem00;

typedef void (*EnItem00ActionFunc)(struct EnItem00*, struct PlayState*);

#define ENITEM00_GET_8000(thisx) ((thisx)->params & 0x8000)
#define ENITEM00_GET_7F00(thisx) (((thisx)->params & 0x7F00) >> 8)

typedef struct EnItem00 {
    /* 0x000 */ Actor actor;
    /* 0x144 */ EnItem00ActionFunc actionFunc;
    /* 0x148 */ s16 collectibleFlag;
    /* 0x14A */ s16 getItemId;
    /* 0x14C */ s16 unk14C;
    /* 0x14E */ s16 unk14E;
    /* 0x150 */ s16 unk150;
    /* 0x152 */ s16 unk152;
    /* 0x154 */ f32 unk154;
    /* 0x158 */ ColliderCylinder collider;
    /* 0x1A4 */ s8 unk1A4;
} EnItem00; // size = 0x1A8

struct EnAObj;

typedef void (*EnAObjActionFunc)(struct EnAObj*, struct PlayState*);

typedef struct EnAObj {
    /* 0x000 */ Actor actor;
    /* 0x144 */ EnAObjActionFunc actionFunc;
    /* 0x148 */ ColliderCylinder collision;
    /* 0x194 */ UNK_TYPE1 pad194[0x14];
} EnAObj; // size = 0x1A8

typedef enum {
    /* 0x00 */ ACTORCAT_SWITCH,
    /* 0x01 */ ACTORCAT_BG,
    /* 0x02 */ ACTORCAT_PLAYER,
    /* 0x03 */ ACTORCAT_EXPLOSIVES,
    /* 0x04 */ ACTORCAT_NPC,
    /* 0x05 */ ACTORCAT_ENEMY,
    /* 0x06 */ ACTORCAT_PROP,
    /* 0x07 */ ACTORCAT_ITEMACTION,
    /* 0x08 */ ACTORCAT_MISC,
    /* 0x09 */ ACTORCAT_BOSS,
    /* 0x0A */ ACTORCAT_DOOR,
    /* 0x0B */ ACTORCAT_CHEST,
    /* 0x0C */ ACTORCAT_MAX
} ActorType;

#define ACTORCTX_FLAG_0 (1 << 0)
#define ACTORCTX_FLAG_1 (1 << 1)
#define ACTORCTX_FLAG_2 (1 << 2)
#define ACTORCTX_FLAG_3 (1 << 3)
#define ACTORCTX_FLAG_4 (1 << 4)
#define ACTORCTX_FLAG_5 (1 << 5)
#define ACTORCTX_FLAG_6 (1 << 6)
#define ACTORCTX_FLAG_7 (1 << 7)


typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ Color_RGBA8 color;
} TargetContextEntry; // size = 0x14

typedef struct TargetContext {
    /* 0x00 */ Vec3f unk0;
    /* 0x0C */ Vec3f targetCenterPos;
    /* 0x18 */ Color_RGBAf fairyInner;
    /* 0x28 */ Color_RGBAf fairyOuter;
    /* 0x38 */ Actor* arrowPointedActor;
    /* 0x3C */ Actor* targetedActor;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ s16 unk48; // alpha
    /* 0x4A */ u8 unk4A;
    /* 0x4B */ u8 unk4B;
    /* 0x4C */ s8 unk4C;
    /* 0x4D */ UNK_TYPE1 pad4D[0x3];
    /* 0x50 */ TargetContextEntry unk50[3];
    /* 0x8C */ Actor* unk8C;
    /* 0x90 */ Actor* bgmEnemy;
    /* 0x94 */ Actor* unk_94;
} TargetContext; // size = 0x98

typedef struct {
    /* 0x0 */ TexturePtr texture;
    /* 0x4 */ s16 x;
    /* 0x6 */ s16 y;
    /* 0x8 */ u8 width;
    /* 0x9 */ u8 height;
    /* 0xA */ u8 durationTimer; // how long the title card appears for before fading
    /* 0xB */ u8 delayTimer; // how long the title card waits to appear
    /* 0xC */ s16 alpha;
    /* 0xE */ s16 intensity;
} TitleCardContext; // size = 0x10

typedef struct ActorContext_unk_20C {
    /* 0x0 */ s16 id;
    /* 0x2 */ s8 isDynamicallyInitialised;
    /* 0x4 */ void* ptr;
} ActorContext_unk_20C; // size = 0x8

typedef struct ActorContextSceneFlags {
    /* 0x00 */ u32 switches[4]; // First 0x40 are permanent, second 0x40 are temporary
    /* 0x10 */ u32 chest;
    /* 0x14 */ u32 clearedRoom;
    /* 0x18 */ u32 clearedRoomTemp;
    /* 0x1C */ u32 collectible[4]; // bitfield of 128 bits
} ActorContextSceneFlags; // size = 0x2C

typedef struct ActorListEntry {
    /* 0x0 */ s32 length; // number of actors loaded of this type
    /* 0x4 */ Actor* first; // pointer to first actor of this type
    /* 0x8 */ s32 unk_08;
} ActorListEntry; // size = 0xC

// Target size when activated
#define LENS_MASK_ACTIVE_SIZE 100

typedef struct ActorContext {
    /* 0x000 */ u8 freezeFlashTimer;
    /* 0x001 */ UNK_TYPE1 pad1;
    /* 0x002 */ u8 unk2;
    /* 0x003 */ u8 lensActive;
    /* 0x004 */ s8 lensMaskSize; // The size of the circle when drawn the lens mask. Larger value leads to a smaller circle
    /* 0x005 */ u8 flags;
    /* 0x006 */ UNK_TYPE1 pad6[0x5];
    /* 0x00B */ s8 lensActorsDrawn;
    /* 0x00C */ s16 unkC;
    /* 0x00E */ u8 totalLoadedActors;
    /* 0x00F */ u8 undrawnActorCount;
    /* 0x010 */ ActorListEntry actorLists[ACTORCAT_MAX];
    /* 0x0A0 */ Actor* undrawnActors[32]; // Records the first 32 actors drawn each frame
    /* 0x120 */ TargetContext targetContext;
    /* 0x1B8 */ ActorContextSceneFlags sceneFlags;
    /* 0x1E4 */ TitleCardContext titleCtxt;
    /* 0x1F4 */ u8 unk1F4;
    /* 0x1F5 */ u8 unk1F5;
    /* 0x1F6 */ UNK_TYPE1 pad1F6[0x2];
    /* 0x1F8 */ f32 unk1F8;
    /* 0x1FC */ Vec3f unk1FC;
    /* 0x208 */ UNK_TYPE1 unk_208[0x4];
    /* 0x20C */ ActorContext_unk_20C unk_20C[8];
    /* 0x24C */ UNK_TYPE1 unk_24C[0x4];
    /* 0x250 */ void* absoluteSpace; // Space used to allocate actor overlays of alloc type ALLOCTYPE_ABSOLUTE
    /* 0x254 */ u32 unk254[5];
    /* 0x268 */ u8 unk268;
    /* 0x269 */ UNK_TYPE1 pad269[0x3];
    /* 0x26C */ Input unk_26C;
} ActorContext; // size = 0x284

typedef enum {
    /*  0 */ ACTOR_DRAW_DMGEFF_FIRE,
    /*  1 */ ACTOR_DRAW_DMGEFF_BLUE_FIRE,
    /* 10 */ ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX = 10,
    /* 11 */ ACTOR_DRAW_DMGEFF_FROZEN_SFX,
    /* 20 */ ACTOR_DRAW_DMGEFF_LIGHT_ORBS = 20,
    /* 21 */ ACTOR_DRAW_DMGEFF_BLUE_LIGHT_ORBS,
    /* 30 */ ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_SMALL = 30,
    /* 31 */ ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_MEDIUM,
    /* 32 */ ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_LARGE
} ActorDrawDamageEffectType;


#define DEFINE_ACTOR(_name, enumValue, _allocType, _debugName) enumValue,
#define DEFINE_ACTOR_INTERNAL(_name, enumValue, _allocType, _debugName) enumValue,
#define DEFINE_ACTOR_UNSET(enumValue) enumValue,

typedef enum ActorId {
    #include "tables/actor_table.h"
    /* 0x2B2 */ ACTOR_ID_MAX // originally "ACTOR_DLF_MAX"
} ActorId;

#undef DEFINE_ACTOR
#undef DEFINE_ACTOR_INTERNAL
#undef DEFINE_ACTOR_UNSET

typedef enum {
    /* 0 */ DOORLOCK_NORMAL,
    /* 1 */ DOORLOCK_BOSS,
    /* 2 */ DOORLOCK_2, // DOORLOCK_NORMAL_SPIRIT on OoT
    /* 3 */ DOORLOCK_MAX
} DoorLockType;

// Targetability / ACTOR_FLAG_TARGETABLE?
#define ACTOR_FLAG_1             (1 << 0)
// 
#define ACTOR_FLAG_2             (1 << 1)
// 
#define ACTOR_FLAG_4             (1 << 2)
// 
#define ACTOR_FLAG_8             (1 << 3)
// 
#define ACTOR_FLAG_10            (1 << 4)
// 
#define ACTOR_FLAG_20            (1 << 5)
// 
#define ACTOR_FLAG_40            (1 << 6)
// Invisible
#define ACTOR_FLAG_80            (1 << 7)
// Related to talk
#define ACTOR_FLAG_100           (1 << 8)
// 
#define ACTOR_FLAG_200           (1 << 9)
// 
#define ACTOR_FLAG_400           (1 << 10)
// 
#define ACTOR_FLAG_800           (1 << 11)
// 
#define ACTOR_FLAG_1000          (1 << 12)
// 
#define ACTOR_FLAG_2000          (1 << 13)
// 
#define ACTOR_FLAG_4000          (1 << 14)
//! Carried by arrow
#define ACTOR_FLAG_8000          (1 << 15)
// 
#define ACTOR_FLAG_10000         (1 << 16)
// 
#define ACTOR_FLAG_20000         (1 << 17)
// 
#define ACTOR_FLAG_40000         (1 << 18)
// 
#define ACTOR_FLAG_80000         (1 << 19)
// 
#define ACTOR_FLAG_100000        (1 << 20)
// 
#define ACTOR_FLAG_200000        (1 << 21)
// 
#define ACTOR_FLAG_400000        (1 << 22)
// 
#define ACTOR_FLAG_800000        (1 << 23)
// 
#define ACTOR_FLAG_1000000       (1 << 24)
// 
#define ACTOR_FLAG_2000000       (1 << 25)
// 
#define ACTOR_FLAG_4000000       (1 << 26)
// 
#define ACTOR_FLAG_8000000       (1 << 27)
// 
#define ACTOR_FLAG_10000000      (1 << 28)
// 
#define ACTOR_FLAG_20000000      (1 << 29)
// 
#define ACTOR_FLAG_40000000      (1 << 30)
// 
#define ACTOR_FLAG_80000000      (1 << 31)

typedef enum {
    /* 0x00 */ CLEAR_TAG_SMALL_EXPLOSION,
    /* 0x01 */ CLEAR_TAG_LARGE_EXPLOSION,
    /* 0x02 */ CLEAR_TAG_POP,
    /* 0x03 */ CLEAR_TAG_SMALL_LIGHT_RAYS,
    /* 0x04 */ CLEAR_TAG_LARGE_LIGHT_RAYS,
    /* 0x23 */ CLEAR_TAG_SPLASH = 35,
    /* 0xC8 */ CLEAR_TAG_SMOKE = 200
} ClearTagType;

typedef enum {
    /* 0x00 */ TATL_HINT_ID_DEFAULT,
    /* 0x01 */ TATL_HINT_ID_SNAPPER,
    /* 0x02 */ TATL_HINT_ID_MINI_BABA,
    /* 0x03 */ TATL_HINT_ID_MAD_JELLY,
    /* 0x04 */ TATL_HINT_ID_SKULLTULA,
    /* 0x05 */ TATL_HINT_ID_RED_CHUCHU,
    /* 0x06 */ TATL_HINT_ID_BLUE_CHUCHU,
    /* 0x07 */ TATL_HINT_ID_DEKU_BABA,
    /* 0x08 */ TATL_HINT_ID_BIO_DEKU_BABA,
    /* 0x09 */ TATL_HINT_ID_WILTED_DEKU_BABA,
    /* 0x0A */ TATL_HINT_ID_DEXIHAND,
    /* 0x0B */ TATL_HINT_ID_NEJIRON,
    /* 0x0C */ TATL_HINT_ID_GIANT_BEE,
    /* 0x0D */ TATL_HINT_ID_DODONGO,
    /* 0x0E */ TATL_HINT_ID_DEEP_PYTHON,
    /* 0x0F */ TATL_HINT_ID_DEATH_ARMOS,
    /* 0x10 */ TATL_HINT_ID_DINOLFOS,
    /* 0x11 */ TATL_HINT_ID_FIRE_KEESE,
    /* 0x12 */ TATL_HINT_ID_KEESE,
    /* 0x13 */ TATL_HINT_ID_ARMOS,
    /* 0x14 */ TATL_HINT_ID_EENO,
    /* 0x15 */ TATL_HINT_ID_DRAGONFLY,
    /* 0x16 */ TATL_HINT_ID_IGOS_DU_IKANA,
    /* 0x17 */ TATL_HINT_ID_GARO,
    /* 0x18 */ TATL_HINT_ID_GARO_MASTER,
    /* 0x19 */ TATL_HINT_ID_WART,
    /* 0x1A */ TATL_HINT_ID_GOMESS,
    /* 0x1B */ TATL_HINT_ID_GOHT,
    /* 0x1C */ TATL_HINT_ID_BLUE_BUBBLE,
    /* 0x1D */ TATL_HINT_ID_KINGS_LACKEYS_DARK_ROOM,
    /* 0x1E */ TATL_HINT_ID_KINGS_LACKEYS_LIGHT_ROOM,
    /* 0x1F */ TATL_HINT_ID_SKULLWALLTULA,
    /* 0x20 */ TATL_HINT_ID_GOLD_SKULLTULA, // Only in JP Ver.
    /* 0x21 */ TATL_HINT_ID_CAPTAIN_KEETA,
    /* 0x22 */ TATL_HINT_ID_YELLOW_CHUCHU,
    /* 0x23 */ TATL_HINT_ID_TAKKURI,
    /* 0x24 */ TATL_HINT_ID_RED_BUBBLE,
    /* 0x25 */ TATL_HINT_ID_HIPLOOP,
    /* 0x26 */ TATL_HINT_ID_MASKED_HIPLOOP,
    /* 0x27 */ TATL_HINT_ID_SHELLBLADE,
    /* 0x28 */ TATL_HINT_ID_GYORG, // "if you go near, you'll be eaten!"
    /* 0x29 */ TATL_HINT_ID_GYORG_STUNNED, // "Jump in and attack it!"
    /* 0x2A */ TATL_HINT_ID_REDEAD,
    /* 0x2B */ TATL_HINT_ID_2B,
    /* 0x2C */ TATL_HINT_ID_BLACK_BOE,
    /* 0x2D */ TATL_HINT_ID_GIBDO,
    /* 0x2E */ TATL_HINT_ID_TWINMOLD,
    /* 0x2F */ TATL_HINT_ID_WEARING_GIANTS_MASK, // Twinmold, JP Ver. only
    /* 0x30 */ TATL_HINT_ID_WALLMASTER,
    /* 0x31 */ TATL_HINT_ID_FLOORMASTER,
    /* 0x32 */ TATL_HINT_ID_MAJORAS_MASK,
    /* 0x33 */ TATL_HINT_ID_MAJORAS_INCARNATION,
    /* 0x34 */ TATL_HINT_ID_MAJORAS_WRATH,
    /* 0x35 */ TATL_HINT_ID_IRON_KNUCKLE,
    /* 0x36 */ TATL_HINT_ID_36, // Empty
    /* 0x37 */ TATL_HINT_ID_LIKE_LIKE,
    /* 0x38 */ TATL_HINT_ID_38, // Empty
    /* 0x39 */ TATL_HINT_ID_BEAMOS,
    /* 0x3A */ TATL_HINT_ID_3A, // Empty
    /* 0x3B */ TATL_HINT_ID_FREEZARD,
    /* 0x3C */ TATL_HINT_ID_WHITE_BOE,
    /* 0x3D */ TATL_HINT_ID_3D, // Empty
    /* 0x3E */ TATL_HINT_ID_3E, // Empty
    /* 0x3F */ TATL_HINT_ID_3F, // Empty
    /* 0x40 */ TATL_HINT_ID_40, // Empty
    /* 0x41 */ TATL_HINT_ID_41, // Empty
    /* 0x42 */ TATL_HINT_ID_OCTOROK,
    /* 0x43 */ TATL_HINT_ID_43, // Empty
    /* 0x44 */ TATL_HINT_ID_POE,
    /* 0x45 */ TATL_HINT_ID_GEKKO_SNAPPER,
    /* 0x46 */ TATL_HINT_ID_BLUE_TEKTITE,
    /* 0x47 */ TATL_HINT_ID_LEEVER,
    /* 0x48 */ TATL_HINT_ID_PEAHAT,
    /* 0x49 */ TATL_HINT_ID_PEAHAT_LARVA,
    /* 0x4A */ TATL_HINT_ID_EYEGORE,
    /* 0x4B */ TATL_HINT_ID_WIZROBE,
    /* 0x4C */ TATL_HINT_ID_WOLFOS,
    /* 0x4D */ TATL_HINT_ID_MAD_SCRUB,
    /* 0x4E */ TATL_HINT_ID_4E, // Empty
    /* 0x4F */ TATL_HINT_ID_4F, // Empty
    /* 0x50 */ TATL_HINT_ID_POE_SISTER_MEG,
    /* 0x51 */ TATL_HINT_ID_POE_SISTER_JO,
    /* 0x52 */ TATL_HINT_ID_POE_SISTER_BETH,
    /* 0x53 */ TATL_HINT_ID_POE_SISTER_AMY,
    /* 0x54 */ TATL_HINT_ID_PIRATE,
    /* 0x55 */ TATL_HINT_ID_STALCHILD,
    /* 0x56 */ TATL_HINT_ID_ICE_KEESE,
    /* 0x57 */ TATL_HINT_ID_WHITE_WOLFOS,
    /* 0x58 */ TATL_HINT_ID_GUAY,
    /* 0x59 */ TATL_HINT_ID_BIG_OCTO,
    /* 0x5A */ TATL_HINT_ID_BIG_POE,
    /* 0x5B */ TATL_HINT_ID_SKULLFISH,
    /* 0x5C */ TATL_HINT_ID_DESBREKO,
    /* 0x5D */ TATL_HINT_ID_GREEN_CHUCHU,
    /* 0x5E */ TATL_HINT_ID_ODOLWA_1,
    /* 0x5F */ TATL_HINT_ID_GEKKO_GIANT_SLIME,
    /* 0x60 */ TATL_HINT_ID_BAD_BAT,
    /* 0x61 */ TATL_HINT_ID_REAL_BOMBCHU,
    /* 0x62 */ TATL_HINT_ID_ODOLWA_2,
    /* 0x63 */ TATL_HINT_ID_ODOLWA_3,
    /* 0x64 */ TATL_HINT_ID_MUSHROOM,
    /* 0xFF */ TATL_HINT_ID_NONE = 0xFF
} TatlHintId;

#endif
