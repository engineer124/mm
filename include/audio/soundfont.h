#ifndef AUDIO_SOUNDFONT_H
#define AUDIO_SOUNDFONT_H

#include "PR/ultratypes.h"

typedef struct {
    /* 0x0 */ s16 delay;
    /* 0x2 */ s16 arg;
} EnvelopePoint; // size = 0x4

typedef enum {
    /* 0 */ LOOP_TYPE_NONE, // Sample does not loop
    /* 1 */ LOOP_TYPE_ALWAYS, // Sample loops indefinitely
    /* 2 */ LOOP_TYPE_CONDITIONAL // Sample loops until PLAYBACK_STATUS_0 is reached, then stops looping
} LoopType;

typedef struct {
    /* 0x00 */ u32 start;
    /* 0x04 */ u32 loopEnd; // numSamples into the sample where the loop ends
    /* 0x08 */ u32 count; // The number of times the loop is played before the sound completes. Setting count to -1 indicates that the loop should play indefinitely.
    /* 0x0C */ u32 sampleEnd; // total number of s16-samples in the 
    /* 0x10 */ s16 predictorState[16]; // only exists if count != 0. 8-byte aligned
} AdpcmLoop; // size = 0x30 (or 0x10)

/**
 * The procedure used to design the codeBook is based on an adaptive clustering algorithm.
 * The size of the codeBook is (8 * order * numPredictors) and is 8-byte aligned
 */
typedef struct {
    /* 0x0 */ s32 order;
    /* 0x4 */ s32 numPredictors;
    /* 0x8 */ s16 codeBook[1]; // a table of prediction coefficients that the coder selects from to optimize sound quality.
} AdpcmBook; // size >= 0x8

typedef struct Sample {
    /* 0x00 */ u32 unk_0 : 1;
    /* 0x00 */ u32 codec : 3; // The state of compression or decompression
    /* 0x00 */ u32 medium : 2; // Medium where sample is currently stored
    /* 0x00 */ u32 unk_bit26 : 1;
    /* 0x00 */ u32 isRelocated : 1; // Has the sample header been relocated (offsets to pointers)
    /* 0x01 */ u32 size : 24; // Size of the sample
    /* 0x04 */ u8* sampleAddr; // Raw sample data. Offset from the start of the sample bank or absolute address to either rom or ram
    /* 0x08 */ AdpcmLoop* loop; // Adpcm loop parameters used by the sample. Offset from the start of the sound font / pointer to ram
    /* 0x0C */ AdpcmBook* book; // Adpcm book parameters used by the sample. Offset from the start of the sound font / pointer to ram
} Sample; // size = 0x10

typedef struct {
    /* 0x0 */ Sample* sample;
    /* 0x4 */ f32 tuning; // frequency scale factor
} TunedSample; // size = 0x8

/**
 * Stores an entry of decompressed samples in a reverb ring buffer.
 * By storing the sample in a ring buffer, the time it takes to loop
 * around back to the same sample acts as a delay, leading to an echo effect.
 */
typedef struct {
    /* 0x00 */ u8 isRelocated; // have the envelope and all samples been relocated (offsets to pointers)
    /* 0x01 */ u8 normalRangeLo;
    /* 0x02 */ u8 normalRangeHi;
    /* 0x03 */ u8 adsrDecayIndex; // index used to obtain adsr decay rate from adsrDecayTable
    /* 0x04 */ EnvelopePoint* envelope;
    /* 0x08 */ TunedSample lowPitchTunedSample;
    /* 0x10 */ TunedSample normalPitchTunedSample;
    /* 0x18 */ TunedSample highPitchTunedSample;
} Instrument; // size = 0x20

typedef struct {
    /* 0x00 */ u8 adsrDecayIndex; // index used to obtain adsr decay rate from adsrDecayTable
    /* 0x01 */ u8 pan;
    /* 0x02 */ u8 isRelocated; // have tunedSample.sample and envelope been relocated (offsets to pointers)
    /* 0x04 */ TunedSample tunedSample;
    /* 0x0C */ EnvelopePoint* envelope;
} Drum; // size = 0x10

typedef struct {
    /* 0x0 */ TunedSample tunedSample;
} SoundEffect; // size = 0x8

typedef struct {
    /* 0x00 */ u8 numInstruments;
    /* 0x01 */ u8 numDrums;
    /* 0x02 */ u8 sampleBankId1;
    /* 0x03 */ u8 sampleBankId2;
    /* 0x04 */ u16 numSfx;
    /* 0x08 */ Instrument** instruments;
    /* 0x0C */ Drum** drums;
    /* 0x10 */ SoundEffect* soundEffects;
} SoundFont; // size = 0x14

#endif
