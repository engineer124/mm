#ifndef AUDIO_LOAD_H
#define AUDIO_LOAD_H

#include "PR/ultratypes.h"
#include "libc/stddef.h"
#include "libc/stdint.h"
#include "ultra64/message.h"
#include "unk.h"
#include "os.h"

struct Sample;

typedef s32 (*DmaHandler)(OSPiHandle* handle, OSIoMesg* mb, s32 direction);

typedef struct {
    /* 0x00 */ u32 endAndMediumKey;
    /* 0x04 */ struct Sample* sample;
    /* 0x08 */ u8* ramAddr;
    /* 0x0C */ u32 encodedInfo;
    /* 0x10 */ s32 isFree;
} AudioPreloadReq; // size = 0x14

void AudioLoad_DecreaseSampleDmaTtls(void);
void* AudioLoad_DmaSampleData(uintptr_t devAddr, size_t size, s32 arg2, u8* dmaIndexRef, s32 medium);
void AudioLoad_InitSampleDmaBuffers(s32 numNotes);
s32 AudioLoad_IsFontLoadComplete(s32 fontId);
s32 AudioLoad_IsSeqLoadComplete(s32 seqId);
void AudioLoad_SetFontLoadStatus(s32 fontId, s32 loadStatus);
void AudioLoad_SetSeqLoadStatus(s32 seqId, s32 loadStatus);
void AudioLoad_SyncLoadSeqParts(s32 seqId, s32 arg1, s32 arg2, OSMesgQueue* arg3);
s32 AudioLoad_SyncLoadInstrument(s32 fontId, s32 instId, s32 drumId);
void AudioLoad_AsyncLoadSeq(s32 seqId, s32 arg1, s32 retData, OSMesgQueue* retQueue);
void AudioLoad_AsyncLoadSampleBank(s32 sampleBankId, s32 arg1, s32 retData, OSMesgQueue* retQueue);
void AudioLoad_AsyncLoadFont(s32 fontId, s32 arg1, s32 retData, OSMesgQueue* retQueue);
u8* AudioLoad_GetFontsForSequence(s32 seqId, u32* outNumFonts);
void AudioLoad_DiscardSeqFonts(s32 seqId);
void func_8018FA60(u32 tableType, u32 id, s32 type, s32 data);
s32 AudioLoad_SyncInitSeqPlayer(s32 seqPlayerIndex, s32 seqId, s32 arg2);
s32 AudioLoad_SyncInitSeqPlayerSkipTicks(s32 seqPlayerIndex, s32 seqId, s32 skipTicks);
void AudioLoad_ProcessLoads(s32 resetStatus);
void AudioLoad_SetDmaHandler(DmaHandler callback);
void AudioLoad_Init(void* heap, u32 heapSize);
void AudioLoad_InitSlowLoads(void);
s32 AudioLoad_SlowLoadSample(s32 fontId, s32 instId, s8* isDone);
s32 AudioLoad_SlowLoadSeq(s32 seqId, u8* ramAddr, s8* isDone);
void AudioLoad_InitAsyncLoads(void);
void AudioLoad_LoadPermanentSamples(void);
void AudioLoad_ScriptLoad(s32 tableType, s32 id, s8* isDone);
void AudioLoad_ProcessScriptLoads(void);
void AudioLoad_InitScriptLoads(void);

#endif
