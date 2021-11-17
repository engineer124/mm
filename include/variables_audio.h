#ifndef _VARIABLES_AUDIO_H_
#define _VARIABLES_AUDIO_H_

#include "z64.h"
#include "segment_symbols.h"

extern s16 sLowPassFilterData[];
extern s16 sHighPassFilterData[];
extern s16 D_801D3070[];
extern s16 D_801D3700[];
// extern UNK_TYPE1 gSawtoothWaveSample;
// extern UNK_TYPE1 gTriangleWaveSample;
// extern UNK_TYPE1 gSineWaveSample;
// extern UNK_TYPE1 gSquareWaveSample;
// extern UNK_TYPE1 gWhiteNoiseSample;
// extern UNK_TYPE1 D_801D4790;
// extern UNK_TYPE1 gEighthPulseWaveSample;
// extern s16* gQuarterPulseWaveSample[9];
extern s16* gWaveSamples[9];
extern f32 gBendPitchOneOctaveFrequencies[256];
extern f32 gBendPitchTwoSemitonesFrequencies[256];
extern f32 gNoteFrequencies[128];
extern u8 gDefaultShortNoteVelocityTable[];
extern u8 gDefaultShortNoteGateTimeTable[];
extern AdsrEnvelope gDefaultEnvelope[];
extern NoteSubEu gZeroNoteSub;
extern NoteSubEu gDefaultNoteSub;
extern u16 gHeadsetPanQuantization[];
// extern UNK_TYPE2 D_801D58A2;
extern s16 D_801D58A8[];
extern f32 gHeadsetPanVolume[];
// extern UNK_TYPE1 D_801D5B24;
extern f32 gStereoPanVolume[];
// extern UNK_TYPE1 D_801D5D24;
extern f32 gDefaultPanVolume[];
// extern UNK_TYPE1 D_801D5F24;
extern u32 D_801D5FB4;
extern u32 D_801D5FB8;
extern u32 D_801D5FBC;
extern u32 D_801D5FC0;
extern u8 D_801D5FD4[];
extern DmaHandler sDmaHandler;
extern void* sUnusedHandler;
extern s32 gAudioContextInitalized;
extern u32 sLoadIndex;
extern AudioTask* sWaitingAudioTask;
extern s32 D_801D5FF4;
extern u8 curCmdRdPos;
extern u32 audRand;
extern u32 D_801D6000;
// extern UNK_TYPE1 D_801D6010;
// extern UNK_TYPE1 D_801D6014;
// extern u8 D_801D6028[];
// extern u8 D_801D6098[];
// extern UNK_PTR D_801D6188;
// extern UNK_PTR D_801D618C;
extern f32 D_801D6190;
extern s32 D_801D6194;
extern u8 D_801D6200[];
extern u8 D_801D6600[7];
extern u8 gIsLargeSoundBank[7];
extern u8 gChannelsPerBank[4][7];
extern u8 gUsedChannelsPerBank[4][7];
extern f32 D_801D6648;
extern f32 D_801D664C;
extern f32 D_801D6650;
extern f32 D_801D6654;
extern s8 D_801D6658;
extern s8 D_801D665C;
extern s8 D_801D6660;
extern u8 sAudioIncreasingTranspose;
extern u8 sPrevChargeLevel;
extern f32 D_801D6684[];
extern f32 D_801D6694;
extern u8 D_801D6698[];
// extern UNK_TYPE1 D_801D669E;
// extern UNK_TYPE1 D_801D669F;
// extern UNK_TYPE1 D_801D66A0;
extern u16 D_801D66A4;
extern s8 D_801D66A8;
extern s8 D_801D66AC;
extern s8 sAudioCutsceneFlag;
extern s8 sSpecReverb;
extern s8 sAudioEnvReverb;
extern s8 sAudioCodeReverb;
extern u8 D_801D66C0;
extern f32 sAudioEnemyDist;
extern s8 sAudioEnemyVol;
extern u16 D_801D66CC;
extern u8 D_801D66D0;
extern u8 D_801D66D4;
extern u32 D_801D66D8;
extern u32 D_801D66DC;
extern u8 sAudioBaseFilter;
extern u8 sAudioExtraFilter;
extern u8 sAudioBaseFilter2;
extern u8 sAudioExtraFilter2;
extern s8 D_801D66F0;
extern Vec3f* sSariaBgmPtr;
extern f32 D_801D66F8;
extern u8 D_801D66FC;
extern u8 D_801D6700[];
extern s8 sSpecReverbs[];
// extern UNK_TYPE1 D_801D6794;
extern u8 sIsOcarinaInputEnabled;
extern s8 D_801D6FB8;
extern u8 sCurOcarinaNoteIdx;
extern u8 sPrevOcarinaNoteIdx;
extern u8 sOcaStaffBtnCur;
extern u8 sLearnSongLastNote;
extern f32 sCurOcarinaBendFreq;
extern f32 D_801D6FD0;
extern s8 sCurOcarinaBendIdx;
extern s8 D_801D6FD8;
extern s8 sCurOcarinaVibrato;
extern u8 sPlaybackState;
extern u8 D_801D6FE4;
extern u8 D_801D6FE8;
extern u32 sOcarinaFlags;
extern s32 sNotePlaybackTimer;
extern u16 sPlaybackNotePos;
extern u16 sStaffPlaybackPos;
extern u32 D_801D6FFC;
extern u8 sDisplayedNoteValue;
extern u8 sNotePlaybackVolume;
extern u8 sNotePlaybackVibrato;
extern s8 sNotePlaybackBend;
extern f32 sNormalizedNotePlaybackTone;
extern f32 sNormalizedNotePlaybackVolume;
extern u32 D_801D7018;
extern u32 sOcarinaWallCounter;
// extern UNK_TYPE1 D_801D701E;
// extern UNK_TYPE1 D_801D701F;
extern u8 sCurOcarinaSong[8];
extern u8 sOcarinaSongAppendPos;
extern u8 D_801D702C;
extern u8 sButtonToNoteMap[5];
extern u8 sOcaMemoryGameAppendPos;
extern u8 sOcaMemoryGameEndPos;
extern u8 sOcaMemoryGameNumNotes[3];
extern OcarinaNote sOcarinaSongNotes[24][20];
extern OcarinaNote sOoTOcarinaSongs[9][20];
extern u8 sOoTOcarinaSongsNumNotes[12];
extern OcarinaNote* sPlaybackSong;
extern u8* gFrogsSongPtr;
extern u8 sRecordingState;
extern u8 sRecordSongPos;
extern u32 D_801D8510;
extern u8 D_801D8514;
extern u8 D_801D8518;
extern u8 D_801D851C;
extern s8 D_801D8520;
extern u8 D_801D8524;
extern u8 D_801D8528;
extern u8 D_801D852C;
extern u8 D_801D8530;
extern u32 D_801D8534;
extern u8 D_801D8538;
extern OcarinaNote sPierresSong[];
// extern UNK_TYPE2 D_801D853E;
// extern UNK_TYPE1 D_801D8544;
extern OcarinaNote* gScarecrowCustomSongPtr;
extern u8* gScarecrowSpawnSongPtr;
extern OcarinaNote* D_801D88A4;
extern u8 sNoteToButtonMap[16];
extern u8 D_801D88B8[400];
extern OcarinaSongButtons gOcarinaSongButtons[24];
extern u8 D_801D8B20;
extern u8 D_801D8B24;
extern u16 D_801D8B28;
extern u8 D_801D8B2C;
extern s8 D_801D8B30[];
extern f32 D_801D8BB0[];
// extern UNK_TYPE2 D_801D8BD0;
// extern f32 D_801D8BD4[3];
// extern UNK_TYPE1 D_801D8BE0;
// extern UNK_TYPE1 D_801D8E3C;
// extern UNK_TYPE4 D_801D8E40;
// extern UNK_TYPE2 D_801D8E44;
// extern UNK_TYPE1 D_801D8E48;

// extern UNK_TYPE1 D_801D8E50;
// extern UNK_TYPE1 D_801D8F70;
// extern UNK_TYPE1 sEnemyBankParams;
// extern UNK_TYPE1 sPlayerBankParams;
// extern UNK_TYPE1 sItemBankParams;
// extern UNK_TYPE1 sEnvBankParams;
// extern UNK_TYPE1 sSystemBankParams;
// extern UNK_TYPE1 sOcarinaBankParams;
// extern UNK_TYPE1 sVoiceBankParams;
extern SoundParams* gSoundParams[7];
extern u8 sSoundRequestWriteIndex;
extern u8 sSoundRequestReadIndex;
extern SoundBankEntry* gSoundBanks[7];
extern u8 sBankSizes[7];
extern u8 gSfxChannelLayout;
extern u16 D_801DB4A0;
extern Vec3f gDefaultSfxPos;
extern f32 gDefaultSfxFreq;
extern s8 gDefaultSfxReverb;
extern u8 sSeqCmdWrPos;
extern u8 sSeqCmdRdPos;
extern u8 D_801DB4C8;
extern u8 D_801DB4CC[];
extern u8 gAudioSpecId;
extern u8 D_801DB4D8;
extern u32 D_801DB4DC;
// extern UNK_TYPE1 D_801DB4E0;
// extern UNK_TYPE1 D_801DB528;
// extern UNK_TYPE1 D_801DB570;
// extern UNK_TYPE1 D_801DB5B8;
// extern UNK_TYPE1 D_801DB600;
// extern UNK_TYPE1 D_801DB648;
// extern UNK_TYPE1 D_801DB690;
// extern UNK_TYPE1 D_801DB6D8;
// extern UNK_TYPE1 D_801DB720;
// extern UNK_TYPE1 D_801DB750;
// extern UNK_TYPE1 D_801DB798;
// extern UNK_TYPE1 D_801DB870;
// extern UNK_TYPE1 D_801DB8B8;
// extern UNK_TYPE1 D_801DB900;
// extern s32 D_801DB930[10];
extern AudioSpec gAudioSpecs[21];

extern AudioContext gAudioContext; // at 0x80200C70

extern void (*D_80208E68)(void);
extern u32 (*D_80208E6C)(s8 value, SequenceChannel* channel);
extern s32 (*D_80208E70)(SoundFontSample*, s32, s8, s32);
extern Acmd* (*D_80208E74)(Acmd*, s32, s32);
// extern UNK_TYPE1 D_80208E90;
// extern UNK_TYPE1 D_80208E94;
// extern UNK_TYPE1 D_80208E98;
// extern UNK_TYPE1 D_80208E99;
// extern UNK_TYPE1 D_80208E9C;

#endif
