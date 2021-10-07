#include "global.h"

// OoT func_800DDE20
f32 func_8018B0F0(f32 arg0) {
    return 256.0f * gAudioContext.audioBufferParameters.unkUpdatesPerFrameScaled / arg0;
}

// OoT func_800DDE3C
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

void AudioHeap_ResetLoadStatus(void) {
    s32 i;

    for (i = 0; i < 0x30; i++) {
        if (gAudioContext.bankLoadStatus[i] != 5) {
            gAudioContext.bankLoadStatus[i] = 0;
        }
    }

    for (i = 0; i < 0x30; i++) {
        if (gAudioContext.sampleBankLoadStatus[i] != 5) {
            gAudioContext.sampleBankLoadStatus[i] = 0;
        }
    }

    for (i = 0; i < 0x80; i++) {
        if (gAudioContext.seqLoadStatus[i] != 5) {
            gAudioContext.seqLoadStatus[i] = 0;
        }
    }
}

void AudioHeap_DiscardBank(s32 bankId) {
    s32 i;

    for (i = 0; i < gAudioContext.numNotes; i++) {
        Note* note = &gAudioContext.notes[i];

        if (note->playbackState.bankId == bankId) {
            if (note->playbackState.unk_04 == 0 && note->playbackState.priority != 0) {
                note->playbackState.parentLayer->enabled = false;
                note->playbackState.parentLayer->finished = true;
            }
            AudioPlayback_NoteDisable(note);
            AudioPlayback_AudioListRemove(&note->listItem);
            AudioSeq_AudioListPushBack(&gAudioContext.noteFreeLists.disabled, &note->listItem);
        }
    }
}

// OoT func_800DE12C
void func_8018B3FC(s32 bankId) {
    s32 i;

    for (i = 0; i < gAudioContext.numNotes; i++) {
        Note* note = &gAudioContext.notes[i];
        NotePlaybackState* state = &note->playbackState;

        if (state->bankId == bankId) {
            if (state->priority != 0) {
                state->priority = 1;
                state->adsr.fadeOutVel = gAudioContext.audioBufferParameters.updatesPerFrameInv;
                state->adsr.action.s.release = true;
            }
        }
    }
}

void AudioHeap_DiscardSequence(s32 seqId) {
    s32 i;

    for (i = 0; i < gAudioContext.audioBufferParameters.numSequencePlayers; i++) {
        if (gAudioContext.seqPlayers[i].enabled && gAudioContext.seqPlayers[i].seqId == seqId) {
            AudioSeq_SequencePlayerDisable(&gAudioContext.seqPlayers[i]);
        }
    }
}

// OoT func_800DE238
void* AudioHeap_WritebackDCache(void* mem, u32 size) {
    Audio_osWritebackDCache(mem, size);
    if (mem) {}
    return (void*)((u32)mem + 0x20000000);
}

// OoT func_800DE258
void* AudioHeap_AllocZeroedMaybeExternal(AudioAllocPool* pool, u32 size) {
    void* ret = NULL;

    if (gAudioContext.externalPool.start != 0) {
        ret = AudioHeap_AllocZeroed(&gAudioContext.externalPool, size);
    }
    if (ret == NULL) {
        ret = AudioHeap_AllocZeroed(pool, size);
    }
    return ret;
}

// OoT func_800DE2B0
void* AudioHeap_AllocMaybeExternal(AudioAllocPool* pool, u32 size) {
    void* ret = NULL;

    if (gAudioContext.externalPool.start != NULL) {
        ret = AudioHeap_Alloc(&gAudioContext.externalPool, size);
    }
    if (ret == NULL) {
        ret = AudioHeap_Alloc(pool, size);
    }
    return ret;
}

void* AudioHeap_AllocDmaMemory(AudioAllocPool* pool, u32 size) {
    void* ret;

    ret = AudioHeap_Alloc(pool, size);
    if (ret != NULL) {
        ret = AudioHeap_WritebackDCache(ret, size);
    }
    return ret;
}

void* AudioHeap_AllocDmaMemoryZeroed(AudioAllocPool* pool, u32 size) {
    void* ret;

    ret = AudioHeap_AllocZeroed(pool, size);
    if (ret != NULL) {
        ret = AudioHeap_WritebackDCache(ret, size);
    }
    return ret;
}

void* AudioHeap_AllocZeroed(AudioAllocPool* pool, u32 size) {
    u8* ret = AudioHeap_Alloc(pool, size);
    u8* ptr;

    if (ret != NULL) {
        for (ptr = ret; ptr < pool->cur; ptr++) {
            *ptr = 0; 
        }
    }

    return ret;
}

// New MM Function
void* func_8018B69C(AudioAllocPool* pool, u32 size) {
    u8* sp1C = pool->cur;
    void* ret = AudioHeap_Alloc(pool, size);

    if (ret != NULL) {
        pool->cur = sp1C;
        pool->count--;
    }
    return ret;
}

void* AudioHeap_Alloc(AudioAllocPool* pool, u32 size) {
    u32 aligned = ALIGN16(size);
    u8* ret = pool->cur;

    if (pool->start + pool->size >= pool->cur + aligned) {
        pool->cur += aligned;
    } else {
        return NULL;
    }
    pool->count++;
    return ret;
}

void AudioHeap_AllocPoolInit(AudioAllocPool* pool, void* mem, u32 size) {
    pool->cur = pool->start = (u8*)ALIGN16((u32)mem);
    pool->size = size - ((u32)mem & 0xF);
    pool->count = 0;
}

void AudioHeap_PersistentCacheClear(AudioPersistentCache* persistent) {
    persistent->pool.count = 0;
    persistent->numEntries = 0;
    persistent->pool.cur = persistent->pool.start;
}

void AudioHeap_TemporaryCacheClear(AudioTemporaryCache* temporary) {
    temporary->pool.count = 0;
    temporary->pool.cur = temporary->pool.start;
    temporary->nextSide = 0;
    temporary->entries[0].ptr = temporary->pool.start;
    temporary->entries[1].ptr = temporary->pool.start + temporary->pool.size;
    temporary->entries[0].id = -1;
    temporary->entries[1].id = -1;
}

// OoT func_800DE4A0
void AudioHeap_ResetPool(AudioAllocPool* pool) {
    pool->count = 0;
    pool->cur = pool->start;
}

// OoT func_800DE4B0
void AudioHeap_PopCache(s32 tableType) {
    AudioCache* loadedPool;
    AudioAllocPool* persistentPool;
    AudioPersistentCache* persistent;
    void* entryPtr;
    u8* table;

    switch (tableType) {
        case SEQUENCE_TABLE:
            loadedPool = &gAudioContext.seqCache;
            table = gAudioContext.seqLoadStatus;
            break;
        case BANK_TABLE:
            loadedPool = &gAudioContext.bankCache;
            table = gAudioContext.bankLoadStatus;
            break;
        case SAMPLE_TABLE:
            loadedPool = &gAudioContext.sampleBankCache;
            table = gAudioContext.sampleBankLoadStatus;
            break;
    }

    persistent = &loadedPool->persistent;
    persistentPool = &persistent->pool;

    if (persistent->numEntries == 0) {
        return;
    }

    entryPtr = persistent->entries[persistent->numEntries - 1].ptr;
    persistentPool->cur = entryPtr;
    persistentPool->count--;

    if (tableType == SAMPLE_TABLE) {
        AudioHeap_DiscardSampleBank(persistent->entries[persistent->numEntries - 1].id);
    }
    if (tableType == BANK_TABLE) {
        AudioHeap_DiscardBank(persistent->entries[persistent->numEntries - 1].id);
    }
    table[persistent->entries[persistent->numEntries - 1].id] = 0;
    persistent->numEntries--;
}

void AudioHeap_InitMainPools(s32 initPoolSize) {
    AudioHeap_AllocPoolInit(&gAudioContext.audioInitPool, gAudioContext.audioHeap, initPoolSize);
    AudioHeap_AllocPoolInit(&gAudioContext.audioSessionPool, gAudioContext.audioHeap + initPoolSize,
                             gAudioContext.audioHeapSize - initPoolSize);
    gAudioContext.externalPool.start = NULL;
}

void AudioHeap_SessionPoolsInit(AudioPoolSplit4* split) {
    gAudioContext.audioSessionPool.cur = gAudioContext.audioSessionPool.start;
    AudioHeap_AllocPoolInit(&gAudioContext.notesAndBuffersPool,
                             AudioHeap_Alloc(&gAudioContext.audioSessionPool, split->wantSeq), split->wantSeq);
    AudioHeap_AllocPoolInit(&gAudioContext.cachePool,
                             AudioHeap_Alloc(&gAudioContext.audioSessionPool, split->wantCustom), split->wantCustom);
}

void AudioHeap_CachePoolInit(AudioPoolSplit2* split) {
    gAudioContext.cachePool.cur = gAudioContext.cachePool.start;
    AudioHeap_AllocPoolInit(&gAudioContext.persistentCommonPool,
                             AudioHeap_Alloc(&gAudioContext.cachePool, split->wantPersistent), split->wantPersistent);
    AudioHeap_AllocPoolInit(&gAudioContext.temporaryCommonPool,
                             AudioHeap_Alloc(&gAudioContext.cachePool, split->wantTemporary), split->wantTemporary);
}

void AudioHeap_PersistentCachesInit(AudioPoolSplit3* split) {
    gAudioContext.persistentCommonPool.cur = gAudioContext.persistentCommonPool.start;
    AudioHeap_AllocPoolInit(&gAudioContext.seqCache.persistent.pool,
                             AudioHeap_Alloc(&gAudioContext.persistentCommonPool, split->wantSeq), split->wantSeq);
    AudioHeap_AllocPoolInit(&gAudioContext.bankCache.persistent.pool,
                             AudioHeap_Alloc(&gAudioContext.persistentCommonPool, split->wantBank), split->wantBank);
    AudioHeap_AllocPoolInit(&gAudioContext.sampleBankCache.persistent.pool,
                             AudioHeap_Alloc(&gAudioContext.persistentCommonPool, split->wantSample), split->wantSample);
    AudioHeap_PersistentCacheClear(&gAudioContext.seqCache.persistent);
    AudioHeap_PersistentCacheClear(&gAudioContext.bankCache.persistent);
    AudioHeap_PersistentCacheClear(&gAudioContext.sampleBankCache.persistent);
}

void AudioHeap_TemporaryCachesInit(AudioPoolSplit3* split) {
    gAudioContext.temporaryCommonPool.cur = gAudioContext.temporaryCommonPool.start;
    AudioHeap_AllocPoolInit(&gAudioContext.seqCache.temporary.pool,
                             AudioHeap_Alloc(&gAudioContext.temporaryCommonPool, split->wantSeq), split->wantSeq);
    AudioHeap_AllocPoolInit(&gAudioContext.bankCache.temporary.pool,
                             AudioHeap_Alloc(&gAudioContext.temporaryCommonPool, split->wantBank), split->wantBank);
    AudioHeap_AllocPoolInit(&gAudioContext.sampleBankCache.temporary.pool,
                             AudioHeap_Alloc(&gAudioContext.temporaryCommonPool, split->wantSample), split->wantSample);
    AudioHeap_TemporaryCacheClear(&gAudioContext.seqCache.temporary);
    AudioHeap_TemporaryCacheClear(&gAudioContext.bankCache.temporary);
    AudioHeap_TemporaryCacheClear(&gAudioContext.sampleBankCache.temporary);
}

void* AudioHeap_AllocCached(s32 tableType, s32 size, s32 where, s32 id) {
    AudioCache* loadedPool;
    AudioTemporaryCache* tp;
    AudioAllocPool* pool;
    void* mem;
    void* ret;
    u8 firstVal;
    u8 secondVal;
    s32 i;
    u8* table;
    s32 side;

    switch (tableType) {
        case SEQUENCE_TABLE:
            loadedPool = &gAudioContext.seqCache;
            table = gAudioContext.seqLoadStatus;
            break;
        case BANK_TABLE:
            loadedPool = &gAudioContext.bankCache;
            table = gAudioContext.bankLoadStatus;
            break;
        case SAMPLE_TABLE:
            loadedPool = &gAudioContext.sampleBankCache;
            table = gAudioContext.sampleBankLoadStatus;
            break;
    }

    if (where == 0) {
        tp = &loadedPool->temporary;
        pool = &tp->pool;

        if (pool->size < size) {
            return NULL;
        }

        firstVal = (tp->entries[0].id == -1) ? 0 : table[tp->entries[0].id];
        secondVal = (tp->entries[1].id == -1) ? 0 : table[tp->entries[1].id];

        if (tableType == BANK_TABLE) {
            if (firstVal == 4) {
                for (i = 0; i < gAudioContext.numNotes; i++) {
                    if (gAudioContext.notes[i].playbackState.bankId == tp->entries[0].id &&
                        gAudioContext.notes[i].noteSubEu.bitField0.s.enabled != 0) {
                        break;
                    }
                }

                if (i == gAudioContext.numNotes) {
                    AudioLoad_SetBankLoadStatus(tp->entries[0].id, 3);
                    firstVal = 3;
                }
            }

            if (secondVal == 4) {
                for (i = 0; i < gAudioContext.numNotes; i++) {
                    if (gAudioContext.notes[i].playbackState.bankId == tp->entries[1].id &&
                        gAudioContext.notes[i].noteSubEu.bitField0.s.enabled != 0) {
                        break;
                    }
                }

                if (i == gAudioContext.numNotes) {
                    AudioLoad_SetBankLoadStatus(tp->entries[1].id, 3);
                    secondVal = 3;
                }
            }
        }

        if (firstVal == 0) {
            tp->nextSide = 0;
        } else if (secondVal == 0) {
            tp->nextSide = 1;
        } else if (firstVal == 3 && secondVal == 3) {
            // Use the opposite side from last time.
        } else if (firstVal == 3) {
            tp->nextSide = 0;
        } else if (secondVal == 3) {
            tp->nextSide = 1;
        } else {
            // Check if there is a side which isn't in active use, if so, evict that one.
            if (tableType == SEQUENCE_TABLE) {
                if (firstVal == 2) {
                    for (i = 0; i < gAudioContext.audioBufferParameters.numSequencePlayers; i++) {
                        if (gAudioContext.seqPlayers[i].enabled != 0 &&
                            gAudioContext.seqPlayers[i].seqId == tp->entries[0].id) {
                            break;
                        }
                    }

                    if (i == gAudioContext.audioBufferParameters.numSequencePlayers) {
                        tp->nextSide = 0;
                        goto done;
                    }
                }

                if (secondVal == 2) {
                    for (i = 0; i < gAudioContext.audioBufferParameters.numSequencePlayers; i++) {
                        if (gAudioContext.seqPlayers[i].enabled != 0 &&
                            gAudioContext.seqPlayers[i].seqId == tp->entries[1].id) {
                            break;
                        }
                    }

                    if (i == gAudioContext.audioBufferParameters.numSequencePlayers) {
                        tp->nextSide = 1;
                        goto done;
                    }
                }
            } else if (tableType == BANK_TABLE) {
                if (firstVal == 2) {
                    for (i = 0; i < gAudioContext.numNotes; i++) {
                        if (gAudioContext.notes[i].playbackState.bankId == tp->entries[0].id &&
                            gAudioContext.notes[i].noteSubEu.bitField0.s.enabled != 0) {
                            break;
                        }
                    }
                    if (i == gAudioContext.numNotes) {
                        tp->nextSide = 0;
                        goto done;
                    }
                }

                if (secondVal == 2) {
                    for (i = 0; i < gAudioContext.numNotes; i++) {
                        if (gAudioContext.notes[i].playbackState.bankId == tp->entries[1].id &&
                            gAudioContext.notes[i].noteSubEu.bitField0.s.enabled != 0) {
                            break;
                        }
                    }
                    if (i == gAudioContext.numNotes) {
                        tp->nextSide = 1;
                        goto done;
                    }
                }
            }

            // No such luck. Evict the side that wasn't chosen last time, except
            // if it is being loaded into.
            if (tp->nextSide == 0) {
                if (firstVal == 1) {
                    if (secondVal == 1) {
                        goto fail;
                    }
                    tp->nextSide = 1;
                }
            } else {
                if (secondVal == 1) {
                    if (firstVal == 1) {
                        goto fail;
                    }
                    tp->nextSide = 0;
                }
            }

            if (0) {
            fail:
                // Both sides are being loaded into.
                return NULL;
            }
        }
    done:

        side = tp->nextSide;

        if (tp->entries[side].id != -1) {
            if (tableType == SAMPLE_TABLE) {
                AudioHeap_DiscardSampleBank(tp->entries[side].id);
            }
            table[tp->entries[side].id] = 0;
            if (tableType == BANK_TABLE) {
                AudioHeap_DiscardBank(tp->entries[side].id);
            }
        }

        switch (side) {
            case 0:
                tp->entries[0].ptr = pool->start;
                tp->entries[0].id = id;
                tp->entries[0].size = size;
                pool->cur = pool->start + size;

                if (tp->entries[1].id != -1 && tp->entries[1].ptr < pool->cur) {
                    if (tableType == SAMPLE_TABLE) {
                        AudioHeap_DiscardSampleBank(tp->entries[1].id);
                    }

                    table[tp->entries[1].id] = 0;
                    switch (tableType) {
                        case SEQUENCE_TABLE:
                            AudioHeap_DiscardSequence((s32)tp->entries[1].id);
                            break;
                        case BANK_TABLE:
                            AudioHeap_DiscardBank((s32)tp->entries[1].id);
                            break;
                    }

                    tp->entries[1].id = -1;
                    tp->entries[1].ptr = pool->start + pool->size;
                }

                ret = tp->entries[0].ptr;
                break;

            case 1:
                tp->entries[1].ptr = (u8*)((u32)(pool->start + pool->size - size) & ~0xF);
                tp->entries[1].id = id;
                tp->entries[1].size = size;
                if (tp->entries[0].id != -1 && tp->entries[1].ptr < pool->cur) {
                    if (tableType == SAMPLE_TABLE) {
                        AudioHeap_DiscardSampleBank(tp->entries[0].id);
                    }

                    table[tp->entries[0].id] = 0;
                    switch (tableType) {
                        case SEQUENCE_TABLE:
                            AudioHeap_DiscardSequence(tp->entries[0].id);
                            break;
                        case BANK_TABLE:
                            AudioHeap_DiscardBank(tp->entries[0].id);
                            break;
                    }

                    tp->entries[0].id = -1;
                    pool->cur = pool->start;
                }
                ret = tp->entries[1].ptr;
                break;

            default:
                return NULL;
        }

        tp->nextSide ^= 1;
        return ret;
    }

    mem = AudioHeap_Alloc(&loadedPool->persistent.pool, size);
    loadedPool->persistent.entries[loadedPool->persistent.numEntries].ptr = mem;

    if (mem == NULL) {
        switch (where) {
            case 2:
                return AudioHeap_AllocCached(tableType, size, 0, id);

            case 0:
            case 1:
                return NULL;
        }
    }

    loadedPool->persistent.entries[loadedPool->persistent.numEntries].id = id;
    loadedPool->persistent.entries[loadedPool->persistent.numEntries].size = size;
    return loadedPool->persistent.entries[loadedPool->persistent.numEntries++].ptr;
}

// OoT func_800DF074
void* AudioHeap_SearchCaches(s32 tableType, s32 where, s32 bankId) {
    // Search order is based on 'where':
    // 0: permanent, temporary, persistent
    // 1: permanent, persistent
    // 2: permanent, persistent, temporary
    // 3: permanent
    void* ret;

    ret = AudioHeap_SearchPermanentCache(tableType, bankId);
    if (ret != NULL) {
        return ret;
    }
    if (where == 3) {
        return NULL;
    }
    return AudioHeap_SearchRegularCaches(tableType, where, bankId);
}

// OoT func_800DF0CC
void* AudioHeap_SearchRegularCaches(s32 tableType, s32 where, s32 bankId) {
    u32 i;
    AudioCache* loadedPool;
    AudioTemporaryCache* temporary;
    AudioPersistentCache* persistent;

    switch (tableType) {
        case SEQUENCE_TABLE:
            loadedPool = &gAudioContext.seqCache;
            break;
        case BANK_TABLE:
            loadedPool = &gAudioContext.bankCache;
            break;
        case SAMPLE_TABLE:
            loadedPool = &gAudioContext.sampleBankCache;
            break;
    }

    temporary = &loadedPool->temporary;
    if (where == 0) {
        if (temporary->entries[0].id == bankId) {
            temporary->nextSide = 1;
            return temporary->entries[0].ptr;
        } else if (temporary->entries[1].id == bankId) {
            temporary->nextSide = 0;
            return temporary->entries[1].ptr;
        } else {
            return NULL;
        }
    }

    persistent = &loadedPool->persistent;
    for (i = 0; i < persistent->numEntries; i++) {
        if (persistent->entries[i].id == bankId) {
            return persistent->entries[i].ptr;
        }
    }

    if (where == 2) {
        return AudioHeap_SearchCaches(tableType, 0, bankId);
    }
    return NULL;
}

// OoT func_800DF1D8
void func_8018C4E4(f32 arg0, f32 arg1, u16* arg2) {
    s32 i;
    f32 tmp[16];

    tmp[0] = (f32)(arg1 * 262159.0f);
    tmp[8] = (f32)(arg0 * 262159.0f);
    tmp[1] = (f32)((arg1 * arg0) * 262159.0f);
    tmp[9] = (f32)(((arg0 * arg0) + arg1) * 262159.0f);

    for (i = 2; i < 8; i++) {
        //! @bug value was probably meant to be stored to tmp[i] and tmp[8 + i]
        arg2[i] = arg1 * tmp[i - 2] + arg0 * tmp[i - 1];
        arg2[8 + i] = arg1 * tmp[6 + i] + arg0 * tmp[7 + i];
    }

    for (i = 0; i < 16; i++) {
        arg2[i] = tmp[i];
    }
}

// OoT func_800DF5AC
void AudioHeap_ClearFilter(s16* filter) {
    s32 i;

    for (i = 0; i < 8; i++) {
        filter[i] = 0;
    }
}
// OoT func_800DF5DC
void AudioHeap_LoadLowPassFilter(s16* filter, s32 cutoff) {
    s32 i;
    s16* ptr = &sLowPassFilterData[8 * cutoff];

    for (i = 0; i < 8; i++) {
        filter[i] = ptr[i];
    }
}
// OoT func_800DF630
void AudioHeap_LoadHighPassFilter(s16* filter, s32 cutoff) {
    s32 i;
    s16* ptr = &sHighPassFilterData[8 * (cutoff - 1)];

    for (i = 0; i < 8; i++) {
        filter[i] = ptr[i];
    }
}

// OoT func_800DF688
#ifdef NON_EQUIVALENT
void AudioHeap_LoadFilter(s16* filter, s32 lowPassCutoff, s32 highPassCutoff) {
    s32 i;
    s32 j;
    u32 flag;

    if (lowPassCutoff == 0 && highPassCutoff == 0) {
        // Identity filter
        AudioHeap_LoadLowPassFilter(filter, 0); 
    } else if (highPassCutoff == 0) {
        AudioHeap_LoadLowPassFilter(filter, lowPassCutoff);
    } else if (lowPassCutoff == 0) {
        AudioHeap_LoadHighPassFilter(filter, highPassCutoff);
    } else {
        s16* ptr1;
        i = 1;
        j = 0xE;
        if (lowPassCutoff < highPassCutoff) {
            if (lowPassCutoff >= 2) {
                flag = ((lowPassCutoff - 1) & 3);
                if (flag != 0) {
                    for (i = 1; i < (flag + 1); i++) {
                        j--;
                    }
                    if (i != lowPassCutoff) {
                        goto otherLoop;
                    }
                } else {
                    j = 0;
                    for (i = 1; i < lowPassCutoff; i+=4) {
                        otherLoop:
                        j *= 4;
                    }
                }
    
            }
            ptr1 = &D_801D3070[j + highPassCutoff - lowPassCutoff - 1];
            for (i = 0; i < 8; i++) {
                filter[i] = ptr1[i];
            }
        } else if (highPassCutoff < lowPassCutoff) {
            
            for (; i < highPassCutoff; i++) {
                j--;
            }
            ptr1 = &D_801D3700[j + lowPassCutoff - highPassCutoff - 1];
            for (i = 0; i < 8; i++) {
                filter[i] = ptr1[i];
            }
        }

    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/AudioHeap_LoadFilter.s")
#endif

// OoT func_800DF7BC
void AudioHeap_UpdateReverb(SynthesisReverb* reverb) {
}

// OoT func_800DF7C4
void AudioHeap_UpdateReverbs(void) {
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
            AudioHeap_UpdateReverb(&gAudioContext.synthesisReverbs[i]);
        }
    }
}

// OoT func_800DF888
void func_8018CC3C(void) {
    s32 ind;
    s32 i;

    ind = gAudioContext.curAIBufIdx;
    gAudioContext.aiBufLengths[ind] = gAudioContext.audioBufferParameters.minAiBufferLength;

    for (i = 0; i < 0x580; i++) {
        gAudioContext.aiBuffers[ind][i] = 0;
    }
}

s32 AudioHeap_ResetStep(void) {
    s32 i;
    s32 j;
    s32 sp24;

    if (gAudioContext.audioBufferParameters.specUnk4 == 2) {
        sp24 = 2;
    } else {
        sp24 = 1;
    }

    switch (gAudioContext.resetStatus) {
        case 5:
            for (i = 0; i < gAudioContext.audioBufferParameters.numSequencePlayers; i++) {
                AudioSeq_SequencePlayerDisableAsFinished(&gAudioContext.seqPlayers[i]);
            }
            gAudioContext.audioResetFadeOutFramesLeft = 2 / sp24;
            gAudioContext.resetStatus--;
            break;

        case 4:
            if (gAudioContext.audioResetFadeOutFramesLeft != 0) {
                gAudioContext.audioResetFadeOutFramesLeft--;
                AudioHeap_UpdateReverbs();
            } else {
                for (i = 0; i < gAudioContext.numNotes; i++) {
                    if (gAudioContext.notes[i].noteSubEu.bitField0.s.enabled &&
                        gAudioContext.notes[i].playbackState.adsr.action.s.state != ADSR_STATE_DISABLED) {
                        gAudioContext.notes[i].playbackState.adsr.fadeOutVel =
                            gAudioContext.audioBufferParameters.updatesPerFrameInv;
                        gAudioContext.notes[i].playbackState.adsr.action.s.release = true;
                    }
                }
                gAudioContext.audioResetFadeOutFramesLeft = 8 / sp24;
                gAudioContext.resetStatus--;
            }
            break;

        case 3:
            if (gAudioContext.audioResetFadeOutFramesLeft != 0) {
                gAudioContext.audioResetFadeOutFramesLeft--;
                AudioHeap_UpdateReverbs();
            } else {
                gAudioContext.audioResetFadeOutFramesLeft = 2 / sp24;
                gAudioContext.resetStatus--;
            }
            break;

        case 2:
            func_8018CC3C();
            if (gAudioContext.audioResetFadeOutFramesLeft != 0) {
                gAudioContext.audioResetFadeOutFramesLeft--;
            } else {
                gAudioContext.resetStatus--;
                AudioHeap_DiscardSampleCaches();
                AudioHeap_DiscardSampleBanks();
            }
            break;

        case 1:
            AudioHeap_Init();
            gAudioContext.resetStatus = 0;
            for (i = 0; i < 3; i++) {
                gAudioContext.aiBufLengths[i] = gAudioContext.audioBufferParameters.maxAiBufferLength;
                for (j = 0; j < 0x580; j++) {
                    gAudioContext.aiBuffers[i][j] = 0;
                }
            }
            break;
    }

    if (gAudioContext.resetStatus < 3) {
        return 0;
    }

    return 1;
}

#ifdef NON_EQUIVALENT
void AudioHeap_Init(void) {
    s32 pad1[4];
    s16* mem;
    s32 persistentMem;
    s32 temporaryMem;
    s32 totalMem;
    s32 wantMisc;
    u32 intMask;
    s32 i;
    s32 j;
    s32 pad2;
    AudioSpec* spec;

    spec = &gAudioSpecs[gAudioContext.audioResetSpecIdToLoad];
    gAudioContext.sampleDmaCount = 0;
    gAudioContext.audioBufferParameters.frequency = spec->frequency;
    gAudioContext.audioBufferParameters.aiFrequency = osAiSetFrequency(gAudioContext.audioBufferParameters.frequency);
    gAudioContext.audioBufferParameters.samplesPerFrameTarget =
        ((gAudioContext.audioBufferParameters.frequency / gAudioContext.refreshRate) + 0xF) & 0xFFF0;
    gAudioContext.audioBufferParameters.minAiBufferLength =
        gAudioContext.audioBufferParameters.samplesPerFrameTarget - 0x10;
    gAudioContext.audioBufferParameters.maxAiBufferLength =
        gAudioContext.audioBufferParameters.samplesPerFrameTarget + 0x10;
    gAudioContext.audioBufferParameters.updatesPerFrame =
        ((gAudioContext.audioBufferParameters.samplesPerFrameTarget + 0x10) / 0xD0) + 1;
    gAudioContext.audioBufferParameters.samplesPerUpdate = (gAudioContext.audioBufferParameters.samplesPerFrameTarget /
                                                            gAudioContext.audioBufferParameters.updatesPerFrame) &
                                                           ~7;
    gAudioContext.audioBufferParameters.samplesPerUpdateMax = gAudioContext.audioBufferParameters.samplesPerUpdate + 8;
    gAudioContext.audioBufferParameters.samplesPerUpdateMin = gAudioContext.audioBufferParameters.samplesPerUpdate - 8;
    gAudioContext.audioBufferParameters.resampleRate = 32000.0f / (s32)gAudioContext.audioBufferParameters.frequency;
    gAudioContext.audioBufferParameters.unkUpdatesPerFrameScaled =
        (1.0f / 256.0f) / gAudioContext.audioBufferParameters.updatesPerFrame;
    gAudioContext.audioBufferParameters.unk_24 = gAudioContext.audioBufferParameters.updatesPerFrame * 0.25f;
    gAudioContext.audioBufferParameters.updatesPerFrameInv = 1.0f / gAudioContext.audioBufferParameters.updatesPerFrame;
    gAudioContext.sampleDmaBufSize1 = spec->unk_10;
    gAudioContext.sampleDmaBufSize2 = spec->unk_12;

    gAudioContext.numNotes = spec->numNotes;
    gAudioContext.audioBufferParameters.numSequencePlayers = spec->numSequencePlayers;
    if (gAudioContext.audioBufferParameters.numSequencePlayers > 4) {
        gAudioContext.audioBufferParameters.numSequencePlayers = 4;
    }
    gAudioContext.unk_2 = spec->unk_14;
    gAudioContext.tempoInternalToExternal = (u32)(gAudioContext.audioBufferParameters.updatesPerFrame * 2880000.0f /
                                                  gTatumsPerBeat / gAudioContext.unk_2960);

    gAudioContext.unk_2870 = gAudioContext.refreshRate;
    gAudioContext.unk_2870 *= gAudioContext.audioBufferParameters.updatesPerFrame;
    gAudioContext.unk_2870 /= gAudioContext.audioBufferParameters.aiFrequency;
    gAudioContext.unk_2870 /= gAudioContext.tempoInternalToExternal;

    gAudioContext.audioBufferParameters.specUnk4 = spec->unk_04;
    gAudioContext.audioBufferParameters.samplesPerFrameTarget *= gAudioContext.audioBufferParameters.specUnk4;
    gAudioContext.audioBufferParameters.maxAiBufferLength *= gAudioContext.audioBufferParameters.specUnk4;
    gAudioContext.audioBufferParameters.minAiBufferLength *= gAudioContext.audioBufferParameters.specUnk4;
    gAudioContext.audioBufferParameters.updatesPerFrame *= gAudioContext.audioBufferParameters.specUnk4;

    if (gAudioContext.audioBufferParameters.specUnk4 >= 2) {
        gAudioContext.audioBufferParameters.maxAiBufferLength -= 0x10;
    }

    gAudioContext.maxAudioCmds =
        gAudioContext.numNotes * 0x10 * gAudioContext.audioBufferParameters.updatesPerFrame +
        spec->numReverbs * 0x18 + 0x140;

    persistentMem = spec->persistentSeqMem + spec->persistentBankMem + spec->persistentSampleMem + 0x10;
    temporaryMem = spec->temporarySeqMem + spec->temporaryBankMem + spec->temporarySampleMem + 0x10;
    totalMem = persistentMem + temporaryMem;
    wantMisc = gAudioContext.audioSessionPool.size - totalMem - 0x100;

    if (gAudioContext.externalPool.start != NULL) {
        gAudioContext.externalPool.cur = gAudioContext.externalPool.start;
    }

    gAudioContext.sessionPoolSplit.wantSeq = wantMisc;
    gAudioContext.sessionPoolSplit.wantCustom = totalMem;
    AudioHeap_SessionPoolsInit(&gAudioContext.sessionPoolSplit);
    gAudioContext.cachePoolSplit.wantPersistent = persistentMem;
    gAudioContext.cachePoolSplit.wantTemporary = temporaryMem;
    AudioHeap_CachePoolInit(&gAudioContext.cachePoolSplit);
    gAudioContext.persistentCommonPoolSplit.wantSeq = spec->persistentSeqMem;
    gAudioContext.persistentCommonPoolSplit.wantBank = spec->persistentBankMem;
    gAudioContext.persistentCommonPoolSplit.wantSample = spec->persistentSampleMem;
    AudioHeap_PersistentCachesInit(&gAudioContext.persistentCommonPoolSplit);
    gAudioContext.temporaryCommonPoolSplit.wantSeq = spec->temporarySeqMem;
    gAudioContext.temporaryCommonPoolSplit.wantBank = spec->temporaryBankMem;
    gAudioContext.temporaryCommonPoolSplit.wantSample = spec->temporarySampleMem;
    AudioHeap_TemporaryCachesInit(&gAudioContext.temporaryCommonPoolSplit);

    AudioHeap_ResetLoadStatus();
    gAudioContext.notes =
        AudioHeap_AllocZeroed(&gAudioContext.notesAndBuffersPool, gAudioContext.numNotes * sizeof(Note));
    AudioPlayback_NoteInitAll();
    AudioPlayback_InitNoteFreeList();
    gAudioContext.noteSubsEu = AudioHeap_AllocZeroed(&gAudioContext.notesAndBuffersPool,
                                                 gAudioContext.audioBufferParameters.updatesPerFrame *
                                                     gAudioContext.numNotes * sizeof(NoteSubEu));

    for (i = 0; i != 2; i++) {
        gAudioContext.abiCmdBufs[i] =
            AudioHeap_AllocDmaMemoryZeroed(&gAudioContext.notesAndBuffersPool, gAudioContext.maxAudioCmds * sizeof(u64));
    }

    gAudioContext.unk_3520 = AudioHeap_Alloc(&gAudioContext.notesAndBuffersPool, 0x100 * sizeof(f32));
    func_8018B10C();
    for (i = 0; i < 4; i++) {
        gAudioContext.synthesisReverbs[i].useReverb = 0;
    }

    // This code below likely related to func_8018E8C8
    /*
    gAudioContext.numSynthesisReverbs = spec->numReverbs;
    for (i = 0; i < gAudioContext.numSynthesisReverbs; i++) {
        ReverbSettings* settings = &spec->reverbSettings[i];
        SynthesisReverb* reverb = &gAudioContext.synthesisReverbs[i];
        reverb->downsampleRate = settings->downsampleRate;
        reverb->windowSize = settings->windowSize * 64;
        reverb->windowSize /= reverb->downsampleRate;
        reverb->unk_0C = settings->unk_4;
        reverb->unk_0A = settings->unk_A;
        reverb->unk_14 = settings->unk_6 * 64;
        reverb->unk_16 = settings->unk_8;
        reverb->unk_18 = 0;
        reverb->leakRtl = settings->leakRtl;
        reverb->leakLtr = settings->leakLtr;
        reverb->unk_05 = settings->unk_10;
        reverb->unk_08 = settings->unk_12;
        reverb->useReverb = 8;
        reverb->leftRingBuf = AudioHeap_AllocZeroedMaybeExternal(&gAudioContext.notesAndBuffersPool, reverb->windowSize * sizeof(s16));
        reverb->rightRingBuf = AudioHeap_AllocZeroedMaybeExternal(&gAudioContext.notesAndBuffersPool, reverb->windowSize * sizeof(s16));
        reverb->nextRingBufPos = 0;
        reverb->unk_20 = 0;
        reverb->curFrame = 0;
        reverb->bufSizePerChan = reverb->windowSize;
        reverb->framesToIgnore = 2;
        reverb->resampleFlags = 1;
        reverb->sound.sample = &reverb->sample;
        reverb->sample.loop = &reverb->loop;
        reverb->sound.tuning = 1.0f;
        reverb->sample.codec = 4;
        reverb->sample.medium = MEDIUM_RAM;
        reverb->sample.size = reverb->windowSize * 2;
        reverb->sample.sampleAddr = (u8*)reverb->leftRingBuf;
        reverb->loop.start = 0;
        reverb->loop.count = 1;
        reverb->loop.end = reverb->windowSize;

        if (reverb->downsampleRate != 1) {
            reverb->unk_0E = 0x8000 / reverb->downsampleRate;
            reverb->unk_30 = AudioHeap_AllocZeroed(&gAudioContext.notesAndBuffersPool, 0x20);
            reverb->unk_34 = AudioHeap_AllocZeroed(&gAudioContext.notesAndBuffersPool, 0x20);
            reverb->unk_38 = AudioHeap_AllocZeroed(&gAudioContext.notesAndBuffersPool, 0x20);
            reverb->unk_3C = AudioHeap_AllocZeroed(&gAudioContext.notesAndBuffersPool, 0x20);
            for (j = 0; j < gAudioContext.audioBufferParameters.updatesPerFrame; j++) {
                mem = AudioHeap_AllocZeroedMaybeExternal(&gAudioContext.notesAndBuffersPool, 0x340);
                reverb->items[0][j].toDownsampleLeft = mem;
                reverb->items[0][j].toDownsampleRight = mem + 0x1A0 / sizeof(s16);
                mem = AudioHeap_AllocZeroedMaybeExternal(&gAudioContext.notesAndBuffersPool, 0x340);
                reverb->items[1][j].toDownsampleLeft = mem;
                reverb->items[1][j].toDownsampleRight = mem + 0x1A0 / sizeof(s16);
            }
        }

        if (settings->lowPassFilterCutoffLeft != 0) {
            reverb->filterLeftState = AudioHeap_AllocDmaMemoryZeroed(&gAudioContext.notesAndBuffersPool, 0x40);
            reverb->filterLeft = AudioHeap_AllocDmaMemory(&gAudioContext.notesAndBuffersPool, 8 * sizeof(s16));
            AudioHeap_LoadLowPassFilter(reverb->filterLeft, settings->lowPassFilterCutoffLeft);
        } else {
            reverb->filterLeft = NULL;
        }

        if (settings->lowPassFilterCutoffRight != 0) {
            reverb->filterRightState = AudioHeap_AllocDmaMemoryZeroed(&gAudioContext.notesAndBuffersPool, 0x40);
            reverb->filterRight = AudioHeap_AllocDmaMemory(&gAudioContext.notesAndBuffersPool, 8 * sizeof(s16));
            AudioHeap_LoadLowPassFilter(reverb->filterRight, settings->lowPassFilterCutoffRight);
        } else {
            reverb->filterRight = NULL;
        }
    }
    */

    AudioSeq_InitSequencePlayers();
    for (j = 0; j < gAudioContext.audioBufferParameters.numSequencePlayers; j++) {
        func_8019AC10(j);
        AudioSeq_ResetSequencePlayer(&gAudioContext.seqPlayers[j]);
    }

    AudioHeap_InitSampleCaches(spec->persistentSampleCacheMem, spec->temporarySampleCacheMem);
    AudioLoad_InitSampleDmaBuffers(gAudioContext.numNotes);
    gAudioContext.preloadSampleStackTop = 0;
    AudioLoad_InitSlowLoads();
    AudioLoad_InitScriptLoads();
    AudioLoad_InitAsyncLoads();
    gAudioContext.unk_4 = 0x1000;
    AudioLoad_LoadPermanentSamples();
    intMask = osSetIntMask(1);
    osWritebackDCacheAll();
    osSetIntMask(intMask);
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/AudioHeap_Init.s")
#endif

// OoT func_800E04E8
void* AudioHeap_SearchPermanentCache(s32 tableType, s32 id) {
    s32 i;

    for (i = 0; i < gAudioContext.permanentPool.count; i++) {
        if (gAudioContext.permanentCache[i].tableType == tableType && gAudioContext.permanentCache[i].id == id) {
            return gAudioContext.permanentCache[i].ptr;
        }
    }
    return NULL;
}

// OoT func_800E0540
void* AudioHeap_AllocPermanent(s32 tableType, s32 id, u32 size) {
    void* ret;
    s32 index;

    index = gAudioContext.permanentPool.count;

    ret = AudioHeap_Alloc(&gAudioContext.permanentPool, size);
    gAudioContext.permanentCache[index].ptr = ret;
    if (ret == NULL) {
        return NULL;
    }
    gAudioContext.permanentCache[index].tableType = tableType;
    gAudioContext.permanentCache[index].id = id;
    gAudioContext.permanentCache[index].size = size;
    //! @bug UB: missing return. "ret" is in v0 at this point, but doing an
    // explicit return uses an additional register.
    // return ret;
}

// OoT func_800E05C4
void* AudioHeap_AllocSampleCache(u32 size, s32 bankId, void* sampleAddr, s8 medium, s32 persistent) {
    SampleCacheEntry* entry;

    if (persistent == false) {
        entry = AudioHeap_AllocTemporarySampleCacheEntry(size);
    } else {
        entry = AudioHeap_AllocPersistentSampleCacheEntry(size);
    }
    if (entry != NULL) {
        //! @bug Should use sampleBankId, not bankId
        entry->sampleBankId = bankId;
        entry->sampleAddr = sampleAddr;
        entry->origMedium = medium;
        return entry->allocatedAddr;
    }
    return NULL;
}

void AudioHeap_InitSampleCaches(u32 persistentSize, u32 temporarySize) { 
    void* mem;

    mem = AudioHeap_AllocMaybeExternal(&gAudioContext.notesAndBuffersPool, persistentSize);
    if (mem == NULL) {
        gAudioContext.persistentSampleCache.pool.size = 0;
    } else {
        AudioHeap_AllocPoolInit(&gAudioContext.persistentSampleCache.pool, mem, persistentSize);
    }
    mem = AudioHeap_AllocMaybeExternal(&gAudioContext.notesAndBuffersPool, temporarySize);
    if (mem == NULL) {
        gAudioContext.temporarySampleCache.pool.size = 0;
    } else {
        AudioHeap_AllocPoolInit(&gAudioContext.temporarySampleCache.pool, mem, temporarySize);
    }
    gAudioContext.persistentSampleCache.size = 0;
    gAudioContext.temporarySampleCache.size = 0;
}

// OoT func_800E06CC
SampleCacheEntry* AudioHeap_AllocTemporarySampleCacheEntry(u32 size) {
    s32 pad2[2];
    void* mem;
    s32 pad3[2];
    u8* allocAfter;
    u8* allocBefore;
    s32 pad1;
    s32 index;
    s32 i;
    SampleCacheEntry* ret;
    AudioPreloadReq* preload;
    AudioSampleCache* pool;
    u8* start;
    u8* end;

    pool = &gAudioContext.temporarySampleCache;
    allocBefore = pool->pool.cur;
    mem = AudioHeap_Alloc(&pool->pool, size);
    if (mem == NULL) {
        // Reset the pool and try again. We still keep pointers to within the
        // pool, so we have to be careful to discard existing overlapping
        // allocations further down.
        u8* old = pool->pool.cur;

        pool->pool.cur = pool->pool.start;
        mem = AudioHeap_Alloc(&pool->pool, size);
        if (mem == NULL) {
            pool->pool.cur = old;
            return NULL;
        }
        allocBefore = pool->pool.start;
    }

    allocAfter = pool->pool.cur;

    index = -1;
    for (i = 0; i < gAudioContext.preloadSampleStackTop; i++) {
        preload = &gAudioContext.preloadSampleStack[i];
        if (preload->isFree == false) {
            start = preload->ramAddr;
            end = preload->ramAddr + preload->sample->size - 1;

            if (end < allocBefore && start < allocBefore) {
                continue;
            }
            if (end >= allocAfter && start >= allocAfter) {
                continue;
            }

            // Overlap
            preload->isFree = true;
        }
    }

    for (i = 0; i < pool->size; i++) {
        if (pool->entries[i].inUse == 0) {
            continue;
        }

        start = pool->entries[i].allocatedAddr;
        end = start + pool->entries[i].size - 1;

        if (end < allocBefore && start < allocBefore) {
            continue;
        }
        if (end >= allocAfter && start >= allocAfter) {
            continue;
        }

        // Overlap, discard existing entry.
        AudioHeap_DiscardSampleCacheEntry(&pool->entries[i]);
        pool->entries[i].inUse = 0;
        if (index == -1) {
            index = i;
        }
    }

    if (index == -1) {
        for (i = 0; i < pool->size; i++) {
            if (pool->entries[i].inUse == 0) {
                break;
            }
        }

        index = i;
        if (index == pool->size) {
            if (pool->size == 0x80) {
                return NULL;
            }
            pool->size++;
        }
    }

    ret = &pool->entries[index];
    ret->inUse = 1;
    ret->allocatedAddr = mem;
    ret->size = size;
    return ret;
}

// OoT func_800E0964
void AudioHeap_UnapplySampleCacheForBank(SampleCacheEntry* entry, s32 bankId) {
    Drum* drum;
    Instrument* inst;
    AudioBankSound* sfx;
    s32 instId;
    s32 drumId;
    s32 sfxId;

    for (instId = 0; instId < gAudioContext.ctlEntries[bankId].numInstruments; instId++) {
        inst = AudioPlayback_GetInstrumentInner(bankId, instId);
        if (inst != NULL) {
            if (inst->normalRangeLo != 0) {
                AudioHeap_UnapplySampleCache(entry, inst->lowNotesSound.sample);
            }
            if (inst->normalRangeHi != 0x7F) {
                AudioHeap_UnapplySampleCache(entry, inst->highNotesSound.sample);
            }
            AudioHeap_UnapplySampleCache(entry, inst->normalNotesSound.sample);
        }
    }

    for (drumId = 0; drumId < gAudioContext.ctlEntries[bankId].numDrums; drumId++) {
        drum = AudioPlayback_GetDrum(bankId, drumId);
        if (drum != NULL) {
            AudioHeap_UnapplySampleCache(entry, drum->sound.sample);
        }
    }

    for (sfxId = 0; sfxId < gAudioContext.ctlEntries[bankId].numSfx; sfxId++) {
        sfx = AudioPlayback_GetSfx(bankId, sfxId);
        if (sfx != NULL) {
            AudioHeap_UnapplySampleCache(entry, sfx->sample);
        }
    }
}

void AudioHeap_DiscardSampleCacheEntry(SampleCacheEntry* entry) {
    s32 numBanks;
    s32 sampleBankId1;
    s32 sampleBankId2;
    s32 bankId;

    numBanks = gAudioContext.audioBankTable->header.entryCnt;
    for (bankId = 0; bankId < numBanks; bankId++) {
        sampleBankId1 = gAudioContext.ctlEntries[bankId].sampleBankId1;
        sampleBankId2 = gAudioContext.ctlEntries[bankId].sampleBankId2;
        if (((sampleBankId1 != 0xFF) && (entry->sampleBankId == sampleBankId1)) || ((sampleBankId2 != 0xFF) && (entry->sampleBankId == sampleBankId2)) ||
            entry->sampleBankId == 0 || entry->sampleBankId == 0xFE) {
            if (AudioHeap_SearchCaches(BANK_TABLE, 2, bankId) != NULL) {
                if (1) {}
                if (AudioLoad_IsBankLoadComplete(bankId) != 0) {
                    AudioHeap_UnapplySampleCacheForBank(entry, bankId);
                }
            }
        }
    }
}

// OoT func_800E0BB4
void AudioHeap_UnapplySampleCache(SampleCacheEntry* entry, AudioBankSample* sample) {
    if (sample != NULL) {
        if (sample->sampleAddr == entry->allocatedAddr) {
            sample->sampleAddr = entry->sampleAddr;
            sample->medium = entry->origMedium;
        }
    }
}

// OoT func_800E0BF8
SampleCacheEntry* AudioHeap_AllocPersistentSampleCacheEntry(u32 size) {
    AudioSampleCache* pool;
    SampleCacheEntry* entry;
    void* mem;

    pool = &gAudioContext.persistentSampleCache;
    mem = AudioHeap_Alloc(&pool->pool, size);
    if (mem == NULL) {
        return NULL;
    }

    if (pool->size == 0x80) {
        return NULL;
    }

    entry = &pool->entries[pool->size];
    entry->inUse = true;
    entry->allocatedAddr = mem;
    entry->size = size;
    pool->size++;
    return entry;
}

// OoT func_800E0C80
void AudioHeap_DiscardSampleCacheForBank(SampleCacheEntry* entry, s32 sampleBankId1, s32 sampleBankId2, s32 bankId) {
    if ((entry->sampleBankId == sampleBankId1) || (entry->sampleBankId == sampleBankId2) || (entry->sampleBankId == 0)) {
        AudioHeap_UnapplySampleCacheForBank(entry, bankId);
    }
}

// OoT func_800E0CBC
void AudioHeap_DiscardSampleCaches(void) {
    s32 numBanks;
    s32 sampleBankId1;
    s32 sampleBankId2;
    s32 bankId;
    s32 j;

    numBanks = gAudioContext.audioBankTable->header.entryCnt;
    for (bankId = 0; bankId < numBanks; bankId++) {
        sampleBankId1 = gAudioContext.ctlEntries[bankId].sampleBankId1;
        sampleBankId2 = gAudioContext.ctlEntries[bankId].sampleBankId2;
        if ((sampleBankId1 == 0xFF) && (sampleBankId2 == 0xFF)) {
            continue;
        }
        if (AudioHeap_SearchCaches(BANK_TABLE, 3, bankId) == NULL || !AudioLoad_IsBankLoadComplete(bankId)) {
            continue;
        }

        for (j = 0; j < gAudioContext.persistentSampleCache.size; j++) {
            AudioHeap_DiscardSampleCacheForBank(&gAudioContext.persistentSampleCache.entries[j], sampleBankId1, sampleBankId2, bankId);
        }
        for (j = 0; j < gAudioContext.temporarySampleCache.size; j++) {
            AudioHeap_DiscardSampleCacheForBank(&gAudioContext.temporarySampleCache.entries[j], sampleBankId1, sampleBankId2, bankId);
        }
    }
}


typedef struct {
    u8* oldAddr;
    u8* newAddr;
    u32 size;
    u8 newMedium;
} StorageChange;

// OoT func_800E0E0C
void AudioHeap_ChangeStorage(StorageChange* change, AudioBankSample* sample) {
    if (sample != NULL && ((sample->medium == change->newMedium) || (D_801FD120 != 1)) && ((sample->medium == 0) || (D_801FD120 != 0))) {
        u8* start = change->oldAddr;
        u8* end = change->oldAddr + change->size;
        u8* pad = sample->sampleAddr;
        u8* sampleAddr = sample->sampleAddr;

        if (start <= sampleAddr && sampleAddr < end) {
            sample->sampleAddr = sampleAddr - start + change->newAddr;
            if (D_801FD120 == 0) {
                sample->medium = change->newMedium;
            } else {
                sample->medium = 0;
            }
        }
    }
}

// OoT func_800E0E6C
void AudioHeap_DiscardSampleBank(s32 sampleBankId) {
    D_801FD120 = 0;
    AudioHeap_ApplySampleBankCacheInternal(0, sampleBankId);
}

// OoT func_800E0E90
void AudioHeap_ApplySampleBankCache(s32 sampleBankId) {
    D_801FD120 = 1;
    AudioHeap_ApplySampleBankCacheInternal(1, sampleBankId);
}

// OoT func_800E0EB4
void AudioHeap_ApplySampleBankCacheInternal(s32 apply, s32 sampleBankId) {
    SampleBankTable* sampleBankTable;
    SampleBankTableEntry* entry;
    s32 numBanks;
    s32 instId;
    s32 drumId;
    s32 sfxId;
    StorageChange change;
    s32 sampleBankId1;
    s32 sampleBankId2;
    s32 bankId;
    Drum* drum;
    Instrument* inst;
    AudioBankSound* sfx;
    u8** fakematch;
    s32 pad[4];

    sampleBankTable = gAudioContext.sampleBankTable;
    numBanks = gAudioContext.audioBankTable->header.entryCnt;
    change.oldAddr = AudioHeap_SearchCaches(SAMPLE_TABLE, 2, sampleBankId);
    if (change.oldAddr == NULL) {
        return;
    }

    entry = &sampleBankTable->entries[sampleBankId];
    change.size = entry->size;
    change.newMedium = entry->medium;
    change.newAddr = entry->romAddr;

    fakematch = &change.oldAddr;
    if ((apply != false) && (apply == true)) {
        u8* temp = change.newAddr;

        change.newAddr = *fakematch; // = change.oldAddr
        change.oldAddr = temp;
    }

    for (bankId = 0; bankId < numBanks; bankId++) {
        sampleBankId1 = gAudioContext.ctlEntries[bankId].sampleBankId1;
        sampleBankId2 = gAudioContext.ctlEntries[bankId].sampleBankId2;
        if ((sampleBankId1 != 0xFF) || (sampleBankId2 != 0xFF)) {
            if (!AudioLoad_IsBankLoadComplete(bankId) || AudioHeap_SearchCaches(BANK_TABLE, 2, bankId) == NULL) {
                continue;
            }

            if (sampleBankId1 == sampleBankId) {
            } else if (sampleBankId2 == sampleBankId) {
            } else {
                continue;
            }

            for (instId = 0; instId < gAudioContext.ctlEntries[bankId].numInstruments; instId++) {
                inst = AudioPlayback_GetInstrumentInner(bankId, instId);
                if (inst != NULL) {
                    if (inst->normalRangeLo != 0) {
                        AudioHeap_ChangeStorage(&change, inst->lowNotesSound.sample);
                    }
                    if (inst->normalRangeHi != 0x7F) {
                        AudioHeap_ChangeStorage(&change, inst->highNotesSound.sample);
                    }
                    AudioHeap_ChangeStorage(&change, inst->normalNotesSound.sample);
                }
            }

            for (drumId = 0; drumId < gAudioContext.ctlEntries[bankId].numDrums; drumId++) {
                drum = AudioPlayback_GetDrum(bankId, drumId);
                if (drum != NULL) {
                    AudioHeap_ChangeStorage(&change, drum->sound.sample);
                }
            }

            for (sfxId = 0; sfxId < gAudioContext.ctlEntries[bankId].numSfx; sfxId++) {
                sfx = AudioPlayback_GetSfx(bankId, sfxId);
                if (sfx != NULL) {
                    AudioHeap_ChangeStorage(&change, sfx->sample);
                }
            }
        }
    }
}

// OoT func_800E1148
void AudioHeap_DiscardSampleBanks(void) {
    AudioCache* pool;
    AudioPersistentCache* persistent;
    AudioTemporaryCache* temporary;
    u32 i;

    pool = &gAudioContext.sampleBankCache;
    temporary = &pool->temporary;

    if (temporary->entries[0].id != -1) {
        AudioHeap_DiscardSampleBank(temporary->entries[0].id);
    }

    if (temporary->entries[1].id != -1) {
        AudioHeap_DiscardSampleBank(temporary->entries[1].id);
    }

    persistent = &pool->persistent;
    for (i = 0; i < persistent->numEntries; i++) {
        AudioHeap_DiscardSampleBank(persistent->entries[i].id);
    }
}

// New MM Function?
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018E344.s")
// ? func_8018E8C8(s32, s32, ?); // extern

// void func_8018E344(s32 id, u32 arg1, s32 arg2, s32 arg3) {
//     void* sp1C;
//     s16* temp_a0_2;
//     s16* temp_a0_4;
//     s32 temp_a0;
//     s32 temp_lo;
//     s32 temp_t1;
//     u8 temp_v0;
//     void* temp_a0_3;
//     void* temp_a0_5;
//     void* temp_v0_2;
//     void* temp_v0_3;
//     void* temp_v0_4;
//     void* temp_v1;
//     SynthesisReverb* reverb;
//     s32 phi_a0;
//     s16* phi_a0_2;
//     void* phi_v1;
//     s16* phi_a0_3;
//     void* phi_v1_2;

//     switch (arg1) {
//     case 0:
//         func_8018E8C8(id, arg2, 0);
//         return;
//     case 1:
//         if (arg2 < 4) {
//             arg2 = 4;
//         }
//         temp_a0 = arg2 << 6;
//         phi_a0 = temp_a0;
//         if (temp_a0 < 0x100) {
//             phi_a0 = 0x100;
//         }
//         temp_t1 = (id * 0x2D0) + 0x18;
//         temp_v1 = temp_t1 + &gAudioContext;
//         temp_v0 = temp_v1->unk4;
//         temp_lo = phi_a0 / temp_v0;
//         if (arg3 == 0) {
//             if (temp_v1->unk1E >= (arg2 / temp_v0)) {
//                 if ((temp_v1->unk20 >= temp_lo) || (temp_v1->unk24 >= temp_lo)) {
//                     temp_v1->unk20 = 0;
//                     temp_v1->unk24 = 0;
//                 }
//                 goto block_12;
//             }
//             // Duplicate return node #41. Try simplifying control flow for better match
//             return;
//         }
// block_12:
//         gAudioContext.unk_0006[temp_t1] = temp_lo;
//         if (((temp_v1->unk4 != 1) || (temp_v1->unk18 != 0)) && (gAudioContext.unk_0006[temp_t1].unk8 = 0x8000 / temp_v1->unk4, (temp_v1->unk30 == 0)) && (sp1C = temp_v1, temp_v1->unk30 = AudioHeap_AllocZeroed(&gAudioContext.notesAndBuffersPool, 0x20), temp_v1->unk34 = AudioHeap_AllocZeroed(&gAudioContext.notesAndBuffersPool, 0x20), temp_v1->unk38 = AudioHeap_AllocZeroed(&gAudioContext.notesAndBuffersPool, 0x20), temp_v0_2 = AudioHeap_AllocZeroed(&gAudioContext.notesAndBuffersPool, 0x20), temp_v1->unk3C = temp_v0_2, (temp_v0_2 == 0))) {
//             temp_v1->unk4 = 1;
//             return;
//         }
//     default:
//         return;
//     case 2:
//         gAudioContext.synthesisReverbs[id].unk_0C = arg2;
//         return;
//     case 3:
//         gAudioContext.synthesisReverbs[id].unk_16 = arg2;
//         return;
//     case 4:
//         gAudioContext.synthesisReverbs[id].unk_0A = arg2;
//         return;
//     case 5:
//         gAudioContext.synthesisReverbs[id].leakRtl = arg2;
//         return;
//     case 6:
//         gAudioContext.synthesisReverbs[id].leakLtr = arg2;
//         return;
//     case 7:
//         if (arg2 != 0) {
//             reverb = &gAudioContext.synthesisReverbs[id];
//             if ((arg3 != 0) || (temp_a0_2 = reverb->unk278, phi_a0_2 = temp_a0_2, phi_v1 = reverb, (temp_a0_2 == 0))) {
//                 reverb =  &gAudioContext.synthesisReverbs[id];
//                 sp1C = reverb;
//                 reverb->unk280 = AudioHeap_AllocDmaMemoryZeroed(&gAudioContext.notesAndBuffersPool, 0x40);
//                 temp_v0_3 = AudioHeap_AllocDmaMemory(&gAudioContext.notesAndBuffersPool, 0x10);
//                 temp_a0_3 = temp_v0_3;
//                 reverb->unk278 = temp_v0_3;
//                 phi_a0_2 = temp_a0_3;
//                 phi_v1 = reverb;
//             }
//             phi_v1->unk270 = phi_a0_2;
//             if (phi_a0_2 != 0) {
//                 AudioHeap_LoadLowPassFilter(phi_a0_2, arg2);
//                 return;
//             }
//             // Duplicate return node #41. Try simplifying control flow for better match
//             return;
//         }
//         reverb = &gAudioContext.synthesisReverbs[id];
//         reverb->unk270 = 0;
//         if (arg3 != 0) {
//             reverb->unk278 = 0;
//             return;
//         }
//         // Duplicate return node #41. Try simplifying control flow for better match
//         return;
//     case 8:
//         if (arg2 != 0) {
//             if ((arg3 != 0) || (reverb = &gAudioContext.synthesisReverbs[id], temp_a0_4 = reverb->unk27C, phi_a0_3 = temp_a0_4, phi_v1_2 = reverb, (temp_a0_4 == 0))) {
//                 reverb = &gAudioContext.synthesisReverbs[id];
//                 sp1C = reverb;
//                 reverb->unk284 = AudioHeap_AllocDmaMemoryZeroed(&gAudioContext.notesAndBuffersPool, 0x40);
//                 temp_v0_4 = AudioHeap_AllocDmaMemory(&gAudioContext.notesAndBuffersPool, 0x10);
//                 temp_a0_5 = temp_v0_4;
//                 reverb->unk27C = temp_v0_4;
//                 phi_a0_3 = temp_a0_5;
//                 phi_v1_2 = reverb;
//             }
//             phi_v1_2->unk274 = phi_a0_3;
//             if (phi_a0_3 != 0) {
//                 AudioHeap_LoadLowPassFilter(phi_a0_3, arg2);
//                 return;
//             }
//             // Duplicate return node #41. Try simplifying control flow for better match
//             return;
//         }
//         reverb = &gAudioContext.synthesisReverbs[id];
//         reverb->unk274 = 0;
//         if (arg3 != 0) {
//             reverb->unk27C = 0;
//             return;
//         }
//         // Duplicate return node #41. Try simplifying control flow for better match
//         return;
//     case 9:
//         reverb = &gAudioContext.synthesisReverbs[id];
//         reverb->unk19 = arg2;
//         if (arg2 == 0) {
//             reverb->unk18 = 0;
//             return;
//         }
//         reverb->unk18 = 1;
//         // Duplicate return node #41. Try simplifying control flow for better match
//         return;
//     }
// }



// New MM Function?
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018E8C8.s")
// ? func_8018E344(s32, ?, u16, s32); // extern

// void func_8018E8C8(s32 id, ReverbSettings* settings, s32 arg2) {
//     s32 temp_v1;
//     u16 temp_v0;
//     u8 temp_t6;
//     void* temp_s0;
//     void* temp_s0_2;
//     void* phi_s0;

//     if (arg2 != 0) {
//         temp_s0 = (id * 0x2D0) + 0x18 + &gAudioContext;
//         temp_s0->unk1E = arg1->unk2 / arg1->unk0;
//         temp_s0->unk30 = 0;
//         phi_s0 = temp_s0;
//     } else if (temp_s0_2->unk1E < (arg1->unk2 / arg1->unk0)) {
//          return
//     }
//         phi_s0->unk4 = arg1->unk0;
//         phi_s0->unk18 = 0;
//         phi_s0->unk19 = 0;
//         phi_s0->unk1A = 0;
//         phi_s0->unk1C = 0;
//         func_8018E344(id, 1, arg1->unk2, arg2);
//         phi_s0->unkC = arg1->unk4;
//         phi_s0->unkA = arg1->unkA;
//         phi_s0->unk14 = arg1->unk6 << 6;
//         phi_s0->unk16 = arg1->unk8;
//         phi_s0->unk10 = arg1->unkC;
//         phi_s0->unk12 = arg1->unkE;
//         phi_s0->unk5 = arg1->unk10;
//         phi_s0->unk1 = 8;
//         phi_s0->unk8 = arg1->unk12;
//         if (arg2 != 0) {
//             phi_s0->unk28 = AudioHeap_AllocZeroedMaybeExternal(&gAudioContext.notesAndBuffersPool, phi_s0->unk6 * 2);
//             phi_s0->unk2C = AudioHeap_AllocZeroedMaybeExternal(&gAudioContext.notesAndBuffersPool, phi_s0->unk6 * 2);
//             phi_s0->unk0 = 1;
//             phi_s0->unk20 = 0;
//             phi_s0->unk24 = 0;
//             phi_s0->unk3 = 0;
//             phi_s0->unk2 = 2;
//         }
//         temp_v0 = phi_s0->unk6;
//         temp_t6 = (phi_s0->unk290 & 0xFF8F) | 0x40;
//         phi_s0->unk290 = temp_t6;
//         phi_s0->unk290 = temp_t6 & 0xF3;
//         temp_v1 = phi_s0->unk290;
//         phi_s0->unk288 = phi_s0 + 0x290;
//         phi_s0->unk298 = phi_s0 + 0x2A0;
//         phi_s0->unk290 = (((((temp_v0 * 2) & 0xFFFFFF) ^ temp_v1) << 8) >> 8) ^ temp_v1;
//         phi_s0->unk2A0 = 0;
//         phi_s0->unk2A8 = 1;
//         phi_s0->unk2A4 = temp_v0;
//         phi_s0->unk28C = 1.0f;
//         phi_s0->unk294 = phi_s0->unk28;
//         func_8018E344(id, 7, arg1->unk14, arg2);
//         func_8018E344(id, 8, arg1->unk16, arg2);
//     }
// }


//     gAudioContext.numSynthesisReverbs = spec->numReverbs;
//     for (i = 0; i < gAudioContext.numSynthesisReverbs; i++) {
//         ReverbSettings* settings = &spec->reverbSettings[i];
//         SynthesisReverb* reverb = &gAudioContext.synthesisReverbs[i];
//         reverb->downsampleRate = settings->downsampleRate;
//         reverb->windowSize = settings->windowSize * 64;
//         reverb->windowSize /= reverb->downsampleRate;
//         reverb->unk_0C = settings->unk_4;
//         reverb->unk_0A = settings->unk_A;
//         reverb->unk_14 = settings->unk_6 * 64;
//         reverb->unk_16 = settings->unk_8;
//         reverb->unk_18 = 0;
//         reverb->leakRtl = settings->leakRtl;
//         reverb->leakLtr = settings->leakLtr;
//         reverb->unk_05 = settings->unk_10;
//         reverb->unk_08 = settings->unk_12;
//         reverb->useReverb = 8;
//         reverb->leftRingBuf = AudioHeap_AllocZeroedMaybeExternal(&gAudioContext.notesAndBuffersPool, reverb->windowSize * sizeof(s16));
//         reverb->rightRingBuf = AudioHeap_AllocZeroedMaybeExternal(&gAudioContext.notesAndBuffersPool, reverb->windowSize * sizeof(s16));
//         reverb->nextRingBufPos = 0;
//         reverb->unk_20 = 0;
//         reverb->curFrame = 0;
//         reverb->bufSizePerChan = reverb->windowSize;
//         reverb->framesToIgnore = 2;
//         reverb->resampleFlags = 1;
//         reverb->sound.sample = &reverb->sample;
//         reverb->sample.loop = &reverb->loop;
//         reverb->sound.tuning = 1.0f;
//         reverb->sample.codec = 4;
//         reverb->sample.medium = 0;
//         reverb->sample.size = reverb->windowSize * 2;
//         reverb->sample.sampleAddr = (u8*)reverb->leftRingBuf;
//         reverb->loop.start = 0;
//         reverb->loop.count = 1;
//         reverb->loop.end = reverb->windowSize;

//         if (reverb->downsampleRate != 1) {
//             reverb->unk_0E = 0x8000 / reverb->downsampleRate;
//             reverb->unk_30 = AudioHeap_AllocZeroed(&gAudioContext.notesAndBuffersPool, 0x20);
//             reverb->unk_34 = AudioHeap_AllocZeroed(&gAudioContext.notesAndBuffersPool, 0x20);
//             reverb->unk_38 = AudioHeap_AllocZeroed(&gAudioContext.notesAndBuffersPool, 0x20);
//             reverb->unk_3C = AudioHeap_AllocZeroed(&gAudioContext.notesAndBuffersPool, 0x20);
//             for (j = 0; j < gAudioContext.audioBufferParameters.updatesPerFrame; j++) {
//                 mem = AudioHeap_AllocZeroedMaybeExternal(&gAudioContext.notesAndBuffersPool, 0x340);
//                 reverb->items[0][j].toDownsampleLeft = mem;
//                 reverb->items[0][j].toDownsampleRight = mem + 0x1A0 / sizeof(s16);
//                 mem = AudioHeap_AllocZeroedMaybeExternal(&gAudioContext.notesAndBuffersPool, 0x340);
//                 reverb->items[1][j].toDownsampleLeft = mem;
//                 reverb->items[1][j].toDownsampleRight = mem + 0x1A0 / sizeof(s16);
//             }
//         }

//         if (settings->unk_14 != 0) {
//             reverb->filterLeftState = AudioHeap_AllocDmaMemoryZeroed(&gAudioContext.notesAndBuffersPool, 0x40);
//             reverb->filterLeft = AudioHeap_AllocDmaMemory(&gAudioContext.notesAndBuffersPool, 8 * sizeof(s16));
//             func_800DF5DC(reverb->filterLeft, settings->unk_14);
//         } else {
//             reverb->filterLeft = NULL;
//         }

//         if (settings->unk_16 != 0) {
//             reverb->filterRightState = AudioHeap_AllocDmaMemoryZeroed(&gAudioContext.notesAndBuffersPool, 0x40);
//             reverb->filterRight = AudioHeap_AllocDmaMemory(&gAudioContext.notesAndBuffersPool, 8 * sizeof(s16));
//             func_800DF5DC(reverb->filterRight, settings->unk_16);
//         } else {
//             reverb->filterRight = NULL;
//         }
//     }

//     AudioSeq_InitSequencePlayers();
