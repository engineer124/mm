#include "global.h"

// OoT func_800DDE20
#ifdef NON_EQUIVALENT
f32 func_8018B0F0(f32 arg0) {
    return 256.0f * gAudioContext.audioBufferParameters.unkUpdatesPerFrameScaled / arg0;
    if (1) {}
}
#else
f32 func_8018B0F0(f32 arg0);
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018B0F0.s")
#endif

// OoT func_800DDE3C
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
            Audio_AudioListPushBack(&gAudioContext.noteFreeLists.disabled, &note->listItem);
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/Audio_DiscardBank.s")
#endif

// OoT func_800DE12C
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

// OoT func_800DE238
#ifdef NON_EQUIVALENT
void func_8018B4F8(void* mem, u32 size) {
    Audio_osWritebackDCache(mem, size);
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018B4F8.s")
#endif

// OoT func_800DE258
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

// OoT func_800DE2B0
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

// OoT func_800DE4A0
void func_8018B7AC(SoundAllocPool* pool) {
    pool->unused = 0;
    pool->cur = pool->start;
    return;
    if (1) {}
}

// OoT func_800DE4B0
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018B7BC.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/Audio_InitMainPools.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/Audio_SessionPoolsInit.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/Audio_SeqAndBankPoolInit.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/Audio_PersistentPoolsInit.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/Audio_TemporaryPoolsInit.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/Audio_AllocBankOrSeq.s")

// OoT func_800DF074
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

// OoT func_800DF0CC
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018C3D8.s")

// OoT func_800DF1D8
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018C4E4.s")

// OoT func_800DF5AC
void func_8018C8B8(s16* filter) {
    s32 i;

    for (i = 0; i < 8; i++) {
        filter[i] = 0;
    }
}
// OoT func_800DF5DC
void func_8018C8E8(s16* filter, s32 arg1) {
    s32 i;
    s16* ptr = &D_801D2E80[8 * arg1];

    for (i = 0; i < 8; i++) {
        filter[i] = ptr[i];
    }
}
// OoT func_800DF630
void func_8018C93C(s16* filter, s32 arg1) {
    s32 i;
    s16* ptr = &D_801D2F80[8 * (arg1 - 1)];

    for (i = 0; i < 8; i++) {
        filter[i] = ptr[i];
    }
}

// OoT func_800DF688
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018C994.s")

// OoT func_800DF7BC
#ifdef NON_EQUIVALENT
void func_8018CB70(SynthesisReverb* reverb) {
    return;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018CB70.s")
#endif

// OoT func_800DF7C4
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
            func_8018CB70(&gAudioContext.synthesisReverbs[i]);
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018CB78.s")
#endif

// OoT func_800DF888
#ifdef NON_EQUIVALENT
void func_8018CC3C(void) {
    s32 ind;
    s32 i;

    ind = gAudioContext.curAIBufIdx;
    gAudioContext.aiBufLengths[ind] = gAudioContext.audioBufferParameters.minAiBufferLength;

    for (i = 0; i < 0x580; i++) {
        gAudioContext.aiBuffers[ind][i] = 0;
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018CC3C.s")
#endif

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/Audio_ResetStep.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/Audio_InitHeap.s")

// OoT func_800E04E8
#ifdef NON_EQUIVALENT
void* func_8018D57C(s32 poolIdx, s32 id) {
    s32 i;

    for (i = 0; i < gAudioContext.unk_2D50.unused; i++) {
        if (gAudioContext.unk_2D60[i].poolIndex == poolIdx && gAudioContext.unk_2D60[i].id == id) {
            return gAudioContext.unk_2D60[i].ptr;
        }
    }
    return NULL;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018D57C.s")
#endif

// OoT func_800E0540
#ifdef NON_EQUIVALENT
void* func_8018D5D4(s32 poolIdx, s32 id, u32 size) {
    void* ret;
    s32 sp18;

    sp18 = gAudioContext.unk_2D50.unused;

    ret = Audio_Alloc(&gAudioContext.unk_2D50, size);
    gAudioContext.unk_2D60[sp18].ptr = ret;
    if (ret == NULL) {
        return NULL;
    }
    gAudioContext.unk_2D60[sp18].poolIndex = poolIdx;
    gAudioContext.unk_2D60[sp18].id = id;
    gAudioContext.unk_2D60[sp18].size = size;
    //! @bug UB: missing return. "ret" is in v0 at this point, but doing an
    // explicit return uses an additional register.
    // return ret;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018D5D4.s")
#endif

// OoT func_800E05C4
#ifdef NON_EQUIVALENT
void* func_8018D658(u32 size, s32 arg1, void* arg2, s8 arg3, s32 arg4) {
    UnkHeapEntry* entry;

    if (arg4 == 0) {
        entry = func_8018D760(size);
    } else {
        entry = func_800E0BF8(size);
    }
    if (entry != NULL) {
        entry->unk_02 = arg1;
        entry->unk_0C = arg2;
        entry->unk_01 = arg3;
        return entry->unk_08;
    }
    return NULL;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018D658.s")
#endif

// OoT func_800E0634
#ifdef NON_EQUIVALENT
void func_8018D6C8(u32 arg0, u32 arg1) {
    void* mem;

    mem = func_8018B578(&gAudioContext.notesAndBuffersPool, arg0);
    if (mem == NULL) {
        gAudioContext.unk_2EE0.pool.size = 0;
    } else {
        Audio_SoundAllocPoolInit(&gAudioContext.unk_2EE0.pool, mem, arg0);
    }
    mem = func_8018B578(&gAudioContext.notesAndBuffersPool, arg1);
    if (mem == NULL) {
        gAudioContext.unk_3174.pool.size = 0;
    } else {
        Audio_SoundAllocPoolInit(&gAudioContext.unk_3174.pool, mem, arg1);
    }
    gAudioContext.unk_2EE0.size = 0;
    gAudioContext.unk_3174.size = 0;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018D6C8.s")
#endif

// OoT func_800E06CC
#ifdef NON_EQUIVALENT
UnkHeapEntry* func_8018D760(u32 size) {
    u8* allocAfter;
    u8* allocBefore;
    void* mem;
    s32 index;
    s32 i;
    UnkHeapEntry* ret;
    AudioStruct0D68* item;
    UnkPool* unkPool;
    u8* start;
    u8* end;

    unkPool = &gAudioContext.unk_3174;
    allocBefore = unkPool->pool.cur;
    mem = Audio_Alloc(&unkPool->pool, size);
    if (mem == NULL) {
        u8* old = unkPool->pool.cur;
        unkPool->pool.cur = unkPool->pool.start;
        mem = Audio_Alloc(&unkPool->pool, size);
        if (mem == NULL) {
            unkPool->pool.cur = old;
            return NULL;
        }
        allocBefore = unkPool->pool.start;
    }

    allocAfter = unkPool->pool.cur;

    index = -1;
    for (i = 0; i < gAudioContext.unk_176C; i++) {
        item = &gAudioContext.unk_0D68[i];
        if (item->isFree == false) {
            start = item->ramAddr;
            end = item->ramAddr + item->sample->size - 1;

            if (end < allocBefore && start < allocBefore) {
                continue;
            }
            if (end >= allocAfter && start >= allocAfter) {
                continue;
            }

            // Overlap
            item->isFree = true;
        }
    }

    for (i = 0; i < unkPool->size; i++) {
        if (unkPool->entries[i].unk_00 == 0) {
            continue;
        }

        start = unkPool->entries[i].unk_08;
        end = start + unkPool->entries[i].size - 1;

        if (end < allocBefore && start < allocBefore) {
            continue;
        }
        if (end >= allocAfter && start >= allocAfter) {
            continue;
        }

        // Overlap. Discard existing entry?
        func_800E0AD8(&unkPool->entries[i]);
        if (index == -1) {
            index = i;
        }
    }

    if (index == -1) {
        index = unkPool->size++;
    }

    ret = &unkPool->entries[index];
    ret->unk_00 = 1;
    ret->unk_08 = mem;
    ret->size = size;
    return ret;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018D760.s")
#endif

// OoT func_800E0964
#ifdef NON_EQUIVALENT
void func_8018DA50(UnkHeapEntry* entry, s32 bankId) {
    Drum* drum;
    Instrument* inst;
    AudioBankSound* sfx;
    s32 instId;
    s32 drumId;
    s32 sfxId;

    for (instId = 0; instId < gAudioContext.ctlEntries[bankId].numInstruments; instId++) {
        inst = Audio_GetInstrumentInner(bankId, instId);
        if (inst != NULL) {
            if (inst->normalRangeLo != 0) {
                func_8018DCB4(entry, inst->lowNotesSound.sample);
            }
            if (inst->normalRangeHi != 0x7F) {
                func_8018DCB4(entry, inst->highNotesSound.sample);
            }
            func_8018DCB4(entry, inst->normalNotesSound.sample);
        }
    }

    for (drumId = 0; drumId < gAudioContext.ctlEntries[bankId].numDrums; drumId++) {
        drum = Audio_GetDrum(bankId, drumId);
        if (drum != NULL) {
            func_8018DCB4(entry, drum->sound.sample);
        }
    }

    for (sfxId = 0; sfxId < gAudioContext.ctlEntries[bankId].numSfx; sfxId++) {
        sfx = Audio_GetSfx(bankId, sfxId);
        if (sfx != NULL) {
            func_8018DCB4(entry, sfx->sample);
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018DA50.s")
#endif

// OoT func_800E0AD8
#ifdef NON_EQUIVALENT
void func_8018DBC4(UnkHeapEntry* entry) {
    s32 numBanks;
    s32 unk2;
    s32 unk3;
    s32 bankId;

    numBanks = gAudioContext.audioBankTable->header.entryCnt;
    for (bankId = 0; bankId < numBanks; bankId++) {
        unk2 = gAudioContext.ctlEntries[bankId].unk_02;
        unk3 = gAudioContext.ctlEntries[bankId].unk_03;
        if (((unk2 != 0xFF) && (entry->unk_02 == unk2)) || ((unk3 != 0xFF) && (entry->unk_02 == unk3)) ||
            entry->unk_02 == 0) {
            if (func_800DF074(1, 2, bankId) != NULL) {
                if (Audio_IsBankLoadComplete(bankId) != 0) {
                    func_800E0964(entry, bankId);
                }
            }
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018DBC4.s")
#endif

// OoT func_800E0E0C
void func_8018DCB4(UnkHeapEntry* entry, AudioBankSample* sample) {
    if (sample != NULL) {
        if (sample->sampleAddr == entry->unk_08) {
            sample->sampleAddr = entry->unk_0C;
            sample->medium = entry->unk_01;
        }
    }
}

// OoT func_800E0BF8
#ifdef NON_EQUIVALENT
UnkHeapEntry* func_8018DCF8(u32 size) {
    UnkPool* pool;
    UnkHeapEntry* entry;
    void* mem;

    pool = &gAudioContext.unk_2EE0;
    mem = Audio_Alloc(&pool->pool, size);
    if (mem == NULL) {
        return NULL;
    }
    entry = &pool->entries[pool->size];
    entry->unk_00 = 1;
    entry->unk_08 = mem;
    entry->size = size;
    pool->size++;
    return entry;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018DCF8.s")
#endif

// OoT func_800E0C80
void func_8018DD98(UnkHeapEntry* entry, s32 arg1, s32 arg2, s32 bankId) {
    if ((entry->unk_02 == arg1) || (entry->unk_02 == arg2) || (entry->unk_02 == 0)) {
        func_8018DA50(entry, bankId);
    }
}

// OoT func_800E0CBC
#ifdef NON_EQUIVALENT
void func_8018DDD4(void) {
    s32 numBanks;
    s32 unk2;
    s32 unk3;
    s32 bankId;
    s32 j;

    numBanks = gAudioContext.audioBankTable->header.entryCnt;
    for (bankId = 0; bankId < numBanks; bankId++) {
        unk2 = gAudioContext.ctlEntries[bankId].unk_02;
        unk3 = gAudioContext.ctlEntries[bankId].unk_03;
        if ((unk2 == 0xFF) && (unk3 == 0xFF)) {
            continue;
        }
        if (func_800DF074(1, 3, bankId) == NULL || !Audio_IsBankLoadComplete(bankId)) {
            continue;
        }

        for (j = 0; j < gAudioContext.unk_2EE0.size; j++) {
            func_800E0C80(&gAudioContext.unk_2EE0.entries[j], unk2, unk3, bankId);
        }
        for (j = 0; j < gAudioContext.unk_3174.size; j++) {
            func_800E0C80(&gAudioContext.unk_3174.entries[j], unk2, unk3, bankId);
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018DDD4.s")
#endif

// OoT func_800E0E0C
#ifdef NON_EQUIVALENT
typedef struct {
    u8* unk_0;
    u8* unk_4;
    u32 unk_8;
    u8 unk_C;
} Struct_8018DF24;

void func_8018DF24(Struct_8018DF24* arg0, AudioBankSample* sample) {
    if (sample != NULL) {
        u8* start = arg0->unk_0;
        u8* end = arg0->unk_0 + arg0->unk_8;
        u8* sampleAddr = sample->sampleAddr;
        if (start <= sampleAddr && sampleAddr < end) {
            sample->sampleAddr = sampleAddr - start + arg0->unk_4;
            sample->medium = arg0->unk_C & 0xFF;
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018DF24.s")
#endif

// OoT func_800E0E6C
void func_8018DFE0(s32 id) {
    D_801FD120 = 0;
    func_8018E03C(0, id);
}

// OoT func_800E0E90
void func_8018E00C(s32 id) {
    D_801FD120 = 1;
    func_8018E03C(1, id);
}

// func_800E0EB4
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018E03C.s")

// OoT func_800E1148
#ifdef NON_EQUIVALENT
void func_8018E2A8(void) {
    SoundMultiPool* pool;
    PersistentPool* persistent;
    TemporaryPool* temporary;
    u32 i;

    pool = &gAudioContext.unusedLoadedPool;
    temporary = &pool->temporary;

    if (temporary->entries[0].id != -1) {
        func_8018DFE0(temporary->entries[0].id);
    }

    if (temporary->entries[1].id != -1) {
        func_8018DFE0(temporary->entries[1].id);
    }

    persistent = &pool->persistent;
    for (i = 0; i < persistent->numEntries; i++) {
        func_8018DFE0(persistent->entries[i].id);
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018E2A8.s")
#endif

// New MM Function?
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018E344.s")

// New MM Function?
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018E8C8.s")
