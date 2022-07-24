#include "global.h"

// Data Memory Addresses for the RSP
#define DMEM_TEMP 0x3B0
#define DMEM_TEMP2 0x3C0
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

Acmd* func_80188AFC(Acmd* cmd, u16 dmem, u16 arg2, s32 arg3);
Acmd* AudioSynth_LoadRingBufferPart(Acmd* cmd, u16 dmem, u16 startPos, s32 size, SynthesisReverb* reverb);
Acmd* AudioSynth_SaveRingBufferPart(Acmd* cmd, u16 dmem, u16 startPos, s32 size, SynthesisReverb* reverb);
Acmd* AudioSynth_DoOneAudioUpdate(s16* aiBuf, s32 aiBufLen, Acmd* cmd, s32 updateIndex);
Acmd* AudioSynth_ProcessNote(s32 noteIndex, NoteSubEu* noteSubEu, NoteSynthesisState* synthState, s16* aiBuf,
                             s32 aiBufLen, Acmd* cmd, s32 updateIndex);
Acmd* AudioSynth_ApplySurroundEffect(Acmd* cmd, NoteSubEu* noteSubEu, NoteSynthesisState* synthState, s32 size,
                                     s32 dmem, s32 flags);
Acmd* AudioSynth_FinalResample(Acmd* cmd, NoteSynthesisState* synthState, s32 size, u16 pitch, u16 inpDmem,
                               s32 resampleFlags);
Acmd* AudioSynth_ProcessEnvelope(Acmd* cmd, NoteSubEu* noteSubEu, NoteSynthesisState* synthState, s32 aiBufLen,
                                 u16 dmemSrc, s32 haasEffectDelaySide, s32 flags);
Acmd* AudioSynth_LoadWaveSamples(Acmd* cmd, NoteSubEu* noteSubEu, NoteSynthesisState* synthState, s32 numSamplesToLoad);
Acmd* AudioSynth_ApplyHaasEffect(Acmd* cmd, NoteSubEu* noteSubEu, NoteSynthesisState* synthState, s32 size, s32 flags,
                                 s32 haasEffectDelaySide);

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

void AudioSynth_InitNextRingBuf(s32 chunkLen, s32 updateIndex, s32 reverbIndex) {
    SynthesisReverb* reverb;
    ReverbRingBufferItem* bufItem;
    s32 extraSamples;
    s32 numSamples;
    s32 temp_t1;
    u16 temp_t2;
    s32 temp_t2_2;
    s32 temp_t3;
    s32 temp_t4;
    s32 phi_a2;
    s32 phi_t1;
    s32 phi_a3;

    reverb = &gAudioContext.synthesisReverbs[reverbIndex];
    bufItem = &reverb->items[reverb->curFrame][updateIndex];

    numSamples = chunkLen / gAudioContext.synthesisReverbs[reverbIndex].downsampleRate;

    if (gAudioContext.synthesisReverbs[reverbIndex].unk_18 != 0) {
        if (reverb->downsampleRate == 1) {
            phi_a2 = 0;
            phi_t1 = 0;

            numSamples += reverb->unk_19;
            temp_t2 = numSamples;
            bufItem->unk_18 = numSamples;
            bufItem->unk_14 = (temp_t2 << 0xF) / chunkLen;
            bufItem->unk_16 = (chunkLen << 0xF) / temp_t2;

            while (true) {
                temp_t2_2 = (bufItem->unk_14 * chunkLen * 2) + reverb->unk_1A;
                temp_t4 = temp_t2_2 >> 0x10;

                if ((numSamples != temp_t4) && (phi_a2 == 0)) {
                    bufItem->unk_14 = ((numSamples << 0x10) - reverb->unk_1A) / (chunkLen * 2);
                    phi_a2++;
                } else {
                    phi_a2++;
                    if (numSamples < temp_t4) {
                        bufItem->unk_14--;
                    } else if (temp_t4 < numSamples) {
                        bufItem->unk_14++;
                    } else {
                        break;
                    }
                }
            }

            reverb->unk_1A = temp_t2_2 & 0xFFFF;

            while (true) {
                temp_t2_2 = (bufItem->unk_16 * numSamples * 2) + reverb->unk_1C;
                temp_t4 = temp_t2_2 >> 0x10;

                if ((chunkLen != temp_t4) && (phi_t1 == 0)) {
                    bufItem->unk_16 = ((chunkLen << 0x10) - reverb->unk_1C) / (numSamples * 2);
                    phi_t1++;
                } else {
                    phi_t1++;
                    if (chunkLen < temp_t4) {
                        bufItem->unk_16--;
                    } else if (temp_t4 < chunkLen) {
                        bufItem->unk_16++;
                    } else {
                        break;
                    }
                }
            }

            reverb->unk_1C = temp_t2_2 & 0xFFFF;
        }
    }

    extraSamples = (reverb->nextRingBufPos + numSamples) - reverb->windowSize;

    temp_t1 = reverb->nextRingBufPos;
    if (extraSamples < 0) {
        bufItem->size = numSamples * 2;
        bufItem->wrappedSize = 0;
        bufItem->startPos = reverb->nextRingBufPos;
        reverb->nextRingBufPos += numSamples;
    } else {
        // End of the buffer is reached. Loop back around
        bufItem->size = (numSamples - extraSamples) * 2;
        bufItem->wrappedSize = extraSamples * 2;
        bufItem->startPos = reverb->nextRingBufPos;
        reverb->nextRingBufPos = extraSamples;
    }

    bufItem->numSamplesAfterDownsampling = numSamples;
    bufItem->chunkLen = chunkLen;

    if (reverb->unk_14 != 0) {
        phi_a3 = reverb->unk_14 + temp_t1;
        if (phi_a3 >= reverb->windowSize) {
            phi_a3 -= reverb->windowSize;
        }

        bufItem = &reverb->items2[reverb->curFrame][updateIndex];
        numSamples = chunkLen / reverb->downsampleRate;
        extraSamples = (phi_a3 + numSamples) - reverb->windowSize;

        if (extraSamples < 0) {
            bufItem->size = numSamples * 2;
            bufItem->wrappedSize = 0;
            bufItem->startPos = phi_a3;
        } else {
            // End of the buffer is reached. Loop back around
            bufItem->size = (numSamples - extraSamples) * 2;
            bufItem->wrappedSize = extraSamples * 2;
            bufItem->startPos = phi_a3;
        }

        bufItem->numSamplesAfterDownsampling = numSamples;
        bufItem->chunkLen = chunkLen;
    }
}

void func_80187B64(s32 updateIndex) {
    NoteSubEu* subEu;
    NoteSubEu* subEu2;
    s32 baseIndex;
    s32 i;

    baseIndex = gAudioContext.numNotes * updateIndex;
    for (i = 0; i < gAudioContext.numNotes; i++) {
        subEu = &gAudioContext.notes[i].noteSubEu;
        subEu2 = &gAudioContext.noteSubsEu[baseIndex + i];
        if (subEu->bitField0.enabled) {
            subEu->bitField0.needsInit = false;
        } else {
            subEu2->bitField0.enabled = false;
        }

        subEu->harmonicIndexCurAndPrev = 0;
    }
}

Acmd* AudioSynth_Update(Acmd* cmdStart, s32* numAbiCmds, s16* aiStart, s32 aiBufLen) {
    s32 chunkLen;
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
            chunkLen = aiBufLen;
        } else if ((aiBufLen / i) >= gAudioContext.audioBufferParameters.samplesPerUpdateMax) {
            chunkLen = gAudioContext.audioBufferParameters.samplesPerUpdateMax;
        } else if (gAudioContext.audioBufferParameters.samplesPerUpdateMin >= (aiBufLen / i)) {
            chunkLen = gAudioContext.audioBufferParameters.samplesPerUpdateMin;
        } else {
            chunkLen = gAudioContext.audioBufferParameters.samplesPerUpdate;
        }

        for (j = 0; j < gAudioContext.numSynthesisReverbs; j++) {
            if (gAudioContext.synthesisReverbs[j].useReverb) {
                AudioSynth_InitNextRingBuf(chunkLen, gAudioContext.audioBufferParameters.updatesPerFrame - i, j);
            }
        }

        curCmd = AudioSynth_DoOneAudioUpdate(curAiBufPos, chunkLen, curCmd,
                                             gAudioContext.audioBufferParameters.updatesPerFrame - i);
        aiBufLen -= chunkLen;
        curAiBufPos += chunkLen * 2;
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
    NoteSubEu* noteSubEu;
    s32 i;

    for (i = updateIndex + 1; i < gAudioContext.audioBufferParameters.updatesPerFrame; i++) {
        noteSubEu = &gAudioContext.noteSubsEu[(gAudioContext.numNotes * i) + noteIndex];
        if (!noteSubEu->bitField0.needsInit) {
            noteSubEu->bitField0.enabled = false;
        } else {
            break;
        }
    }
}

Acmd* AudioSynth_LoadRingBuffer1AtTemp(Acmd* cmd, SynthesisReverb* reverb, s16 updateIndex) {
    ReverbRingBufferItem* bufItem = &reverb->items[reverb->curFrame][updateIndex];

    cmd = AudioSynth_LoadRingBufferPart(cmd, DMEM_WET_TEMP, bufItem->startPos, bufItem->size, reverb);
    if (bufItem->wrappedSize != 0) {
        // Ring buffer wrapped
        cmd = AudioSynth_LoadRingBufferPart(cmd, DMEM_WET_TEMP + bufItem->size, 0, bufItem->wrappedSize, reverb);
    }
    return cmd;
}

Acmd* AudioSynth_SaveRingBuffer1AtTemp(Acmd* cmd, SynthesisReverb* reverb, s16 updateIndex) {
    ReverbRingBufferItem* bufItem = &reverb->items[reverb->curFrame][updateIndex];

    cmd = AudioSynth_SaveRingBufferPart(cmd, DMEM_WET_TEMP, bufItem->startPos, bufItem->size, reverb);
    if (bufItem->wrappedSize != 0) {
        // Ring buffer wrapped
        cmd = AudioSynth_SaveRingBufferPart(cmd, DMEM_WET_TEMP + bufItem->size, 0, bufItem->wrappedSize, reverb);
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

// Related to OoT AudioSynth_UnkCmd19
void func_801881F8(Acmd* cmd, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    cmd->words.w0 = _SHIFTL(arg4, 16, 8) | _SHIFTL(arg3, 0, 16);
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

Acmd* func_80188304(Acmd* cmd, s32 aiBufLen, SynthesisReverb* reverb, s16 updateIndex) {
    ReverbRingBufferItem* bufItem = &reverb->items[reverb->curFrame][updateIndex];
    s16 offsetA = (bufItem->startPos & 7) * SAMPLE_SIZE;
    s16 offsetB = ALIGN16(offsetA + bufItem->size);

    cmd = AudioSynth_LoadRingBufferPart(cmd, DMEM_WET_TEMP, bufItem->startPos - (offsetA / 2), DMEM_1CH_SIZE, reverb);

    if (bufItem->wrappedSize != 0) {
        // Ring buffer wrapped
        cmd = AudioSynth_LoadRingBufferPart(cmd, DMEM_WET_TEMP + offsetB, 0, DMEM_1CH_SIZE - offsetB, reverb);
    }

    aSetBuffer(cmd++, 0, DMEM_WET_TEMP + offsetA, DMEM_WET_LEFT_CH, aiBufLen * SAMPLE_SIZE);
    aResample(cmd++, reverb->resampleFlags, reverb->unk_0E, reverb->unk_30);
    aSetBuffer(cmd++, 0, DMEM_WET_TEMP + DMEM_1CH_SIZE + offsetA, DMEM_WET_RIGHT_CH, aiBufLen * SAMPLE_SIZE);
    aResample(cmd++, reverb->resampleFlags, reverb->unk_0E, reverb->unk_34);

    return cmd;
}

// OoT func_800DB680
Acmd* func_801884A0(Acmd* cmd, SynthesisReverb* reverb, s16 updateIndex) {
    ReverbRingBufferItem* bufItem = &reverb->items[reverb->curFrame][updateIndex];
    s16 chunkLen = bufItem->chunkLen;
    u32 sp28 = chunkLen * 2;

    aDMEMMove(cmd++, DMEM_WET_LEFT_CH, DMEM_WET_TEMP, sp28);

    aSetBuffer(cmd++, 0, DMEM_WET_TEMP, DMEM_WET_SCRATCH, bufItem->unk_18 * SAMPLE_SIZE);
    aResample(cmd++, reverb->resampleFlags, bufItem->unk_16, reverb->unk_38);

    cmd = func_80188AFC(cmd, DMEM_WET_SCRATCH, bufItem->size, &reverb->leftRingBuf[bufItem->startPos]);

    if (bufItem->wrappedSize != 0) {
        // Ring buffer wrapped
        cmd = func_80188AFC(cmd, bufItem->size + DMEM_WET_SCRATCH, bufItem->wrappedSize, reverb->leftRingBuf);
    }

    aDMEMMove(cmd++, DMEM_WET_RIGHT_CH, DMEM_WET_TEMP, sp28);
    aSetBuffer(cmd++, 0, DMEM_WET_TEMP, DMEM_WET_SCRATCH, bufItem->unk_18 * 2);
    aResample(cmd++, reverb->resampleFlags, bufItem->unk_16, reverb->unk_3C);

    cmd = func_80188AFC(cmd, DMEM_WET_SCRATCH, bufItem->size, &reverb->rightRingBuf[bufItem->startPos]);

    if (bufItem->wrappedSize != 0) {
        // Ring buffer wrapped
        cmd = func_80188AFC(cmd, bufItem->size + DMEM_WET_SCRATCH, bufItem->wrappedSize, reverb->rightRingBuf);
    }

    return cmd;
}

void* func_80188698(Acmd* cmd, s32 aiBufLen, SynthesisReverb* reverb, s16 updateIndex) {
    ReverbRingBufferItem* bufItem = &reverb->items[reverb->curFrame][updateIndex];
    s16 offsetA = (bufItem->startPos & 7) * 2;
    s16 offsetB = ALIGN16(offsetA + bufItem->size);

    cmd = AudioSynth_LoadRingBufferPart(cmd, DMEM_WET_TEMP, bufItem->startPos - (offsetA / 2), DMEM_1CH_SIZE, reverb);

    if (bufItem->wrappedSize != 0) {
        // Ring buffer wrapped
        cmd = AudioSynth_LoadRingBufferPart(cmd, offsetB + DMEM_WET_TEMP, 0, DMEM_1CH_SIZE - offsetB, reverb);
    }

    aSetBuffer(cmd++, 0, offsetA + DMEM_WET_TEMP, DMEM_WET_LEFT_CH, aiBufLen * 2);
    aResample(cmd++, reverb->resampleFlags, bufItem->unk_14, reverb->unk_30);
    aSetBuffer(cmd++, 0, offsetA + DMEM_UNCOMPRESSED_NOTE, DMEM_WET_RIGHT_CH, aiBufLen * 2);
    aResample(cmd++, reverb->resampleFlags, bufItem->unk_14, reverb->unk_34);

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

Acmd* AudioSynth_MaybeMixRingBuffer1(Acmd* cmd, SynthesisReverb* reverb, s32 updateIndex) {
    SynthesisReverb* reverb2;

    if (reverb->unk_05 >= gAudioContext.numSynthesisReverbs) {
        return cmd;
    }

    reverb2 = &gAudioContext.synthesisReverbs[reverb->unk_05];
    if (reverb2->downsampleRate == 1) {
        cmd = AudioSynth_LoadRingBuffer1AtTemp(cmd, reverb2, updateIndex);
        aMix(cmd++, DMEM_2CH_SIZE >> 4, reverb->unk_08, DMEM_WET_LEFT_CH, DMEM_WET_TEMP);
        cmd = AudioSynth_SaveRingBuffer1AtTemp(cmd, reverb2, updateIndex);
    }

    return cmd;
}

Acmd* AudioSynth_LoadRingBuffer1(Acmd* cmd, s32 aiBufLen, SynthesisReverb* reverb, s16 updateIndex) {
    ReverbRingBufferItem* bufItem = &reverb->items[reverb->curFrame][updateIndex];

    cmd = AudioSynth_LoadRingBufferPart(cmd, DMEM_WET_LEFT_CH, bufItem->startPos, bufItem->size, reverb);
    if (bufItem->wrappedSize != 0) {
        // Ring buffer wrapped
        cmd = AudioSynth_LoadRingBufferPart(cmd, DMEM_WET_LEFT_CH + bufItem->size, 0, bufItem->wrappedSize, reverb);
    }

    return cmd;
}

Acmd* AudioSynth_LoadRingBuffer2(Acmd* cmd, s32 aiBufLen, SynthesisReverb* reverb, s16 updateIndex) {
    ReverbRingBufferItem* bufItem = &reverb->items2[reverb->curFrame][updateIndex];

    cmd = AudioSynth_LoadRingBufferPart(cmd, DMEM_WET_LEFT_CH, bufItem->startPos, bufItem->size, reverb);
    if (bufItem->wrappedSize != 0) {
        // Ring buffer wrapped
        cmd = AudioSynth_LoadRingBufferPart(cmd, DMEM_WET_LEFT_CH + bufItem->size, 0, bufItem->wrappedSize, reverb);
    }
    return cmd;
}

Acmd* func_80188AFC(Acmd* cmd, u16 dmem, u16 arg2, s32 arg3) {
    s32 temp_t0;
    s32 temp_v0;
    s32 temp_t2;

    temp_v0 = arg2 + arg3;
    temp_t2 = temp_v0 & 0xF;

    if (temp_t2 != 0) {
        aLoadBuffer(cmd++, (temp_v0 - temp_t2), DMEM_TEMP, 0x10);
        aDMEMMove(cmd++, dmem, DMEM_TEMP2, arg2);
        aDMEMMove(cmd++, temp_t2 + DMEM_TEMP, arg2 + DMEM_TEMP2, 0x10 - temp_t2);

        arg2 += (0x10 - temp_t2);
        dmem = DMEM_TEMP2;
    }

    temp_t0 = arg3 & 0xF;

    if (temp_t0 != 0) {
        aLoadBuffer(cmd++, arg3 - temp_t0, DMEM_TEMP, 0x10);
        aDMEMMove(cmd++, dmem, temp_t0 + DMEM_TEMP, arg2);

        arg2 += temp_t0;
        dmem = DMEM_TEMP;
    }

    aSaveBuffer(cmd++, dmem, (arg3 - temp_t0), arg2);

    return cmd;
}

Acmd* AudioSynth_LoadRingBufferPart(Acmd* cmd, u16 dmem, u16 startPos, s32 size, SynthesisReverb* reverb) {
    aLoadBuffer(cmd++, &reverb->leftRingBuf[startPos], dmem, size);
    aLoadBuffer(cmd++, &reverb->rightRingBuf[startPos], dmem + DMEM_1CH_SIZE, size);
    return cmd;
}

Acmd* AudioSynth_SaveRingBufferPart(Acmd* cmd, u16 dmem, u16 startPos, s32 size, SynthesisReverb* reverb) {
    aSaveBuffer(cmd++, dmem, &reverb->leftRingBuf[startPos], size);
    aSaveBuffer(cmd++, dmem + DMEM_1CH_SIZE, &reverb->rightRingBuf[startPos], size);
    return cmd;
}

// OoT AudioSynth_SaveBufferOffset?
void AudioSynth_Noop26(void) {
}

Acmd* AudioSynth_MaybeLoadRingBuffer2(Acmd* cmd, s32 aiBufLen, SynthesisReverb* reverb, s16 updateIndex) {
    if (reverb->downsampleRate == 1) {
        cmd = AudioSynth_LoadRingBuffer2(cmd, aiBufLen, reverb, updateIndex);
    }

    return cmd;
}

Acmd* AudioSynth_LoadReverbSamples(Acmd* cmd, s32 aiBufLen, SynthesisReverb* reverb, s16 updateIndex) {
    // Sets DMEM_WET_{LEFT,RIGHT}_CH, clobbers DMEM_TEMP
    if (reverb->downsampleRate == 1) {
        if (reverb->unk_18 != 0) {
            cmd = func_80188698(cmd, aiBufLen, reverb, updateIndex);
        } else {
            cmd = AudioSynth_LoadRingBuffer1(cmd, aiBufLen, reverb, updateIndex);
        }
    } else {
        cmd = func_80188304(cmd, aiBufLen, reverb, updateIndex);
    }
    return cmd;
}

Acmd* AudioSynth_SaveReverbSamples(Acmd* cmd, SynthesisReverb* reverb, s16 updateIndex) {
    ReverbRingBufferItem* bufItem = &reverb->items[reverb->curFrame][updateIndex];
    s32 i;
    s32 phi_a2;

    if (reverb->downsampleRate == 1) {
        if (reverb->unk_18 != 0) {
            cmd = func_801884A0(cmd, reverb, updateIndex);
        } else {
            // Put the oldest samples in the ring buffer into the wet channels
            cmd = AudioSynth_SaveRingBufferPart(cmd, DMEM_WET_LEFT_CH, bufItem->startPos, bufItem->size, reverb);
            if (bufItem->wrappedSize != 0) {
                // Ring buffer wrapped
                cmd = AudioSynth_SaveRingBufferPart(cmd, DMEM_WET_LEFT_CH + bufItem->size, 0, bufItem->wrappedSize,
                                                    reverb);
            }
        }
    } else {
        if (1) {}

        i = reverb->downsampleRate;
        phi_a2 = 0xD0;
        for (; i >= 2;) {
            aInterl(cmd++, DMEM_WET_LEFT_CH, DMEM_WET_LEFT_CH, phi_a2);
            aInterl(cmd++, DMEM_WET_RIGHT_CH, DMEM_WET_RIGHT_CH, phi_a2);
            i >>= 1;
            phi_a2 >>= 1;
        }

        if (bufItem->size != 0) {
            cmd = func_80188AFC(cmd, DMEM_WET_LEFT_CH, bufItem->size, &reverb->leftRingBuf[bufItem->startPos]);
            cmd = func_80188AFC(cmd, DMEM_WET_RIGHT_CH, bufItem->size, &reverb->rightRingBuf[bufItem->startPos]);
        }

        if (bufItem->wrappedSize != 0) {
            cmd = func_80188AFC(cmd, bufItem->size + DMEM_WET_LEFT_CH, bufItem->wrappedSize, reverb->leftRingBuf);
            cmd = func_80188AFC(cmd, bufItem->size + DMEM_WET_RIGHT_CH, bufItem->wrappedSize, reverb->rightRingBuf);
        }
    }

    reverb->resampleFlags = 0;
    return cmd;
}

Acmd* AudioSynth_SaveRingBuffer2(Acmd* cmd, SynthesisReverb* reverb, s16 updateIndex) {
    ReverbRingBufferItem* bufItem = &reverb->items2[reverb->curFrame][updateIndex];

    cmd = AudioSynth_SaveRingBufferPart(cmd, DMEM_WET_LEFT_CH, bufItem->startPos, bufItem->size, reverb);
    if (bufItem->wrappedSize != 0) {
        // Ring buffer wrapped
        cmd = AudioSynth_SaveRingBufferPart(cmd, DMEM_WET_LEFT_CH + bufItem->size, 0, bufItem->wrappedSize, reverb);
    }
    return cmd;
}

Acmd* AudioSynth_DoOneAudioUpdate(s16* aiBuf, s32 aiBufLen, Acmd* cmd, s32 updateIndex) {
    u8 noteIndices[0x5C];
    s16 count;
    s16 reverbIndex;
    SynthesisReverb* reverb;
    s32 useReverb;
    s32 t;
    s32 i;
    NoteSubEu* noteSubEu;
    NoteSubEu* noteSubEu2;
    s32 unk14;

    t = gAudioContext.numNotes * updateIndex;
    count = 0;
    if (gAudioContext.numSynthesisReverbs == 0) {
        for (i = 0; i < gAudioContext.numNotes; i++) {
            if (gAudioContext.noteSubsEu[t + i].bitField0.enabled) {
                noteIndices[count++] = i;
            }
        }
    } else {
        for (reverbIndex = 0; reverbIndex < gAudioContext.numSynthesisReverbs; reverbIndex++) {
            for (i = 0; i < gAudioContext.numNotes; i++) {
                noteSubEu = &gAudioContext.noteSubsEu[t + i];
                if (noteSubEu->bitField0.enabled && noteSubEu->bitField1.reverbIndex == reverbIndex) {
                    noteIndices[count++] = i;
                }
            }
        }

        for (i = 0; i < gAudioContext.numNotes; i++) {
            noteSubEu = &gAudioContext.noteSubsEu[t + i];
            if (noteSubEu->bitField0.enabled && noteSubEu->bitField1.reverbIndex >= gAudioContext.numSynthesisReverbs) {
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
            cmd = AudioSynth_LoadReverbSamples(cmd, aiBufLen, reverb, updateIndex);

            // Mixes reverb sample into the main dry channel
            // reverb->volume is always set to 0x7FFF (audio spec), and DMEM_LEFT_CH is cleared before reverbs.
            // So this is essentially a DMEMmove from DMEM_WET_LEFT_CH to DMEM_LEFT_CH
            aMix(cmd++, DMEM_2CH_SIZE >> 4, reverb->volume, DMEM_WET_LEFT_CH, DMEM_LEFT_CH);

            unk14 = reverb->unk_14;
            if (unk14) {
                aDMEMMove(cmd++, DMEM_WET_LEFT_CH, DMEM_WET_TEMP, DMEM_2CH_SIZE);
            }

            // Decays reverb over time. The (+ 0x8000) here is -100%
            aMix(cmd++, DMEM_2CH_SIZE >> 4, reverb->decayRate + 0x8000, DMEM_WET_LEFT_CH, DMEM_WET_LEFT_CH);

            if ((reverb->leakRtl != 0 || reverb->leakLtr != 0) && (gAudioContext.soundMode != AUDIO_MODE_MONO)) {
                cmd = AudioSynth_LeakReverb(cmd, reverb);
            }

            if (unk14) {
                if (reverb->unk_05 != -1) {
                    cmd = AudioSynth_MaybeMixRingBuffer1(cmd, reverb, updateIndex);
                }
                cmd = AudioSynth_SaveReverbSamples(cmd, reverb, updateIndex);
                cmd = AudioSynth_MaybeLoadRingBuffer2(cmd, aiBufLen, reverb, updateIndex);
                aMix(cmd++, DMEM_2CH_SIZE >> 4, reverb->unk_16, DMEM_WET_TEMP, DMEM_WET_LEFT_CH);
            }
        }

        while (i < count) {
            noteSubEu2 = &gAudioContext.noteSubsEu[noteIndices[i] + t];
            if (noteSubEu2->bitField1.reverbIndex == reverbIndex) {
                cmd = AudioSynth_ProcessNote(noteIndices[i], noteSubEu2,
                                             &gAudioContext.notes[noteIndices[i]].synthesisState, aiBuf, aiBufLen, cmd,
                                             updateIndex);
            } else {
                break;
            }
            i++;
        }

        if (useReverb) {
            if ((reverb->filterLeft != NULL) || (reverb->filterRight != NULL)) {
                cmd = AudioSynth_FilterReverb(cmd, aiBufLen * 2, reverb);
            }

            // Saves the wet channel sample from DMEM (DMEM_WET_LEFT_CH) into (ringBuffer) DRAM for future use
            if (unk14) {
                cmd = AudioSynth_SaveRingBuffer2(cmd, reverb, updateIndex);
            } else {
                if (reverb->unk_05 != -1) {
                    cmd = AudioSynth_MaybeMixRingBuffer1(cmd, reverb, updateIndex);
                }
                cmd = AudioSynth_SaveReverbSamples(cmd, reverb, updateIndex);
            }
        }
    }

    while (i < count) {
        cmd = AudioSynth_ProcessNote(noteIndices[i], &gAudioContext.noteSubsEu[t + noteIndices[i]],
                                     &gAudioContext.notes[noteIndices[i]].synthesisState, aiBuf, aiBufLen, cmd,
                                     updateIndex);
        i++;
    }

    unk14 = aiBufLen * 2;
    aInterleave(cmd++, DMEM_TEMP, DMEM_LEFT_CH, DMEM_RIGHT_CH, unk14);

    if (D_80208E74 != NULL) {
        cmd = D_80208E74(cmd, unk14 * 2, updateIndex);
    }
    aSaveBuffer(cmd++, DMEM_TEMP, aiBuf, unk14 * 2);

    return cmd;
}

Acmd* AudioSynth_ProcessNote(s32 noteIndex, NoteSubEu* noteSubEu, NoteSynthesisState* synthState, s16* aiBuf,
                             s32 aiBufLen, Acmd* cmd, s32 updateIndex) {
    s32 pad1[2];
    s32 phi_a3;
    Sample* sample;
    AdpcmLoop* loopInfo;
    s32 nSamplesUntilLoopEnd;
    s32 nSamplesInThisIteration;
    s32 noteFinished;
    s32 restart;
    s32 flags;
    u16 resamplingRateFixedPoint;
    s32 gain;
    s32 frameIndex;
    s32 skipBytes;
    s32 temp_v1_6;
    void* combFilterState;
    s32 nSamplesToDecode;
    u32 sampleDataStartAddr;
    u32 samplesLenFixedPoint;
    s32 samplesLenAdjusted;
    s32 nSamplesProcessed;
    s32 loopEndPos;
    s32 nSamplesToProcess;
    s32 dmemUncompressedAddrOffset2;
    s32 nFirstFrameSamplesToIgnore;
    s32 pad2[2];
    s32 nSamplesInFirstFrame;
    s32 nTrailingSamplesToIgnore;
    s32 new_var;
    s32 pad3;
    u32 new_var2;
    s32 frameSize;
    s32 nFramesToDecode;
    s32 skipInitialSamples;
    s32 sampleDataStartOffset;
    u8* sampleDataChunkAddr;
    s32 nParts;
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

    bookOffset = noteSubEu->bitField1.bookOffset;
    finished = noteSubEu->bitField0.finished;
    note = &gAudioContext.notes[noteIndex];
    flags = A_CONTINUE;

    if (noteSubEu->bitField0.needsInit == true) {
        flags = A_INIT;
        synthState->restart_bit0 = 0;
        synthState->restart_bit1 = 0;
        synthState->samplePosInt = note->playbackState.startSamplePos;
        synthState->samplePosFrac = 0;
        synthState->curVolLeft = 0;
        synthState->curVolRight = 0;
        synthState->prevHaasEffectLeftDelaySize = 0;
        synthState->prevHaasEffectRightDelaySize = 0;
        synthState->reverbVol = noteSubEu->reverbVol;
        synthState->numParts = 0;
        synthState->combFilterNeedsInit = true;
        note->noteSubEu.bitField0.finished = false;
        synthState->unk_1F = note->playbackState.unk_7C[4];
        finished = false;
    }

    resamplingRateFixedPoint = noteSubEu->resamplingRateFixedPoint;
    nParts = noteSubEu->bitField1.hasTwoParts + 1;
    samplesLenFixedPoint = (resamplingRateFixedPoint * aiBufLen * 2) + synthState->samplePosFrac;
    numSamplesToLoad = samplesLenFixedPoint >> 16;

    if (numSamplesToLoad == 0) {
        skipBytes = false;
    }

    synthState->samplePosFrac = samplesLenFixedPoint & 0xFFFF;

    // Partially-optimized out no-op ifs required for matching. SM64 decomp
    // makes it clear that this is how it should look.
    if (synthState->numParts == 1 && nParts == 2) {
    } else if (synthState->numParts == 2 && nParts == 1) {
    } else {
    }

    synthState->numParts = nParts;

    if (noteSubEu->bitField1.isSyntheticWave) {
        cmd = AudioSynth_LoadWaveSamples(cmd, noteSubEu, synthState, numSamplesToLoad);
        sampleDmemBeforeResampling = DMEM_UNCOMPRESSED_NOTE + (synthState->samplePosInt * 2);
        synthState->samplePosInt += numSamplesToLoad;
    } else {
        sample = noteSubEu->tunedSample->sample;
        loopInfo = sample->loop;

        if (note->playbackState.unk_04 != 0) {
            synthState->restart_bit1 = 1;
        }

        if ((loopInfo->count == 2) && synthState->restart_bit1) {
            loopEndPos = loopInfo->unk_0C;
        } else {
            loopEndPos = loopInfo->end;
        }

        sampleDataStartAddr = sample->sampleAddr;
        resampledTempLen = 0;

        for (curPart = 0; curPart < nParts; curPart++) {
            nSamplesProcessed = 0;
            dmemUncompressedAddrOffset1 = 0;

            if (nParts == 1) {
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

            while (nSamplesProcessed != samplesLenAdjusted) {
                noteFinished = false;
                restart = false;
                dmemUncompressedAddrOffset2 = 0;

                nFirstFrameSamplesToIgnore = synthState->samplePosInt & 0xF;
                nSamplesUntilLoopEnd = loopEndPos - synthState->samplePosInt;
                nSamplesToProcess = samplesLenAdjusted - nSamplesProcessed;

                if (nFirstFrameSamplesToIgnore == 0 && !(synthState->restart_bit0)) {
                    nFirstFrameSamplesToIgnore = 16;
                }
                nSamplesInFirstFrame = 16 - nFirstFrameSamplesToIgnore;

                if (nSamplesToProcess < nSamplesUntilLoopEnd) {
                    nFramesToDecode = (s32)(nSamplesToProcess - nSamplesInFirstFrame + 15) / 16;
                    nSamplesToDecode = nFramesToDecode * 16;
                    nTrailingSamplesToIgnore = nSamplesInFirstFrame + nSamplesToDecode - nSamplesToProcess;
                } else {
                    nSamplesToDecode = nSamplesUntilLoopEnd - nSamplesInFirstFrame;
                    nTrailingSamplesToIgnore = 0;
                    if (nSamplesToDecode <= 0) {
                        nSamplesToDecode = 0;
                        nSamplesInFirstFrame = nSamplesUntilLoopEnd;
                    }
                    nFramesToDecode = (nSamplesToDecode + 15) / 16;
                    if (loopInfo->count != 0) {
                        if (loopInfo->count == 2 && ((synthState->restart_bit1))) {
                            noteFinished = true;
                        } else {
                            // Loop around and restart
                            restart = true;
                        }
                    } else {
                        noteFinished = true;
                    }
                }

                // Set parameters based on compression type
                switch (sample->codec) {
                    case CODEC_ADPCM:
                        frameSize = 9;
                        skipInitialSamples = 16;
                        sampleDataStartOffset = 0;
                        break;

                    case CODEC_SMALL_ADPCM:
                        frameSize = 5;
                        skipInitialSamples = 16;
                        sampleDataStartOffset = 0;
                        break;

                    case CODEC_UNK7:
                        frameSize = 4;
                        skipInitialSamples = 16;
                        sampleDataStartOffset = 0;
                        break;

                    case CODEC_S8:
                        frameSize = 16;
                        skipInitialSamples = 16;
                        sampleDataStartOffset = 0;
                        break;

                    case CODEC_REVERB:
                        phi_a3 = -1;
                        if (D_80208E70 != NULL) {
                            phi_a3 = D_80208E70(sample, samplesLenAdjusted, flags, noteIndex);
                        }

                        if (phi_a3 == -1) {
                            noteFinished = true;
                        } else if (phi_a3 == 0) {
                            return cmd;
                        } else {
                            AudioSynth_LoadBuffer(cmd++, DMEM_UNCOMPRESSED_NOTE, (samplesLenAdjusted * 2) + 0x20,
                                                  phi_a3);
                            flags = A_CONTINUE;
                            skipBytes = 0;
                            nSamplesProcessed = samplesLenAdjusted;
                            dmemUncompressedAddrOffset1 = samplesLenAdjusted;
                        }

                        goto skip;

                    case CODEC_S16_INMEMORY:
                    case CODEC_UNK6:
                        AudioSynth_ClearBuffer(cmd++, DMEM_UNCOMPRESSED_NOTE, (samplesLenAdjusted * 2) + 0x20);
                        flags = A_CONTINUE;
                        skipBytes = 0;
                        nSamplesProcessed = samplesLenAdjusted;
                        dmemUncompressedAddrOffset1 = samplesLenAdjusted;
                        goto skip;

                    case CODEC_S16:
                        AudioSynth_ClearBuffer(cmd++, DMEM_UNCOMPRESSED_NOTE, (samplesLenAdjusted * 2) + 0x20);
                        flags = A_CONTINUE;
                        skipBytes = 0;
                        nSamplesProcessed = samplesLenAdjusted;
                        dmemUncompressedAddrOffset1 = samplesLenAdjusted;
                        goto skip;

                    default:
                        break;
                }

                // Move the compressed raw sample data into the rsp (DMEM)
                if (nFramesToDecode != 0) {
                    // Get the offset from the start of the sample to where the sample is currently playing from
                    frameIndex = (synthState->samplePosInt + skipInitialSamples - nFirstFrameSamplesToIgnore) / 16;
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
                                                    ALIGN16((nFramesToDecode * frameSize) + 0x10), flags,
                                                    &synthState->sampleDmaIndex, sample->medium);
                    }

                    if (sampleDataChunkAddr == NULL) {
                        // The ram address was unsuccesffully allocated
                        return cmd;
                    }

                    // Move the raw sample chunk from ram to the rsp
                    // DMEM at the addresses before DMEM_COMPRESSED_ADPCM_DATA
                    sampleDataChunkAlignPad = (u32)sampleDataChunkAddr & 0xF;
                    sampleDataChunkSize = ALIGN16((nFramesToDecode * frameSize) + 16);
                    sampleDataDmemAddr = DMEM_COMPRESSED_ADPCM_DATA - sampleDataChunkSize;
                    aLoadBuffer(cmd++, sampleDataChunkAddr - sampleDataChunkAlignPad, sampleDataDmemAddr,
                                sampleDataChunkSize);
                } else {
                    nSamplesToDecode = 0;
                    sampleDataChunkAlignPad = 0;
                }

                if (synthState->restart_bit0) {
                    aSetLoop(cmd++, sample->loop->predictorState);
                    flags = A_LOOP;
                    synthState->restart_bit0 = false;
                }

                nSamplesInThisIteration = nSamplesToDecode + nSamplesInFirstFrame - nTrailingSamplesToIgnore;

                if (nSamplesProcessed == 0) {
                    if (1) {}
                    skipBytes = nFirstFrameSamplesToIgnore * 2;
                } else {
                    dmemUncompressedAddrOffset2 = ALIGN16(dmemUncompressedAddrOffset1 + 16);
                }

                // Decompress the raw sample chunks in the rsp
                // Goes from adpcm (compressed) sample data to pcm (uncompressed) sample data
                switch (sample->codec) {
                    case CODEC_ADPCM:
                        sampleDataChunkSize = ALIGN16((nFramesToDecode * frameSize) + 0x10);
                        sampleDataDmemAddr = DMEM_COMPRESSED_ADPCM_DATA - sampleDataChunkSize;
                        aSetBuffer(cmd++, 0, sampleDataDmemAddr + sampleDataChunkAlignPad,
                                   DMEM_UNCOMPRESSED_NOTE + dmemUncompressedAddrOffset2, nSamplesToDecode * 2);
                        aADPCMdec(cmd++, flags, synthState->synthesisBuffers->adpcmState);
                        break;

                    case CODEC_SMALL_ADPCM:
                        sampleDataChunkSize = ALIGN16((nFramesToDecode * frameSize) + 0x10);
                        sampleDataDmemAddr = DMEM_COMPRESSED_ADPCM_DATA - sampleDataChunkSize;
                        aSetBuffer(cmd++, 0, sampleDataDmemAddr + sampleDataChunkAlignPad,
                                   DMEM_UNCOMPRESSED_NOTE + dmemUncompressedAddrOffset2, nSamplesToDecode * 2);
                        aADPCMdec(cmd++, flags | 4, synthState->synthesisBuffers->adpcmState);
                        break;

                    case CODEC_S8:
                        sampleDataChunkSize = ALIGN16((nFramesToDecode * frameSize) + 0x10);
                        sampleDataDmemAddr = DMEM_COMPRESSED_ADPCM_DATA - sampleDataChunkSize;
                        AudioSynth_SetBuffer(cmd++, 0, sampleDataDmemAddr + sampleDataChunkAlignPad,
                                             DMEM_UNCOMPRESSED_NOTE + dmemUncompressedAddrOffset2,
                                             nSamplesToDecode * 2);
                        AudioSynth_S8Dec(cmd++, flags, synthState->synthesisBuffers->adpcmState);
                        break;

                    case CODEC_UNK7:
                        break;
                }

                if (nSamplesProcessed != 0) {
                    aDMEMMove(cmd++,
                              DMEM_UNCOMPRESSED_NOTE + dmemUncompressedAddrOffset2 + (nFirstFrameSamplesToIgnore * 2),
                              DMEM_UNCOMPRESSED_NOTE + dmemUncompressedAddrOffset1, nSamplesInThisIteration * 2);
                }

                nSamplesProcessed += nSamplesInThisIteration;

                switch (flags) {
                    case A_INIT:
                        skipBytes = 0x20;
                        dmemUncompressedAddrOffset1 = (nSamplesToDecode + 0x10) * 2;
                        break;

                    case A_LOOP:
                        dmemUncompressedAddrOffset1 = nSamplesInThisIteration * 2 + dmemUncompressedAddrOffset1;
                        break;

                    default:
                        if (dmemUncompressedAddrOffset1 != 0) {
                            dmemUncompressedAddrOffset1 = nSamplesInThisIteration * 2 + dmemUncompressedAddrOffset1;
                        } else {
                            dmemUncompressedAddrOffset1 = (nFirstFrameSamplesToIgnore + nSamplesInThisIteration) * 2;
                        }
                        break;
                }

                flags = A_CONTINUE;

            skip:
                if (noteFinished) {
                    if ((samplesLenAdjusted - nSamplesProcessed) != 0) {
                        AudioSynth_ClearBuffer(cmd++, DMEM_UNCOMPRESSED_NOTE + dmemUncompressedAddrOffset1,
                                               (samplesLenAdjusted - nSamplesProcessed) * 2);
                    }
                    finished = true;
                    note->noteSubEu.bitField0.finished = true;
                    func_80187DE8(updateIndex, noteIndex);
                    break; // break out of the for-loop
                } else if (restart) {
                    synthState->restart_bit0 = true;
                    synthState->samplePosInt = loopInfo->start;
                } else {
                    synthState->samplePosInt += nSamplesToProcess;
                }
            }

            switch (nParts) {
                case 1:
                    sampleDmemBeforeResampling = DMEM_UNCOMPRESSED_NOTE + skipBytes;
                    break;

                case 2:
                    switch (curPart) {
                        case 0:
                            AudioSynth_InterL(cmd++, DMEM_UNCOMPRESSED_NOTE + skipBytes, DMEM_TEMP + 0x20,
                                              ALIGN8(samplesLenAdjusted / 2));
                            resampledTempLen = samplesLenAdjusted;
                            sampleDmemBeforeResampling = DMEM_TEMP + 0x20;
                            if (finished) {
                                AudioSynth_ClearBuffer(cmd++, sampleDmemBeforeResampling + resampledTempLen,
                                                       samplesLenAdjusted + 0x10);
                            }
                            break;

                        case 1:
                            AudioSynth_InterL(cmd++, DMEM_UNCOMPRESSED_NOTE + skipBytes,
                                              DMEM_TEMP + 0x20 + resampledTempLen, ALIGN8(samplesLenAdjusted / 2));
                            break;
                    }
            }
            if (finished) {
                break;
            }
        }
    }

    flags = A_CONTINUE;
    if (noteSubEu->bitField0.needsInit == true) {
        noteSubEu->bitField0.needsInit = false;
        flags = A_INIT;
    }

    cmd = AudioSynth_FinalResample(cmd, synthState, aiBufLen * 2, resamplingRateFixedPoint, sampleDmemBeforeResampling,
                                   flags);
    if (bookOffset == 3) {
        func_801881F8(cmd++, DMEM_TEMP, DMEM_TEMP, aiBufLen * 2, 0);
    }

    gain = noteSubEu->gain;
    if (gain != 0) {
        // A gain of 0x10 (a UQ4.4 number) is equivalent to 1.0 and represents no volume change
        if (gain < 0x10) {
            gain = 0x10;
        }
        AudioSynth_HiLoGain(cmd++, gain, DMEM_TEMP, 0, (aiBufLen * 2) + 0x20);
    }

    filter = noteSubEu->filter;
    if (filter != 0) {
        AudioSynth_LoadFilterSize(cmd++, aiBufLen * 2, filter);
        AudioSynth_LoadFilterBuffer(cmd++, flags, DMEM_TEMP, synthState->synthesisBuffers->filterState);
    }

    combFilterSize = noteSubEu->combFilterSize;
    combFilterGain = noteSubEu->combFilterGain;
    combFilterState = synthState->synthesisBuffers->combFilterState;
    if ((combFilterSize != 0) && (noteSubEu->combFilterGain != 0)) {
        AudioSynth_DMemMove(cmd++, DMEM_TEMP, DMEM_SCRATCH2, aiBufLen * 2);
        dmemAddr = DMEM_SCRATCH2 - combFilterSize;
        if (synthState->combFilterNeedsInit) {
            AudioSynth_ClearBuffer(cmd++, dmemAddr, combFilterSize);
            synthState->combFilterNeedsInit = false;
        } else {
            AudioSynth_LoadBuffer(cmd++, dmemAddr, combFilterSize, combFilterState);
        }
        AudioSynth_SaveBuffer(cmd++, DMEM_TEMP + (aiBufLen * 2) - combFilterSize, combFilterSize, combFilterState);
        AudioSynth_Mix(cmd++, (aiBufLen * 2) >> 4, combFilterGain, DMEM_SCRATCH2, dmemAddr);
        AudioSynth_DMemMove(cmd++, dmemAddr, DMEM_TEMP, aiBufLen * 2);
    } else {
        synthState->combFilterNeedsInit = true;
    }

    if ((noteSubEu->haasEffectLeftDelaySize != 0) || (synthState->prevHaasEffectLeftDelaySize != 0)) {
        haasEffectDelaySide = HAAS_EFFECT_DELAY_LEFT;
    } else if ((noteSubEu->haasEffectRightDelaySize != 0) || (synthState->prevHaasEffectRightDelaySize != 0)) {
        haasEffectDelaySide = HAAS_EFFECT_DELAY_RIGHT;
    } else {
        haasEffectDelaySide = HAAS_EFFECT_DELAY_NONE;
    }

    if (gAudioContext.soundMode == AUDIO_MODE_SURROUND) {
        noteSubEu->targetVolLeft = noteSubEu->targetVolLeft >> 1;
        noteSubEu->targetVolRight = noteSubEu->targetVolRight >> 1;
        if (noteSubEu->unk_19 != 0xFF) {
            cmd = AudioSynth_ApplySurroundEffect(cmd, noteSubEu, synthState, aiBufLen, DMEM_TEMP, flags);
        }
    }

    cmd = AudioSynth_ProcessEnvelope(cmd, noteSubEu, synthState, aiBufLen, DMEM_TEMP, haasEffectDelaySide, flags);

    if (noteSubEu->bitField1.useHaasEffect) {
        if (!(flags & A_INIT)) {
            flags = A_CONTINUE;
        }
        cmd = AudioSynth_ApplyHaasEffect(cmd, noteSubEu, synthState, aiBufLen * 2, flags, haasEffectDelaySide);
    }

    return cmd;
}

Acmd* AudioSynth_ApplySurroundEffect(Acmd* cmd, NoteSubEu* noteSubEu, NoteSynthesisState* synthState, s32 aiBufLen,
                                     s32 dmem, s32 flags) {
    s32 temp_v0;
    u16 temp_t0;
    s64 new_var = 0x4B0;
    f32 phi_f0;

    AudioSynth_DMemMove(cmd++, dmem, DMEM_HAAS_TEMP, aiBufLen * 2);
    temp_t0 = synthState->unk_08;

    if (flags == 1) {
        aClearBuffer(cmd++, new_var, 0x100);
        synthState->unk_08 = 0;
    } else {
        aLoadBuffer(cmd++, synthState->synthesisBuffers->surroundEffectState, new_var, 0x100);
        aMix(cmd++, (aiBufLen * 2) >> 4, temp_t0, new_var, DMEM_LEFT_CH);
        aMix(cmd++, (aiBufLen * 2) >> 4, (temp_t0 ^ 0xFFFF), new_var, DMEM_RIGHT_CH);

        temp_v0 = (temp_t0 * synthState->reverbVol) >> 7;

        aMix(cmd++, (aiBufLen * 2) >> 4, temp_v0, new_var, DMEM_WET_LEFT_CH);
        aMix(cmd++, (aiBufLen * 2) >> 4, (temp_v0 ^ 0xFFFF), new_var, DMEM_WET_RIGHT_CH);
    }

    aSaveBuffer(cmd++, (aiBufLen * 2) + 0x4B0, synthState->synthesisBuffers->surroundEffectState, 0x100);

    phi_f0 = (noteSubEu->targetVolLeft + noteSubEu->targetVolRight) * 0.00012207031f;

    if (phi_f0 > 1.0f) {
        phi_f0 = 1.0f;
    }

    phi_f0 = gDefaultPanVolume[127 - noteSubEu->unk_19] * phi_f0;
    synthState->unk_08 = ((phi_f0 * 32767.0f) + synthState->unk_08) / 2;

    AudioSynth_DMemMove(cmd++, DMEM_HAAS_TEMP, dmem, aiBufLen * 2);

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

Acmd* AudioSynth_ProcessEnvelope(Acmd* cmd, NoteSubEu* noteSubEu, NoteSynthesisState* synthState, s32 aiBufLen,
                                 u16 dmemSrc, s32 haasEffectDelaySide, s32 flags) {
    u32 dmemDests;
    u16 curVolLeft;
    u16 targetVolLeft;
    s32 phi_t1;
    u16 curVolRight;
    s16 reverbVol;
    s16 rampLeft;
    s16 rampRight;
    s16 rampReverb;
    s16 sourceReverbVol;
    u16 targetVolRight;
    f32 temp_f0;
    s32 pad;

    reverbVol = noteSubEu->reverbVol;
    curVolLeft = synthState->curVolLeft;
    curVolRight = synthState->curVolRight;

    targetVolLeft = noteSubEu->targetVolLeft;
    targetVolLeft <<= 4;
    targetVolRight = noteSubEu->targetVolRight;
    targetVolRight <<= 4;

    if (gAudioContext.soundMode == AUDIO_MODE_SURROUND) {
        if (noteSubEu->unk_19 != 0xFF) {
            temp_f0 = gDefaultPanVolume[noteSubEu->unk_19];
            targetVolLeft *= temp_f0;
            targetVolRight *= temp_f0;
        }
    }

    if (targetVolLeft != curVolLeft) {
        rampLeft = (targetVolLeft - curVolLeft) / (aiBufLen >> 3);
    } else {
        rampLeft = 0;
    }

    if (targetVolRight != curVolRight) {
        rampRight = (targetVolRight - curVolRight) / (aiBufLen >> 3);
    } else {
        rampRight = 0;
    }

    sourceReverbVol = synthState->reverbVol;
    phi_t1 = sourceReverbVol & 0x7F;

    if (sourceReverbVol != reverbVol) {
        rampReverb = (((reverbVol & 0x7F) - phi_t1) << 9) / (aiBufLen >> 3);
        synthState->reverbVol = reverbVol;
    } else {
        rampReverb = 0;
    }

    synthState->curVolLeft = curVolLeft + (rampLeft * (aiBufLen >> 3));
    synthState->curVolRight = curVolRight + (rampRight * (aiBufLen >> 3));

    if (noteSubEu->bitField1.useHaasEffect) {
        AudioSynth_ClearBuffer(cmd++, DMEM_HAAS_TEMP, DMEM_1CH_SIZE);
        AudioSynth_EnvSetup1(cmd++, phi_t1 * 2, rampReverb, rampLeft, rampRight);
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
        aEnvSetup1(cmd++, phi_t1 * 2, rampReverb, rampLeft, rampRight);
        aEnvSetup2(cmd++, curVolLeft, curVolRight);
        dmemDests = sEnvMixerDefaultDmemDests;
    }

    aEnvMixer(cmd++, dmemSrc, aiBufLen, (sourceReverbVol & 0x80) >> 7, noteSubEu->bitField0.envMixerNegWetLeft,
              noteSubEu->bitField0.envMixerNegWetRight, noteSubEu->bitField0.envMixerNegDryLeft,
              noteSubEu->bitField0.envMixerNegDryRight, dmemDests, sEnvMixerOp);

    return cmd;
}

Acmd* AudioSynth_LoadWaveSamples(Acmd* cmd, NoteSubEu* noteSubEu, NoteSynthesisState* synthState,
                                 s32 numSamplesToLoad) {
    s32 numSamplesAvail;
    s32 harmonicIndexCurAndPrev = noteSubEu->harmonicIndexCurAndPrev;
    s32 samplePosInt = synthState->samplePosInt;
    s32 numDuplicates;

    if (noteSubEu->bitField1.bookOffset != 0) {
        // Move the noise wave (that reads compiled assembly as samples) from ram to dmem
        AudioSynth_LoadBuffer(cmd++, DMEM_UNCOMPRESSED_NOTE, ALIGN16(numSamplesToLoad * SAMPLE_SIZE), gWaveSamples[8]);
        // Offset the address for the samples read by gWaveSamples[8] to the next set of samples
        gWaveSamples[8] += numSamplesToLoad * 2;

        return cmd;
    } else {
        // Move the synthetic wave from ram to dmem
        aLoadBuffer(cmd++, noteSubEu->waveSampleAddr, DMEM_UNCOMPRESSED_NOTE, WAVE_SAMPLE_COUNT * SAMPLE_SIZE);

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
Acmd* AudioSynth_ApplyHaasEffect(Acmd* cmd, NoteSubEu* noteSubEu, NoteSynthesisState* synthState, s32 size, s32 flags,
                                 s32 haasEffectDelaySide) {
    u16 dmemDest;
    u16 pitch;
    u8 prevHaasEffectDelaySize;
    u8 haasEffectDelaySize;

    switch (haasEffectDelaySide) {
        case HAAS_EFFECT_DELAY_LEFT:
            // Delay the sample on the left channel
            // This allows the right channel to be heard first
            dmemDest = DMEM_LEFT_CH;
            haasEffectDelaySize = noteSubEu->haasEffectLeftDelaySize;
            prevHaasEffectDelaySize = synthState->prevHaasEffectLeftDelaySize;
            synthState->prevHaasEffectRightDelaySize = 0;
            synthState->prevHaasEffectLeftDelaySize = haasEffectDelaySize;
            break;

        case HAAS_EFFECT_DELAY_RIGHT:
            // Delay the sample on the right channel
            // This allows the left channel to be heard first
            dmemDest = DMEM_RIGHT_CH;
            haasEffectDelaySize = noteSubEu->haasEffectRightDelaySize;
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
