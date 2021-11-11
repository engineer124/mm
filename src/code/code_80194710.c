#include "global.h"

void Audio_osInvalDCache(void* buf, s32 size) {
    OSIntMask prevMask = osSetIntMask(1);
    osInvalDCache(buf, size);
    osSetIntMask(prevMask);
}

void Audio_osWritebackDCache(void* buf, s32 size) {
    OSIntMask prevMask = osSetIntMask(1);
    osWritebackDCache(buf, size);
    osSetIntMask(prevMask);
}

// Matches except for data (in wrong address)
#ifdef NON_MATCHING
s32 osAiSetNextBuffer(void* buf, u32 size) {
    static u8 D_801D6010 = false;
    u32 bufAdjusted = (u32)buf;
    s32 status;

    if (D_801D6010) {
        bufAdjusted = (u32)buf - 0x2000;
    }
    if ((((u32)buf + size) & 0x1FFF) == 0) {
        D_801D6010 = true;
    } else {
        D_801D6010 = false;
    }

    status = HW_REG(AI_STATUS_REG, s32);
    if (status & AI_STATUS_AI_FULL) {
        return -1;
    }

    HW_REG(AI_DRAM_ADDR_REG, u32) = PHYSICAL_TO_VIRTUAL(bufAdjusted);
    HW_REG(AI_LEN_REG, u32) = size;
    return 0;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/code_80194710/osAiSetNextBuffer.s")
#endif

// Unused
#pragma GLOBAL_ASM("asm/non_matchings/code/code_80194710/func_80194804.s")
// extern ? D_801D6028;
// extern ? D_801D6098;

// void func_80194804(s32 arg0) {
//     D_801D6188 = (arg0 * 0x22) + &D_801D6098;
//     D_801D618C = (arg0 * 0x10) + &D_801D6028;
// }

#pragma GLOBAL_ASM("asm/non_matchings/code/code_80194710/func_80194840.s")
// extern ? D_801D6014;

// s16 func_80194840(s32 arg0) {
//     s32 temp_a1;
//     s32 temp_v1;
//     s16* phi_a2;
//     s32 phi_a1;
//     s32 phi_a1_2;
//     s32 phi_v1;
//     s32 phi_v1_2;

//     phi_a2 = D_801D6188 + 2;
//     phi_a1 = 2;
//     phi_v1_2 = 0;
// loop_1:
//     phi_a2 += 2;
//     phi_a1_2 = phi_a1;
//     phi_v1 = phi_v1_2;
//     if (arg0 >= *phi_a2) {
//         temp_a1 = phi_a1 + 2;
//         temp_v1 = phi_v1_2 + 1;
//         phi_a1 = temp_a1;
//         phi_a1_2 = temp_a1;
//         phi_v1 = temp_v1;
//         phi_v1_2 = temp_v1;
//         if (temp_a1 != 0x20) {
//             goto loop_1;
//         }
//     }
//     if (phi_a1_2 == 0x20) {
//         return 0;
//     }
//     return *(&D_801D6014 + (*(D_801D618C + phi_v1) * 2));
// }

// Unused
#pragma GLOBAL_ASM("asm/non_matchings/code/code_80194710/func_801948B0.s")
// s32 func_80194840(s32); // extern

// ? func_801948B0(s32* arg0, s32* arg1) {
//     s32 temp_v0;
//     s32 phi_s0;
//     s32 phi_s1;

//     phi_s0 = *arg0;
//     phi_s1 = *arg1;
// loop_1:
//     temp_v0 = func_80194840(phi_s0);
//     if (temp_v0 == 0) {
//         return -1;
//     }
//     if (phi_s1 >= temp_v0) {
//         phi_s0 += 1;
//         phi_s1 -= temp_v0;
//         goto loop_1;
//     }
//     *arg0 = phi_s0;
//     *arg1 = phi_s1;
//     return 0;
// }
