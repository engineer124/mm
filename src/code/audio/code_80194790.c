#include "global.h"

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
#pragma GLOBAL_ASM("asm/non_matchings/code/code_80194790/osAiSetNextBuffer.s")
#endif

extern s16 D_801D6014[];
extern s8 D_801D6028[7][0x10];
extern s16 D_801D6098[7][0x11];
extern s16* D_801D6188;
extern s8* D_801D618C;

// Unused
void func_80194804(s32 arg0) {
    D_801D6188 = D_801D6098[arg0];
    D_801D618C = D_801D6028[arg0];
}

s16 func_80194840(s32 arg0) {
    s32 i;
    s32 j = 0;

    for (i = 1; i < 0x10; i++) {
        if (arg0 < D_801D6188[i]) {
            break;
        }
        j++;
    }

    if (i == 0x10) {
        return 0;
    } else {
        j = D_801D618C[j];
        return D_801D6014[j];
    }
}

// Unused
s32 func_801948B0(s32* arg0, s32* arg1) {
    s32 temp_v0;
    s32 phi_s0;
    s32 phi_s1;

    phi_s0 = *arg0;
    phi_s1 = *arg1;

    while (true) {
        temp_v0 = func_80194840(phi_s0);
        if (temp_v0 == 0) {
            return -1;
        }

        if (phi_s1 >= temp_v0) {
            phi_s1 -= temp_v0;
            phi_s0++;
        } else {
            break;
        }
    }
    
    *arg0 = phi_s0;
    *arg1 = phi_s1;

    return 0;
}

