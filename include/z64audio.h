#ifndef Z64AUDIO_H
#define Z64AUDIO_H

#include "PR/ultratypes.h"
#include "PR/os_voice.h"
#include "audiothread_cmd.h"
#include "libc/stddef.h"
#include "unk.h"
#include "audiothread_cmd.h"

#include "audio/effects.h"
#include "audio/heap.h"
#include "audio/list.h"
#include "audio/load.h"
#include "audio/playback.h"
#include "audio/reverb.h"
#include "audio/seqplayer.h"
#include "audio/soundfont.h"
#include "audio/synthesis.h"
#include "sequence.h"

typedef enum {
    /* 0 */ AUDIO_HEAP_RESET_STATE_NONE,
    /* 1 */ AUDIO_HEAP_RESET_STATE_RESETTING,
    /* 2 */ AUDIO_HEAP_RESET_STATE_RESETTING_ALT // Never set to
} AudioHeapResetState;

#define MAX_CHANNELS_PER_BANK 3

#define AUDIO_LERPIMP(v0, v1, t) (v0 + ((v1 - v0) * t))

#define AIBUF_LEN (88 * SAMPLES_PER_FRAME) // number of samples
#define AIBUF_SIZE (AIBUF_LEN * SAMPLE_SIZE) // number of bytes

// Filter sizes
#define FILTER_SIZE (8 * SAMPLE_SIZE)
#define FILTER_BUF_PART1 (8 * SAMPLE_SIZE)
#define FILTER_BUF_PART2 (8 * SAMPLE_SIZE)

#define AUDIO_RELOCATED_ADDRESS_START K0BASE

// To be used with AudioThread_CountAndReleaseNotes()
#define AUDIO_NOTE_RELEASE (1 << 0)
#define AUDIO_NOTE_SAMPLE_NOTES (1 << 1)



typedef enum {
    /* 0 */ SOUNDMODE_STEREO,
    /* 1 */ SOUNDMODE_HEADSET,
    /* 2 */ SOUNDMODE_SURROUND_EXTERNAL,
    /* 3 */ SOUNDMODE_MONO,
    /* 4 */ SOUNDMODE_SURROUND
} SoundMode;

/**
 * The high-level audio specifications requested when initializing or resetting the audio pool.
 * Most often resets during scene transitions, but will highly depend on game play.
 */ 
typedef struct {
    /* 0x00 */ u32 samplingFreq; // Target sampling rate in Hz
    /* 0x04 */ u8 unk_04;
    /* 0x05 */ u8 numNotes;
    /* 0x06 */ u8 numSequencePlayers;
    /* 0x07 */ u8 unk_07; // unused, set to zero
    /* 0x08 */ u8 unk_08; // unused, set to zero
    /* 0x09 */ u8 numReverbs;
    /* 0x0C */ ReverbSettings* reverbSettings;
    /* 0x10 */ u16 sampleDmaBufSize1;
    /* 0x12 */ u16 sampleDmaBufSize2;
    /* 0x14 */ u16 unk_14;
    /* 0x18 */ size_t persistentSeqCacheSize;        // size of cache on audio pool to store sequences persistently
    /* 0x1C */ size_t persistentFontCacheSize;       // size of cache on audio pool to store soundFonts persistently
    /* 0x20 */ size_t persistentSampleBankCacheSize; // size of cache on audio pool to store entire sample banks persistently
    /* 0x24 */ size_t temporarySeqCacheSize;         // size of cache on audio pool to store sequences temporarily
    /* 0x28 */ size_t temporaryFontCacheSize;        // size of cache on audio pool to store soundFonts temporarily
    /* 0x2C */ size_t temporarySampleBankCacheSize;  // size of cache on audio pool to store entire sample banks temporarily
    /* 0x30 */ size_t persistentSampleCacheSize;     // size of cache on audio pool to store individual samples persistently
    /* 0x34 */ size_t temporarySampleCacheSize;      // size of cache on audio pool to store individual samples temporarily
} AudioSpec; // size = 0x38

/**
 * The audio buffer stores the fully processed digital audio before it is sent to the audio interface (AI), then to the
 * digital-analog converter (DAC), then to play on the speakers. The audio buffer is written to by the rsp after
 * processing audio commands, and the audio buffer is read by AI which sends the data to the DAC.
 * This struct parameterizes that buffer.
 */
typedef struct {
    /* 0x00 */ s16 specUnk4;
    /* 0x02 */ u16 samplingFreq; // Target sampling rate in Hz
    /* 0x04 */ u16 aiSamplingFreq; // True sampling rate set to the audio interface (AI) for the audio digital-analog converter (DAC)
    /* 0x06 */ s16 numSamplesPerFrameTarget;
    /* 0x08 */ s16 numSamplesPerFrameMax;
    /* 0x0A */ s16 numSamplesPerFrameMin;
    /* 0x0C */ s16 updatesPerFrame; // for each frame of the audio thread (default 60 fps), number of updates to process audio
    /* 0x0E */ s16 numSamplesPerUpdate;
    /* 0x10 */ s16 numSamplesPerUpdateMax;
    /* 0x12 */ s16 numSamplesPerUpdateMin;
    /* 0x14 */ s16 numSequencePlayers;
    /* 0x18 */ f32 resampleRate;
    /* 0x1C */ f32 updatesPerFrameInv; // inverse (reciprocal) of updatesPerFrame
    /* 0x20 */ f32 updatesPerFrameInvScaled; // updatesPerFrameInv scaled down by a factor of 256
    /* 0x24 */ f32 updatesPerFrameScaled; // updatesPerFrame scaled down by a factor of 4
} AudioBufferParameters; // size = 0x28

typedef struct {
    union {
    /* 0x0 */ u32 opArgs;
        struct {
            /* 0x0 */ u8 op;
            /* 0x1 */ u8 arg0;
            /* 0x2 */ u8 arg1;
            /* 0x3 */ u8 arg2;
        };
    };
    union {
        /* 0x4 */ void* data;
        /* 0x4 */ f32 asFloat;
        /* 0x4 */ s32 asInt;
        /* 0x4 */ u16 asUShort;
        /* 0x4 */ s8 asSbyte;
        /* 0x4 */ u8 asUbyte;
        /* 0x4 */ u32 asUInt;
        /* 0x4 */ void* asPtr;
    };
} AudioCmd; // size = 0x8

typedef struct {
    /* 0x00 */ OSTask task;
    /* 0x40 */ OSMesgQueue* taskQueue;
    /* 0x44 */ void* unk_44; // probably a message that gets unused.
    /* 0x48 */ char unk_48[0x8];
} AudioTask; // size = 0x50

typedef struct {
    /* 0x0000 */ char unk_0000;
    /* 0x0001 */ s8 numSynthesisReverbs;
    /* 0x0002 */ u16 unk_2; // reads from audio spec unk_14, never used, always set to 0x7FFF
    /* 0x0004 */ u16 unk_4;
    /* 0x0006 */ char unk_0006[0xA];
    /* 0x0010 */ s16* adpcmCodeBook;
    /* 0x0014 */ NoteSampleState* sampleStateList;
    /* 0x0018 */ SynthesisReverb synthesisReverbs[4];
    /* 0x0B58 */ char unk_0B58[0x30];
    /* 0x0B88 */ Sample* usedSamples[128];
    /* 0x0D88 */ AudioPreloadReq preloadSampleStack[128];
    /* 0x1788 */ s32 numUsedSamples;
    /* 0x178C */ s32 preloadSampleStackTop;
    /* 0x1790 */ AudioAsyncLoad asyncLoads[0x10];
    /* 0x1D10 */ OSMesgQueue asyncLoadUnkMediumQueue;
    /* 0x1D28 */ char unk_1D08[0x40];
    /* 0x1D68 */ AudioAsyncLoad* curUnkMediumLoad;
    /* 0x1D6C */ u32 slowLoadPos;
    /* 0x1D70 */ AudioSlowLoad slowLoads[2];
    /* 0x1E38 */ OSPiHandle* cartHandle;
    /* 0x1E2C */ OSPiHandle* driveHandle;
    /* 0x1E40 */ OSMesgQueue externalLoadQueue;
    /* 0x1E58 */ OSMesg externalLoadMesgBuf[0x10];
    /* 0x1E98 */ OSMesgQueue preloadSampleQueue;
    /* 0x1EB0 */ OSMesg preloadSampleMesgBuf[0x10];
    /* 0x1EF0 */ OSMesgQueue curAudioFrameDmaQueue;
    /* 0x1F08 */ OSMesg currAudioFrameDmaMesgBuf[0x40];
    /* 0x2008 */ OSIoMesg currAudioFrameDmaIoMesgBuf[0x40];
    /* 0x2608 */ OSMesgQueue syncDmaQueue;
    /* 0x2620 */ OSMesg syncDmaMesg;
    /* 0x2624 */ OSIoMesg syncDmaIoMesg;
    /* 0x263C */ SampleDma* sampleDmas;
    /* 0x2640 */ u32 sampleDmaCount;
    /* 0x2644 */ u32 sampleDmaListSize1;
    /* 0x2648 */ s32 unused2648;
    /* 0x264C */ u8 sampleDmaReuseQueue1[0x100]; // read pos <= write pos, wrapping mod 256
    /* 0x274C */ u8 sampleDmaReuseQueue2[0x100];
    /* 0x284C */ u8 sampleDmaReuseQueue1RdPos; // Read position for dma 1
    /* 0x284D */ u8 sampleDmaReuseQueue2RdPos; // Read position for dma 2
    /* 0x284E */ u8 sampleDmaReuseQueue1WrPos; // Write position for dma 1
    /* 0x284F */ u8 sampleDmaReuseQueue2WrPos; // Write position for dma 2
    /* 0x2850 */ AudioTable* sequenceTable;
    /* 0x2854 */ AudioTable* soundFontTable;
    /* 0x2858 */ AudioTable* sampleBankTable;
    /* 0x285C */ char unk_285C[0x4];
    /* 0x2860 */ u8* sequenceFontTable;
    /* 0x2864 */ u16 numSequences;
    /* 0x2868 */ SoundFont* soundFontList;
    /* 0x286C */ AudioBufferParameters audioBufferParameters;
    /* 0x2994 */ f32 unk_2870;
    /* 0x2898 */ s32 sampleDmaBufSize1;
    /* 0x289C */ s32 sampleDmaBufSize2;
    /* 0x28A0 */ char unk_287C[0x10];
    /* 0x28B0 */ s32 sampleDmaBufSize;
    /* 0x28B4 */ s32 maxAudioCmds;
    /* 0x28B8 */ s32 numNotes;
    /* 0x2898 */ s16 maxTempo;
    /* 0x28BE */ s8 soundMode;
    /* 0x28C0 */ s32 totalTaskCount; // The total number of times the top-level function on the audio thread is run since the last audio reset
    /* 0x28C4 */ s32 curAudioFrameDmaCount;
    /* 0x28C8 */ s32 rspTaskIndex;
    /* 0x28CC */ s32 curAiBufferIndex;
    /* 0x28AC */ Acmd* abiCmdBufs[2]; // Pointer to audio heap where the audio binary interface command lists are stored. Two lists that alternative every frame
    /* 0x28B4 */ Acmd* curAbiCmdBuf;
    /* 0x28DC */ AudioTask* curTask;
    /* 0x28C0 */ AudioTask rspTask[2];
    /* 0x2980 */ f32 unk_2960;
    /* 0x2984*/ s32 refreshRate;
    /* 0x2988 */ s16* aiBuffers[3]; // Pointers to the audio buffer allocated on the initPool contained in the audio heap. Stores fully processed digital audio before transferring to the audio interface (AI)
    /* 0x2994 */ s16 numSamplesPerFrame[3]; // Number of samples to transfer to the audio interface buffer
    /* 0x299C */ u32 audioRandom;
    /* 0x29A0 */ s32 audioErrorFlags;
    /* 0x29A4 */ volatile u32 resetTimer;
    /* 0x29A8 */ u32 (*customSeqFunctions[4])(s8 value, SequenceChannel* channel);
    /* 0x29B8 */ s8 unk_29B8;
    /* 0x29BC */ s32 numAbiCmdsMax; // sMaxAbiCmdCnt
    /* 0x29C0 */ AudioAllocPool sessionPool; // A sub-pool to main pool, contains all sub-pools and data that changes every audio reset
    /* 0x29D0 */ AudioAllocPool externalPool; // pool allocated on an external device. Never used in game
    /* 0x29E0 */ AudioAllocPool initPool; // A sub-pool to the main pool, contains all sub-pools and data that persists every audio reset
    /* 0x29F0 */ AudioAllocPool miscPool; // A sub-pool to the session pool, 
    /* 0x2A00 */ char unk_29D0[0x20]; // probably two unused pools
    /* 0x2A20 */ AudioAllocPool cachePool; // The common pool for all cache entries
    /* 0x2A30 */ AudioAllocPool persistentCommonPool; // A sub-pool to the cache pool, contains all caches for data stored persistently
    /* 0x2A40 */ AudioAllocPool temporaryCommonPool; // A sub-pool to the cache pool, contains all caches for data stored temporarily
    /* 0x2A50 */ AudioCache seqCache; // Cache to store sequences
    /* 0x2B60 */ AudioCache fontCache; // Cache to store soundFonts
    /* 0x2C70 */ AudioCache sampleBankCache; // Cache for loading entire sample banks
    /* 0x2D80 */ AudioAllocPool permanentPool; // Pool to stores audio data that is always loaded in. Primarily used for sfxs
    /* 0x2D90 */ AudioCacheEntry permanentEntries[32]; // indificual entries to the permanent pool
    /* 0x3690 */ AudioSampleCache persistentSampleCache; // Stores individual samples persistently
    /* 0x40A4 */ AudioSampleCache temporarySampleCache; // Stores individual samples temporarily
    /* 0x4338 */ AudioSessionPoolSplit sessionPoolSplit; // splits session pool into the cache pool and misc pool
    /* 0x4348 */ AudioCachePoolSplit cachePoolSplit; // splits cache pool into the persistent & temporary common pools
    /* 0x4350 */ AudioCommonPoolSplit persistentCommonPoolSplit; // splits persistent common pool into caches for sequences, soundFonts, sample banks
    /* 0x435C */ AudioCommonPoolSplit temporaryCommonPoolSplit; // splits temporary common pool into caches for sequences, soundFonts, sample banks
    /* 0x4368 */ u8 sampleFontLoadStatus[0x30];
    /* 0x4398 */ u8 fontLoadStatus[0x30];
    /* 0x43C8 */ u8 seqLoadStatus[0x80];
    /* 0x4448 */ volatile u8 resetStatus;
    /* 0x4449 */ u8 specId;
    /* 0x444C */ s32 audioResetFadeOutFramesLeft;
    /* 0x4450 */ f32* adsrDecayTable; // A table on the audio heap that stores decay rates used for ADSR
    /* 0x4454 */ u8* audioHeap;
    /* 0x4458 */ size_t audioHeapSize;
    /* 0x445C */ Note* notes;
    /* 0x4460 */ SequencePlayer seqPlayers[5];
    /* 0x4B40 */ SequenceLayer sequenceLayers[80];
    /* 0x7840 */ SequenceChannel sequenceChannelNone;
    /* 0x7924 */ s32 sampleStateOffset; // Start of the list of sample states for this update. Resets after each audio frame.
    /* 0x7928 */ AudioListItem layerFreeList;
    /* 0x7938 */ NotePool noteFreeLists;
    /* 0x7978 */ u8 threadCmdWritePos;
    /* 0x7979 */ u8 threadCmdReadPos;
    /* 0x797A */ u8 threadCmdQueueFinished;
    /* 0x797C */ u16 threadCmdChannelMask[5]; // bit-packed for 16 channels. When processing an audio thread channel command on all channels, only process channels with their bit set.
    /* 0x7988 */ OSMesgQueue* audioResetQueueP;
    /* 0x798C */ OSMesgQueue* taskStartQueueP;
    /* 0x7990 */ OSMesgQueue* threadCmdProcQueueP;
    /* 0x7994 */ OSMesgQueue taskStartQueue;
    /* 0x79AC */ OSMesgQueue threadCmdProcQueue;
    /* 0x79C4 */ OSMesgQueue audioResetQueue;
    /* 0x79DC */ OSMesg taskStartMsgs[1];
    /* 0x79E0 */ OSMesg audioResetMesgs[1];
    /* 0x79E4 */ OSMesg threadCmdProcMsgBuf[4];
    /* 0x79F4 */ AudioCmd threadCmdBuf[0x100]; // Audio commands used to transfer audio requests from the graph thread to the audio thread
    /* 0x81F4 */ UNK_TYPE1 unk_81F4[4];
} AudioContext; // size = 0x81F8

typedef struct {
    /* 0x00 */ f32 volCur;
    /* 0x04 */ f32 volTarget;
    /* 0x08 */ f32 volStep;
    /* 0x0C */ f32 freqScaleCur;
    /* 0x10 */ f32 freqScaleTarget;
    /* 0x14 */ f32 freqScaleStep;
    /* 0x18 */ u16 volTimer;
    /* 0x1A */ u16 freqScaleTimer;
} ActiveSequenceChannelData; // size = 0x1C

typedef struct {
    /* 0x000 */ ActiveSequenceChannelData channelData[SEQ_NUM_CHANNELS];
    /* 0x1C0 */ f32 volCur;
    /* 0x1C4 */ f32 volTarget;
    /* 0x1C8 */ f32 volStep;
    /* 0x1CC */ u32 tempoCmd;
    /* 0x1D0 */ f32 tempoCur;
    /* 0x1D4 */ f32 tempoTarget;
    /* 0x1D8 */ f32 tempoStep;
    /* 0x1DC */ u32 setupCmd[8]; // setup commands
    /* 0x1FC */ u32 startAsyncSeqCmd; // temporarily stores the seqCmd used in SEQCMD_PLAY_SEQUENCE, to be called again once the font is reloaded in
    /* 0x200 */ u16 volTimer;
    /* 0x202 */ u16 tempoOriginal;
    /* 0x204 */ u16 tempoTimer;
    /* 0x206 */ u16 freqScaleChannelFlags;
    /* 0x208 */ u16 volChannelFlags;
    /* 0x20A */ u16 seqId;
    /* 0x20C */ u16 prevSeqId; // last seqId played on a player
    /* 0x20E */ u16 channelPortMask;
    /* 0x210 */ u8 isWaitingForFonts;
    /* 0x211 */ u8 fontId;
    /* 0x212 */ u8 volScales[VOL_SCALE_INDEX_MAX];
    /* 0x216 */ u8 volFadeTimer;
    /* 0x217 */ u8 fadeVolUpdate;
    /* 0x218 */ u8 setupCmdTimer;
    /* 0x219 */ u8 setupCmdNum; // number of setup commands
    /* 0x21A */ u8 setupFadeTimer;
    /* 0x21B */ u8 isSeqPlayerInit;
} ActiveSequence; // size = 0x21C

typedef struct {
    /* 0x0 */ u8 seqId;
    /* 0x1 */ u8 priority;
} SeqRequest; // size = 0x02

typedef enum {
    /* 0 */ BANK_PLAYER,
    /* 1 */ BANK_ITEM,
    /* 2 */ BANK_ENV,
    /* 3 */ BANK_ENEMY,
    /* 4 */ BANK_SYSTEM,
    /* 5 */ BANK_OCARINA,
    /* 6 */ BANK_VOICE
} SfxBankType;

typedef enum {
    /* 0 */ SFX_STATE_EMPTY,
    /* 1 */ SFX_STATE_QUEUED,
    /* 2 */ SFX_STATE_READY,
    /* 3 */ SFX_STATE_PLAYING_REFRESH,
    /* 4 */ SFX_STATE_PLAYING,
    /* 5 */ SFX_STATE_PLAYING_ONE_FRAME
} SfxState;

typedef struct {
    /* 0x00 */ f32* posX;
    /* 0x04 */ f32* posY;
    /* 0x08 */ f32* posZ;
    /* 0x0C */ f32* freqScale;
    /* 0x10 */ f32* volume;
    /* 0x14 */ s8* reverbAdd;
    /* 0x18 */ f32 dist;
    /* 0x1C */ u32 priority; // lower is more prioritized
    /* 0x20 */ u16 sfxParams;
    /* 0x22 */ u16 sfxId;
    /* 0x25 */ u8 sfxFlags;
    /* 0x24 */ u8 sfxImportance;
    /* 0x26 */ u8 state; // uses SfxState enum
    /* 0x27 */ u8 freshness;
    /* 0x28 */ u8 prev;
    /* 0x29 */ u8 next;
    /* 0x2A */ u8 channelIndex;
    /* 0x2B */ u8 randFreq;
    /* 0x2C */ u8 token;
} SfxBankEntry; // size = 0x30

/*
 * SfxId:
 *
 * & 03FF    0000000111111111    index
 * & 0400    0000010000000000    unused flag
 * & 0800    0000100000000000    SFX_FLAG
 * & 0C00    0000110000000000    Flag Mask
 * & F000    1111000000000000    observed in audio code
 */

#define SFX_BANK_SHIFT(sfxId)   (((sfxId) >> 12) & 0xFF)

#define SFX_BANK_MASK(sfxId)    ((sfxId) & 0xF000)

#define SFX_INDEX(sfxId)    ((sfxId) & 0x3FF)
#define SFX_BANK(sfxId)     SFX_BANK_SHIFT(SFX_BANK_MASK(sfxId))

typedef struct {
    /* 0x0 */ u32 priority; // lower is more prioritized
    /* 0x4 */ u8 entryIndex;
} ActiveSfx; // size = 0x08

// SfxParams bit-packing

// Slows the decay of volume with distance (a 3-bit number ranging from 0-7)
#define SFX_PARAM_DIST_RANGE_SHIFT 0
#define SFX_PARAM_DIST_RANGE_MASK_UPPER (4 << SFX_PARAM_DIST_RANGE_SHIFT)
#define SFX_PARAM_DIST_RANGE_MASK (7 << SFX_PARAM_DIST_RANGE_SHIFT)

// Lower SEQ_PLAYER_BGM_MAIN and SEQ_PLAYER_BGM_SUB while the sfx is playing
#define SFX_FLAG_LOWER_VOLUME_BGM (1 << 3)

// Sfx priority is not raised with distance (making it more likely to be ejected)
#define SFX_FLAG_PRIORITY_NO_DIST (1 << 4)

// If a new sfx is requested at both the same position with the same importance,
// Block that new sfx from replacing the current sfx
// Note: Only 1 sfx can be played at a specific position at once
#define SFX_FLAG_BLOCK_EQUAL_IMPORTANCE (1 << 5)

// Applies increasingly random offsets to frequency (a 2-bit number ranging from 0-3)
#define SFX_PARAM_RAND_FREQ_RAISE_SHIFT 6
#define SFX_PARAM_RAND_FREQ_RAISE_MASK (3 << SFX_PARAM_RAND_FREQ_RAISE_SHIFT)

// Sets a flag to ioPort 5
#define SFX_FLAG_8 (1 << 8)

// Use lowpass filter on surround sound
#define SFX_FLAG_SURROUND_LOWPASS_FILTER (1 << 9)

// Unused remnant of OoT
#define SFX_FLAG_BEHIND_SCREEN_Z_INDEX_SHIFT 10
#define SFX_FLAG_BEHIND_SCREEN_Z_INDEX (1 << SFX_FLAG_BEHIND_SCREEN_Z_INDEX_SHIFT)

// Randomly scale base frequency each frame through mutiplicative offset
#define SFX_PARAM_RAND_FREQ_SCALE (1 << 11)

// Sfx reverb is not raised with distance
#define SFX_FLAG_REVERB_NO_DIST (1 << 12)

// Sfx volume is not lowered with distance
#define SFX_FLAG_VOLUME_NO_DIST (1 << 13)

// SFX_FLAG_VIBRATO 
// Randomly lower base frequency each frame through additive offset
#define SFX_PARAM_RAND_FREQ_LOWER (1 << 14)

// Sfx frequency is not raised with distance
#define SFX_FLAG_FREQ_NO_DIST (1 << 15)

// Force the sfx to reset from the beginning when requested again
#define SFX_FLAG2_FORCE_RESET (1 << 0)

// Unused
#define SFX_FLAG2_UNUSED2 (1 << 2)
#define SFX_FLAG2_UNUSED4 (1 << 4)

// Do not use highpass filter on surround sound
#define SFX_FLAG2_SURROUND_NO_HIGHPASS_FILTER (1 << 5)

// Unused
#define SFX_FLAG2_UNUSED6 (1 << 6)

// Apply a low-pass filter with a lowPassCutoff of 4
#define SFX_FLAG2_APPLY_LOWPASS_FILTER (1 << 7)

typedef struct {
    /* 0x0 */ u8 importance;
    /* 0x1 */ u8 flags;
    /* 0x2 */ u16 params;
} SfxParams; // size = 0x4

#endif
