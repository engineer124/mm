#include "global.h"

// TODO: can these macros be shared between files? code_800F9280 seems to use
// versions without any casts...
#define Audio_DisableSeq(seqIdx, fadeOut) Audio_QueueCmdS32(0x83000000 | ((u8)seqIdx << 16), fadeOut)
#define Audio_StartSeq(seqIdx, fadeTimer, seqId) \
    Audio_QueueSeqCmd(0x00000000 | ((u8)seqIdx << 24) | ((u8)(fadeTimer) << 0x10) | (u16)seqId)
#define Audio_SeqCmd7(seqIdx, a, b) Audio_QueueSeqCmd(0x70000000 | ((u8)seqIdx << 0x18) | ((u8)a << 0x10) | (u8)(b))
#define Audio_SeqCmdC(seqIdx, a, b, c) \
    Audio_QueueSeqCmd(0xC0000000 | ((u8)seqIdx << 24) | ((u8)a << 16) | ((u8)b << 8) | ((u8)(c)))
#define Audio_SeqCmdA(seqIdx, a) Audio_QueueSeqCmd(0xA0000000 | ((u8)seqIdx << 24) | ((u16)(a)))
#define Audio_SeqCmd1(seqIdx, a) Audio_QueueSeqCmd(0x100000FF | ((u8)seqIdx << 24) | ((u8)(a) << 16))
#define Audio_SeqCmdB(seqIdx, a, b, c) \
    Audio_QueueSeqCmd(0xB0000000 | ((u8)seqIdx << 24) | ((u8)a << 16) | ((u8)b << 8) | ((u8)c))
#define Audio_SeqCmdB40(seqIdx, a, b) Audio_QueueSeqCmd(0xB0004000 | ((u8)seqIdx << 24) | ((u8)a << 16) | ((u8)b))
#define Audio_SeqCmd6(seqIdx, a, b, c) \
    Audio_QueueSeqCmd(0x60000000 | ((u8)seqIdx << 24) | ((u8)(a) << 16) | ((u8)b << 8) | ((u8)c))
#define Audio_SeqCmdE0(seqIdx, a) Audio_QueueSeqCmd(0xE0000000 | ((u8)seqIdx << 24) | ((u8)a))
#define Audio_SeqCmdE01(seqIdx, a) Audio_QueueSeqCmd(0xE0000100 | ((u8)seqIdx << 24) | ((u16)a))
#define Audio_SeqCmd8(seqIdx, a, b, c) \
    Audio_QueueSeqCmd(0x80000000 | ((u8)seqIdx << 24) | ((u8)a << 16) | ((u8)b << 8) | ((u8)c))
#define Audio_SeqCmdF(seqIdx, a) Audio_QueueSeqCmd(0xF0000000 | ((u8)seqIdx << 24) | ((u8)a))

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/D_801E0BD0.s")

#ifdef NON_MATCHING
void Audio_GetOcaInput(void) {
    Input inputs[4];
    Input* controller1 = &inputs[0];
    u32 sp18;

    sp18 = sCurOcarinaBtnPress;
    Padmgr_GetInput2(inputs, 0);
    sCurOcarinaBtnPress = controller1->cur.button;
    sPrevOcarinaBtnPress = sp18;
    sCurOcaStick.x = controller1->rel.stick_x;
    sCurOcaStick.y = controller1->rel.stick_y;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/Audio_GetOcaInput.s")
#endif

f32 Audio_OcaAdjStick(s8 inp) {
    s8 inpAdj;
    f32 ret;

    if (inp > 0x40) {
        inpAdj = 127;
    } else if (inp < -0x40) {
        inpAdj = -128;
    } else if (inp >= 0) {
        inpAdj = (inp * 127) / 64;
    } else {
        inpAdj = (inp * 128) / 64;
    }
    ret = D_801D53B4[inpAdj];
    return ret;
}

// OoT func_800ECAF0
u8 func_8019AFE8(void) {
    u8 ret;

    if (D_801D8528 != 0) {
        ret = D_801D8528 - 1;
        D_801D8528 = 0;
    } else if (D_801D6FEC != 0) {
        ret = 0xFE;
    } else {
        ret = 0xFF;
    }

    return ret;
}

u8 Audio_OcaMapNoteValue(u8 arg0) {
    u8 temp_v1 = sNoteValueIndexMap[arg0 & 0x3F];

    if (temp_v1 == 5) {
        if (arg0 & 0x80) {
            return 2;
        }
        return 3;
    }
    return temp_v1;
}

void func_8019B074(u8 songIdx, u8 arg1, u8 arg2) {
    u8 savedSongIdx;
    u8 scarecrowSongIdx;
    u8 noteIdx;

    savedSongIdx = 0;
    scarecrowSongIdx = 0;

    while (savedSongIdx < arg2 && scarecrowSongIdx < 0x10) {
        noteIdx = sOcarinaSongs[songIdx][scarecrowSongIdx++].noteIdx;
        if (noteIdx != 0xFF) {
            gOcarinaSongNotes[arg1].notesIdx[savedSongIdx++] = sNoteValueIndexMap[noteIdx];
        }
    }

    gOcarinaSongNotes[arg1].len = arg2;
}

// start ocarina.
#ifdef NON_EQUIVALENT
void func_8019B144(u32 flg) {
    u8 i;
    s32 j;

    if ((sOcarinaSongs[22][1].volume != 0xFF) && ((flg & 0x3FFF) == 0x3FFF)) {
        flg |= 0x400000;
    }

    if ((flg == 0xC0003FFF) && (sOcarinaSongs[22][1].volume != 0xFF)) {
        flg = 0xC0003FFF;
    }

    if ((flg == 0x3FFF) && (sOcarinaSongs[22][1].volume != 0xFF)) {
        flg = 0x3FFF;
    }

    if (flg != 0xFFFF) {
        D_801D6FEC = 0x80000000 + (u32)flg;
        sOcarinaSongNoteStartIdx = 0;
        sOcarinaSongCount = 0x18;
        sOcarinaAvailSongs = flg & 0xFFFFFF;
        D_801D852C = 8;
        sOcarinaHasStartedSong = 0;
        D_801D8528 = 0;
        D_801FD46C = 0;
        D_801FD43B.noteIdx = func_8019AFE8();
        sOcarinaInpEnabled = 1;
        D_801D6FFC = 0;
        for (i = 0; i < 0x15; i++) {
            sLearnSongPos[i] = 0;

            j = 0;
            while (sOcarinaSongs[i][j & 0xFFFF].noteIdx == 0xFF) {
                sLearnSongPos[i] = j & 0xFFFF;
            }

            D_801FD4A0[i] = 0;
            D_801FD4D0[i] = 0;
            D_801FD500[i] = 0;

            if (D_801D8530 != 0) {
                if (((1 << i) & flg) != 0) {
                    D_801FD518[i] = 0;
                } else {
                    D_801FD518[i] = 0xFF;
                }
            }
        }

        if (flg & 0x80000000) {
            D_801D852C = 0;
        }

        if (flg & 0x40000000) {
            sOcarinaSongAppendPos = 0;
        }

        if (flg & 0xC0400000) {
            func_8019B074(0x16, 0x16, 8);
            return;
        }
    } else {
        D_801D6FEC = 0;
        sOcarinaInpEnabled = 0;
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019B144.s")
#endif

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019B378.s")


#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019B38C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019B3D0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019B4B8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019B544.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019B568.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019B5AC.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019B5EC.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019B618.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019B654.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019B6B4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019BC44.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019BE98.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019C1C0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019C1D0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019C268.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019C2E4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019C300.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019C398.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019C5A0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019C8D8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/D_801E0C14.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019CD08.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019CE34.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019CE6C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019CEBC.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019CF6C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019CF78.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019CF9C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019CFA8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019D134.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019D26C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019D488.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019D4F8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019D600.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019D758.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019D864.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019D8B4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019D8E4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019DF28.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019DF64.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019DFF8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019E000.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019E00C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019E014.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019E0FC.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019E104.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019E110.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019E14C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019E324.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019E4B0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019E634.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019E864.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019EA40.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/Audio_SetSoundProperties.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019F024.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019F05C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/play_sound.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019F128.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019F170.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019F1C0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019F208.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019F230.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019F258.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019F300.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019F420.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019F4AC.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019F540.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019F570.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019F5AC.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019F638.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019F780.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019F7D8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019F830.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019F88C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019F900.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019FA18.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019FAD8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019FB0C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019FC20.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019FCB8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019FD90.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019FDC8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019FE1C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019FE74.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019FEDC.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019FF38.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019FF9C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A0048.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A00EC.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A0124.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A0184.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A01C4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A0204.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A0238.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A026C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A0318.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A0450.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A046C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A0554.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A05E4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A05F0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A0654.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A0810.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A0868.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A09D4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A0C70.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A0C90.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A0CB0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A0E44.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A1290.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A1348.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A13BC.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A153C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A17F4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A1904.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A1A10.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A1A8C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A1D44.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A1DB8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A1E0C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A1F00.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A1F88.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A1FB4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A2090.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A21FC.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A2460.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A246C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A2544.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A257C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A25E4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A2670.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A2778.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A27E8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A281C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A29D4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A2BB8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A2C20.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A2C44.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A2C88.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A2D54.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A2DE0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A2E54.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A2ED8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A2F88.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A2FC4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A3000.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A3038.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A3098.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A312C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A31EC.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A3238.s")

// OoT func_800F5E90
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A32CC.s")

// OoT func_801A3590
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A3590.s")

// OoT func_801A36F0
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A36F0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A3950.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A39F8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A3A7C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A3AC0.s")

// OoT func_800F64E0
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A3AEC.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A3B48.s")

// OoT func_800F6584
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A3B90.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/Audio_SetEnvReverb.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/Audio_SetCodeReverb.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A3D54.s")

// OoT func_800F6700
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A3D98.s")

// OoT func_800F67A0
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A3E38.s")

// OoT func_800F6828
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A3EC0.s")

void Audio_SetCutsceneFlag(s8 flag) {
    sAudioCutsceneFlag = flag;
}

void Audio_PlaySoundGeneralIfNotInCutscene(u16 sfxId, Vec3f* pos, u8 arg2, f32* freqScale, f32* arg4, s8* reverbAdd) {
    if (!sAudioCutsceneFlag) {
        Audio_PlaySoundGeneral(sfxId, pos, arg2, freqScale, arg4, reverbAdd);
    }
}

void Audio_PlaySoundIfNotInCutscene(u16 sfxId) {
    Audio_PlaySoundGeneralIfNotInCutscene(sfxId, &D_801DB4A4, 4, &D_801DB4B0, &D_801DB4B0, &D_801DB4B8);
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A3FFC.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/audio_setBGM.s")

// OoT func_800F6964
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A4058.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A41C8.s")

// OoT func_800F6AB0
#ifdef NON_EQUIVALENT
void func_801A41F8(u16 arg0) {
    Audio_SeqCmd1(0, arg0);
    Audio_SeqCmd1(1, arg0);
    Audio_SeqCmd1(3, arg0);
    Audio_SetVolScale(0, 3, 0x7F, 0);
    Audio_SetVolScale(0, 1, 0x7F, 0);
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A41F8.s")
#endif

// OoT func_800F6B3C
void func_801A429C(void) {
    func_801A7B10(2, 0, 0xFF, 5);
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/Audio_DisableAllSeq.s")

// OoT func_800F6BB8
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A4324.s")

// OoT func_800F6BDC
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A4348.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A4380.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A4428.s")

void Audio_PreNMI(void) {
    Audio_PreNMIInternal();
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A44C4.s")

// OoT func_800F6C34
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A44D4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A46F8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A4748.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A479C.s")

// OoT func_800F6D58
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A47DC.s")

// OoT func_800F6E7C
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A48E0.s")

// OoT func_800F6FB4
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A4A28.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A4B80.s")

// OoT func_800F70F8
void func_801A4C30(void) {
    Audio_ContextInit(0, 0);
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A4C54.s")

// OoT func_800F711C
void func_801A4D00(void) {
    func_801A44D4();
    func_8019DF64();
    func_8019F05C();
    func_801A9A74();
    func_801A794C();
    func_801A5A10();
    func_801A4C54(0xA);
}

// OoT func_800F7170
void func_801A4D50(void) {
    func_801A4C54(1);
    Audio_QueueCmdS32(0xF2FF0000, 1);
    Audio_ScheduleProcessCmds();
    Audio_QueueCmdS32(0xF8000000, 0);
    D_801FD3D8 = 0;
    D_801D66FC = 0;
    D_801FD3AE = 0;
}

// New to MM
// Wait for more info on D_801DB930
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A4DA4.s")

// OoT func_800F71BC
void func_801A4DF4(s32 arg0) {
    D_801DB4D8 = 1;
    func_801A44D4();
    func_8019DF64();
    func_8019F05C();
    func_801A99B8();
    func_801A794C();
    func_801A4FD8();
    if (D_801DB4D4 == 0xB) {
        Audio_SetSoundBanksMute(0x6F);
    }
}

// func_800F7208
void func_801A4E64(void) {
    func_801A99B8();
    Audio_QueueCmdS32(0xF2FF0000, 1);
    func_801A44D4();
    func_8019F05C();
    func_801A4C54(1);
    func_801A4FD8();
}

