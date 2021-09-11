#include "global.h"

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_playback/Audio_InitNoteSub.s")

#ifdef NON_MATCHING
void Audio_NoteSetResamplingRate(NoteSubEu* noteSubEu, f32 resamplingRateInput) {
    f32 resamplingRate = 0.0f;

    if (resamplingRateInput < 2.0f) {
        noteSubEu->bitField1.s.hasTwoAdpcmParts = false;

        if (1.99998f < resamplingRateInput) {
            resamplingRate = 1.99998f;
        } else {
            resamplingRate = resamplingRateInput;
        }

    } else {
        noteSubEu->bitField1.s.hasTwoAdpcmParts = true;
        if (3.99996f < resamplingRateInput) {
            resamplingRate = 1.99998f;
        } else {
            resamplingRate = resamplingRateInput * 0.5f;
        }
    }
    noteSubEu->resamplingRateFixedPoint = (s32)(resamplingRate * 32768.0f);
    return;
    if (1) {}
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_playback/Audio_NoteSetResamplingRate.s")
#endif

#ifdef NON_MATCHING
void Audio_NoteInit(Note* note) {
    if (note->playbackState.parentLayer->adsr.releaseRate == 0) {
        Audio_AdsrInit(&note->playbackState.adsr, note->playbackState.parentLayer->seqChannel->adsr.envelope,
                       &note->playbackState.adsrVolScale);
    } else {
        Audio_AdsrInit(&note->playbackState.adsr, note->playbackState.parentLayer->adsr.envelope,
                       &note->playbackState.adsrVolScale);
    }

    note->playbackState.unk_04 = 0;
    note->playbackState.adsr.action.s.state = ADSR_STATE_INITIAL;
    note->noteSubEu = gDefaultNoteSub;
    return;
    if (1) {}
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_playback/Audio_NoteInit.s")
#endif

#ifdef NON_MATCHING
void Audio_NoteDisable(Note* note) {
    if (note->noteSubEu.bitField0.s.needsInit == true) {
        note->noteSubEu.bitField0.s.needsInit = false;
    }
    note->playbackState.priority = 0;
    note->noteSubEu.bitField0.s.enabled = false; 
    note->playbackState.unk_04 = 0;
    note->noteSubEu.bitField0.s.finished = false;
    note->playbackState.parentLayer = NO_LAYER;
    note->playbackState.prevParentLayer = NO_LAYER;
    note->playbackState.adsr.action.s.state = ADSR_STATE_DISABLED;
    note->playbackState.adsr.current = 0;
    return;
    if (1) {}
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_playback/Audio_NoteDisable.s")
#endif

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_playback/Audio_ProcessNotes.s")

AudioBankSound* Audio_InstrumentGetAudioBankSound(Instrument* instrument, s32 semitone) {
    AudioBankSound* sound;

    if (semitone < instrument->normalRangeLo) {
        sound = &instrument->lowNotesSound;
    } else if (semitone <= instrument->normalRangeHi) {
        sound = &instrument->normalNotesSound;
    } else {
        sound = &instrument->highNotesSound;
    }
    return sound;
    if (1) {}
}

#ifdef NON_MATCHING
Instrument* Audio_GetInstrumentInner(s32 bankId, s32 instId) {
    Instrument* inst;

    if (bankId == 0xFF) {
        return NULL;
    }

    if (!Audio_IsBankLoadComplete(bankId)) {
        gAudioContext.audioErrorFlags = bankId + 0x10000000;
        return NULL;
    }

    if (instId >= gAudioContext.ctlEntries[bankId].numInstruments) {
        gAudioContext.audioErrorFlags = ((bankId << 8) + instId) + 0x3000000;
        return NULL;
    }

    inst = gAudioContext.ctlEntries[bankId].instruments[instId];
    if (inst == NULL) {
        gAudioContext.audioErrorFlags = ((bankId << 8) + instId) + 0x1000000;
        return inst;
    }

    return inst;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_playback/Audio_GetInstrumentInner.s")
#endif

#ifdef NON_MATCHING
Drum* Audio_GetDrum(s32 bankId, s32 drumId) {
    Drum* drum;

    if (bankId == 0xFF) {
        return NULL;
    }

    if (!Audio_IsBankLoadComplete(bankId)) {
        gAudioContext.audioErrorFlags = bankId + 0x10000000;
        return NULL;
    }

    if (drumId >= gAudioContext.ctlEntries[bankId].numDrums) {
        gAudioContext.audioErrorFlags = ((bankId << 8) + drumId) + 0x4000000;
        return NULL;
    }
    if ((u32)gAudioContext.ctlEntries[bankId].drums < 0x80000000U) {
        return NULL;
    }
    drum = gAudioContext.ctlEntries[bankId].drums[drumId];

    if (drum == NULL) {
        gAudioContext.audioErrorFlags = ((bankId << 8) + drumId) + 0x5000000;
    }

    return drum;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_playback/Audio_GetDrum.s")
#endif

#ifdef NON_MATCHING
AudioBankSound* Audio_GetSfx(s32 bankId, s32 sfxId) {
    AudioBankSound* sfx;

    if (bankId == 0xFF) {
        return NULL;
    }

    if (!Audio_IsBankLoadComplete(bankId)) {
        gAudioContext.audioErrorFlags = bankId + 0x10000000;
        return NULL;
    }

    if (sfxId >= gAudioContext.ctlEntries[bankId].numSfx) {
        gAudioContext.audioErrorFlags = ((bankId << 8) + sfxId) + 0x4000000;
        return NULL;
    }

    if ((u32)gAudioContext.ctlEntries[bankId].soundEffects < 0x80000000U) {
        return NULL;
    }

    sfx = &gAudioContext.ctlEntries[bankId].soundEffects[sfxId];

    if (sfx == NULL) {
        gAudioContext.audioErrorFlags = ((bankId << 8) + sfxId) + 0x5000000;
    }

    if (sfx->sample == NULL) {
        return NULL;
    }

    return sfx;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_playback/Audio_GetSfx.s")
#endif

#ifdef NON_MATCHING
s32 func_801957B4(s32 instrument, s32 bankId, s32 instId, void* arg3) {
    if (bankId == 0xFF) {
        return -1;
    }

    if (!Audio_IsBankLoadComplete(bankId)) {
        return -2;
    }

    switch (instrument) {
        case 0:
            if (instId >= gAudioContext.ctlEntries[bankId].numDrums) {
                return -3;
            }
            gAudioContext.ctlEntries[bankId].drums[instId] = arg3;
            break;

        case 1:
            if (instId >= gAudioContext.ctlEntries[bankId].numSfx) {
                return -3;
            }
            gAudioContext.ctlEntries[bankId].soundEffects[instId] = *(AudioBankSound*)arg3;
            break;

        default:
            if (instId >= gAudioContext.ctlEntries[bankId].numInstruments) {
                return -3;
            }
            gAudioContext.ctlEntries[bankId].instruments[instId] = arg3;
            break;
    }

    return 0;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_playback/func_801957B4.s")
#endif

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_playback/Audio_SeqChanLayerDecayRelease.s")

void Audio_SeqChanLayerNoteDecay(SequenceChannelLayer* seqLayer) {
    Audio_SeqChanLayerDecayRelease(seqLayer, ADSR_STATE_DECAY);
}

void Audio_SeqChanLayerNoteRelease(SequenceChannelLayer* seqLayer) {
    Audio_SeqChanLayerDecayRelease(seqLayer, ADSR_STATE_RELEASE);
}

#ifdef NON_MATCHING
s32 Audio_BuildSyntheticWave(Note* note, SequenceChannelLayer* seqLayer, s32 waveId) {
    f32 freqScale;
    f32 ratio;
    u8 sampleCountIndex;

    if (waveId < 128) {
        waveId = 128;
    }

    freqScale = seqLayer->freqScale;
    if (seqLayer->portamento.mode != 0 && 0.0f < seqLayer->portamento.extent) {
        freqScale *= (seqLayer->portamento.extent + 1.0f);
    }
    if (freqScale < 0.99999f) {
        sampleCountIndex = 0;
        ratio = 1.0465f;
    } else if (freqScale < 1.99999f) {
        sampleCountIndex = 1;
        ratio = 0.52325f;
    } else if (freqScale < 3.99999f) {
        sampleCountIndex = 2; 
        ratio = 0.26263f;
    } else {
        sampleCountIndex = 3;
        ratio = 0.13081f;
    }
    seqLayer->freqScale *= ratio;
    note->playbackState.waveId = waveId;
    note->playbackState.sampleCountIndex = sampleCountIndex;

    note->noteSubEu.sound.samples = &gWaveSamples[waveId - 128][sampleCountIndex * 64];

    return sampleCountIndex;
    if (1) {}
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_playback/Audio_BuildSyntheticWave.s")
#endif

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_playback/Audio_InitSyntheticWave.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_playback/Audio_InitNoteList.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_playback/Audio_InitNoteLists.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_playback/Audio_InitNoteFreeList.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_playback/Audio_NotePoolClear.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_playback/Audio_NotePoolFill.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_playback/Audio_AudioListPushFront.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_playback/Audio_AudioListRemove.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_playback/Audio_PopNodeWithValueLessEqual.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_playback/Audio_NoteInitForLayer.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_playback/func_801963E8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_playback/Audio_NoteReleaseAndTakeOwnership.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_playback/Audio_AllocNoteFromDisabled.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_playback/Audio_AllocNoteFromDecaying.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_playback/Audio_AllocNoteFromActive.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_playback/Audio_AllocNote.s")

#ifdef NON_MATCHING
void Audio_NoteInitAll(void) {
    Note* note;
    s32 i;

    for (i = 0; i < gAudioContext.maxSimultaneousNotes; i++) {
        note = &gAudioContext.notes[i];
        note->noteSubEu = gZeroNoteSub;
        note->playbackState.priority = 0;
        note->playbackState.unk_04 = 0;
        note->playbackState.parentLayer = NO_LAYER;
        note->playbackState.prevParentLayer = NO_LAYER;
        note->playbackState.wantedParentLayer = NO_LAYER;
        note->playbackState.waveId = 0;
        note->playbackState.attributes.velocity = 0.0f;
        note->playbackState.adsrVolScale = 0;
        note->playbackState.adsr.action.asByte = 0;
        note->vibratoState.active = 0;
        note->portamento.cur = 0;
        note->portamento.speed = 0;
        note->playbackState.stereoHeadsetEffects = false;
        note->unk_BC = 0;

        // Only one of these is right, likely the first
        note->synthesisState.synthesisBuffers = Audio_AllocDmaMemory(&gAudioContext.notesAndBuffersPool, 0x1E0);
        note->synthesisState.synthesisBuffers = Audio_AllocDmaMemory(&gAudioContext.notesAndBuffersPool, 0x10);
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_playback/Audio_NoteInitAll.s")
#endif
