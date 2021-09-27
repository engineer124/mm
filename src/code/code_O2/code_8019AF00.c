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

void func_8019B378(void) {
    D_801D702C = sOcarinaSongAppendPos;
}

void func_8019B38C(u32 arg0) {
    D_801D8530 = 0;
    func_8019B144(arg0);
    if (D_801D702C != 0) {
        sOcarinaSongAppendPos = D_801D702C;
        D_801D702C = 0;
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019B3D0.s")

void func_8019B4B8(u32 arg0) {
    u8 phi_v0;

    for (phi_v0 = 0; phi_v0 < 0x18; phi_v0++) {
        if (((1 << phi_v0) & arg0) != 0) {
            D_801FD530[phi_v0] = 0;
        } else {
            D_801FD530[phi_v0] = 0xFF;
        }
    }
    
    D_801D8530 = 1;
    D_801D8534 = arg0;
    func_8019B144(arg0);
}

// Gakufu
void func_8019B544(u32 arg0) {
    D_801D8530 = 0;
    func_8019B144(arg0);
}

u8 func_8019B568(void) {
    u32 temp_v0 = D_801D8534;
    u8 temp_v1 = 0;

    if ((temp_v0 % 2) == 0) {
        while (temp_v1 < 0x18) {
            temp_v1++;
            if (((temp_v0 >> temp_v1) & 1) != 0) {
                break;
            }
        }
    }
    return temp_v1;
}

u8 func_8019B5AC(void) {
    u8 temp_v0 = func_8019B568();

    if (temp_v0 < 0x18) {
        return D_801FD518[temp_v0];
    }
    return 0xFF;
}

u8 func_8019B5EC(void) {
    return D_801FD530[func_8019B568()];
}

// OoT func_800ECDBC
void func_8019B618(void) {
    if (sCurOcarinaBtnVal != 0xFF && sOcarinaHasStartedSong == 0) {
        sOcarinaHasStartedSong = 1;
        sLearnSongLastBtn = 0xFF;
    }
}

// OoT part of func_800ED200
void func_8019B654(void) {
    u8 i;

    if (sOcarinaSongAppendPos == 8) {
        for (i = 0; i < 7; i++) {
            sCurOcarinaSong[i] = sCurOcarinaSong[i + 1];
        }
    } else {
        sOcarinaSongAppendPos++;
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019B6B4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019BC44.s")

// OoT func_800ED458
void func_8019BE98(u8 arg0) {
    u32 phi_v1_2;

    if (D_801D6FEC != 0 && D_801D8538 != 0) {
        D_801D8538--;
        return;
    }

    if ((D_801FD454 == 0) ||
        ((D_801FD454 & 0x800F) != (sCurOcarinaBtnPress & 0x800F))) {
        D_801FD454 = 0;
        if (1) {}
        sCurOcarinaBtnVal = 0xFF;
        sCurOcarinaBtnIdx = 0xFF;
        phi_v1_2 = (sCurOcarinaBtnPress & 0x800F) & (sPrevOcarinaBtnPress & 0x800F);
        if (!(D_801FD45C & phi_v1_2) && (sCurOcarinaBtnPress != 0)) {
            D_801FD45C = sCurOcarinaBtnPress;
        } else {
            D_801FD45C &= phi_v1_2;
        }

        if (D_801FD45C & 0x8000) {
            sCurOcarinaBtnVal = 2;
            sCurOcarinaBtnIdx = 0;
        } else if (D_801FD45C & 0x4) {
            sCurOcarinaBtnVal = 5;
            sCurOcarinaBtnIdx = 1;
        } else if (D_801FD45C & 0x1) {
            sCurOcarinaBtnVal = 9;
            sCurOcarinaBtnIdx = 2;
        } else if (D_801FD45C & 0x2) {
            sCurOcarinaBtnVal = 11;
            sCurOcarinaBtnIdx = 3;
        } else if (D_801FD45C & 0x8) {
            sCurOcarinaBtnVal = 14;
            sCurOcarinaBtnIdx = 4;
        }

        if (sCurOcarinaBtnPress) {}

        if (sCurOcarinaBtnVal != 0xFF && sCurOcarinaBtnPress & 0x10 && D_801D8508 != 2) {
            sCurOcarinaBtnIdx += 0x80;
            sCurOcarinaBtnVal++;
        }

        if ((sCurOcarinaBtnVal != 0xFF) && (sCurOcarinaBtnPress & 0x2000) && (D_801D8508 != 2)) {
            sCurOcarinaBtnIdx += 0x40;
            sCurOcarinaBtnVal--;
        }

        if (D_801D8508 != 2) {
            D_801D6FD4 = sCurOcaStick.y;
            D_801D6FCC = Audio_OcaAdjStick(D_801D6FD4);

            D_801D6FDC = (sCurOcaStick.x < 0 ? -sCurOcaStick.x : sCurOcaStick.x) >> 2;
            Audio_QueueCmdS8(0x6020D06, D_801D6FDC);
        } else {
            D_801D6FD4 = 0;
            D_801D6FDC = 0;
            D_801D6FCC = 1.0f;
        }

        if ((sCurOcarinaBtnVal != 0xFF) && (sPrevOcarinaNoteVal != sCurOcarinaBtnVal)) {
            Audio_QueueCmdS8(0x6020D07, D_801D6FB8 - 1);
            Audio_QueueCmdS8(0x6020D05, sCurOcarinaBtnVal);
            Audio_PlaySoundGeneral(NA_SE_OC_OCARINA, &D_801DB4A4, 4, &D_801D6FCC, &D_801D6FD0, &D_801DB4B8);
        } else if ((sPrevOcarinaNoteVal != 0xFF) && (sCurOcarinaBtnVal == 0xFF) && (arg0 == 0)) {
            Audio_StopSfx(NA_SE_OC_OCARINA);
        }
    }
}

void func_8019C1C0(u8 arg0) {
    sOcarinaInpEnabled = arg0;
}

void func_8019C1D0(void) {
    sCurOcarinaBtnPress = 0;
    sPrevOcarinaBtnPress = 0;

    D_801FD45C = 0;
    D_801FD454 = 0xFFFF;

    func_8019BE98(0);
    Audio_StopSfx(0x5800);

    if (D_801DB4D4 != 0xC) {
        Audio_SetSoundBanksMute(0);
    }

    sPlaybackState = 0;
    sStaffPlaybackPos = 0;
    sOcarinaInpEnabled = 0;

    D_801D6FEC = 0;

    Audio_ClearBGMMute(0xD);
    func_801A4380(0x7F);
}

void func_8019C268(void) {
    sCurOcarinaBtnPress = 0;
    sPrevOcarinaBtnPress = 0;
    D_801FD45C = 0;
    D_801FD454 = 0xFFFF;
    func_8019BE98(1);
    sPlaybackState = 0;
    sStaffPlaybackPos = 0;
    sOcarinaInpEnabled = 0;
    D_801D6FEC = 0;
    sCurOcarinaBtnPress = 0;
    Audio_GetOcaInput();
    D_801FD454 = sCurOcarinaBtnPress;
}

void func_8019C2E4(u8 arg0, u8 arg1) {
    D_801FD460 = arg1;
    D_801FD461 = arg0;
}

// Gakufu
void func_8019C300(u8 arg0) {
    if (D_801D6FB8 != arg0 || arg0 == 1) {
        Audio_QueueSeqCmd((u32)arg0 | 0x82010D00);
        D_801D6FB8 = arg0;

        if (arg0 == 0) {
            func_8019C1D0();
        } else {
            sCurOcarinaBtnPress = 0;
            Audio_GetOcaInput();
            D_801FD454 = sCurOcarinaBtnPress;
            func_801A4380(0x40);
            Audio_QueueSeqCmdMute(0xD);
        }
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019C398.s")

void Audio_OcaPlayback(void) {
    s32 noteTimerStep;
    u32 nextNoteTimerStep;

    if (sPlaybackState != 0) {
        if (sStaffPlaybackPos == 0) {
            noteTimerStep = 3;
        } else {
            noteTimerStep = D_801FD448 - D_801D7018;
        }

        if (noteTimerStep < sNotePlaybackTimer) {
            sNotePlaybackTimer -= noteTimerStep;
        } else {
            nextNoteTimerStep = noteTimerStep - sNotePlaybackTimer;
            sNotePlaybackTimer = 0;
        }

        if (sNotePlaybackTimer == 0) {

            if (sStaffPlaybackPos == D_801D6FE4) {
                sPlaybackState = 0;
                return;
            }

            sNotePlaybackTimer = sPlaybackSong[sPlaybackNotePos].unk_02;

            if (sPlaybackNotePos == 1) {
                sNotePlaybackTimer++;
            }

            if (sNotePlaybackTimer == 0) {
                sPlaybackState--;
                if (sPlaybackState != 0) {
                    sPlaybackNotePos = 0;
                    sStaffPlaybackPos = 0;
                    sDisplayedNoteValue = 0xFF;
                } else {
                    Audio_StopSfx(NA_SE_OC_OCARINA);
                }
                return;
            } else {
                sNotePlaybackTimer -= nextNoteTimerStep;
            }

            if (sNotePlaybackVolume != sPlaybackSong[sPlaybackNotePos].volume) {
                sNotePlaybackVolume = sPlaybackSong[sPlaybackNotePos].volume;
                sNormalizedNotePlaybackVolume = sNotePlaybackVolume / 127.0f;
            }

            sNotePlaybackVibrato = sPlaybackSong[sPlaybackNotePos].vibrato;
            Audio_QueueCmdS8(0x06020D06, sNotePlaybackVibrato);

            if (sNotePlaybackTone != sPlaybackSong[sPlaybackNotePos].tone) {
                sNotePlaybackTone = sPlaybackSong[sPlaybackNotePos].tone;
                sNormalizedNotePlaybackTone = Audio_OcaAdjStick(sNotePlaybackTone);
            }

            if ((sPlaybackSong[sPlaybackNotePos].volume == sPlaybackSong[sPlaybackNotePos - 1].volume &&
                 (sPlaybackSong[sPlaybackNotePos].vibrato == sPlaybackSong[sPlaybackNotePos - 1].vibrato) &&
                 (sPlaybackSong[sPlaybackNotePos].tone == sPlaybackSong[sPlaybackNotePos - 1].tone))) {
                sDisplayedNoteValue = 0xFE;
            }

            if (sDisplayedNoteValue != sPlaybackSong[sPlaybackNotePos].noteIdx) {
                u8 noteIdx = sPlaybackSong[sPlaybackNotePos].noteIdx;

                if (noteIdx == 10) {
                    sDisplayedNoteValue = noteIdx + sPlaybackSong[sPlaybackNotePos].semitone;
                } else {
                    sDisplayedNoteValue = noteIdx;
                }

                if (sDisplayedNoteValue != 0xFF) {
                    sStaffPlaybackPos++;
                    Audio_QueueCmdS8(0x6020D07, D_801D6FB8 - 1);
                    Audio_QueueCmdS8(0x6020D05, sDisplayedNoteValue & 0x3F);
                    Audio_PlaySoundGeneral(NA_SE_OC_OCARINA, &D_801DB4A4, 4, &sNormalizedNotePlaybackTone,
                                           &sNormalizedNotePlaybackVolume, &D_801DB4B8);
                } else {
                    Audio_StopSfx(NA_SE_OC_OCARINA);
                }
            }
            sPlaybackNotePos++;
        }
    }
}

// Probably OoT func_800EDD68
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019C8D8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/D_801E0C14.s")

// Likely OoT func_800EE170
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019CD08.s")

// OoT func_800EE29C
void func_8019CE34(void) {
    D_801FD442.state = D_801D8508;
    D_801FD442.pos = D_801FD46C;
    if (D_801D8508 == 0xFF) {
        D_801D8508 = 0;
    }
}

// OoT func_800EE2D4
void func_8019CE6C(void) {
    if (sCurOcarinaBtnIdx != 0xFF) {
        D_801FD43A.noteIdx = sCurOcarinaBtnIdx & 0x3F;
    }
    D_801FD43A.state = func_8019AFE8();
    D_801FD43A.pos = D_801FD46C;
}

// OoT func_800EE318
void func_8019CEBC(void) {
    if ((sDisplayedNoteValue & 0x3F) < 0x10) {
        sDisplayedStaff.noteIdx = Audio_OcaMapNoteValue(sDisplayedNoteValue);
    }

    sDisplayedStaff.state = sPlaybackState;

    if (sPlaybackSong != sPierresSong) {
        sDisplayedStaff.pos = sStaffPlaybackPos;
    } else if (sStaffPlaybackPos == 0) {
        sDisplayedStaff.pos = 0;
    } else {
        sDisplayedStaff.pos = ((sStaffPlaybackPos - 1) % 8) + 1;
    }
}

// OoT func_800EE3C8
OcarinaStaff* func_8019CF6C(void) {
    return &D_801FD442;
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019CF78.s")

OcarinaStaff* Audio_OcaGetDisplayStaff(void) {
    return &sDisplayedStaff;
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019CFA8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019D134.s")

// Gakufu
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019D26C.s")
// s32 func_8019D134(?, ?); // extern
// extern s32 D_801D701C;
// extern ? D_801D7030;
// extern ? D_801D7F44;
// extern ? D_801D84E4;

// void func_8019D26C(void) {
//     s32 temp_s0;
//     s32 temp_s0_2;
//     s32 temp_s0_3;
//     s32 temp_s1;
//     s32 temp_s1_2;
//     s32 temp_v0;
//     s32 temp_v0_3;
//     s32 temp_v0_4;
//     u32 temp_hi;
//     u8 temp_t1;
//     void* temp_a0;
//     void* temp_t0;
//     void* temp_v0_2;
//     void* temp_v0_5;
//     void* temp_v1;
//     void* temp_v1_2;
//     void* temp_v1_3;
//     s32 phi_s0;
//     s32 phi_s1;
//     void* phi_v1;
//     s32 phi_v0;
//     s32 phi_s1_2;
//     s32 phi_s1_3;

//     do {
//         temp_v0 = D_801D701C;
//         D_801D701C = temp_v0 + 1;
//         phi_s0 = 0;
//         phi_s1 = 0;
//         phi_s1_2 = 0;
//         phi_s1_3 = 0;
//         if ((temp_v0 & 1) != 0) {
//             do {
//                 temp_t1 = *(&D_801D7030 + ((Audio_NextRandom() % 5) & 0xFF));
//                 temp_v1 = sOcarinaSongs + (phi_s0 * 8);
//                 temp_s0 = (phi_s0 + 1) & 0xFF;
//                 temp_v1->unkE62 = 0x13;
//                 temp_v1->unkE64 = 0x50;
//                 temp_v1->unkE65 = 0;
//                 temp_v1->unkE66 = 0;
//                 temp_v1->unkE60 = temp_t1;
//                 temp_v1_2 = sOcarinaSongs + (temp_s0 * 8);
//                 temp_s1 = (phi_s1 + 1) & 0xFF;
//                 temp_v1_2->unkE60 = 0xFF;
//                 temp_v1_2->unkE62 = 3;
//                 temp_v1_2->unkE64 = 0;
//                 temp_v1_2->unkE65 = 0;
//                 temp_v1_2->unkE66 = 0;
//                 temp_s0_2 = (temp_s0 + 1) & 0xFF;
//                 phi_s0 = temp_s0_2;
//                 phi_s1 = temp_s1;
//             } while (temp_s1 < 8);
//             temp_v0_2 = sOcarinaSongs + (temp_s0_2 * 8);
//             temp_v0_2->unkE52 = 0x5A;
//             temp_v0_2->unkE5A = 0x16;
//             temp_v0_2->unkE68 = 0xFF;
//             temp_v0_2->unkE6A = 0;
//             func_8019B074(0x17, 0x17, 8);
//         } else {
//             temp_hi = Audio_NextRandom() % 9;
//             temp_a0 = ((temp_hi & 0xFF) * 0xA0) + &D_801D7F44;
//             temp_s0_3 = temp_hi & 0xFF;
//             if (temp_a0->unk2 != 0) {
//                 temp_v0_3 = 0 * 8;
//                 phi_v1 = temp_a0 + temp_v0_3;
//                 phi_v0 = temp_v0_3;
//                 do {
//                     temp_t0 = sOcarinaSongs + phi_v0;
//                     temp_t0->unkE60 = (unaligned s32) phi_v1->unk0;
//                     temp_s1_2 = (phi_s1_2 + 1) & 0xFF;
//                     temp_v0_4 = temp_s1_2 * 8;
//                     temp_t0->unkE64 = (unaligned s32) phi_v1->unk4;
//                     temp_v1_3 = temp_a0 + temp_v0_4;
//                     phi_v1 = temp_v1_3;
//                     phi_v0 = temp_v0_4;
//                     phi_s1_2 = temp_s1_2;
//                     phi_s1_3 = temp_s1_2;
//                 } while (temp_v1_3->unk2 != 0);
//             }
//             temp_v0_5 = sOcarinaSongs + (phi_s1_3 * 8);
//             temp_v0_5->unkE60 = 0xFF;
//             temp_v0_5->unkE62 = 0;
//             func_8019B074(0x17, 0x17, *(&D_801D84E4 + temp_s0_3));
//         }
//     } while (func_8019D134(0x17, 0x17) != 0);
// }

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019D488.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019D4F8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019D600.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019D758.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019D864.s")

void func_8019D8B4(void) {
    D_801D8B2C = 1;
    Audio_QueueSeqCmd(0x1000077);
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019D8E4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019DF28.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019DF64.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019DFF8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019E000.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019E00C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019E014.s")

// OoT func_800F3138
void func_8019E0FC(UNK_TYPE arg0) {
}

// OoT func_800F3140
void func_8019E104(UNK_TYPE arg0, UNK_TYPE arg1) {
}

// OoT func_800F314C
void func_8019E110(s8 arg0) {
    Audio_QueueCmdS32(0x82000000 | (((u8)arg0 & 0xFF) << 8), 1);
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019E14C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019E324.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019E4B0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019E634.s")

// OoT func_800F37B8
#ifdef NON_EQUIVALENT
u8 func_8019E864(f32 arg0, SoundBankEntry* arg1, s8 arg2) {
    s8 phi_v0;
    u8 phi_v1;
    f32 phi_f0;
    f32 phi_f12;

    // Remnant of OoT
    if (*arg1->posZ < arg0) {
        phi_v1 = 0;
    } else {
        phi_v1 = 0;
    }

    if (arg1->sfxParams & 0x200) {
         phi_v1 = 0xF;
    }

    switch (arg1->sfxParams & 7) {
        case 0:
            phi_f0 = 15.0f;
            break;
        case 1:
            phi_f0 = 12.0f;
            break;
        case 2:
            phi_f0 = 9.0f;
            break;
        case 3:
            phi_f0 = 6.0f;
            break;
        case 4:
            phi_f0 = 18.0f;
            break;
        case 5:
            phi_f0 = 21.0f;
            break;
        case 6:
            phi_f0 = 24.0f;
            break;
        case 7:
            phi_f0 = 27.0f;
            break;
        default:
            break;
    }

    phi_f12 = CLAMP_MAX(arg1->dist, 1923.077f);

    return (phi_v1 * 0x10) + (u8)((phi_f0 * phi_f12) / 1923.077f);
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019E864.s")
#endif

// OoT func_800F3990
s8 func_8019EA40(f32 arg0, u16 sfxParams) {
    s8 ret;

    if (arg0 < 0.0f) {
        if (arg0 < -625.0f) {
            ret = -32;
        } else {
            ret = (s8)(((625.0f + arg0) / 625.0f) * 31.0f) + 0xE0;
        }
    } else if (arg0 > 1250.0f) {
        ret = 127;
    } else {
        ret = (arg0 / 1250.0f) * 126.0f;
    }

    return ret | 1;

}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/Audio_SetSoundProperties.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019F024.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019F05C.s")

void play_sound(u16 sfxId) {
    Audio_PlaySoundGeneral(sfxId, &D_801DB4A4, 4, &D_801DB4B0, &D_801DB4B0, &D_801DB4B8);
    if (sfxId == NA_SE_OC_TELOP_IMPACT) {
        func_801A32CC(0);
    } 
}

void func_8019F128(u16 sfxId) {
    Audio_PlaySoundGeneral(sfxId, &D_801DB4A4, 4, &D_801DB4B0, &D_801DB4B0, &D_801DB4B8);
}

void func_8019F170(Vec3f* pos, u16 sfxId) {
    Audio_PlaySoundGeneral(sfxId, pos, 4, &D_801FD254, &D_801DB4B0, &D_801FD258);
}

void func_8019F1C0(Vec3f* pos, u16 sfxId) {
    Audio_PlaySoundGeneral(sfxId, pos, 4, &D_801DB4B0, &D_801DB4B0, &D_801DB4B8);
}

void func_8019F208(void) {
    play_sound(NA_SE_SY_DECIDE);
    Audio_StopSfx(NA_SE_SY_MESSAGE_PASS);
}

void func_8019F230(void) {
    play_sound(NA_SE_SY_CANCEL);
    Audio_StopSfx(NA_SE_SY_MESSAGE_PASS);
}

// Need more info on D_801FD1F0
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019F258.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019F300.s")

void func_8019F420(Vec3f* pos, u16 sfxId) {
    if ((sfxId == NA_SE_EN_KONB_JUMP_OLD) || (sfxId == NA_SE_EN_KONB_SINK_OLD)) {
        Audio_PlaySoundGeneral(sfxId, pos, 4, &D_801DB4B0, &D_801DB4B0, &D_801DB4B8);
    } else {

        Audio_PlaySoundGeneral(sfxId, pos, 4, &D_801DB4B0, &D_801DB4B0, &D_801D66F0);
    }    
}

// Need more info on func_8019F258
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019F4AC.s")

void func_8019F540(s8 arg0) {
    if (arg0 != 0) {
        D_801D66F0 = -0x80;
    } else {
        D_801D66F0 = 0;
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019F570.s")

// OoT func_800F3F84
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019F5AC.s")

// OoT func_800F4010
void func_8019F638(Vec3f* pos, u16 sfxId, f32 arg2) {
    f32 sp2C;
    f32 phi_f0;
    s32 phi_v0;
    u16 sfxId2;

    sfxId2 = 0;
    sp2C = func_8019F5AC(arg2);
    Audio_PlaySoundGeneral(sfxId, pos, 4, &D_801FD264, &D_801FD25C, &D_801DB4B8);
    if ((sfxId & 0xF0) == 0xB0) {
        phi_f0 = 0.3f;
        phi_v0 = 1;
        sp2C = 1.0f;
    } else {
        phi_f0 = 1.1f;
        phi_v0 = gAudioContext.audioRandom & 1 & 0xFF;
    }
    if (phi_f0 < arg2) {
        if (phi_v0 != 0) {
            if ((sfxId & 0x1FF) < 0x80) {
                sfxId2 = NA_SE_PL_METALEFFECT_KID;
            } else if ((sfxId & 0x1FF) < 0xF0) {
                sfxId2 = NA_SE_PL_METALEFFECT_ADULT;
            }
            if (sfxId2 != 0) {
                D_801FD260 = (sp2C * 0.7) + 0.3;
                Audio_PlaySoundGeneral(sfxId2, pos, 4, &D_801FD264, &D_801FD260, &D_801DB4B8);
            }
        }
    }
}

// OoT func_800F4138
void func_8019F780(Vec3f* pos, u16 sfxId, f32 arg2) {
    func_8019F5AC(arg2);
    Audio_PlaySoundGeneral(sfxId, pos, 4, &D_801FD264, &D_801FD25C, &D_801DB4B8);
}

void func_8019F7D8(Vec3f* pos, u16 sfxId) {
    Audio_PlaySoundGeneral(sfxId | 0xE0, pos, 4, &D_801D6648, &D_801DB4B0, &D_801D664C);
}

void func_8019F830(Vec3f* pos, u16 sfxId) {
    Audio_PlaySoundGeneral((sfxId & 0x681F) + 0x20, pos, 4, &D_801D6648, &D_801DB4B0, &D_801D664C);
}

void Audio_PlaySoundRandom(Vec3f* pos, u16 baseSfxId, u8 randLim) {
    u8 offset = Audio_NextRandom() % randLim;

    Audio_PlaySoundGeneral(baseSfxId + offset, pos, 4, &D_801DB4B0, &D_801DB4B0, &D_801DB4B8);
}

// OoT func_800F4254
void func_8019F900(Vec3f* pos, u8 level) {
    level &= 3;
    if (level != sPrevChargeLevel) {
        D_801D6694 = D_801D6684[level];
        switch (level) {
            case 1:
                Audio_PlaySoundGeneral(NA_SE_PL_SWORD_CHARGE, pos, 4, &D_801D6694, &D_801DB4B0, &D_801DB4B8);
                break;
            case 2:
                Audio_PlaySoundGeneral(NA_SE_PL_SWORD_CHARGE, pos, 4, &D_801D6694, &D_801DB4B0, &D_801DB4B8);
                break;
        }
        sPrevChargeLevel = level;
    }
    if (level != 0) {
        Audio_PlaySoundGeneral(NA_SE_IT_SWORD_CHARGE - SFX_FLAG, pos, 4, &D_801D6694, &D_801DB4B0, &D_801DB4B8);
    }
}

// OoT func_800F436C
// More info on func_8019F258
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019FA18.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019FAD8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019FB0C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019FC20.s")

// OoT func_800F4414
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019FCB8.s")

// OoT func_800F44EC
void func_8019FD90(s8 arg0, s8 arg1) {
    D_801D6658 = 1;
    D_801D665C = arg1;
    D_801D6660 = arg0;
}

// OoT func_800F4524
void func_8019FDC8(Vec3f* pos, u16 sfxId, s8 arg2) {
    D_801FD28C = arg2;
    Audio_PlaySoundGeneral(sfxId, pos, 4, &D_801DB4B0, &D_801DB4B0, &D_801FD28C);
}

// OoT func_800F4578
void func_8019FE1C(Vec3f* pos, u16 sfxId, f32 arg2) {
    D_801D6654 = arg2;
    Audio_PlaySoundGeneral(sfxId, pos, 4, &D_801DB4B0, &D_801D6654, &D_801DB4B8);
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019FE74.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019FEDC.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019FF38.s")

void Audio_PlaySoundRiver(Vec3f* pos, f32 freqScale) {
    if (!Audio_IsSfxPlaying(NA_SE_EV_RIVER_STREAM - SFX_FLAG)) {
        sRiverFreqScaleLerp.value = freqScale;
    } else if (freqScale != sRiverFreqScaleLerp.value) {
        sRiverFreqScaleLerp.target = freqScale;
        sRiverFreqScaleLerp.remainingFrames = 40;
        sRiverFreqScaleLerp.step = (sRiverFreqScaleLerp.target - sRiverFreqScaleLerp.value) / 40;
    }
    Audio_PlaySoundGeneral(NA_SE_EV_RIVER_STREAM - SFX_FLAG, pos, 4, &sRiverFreqScaleLerp.value, &D_801DB4B0, &D_801DB4B8);
}

void Audio_PlaySoundWaterfall(Vec3f* pos, f32 freqScale) {
    if (!Audio_IsSfxPlaying(NA_SE_EV_WATER_WALL_BIG - SFX_FLAG)) {
        sWaterfallFreqScaleLerp.value = freqScale;
    } else if (freqScale != sWaterfallFreqScaleLerp.value) {
        sWaterfallFreqScaleLerp.target = freqScale;
        sWaterfallFreqScaleLerp.remainingFrames = 40;
        sWaterfallFreqScaleLerp.step = (sWaterfallFreqScaleLerp.target - sWaterfallFreqScaleLerp.value) / 40;
    }
    Audio_PlaySoundGeneral(NA_SE_EV_WATER_WALL_BIG - SFX_FLAG, pos, 4, &sWaterfallFreqScaleLerp.value,
                           &sWaterfallFreqScaleLerp.value, &D_801DB4B8);
}

void Audio_StepFreqLerp(FreqLerp* lerp) {
    if (lerp->remainingFrames != 0) {
        lerp->remainingFrames--;
        if (lerp->remainingFrames != 0) {
            lerp->value += lerp->step;
        } else {
            lerp->value = lerp->target;
        }
    }
}

#ifdef NON_EQUIVALENT
void func_801A0124(Vec3f* pos, u8 arg1) {
    Audio_PlaySoundGeneral(NA_SE_EV_SIGNAL_BIGBELL, pos, 4, &D_801DB4B0, (D_801D8BB0 + (u32)(4 * (arg1 & 7))), &D_801DB4B8);
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A0124.s")
#endif

// OoT func_800F47BC
void func_801A0184(void) {
    Audio_SetVolScale(0, 1, 0, 10);
    Audio_SetVolScale(3, 1, 0, 10);
}

// OoT func_800F47FC
void func_801A01C4(void) {
    Audio_SetVolScale(0, 1, 0x7F, 3);
    Audio_SetVolScale(3, 1, 0x7F, 3);
}

#ifdef NON_MATCHING
void func_801A0204(s8 arg0) {
    Audio_QueueCmdS8(MK_CMD(0x46,0, 0, 2), arg0);
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A0204.s")
#endif

// OoT func_800F483C
void func_801A0238(u8 targetVol, u8 volFadeTimer) {
    Audio_SetVolScale(0, 0, targetVol, volFadeTimer);
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A026C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A0318.s")

void func_801A0450(u8 arg0) {
    D_801FD28D = arg0;
    D_801FD28F = 1;
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A046C.s")

void Audio_PlaySoundIncreasinglyTransposed(Vec3f* pos, s16 sfxId, u8* semitones) {
    Audio_PlaySoundGeneral(sfxId, pos, 4, &gNoteFrequencies[semitones[sAudioIncreasingTranspose] + 39], &D_801DB4B0, &D_801DB4B8);
    if (sAudioIncreasingTranspose < 15) {
        sAudioIncreasingTranspose++;
    }
}

void Audio_ResetIncreasingTranspose(void) {
    sAudioIncreasingTranspose = 0;
}

void Audio_PlaySoundTransposed(Vec3f* pos, u16 sfxId, s8 semitone) {
    Audio_PlaySoundGeneral(sfxId, pos, 4, &gNoteFrequencies[semitone + 39], &D_801DB4B0, &D_801DB4B8);
}

void func_801A0654(Vec3f* pos, u16 sfxId, u8 arg2);
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A0654.s")

void func_801A0810(Vec3f* pos, u16 sfxId, u8 arg2) {
    func_801A0654(pos, sfxId, arg2);
    Audio_PlaySoundGeneral(sfxId, pos, 4, &D_801DB4B0, &D_801DB4B0, &D_801DB4B8);
}

// OoT func_800F4C58
void func_801A0868(Vec3f* pos, u16 sfxId, u8 arg2) {
    u8 phi_s1 = 0;
    u8 i;
    u8 bank;

    bank = SFX_BANK_SHIFT(sfxId);
    for (i = 0; i < bank; i++) {
        phi_s1 += D_801D6610[D_801DB49C][i];
    }

    for (i = 0; i < D_801D6610[D_801DB49C][bank]; i++) {
        Audio_QueueCmdS8(_SHIFTL(6, 24, 8) | _SHIFTL(2, 16, 8) | _SHIFTL(phi_s1++, 8, 8) | _SHIFTL(6, 0, 8), arg2);
    }

    Audio_PlaySoundGeneral(sfxId, pos, 4, &D_801DB4B0, &D_801DB4B0, &D_801DB4B8);
}

// OoT func_800F4E30
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A09D4.s")

// OoT Audio_ClearSariaBgm
void func_801A0C70(void) {
    if (D_801D66F4 != NULL) {
        D_801D66F4 = NULL;
    }
}

// OoT Audio_ClearSariaBgmAtPos
void func_801A0C90(Vec3f* pos) {
    if (D_801D66F4 == pos) {
        D_801D66F4 = NULL;
    }
}

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

// OoT Audio_PlaySariaBgm
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A21FC.s")

// OoT Audio_ClearSariaBgm2
void func_801A2460(void) {
    D_801D66F4 = NULL;
}

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
void func_801A3AEC(u8 arg0) {
    D_801D66AC = arg0;
    if (arg0 != 0) {
        play_sound(NA_SE_SY_WIN_OPEN);
        Audio_QueueCmdS32(0xF1FF0000, 0);
    } else {
        play_sound(NA_SE_SY_WIN_CLOSE);
        Audio_QueueCmdS32(0xF2FF0000, 0);
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A3B48.s")

// OoT func_800F6584
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A3B90.s")

void Audio_SetEnvReverb(s8 reverb) {
    sAudioEnvReverb = reverb & 0x7F;
}

void Audio_SetCodeReverb(s8 reverb) {
    u8 temp_a0;

    if (reverb != 0) {
        if ((reverb & 0x40) != (sAudioCodeReverb & 0x40)) {
            temp_a0 = (reverb >> 6) + 1;
            func_801A4428(temp_a0, reverb);
        }
        sAudioCodeReverb = reverb & 0x7F;
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A3D54.s")

// OoT func_800F6700
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A3D98.s")

// OoT func_800F67A0
void func_801A3E38(u8 arg0) {
    if (D_801D66E0 != arg0) {
        if (arg0 == 0) {
            Audio_StopSfx(NA_SE_PL_IN_BUBBLE);
        } else if (D_801D66E0 == 0) {
            Audio_PlaySoundGeneral(NA_SE_PL_IN_BUBBLE, &D_801DB4A4, 4, &D_801DB4B0, &D_801DB4B0, &D_801DB4B8);
        }
    }
    D_801D66E0 = arg0;
    D_801D66E8 = arg0;
}

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

void func_801A4380(u8 arg0) {
    u8 i;

    if (D_801FD3AF == 0) {
        for (i = 0; i < 0x10; i++) {
            switch (i) {
                case 0xB:
                case 0xC:
                case 0xD:
                    break;
                default:
                    Audio_QueueSeqCmd(0x60000000 | (2 << 24) | ((0xA) << 16) | ((u32)i << 8) | (arg0));
                    break;
            }
        }
    }
}

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

