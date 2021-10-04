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

// OoT func_800F9280
void func_801A7B10(u8 seqIdx, u8 seqId, u8 arg2, u16 fadeTimer) {
    u8 i;
    u16 dur;
    s32 pad;

    if (D_801DB4C8 == 0 || seqIdx == 2) {
        arg2 &= 0x7F;
        if (arg2 == 0x7F) {
            dur = (fadeTimer >> 3) * 60 * gAudioContext.audioBufferParameters.updatesPerFrame;
            Audio_QueueCmdS32(0x85000000 | _SHIFTL(seqIdx, 16, 8) | _SHIFTL(seqId, 8, 8), dur);
        } else {
            Audio_QueueCmdS32(0x82000000 | _SHIFTL(seqIdx, 16, 8) | _SHIFTL(seqId, 8, 8),
                              (fadeTimer * (u16)gAudioContext.audioBufferParameters.updatesPerFrame) / 4);
        }

        D_80200140[seqIdx].unk_254 = seqId | (arg2 << 8);
        D_80200140[seqIdx].unk_256 = seqId | (arg2 << 8);
        D_80200140[seqIdx].unk_21B = 1;

        if (D_80200140[seqIdx].volCur != 1.0f) {
            Audio_QueueCmdF32(0x41000000 | _SHIFTL(seqIdx, 16, 8), D_80200140[seqIdx].volCur);
        }

        D_80200140[seqIdx].unk_28 = 0;
        D_80200140[seqIdx].unk_18 = 0;
        D_80200140[seqIdx].unk_14 = 0;

        for (i = 0; i < 0x10; i++) {
            D_80200140[seqIdx].unk_50[i].unk_00 = 1.0f;
            D_80200140[seqIdx].unk_50[i].unk_0C = 0;
            D_80200140[seqIdx].unk_50[i].unk_10 = 1.0f;
            D_80200140[seqIdx].unk_50[i].unk_1C = 0;
        }

        D_80200140[seqIdx].unk_250 = 0;
        D_80200140[seqIdx].unk_252 = 0;
    }
}

// OoT func_800F9474
void func_801A7D04(u8 arg0, u16 arg1) {
    Audio_QueueCmdS32(0x83000000 | ((u8)arg0 << 16),
                      (arg1 * (u16)gAudioContext.audioBufferParameters.updatesPerFrame) / 4);
    D_80200140[arg0].unk_254 = 0xFFFF;
}

#ifdef NON_EQUIVALENT
void Audio_ProcessSeqCmd(u32 cmd) {
    s32 pad[2];
    u16 fadeTimer;
    u16 channelMask;
    u16 val;
    u8 oldSpec;
    u8 spec;
    u8 op;
    u8 subOp;
    u8 seqIdx;
    u8 seqId;
    u8 seqArgs;
    u8 found;
    u8 port;
    u8 duration;
    u8 chanIdx;
    u8 i;
    s32 new_var;
    f32 freqScale;

    op = cmd >> 28;
    seqIdx = (cmd & 0xF000000) >> 24;

    switch (op) {
        case 0x0:
            // play sequence immediately
            seqId = cmd & 0xFF;
            seqArgs = (cmd & 0xFF00) >> 8;
            fadeTimer = (cmd & 0xFF0000) >> 13;
            if ((D_80200140[seqIdx].unk_260 == 0) && (seqArgs < 0x80)) {
                func_800F9280(seqIdx, seqId, seqArgs, fadeTimer);
            }
            break;

        case 0x1:
            // disable seq player
            fadeTimer = (cmd & 0xFF0000) >> 13;
            func_800F9474(seqIdx, fadeTimer);
            break;

        case 0x2:
            // queue sequence
            seqId = cmd & 0xFF;
            seqArgs = (cmd & 0xFF00) >> 8;
            fadeTimer = (cmd & 0xFF0000) >> 13;
            new_var = seqArgs;
            for (i = 0; i < D_801FFD34[seqIdx]; i++) {
                if (D_801FFD00[seqIdx][i].unk_0 == seqId) {
                    if (i == 0) {
                        func_800F9280(seqIdx, seqId, seqArgs, fadeTimer);
                    }
                    return;
                }
            }

            found = D_801FFD34[seqIdx];
            for (i = 0; i < D_801FFD34[seqIdx]; i++) {
                if (D_801FFD00[seqIdx][i].unk_1 <= new_var) {
                    found = i;
                    i = D_801FFD34[seqIdx]; // "break;"
                }
            }

            if (D_801FFD34[seqIdx] < 5) {
                D_801FFD34[seqIdx]++;
            }
            for (i = D_801FFD34[seqIdx] - 1; i != found; i--) {
                D_801FFD00[seqIdx][i].unk_1 = D_801FFD00[seqIdx][i - 1].unk_1;
                D_801FFD00[seqIdx][i].unk_0 = D_801FFD00[seqIdx][i - 1].unk_0;
            }
            D_801FFD00[seqIdx][found].unk_1 = seqArgs;
            D_801FFD00[seqIdx][found].unk_0 = seqId;

            if (found == 0) {
                func_800F9280(seqIdx, seqId, seqArgs, fadeTimer);
            }
            break;

        case 0x3:
            // unqueue/stop sequence
            seqId = cmd & 0xFF;
            fadeTimer = (cmd & 0xFF0000) >> 13;

            found = D_801FFD34[seqIdx];
            for (i = 0; i < D_801FFD34[seqIdx]; i++) {
                if (D_801FFD00[seqIdx][i].unk_0 == seqId) {
                    found = i;
                    i = D_801FFD34[seqIdx]; // "break;"
                }
            }

            if (found != D_801FFD34[seqIdx]) {
                for (i = found; i < D_801FFD34[seqIdx] - 1; i++) {
                    D_801FFD00[seqIdx][i].unk_1 = D_801FFD00[seqIdx][i + 1].unk_1;
                    D_801FFD00[seqIdx][i].unk_0 = D_801FFD00[seqIdx][i + 1].unk_0;
                }
                D_801FFD34[seqIdx]--;
            }

            if (found == 0) {
                func_800F9474(seqIdx, fadeTimer);
                if (D_801FFD34[seqIdx] != 0) {
                    func_800F9280(seqIdx, D_801FFD00[seqIdx][0].unk_0, D_801FFD00[seqIdx][0].unk_1, fadeTimer);
                }
            }
            break;

        case 0x4:
            // transition seq volume
            duration = (cmd & 0xFF0000) >> 15;
            val = cmd & 0xFF;
            if (duration == 0) {
                duration++;
            }
            D_80200140[seqIdx].volTarget = (f32)val / 127.0f;
            if (D_80200140[seqIdx].volCur != D_80200140[seqIdx].volTarget) {
                D_80200140[seqIdx].unk_08 = (D_80200140[seqIdx].volCur - D_80200140[seqIdx].volTarget) / (f32)duration;
                D_80200140[seqIdx].unk_0C = duration;
            }
            break;

        case 0x5:
            // transition freq scale for all channels
            duration = (cmd & 0xFF0000) >> 15;
            val = cmd & 0xFFFF;
            if (duration == 0) {
                duration++;
            }
            freqScale = (f32)val / 1000.0f;
            for (i = 0; i < 16; i++) {
                D_80200140[seqIdx].unk_50[i].unk_14 = freqScale;
                D_80200140[seqIdx].unk_50[i].unk_1C = duration;
                D_80200140[seqIdx].unk_50[i].unk_18 = (D_80200140[seqIdx].unk_50[i].unk_10 - freqScale) / (f32)duration;
            }
            D_80200140[seqIdx].unk_250 = 0xFFFF;
            break;

        case 0xD:
            // transition freq scale
            duration = (cmd & 0xFF0000) >> 15;
            chanIdx = (cmd & 0xF000) >> 12;
            val = cmd & 0xFFF;
            if (duration == 0) {
                duration++;
            }
            freqScale = (f32)val / 1000.0f;
            D_80200140[seqIdx].unk_50[chanIdx].unk_14 = freqScale;
            D_80200140[seqIdx].unk_50[chanIdx].unk_18 =
                (D_80200140[seqIdx].unk_50[chanIdx].unk_10 - freqScale) / (f32)duration;
            D_80200140[seqIdx].unk_50[chanIdx].unk_1C = duration;
            D_80200140[seqIdx].unk_250 |= 1 << chanIdx;
            break;

        case 0x6:
            // transition vol scale
            duration = (cmd & 0xFF0000) >> 15;
            chanIdx = (cmd & 0xF00) >> 8;
            val = cmd & 0xFF;
            if (duration == 0) {
                duration++;
            }
            D_80200140[seqIdx].unk_50[chanIdx].unk_04 = (f32)val / 127.0f;
            if (D_80200140[seqIdx].unk_50[chanIdx].unk_00 != D_80200140[seqIdx].unk_50[chanIdx].unk_04) {
                D_80200140[seqIdx].unk_50[chanIdx].unk_08 =
                    (D_80200140[seqIdx].unk_50[chanIdx].unk_00 - D_80200140[seqIdx].unk_50[chanIdx].unk_04) /
                    (f32)duration;
                D_80200140[seqIdx].unk_50[chanIdx].unk_0C = duration;
                D_80200140[seqIdx].unk_252 |= 1 << chanIdx;
            }
            break;

        case 0x7:
            // set global io port
            port = (cmd & 0xFF0000) >> 16;
            val = cmd & 0xFF;
            Audio_QueueCmdS8(0x46000000 | _SHIFTL(seqIdx, 16, 8) | _SHIFTL(port, 0, 8), val);
            break;

        case 0x8:
            // set io port if channel masked
            chanIdx = (cmd & 0xF00) >> 8;
            port = (cmd & 0xFF0000) >> 16;
            val = cmd & 0xFF;
            if ((D_80200140[seqIdx].unk_258 & (1 << chanIdx)) == 0) {
                Audio_QueueCmdS8(0x06000000 | _SHIFTL(seqIdx, 16, 8) | _SHIFTL(chanIdx, 8, 8) | _SHIFTL(port, 0, 8),
                                 val);
            }
            break;

        case 0x9:
            // set channel mask for command 0x8
            D_80200140[seqIdx].unk_258 = cmd & 0xFFFF;
            break;

        case 0xA:
            // set channel stop mask
            channelMask = cmd & 0xFFFF;
            if (channelMask != 0) {
                // with channel mask channelMask...
                Audio_QueueCmdU16(0x90000000 | _SHIFTL(seqIdx, 16, 8), channelMask);
                // stop channels
                Audio_QueueCmdS8(0x08000000 | _SHIFTL(seqIdx, 16, 8) | 0xFF00, 1);
            }
            if ((channelMask ^ 0xFFFF) != 0) {
                // with channel mask ~channelMask...
                Audio_QueueCmdU16(0x90000000 | _SHIFTL(seqIdx, 16, 8), (channelMask ^ 0xFFFF));
                // unstop channels
                Audio_QueueCmdS8(0x08000000 | _SHIFTL(seqIdx, 16, 8) | 0xFF00, 0);
            }
            break;

        case 0xB:
            // update tempo
            D_80200140[seqIdx].unk_14 = cmd;
            break;

        case 0xC:
            // start sequence with setup commands
            subOp = (cmd & 0xF00000) >> 20;
            if (subOp != 0xF) {
                if (D_80200140[seqIdx].unk_4D < 7) {
                    found = D_80200140[seqIdx].unk_4D++;
                    if (found < 8) {
                        D_80200140[seqIdx].unk_2C[found] = cmd;
                        D_80200140[seqIdx].unk_4C = 2;
                    }
                }
            } else {
                D_80200140[seqIdx].unk_4D = 0;
            }
            break;

        case 0xE:
            subOp = (cmd & 0xF00) >> 8;
            val = cmd & 0xFF;
            switch (subOp) {
                case 0:
                    // set sound mode
                    Audio_QueueCmdS32(0xF0000000, D_801DB4CC[val]);
                    break;
                case 1:
                    // set sequence starting disabled?
                    D_801DB4C8 = val & 1;
                    break;
                case 2:
                    // set sequence starting disabled?
                    D_801DB4C8 = val & 2;
                    break;
            }
            break;

        case 0xF:
            // change spec
            spec = cmd & 0xFF;
            gSfxChannelLayout = (cmd & 0xFF00) >> 8;
            oldSpec = gAudioSpecId;
            gAudioSpecId = spec;
            func_800E5F88(spec);
            func_800F71BC(oldSpec);
            Audio_QueueCmdS32(0xF8000000, 0);
            break;
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A7B10/Audio_ProcessSeqCmd.s")
#endif

void Audio_QueueSeqCmd(u32 cmd) {
    sAudioSeqCmds[sSeqCmdWrPos++] = cmd;
}

void Audio_ProcessSeqCmds(void) {
    while (sSeqCmdWrPos != sSeqCmdRdPos) {
        Audio_ProcessSeqCmd(sAudioSeqCmds[sSeqCmdRdPos++]);
    }
}

// OoT func_800FA0B4
u16 func_801A8A50(u8 playerIdx) {
    if (D_80200140[playerIdx].unk_260 == 1) {
        return D_80200140[playerIdx].unk_25C & 0xFF;
    }

    if (D_80200140[playerIdx].unk_254 != 0xFFFF) {
        return D_80200140[playerIdx].unk_254;
    }

    return 0xFFFF;
}

// OoT func_800FA11C
s32 func_801A8ABC(u32 arg0, u32 arg1) {
    u8 i;

    for (i = sSeqCmdRdPos; i != sSeqCmdWrPos; i++) {
        if (arg0 == (sAudioSeqCmds[i] & arg1)) {
            return false;
        }
    }

    return true;
}

// OoT func_800FA174
void func_801A8B14(u8 arg0) {
    D_801FFD34[arg0] = 0;
}

// OoT func_800FA18C
void func_801A8B2C(u8 arg0, u8 arg1) {
    u8 i;

    for (i = 0; i < D_80200140[arg0].unk_4D; i++) {
        u8 unkb = (D_80200140[arg0].unk_2C[i] & 0xF00000) >> 20;

        if (unkb == arg1) {
            D_80200140[arg0].unk_2C[i] = 0xFF000000;
        }
    }
}

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

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A7B10/func_801A8D5C.s")

// OoT func_800FAD34
u8 func_801A9768(void) {
    if (D_801DB4D8 != 0) {
        if (D_801DB4D8 == 1) {
            if (func_80193C5C() == 1) {
                D_801DB4D8 = 0;
                Audio_QueueCmdS8(0x46020000, gSfxChannelLayout);
                func_801A4DA4();
            }
        } else if (D_801DB4D8 == 2) {
            while (func_80193C5C() != 1) {}
            D_801DB4D8 = 0;
            Audio_QueueCmdS8(0x46020000, gSfxChannelLayout);
            func_801A4DA4();
        }
    }

    return D_801DB4D8;
}

#ifdef NON_EQUIVALENT
u8 func_801A982C(void) {
    u8 ret;
    u8 temp_v0;
    u8 temp_v1_2;
    u8 phi_s1_2;

    ret = false;
    temp_v0 = ((D_801DB4DC & 0xFF0000) >> 0x10);
    phi_s1_2 = 0;
    if (D_801DB4DC != 0) {
        D_80200BCC--;
        if (D_80200BCC != 0) {
            for (; temp_v0 != 0; temp_v0 = (temp_v0 >> 1), phi_s1_2++) {
                if ((temp_v0 % 2) != 0) {
                    Audio_QueueCmdS32((phi_s1_2 << 8) | 0xE6040000, D_80200BCE);
                    Audio_ScheduleProcessCmds();
                }
            }

            ret = true;
            D_80200BCE -= D_80200BD0;
        } else {
            for (; temp_v0 != 0; temp_v0 = (temp_v0 >> 1), phi_s1_2++) {
                if ((temp_v0 % 2) != 0) {
                    Audio_QueueCmdS32((phi_s1_2 << 8) | 0xE6000000, D_801DB930[D_801DB4DC & 0xFF][phi_s1_2]);
                    Audio_ScheduleProcessCmds();
                }
            }

            D_801DB4DC = 0;
            Audio_QueueCmdS8(0x46020000, gSfxChannelLayout);
            func_801A4D50();
        }
    }

    return ret;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A7B10/func_801A982C.s")
#endif

// OoT func_800FADF8
void func_801A99B8(void) {
    u8 i, j;

    for (i = 0; i < 5; i++) {
        D_801FFD34[i] = 0;
        D_80200140[i].unk_254 = 0xFFFF;
        D_80200140[i].unk_256 = 0xFFFF;
        D_80200140[i].unk_28 = 0;
        D_80200140[i].unk_18 = 0;
        D_80200140[i].unk_14 = 0;
        D_80200140[i].unk_258 = 0;
        D_80200140[i].unk_4D = 0;
        D_80200140[i].unk_4E = 0;
        D_80200140[i].unk_250 = 0;
        D_80200140[i].unk_252 = 0;
        D_80200140[i].unk_260 = 0;
        D_80200140[i].unk_21B = 0;
        for (j = 0; j < 4; j++) {
            D_80200140[i].volScales[j] = 0x7F;
        }
        D_80200140[i].volFadeTimer = 1;
        D_80200140[i].fadeVolUpdate = 1;
    }
}

// OoT func_800FAEB4
void func_801A9A74(void) {
    u8 i, j;

    for (i = 0; i < 5; i++) {
        D_80200140[i].volCur = 1.0f;
        D_80200140[i].unk_0C = 0;
        D_80200140[i].fadeVolUpdate = 0;
        for (j = 0; j < 4; j++) {
            D_80200140[i].volScales[j] = 0x7F;
        }
    }
    func_801A99B8();
}

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A7B10/func_801A9B10.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A7B10/func_801A9B78.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A7B10/func_801A9BFC.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A7B10/func_801A9C68.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A7B10/func_801A9D10.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A7B10/func_801A9DCC.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A7B10/func_801A9EA4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/code_801A7B10/func_801A9F4C.s")
