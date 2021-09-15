#include "global.h"

// TODO: clean up these macros. They are similar to ones in code_800EC960.c but without casts.
#define Audio_StartSeq(seqIdx, fadeTimer, seqId) \
    Audio_QueueSeqCmd(0x00000000 | ((seqIdx) << 24) | ((fadeTimer) << 16) | (seqId))
#define Audio_SeqCmdA(seqIdx, a) Audio_QueueSeqCmd(0xA0000000 | ((seqIdx) << 24) | (a))
#define Audio_SeqCmdB30(seqIdx, a, b) Audio_QueueSeqCmd(0xB0003000 | ((seqIdx) << 24) | ((a) << 16) | (b))
#define Audio_SeqCmdB40(seqIdx, a, b) Audio_QueueSeqCmd(0xB0004000 | ((seqIdx) << 24) | ((a) << 16) | (b))
#define Audio_SeqCmd3(seqIdx, a) Audio_QueueSeqCmd(0x30000000 | ((seqIdx) << 24) | (a))
#define Audio_SeqCmd5(seqIdx, a, b) Audio_QueueSeqCmd(0x50000000 | ((seqIdx) << 24) | ((a) << 16) | (b))
#define Audio_SeqCmd4(seqIdx, a, b) Audio_QueueSeqCmd(0x40000000 | ((seqIdx) << 24) | ((a) << 16) | (b))
#define Audio_SetVolScaleNow(seqIdx, volFadeTimer, volScale) \
    Audio_ProcessSeqCmd(0x40000000 | ((u8)seqIdx << 24) | ((u8)volFadeTimer << 16) | ((u8)(volScale * 127.0f)));

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A7B10/func_801A7B10.s")


#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A7B10/func_801A7D04.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A7B10/Audio_ProcessSeqCmd.s")

void Audio_QueueSeqCmd(u32 cmd) {
    sAudioSeqCmds[sSeqCmdWrPos++] = cmd;
}

void Audio_ProcessSeqCmds(void) {
    while (sSeqCmdWrPos != sSeqCmdRdPos) {
        Audio_ProcessSeqCmd(sAudioSeqCmds[sSeqCmdRdPos++]);
    }
}

#ifdef NON_EQUIVALENT
u16 func_801A8A50(u8 playerIdx) {
    if (!gAudioContext.seqPlayers[playerIdx].enabled) {
        return 0xFFFF;
    }
    return D_80200140[playerIdx].unk_254;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A7B10/func_801A8A50.s")
#endif

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A7B10/func_801A8ABC.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A7B10/func_801A8B14.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A7B10/func_801A8B2C.s")

#ifdef NON_EQUIVALENT
void Audio_SetVolScale(u8 seqIdx, u8 scaleIdx, u8 targetVol, u8 volFadeTimer) {
    f32 volScale;
    u8 i;

    D_80200140[seqIdx].volScales[scaleIdx] = targetVol & 0x7F;

    if (volFadeTimer != 0) {
        D_80200140[seqIdx].fadeVolUpdate = 1;
        D_80200140[seqIdx].volFadeTimer = volFadeTimer;
    } else {
        for (i = 0, volScale = 1.0f; i < 4; i++) {
            volScale *= D_80200140[seqIdx].volScales[i] / 127.0f;
        }

        Audio_SetVolScaleNow(seqIdx, volFadeTimer, volScale);
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A7B10/Audio_SetVolScale.s")
#endif

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A7B10/func_801A8D5C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A7B10/func_801A9768.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A7B10/func_801A982C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A7B10/func_801A99B8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A7B10/func_801A9A74.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A7B10/func_801A9B10.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A7B10/func_801A9B78.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A7B10/func_801A9BFC.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A7B10/func_801A9C68.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A7B10/func_801A9D10.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A7B10/func_801A9DCC.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A7B10/func_801A9EA4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A7B10/func_801A9F4C.s")
