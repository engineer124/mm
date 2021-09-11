#include "global.h"

#ifdef NON_EQUIVALENT
f32 func_8018B0F0(f32 arg0) {
    return 256.0f * gAudioContext.audioBufferParameters.unkUpdatesPerFrameScaled / arg0;
    if (1) {}
}
#else
f32 func_8018B0F0(f32 arg0);
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018B0F0.s")
#endif

#ifdef NON_EQUIVALENT
void func_8018B10C(void) {
    s32 i;

    gAudioContext.unk_3520[255] = func_8018B0F0(0.25f);
    gAudioContext.unk_3520[254] = func_8018B0F0(0.33f);
    gAudioContext.unk_3520[253] = func_8018B0F0(0.5f);
    gAudioContext.unk_3520[252] = func_8018B0F0(0.66f);
    gAudioContext.unk_3520[251] = func_8018B0F0(0.75f);

    for (i = 128; i < 251; i++) {
        gAudioContext.unk_3520[i] = func_8018B0F0(251 - i);
    }

    for (i = 16; i < 128; i++) {
        gAudioContext.unk_3520[i] = func_8018B0F0(4 * (143 - i));
    }

    for (i = 1; i < 16; i++) {
        gAudioContext.unk_3520[i] = func_8018B0F0(60 * (23 - i));
    }

    gAudioContext.unk_3520[0] = 0.0f;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018B10C.s")
#endif

#ifdef NON_EQUIVALENT
void Audio_ResetLoadStatus(void) {
    s32 i;

    for (i = 0; i < 0x30; i++) {
        if (gAudioContext.bankLoadStatus[i] != 5) {
            gAudioContext.bankLoadStatus[i] = 0;
        }
    }

    for (i = 0; i < 0x30; i++) {
        if (gAudioContext.audioTableLoadStatus[i] != 5) {
            gAudioContext.audioTableLoadStatus[i] = 0;
        }
    }

    for (i = 0; i < 0x80; i++) {
        if (gAudioContext.seqLoadStatus[i] != 5) {
            gAudioContext.seqLoadStatus[i] = 0;
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/Audio_ResetLoadStatus.s")
#endif

#ifdef NON_EQUIVALENT
void Audio_DiscardBank(s32 bankId) {
    s32 i;

    for (i = 0; i < gAudioContext.maxSimultaneousNotes; i++) {
        Note* note = &gAudioContext.notes[i];

        if (note->playbackState.bankId == bankId) {
            if (note->playbackState.unk_04 == 0 && note->playbackState.priority != 0) {
                note->playbackState.parentLayer->enabled = false;
                note->playbackState.parentLayer->finished = true;
            }
            Audio_NoteDisable(note);
            Audio_AudioListRemove(&note->listItem);
            // Audio_AudioListPushBack
            func_80197E08(&gAudioContext.noteFreeLists.disabled, &note->listItem);
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/Audio_DiscardBank.s")
#endif

#ifdef NON_EQUIVALENT
void func_8018B3FC(s32 bankId) {
    s32 i;

    for (i = 0; i < gAudioContext.maxSimultaneousNotes; i++) {
        Note* note = &gAudioContext.notes[i];
        NotePlaybackState* state = &note->playbackState;

        if (state->bankId == bankId) {
            if (state->priority != 0 && state->adsr.action.s.state == ADSR_STATE_DECAY) {
                state->priority = 1;
                state->adsr.fadeOutVel = gAudioContext.audioBufferParameters.updatesPerFrameInv;
                state->adsr.action.s.release = true;
            }
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018B3FC.s")
#endif

#ifdef NON_EQUIVALENT
void Audio_DiscardSequence(s32 seqId) {
    s32 i;

    for (i = 0; i < gAudioContext.audioBufferParameters.numSequencePlayers; i++) {
        if (gAudioContext.seqPlayers[i].enabled && gAudioContext.seqPlayers[i].seqId == seqId) {
            Audio_SequencePlayerDisable(&gAudioContext.seqPlayers[i]);
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/Audio_DiscardSequence.s")
#endif

#ifdef NON_EQUIVALENT
void func_8018B4F8(void* mem, u32 size) {
    Audio_osWritebackDCache(mem, size);
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018B4F8.s")
#endif

#ifdef NON_EQUIVALENT
void* func_8018B520(SoundAllocPool* pool, u32 size) {
    void* ret = NULL;

    if (gAudioContext.unkPool.start != 0) {
        ret = Audio_AllocZeroed(&gAudioContext.unkPool, size);
    }
    if (ret == NULL) {
        ret = Audio_AllocZeroed(pool, size);
    }
    return ret;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018B520.s")
#endif

#ifdef NON_EQUIVALENT
void* func_8018B578(SoundAllocPool* pool, u32 size) {
    void* ret = NULL;

    if (gAudioContext.unkPool.start != NULL) {
        ret = Audio_Alloc(&gAudioContext.unkPool, size);
    }
    if (ret == NULL) {
        ret = Audio_Alloc(pool, size);
    }
    return ret;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018B578.s")
#endif

#ifdef NON_EQUIVALENT
void* Audio_AllocDmaMemory(SoundAllocPool* pool, u32 size) {
    void* ret;

    ret = Audio_Alloc(pool, size);
    if (ret != NULL) {
        func_8018B4F8(ret, size);
    }
    return ret;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/Audio_AllocDmaMemory.s")
#endif

#ifdef NON_EQUIVALENT
void* Audio_AllocDmaMemoryZeroed(SoundAllocPool* pool, u32 size) {
    void* ret;

    ret = Audio_AllocZeroed(pool, size);
    if (ret != NULL) {
        func_8018B4F8(ret, size);
    }
    return ret;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/Audio_AllocDmaMemoryZeroed.s")
#endif

void* Audio_AllocZeroed(SoundAllocPool* pool, u32 size) {
    u8* ret = Audio_Alloc(pool, size);
    u8* ptr;

    if (ret != NULL) {
        for (ptr = ret; ptr < pool->cur; ptr++) {
            *ptr = 0; 
        }
    }

    return ret;
    if (1) {}
}

// New MM Function
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018B69C.s")

void* Audio_Alloc(SoundAllocPool* pool, u32 size) {
    u32 aligned = ALIGN16(size);
    u8* ret = pool->cur;

    if (pool->start + pool->size >= pool->cur + aligned) {
        pool->cur += aligned;
    } else {
        return NULL;
    }
    pool->unused++;
    return ret;
}

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/Audio_SoundAllocPoolInit.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/Audio_PersistentPoolClear.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/Audio_TemporaryPoolClear.s")

void func_8018B7AC(SoundAllocPool* pool) {
    pool->unused = 0;
    pool->cur = pool->start;
    return;
    if (1) {}
}

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018B7BC.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/Audio_InitMainPools.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/Audio_SessionPoolsInit.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/Audio_SeqAndBankPoolInit.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/Audio_PersistentPoolsInit.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/Audio_TemporaryPoolsInit.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/Audio_AllocBankOrSeq.s")

#ifdef NON_EQUIVALENT
void* func_8018C380(s32 poolIdx, s32 arg1, s32 id) {
    void* ret;

    ret = func_8018D57C(poolIdx, id);
    if (ret != NULL) {
        return ret;
    }
    if (arg1 == 3) {
        return NULL;
    }
    return func_8018C3D8(poolIdx, arg1, id);
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018C380.s")
#endif

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018C3D8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018C4E4.s")

void func_8018C8B8(s16* filter) {
    s32 i;

    for (i = 0; i < 8; i++) {
        filter[i] = 0;
    }
}

void func_8018C8E8(s16* filter, s32 arg1) {
    s32 i;
    s16* ptr = &D_801D2E80[8 * arg1];

    for (i = 0; i < 8; i++) {
        filter[i] = ptr[i];
    }
}

void func_8018C93C(s16* filter, s32 arg1) {
    s32 i;
    s16* ptr = &D_801D2F80[8 * (arg1 - 1)];

    for (i = 0; i < 8; i++) {
        filter[i] = ptr[i];
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018C994.s")

#ifdef NON_EQUIVALENT
void func_8018CB70(SynthesisReverb* reverb) {
    return;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018CB70.s")
#endif

#ifdef NON_EQUIVALENT
void func_8018CB78(void) {
    s32 count;
    s32 i;
    s32 j;

    if (gAudioContext.audioBufferParameters.specUnk4 == 2) {
        count = 2;
    } else {
        count = 1;
    }

    for (i = 0; i < gAudioContext.numSynthesisReverbs; i++) {
        for (j = 0; j < count; j++) {
            func_800DF7BC(&gAudioContext.synthesisReverbs[i]);
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018CB78.s")
#endif

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018CC3C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/Audio_ResetStep.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/Audio_InitHeap.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018D57C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018D5D4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018D658.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018D6C8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018D760.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018DA50.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018DBC4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018DCB4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018DCF8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018DD98.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018DDD4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018DF24.s")

void func_8018DFE0(s32 id) {
    D_801FD120 = 0;
    func_8018E03C(0, id);
}

void func_8018E00C(s32 id) {
    D_801FD120 = 1;
    func_8018E03C(1, id);
}

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018E03C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018E2A8.s")

// New MM Function?
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018E344.s")

// New MM Function?
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018E8C8.s")
