#include "global.h"

// Break here
s32 func_80175008();
void func_8017506C(s32);
void osVoiceStartReadData(OSVoiceHandle*);  

typedef struct {
    /* 0x00 */ u8* unk_00;
    /* 0x04 */ u8 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ s16 unk_0E;
    /* 0x10 */ s16 unk_10;
    /* 0x12 */ s16 unk_12;
    /* 0x14 */ u16 unk_14;
} OSVoiceUnk; // size = 0x18

extern OSVoiceUnk D_801FD5A0;
extern OSVoiceHandle D_801FD5B8;
extern u8 D_801D8E3C;
extern u16 D_801D8E44;
extern u8 D_801FD608[];

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A51F0/D_801E0EC0.s")

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

void func_801A5A1C(u8*);                      /* extern */
s32 osVoiceSetWord(OSVoiceHandle* hd, u8* word);   /* extern */
s32 osVoiceCheckWord(u8* word);

#ifdef NON_EQUIVALENT
s32 func_801A5228(u8* arg0) {
    s32 temp_s2;
    s32 temp_s3;
    s32 temp_v1;
    u8 temp_s1;
    u8 i;

    D_801FD5A0.unk_04 = 0;
    D_801FD5A0.unk_08 = 0;
    D_801FD5A0.unk_0C = 0x3E8;
    D_801FD5A0.unk_0E = 5;
    D_801FD5A0.unk_10 = 0;
    D_801FD5A0.unk_00 = arg0;
    temp_s1 = arg0[0x258];
    temp_s2 = func_80175008();
    temp_s3 = osVoiceClearDictionary(&D_801FD5B8, temp_s1);
    func_8017506C(temp_s2);

    if (temp_s3 != 0) {
        return temp_s3;
    } 

    i = 0;
    temp_v1 = ((temp_s1 - 1) / 8) + 1;

    for (; i < temp_v1; i++) {
        D_801FD608[i] = 0;

    }


    for (i = 0; i < temp_v1; i++) {
        temp_s2 = func_80175008();
        temp_s3 = osVoiceSetWord(&D_801FD5B8, &arg0[i]);
        func_8017506C(temp_s2);
        
        if (func_801A51F0(temp_s3) != 0) {
            func_801A5A1C(&arg0[i]);
        }
    }

    return temp_s3;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A51F0/func_801A5228.s")
#endif

s32 func_801A5390(void) {
    s32 temp_t6;
    s32 sp18;

    temp_t6 = D_801FD5A0.unk_08;
    D_801FD5A0.unk_08 = 0;
    sp18 = func_80175008();
    osVoiceStartReadData(&D_801FD5B8);
    func_8017506C(sp18);
    return temp_t6;
}

s32 func_801A53DC(void) {
    return D_801FD5A0.unk_00;
}

void func_801A53E8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u16 arg4) {
    s32* arg0Temp = &arg0;
    s32* arg1Temp = &arg1;
    s32* arg2Temp = &arg2;
    s32* arg3Temp = &arg3;

    D_801FD5A0.unk_0C = *arg0Temp;
    D_801FD5A0.unk_0E = *arg1Temp;
    D_801FD5A0.unk_10 = *arg2Temp;
    D_801FD5A0.unk_12 = *arg3Temp;
    D_801FD5A0.unk_14 = arg4;
}

void func_801A541C(s32 analog, s32 digital) {
    s32 sp1C;
    s32 sp18;

    if (D_801FD5A0.unk_00 != 0) {
        sp18 = func_80175008();
        sp1C = osVoiceControlGain(&D_801FD5B8, analog, digital);
        func_8017506C(sp18);
        if (sp1C != 0) {
            func_801A51F0(sp1C);
        }
    }
}

s32 func_801A5488(u8* word) {
    s32 sp1C;
    s32 sp18;
    s32 temp_v0;

    sp18 = func_80175008();
    sp1C = osVoiceCheckWord(word);
    func_8017506C(sp18);
    return sp1C;
}

UNK_PTR func_801A54C4(void) {
    return &D_801FD608;
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A51F0/func_801A54D0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A51F0/func_801A5680.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A51F0/func_801A5808.s")

void AudioVoice_ResetData(void) {
    D_801FD5A0.unk_00 = 0;
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A51F0/func_801A5A1C.s")
