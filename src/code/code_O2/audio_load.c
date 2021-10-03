#include "global.h"

// opaque type for unpatched audio bank data (should maybe get rid of this?)
typedef void AudioBankData;

AudioBankData* func_8018FE5C(u32 bankId);
void func_80191D94(s32 bankId, AudioBankData* mem, RelocInfo* relocInfo, s32 arg3);

typedef enum { LOAD_STATUS_WAITING, LOAD_STATUS_START, LOAD_STATUS_LOADING, LOAD_STATUS_DONE } SyncLoadStatus;

#define OS_MESG_PRI_NORMAL  0
#define OS_MESG_PRI_HIGH    1

typedef struct {
    u16 numInstruments;
    u16 numDrums;
    u16 numSfx;
} Struct_8018FF60;

// OoT func_800E11F0
void func_8018EB60(void) {
    u32 i;

    for (i = 0; i < gAudioContext.sampleDmaListSize1; i++) {
        SampleDma* req = &gAudioContext.sampleDmas[i];

        if (req->ttl != 0) {
            req->ttl--;
            if (req->ttl == 0) {
                req->reuseIndex = gAudioContext.sampleDmaReuseQueue1WrPos;
                gAudioContext.sampleDmaReuseQueue1[gAudioContext.sampleDmaReuseQueue1WrPos] = i;
                gAudioContext.sampleDmaReuseQueue1WrPos++;
            }
        }
    }

    for (i = gAudioContext.sampleDmaListSize1; i < gAudioContext.sampleDmaCount; i++) {
        SampleDma* req = &gAudioContext.sampleDmas[i];

        if (req->ttl != 0) {
            req->ttl--;
            if (req->ttl == 0) {
                req->reuseIndex = gAudioContext.sampleDmaReuseQueue2WrPos;
                gAudioContext.sampleDmaReuseQueue2[gAudioContext.sampleDmaReuseQueue2WrPos] = i;
                gAudioContext.sampleDmaReuseQueue2WrPos++;
            }
        }
    }

    gAudioContext.unused2628 = 0;
}

void* Audio_DmaSampleData(u32 devAddr, u32 size, s32 arg2, u8* dmaIndexRef, s32 medium) {
    s32 sp60;
    SampleDma* dma;
    s32 hasDma = false;
    u32 dmaDevAddr;
    u32 pad;
    u32 dmaIndex;
    u32 transfer;
    s32 bufferPos;
    u32 i;

    if (arg2 != 0 || *dmaIndexRef >= gAudioContext.sampleDmaListSize1) {
        for (i = gAudioContext.sampleDmaListSize1; i < gAudioContext.sampleDmaCount; i++) {
            dma = &gAudioContext.sampleDmas[i];
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
                            .sampleDmas[gAudioContext.sampleDmaReuseQueue2[gAudioContext.sampleDmaReuseQueue2RdPos]]
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
            dma = gAudioContext.sampleDmas + dmaIndex;
            hasDma = true;
        }
    } else {
    search_short_lived:
        dma = gAudioContext.sampleDmas + *dmaIndexRef;
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
                        .sampleDmas[gAudioContext.sampleDmaReuseQueue1[gAudioContext.sampleDmaReuseQueue1RdPos]]
                        .reuseIndex = dma->reuseIndex;
                }
                gAudioContext.sampleDmaReuseQueue1RdPos++;
            }
            dma->ttl = 2;
            return dma->ramAddr + (devAddr - dma->devAddr);
        }
        dma = gAudioContext.sampleDmas + i++;
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
        dma = gAudioContext.sampleDmas + dmaIndex;
        hasDma = true;
    }

    transfer = dma->size;
    dmaDevAddr = devAddr & ~0xF;
    dma->ttl = 3;
    dma->devAddr = dmaDevAddr;
    dma->sizeUnused = transfer;
    Audio_DMA(&gAudioContext.currAudioFrameDmaIoMesgBuf[gAudioContext.curAudioFrameDmaCount++], OS_MESG_PRI_NORMAL,
              OS_READ, dmaDevAddr, dma->ramAddr, transfer, &gAudioContext.currAudioFrameDmaQueue, medium, "SUPERDMA");
    *dmaIndexRef = dmaIndex;
    return (devAddr - dmaDevAddr) + dma->ramAddr;
}

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/D_801E030C.s")

// OoT func_800E1618
void func_8018EF88(s32 arg0) {
    SampleDma* temp_s0;
    s32 i;
    s32 t2;
    s32 j;

    gAudioContext.sampleDmaBufSize = gAudioContext.sampleDmaBufSize1;
    gAudioContext.sampleDmas =
        Audio_Alloc(&gAudioContext.notesAndBuffersPool, 4 * gAudioContext.numNotes * sizeof(SampleDma) *
                                                            gAudioContext.audioBufferParameters.specUnk4);
    t2 = 3 * gAudioContext.numNotes * gAudioContext.audioBufferParameters.specUnk4;
    for (i = 0; i < t2; i++) {
        temp_s0 = &gAudioContext.sampleDmas[gAudioContext.sampleDmaCount];
        temp_s0->ramAddr = func_8018B578(&gAudioContext.notesAndBuffersPool, gAudioContext.sampleDmaBufSize);
        if (temp_s0->ramAddr == NULL) {
            break;
        } else {
            func_8018B4F8(temp_s0->ramAddr, gAudioContext.sampleDmaBufSize);
            temp_s0->size = gAudioContext.sampleDmaBufSize;
            temp_s0->devAddr = 0;
            temp_s0->sizeUnused = 0;
            temp_s0->unused = 0;
            temp_s0->ttl = 0;
            gAudioContext.sampleDmaCount++;
        }
    }

    for (i = 0; (u32)i < gAudioContext.sampleDmaCount; i++) {
        gAudioContext.sampleDmaReuseQueue1[i] = i;
        gAudioContext.sampleDmas[i].reuseIndex = i;
    }

    for (i = gAudioContext.sampleDmaCount; i < 0x100; i++) {
        gAudioContext.sampleDmaReuseQueue1[i] = 0;
    }

    gAudioContext.sampleDmaReuseQueue1RdPos = 0;
    gAudioContext.sampleDmaReuseQueue1WrPos = gAudioContext.sampleDmaCount;
    gAudioContext.sampleDmaListSize1 = gAudioContext.sampleDmaCount;
    gAudioContext.sampleDmaBufSize = gAudioContext.sampleDmaBufSize2;

    for (j = 0; j < gAudioContext.numNotes; j++) {
        temp_s0 = &gAudioContext.sampleDmas[gAudioContext.sampleDmaCount];
        temp_s0->ramAddr = func_8018B578(&gAudioContext.notesAndBuffersPool, gAudioContext.sampleDmaBufSize);
        if (temp_s0->ramAddr == NULL) {
            break;
        } else {
            func_8018B4F8(temp_s0->ramAddr, gAudioContext.sampleDmaBufSize);
            temp_s0->size = gAudioContext.sampleDmaBufSize;
            temp_s0->devAddr = 0U;
            temp_s0->sizeUnused = 0;
            temp_s0->unused = 0;
            temp_s0->ttl = 0;
            gAudioContext.sampleDmaCount++;
        }
    }

    for (i = gAudioContext.sampleDmaListSize1; (u32)i < gAudioContext.sampleDmaCount; i++) {
        gAudioContext.sampleDmaReuseQueue2[i - gAudioContext.sampleDmaListSize1] = i;
        gAudioContext.sampleDmas[i].reuseIndex = i - gAudioContext.sampleDmaListSize1;
    }

    for (i = gAudioContext.sampleDmaCount; i < 0x100; i++) {
        gAudioContext.sampleDmaReuseQueue2[i] = gAudioContext.sampleDmaListSize1;
    }

    gAudioContext.sampleDmaReuseQueue2RdPos = 0;
    gAudioContext.sampleDmaReuseQueue2WrPos = gAudioContext.sampleDmaCount - gAudioContext.sampleDmaListSize1;
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
    } else if (gAudioContext.sampleBankLoadStatus[tabId] >= 2) {
        return true;
    } else if (gAudioContext.sampleBankLoadStatus[Audio_GetTableIndex(SAMPLE_TABLE, tabId)] >= 2) {
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
        if (gAudioContext.sampleBankLoadStatus[arg0] != 5) {
            gAudioContext.sampleBankLoadStatus[arg0] = arg1;
        }

        if ((gAudioContext.sampleBankLoadStatus[arg0] == 5) || (gAudioContext.sampleBankLoadStatus[arg0] == 2)) {
            func_8018E00C(arg0);
        }
    }
}

void Audio_SetAudtabLoadStatus(s32 tabId, s32 status) {
    if ((tabId != 0xFF) && (gAudioContext.sampleBankLoadStatus[tabId] != 5)) {
        gAudioContext.sampleBankLoadStatus[tabId] = status;
    }
}

void Audio_InitAudioTable(SampleBankTable* table, u32 romAddr, u16 arg2) {
    s32 i;

    table->header.unk_02 = arg2;
    table->header.romAddr = romAddr;

    for (i = 0; i < table->header.entryCnt; i++) {
        if ((table->entries[i].size != 0) && (table->entries[i].medium == 2)) {
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

    if (arg0 >= gAudioContext.numSequences) {
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
void func_8018F588(s32 channelIdx, s32 arg1, s32 arg2, OSMesgQueue* arg3) {
    s32 pad;
    u32 sp18;

    if (channelIdx < gAudioContext.numSequences) {
        if (arg1 & 2) {
            func_8018F4D8(channelIdx, &sp18);
        }
        if (arg1 & 1) {
            func_8018FCCC(channelIdx);
        }
        if (arg2 != 0) {
            osSendMesg(arg3, arg2 << 0x18, 0);
        }
    }
}

// OoT func_800E1C78
s32 func_8018F604(AudioBankSample* sample, s32 arg1) {
    void* sampleAddr;

    if (sample->unk_bits25 == 1) {
        if (sample->medium != 0) {
            sampleAddr = func_8018D658(sample->size, arg1, (void*)sample->sampleAddr, sample->medium, 1);
            if (sampleAddr == NULL) {
                return -1;
            }

            if (sample->medium == 1) {
                Audio_NoopCopy(sample->sampleAddr, sampleAddr, sample->size, gAudioContext.sampleBankTable->header.unk_02);
            } else {
                Audio_DMAFastCopy(sample->sampleAddr, sampleAddr, sample->size, sample->medium);
            }
            sample->medium = 0;
            sample->sampleAddr = sampleAddr;
        }
    }
}

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
    Audio_AsyncLoad(SAMPLE_TABLE, arg0, 0, arg2, queue);
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
    AudioCache* pool = &gAudioContext.bankCache;
    AudioPersistentCache* persistent;

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

// New to MM
void func_8018FA60(u32 tableType, u32 tableIdx, s32 arg2, s32 arg3) {
    SampleBankTable* table = Audio_GetLoadTable(tableType);

    if (table->entries[tableIdx].medium == 5) {
        switch (arg2) {
            case 0:
                table->entries[tableIdx].romAddr = arg3;
                break;
            case 1:
                table->entries[tableIdx].size = arg3;
                break;
            default:
                break;
        }
    }
}

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

    if (seqId >= gAudioContext.numSequences) {
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
    SampleBankTable* audioTable;
    u32 temp_s0 = Audio_GetTableIndex(SAMPLE_TABLE, arg0);
    s8 tmp;

    audioTable = Audio_GetLoadTable(SAMPLE_TABLE);
    if (temp_v0 = func_80190240(SAMPLE_TABLE, temp_s0), temp_v0 != NULL) {
        if (gAudioContext.sampleBankLoadStatus[temp_s0] != 1) {
            Audio_SetAudtabLoadStatus(temp_s0, 2);
        }
        *arg1 = 0;
        return temp_v0;
    } else if (tmp = audioTable->entries[arg0].unk_09, tmp == 4 || arg2 == 1) {
        *arg1 = audioTable->entries[arg0].medium;
        return audioTable->entries[temp_s0].romAddr;
    } else if (temp_v0 = func_8018FF60(SAMPLE_TABLE, arg0, &arg2), temp_v0 != NULL) {
        *arg1 = 0;
        return temp_v0;
    } else {
        *arg1 = audioTable->entries[arg0].medium;
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

void* func_8018FF60(s32 tableType, u32 tableIdx, s32* didAllocate) {
    u32 size;
    SampleBankTable* table;
    s32 sp40_2;
    u32 sp40;
    s32 status;
    u32 romAddr;
    s32 sp24;
    void* ret;
    u32 id;
    s32 one = 1;

    id = Audio_GetTableIndex(tableType, tableIdx);
    ret = func_80190240(tableType, id);
    if (ret != NULL) {
        *didAllocate = false;
        status = 2;
    } else {
        table = Audio_GetLoadTable(tableType);
        size = table->entries[id].size;
        size = ALIGN16(size);
        sp40 = table->entries[tableIdx].medium;
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

        sp40_2 = sp40;
        if (sp40_2 == 5) {
            if (romAddr == NULL) {
                return NULL;
            }
            if (tableType == one) {
                CtlEntry* ctlEntry = &gAudioContext.ctlEntries[id];
                ctlEntry->numInstruments = ((Struct_8018FF60*)romAddr)->numInstruments;
                ctlEntry->numDrums = ((Struct_8018FF60*)romAddr)->numDrums;
                ctlEntry->numSfx = ((Struct_8018FF60*)romAddr)->numSfx;
                romAddr += 0x10;
                size += -0x10;
            }
            bcopy(romAddr, ret, size);
        } else if (sp40_2 == one) {
            Audio_NoopCopy(romAddr, ret, size, (s16)table->header.unk_02);
        } else {
            Audio_DMAFastCopy(romAddr, ret, size, sp40_2);
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
        case SAMPLE_TABLE:
            func_8018F3E8(id, status);
            break;
        default:
            break;
    }

    return ret;
}

u32 Audio_GetTableIndex(s32 tableType, u32 tableIdx) {
    SampleBankTable* table = Audio_GetLoadTable(tableType);

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

void* Audio_GetLoadTable(s32 tableType) {
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
        case SAMPLE_TABLE:
            ret = gAudioContext.sampleBankTable;
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
                    Audio_SampleReloc(&drum->sound, mem, relocInfo);
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
                    Audio_SampleReloc(sfx, mem, relocInfo);
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
                    Audio_SampleReloc(&inst->lowNotesSound, mem, relocInfo);
                }
                Audio_SampleReloc(&inst->normalNotesSound, mem, relocInfo);
                if (inst->normalRangeHi != 0x7F) {
                    Audio_SampleReloc(&inst->highNotesSound, mem, relocInfo);
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
    OSMesgQueue* msgQueue = &gAudioContext.syncDmaQueue;
    OSIoMesg* ioMesg = &gAudioContext.syncDmaIoMesg;
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
            handle = gAudioContext.driveHandle;
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

void* Audio_AsyncLoadInner(s32 tableType, s32 arg1, s32 arg2, s32 arg3, OSMesgQueue* retQueue) {
    u32 size;
    SampleBankTable* sp50;
    void* sp4C;
    s32 sp48;
    s8 temp_a1;
    u32 devAddr;
    s32 sp3C;
    CtlEntry* ctlEntry;
    u32 sp34;
    u32 temp_v0;

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
        case SAMPLE_TABLE:
            if (gAudioContext.sampleBankLoadStatus[sp34] == 1) {
                return NULL;
            }
            break;
    }

    sp4C = func_80190240(tableType, sp34);
    if (sp4C != NULL) {
        sp3C = 2;
        osSendMesg(retQueue, arg3 << 0x18, 0);
    } else {
        sp50 = Audio_GetLoadTable(tableType);
        size = sp50->entries[sp34].size;
        size = ALIGN16(size);
        sp48 = sp50->entries[arg1].medium;
        temp_a1 = sp50->entries[arg1].unk_09;
        devAddr = sp50->entries[sp34].romAddr;
        sp3C = 2;
        switch (temp_a1) {
            case 0:
                sp4C = func_8018D5D4(tableType, sp34, size);
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

        if (sp48 == 5) {
            if (devAddr == NULL) {
                return NULL;
            }
            if (tableType == 1) {
                ctlEntry = &gAudioContext.ctlEntries[sp34];
                
                ctlEntry->numInstruments = ((Struct_8018FF60*)devAddr)->numInstruments;
                ctlEntry->numDrums = ((Struct_8018FF60*)devAddr)->numDrums;
                ctlEntry->numSfx = ((Struct_8018FF60*)devAddr)->numSfx;
                devAddr += 0x10;
                size += -0x10;
            }
        }

        if (sp48 == 1) {
            func_80191598((s16)sp50->header.unk_02, devAddr, sp4C, size, sp48, arg2, retQueue,
                          (arg3 << 0x18) | (tableType << 0x10) | (sp34 << 8) | sp3C);
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

void Audio_ProcessLoads(s32 resetStatus) {
    Audio_ProcessSyncLoads(resetStatus);
    func_8019218C(resetStatus);
    Audio_ProcessAsyncLoads(resetStatus);
}

// OoT func_800E301C (SetDmaCallback)
void AudioLoad_SetDmaHandler(void* callback) {
    sDmaHandler = callback;
}

// func_800E3028
void func_80190B44(u32 arg0) {
    D_801D5FE4 = arg0;
}

// OoT func_800E3034 (InitCtlTable)
void func_80190B50(s32 arg0) {
    CtlEntry* ctlEnt = &gAudioContext.ctlEntries[arg0];
    AudioBankTableEntry* tableEnt = &gAudioContext.audioBankTable->entries[arg0];

    ctlEnt->unk_02 = (tableEnt->sampleBankBytes >> 8) & 0xFF;
    ctlEnt->unk_03 = (tableEnt->sampleBankBytes) & 0xFF;
    ctlEnt->numInstruments = (tableEnt->unk_0C >> 8) & 0xFF;
    ctlEnt->numDrums = tableEnt->unk_0C & 0xFF;
    ctlEnt->numSfx = tableEnt->unk_0E;
}

void Audio_ContextInit(void* heap, u32 heapSize) {
    char pad[0x24];
    s32 sp24;
    void* temp_v0_3;
    s32 i;
    u64* heapP;
    u8* ctxP;
    s16* u2974p;

    D_80208E68 = NULL;
    D_80208E70 = NULL;
    D_80208E74 = NULL;

    for (i = 0; i < 4; i++) {
        gAudioContext.unk_29A8[i] = NULL;
    }

    gAudioContext.resetTimer = 0;
    gAudioContext.unk_29B8 = 0; // s8?

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

    Audio_InitMesgQueues();
    if (1) {}

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
    osCreateMesgQueue(&gAudioContext.syncDmaQueue, &gAudioContext.syncDmaMesg, 1);
    osCreateMesgQueue(&gAudioContext.currAudioFrameDmaQueue, gAudioContext.currAudioFrameDmaMesgBuf, 0x40);
    osCreateMesgQueue(&gAudioContext.externalLoadQueue, gAudioContext.externalLoadMesgBuf, 0x10);
    osCreateMesgQueue(&gAudioContext.preloadSampleQueue, gAudioContext.preloadSampleMesgBuf, 0x10);
    gAudioContext.curAudioFrameDmaCount = 0;
    gAudioContext.sampleDmaCount = 0;
    gAudioContext.cartHandle = osCartRomInit();

    if (heap == NULL) {
        gAudioContext.audioHeap = gAudioHeap;
        gAudioContext.audioHeapSize = D_801E1104.heapSize;
    } else {
        void** hp = &heap;
        gAudioContext.audioHeap = *hp;
        gAudioContext.audioHeapSize = heapSize;
    }

    for (i = 0; i < (s32)gAudioContext.audioHeapSize / 8; i++) {
        ((u64*)gAudioContext.audioHeap)[i] = 0;
    }

    Audio_InitMainPools(D_801E1104.initPoolSize);

    for (i = 0; i < 3; i++) {
        gAudioContext.aiBuffers[i] = Audio_AllocZeroed(&gAudioContext.audioInitPool, AIBUF_LEN);
    }

    gAudioContext.sequenceTable = &gSequenceTable;
    gAudioContext.audioBankTable = &gAudioBankTable;
    gAudioContext.sampleBankTable = &gAudioTable;
    gAudioContext.unk_283C = &D_801E1420;
    gAudioContext.numSequences = gAudioContext.sequenceTable->header.entryCnt;

    gAudioContext.audioResetSpecIdToLoad = 0;
    gAudioContext.resetStatus = 1;

    Audio_ResetStep();
    Audio_InitAudioTable(gAudioContext.sequenceTable, _AudioseqSegmentRomStart, 0);
    Audio_InitAudioTable(gAudioContext.audioBankTable, _AudiobankSegmentRomStart, 0);
    Audio_InitAudioTable(gAudioContext.sampleBankTable, _AudiotableSegmentRomStart, 0);
    sp24 = gAudioContext.audioBankTable->header.entryCnt;
    gAudioContext.ctlEntries = Audio_Alloc(&gAudioContext.audioInitPool, sp24 * sizeof(CtlEntry));

    for (i = 0; i < sp24; i++) {
        func_80190B50(i);
    }

    if (temp_v0_3 = Audio_Alloc(&gAudioContext.audioInitPool, D_801E1104.permanentPoolSize), temp_v0_3 == NULL) {

        *((u32*)&D_801E1104.permanentPoolSize) = 0;
    }

    Audio_SoundAllocPoolInit(&gAudioContext.permanentPool, temp_v0_3, D_801E1104.permanentPoolSize);
    gAudioContextInitalized = 1;
    osSendMesg(gAudioContext.taskStartQueueP, (void*)gAudioContext.totalTaskCnt, 0);
}

void Audio_SyncLoadsInit(void) {
    gAudioContext.slowLoads[0].status = 0;
    gAudioContext.slowLoads[1].status = 0;
}

s32 Audio_SyncLoadSample(s32 arg0, s32 arg1, s8* isDone) {
    AudioBankSample* sample;
    AudioSlowLoad* syncLoad;

    sample = Audio_GetBankSample(arg0, arg1);
    if (sample == NULL) {
        *isDone = 0;
        return -1;
    }

    if (sample->medium == 0) {
        *isDone = 2;
        return 0;
    }

    syncLoad = &gAudioContext.slowLoads[gAudioContext.slowLoadPos];
    if (syncLoad->status == LOAD_STATUS_DONE) {
        syncLoad->status = LOAD_STATUS_WAITING;
    }

    syncLoad->sample = *sample;
    syncLoad->isDone = isDone; 
    syncLoad->ramAddr = func_8018D658(sample->size, arg0, sample->sampleAddr, (s32)sample->medium, 0);

    if (syncLoad->ramAddr == NULL) {
        if (sample->medium == 1 || sample->codec == 2) { 
            *isDone = 0;
            return -1;
        } else {
            *isDone = 3;
            return -1;
        }
    }

    syncLoad->status = LOAD_STATUS_START;
    syncLoad->bytesRemaining = ALIGN16(sample->size);
    syncLoad->ramSampleAddr = syncLoad->ramAddr;
    syncLoad->devAddr = sample->sampleAddr;
    syncLoad->medium = sample->medium;
    syncLoad->seqOrBankId = arg0;
    syncLoad->instId = arg1;
    if (syncLoad->medium == 1) {
        syncLoad->unk_04 = gAudioContext.sampleBankTable->header.unk_02;
    }

    gAudioContext.slowLoadPos ^= 1;
    return 0;
}

AudioBankSample* Audio_GetBankSample(s32 bankId, s32 sfxId) {
    AudioBankSample* ret;

    if (sfxId < 0x80) {
        Instrument* instrument = Audio_GetInstrumentInner(bankId, sfxId);
        if (instrument == NULL) {
            return NULL;
        }
        ret = instrument->normalNotesSound.sample;
    } else if (sfxId < 0x100) {
        Drum* drum = Audio_GetDrum(bankId, sfxId - 0x80);
        if (drum == NULL) {
            return NULL;
        }
        ret = drum->sound.sample;
    } else {
        AudioBankSound* bankSound = Audio_GetSfx(bankId, sfxId - 0x100);
        if (bankSound == NULL) {
            return NULL;
        }
        ret = bankSound->sample;
    }
    return ret;
}

// OoT func_800E3670
void func_801911C4(void) {
}

// OoT func_800E3678
void func_801911CC(AudioSlowLoad* syncLoad) {
    AudioBankSample* sample;

    if (syncLoad->sample.sampleAddr == NULL) {
        return;
    }

    sample = Audio_GetBankSample(syncLoad->seqOrBankId, syncLoad->instId);
    if (sample == NULL) {
        return;
    }

    syncLoad->sample = *sample;
    sample->sampleAddr = syncLoad->ramSampleAddr;
    sample->medium = 0;
}

void Audio_ProcessSyncLoads(s32 resetStatus) {
    AudioSlowLoad* syncLoad;
    s32 i;

    for (i = 0; i < ARRAY_COUNT(gAudioContext.slowLoads); i++) {
        syncLoad = &gAudioContext.slowLoads[i];
        switch (gAudioContext.slowLoads[i].status) {
            case LOAD_STATUS_LOADING:
                if (syncLoad->medium != 1) {
                    osRecvMesg(&syncLoad->msgqueue, NULL, OS_MESG_BLOCK);
                }

                if (resetStatus != 0) {
                    syncLoad->status = LOAD_STATUS_DONE;
                    continue;
                }
            case LOAD_STATUS_START:
                syncLoad->status = LOAD_STATUS_LOADING;
                if (syncLoad->bytesRemaining == 0) {
                    func_801911CC(syncLoad);
                    syncLoad->status = LOAD_STATUS_DONE;
                    *syncLoad->isDone = 1;
                } else if (syncLoad->bytesRemaining < 0x400) {
                    if (syncLoad->medium == 1) {
                        u32 size = syncLoad->bytesRemaining;

                        func_8019144C(syncLoad->devAddr, syncLoad->ramAddr, size, syncLoad->unk_04);
                    } else {
                        func_801913C8(syncLoad, syncLoad->bytesRemaining);
                    }
                    syncLoad->bytesRemaining = 0;
                } else {
                    if (syncLoad->medium == 1) {
                        func_8019144C(syncLoad->devAddr, syncLoad->ramAddr, 0x400, syncLoad->unk_04);
                    } else {
                        func_801913C8(syncLoad, 0x400);
                    }
                    syncLoad->bytesRemaining -= 0x400;
                    syncLoad->ramAddr += 0x400;
                    syncLoad->devAddr += 0x400;
                }
                break;
        }
    }
}

// OoT func_800E3874
void func_801913C8(AudioSlowLoad* arg0, s32 size) {
    Audio_osInvalDCache(arg0->ramAddr, size);
    osCreateMesgQueue(&arg0->msgqueue, &arg0->msg, 1);
    Audio_DMA(&arg0->ioMesg, 0U, 0, arg0->devAddr, arg0->ramAddr, size, &arg0->msgqueue, arg0->medium, "SLOWCOPY");
}

// OoT func_800E38F8
void func_8019144C(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
}

s32 Audio_SyncLoadSeq(s32 seqIdx, u8* ramAddr, s8* isDone) {
    AudioSlowLoad* syncLoad;
    SequenceTable* seqTable;
    u32 size;

    if (seqIdx >= gAudioContext.numSequences) {
        *isDone = 0;
        return -1;
    }

    seqIdx = Audio_GetTableIndex(SEQUENCE_TABLE, seqIdx);
    seqTable = Audio_GetLoadTable(SEQUENCE_TABLE);
    syncLoad = &gAudioContext.slowLoads[gAudioContext.slowLoadPos];
    if (syncLoad->status == LOAD_STATUS_DONE) {
        syncLoad->status = LOAD_STATUS_WAITING;
    }

    syncLoad->sample.sampleAddr = NULL;
    syncLoad->isDone = isDone;
    size = seqTable->entries[seqIdx].size;
    size = ALIGN16(size);
    syncLoad->ramAddr = ramAddr;
    syncLoad->status = LOAD_STATUS_START;
    syncLoad->bytesRemaining = size;
    syncLoad->ramSampleAddr = ramAddr;
    syncLoad->devAddr = seqTable->entries[seqIdx].romAddr;
    syncLoad->medium = seqTable->entries[seqIdx].medium;
    syncLoad->seqOrBankId = seqIdx;

    if (syncLoad->medium == 1) {
        syncLoad->unk_04 = seqTable->header.unk_02;
    }

    gAudioContext.slowLoadPos ^= 1;
    return 0;
}

void Audio_AsyncLoadReqInit(void) {
    s32 i;

    for (i = 0; i < ARRAY_COUNT(gAudioContext.asyncLoads); i++) {
        gAudioContext.asyncLoads[i].status = 0;
    }
}

// OoT func_800E3A44
AudioAsyncLoad* func_80191598(s32 arg0, u32 devAddr, void* ramAddr, s32 size, s32 arg4, s32 nChunks,
                            OSMesgQueue* retQueue, s32 retMsg) {
    AudioAsyncLoad* loadReq;

    loadReq = Audio_InitAsyncReq(devAddr, ramAddr, size, arg4, nChunks, retQueue, retMsg);

    if (loadReq == NULL) {
        return NULL;
    }

    osSendMesg(&gAudioContext.asyncLoadUnkMediumQueue, loadReq, 0);
    loadReq->unk_18 = arg0;
    return loadReq;
}

AudioAsyncLoad* Audio_InitAsyncReq(u32 devAddr, void* ramAddr, u32 size, s32 arg3, s32 nChunks, OSMesgQueue* retQueue,
                                 s32 retMsg) {
    AudioAsyncLoad* loadReq;
    s32 i;

    for (i = 0; i < ARRAY_COUNT(gAudioContext.asyncLoads); i++) {
        if (gAudioContext.asyncLoads[i].status == 0) {
            loadReq = &gAudioContext.asyncLoads[i];
            break;
        }
    }

    // no more available async reqs
    if (i == ARRAY_COUNT(gAudioContext.asyncLoads)) {
        return NULL;
    }

    loadReq->status = LOAD_STATUS_START;
    loadReq->devAddr = devAddr;
    loadReq->ramAddr2 = ramAddr;
    loadReq->ramAddr = ramAddr;
    loadReq->bytesRemaining = size;

    if (nChunks == 0) {
        loadReq->chunkSize = 0x1000;
    } else if (nChunks == 1) {
        loadReq->chunkSize = size;
    } else {
        loadReq->chunkSize = (((s32)size / nChunks) + 0xFF) & ~0xFF;
        if (loadReq->chunkSize < 0x100) {
            loadReq->chunkSize = 0x100;
        }
    }

    loadReq->retQueue = retQueue;
    loadReq->delay = 3;
    loadReq->medium = arg3;
    loadReq->retMsg = retMsg;
    osCreateMesgQueue(&loadReq->msgQueue, &loadReq->msg, 1);
    return loadReq;
}

void Audio_ProcessAsyncLoads(s32 resetStatus) {
    AudioAsyncLoad* loadReq;
    s32 i;

    if (gAudioContext.resetTimer == 1) {
        return;
    }

    if (gAudioContext.curUnkMediumLoad == NULL) {
        if (resetStatus != 0) {
            do {
            } while (osRecvMesg(&gAudioContext.asyncLoadUnkMediumQueue, (OSMesg*)&loadReq, OS_MESG_NOBLOCK) != -1);
        } else if (osRecvMesg(&gAudioContext.asyncLoadUnkMediumQueue, (OSMesg*)&loadReq, OS_MESG_NOBLOCK) == -1) {
            gAudioContext.curUnkMediumLoad = NULL;
        } else {
            gAudioContext.curUnkMediumLoad = loadReq;
        }
    }

    if (gAudioContext.curUnkMediumLoad != NULL) {
        Audio_HandleAsyncMsg(gAudioContext.curUnkMediumLoad, resetStatus);
    }

    for (i = 0; i < ARRAY_COUNT(gAudioContext.asyncLoads); i++) {
        if (gAudioContext.asyncLoads[i].status == 1) {
            loadReq = &gAudioContext.asyncLoads[i];
            if (loadReq->medium != 1) {
                Audio_UpdateAsyncReq(loadReq, resetStatus);
            }
        }
    }
}

void Audio_HandleAsyncMsg(AudioAsyncLoad* arg0, s32 arg1) {
}

void Audio_AsyncReqDone(AudioAsyncLoad* loadReq) {
    u32 retMsg = loadReq->retMsg;
    u32 b2;
    u32 pad;
    u32 t1;
    OSMesg doneMsg;
    u32 t2;
    RelocInfo sp30;

    if (1) {}
    switch (ASYNC_TBLTYPE(retMsg)) {
        case SEQUENCE_TABLE:
            Audio_SetSeqLoadStatus(ASYNC_B2(retMsg), ASYNC_B3(retMsg));
            break;
        case SAMPLE_TABLE:
            func_8018F3E8(ASYNC_B2(retMsg), ASYNC_B3(retMsg));
            break;
        case BANK_TABLE:
            b2 = ASYNC_B2(retMsg);
            t1 = gAudioContext.ctlEntries[b2].unk_02;
            t2 = gAudioContext.ctlEntries[b2].unk_03;
            sp30.index1 = t1;
            sp30.index2 = t2;
            sp30.baseAddr1 = t1 != 0xFF ? func_8018FD20(t1, &sp30.medium1) : 0;
            sp30.baseAddr2 = t2 != 0xFF ? func_8018FD20(t2, &sp30.medium2) : 0;
            Audio_SetBankLoadStatus(b2, ASYNC_B3(retMsg));
            func_80191D94(b2, loadReq->ramAddr2, &sp30, 1);
            break;
    }

    doneMsg = loadReq->retMsg;
    if (1) {}
    loadReq->status = LOAD_STATUS_WAITING;
    osSendMesg(loadReq->retQueue, doneMsg, OS_MESG_NOBLOCK);
}

void Audio_UpdateAsyncReq(AudioAsyncLoad* req, s32 resetStatus) {
    SampleBankTable* audioTable = gAudioContext.sampleBankTable;

    if (req->delay >= 2) {
        req->delay--;
        return;
    }

    if (req->delay == 1) {
        req->delay = 0;
    } else if (resetStatus != 0) {
        osRecvMesg(&req->msgQueue, NULL, OS_MESG_BLOCK);
        req->status = LOAD_STATUS_WAITING;
        return;
    } else if (osRecvMesg(&req->msgQueue, NULL, OS_MESG_NOBLOCK) == -1) {
        return;
    }

    if (req->bytesRemaining == 0) {
        Audio_AsyncReqDone(req);
        return;
    }

    if (req->bytesRemaining < req->chunkSize) {
        if (req->medium == 1) {
            func_80191C40(req->devAddr, req->ramAddr, req->bytesRemaining, audioTable->header.unk_02);
        } else if (req->medium == 5) {
            func_80191BD0(req, req->bytesRemaining);
        } else {
            func_80191B40(req, req->bytesRemaining);
        }
        req->bytesRemaining = 0;
        return;
    }

    if (req->medium == 1) {
        func_80191C40(req->devAddr, req->ramAddr, req->chunkSize, audioTable->header.unk_02);
    } else if (req->medium == 5) {
        func_80191BD0(req, req->chunkSize);
    } else {
        func_80191B40(req, req->chunkSize);
    }

    req->bytesRemaining -= req->chunkSize;
    req->devAddr += req->chunkSize;
    req->ramAddr = req->ramAddr + req->chunkSize;
}

// OoT func_800E3FB4
void func_80191B40(AudioAsyncLoad* req, u32 size) {
    if (size) {}
    size = ALIGN16(size);
    Audio_osInvalDCache(req->ramAddr, size);
    osCreateMesgQueue(&req->msgQueue, &req->msg, 1);
    Audio_DMA(&req->ioMesg, 0, 0, req->devAddr, req->ramAddr, size, &req->msgQueue, req->medium, "BGCOPY");
}

// New to MM
void func_80191BD0(AudioAsyncLoad* req, u32 size) {
    if (size) {}
    size = ALIGN16(size);
    Audio_osInvalDCache(req->ramAddr, size);
    osCreateMesgQueue(&req->msgQueue, &req->msg, 1);
    bcopy(req->devAddr, req->ramAddr, size);
    osSendMesg(&req->msgQueue, NULL, 0);
}

// OoT func_800E4044
void func_80191C40(u32 devAddr, void* ramAddr, u32 size, s16 arg3) {
}

#define RELOC(v, base) (reloc = (void*)((u32)(v) + (u32)(base)))

void Audio_SampleReloc(AudioBankSound* sound, u32 arg1, RelocInfo* arg2) {
    AudioBankSample* sample;
    void* reloc;

    if ((u32)sound->sample <= 0x80000000) {
        sample = sound->sample = RELOC(sound->sample, arg1);
        if (sample->size != 0 && sample->unk_bits25 != 1) {
            sample->loop = RELOC(sample->loop, arg1);
            sample->book = RELOC(sample->book, arg1);
            switch (sample->medium) {
                case 0:
                    sample->sampleAddr = RELOC(sample->sampleAddr, arg2->baseAddr1);
                    sample->medium = arg2->medium1;
                    break;
                case 1:
                    sample->sampleAddr = RELOC(sample->sampleAddr, arg2->baseAddr2);
                    sample->medium = arg2->medium2;
                    break;
                case 2:
                case 3:
                    break;
            }

            sample->unk_bits25 = 1;
            if (sample->unk_bits26 && (sample->medium != 0)) {
                gAudioContext.usedSamples[gAudioContext.usedSamplesCount++] = sample;
            }
        }
    }
}

#undef RELOC

// OoT func_800E4198
void func_80191D94(s32 bankId, AudioBankData* mem, RelocInfo* relocInfo, s32 arg3) {
    AudioPreloadReq* item;
    AudioPreloadReq* item2;
    AudioBankSample* sample;
    s32 count;
    s32 nChunks;
    u8* addr;
    s32 sp4C;
    s32 i;

    sp4C = false;
    if (gAudioContext.preloadSampleStackTop != 0) {
        sp4C = true;
    } else {
        D_801FD1E0 = 0;
    }
    gAudioContext.usedSamplesCount = 0;
    func_801902D8(bankId, mem, relocInfo);

    count = 0;
    for (i = 0; i < gAudioContext.usedSamplesCount; i++) {
        count += ALIGN16(gAudioContext.usedSamples[i]->size);
    }
    if (count && count) {}

    for (i = 0; i < gAudioContext.usedSamplesCount; i++) {
        if (gAudioContext.preloadSampleStackTop == 120) {
            break;
        }

        sample = gAudioContext.usedSamples[i];
        addr = NULL;
        switch (arg3) {
            case 0:
                if (sample->medium == relocInfo->medium1) {
                    addr = func_8018D658(sample->size, relocInfo->index1, sample->sampleAddr, sample->medium, 1);
                } else if (sample->medium == relocInfo->medium2) {
                    addr = func_8018D658(sample->size, relocInfo->index2, sample->sampleAddr, sample->medium, 1);
                } else if (sample->medium == 3) {
                    addr = func_8018D658(sample->size, 0xFE, sample->sampleAddr, sample->medium, 1);
                }
                break;

            case 1:
                if (sample->medium == relocInfo->medium1) {
                    addr = func_8018D658(sample->size, relocInfo->index1, sample->sampleAddr, sample->medium, 0);
                } else if (sample->medium == relocInfo->medium2) {
                    addr = func_8018D658(sample->size, relocInfo->index2, sample->sampleAddr, sample->medium, 0);
                } else if (sample->medium == 3) {
                    addr = func_8018D658(sample->size, 0xFE, sample->sampleAddr, sample->medium, 0);
                }
                break;
        }
        if (addr == NULL) {
            continue;
        }

        switch (arg3) {
            case 0:
                if (sample->medium == 1) {
                    Audio_NoopCopy((u32)sample->sampleAddr, addr, sample->size,
                                   gAudioContext.sampleBankTable->header.unk_02);
                    sample->sampleAddr = addr;
                    sample->medium = 0;
                } else {
                    Audio_DMAFastCopy((u32)sample->sampleAddr, addr, sample->size, sample->medium);
                    sample->sampleAddr = addr;
                    sample->medium = 0;
                }
                if (sample->medium == 3) {}
                break;

            case 1:
                item = &gAudioContext.preloadSampleStack[gAudioContext.preloadSampleStackTop];
                item->sample = sample;
                item->ramAddr = addr;
                item->encodedInfo = (gAudioContext.preloadSampleStackTop << 24) | 0xFFFFFF;
                item->isFree = false;
                item->endAndMediumKey = (u32)sample->sampleAddr + sample->size + sample->medium;
                gAudioContext.preloadSampleStackTop++;
                break;
        }
    }

    gAudioContext.usedSamplesCount = 0;
    if (gAudioContext.preloadSampleStackTop != 0 && !sp4C) {
        item2 = &gAudioContext.preloadSampleStack[gAudioContext.preloadSampleStackTop - 1];
        sample = item2->sample;
        nChunks = (sample->size >> 12) + 1;
        Audio_InitAsyncReq((u32)sample->sampleAddr, item2->ramAddr, sample->size, sample->medium, nChunks,
                           &gAudioContext.preloadSampleQueue, item2->encodedInfo);
    }
}

// OoT func_800E4590
s32 func_8019218C(s32 resetStatus) {
    AudioBankSample* sample;
    AudioPreloadReq* item;
    u32 sp4C;
    u32 key;
    u32 unk;
    s32 pad;

    if (gAudioContext.preloadSampleStackTop > 0) {
        if (resetStatus != 0) {
            osRecvMesg(&gAudioContext.preloadSampleQueue, (OSMesg*)&sp4C, OS_MESG_NOBLOCK);
            gAudioContext.preloadSampleStackTop = 0;
            return 0;
        }
        if (osRecvMesg(&gAudioContext.preloadSampleQueue, (OSMesg*)&sp4C, OS_MESG_NOBLOCK) == -1) {
            return 0;
        }

        sp4C >>= 24;
        item = &gAudioContext.preloadSampleStack[sp4C];

        if (item->isFree == false) {
            sample = item->sample;
            key = (u32)sample->sampleAddr + sample->size + sample->medium;
            if (key == item->endAndMediumKey) {
                sample->sampleAddr = item->ramAddr;
                sample->medium = 0;
            }
            item->isFree = true;
        }

        for (;;) {
            if (gAudioContext.preloadSampleStackTop <= 0) {
                break;
            }
            item = &gAudioContext.preloadSampleStack[gAudioContext.preloadSampleStackTop - 1];
            if (item->isFree == true) {
                gAudioContext.preloadSampleStackTop--;
                continue;
            }

            sample = item->sample;
            unk = (sample->size >> 12) + 1;
            key = (u32)sample->sampleAddr + sample->size + sample->medium;
            if (key != item->endAndMediumKey) {
                item->isFree = true;
                gAudioContext.preloadSampleStackTop--;
            } else {
                Audio_InitAsyncReq((u32)sample->sampleAddr, item->ramAddr, sample->size, sample->medium, unk,
                                   &gAudioContext.preloadSampleQueue, item->encodedInfo);
                break;
            }
        }
    }
    return 1;
}

// OoT func_800E4590
s32 func_80192340(AudioBankSample* sample, s32 sampleCnt, AudioBankSample** sampleList) {
    s32 i;

    for (i = 0; i < sampleCnt; i++) {
        if (sample->sampleAddr == sampleList[i]->sampleAddr) {
            break;
        }
    }

    if (i == sampleCnt) {
        sampleList[sampleCnt] = sample;
        sampleCnt++;
    }

    return sampleCnt;
}

// OoT func_800E478C
s32 func_80192388(s32 bankId, AudioBankSample** sampleList) {
    s32 i;
    s32 drumCnt;
    s32 instrumentCnt;
    s32 sampleCnt = 0;

    drumCnt = gAudioContext.ctlEntries[bankId].numDrums;
    instrumentCnt = gAudioContext.ctlEntries[bankId].numInstruments;

    for (i = 0; i < drumCnt; i++) {
        Drum* drum = Audio_GetDrum(bankId, i);
        if (1) {}
        if (drum != NULL) {
            sampleCnt = func_80192340(drum->sound.sample, sampleCnt, sampleList);
        }
    }

    for (i = 0; i < instrumentCnt; i++) {
        Instrument* instrument = Audio_GetInstrumentInner(bankId, i);
        if (instrument != NULL) {
            if (instrument->normalRangeLo != 0) {
                sampleCnt = func_80192340(instrument->lowNotesSound.sample, sampleCnt, sampleList);
            }
            if (instrument->normalRangeHi != 0x7F) {
                sampleCnt = func_80192340(instrument->highNotesSound.sample, sampleCnt, sampleList);
            }
            sampleCnt = func_80192340(instrument->normalNotesSound.sample, sampleCnt, sampleList);
        }
    }
    return sampleCnt;
}

// OoT func_800E48C0
void func_801924BC(AudioBankSound* sound) {
    AudioBankSample* sample = sound->sample;

    if ((sample->size != 0) && (sample->unk_bits26) && (sample->medium)) {
        gAudioContext.usedSamples[gAudioContext.usedSamplesCount++] = sample;
    }
}

// OoT func_800E4918
void func_80192514(s32 bankId, s32 arg1, RelocInfo* relocInfo) {
    s32 numDrums;
    s32 numInstruments;
    s32 numSfx;
    Drum* drum;
    Instrument* instrument;
    AudioBankSound* sound;
    AudioPreloadReq* item;
    AudioPreloadReq* item2;
    u8* addr;
    s32 count;
    s32 i;
    AudioBankSample* sample;
    s32 sp44;
    s32 nChunks;

    sp44 = false;
    if (gAudioContext.preloadSampleStackTop != 0) {
        sp44 = true;
    }
    gAudioContext.usedSamplesCount = 0;

    numDrums = gAudioContext.ctlEntries[bankId].numDrums;
    numInstruments = gAudioContext.ctlEntries[bankId].numInstruments;
    numSfx = gAudioContext.ctlEntries[bankId].numSfx;

    for (i = 0; i < numInstruments; i++) {
        instrument = Audio_GetInstrumentInner(bankId, i);
        if (instrument != NULL) {
            if (instrument->normalRangeLo != 0) {
                func_801924BC(&instrument->lowNotesSound);
            }
            if (instrument->normalRangeHi != 0x7F) {
                func_801924BC(&instrument->highNotesSound);
            }
            func_801924BC(&instrument->normalNotesSound);
        }
    }

    for (i = 0; i < numDrums; i++) {
        drum = Audio_GetDrum(bankId, i);
        if (drum != NULL) {
            func_801924BC(&drum->sound);
        }
    }

    for (i = 0; i < numSfx; i++) {
        sound = Audio_GetSfx(bankId, i);
        if (sound != NULL) {
            func_801924BC(sound);
        }
    }

    if (gAudioContext.usedSamplesCount == 0) {
        return;
    }

    count = 0;
    for (i = 0; i < gAudioContext.usedSamplesCount; i++) {
        count += ALIGN16(gAudioContext.usedSamples[i]->size);
    }
    if (count) {}

    for (i = 0; i < gAudioContext.usedSamplesCount; i++) {
        if (gAudioContext.preloadSampleStackTop == 120) {
            break;
        }

        sample = gAudioContext.usedSamples[i];
        if (sample->medium == 0) {
            continue;
        }

        switch (arg1) {
            case 0:
                if (sample->medium == relocInfo->medium1) {
                    addr = func_8018D658(sample->size, relocInfo->index1, sample->sampleAddr, sample->medium, 1);
                } else if (sample->medium == relocInfo->medium2) {
                    addr = func_8018D658(sample->size, relocInfo->index2, sample->sampleAddr, sample->medium, 1);
                }
                break;

            case 1:
                if (sample->medium == relocInfo->medium1) {
                    addr = func_8018D658(sample->size, relocInfo->index1, sample->sampleAddr, sample->medium, 0);
                } else if (sample->medium == relocInfo->medium2) {
                    addr = func_8018D658(sample->size, relocInfo->index2, sample->sampleAddr, sample->medium, 0);
                }
                break;
        }
        if (addr == NULL) {
            continue;
        }

        switch (arg1) {
            case 0:
                if (sample->medium == 1) {
                    Audio_NoopCopy((u32)sample->sampleAddr, addr, sample->size,
                                   gAudioContext.sampleBankTable->header.unk_02);
                    sample->sampleAddr = addr;
                    sample->medium = 0;
                } else {
                    Audio_DMAFastCopy((u32)sample->sampleAddr, addr, sample->size, sample->medium);
                    sample->sampleAddr = addr;
                    sample->medium = 0;
                }
                break;

            case 1:
                item = &gAudioContext.preloadSampleStack[gAudioContext.preloadSampleStackTop];
                item->sample = sample;
                item->ramAddr = addr;
                item->encodedInfo = (gAudioContext.preloadSampleStackTop << 24) | 0xFFFFFF;
                item->isFree = false;
                item->endAndMediumKey = (u32)sample->sampleAddr + sample->size + sample->medium;
                gAudioContext.preloadSampleStackTop++;
                break;
        }
    }

    gAudioContext.usedSamplesCount = 0;
    if (gAudioContext.preloadSampleStackTop != 0 && !sp44) {
        item2 = &gAudioContext.preloadSampleStack[gAudioContext.preloadSampleStackTop - 1];
        sample = item2->sample;
        nChunks = (sample->size >> 12) + 1;
        Audio_InitAsyncReq((u32)sample->sampleAddr, item2->ramAddr, sample->size, sample->medium, nChunks,
                           &gAudioContext.preloadSampleQueue, item2->encodedInfo);
    }
}

// OoT func_800E4D94
void func_80192990(void) {
    s32 pad;
    u32 temp_s2;
    SampleBankTable* audioTable;
    s32 pad2;
    s32 i;

    audioTable = Audio_GetLoadTable(SAMPLE_TABLE);
    for (i = 0; i < gAudioContext.permanentPool.count; i++) {
        RelocInfo sp4C;
        if (gAudioContext.permanentCache[i].tableType == 1) {
            temp_s2 = Audio_GetTableIndex(BANK_TABLE, gAudioContext.permanentCache[i].id);
            sp4C.index1 = gAudioContext.ctlEntries[temp_s2].unk_02;
            sp4C.index2 = gAudioContext.ctlEntries[temp_s2].unk_03;

            if (sp4C.index1 != 0xFF) {
                sp4C.index1 = Audio_GetTableIndex(SAMPLE_TABLE, sp4C.index1);
                sp4C.medium1 = audioTable->entries[sp4C.index1].medium;
            }

            if (sp4C.index2 != 0xFF) {
                sp4C.index2 = Audio_GetTableIndex(SAMPLE_TABLE, sp4C.index2);
                sp4C.medium2 = audioTable->entries[sp4C.index2].medium;
            }
            func_80192514(temp_s2, 0, &sp4C);
        }
    }
}

void func_80192AD0(void) {
}

void func_80192AD8(void) {
}

void func_80192AE0(void) {
}

// Matching, need to migrate data from code_801D55B0.data.s
// OoT func_800E4EEC
#ifdef NON_MATCHING
void func_80192AE8(s32 tableType, s32 arg1, s8* arg2) {
    static u32 D_801D5FEC = 0;

    D_801FD198[D_801D5FEC] = arg2;
    Audio_AsyncLoad(tableType, arg1, 0, D_801D5FEC, &D_801FD140);
    D_801D5FEC++;
    if (D_801D5FEC == 0x10) {
        D_801D5FEC = 0;
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80192AE8.s")
#endif

// OoT func_800E4F58
void func_80192B54(void) {
    u32 pad;
    u32 sp20;
    s8* temp_v0;

    if (osRecvMesg(&D_801FD140, (OSMesg*)&sp20, OS_MESG_NOBLOCK) != -1) {
        pad = sp20 >> 24;
        temp_v0 = D_801FD198[pad];
        if (temp_v0 != NULL) {
            *temp_v0 = 0;
        }
    }
}

// OoT func_800E4FB0
void func_80192BAC(void) {
    osCreateMesgQueue(&D_801FD140, D_801FD158, 0x10);
}
