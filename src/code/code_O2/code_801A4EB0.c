#include "global.h"

void func_801A5680(u16 arg0);
void func_801A54D0(u16 arg0);

extern u8 D_801D8E3C;
extern u16 D_801D8E44;
extern s32 D_801FD5A0;
extern UNK_TYPE4 D_801FD608;

void func_801A4EB0(void) {
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A4EB8.s")
// s32 func_80175008(); // extern
// ? func_8017506C(s32); // extern
// s32 func_801A5228(?*); // extern
// ? func_801A53E8(?, ?, ?, ?, s32); // extern
// s32 func_801A54C4(); // extern
// ? func_801A54D0(s32); // extern
// ? osVoiceStopReadData(?*); // extern
// extern ? D_801D8BE0;
// extern u8 D_801D8E3C;
// extern ? D_801FD5B8;

// void func_801A4EB8(void) {
//     ? sp38;
//     s32 temp_s0;
//     s32 temp_s0_2;
//     s32 temp_s0_3;
//     s32 phi_s0;
//     s32 phi_v0;
//     s32 phi_s0_2;

//     if (D_801D8E3C != 0) {
//         temp_s0 = func_80175008();
//         osVoiceStopReadData(&D_801FD5B8);
//         func_8017506C(temp_s0);
//     }
//     phi_s0 = 0;
//     phi_s0_2 = 0;
//     do {
//         temp_s0_2 = (phi_s0 + 1) & 0xFF;
//         *(&sp38 + phi_s0) = *(phi_s0 + func_801A54C4());
//         phi_s0 = temp_s0_2;
//     } while (temp_s0_2 <= 0);
//     if (func_801A5228(&D_801D8BE0) == 0) {
//         phi_v0 = 0;
//         do {
//             if (((*(&sp38 + (phi_v0 / 8)) >> (phi_v0 % 8)) & 1) == 1) {
//                 func_801A54D0(phi_s0_2 & 0xFFFF);
//             }
//             temp_s0_3 = (phi_s0_2 + 1) & 0xFF;
//             phi_v0 = temp_s0_3;
//             phi_s0_2 = temp_s0_3;
//         } while (temp_s0_3 < 6);
//         func_801A53E8(0x320, 2, 0x400, 0x1F4, 0x7D0);
//         D_801D8E3C = 1;
//     }
// }

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A4FD8.s")


void func_801A5080(u16 arg0) {
    if ((D_801D8E3C != 0) && (arg0 < 6)) {
        func_801A5680(arg0);
    }
}

void func_801A50C0(u16 arg0) {
    if ((D_801D8E3C != 0) && (arg0 < 6)) {
        func_801A54D0(arg0);
    }
}

u16 func_801A5100(void) {
    return D_801D8E44;
}

u8 func_801A510C(void) {
    return D_801D8E3C;
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A5118.s")

// Break here

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A4EB0/D_801E0EC0.s")

UNK_TYPE func_801A51F0(UNK_TYPE arg0) {
    switch (arg0) {
        case 1:
        case 4:
        case 5:
        case 11:
        case 13:
        case 14:
        case 15:
            return -1;
        default:
            return 0;
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A4EB0/func_801A5228.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A4EB0/func_801A5390.s")

s32 func_801A53DC(void) {
    return D_801FD5A0;
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A4EB0/func_801A53E8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A4EB0/func_801A541C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A4EB0/func_801A5488.s")

UNK_PTR func_801A54C4(void) {
    return &D_801FD608;
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A4EB0/func_801A54D0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A4EB0/func_801A5680.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A4EB0/func_801A5808.s")

void AudioVoice_ResetData(void) {
    D_801FD5A0 = 0;
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A4EB0/func_801A5A1C.s")
