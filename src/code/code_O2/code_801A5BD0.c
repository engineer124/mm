#include "global.h"

void Audio_SetSoundBanksMute(u16 muteMask) {
    u8 bankId;

    for (bankId = 0; bankId < 7; bankId++) {
        if (muteMask & 1) {
            gSoundBankMuted[bankId] = true;
        } else {
            gSoundBankMuted[bankId] = false;
        }
        muteMask = muteMask >> 1;
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A5BD0/Audio_QueueSeqCmdMute.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A5BD0/Audio_ClearBGMMute.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A5BD0/Audio_PlaySoundGeneral.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A5BD0/Audio_RemoveMatchingSoundRequests.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A5BD0/Audio_ProcessSoundRequest.s")

// OoT func_800F7B54
#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A5BD0/func_801A6430.s")

// OoT func_800F7CEC
#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A5BD0/func_801A65C8.s")

// OoT func_800F8480
#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A5BD0/func_801A6D0C.s")

// OoT func_800F87A0
#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A5BD0/func_801A7084.s")

// OoT func_800F8884
#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A5BD0/func_801A7168.s")

// OoT func_800F89A0
#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A5BD0/func_801A7284.s")

// OoT func_800F89E8
#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A5BD0/func_801A72CC.s")

// OoT func_800F8A44
#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A5BD0/func_801A7328.s")

// OoT func_800F8BA0
#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A5BD0/func_801A7484.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A5BD0/Audio_StopSfx.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A5BD0/Audio_ProcessSoundRequests.s")

// OoT func_800F8EA0
#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A5BD0/func_801A7794.s")

// OoT func_800F8F34
#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A5BD0/func_801A7828.s")

// OoT func_800F8F88
#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A5BD0/func_801A787C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A5BD0/Audio_IsSfxPlaying.s")

// func_800F905C
#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A5BD0/func_801A794C.s")
