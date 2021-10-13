#include "global.h"

#define DEFAULT_LEN_1CH 0x1A0
#define DEFAULT_LEN_2CH 0x340

#define DMEM_TEMP 0x3B0
#define DMEM_UNCOMPRESSED_NOTE 0x570
#define DMEM_NOTE_PAN_TEMP 0x5C0
#define DMEM_SCRATCH2 0x750              // = DMEM_TEMP + DEFAULT_LEN_2CH + a bit more
#define DMEM_COMPRESSED_ADPCM_DATA 0x930 // = DMEM_LEFT_CH
#define DMEM_LEFT_CH 0x930
#define DMEM_RIGHT_CH 0xAE0
#define DMEM_WET_TEMP 0x3D0
#define DMEM_WET_SCRATCH 0x720 // = DMEM_WET_TEMP + DEFAULT_LEN_2CH
#define DMEM_WET_LEFT_CH 0xC70
#define DMEM_WET_RIGHT_CH 0xE10 // = DMEM_WET_LEFT_CH + DEFAULT_LEN_1CH

Acmd* func_80188AFC(Acmd* cmd, u16 arg1, u16 arg2, s32 arg3);

void AudioSynth_InitNextRingBuf(s32 chunkLen, s32 bufIndex, s32 reverbIndex) {
    SynthesisReverb* reverb;
    ReverbRingBufferItem* bufItem;
    s32 extraSamples;
    s32 sampleCnt;
    s32 temp_t1;
    u16 temp_t2;
    s32 temp_t2_2;
    s32 temp_t3;
    s32 temp_t4;
    s32 phi_a2;
    s32 phi_t1;
    s32 phi_a3;
    
    reverb = &gAudioContext.synthesisReverbs[reverbIndex];
    bufItem = &reverb->items[reverb->curFrame][bufIndex];

    sampleCnt = chunkLen / gAudioContext.synthesisReverbs[reverbIndex].downsampleRate;

    if (gAudioContext.synthesisReverbs[reverbIndex].unk_18 != 0) {
        if (reverb->downsampleRate == 1) {
            phi_a2 = 0;
            phi_t1 = 0;

            sampleCnt += reverb->unk_19;
            temp_t2 = sampleCnt;
            bufItem->unk_18 = sampleCnt;
            bufItem->unk_14 = (temp_t2 << 0xF) / chunkLen;
            bufItem->unk_16 = (chunkLen << 0xF) / temp_t2;
            
            while (true) {
                temp_t2_2 = (bufItem->unk_14 * chunkLen * 2) + reverb->unk_1A;
                temp_t4 = temp_t2_2 >> 0x10;
                
                if ((sampleCnt != temp_t4) && (phi_a2 == 0)) {
                    bufItem->unk_14 = ((sampleCnt << 0x10) - reverb->unk_1A) / (chunkLen * 2);
                    phi_a2++;
                } else {
                    phi_a2++;
                    if (sampleCnt < temp_t4) {
                        bufItem->unk_14--;
                    } else if (temp_t4 < sampleCnt) {
                        bufItem->unk_14++;
                    } else {
                        break;
                    }
                }
            }

            reverb->unk_1A = temp_t2_2 & 0xFFFF;

            while (true) {
                temp_t2_2 = (bufItem->unk_16 * sampleCnt * 2) + reverb->unk_1C;
                temp_t4 = temp_t2_2 >> 0x10;
                
                if ((chunkLen != temp_t4) && (phi_t1 == 0)) {
                    bufItem->unk_16 = ((chunkLen << 0x10) - reverb->unk_1C) / (sampleCnt * 2);
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

    extraSamples = (reverb->nextRingBufPos + sampleCnt) - reverb->windowSize;

    temp_t1 = reverb->nextRingBufPos;
    if (extraSamples < 0) {
        bufItem->lengthA = sampleCnt * 2;
        bufItem->lengthB = 0;
        bufItem->startPos = reverb->nextRingBufPos;
        reverb->nextRingBufPos += sampleCnt;
    } else {
        bufItem->lengthA = (sampleCnt - extraSamples) * 2;
        bufItem->lengthB = extraSamples * 2;
        bufItem->startPos = reverb->nextRingBufPos;
        reverb->nextRingBufPos = extraSamples;
    }

    bufItem->numSamplesAfterDownsampling = sampleCnt;
    bufItem->chunkLen = chunkLen;

    if (reverb->unk_14 != 0) {
        phi_a3 = reverb->unk_14 + temp_t1;
        if (phi_a3 >= reverb->windowSize) {
            phi_a3 -= reverb->windowSize;
        }

        bufItem = &reverb->items2[reverb->curFrame][bufIndex];
        sampleCnt = chunkLen / reverb->downsampleRate;
        extraSamples = (phi_a3 + sampleCnt) - reverb->windowSize;

        if (extraSamples < 0) {
            bufItem->lengthA = sampleCnt * 2;
            bufItem->lengthB = 0;
            bufItem->startPos = phi_a3;
        } else {
            bufItem->lengthA = (sampleCnt - extraSamples) * 2;
            bufItem->lengthB = extraSamples * 2;
            bufItem->startPos = phi_a3;
        }
        
        bufItem->numSamplesAfterDownsampling = sampleCnt;
        bufItem->chunkLen = chunkLen;
    }
}

// OoT func_800DB03C
void func_80187B64(s32 arg0) {
    NoteSubEu* subEu;
    NoteSubEu* subEu2;
    s32 baseIndex;
    s32 i;

    baseIndex = gAudioContext.numNotes * arg0;
    for (i = 0; i < gAudioContext.numNotes; i++) {
        subEu = &gAudioContext.notes[i].noteSubEu;
        subEu2 = &gAudioContext.noteSubsEu[baseIndex + i];
        if (subEu->bitField0.enabled) {
            subEu->bitField0.needsInit = false;
        } else {
            subEu2->bitField0.enabled = false;
        }

        subEu->unk_06 = 0;
    }
}

Acmd* AudioSynth_Update(Acmd* cmdStart, s32* cmdCnt, s16* aiStart, s32 aiBufLen) {
    s32 chunkLen;
    s16* aiBufP;
    Acmd* cmdP;
    s32 i;
    s32 j;
    SynthesisReverb* reverb;

    cmdP = cmdStart;
    for (i = gAudioContext.audioBufferParameters.updatesPerFrame; i > 0; i--) {
        AudioSeq_ProcessSequences(i - 1);
        func_80187B64(gAudioContext.audioBufferParameters.updatesPerFrame - i);
    }

    aiBufP = aiStart;
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

        cmdP = AudioSynth_DoOneAudioUpdate(aiBufP, chunkLen, cmdP,
                                           gAudioContext.audioBufferParameters.updatesPerFrame - i);
        aiBufLen -= chunkLen;
        aiBufP += chunkLen * 2;
    }

    for (j = 0; j < gAudioContext.numSynthesisReverbs; j++) {
        if (gAudioContext.synthesisReverbs[j].framesToIgnore != 0) {
            gAudioContext.synthesisReverbs[j].framesToIgnore--;
        }
        gAudioContext.synthesisReverbs[j].curFrame ^= 1;
    }

    *cmdCnt = cmdP - cmdStart;
    return cmdP;
}

// OoT func_800DB2C0
void func_80187DE8(s32 updateIndexStart, s32 noteIndex) {
    NoteSubEu* temp_v1;
    s32 i;

    for (i = updateIndexStart + 1; i < gAudioContext.audioBufferParameters.updatesPerFrame; i++) {
        temp_v1 = &gAudioContext.noteSubsEu[(gAudioContext.numNotes * i) + noteIndex];
        if (!temp_v1->bitField0.needsInit) {
            temp_v1->bitField0.enabled = 0;
        } else {
            break;
        }
    }
}

Acmd* AudioSynth_LoadRingBuffer1AtTemp(Acmd* cmd, SynthesisReverb* reverb, s16 bufIndex) {
    ReverbRingBufferItem* bufItem = &reverb->items[reverb->curFrame][bufIndex];

    cmd = AudioSynth_LoadRingBufferPart(cmd, DMEM_WET_TEMP, bufItem->startPos, bufItem->lengthA, reverb);
    if (bufItem->lengthB != 0) {
        // Ring buffer wrapped
        cmd = AudioSynth_LoadRingBufferPart(cmd, DMEM_WET_TEMP + bufItem->lengthA, 0, bufItem->lengthB, reverb);
    }
    return cmd;
}

Acmd* AudioSynth_SaveRingBuffer1AtTemp(Acmd* cmd, SynthesisReverb* reverb, s16 bufIndex) {
    ReverbRingBufferItem* bufItem = &reverb->items[reverb->curFrame][bufIndex];

    cmd = AudioSynth_SaveRingBufferPart(cmd, DMEM_WET_TEMP, bufItem->startPos, bufItem->lengthA, reverb);
    if (bufItem->lengthB != 0) {
        // Ring buffer wrapped
        cmd = AudioSynth_SaveRingBufferPart(cmd, DMEM_WET_TEMP + bufItem->lengthA, 0, bufItem->lengthB, reverb);
    }
    return cmd;
}

void func_80187FA8(void) {
}

void AudioSynth_ClearBuffer(Acmd* cmd, s32 arg1, s32 arg2) {
    aClearBuffer(cmd, arg1, arg2);
}

void func_80187FD0(void) {
}

void func_80187FD8(void) {
}

void func_80187FE0(void) {
}

void AudioSynth_Mix(Acmd* cmd, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    aMix(cmd, arg1, arg2, arg3, arg4);
}

void func_8018801C(void) {
}

void func_80188024(void) {
}

void func_8018802C(void) {
}

void AudioSynth_SetBuffer(Acmd* cmd, s32 flags, s32 dmemIn, s32 dmemOut, s32 count) {
    aSetBuffer(cmd, flags, dmemIn, dmemOut, count);
}

void func_80188068(void) {
}

void func_80188070(void) {
}

// possible fake match?
void AudioSynth_DMemMove(Acmd* cmd, s32 dmemIn, s32 dmemOut, s32 count) {
    cmd->words.w0 = _SHIFTL(A_DMEMMOVE, 24, 8) | _SHIFTL(dmemIn, 0, 24);
    cmd->words.w1 = _SHIFTL(dmemOut, 16, 16) | _SHIFTL(count, 0, 16);
}

void func_801880A4(void) {
}

void func_801880AC(void) {
}

void func_801880B4(void) {
}

void func_801880BC(void) {
}

void AudioSynth_InterL(Acmd* cmd, s32 dmemIn, s32 dmemOut, s32 count) {
    cmd->words.w0 = _SHIFTL(A_INTERL, 24, 8) | _SHIFTL(count, 0, 16);
    cmd->words.w1 = _SHIFTL(dmemIn, 16, 16) | _SHIFTL(dmemOut, 0, 16);
}

void AudioSynth_EnvSetup1(Acmd* cmd, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    aEnvSetup1(cmd, arg1, arg2, arg3, arg4);
}

void func_8018811C(void) { 
}

void AudioSynth_LoadBuffer(Acmd* cmd, s32 arg1, s32 arg2, s32 arg3) {
    aLoadBuffer(cmd, arg3, arg1, arg2);
}

void AudioSynth_SaveBuffer(Acmd* cmd, s32 arg1, s32 arg2, s32 arg3) {
    aSaveBuffer(cmd, arg1, arg3, arg2);
}

void AudioSynth_EnvSetup2(Acmd* cmd, s32 volLeft, s32 volRight) {
    cmd->words.w0 = _SHIFTL(A_ENVSETUP2, 24, 8);
    cmd->words.w1 = _SHIFTL(volLeft, 16, 16) | _SHIFTL(volRight, 0, 16);
}
void func_80188190(void) {
}

void func_80188198(void) {
}

void func_801881A0(void) {
}

void AudioSynth_S8Dec(Acmd* cmd, s32 flags, s16* state) {
    aS8Dec(cmd, flags, state);
}

void AudioSynth_HiLoGain(Acmd* cmd, s32 gain, s32 dmemIn, s32 dmemOut, s32 count) {
    cmd->words.w0 = _SHIFTL(A_HILOGAIN, 24, 8) | _SHIFTL(gain, 16, 8) | _SHIFTL(count, 0, 16);
    cmd->words.w1 = _SHIFTL(dmemIn, 16, 16) | _SHIFTL(dmemOut, 0, 16);
}

void func_801881F8(Acmd* cmd, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    cmd->words.w0 = _SHIFTL(arg4, 16, 8) | _SHIFTL(arg3, 0, 16);
    cmd->words.w1 = _SHIFTL(arg1, 16, 16) | _SHIFTL(arg2, 0, 16);
}

void func_80188224(void) {
}

void func_8018822C(void) {
}

void func_80188234(void) {
}

void func_8018823C(void) {
}

// OoT AudioSynth_UnkCmd3
void func_80188244(void) {
}

void func_8018824C(void) {
}

void func_80188254(void) {
}

void func_8018825C(void) {
}

void AudioSynth_LoadFilter(Acmd* cmd, s32 flags, s32 countOrBuf, s32 addr) {
    aFilter(cmd, flags, countOrBuf, addr);
}

void AudioSynth_LoadFilterCount(Acmd* cmd, s32 count, s32 addr) {
    aFilter(cmd, 2, count, addr);
}

// New MM Function
void* func_801882A0(Acmd* cmd, ReverbSettings* arg1) {
    aDMEMMove(cmd++, 0xC70, 0x710, 0x1A0);
    aMix(cmd++, 0x1A, (s16)arg1->unk_10u, 0xE10, 0xC70);
    aMix(cmd++, 0x1A, (s16)arg1->unk_12, 0x710, 0xE10);
    return cmd;
}

// New MM Function
void* func_80188304(Acmd* cmd, s32 arg1, SynthesisReverb* reverb, s16 bufIndex) {
    ReverbRingBufferItem* bufItem = &reverb->items[reverb->curFrame][bufIndex];
    s16 sp2A;
    s16 sp28;

    sp2A = (bufItem->startPos & 7) * 2;
    sp28 = ALIGN16(sp2A + bufItem->lengthA);
    
    cmd = AudioSynth_LoadRingBufferPart(cmd, 0x3D0, bufItem->startPos - (sp2A / 2), 0x1A0, reverb);

    if (bufItem->lengthB != 0) {
        cmd = AudioSynth_LoadRingBufferPart(cmd, sp28 + 0x3D0, 0, 0x1A0 - sp28, reverb);
    }

    aSetBuffer(cmd++, 0, sp2A + 0x3D0, 0xC70, arg1 * 2);
    aResample(cmd++, reverb->resampleFlags, reverb->unk_0E, reverb->unk_30);
    aSetBuffer(cmd++, 0, sp2A + 0x570, 0xE10, arg1 * 2);
    aResample(cmd++, reverb->resampleFlags, reverb->unk_0E, reverb->unk_34);

    return cmd;
}

// New MM Function
Acmd* func_801884A0(Acmd* cmd, SynthesisReverb* reverb, s16 bufIndex) {
    ReverbRingBufferItem* bufItem = &reverb->items[reverb->curFrame][bufIndex];
    s16 chuchLen = bufItem->chunkLen;
    u32 sp28 = chuchLen * 2;

    aDMEMMove(cmd++, 0xC70, 0x3D0, sp28);
    aSetBuffer(cmd++, 0, 0x3D0, 0x710, bufItem->unk_18 * 2);
    aResample(cmd++, reverb->resampleFlags, bufItem->unk_16, reverb->unk_38); 

    cmd = func_80188AFC(cmd, 0x710, bufItem->lengthA, &reverb->leftRingBuf[bufItem->startPos]);

    if (bufItem->lengthB != 0) {
        cmd = func_80188AFC(cmd, bufItem->lengthA + 0x710, bufItem->lengthB, reverb->leftRingBuf);
    }

    aDMEMMove(cmd++, 0xE10, 0x3D0, sp28);
    aSetBuffer(cmd++, 0, 0x3D0, 0x710, bufItem->unk_18 * 2);
    aResample(cmd++, reverb->resampleFlags, bufItem->unk_16, reverb->unk_3C);

    cmd = func_80188AFC(cmd, 0x710, bufItem->lengthA, &reverb->rightRingBuf[bufItem->startPos]);

    if (bufItem->lengthB != 0) {
        cmd = func_80188AFC(cmd, bufItem->lengthA + 0x710, bufItem->lengthB, reverb->rightRingBuf);
    }

    return cmd;
}

// New MM Function
void* func_80188698(Acmd* cmd, s32 arg1, SynthesisReverb* reverb, s16 bufIndex) {
    ReverbRingBufferItem* bufItem = &reverb->items[reverb->curFrame][bufIndex];
    s16 sp2A = (bufItem->startPos & 7) * 2;
    s16 sp28 = ALIGN16(sp2A + bufItem->lengthA);

    cmd = AudioSynth_LoadRingBufferPart(cmd, 0x3D0, bufItem->startPos - (sp2A / 2), 0x1A0, reverb);

    if (bufItem->lengthB != 0) {
        cmd = AudioSynth_LoadRingBufferPart(cmd, sp28 + 0x3D0, 0, 0x1A0 - sp28, reverb);
    }

    aSetBuffer(cmd++, 0, sp2A + 0x3D0, 0xC70, arg1 * 2);
    aResample(cmd++, reverb->resampleFlags, bufItem->unk_14, reverb->unk_30);
    aSetBuffer(cmd++, 0, sp2A + 0x570, 0xE10, arg1 * 2);
    aResample(cmd++, reverb->resampleFlags, bufItem->unk_14, reverb->unk_34);

    return cmd;
}

// New MM Function
Acmd* func_8018883C(Acmd* cmd, s32 arg1, SynthesisReverb* reverb) {
    if (reverb->filterLeft != NULL) {
        aFilter(cmd++, 2, arg1, reverb->filterLeft);
        aFilter(cmd++, reverb->resampleFlags, 0xC70, reverb->unk_280);
    }

    if (reverb->filterRight != NULL) {
        aFilter(cmd++, 2, arg1, reverb->filterRight);
        aFilter(cmd++, reverb->resampleFlags, 0xE10, reverb->unk_284);
    }
    return cmd;
}

// New MM Function
Acmd* func_801888E4(Acmd* cmd, SynthesisReverb* reverb, s32 arg2) {
    SynthesisReverb* reverb2;

    if (reverb->unk_05 >= gAudioContext.numSynthesisReverbs) {
        return cmd;
    } 
    
    reverb2 = &gAudioContext.synthesisReverbs[reverb->unk_05];
    if (reverb2->downsampleRate == 1) {
        cmd = AudioSynth_LoadRingBuffer1AtTemp(cmd, reverb2, arg2);
        aMix(cmd++, 0x34, reverb->unk_08, 0xC70, 0x3D0);
        cmd = AudioSynth_SaveRingBuffer1AtTemp(cmd, reverb2, arg2);
    }

    return cmd;
}

Acmd* AudioSynth_LoadRingBuffer1(Acmd* cmd, s32 arg1, SynthesisReverb* reverb, s16 bufIndex) {
    ReverbRingBufferItem* bufItem = &reverb->items[reverb->curFrame][bufIndex];

    cmd =
        AudioSynth_LoadRingBufferPart(cmd, DMEM_WET_LEFT_CH, bufItem->startPos, bufItem->lengthA, reverb);
    if (bufItem->lengthB != 0) {
        // Ring buffer wrapped
        cmd = AudioSynth_LoadRingBufferPart(cmd, DMEM_WET_LEFT_CH + bufItem->lengthA, 0, bufItem->lengthB,
                                            reverb);
    }

    return cmd;
}

Acmd* AudioSynth_LoadRingBuffer2(Acmd* cmd, s32 arg1, SynthesisReverb* reverb, s16 bufIndex) {
    ReverbRingBufferItem* bufItem = &reverb->items2[reverb->curFrame][bufIndex];

    cmd = AudioSynth_LoadRingBufferPart(cmd, DMEM_WET_LEFT_CH, bufItem->startPos, bufItem->lengthA, reverb);
    if (bufItem->lengthB != 0) {
        // Ring buffer wrapped
        cmd = AudioSynth_LoadRingBufferPart(cmd, DMEM_WET_LEFT_CH + bufItem->lengthA, 0, bufItem->lengthB, reverb);
    }
    return cmd;
}

Acmd* func_80188AFC(Acmd* cmd, u16 arg1, u16 arg2, s32 arg3) {
    s32 temp_t0;
    s32 temp_v0;
    s32 temp_t2;

    temp_v0 = arg2 + arg3;
    temp_t2 = temp_v0 & 0xF;

    if (temp_t2 != 0) {
        aLoadBuffer(cmd++, (temp_v0 - temp_t2), 0x3B0, 0x10);
        aDMEMMove(cmd++, arg1, 0x3C0, arg2);
        aDMEMMove(cmd++, temp_t2 + 0x3B0, (arg2 + 0x3C0), 0x10 - temp_t2);

        arg2 += (0x10 - temp_t2);
        arg1 = 0x3C0;
    }

    temp_t0 = arg3 & 0xF;

    if (temp_t0 != 0) {
        aLoadBuffer(cmd++, arg3 - temp_t0, 0x3B0, 0x10);
        aDMEMMove(cmd++, arg1, (temp_t0 + 0x3B0), arg2);

        arg2 += temp_t0;
        arg1 = 0x3B0;
    }

    aSaveBuffer(cmd++, arg1, (arg3 - temp_t0), arg2);

    return cmd;
}

Acmd* AudioSynth_LoadRingBufferPart(Acmd* cmd, u16 dmem, u16 startPos, s32 length, SynthesisReverb* reverb) {
    aLoadBuffer(cmd++, &reverb->leftRingBuf[startPos], dmem, length);
    aLoadBuffer(cmd++, &reverb->rightRingBuf[startPos], dmem + DEFAULT_LEN_1CH, length);
    return cmd;
}

Acmd* AudioSynth_SaveRingBufferPart(Acmd* cmd, u16 dmem, u16 startPos, s32 length, SynthesisReverb* reverb) {
    aSaveBuffer(cmd++, dmem, &reverb->leftRingBuf[startPos], length);
    aSaveBuffer(cmd++, dmem + DEFAULT_LEN_1CH, &reverb->rightRingBuf[startPos], length);
    return cmd;
}

void func_80188D20(void) {
}

Acmd* AudioSynth_MaybeLoadRingBuffer2(Acmd* cmd, s32 arg1, SynthesisReverb* reverb, s16 bufIndex) {
    if (reverb->downsampleRate == 1) {
        cmd = AudioSynth_LoadRingBuffer2(cmd, arg1, reverb, bufIndex);
    }

    return cmd;
}

// OoT func_800DC164
Acmd* func_80188D68(Acmd* cmd, s32 arg1, SynthesisReverb* reverb, s16 arg3) {
    // Sets DMEM_WET_{LEFT,RIGHT}_CH, clobbers DMEM_TEMP
    if (reverb->downsampleRate == 1) {
        if (reverb->unk_18 != 0) {
            cmd = func_80188698(cmd, arg1, reverb, arg3);
        } else {
            cmd = AudioSynth_LoadRingBuffer1(cmd, arg1, reverb, arg3);
        }
    } else {
        cmd = func_80188304(cmd, arg1, reverb, arg3);
    }
    return cmd;
}

Acmd* AudioSynth_SaveReverbSamples(Acmd* cmd, SynthesisReverb* reverb, s16 bufIndex) {
    ReverbRingBufferItem* bufItem = &reverb->items[reverb->curFrame][bufIndex];
    s32 i;
    s32 phi_a2;

    if (reverb->downsampleRate == 1) {
        if (reverb->unk_18 != 0) {
            cmd = func_801884A0(cmd, reverb, bufIndex);
        } else {
            // Put the oldest samples in the ring buffer into the wet channels
            cmd = AudioSynth_SaveRingBufferPart(cmd, DMEM_WET_LEFT_CH, bufItem->startPos, bufItem->lengthA, reverb);
            if (bufItem->lengthB != 0) {
                // Ring buffer wrapped
                cmd = AudioSynth_SaveRingBufferPart(cmd, DMEM_WET_LEFT_CH + bufItem->lengthA, 0, bufItem->lengthB,
                                                    reverb);
            }
        }
    } else {
        if (1) {}

        i = reverb->downsampleRate;
        phi_a2 = 0xD0;
        for (; i >= 2;) {
            aInterl(cmd++, 0xC70, 0xC70, phi_a2);
            aInterl(cmd++, 0xE10, 0xE10, phi_a2);
            i >>= 1;
            phi_a2 >>= 1;
        }

        if (bufItem->lengthA != 0) {
            cmd = func_80188AFC(cmd, 0xC70, bufItem->lengthA, &reverb->leftRingBuf[bufItem->startPos]);
            cmd = func_80188AFC(cmd, 0xE10, bufItem->lengthA, &reverb->rightRingBuf[bufItem->startPos]);
        }

        if (bufItem->lengthB != 0) {
            cmd = func_80188AFC(cmd, bufItem->lengthA + 0xC70, bufItem->lengthB, reverb->leftRingBuf);
            cmd = func_80188AFC(cmd, bufItem->lengthA + 0xE10, bufItem->lengthB, reverb->rightRingBuf);
        }
    }

    reverb->resampleFlags = 0;
    return cmd;
}

Acmd* AudioSynth_SaveRingBuffer2(Acmd* cmd, SynthesisReverb* reverb, s16 bufIndex) {
    ReverbRingBufferItem* bufItem = &reverb->items2[reverb->curFrame][bufIndex];

    cmd = AudioSynth_SaveRingBufferPart(cmd, DMEM_WET_LEFT_CH, bufItem->startPos, bufItem->lengthA, reverb);
    if (bufItem->lengthB != 0) {
        // Ring buffer wrapped
        cmd = AudioSynth_SaveRingBufferPart(cmd, DMEM_WET_LEFT_CH + bufItem->lengthA, 0, bufItem->lengthB, reverb);
    }
    return cmd;
}

#ifdef NON_EQUIVALENT
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
            if (noteSubEu->bitField0.enabled &&
                noteSubEu->bitField1.reverbIndex >= gAudioContext.numSynthesisReverbs) {
                noteIndices[count++] = i;
            }
        }
    }

    aClearBuffer(cmd++, DMEM_LEFT_CH, DEFAULT_LEN_2CH);
    i = 0;
    for (reverbIndex = 0; reverbIndex < gAudioContext.numSynthesisReverbs; reverbIndex++) {
        reverb = &gAudioContext.synthesisReverbs[reverbIndex];
        useReverb = reverb->useReverb;
        if (useReverb) {
            cmd = func_80188D68(cmd, aiBufLen, reverb, updateIndex);
            aMix(cmd++, 0x34, reverb->unk_0A, DMEM_WET_LEFT_CH, DMEM_LEFT_CH);
            unk14 = reverb->unk_14;
            if (unk14) {
                aDMEMMove(cmd++, DMEM_WET_LEFT_CH, DMEM_WET_TEMP, DEFAULT_LEN_2CH);
            }

            aMix(cmd++, 0x34, reverb->unk_0C + 0x8000, DMEM_WET_LEFT_CH, DMEM_WET_LEFT_CH);
            if (reverb->leakRtl != 0 || reverb->leakLtr != 0) {
                cmd = AudioSynth_LeakReverb(cmd, reverb);
            }

            if (unk14) {
                cmd = AudioSynth_SaveReverbSamples(cmd, reverb, updateIndex);
                if (reverb->unk_05 != -1) {
                    cmd = AudioSynth_MaybeMixRingBuffer1(cmd, reverb, updateIndex);
                }
                cmd = AudioSynth_MaybeLoadRingBuffer2(cmd, aiBufLen, reverb, updateIndex);
                aMix(cmd++, 0x34, reverb->unk_16, DMEM_WET_TEMP, DMEM_WET_LEFT_CH);
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
            if (reverb->filterLeft != NULL || reverb->filterRight != NULL) {
                cmd = AudioSynth_FilterReverb(cmd, aiBufLen * 2, reverb);
            }
            if (unk14) {
                cmd = AudioSynth_SaveRingBuffer2(cmd, reverb, updateIndex);
            } else {
                cmd = AudioSynth_SaveReverbSamples(cmd, reverb, updateIndex);
                if (reverb->unk_05 != -1) { 
                    cmd = AudioSynth_MaybeMixRingBuffer1(cmd, reverb, updateIndex);
                }
            }
        }
    }

    while (i < count) {
        cmd = AudioSynth_ProcessNote(noteIndices[i], &gAudioContext.noteSubsEu[t + noteIndices[i]],
                                     &gAudioContext.notes[noteIndices[i]].synthesisState, aiBuf, aiBufLen, cmd,
                                     updateIndex);
        i++;
    }

    updateIndex = aiBufLen * 2;
    aInterleave(cmd++, DMEM_TEMP, DMEM_LEFT_CH, DMEM_RIGHT_CH, updateIndex);
    if (D_80208E74 != NULL) {
        cmd = D_80208E74();
    }
    aSaveBuffer(cmd++, DMEM_TEMP, aiBuf, updateIndex * 2);

    return cmd;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_synthesis/AudioSynth_DoOneAudioUpdate.s")
#endif

Acmd* AudioSynth_ProcessNote(s32 noteIndex, NoteSubEu* noteSubEu, NoteSynthesisState* synthState, s16* aiBuf,
                             s32 aiBufLen, Acmd* cmd, s32 updateIndex) {
    s32 pad1[2];
    s32 phi_a3;
    SoundFontSample* audioFontSample;
    AdpcmLoop* loopInfo;
    s32 nSamplesUntilLoopEnd;
    s32 nSamplesInThisIteration;
    s32 noteFinished;
    s32 restart;
    s32 flags;
    u16 resamplingRateFixedPoint;
    s32 phi_a1_2;
    s32 frameIndex;
    s32 skipBytes;
    s32 temp_v1_6;
    void* buf;
    s32 nSamplesToDecode;
    u32 sampleAddr;
    u32 samplesLenFixedPoint;
    s32 samplesLenAdjusted;
    s32 nSamplesProcessed;
    s32 loopEndPos;
    s32 nSamplesToProcess;
    s32 phi_s4;
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
    s32 sampleDataStart;
    u8* sampleData;
    s32 nParts;
    s32 curPart;
    s32 sampleDataStartPad;
    s32 side;
    s32 resampledTempLen;
    u16 noteSamplesDmemAddrBeforeResampling;
    s32 sampleDataOffset;
    s32 thing;
    s32 s5;
    Note* note;
    u32 nSamplesToLoad;
    u16 unk7;
    u16 unkE;
    s16* filter;
    s32 bookOffset;
    s32 finished;
    s32 aligned;
    s16 addr;

    bookOffset = noteSubEu->bitField1.bookOffset;
    finished = noteSubEu->bitField0.finished;
    note = &gAudioContext.notes[noteIndex];
    flags = A_CONTINUE;

    if (noteSubEu->bitField0.needsInit == true) {
        flags = A_INIT;
        synthState->restart_bit0 = 0;
        synthState->restart_bit1 = 0;
        synthState->samplePosInt = note->unk_BC;
        synthState->samplePosFrac = 0;
        synthState->curVolLeft = 0;
        synthState->curVolRight = 0;
        synthState->prevHeadsetPanRight = 0;
        synthState->prevHeadsetPanLeft = 0;
        synthState->reverbVol = noteSubEu->reverbVol;
        synthState->numParts = 0;
        synthState->unk_1A = 1;
        note->noteSubEu.bitField0.finished = false;
        synthState->unk_1F = note->unk_B0[4]; 
        finished = false;
    }

    resamplingRateFixedPoint = noteSubEu->resamplingRateFixedPoint;
    nParts = noteSubEu->bitField1.hasTwoParts + 1;
    samplesLenFixedPoint = (resamplingRateFixedPoint * aiBufLen * 2) + synthState->samplePosFrac;
    nSamplesToLoad = samplesLenFixedPoint >> 16;

    if (nSamplesToLoad == 0) {
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
        cmd = AudioSynth_LoadWaveSamples(cmd, noteSubEu, synthState, nSamplesToLoad);
        noteSamplesDmemAddrBeforeResampling = DMEM_UNCOMPRESSED_NOTE + (synthState->samplePosInt * 2);
        synthState->samplePosInt += nSamplesToLoad;
    } else {
        audioFontSample = noteSubEu->sound.soundFontSound->sample;
        loopInfo = audioFontSample->loop;

        if (note->playbackState.unk_04 != 0) {
            synthState->restart_bit1 = 1;
        }

        if ((loopInfo->count == 2) && synthState->restart_bit1) {
            loopEndPos = loopInfo->unk_0C;
        } else {
            loopEndPos = loopInfo->end;
        }

        sampleAddr = audioFontSample->sampleAddr;
        resampledTempLen = 0;

        for (curPart = 0; curPart < nParts; curPart++) {
            nSamplesProcessed = 0;
            s5 = 0;

            if (nParts == 1) {
                samplesLenAdjusted = nSamplesToLoad;
            } else if (nSamplesToLoad & 1) {
                samplesLenAdjusted = (nSamplesToLoad & ~1) + (curPart * 2);
            } else {
                samplesLenAdjusted = nSamplesToLoad;
            }

            if (audioFontSample->codec == CODEC_ADPCM || audioFontSample->codec == CODEC_SMALL_ADPCM) {
                if (gAudioContext.curLoadedBook != audioFontSample->book->book) {
                    u32 nEntries;
                    switch (bookOffset) {
                        case 1:
                            gAudioContext.curLoadedBook = D_801D58AA;
                            break;
                        case 2:
                        case 3:
                        default:
                            gAudioContext.curLoadedBook = audioFontSample->book->book;
                            break;
                    }
                    nEntries = 16 * audioFontSample->book->order * audioFontSample->book->npredictors;
                    aLoadADPCM(cmd++, nEntries, gAudioContext.curLoadedBook);
                }
            }

            while (nSamplesProcessed != samplesLenAdjusted) {
                noteFinished = false;
                restart = false;
                phi_s4 = 0;

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
                            restart = true;
                        }
                    } else {
                        noteFinished = true;
                    }
                }

                switch (audioFontSample->codec) {
                    case CODEC_ADPCM:
                        frameSize = 9;
                        skipInitialSamples = 16;
                        sampleDataStart = 0;
                        break;
                    case CODEC_SMALL_ADPCM:
                        frameSize = 5;
                        skipInitialSamples = 16;
                        sampleDataStart = 0;
                        break;
                    case 7:
                        frameSize = 4;
                        skipInitialSamples = 16;
                        sampleDataStart = 0;
                        break;
                    case CODEC_S8:
                        frameSize = 16;
                        skipInitialSamples = 16;
                        sampleDataStart = 0;
                        break;
                    case 4:                       
                        phi_a3 = -1;
                        if (D_80208E70 != NULL) {
                            phi_a3 = D_80208E70(audioFontSample, samplesLenAdjusted, flags, noteIndex);
                        } 

                        if (phi_a3 == -1) {
                            noteFinished = true;
                        } else if (phi_a3 == 0) {
                            return cmd;
                        } else {
                            AudioSynth_LoadBuffer(cmd++, 0x570, (samplesLenAdjusted * 2) + 0x20, phi_a3);
                            flags = A_CONTINUE;
                            skipBytes = 0;
                            nSamplesProcessed = samplesLenAdjusted;
                            s5 = samplesLenAdjusted;
                        }

                        goto skip;

                    case CODEC_S16_INMEMORY:
                    case 6:
                        AudioSynth_ClearBuffer(cmd++, DMEM_UNCOMPRESSED_NOTE, (samplesLenAdjusted * 2) + 0x20);
                        flags = A_CONTINUE;
                        skipBytes = 0;
                        nSamplesProcessed = samplesLenAdjusted;
                        s5 = samplesLenAdjusted;
                        goto skip;
                    case CODEC_S16:
                        AudioSynth_ClearBuffer(cmd++, DMEM_UNCOMPRESSED_NOTE, (samplesLenAdjusted * 2) + 0x20);
                        flags = A_CONTINUE;
                        skipBytes = 0;
                        nSamplesProcessed = samplesLenAdjusted;
                        s5 = samplesLenAdjusted;
                        goto skip;
                    default:
                        break;
                }

                if (nFramesToDecode != 0) {
                    frameIndex = (synthState->samplePosInt + skipInitialSamples - nFirstFrameSamplesToIgnore) / 16;
                    sampleDataOffset = (frameIndex * frameSize);
                    if (audioFontSample->medium == MEDIUM_RAM) {
                        sampleData = (u8*)(sampleDataStart + sampleDataOffset + sampleAddr);
                    } else if (gAudioContext.unk_29B8 != 0) {
                        return cmd;
                    } else if (audioFontSample->medium == MEDIUM_UNK1) {
                        return cmd;
                    } else {
                        sampleData =
                            AudioLoad_DmaSampleData(sampleDataStart + sampleDataOffset + sampleAddr, ALIGN16((nFramesToDecode * frameSize) + 0x10),
                                                flags, &synthState->sampleDmaIndex, audioFontSample->medium);
                    }

                    if (sampleData == NULL) {
                        return cmd;
                    }

                    sampleDataStartPad = (u32)sampleData & 0xF;
                    aligned = ALIGN16((nFramesToDecode * frameSize) + 16);
                    addr = DMEM_COMPRESSED_ADPCM_DATA - aligned;
                    aLoadBuffer(cmd++, sampleData - sampleDataStartPad, addr, aligned);
                } else {
                    nSamplesToDecode = 0;
                    sampleDataStartPad = 0;
                }

                if (synthState->restart_bit0) {
                    aSetLoop(cmd++, audioFontSample->loop->state);
                    flags = A_LOOP;
                    synthState->restart_bit0 = false;
                }

                nSamplesInThisIteration = nSamplesToDecode + nSamplesInFirstFrame - nTrailingSamplesToIgnore;
                if (nSamplesProcessed == 0) {
                    if (1) {}
                    skipBytes = nFirstFrameSamplesToIgnore * 2;
                } else {
                    phi_s4 = ALIGN16(s5 + 16);
                }
                switch (audioFontSample->codec) {
                    case CODEC_ADPCM:
                        aligned = ALIGN16((nFramesToDecode * frameSize) + 0x10);
                        addr = DMEM_COMPRESSED_ADPCM_DATA - aligned;
                        aSetBuffer(cmd++, 0, addr + sampleDataStartPad, DMEM_UNCOMPRESSED_NOTE + phi_s4, nSamplesToDecode * 2);
                        aADPCMdec(cmd++, flags, synthState->synthesisBuffers->adpcmdecState);
                        break;
                    case CODEC_SMALL_ADPCM:
                        aligned = ALIGN16((nFramesToDecode * frameSize) + 0x10);
                        addr = DMEM_COMPRESSED_ADPCM_DATA - aligned;
                        aSetBuffer(cmd++, 0, addr + sampleDataStartPad, DMEM_UNCOMPRESSED_NOTE + phi_s4, nSamplesToDecode * 2);
                        aADPCMdec(cmd++, flags | 4, synthState->synthesisBuffers->adpcmdecState);
                        break;
                    case CODEC_S8:
                        aligned = ALIGN16((nFramesToDecode * frameSize) + 0x10);
                        addr = DMEM_COMPRESSED_ADPCM_DATA - aligned;
                        AudioSynth_SetBuffer(cmd++, 0, addr + sampleDataStartPad, DMEM_UNCOMPRESSED_NOTE + phi_s4, nSamplesToDecode * 2);
                        AudioSynth_S8Dec(cmd++, flags, synthState->synthesisBuffers->adpcmdecState);
                        break;
                    case 7:
                        break;
                }

                if (nSamplesProcessed != 0) {
                    aDMEMMove(cmd++, DMEM_UNCOMPRESSED_NOTE + phi_s4 + (nFirstFrameSamplesToIgnore * 2), DMEM_UNCOMPRESSED_NOTE + s5,
                              nSamplesInThisIteration * 2);
                }

                nSamplesProcessed += nSamplesInThisIteration;

                switch (flags) {
                    case A_INIT:
                        skipBytes = 0x20;
                        s5 = (nSamplesToDecode + 0x10) * 2;
                        break;
                    case A_LOOP:
                        s5 = nSamplesInThisIteration * 2 + s5;
                        break;
                    default:
                        if (s5 != 0) {
                            s5 = nSamplesInThisIteration * 2 + s5;
                        } else {
                            s5 = (nFirstFrameSamplesToIgnore + nSamplesInThisIteration) * 2;
                        }
                        break;
                }

                flags = A_CONTINUE;

            skip:
                if (noteFinished) {
                    if ((samplesLenAdjusted - nSamplesProcessed) != 0) {
                        AudioSynth_ClearBuffer(cmd++, DMEM_UNCOMPRESSED_NOTE + s5,
                                            (samplesLenAdjusted - nSamplesProcessed) * 2);
                    }
                    finished = true;
                    note->noteSubEu.bitField0.finished = true;
                    func_80187DE8(updateIndex, noteIndex);
                    break;
                } else {
                    if (restart) {
                        synthState->restart_bit0 = true;
                        synthState->samplePosInt = loopInfo->start;
                    } else {
                        synthState->samplePosInt += nSamplesToProcess;
                    }
                }
            }

            switch (nParts) {
                case 1:
                    noteSamplesDmemAddrBeforeResampling = DMEM_UNCOMPRESSED_NOTE + skipBytes;
                    break;
                case 2:
                    switch (curPart) {
                        case 0:
                            AudioSynth_InterL(cmd++, DMEM_UNCOMPRESSED_NOTE + skipBytes, DMEM_TEMP + 0x20,
                                              ((samplesLenAdjusted / 2) + 7) & ~7);
                            resampledTempLen = samplesLenAdjusted;
                            noteSamplesDmemAddrBeforeResampling = DMEM_TEMP + 0x20;
                            if (finished) {
                                AudioSynth_ClearBuffer(cmd++, noteSamplesDmemAddrBeforeResampling + resampledTempLen,
                                                       samplesLenAdjusted + 0x10);
                            }
                            break;
                        case 1:
                            AudioSynth_InterL(cmd++, DMEM_UNCOMPRESSED_NOTE + skipBytes,
                                              DMEM_TEMP + 0x20 + resampledTempLen, ((samplesLenAdjusted / 2) + 7) & ~7);
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

    cmd = AudioSynth_FinalResample(cmd, synthState, aiBufLen * 2, resamplingRateFixedPoint,
                                   noteSamplesDmemAddrBeforeResampling, flags);
    if (bookOffset == 3) {
        func_801881F8(cmd++, DMEM_TEMP, DMEM_TEMP, aiBufLen * 2, 0);
    }

    phi_a1_2 = noteSubEu->unk_2;
    if (phi_a1_2 != 0) {
        if (phi_a1_2 < 0x10) {
            phi_a1_2 = 0x10;
        }
        AudioSynth_HiLoGain(cmd++, phi_a1_2, DMEM_TEMP, 0, (aiBufLen * 2) + 0x20);
    }

    filter = noteSubEu->filter;
    if (filter != 0) {
        AudioSynth_LoadFilterCount(cmd++, aiBufLen * 2, filter);
        AudioSynth_LoadFilter(cmd++, flags, DMEM_TEMP, synthState->synthesisBuffers->mixEnvelopeState);
    }

    unk7 = noteSubEu->unk_07;
    unkE = noteSubEu->unk_0E;
    buf = &synthState->synthesisBuffers->panSamplesBuffer[0x18];
    if (unk7 != 0 && noteSubEu->unk_0E != 0) {
        AudioSynth_DMemMove(cmd++, DMEM_TEMP, DMEM_SCRATCH2, aiBufLen * 2);
        thing = DMEM_SCRATCH2 - unk7;
        if (synthState->unk_1A != 0) {
            AudioSynth_ClearBuffer(cmd++, thing, unk7);
            synthState->unk_1A = 0;
        } else {
            AudioSynth_LoadBuffer(cmd++, thing, unk7, buf);
        }
        AudioSynth_SaveBuffer(cmd++, DMEM_TEMP + (aiBufLen * 2) - unk7, unk7, buf);
        AudioSynth_Mix(cmd++, (aiBufLen * 2) >> 4, unkE, DMEM_SCRATCH2, thing);
        AudioSynth_DMemMove(cmd++, thing, DMEM_TEMP, aiBufLen * 2);
    } else {
        synthState->unk_1A = 1;
    }

    if (noteSubEu->headsetPanRight != 0 || synthState->prevHeadsetPanRight != 0) {
        side = 1;
    } else if (noteSubEu->headsetPanLeft != 0 || synthState->prevHeadsetPanLeft != 0) {
        side = 2;
    } else {
        side = 0;
    }

    if (gAudioContext.soundMode == 4) {
            noteSubEu->targetVolLeft = noteSubEu->targetVolLeft >> 1;
            noteSubEu->targetVolRight = noteSubEu->targetVolRight >> 1;
            if (noteSubEu->unk_19 != 0xFF) {
                cmd = func_8018A4B4(cmd, noteSubEu, synthState, aiBufLen, 0x3B0, flags);
            }
        }

    cmd = AudioSynth_ProcessEnvelope(cmd, noteSubEu, synthState, aiBufLen, DMEM_TEMP, side, flags);
    if (noteSubEu->bitField1.usesHeadsetPanEffects2) {
        if (!(flags & A_INIT)) {
            flags = A_CONTINUE;
        }
        cmd = AudioSynth_NoteApplyHeadsetPanEffects(cmd, noteSubEu, synthState, aiBufLen * 2, flags, side);
    }
    return cmd;
}

// New MM Function?
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_synthesis/func_8018A4B4.s")

Acmd* AudioSynth_FinalResample(Acmd* cmd, NoteSynthesisState* synthState, s32 count, u16 pitch, u16 inpDmem,
                               s32 resampleFlags) {
    if (pitch == 0) {
        AudioSynth_ClearBuffer(cmd++, DMEM_TEMP, count);
    } else {
        aSetBuffer(cmd++, 0, inpDmem, DMEM_TEMP, count);
        aResample(cmd++, resampleFlags, pitch, synthState->synthesisBuffers->finalResampleState);
    }
    return cmd;
}

#ifdef NON_EQUIVALENT
extern u32 D_801D5FB4;
extern u32 D_801D5FB8;
extern u32 D_801D5FBC;
extern u32 D_801D5FC0;
Acmd* AudioSynth_ProcessEnvelope(Acmd* cmd, NoteSubEu* noteSubEu, NoteSynthesisState* synthState, s32 aiBufLen,
                                 u16 inBuf, s32 headsetPanSettings, s32 flags) {
    u32 phi_a1;
    u16 curVolLeft;
    u16 targetVolLeft;
    s32 phi_t1;
    s16 reverbVol;
    u16 curVolRight;
    s16 rampLeft;
    s16 rampRight;
    s16 rampReverb;
    s16 sourceReverbVol;
    u16 targetVolRight;
    s32 pad;

    curVolLeft = synthState->curVolLeft;
    targetVolLeft = noteSubEu->targetVolLeft;
    targetVolLeft <<= 4;
    reverbVol = noteSubEu->reverbVol;
    curVolRight = synthState->curVolRight;
    targetVolRight = noteSubEu->targetVolRight;
    targetVolRight <<= 4;

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

    if (noteSubEu->bitField1.usesHeadsetPanEffects2) {
        AudioSynth_ClearBuffer(cmd++, DMEM_NOTE_PAN_TEMP, DEFAULT_LEN_1CH);
        AudioSynth_EnvSetup1(cmd++, phi_t1 * 2, rampReverb, rampLeft, rampRight);
        AudioSynth_EnvSetup2(cmd++, curVolLeft, curVolRight);
        switch (headsetPanSettings) {
            case 1:
                phi_a1 = D_801D5FB8;
                break;
            case 2:
                phi_a1 = D_801D5FBC;
                break;
            default:
                phi_a1 = D_801D5FC0;
                break;
        }
    } else {
        aEnvSetup1(cmd++, phi_t1 * 2, rampReverb, rampLeft, rampRight);
        aEnvSetup2(cmd++, curVolLeft, curVolRight);
        phi_a1 = D_801D5FC0;
    }

    aEnvMixer(cmd++, inBuf, aiBufLen, (sourceReverbVol & 0x80) >> 7, noteSubEu->bitField0.stereoHeadsetEffects,
              noteSubEu->bitField0.usesHeadsetPanEffects, noteSubEu->bitField0.stereoStrongRight,
              noteSubEu->bitField0.stereoStrongLeft, phi_a1, D_801D5FB4);
    return cmd;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_synthesis/AudioSynth_ProcessEnvelope.s")
#endif

Acmd* AudioSynth_LoadWaveSamples(Acmd* cmd, NoteSubEu* noteSubEu, NoteSynthesisState* synthState, s32 nSamplesToLoad) {
    s32 temp_v0;
    s32 unk6 = noteSubEu->unk_06;
    s32 samplePosInt = synthState->samplePosInt;
    s32 repeats;

    if (noteSubEu->bitField1.bookOffset != 0) {
        AudioSynth_LoadBuffer(cmd++, DMEM_UNCOMPRESSED_NOTE, ALIGN16(nSamplesToLoad * 2), gWaveSamples[8]);
        gWaveSamples[8] += nSamplesToLoad * 2;
        return cmd;
    } else {
        aLoadBuffer(cmd++, noteSubEu->sound.samples, DMEM_UNCOMPRESSED_NOTE, 0x80);
        if (unk6 != 0) {
            samplePosInt = (samplePosInt * D_801D5FD4[unk6 >> 2]) / D_801D5FD4[unk6 & 3];
        }
        samplePosInt &= 0x3F;
        temp_v0 = 0x40 - samplePosInt;
        if (temp_v0 < nSamplesToLoad) {
            repeats = ((nSamplesToLoad - temp_v0 + 0x3F) / 0x40);
            if (repeats != 0) {
                aDuplicate(cmd++, repeats, DMEM_UNCOMPRESSED_NOTE, DMEM_UNCOMPRESSED_NOTE + 0x80, 0x80);
            }
        }
        synthState->samplePosInt = samplePosInt;
    }
    return cmd;
}

#ifdef NON_EQUIVALENT
Acmd* AudioSynth_NoteApplyHeadsetPanEffects(Acmd* cmd, NoteSubEu* noteSubEu, NoteSynthesisState* synthState, s32 bufLen,
                                            s32 flags, s32 side) {
    u16 dest;
    u16 pitch;
    u8 prevPanShift;
    u8 panShift;

    switch (side) {
        case 1:
            dest = DMEM_LEFT_CH;
            panShift = noteSubEu->headsetPanRight;
            prevPanShift = synthState->prevHeadsetPanRight;
            synthState->prevHeadsetPanLeft = 0;
            synthState->prevHeadsetPanRight = panShift;
            break;
        case 2:
            dest = DMEM_RIGHT_CH;
            panShift = noteSubEu->headsetPanLeft;
            prevPanShift = synthState->prevHeadsetPanLeft;
            synthState->prevHeadsetPanLeft = panShift;
            synthState->prevHeadsetPanRight = 0;
            break;
        default:
            return cmd;
    }

    if (flags != A_INIT) {
        // Slightly adjust the sample rate in order to fit a change in pan shift
        if (panShift != prevPanShift) {
            pitch = (((bufLen << 0xF) / 2) - 1) / ((bufLen + panShift - prevPanShift - 2) / 2);
            aSetBuffer(cmd++, 0, DMEM_NOTE_PAN_TEMP, DMEM_TEMP, bufLen + panShift - prevPanShift);
            aResampleZoh(cmd++, pitch, 0);
        } else {
            aDMEMMove(cmd++, DMEM_NOTE_PAN_TEMP, DMEM_TEMP, bufLen);
        }

        if (prevPanShift != 0) {
            aLoadBuffer(cmd++, &synthState->synthesisBuffers->panResampleState[0x8], DMEM_NOTE_PAN_TEMP,
                        ALIGN16(prevPanShift));
            aDMEMMove(cmd++, DMEM_TEMP, DMEM_NOTE_PAN_TEMP + prevPanShift, bufLen + panShift - prevPanShift);
        } else {
            aDMEMMove(cmd++, DMEM_TEMP, DMEM_NOTE_PAN_TEMP, bufLen + panShift);
        }
    } else {
        // Just shift right
        aDMEMMove(cmd++, DMEM_NOTE_PAN_TEMP, DMEM_TEMP, bufLen);
        aClearBuffer(cmd++, DMEM_NOTE_PAN_TEMP, panShift);
        aDMEMMove(cmd++, DMEM_TEMP, DMEM_NOTE_PAN_TEMP + panShift, bufLen);
    }

    if (panShift) {
        // Save excessive samples for next iteration
        aSaveBuffer(cmd++, DMEM_NOTE_PAN_TEMP + bufLen, &synthState->synthesisBuffers->panResampleState[0x8],
                    ALIGN16(panShift));
    }
    aAddMixer(cmd++, ALIGN64(bufLen), DMEM_NOTE_PAN_TEMP, dest, 0x7FFF);
    return cmd;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_synthesis/AudioSynth_NoteApplyHeadsetPanEffects.s")
#endif
