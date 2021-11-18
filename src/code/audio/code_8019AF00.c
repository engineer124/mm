#include "global.h"

// TODO: can these macros be shared between files? code_800F9280 seems to use
// versions without any casts...
#define Audio_DisableSeq(playerIdx, fadeOut) Audio_QueueCmdS32(0x83000000 | ((u8)playerIdx << 16), fadeOut)
#define Audio_StartSeq(playerIdx, fadeTimer, seqId) \
    Audio_QueueSeqCmd(0x00000000 | ((u8)playerIdx << 24) | ((u8)(fadeTimer) << 0x10) | (u16)seqId)
#define Audio_SeqCmd7(playerIdx, a, b) \
    Audio_QueueSeqCmd(0x70000000 | ((u8)playerIdx << 0x18) | ((u8)a << 0x10) | (u8)(b))
#define Audio_SeqCmdC(playerIdx, a, b, c) \
    Audio_QueueSeqCmd(0xC0000000 | ((u8)playerIdx << 24) | ((u8)a << 16) | ((u8)b << 8) | ((u8)(c)))
#define Audio_SeqCmdA(playerIdx, a) Audio_QueueSeqCmd(0xA0000000 | ((u8)playerIdx << 24) | ((u16)(a)))
#define Audio_SeqCmd1(playerIdx, a) Audio_QueueSeqCmd(0x100000FF | ((u8)playerIdx << 24) | ((a) << 16))
#define Audio_SeqCmdB(playerIdx, a, b, c) \
    Audio_QueueSeqCmd(0xB0000000 | ((u8)playerIdx << 24) | ((u8)a << 16) | ((u8)b << 8) | ((u8)c))
#define Audio_SeqCmdB40(playerIdx, a, b) Audio_QueueSeqCmd(0xB0004000 | ((u8)playerIdx << 24) | ((u8)a << 16) | ((u8)b))
#define Audio_SeqCmd6(playerIdx, a, b, c) \
    Audio_QueueSeqCmd(0x60000000 | ((u32)playerIdx << 24) | ((u32)(a) << 16) | ((u32)b << 8) | ((u8)c))
#define Audio_SeqCmdE0(playerIdx, a) Audio_QueueSeqCmd(0xE0000000 | ((u8)playerIdx << 24) | (a))
#define Audio_SeqCmdE01(playerIdx, a) Audio_QueueSeqCmd(0xE0000100 | ((u8)playerIdx << 24) | ((u16)a))
#define Audio_SeqCmd8(playerIdx, a, b, c) Audio_QueueSeqCmd(0x80000000 | (playerIdx << 24) | (a << 16) | (b << 8) | (c))
#define Audio_SeqCmdF(playerIdx, a) Audio_QueueSeqCmd(0xF0000000 | ((u8)playerIdx << 24) | ((u8)a))

typedef struct {
    /* 0x0 */ s8 x;
    /* 0x1 */ s8 y;
} OcarinaControlStick; // size = 0x2

typedef struct {
    /* 0x0 */ u16 unk_00;
    /* 0x2 */ u16 unk_02;
    /* 0x4 */ u8 unk_04[100];
} NatureAmbienceData; // size = 0x68

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

typedef struct {
    /* 0x0 */ f32 value;
    /* 0x4 */ f32 target;
    /* 0x8 */ f32 step;
    /* 0xC */ s32 remainingFrames;
} FreqLerp; // size = 0x10

s32 AudioOcarina_MemoryGameGenerateNotes(void);
void func_801A1A10(u8 arg0, u8 arg1);
void func_801A257C(u16 seqId);
void func_801A2670(u16 seqId);
void func_801A3238(s8 playerIdx, u16 seqId, u8 fadeTimer, s8 arg3, u8 arg4);
void func_801A4FD8(void);
void func_801A7D04(s32, s32);
void func_801A7B10(u8 playerIdx, u8 seqId, u8 arg2, u16 fadeTimer);
s32 func_801A8ABC(u32 arg0, u32 arg1);
void func_801A9A74(void);
void AudioVoice_ResetData(void);
void func_801A99B8(void);
void Audio_ProcessSeqCmds(void);
void func_801A4428(u8 arg0);
void func_801A3038(void);
void Audio_SetupBgmNatureAmbience(u8 natureSeqId);
s32 func_801A0318(u8);
void Audio_StepFreqLerp(FreqLerp* lerp); // extern
void func_8019D864(void);                // extern
void func_8019F300(void);                // extern
void func_8019FEDC(void);                // extern
void func_801A046C(void);                // extern
void func_801A1290(void);                // extern
void func_801A1904(void);                // extern
void func_801A1E0C(void);                // extern
void func_801A2090(void);                // extern
void func_801A2778(void);                // extern
void func_801A312C(void);                // extern
void func_801A3AC0(void);                // extern
void func_801A44C4(void);                // extern
void func_801A5118(void);                // extern
void func_801A8D5C(void);                // extern
s32 func_801A9768(void);                 // extern
s32 func_801A982C(void);                 // extern

// bss
SfxSettings sSfxSettings[8];
u8 D_801FD250;
f32 gLoweredSfxFreq;
s8 sIncreasedSfxReverb;
f32 D_801FD25C;
f32 D_801FD260;
f32 D_801FD264;
FreqLerp sRiverFreqScaleLerp;
FreqLerp sWaterfallFreqScaleLerp;
f32 D_801FD288;
s8 D_801FD28C;
u8 D_801FD28D;
u8 D_801FD28E;
u8 D_801FD28F;
u8 D_801FD290;
u8 D_801FD291;
f32* D_801FD294;
f32 D_801FD298;
f32 D_801FD29C;
u16 D_801FD2A0;
SfxPlayerState sSfxChannelState[16];
u8 D_801FD3A8;
u8 D_801FD3A9;
u8 D_801FD3AA;
u8 D_801FD3AB;
u16 D_801FD3AC;
u8 D_801FD3AE;
u8 D_801FD3AF;
u8 D_801FD3B0;
u8 D_801FD3B4[4];
u8 D_801FD3B8[0x20];
u8 D_801FD3D8;
u8 D_801FD3D9;
u8 D_801FD3DA;
Vec3f D_801FD3E0;
u8 D_801FD3EC;
Vec3f D_801FD3F0;
s16 D_801FD3FC;
f32 D_801FD400;
f32 D_801FD404;
f32 D_801FD408;
f32 D_801FD40C;
Vec3f D_801FD410;
Vec3f D_801FD420;
f32 D_801FD42C;
u8 D_801FD430;
u8 D_801FD431;
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
u32 sCurOcarinaInputButtonIdx; // TODO: Not Oca Btn Idx, but Controller Btn Idx!
u32 D_801FD454; // Controller Btn Idx
u32 sPrevOcarinaInputButtonIdx; // TODO: Not Oca Btn Idx, but Controller Btn Idx!
s32 sOcaInputBtnPress;
u8 D_801FD460;
u8 D_801FD461;
u8 sOcarinaHasStartedSong;
u8 sOcarinaSongNoteStartIdx;
u8 sOcarinaSongCount;
u32 sOcarinaAvailSongs;
u8 sStaffPlayingPos;
u16 sLearnSongPos[OCARINA_SONG_MAX];
u16 D_801FD4A0[OCARINA_SONG_MAX];
u16 sLearnSongExpectedLength[OCARINA_SONG_MAX];
u8 sLearnSongExpectedNoteIdx[OCARINA_SONG_MAX];
u8 D_801FD518[OCARINA_SONG_MAX];
u32 D_801FD530[OCARINA_SONG_MAX];
OcarinaNote sRecordingSongNote;
u16 D_801FD598;

// data
u8 D_801D6600[] = {
	true, false, true, true, false, false, true,
};

// OoT D_80130570
u8 gIsLargeSoundBank[] = {
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
f32 D_801D6654 = 1.0f;
s8 D_801D6658 = 20;
s8 D_801D665C = 30;
s8 D_801D6660 = 20;
f32 sBehindScreenZ[2] = { -15.0f, -65.0f }; // Unused Remnant of OoT
u8 sAudioIncreasingTranspose = 0; // Remnant of OoT, only unsed in unused functions
u8 gMorphaTransposeTable[16] = { 0, 0, 0, 1, 1, 2, 4, 6, 8, 8, 8, 8, 8, 8, 8, 8 }; // Unused Remnant of OoT
u8 sPrevChargeLevel = 0;
f32 sChargeLevelsSfxFreq[] = {
	1.0f, 1.12246f, 1.33484f, 1.33484f
};
f32 sCurChargeLevelSfxFreq = 1.0f;
u8 D_801D6698[] = { 
	127, 80, 75, 73, 70, 68, 65, 60 // volumes
};
u8 D_801D66A0 = 0;
u16 D_801D66A4 = 0;
s8 D_801D66A8 = 0;
s8 D_801D66AC = 0;
s8 sAudioCutsceneFlag = false;
s8 sSpecReverb = 0;
s8 sAudioEnvReverb = 0;
s8 sAudioCodeReverb = 0;
u8 D_801D66C0 = 0;
f32 sAudioEnemyDist = 0.0f;
s8 sAudioEnemyVol = 127;
u16 D_801D66CC = NA_BGM_DISABLED;
u8 D_801D66D0 = 0;
u8 D_801D66D4 = NA_BGM_GENERAL_SFX;
u32 D_801D66D8 = 0;
u32 D_801D66DC = 0;
u8 sAudioBaseFilter = 0;
u8 sAudioExtraFilter = 0;
u8 sAudioBaseFilter2 = 0;
u8 sAudioExtraFilter2 = 0;
s8 D_801D66F0 = 0;
Vec3f* sSariaBgmPtr = NULL;
f32 D_801D66F8 = 2000.0f;
u8 D_801D66FC = NA_BGM_GENERAL_SFX;

// OoT D_80130658
u8 sBgmFlags[] = {
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
    0, 0, 0, 0, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// OoT D_801306DC[20]
NatureAmbienceData sNatureAmbienceData[20] = {
    // natureSeqId: 0
    { 0xC0FF, 0xC0FE, { 0, 2, 0,  0, 3, 0,   1, 2, 9,   1, 3, 64, 1, 4, 0,  1, 5, 32,  2, 2, 4,   2, 3, 0,  2,   4, 1,
                        2, 5, 16, 3, 2, 10,  3, 3, 112, 3, 4, 1,  3, 5, 48, 4, 2, 3,   4, 3, 127, 4, 4, 0,  4,   5, 16,
                        5, 2, 0,  5, 3, 127, 5, 4, 1,   5, 5, 16, 6, 2, 1,  6, 3, 127, 6, 4, 3,   6, 5, 16, 0xFF } },
    // natureSeqId: 1
    { 0xC0FF, 0xC0FE, { 0, 2, 0,  0, 3, 0,   1, 2, 4,   1, 3, 64, 1, 4, 0,  1, 5, 32,  2, 2, 16,  2, 3, 0,  2,   4, 1,
                        2, 5, 16, 3, 2, 12,  3, 3, 112, 3, 4, 0,  3, 5, 48, 4, 2, 15,  4, 3, 127, 4, 4, 1,  4,   5, 16,
                        5, 2, 6,  5, 3, 127, 5, 4, 1,   5, 5, 16, 6, 2, 1,  6, 3, 127, 6, 4, 3,   6, 5, 16, 0xFF } },
    // natureSeqId: 2
    { 0xC0FF, 0xC0FE, { 0, 2, 0,  0, 3, 0,  1, 2, 4,  1, 3, 64, 1, 4, 0,  1, 5, 48,  2, 2, 10,  2, 3, 0,  2,   4, 1,
                        2, 5, 16, 3, 2, 4,  3, 3, 48, 3, 4, 1,  3, 5, 32, 4, 2, 3,   4, 3, 127, 4, 4, 0,  4,   5, 16,
                        5, 2, 1,  5, 3, 64, 5, 4, 1,  5, 5, 0,  6, 2, 4,  6, 3, 127, 6, 4, 0,   6, 5, 63, 0xFF } },
    // natureSeqId: 3
    { 0xC0FF,
      0xC0FE,
      { 0, 2, 0,  0, 3, 0,   1, 2, 9,  1, 3, 64, 1, 4, 0,  1, 5, 32,  2, 2, 4,  2, 3, 64, 2,   4, 0,
        2, 5, 48, 3, 2, 10,  3, 3, 32, 3, 4, 1,  3, 5, 32, 4, 2, 14,  4, 3, 64, 4, 4, 1,  4,   5, 16,
        5, 2, 0,  5, 3, 127, 5, 4, 1,  5, 5, 16, 6, 2, 1,  6, 3, 127, 6, 4, 3,  6, 5, 16, 0xFF

      } },
    // natureSeqId: 4
    { 0xC0FF,
      0xC0FE,
      { 0, 2,  0, 0,  3,  0, 1, 2,   4, 1,  3, 64, 1, 4,  0, 1,   5,  32, 2, 2,  2, 2,  3,  64, 2,  4,   1, 2,
        5, 16, 3, 2,  12, 3, 3, 112, 3, 4,  1, 3,  5, 48, 4, 2,   13, 4,  3, 64, 4, 4,  1,  4,  5,  16,  5, 2,
        1, 5,  3, 64, 5,  4, 1, 5,   5, 16, 6, 2,  2, 6,  3, 112, 6,  4,  0, 6,  5, 48, 14, 4,  63, 0xFF

      } },
    // natureSeqId: 5
    { 0xC0FF,
      0xC0FE,
      { 0, 2, 0,  0, 3, 0,   1, 2, 9,   1, 3, 64, 1, 4, 0,  1, 5, 32,  2, 2, 4,   2, 3, 0,  2,   4, 1,
        2, 5, 16, 3, 2, 10,  3, 3, 112, 3, 4, 1,  3, 5, 48, 4, 2, 13,  4, 3, 127, 4, 4, 0,  4,   5, 63,
        5, 2, 0,  5, 3, 127, 5, 4, 1,   5, 5, 16, 6, 2, 1,  6, 3, 127, 6, 4, 3,   6, 5, 16, 0xFF

      } },
    // natureSeqId: 6
    { 0xC0FF, 0xC0FE, { 0, 2, 0,  0, 3, 0,   1, 2, 11, 1, 3, 112, 1, 4, 0,  1, 5, 48,  2, 2, 15, 2, 3, 112, 2,   4, 0,
                        2, 5, 63, 3, 2, 11,  3, 3, 48, 3, 4, 1,   3, 5, 16, 4, 2, 14,  4, 3, 48, 4, 4, 1,   4,   5, 16,
                        5, 2, 11, 5, 3, 127, 5, 4, 0,  5, 5, 32,  6, 2, 2,  6, 3, 127, 6, 4, 0,  6, 5, 48,  0xFF } },
    // natureSeqId: 7
    { 0xC001, 0xC000, { 0, 2, 0, 0, 3, 0, 0xFF } },
    // natureSeqId: 8
    { 0xC003, 0xC000, { 0, 2, 0, 0, 3, 0, 1, 2, 1, 1, 3, 127, 1, 4, 3, 1, 5, 16, 0xFF } },
    // natureSeqId: 9
    { 0xC00F, 0xC000, { 0, 2,   0, 0, 3, 0, 1, 2,  16, 1, 3,  0, 1, 4,   2, 1, 5, 16, 2, 2,  12,  2,
                        3, 112, 2, 4, 0, 2, 5, 48, 3,  2, 15, 3, 3, 127, 3, 4, 1, 3,  5, 16, 0xFF } },
    // natureSeqId: 10
    { 0xC081, 0xC000, { 0, 2, 1, 0, 3, 8, 7, 2, 11, 7, 3, 112, 7, 4, 2, 7, 5, 32, 0xFF } },
    // natureSeqId: 11
    { 0xC00F, 0xC000, { 0, 2,   3, 0, 3, 8, 1, 2,  1, 1, 3, 127, 1, 4,   3, 1, 5, 16, 2, 2,  0,   2,
                        3, 127, 2, 4, 2, 2, 5, 16, 3, 2, 6, 3,   3, 127, 3, 4, 1, 3,  5, 16, 0xFF } },
    // natureSeqId: 12
    { 0xC007, 0xC000, { 0, 2, 0, 1, 2, 0, 1, 3, 127, 1, 4, 1, 1, 5, 16, 2, 2, 1, 2, 3, 127, 2, 4, 3, 2, 5, 16, 0xFF } },
    // natureSeqId: 13
    { 0xC003, 0xC000, { 0, 2, 0, 0, 3, 0, 1, 2, 4, 1, 3, 0, 1, 4, 1, 1, 5, 16, 0xFF } },
    // natureSeqId: 14
    { 0xC003, 0xC000, { 0, 2, 0, 0, 3, 0, 1, 2, 4, 1, 3, 0, 1, 4, 1, 1, 5, 16, 0xFF } },
    // natureSeqId: 15
    { 0xC003, 0xC000, { 0, 2, 0, 0, 3, 0, 1, 2, 4, 1, 3, 0, 1, 4, 1, 1, 5, 16, 0xFF } },
    // natureSeqId: 16
    { 0xC003, 0xC000, { 0, 2, 0, 0, 3, 0, 1, 2, 4, 1, 3, 0, 1, 4, 1, 1, 5, 16, 0xFF } },
    // natureSeqId: 17
    { 0xC003, 0xC000, { 0, 2, 0, 0, 3, 0, 1, 2, 4, 1, 3, 0, 1, 4, 1, 1, 5, 16, 0xFF } },
    // natureSeqId: 18
    { 0xC000, 0xC000, { 0, 2, 0, 0xFF } },
    // natureSeqId: 19
    { 0xC003, 0xC000, { 0, 2, 0, 0, 3, 0, 1, 2, 4, 1, 3, 0, 1, 4, 1, 1, 5, 16, 0xFF } },
};

u8 sIsOcarinaInputEnabled = false;
s8 D_801D6FB8 = 0;
u8 sCurOcarinaNoteIdx = NOTE_NONE;
u8 sPrevOcarinaNoteIdx = 0;
u8 sCurOcarinaButtonIdx = 0;
u8 sLearnSongPrevNoteIdx = 0;
f32 sCurOcarinaBendFreq = 1.0f;
f32 sDefaultOcarinaVolume = 0.68503935f;
s8 sCurOcarinaBendIdx = 0;
s8 sCurOcarinaVolume = 0x57;
s8 sCurOcarinaVibrato = 0;
u8 sPlaybackState = 0;
u8 D_801D6FE4 = 0xFF;
u8 D_801D6FE8 = 0xFF;
u32 sOcarinaSongFlags = 0;
s32 sNotePlaybackTimer = 0;
u16 sPlaybackNotePos = 0;
u16 sStaffPlaybackPos = 0;
u32 sPrevOcarinaSongFlags = 0; // Stores sOcarinaSongFlags but never used
u8 sDisplayedNoteValue = NOTE_NONE;
u8 sNotePlaybackVolume = 0;
u8 sNotePlaybackVibrato = 0;
s8 sNotePlaybackBend = 0;
f32 sNormalizedNotePlaybackTone = 1.0f;
f32 sNormalizedNotePlaybackVolume = 1.0f;
u32 sOcarinaPlaybackTaskStart = 0;
u32 sOcarinaWallCounter = 0;
u8 sCurOcarinaSong[8] = {
	NOTE_C4, NOTE_C4, NOTE_C4, NOTE_C4,
	NOTE_C4, NOTE_C4, NOTE_C4, NOTE_C4,
};
u8 sOcarinaSongAppendPos = 0;
u8 D_801D702C = 0;

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

OcarinaNote* sPlaybackSong = sOcarinaSongNotes[OCARINA_SONG_SONATA];
u8 D_801D84F4[14] = { 0, 3, 2, 1, 3, 2, 1, 0, 1, 0, 1, 2, 3, 0 };
u8* gFrogsSongPtr = D_801D84F4;
u8 sRecordingState = 0;
u8 sRecordSongPos = 0;
u32 sOcarinaRecordTaskStart = 0;
u8 sRecordOcarinaNoteIdx = 0;
u8 sRecordOcarinaVolume = 0;
u8 sRecordOcarinaVibrato = 0;
s8 sRecordOcarinaBendIdx = 0;
u8 sRecordOcarinaButtonIdx = 0;
u8 D_801D8528 = 0;
u8 D_801D852C = 0;
u8 D_801D8530 = false;
u32 D_801D8534 = 0;
u8 D_801D8538 = 0;

OcarinaNote sPierresSongNotes[108] = {
	{ NOTE_NONE, 0, 0, 0, 0, 0 },
    { NOTE_NONE, 0, 0, 0, 0, 0 },
};

OcarinaNote* gScarecrowCustomSongPtr = sPierresSongNotes;
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
// seqData
u8 D_801D88B8[400] = {
    0xFE, 0xFE, 0xD3, 0x20, 0xD7, 0x00, 0x01, 0xCC, 0x00, 0x70, 0x90, 0x00, 0x16, 0xDB, 0x64, 0xDD, 0x78,
    0xFE, 0x00, 0xF3, 0xFC, 0xFF, 0xC3, 0x88, 0x00, 0x29, 0x89, 0x00, 0x2B, 0xDF, 0x7F, 0xE9, 0x0F, 0xDD,
    0x37, 0xD4, 0x40, 0xC1, 0x52, 0xFE, 0x80, 0xF3, 0xFC, 0xFF, 0xC2, 0xFB, 0xC0, 0x00, 0xC1, 0x57, 0xC9,
};

OcarinaSongButtons gOcarinaSongButtons[OCARINA_SONG_MAX] = { 
	// 0: Sonata of Awakening
	{ 7, 
		{ 
			OCARINA_BTN_C_UP, 
			OCARINA_BTN_C_LEFT, 
			OCARINA_BTN_C_UP, 
			OCARINA_BTN_C_LEFT, 
			OCARINA_BTN_A, 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_A 
		}
	},
	
	// 1: Goron Lullaby
	{ 8, 
		{ 
			OCARINA_BTN_A, 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_C_LEFT, 
			OCARINA_BTN_A, 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_C_LEFT, 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_A 
		}
	},

	// 2: New Wave Bossa Nova
	{ 7, 
		{ 
			OCARINA_BTN_C_LEFT, 
			OCARINA_BTN_C_UP, 
			OCARINA_BTN_C_LEFT, 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_C_DOWN, 
			OCARINA_BTN_C_LEFT, 
			OCARINA_BTN_C_RIGHT 
		}
	},

	// 3: Elegy of Emptyness
	{ 7, 
		{ 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_C_LEFT, 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_C_DOWN, 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_C_UP, 
			OCARINA_BTN_C_LEFT 
		}
	},

	// 4: Oath to Order
	{ 6, { 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_C_DOWN, 
			OCARINA_BTN_A, 
			OCARINA_BTN_C_DOWN, 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_C_UP }},

	// 5; Sarias Song
	{ 6, { 
			OCARINA_BTN_C_DOWN, 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_C_LEFT, 
			OCARINA_BTN_C_DOWN, 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_C_LEFT }},

	// 6: Song of Time
	{ 6, { 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_A, 
			OCARINA_BTN_C_DOWN, 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_A, 
			OCARINA_BTN_C_DOWN }},

	// 7: Song of Healing
	{ 6, { 
			OCARINA_BTN_C_LEFT, 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_C_DOWN, 
			OCARINA_BTN_C_LEFT, 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_C_DOWN }},

	// 8: Eponas Song
	{ 6, { 
			OCARINA_BTN_C_UP, 
			OCARINA_BTN_C_LEFT, 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_C_UP, 
			OCARINA_BTN_C_LEFT, 
			OCARINA_BTN_C_RIGHT }},

	// 9: Song of Soaring
	{ 6, { 
			OCARINA_BTN_C_DOWN, 
			OCARINA_BTN_C_LEFT, 
			OCARINA_BTN_C_UP, 
			OCARINA_BTN_C_DOWN, 
			OCARINA_BTN_C_LEFT, 
			OCARINA_BTN_C_UP }},

	// 10: Song of Storms
	{ 6, { 
			OCARINA_BTN_A, 
			OCARINA_BTN_C_DOWN, 
			OCARINA_BTN_C_UP, 
			OCARINA_BTN_A, 
			OCARINA_BTN_C_DOWN, 
			OCARINA_BTN_C_UP }},

	// 11: Suns Song
	{ 6, { 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_C_DOWN, 
			OCARINA_BTN_C_UP, 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_C_DOWN, 
			OCARINA_BTN_C_UP }},

	// 12: Inverted Song of Time
	{ 6, { 
			OCARINA_BTN_C_DOWN, 
			OCARINA_BTN_A, 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_C_DOWN, 
			OCARINA_BTN_A, 
			OCARINA_BTN_C_RIGHT }},

	// 13; Song of Double Time
	{ 6, { 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_A, 
			OCARINA_BTN_A, 
			OCARINA_BTN_C_DOWN, 
			OCARINA_BTN_C_DOWN }},

	// 14: Goron Lullaby Intro
	{ 6, { 
			OCARINA_BTN_A, 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_C_LEFT,
			 OCARINA_BTN_A, 
			 OCARINA_BTN_C_RIGHT, 
			 OCARINA_BTN_C_LEFT }},

	// 15: Milk Bar Jam "Ballad of the Wind Fish" Human
	{ 4, { 
			OCARINA_BTN_C_UP, 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_C_LEFT, 
			OCARINA_BTN_C_RIGHT }},

	// 16: Milk Bar Jam "Ballad of the Wind Fish" Goron
	{ 8, { 
			OCARINA_BTN_A, 
			OCARINA_BTN_A, 
			OCARINA_BTN_A, 
			OCARINA_BTN_C_DOWN, 
			OCARINA_BTN_A, 
			OCARINA_BTN_A, 
			OCARINA_BTN_A, 
			OCARINA_BTN_C_RIGHT }},

	// 17: Milk Bar Jam "Ballad of the Wind Fish" Zora
	{ 8, { 
			OCARINA_BTN_C_UP, 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_C_DOWN, 
			OCARINA_BTN_A, 
			OCARINA_BTN_C_UP, 
			OCARINA_BTN_C_LEFT, 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_C_DOWN }},

	// 18: Milk Bar Jam "Ballad of the Wind Fish" Deku
	{ 5, { 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_A, 
			OCARINA_BTN_C_DOWN, 
			OCARINA_BTN_C_LEFT, 
			OCARINA_BTN_C_RIGHT }},

	// 19: Evan HP (Zora Band Leader) Song Part 1
	{ 8, { 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_C_DOWN, 
			OCARINA_BTN_A, 
			OCARINA_BTN_A, 
			OCARINA_BTN_C_DOWN, 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_A }},

	// 20: Evan HP (Zora Band Leader) Song Part 2
	{ 8, { 
			OCARINA_BTN_C_LEFT, 
			OCARINA_BTN_C_LEFT, 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_C_DOWN, 
			OCARINA_BTN_C_DOWN, 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_C_LEFT, 
			OCARINA_BTN_C_DOWN }},

	// 21: Zeldas Lullaby
	{ 6, { 
			OCARINA_BTN_C_LEFT, 
			OCARINA_BTN_C_UP, 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_C_LEFT,
			 OCARINA_BTN_C_UP, 
			 OCARINA_BTN_C_RIGHT }},

	// 22: Scarecrow
	{ 8, { 
			OCARINA_BTN_A, 
			OCARINA_BTN_A, 
			OCARINA_BTN_A, 
			OCARINA_BTN_A, 
			OCARINA_BTN_A, 
			OCARINA_BTN_A, 
			OCARINA_BTN_A, 
			OCARINA_BTN_A }},

	// 23: Termina Field 2D Song Buttons Appearing on Wall (In OoT, this is Ocarina Memory Game)
	{ 8, { 
			OCARINA_BTN_A, 
			OCARINA_BTN_C_DOWN, 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_C_LEFT, 
			OCARINA_BTN_C_UP, 
			OCARINA_BTN_C_LEFT, 
			OCARINA_BTN_C_RIGHT, 
			OCARINA_BTN_C_DOWN }},
};

// OoT's soundEffects from EnRiverSound
const u16 D_801E0BD0[] = {
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

const char D_801E0C14[] = "key step is too long !!! %d:%d>%d\n";
const char D_801E0C38[] = "You played %d Melody !!! (last step:%d)\n";
const char D_801E0C64[] = "pass 0\n";
const char D_801E0C6C[] = "pass 1\n";
const char D_801E0C74[] = "pass 2\n";
const char D_801E0C7C[] = "last key is bad !!! %d %d %02X %02X\n";
const char D_801E0CA4[] = "last key step is too short !!! %d:%d %d<%d\n";
const char D_801E0CD0[] = "check is over!!! %d %d %d\n";


void AudioOcarina_ReadControllerInput(void) {
    Input inputs[4];
    Input* controller1 = &inputs[0];
    u32 prevOcarinaInputButtonIdx = sCurOcarinaInputButtonIdx;

    Padmgr_GetInput2(inputs, 0);
    sCurOcarinaInputButtonIdx = controller1->cur.button;
    sPrevOcarinaInputButtonIdx = prevOcarinaInputButtonIdx;
    sOcarinaInputStickRel.x = controller1->rel.stick_x;
    sOcarinaInputStickRel.y = controller1->rel.stick_y;
}

/**
 * Looks up the pitch frequency to bend the note by.
 * The note will bend up to a maximum of 2 semitones
 * in each direction giving a total range of 4 semitones
 */
f32 AudioOcarina_BendPitchTwoSemitones(s8 bendIdx) {
    s8 scaledBendIdx;
    f32 bendFreq;

    if (bendIdx > 64) {
        scaledBendIdx = 127;
    } else if (bendIdx < -64) {
        scaledBendIdx = -128;
    } else if (bendIdx >= 0) {
        scaledBendIdx = (bendIdx * 127) / 64;
    } else {
        scaledBendIdx = (bendIdx * 128) / 64;
    }

    /**
     * index 128 is in the middle of the table and
     * contains the value 1.0f i.e. no bend
     * absolute indices above 128 will bend the pitch upwards
     * absolute indices below 128 will bend the pitch downwards
     */
    bendFreq = gBendPitchTwoSemitonesFrequencies[scaledBendIdx + 128];
    return bendFreq;
}

// OoT func_800ECAF0
u8 func_8019AFE8(void) {
    u8 ret;

    if (D_801D8528 != 0) {
        ret = D_801D8528 - 1;
        D_801D8528 = 0;
    } else if (sOcarinaSongFlags) {
        ret = 0xFE;
    } else {
        ret = 0xFF;
    }

    return ret;
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

void AudioOcarina_StartOcarina(u32 ocarinaSongFlag) {
    u8 songIdx;

    if ((sOcarinaSongNotes[OCARINA_SONG_SCARECROW][1].volume != 0xFF) && ((ocarinaSongFlag & OCARINA_SONGS_PLAYABLE_FLAGS) == OCARINA_SONGS_PLAYABLE_FLAGS)) {
        ocarinaSongFlag |= (1 << OCARINA_SONG_SCARECROW);
    }

    if ((ocarinaSongFlag == (0xC0000000 | OCARINA_SONGS_PLAYABLE_FLAGS)) && (sOcarinaSongNotes[OCARINA_SONG_SCARECROW][1].volume != 0xFF)) {
        ocarinaSongFlag = (0xC0000000 | OCARINA_SONGS_PLAYABLE_FLAGS);
    }

    if ((ocarinaSongFlag == OCARINA_SONGS_PLAYABLE_FLAGS) && (sOcarinaSongNotes[OCARINA_SONG_SCARECROW][1].volume != 0xFF)) {
        ocarinaSongFlag = OCARINA_SONGS_PLAYABLE_FLAGS;
    }

    if (ocarinaSongFlag != 0xFFFF) {
        sOcarinaSongFlags = 0x80000000 + (u32)ocarinaSongFlag;
        sOcarinaSongNoteStartIdx = OCARINA_SONG_SONATA;
        sOcarinaSongCount = OCARINA_SONG_MAX;
        sOcarinaAvailSongs = ocarinaSongFlag & 0xFFFFFF;
        D_801D852C = 8;
        sOcarinaHasStartedSong = false;
        D_801D8528 = 0;
        sStaffPlayingPos = 0;
        sPlayingStaff.state = func_8019AFE8();
        sIsOcarinaInputEnabled = true;
        sPrevOcarinaSongFlags = 0;
        for (songIdx = OCARINA_SONG_SONATA; songIdx <= OCARINA_SONG_EVAN_PART2; songIdx++) {
            for (sLearnSongPos[songIdx] = 0; sOcarinaSongNotes[songIdx][sLearnSongPos[songIdx]].noteIdx == NOTE_NONE;) {
                sLearnSongPos[songIdx]++;
            }

            D_801FD4A0[songIdx] = 0;
            sLearnSongExpectedLength[songIdx] = 0;
            sLearnSongExpectedNoteIdx[songIdx] = 0;

            if (D_801D8530) {
                if ((1 << songIdx) & ocarinaSongFlag) {
                    D_801FD518[songIdx] = 0;
                } else {
                    D_801FD518[songIdx] = 0xFF;
                }
            }
        }

        if (ocarinaSongFlag & 0x80000000) {
            D_801D852C = 0;
        }

        if (ocarinaSongFlag & 0x40000000) {
            sOcarinaSongAppendPos = 0;
        }

        if (ocarinaSongFlag & 0xC0400000) {
            AudioOcarina_MapSongFromNotesToButtons(OCARINA_SONG_SCARECROW, OCARINA_SONG_SCARECROW, 8);
        }
    } else {
        sOcarinaSongFlags = 0;
        sIsOcarinaInputEnabled = false;
    }
}

#undef OCARINA_SONGS_PLAYABLE_FLAGS

void func_8019B378(void) {
    D_801D702C = sOcarinaSongAppendPos;
}

void AudioOcarina_SetupStartOcarina1(u32 ocarinaSongFlag) {
    D_801D8530 = false;
    AudioOcarina_StartOcarina(ocarinaSongFlag);
    if (D_801D702C != 0) {
        sOcarinaSongAppendPos = D_801D702C;
        D_801D702C = 0;
    }
}

void AudioOcarina_SetupStartOcarina2(u32 ocarinaSongFlag, u8 arg1) {
    u8 i;
    u8 j;

    AudioOcarina_StartOcarina(ocarinaSongFlag);
    D_801D852C = 8;
    sStaffPlayingPos = arg1;

    for (i = 0; i <= OCARINA_SONG_EVAN_PART2; i++) {
        for (j = 0; j < arg1;) {
            if (sOcarinaSongNotes[i][sLearnSongPos[i]].noteIdx != NOTE_NONE) {
                j++;
            }
            sLearnSongPos[i]++;
        }

        if (sOcarinaSongNotes[i][sLearnSongPos[i]].noteIdx == NOTE_NONE) {
            sLearnSongPos[i]++;
        }
    }
}

void AudioOcarina_SetupStartOcarina3(u32 ocarinaSongFlag) {
    u8 songIdx;

    for (songIdx = OCARINA_SONG_SONATA; songIdx < OCARINA_SONG_MAX; songIdx++) {
        if ((1 << songIdx) & ocarinaSongFlag) {
            D_801FD530[songIdx] = 0;
        } else {
            D_801FD530[songIdx] = 0xFF;
        }
    }

    D_801D8530 = true;
    D_801D8534 = ocarinaSongFlag;
    AudioOcarina_StartOcarina(ocarinaSongFlag);
}

void AudioOcarina_SetupStartOcarina4(u32 ocarinaSongFlag) {
    D_801D8530 = false;
    AudioOcarina_StartOcarina(ocarinaSongFlag);
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

// OoT func_800ECDBC
void func_8019B618(void) {
    if (sCurOcarinaNoteIdx != NOTE_NONE && !sOcarinaHasStartedSong) {
        sOcarinaHasStartedSong = true;
        sLearnSongPrevNoteIdx = NOTE_NONE;
    }
}

// OoT part of func_800ED200
void func_8019B654(void) {
    u8 i;

    if (sOcarinaSongAppendPos == 8) {
        for (i = 0; i < ARRAY_COUNT(sCurOcarinaSong) - 1; i++) {
            sCurOcarinaSong[i] = sCurOcarinaSong[i + 1];
        }
    } else {
        sOcarinaSongAppendPos++;
    }
}

extern u8 D_801D701F[];

// OoT func_800ECDF8
void func_8019B6B4(void) {
    OcarinaNote* prevNote;
    u32 curOcarinaSongFlag;
    u8 sp5F;
    u8 phi_v0;
    u8 songIdx;
    s8 pad;
    s8 sp5B;
    u16* temp_t0;
    OcarinaNote* note;

    sp5F = 0;
    sp5B = 0;
    func_8019B618();

    if (!sOcarinaHasStartedSong) {
        return;
    }

    if (ABS_ALT(sCurOcarinaBendIdx) > 20) {
        sOcarinaSongFlags = 0;

        for (songIdx = OCARINA_SONG_SONATA; songIdx < OCARINA_SONG_MAX; songIdx++) {
            D_801FD518[songIdx] = 4;
        }
        sIsOcarinaInputEnabled = false;
        return;
    }

    if ((sPrevOcarinaNoteIdx == sCurOcarinaNoteIdx) || (sCurOcarinaNoteIdx == NOTE_NONE)) {
        sp5F = 1;
    } else {
        func_8019B654();
        sCurOcarinaSong[sOcarinaSongAppendPos - 1] = sCurOcarinaNoteIdx;
    }

    for (songIdx = sOcarinaSongNoteStartIdx; songIdx < sOcarinaSongCount; songIdx++) {
        curOcarinaSongFlag = 1 << songIdx;

        if (sOcarinaAvailSongs & curOcarinaSongFlag) {
            if (D_801D8530) {
                D_801FD4A0[songIdx] += sOcarinaUpdateTaskCurrent - sOcarinaPlaybackTaskStart;
            } else {
                D_801FD4A0[songIdx] = sLearnSongExpectedLength[songIdx] + 9;
            }

            if (sp5F) {
                if ((D_801FD4A0[songIdx] >= 9) && (sLearnSongPrevNoteIdx != sLearnSongExpectedNoteIdx[songIdx])) {
                    sOcarinaAvailSongs ^= curOcarinaSongFlag;
                    if (D_801D8530) {
                        D_801FD518[songIdx] = 1;
                    }
                }

                if (D_801D8530 && ((sLearnSongExpectedLength[songIdx] + 9) < D_801FD4A0[songIdx])) {
                    sOcarinaAvailSongs ^= curOcarinaSongFlag;
                    D_801FD518[songIdx] = 3;
                } else if ((D_801FD4A0[songIdx] >= (sLearnSongExpectedLength[songIdx] - 9)) && (D_801FD4A0[songIdx] >= sLearnSongExpectedLength[songIdx]) &&
                    (sOcarinaSongNotes[songIdx][sLearnSongPos[songIdx]].length == 0) &&
                    (sLearnSongPrevNoteIdx == sLearnSongExpectedNoteIdx[songIdx])) {
                    D_801D8528 = songIdx + 1;
                    sIsOcarinaInputEnabled = false;
                    sOcarinaSongFlags = 0;
                    if (D_801D8530) {
                        D_801FD518[songIdx] = 5;
                    }
                }
            } else {
                if (D_801FD4A0[songIdx] >= (sLearnSongExpectedLength[songIdx] - 9)) {
                    if (sLearnSongPrevNoteIdx != NOTE_NONE) {
                        if (sLearnSongPrevNoteIdx == sLearnSongExpectedNoteIdx[songIdx]) {
                            if (D_801D8530) {
                                D_801FD530[songIdx] += ABS_ALT(sLearnSongExpectedLength[songIdx] - D_801FD4A0[songIdx]);
                            }
                            D_801FD4A0[songIdx] = 0;
                        } else {
                            sOcarinaAvailSongs ^= curOcarinaSongFlag;
                            if (D_801D8530) {
                                D_801FD518[songIdx] = 1;
                            }
                        }
                    }

                    prevNote = &sOcarinaSongNotes[songIdx][sLearnSongPos[songIdx]];
                    note = &sOcarinaSongNotes[songIdx][++sLearnSongPos[songIdx]];
                    sLearnSongExpectedLength[songIdx] = prevNote->length;
                    sLearnSongExpectedNoteIdx[songIdx] = prevNote->noteIdx;

                    while (prevNote->noteIdx == note->noteIdx || (note->noteIdx == NOTE_NONE && note->length != 0)) {
                        sLearnSongExpectedLength[songIdx] += note->length;
                        prevNote = &sOcarinaSongNotes[songIdx][sLearnSongPos[songIdx]];
                        note = &sOcarinaSongNotes[songIdx][sLearnSongPos[songIdx] + 1];
                        sLearnSongPos[songIdx]++;
                    }
                    
                } else if (D_801FD4A0[songIdx] < 9) {
                    sp5B = -1;
                    D_801FD4A0[songIdx] = 0;
                    sLearnSongPrevNoteIdx = sCurOcarinaNoteIdx;
                } else {
                    if (sOcarinaSongNotes[songIdx][sLearnSongPos[songIdx]].length == 0) {
                        D_801FD518[songIdx] = 1;
                    } else if (D_801D8530) {
                        D_801FD518[songIdx] = 2;
                    }
                    sOcarinaAvailSongs ^= curOcarinaSongFlag;
                }
            }
        }
        if ((sOcarinaAvailSongs == 0) && (sStaffPlayingPos >= D_801D852C)) {
            sIsOcarinaInputEnabled = false;
            if (!D_801D8530) {
                if ((sOcarinaSongFlags & 0x40000000) && (sCurOcarinaNoteIdx == sOcarinaSongNotes[songIdx][0].noteIdx)) {
                    sPrevOcarinaSongFlags = sOcarinaSongFlags;
                }
            }
            sOcarinaSongFlags = 0;
            return;
        }
    }

    if (!sp5F) {
        sLearnSongPrevNoteIdx = sCurOcarinaNoteIdx;
        sStaffPlayingPos += sp5B + 1;
    }
}

// OoT func_800ED200
void func_8019BC44(void) {
    u32 noteIdx;
    u8 temp_v0_2;
    u8 i;
    u8 j;
    u8 k;

    if (CHECK_BTN_ANY(sCurOcarinaInputButtonIdx, BTN_L) &&
        CHECK_BTN_ANY(sCurOcarinaInputButtonIdx, BTN_A | BTN_CRIGHT | BTN_CLEFT | BTN_CDOWN | BTN_CUP)) {
        AudioOcarina_SetupStartOcarina4(sOcarinaSongFlags);
        return;
    }

    func_8019B618();

    if (sOcarinaHasStartedSong) {
        temp_v0_2 = sStaffPlayingPos;
        if ((sPrevOcarinaNoteIdx != sCurOcarinaNoteIdx) && (sCurOcarinaNoteIdx != NOTE_NONE)) {
            sStaffPlayingPos++;
            if (sStaffPlayingPos >= 9) {
                sStaffPlayingPos = 1;
            }

            func_8019B654();

            if ((ABS_ALT(sCurOcarinaBendIdx) > 20) && (temp_v0_2 != sStaffPlayingPos)) {
                sCurOcarinaSong[sOcarinaSongAppendPos - 1] = NOTE_NONE;
            } else {
                sCurOcarinaSong[sOcarinaSongAppendPos - 1] = sCurOcarinaNoteIdx;
            }

            // Tests to see if the notes from the ocarina are identical to any of the songIdx from 0 to sOcarinaSongCount
            for (i = sOcarinaSongNoteStartIdx; i < sOcarinaSongCount; i++) {
                if ((u32)sOcarinaAvailSongs & (1 << i)) {
                    for (j = 0, k = 0; j < gOcarinaSongButtons[i].numButtons && k == 0 &&
                                       sOcarinaSongAppendPos >= gOcarinaSongButtons[i].numButtons;) {

                        noteIdx = sCurOcarinaSong[(sOcarinaSongAppendPos - gOcarinaSongButtons[i].numButtons) + j];

                        if (noteIdx == sButtonToNoteMap[gOcarinaSongButtons[i].buttonIdx[j]]) {
                            j++;
                        } else {
                            k++;
                        }
                    }

                    // This conditional is true if the ocarina song matches songIdx i
                    if (j == gOcarinaSongButtons[i].numButtons) {
                        D_801D8528 = i + 1;
                        sIsOcarinaInputEnabled = false;
                        sOcarinaSongFlags = 0;
                    }
                }
            }
        }
    }
}

// OoT func_800ED458
void AudioOcarina_ProcessControllerInput(u8 arg0) {
    u32 ocarinaBtnsHeld;

    if (sOcarinaSongFlags != 0 && D_801D8538 != 0) {
        D_801D8538--;
        return;
    }

    if ((D_801FD454 == 0) || ((D_801FD454 & (BTN_A | BTN_CRIGHT | BTN_CLEFT | BTN_CDOWN | BTN_CUP)) !=
                              (sCurOcarinaInputButtonIdx & (BTN_A | BTN_CRIGHT | BTN_CLEFT | BTN_CDOWN | BTN_CUP)))) {

        D_801FD454 = 0;

        if (1) {}

        sCurOcarinaNoteIdx = NOTE_NONE;
        sCurOcarinaButtonIdx = OCARINA_BTN_INVALID;

        ocarinaBtnsHeld = (sCurOcarinaInputButtonIdx & (BTN_A | BTN_CRIGHT | BTN_CLEFT | BTN_CDOWN | BTN_CUP)) &
                          (sPrevOcarinaInputButtonIdx & (BTN_A | BTN_CRIGHT | BTN_CLEFT | BTN_CDOWN | BTN_CUP));

        if (!(sOcaInputBtnPress & ocarinaBtnsHeld) && (sCurOcarinaInputButtonIdx != 0)) {
            sOcaInputBtnPress = sCurOcarinaInputButtonIdx;
        } else {
            sOcaInputBtnPress &= ocarinaBtnsHeld;
        }

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

        if (sCurOcarinaInputButtonIdx) {}

        // Pressing the R Button will raise the pitch by 1 semitone
        if ((sCurOcarinaNoteIdx != NOTE_NONE) && (sCurOcarinaInputButtonIdx & BTN_R) && (sRecordingState != 2)) {
            sCurOcarinaButtonIdx += 0x80; // Flag to resolve B Flat 4
            sCurOcarinaNoteIdx++;    // Raise the pitch by 1 semitone
        }

        // Pressing the Z Button will lower the pitch by 1 semitone
        if ((sCurOcarinaNoteIdx != NOTE_NONE) && (sCurOcarinaInputButtonIdx & BTN_Z) && (sRecordingState != 2)) {
            sCurOcarinaButtonIdx += 0x40; // Flag to resolve B Flat 4
            sCurOcarinaNoteIdx--;    // Lower the pitch by 1 semitone
        }

        if (sRecordingState != 2) {
            // Bend the pitch of the note based on y control stick
            sCurOcarinaBendIdx = sOcarinaInputStickRel.y;
            sCurOcarinaBendFreq = AudioOcarina_BendPitchTwoSemitones(sCurOcarinaBendIdx);

            // Add vibrato of the ocarina note based on the x control stick
            sCurOcarinaVibrato = ABS_ALT(sOcarinaInputStickRel.x) >> 2;
            Audio_QueueCmdS8(0x6020D06, sCurOcarinaVibrato);
        } else {
            sCurOcarinaBendIdx = 0;
            sCurOcarinaVibrato = 0;
            sCurOcarinaBendFreq = 1.0f;
        }

        if ((sCurOcarinaNoteIdx != NOTE_NONE) && (sPrevOcarinaNoteIdx != sCurOcarinaNoteIdx)) {
            Audio_QueueCmdS8(0x6020D07, D_801D6FB8 - 1);
            Audio_QueueCmdS8(0x6020D05, sCurOcarinaNoteIdx);
            Audio_PlaySfxGeneral(NA_SE_OC_OCARINA, &gDefaultSfxPos, 4, &sCurOcarinaBendFreq, &sDefaultOcarinaVolume,
                                 &gDefaultSfxReverbAdd);
        } else if ((sPrevOcarinaNoteIdx != NOTE_NONE) && (sCurOcarinaNoteIdx == NOTE_NONE) && !arg0) {
            Audio_StopSfxById(NA_SE_OC_OCARINA);
        }
    }
}

void AudioOcarina_SetInputEnabled(u8 isEnabled) {
    sIsOcarinaInputEnabled = isEnabled;
}

void AudioOcarina_ResetOcarina(void) {
    sCurOcarinaInputButtonIdx = 0;
    sPrevOcarinaInputButtonIdx = 0;
    sOcaInputBtnPress = 0;

    D_801FD454 = 0xFFFF;

    AudioOcarina_ProcessControllerInput(0);
    Audio_StopSfxById(NA_SE_OC_OCARINA);

    if (gAudioSpecId != 0xC) {
        Audio_SetSoundBanksMute(0);
    }

    sPlaybackState = 0;
    sStaffPlaybackPos = 0;
    sIsOcarinaInputEnabled = false;

    sOcarinaSongFlags = 0;

    Audio_ClearBGMMute(0xD);
    func_801A4380(0x7F);
}

void AudioOcarina_SetupReadControllerInput1(void) {
    sCurOcarinaInputButtonIdx = 0;
    sPrevOcarinaInputButtonIdx = 0;
    sOcaInputBtnPress = 0;
    D_801FD454 = 0xFFFF;
    AudioOcarina_ProcessControllerInput(1);
    sPlaybackState = 0;
    sStaffPlaybackPos = 0;
    sIsOcarinaInputEnabled = false;
    sOcarinaSongFlags = 0;
    sCurOcarinaInputButtonIdx = 0;
    AudioOcarina_ReadControllerInput();
    D_801FD454 = sCurOcarinaInputButtonIdx;
}

void func_8019C2E4(u8 arg0, u8 arg1) {
    D_801FD460 = arg1;
    D_801FD461 = arg0;
}

// Gakufu
// s32?
u32 AudioOcarina_SetupReadControllerInput2(u8 arg0) {
    if (D_801D6FB8 != arg0 || arg0 == 1) {
        Audio_QueueSeqCmd((u32)arg0 | 0x82010D00);
        D_801D6FB8 = arg0;

        if (arg0 == 0) {
            AudioOcarina_ResetOcarina();
        } else {
            sCurOcarinaInputButtonIdx = 0;
            AudioOcarina_ReadControllerInput();
            D_801FD454 = sCurOcarinaInputButtonIdx;
            func_801A4380(0x40);
            Audio_QueueSeqCmdMute(0xD);
        }
    }
}

// OoT func_800ED93C
void func_8019C398(s8 songIdxPlusOne, u8 arg1) {
    u8 i = 0;
    s32 phi_a3;

    if (songIdxPlusOne == 0) {
        sPlaybackState = 0;
        Audio_StopSfxById(NA_SE_OC_OCARINA);
        return;
    }

    if (songIdxPlusOne < (OCARINA_SONG_MAX + 1)) {
        sPlaybackSong = sOcarinaSongNotes[songIdxPlusOne - 1];
    } else {
        sPlaybackSong = sPierresSongNotes;
    }

    sPlaybackNotePos = 0;
    if (arg1 & 0x80) {
        sPlaybackState = 1;
        D_801D6FE4 = arg1 & 0xF;
        D_801D6FE8 = 0xFF;
        phi_a3 = D_801D6FE8;
    } else if (arg1 & 0x40) {
        sPlaybackState = 1;
        D_801D6FE8 = arg1 & 0xF;
        D_801D6FE4 = 0xFF;
        phi_a3 = D_801D6FE8;
        for (; i < phi_a3;) {
            if (sOcarinaSongNotes[songIdxPlusOne - 1][sPlaybackNotePos].noteIdx != NOTE_NONE) {
                i++;
            }
            sPlaybackNotePos++;
        }
    } else {
        sPlaybackState = arg1;
        D_801D6FE4 = 0xFF;
        D_801D6FE8 = 0xFF;
        phi_a3 = D_801D6FE8;
    }

    sNotePlaybackTimer = 0;
    sDisplayedNoteValue = 0xFF;
    sStaffPlaybackPos = 0;

    if ((phi_a3 ^ 0) == 0xFF) {
        while (sPlaybackSong[sPlaybackNotePos].noteIdx == NOTE_NONE && songIdxPlusOne > (OCARINA_SONG_EVAN_PART2 + 1)) {
            sPlaybackNotePos++;
        }
    }
}

void AudioOcarina_PlaybackSong(void) {
    s32 noteTimerStep;
    u32 nextNoteTimerStep;

    if (sPlaybackState != 0) {
        if (sStaffPlaybackPos == 0) {
            noteTimerStep = 3;
        } else {
            noteTimerStep = sOcarinaUpdateTaskCurrent - sOcarinaPlaybackTaskStart;
        }

        if (noteTimerStep < sNotePlaybackTimer) {
            sNotePlaybackTimer -= noteTimerStep;
        } else {
            nextNoteTimerStep = noteTimerStep - sNotePlaybackTimer;
            sNotePlaybackTimer = 0;
        }

        if (sNotePlaybackTimer == 0) {

            if (sStaffPlaybackPos == D_801D6FE4) {
                sPlaybackState = 0;
                return;
            }

            sNotePlaybackTimer = sPlaybackSong[sPlaybackNotePos].length;

            if (sPlaybackNotePos == 1) {
                sNotePlaybackTimer++;
            }

            if (sNotePlaybackTimer == 0) {
                sPlaybackState--;
                if (sPlaybackState != 0) {
                    sPlaybackNotePos = 0;
                    sStaffPlaybackPos = 0;
                    sDisplayedNoteValue = NOTE_NONE;
                } else {
                    Audio_StopSfxById(NA_SE_OC_OCARINA);
                }
                return;
            } else {
                sNotePlaybackTimer -= nextNoteTimerStep;
            }

            if (sNotePlaybackVolume != sPlaybackSong[sPlaybackNotePos].volume) {
                sNotePlaybackVolume = sPlaybackSong[sPlaybackNotePos].volume;
                sNormalizedNotePlaybackVolume = sNotePlaybackVolume / 127.0f;
            }

            sNotePlaybackVibrato = sPlaybackSong[sPlaybackNotePos].vibrato;
            Audio_QueueCmdS8(0x06020D06, sNotePlaybackVibrato);

            if (sNotePlaybackBend != sPlaybackSong[sPlaybackNotePos].bend) {
                sNotePlaybackBend = sPlaybackSong[sPlaybackNotePos].bend;
                sNormalizedNotePlaybackTone = AudioOcarina_BendPitchTwoSemitones(sNotePlaybackBend);
            }

            if ((sPlaybackSong[sPlaybackNotePos].volume == sPlaybackSong[sPlaybackNotePos - 1].volume &&
                 (sPlaybackSong[sPlaybackNotePos].vibrato == sPlaybackSong[sPlaybackNotePos - 1].vibrato) &&
                 (sPlaybackSong[sPlaybackNotePos].bend == sPlaybackSong[sPlaybackNotePos - 1].bend))) {
                sDisplayedNoteValue = 0xFE;
            }

            if (sDisplayedNoteValue != sPlaybackSong[sPlaybackNotePos].noteIdx) {
                u8 noteIdx = sPlaybackSong[sPlaybackNotePos].noteIdx;

                if (noteIdx == NOTE_BFLAT4) {
                    sDisplayedNoteValue = noteIdx + sPlaybackSong[sPlaybackNotePos].BFlat4Flag;
                } else {
                    sDisplayedNoteValue = noteIdx;
                }

                if (sDisplayedNoteValue != NOTE_NONE) {
                    sStaffPlaybackPos++;
                    Audio_QueueCmdS8(0x6020D07, D_801D6FB8 - 1);
                    Audio_QueueCmdS8(0x6020D05, sDisplayedNoteValue & 0x3F);
                    Audio_PlaySfxGeneral(NA_SE_OC_OCARINA, &gDefaultSfxPos, 4, &sNormalizedNotePlaybackTone,
                                         &sNormalizedNotePlaybackVolume, &gDefaultSfxReverbAdd);
                } else {
                    Audio_StopSfxById(NA_SE_OC_OCARINA);
                }
            }
            sPlaybackNotePos++;
        }
    }
}

// OoT func_800EDD68
void func_8019C8D8(u8 arg0) {
    u16 i;
    u16 i2;
    u16 pad;
    u8 noteIdx;
    OcarinaNote* note;
    u8 j;
    u8 k;
    s32 t;
    OcarinaNote* song;

    if (sRecordingState == 1) {
        song = gScarecrowCustomSongPtr;
    } else {
        song = sTerminaWallSongPtr;
    }

    song[sRecordSongPos].noteIdx = sRecordOcarinaNoteIdx;
    song[sRecordSongPos].length = sOcarinaUpdateTaskCurrent - sOcarinaRecordTaskStart;
    song[sRecordSongPos].volume = sRecordOcarinaVolume;
    song[sRecordSongPos].vibrato = sRecordOcarinaVibrato;
    song[sRecordSongPos].bend = sRecordOcarinaBendIdx;
    song[sRecordSongPos].BFlat4Flag = sRecordOcarinaButtonIdx & 0xC0;

    sRecordOcarinaNoteIdx = sCurOcarinaNoteIdx;
    sRecordOcarinaVolume = sCurOcarinaVolume;
    sRecordOcarinaVibrato = sCurOcarinaVibrato;
    sRecordOcarinaBendIdx = sCurOcarinaBendIdx;
    sRecordOcarinaButtonIdx = sCurOcarinaButtonIdx;

    sRecordSongPos++;

    if ((sRecordSongPos != ARRAY_COUNT(sPierresSongNotes) - 1) && (arg0 == 0)) {
        return;
    }

    i = sRecordSongPos;
    noteIdx = NOTE_NONE;
    while (i != 0 && noteIdx == NOTE_NONE) {
        i--;
        noteIdx = song[i].noteIdx;
    }

    if (sRecordSongPos != (i + 1)) {
        sRecordSongPos = i + 2;
        song[sRecordSongPos - 1].length = 0;
    }

    song[sRecordSongPos].length = 0;

    if (sRecordingState == 2) {
        if (sStaffPlayingPos >= 8) {
            for (i = 0; i < sRecordSongPos; i++) {
                song[i] = song[i + 1];
            }

            AudioOcarina_MapSongFromNotesToButtons(OCARINA_SONG_TERMINA_WALL, OCARINA_SONG_SCARECROW, 8);

            // Tests to see if the notes from the scarecrow song contain identical 
            // notes within its song to any of the reserved songIdx from 0 to 21
            for (i = 0; i < OCARINA_SONG_SCARECROW; i++) {
                if (sIsOcarinaSongReserved[i]) {
                    for (j = 0; (j < 9 - gOcarinaSongButtons[i].numButtons); j++) {
                        for (k = 0; k < gOcarinaSongButtons[i].numButtons && k + j < 8 &&
                                    gOcarinaSongButtons[i].buttonIdx[k] == gOcarinaSongButtons[OCARINA_SONG_SCARECROW].buttonIdx[k + j];
                             k++) {
                            ;
                        }

                        // This conditional is true if the recorded song matches songIdx i
                        if (k == gOcarinaSongButtons[i].numButtons) {
                            sRecordingState = 0xFF;
                            sOcarinaSongNotes[OCARINA_SONG_SCARECROW][1].volume = 0xFF;
                            return;
                        }
                    }
                }
            }

            i = 1;
            while (i < 8) {
                if (gOcarinaSongButtons[OCARINA_SONG_SCARECROW].buttonIdx[0] != gOcarinaSongButtons[OCARINA_SONG_SCARECROW].buttonIdx[i]) {
                    i = 9;
                } else {
                    i++;
                }
            }

            if (i == 8) {
                sRecordingState = 0xFF;
                sOcarinaSongNotes[OCARINA_SONG_SCARECROW][1].volume = 0xFF;
                return;
            }

            for (i = 0; i < sRecordSongPos; i++) {
                sOcarinaSongNotes[OCARINA_SONG_SCARECROW][i] = sOcarinaSongNotes[OCARINA_SONG_TERMINA_WALL][i];
            }

            sIsOcarinaInputEnabled = 0;
        } else {
            sOcarinaSongNotes[OCARINA_SONG_SCARECROW][1].volume = 0xFF;
        }
    }
    sRecordingState = 0;
}

// OoT func_800EE170
void func_8019CD08(u8 recordingState) {
    if (sRecordingState != recordingState) {
        if (recordingState) {
            sOcarinaRecordTaskStart = sOcarinaUpdateTaskCurrent;
            sRecordOcarinaNoteIdx = NOTE_NONE;
            sRecordOcarinaVolume = 0x57;
            sRecordOcarinaVibrato = 0;
            sRecordOcarinaBendIdx = 0;
            sRecordOcarinaButtonIdx = 0;
            sRecordSongPos = 0;
            sIsOcarinaInputEnabled = true;
            sStaffPlayingPos = 0;
            sRecordingSongNote = sPierresSongNotes[1];
        } else {
            if (sRecordSongPos == 0) {
                sPierresSongNotes[1] = sRecordingSongNote;
            } else {
                if (sRecordingState == 2) {
                    sStaffPlayingPos = 1;
                }
                func_8019C8D8(1);
            }
            sIsOcarinaInputEnabled = false;
            sStaffPlayingPos = 0;
        }
        sRecordingState = recordingState;
    }
}

// OoT func_800EE29C
void AudioOcarina_UpdateRecordingStaff(void) {
    sRecordingStaff.state = sRecordingState;
    sRecordingStaff.pos = sStaffPlayingPos;
    if (sRecordingState == 0xFF) {
        sRecordingState = 0;
    }
}

// OoT func_800EE2D4
void AudioOcarina_UpdatePlayingStaff(void) {
    if (sCurOcarinaButtonIdx != OCARINA_BTN_INVALID) {
        sPlayingStaff.buttonIdx = sCurOcarinaButtonIdx & 0x3F;
    }
    sPlayingStaff.state = func_8019AFE8();
    sPlayingStaff.pos = sStaffPlayingPos;
}

// OoT func_800EE318
void AudioOcarina_UpdateDisplayedStaff(void) {
    if ((sDisplayedNoteValue & 0x3F) < 0x10) {
        sDisplayedStaff.buttonIdx = AudioOcarina_MapNoteToButton(sDisplayedNoteValue);
    }

    sDisplayedStaff.state = sPlaybackState;

    if (sPlaybackSong != sPierresSongNotes) {
        sDisplayedStaff.pos = sStaffPlaybackPos;
    } else if (sStaffPlaybackPos == 0) {
        sDisplayedStaff.pos = 0;
    } else {
        sDisplayedStaff.pos = ((sStaffPlaybackPos - 1) % 8) + 1;
    }
}

// OoT func_800EE3C8
OcarinaStaff* AudioOcarina_GetRecordingStaff(void) {
    return &sRecordingStaff;
}

OcarinaStaff* AudioOcarina_GetPlayingStaff(void) {
    if (sPlayingStaff.state < 0xFE) {
        sOcarinaSongFlags = 0;
    }
    return &sPlayingStaff;
}

OcarinaStaff* AudioOcarina_GetDisplayedStaff(void) {
    return &sDisplayedStaff;
}

void func_8019CFA8(void) {
    s32 phi_v0;

    if ((sRecordingState != 0) && ((sOcarinaUpdateTaskCurrent - sOcarinaRecordTaskStart) >= 3)) {
        phi_v0 = false;
        if (sRecordOcarinaNoteIdx != sCurOcarinaNoteIdx) {
            if (sCurOcarinaNoteIdx != NOTE_NONE) {
                sRecordingStaff.buttonIdx = sCurOcarinaButtonIdx & 0x3F;
                sStaffPlayingPos += 1;
            } else if ((sRecordingState == 2) && (sStaffPlayingPos == 8)) {
                func_8019C8D8(1);
                return;
            }

            if (sStaffPlayingPos >= 9) {
                if (sRecordingState == 2) {
                    func_8019C8D8(1);
                    return;
                }
                sStaffPlayingPos = 1;
            }
            phi_v0 = true;
        } else if (sRecordOcarinaVolume != sCurOcarinaVolume) {
            phi_v0 = true;
        } else if (sRecordOcarinaVibrato != sCurOcarinaVibrato) {
            if (sRecordingState != 2) {
                phi_v0 = true;
            }
        } else if ((sRecordOcarinaBendIdx != sCurOcarinaBendIdx) && (sRecordingState != 2)) {
            phi_v0 = true;
        }

        if (phi_v0) {
            func_8019C8D8(0);
            sOcarinaRecordTaskStart = sOcarinaUpdateTaskCurrent;
        }
    }
}

/**
 * Tests to see if the notes from songIdx contain identical notes 
 * within its song to any of the reserved songIdx from 0 up to maxSongIdx
 */
s32 AudioOcarina_MusicWallValidateNotes(u8 songIdx, u8 maxSongIdx) {
    u8 curSongIdx;
    u8 j;
    u8 k;

    for (curSongIdx = 0; curSongIdx < maxSongIdx; curSongIdx++) {
        if (sIsOcarinaSongReserved[curSongIdx]) {
            for (j = 0; j < (9 - gOcarinaSongButtons[curSongIdx].numButtons); j++) {
                for (k = 0; (k < gOcarinaSongButtons[curSongIdx].numButtons) && ((k + j) < 8) &&
                            (gOcarinaSongButtons[curSongIdx].buttonIdx[k] == gOcarinaSongButtons[songIdx].buttonIdx[(k + j)]);
                     k++) {
                    continue;
                }

                if (k == gOcarinaSongButtons[curSongIdx].numButtons) {
                    // failure: songIdx is identical to curSongIdx
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
 */
void AudioOcarina_MusicWallGenerateNotes(void) {
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
            AudioOcarina_MapSongFromNotesToButtons(OCARINA_SONG_TERMINA_WALL, OCARINA_SONG_TERMINA_WALL, sOoTOcarinaSongsNumNotes[j]);
        }
    } while (AudioOcarina_MusicWallValidateNotes(OCARINA_SONG_TERMINA_WALL, OCARINA_SONG_TERMINA_WALL) != 0);
}

// func_800EE57C
void AudioOcarina_MemoryGameSetNumNotes(u8 memoryGameIdx) {
    u8 i;

    if (memoryGameIdx > 2) {
        memoryGameIdx = 2;
    }

    sOcaMemoryGameAppendPos = 0;
    sOcaMemoryGameEndPos = sOcaMemoryGameNumNotes[memoryGameIdx];

    for (i = 0; i < 3; i++) {
        AudioOcarina_MemoryGameGenerateNotes();
    }
}

// OoT func_800EE5EC
// Remnant of OoT, Id 14 now represent Goron Lullaby Intro instead of the OoT ocarina memory game
#define OCARINA_SONG_MEMORYGAME OCARINA_SONG_GORON_LULLABY_INTRO
s32 AudioOcarina_MemoryGameGenerateNotes(void) {
    u32 rnd;
    u8 rndNoteIdx;

    if (sOcaMemoryGameAppendPos == sOcaMemoryGameEndPos) {
        return true;
    }

    rnd = Audio_NextRandom();
    rndNoteIdx = sButtonToNoteMap[rnd % 5];

    if (sOcarinaSongNotes[OCARINA_SONG_GORON_LULLABY_INTRO][sOcaMemoryGameAppendPos - 1].noteIdx == rndNoteIdx) {
        rndNoteIdx = sButtonToNoteMap[(rnd + 1) % 5];
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

// OoT func_800EE6F4
// input update?
void AudioOcarina_Update(void) {
    sOcarinaUpdateTaskCurrent = gAudioContext.totalTaskCnt;
    if (D_801D6FB8 != 0) {
        if (sIsOcarinaInputEnabled == true) {
            AudioOcarina_ReadControllerInput();
        }

        if ((sPlaybackState == 0) && (sIsOcarinaInputEnabled == true)) {
            AudioOcarina_ProcessControllerInput(0);
        }

        if (sOcarinaSongFlags != 0) {
            if (sOcarinaSongFlags & 0x40000000) {
                func_8019BC44();
            } else {
                func_8019B6B4();
            }
        }

        AudioOcarina_PlaybackSong();
        sOcarinaPlaybackTaskStart = sOcarinaUpdateTaskCurrent;

        if (sPlaybackState == 0) {
            func_8019CFA8();
        }

        if ((sOcarinaSongFlags != 0) && (sPrevOcarinaNoteIdx != sCurOcarinaNoteIdx)) {
            D_801D8538 = 1;
        }

        sPrevOcarinaNoteIdx = sCurOcarinaNoteIdx;

        if (D_801FD460 != 0) {
            D_801FD460--;
            if (D_801FD460 == 0) {
                AudioOcarina_SetupReadControllerInput2(0);
            }
        }
    }

    AudioOcarina_UpdatePlayingStaff();
    AudioOcarina_UpdateDisplayedStaff();
    AudioOcarina_UpdateRecordingStaff();
}

void func_8019D758(void) {
    static u8 D_801D8B20 = 0;
    static u8 D_801D8B24 = 1;
    static u16 D_801D8B28 = 1200;

    switch (D_801D8B20) {
        case 0:
            if (D_801D8B28-- == 0) {
                if (D_801D8B24 < 7) {
                    D_801D8B20++;
                } else {
                    D_801D8B20 = 3;
                    AudioOcarina_SetupReadControllerInput2(0);
                }
                D_801D8B28 = 1200;
            }
            break;
        case 1:
            Audio_SetSoundBanksMute(0);
            AudioOcarina_SetupReadControllerInput2(D_801D8B24);
            func_8019C398(0x19, 1);
            D_801D8B24++;
            D_801D8B20++;
            break;
        case 2:
            if (AudioOcarina_GetDisplayedStaff()->state == 0) {
                D_801D8B20 = 0;
            }
            break;
    }
}

u8 D_801D8B2C = false;

// New to MM
void func_8019D864(void) {
    if (D_801D8B2C && gAudioContext.seqPlayers[1].enabled &&
        ((u8)gAudioContext.seqPlayers[1].soundScriptIO[0] == 0xFF)) {
        gAudioContext.seqPlayers[1].seqData = D_801D88B8;
        D_801D8B2C = false;
    }
}

// New to MM
void func_8019D8B4(void) {
    D_801D8B2C = true;
    Audio_QueueSeqCmd(0x1000000 | NA_BGM_DUNGEON_APPEAR);
}

// New to MM
// Very non-equivalent
#ifdef NON_EQUIVALENT
s32 func_8019D8E4(void) {
    s32 sp34;
    u16 temp_a0;
    s32 temp_a2;
    u16 temp_lo;
    s32 temp_t9_4;
    u16 phi_a2;
    u16 phi_v0;
    s32 phi_t5;
    u16 phi_v0_2;
    s32 phi_v0_3;
    s8 phi_a0;
    u16 phi_s4;
    u16 i;

    sp34 = sPierresSongNotes[0].noteIdx;
    D_801FD598 = 0x27;
    phi_a2 = 0;
    phi_t5 = 0;
    phi_s4 = 0;
    // i = 1;

    for (i = 0; (sPierresSongNotes[i].noteIdx == 0xFF) && (sPierresSongNotes[i].length != 0); i++) {
        i++;
    }

    if ((sPierresSongNotes[i].length != 0) && (D_801FD598 < 0x18A)) {
    loop_7:
        temp_a0 = ((sPierresSongNotes[i].length * 0x30) / 30);
        if (sPierresSongNotes[i].vibrato != sPierresSongNotes[i - 1].vibrato) {
            D_801D88B8[D_801FD598] = 0xFD;
            D_801FD598++;
            if (phi_a2 >= 0x80) {
                D_801D88B8[D_801FD598] = ((phi_a2 >> 8) & 0x7F) + 0x80;
                D_801FD598++;
                D_801D88B8[D_801FD598] = phi_a2;
                D_801FD598++;
            } else {
                D_801D88B8[D_801FD598] = phi_a2;
                D_801FD598++;
            }
            D_801D88B8[D_801FD598] = 0xD8;
            D_801FD598++;
            D_801D88B8[D_801FD598] = sPierresSongNotes[i].vibrato;
            D_801FD598++;
            phi_a2 = temp_a0;
        } else {
            phi_a2 += temp_a0;
        }
        i++;
        if ((sPierresSongNotes[i].length != 0) && (D_801FD598 < 0x18A)) {
            goto loop_7;
        }
    }

    if (phi_a2 != 0) {
        D_801D88B8[D_801FD598] = 0xFD;
        D_801FD598++;
        if (phi_a2 >= 0x80) {
            D_801D88B8[D_801FD598] = ((phi_a2 >> 8) & 0x7F) + 0x80;
            D_801FD598++;
            D_801D88B8[D_801FD598] = phi_a2;
            D_801FD598++;
        } else {
            D_801D88B8[D_801FD598] = phi_a2;
            D_801FD598++;
        }
    }

    D_801D88B8[D_801FD598] = 0xFF;
    D_801FD598++;
    temp_t9_4 = D_801FD598 + 4;
    D_801D88B8[0x18] = D_801FD598 >> 8;
    D_801D88B8[0x19] = D_801FD598;
    D_801D88B8[0x1B] = temp_t9_4 >> 8;
    D_801D88B8[0x1C] = temp_t9_4;
    D_801D88B8[D_801FD598] = 0xC2;
    D_801FD598++;
    D_801D88B8[D_801FD598] = 0xFB;
    D_801FD598++;
    D_801D88B8[D_801FD598] = 0xC0;
    D_801FD598++;
    D_801D88B8[D_801FD598] = 8;
    D_801FD598++;
    D_801D88B8[D_801FD598] = 0xC1;
    D_801FD598++;
    D_801D88B8[D_801FD598] = 0x57;
    D_801FD598++;
    D_801D88B8[D_801FD598] = 0xC9;
    D_801FD598++;
    D_801D88B8[D_801FD598] = 0;

    for (i = 0; (sPierresSongNotes[i].noteIdx == 0xFF) && (sPierresSongNotes[i].length != 0); i++) {
        i++;
    }

    if ((sPierresSongNotes[i].length != 0) && (D_801FD598 < 0x18A)) {
    loop_25:
        if (sPierresSongNotes[i].noteIdx == sPierresSongNotes[i - 1].noteIdx) {
            if ((sPierresSongNotes[i].length != 0) && (phi_t5 == 0)) {
                D_801D88B8[D_801FD598] = 0xC4;
                D_801FD598++;
                phi_t5 = 1;
            }
        } else if ((phi_t5 == 1) && (sPierresSongNotes[i - 1].noteIdx != 0xFF) && (sPierresSongNotes[i - 1].noteIdx != 0)) {
            D_801D88B8[D_801FD598] = 0xC5;
            D_801FD598++;
        }

        if (sPierresSongNotes[i].bend != sPierresSongNotes[i - 1].bend) {
            D_801D88B8[D_801FD598] = 0xCE;
            D_801FD598++;
            // phi_v0_3 = ABS_ALT(sPierresSongNotes[i].bend);

            // if (sPierresSongNotes[i].bend < 0) {
            //     phi_v0_3 = -sPierresSongNotes[i].bend;
            // }

            if (ABS_ALT(sPierresSongNotes[i].bend) > 0x40) {
                phi_a0 = 0x7F;
            } else {
                if (sPierresSongNotes[i].bend < 0) {
                    phi_a0 = sPierresSongNotes[i].bend * -0x1000000;
                } else {
                    phi_a0 = ((sPierresSongNotes[i].bend * 0x7F) / 64);
                }
            }

            if (sPierresSongNotes[i].bend < 0) {
                phi_a0 *= -1;
            }

            D_801D88B8[D_801FD598] = phi_a0;
            D_801FD598++;
        }

        if (sPierresSongNotes[i].noteIdx != 0xFF) {
            D_801D88B8[D_801FD598] = sPierresSongNotes[i].noteIdx + 0x27;
            D_801FD598++;
        } else {
            D_801D88B8[D_801FD598] = 0xC0;
            D_801FD598++;
        }

        temp_a2 = (sPierresSongNotes[i].length * 0x30) + phi_s4;
        temp_lo = temp_a2 / 30;
        if (temp_lo < 0x80) {
            D_801D88B8[D_801FD598] = temp_lo;
            D_801FD598++;
        } else {
            D_801D88B8[D_801FD598] = ((temp_lo >> 8) & 0x7F) + 0x80;
            D_801FD598++;
            D_801D88B8[D_801FD598] = temp_lo;
            D_801FD598++;
        }
        i++;
        phi_s4 = temp_a2 - (temp_lo * 0x1E);
        if ((sPierresSongNotes[i].length != 0) && (D_801FD598 < 0x18A)) {
            goto loop_25;
        }
    }

    D_801D88B8[D_801FD598] = 0xFF;
    D_801FD598++;

    func_8019D8B4();

    if (i >= 0x18A) {
        return -1;
    } else {
        return 0;
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019D8E4.s")
#endif

// New to MM
void func_8019DF28(void) {
    if (D_801D6FB8 != 0) {
        Audio_QueueSeqCmd(0x82010D00);
        D_801D6FB8 = 0;
        AudioOcarina_ResetOcarina();
    }
}

// OoT func_800EE930
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
    D_801FD460 = 0;
    D_801D8538 = 0;

    for (songIdx = OCARINA_SONG_SONATA; songIdx < OCARINA_SONG_MAX; songIdx++) {
        D_801FD518[songIdx] = 0xFF;
        D_801FD530[songIdx] = 0;
    }

    D_801D8534 = 0;
}

// AudioDebug Remnant
void func_8019DFF8(s32 arg0) {
}

// AudioDebug Remnant
void func_8019E000(s32 arg0, s32 arg1) {
}

// AudioDebug Remnant
void func_8019E00C(void) {
}

// OoT func_800F3054
void Audio_Update(void) {
    if ((func_801A9768() == 0) && (func_801A982C() == 0)) {
        func_8019D864();
        AudioOcarina_Update();
        func_801A5118();
        Audio_StepFreqLerp(&sRiverFreqScaleLerp);
        Audio_StepFreqLerp(&sWaterfallFreqScaleLerp);
        func_801A046C();
        func_801A2778();
        func_801A312C();
        func_8019FEDC();
        func_801A1E0C();
        func_801A1904();
        func_801A2090();
        func_801A3AC0();
        func_801A1290();
        func_801A44C4();
        Audio_ProcessSoundRequests();
        Audio_ProcessSeqCmds();
        func_801A787C();
        func_801A8D5C();
        func_8019F300();
        Audio_ScheduleProcessCmds();
    }
}

// OoT func_800F3138
void func_8019E0FC(UNK_TYPE arg0) {
}

// OoT func_800F3140
void func_8019E104(UNK_TYPE arg0, UNK_TYPE arg1) {
}

// OoT func_800F314C
void func_8019E110(s8 arg0) {
    Audio_QueueCmdS32(0x82000000 | (((u8)arg0 & 0xFF) << 8), 1);
}

// OoT func_800F3188
f32 Audio_ComputeSoundVolume(u8 bankId, u8 entryIdx) {
    SoundBankEntry* bankEntry = &gSoundBanks[bankId][entryIdx];
    f32 minDist;
    f32 phi_f14;
    f32 baseDist;
    f32 phi_f12;
    f32 ret;
    s32 pad[3];

    if (bankEntry->sfxParams & 0x2000) {
        return 1.0f;
    }

    if (bankEntry->dist > 10000.0f) {
        ret = 0.0f;
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
            ret = 1.0f;
        } else if (bankEntry->dist < baseDist) {
            ret = ((((baseDist - minDist) - (bankEntry->dist - minDist)) / (baseDist - minDist)) * phi_f14) + phi_f12;
        } else {
            ret = (1.0f - ((bankEntry->dist - baseDist) / (10000.0f - baseDist))) * phi_f12;
        }
        ret = SQ(ret);
    }

    return ret;
}

s8 Audio_ComputeSoundReverb(u8 bankId, u8 entryIdx, u8 channelIdx) {
    s8 distAdd = 0;
    s32 scriptAdd = 0;
    SoundBankEntry* entry = &gSoundBanks[bankId][entryIdx];
    s32 reverb;

    if (!(entry->sfxParams & 0x1000)) {
        if (entry->dist < 2500.0f) {
            distAdd = *entry->posZ > 0.0f ? (entry->dist / 2500.0f) * 70.0f : (entry->dist / 2500.0f) * 91.0f;
        } else {
            distAdd = 70;
        }
    }

    if (IS_SEQUENCE_CHANNEL_VALID(gAudioContext.seqPlayers[2].channels[channelIdx])) {
        scriptAdd = gAudioContext.seqPlayers[2].channels[channelIdx]->soundScriptIO[1];
        if (gAudioContext.seqPlayers[2].channels[channelIdx]->soundScriptIO[1] < 0) {
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

s8 Audio_ComputeSoundPanSigned(f32 x, f32 z, u8 token) {
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

f32 Audio_ComputeSoundFreqScale(u8 bankId, u8 entryIdx) {
    s32 phi_v0 = 0;
    SoundBankEntry* entry = &gSoundBanks[bankId][entryIdx];
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
u8 func_8019E864(f32 behindScreenZ, SoundBankEntry* entry, s8 arg2) {
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
s8 func_8019EA40(f32 arg0, u16 sfxParams) {
    s8 ret;

    if (arg0 < 0.0f) {
        if (arg0 < -625.0f) {
            ret = -32;
        } else {
            ret = (s8)(((625.0f + arg0) / 625.0f) * 31.0f) + 0xE0;
        }
    } else if (arg0 > 1250.0f) {
        ret = 127;
    } else {
        ret = (arg0 / 1250.0f) * 126.0f;
    }

    return ret | 1;
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
    s8 sp38 = 0;
    s8 sp37 = -1;
    f32 behindScreenZ;
    u8 baseFilter = 0;
    SoundBankEntry* entry = &gSoundBanks[bankId][entryIdx];
    u8 pad2;

    switch (bankId) {
        case BANK_PLAYER:
        case BANK_ITEM:
        case BANK_ENV:
        case BANK_ENEMY:
        case BANK_VOICE:
            if (D_801D66A8 == 2) {
                sp38 = func_8019EA40(*entry->posY, entry->sfxParams);
            }
            // fallthrough
        case BANK_OCARINA:
            entry->dist = sqrtf(entry->dist * 10.0f);
            vol = Audio_ComputeSoundVolume(bankId, entryIdx) * *entry->vol;
            reverb = Audio_ComputeSoundReverb(bankId, entryIdx, channelIdx);
            panSigned = Audio_ComputeSoundPanSigned(*entry->posX, *entry->posZ, entry->token);
            freqScale = Audio_ComputeSoundFreqScale(bankId, entryIdx) * *entry->freqScale;

            if (D_801D66A8 == 2) {
                if (*entry->posZ >= 0.0f) {
                    if (*entry->posZ < 200.0f) {
                        sp37 = 32.0f - ((*entry->posZ / 300.0f) * 32.0f);
                    } else {
                        sp37 = 0;
                    }
                } else if (*entry->posZ < 0.0f) {
                    if (*entry->posZ > -300.0f) {
                        sp37 = 32.0f - ((*entry->posZ / 300.0f) * 95.0f);
                    } else {
                        sp37 = 127;
                    }
                }
                sp37 = D_801D8B30[sp37];
            }

            if (sAudioBaseFilter != 0) {
                if ((bankId == BANK_PLAYER) || (bankId == BANK_ITEM) ||
                    ((bankId == BANK_VOICE) && (((entry->sfxId & 0xFF) < 0x40) || ((entry->sfxId & 0xFF) >= 0x80)))) {
                    baseFilter = sAudioBaseFilter;
                    // pad = sAudioExtraFilter;
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
            } else if ((D_801D66A8 == 2) && (entry->sfxParams & 0x2000) == 0) {
                filter = func_8019E864(behindScreenZ, entry, panSigned);
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
    Audio_QueueCmdS8(0x6020000 | (channelIdx << 8) | 2, volS8);
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
    if (sSfxChannelState[channelIdx].unk_0C != sp38) {
        sSfxChannelState[channelIdx].unk_0C = sp38;
    }
    if (sSfxChannelState[channelIdx].unk_0D != sp37) {
        // CHAN_UPD_UNK_0F
        // CHAN_UPD_UNK_20
        sSfxChannelState[channelIdx].unk_0D = sp37;
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

    sSfxChannelState[13].unk_0C = 0;
    D_801D66C0 = 0;
    sAudioCodeReverb = 0;
}

void Audio_PlaySfx1(u16 sfxId) {
    Audio_PlaySfxGeneral(sfxId, &gDefaultSfxPos, 4, &gDefaultSfxVolOrFreq, &gDefaultSfxVolOrFreq, &gDefaultSfxReverbAdd);
    if (sfxId == NA_SE_OC_TELOP_IMPACT) {
        func_801A32CC(0);
    }
}

void Audio_PlaySfx2(u16 sfxId) {
    Audio_PlaySfxGeneral(sfxId, &gDefaultSfxPos, 4, &gDefaultSfxVolOrFreq, &gDefaultSfxVolOrFreq, &gDefaultSfxReverbAdd);
}

/**
 * Bends the pitch of the sfx by a little under two semitones and adds reverb
 */
void Audio_PlaySfxAtPosWithLowFreqAndReverb(Vec3f* pos, u16 sfxId) {
    Audio_PlaySfxGeneral(sfxId, pos, 4, &gLoweredSfxFreq, &gDefaultSfxVolOrFreq, &sIncreasedSfxReverb);
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

SfxSettings* func_8019F258(Vec3f* pos) {
    SfxSettings* sfxSettings;
    u8 i = 0;
    u8 phi_v1 = 0xFF;

    for (; i < ARRAY_COUNT(sSfxSettings); i++) {
        sfxSettings = &sSfxSettings[i];

        if ((sfxSettings->pos == NULL) && (phi_v1 == 0xFF)) {
            phi_v1 = i;
        }

        if (sfxSettings->pos == pos) {
            return sfxSettings;
        }
    }

    if (phi_v1 != 0xFF) {
        D_801FD250 |= 1 << phi_v1;
        sfxSettings = &sSfxSettings[phi_v1];
        sfxSettings->pos = pos;
        return sfxSettings;
    }

    return NULL;
}

void func_8019F300(void) {
    SoundBankEntry* entry;
    s32 temp_a2;
    u8 temp_v1;
    u8 phi_a1;
    u8 phi_v0 = 0;
    u8 bankId;
    u8 entryIndex;
    s32 phi_a0;

    if (D_801FD250 != 0) {
        phi_a1 = D_801FD250;

        while (phi_a1 != 0) {

            bankId = 2;
            if ((phi_a1 & (1 << phi_v0))) {

                phi_a0 = false;
                while ((bankId < 4) && !phi_a0) {
                    entryIndex = gSoundBanks[bankId]->next;

                    while (entryIndex != 0xFF) {
                        entry = &gSoundBanks[bankId][entryIndex];
                        entryIndex = 0xFF;
                        if (entry->posX == &sSfxSettings[phi_v0].pos->x) {
                            phi_a0 = true;
                        } else {
                            entryIndex = entry->next;
                        }
                    }
                    bankId++;
                }

                if (!phi_a0) {
                    D_801FD250 ^= (1 << phi_v0);
                    sSfxSettings[phi_v0].pos = NULL;
                }

                phi_a1 ^= (1 << phi_v0);
            }

            phi_v0++;
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
        Audio_PlaySfxGeneral(sfxId, pos, 4, &gDefaultSfxVolOrFreq, &gDefaultSfxVolOrFreq, &D_801D66F0);
    }
}

/**
 * Used only for eating the goron sirloin by the goron with Don Gero's Mask
 */
void func_8019F4AC(Vec3f* pos, u16 sfxId) {
    SfxSettings* sfxSettings;

    if ((sfxId == NA_SE_EN_KONB_JUMP_OLD) || (sfxId == NA_SE_EN_KONB_SINK_OLD)) {
        Audio_PlaySfxGeneral(sfxId, pos, 4, &gDefaultSfxVolOrFreq, &gDefaultSfxVolOrFreq, &gDefaultSfxReverbAdd);
    } else {
        sfxSettings = func_8019F258(pos);

        if (sfxSettings != NULL) {
            Audio_PlaySfxGeneral(sfxId, pos, 4, &gDefaultSfxVolOrFreq, &gDefaultSfxVolOrFreq, &sfxSettings->reverbAdd);
        }
    }

}

void func_8019F540(s8 arg0) {
    if (arg0 != 0) {
        D_801D66F0 = -0x80;
    } else {
        D_801D66F0 = 0;
    }
}

void func_8019F570(Vec3f* pos, s8 arg1) {
    SfxSettings* sfxSettings = func_8019F258(pos);

    if (sfxSettings != NULL) {
        if (arg1 != 0) {
            sfxSettings->reverbAdd = -0x80;
        } else {
            sfxSettings->reverbAdd = 0;
        }
    }
}

// OoT func_800F3F84
f32 func_8019F5AC(f32 arg0) {
    f32 ret = 1.0f;

    if (arg0 > 6.0f) {
        D_801FD25C = 1.0f;
        D_801FD264 = 1.1f;
    } else {
        ret = arg0 / 6.0f;
        D_801FD25C = (ret * 0.22500002f) + 0.775f;
        D_801FD264 = (ret * 0.2f) + 0.9f;
    }

    return ret;
}

// OoT func_800F4010
void func_8019F638(Vec3f* pos, u16 sfxId, f32 arg2) {
    f32 sp2C;
    f32 phi_f0;
    s32 phi_v0;
    u16 sfxId2 = 0;

    sp2C = func_8019F5AC(arg2);
    Audio_PlaySfxGeneral(sfxId, pos, 4, &D_801FD264, &D_801FD25C, &gDefaultSfxReverbAdd);
    if ((sfxId & 0xF0) == 0xB0) {
        phi_f0 = 0.3f;
        phi_v0 = 1;
        sp2C = 1.0f;
    } else {
        phi_f0 = 1.1f;
        phi_v0 = gAudioContext.audioRandom & 1 & 0xFF;
    }
    if (phi_f0 < arg2) {
        if (phi_v0 != 0) {
            if ((sfxId & 0x1FF) < 0x80) {
                sfxId2 = NA_SE_PL_METALEFFECT_KID;
            } else if ((sfxId & 0x1FF) < 0xF0) {
                sfxId2 = NA_SE_PL_METALEFFECT_ADULT;
            }
            if (sfxId2 != 0) {
                D_801FD260 = (sp2C * 0.7) + 0.3;
                Audio_PlaySfxGeneral(sfxId2, pos, 4, &D_801FD264, &D_801FD260, &gDefaultSfxReverbAdd);
            }
        }
    }
}

// OoT func_800F4138
void func_8019F780(Vec3f* pos, u16 sfxId, f32 arg2) {
    func_8019F5AC(arg2);
    Audio_PlaySfxGeneral(sfxId, pos, 4, &D_801FD264, &D_801FD25C, &gDefaultSfxReverbAdd);
}

void Audio_PlaySfxForGiantsMaskUnused(Vec3f* pos, u16 sfxId) {
    Audio_PlaySfxGeneral(sfxId | 0xE0, pos, 4, &sGiantsMaskFreq, &gDefaultSfxVolOrFreq, &sGiantsMaskReverbAdd);
}

void Audio_PlaySfxForGiantsMask(Vec3f* pos, u16 sfxId) {
    Audio_PlaySfxGeneral((sfxId & 0x681F) + 0x20, pos, 4, &sGiantsMaskFreq, &gDefaultSfxVolOrFreq, &sGiantsMaskReverbAdd);
}

void Audio_PlaySfxRandom(Vec3f* pos, u16 baseSfxId, u8 randLim) {
    u8 offset = Audio_NextRandom() % randLim;

    Audio_PlaySfxGeneral(baseSfxId + offset, pos, 4, &gDefaultSfxVolOrFreq, &gDefaultSfxVolOrFreq, &gDefaultSfxReverbAdd);
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
                Audio_PlaySfxGeneral(NA_SE_PL_SWORD_CHARGE, pos, 4, &sCurChargeLevelSfxFreq, &gDefaultSfxVolOrFreq, &gDefaultSfxReverbAdd);
                break;
            case 2:
                Audio_PlaySfxGeneral(NA_SE_PL_SWORD_CHARGE, pos, 4, &sCurChargeLevelSfxFreq, &gDefaultSfxVolOrFreq, &gDefaultSfxReverbAdd);
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
    SfxSettings* sfxSettings = func_8019F258(pos);
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

void func_8019FB0C(Vec3f* pos, u16 sfxId, f32 freqScale, u8 arg3) {
    if (freqScale > 1.0f) {
        freqScale = 1.0f;
    }

    func_801A0654(pos, sfxId, (arg3 - (u32)(freqScale * arg3)) & 0xFF);
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
        func_801A0654(pos, sfxId, 0);
        Audio_PlaySfxAtPosWithFreqAndVolume(pos, sfxId, 1.0f, &sWaterWheelVolume);
    }
}

// OoT func_800F4414
// Used for "NA_SE_IT_DEKUNUTS_FLOWER_ROLL" and "NA_SE_IT_FISHING_REEL_SLOW"
// D_801FD288 was modified in OoT, but remains 1.0f in MM
void func_8019FCB8(Vec3f* pos, u16 sfxId, f32 arg2) {
    D_801D6658--;
    if (D_801D6658 == 0) {
        Audio_PlaySfxGeneral(sfxId, pos, 4, &D_801FD288, &gDefaultSfxVolOrFreq, &gDefaultSfxReverbAdd);

        if (arg2 > 2.0f) {
            arg2 = 2.0f;
        }
        D_801D6658 = (s8)((D_801D6660 - D_801D665C) * (1.0f - arg2)) + D_801D6660;
    }
}

// OoT func_800F44EC
void func_8019FD90(s8 arg0, s8 arg1) {
    D_801D6658 = 1;
    D_801D665C = arg1;
    D_801D6660 = arg0;
}

// OoT func_800F4524
void Audio_PlaySfxAtPosWithReverb(Vec3f* pos, u16 sfxId, s8 reverbAdd) {
    D_801FD28C = reverbAdd;
    Audio_PlaySfxGeneral(sfxId, pos, 4, &gDefaultSfxVolOrFreq, &gDefaultSfxVolOrFreq, &D_801FD28C);
}

// OoT func_800F4578
void Audio_PlaySfxAtPosWithVolume(Vec3f* pos, u16 sfxId, f32 volume) {
    D_801D6654 = volume;
    Audio_PlaySfxGeneral(sfxId, pos, 4, &gDefaultSfxVolOrFreq, &D_801D6654, &gDefaultSfxReverbAdd);
}

void func_8019FE74(f32* arg0, f32 arg1, u16 arg2) {
    D_801FD294 = arg0;
    D_801FD298 = arg1;
    D_801D66A4 = arg2;
    D_801FD29C = (*D_801FD294 - D_801FD298) / D_801D66A4;
}

void func_8019FEDC(void) {
    if (D_801D66A4 != 0) {
        D_801D66A4--;
        if (D_801D66A4 == 0) {
            *D_801FD294 = D_801FD298;
        } else {
            *D_801FD294 -= D_801FD29C;
        }
    }
}

// OoT func_800F45D0
void func_8019FF38(f32 arg0) {
    func_8019FCB8(&gDefaultSfxPos, NA_SE_IT_FISHING_REEL_SLOW - SFX_FLAG, arg0);
    Audio_PlaySfxAtPosWithFreq(&gDefaultSfxPos, 0, (0.15f * arg0) + 1.4f);
}

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



void Audio_PlaySignalBigBellsSfx(Vec3f* pos, u8 arg1) {
    static f32 sBigBellsVolume[8] = {
        1.0f, 0.9f, 0.8f, 0.7f, 0.6f, 0.5f, 0.4f, 0.3f,
    };
    Audio_PlaySfxGeneral(NA_SE_EV_SIGNAL_BIGBELL, pos, 4, &gDefaultSfxVolOrFreq, &sBigBellsVolume[arg1 & 7], &gDefaultSfxReverbAdd);
}

// OoT func_800F47BC
void func_801A0184(void) {
    Audio_SetVolScale(0, 1, 0, 10);
    Audio_SetVolScale(3, 1, 0, 10);
}

// OoT func_800F47FC
void func_801A01C4(void) {
    Audio_SetVolScale(0, 1, 0x7F, 3);
    Audio_SetVolScale(3, 1, 0x7F, 3);
}

void func_801A0204(s8 seqId) {
    Audio_QueueCmdS8(MK_AUDIO_CMD(0x46, 0, 0, 2), (u8)seqId);
}

// OoT func_800F483C
void func_801A0238(u8 targetVolume, u8 volumeFadeTimer) {
    Audio_SetVolScale(0, 0, targetVolume, volumeFadeTimer);
}

void func_801A026C(u8 arg0) {
    u8 i;
    s8 phi_s1 = 0;

    if (arg0 == 0) {
        phi_s1 = 0x7F;
    }

    for (i = 0; i < 16; i++) {
        Audio_QueueCmdS8(((u8)(u32)i << 8) | 0x7000000, phi_s1);
    }

    if (arg0 == 7) {
        D_801D66A0 = 2;
    } else {
        func_801A0318(D_801D6698[arg0 & 7]);
    }
}

s32 func_801A0318(u8 targetVolume) {
    s8 temp_v1;
    u8 phi_v0;
    u8 i;
    s32 phi_v0_2;

    if (D_801FD291 != targetVolume) {
        Audio_SetVolScale(0, 0, targetVolume, 2);

        if (targetVolume < 0x40) {
            phi_v0 = 0x10;
        } else {
            phi_v0 = (((targetVolume - 0x40) >> 2) + 1) << 4;
        }

        Audio_QueueSeqCmd(phi_v0 | 0x80040F00);

        for (i = 0; i < ARRAY_COUNT(gAudioContext.seqPlayers[0].channels); i++) {

            if (&gAudioContext.sequenceChannelNone != gAudioContext.seqPlayers[0].channels[i]) {
                temp_v1 = gAudioContext.seqPlayers[0].channels[i]->soundScriptIO[5];
                if ((temp_v1 & 0xFF) != 0xFF) {

                    phi_v0_2 = (u16)((temp_v1 - targetVolume) + 0x7F);

                    temp_v1 = (u32)i & 0xFF;
                    if (phi_v0_2 > 0x7F) {
                        phi_v0_2 = 0x7F;
                    }

                    Audio_QueueCmdS8(((u8)temp_v1 << 8) | 0x5000000, (u16)phi_v0_2);
                }
            }
        }
        D_801FD291 = targetVolume;
    }
    return -1;
}

void func_801A0450(u8 arg0) {
    D_801FD28D = arg0;
    D_801FD28F = true;
}

void func_801A046C(void) {
    if (D_801FD28F == true) {
        if (D_801FD28E != D_801FD28D) {
            Audio_SetVolScale(0, 0, D_801FD28D, 10);
            D_801FD28E = D_801FD28D;
            D_801FD290 = 1;
        }
        D_801FD28F = false;
    } else if ((D_801FD290 == 1) && (D_801D66AC == 0)) {
        Audio_SetVolScale(0, 0, 0x7F, 10);
        D_801FD28E = 0x7F;
        D_801FD290 = 0;
    }

    if (D_801D66A0 != 0) {
        D_801D66A0--;
        if (D_801D66A0 == 0) {
            func_801A0318(D_801D6698[7]);
        }
    }
}

// Unused remnant of OoT
void Audio_PlaySfxIncreasinglyTransposed(Vec3f* pos, s16 sfxId, u8* semitones) {
    Audio_PlaySfxGeneral(sfxId, pos, 4, &gNoteFrequencies[semitones[sAudioIncreasingTranspose] + 39], &gDefaultSfxVolOrFreq,
                         &gDefaultSfxReverbAdd);
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
void func_801A0654(Vec3f* pos, u16 sfxId, u8 arg2) {
    u8 phi_s1 = 0;
    u8 bankId = SFX_BANK_SHIFT(sfxId);
    u8 entryIndex;
    u8 i;

    for (i = 0; i < bankId; i++) {
        phi_s1 += gChannelsPerBank[gSfxChannelLayout][i];
    }

    for (i = 0; i < gChannelsPerBank[gSfxChannelLayout][bankId]; i++) {
        entryIndex = gActiveSounds[bankId][i].entryIndex;

        if (entryIndex != 0xFF) {
            if ((sfxId == gSoundBanks[bankId][entryIndex].sfxId) &&
                (&pos->x == gSoundBanks[bankId][entryIndex].posX)) {
                Audio_QueueCmdS8(_SHIFTL(6, 24, 8) | _SHIFTL(2, 16, 8) | _SHIFTL(phi_s1, 8, 8) | _SHIFTL(6, 0, 8),
                                 arg2);
            }
        }
        phi_s1++;
    }
}

void func_801A0810(Vec3f* pos, u16 sfxId, u8 arg2) {
    func_801A0654(pos, sfxId, arg2);
    Audio_PlaySfxGeneral(sfxId, pos, 4, &gDefaultSfxVolOrFreq, &gDefaultSfxVolOrFreq, &gDefaultSfxReverbAdd);
}

// OoT func_800F4C58
void func_801A0868(Vec3f* pos, u16 sfxId, u8 arg2) {
    u8 phi_s1 = 0;
    u8 i;
    u8 bankId = SFX_BANK_SHIFT(sfxId);

    for (i = 0; i < bankId; i++) {
        phi_s1 += gChannelsPerBank[gSfxChannelLayout][i];
    }

    for (i = 0; i < gChannelsPerBank[gSfxChannelLayout][bankId]; i++) {
        Audio_QueueCmdS8(_SHIFTL(6, 24, 8) | _SHIFTL(2, 16, 8) | _SHIFTL(phi_s1++, 8, 8) | _SHIFTL(6, 0, 8), arg2);
    }

    Audio_PlaySfxGeneral(sfxId, pos, 4, &gDefaultSfxVolOrFreq, &gDefaultSfxVolOrFreq, &gDefaultSfxReverbAdd);
}

// OoT func_800F4E30
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
            Audio_SeqCmd6(0, 2, i, (127.0f * phi_f22));
            Audio_QueueCmdS8(0x03000000 | ((u8)((u32)i) << 8), phi_s4);
        }
    }
}

void Audio_ClearSariaBgm(void) {
    if (sSariaBgmPtr != NULL) {
        sSariaBgmPtr = NULL;
    }
}

void Audio_ClearSariaBgmAtPos(Vec3f* pos) {
    if (sSariaBgmPtr == pos) {
        sSariaBgmPtr = NULL;
    }
}

// OoT func_800F510C
void func_801A0CB0(s8 volumeSplit) {
    u8 volume;
    u8 priority;
    u16 channelBits;
    u8 players[2] = { 0, 3 };
    u8 i;
    u8 j;

    if ((func_801A8A50(1) == NA_BGM_DISABLED) && (func_801A8A50(3) != NA_BGM_ROMANI_RANCH)) {
        for (i = 0; i < 2; i++) {
            if (i == 0) {
                volume = volumeSplit;
            } else {
                volume = 0x7F - volumeSplit;
            }

            if (volume > 100) {
                priority = 11;
            } else if (volume < 20) {
                priority = 2;
            } else {
                priority = ((volume - 20) / 10) + 2;
            }

            channelBits = 0;
            for (j = 0; j < ARRAY_COUNT(gAudioContext.seqPlayers[players[i]].channels); j++) {
                if (gAudioContext.seqPlayers[players[i]].channels[j]->notePriority < priority) {
                    channelBits += (1 << j);
                }
            }

            Audio_SeqCmdA(players[i], channelBits);
        }
    }
}

void func_801A0E44(u8 arg0, Vec3f* arg1, s16 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6) {
    f32 temp_f0;
    f32 sp28;
    s8 sp27;
    s8 sp26;
    s32 sp1C;

    if (arg1->z > 0.0f) {
        if (arg1->z > 100.0f) {
            sp27 = 0;
        } else {
            sp27 = ((100.0f - arg1->z) / 100.0f) * 64.0f;
        }
    } else if (arg1->z < -100.0f) {
        sp27 = 0x7F;
    } else {
        sp27 = (s8)((-arg1->z / 100.0f) * 64.0f) + 0x3F;
    }

    if (arg1->x > 0.0f) {
        if (arg1->x > 200.0f) {
            sp26 = 0x6C;
        } else {
            sp26 = (s8)((arg1->x / 200.0f) * 45.0f) + 0x3F;
        }
    } else if (arg1->x < -200.0f) {
        sp26 = 0x14;
    } else {
        sp26 = (s8)(((arg1->x + 200.0f) / 200.0f) * 44.0f) + 0x14;
    }

    temp_f0 = sqrtf(SQ(arg1->z) + ((SQ(arg1->x) * 0.25f) + (SQ(arg1->y) / 6.0f)));

    if (arg4 < temp_f0) {
        sp28 = arg6;
    } else if (temp_f0 < arg3) {
        sp28 = 1.0f;
    } else {
        sp28 = (((arg4 - temp_f0) / (arg4 - arg3)) * (1.0f - arg6)) + arg6;
    }

    Audio_QueueCmdU16(arg0 << 0x10 | 0x90000000, 0xFFFF);

    if (arg2 & 1) {
        Audio_QueueCmdS8(arg0 << 0x10 | 0x12000000 | 0xFF00, sp27);
    }

    if (arg2 & 2) {
        Audio_QueueCmdS8(arg0 << 0x10 | 0x3000000 | 0xFF00, sp26);
    }

    if (arg2 & 4) {
        Audio_QueueCmdS8(arg0 << 0x10 | 0x7000000 | 0xFF00, 0x7F);
    }

    if (arg2 & 8) {
        Audio_QueueCmdS32(arg0 << 0x10 | 0x13000000 | 0xFF00 | 0x54, ((u32)&D_801FD3B8 & ~0xF) + 0x10);
    } else {
        Audio_QueueCmdS32(arg0 << 0x10 | 0x13000000 | 0xFF00, ((u32)&D_801FD3B8 & ~0xF) + 0x10);
    }

    if (arg2 & 0x10) {
        Audio_QueueCmdS8(arg0 << 0x10 | 0x14000000 | 0xFF00, 0x7F);
    } else {
        Audio_QueueCmdS8(arg0 << 0x10 | 0x14000000 | 0xFF00, 0);
    }

    if (arg2 & 0x20) {
        Audio_QueueCmdF32(arg0 << 0x10 | 0x1000000 | 0xFF00, arg5 * sp28);
    }

    if (arg2 & 0x40) {
        Audio_QueueCmdS8(arg0 << 0x10 | 0x7000000 | 0xFF00, 0x40);
    }

    if (arg2 & 0x80) {
        Audio_QueueCmdS8(arg0 << 0x10 | 0x11000000 | 0xFF00, 1);
    }

    if (arg2 & 0x100) {
        Audio_QueueCmdS8(arg0 << 0x10 | 0x5000000 | 0xFF00, 0x37);
    }
}

void func_801A1290(void) {
    if (D_801FD3EC != 0xFF) {
        if ((func_801A8A50(D_801FD3EC) != NA_BGM_FINAL_HOURS) &&
            func_801A8ABC((D_801FD3EC << 24) + NA_BGM_FINAL_HOURS, 0xFF0000FF) && !D_801FD3D8) {
            func_801A0E44(D_801FD3EC, &D_801FD3F0, D_801FD3FC, D_801FD400, D_801FD404, D_801FD408, D_801FD40C);
        }
        D_801FD3EC = 0xFF;
    }
}

void func_801A1348(u8 arg0, Vec3f* arg1, s16 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6) {
    D_801FD3EC = arg0;
    D_801FD3F0.x = arg1->x;
    D_801FD3F0.y = arg1->y;
    D_801FD3F0.z = arg1->z;
    D_801FD3FC = arg2;
    D_801FD400 = arg3;
    D_801FD404 = arg4;
    D_801FD408 = arg5;
    D_801FD40C = arg6;
}

void func_801A13BC(u8 playerIdx, Vec3f* arg1, s8 seqId, u16 arg3) {
    s32 pad[3];
    s32 sp30 = playerIdx << 0x18;
    u32 temp_a0;

    if ((func_801A8A50(playerIdx) == NA_BGM_FINAL_HOURS) || !func_801A8ABC(sp30 + NA_BGM_FINAL_HOURS, 0xFF0000FF) || D_801FD3D8) {
        D_801FD3D8 = true;
    } else if (arg1 != 0) {
        if ((seqId != (s8)(func_801A8A50(playerIdx) & 0xFFFF)) && !gAudioContext.seqPlayers[playerIdx].enabled &&
            (D_801D66FC == NA_BGM_GENERAL_SFX)) {

            temp_a0 = 0xFFFF;
            temp_a0 = (((((Audio_NextRandom() % 0x1E) & 0xFF) + 1) << 0x10) | sp30) | ((seqId & temp_a0) + arg3);
            Audio_QueueSeqCmd(temp_a0);
            D_801D66FC = seqId;
        }

        func_801A1348(playerIdx, arg1, 0x7F, 320.0f, 1280.0f, 1.0f, 0.0f);
    } else {
        Audio_QueueSeqCmd(sp30 | 0x100000FF | 0x50000);
    }
}

// z_obj_sound
void func_801A153C(Vec3f* arg0, s8 seqId) {
    s32 pad[2];
    u16 sp36;
    s32 sp2C;
    u16 seqId0 = func_801A8A50(0);
    u8 seqId1;
    u32 temp_a0;

    if ((seqId0 == NA_BGM_FINAL_HOURS) || !func_801A8ABC(NA_BGM_FINAL_HOURS, 0xFF0000FF) || D_801FD3D8) {
        D_801FD3D8 = true;
        return;
    }

    if (seqId0 == NA_BGM_SONG_OF_SOARING) {
        D_801FD3D8 = true;
    }

    if (arg0 != NULL) {
        if (seqId == NA_BGM_ASTRAL_OBSERVATORY) {
            seqId1 = seqId0 & 0xFF;

            if ((seqId != seqId1) && (D_801FD3AF == 0)) {
                Audio_QueueSeqCmd((u16)seqId);
                D_801D66FC = seqId;
            } else if ((seqId == seqId1) && (D_801D66FC == NA_BGM_GENERAL_SFX)) {
                D_801D66FC = seqId;
            }

            func_801A1348(0, arg0, 0x20, 100.0f, 1500.0f, 0.9f, 0.0f);
        } else {
            if (D_801D66FC == NA_BGM_GENERAL_SFX) {
                temp_a0 = ((((Audio_NextRandom() % 0x1E) & 0xFF) + 1) << 0x10) | ((u16)seqId + 0x7F00);
                Audio_QueueSeqCmd(temp_a0);
                D_801D66FC = seqId;
            }

            if (seqId == NA_BGM_MILK_BAR_DUPLICATE) {
                func_801A1348(0, arg0, 0x1E3, 0.0f, 600.0f, 0.9f, 0.55f);
            } else if (seqId == NA_BGM_MILK_BAR) {
                func_801A1348(0, arg0, 0x1FF, 0.0f, 600.0f, 0.9f, 0.55f);
            } else {
                func_801A1348(0, arg0, 0x3F, 0.0f, 600.0f, 0.9f, 0.55f);
            }
        }
    } else {
        if (D_801D66FC == NA_BGM_ASTRAL_OBSERVATORY) {
            Audio_QueueCmdU16(0x90000000, 0xFFFF);
            Audio_QueueCmdF32(0x100FF00, 1.0f);
            Audio_QueueSeqCmd(NA_BGM_CAVERN | 0xA0000);
        } else {
            Audio_QueueSeqCmd(0x100500FF);
        }

        D_801D66FC = NA_BGM_GENERAL_SFX;
    }

}

// z_obj_sound
void func_801A17F4(Vec3f* arg0, s8 seqId) {
    s32 phi_v0 = false;
    s32 pad;

    if (D_801FD3D9 == NA_BGM_GENERAL_SFX) {
        phi_v0 = true;
    } else if (sqrtf(SQ(arg0->z) + ((SQ(arg0->x) * 0.25f) + (SQ(arg0->y) / 6.0f))) <
               sqrtf(SQ(D_801FD3E0.z) + ((SQ(D_801FD3E0.x) * 0.25f) + (SQ(D_801FD3E0.y) / 6.0f)))) {
        phi_v0 = true;
    }

    if (phi_v0) {
        D_801FD3E0.x = arg0->x;
        D_801FD3E0.y = arg0->y;
        D_801FD3E0.z = arg0->z;
        D_801FD3D9 = seqId;
        D_801FD3DA = 1;
    }
}

void func_801A1904(void) {
    if ((D_801FD3DA != 0) && (D_801FD3B0 == 0)) {
        if (D_801FD3D9 != NA_BGM_GENERAL_SFX) {
            func_801A13BC(1, &D_801FD3E0, D_801FD3D9, 0);
            if (func_801A8A50(1) == NA_BGM_DISABLED) {
                func_801A3038();
            }
            if ((func_801A8A50(0) != NA_BGM_DISABLED) && (func_801A8A50(4) == NA_BGM_DISABLED)) {
                Audio_SetupBgmNatureAmbience(9);
            }
            sAudioCutsceneFlag = true;
        } else {
            func_801A13BC(1, NULL, D_801FD3D9, 0);
            if (func_801A8A50(0) != NA_BGM_DISABLED) {
                Audio_QueueSeqCmd(0x140000FF);
            }
            D_801FD3DA = 0;
            D_801D66FC = NA_BGM_GENERAL_SFX;
            sAudioCutsceneFlag = false;
        }
        D_801FD3D9 = NA_BGM_GENERAL_SFX;
    }
}

void func_801A1A10(u8 playerIdx, u8 volumeFadeTimer) {
    Audio_QueueSeqCmd(((u32)playerIdx << 24) | 0x100000FF | 0x140000);
    if ((playerIdx == 3) && (func_801A8A50(0) != NA_BGM_DISABLED)) {
        Audio_SetVolScale(0, 3, 0x7F, volumeFadeTimer);
        func_801A0CB0(0);
    }
}

void func_801A1A8C(u8 playerIdx, Vec3f* arg1, u8 seqId, u8 arg3, f32 arg4, f32 arg5, f32 arg6) {
    f32 sp44 = sqrtf(SQ(arg1->z) + ((SQ(arg1->x) * 0.25f) + (SQ(arg1->y) / 6.0f)));
    u8 targetVolume;
    u16 seqId0 = func_801A8A50(playerIdx);
    f32 phi_f0;
    s32 pad;

    if (arg5 < sp44) {
        if ((u8)seqId0 == seqId) {
            func_801A1A10(playerIdx, 10);
            D_801FD3B4[playerIdx] = false;
        }
        return;
    }

    if ((!gAudioContext.seqPlayers[playerIdx].enabled && (D_801FD3AF == 0)) || (seqId0 == (NA_BGM_ENEMY | 0x800))) {
        if (playerIdx == 3) {
            Audio_SetVolScale(playerIdx, 3, 0x7F, 1);
        }

        Audio_QueueSeqCmd(((u32)playerIdx << 24) | 0x10000 | seqId);

        D_801FD3B4[playerIdx] = true;
    }

    func_801A0E44(playerIdx, arg1, arg3, arg4, arg5, 1.0, 0.05f);

    if ((playerIdx == 3) && (gAudioContext.seqPlayers[0].enabled == 1)) {

        if (arg5 < sp44) {
            phi_f0 = 1.0f;
        } else if (sp44 < arg4) {
            phi_f0 = 0.0f;
        } else {
            phi_f0 = 1.0f - ((arg5 - sp44) / (arg5 - arg4));
        }

        targetVolume = (u8)(phi_f0 * 127.0f);

        Audio_SetVolScale(0, 3, targetVolume, 10);
        func_801A0CB0(0x7F - targetVolume);
    }
}

void func_801A1D44(Vec3f* vec, u8 seqId, f32 arg2) {
    if (gAudioSpecId != 0xC) {
        D_801FD410.x = vec->x;
        D_801FD410.y = vec->y;
        D_801FD410.z = vec->z;
        D_801FD430 = seqId;
        D_801FD42C = arg2;
        D_801FD431 |= 2;
        D_801FD433 = 4;
    }
}

void func_801A1DB8(Vec3f* vec, u8 seqId, f32 arg2) {
    D_801FD420.x = vec->x;
    D_801FD420.y = vec->y;
    D_801FD420.z = vec->z;
    D_801FD430 = seqId;
    D_801FD431 |= 1;
    D_801FD433 = 4;
}

void func_801A1E0C(void) {
    if (D_801FD433 != 0) {
        if ((D_801FD431 & 2) != 0) {
            func_801A1A8C(3, &D_801FD410, D_801FD430, 0x20, 100.0f, D_801FD42C, 1.0f);
        } else {
            func_801A1A8C(3, &D_801FD420, D_801FD430, 0x28, 100.0f, D_801FD42C, 1.0f);
        }

        D_801FD433--;
        if (D_801FD433 == 0) {
            func_801A1A10(3, 10);
        }

        D_801FD431 = 0;
    }
}

void func_801A1F00(u8 arg0, u16 seqId) {
    if (!sAudioCutsceneFlag && (gAudioSpecId != 0xC)) {
        D_801FD420.x = gDefaultSfxPos.x;
        D_801FD420.y = gDefaultSfxPos.y;
        D_801FD420.z = gDefaultSfxPos.z;
        D_801FD42C = 10000.0f;
        D_801FD436 = 0x80;
        D_801FD430 = seqId;
        D_801FD435 = arg0;
    }
}

void func_801A1F88(void) {
    if (gAudioSpecId != 0xC) {
        D_801FD436 = 1;
        D_801FD430 = NA_BGM_GENERAL_SFX;
    }
}

void func_801A1FB4(u8 arg0, Vec3f* arg1, u16 seqId, f32 arg3) {
    if (!sAudioCutsceneFlag) {
        if ((func_801A8A50(0) & 0xFF) != NA_BGM_SONG_OF_SOARING) {
            if (gAudioSpecId != 0xC) {
                if (arg1 != 0) {
                    if (((D_801FD435 != 0) || (func_801A8A50(0) != NA_BGM_FINAL_HOURS))) {
                        D_801FD420.x = arg1->x;
                        D_801FD420.y = arg1->y;
                        D_801FD420.z = arg1->z;
                        D_801FD42C = arg3;
                        D_801FD436 = 2;
                        D_801FD430 = seqId & 0xFF;
                        D_801FD435 = arg0;
                    }
                }
            }
        }
    }
}

void func_801A2090(void) {
    u16 seqId0 = func_801A8A50(0);
    u8 volumeFadeTimer;

    if ((D_801FD436 != 0) && (D_801FD3B0 == 0)) {
        if ((D_801FD430 == NA_BGM_GENERAL_SFX) || (seqId0 == NA_BGM_SONG_OF_SOARING)) {
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
                func_801A1A10(D_801FD435, volumeFadeTimer);
                D_801FD3B4[D_801FD435] = false;
            }
        } else {
            if ((D_801FD435 == 0) && (seqId0 == NA_BGM_FINAL_HOURS)) {
                func_801A1A10(D_801FD435, 10);
                D_801FD3B4[D_801FD435] = false;
                return;
            }

            func_801A1A8C(D_801FD435, &D_801FD420, D_801FD430, 0x20, 200.0f, D_801FD42C, 1.0f);
            if (!D_801FD3B4[D_801FD435]) {
                D_801FD436 = 0;
            }
        }

        if (D_801FD436 < 128) {
            D_801FD430 = NA_BGM_GENERAL_SFX;
        }
    }
}

// Unused remnant of OoT
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
        func_801A3238(3, seqId, 0, 7, 2);
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
        func_801A0CB0(targetVolume);
    }

    Audio_SetVolScale(3, 3, targetVolume, 0);
    Audio_SetVolScale(0, 3, 0x7F - targetVolume, 0);
}

void Audio_ClearSariaBgm2(void) {
    sSariaBgmPtr = NULL;
}

void func_801A246C(u8 playerIdx, u8 arg1) {
    u16 seqId;
    u8 targetVolume;
    s32 phi_v1 = 0;

    targetVolume = 0;

    switch (arg1) {
        case 0:
            targetVolume = 0x7F;
            break;
        case 1:
            phi_v1 = 0xFFFF;
            break;
        case 2:
            targetVolume = 0xFF;
            break;
    }

    if (targetVolume != 0xFF) {
        Audio_QueueSeqCmd((playerIdx << 24) | 0xA0000000 | phi_v1);
        Audio_SetVolScale(0, 3, targetVolume, 1);
    } else {
        Audio_SetVolScale(0, 3, 0x7F, 0);
        seqId = func_801A8A50(playerIdx);

        if (seqId != NA_BGM_DISABLED) {
            Audio_QueueSeqCmd((playerIdx << 24) | seqId);
        }
    }
}

void func_801A2544(u16 seqId, u8 arg1) {
    func_801A257C(seqId);
    Audio_QueueSeqCmd(arg1 | 0x70040000);
}

void func_801A257C(u16 seqId) {
    if (seqId != NA_BGM_NATURE_AMBIENCE) {
        Audio_QueueSeqCmd(0x140000FF);
        func_801A2670(seqId);
        func_801A3238(0, seqId, 0, 0, 1);
    } else {
        Audio_SetupBgmNatureAmbience(8);
    }
}

void func_801A25E4(u16 seqId, u8 arg1) {
    if (D_801FD2A0 != seqId) {
        if (seqId == NA_BGM_NATURE_AMBIENCE) {
            Audio_SetupBgmNatureAmbience(8);
        } else if ((seqId != NA_BGM_FINAL_HOURS) || (D_801D66CC == NA_BGM_DISABLED)) {
            func_801A2670(seqId);
            Audio_QueueSeqCmd(arg1 | 0x70040000);
        }
        D_801FD2A0 = seqId;
    }
}

void func_801A2670(u16 seqId) {
    u8 seqId0;
    u8 fadeTimer = 0;
    u8 phi_v0;

    if ((sBgmFlags[D_801D66D4] & 0x20) && (seqId0 = seqId, (sBgmFlags[seqId0 & 0xFF] & 0x10))) {
        if ((D_801D66D0 % 64) != 0) {
            fadeTimer = 30;
        }
        func_801A3238(0, seqId, fadeTimer, 7, D_801D66D0);
        D_801D66D0 = 0;
    } else {
        seqId0 = seqId;
        if (sBgmFlags[seqId0 & 0xFF] & 0x40) {
            phi_v0 = 1;
        } else {
            phi_v0 = 0xFF;
        }
        func_801A3238(0, seqId, 0, 7, phi_v0);
        if (!(sBgmFlags[seqId] & 0x20)) {
            D_801D66D0 = 0xC0;
        }
    }
    D_801D66D4 = seqId0;
}

void func_801A2778(void) {
    u16 seqId0 = func_801A8A50(0);

    if ((seqId0 != NA_BGM_DISABLED) && (sBgmFlags[(u8)seqId0 & 0xFF] & 0x10)) {
        if (D_801D66D0 != 0xC0) {
            D_801D66D0 = gAudioContext.seqPlayers[0].soundScriptIO[3];
        } else {
            D_801D66D0 = 0;
        }
    }
}

void func_801A27E8(void) {
    if (func_801A8A50(0) != NA_BGM_SONG_OF_STORMS) {
        Audio_QueueSeqCmd(NA_BGM_SONG_OF_STORMS | 0x8000);
    }
}

void func_801A281C(f32 arg0, u8 arg1) {
    if (arg0 == 1.0f) {
        Audio_QueueSeqCmd(arg1 << 0x10 | 0xB0004000);
    } else {
        Audio_QueueSeqCmd(arg1 << 0x10 | 0xB0003000 | (u16)(arg0 * 100.0f));
    }

    Audio_QueueSeqCmd(arg1 << 0x10 | 0x50000000 | (u16)(arg0 * 1000.0f));
}

void func_801A29D4(u8 playerIdx, f32 arg1, u8 arg2) {
    if (arg1 == 1.0f) {
        Audio_QueueSeqCmd(playerIdx << 24 | 0xB0004000 | arg2 << 0x10);
    } else {
        Audio_QueueSeqCmd(playerIdx << 24 | 0xB0003000 | arg2 << 0x10 | (u16)(arg1 * 100.0f));
    }

    Audio_QueueSeqCmd(playerIdx << 24 | 0x50000000 | arg2 << 0x10 | (u16)(arg1 * 1000.0f));
}

void func_801A2BB8(u16 seqId) {
    Audio_SetVolScale(3, 3, 0x7F, 0);
    Audio_QueueSeqCmd(seqId | 0x3000000);
    Audio_SetVolScale(0, 3, 0, 5);
    Audio_QueueSeqCmd(0xC380030A);
    Audio_QueueSeqCmd(0xC3900000);
}

void func_801A2C20(void) {
    Audio_QueueSeqCmd(0x130000FF);
}

void func_801A2C44(void) {
    if ((func_801A8A50(0) == NA_BGM_MINI_GAME_2) && func_801A8ABC(0, 0xF0000000)) {
        Audio_QueueSeqCmd(0xB00500D2);
    }
}

void func_801A2C88(u16 seqId) {
    u8 seqId0 = seqId;

    if (sBgmFlags[seqId0 & 0xFF] & 2) {
        func_801A3098(seqId);
    } else if (sBgmFlags[seqId0 & 0xFF] & 4) {
        Audio_QueueSeqCmd(seqId | 0x1000000);
    } else if (sBgmFlags[seqId0 & 0xFF] & 0x80) {
        Audio_QueueSeqCmd(seqId | 0x3000000);
    } else {
        func_801A3238(0, seqId, 0, 7, 0xFF);
        Audio_SetVolScale(0, 1, 0x7F, 0);
        Audio_SetVolScale(0, 3, 0x7F, 0);
        Audio_QueueSeqCmd(0x110000FF);
    }
}

void func_801A2D54(u16 seqId) {
    u8 seqId0 = seqId;

    if (sBgmFlags[seqId0 & 0xFF] & 2) {
        Audio_QueueSeqCmd(0x110000FF);
    } else if (sBgmFlags[seqId0 & 0xFF] & 4) {
        Audio_QueueSeqCmd(0x110000FF);
    } else if (sBgmFlags[seqId0 & 0xFF] & 0x80) {
        Audio_QueueSeqCmd(0x130000FF);
    } else {
        Audio_QueueSeqCmd(0x100000FF);
    }
}

// OoT func_800F5A58
s32 Audio_IsSequencePlaying(u8 seqId) {
    u8 playerIdx = 0;

    if (sBgmFlags[seqId & 0xFF] & 2) {
        playerIdx = 1;
    } else if (sBgmFlags[seqId & 0xFF] & 4) {
        playerIdx = 1;
    }

    if (seqId == (func_801A8A50(playerIdx) & 0xFF)) {
        return true;
    } else {
        return false;
    }
}

void func_801A2E54(u16 seqId) {
    u16 curSeqId = func_801A8A50(0);

    if (curSeqId == NA_BGM_DISABLED) {
        curSeqId = NA_BGM_GENERAL_SFX;
    }

    if (curSeqId != seqId) {
        func_801A32CC(3);
        if (!(sBgmFlags[curSeqId] & 8)) {
            D_801D66CC = curSeqId;
        }
        Audio_QueueSeqCmd(seqId + 0x8000);
    }
}

void func_801A2ED8(void) {
    if ((func_801A8A50(0) != NA_BGM_DISABLED) && (sBgmFlags[func_801A8A50(0) & 0xFF] & 8)) {
        if ((D_801D66CC == NA_BGM_DISABLED) || (D_801D66CC == NA_BGM_GENERAL_SFX)) {
            Audio_QueueSeqCmd(0x100000FF);
        } else {
            if (D_801D66CC == NA_BGM_NATURE_AMBIENCE) {
                D_801D66CC = D_801FD438;
            }
            Audio_QueueSeqCmd(D_801D66CC + 0x8000);
        }
        D_801D66CC = NA_BGM_DISABLED;
    }
}

// Unused
void func_801A2F88(u8 natureSeqId) {
    u16 seqId = func_801A8A50(0);

    if (seqId != NA_BGM_NATURE_AMBIENCE) {
        D_801D66CC = seqId;
    }

    Audio_SetupBgmNatureAmbience(natureSeqId);
}

// Unused
void func_801A2FC4(void) {
    if (D_801D66CC != NA_BGM_DISABLED) {
        Audio_QueueSeqCmd(D_801D66CC + 0x8000);
    }

    D_801D66CC = NA_BGM_DISABLED;
}

void func_801A3000(u16 seqId, u8 arg1) {
    func_801A3098(seqId);
    Audio_QueueSeqCmd(arg1 | 0x71070000);
}

void func_801A3038(void) {
    Audio_SetVolScale(0, 1, 0, 5);
    Audio_SetVolScale(3, 1, 0, 5);
    Audio_QueueSeqCmd(0xC180010A);
    Audio_QueueSeqCmd(0xC183010A);
    Audio_QueueSeqCmd(0xC1900000);
}

void func_801A3098(u16 seqId) {
    u16 seqId1 = func_801A8A50(1);
    u32 sp20;
    u8* sp1C = func_80193C04(seqId1 & 0xFF, &sp20);
    u8* ret = func_80193C04(seqId & 0xFF, &sp20);

    if ((seqId1 == NA_BGM_DISABLED) || (*sp1C == *ret)) {
        D_801FD3AB = 1;
    } else {
        D_801FD3AB = 5;
        Audio_QueueSeqCmd(0x110000FF);
    }

    D_801FD3AC = seqId;
}

// OoT func_800F5CF8
void func_801A312C(void) {
    if (D_801FD3AB != 0) {
        if ((D_801FD3AB != 5) && !func_801A8ABC(0x11000000, 0xFF000000)) {
            D_801FD3AB = 0;
        } else {
            D_801FD3AB--;
            if (D_801FD3AB == 0) {
                Audio_QueueCmdS32(0xE3000000, SEQUENCE_TABLE);
                Audio_QueueCmdS32(0xE3000000, FONT_TABLE);
                func_801A8A50(0);
                if (func_801A8A50(1) == NA_BGM_DISABLED) {
                    func_801A3038();
                }
                Audio_QueueSeqCmd(D_801FD3AC | 0x1000000);
                Audio_QueueSeqCmd(0xA000FFFF);
            }
        }
    }
}

// OoT func_800F5E18
void func_801A31EC(u16 seqId, s8 arg1, u8 arg2) {
    Audio_QueueSeqCmd((arg1 << 0x10) | 0x71000000 | arg2);
    func_801A3098(seqId);
}

// OoT func_800F5E18
void func_801A3238(s8 playerIdx, u16 seqId, u8 fadeTimer, s8 arg3, u8 arg4) {
    u16 phi_a1;

    Audio_QueueSeqCmd(playerIdx << 24 | 0x70000000 | (arg3 << 0x10) | arg4);
    if ((seqId & 0xFF) < 2) {
        phi_a1 = seqId;
    } else {
        phi_a1 = seqId | 0x8000;
    }
    Audio_QueueSeqCmd(playerIdx << 24 | (fadeTimer << 0x10) | phi_a1);
}

// OoT func_800F5E90
void func_801A32CC(u8 arg0) {
    s32 volumeFadeTimer1;
    u16 seqId0;
    u8 seqId1;
    u8 volumeFadeTimer2;

    if ((D_801D66CC == NA_BGM_DISABLED) && (D_801D66CC == NA_BGM_DISABLED)) {
        // clang-format off
        if (sAudioCutsceneFlag || D_801FD3B4[3]) { \
            arg0 = 3;
        }
        // clang-format on
        seqId0 = D_80200140[0].unk_254;
        seqId1 = seqId0 & 0xFF;
        if ((seqId0 == NA_BGM_DISABLED) || (sBgmFlags[seqId1] & 1)|| ((D_801D66C0 & 0x7F) == 1)) {
            if (arg0 != (D_801D66C0 & 0x7F)) {
                if (arg0 == 1) {
                    if (D_80200140[3].volScales[1] - sAudioEnemyVol < 0) {
                        volumeFadeTimer1 = -(D_80200140[3].volScales[1] - sAudioEnemyVol);
                    } else {
                        volumeFadeTimer1 = D_80200140[3].volScales[1] - sAudioEnemyVol;
                    }

                    Audio_SetVolScale(3, 3, sAudioEnemyVol, volumeFadeTimer1);
                    Audio_StartSeq(3, 10, NA_BGM_ENEMY | 0x800);
                    
                    if (seqId0 >= NA_BGM_TERMINA_FIELD) {
                        Audio_SetVolScale(0, 3, 0x7F - sAudioEnemyVol, 10);
                        func_801A0CB0(sAudioEnemyVol);
                    }
                } else {
                    if ((D_801D66C0 & 0x7F) == 1) {
                        Audio_SeqCmd1(3, 10);
                        
                        if (arg0 == 3) {
                            volumeFadeTimer2 = 0;
                        } else {
                            volumeFadeTimer2 = 10;
                        }

                        Audio_SetVolScale(0, 3, 0x7F, volumeFadeTimer2);
                        func_801A0CB0(0);
                    }
                }

                D_801D66C0 = arg0 + 0x80;
            } else {
                if ((arg0 == 1) && (func_801A8A50(3) == NA_BGM_DISABLED) && (seqId0 != NA_BGM_DISABLED) &&
                    (sBgmFlags[seqId1] & 1)) {
                    Audio_QueueSeqCmd(0x30A0800 | NA_BGM_ENEMY);
                    D_801D66C0 = arg0 + 0x80;
                }
            }
        } else {
            if (arg0 == 0) {
                if (D_801D66C0 == 2) {
                    D_801D66DC = 0;
                }
                D_801D66D8 = 0;
                D_801D66DC++;
            } else {
                D_801D66D8++;
            }

            if (arg0 == 2 && D_801D66D8 < 30 && D_801D66DC > 20) {
                arg0 = 0;
                if (!arg0 && !arg0) {}
            }

            D_801D66C0 = arg0;
            Audio_SeqCmd7(0, 2, arg0);
        }
    }
}

// OoT func_800F6114
void func_801A3590(f32 dist) {
    f32 adjDist;
    u16 seqId0 = D_8020034A;
    u16 seqId1;

    if (D_801D66C0 == 0x81) {
        if (dist != sAudioEnemyDist) {
            if (dist < 150.0f) {
                adjDist = 0.0f;
            } else if (dist > 500.0f) {
                adjDist = 350.0f;
            } else {
                adjDist = dist - 150.0f;
            }

            sAudioEnemyVol = ((350.0f - adjDist) * 127.0f) / 350.0f;
            Audio_SetVolScale(3, 3, sAudioEnemyVol, 10);
            seqId1 = seqId0 & 0xFF;
            if ((seqId0 >= NA_BGM_TERMINA_FIELD) && !(sBgmFlags[seqId1] & 4)) {
                Audio_SetVolScale(0, 3, (0x7F - sAudioEnemyVol), 10);
            }
        }

        func_801A0CB0(sAudioEnemyVol);
    }
    sAudioEnemyDist = dist;
}

// OoT func_800F6114
void func_801A36F0(f32 dist, u16 seqId) {
    s8 pad;
    s8 phi_v1;
    s16 seqId0;

    if (D_801FD3A9 == 0) {
        seqId0 = (s8)(func_801A8A50(0) & 0xFF);
        if (seqId0 == (seqId & 0xFF)) {
            if ((seqId & 0xFF) == NA_BGM_ROMANI_RANCH) {

                if (dist > 2000.0f) {
                    phi_v1 = 127;
                } else if (dist < 200.0f) {
                    phi_v1 = 0;
                } else {
                    phi_v1 = (s8)(((dist - 200.0f) * 127.0f) / 1800.0f);
                }
                Audio_SeqCmd6(0, 3, 0, 127 - phi_v1);
                Audio_SeqCmd6(0, 3, 1, 127 - phi_v1);
                Audio_QueueSeqCmd(phi_v1 | 0x60030D00);
                if (D_801FD3A8 == 0) {
                    D_801FD3A8++;
                }
            }
        } else if ((seqId0 == NA_BGM_NATURE_AMBIENCE) && ((seqId & 0xFF) == NA_BGM_ROMANI_RANCH)) {
            seqId0 = (s8)(func_801A8A50(3) & 0xFF);
            if ((seqId0 != (seqId & 0xFF)) && (D_801FD3A8 < 10)) {
                func_801A3238(3, NA_BGM_ROMANI_RANCH, 0, 0, 0);
                Audio_SeqCmdA(3, 0xFFFC);
                D_801FD3A8 = 10;
            }

            if (dist > 2000.0f) {
                phi_v1 = 127;
            } else if (dist < 200.0f) {
                phi_v1 = 0;
            } else {
                phi_v1 = (s8)(((dist - 200.0f) * 127.0f) / 1800.0f);
            }
            Audio_SeqCmd6(3, 3, 0, 127 - phi_v1);
            Audio_SeqCmd6(3, 3, 1, 127 - phi_v1);
        }

        if (D_801FD3A8 < 10) {
            D_801FD3A8++;
        }
    }
}

u8 func_801A3950(u8 playerIdx, u8 arg1) {
    u8 ret = 0xFF;
    u8 temp_a2;
    SequenceChannel* channel = gAudioContext.seqPlayers[playerIdx].channels[15];
    s8 temp_v0;

    if (gAudioContext.seqPlayers[playerIdx].enabled && channel->enabled) {
        temp_v0 = channel->soundScriptIO[0];
        if (temp_v0 != -1) {
            temp_a2 = temp_v0;
            ret = temp_a2;
            if (arg1 == 1) {
                Audio_QueueSeqCmd((playerIdx << 24) | 0x80000000 | 0xF00 | 0xFF);
            }
        }
    }
    return ret;
}

u8 func_801A39F8(void) {
    u8 ret = 0xFF;
    s32 pad;
    SequenceChannel* channel = gAudioContext.seqPlayers[3].channels[15];
    s8 temp_v0;

    if (gAudioContext.seqPlayers[3].enabled) {
        if ((func_801A8A50(3) == NA_BGM_FROG_SONG) && channel->enabled) {
            temp_v0 = channel->soundScriptIO[0];
            if (temp_v0 != -1) {
                ret = temp_v0;
                Audio_QueueCmdS8(0x6030F00, -1);
            }
        }
    }
    return ret;
}

void func_801A3A7C(u8 arg0) {
    if ((arg0 == 0) && (D_801FD3B0 != 0)) {
        D_801FD3B0 = 1;
    } else {
        D_801FD3B0 = arg0 * 2;
    }
}

void func_801A3AC0(void) {
    if ((D_801FD3B0 != 0) && (D_801FD3B0 != 2)) {
        D_801FD3B0--;
    }
}

// OoT func_800F64E0
void func_801A3AEC(u8 arg0) {
    D_801D66AC = arg0;
    if (arg0 != 0) {
        Audio_PlaySfx1(NA_SE_SY_WIN_OPEN);
        Audio_QueueCmdS32(0xF1FF0000, 0);
    } else {
        Audio_PlaySfx1(NA_SE_SY_WIN_CLOSE);
        Audio_QueueCmdS32(0xF2FF0000, 0);
    }
}

void func_801A3B48(u8 arg0) {
    if (arg0 == 1) {
        Audio_QueueCmdS32(0xF1030000, 0);
    } else {
        Audio_QueueCmdS32(0xF2030000, 0);
    }
}

// OoT func_800F6584
#define Audio_SeqCmd62(playerIdx, a, b, c) \
    Audio_QueueSeqCmd(0x60000000 | ((u32)playerIdx << 24) | ((u8)(a) << 16) | ((u8)b << 8) | ((u8)c))
#define Audio_SeqCmdA2(playerIdx, a) Audio_QueueSeqCmd(0xA0000000 | ((u32)playerIdx << 24) | ((a)))

void func_801A3B90(u8 arg0) {
    u8 playerIdx;
    u16 sp34;

    D_801FD3A9 = arg0;
    if ((func_801A8A50(0) & 0xFF) == NA_BGM_ROMANI_RANCH) {
        playerIdx = 0;
        sp34 = 0;
    } else if ((func_801A8A50(3) & 0xFF) == NA_BGM_ROMANI_RANCH) {
        playerIdx = 3;
        sp34 = 0xFFFC;
    } else {
        return;
    }

    if (arg0 != 0) {
        Audio_SeqCmd62(playerIdx, 1, 0, 0);
        Audio_SeqCmd62(playerIdx, 1, 1, 0);
        if (playerIdx == 3) {
            Audio_SeqCmdA2(playerIdx, sp34 | 3);
        }
    } else {
        if (playerIdx == 3) {
            func_801A3238(3, NA_BGM_ROMANI_RANCH, 0, 0, 0);
        }
        Audio_SeqCmd62(playerIdx, 1, 0, 0x7F);
        Audio_SeqCmd62(playerIdx, 1, 1, 0x7F);
        if (playerIdx == 3) {
            Audio_SeqCmdA2(playerIdx, sp34);
        }
    }
}

void Audio_SetEnvReverb(s8 reverb) {
    sAudioEnvReverb = reverb & 0x7F;
}

void Audio_SetCodeReverb(s8 reverb) {
    u8 temp_a0;

    if (reverb != 0) {
        if ((reverb & 0x40) != (sAudioCodeReverb & 0x40)) {
            temp_a0 = (reverb >> 6) + 1;
            func_801A4428(temp_a0);
        }
        sAudioCodeReverb = reverb & 0x7F;
    }
}

void func_801A3D54(void) {
    s32 phi_v0 = 0;

    if (D_801D66A8 == 2) {
        phi_v0 = 2;
    }

    func_801A0868(&gDefaultSfxPos, NA_SE_SY_SOUT_DEMO, phi_v0);
}

// OoT func_800F6700
void func_801A3D98(s8 arg0) {
    s8 sp1F;

    switch (arg0) {
        case 0:
            sp1F = 0;
            D_801D66A8 = 0;
            break;
        case 1:
            sp1F = 3;
            D_801D66A8 = 3;
            break;
        case 2:
            sp1F = 1;
            D_801D66A8 = 1;
            break;
        case 3:
            sp1F = 4;
            D_801D66A8 = 2;
            break;
    }

    Audio_SeqCmdE0(0, sp1F);
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
    u32 t;
    u8 i;

    sAudioExtraFilter2 = filter;
    sAudioExtraFilter = filter;
    if (D_80200140[4].unk_254 == NA_BGM_NATURE_AMBIENCE) {
        for (i = 0; i < 16; i++) {
            t = i;
            // CHAN_UPD_SCRIPT_IO (seq player 4, all channels, slot 6)
            Audio_QueueCmdS8(((t & 0xFF) << 8) | 0x6040000 | 6, filter);
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

void func_801A3FFC(u8 arg0) {
    D_801FD3AE = arg0;
}

// TODO: bad name (related to scene code)
void audio_setBGM(u8 bgmId) {
    if (D_801FD3AE == 0) {
        Audio_QueueSeqCmd(bgmId | 0xF0000000);
    } else {
        Audio_QueueSeqCmd(bgmId | 0xF0020000);
    }
}

// OoT func_800F6964
void func_801A4058(u16 arg0) {
    s32 skip;
    u8 i;

    if (D_801FD3AE == 0) {
        D_801FD3AF = 1;
        Audio_SeqCmd1(0, (arg0 * 3) / 2);
        Audio_SeqCmd1(1, (arg0 * 3) / 2);
        Audio_SeqCmd1(3, (arg0 * 3) / 2);
    }
    Audio_SeqCmd1(4, (arg0 * 3) / 2);

    for (i = 0; i < 16; i++) {
        skip = false;
        switch (i) {
            case 11:
            case 12:
                if (gAudioSpecId == 10) {} // Remnant of OoT
                if (gAudioSpecId == 11) {}
                skip = true;
                break;
            case 13:
                skip = true;
                break;
        }

        if (!skip) {
            Audio_SeqCmd6(2, (u8)(arg0 >> 1), i, 0);
        }
    }
}

void func_801A41C8(u16 arg0) {
    D_801FD3AE = 1;
    func_801A4058(arg0);
}

// OoT func_800F6AB0
void func_801A41F8(u16 arg0) {
    Audio_SeqCmd1(0, arg0);
    if (func_801A8A50(1) != NA_BGM_DISABLED) {
        Audio_SeqCmd1(1, arg0);
    }
    Audio_SeqCmd1(3, arg0);
    Audio_SeqCmd1(4, arg0);
    Audio_SetVolScale(0, 3, 0x7F, 0);
    Audio_SetVolScale(0, 1, 0x7F, 0);
}

// OoT func_800F6B3C
void func_801A429C(void) {
    func_801A7B10(2, 0, 0xFF, 5);
}

void Audio_DisableAllSeq(void) {
    Audio_DisableSeq(0, 0);
    Audio_DisableSeq(1, 0);
    Audio_DisableSeq(2, 0);
    Audio_DisableSeq(3, 0);
    Audio_DisableSeq(4, 0);
    Audio_ScheduleProcessCmds();
}

// OoT func_800F6BB8
s8 func_801A4324(void) {
    return func_80194528();
}

// OoT func_800F6BDC
void func_801A4348(void) {
    Audio_DisableAllSeq();
    Audio_ScheduleProcessCmds();
    while (true) {
        if (!func_801A4324()) {
            break;
        }
    }
}

void func_801A4380(u8 arg0) {
    u8 i;

    if (D_801FD3AF == 0) {
        for (i = 0; i < 16; i++) {
            switch (i) {
                case 11:
                case 12:
                case 13:
                    break;
                default:
                    Audio_QueueSeqCmd(0x60000000 | (2 << 24) | ((0xA) << 16) | ((u32)i << 8) | (arg0));
                    break;
            }
        }
    }
}

void func_801A4428(u8 arg0) {
    u8 i;

    for (i = 0; i < 16; i++) {
        if (i != 13) {
            Audio_QueueCmdS8(0x11020000 | (((u32)i & 0xFF) << 8), arg0);
        }
    }
}

void Audio_PreNMI(void) {
    Audio_PreNMIInternal();
}

void func_801A44C4(void) {
    D_801FD2A0 = NA_BGM_DISABLED;
}

// OoT func_800F6C34
void Audio_ResetData(void) {
    u8 i;

    for (i = 0; i < ARRAY_COUNT(sSfxSettings); i++) {
        sSfxSettings[i].pos = NULL;
        sSfxSettings[i].freqScale = 1.0;
        sSfxSettings[i].reverbAdd = 0;
    }
    D_801FD250 = 0;
    gLoweredSfxFreq = 0.9f;
    sIncreasedSfxReverb = 20;
    D_801D66A4 = 0;
    D_801D66C0 = 0;
    D_801FD25C = 1.0f;
    D_801FD264 = 1.0f;
    sAudioBaseFilter = 0;
    sAudioExtraFilter = 0;
    sAudioBaseFilter2 = 0;
    sAudioExtraFilter2 = 0;
    func_8019DF28();
    sRiverFreqScaleLerp.remainingFrames = 0;
    sWaterfallFreqScaleLerp.remainingFrames = 0;
    sRiverFreqScaleLerp.value = 1;
    sWaterfallFreqScaleLerp.value = 1;
    D_801FD288 = 1;
    D_801FD28D = 0x7F;
    D_801FD28E = 0x7F;
    D_801FD28F = false;
    D_801FD290 = 0;
    D_801FD291 = 0xFF;
    D_801FD3A8 = 0;
    D_801FD3DA = 0;
    sSpecReverb = sSpecReverbs[gAudioSpecId];
    D_801D66AC = 0;
    D_801D66CC = NA_BGM_DISABLED;
    Audio_QueueCmdS8(0x46000000, -1);
    sSariaBgmPtr = NULL;
    D_801FD3AB = 0;
    D_801FD3AA = 1;
    D_801FD3A9 = 0;
    D_801D66FC = NA_BGM_GENERAL_SFX;
    D_801FD438 = NA_BGM_DISABLED;
    D_801FD431 = 0;
    D_801FD432 = 0;
    D_801FD430 = NA_BGM_GENERAL_SFX;
    D_801FD433 = 0;
    D_801FD434 = 0;
    D_801FD436 = 0;
    D_801FD3AF = 0;
    D_801FD3B0 = 0;
    D_801FD3EC = 0xFF;
    D_801FD3D8 = false;

    for (i = 0; i < ARRAY_COUNT(D_801FD3B4); i++) {
        D_801FD3B4[i] = false;
    }
}

s32 func_801A46F8(void) {
    switch (func_801A3950(1, 1)) {
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

void func_801A4748(Vec3f* pos, u16 sfxId) {
    static f32 D_801D8BD4[] = { 0.0f, 0.0f, 0.0f };
    s32 i;

    for (i = 0; i < ARRAY_COUNT(D_801D8BD4); i++) {
        D_801D8BD4[i] = ((f32*)pos)[i];
    }

    Audio_PlaySfxAtPos((Vec3f*)D_801D8BD4, sfxId);
}

void func_801A479C(Vec3f* pos, u16 sfxId, u16 arg2) {
    Audio_PlaySfxAtPosWithVolume(pos, sfxId, 0.0f);
    func_8019FE74(&D_801D6654, 1.0f, arg2);
}

// OoT func_800F6D58
void func_801A47DC(u8 arg0, u8 arg1, u8 arg2) {
    u8 t;
    u8 temp_a0;
    u8 i;
    u8 a2 = arg2;

    if ((D_80200140[4].unk_254 != NA_BGM_NATURE_AMBIENCE) && func_801A8ABC(NA_BGM_NATURE_AMBIENCE, 0xF00000FF)) {
        return;
    }

    if (((arg0 << 8) + (u32)arg1) == 0x101) {
        if (func_801A8A50(3) != NA_BGM_ROMANI_RANCH) {
            D_801FD3A8 = 0;
        }
    }

    t = arg0 >> 4;
    temp_a0 = arg0 & 0xF;
    if (t == 0) {
        t = arg0 & 0xF;
    }

    for (i = t; i <= temp_a0; i++) {
        Audio_SeqCmd8(4, arg1, (u32)i, (u32)arg2);
    }
}

// OoT func_800F6E7C
void Audio_PlayBgmNatureAmbience(u16 arg0, u16 arg1) {
    u8 i = arg0 & 0xFF;

    Audio_QueueSeqCmd(0x74000001);
    Audio_QueueSeqCmd(((arg0 >> 8) & 0xFF) | 0x74040000);
    Audio_QueueSeqCmd(i | 0x74050000);
    Audio_SetVolScale(0, 0, 0x7F, 1);

    if ((func_801A8A50(4) != NA_BGM_DISABLED) && (func_801A8A50(4) != NA_BGM_NATURE_AMBIENCE)) {
        func_801A7D04(4, 0);
        Audio_QueueCmdS32(0xF8000000, 0);
    }

    if (func_801A8A50(3) == (NA_BGM_ENEMY | 0x800)) {
        Audio_SetVolScale(0, 3, 0x7F, 1);
    }

    Audio_QueueSeqCmd(NA_BGM_NATURE_AMBIENCE | 0x4000000);

    for (i = 0; i < 16; i++) {
        if (!(arg1 & (1 << i)) && (arg0 & (1 << i))) {
            Audio_QueueSeqCmd(((u32)i << 8) | 0x84010000 | 1);
        }
    }
}

// OoT func_800F6FB4
void Audio_SetupBgmNatureAmbience(u8 natureSeqId) {
    u8 i = 0;
    u8 b0;
    u8 b1;
    u8 b2;

    if ((D_80200140[4].unk_254 == NA_BGM_DISABLED) ||
        !(sBgmFlags[((u8)(D_80200140[4].unk_254 ^ 0)) & 0xFF] & 0x80)) {
        if (D_80200140[4].unk_254 != NA_BGM_NATURE_AMBIENCE) {
            D_801FD438 = D_80200140[4].unk_254;
        }

        Audio_PlayBgmNatureAmbience(sNatureAmbienceData[natureSeqId].unk_00, sNatureAmbienceData[natureSeqId].unk_02);

        while ((sNatureAmbienceData[natureSeqId].unk_04[i] != 0xFF) && (i < ARRAY_COUNT(sNatureAmbienceData[natureSeqId].unk_04))) {
            // Probably a fake match, using Audio_SeqCmd8 doesn't work.
            b0 = sNatureAmbienceData[natureSeqId].unk_04[i++];
            b1 = sNatureAmbienceData[natureSeqId].unk_04[i++];
            b2 = sNatureAmbienceData[natureSeqId].unk_04[i++];
            Audio_QueueSeqCmd(0x84000000 | (b1 << 0x10) | (b0 << 8) | b2);
        }

        Audio_SeqCmd8(4, 0x07, 0xD, D_801D66A8);
    }
}

// TODO: UB return
u32 func_801A4B80(u8 channelIdx, SequenceChannel* channel) {
    u32 ret;

    gAudioContext.seqPlayers[4].unk_0b1 = 1;
    if ((channelIdx == 0) || (channelIdx == 0xFF)) {
        gAudioContext.seqPlayers[4].unk_34 = 1.0f;
        // return channelIdx;
    } else {
        ret = Audio_NextRandom();
        gAudioContext.seqPlayers[4].unk_34 = 1.0293 - ((gAudioContext.audioRandom & 0xF) / 144.0f);
        return ret;
    }
}

// OoT func_800F70F8
void Audio_Init(void) {
    AudioLoad_Init(NULL, 0);
}

void func_801A4C54(u16 arg0) {
    u8 i;

    Audio_ScheduleProcessCmds();
    func_801A7B10(2, 0, 0x70, arg0);
    for (i = 0; i < ARRAY_COUNT(sSfxChannelState); i++) {
        Audio_QueueCmdS32(((u8)(u32)i << 8) | 0x10020000, &sSfxChannelState[i]);
    }
    Audio_QueueCmdS32(0xE4000000, func_8019F024);
    Audio_QueueCmdS32(0xE4000001, func_801A4B80);
}

// OoT func_800F711C
void Audio_InitSound(void) {
    Audio_ResetData();
    AudioOcarina_ResetStaff();
    Audio_ResetSfxChannelState();
    func_801A9A74();
    Audio_ResetSounds();
    AudioVoice_ResetData();
    func_801A4C54(0xA);
}

// OoT func_800F7170
void func_801A4D50(void) {
    func_801A4C54(1);
    Audio_QueueCmdS32(0xF2FF0000, 1);
    Audio_ScheduleProcessCmds();
    Audio_QueueCmdS32(0xF8000000, 0);
    D_801FD3D8 = false;
    D_801D66FC = NA_BGM_GENERAL_SFX;
    D_801FD3AE = 0;
}

// New to MM
// D_801DB930[gAudioSpecId][1] somehow
extern ReverbSettings* D_801DB930[10];
void func_801A4DA4(void) {
    func_801A4D50();
    if (gAudioSpecId < 10) {
        Audio_QueueCmdS32(0xE6000100, (s32)(D_801DB930[gAudioSpecId] + 1));
    }
}

// OoT func_800F71BC
void func_801A4DF4(s32 arg0) {
    D_801DB4D8 = 1;
    Audio_ResetData();
    AudioOcarina_ResetStaff();
    Audio_ResetSfxChannelState();
    func_801A99B8();
    Audio_ResetSounds();
    func_801A4FD8();
    if (gAudioSpecId == 0xB) {
        Audio_SetSoundBanksMute(0x6F);
    }
}

// func_800F7208
void func_801A4E64(void) {
    func_801A99B8();
    Audio_QueueCmdS32(0xF2FF0000, 1);
    Audio_ResetData();
    Audio_ResetSfxChannelState();
    func_801A4C54(1);
    func_801A4FD8();
}
