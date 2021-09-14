#ifndef _VARIABLES_AUDIO_H_
#define _VARIABLES_AUDIO_H_

#include "z64.h"
#include "segment_symbols.h"

struct DummyA0 { int x; };
struct DummyA1 { int x; };
struct DummyA2 { int x; };
struct DummyA3 { int x; };
struct DummyA4 { int x; };
struct DummyA5 { int x; };
struct DummyA6 { int x; };
struct DummyA7 { int x; };
struct DummyA8 { int x; };
struct DummyA9 { int x; };
struct DummyA10 { int x; };
struct DummyA11 { int x; };
struct DummyA12 { int x; };
struct DummyA13 { int x; };
struct DummyA14 { int x; };
struct DummyA15 { int x; };
struct DummyA16 { int x; };
struct DummyA17 { int x; };
struct DummyA18 { int x; };
struct DummyA19 { int x; };
struct DummyA20 { int x; };
struct DummyA21 { int x; };
struct DummyA22 { int x; };
struct DummyA23 { int x; };
struct DummyA24 { int x; };
struct DummyA25 { int x; };
struct DummyA26 { int x; };
struct DummyA27 { int x; };
struct DummyA28 { int x; };
struct DummyA29 { int x; };
struct DummyA30 { int x; };
struct DummyA31 { int x; };
struct DummyA32 { int x; };
struct DummyA33 { int x; };
struct DummyA34 { int x; };
struct DummyA35 { int x; };
struct DummyA36 { int x; };
struct DummyA37 { int x; };
struct DummyA38 { int x; };
struct DummyA39 { int x; };
struct DummyA40 { int x; };
struct DummyA41 { int x; };
struct DummyA42 { int x; };
struct DummyA43 { int x; };
struct DummyA44 { int x; };
struct DummyA45 { int x; };
struct DummyA46 { int x; };
struct DummyA47 { int x; };
struct DummyA48 { int x; };
struct DummyA49 { int x; };
struct DummyA50 { int x; };
struct DummyA51 { int x; };
struct DummyA52 { int x; };
struct DummyA53 { int x; };
struct DummyA54 { int x; };
struct DummyA55 { int x; };
struct DummyA56 { int x; };
struct DummyA57 { int x; };
struct DummyA58 { int x; };
struct DummyA59 { int x; };
struct DummyA60 { int x; };
struct DummyA61 { int x; };
struct DummyA62 { int x; };
typedef int DummyA63;

extern s16 D_801D2E80[];
extern s16 D_801D2F80[];
// extern s16 D_801D3070[];
// extern s16 D_801D3700[];
// extern UNK_TYPE1 D_801D3D90;
// extern UNK_TYPE1 D_801D3F90;
// extern UNK_TYPE1 D_801D4190;
// extern UNK_TYPE1 D_801D4390;
// extern UNK_TYPE1 D_801D4590;
// extern UNK_TYPE1 D_801D4790;
// extern UNK_TYPE1 D_801D4990;
extern s16* gWaveSamples[9];
extern UNK_PTR D_801D4D90;
extern s16* D_801D4D98[9];
extern UNK_PTR D_801D4DB0;
// extern UNK_TYPE1 D_801D4DB4;
extern f32 gPitchBendFrequencyScale[256];
// extern UNK_TYPE4 D_801D51B4;
extern f32 D_801D53B4[];
// extern UNK_TYPE1 D_801D55B4;
extern u8 gDefaultShortNoteVelocityTable[];
extern u8 gDefaultShortNoteDurationTable[];
extern AdsrEnvelope gDefaultEnvelope[];
extern NoteSubEu gZeroNoteSub;
extern NoteSubEu gDefaultNoteSub;
extern u16 gHeadsetPanQuantization[];
// extern UNK_TYPE2 D_801D58A2;
extern s16 D_801D58AA;
extern f32 gHeadsetPanVolume[];
// extern UNK_TYPE1 D_801D5B24;
extern f32 gStereoPanVolume[];
// extern UNK_TYPE1 D_801D5D24;
extern f32 gDefaultPanVolume[];
// extern UNK_TYPE1 D_801D5F24;
// extern UNK_TYPE4 D_801D5FB4;
// extern UNK_TYPE4 D_801D5FB8;
// extern UNK_TYPE4 D_801D5FBC;
// extern UNK_TYPE4 D_801D5FC0;
// extern u8 D_801D5FD4[];
extern s32 (*sDmaHandler)(OSPiHandle* handle, OSIoMesg* mb, s32 direction);
extern u32 D_801D5FE4;
extern s32 gAudioContextInitalized;
// extern UNK_TYPE4 D_801D5FEC;
// extern UNK_TYPE4 D_801D5FF0;
extern s32 D_801D5FF4;
// extern u8 curCmdRdPos;
// extern UNK_TYPE4 audRand;
extern UNK_PTR D_801D6000;
// extern UNK_TYPE1 D_801D6010;
// extern UNK_TYPE1 D_801D6014;
// extern UNK_TYPE1 D_801D6028;
// extern UNK_TYPE1 D_801D6098;
extern u8 D_801D6100[];
extern UNK_PTR D_801D6188;
extern UNK_PTR D_801D618C;
extern f32 D_801D6190;
extern f32 D_801D6194;
// extern UNK_TYPE1 D_801D6200;
extern u8 D_801D6600[7];
extern u8 D_801D6608[7];
extern u8 D_801D6610[8][7];
extern u8 D_801D662C[8][7];
// extern UNK_TYPE1 D_801D6648;
// extern UNK_TYPE1 D_801D664C;
// extern UNK_TYPE1 D_801D6650;
// extern UNK_TYPE4 D_801D6654;
// extern UNK_TYPE1 D_801D6658;
// extern UNK_TYPE1 D_801D665C;
// extern UNK_TYPE1 D_801D6660;
// extern UNK_TYPE1 D_801D666C;
// extern UNK_TYPE1 D_801D6680;
// extern UNK_TYPE1 D_801D6684;
// extern UNK_TYPE4 D_801D6694;
// extern UNK_TYPE1 D_801D6698;
// extern UNK_TYPE1 D_801D669E;
// extern UNK_TYPE1 D_801D669F;
// extern UNK_TYPE1 D_801D66A0;
// extern UNK_TYPE2 D_801D66A4;
// extern UNK_TYPE1 D_801D66A8;
// extern UNK_TYPE1 D_801D66AC;
// extern UNK_TYPE1 D_801D66B0;
// extern UNK_TYPE1 D_801D66B4;
// extern UNK_TYPE1 D_801D66B8;
// extern UNK_TYPE1 D_801D66BC;
// extern UNK_TYPE1 D_801D66C0;
// extern UNK_TYPE4 sAudioEnemyDist;
// extern UNK_TYPE1 sAudioEnemyVol;
// extern UNK_TYPE2 D_801D66CC;
// extern UNK_TYPE1 D_801D66D0;
// extern UNK_TYPE1 D_801D66D4;
// extern UNK_TYPE4 D_801D66D8;
// extern UNK_TYPE4 D_801D66DC;
// extern UNK_TYPE1 D_801D66E0;
// extern UNK_TYPE1 D_801D66E4;
// extern UNK_TYPE1 D_801D66E8;
// extern UNK_TYPE1 D_801D66EC;
// extern UNK_TYPE1 D_801D66F0;
// extern UNK_TYPE4 D_801D66F4;
// extern UNK_TYPE4 D_801D66F8;
// extern UNK_TYPE1 D_801D66FC;
// extern UNK_TYPE1 D_801D6700;
// extern UNK_TYPE1 D_801D6780;
// extern UNK_TYPE1 D_801D6794;
extern u8 sOcarinaInpEnabled;
// extern UNK_TYPE1 D_801D6FB8;
// extern UNK_TYPE1 D_801D6FBC;
// extern UNK_TYPE1 D_801D6FC0;
// extern UNK_TYPE1 D_801D6FC4;
// extern UNK_TYPE1 D_801D6FC8;
// extern UNK_TYPE4 D_801D6FCC;
// extern UNK_TYPE1 D_801D6FD0;
// extern UNK_TYPE1 D_801D6FD4;
// extern UNK_TYPE1 D_801D6FD8;
// extern UNK_TYPE1 D_801D6FDC;
// extern UNK_TYPE1 D_801D6FE0;
// extern UNK_TYPE1 D_801D6FE4;
// extern UNK_TYPE1 D_801D6FE8;
extern u32 D_801D6FEC;
// extern UNK_TYPE4 D_801D6FF0;
// extern UNK_TYPE2 D_801D6FF4;
// extern UNK_TYPE2 D_801D6FF8;
extern u16 D_801D6FFC;
// extern UNK_TYPE1 D_801D7000;
// extern UNK_TYPE1 D_801D7004;
// extern UNK_TYPE1 D_801D7008;
// extern UNK_TYPE1 D_801D700C;
// extern UNK_TYPE4 D_801D7010;
// extern UNK_TYPE4 D_801D7014;
// extern UNK_TYPE4 D_801D7018;
// extern UNK_TYPE4 D_801D701C;
// extern UNK_TYPE1 D_801D701E;
// extern UNK_TYPE1 D_801D701F;
// extern UNK_TYPE1 D_801D7020;
extern u8 sOcarinaSongAppendPos;
// extern UNK_TYPE1 D_801D702C;
// extern UNK_TYPE1 D_801D7030;
// extern UNK_TYPE1 D_801D7038;
// extern UNK_TYPE1 D_801D703C;
// extern UNK_TYPE1 D_801D7040;
extern OcarinaNote sOcarinaSongs[22][20];
// extern UNK_TYPE4 D_801D7E04;
// extern UNK_TYPE1 D_801D7EA4;
// extern UNK_TYPE1 D_801D7F44;
// extern UNK_TYPE1 D_801D84E4;
extern UNK_PTR D_801D84F0;
// extern UNK_TYPE1 D_801D84F4;
// extern UNK_TYPE1 D_801D8508;
// extern UNK_TYPE1 D_801D850C;
// extern UNK_TYPE4 D_801D8510;
// extern UNK_TYPE1 D_801D8514;
// extern UNK_TYPE1 D_801D8518;
// extern UNK_TYPE1 D_801D851C;
// extern UNK_TYPE1 D_801D8520;
// extern UNK_TYPE1 D_801D8524;
extern u8 D_801D8528;
extern u8 D_801D852C;
extern s32 D_801D8530;
// extern UNK_TYPE4 D_801D8534;
// extern UNK_TYPE1 D_801D8538;
// extern UNK_TYPE1 D_801D853C;
// extern UNK_TYPE2 D_801D853E;
// extern UNK_TYPE1 D_801D8544;
extern UNK_PTR D_801D889C;
extern UNK_PTR D_801D88A0;
extern UNK_PTR D_801D88A4;
extern u8 sNoteValueIndexMap[16];
// extern UNK_TYPE1 D_801D88B8;
extern OcarinaSongInfo gOcarinaSongNotes[24];
// extern UNK_TYPE1 D_801D8B20;
extern UNK_PTR D_801D8B24;
// extern UNK_TYPE2 D_801D8B28;
// extern UNK_TYPE1 D_801D8B2C;
// extern UNK_TYPE1 D_801D8B30;
// extern UNK_TYPE1 D_801D8BB0;
// extern UNK_TYPE2 D_801D8BD0;
// extern UNK_TYPE4 D_801D8BD4;
// extern UNK_TYPE1 D_801D8BE0;
// extern UNK_TYPE1 D_801D8E3C;
// extern UNK_TYPE4 D_801D8E40;
// extern UNK_TYPE2 D_801D8E44;
// extern UNK_TYPE1 D_801D8E48;

// extern UNK_TYPE1 D_801D8E50;
// extern UNK_TYPE1 D_801D8F70;
// extern UNK_TYPE1 D_801D9090;
// extern UNK_TYPE1 D_801D9C10;
// extern UNK_TYPE1 D_801DA350;
// extern UNK_TYPE1 D_801DA510;
// extern UNK_TYPE1 D_801DAC50;
// extern UNK_TYPE1 D_801DADD0;
// extern UNK_TYPE1 D_801DAE10;
extern SoundParams* gSoundParams[7];
extern u8 sSoundRequestWriteIndex;
extern u8 sSoundRequestReadIndex;
extern SoundBankEntry* gSoundBanks[7];
extern u8 sBankSizes[7];
extern u8 D_801DB49C;
extern u16 D_801DB4A0;
extern Vec3f D_801DB4A4;
// extern UNK_TYPE1 D_801DB4B0;
// extern UNK_TYPE1 D_801DB4B8;
extern u8 sSeqCmdWrPos;
// extern UNK_TYPE1 D_801DB4C4;
// extern UNK_TYPE1 D_801DB4C8;
// extern UNK_TYPE1 D_801DB4CC;
// extern UNK_TYPE1 D_801DB4D4;
// extern UNK_TYPE1 D_801DB4D8;
// extern UNK_TYPE4 D_801DB4DC;
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
extern UNK_PTR D_801DB930;
extern AudioSpec gAudioSpecs[21];

extern AudioContext gAudioContext; // at 0x80200C70

// extern UNK_TYPE4 D_80208E6C;
extern UNK_PTR D_80208E68;
extern UNK_PTR D_80208E70;
extern UNK_PTR D_80208E74;
// extern UNK_TYPE1 D_80208E90;
// extern UNK_TYPE1 D_80208E94;
// extern UNK_TYPE1 D_80208E98;
// extern UNK_TYPE1 D_80208E99;
// extern UNK_TYPE1 D_80208E9C;

#endif
