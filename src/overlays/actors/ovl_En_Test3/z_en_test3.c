/*
 * File: z_en_test3.c
 * Overlay: ovl_En_Test3
 * Description: Kafei
 */

#include "prevent_bss_reordering.h"
#include "z_en_test3.h"
#include "objects/object_test3/object_test3.h"
#include "overlays/actors/ovl_En_Door/z_en_door.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "objects/object_mask_ki_tan/object_mask_ki_tan.h"

#define FLAGS (ACTOR_FLAG_10 | ACTOR_FLAG_20 | ACTOR_FLAG_4000000)

#define THIS ((EnTest3*)thisx)

typedef struct {
    /* 0x0 */ s8 scheduleType;
    /* 0x1 */ s8 unk_1_0 : 4;
    /* 0x1 */ s8 unk_1_4 : 4;
} struct_80A41828; // size = 0x2

typedef s32 (*EnTest3UnkFunc2)(EnTest3* this, PlayState* play, struct_80A41828* arg2, ScheduleOutput* scheduleOutput);
typedef s32 (*EnTest3UnkFunc)(EnTest3* this, PlayState* play);

typedef struct {
    /* 0x0 */ EnTest3UnkFunc2 unk_0;
    /* 0x4 */ EnTest3UnkFunc unk_4;
} KafeiScheduleType; // size = 0x8

typedef enum {
    /* 0 */ KAFEI_SCH_TYPE_0,
    /* 1 */ KAFEI_SCH_TYPE_FOLLOW_PATH,
    /* 2 */ KAFEI_SCH_TYPE_2,
    /* 3 */ KAFEI_SCH_TYPE_CHECK_MAIL,
    /* 4 */ KAFEI_SCH_TYPE_RESPOND_TO_BELL,
    /* 5 */ KAFEI_SCH_TYPE_SAKON_HIDEOUT,
    /* 6 */ KAFEI_SCH_TYPE_6,
    /* 7 */ KAFEI_SCH_TYPE_7,
} KafeiScheduleTypeId;

typedef enum {
    /*  0 */ KAFEI_SCH_NONE,
    /*  1 */ KAFEI_SCH_DAY2_CHECK_MAIL,
    /*  2 */ KAFEI_SCH_UNUSED,
    /*  3 */ KAFEI_SCH_DAY2_CURIOSITY_SHOP_WAITING,
    /*  4 */ KAFEI_SCH_DAY3_CURIOSITY_SHOP_WAITING,
    /*  5 */ KAFEI_SCH_DAY3_IKANA_WAITING,
    /*  6 */ KAFEI_SCH_DAY3_INN_NO_ANJU_FINAL,
    /*  7 */ KAFEI_SCH_DAY3_SAKON_HIDEOUT,
    /*  8 */ KAFEI_SCH_DAY3_INN_WITH_ANJU,
    /*  9 */ KAFEI_SCH_DAY3_IKANA_GOING_TO_ANJU,
    /*  10 */ KAFEI_SCH_BELL,
    /*  11 */ KAFEI_SCH_DAY3_EAST_CLOCK_TOWN,
    /*  12 */ KAFEI_SCH_DAY2_CURIOSITY_SHOP_EVENT,
    /*  13 */ KAFEI_SCH_DAY1_ENTER_SOUTH_CLOCKTOWN,
    /*  14 */ KAFEI_SCH_DAY1_LEAVE_SOUTH_CLOCKTOWN,
    /*  15 */ KAFEI_SCH_DAY1_ALLEY,
    /*  16 */ KAFEI_SCH_DAY2_LAUNDRY_POOL_ENTER,
    /*  17 */ KAFEI_SCH_DAY2_LAUNDRY_POOL_EXIT,
    /*  18 */ KAFEI_SCH_DAY3_INN_WALKING_TO_ROOM,
    /*  19 */ KAFEI_SCH_DAY3_INN_NO_ANJU_WAITING,
    /*  20 */ KAFEI_SCH_DAY3_IKANA_DOOR_OPEN,
} KafeiScheduleResult;

typedef struct {
    /* 0x0 */ EnTest3ActionFunc unk_0; // might not actually be an action function
    /* 0x4 */ EnTest3ActionFunc actionFunc;
} struct_80A4168C; // size = 0x8

// Main functions
void EnTest3_Init(Actor* thisx, PlayState* play2);
void EnTest3_Destroy(Actor* thisx, PlayState* play2);
void EnTest3_Update(Actor* thisx, PlayState* play2);
void EnTest3_Draw(Actor* thisx, PlayState* play2);

// Functions used in D_80A4169C and D_80A416C0. Purpose unclear, but related to Schedule
s32 func_80A3E870(EnTest3* this, PlayState* play);
s32 func_80A3E884(EnTest3* this, PlayState* play);
s32 func_80A3E898(EnTest3* this, PlayState* play);
s32 func_80A3E960(EnTest3* this, PlayState* play);
s32 func_80A3E97C(EnTest3* this, PlayState* play);
s32 func_80A3E9DC(EnTest3* this, PlayState* play);
s32 func_80A3EA30(EnTest3* this, PlayState* play);
s32 func_80A3EAC4(EnTest3* this, PlayState* play);
s32 func_80A3EAF8(EnTest3* this, PlayState* play);
s32 func_80A3EB8C(EnTest3* this, PlayState* play);
s32 func_80A3EBFC(EnTest3* this, PlayState* play);
s32 func_80A3EC30(EnTest3* this, PlayState* play);
s32 func_80A3EC44(EnTest3* this, PlayState* play);

// Functions used in sKafeiScheduleType. Purpose unclear, but related to Schedule
s32 EnTest3_Sch0_Noop(EnTest3* this, PlayState* play, struct_80A41828* arg2, ScheduleOutput* scheduleOutput);
s32 EnTest3_Sch1_Noop(EnTest3* this, PlayState* play);
s32 func_80A3F62C(EnTest3* this, PlayState* play, struct_80A41828* arg2, ScheduleOutput* scheduleOutput);
s32 func_80A3F73C(EnTest3* this, PlayState* play);
s32 EnTest3_Sch0_CheckMail(EnTest3* this, PlayState* play, struct_80A41828* arg2, ScheduleOutput* scheduleOutput);
s32 EnTest3_Sch1_CheckMail(EnTest3* this, PlayState* play);
s32 EnTest3_Sch0_RespondToBell(EnTest3* this, PlayState* play, struct_80A41828* arg2, ScheduleOutput* scheduleOutput);
s32 EnTest3_Sch1_RespondToBell(EnTest3* this, PlayState* play);
s32 EnTest3_Sch0_SakonHideout(EnTest3* this, PlayState* play, struct_80A41828* arg2, ScheduleOutput* scheduleOutput);
s32 EnTest3_Sch1_SakonHideout(EnTest3* this, PlayState* play);
s32 func_80A3FDE4(EnTest3* this, PlayState* play, struct_80A41828* arg2, ScheduleOutput* scheduleOutput);
s32 func_80A3FE20(EnTest3* this, PlayState* play);
s32 func_80A3FF10(EnTest3* this, PlayState* play, struct_80A41828* arg2, ScheduleOutput* scheduleOutput);
s32 func_80A3FFD0(EnTest3* this, PlayState* play2);
s32 EnTest3_InitTimePath(EnTest3* this, PlayState* play, struct_80A41828* arg2, ScheduleOutput* scheduleOutput);
s32 EnTest3_FollowTimePath(EnTest3* this, PlayState* play);

// Action functions
void EnTest3_FollowSchedule(EnTest3* this, PlayState* play);
void func_80A40824(EnTest3* this, PlayState* play);
void func_80A4084C(EnTest3* this, PlayState* play);
void func_80A40908(EnTest3* this, PlayState* play);
void func_80A40A6C(EnTest3* this, PlayState* play);

static u8 sScheduleScript[] = {
    /* ===== DAY 1 SCRIPTS ===== */
    /* 0x000 */ SCHEDULE_CMD_CHECK_NOT_IN_DAY_S(1, 0x046 - 0x004),

    // Laundry Pool
    /* 0x004 */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_ALLEY, 0x020 - 0x008),
    /* 0x008 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(6, 50, 7, 20, 0x01A - 0x00E),
    /* 0x00E */ SCHEDULE_CMD_CHECK_BEFORE_TIME_S(7, 30, 0x019 - 0x012),
    /* 0x012 */ SCHEDULE_CMD_CHECK_FLAG_S(WEEKEVENTREG_HIT_KAFEI_BELL, 0x017 - 0x016),
    /* 0x016 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x017 */ SCHEDULE_CMD_RET_VAL_S(KAFEI_SCH_BELL),
    /* 0x019 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x01A */ SCHEDULE_CMD_RET_TIME(6, 50, 7, 20, KAFEI_SCH_DAY1_ALLEY),

    // South Clock Town
    /* 0x020 */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_CLOCKTOWER, 0x045 - 0x024),
    /* 0x024 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(6, 0, 6, 30, 0x03F - 0x02A),
    /* 0x02A */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(6, 30, 6, 35, 0x03D - 0x030),
    /* 0x030 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(6, 35, 6, 50, 0x037 - 0x036),
    /* 0x036 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x037 */ SCHEDULE_CMD_RET_TIME(6, 35, 6, 50, KAFEI_SCH_DAY1_LEAVE_SOUTH_CLOCKTOWN),
    /* 0x03D */ SCHEDULE_CMD_RET_VAL_S(KAFEI_SCH_DAY2_CHECK_MAIL),
    /* 0x03F */ SCHEDULE_CMD_RET_TIME(6, 0, 6, 30, KAFEI_SCH_DAY1_ENTER_SOUTH_CLOCKTOWN),

    // Other Scene
    /* 0x045 */ SCHEDULE_CMD_RET_NONE(),

    /* ===== DAY 2 SCRIPTS ===== */
    /* 0x046 */ SCHEDULE_CMD_CHECK_NOT_IN_DAY_S(2, 0x09B - 0x04A),

    // Kafei Letter has been delivered?
    /* 0x04A */ SCHEDULE_CMD_CHECK_FLAG_S(WEEKEVENTREG_51_02, 0x058 - 0x04E),
    /* 0x04E */ SCHEDULE_CMD_BRANCH_S(0x0),
    // Bell has been hit
    /* 0x050 */ SCHEDULE_CMD_CHECK_FLAG_S(WEEKEVENTREG_HIT_KAFEI_BELL, 0x055 - 0x054),
    /* 0x054 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x055 */ SCHEDULE_CMD_RET_VAL_L(KAFEI_SCH_BELL),

    // Laundry Pool
    /* 0x058 */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_ALLEY, 0x081 - 0x05C),
    /* 0x05C */ SCHEDULE_CMD_CHECK_BEFORE_TIME_S(13, 0, 0x050 - 0x060),
    /* 0x060 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(15, 5, 15, 25, 0x07B - 0x066),
    /* 0x066 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(15, 25, 15, 40, 0x079 - 0x06C),
    /* 0x06C */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(15, 40, 16, 0, 0x073 - 0x072),
    /* 0x072 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x073 */ SCHEDULE_CMD_RET_TIME(15, 40, 16, 0, KAFEI_SCH_DAY2_LAUNDRY_POOL_EXIT),
    /* 0x079 */ SCHEDULE_CMD_RET_VAL_S(KAFEI_SCH_DAY2_CHECK_MAIL),
    /* 0x07B */ SCHEDULE_CMD_RET_TIME(15, 5, 15, 25, KAFEI_SCH_DAY2_LAUNDRY_POOL_ENTER),

    // Curiosity Shop
    /* 0x081 */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_AYASHIISHOP, 0x09A - 0x085),
    /* 0x085 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(15, 55, 16, 10, 0x094 - 0x08B),
    /* 0x08B */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(16, 10, 22, 10, 0x092 - 0x091),
    /* 0x091 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x092 */ SCHEDULE_CMD_RET_VAL_S(KAFEI_SCH_DAY2_CURIOSITY_SHOP_WAITING),
    /* 0x094 */ SCHEDULE_CMD_RET_TIME(15, 55, 16, 10, KAFEI_SCH_DAY2_CURIOSITY_SHOP_EVENT),
    /* 0x09A */ SCHEDULE_CMD_RET_NONE(),

    /* ===== DAY 3 SCRIPTS ===== */
    /* 0x09B */ SCHEDULE_CMD_CHECK_NOT_IN_DAY_L(3, 0x138 - 0x0A0),

    // Events
    /* 0x0A0 */ SCHEDULE_CMD_CHECK_FLAG_L(WEEKEVENTREG_SAVED_BOMB_LADY, 0x12C - 0x0A5),
    /* 0x0A5 */ SCHEDULE_CMD_CHECK_FLAG_L(WEEKEVENTREG_SAKON_EXPLODED, 0x12C - 0x0AA),

    // Sakon's Hideout
    /* 0x0AA */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_SECOM, 0x0B0 - 0x0AE),
    /* 0x0AE */ SCHEDULE_CMD_RET_VAL_S(KAFEI_SCH_DAY3_SAKON_HIDEOUT),

    // Events
    /* 0x0B0 */ SCHEDULE_CMD_CHECK_FLAG_S(WEEKEVENTREG_SAVED_SUN_MASK, 0x0DD - 0x0B4),

    // Ikana Canyon
    /* 0x0B4 */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_IKANA, 0x0DC - 0x0B8),
    /* 0x0B8 */ SCHEDULE_CMD_CHECK_FLAG_S(WEEKEVENTREG_OBTAINED_PENDANT, 0x0BE - 0x0BC),
    /* 0x0BC */ SCHEDULE_CMD_RET_VAL_S(KAFEI_SCH_DAY3_IKANA_WAITING),
    /* 0x0BE */ SCHEDULE_CMD_CHECK_FLAG_S(WEEKEVENTREG_SAKON_HIDEOUT_SPOTTED, 0x0DA - 0x0C2),
    /* 0x0C2 */ SCHEDULE_CMD_CHECK_FLAG_S(WEEKEVENTREG_SAKON_HIDEOUT_OPENED, 0x0CD - 0x0C6),
    /* 0x0C6 */ SCHEDULE_CMD_CHECK_BEFORE_TIME_S(19, 0, 0x0CB - 0x0CA),
    /* 0x0CA */ SCHEDULE_CMD_RET_NONE(),
    /* 0x0CB */ SCHEDULE_CMD_RET_VAL_S(KAFEI_SCH_DAY3_IKANA_WAITING),
    /* 0x0CD */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(19, 0, 19, 10, 0x0D4 - 0x0D3),
    /* 0x0D3 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x0D4 */ SCHEDULE_CMD_RET_TIME(19, 0, 19, 10, KAFEI_SCH_DAY3_IKANA_DOOR_OPEN),
    /* 0x0DA */ SCHEDULE_CMD_RET_VAL_S(KAFEI_SCH_DAY3_IKANA_WAITING),
    /* 0x0DC */ SCHEDULE_CMD_RET_NONE(),

    // Ikana Canyon
    /* 0x0DD */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_IKANA, 0x0E8 - 0x0E1),
    /* 0x0E1 */ SCHEDULE_CMD_CHECK_FLAG_S(WEEKEVENTREG_KAFEI_GOING_TO_ANJU, 0x0E7 - 0x0E5),
    /* 0x0E5 */ SCHEDULE_CMD_RET_VAL_S(KAFEI_SCH_DAY3_IKANA_GOING_TO_ANJU),
    /* 0x0E7 */ SCHEDULE_CMD_RET_NONE(),

    // East Clock Town
    /* 0x0E8 */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_TOWN, 0x0F9 - 0x0EC),
    /* 0x0EC */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(4, 0, 4, 10, 0x0F3 - 0x0F2),
    /* 0x0F2 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x0F3 */ SCHEDULE_CMD_RET_TIME(4, 0, 4, 10, KAFEI_SCH_DAY3_EAST_CLOCK_TOWN),

    // Stock Pot Inn
    /* 0x0F9 */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_YADOYA, 0x12B - 0x0FD),
    /* 0x0FD */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(4, 10, 4, 30, 0x125 - 0x103),
    /* 0x103 */ SCHEDULE_CMD_CHECK_FLAG_S(WEEKEVENTREG_GAVE_PENDANT_TO_ANJU, 0x11C - 0x107),
    /* 0x107 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(4, 30, 4, 45, 0x116 - 0x10D),
    /* 0x10D */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(4, 45, 6, 0, 0x114 - 0x113),
    /* 0x113 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x114 */ SCHEDULE_CMD_RET_VAL_S(KAFEI_SCH_DAY3_INN_NO_ANJU_FINAL),
    /* 0x116 */ SCHEDULE_CMD_RET_TIME(4, 30, 4, 45, KAFEI_SCH_DAY3_INN_NO_ANJU_WAITING),
    /* 0x11C */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(4, 30, 6, 0, 0x123 - 0x122),
    /* 0x122 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x123 */ SCHEDULE_CMD_RET_VAL_S(KAFEI_SCH_DAY3_INN_WITH_ANJU),
    /* 0x125 */ SCHEDULE_CMD_RET_TIME(4, 10, 4, 30, KAFEI_SCH_DAY3_INN_WALKING_TO_ROOM),
    /* 0x12B */ SCHEDULE_CMD_RET_NONE(),

    // Curiosity Shop
    /* 0x12C */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_AYASHIISHOP, 0x137 - 0x130),
    /* 0x130 */ SCHEDULE_CMD_CHECK_FLAG_S(WEEKEVENTREG_OBTAINED_PENDANT, 0x135 - 0x134),
    /* 0x134 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x135 */ SCHEDULE_CMD_RET_VAL_S(KAFEI_SCH_DAY3_CURIOSITY_SHOP_WAITING),
    /* 0x137 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x138 */ SCHEDULE_CMD_RET_NONE(),
};

ActorInit En_Test3_InitVars = {
    ACTOR_EN_TEST3,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_TEST3,
    sizeof(EnTest3),
    (ActorFunc)EnTest3_Init,
    (ActorFunc)EnTest3_Destroy,
    (ActorFunc)EnTest3_Update,
    (ActorFunc)NULL,
};

static struct_80A4168C D_80A4168C[] = {
    { func_80A40A6C, NULL },
    { NULL, NULL },
};

static EnTest3UnkFunc D_80A4169C[] = {
    func_80A3E898, func_80A3E898, func_80A3E884, func_80A3E898, func_80A3E898,
    func_80A3EA30, func_80A3E898, func_80A3E960, func_80A3E870,
};

static EnTest3UnkFunc D_80A416C0[] = {
    func_80A3EAC4, func_80A3EAF8, func_80A3EBFC, func_80A3EC44,
    func_80A3EC30, func_80A3E9DC, func_80A3EB8C, func_80A3E97C,
};

static PlayerAgeProperties sAgeProperties = {
    40.0f,
    60.0f,
    11.0f / 17.0f,
    71.0f,
    50.0f,
    49.0f,
    39.0f,
    27.0f,
    19.0f,
    22.0f,
    32.4f,
    32.0f,
    48.0f,
    11.0f / 17.0f * 70.0f,
    14.0f,
    12.0f,
    55.0f,
    { 0xFFE8, 0x0DED, 0x036C },
    { { 0xFFE8, 0x0D92, 0x035E }, { 0xFFE8, 0x1371, 0x03A9 }, { 0x0008, 0x1256, 0x017C }, { 0x0009, 0x17EA, 0x0167 } },
    { { 0xFFE8, 0x1371, 0x03A9 }, { 0xFFE8, 0x195F, 0x03A9 }, { 0x0009, 0x17EA, 0x0167 }, { 0x0009, 0x1E0D, 0x017C } },
    { { 0x0008, 0x1256, 0x017C }, { 0x0009, 0x17EA, 0x0167 }, { 0xF9C8, 0x1256, 0x017C }, { 0xF9C9, 0x17EA, 0x0167 } },
    0x0020,
    0x0000,
    22.0f,
    29.4343f,
    &gPlayerAnim_clink_demo_Tbox_open,
    &gPlayerAnim_clink_demo_goto_future,
    &gPlayerAnim_clink_demo_return_to_future,
    &gPlayerAnim_clink_normal_climb_startA,
    &gPlayerAnim_clink_normal_climb_startB,
    { &gPlayerAnim_clink_normal_climb_upL, &gPlayerAnim_clink_normal_climb_upR, &gPlayerAnim_link_normal_Fclimb_upL,
      &gPlayerAnim_link_normal_Fclimb_upR },
    { &gPlayerAnim_link_normal_Fclimb_sideL, &gPlayerAnim_link_normal_Fclimb_sideR },
    { &gPlayerAnim_clink_normal_climb_endAL, &gPlayerAnim_clink_normal_climb_endAR },
    { &gPlayerAnim_clink_normal_climb_endBR, &gPlayerAnim_clink_normal_climb_endBL },
};

static EffectBlureInit2 sBlureInit = {
    0, 8, 0, { 255, 255, 255, 255 }, { 255, 255, 255, 64 }, { 255, 255, 255, 0 }, { 255, 255, 255, 0 }, 4,
    0, 2, 0, { 0, 0, 0, 0 },         { 0, 0, 0, 0 },
};

static EffectTireMarkInit sTireMarkInit = {
    0,
    63,
    { 0, 0, 15, 100 },
};

static KafeiScheduleType sKafeiScheduleType[] = {
    { EnTest3_Sch0_Noop, EnTest3_Sch1_Noop },                   // KAFEI_SCH_TYPE_0
    { EnTest3_InitTimePath, EnTest3_FollowTimePath },           // KAFEI_SCH_TYPE_FOLLOW_PATH
    { func_80A3F62C, func_80A3F73C },                           // KAFEI_SCH_TYPE_2
    { EnTest3_Sch0_CheckMail, EnTest3_Sch1_CheckMail },         // KAFEI_SCH_TYPE_CHECK_MAIL
    { EnTest3_Sch0_RespondToBell, EnTest3_Sch1_RespondToBell }, // KAFEI_SCH_TYPE_RESPOND_TO_BELL
    { EnTest3_Sch0_SakonHideout, EnTest3_Sch1_SakonHideout },   // KAFEI_SCH_TYPE_SAKON_HIDEOUT
    { func_80A3FDE4, func_80A3FE20 },                           // KAFEI_SCH_TYPE_6
    { func_80A3FF10, func_80A3FFD0 },                           // KAFEI_SCH_TYPE_7
};

static struct_80A41828 D_80A41828[] = {
    { KAFEI_SCH_TYPE_0, 0, 0 },               // KAFEI_SCH_NONE
    { KAFEI_SCH_TYPE_CHECK_MAIL, -2, 0 },     // KAFEI_SCH_DAY2_CHECK_MAIL
    { KAFEI_SCH_TYPE_2, -1, 0 },              // KAFEI_SCH_2
    { KAFEI_SCH_TYPE_2, -1, 1 },              // KAFEI_SCH_DAY2_CURIOSITY_SHOP_WAITING
    { KAFEI_SCH_TYPE_2, -1, 2 },              // KAFEI_SCH_DAY3_CURIOSITY_SHOP_WAITING
    { KAFEI_SCH_TYPE_2, 1, 3 },               // KAFEI_SCH_DAY3_IKANA_WAITING
    { KAFEI_SCH_TYPE_2, -2, 4 },              // KAFEI_SCH_DAY3_INN_NO_ANJU_FINAL
    { KAFEI_SCH_TYPE_SAKON_HIDEOUT, 1, 0 },   // KAFEI_SCH_DAY3_SAKON_HIDEOUT
    { KAFEI_SCH_TYPE_7, -1, 0 },              // KAFEI_SCH_DAY3_INN_WITH_ANJU
    { KAFEI_SCH_TYPE_6, 2, 0 },               // KAFEI_SCH_DAY3_IKANA_GOING_TO_ANJU
    { KAFEI_SCH_TYPE_RESPOND_TO_BELL, 4, 0 }, // KAFEI_SCH_BELL
    { KAFEI_SCH_TYPE_FOLLOW_PATH, 1, 0 },     // KAFEI_SCH_DAY3_EAST_CLOCK_TOWN
    { KAFEI_SCH_TYPE_FOLLOW_PATH, 1, 0 },     // KAFEI_SCH_DAY2_CURIOSITY_SHOP_EVENT
    { KAFEI_SCH_TYPE_FOLLOW_PATH, 2, 0 },     // KAFEI_SCH_DAY1_ENTER_SOUTH_CLOCKTOWN
    { KAFEI_SCH_TYPE_FOLLOW_PATH, 1, 0 },     // KAFEI_SCH_DAY1_LEAVE_SOUTH_CLOCKTOWN
    { KAFEI_SCH_TYPE_FOLLOW_PATH, 1, 0 },     // KAFEI_SCH_DAY1_ALLEY
    { KAFEI_SCH_TYPE_FOLLOW_PATH, 2, 0 },     // KAFEI_SCH_DAY2_LAUNDRY_POOL_ENTER
    { KAFEI_SCH_TYPE_FOLLOW_PATH, 3, 0 },     // KAFEI_SCH_DAY2_LAUNDRY_POOL_EXIT
    { KAFEI_SCH_TYPE_FOLLOW_PATH, 1, 0 },     // KAFEI_SCH_DAY3_INN_WALKING_TO_ROOM
    { KAFEI_SCH_TYPE_FOLLOW_PATH, 2, 0 },     // KAFEI_SCH_DAY3_INN_NO_ANJU_WAITING
    { KAFEI_SCH_TYPE_FOLLOW_PATH, 1, 0 },     // KAFEI_SCH_DAY3_IKANA_DOOR_OPEN
};

static EnTest3_struct_D78 D_80A41854[] = {
    { 4, 0, 0x2B25 }, { 1, 0, 0x2969 }, { 3, 1, 0x296A },  { 1, 0, 0x296B }, { 5, 1, 0x0000 }, { 8, 0, 0x0000 },
    { 4, 0, 0x2976 }, { 6, 0, 0x2977 }, { 7, 10, 0x2978 }, { 4, 1, 0x0000 }, { 4, 0, 0x2968 }, { 4, 0, 0x297A },
    { 1, 0, 0x145D }, { 1, 0, 0x145E }, { 5, 1, 0x145F },  { 1, 0, 0x145F }, { 5, 0, 0x0000 }, { 4, 0, 0x1460 },
    { 4, 0, 0x145C }, { 4, 0, 0x2913 }, { 4, 0, 0x1465 },
};

s32 sKafeiSakonHideoutState;
s32 D_80A41D24;
Input sKafeiInput;
f32 sKafeiInputStickMagnitude;
s16 sKafeiInputStickYaw;
s32 D_80A41D48;
Vec3f D_80A41D50;
s32 D_80A41D5C;
s32 D_80A41D60;
s32 D_80A41D64;
s32 D_80A41D68;
Vec3f* D_80A41D6C;

s32 EnTest3_SetupAction(EnTest3* this, EnTest3ActionFunc actionFunc) {
    if (actionFunc == this->actionFunc) {
        return false;
    } else {
        this->actionFunc = actionFunc;
        this->unk_D8A = 0;
        this->scheduleResult = KAFEI_SCH_NONE;
        return true;
    }
}

s32 func_80A3E80C(EnTest3* this, PlayState* play, s32 arg2) {
    s32 pad;

    D_80A4168C[arg2].unk_0(this, play);
    if (D_80A4168C[arg2].actionFunc == NULL) {
        // D_80A4168C[arg2].actionFunc is always NULL
        return false;
    } else {
        EnTest3_SetupAction(this, D_80A4168C[arg2].actionFunc);
        return true;
    }
}

s32 func_80A3E870(EnTest3* this, PlayState* play) {
    return true;
}

s32 func_80A3E884(EnTest3* this, PlayState* play) {
    return false;
}

s32 func_80A3E898(EnTest3* this, PlayState* play) {
    u16 textId = this->unk_D78->textId;

    if ((this->unk_D78->unk_0 == 4) && CHECK_WEEKEVENTREG(WEEKEVENTREG_OBTAINED_PENDANT)) {
        func_80151BB4(play, 2);
    }
    if (textId == 0xFFFF) {
        func_801477B4(play);
    } else if (textId) { // != 0
        func_80151938(play, textId);
    }
    if (textId == 0x296B) {
        LinkAnimation_PlayOnceSetSpeed(play, &this->kafeiPlayer.skelAnime, &gPlayerAnim_al_yareyare, 2.0f / 3.0f);
    }
    return false;
}

s32 func_80A3E960(EnTest3* this, PlayState* play) {
    this->unk_D8C = this->unk_D78->unk_1;
    return false;
}

s32 func_80A3E97C(EnTest3* this, PlayState* play) {
    if (DECR(this->unk_D8C) == 0) {
        Message_StartTextbox(play, this->unk_D78->textId, NULL);
        return true;
    }
    return false;
}

s32 func_80A3E9DC(EnTest3* this, PlayState* play) {
    if (ActorCutscene_GetCanPlayNext(this->actorCsId)) {
        ActorCutscene_StartAndSetUnkLinkFields(this->actorCsId, &this->kafeiPlayer.actor);
        return true;
    } else {
        ActorCutscene_SetIntentToPlay(this->actorCsId);
        return false;
    }
}

s32 func_80A3EA30(EnTest3* this, PlayState* play) {
    if (this->unk_D78->textId == 0x145F) {
        Actor* hideoutDoor = SubS_FindActor(play, NULL, ACTORCAT_BG, ACTOR_BG_IKNV_OBJ);

        if (hideoutDoor != NULL) {
            this->kafeiPlayer.targetedActor = hideoutDoor;
        }
    }
    if (this->unk_D78->unk_1 != 0) {
        ActorCutscene_Stop(0x7C);
        ActorCutscene_SetIntentToPlay(this->actorCsId);
        play->msgCtx.msgMode = 0x44;
    }
    return false;
}

s32 func_80A3EAC4(EnTest3* this, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == TEXT_STATE_DONE) {
        return 1;
    }
    return 0;
}

s32 func_80A3EAF8(EnTest3* this, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == TEXT_STATE_5) && Message_ShouldAdvance(play)) {
        if (this->unk_D78->textId == 0x145F) {
            ActorCutscene_Stop(this->actorCsId);
            this->actorCsId = 0x7C;
            ActorCutscene_SetIntentToPlay(this->actorCsId);
            this->kafeiPlayer.targetedActor = &GET_PLAYER(play)->actor;
        }
        return 1;
    }
    return 0;
}

s32 func_80A3EB8C(EnTest3* this, PlayState* play) {
    if (func_80A3EAF8(this, play)) {
        Actor* hideoutObject = SubS_FindActor(play, NULL, ACTORCAT_ITEMACTION, ACTOR_OBJ_NOZOKI);

        if (hideoutObject != NULL) {
            this->kafeiPlayer.targetedActor = hideoutObject;
        }
        play->msgCtx.msgMode = 0x44;
        return 1;
    }
    return 0;
}

s32 func_80A3EBFC(EnTest3* this, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == TEXT_STATE_CLOSING) {
        return 1;
    }
    return 0;
}

s32 func_80A3EC30(EnTest3* this, PlayState* play) {
    return 0;
}

s32 func_80A3EC44(EnTest3* this, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == TEXT_STATE_CHOICE) && Message_ShouldAdvance(play)) {
        if (play->msgCtx.choiceIndex != 0) {
            func_8019F230();
        } else {
            func_8019F208();
        }
        if (play->msgCtx.choiceIndex != 0) {
            return 1;
        } else {
            s32 ret = this->unk_D78->unk_1 + 1;

            return ret;
        }
    }
    return 0;
}

s32 func_80A3ECEC(EnTest3* this, PlayState* play) {
    return D_80A4169C[this->unk_D78->unk_0](this, play);
}

s32 func_80A3ED24(EnTest3* this, PlayState* play) {
    s32 temp = D_80A416C0[this->unk_D78->unk_0](this, play);

    if (temp != 0) {
        this->unk_D78 = &this->unk_D78[temp];
        return func_80A3ECEC(this, play);
    }
    return false;
}

void EnTest3_Init(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EnTest3* this = THIS;
    Camera* subCam;

    if (D_80A41D24) {
        Actor_Kill(&this->kafeiPlayer.actor);
        return;
    }
    D_80A41D24 = true;

    this->kafeiPlayer.actor.room = -1;
    this->kafeiPlayer.unk_A86 = -1;
    this->kafeiPlayer.transformation = PLAYER_FORM_HUMAN;
    this->kafeiPlayer.ageProperties = &sAgeProperties;
    this->kafeiPlayer.heldItemAction = PLAYER_IA_NONE;
    this->kafeiPlayer.heldItemId = ITEM_OCARINA;

    Player_SetModelGroup(&this->kafeiPlayer, 3);
    play->playerInit(&this->kafeiPlayer, play, &object_test3_Skel_00F7EC);

    Effect_Add(play, &this->kafeiPlayer.meleeWeaponEffectIndex[0], EFFECT_BLURE2, 0, 0, &sBlureInit);
    Effect_Add(play, &this->kafeiPlayer.meleeWeaponEffectIndex[1], EFFECT_BLURE2, 0, 0, &sBlureInit);
    Effect_Add(play, &this->kafeiPlayer.meleeWeaponEffectIndex[2], EFFECT_TIRE_MARK, 0, 0, &sTireMarkInit);

    this->kafeiPlayer.maskObjectSegment = ZeldaArena_Malloc(0x3800);
    play->func_18780(&this->kafeiPlayer, play);
    this->linkPlayer = GET_PLAYER(play);
    this->kafeiPlayer.giObjectSegment = this->linkPlayer->giObjectSegment;
    this->kafeiPlayer.tatlActor = this->linkPlayer->tatlActor;
    if ((CURRENT_DAY != 3) || CHECK_WEEKEVENTREG(WEEKEVENTREG_SAVED_BOMB_LADY) ||
        !CHECK_WEEKEVENTREG(WEEKEVENTREG_OBTAINED_PENDANT)) {
        this->kafeiPlayer.currentMask = PLAYER_MASK_KEATON;
    }
    this->kafeiPlayer.prevMask = this->kafeiPlayer.currentMask;

    if (play->sceneId == SCENE_SECOM) {
        this->subCamId = Play_CreateSubCamera(play);
        subCam = Play_GetCamera(play, this->subCamId);
        Camera_InitPlayerSettings(subCam, &this->kafeiPlayer);
        Camera_SetFlags(subCam, CAM_STATE_0 | CAM_STATE_6);
        Play_ChangeCameraStatus(play, this->subCamId, CAM_STATUS_WAIT);
    }

    this->kafeiPlayer.actor.colChkInfo.cylRadius = 20;
    this->kafeiPlayer.actor.colChkInfo.cylHeight = 60;
    this->kafeiPlayer.actor.colChkInfo.health = 255;

    if (KAFEI_GET_PARAM_1E0(&this->kafeiPlayer.actor) == 0) {
        EnTest3_SetupAction(this, func_80A40824);
    } else {
        EnTest3_SetupAction(this, EnTest3_FollowSchedule);
    }
}

void EnTest3_Destroy(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EnTest3* this = THIS;

    Effect_Destroy(play, this->kafeiPlayer.meleeWeaponEffectIndex[0]);
    Effect_Destroy(play, this->kafeiPlayer.meleeWeaponEffectIndex[1]);
    Effect_Destroy(play, this->kafeiPlayer.meleeWeaponEffectIndex[2]);
    Collider_DestroyCylinder(play, &this->kafeiPlayer.cylinder);
    Collider_DestroyCylinder(play, &this->kafeiPlayer.shieldCylinder);
    Collider_DestroyQuad(play, &this->kafeiPlayer.meleeWeaponQuads[0]);
    Collider_DestroyQuad(play, &this->kafeiPlayer.meleeWeaponQuads[1]);
    Collider_DestroyQuad(play, &this->kafeiPlayer.shieldQuad);
    ZeldaArena_Free(this->kafeiPlayer.maskObjectSegment);
    Environment_StartTime();
}

s32 EnTest3_Sch0_Noop(EnTest3* this, PlayState* play, struct_80A41828* arg2, ScheduleOutput* scheduleOutput) {
    return true;
}

s32 EnTest3_Sch1_Noop(EnTest3* this, PlayState* play) {
    this->kafeiPlayer.actor.draw = NULL;
    return true;
}

void EnTest3_ReturnInputToGame(EnTest3* this, PlayState* play) {
    func_800BC154(play, &play->actorCtx, &this->linkPlayer->actor, ACTORCAT_PLAYER);
    func_800BC154(play, &play->actorCtx, &this->kafeiPlayer.actor, ACTORCAT_NPC);
    this->linkPlayer->stateFlags1 &= ~PLAYER_STATE1_20; // PLAYER_STATE1_INPUT_DISABLED
}

void func_80A3F114(EnTest3* this, PlayState* play) {
    if (this->kafeiPlayer.csMode != 0) {
        play->startPlayerCutscene(play, &this->kafeiPlayer, 6);
    }
}

s32 func_80A3F15C(EnTest3* this, PlayState* play, struct_80A41828* arg2) {
    s32 pathIndex;
    Path* path;
    Vec3s* curPathPoint;
    Vec3s* nextPathPoint;
    Vec3f curPathPos;
    Vec3f nextPathPos;

    pathIndex = ABS_ALT(arg2->unk_1_0) - 1;

    if (pathIndex >= 0) {
        path = SubS_GetAdditionalPath(play, KAFEI_GET_PARAM_1F(&this->kafeiPlayer.actor), pathIndex);

        curPathPoint = Lib_SegmentedToVirtual(path->points);
        if (arg2->unk_1_0 > 0) {
            nextPathPoint = curPathPoint + 1;
        } else {
            curPathPoint += path->count - 1;
            nextPathPoint = curPathPoint - 1;
        }
        Math_Vec3s_ToVec3f(&curPathPos, curPathPoint);
        Math_Vec3s_ToVec3f(&nextPathPos, nextPathPoint);
        if (Math_Vec3f_DistXZ(&this->kafeiPlayer.actor.world.pos, &curPathPos) > 10.0f) {
            Math_Vec3f_Copy(&this->kafeiPlayer.actor.world.pos, &curPathPos);
            Math_Vec3f_Copy(&this->kafeiPlayer.actor.home.pos, &curPathPos);
            Math_Vec3f_Copy(&this->kafeiPlayer.actor.prevPos, &curPathPos);
            this->kafeiPlayer.currentYaw = Math_Vec3f_Yaw(&this->kafeiPlayer.actor.world.pos, &nextPathPos);
            if (arg2->unk_1_0 < 0) {
                this->kafeiPlayer.currentYaw += 0x8000;
            }
            this->kafeiPlayer.actor.shape.rot.y = this->kafeiPlayer.currentYaw;
            return true;
        }
    }
    return false;
}

Actor* func_80A3F2BC(PlayState* play, EnTest3* this, s32 actorId, s32 category, f32 arg4, f32 arg5) {
    Actor* actor = play->actorCtx.actorLists[category].first;

    while (actor != NULL) {
        if (actorId == actor->id) {
            f32 dy = this->kafeiPlayer.actor.world.pos.y - actor->world.pos.y;

            if ((fabsf(dy) < arg5) && (Actor_XZDistanceBetweenActors(&this->kafeiPlayer.actor, actor) < arg4)) {
                return actor;
            }
        }
        actor = actor->next;
    }
    return NULL;
}

s32 func_80A3F384(EnTest3* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    EnDoor* door = (EnDoor*)func_80A3F2BC(play, this, ACTOR_EN_DOOR, ACTORCAT_DOOR, 55.0f, 20.0f);
    Vec3f offset;

    if ((door != NULL) && (door->unk_1A1 == 0) &&
        ((player->doorType == 0) || (&door->dyna.actor != player->doorActor)) &&
        Actor_ActorAIsFacingActorB(&this->kafeiPlayer.actor, &door->dyna.actor, 0x3000)) {
        Actor_OffsetOfPointInActorCoords(&door->dyna.actor, &offset, &this->kafeiPlayer.actor.world.pos);
        this->kafeiPlayer.doorType = 1;
        this->kafeiPlayer.doorDirection = (offset.z >= 0.0f) ? 1.0f : -1.0f;
        this->kafeiPlayer.doorActor = &door->dyna.actor;
        this->kafeiPlayer.unk_A86 = -1;
        return true;
    }
    return false;
}

s32 func_80A3F4A4(PlayState* play) {
    return (Player_GetMask(play) == PLAYER_MASK_NONE) || (Player_GetMask(play) == PLAYER_MASK_BUNNY) ||
           (Player_GetMask(play) == PLAYER_MASK_POSTMAN) || (Player_GetMask(play) == PLAYER_MASK_KEATON) ||
           (Player_GetMask(play) == PLAYER_MASK_KAFEIS_MASK);
}

void func_80A3F534(EnTest3* this, PlayState* play) {
    if (!func_80A3F4A4(play)) {
        this->unk_D78 = &D_80A41854[10];
    } else if (CHECK_WEEKEVENTREG(WEEKEVENTREG_OBTAINED_PENDANT)) {
        this->unk_D78 = &D_80A41854[7];
    } else {
        this->unk_D78 = &D_80A41854[1];
    }
    this->actorCsId = this->kafeiPlayer.actor.cutscene;
}

void func_80A3F5A4(EnTest3* this, PlayState* play) {
    if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_OBTAINED_PENDANT) || !func_80A3F4A4(play)) {
        this->unk_D78 = &D_80A41854[18];
        D_80A41D5C = false;
    } else if (D_80A41D5C) {
        this->unk_D78 = &D_80A41854[17];
    } else {
        this->unk_D78 = &D_80A41854[12];
    }
    this->actorCsId = this->kafeiPlayer.actor.cutscene;
}

s32 func_80A3F62C(EnTest3* this, PlayState* play, struct_80A41828* arg2, ScheduleOutput* scheduleOutput) {
    static EnTest3_struct_D78* D_80A418A8[] = {
        &D_80A41854[0], &D_80A41854[1], &D_80A41854[11], &D_80A41854[12], &D_80A41854[19],
    };

    if (((func_80A3F15C(this, play, arg2) || (this->scheduleResult >= KAFEI_SCH_DAY3_INN_WITH_ANJU)) &&
         ((arg2->unk_1_4 == 1) || (arg2->unk_1_4 == 2))) ||
        (arg2->unk_1_4 == 4)) {
        if (arg2->unk_1_4 == 4) {
            this->kafeiPlayer.actor.home.rot.y = 0x7FFF;
        } else {
            this->kafeiPlayer.actor.home.rot.y = this->kafeiPlayer.actor.shape.rot.y + 0x8000;
        }
        this->kafeiPlayer.stateFlags2 |= PLAYER_STATE2_40000;
        play->startPlayerCutscene(play, &this->kafeiPlayer, -1);
    }
    this->unk_D78 = D_80A418A8[arg2->unk_1_4];
    return true;
}

s32 func_80A3F73C(EnTest3* this, PlayState* play) {
    if (Actor_ProcessTalkRequest(&this->kafeiPlayer.actor, &play->state)) {
        EnTest3_SetupAction(this, func_80A4084C);
        this->kafeiPlayer.targetedActor = &GET_PLAYER(play)->actor;
        this->kafeiPlayer.stateFlags2 &= ~PLAYER_STATE2_40000;
        D_80A41D5C = true;
        if ((this->unk_D78->unk_0 == 4) && CHECK_WEEKEVENTREG(WEEKEVENTREG_OBTAINED_PENDANT)) {
            func_80151BB4(play, 2);
        }
    } else {
        if (play->actorCtx.flags & ACTORCTX_FLAG_TOGGLE_KAFEI_CONTROL) {
            play->actorCtx.flags &= ~ACTORCTX_FLAG_TOGGLE_KAFEI_CONTROL;
            this->kafeiPlayer.stateFlags2 &= ~PLAYER_STATE2_40000;
            this->linkPlayer->stateFlags1 |= PLAYER_STATE1_20; // PLAYER_STATE1_INPUT_DISABLED
            func_800BC154(play, &play->actorCtx, &this->linkPlayer->actor, ACTORCAT_NPC);
            func_800BC154(play, &play->actorCtx, &this->kafeiPlayer.actor, ACTORCAT_PLAYER);
            ActorCutscene_SetReturnCamera(this->subCamId);
            play->startPlayerCutscene(play, &this->kafeiPlayer, 7);
        }
        func_800B863C(&this->kafeiPlayer.actor, play);
        if (this->scheduleResult == KAFEI_SCH_DAY2_CURIOSITY_SHOP_WAITING) {
            func_80A3F534(this, play);
        } else if (this->scheduleResult == KAFEI_SCH_DAY3_IKANA_WAITING) {
            func_80A3F5A4(this, play);
        }
        this->kafeiPlayer.actor.textId = this->unk_D78->textId;
        this->kafeiPlayer.actor.flags |= (ACTOR_FLAG_1 | ACTOR_FLAG_8);
    }
    return false;
}

s32 EnTest3_Sch0_CheckMail(EnTest3* this, PlayState* play, struct_80A41828* arg2, ScheduleOutput* scheduleOutput) {
    Actor* postActor;

    func_80A3F15C(this, play, arg2);
    if (((postActor = func_80A3F2BC(play, this, ACTOR_EN_PST, ACTORCAT_PROP, 100.0f, 20.0f)) != NULL) ||
        ((postActor = func_80A3F2BC(play, this, ACTOR_EN_PM, ACTORCAT_NPC, 100.0f, 20.0f)) != NULL)) {
        this->kafeiPlayer.actor.home.rot.y = Actor_YawBetweenActors(&this->kafeiPlayer.actor, postActor);
    }
    play->startPlayerCutscene(play, &this->kafeiPlayer, 0x61);
    return true;
}

s32 EnTest3_Sch1_CheckMail(EnTest3* this, PlayState* play) {
    Math_ScaledStepToS(&this->kafeiPlayer.actor.shape.rot.y, this->kafeiPlayer.actor.home.rot.y, 0x320);
    this->kafeiPlayer.currentYaw = this->kafeiPlayer.actor.shape.rot.y;
    return false;
}

s32 EnTest3_Sch0_RespondToBell(EnTest3* this, PlayState* play, struct_80A41828* arg2, ScheduleOutput* scheduleOutput) {
    scheduleOutput->time0 = SCHEDULE_TIME_NOW;
    scheduleOutput->time1 = (u16)(scheduleOutput->time0 + 70);
    EnTest3_InitTimePath(this, play, arg2, scheduleOutput);
    if (this->kafeiPlayer.actor.xzDistToPlayer < 300.0f) {
        this->unk_D8A = -1;
    } else {
        this->unk_D8A = 120;
    }
    return true;
}

s32 EnTest3_Sch1_RespondToBell(EnTest3* this, PlayState* play) {
    s32 pad;
    Player* player = GET_PLAYER(play);
    u32 cond;
    struct_80A41828 sp40;
    ScheduleOutput scheduleOutput;

    if (player->stateFlags1 & PLAYER_STATE1_40) {
        return false;
    }
    cond = EnTest3_FollowTimePath(this, play);
    if (this->unk_D8A > 0) {
        this->unk_D8A--;
        cond = cond && (this->kafeiPlayer.actor.xzDistToPlayer < 200.0f);
        if (cond || this->unk_D8A <= 0) {
            func_80A3F114(this, play);
            sp40.unk_1_0 = 5;
            scheduleOutput.time0 = SCHEDULE_TIME_NOW;
            scheduleOutput.time1 = (u16)(scheduleOutput.time0 + (cond ? 80 : 140));

            EnTest3_InitTimePath(this, play, &sp40, &scheduleOutput);
            this->unk_D8A = -40;
            return false;
        }
        if (this->unk_D8A == 90) {
            play->startPlayerCutscene(play, &this->kafeiPlayer, 0x15);
        }
    } else {
        this->unk_D8A++;
        if (this->unk_D8A == 0) {
            CLEAR_WEEKEVENTREG(WEEKEVENTREG_HIT_KAFEI_BELL);
            this->scheduleResult = KAFEI_SCH_NONE;
        }
    }
    return false;
}

s32 EnTest3_Sch0_SakonHideout(EnTest3* this, PlayState* play, struct_80A41828* arg2, ScheduleOutput* scheduleOutput) {
    return true;
}

s32 EnTest3_Sch1_SakonHideout(EnTest3* this, PlayState* play) {
    if (sKafeiSakonHideoutState == 0) {
        // Init
        if (!Play_InCsMode(play)) {
            sKafeiSakonHideoutState = 1;
            this->unk_D78 = &D_80A41854[20];
            this->actorCsId = this->kafeiPlayer.actor.cutscene;
            this->kafeiPlayer.actor.textId = this->unk_D78->textId;
        }
    } else if (sKafeiSakonHideoutState == 1) {
        // Update
        if (this->actorCsId >= 0) {
            if (func_80A3E9DC(this, play)) {
                this->actorCsId = -1;
                Environment_StopTime();
            }
        } else if ((play->actorCtx.flags & ACTORCTX_FLAG_SUNMASK_SUCCESS) ||
                   (play->actorCtx.flags & ACTORCTX_FLAG_SUNMASK_FAIL)) {
            this->actorCsId = ActorCutscene_GetAdditionalCutscene(this->kafeiPlayer.actor.cutscene);
            SET_WEEKEVENTREG(WEEKEVENTREG_KAFEI_GOING_TO_ANJU);
            if (play->actorCtx.flags & ACTORCTX_FLAG_SUNMASK_FAIL) {
                this->actorCsId = ActorCutscene_GetAdditionalCutscene(this->actorCsId);
            }
            Audio_QueueSeqCmd(NA_BGM_STOP | 0x10000);
            sKafeiSakonHideoutState = 2;
        } else {
            func_80A3F73C(this, play);
        }
    } else if ((sKafeiSakonHideoutState == 2) && func_80A3E9DC(this, play)) {
        // Finish
        ActorCutscene_SetReturnCamera(CAM_ID_MAIN);
        Environment_StartTime();
        if (((void)0, gSaveContext.save.time) > CLOCK_TIME(6, 0)) {
            Environment_SetTimeJump(TIME_TO_MINUTES_ALT_F(fabsf((s16) - ((void)0, gSaveContext.save.time))));
        }
        if (play->actorCtx.flags & ACTORCTX_FLAG_SUNMASK_SUCCESS) {
            SET_WEEKEVENTREG(WEEKEVENTREG_SAVED_SUN_MASK);
            CLEAR_WEEKEVENTREG(WEEKEVENTREG_KAFEI_GOING_TO_ANJU);
        }
        sKafeiSakonHideoutState = 3;
    }
    return false;
}

s32 func_80A3FDE4(EnTest3* this, PlayState* play, struct_80A41828* arg2, ScheduleOutput* scheduleOutput) {
    this->actorCsId = ActorCutscene_GetAdditionalCutscene(this->kafeiPlayer.actor.cutscene);
    return true;
}

s32 func_80A3FE20(EnTest3* this, PlayState* play) {
    struct_80A41828 sp2C;
    ScheduleOutput scheduleOutput;

    if (D_80A41D64 == 0) {
        if (func_80A3E9DC(this, play)) {
            sp2C.unk_1_0 = 2;
            scheduleOutput.time0 = SCHEDULE_TIME_NOW;
            scheduleOutput.time1 = (u16)(scheduleOutput.time0 + 1000);
            EnTest3_InitTimePath(this, play, &sp2C, &scheduleOutput);
            D_80A41D64 = 1;
            return false;
        }
    } else if (D_80A41D64 == 1) {
        EnTest3_FollowTimePath(this, play);
    } else if (D_80A41D64 == 2) {
        ActorCutscene_Stop(this->actorCsId);
        SET_WEEKEVENTREG(WEEKEVENTREG_KAFEI_GOING_TO_ANJU);
        D_80A41D64 = 3;
    }
    return false;
}

s32 func_80A3FF10(EnTest3* this, PlayState* play, struct_80A41828* arg2, ScheduleOutput* scheduleOutput) {
    static Vec3f D_80A418BC = { -420.0f, 210.0f, -162.0f };

    if (CHECK_WEEKEVENTREG(WEEKEVENTREG_COUPLES_MASK_CS_FINISHED)) {
        D_80A41D68 = 2;
        Math_Vec3f_Copy(&this->kafeiPlayer.actor.world.pos, &D_80A418BC);
        Math_Vec3f_Copy(&this->kafeiPlayer.actor.home.pos, &D_80A418BC);

        this->kafeiPlayer.actor.home.rot.y = -0x2AAB;
        this->kafeiPlayer.actor.shape.rot.y = -0x2AAB;
        this->kafeiPlayer.currentYaw = -0x2AAB;
        if (1) {} // macro?
        return true;
    } else {
        func_80A3F15C(this, play, arg2);
        this->actorCsId = this->kafeiPlayer.actor.cutscene;
        if (play->roomCtx.curRoom.num == 2) {
            this->actorCsId = ActorCutscene_GetAdditionalCutscene(this->actorCsId);
        }
        return true;
    }
}

s32 func_80A3FFD0(EnTest3* this, PlayState* play2) {
    PlayState* play = play2;

    if (D_80A41D68 == 0) {
        if (!Play_InCsMode(play) && (play->roomCtx.curRoom.num == 2)) {
            D_80A41D68 = 1;
        }
    } else if (D_80A41D68 == 1) {
        if (func_80A3E9DC(this, play)) {
            D_80A41D68 = 2;
        }
    } else {
        SET_WEEKEVENTREG(WEEKEVENTREG_COUPLES_MASK_CS_FINISHED);
        play->startPlayerCutscene(play, &this->kafeiPlayer, 0x6E);
    }
    return false;
}

s32 EnTest3_InitTimePath(EnTest3* this, PlayState* play, struct_80A41828* arg2, ScheduleOutput* scheduleOutput) {
    u16 now = SCHEDULE_TIME_NOW;
    u16 startTime;
    u16 numWaypoints;

    func_80A3F15C(this, play, arg2);

    this->timePath =
        SubS_GetAdditionalPath(play, KAFEI_GET_PARAM_1F(&this->kafeiPlayer.actor), ABS_ALT(arg2->unk_1_0) - 1);

    if ((this->scheduleResult <= KAFEI_SCH_DAY3_INN_NO_ANJU_FINAL) && (this->scheduleResult != KAFEI_SCH_NONE) &&
        (this->timePathTimeSpeed >= 0)) {
        startTime = now;
    } else {
        startTime = scheduleOutput->time0;
    }

    if (scheduleOutput->time1 < startTime) {
        this->timePathTotalTime = (startTime - scheduleOutput->time1) + (DAY_LENGTH - 1);
    } else {
        this->timePathTotalTime = scheduleOutput->time1 - startTime;
    }

    this->timePathElapsedTime = now - startTime;

    numWaypoints = startTime = this->timePath->count - (SUBS_TIME_PATHING_ORDER - 1);
    this->timePathWaypointTime = this->timePathTotalTime / numWaypoints;

    this->timePathWaypoint = (this->timePathElapsedTime / this->timePathWaypointTime) + (SUBS_TIME_PATHING_ORDER - 1);
    this->timePathIsSetup &= ~1;
    this->unk_D84 = 1.0f;

    return true;
}

s32 EnTest3_FollowTimePath(EnTest3* this, PlayState* play) {
    f32 knots[265];
    Vec3f worldPos;
    Vec3f sp7C;
    Vec3f timePathPoint;
    s32 prevTimePathElapsedTime = 0;
    s32 prevTimePathWaypoint = 0;
    s32 pad1;
    f32 dx;
    f32 dy;
    s32 ret = false;

    SubS_TimePathing_FillKnots(knots, SUBS_TIME_PATHING_ORDER, this->timePath->count + SUBS_TIME_PATHING_ORDER);

    if (!(this->timePathIsSetup & 1)) {
        timePathPoint = gZeroVec3f;
        SubS_TimePathing_Update(this->timePath, &this->timePathProgress, &this->timePathElapsedTime,
                                this->timePathWaypointTime, this->timePathTotalTime, &this->timePathWaypoint, knots,
                                &timePathPoint, this->timePathTimeSpeed);
        SubS_TimePathing_ComputeInitialY(play, this->timePath, this->timePathWaypoint, &timePathPoint);
        Math_Vec3f_Copy(&this->kafeiPlayer.actor.home.pos, &timePathPoint);
        Math_Vec3f_Copy(&this->kafeiPlayer.actor.prevPos, &timePathPoint);
        this->kafeiPlayer.actor.world.pos.y = timePathPoint.y;
        this->timePathIsSetup |= 1;
    } else {
        timePathPoint = this->timePathTargetPos;
    }

    this->kafeiPlayer.actor.world.pos.x = timePathPoint.x;
    this->kafeiPlayer.actor.world.pos.z = timePathPoint.z;

    if (play->transitionMode != TRANS_MODE_OFF) {
        prevTimePathElapsedTime = this->timePathElapsedTime;
        prevTimePathWaypoint = this->timePathWaypoint;
        timePathPoint = this->kafeiPlayer.actor.world.pos;
    }

    this->timePathTargetPos = gZeroVec3f;

    if (SubS_TimePathing_Update(this->timePath, &this->timePathProgress, &this->timePathElapsedTime,
                                this->timePathWaypointTime, this->timePathTotalTime, &this->timePathWaypoint, knots,
                                &this->timePathTargetPos, this->timePathTimeSpeed)) {
        if (this->scheduleResult == KAFEI_SCH_DAY3_IKANA_DOOR_OPEN) {
            CLEAR_WEEKEVENTREG(WEEKEVENTREG_SAKON_HIDEOUT_IS_OPENING);
            this->kafeiPlayer.actor.draw = NULL;
        } else if (this->scheduleResult == KAFEI_SCH_DAY3_IKANA_GOING_TO_ANJU) {
            D_80A41D64 = 2;
        }
        ret = true;
    } else {
        worldPos = this->kafeiPlayer.actor.world.pos;
        sp7C = this->timePathTargetPos;
        this->kafeiPlayer.actor.world.rot.y = Math_Vec3f_Yaw(&worldPos, &sp7C);
    }

    if (play->transitionMode != TRANS_MODE_OFF) {
        this->timePathElapsedTime = prevTimePathElapsedTime;
        this->timePathWaypoint = prevTimePathWaypoint;
        this->timePathTargetPos = timePathPoint;
    }

    dx = this->kafeiPlayer.actor.world.pos.x - this->kafeiPlayer.actor.prevPos.x;
    dy = this->kafeiPlayer.actor.world.pos.z - this->kafeiPlayer.actor.prevPos.z;

    if (!Math_StepToF(&this->unk_D84, 1.0f, 0.1f)) {
        this->kafeiPlayer.actor.world.pos.x = this->kafeiPlayer.actor.prevPos.x + (dx * this->unk_D84);
        this->kafeiPlayer.actor.world.pos.z = this->kafeiPlayer.actor.prevPos.z + (dy * this->unk_D84);
    }

    Math_Vec3f_Copy(&D_80A41D50, &this->kafeiPlayer.actor.world.pos);

    dx = this->kafeiPlayer.actor.world.pos.x - this->kafeiPlayer.actor.prevPos.x;
    dy = this->kafeiPlayer.actor.world.pos.z - this->kafeiPlayer.actor.prevPos.z;
    this->kafeiPlayer.linearVelocity = sqrtf(SQ(dx) + SQ(dy));
    this->kafeiPlayer.linearVelocity *= 1.0f + (1.05f * fabsf(Math_SinS(this->kafeiPlayer.unk_B6C)));

    sKafeiInputStickMagnitude = (this->kafeiPlayer.linearVelocity * 10.0f) + 20.0f;
    sKafeiInputStickMagnitude = CLAMP_MAX(sKafeiInputStickMagnitude, 60.0f);
    sKafeiInputStickYaw = this->kafeiPlayer.actor.world.rot.y;

    this->kafeiPlayer.actor.world.pos.x = this->kafeiPlayer.actor.prevPos.x;
    this->kafeiPlayer.actor.world.pos.z = this->kafeiPlayer.actor.prevPos.z;

    if (!func_80A3F384(this, play)) {
        D_80A41D48 = true;
    }

    return ret;
}

void EnTest3_FollowSchedule(EnTest3* this, PlayState* play) {
    struct_80A41828* sp3C;
    ScheduleOutput scheduleOutput;

    this->timePathTimeSpeed =
        ((this->scheduleResult == KAFEI_SCH_DAY3_IKANA_DOOR_OPEN) || (this->scheduleResult == KAFEI_SCH_BELL) ||
         (this->scheduleResult == KAFEI_SCH_DAY3_IKANA_GOING_TO_ANJU))
            ? 3
        : Play_InCsMode(play) ? 0
                              : R_TIME_SPEED + ((void)0, gSaveContext.save.timeSpeedOffset);

    if (Schedule_RunScript(play, sScheduleScript, &scheduleOutput)) {
        if (this->scheduleResult != scheduleOutput.result) {
            sp3C = &D_80A41828[scheduleOutput.result];
            func_80A3F114(this, play);
            if (sp3C->scheduleType != KAFEI_SCH_TYPE_RESPOND_TO_BELL) {
                CLEAR_WEEKEVENTREG(WEEKEVENTREG_HIT_KAFEI_BELL);
            }
            if (!sKafeiScheduleType[sp3C->scheduleType].unk_0(this, play, sp3C, &scheduleOutput)) {
                return;
            }

            if (scheduleOutput.result == KAFEI_SCH_DAY3_INN_NO_ANJU_FINAL) {
                this->kafeiPlayer.actor.home.rot.y = 0x7FFF;
                this->kafeiPlayer.stateFlags2 |= PLAYER_STATE2_40000; // Kafei-only
                play->startPlayerCutscene(play, &this->kafeiPlayer, -1);
            }
        }
    } else {
        scheduleOutput.result = KAFEI_SCH_NONE;
    }
    this->scheduleResult = scheduleOutput.result;
    sp3C = &D_80A41828[this->scheduleResult];
    sKafeiScheduleType[sp3C->scheduleType].unk_4(this, play);
}

void func_80A40824(EnTest3* this, PlayState* play) {
    this->unk_D78 = &D_80A41854[0];
    func_80A3F73C(this, play);
}

void func_80A4084C(EnTest3* this, PlayState* play) {
    if (Actor_TextboxIsClosing(&this->kafeiPlayer.actor, play)) {
        if ((this->unk_D78->unk_1 == 0) || !func_80A3E80C(this, play, this->unk_D78->unk_1 - 1)) {
            if (KAFEI_GET_PARAM_1E0(&this->kafeiPlayer.actor) == 0) {
                EnTest3_SetupAction(this, func_80A40824);
            } else {
                EnTest3_SetupAction(this, EnTest3_FollowSchedule);
            }
            this->kafeiPlayer.targetedActor = NULL;
        }
    } else if (func_80A3ED24(this, play)) {
        EnTest3_SetupAction(this, func_80A40908);
    }
}

void func_80A40908(EnTest3* this, PlayState* play) {
    if (Actor_ProcessTalkRequest(&this->kafeiPlayer.actor, &play->state)) {
        EnTest3_SetupAction(this, func_80A4084C);
        this->kafeiPlayer.targetedActor = &GET_PLAYER(play)->actor;
        SET_WEEKEVENTREG(WEEKEVENTREG_OBTAINED_PENDANT);
        func_80151BB4(play, 0x19);
        func_80151BB4(play, 2);
    } else {
        func_800B8500(&this->kafeiPlayer.actor, play, 9999.9f, 9999.9f, PLAYER_IA_MINUS1);
        this->unk_D78 = &D_80A41854[6];
        this->kafeiPlayer.actor.textId = this->unk_D78->textId;
        this->kafeiPlayer.actor.flags |= (ACTOR_FLAG_1 | ACTOR_FLAG_8);
    }
}

void EnTest3_GetControllerInput(EnTest3* this, PlayState* play) {
    if ((play->actorCtx.flags & ACTORCTX_FLAG_SUNMASK_FAIL) ||
        (play->actorCtx.flags & ACTORCTX_FLAG_TOGGLE_KAFEI_CONTROL)) {
        play->actorCtx.flags &= ~ACTORCTX_FLAG_TOGGLE_KAFEI_CONTROL;
        EnTest3_ReturnInputToGame(this, play);
        ActorCutscene_SetReturnCamera(CAM_ID_MAIN);
    } else {
        sKafeiInput = *CONTROLLER1(&play->state);
    }
}

void func_80A40A6C(EnTest3* this, PlayState* play) {
    SET_WEEKEVENTREG(WEEKEVENTREG_64_20);
}

void EnTest3_Update(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EnTest3* this = THIS;

    sKafeiInput.rel.button = sKafeiInput.cur.button;
    sKafeiInput.cur.button = 0;
    sKafeiInput.rel.stick_x = 0;
    sKafeiInput.rel.stick_y = 0;

    play->actorCtx.flags &= ~ACTORCTX_FLAG_KAFEI_ON_SWITCH;
    this->kafeiPlayer.actor.draw = EnTest3_Draw;
    D_80A41D48 = false;
    this->kafeiPlayer.actor.flags &= ~(ACTOR_FLAG_1 | ACTOR_FLAG_8);

    if (Cutscene_CheckActorAction(play, 0x1FA) && !((this->kafeiPlayer.actor.category == ACTORCAT_PLAYER) &&
                                                    ((play->actorCtx.flags & ACTORCTX_FLAG_SUNMASK_FAIL) ||
                                                     (play->actorCtx.flags & ACTORCTX_FLAG_TOGGLE_KAFEI_CONTROL)))) {
        if (this->kafeiPlayer.csMode != 5) {
            play->startPlayerCutscene(play, &this->kafeiPlayer, 5);
        }
        play->actorCtx.flags &= ~ACTORCTX_FLAG_TOGGLE_KAFEI_CONTROL;
    } else if (this->kafeiPlayer.actor.category == ACTORCAT_PLAYER) {
        // Manual control
        EnTest3_GetControllerInput(this, play);
    } else if (play->startPlayerCutscene(play, &this->kafeiPlayer, 0)) {
        if (this->scheduleResult >= KAFEI_SCH_DAY3_SAKON_HIDEOUT) {
            Vec3f worldPos;

            Math_Vec3f_Copy(&worldPos, &this->kafeiPlayer.actor.world.pos);
            this->timePathTimeSpeed = 4;
            EnTest3_FollowTimePath(this, play);
            Math_Vec3f_Copy(&this->kafeiPlayer.actor.world.pos, &worldPos);
            D_80A41D48 = false;
            this->unk_D84 = 0.0f;
        }
    } else {
        // Game control
        // Set default input (may be recalculated in actionFunc)
        sKafeiInputStickMagnitude = 0.0f;
        sKafeiInputStickYaw = this->kafeiPlayer.actor.shape.rot.y;

        // Update game control
        this->actionFunc(this, play);

        // Apply input
        sKafeiInput.press.button = (sKafeiInput.rel.button ^ sKafeiInput.cur.button) & sKafeiInput.cur.button;
        func_800B6F20(play, &sKafeiInput, sKafeiInputStickMagnitude, sKafeiInputStickYaw);
    }

    play->playerUpdate(&this->kafeiPlayer, play, &sKafeiInput);

    if (D_80A41D48) {
        this->kafeiPlayer.actor.world.pos.x = D_80A41D50.x;
        this->kafeiPlayer.actor.world.pos.z = D_80A41D50.z;
        this->kafeiPlayer.linearVelocity = 0.0f;
    }
}

s32 D_80A418C8 = false;

s32 EnTest3_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    EnTest3* this = THIS;

    if (limbIndex == OBJECT_TEST3_LIMB_01) {
        D_80A41D6C = &this->kafeiPlayer.bodyPartsPos[-1];
        if (!(this->kafeiPlayer.skelAnime.moveFlags & ANIM_FLAG_4) ||
            (this->kafeiPlayer.skelAnime.moveFlags & ANIM_FLAG_1)) {
            pos->x *= this->kafeiPlayer.ageProperties->unk_08;
            pos->z *= this->kafeiPlayer.ageProperties->unk_08;
        }
        if (!(this->kafeiPlayer.skelAnime.moveFlags & ANIM_FLAG_4) ||
            (this->kafeiPlayer.skelAnime.moveFlags & ANIM_FLAG_UPDATEY)) {
            pos->y *= this->kafeiPlayer.ageProperties->unk_08;
        }
        pos->y -= this->kafeiPlayer.shapeOffsetY;
        if (this->kafeiPlayer.unk_AAA != 0) {

            Matrix_Translate(pos->x, ((Math_CosS(this->kafeiPlayer.unk_AAA) - 1.0f) * 200.0f) + pos->y, pos->z,
                             MTXMODE_APPLY);
            Matrix_RotateXS(this->kafeiPlayer.unk_AAA, MTXMODE_APPLY);
            Matrix_RotateZYX(rot->x, rot->y, rot->z, MTXMODE_APPLY);
            func_80125318(pos, rot);
        }
    } else {
        if (*dList != NULL) {
            D_80A41D6C++;
        }
        if (D_80A418C8) {
            *dList = NULL;
        }
        if (limbIndex == OBJECT_TEST3_LIMB_0B) {
            rot->x += this->kafeiPlayer.headRot.z;
            rot->y -= this->kafeiPlayer.headRot.y;
            rot->z += this->kafeiPlayer.headRot.x;
        } else if (limbIndex == OBJECT_TEST3_LIMB_0A) {
            s32 requiredScopeTemp;

            if (this->kafeiPlayer.unk_AA8 != 0) {
                Matrix_RotateZS(0x44C, MTXMODE_APPLY);
                Matrix_RotateYS(this->kafeiPlayer.unk_AA8, MTXMODE_APPLY);
            }
            if (this->kafeiPlayer.upperBodyRot.y != 0) {
                Matrix_RotateYS(this->kafeiPlayer.upperBodyRot.y, MTXMODE_APPLY);
            }
            Matrix_RotateXS(this->kafeiPlayer.upperBodyRot.x, MTXMODE_APPLY);
            if (this->kafeiPlayer.upperBodyRot.z != 0) {
                Matrix_RotateZS(this->kafeiPlayer.upperBodyRot.z, MTXMODE_APPLY);
            }
        } else {
            func_80125500(play, &this->kafeiPlayer, limbIndex, pos, rot);
        }
    }
    return false;
}

void EnTest3_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList1, Gfx** dList2, Vec3s* rot, Actor* thisx) {
    s32 pad;
    EnTest3* this = THIS;

    if (*dList2 != NULL) {
        Matrix_MultZero(D_80A41D6C);
    }

    if (limbIndex == OBJECT_TEST3_LIMB_10) {
        MtxF curMtxF;
        Actor* leftHandActor;

        Math_Vec3f_Copy(&this->kafeiPlayer.leftHandWorld.pos, D_80A41D6C);

        if (*dList1 != NULL) {
            func_80128640(play, &this->kafeiPlayer, *dList1);
            if (this->kafeiPlayer.stateFlags3 & PLAYER_STATE3_20000000) {
                // Bremen march
                OPEN_DISPS(play->state.gfxCtx);
                gSPDisplayList(POLY_OPA_DISP++, object_test3_DL_00EDD0);
                CLOSE_DISPS(play->state.gfxCtx);
            }
        }
        leftHandActor = this->kafeiPlayer.heldActor;
        if ((leftHandActor != NULL) && (this->kafeiPlayer.stateFlags1 & PLAYER_STATE1_800)) {
            // Holding actor
            Vec3s curRot;

            Matrix_Get(&curMtxF);
            Matrix_MtxFToYXZRot(&curMtxF, &curRot, false);
            leftHandActor->world.rot.y = this->kafeiPlayer.actor.shape.rot.y + this->kafeiPlayer.leftHandWorld.rot.y;
            leftHandActor->shape.rot.y = leftHandActor->world.rot.y;
        } else {
            Matrix_Get(&this->kafeiPlayer.mf_CC4);
            Matrix_MtxFToYXZRot(&this->kafeiPlayer.mf_CC4, &this->kafeiPlayer.leftHandWorld.rot, false);
            func_80126B8C(play, &this->kafeiPlayer);
        }

    } else if (limbIndex == OBJECT_TEST3_LIMB_13) {
        Actor* leftHandActor = this->kafeiPlayer.heldActor;

        if (leftHandActor != NULL) {
            leftHandActor->world.pos.x =
                (this->kafeiPlayer.bodyPartsPos[15].x + this->kafeiPlayer.leftHandWorld.pos.x) / 2.0f;
            leftHandActor->world.pos.y =
                (this->kafeiPlayer.bodyPartsPos[15].y + this->kafeiPlayer.leftHandWorld.pos.y) / 2.0f;
            leftHandActor->world.pos.z =
                (this->kafeiPlayer.bodyPartsPos[15].z + this->kafeiPlayer.leftHandWorld.pos.z) / 2.0f;
        }
    } else if (limbIndex == OBJECT_TEST3_LIMB_0B) {
        Actor* targetedActor = this->kafeiPlayer.targetedActor;

        if ((*dList1 != NULL) && ((u32)this->kafeiPlayer.currentMask != PLAYER_MASK_NONE) &&
            !(this->kafeiPlayer.stateFlags2 & PLAYER_STATE2_1000000)) {
            if ((this->kafeiPlayer.skelAnime.animation != &gPlayerAnim_cl_maskoff) &&
                ((this->kafeiPlayer.skelAnime.animation != &gPlayerAnim_cl_setmask) ||
                 (this->kafeiPlayer.skelAnime.curFrame >= 12.0f))) {
                if (func_80127438(play, &this->kafeiPlayer, this->kafeiPlayer.currentMask)) {
                    OPEN_DISPS(play->state.gfxCtx);
                    gSPDisplayList(POLY_OPA_DISP++, object_mask_ki_tan_DL_0004A0);
                    CLOSE_DISPS(play->state.gfxCtx);
                }
            }
        }
        if ((targetedActor != NULL) && (targetedActor->id == ACTOR_BG_IKNV_OBJ)) {
            // Sakon's Door
            Math_Vec3f_Copy(&this->kafeiPlayer.actor.focus.pos, &targetedActor->focus.pos);
        } else {
            static Vec3f D_80A418CC = { 1100.0f, -700.0f, 0.0f };

            Matrix_MultVec3f(&D_80A418CC, &this->kafeiPlayer.actor.focus.pos);
        }
    } else if (limbIndex == OBJECT_TEST3_LIMB_15) {
        if (D_80A41D60 || CHECK_WEEKEVENTREG(WEEKEVENTREG_TALKED_TO_KAFEI_AFTER_LETTER) ||
            (INV_CONTENT(ITEM_PENDANT_OF_MEMORIES) == ITEM_PENDANT_OF_MEMORIES) ||
            ((this->kafeiPlayer.getItemDrawId - 1) == GID_PENDANT_OF_MEMORIES)) {
            D_80A41D60 = true;
        } else {
            // Draw pendant of memories neclace
            OPEN_DISPS(play->state.gfxCtx);
            gSPDisplayList(POLY_OPA_DISP++, object_test3_DL_00CB60);
            CLOSE_DISPS(play->state.gfxCtx);
        }
    } else {
        func_80128B74(play, &this->kafeiPlayer, limbIndex);
    }
}

static TexturePtr sEyeTextures[] = {
    object_test3_Tex_000DC0, object_test3_Tex_003680, object_test3_Tex_003E80, object_test3_Tex_004680,
    object_test3_Tex_004E80, object_test3_Tex_005680, object_test3_Tex_005E80, object_test3_Tex_006680,
};

static TexturePtr sMouthTextures[] = {
    object_test3_Tex_0009C0,
    object_test3_Tex_006E80,
    object_test3_Tex_007280,
    object_test3_Tex_007680,
};

typedef struct {
    /* 0x0 */ u8 eyeIndex;
    /* 0x1 */ u8 mouthIndex;
} KafeiFace; // size = 0x2

static KafeiFace sFaceExpressions[] = {
    { 0, 0 }, { 1, 0 }, { 2, 0 }, { 0, 0 }, { 1, 0 }, { 2, 0 }, { 4, 0 }, { 5, 1 }, { 7, 2 }, { 0, 2 },
    { 3, 0 }, { 4, 0 }, { 2, 2 }, { 1, 1 }, { 0, 2 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
};

void EnTest3_Draw(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EnTest3* this = THIS;
    s32 eyeTexIndex = (this->kafeiPlayer.skelAnime.jointTable[OBJECT_TEST3_LIMB_MAX].x & 0xF) - 1;
    s32 mouthTexIndex = ((this->kafeiPlayer.skelAnime.jointTable[OBJECT_TEST3_LIMB_MAX].x >> 4) & 0xF) - 1;
    Gfx* gfx;

    OPEN_DISPS(play->state.gfxCtx);

    func_8012C268(play);

    if (this->kafeiPlayer.invincibilityTimer > 0) {
        s32 temp2; // Must exist for stack order. Could hold the result of CLAMP instead.

        this->kafeiPlayer.damageFlashTimer += CLAMP(50 - this->kafeiPlayer.invincibilityTimer, 8, 40);
        temp2 = Math_CosS(this->kafeiPlayer.damageFlashTimer * 0x100) * 2000.0f;
        POLY_OPA_DISP = Gfx_SetFog(POLY_OPA_DISP, 255, 0, 0, 0, 0, 4000 - temp2);
    }

    func_800B8050(&this->kafeiPlayer.actor, play, 0);

    D_80A418C8 = false;

    if (this->kafeiPlayer.stateFlags1 & PLAYER_STATE1_100000) {
        // In first-person mode
        Vec3f sp4C;

        SkinMatrix_Vec3fMtxFMultXYZ(&play->viewProjectionMtxF, &this->kafeiPlayer.actor.focus.pos, &sp4C);
        if (sp4C.z < -4.0f) {
            D_80A418C8 = true;
        }
    }

    gfx = POLY_OPA_DISP;

    if (eyeTexIndex < 0) {
        eyeTexIndex = sFaceExpressions[this->kafeiPlayer.actor.shape.face].eyeIndex;
    }
    gSPSegment(&gfx[0], 0x08, Lib_SegmentedToVirtual(sEyeTextures[eyeTexIndex]));

    if (mouthTexIndex < 0) {
        mouthTexIndex = sFaceExpressions[this->kafeiPlayer.actor.shape.face].mouthIndex;
    }
    gSPSegment(&gfx[1], 0x09, Lib_SegmentedToVirtual(sMouthTextures[mouthTexIndex]));

    POLY_OPA_DISP = &gfx[2];

    SkelAnime_DrawFlexLod(play, this->kafeiPlayer.skelAnime.skeleton, this->kafeiPlayer.skelAnime.jointTable,
                          this->kafeiPlayer.skelAnime.dListCount, EnTest3_OverrideLimbDraw, EnTest3_PostLimbDraw,
                          &this->kafeiPlayer.actor, 0);

    if (this->kafeiPlayer.invincibilityTimer > 0) {
        // Play_SetFog
        POLY_OPA_DISP = func_801660B8(play, POLY_OPA_DISP);
    }

    if ((this->kafeiPlayer.getItemDrawId - 1) != GID_NONE) {
        Player_DrawGetItem(play, &this->kafeiPlayer);
    }

    CLOSE_DISPS(play->state.gfxCtx);
}
