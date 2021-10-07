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
#define Audio_SeqCmd1(seqIdx, a) Audio_QueueSeqCmd(0x100000FF | ((u8)seqIdx << 24) | ((a) << 16))
#define Audio_SeqCmdB(seqIdx, a, b, c) \
    Audio_QueueSeqCmd(0xB0000000 | ((u8)seqIdx << 24) | ((u8)a << 16) | ((u8)b << 8) | ((u8)c))
#define Audio_SeqCmdB40(seqIdx, a, b) Audio_QueueSeqCmd(0xB0004000 | ((u8)seqIdx << 24) | ((u8)a << 16) | ((u8)b))
#define Audio_SeqCmd6(seqIdx, a, b, c) \
    Audio_QueueSeqCmd(0x60000000 | ((u32)seqIdx << 24) | ((u32)(a) << 16) | ((u32)b << 8) | ((u8)c))
#define Audio_SeqCmdE0(seqIdx, a) Audio_QueueSeqCmd(0xE0000000 | ((u8)seqIdx << 24) | (a))
#define Audio_SeqCmdE01(seqIdx, a) Audio_QueueSeqCmd(0xE0000100 | ((u8)seqIdx << 24) | ((u16)a))
#define Audio_SeqCmd8(seqIdx, a, b, c) \
    Audio_QueueSeqCmd(0x80000000 | (seqIdx << 24) | (a << 16) | (b << 8) | (c))
#define Audio_SeqCmdF(seqIdx, a) Audio_QueueSeqCmd(0xF0000000 | ((u8)seqIdx << 24) | ((u8)a))

s32 func_8019D4F8(void);

/*
s16 D_801E0BD0[] = {
    0x2006, 0x20F2, 0x2007, 0x2028, 0x200B, 0x2032, 0x203B, 0x2031, 0x2041, 0x2155, 0x205D,
    0x20CD, 0x210F, 0x203F, 0x20CA, 0x20E5, 0x2176, 0x20C1, 0x20C7, 0x20B7, 0x20B7, 0x20B7,
};
*/
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
        sStaffPlayingPos = 0;
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
// TODO: u16?
// start ocarina.
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

// OoT func_800ED200
void func_8019BC44(void) {
    u32 temp_v0;
    u8 temp_v0_2;
    u8 i;
    u8 j;
    u8 k;

    if (CHECK_BTN_ANY(sCurOcarinaBtnPress, BTN_L) && CHECK_BTN_ANY(sCurOcarinaBtnPress, 0x800F)) {
        func_8019B544(D_801D6FEC);
        return;
    }

    func_8019B618();

    if (sOcarinaHasStartedSong) {
        temp_v0_2 = sStaffPlayingPos;
        if ((sPrevOcarinaNoteVal != sCurOcarinaBtnVal) && (sCurOcarinaBtnVal != 0xFF)) {
            sStaffPlayingPos++;
            if (sStaffPlayingPos >= 9) {
                sStaffPlayingPos = 1;
            }

            func_8019B654();

            if (((D_801D6FD4 < 0 ? -D_801D6FD4 : D_801D6FD4) >= 0x15) && (temp_v0_2 != sStaffPlayingPos)) {
                sCurOcarinaSong[sOcarinaSongAppendPos - 1] = 0xFF;
            } else {
                sCurOcarinaSong[sOcarinaSongAppendPos - 1] = sCurOcarinaBtnVal;
            }

            for (i = sOcarinaSongNoteStartIdx; i < sOcarinaSongCount; i++) {
                if ((u32)sOcarinaAvailSongs & (1 << i)) {
                    for (j = 0, k = 0;
                         j < gOcarinaSongNotes[i].len && k == 0 && sOcarinaSongAppendPos >= gOcarinaSongNotes[i].len;) {
                        temp_v0 = sCurOcarinaSong[(sOcarinaSongAppendPos - gOcarinaSongNotes[i].len) + j];
                        if (temp_v0 == sOcarinaNoteValues[gOcarinaSongNotes[i].notesIdx[j]]) {
                            j++;
                        } else {
                            k++;
                        }
                    }

                    if (j == gOcarinaSongNotes[i].len) {
                        D_801D8528 = i + 1;
                        sOcarinaInpEnabled = 0;
                        D_801D6FEC = 0;
                    }
                }
            }
        }
    }
}

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

        if (sCurOcarinaBtnVal != 0xFF && sCurOcarinaBtnPress & 0x10 && sRecordingState != 2) {
            sCurOcarinaBtnIdx += 0x80;
            sCurOcarinaBtnVal++;
        }

        if ((sCurOcarinaBtnVal != 0xFF) && (sCurOcarinaBtnPress & 0x2000) && (sRecordingState != 2)) {
            sCurOcarinaBtnIdx += 0x40;
            sCurOcarinaBtnVal--;
        }

        if (sRecordingState != 2) {
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
            Audio_StopSfxById(NA_SE_OC_OCARINA);
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
    Audio_StopSfxById(0x5800);

    if (gAudioSpecId != 0xC) {
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
// s32?
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
                    Audio_StopSfxById(NA_SE_OC_OCARINA);
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
                    Audio_StopSfxById(NA_SE_OC_OCARINA);
                }
            }
            sPlaybackNotePos++;
        }
    }
}

// Probably OoT func_800EDD68
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019C8D8.s")

/*
char* D_801E0C14 = "key step is too long !!! %d:%d>%d\n";
char* D_801E0C38 = "You played %d Melody !!! (last step:%d)\n";
char* D_801E0C64 = "pass 0\n";
char* D_801E0C6C = "pass 1\n";
char* D_801E0C74 = "pass 2\n";
char* D_801E0C7C = "last key is bad !!! %d %d %02X %02X\n";
char* D_801E0CA4 = "last key step is too short !!! %d:%d %d<%d\n";
char* D_801E0CD0 = "check is over!!! %d %d %d\n";
*/
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/D_801E0C14.s")

// Likely OoT func_800EE170
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019CD08.s")

// OoT func_800EE29C
void Audio_OcaUpdateRecordingStaff(void) {
    sRecordingStaff.state = sRecordingState;
    sRecordingStaff.pos = sStaffPlayingPos;
    if (sRecordingState == 0xFF) {
        sRecordingState = 0;
    }
}

// OoT func_800EE2D4
void Audio_OcaUpdatePlayingStaff(void) {
    if (sCurOcarinaBtnIdx != 0xFF) {
        sPlayingStaff.noteIdx = sCurOcarinaBtnIdx & 0x3F;
    }
    sPlayingStaff.state = func_8019AFE8();
    sPlayingStaff.pos = sStaffPlayingPos;
}

// OoT func_800EE318
void Audio_OcaUpdateDisplayedStaff(void) {
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
OcarinaStaff* Audio_OcaGetRecordingStaff(void) {
    return &sRecordingStaff;
}

OcarinaStaff* Audio_OcaGetPlayStaff(void) {
    if (sPlayingStaff.state < 0xFE) {
        D_801D6FEC = 0;
    }
    return &sPlayingStaff;
}

OcarinaStaff* Audio_OcaGetDisplayStaff(void) {
    return &sDisplayedStaff;
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019CFA8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019D134.s")

// Gakufu
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019D26C.s")
// s32 func_8019D134(?, ?); // extern
// extern s32 D_801D701C;
// extern ? sOcarinaNoteValues;
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
//                 temp_t1 = *(&sOcarinaNoteValues + ((Audio_NextRandom() % 5) & 0xFF));
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



// func_800EE57C
void func_8019D488(u8 minigameIdx) {
    u8 i;

    if (minigameIdx > 2) {
        minigameIdx = 2;
    }
    sOcaMinigameAppendPos = 0;
    sOcaMinigameEndPos = sOcaMinigameNoteCnts[minigameIdx];

    for (i = 0; i < 3; i++) {
        func_8019D4F8();
    }
}

// OoT func_800EE5EC
#define OCA_MEMORYGAME_IDX 14
s32 func_8019D4F8(void) {
    u32 rnd;
    u8 rndNote;

    if (sOcaMinigameAppendPos == sOcaMinigameEndPos) {
        return 1;
    }

    rnd = Audio_NextRandom();
    rndNote = sOcarinaNoteValues[rnd % 5];

    if (sOcarinaSongs[OCA_MEMORYGAME_IDX][sOcaMinigameAppendPos - 1].noteIdx == rndNote) {
        rndNote = sOcarinaNoteValues[(rnd + 1) % 5];
    }

    sOcarinaSongs[OCA_MEMORYGAME_IDX][sOcaMinigameAppendPos].noteIdx = rndNote;
    sOcarinaSongs[OCA_MEMORYGAME_IDX][sOcaMinigameAppendPos].unk_02 = 0x2D;
    sOcarinaSongs[OCA_MEMORYGAME_IDX][sOcaMinigameAppendPos].volume = 0x50;
    sOcarinaSongs[OCA_MEMORYGAME_IDX][sOcaMinigameAppendPos].vibrato = 0;
    sOcarinaSongs[OCA_MEMORYGAME_IDX][sOcaMinigameAppendPos].tone = 0;

    sOcaMinigameAppendPos++;

    sOcarinaSongs[OCA_MEMORYGAME_IDX][sOcaMinigameAppendPos].noteIdx = 0xFF;
    sOcarinaSongs[OCA_MEMORYGAME_IDX][sOcaMinigameAppendPos].unk_02 = 0;
    sOcarinaSongs[OCA_MEMORYGAME_IDX][sOcaMinigameAppendPos + 1].noteIdx = 0xFF;
    sOcarinaSongs[OCA_MEMORYGAME_IDX][sOcaMinigameAppendPos + 1].unk_02 = 0;
    if (1) {}
    return 0;
}

// OoT func_800EE6F4
// input update?
void func_8019D600(void) {
    D_801FD448 = gAudioContext.totalTaskCnt;
    if (D_801D6FB8 != 0) {
        if (sOcarinaInpEnabled == 1) {
            Audio_GetOcaInput();
        }
        if ((sPlaybackState == 0) && (sOcarinaInpEnabled == 1)) {
            func_8019BE98(0);
        }
        if (D_801D6FEC != 0) {
            if (D_801D6FEC & 0x40000000) {
                func_8019BC44();
            } else {
                func_8019B6B4();
            }
        }

        Audio_OcaPlayback();
        D_801D7018 = D_801FD448;

        if (sPlaybackState == 0) {
            func_8019CFA8();
        }

        if ((D_801D6FEC != 0) && (sPrevOcarinaNoteVal != sCurOcarinaBtnVal)) {
            D_801D8538 = 1;
        }

        sPrevOcarinaNoteVal = sCurOcarinaBtnVal;

        if (D_801FD460 != 0) {
            D_801FD460--;
            if (D_801FD460 == 0) {
                func_8019C300(0);
            }
        }

    }

    Audio_OcaUpdatePlayingStaff();
    Audio_OcaUpdateDisplayedStaff();
    Audio_OcaUpdateRecordingStaff();
}

// Is matching: need to migrate data over
#ifdef NON_MATCHING
void func_8019D758(void) {
    static u8 D_801D8B20 = 0;
    static u8 D_801D8B24 = 1;
    static u16 D_801D8B28 = 1200;

    switch (D_801D8B20) {
        case 0:
            if (D_801D8B28-- == 0) {
                if (D_801D8B24 < 7) {
                    D_801D8B20++;
                } else {
                    D_801D8B20 = 3;
                    func_8019C300(0);
                }
                D_801D8B28 = 1200;
            }
            break;
        case 1:
            Audio_SetSoundBanksMute(0);
            func_8019C300(D_801D8B24);
            func_8019C398(0x19, 1);
            D_801D8B24++;
            D_801D8B20++;
            break;
        case 2:
            if (Audio_OcaGetDisplayStaff()->state == 0) {
                D_801D8B20 = 0;
            }
            break;
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019D758.s")
#endif

// New to MM
void func_8019D864(void) {
    if (D_801D8B2C && gAudioContext.seqPlayers[1].enabled && ((u8)gAudioContext.seqPlayers[1].unk_158[0] == 0xFF)) {
        gAudioContext.seqPlayers[1].seqData = D_801D88B8;
        D_801D8B2C = false;
    }
}

// New to MM
void func_8019D8B4(void) {
    D_801D8B2C = true;
    Audio_QueueSeqCmd(0x1000000 | NA_BGM_DUNGEON_APPEAR);
}

// New to MM
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019D8E4.s")

// New to MM
void func_8019DF28(void) {
    if (D_801D6FB8 != 0) {
        Audio_QueueSeqCmd(0x82010D00);
        D_801D6FB8 = 0;
        func_8019C1D0();
    }
}

// OoT func_800EE930
void func_8019DF64(void) {
    u8 i;

    sPlayingStaff.noteIdx = 0xFF;
    sPlayingStaff.state = 0xFF;
    sPlayingStaff.pos = 0;
    sDisplayedStaff.noteIdx = 0xFF;
    sDisplayedStaff.state = 0;
    sDisplayedStaff.pos = 0;
    sRecordingStaff.noteIdx = 0xFF;
    sRecordingStaff.state = 0xFF;
    sRecordingStaff.pos = 0;
    D_801FD460 = 0;
    D_801D8538 = 0;

    for (i = 0; i < 24; i++) {
        D_801FD518[i] = 0xFF;
        D_801FD530[i] = 0;
    }

    D_801D8534 = 0;
}

// AudioDebug Remnant
void func_8019DFF8(s32 arg0) {
}

// AudioDebug Remnant
void func_8019E000(s32 arg0, s32 arg1) {
}

// AudioDebug Remnant
void func_8019E00C(void) {
}

void Audio_StepFreqLerp(FreqLerp* lerp); // extern
void func_8019D600(void); // extern
void func_8019D864(void); // extern
void func_8019F300(void); // extern
void func_8019FEDC(void); // extern
void func_801A046C(void); // extern
void func_801A1290(void); // extern
void func_801A1904(void); // extern
void func_801A1E0C(void); // extern
void func_801A2090(void); // extern
void func_801A2778(void); // extern
void func_801A312C(void); // extern
void func_801A3AC0(void); // extern
void func_801A44C4(void); // extern
void func_801A5118(void); // extern
void func_801A8D5C(void); // extern
s32 func_801A9768(void); // extern
s32 func_801A982C(void); // extern

// OoT func_800F3054
void func_8019E014(void) {
    if ((func_801A9768() == 0) && (func_801A982C() == 0)) {
        func_8019D864();
        func_8019D600();
        func_801A5118();
        Audio_StepFreqLerp(&sRiverFreqScaleLerp);
        Audio_StepFreqLerp(&sWaterfallFreqScaleLerp);
        func_801A046C();
        func_801A2778();
        func_801A312C();
        func_8019FEDC();
        func_801A1E0C();
        func_801A1904();
        func_801A2090();
        func_801A3AC0();
        func_801A1290();
        func_801A44C4();
        Audio_ProcessSoundRequests();
        Audio_ProcessSeqCmds();
        func_801A787C();
        func_801A8D5C();
        func_8019F300();
        Audio_ScheduleProcessCmds();
    }
}

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

// OoT func_800F3188
f32 func_8019E14C(u8 bankIdx, u8 entryIdx) {
    SoundBankEntry* bankEntry = &gSoundBanks[bankIdx][entryIdx];
    f32 temp_f14;
    f32 phi_f14;
    f32 phi_f0;
    f32 phi_f12;
    f32 ret;
    s32 pad[3];

    if (bankEntry->sfxParams & 0x2000) {
        return 1.0f;
    }

    if (bankEntry->dist > 10000.0f) {
        ret = 0.0f;
    } else {
        if (bankEntry->sfxParams & 4) {
            phi_f12 = 0.35f;
            phi_f14 = 0.65f;
        } else {
            phi_f12 = 0.81f;
            phi_f14 = 0.19f;
        }
        switch (bankEntry->sfxParams & 7) {
            case 0:
                phi_f0 = 500.0f;
                break;
            case 1:
                phi_f0 = 666.6667f;
                break;
            case 2:
                phi_f0 = 952.381f;
                break;
            case 3:
                phi_f0 = 3846.154f;
                break;
            case 4:
                phi_f0 = 1000.0f;
                break;
            case 5:
                phi_f0 = 666.6667f;
                break;
            case 6:
                phi_f0 = 500.0f;
                break;
            case 7:
                phi_f0 = 400.0f;
                break;
        }

        temp_f14 = phi_f0 / 5.0f;

        if (bankEntry->dist < temp_f14) {
            ret = 1.0f;
        } else if (bankEntry->dist < phi_f0) {
            ret = ((((phi_f0 - temp_f14) - (bankEntry->dist - temp_f14)) / (phi_f0 - temp_f14)) * phi_f14) + phi_f12;
        } else {
            ret = (1.0f - ((bankEntry->dist - phi_f0) / (10000.0f - phi_f0))) * phi_f12;
        }
        ret = SQ(ret);
    }

    return ret;
}

s8 Audio_ComputeSoundReverb(u8 bankIdx, u8 entryIdx, u8 channelIdx) {
    s8 distAdd = 0;
    s32 scriptAdd = 0;
    SoundBankEntry* entry = &gSoundBanks[bankIdx][entryIdx];
    s32 reverb;

    if (!(entry->sfxParams & 0x1000)) {
        if (entry->dist < 2500.0f) {
            distAdd = *entry->posZ > 0.0f ? (entry->dist / 2500.0f) * 70.0f : (entry->dist / 2500.0f) * 91.0f;
        } else {
            distAdd = 70;
        }
    }

    if (IS_SEQUENCE_CHANNEL_VALID(gAudioContext.seqPlayers[2].channels[channelIdx])) {
        scriptAdd = gAudioContext.seqPlayers[2].channels[channelIdx]->soundScriptIO[1];
        if (gAudioContext.seqPlayers[2].channels[channelIdx]->soundScriptIO[1] < 0) {
            scriptAdd = 0;
        }
    }

    reverb = (*entry->reverbAdd & 0x7F) + distAdd;

    if (entry->state != 2) {
        reverb += scriptAdd;
    }

    if ((bankIdx != BANK_OCARINA) || !((entry->sfxId & 0x3FF) < 2)) {
        reverb += sAudioEnvReverb + (sAudioCodeReverb & 0x3F) + sSpecReverb;
    }

    if (reverb > 0x7F) {
        reverb = 0x7F;
    }

    return reverb;
}

// Matches, but breaks rodata in unreferenced strings in D_801E0C14.s
#ifdef NON_MATCHING
s8 Audio_ComputeSoundPanSigned(f32 x, f32 z, u8 arg2) {
    f32 absX;
    f32 absZ;
    f32 pan;

    if (x < 0) {
        absX = -x;
    } else {
        absX = x;
    }
    if (z < 0) {
        absZ = -z;
    } else {
        absZ = z;
    }

    if (absX > 8000.0f) {
        absX = 8000.0f;
    }

    if (absZ > 8000.0f) {
        absZ = 8000.0f;
    }

    if ((x == 0.0f) && (z == 0.0f)) {
        pan = 0.5f;
    } else if (absZ <= absX) {
        pan = (16000.0f - absX) / (4.5f * (16000.0f - absZ));
        if (x >= 0.0f) {
            pan = 1.0f - pan;
        }
    } else {
        pan = (x / (5.0769234f * absZ)) + 0.5f;
    }

    if (absZ < 50.0f) {
        if (absX < 50.0f) {
            pan = ((pan - 0.5f) * SQ(absX / 50.0f)) + 0.5f;
        }
    }
    return (s8)((pan * 127.0f) + 0.5f);
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/Audio_ComputeSoundPanSigned.s")
#endif

f32 Audio_ComputeSoundFreqScale(u8 bankIdx, u8 entryIdx) {
    s32 phi_v0 = 0;
    SoundBankEntry* entry = &gSoundBanks[bankIdx][entryIdx];
    f32 unk1C;
    f32 freq = 1.0f;

    if (entry->sfxParams & 0x4000) {
        freq = 1.0f - ((gAudioContext.audioRandom & 0xF) / 192.0f);
    }

    switch (bankIdx) {
        case BANK_VOICE:
            if ((entry->sfxId & 0xFF) < 0x40 && sAudioBaseFilter2 != 0) {
                phi_v0 = 1;
            } else if ((entry->sfxId & 0xFF) >= 0x40 && sAudioExtraFilter2 != 0) {
                phi_v0 = 1;
            }
            break;
        case BANK_PLAYER:
        case BANK_ITEM:
                
            if (sAudioBaseFilter2 != 0) {
                phi_v0 = 1;
            }
            break;
        case BANK_ENV:
        case BANK_ENEMY:
            if (((*entry->reverbAdd & 0x80) != 0) | (sAudioExtraFilter2 != 0)) {
                phi_v0 = 1;
            }
            break;
        case BANK_SYSTEM:
        case BANK_OCARINA:
            break;
    }

    if (phi_v0 == 1) {
        if (!(entry->sfxParams & 0x800)) {
            freq *= (1.0293 - ((gAudioContext.audioRandom & 0xF) / 144.0f));
        }
    }

    unk1C = entry->dist;
    if (!(entry->sfxParams & 0x2000)) {
        if (!(entry->sfxParams & 0x8000)) {
            if (unk1C >= 10000.0f) {
                freq += 0.2f;
            } else {
                freq += (0.2f * (unk1C / 10000.0f));
            }
        }
    }

    if (entry->sfxParams & 0xC0) {
        freq += (entry->unk_2F / 192.0f);
    }

    return freq;
}

// OoT func_800F37B8
#ifdef NON_EQUIVALENT
u8 func_8019E864(f32 behindScreenZ, SoundBankEntry* arg1, s8 arg2) {
    s8 phi_v0;
    u8 phi_v1;
    f32 phi_f0;
    f32 phi_f12;

    // Remnant of OoT
    if (*arg1->posZ < behindScreenZ) {
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

// Possibly func_800F3ED4/Audio_ResetSfxChannelState
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
    Audio_StopSfxById(NA_SE_SY_MESSAGE_PASS);
}

void func_8019F230(void) {
    play_sound(NA_SE_SY_CANCEL);
    Audio_StopSfxById(NA_SE_SY_MESSAGE_PASS);
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
// ? func_8019FA18(Vec3f*, u16, ?, ?*); // extern
// extern ? D_801D6650;

// void func_8019FC20(Vec3f* arg0, u16 arg1) {
//     u8 sp1F;
//     u8 phi_v1;

//     phi_v1 = 0;
//     if (arg1 != 0x2173) {
//         if (arg1 == 0x2174) {
//             sp1F = 0;
//             phi_v1 = sp1F;
//             if (Audio_IsSfxPlaying(NA_SE_EV_BIG_WATER_WHEEL_LR) == 0) {
//                 goto block_5;
//             }
//         }
//     } else {
//         sp1F = 0;
//         if (Audio_IsSfxPlaying(NA_SE_EV_BIG_WATER_WHEEL_RR) == 0) {
// block_5:
//             phi_v1 = 1;
//         }
//     }
//     if (phi_v1 != 0) {
//         func_801A0654(arg0, arg1, 0);
//         func_8019FA18(arg0, arg1, 0x3F800000, &D_801D6650);
//     }
// }


// OoT func_800F4414
void func_8019FCB8(Vec3f* pos, u16 sfxId, f32 arg2) {
    D_801D6658--;
    if (D_801D6658 == 0) {
        Audio_PlaySoundGeneral(sfxId, pos, 4, &D_801FD288, &D_801DB4B0, &D_801DB4B8);

        if (arg2 > 2.0f) {
            arg2 = 2.0f;
        }
        D_801D6658 = (s8)((D_801D6660 - D_801D665C) * (1.0f - arg2)) + D_801D6660;
    }
}

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


void func_8019FE74(f32*, f32, u16);
extern u16 D_801D66A4;
// extern f32* D_801FD294;
// extern f32 D_801FD298;
// extern f32 D_801FD29C;
#ifdef NON_EQUIVALENT
void func_8019FE74(f32* arg0, f32 arg1, s16 arg2) {
    u16 temp_t7;

    D_801FD298 = arg1;
    D_801FD294 = arg0;
    D_801D66A4 = arg2;
    
    temp_t7 = arg2;

    D_801FD29C = (*arg0 - D_801FD298) / temp_t7;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019FE74.s")
#endif

#ifdef NON_EQUIVALENT
extern f32* D_801FD294;
extern f32 D_801FD298;
extern f32 D_801FD29C;

void func_8019FEDC(void) {
    u16 temp_t6;

    temp_t6 = D_801D66A4;
    temp_t6--;
    if (temp_t6 != 0) {
        D_801D66A4 = temp_t6;
        if (D_801D66A4 == 0) {
            *D_801FD294 = D_801FD298;
        } else {
            *D_801FD294 -= D_801FD29C;
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019FEDC.s")
#endif

// OoT func_800F45D0
void func_8019FF38(f32 arg0) {
    func_8019FCB8(&D_801DB4A4, NA_SE_IT_FISHING_REEL_SLOW - SFX_FLAG, arg0);
    func_8019FAD8(&D_801DB4A4, 0, (0.15f * arg0) + 1.4f);
}

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

void func_801A0204(s8 arg0) {
    Audio_QueueCmdS8(MK_CMD(0x46,0, 0, 2), (u8)arg0);
}

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
        phi_s1 += gChannelsPerBank[gSfxChannelLayout][i];
    }

    for (i = 0; i < gChannelsPerBank[gSfxChannelLayout][bank]; i++) {
        Audio_QueueCmdS8(_SHIFTL(6, 24, 8) | _SHIFTL(2, 16, 8) | _SHIFTL(phi_s1++, 8, 8) | _SHIFTL(6, 0, 8), arg2);
    }

    Audio_PlaySoundGeneral(sfxId, pos, 4, &D_801DB4B0, &D_801DB4B0, &D_801DB4B8);
}

// OoT func_800F4E30
void func_801A09D4(Vec3f* pos, f32 arg1) {
    f32 phi_f22;
    s8 phi_s4;
    u8 i;

    if (sSariaBgmPtr == NULL) {
        sSariaBgmPtr = pos;
        D_801D66F8 = arg1;
    } else if (pos != sSariaBgmPtr) {
        if (arg1 < D_801D66F8) {
            sSariaBgmPtr = pos;
            D_801D66F8 = arg1;
        }
    } else {
        D_801D66F8 = arg1;
    }

    if (sSariaBgmPtr->x > 100.0f) {
        phi_s4 = 0x7F;
    } else if (sSariaBgmPtr->x < -100.0f) {
        phi_s4 = 0;
    } else {
        phi_s4 = ((sSariaBgmPtr->x / 100.0f) * 64.0f) + 64.0f;
    }

    if (D_801D66F8 > 400.0f) {
        phi_f22 = 0.1f;
    } else if (D_801D66F8 < 120.0f) {
        phi_f22 = 1.0f;
    } else {
        phi_f22 = ((1.0f - ((D_801D66F8 - 120.0f) / 280.0f)) * 0.9f) + 0.1f;
    }

    for (i = 0; i < 0x10; i++) {
        if (i != 9) {
            Audio_SeqCmd6(0, 2, i, (127.0f * phi_f22));
            Audio_QueueCmdS8(0x03000000 | ((u8)((u32)i) << 8), phi_s4);
        }
    }
}

void Audio_ClearSariaBgm(void) {
    if (sSariaBgmPtr != NULL) {
        sSariaBgmPtr = NULL;
    }
}

void Audio_ClearSariaBgmAtPos(Vec3f* pos) {
    if (sSariaBgmPtr == pos) {
        sSariaBgmPtr = NULL;
    }
}

// OoT func_800F510C
void func_801A0CB0(s8 volSplit);
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A0CB0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A0E44.s")

// Easy
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A1290.s")

void func_801A1348(u8 arg0, Vec3f* arg1, s16 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6) {
    D_801FD3EC = arg0;
    D_801FD3F0.x = arg1->x;
    D_801FD3F0.y = arg1->y;
    D_801FD3F0.z = arg1->z;
    D_801FD3FC = arg2;
    D_801FD400 = arg3;
    D_801FD404 = arg4;
    D_801FD408 = arg5;
    D_801FD40C = arg6;
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A13BC.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A153C.s")

void func_801A17F4(Vec3f* arg0, s8 arg1) {
    s32 phi_v0 = false;
    s32 pad;

    if (D_801FD3D9 == 0) {
        phi_v0 = true;
    } else if (sqrtf(SQ(arg0->z) + ((SQ(arg0->x) * 0.25f) + (SQ(arg0->y) / 6.0f))) <
               sqrtf(SQ(D_801FD3E0.z) + ((SQ(D_801FD3E0.x) * 0.25f) + (SQ(D_801FD3E0.y) / 6.0f)))) {
        phi_v0 = true;
    }

    if (phi_v0) {
        D_801FD3E0.x = arg0->x;
        D_801FD3E0.y = arg0->y;
        D_801FD3E0.z = arg0->z;
        D_801FD3D9 = arg1;
        D_801FD3DA = 1;
    }
}

void func_801A13BC(u8, Vec3f*, s8, u8); // extern
void func_801A3038(void); // extern
void func_801A4A28(u8 arg0);

void func_801A1904(void) {
    if ((D_801FD3DA != 0) && (D_801FD3B0 == 0)) {
        if (D_801FD3D9 != 0) {
            func_801A13BC(1, &D_801FD3E0, D_801FD3D9, 0);
            if (func_801A8A50(1) == 0xFFFF) {
                func_801A3038();
            }
            if ((func_801A8A50(0) != 0xFFFF) && (func_801A8A50(4) == 0xFFFF)) {
                func_801A4A28(9);
            }
            sAudioCutsceneFlag = 1;
        } else {
            func_801A13BC(1, NULL, D_801FD3D9, 0);
            if (func_801A8A50(0) != 0xFFFF) {
                Audio_QueueSeqCmd(0x140000FF);
            }
            D_801FD3DA = 0;
            D_801D66FC = 0;
            sAudioCutsceneFlag = 0;
        }
        D_801FD3D9 = 0;
    }
}


void func_801A1A10(u8 arg0, u8 arg1) {
    Audio_QueueSeqCmd(((u32)arg0 << 0x18) | 0x100000FF | 0x140000);
    if ((arg0 == 3) && (func_801A8A50(0) != 0xFFFF)) {
        Audio_SetVolScale(0, 3, 0x7F, arg1);
        func_801A0CB0(0);
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A1A8C.s")

void func_801A1D44(Vec3f* vec, u8 arg1, f32 arg2) {
    if (gAudioSpecId != 0xC) {
        D_801FD410.x = vec->x;
        D_801FD410.y = vec->y;
        D_801FD410.z = vec->z;
        D_801FD430 = arg1;
        D_801FD42C = arg2;
        D_801FD431 |= 2;
        D_801FD433 = 4;
    }
}

void func_801A1DB8(Vec3f* vec, u8 arg1, f32 arg2) {
    D_801FD420.x = vec->x;
    D_801FD420.y = vec->y;
    D_801FD420.z = vec->z;
    D_801FD430 = arg1;
    D_801FD431 |= 1;
    D_801FD433 = 4;
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A1E0C.s")

void func_801A1F00(u8 arg0, u16 arg1) {
    if ((sAudioCutsceneFlag == 0) && (gAudioSpecId != 0xC)) {
        D_801FD420.x = D_801DB4A4.x;
        D_801FD420.y = D_801DB4A4.y;
        D_801FD420.z = D_801DB4A4.z;
        D_801FD42C = 10000.0f;
        D_801FD436 = 0x80;
        D_801FD430 = arg1;
        D_801FD435 = arg0;
    }
}

void func_801A1F88(void) {
    if (gAudioSpecId != 0xC) {
        D_801FD436 = 1;
        D_801FD430 = 0;
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A1FB4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A2090.s")



void func_801A3238(u8 seqIdx, u16 seqId, u8 fadeTimer, s8 arg3, u8 arg4);
void Audio_PlaySariaBgm(Vec3f* pos, u16 seqId, u16 distMax) {
    f32 absY;
    f32 dist;
    u8 vol;
    f32 prevDist;

    if (D_801FD3AA != 0) {
        D_801FD3AA--;
        return;
    }

    dist = sqrtf(SQ(pos->z) + (SQ(pos->x) + SQ(pos->y)));
    if (sSariaBgmPtr == NULL) {
        sSariaBgmPtr = pos;
        // func_800F5E18
        func_801A3238(3, seqId, 0, 7, 2);
    } else {
        prevDist = sqrtf(SQ(sSariaBgmPtr->z) + SQ(sSariaBgmPtr->x));
        if (dist < prevDist) {
            sSariaBgmPtr = pos;
        } else {
            dist = prevDist;
        }
    }

    if (pos->y < 0.0f) {
        absY = -pos->y;
    } else {
        absY = pos->y;
    }

    if ((distMax / 15.0f) < absY) {
        vol = 0;
    } else if (dist < distMax) {
        vol = (1.0f - (dist / distMax)) * 127.0f;
    } else {
        vol = 0;
    }

    if (seqId != 40) {
        func_801A0CB0(vol);
    }

    Audio_SetVolScale(3, 3, vol, 0);
    Audio_SetVolScale(0, 3, 0x7F - vol, 0);
}

void Audio_ClearSariaBgm2(void) {
    sSariaBgmPtr = NULL;
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A246C.s")

void func_801A257C(u16); // extern
void func_801A2670(u16); // extern


void func_801A2544(u16 arg0, u8 arg1) {
    func_801A257C(arg0);
    Audio_QueueSeqCmd(arg1 | 0x70040000);
}

void func_801A257C(u16 arg0) {
    if (arg0 != 1) {
        Audio_QueueSeqCmd(0x140000FF);
        func_801A2670(arg0);
        func_801A3238(0, arg0, 0, 0, 1);
        return;
    }
    func_801A4A28(8);
}

void func_801A25E4(u16 arg0, u8 arg1) {
    if (D_801FD2A0 != arg0) {
        if (arg0 == 1) {
            func_801A4A28(8);
        } else if ((arg0 != 0x57) || (D_801D66CC == 0xFFFF)) {
            func_801A2670(arg0);
            Audio_QueueSeqCmd(arg1 | 0x70040000);
        }
        D_801FD2A0 = arg0;
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A2670.s")

void func_801A2778(void) {
    u16 temp_v0 = func_801A8A50(0);

    if ((temp_v0 != 0xFFFF) && (D_801D6700[(u8)temp_v0 & 0xFF] & 0x10)) {
        if (D_801D66D0 != 0xC0) {
            D_801D66D0 = gAudioContext.seqPlayers[0].unk_158[3];
        } else {
            D_801D66D0 = 0;
        }
    }
}


void func_801A27E8(void) {
    if (func_801A8A50(0) != NA_BGM_FUSHA) {
        Audio_QueueSeqCmd(0x8000 | NA_BGM_FUSHA);
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A281C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A29D4.s")

void func_801A2BB8(u16 arg0) {
    Audio_SetVolScale(3, 3, 0x7F, 0);
    Audio_QueueSeqCmd(arg0 | 0x3000000);
    Audio_SetVolScale(0, 3, 0, 5);
    Audio_QueueSeqCmd(0xC380030A);
    Audio_QueueSeqCmd(0xC3900000);
}

void func_801A2C20(void) {
    Audio_QueueSeqCmd(0x130000FF);
}

void func_801A2C44(void) {
    if ((func_801A8A50(0) == NA_BGM_MINI_GAME_2) && (func_801A8ABC(0, 0xF0000000) != 0)) {
        Audio_QueueSeqCmd(0xB00500D2);
    }
}

void func_801A2C88(u16 arg0) {
    u8 a0 = arg0;

    if (D_801D6700[a0 & 0xFF] & 2) {
        func_801A3098(arg0);
    } else if (D_801D6700[a0 & 0xFF] & 4) {
        Audio_QueueSeqCmd(arg0 | 0x1000000);
    } else if (D_801D6700[a0 & 0xFF] & 0x80) {
        Audio_QueueSeqCmd(arg0 | 0x3000000);
    } else {
        func_801A3238(0, arg0, 0, 7, 0xFF);
        Audio_SetVolScale(0, 1, 0x7F, 0);
        Audio_SetVolScale(0, 3, 0x7F, 0);
        Audio_QueueSeqCmd(0x110000FF);
    }
}

void func_801A2D54(u16 arg0) {
    u8 a0 = arg0;

    if ((D_801D6700[a0 & 0xFF] & 2) != 0) {
        Audio_QueueSeqCmd(0x110000FF);
    } else if ((D_801D6700[a0 & 0xFF] & 4) != 0) {
        Audio_QueueSeqCmd(0x110000FF);
    } else if ((D_801D6700[a0 & 0xFF] & 0x80) != 0) {
        Audio_QueueSeqCmd(0x130000FF);
    } else {
        Audio_QueueSeqCmd(0x100000FF);
    }
}

s32 func_801A2DE0(u8 arg0) {
    u8 phi_a1 = 0;

    if (D_801D6700[arg0 & 0xFF] & 2) {
        phi_a1 = 1;
    } else if (D_801D6700[arg0 & 0xFF] & 4) {
        phi_a1 = 1;
    }

    if (arg0 == (func_801A8A50(phi_a1) & 0xFF)) {
        return true;
    } else {
        return false;
    }
}

void func_801A2E54(u16 arg0) {
    u16 phi_v1 = func_801A8A50(0);

    if (phi_v1 == 0xFFFF) {
        phi_v1 = 0;
    }

    if (phi_v1 != arg0) {
        func_801A32CC(3);
        if (!(D_801D6700[phi_v1] & 8)) {
            D_801D66CC = phi_v1;
        }
        Audio_QueueSeqCmd(arg0 + 0x8000);
    }
}

void func_801A2ED8(void) {
    if ((func_801A8A50(0) != 0xFFFF) && (D_801D6700[func_801A8A50(0) & 0xFF] & 8)) {
        if ((D_801D66CC == 0xFFFF) || (D_801D66CC == 0)) {
            Audio_QueueSeqCmd(0x100000FF);
        } else {
            if (D_801D66CC == 1) {
                D_801D66CC = D_801FD438;
            }
            Audio_QueueSeqCmd(D_801D66CC + 0x8000);
        }
        D_801D66CC = 0xFFFF;
    }
}

void func_801A2F88(u8 arg0) {
    u16 temp_v0 = func_801A8A50(0);

    if (temp_v0 != 1) {
        D_801D66CC = temp_v0;
    }
    func_801A4A28(arg0);
}

void func_801A2FC4(void) {
    if (D_801D66CC != 0xFFFF) {
        Audio_QueueSeqCmd(D_801D66CC + 0x8000);
    }
    D_801D66CC = 0xFFFF;
}

void func_801A3000(u16 arg0, u8 arg1) {
    func_801A3098(arg0);
    Audio_QueueSeqCmd(arg1 | 0x71070000);
}

void func_801A3038(void) {
    Audio_SetVolScale(0, 1, 0, 5);
    Audio_SetVolScale(3, 1, 0, 5);
    Audio_QueueSeqCmd(0xC180010A);
    Audio_QueueSeqCmd(0xC183010A);
    Audio_QueueSeqCmd(0xC1900000);
}

void func_801A3098(u16 arg0) {
    u16 sp26;
    u32 sp20;
    u8* sp1C;
    u8* ret;

    sp26 = func_801A8A50(1);
    sp1C = func_80193C04(sp26 & 0xFF, &sp20);
    ret = func_80193C04(arg0 & 0xFF, &sp20);
    if ((sp26 == 0xFFFF) || (*sp1C == *ret)) {
        D_801FD3AB = 1;
    } else {
        D_801FD3AB = 5;
        Audio_QueueSeqCmd(0x110000FF);
    }
    
    D_801FD3AC = arg0;
}

void func_801A312C(void) {
    if (D_801FD3AB != 0) {
        if ((D_801FD3AB != 5) && (func_801A8ABC(0x11000000, 0xFF000000) == 0)) {
            D_801FD3AB = 0;
        } else {
            D_801FD3AB--;
            if (D_801FD3AB == 0) {
                Audio_QueueCmdS32(0xE3000000, 0);
                Audio_QueueCmdS32(0xE3000000, 1);
                func_801A8A50(0);
                if (func_801A8A50(1) == 0xFFFF) {
                    func_801A3038();
                }
                Audio_QueueSeqCmd(D_801FD3AC | 0x1000000);
                Audio_QueueSeqCmd(0xA000FFFF);
            }
        }
    }
}

void func_801A31EC(u16 arg1, s8 arg2, u8 arg3) {
    Audio_QueueSeqCmd((arg2 << 0x10) | 0x71000000 | arg3);
    func_801A3098(arg1);
}

// OoT func_800F5E18
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A3238.s")

// OoT func_800F5E90
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A32CC.s")

// OoT func_801A3590
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A3590.s")

// OoT func_801A36F0
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A36F0.s")

u8 func_801A3950(u8 arg0, u8 arg1) {
    u8 ret;
    u8 temp_a2;
    SequenceChannel* channel;
    s8 temp_v0;

    ret = 0xFF;
    channel = gAudioContext.seqPlayers[arg0].channels[15];
    if (gAudioContext.seqPlayers[arg0].enabled && channel->enabled) {
        temp_v0 = channel->soundScriptIO[0];
        if (temp_v0 != -1) {
            temp_a2 = temp_v0;
            ret = temp_a2;
            if (arg1 == 1) {
                Audio_QueueSeqCmd((arg0 << 0x18) | 0x80000000 | 0xF00 | 0xFF);
            }
        }
    }
    return ret;
}

u8 func_801A39F8(void) {
    u8 sp27;
    s32 pad;
    SequenceChannel* channel;
    s8 temp_v0;

    sp27 = 0xFF;
    channel = gAudioContext.seqPlayers[3].channels[15];
    if (gAudioContext.seqPlayers[3].enabled) {
        if ((func_801A8A50(3) == 0x5A) && channel->enabled) {
            temp_v0 = channel->soundScriptIO[0];
            if (temp_v0 != -1) {
                sp27 = temp_v0;
                Audio_QueueCmdS8(0x6030F00, -1);
            }
        }
    }
    return sp27;
}

void func_801A3A7C(u8 arg0) {
    if ((arg0 == 0) && (D_801FD3B0 != 0)) {
        D_801FD3B0 = 1;
    } else {
        D_801FD3B0 = arg0 * 2;
    }
}

void func_801A3AC0(void) {
    if ((D_801FD3B0 != 0) && (D_801FD3B0 != 2)) {
        D_801FD3B0--;
    }
}

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

void func_801A4428(u8 arg0);

void Audio_SetCodeReverb(s8 reverb) {
    u8 temp_a0;

    if (reverb != 0) {
        if ((reverb & 0x40) != (sAudioCodeReverb & 0x40)) {
            temp_a0 = (reverb >> 6) + 1;
            func_801A4428(temp_a0);
        }
        sAudioCodeReverb = reverb & 0x7F;
    }
}

void func_801A3D54(void) {
    s32 phi_v0;

    phi_v0 = 0;
    if (D_801D66A8 == 2) {
        phi_v0 = 2;
    }

    func_801A0868(&D_801DB4A4, NA_SE_SY_SOUT_DEMO, phi_v0);
}

// OoT func_800F6700
void func_801A3D98(s8 arg0) {
    s8 sp1F;

    switch (arg0) {
        case 0:
            sp1F = 0;
            D_801D66A8 = 0;
            break;
        case 1:
            sp1F = 3;
            D_801D66A8 = 3;
            break;
        case 2:
            sp1F = 1;
            D_801D66A8 = 1;
            break;
        case 3:
            sp1F = 4;
            D_801D66A8 = 2;
            break;
    }

    Audio_SeqCmdE0(0, sp1F);
}

// OoT func_800F67A0
void Audio_SetBaseFilter(u8 filter) {
    if (sAudioBaseFilter != filter) {
        if (filter == 0) {
            Audio_StopSfxById(NA_SE_PL_IN_BUBBLE);
        } else if (sAudioBaseFilter == 0) {
            Audio_PlaySoundGeneral(NA_SE_PL_IN_BUBBLE, &D_801DB4A4, 4, &D_801DB4B0, &D_801DB4B0, &D_801DB4B8);
        }
    }
    sAudioBaseFilter = filter;
    sAudioBaseFilter2 = filter;
}

// OoT func_800F6828
void Audio_SetExtraFilter(u8 arg0) {
    u32 t;
    u8 i;

    sAudioExtraFilter2 = arg0;
    sAudioExtraFilter = arg0;
    if (D_80200140[4].unk_254 == 1) {
        for (i = 0; i < 16; i++) {
            t = i;
            // CHAN_UPD_SCRIPT_IO (seq player 4, all channels, slot 6)
            Audio_QueueCmdS8(((t & 0xFF) << 8) | 0x6040000 | 6, arg0);
        }
    }
}

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

void func_801A3FFC(u8 arg0) {
    D_801FD3AE = arg0;
}

// TODO: Possibly bad name (related to scene code)
void audio_setBGM(u8 bgmId) {
    if (D_801FD3AE == 0) {
        Audio_QueueSeqCmd(bgmId | 0xF0000000);
    } else {
        Audio_QueueSeqCmd(bgmId | 0xF0020000);
    }
}

// OoT func_800F6964
void func_801A4058(u16 arg0) {
    s32 skip;
    u8 i;

    if (D_801FD3AE == 0) {
        D_801FD3AF = 1;
        Audio_SeqCmd1(0, (arg0 * 3) / 2);
        Audio_SeqCmd1(1, (arg0 * 3) / 2);
        Audio_SeqCmd1(3, (arg0 * 3) / 2);
    }
    Audio_SeqCmd1(4, (arg0 * 3) / 2);

    for (i = 0; i < 0x10; i++) {
        skip = false;
        switch (i) {
            case 11:
            case 12:
                if (gAudioSpecId == 10) {}
                if (gAudioSpecId == 11) {}
                skip = true;
                break;
            case 13:
                skip = true;
                break;
        }

        if (!skip) {
            Audio_SeqCmd6(2, (u8)(arg0 >> 1) , i, 0);
        }
    }

}

void func_801A41C8(u16 arg0) {
    D_801FD3AE = 1;
    func_801A4058(arg0);
}

// OoT func_800F6AB0
void func_801A41F8(u16 arg0) {
    Audio_SeqCmd1(0, arg0);
    if (func_801A8A50(1) != 0xFFFF) {
        Audio_SeqCmd1(1, arg0);
    }
    Audio_SeqCmd1(3, arg0);
    Audio_SeqCmd1(4, arg0);
    Audio_SetVolScale(0, 3, 0x7F, 0);
    Audio_SetVolScale(0, 1, 0x7F, 0);
}

// OoT func_800F6B3C
void func_801A429C(void) {
    func_801A7B10(2, 0, 0xFF, 5);
}

void Audio_DisableAllSeq(void) {
    Audio_DisableSeq(0, 0);
    Audio_DisableSeq(1, 0);
    Audio_DisableSeq(2, 0);
    Audio_DisableSeq(3, 0);
    Audio_DisableSeq(4, 0);
    Audio_ScheduleProcessCmds();
}

// OoT func_800F6BB8
s8 func_801A4324(void) {
    return func_80194528();
}

// OoT func_800F6BDC
void func_801A4348(void) {
    Audio_DisableAllSeq();
    Audio_ScheduleProcessCmds();
    while (true) {
        if (!func_801A4324()) {
            return;
        }
    }
}

void func_801A4380(u8 arg0) {
    u8 i;

    if (D_801FD3AF == 0) {
        for (i = 0; i < 16; i++) {
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

void func_801A4428(u8 arg0) {
    u8 i;

    for (i = 0; i < 16; i++) {
        if (i != 0xD) {
            Audio_QueueCmdS8(0x11020000 | (((u32)i & 0xFF) << 8), arg0);
        }
    }
}

void Audio_PreNMI(void) {
    Audio_PreNMIInternal();
}

void func_801A44C4(void) {
    D_801FD2A0 = 0xFFFF;
}

// OoT func_800F6C34
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A44D4.s")

s32 func_801A46F8(void) {
    switch (func_801A3950(1, 1)) {
        case 0:
        case 8:
        case 16:
        case 24:
        case 32:
            return true;
        default:
            return false;
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A4748.s")

void func_801A479C(Vec3f* pos, u16 sfxId, u16 arg2) {
    func_8019FE1C(pos, sfxId, 0.0f);
    func_8019FE74(&D_801D6654, 1.0f, arg2);
}


s32 func_801A8ABC(u32 arg0, u32 arg1);
// OoT func_800F6D58
void func_801A47DC(u8 arg0, u8 arg1, u8 arg2) {
    u8 t;
    u8 temp_a0;
    u8 i;
    u8 a2 = arg2;

    // sAudioNatureFailed = 1;
    if ((D_80200140[4].unk_254 != 1) && func_801A8ABC(1, 0xF00000FF)) {
        return;
    }

    if (((arg0 << 8) + (u32)arg1) == 0x101) {
        if (func_801A8A50(3) != NA_BGM_ROMANI_RANCH) {
            D_801FD3A8 = 0;
        }
    }

    t = arg0 >> 4;
    temp_a0 = arg0 & 0xF;
    if (t == 0) {
        t = arg0 & 0xF;
    }

    for (i = t; i <= temp_a0; i++) {
        Audio_SeqCmd8(4, arg1, (u32)i, (u32)arg2);
    }
}

// OoT func_800F6E7C
void func_801A48E0(u16 arg0, u16 arg1);
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A48E0.s")

typedef struct {
    /* 0x0 */ u16 unk_00;
    /* 0x2 */ u16 unk_02;
    /* 0x4 */ u8 unk_04[100];
} D_801306DC_s; // size = 0x68

extern D_801306DC_s D_801D6794[];


// OoT func_800F6FB4
void func_801A4A28(u8 arg0) {
    u8 i = 0;
    u8 b0;
    u8 b1;
    u8 b2;

    if ((D_80200140[4].unk_254 == 0xFFFF) || ((D_801D6700[((u8)(D_80200140[4].unk_254 ^ 0)) & 0xFF] & 0x80) == 0)) {
        if (D_80200140[4].unk_254 != 1) {
            D_801FD438 = D_80200140[4].unk_254;
        }
        
        func_801A48E0(D_801D6794[arg0].unk_00, D_801D6794[arg0].unk_02);

        while ((D_801D6794[arg0].unk_04[i] != 0xFF) && (i < 100)) {
            // Probably a fake match, using Audio_SeqCmd8 doesn't work.
            b0 = D_801D6794[arg0].unk_04[i++];
            b1 = D_801D6794[arg0].unk_04[i++];
            b2 = D_801D6794[arg0].unk_04[i++];
            Audio_QueueSeqCmd(0x84000000 | (b1 << 0x10) | (b0 << 8) | b2);
        }

        Audio_SeqCmd8(4, 0x07, 0xD, D_801D66A8);
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A4B80.s")

// OoT func_800F70F8
void Audio_Init(void) {
    AudioLoad_Init(0, 0);
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A4C54.s")
// ? func_801A7B10(?, ?, ?, s32); // extern
// extern ? sSfxChannelState;
// extern ? func_8019F024;
// extern ? func_801A4B80;

// void func_801A4C54(s32 arg0) {
//     s32 temp_s0;
//     s32 phi_s0;

//     Audio_ScheduleProcessCmds();
//     func_801A7B10(2, 0, 0x70, arg0 & 0xFFFF & 0xFFFF);
//     phi_s0 = 0;
// loop_1:
//     Audio_QueueCmdS32(((phi_s0 & 0xFF) << 8) | 0x10020000, &sSfxChannelState + (phi_s0 * 0x10));
//     temp_s0 = (phi_s0 + 1) & 0xFF;
//     phi_s0 = temp_s0;
//     if (temp_s0 < 0x10) {
//         goto loop_1;
//     }
//     Audio_QueueCmdS32(0xE4000000, &func_8019F024);
//     Audio_QueueCmdS32(0xE4000001, &func_801A4B80);
//     return;
// }


// OoT func_800F711C
void Audio_InitSound(void) {
    func_801A44D4();
    func_8019DF64();
    func_8019F05C();
    func_801A9A74();
    Audio_ResetSounds();
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
    Audio_ResetSounds();
    func_801A4FD8();
    if (gAudioSpecId == 0xB) {
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
