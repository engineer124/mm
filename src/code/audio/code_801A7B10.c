#include "global.h"

void func_801A4DA4(void);

u8 sSeqCmdWritePos = 0;
u8 sSeqCmdReadPos = 0;
u8 sIsSeqStartDisabled = 0;
u8 sSoundModes[] = {
    AUDIO_MODE_STEREO, 
    AUDIO_MODE_HEADSET, 
    AUDIO_MODE_UNK, 
    AUDIO_MODE_MONO, 
    AUDIO_MODE_SURROUND,
};
u8 gAudioSpecId = 0;
u8 D_801DB4D8 = 0;
u32 sChangeSpecCmd = 0;

void Audio_StartSequence(u8 playerIdx, u8 seqId, u8 seqArgs, u16 fadeTimer) {
    u8 channelIdx;
    u16 duration;
    s32 pad;

    if (sIsSeqStartDisabled == 0 || playerIdx == SEQ_PLAYER_SFX) {
        seqArgs &= 0x7F;
        if (seqArgs == 0x7F) {
            duration = (fadeTimer >> 3) * 60 * gAudioContext.audioBufferParameters.updatesPerFrame;
            Audio_QueueCmdS32(0x85000000 | _SHIFTL(playerIdx, 16, 8) | _SHIFTL(seqId, 8, 8), duration);
        } else {
            Audio_QueueCmdS32(0x82000000 | _SHIFTL(playerIdx, 16, 8) | _SHIFTL(seqId, 8, 8),
                              (fadeTimer * (u16)gAudioContext.audioBufferParameters.updatesPerFrame) / 4);
        }

        gActiveSeqs[playerIdx].seqId = seqId | (seqArgs << 8);
        gActiveSeqs[playerIdx].prevSeqId = seqId | (seqArgs << 8);
        gActiveSeqs[playerIdx].isSeqPlayerInit = true;

        if (gActiveSeqs[playerIdx].volCur != 1.0f) {
            Audio_QueueCmdF32(0x41000000 | _SHIFTL(playerIdx, 16, 8), gActiveSeqs[playerIdx].volCur);
        }

        gActiveSeqs[playerIdx].tempoDuration = 0;
        gActiveSeqs[playerIdx].tempoDefault = 0;
        gActiveSeqs[playerIdx].tempoCmd = 0;

        for (channelIdx = 0; channelIdx < ARRAY_COUNT(gActiveSeqs[playerIdx].channelData); channelIdx++) {
            gActiveSeqs[playerIdx].channelData[channelIdx].volCur = 1.0f;
            gActiveSeqs[playerIdx].channelData[channelIdx].volDuration = 0;
            gActiveSeqs[playerIdx].channelData[channelIdx].freqScaleCur = 1.0f;
            gActiveSeqs[playerIdx].channelData[channelIdx].freqScaleDuration = 0;
        }

        gActiveSeqs[playerIdx].freqScaleChannelFlags = 0;
        gActiveSeqs[playerIdx].volChannelFlags = 0;
    }
}

void Audio_StopSequence(u8 playerIdx, u16 fadeTimer) {
    Audio_QueueCmdS32(0x83000000 | ((u8)playerIdx << 16),
                      (fadeTimer * (u16)gAudioContext.audioBufferParameters.updatesPerFrame) / 4);
    gActiveSeqs[playerIdx].seqId = NA_BGM_DISABLED;
}

void Audio_ProcessSeqCmd(u32 cmd) {
    s32 importance;
    s32 channelMaskN;
    u16 channelMask;
    u16 fadeTimer;
    u16 val;
    u8 oldSpecId;
    u8 specId;
    u8 specMode;
    u8 op;
    u8 subOp;
    u8 playerIdx;
    u8 seqId;
    u8 seqArgs;
    u8 found;
    u8 port;
    u8 duration;
    u8 channelIdx;
    u8 i;
    f32 freqScaleTarget;
    u32 outNumFonts;

    op = cmd >> 28;
    playerIdx = (cmd & 0x7000000) >> 24;

    switch (op) {
        case 0x0:
            // play sequence immediately
            seqId = cmd & 0xFF;
            seqArgs = (cmd & 0xFF00) >> 8;
            fadeTimer = (cmd & 0xFF0000) >> 13;
            if (!gActiveSeqs[playerIdx].isWaitingForFonts && (sIsSeqStartDisabled == 0)) {
                if (seqArgs < 0x80) {
                    Audio_StartSequence(playerIdx, seqId, seqArgs, fadeTimer);
                } else {
                    // This case is taken for the new 0x8000 flag introduced in MM

                    // Store the cmd to be called again once the fonts are loaded
                    // but changes the command so that next time, the (seqArgs < 0x80) case is taken
                    gActiveSeqs[playerIdx].startSeqCmd = (cmd & ~0x8008000) + 0x8000000;
                    gActiveSeqs[playerIdx].isWaitingForFonts = true;
                    gActiveSeqs[playerIdx].fontId = *AudioCmd_GetFontsForSequence(seqId, &outNumFonts);
                    Audio_StopSequence(playerIdx, 1);

                    if (gActiveSeqs[playerIdx].prevSeqId != NA_BGM_DISABLED) {
                        if (*AudioCmd_GetFontsForSequence(seqId, &outNumFonts) !=
                            *AudioCmd_GetFontsForSequence(gActiveSeqs[playerIdx].prevSeqId & 0xFF, &outNumFonts)) {
                            // Discard Seq Fonts
                            Audio_QueueCmdS32(((seqId & 0xFF) << 8) | 0xF6000000, 0);
                        }
                    }

                    Audio_QueueCmdS8((((*AudioCmd_GetFontsForSequence(seqId, &outNumFonts) & 0xFF)) << 0x10) |
                                         0xF5000000 | 0x1400 | (((playerIdx + 1) & 0xFF & 0xFF & 0xFF)),
                                     0);
                }
            }
            break;

        case 0x1:
            // disable seq player
            fadeTimer = (cmd & 0xFF0000) >> 13;
            Audio_StopSequence(playerIdx, fadeTimer);
            break;

        case 0x2:
            // queue sequence
            seqId = cmd & 0xFF;
            seqArgs = (cmd & 0xFF00) >> 8;
            fadeTimer = (cmd & 0xFF0000) >> 13;
            importance = seqArgs;

            // Checks if the requested sequence is first in the list of requested
            // If it is already queued and first in the list, then play the sequence immediately
            for (i = 0; i < sNumSeqRequests[playerIdx]; i++) {
                if (sSeqRequests[playerIdx][i].seqId == seqId) {
                    if (i == 0) {
                        Audio_StartSequence(playerIdx, seqId, seqArgs, fadeTimer);
                    }
                    return;
                }
            }

            // Searches the sequence requests for the first request that does not have a higher priority
            // than the current incoming request
            found = sNumSeqRequests[playerIdx];
            for (i = 0; i < sNumSeqRequests[playerIdx]; i++) {
                if (sSeqRequests[playerIdx][i].importance <= importance) {
                    found = i;
                    i = sNumSeqRequests[playerIdx]; // "break;"
                }
            }

            if (sNumSeqRequests[playerIdx] < 5) {
                sNumSeqRequests[playerIdx]++;
            }

            for (i = sNumSeqRequests[playerIdx] - 1; i != found; i--) {
                sSeqRequests[playerIdx][i].importance = sSeqRequests[playerIdx][i - 1].importance;
                sSeqRequests[playerIdx][i].seqId = sSeqRequests[playerIdx][i - 1].seqId;
            }
            sSeqRequests[playerIdx][found].importance = seqArgs;
            sSeqRequests[playerIdx][found].seqId = seqId;

            if (found == 0) {
                Audio_StartSequence(playerIdx, seqId, seqArgs, fadeTimer);
            }
            break;

        case 0x3:
            // unqueue/stop sequence
            fadeTimer = (cmd & 0xFF0000) >> 13;

            found = sNumSeqRequests[playerIdx];
            for (i = 0; i < sNumSeqRequests[playerIdx]; i++) {
                seqId = cmd & 0xFF;
                if (sSeqRequests[playerIdx][i].seqId == seqId) {
                    found = i;
                    i = sNumSeqRequests[playerIdx]; // "break;"
                }
            }

            if (found != sNumSeqRequests[playerIdx]) {
                for (i = found; i < sNumSeqRequests[playerIdx] - 1; i++) {
                    sSeqRequests[playerIdx][i].importance = sSeqRequests[playerIdx][i + 1].importance;
                    sSeqRequests[playerIdx][i].seqId = sSeqRequests[playerIdx][i + 1].seqId;
                }
                sNumSeqRequests[playerIdx]--;
            }

            if (found == 0) {
                // Stop current sequence
                Audio_StopSequence(playerIdx, fadeTimer);
                if (sNumSeqRequests[playerIdx] != 0) {
                    // Play the next sequence in the queue
                    Audio_StartSequence(playerIdx, sSeqRequests[playerIdx][0].seqId,
                                        sSeqRequests[playerIdx][0].importance, fadeTimer);
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
            gActiveSeqs[playerIdx].volTarget = (f32)val / 127.0f;
            if (gActiveSeqs[playerIdx].volCur != gActiveSeqs[playerIdx].volTarget) {
                gActiveSeqs[playerIdx].volVelocity =
                    (gActiveSeqs[playerIdx].volCur - gActiveSeqs[playerIdx].volTarget) / (f32)duration;
                gActiveSeqs[playerIdx].volDuration = duration;
            }
            break;

        case 0x5:
            // transition freq scale for all channels
            duration = (cmd & 0xFF0000) >> 15;
            val = cmd & 0xFFFF;
            if (duration == 0) {
                duration++;
            }
            freqScaleTarget = (f32)val / 1000.0f;
            for (i = 0; i < ARRAY_COUNT(gActiveSeqs[playerIdx].channelData); i++) {
                gActiveSeqs[playerIdx].channelData[i].freqScaleTarget = freqScaleTarget;
                gActiveSeqs[playerIdx].channelData[i].freqScaleDuration = duration;
                gActiveSeqs[playerIdx].channelData[i].freqScaleVelocity =
                    (gActiveSeqs[playerIdx].channelData[i].freqScaleCur - freqScaleTarget) / (f32)duration;
            }
            gActiveSeqs[playerIdx].freqScaleChannelFlags = 0xFFFF;
            break;

        case 0xD:
            // transition freq scale
            duration = (cmd & 0xFF0000) >> 15;
            channelIdx = (cmd & 0xF000) >> 12;
            val = cmd & 0xFFF;
            if (duration == 0) {
                duration++;
            }
            freqScaleTarget = (f32)val / 1000.0f;
            gActiveSeqs[playerIdx].channelData[channelIdx].freqScaleTarget = freqScaleTarget;
            gActiveSeqs[playerIdx].channelData[channelIdx].freqScaleVelocity =
                (gActiveSeqs[playerIdx].channelData[channelIdx].freqScaleCur - freqScaleTarget) / (f32)duration;
            gActiveSeqs[playerIdx].channelData[channelIdx].freqScaleDuration = duration;
            gActiveSeqs[playerIdx].freqScaleChannelFlags |= 1 << channelIdx;
            break;

        case 0x6:
            // transition vol scale
            duration = (cmd & 0xFF0000) >> 15;
            channelIdx = (cmd & 0xF00) >> 8;
            val = cmd & 0xFF;
            if (duration == 0) {
                duration++;
            }
            gActiveSeqs[playerIdx].channelData[channelIdx].volTarget = (f32)val / 127.0f;
            if (gActiveSeqs[playerIdx].channelData[channelIdx].volCur !=
                gActiveSeqs[playerIdx].channelData[channelIdx].volTarget) {
                gActiveSeqs[playerIdx].channelData[channelIdx].volVelocity =
                    (gActiveSeqs[playerIdx].channelData[channelIdx].volCur -
                     gActiveSeqs[playerIdx].channelData[channelIdx].volTarget) /
                    (f32)duration;
                gActiveSeqs[playerIdx].channelData[channelIdx].volDuration = duration;
                gActiveSeqs[playerIdx].volChannelFlags |= 1 << channelIdx;
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
            channelIdx = (cmd & 0xF00) >> 8;
            port = (cmd & 0xFF0000) >> 16;
            val = cmd & 0xFF;
            if ((gActiveSeqs[playerIdx].channelPortMask & (1 << channelIdx)) == 0) {
                Audio_QueueCmdS8(
                    0x06000000 | _SHIFTL(playerIdx, 16, 8) | _SHIFTL(channelIdx, 8, 8) | _SHIFTL(port, 0, 8), val);
            }
            break;

        case 0x9:
            // set channel mask for command 0x8
            gActiveSeqs[playerIdx].channelPortMask = cmd & 0xFFFF;
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
            channelMaskN = channelMask ^ 0xFFFF;
            if (channelMaskN != 0) {
                // with channel mask ~channelMask...
                Audio_QueueCmdU16(0x90000000 | _SHIFTL(playerIdx, 16, 8), channelMaskN);
                // unstop channels
                Audio_QueueCmdS8(0x08000000 | _SHIFTL(playerIdx, 16, 8) | 0xFF00, 0);
            }
            break;

        case 0xB:
            // update tempo
            gActiveSeqs[playerIdx].tempoCmd = cmd;
            break;

        case 0xC:
            // start sequence with setup commands
            subOp = (cmd & 0xF00000) >> 20;
            if (subOp != 0xF) {
                if (gActiveSeqs[playerIdx].numSetupCmd < 7) {
                    found = gActiveSeqs[playerIdx].numSetupCmd++;
                    if (found < 8) {
                        gActiveSeqs[playerIdx].setupCmd[found] = cmd;
                        gActiveSeqs[playerIdx].setupCmdTimer = 2;
                    }
                }
            } else {
                gActiveSeqs[playerIdx].numSetupCmd = 0;
            }
            break;

        case 0xE:
            subOp = (cmd & 0xF00) >> 8;
            val = cmd & 0xFF;
            switch (subOp) {
                case 0:
                    // set sound mode
                    Audio_QueueCmdS32(0xF0000000, sSoundModes[val]);
                    break;
                case 1:
                    // set sequence disabled true in (sIsSeqStartDisabled & 1) bit
                    sIsSeqStartDisabled = (sIsSeqStartDisabled & 0xFE) | (u8)(val & 1);
                    break;
                case 2:
                    // set sequence disabled true in (sIsSeqStartDisabled & 2) bit
                    sIsSeqStartDisabled = (sIsSeqStartDisabled & 0xFD) | (u8)((val & 1) * 2);
                    break;
            }
            break;

        case 0xF:
            // change spec
            specId = cmd & 0xFF;
            specMode = (cmd & 0xFF0000) >> 16;
            if (specMode == 0) {
                gSfxChannelLayout = (cmd & 0xFF00) >> 8;
                oldSpecId = gAudioSpecId;
                gAudioSpecId = specId;
                func_80193D08(specId);
                func_801A4DF4(oldSpecId);
                Audio_QueueCmdS32(0xF8000000, 0);
            } else {
                sChangeSpecCmd = cmd;
                D_80200BCE = 0x7FFF;
                D_80200BCC = 0x14;
                D_80200BD0 = 0x666;
            }
            break;
    }
}

void Audio_QueueSeqCmd(u32 cmd) {
    sAudioSeqCmds[sSeqCmdWritePos++] = cmd;
}

void Audio_ProcessSeqCmds(void) {
    while (sSeqCmdWritePos != sSeqCmdReadPos) {
        Audio_ProcessSeqCmd(sAudioSeqCmds[sSeqCmdReadPos++]);
    }
}

u16 Audio_GetActiveSequence(u8 playerIdx) {
    if (gActiveSeqs[playerIdx].isWaitingForFonts == true) {
        return gActiveSeqs[playerIdx].startSeqCmd & 0xFF;
    }

    if (gActiveSeqs[playerIdx].seqId != NA_BGM_DISABLED) {
        return gActiveSeqs[playerIdx].seqId;
    }

    return NA_BGM_DISABLED;
}

s32 Audio_IsSeqCmdNotQueued(u32 cmdVal, u32 cmdMask) {
    u8 i;

    for (i = sSeqCmdReadPos; i != sSeqCmdWritePos; i++) {
        if ((sAudioSeqCmds[i] & cmdMask) == cmdVal) {
            return false;
        }
    }

    return true;
}

// Unused
void Audio_ResetSequenceRequests(u8 playerIdx) {
    sNumSeqRequests[playerIdx] = 0;
}

// Unused
void Audio_DisableSeqCmdSetupOp(u8 playerIdx, u8 setupOpDisabled) {
    u8 i;

    for (i = 0; i < gActiveSeqs[playerIdx].numSetupCmd; i++) {
        u8 setupOp = (gActiveSeqs[playerIdx].setupCmd[i] & 0xF00000) >> 20;

        if (setupOp == setupOpDisabled) {
            gActiveSeqs[playerIdx].setupCmd[i] = 0xFF000000;
        }
    }
}

void Audio_SetVolumeScale(u8 playerIdx, u8 scaleIdx, u8 targetVol, u8 volFadeTimer) {
    f32 volScale;
    u8 i;

    gActiveSeqs[playerIdx].volScales[scaleIdx] = targetVol & 0x7F;

    if (volFadeTimer != 0) {
        gActiveSeqs[playerIdx].fadeVolUpdate = 1;
        gActiveSeqs[playerIdx].volFadeTimer = volFadeTimer;
    } else {
        for (i = 0, volScale = 1.0f; i < 4; i++) {
            volScale *= gActiveSeqs[playerIdx].volScales[i] / 127.0f;
        }

        Audio_SetVolumeScaleNow(playerIdx, volFadeTimer, volScale);
    }
}

/**
 * Apply various high-level commands to the active sequences
 */
void Audio_UpdateActiveSequences(void) {
    u32 tempoCmd;
    u16 tempoDefault;
    u16 seqId;
    u16 channelMask;
    u16 tempoTarget;
    u8 setupOp;
    u8 setupPlayerIdx;
    u8 setupVal2;
    u8 setupVal1;
    u8 tempoOp;
    s32 pad[2];
    u32 dummy;
    f32 volume;
    u8 tempoDuration;
    u8 playerIdx;
    u8 j;
    u8 channelIdx;

    for (playerIdx = 0; playerIdx < ARRAY_COUNT(gActiveSeqs); playerIdx++) {

        // The seqPlayer has finished initializing and is currently playing the active sequences
        if (gActiveSeqs[playerIdx].isSeqPlayerInit && gAudioContext.seqPlayers[playerIdx].enabled) {
            gActiveSeqs[playerIdx].isSeqPlayerInit = false;
        }

        // The seqPlayer is no longer playing the active sequences
        if ((Audio_GetActiveSequence(playerIdx) != NA_BGM_DISABLED) && !gAudioContext.seqPlayers[playerIdx].enabled &&
            (!gActiveSeqs[playerIdx].isSeqPlayerInit)) {
            gActiveSeqs[playerIdx].seqId = NA_BGM_DISABLED;
        }

        // Check if the requested sequences is waiting for fonts to load
        if (gActiveSeqs[playerIdx].isWaitingForFonts) {
            switch ((s32)func_80193BA0(&dummy)) {
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                    // The fonts have been loaded successfully.
                    gActiveSeqs[playerIdx].isWaitingForFonts = false;
                    // Queue the same command that was stored previously, but without the 0x8000
                    Audio_ProcessSeqCmd(gActiveSeqs[playerIdx].startSeqCmd);
                    break;
                case 0xFF:
                    // There was an error in loading the fonts
                    gActiveSeqs[playerIdx].isWaitingForFonts = false;
                    break;
            }
        }

        if (gActiveSeqs[playerIdx].fadeVolUpdate) {
            volume = 1.0f;
            for (j = 0; j < 4; j++) {
                volume *= (gActiveSeqs[playerIdx].volScales[j] / 127.0f);
            }

            AudioSeqCmd_SetPlayerVol((u8)(playerIdx + 8), gActiveSeqs[playerIdx].volFadeTimer, (u8)(volume * 127.0f));
            gActiveSeqs[playerIdx].fadeVolUpdate = 0;
        }

        if (gActiveSeqs[playerIdx].volDuration != 0) {
            gActiveSeqs[playerIdx].volDuration--;

            if (gActiveSeqs[playerIdx].volDuration != 0) {
                gActiveSeqs[playerIdx].volCur -= gActiveSeqs[playerIdx].volVelocity;
            } else {
                gActiveSeqs[playerIdx].volCur = gActiveSeqs[playerIdx].volTarget;
            }

            Audio_QueueCmdF32(0x41000000 | _SHIFTL(playerIdx, 16, 8), gActiveSeqs[playerIdx].volCur);
        }

        if (gActiveSeqs[playerIdx].tempoCmd != 0) {
            tempoCmd = gActiveSeqs[playerIdx].tempoCmd;
            tempoDuration = (tempoCmd & 0xFF0000) >> 15;
            tempoTarget = tempoCmd & 0xFFF;
            if (tempoDuration == 0) {
                tempoDuration++;
            }

            if (gAudioContext.seqPlayers[playerIdx].enabled) {
                tempoDefault = gAudioContext.seqPlayers[playerIdx].tempo / 0x30;
                tempoOp = (tempoCmd & 0xF000) >> 12;
                switch (tempoOp) {
                    case 1:
                        tempoTarget += tempoDefault;
                        break;
                    case 2:
                        if (tempoTarget < tempoDefault) {
                            tempoTarget = tempoDefault - tempoTarget;
                        }
                        break;
                    case 3:
                        tempoTarget = tempoDefault * (tempoTarget / 100.0f);
                        break;
                    case 4:
                        if (gActiveSeqs[playerIdx].tempoDefault) {
                            tempoTarget = gActiveSeqs[playerIdx].tempoDefault;
                        } else {
                            tempoTarget = tempoDefault;
                        }
                        break;
                }

                if (gActiveSeqs[playerIdx].tempoDefault == 0) {
                    gActiveSeqs[playerIdx].tempoDefault = tempoDefault;
                }

                gActiveSeqs[playerIdx].tempoTarget = tempoTarget;
                gActiveSeqs[playerIdx].tempoCur = gAudioContext.seqPlayers[playerIdx].tempo / 0x30;
                gActiveSeqs[playerIdx].tempoVelocity =
                    (gActiveSeqs[playerIdx].tempoCur - gActiveSeqs[playerIdx].tempoTarget) / tempoDuration;
                gActiveSeqs[playerIdx].tempoDuration = tempoDuration;
                gActiveSeqs[playerIdx].tempoCmd = 0;
            }
        }

        if (gActiveSeqs[playerIdx].tempoDuration != 0) {
            gActiveSeqs[playerIdx].tempoDuration--;
            if (gActiveSeqs[playerIdx].tempoDuration != 0) {
                gActiveSeqs[playerIdx].tempoCur -= gActiveSeqs[playerIdx].tempoVelocity;
            } else {
                gActiveSeqs[playerIdx].tempoCur = gActiveSeqs[playerIdx].tempoTarget;
            }
            // set tempo
            Audio_QueueCmdS32(0x47000000 | _SHIFTL(playerIdx, 16, 8), gActiveSeqs[playerIdx].tempoCur);
        }

        if (gActiveSeqs[playerIdx].volChannelFlags != 0) {
            for (channelIdx = 0; channelIdx < 16; channelIdx++) {
                if (gActiveSeqs[playerIdx].channelData[channelIdx].volDuration != 0) {
                    gActiveSeqs[playerIdx].channelData[channelIdx].volDuration--;
                    if (gActiveSeqs[playerIdx].channelData[channelIdx].volDuration != 0) {
                        gActiveSeqs[playerIdx].channelData[channelIdx].volCur -=
                            gActiveSeqs[playerIdx].channelData[channelIdx].volVelocity;
                    } else {
                        gActiveSeqs[playerIdx].channelData[channelIdx].volCur =
                            gActiveSeqs[playerIdx].channelData[channelIdx].volTarget;
                        gActiveSeqs[playerIdx].volChannelFlags ^= (1 << channelIdx);
                    }
                    // CHAN_UPD_VOL_SCALE (playerIdx = seq, channelIdx = chan)
                    Audio_QueueCmdF32(0x01000000 | _SHIFTL(playerIdx, 16, 8) | _SHIFTL(channelIdx, 8, 8),
                                      gActiveSeqs[playerIdx].channelData[channelIdx].volCur);
                }
            }
        }

        if (gActiveSeqs[playerIdx].freqScaleChannelFlags != 0) {
            for (channelIdx = 0; channelIdx < 16; channelIdx++) {
                if (gActiveSeqs[playerIdx].channelData[channelIdx].freqScaleDuration != 0) {
                    gActiveSeqs[playerIdx].channelData[channelIdx].freqScaleDuration--;
                    if (gActiveSeqs[playerIdx].channelData[channelIdx].freqScaleDuration != 0) {
                        gActiveSeqs[playerIdx].channelData[channelIdx].freqScaleCur -=
                            gActiveSeqs[playerIdx].channelData[channelIdx].freqScaleVelocity;
                    } else {
                        gActiveSeqs[playerIdx].channelData[channelIdx].freqScaleCur =
                            gActiveSeqs[playerIdx].channelData[channelIdx].freqScaleTarget;
                        gActiveSeqs[playerIdx].freqScaleChannelFlags ^= (1 << channelIdx);
                    }
                    // CHAN_UPD_FREQ_SCALE
                    Audio_QueueCmdF32(0x04000000 | _SHIFTL(playerIdx, 16, 8) | _SHIFTL(channelIdx, 8, 8),
                                      gActiveSeqs[playerIdx].channelData[channelIdx].freqScaleCur);
                }
            }
        }

        // Process setup commands
        if (gActiveSeqs[playerIdx].numSetupCmd != 0) {
            // If there is any "AudioSeqCmd_SetSpec" queued, drop all setup commands
            if (!Audio_IsSeqCmdNotQueued(0xF0000000, 0xF0000000)) {
                gActiveSeqs[playerIdx].numSetupCmd = 0;
                return;
            }

            // Only process setup commands once the timer reaches zero
            if (gActiveSeqs[playerIdx].setupCmdTimer != 0) {
                gActiveSeqs[playerIdx].setupCmdTimer--;
                continue;
            }

            // Do not process setup commands if the seqPlayer is already enabled
            if (gAudioContext.seqPlayers[playerIdx].enabled) {
                continue;
            }

            for (j = 0; j < gActiveSeqs[playerIdx].numSetupCmd; j++) {
                setupOp = (gActiveSeqs[playerIdx].setupCmd[j] & 0x00F00000) >> 20;
                setupPlayerIdx = (gActiveSeqs[playerIdx].setupCmd[j] & 0x000F0000) >> 16;
                setupVal2 = (gActiveSeqs[playerIdx].setupCmd[j] & 0xFF00) >> 8;
                setupVal1 = gActiveSeqs[playerIdx].setupCmd[j] & 0xFF;

                switch (setupOp) {
                    case 0x0:
                        Audio_SetVolumeScale(setupPlayerIdx, 1, 0x7F, setupVal1);
                        break;
                    case 0x7:
                        if (sNumSeqRequests[playerIdx] == setupVal1) {
                            Audio_SetVolumeScale(setupPlayerIdx, 1, 0x7F, setupVal2);
                        }
                        break;
                    case 0x1:
                        AudioSeqCmd_UnqueueSequence((u8)(playerIdx + 8), gActiveSeqs[playerIdx].seqId);
                        break;
                    case 0x2:
                        AudioSeqCmd_StartSequence((u8)(setupPlayerIdx + 8), 1, gActiveSeqs[setupPlayerIdx].seqId);
                        gActiveSeqs[setupPlayerIdx].fadeVolUpdate = 1;
                        gActiveSeqs[setupPlayerIdx].volScales[1] = 0x7F;
                        break;
                    case 0x3:
                        AudioSeqCmd_ScaleTempo((u8)(setupPlayerIdx + 8), setupVal2, setupVal1);
                        break;
                    case 0x4:
                        AudioSeqCmd_ResetTempo((u8)(setupPlayerIdx + 8), setupVal1);
                        break;
                    case 0x5:
                        seqId = gActiveSeqs[playerIdx].setupCmd[j] & 0xFFFF;
                        AudioSeqCmd_StartSequence((u8)(setupPlayerIdx + 8), gActiveSeqs[setupPlayerIdx].setupFadeTimer,
                                                  seqId);
                        Audio_SetVolumeScale(setupPlayerIdx, 1, 0x7F, 0);
                        gActiveSeqs[setupPlayerIdx].setupFadeTimer = 0;
                        break;
                    case 0x6:
                        gActiveSeqs[playerIdx].setupFadeTimer = setupVal2;
                        break;
                    case 0x8:
                        Audio_SetVolumeScale(setupPlayerIdx, setupVal2, 0x7F, setupVal1);
                        break;
                    case 0xE:
                        if (setupVal1 & 1) {
                            Audio_QueueCmdS32(0xE3000000, SEQUENCE_TABLE);
                        }
                        if (setupVal1 & 2) {
                            Audio_QueueCmdS32(0xE3000000, FONT_TABLE);
                        }
                        if (setupVal1 & 4) {
                            Audio_QueueCmdS32(0xE3000000, SAMPLE_TABLE);
                        }
                        break;
                    case 0x9:
                        channelMask = gActiveSeqs[playerIdx].setupCmd[j] & 0xFFFF;
                        AudioSeqCmd_SetActiveChannels((u8)(setupPlayerIdx + 8), channelMask);
                        break;
                    case 0xA:
                        AudioSeqCmd_SetPlayerFreq((u8)(setupPlayerIdx + 8), setupVal2, (setupVal1 * 10) & 0xFFFF);
                        break;
                }
            }

            gActiveSeqs[playerIdx].numSetupCmd = 0;
        }
    }
}

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
    u8 ret = false;
    u8 specMode = ((sChangeSpecCmd & 0xFF0000) >> 16);
    u8 reverbIndex = 0;

    if (sChangeSpecCmd != 0) {
        if (D_80200BCC--) {
            for (; specMode != 0;) {
                if (specMode & 1) {
                    // Set Reverb Data, dataType = 4, reverbIndex = reverbIndex, data = D_80200BCE, flags = 0
                    // dataType = 4:
                    //      synthesisReverbs[reverbIndex].volume = D_80200BCE
                    Audio_QueueCmdS32(((reverbIndex & 0xFF) << 8) | 0xE6040000, D_80200BCE);
                    Audio_ScheduleProcessCmds();
                }
                reverbIndex++;
                specMode = specMode >> 1;
            }

            D_80200BCE -= D_80200BD0;
            ret = true;
        } else {
            for (; specMode != 0;) {
                if (specMode & 1) {
                    // Set Reverb Data, dataType = 0, reverbIndex = reverbIndex, data = gReverbSettingsTable, flags = 0
                    // dataType = 0:
                    //      AudioHeap_InitReverb(reverbIndex, gReverbSettingsTable[sChangeSpecCmd] + new_var, 0);
                    Audio_QueueCmdS32(((reverbIndex & 0xFF) << 8) | 0xE6000000,
                                      (s32)(gReverbSettingsTable[sChangeSpecCmd & 0xFF & 0xFF] + reverbIndex));
                    Audio_ScheduleProcessCmds();
                }
                reverbIndex++;
                specMode = specMode >> 1;
            }

            sChangeSpecCmd = 0;
            Audio_QueueCmdS8(0x46020000, gSfxChannelLayout);
            func_801A4D50();
        }
    }

    return ret;
}

void Audio_ResetSequences(void) {
    u8 playerIdx;
    u8 j;

    for (playerIdx = 0; playerIdx < 5; playerIdx++) {
        sNumSeqRequests[playerIdx] = 0;

        gActiveSeqs[playerIdx].seqId = NA_BGM_DISABLED;
        gActiveSeqs[playerIdx].prevSeqId = NA_BGM_DISABLED;
        gActiveSeqs[playerIdx].tempoDuration = 0;
        gActiveSeqs[playerIdx].tempoDefault = 0;
        gActiveSeqs[playerIdx].tempoCmd = 0;
        gActiveSeqs[playerIdx].channelPortMask = 0;
        gActiveSeqs[playerIdx].numSetupCmd = 0;
        gActiveSeqs[playerIdx].setupFadeTimer = 0;
        gActiveSeqs[playerIdx].freqScaleChannelFlags = 0;
        gActiveSeqs[playerIdx].volChannelFlags = 0;
        gActiveSeqs[playerIdx].isWaitingForFonts = false;
        gActiveSeqs[playerIdx].isSeqPlayerInit = false;

        for (j = 0; j < ARRAY_COUNT(gActiveSeqs[playerIdx].volScales); j++) {
            gActiveSeqs[playerIdx].volScales[j] = 0x7F;
        }

        gActiveSeqs[playerIdx].volFadeTimer = 1;
        gActiveSeqs[playerIdx].fadeVolUpdate = 1;
    }
}

void Audio_ResetSequencesAndVolume(void) {
    u8 playerIdx;
    u8 j;

    for (playerIdx = 0; playerIdx < 5; playerIdx++) {
        gActiveSeqs[playerIdx].volCur = 1.0f;
        gActiveSeqs[playerIdx].volDuration = 0;
        gActiveSeqs[playerIdx].fadeVolUpdate = 0;
        for (j = 0; j < ARRAY_COUNT(gActiveSeqs[playerIdx].volScales); j++) {
            gActiveSeqs[playerIdx].volScales[j] = 0x7F;
        }
    }
    Audio_ResetSequences();
}
