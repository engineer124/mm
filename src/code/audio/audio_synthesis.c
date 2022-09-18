#include "global.h"

// Data Memory Addresses for the RSP
#define DMEM_TEMP 0x3B0
#define DMEM_TEMP2 0x3C0
#define DMEM_SURROUND_TEMP 0x4B0
#define DMEM_UNCOMPRESSED_NOTE 0x570
#define DMEM_HAAS_TEMP 0x5B0
#define DMEM_SCRATCH2 0x750              // = DMEM_TEMP + DMEM_2CH_SIZE + a bit more
#define DMEM_COMPRESSED_ADPCM_DATA 0x930 // = DMEM_LEFT_CH
#define DMEM_LEFT_CH 0x930
#define DMEM_RIGHT_CH 0xAD0
#define DMEM_WET_TEMP 0x3D0
#define DMEM_WET_SCRATCH 0x710 // = DMEM_WET_TEMP + DMEM_2CH_SIZE
#define DMEM_WET_LEFT_CH 0xC70
#define DMEM_WET_RIGHT_CH 0xE10 // = DMEM_WET_LEFT_CH + DMEM_1CH_SIZE

typedef enum {
    /* 0 */ HAAS_EFFECT_DELAY_NONE,
    /* 1 */ HAAS_EFFECT_DELAY_LEFT, // Delay left channel so that right channel is heard first
    /* 2 */ HAAS_EFFECT_DELAY_RIGHT // Delay right channel so that left channel is heard first
} HaasEffectDelaySide;

Acmd* AudioSynth_SaveResampledReverbSampleImpl(Acmd* cmd, u16 dmem, u16 arg2, uintptr_t arg3);
Acmd* AudioSynth_LoadReverbSampleImpl(Acmd* cmd, u16 dmem, u16 startPos, s32 size, SynthesisReverb* reverb);
Acmd* AudioSynth_SaveReverbSampleImpl(Acmd* cmd, u16 dmem, u16 startPos, s32 size, SynthesisReverb* reverb);
Acmd* AudioSynth_DoOneAudioUpdate(s16* aiBuf, s32 aiBufNumSamples, Acmd* cmd, s32 updateIndex);
Acmd* AudioSynth_ProcessNote(s32 noteIndex, NoteSampleState* freeSampleState, NoteSynthesisState* synthState,
                             s16* aiBuf, s32 aiBufNumSamples, Acmd* cmd, s32 updateIndex);
Acmd* AudioSynth_ApplySurroundEffect(Acmd* cmd, NoteSampleState* freeSampleState, NoteSynthesisState* synthState,
                                     s32 size, s32 dmem, s32 flags);
Acmd* AudioSynth_FinalResample(Acmd* cmd, NoteSynthesisState* synthState, s32 size, u16 pitch, u16 inpDmem,
                               s32 resampleFlags);
Acmd* AudioSynth_ProcessEnvelope(Acmd* cmd, NoteSampleState* freeSampleState, NoteSynthesisState* synthState,
                                 s32 aiBufNumSamples, u16 dmemSrc, s32 haasEffectDelaySide, s32 flags);
Acmd* AudioSynth_LoadWaveSamples(Acmd* cmd, NoteSampleState* freeSampleState, NoteSynthesisState* synthState,
                                 s32 numSamplesToLoad);
Acmd* AudioSynth_ApplyHaasEffect(Acmd* cmd, NoteSampleState* freeSampleState, NoteSynthesisState* synthState, s32 size,
                                 s32 flags, s32 haasEffectDelaySide);

s32 D_801D5FB0 = 0;

u32 sEnvMixerOp = _SHIFTL(A_ENVMIXER, 24, 8);

// Store the left dry channel in a temp space to be delayed to produce the haas effect
u32 sEnvMixerLeftHaasDmemDests =
    MK_AUDIO_CMD(DMEM_HAAS_TEMP >> 4, DMEM_RIGHT_CH >> 4, DMEM_WET_LEFT_CH >> 4, DMEM_WET_RIGHT_CH >> 4);

// Store the right dry channel in a temp space to be delayed to produce the haas effect
u32 sEnvMixerRightHaasDmemDests =
    MK_AUDIO_CMD(DMEM_LEFT_CH >> 4, DMEM_HAAS_TEMP >> 4, DMEM_WET_LEFT_CH >> 4, DMEM_WET_RIGHT_CH >> 4);

u32 sEnvMixerDefaultDmemDests =
    MK_AUDIO_CMD(DMEM_LEFT_CH >> 4, DMEM_RIGHT_CH >> 4, DMEM_WET_LEFT_CH >> 4, DMEM_WET_RIGHT_CH >> 4);

// Unused Data
u16 D_801D5FC4[] = {
    0x7FFF, 0xD001, 0x3FFF, 0xF001, 0x5FFF, 0x9001, 0x7FFF, 0x8001,
};

u8 sNumSamplesPerWavePeriod[] = {
    WAVE_SAMPLE_COUNT,     // 1st harmonic
    WAVE_SAMPLE_COUNT / 2, // 2nd harmonic
    WAVE_SAMPLE_COUNT / 4, // 4th harmonic
    WAVE_SAMPLE_COUNT / 8, // 8th harmonic
};

void AudioSynth_AddReverbSampleBufferEntry(s32 numSamples, s32 updateIndex, s32 reverbIndex) {
    SynthesisReverb* reverb;
    ReverbSampleBufferEntry* entry;
    s32 extraSamples;
    s32 numSamplesAfterDownsampling;
    s32 temp_t1;
    u16 temp_t2;
    s32 temp_t2_2;
    s32 temp_t3;
    s32 temp_t4;
    s32 phi_a2;
    s32 phi_t1;
    s32 nextReverbSubBufPos;

    reverb = &gAudioContext.synthesisReverbs[reverbIndex];
    entry = &reverb->bufEntry[reverb->curFrame][updateIndex];

    numSamplesAfterDownsampling = numSamples / gAudioContext.synthesisReverbs[reverbIndex].downsampleRate;

    if (gAudioContext.synthesisReverbs[reverbIndex].resampleEffectOn) {
        if (reverb->downsampleRate == 1) {
            phi_a2 = 0;
            phi_t1 = 0;

            numSamplesAfterDownsampling += reverb->resampleEffectExtraSamples;
            temp_t2 = numSamplesAfterDownsampling;
            entry->saveResampleNumSamples = numSamplesAfterDownsampling;
            entry->loadResamplePitch = (temp_t2 << 0xF) / numSamples;
            entry->saveResamplePitch = (numSamples << 0xF) / temp_t2;

            while (true) {
                temp_t2_2 = (entry->loadResamplePitch * numSamples * 2) + reverb->resampleEffectLoadUnk;
                temp_t4 = temp_t2_2 >> 0x10;

                if ((numSamplesAfterDownsampling != temp_t4) && (phi_a2 == 0)) {
                    entry->loadResamplePitch =
                        ((numSamplesAfterDownsampling << 0x10) - reverb->resampleEffectLoadUnk) / (numSamples * 2);
                    phi_a2++;
                } else {
                    phi_a2++;
                    if (numSamplesAfterDownsampling < temp_t4) {
                        entry->loadResamplePitch--;
                    } else if (temp_t4 < numSamplesAfterDownsampling) {
                        entry->loadResamplePitch++;
                    } else {
                        break;
                    }
                }
            }

            reverb->resampleEffectLoadUnk = temp_t2_2 & 0xFFFF;

            while (true) {
                temp_t2_2 =
                    (entry->saveResamplePitch * numSamplesAfterDownsampling * 2) + reverb->resampleEffectSaveUnk;
                temp_t4 = temp_t2_2 >> 0x10;

                if ((numSamples != temp_t4) && (phi_t1 == 0)) {
                    entry->saveResamplePitch =
                        ((numSamples << 0x10) - reverb->resampleEffectSaveUnk) / (numSamplesAfterDownsampling * 2);
                    phi_t1++;
                } else {
                    phi_t1++;
                    if (numSamples < temp_t4) {
                        entry->saveResamplePitch--;
                    } else if (temp_t4 < numSamples) {
                        entry->saveResamplePitch++;
                    } else {
                        break;
                    }
                }
            }

            reverb->resampleEffectSaveUnk = temp_t2_2 & 0xFFFF;
        }
    }

    extraSamples = (reverb->nextReverbBufPos + numSamplesAfterDownsampling) - reverb->delayNumSamples;

    temp_t1 = reverb->nextReverbBufPos;

    // Add a reverb entry
    if (extraSamples < 0) {
        entry->size = numSamplesAfterDownsampling * SAMPLE_SIZE;
        entry->wrappedSize = 0;
        entry->startPos = reverb->nextReverbBufPos;
        reverb->nextReverbBufPos += numSamplesAfterDownsampling;
    } else {
        // End of the buffer is reached. Loop back around
        entry->size = (numSamplesAfterDownsampling - extraSamples) * SAMPLE_SIZE;
        entry->wrappedSize = extraSamples * SAMPLE_SIZE;
        entry->startPos = reverb->nextReverbBufPos;
        reverb->nextReverbBufPos = extraSamples;
    }

    entry->numSamplesAfterDownsampling = numSamplesAfterDownsampling;
    entry->numSamples = numSamples;

    // Add a sub-reverb entry
    if (reverb->subDelay != 0) {
        nextReverbSubBufPos = reverb->subDelay + temp_t1;
        if (nextReverbSubBufPos >= reverb->delayNumSamples) {
            nextReverbSubBufPos -= reverb->delayNumSamples;
        }

        entry = &reverb->subBufEntry[reverb->curFrame][updateIndex];
        numSamplesAfterDownsampling = numSamples / reverb->downsampleRate;
        extraSamples = (nextReverbSubBufPos + numSamplesAfterDownsampling) - reverb->delayNumSamples;

        if (extraSamples < 0) {
            entry->size = numSamplesAfterDownsampling * SAMPLE_SIZE;
            entry->wrappedSize = 0;
            entry->startPos = nextReverbSubBufPos;
        } else {
            // End of the buffer is reached. Loop back around
            entry->size = (numSamplesAfterDownsampling - extraSamples) * SAMPLE_SIZE;
            entry->wrappedSize = extraSamples * SAMPLE_SIZE;
            entry->startPos = nextReverbSubBufPos;
        }

        entry->numSamplesAfterDownsampling = numSamplesAfterDownsampling;
        entry->numSamples = numSamples;
    }
}

void func_80187B64(s32 updateIndex) {
    NoteSampleState* noteSampleState;
    NoteSampleState* freeSampleState;
    s32 baseIndex;
    s32 i;

    baseIndex = gAudioContext.numNotes * updateIndex;
    for (i = 0; i < gAudioContext.numNotes; i++) {
        noteSampleState = &gAudioContext.notes[i].noteSampleState;
        freeSampleState = &gAudioContext.freeSampleStateList[baseIndex + i];
        if (noteSampleState->bitField0.enabled) {
            noteSampleState->bitField0.needsInit = false;
        } else {
            freeSampleState->bitField0.enabled = false;
        }

        noteSampleState->harmonicIndexCurAndPrev = 0;
    }
}

Acmd* AudioSynth_Update(Acmd* cmdStart, s32* numAbiCmds, s16* aiStart, s32 aiBufNumSamples) {
    s32 numSamples;
    s16* curAiBufPos;
    Acmd* curCmd;
    s32 i;
    s32 j;
    SynthesisReverb* reverb;

    curCmd = cmdStart;
    for (i = gAudioContext.audioBufferParameters.updatesPerFrame; i > 0; i--) {
        AudioSeq_ProcessSequences(i - 1);
        func_80187B64(gAudioContext.audioBufferParameters.updatesPerFrame - i);
    }

    curAiBufPos = aiStart;
    gAudioContext.curLoadedBook = NULL;

    for (i = gAudioContext.audioBufferParameters.updatesPerFrame; i > 0; i--) {
        if (i == 1) {
            numSamples = aiBufNumSamples;
        } else if ((aiBufNumSamples / i) >= gAudioContext.audioBufferParameters.samplesPerUpdateMax) {
            numSamples = gAudioContext.audioBufferParameters.samplesPerUpdateMax;
        } else if (gAudioContext.audioBufferParameters.samplesPerUpdateMin >= (aiBufNumSamples / i)) {
            numSamples = gAudioContext.audioBufferParameters.samplesPerUpdateMin;
        } else {
            numSamples = gAudioContext.audioBufferParameters.samplesPerUpdate;
        }

        for (j = 0; j < gAudioContext.numSynthesisReverbs; j++) {
            if (gAudioContext.synthesisReverbs[j].useReverb) {
                AudioSynth_AddReverbSampleBufferEntry(numSamples,
                                                      gAudioContext.audioBufferParameters.updatesPerFrame - i, j);
            }
        }

        curCmd = AudioSynth_DoOneAudioUpdate(curAiBufPos, numSamples, curCmd,
                                             gAudioContext.audioBufferParameters.updatesPerFrame - i);
        aiBufNumSamples -= numSamples;
        curAiBufPos += numSamples * SAMPLE_SIZE;
    }

    for (j = 0; j < gAudioContext.numSynthesisReverbs; j++) {
        if (gAudioContext.synthesisReverbs[j].framesToIgnore != 0) {
            gAudioContext.synthesisReverbs[j].framesToIgnore--;
        }
        gAudioContext.synthesisReverbs[j].curFrame ^= 1;
    }

    *numAbiCmds = curCmd - cmdStart;
    return curCmd;
}

void func_80187DE8(s32 updateIndex, s32 noteIndex) {
    NoteSampleState* freeSampleState;
    s32 i;

    for (i = updateIndex + 1; i < gAudioContext.audioBufferParameters.updatesPerFrame; i++) {
        freeSampleState = &gAudioContext.freeSampleStateList[(gAudioContext.numNotes * i) + noteIndex];
        if (!freeSampleState->bitField0.needsInit) {
            freeSampleState->bitField0.enabled = false;
        } else {
            break;
        }
    }
}

Acmd* AudioSynth_LoadReverbSamplesToTempDmem(Acmd* cmd, SynthesisReverb* reverb, s16 updateIndex) {
    ReverbSampleBufferEntry* entry = &reverb->bufEntry[reverb->curFrame][updateIndex];

    cmd = AudioSynth_LoadReverbSampleImpl(cmd, DMEM_WET_TEMP, entry->startPos, entry->size, reverb);
    if (entry->wrappedSize != 0) {
        // Ring buffer wrapped
        cmd = AudioSynth_LoadReverbSampleImpl(cmd, DMEM_WET_TEMP + entry->size, 0, entry->wrappedSize, reverb);
    }
    return cmd;
}

Acmd* AudioSynth_SaveReverbSamplesFromTempDmem(Acmd* cmd, SynthesisReverb* reverb, s16 updateIndex) {
    ReverbSampleBufferEntry* entry = &reverb->bufEntry[reverb->curFrame][updateIndex];

    cmd = AudioSynth_SaveReverbSampleImpl(cmd, DMEM_WET_TEMP, entry->startPos, entry->size, reverb);
    if (entry->wrappedSize != 0) {
        // Ring buffer wrapped
        cmd = AudioSynth_SaveReverbSampleImpl(cmd, DMEM_WET_TEMP + entry->size, 0, entry->wrappedSize, reverb);
    }
    return cmd;
}

void AudioSynth_Noop1(void) {
}

void AudioSynth_ClearBuffer(Acmd* cmd, s32 dmem, s32 size) {
    aClearBuffer(cmd, dmem, size);
}

void AudioSynth_Noop2(void) {
}

void AudioSynth_Noop3(void) {
}

void AudioSynth_Noop4(void) {
}

void AudioSynth_Mix(Acmd* cmd, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    aMix(cmd, arg1, arg2, arg3, arg4);
}

void AudioSynth_Noop5(void) {
}

void AudioSynth_Noop6(void) {
}

void AudioSynth_Noop7(void) {
}

void AudioSynth_SetBuffer(Acmd* cmd, s32 flags, s32 dmemIn, s32 dmemOut, s32 size) {
    aSetBuffer(cmd, flags, dmemIn, dmemOut, size);
}

void AudioSynth_Noop8(void) {
}

void AudioSynth_Noop9(void) {
}

// possible fake match?
void AudioSynth_DMemMove(Acmd* cmd, s32 dmemIn, s32 dmemOut, s32 size) {
    cmd->words.w0 = _SHIFTL(A_DMEMMOVE, 24, 8) | _SHIFTL(dmemIn, 0, 24);
    cmd->words.w1 = _SHIFTL(dmemOut, 16, 16) | _SHIFTL(size, 0, 16);
}

void AudioSynth_Noop10(void) {
}

void AudioSynth_Noop11(void) {
}

void AudioSynth_Noop12(void) {
}

void AudioSynth_Noop13(void) {
}

void AudioSynth_InterL(Acmd* cmd, s32 dmemIn, s32 dmemOut, s32 numSamples) {
    cmd->words.w0 = _SHIFTL(A_INTERL, 24, 8) | _SHIFTL(numSamples, 0, 16);
    cmd->words.w1 = _SHIFTL(dmemIn, 16, 16) | _SHIFTL(dmemOut, 0, 16);
}

void AudioSynth_EnvSetup1(Acmd* cmd, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    aEnvSetup1(cmd, arg1, arg2, arg3, arg4);
}

void AudioSynth_Noop14(void) {
}

void AudioSynth_LoadBuffer(Acmd* cmd, s32 dmemDest, s32 size, void* addrSrc) {
    aLoadBuffer(cmd, addrSrc, dmemDest, size);
}

void AudioSynth_SaveBuffer(Acmd* cmd, s32 dmemSrc, s32 size, void* addrDest) {
    aSaveBuffer(cmd, dmemSrc, addrDest, size);
}

void AudioSynth_EnvSetup2(Acmd* cmd, s32 volLeft, s32 volRight) {
    cmd->words.w0 = _SHIFTL(A_ENVSETUP2, 24, 8);
    cmd->words.w1 = _SHIFTL(volLeft, 16, 16) | _SHIFTL(volRight, 0, 16);
}

void AudioSynth_Noop15(void) {
}

void AudioSynth_Noop16(void) {
}

void AudioSynth_Noop17(void) {
}

void AudioSynth_S8Dec(Acmd* cmd, s32 flags, s16* state) {
    aS8Dec(cmd, flags, state);
}

void AudioSynth_HiLoGain(Acmd* cmd, s32 gain, s32 dmemIn, s32 dmemOut, s32 size) {
    cmd->words.w0 = _SHIFTL(A_HILOGAIN, 24, 8) | _SHIFTL(gain, 16, 8) | _SHIFTL(size, 0, 16);
    cmd->words.w1 = _SHIFTL(dmemIn, 16, 16) | _SHIFTL(dmemOut, 0, 16);
}

// Remnant of OoT
void AudioSynth_UnkCmd19(Acmd* cmd, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    cmd->words.w0 = _SHIFTL(A_SPNOOP, 24, 8) | _SHIFTL(arg4, 16, 8) | _SHIFTL(arg3, 0, 16);
    cmd->words.w1 = _SHIFTL(arg1, 16, 16) | _SHIFTL(arg2, 0, 16);
}

void AudioSynth_Noop18(void) {
}

void AudioSynth_Noop19(void) {
}

void AudioSynth_Noop20(void) {
}

void AudioSynth_Noop21(void) {
}

void AudioSynth_Noop22(void) {
}

void AudioSynth_Noop23(void) {
}

void AudioSynth_Noop24(void) {
}

void AudioSynth_Noop25(void) {
}

void AudioSynth_LoadFilterBuffer(Acmd* cmd, s32 flags, s32 buf, s32 addr) {
    aFilter(cmd, flags, buf, addr);
}

void AudioSynth_LoadFilterSize(Acmd* cmd, s32 size, s32 addr) {
    aFilter(cmd, 2, size, addr);
}

/**
 * Leak some audio from the left reverb channel into the right reverb channel and vice versa (pan)
 */
Acmd* AudioSynth_LeakReverb(Acmd* cmd, SynthesisReverb* reverb) {
    aDMEMMove(cmd++, DMEM_WET_LEFT_CH, DMEM_WET_SCRATCH, DMEM_1CH_SIZE);
    aMix(cmd++, DMEM_1CH_SIZE >> 4, reverb->leakRtl, DMEM_WET_RIGHT_CH, DMEM_WET_LEFT_CH);
    aMix(cmd++, DMEM_1CH_SIZE >> 4, reverb->leakLtr, DMEM_WET_SCRATCH, DMEM_WET_RIGHT_CH);

    return cmd;
}

Acmd* AudioSynth_LoadReverbSampleAndResampleWithDownsample(Acmd* cmd, s32 aiBufNumSamples, SynthesisReverb* reverb,
                                                           s16 updateIndex) {
    ReverbSampleBufferEntry* entry = &reverb->bufEntry[reverb->curFrame][updateIndex];
    s16 offsetSize = (entry->startPos & 7) * SAMPLE_SIZE;
    s16 wrappedOffsetSize = ALIGN16(offsetSize + entry->size);

    cmd =
        AudioSynth_LoadReverbSampleImpl(cmd, DMEM_WET_TEMP, entry->startPos - (offsetSize / 2), DMEM_1CH_SIZE, reverb);

    if (entry->wrappedSize != 0) {
        // Ring buffer wrapped
        cmd = AudioSynth_LoadReverbSampleImpl(cmd, DMEM_WET_TEMP + wrappedOffsetSize, 0,
                                              DMEM_1CH_SIZE - wrappedOffsetSize, reverb);
    }

    aSetBuffer(cmd++, 0, DMEM_WET_TEMP + offsetSize, DMEM_WET_LEFT_CH, aiBufNumSamples * SAMPLE_SIZE);
    aResample(cmd++, reverb->resampleFlags, reverb->downsamplePitch, reverb->leftLoadResampleBuf);
    aSetBuffer(cmd++, 0, DMEM_WET_TEMP + DMEM_1CH_SIZE + offsetSize, DMEM_WET_RIGHT_CH, aiBufNumSamples * SAMPLE_SIZE);
    aResample(cmd++, reverb->resampleFlags, reverb->downsamplePitch, reverb->rightLoadResampleBuf);

    return cmd;
}

Acmd* AudioSynth_SaveReverbSampleAndResampleWithoutDownsample(Acmd* cmd, SynthesisReverb* reverb, s16 updateIndex) {
    ReverbSampleBufferEntry* entry = &reverb->bufEntry[reverb->curFrame][updateIndex];
    s16 numSamples = entry->numSamples;
    u32 size = numSamples * SAMPLE_SIZE;

    // Left Resample
    aDMEMMove(cmd++, DMEM_WET_LEFT_CH, DMEM_WET_TEMP, size);
    aSetBuffer(cmd++, 0, DMEM_WET_TEMP, DMEM_WET_SCRATCH, entry->saveResampleNumSamples * SAMPLE_SIZE);
    aResample(cmd++, reverb->resampleFlags, entry->saveResamplePitch, reverb->leftSaveResampleBuf);

    cmd = AudioSynth_SaveResampledReverbSampleImpl(cmd, DMEM_WET_SCRATCH, entry->size,
                                                   &reverb->leftReverbSampleBuf[entry->startPos]);

    if (entry->wrappedSize != 0) {
        // Ring buffer wrapped
        cmd = AudioSynth_SaveResampledReverbSampleImpl(cmd, entry->size + DMEM_WET_SCRATCH, entry->wrappedSize,
                                                       reverb->leftReverbSampleBuf);
    }

    // Right Resample
    aDMEMMove(cmd++, DMEM_WET_RIGHT_CH, DMEM_WET_TEMP, size);
    aSetBuffer(cmd++, 0, DMEM_WET_TEMP, DMEM_WET_SCRATCH, entry->saveResampleNumSamples * SAMPLE_SIZE);
    aResample(cmd++, reverb->resampleFlags, entry->saveResamplePitch, reverb->rightSaveResampleBuf);

    cmd = AudioSynth_SaveResampledReverbSampleImpl(cmd, DMEM_WET_SCRATCH, entry->size,
                                                   &reverb->rightReverbSampleBuf[entry->startPos]);

    if (entry->wrappedSize != 0) {
        // Ring buffer wrapped
        cmd = AudioSynth_SaveResampledReverbSampleImpl(cmd, entry->size + DMEM_WET_SCRATCH, entry->wrappedSize,
                                                       reverb->rightReverbSampleBuf);
    }

    return cmd;
}

Acmd* AudioSynth_LoadReverbSampleAndResampleWithoutDownsample(Acmd* cmd, s32 aiBufNumSamples, SynthesisReverb* reverb,
                                                              s16 updateIndex) {
    ReverbSampleBufferEntry* entry = &reverb->bufEntry[reverb->curFrame][updateIndex];
    s16 offsetSize = (entry->startPos & 7) * SAMPLE_SIZE;
    s16 wrappedOffsetSize = ALIGN16(offsetSize + entry->size);

    cmd = AudioSynth_LoadReverbSampleImpl(cmd, DMEM_WET_TEMP, entry->startPos - (offsetSize / (s32)SAMPLE_SIZE),
                                          DMEM_1CH_SIZE, reverb);

    if (entry->wrappedSize != 0) {
        // Ring buffer wrapped
        cmd = AudioSynth_LoadReverbSampleImpl(cmd, wrappedOffsetSize + DMEM_WET_TEMP, 0,
                                              DMEM_1CH_SIZE - wrappedOffsetSize, reverb);
    }

    aSetBuffer(cmd++, 0, offsetSize + DMEM_WET_TEMP, DMEM_WET_LEFT_CH, aiBufNumSamples * SAMPLE_SIZE);
    aResample(cmd++, reverb->resampleFlags, entry->loadResamplePitch, reverb->leftLoadResampleBuf);
    aSetBuffer(cmd++, 0, offsetSize + DMEM_UNCOMPRESSED_NOTE, DMEM_WET_RIGHT_CH, aiBufNumSamples * SAMPLE_SIZE);
    aResample(cmd++, reverb->resampleFlags, entry->loadResamplePitch, reverb->rightLoadResampleBuf);

    return cmd;
}

/**
 * Apply a filter (convolution) to each reverb channel.
 */
Acmd* AudioSynth_FilterReverb(Acmd* cmd, s32 size, SynthesisReverb* reverb) {
    if (reverb->filterLeft != NULL) {
        aFilter(cmd++, 2, size, reverb->filterLeft);
        aFilter(cmd++, reverb->resampleFlags, DMEM_WET_LEFT_CH, reverb->filterLeftState);
    }

    if (reverb->filterRight != NULL) {
        aFilter(cmd++, 2, size, reverb->filterRight);
        aFilter(cmd++, reverb->resampleFlags, DMEM_WET_RIGHT_CH, reverb->filterRightState);
    }

    return cmd;
}

/**
 * Mix in reverb from a different reverb index
 */
Acmd* AudioSynth_MixOtherReverbIndex(Acmd* cmd, SynthesisReverb* reverb, s32 updateIndex) {
    SynthesisReverb* mixReverb;

    if (reverb->mixReverbIndex >= gAudioContext.numSynthesisReverbs) {
        return cmd;
    }

    mixReverb = &gAudioContext.synthesisReverbs[reverb->mixReverbIndex];
    if (mixReverb->downsampleRate == 1) {
        cmd = AudioSynth_LoadReverbSamplesToTempDmem(cmd, mixReverb, updateIndex);
        aMix(cmd++, DMEM_2CH_SIZE >> 4, reverb->mixReverbStrength, DMEM_WET_LEFT_CH, DMEM_WET_TEMP);
        cmd = AudioSynth_SaveReverbSamplesFromTempDmem(cmd, mixReverb, updateIndex);
    }

    return cmd;
}

Acmd* AudioSynth_LoadReverbSamplesFromWetDmem(Acmd* cmd, s32 aiBufNumSamples, SynthesisReverb* reverb,
                                              s16 updateIndex) {
    ReverbSampleBufferEntry* entry = &reverb->bufEntry[reverb->curFrame][updateIndex];

    cmd = AudioSynth_LoadReverbSampleImpl(cmd, DMEM_WET_LEFT_CH, entry->startPos, entry->size, reverb);
    if (entry->wrappedSize != 0) {
        // Ring buffer wrapped
        cmd = AudioSynth_LoadReverbSampleImpl(cmd, DMEM_WET_LEFT_CH + entry->size, 0, entry->wrappedSize, reverb);
    }

    return cmd;
}

Acmd* AudioSynth_LoadSubReverbSamplesFromWetDmem(Acmd* cmd, s32 aiBufNumSamples, SynthesisReverb* reverb,
                                                 s16 updateIndex) {
    ReverbSampleBufferEntry* entry = &reverb->subBufEntry[reverb->curFrame][updateIndex];

    cmd = AudioSynth_LoadReverbSampleImpl(cmd, DMEM_WET_LEFT_CH, entry->startPos, entry->size, reverb);
    if (entry->wrappedSize != 0) {
        // Ring buffer wrapped
        cmd = AudioSynth_LoadReverbSampleImpl(cmd, DMEM_WET_LEFT_CH + entry->size, 0, entry->wrappedSize, reverb);
    }

    return cmd;
}

Acmd* AudioSynth_SaveResampledReverbSampleImpl(Acmd* cmd, u16 dmem, u16 size, uintptr_t startAddr) {
    s32 startAddrAlignDropped;
    u32 endAddr;
    s32 endAddrAlignDropped;

    endAddr = startAddr + size;

    endAddrAlignDropped = endAddr & 0xF;
    if (endAddrAlignDropped != 0) {
        aLoadBuffer(cmd++, (endAddr - endAddrAlignDropped), DMEM_TEMP, 0x10);
        aDMEMMove(cmd++, dmem, DMEM_TEMP2, size);
        aDMEMMove(cmd++, DMEM_TEMP + endAddrAlignDropped, size + DMEM_TEMP2, 0x10 - endAddrAlignDropped);

        size += (0x10 - endAddrAlignDropped);
        dmem = DMEM_TEMP2;
    }

    startAddrAlignDropped = startAddr & 0xF;
    if (startAddrAlignDropped != 0) {
        aLoadBuffer(cmd++, startAddr - startAddrAlignDropped, DMEM_TEMP, 0x10);
        aDMEMMove(cmd++, dmem, startAddrAlignDropped + DMEM_TEMP, size);

        size += startAddrAlignDropped;
        dmem = DMEM_TEMP;
    }

    aSaveBuffer(cmd++, dmem, startAddr - startAddrAlignDropped, size);

    return cmd;
}

Acmd* AudioSynth_LoadReverbSampleImpl(Acmd* cmd, u16 dmem, u16 startPos, s32 size, SynthesisReverb* reverb) {
    aLoadBuffer(cmd++, &reverb->leftReverbSampleBuf[startPos], dmem, size);
    aLoadBuffer(cmd++, &reverb->rightReverbSampleBuf[startPos], dmem + DMEM_1CH_SIZE, size);

    return cmd;
}

Acmd* AudioSynth_SaveReverbSampleImpl(Acmd* cmd, u16 dmem, u16 startPos, s32 size, SynthesisReverb* reverb) {
    aSaveBuffer(cmd++, dmem, &reverb->leftReverbSampleBuf[startPos], size);
    aSaveBuffer(cmd++, dmem + DMEM_1CH_SIZE, &reverb->rightReverbSampleBuf[startPos], size);

    return cmd;
}

void AudioSynth_Noop26(void) {
}

Acmd* AudioSynth_LoadSubReverbSamplesFromWetDmemWithoutDownsample(Acmd* cmd, s32 aiBufNumSamples,
                                                                  SynthesisReverb* reverb, s16 updateIndex) {
    if (reverb->downsampleRate == 1) {
        cmd = AudioSynth_LoadSubReverbSamplesFromWetDmem(cmd, aiBufNumSamples, reverb, updateIndex);
    }

    return cmd;
}

Acmd* AudioSynth_LoadReverbSamples(Acmd* cmd, s32 aiBufNumSamples, SynthesisReverb* reverb, s16 updateIndex) {
    if (reverb->downsampleRate == 1) {
        if (reverb->resampleEffectOn) {
            cmd = AudioSynth_LoadReverbSampleAndResampleWithoutDownsample(cmd, aiBufNumSamples, reverb, updateIndex);
        } else {
            cmd = AudioSynth_LoadReverbSamplesFromWetDmem(cmd, aiBufNumSamples, reverb, updateIndex);
        }
    } else {
        cmd = AudioSynth_LoadReverbSampleAndResampleWithDownsample(cmd, aiBufNumSamples, reverb, updateIndex);
    }

    return cmd;
}

Acmd* AudioSynth_SaveReverbSamples(Acmd* cmd, SynthesisReverb* reverb, s16 updateIndex) {
    ReverbSampleBufferEntry* entry = &reverb->bufEntry[reverb->curFrame][updateIndex];
    s32 downsampleRate;
    s32 numSamples;

    if (reverb->downsampleRate == 1) {
        if (reverb->resampleEffectOn) {
            cmd = AudioSynth_SaveReverbSampleAndResampleWithoutDownsample(cmd, reverb, updateIndex);
        } else {
            // Put the oldest samples in the ring buffer into the wet channels
            cmd = AudioSynth_SaveReverbSampleImpl(cmd, DMEM_WET_LEFT_CH, entry->startPos, entry->size, reverb);
            if (entry->wrappedSize != 0) {
                // Ring buffer wrapped
                cmd =
                    AudioSynth_SaveReverbSampleImpl(cmd, DMEM_WET_LEFT_CH + entry->size, 0, entry->wrappedSize, reverb);
            }
        }
    } else {
        if (1) {}

        downsampleRate = reverb->downsampleRate;
        numSamples = 0xD0;
        while (downsampleRate >= 2) {
            aInterl(cmd++, DMEM_WET_LEFT_CH, DMEM_WET_LEFT_CH, numSamples);
            aInterl(cmd++, DMEM_WET_RIGHT_CH, DMEM_WET_RIGHT_CH, numSamples);
            downsampleRate >>= 1;
            numSamples >>= 1;
        }

        if (entry->size != 0) {
            cmd = AudioSynth_SaveResampledReverbSampleImpl(cmd, DMEM_WET_LEFT_CH, entry->size,
                                                           &reverb->leftReverbSampleBuf[entry->startPos]);
            cmd = AudioSynth_SaveResampledReverbSampleImpl(cmd, DMEM_WET_RIGHT_CH, entry->size,
                                                           &reverb->rightReverbSampleBuf[entry->startPos]);
        }

        if (entry->wrappedSize != 0) {
            cmd = AudioSynth_SaveResampledReverbSampleImpl(cmd, entry->size + DMEM_WET_LEFT_CH, entry->wrappedSize,
                                                           reverb->leftReverbSampleBuf);
            cmd = AudioSynth_SaveResampledReverbSampleImpl(cmd, entry->size + DMEM_WET_RIGHT_CH, entry->wrappedSize,
                                                           reverb->rightReverbSampleBuf);
        }
    }

    reverb->resampleFlags = 0;

    return cmd;
}

Acmd* AudioSynth_SaveReverbSamplesFromWetDmem(Acmd* cmd, SynthesisReverb* reverb, s16 updateIndex) {
    ReverbSampleBufferEntry* entry = &reverb->subBufEntry[reverb->curFrame][updateIndex];

    cmd = AudioSynth_SaveReverbSampleImpl(cmd, DMEM_WET_LEFT_CH, entry->startPos, entry->size, reverb);
    if (entry->wrappedSize != 0) {
        // Ring buffer wrapped
        cmd = AudioSynth_SaveReverbSampleImpl(cmd, DMEM_WET_LEFT_CH + entry->size, 0, entry->wrappedSize, reverb);
    }

    return cmd;
}

Acmd* AudioSynth_DoOneAudioUpdate(s16* aiBuf, s32 aiBufNumSamples, Acmd* cmd, s32 updateIndex) {
    s32 size;
    u8 noteIndices[0x58];
    s16 count;
    s16 reverbIndex;
    SynthesisReverb* reverb;
    s32 useReverb;
    s32 t;
    s32 i;
    NoteSampleState* freeSampleState;
    NoteSampleState* freeSampleState2;
    s32 subDelay;

    t = gAudioContext.numNotes * updateIndex;
    count = 0;
    if (gAudioContext.numSynthesisReverbs == 0) {
        for (i = 0; i < gAudioContext.numNotes; i++) {
            if (gAudioContext.freeSampleStateList[t + i].bitField0.enabled) {
                noteIndices[count++] = i;
            }
        }
    } else {
        for (reverbIndex = 0; reverbIndex < gAudioContext.numSynthesisReverbs; reverbIndex++) {
            for (i = 0; i < gAudioContext.numNotes; i++) {
                freeSampleState = &gAudioContext.freeSampleStateList[t + i];
                if (freeSampleState->bitField0.enabled && (freeSampleState->bitField1.reverbIndex == reverbIndex)) {
                    noteIndices[count++] = i;
                }
            }
        }

        for (i = 0; i < gAudioContext.numNotes; i++) {
            freeSampleState = &gAudioContext.freeSampleStateList[t + i];
            if (freeSampleState->bitField0.enabled &&
                (freeSampleState->bitField1.reverbIndex >= gAudioContext.numSynthesisReverbs)) {
                noteIndices[count++] = i;
            }
        }
    }

    aClearBuffer(cmd++, DMEM_LEFT_CH, DMEM_2CH_SIZE);

    i = 0;
    for (reverbIndex = 0; reverbIndex < gAudioContext.numSynthesisReverbs; reverbIndex++) {
        reverb = &gAudioContext.synthesisReverbs[reverbIndex];
        useReverb = reverb->useReverb;
        if (useReverb) {

            // Loads reverb samples from DRAM (ringBuffer) into DMEM (DMEM_WET_LEFT_CH)
            cmd = AudioSynth_LoadReverbSamples(cmd, aiBufNumSamples, reverb, updateIndex);

            // Mixes reverb sample into the main dry channel
            // reverb->volume is always set to 0x7FFF (audio spec), and DMEM_LEFT_CH is cleared before reverbs.
            // So this is essentially a DMEMmove from DMEM_WET_LEFT_CH to DMEM_LEFT_CH
            aMix(cmd++, DMEM_2CH_SIZE >> 4, reverb->volume, DMEM_WET_LEFT_CH, DMEM_LEFT_CH);

            subDelay = reverb->subDelay;
            if (subDelay != 0) {
                aDMEMMove(cmd++, DMEM_WET_LEFT_CH, DMEM_WET_TEMP, DMEM_2CH_SIZE);
            }

            // Decays reverb over time. The (+ 0x8000) here is -100%
            aMix(cmd++, DMEM_2CH_SIZE >> 4, reverb->decayRatio + 0x8000, DMEM_WET_LEFT_CH, DMEM_WET_LEFT_CH);

            if ((reverb->leakRtl != 0 || reverb->leakLtr != 0) && (gAudioContext.soundMode != SOUNDMODE_MONO)) {
                cmd = AudioSynth_LeakReverb(cmd, reverb);
            }

            if (subDelay != 0) {
                if (reverb->mixReverbIndex != -1) {
                    cmd = AudioSynth_MixOtherReverbIndex(cmd, reverb, updateIndex);
                }
                cmd = AudioSynth_SaveReverbSamples(cmd, reverb, updateIndex);
                cmd = AudioSynth_LoadSubReverbSamplesFromWetDmemWithoutDownsample(cmd, aiBufNumSamples, reverb,
                                                                                  updateIndex);
                aMix(cmd++, DMEM_2CH_SIZE >> 4, reverb->subVolume, DMEM_WET_TEMP, DMEM_WET_LEFT_CH);
            }
        }

        while (i < count) {
            freeSampleState2 = &gAudioContext.freeSampleStateList[noteIndices[i] + t];
            if (freeSampleState2->bitField1.reverbIndex == reverbIndex) {
                cmd = AudioSynth_ProcessNote(noteIndices[i], freeSampleState2,
                                             &gAudioContext.notes[noteIndices[i]].synthesisState, aiBuf,
                                             aiBufNumSamples, cmd, updateIndex);
            } else {
                break;
            }
            i++;
        }

        if (useReverb) {
            if ((reverb->filterLeft != NULL) || (reverb->filterRight != NULL)) {
                cmd = AudioSynth_FilterReverb(cmd, aiBufNumSamples * SAMPLE_SIZE, reverb);
            }

            // Saves the wet channel sample from DMEM (DMEM_WET_LEFT_CH) into (ringBuffer) DRAM for future use
            if (subDelay != 0) {
                cmd = AudioSynth_SaveReverbSamplesFromWetDmem(cmd, reverb, updateIndex);
            } else {
                if (reverb->mixReverbIndex != -1) {
                    cmd = AudioSynth_MixOtherReverbIndex(cmd, reverb, updateIndex);
                }
                cmd = AudioSynth_SaveReverbSamples(cmd, reverb, updateIndex);
            }
        }
    }

    while (i < count) {
        cmd = AudioSynth_ProcessNote(noteIndices[i], &gAudioContext.freeSampleStateList[t + noteIndices[i]],
                                     &gAudioContext.notes[noteIndices[i]].synthesisState, aiBuf, aiBufNumSamples, cmd,
                                     updateIndex);
        i++;
    }

    size = aiBufNumSamples * SAMPLE_SIZE;
    aInterleave(cmd++, DMEM_TEMP, DMEM_LEFT_CH, DMEM_RIGHT_CH, size);

    if (gCustomSynthFunction != NULL) {
        cmd = gCustomSynthFunction(cmd, 2 * size, updateIndex);
    }
    aSaveBuffer(cmd++, DMEM_TEMP, aiBuf, 2 * size);

    return cmd;
}

Acmd* AudioSynth_ProcessNote(s32 noteIndex, NoteSampleState* freeSampleState, NoteSynthesisState* synthState,
                             s16* aiBuf, s32 aiBufNumSamples, Acmd* cmd, s32 updateIndex) {
    s32 pad1[2];
    s32 phi_a3;
    Sample* sample;
    AdpcmLoop* loopInfo;
    s32 numSamplesUntilLoopEnd;
    s32 numSamplesInThisIteration;
    s32 noteFinished;
    s32 loopToPoint;
    s32 flags;
    u16 resamplingRateFixedPoint;
    s32 gain;
    s32 frameIndex;
    s32 skipBytes;
    void* combFilterState;
    s32 numSamplesToDecode;
    s32 numFirstFrameSamplesToIgnore;
    u32 sampleDataStartAddr;
    u32 samplesLenFixedPoint;
    s32 samplesLenAdjusted;
    s32 numSamplesProcessed;
    s32 loopEndPos;
    s32 numSamplesToProcess;
    s32 dmemUncompressedAddrOffset2;
    s32 pad2[3];
    s32 numSamplesInFirstFrame;
    s32 numTrailingSamplesToIgnore;
    s32 pad3[3];
    s32 frameSize;
    s32 numFramesToDecode;
    s32 skipInitialSamples;
    s32 sampleDataStartOffset;
    u8* sampleDataChunkAddr;
    s32 numParts;
    s32 curPart;
    s32 sampleDataChunkAlignPad;
    s32 haasEffectDelaySide;
    s32 resampledTempLen;
    u16 sampleDmemBeforeResampling;
    s32 sampleDataChunkOffset;
    s32 dmemAddr;
    s32 dmemUncompressedAddrOffset1;
    Note* note;
    u32 numSamplesToLoad;
    u16 combFilterSize;
    u16 combFilterGain;
    s16* filter;
    s32 bookOffset;
    s32 finished;
    s32 sampleDataChunkSize;
    s16 sampleDataDmemAddr;

    bookOffset = freeSampleState->bitField1.bookOffset;
    finished = freeSampleState->bitField0.finished;
    note = &gAudioContext.notes[noteIndex];
    flags = A_CONTINUE;

    if (freeSampleState->bitField0.needsInit == true) {
        flags = A_INIT;
        synthState->loopBitNeedsSet = false;
        synthState->loopBitLoopAtEnd = false;
        synthState->samplePosInt = note->playbackState.startSamplePos;
        synthState->samplePosFrac = 0;
        synthState->curVolLeft = 0;
        synthState->curVolRight = 0;
        synthState->prevHaasEffectLeftDelaySize = 0;
        synthState->prevHaasEffectRightDelaySize = 0;
        synthState->curReverbVol = freeSampleState->targetReverbVol;
        synthState->numParts = 0;
        synthState->combFilterNeedsInit = true;
        note->noteSampleState.bitField0.finished = false;
        synthState->unk_1F = note->playbackState.unk_7C[4];
        finished = false;
    }

    resamplingRateFixedPoint = freeSampleState->resamplingRateFixedPoint;
    numParts = freeSampleState->bitField1.hasTwoParts + 1;
    samplesLenFixedPoint = (resamplingRateFixedPoint * aiBufNumSamples * 2) + synthState->samplePosFrac;
    numSamplesToLoad = samplesLenFixedPoint >> 16;

    if (numSamplesToLoad == 0) {
        skipBytes = false;
    }

    synthState->samplePosFrac = samplesLenFixedPoint & 0xFFFF;

    // Partially-optimized out no-op ifs required for matching. SM64 decomp
    // makes it clear that this is how it should look.
    if ((synthState->numParts == 1) && (numParts == 2)) {
    } else if ((synthState->numParts == 2) && (numParts == 1)) {
    } else {
    }

    synthState->numParts = numParts;

    if (freeSampleState->bitField1.isSyntheticWave) {
        cmd = AudioSynth_LoadWaveSamples(cmd, freeSampleState, synthState, numSamplesToLoad);
        sampleDmemBeforeResampling = DMEM_UNCOMPRESSED_NOTE + (synthState->samplePosInt * 2);
        synthState->samplePosInt += numSamplesToLoad;
    } else {
        sample = freeSampleState->tunedSample->sample;
        loopInfo = sample->loop;

        if (note->playbackState.status != PLAYBACK_STATUS_0) {
            synthState->loopBitLoopAtEnd = true;
        }

        if ((loopInfo->count == 2) && synthState->loopBitLoopAtEnd) {
            loopEndPos = loopInfo->sampleEnd;
        } else {
            loopEndPos = loopInfo->loopEnd;
        }

        sampleDataStartAddr = sample->sampleAddr;
        resampledTempLen = 0;

        for (curPart = 0; curPart < numParts; curPart++) {
            numSamplesProcessed = 0;
            dmemUncompressedAddrOffset1 = 0;

            if (numParts == 1) {
                samplesLenAdjusted = numSamplesToLoad;
            } else if (numSamplesToLoad & 1) {
                samplesLenAdjusted = (numSamplesToLoad & ~1) + (curPart * 2);
            } else {
                samplesLenAdjusted = numSamplesToLoad;
            }

            if ((sample->codec == CODEC_ADPCM) || (sample->codec == CODEC_SMALL_ADPCM)) {
                if (gAudioContext.curLoadedBook != sample->book->book) {
                    u32 numEntries;

                    switch (bookOffset) {
                        case 1:
                            gAudioContext.curLoadedBook = &D_801D58A8[1];
                            break;

                        case 2:
                        case 3:
                        default:
                            gAudioContext.curLoadedBook = sample->book->book;
                            break;
                    }

                    numEntries = 16 * sample->book->order * sample->book->numPredictors;
                    aLoadADPCM(cmd++, numEntries, gAudioContext.curLoadedBook);
                }
            }

            while (numSamplesProcessed != samplesLenAdjusted) {
                noteFinished = false;
                loopToPoint = false;
                dmemUncompressedAddrOffset2 = 0;

                numFirstFrameSamplesToIgnore = synthState->samplePosInt & 0xF;
                numSamplesUntilLoopEnd = loopEndPos - synthState->samplePosInt;
                numSamplesToProcess = samplesLenAdjusted - numSamplesProcessed;

                if (numFirstFrameSamplesToIgnore == 0 && !synthState->loopBitNeedsSet) {
                    numFirstFrameSamplesToIgnore = SAMPLES_PER_FRAME;
                }
                numSamplesInFirstFrame = SAMPLES_PER_FRAME - numFirstFrameSamplesToIgnore;

                if (numSamplesToProcess < numSamplesUntilLoopEnd) {
                    numFramesToDecode =
                        (s32)(numSamplesToProcess - numSamplesInFirstFrame + SAMPLES_PER_FRAME - 1) / SAMPLES_PER_FRAME;
                    numSamplesToDecode = numFramesToDecode * SAMPLES_PER_FRAME;
                    numTrailingSamplesToIgnore = numSamplesInFirstFrame + numSamplesToDecode - numSamplesToProcess;
                } else {
                    numSamplesToDecode = numSamplesUntilLoopEnd - numSamplesInFirstFrame;
                    numTrailingSamplesToIgnore = 0;
                    if (numSamplesToDecode <= 0) {
                        numSamplesToDecode = 0;
                        numSamplesInFirstFrame = numSamplesUntilLoopEnd;
                    }
                    numFramesToDecode = (numSamplesToDecode + SAMPLES_PER_FRAME - 1) / SAMPLES_PER_FRAME;
                    if (loopInfo->count != 0) {
                        if ((loopInfo->count == 2) && synthState->loopBitLoopAtEnd) {
                            noteFinished = true;
                        } else {
                            // Loop around and restart
                            loopToPoint = true;
                        }
                    } else {
                        noteFinished = true;
                    }
                }

                // Set parameters based on compression type
                switch (sample->codec) {
                    // 16 2-byte samples (32 bytes) compressed into 4-bit samples (8 bytes) + 1 header byte
                    case CODEC_ADPCM:
                        frameSize = 9;
                        skipInitialSamples = SAMPLES_PER_FRAME;
                        sampleDataStartOffset = 0;
                        break;

                    case CODEC_SMALL_ADPCM:
                        // 16 2-byte samples (32 bytes) compressed into 2-bit samples (4 bytes) + 1 header byte
                        frameSize = 5;
                        skipInitialSamples = SAMPLES_PER_FRAME;
                        sampleDataStartOffset = 0;
                        break;

                    case CODEC_UNK7:
                        frameSize = 4;
                        skipInitialSamples = SAMPLES_PER_FRAME;
                        sampleDataStartOffset = 0;
                        break;

                    case CODEC_S8:
                        // 16 2-byte samples (32 bytes) compressed into 8-bit samples (16 bytes)
                        frameSize = 16;
                        skipInitialSamples = SAMPLES_PER_FRAME;
                        sampleDataStartOffset = 0;
                        break;

                    case CODEC_REVERB:
                        phi_a3 = -1;
                        if (gCustomReverbFunction != NULL) {
                            phi_a3 = gCustomReverbFunction(sample, samplesLenAdjusted, flags, noteIndex);
                        }

                        if (phi_a3 == -1) {
                            noteFinished = true;
                        } else if (phi_a3 == 0) {
                            return cmd;
                        } else {
                            AudioSynth_LoadBuffer(cmd++, DMEM_UNCOMPRESSED_NOTE,
                                                  (samplesLenAdjusted + SAMPLES_PER_FRAME) * SAMPLE_SIZE, phi_a3);
                            flags = A_CONTINUE;
                            skipBytes = 0;
                            numSamplesProcessed = samplesLenAdjusted;
                            dmemUncompressedAddrOffset1 = samplesLenAdjusted;
                        }

                        goto skip;

                    case CODEC_S16_INMEMORY:
                    case CODEC_UNK6:
                        AudioSynth_ClearBuffer(cmd++, DMEM_UNCOMPRESSED_NOTE,
                                               (samplesLenAdjusted + SAMPLES_PER_FRAME) * SAMPLE_SIZE);
                        flags = A_CONTINUE;
                        skipBytes = 0;
                        numSamplesProcessed = samplesLenAdjusted;
                        dmemUncompressedAddrOffset1 = samplesLenAdjusted;
                        goto skip;

                    case CODEC_S16:
                        AudioSynth_ClearBuffer(cmd++, DMEM_UNCOMPRESSED_NOTE,
                                               (samplesLenAdjusted + SAMPLES_PER_FRAME) * SAMPLE_SIZE);
                        flags = A_CONTINUE;
                        skipBytes = 0;
                        numSamplesProcessed = samplesLenAdjusted;
                        dmemUncompressedAddrOffset1 = samplesLenAdjusted;
                        goto skip;

                    default:
                        break;
                }

                // Move the compressed raw sample data into the rsp (DMEM)
                if (numFramesToDecode != 0) {
                    // Get the offset from the start of the sample to where the sample is currently playing from
                    frameIndex = (synthState->samplePosInt + skipInitialSamples - numFirstFrameSamplesToIgnore) /
                                 SAMPLES_PER_FRAME;
                    sampleDataChunkOffset = frameIndex * frameSize;

                    // Get the ram address of the requested sample chunk
                    if (sample->medium == MEDIUM_RAM) {
                        // Sample is already loaded into ram
                        sampleDataChunkAddr =
                            (u8*)(sampleDataStartOffset + sampleDataChunkOffset + sampleDataStartAddr);
                    } else if (gAudioContext.unk_29B8) { // always false
                        return cmd;
                    } else if (sample->medium == MEDIUM_UNK) {
                        // This medium is unsupported so terminate processing this note
                        return cmd;
                    } else {
                        // This medium is not in ram, so dma the requested sample into ram
                        sampleDataChunkAddr =
                            AudioLoad_DmaSampleData(sampleDataStartOffset + sampleDataChunkOffset + sampleDataStartAddr,
                                                    ALIGN16((numFramesToDecode * frameSize) + SAMPLES_PER_FRAME), flags,
                                                    &synthState->sampleDmaIndex, sample->medium);
                    }

                    if (sampleDataChunkAddr == NULL) {
                        // The ram address was unsuccesffully allocated
                        return cmd;
                    }

                    // Move the raw sample chunk from ram to the rsp
                    // DMEM at the addresses before DMEM_COMPRESSED_ADPCM_DATA
                    sampleDataChunkAlignPad = (u32)sampleDataChunkAddr & 0xF;
                    sampleDataChunkSize = ALIGN16((numFramesToDecode * frameSize) + SAMPLES_PER_FRAME);
                    sampleDataDmemAddr = DMEM_COMPRESSED_ADPCM_DATA - sampleDataChunkSize;
                    aLoadBuffer(cmd++, sampleDataChunkAddr - sampleDataChunkAlignPad, sampleDataDmemAddr,
                                sampleDataChunkSize);
                } else {
                    numSamplesToDecode = 0;
                    sampleDataChunkAlignPad = 0;
                }

                if (synthState->loopBitNeedsSet) {
                    aSetLoop(cmd++, sample->loop->predictorState);
                    flags = A_LOOP;
                    synthState->loopBitNeedsSet = false;
                }

                numSamplesInThisIteration = numSamplesToDecode + numSamplesInFirstFrame - numTrailingSamplesToIgnore;

                if (numSamplesProcessed == 0) {
                    if (1) {}
                    skipBytes = numFirstFrameSamplesToIgnore * SAMPLE_SIZE;
                } else {
                    dmemUncompressedAddrOffset2 = ALIGN16(dmemUncompressedAddrOffset1 + 8 * SAMPLE_SIZE);
                }

                // Decompress the raw sample chunks in the rsp
                // Goes from adpcm (compressed) sample data to pcm (uncompressed) sample data
                switch (sample->codec) {
                    case CODEC_ADPCM:
                        sampleDataChunkSize = ALIGN16((numFramesToDecode * frameSize) + SAMPLES_PER_FRAME);
                        sampleDataDmemAddr = DMEM_COMPRESSED_ADPCM_DATA - sampleDataChunkSize;
                        aSetBuffer(cmd++, 0, sampleDataDmemAddr + sampleDataChunkAlignPad,
                                   DMEM_UNCOMPRESSED_NOTE + dmemUncompressedAddrOffset2,
                                   numSamplesToDecode * SAMPLE_SIZE);
                        aADPCMdec(cmd++, flags, synthState->synthesisBuffers->adpcmState);
                        break;

                    case CODEC_SMALL_ADPCM:
                        sampleDataChunkSize = ALIGN16((numFramesToDecode * frameSize) + SAMPLES_PER_FRAME);
                        sampleDataDmemAddr = DMEM_COMPRESSED_ADPCM_DATA - sampleDataChunkSize;
                        aSetBuffer(cmd++, 0, sampleDataDmemAddr + sampleDataChunkAlignPad,
                                   DMEM_UNCOMPRESSED_NOTE + dmemUncompressedAddrOffset2,
                                   numSamplesToDecode * SAMPLE_SIZE);
                        aADPCMdec(cmd++, flags | 4, synthState->synthesisBuffers->adpcmState);
                        break;

                    case CODEC_S8:
                        sampleDataChunkSize = ALIGN16((numFramesToDecode * frameSize) + SAMPLES_PER_FRAME);
                        sampleDataDmemAddr = DMEM_COMPRESSED_ADPCM_DATA - sampleDataChunkSize;
                        AudioSynth_SetBuffer(cmd++, 0, sampleDataDmemAddr + sampleDataChunkAlignPad,
                                             DMEM_UNCOMPRESSED_NOTE + dmemUncompressedAddrOffset2,
                                             numSamplesToDecode * SAMPLE_SIZE);
                        AudioSynth_S8Dec(cmd++, flags, synthState->synthesisBuffers->adpcmState);
                        break;

                    case CODEC_UNK7:
                        break;
                }

                if (numSamplesProcessed != 0) {
                    aDMEMMove(cmd++,
                              DMEM_UNCOMPRESSED_NOTE + dmemUncompressedAddrOffset2 +
                                  (numFirstFrameSamplesToIgnore * SAMPLE_SIZE),
                              DMEM_UNCOMPRESSED_NOTE + dmemUncompressedAddrOffset1,
                              numSamplesInThisIteration * SAMPLE_SIZE);
                }

                numSamplesProcessed += numSamplesInThisIteration;

                switch (flags) {
                    case A_INIT:
                        skipBytes = SAMPLES_PER_FRAME * SAMPLE_SIZE;
                        dmemUncompressedAddrOffset1 = (numSamplesToDecode + SAMPLES_PER_FRAME) * SAMPLE_SIZE;
                        break;

                    case A_LOOP:
                        dmemUncompressedAddrOffset1 =
                            numSamplesInThisIteration * SAMPLE_SIZE + dmemUncompressedAddrOffset1;
                        break;

                    default:
                        if (dmemUncompressedAddrOffset1 != 0) {
                            dmemUncompressedAddrOffset1 =
                                numSamplesInThisIteration * SAMPLE_SIZE + dmemUncompressedAddrOffset1;
                        } else {
                            dmemUncompressedAddrOffset1 =
                                (numFirstFrameSamplesToIgnore + numSamplesInThisIteration) * SAMPLE_SIZE;
                        }
                        break;
                }

                flags = A_CONTINUE;

            skip:
                if (noteFinished) {
                    if ((samplesLenAdjusted - numSamplesProcessed) != 0) {
                        AudioSynth_ClearBuffer(cmd++, DMEM_UNCOMPRESSED_NOTE + dmemUncompressedAddrOffset1,
                                               (samplesLenAdjusted - numSamplesProcessed) * SAMPLE_SIZE);
                    }
                    finished = true;
                    note->noteSampleState.bitField0.finished = true;
                    func_80187DE8(updateIndex, noteIndex);
                    break; // break out of the for-loop
                } else if (loopToPoint) {
                    synthState->loopBitNeedsSet = true;
                    synthState->samplePosInt = loopInfo->start;
                } else {
                    synthState->samplePosInt += numSamplesToProcess;
                }
            }

            switch (numParts) {
                case 1:
                    sampleDmemBeforeResampling = DMEM_UNCOMPRESSED_NOTE + skipBytes;
                    break;

                case 2:
                    switch (curPart) {
                        case 0:
                            AudioSynth_InterL(cmd++, DMEM_UNCOMPRESSED_NOTE + skipBytes,
                                              DMEM_TEMP + (SAMPLES_PER_FRAME * SAMPLE_SIZE),
                                              ALIGN8(samplesLenAdjusted / 2));
                            resampledTempLen = samplesLenAdjusted;
                            sampleDmemBeforeResampling = DMEM_TEMP + (SAMPLES_PER_FRAME * SAMPLE_SIZE);
                            if (finished) {
                                AudioSynth_ClearBuffer(cmd++, sampleDmemBeforeResampling + resampledTempLen,
                                                       samplesLenAdjusted + SAMPLES_PER_FRAME);
                            }
                            break;

                        case 1:
                            AudioSynth_InterL(cmd++, DMEM_UNCOMPRESSED_NOTE + skipBytes,
                                              DMEM_TEMP + (SAMPLES_PER_FRAME * SAMPLE_SIZE) + resampledTempLen,
                                              ALIGN8(samplesLenAdjusted / 2));
                            break;
                    }
            }
            if (finished) {
                break;
            }
        }
    }

    flags = A_CONTINUE;
    if (freeSampleState->bitField0.needsInit == true) {
        freeSampleState->bitField0.needsInit = false;
        flags = A_INIT;
    }

    cmd = AudioSynth_FinalResample(cmd, synthState, aiBufNumSamples * SAMPLE_SIZE, resamplingRateFixedPoint,
                                   sampleDmemBeforeResampling, flags);
    if (bookOffset == 3) {
        AudioSynth_UnkCmd19(cmd++, DMEM_TEMP, DMEM_TEMP, aiBufNumSamples * (s32)SAMPLE_SIZE, 0);
    }

    gain = freeSampleState->gain;
    if (gain != 0) {
        // A gain of 0x10 (a UQ4.4 number) is equivalent to 1.0 and represents no volume change
        if (gain < 0x10) {
            gain = 0x10;
        }
        AudioSynth_HiLoGain(cmd++, gain, DMEM_TEMP, 0, (aiBufNumSamples + SAMPLES_PER_FRAME) * SAMPLE_SIZE);
    }

    filter = freeSampleState->filter;
    if (filter != 0) {
        AudioSynth_LoadFilterSize(cmd++, aiBufNumSamples * 2, filter);
        AudioSynth_LoadFilterBuffer(cmd++, flags, DMEM_TEMP, synthState->synthesisBuffers->filterState);
    }

    combFilterSize = freeSampleState->combFilterSize;
    combFilterGain = freeSampleState->combFilterGain;
    combFilterState = synthState->synthesisBuffers->combFilterState;
    if ((combFilterSize != 0) && (freeSampleState->combFilterGain != 0)) {
        AudioSynth_DMemMove(cmd++, DMEM_TEMP, DMEM_SCRATCH2, aiBufNumSamples * 2);
        dmemAddr = DMEM_SCRATCH2 - combFilterSize;
        if (synthState->combFilterNeedsInit) {
            AudioSynth_ClearBuffer(cmd++, dmemAddr, combFilterSize);
            synthState->combFilterNeedsInit = false;
        } else {
            AudioSynth_LoadBuffer(cmd++, dmemAddr, combFilterSize, combFilterState);
        }
        AudioSynth_SaveBuffer(cmd++, DMEM_TEMP + (aiBufNumSamples * 2) - combFilterSize, combFilterSize,
                              combFilterState);
        AudioSynth_Mix(cmd++, (aiBufNumSamples * 2) >> 4, combFilterGain, DMEM_SCRATCH2, dmemAddr);
        AudioSynth_DMemMove(cmd++, dmemAddr, DMEM_TEMP, aiBufNumSamples * 2);
    } else {
        synthState->combFilterNeedsInit = true;
    }

    if ((freeSampleState->haasEffectLeftDelaySize != 0) || (synthState->prevHaasEffectLeftDelaySize != 0)) {
        haasEffectDelaySide = HAAS_EFFECT_DELAY_LEFT;
    } else if ((freeSampleState->haasEffectRightDelaySize != 0) || (synthState->prevHaasEffectRightDelaySize != 0)) {
        haasEffectDelaySide = HAAS_EFFECT_DELAY_RIGHT;
    } else {
        haasEffectDelaySide = HAAS_EFFECT_DELAY_NONE;
    }

    if (gAudioContext.soundMode == SOUNDMODE_SURROUND) {
        freeSampleState->targetVolLeft = freeSampleState->targetVolLeft >> 1;
        freeSampleState->targetVolRight = freeSampleState->targetVolRight >> 1;
        if (freeSampleState->surroundEffectIndex != 0xFF) {
            cmd = AudioSynth_ApplySurroundEffect(cmd, freeSampleState, synthState, aiBufNumSamples, DMEM_TEMP, flags);
        }
    }

    cmd = AudioSynth_ProcessEnvelope(cmd, freeSampleState, synthState, aiBufNumSamples, DMEM_TEMP, haasEffectDelaySide,
                                     flags);

    if (freeSampleState->bitField1.useHaasEffect) {
        if (!(flags & A_INIT)) {
            flags = A_CONTINUE;
        }
        cmd = AudioSynth_ApplyHaasEffect(cmd, freeSampleState, synthState, aiBufNumSamples * 2, flags,
                                         haasEffectDelaySide);
    }

    return cmd;
}

Acmd* AudioSynth_ApplySurroundEffect(Acmd* cmd, NoteSampleState* freeSampleState, NoteSynthesisState* synthState,
                                     s32 aiBufNumSamples, s32 haasDmem, s32 flags) {
    s32 wetGain;
    u16 dryGain;
    s64 dmem = DMEM_SURROUND_TEMP;
    f32 decayGain;

    AudioSynth_DMemMove(cmd++, haasDmem, DMEM_HAAS_TEMP, aiBufNumSamples * 2);
    dryGain = synthState->surroundEffectGain;

    if (flags == 1) {
        aClearBuffer(cmd++, dmem, sizeof(synthState->synthesisBuffers->surroundEffectState));
        synthState->surroundEffectGain = 0;
    } else {
        aLoadBuffer(cmd++, synthState->synthesisBuffers->surroundEffectState, dmem,
                    sizeof(synthState->synthesisBuffers->surroundEffectState));
        aMix(cmd++, (aiBufNumSamples * (s32)SAMPLE_SIZE) >> 4, dryGain, dmem, DMEM_LEFT_CH);
        aMix(cmd++, (aiBufNumSamples * (s32)SAMPLE_SIZE) >> 4, (dryGain ^ 0xFFFF), dmem, DMEM_RIGHT_CH);

        wetGain = (dryGain * synthState->curReverbVol) >> 7;

        aMix(cmd++, (aiBufNumSamples * (s32)SAMPLE_SIZE) >> 4, wetGain, dmem, DMEM_WET_LEFT_CH);
        aMix(cmd++, (aiBufNumSamples * (s32)SAMPLE_SIZE) >> 4, (wetGain ^ 0xFFFF), dmem, DMEM_WET_RIGHT_CH);
    }

    aSaveBuffer(cmd++, DMEM_SURROUND_TEMP + (aiBufNumSamples * 2), synthState->synthesisBuffers->surroundEffectState,
                sizeof(synthState->synthesisBuffers->surroundEffectState));

    decayGain = (freeSampleState->targetVolLeft + freeSampleState->targetVolRight) * (1.0f / 0x2000);

    if (decayGain > 1.0f) {
        decayGain = 1.0f;
    }

    decayGain = decayGain * gDefaultPanVolume[127 - freeSampleState->surroundEffectIndex];
    synthState->surroundEffectGain = ((decayGain * 0x7FFF) + synthState->surroundEffectGain) / 2;

    AudioSynth_DMemMove(cmd++, DMEM_HAAS_TEMP, haasDmem, aiBufNumSamples * 2);

    return cmd;
}

Acmd* AudioSynth_FinalResample(Acmd* cmd, NoteSynthesisState* synthState, s32 size, u16 pitch, u16 inpDmem,
                               s32 resampleFlags) {
    if (pitch == 0) {
        AudioSynth_ClearBuffer(cmd++, DMEM_TEMP, size);
    } else {
        aSetBuffer(cmd++, 0, inpDmem, DMEM_TEMP, size);
        aResample(cmd++, resampleFlags, pitch, synthState->synthesisBuffers->finalResampleState);
    }

    return cmd;
}

Acmd* AudioSynth_ProcessEnvelope(Acmd* cmd, NoteSampleState* freeSampleState, NoteSynthesisState* synthState,
                                 s32 aiBufNumSamples, u16 dmemSrc, s32 haasEffectDelaySide, s32 flags) {
    u32 dmemDests;
    u16 curVolLeft;
    u16 targetVolLeft;
    s32 curReverbVol;
    u16 curVolRight;
    s16 targetReverbVol;
    s16 rampLeft;
    s16 rampRight;
    s16 rampReverb;
    s16 curReverbVolAndFlags;
    u16 targetVolRight;
    f32 temp_f0;
    s32 pad;

    targetReverbVol = freeSampleState->targetReverbVol;

    curVolLeft = synthState->curVolLeft;
    curVolRight = synthState->curVolRight;

    targetVolLeft = freeSampleState->targetVolLeft;
    targetVolLeft <<= 4;
    targetVolRight = freeSampleState->targetVolRight;
    targetVolRight <<= 4;

    if (gAudioContext.soundMode == SOUNDMODE_SURROUND) {
        if (freeSampleState->surroundEffectIndex != 0xFF) {
            temp_f0 = gDefaultPanVolume[freeSampleState->surroundEffectIndex];
            targetVolLeft *= temp_f0;
            targetVolRight *= temp_f0;
        }
    }

    if (targetVolLeft != curVolLeft) {
        rampLeft = (targetVolLeft - curVolLeft) / (aiBufNumSamples >> 3);
    } else {
        rampLeft = 0;
    }

    if (targetVolRight != curVolRight) {
        rampRight = (targetVolRight - curVolRight) / (aiBufNumSamples >> 3);
    } else {
        rampRight = 0;
    }

    curReverbVolAndFlags = synthState->curReverbVol;
    curReverbVol = curReverbVolAndFlags & 0x7F;

    if (curReverbVolAndFlags != targetReverbVol) {
        rampReverb = (((targetReverbVol & 0x7F) - curReverbVol) << 9) / (aiBufNumSamples >> 3);
        synthState->curReverbVol = targetReverbVol;
    } else {
        rampReverb = 0;
    }

    synthState->curVolLeft = curVolLeft + (rampLeft * (aiBufNumSamples >> 3));
    synthState->curVolRight = curVolRight + (rampRight * (aiBufNumSamples >> 3));

    if (freeSampleState->bitField1.useHaasEffect) {
        AudioSynth_ClearBuffer(cmd++, DMEM_HAAS_TEMP, DMEM_1CH_SIZE);
        AudioSynth_EnvSetup1(cmd++, curReverbVol * 2, rampReverb, rampLeft, rampRight);
        AudioSynth_EnvSetup2(cmd++, curVolLeft, curVolRight);

        switch (haasEffectDelaySide) {
            case HAAS_EFFECT_DELAY_LEFT:
                // Store the left dry channel in a temp space to be delayed to produce the haas effect
                dmemDests = sEnvMixerLeftHaasDmemDests;
                break;

            case HAAS_EFFECT_DELAY_RIGHT:
                // Store the right dry channel in a temp space to be delayed to produce the haas effect
                dmemDests = sEnvMixerRightHaasDmemDests;
                break;

            default: // HAAS_EFFECT_DELAY_NONE
                dmemDests = sEnvMixerDefaultDmemDests;
                break;
        }
    } else {
        aEnvSetup1(cmd++, curReverbVol * 2, rampReverb, rampLeft, rampRight);
        aEnvSetup2(cmd++, curVolLeft, curVolRight);
        dmemDests = sEnvMixerDefaultDmemDests;
    }

    aEnvMixer(cmd++, dmemSrc, aiBufNumSamples, (curReverbVolAndFlags & 0x80) >> 7,
              freeSampleState->bitField0.strongReverbRight, freeSampleState->bitField0.strongReverbLeft,
              freeSampleState->bitField0.strongRight, freeSampleState->bitField0.strongLeft, dmemDests, sEnvMixerOp);

    return cmd;
}

Acmd* AudioSynth_LoadWaveSamples(Acmd* cmd, NoteSampleState* freeSampleState, NoteSynthesisState* synthState,
                                 s32 numSamplesToLoad) {
    s32 numSamplesAvail;
    s32 harmonicIndexCurAndPrev = freeSampleState->harmonicIndexCurAndPrev;
    s32 samplePosInt = synthState->samplePosInt;
    s32 numDuplicates;

    if (freeSampleState->bitField1.bookOffset != 0) {
        // Move the noise wave (that reads compiled assembly as samples) from ram to dmem
        AudioSynth_LoadBuffer(cmd++, DMEM_UNCOMPRESSED_NOTE, ALIGN16(numSamplesToLoad * SAMPLE_SIZE), gWaveSamples[8]);
        // Offset the address for the samples read by gWaveSamples[8] to the next set of samples
        gWaveSamples[8] += numSamplesToLoad * SAMPLE_SIZE;

        return cmd;
    } else {
        // Move the synthetic wave from ram to dmem
        aLoadBuffer(cmd++, freeSampleState->waveSampleAddr, DMEM_UNCOMPRESSED_NOTE, WAVE_SAMPLE_COUNT * SAMPLE_SIZE);

        // If the harmonic changes, map the offset in the wave from one harmonic to another for continuity
        if (harmonicIndexCurAndPrev != 0) {
            samplePosInt = (samplePosInt * sNumSamplesPerWavePeriod[harmonicIndexCurAndPrev >> 2]) /
                           sNumSamplesPerWavePeriod[harmonicIndexCurAndPrev & 3];
        }

        // Offset in the WAVE_SAMPLE_COUNT samples of gWaveSamples to start processing the wave for continuity
        samplePosInt = (u32)samplePosInt % WAVE_SAMPLE_COUNT;
        // Number of samples in the initial WAVE_SAMPLE_COUNT samples available to be used to process
        numSamplesAvail = WAVE_SAMPLE_COUNT - samplePosInt;

        // Require duplicates if there are more samples to load than available
        if (numSamplesToLoad > numSamplesAvail) {
            // Duplicate (copy) the WAVE_SAMPLE_COUNT samples as many times as needed to reach numSamplesToLoad.
            // (numSamplesToLoad - numSamplesAvail) is the number of samples missing.
            // Divide by WAVE_SAMPLE_COUNT, rounding up, to get the amount of duplicates
            numDuplicates = ((numSamplesToLoad - numSamplesAvail + WAVE_SAMPLE_COUNT - 1) / WAVE_SAMPLE_COUNT);
            if (numDuplicates != 0) {
                aDuplicate(cmd++, numDuplicates, DMEM_UNCOMPRESSED_NOTE,
                           DMEM_UNCOMPRESSED_NOTE + (WAVE_SAMPLE_COUNT * SAMPLE_SIZE));
            }
        }
        synthState->samplePosInt = samplePosInt;
    }

    return cmd;
}

/**
 * The Haas Effect gives directionality to sound by appling a small (< 35ms) delay to either the left or right channel.
 * The delay is small enough that the sound is still perceived as one sound, but the channel that is not delayed will
 * reach our ear first and give a sense of directionality. The sound is directed towards the opposite side of the delay.
 */
Acmd* AudioSynth_ApplyHaasEffect(Acmd* cmd, NoteSampleState* freeSampleState, NoteSynthesisState* synthState, s32 size,
                                 s32 flags, s32 haasEffectDelaySide) {
    u16 dmemDest;
    u16 pitch;
    u8 prevHaasEffectDelaySize;
    u8 haasEffectDelaySize;

    switch (haasEffectDelaySide) {
        case HAAS_EFFECT_DELAY_LEFT:
            // Delay the sample on the left channel
            // This allows the right channel to be heard first
            dmemDest = DMEM_LEFT_CH;
            haasEffectDelaySize = freeSampleState->haasEffectLeftDelaySize;
            prevHaasEffectDelaySize = synthState->prevHaasEffectLeftDelaySize;
            synthState->prevHaasEffectRightDelaySize = 0;
            synthState->prevHaasEffectLeftDelaySize = haasEffectDelaySize;
            break;

        case HAAS_EFFECT_DELAY_RIGHT:
            // Delay the sample on the right channel
            // This allows the left channel to be heard first
            dmemDest = DMEM_RIGHT_CH;
            haasEffectDelaySize = freeSampleState->haasEffectRightDelaySize;
            prevHaasEffectDelaySize = synthState->prevHaasEffectRightDelaySize;
            synthState->prevHaasEffectRightDelaySize = haasEffectDelaySize;
            synthState->prevHaasEffectLeftDelaySize = 0;
            break;

        default: // HAAS_EFFECT_DELAY_NONE
            return cmd;
    }

    if (flags != A_INIT) {
        // Slightly adjust the sample rate in order to fit a change in sample delay
        if (haasEffectDelaySize != prevHaasEffectDelaySize) {
            pitch = (((size << 0xF) / 2) - 1) / ((size + haasEffectDelaySize - prevHaasEffectDelaySize - 2) / 2);
            aSetBuffer(cmd++, 0, DMEM_HAAS_TEMP, DMEM_TEMP, size + haasEffectDelaySize - prevHaasEffectDelaySize);
            aResampleZoh(cmd++, pitch, 0);
        } else {
            aDMEMMove(cmd++, DMEM_HAAS_TEMP, DMEM_TEMP, size);
        }

        if (prevHaasEffectDelaySize != 0) {
            aLoadBuffer(cmd++, synthState->synthesisBuffers->haasEffectDelayState, DMEM_HAAS_TEMP,
                        ALIGN16(prevHaasEffectDelaySize));
            aDMEMMove(cmd++, DMEM_TEMP, DMEM_HAAS_TEMP + prevHaasEffectDelaySize,
                      size + haasEffectDelaySize - prevHaasEffectDelaySize);
        } else {
            aDMEMMove(cmd++, DMEM_TEMP, DMEM_HAAS_TEMP, size + haasEffectDelaySize);
        }
    } else {
        // Just apply a delay directly
        aDMEMMove(cmd++, DMEM_HAAS_TEMP, DMEM_TEMP, size);
        if (haasEffectDelaySize) {
            aClearBuffer(cmd++, DMEM_HAAS_TEMP, haasEffectDelaySize);
        }
        aDMEMMove(cmd++, DMEM_TEMP, DMEM_HAAS_TEMP + haasEffectDelaySize, size);
    }

    if (haasEffectDelaySize) { // != 0
        // Save excessive samples for next iteration
        aSaveBuffer(cmd++, DMEM_HAAS_TEMP + size, synthState->synthesisBuffers->haasEffectDelayState,
                    ALIGN16(haasEffectDelaySize));
    }

    aAddMixer(cmd++, ALIGN64(size), DMEM_HAAS_TEMP, dmemDest, 0x7FFF);

    return cmd;
}
