#include "global.h"

// TODO: can these macros be shared between files? code_800F9280 seems to use
// versions without any casts...
#define Audio_DisableSeq(playerIdx, fadeOut) Audio_QueueCmdS32(0x83000000 | ((u8)playerIdx << 16), fadeOut)
#define Audio_StartSeq(playerIdx, fadeTimer, seqId) \
    Audio_QueueSeqCmd(0x00000000 | ((u8)playerIdx << 24) | ((u8)(fadeTimer) << 0x10) | (u16)seqId)
#define Audio_SeqCmd7(playerIdx, a, b) Audio_QueueSeqCmd(0x70000000 | ((u8)playerIdx << 0x18) | ((u8)a << 0x10) | (u8)(b))
#define Audio_SeqCmdC(playerIdx, a, b, c) \
    Audio_QueueSeqCmd(0xC0000000 | ((u8)playerIdx << 24) | ((u8)a << 16) | ((u8)b << 8) | ((u8)(c)))
#define Audio_SeqCmdA(playerIdx, a) Audio_QueueSeqCmd(0xA0000000 | ((u8)playerIdx << 24) | ((u16)(a)))
#define Audio_SeqCmd1(playerIdx, a) Audio_QueueSeqCmd(0x100000FF | ((u8)playerIdx << 24) | ((a) << 16))
#define Audio_SeqCmdB(playerIdx, a, b, c) \
    Audio_QueueSeqCmd(0xB0000000 | ((u8)playerIdx << 24) | ((u8)a << 16) | ((u8)b << 8) | ((u8)c))
#define Audio_SeqCmdB40(playerIdx, a, b) Audio_QueueSeqCmd(0xB0004000 | ((u8)playerIdx << 24) | ((u8)a << 16) | ((u8)b))
#define Audio_SeqCmd6(playerIdx, a, b, c) \
    Audio_QueueSeqCmd(0x60000000 | ((u32)playerIdx << 24) | ((u32)(a) << 16) | ((u32)b << 8) | ((u8)c))
#define Audio_SeqCmdE0(playerIdx, a) Audio_QueueSeqCmd(0xE0000000 | ((u8)playerIdx << 24) | (a))
#define Audio_SeqCmdE01(playerIdx, a) Audio_QueueSeqCmd(0xE0000100 | ((u8)playerIdx << 24) | ((u16)a))
#define Audio_SeqCmd8(playerIdx, a, b, c) \
    Audio_QueueSeqCmd(0x80000000 | (playerIdx << 24) | (a << 16) | (b << 8) | (c))
#define Audio_SeqCmdF(playerIdx, a) Audio_QueueSeqCmd(0xF0000000 | ((u8)playerIdx << 24) | ((u8)a))

typedef struct {
    /* 0x0 */ u16 unk_00;
    /* 0x2 */ u16 unk_02;
    /* 0x4 */ u8 unk_04[100];
} D_801306DC_s; // size = 0x68

extern D_801306DC_s D_801D6794[];

s32 AudioOcarina_MemoryGameGenerateNotes(void);
void func_801A1A10(u8 arg0, u8 arg1);
void func_801A257C(u16);
void func_801A2670(u16);
void func_801A7D04(s32, s32);
void func_801A7B10(u8 playerIdx, u8 seqId, u8 arg2, u16 fadeTimer);
s32 func_801A8ABC(u32 arg0, u32 arg1);

/*
// OoT's soundEffects from EnRiverSound
s16 D_801E0BD0[] = {
    NA_SE_EV_RIVER_STREAM - SFX_FLAG, 
    NA_SE_EV_WAVE - SFX_FLAG, 
    NA_SE_EV_WATER_WALL_BIG - SFX_FLAG, 
    NA_SE_EV_WATER_WALL - SFX_FLAG, 
    NA_SE_EV_MAGMA_LEVEL - SFX_FLAG, 
    NA_SE_EV_MAGMA_LEVEL_M - SFX_FLAG, 
    NA_SE_EV_MAGMA_LEVEL_L - SFX_FLAG, 
    NA_SE_EV_TORCH - SFX_FLAG, 
    NA_SE_EV_FOUNTAIN - SFX_FLAG, 
    NA_SE_EV_DRAIN - SFX_FLAG, 
    NA_SE_EV_CROWD - SFX_FLAG,
    NA_SE_EV_WATER_CONVECTION - SFX_FLAG, 
    NA_SE_EV_GORON_CHEER - SFX_FLAG, 
    NA_SE_EV_WATER_WALL_BIG_SILENT - SFX_FLAG, 
    NA_SE_EV_WATER_BUBBLE - SFX_FLAG, 
    NA_SE_EV_COW_CRY_LV - SFX_FLAG, 
    NA_SE_EV_MAKE_TURRET - SFX_FLAG, 
    NA_SE_EV_BOILED_WATER_S - SFX_FLAG, 
    NA_SE_EV_BOILED_WATER_L - SFX_FLAG, 
    NA_SE_EV_WAVE_S - SFX_FLAG, 
    NA_SE_EV_WAVE_S - SFX_FLAG, 
    NA_SE_EV_WAVE_S - SFX_FLAG,
};
*/
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/D_801E0BD0.s")

// ISMATCHING: will matching once sOcaInputStickRel is declared in-file
#ifdef NON_MATCHING
void AudioOcarina_ReadControllerInput(void) {
    Input inputs[4];
    Input* controller1 = &inputs[0];
    u32 ocaInputBtnPrev = sOcaInputBtnCur;

    Padmgr_GetInput2(inputs, 0);
    sOcaInputBtnCur = controller1->cur.button;
    sOcaInputBtnPrev = ocaInputBtnPrev;
    sOcaInputStickRel.x = controller1->rel.stick_x;
    sOcaInputStickRel.y = controller1->rel.stick_y;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/AudioOcarina_ReadControllerInput.s")
#endif

/**
 * Looks up the pitch frequency to bend the note by.
 * The note will bend up to a maximum of 2 semitones 
 * in each direction giving a total range of 4 semitones
 */
f32 AudioOcarina_BendPitchTwoSemitones(s8 bendIdx) {
    s8 scaledBendIdx;
    f32 bendFreq;

    if (bendIdx > 64) {
        scaledBendIdx = 127;
    } else if (bendIdx < -64) {
        scaledBendIdx = -128;
    } else if (bendIdx >= 0) {
        scaledBendIdx = (bendIdx * 127) / 64;
    } else {
        scaledBendIdx = (bendIdx * 128) / 64;
    }

    /**
     * index 128 is in the middle of the table and
     * contains the value 1.0f i.e. no bend
     * absolute indices above 128 will bend the pitch upwards
     * absolute indices below 128 will bend the pitch downwards
     */
    bendFreq = gBendPitchTwoSemitonesFrequencies[scaledBendIdx + 128];
    return bendFreq;
}

// OoT func_800ECAF0
u8 func_8019AFE8(void) {
    u8 ret;

    if (D_801D8528 != 0) {
        ret = D_801D8528 - 1;
        D_801D8528 = 0;
    } else if (sOcarinaFlags != 0) {
        ret = 0xFE;
    } else {
        ret = 0xFF;
    }

    return ret;
}

u8 AudioOcarina_MapNoteToButton(u8 noteIdx) {
    u8 buttonIdx = sNoteToButtonMap[noteIdx & 0x3F];

    /**
     * Special case for BFlat4:
     * CRIGHT and CLEFT are the only two notes that are 2 semitones apart
     * which are notes A4 and B4 respectively
     * BFlat4 is in the middle of those two and is the only note that can not 
     * be resolved between the two buttons without external information.
     * That information is stored as flags in noteIdx with the mask:
     * (noteIdx & 0xC0)
     */
    if (buttonIdx == (OCARINA_BTN_C_RIGHT + OCARINA_BTN_C_LEFT)) {
        if (noteIdx & 0x80) {
            return OCARINA_BTN_C_RIGHT;
        }
        return OCARINA_BTN_C_LEFT;
    }

    return buttonIdx;
}

void AudioOcarina_MapSongFromNotesToButtons(u8 noteSongIdx, u8 buttonSongIdx, u8 numButtons) {
    u8 buttonSongPos;
    u8 noteSongPos;
    u8 noteIdx;

    buttonSongPos = 0;
    noteSongPos = 0;

    while (buttonSongPos < numButtons && noteSongPos < 16) {
        noteIdx = sOcarinaSongNotes[noteSongIdx][noteSongPos++].noteIdx;
        if (noteIdx != NOTE_INVALID) {
            gOcarinaSongButtons[buttonSongIdx].buttonIdx[buttonSongPos++] = sNoteToButtonMap[noteIdx];
        }
    }

    gOcarinaSongButtons[buttonSongIdx].numButtons = numButtons;
}

void AudioOcarina_StartOcarina(u32 ocarinaFlags) {
    u8 i;

    if ((sOcarinaSongNotes[22][1].volume != 0xFF) && ((ocarinaFlags & 0x3FFF) == 0x3FFF)) {
        ocarinaFlags |= 0x400000;
    }

    if ((ocarinaFlags == 0xC0003FFF) && (sOcarinaSongNotes[22][1].volume != 0xFF)) {
        ocarinaFlags = 0xC0003FFF;
    }

    if ((ocarinaFlags == 0x3FFF) && (sOcarinaSongNotes[22][1].volume != 0xFF)) {
        ocarinaFlags = 0x3FFF;
    }

    if (ocarinaFlags != 0xFFFF) {
        sOcarinaFlags = 0x80000000 + (u32)ocarinaFlags;
        sOcarinaSongNoteStartIdx = 0;
        sOcarinaSongCount = 24;
        sOcarinaAvailSongs = ocarinaFlags & 0xFFFFFF;
        D_801D852C = 8;
        sOcarinaHasStartedSong = false;
        D_801D8528 = 0;
        sStaffPlayingPos = 0;
        sPlayingStaff.state = func_8019AFE8();
        sIsOcarinaInputEnabled = true;
        D_801D6FFC = 0;
        for (i = 0; i <= 20; i++) {
            for (sLearnSongPos[i] = 0; sOcarinaSongNotes[i][sLearnSongPos[i]].noteIdx == NOTE_INVALID;) {
                sLearnSongPos[i]++;
            }   

            D_801FD4A0[i] = 0;
            D_801FD4D0[i] = 0;
            D_801FD500[i] = 0;

            if (D_801D8530 != 0) {
                if (((1 << i) & ocarinaFlags) != 0) {
                    D_801FD518[i] = 0;
                } else {
                    D_801FD518[i] = 0xFF;
                }
            }
        }

        if (ocarinaFlags & 0x80000000) {
            D_801D852C = 0;
        }

        if (ocarinaFlags & 0x40000000) {
            sOcarinaSongAppendPos = 0;
        }

        if (ocarinaFlags & 0xC0400000) {
            AudioOcarina_MapSongFromNotesToButtons(22, 22, 8);
            return;
        }
    } else {
        sOcarinaFlags = 0;
        sIsOcarinaInputEnabled = false;
    }
}

void func_8019B378(void) {
    D_801D702C = sOcarinaSongAppendPos;
}

void AudioOcarina_SetupStartOcarina1(u32 ocarinaFlags) {
    D_801D8530 = 0;
    AudioOcarina_StartOcarina(ocarinaFlags);
    if (D_801D702C != 0) {
        sOcarinaSongAppendPos = D_801D702C;
        D_801D702C = 0;
    }
}

void AudioOcarina_SetupStartOcarina2(u32 ocarinaFlags, u8 arg1) {
    u8 i;
    u8 j;

    AudioOcarina_StartOcarina(ocarinaFlags);
    D_801D852C = 8;
    sStaffPlayingPos = arg1;

    for (i = 0; i < 21; i++) {
        for (j = 0; j < arg1;) {
            if (sOcarinaSongNotes[i][sLearnSongPos[i]].noteIdx != NOTE_INVALID) {
                j++;
            }
            sLearnSongPos[i]++;
        }

        if (sOcarinaSongNotes[i][sLearnSongPos[i]].noteIdx == NOTE_INVALID) {
            sLearnSongPos[i]++;
        }
    }
}

void AudioOcarina_SetupStartOcarina3(u32 ocarinaFlags) {
    u8 phi_v0;

    for (phi_v0 = 0; phi_v0 < 0x18; phi_v0++) {
        if (((1 << phi_v0) & ocarinaFlags) != 0) {
            D_801FD530[phi_v0] = 0;
        } else {
            D_801FD530[phi_v0] = 0xFF;
        }
    }
    
    D_801D8530 = 1;
    D_801D8534 = ocarinaFlags;
    AudioOcarina_StartOcarina(ocarinaFlags);
}

void AudioOcarina_SetupStartOcarina4(u32 ocarinaFlags) {
    D_801D8530 = 0;
    AudioOcarina_StartOcarina(ocarinaFlags);
}

u8 func_8019B568(void) {
    u32 temp_v0 = D_801D8534;
    u8 temp_v1 = 0;

    if ((temp_v0 % 2) == 0) {
        while (temp_v1 < 24) {
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

    if (temp_v0 < ARRAY_COUNT(D_801FD518)) {
        return D_801FD518[temp_v0];
    }
    return 0xFF;
}

u8 func_8019B5EC(void) {
    return D_801FD530[func_8019B568()];
}

// OoT func_800ECDBC
void func_8019B618(void) {
    if (sCurOcarinaNoteIdx != NOTE_INVALID && !sOcarinaHasStartedSong) {
        sOcarinaHasStartedSong = true;
        sLearnSongLastNote = NOTE_INVALID;
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

extern u8 D_801D701F[];

#ifdef NON_EQUIVALENT
void func_8019B6B4(void) {
    u8 sp5F;
    s8 sp5B;
    s32 sp40;
    s32 sp3C;
    OcarinaNote* temp_a0_3;
    OcarinaNote* temp_a0_4;
    OcarinaNote* temp_a1_2;
    OcarinaNote* temp_a2_2;
    OcarinaNote* temp_t1;
    s32 temp_a3;
    s32 temp_t2;
    s32 temp_v0_4;
    u16 temp_a1;
    u16 temp_v0_6;
    u16 temp_v0_7;
    u16 temp_v1_3;
    u16* temp_a3_2;
    u16* temp_t0;
    u32 temp_t5;
    u32* temp_v0_5;
    u8 temp_t3;
    s32 phi_a0;
    u8 phi_v0;
    u8 i;
    s32 phi_a1;
    s32 phi_a0_2;
    u16* phi_t0;
    OcarinaNote* phi_a0_3;
    u32 phi_t5_2;

    sp5F = 0;
    sp5B = 0;
    func_8019B618();
    if (sOcarinaHasStartedSong) {
        phi_a0 = sCurOcarinaBendIdx;
        if (sCurOcarinaBendIdx < 0) {
            phi_a0 = -sCurOcarinaBendIdx;
        }
        if (phi_a0 > 20) {
            sOcarinaFlags = 0;

            for (i = 0; i < 24; i++) {
                D_801FD518[i] = 4;
            }
            sIsOcarinaInputEnabled = false;
            return;
        }

        sp40 = sCurOcarinaNoteIdx;
        if ((sCurOcarinaNoteIdx == sPrevOcarinaNoteIdx) || (sCurOcarinaNoteIdx == NOTE_INVALID)) {
            sp5F = 1;
        } else {
            func_8019B654();
            D_801D701F[sOcarinaSongAppendPos] = sCurOcarinaNoteIdx;
            sp40 = sCurOcarinaNoteIdx;
        }

        i = sOcarinaSongNoteStartIdx;
        if (sOcarinaSongNoteStartIdx < sOcarinaSongCount) {
            sp3C = sOcarinaSongCount;
loop_13:
            temp_a3 = 1 << i;
            if ((sOcarinaAvailSongs & temp_a3) != 0) {
                if (D_801D8530 != 0) {
                    D_801FD4A0[i] += D_801FD448 - D_801D7018;
                    phi_a1 = D_801FD4D0[i];
                } else {
                    // temp_a1 = *&D_801FD4D0[i];
                    D_801FD4A0[i] = D_801FD4D0[i] + 9;
                    phi_a1 = D_801FD4D0[i];
                }
                if (sp5F != 0) {
                    if ((D_801FD4A0[i] >= 9) && (sLearnSongLastNote != D_801FD500[i])) {
                        sOcarinaAvailSongs ^= temp_a3;
                        if (D_801D8530 != 0) {
                            D_801FD518[i] = 1;
                        }
                    }

                    if ((D_801D8530 != 0) && ((phi_a1 + 9) < D_801FD4A0[i])) {
                        sOcarinaAvailSongs ^= temp_a3;
                        D_801FD518[i] = 3;
                        goto block_56;
                    }

                    if ((D_801FD4A0[i] >= (phi_a1 - 9)) && (D_801FD4A0[i] >= phi_a1) && (sOcarinaSongNotes[i][sLearnSongPos[i * 2]].length == 0) && (sLearnSongLastNote == D_801FD500[i])) {
                        D_801D8528 = i + 1;
                        sIsOcarinaInputEnabled = false;
                        sOcarinaFlags = 0;
                        if (D_801D8530 != 0) {
                            D_801FD518[i] = 5;
                        }
                    }
                } else {
                    if (D_801FD4A0[i] >= (phi_a1 - 9)) {
                        if (sLearnSongLastNote != 0xFF) {
                            if (sLearnSongLastNote == D_801FD500[i]) {
                                temp_v0_4 = phi_a1 - D_801FD4A0[i];
                                if (D_801D8530 != 0) {
                                    if (temp_v0_4 < 0) {
                                        phi_a0_2 = -temp_v0_4;
                                    } else {
                                        phi_a0_2 = temp_v0_4;
                                    }
                                    temp_v0_5 = &D_801FD530[i];
                                    *temp_v0_5 += phi_a0_2;
                                }
                                D_801FD4A0[i] = 0;
                            } else {
                                sOcarinaAvailSongs ^= temp_a3;
                                if (D_801D8530 != 0) {
                                    D_801FD518[i] = 1;
                                }
                            }
                        }
                        // temp_a3_2 = sLearnSongPos + i * 2;
                        temp_v0_6 = sLearnSongPos[i * 2];
                        temp_a2_2 = sOcarinaSongNotes[i];
                        temp_v1_3 = (temp_v0_6 + 1) & 0xFFFF;
                        temp_a1_2 = &temp_a2_2[temp_v0_6];
                        sLearnSongPos[i * 2] = temp_v1_3;
                        D_801FD4D0[i] = temp_a1_2->unk_02;
                        temp_t1 = &temp_a2_2[temp_v1_3];
                        D_801FD500[i] = temp_a1_2->noteIdx;
                        temp_t3 = temp_t1->noteIdx;
                        temp_a0_3 = temp_t1;
                        if ((temp_t3 == temp_a1_2->noteIdx) || ((temp_t3 == 0xFF) && (temp_t1->unk_02 != 0))) {
loop_45:
                            temp_v0_7 = sLearnSongPos[i * 2];
                            D_801FD4D0[i] += temp_a0_3->unk_02;
                            sLearnSongPos[i * 2]++;
                            temp_a0_3 = &temp_a2_2[sLearnSongPos[i * 2]];
                            temp_a0_3 = temp_a0_3 + 8;
                            if (temp_a0_3->noteIdx == temp_a2_2[temp_v0_7].noteIdx) {
                                goto loop_45;
                            }
                            if (temp_a0_3->noteIdx == 0xFF) {
                                if (temp_a0_3->unk_02 == 0) {

                                } else {
                                    goto loop_45;
                                }
                            }
                        }
                    } else if (D_801FD4A0[i] < 9) {
                        sp5B = -1;
                        D_801FD4A0[i] = 0;
                        sLearnSongLastNote = sp40;
                    } else {
                        if (sOcarinaSongNotes[i][sLearnSongPos[i * 2]].length == 0) {
                            D_801FD518[i] = 1;
                        } else if (D_801D8530 != 0) {
                            D_801FD518[i] = 2;
                        }
                        sOcarinaAvailSongs ^= temp_a3;
                    }
                }
            }
block_56:
            if ((sOcarinaAvailSongs == 0) && (sStaffPlayingPos >= D_801D852C)) {
                sIsOcarinaInputEnabled = false;
                if (D_801D8530 == 0) {
                    if (((sOcarinaFlags * 2) < 0) && (sp40 == sOcarinaSongNotes[i]->noteIdx)) {
                        D_801D6FFC = sOcarinaFlags;
                    }
                }
                sOcarinaFlags = 0;
                return;
            }
            i++;
            if (i >= sp3C) {
                goto block_65;
            }
            goto loop_13;
        }
block_65:
        if (sp5F == 0) {
            sLearnSongLastNote = sp40;
            sStaffPlayingPos += sp5B + 1;
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019B6B4.s")
#endif


// OoT func_800ED200
void func_8019BC44(void) {
    u32 noteIdx;
    u8 temp_v0_2;
    u8 i;
    u8 j;
    u8 k;

    if (CHECK_BTN_ANY(sOcaInputBtnCur, BTN_L) && CHECK_BTN_ANY(sOcaInputBtnCur, BTN_A | BTN_CRIGHT | BTN_CLEFT | BTN_CDOWN | BTN_CUP)) {
        AudioOcarina_SetupStartOcarina4(sOcarinaFlags);
        return;
    }

    func_8019B618();

    if (sOcarinaHasStartedSong) {
        temp_v0_2 = sStaffPlayingPos;
        if ((sPrevOcarinaNoteIdx != sCurOcarinaNoteIdx) && (sCurOcarinaNoteIdx != NOTE_INVALID)) {
            sStaffPlayingPos++;
            if (sStaffPlayingPos >= 9) {
                sStaffPlayingPos = 1;
            }

            func_8019B654();

            if ((ABS_ALT(sCurOcarinaBendIdx) > 20) && (temp_v0_2 != sStaffPlayingPos)) {
                sCurOcarinaSong[sOcarinaSongAppendPos - 1] = NOTE_INVALID;
            } else {
                sCurOcarinaSong[sOcarinaSongAppendPos - 1] = sCurOcarinaNoteIdx;
            }

            for (i = sOcarinaSongNoteStartIdx; i < sOcarinaSongCount; i++) {
                if ((u32)sOcarinaAvailSongs & (1 << i)) {
                    for (j = 0, k = 0;
                         j < gOcarinaSongButtons[i].numButtons && k == 0 && sOcarinaSongAppendPos >= gOcarinaSongButtons[i].numButtons;) {

                        noteIdx = sCurOcarinaSong[(sOcarinaSongAppendPos - gOcarinaSongButtons[i].numButtons) + j];
                        
                        if (noteIdx == sButtonToNoteMap[gOcarinaSongButtons[i].buttonIdx[j]]) {
                            j++;
                        } else {
                            k++;
                        }
                    }

                    if (j == gOcarinaSongButtons[i].numButtons) {
                        D_801D8528 = i + 1;
                        sIsOcarinaInputEnabled = false;
                        sOcarinaFlags = 0;
                    }
                }
            }
        }
    }
}

// OoT func_800ED458
void AudioOcarina_ProcessControllerInput(u8 arg0) {
    u32 ocarinaBtnsHeld;

    if (sOcarinaFlags != 0 && D_801D8538 != 0) {
        D_801D8538--;
        return;
    }

    if ((D_801FD454 == 0) ||
        ((D_801FD454 & (BTN_A | BTN_CRIGHT | BTN_CLEFT | BTN_CDOWN | BTN_CUP)) != (sOcaInputBtnCur & (BTN_A | BTN_CRIGHT | BTN_CLEFT | BTN_CDOWN | BTN_CUP)))) {
        
        D_801FD454 = 0;

        if (1) {}

        sCurOcarinaNoteIdx = NOTE_INVALID;
        sOcaStaffBtnCur = OCARINA_BTN_INVALID;

        ocarinaBtnsHeld = (sOcaInputBtnCur & (BTN_A | BTN_CRIGHT | BTN_CLEFT | BTN_CDOWN | BTN_CUP)) & (sOcaInputBtnPrev & (BTN_A | BTN_CRIGHT | BTN_CLEFT | BTN_CDOWN | BTN_CUP));
        
        if (!(sOcaInputBtnPress & ocarinaBtnsHeld) && (sOcaInputBtnCur != 0)) {
            sOcaInputBtnPress = sOcaInputBtnCur;
        } else {
            sOcaInputBtnPress &= ocarinaBtnsHeld;
        }

        if (sOcaInputBtnPress & BTN_A) {
            sCurOcarinaNoteIdx = NOTE_D4;
            sOcaStaffBtnCur = OCARINA_BTN_A;

        } else if (sOcaInputBtnPress & BTN_CDOWN) {
            sCurOcarinaNoteIdx = NOTE_F4;
            sOcaStaffBtnCur = OCARINA_BTN_C_DOWN;

        } else if (sOcaInputBtnPress & BTN_CRIGHT) {
            sCurOcarinaNoteIdx = NOTE_A4;
            sOcaStaffBtnCur = OCARINA_BTN_C_RIGHT;

        } else if (sOcaInputBtnPress & BTN_CLEFT) {
            sCurOcarinaNoteIdx = NOTE_B4;
            sOcaStaffBtnCur = OCARINA_BTN_C_LEFT;

        } else if (sOcaInputBtnPress & BTN_CUP) {
            sCurOcarinaNoteIdx = NOTE_D5;
            sOcaStaffBtnCur = OCARINA_BTN_C_UP;
        }

        if (sOcaInputBtnCur) {}

        // Pressing the R Button will raise the pitch by 1 semitone
        if ((sCurOcarinaNoteIdx != NOTE_INVALID) && (sOcaInputBtnCur & BTN_R) && (sRecordingState != 2)) {
            sOcaStaffBtnCur += 0x80; // Flag to resolve B Flat 4
            sCurOcarinaNoteIdx++; // Raise the pitch by 1 semitone
        }

        // Pressing the Z Button will lower the pitch by 1 semitone
        if ((sCurOcarinaNoteIdx != NOTE_INVALID) && (sOcaInputBtnCur & BTN_Z) && (sRecordingState != 2)) {
            sOcaStaffBtnCur += 0x40; // Flag to resolve B Flat 4
            sCurOcarinaNoteIdx--; // Lower the pitch by 1 semitone
        }

        if (sRecordingState != 2) {
            // Bend the pitch of the note based on y control stick
            sCurOcarinaBendIdx = sOcaInputStickRel.y;
            sCurOcarinaBendFreq = AudioOcarina_BendPitchTwoSemitones(sCurOcarinaBendIdx);

            // Add vibrato of the ocarina note based on the x control stick
            sCurOcarinaVibrato = ABS_ALT(sOcaInputStickRel.x) >> 2;
            Audio_QueueCmdS8(0x6020D06, sCurOcarinaVibrato);
        } else {
            sCurOcarinaBendIdx = 0;
            sCurOcarinaVibrato = 0;
            sCurOcarinaBendFreq = 1.0f;
        }

        if ((sCurOcarinaNoteIdx != NOTE_INVALID) && (sPrevOcarinaNoteIdx != sCurOcarinaNoteIdx)) {
            Audio_QueueCmdS8(0x6020D07, D_801D6FB8 - 1);
            Audio_QueueCmdS8(0x6020D05, sCurOcarinaNoteIdx);
            Audio_PlaySfxGeneral(NA_SE_OC_OCARINA, &gDefaultSfxPos, 4, &sCurOcarinaBendFreq, &D_801D6FD0, &gDefaultSfxReverb);
        } else if ((sPrevOcarinaNoteIdx != NOTE_INVALID) && (sCurOcarinaNoteIdx == NOTE_INVALID) && !arg0) {
            Audio_StopSfxById(NA_SE_OC_OCARINA);
        }
    }
}

void AudioOcarina_SetInputEnabled(u8 isEnabled) {
    sIsOcarinaInputEnabled = isEnabled;
}

void AudioOcarina_ResetOcarina(void) {
    sOcaInputBtnCur = 0;
    sOcaInputBtnPrev = 0;
    sOcaInputBtnPress = 0;
    
    D_801FD454 = 0xFFFF;

    AudioOcarina_ProcessControllerInput(0);
    Audio_StopSfxById(NA_SE_OC_OCARINA);

    if (gAudioSpecId != 0xC) {
        Audio_SetSoundBanksMute(0);
    }

    sPlaybackState = 0;
    sStaffPlaybackPos = 0;
    sIsOcarinaInputEnabled = false;

    sOcarinaFlags = 0;

    Audio_ClearBGMMute(0xD);
    func_801A4380(0x7F);
}

void AudioOcarina_SetupReadControllerInput1(void) {
    sOcaInputBtnCur = 0;
    sOcaInputBtnPrev = 0;
    sOcaInputBtnPress = 0;
    D_801FD454 = 0xFFFF;
    AudioOcarina_ProcessControllerInput(1);
    sPlaybackState = 0;
    sStaffPlaybackPos = 0;
    sIsOcarinaInputEnabled = false;
    sOcarinaFlags = 0;
    sOcaInputBtnCur = 0;
    AudioOcarina_ReadControllerInput();
    D_801FD454 = sOcaInputBtnCur;
}

void func_8019C2E4(u8 arg0, u8 arg1) {
    D_801FD460 = arg1;
    D_801FD461 = arg0;
}

// Gakufu
// s32?
u32 AudioOcarina_SetupReadControllerInput2(u8 arg0) {
    if (D_801D6FB8 != arg0 || arg0 == 1) {
        Audio_QueueSeqCmd((u32)arg0 | 0x82010D00);
        D_801D6FB8 = arg0;

        if (arg0 == 0) {
            AudioOcarina_ResetOcarina();
        } else {
            sOcaInputBtnCur = 0;
            AudioOcarina_ReadControllerInput();
            D_801FD454 = sOcaInputBtnCur;
            func_801A4380(0x40);
            Audio_QueueSeqCmdMute(0xD);
        }
    }
}

// OoT func_800ED93C
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_8019C398.s")

void AudioOcarina_PlaybackSong(void) {
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

            sNotePlaybackTimer = sPlaybackSong[sPlaybackNotePos].length;

            if (sPlaybackNotePos == 1) {
                sNotePlaybackTimer++;
            }

            if (sNotePlaybackTimer == 0) {
                sPlaybackState--;
                if (sPlaybackState != 0) {
                    sPlaybackNotePos = 0;
                    sStaffPlaybackPos = 0;
                    sDisplayedNoteValue = NOTE_INVALID;
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

            if (sNotePlaybackBend != sPlaybackSong[sPlaybackNotePos].bend) {
                sNotePlaybackBend = sPlaybackSong[sPlaybackNotePos].bend;
                sNormalizedNotePlaybackTone = AudioOcarina_BendPitchTwoSemitones(sNotePlaybackBend);
            }

            if ((sPlaybackSong[sPlaybackNotePos].volume == sPlaybackSong[sPlaybackNotePos - 1].volume &&
                 (sPlaybackSong[sPlaybackNotePos].vibrato == sPlaybackSong[sPlaybackNotePos - 1].vibrato) &&
                 (sPlaybackSong[sPlaybackNotePos].bend == sPlaybackSong[sPlaybackNotePos - 1].bend))) {
                sDisplayedNoteValue = 0xFE;
            }

            if (sDisplayedNoteValue != sPlaybackSong[sPlaybackNotePos].noteIdx) {
                u8 noteIdx = sPlaybackSong[sPlaybackNotePos].noteIdx;

                if (noteIdx == NOTE_BFLAT4) {
                    sDisplayedNoteValue = noteIdx + sPlaybackSong[sPlaybackNotePos].BFlat4Flag;
                } else {
                    sDisplayedNoteValue = noteIdx;
                }

                if (sDisplayedNoteValue != NOTE_INVALID) {
                    sStaffPlaybackPos++;
                    Audio_QueueCmdS8(0x6020D07, D_801D6FB8 - 1);
                    Audio_QueueCmdS8(0x6020D05, sDisplayedNoteValue & 0x3F);
                    Audio_PlaySfxGeneral(NA_SE_OC_OCARINA, &gDefaultSfxPos, 4, &sNormalizedNotePlaybackTone,
                                           &sNormalizedNotePlaybackVolume, &gDefaultSfxReverb);
                } else {
                    Audio_StopSfxById(NA_SE_OC_OCARINA);
                }
            }
            sPlaybackNotePos++;
        }
    }
}

// Probably OoT func_800EDD68
void func_8019C8D8(u8);
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

// OoT func_800EE170
void func_8019CD08(u8 arg0) {
    if (sRecordingState != arg0) {
        if (arg0) {
            D_801D8510 = D_801FD448;
            D_801D8514 = NOTE_INVALID;
            D_801D8518 = 0x57;
            D_801D851C = 0;
            D_801D8520 = 0;
            D_801D8524 = 0;
            sRecordSongPos = 0;
            sIsOcarinaInputEnabled = true;
            sStaffPlayingPos = 0;
            D_801FD590 = sPierresSong[1];
        } else {
            if (sRecordSongPos == 0) {
                sPierresSong[1] = D_801FD590;
            } else {
                if (sRecordingState == 2) {
                    sStaffPlayingPos = 1;
                }
                func_8019C8D8(1);
            }
            sIsOcarinaInputEnabled = false;
            sStaffPlayingPos = 0;
        }
        sRecordingState = arg0;
    }
}

// OoT func_800EE29C
void AudioOcarina_UpdateRecordingStaff(void) {
    sRecordingStaff.state = sRecordingState;
    sRecordingStaff.pos = sStaffPlayingPos;
    if (sRecordingState == 0xFF) {
        sRecordingState = 0;
    }
}

// OoT func_800EE2D4
void AudioOcarina_UpdatePlayingStaff(void) {
    if (sOcaStaffBtnCur != OCARINA_BTN_INVALID) {
        sPlayingStaff.buttonIdx = sOcaStaffBtnCur & 0x3F;
    }
    sPlayingStaff.state = func_8019AFE8();
    sPlayingStaff.pos = sStaffPlayingPos;
}

// OoT func_800EE318
void AudioOcarina_UpdateDisplayedStaff(void) {
    if ((sDisplayedNoteValue & 0x3F) < 0x10) {
        sDisplayedStaff.buttonIdx = AudioOcarina_MapNoteToButton(sDisplayedNoteValue);
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
OcarinaStaff* AudioOcarina_GetRecordingStaff(void) {
    return &sRecordingStaff;
}

OcarinaStaff* AudioOcarina_GetPlayingStaff(void) {
    if (sPlayingStaff.state < 0xFE) {
        sOcarinaFlags = 0;
    }
    return &sPlayingStaff;
}

OcarinaStaff* AudioOcarina_GetDisplayedStaff(void) {
    return &sDisplayedStaff;
}

void func_8019CFA8(void) {
    s32 phi_v0;

    if ((sRecordingState != 0) && ((D_801FD448 - D_801D8510) >= 3)) {
        phi_v0 = false;
        if (D_801D8514 != sCurOcarinaNoteIdx) {
            if (sCurOcarinaNoteIdx != NOTE_INVALID) {
                sRecordingStaff.buttonIdx = sOcaStaffBtnCur & 0x3F;
                sStaffPlayingPos += 1;
            } else if ((sRecordingState == 2) && (sStaffPlayingPos == 8)) {
                func_8019C8D8(1);
                return;
            }

            if (sStaffPlayingPos >= 9) {
                if (sRecordingState == 2) {
                    func_8019C8D8(1);
                    return;
                }
                sStaffPlayingPos = 1;
            }
            phi_v0 = true;
        } else if (D_801D8518 != D_801D6FD8) {
            phi_v0 = true;
        } else if (D_801D851C != sCurOcarinaVibrato) {
            if (sRecordingState != 2) {
                phi_v0 = true;
            }
        } else if ((D_801D8520 != sCurOcarinaBendIdx) && (sRecordingState != 2)) {
            phi_v0 = true;
        }

        if (phi_v0) {
            func_8019C8D8(0);
            D_801D8510 = D_801FD448;
        }
    }
}

/**
 * ChecksoGenerates the notes displayed on the Termina Field wall of musical notes
 */
s32 AudioOcarina_MusicWallValidateNotes(u8 songIdx, u8 maxSongIdx) {
    u8 i;
    u8 j;
    u8 k;

    for (i = 0; i < maxSongIdx; i++) {
        // D_801E0BFC is in rodata
        if (D_801E0BFC[i]) {
            for (j = 0; j < (9 - gOcarinaSongButtons[i].numButtons); j++) {
                for (k = 0; (k < gOcarinaSongButtons[i].numButtons) && ((k + j) < 8) &&
                            (gOcarinaSongButtons[i].buttonIdx[k] == gOcarinaSongButtons[songIdx].buttonIdx[(k + j)]);
                     k++) {
                    continue;
                }
                if (k == gOcarinaSongButtons[i].numButtons) {
                    // success: notes are accepted and used
                    return -1;
                }
            }
        }
    }

    // failure: retry and generate a new set of notes
    return 0;
}

/**
 * Generates the notes displayed on the Termina Field wall of musical notes
 */
void AudioOcarina_MusicWallGenerateNotes(void) {
    OcarinaNote* ocarinaNote;
    u8 randButton;
    u8 i;
    u8 j;

    do {
        i = 0;
        if (sOcarinaWallCounter++ % 2) {
            j = 0;

            for (; i < 8; i++) {
                randButton = Audio_NextRandom() % 5;
                sOcarinaSongNotes[23][j].noteIdx = sButtonToNoteMap[randButton];
                sOcarinaSongNotes[23][j].length = 19;
                sOcarinaSongNotes[23][j].volume = 80;
                sOcarinaSongNotes[23][j].vibrato = 0;
                sOcarinaSongNotes[23][j].bend = 0;
                j++;
                sOcarinaSongNotes[23][j].noteIdx = NOTE_INVALID;
                sOcarinaSongNotes[23][j].length = 3;
                sOcarinaSongNotes[23][j].volume = 0;
                sOcarinaSongNotes[23][j].vibrato = 0;
                sOcarinaSongNotes[23][j].bend = 0;
                j++;
            }
            
            sOcarinaSongNotes[23][j-2].length = 90;
            sOcarinaSongNotes[23][j-1].length = 22;
            sOcarinaSongNotes[23][j+1].noteIdx = NOTE_INVALID;
            sOcarinaSongNotes[23][j+1].length = 0;
            AudioOcarina_MapSongFromNotesToButtons(23, 23, 8);
        } else {
            j = Audio_NextRandom() % 9;
            ocarinaNote = sOoTOcarinaSongs[j];

            for (; ocarinaNote[i].length != 0; i++) {
                sOcarinaSongNotes[23][i] = ocarinaNote[i];
            }

            sOcarinaSongNotes[23][i].noteIdx = NOTE_INVALID;
            sOcarinaSongNotes[23][i].length = 0;
            AudioOcarina_MapSongFromNotesToButtons(23, 23, sOoTOcarinaSongsNumNotes[j]);
        }
    } while (AudioOcarina_MusicWallValidateNotes(23, 23) != 0);
}

// func_800EE57C
void AudioOcarina_MemoryGameSetNumNotes(u8 memoryGameIdx) {
    u8 i;

    if (memoryGameIdx > 2) {
        memoryGameIdx = 2;
    }

    sOcaMemoryGameAppendPos = 0;
    sOcaMemoryGameEndPos = sOcaMemoryGameNumNotes[memoryGameIdx];

    for (i = 0; i < 3; i++) {
        AudioOcarina_MemoryGameGenerateNotes();
    }
}

// OoT func_800EE5EC
// Remnant of OoT, ID 14 now represent Goron Lullaby Intro
#define OCA_SONGS_MEMORYGAME 14
s32 AudioOcarina_MemoryGameGenerateNotes(void) {
    u32 rnd;
    u8 rndNote;

    if (sOcaMemoryGameAppendPos == sOcaMemoryGameEndPos) {
        return 1;
    }

    rnd = Audio_NextRandom();
    rndNote = sButtonToNoteMap[rnd % 5];

    if (sOcarinaSongNotes[OCA_SONGS_MEMORYGAME][sOcaMemoryGameAppendPos - 1].noteIdx == rndNote) {
        rndNote = sButtonToNoteMap[(rnd + 1) % 5];
    }

    sOcarinaSongNotes[OCA_SONGS_MEMORYGAME][sOcaMemoryGameAppendPos].noteIdx = rndNote;
    sOcarinaSongNotes[OCA_SONGS_MEMORYGAME][sOcaMemoryGameAppendPos].length = 45;
    sOcarinaSongNotes[OCA_SONGS_MEMORYGAME][sOcaMemoryGameAppendPos].volume = 0x50;
    sOcarinaSongNotes[OCA_SONGS_MEMORYGAME][sOcaMemoryGameAppendPos].vibrato = 0;
    sOcarinaSongNotes[OCA_SONGS_MEMORYGAME][sOcaMemoryGameAppendPos].bend = 0;

    sOcaMemoryGameAppendPos++;

    sOcarinaSongNotes[OCA_SONGS_MEMORYGAME][sOcaMemoryGameAppendPos].noteIdx = NOTE_INVALID;
    sOcarinaSongNotes[OCA_SONGS_MEMORYGAME][sOcaMemoryGameAppendPos].length = 0;
    sOcarinaSongNotes[OCA_SONGS_MEMORYGAME][sOcaMemoryGameAppendPos + 1].noteIdx = NOTE_INVALID;
    sOcarinaSongNotes[OCA_SONGS_MEMORYGAME][sOcaMemoryGameAppendPos + 1].length = 0;
    if (1) {}
    return 0;
}

// OoT func_800EE6F4
// input update?
void AudioOcarina_Update(void) {
    D_801FD448 = gAudioContext.totalTaskCnt;
    if (D_801D6FB8 != 0) {
        if (sIsOcarinaInputEnabled == true) {
            AudioOcarina_ReadControllerInput();
        }

        if ((sPlaybackState == 0) && (sIsOcarinaInputEnabled == true)) {
            AudioOcarina_ProcessControllerInput(0);
        }

        if (sOcarinaFlags != 0) {
            if (sOcarinaFlags & 0x40000000) {
                func_8019BC44();
            } else {
                func_8019B6B4();
            }
        }

        AudioOcarina_PlaybackSong();
        D_801D7018 = D_801FD448;

        if (sPlaybackState == 0) {
            func_8019CFA8();
        }

        if ((sOcarinaFlags != 0) && (sPrevOcarinaNoteIdx != sCurOcarinaNoteIdx)) {
            D_801D8538 = 1;
        }

        sPrevOcarinaNoteIdx = sCurOcarinaNoteIdx;

        if (D_801FD460 != 0) {
            D_801FD460--;
            if (D_801FD460 == 0) {
                AudioOcarina_SetupReadControllerInput2(0);
            }
        }

    }

    AudioOcarina_UpdatePlayingStaff();
    AudioOcarina_UpdateDisplayedStaff();
    AudioOcarina_UpdateRecordingStaff();
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
                    AudioOcarina_SetupReadControllerInput2(0);
                }
                D_801D8B28 = 1200;
            }
            break;
        case 1:
            Audio_SetSoundBanksMute(0);
            AudioOcarina_SetupReadControllerInput2(D_801D8B24);
            func_8019C398(0x19, 1);
            D_801D8B24++;
            D_801D8B20++;
            break;
        case 2:
            if (AudioOcarina_GetDisplayedStaff()->state == 0) {
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
    if (D_801D8B2C && gAudioContext.seqPlayers[1].enabled && ((u8)gAudioContext.seqPlayers[1].soundScriptIO[0] == 0xFF)) {
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
        AudioOcarina_ResetOcarina();
    }
}

// OoT func_800EE930
void AudioOcarina_ResetStaff(void) {
    u8 i;

    sPlayingStaff.buttonIdx = OCARINA_BTN_INVALID;
    sPlayingStaff.state = 0xFF;
    sPlayingStaff.pos = 0;
    sDisplayedStaff.buttonIdx = OCARINA_BTN_INVALID;
    sDisplayedStaff.state = 0;
    sDisplayedStaff.pos = 0;
    sRecordingStaff.buttonIdx = OCARINA_BTN_INVALID;
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
void Audio_Update(void) {
    if ((func_801A9768() == 0) && (func_801A982C() == 0)) {
        func_8019D864();
        AudioOcarina_Update();
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
f32 Audio_ComputeSoundVolume(u8 bankId, u8 entryIdx) {
    SoundBankEntry* bankEntry = &gSoundBanks[bankId][entryIdx];
    f32 minDist;
    f32 phi_f14;
    f32 baseDist;
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
                baseDist = 500.0f;
                break;
            case 1:
                baseDist = 666.6667f;
                break;
            case 2:
                baseDist = 952.381f;
                break;
            case 3:
                baseDist = 3846.154f;
                break;
            case 4:
                baseDist = 1000.0f;
                break;
            case 5:
                baseDist = 666.6667f;
                break;
            case 6:
                baseDist = 500.0f;
                break;
            case 7:
                baseDist = 400.0f;
                break;
        }

        minDist = baseDist / 5.0f;

        if (bankEntry->dist < minDist) {
            ret = 1.0f;
        } else if (bankEntry->dist < baseDist) {
            ret = ((((baseDist - minDist) - (bankEntry->dist - minDist)) / (baseDist - minDist)) * phi_f14) + phi_f12;
        } else {
            ret = (1.0f - ((bankEntry->dist - baseDist) / (10000.0f - baseDist))) * phi_f12;
        }
        ret = SQ(ret);
    }

    return ret;
}

s8 Audio_ComputeSoundReverb(u8 bankId, u8 entryIdx, u8 channelIdx) {
    s8 distAdd = 0;
    s32 scriptAdd = 0;
    SoundBankEntry* entry = &gSoundBanks[bankId][entryIdx];
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

    if ((bankId != BANK_OCARINA) || !((entry->sfxId & 0x3FF) < 2)) {
        reverb += sAudioEnvReverb + (sAudioCodeReverb & 0x3F) + sSpecReverb;
    }

    if (reverb > 0x7F) {
        reverb = 0x7F;
    }

    return reverb;
}

// ISMATCHING: breaks rodata in unreferenced strings in D_801E0C14.s
#ifdef NON_MATCHING
s8 Audio_ComputeSoundPanSigned(f32 x, f32 z, u8 token) {
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

f32 Audio_ComputeSoundFreqScale(u8 bankId, u8 entryIdx) {
    s32 phi_v0 = 0;
    SoundBankEntry* entry = &gSoundBanks[bankId][entryIdx];
    f32 unk1C;
    f32 freq = 1.0f;

    if (entry->sfxParams & 0x4000) {
        freq = 1.0f - ((gAudioContext.audioRandom & 0xF) / 192.0f);
    }

    switch (bankId) {
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

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/Audio_SetSfxProperties.s")

u32 func_8019F024(u8 channelIdx, SequenceChannel* channel) {
    channel->stereo.asByte = sSfxChannelState[channelIdx].stereoBits;
    channel->freqScale = sSfxChannelState[channelIdx].freqScale;
    channel->changes.s.freqScale = true;
    return channelIdx;
}

// OoT func_800F3ED4
void Audio_ResetSfxChannelState(void) {
    SfxPlayerState* state;
    u8 i;

    for (i = 0; i < 16; i++) {
        state = &sSfxChannelState[i];
        state->vol = 1.0f;
        state->freqScale = 1.0f;
        state->reverb = 0;
        state->panSigned = 0x40;
        state->stereoBits = 0;
        state->filter = 0xFF;
        state->unk_0C = 0xFF;
        state->unk_0D = 0xFF;
    }


    sSfxChannelState[13].unk_0C = 0;
    D_801D66C0 = 0;
    sAudioCodeReverb = 0;
}

void Audio_PlaySfxById1(u16 sfxId) {
    Audio_PlaySfxGeneral(sfxId, &gDefaultSfxPos, 4, &gDefaultSfxFreq, &gDefaultSfxFreq, &gDefaultSfxReverb);
    if (sfxId == NA_SE_OC_TELOP_IMPACT) {
        func_801A32CC(0);
    } 
}

void Audio_PlaySfxById2(u16 sfxId) {
    Audio_PlaySfxGeneral(sfxId, &gDefaultSfxPos, 4, &gDefaultSfxFreq, &gDefaultSfxFreq, &gDefaultSfxReverb);
}

/**
 * Bends the pitch of the sfx by a little under two semitones and adds reverb
 */
void Audio_PlaySfxByPosAndIdWithLowFreqAndReverb(Vec3f* pos, u16 sfxId) {
    Audio_PlaySfxGeneral(sfxId, pos, 4, &gLoweredSfxFreq, &gDefaultSfxFreq, &sIncreasedSfxReverb);
}

void Audio_PlaySfxByPosAndId(Vec3f* pos, u16 sfxId) {
    Audio_PlaySfxGeneral(sfxId, pos, 4, &gDefaultSfxFreq, &gDefaultSfxFreq, &gDefaultSfxReverb);
}

void Audio_PlayMessageDecideSfx(void) {
    Audio_PlaySfxById1(NA_SE_SY_DECIDE);
    Audio_StopSfxById(NA_SE_SY_MESSAGE_PASS);
}

void Audio_PlayMessageCancelSfx(void) {
    Audio_PlaySfxById1(NA_SE_SY_CANCEL);
    Audio_StopSfxById(NA_SE_SY_MESSAGE_PASS);
}

Struct_D_801FD1F0* func_8019F258(f32* arg0) {
    Struct_D_801FD1F0* ret;
    u8 i = 0;
    u8 phi_v1 = 0xFF;
    
    for (; i < 8; i++) {
        ret = &D_801FD1F0[i];

        if ((ret->unk_0 == 0) && (phi_v1 == 0xFF)) {
            phi_v1 = i;
        }

        if (ret->unk_0 == arg0) {
            return ret;
        }
    }

    if (phi_v1 != 0xFF) {
        D_801FD250 |= 1 << phi_v1;
        ret = &D_801FD1F0[phi_v1];
        ret->unk_0 = arg0;
        return ret;
    }

    return NULL;
}

void func_8019F300(void) {
    SoundBankEntry* temp_t0;
    SoundBankEntry* temp_t1;
    s32 temp_a2;
    u8 temp_v1;
    u8 phi_a1;
    u8 phi_v0;
    u8 phi_v1;
    u8 phi_a3;

    s32 phi_a0;
    
    phi_v0 = 0;
    if (D_801FD250 != 0) {
        phi_a1 = D_801FD250;

        while (phi_a1 != 0) {
            
            phi_v1 = 2; 
            if ((phi_a1 & (1 << phi_v0))) {

                phi_a0 = 0;
                while ((phi_v1 < 4) && (phi_a0 == 0)){
                    phi_a3 = gSoundBanks[phi_v1]->next;

                    while (phi_a3 != 0xFF) {
                        temp_t1 = &gSoundBanks[phi_v1][phi_a3];
                        phi_a3 = 0xFF;
                        if (temp_t1->posX == D_801FD1F0[phi_v0].unk_0) {
                            phi_a0 = 1;
                        } else {
                            phi_a3 = temp_t1->next;
                        }
                    }
                    phi_v1++;
                }

                if (phi_a0 == 0) {
                    D_801FD250 ^= (1 << phi_v0);
                    D_801FD1F0[phi_v0].unk_0 = NULL;
                }

                phi_a1 ^= (1 << phi_v0);
            }

            phi_v0++;
        }
    }
}

void func_8019F420(Vec3f* pos, u16 sfxId) {
    if ((sfxId == NA_SE_EN_KONB_JUMP_OLD) || (sfxId == NA_SE_EN_KONB_SINK_OLD)) {
        Audio_PlaySfxGeneral(sfxId, pos, 4, &gDefaultSfxFreq, &gDefaultSfxFreq, &gDefaultSfxReverb);
    } else {

        Audio_PlaySfxGeneral(sfxId, pos, 4, &gDefaultSfxFreq, &gDefaultSfxFreq, &D_801D66F0);
    }    
}

void func_8019F4AC(s32 arg0, u16 arg1) {
    Struct_D_801FD1F0* temp_v0;

    if ((arg1 == NA_SE_EN_KONB_JUMP_OLD) || (arg1 == NA_SE_EN_KONB_SINK_OLD)) {
        Audio_PlaySfxGeneral(arg1, arg0, 4, &gDefaultSfxFreq, &gDefaultSfxFreq, &gDefaultSfxReverb);
        return;
    }
    temp_v0 = func_8019F258(arg0);
    if (temp_v0 != NULL) {
        Audio_PlaySfxGeneral(arg1, arg0, 4, &gDefaultSfxFreq, &gDefaultSfxFreq, &temp_v0->unk_8);
    }
}

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
    Audio_PlaySfxGeneral(sfxId, pos, 4, &D_801FD264, &D_801FD25C, &gDefaultSfxReverb);
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
                Audio_PlaySfxGeneral(sfxId2, pos, 4, &D_801FD264, &D_801FD260, &gDefaultSfxReverb);
            }
        }
    }
}

// OoT func_800F4138
void func_8019F780(Vec3f* pos, u16 sfxId, f32 arg2) {
    func_8019F5AC(arg2);
    Audio_PlaySfxGeneral(sfxId, pos, 4, &D_801FD264, &D_801FD25C, &gDefaultSfxReverb);
}

void func_8019F7D8(Vec3f* pos, u16 sfxId) {
    Audio_PlaySfxGeneral(sfxId | 0xE0, pos, 4, &D_801D6648, &gDefaultSfxFreq, &D_801D664C);
}

void func_8019F830(Vec3f* pos, u16 sfxId) {
    Audio_PlaySfxGeneral((sfxId & 0x681F) + 0x20, pos, 4, &D_801D6648, &gDefaultSfxFreq, &D_801D664C);
}

void Audio_PlayRandomSfx(Vec3f* pos, u16 baseSfxId, u8 randLim) {
    u8 offset = Audio_NextRandom() % randLim;

    Audio_PlaySfxGeneral(baseSfxId + offset, pos, 4, &gDefaultSfxFreq, &gDefaultSfxFreq, &gDefaultSfxReverb);
}

// OoT func_800F4254
void func_8019F900(Vec3f* pos, u8 level) {
    level &= 3;
    if (level != sPrevChargeLevel) {
        D_801D6694 = D_801D6684[level];
        switch (level) {
            case 1:
                Audio_PlaySfxGeneral(NA_SE_PL_SWORD_CHARGE, pos, 4, &D_801D6694, &gDefaultSfxFreq, &gDefaultSfxReverb);
                break;
            case 2:
                Audio_PlaySfxGeneral(NA_SE_PL_SWORD_CHARGE, pos, 4, &D_801D6694, &gDefaultSfxFreq, &gDefaultSfxReverb);
                break;
        }
        sPrevChargeLevel = level;
    }
    if (level != 0) {
        Audio_PlaySfxGeneral(NA_SE_IT_SWORD_CHARGE - SFX_FLAG, pos, 4, &D_801D6694, &gDefaultSfxFreq, &gDefaultSfxReverb);
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
        Audio_PlaySfxGeneral(sfxId, pos, 4, &D_801FD288, &gDefaultSfxFreq, &gDefaultSfxReverb);

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
    Audio_PlaySfxGeneral(sfxId, pos, 4, &gDefaultSfxFreq, &gDefaultSfxFreq, &D_801FD28C);
}

// OoT func_800F4578
void func_8019FE1C(Vec3f* pos, u16 sfxId, f32 arg2) {
    D_801D6654 = arg2;
    Audio_PlaySfxGeneral(sfxId, pos, 4, &gDefaultSfxFreq, &D_801D6654, &gDefaultSfxReverb);
}

void func_8019FE74(f32* arg0, f32 arg1, u16 arg2) {
    D_801FD294 = arg0;
    D_801FD298 = arg1;
    D_801D66A4 = arg2;
    D_801FD29C = (*D_801FD294 - D_801FD298) / D_801D66A4;
}

void func_8019FEDC(void) {
    if (D_801D66A4 != 0) {
        D_801D66A4--;
        if (D_801D66A4 == 0) {
            *D_801FD294 = D_801FD298;
        } else {
            *D_801FD294 -= D_801FD29C;
        }
    }
}

// OoT func_800F45D0
void func_8019FF38(f32 arg0) {
    func_8019FCB8(&gDefaultSfxPos, NA_SE_IT_FISHING_REEL_SLOW - SFX_FLAG, arg0);
    func_8019FAD8(&gDefaultSfxPos, 0, (0.15f * arg0) + 1.4f);
}

void Audio_PlaySoundRiver(Vec3f* pos, f32 freqScale) {
    if (!Audio_IsSfxPlaying(NA_SE_EV_RIVER_STREAM - SFX_FLAG)) {
        sRiverFreqScaleLerp.value = freqScale;
    } else if (freqScale != sRiverFreqScaleLerp.value) {
        sRiverFreqScaleLerp.target = freqScale;
        sRiverFreqScaleLerp.remainingFrames = 40;
        sRiverFreqScaleLerp.step = (sRiverFreqScaleLerp.target - sRiverFreqScaleLerp.value) / 40;
    }
    Audio_PlaySfxGeneral(NA_SE_EV_RIVER_STREAM - SFX_FLAG, pos, 4, &sRiverFreqScaleLerp.value, &gDefaultSfxFreq, &gDefaultSfxReverb);
}

void Audio_PlaySoundWaterfall(Vec3f* pos, f32 freqScale) {
    if (!Audio_IsSfxPlaying(NA_SE_EV_WATER_WALL_BIG - SFX_FLAG)) {
        sWaterfallFreqScaleLerp.value = freqScale;
    } else if (freqScale != sWaterfallFreqScaleLerp.value) {
        sWaterfallFreqScaleLerp.target = freqScale;
        sWaterfallFreqScaleLerp.remainingFrames = 40;
        sWaterfallFreqScaleLerp.step = (sWaterfallFreqScaleLerp.target - sWaterfallFreqScaleLerp.value) / 40;
    }
    Audio_PlaySfxGeneral(NA_SE_EV_WATER_WALL_BIG - SFX_FLAG, pos, 4, &sWaterfallFreqScaleLerp.value,
                           &sWaterfallFreqScaleLerp.value, &gDefaultSfxReverb);
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

void Audio_PlaySignalBigBellsSfx(Vec3f* pos, u8 arg1) {
    Audio_PlaySfxGeneral(NA_SE_EV_SIGNAL_BIGBELL, pos, 4, &gDefaultSfxFreq, &D_801D8BB0[arg1 & 7], &gDefaultSfxReverb);
}

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

void func_801A0318(u8); // extern
extern s8 D_801D66A0;

void func_801A026C(u8 arg0) {
    u8 i;
    s8 phi_s1;

    phi_s1 = 0;

    if (arg0 == 0) {
        phi_s1 = 0x7F;
    }

    for (i = 0; i < 16; i++) {
        Audio_QueueCmdS8(((u8)(u32)i << 8) | 0x7000000, phi_s1);
    }

    if (arg0 == 7) {
        D_801D66A0 = 2;
    } else {
        func_801A0318(D_801D6698[arg0 & 7]);
    }

}


#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A0318.s")

void func_801A0450(u8 arg0) {
    D_801FD28D = arg0;
    D_801FD28F = 1;
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A046C.s")

void Audio_PlaySoundIncreasinglyTransposed(Vec3f* pos, s16 sfxId, u8* semitones) {
    Audio_PlaySfxGeneral(sfxId, pos, 4, &gNoteFrequencies[semitones[sAudioIncreasingTranspose] + 39], &gDefaultSfxFreq, &gDefaultSfxReverb);
    if (sAudioIncreasingTranspose < 15) {
        sAudioIncreasingTranspose++;
    }
}

void Audio_ResetIncreasingTranspose(void) {
    sAudioIncreasingTranspose = 0;
}

void Audio_PlaySoundTransposed(Vec3f* pos, u16 sfxId, s8 semitone) {
    Audio_PlaySfxGeneral(sfxId, pos, 4, &gNoteFrequencies[semitone + 39], &gDefaultSfxFreq, &gDefaultSfxReverb);
}

void func_801A0654(Vec3f* pos, u16 sfxId, u8 arg2) {
    u8 temp_v0;
    u8 i;
    u8 phi_s1;
    u8 entryIndex;

    phi_s1 = 0;
    temp_v0 = sfxId >> 0xC;
    
    for (i = 0; i < temp_v0; i++) {
        phi_s1 += gChannelsPerBank[gSfxChannelLayout][i];
    }

    for (i = 0; i < gChannelsPerBank[gSfxChannelLayout][temp_v0]; i++) {
        entryIndex = gActiveSounds[temp_v0][i].entryIndex;
        if (entryIndex != 0xFF) {
            if ((sfxId == gSoundBanks[temp_v0][entryIndex].sfxId) && (&pos->x == gSoundBanks[temp_v0][entryIndex].posX)) {
                Audio_QueueCmdS8(_SHIFTL(6, 24, 8) | _SHIFTL(2, 16, 8) | _SHIFTL(phi_s1, 8, 8) | _SHIFTL(6, 0, 8), arg2);
            }
        }
        phi_s1++;
    }
}

void func_801A0810(Vec3f* pos, u16 sfxId, u8 arg2) {
    func_801A0654(pos, sfxId, arg2);
    Audio_PlaySfxGeneral(sfxId, pos, 4, &gDefaultSfxFreq, &gDefaultSfxFreq, &gDefaultSfxReverb);
}

// OoT func_800F4C58
void func_801A0868(Vec3f* pos, u16 sfxId, u8 arg2) {
    u8 phi_s1 = 0;
    u8 i;
    u8 bankId;

    bankId = SFX_BANK_SHIFT(sfxId);
    for (i = 0; i < bankId; i++) {
        phi_s1 += gChannelsPerBank[gSfxChannelLayout][i];
    }

    for (i = 0; i < gChannelsPerBank[gSfxChannelLayout][bankId]; i++) {
        Audio_QueueCmdS8(_SHIFTL(6, 24, 8) | _SHIFTL(2, 16, 8) | _SHIFTL(phi_s1++, 8, 8) | _SHIFTL(6, 0, 8), arg2);
    }

    Audio_PlaySfxGeneral(sfxId, pos, 4, &gDefaultSfxFreq, &gDefaultSfxFreq, &gDefaultSfxReverb);
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
// ISMATCHING: Need to migrate data
#ifdef NON_MATCHING
void func_801A0CB0(s8 volSplit) {
    u8 vol;
    u8 prio;
    u16 channelBits;
    u8 players[2] = { 0, 3 };
    u8 i;
    u8 j;

    if ((func_801A8A50(1) == NA_BGM_DISABLED) && (func_801A8A50(3) != NA_BGM_ROMANI_RANCH)) {
        for (i = 0; i < 2; i++) {
            if (i == 0) {
                vol = volSplit;
            } else {
                vol = 0x7F - volSplit;
            }

            if (vol > 100) {
                prio = 11;
            } else if (vol < 20) {
                prio = 2;
            } else {
                prio = ((vol - 20) / 10) + 2;
            }

            channelBits = 0;
            for (j = 0; j < 0x10; j++) {
                if (gAudioContext.seqPlayers[players[i]].channels[j]->notePriority < prio) {
                    channelBits += (1 << j);
                }
            }

            Audio_SeqCmdA(players[i], channelBits);
        }
    }
}
#else
void func_801A0CB0(s8 volSplit);
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A0CB0.s")
#endif

void func_801A0E44(u8 arg0, Vec3f* arg1, s16 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6) {
    f32 temp_f0;
    f32 sp28;
    s8 sp27;
    s8 sp26;
    s32 sp1C;

    if (arg1->z > 0.0f) {
        if (arg1->z > 100.0f) {
            sp27 = 0;
        } else {
            sp27 = ((100.0f - arg1->z) / 100.0f) * 64.0f;
        }
    } else if (arg1->z < -100.0f) {
        sp27 = 0x7F;
    } else {
        sp27 = (s8)((-arg1->z / 100.0f) * 64.0f) + 0x3F;
    }

    if (arg1->x > 0.0f) {
        if (arg1->x > 200.0f) {
            sp26 = 0x6C;
        } else {
            sp26 = (s8)((arg1->x / 200.0f) * 45.0f) + 0x3F;
        }
    } else if (arg1->x < -200.0f) {
        sp26 = 0x14;
    } else {
        sp26 = (s8)(((arg1->x + 200.0f) / 200.0f) * 44.0f) + 0x14;
    }

    temp_f0 = sqrtf(SQ(arg1->z) + ((SQ(arg1->x) * 0.25f) + (SQ(arg1->y) / 6.0f)));
    
    if (arg4 < temp_f0) {
        sp28 = arg6;
    } else if (temp_f0 < arg3) {
        sp28 = 1.0f;
    } else {
        sp28 = (((arg4 - temp_f0) / (arg4 - arg3)) * (1.0f - arg6)) + arg6;
    }

    Audio_QueueCmdU16(arg0 << 0x10 | 0x90000000, 0xFFFF);

    if (arg2 & 1) {
        Audio_QueueCmdS8(arg0 << 0x10 | 0x12000000 | 0xFF00, sp27);
    }

    if (arg2 & 2) {
        Audio_QueueCmdS8(arg0 << 0x10 | 0x3000000 | 0xFF00, sp26);
    }

    if (arg2 & 4) {
        Audio_QueueCmdS8(arg0 << 0x10 | 0x7000000 | 0xFF00, 0x7F);
    }

    if (arg2 & 8) {
        Audio_QueueCmdS32(arg0 << 0x10 | 0x13000000 | 0xFF00 | 0x54, ((u32)&D_801FD3B8 & ~0xF) + 0x10);
    } else {
        Audio_QueueCmdS32(arg0 << 0x10 | 0x13000000 | 0xFF00, ((u32)&D_801FD3B8 & ~0xF) + 0x10);
    }

    if (arg2 & 0x10) {
        Audio_QueueCmdS8(arg0 << 0x10 | 0x14000000 | 0xFF00, 0x7F);
    } else {
        Audio_QueueCmdS8(arg0 << 0x10 | 0x14000000 | 0xFF00, 0);
    }

    if (arg2 & 0x20) {
        Audio_QueueCmdF32(arg0 << 0x10 | 0x1000000 | 0xFF00, arg5 * sp28);
    }

    if (arg2 & 0x40) {
        Audio_QueueCmdS8(arg0 << 0x10 | 0x7000000 | 0xFF00, 0x40);
    }

    if (arg2 & 0x80) {
        Audio_QueueCmdS8(arg0 << 0x10 | 0x11000000 | 0xFF00, 1);
    }

    if (arg2 & 0x100) {
        Audio_QueueCmdS8(arg0 << 0x10 | 0x5000000 | 0xFF00, 0x37);
    }
}

void func_801A1290(void) {
    if (D_801FD3EC != 0xFF) {
        if ((func_801A8A50(D_801FD3EC) != NA_BGM_FINAL_HOURS) && (func_801A8ABC((D_801FD3EC << 0x18) + 0x57, 0xFF0000FF) != 0) && (D_801FD3D8 == 0)) {
            func_801A0E44(D_801FD3EC, &D_801FD3F0, D_801FD3FC, D_801FD400, D_801FD404, D_801FD408, D_801FD40C);
        }
        D_801FD3EC = 0xFF;
    }
}

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
            if (func_801A8A50(1) == NA_BGM_DISABLED) {
                func_801A3038();
            }
            if ((func_801A8A50(0) != NA_BGM_DISABLED) && (func_801A8A50(4) == NA_BGM_DISABLED)) {
                func_801A4A28(9);
            }
            sAudioCutsceneFlag = 1;
        } else {
            func_801A13BC(1, NULL, D_801FD3D9, 0);
            if (func_801A8A50(0) != NA_BGM_DISABLED) {
                Audio_QueueSeqCmd(0x140000FF);
            }
            D_801FD3DA = 0;
            D_801D66FC = 0;
            sAudioCutsceneFlag = 0;
        }
        D_801FD3D9 = 0;
    }
}


void func_801A1A10(u8 arg0, u8 volFadeTimer) {
    Audio_QueueSeqCmd(((u32)arg0 << 0x18) | 0x100000FF | 0x140000);
    if ((arg0 == 3) && (func_801A8A50(0) != NA_BGM_DISABLED)) {
        Audio_SetVolScale(0, 3, 0x7F, volFadeTimer);
        func_801A0CB0(0);
    }
}

void func_801A1A8C(u8, Vec3f*, u8, u8, f32, f32, f32); // extern
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A1A8C.s")

void func_801A1D44(Vec3f* vec, u8 seqId, f32 arg2) {
    if (gAudioSpecId != 0xC) {
        D_801FD410.x = vec->x;
        D_801FD410.y = vec->y;
        D_801FD410.z = vec->z;
        D_801FD430 = seqId;
        D_801FD42C = arg2;
        D_801FD431 |= 2;
        D_801FD433 = 4;
    }
}

void func_801A1DB8(Vec3f* vec, u8 seqId, f32 arg2) {
    D_801FD420.x = vec->x;
    D_801FD420.y = vec->y;
    D_801FD420.z = vec->z;
    D_801FD430 = seqId;
    D_801FD431 |= 1;
    D_801FD433 = 4;
}

void func_801A1E0C(void) {
    if (D_801FD433 != 0) {
        if ((D_801FD431 & 2) != 0) {
            func_801A1A8C(3, &D_801FD410, D_801FD430, 0x20, 100.0f, D_801FD42C, 1.0f);
        } else {
            func_801A1A8C(3, &D_801FD420, D_801FD430, 0x28, 100.0f, D_801FD42C, 1.0f);
        }

        D_801FD433--;
        if (D_801FD433 == 0) {
            func_801A1A10(3, 10);
        }

        D_801FD431 = 0;
    }
}

void func_801A1F00(u8 arg0, u16 seqId) {
    if (!sAudioCutsceneFlag && (gAudioSpecId != 0xC)) {
        D_801FD420.x = gDefaultSfxPos.x;
        D_801FD420.y = gDefaultSfxPos.y;
        D_801FD420.z = gDefaultSfxPos.z;
        D_801FD42C = 10000.0f;
        D_801FD436 = 0x80;
        D_801FD430 = seqId;
        D_801FD435 = arg0;
    }
}

void func_801A1F88(void) {
    if (gAudioSpecId != 0xC) {
        D_801FD436 = 1;
        D_801FD430 = 0;
    }
}

void func_801A1FB4(u8 arg0, Vec3f* arg1, u16 seqId, f32 arg3) {
    if (!sAudioCutsceneFlag) {
        if ((func_801A8A50(0) & 0xFF) != NA_BGM_WING_WARP) {
            if (gAudioSpecId != 0xC) {
                if (arg1 != 0) {
                    if (((D_801FD435 != 0) || (func_801A8A50(0) != NA_BGM_FINAL_HOURS))) {
                        D_801FD420.x = arg1->x;
                        D_801FD420.y = arg1->y;
                        D_801FD420.z = arg1->z;
                        D_801FD42C = arg3;
                        D_801FD436 = 2;
                        D_801FD430 = seqId & 0xFF;
                        D_801FD435 = arg0;
                    }
                }
            }
        }
    }
}

void func_801A2090(void) {
    u16 seqId0 = func_801A8A50(0);
    u8 volFadeTimer;

    if ((D_801FD436 != 0) && (D_801FD3B0 == 0)) {
        if ((D_801FD430 == NA_BGM_GENERAL_SFX) || (seqId0 == NA_BGM_WING_WARP)) {
            volFadeTimer = 10;

            if (seqId0 == NA_BGM_WING_WARP) {
                D_801FD436 = 0;
                volFadeTimer = 1;
            } else {
                if (D_801FD436 < 128) {
                    D_801FD436--;
                }
            }

            if (D_801FD436 == 0) {
                func_801A1A10(D_801FD435, volFadeTimer);
                D_801FD3B4[D_801FD435] = 0;
            }
        } else {
            if ((D_801FD435 == 0) && (seqId0 == NA_BGM_FINAL_HOURS)) {
                func_801A1A10(D_801FD435, 10);
                D_801FD3B4[D_801FD435] = 0;
                return;
            }

            func_801A1A8C(D_801FD435, &D_801FD420, D_801FD430, 0x20, 200.0f, D_801FD42C, 1.0f);
            if (D_801FD3B4[D_801FD435] == 0) {
                D_801FD436 = 0;
            }
        }

        if (D_801FD436 < 128) {
            D_801FD430 = NA_BGM_GENERAL_SFX;
        }
    }
}

void func_801A3238(s8 playerIdx, u16 seqId, u8 fadeTimer, s8 arg3, u8 arg4);
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

    absY = ABS_ALT(pos->y);

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

void func_801A246C(u8 arg0, u8 arg1) {
    u16 seqId;
    u8 phi_a2;
    s32 phi_v1;

    phi_v1 = 0;
    phi_a2 = 0;

    switch (arg1) {
        case 0:
            phi_a2 = 0x7F;
            break;
        case 1:
            phi_v1 = 0xFFFF;
            break;
        case 2:
            phi_a2 = 0xFF;
            break;
    }

    if (phi_a2 != 0xFF) {
        Audio_QueueSeqCmd((arg0 << 0x18) | 0xA0000000 | phi_v1);
        Audio_SetVolScale(0, 3, phi_a2, 1);
        return;
    }
    Audio_SetVolScale(0, 3, 0x7F, 0);
    seqId = func_801A8A50(arg0);
    if (seqId != NA_BGM_DISABLED) {
        Audio_QueueSeqCmd((arg0 << 0x18) | seqId);
    }
}

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
        } else if ((arg0 != 0x57) || (D_801D66CC == NA_BGM_DISABLED)) {
            func_801A2670(arg0);
            Audio_QueueSeqCmd(arg1 | 0x70040000);
        }
        D_801FD2A0 = arg0;
    }
}

void func_801A2670(u16 arg0) {
    u8 temp_v1;
    u8 phi_a2;
    u8 phi_v0;

    phi_a2 = 0;
    if (((D_801D6700[D_801D66D4] & 0x20) != 0) && (temp_v1 = arg0, ((D_801D6700[temp_v1 & 0xFF] & 0x10) != 0))) {
        if ((D_801D66D0 & 0x3F) != 0) {
            phi_a2 = 0x1E;
        }
        func_801A3238(0, arg0, phi_a2, 7, D_801D66D0);
        D_801D66D0 = 0;
    } else {
        temp_v1 = arg0;
        if ((D_801D6700[temp_v1 & 0xFF] & 0x40) != 0) {
            phi_v0 = 1;
        } else {
            phi_v0 = 0xFF;
        }
        func_801A3238(0, arg0, 0, 7, phi_v0);
        if ((D_801D6700[arg0] & 0x20) == 0) {
            D_801D66D0 = 0xC0;
        }
    }
    D_801D66D4 = temp_v1;
}

void func_801A2778(void) {
    u16 seqId0 = func_801A8A50(0);

    if ((seqId0 != NA_BGM_DISABLED) && (D_801D6700[(u8)seqId0 & 0xFF] & 0x10)) {
        if (D_801D66D0 != 0xC0) {
            D_801D66D0 = gAudioContext.seqPlayers[0].soundScriptIO[3];
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

void func_801A281C(f32 arg0, u8 arg1) {
    if (arg0 == 1.0f) {
        Audio_QueueSeqCmd(arg1 << 0x10 | 0xB0004000);
    } else {
        Audio_QueueSeqCmd(arg1 << 0x10 | 0xB0003000 | (u16)(arg0 * 100.0f));
    }

    Audio_QueueSeqCmd(arg1 << 0x10 | 0x50000000 | (u16)(arg0 * 1000.0f));
}

void func_801A29D4(u8 arg0, f32 arg1, u8 arg2) {
    if (arg1 == 1.0f) {
        Audio_QueueSeqCmd(arg0 << 0x18 | 0xB0004000 | arg2 << 0x10);
    } else {
        Audio_QueueSeqCmd(arg0 << 0x18 | 0xB0003000 | arg2 << 0x10 | (u16)(arg1 * 100.0f));
    }

    Audio_QueueSeqCmd(arg0 << 0x18 | 0x50000000 | arg2 << 0x10 | (u16)(arg1 * 1000.0f));
}

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

void func_801A2C88(u16 seqId) {
    u8 seqIdTemp = seqId;

    if (D_801D6700[seqIdTemp & 0xFF] & 2) {
        func_801A3098(seqId);
    } else if (D_801D6700[seqIdTemp & 0xFF] & 4) {
        Audio_QueueSeqCmd(seqId | 0x1000000);
    } else if (D_801D6700[seqIdTemp & 0xFF] & 0x80) {
        Audio_QueueSeqCmd(seqId | 0x3000000);
    } else {
        func_801A3238(0, seqId, 0, 7, 0xFF);
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

// OoT func_800F5A58
s32 Audio_IsSequencePlaying(u8 seqId) {
    u8 phi_a1 = 0;

    if (D_801D6700[seqId & 0xFF] & 2) {
        phi_a1 = 1;
    } else if (D_801D6700[seqId & 0xFF] & 4) {
        phi_a1 = 1;
    }

    if (seqId == (func_801A8A50(phi_a1) & 0xFF)) {
        return true;
    } else {
        return false;
    }
}

void func_801A2E54(u16 seqId) {
    u16 seqId0 = func_801A8A50(0);

    if (seqId0 == NA_BGM_DISABLED) {
        seqId0 = NA_BGM_GENERAL_SFX;
    }

    if (seqId0 != seqId) {
        func_801A32CC(3);
        if (!(D_801D6700[seqId0] & 8)) {
            D_801D66CC = seqId0;
        }
        Audio_QueueSeqCmd(seqId + 0x8000);
    }
}

void func_801A2ED8(void) {
    if ((func_801A8A50(0) != NA_BGM_DISABLED) && (D_801D6700[func_801A8A50(0) & 0xFF] & 8)) {
        if ((D_801D66CC == NA_BGM_DISABLED) || (D_801D66CC == NA_BGM_GENERAL_SFX)) {
            Audio_QueueSeqCmd(0x100000FF);
        } else {
            if (D_801D66CC == NA_BGM_NATURE_AMBIENCE) {
                D_801D66CC = D_801FD438;
            }
            Audio_QueueSeqCmd(D_801D66CC + 0x8000);
        }
        D_801D66CC = NA_BGM_DISABLED;
    }
}

void func_801A2F88(u8 arg0) {
    u16 seqId0 = func_801A8A50(0);

    if (seqId0 != NA_BGM_NATURE_AMBIENCE) {
        D_801D66CC = seqId0;
    }
    func_801A4A28(arg0);
}

void func_801A2FC4(void) {
    if (D_801D66CC != NA_BGM_DISABLED) {
        Audio_QueueSeqCmd(D_801D66CC + 0x8000);
    }
    D_801D66CC = NA_BGM_DISABLED;
}

void func_801A3000(u16 seqId, u8 arg1) {
    func_801A3098(seqId);
    Audio_QueueSeqCmd(arg1 | 0x71070000);
}

void func_801A3038(void) {
    Audio_SetVolScale(0, 1, 0, 5);
    Audio_SetVolScale(3, 1, 0, 5);
    Audio_QueueSeqCmd(0xC180010A);
    Audio_QueueSeqCmd(0xC183010A);
    Audio_QueueSeqCmd(0xC1900000);
}

void func_801A3098(u16 seqId) {
    u16 seqId1;
    u32 sp20;
    u8* sp1C;
    u8* ret;

    seqId1 = func_801A8A50(1);
    sp1C = func_80193C04(seqId1 & 0xFF, &sp20);
    ret = func_80193C04(seqId & 0xFF, &sp20);
    if ((seqId1 == NA_BGM_DISABLED) || (*sp1C == *ret)) {
        D_801FD3AB = 1;
    } else {
        D_801FD3AB = 5;
        Audio_QueueSeqCmd(0x110000FF);
    }
    
    D_801FD3AC = seqId;
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
                if (func_801A8A50(1) == NA_BGM_DISABLED) {
                    func_801A3038();
                }
                Audio_QueueSeqCmd(D_801FD3AC | 0x1000000);
                Audio_QueueSeqCmd(0xA000FFFF);
            }
        }
    }
}

void func_801A31EC(u16 seqId, s8 arg1, u8 arg2) {
    Audio_QueueSeqCmd((arg1 << 0x10) | 0x71000000 | arg2);
    func_801A3098(seqId);
}

// OoT func_800F5E18
void func_801A3238(s8 playerIdx, u16 seqId, u8 fadeTimer, s8 arg3, u8 arg4) {
    u16 phi_a1;

    Audio_QueueSeqCmd(playerIdx << 0x18 | 0x70000000 | (arg3 << 0x10) | arg4);
    if ((seqId & 0xFF) < 2) {
        phi_a1 = seqId;
    } else {
        phi_a1 = (seqId | 0x8000);
    }
    Audio_QueueSeqCmd(playerIdx << 0x18 | (fadeTimer << 0x10) | phi_a1);
}

// OoT func_800F5E90
void func_801A32CC(u8 arg0) {
    s32 phi_t0;
    u16 phi_t1;
    u8 phi_a3;

    if ((D_801D66CC == NA_BGM_DISABLED) && (D_801D66CC == NA_BGM_DISABLED)) {
        if (sAudioCutsceneFlag || (D_801FD3B4[3] != 0)) { arg0 = 3;
        }
        phi_t1 = D_80200140[0].unk_254;
        phi_a3 = phi_t1 & 0xFF;
        if ((phi_t1 == NA_BGM_DISABLED) || ((D_801D6700[phi_a3] & 1) != 0) || ((D_801D66C0 & 0x7F) == 1)) {
            if (arg0 != (D_801D66C0 & 0x7F)) {
                if (arg0 == 1) {
                    if (D_80200140[3].volScales[1] - sAudioEnemyVol < 0) {
                        phi_t0 = -(D_80200140[3].volScales[1] - sAudioEnemyVol);
                    } else {
                        phi_t0 = D_80200140[3].volScales[1] - sAudioEnemyVol;
                    }
                    Audio_SetVolScale(3, 3, sAudioEnemyVol, phi_t0);
                    Audio_StartSeq(3, 10, 0x800 | NA_BGM_ENEMY);
                    if (phi_t1 >= NA_BGM_FIELD) {
                        Audio_SetVolScale(0, 3, (0x7F - sAudioEnemyVol) & 0xFF, 0xA);
                        func_801A0CB0(sAudioEnemyVol);
                    }
                } else {
                    if ((D_801D66C0 & 0x7F) == 1) {
                        Audio_SeqCmd1(3, 10);
                        if (arg0 == 3) {
                            phi_a3 = 0;
                        } else {
                            phi_a3 = 10;
                        }

                        Audio_SetVolScale(0, 3, 0x7F, phi_a3);
                        func_801A0CB0(0);
                    }
                }

                D_801D66C0 = arg0 + 0x80;
            } else {
                if ((arg0 == 1) && (func_801A8A50(3) == NA_BGM_DISABLED) && (phi_t1 != NA_BGM_DISABLED) && ((D_801D6700[phi_a3] & 1) != 0)) {
                    Audio_QueueSeqCmd(0x30A0800 | NA_BGM_ENEMY);
                    D_801D66C0 = arg0 + 0x80;
            }
            }
        } else {
            if (arg0 == 0) {
                if (D_801D66C0 == 2) {
                    D_801D66DC = 0;
                }
                D_801D66D8 = 0;
                D_801D66DC++;
            } else {
                D_801D66D8++;
            }

            if (arg0 == 2 && D_801D66D8 < 30 && D_801D66DC > 20) {
                arg0 = 0;
                if (!arg0 && !arg0) {}
            }

            D_801D66C0 = arg0;
            Audio_SeqCmd7(0, 2, arg0);
        }
    }
}

// OoT func_800F6114
void func_801A3590(f32 dist) {
    f32 adjDist;
    u16 bgmId = D_8020034A;
    u16 index;

    if (D_801D66C0 == 0x81) {
        if (dist != sAudioEnemyDist) {
            if (dist < 150.0f) {
                adjDist = 0.0f;
            } else if (dist > 500.0f) {
                adjDist = 350.0f;
            } else {
                adjDist = dist - 150.0f;
            }

            sAudioEnemyVol = ((350.0f - adjDist) * 127.0f) / 350.0f;
            Audio_SetVolScale(3, 3, sAudioEnemyVol, 0xA);
            index = bgmId & 0xFF;
            if ((bgmId >= NA_BGM_FIELD) && !(D_801D6700[index] & 4)) {
                Audio_SetVolScale(0, 3, (0x7F - sAudioEnemyVol), 0xA);
            }
        }

        func_801A0CB0(sAudioEnemyVol);
    }
    sAudioEnemyDist = dist;
}

// OoT func_800F6114
void func_801A36F0(f32 dist, u16 seqId) {
    s8 pad;
    s8 phi_v1;
    s16 seqIdCurr;

    if (D_801FD3A9 == 0) {
        seqIdCurr = (s8)(func_801A8A50(0) & 0xFF);
        if (seqIdCurr == (seqId & 0xFF)) {
            if ((seqId & 0xFF) == NA_BGM_ROMANI_RANCH) {

                if (dist > 2000.0f) {
                    phi_v1 = 127;
                } else if (dist < 200.0f) {
                    phi_v1 = 0;
                } else {
                    phi_v1 = (s8)(((dist - 200.0f) * 127.0f) / 1800.0f);
                }
                Audio_SeqCmd6(0, 3, 0, 127 - phi_v1);
                Audio_SeqCmd6(0, 3, 1, 127 - phi_v1);
                Audio_QueueSeqCmd(phi_v1 | 0x60030D00);
                if (D_801FD3A8 == 0) {
                    D_801FD3A8++;
                }
            }
        } else if ((seqIdCurr == NA_BGM_NATURE_AMBIENCE) && ((seqId & 0xFF) == NA_BGM_ROMANI_RANCH)) {
            seqIdCurr = (s8)(func_801A8A50(3) & 0xFF);
            if ((seqIdCurr != (seqId & 0xFF)) && (D_801FD3A8 < 10)) {
                func_801A3238(3, NA_BGM_ROMANI_RANCH, 0, 0, 0);
                Audio_SeqCmdA(3, 0xFFFC);
                D_801FD3A8 = 10;
            }

            if (dist > 2000.0f) {
                phi_v1 = 127;
            } else if (dist < 200.0f) {
                phi_v1 = 0;
            } else {
                phi_v1 = (s8)(((dist - 200.0f) * 127.0f) / 1800.0f);
            }
            Audio_SeqCmd6(3, 3, 0, 127 - phi_v1);
            Audio_SeqCmd6(3, 3, 1, 127 - phi_v1);
        }

        if (D_801FD3A8 < 10) {
            D_801FD3A8++;
        }
    }
}

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
        if ((func_801A8A50(3) == NA_BGM_FROG_SONG) && channel->enabled) {
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
        Audio_PlaySfxById1(NA_SE_SY_WIN_OPEN);
        Audio_QueueCmdS32(0xF1FF0000, 0);
    } else {
        Audio_PlaySfxById1(NA_SE_SY_WIN_CLOSE);
        Audio_QueueCmdS32(0xF2FF0000, 0);
    }
}

void func_801A3B48(u8 arg0) {
    if (arg0 == 1) {
        Audio_QueueCmdS32(0xF1030000, 0);
    } else {
        Audio_QueueCmdS32(0xF2030000, 0);
    }
}

// OoT func_800F6584
#ifdef NON_MATCHING
#define Audio_SeqCmd62(playerIdx, a, b, c) \
    Audio_QueueSeqCmd(0x60000000 | ((u8)playerIdx << 24) | ((u8)(a) << 16) | ((u8)b << 8) | ((u8)c))
#define Audio_SeqCmdA2(playerIdx, a) Audio_QueueSeqCmd(0xA0000000 | ((u8)playerIdx << 24) | ((u16)(a)))
void func_801A3B90(u8 arg0) {
    u8 playerIdx;
    u16 sp34;

    D_801FD3A9 = arg0;
    if ((func_801A8A50(0) & 0xFF) == NA_BGM_ROMANI_RANCH) {
        playerIdx = 0;
        sp34 = 0;
    } else if ((func_801A8A50(3) & 0xFF) == NA_BGM_ROMANI_RANCH) {
        playerIdx = 3;
        sp34 = 0xFFFC;
    } else {
        return;
    }

    if (arg0 != 0) {
        Audio_SeqCmd62(playerIdx, 1, 0, 0);
        Audio_SeqCmd62(playerIdx, 1, 1, 0);
        // Audio_SeqCmd6(playerIdx, 1, 0, 0);
        // Audio_SeqCmd6(playerIdx, 1, 1, 0);
        if (playerIdx == 3) {
            Audio_QueueSeqCmd(playerIdx | 0xA0000000 | (sp34 | 3));
        }
    } else {
        if (playerIdx == 3) {
            func_801A3238(3, NA_BGM_ROMANI_RANCH, 0, 0, 0);
        }
        // Audio_SeqCmd6(playerIdx, 1, 0, 0x7F);
        // Audio_SeqCmd6(playerIdx, 1, 1, 0x7F);
        Audio_SeqCmd6(playerIdx, 1, 0, 0x7F);
        Audio_SeqCmd6(playerIdx, 1, 1, 0x7F);
        if (playerIdx == 3) {
            Audio_QueueSeqCmd(playerIdx | 0xA0000000 | sp34);
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/code_8019AF00/func_801A3B90.s")
#endif

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

    func_801A0868(&gDefaultSfxPos, NA_SE_SY_SOUT_DEMO, phi_v0);
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
            Audio_PlaySfxGeneral(NA_SE_PL_IN_BUBBLE, &gDefaultSfxPos, 4, &gDefaultSfxFreq, &gDefaultSfxFreq, &gDefaultSfxReverb);
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
    if (D_80200140[4].unk_254 == NA_BGM_NATURE_AMBIENCE) {
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

void Audio_PlaySfxGeneralIfNotInCutscene(u16 sfxId, Vec3f* pos, u8 arg2, f32* freqScale, f32* arg4, s8* reverbAdd) {
    if (!sAudioCutsceneFlag) {
        Audio_PlaySfxGeneral(sfxId, pos, arg2, freqScale, arg4, reverbAdd);
    }
}

void Audio_PlaySfxByIdIfNotInCutscene(u16 sfxId) {
    Audio_PlaySfxGeneralIfNotInCutscene(sfxId, &gDefaultSfxPos, 4, &gDefaultSfxFreq, &gDefaultSfxFreq, &gDefaultSfxReverb);
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
    if (func_801A8A50(1) != NA_BGM_DISABLED) {
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
void Audio_ResetData(void) {
    u8 i;

    for (i = 0; i < 8; i++) {
        D_801FD1F0[i].unk_0 = NULL;
        D_801FD1F0[i].unk_4 = 1;
        D_801FD1F0[i].unk_8 = 0;
    }
    D_801FD250 = 0;
    gLoweredSfxFreq = 0.9f;
    sIncreasedSfxReverb = 20;
    D_801D66A4 = 0;
    D_801D66C0 = 0;
    D_801FD25C = 1.0f;
    D_801FD264 = 1.0f;
    sAudioBaseFilter = 0;
    sAudioExtraFilter = 0;
    sAudioBaseFilter2 = 0;
    sAudioExtraFilter2 = 0;
    func_8019DF28();
    sRiverFreqScaleLerp.remainingFrames = 0;
    sWaterfallFreqScaleLerp.remainingFrames = 0;
    sRiverFreqScaleLerp.value = 1;
    sWaterfallFreqScaleLerp.value = 1;
    D_801FD288 = 1;
    D_801FD28D = 0x7F;
    D_801FD28E = 0x7F;
    D_801FD28F = 0;
    D_801FD290 = 0;
    D_801FD291 = 0xFF;
    D_801FD3A8 = 0;
    D_801FD3DA = 0;
    sSpecReverb = sSpecReverbs[gAudioSpecId];
    D_801D66AC = 0;
    D_801D66CC = NA_BGM_DISABLED;
    Audio_QueueCmdS8(0x46000000, -1);
    sSariaBgmPtr = NULL;
    D_801FD3AB = 0;
    D_801FD3AA = 1;
    D_801FD3A9 = 0;
    D_801D66FC = 0;
    D_801FD438 = NA_BGM_DISABLED;
    D_801FD431 = 0;
    D_801FD432 = 0;
    D_801FD430 = 0;
    D_801FD433 = 0;
    D_801FD434 = 0;
    D_801FD436 = 0;
    D_801FD3AF = 0;
    D_801FD3B0 = 0;
    D_801FD3EC = 0xFF;
    D_801FD3D8 = 0;

    for (i = 0; i < 4; i++) {
        D_801FD3B4[i] = 0;
    }
}

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

void func_801A4748(f32* arg0, u16 sfxId) {
    s32 i;

    for (i = 0; i < 3; i++) {
        D_801D8BD4[i] = arg0[i];
    }

    Audio_PlaySfxByPosAndId((Vec3f*)D_801D8BD4, sfxId);
}

void func_801A479C(Vec3f* pos, u16 sfxId, u16 arg2) {
    func_8019FE1C(pos, sfxId, 0.0f);
    func_8019FE74(&D_801D6654, 1.0f, arg2);
}

// OoT func_800F6D58
void func_801A47DC(u8 arg0, u8 arg1, u8 arg2) {
    u8 t;
    u8 temp_a0;
    u8 i;
    u8 a2 = arg2;

    // sAudioNatureFailed = 1;
    if ((D_80200140[4].unk_254 != NA_BGM_NATURE_AMBIENCE) && func_801A8ABC(1, 0xF00000FF)) {
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
void func_801A48E0(u16 arg0, u16 arg1) {
    u8 i;

    i = arg0 & 0xFF;
    Audio_QueueSeqCmd(0x74000001);
    Audio_QueueSeqCmd(((arg0 >> 8) & 0xFF) | 0x74040000);
    Audio_QueueSeqCmd(i | 0x74050000);
    Audio_SetVolScale(0, 0, 0x7F, 1);

    if ((func_801A8A50(4) != NA_BGM_DISABLED) && (func_801A8A50(4) != NA_BGM_NATURE_AMBIENCE)) {
        func_801A7D04(4, 0);
        Audio_QueueCmdS32(0xF8000000, 0);
    }

    if (func_801A8A50(3) == (0x800 | NA_BGM_ENEMY)) {
        Audio_SetVolScale(0, 3, 0x7F, 1);
    }

    Audio_QueueSeqCmd(0x4000001);
    
    for (i = 0; i < 16; i++) {
        if (((arg1 & (1 << i)) == 0) && ((arg0 & (1 << i)) != 0)) {
            Audio_QueueSeqCmd(((u32)i << 8) | 0x84010000 | 1);
        }
    }
}

// OoT func_800F6FB4
void func_801A4A28(u8 arg0) {
    u8 i = 0;
    u8 b0;
    u8 b1;
    u8 b2;

    if ((D_80200140[4].unk_254 == NA_BGM_DISABLED) || ((D_801D6700[((u8)(D_80200140[4].unk_254 ^ 0)) & 0xFF] & 0x80) == 0)) {
        if (D_80200140[4].unk_254 != NA_BGM_NATURE_AMBIENCE) {
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

// TODO: UB return
u32 func_801A4B80(u8 channelIdx, SequenceChannel* channel) {
    u32 temp_v0;

    gAudioContext.seqPlayers[4].unk_0b1 = 1;
    if ((channelIdx == 0) || (channelIdx == 0xFF)) {
        gAudioContext.seqPlayers[4].unk_34 = 1.0f;
        // return channelIdx;
    } else {
        temp_v0 = Audio_NextRandom();
        gAudioContext.seqPlayers[4].unk_34 = 1.0293 - ((gAudioContext.audioRandom & 0xF) / 144.0f);
        return temp_v0;
    }

}

// OoT func_800F70F8
void Audio_Init(void) {
    AudioLoad_Init(0, 0);
}

void func_801A4C54(u16 arg0) {
    u8 i;

    Audio_ScheduleProcessCmds();
    func_801A7B10(2, 0, 0x70, arg0);
    for (i = 0; i < 16; i++) {
        Audio_QueueCmdS32(((u8)(u32)i << 8) | 0x10020000, &sSfxChannelState[i]);
    }
    Audio_QueueCmdS32(0xE4000000, func_8019F024);
    Audio_QueueCmdS32(0xE4000001, func_801A4B80);
}

// OoT func_800F711C
void Audio_InitSound(void) {
    Audio_ResetData();
    AudioOcarina_ResetStaff();
    Audio_ResetSfxChannelState();
    func_801A9A74();
    Audio_ResetSounds();
    AudioVoice_ResetData();
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
void func_801A4DA4(void) {
    func_801A4D50();
    if (gAudioSpecId < 0xA) {
        Audio_QueueCmdS32(0xE6000100, (*D_801DB930)[gAudioSpecId] + 0x18);
    }
}

// OoT func_800F71BC
void func_801A4DF4(s32 arg0) {
    D_801DB4D8 = 1;
    Audio_ResetData();
    AudioOcarina_ResetStaff();
    Audio_ResetSfxChannelState();
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
    Audio_ResetData();
    Audio_ResetSfxChannelState();
    func_801A4C54(1);
    func_801A4FD8();
}
