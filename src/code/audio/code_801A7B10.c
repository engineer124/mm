#include "global.h"

void func_801A4DA4(void);

u8 sSeqCmdWritePos = 0;
u8 sSeqCmdReadPos = 0;
u8 sIsSeqStartDisabled = 0;
u8 sSoundModeList[] = {
    AUDIO_MODE_STEREO, AUDIO_MODE_HEADSET, AUDIO_MODE_UNK, AUDIO_MODE_MONO, AUDIO_MODE_SURROUND,
};
u8 gAudioSpecId = 0;
u8 D_801DB4D8 = 0;
u32 sChangeSpecCmd = 0;

void Audio_StartSequence(u8 playerIndex, u8 seqId, u8 seqArgs, u16 fadeTimer) {
    u8 channelIdx;
    u16 duration;
    s32 pad;

    if ((sIsSeqStartDisabled == 0) || (playerIndex == SEQ_PLAYER_SFX)) {
        seqArgs &= 0x7F;
        if (seqArgs == 0x7F) {
            duration = (fadeTimer >> 3) * 60 * gAudioContext.audioBufferParameters.updatesPerFrame;
            AudioThread_QueueCmdS32(0x85000000 | _SHIFTL(playerIndex, 16, 8) | _SHIFTL(seqId, 8, 8), duration);
        } else {
            AudioThread_QueueCmdS32(0x82000000 | _SHIFTL(playerIndex, 16, 8) | _SHIFTL(seqId, 8, 8),
                                    (fadeTimer * (u16)gAudioContext.audioBufferParameters.updatesPerFrame) / 4);
        }

        gActiveSeqs[playerIndex].seqId = seqId | (seqArgs << 8);
        gActiveSeqs[playerIndex].prevSeqId = seqId | (seqArgs << 8);
        gActiveSeqs[playerIndex].isSeqPlayerInit = true;

        if (gActiveSeqs[playerIndex].volCur != 1.0f) {
            AudioThread_QueueCmdF32(0x41000000 | _SHIFTL(playerIndex, 16, 8), gActiveSeqs[playerIndex].volCur);
        }

        gActiveSeqs[playerIndex].tempoDuration = 0;
        gActiveSeqs[playerIndex].tempoDefault = 0;
        gActiveSeqs[playerIndex].tempoCmd = 0;

        for (channelIdx = 0; channelIdx < ARRAY_COUNT(gActiveSeqs[playerIndex].channelData); channelIdx++) {
            gActiveSeqs[playerIndex].channelData[channelIdx].volCur = 1.0f;
            gActiveSeqs[playerIndex].channelData[channelIdx].volDuration = 0;
            gActiveSeqs[playerIndex].channelData[channelIdx].freqScaleCur = 1.0f;
            gActiveSeqs[playerIndex].channelData[channelIdx].freqScaleDuration = 0;
        }

        gActiveSeqs[playerIndex].freqScaleChannelFlags = 0;
        gActiveSeqs[playerIndex].volChannelFlags = 0;
    }
}

void Audio_StopSequence(u8 playerIndex, u16 fadeTimer) {
    AudioThread_QueueCmdS32(0x83000000 | ((u8)playerIndex << 16),
                            (fadeTimer * (u16)gAudioContext.audioBufferParameters.updatesPerFrame) / 4);
    gActiveSeqs[playerIndex].seqId = NA_BGM_DISABLED;
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
    u8 playerIndex;
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
    playerIndex = (cmd & 0x7000000) >> 24;

    switch (op) {
        case 0x0:
            // play sequence immediately
            seqId = cmd & 0xFF;
            seqArgs = (cmd & 0xFF00) >> 8;
            fadeTimer = (cmd & 0xFF0000) >> 13;
            if (!gActiveSeqs[playerIndex].isWaitingForFonts && (sIsSeqStartDisabled == 0)) {
                if (seqArgs < 0x80) {
                    Audio_StartSequence(playerIndex, seqId, seqArgs, fadeTimer);
                } else {
                    // This case is taken for the new 0x8000 flag introduced in MM

                    // Store the cmd to be called again once the fonts are loaded
                    // but changes the command so that next time, the (seqArgs < 0x80) case is taken
                    gActiveSeqs[playerIndex].startSeqCmd = (cmd & ~0x8008000) + 0x8000000;
                    gActiveSeqs[playerIndex].isWaitingForFonts = true;
                    gActiveSeqs[playerIndex].fontId = *AudioThread_GetFontsForSequence(seqId, &outNumFonts);
                    Audio_StopSequence(playerIndex, 1);

                    if (gActiveSeqs[playerIndex].prevSeqId != NA_BGM_DISABLED) {
                        if (*AudioThread_GetFontsForSequence(seqId, &outNumFonts) !=
                            *AudioThread_GetFontsForSequence(gActiveSeqs[playerIndex].prevSeqId & 0xFF, &outNumFonts)) {
                            // Discard Seq Fonts
                            AudioThread_QueueCmdS32(((seqId & 0xFF) << 8) | 0xF6000000, 0);
                        }
                    }

                    AudioThread_QueueCmdS8((((*AudioThread_GetFontsForSequence(seqId, &outNumFonts) & 0xFF)) << 0x10) |
                                               0xF5000000 | 0x1400 | (((playerIndex + 1) & 0xFF & 0xFF & 0xFF)),
                                           0);
                }
            }
            break;

        case 0x1:
            // disable seq player
            fadeTimer = (cmd & 0xFF0000) >> 13;
            Audio_StopSequence(playerIndex, fadeTimer);
            break;

        case 0x2:
            // queue sequence
            seqId = cmd & 0xFF;
            seqArgs = (cmd & 0xFF00) >> 8;
            fadeTimer = (cmd & 0xFF0000) >> 13;
            importance = seqArgs;

            // Checks if the requested sequence is first in the list of requested
            // If it is already queued and first in the list, then play the sequence immediately
            for (i = 0; i < sNumSeqRequests[playerIndex]; i++) {
                if (sSeqRequests[playerIndex][i].seqId == seqId) {
                    if (i == 0) {
                        Audio_StartSequence(playerIndex, seqId, seqArgs, fadeTimer);
                    }
                    return;
                }
            }

            // Searches the sequence requests for the first request that does not have a higher priority
            // than the current incoming request
            found = sNumSeqRequests[playerIndex];
            for (i = 0; i < sNumSeqRequests[playerIndex]; i++) {
                if (sSeqRequests[playerIndex][i].importance <= importance) {
                    found = i;
                    i = sNumSeqRequests[playerIndex]; // "break;"
                }
            }

            if (sNumSeqRequests[playerIndex] < 5) {
                sNumSeqRequests[playerIndex]++;
            }

            for (i = sNumSeqRequests[playerIndex] - 1; i != found; i--) {
                sSeqRequests[playerIndex][i].importance = sSeqRequests[playerIndex][i - 1].importance;
                sSeqRequests[playerIndex][i].seqId = sSeqRequests[playerIndex][i - 1].seqId;
            }
            sSeqRequests[playerIndex][found].importance = seqArgs;
            sSeqRequests[playerIndex][found].seqId = seqId;

            if (found == 0) {
                Audio_StartSequence(playerIndex, seqId, seqArgs, fadeTimer);
            }
            break;

        case 0x3:
            // unqueue/stop sequence
            fadeTimer = (cmd & 0xFF0000) >> 13;

            found = sNumSeqRequests[playerIndex];
            for (i = 0; i < sNumSeqRequests[playerIndex]; i++) {
                seqId = cmd & 0xFF;
                if (sSeqRequests[playerIndex][i].seqId == seqId) {
                    found = i;
                    i = sNumSeqRequests[playerIndex]; // "break;"
                }
            }

            if (found != sNumSeqRequests[playerIndex]) {
                for (i = found; i < sNumSeqRequests[playerIndex] - 1; i++) {
                    sSeqRequests[playerIndex][i].importance = sSeqRequests[playerIndex][i + 1].importance;
                    sSeqRequests[playerIndex][i].seqId = sSeqRequests[playerIndex][i + 1].seqId;
                }
                sNumSeqRequests[playerIndex]--;
            }

            if (found == 0) {
                // Stop current sequence
                Audio_StopSequence(playerIndex, fadeTimer);
                if (sNumSeqRequests[playerIndex] != 0) {
                    // Play the next sequence in the queue
                    Audio_StartSequence(playerIndex, sSeqRequests[playerIndex][0].seqId,
                                        sSeqRequests[playerIndex][0].importance, fadeTimer);
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
            gActiveSeqs[playerIndex].volTarget = (f32)val / 127.0f;
            if (gActiveSeqs[playerIndex].volCur != gActiveSeqs[playerIndex].volTarget) {
                gActiveSeqs[playerIndex].volVelocity =
                    (gActiveSeqs[playerIndex].volCur - gActiveSeqs[playerIndex].volTarget) / (f32)duration;
                gActiveSeqs[playerIndex].volDuration = duration;
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
            for (i = 0; i < ARRAY_COUNT(gActiveSeqs[playerIndex].channelData); i++) {
                gActiveSeqs[playerIndex].channelData[i].freqScaleTarget = freqScaleTarget;
                gActiveSeqs[playerIndex].channelData[i].freqScaleDuration = duration;
                gActiveSeqs[playerIndex].channelData[i].freqScaleVelocity =
                    (gActiveSeqs[playerIndex].channelData[i].freqScaleCur - freqScaleTarget) / (f32)duration;
            }
            gActiveSeqs[playerIndex].freqScaleChannelFlags = 0xFFFF;
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
            gActiveSeqs[playerIndex].channelData[channelIdx].freqScaleTarget = freqScaleTarget;
            gActiveSeqs[playerIndex].channelData[channelIdx].freqScaleVelocity =
                (gActiveSeqs[playerIndex].channelData[channelIdx].freqScaleCur - freqScaleTarget) / (f32)duration;
            gActiveSeqs[playerIndex].channelData[channelIdx].freqScaleDuration = duration;
            gActiveSeqs[playerIndex].freqScaleChannelFlags |= 1 << channelIdx;
            break;

        case 0x6:
            // transition vol scale
            duration = (cmd & 0xFF0000) >> 15;
            channelIdx = (cmd & 0xF00) >> 8;
            val = cmd & 0xFF;
            if (duration == 0) {
                duration++;
            }
            gActiveSeqs[playerIndex].channelData[channelIdx].volTarget = (f32)val / 127.0f;
            if (gActiveSeqs[playerIndex].channelData[channelIdx].volCur !=
                gActiveSeqs[playerIndex].channelData[channelIdx].volTarget) {
                gActiveSeqs[playerIndex].channelData[channelIdx].volVelocity =
                    (gActiveSeqs[playerIndex].channelData[channelIdx].volCur -
                     gActiveSeqs[playerIndex].channelData[channelIdx].volTarget) /
                    (f32)duration;
                gActiveSeqs[playerIndex].channelData[channelIdx].volDuration = duration;
                gActiveSeqs[playerIndex].volChannelFlags |= 1 << channelIdx;
            }
            break;

        case 0x7:
            // set global io port
            port = (cmd & 0xFF0000) >> 16;
            val = cmd & 0xFF;
            AudioThread_QueueCmdS8(0x46000000 | _SHIFTL(playerIndex, 16, 8) | _SHIFTL(port, 0, 8), val);
            break;

        case 0x8:
            // set io port if channel masked
            channelIdx = (cmd & 0xF00) >> 8;
            port = (cmd & 0xFF0000) >> 16;
            val = cmd & 0xFF;
            if ((gActiveSeqs[playerIndex].channelPortMask & (1 << channelIdx)) == 0) {
                AudioThread_QueueCmdS8(
                    0x06000000 | _SHIFTL(playerIndex, 16, 8) | _SHIFTL(channelIdx, 8, 8) | _SHIFTL(port, 0, 8), val);
            }
            break;

        case 0x9:
            // set channel mask for command 0x8
            gActiveSeqs[playerIndex].channelPortMask = cmd & 0xFFFF;
            break;

        case 0xA:
            // set channel stop mask
            channelMask = cmd & 0xFFFF;
            if (channelMask != 0) {
                // with channel mask channelMask...
                AudioThread_QueueCmdU16(0x90000000 | _SHIFTL(playerIndex, 16, 8), channelMask);
                // stop channels
                AudioThread_QueueCmdS8(0x08000000 | _SHIFTL(playerIndex, 16, 8) | 0xFF00, 1);
            }
            channelMaskN = channelMask ^ 0xFFFF;
            if (channelMaskN != 0) {
                // with channel mask ~channelMask...
                AudioThread_QueueCmdU16(0x90000000 | _SHIFTL(playerIndex, 16, 8), channelMaskN);
                // unstop channels
                AudioThread_QueueCmdS8(0x08000000 | _SHIFTL(playerIndex, 16, 8) | 0xFF00, 0);
            }
            break;

        case 0xB:
            // update tempo
            gActiveSeqs[playerIndex].tempoCmd = cmd;
            break;

        case 0xC:
            // start sequence with setup commands
            subOp = (cmd & 0xF00000) >> 20;
            if (subOp != 0xF) {
                if (gActiveSeqs[playerIndex].numSetupCmd < 7) {
                    found = gActiveSeqs[playerIndex].numSetupCmd++;
                    if (found < 8) {
                        gActiveSeqs[playerIndex].setupCmd[found] = cmd;
                        gActiveSeqs[playerIndex].setupCmdTimer = 2;
                    }
                }
            } else {
                gActiveSeqs[playerIndex].numSetupCmd = 0;
            }
            break;

        case 0xE:
            subOp = (cmd & 0xF00) >> 8;
            val = cmd & 0xFF;
            switch (subOp) {
                case 0:
                    // set sound mode
                    AudioThread_QueueCmdS32(0xF0000000, sSoundModeList[val]);
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
                AudioThread_QueueCmdS32(0xF8000000, 0);
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

u16 Audio_GetActiveSequence(u8 playerIndex) {
    if (gActiveSeqs[playerIndex].isWaitingForFonts == true) {
        return gActiveSeqs[playerIndex].startSeqCmd & 0xFF;
    }

    if (gActiveSeqs[playerIndex].seqId != NA_BGM_DISABLED) {
        return gActiveSeqs[playerIndex].seqId;
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
void Audio_ResetSequenceRequests(u8 playerIndex) {
    sNumSeqRequests[playerIndex] = 0;
}

// Unused
void Audio_DisableSeqCmdSetupOp(u8 playerIndex, u8 setupOpDisabled) {
    u8 i;

    for (i = 0; i < gActiveSeqs[playerIndex].numSetupCmd; i++) {
        u8 setupOp = (gActiveSeqs[playerIndex].setupCmd[i] & 0xF00000) >> 20;

        if (setupOp == setupOpDisabled) {
            gActiveSeqs[playerIndex].setupCmd[i] = 0xFF000000;
        }
    }
}

void Audio_SetVolumeScale(u8 playerIndex, u8 scaleIdx, u8 targetVol, u8 volFadeTimer) {
    f32 volScale;
    u8 i;

    gActiveSeqs[playerIndex].volScales[scaleIdx] = targetVol & 0x7F;

    if (volFadeTimer != 0) {
        gActiveSeqs[playerIndex].fadeVolUpdate = 1;
        gActiveSeqs[playerIndex].volFadeTimer = volFadeTimer;
    } else {
        for (i = 0, volScale = 1.0f; i < 4; i++) {
            volScale *= gActiveSeqs[playerIndex].volScales[i] / 127.0f;
        }

        Audio_SetVolumeScaleNow(playerIndex, volFadeTimer, volScale);
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
    u8 playerIndex;
    u8 j;
    u8 channelIdx;

    for (playerIndex = 0; playerIndex < ARRAY_COUNT(gActiveSeqs); playerIndex++) {

        // The seqPlayer has finished initializing and is currently playing the active sequences
        if (gActiveSeqs[playerIndex].isSeqPlayerInit && gAudioContext.seqPlayers[playerIndex].enabled) {
            gActiveSeqs[playerIndex].isSeqPlayerInit = false;
        }

        // The seqPlayer is no longer playing the active sequences
        if ((Audio_GetActiveSequence(playerIndex) != NA_BGM_DISABLED) &&
            !gAudioContext.seqPlayers[playerIndex].enabled && (!gActiveSeqs[playerIndex].isSeqPlayerInit)) {
            gActiveSeqs[playerIndex].seqId = NA_BGM_DISABLED;
        }

        // Check if the requested sequences is waiting for fonts to load
        if (gActiveSeqs[playerIndex].isWaitingForFonts) {
            switch ((s32)func_80193BA0(&dummy)) {
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                    // The fonts have been loaded successfully.
                    gActiveSeqs[playerIndex].isWaitingForFonts = false;
                    // Queue the same command that was stored previously, but without the 0x8000
                    Audio_ProcessSeqCmd(gActiveSeqs[playerIndex].startSeqCmd);
                    break;
                case 0xFF:
                    // There was an error in loading the fonts
                    gActiveSeqs[playerIndex].isWaitingForFonts = false;
                    break;
            }
        }

        if (gActiveSeqs[playerIndex].fadeVolUpdate) {
            volume = 1.0f;
            for (j = 0; j < 4; j++) {
                volume *= (gActiveSeqs[playerIndex].volScales[j] / 127.0f);
            }

            AudioSeqCmd_SetPlayerVol((u8)(playerIndex + 8), gActiveSeqs[playerIndex].volFadeTimer,
                                     (u8)(volume * 127.0f));
            gActiveSeqs[playerIndex].fadeVolUpdate = 0;
        }

        if (gActiveSeqs[playerIndex].volDuration != 0) {
            gActiveSeqs[playerIndex].volDuration--;

            if (gActiveSeqs[playerIndex].volDuration != 0) {
                gActiveSeqs[playerIndex].volCur -= gActiveSeqs[playerIndex].volVelocity;
            } else {
                gActiveSeqs[playerIndex].volCur = gActiveSeqs[playerIndex].volTarget;
            }

            AudioThread_QueueCmdF32(0x41000000 | _SHIFTL(playerIndex, 16, 8), gActiveSeqs[playerIndex].volCur);
        }

        if (gActiveSeqs[playerIndex].tempoCmd != 0) {
            tempoCmd = gActiveSeqs[playerIndex].tempoCmd;
            tempoDuration = (tempoCmd & 0xFF0000) >> 15;
            tempoTarget = tempoCmd & 0xFFF;
            if (tempoDuration == 0) {
                tempoDuration++;
            }

            if (gAudioContext.seqPlayers[playerIndex].enabled) {
                tempoDefault = gAudioContext.seqPlayers[playerIndex].tempo / 0x30;
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
                        if (gActiveSeqs[playerIndex].tempoDefault) {
                            tempoTarget = gActiveSeqs[playerIndex].tempoDefault;
                        } else {
                            tempoTarget = tempoDefault;
                        }
                        break;
                }

                if (gActiveSeqs[playerIndex].tempoDefault == 0) {
                    gActiveSeqs[playerIndex].tempoDefault = tempoDefault;
                }

                gActiveSeqs[playerIndex].tempoTarget = tempoTarget;
                gActiveSeqs[playerIndex].tempoCur = gAudioContext.seqPlayers[playerIndex].tempo / 0x30;
                gActiveSeqs[playerIndex].tempoVelocity =
                    (gActiveSeqs[playerIndex].tempoCur - gActiveSeqs[playerIndex].tempoTarget) / tempoDuration;
                gActiveSeqs[playerIndex].tempoDuration = tempoDuration;
                gActiveSeqs[playerIndex].tempoCmd = 0;
            }
        }

        if (gActiveSeqs[playerIndex].tempoDuration != 0) {
            gActiveSeqs[playerIndex].tempoDuration--;
            if (gActiveSeqs[playerIndex].tempoDuration != 0) {
                gActiveSeqs[playerIndex].tempoCur -= gActiveSeqs[playerIndex].tempoVelocity;
            } else {
                gActiveSeqs[playerIndex].tempoCur = gActiveSeqs[playerIndex].tempoTarget;
            }
            // set tempo
            AudioThread_QueueCmdS32(0x47000000 | _SHIFTL(playerIndex, 16, 8), gActiveSeqs[playerIndex].tempoCur);
        }

        if (gActiveSeqs[playerIndex].volChannelFlags != 0) {
            for (channelIdx = 0; channelIdx < 16; channelIdx++) {
                if (gActiveSeqs[playerIndex].channelData[channelIdx].volDuration != 0) {
                    gActiveSeqs[playerIndex].channelData[channelIdx].volDuration--;
                    if (gActiveSeqs[playerIndex].channelData[channelIdx].volDuration != 0) {
                        gActiveSeqs[playerIndex].channelData[channelIdx].volCur -=
                            gActiveSeqs[playerIndex].channelData[channelIdx].volVelocity;
                    } else {
                        gActiveSeqs[playerIndex].channelData[channelIdx].volCur =
                            gActiveSeqs[playerIndex].channelData[channelIdx].volTarget;
                        gActiveSeqs[playerIndex].volChannelFlags ^= (1 << channelIdx);
                    }
                    // CHAN_UPD_VOL_SCALE (playerIndex = seq, channelIdx = chan)
                    AudioThread_QueueCmdF32(0x01000000 | _SHIFTL(playerIndex, 16, 8) | _SHIFTL(channelIdx, 8, 8),
                                            gActiveSeqs[playerIndex].channelData[channelIdx].volCur);
                }
            }
        }

        if (gActiveSeqs[playerIndex].freqScaleChannelFlags != 0) {
            for (channelIdx = 0; channelIdx < 16; channelIdx++) {
                if (gActiveSeqs[playerIndex].channelData[channelIdx].freqScaleDuration != 0) {
                    gActiveSeqs[playerIndex].channelData[channelIdx].freqScaleDuration--;
                    if (gActiveSeqs[playerIndex].channelData[channelIdx].freqScaleDuration != 0) {
                        gActiveSeqs[playerIndex].channelData[channelIdx].freqScaleCur -=
                            gActiveSeqs[playerIndex].channelData[channelIdx].freqScaleVelocity;
                    } else {
                        gActiveSeqs[playerIndex].channelData[channelIdx].freqScaleCur =
                            gActiveSeqs[playerIndex].channelData[channelIdx].freqScaleTarget;
                        gActiveSeqs[playerIndex].freqScaleChannelFlags ^= (1 << channelIdx);
                    }
                    // CHAN_UPD_FREQ_SCALE
                    AudioThread_QueueCmdF32(0x04000000 | _SHIFTL(playerIndex, 16, 8) | _SHIFTL(channelIdx, 8, 8),
                                            gActiveSeqs[playerIndex].channelData[channelIdx].freqScaleCur);
                }
            }
        }

        // Process setup commands
        if (gActiveSeqs[playerIndex].numSetupCmd != 0) {
            // If there is any "AudioSeqCmd_SetSpec" queued, drop all setup commands
            if (!Audio_IsSeqCmdNotQueued(0xF0000000, 0xF0000000)) {
                gActiveSeqs[playerIndex].numSetupCmd = 0;
                return;
            }

            // Only process setup commands once the timer reaches zero
            if (gActiveSeqs[playerIndex].setupCmdTimer != 0) {
                gActiveSeqs[playerIndex].setupCmdTimer--;
                continue;
            }

            // Do not process setup commands if the seqPlayer is already enabled
            if (gAudioContext.seqPlayers[playerIndex].enabled) {
                continue;
            }

            for (j = 0; j < gActiveSeqs[playerIndex].numSetupCmd; j++) {
                setupOp = (gActiveSeqs[playerIndex].setupCmd[j] & 0x00F00000) >> 20;
                setupPlayerIdx = (gActiveSeqs[playerIndex].setupCmd[j] & 0x000F0000) >> 16;
                setupVal2 = (gActiveSeqs[playerIndex].setupCmd[j] & 0xFF00) >> 8;
                setupVal1 = gActiveSeqs[playerIndex].setupCmd[j] & 0xFF;

                switch (setupOp) {
                    case 0x0:
                        Audio_SetVolumeScale(setupPlayerIdx, 1, 0x7F, setupVal1);
                        break;
                    case 0x7:
                        if (sNumSeqRequests[playerIndex] == setupVal1) {
                            Audio_SetVolumeScale(setupPlayerIdx, 1, 0x7F, setupVal2);
                        }
                        break;
                    case 0x1:
                        AudioSeqCmd_UnqueueSequence((u8)(playerIndex + 8), gActiveSeqs[playerIndex].seqId);
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
                        seqId = gActiveSeqs[playerIndex].setupCmd[j] & 0xFFFF;
                        AudioSeqCmd_StartSequence((u8)(setupPlayerIdx + 8), gActiveSeqs[setupPlayerIdx].setupFadeTimer,
                                                  seqId);
                        Audio_SetVolumeScale(setupPlayerIdx, 1, 0x7F, 0);
                        gActiveSeqs[setupPlayerIdx].setupFadeTimer = 0;
                        break;
                    case 0x6:
                        gActiveSeqs[playerIndex].setupFadeTimer = setupVal2;
                        break;
                    case 0x8:
                        Audio_SetVolumeScale(setupPlayerIdx, setupVal2, 0x7F, setupVal1);
                        break;
                    case 0xE:
                        if (setupVal1 & 1) {
                            AudioThread_QueueCmdS32(0xE3000000, SEQUENCE_TABLE);
                        }
                        if (setupVal1 & 2) {
                            AudioThread_QueueCmdS32(0xE3000000, FONT_TABLE);
                        }
                        if (setupVal1 & 4) {
                            AudioThread_QueueCmdS32(0xE3000000, SAMPLE_TABLE);
                        }
                        break;
                    case 0x9:
                        channelMask = gActiveSeqs[playerIndex].setupCmd[j] & 0xFFFF;
                        AudioSeqCmd_SetActiveChannels((u8)(setupPlayerIdx + 8), channelMask);
                        break;
                    case 0xA:
                        AudioSeqCmd_SetPlayerFreq((u8)(setupPlayerIdx + 8), setupVal2, (setupVal1 * 10) & 0xFFFF);
                        break;
                }
            }

            gActiveSeqs[playerIndex].numSetupCmd = 0;
        }
    }
}

u8 func_801A9768(void) {
    if (D_801DB4D8 != 0) {
        if (D_801DB4D8 == 1) {
            if (func_80193C5C() == 1) {
                D_801DB4D8 = 0;
                AudioThread_QueueCmdS8(0x46020000, gSfxChannelLayout);
                func_801A4DA4();
            }
        } else if (D_801DB4D8 == 2) {
            while (func_80193C5C() != 1) {}
            D_801DB4D8 = 0;
            AudioThread_QueueCmdS8(0x46020000, gSfxChannelLayout);
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
                    AudioThread_QueueCmdS32(((reverbIndex & 0xFF) << 8) | 0xE6040000, D_80200BCE);
                    AudioThread_ScheduleProcessCmds();
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
                    AudioThread_QueueCmdS32(((reverbIndex & 0xFF) << 8) | 0xE6000000,
                                            (s32)(gReverbSettingsTable[sChangeSpecCmd & 0xFF & 0xFF] + reverbIndex));
                    AudioThread_ScheduleProcessCmds();
                }
                reverbIndex++;
                specMode = specMode >> 1;
            }

            sChangeSpecCmd = 0;
            AudioThread_QueueCmdS8(0x46020000, gSfxChannelLayout);
            func_801A4D50();
        }
    }

    return ret;
}

void Audio_ResetSequences(void) {
    u8 playerIndex;
    u8 j;

    for (playerIndex = 0; playerIndex < 5; playerIndex++) {
        sNumSeqRequests[playerIndex] = 0;

        gActiveSeqs[playerIndex].seqId = NA_BGM_DISABLED;
        gActiveSeqs[playerIndex].prevSeqId = NA_BGM_DISABLED;
        gActiveSeqs[playerIndex].tempoDuration = 0;
        gActiveSeqs[playerIndex].tempoDefault = 0;
        gActiveSeqs[playerIndex].tempoCmd = 0;
        gActiveSeqs[playerIndex].channelPortMask = 0;
        gActiveSeqs[playerIndex].numSetupCmd = 0;
        gActiveSeqs[playerIndex].setupFadeTimer = 0;
        gActiveSeqs[playerIndex].freqScaleChannelFlags = 0;
        gActiveSeqs[playerIndex].volChannelFlags = 0;
        gActiveSeqs[playerIndex].isWaitingForFonts = false;
        gActiveSeqs[playerIndex].isSeqPlayerInit = false;

        for (j = 0; j < ARRAY_COUNT(gActiveSeqs[playerIndex].volScales); j++) {
            gActiveSeqs[playerIndex].volScales[j] = 0x7F;
        }

        gActiveSeqs[playerIndex].volFadeTimer = 1;
        gActiveSeqs[playerIndex].fadeVolUpdate = 1;
    }
}

void Audio_ResetSequencesAndVolume(void) {
    u8 playerIndex;
    u8 j;

    for (playerIndex = 0; playerIndex < 5; playerIndex++) {
        gActiveSeqs[playerIndex].volCur = 1.0f;
        gActiveSeqs[playerIndex].volDuration = 0;
        gActiveSeqs[playerIndex].fadeVolUpdate = 0;
        for (j = 0; j < ARRAY_COUNT(gActiveSeqs[playerIndex].volScales); j++) {
            gActiveSeqs[playerIndex].volScales[j] = 0x7F;
        }
    }
    Audio_ResetSequences();
}
