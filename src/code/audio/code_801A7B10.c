#include "global.h"

// TODO: clean up these macros. They are similar to ones in code_800EC960.c but without casts.
#define Audio_StartSeq(playerIdx, fadeTimer, seqId) \
    Audio_QueueSeqCmd(0x00000000 | ((playerIdx) << 24) | ((fadeTimer) << 16) | (seqId))
#define Audio_SeqCmdA(playerIdx, a) Audio_QueueSeqCmd(0xA0000000 | ((playerIdx) << 24) | (a))
#define Audio_SeqCmdB30(playerIdx, a, b) Audio_QueueSeqCmd(0xB0003000 | ((playerIdx) << 24) | ((a) << 16) | (b))
#define Audio_SeqCmdB40(playerIdx, a, b) Audio_QueueSeqCmd(0xB0004000 | ((playerIdx) << 24) | ((a) << 16) | (b))
#define Audio_SeqCmd3(playerIdx, a) Audio_QueueSeqCmd(0x30000000 | ((playerIdx) << 24) | (a))
#define Audio_SeqCmd5(playerIdx, a, b) Audio_QueueSeqCmd(0x50000000 | ((playerIdx) << 24) | ((a) << 16) | (b))
#define Audio_SeqCmd4(playerIdx, a, b) Audio_QueueSeqCmd(0x40000000 | ((playerIdx) << 24) | ((a) << 16) | (b))
#define Audio_SetVolScaleNow(playerIdx, volFadeTimer, volScale) \
    Audio_ProcessSeqCmd(0x40000000 | ((u8)playerIdx << 24) | ((u8)volFadeTimer << 16) | ((u8)(volScale * 127.0f)));

// OoT func_800F9280
void func_801A7B10(u8 playerIdx, u8 seqId, u8 arg2, u16 fadeTimer) {
    u8 i;
    u16 dur;
    s32 pad;

    if (D_801DB4C8 == 0 || playerIdx == 2) {
        arg2 &= 0x7F;
        if (arg2 == 0x7F) {
            dur = (fadeTimer >> 3) * 60 * gAudioContext.audioBufferParameters.updatesPerFrame;
            Audio_QueueCmdS32(0x85000000 | _SHIFTL(playerIdx, 16, 8) | _SHIFTL(seqId, 8, 8), dur);
        } else {
            Audio_QueueCmdS32(0x82000000 | _SHIFTL(playerIdx, 16, 8) | _SHIFTL(seqId, 8, 8),
                              (fadeTimer * (u16)gAudioContext.audioBufferParameters.updatesPerFrame) / 4);
        }

        D_80200140[playerIdx].unk_254 = seqId | (arg2 << 8);
        D_80200140[playerIdx].unk_256 = seqId | (arg2 << 8);
        D_80200140[playerIdx].unk_21B = 1;

        if (D_80200140[playerIdx].volCur != 1.0f) {
            Audio_QueueCmdF32(0x41000000 | _SHIFTL(playerIdx, 16, 8), D_80200140[playerIdx].volCur);
        }

        D_80200140[playerIdx].unk_28 = 0;
        D_80200140[playerIdx].unk_18 = 0;
        D_80200140[playerIdx].unk_14 = 0;

        for (i = 0; i < 0x10; i++) {
            D_80200140[playerIdx].unk_50[i].unk_00 = 1.0f;
            D_80200140[playerIdx].unk_50[i].unk_0C = 0;
            D_80200140[playerIdx].unk_50[i].unk_10 = 1.0f;
            D_80200140[playerIdx].unk_50[i].unk_1C = 0;
        }

        D_80200140[playerIdx].unk_250 = 0;
        D_80200140[playerIdx].unk_252 = 0;
    }
}

// OoT func_800F9474
void func_801A7D04(u8 arg0, u16 arg1) {
    Audio_QueueCmdS32(0x83000000 | ((u8)arg0 << 16),
                      (arg1 * (u16)gAudioContext.audioBufferParameters.updatesPerFrame) / 4);
    D_80200140[arg0].unk_254 = NA_BGM_DISABLED;
}

void Audio_ProcessSeqCmd(u32 cmd) {
    u32 pad;
    s32 new_var;
    u16 fadeTimer;
    u16 channelMask;
    u16 val;
    u8 oldSpec;
    u8 spec;
    u8 op;
    u8 subOp;
    u8 playerIdx;
    u8 seqId;
    u8 seqArgs;
    u8 found;
    u8 port;
    u8 duration;
    u8 chanIdx;
    u8 i;
    f32 freqScale;
    u32 sp4C;

    op = cmd >> 28;
    playerIdx = (cmd & 0x7000000) >> 24;

    switch (op) {
        case 0x0:
            // play sequence immediately
            seqId = cmd & 0xFF;
            seqArgs = (cmd & 0xFF00) >> 8;
            fadeTimer = (cmd & 0xFF0000) >> 13;
            if ((D_80200140[playerIdx].unk_260 == 0) && (D_801DB4C8 == 0)) {
                if (seqArgs < 0x80) {
                    func_801A7B10(playerIdx, seqId, seqArgs, fadeTimer);
                } else {
                    D_80200140[playerIdx].unk_25C = (cmd & 0xF7FF7FFF) + 0x8000000;
                    D_80200140[playerIdx].unk_260 = 1;
                    D_80200140[playerIdx].unk_261[0] = *func_80193C04(seqId, &sp4C);
                    func_801A7D04(playerIdx, 1);
                    if (D_80200140[playerIdx].unk_256 != 0xFFFF) {
                        if (*func_80193C04(seqId, &sp4C) !=
                            *func_80193C04(D_80200140[playerIdx].unk_256 & 0xFF, &sp4C)) {
                            Audio_QueueCmdS32(((seqId & 0xFF) << 8) | 0xF6000000, 0);
                        }
                    }

                    Audio_QueueCmdS8((((*func_80193C04(seqId, &sp4C) & 0xFF)) << 0x10) | 0xF5000000 | 0x1400 |
                                         (((playerIdx + 1) & 0xFF & 0xFF & 0xFF)),
                                     0);
                }
            }
            break;

        case 0x1:
            // disable seq player
            fadeTimer = (cmd & 0xFF0000) >> 13;
            func_801A7D04(playerIdx, fadeTimer);
            break;

        case 0x2:
            // queue sequence
            seqId = cmd & 0xFF;
            seqArgs = (cmd & 0xFF00) >> 8;
            fadeTimer = (cmd & 0xFF0000) >> 13;
            new_var = seqArgs;
            for (i = 0; i < D_801FFD34[playerIdx]; i++) {
                if (D_801FFD00[playerIdx][i].unk_0 == seqId) {
                    if (i == 0) {
                        func_801A7B10(playerIdx, seqId, seqArgs, fadeTimer);
                    }
                    return;
                }
            }

            found = D_801FFD34[playerIdx];
            for (i = 0; i < D_801FFD34[playerIdx]; i++) {
                if (D_801FFD00[playerIdx][i].unk_1 <= new_var) {
                    found = i;
                    i = D_801FFD34[playerIdx]; // "break;"
                }
            }

            if (D_801FFD34[playerIdx] < 5) {
                D_801FFD34[playerIdx]++;
            }
            for (i = D_801FFD34[playerIdx] - 1; i != found; i--) {
                D_801FFD00[playerIdx][i].unk_1 = D_801FFD00[playerIdx][i - 1].unk_1;
                D_801FFD00[playerIdx][i].unk_0 = D_801FFD00[playerIdx][i - 1].unk_0;
            }
            D_801FFD00[playerIdx][found].unk_1 = seqArgs;
            D_801FFD00[playerIdx][found].unk_0 = seqId;

            if (found == 0) {
                func_801A7B10(playerIdx, seqId, seqArgs, fadeTimer);
            }
            break;

        case 0x3:
            // unqueue/stop sequence
            fadeTimer = (cmd & 0xFF0000) >> 13;

            found = D_801FFD34[playerIdx];
            for (i = 0; i < D_801FFD34[playerIdx]; i++) {
                seqId = cmd & 0xFF;
                if (D_801FFD00[playerIdx][i].unk_0 == seqId) {
                    found = i;
                    i = D_801FFD34[playerIdx]; // "break;"
                }
            }

            if (found != D_801FFD34[playerIdx]) {
                for (i = found; i < D_801FFD34[playerIdx] - 1; i++) {
                    D_801FFD00[playerIdx][i].unk_1 = D_801FFD00[playerIdx][i + 1].unk_1;
                    D_801FFD00[playerIdx][i].unk_0 = D_801FFD00[playerIdx][i + 1].unk_0;
                }
                D_801FFD34[playerIdx]--;
            }

            if (found == 0) {
                func_801A7D04(playerIdx, fadeTimer);
                if (D_801FFD34[playerIdx] != 0) {
                    func_801A7B10(playerIdx, D_801FFD00[playerIdx][0].unk_0, D_801FFD00[playerIdx][0].unk_1, fadeTimer);
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
            D_80200140[playerIdx].volTarget = (f32)val / 127.0f;
            if (D_80200140[playerIdx].volCur != D_80200140[playerIdx].volTarget) {
                D_80200140[playerIdx].unk_08 =
                    (D_80200140[playerIdx].volCur - D_80200140[playerIdx].volTarget) / (f32)duration;
                D_80200140[playerIdx].unk_0C = duration;
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
                D_80200140[playerIdx].unk_50[i].unk_14 = freqScale;
                D_80200140[playerIdx].unk_50[i].unk_1C = duration;
                D_80200140[playerIdx].unk_50[i].unk_18 =
                    (D_80200140[playerIdx].unk_50[i].unk_10 - freqScale) / (f32)duration;
            }
            D_80200140[playerIdx].unk_250 = 0xFFFF;
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
            D_80200140[playerIdx].unk_50[chanIdx].unk_14 = freqScale;
            D_80200140[playerIdx].unk_50[chanIdx].unk_18 =
                (D_80200140[playerIdx].unk_50[chanIdx].unk_10 - freqScale) / (f32)duration;
            D_80200140[playerIdx].unk_50[chanIdx].unk_1C = duration;
            D_80200140[playerIdx].unk_250 |= 1 << chanIdx;
            break;

        case 0x6:
            // transition vol scale
            duration = (cmd & 0xFF0000) >> 15;
            chanIdx = (cmd & 0xF00) >> 8;
            val = cmd & 0xFF;
            if (duration == 0) {
                duration++;
            }
            D_80200140[playerIdx].unk_50[chanIdx].unk_04 = (f32)val / 127.0f;
            if (D_80200140[playerIdx].unk_50[chanIdx].unk_00 != D_80200140[playerIdx].unk_50[chanIdx].unk_04) {
                D_80200140[playerIdx].unk_50[chanIdx].unk_08 =
                    (D_80200140[playerIdx].unk_50[chanIdx].unk_00 - D_80200140[playerIdx].unk_50[chanIdx].unk_04) /
                    (f32)duration;
                D_80200140[playerIdx].unk_50[chanIdx].unk_0C = duration;
                D_80200140[playerIdx].unk_252 |= 1 << chanIdx;
            }
            break;

        case 0x7:
            // set global io port
            port = (cmd & 0xFF0000) >> 16;
            val = cmd & 0xFF;
            Audio_QueueCmdS8(0x46000000 | _SHIFTL(playerIdx, 16, 8) | _SHIFTL(port, 0, 8), val);
            break;

        case 0x8:
            // set io port if channel masked
            chanIdx = (cmd & 0xF00) >> 8;
            port = (cmd & 0xFF0000) >> 16;
            val = cmd & 0xFF;
            if ((D_80200140[playerIdx].unk_258 & (1 << chanIdx)) == 0) {
                Audio_QueueCmdS8(0x06000000 | _SHIFTL(playerIdx, 16, 8) | _SHIFTL(chanIdx, 8, 8) | _SHIFTL(port, 0, 8),
                                 val);
            }
            break;

        case 0x9:
            // set channel mask for command 0x8
            D_80200140[playerIdx].unk_258 = cmd & 0xFFFF;
            break;

        case 0xA:
            // set channel stop mask
            channelMask = cmd & 0xFFFF;
            if (channelMask != 0) {
                // with channel mask channelMask...
                Audio_QueueCmdU16(0x90000000 | _SHIFTL(playerIdx, 16, 8), channelMask);
                // stop channels
                Audio_QueueCmdS8(0x08000000 | _SHIFTL(playerIdx, 16, 8) | 0xFF00, 1);
            }
            if ((channelMask ^ 0xFFFF) != 0) {
                // with channel mask ~channelMask...
                Audio_QueueCmdU16(0x90000000 | _SHIFTL(playerIdx, 16, 8), (channelMask ^ 0xFFFF));
                // unstop channels
                Audio_QueueCmdS8(0x08000000 | _SHIFTL(playerIdx, 16, 8) | 0xFF00, 0);
            }
            break;

        case 0xB:
            // update tempo
            D_80200140[playerIdx].unk_14 = cmd;
            break;

        case 0xC:
            // start sequence with setup commands
            subOp = (cmd & 0xF00000) >> 20;
            if (subOp != 0xF) {
                if (D_80200140[playerIdx].unk_4D < 7) {
                    found = D_80200140[playerIdx].unk_4D++;
                    if (found < 8) {
                        D_80200140[playerIdx].unk_2C[found] = cmd;
                        D_80200140[playerIdx].unk_4C = 2;
                    }
                }
            } else {
                D_80200140[playerIdx].unk_4D = 0;
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
                    D_801DB4C8 = (D_801DB4C8 & 0xFE) | (u8)(val & 1);
                    break;
                case 2:
                    // set sequence starting disabled?
                    D_801DB4C8 = (D_801DB4C8 & 0xFD) | (u8)((val & 1) * 2);
                    break;
            }
            break;

        case 0xF:
            // change spec
            spec = cmd & 0xFF;
            port = (cmd & 0xFF0000) >> 16;
            if (port == 0) {
                gSfxChannelLayout = (cmd & 0xFF00) >> 8;
                oldSpec = gAudioSpecId;
                gAudioSpecId = spec;
                func_80193D08(spec);
                func_801A4DF4(oldSpec);
                Audio_QueueCmdS32(0xF8000000, 0);
            } else {
                D_801DB4DC = cmd;
                D_80200BCE = 0x7FFF;
                D_80200BCC = 0x14;
                D_80200BD0 = 0x666;
            }
            break;
    }
}

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

    if (D_80200140[playerIdx].unk_254 != NA_BGM_DISABLED) {
        return D_80200140[playerIdx].unk_254;
    }

    return NA_BGM_DISABLED;
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

void Audio_SetVolScale(u8 playerIdx, u8 scaleIdx, u8 targetVol, u8 volFadeTimer) {
    f32 volScale;
    u8 i;

    D_80200140[playerIdx].volScales[scaleIdx] = targetVol & 0x7F;

    if (volFadeTimer != 0) {
        D_80200140[playerIdx].fadeVolUpdate = 1;
        D_80200140[playerIdx].volFadeTimer = volFadeTimer;
    } else {
        for (i = 0, volScale = 1.0f; i < 4; i++) {
            volScale *= D_80200140[playerIdx].volScales[i] / 127.0f;
        }

        Audio_SetVolScaleNow(playerIdx, volFadeTimer, volScale);
    }
}

void func_801A8D5C(void) {
    u32 temp_a1;
    u16 temp_lo;
    u16 temp_v1;
    u16 phi_a2;
    u8 temp_v0_4;
    u8 temp_a0;
    u8 temp_s1;
    u8 temp_s0_3;
    u8 temp_a3_3;
    s32 pad[2];
    u16 new_var;
    u32 sp70;
    f32 phi_f0;
    u8 phi_t0;
    u8 i;
    u8 j;
    u8 k;

    for (i = 0; i < 5; i++) {

        if ((D_80200140[i].unk_21B != 0) && gAudioContext.seqPlayers[i].enabled) {
            D_80200140[i].unk_21B = 0;
        }

        if ((func_801A8A50(i) != 0xFFFF) && !gAudioContext.seqPlayers[i].enabled && (D_80200140[i].unk_21B == 0)) {
            D_80200140[i].unk_254 = 0xFFFF;
        }

        if (D_80200140[i].unk_260 != 0) {
            switch ((s32)func_80193BA0(&sp70)) {
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                    D_80200140[i].unk_260 = 0;
                    Audio_ProcessSeqCmd(D_80200140[i].unk_25C);
                    break;
                case 0xFF:
                    D_80200140[i].unk_260 = 0;
                    break;
            }
        }

        if (D_80200140[i].fadeVolUpdate) {
            phi_f0 = 1.0f;
            for (j = 0; j < 4; j++) {
                phi_f0 *= (D_80200140[i].volScales[j] / 127.0f);
            }

            Audio_SeqCmd4((u8)(i + 8), D_80200140[i].volFadeTimer, (u8)(phi_f0 * 127.0f));
            D_80200140[i].fadeVolUpdate = 0;
        }

        if (D_80200140[i].unk_0C != 0) {
            D_80200140[i].unk_0C--;

            if (D_80200140[i].unk_0C != 0) {
                D_80200140[i].volCur = D_80200140[i].volCur - D_80200140[i].unk_08;
            } else {
                D_80200140[i].volCur = D_80200140[i].volTarget;
            }

            Audio_QueueCmdF32(0x41000000 | _SHIFTL(i, 16, 8), D_80200140[i].volCur);
        }

        if (D_80200140[i].unk_14 != 0) {
            temp_a1 = D_80200140[i].unk_14;
            phi_t0 = (temp_a1 & 0xFF0000) >> 15;
            phi_a2 = temp_a1 & 0xFFF;
            if (phi_t0 == 0) {
                phi_t0++;
            }

            if (gAudioContext.seqPlayers[i].enabled) {
                temp_lo = gAudioContext.seqPlayers[i].tempo / 0x30;
                temp_v0_4 = (temp_a1 & 0xF000) >> 12;
                switch (temp_v0_4) {
                    case 1:
                        phi_a2 += temp_lo;
                        break;
                    case 2:
                        if (phi_a2 < temp_lo) {
                            phi_a2 = temp_lo - phi_a2;
                        }
                        break;
                    case 3:
                        phi_a2 = temp_lo * (phi_a2 / 100.0f);
                        break;
                    case 4:
                        if (D_80200140[i].unk_18) {
                            phi_a2 = D_80200140[i].unk_18;
                        } else {
                            phi_a2 = temp_lo;
                        }
                        break;
                }

                if (D_80200140[i].unk_18 == 0) {
                    D_80200140[i].unk_18 = temp_lo;
                }

                D_80200140[i].unk_20 = phi_a2;
                D_80200140[i].unk_1C = gAudioContext.seqPlayers[i].tempo / 0x30;
                D_80200140[i].unk_24 = (D_80200140[i].unk_1C - D_80200140[i].unk_20) / phi_t0;
                D_80200140[i].unk_28 = phi_t0;
                D_80200140[i].unk_14 = 0;
            }
        }

        if (D_80200140[i].unk_28 != 0) {
            D_80200140[i].unk_28--;
            if (D_80200140[i].unk_28 != 0) {
                D_80200140[i].unk_1C = D_80200140[i].unk_1C - D_80200140[i].unk_24;
            } else {
                D_80200140[i].unk_1C = D_80200140[i].unk_20;
            }
            // set tempo
            Audio_QueueCmdS32(0x47000000 | _SHIFTL(i, 16, 8), D_80200140[i].unk_1C);
        }

        if (D_80200140[i].unk_252 != 0) {
            for (k = 0; k < 0x10; k++) {
                if (D_80200140[i].unk_50[k].unk_0C != 0) {
                    D_80200140[i].unk_50[k].unk_0C--;
                    if (D_80200140[i].unk_50[k].unk_0C != 0) {
                        D_80200140[i].unk_50[k].unk_00 -= D_80200140[i].unk_50[k].unk_08;
                    } else {
                        D_80200140[i].unk_50[k].unk_00 = D_80200140[i].unk_50[k].unk_04;
                        D_80200140[i].unk_252 ^= (1 << k);
                    }
                    // CHAN_UPD_VOL_SCALE (i = seq, k = chan)
                    Audio_QueueCmdF32(0x01000000 | _SHIFTL(i, 16, 8) | _SHIFTL(k, 8, 8),
                                      D_80200140[i].unk_50[k].unk_00);
                }
            }
        }

        if (D_80200140[i].unk_250 != 0) {
            for (k = 0; k < 0x10; k++) {
                if (D_80200140[i].unk_50[k].unk_1C != 0) {
                    D_80200140[i].unk_50[k].unk_1C--;
                    if (D_80200140[i].unk_50[k].unk_1C != 0) {
                        D_80200140[i].unk_50[k].unk_10 -= D_80200140[i].unk_50[k].unk_18;
                    } else {
                        D_80200140[i].unk_50[k].unk_10 = D_80200140[i].unk_50[k].unk_14;
                        D_80200140[i].unk_250 ^= (1 << k);
                    }
                    // CHAN_UPD_FREQ_SCALE
                    Audio_QueueCmdF32(0x04000000 | _SHIFTL(i, 16, 8) | _SHIFTL(k, 8, 8),
                                      D_80200140[i].unk_50[k].unk_10);
                }
            }
        }

        if (D_80200140[i].unk_4D != 0) {
            if (func_801A8ABC(0xF0000000, 0xF0000000) == 0) {
                D_80200140[i].unk_4D = 0;
                return;
            }

            if (D_80200140[i].unk_4C != 0) {
                D_80200140[i].unk_4C--;
                continue;
            }

            if (gAudioContext.seqPlayers[i].enabled) {
                continue;
            }

            for (j = 0; j < D_80200140[i].unk_4D; j++) {
                temp_a0 = (D_80200140[i].unk_2C[j] & 0x00F00000) >> 20;
                temp_s1 = (D_80200140[i].unk_2C[j] & 0x000F0000) >> 16;
                temp_s0_3 = (D_80200140[i].unk_2C[j] & 0xFF00) >> 8;
                temp_a3_3 = D_80200140[i].unk_2C[j] & 0xFF;

                switch (temp_a0) {
                    case 0:
                        Audio_SetVolScale(temp_s1, 1, 0x7F, temp_a3_3);
                        break;
                    case 7:
                        if (D_801FFD34[i] == temp_a3_3) {
                            Audio_SetVolScale(temp_s1, 1, 0x7F, temp_s0_3);
                        }
                        break;
                    case 1:
                        Audio_SeqCmd3((u8)(i + 8), D_80200140[i].unk_254);
                        break;
                    case 2:
                        Audio_StartSeq((u8)(temp_s1 + 8), 1, D_80200140[temp_s1].unk_254);
                        D_80200140[temp_s1].fadeVolUpdate = 1;
                        D_80200140[temp_s1].volScales[1] = 0x7F;
                        break;
                    case 3:
                        Audio_SeqCmdB30((u8)(temp_s1 + 8), temp_s0_3, temp_a3_3);
                        break;
                    case 4:
                        Audio_SeqCmdB40((u8)(temp_s1 + 8), temp_a3_3, 0);
                        break;
                    case 5:
                        temp_v1 = D_80200140[i].unk_2C[j] & 0xFFFF;
                        Audio_StartSeq((u8)(temp_s1 + 8), D_80200140[temp_s1].unk_4E, temp_v1);
                        Audio_SetVolScale(temp_s1, 1, 0x7F, 0);
                        D_80200140[temp_s1].unk_4E = 0;
                        break;
                    case 6:
                        D_80200140[i].unk_4E = temp_s0_3;
                        break;
                    case 8:
                        Audio_SetVolScale(temp_s1, temp_s0_3, 0x7F, temp_a3_3);
                        break;
                    case 14:
                        if (temp_a3_3 & 1) {
                            Audio_QueueCmdS32(0xE3000000, 0);
                        }
                        if (temp_a3_3 & 2) {
                            Audio_QueueCmdS32(0xE3000000, 1);
                        }
                        if (temp_a3_3 & 4) {
                            Audio_QueueCmdS32(0xE3000000, 2);
                        }
                        break;
                    case 9:
                        temp_v1 = D_80200140[i].unk_2C[j] & 0xFFFF;
                        Audio_SeqCmdA((u8)(temp_s1 + 8), temp_v1);
                        break;
                    case 10:
                        Audio_SeqCmd5((u8)(temp_s1 + 8), temp_s0_3, (temp_a3_3 * 10) & 0xFFFF);
                        break;
                }
            }

            D_80200140[i].unk_4D = 0;
        }
    }
}

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

u8 func_801A982C(void) {
    u8 ret;
    u8 temp_v0;
    u8 temp_v1_2;
    u8 phi_s1_2;
    u8 new_var;

    ret = false;
    temp_v0 = ((D_801DB4DC & 0xFF0000) >> 0x10);
    phi_s1_2 = 0;
    if (D_801DB4DC != 0) {
        temp_v1_2 = D_80200BCC;
        if (temp_v1_2) {}
        D_80200BCC--;
        if (temp_v1_2 != 0) {
            for (; temp_v0 != 0;) {
                if ((temp_v0 & 1) != 0) {
                    Audio_QueueCmdS32(((phi_s1_2 & 0xFF) << 8) | 0xE6040000, D_80200BCE);
                    Audio_ScheduleProcessCmds();
                }
                phi_s1_2++;
                temp_v0 = (temp_v0 >> 1);
            }

            D_80200BCE -= D_80200BD0;
            ret = true;
        } else {
            for (; temp_v0 != 0;) {
                new_var = phi_s1_2;
                if ((temp_v0 & 1) != 0) {
                    Audio_QueueCmdS32(((phi_s1_2 & 0xFF) << 8) | 0xE6000000,
                                      D_801DB930[0][D_801DB4DC & 0xFF & 0xFF] + (new_var * 0x18));
                    Audio_ScheduleProcessCmds();
                }
                phi_s1_2++;
                temp_v0 = (temp_v0 >> 1);
            }

            D_801DB4DC = 0;
            Audio_QueueCmdS8(0x46020000, gSfxChannelLayout);
            func_801A4D50();
        }
    }

    return ret;
}

// OoT func_800FADF8
void func_801A99B8(void) {
    u8 i, j;

    for (i = 0; i < 5; i++) {
        D_801FFD34[i] = 0;
        D_80200140[i].unk_254 = NA_BGM_DISABLED;
        D_80200140[i].unk_256 = NA_BGM_DISABLED;
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
