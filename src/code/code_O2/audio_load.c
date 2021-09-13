#include "global.h"

// opaque type for unpatched audio bank data (should maybe get rid of this?)
typedef void AudioBankData;

void func_8018F9B8(s32 arg0);
u8* func_8018FCCC(s32 seqId);
u32 func_8018FD40(u32 arg0, u32* arg1, s32 arg2);
AudioBankData* func_8018FE5C(u32 bankId);
void* func_8018FF60(u32 tableType, u32 tableIdx, s32* didAllocate);
u32 Audio_GetTableIndex(s32 tableType, u32 tableIdx);
void* func_80190240(s32 tableType, s32 id);
void* func_80190294(s32 tableType);
void* Audio_AsyncLoadInner(s32 tableType, s32 arg1, s32 arg2, s32 arg3, OSMesgQueue* retQueue);

typedef enum { LOAD_STATUS_WAITING, LOAD_STATUS_START, LOAD_STATUS_LOADING, LOAD_STATUS_DONE } SyncLoadStatus;

#define OS_MESG_PRI_NORMAL  0
#define OS_MESG_PRI_HIGH    1


// OoT func_800E11F0
void func_8018EB60(void) {
    u32 i;

    for (i = 0; i < gAudioContext.sampleDmaListSize1; i++) {
        SampleDmaReq* req = &gAudioContext.sampleDmaReqs[i];

        if (req->ttl != 0) {
            req->ttl--;
            if (req->ttl == 0) {
                req->reuseIndex = gAudioContext.sampleDmaReuseQueue1WrPos;
                gAudioContext.sampleDmaReuseQueue1[gAudioContext.sampleDmaReuseQueue1WrPos] = i;
                gAudioContext.sampleDmaReuseQueue1WrPos++;
            }
        }
    }

    for (i = gAudioContext.sampleDmaListSize1; i < gAudioContext.sampleDmaReqCnt; i++) {
        SampleDmaReq* req = &gAudioContext.sampleDmaReqs[i];

        if (req->ttl != 0) {
            req->ttl--;
            if (req->ttl == 0) {
                req->reuseIndex = gAudioContext.sampleDmaReuseQueue2WrPos;
                gAudioContext.sampleDmaReuseQueue2[gAudioContext.sampleDmaReuseQueue2WrPos] = i;
                gAudioContext.sampleDmaReuseQueue2WrPos++;
            }
        }
    }

    gAudioContext.unk_2628 = 0;
}

void* Audio_DmaSampleData(u32 devAddr, u32 size, s32 arg2, u8* dmaIndexRef, s32 medium) {
    s32 sp60;
    SampleDmaReq* dma;
    s32 hasDma = false;
    u32 dmaDevAddr;
    u32 pad;
    u32 dmaIndex;
    u32 transfer;
    s32 bufferPos;
    u32 i;

    if (arg2 != 0 || *dmaIndexRef >= gAudioContext.sampleDmaListSize1) {
        for (i = gAudioContext.sampleDmaListSize1; i < gAudioContext.sampleDmaReqCnt; i++) {
            dma = &gAudioContext.sampleDmaReqs[i];
            bufferPos = devAddr - dma->devAddr;
            if (0 <= bufferPos && (u32)bufferPos <= dma->size - size) {
                // We already have a DMA request for this memory range.
                if (dma->ttl == 0 &&
                    gAudioContext.sampleDmaReuseQueue2RdPos != gAudioContext.sampleDmaReuseQueue2WrPos) {
                    // Move the DMA out of the reuse queue, by swapping it with the
                    // read pos, and then incrementing the read pos.
                    if (dma->reuseIndex != gAudioContext.sampleDmaReuseQueue2RdPos) {
                        gAudioContext.sampleDmaReuseQueue2[dma->reuseIndex] =
                            gAudioContext.sampleDmaReuseQueue2[gAudioContext.sampleDmaReuseQueue2RdPos];
                        gAudioContext
                            .sampleDmaReqs[gAudioContext.sampleDmaReuseQueue2[gAudioContext.sampleDmaReuseQueue2RdPos]]
                            .reuseIndex = dma->reuseIndex;
                    }
                    gAudioContext.sampleDmaReuseQueue2RdPos++;
                }
                dma->ttl = 32;
                *dmaIndexRef = (u8)i;
                return &dma->ramAddr[devAddr - dma->devAddr];
            }
        }

        if (arg2 == 0) {
            goto search_short_lived;
        }

        if (gAudioContext.sampleDmaReuseQueue2RdPos != gAudioContext.sampleDmaReuseQueue2WrPos && arg2 != 0) {
            // Allocate a DMA from reuse queue 2, unless full.
            dmaIndex = gAudioContext.sampleDmaReuseQueue2[gAudioContext.sampleDmaReuseQueue2RdPos];
            gAudioContext.sampleDmaReuseQueue2RdPos++;
            dma = gAudioContext.sampleDmaReqs + dmaIndex;
            hasDma = true;
        }
    } else {
    search_short_lived:
        dma = gAudioContext.sampleDmaReqs + *dmaIndexRef;
        i = 0;
    again:
        bufferPos = devAddr - dma->devAddr;
        if (0 <= bufferPos && (u32)bufferPos <= dma->size - size) {
            // We already have DMA for this memory range.
            if (dma->ttl == 0) {
                // Move the DMA out of the reuse queue, by swapping it with the
                // read pos, and then incrementing the read pos.
                if (dma->reuseIndex != gAudioContext.sampleDmaReuseQueue1RdPos) {
                    gAudioContext.sampleDmaReuseQueue1[dma->reuseIndex] =
                        gAudioContext.sampleDmaReuseQueue1[gAudioContext.sampleDmaReuseQueue1RdPos];
                    gAudioContext
                        .sampleDmaReqs[gAudioContext.sampleDmaReuseQueue1[gAudioContext.sampleDmaReuseQueue1RdPos]]
                        .reuseIndex = dma->reuseIndex;
                }
                gAudioContext.sampleDmaReuseQueue1RdPos++;
            }
            dma->ttl = 2;
            return dma->ramAddr + (devAddr - dma->devAddr);
        }
        dma = gAudioContext.sampleDmaReqs + i++;
        if (i <= gAudioContext.sampleDmaListSize1) {
            goto again;
        }
    }

    if (!hasDma) {
        if (gAudioContext.sampleDmaReuseQueue1RdPos == gAudioContext.sampleDmaReuseQueue1WrPos) {
            return NULL;
        }
        // Allocate a DMA from reuse queue 1.
        dmaIndex = gAudioContext.sampleDmaReuseQueue1[gAudioContext.sampleDmaReuseQueue1RdPos++];
        dma = gAudioContext.sampleDmaReqs + dmaIndex;
        hasDma = true;
    }

    transfer = dma->size;
    dmaDevAddr = devAddr & ~0xF;
    dma->ttl = 3;
    dma->devAddr = dmaDevAddr;
    dma->sizeUnused = transfer;
    Audio_DMA(&gAudioContext.currAudioFrameDmaIoMesgBufs[gAudioContext.curAudioFrameDmaCount++], OS_MESG_PRI_NORMAL,
              OS_READ, dmaDevAddr, dma->ramAddr, transfer, &gAudioContext.currAudioFrameDmaQueue, medium, "SUPERDMA");
    *dmaIndexRef = dmaIndex;
    return (devAddr - dmaDevAddr) + dma->ramAddr;
}

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/D_801E030C.s")

// OoT func_800E1618
void func_8018EF88(s32 arg0) {
    SampleDmaReq* temp_s0;
    s32 i;
    s32 t2;
    s32 j;

    gAudioContext.unk_288C = gAudioContext.unk_2874;
    gAudioContext.sampleDmaReqs =
        Audio_Alloc(&gAudioContext.notesAndBuffersPool, 4 * gAudioContext.maxSimultaneousNotes * sizeof(SampleDmaReq) *
                                                            gAudioContext.audioBufferParameters.specUnk4);
    t2 = 3 * gAudioContext.maxSimultaneousNotes * gAudioContext.audioBufferParameters.specUnk4;
    for (i = 0; i < t2; i++) {
        temp_s0 = &gAudioContext.sampleDmaReqs[gAudioContext.sampleDmaReqCnt];
        temp_s0->ramAddr = func_8018B578(&gAudioContext.notesAndBuffersPool, gAudioContext.unk_288C);
        if (temp_s0->ramAddr == NULL) {
            break;
        } else {
            func_8018B4F8(temp_s0->ramAddr, gAudioContext.unk_288C);
            temp_s0->size = gAudioContext.unk_288C;
            temp_s0->devAddr = 0;
            temp_s0->sizeUnused = 0;
            temp_s0->unused = 0;
            temp_s0->ttl = 0;
            gAudioContext.sampleDmaReqCnt++;
        }
    }

    for (i = 0; (u32)i < gAudioContext.sampleDmaReqCnt; i++) {
        gAudioContext.sampleDmaReuseQueue1[i] = i;
        gAudioContext.sampleDmaReqs[i].reuseIndex = i;
    }

    for (i = gAudioContext.sampleDmaReqCnt; i < 0x100; i++) {
        gAudioContext.sampleDmaReuseQueue1[i] = 0;
    }

    gAudioContext.sampleDmaReuseQueue1RdPos = 0;
    gAudioContext.sampleDmaReuseQueue1WrPos = gAudioContext.sampleDmaReqCnt;
    gAudioContext.sampleDmaListSize1 = gAudioContext.sampleDmaReqCnt;
    gAudioContext.unk_288C = gAudioContext.unk_2878;

    for (j = 0; j < gAudioContext.maxSimultaneousNotes; j++) {
        temp_s0 = &gAudioContext.sampleDmaReqs[gAudioContext.sampleDmaReqCnt];
        temp_s0->ramAddr = func_8018B578(&gAudioContext.notesAndBuffersPool, gAudioContext.unk_288C);
        if (temp_s0->ramAddr == NULL) {
            break;
        } else {
            func_8018B4F8(temp_s0->ramAddr, gAudioContext.unk_288C);
            temp_s0->size = gAudioContext.unk_288C;
            temp_s0->devAddr = 0U;
            temp_s0->sizeUnused = 0;
            temp_s0->unused = 0;
            temp_s0->ttl = 0;
            gAudioContext.sampleDmaReqCnt++;
        }
    }

    for (i = gAudioContext.sampleDmaListSize1; (u32)i < gAudioContext.sampleDmaReqCnt; i++) {
        gAudioContext.sampleDmaReuseQueue2[i - gAudioContext.sampleDmaListSize1] = i;
        gAudioContext.sampleDmaReqs[i].reuseIndex = i - gAudioContext.sampleDmaListSize1;
    }

    for (i = gAudioContext.sampleDmaReqCnt; i < 0x100; i++) {
        gAudioContext.sampleDmaReuseQueue2[i] = gAudioContext.sampleDmaListSize1;
    }

    gAudioContext.sampleDmaReuseQueue2RdPos = 0;
    gAudioContext.sampleDmaReuseQueue2WrPos = gAudioContext.sampleDmaReqCnt - gAudioContext.sampleDmaListSize1;
}

s32 Audio_IsBankLoadComplete(s32 bankId) {
    if (bankId == 0xFF) {
        return true;
    } else if (gAudioContext.bankLoadStatus[bankId] >= 2) {
        return true;
    } else if (gAudioContext.bankLoadStatus[Audio_GetTableIndex(BANK_TABLE, bankId)] >= 2) {
        return true;
    } else {
        return false;
    }
}

s32 Audio_IsSeqLoadComplete(s32 seqId) {
    if (seqId == 0xFF) {
        return true;
    } else if (gAudioContext.seqLoadStatus[seqId] >= 2) {
        return true;
    } else if (gAudioContext.seqLoadStatus[Audio_GetTableIndex(SEQUENCE_TABLE, seqId)] >= 2) {
        return true;
    } else {
        return false;
    }
}

s32 Audio_IsAudTabLoadComplete(s32 tabId) {
    if (tabId == 0xFF) {
        return true;
    } else if (gAudioContext.audioTableLoadStatus[tabId] >= 2) {
        return true;
    } else if (gAudioContext.audioTableLoadStatus[Audio_GetTableIndex(AUDIO_TABLE, tabId)] >= 2) {
        return true;
    } else {
        return false;
    }
}

void Audio_SetBankLoadStatus(s32 bankId, s32 status) {
    if ((bankId != 0xFF) && (gAudioContext.bankLoadStatus[bankId] != 5)) {
        gAudioContext.bankLoadStatus[bankId] = status;
    }
}

void Audio_SetSeqLoadStatus(s32 seqId, s32 status) {
    if ((seqId != 0xFF) && (gAudioContext.seqLoadStatus[seqId] != 5)) {
        gAudioContext.seqLoadStatus[seqId] = status;
    }
}

// OoT func_800E1A78
void func_8018F3E8(s32 arg0, s32 arg1) {
    if (arg0 != 0xFF) {
        if (gAudioContext.audioTableLoadStatus[arg0] != 5) {
            gAudioContext.audioTableLoadStatus[arg0] = arg1;
        }

        if ((gAudioContext.audioTableLoadStatus[arg0] == 5) || (gAudioContext.audioTableLoadStatus[arg0] == 2)) {
            func_8018E00C(arg0);
        }
    }
}

void Audio_SetAudtabLoadStatus(s32 tabId, s32 status) {
    if ((tabId != 0xFF) && (gAudioContext.audioTableLoadStatus[tabId] != 5)) {
        gAudioContext.audioTableLoadStatus[tabId] = status;
    }
}

void Audio_InitAudioTable(AudioTable* table, u32 romAddr, u16 arg2) {
    s32 i;

    table->header.unk_02 = arg2;
    table->header.romAddr = romAddr;

    for (i = 0; i < table->header.entryCnt; i++) {
        if ((table->entries[i].size != 0) && (table->entries[i].unk_08 == 2)) {
            table->entries[i].romAddr += romAddr;
        }
    }
}

// OoT func_800E1B68
AudioBankData* func_8018F4D8(s32 arg0, u32* arg1) {
    char pad[0x8];
    s32 phi_s0;
    AudioBankData* sp28;
    s32 phi_s1;
    s32 phi_s2;
    s32 i;

    if (arg0 >= gAudioContext.seqTabEntCnt) {
        return 0;
    }

    phi_s2 = 0xFF;
    phi_s0 = gAudioContext.unk_283C[arg0]; // ofset into unk_283C for cnt?
    phi_s1 = *(phi_s0 + gAudioContext.unk_283Cb);
    phi_s0++;

    while (phi_s1 > 0) {
        phi_s2 = gAudioContext.unk_283Cb[phi_s0++];
        sp28 = func_8018FE5C(phi_s2);
        phi_s1--;
    }

    *arg1 = phi_s2;
    return sp28;
}

// OoT func_800E1C18
#ifdef NON_EQUIVALENT
void func_8018F588(s32 channelIdx, s32 arg1) {
    s32 pad;
    u32 sp18;

    if (channelIdx < gAudioContext.seqTabEntCnt) {
        if (arg1 & 2) {
            func_8018F4D8(channelIdx, &sp18);
        }
        if (arg1 & 1) {
            func_8018FCCC(channelIdx);
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018F588.s")
#endif

// OoT func_800E1C78
#ifdef NON_EQUIVALENT
s32 func_8018F604(AudioBankSample* sample, s32 arg1) {
    void* sampleAddr;

    if (sample->unk_bits25 == 1) {
        if (sample->medium != 0) {
            sampleAddr = func_8018D658(sample->size, arg1, (void*)sample->sampleAddr, sample->medium, 1);
            if (sampleAddr == NULL) {
                return -1;
            }

            if (sample->medium == 1) {
                Audio_NoopCopy(sample->sampleAddr, sampleAddr, sample->size, gAudioContext.audioTable->header.unk_02);
            } else {
                Audio_DMAFastCopy(sample->sampleAddr, sampleAddr, sample->size, sample->medium);
            }
            sample->medium = 0;
            sample->sampleAddr = sampleAddr;
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018F604.s")
#endif

// OoT func_800E1D64
s32 func_8018F6F0(s32 arg0, s32 arg1, s32 arg2) {
    if (arg1 < 0x7F) {
        Instrument* instrument = Audio_GetInstrumentInner(arg0, arg1);

        if (instrument == NULL) {
            return -1;
        }
        if (instrument->normalRangeLo != 0) {
            func_8018F604(instrument->lowNotesSound.sample, arg0);
        }
        func_8018F604(instrument->normalNotesSound.sample, arg0);
        if (instrument->normalRangeHi != 0x7F) {
            return func_8018F604(instrument->highNotesSound.sample, arg0);
        }
    } else if (arg1 == 0x7F) {
        Drum* drum = Audio_GetDrum(arg0, arg2);

        if (drum == NULL) {
            return -1;
        }
        func_8018F604(drum->sound.sample, arg0);
        return 0;
    }
}

void Audio_AsyncLoad(s32 tableType, s32 arg1, s32 arg2, s32 arg3, OSMesgQueue* queue) {
    if (Audio_AsyncLoadInner(tableType, arg1, arg2, arg3, queue) == NULL) {
        osSendMesg(queue, -1, OS_MESG_NOBLOCK);
    }
}

void Audio_AudioSeqAsyncLoad(s32 arg0, s32 arg1, s32 arg2, OSMesgQueue* queue) {
    Audio_AsyncLoad(SEQUENCE_TABLE, arg0, 0, arg2, queue);
}

void Audio_AudioTableAsyncLoad(s32 arg0, s32 arg1, s32 arg2, OSMesgQueue* queue) {
    Audio_AsyncLoad(AUDIO_TABLE, arg0, 0, arg2, queue);
}

void Audio_AudioBankAsyncLoad(s32 arg0, s32 arg1, s32 arg2, OSMesgQueue* queue) {
    Audio_AsyncLoad(BANK_TABLE, arg0, 0, arg2, queue);
}

// OoT func_800E1F38
u8* func_8018F8C4(s32 arg0, u32* arg1) {
    s32 temp_v1;

    temp_v1 = gAudioContext.unk_283C[arg0];

    *arg1 = *(temp_v1 + gAudioContext.unk_283Cb);
    temp_v1++;
    if (*arg1 == 0) {
        return NULL;
    }
    return &gAudioContext.unk_283Cb[temp_v1];
}

// OoT func_800E1F7C
void func_8018F908(s32 arg0) {
    s32 temp_s0;
    s32 phi_s1;
    s32 phi_s2;

    phi_s1 = gAudioContext.unk_283C[arg0];
    phi_s2 = *(phi_s1 + gAudioContext.unk_283Cb);
    phi_s1++;

    while (phi_s2 > 0) {
        phi_s2--;
        temp_s0 = Audio_GetTableIndex(BANK_TABLE, gAudioContext.unk_283Cb[phi_s1++]);
        if (func_8018D57C(BANK_TABLE, temp_s0) == NULL) {
            func_8018F9B8(temp_s0);
            Audio_SetBankLoadStatus(temp_s0, 0);
        }
    }
}

// OoT func_800E202C
void func_8018F9B8(s32 arg0) {
    u32 i;
    SoundMultiPool* pool = &gAudioContext.bankLoadedPool;
    PersistentPool* persistent;

    if (arg0 == pool->temporary.entries[0].id) {
        pool->temporary.entries[0].id = -1;
    } else if (arg0 == pool->temporary.entries[1].id) {
        pool->temporary.entries[1].id = -1;
    }

    persistent = &pool->persistent;
    for (i = 0; i < persistent->numEntries; i++) {
        if (arg0 == persistent->entries[i].id) {
            persistent->entries[i].id = -1;
        }
    }

    Audio_DiscardBank(arg0);
}

// New to MM?
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018FA60.s")

// OoT func_800E20D4
s32 func_8018FAD0(s32 playerIndex, s32 seqId, s32 arg2) {
    if (gAudioContext.resetTimer != 0) {
        return 0;
    } else {
        gAudioContext.seqPlayers[playerIndex].unk_DC = 0;
        return func_8018FB78(playerIndex, seqId, arg2);
    }
}

// func_800E2124
s32 func_8018FB20(s32 playerIndex, s32 seqId, s32 arg2) {
    if (gAudioContext.resetTimer != 0) {
        return 0;
    }

    gAudioContext.seqPlayers[playerIndex].unk_DC = arg2;
    return func_8018FB78(playerIndex, seqId, 0);
}

// OoT func_800E217C (InitSeqPlayer)
s32 func_8018FB78(s32 playerIndex, s32 seqId, s32 arg2) {
    SequencePlayer* seqPlayer = &gAudioContext.seqPlayers[playerIndex];
    u8* seqData;
    s32 phi_s0;
    s32 phi_s1;
    s32 phi_s2;

    if (seqId >= gAudioContext.seqTabEntCnt) {
        return 0;
    }

    Audio_SequencePlayerDisable(seqPlayer);

    if (1) {}
    phi_s2 = 0xFF;
    phi_s0 = gAudioContext.unk_283C[seqId];
    phi_s1 = gAudioContext.unk_283Cb[phi_s0++];

    while (phi_s1 > 0) {
        phi_s2 = gAudioContext.unk_283Cb[phi_s0++];
        func_8018FE5C(phi_s2);
        phi_s1--;
    }

    seqData = func_8018FCCC(seqId);
    if (seqData == NULL) {
        return 0;
    }

    Audio_ResetSequencePlayer(seqPlayer);
    seqPlayer->seqId = seqId;

    if (phi_s2 != 0xFF) {
        seqPlayer->defaultBank = Audio_GetTableIndex(BANK_TABLE, phi_s2);
    } else {
        seqPlayer->defaultBank = 0xFF;
    }

    seqPlayer->seqData = seqData;
    seqPlayer->enabled = 1;
    seqPlayer->scriptState.pc = seqData;
    seqPlayer->scriptState.depth = 0;
    seqPlayer->delay = 0;
    seqPlayer->finished = 0;
    seqPlayer->playerIndex = playerIndex;
}

// OoT func_800E22C4 (LoadSequence)
u8* func_8018FCCC(s32 seqId) {
    s32 pad;
    s32 sp20;

    if (gAudioContext.seqLoadStatus[Audio_GetTableIndex(SEQUENCE_TABLE, seqId)] == 1) {
        return NULL;
    }

    return func_8018FF60(SEQUENCE_TABLE, seqId, &sp20);
}

// OoT func_800E2318
u32 func_8018FD20(u32 arg0, u32* arg1) {
    return func_8018FD40(arg0, arg1, 1);
}

// OoT func_800E2338
u32 func_8018FD40(u32 arg0, u32* arg1, s32 arg2) {
    void* temp_v0;
    AudioTable* audioTable;
    u32 temp_s0 = Audio_GetTableIndex(AUDIO_TABLE, arg0);
    s8 tmp;

    audioTable = func_80190294(AUDIO_TABLE);
    if (temp_v0 = func_80190240(AUDIO_TABLE, temp_s0), temp_v0 != NULL) {
        if (gAudioContext.audioTableLoadStatus[temp_s0] != 1) {
            Audio_SetAudtabLoadStatus(temp_s0, 2);
        }
        *arg1 = 0;
        return temp_v0;
    } else if (tmp = audioTable->entries[arg0].unk_09, tmp == 4 || arg2 == 1) {
        *arg1 = audioTable->entries[arg0].unk_08;
        return audioTable->entries[temp_s0].romAddr;
    } else if (temp_v0 = func_8018FF60(AUDIO_TABLE, arg0, &arg2), temp_v0 != NULL) {
        *arg1 = 0;
        return temp_v0;
    } else {
        *arg1 = audioTable->entries[arg0].unk_08;
        return audioTable->entries[temp_s0].romAddr;
    }
}

// OoT func_800E2454
AudioBankData* func_8018FE5C(u32 bankId) {
    AudioBankData* temp_ret;
    s32 unk02;
    s32 unk03;
    s32 sp38;
    RelocInfo relocInfo;
    s32 idx = Audio_GetTableIndex(BANK_TABLE, bankId);

    if (gAudioContext.bankLoadStatus[idx] == 1) {
        return NULL;
    }
    unk02 = gAudioContext.ctlEntries[idx].unk_02;
    unk03 = gAudioContext.ctlEntries[idx].unk_03;

    relocInfo.index1 = unk02;
    relocInfo.index2 = unk03;
    if (relocInfo.index1 != 0xFF) {
        relocInfo.baseAddr1 = func_8018FD40(relocInfo.index1, &relocInfo.medium1, 0);
    } else {
        relocInfo.baseAddr1 = 0;
    }

    if (unk03 != 0xFF) {
        relocInfo.baseAddr2 = func_8018FD40(unk03, &relocInfo.medium2, 0);
    } else {
        relocInfo.baseAddr2 = 0;
    }

    temp_ret = func_8018FF60(BANK_TABLE, bankId, &sp38);
    if (temp_ret == NULL) {
        return NULL;
    }
    if (sp38 == 1) {
        func_80191D94(idx, temp_ret, &relocInfo, 0);
    }

    return temp_ret;
}

#ifdef NON_EQUIVALENT
void* func_8018FF60(u32 tableType, u32 tableIdx, s32* didAllocate) {
    u32 size;
    AudioTable* table;
    s32 pad;
    u32 sp40;
    s32 status;
    u32 romAddr;
    s32 sp24;
    void* ret;
    u32 id;

    id = Audio_GetTableIndex(tableType, tableIdx);
    ret = func_80190240(tableType, id);
    if (ret != NULL) {
        *didAllocate = false;
        status = 2;
    } else {
        table = func_80190294(tableType);
        size = table->entries[id].size;
        size = ALIGN16(size);
        sp40 = table->entries[tableIdx].unk_08;
        sp24 = table->entries[tableIdx].unk_09;
        romAddr = table->entries[id].romAddr;
        switch (sp24) {
            case 0:
                ret = func_8018D5D4(tableType, id, size);
                if (ret == NULL) {
                    return ret;
                }
                break;
            case 1:
                ret = Audio_AllocBankOrSeq(tableType, size, 1, id);
                if (ret == NULL) {
                    return ret;
                }
                break;
            case 2:
                ret = Audio_AllocBankOrSeq(tableType, size, 0, id);
                if (ret == NULL) {
                    return ret;
                }
                break;
            case 3:
            case 4:
                ret = Audio_AllocBankOrSeq(tableType, size, 2, id);
                if (ret == NULL) {
                    return ret;
                }
                break;
        }

        *didAllocate = true;

        if (sp40 == 5) {
            if (romAddr == NULL) {
                return NULL;
            }
            if (tableType == BANK_TABLE) {
                // AudioTableEntry D_802034D8 ???
                // temp_v0_8 = (sp34 * 0x14) + D_802034D8;
                // temp_v0_8->unk0 = romAddr->unk0;
                // temp_v0_8->unk1 = romAddr->unk2;
                // temp_v0_8->unk4 = romAddr->unk4;
                romAddr += 0x10;
                size += -0x10;
            }
            bcopy(sp40, ret, size);
        } else if (sp40 == 1) {
            Audio_NoopCopy(romAddr, ret, size, (s16)table->header.unk_02);
        } else {
            Audio_DMAFastCopy(romAddr, ret, size, sp40);
        }

        status = sp24 == 0 ? 5 : 2;
    }

    switch (tableType) {
        case SEQUENCE_TABLE:
            Audio_SetSeqLoadStatus(id, status);
            break;
        case BANK_TABLE:
            Audio_SetBankLoadStatus(id, status);
            break;
        case AUDIO_TABLE:
            func_8018F3E8(id, status);
            break;
        default:
            break;
    }

    return ret;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018FF60.s")
#endif

u32 Audio_GetTableIndex(s32 tableType, u32 tableIdx) {
    AudioTable* table = func_80190294(tableType);

    if (table->entries[tableIdx].size == 0) {
        tableIdx = table->entries[tableIdx].romAddr;
    }

    return tableIdx;
}

// OoT func_800E27A4
void* func_80190240(s32 tableType, s32 id) {
    void* ret;

    ret = func_8018D57C(tableType, id);
    if (ret != NULL) {
        return ret;
    }

    ret = func_8018C380(tableType, 2, id);
    if (ret != NULL) {
        return ret;
    }

    return NULL;
}

// Audio_GetLoadTable
void* func_80190294(s32 tableType) {
    void* ret;

    switch (tableType) {
        case SEQUENCE_TABLE:
            ret = gAudioContext.sequenceTable;
            break;
        case BANK_TABLE:
            ret = gAudioContext.audioBankTable;
            break;
        default:
            ret = NULL;
            break;
        case AUDIO_TABLE:
            ret = gAudioContext.audioTable;
            break;
    }
    return ret;
}

#define BASE_OFFSET(x, off) (void*)((u32)(x) + (u32)(off))

// OoT func_800E283C
void func_801902D8(s32 bankId, AudioBankData* mem, RelocInfo* relocInfo) {
    u32 reloc;
    u32 reloc2;
    Instrument* inst;
    Drum* drum;
    AudioBankSound* sfx;
    s32 i;
    s32 numDrums = gAudioContext.ctlEntries[bankId].numDrums;
    s32 numInstruments = gAudioContext.ctlEntries[bankId].numInstruments;
    s32 numSfx = gAudioContext.ctlEntries[bankId].numSfx;
    void** ptrs = (void**)mem;

    reloc2 = ptrs[0];
    if (1) {}
    if ((reloc2 != 0) && (numDrums != 0)) {
        ptrs[0] = BASE_OFFSET(reloc2, mem);
        for (i = 0; i < numDrums; i++) {
            reloc = ((Drum**)ptrs[0])[i];
            if (reloc != 0) {
                reloc = BASE_OFFSET(reloc, mem);
                ((Drum**)ptrs[0])[i] = drum = reloc;
                if (!drum->loaded) {
                    func_80191C54(&drum->sound, mem, relocInfo);
                    reloc = drum->envelope;
                    drum->envelope = BASE_OFFSET(reloc, mem);
                    drum->loaded = 1;
                }
            }
        }
    }

    reloc2 = ptrs[1];
    if (1) {}
    if ((reloc2 != 0) && (numSfx != 0)) {
        ptrs[1] = BASE_OFFSET(reloc2, mem);
        for (i = 0; i < numSfx; i++) {
            reloc = (AudioBankSound*)ptrs[1] + i;
            if (reloc != 0) {
                sfx = reloc;
                if (sfx->sample != NULL) {
                    func_80191C54(sfx, mem, relocInfo);
                }
            }
        }
    }

    if (numInstruments > 0x7E) {
        numInstruments = 0x7E;
    }

    for (i = 2; i <= 2 + numInstruments - 1; i++) {
        if (ptrs[i] != NULL) {
            ptrs[i] = BASE_OFFSET(ptrs[i], mem);
            inst = ptrs[i];
            if (!inst->loaded) {
                if (inst->normalRangeLo != 0) {
                    func_80191C54(&inst->lowNotesSound, mem, relocInfo);
                }
                func_80191C54(&inst->normalNotesSound, mem, relocInfo);
                if (inst->normalRangeHi != 0x7F) {
                    func_80191C54(&inst->highNotesSound, mem, relocInfo);
                }

                reloc = inst->envelope;
                inst->envelope = BASE_OFFSET(reloc, mem);
                inst->loaded = 1;
            }
        }
    }

    gAudioContext.ctlEntries[bankId].drums = ptrs[0];
    gAudioContext.ctlEntries[bankId].soundEffects = ptrs[1];
    gAudioContext.ctlEntries[bankId].instruments = (Instrument**)(ptrs + 2);
}

#undef BASE_OFFSET

void Audio_DMAFastCopy(u32 devAddr, u8* addr, u32 size, s32 handleType) {
    OSMesgQueue* msgQueue = &gAudioContext.unk_25E8;
    OSIoMesg* ioMesg = &gAudioContext.unk_2604;
    size = ALIGN16(size);

    Audio_osInvalDCache(addr, size);

    while (true) {
        if (size < 0x400) {
            break;
        }
        Audio_DMA(ioMesg, OS_MESG_PRI_HIGH, OS_READ, devAddr, addr, 0x400, msgQueue, handleType, "FastCopy");
        osRecvMesg(msgQueue, NULL, OS_MESG_BLOCK);
        size -= 0x400;
        devAddr += 0x400;
        addr += 0x400;
    }

    if (size != 0) {
        Audio_DMA(ioMesg, OS_MESG_PRI_HIGH, OS_READ, devAddr, addr, size, msgQueue, handleType, "FastCopy");
        osRecvMesg(msgQueue, NULL, OS_MESG_BLOCK);
    }
}

void Audio_NoopCopy(u32 devAddr, u8* addr, u32 size, s32 handleType) {
}

s32 Audio_DMA(OSIoMesg* mesg, u32 priority, s32 direction, u32 devAddr, void* ramAddr, u32 size, OSMesgQueue* reqQueue,
              s32 handleType, const char* dmaFuncType) {
    OSPiHandle* handle;

    if (gAudioContext.resetTimer > 0x10) {
        return -1;
    }

    switch (handleType) {
        case 2:
            handle = gAudioContext.cartHandle;
            break;
        case 3:
            handle = gAudioContext.unk_1E1C;
            break;
        default:
            return 0;
    }

    if ((size % 0x10) != 0) {
        size = ALIGN16(size);
    }

    mesg->hdr.pri = priority;
    mesg->hdr.retQueue = reqQueue;
    mesg->dramAddr = ramAddr;
    mesg->devAddr = devAddr;
    mesg->size = size;
    handle->transferInfo.cmdType = 2;
    sDmaHandler(handle, mesg, direction);
    return 0;
}

// OoT func_800E2CB8
void func_80190754(void) {
}

// OoT func_800E2CC0
void func_8019075C(u32 tableType, u32 bankId) {
    s32 sp1C;

    func_8018FF60(tableType, bankId, &sp1C);
}

#ifdef NON_EQUIVALENT
void* Audio_AsyncLoadInner(s32 tableType, s32 arg1, s32 arg2, s32 arg3, OSMesgQueue* retQueue) {
    u32 size;
    AudioTable* sp50;
    void* sp4C;
    s32 sp48;
    s8 temp_a1;
    u32 devAddr;
    s32 sp3C;
    u32 temp_v0;
    u32 sp34;

    sp34 = Audio_GetTableIndex(tableType, arg1);
    switch (tableType) {
        case SEQUENCE_TABLE:
            if (gAudioContext.seqLoadStatus[sp34] == 1) {
                return NULL;
            }
            break;
        case BANK_TABLE:
            if (gAudioContext.bankLoadStatus[sp34] == 1) {
                return NULL;
            }
            break;
        case AUDIO_TABLE:
            if (gAudioContext.audioTableLoadStatus[sp34] == 1) {
                return NULL;
            }
            break;
    }

    sp4C = func_800E27A4(tableType, sp34);
    if (sp4C != NULL) {
        sp3C = 2;
        osSendMesg(retQueue, arg3 << 0x18, 0);
    } else {
        sp50 = Audio_GetLoadTable(tableType);
        size = sp50->entries[sp34].size;
        size = ALIGN16(size);
        sp48 = sp50->entries[arg1].unk_08;
        temp_a1 = sp50->entries[arg1].unk_09;
        devAddr = sp50->entries[sp34].romAddr;
        sp3C = 2;
        switch (temp_a1) {
            case 0:
                sp4C = func_800E0540(tableType, sp34, size);
                if (sp4C == NULL) {
                    return sp4C;
                }
                sp3C = 5;
                break;
            case 1:
                sp4C = Audio_AllocBankOrSeq(tableType, size, 1, sp34);
                if (sp4C == NULL) {
                    return sp4C;
                }
                break;
            case 2:
                sp4C = Audio_AllocBankOrSeq(tableType, size, 0, sp34);
                if (sp4C == NULL) {
                    return sp4C;
                }
                break;
            case 3:
            case 4:
                sp4C = Audio_AllocBankOrSeq(tableType, size, 2, sp34);
                if (sp4C == NULL) {
                    return sp4C;
                }
                break;
        }

        if (sp48 == 1) {
            func_800E3A44((s16)sp50->header.unk_02, devAddr, sp4C, size, sp48, arg2, retQueue,
                          (arg3 << 0x18) | (tableType << 0x10) | (arg1 << 8) | sp3C);
        } else {
            Audio_InitAsyncReq(devAddr, sp4C, size, sp48, arg2, retQueue, MK_ASYNC_MSG(arg3, tableType, sp34, sp3C));
        }
        sp3C = 1;
    }

    switch (tableType) {
        case 0:
            Audio_SetSeqLoadStatus(sp34, sp3C);
            break;
        case 1:
            Audio_SetBankLoadStatus(sp34, sp3C);
            break;
        case 2:
            func_8018F3E8(sp34, sp3C);
            break;
        default:
            break;
    }

    return sp4C;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/Audio_AsyncLoadInner.s")
#endif

// Audio_ProcessLoads
void func_80190B08(s32 resetStatus) {
    func_80191240(resetStatus);
    func_8019218C(resetStatus);
    func_80191740(resetStatus);
}

// OoT func_800E301C (SetDmaCallback)
void func_80190B38(void* callback) {
    sDmaHandler = callback;
}

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80190B44.s")


// OoT func_800E3034
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80190B50.s")

#ifdef NON_EQUIVALENT
void Audio_ContextInit(void* heap, u32 heapSize) {
    char pad[0x48];
    s32 sp24;
    void* temp_v0_3;
    s32 i;
    u64* heapP;
    u8* ctxP;
    s16* u2974p;

    // Something funny happens when this data is externed
    D_80208E68 = 0;
    D_80208E70 = 0;
    D_80208E74 = 0;
    D_80203618 = 0;
    D_8020361C = 0;
    D_80203620 = 0;
    D_80203624 = 0;

    // D_801755D0 = NULL;
    gAudioContext.resetTimer = 0;
    gAudioContext.unk_29A8 = 0; // s8?

    {
        s32 i;
        u8* ctxP = (u8*)&gAudioContext;
        for (i = sizeof(gAudioContext); i >= 0; i--) {
            *ctxP++ = 0;
        }
    }

    switch (osTvType) {
        case OS_TV_PAL:
            gAudioContext.unk_2960 = 20.03042f;
            gAudioContext.refreshRate = 50;
            break;
        case OS_TV_MPAL:
            gAudioContext.unk_2960 = 16.546f;
            gAudioContext.refreshRate = 60;
            break;
        case OS_TV_NTSC:
        default:
            gAudioContext.unk_2960 = 16.713f;
            gAudioContext.refreshRate = 60;
    }

    func_801946E4(); 

    for (i = 0; i < 3; i++) {
        gAudioContext.aiBufLengths[i] = 0xA0;
    }

    gAudioContext.totalTaskCnt = 0;
    gAudioContext.rspTaskIdx = 0;
    gAudioContext.curAIBufIdx = 0;
    gAudioContext.soundMode = 0;
    gAudioContext.currTask = NULL;
    gAudioContext.rspTask[0].task.t.data_size = 0;
    gAudioContext.rspTask[1].task.t.data_size = 0;
    osCreateMesgQueue(&gAudioContext.unk_25E8, &gAudioContext.unk_2600, 1);
    osCreateMesgQueue(&gAudioContext.currAudioFrameDmaQueue, gAudioContext.currAudioFrameDmaMesgBufs, 0x40);
    osCreateMesgQueue(&gAudioContext.unk_1E20, gAudioContext.unk_1E38, 0x10);
    osCreateMesgQueue(&gAudioContext.unk_1E78, gAudioContext.unk_1E90, 0x10);
    gAudioContext.curAudioFrameDmaCount = 0;
    gAudioContext.sampleDmaReqCnt = 0;
    gAudioContext.cartHandle = osCartRomInit();

    if (heap == NULL) {
        gAudioContext.audioHeap = gAudioHeap;
        gAudioContext.audioHeapSize = D_801E1104.heap;
    } else {
        void** hp = &heap;
        gAudioContext.audioHeap = *hp;
        gAudioContext.audioHeapSize = heapSize;
    }

    for (i = 0; i < (s32)gAudioContext.audioHeapSize / 8; i++) {
        ((u64*)gAudioContext.audioHeap)[i] = 0;
    }

    Audio_InitMainPools(D_801E1104.mainPool);

    for (i = 0; i < 3; i++) {
        gAudioContext.aiBuffers[i] = Audio_AllocZeroed(&gAudioContext.audioInitPool, AIBUF_LEN);
    }

    gAudioContext.sequenceTable = &gSequenceTable;
    gAudioContext.audioBankTable = &gAudioBankTable;
    gAudioContext.audioTable = &gAudioTable;
    gAudioContext.unk_283C = &D_801E1420;
    gAudioContext.seqTabEntCnt = gAudioContext.sequenceTable->header.entryCnt;

    gAudioContext.audioResetSpecIdToLoad = 0;
    gAudioContext.resetStatus = 1;

    Audio_ResetStep();
    Audio_InitAudioTable(gAudioContext.sequenceTable, _AudioseqSegmentRomStart, 0);
    Audio_InitAudioTable(gAudioContext.audioBankTable, _AudiobankSegmentRomStart, 0);
    Audio_InitAudioTable(gAudioContext.audioTable, _AudiotableSegmentRomStart, 0);
    sp24 = gAudioContext.audioBankTable->header.entryCnt;
    gAudioContext.ctlEntries = Audio_Alloc(&gAudioContext.audioInitPool, sp24 * sizeof(CtlEntry));

    for (i = 0; i < sp24; i++) {
        func_80190B50(i);
    }

    if (temp_v0_3 = Audio_Alloc(&gAudioContext.audioInitPool, D_801E1104.initPool), temp_v0_3 == NULL) {

        *((u32*)&D_801E1104.initPool) = 0;
    }

    Audio_SoundAllocPoolInit(&gAudioContext.unk_2D50, temp_v0_3, D_801E1104.initPool);
    gAudioContextInitalized = 1;
    osSendMesg(gAudioContext.taskStartQueueP, (void*)gAudioContext.totalTaskCnt, 0);
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/Audio_ContextInit.s")
#endif

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80190F50.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80190F64.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80191134.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_801911C4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_801911CC.s")

// Audio_ProcessSyncLoads
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80191240.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_801913C8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8019144C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80191460.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80191568.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80191598.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8019161C.s")

// Audio_ProcessAsyncLoads
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80191740.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80191864.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80191870.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_801919AC.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80191B40.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80191BD0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80191C40.s")

// Audio_SampleReloc
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80191C54.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80191D94.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8019218C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80192340.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80192388.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_801924BC.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80192514.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80192990.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80192AD0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80192AD8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80192AE0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80192AE8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80192B54.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80192BAC.s")
