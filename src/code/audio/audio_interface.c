#include "global.h"

typedef struct {
    /* 0x0 */ s8 x;
    /* 0x1 */ s8 y;
} OcarinaControlStick; // size = 0x2

typedef struct {
    /* 0x0 */ u16 playerIO;
    /* 0x2 */ u16 channelMask;
    /* 0x4 */ u8 channelIO[3 * 33 + 1];
} NatureAmbienceDataIO; // size = 0x68

typedef struct {
    /* 0x0 */ Vec3f* pos;
    /* 0x4 */ f32 freqScale;
    /* 0x8 */ s8 reverbAdd;
} SfxSettings; // size = 0x9

typedef struct {
    /* 0x0 */ f32 vol;
    /* 0x4 */ f32 freqScale;
    /* 0x8 */ s8 reverb;
    /* 0x9 */ s8 panSigned;
    /* 0xA */ s8 stereoBits;
    /* 0xB */ u8 filter;
    /* 0xC */ u8 unk_0C;
    /* 0xD */ u8 unk_0D;
} SfxPlayerState; // size = 0xE

typedef enum {
    /* 0x0 */ SFX_CHANNEL_PLAYER0, // SfxPlayerBank
    /* 0x1 */ SFX_CHANNEL_PLAYER1,
    /* 0x2 */ SFX_CHANNEL_PLAYER2,
    /* 0x3 */ SFX_CHANNEL_ITEM0, // SfxItemBank
    /* 0x4 */ SFX_CHANNEL_ITEM1,
    /* 0x5 */ SFX_CHANNEL_ENV0, // SfxEnvironmentBank
    /* 0x6 */ SFX_CHANNEL_ENV1,
    /* 0x7 */ SFX_CHANNEL_ENV2,
    /* 0x8 */ SFX_CHANNEL_ENEMY0, // SfxEnemyBank
    /* 0x9 */ SFX_CHANNEL_ENEMY1,
    /* 0xA */ SFX_CHANNEL_ENEMY2,
    /* 0xB */ SFX_CHANNEL_SYSTEM0, // SfxSystemBank
    /* 0xC */ SFX_CHANNEL_SYSTEM1,
    /* 0xD */ SFX_CHANNEL_OCARINA, // SfxOcarinaBank
    /* 0xE */ SFX_CHANNEL_VOICE0,  // SfxVoiceBank
    /* 0xF */ SFX_CHANNEL_VOICE1
} SfxChannelIdx; // playerIdx = 2

typedef enum {
    /* 0x0 */ PLAYER_IO_PORT_0,
    /* 0x1 */ PLAYER_IO_PORT_1,
    /* 0x2 */ PLAYER_IO_PORT_2,
    /* 0x3 */ PLAYER_IO_PORT_3,
    /* 0x4 */ PLAYER_IO_PORT_4,
    /* 0x5 */ PLAYER_IO_PORT_5,
    /* 0x6 */ PLAYER_IO_PORT_6,
    /* 0x7 */ PLAYER_IO_PORT_7,
} PlayerIOPort;

typedef struct {
    /* 0x0 */ f32 value;
    /* 0x4 */ f32 target;
    /* 0x8 */ f32 step;
    /* 0xC */ s32 remainingFrames;
} FreqLerp; // size = 0x10

s32 AudioOcarina_MemoryGameGenerateNotes(void);
void Audio_StopSequenceAndBgmVolumeOn(u8 playerIdx, u8 volumeFadeTimer);
void Audio_SetupBgmForNewDayScene(u16 seqId);
void Audio_StartBgmForScene(u16 seqId);
void Audio_PlaySeqWithPlayerIO(s8 playerIdx, u16 seqId, u8 fadeTimer, s8 arg3, u8 arg4);
void func_801A4FD8(void);
void Audio_StopSequence(s32, s32);
void Audio_StartSequence(u8 playerIdx, u8 seqId, u8 arg2, u16 fadeTimer);
s32 Audio_IsSeqCmdNotQueued(u32 arg0, u32 arg1);
void Audio_ResetSequencesAndVolume(void);
void AudioVoice_ResetData(void);
void Audio_ResetSequences(void);
void Audio_ProcessSeqCmds(void);
void Audio_SetSfxReverbIndexExceptOcarinaBank(u8 reverbIndex);
void Audio_MuteBgmPlayersForFanfare(void);
void Audio_PlayNatureAmbienceSequence(u8 natureSeqId);
s32 Audio_SetGanonsTowerBgmVolume(u8);
void Audio_StepFreqLerp(FreqLerp* lerp);
void AudioOcarina_SetCustomSequence(void);
void Audio_ProcessSfxSettings(void);
void Audio_UpdateSfxVolumeTransition(void);
void Audio_UpdateRiverSoundVolumes(void);
void Audio_UpdateSequenceAtFixedPos(void);
void Audio_UpdateFanfareAtFixedPos(void);
void Audio_UpdateSubBgmAtFixedPos1(void);
void Audio_UpdateSubBgmAtFixedPos2(void);
void Audio_UpdateBgmPlayerIOPort7(void);
void Audio_UpdateFanfare(void);
void Audio_UpdatePauseState(void);
void Audio_ResetNewSceneSeqId(void);
void AudioVoice_Update(void);
void Audio_UpdateActiveSequences(void);
s32 func_801A9768(void); 
s32 func_801A982C(void); 

// bss
SfxSettings sSfxSettings[8];
u8 sSfxSettingsFlags;
f32 sTwoSemitonesLoweredFreq;
s8 sIncreasedSfxReverb;
f32 sSyncedSfxVolume;
f32 sSyncedSfxVolumeForMetalEffects;
f32 sSyncedSfxFreq;
FreqLerp sRiverFreqScaleLerp;
FreqLerp sWaterfallFreqScaleLerp;
f32 sAdjustedSfxFreq;
s8 D_801FD28C;
u8 sRiverSoundMainBgmVol;
u8 sRiverSoundMainBgmCurrentVol;
u8 sRiverSoundMainBgmLower;
u8 sRiverSoundMainBgmRestore;
u8 sGanonsTowerVol;
f32* sSfxVolumeCur;
f32 sSfxVolumeTarget;
f32 sSfxVolumeRate;
u16 sNewSceneSeqId;
SfxPlayerState sSfxChannelState[16];
u8 D_801FD3A8;
u8 D_801FD3A9;
u8 D_801FD3AA;
u8 sFanfareState;
u16 sFanfareSeqId;
u8 sMuteOnlySfxAndNatureSeq;
u8 sAllPlayersMutedExceptOcaAndSys;
u8 sAudioPauseState;
u8 D_801FD3B4[4]; // Should be [5], bug?
u8 sSequenceFilter[8 * 4]; // Filter data
u8 sIsFinalHoursOrSoaring;
u8 sFanfareAtFixedPosSeqId;
u8 sFanfareAtFixedPosInUse;
Vec3f sFanfareAtFixedPosPos;
u8 sSeqAtFixedPosPlayerIdx;
Vec3f sSeqAtFixedPosPos;
s16 sSeqAtFixedPosFlags;
f32 sSeqAtFixedPosMinDist;
f32 sSeqAtFixedPosMaxDist;
f32 sSeqAtFixedPosMaxVol;
f32 sSeqAtFixedPosMinVol;
Vec3f sSubBgmAtFixedPosNoFilterPos;
Vec3f sSubBgmAtFixedPosFilterPos;
f32 D_801FD42C;
u8 sSubBgmAtFixedPosSeqId;
u8 sSubBgmAtFixedPosFlags;
u8 D_801FD432;
u8 D_801FD433;
u8 D_801FD434;
u8 D_801FD435;
u8 D_801FD436;
u16 D_801FD438;
OcarinaStaff sPlayingStaff;
OcarinaStaff sDisplayedStaff;
OcarinaStaff sRecordingStaff;
u32 sOcarinaUpdateTaskCurrent;
OcarinaControlStick sOcarinaInputStickRel;
u32 sOcarinaInputButtonCur;  // TODO: Not Oca Btn Idx, but Controller Btn Idx!
u32 sOcarinaInputButtonStart;                 // Controller Btn Idx
u32 sOcarinaInputButtonPrev; // TODO: Not Oca Btn Idx, but Controller Btn Idx!
s32 sOcaInputBtnPress;
u8 sOcarinaResetDelay;
u8 sOcarinaResetUnused;
u8 sOcarinaHasStartedSong;
u8 sFirstOcarinaSongIdx;
u8 sLastOcarinaSongIdx;
u32 sOcarinaAvailSongs;
u8 sOcarinaStaffPlayingPos;
u16 sMusicStaffPos[OCARINA_SONG_MAX];
u16 sMusicStaffCurHeldLength[OCARINA_SONG_MAX];
u16 sMusicStaffExpectedLength[OCARINA_SONG_MAX];
u8 sMusicStaffExpectedNoteIdx[OCARINA_SONG_MAX]; // Next required noteIdx in song playback
u8 D_801FD518[OCARINA_SONG_MAX];
u32 D_801FD530[OCARINA_SONG_MAX];
OcarinaNote sRecordingSongNote;
u16 sCustomSequencePc;

// data
u8 D_801D6600[] = {
    true, false, true, true, false, false, true,
};

u8 D_801D6608[] = {
    true, true, true, true, true, false, true,
};

u8 gChannelsPerBank[4][7] = {
    { 3, 2, 3, 3, 2, 1, 2 },
    { 3, 2, 2, 2, 2, 2, 2 },
    { 3, 2, 2, 2, 2, 2, 2 },
    { 4, 1, 0, 0, 2, 2, 2 },
};

u8 gUsedChannelsPerBank[4][7] = {
    { 3, 2, 3, 2, 2, 1, 1 },
    { 3, 1, 1, 1, 2, 1, 1 },
    { 3, 1, 1, 1, 2, 1, 1 },
    { 2, 1, 0, 0, 1, 1, 1 },
};

f32 sGiantsMaskFreq = 0.89167805f; // Around 2 semitones down in pitch
s8 sGiantsMaskReverbAdd = 0x40;
f32 sWaterWheelVolume = 0.65f;
f32 sSfxVolume = 1.0f;
s8 sSfxTimer = 20;
s8 sSfxTimerLerpRange2 = 30;
s8 sSfxTimerLerpRange1 = 20;
f32 sBehindScreenZ[2] = { -15.0f, -65.0f }; // Unused Remnant of OoT
u8 sAudioIncreasingTranspose = 0;           // Remnant of OoT, only unsed in unused functions
u8 gMorphaTransposeTable[16] = { 0, 0, 0, 1, 1, 2, 4, 6, 8, 8, 8, 8, 8, 8, 8, 8 }; // Unused Remnant of OoT
u8 sPrevChargeLevel = 0;
f32 sChargeLevelsSfxFreq[] = { 1.0f, 1.12246f, 1.33484f, 1.33484f };
f32 sCurChargeLevelSfxFreq = 1.0f;
u8 sGanonsTowerLevelsVol[] = {
    127, 80, 75, 73, 70, 68, 65, 60 // volumes
};
u8 sEnterGanonsTowerTimer = 0;
u16 sSfxVolumeDuration = 0;
s8 sAudioFileSelectSetting = AUDIO_FS_STEREO;
s8 sAudioIsWindowOpen = false;
s8 sAudioCutsceneFlag = false;
s8 sSpecReverb = 0;
s8 sAudioEnvReverb = 0;
s8 sAudioCodeReverb = 0;
u8 sPrevSeqMode = 0;
f32 sAudioEnemyDist = 0.0f;
s8 sAudioEnemyVol = 127;
u16 sPrevMainBgmSeqId = NA_BGM_DISABLED;
u8 sBgmPlayerIOPort7 = 0;
u8 sSeqIdForBgmPlayerIOPort7 = NA_BGM_GENERAL_SFX;
u32 sNumFramesStill = 0;
u32 sNumFramesMoving = 0;
u8 sAudioBaseFilter = 0;
u8 sAudioExtraFilter = 0;
u8 sAudioBaseFilter2 = 0;
u8 sAudioExtraFilter2 = 0;
s8 gUnderwaterSfxReverbAdd = 0;
Vec3f* sSariaBgmPtr = NULL;
f32 D_801D66F8 = 2000.0f;
u8 sSeqAtFixedPosSeqId = NA_BGM_GENERAL_SFX;

// OoT D_80130658
u8 sSeqFlags[] = {
    0x3,  // NA_BGM_GENERAL_SFX
    0x1,  // NA_BGM_NATURE_AMBIENCE
    0x1,  // NA_BGM_TERMINA_FIELD
    0x8,  // NA_BGM_CHASE
    0,    // NA_BGM_MAJORAS_THEME
    0,    // NA_BGM_CLOCK_TOWER
    0x1,  // NA_BGM_STONE_TOWER_TEMPLE
    0x1,  // NA_BGM_INV_STONE_TOWER_TEMPLE
    0x2,  // NA_BGM_FAILURE_0
    0x2,  // NA_BGM_FAILURE_1
    0,    // NA_BGM_HAPPY_MASK_SALESMAN
    0,    // NA_BGM_SONG_OF_HEALING
    0x1,  // NA_BGM_SWAMP_REGION
    0,    // NA_BGM_ALIEN_INVASION
    0x2,  // NA_BGM_SWAMP_CRUISE
    0,    // NA_BGM_SHARPS_CURSE
    0x1,  // NA_BGM_GREAT_BAY_REGION
    0x1,  // NA_BGM_IKANA_REGION
    0,    // NA_BGM_DEKU_KING
    0x1,  // NA_BGM_MOUNTAIN_REGION
    0,    // NA_BGM_PIRATES_FORTRESS
    0,    // NA_BGM_CLOCK_TOWN_DAY_1
    0,    // NA_BGM_CLOCK_TOWN_DAY_2
    0,    // NA_BGM_CLOCK_TOWN_DAY_3
    0x40, // NA_BGM_FILE_SELECT
    0x10, // NA_BGM_CLEAR_EVENT
    0,    // NA_BGM_ENEMY
    0x8,  // NA_BGM_BOSS
    0x1,  // NA_BGM_WOODFALL_TEMPLE
    0,    // NA_BGM_MARKET
    0,    // NA_BGM_OPENING
    0x20, // NA_BGM_INSIDE_A_HOUSE
    0x2,  // NA_BGM_GAME_OVER
    0,    // NA_BGM_CLEAR_BOSS
    0x2,  // NA_BGM_GET_ITEM
    0x2,  // NA_BGM_GATE_OPEN
    0x2,  // NA_BGM_GET_HEART
    0x8,  // NA_BGM_MINI_GAME_2
    0,    // NA_BGM_GORON_RACE
    0,    // NA_BGM_MUSIC_BOX_HOUSE
    0,    // NA_BGM_FAIRY_FOUNTAIN
    0,    // NA_BGM_ZELDAS_LULLABY
    0,    // NA_BGM_ROSA_SISTERS
    0x2,  // NA_BGM_OPEN_CHEST
    0,    // NA_BGM_MARINE_RESEARCH_LAB
    0x40, // NA_BGM_GIANTS_THEME
    0,    // NA_BGM_SONG_OF_STORMS
    0,    // NA_BGM_ROMANI_RANCH
    0,    // NA_BGM_GORON_VILLAGE
    0,    // NA_BGM_MAYORS_OFFICE
    0x2,  // NA_BGM_OCA_EPONA
    0x2,  // NA_BGM_OCA_SUNS
    0x2,  // NA_BGM_OCA_TIME
    0x2,  // NA_BGM_OCA_STORM
    0x10, // NA_BGM_ZORA_HALL
    0x2,  // NA_BGM_GET_NEW_MASK
    0x8,  // NA_BGM_MINI_BOSS
    0x2,  // NA_BGM_GET_SMALL_ITEM
    0,    // NA_BGM_ASTRAL_OBSERVATORY
    0x1,  // NA_BGM_CAVERN
    0x11, // NA_BGM_MILK_BAR
    0x2,  // NA_BGM_ZELDA_APPEAR
    0,    // NA_BGM_SARIAS_SONG
    0,    // NA_BGM_GORON_GOAL
    0,    // NA_BGM_HORSE
    0,    // NA_BGM_HORSE_GOAL
    0,    // NA_BGM_INGO
    0,    // NA_BGM_KOTAKE_POTION_SHOP
    0x20, // NA_BGM_SHOP
    0x2,  // NA_BGM_OWL
    0x20, // NA_BGM_MINI_GAME
    0x2,  // NA_BGM_OCA_SOARING
    0x2,  // NA_BGM_OCA_HEALING
    0x2,  // NA_BGM_INVERTED_SONG_OF_TIME
    0x2,  // NA_BGM_SONG_OF_DOUBLE_TIME
    0x2,  // NA_BGM_SONATA_OF_AWAKENING
    0x2,  // NA_BGM_GORON_LULLABY
    0x2,  // NA_BGM_NEW_WAVE_BOSSA_NOVA
    0x2,  // NA_BGM_ELEGY_OF_EMPTINESS
    0x2,  // NA_BGM_OATH_TO_ORDER
    0,    // NA_BGM_SWORD_TRAINING_HALL
    0x2,  // NA_BGM_GORON_LULLABY_INTRO
    0x2,  // NA_BGM_OCA_FAIRY
    0x2,  // NA_BGM_BREMEN_MARCH
    0x2,  // NA_BGM_BALLAD_OF_THE_WIND_FISH
    0x8,  // NA_BGM_SONG_OF_SOARING
    0,    // NA_BGM_MILK_BAR_DUPLICATE
    0,    // NA_BGM_FINAL_HOURS
    0x2,  // NA_BGM_MIKAU_RIFF
    0x2,  // NA_BGM_MIKAU_FINALE
    0,    // NA_BGM_FROG_SONG
    0x2,  // NA_BGM_OCA_SONATA
    0x2,  // NA_BGM_OCA_LULLABY
    0x2,  // NA_BGM_OCA_NEW_WAVE
    0x2,  // NA_BGM_OCA_ELEGY
    0x2,  // NA_BGM_OCA_OATH
    0,    // NA_BGM_MAJORAS_LAIR
    0x2,  // NA_BGM_OCA_LULLABY_INTRO
    0x2,  // NA_BGM_OCA_GUITAR_BASS_SESSION
    0x2,  // NA_BGM_PIANO_SESSION
    0x2,  // NA_BGM_INDIGO_GO_SESSION
    0x1,  // NA_BGM_SNOWHEAD_TEMPLE
    0x1,  // NA_BGM_GREAT_BAY_TEMPLE
    0x2,  // NA_BGM_NEW_WAVE_SAXOPHONE
    0x2,  // NA_BGM_NEW_WAVE_VOCAL
    0,    // NA_BGM_MAJORAS_WRATH
    0,    // NA_BGM_MAJORAS_INCARNATION
    0,    // NA_BGM_MAJORAS_MASK
    0x2,  // NA_BGM_BASS_PLAY
    0x2,  // NA_BGM_DRUMS_PLAY
    0x2,  // NA_BGM_PIANO_PLAY
    0x1,  // NA_BGM_IKANA_CASTLE
    0,    // NA_BGM_GATHERING_GIANTS
    0x4,  // NA_BGM_KAMARO_DANCE
    0,    // NA_BGM_CREMIA_CARRIAGE
    0x2,  // NA_BGM_KEATON_QUIZ
    0,    // NA_BGM_END_CREDITS_1
    0,    // NA_BGM_OPENING_LOOP
    0,    // NA_BGM_TITLE_THEME
    0x2,  // NA_BGM_DUNGEON_APPEAR
    0x2,  // NA_BGM_WOODFALL_CLEAR
    0x2,  // NA_BGM_SNOWHEAD_CLEAR
    0,    //
    0,    // NA_BGM_INTO_THE_MOON
    0,    // NA_BGM_GOODBYE_GIANT
    0,    // NA_BGM_TATL_AND_TAEL
    0,    // NA_BGM_MOONS_DESTRUCTION
    0,    // NA_BGM_END_CREDITS_2
};

s8 sSpecReverbs[20] = {
    0, 0, 0, 0, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

NatureAmbienceDataIO sNatureAmbienceData[20] = {
    // natureSeqId: 0
    {
        0xC0FF, // PlayerIO Data
        0xC0FE, // Channel Mask
        {
            // Channel 0
            NATURE_IO_STREAM_0_TYPE(NATURE_STREAM_0),
            NATURE_IO_STREAM_0_PORT3(0),

            // Channel 1
            NATURE_IO_CRITTER_0_TYPE(NATURE_CRITTER_09),
            NATURE_IO_CRITTER_0_BEND_PITCH(64),
            NATURE_IO_CRITTER_0_NUM_LAYERS(0),
            NATURE_IO_CRITTER_0_PORT5(32),

            // Channel 2
            NATURE_IO_CRITTER_1_TYPE(NATURE_CRITTER_04),
            NATURE_IO_CRITTER_1_BEND_PITCH(0),
            NATURE_IO_CRITTER_1_NUM_LAYERS(1),
            NATURE_IO_CRITTER_1_PORT5(16),

            // Channel 3
            NATURE_IO_CRITTER_2_TYPE(NATURE_CRITTER_10),
            NATURE_IO_CRITTER_2_BEND_PITCH(112),
            NATURE_IO_CRITTER_2_NUM_LAYERS(1),
            NATURE_IO_CRITTER_2_PORT5(48),

            // Channel 4
            NATURE_IO_CRITTER_3_TYPE(NATURE_CRITTER_03),
            NATURE_IO_CRITTER_3_BEND_PITCH(127),
            NATURE_IO_CRITTER_3_NUM_LAYERS(0),
            NATURE_IO_CRITTER_3_PORT5(16),

            // Channel 5
            NATURE_IO_CRITTER_4_TYPE(NATURE_CRITTER_00),
            NATURE_IO_CRITTER_4_BEND_PITCH(127),
            NATURE_IO_CRITTER_4_NUM_LAYERS(1),
            NATURE_IO_CRITTER_4_PORT5(16),

            // Channel 6
            NATURE_IO_CRITTER_5_TYPE(NATURE_CRITTER_01),
            NATURE_IO_CRITTER_5_BEND_PITCH(127),
            NATURE_IO_CRITTER_5_NUM_LAYERS(3),
            NATURE_IO_CRITTER_5_PORT5(16),

            // End
            NATURE_IO_ENTRIES_END,
        },
    },

    // natureSeqId: 1
    {
        0xC0FF, // PlayerIO Data
        0xC0FE, // Channel Mask
        {
            // Channel 0
            NATURE_IO_STREAM_0_TYPE(NATURE_STREAM_0),
            NATURE_IO_STREAM_0_PORT3(0),

            // Channel 1
            NATURE_IO_CRITTER_0_TYPE(NATURE_CRITTER_04),
            NATURE_IO_CRITTER_0_BEND_PITCH(64),
            NATURE_IO_CRITTER_0_NUM_LAYERS(0),
            NATURE_IO_CRITTER_0_PORT5(32),

            // Channel 2
            NATURE_IO_CRITTER_1_TYPE(NATURE_CRITTER_16),
            NATURE_IO_CRITTER_1_BEND_PITCH(0),
            NATURE_IO_CRITTER_1_NUM_LAYERS(1),
            NATURE_IO_CRITTER_1_PORT5(16),

            // Channel 3
            NATURE_IO_CRITTER_2_TYPE(NATURE_CRITTER_12),
            NATURE_IO_CRITTER_2_BEND_PITCH(112),
            NATURE_IO_CRITTER_2_NUM_LAYERS(0),
            NATURE_IO_CRITTER_2_PORT5(48),

            // Channel 4
            NATURE_IO_CRITTER_3_TYPE(NATURE_CRITTER_15),
            NATURE_IO_CRITTER_3_BEND_PITCH(127),
            NATURE_IO_CRITTER_3_NUM_LAYERS(1),
            NATURE_IO_CRITTER_3_PORT5(16),

            // Channel 5
            NATURE_IO_CRITTER_4_TYPE(NATURE_CRITTER_06),
            NATURE_IO_CRITTER_4_BEND_PITCH(127),
            NATURE_IO_CRITTER_4_NUM_LAYERS(1),
            NATURE_IO_CRITTER_4_PORT5(16),

            // Channel 6
            NATURE_IO_CRITTER_5_TYPE(NATURE_CRITTER_01),
            NATURE_IO_CRITTER_5_BEND_PITCH(127),
            NATURE_IO_CRITTER_5_NUM_LAYERS(3),
            NATURE_IO_CRITTER_5_PORT5(16),

            // End
            NATURE_IO_ENTRIES_END,
        },
    },

    // natureSeqId: 2
    {
        0xC0FF, // PlayerIO Data
        0xC0FE, // Channel Mask
        {
            // Channel 0
            NATURE_IO_STREAM_0_TYPE(NATURE_STREAM_0),
            NATURE_IO_STREAM_0_PORT3(0),

            // Channel 1
            NATURE_IO_CRITTER_0_TYPE(NATURE_CRITTER_04),
            NATURE_IO_CRITTER_0_BEND_PITCH(64),
            NATURE_IO_CRITTER_0_NUM_LAYERS(0),
            NATURE_IO_CRITTER_0_PORT5(48),

            // Channel 2
            NATURE_IO_CRITTER_1_TYPE(NATURE_CRITTER_10),
            NATURE_IO_CRITTER_1_BEND_PITCH(0),
            NATURE_IO_CRITTER_1_NUM_LAYERS(1),
            NATURE_IO_CRITTER_1_PORT5(16),

            // Channel 3
            NATURE_IO_CRITTER_2_TYPE(NATURE_CRITTER_04),
            NATURE_IO_CRITTER_2_BEND_PITCH(48),
            NATURE_IO_CRITTER_2_NUM_LAYERS(1),
            NATURE_IO_CRITTER_2_PORT5(32),

            // Channel 4
            NATURE_IO_CRITTER_3_TYPE(NATURE_CRITTER_03),
            NATURE_IO_CRITTER_3_BEND_PITCH(127),
            NATURE_IO_CRITTER_3_NUM_LAYERS(0),
            NATURE_IO_CRITTER_3_PORT5(16),

            // Channel 5
            NATURE_IO_CRITTER_4_TYPE(NATURE_CRITTER_01),
            NATURE_IO_CRITTER_4_BEND_PITCH(64),
            NATURE_IO_CRITTER_4_NUM_LAYERS(1),
            NATURE_IO_CRITTER_4_PORT5(0),

            // Channel 6
            NATURE_IO_CRITTER_5_TYPE(NATURE_CRITTER_04),
            NATURE_IO_CRITTER_5_BEND_PITCH(127),
            NATURE_IO_CRITTER_5_NUM_LAYERS(0),
            NATURE_IO_CRITTER_5_PORT5(63),

            // End
            NATURE_IO_ENTRIES_END,
        },
    },

    // natureSeqId: 3
    {
        0xC0FF, // PlayerIO Data
        0xC0FE, // Channel Mask
        {
            // Channel 0
            NATURE_IO_STREAM_0_TYPE(NATURE_STREAM_0),
            NATURE_IO_STREAM_0_PORT3(0),

            // Channel 1
            NATURE_IO_CRITTER_0_TYPE(NATURE_CRITTER_09),
            NATURE_IO_CRITTER_0_BEND_PITCH(64),
            NATURE_IO_CRITTER_0_NUM_LAYERS(0),
            NATURE_IO_CRITTER_0_PORT5(32),

            // Channel 2
            NATURE_IO_CRITTER_1_TYPE(NATURE_CRITTER_04),
            NATURE_IO_CRITTER_1_BEND_PITCH(64),
            NATURE_IO_CRITTER_1_NUM_LAYERS(0),
            NATURE_IO_CRITTER_1_PORT5(48),

            // Channel 3
            NATURE_IO_CRITTER_2_TYPE(NATURE_CRITTER_10),
            NATURE_IO_CRITTER_2_BEND_PITCH(32),
            NATURE_IO_CRITTER_2_NUM_LAYERS(1),
            NATURE_IO_CRITTER_2_PORT5(32),

            // Channel 4
            NATURE_IO_CRITTER_3_TYPE(NATURE_CRITTER_14),
            NATURE_IO_CRITTER_3_BEND_PITCH(64),
            NATURE_IO_CRITTER_3_NUM_LAYERS(1),
            NATURE_IO_CRITTER_3_PORT5(16),

            // Channel 5
            NATURE_IO_CRITTER_4_TYPE(NATURE_CRITTER_00),
            NATURE_IO_CRITTER_4_BEND_PITCH(127),
            NATURE_IO_CRITTER_4_NUM_LAYERS(1),
            NATURE_IO_CRITTER_4_PORT5(16),

            // Channel 6
            NATURE_IO_CRITTER_5_TYPE(NATURE_CRITTER_01),
            NATURE_IO_CRITTER_5_BEND_PITCH(127),
            NATURE_IO_CRITTER_5_NUM_LAYERS(3),
            NATURE_IO_CRITTER_5_PORT5(16),

            // End
            NATURE_IO_ENTRIES_END,
        },
    },

    // natureSeqId: 4
    {
        0xC0FF, // PlayerIO Data
        0xC0FE, // Channel Mask
        {
            // Channel 0
            NATURE_IO_STREAM_0_TYPE(0),
            NATURE_IO_STREAM_0_PORT3(0),

            // Channel 1
            NATURE_IO_CRITTER_0_TYPE(NATURE_CRITTER_04),
            NATURE_IO_CRITTER_0_BEND_PITCH(64),
            NATURE_IO_CRITTER_0_NUM_LAYERS(0),
            NATURE_IO_CRITTER_0_PORT5(32),

            // Channel 2
            NATURE_IO_CRITTER_1_TYPE(NATURE_CRITTER_02),
            NATURE_IO_CRITTER_1_BEND_PITCH(64),
            NATURE_IO_CRITTER_1_NUM_LAYERS(1),
            NATURE_IO_CRITTER_1_PORT5(16),

            // Channel 3
            NATURE_IO_CRITTER_2_TYPE(NATURE_CRITTER_12),
            NATURE_IO_CRITTER_2_BEND_PITCH(112),
            NATURE_IO_CRITTER_2_NUM_LAYERS(1),
            NATURE_IO_CRITTER_2_PORT5(48),

            // Channel 4
            NATURE_IO_CRITTER_3_TYPE(NATURE_CRITTER_13),
            NATURE_IO_CRITTER_3_BEND_PITCH(64),
            NATURE_IO_CRITTER_3_NUM_LAYERS(1),
            NATURE_IO_CRITTER_3_PORT5(16),

            // Channel 5
            NATURE_IO_CRITTER_4_TYPE(NATURE_CRITTER_01),
            NATURE_IO_CRITTER_4_BEND_PITCH(64),
            NATURE_IO_CRITTER_4_NUM_LAYERS(1),
            NATURE_IO_CRITTER_4_PORT5(16),

            // Channel 6
            NATURE_IO_CRITTER_5_TYPE(NATURE_CRITTER_02),
            NATURE_IO_CRITTER_5_BEND_PITCH(112),
            NATURE_IO_CRITTER_5_NUM_LAYERS(0),
            NATURE_IO_CRITTER_5_PORT5(48),

            // Channel 14
            NATURE_IO_RAIN_PORT4(63),

            // End
            NATURE_IO_ENTRIES_END,

        },
    },

    // natureSeqId: 5
    {
        0xC0FF, // PlayerIO Data
        0xC0FE, // Channel Mask
        {
            // Channel 0
            NATURE_IO_STREAM_0_TYPE(NATURE_STREAM_0),
            NATURE_IO_STREAM_0_PORT3(0),

            // Channel 1
            NATURE_IO_CRITTER_0_TYPE(NATURE_CRITTER_09),
            NATURE_IO_CRITTER_0_BEND_PITCH(64),
            NATURE_IO_CRITTER_0_NUM_LAYERS(0),
            NATURE_IO_CRITTER_0_PORT5(32),

            // Channel 2
            NATURE_IO_CRITTER_1_TYPE(NATURE_CRITTER_04),
            NATURE_IO_CRITTER_1_BEND_PITCH(0),
            NATURE_IO_CRITTER_1_NUM_LAYERS(1),
            NATURE_IO_CRITTER_1_PORT5(16),

            // Channel 3
            NATURE_IO_CRITTER_2_TYPE(NATURE_CRITTER_10),
            NATURE_IO_CRITTER_2_BEND_PITCH(112),
            NATURE_IO_CRITTER_2_NUM_LAYERS(1),
            NATURE_IO_CRITTER_2_PORT5(48),

            // Channel 4
            NATURE_IO_CRITTER_3_TYPE(NATURE_CRITTER_13),
            NATURE_IO_CRITTER_3_BEND_PITCH(127),
            NATURE_IO_CRITTER_3_NUM_LAYERS(0),
            NATURE_IO_CRITTER_3_PORT5(63),

            // Channel 5
            NATURE_IO_CRITTER_4_TYPE(NATURE_CRITTER_00),
            NATURE_IO_CRITTER_4_BEND_PITCH(127),
            NATURE_IO_CRITTER_4_NUM_LAYERS(1),
            NATURE_IO_CRITTER_4_PORT5(16),

            // Channel 6
            NATURE_IO_CRITTER_5_TYPE(NATURE_CRITTER_01),
            NATURE_IO_CRITTER_5_BEND_PITCH(127),
            NATURE_IO_CRITTER_5_NUM_LAYERS(3),
            NATURE_IO_CRITTER_5_PORT5(16),

            // End
            NATURE_IO_ENTRIES_END,
        },
    },

    // natureSeqId: 6
    {
        0xC0FF, // PlayerIO Data
        0xC0FE, // Channel Mask
        {
            // Channel 0
            NATURE_IO_STREAM_0_TYPE(NATURE_STREAM_0),
            NATURE_IO_STREAM_0_PORT3(0),

            // Channel 1
            NATURE_IO_CRITTER_0_TYPE(NATURE_CRITTER_11),
            NATURE_IO_CRITTER_0_BEND_PITCH(112),
            NATURE_IO_CRITTER_0_NUM_LAYERS(0),
            NATURE_IO_CRITTER_0_PORT5(48),

            // Channel 2
            NATURE_IO_CRITTER_1_TYPE(NATURE_CRITTER_15),
            NATURE_IO_CRITTER_1_BEND_PITCH(112),
            NATURE_IO_CRITTER_1_NUM_LAYERS(0),
            NATURE_IO_CRITTER_1_PORT5(63),

            // Channel 3
            NATURE_IO_CRITTER_2_TYPE(NATURE_CRITTER_11),
            NATURE_IO_CRITTER_2_BEND_PITCH(48),
            NATURE_IO_CRITTER_2_NUM_LAYERS(1),
            NATURE_IO_CRITTER_2_PORT5(16),

            // Channel 4
            NATURE_IO_CRITTER_3_TYPE(NATURE_CRITTER_14),
            NATURE_IO_CRITTER_3_BEND_PITCH(48),
            NATURE_IO_CRITTER_3_NUM_LAYERS(1),
            NATURE_IO_CRITTER_3_PORT5(16),

            // Channel 5
            NATURE_IO_CRITTER_4_TYPE(NATURE_CRITTER_11),
            NATURE_IO_CRITTER_4_BEND_PITCH(127),
            NATURE_IO_CRITTER_4_NUM_LAYERS(0),
            NATURE_IO_CRITTER_4_PORT5(32),

            // Channel 6
            NATURE_IO_CRITTER_5_TYPE(NATURE_CRITTER_02),
            NATURE_IO_CRITTER_5_BEND_PITCH(127),
            NATURE_IO_CRITTER_5_NUM_LAYERS(0),
            NATURE_IO_CRITTER_5_PORT5(48),

            // End
            NATURE_IO_ENTRIES_END,
        },
    },

    // natureSeqId: 7
    {
        0xC001, // PlayerIO Data
        0xC000, // Channel Mask
        {
            // Channel 0
            NATURE_IO_STREAM_0_TYPE(NATURE_STREAM_0),
            NATURE_IO_STREAM_0_PORT3(0),

            // End
            NATURE_IO_ENTRIES_END,
        },
    },

    // natureSeqId: 8
    {
        0xC003, // PlayerIO Data
        0xC000, // Channel Mask
        {
            // Channel 0
            NATURE_IO_STREAM_0_TYPE(NATURE_STREAM_0),
            NATURE_IO_STREAM_0_PORT3(0),

            // Channel 1
            NATURE_IO_CRITTER_0_TYPE(NATURE_CRITTER_01),
            NATURE_IO_CRITTER_0_BEND_PITCH(127),
            NATURE_IO_CRITTER_0_NUM_LAYERS(3),
            NATURE_IO_CRITTER_0_PORT5(16),

            // End
            NATURE_IO_ENTRIES_END,
        },
    },

    // natureSeqId: 9
    {
        0xC00F, // PlayerIO Data
        0xC000, // Channel Mask
        {
            // Channel 0
            NATURE_IO_STREAM_0_TYPE(NATURE_STREAM_0),
            NATURE_IO_STREAM_0_PORT3(0),

            // Channel 1
            NATURE_IO_CRITTER_0_TYPE(NATURE_CRITTER_16),
            NATURE_IO_CRITTER_0_BEND_PITCH(0),
            NATURE_IO_CRITTER_0_NUM_LAYERS(2),
            NATURE_IO_CRITTER_0_PORT5(16),

            // Channel 2
            NATURE_IO_CRITTER_1_TYPE(NATURE_CRITTER_12),
            NATURE_IO_CRITTER_1_BEND_PITCH(112),
            NATURE_IO_CRITTER_1_NUM_LAYERS(0),
            NATURE_IO_CRITTER_1_PORT5(48),

            // Channel 3
            NATURE_IO_CRITTER_2_TYPE(NATURE_CRITTER_15),
            NATURE_IO_CRITTER_2_BEND_PITCH(127),
            NATURE_IO_CRITTER_2_NUM_LAYERS(1),
            NATURE_IO_CRITTER_2_PORT5(16),

            // End
            NATURE_IO_ENTRIES_END,
        },
    },

    // natureSeqId: 10
    {
        0xC081, // PlayerIO Data
        0xC000, // Channel Mask
        {
            // Channel 0
            NATURE_IO_STREAM_0_TYPE(NATURE_STREAM_1),
            NATURE_IO_STREAM_0_PORT3(8),

            // Channel 7
            NATURE_IO_CRITTER_6_TYPE(NATURE_CRITTER_11),
            NATURE_IO_CRITTER_6_BEND_PITCH(112),
            NATURE_IO_CRITTER_6_NUM_LAYERS(2),
            NATURE_IO_CRITTER_6_PORT5(32),

            // End
            NATURE_IO_ENTRIES_END,
        },
    },

    // natureSeqId: 11
    {
        0xC00F, // PlayerIO Data
        0xC000, // Channel Mask
        {
            // Channel 0
            NATURE_IO_STREAM_0_TYPE(NATURE_STREAM_3),
            NATURE_IO_STREAM_0_PORT3(8),

            // Channel 1
            NATURE_IO_CRITTER_0_TYPE(NATURE_CRITTER_01),
            NATURE_IO_CRITTER_0_BEND_PITCH(127),
            NATURE_IO_CRITTER_0_NUM_LAYERS(3),
            NATURE_IO_CRITTER_0_PORT5(16),

            // Channel 2
            NATURE_IO_CRITTER_1_TYPE(NATURE_CRITTER_00),
            NATURE_IO_CRITTER_1_BEND_PITCH(127),
            NATURE_IO_CRITTER_1_NUM_LAYERS(2),
            NATURE_IO_CRITTER_1_PORT5(16),

            // Channel 3
            NATURE_IO_CRITTER_2_TYPE(NATURE_CRITTER_06),
            NATURE_IO_CRITTER_2_BEND_PITCH(127),
            NATURE_IO_CRITTER_2_NUM_LAYERS(1),
            NATURE_IO_CRITTER_2_PORT5(16),

            // End
            NATURE_IO_ENTRIES_END,
        },
    },

    // natureSeqId: 12
    {
        0xC007, // PlayerIO Data
        0xC000, // Channel Mask
        {
            // Channel 0
            NATURE_IO_STREAM_0_TYPE(NATURE_STREAM_0),

            // Channel 1
            NATURE_IO_CRITTER_0_TYPE(NATURE_CRITTER_00),
            NATURE_IO_CRITTER_0_BEND_PITCH(127),
            NATURE_IO_CRITTER_0_NUM_LAYERS(1),
            NATURE_IO_CRITTER_0_PORT5(16),

            // Channel 2
            NATURE_IO_CRITTER_1_TYPE(NATURE_CRITTER_01),
            NATURE_IO_CRITTER_1_BEND_PITCH(127),
            NATURE_IO_CRITTER_1_NUM_LAYERS(3),
            NATURE_IO_CRITTER_1_PORT5(16),

            // End
            NATURE_IO_ENTRIES_END,
        },
    },

    // natureSeqId: 13
    {
        0xC003, // PlayerIO Data
        0xC000, // Channel Mask
        {
            // Channel 0
            NATURE_IO_STREAM_0_TYPE(NATURE_STREAM_0),
            NATURE_IO_STREAM_0_PORT3(0),

            // Channel 1
            NATURE_IO_CRITTER_0_TYPE(NATURE_CRITTER_04),
            NATURE_IO_CRITTER_0_BEND_PITCH(0),
            NATURE_IO_CRITTER_0_NUM_LAYERS(1),
            NATURE_IO_CRITTER_0_PORT5(16),

            // End
            NATURE_IO_ENTRIES_END,
        },
    },

    // natureSeqId: 14
    {
        0xC003, // PlayerIO Data
        0xC000, // Channel Mask
        {
            // Channel 0
            NATURE_IO_STREAM_0_TYPE(NATURE_STREAM_0),
            NATURE_IO_STREAM_0_PORT3(0),

            // Channel 1
            NATURE_IO_CRITTER_0_TYPE(NATURE_CRITTER_04),
            NATURE_IO_CRITTER_0_BEND_PITCH(0),
            NATURE_IO_CRITTER_0_NUM_LAYERS(1),
            NATURE_IO_CRITTER_0_PORT5(16),

            // End
            NATURE_IO_ENTRIES_END,
        },
    },

    // natureSeqId: 15
    {
        0xC003, // PlayerIO Data
        0xC000, // Channel Mask
        {
            // Channel 0
            NATURE_IO_STREAM_0_TYPE(NATURE_STREAM_0),
            NATURE_IO_STREAM_0_PORT3(0),

            // Channel 1
            NATURE_IO_CRITTER_0_TYPE(NATURE_CRITTER_04),
            NATURE_IO_CRITTER_0_BEND_PITCH(0),
            NATURE_IO_CRITTER_0_NUM_LAYERS(1),
            NATURE_IO_CRITTER_0_PORT5(16),

            // End
            NATURE_IO_ENTRIES_END,
        },
    },

    // natureSeqId: 16
    {
        0xC003, // PlayerIO Data
        0xC000, // Channel Mask
        {
            // Channel 0
            NATURE_IO_STREAM_0_TYPE(NATURE_STREAM_0),
            NATURE_IO_STREAM_0_PORT3(0),

            // Channel 1
            NATURE_IO_CRITTER_0_TYPE(NATURE_CRITTER_04),
            NATURE_IO_CRITTER_0_BEND_PITCH(0),
            NATURE_IO_CRITTER_0_NUM_LAYERS(1),
            NATURE_IO_CRITTER_0_PORT5(16),

            // End
            NATURE_IO_ENTRIES_END,
        },
    },

    // natureSeqId: 17
    {
        0xC003, // PlayerIO Data
        0xC000, // Channel Mask
        {
            // Channel 0
            NATURE_IO_STREAM_0_TYPE(NATURE_STREAM_0),
            NATURE_IO_STREAM_0_PORT3(0),

            // Channel 1
            NATURE_IO_CRITTER_0_TYPE(NATURE_CRITTER_04),
            NATURE_IO_CRITTER_0_BEND_PITCH(0),
            NATURE_IO_CRITTER_0_NUM_LAYERS(1),
            NATURE_IO_CRITTER_0_PORT5(16),

            // End
            NATURE_IO_ENTRIES_END,
        },
    },

    // natureSeqId: 18
    {
        0xC000, // PlayerIO Data
        0xC000, // Channel Mask
        {
            // Channel 0
            NATURE_IO_STREAM_0_TYPE(NATURE_STREAM_0),

            // End
            NATURE_IO_ENTRIES_END,
        },
    },

    // natureSeqId: 19
    {
        0xC003, // PlayerIO Data
        0xC000, // Channel Mask
        {
            NATURE_IO_STREAM_0_TYPE(NATURE_STREAM_0),
            NATURE_IO_STREAM_0_PORT3(0),

            // Channel 1
            NATURE_IO_CRITTER_0_TYPE(NATURE_CRITTER_04),
            NATURE_IO_CRITTER_0_BEND_PITCH(0),
            NATURE_IO_CRITTER_0_NUM_LAYERS(1),
            NATURE_IO_CRITTER_0_PORT5(16),

            // End
            NATURE_IO_ENTRIES_END,

        },
    },
};

u8 sIsOcarinaInputEnabled = false;
s8 sOcarinaInstrumentId = OCARINA_INSTRUMENT_OFF;
u8 sCurOcarinaNoteIdx = NOTE_NONE;
u8 sPrevOcarinaNoteIdx = 0;
u8 sCurOcarinaButtonIdx = 0;
u8 sMusicStaffPrevNoteIdx = 0;
f32 sCurOcarinaBendFreq = 1.0f;
f32 sDefaultOcarinaVolume = 0.68503935f;
s8 sCurOcarinaBendIdx = 0;
s8 sCurOcarinaVolume = 0x57;
s8 sCurOcarinaVibrato = 0;
u8 sDisplayedState = 0;
u8 D_801D6FE4 = 0xFF;
u8 D_801D6FE8 = 0xFF;
u32 sOcarinaFlags = 0;
s32 sDisplayedNoteTimer = 0;
u16 sDisplayedNotePos = 0;
u16 sDisplayedStaffPos = 0;
u32 sPrevOcarinaSongFlags = 0; // Stores sOcarinaFlags but never used
u8 sDisplayedNoteValue = NOTE_NONE;
u8 sNotePlaybackVolume = 0;
u8 sNotePlaybackVibrato = 0;
s8 sNotePlaybackBend = 0;
f32 sNormalizedNotePlaybackTone = 1.0f;
f32 sNormalizedNotePlaybackVolume = 1.0f;
u32 sOcarinaPlaybackTaskStart = 0;
u32 sOcarinaWallCounter = 0;
u8 sCurOcarinaSong[8] = {
    NOTE_C4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_C4,
};
u8 sOcarinaSongAppendPos = 0;
u8 sOcarinaSongStartingPos = 0;

u8 sButtonToNoteMap[5] = {
    NOTE_D4, // OCARINA_BTN_A
    NOTE_F4, // OCARINA_BTN_C_DOWN
    NOTE_A4, // OCARINA_BTN_C_RIGHT
    NOTE_B4, // OCARINA_BTN_C_LEFT
    NOTE_D5, // OCARINA_BTN_C_UP
};

u8 sOcaMemoryGameAppendPos = 0;
u8 sOcaMemoryGameEndPos = 0;
u8 sOcaMemoryGameNumNotes[] = { 5, 6, 8 };
OcarinaNote sOcarinaSongNotes[OCARINA_SONG_MAX][20] = {
    // 0: Sonata of Awakening
    {
        { NOTE_D5, 19, 92, 0, 0, 0 },
        { NOTE_B4, 19, 90, 0, 0, 0 },
        { NOTE_D5, 19, 90, 0, 0, 0 },
        { NOTE_B4, 38, 90, 0, 0, 0 },
        { NOTE_D4, 39, 92, 0, 0, 0 },
        { NOTE_A4, 76, 89, 0, 0, 0 },
        { NOTE_D4, 77, 82, 0, 0, 0 },
        { NOTE_NONE, 0, 86, 0, 0, 0 },
    },

    // 1: Goron Lullaby
    {
        { NOTE_D4, 41, 80, 0, 0, 0 },
        { NOTE_A4, 40, 72, 0, 0, 0 },
        { NOTE_B4, 39, 84, 0, 0, 0 },
        { NOTE_D4, 42, 76, 0, 0, 0 },
        { NOTE_A4, 40, 84, 0, 0, 0 },
        { NOTE_B4, 39, 76, 0, 0, 0 },
        { NOTE_A4, 41, 84, 0, 0, 0 },
        { NOTE_D4, 80, 76, 0, 0, 0 },
        { NOTE_NONE, 40, 76, 0, 0, 0 },
        { NOTE_NONE, 0, 66, 0, 0, 0 },
    },

    // 2: New Wave Bossa Nova
    {
        { NOTE_B4, 64, 74, 0, 0, 0 },
        { NOTE_D5, 13, 88, 0, 0, 0 },
        { NOTE_B4, 12, 90, 0, 0, 0 },
        { NOTE_A4, 78, 88, 0, 0, 0 },
        { NOTE_F4, 12, 76, 0, 0, 0 },
        { NOTE_B4, 13, 76, 0, 0, 0 },
        { NOTE_A4, 114, 76, 6, 0, 0 },
        { NOTE_NONE, 0, 90, 0, 0, 0 },
    },

    // 3: Elegy of Emptyness
    {
        { NOTE_A4, 85, 93, 0, 0, 0 },
        { NOTE_B4, 43, 91, 0, 0, 0 },
        { NOTE_A4, 43, 93, 0, 0, 0 },
        { NOTE_F4, 21, 88, 0, 0, 0 },
        { NOTE_A4, 21, 88, 0, 0, 0 },
        { NOTE_D5, 43, 101, 0, 0, 0 },
        { NOTE_B4, 85, 95, 0, 0, 0 },
        { NOTE_NONE, 0, 94, 0, 0, 0 },
    },

    // 4: Oath to Order
    {
        { NOTE_A4, 97, 104, 0, 0, 0 },
        { NOTE_F4, 48, 88, 0, 0, 0 },
        { NOTE_D4, 49, 78, 0, 0, 0 },
        { NOTE_F4, 49, 78, 0, 0, 0 },
        { NOTE_A4, 48, 94, 0, 0, 0 },
        { NOTE_D5, 97, 100, 0, 0, 0 },
        { NOTE_NONE, 0, 96, 0, 0, 0 },

    },

    // 5: Sarias Song
    {
        { NOTE_F4, 17, 84, 0, 0, 0 },
        { NOTE_A4, 17, 88, 0, 0, 0 },
        { NOTE_B4, 34, 80, 0, 0, 0 },
        { NOTE_F4, 17, 84, 0, 0, 0 },
        { NOTE_A4, 17, 88, 0, 0, 0 },
        { NOTE_B4, 136, 80, 0, 0, 0 },
        { NOTE_NONE, 0, 90, 0, 0, 0 },
    },

    // 6: Song of Time
    {
        { NOTE_A4, 32, 84, 0, 0, 0 },
        { NOTE_D4, 65, 88, 0, 0, 0 },
        { NOTE_F4, 33, 80, 0, 0, 0 },
        { NOTE_A4, 32, 84, 0, 0, 0 },
        { NOTE_D4, 65, 88, 0, 0, 0 },
        { NOTE_F4, 99, 80, 0, 0, 0 },
        { NOTE_NONE, 0, 90, 0, 0, 0 },
    },

    // 7: Song of Healing
    {
        { NOTE_B4, 32, 88, 0, 0, 0 },
        { NOTE_A4, 33, 88, 0, 0, 0 },
        { NOTE_F4, 33, 69, 0, 0, 0 },
        { NOTE_B4, 32, 94, 0, 0, 0 },
        { NOTE_A4, 33, 88, 0, 0, 0 },
        { NOTE_F4, 121, 86, 2, 0, 0 },
        { NOTE_NONE, 10, 84, 0, 0, 0 },
        { NOTE_NONE, 0, 90, 0, 0, 0 },
    },

    // 8: Eponas Song
    {
        { NOTE_D5, 18, 84, 0, 0, 0 },
        { NOTE_B4, 18, 88, 0, 0, 0 },
        { NOTE_A4, 72, 80, 0, 0, 0 },
        { NOTE_D5, 18, 84, 0, 0, 0 },
        { NOTE_B4, 18, 88, 0, 0, 0 },
        { NOTE_A4, 144, 80, 0, 0, 0 },
        { NOTE_NONE, 0, 90, 0, 0, 0 },
    },

    // 9: Song of Soaring
    {
        { NOTE_F4, 18, 84, 0, 0, 0 },
        { NOTE_B4, 18, 80, 0, 0, 0 },
        { NOTE_D5, 36, 94, 0, 0, 0 },
        { NOTE_F4, 18, 73, 0, 0, 0 },
        { NOTE_B4, 18, 76, 0, 0, 0 },
        { NOTE_D5, 108, 96, 2, 0, 0 },
        { NOTE_NONE, 0, 90, 0, 0, 0 },
    },

    // 10: Song of Storms
    {
        { NOTE_D4, 11, 84, 0, 0, 0 },
        { NOTE_F4, 11, 88, 0, 0, 0 },
        { NOTE_D5, 45, 80, 0, 0, 0 },
        { NOTE_D4, 11, 84, 0, 0, 0 },
        { NOTE_F4, 11, 88, 0, 0, 0 },
        { NOTE_D5, 90, 80, 0, 0, 0 },
        { NOTE_NONE, 0, 90, 0, 0, 0 },
    },

    // 11: Suns Song
    {
        { NOTE_A4, 12, 84, 0, 0, 0 },
        { NOTE_F4, 13, 88, 0, 0, 0 },
        { NOTE_D5, 29, 80, 2, 0, 0 },
        { NOTE_NONE, 9, 84, 0, 0, 0 },
        { NOTE_A4, 12, 84, 0, 0, 0 },
        { NOTE_F4, 13, 88, 0, 0, 0 },
        { NOTE_D5, 120, 80, 3, 0, 0 },
        { NOTE_NONE, 0, 90, 0, 0, 0 },
    },

    // 12: Inverted Song of Time
    {
        { NOTE_F4, 32, 84, 0, 0, 0 },
        { NOTE_D4, 65, 88, 0, 0, 0 },
        { NOTE_A4, 33, 80, 0, 0, 0 },
        { NOTE_F4, 32, 84, 0, 0, 0 },
        { NOTE_D4, 65, 88, 0, 0, 0 },
        { NOTE_A4, 99, 80, 0, 0, 0 },
        { NOTE_NONE, 0, 90, 0, 0, 0 },
    },

    // 13: Song of Double Time
    {
        { NOTE_A4, 29, 84, 0, 0, 0 },
        { NOTE_NONE, 3, 84, 0, 0, 0 },
        { NOTE_A4, 30, 84, 0, 0, 0 },
        { NOTE_NONE, 3, 84, 0, 0, 0 },
        { NOTE_D4, 29, 84, 0, 0, 0 },
        { NOTE_NONE, 3, 84, 0, 0, 0 },
        { NOTE_D4, 30, 84, 0, 0, 0 },
        { NOTE_NONE, 3, 84, 0, 0, 0 },
        { NOTE_F4, 29, 84, 0, 0, 0 },
        { NOTE_NONE, 3, 84, 0, 0, 0 },
        { NOTE_F4, 99, 84, 0, 0, 0 },
        { NOTE_NONE, 0, 0, 0, 0, 0 },
    },

    // 14: Goron Lullaby Intro
    {
        { NOTE_D4, 32, 78, 0, 0, 0 },
        { NOTE_A4, 33, 90, 0, 0, 0 },
        { NOTE_B4, 33, 87, 0, 0, 0 },
        { NOTE_D4, 32, 92, 0, 0, 0 },
        { NOTE_A4, 33, 86, 0, 0, 0 },
        { NOTE_B4, 130, 80, 0, 0, 0 },
        { NOTE_NONE, 0, 66, 0, 0, 0 },
    },

    // 15: Milk Bar Jam "Ballad of the Wind Fish" Human
    {
        { NOTE_D5, 89, 80, 0, 0, 0 },
        { NOTE_A4, 41, 72, 0, 0, 0 },
        { NOTE_B4, 22, 84, 0, 0, 0 },
        { NOTE_A4, 91, 76, 0, 0, 0 },
        { NOTE_NONE, 30, 66, 0, 0, 0 },
        { NOTE_NONE, 0, 66, 0, 0, 0 },
    },

    // 16: Milk Bar Jam "Ballad of the Wind Fish" Goron
    {
        { NOTE_D4, 52, 80, 0, 0, 0 },
        { NOTE_NONE, 3, 66, 0, 0, 0 },
        { NOTE_D4, 8, 72, 0, 0, 0 },
        { NOTE_NONE, 3, 66, 0, 0, 0 },
        { NOTE_D4, 30, 84, 0, 0, 0 },
        { NOTE_NONE, 3, 66, 0, 0, 0 },
        { NOTE_F4, 34, 76, 0, 0, 0 },
        { NOTE_D4, 52, 80, 0, 0, 0 },
        { NOTE_NONE, 3, 66, 0, 0, 0 },
        { NOTE_D4, 8, 72, 0, 0, 0 },
        { NOTE_NONE, 3, 66, 0, 0, 0 },
        { NOTE_D4, 30, 84, 0, 0, 0 },
        { NOTE_NONE, 3, 66, 0, 0, 0 },
        { NOTE_A4, 34, 76, 0, 0, 0 },
        { NOTE_NONE, 0, 66, 0, 0, 0 },
    },

    // 17: Milk Bar Jam "Ballad of the Wind Fish" Zora
    {
        { NOTE_D5, 11, 80, 0, 0, 0 },
        { NOTE_A4, 11, 72, 0, 0, 0 },
        { NOTE_F4, 11, 84, 0, 0, 0 },
        { NOTE_D4, 100, 76, 0, 0, 0 },
        { NOTE_D5, 11, 84, 0, 0, 0 },
        { NOTE_B4, 11, 76, 0, 0, 0 },
        { NOTE_A4, 11, 72, 0, 0, 0 },
        { NOTE_F4, 100, 84, 0, 0, 0 },
        { NOTE_NONE, 0, 66, 0, 0, 0 },
    },

    // 18: Milk Bar Jam "Ballad of the Wind Fish" Deku
    {
        { NOTE_A4, 54, 80, 0, 0, 0 },
        { NOTE_D4, 77, 72, 0, 0, 0 },
        { NOTE_F4, 19, 84, 0, 0, 0 },
        { NOTE_B4, 20, 76, 0, 0, 0 },
        { NOTE_A4, 78, 84, 0, 0, 0 },
        { NOTE_NONE, 0, 66, 0, 0, 0 },
    },

    // 19: Evan HP (Zora Band Leader) Song Part 1
    {
        { NOTE_A4, 33, 100, 0, 0, 0 },
        { NOTE_NONE, 3, 92, 0, 0, 0 },
        { NOTE_A4, 37, 104, 0, 0, 0 },
        { NOTE_F4, 24, 100, 0, 0, 0 },
        { NOTE_D4, 70, 97, 0, 0, 0 },
        { NOTE_NONE, 3, 96, 0, 0, 0 },
        { NOTE_D4, 12, 93, 0, 0, 0 },
        { NOTE_F4, 12, 100, 0, 0, 0 },
        { NOTE_A4, 12, 62, 0, 0, 0 },
        { NOTE_D4, 170, 91, 0, 0, 0 },
        { NOTE_NONE, 0, 66, 0, 0, 0 },
    },

    // 20: Evan HP (Zora Band Leader) Song Part 2
    {
        { NOTE_B4, 33, 107, 0, 0, 0 },
        { NOTE_NONE, 3, 100, 0, 0, 0 },
        { NOTE_B4, 37, 104, 0, 0, 0 },
        { NOTE_A4, 24, 97, 0, 0, 0 },
        { NOTE_F4, 70, 104, 0, 0, 0 },
        { NOTE_NONE, 3, 104, 0, 0, 0 },
        { NOTE_F4, 12, 90, 0, 0, 0 },
        { NOTE_A4, 12, 96, 0, 0, 0 },
        { NOTE_B4, 12, 81, 0, 0, 0 },
        { NOTE_F4, 170, 66, 0, 0, 0 },
        { NOTE_NONE, 0, 66, 0, 0, 0 },
    },

    // 21: Zeldas Lullaby
    {
        { NOTE_B4, 51, 84, 0, 0, 0 },
        { NOTE_D5, 25, 88, 0, 0, 0 },
        { NOTE_A4, 78, 80, 0, 0, 0 },
        { NOTE_B4, 51, 84, 0, 0, 0 },
        { NOTE_D5, 25, 88, 0, 0, 0 },
        { NOTE_A4, 100, 80, 0, 0, 0 },
        { NOTE_NONE, 0, 90, 0, 0, 0 },
    },

    // 22: Scarecrow
    {
        { NOTE_D4, 3, 0, 0, 0, 0 },
        { NOTE_NONE, 0, 255, 0, 0, 0 },
    },

    // 23: Termina Field 2D Song Buttons Appearing on Wall (In OoT, this is Ocarina Memory Game)
    {
        { NOTE_D4, 3, 0, 0, 0, 0 },
        { NOTE_NONE, 0, 255, 0, 0, 0 },
    },
};

OcarinaNote sOoTOcarinaSongNotes[9][20] = {
    // Minuet
    {
        { NOTE_D4, 18, 86, 0, 0, 0 },
        { NOTE_D5, 18, 92, 0, 0, 0 },
        { NOTE_B4, 72, 86, 0, 0, 0 },
        { NOTE_A4, 18, 80, 0, 0, 0 },
        { NOTE_B4, 18, 88, 0, 0, 0 },
        { NOTE_A4, 144, 86, 0, 0, 0 },
        { NOTE_NONE, 0, 86, 0, 0, 0 },
    },

    // Bolero
    {
        { NOTE_F4, 15, 80, 0, 0, 0 },
        { NOTE_D4, 15, 72, 0, 0, 0 },
        { NOTE_F4, 15, 84, 0, 0, 0 },
        { NOTE_D4, 15, 76, 0, 0, 0 },
        { NOTE_A4, 15, 84, 0, 0, 0 },
        { NOTE_F4, 15, 74, 0, 0, 0 },
        { NOTE_A4, 15, 78, 0, 0, 0 },
        { NOTE_F4, 135, 66, 0, 0, 0 },
        { NOTE_NONE, 0, 66, 0, 0, 0 },
    },

    // Serenade
    {
        { NOTE_D4, 36, 60, 0, 0, 0 },
        { NOTE_F4, 36, 78, 0, 0, 0 },
        { NOTE_A4, 33, 82, 0, 0, 0 },
        { NOTE_NONE, 3, 82, 0, 0, 0 },
        { NOTE_A4, 36, 84, 0, 0, 0 },
        { NOTE_B4, 144, 90, 0, 0, 0 },
        { NOTE_NONE, 0, 90, 0, 0, 0 },
    },

    // Requiem
    {
        { NOTE_D4, 45, 88, 0, 0, 0 },
        { NOTE_F4, 23, 86, 0, 0, 0 },
        { NOTE_D4, 22, 84, 0, 0, 0 },
        { NOTE_A4, 45, 86, 0, 0, 0 },
        { NOTE_F4, 45, 94, 0, 0, 0 },
        { NOTE_D4, 180, 94, 0, 0, 0 },
        { NOTE_NONE, 0, 94, 0, 0, 0 },
    },

    // Nocturne
    {
        { NOTE_B4, 36, 88, 0, 0, 0 },
        { NOTE_A4, 33, 84, 0, 0, 0 },
        { NOTE_NONE, 3, 84, 0, 0, 0 },
        { NOTE_A4, 18, 82, 0, 0, 0 },
        { NOTE_D4, 18, 60, 0, 0, 0 },
        { NOTE_B4, 18, 90, 0, 0, 0 },
        { NOTE_A4, 18, 88, 0, 0, 0 },
        { NOTE_F4, 144, 96, 0, 0, 0 },
        { NOTE_NONE, 0, 96, 0, 0, 0 },
    },

    // Prelude
    {
        { NOTE_D5, 15, 84, 0, 0, 0 },
        { NOTE_A4, 45, 88, 0, 0, 0 },
        { NOTE_D5, 15, 88, 0, 0, 0 },
        { NOTE_A4, 15, 82, 0, 0, 0 },
        { NOTE_B4, 15, 86, 0, 0, 0 },
        { NOTE_D5, 60, 90, 0, 0, 0 },
        { NOTE_NONE, 75, 90, 0, 0, 0 },
        { NOTE_NONE, 0, 90, 0, 0, 0 },
    },

    // Sarias
    {
        { NOTE_F4, 17, 84, 0, 0, 0 },
        { NOTE_A4, 17, 88, 0, 0, 0 },
        { NOTE_B4, 34, 80, 0, 0, 0 },
        { NOTE_F4, 17, 84, 0, 0, 0 },
        { NOTE_A4, 17, 88, 0, 0, 0 },
        { NOTE_B4, 136, 80, 0, 0, 0 },
        { NOTE_NONE, 0, 90, 0, 0, 0 },
    },

    // Zeldas Lullaby
    {
        { NOTE_B4, 51, 84, 0, 0, 0 },
        { NOTE_D5, 25, 88, 0, 0, 0 },
        { NOTE_A4, 78, 80, 0, 0, 0 },
        { NOTE_B4, 51, 84, 0, 0, 0 },
        { NOTE_D5, 25, 88, 0, 0, 0 },
        { NOTE_A4, 100, 80, 0, 0, 0 },
        { NOTE_NONE, 0, 90, 0, 0, 0 },
    },

    // Suns Song
    {
        { NOTE_A4, 12, 84, 0, 0, 0 },
        { NOTE_F4, 13, 88, 0, 0, 0 },
        { NOTE_D5, 29, 80, 2, 0, 0 },
        { NOTE_NONE, 9, 84, 0, 0, 0 },
        { NOTE_A4, 12, 84, 0, 0, 0 },
        { NOTE_F4, 13, 88, 0, 0, 0 },
        { NOTE_D5, 120, 80, 3, 0, 0 },
        { NOTE_NONE, 0, 90, 0, 0, 0 },
    },
};

u8 sOoTOcarinaSongsNumNotes[] = {
    6, // Minuet
    8, // Bolero
    5, // Serenade
    6, // Requiem
    7, // Nocturne
    6, // Prelude
    6, // Sarias
    6, // Zeldas Lullaby
    6, // Suns Song
};

OcarinaNote* sDisplayedSong = sOcarinaSongNotes[OCARINA_SONG_SONATA];
u8 D_801D84F4[14] = { 0, 3, 2, 1, 3, 2, 1, 0, 1, 0, 1, 2, 3, 0 };
u8* gFrogsSongPtr = D_801D84F4;
u8 sRecordingState = OCARINA_RECORD_OFF;
u8 sRecordSongPos = 0;
u32 sOcarinaRecordTaskStart = 0;
u8 sRecordOcarinaNoteIdx = 0;
u8 sRecordOcarinaVolume = 0;
u8 sRecordOcarinaVibrato = 0;
s8 sRecordOcarinaBendIdx = 0;
u8 sRecordOcarinaButtonIdx = 0;
u8 sPlayedOcarinaSongIdxPlusOne = 0;
u8 sMusicStaffNumNotesPerTest = 0;
u8 D_801D8530 = false;
u32 D_801D8534 = 0;
u8 sIsOcarinaNoteChanged = false;

OcarinaNote sScarecrowsLongSongNotes[108] = {
    { NOTE_NONE, 0, 0, 0, 0, 0 },
    { NOTE_NONE, 0, 0, 0, 0, 0 },
};

OcarinaNote* gScarecrowLongSongPtr = sScarecrowsLongSongNotes;
u8* gScarecrowSpawnSongPtr = (u8*)&sOcarinaSongNotes[OCARINA_SONG_SCARECROW];
OcarinaNote* sTerminaWallSongPtr = sOcarinaSongNotes[OCARINA_SONG_TERMINA_WALL];

u8 sNoteToButtonMap[16] = {
    OCARINA_BTN_A,       // NOTE_C4
    OCARINA_BTN_A,       // NOTE_DFLAT4
    OCARINA_BTN_A,       // NOTE_D4
    OCARINA_BTN_A,       // NOTE_EFLAT4
    OCARINA_BTN_C_DOWN,  // NOTE_E4
    OCARINA_BTN_C_DOWN,  // NOTE_F4
    OCARINA_BTN_C_DOWN,  // NOTE_GFLAT4
    OCARINA_BTN_C_RIGHT, // NOTE_G4
    OCARINA_BTN_C_RIGHT, // NOTE_AFLAT4
    OCARINA_BTN_C_RIGHT, // NOTE_A4
    OCARINA_BTN_C_RIGHT + OCARINA_BTN_C_LEFT, // NOTE_BFLAT4: Interface/Overlap between C_RIGHT and C_LEFT
    OCARINA_BTN_C_LEFT, // NOTE_B4
    OCARINA_BTN_C_LEFT, // NOTE_C5
    OCARINA_BTN_C_UP,   // NOTE_DFLAT5
    OCARINA_BTN_C_UP,   // NOTE_D5
    OCARINA_BTN_C_UP,   // NOTE_EFLAT5
};

// New to MM
// seqData written in the music macro language
// Only used in unused functions
u8 sCustomSequenceScript[400] = {
    0xFE, 0xFE, 0xD3, 0x20, 0xD7, 0x00, 0x01, 0xCC, 0x00, 0x70, 0x90, 0x00, 0x16, 0xDB, 0x64, 0xDD, 0x78,
    0xFE, 0x00, 0xF3, 0xFC, 0xFF, 0xC3, 0x88, 0x00, 0x29, 0x89, 0x00, 0x2B, 0xDF, 0x7F, 0xE9, 0x0F, 0xDD,
    0x37, 0xD4, 0x40, 0xC1, 0x52, 0xFE, 0x80, 0xF3, 0xFC, 0xFF, 0xC2, 0xFB, 0xC0, 0x00, 0xC1, 0x57, 0xC9,
};

OcarinaSongButtons gOcarinaSongButtons[OCARINA_SONG_MAX] = {
    // 0: Sonata of Awakening
    {
        7,
        {
            OCARINA_BTN_C_UP,
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_C_UP,
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_A,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_A,
        },
    },

    // 1: Goron Lullaby
    {
        8,
        {
            OCARINA_BTN_A,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_A,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_A,
        },
    },

    // 2: New Wave Bossa Nova
    {
        7,
        {
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_C_UP,
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_DOWN,
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_C_RIGHT,
        },
    },

    // 3: Elegy of Emptyness
    {
        7,
        {
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_DOWN,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_UP,
            OCARINA_BTN_C_LEFT,
        },
    },

    // 4: Oath to Order
    {
        6,
        {
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_DOWN,
            OCARINA_BTN_A,
            OCARINA_BTN_C_DOWN,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_UP,
        },
    },

    // 5; Sarias Song
    {
        6,
        {
            OCARINA_BTN_C_DOWN,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_C_DOWN,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_LEFT,
        },
    },

    // 6: Song of Time
    {
        6,
        {
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_A,
            OCARINA_BTN_C_DOWN,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_A,
            OCARINA_BTN_C_DOWN,
        },
    },

    // 7: Song of Healing
    {
        6,
        {
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_DOWN,
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_DOWN,
        },
    },

    // 8: Eponas Song
    {
        6,
        {
            OCARINA_BTN_C_UP,
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_UP,
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_C_RIGHT,
        },
    },

    // 9: Song of Soaring
    {
        6,
        {
            OCARINA_BTN_C_DOWN,
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_C_UP,
            OCARINA_BTN_C_DOWN,
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_C_UP,
        },
    },

    // 10: Song of Storms
    {
        6,
        {
            OCARINA_BTN_A,
            OCARINA_BTN_C_DOWN,
            OCARINA_BTN_C_UP,
            OCARINA_BTN_A,
            OCARINA_BTN_C_DOWN,
            OCARINA_BTN_C_UP,
        },
    },

    // 11: Suns Song
    {
        6,
        {
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_DOWN,
            OCARINA_BTN_C_UP,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_DOWN,
            OCARINA_BTN_C_UP,
        },
    },

    // 12: Inverted Song of Time
    {
        6,
        {
            OCARINA_BTN_C_DOWN,
            OCARINA_BTN_A,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_DOWN,
            OCARINA_BTN_A,
            OCARINA_BTN_C_RIGHT,
        },
    },

    // 13; Song of Double Time
    {
        6,
        {
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_A,
            OCARINA_BTN_A,
            OCARINA_BTN_C_DOWN,
            OCARINA_BTN_C_DOWN,
        },
    },

    // 14: Goron Lullaby Intro
    {
        6,
        {
            OCARINA_BTN_A,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_A,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_LEFT,
        },
    },

    // 15: Milk Bar Jam "Ballad of the Wind Fish" Human
    {
        4,
        {
            OCARINA_BTN_C_UP,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_C_RIGHT,
        },
    },

    // 16: Milk Bar Jam "Ballad of the Wind Fish" Goron
    {
        8,
        {
            OCARINA_BTN_A,
            OCARINA_BTN_A,
            OCARINA_BTN_A,
            OCARINA_BTN_C_DOWN,
            OCARINA_BTN_A,
            OCARINA_BTN_A,
            OCARINA_BTN_A,
            OCARINA_BTN_C_RIGHT,
        },
    },

    // 17: Milk Bar Jam "Ballad of the Wind Fish" Zora
    {
        8,
        {
            OCARINA_BTN_C_UP,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_DOWN,
            OCARINA_BTN_A,
            OCARINA_BTN_C_UP,
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_DOWN,
        },
    },

    // 18: Milk Bar Jam "Ballad of the Wind Fish" Deku
    {
        5,
        {
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_A,
            OCARINA_BTN_C_DOWN,
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_C_RIGHT,
        },
    },

    // 19: Evan HP (Zora Band Leader) Song Part 1
    {
        8,
        {
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_DOWN,
            OCARINA_BTN_A,
            OCARINA_BTN_A,
            OCARINA_BTN_C_DOWN,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_A,
        },
    },

    // 20: Evan HP (Zora Band Leader) Song Part 2
    {
        8,
        {
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_DOWN,
            OCARINA_BTN_C_DOWN,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_C_DOWN,
        },
    },

    // 21: Zeldas Lullaby
    {
        6,
        {
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_C_UP,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_C_UP,
            OCARINA_BTN_C_RIGHT,
        },
    },

    // 22: Scarecrow
    {
        8,
        {
            OCARINA_BTN_A,
            OCARINA_BTN_A,
            OCARINA_BTN_A,
            OCARINA_BTN_A,
            OCARINA_BTN_A,
            OCARINA_BTN_A,
            OCARINA_BTN_A,
            OCARINA_BTN_A,
        },
    },

    // 23: Termina Field 2D Song Buttons Appearing on Wall (In OoT, this is Ocarina Memory Game)
    {
        8,
        {
            OCARINA_BTN_A,
            OCARINA_BTN_C_DOWN,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_C_UP,
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_DOWN,
        },
    },
};

// OoT's soundEffects from EnRiverSound
const u16 gAudioEnvironmentalSfx[] = {
    NA_SE_EV_RIVER_STREAM - SFX_FLAG,
    NA_SE_EV_WAVE - SFX_FLAG,
    NA_SE_EV_WATER_WALL_BIG - SFX_FLAG,
    NA_SE_EV_WATER_WALL - SFX_FLAG,
    NA_SE_EV_MAGMA_LEVEL - SFX_FLAG,
    NA_SE_EV_MAGMA_LEVEL_M - SFX_FLAG,
    NA_SE_EV_MAGMA_LEVEL_L - SFX_FLAG,
    NA_SE_EV_TORCH - SFX_FLAG,
    NA_SE_EV_FOUNTAIN - SFX_FLAG,
    NA_SE_EV_DRAIN - SFX_FLAG,
    NA_SE_EV_CROWD - SFX_FLAG,
    NA_SE_EV_WATER_CONVECTION - SFX_FLAG,
    NA_SE_EV_GORON_CHEER - SFX_FLAG,
    NA_SE_EV_WATER_WALL_BIG_SILENT - SFX_FLAG,
    NA_SE_EV_WATER_BUBBLE - SFX_FLAG,
    NA_SE_EV_COW_CRY_LV - SFX_FLAG,
    NA_SE_EV_MAKE_TURRET - SFX_FLAG,
    NA_SE_EV_BOILED_WATER_S - SFX_FLAG,
    NA_SE_EV_BOILED_WATER_L - SFX_FLAG,
    NA_SE_EV_WAVE_S - SFX_FLAG,
    NA_SE_EV_WAVE_S - SFX_FLAG,
    NA_SE_EV_WAVE_S - SFX_FLAG,
};

const u8 sIsOcarinaSongReserved[OCARINA_SONG_MAX] = {
    true,  // OCARINA_SONG_SONATA
    true,  // OCARINA_SONG_GORON_LULLABY
    true,  // OCARINA_SONG_NEW_WAVE
    true,  // OCARINA_SONG_ELEGY
    true,  // OCARINA_SONG_OATH
    false, // OCARINA_SONG_SARIAS
    true,  // OCARINA_SONG_TIME
    true,  // OCARINA_SONG_HEALING
    true,  // OCARINA_SONG_EPONAS
    true,  // OCARINA_SONG_SOARING
    true,  // OCARINA_SONG_STORMS
    false, // OCARINA_SONG_SUNS
    true,  // OCARINA_SONG_INVERTED_TIME
    true,  // OCARINA_SONG_DOUBLE_TIME
    true,  // OCARINA_SONG_GORON_LULLABY_INTRO
    false, // OCARINA_SONG_WIND_FISH_HUMAN
    false, // OCARINA_SONG_WIND_FISH_GORON
    false, // OCARINA_SONG_WIND_FISH_ZORA
    false, // OCARINA_SONG_WIND_FISH_DEKU
    true,  // OCARINA_SONG_EVAN_PART1
    true,  // OCARINA_SONG_EVAN_PART2
    false, // OCARINA_SONG_ZELDAS_LULLABY
    true,  // OCARINA_SONG_SCARECROW
    false, // OCARINA_SONG_TERMINA_WALL
};

const char sAudioOcarinaUnusedText0[] = "key step is too long !!! %d:%d>%d\n";
const char sAudioOcarinaUnusedText1[] = "You played %d Melody !!! (last step:%d)\n";
const char sAudioOcarinaUnusedText2[] = "pass 0\n";
const char sAudioOcarinaUnusedText3[] = "pass 1\n";
const char sAudioOcarinaUnusedText4[] = "pass 2\n";
const char sAudioOcarinaUnusedText5[] = "last key is bad !!! %d %d %02X %02X\n";
const char sAudioOcarinaUnusedText6[] = "last key step is too short !!! %d:%d %d<%d\n";
const char sAudioOcarinaUnusedText7[] = "check is over!!! %d %d %d\n";

void AudioOcarina_ReadControllerInput(void) {
    Input inputs[4];
    Input* input = &inputs[0];
    u32 ocarinaInputButtonPrev = sOcarinaInputButtonCur;

    Padmgr_GetInput2(inputs, 0);
    sOcarinaInputButtonCur = input->cur.button;
    sOcarinaInputButtonPrev = ocarinaInputButtonPrev;
    sOcarinaInputStickRel.x = input->rel.stick_x;
    sOcarinaInputStickRel.y = input->rel.stick_y;
}

/**
 * Looks up the pitch frequency to bend the note by.
 * The note will bend up to a maximum of 2 semitones
 * in each direction giving a total range of 4 semitones
 */
f32 AudioOcarina_BendPitchTwoSemitones(s8 bendIdx) {
    s8 adjBendIdx;
    f32 bendFreq;

    if (bendIdx > 64) {
        adjBendIdx = 127;
    } else if (bendIdx < -64) {
        adjBendIdx = -128;
    } else if (bendIdx >= 0) {
        adjBendIdx = (bendIdx * 127) / 64;
    } else {
        adjBendIdx = (bendIdx * 128) / 64;
    }

    /**
     * index 128 is in the middle of the table and
     * contains the value 1.0f i.e. no bend
     * absolute indices above 128 will bend the pitch 2 semitones upwards
     * absolute indices below 128 will bend the pitch 2 semitones downwards
     */
    bendFreq = gBendPitchTwoSemitonesFrequencies[adjBendIdx + 128];
    return bendFreq;
}

/**
 * If an available song has been played, then return that song index
 * If the ocarina is on, but no song has been played then return 0xFE
 * If the ocarina is off, return 0xFF
 */
u8 AudioOcarina_GetPlayingState(void) {
    u8 playedOcarinaSongIdx;

    if (sPlayedOcarinaSongIdxPlusOne != 0) {
        playedOcarinaSongIdx = sPlayedOcarinaSongIdxPlusOne - 1;
        sPlayedOcarinaSongIdxPlusOne = 0;
    } else if (sOcarinaFlags != 0) {
        playedOcarinaSongIdx = 0xFE;
    } else {
        playedOcarinaSongIdx = 0xFF;
    }

    return playedOcarinaSongIdx;
}

u8 AudioOcarina_MapNoteToButton(u8 noteIdx) {
    u8 buttonIdx = sNoteToButtonMap[noteIdx & 0x3F];

    /**
     * Special case for BFlat4:
     * CRIGHT and CLEFT are the only two notes that are 2 semitones apart
     * which are notes A4 and B4 respectively
     * BFlat4 is in the middle of those two and is the only note that can not
     * be resolved between the two buttons without external information.
     * That information is stored as flags in noteIdx with the mask:
     * (noteIdx & 0xC0)
     */
    if (buttonIdx == (OCARINA_BTN_C_RIGHT + OCARINA_BTN_C_LEFT)) {
        if (noteIdx & 0x80) {
            return OCARINA_BTN_C_RIGHT;
        }
        return OCARINA_BTN_C_LEFT;
    }

    return buttonIdx;
}

void AudioOcarina_MapSongFromNotesToButtons(u8 noteSongIdx, u8 buttonSongIdx, u8 numButtons) {
    u8 buttonSongPos = 0;
    u8 noteSongPos = 0;
    u8 noteIdx;

    while (buttonSongPos < numButtons && noteSongPos < 16) {
        noteIdx = sOcarinaSongNotes[noteSongIdx][noteSongPos++].noteIdx;

        if (noteIdx != NOTE_NONE) {
            gOcarinaSongButtons[buttonSongIdx].buttonIdx[buttonSongPos++] = sNoteToButtonMap[noteIdx];
        }
    }

    gOcarinaSongButtons[buttonSongIdx].numButtons = numButtons;
}

// The ocarina songs that can be performed at any time = 0x3FFF
#define OCARINA_SONGS_PLAYABLE_FLAGS ( \
    (1 << OCARINA_SONG_SONATA) | \
    (1 << OCARINA_SONG_GORON_LULLABY) | \
    (1 << OCARINA_SONG_NEW_WAVE) | \
    (1 << OCARINA_SONG_ELEGY) | \
    (1 << OCARINA_SONG_OATH) | \
    (1 << OCARINA_SONG_SARIAS) | \
    (1 << OCARINA_SONG_TIME) | \
    (1 << OCARINA_SONG_HEALING) | \
    (1 << OCARINA_SONG_EPONAS) | \
    (1 << OCARINA_SONG_SOARING) | \
    (1 << OCARINA_SONG_STORMS) | \
    (1 << OCARINA_SONG_SUNS) | \
    (1 << OCARINA_SONG_INVERTED_TIME) | \
    (1 << OCARINA_SONG_DOUBLE_TIME))

void AudioOcarina_Start(u32 ocarinaFlags) {
    u8 songIdx;

    if ((sOcarinaSongNotes[OCARINA_SONG_SCARECROW][1].volume != 0xFF) && ((ocarinaFlags & OCARINA_SONGS_PLAYABLE_FLAGS) == OCARINA_SONGS_PLAYABLE_FLAGS)) {
        ocarinaFlags |= (1 << OCARINA_SONG_SCARECROW);
    }

    if ((ocarinaFlags == (0xC0000000 | OCARINA_SONGS_PLAYABLE_FLAGS)) && (sOcarinaSongNotes[OCARINA_SONG_SCARECROW][1].volume != 0xFF)) {
        ocarinaFlags = (0xC0000000 | OCARINA_SONGS_PLAYABLE_FLAGS);
    }

    if ((ocarinaFlags == OCARINA_SONGS_PLAYABLE_FLAGS) && (sOcarinaSongNotes[OCARINA_SONG_SCARECROW][1].volume != 0xFF)) {
        ocarinaFlags = OCARINA_SONGS_PLAYABLE_FLAGS;
    }

    if (ocarinaFlags != 0xFFFF) {
        sOcarinaFlags = 0x80000000 + (u32)ocarinaFlags;
        sFirstOcarinaSongIdx = OCARINA_SONG_SONATA;
        sLastOcarinaSongIdx = OCARINA_SONG_MAX;
        sOcarinaAvailSongs = ocarinaFlags & 0xFFFFFF;
        sMusicStaffNumNotesPerTest = 8; // Ocarina Check
        sOcarinaHasStartedSong = false;
        sPlayedOcarinaSongIdxPlusOne = 0;
        sOcarinaStaffPlayingPos = 0;
        sPlayingStaff.state = AudioOcarina_GetPlayingState();
        sIsOcarinaInputEnabled = true;
        sPrevOcarinaSongFlags = 0;

        // Reset music staff song check
        for (songIdx = OCARINA_SONG_SONATA; songIdx <= OCARINA_SONG_EVAN_PART2; songIdx++) {
            for (sMusicStaffPos[songIdx] = 0; sOcarinaSongNotes[songIdx][sMusicStaffPos[songIdx]].noteIdx == NOTE_NONE;) {
                sMusicStaffPos[songIdx]++;
            }

            sMusicStaffCurHeldLength[songIdx] = 0;
            sMusicStaffExpectedLength[songIdx] = 0;
            sMusicStaffExpectedNoteIdx[songIdx] = 0;

            if (D_801D8530) {
                if ((1 << songIdx) & ocarinaFlags) {
                    D_801FD518[songIdx] = 0;
                } else {
                    D_801FD518[songIdx] = 0xFF;
                }
            }
        }

        if (ocarinaFlags & 0x80000000) {
            sMusicStaffNumNotesPerTest = 0; // Ocarina Playback
        }

        if (ocarinaFlags & 0x40000000) {
            sOcarinaSongAppendPos = 0;
        }

        if (ocarinaFlags & 0xC0400000) {
            AudioOcarina_MapSongFromNotesToButtons(OCARINA_SONG_SCARECROW, OCARINA_SONG_SCARECROW, 8);
        }
    } else {
        sOcarinaFlags = 0;
        sIsOcarinaInputEnabled = false;
    }
}

#undef OCARINA_SONGS_PLAYABLE_FLAGS

void AudioOcarina_SetSongStartingPos(void) {
    sOcarinaSongStartingPos = sOcarinaSongAppendPos;
}

void AudioOcarina_StartAtSongStartingPos(u32 ocarinaFlags) {
    D_801D8530 = false;
    AudioOcarina_Start(ocarinaFlags);
    if (sOcarinaSongStartingPos != 0) {
        sOcarinaSongAppendPos = sOcarinaSongStartingPos;
        sOcarinaSongStartingPos = 0;
    }
}

void AudioOcarina_StartForSongCheck(u32 ocarinaFlags, u8 arg1) {
    u8 i;
    u8 j;

    AudioOcarina_Start(ocarinaFlags);
    sMusicStaffNumNotesPerTest = 8;
    sOcarinaStaffPlayingPos = arg1;

    for (i = 0; i <= OCARINA_SONG_EVAN_PART2; i++) {
        for (j = 0; j < arg1;) {
            if (sOcarinaSongNotes[i][sMusicStaffPos[i]].noteIdx != NOTE_NONE) {
                j++;
            }
            sMusicStaffPos[i]++;
        }

        if (sOcarinaSongNotes[i][sMusicStaffPos[i]].noteIdx == NOTE_NONE) {
            sMusicStaffPos[i]++;
        }
    }
}

void AudioOcarina_StartWithSongNoteLengths(u32 ocarinaFlags) {
    u8 songIdx;

    for (songIdx = OCARINA_SONG_SONATA; songIdx < OCARINA_SONG_MAX; songIdx++) {
        if ((1 << songIdx) & ocarinaFlags) {
            D_801FD530[songIdx] = 0;
        } else {
            D_801FD530[songIdx] = 0xFF;
        }
    }

    D_801D8530 = true;
    D_801D8534 = ocarinaFlags;
    AudioOcarina_Start(ocarinaFlags);
}

void AudioOcarina_StartDefault(u32 ocarinaFlags) {
    D_801D8530 = false;
    AudioOcarina_Start(ocarinaFlags);
}

u8 func_8019B568(void) {
    u32 temp_v0 = D_801D8534;
    u8 songIdx;

    for (songIdx = OCARINA_SONG_SONATA; !((temp_v0 >> songIdx) & 1) && (songIdx < OCARINA_SONG_MAX); songIdx++) {}

    return songIdx;
}

u8 func_8019B5AC(void) {
    u8 songIdx = func_8019B568();

    if (songIdx < OCARINA_SONG_MAX) {
        return D_801FD518[songIdx];
    }

    return 0xFF;
}

u8 func_8019B5EC(void) {
    return D_801FD530[func_8019B568()];
}

void AudioOcarina_CheckIfStartedSong(void) {
    if (sCurOcarinaNoteIdx != NOTE_NONE && !sOcarinaHasStartedSong) {
        sOcarinaHasStartedSong = true;
        sMusicStaffPrevNoteIdx = NOTE_NONE;
    }
}

void AudioOcarina_UpdateCurOcarinaSong(void) {
    u8 i;

    if (sOcarinaSongAppendPos == 8) {
        for (i = 0; i < ARRAY_COUNT(sCurOcarinaSong) - 1; i++) {
            sCurOcarinaSong[i] = sCurOcarinaSong[i + 1];
        }
    } else {
        sOcarinaSongAppendPos++;
    }
}

void AudioOcarina_CheckSongsWithMusicStaff(void) {
    OcarinaNote* prevNote;
    u32 curOcarinaSongFlag;
    u8 inputChanged = false;
    u8 phi_v0;
    u8 songIdx;
    s8 pad;
    s8 sp5B = 0;
    u16* temp_t0;
    OcarinaNote* note;

    AudioOcarina_CheckIfStartedSong();

    if (!sOcarinaHasStartedSong) {
        return;
    }

    if (ABS_ALT(sCurOcarinaBendIdx) > 20) {
        sOcarinaFlags = 0;
        for (songIdx = OCARINA_SONG_SONATA; songIdx < OCARINA_SONG_MAX; songIdx++) {
            D_801FD518[songIdx] = 4;
        }
        sIsOcarinaInputEnabled = false;
        return;
    }

    if ((sPrevOcarinaNoteIdx == sCurOcarinaNoteIdx) || (sCurOcarinaNoteIdx == NOTE_NONE)) {
        inputChanged = true;
    } else {
        AudioOcarina_UpdateCurOcarinaSong();
        sCurOcarinaSong[sOcarinaSongAppendPos - 1] = sCurOcarinaNoteIdx;
    }

    for (songIdx = sFirstOcarinaSongIdx; songIdx < sLastOcarinaSongIdx; songIdx++) {
        curOcarinaSongFlag = 1 << songIdx;

        if (sOcarinaAvailSongs & curOcarinaSongFlag) {
            if (D_801D8530) {
                sMusicStaffCurHeldLength[songIdx] += sOcarinaUpdateTaskCurrent - sOcarinaPlaybackTaskStart;
            } else {
                sMusicStaffCurHeldLength[songIdx] = sMusicStaffExpectedLength[songIdx] + 9;
            }

            if (inputChanged) {
                if ((sMusicStaffCurHeldLength[songIdx] > 8) && (sMusicStaffPrevNoteIdx != sMusicStaffExpectedNoteIdx[songIdx])) {
                    sOcarinaAvailSongs ^= curOcarinaSongFlag;
                    if (D_801D8530) {
                        D_801FD518[songIdx] = 1;
                    }
                }

                if (D_801D8530 && ((sMusicStaffExpectedLength[songIdx] + 9) < sMusicStaffCurHeldLength[songIdx])) {
                    sOcarinaAvailSongs ^= curOcarinaSongFlag;
                    D_801FD518[songIdx] = 3;
                } else if ((sMusicStaffCurHeldLength[songIdx] >= (sMusicStaffExpectedLength[songIdx] - 9)) &&
                           (sMusicStaffCurHeldLength[songIdx] >= sMusicStaffExpectedLength[songIdx]) &&
                           (sOcarinaSongNotes[songIdx][sMusicStaffPos[songIdx]].length == 0) &&
                           (sMusicStaffPrevNoteIdx == sMusicStaffExpectedNoteIdx[songIdx])) {
                    sPlayedOcarinaSongIdxPlusOne = songIdx + 1;
                    sIsOcarinaInputEnabled = false;
                    sOcarinaFlags = 0;
                    if (D_801D8530) {
                        D_801FD518[songIdx] = 5;
                    }
                }
            } else {
                if (sMusicStaffCurHeldLength[songIdx] >= (sMusicStaffExpectedLength[songIdx] - 9)) {
                    if (sMusicStaffPrevNoteIdx != NOTE_NONE) {
                        if (sMusicStaffPrevNoteIdx == sMusicStaffExpectedNoteIdx[songIdx]) {
                            if (D_801D8530) {
                                D_801FD530[songIdx] += ABS_ALT(sMusicStaffExpectedLength[songIdx] - sMusicStaffCurHeldLength[songIdx]);
                            }
                            sMusicStaffCurHeldLength[songIdx] = 0;
                        } else {
                            sOcarinaAvailSongs ^= curOcarinaSongFlag;
                            if (D_801D8530) {
                                D_801FD518[songIdx] = 1;
                            }
                        }
                    }

                    prevNote = &sOcarinaSongNotes[songIdx][sMusicStaffPos[songIdx]];
                    note = &sOcarinaSongNotes[songIdx][++sMusicStaffPos[songIdx]];
                    sMusicStaffExpectedLength[songIdx] = prevNote->length;
                    sMusicStaffExpectedNoteIdx[songIdx] = prevNote->noteIdx;

                    while (prevNote->noteIdx == note->noteIdx || (note->noteIdx == NOTE_NONE && note->length != 0)) {
                        sMusicStaffExpectedLength[songIdx] += note->length;
                        prevNote = &sOcarinaSongNotes[songIdx][sMusicStaffPos[songIdx]];
                        note = &sOcarinaSongNotes[songIdx][sMusicStaffPos[songIdx] + 1];
                        sMusicStaffPos[songIdx]++;
                    }

                } else if (sMusicStaffCurHeldLength[songIdx] < 9) {
                    sp5B = -1;
                    sMusicStaffCurHeldLength[songIdx] = 0;
                    sMusicStaffPrevNoteIdx = sCurOcarinaNoteIdx;
                } else {
                    if (sOcarinaSongNotes[songIdx][sMusicStaffPos[songIdx]].length == 0) {
                        D_801FD518[songIdx] = 1;
                    } else if (D_801D8530) {
                        D_801FD518[songIdx] = 2;
                    }
                    sOcarinaAvailSongs ^= curOcarinaSongFlag;
                }
            }
        }
        if ((sOcarinaAvailSongs == 0) && (sOcarinaStaffPlayingPos >= sMusicStaffNumNotesPerTest)) {
            sIsOcarinaInputEnabled = false;
            if (!D_801D8530) {
                if ((sOcarinaFlags & 0x40000000) && (sCurOcarinaNoteIdx == sOcarinaSongNotes[songIdx][0].noteIdx)) {
                    sPrevOcarinaSongFlags = sOcarinaFlags;
                }
            }
            sOcarinaFlags = 0;
            return;
        }
    }

    if (!inputChanged) {
        sMusicStaffPrevNoteIdx = sCurOcarinaNoteIdx;
        sOcarinaStaffPlayingPos += sp5B + 1;
    }
}

/**
 * Checks for ocarina songs from user input with no music staff prompt.
 * Includes ocarina actions such as free play, no warp
 */
void AudioOcarina_CheckSongsWithoutMusicStaff(void) {
    u32 noteIdx;
    u8 temp_v0_2;
    u8 songIdx;
    u8 j;
    u8 k;

    if (CHECK_BTN_ANY(sOcarinaInputButtonCur, BTN_L) &&
        CHECK_BTN_ANY(sOcarinaInputButtonCur, BTN_A | BTN_CRIGHT | BTN_CLEFT | BTN_CDOWN | BTN_CUP)) {
        AudioOcarina_StartDefault(sOcarinaFlags);
        return;
    }

    AudioOcarina_CheckIfStartedSong();

    if (!sOcarinaHasStartedSong) {
        return;
    }

    temp_v0_2 = sOcarinaStaffPlayingPos;
    if ((sPrevOcarinaNoteIdx != sCurOcarinaNoteIdx) && (sCurOcarinaNoteIdx != NOTE_NONE)) {
        sOcarinaStaffPlayingPos++;
        if (sOcarinaStaffPlayingPos > 8) {
            sOcarinaStaffPlayingPos = 1;
        }

        AudioOcarina_UpdateCurOcarinaSong();

        if ((ABS_ALT(sCurOcarinaBendIdx) > 20) && (temp_v0_2 != sOcarinaStaffPlayingPos)) {
            sCurOcarinaSong[sOcarinaSongAppendPos - 1] = NOTE_NONE;
        } else {
            sCurOcarinaSong[sOcarinaSongAppendPos - 1] = sCurOcarinaNoteIdx;
        }

        // Tests to see if the notes from the ocarina are identical to any of the songIdx from 0 to

        // Loop through each of the songs
        for (songIdx = sFirstOcarinaSongIdx; songIdx < sLastOcarinaSongIdx; songIdx++) {
            // Checks to see if the song is available to be played
            if ((u32)sOcarinaAvailSongs & (1 << songIdx)) {
                // Loops through all possible starting indices?
                // Loops through the notes of the song?
                for (j = 0, k = 0; j < gOcarinaSongButtons[songIdx].numButtons && k == 0 &&
                                    sOcarinaSongAppendPos >= gOcarinaSongButtons[songIdx].numButtons;) {

                    noteIdx = sCurOcarinaSong[(sOcarinaSongAppendPos - gOcarinaSongButtons[songIdx].numButtons) + j];

                    if (noteIdx == sButtonToNoteMap[gOcarinaSongButtons[songIdx].buttonIdx[j]]) {
                        j++;
                    } else {
                        k++;
                    }
                }

                // This conditional is true if songIdx = i is detected
                if (j == gOcarinaSongButtons[songIdx].numButtons) {
                    sPlayedOcarinaSongIdxPlusOne = songIdx + 1;
                    sIsOcarinaInputEnabled = false;
                    sOcarinaFlags = 0;
                }
            }
        }
    }
}

void AudioOcarina_PlayControllerInput(u8 isOcarinaSfxSuppressedWhenCancelled) {
    u32 ocarinaBtnsHeld;

    // Prevents two different ocarina notes from being played on two consecutive frames
    if (sOcarinaFlags != 0 && sIsOcarinaNoteChanged) {
        sIsOcarinaNoteChanged--;
        return;
    }

    // Ensures the button pressed to start the ocarina does not also play an ocarina note
    if ((sOcarinaInputButtonStart == 0) || ((sOcarinaInputButtonStart & (BTN_A | BTN_CRIGHT | BTN_CLEFT | BTN_CDOWN | BTN_CUP)) !=
                              (sOcarinaInputButtonCur & (BTN_A | BTN_CRIGHT | BTN_CLEFT | BTN_CDOWN | BTN_CUP)))) {
        sOcarinaInputButtonStart = 0;
        if (1) {}
        sCurOcarinaNoteIdx = NOTE_NONE;
        sCurOcarinaButtonIdx = OCARINA_BTN_INVALID;
        ocarinaBtnsHeld = (sOcarinaInputButtonCur & (BTN_A | BTN_CRIGHT | BTN_CLEFT | BTN_CDOWN | BTN_CUP)) &
                          (sOcarinaInputButtonPrev & (BTN_A | BTN_CRIGHT | BTN_CLEFT | BTN_CDOWN | BTN_CUP));

        if (!(sOcaInputBtnPress & ocarinaBtnsHeld) && (sOcarinaInputButtonCur != 0)) {
            sOcaInputBtnPress = sOcarinaInputButtonCur;
        } else {
            sOcaInputBtnPress &= ocarinaBtnsHeld;
        }

        // Interprets and transforms controller input into ocarina buttons and notes
        if (sOcaInputBtnPress & BTN_A) {
            sCurOcarinaNoteIdx = NOTE_D4;
            sCurOcarinaButtonIdx = OCARINA_BTN_A;

        } else if (sOcaInputBtnPress & BTN_CDOWN) {
            sCurOcarinaNoteIdx = NOTE_F4;
            sCurOcarinaButtonIdx = OCARINA_BTN_C_DOWN;

        } else if (sOcaInputBtnPress & BTN_CRIGHT) {
            sCurOcarinaNoteIdx = NOTE_A4;
            sCurOcarinaButtonIdx = OCARINA_BTN_C_RIGHT;

        } else if (sOcaInputBtnPress & BTN_CLEFT) {
            sCurOcarinaNoteIdx = NOTE_B4;
            sCurOcarinaButtonIdx = OCARINA_BTN_C_LEFT;

        } else if (sOcaInputBtnPress & BTN_CUP) {
            sCurOcarinaNoteIdx = NOTE_D5;
            sCurOcarinaButtonIdx = OCARINA_BTN_C_UP;
        }

        if (sOcarinaInputButtonCur) {}

        // Pressing the R Button will raise the pitch by 1 semitone
        if ((sCurOcarinaNoteIdx != NOTE_NONE) && (sOcarinaInputButtonCur & BTN_R) && (sRecordingState != OCARINA_RECORD_SCARECROW_SPAWN)) {
            sCurOcarinaButtonIdx += 0x80; // Flag to resolve B Flat 4
            sCurOcarinaNoteIdx++;         // Raise the pitch by 1 semitone
        }

        // Pressing the Z Button will lower the pitch by 1 semitone
        if ((sCurOcarinaNoteIdx != NOTE_NONE) && (sOcarinaInputButtonCur & BTN_Z) && (sRecordingState != OCARINA_RECORD_SCARECROW_SPAWN)) {
            sCurOcarinaButtonIdx += 0x40; // Flag to resolve B Flat 4
            sCurOcarinaNoteIdx--;         // Lower the pitch by 1 semitone
        }

        if (sRecordingState != OCARINA_RECORD_SCARECROW_SPAWN) {
            // Bend the pitch of the note based on y control stick
            sCurOcarinaBendIdx = sOcarinaInputStickRel.y;
            sCurOcarinaBendFreq = AudioOcarina_BendPitchTwoSemitones(sCurOcarinaBendIdx);

            // Add vibrato of the ocarina note based on the x control stick
            sCurOcarinaVibrato = ABS_ALT(sOcarinaInputStickRel.x) >> 2;
            Audio_QueueCmdS8(6 << 24 | 2 << 16 | SFX_CHANNEL_OCARINA << 8 | 6, sCurOcarinaVibrato);
        } else {
            sCurOcarinaBendIdx = 0;
            sCurOcarinaVibrato = 0;
            sCurOcarinaBendFreq = 1.0f;
        }

        if ((sCurOcarinaNoteIdx != NOTE_NONE) && (sPrevOcarinaNoteIdx != sCurOcarinaNoteIdx)) {
            Audio_QueueCmdS8(6 << 24 | 2 << 16 | SFX_CHANNEL_OCARINA << 8 | 7, sOcarinaInstrumentId - 1);
            Audio_QueueCmdS8(6 << 24 | 2 << 16 | SFX_CHANNEL_OCARINA << 8 | 5, sCurOcarinaNoteIdx);
            Audio_PlaySfxGeneral(NA_SE_OC_OCARINA, &gDefaultSfxPos, 4, &sCurOcarinaBendFreq, &sDefaultOcarinaVolume,
                                 &gDefaultSfxReverbAdd);
        } else if ((sPrevOcarinaNoteIdx != NOTE_NONE) && (sCurOcarinaNoteIdx == NOTE_NONE) && !isOcarinaSfxSuppressedWhenCancelled) {
            Audio_StopSfxById(NA_SE_OC_OCARINA);
        }
    }
}

/**
 * Directly enable the ocarina to receive input without
 * properly resetting it based on an ocarina instrument id
 */
void AudioOcarina_EnableInput(u8 isEnabled) {
    sIsOcarinaInputEnabled = isEnabled;
}

/**
 * Resets ocarina properties
 */
void AudioOcarina_ResetAndMute(void) {
    sOcarinaInputButtonCur = 0;
    sOcarinaInputButtonPrev = 0;
    sOcaInputBtnPress = 0;

    sOcarinaInputButtonStart = 0xFFFF;

    AudioOcarina_PlayControllerInput(false);
    Audio_StopSfxById(NA_SE_OC_OCARINA);

    if (gAudioSpecId != 0xC) {
        Audio_SetSfxBanksMute(0);
    }

    sDisplayedState = 0;
    sDisplayedStaffPos = 0;
    sIsOcarinaInputEnabled = false;

    sOcarinaFlags = 0;

    // return to full volume for both bgm players after ocarina is finished
    Audio_ClearFlagForBgmVolumeLow(SFX_CHANNEL_OCARINA);
    // return to full volume for all sfx channels except system & ocarina banks
    Audio_SetSfxVolumeExceptSystemAndOcarinaBanks(0x7F);
}

void AudioOcarina_ResetAndReadInput(void) {
    sOcarinaInputButtonCur = 0;
    sOcarinaInputButtonPrev = 0;
    sOcaInputBtnPress = 0;

    sOcarinaInputButtonStart = 0xFFFF;

    AudioOcarina_PlayControllerInput(true);

    sDisplayedState = 0;
    sDisplayedStaffPos = 0;
    sIsOcarinaInputEnabled = false;

    sOcarinaFlags = 0;

    sOcarinaInputButtonCur = 0;
    
    AudioOcarina_ReadControllerInput();
    sOcarinaInputButtonStart = sOcarinaInputButtonCur;
}

void AudioOcarina_SetResetDelay(u8 unused, u8 resetDelay) {
    sOcarinaResetDelay = resetDelay;
    sOcarinaResetUnused = unused;
}

u32 AudioOcarina_SetInstrumentId(u8 ocarinaInstrumentId) {
    if (sOcarinaInstrumentId != ocarinaInstrumentId || ocarinaInstrumentId == 1) {
        AudioSeqCmd_SetChannelIO(SEQ_PLAYER_SFX, 1, SFX_CHANNEL_OCARINA, ocarinaInstrumentId);
        sOcarinaInstrumentId = ocarinaInstrumentId;

        if (ocarinaInstrumentId == OCARINA_INSTRUMENT_OFF) {
            AudioOcarina_ResetAndMute();
        } else {
            sOcarinaInputButtonCur = 0;
            AudioOcarina_ReadControllerInput();
            sOcarinaInputButtonStart = sOcarinaInputButtonCur;
            // lowers volume of all sfx channels except system & ocarina banks
            Audio_SetSfxVolumeExceptSystemAndOcarinaBanks(0x40);
            // lowers volume of bgm players while playing ocarina
            Audio_SetFlagForBgmVolumeLow(SFX_CHANNEL_OCARINA);
        }
    }
}

void AudioOcarina_SetDisplayedSong(s8 songIdxPlusOne, u8 displayedState) {
    u8 i = 0;
    s32 phi_a3;

    if (songIdxPlusOne == 0) {
        sDisplayedState = 0;
        Audio_StopSfxById(NA_SE_OC_OCARINA);
        return;
    }

    if (songIdxPlusOne < (OCARINA_SONG_MAX + 1)) {
        sDisplayedSong = sOcarinaSongNotes[songIdxPlusOne - 1];
    } else {
        sDisplayedSong = sScarecrowsLongSongNotes;
    }

    sDisplayedNotePos = 0;
    if (displayedState & 0x80) {
        sDisplayedState = 1;
        D_801D6FE4 = displayedState & 0xF;
        D_801D6FE8 = 0xFF;
        phi_a3 = D_801D6FE8;
    } else if (displayedState & 0x40) {
        sDisplayedState = 1;
        D_801D6FE8 = displayedState & 0xF;
        D_801D6FE4 = 0xFF;
        phi_a3 = D_801D6FE8;
        for (; i < phi_a3;) {
            if (sOcarinaSongNotes[songIdxPlusOne - 1][sDisplayedNotePos].noteIdx != NOTE_NONE) {
                i++;
            }
            sDisplayedNotePos++;
        }
    } else {
        sDisplayedState = displayedState;
        D_801D6FE4 = 0xFF;
        D_801D6FE8 = 0xFF;
        phi_a3 = D_801D6FE8;
    }

    sDisplayedNoteTimer = 0;
    sDisplayedNoteValue = NOTE_NONE;
    sDisplayedStaffPos = 0;

    if ((phi_a3 ^ 0) == 0xFF) {
        while (sDisplayedSong[sDisplayedNotePos].noteIdx == NOTE_NONE && songIdxPlusOne > (OCARINA_SONG_EVAN_PART2 + 1)) {
            sDisplayedNotePos++;
        }
    }
}

void AudioOcarina_PlayDisplayedSong(void) {
    s32 noteTimerStep;
    u32 nextNoteTimerStep;

    if (sDisplayedState == 0) {
        return;
    }

    if (sDisplayedStaffPos == 0) {
        noteTimerStep = 3;
    } else {
        noteTimerStep = sOcarinaUpdateTaskCurrent - sOcarinaPlaybackTaskStart;
    }

    if (noteTimerStep < sDisplayedNoteTimer) {
        sDisplayedNoteTimer -= noteTimerStep;
    } else {
        nextNoteTimerStep = noteTimerStep - sDisplayedNoteTimer;
        sDisplayedNoteTimer = 0;
    }

    if (sDisplayedNoteTimer == 0) {

        if (sDisplayedStaffPos == D_801D6FE4) {
            sDisplayedState = 0;
            return;
        }

        sDisplayedNoteTimer = sDisplayedSong[sDisplayedNotePos].length;

        if (sDisplayedNotePos == 1) {
            sDisplayedNoteTimer++;
        }

        if (sDisplayedNoteTimer == 0) {
            sDisplayedState--;
            if (sDisplayedState != 0) {
                sDisplayedNotePos = 0;
                sDisplayedStaffPos = 0;
                sDisplayedNoteValue = NOTE_NONE;
            } else {
                Audio_StopSfxById(NA_SE_OC_OCARINA);
            }
            return;
        } else {
            sDisplayedNoteTimer -= nextNoteTimerStep;
        }

        // Update volume
        if (sNotePlaybackVolume != sDisplayedSong[sDisplayedNotePos].volume) {
            sNotePlaybackVolume = sDisplayedSong[sDisplayedNotePos].volume;
            sNormalizedNotePlaybackVolume = sNotePlaybackVolume / 127.0f;
        }

        // Update vibrato
        sNotePlaybackVibrato = sDisplayedSong[sDisplayedNotePos].vibrato;
        Audio_QueueCmdS8(6 << 24 | 2 << 16 | SFX_CHANNEL_OCARINA << 8 | 6, sNotePlaybackVibrato);

        // Update bend
        if (sNotePlaybackBend != sDisplayedSong[sDisplayedNotePos].bend) {
            sNotePlaybackBend = sDisplayedSong[sDisplayedNotePos].bend;
            sNormalizedNotePlaybackTone = AudioOcarina_BendPitchTwoSemitones(sNotePlaybackBend);
        }

        // No changes in volume, vibrato, or bend between notes
        if ((sDisplayedSong[sDisplayedNotePos].volume == sDisplayedSong[sDisplayedNotePos - 1].volume &&
                (sDisplayedSong[sDisplayedNotePos].vibrato == sDisplayedSong[sDisplayedNotePos - 1].vibrato) &&
                (sDisplayedSong[sDisplayedNotePos].bend == sDisplayedSong[sDisplayedNotePos - 1].bend))) {
            sDisplayedNoteValue = 0xFE;
        }

        if (sDisplayedNoteValue != sDisplayedSong[sDisplayedNotePos].noteIdx) {
            u8 noteIdx = sDisplayedSong[sDisplayedNotePos].noteIdx;

            // As BFlat4 is exactly in the middle of notes B & A, a flag is
            // added to the noteIdx to resolve which button to map Bflat4 to
            if (noteIdx == NOTE_BFLAT4) {
                sDisplayedNoteValue = noteIdx + sDisplayedSong[sDisplayedNotePos].BFlat4Flag;
            } else {
                sDisplayedNoteValue = noteIdx;
            }

            if (sDisplayedNoteValue != NOTE_NONE) {
                sDisplayedStaffPos++;
                Audio_QueueCmdS8(6 << 24 | 2 << 16 | SFX_CHANNEL_OCARINA << 8 | 7, sOcarinaInstrumentId - 1);
                Audio_QueueCmdS8(6 << 24 | 2 << 16 | SFX_CHANNEL_OCARINA << 8 | 5, sDisplayedNoteValue & 0x3F);
                Audio_PlaySfxGeneral(NA_SE_OC_OCARINA, &gDefaultSfxPos, 4, &sNormalizedNotePlaybackTone,
                                        &sNormalizedNotePlaybackVolume, &gDefaultSfxReverbAdd);
            } else {
                Audio_StopSfxById(NA_SE_OC_OCARINA);
            }
        }
        sDisplayedNotePos++;
    }
    

}

void AudioOcarina_SetRecordingSong(u8 isRecordingComplete) {
    u16 i;
    u16 i2;
    u16 pad;
    u8 noteIdx;
    OcarinaNote* note;
    u8 j;
    u8 k;
    s32 t;
    OcarinaNote* recordedSong;

    if (sRecordingState == OCARINA_RECORD_SCARECROW_LONG) {
        recordedSong = gScarecrowLongSongPtr;
    } else {
        /**
         * OCARINA_RECORD_SCARECROW_SPAWN
         *
         * The notes for scarecrows spawn song are first recorded into the ocarina termina
         * wall address to act as a buffer. That way, if a new scarecrow spawn song is
         * rejected, the previous scarecrow spawn song is not overwritten. If the scarecrow
         * spawn song is accepted, then the notes are then copied over to the scarecrow spawn
         * song address
         */
        recordedSong = sTerminaWallSongPtr;
    }

    recordedSong[sRecordSongPos].noteIdx = sRecordOcarinaNoteIdx;
    recordedSong[sRecordSongPos].length = sOcarinaUpdateTaskCurrent - sOcarinaRecordTaskStart;
    recordedSong[sRecordSongPos].volume = sRecordOcarinaVolume;
    recordedSong[sRecordSongPos].vibrato = sRecordOcarinaVibrato;
    recordedSong[sRecordSongPos].bend = sRecordOcarinaBendIdx;
    recordedSong[sRecordSongPos].BFlat4Flag = sRecordOcarinaButtonIdx & 0xC0;

    sRecordOcarinaNoteIdx = sCurOcarinaNoteIdx;
    sRecordOcarinaVolume = sCurOcarinaVolume;
    sRecordOcarinaVibrato = sCurOcarinaVibrato;
    sRecordOcarinaBendIdx = sCurOcarinaBendIdx;
    sRecordOcarinaButtonIdx = sCurOcarinaButtonIdx;

    sRecordSongPos++;

    if ((sRecordSongPos != ARRAY_COUNT(sScarecrowsLongSongNotes) - 1) && !isRecordingComplete) {
        return;
    }

    i = sRecordSongPos;
    noteIdx = NOTE_NONE;
    while (i != 0 && noteIdx == NOTE_NONE) {
        i--;
        noteIdx = recordedSong[i].noteIdx;
    }

    if (sRecordSongPos != (i + 1)) {
        sRecordSongPos = i + 2;
        recordedSong[sRecordSongPos - 1].length = 0;
    }

    recordedSong[sRecordSongPos].length = 0;

    if (sRecordingState == OCARINA_RECORD_SCARECROW_SPAWN) {
        if (sOcarinaStaffPlayingPos >= 8) {
            for (i = 0; i < sRecordSongPos; i++) {
                recordedSong[i] = recordedSong[i + 1];
            }

            AudioOcarina_MapSongFromNotesToButtons(OCARINA_SONG_TERMINA_WALL, OCARINA_SONG_SCARECROW, 8);

            // Tests to see if the notes from the scarecrow song contain identical
            // notes within its song to any of the reserved songIdx from 0 to 21

            // OCATEMP
            // Loop through each of the songs
            for (i = 0; i < OCARINA_SONG_SCARECROW; i++) {
                // Checks to see if the song is reserved
                if (sIsOcarinaSongReserved[i]) {
                    // Loops through all possible starting indices
                    for (j = 0; (j < 9 - gOcarinaSongButtons[i].numButtons); j++) {
                        // Loops through the notes of the song
                        for (k = 0; k < gOcarinaSongButtons[i].numButtons && k + j < 8 &&
                                    gOcarinaSongButtons[i].buttonIdx[k] ==
                                        gOcarinaSongButtons[OCARINA_SONG_SCARECROW].buttonIdx[k + j];
                             k++) {
                            ;
                        }

                        // This conditional is true if the recorded song matches a reserved song
                        // recorded song is cancelled
                        if (k == gOcarinaSongButtons[i].numButtons) {
                            sRecordingState = OCARINA_RECORD_REJECTED;
                            sOcarinaSongNotes[OCARINA_SONG_SCARECROW][1].volume = 0xFF;
                            return;
                        }
                    }
                }
            }

            // Counts how many times a note is repeated
            i = 1;
            while (i < 8) {
                if (gOcarinaSongButtons[OCARINA_SONG_SCARECROW].buttonIdx[0] !=
                    gOcarinaSongButtons[OCARINA_SONG_SCARECROW].buttonIdx[i]) {
                    i = 9;
                } else {
                    i++;
                }
            }

            // This condition is true if all 8 notes are the same pitch
            if (i == 8) {
                sRecordingState = OCARINA_RECORD_REJECTED;
                sOcarinaSongNotes[OCARINA_SONG_SCARECROW][1].volume = 0xFF;
                return;
            }

            // The scarecrow spawn song is accepted and copied from the buffer to the scarecrow spawn notes
            for (i = 0; i < sRecordSongPos; i++) {
                sOcarinaSongNotes[OCARINA_SONG_SCARECROW][i] = sOcarinaSongNotes[OCARINA_SONG_TERMINA_WALL][i];
            }

            sIsOcarinaInputEnabled = 0;
        } else {
            sOcarinaSongNotes[OCARINA_SONG_SCARECROW][1].volume = 0xFF;
        }
    }
    sRecordingState = OCARINA_RECORD_OFF;
}

/**
 * recordingState = 1, start long scarecrows song
 * recordingState = 0, end
 * recordingState = 2, also scarecrows song
 */
void AudioOcarina_SetRecordingState(u8 recordingState) {
    if ((u32)recordingState == sRecordingState) {
        return;
    }

    if (recordingState != OCARINA_RECORD_OFF) {
        sOcarinaRecordTaskStart = sOcarinaUpdateTaskCurrent;
        sRecordOcarinaNoteIdx = NOTE_NONE;
        sRecordOcarinaVolume = 0x57;
        sRecordOcarinaVibrato = 0;
        sRecordOcarinaBendIdx = 0;
        sRecordOcarinaButtonIdx = 0;
        sRecordSongPos = 0;
        sIsOcarinaInputEnabled = true;
        sOcarinaStaffPlayingPos = 0;
        sRecordingSongNote = sScarecrowsLongSongNotes[1];
    } else {
        if (sRecordSongPos == 0) {
            sScarecrowsLongSongNotes[1] = sRecordingSongNote;
        } else {
            if (sRecordingState == OCARINA_RECORD_SCARECROW_SPAWN) {
                sOcarinaStaffPlayingPos = 1;
            }

            AudioOcarina_SetRecordingSong(true);
        }

        sIsOcarinaInputEnabled = false;
        sOcarinaStaffPlayingPos = 0;
    }

    sRecordingState = recordingState;
    
}

void AudioOcarina_UpdateRecordingStaff(void) {
    sRecordingStaff.state = sRecordingState;
    sRecordingStaff.pos = sOcarinaStaffPlayingPos;
    if (sRecordingState == OCARINA_RECORD_REJECTED) {
        sRecordingState = OCARINA_RECORD_OFF;
    }
}

void AudioOcarina_UpdatePlayingStaff(void) {
    if (sCurOcarinaButtonIdx != OCARINA_BTN_INVALID) {
        sPlayingStaff.buttonIdx = sCurOcarinaButtonIdx & 0x3F;
    }
    sPlayingStaff.state = AudioOcarina_GetPlayingState();
    sPlayingStaff.pos = sOcarinaStaffPlayingPos;
}

void AudioOcarina_UpdateDisplayedStaff(void) {
    if ((sDisplayedNoteValue & 0x3F) <= NOTE_EFLAT5) {
        sDisplayedStaff.buttonIdx = AudioOcarina_MapNoteToButton(sDisplayedNoteValue);
    }

    sDisplayedStaff.state = sDisplayedState;

    if (sDisplayedSong != sScarecrowsLongSongNotes) {
        sDisplayedStaff.pos = sDisplayedStaffPos;
    } else if (sDisplayedStaffPos == 0) {
        sDisplayedStaff.pos = 0;
    } else {
        sDisplayedStaff.pos = ((sDisplayedStaffPos - 1) % 8) + 1;
    }
}

OcarinaStaff* AudioOcarina_GetRecordingStaff(void) {
    return &sRecordingStaff;
}

OcarinaStaff* AudioOcarina_GetPlayingStaff(void) {
    if (sPlayingStaff.state < 0xFE) {
        sOcarinaFlags = 0;
    }
    
    return &sPlayingStaff;
}

OcarinaStaff* AudioOcarina_GetDisplayedStaff(void) {
    return &sDisplayedStaff;
}

void AudioOcarina_RecordSong(void) {
    s32 noteChanged;

    if ((sRecordingState != OCARINA_RECORD_OFF) && ((sOcarinaUpdateTaskCurrent - sOcarinaRecordTaskStart) >= 3)) {
        noteChanged = false;
        if (sRecordOcarinaNoteIdx != sCurOcarinaNoteIdx) {
            if (sCurOcarinaNoteIdx != NOTE_NONE) {
                sRecordingStaff.buttonIdx = sCurOcarinaButtonIdx & 0x3F;
                sOcarinaStaffPlayingPos++;
            } else if ((sRecordingState == OCARINA_RECORD_SCARECROW_SPAWN) && (sOcarinaStaffPlayingPos == 8)) {
                AudioOcarina_SetRecordingSong(true);
                return;
            }

            if (sOcarinaStaffPlayingPos > 8) {
                if (sRecordingState == OCARINA_RECORD_SCARECROW_SPAWN) {
                    // notes played are over 8 and in recording mode.
                    AudioOcarina_SetRecordingSong(true);
                    return;
                }
                sOcarinaStaffPlayingPos = 1;
            }
            noteChanged = true;
        } else if (sRecordOcarinaVolume != sCurOcarinaVolume) {
            noteChanged = true;
        } else if (sRecordOcarinaVibrato != sCurOcarinaVibrato) {
            if (sRecordingState != OCARINA_RECORD_SCARECROW_SPAWN) {
                noteChanged = true;
            }
        } else if ((sRecordOcarinaBendIdx != sCurOcarinaBendIdx) && (sRecordingState != OCARINA_RECORD_SCARECROW_SPAWN)) {
            noteChanged = true;
        }

        if (noteChanged) {
            AudioOcarina_SetRecordingSong(false);
            sOcarinaRecordTaskStart = sOcarinaUpdateTaskCurrent;
        }
    }
}

/**
 * Tests to see if the notes from songIdx contain identical notes
 * within its song to any of the reserved songIdx from 0 up to maxSongIdx
 */
s32 AudioOcarina_TerminaWallValidateNotes(u8 songIdx, u8 maxSongIdx) {
    u8 curSongIdx;
    u8 j;
    u8 k;

    // loop through all possible songs up to maxSongIdx
    for (curSongIdx = 0; curSongIdx < maxSongIdx; curSongIdx++) {
        // check to see if the song is reserved or not
        if (sIsOcarinaSongReserved[curSongIdx]) {
            // starting index to test the song
            for (j = 0; j < (9 - gOcarinaSongButtons[curSongIdx].numButtons); j++) {
                // loop through each note in the song
                for (k = 0;
                     (k < gOcarinaSongButtons[curSongIdx].numButtons) && ((k + j) < 8) &&
                     (gOcarinaSongButtons[curSongIdx].buttonIdx[k] == gOcarinaSongButtons[songIdx].buttonIdx[(k + j)]);
                     k++) {
                    continue;
                }

                if (k == gOcarinaSongButtons[curSongIdx].numButtons) {
                    // failure: songIdx is identical to curSongIdx.
                    return -1;
                }
            }
        }
    }

    // success: notes are accepted and used
    return 0;
}

/**
 * Generates the notes displayed on the Termina Field wall of musical notes
 * Song generation iterates back and forth from 8 random notes to a song from Ocarina of Time (OoT).
 * Will check to see that the notes are valid by ensuring no playable song is within the selected notes
 * All OoT songs are valid, so the outer loop will run a maxiumum of two times.
 * i.e. if random notes fails, then the next set of notes will be from a valid OoT song
 */
void AudioOcarina_TerminaWallGenerateNotes(void) {
    OcarinaNote* ocarinaNote;
    u8 randButton;
    u8 i;
    u8 j;

    do {
        i = 0;
        if (sOcarinaWallCounter++ % 2) {
            j = 0;

            for (; i < 8; i++) {
                randButton = Audio_NextRandom() % 5;
                sOcarinaSongNotes[OCARINA_SONG_TERMINA_WALL][j].noteIdx = sButtonToNoteMap[randButton];
                sOcarinaSongNotes[OCARINA_SONG_TERMINA_WALL][j].length = 19;
                sOcarinaSongNotes[OCARINA_SONG_TERMINA_WALL][j].volume = 80;
                sOcarinaSongNotes[OCARINA_SONG_TERMINA_WALL][j].vibrato = 0;
                sOcarinaSongNotes[OCARINA_SONG_TERMINA_WALL][j].bend = 0;
                j++;
                sOcarinaSongNotes[OCARINA_SONG_TERMINA_WALL][j].noteIdx = NOTE_NONE;
                sOcarinaSongNotes[OCARINA_SONG_TERMINA_WALL][j].length = 3;
                sOcarinaSongNotes[OCARINA_SONG_TERMINA_WALL][j].volume = 0;
                sOcarinaSongNotes[OCARINA_SONG_TERMINA_WALL][j].vibrato = 0;
                sOcarinaSongNotes[OCARINA_SONG_TERMINA_WALL][j].bend = 0;
                j++;
            }

            sOcarinaSongNotes[OCARINA_SONG_TERMINA_WALL][j - 2].length = 90;
            sOcarinaSongNotes[OCARINA_SONG_TERMINA_WALL][j - 1].length = 22;
            sOcarinaSongNotes[OCARINA_SONG_TERMINA_WALL][j + 1].noteIdx = NOTE_NONE;
            sOcarinaSongNotes[OCARINA_SONG_TERMINA_WALL][j + 1].length = 0;
            AudioOcarina_MapSongFromNotesToButtons(OCARINA_SONG_TERMINA_WALL, OCARINA_SONG_TERMINA_WALL, 8);
        } else {
            j = Audio_NextRandom() % 9;
            ocarinaNote = sOoTOcarinaSongNotes[j];

            for (; ocarinaNote[i].length != 0; i++) {
                sOcarinaSongNotes[OCARINA_SONG_TERMINA_WALL][i] = ocarinaNote[i];
            }

            sOcarinaSongNotes[OCARINA_SONG_TERMINA_WALL][i].noteIdx = NOTE_NONE;
            sOcarinaSongNotes[OCARINA_SONG_TERMINA_WALL][i].length = 0;
            AudioOcarina_MapSongFromNotesToButtons(OCARINA_SONG_TERMINA_WALL, OCARINA_SONG_TERMINA_WALL,
                                                   sOoTOcarinaSongsNumNotes[j]);
        }
    } while (AudioOcarina_TerminaWallValidateNotes(OCARINA_SONG_TERMINA_WALL, OCARINA_SONG_TERMINA_WALL) != 0);
}

// Unused remnant of OoT
void AudioOcarina_MemoryGameSetNumNotes(u8 minigameRound) {
    u8 i;

    if (minigameRound > 2) {
        minigameRound = 2;
    }

    sOcaMemoryGameAppendPos = 0;
    sOcaMemoryGameEndPos = sOcaMemoryGameNumNotes[minigameRound];

    for (i = 0; i < 3; i++) {
        AudioOcarina_MemoryGameGenerateNotes();
    }
}

/**
 * Unused remnant of OoT, Id 14 now represent Goron Lullaby Intro instead of the OoT ocarina memory game
 */
#define OCARINA_SONG_MEMORYGAME OCARINA_SONG_GORON_LULLABY_INTRO
s32 AudioOcarina_MemoryGameGenerateNotes(void) {
    u32 rndButtonIdx;
    u8 rndNoteIdx;

    if (sOcaMemoryGameAppendPos == sOcaMemoryGameEndPos) {
        return true;
    }

    rndButtonIdx = Audio_NextRandom();
    rndNoteIdx = sButtonToNoteMap[rndButtonIdx % 5];

    if (sOcarinaSongNotes[OCARINA_SONG_GORON_LULLABY_INTRO][sOcaMemoryGameAppendPos - 1].noteIdx == rndNoteIdx) {
        rndNoteIdx = sButtonToNoteMap[(rndButtonIdx + 1) % 5];
    }

    sOcarinaSongNotes[OCARINA_SONG_GORON_LULLABY_INTRO][sOcaMemoryGameAppendPos].noteIdx = rndNoteIdx;
    sOcarinaSongNotes[OCARINA_SONG_GORON_LULLABY_INTRO][sOcaMemoryGameAppendPos].length = 45;
    sOcarinaSongNotes[OCARINA_SONG_GORON_LULLABY_INTRO][sOcaMemoryGameAppendPos].volume = 0x50;
    sOcarinaSongNotes[OCARINA_SONG_GORON_LULLABY_INTRO][sOcaMemoryGameAppendPos].vibrato = 0;
    sOcarinaSongNotes[OCARINA_SONG_GORON_LULLABY_INTRO][sOcaMemoryGameAppendPos].bend = 0;

    sOcaMemoryGameAppendPos++;

    sOcarinaSongNotes[OCARINA_SONG_GORON_LULLABY_INTRO][sOcaMemoryGameAppendPos].noteIdx = NOTE_NONE;
    sOcarinaSongNotes[OCARINA_SONG_GORON_LULLABY_INTRO][sOcaMemoryGameAppendPos].length = 0;
    sOcarinaSongNotes[OCARINA_SONG_GORON_LULLABY_INTRO][sOcaMemoryGameAppendPos + 1].noteIdx = NOTE_NONE;
    sOcarinaSongNotes[OCARINA_SONG_GORON_LULLABY_INTRO][sOcaMemoryGameAppendPos + 1].length = 0;
    if (1) {}
    return false;
}

void AudioOcarina_Update(void) {
    sOcarinaUpdateTaskCurrent = gAudioContext.totalTaskCnt;
    if (sOcarinaInstrumentId != OCARINA_INSTRUMENT_OFF) {
        if (sIsOcarinaInputEnabled == true) {
            AudioOcarina_ReadControllerInput();
        }

        if ((sDisplayedState == 0) && (sIsOcarinaInputEnabled == true)) {
            AudioOcarina_PlayControllerInput(false);
        }

        if (sOcarinaFlags != 0) {
            if (sOcarinaFlags & 0x40000000) {
                AudioOcarina_CheckSongsWithoutMusicStaff();
            } else {
                AudioOcarina_CheckSongsWithMusicStaff();
            }
        }

        AudioOcarina_PlayDisplayedSong();
        sOcarinaPlaybackTaskStart = sOcarinaUpdateTaskCurrent;

        if (sDisplayedState == 0) {
            AudioOcarina_RecordSong();
        }

        if ((sOcarinaFlags != 0) && (sPrevOcarinaNoteIdx != sCurOcarinaNoteIdx)) {
            sIsOcarinaNoteChanged = true;
        }

        sPrevOcarinaNoteIdx = sCurOcarinaNoteIdx;

        if (sOcarinaResetDelay != 0) {
            sOcarinaResetDelay--;
            if (sOcarinaResetDelay == 0) {
                AudioOcarina_SetInstrumentId(OCARINA_INSTRUMENT_OFF);
            }
        }
    }

    AudioOcarina_UpdatePlayingStaff();
    AudioOcarina_UpdateDisplayedStaff();
    AudioOcarina_UpdateRecordingStaff();
}

void AudioOcarina_PlayLongScarecrowAfterCredits(void) {
    static u8 sScarecrowAfterCreditsState = 0;
    static u8 sScarecrowAfterCreditsIntrumentId = 1;
    static u16 sScarecrowAfterCreditsTimer = 1200;

    switch (sScarecrowAfterCreditsState) {
        case 0:
            if (sScarecrowAfterCreditsTimer-- == 0) {
                if (sScarecrowAfterCreditsIntrumentId < 7) {
                    // set next ocarina instrument and restart
                    sScarecrowAfterCreditsState++;
                } else {
                    // finished
                    sScarecrowAfterCreditsState = 3;
                    AudioOcarina_SetInstrumentId(OCARINA_INSTRUMENT_OFF);
                }
                sScarecrowAfterCreditsTimer = 1200;
            }
            break;
        case 1:
            Audio_SetSfxBanksMute(0);
            AudioOcarina_SetInstrumentId(sScarecrowAfterCreditsIntrumentId);
            AudioOcarina_SetDisplayedSong(OCARINA_SONG_SCARECROW_LONG + 1, 1);
            sScarecrowAfterCreditsIntrumentId++;
            sScarecrowAfterCreditsState++;
            break;
        case 2:
            if (AudioOcarina_GetDisplayedStaff()->state == 0) {
                sScarecrowAfterCreditsState = 0;
            }
            break;
    }
}

u8 sRequestCustomSequence = false;

// New to MM
void AudioOcarina_SetCustomSequence(void) {
    // Never passes true as sRequestCustomSequence is never set true
    if (sRequestCustomSequence && gAudioContext.seqPlayers[SEQ_PLAYER_FANFARE].enabled &&
        ((u8)gAudioContext.seqPlayers[SEQ_PLAYER_FANFARE].soundScriptIO[0] == 0xFF)) {
        gAudioContext.seqPlayers[SEQ_PLAYER_FANFARE].seqData = sCustomSequenceScript;
        sRequestCustomSequence = false;
    }
}

// New to MM
// Called by unused function
void AudioOcarina_PlayCustomSequence(void) {
    sRequestCustomSequence = true;
    AudioSeqCmd_StartSequence(SEQ_PLAYER_FANFARE, 0, NA_BGM_DUNGEON_APPEAR);
}

// Unused
// New to MM
#ifdef NON_EQUIVALENT
s32 AudioOcarina_CreateCustomSequence(void) {
    OcarinaNote* prevNote;
    u16 i;
    u16 phi_s4;
    u16 temp_a0;
    u16 temp_lo;
    u16 phi_a2;
    u8 new_var2;
    s8 phi_a0;
    s8 phi_s2; // bend
    u8 phi_t1; // vibrato
    u8 phi_t5;
    s32 temp_a2;


    phi_a2 = 0;
    phi_t1 = 0;
    phi_s2 = 0;
    phi_t5 = 0;
    phi_s4 = 0;

    sCustomSequencePc = 0x27;

    prevNote = &sScarecrowsLongSongNotes[0];
    for (i = 1; ((prevNote->noteIdx == 0xFF) && (prevNote->length != 0));) {
        prevNote = &sScarecrowsLongSongNotes[i];
        i++;
    } 

    for (;(prevNote->length != 0) && (sCustomSequencePc < 0x18A);) {

        temp_a0 = ((prevNote->length * 0x30) / 30);

        if (phi_t1 != prevNote->vibrato) {
            sCustomSequenceScript[sCustomSequencePc++] = 0xFD;
            
            if (phi_a2 >= 0x80) {
                sCustomSequenceScript[sCustomSequencePc++] = (((phi_a2 >> 8) & 0xFF) & 0x7F) + 0x80;
                sCustomSequenceScript[sCustomSequencePc++] = phi_a2 & 0xFF;
            } else {
                sCustomSequenceScript[sCustomSequencePc++] = phi_a2;
            }

            sCustomSequenceScript[sCustomSequencePc++] = 0xD8;
            sCustomSequenceScript[sCustomSequencePc++] = prevNote->vibrato;
            phi_a2 = temp_a0;
            phi_t1 = prevNote->vibrato;
        } else {
            phi_a2 += temp_a0;
        }

        prevNote = &sScarecrowsLongSongNotes[i];
        i++;
    }

    if (phi_a2 != 0) {
        sCustomSequenceScript[sCustomSequencePc++] = 0xFD;
        if (phi_a2 >= 0x80) {
            sCustomSequenceScript[sCustomSequencePc++] = (((phi_a2 >> 8) & 0xFF) & 0x7F) + 0x80;
            sCustomSequenceScript[sCustomSequencePc++] = phi_a2 & 0xFF;
        } else {
            sCustomSequenceScript[sCustomSequencePc++] = phi_a2;
        }
    }

    sCustomSequenceScript[sCustomSequencePc++] = 0xFF;

    sCustomSequenceScript[0x18] = sCustomSequencePc >> 8;
    sCustomSequenceScript[0x19] = sCustomSequencePc & 0xFF;
    sCustomSequenceScript[0x1B] = (sCustomSequencePc + 4) >> 8;
    sCustomSequenceScript[0x1C] = (sCustomSequencePc + 4) & 0xFF;

    sCustomSequenceScript[sCustomSequencePc++] = 0xC2;
    sCustomSequenceScript[sCustomSequencePc++] = 0xFB;
    sCustomSequenceScript[sCustomSequencePc++] = 0xC0;
    sCustomSequenceScript[sCustomSequencePc++] = 8;

    sCustomSequenceScript[sCustomSequencePc++] = 0xC1;
    sCustomSequenceScript[sCustomSequencePc++] = 0x57;
    sCustomSequenceScript[sCustomSequencePc++] = 0xC9;
    sCustomSequenceScript[sCustomSequencePc++] = 0;

    prevNote = &sScarecrowsLongSongNotes[0];
    temp_lo += 0; // TODO: Needed?
    for (i = 1; ((prevNote->noteIdx == 0xFF) && (prevNote->length != 0)); i++) {
        prevNote = &sScarecrowsLongSongNotes[i];
    }

    for (;(prevNote->length != 0) && (sCustomSequencePc < 0x18A);prevNote = &sScarecrowsLongSongNotes[i], i++) {

        if (prevNote->noteIdx == sScarecrowsLongSongNotes[i].noteIdx) {
            temp_a0 = sScarecrowsLongSongNotes[i].length; // TODO: Fake temp
            if ((temp_a0 != 0) && (phi_t5 == 0)) {
                sCustomSequenceScript[sCustomSequencePc++] = 0xC4;
                phi_t5 = 1;
            }
        } else if ((phi_t5 == 1) && (sScarecrowsLongSongNotes[i].noteIdx != 0xFF) &&
                   (sScarecrowsLongSongNotes[i].noteIdx != 0)) {
            sCustomSequenceScript[sCustomSequencePc++] = 0xC5;
            phi_t5 = 0;
        }

        if (temp_lo) {} // TODO: Needed?

        if (phi_s2 != prevNote->bend) {
            sCustomSequenceScript[sCustomSequencePc++] = 0xCE;


            if (ABS_ALT(prevNote->bend) > 0x40) {
                phi_a0 = 0x7F;
            } else if (prevNote->bend < 0) {
                phi_a0 = -prevNote->bend;
            } else {
                phi_a0 = ((prevNote->bend * 0x7F) / 0x40);
            }
            

            if (prevNote->bend < 0) {
                phi_a0 *= -1;
            }

            sCustomSequenceScript[sCustomSequencePc++] = phi_a0;

            phi_s2 = prevNote->bend;
        }
        
        new_var2 = prevNote->noteIdx + 0x27;
        if (prevNote->noteIdx != 0xFF) {
            sCustomSequenceScript[sCustomSequencePc++] = new_var2;
        } else {
            sCustomSequenceScript[sCustomSequencePc++] = 0xC0;
        }

        do {
        temp_a2 = (prevNote->length * (0x30)) + phi_s4;
        temp_lo = temp_a2 / 30;
        
        if (temp_lo < 0x80) {
            sCustomSequenceScript[sCustomSequencePc++] = temp_lo;
        } else {
            sCustomSequenceScript[sCustomSequencePc++] = ((temp_lo >> 8) & 0x7F) + 0x80;
            sCustomSequenceScript[sCustomSequencePc++] = temp_lo;
            if ((!prevNote->bend) && (!prevNote->bend)) {}  // TODO: Needed?
        }
        
        phi_s4 = temp_a2 - (temp_lo * 30);

        } while (0); // TODO: Needed?
        
        // prevNote = &sScarecrowsLongSongNotes[i]; i++;
    }

    sCustomSequenceScript[sCustomSequencePc++] = 0xFF;

    AudioOcarina_PlayCustomSequence();

    if (i >= 0x18A) {
        return -1;
    } else {
        return 0;
    }
    
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_interface/AudioOcarina_CreateCustomSequence.s")
#endif

void AudioOcarina_ResetInstrumentId(void) {
    if (sOcarinaInstrumentId != OCARINA_INSTRUMENT_OFF) {
        AudioSeqCmd_SetChannelIO(SEQ_PLAYER_SFX, 1, SFX_CHANNEL_OCARINA, 0);
        sOcarinaInstrumentId = OCARINA_INSTRUMENT_OFF;
        AudioOcarina_ResetAndMute();
    }
}

void AudioOcarina_ResetStaff(void) {
    u8 songIdx;

    sPlayingStaff.buttonIdx = OCARINA_BTN_INVALID;
    sPlayingStaff.state = 0xFF;
    sPlayingStaff.pos = 0;
    sDisplayedStaff.buttonIdx = OCARINA_BTN_INVALID;
    sDisplayedStaff.state = 0;
    sDisplayedStaff.pos = 0;
    sRecordingStaff.buttonIdx = OCARINA_BTN_INVALID;
    sRecordingStaff.state = 0xFF;
    sRecordingStaff.pos = 0;
    sOcarinaResetDelay = 0;
    sIsOcarinaNoteChanged = false;

    for (songIdx = OCARINA_SONG_SONATA; songIdx < OCARINA_SONG_MAX; songIdx++) {
        D_801FD518[songIdx] = 0xFF;
        D_801FD530[songIdx] = 0;
    }

    D_801D8534 = 0;
}

// AudioDebug Remnant
void Audio_Unused1(s32 arg0) {
}

// AudioDebug Remnant
void Audio_Unused2(s32 arg0, s32 arg1) {
}

// AudioDebug Remnant
void Audio_Unused3(void) {
}

// OoT func_800F3054
void Audio_Update(void) {
    if ((func_801A9768() == 0) && (func_801A982C() == 0)) {
        AudioOcarina_SetCustomSequence();
        AudioOcarina_Update();
        AudioVoice_Update();
        Audio_StepFreqLerp(&sRiverFreqScaleLerp);
        Audio_StepFreqLerp(&sWaterfallFreqScaleLerp);
        Audio_UpdateRiverSoundVolumes();
        Audio_UpdateBgmPlayerIOPort7();
        Audio_UpdateFanfare();
        Audio_UpdateSfxVolumeTransition();
        Audio_UpdateSubBgmAtFixedPos1();
        Audio_UpdateFanfareAtFixedPos();
        Audio_UpdateSubBgmAtFixedPos2();
        Audio_UpdatePauseState();
        Audio_UpdateSequenceAtFixedPos();
        Audio_ResetNewSceneSeqId();
        Audio_ProcessSfxRequests();
        Audio_ProcessSeqCmds();
        Audio_ProcessActiveSfxs();
        Audio_UpdateActiveSequences();
        Audio_ProcessSfxSettings();
        Audio_ScheduleProcessCmds();
    }
}

// OoT func_800F3138
void Audio_Unused4(UNK_TYPE arg0) {
}

// OoT func_800F3140
void Audio_Unused5(UNK_TYPE arg0, UNK_TYPE arg1) {
}

/**
 * Queues a bgm sequence directly to the internal audio queue system. 
 * Skips the external audio command process
 * Unused
 */
void Audio_PlayMainBgm(s8 seqId) {
    Audio_QueueCmdS32(0x82 << 24 | SEQ_PLAYER_BGM_MAIN << 16 | (((u8)seqId & 0xFF) << 8), 1);
}

// OoT func_800F3188
f32 Audio_ComputeSfxVolume(u8 bankId, u8 entryIdx) {
    SfxBankEntry* bankEntry = &gSfxBanks[bankId][entryIdx];
    f32 minDist;
    f32 phi_f14;
    f32 baseDist;
    f32 phi_f12;
    f32 volume; // Also acts as intermediate volumeSquared
    s32 pad[3];

    if (bankEntry->sfxParams & 0x2000) {
        return 1.0f;
    }

    if (bankEntry->dist > 10000.0f) {
        volume = 0.0f;
    } else {
        if (bankEntry->sfxParams & 4) {
            phi_f12 = 0.35f;
            phi_f14 = 0.65f;
        } else {
            phi_f12 = 0.81f;
            phi_f14 = 0.19f;
        }
        switch (bankEntry->sfxParams & 7) {
            case 0:
                baseDist = 500.0f;
                break;
            case 1:
                baseDist = 666.6667f;
                break;
            case 2:
                baseDist = 952.381f;
                break;
            case 3:
                baseDist = 3846.154f;
                break;
            case 4:
                baseDist = 1000.0f;
                break;
            case 5:
                baseDist = 666.6667f;
                break;
            case 6:
                baseDist = 500.0f;
                break;
            case 7:
                baseDist = 400.0f;
                break;
        }

        minDist = baseDist / 5.0f;

        if (bankEntry->dist < minDist) {
            volume = 1.0f;
        } else if (bankEntry->dist < baseDist) {
            volume = ((((baseDist - minDist) - (bankEntry->dist - minDist)) / (baseDist - minDist)) * phi_f14) + phi_f12;
        } else {
            volume = (1.0f - ((bankEntry->dist - baseDist) / (10000.0f - baseDist))) * phi_f12;
        }
        
        volume = SQ(volume);
    }

    return volume;
}

s8 Audio_ComputeSfxReverb(u8 bankId, u8 entryIdx, u8 channelIdx) {
    s8 distAdd = 0;
    s32 scriptAdd = 0;
    SfxBankEntry* entry = &gSfxBanks[bankId][entryIdx];
    s32 reverb;

    if (!(entry->sfxParams & 0x1000)) {
        if (entry->dist < 2500.0f) {
            distAdd = *entry->posZ > 0.0f ? (entry->dist / 2500.0f) * 70.0f : (entry->dist / 2500.0f) * 91.0f;
        } else {
            distAdd = 70;
        }
    }

    if (IS_SEQUENCE_CHANNEL_VALID(gAudioContext.seqPlayers[SEQ_PLAYER_SFX].channels[channelIdx])) {
        scriptAdd = gAudioContext.seqPlayers[SEQ_PLAYER_SFX].channels[channelIdx]->soundScriptIO[1];
        if (gAudioContext.seqPlayers[SEQ_PLAYER_SFX].channels[channelIdx]->soundScriptIO[1] < 0) {
            scriptAdd = 0;
        }
    }

    reverb = (*entry->reverbAdd & 0x7F) + distAdd;

    if (entry->state != 2) {
        reverb += scriptAdd;
    }

    if ((bankId != BANK_OCARINA) || !((entry->sfxId & 0x3FF) < 2)) {
        reverb += sAudioEnvReverb + (sAudioCodeReverb & 0x3F) + sSpecReverb;
    }

    if (reverb > 0x7F) {
        reverb = 0x7F;
    }

    return reverb;
}

s8 Audio_ComputeSfxPanSigned(f32 x, f32 z, u8 token) {
    f32 absX;
    f32 absZ;
    f32 pan;

    if (x < 0) {
        absX = -x;
    } else {
        absX = x;
    }
    if (z < 0) {
        absZ = -z;
    } else {
        absZ = z;
    }

    if (absX > 8000.0f) {
        absX = 8000.0f;
    }

    if (absZ > 8000.0f) {
        absZ = 8000.0f;
    }

    if ((x == 0.0f) && (z == 0.0f)) {
        pan = 0.5f;
    } else if (absZ <= absX) {
        pan = (16000.0f - absX) / (4.5f * (16000.0f - absZ));
        if (x >= 0.0f) {
            pan = 1.0f - pan;
        }
    } else {
        pan = (x / (3.6f * absZ)) + 0.5f;
    }

    if (absZ < 50.0f) {
        if (absX < 50.0f) {
            pan = ((pan - 0.5f) * SQ(absX / 50.0f)) + 0.5f;
        }
    }
    return (s8)((pan * 127.0f) + 0.5f);
}

f32 Audio_ComputeSfxFreqScale(u8 bankId, u8 entryIdx) {
    s32 phi_v0 = 0;
    SfxBankEntry* entry = &gSfxBanks[bankId][entryIdx];
    f32 unk1C;
    f32 freq = 1.0f;

    if (entry->sfxParams & 0x4000) {
        freq = 1.0f - ((gAudioContext.audioRandom & 0xF) / 192.0f);
    }

    switch (bankId) {
        case BANK_VOICE:
            if ((entry->sfxId & 0xFF) < 0x40 && sAudioBaseFilter2 != 0) {
                phi_v0 = 1;
            } else if ((entry->sfxId & 0xFF) >= 0x40 && sAudioExtraFilter2 != 0) {
                phi_v0 = 1;
            }
            break;
        case BANK_PLAYER:
        case BANK_ITEM:

            if (sAudioBaseFilter2 != 0) {
                phi_v0 = 1;
            }
            break;
        case BANK_ENV:
        case BANK_ENEMY:
            if (((*entry->reverbAdd & 0x80) != 0) | (sAudioExtraFilter2 != 0)) {
                phi_v0 = 1;
            }
            break;
        case BANK_SYSTEM:
        case BANK_OCARINA:
            break;
    }

    if (phi_v0 == 1) {
        if (!(entry->sfxParams & 0x800)) {
            freq *= (1.0293 - ((gAudioContext.audioRandom & 0xF) / 144.0f));
        }
    }

    unk1C = entry->dist;
    if (!(entry->sfxParams & 0x2000)) {
        if (!(entry->sfxParams & 0x8000)) {
            if (unk1C >= 10000.0f) {
                freq += 0.2f;
            } else {
                freq += (0.2f * (unk1C / 10000.0f));
            }
        }
    }

    if (entry->sfxParams & 0xC0) {
        freq += (entry->unk_2F / 192.0f);
    }

    return freq;
}

// OoT func_800F37B8
u8 Audio_ComputeSurroundSoundFilter(f32 behindScreenZ, SfxBankEntry* entry, s8 arg2) {
    u8 phi_v1;
    u8 phi_a0;
    u16* sfxParams;
    f32 phi_f0;
    f32 phi_f12;

    // Remnant of OoT
    if (*entry->posZ < behindScreenZ) {
        phi_v1 = 0;
    } else {
        phi_v1 = 0;
    }

    sfxParams = &entry->sfxParams;
    if (*sfxParams & 0x200) {
        phi_v1 = 0xF;
    }

    switch (*sfxParams & 7) {
        case 0:
            phi_f0 = 15.0f;
            break;
        case 1:
            phi_f0 = 12.0f;
            break;
        case 2:
            phi_f0 = 9.0f;
            break;
        case 3:
            phi_f0 = 6.0f;
            break;
        case 4:
            phi_f0 = 18.0f;
            break;
        case 5:
            phi_f0 = 21.0f;
            break;
        case 6:
            phi_f0 = 24.0f;
            break;
        case 7:
            phi_f0 = 27.0f;
            break;
        default:
            break;
    }

    if (!(entry->sfxFlags & 0x20)) {
        if (entry->dist > 1923.077f) {
            phi_f12 = 1923.077f;
        } else {
            phi_f12 = entry->dist;
        }

        phi_a0 = (phi_f0 * phi_f12) / 3846.154f;
    } else {
        phi_a0 = 0;
    }

    return (phi_v1 * 0x10) + phi_a0;
}

// OoT func_800F3990
s8 Audio_ComputeSurroundSoundMixVolume(f32 yPos, u16 sfxParams) {
    s8 volume;

    if (yPos < 0.0f) {
        if (yPos < -625.0f) {
            volume = -32;
        } else {
            volume = (s8)(((625.0f + yPos) / 625.0f) * 31.0f) + 0xE0;
        }
    } else if (yPos > 1250.0f) {
        volume = 127;
    } else {
        volume = (yPos / 1250.0f) * 126.0f;
    }

    return volume | 1;
}

void Audio_SetSfxProperties(u8 bankId, u8 entryIdx, u8 channelIdx) {
    static s8 D_801D8B30[] = {
        0,  2,  4,   6,   8,   10,  12,  14,  16,  18,  20,  22,  24,  25,  26,  27,  28,  29,  30, 31, 32, 33,
        34, 35, 36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52, 53, 54, 55,
        56, 56, 57,  57,  58,  58,  59,  59,  60,  60,  60,  61,  61,  61,  62,  62,  62,  63,  63, 63, 64, 64,
        64, 65, 65,  65,  66,  66,  66,  67,  67,  67,  68,  68,  69,  69,  70,  70,  71,  71,  72, 73, 74, 75,
        76, 77, 78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94, 95, 96, 97,
        98, 99, 100, 101, 102, 103, 105, 107, 109, 111, 113, 115, 117, 119, 121, 123, 125, 127,
    };
    f32 vol = 1.0f;
    s8 volS8;
    s8 reverb = 0;
    f32 freqScale = 1.0f;
    s8 panSigned = 0x40;
    u8 pad;
    u8 filter = 0;
    s8 yVolume = 0;
    s8 zVolume = -1;
    f32 behindScreenZ;
    u8 baseFilter = 0;
    SfxBankEntry* entry = &gSfxBanks[bankId][entryIdx];
    u8 pad2;

    switch (bankId) {
        case BANK_PLAYER:
        case BANK_ITEM:
        case BANK_ENV:
        case BANK_ENEMY:
        case BANK_VOICE:
            if (sAudioFileSelectSetting == AUDIO_FS_SURROUND) {
                yVolume = Audio_ComputeSurroundSoundMixVolume(*entry->posY, entry->sfxParams);
            }
            // fallthrough
        case BANK_OCARINA:
            entry->dist = sqrtf(entry->dist * 10.0f);
            vol = Audio_ComputeSfxVolume(bankId, entryIdx) * *entry->vol;
            reverb = Audio_ComputeSfxReverb(bankId, entryIdx, channelIdx);
            panSigned = Audio_ComputeSfxPanSigned(*entry->posX, *entry->posZ, entry->token);
            freqScale = Audio_ComputeSfxFreqScale(bankId, entryIdx) * *entry->freqScale;

            if (sAudioFileSelectSetting == AUDIO_FS_SURROUND) {
                if (*entry->posZ >= 0.0f) {
                    if (*entry->posZ < 200.0f) {
                        zVolume = 32.0f - ((*entry->posZ / 300.0f) * 32.0f);
                    } else {
                        zVolume = 0;
                    }
                } else if (*entry->posZ < 0.0f) {
                    if (*entry->posZ > -300.0f) {
                        zVolume = 32.0f - ((*entry->posZ / 300.0f) * 95.0f);
                    } else {
                        zVolume = 127;
                    }
                }
                zVolume = D_801D8B30[zVolume];
            }

            if (sAudioBaseFilter != 0) {
                if ((bankId == BANK_PLAYER) || (bankId == BANK_ITEM) ||
                    ((bankId == BANK_VOICE) && (((entry->sfxId & 0xFF) < 0x40) || ((entry->sfxId & 0xFF) >= 0x80)))) {
                    baseFilter = sAudioBaseFilter;
                    if (sAudioExtraFilter) {
                        reverb >>= 2;
                    }
                }
            }

            if (((*entry->reverbAdd & 0x80) | baseFilter | sAudioExtraFilter | (entry->sfxFlags & 0x80)) != 0) {
                filter = ((((*entry->reverbAdd & 0x80) >> 2) | baseFilter | sAudioExtraFilter |
                           ((entry->sfxFlags & 0x80) >> 2)) *
                          2);
                filter &= 0xFF;
            } else if ((sAudioFileSelectSetting == AUDIO_FS_SURROUND) && (entry->sfxParams & 0x2000) == 0) {
                filter = Audio_ComputeSurroundSoundFilter(behindScreenZ, entry, panSigned);
            }
            break;
        case BANK_SYSTEM:
            break;
    }

    if (sSfxChannelState[channelIdx].vol != vol) {
        volS8 = (u8)(vol * 127.0f);
        sSfxChannelState[channelIdx].vol = vol;
    } else {
        volS8 = -1;
        if (1) {} // TODO: Needed?
    }

    // CHAN_UPD_SCRIPT_IO (slot 2, sets volume)
    Audio_QueueCmdS8(6 << 24 | 2 << 16 | (channelIdx << 8) | 2, volS8);
    
    if (sSfxChannelState[channelIdx].reverb != reverb) {
        sSfxChannelState[channelIdx].reverb = reverb;
    }
    if (sSfxChannelState[channelIdx].freqScale != freqScale) {
        sSfxChannelState[channelIdx].freqScale = freqScale;
    }
    if (sSfxChannelState[channelIdx].stereoBits != 0) {
        sSfxChannelState[channelIdx].stereoBits = 0;
    }
    if (sSfxChannelState[channelIdx].filter != filter) {
        // CHAN_UPD_SCRIPT_IO (slot 3, sets filter)
        sSfxChannelState[channelIdx].filter = filter;
    }
    if (sSfxChannelState[channelIdx].unk_0C != yVolume) {
        sSfxChannelState[channelIdx].unk_0C = yVolume;
    }
    if (sSfxChannelState[channelIdx].unk_0D != zVolume) {
        sSfxChannelState[channelIdx].unk_0D = zVolume;
    }
    if (sSfxChannelState[channelIdx].panSigned != panSigned) {
        sSfxChannelState[channelIdx].panSigned = panSigned;
    }
}

u32 func_8019F024(u8 channelIdx, SequenceChannel* channel) {
    channel->stereo.asByte = sSfxChannelState[channelIdx].stereoBits;
    channel->freqScale = sSfxChannelState[channelIdx].freqScale;
    channel->changes.s.freqScale = true;
    return channelIdx;
}

// OoT func_800F3ED4
void Audio_ResetSfxChannelState(void) {
    SfxPlayerState* state;
    u8 i;

    for (i = 0; i < ARRAY_COUNT(sSfxChannelState); i++) {
        state = &sSfxChannelState[i];
        state->vol = 1.0f;
        state->freqScale = 1.0f;
        state->reverb = 0;
        state->panSigned = 0x40;
        state->stereoBits = 0;
        state->filter = 0xFF;
        state->unk_0C = 0xFF;
        state->unk_0D = 0xFF;
    }

    sSfxChannelState[SFX_CHANNEL_OCARINA].unk_0C = 0;
    sPrevSeqMode = SEQ_MODE_DEFAULT;
    sAudioCodeReverb = 0;
}

void Audio_PlaySfx1(u16 sfxId) {
    Audio_PlaySfxGeneral(sfxId, &gDefaultSfxPos, 4, &gDefaultSfxVolOrFreq, &gDefaultSfxVolOrFreq,
                         &gDefaultSfxReverbAdd);
    if (sfxId == NA_SE_OC_TELOP_IMPACT) {
        Audio_SetSequenceMode(SEQ_MODE_DEFAULT);
    }
}

void Audio_PlaySfx2(u16 sfxId) {
    Audio_PlaySfxGeneral(sfxId, &gDefaultSfxPos, 4, &gDefaultSfxVolOrFreq, &gDefaultSfxVolOrFreq,
                         &gDefaultSfxReverbAdd);
}

/**
 * Bends the pitch of the sfx by a little under two semitones and adds reverb
 */
void Audio_PlaySfxAtPosWithPresetLowFreqAndHighReverb(Vec3f* pos, u16 sfxId) {
    Audio_PlaySfxGeneral(sfxId, pos, 4, &sTwoSemitonesLoweredFreq, &gDefaultSfxVolOrFreq, &sIncreasedSfxReverb);
}

void Audio_PlaySfxAtPos(Vec3f* pos, u16 sfxId) {
    Audio_PlaySfxGeneral(sfxId, pos, 4, &gDefaultSfxVolOrFreq, &gDefaultSfxVolOrFreq, &gDefaultSfxReverbAdd);
}

void Audio_PlaySfxForMessageDecide(void) {
    Audio_PlaySfx1(NA_SE_SY_DECIDE);
    Audio_StopSfxById(NA_SE_SY_MESSAGE_PASS);
}

void Audio_PlaySfxForMessageCancel(void) {
    Audio_PlaySfx1(NA_SE_SY_CANCEL);
    Audio_StopSfxById(NA_SE_SY_MESSAGE_PASS);
}

SfxSettings* Audio_AddSfxSetting(Vec3f* pos) {
    SfxSettings* sfxSettings;
    u8 i = 0;
    u8 sfxSettingIndex = 0xFF;

    for (; i < ARRAY_COUNT(sSfxSettings); i++) {
        sfxSettings = &sSfxSettings[i];

        if ((sfxSettings->pos == NULL) && (sfxSettingIndex == 0xFF)) {
            sfxSettingIndex = i;
        }

        if (sfxSettings->pos == pos) {
            return sfxSettings;
        }
    }

    if (sfxSettingIndex != 0xFF) {
        sSfxSettingsFlags |= 1 << sfxSettingIndex;
        sfxSettings = &sSfxSettings[sfxSettingIndex];
        sfxSettings->pos = pos;
        return sfxSettings;
    }

    return NULL;
}

void Audio_ProcessSfxSettings(void) {
    SfxBankEntry* entry;
    s32 temp_a2;
    u8 temp_v1;
    u8 sfxSettingsFlags;
    u8 sfxSettingIndex = 0;
    u8 bankId;
    u8 entryIndex;
    s32 phi_a0;

    if (sSfxSettingsFlags != 0) {
        sfxSettingsFlags = sSfxSettingsFlags;

        while (sfxSettingsFlags != 0) {

            bankId = 2;
            if ((sfxSettingsFlags & (1 << sfxSettingIndex))) {

                phi_a0 = false;
                while ((bankId < 4) && !phi_a0) {
                    entryIndex = gSfxBanks[bankId]->next;

                    while (entryIndex != 0xFF) {
                        entry = &gSfxBanks[bankId][entryIndex];
                        entryIndex = 0xFF;
                        if (entry->posX == &sSfxSettings[sfxSettingIndex].pos->x) {
                            phi_a0 = true;
                        } else {
                            entryIndex = entry->next;
                        }
                    }
                    bankId++;
                }

                if (!phi_a0) {
                    sSfxSettingsFlags ^= (1 << sfxSettingIndex);
                    sSfxSettings[sfxSettingIndex].pos = NULL;
                }

                sfxSettingsFlags ^= (1 << sfxSettingIndex);
            }

            sfxSettingIndex++;
        }
    }
}

/**
 * Used for Gyorg and Bigslime
 */
void Audio_PlaySfxForUnderwaterBosses(Vec3f* pos, u16 sfxId) {
    if ((sfxId == NA_SE_EN_KONB_JUMP_OLD) || (sfxId == NA_SE_EN_KONB_SINK_OLD)) {
        Audio_PlaySfxGeneral(sfxId, pos, 4, &gDefaultSfxVolOrFreq, &gDefaultSfxVolOrFreq, &gDefaultSfxReverbAdd);
    } else {
        Audio_PlaySfxGeneral(sfxId, pos, 4, &gDefaultSfxVolOrFreq, &gDefaultSfxVolOrFreq, &gUnderwaterSfxReverbAdd);
    }
}

/**
 * Used only for eating the goron sirloin by the goron with Don Gero's Mask
 */
void Audio_PlaySfxWithSfxSettingsReverb(Vec3f* pos, u16 sfxId) {
    SfxSettings* sfxSettings;

    if ((sfxId == NA_SE_EN_KONB_JUMP_OLD) || (sfxId == NA_SE_EN_KONB_SINK_OLD)) {
        Audio_PlaySfxGeneral(sfxId, pos, 4, &gDefaultSfxVolOrFreq, &gDefaultSfxVolOrFreq, &gDefaultSfxReverbAdd);
    } else {
        sfxSettings = Audio_AddSfxSetting(pos);

        if (sfxSettings != NULL) {
            Audio_PlaySfxGeneral(sfxId, pos, 4, &gDefaultSfxVolOrFreq, &gDefaultSfxVolOrFreq, &sfxSettings->reverbAdd);
        }
    }
}

void Audio_ToggleUnderwaterReverb(s8 isUnderwaterReverbActivated) {
    if (isUnderwaterReverbActivated) {
        gUnderwaterSfxReverbAdd = -0x80;
    } else {
        gUnderwaterSfxReverbAdd = 0;
    }
}

void Audio_LowerSfxSettingsReverb(Vec3f* pos, s8 isReverbLowered) {
    SfxSettings* sfxSettings = Audio_AddSfxSetting(pos);

    if (sfxSettings != NULL) {
        if (isReverbLowered) {
            sfxSettings->reverbAdd = -0x80;
        } else {
            sfxSettings->reverbAdd = 0;
        }
    }
}

// OoT func_800F3F84
f32 Audio_SetSyncedSfxFreqAndVolume(f32 arg0) {
    f32 ret = 1.0f;

    if (arg0 > 6.0f) {
        sSyncedSfxVolume = 1.0f;
        sSyncedSfxFreq = 1.1f;
    } else {
        ret = arg0 / 6.0f;
        sSyncedSfxVolume = (ret * 0.22500002f) + 0.775f;
        sSyncedSfxFreq = (ret * 0.2f) + 0.9f;
    }

    return ret;
}

// OoT func_800F4010
/**
 * Adjusts both frequency and volume based on a single parameter "freqVolParam"
 * When freqVolParam >= 6.0f, frequency is increased to 1.1f and volume remains fixed at 1.0f
 * For every -1.0f taken from freqVolParam (eg. 5.0f, 4.0f, 3.0f...):
 *     - volume will decrease by 0.0375f
 *     - frequency will decrease by 0.0333333f
 */
void Audio_PlaySfxAtPosForMetalEffectsWithSyncedFreqAndVolume(Vec3f* pos, u16 sfxId, f32 freqVolParam) {
    f32 sp2C;
    f32 phi_f0;
    s32 phi_v0;
    u16 metalSfxId = 0;

    sp2C = Audio_SetSyncedSfxFreqAndVolume(freqVolParam);
    Audio_PlaySfxGeneral(sfxId, pos, 4, &sSyncedSfxFreq, &sSyncedSfxVolume, &gDefaultSfxReverbAdd);

    if ((sfxId & 0xF0) == 0xB0) {
        phi_f0 = 0.3f;
        phi_v0 = true;
        sp2C = 1.0f;
    } else {
        phi_f0 = 1.1f;
        phi_v0 = gAudioContext.audioRandom & 1 & 0xFF;
    }

    if (phi_f0 < freqVolParam) {
        if (phi_v0) {
            if ((sfxId & 0x1FF) < 0x80) {
                metalSfxId = NA_SE_PL_METALEFFECT_KID;
            } else if ((sfxId & 0x1FF) < 0xF0) {
                metalSfxId = NA_SE_PL_METALEFFECT_ADULT;
            }

            if (metalSfxId != 0) {
                sSyncedSfxVolumeForMetalEffects = (sp2C * 0.7) + 0.3;
                Audio_PlaySfxGeneral(metalSfxId, pos, 4, &sSyncedSfxFreq, &sSyncedSfxVolumeForMetalEffects,
                                     &gDefaultSfxReverbAdd);
            }
        }
    }
}

// OoT func_800F4138
/**
 * Adjusts both frequency and volume based on a single parameter "freqVolParam"
 * When freqVolParam >= 6.0f, frequency is increased to 1.1f and volume remains fixed at 1.0f
 * For every -1.0f taken from freqVolParam (eg. 5.0f, 4.0f, 3.0f...):
 *     - volume will decrease by 0.0375f
 *     - frequency will decrease by 0.0333333f
 */
void Audio_PlaySfxAtPosWithSyncedFreqAndVolume(Vec3f* pos, u16 sfxId, f32 freqVolParam) {
    Audio_SetSyncedSfxFreqAndVolume(freqVolParam);
    Audio_PlaySfxGeneral(sfxId, pos, 4, &sSyncedSfxFreq, &sSyncedSfxVolume, &gDefaultSfxReverbAdd);
}

void Audio_PlaySfxForGiantsMaskUnused(Vec3f* pos, u16 sfxId) {
    Audio_PlaySfxGeneral(sfxId | 0xE0, pos, 4, &sGiantsMaskFreq, &gDefaultSfxVolOrFreq, &sGiantsMaskReverbAdd);
}

void Audio_PlaySfxForGiantsMask(Vec3f* pos, u16 sfxId) {
    Audio_PlaySfxGeneral((sfxId & 0x681F) + 0x20, pos, 4, &sGiantsMaskFreq, &gDefaultSfxVolOrFreq,
                         &sGiantsMaskReverbAdd);
}

void Audio_PlaySfxRandomized(Vec3f* pos, u16 baseSfxId, u8 randLim) {
    u8 offset = Audio_NextRandom() % randLim;

    Audio_PlaySfxGeneral(baseSfxId + offset, pos, 4, &gDefaultSfxVolOrFreq, &gDefaultSfxVolOrFreq,
                         &gDefaultSfxReverbAdd);
}

// OoT func_800F4254
/**
 * Plays increasingly high-pitched sword charging sfx as Player charges up the sword
 */
void Audio_PlaySfxForSwordCharge(Vec3f* pos, u8 chargeLevel) {
    chargeLevel &= 3;
    if (chargeLevel != sPrevChargeLevel) {
        sCurChargeLevelSfxFreq = sChargeLevelsSfxFreq[chargeLevel];
        switch (chargeLevel) {
            case 1:
                Audio_PlaySfxGeneral(NA_SE_PL_SWORD_CHARGE, pos, 4, &sCurChargeLevelSfxFreq, &gDefaultSfxVolOrFreq,
                                     &gDefaultSfxReverbAdd);
                break;
            case 2:
                Audio_PlaySfxGeneral(NA_SE_PL_SWORD_CHARGE, pos, 4, &sCurChargeLevelSfxFreq, &gDefaultSfxVolOrFreq,
                                     &gDefaultSfxReverbAdd);
                break;
        }
        sPrevChargeLevel = chargeLevel;
    }

    if (chargeLevel != 0) {
        Audio_PlaySfxGeneral(NA_SE_IT_SWORD_CHARGE - SFX_FLAG, pos, 4, &sCurChargeLevelSfxFreq, &gDefaultSfxVolOrFreq,
                             &gDefaultSfxReverbAdd);
    }
}

// OoT func_800F436C
void Audio_PlaySfxAtPosWithFreqAndVolume(Vec3f* pos, u16 sfxId, f32 freqScale, f32* volume) {
    SfxSettings* sfxSettings = Audio_AddSfxSetting(pos);
    f32* freqScaleAdj;

    if (sfxSettings != NULL) {
        freqScaleAdj = &sfxSettings->freqScale;
        if (freqScale < 0.75f) {
            *freqScaleAdj = ((freqScale / 0.75f) * 0.25f) + 0.5f;
        } else {
            *freqScaleAdj = freqScale;
        }

        if (*freqScaleAdj > 0.5f) {
            Audio_PlaySfxGeneral(sfxId, pos, 4, freqScaleAdj, volume, &gDefaultSfxReverbAdd);
        }
    }
}

void Audio_PlaySfxAtPosWithFreq(Vec3f* pos, u16 sfxId, f32 freqScale) {
    Audio_PlaySfxAtPosWithFreqAndVolume(pos, sfxId, freqScale, &gDefaultSfxVolOrFreq);
}

void Audio_PlaySfxAtPosWithFreqAndSoundScriptIO(Vec3f* pos, u16 sfxId, f32 freqScale, u8 arg3) {
    if (freqScale > 1.0f) {
        freqScale = 1.0f;
    }

    Audio_SetSfxChannelIO(pos, sfxId, (arg3 - (u32)(freqScale * arg3)) & 0xFF);
    Audio_PlaySfxAtPosWithFreq(pos, sfxId, freqScale);
}

void Audio_PlaySfxForWaterWheel(Vec3f* pos, u16 sfxId) {
    u8 isWaterWheelSfxNotPlaying = false;

    switch (sfxId) {
        case NA_SE_EV_DUMMY_WATER_WHEEL_LR - SFX_FLAG:
            if (!Audio_IsSfxPlaying(NA_SE_EV_BIG_WATER_WHEEL_LR - SFX_FLAG)) {
                isWaterWheelSfxNotPlaying = true;
            }
            break;
        case NA_SE_EV_DUMMY_WATER_WHEEL_RR - SFX_FLAG:
            if (!Audio_IsSfxPlaying(NA_SE_EV_BIG_WATER_WHEEL_RR - SFX_FLAG)) {
                isWaterWheelSfxNotPlaying = true;
            }
            break;
    }

    if (isWaterWheelSfxNotPlaying) {
        Audio_SetSfxChannelIO(pos, sfxId, 0);
        Audio_PlaySfxAtPosWithFreqAndVolume(pos, sfxId, 1.0f, &sWaterWheelVolume);
    }
}

// OoT func_800F4414
/**
 * at timerShiftedLerp == 1: use sSfxTimerLerpRange1
 * at timerShiftedLerp == 2: use sSfxTimerLerpRange2
 *
 * sAdjustedSfxFreq was modified in OoT, but remains 1.0f in MM
 *
 * Used for "NA_SE_IT_DEKUNUTS_FLOWER_ROLL" and "NA_SE_IT_FISHING_REEL_SLOW"
 */
void Audio_PlaySfxAtPosWithTimer(Vec3f* pos, u16 sfxId, f32 timerShiftedLerp) {
    sSfxTimer--;
    if (sSfxTimer == 0) {
        Audio_PlaySfxGeneral(sfxId, pos, 4, &sAdjustedSfxFreq, &gDefaultSfxVolOrFreq, &gDefaultSfxReverbAdd);

        if (timerShiftedLerp > 2.0f) {
            timerShiftedLerp = 2.0f;
        }

        // Linear interpolation between "sSfxTimerLerpRange1" and "sSfxTimerLerpRange2" from lerp factor
        // (timerShiftedLerp - 1.0f)
        sSfxTimer = (s8)((sSfxTimerLerpRange1 - sSfxTimerLerpRange2) * (1.0f - timerShiftedLerp)) + sSfxTimerLerpRange1;
    }
}

// OoT func_800F44EC
void Audio_SetSfxTimerLerpInterval(s8 timerLerpRange1, s8 timerLerpRange2) {
    sSfxTimer = 1;
    sSfxTimerLerpRange2 = timerLerpRange2;
    sSfxTimerLerpRange1 = timerLerpRange1;
}

// OoT func_800F4524
void Audio_PlaySfxAtPosWithReverb(Vec3f* pos, u16 sfxId, s8 reverbAdd) {
    D_801FD28C = reverbAdd;
    Audio_PlaySfxGeneral(sfxId, pos, 4, &gDefaultSfxVolOrFreq, &gDefaultSfxVolOrFreq, &D_801FD28C);
}

// OoT func_800F4578
void Audio_PlaySfxAtPosWithVolume(Vec3f* pos, u16 sfxId, f32 volume) {
    sSfxVolume = volume;
    Audio_PlaySfxGeneral(sfxId, pos, 4, &gDefaultSfxVolOrFreq, &sSfxVolume, &gDefaultSfxReverbAdd);
}

void Audio_SetSfxVolumeTransition(f32* volume, f32 volumeTarget, u16 duration) {
    sSfxVolumeCur = volume;
    sSfxVolumeTarget = volumeTarget;
    sSfxVolumeDuration = duration;
    sSfxVolumeRate = (*sSfxVolumeCur - sSfxVolumeTarget) / sSfxVolumeDuration;
}

// Part of audio update (runs every frame)
void Audio_UpdateSfxVolumeTransition(void) {
    if (sSfxVolumeDuration != 0) {
        sSfxVolumeDuration--;
        if (sSfxVolumeDuration == 0) {
            *sSfxVolumeCur = sSfxVolumeTarget;
        } else {
            *sSfxVolumeCur -= sSfxVolumeRate;
        }
    }
}

// OoT func_800F45D0
void Audio_PlaySfxForFishingReel(f32 timerShiftedLerp) {
    Audio_PlaySfxAtPosWithTimer(&gDefaultSfxPos, NA_SE_IT_FISHING_REEL_SLOW - SFX_FLAG, timerShiftedLerp);
    Audio_PlaySfxAtPosWithFreq(&gDefaultSfxPos, 0, (0.15f * timerShiftedLerp) + 1.4f);
}

/**
 * Used for EnRiverSound
 */
void Audio_PlaySfxForRiver(Vec3f* pos, f32 freqScale) {
    if (!Audio_IsSfxPlaying(NA_SE_EV_RIVER_STREAM - SFX_FLAG)) {
        sRiverFreqScaleLerp.value = freqScale;
    } else if (freqScale != sRiverFreqScaleLerp.value) {
        sRiverFreqScaleLerp.target = freqScale;
        sRiverFreqScaleLerp.remainingFrames = 40;
        sRiverFreqScaleLerp.step = (sRiverFreqScaleLerp.target - sRiverFreqScaleLerp.value) / 40;
    }
    Audio_PlaySfxGeneral(NA_SE_EV_RIVER_STREAM - SFX_FLAG, pos, 4, &sRiverFreqScaleLerp.value, &gDefaultSfxVolOrFreq,
                         &gDefaultSfxReverbAdd);
}

/**
 * Unused remnant of OoT's Zora's River Waterfall
 */
void Audio_PlaySfxForWaterfall(Vec3f* pos, f32 freqScale) {
    if (!Audio_IsSfxPlaying(NA_SE_EV_WATER_WALL_BIG - SFX_FLAG)) {
        sWaterfallFreqScaleLerp.value = freqScale;
    } else if (freqScale != sWaterfallFreqScaleLerp.value) {
        sWaterfallFreqScaleLerp.target = freqScale;
        sWaterfallFreqScaleLerp.remainingFrames = 40;
        sWaterfallFreqScaleLerp.step = (sWaterfallFreqScaleLerp.target - sWaterfallFreqScaleLerp.value) / 40;
    }
    Audio_PlaySfxGeneral(NA_SE_EV_WATER_WALL_BIG - SFX_FLAG, pos, 4, &sWaterfallFreqScaleLerp.value,
                         &sWaterfallFreqScaleLerp.value, &gDefaultSfxReverbAdd);
}

/**
 * Used for EnRiverSound variables
 */
void Audio_StepFreqLerp(FreqLerp* lerp) {
    if (lerp->remainingFrames != 0) {
        lerp->remainingFrames--;
        if (lerp->remainingFrames != 0) {
            lerp->value += lerp->step;
        } else {
            lerp->value = lerp->target;
        }
    }
}

void Audio_PlaySfxForBigBells(Vec3f* pos, u8 arg1) {
    static f32 sBigBellsVolume[8] = {
        1.0f, 0.9f, 0.8f, 0.7f, 0.6f, 0.5f, 0.4f, 0.3f,
    };
    Audio_PlaySfxGeneral(NA_SE_EV_SIGNAL_BIGBELL, pos, 4, &gDefaultSfxVolOrFreq, &sBigBellsVolume[arg1 & 7],
                         &gDefaultSfxReverbAdd);
}

// OoT func_800F47BC
void Audio_SetBgmVolumeOff(void) {
    Audio_SetVolumeScale(SEQ_PLAYER_BGM_MAIN, 1, 0, 10);
    Audio_SetVolumeScale(SEQ_PLAYER_BGM_SUB, 1, 0, 10);
}

// OoT func_800F47FC
void Audio_SetBgmVolumeOn(void) {
    Audio_SetVolumeScale(SEQ_PLAYER_BGM_MAIN, 1, 0x7F, 3);
    Audio_SetVolumeScale(SEQ_PLAYER_BGM_SUB, 1, 0x7F, 3);
}

void func_801A0204(s8 seqId) {
    Audio_QueueCmdS8(MK_AUDIO_CMD(0x46, 0, 0, 2), (u8)seqId);
}

// OoT func_800F483C
void Audio_SetMainBgmVolume(u8 targetVolume, u8 volumeFadeTimer) {
    Audio_SetVolumeScale(SEQ_PLAYER_BGM_MAIN, 0, targetVolume, volumeFadeTimer);
}

/**
 * Unused remnant from OoT
 * Was designed to incrementally increase volume of NA_BGM_GANON_TOWER for each new room during the climb of Ganon's Tower
 */
void Audio_SetGanonsTowerBgmVolumeLevel(u8 ganonsTowerLevel) {
    u8 channelIdx;
    s8 pan = 0;

    // Ganondorfs's Lair
    if (ganonsTowerLevel == 0) {
        pan = 0x7F;
    }

    for (channelIdx = 0; channelIdx < 16; channelIdx++) {
        // CHAN_UPD_PAN_UNSIGNED
        Audio_QueueCmdS8(((u8)(u32)channelIdx << 8) | 0x7000000, pan);
    }

    // Lowest room in Ganon's Tower (Entrance Room)
    if (ganonsTowerLevel == 7) {
        // Adds a delay to setting the volume in the first room
        sEnterGanonsTowerTimer = 2;
    } else {
        Audio_SetGanonsTowerBgmVolume(sGanonsTowerLevelsVol[ganonsTowerLevel % ARRAY_COUNTU(sGanonsTowerLevelsVol)]);
    }
}

/**
 * Unused remnant from OoT's EnRiverSound
 * If a new volume is requested for ganon's tower, update the volume and
 * calculate a new low-pass filter cutoff and reverb based on the new volume
 */
s32 Audio_SetGanonsTowerBgmVolume(u8 targetVolume) {
    u8 lowPassFilterCutoff;
    u8 channelIdx;
    u16 reverb;

    if (sGanonsTowerVol != targetVolume) {
        // Sets the volume
        Audio_SetVolumeScale(SEQ_PLAYER_BGM_MAIN, 0, targetVolume, 2);

        // Sets the filter cutoff of the form (lowPassFilterCutoff << 4) | (highPassFilter & 0xF). highPassFilter is
        // always set to 0
        if (targetVolume < 0x40) {
            // Only the first room
            lowPassFilterCutoff = 0x10;
        } else {
            // Higher volume leads to a higher cut-off frequency in the low-pass filtering
            lowPassFilterCutoff = (((targetVolume - 0x40) >> 2) + 1) << 4;
        }

        AudioSeqCmd_SetChannelIO(SEQ_PLAYER_BGM_MAIN, 4, 15, lowPassFilterCutoff);

        // Sets the reverb
        for (channelIdx = 0; channelIdx < ARRAY_COUNT(gAudioContext.seqPlayers[SEQ_PLAYER_BGM_MAIN].channels); channelIdx++) {
            if (&gAudioContext.sequenceChannelNone != gAudioContext.seqPlayers[SEQ_PLAYER_BGM_MAIN].channels[channelIdx]) {
                // soundScriptIO[5] was set to 0x40 in channels 0, 1, and 4 (BGM no longer in OoT)
                if ((u8)gAudioContext.seqPlayers[SEQ_PLAYER_BGM_MAIN].channels[channelIdx]->soundScriptIO[5] != 0xFF) {
                    // Higher volume leads to lower reverb
                    reverb = (((u16)gAudioContext.seqPlayers[SEQ_PLAYER_BGM_MAIN].channels[channelIdx]->soundScriptIO[5] - targetVolume) + 0x7F);

                    if (reverb > 0x7F) {
                        reverb = 0x7F;
                    }

                    // CHAN_UPD_REVERB
                    Audio_QueueCmdS8(_SHIFTL(5, 24, 8) | _SHIFTL(SEQ_PLAYER_BGM_MAIN, 16, 8) | _SHIFTL(channelIdx, 8, 8), (u8)reverb);
                }
            }
        }

        sGanonsTowerVol = targetVolume;
    }
    return -1;
}

/**
 * Unused remnant from OoT's EnRiverSound
 * Responsible for lowering market bgm in Child Market Entrance and Child Market Back Alley
 * Only lowers volume for 1 frame, so must be called every frame to maintain lower volume
 */
void Audio_LowerMainBgmVolume(u8 volume) {
    sRiverSoundMainBgmVol = volume;
    sRiverSoundMainBgmLower = true;
}

/**
 * Remnant of OoT's EnRiverSound. Still called by Audio_Update every frame, but none of these processes get initialized
 */
void Audio_UpdateRiverSoundVolumes(void) {
    // Updates Main Bgm Volume (RiverSound of type RS_LOWER_MAIN_BGM_VOLUME)
    if (sRiverSoundMainBgmLower == true) {
        if (sRiverSoundMainBgmCurrentVol != sRiverSoundMainBgmVol) {
            // lowers the volume for 1 frame
            Audio_SetVolumeScale(SEQ_PLAYER_BGM_MAIN, 0, sRiverSoundMainBgmVol, 10);
            sRiverSoundMainBgmCurrentVol = sRiverSoundMainBgmVol;
            sRiverSoundMainBgmRestore = true;
        }
        sRiverSoundMainBgmLower = false;
    } else if ((sRiverSoundMainBgmRestore == true) && !sAudioIsWindowOpen) {
        // restores the volume every frame
        Audio_SetVolumeScale(SEQ_PLAYER_BGM_MAIN, 0, 0x7F, 10);
        sRiverSoundMainBgmCurrentVol = 0x7F;
        sRiverSoundMainBgmRestore = false;
    }

    // Update Ganon's Tower Volume (RiverSound of type RS_GANON_TOWER_7)
    if (sEnterGanonsTowerTimer != 0) {
        sEnterGanonsTowerTimer--;
        if (sEnterGanonsTowerTimer == 0) {
            Audio_SetGanonsTowerBgmVolume(sGanonsTowerLevelsVol[7]);
        }
    }
}

// Unused remnant of OoT
void Audio_PlaySfxIncreasinglyTransposed(Vec3f* pos, s16 sfxId, u8* semitones) {
    Audio_PlaySfxGeneral(sfxId, pos, 4, &gNoteFrequencies[semitones[sAudioIncreasingTranspose] + 39],
                         &gDefaultSfxVolOrFreq, &gDefaultSfxReverbAdd);
    if (sAudioIncreasingTranspose < 15) {
        sAudioIncreasingTranspose++;
    }
}

// Unused remnant of OoT
void Audio_ResetIncreasingTranspose(void) {
    sAudioIncreasingTranspose = 0;
}

// Unused remnant of OoT
void Audio_PlaySfxTransposed(Vec3f* pos, u16 sfxId, s8 semitone) {
    Audio_PlaySfxGeneral(sfxId, pos, 4, &gNoteFrequencies[semitone + 39], &gDefaultSfxVolOrFreq, &gDefaultSfxReverbAdd);
}

// OoT func_800F4C58
void Audio_SetSfxChannelIO(Vec3f* pos, u16 sfxId, u8 val) {
    u8 channelIdx = 0;
    u8 bankId = SFX_BANK_SHIFT(sfxId);
    u8 entryIndex;
    u8 i;

    for (i = 0; i < bankId; i++) {
        channelIdx += gChannelsPerBank[gSfxChannelLayout][i];
    }

    for (i = 0; i < gChannelsPerBank[gSfxChannelLayout][bankId]; i++) {
        entryIndex = gActiveSfxs[bankId][i].entryIndex;

        if (entryIndex != 0xFF) {
            if ((sfxId == gSfxBanks[bankId][entryIndex].sfxId) && (&pos->x == gSfxBanks[bankId][entryIndex].posX)) {
                Audio_QueueCmdS8(_SHIFTL(6, 24, 8) | _SHIFTL(SEQ_PLAYER_SFX, 16, 8) | _SHIFTL(channelIdx, 8, 8) | _SHIFTL(6, 0, 8),
                                 val);
            }
        }
        channelIdx++;
    }
}

/**
 * Plays sfx and sets val to io port 6 if the sfx is active
 */
void Audio_PlaySfxAtPosWithChannelIO(Vec3f* pos, u16 sfxId, u8 val) {
    Audio_SetSfxChannelIO(pos, sfxId, val);
    Audio_PlaySfxGeneral(sfxId, pos, 4, &gDefaultSfxVolOrFreq, &gDefaultSfxVolOrFreq, &gDefaultSfxReverbAdd);
}

/**
 * Plays sfx and sets val to io port 6
 */
// OoT func_800F4C58
void Audio_PlaySfxAtPosWithAllChannelsIO(Vec3f* pos, u16 sfxId, u8 val) {
    u8 channelIdx = 0;
    u8 i;
    u8 bankId = SFX_BANK_SHIFT(sfxId);

    for (i = 0; i < bankId; i++) {
        channelIdx += gChannelsPerBank[gSfxChannelLayout][i];
    }

    for (i = 0; i < gChannelsPerBank[gSfxChannelLayout][bankId]; i++) {
        Audio_QueueCmdS8(_SHIFTL(6, 24, 8) | _SHIFTL(SEQ_PLAYER_SFX, 16, 8) | _SHIFTL(channelIdx++, 8, 8) | _SHIFTL(6, 0, 8), val);
    }

    Audio_PlaySfxGeneral(sfxId, pos, 4, &gDefaultSfxVolOrFreq, &gDefaultSfxVolOrFreq, &gDefaultSfxReverbAdd);
}

/**
 * Unused remnant of OoT's EnRiverSound (func_800F4E30)
 */
void func_801A09D4(Vec3f* pos, f32 arg1) {
    f32 phi_f22;
    s8 phi_s4;
    u8 i;

    if (sSariaBgmPtr == NULL) {
        sSariaBgmPtr = pos;
        D_801D66F8 = arg1;
    } else if (pos != sSariaBgmPtr) {
        if (arg1 < D_801D66F8) {
            sSariaBgmPtr = pos;
            D_801D66F8 = arg1;
        }
    } else {
        D_801D66F8 = arg1;
    }

    if (sSariaBgmPtr->x > 100.0f) {
        phi_s4 = 0x7F;
    } else if (sSariaBgmPtr->x < -100.0f) {
        phi_s4 = 0;
    } else {
        phi_s4 = ((sSariaBgmPtr->x / 100.0f) * 64.0f) + 64.0f;
    }

    if (D_801D66F8 > 400.0f) {
        phi_f22 = 0.1f;
    } else if (D_801D66F8 < 120.0f) {
        phi_f22 = 1.0f;
    } else {
        phi_f22 = ((1.0f - ((D_801D66F8 - 120.0f) / 280.0f)) * 0.9f) + 0.1f;
    }

    for (i = 0; i < 0x10; i++) {
        if (i != 9) {
            AudioSeqCmd_SetChannelVol(SEQ_PLAYER_BGM_MAIN, 2, i, (127.0f * phi_f22));
            Audio_QueueCmdS8(0x03000000 | ((u8)((u32)i) << 8), phi_s4);
        }
    }
}

/**
 * Unused remnant of OoT's EnRiverSound
 */
void Audio_ClearSariaBgm(void) {
    if (sSariaBgmPtr != NULL) {
        sSariaBgmPtr = NULL;
    }
}

/**
 * Unused remnant of OoT's EnRiverSound
 */
void Audio_ClearSariaBgmAtPos(Vec3f* pos) {
    if (sSariaBgmPtr == pos) {
        sSariaBgmPtr = NULL;
    }
}

/**
 * Turns on and off channels from both bgm players in a way that splits
 * equally between the two bgm channels. Split based on note priority
 */
void Audio_SplitBgmChannels(s8 volumeSplit) {
    u8 volume;
    u8 notePriority;
    u16 channelBits;
    u8 bgmPlayers[2] = { SEQ_PLAYER_BGM_MAIN, SEQ_PLAYER_BGM_SUB };
    u8 i;
    u8 channelIdx;

    if ((Audio_GetActiveSequence(SEQ_PLAYER_FANFARE) == NA_BGM_DISABLED) && (Audio_GetActiveSequence(SEQ_PLAYER_BGM_SUB) != NA_BGM_ROMANI_RANCH)) {
        for (i = 0; i < 2; i++) {
            if (i == 0) {
                // Main Bgm SeqPlayer
                volume = volumeSplit;
            } else {
                // Sub Bgm SeqPlayer
                volume = 0x7F - volumeSplit;
            }

            if (volume > 100) {
                notePriority = 11;
            } else if (volume < 20) {
                notePriority = 2;
            } else {
                notePriority = ((volume - 20) / 10) + 2;
            }

            channelBits = 0;
            for (channelIdx = 0; channelIdx < ARRAY_COUNT(gAudioContext.seqPlayers[bgmPlayers[i]].channels); channelIdx++) {
                if (gAudioContext.seqPlayers[bgmPlayers[i]].channels[channelIdx]->notePriority < notePriority) {
                    // If the note currently playing in the channel is a high enough priority,
                    // then keep the channel on by setting a channelBit
                    // If this condition fails, then the channel will be shut off
                    channelBits += (1 << channelIdx);
                }
            }

            AudioSeqCmd_SetActiveChannels(bgmPlayers[i], channelBits);
        }
    }
}

void Audio_SetSequenceProperties(u8 playerIdx, Vec3f* pos, s16 flags, f32 minDist, f32 maxDist, f32 maxVolume, f32 minVolume) {
    f32 dist;
    f32 volume;
    s8 sp27;
    s8 sp26;
    s32 pad;

    // calculating pan? from z-position
    if (pos->z > 0.0f) {
        if (pos->z > 100.0f) {
            sp27 = 0;
        } else {
            sp27 = ((100.0f - pos->z) / 100.0f) * 64.0f;
        }
    } else {
        if (pos->z < -100.0f) {
            sp27 = 0x7F;
        } else {
            sp27 = (s8)((-pos->z / 100.0f) * 64.0f) + 0x3F;
        }
    }

    // calculating pan from x-position
    if (pos->x > 0.0f) {
        if (pos->x > 200.0f) {
            sp26 = 0x6C;
        } else {
            sp26 = (s8)((pos->x / 200.0f) * 45.0f) + 0x3F;
        }
    } else {
        if (pos->x < -200.0f) {
            sp26 = 0x14;
        } else {
            sp26 = (s8)(((pos->x + 200.0f) / 200.0f) * 44.0f) + 0x14;
        }
    }

    // Calculate volume
    dist = sqrtf(SQ(pos->z) + ((SQ(pos->x) * 0.25f) + (SQ(pos->y) / 6.0f)));

    if (dist > maxDist) {
        volume = minVolume;
    } else if (dist < minDist) {
        volume = 1.0f;
    } else {
        volume = (((maxDist - dist) / (maxDist - minDist)) * (1.0f - minVolume)) + minVolume;
    }

    Audio_QueueCmdU16(playerIdx << 0x10 | 0x90000000, 0xFFFF);

    if (flags & 1) {
        Audio_QueueCmdS8(playerIdx << 0x10 | 0x12000000 | 0xFF00, sp27);
    }

    // CHAN_UPD_PAN_SIGNED
    if (flags & 2) {
        Audio_QueueCmdS8(playerIdx << 0x10 | 0x3000000 | 0xFF00, sp26);
    }

    // CHAN_UPD_PAN_UNSIGNED
    if (flags & 4) {
        Audio_QueueCmdS8(playerIdx << 0x10 | 0x7000000 | 0xFF00, 0x7F);
    }

    // applies filter, stores result in sSequenceFilter
    if (flags & 8) {
        // Uses new filter sBandPassFilterData, loads it into 
        // Then channel->filter points to sBandPassFilterData
        // AudioHeap_LoadFilter(((u32)&D_801FD3B4[SEQ_PLAYER_NATURE] & ~0xF) + 0x10, 5, 4);
        // ALIGN16(sSequenceFilter)
        Audio_QueueCmdS32(playerIdx << 0x10 | 0x13000000 | 0xFF00 | 0x54, ((u32)&sSequenceFilter[0] & ~0xF) + 0x10);
    } else {
        // Identity Filter
        // AudioHeap_LoadFilter(((u32)&D_801FD3B4[SEQ_PLAYER_NATURE] & ~0xF) + 0x10, 0, 0);
        Audio_QueueCmdS32(playerIdx << 0x10 | 0x13000000 | 0xFF00, ((u32)&sSequenceFilter[0] & ~0xF) + 0x10);
    }

    if (flags & 0x10) {
        Audio_QueueCmdS8(playerIdx << 0x10 | 0x14000000 | 0xFF00, 0x7F);
    } else {
        Audio_QueueCmdS8(playerIdx << 0x10 | 0x14000000 | 0xFF00, 0);
    }

    // CHAN_UPD_VOL_SCALE
    if (flags & 0x20) {
        Audio_QueueCmdF32(playerIdx << 0x10 | 0x1000000 | 0xFF00, maxVolume * volume);
    }

    // CHAN_UPD_PAN_UNSIGNED
    if (flags & 0x40) {
        Audio_QueueCmdS8(playerIdx << 0x10 | 0x7000000 | 0xFF00, 0x40);
    }

    if (flags & 0x80) {
        Audio_QueueCmdS8(playerIdx << 0x10 | 0x11000000 | 0xFF00, 1);
    }

    // CHAN_UPD_REVERB
    if (flags & 0x100) {
        Audio_QueueCmdS8(playerIdx << 0x10 | 0x5000000 | 0xFF00, 0x37);
    }
}

// ======== BEGIN Z_OBJ_SOUND FUNCTIONS ========

// Part of audio update (runs every frame), related to z_obj_sound
/**
 * Used for Main Bgm and Fanfares
 */
void Audio_UpdateSequenceAtFixedPos(void) {
    if (sSeqAtFixedPosPlayerIdx != SEQ_PLAYER_INVALID) {
        if ((Audio_GetActiveSequence(sSeqAtFixedPosPlayerIdx) != NA_BGM_FINAL_HOURS) &&
            Audio_IsSeqCmdNotQueued((sSeqAtFixedPosPlayerIdx << 24) + NA_BGM_FINAL_HOURS, 0xFF0000FF) && !sIsFinalHoursOrSoaring) {
            Audio_SetSequenceProperties(sSeqAtFixedPosPlayerIdx, &sSeqAtFixedPosPos, sSeqAtFixedPosFlags, sSeqAtFixedPosMinDist, sSeqAtFixedPosMaxDist, sSeqAtFixedPosMaxVol, sSeqAtFixedPosMinVol);
        }
        sSeqAtFixedPosPlayerIdx = SEQ_PLAYER_INVALID;
    }
}

// related to z_obj_sound
/**
 * Used for Main Bgm and Fanfares
 */
void Audio_SetSequenceAtFixedPosParams(u8 playerIdx, Vec3f* pos, s16 flags, f32 minDist, f32 maxDist, f32 maxVolume, f32 minVolume) {
    sSeqAtFixedPosPlayerIdx = playerIdx;
    sSeqAtFixedPosPos.x = pos->x;
    sSeqAtFixedPosPos.y = pos->y;
    sSeqAtFixedPosPos.z = pos->z;
    sSeqAtFixedPosFlags = flags;
    sSeqAtFixedPosMinDist = minDist;
    sSeqAtFixedPosMaxDist = maxDist;
    sSeqAtFixedPosMaxVol = maxVolume;
    sSeqAtFixedPosMinVol = minVolume;
}

// related to z_obj_sound
/**
 * 
 */
void Audio_StartFanfareAtFixedPos(u8 playerIdx, Vec3f* pos, s8 seqId, u16 seqIdOffset) {
    s32 pad[3];
    u32 mask;

    if ((Audio_GetActiveSequence(playerIdx) == NA_BGM_FINAL_HOURS) || !Audio_IsSeqCmdNotQueued((playerIdx << 0x18) + NA_BGM_FINAL_HOURS, 0xFF0000FF) ||
        sIsFinalHoursOrSoaring) {
        sIsFinalHoursOrSoaring = true;
    } else if (pos != NULL) {
        if ((seqId != (s8)(Audio_GetActiveSequence(playerIdx) & 0xFFFF)) && !gAudioContext.seqPlayers[playerIdx].enabled &&
            (sSeqAtFixedPosSeqId == NA_BGM_GENERAL_SFX)) {

            mask = 0xFFFF;

            // AudioSeqCmd_StartSequence(playerIdx, ((((Audio_NextRandom() % 0x1E) & 0xFF) + 1)), ((seqId & mask) + seqIdOffset));
            Audio_QueueSeqCmd((playerIdx << 0x18) | ((u32)(((Audio_NextRandom() % 0x1E) & 0xFF) + 1) << 0x10) | (u32)((seqId & mask) + seqIdOffset));
            sSeqAtFixedPosSeqId = seqId;
        }

        Audio_SetSequenceAtFixedPosParams(playerIdx, pos, 0x7F, 320.0f, 1280.0f, 1.0f, 0.0f);
    } else {
        AudioSeqCmd_StopSequence(playerIdx, 5);
    }
}

/**
 * z_obj_sound
 *    - NA_BGM_SHOP
 *    - NA_BGM_MINI_GAME
 *    - NA_BGM_MILK_BAR
 *    - NA_BGM_MILK_BAR_DUPLICATE
 *    - NA_BGM_ASTRAL_OBSERVATORY
 */  

void Audio_PlayMainBgmAtFixedPos(Vec3f* pos, s8 seqId) {
    s32 pad[2];
    u16 sp36;
    s32 sp2C;
    u16 seqId0 = Audio_GetActiveSequence(SEQ_PLAYER_BGM_MAIN);
    u32 temp_a0;

    if ((seqId0 == NA_BGM_FINAL_HOURS) || !Audio_IsSeqCmdNotQueued(NA_BGM_FINAL_HOURS, 0xFF0000FF) || sIsFinalHoursOrSoaring) {
        sIsFinalHoursOrSoaring = true;
        return;
    }

    if (seqId0 == NA_BGM_SONG_OF_SOARING) {
        sIsFinalHoursOrSoaring = true;
    }

    if (pos != NULL) {
        if (seqId == NA_BGM_ASTRAL_OBSERVATORY) {

            if ((seqId != (u8)(seqId0 & 0xFF)) && !sAllPlayersMutedExceptOcaAndSys) {
                AudioSeqCmd_StartSequence(SEQ_PLAYER_BGM_MAIN, 0, (u16)seqId);
                sSeqAtFixedPosSeqId = seqId;
            } else if ((seqId == (u8)(seqId0 & 0xFF)) && (sSeqAtFixedPosSeqId == NA_BGM_GENERAL_SFX)) {
                sSeqAtFixedPosSeqId = seqId;
            }

            Audio_SetSequenceAtFixedPosParams(SEQ_PLAYER_BGM_MAIN, pos, 0x20, 100.0f, 1500.0f, 0.9f, 0.0f);
        } else {
            if (sSeqAtFixedPosSeqId == NA_BGM_GENERAL_SFX) {
                temp_a0 = ((((Audio_NextRandom() % 0x1E) & 0xFF) + 1) << 0x10) | ((u16)seqId + 0x7F00);
                AudioSeqCmd_StartSequence(SEQ_PLAYER_BGM_MAIN, 0, temp_a0);
                sSeqAtFixedPosSeqId = seqId;
            }

            if (seqId == NA_BGM_MILK_BAR_DUPLICATE) {
                Audio_SetSequenceAtFixedPosParams(SEQ_PLAYER_BGM_MAIN, pos, 0x1E3, 0.0f, 600.0f, 0.9f, 0.55f);
            } else if (seqId == NA_BGM_MILK_BAR) {
                Audio_SetSequenceAtFixedPosParams(SEQ_PLAYER_BGM_MAIN, pos, 0x1FF, 0.0f, 600.0f, 0.9f, 0.55f);
            } else {
                Audio_SetSequenceAtFixedPosParams(SEQ_PLAYER_BGM_MAIN, pos, 0x3F, 0.0f, 600.0f, 0.9f, 0.55f);
            }
        }
    } else {
        if (sSeqAtFixedPosSeqId == NA_BGM_ASTRAL_OBSERVATORY) {
            Audio_QueueCmdU16(0x90000000, 0xFFFF);
            Audio_QueueCmdF32(0x100FF00, 1.0f);
            AudioSeqCmd_StartSequence(SEQ_PLAYER_BGM_MAIN, 10, NA_BGM_CAVERN);
        } else {
            AudioSeqCmd_StopSequence(SEQ_PLAYER_BGM_MAIN, 5);
        }

        sSeqAtFixedPosSeqId = NA_BGM_GENERAL_SFX;
    }
}

// z_obj_sound (NA_BGM_SWAMP_CRUISE)
void Audio_PlayFanfareAtFixedPos(Vec3f* pos, s8 seqId) {
    s32 phi_v0 = false;
    s32 pad;

    if (sFanfareAtFixedPosSeqId == NA_BGM_GENERAL_SFX) {
        phi_v0 = true;
    } else if (sqrtf(SQ(pos->z) + ((SQ(pos->x) * 0.25f) + (SQ(pos->y) / 6.0f))) <
               sqrtf(SQ(sFanfareAtFixedPosPos.z) + ((SQ(sFanfareAtFixedPosPos.x) * 0.25f) + (SQ(sFanfareAtFixedPosPos.y) / 6.0f)))) {
        phi_v0 = true;
    }

    if (phi_v0) {
        sFanfareAtFixedPosPos.x = pos->x;
        sFanfareAtFixedPosPos.y = pos->y;
        sFanfareAtFixedPosPos.z = pos->z;
        sFanfareAtFixedPosSeqId = seqId;
        sFanfareAtFixedPosInUse = true;
    }
}

// Part of audio update (runs every frame), related to z_obj_sound
void Audio_UpdateFanfareAtFixedPos(void) {
    if (sFanfareAtFixedPosInUse && (sAudioPauseState == 0)) {
        if (sFanfareAtFixedPosSeqId != NA_BGM_GENERAL_SFX) {
            Audio_StartFanfareAtFixedPos(SEQ_PLAYER_FANFARE, &sFanfareAtFixedPosPos, sFanfareAtFixedPosSeqId, 0);

            if (Audio_GetActiveSequence(SEQ_PLAYER_FANFARE) == NA_BGM_DISABLED) {
                Audio_MuteBgmPlayersForFanfare();
            }

            if ((Audio_GetActiveSequence(SEQ_PLAYER_BGM_MAIN) != NA_BGM_DISABLED) && (Audio_GetActiveSequence(SEQ_PLAYER_NATURE) == NA_BGM_DISABLED)) {
                Audio_PlayNatureAmbienceSequence(9);
            }

            sAudioCutsceneFlag = true;

        } else {
            Audio_StartFanfareAtFixedPos(SEQ_PLAYER_FANFARE, NULL, sFanfareAtFixedPosSeqId, 0);
            if (Audio_GetActiveSequence(SEQ_PLAYER_BGM_MAIN) != NA_BGM_DISABLED) {
                AudioSeqCmd_StopSequence(SEQ_PLAYER_NATURE, 0);
            }
            sFanfareAtFixedPosInUse = false;
            sSeqAtFixedPosSeqId = NA_BGM_GENERAL_SFX;
            sAudioCutsceneFlag = false;
        }
        sFanfareAtFixedPosSeqId = NA_BGM_GENERAL_SFX;
    }
}

// ======== END Z_OBJ_SOUND FUNCTIONS ========

void Audio_StopSequenceAndBgmVolumeOn(u8 playerIdx, u8 volumeFadeTimer) {
    AudioSeqCmd_StopSequence((u32)playerIdx, 20);
    if ((playerIdx == SEQ_PLAYER_BGM_SUB) && (Audio_GetActiveSequence(SEQ_PLAYER_BGM_MAIN) != NA_BGM_DISABLED)) {
        Audio_SetVolumeScale(SEQ_PLAYER_BGM_MAIN, 3, 0x7F, volumeFadeTimer);
        Audio_SplitBgmChannels(0);
    }
}

void Audio_StartSubBgmAtFixedPos(u8 playerIdx, Vec3f* pos, u8 seqId, u8 flags, f32 minDist, f32 maxDist, f32 arg6) {
    f32 dist = sqrtf(SQ(pos->z) + ((SQ(pos->x) * 0.25f) + (SQ(pos->y) / 6.0f)));
    u8 targetVolume;
    u16 seqId0 = Audio_GetActiveSequence(playerIdx);
    f32 phi_f0;
    s32 pad;

    if (dist > maxDist) {
        if ((u8)seqId0 == seqId) {
            Audio_StopSequenceAndBgmVolumeOn(playerIdx, 10);
            D_801FD3B4[playerIdx] = false;
        }
        return;
    }

    if ((!gAudioContext.seqPlayers[playerIdx].enabled && !sAllPlayersMutedExceptOcaAndSys) || (seqId0 == (NA_BGM_ENEMY | 0x800))) {
        if (playerIdx == SEQ_PLAYER_BGM_SUB) {
            Audio_SetVolumeScale(playerIdx, 3, 0x7F, 1);
        }

        AudioSeqCmd_StartSequence((u32)playerIdx, 1, seqId);

        D_801FD3B4[playerIdx] = true;
    }

    Audio_SetSequenceProperties(playerIdx, pos, flags, minDist, maxDist, 1.0, 0.05f);

    if ((playerIdx == SEQ_PLAYER_BGM_SUB) && (gAudioContext.seqPlayers[SEQ_PLAYER_BGM_MAIN].enabled == true)) {

        if (dist > maxDist) {
            phi_f0 = 1.0f;
        } else if (dist < minDist) {
            phi_f0 = 0.0f;
        } else {
            phi_f0 = 1.0f - ((maxDist - dist) / (maxDist - minDist));
        }

        targetVolume = (u8)(phi_f0 * 127.0f);

        Audio_SetVolumeScale(SEQ_PLAYER_BGM_MAIN, 3, targetVolume, 10);
        Audio_SplitBgmChannels(0x7F - targetVolume);
    }
}

/**
 * sSubBgmAtFixedPosNoFilterPos takes priority over sSubBgmAtFixedPosFilterPos
 * Used only by guru guru for song of storms
 */
void Audio_PlaySubBgmAtFixedPos(Vec3f* pos, u8 seqId, f32 maxDist) {
    if (gAudioSpecId != 0xC) {
        sSubBgmAtFixedPosNoFilterPos.x = pos->x;
        sSubBgmAtFixedPosNoFilterPos.y = pos->y;
        sSubBgmAtFixedPosNoFilterPos.z = pos->z;
        sSubBgmAtFixedPosSeqId = seqId;
        D_801FD42C = maxDist;
        sSubBgmAtFixedPosFlags |= 2; // Only update volume
        D_801FD433 = 4;
    }
}

// Used only by guru guru for song of storms in stock pot from hallway or neighboring room
void Audio_PlaySubBgmAtFixedPosWithFilter(Vec3f* pos, u8 seqId, f32 maxDist) {
    sSubBgmAtFixedPosFilterPos.x = pos->x;
    sSubBgmAtFixedPosFilterPos.y = pos->y;
    sSubBgmAtFixedPosFilterPos.z = pos->z;
    sSubBgmAtFixedPosSeqId = seqId;
    //! @bug Did not set D_801FD42C = maxDist; This will use the previously set value of D_801FD42C
    sSubBgmAtFixedPosFlags |= 1; // Update with volume and filter
    D_801FD433 = 4;
}

// Part of audio update (runs every frame)
// Another bgm by pos, less customization
void Audio_UpdateSubBgmAtFixedPos1(void) {
    if (D_801FD433 != 0) {
        if (sSubBgmAtFixedPosFlags & 2) {
            // Affects only volume
            Audio_StartSubBgmAtFixedPos(SEQ_PLAYER_BGM_SUB, &sSubBgmAtFixedPosNoFilterPos, sSubBgmAtFixedPosSeqId, 0x20, 100.0f, D_801FD42C, 1.0f);
        } else {
            // Set volume with band-pass filter
            Audio_StartSubBgmAtFixedPos(SEQ_PLAYER_BGM_SUB, &sSubBgmAtFixedPosFilterPos, sSubBgmAtFixedPosSeqId, 0x28, 100.0f, D_801FD42C, 1.0f);
        }

        D_801FD433--;
        if (D_801FD433 == 0) {
            Audio_StopSequenceAndBgmVolumeOn(SEQ_PLAYER_BGM_SUB, 10);
        }

        sSubBgmAtFixedPosFlags = 0;
    }
}

// Used only by minifrog for the frog song
void func_801A1F00(u8 playerIdx, u16 seqId) {
    if (!sAudioCutsceneFlag && (gAudioSpecId != 0xC)) {
        sSubBgmAtFixedPosFilterPos.x = gDefaultSfxPos.x;
        sSubBgmAtFixedPosFilterPos.y = gDefaultSfxPos.y;
        sSubBgmAtFixedPosFilterPos.z = gDefaultSfxPos.z;
        D_801FD42C = 10000.0f;
        D_801FD436 = 128;
        sSubBgmAtFixedPosSeqId = seqId;
        D_801FD435 = playerIdx;
    }
}

// Used only by minifrog
void func_801A1F88(void) {
    if (gAudioSpecId != 0xC) {
        D_801FD436 = 1;
        sSubBgmAtFixedPosSeqId = NA_BGM_GENERAL_SFX;
    }
}

// Used all over the place
void func_801A1FB4(u8 playerIdx, Vec3f* pos, u16 seqId, f32 arg3) {
    if (!sAudioCutsceneFlag) {
        if ((Audio_GetActiveSequence(SEQ_PLAYER_BGM_MAIN) & 0xFF) != NA_BGM_SONG_OF_SOARING) {
            if (gAudioSpecId != 0xC) {
                if (pos != NULL) {
                    if (((D_801FD435 != SEQ_PLAYER_BGM_MAIN) || (Audio_GetActiveSequence(SEQ_PLAYER_BGM_MAIN) != NA_BGM_FINAL_HOURS))) {
                        sSubBgmAtFixedPosFilterPos.x = pos->x;
                        sSubBgmAtFixedPosFilterPos.y = pos->y;
                        sSubBgmAtFixedPosFilterPos.z = pos->z;
                        D_801FD42C = arg3;
                        D_801FD436 = 2;
                        sSubBgmAtFixedPosSeqId = seqId & 0xFF;
                        D_801FD435 = playerIdx;
                    }
                }
            }
        }
    }
}

// Part of audio update (runs every frame)
void Audio_UpdateSubBgmAtFixedPos2(void) {
    u16 seqId0 = Audio_GetActiveSequence(SEQ_PLAYER_BGM_MAIN);
    u8 volumeFadeTimer;

    if ((D_801FD436 != 0) && (sAudioPauseState == 0)) {
        if ((sSubBgmAtFixedPosSeqId == NA_BGM_GENERAL_SFX) || (seqId0 == NA_BGM_SONG_OF_SOARING)) {
            volumeFadeTimer = 10;

            if (seqId0 == NA_BGM_SONG_OF_SOARING) {
                D_801FD436 = 0;
                volumeFadeTimer = 1;
            } else {
                if (D_801FD436 < 128) {
                    D_801FD436--;
                }
            }

            if (D_801FD436 == 0) {
                Audio_StopSequenceAndBgmVolumeOn(D_801FD435, volumeFadeTimer);
                D_801FD3B4[D_801FD435] = false;
            }
        } else {
            if ((D_801FD435 == SEQ_PLAYER_BGM_MAIN) && (seqId0 == NA_BGM_FINAL_HOURS)) {
                Audio_StopSequenceAndBgmVolumeOn(D_801FD435, 10);
                D_801FD3B4[D_801FD435] = false;
                return;
            }

            Audio_StartSubBgmAtFixedPos(D_801FD435, &sSubBgmAtFixedPosFilterPos, sSubBgmAtFixedPosSeqId, 0x20, 200.0f, D_801FD42C, 1.0f);
            if (!D_801FD3B4[D_801FD435]) {
                D_801FD436 = 0;
            }
        }

        if (D_801FD436 < 128) {
            sSubBgmAtFixedPosSeqId = NA_BGM_GENERAL_SFX;
        }
    }
}

/**
 * Unused remnant of OoT's EnRiverSound
 */
void Audio_PlaySariaBgm(Vec3f* pos, u16 seqId, u16 distMax) {
    f32 absY;
    f32 dist;
    u8 targetVolume;
    f32 prevDist;

    if (D_801FD3AA != 0) {
        D_801FD3AA--;
        return;
    }

    dist = sqrtf(SQ(pos->z) + (SQ(pos->x) + SQ(pos->y)));
    if (sSariaBgmPtr == NULL) {
        sSariaBgmPtr = pos;
        Audio_PlaySeqWithPlayerIO(SEQ_PLAYER_BGM_SUB, seqId, 0, 7, 2);
    } else {
        prevDist = sqrtf(SQ(sSariaBgmPtr->z) + SQ(sSariaBgmPtr->x));
        if (dist < prevDist) {
            sSariaBgmPtr = pos;
        } else {
            dist = prevDist;
        }
    }

    absY = ABS_ALT(pos->y);

    if ((distMax / 15.0f) < absY) {
        targetVolume = 0;
    } else if (dist < distMax) {
        targetVolume = (1.0f - (dist / distMax)) * 127.0f;
    } else {
        targetVolume = 0;
    }

    if (seqId != NA_BGM_FAIRY_FOUNTAIN) {
        Audio_SplitBgmChannels(targetVolume);
    }

    Audio_SetVolumeScale(SEQ_PLAYER_BGM_SUB, 3, targetVolume, 0);
    Audio_SetVolumeScale(SEQ_PLAYER_BGM_MAIN, 3, 0x7F - targetVolume, 0);
}

/**
 * Unused remnant of OoT's EnRiverSound
 */
void Audio_ClearSariaBgm2(void) {
    sSariaBgmPtr = NULL;
}

/**
 * Only used in z_demo
 */
void func_801A246C(u8 playerIdx, u8 type) {
    u16 seqId;
    u8 targetVolume;
    s32 channelMask = 0;

    targetVolume = 0;

    switch (type) {
        case 0:
            targetVolume = 0x7F;
            break;
        case 1:
            channelMask = 0xFFFF;
            break;
        case 2:
            targetVolume = 0xFF;
            break;
    }

    if (targetVolume != 0xFF) {
        AudioSeqCmd_SetActiveChannels(playerIdx, channelMask);
        Audio_SetVolumeScale(SEQ_PLAYER_BGM_MAIN, 3, targetVolume, 1);
    } else {
        Audio_SetVolumeScale(SEQ_PLAYER_BGM_MAIN, 3, 0x7F, 0);
        seqId = Audio_GetActiveSequence(playerIdx);

        if (seqId != NA_BGM_DISABLED) {
            AudioSeqCmd_StartSequence(playerIdx, 0, seqId);
        }
    }
}

// Scene background music at the start of a new day
void Audio_PlayBgmForNewDayScene(u16 seqId, u8 day) {
    Audio_SetupBgmForNewDayScene(seqId);
    AudioSeqCmd_SetPlayerIO(SEQ_PLAYER_BGM_MAIN, 4, day);
}

/**
 * Identical to Audio_PlayBgmForDayScene but without the sNewSceneSeqId
 */
void Audio_SetupBgmForNewDayScene(u16 seqId) {
    if (seqId != NA_BGM_NATURE_AMBIENCE) {
        AudioSeqCmd_StopSequence(SEQ_PLAYER_NATURE, 0);
        Audio_StartBgmForScene(seqId);
        Audio_PlaySeqWithPlayerIO(SEQ_PLAYER_BGM_MAIN, seqId, 0, 0, 1);
    } else {
        Audio_PlayNatureAmbienceSequence(8);
    }
}

// Regular scene background music
void Audio_PlayBgmForDayScene(u16 seqId, u8 day) {
    if (sNewSceneSeqId != seqId) {
        if (seqId == NA_BGM_NATURE_AMBIENCE) {
            Audio_PlayNatureAmbienceSequence(8);
        } else if ((seqId != NA_BGM_FINAL_HOURS) || (sPrevMainBgmSeqId == NA_BGM_DISABLED)) {
            Audio_StartBgmForScene(seqId);
            AudioSeqCmd_SetPlayerIO(SEQ_PLAYER_BGM_MAIN, 4, day);
        }
        sNewSceneSeqId = seqId;
    }
}

void Audio_StartBgmForScene(u16 seqId) {
    u8 seqId0 = seqId;
    u8 fadeTimer = 0;
    u8 val;

    if ((sSeqFlags[sSeqIdForBgmPlayerIOPort7] & 0x20) && (sSeqFlags[((u8)seqId & 0xFF)] & 0x10)) {
        if ((sBgmPlayerIOPort7 % 64) != 0) {
            fadeTimer = 30;
        }
        Audio_PlaySeqWithPlayerIO(SEQ_PLAYER_BGM_MAIN, seqId, fadeTimer, 7, sBgmPlayerIOPort7);
        sBgmPlayerIOPort7 = 0;
    } else {
        if (sSeqFlags[((u8)seqId & 0xFF)] & 0x40) {
            val = 1;
        } else {
            val = 0xFF;
        }
        Audio_PlaySeqWithPlayerIO(SEQ_PLAYER_BGM_MAIN, seqId, 0, 7, val);
        if (!(sSeqFlags[seqId] & 0x20)) {
            sBgmPlayerIOPort7 = 0xC0;
        }
    }
    sSeqIdForBgmPlayerIOPort7 = seqId0;
}

// Part of audio update (runs every frame)
void Audio_UpdateBgmPlayerIOPort7(void) {
    u16 seqId0 = Audio_GetActiveSequence(SEQ_PLAYER_BGM_MAIN);

    if ((seqId0 != NA_BGM_DISABLED) && (sSeqFlags[(u8)seqId0 & 0xFF] & 0x10)) {
        if (sBgmPlayerIOPort7 != 0xC0) {
            sBgmPlayerIOPort7 = gAudioContext.seqPlayers[SEQ_PLAYER_BGM_MAIN].soundScriptIO[3];
        } else {
            sBgmPlayerIOPort7 = 0;
        }
    }
}

// Unused
void Audio_PlayBgmForSongOfStorms(void) {
    if (Audio_GetActiveSequence(SEQ_PLAYER_BGM_MAIN) != NA_BGM_SONG_OF_STORMS) {
        AudioSeqCmd_StartSequence(SEQ_PLAYER_BGM_MAIN, 0, NA_BGM_SONG_OF_STORMS | 0x8000);
    }
}

/**
 * Specifically for SEQ_PLAYER_BGM_MAIN
 */
void Audio_ScaleTempoAndFreqForMainBgm(f32 freqTempoScale, u8 duration) {
    if (freqTempoScale == 1.0f) {
        AudioSeqCmd_ResetTempo(SEQ_PLAYER_BGM_MAIN, duration);
    } else {
        // AudioSeqCmd_ScaleTempo(SEQ_PLAYER_BGM_MAIN, duration, freqTempoScale)
        Audio_QueueSeqCmd((u32)(duration << 0x10) | 0xB0003000 | (u16)(freqTempoScale * 100.0f));
    }

    AudioSeqCmd_SetPlayerFreq(SEQ_PLAYER_BGM_MAIN, duration, (u16)(freqTempoScale * 1000.0f));
}

void Audio_ScaleTempoAndFreqForSequence(u8 playerIdx, f32 freqTempoScale, u8 duration) {
    if (freqTempoScale == 1.0f) {
        AudioSeqCmd_ResetTempo(playerIdx, duration);
    } else {
        // AudioSeqCmd_ScaleTempo(playerIdx, duration, freqTempoScale)
        Audio_QueueSeqCmd(((u32)(playerIdx << 24)) | 0xB0003000 | (u32)(duration << 0x10) | (u16)(freqTempoScale * 100.0f));
    }

    AudioSeqCmd_SetPlayerFreq(playerIdx, duration, (u16)(freqTempoScale * 1000.0f));
}

void Audio_PlaySubBgm(u16 seqId) {
    Audio_SetVolumeScale(SEQ_PLAYER_BGM_SUB, 3, 0x7F, 0);
    AudioSeqCmd_StartSequence(SEQ_PLAYER_BGM_SUB, 0, seqId);
    Audio_SetVolumeScale(SEQ_PLAYER_BGM_MAIN, 3, 0, 5);

    // AudioSeqCmd_SetupSetPlayerVolumeWithFade
    AudioSeqCmd_SetupCmd(SEQ_PLAYER_BGM_SUB, 0x80 | SEQ_PLAYER_BGM_MAIN, 3, 10);
    // AudioSeqCmd_SetupSetActiveChannels
    AudioSeqCmd_SetupCmd(SEQ_PLAYER_BGM_SUB, 0x90 | SEQ_PLAYER_BGM_MAIN, 0, 00);
}

void Audio_StopSubBgm(void) {
    AudioSeqCmd_StopSequence(SEQ_PLAYER_BGM_SUB, 0);
}

// OoT func_800F5918
// Unused remnant of OoT
void Audio_IncreaseTempoForMinigame(void) {
    if ((Audio_GetActiveSequence(SEQ_PLAYER_BGM_MAIN) == NA_BGM_MINI_GAME_2) && Audio_IsSeqCmdNotQueued(0, 0xF0000000)) {
        // Set tempo to be higher than the default tempo for sequence mini-game 2
        AudioSeqCmd_SetTempoCmd(SEQ_PLAYER_BGM_MAIN, 5, 0, 210);
    }
}

// OoT func_800F595C
void Audio_PlaySequenceInCutscene(u16 seqId) {
    u8 seqId0 = seqId;

    if (sSeqFlags[seqId0 & 0xFF] & 2) {
        Audio_PlayFanfare(seqId);
    } else if (sSeqFlags[seqId0 & 0xFF] & 4) {
        AudioSeqCmd_StartSequence(SEQ_PLAYER_FANFARE, 0, seqId);
    } else if (sSeqFlags[seqId0 & 0xFF] & 0x80) {
        AudioSeqCmd_StartSequence(SEQ_PLAYER_BGM_SUB, 0, seqId);
    } else {
        Audio_PlaySeqWithPlayerIO(SEQ_PLAYER_BGM_MAIN, seqId, 0, 7, 0xFF);
        Audio_SetVolumeScale(SEQ_PLAYER_BGM_MAIN, 1, 0x7F, 0);
        Audio_SetVolumeScale(SEQ_PLAYER_BGM_MAIN, 3, 0x7F, 0);
        AudioSeqCmd_StopSequence(SEQ_PLAYER_FANFARE, 0);
    }
}

// OoT func_800F59E8
void Audio_StopSequenceInCutscene(u16 seqId) {
    u8 seqId0 = seqId;

    if (sSeqFlags[seqId0 & 0xFF] & 2) {
        AudioSeqCmd_StopSequence(SEQ_PLAYER_FANFARE, 0);
    } else if (sSeqFlags[seqId0 & 0xFF] & 4) {
        AudioSeqCmd_StopSequence(SEQ_PLAYER_FANFARE, 0);
    } else if (sSeqFlags[seqId0 & 0xFF] & 0x80) {
        AudioSeqCmd_StopSequence(SEQ_PLAYER_BGM_SUB, 0);
    } else {
        AudioSeqCmd_StopSequence(SEQ_PLAYER_BGM_MAIN, 0);
    }
}

// OoT func_800F5A58
s32 Audio_IsSequencePlaying(u8 seqId) {
    u8 playerIdx = SEQ_PLAYER_BGM_MAIN;

    if (sSeqFlags[seqId & 0xFF] & 2) {
        playerIdx = SEQ_PLAYER_FANFARE;
    } else if (sSeqFlags[seqId & 0xFF] & 4) {
        playerIdx = SEQ_PLAYER_FANFARE;
    }

    if (seqId == (Audio_GetActiveSequence(playerIdx) & 0xFF)) {
        return true;
    } else {
        return false;
    }
}

// Miniboss music
void Audio_PlayBgmForMiniBoss(u16 seqId) {
    u16 seqId0 = Audio_GetActiveSequence(SEQ_PLAYER_BGM_MAIN);

    if (seqId0 == NA_BGM_DISABLED) {
        seqId0 = NA_BGM_GENERAL_SFX;
    }

    if (seqId0 != seqId) {
        Audio_SetSequenceMode(SEQ_MODE_IGNORE);
        if (!(sSeqFlags[seqId0] & 8)) {
            sPrevMainBgmSeqId = seqId0;
        }

        AudioSeqCmd_StartSequence(SEQ_PLAYER_BGM_MAIN, 0, (seqId + 0x8000));
    }
}

/**
 * To be used in conjunction with Audio_PlayBgmForMiniBoss
 */
void Audio_RestorePreviousBgm(void) {
    if ((Audio_GetActiveSequence(SEQ_PLAYER_BGM_MAIN) != NA_BGM_DISABLED) && (sSeqFlags[Audio_GetActiveSequence(SEQ_PLAYER_BGM_MAIN) & 0xFF] & 8)) {
        if ((sPrevMainBgmSeqId == NA_BGM_DISABLED) || (sPrevMainBgmSeqId == NA_BGM_GENERAL_SFX)) {
            AudioSeqCmd_StopSequence(SEQ_PLAYER_BGM_MAIN, 0);
        } else {
            if (sPrevMainBgmSeqId == NA_BGM_NATURE_AMBIENCE) {
                sPrevMainBgmSeqId = D_801FD438;
            }
            AudioSeqCmd_StartSequence(SEQ_PLAYER_BGM_MAIN, 0, sPrevMainBgmSeqId + 0x8000);
        }
        sPrevMainBgmSeqId = NA_BGM_DISABLED;
    }
}

// Unused
void Audio_SetupNatureAmbienceSequence(u8 natureSeqId) {
    u16 seqId = Audio_GetActiveSequence(SEQ_PLAYER_BGM_MAIN);

    if (seqId != NA_BGM_NATURE_AMBIENCE) {
        sPrevMainBgmSeqId = seqId;
    }

    Audio_PlayNatureAmbienceSequence(natureSeqId);
}

// Unused
void Audio_ForceRestorePreviousBgm(void) {
    if (sPrevMainBgmSeqId != NA_BGM_DISABLED) {
        AudioSeqCmd_StartSequence(SEQ_PLAYER_BGM_MAIN, 0, sPrevMainBgmSeqId + 0x8000);
    }

    sPrevMainBgmSeqId = NA_BGM_DISABLED;
}


// ======== BEGIN FANFARE FUNCTIONS ========


void Audio_PlayFanfareWithPlayerIOPort7(u16 seqId, u8 val) {
    Audio_PlayFanfare(seqId);
    AudioSeqCmd_SetPlayerIO(SEQ_PLAYER_FANFARE, 7, val);
}

void Audio_MuteBgmPlayersForFanfare(void) {
    Audio_SetVolumeScale(SEQ_PLAYER_BGM_MAIN, 1, 0, 5);
    Audio_SetVolumeScale(SEQ_PLAYER_BGM_SUB, 1, 0, 5);
    // AudioSeqCmd_SetupSetPlayerVolumeWithFade(SEQ_PLAYER_FANFARE, SEQ_PLAYER_BGM_MAIN, ...)
    AudioSeqCmd_SetupCmd(SEQ_PLAYER_FANFARE, 0x80, 1, 10);
    // AudioSeqCmd_SetupSetPlayerVolumeWithFade(SEQ_PLAYER_FANFARE, SEQ_PLAYER_BGM_SUB, ...)
    AudioSeqCmd_SetupCmd(SEQ_PLAYER_FANFARE, 0x83, 1, 10);
    // AudioSeqCmd_SetupSetActiveChannels(SEQ_PLAYER_FANFARE, ...)
    AudioSeqCmd_SetupCmd(SEQ_PLAYER_FANFARE, 0x90, 0, 0);
}

/**
 * Sets up seqId to play on playerIdx 1
 */
void Audio_PlayFanfare(u16 seqId) {
    u16 prevSeqId = Audio_GetActiveSequence(SEQ_PLAYER_FANFARE);
    u32 outNumFonts;
    u8* prevFontId = AudioCmd_GetFontsForSequence(prevSeqId & 0xFF, &outNumFonts);
    u8* fontId = AudioCmd_GetFontsForSequence(seqId & 0xFF, &outNumFonts);

    if ((prevSeqId == NA_BGM_DISABLED) || (*prevFontId == *fontId)) {
        sFanfareState = 1;
    } else {
        sFanfareState = 5;
        AudioSeqCmd_StopSequence(SEQ_PLAYER_FANFARE, 0);
    }

    sFanfareSeqId = seqId;
}

// OoT func_800F5CF8
// Part of audio update (runs every frame)
void Audio_UpdateFanfare(void) {
    if (sFanfareState != 0) {
        if ((sFanfareState != 5) && !Audio_IsSeqCmdNotQueued(0x11000000, 0xFF000000)) {
            sFanfareState = 0;
        } else {
            sFanfareState--;
            if (sFanfareState == 0) {
                Audio_QueueCmdS32(0xE3000000, SEQUENCE_TABLE);
                Audio_QueueCmdS32(0xE3000000, FONT_TABLE);
                Audio_GetActiveSequence(SEQ_PLAYER_BGM_MAIN);
                if (Audio_GetActiveSequence(SEQ_PLAYER_FANFARE) == NA_BGM_DISABLED) {
                    Audio_MuteBgmPlayersForFanfare();
                }
                AudioSeqCmd_StartSequence(SEQ_PLAYER_FANFARE, 0, sFanfareSeqId);
                AudioSeqCmd_SetActiveChannels(SEQ_PLAYER_BGM_MAIN, 0xFFFF);
            }
        }
    }
}

// OoT func_800F5E18
void Audio_PlayFanfareWithPlayerIOCustomPort(u16 seqId, s8 port, u8 val) {
    AudioSeqCmd_SetPlayerIO(SEQ_PLAYER_FANFARE, port, val);
    Audio_PlayFanfare(seqId);
}


// ======== END FANFARE FUNCTIONS ========


// OoT func_800F5E18
void Audio_PlaySeqWithPlayerIO(s8 playerIdx, u16 seqId, u8 fadeTimer, s8 port, u8 val) {
    u16 seqId0;

    AudioSeqCmd_SetPlayerIO(playerIdx, port, val);
    if ((seqId & 0xFF) < 2) {
        seqId0 = seqId;
    } else {
        seqId0 = seqId | 0x8000;
    }
    AudioSeqCmd_StartSequence(playerIdx, fadeTimer, seqId0);
}

void Audio_SetSequenceMode(u8 seqMode) {
    s32 volumeFadeInTimer;
    u16 seqId;
    u8 volumeFadeOutTimer;

    if ((sPrevMainBgmSeqId == NA_BGM_DISABLED) && (sPrevMainBgmSeqId == NA_BGM_DISABLED)) {
        // clang-format off
        if (sAudioCutsceneFlag || D_801FD3B4[SEQ_PLAYER_BGM_SUB]) { \
            seqMode = SEQ_MODE_IGNORE;
        }
        // clang-format on

        seqId = gActiveSeqs[SEQ_PLAYER_BGM_MAIN].seqId;
        
        if ((seqId == NA_BGM_DISABLED) || (sSeqFlags[(u8)(seqId & 0xFF)] & 1) || ((sPrevSeqMode & 0x7F) == SEQ_MODE_ENEMY)) {
            if (seqMode != (sPrevSeqMode & 0x7F)) {
                if (seqMode == SEQ_MODE_ENEMY) {
                    // If only seqMode = SEQ_MODE_ENEMY (Start)
                    if (gActiveSeqs[SEQ_PLAYER_BGM_SUB].volScales[1] - sAudioEnemyVol < 0) {
                        volumeFadeInTimer = -(gActiveSeqs[SEQ_PLAYER_BGM_SUB].volScales[1] - sAudioEnemyVol);
                    } else {
                        volumeFadeInTimer = gActiveSeqs[SEQ_PLAYER_BGM_SUB].volScales[1] - sAudioEnemyVol;
                    }

                    Audio_SetVolumeScale(SEQ_PLAYER_BGM_SUB, 3, sAudioEnemyVol, volumeFadeInTimer);
                    AudioSeqCmd_StartSequence(SEQ_PLAYER_BGM_SUB, 10, NA_BGM_ENEMY | 0x800);

                    if (seqId >= NA_BGM_TERMINA_FIELD) {
                        Audio_SetVolumeScale(SEQ_PLAYER_BGM_MAIN, 3, 0x7F - sAudioEnemyVol, 10);
                        Audio_SplitBgmChannels(sAudioEnemyVol);
                    }
                } else if ((sPrevSeqMode & 0x7F) == SEQ_MODE_ENEMY) {
                    // If only sPrevSeqMode = SEQ_MODE_ENEMY (End)
                    AudioSeqCmd_StopSequence(SEQ_PLAYER_BGM_SUB, 10);

                    if (seqMode == SEQ_MODE_IGNORE) {
                        volumeFadeOutTimer = 0;
                    } else {
                        volumeFadeOutTimer = 10;
                    }

                    Audio_SetVolumeScale(SEQ_PLAYER_BGM_MAIN, 3, 0x7F, volumeFadeOutTimer);
                    Audio_SplitBgmChannels(0);
                }

                sPrevSeqMode = seqMode + 0x80;
            } else {
                if (seqMode == SEQ_MODE_ENEMY) {
                    // If both seqMode = sPrevSeqMode = SEQ_MODE_ENEMY
                    if ((Audio_GetActiveSequence(SEQ_PLAYER_BGM_SUB) == NA_BGM_DISABLED) &&
                        (seqId != NA_BGM_DISABLED) && (sSeqFlags[(u8)(seqId & 0xFF)] & 1)) {
                        AudioSeqCmd_StartSequence(SEQ_PLAYER_BGM_SUB, 10, NA_BGM_ENEMY | 0x800);
                        sPrevSeqMode = seqMode + 0x80;
                    }
                }
            }
        } else {
            if (seqMode == SEQ_MODE_DEFAULT) {
                if (sPrevSeqMode == SEQ_MODE_STILL) {
                    sNumFramesMoving = 0;
                }
                sNumFramesStill = 0;
                sNumFramesMoving++;
            } else {
                sNumFramesStill++;
            }

            if ((seqMode == SEQ_MODE_STILL) && (sNumFramesStill < 30) && (sNumFramesMoving > 20)) {
                seqMode = SEQ_MODE_DEFAULT;
                if (!seqMode && !seqMode) {}
            }

            sPrevSeqMode = seqMode;
            AudioSeqCmd_SetPlayerIO(SEQ_PLAYER_BGM_MAIN, 2, seqMode);
        }
    }
}

// OoT func_800F6114
void Audio_UpdateEnemyBgmVolume(f32 dist) {
    f32 adjDist;
    u16 seqId0 = D_8020034A;
    u16 seqId1;

    if (sPrevSeqMode == (0x80 | SEQ_MODE_ENEMY)) {
        if (dist != sAudioEnemyDist) {
            if (dist < 150.0f) {
                adjDist = 0.0f;
            } else if (dist > 500.0f) {
                adjDist = 350.0f;
            } else {
                adjDist = dist - 150.0f;
            }

            sAudioEnemyVol = ((350.0f - adjDist) * 127.0f) / 350.0f;
            Audio_SetVolumeScale(SEQ_PLAYER_BGM_SUB, 3, sAudioEnemyVol, 10);
            seqId1 = seqId0 & 0xFF;
            if ((seqId0 >= NA_BGM_TERMINA_FIELD) && !(sSeqFlags[seqId1] & 4)) {
                Audio_SetVolumeScale(SEQ_PLAYER_BGM_MAIN, 3, (0x7F - sAudioEnemyVol), 10);
            }
        }

        Audio_SplitBgmChannels(sAudioEnemyVol);
    }
    sAudioEnemyDist = dist;
}

// OoT func_800F6114
// Unused remnant of OoT
void func_801A36F0(f32 dist, u16 seqId) {
    s8 pad;
    s8 phi_v1;
    s16 seqId0;

    if (D_801FD3A9) {
        return;
    }

    seqId0 = (s8)(Audio_GetActiveSequence(SEQ_PLAYER_BGM_MAIN) & 0xFF);
    if (seqId0 == (seqId & 0xFF)) {
        if ((seqId & 0xFF) == NA_BGM_ROMANI_RANCH) {

            if (dist > 2000.0f) {
                phi_v1 = 127; 
            } else if (dist < 200.0f) {
                phi_v1 = 0;
            } else {
                phi_v1 = (s8)(((dist - 200.0f) * 127.0f) / 1800.0f);
            }
            AudioSeqCmd_SetChannelVol(SEQ_PLAYER_BGM_MAIN, 3, 0, 127 - phi_v1);
            AudioSeqCmd_SetChannelVol(SEQ_PLAYER_BGM_MAIN, 3, 1, 127 - phi_v1);
            Audio_QueueSeqCmd(0x60000000 | ((u32)SEQ_PLAYER_BGM_MAIN << 24) | ((u32)(3) << 16) | ((u32)(0xD) << 8) | (phi_v1));
            if (D_801FD3A8 == 0) {
                D_801FD3A8++;
            }
        }
    } else if ((seqId0 == NA_BGM_NATURE_AMBIENCE) && ((seqId & 0xFF) == NA_BGM_ROMANI_RANCH)) {
        seqId0 = (s8)(Audio_GetActiveSequence(SEQ_PLAYER_BGM_SUB) & 0xFF);
        if ((seqId0 != (seqId & 0xFF)) && (D_801FD3A8 < 10)) {
            Audio_PlaySeqWithPlayerIO(SEQ_PLAYER_BGM_SUB, NA_BGM_ROMANI_RANCH, 0, 0, 0);
            AudioSeqCmd_SetActiveChannels(SEQ_PLAYER_BGM_SUB, 0xFFFC);
            D_801FD3A8 = 10;
        }

        if (dist > 2000.0f) {
            phi_v1 = 127;
        } else if (dist < 200.0f) {
            phi_v1 = 0;
        } else {
            phi_v1 = (s8)(((dist - 200.0f) * 127.0f) / 1800.0f);
        }
        AudioSeqCmd_SetChannelVol(SEQ_PLAYER_BGM_SUB, 3, 0, 127 - phi_v1);
        AudioSeqCmd_SetChannelVol(SEQ_PLAYER_BGM_SUB, 3, 1, 127 - phi_v1);
    }

    if (D_801FD3A8 < 10) {
        D_801FD3A8++;
    }
}

u8 func_801A3950(u8 playerIdx, u8 isChannelIOSet) {
    u8 ret = 0xFF;
    SequenceChannel* channel = gAudioContext.seqPlayers[playerIdx].channels[15];

    if (gAudioContext.seqPlayers[playerIdx].enabled && channel->enabled) {
        if (channel->soundScriptIO[0] != -1) {
            ret = channel->soundScriptIO[0];
            if (isChannelIOSet == true) {
                // Set value 0xFF to channel 15 in io port 0
                AudioSeqCmd_SetChannelIO(playerIdx, 0, 15, 0xFF);
            }
        }
    }
    return ret;
}

// Used only by minifrog
u8 func_801A39F8(void) {
    u8 frogIndex = 0xFF;
    s32 pad;
    SequenceChannel* channel = gAudioContext.seqPlayers[SEQ_PLAYER_BGM_SUB].channels[15];

    if (gAudioContext.seqPlayers[SEQ_PLAYER_BGM_SUB].enabled) {
        if ((Audio_GetActiveSequence(SEQ_PLAYER_BGM_SUB) == NA_BGM_FROG_SONG) && channel->enabled) {
            if (channel->soundScriptIO[0] != -1) {
                frogIndex = channel->soundScriptIO[0];
                Audio_QueueCmdS8(0x6030F00, -1);
            }
        }
    }
    return frogIndex;
}

// Used by kaleido scope/setup
// New to MM
// Setup: arg0 = 1 (Used when opening up the window)
// Scope: arg0 = 0 (relates to the last case in KaleidoScope_Update in OoT's version)
void Audio_SetPauseState(u8 arg0) {
    if ((arg0 == 0) && (sAudioPauseState != 0)) {
        sAudioPauseState = 1;
    } else {
        sAudioPauseState = arg0 << 1; // sAudioPauseState = 2
    }
}

// Part of audio update (runs every frame)
// New to MM
void Audio_UpdatePauseState(void) {
    if ((sAudioPauseState != 0) && (sAudioPauseState != 2)) {
        sAudioPauseState--;
    }
}

// OoT func_800F64E0
void Audio_PlaySfxForWindow(u8 windowToggleDirection) {
    sAudioIsWindowOpen = windowToggleDirection;
    if (windowToggleDirection) {
        Audio_PlaySfx1(NA_SE_SY_WIN_OPEN);
        // mute all seqplayers
        Audio_QueueCmdS32(0xF1 << 24 | 0xFF << 16, 0);
    } else {
        Audio_PlaySfx1(NA_SE_SY_WIN_CLOSE);
        // unmute all seqplayers
        Audio_QueueCmdS32(0xF2 << 24 | 0xFF << 16, 0);
    }
}

// Only used by guru guru
void Audio_MuteSeqPlayerBgmSub(u8 isMuted) {
    if (isMuted == true) {
        // mute seq player bgm-sub
        Audio_QueueCmdS32(0xF1 << 24 | SEQ_PLAYER_BGM_SUB << 16, 0);
    } else {
        // unmute seq player bgm-sub
        Audio_QueueCmdS32(0xF2 << 24 | SEQ_PLAYER_BGM_SUB << 16, 0);
    }
}

// OoT func_800F6584
// unused remnant of OoT
void func_801A3B90(u8 arg0) {
    u8 playerIdx;
    u16 channelMask;

    D_801FD3A9 = arg0;
    if ((Audio_GetActiveSequence(SEQ_PLAYER_BGM_MAIN) & 0xFF) == NA_BGM_ROMANI_RANCH) {
        playerIdx = SEQ_PLAYER_BGM_MAIN;
        channelMask = 0;
    } else if ((Audio_GetActiveSequence(SEQ_PLAYER_BGM_SUB) & 0xFF) == NA_BGM_ROMANI_RANCH) {
        playerIdx = SEQ_PLAYER_BGM_SUB;
        channelMask = 0xFFFC;
    } else {
        return;
    }

    if (arg0 != 0) {
        AudioSeqCmd_SetChannelVol(playerIdx, 1, 0, 0);
        AudioSeqCmd_SetChannelVol(playerIdx, 1, 1, 0);
        if (playerIdx == SEQ_PLAYER_BGM_SUB) {
            AudioSeqCmd_SetActiveChannels(playerIdx, channelMask | 3);
        }
    } else {
        if (playerIdx == SEQ_PLAYER_BGM_SUB) {
            Audio_PlaySeqWithPlayerIO(SEQ_PLAYER_BGM_SUB, NA_BGM_ROMANI_RANCH, 0, 0, 0);
        }
        AudioSeqCmd_SetChannelVol(playerIdx, 1, 0, 0x7F);
        AudioSeqCmd_SetChannelVol(playerIdx, 1, 1, 0x7F);
        if (playerIdx == SEQ_PLAYER_BGM_SUB) {
            AudioSeqCmd_SetActiveChannels(playerIdx, channelMask);
        }
    }
}

void Audio_SetEnvReverb(s8 reverb) {
    sAudioEnvReverb = reverb & 0x7F;
}

void Audio_SetCodeReverb(s8 reverb) {
    u8 reverbIndex;

    if (reverb != 0) {
        if ((reverb & 0x40) != (sAudioCodeReverb & 0x40)) {
            reverbIndex = (reverb >> 6) + 1;
            Audio_SetSfxReverbIndexExceptOcarinaBank(reverbIndex);
        }
        sAudioCodeReverb = reverb & 0x7F;
    }
}

/**
 * Possibly a test for surround sound
 * Unused
 */
void Audio_PlaySfxForSurroundSoundTest(void) {
    s32 val = 0;

    if (sAudioFileSelectSetting == AUDIO_FS_SURROUND) {
        val = 2;
    }

    Audio_PlaySfxAtPosWithAllChannelsIO(&gDefaultSfxPos, NA_SE_SY_SOUT_DEMO, val);
}

// OoT func_800F6700
void Audio_ApplyFileSelectSettings(s8 audioSetting) {
    s8 soundMode;

    switch (audioSetting) {
        case FS_AUDIO_STEREO:
            soundMode = AUDIO_MODE_STEREO;
            sAudioFileSelectSetting = AUDIO_FS_STEREO;
            break;
        case FS_AUDIO_MONO:
            soundMode = AUDIO_MODE_MONO;
            sAudioFileSelectSetting = AUDIO_FS_MONO;
            break;
        case FS_AUDIO_HEADSET:
            soundMode = AUDIO_MODE_HEADSET;
            sAudioFileSelectSetting = AUDIO_FS_HEADSET;
            break;
        case FS_AUDIO_SURROUND:
            soundMode = AUDIO_MODE_SURROUND;
            sAudioFileSelectSetting = AUDIO_FS_SURROUND;
            break;
    }

    AudioSeqCmd_SetSoundMode(SEQ_PLAYER_BGM_MAIN, soundMode);
}

void Audio_SetBaseFilter(u8 filter) {
    if (sAudioBaseFilter != filter) {
        if (filter == 0) {
            Audio_StopSfxById(NA_SE_PL_IN_BUBBLE);
        } else if (sAudioBaseFilter == 0) {
            Audio_PlaySfxGeneral(NA_SE_PL_IN_BUBBLE, &gDefaultSfxPos, 4, &gDefaultSfxVolOrFreq, &gDefaultSfxVolOrFreq,
                                 &gDefaultSfxReverbAdd);
        }
    }
    sAudioBaseFilter = filter;
    sAudioBaseFilter2 = filter;
}

void Audio_SetExtraFilter(u8 filter) {
    u8 channelIdx;

    sAudioExtraFilter2 = filter;
    sAudioExtraFilter = filter;
    if (gActiveSeqs[SEQ_PLAYER_NATURE].seqId == NA_BGM_NATURE_AMBIENCE) {
        for (channelIdx = 0; channelIdx < 16; channelIdx++) {
            // CHAN_UPD_SCRIPT_IO (seq player 4, all channels, slot 6)
            Audio_QueueCmdS8((((u32)channelIdx & 0xFF) << 8) | 0x6040000 | 6, filter);
        }
    }
}

void Audio_SetCutsceneFlag(s8 flag) {
    sAudioCutsceneFlag = flag;
}

void Audio_PlaySfxGeneralIfNotInCutscene(u16 sfxId, Vec3f* pos, u8 token, f32* freqScale, f32* volume, s8* reverbAdd) {
    if (!sAudioCutsceneFlag) {
        Audio_PlaySfxGeneral(sfxId, pos, token, freqScale, volume, reverbAdd);
    }
}

void Audio_PlaySfxIfNotInCutscene(u16 sfxId) {
    Audio_PlaySfxGeneralIfNotInCutscene(sfxId, &gDefaultSfxPos, 4, &gDefaultSfxVolOrFreq, &gDefaultSfxVolOrFreq,
                                        &gDefaultSfxReverbAdd);
}

// Unused
void Audio_MuteSfxAndNatureSeqExceptOcaAndSys(u8 arg0) {
    sMuteOnlySfxAndNatureSeq = arg0;
}

void Audio_SetSpec(u8 specId) {
    if (!sMuteOnlySfxAndNatureSeq) {
        AudioSeqCmd_SetSpec(SEQ_PLAYER_BGM_MAIN, 0, 0, specId);
    } else {
        AudioSeqCmd_SetSpec(SEQ_PLAYER_BGM_MAIN, 2, 0, specId);
    }
}

// OoT func_800F6964
void Audio_MuteAllSeqExceptSysAndOca(u16 duration) {
    s32 skip;
    u8 channelIdx;

    if (!sMuteOnlySfxAndNatureSeq) {
        sAllPlayersMutedExceptOcaAndSys = true;
        AudioSeqCmd_StopSequence(SEQ_PLAYER_BGM_MAIN, (duration * 3) / 2);
        AudioSeqCmd_StopSequence(SEQ_PLAYER_FANFARE, (duration * 3) / 2);
        AudioSeqCmd_StopSequence(SEQ_PLAYER_BGM_SUB, (duration * 3) / 2);
    }
    
    AudioSeqCmd_StopSequence(SEQ_PLAYER_NATURE, (duration * 3) / 2);

    for (channelIdx = 0; channelIdx < 16; channelIdx++) {
        skip = false;
        switch (channelIdx) {
            case SFX_CHANNEL_SYSTEM0:
            case SFX_CHANNEL_SYSTEM1:
                if (gAudioSpecId == 10) {} // Remnant of OoT
                if (gAudioSpecId == 11) {}
                skip = true;
                break;
            case SFX_CHANNEL_OCARINA:
                skip = true;
                break;
        }

        if (!skip) {
            AudioSeqCmd_SetChannelVol(SEQ_PLAYER_SFX, (u8)(duration >> 1), channelIdx, 0);
        }
    }
}

void Audio_MuteSfxAndNatureSeqExceptSysAndOca(u16 arg0) {
    sMuteOnlySfxAndNatureSeq = true;
    Audio_MuteAllSeqExceptSysAndOca(arg0);
}

// OoT func_800F6AB0
void Audio_StopFanfare(u16 arg0) {
    AudioSeqCmd_StopSequence(SEQ_PLAYER_BGM_MAIN, arg0);
    if (Audio_GetActiveSequence(SEQ_PLAYER_FANFARE) != NA_BGM_DISABLED) {
        AudioSeqCmd_StopSequence(SEQ_PLAYER_FANFARE, arg0);
    }
    AudioSeqCmd_StopSequence(SEQ_PLAYER_BGM_SUB, arg0);
    AudioSeqCmd_StopSequence(SEQ_PLAYER_NATURE, arg0);
    Audio_SetVolumeScale(SEQ_PLAYER_BGM_MAIN, 3, 0x7F, 0);
    Audio_SetVolumeScale(SEQ_PLAYER_BGM_MAIN, 1, 0x7F, 0);
}

/**
 * The flag 0xFF makes the sequence process SkipTicks and SkipForwardSequence
 * OoT func_800F6B3C
 * Unused
 */
void Audio_StartSfxPlayer(void) {
    Audio_StartSequence(SEQ_PLAYER_SFX, NA_BGM_GENERAL_SFX, 0xFF, 5);
}

void Audio_DisableAllSeq(void) {
    Audio_InternalCmdDisableSeq(SEQ_PLAYER_BGM_MAIN, 0);
    Audio_InternalCmdDisableSeq(SEQ_PLAYER_FANFARE, 0);
    Audio_InternalCmdDisableSeq(SEQ_PLAYER_SFX, 0);
    Audio_InternalCmdDisableSeq(SEQ_PLAYER_BGM_SUB, 0);
    Audio_InternalCmdDisableSeq(SEQ_PLAYER_NATURE, 0);
    Audio_ScheduleProcessCmds();
}

// OoT func_800F6BB8
s8 func_801A4324(void) {
    return func_80194528();
}

// OoT func_800F6BDC
// Unused
void func_801A4348(void) {
    Audio_DisableAllSeq();
    Audio_ScheduleProcessCmds();
    while (true) {
        if (!func_801A4324()) {
            break;
        }
    }
}

void Audio_SetSfxVolumeExceptSystemAndOcarinaBanks(u8 volume) {
    u8 channelIdx;

    if (!sAllPlayersMutedExceptOcaAndSys) {
        for (channelIdx = 0; channelIdx < 16; channelIdx++) {
            switch (channelIdx) {
                case SFX_CHANNEL_SYSTEM0:
                case SFX_CHANNEL_SYSTEM1:
                case SFX_CHANNEL_OCARINA:
                    break;
                default:
                    AudioSeqCmd_SetChannelVol(SEQ_PLAYER_SFX, 10, channelIdx, volume);
                    break;
            }
        }
    }
}

void Audio_SetSfxReverbIndexExceptOcarinaBank(u8 reverbIndex) {
    u8 channelIdx;

    for (channelIdx = 0; channelIdx < 16; channelIdx++) {
        if (channelIdx != SFX_CHANNEL_OCARINA) {
            Audio_QueueCmdS8(0x11 << 24 | SEQ_PLAYER_SFX << 16 | (((u32)channelIdx & 0xFF) << 8), reverbIndex);
        }
    }
}

void Audio_PreNMI(void) {
    Audio_PreNMIInternal();
}

// Part of audio update (runs every frame)
void Audio_ResetNewSceneSeqId(void) {
    sNewSceneSeqId = NA_BGM_DISABLED;
}

// OoT func_800F6C34
void Audio_ResetData(void) {
    u8 i;

    for (i = 0; i < ARRAY_COUNT(sSfxSettings); i++) {
        sSfxSettings[i].pos = NULL;
        sSfxSettings[i].freqScale = 1.0;
        sSfxSettings[i].reverbAdd = 0;
    }
    sSfxSettingsFlags = 0;
    sTwoSemitonesLoweredFreq = 0.9f;
    sIncreasedSfxReverb = 20;
    sSfxVolumeDuration = 0;
    sPrevSeqMode = 0;
    sSyncedSfxVolume = 1.0f;
    sSyncedSfxFreq = 1.0f;
    sAudioBaseFilter = 0;
    sAudioExtraFilter = 0;
    sAudioBaseFilter2 = 0;
    sAudioExtraFilter2 = 0;
    AudioOcarina_ResetInstrumentId();
    sRiverFreqScaleLerp.remainingFrames = 0;
    sWaterfallFreqScaleLerp.remainingFrames = 0;
    sRiverFreqScaleLerp.value = 1;
    sWaterfallFreqScaleLerp.value = 1;
    sAdjustedSfxFreq = 1;
    sRiverSoundMainBgmVol = 0x7F;
    sRiverSoundMainBgmCurrentVol = 0x7F;
    sRiverSoundMainBgmLower = false;
    sRiverSoundMainBgmRestore = false;
    sGanonsTowerVol = 0xFF;
    D_801FD3A8 = 0;
    sFanfareAtFixedPosInUse = 0;
    sSpecReverb = sSpecReverbs[gAudioSpecId];
    sAudioIsWindowOpen = false;
    sPrevMainBgmSeqId = NA_BGM_DISABLED;
    Audio_QueueCmdS8(0x46000000, -1);
    sSariaBgmPtr = NULL;
    sFanfareState = 0;
    D_801FD3AA = 1;
    D_801FD3A9 = false;
    sSeqAtFixedPosSeqId = NA_BGM_GENERAL_SFX;
    D_801FD438 = NA_BGM_DISABLED;
    sSubBgmAtFixedPosFlags = 0;
    D_801FD432 = 0;
    sSubBgmAtFixedPosSeqId = NA_BGM_GENERAL_SFX;
    D_801FD433 = 0;
    D_801FD434 = 0;
    D_801FD436 = 0;
    sAllPlayersMutedExceptOcaAndSys = false;
    sAudioPauseState = 0;
    sSeqAtFixedPosPlayerIdx = SEQ_PLAYER_INVALID;
    sIsFinalHoursOrSoaring = false;

    for (i = SEQ_PLAYER_BGM_MAIN; i <= SEQ_PLAYER_BGM_SUB; i++) {
        D_801FD3B4[i] = false;
    }
}

s32 func_801A46F8(void) {
    switch (func_801A3950(SEQ_PLAYER_FANFARE, true)) {
        case 0:
        case 0x8:
        case 0x10:
        case 0x18:
        case 0x20:
            return true;
        default:
            return false;
    }
}

// used for z_obj_sound and z_en_gk
void Audio_PlaySfxAtFixedPos(Vec3f* pos, u16 sfxId) {
    static f32 sSfxOriginalPos[] = { 0.0f, 0.0f, 0.0f };
    s32 i;

    for (i = 0; i < ARRAY_COUNT(sSfxOriginalPos); i++) {
        sSfxOriginalPos[i] = ((f32*)pos)[i];
    }

    Audio_PlaySfxAtPos((Vec3f*)sSfxOriginalPos, sfxId);
}

void Audio_PlaySfxAtPosWithVolumeTransition(Vec3f* pos, u16 sfxId, u16 duration) {
    Audio_PlaySfxAtPosWithVolume(pos, sfxId, 0.0f);
    Audio_SetSfxVolumeTransition(&sSfxVolume, 1.0f, duration);
}


// ======== BEGIN NATURE AMBIENCE FUNCTIONS ========


// OoT func_800F6D58
void Audio_SetNatureAmbienceChannelIO(u8 channelIdxRange, u8 port, u8 val) {
    u8 firstChannelIdx;
    u8 lastChannelIdx;
    u8 channelIdx;

    if ((gActiveSeqs[SEQ_PLAYER_NATURE].seqId != NA_BGM_NATURE_AMBIENCE) && Audio_IsSeqCmdNotQueued(NA_BGM_NATURE_AMBIENCE, 0xF00000FF)) {
        return;
    }

    // channelIdxRange = 01 on port 1
    if (((channelIdxRange << 8) + (u32)port) == ((1 << 8) | (u32)1)) {
        if (Audio_GetActiveSequence(SEQ_PLAYER_BGM_SUB) != NA_BGM_ROMANI_RANCH) {
            D_801FD3A8 = 0;
        }
    }

    firstChannelIdx = channelIdxRange >> 4;
    lastChannelIdx = channelIdxRange & 0xF;
    if (firstChannelIdx == 0) {
        firstChannelIdx = channelIdxRange & 0xF;
    }

    for (channelIdx = firstChannelIdx; channelIdx <= lastChannelIdx; channelIdx++) {
        AudioSeqCmd_SetChannelIO(SEQ_PLAYER_NATURE, port, channelIdx, val);
    }
}

// OoT func_800F6E7C
void Audio_StartNatureAmbienceSequence(u16 playerIO, u16 channelMask) {
    u8 channelIdx;

    AudioSeqCmd_SetPlayerIO(SEQ_PLAYER_NATURE, PLAYER_IO_PORT_0, 1);
    AudioSeqCmd_SetPlayerIO(SEQ_PLAYER_NATURE, PLAYER_IO_PORT_4, (u8)(playerIO >> 8));
    AudioSeqCmd_SetPlayerIO(SEQ_PLAYER_NATURE, PLAYER_IO_PORT_5, (u8)(playerIO & 0xFF));
    Audio_SetVolumeScale(SEQ_PLAYER_BGM_MAIN, 0, 0x7F, 1);

    if ((Audio_GetActiveSequence(SEQ_PLAYER_NATURE) != NA_BGM_DISABLED) && (Audio_GetActiveSequence(SEQ_PLAYER_NATURE) != NA_BGM_NATURE_AMBIENCE)) {
        Audio_StopSequence(SEQ_PLAYER_NATURE, 0);
        Audio_QueueCmdS32(0xF8 << 24, 0);
    }

    if (Audio_GetActiveSequence(SEQ_PLAYER_BGM_SUB) == (NA_BGM_ENEMY | 0x800)) {
        Audio_SetVolumeScale(SEQ_PLAYER_BGM_MAIN, 3, 0x7F, 1);
    }

    AudioSeqCmd_StartSequence(SEQ_PLAYER_NATURE, 0, NA_BGM_NATURE_AMBIENCE);

    for (channelIdx = 0; channelIdx < 16; channelIdx++) {
        if (!(channelMask & (1 << channelIdx)) && (playerIO & (1 << channelIdx))) {
            AudioSeqCmd_SetChannelIO(SEQ_PLAYER_NATURE, CHANNEL_IO_PORT_1, channelIdx, 1);
        }
    }
}

// OoT func_800F6FB4
void Audio_PlayNatureAmbienceSequence(u8 natureSeqId) {
    u8 i = 0;
    u8 channelIdx;
    u8 port;
    u8 val;

    // No seq flags are set to 0x80
    if ((gActiveSeqs[SEQ_PLAYER_NATURE].seqId == NA_BGM_DISABLED) || !(sSeqFlags[((u8)(gActiveSeqs[SEQ_PLAYER_NATURE].seqId ^ 0)) & 0xFF] & 0x80)) {
        if (gActiveSeqs[SEQ_PLAYER_NATURE].seqId != NA_BGM_NATURE_AMBIENCE) {
            D_801FD438 = gActiveSeqs[SEQ_PLAYER_NATURE].seqId;
        }

        Audio_StartNatureAmbienceSequence(sNatureAmbienceData[natureSeqId].playerIO, sNatureAmbienceData[natureSeqId].channelMask);

        while ((sNatureAmbienceData[natureSeqId].channelIO[i] != 0xFF) &&
               (i < ARRAY_COUNT(sNatureAmbienceData[natureSeqId].channelIO))) {
            channelIdx = sNatureAmbienceData[natureSeqId].channelIO[i++];
            port = sNatureAmbienceData[natureSeqId].channelIO[i++];
            val = sNatureAmbienceData[natureSeqId].channelIO[i++];
            AudioSeqCmd_SetChannelIO(SEQ_PLAYER_NATURE, port, channelIdx, val);
        }

        AudioSeqCmd_SetChannelIO(SEQ_PLAYER_NATURE, CHANNEL_IO_PORT_7, NATURE_CHANNEL_UNK, sAudioFileSelectSetting);
    }
}

// TODO: UB return
u32 Audio_SetNatureAmbienceBend(u8 channelIdx, SequenceChannel* channel) {
    u32 ret;

    gAudioContext.seqPlayers[SEQ_PLAYER_NATURE].applyBend = true;
    if ((channelIdx == 0) || (channelIdx == 0xFF)) {
        gAudioContext.seqPlayers[SEQ_PLAYER_NATURE].bend = 1.0f;
        // return channelIdx;
    } else {
        ret = Audio_NextRandom();
        gAudioContext.seqPlayers[SEQ_PLAYER_NATURE].bend = 1.0293 - ((gAudioContext.audioRandom & 0xF) / 144.0f);
        return ret;
    }
}


// ======== END NATURE AMBIENCE FUNCTIONS ========


// OoT func_800F70F8
void Audio_Init(void) {
    AudioLoad_Init(NULL, 0);
}

void func_801A4C54(u16 fadeTimer) {
    u8 channelIdx;

    Audio_ScheduleProcessCmds();
    Audio_StartSequence(SEQ_PLAYER_SFX, NA_BGM_GENERAL_SFX, 0x70, fadeTimer);
    for (channelIdx = 0; channelIdx < ARRAY_COUNT(sSfxChannelState); channelIdx++) {
        Audio_QueueCmdS32(0x10 << 24 | SEQ_PLAYER_SFX << 16 | ((u8)(u32)channelIdx << 8), &sSfxChannelState[channelIdx]);
    }
    Audio_QueueCmdS32(0xE4000000, func_8019F024);
    Audio_QueueCmdS32(0xE4000001, Audio_SetNatureAmbienceBend);
}

// OoT func_800F711C
void Audio_InitSfx(void) {
    Audio_ResetData();
    AudioOcarina_ResetStaff();
    Audio_ResetSfxChannelState();
    Audio_ResetSequencesAndVolume();
    Audio_ResetSfxs();
    AudioVoice_ResetData();
    func_801A4C54(10);
}

// OoT func_800F7170
void func_801A4D50(void) {
    func_801A4C54(1);
    Audio_QueueCmdS32(0xF2FF0000, 1);
    Audio_ScheduleProcessCmds();
    Audio_QueueCmdS32(0xF8000000, 0);
    sIsFinalHoursOrSoaring = false;
    sSeqAtFixedPosSeqId = NA_BGM_GENERAL_SFX;
    sMuteOnlySfxAndNatureSeq = false;
}

// New to MM
// gReverbSettingsTable[gAudioSpecId][1] somehow
extern ReverbSettings* gReverbSettingsTable[10];
void func_801A4DA4(void) {
    func_801A4D50();
    if (gAudioSpecId < 10) {
        Audio_QueueCmdS32(0xE6000100, (s32)(gReverbSettingsTable[gAudioSpecId] + 1));
    }
}

// OoT func_800F71BC
void func_801A4DF4(s32 specId) {
    D_801DB4D8 = 1;
    Audio_ResetData();
    AudioOcarina_ResetStaff();
    Audio_ResetSfxChannelState();
    Audio_ResetSequences();
    Audio_ResetSfxs();
    func_801A4FD8();
    if (gAudioSpecId == 0xB) {
        Audio_SetSfxBanksMute(0x6F);
    }
}

// func_800F7208
void func_801A4E64(void) {
    Audio_ResetSequences();
    Audio_QueueCmdS32(0xF2FF0000, 1);
    Audio_ResetData();
    Audio_ResetSfxChannelState();
    func_801A4C54(1);
    func_801A4FD8();
}
