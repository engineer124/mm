#include "global.h"

typedef enum { LOAD_STATUS_WAITING, LOAD_STATUS_START, LOAD_STATUS_LOADING, LOAD_STATUS_DONE } SyncLoadStatus;

// opaque type for unpatched audio bank data (should maybe get rid of this?)
typedef void AudioBankData;

#define OS_MESG_PRI_NORMAL  0
#define OS_MESG_PRI_HIGH    1

// OoT func_800E11F0
#ifdef NON_EQUIVALENT
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
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018EB60.s")
#endif

// Audio_DmaSampleData
#ifdef NON_EQUIVALENT
void* func_8018EC4C(u32 devAddr, u32 size, s32 arg2, u8* dmaIndexRef, s32 medium) {
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
    func_8019067C(&gAudioContext.currAudioFrameDmaIoMesgBufs[gAudioContext.curAudioFrameDmaCount++], OS_MESG_PRI_NORMAL,
              OS_READ, dmaDevAddr, dma->ramAddr, transfer, &gAudioContext.currAudioFrameDmaQueue, medium, "SUPERDMA");
    *dmaIndexRef = dmaIndex;
    return (devAddr - dmaDevAddr) + dma->ramAddr;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018EC4C.s")
#endif

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/D_801E030C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018EF88.s")

#ifdef NON_MATCHING
s32 Audio_IsBankLoadComplete(s32 bankId) {
    if (bankId == 0xFF) {
        return true;
    } else if (gAudioContext.bankLoadStatus[bankId] >= 2) {
        return true;
    } else if (gAudioContext.bankLoadStatus[func_80190204(BANK_TABLE, bankId)] >= 2) {
        return true;
    } else {
        return false;
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/Audio_IsBankLoadComplete.s")
#endif

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018F298.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018F310.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018F388.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018F3B8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018F3E8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018F448.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018F478.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018F4D8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018F588.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018F604.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018F6F0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018F7C0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018F7F8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018F83C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018F880.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018F8C4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018F908.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018F9B8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018FA60.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018FAD0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018FB20.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018FB78.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018FCCC.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018FD20.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018FD40.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018FE5C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8018FF60.s")

// Audio_GetTableIndex
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80190204.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80190240.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80190294.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_801902D8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80190544.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80190668.s")

// Audio_DMA
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8019067C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80190754.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8019075C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8019077C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80190B08.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80190B38.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80190B44.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80190B50.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80190BB0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80190F50.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80190F64.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80191134.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_801911C4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_801911CC.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80191240.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_801913C8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8019144C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80191460.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80191568.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80191598.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_8019161C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80191740.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80191864.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80191870.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_801919AC.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80191B40.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80191BD0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_load/func_80191C40.s")

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
