#ifndef Z64SAVE_H
#define Z64SAVE_H

#include "ultra64.h"
#include "z64item.h"
#include "z64math.h"
#include "os.h"

struct GameState;
struct PlayState;
struct FileSelectState;

// TODO: properly name DOWN, RETURN and TOP
typedef enum RespawnMode {
    /* 0 */ RESPAWN_MODE_DOWN,                          // "RESTART_MODE_DOWN"
    /* 1 */ RESPAWN_MODE_RETURN,                        // "RESTART_MODE_RETURN"
    /* 2 */ RESPAWN_MODE_TOP,                           // "RESTART_MODE_TOP"
    /* 3 */ RESPAWN_MODE_UNK_3,                         // Related to grottos
    /* 4 */ RESPAWN_MODE_GORON,                         // "RESTART_MODE_GORON"
    /* 5 */ RESPAWN_MODE_ZORA,                          // "RESTART_MODE_ZORA"
    /* 6 */ RESPAWN_MODE_DEKU,                          // "RESTART_MODE_NUTS"
    /* 7 */ RESPAWN_MODE_HUMAN,                         // "RESTART_MODE_CHILD"
    /* 8 */ RESPAWN_MODE_MAX
} RespawnMode;

#define SAVE_BUFFER_SIZE 0x4000

typedef enum {
    /* 0  */ MAGIC_STATE_IDLE, // Regular gameplay
    /* 1  */ MAGIC_STATE_CONSUME_SETUP, // Sets the speed at which the magic border flashes
    /* 2  */ MAGIC_STATE_CONSUME, // Consume magic until target is reached or no more magic is available
    /* 3  */ MAGIC_STATE_METER_FLASH_1, // Flashes border
    /* 4  */ MAGIC_STATE_METER_FLASH_2, // Flashes border and draws yellow magic to preview target consumption
    /* 5  */ MAGIC_STATE_RESET, // Reset colors and return to idle
    /* 6  */ MAGIC_STATE_METER_FLASH_3, // Flashes border with no additional behaviour
    /* 7  */ MAGIC_STATE_CONSUME_LENS, // Magic slowly consumed by Lens of Truth
    /* 8  */ MAGIC_STATE_STEP_CAPACITY, // Step `magicCapacity` to full capacity
    /* 9  */ MAGIC_STATE_FILL, // Add magic until magicFillTarget is reached
    /* 10 */ MAGIC_STATE_CONSUME_GORON_ZORA_SETUP,
    /* 11 */ MAGIC_STATE_CONSUME_GORON_ZORA, // Magic slowly consumed by Goron spiked rolling or Zora electric barrier.
    /* 12 */ MAGIC_STATE_CONSUME_GIANTS_MASK // Magic slowly consumed by Giant's Mask
} MagicState;

typedef enum {
    /* 0 */ MAGIC_CONSUME_NOW, // Consume magic immediately without preview
    /* 1 */ MAGIC_CONSUME_WAIT_NO_PREVIEW, // Sets consume target but waits to consume. No yellow magic preview to target consumption. Unused
    /* 2 */ MAGIC_CONSUME_NOW_ALT, // Identical behaviour to MAGIC_CONSUME_NOW. Unused
    /* 3 */ MAGIC_CONSUME_LENS, // Lens of Truth consumption
    /* 4 */ MAGIC_CONSUME_WAIT_PREVIEW, // Sets consume target but waits to consume. Show magic to be consumed in yellow.
    /* 5 */ MAGIC_CONSUME_GORON_ZORA, // Goron spiked rolling or Zora electric barrier slow consumption
    /* 6 */ MAGIC_CONSUME_GIANTS_MASK, // Giant's Mask slow consumption
    /* 7 */ MAGIC_CONSUME_DEITY_BEAM // Fierce Deity Beam consumption, consumed magic now and not via request
} MagicChangeType;

#define MAGIC_NORMAL_METER 0x30
#define MAGIC_DOUBLE_METER (2 * MAGIC_NORMAL_METER)

// See `R_MAGIC_DBG_SET_UPGRADE`
#define MAGIC_DBG_SET_UPGRADE_NO_ACTION 0
#define MAGIC_DBG_SET_UPGRADE_NORMAL_METER -1
#define MAGIC_DBG_SET_UPGRADE_DOUBLE_METER 1

#define ENV_HAZARD_TEXT_TRIGGER_HOTROOM (1 << 0)
#define ENV_HAZARD_TEXT_TRIGGER_UNDERWATER (1 << 1)

#define SECONDS_TO_TIMER(seconds) ((seconds) * 100)

#define OSTIME_TO_TIMER(osTime) ((osTime) * 64 / 3000 / 10000)
#define OSTIME_TO_TIMER_ALT(osTime) ((osTime) / 10000 * 64 / 3000)

// 1 centisecond = 10 milliseconds = 1/100 seconds
#define SECONDS_TO_TIMER_PRECISE(seconds, centiSeconds) ((seconds) * 100 + (centiSeconds))

typedef enum {
    /*  0 */ TIMER_ID_POSTMAN, // postman's counting minigame
    /*  1 */ TIMER_ID_MINIGAME_1, // minigame timer
    /*  2 */ TIMER_ID_2,
    /*  3 */ TIMER_ID_MOON_CRASH, // timer used for mooncrash on the clocktower roof
    /*  4 */ TIMER_ID_MINIGAME_2, // minigame timer
    /*  5 */ TIMER_ID_ENV_HAZARD, // environmental hazard timer (underwater or hot room)
    /*  6 */ TIMER_ID_GORON_RACE_UNUSED,
    /*  7 */ TIMER_ID_MAX,
    /* 99 */ TIMER_ID_NONE = 99,
} TimerId;

typedef enum {
    /* 0 */ TIMER_COUNT_DOWN,
    /* 1 */ TIMER_COUNT_UP
} TimerDirection;

typedef enum {
    /*  0 */ TIMER_STATE_OFF,
    /*  1 */ TIMER_STATE_START,
    /*  2 */ TIMER_STATE_HOLD_TIMER, // Hold timer frozen at the screen center
    /*  3 */ TIMER_STATE_MOVING_TIMER, // Move timer to a target location
    /*  4 */ TIMER_STATE_COUNTING,
    /*  5 */ TIMER_STATE_STOP,
    /*  6 */ TIMER_STATE_6, // like `TIMER_STATE_STOP` but with extra minigame checks
    /*  7 */ TIMER_STATE_7, // stopped but still update `timerCurTimes`
    /*  8 */ TIMER_STATE_ENV_HAZARD_START,
    /*  9 */ TIMER_STATE_ALT_START,
    /* 10 */ TIMER_STATE_10, // precursor to `TIMER_STATE_ALT_COUNTING`
    /* 11 */ TIMER_STATE_ALT_COUNTING,
    /* 12 */ TIMER_STATE_12, // Updated paused time?
    /* 13 */ TIMER_STATE_POSTMAN_START,
    /* 14 */ TIMER_STATE_POSTMAN_COUNTING,
    /* 15 */ TIMER_STATE_POSTMAN_STOP,
    /* 16 */ TIMER_STATE_POSTMAN_END
} TimerState;

typedef enum {
    /* 0 */ BOTTLE_TIMER_STATE_OFF,
    /* 1 */ BOTTLE_TIMER_STATE_COUNTING
} BottleTimerState;

typedef enum {
    /* 0 */ MINIGAME_STATUS_INACTIVE,
    /* 1 */ MINIGAME_STATUS_ACTIVE,
    /* 3 */ MINIGAME_STATUS_END = 3
} MinigameStatus;

typedef enum {
    /*  0 */ HUD_VISIBILITY_IDLE,
    /*  1 */ HUD_VISIBILITY_NONE,
    /*  2 */ HUD_VISIBILITY_NONE_ALT, // Identical to HUD_VISIBILITY_NONE
    /*  3 */ HUD_VISIBILITY_HEARTS_WITH_OVERWRITE, // Uses Interface_UpdateButtonAlphas so gives the opportunity to dim only disabled buttons
    /*  4 */ HUD_VISIBILITY_A,
    /*  5 */ HUD_VISIBILITY_A_HEARTS_MAGIC_WITH_OVERWRITE, // Uses Interface_UpdateButtonAlphas so gives the opportunity to dim only disabled buttons
    /*  6 */ HUD_VISIBILITY_A_HEARTS_MAGIC_MINIMAP_WITH_OVERWRITE, // Uses Interface_UpdateButtonAlphas so gives the opportunity to dim only disabled buttons
    /*  7 */ HUD_VISIBILITY_ALL_NO_MINIMAP_W_DISABLED, // Only raises button alphas if not disabled
    /*  8 */ HUD_VISIBILITY_B,
    /*  9 */ HUD_VISIBILITY_HEARTS_MAGIC,
    /* 10 */ HUD_VISIBILITY_B_ALT,
    /* 11 */ HUD_VISIBILITY_HEARTS,
    /* 12 */ HUD_VISIBILITY_A_B_MINIMAP,
    /* 13 */ HUD_VISIBILITY_HEARTS_MAGIC_WITH_OVERWRITE, // Uses Interface_UpdateButtonAlphas so gives the opportunity to dim only disabled buttons
    /* 14 */ HUD_VISIBILITY_HEARTS_MAGIC_C,
    /* 15 */ HUD_VISIBILITY_ALL_NO_MINIMAP,
    /* 16 */ HUD_VISIBILITY_A_B_C,
    /* 17 */ HUD_VISIBILITY_B_MINIMAP,
    /* 18 */ HUD_VISIBILITY_HEARTS_MAGIC_MINIMAP,
    /* 19 */ HUD_VISIBILITY_A_HEARTS_MAGIC_MINIMAP,
    /* 20 */ HUD_VISIBILITY_B_MAGIC,
    /* 21 */ HUD_VISIBILITY_A_B,
    /* 22 */ HUD_VISIBILITY_A_B_HEARTS_MAGIC_MINIMAP,
    /* 50 */ HUD_VISIBILITY_ALL = 50,
    /* 52 */ HUD_VISIBILITY_NONE_INSTANT = 52
} HudVisibility;

#define PICTO_PHOTO_WIDTH 160
#define PICTO_PHOTO_HEIGHT 112

#define PICTO_PHOTO_TOPLEFT_X ((SCREEN_WIDTH - PICTO_PHOTO_WIDTH) / 2)
#define PICTO_PHOTO_TOPLEFT_Y ((SCREEN_HEIGHT - PICTO_PHOTO_HEIGHT) / 2)

#define PICTO_PHOTO_SIZE (PICTO_PHOTO_WIDTH * PICTO_PHOTO_HEIGHT)
#define PICTO_PHOTO_COMPRESSED_SIZE (PICTO_PHOTO_SIZE * 5 / 8)

#define BIT_WEEKEVENTREG_FLAG(flag) (((flag / 8) << 8) | (1 << (flag % 8)))

#define DEFINE_WEEKEVENTREG(name, _isPersistent) name##_ENUM,
#define DEFINE_WEEKEVENTREG_8(name, _isPersistent) name##_ENUM,

typedef enum WeekEventRegEnum {
    #include "tables/weekeventregs_table.h"
} WeekEventRegEnum;

#undef DEFINE_WEEKEVENTREG
#undef DEFINE_WEEKEVENTREG_8


#define DEFINE_WEEKEVENTREG(name, _isPersistent) name = BIT_WEEKEVENTREG_FLAG(name##_ENUM),
#define DEFINE_WEEKEVENTREG_8(name, _isPersistent) name = BIT_WEEKEVENTREG_FLAG(name##_ENUM),

typedef enum WeekEventRegFlag {
    #include "tables/weekeventregs_table.h"
    /* 0x6381 */ WEEKEVENTREG_MAX
} WeekEventRegFlag;

#undef DEFINE_WEEKEVENTREG
#undef DEFINE_WEEKEVENTREG_8

typedef struct SramContext {
    /* 0x00 */ u8* readBuff;
    /* 0x04 */ u8 *saveBuf;
    /* 0x08 */ char unk_08[4];
    /* 0x0C */ s16 status;
    /* 0x10 */ u32 curPage;
    /* 0x14 */ u32 numPages;
    /* 0x18 */ OSTime unk_18;
    /* 0x20 */ s16 unk_20;
    /* 0x22 */ s16 unk_22;
    /* 0x24 */ s16 unk_24;
} SramContext; // size = 0x28

typedef struct ItemEquips {
    /* 0x00 */ u8 buttonItems[4][4];                    // "register_item"
    /* 0x10 */ u8 cButtonSlots[4][4];                   // "register_item_pt"
    /* 0x20 */ u16 equipment;
} ItemEquips; // size = 0x22

typedef struct Inventory {
    /* 0x00 */ u8 items[48];                            // "item_register", first 24 elements are normal items and the other 24 are masks
    /* 0x30 */ s8 ammo[24];                             // "item_count"
    /* 0x48 */ u32 upgrades;                            // "non_equip_register" some bits are wallet upgrades
    /* 0x4C */ u32 questItems;                          // "collect_register"
    /* 0x50 */ u8 dungeonItems[10];                     // "key_compass_map"
    /* 0x5A */ s8 dungeonKeys[9];                       // "key_register"
    /* 0x63 */ s8 defenseHearts;
    /* 0x64 */ s8 strayFairies[10];                     // "orange_fairy"
    /* 0x6E */ char dekuPlaygroundPlayerName[3][8];     // "degnuts_memory_name" Stores playerName (8 char) over (3 days) when getting a new high score
} Inventory; // size = 0x88

typedef struct HorseData {
    /* 0x0 */ s16 sceneId;                             // "spot_no"
    /* 0x2 */ Vec3s pos;                               // "horse_x", "horse_y" and "horse_z"
    /* 0x8 */ s16 yaw;                                 // "horse_a"
} HorseData; // size = 0xA

typedef struct RespawnData {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ s16 yaw;
    /* 0x0E */ s16 playerParams;
    /* 0x10 */ u16 entrance;
    /* 0x12 */ u8 roomIndex;
    /* 0x13 */ s8 data;
    /* 0x14 */ u32 tempSwitchFlags;
    /* 0x18 */ u32 unk_18;
    /* 0x1C */ u32 tempCollectFlags;
} RespawnData; // size = 0x20

typedef struct PermanentSceneFlags {
    /* 0x00 */ u32 chest;
    /* 0x04 */ u32 switch0;
    /* 0x08 */ u32 switch1;
    /* 0x0C */ u32 clearedRoom;
    /* 0x10 */ u32 collectible;
    /* 0x14 */ u32 unk_14; // varies based on scene. For dungeons, floors visited. 
    /* 0x18 */ u32 rooms;
} PermanentSceneFlags; // size = 0x1C

typedef struct CycleSceneFlags {
    /* 0x00 */ u32 chest;
    /* 0x04 */ u32 switch0;
    /* 0x08 */ u32 switch1;
    /* 0x0C */ u32 clearedRoom;
    /* 0x10 */ u32 collectible;
} CycleSceneFlags; // size = 0x14

typedef struct SaveOptions {
    /* 0x0 */ u16 optionId;                            // "option_id"
    /* 0x2 */ u8 language;                             // "j_n"
    /* 0x3 */ s8 audioSetting;                         // "s_sound"
    /* 0x4 */ u8 languageSetting;                      // "language"
    /* 0x5 */ u8 zTargetSetting;                       // "z_attention", 0: Switch; 1: Hold
} SaveOptions; // size = 0x6

typedef struct SavePlayerData {
    /* 0x00 */ char newf[6];                          // "newf"               Will always be "ZELDA3 for a valid save
    /* 0x06 */ u16 deaths;                            // "savect"
    /* 0x08 */ char playerName[8];                    // "player_name"
    /* 0x10 */ s16 healthCapacity;                    // "max_life"
    /* 0x12 */ s16 health;                            // "now_life"
    /* 0x14 */ s8 magicLevel; // 0 for no magic/new load, 1 for magic, 2 for double magic "magic_max"
    /* 0x15 */ s8 magic; // current magic available for use "magic_now"
    /* 0x16 */ s16 rupees;                            // "lupy_count"
    /* 0x18 */ u16 swordHealth;                       // "long_sword_hp"
    /* 0x1A */ u16 tatlTimer;                         // "navi_timer"
    /* 0x1C */ u8 isMagicAcquired;                    // "magic_mode"
    /* 0x1D */ u8 isDoubleMagicAcquired;              // "magic_ability"
    /* 0x1E */ u8 doubleDefense;                      // "life_ability"
    /* 0x1F */ u8 unk_1F;                             // "ocarina_round"
    /* 0x20 */ u8 unk_20;                             // "first_memory"
    /* 0x22 */ u16 owlActivationFlags;                // "memory_warp_point"
    /* 0x24 */ u8 unk_24;                             // "last_warp_pt"
    /* 0x26 */ s16 savedSceneId;                      // "scene_data_ID"
} SavePlayerData; // size = 0x28

typedef struct SaveInfo {
    /* 0x000 */ SavePlayerData playerData;
    /* 0x028 */ ItemEquips equips;
    /* 0x04C */ Inventory inventory;
    /* 0x0D4 */ PermanentSceneFlags permanentSceneFlags[120];
    /* 0xDF4 */ UNK_TYPE1 unk_DF4[0x54];
    /* 0xE48 */ u32 dekuPlaygroundHighScores[3];
    /* 0xE54 */ u32 pictoFlags0;                       // Flags set by `PictoActor`s if pictograph is valid
    /* 0xE58 */ u32 pictoFlags1;                       // Flags set by Snap_ValidatePictograph() to record errors; volatile since that function is run many times in succession
    /* 0xE5C */ u32 unk_E5C;
    /* 0xE60 */ u32 unk_E60;
    /* 0xE64 */ u32 unk_E64[7];                        // Invadepoh flags
    /* 0xE80 */ u32 scenesVisible[7];                  // tingle maps and clouded regions on pause map. Stores scenes bitwise for up to 224 scenes even though there are not that many scenes
    /* 0xE9C */ u32 skullTokenCount;                   // upper 16 bits store Swamp skulls, lower 16 bits store Ocean skulls
    /* 0xEA0 */ u32 unk_EA0;                           // Gossic stone heart piece flags
    /* 0xEA4 */ u32 unk_EA4;
    /* 0xEA8 */ u32 unk_EA8[2];                        // Related to blue warps
    /* 0xEB0 */ u32 stolenItems;                       // Items stolen by Takkuri and given to Curiosity Shop Man
    /* 0xEB4 */ u32 unk_EB4;
    /* 0xEB8 */ u32 bankRupees;
    /* 0xEBC */ u32 unk_EBC;
    /* 0xEC0 */ u32 unk_EC0;                           // Fishing flags
    /* 0xEC4 */ u32 unk_EC4;
    /* 0xEC8 */ u32 horseBackBalloonHighScore;
    /* 0xECC */ u32 lotteryCodeGuess;                  // Lottery code chosen by player (only uses lower three hex digits)
    /* 0xED0 */ u32 shootingGalleryHighScores;         // High scores for both shooting galleries. Town uses lower 16 bits, Swamp uses higher 16 bits.
    /* 0xED4 */ u8 weekEventReg[(WEEKEVENTREG_MAX >> 8) + 1]; // "week_event_reg"
    /* 0xF38 */ u32 regionsVisited;                    // "area_arrival"
    /* 0xF3C */ u32 worldMapCloudVisibility;           // "cloud_clear"
    /* 0xF40 */ u8 unk_F40;                            // "oca_rec_flag"                   has scarecrows song
    /* 0xF41 */ u8 unk_F41;                            // "oca_rec_flag8"                  scarecrows song set?
    /* 0xF42 */ u8 scarecrowSpawnSong[128];
    /* 0xFC2 */ s8 bombersCaughtNum;                   // "aikotoba_index"
    /* 0xFC3 */ s8 bombersCaughtOrder[5];              // "aikotoba_table"
    /* 0xFC8 */ s8 lotteryCodes[3][3];                 // "numbers_table", Preset lottery codes
    /* 0xFD1 */ s8 spiderHouseMaskOrder[6];            // "kinsta_color_table"
    /* 0xFD7 */ s8 bomberCode[5];                      // "bombers_aikotoba_table"
    /* 0xFDC */ HorseData horseData;
    /* 0xFE6 */ u16 checksum;                          // "check_sum"
} SaveInfo; // size = 0xFE8

typedef struct Save {
    /* 0x00 */ s32 entrance;                            // "scene_no"
    /* 0x04 */ u8 equippedMask;                         // "player_mask"
    /* 0x05 */ u8 isFirstCycle;                         // "opening_flag"
    /* 0x06 */ u8 unk_06;
    /* 0x07 */ u8 linkAge;                              // "link_age"
    /* 0x08 */ s32 cutsceneIndex;                       // "day_time"
    /* 0x0C */ u16 time;                                // "zelda_time"
    /* 0x0E */ u16 owlSaveLocation;
    /* 0x10 */ s32 isNight;                             // "asahiru_fg"
    /* 0x14 */ s32 timeSpeedOffset;                     // "change_zelda_time"
    /* 0x18 */ s32 day;                                 // "totalday"
    /* 0x1C */ s32 daysElapsed;                         // "eventday"
    /* 0x20 */ u8 playerForm;                           // "player_character"
    /* 0x21 */ u8 snowheadCleared;                      // "spring_flag"
    /* 0x22 */ u8 hasTatl;                              // "bell_flag"
    /* 0x23 */ u8 isOwlSave;
    /* 0x24 */ SaveInfo saveInfo;
} Save; // size = 0x100C

typedef struct SaveContext {
    /* 0x0000 */ Save save;
    /* 0x100C */ u8 eventInf[8];                        // "event_inf"
    /* 0x1014 */ u8 unk_1014;                           // "stone_set_flag"
    /* 0x1015 */ u8 bButtonStatus;
    /* 0x1016 */ u16 jinxTimer;
    /* 0x1018 */ s16 rupeeAccumulator;                  // "lupy_udct"
    /* 0x101A */ u8 bottleTimerStates[BOTTLE_MAX]; // See the `BottleTimerState` enum. "bottle_status"
    /* 0x1020 */ OSTime bottleTimerStartOsTimes[BOTTLE_MAX]; // The osTime when the timer starts. "bottle_ostime"
    /* 0x1050 */ u64 bottleTimerTimeLimits[BOTTLE_MAX]; // The original total time given before the timer expires, in centiseconds (1/100th sec). "bottle_sub"
    /* 0x1080 */ u64 bottleTimerCurTimes[BOTTLE_MAX]; // The remaining time left before the timer expires, in centiseconds (1/100th sec). "bottle_time"
    /* 0x10B0 */ OSTime bottleTimerPausedOsTimes[BOTTLE_MAX]; // The cumulative osTime spent with the timer paused. "bottle_stop_time"
    /* 0x10E0 */ u8 pictoPhotoI5[PICTO_PHOTO_COMPRESSED_SIZE]; // buffer containing the pictograph photo, compressed to I5 from I8
    /* 0x3CA0 */ s32 fileNum;                           // "file_no"
    /* 0x3CA4 */ s16 powderKegTimer;                    // "big_bom_timer"
    /* 0x3CA6 */ u8 unk_3CA6;
    /* 0x3CA7 */ u8 unk_3CA7;                           // "day_night_flag"
    /* 0x3CA8 */ s32 gameMode;                          // "mode"
    /* 0x3CAC */ s32 sceneLayer;                        // "counter"
    /* 0x3CB0 */ s32 respawnFlag;                       // "restart_flag"
    /* 0x3CB4 */ RespawnData respawn[RESPAWN_MODE_MAX]; // "restart_data"
    /* 0x3DB4 */ f32 entranceSpeed;                     // "player_wipe_speedF"
    /* 0x3DB8 */ u16 entranceSound;                     // "player_wipe_door_SE"
    /* 0x3DBA */ u8 unk_3DBA;                           // "player_wipe_item"
    /* 0x3DBB */ u8 unk_3DBB;                           // "next_walk"
    /* 0x3DBC */ s16 dogParams;                         // OoT leftover. "dog_flag"
    /* 0x3DBE */ u8 envHazardTextTriggerFlags;          // "guide_status"
    /* 0x3DBF */ u8 showTitleCard;                      // "name_display"
    /* 0x3DC0 */ s16 unk_3DC0;                          // "shield_magic_timer"
    /* 0x3DC2 */ u8 unk_3DC2;                           // "pad1"
    /* 0x3DC8 */ OSTime postmanTimerStopOsTime; // The osTime when the timer stops for the postman minigame. "get_time"
    /* 0x3DD0 */ u8 timerStates[TIMER_ID_MAX]; // See the `TimerState` enum. "event_fg"
    /* 0x3DD7 */ u8 timerDirections[TIMER_ID_MAX]; // See the `TimerDirection` enum. "calc_flag"
    /* 0x3DE0 */ u64 timerCurTimes[TIMER_ID_MAX]; // For countdown, the remaining time left. For countup, the time since the start. In centiseconds (1/100th sec). "event_ostime"
    /* 0x3E18 */ u64 timerTimeLimits[TIMER_ID_MAX]; // The original total time given for the timer to count from, in centiseconds (1/100th sec). "event_sub"
    /* 0x3E50 */ OSTime timerStartOsTimes[TIMER_ID_MAX]; // The osTime when the timer starts. "func_time"
    /* 0x3E88 */ u64 timerStopTimes[TIMER_ID_MAX];  // The total amount of time taken between the start and end of the timer, in centiseconds (1/100th sec). "func_end_time"
    /* 0x3EC0 */ OSTime timerPausedOsTimes[TIMER_ID_MAX]; // The cumulative osTime spent with the timer paused. "func_stop_time"
    /* 0x3EF8 */ s16 timerX[TIMER_ID_MAX];              // "event_xp"
    /* 0x3F06 */ s16 timerY[TIMER_ID_MAX];              // "event_yp"
    /* 0x3F14 */ s16 unk_3F14;                          // "character_change"
    /* 0x3F16 */ u8 seqId;                              // "old_bgm"
    /* 0x3F17 */ u8 ambienceId;                         // "old_env"
    /* 0x3F18 */ u8 buttonStatus[6];                    // "button_item"
    /* 0x3F1E */ u8 hudVisibilityForceButtonAlphasByStatus; // if btn alphas are updated through Interface_UpdateButtonAlphas, instead update them through Interface_UpdateButtonAlphasByStatus "ck_fg"
    /* 0x3F20 */ u16 nextHudVisibility; // triggers the hud to change visibility to the requested value. Reset to HUD_VISIBILITY_IDLE when target is reached "alpha_type"
    /* 0x3F22 */ u16 hudVisibility; // current hud visibility "prev_alpha_type"
    /* 0x3F24 */ u16 hudVisibilityTimer; // number of frames in the transition to a new hud visibility. Used to step alpha "alpha_count"
    /* 0x3F26 */ u16 prevHudVisibility; // used to store and recover hud visibility for pause menu and text boxes "last_time_type"
    /* 0x3F28 */ s16 magicState; // determines magic meter behavior on each frame "magic_flag"
    /* 0x3F2A */ s16 isMagicRequested; // a request to add magic has been given "recovery_magic_flag"
    /* 0x3F2C */ s16 magicFlag; // Set to 0 in func_80812D94(), otherwise unused "keep_magic_flag"
    /* 0x3F2E */ s16 magicCapacity; // maximum magic available "magic_now_max"
    /* 0x3F30 */ s16 magicFillTarget; // target used to fill magic "magic_now_now"
    /* 0x3F32 */ s16 magicToConsume; // accumulated magic that is requested to be consumed "magic_used"
    /* 0x3F34 */ s16 magicToAdd; // accumulated magic that is requested to be added "magic_recovery"
    /* 0x3F36 */ u16 mapIndex;                          // "scene_ID"
    /* 0x3F38 */ u16 minigameStatus;                    // "yabusame_mode"
    /* 0x3F3A */ u16 minigameScore;                     // "yabusame_total"
    /* 0x3F3C */ u16 minigameHiddenScore;               // "yabusame_out_ct"
    /* 0x3F3E */ u8 unk_3F3E;                           // "no_save"
    /* 0x3F3F */ u8 unk_3F3F;                           // "flash_flag"
    /* 0x3F40 */ SaveOptions options;
    /* 0x3F46 */ u16 unk_3F46;                          // "NottoriBgm"
    /* 0x3F48 */ u8 cutsceneTransitionControl;          // "fade_go"
    /* 0x3F4A */ u16 nextCutsceneIndex;                 // "next_daytime"
    /* 0x3F4C */ u8 cutsceneTrigger;                    // "doukidemo"
    /* 0x3F4D */ u8 unk_3F4D;                           // "Kenjya_no"
    /* 0x3F4E */ u16 nextDayTime;                       // "next_zelda_time"
    /* 0x3F50 */ u8 transFadeDuration;                  // "fade_speed"
    /* 0x3F51 */ u8 transWipeSpeed;                     // "wipe_speed"           transition related
    /* 0x3F52 */ u16 skyboxTime;                        // "kankyo_time"
    /* 0x3F54 */ u8 dogIsLost;                          // "dog_event_flag"
    /* 0x3F55 */ u8 nextTransitionType;                 // "next_wipe"
    /* 0x3F56 */ s16 worldMapArea;                      // "area_type"
    /* 0x3F58 */ s16 sunsSongState;                     // "sunmoon_flag"
    /* 0x3F5A */ s16 healthAccumulator;                 // "life_mode"
    /* 0x3F5C */ s32 unk_3F5C;                          // "bet_rupees"
    /* 0x3F60 */ u8 screenScaleFlag;                    // "framescale_flag"
    /* 0x3F64 */ f32 screenScale;                       // "framescale_scale"
    /* 0x3F68 */ CycleSceneFlags cycleSceneFlags[120];  // Scene flags that are temporarily stored over the duration of a single 3-day cycle
    /* 0x48C8 */ u16 dungeonIndex;                      // "scene_id_mix"
    /* 0x48CA */ u8 masksGivenOnMoon[27];               // bit-packed, masks given away on the Moon. "mask_mask_bit"
} SaveContext; // size = 0x48C8

typedef enum ButtonStatus {
    /* 0x00 */ BTN_ENABLED,
    /* 0xFF */ BTN_DISABLED = 0xFF
} ButtonStatus;

typedef enum SunsSongState {
    /* 0 */ SUNSSONG_INACTIVE,
    /* 1 */ SUNSSONG_START, // the suns ocarina effect signals that the song has finished playing
    /* 2 */ SUNSSONG_SPEED_TIME, // suns was played where time passes, speed up the advancement of time
    /* 3 */ SUNSSONG_SPECIAL // time does not advance, but signals the song was played. used for freezing redeads
} SunsSongState;

typedef enum {
    /* 0 */ GAMEMODE_NORMAL,
    /* 1 */ GAMEMODE_TITLE_SCREEN,
    /* 2 */ GAMEMODE_FILE_SELECT,
    /* 3 */ GAMEMODE_END_CREDITS,
    /* 4 */ GAMEMODE_OWL_SAVE
} GameMode;

// linkAge still exists in MM, but is always set to 0 (always adult)
// There are remnants of these macros from OOT, but they are essentially useless
#define LINK_IS_CHILD (gSaveContext.save.linkAge == 1)
#define LINK_IS_ADULT (gSaveContext.save.linkAge == 0)

#define CURRENT_DAY (((void)0, gSaveContext.save.day) % 5)

// The day begins at CLOCK_TIME(6, 0) so it must be offset.
#define TIME_UNTIL_MOON_CRASH \
    ((4 - CURRENT_DAY) * DAY_LENGTH - (u16)(((void)0, gSaveContext.save.time) - CLOCK_TIME(6, 0)));
#define TIME_UNTIL_NEW_DAY (DAY_LENGTH - (u16)(((void)0, gSaveContext.save.time) - CLOCK_TIME(6, 0)));

#define GET_PLAYER_FORM ((void)0, gSaveContext.save.playerForm)

#define SLOT(item) gItemSlots[item]
#define AMMO(item) gSaveContext.save.saveInfo.inventory.ammo[SLOT(item)]
#define INV_CONTENT(item) gSaveContext.save.saveInfo.inventory.items[SLOT(item)]
#define GET_INV_CONTENT(item) ((void)0, gSaveContext.save.saveInfo.inventory.items)[SLOT(item)]

#define CUR_FORM ((gSaveContext.save.playerForm == PLAYER_FORM_HUMAN) ? 0 : gSaveContext.save.playerForm)

#define GET_SAVE_EQUIPS_EQUIPMENT ((void)0, gSaveContext.save.saveInfo.equips.equipment)
#define GET_SAVE_INVENTORY_UPGRADES ((void)0, gSaveContext.save.saveInfo.inventory.upgrades)
#define GET_SAVE_INVENTORY_QUEST_ITEMS ((void)0, gSaveContext.save.saveInfo.inventory.questItems)

#define GET_CUR_EQUIP_VALUE(equip) ((GET_SAVE_EQUIPS_EQUIPMENT & gEquipMasks[equip]) >> gEquipShifts[equip])

#define CUR_UPG_VALUE(upg) ((gSaveContext.save.saveInfo.inventory.upgrades & gUpgradeMasks[upg]) >> gUpgradeShifts[upg])
#define GET_CUR_UPG_VALUE(upg) ((GET_SAVE_INVENTORY_UPGRADES & gUpgradeMasks[upg]) >> gUpgradeShifts[upg])

#define SET_EQUIP_VALUE(equip, value) (gSaveContext.save.saveInfo.equips.equipment = ((GET_SAVE_EQUIPS_EQUIPMENT & gEquipNegMasks[equip]) | (u16)((u16)(value) << gEquipShifts[equip])))

#define BUTTON_ITEM_EQUIP(form, button) (gSaveContext.save.saveInfo.equips.buttonItems[form][button])
#define CUR_FORM_EQUIP(button) BUTTON_ITEM_EQUIP(CUR_FORM, button)

#define C_SLOT_EQUIP(form, button) (gSaveContext.save.saveInfo.equips.cButtonSlots[form][button])
#define CHECK_QUEST_ITEM(item) (GET_SAVE_INVENTORY_QUEST_ITEMS & gBitFlags[item])
#define SET_QUEST_ITEM(item) (gSaveContext.save.saveInfo.inventory.questItems = (GET_SAVE_INVENTORY_QUEST_ITEMS | gBitFlags[item]))
#define REMOVE_QUEST_ITEM(item) (gSaveContext.save.saveInfo.inventory.questItems = (GET_SAVE_INVENTORY_QUEST_ITEMS & (-1 - gBitFlags[item])))

#define GET_QUEST_HEART_PIECE_COUNT ((GET_SAVE_INVENTORY_QUEST_ITEMS & 0xF0000000) >> QUEST_HEART_PIECE_COUNT)
#define EQ_MAX_QUEST_HEART_PIECE_COUNT ((GET_SAVE_INVENTORY_QUEST_ITEMS & 0xF0000000) == (4 << QUEST_HEART_PIECE_COUNT))
#define LEQ_MAX_QUEST_HEART_PIECE_COUNT ((GET_SAVE_INVENTORY_QUEST_ITEMS & 0xF0000000) <= (4 << QUEST_HEART_PIECE_COUNT))
#define INCREMENT_QUEST_HEART_PIECE_COUNT (gSaveContext.save.saveInfo.inventory.questItems += (1 << QUEST_HEART_PIECE_COUNT))
#define DECREMENT_QUEST_HEART_PIECE_COUNT (gSaveContext.save.saveInfo.inventory.questItems -= (1 << QUEST_HEART_PIECE_COUNT))
#define RESET_HEART_PIECE_COUNT (gSaveContext.save.saveInfo.inventory.questItems ^= (4 << QUEST_HEART_PIECE_COUNT))

#define CHECK_DUNGEON_ITEM(item, dungeonIndex) (gSaveContext.save.saveInfo.inventory.dungeonItems[(void)0, dungeonIndex] & gBitFlags[item])
#define SET_DUNGEON_ITEM(item, dungeonIndex) (gSaveContext.save.saveInfo.inventory.dungeonItems[(void)0, dungeonIndex] |= (u8)gBitFlags[item])
#define DUNGEON_KEY_COUNT(dungeonIndex) (gSaveContext.save.saveInfo.inventory.dungeonKeys[(void)0, dungeonIndex])

#define GET_CUR_FORM_BTN_ITEM(btn) ((u8)((btn) == EQUIP_SLOT_B ? BUTTON_ITEM_EQUIP(CUR_FORM, btn) : BUTTON_ITEM_EQUIP(0, btn)))
#define GET_CUR_FORM_BTN_SLOT(btn) ((u8)((btn) == EQUIP_SLOT_B ? C_SLOT_EQUIP(CUR_FORM, btn) : C_SLOT_EQUIP(0, btn)))

#define C_BTN_ITEM(btn)                                 \
    ((gSaveContext.buttonStatus[(btn)] != BTN_DISABLED) \
         ? BUTTON_ITEM_EQUIP(0, (btn))                  \
         : ((gSaveContext.hudVisibility == HUD_VISIBILITY_A_B_C) ? BUTTON_ITEM_EQUIP(0, (btn)) : ITEM_NONE))

#define SET_CUR_FORM_BTN_ITEM(btn, item)             \
    if ((btn) == EQUIP_SLOT_B) {                     \
        BUTTON_ITEM_EQUIP(CUR_FORM, (btn)) = (item); \
    } else {                                         \
        BUTTON_ITEM_EQUIP(0, (btn)) = (item);        \
    }                                                \
    (void)0

#define SET_CUR_FORM_BTN_SLOT(btn, item)        \
    if ((btn) == EQUIP_SLOT_B) {                \
        C_SLOT_EQUIP(CUR_FORM, (btn)) = (item); \
    } else {                                    \
        C_SLOT_EQUIP(0, (btn)) = (item);        \
    }                                           \
    (void)0

#define STOLEN_ITEM_NONE (0)

#define STOLEN_ITEM_1 ((gSaveContext.save.saveInfo.stolenItems & 0xFF000000) >> 0x18)
#define STOLEN_ITEM_2 ((gSaveContext.save.saveInfo.stolenItems & 0x00FF0000) >> 0x10)

#define SET_STOLEN_ITEM_1(itemId) \
    (gSaveContext.save.saveInfo.stolenItems = (gSaveContext.save.saveInfo.stolenItems & ~0xFF000000) | ((itemId & 0xFF) << 0x18))
#define SET_STOLEN_ITEM_2(itemId) \
    (gSaveContext.save.saveInfo.stolenItems = (gSaveContext.save.saveInfo.stolenItems & ~0x00FF0000) | ((itemId & 0xFF) << 0x10))

#define GET_TOWN_SHOOTING_GALLERY_HIGH_SCORE() ((s32)(gSaveContext.save.saveInfo.shootingGalleryHighScores & 0xFFFF))
#define GET_SWAMP_SHOOTING_GALLERY_HIGH_SCORE() ((s32)((gSaveContext.save.saveInfo.shootingGalleryHighScores & 0xFFFF0000) >> 0x10))
#define SET_TOWN_SHOOTING_GALLERY_HIGH_SCORE(score) (gSaveContext.save.saveInfo.shootingGalleryHighScores = (gSaveContext.save.saveInfo.shootingGalleryHighScores & 0xFFFF0000) | ((u16)(score)))
#define SET_SWAMP_SHOOTING_GALLERY_HIGH_SCORE(score) (gSaveContext.save.saveInfo.shootingGalleryHighScores = ((gSaveContext.save.saveInfo.shootingGalleryHighScores) & 0xFFFF) | ((u16)(score) << 0x10))

/**
 * gSaveContext.save.saveInfo.weekEventReg
 */

#define PACK_WEEKEVENTREG_FLAG(index, mask) (((index) << 8) | (mask))

#define WEEKEVENTREG(index) (gSaveContext.save.saveInfo.weekEventReg[(index)])
#define GET_WEEKEVENTREG(index) ((void)0, WEEKEVENTREG(index))

#define CHECK_WEEKEVENTREG(flag) (WEEKEVENTREG((flag) >> 8) & ((flag) & 0xFF))
#define CHECK_WEEKEVENTREG_ALT(flag) (GET_WEEKEVENTREG((flag) >> 8) & ((flag) & 0xFF))
#define SET_WEEKEVENTREG(flag) (WEEKEVENTREG((flag) >> 8) = GET_WEEKEVENTREG((flag) >> 8) | ((flag) & 0xFF))
#define CLEAR_WEEKEVENTREG(flag) (WEEKEVENTREG((flag) >> 8) = GET_WEEKEVENTREG((flag) >> 8) & (u8)~((flag) & 0xFF))

typedef enum {
    /* 0 */ WEEKEVENTREG_HORSE_RACE_STATE_END,
    /* 1 */ WEEKEVENTREG_HORSE_RACE_STATE_START,
    /* 2 */ WEEKEVENTREG_HORSE_RACE_STATE_2,
    /* 3 */ WEEKEVENTREG_HORSE_RACE_STATE_3,
    /* 4 */ WEEKEVENTREG_HORSE_RACE_STATE_4,
    /* 7 */ WEEKEVENTREG_HORSE_RACE_STATE_MAX = 7
} WeekEventRegHorseRaceState;

#define GET_WEEKEVENTREG_HORSE_RACE_STATE (WEEKEVENTREG(92) & WEEKEVENTREG_HORSE_RACE_STATE_MAX)

#define SET_WEEKEVENTREG_HORSE_RACE_STATE(state)                                                                       \
    {                                                                                                                  \
        WEEKEVENTREG(92) &= (u8)~WEEKEVENTREG_HORSE_RACE_STATE_MAX;                                                    \
        WEEKEVENTREG(92) = WEEKEVENTREG(92) | (u8)((WEEKEVENTREG(92) & ~WEEKEVENTREG_HORSE_RACE_STATE_MAX) | (state)); \
    }                                                                                                                  \
    (void)0

#define GET_WEEKEVENTREG_DOG_RACE_TEXT(index, baseTextId)                         \
    (index % 2) ? (baseTextId + (((WEEKEVENTREG(42 + (index / 2))) & 0xF0) >> 4)) \
                : (baseTextId + ((WEEKEVENTREG(42 + (index / 2))) & 0x0F));

#define SET_WEEKEVENTREG_DOG_RACE_TEXT_OFFSETS(index, packedOffsets)                    \
    {                                                                                   \
        WEEKEVENTREG(42 + index) = ((void)0, WEEKEVENTREG(42 + index)) | packedOffsets; \
    }

#define CLEAR_WEEKEVENTREG_DOG_RACE_TEXT_OFFSETS(index) \
    {                                                   \
        WEEKEVENTREG(42 + index) = 0;                   \
    }

/**
 * gSaveContext.eventInf
 */

// gSaveContext.eventInf[0] is used to dog race information
// #define EVENTINF_00 0x00
// #define EVENTINF_01 0x01
// #define EVENTINF_02 0x02
// #define EVENTINF_03 0x03
// #define EVENTINF_04 0x04
// #define EVENTINF_05 0x05
// #define EVENTINF_06 0x06
// #define EVENTINF_07 0x07

#define EVENTINF_10 0x10
#define EVENTINF_11 0x11
#define EVENTINF_12 0x12
#define EVENTINF_13 0x13
#define EVENTINF_14 0x14
#define EVENTINF_15 0x15
#define EVENTINF_16 0x16
#define EVENTINF_17 0x17
#define EVENTINF_20 0x20
#define EVENTINF_21 0x21
#define EVENTINF_22 0x22
#define EVENTINF_23 0x23
#define EVENTINF_24 0x24
#define EVENTINF_25 0x25
#define EVENTINF_26 0x26
#define EVENTINF_27 0x27
#define EVENTINF_30 0x30

// EVENTINF_31 is used to track if Player is within range of EnGakufu (2D Song Buttons Appearing on Wall)
#define EVENTINF_31 0x31
// EVENTINF_32 is used to track if Player has played the notes of EnGakufu (2D Song Buttons Appearing on Wall)
#define EVENTINF_32 0x32

#define EVENTINF_33 0x33

// Related to Deku playground minigame
#define EVENTINF_34 0x34
#define EVENTINF_35 0x35
#define EVENTINF_36 0x36
#define EVENTINF_37 0x37
#define EVENTINF_40 0x40

// Related to swamp boat (non-minigame)?
#define EVENTINF_41 0x41
#define EVENTINF_42 0x42
#define EVENTINF_43 0x43
#define EVENTINF_44 0x44
#define EVENTINF_45 0x45
#define EVENTINF_46 0x46
#define EVENTINF_47 0x47
#define EVENTINF_50 0x50
#define EVENTINF_51 0x51
#define EVENTINF_52 0x52
#define EVENTINF_53 0x53
#define EVENTINF_54 0x54
#define EVENTINF_55 0x55

// Enabled when Gyorg's intro cutscene has been watched
#define EVENTINF_56 0x56

#define EVENTINF_57 0x57
#define EVENTINF_60 0x60
#define EVENTINF_61 0x61
#define EVENTINF_62 0x62
#define EVENTINF_63 0x63
#define EVENTINF_64 0x64
#define EVENTINF_65 0x65
#define EVENTINF_66 0x66
#define EVENTINF_67 0x67
#define EVENTINF_70 0x70
#define EVENTINF_71 0x71
#define EVENTINF_72 0x72
#define EVENTINF_73 0x73
#define EVENTINF_74 0x74
#define EVENTINF_75 0x75
#define EVENTINF_76 0x76
#define EVENTINF_77 0x77

#define CHECK_EVENTINF(flag) (gSaveContext.eventInf[(flag) >> 4] & (1 << ((flag) & 0xF)))
#define SET_EVENTINF(flag) (gSaveContext.eventInf[(flag) >> 4] |= (1 << ((flag) & 0xF)))
#define CLEAR_EVENTINF(flag) (gSaveContext.eventInf[(flag) >> 4] &= (u8)~(1 << ((flag) & 0xF)))
#define CLEAR_EVENTINF_ALT(flag) (gSaveContext.eventInf[(flag) >> 4] &= ~(1 << ((flag) & 0xF)))

typedef enum {
    /* 0 */ EVENTINF_DOG_RACE_STATE_NOT_STARTED,
    /* 2 */ EVENTINF_DOG_RACE_STATE_STARTED = 2,
    /* 3 */ EVENTINF_DOG_RACE_STATE_ENDED,
    /* 7 */ EVENTINF_DOG_RACE_STATE_MAX = 7
} EventInfDogRaceState;

#define GET_EVENTINF_DOG_RACE_STATE (gSaveContext.eventInf[0] & EVENTINF_DOG_RACE_STATE_MAX)
#define SET_EVENTINF_DOG_RACE_STATE(state) \
    (gSaveContext.eventInf[0] = ((gSaveContext.eventInf[0] & (u8)~EVENTINF_DOG_RACE_STATE_MAX) | (state)))

#define GET_EVENTINF_DOG_RACE_SELECTED_DOG_INDEX ((gSaveContext.eventInf[0] & (u8)~EVENTINF_DOG_RACE_STATE_MAX) >> 3)
#define SET_EVENTINF_DOG_RACE_SELECTED_DOG_INDEX(selectedDogIndex) \
    (gSaveContext.eventInf[0] = (gSaveContext.eventInf[0] & EVENTINF_DOG_RACE_STATE_MAX) | (selectedDogIndex << 3))

// This is exactly the same as the selected dog index; they never need both of them at the same time.
#define GET_EVENTINF_DOG_RACE_RACE_STANDING ((gSaveContext.eventInf[0] & (u8)~EVENTINF_DOG_RACE_STATE_MAX) >> 3)
#define SET_EVENTINF_DOG_RACE_RACE_STANDING(raceStanding) \
    (gSaveContext.eventInf[0] = (gSaveContext.eventInf[0] & EVENTINF_DOG_RACE_STATE_MAX) | (raceStanding << 3))

typedef enum {
    /* 0 */ DUNGEON_INDEX_WOODFALL_TEMPLE,
    /* 1 */ DUNGEON_INDEX_SNOWHEAD_TEMPLE,
    /* 2 */ DUNGEON_INDEX_GREAT_BAY_TEMPLE,
    /* 3 */ DUNGEON_INDEX_STONE_TOWER_TEMPLE, // Also applies to Inverted Stone Tower Temple
} DungeonIndex;

void Sram_ActivateOwl(u8 owlId);
void Sram_ClearFlagsAtDawnOfTheFirstDay(void);
void Sram_SaveEndOfCycle(struct PlayState* play);
void Sram_IncrementDay(void);
u16 Sram_CalcChecksum(void* data, size_t count);
void Sram_InitNewSave(void);
void Sram_InitDebugSave(void);
void Sram_ResetSaveFromMoonCrash(SramContext* sramCtx);
void Sram_OpenSave(struct FileSelectState* fileSelect, SramContext* sramCtx);
void func_8014546C(SramContext* sramCtx);
void func_801457CC(struct FileSelectState* fileSelect, SramContext* sramCtx);
void func_80146580(struct FileSelectState* fileSelect2, SramContext* sramCtx, s32 fileNum);
void func_80146628(struct FileSelectState* fileSelect2, SramContext* sramCtx);
void Sram_InitSave(struct FileSelectState* fileSelect2, SramContext* sramCtx);
void func_80146DF8(SramContext* sramCtx);
void Sram_InitSram(struct GameState* gameState, SramContext* sramCtx);
void Sram_Alloc(struct GameState* gameState, SramContext* sramCtx);
void Sram_SaveSpecialEnterClockTown(struct PlayState* play);
void Sram_SaveSpecialNewDay(struct PlayState* play);
void func_80147008(SramContext* sramCtx, u32 curPage, u32 numPages);
void func_80147020(SramContext* sramCtx);
void func_80147068(SramContext* sramCtx);
void func_80147138(SramContext* sramCtx, s32 curPage, s32 numPages);
void func_80147150(SramContext* sramCtx);
void func_80147198(SramContext* sramCtx);

extern s32 D_801C6798[];
extern u8 gAmmoItems[];
extern s32 D_801C67C8[];
extern s32 D_801C67F0[];
extern s32 D_801C6818[];
extern s32 D_801C6840[];
extern s32 D_801C6850[];

#endif
