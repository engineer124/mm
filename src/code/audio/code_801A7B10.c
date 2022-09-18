/**
 * @file code_801A7B10.c
 *
 * This file implements a set of high-level audio sequence commands that allow sequences to be modified in real-time.
 * These commands are intended to interface external to the audio library.
 *
 * These commands are generated using `Audio_QueueSeqCmd`, and a user-friendly interface for this function
 * can be found in `seqcmd.h`
 *
 * These commands change sequences by generating internal audio commands `Audio_QueueCmd` which allows these
 * sequence requests to be passed onto the audio thread. It is worth noting all functions in this file are
 * called from the graph thread.
 *
 * These commands are not to be confused with the sequence instructions used by the sequences themselves
 * which are a midi-based scripting language.
 *
 * Nor are these commands to be confused with the internal audio commands used to transfer requests from
 * the graph thread to the audio thread.
 */
#include "global.h"

// Direct audio command (skips the queueing system)
#define SEQCMD_SET_PLAYER_VOLUME_NOW(seqPlayerIndex, duration, volume)                                                \
    Audio_ProcessSeqCmd((SEQCMD_OP_SET_PLAYER_VOLUME << 28) | ((u8)(seqPlayerIndex) << 24) | ((u8)(duration) << 16) | \
                        ((u8)((volume)*127.0f)));

u8 sSeqCmdWritePos = 0;
u8 sSeqCmdReadPos = 0;
u8 sStartSeqDisabled = 0;
u8 sSoundModeList[] = {
    SOUNDMODE_STEREO, SOUNDMODE_HEADSET, SOUNDMODE_SURROUND_EXTERNAL, SOUNDMODE_MONO, SOUNDMODE_SURROUND,
};
u8 gAudioSpecId = 0;
u8 D_801DB4D8 = 0;
u32 sChangeSpecCmd = 0;

void Audio_StartSequence(u8 seqPlayerIndex, u8 seqId, u8 seqArgs, u16 fadeInDuration) {
    u8 channelIndex;
    u16 skipTicks;
    s32 pad;

    if ((sStartSeqDisabled == 0) || (seqPlayerIndex == SEQ_PLAYER_SFX)) {
        seqArgs &= 0x7F;
        if (seqArgs == 0x7F) {
            // `fadeInDuration` is interpreted as skip ticks
            skipTicks = (fadeInDuration >> 3) * 60 * gAudioContext.audioBufferParameters.updatesPerFrame;
            AUDIOCMD_GLOBAL_SYNC_INIT_SEQPLAYER_SKIP_TICKS(seqPlayerIndex, seqId, skipTicks);
        } else {
            // `fadeInDuration` is interpreted as number of frames at 30 fps
            AUDIOCMD_GLOBAL_SYNC_INIT_SEQPLAYER(
                seqPlayerIndex, seqId, 0,
                (fadeInDuration * (u16)gAudioContext.audioBufferParameters.updatesPerFrame) / 4);
        }

        gActiveSeqs[seqPlayerIndex].seqId = seqId | (seqArgs << 8);
        gActiveSeqs[seqPlayerIndex].prevSeqId = seqId | (seqArgs << 8);
        gActiveSeqs[seqPlayerIndex].isSeqPlayerInit = true;

        if (gActiveSeqs[seqPlayerIndex].volCur != 1.0f) {
            AUDIOCMD_SEQPLAYER_FADE_VOLUME_SCALE(seqPlayerIndex, gActiveSeqs[seqPlayerIndex].volCur);
        }

        gActiveSeqs[seqPlayerIndex].tempoTimer = 0;
        gActiveSeqs[seqPlayerIndex].tempoOriginal = 0;
        gActiveSeqs[seqPlayerIndex].tempoCmd = 0;

        for (channelIndex = 0; channelIndex < SEQ_NUM_CHANNELS; channelIndex++) {
            gActiveSeqs[seqPlayerIndex].channelData[channelIndex].volCur = 1.0f;
            gActiveSeqs[seqPlayerIndex].channelData[channelIndex].volTimer = 0;
            gActiveSeqs[seqPlayerIndex].channelData[channelIndex].freqScaleCur = 1.0f;
            gActiveSeqs[seqPlayerIndex].channelData[channelIndex].freqScaleTimer = 0;
        }

        gActiveSeqs[seqPlayerIndex].freqScaleChannelFlags = 0;
        gActiveSeqs[seqPlayerIndex].volChannelFlags = 0;
    }
}

void Audio_StopSequence(u8 seqPlayerIndex, u16 fadeOutDuration) {
    AUDIOCMD_GLOBAL_DISABLE_SEQPLAYER(seqPlayerIndex,
                                      (fadeOutDuration * (u16)gAudioContext.audioBufferParameters.updatesPerFrame) / 4);
    gActiveSeqs[seqPlayerIndex].seqId = NA_BGM_DISABLED;
}

void Audio_ProcessSeqCmd(u32 cmd) {
    s32 priority;
    s32 channelMaskEnable;
    u16 channelMaskDisable;
    u16 fadeTimer;
    u16 val;
    u8 oldSpecId;
    u8 specId;
    u8 specMode;
    u8 op;
    u8 subOp;
    u8 seqPlayerIndex;
    u8 seqId;
    u8 seqArgs;
    u8 found;
    u8 port;
    u8 duration;
    u8 channelIndex;
    u8 i;
    f32 freqScaleTarget;
    u32 outNumFonts;

    op = cmd >> 28;
    seqPlayerIndex = (cmd & 0x7000000) >> 24;

    switch (op) {
        case SEQCMD_OP_PLAY_SEQUENCE:
            // Play a new sequence
            seqId = cmd & 0xFF;
            seqArgs = (cmd & 0xFF00) >> 8;
            // `fadeTimer` is only shifted 13 bits instead of 16 bits.
            // `fadeTimer` continues to be scaled in `Audio_StartSequence`
            fadeTimer = (cmd & 0xFF0000) >> 13;
            if (!gActiveSeqs[seqPlayerIndex].isWaitingForFonts && (sStartSeqDisabled == 0)) {
                if (seqArgs < 0x80) {
                    Audio_StartSequence(seqPlayerIndex, seqId, seqArgs, fadeTimer);
                } else {
                    // This case is taken for the new 0x8000 flag introduced in MM

                    // Store the cmd to be called again once the fonts are loaded
                    // but changes the command so that next time, the (seqArgs < 0x80) case is taken
                    gActiveSeqs[seqPlayerIndex].startSeqCmd = (cmd & ~0x8008000) + 0x8000000;
                    gActiveSeqs[seqPlayerIndex].isWaitingForFonts = true;
                    gActiveSeqs[seqPlayerIndex].fontId = *AudioThread_GetFontsForSequence(seqId, &outNumFonts);
                    Audio_StopSequence(seqPlayerIndex, 1);

                    if (gActiveSeqs[seqPlayerIndex].prevSeqId != NA_BGM_DISABLED) {
                        if (*AudioThread_GetFontsForSequence(seqId, &outNumFonts) !=
                            *AudioThread_GetFontsForSequence(gActiveSeqs[seqPlayerIndex].prevSeqId & 0xFF,
                                                             &outNumFonts)) {
                            // Discard Seq Fonts
                            AUDIOCMD_GLOBAL_DISCARD_SEQ_FONTS((s32)seqId);
                        }
                    }

                    AUDIOCMD_GLOBAL_ASYNC_LOAD_FONT(*AudioThread_GetFontsForSequence(seqId, &outNumFonts), 20,
                                                    (seqPlayerIndex + 1) & 0xFF & 0xFF);
                }
            }
            break;

        case SEQCMD_OP_STOP_SEQUENCE:
            // Stop a sequence and disable the sequence player
            fadeTimer = (cmd & 0xFF0000) >> 13;
            Audio_StopSequence(seqPlayerIndex, fadeTimer);
            break;

        case SEQCMD_OP_QUEUE_SEQUENCE:
            // Queue a sequence into `sSeqRequests`
            seqId = cmd & 0xFF;
            seqArgs = (cmd & 0xFF00) >> 8;
            fadeTimer = (cmd & 0xFF0000) >> 13;
            priority = seqArgs;

            // Checks if the requested sequence is first in the list of requests
            // If it is already queued and first in the list, then play the sequence immediately
            for (i = 0; i < sNumSeqRequests[seqPlayerIndex]; i++) {
                if (sSeqRequests[seqPlayerIndex][i].seqId == seqId) {
                    if (i == 0) {
                        Audio_StartSequence(seqPlayerIndex, seqId, seqArgs, fadeTimer);
                    }
                    return;
                }
            }

            // Searches the sequence requests for the first request that does not have a higher priority
            // than the current incoming request
            found = sNumSeqRequests[seqPlayerIndex];
            for (i = 0; i < sNumSeqRequests[seqPlayerIndex]; i++) {
                if (priority >= sSeqRequests[seqPlayerIndex][i].priority) {
                    found = i;
                    i = sNumSeqRequests[seqPlayerIndex]; // "break;"
                }
            }

            // Check if the queue is full
            if (sNumSeqRequests[seqPlayerIndex] < ARRAY_COUNT(sSeqRequests[seqPlayerIndex])) {
                sNumSeqRequests[seqPlayerIndex]++;
            }

            for (i = sNumSeqRequests[seqPlayerIndex] - 1; i != found; i--) {
                // Move all requests of lower priority backwards 1 place in the queue
                // If the queue is full, overwrite the entry with the lowest priority
                sSeqRequests[seqPlayerIndex][i].priority = sSeqRequests[seqPlayerIndex][i - 1].priority;
                sSeqRequests[seqPlayerIndex][i].seqId = sSeqRequests[seqPlayerIndex][i - 1].seqId;
            }

            // Fill the newly freed space in the queue with the new request
            sSeqRequests[seqPlayerIndex][found].priority = seqArgs;
            sSeqRequests[seqPlayerIndex][found].seqId = seqId;

            // The sequence is first in queue, so start playing.
            if (found == 0) {
                Audio_StartSequence(seqPlayerIndex, seqId, seqArgs, fadeTimer);
            }
            break;

        case SEQCMD_OP_UNQUEUE_SEQUENCE:
            // Unqueue sequence
            fadeTimer = (cmd & 0xFF0000) >> 13;

            found = sNumSeqRequests[seqPlayerIndex];
            for (i = 0; i < sNumSeqRequests[seqPlayerIndex]; i++) {
                seqId = cmd & 0xFF;
                if (sSeqRequests[seqPlayerIndex][i].seqId == seqId) {
                    found = i;
                    i = sNumSeqRequests[seqPlayerIndex]; // "break;"
                }
            }

            if (found != sNumSeqRequests[seqPlayerIndex]) {
                // Move all requests of lower priority forward 1 place in the queue
                for (i = found; i < sNumSeqRequests[seqPlayerIndex] - 1; i++) {
                    sSeqRequests[seqPlayerIndex][i].priority = sSeqRequests[seqPlayerIndex][i + 1].priority;
                    sSeqRequests[seqPlayerIndex][i].seqId = sSeqRequests[seqPlayerIndex][i + 1].seqId;
                }
                sNumSeqRequests[seqPlayerIndex]--;
            }

            // If the sequence was first in queue (it is currently playing),
            // Then stop the sequence and play the next sequence in the queue.
            if (found == 0) {
                Audio_StopSequence(seqPlayerIndex, fadeTimer);
                if (sNumSeqRequests[seqPlayerIndex] != 0) {
                    Audio_StartSequence(seqPlayerIndex, sSeqRequests[seqPlayerIndex][0].seqId,
                                        sSeqRequests[seqPlayerIndex][0].priority, fadeTimer);
                }
            }
            break;

        case SEQCMD_OP_SET_PLAYER_VOLUME:
            // Transition volume to a target volume for an entire player
            duration = (cmd & 0xFF0000) >> 15;
            val = cmd & 0xFF;
            if (duration == 0) {
                duration++;
            }
            // Volume is scaled relative to 127
            gActiveSeqs[seqPlayerIndex].volTarget = (f32)val / 127.0f;
            if (gActiveSeqs[seqPlayerIndex].volCur != gActiveSeqs[seqPlayerIndex].volTarget) {
                gActiveSeqs[seqPlayerIndex].volStep =
                    (gActiveSeqs[seqPlayerIndex].volCur - gActiveSeqs[seqPlayerIndex].volTarget) / (f32)duration;
                gActiveSeqs[seqPlayerIndex].volTimer = duration;
            }
            break;

        case SEQCMD_OP_SET_PLAYER_FREQ:
            // Transition freq scale to a target freq for all channels
            duration = (cmd & 0xFF0000) >> 15;
            val = cmd & 0xFFFF;
            if (duration == 0) {
                duration++;
            }
            // Frequency is scaled relative to 1000
            freqScaleTarget = (f32)val / 1000.0f;
            for (i = 0; i < SEQ_NUM_CHANNELS; i++) {
                gActiveSeqs[seqPlayerIndex].channelData[i].freqScaleTarget = freqScaleTarget;
                gActiveSeqs[seqPlayerIndex].channelData[i].freqScaleTimer = duration;
                gActiveSeqs[seqPlayerIndex].channelData[i].freqScaleStep =
                    (gActiveSeqs[seqPlayerIndex].channelData[i].freqScaleCur - freqScaleTarget) / (f32)duration;
            }
            gActiveSeqs[seqPlayerIndex].freqScaleChannelFlags = 0xFFFF;
            break;

        case SEQCMD_OP_SET_CHANNEL_FREQ:
            // Transition freq scale to a target for a specific channel
            duration = (cmd & 0xFF0000) >> 15;
            channelIndex = (cmd & 0xF000) >> 12;
            val = cmd & 0xFFF;
            if (duration == 0) {
                duration++;
            }
            // Frequency is scaled relative to 1000
            freqScaleTarget = (f32)val / 1000.0f;
            gActiveSeqs[seqPlayerIndex].channelData[channelIndex].freqScaleTarget = freqScaleTarget;
            gActiveSeqs[seqPlayerIndex].channelData[channelIndex].freqScaleStep =
                (gActiveSeqs[seqPlayerIndex].channelData[channelIndex].freqScaleCur - freqScaleTarget) / (f32)duration;
            gActiveSeqs[seqPlayerIndex].channelData[channelIndex].freqScaleTimer = duration;
            gActiveSeqs[seqPlayerIndex].freqScaleChannelFlags |= 1 << channelIndex;
            break;

        case SEQCMD_OP_SET_CHANNEL_VOLUME:
            // Transition volume to a target volume for a specific channel
            duration = (cmd & 0xFF0000) >> 15;
            channelIndex = (cmd & 0xF00) >> 8;
            val = cmd & 0xFF;
            if (duration == 0) {
                duration++;
            }
            // Volume is scaled relative to 127
            gActiveSeqs[seqPlayerIndex].channelData[channelIndex].volTarget = (f32)val / 127.0f;
            if (gActiveSeqs[seqPlayerIndex].channelData[channelIndex].volCur !=
                gActiveSeqs[seqPlayerIndex].channelData[channelIndex].volTarget) {
                gActiveSeqs[seqPlayerIndex].channelData[channelIndex].volStep =
                    (gActiveSeqs[seqPlayerIndex].channelData[channelIndex].volCur -
                     gActiveSeqs[seqPlayerIndex].channelData[channelIndex].volTarget) /
                    (f32)duration;
                gActiveSeqs[seqPlayerIndex].channelData[channelIndex].volTimer = duration;
                gActiveSeqs[seqPlayerIndex].volChannelFlags |= 1 << channelIndex;
            }
            break;

        case SEQCMD_OP_SET_PLAYER_IO:
            // Set global io port
            port = (cmd & 0xFF0000) >> 16;
            val = cmd & 0xFF;
            AUDIOCMD_SEQPLAYER_SET_IO(seqPlayerIndex, port, val);
            break;

        case SEQCMD_OP_SET_CHANNEL_IO:
            // Set io port if channel masked
            channelIndex = (cmd & 0xF00) >> 8;
            port = (cmd & 0xFF0000) >> 16;
            val = cmd & 0xFF;
            if (!(gActiveSeqs[seqPlayerIndex].channelPortMask & (1 << channelIndex))) {
                AUDIOCMD_CHANNEL_IO(seqPlayerIndex, channelIndex, port, val);
            }
            break;

        case SEQCMD_OP_SET_CHANNEL_IO_DISABLE_MASK:
            // Disable channel io specifically for
            // `SEQCMD_OP_SET_CHANNEL_IO` This can be bypassed by setting channel io through `Audio_QueueCmdS8` 0x6
            // directly. This is accomplished by setting a channel mask.
            gActiveSeqs[seqPlayerIndex].channelPortMask = cmd & 0xFFFF;
            break;

        case SEQCMD_OP_SET_CHANNEL_DISABLE_MASK:
            // Disable or Reenable channels

            // Disable channels
            channelMaskDisable = cmd & 0xFFFF;
            if (channelMaskDisable != 0) {
                // Apply channel mask `channelMaskDisable`
                AUDIOCMD_GLOBAL_SET_ACTIVE_CHANNEL_FLAGS(seqPlayerIndex, channelMaskDisable);
                // Disable channels
                AUDIOCMD_CHANNEL_STOP_SOMETHING2(seqPlayerIndex, SEQ_ALL_CHANNELS, true);
            }

            // Reenable channels
            channelMaskEnable = channelMaskDisable ^ 0xFFFF;
            if (channelMaskEnable != 0) {
                // Apply channel mask `channelMaskEnable`
                AUDIOCMD_GLOBAL_SET_ACTIVE_CHANNEL_FLAGS(seqPlayerIndex, channelMaskEnable);
                // Enable channels
                AUDIOCMD_CHANNEL_STOP_SOMETHING2(seqPlayerIndex, SEQ_ALL_CHANNELS, false);
            }
            break;

        case SEQCMD_OP_TEMPO_CMD:
            // Update a tempo using a sub-command system.
            // Stores the cmd for processing elsewhere.
            gActiveSeqs[seqPlayerIndex].tempoCmd = cmd;
            break;

        case SEQCMD_OP_SETUP_CMD:
            // Queue a sub-command to execute once the sequence is finished playing
            subOp = (cmd & 0xF00000) >> 20;
            if (subOp != SEQCMD_SUB_OP_SETUP_RESET_SETUP_CMDS) {
                // Ensure the maximum number of setup commands is not exceeded
                if (gActiveSeqs[seqPlayerIndex].setupCmdNum < (ARRAY_COUNT(gActiveSeqs[seqPlayerIndex].setupCmd) - 1)) {
                    found = gActiveSeqs[seqPlayerIndex].setupCmdNum++;
                    if (found < ARRAY_COUNT(gActiveSeqs[seqPlayerIndex].setupCmd)) {
                        gActiveSeqs[seqPlayerIndex].setupCmd[found] = cmd;
                        // Adds a delay of 2 frames before executing any setup commands.
                        // This allows setup commands to be requested along with a new sequence on a seqPlayerIndex.
                        // This 2 frame delay ensures the player is enabled before its state is checked for
                        // the purpose of deciding if the setup commands should be run.
                        // Otherwise, the setup commands will be executed before the sequence starts,
                        // when the player is still disabled, instead of when the newly played sequence ends.
                        gActiveSeqs[seqPlayerIndex].setupCmdTimer = 2;
                    }
                }
            } else {
                // `SEQCMD_SUB_OP_SETUP_RESET_SETUP_CMDS`
                // Discard all setup command requests on `seqPlayerIndex`
                gActiveSeqs[seqPlayerIndex].setupCmdNum = 0;
            }
            break;

        case SEQCMD_OP_GLOBAL_CMD:
            // Apply a command that applies to all sequence players
            subOp = (cmd & 0xF00) >> 8;
            val = cmd & 0xFF;
            switch (subOp) {
                case SEQCMD_SUB_OP_GLOBAL_SET_SOUND_MODE:
                    // Set sound mode
                    AUDIOCMD_GLOBAL_SET_SOUND_MODE(sSoundModeList[val]);
                    break;

                case 1:
                    // set sequence disabled true in (sStartSeqDisabled & 1) bit
                    sStartSeqDisabled = (sStartSeqDisabled & 0xFE) | (u8)(val & 1);
                    break;

                case 2:
                    // set sequence disabled true in (sStartSeqDisabled & 2) bit
                    sStartSeqDisabled = (sStartSeqDisabled & 0xFD) | (u8)((val & 1) * 2);
                    break;
            }
            break;

        case SEQCMD_OP_RESET_AUDIO_HEAP:
            // Resets the audio heap based on the audio specifications, audio mode, and sfx channel layout
            specId = cmd & 0xFF;
            specMode = (cmd & 0xFF0000) >> 16;
            if (specMode == 0) {
                gSfxChannelLayout = (cmd & 0xFF00) >> 8;
                oldSpecId = gAudioSpecId;
                gAudioSpecId = specId;
                func_80193D08(specId);
                func_801A4DF4(oldSpecId);
                AUDIOCMD_GLOBAL_STOP_AUDIOCMDS();
            } else {
                sChangeSpecCmd = cmd;
                D_80200BCE = 0x7FFF;
                D_80200BCC = 0x14;
                D_80200BD0 = 0x666;
            }
            break;
    }
}

/**
 * Add the sequence cmd to the `sAudioSeqCmds` queue
 */
void Audio_QueueSeqCmd(u32 cmd) {
    sAudioSeqCmds[sSeqCmdWritePos++] = cmd;
}

void Audio_ProcessSeqCmds(void) {
    while (sSeqCmdWritePos != sSeqCmdReadPos) {
        Audio_ProcessSeqCmd(sAudioSeqCmds[sSeqCmdReadPos++]);
    }
}

u16 Audio_GetActiveSeqId(u8 seqPlayerIndex) {
    if (gActiveSeqs[seqPlayerIndex].isWaitingForFonts == true) {
        return gActiveSeqs[seqPlayerIndex].startSeqCmd & 0xFF;
    }

    if (gActiveSeqs[seqPlayerIndex].seqId != NA_BGM_DISABLED) {
        return gActiveSeqs[seqPlayerIndex].seqId;
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
void Audio_ResetSequenceRequests(u8 seqPlayerIndex) {
    sNumSeqRequests[seqPlayerIndex] = 0;
}

/**
 * Check if the setup command is queued. If it is, then replace the command
 * with `SEQCMD_SUB_OP_SETUP_RESTORE_VOLUME`.
 * Unused
 */
void Audio_ReplaceSeqCmdSetupOpVolRestore(u8 seqPlayerIndex, u8 setupOpDisabled) {
    u8 i;

    for (i = 0; i < gActiveSeqs[seqPlayerIndex].setupCmdNum; i++) {
        u8 setupOp = (gActiveSeqs[seqPlayerIndex].setupCmd[i] & 0xF00000) >> 20;

        if (setupOp == setupOpDisabled) {
            gActiveSeqs[seqPlayerIndex].setupCmd[i] = 0xFF000000;
        }
    }
}

void Audio_SetVolumeScale(u8 seqPlayerIndex, u8 scaleIndex, u8 targetVol, u8 volFadeTimer) {
    f32 volScale;
    u8 i;

    gActiveSeqs[seqPlayerIndex].volScales[scaleIndex] = targetVol & 0x7F;

    if (volFadeTimer != 0) {
        gActiveSeqs[seqPlayerIndex].fadeVolUpdate = true;
        gActiveSeqs[seqPlayerIndex].volFadeTimer = volFadeTimer;
    } else {
        for (i = 0, volScale = 1.0f; i < VOL_SCALE_INDEX_MAX; i++) {
            volScale *= gActiveSeqs[seqPlayerIndex].volScales[i] / 127.0f;
        }

        SEQCMD_SET_PLAYER_VOLUME_NOW(seqPlayerIndex, volFadeTimer, volScale);
    }
}

/**
 * Update different commands and requests for active sequences
 */
void Audio_UpdateActiveSequences(void) {
    u32 tempoCmd;
    u16 tempoPrev;
    u16 seqId;
    u16 channelMask;
    u16 tempoTarget;
    u8 setupOp;
    u8 targetSeqPlayerIndex;
    u8 setupVal2;
    u8 setupVal1;
    u8 tempoOp;
    s32 pad[2];
    u32 retMsg;
    f32 volume;
    u8 tempoTimer;
    u8 seqPlayerIndex;
    u8 j;
    u8 channelIndex;

    for (seqPlayerIndex = 0; seqPlayerIndex < ARRAY_COUNT(gActiveSeqs); seqPlayerIndex++) {

        // The seqPlayer has finished initializing and is currently playing the active sequences
        if (gActiveSeqs[seqPlayerIndex].isSeqPlayerInit && gAudioContext.seqPlayers[seqPlayerIndex].enabled) {
            gActiveSeqs[seqPlayerIndex].isSeqPlayerInit = false;
        }

        // The seqPlayer is no longer playing the active sequences
        if ((Audio_GetActiveSeqId(seqPlayerIndex) != NA_BGM_DISABLED) &&
            !gAudioContext.seqPlayers[seqPlayerIndex].enabled && (!gActiveSeqs[seqPlayerIndex].isSeqPlayerInit)) {
            gActiveSeqs[seqPlayerIndex].seqId = NA_BGM_DISABLED;
        }

        // Check if the requested sequences is waiting for fonts to load
        if (gActiveSeqs[seqPlayerIndex].isWaitingForFonts) {
            switch ((s32)func_80193BA0(&retMsg)) {
                case SEQ_PLAYER_BGM_MAIN + 1:
                case SEQ_PLAYER_FANFARE + 1:
                case SEQ_PLAYER_SFX + 1:
                case SEQ_PLAYER_BGM_SUB + 1:
                case SEQ_PLAYER_AMBIENCE + 1:
                    // The fonts have been loaded successfully.
                    gActiveSeqs[seqPlayerIndex].isWaitingForFonts = false;
                    // Queue the same command that was stored previously, but without the 0x8000
                    Audio_ProcessSeqCmd(gActiveSeqs[seqPlayerIndex].startSeqCmd);
                    break;
                case 0xFF:
                    // There was an error in loading the fonts
                    gActiveSeqs[seqPlayerIndex].isWaitingForFonts = false;
                    break;
            }
        }

        // Update global volume
        if (gActiveSeqs[seqPlayerIndex].fadeVolUpdate) {
            volume = 1.0f;
            for (j = 0; j < VOL_SCALE_INDEX_MAX; j++) {
                volume *= (gActiveSeqs[seqPlayerIndex].volScales[j] / 127.0f);
            }

            SEQCMD_SET_PLAYER_VOLUME((u8)(seqPlayerIndex + 8), gActiveSeqs[seqPlayerIndex].volFadeTimer,
                                     (u8)(volume * 127.0f));
            gActiveSeqs[seqPlayerIndex].fadeVolUpdate = false;
        }

        if (gActiveSeqs[seqPlayerIndex].volTimer != 0) {
            gActiveSeqs[seqPlayerIndex].volTimer--;

            if (gActiveSeqs[seqPlayerIndex].volTimer != 0) {
                gActiveSeqs[seqPlayerIndex].volCur -= gActiveSeqs[seqPlayerIndex].volStep;
            } else {
                gActiveSeqs[seqPlayerIndex].volCur = gActiveSeqs[seqPlayerIndex].volTarget;
            }

            AUDIOCMD_SEQPLAYER_FADE_VOLUME_SCALE(seqPlayerIndex, gActiveSeqs[seqPlayerIndex].volCur);
        }

        // Process tempo
        if (gActiveSeqs[seqPlayerIndex].tempoCmd != 0) {
            tempoCmd = gActiveSeqs[seqPlayerIndex].tempoCmd;
            tempoTimer = (tempoCmd & 0xFF0000) >> 15;
            tempoTarget = tempoCmd & 0xFFF;
            if (tempoTimer == 0) {
                tempoTimer++;
            }

            // Process tempo commands
            if (gAudioContext.seqPlayers[seqPlayerIndex].enabled) {
                tempoPrev = gAudioContext.seqPlayers[seqPlayerIndex].tempo / TATUMS_PER_BEAT;
                tempoOp = (tempoCmd & 0xF000) >> 12;
                switch (tempoOp) {
                    case SEQCMD_SUB_OP_TEMPO_SPEED_UP:
                        // Speed up tempo by `tempoTarget` amount
                        tempoTarget += tempoPrev;
                        break;

                    case SEQCMD_SUB_OP_TEMPO_SLOW_DOWN:
                        // Slow down tempo by `tempoTarget` amount
                        if (tempoTarget < tempoPrev) {
                            tempoTarget = tempoPrev - tempoTarget;
                        }
                        break;

                    case SEQCMD_SUB_OP_TEMPO_SCALE:
                        // Scale tempo by a multiplicative factor
                        tempoTarget = tempoPrev * (tempoTarget / 100.0f);
                        break;

                    case SEQCMD_SUB_OP_TEMPO_RESET:
                        // Reset tempo to original tempo
                        tempoTarget = (gActiveSeqs[seqPlayerIndex].tempoOriginal != 0)
                                          ? gActiveSeqs[seqPlayerIndex].tempoOriginal
                                          : tempoPrev;
                        break;

                    default: // `SEQCMD_SUB_OP_TEMPO_SET`
                        // `tempoTarget` is the new tempo
                        break;
                }

                if (gActiveSeqs[seqPlayerIndex].tempoOriginal == 0) {
                    gActiveSeqs[seqPlayerIndex].tempoOriginal = tempoPrev;
                }

                gActiveSeqs[seqPlayerIndex].tempoTarget = tempoTarget;
                gActiveSeqs[seqPlayerIndex].tempoCur = gAudioContext.seqPlayers[seqPlayerIndex].tempo / 0x30;
                gActiveSeqs[seqPlayerIndex].tempoStep =
                    (gActiveSeqs[seqPlayerIndex].tempoCur - gActiveSeqs[seqPlayerIndex].tempoTarget) / tempoTimer;
                gActiveSeqs[seqPlayerIndex].tempoTimer = tempoTimer;
                gActiveSeqs[seqPlayerIndex].tempoCmd = 0;
            }
        }

        // Step tempo to target
        if (gActiveSeqs[seqPlayerIndex].tempoTimer != 0) {
            gActiveSeqs[seqPlayerIndex].tempoTimer--;
            if (gActiveSeqs[seqPlayerIndex].tempoTimer != 0) {
                gActiveSeqs[seqPlayerIndex].tempoCur -= gActiveSeqs[seqPlayerIndex].tempoStep;
            } else {
                gActiveSeqs[seqPlayerIndex].tempoCur = gActiveSeqs[seqPlayerIndex].tempoTarget;
            }
            // Set tempo
            AUDIOCMD_SEQPLAYER_SET_TEMPO(seqPlayerIndex, gActiveSeqs[seqPlayerIndex].tempoCur);
        }

        // Update channel volumes
        if (gActiveSeqs[seqPlayerIndex].volChannelFlags != 0) {
            for (channelIndex = 0; channelIndex < SEQ_NUM_CHANNELS; channelIndex++) {
                if (gActiveSeqs[seqPlayerIndex].channelData[channelIndex].volTimer != 0) {
                    gActiveSeqs[seqPlayerIndex].channelData[channelIndex].volTimer--;
                    if (gActiveSeqs[seqPlayerIndex].channelData[channelIndex].volTimer != 0) {
                        gActiveSeqs[seqPlayerIndex].channelData[channelIndex].volCur -=
                            gActiveSeqs[seqPlayerIndex].channelData[channelIndex].volStep;
                    } else {
                        gActiveSeqs[seqPlayerIndex].channelData[channelIndex].volCur =
                            gActiveSeqs[seqPlayerIndex].channelData[channelIndex].volTarget;
                        gActiveSeqs[seqPlayerIndex].volChannelFlags ^= (1 << channelIndex);
                    }

                    AUDIOCMD_CHANNEL_VOL_SCALE(seqPlayerIndex, channelIndex,
                                               gActiveSeqs[seqPlayerIndex].channelData[channelIndex].volCur);
                }
            }
        }

        // Update frequencies
        if (gActiveSeqs[seqPlayerIndex].freqScaleChannelFlags != 0) {
            for (channelIndex = 0; channelIndex < SEQ_NUM_CHANNELS; channelIndex++) {
                if (gActiveSeqs[seqPlayerIndex].channelData[channelIndex].freqScaleTimer != 0) {
                    gActiveSeqs[seqPlayerIndex].channelData[channelIndex].freqScaleTimer--;
                    if (gActiveSeqs[seqPlayerIndex].channelData[channelIndex].freqScaleTimer != 0) {
                        gActiveSeqs[seqPlayerIndex].channelData[channelIndex].freqScaleCur -=
                            gActiveSeqs[seqPlayerIndex].channelData[channelIndex].freqScaleStep;
                    } else {
                        gActiveSeqs[seqPlayerIndex].channelData[channelIndex].freqScaleCur =
                            gActiveSeqs[seqPlayerIndex].channelData[channelIndex].freqScaleTarget;
                        gActiveSeqs[seqPlayerIndex].freqScaleChannelFlags ^= (1 << channelIndex);
                    }

                    AUDIOCMD_CHANNEL_FREQ_SCALE(seqPlayerIndex, channelIndex,
                                                gActiveSeqs[seqPlayerIndex].channelData[channelIndex].freqScaleCur);
                }
            }
        }

        // Process setup commands
        if (gActiveSeqs[seqPlayerIndex].setupCmdNum != 0) {
            // If there is a SeqCmd to reset the audio heap queued, then drop all setup commands
            if (!Audio_IsSeqCmdNotQueued(SEQCMD_OP_RESET_AUDIO_HEAP << 28, SEQCMD_OP_MASK)) {
                gActiveSeqs[seqPlayerIndex].setupCmdNum = 0;
                return;
            }

            // Only process setup commands once the timer reaches zero
            if (gActiveSeqs[seqPlayerIndex].setupCmdTimer != 0) {
                gActiveSeqs[seqPlayerIndex].setupCmdTimer--;
                continue;
            }

            // Only process setup commands if `seqPlayerIndex` if no longer playing
            // i.e. the `seqPlayer` is no longer enabled
            if (gAudioContext.seqPlayers[seqPlayerIndex].enabled) {
                continue;
            }

            for (j = 0; j < gActiveSeqs[seqPlayerIndex].setupCmdNum; j++) {
                setupOp = (gActiveSeqs[seqPlayerIndex].setupCmd[j] & 0x00F00000) >> 20;
                targetSeqPlayerIndex = (gActiveSeqs[seqPlayerIndex].setupCmd[j] & 0x000F0000) >> 16;
                setupVal2 = (gActiveSeqs[seqPlayerIndex].setupCmd[j] & 0xFF00) >> 8;
                setupVal1 = gActiveSeqs[seqPlayerIndex].setupCmd[j] & 0xFF;

                switch (setupOp) {
                    case SEQCMD_SUB_OP_SETUP_RESTORE_VOLUME:
                        // Restore `targetSeqPlayerIndex` volume back to normal levels
                        Audio_SetVolumeScale(targetSeqPlayerIndex, VOL_SCALE_INDEX_FANFARE, 0x7F, setupVal1);
                        break;

                    case SEQCMD_SUB_OP_SETUP_RESTORE_VOLUME_IF_QUEUED:
                        // Restore `targetSeqPlayerIndex` volume back to normal levels,
                        // but only if the number of sequence queue requests from `sSeqRequests`
                        // exactly matches the argument to the command
                        if (setupVal1 == sNumSeqRequests[seqPlayerIndex]) {
                            Audio_SetVolumeScale(targetSeqPlayerIndex, VOL_SCALE_INDEX_FANFARE, 0x7F, setupVal2);
                        }
                        break;

                    case SEQCMD_SUB_OP_SETUP_SEQ_UNQUEUE:
                        // Unqueue `seqPlayerIndex` from sSeqRequests
                        //! @bug this command does not work as intended as unqueueing
                        //! the sequence relies on `gActiveSeqs[seqPlayerIndex].seqId`
                        //! However, `gActiveSeqs[seqPlayerIndex].seqId` is reset before the sequence on
                        //! `seqPlayerIndex` is requested to stop, i.e. before the sequence is disabled and setup
                        //! commands (including this command) can run. A simple fix would have been to unqueue based on
                        //! `gActiveSeqs[seqPlayerIndex].prevSeqId` instead
                        SEQCMD_UNQUEUE_SEQUENCE((u8)(seqPlayerIndex + 8), 0, gActiveSeqs[seqPlayerIndex].seqId);
                        break;

                    case SEQCMD_SUB_OP_SETUP_RESTART_SEQ:
                        // Restart the currently active sequence on `targetSeqPlayerIndex` with full volume.
                        // Sequence on `targetSeqPlayerIndex` must still be active to play (can be muted)
                        SEQCMD_PLAY_SEQUENCE((u8)(targetSeqPlayerIndex + 8), 1,
                                             gActiveSeqs[targetSeqPlayerIndex].seqId);
                        gActiveSeqs[targetSeqPlayerIndex].fadeVolUpdate = true;
                        gActiveSeqs[targetSeqPlayerIndex].volScales[1] = 0x7F;
                        break;

                    case SEQCMD_SUB_OP_SETUP_TEMPO_SCALE:
                        // Scale tempo by a multiplicative factor
                        SEQCMD_SCALE_TEMPO((u8)(targetSeqPlayerIndex + 8), setupVal2, setupVal1);
                        break;

                    case SEQCMD_SUB_OP_SETUP_TEMPO_RESET:
                        // Reset tempo to previous tempo
                        SEQCMD_RESET_TEMPO((u8)(targetSeqPlayerIndex + 8), setupVal1);
                        break;

                    case SEQCMD_SUB_OP_SETUP_PLAY_SEQ:
                        // Play the requested sequence
                        // Uses the fade timer set by `SEQCMD_SUB_OP_SETUP_SET_FADE_TIMER`
                        seqId = gActiveSeqs[seqPlayerIndex].setupCmd[j] & 0xFFFF;
                        SEQCMD_PLAY_SEQUENCE((u8)(targetSeqPlayerIndex + 8),
                                             gActiveSeqs[targetSeqPlayerIndex].setupFadeTimer, seqId);
                        Audio_SetVolumeScale(targetSeqPlayerIndex, VOL_SCALE_INDEX_FANFARE, 0x7F, 0);
                        gActiveSeqs[targetSeqPlayerIndex].setupFadeTimer = 0;
                        break;

                    case SEQCMD_SUB_OP_SETUP_SET_FADE_TIMER:
                        // A command specifically to support `SEQCMD_SUB_OP_SETUP_PLAY_SEQ`
                        // Sets the fade timer for the sequence requested in `SEQCMD_SUB_OP_SETUP_PLAY_SEQ`
                        gActiveSeqs[seqPlayerIndex].setupFadeTimer = setupVal2;
                        break;

                    case SEQCMD_SUB_OP_SETUP_RESTORE_VOLUME_WITH_SCALE_INDEX:
                        // Restore the volume back to default levels
                        // Allows a `scaleIndex` to be specified.
                        Audio_SetVolumeScale(targetSeqPlayerIndex, setupVal2, 0x7F, setupVal1);
                        break;

                    case SEQCMD_SUB_OP_SETUP_POP_PERSISTENT_CACHE:
                        // Discard audio data by popping one more audio caches from the audio heap
                        if (setupVal1 & (1 << SEQUENCE_TABLE)) {
                            AUDIOCMD_GLOBAL_POP_PERSISTENT_CACHE(SEQUENCE_TABLE);
                        }
                        if (setupVal1 & (1 << FONT_TABLE)) {
                            AUDIOCMD_GLOBAL_POP_PERSISTENT_CACHE(FONT_TABLE);
                        }
                        if (setupVal1 & (1 << SAMPLE_TABLE)) {
                            AUDIOCMD_GLOBAL_POP_PERSISTENT_CACHE(SAMPLE_TABLE);
                        }
                        break;

                    case SEQCMD_SUB_OP_SETUP_SET_CHANNEL_DISABLE_MASK:
                        // Disable (or reenable) specific channels of `targetSeqPlayerIndex`
                        channelMask = gActiveSeqs[seqPlayerIndex].setupCmd[j] & 0xFFFF;
                        SEQCMD_SET_CHANNEL_DISABLE_MASK((u8)(targetSeqPlayerIndex + 8), channelMask);
                        break;

                    case SEQCMD_SUB_OP_SETUP_SET_PLAYER_FREQ:
                        // Scale all channels of `targetSeqPlayerIndex`
                        SEQCMD_SET_PLAYER_FREQ((u8)(targetSeqPlayerIndex + 8), setupVal2, (setupVal1 * 10) & 0xFFFF);
                        break;

                    default:
                        break;
                }
            }

            gActiveSeqs[seqPlayerIndex].setupCmdNum = 0;
        }
    }
}

u8 func_801A9768(void) {
    if (D_801DB4D8 != 0) {
        if (D_801DB4D8 == 1) {
            if (func_80193C5C() == 1) {
                D_801DB4D8 = 0;
                AUDIOCMD_SEQPLAYER_SET_IO(SEQ_PLAYER_SFX, 0, gSfxChannelLayout);
                func_801A4DA4();
            }
        } else if (D_801DB4D8 == 2) {
            while (func_80193C5C() != 1) {}
            D_801DB4D8 = 0;
            AUDIOCMD_SEQPLAYER_SET_IO(SEQ_PLAYER_SFX, 0, gSfxChannelLayout);
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
                    // Set Reverb Data, dataType = REVERB_DATA_TYPE_VOLUME, reverbIndex = reverbIndex, data =
                    // D_80200BCE, flags = 0 dataType = REVERB_DATA_TYPE_VOLUME:
                    //      synthesisReverbs[reverbIndex].volume = D_80200BCE
                    AUDIOCMD_GLOBAL_SET_REVERB_DATA(reverbIndex, REVERB_DATA_TYPE_VOLUME, D_80200BCE);
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
                    // Set Reverb Data, dataType = REVERB_DATA_TYPE_SETTINGS, reverbIndex = reverbIndex, data =
                    // gReverbSettingsTable, flags = 0 dataType = REVERB_DATA_TYPE_SETTINGS:
                    //      AudioHeap_InitReverb(reverbIndex, gReverbSettingsTable[sChangeSpecCmd] + new_var, 0);
                    AUDIOCMD_GLOBAL_SET_REVERB_DATA(
                        reverbIndex, REVERB_DATA_TYPE_SETTINGS,
                        (s32)(gReverbSettingsTable[sChangeSpecCmd & 0xFF & 0xFF] + reverbIndex));
                    AudioThread_ScheduleProcessCmds();
                }
                reverbIndex++;
                specMode = specMode >> 1;
            }

            sChangeSpecCmd = 0;
            AUDIOCMD_SEQPLAYER_SET_IO(SEQ_PLAYER_SFX, 0, gSfxChannelLayout);
            func_801A4D50();
        }
    }

    return ret;
}

void Audio_ResetActiveSequences(void) {
    u8 seqPlayerIndex;
    u8 scaleIndex;

    for (seqPlayerIndex = 0; seqPlayerIndex < 5; seqPlayerIndex++) {
        sNumSeqRequests[seqPlayerIndex] = 0;

        gActiveSeqs[seqPlayerIndex].seqId = NA_BGM_DISABLED;
        gActiveSeqs[seqPlayerIndex].prevSeqId = NA_BGM_DISABLED;
        gActiveSeqs[seqPlayerIndex].tempoTimer = 0;
        gActiveSeqs[seqPlayerIndex].tempoOriginal = 0;
        gActiveSeqs[seqPlayerIndex].tempoCmd = 0;
        gActiveSeqs[seqPlayerIndex].channelPortMask = 0;
        gActiveSeqs[seqPlayerIndex].setupCmdNum = 0;
        gActiveSeqs[seqPlayerIndex].setupFadeTimer = 0;
        gActiveSeqs[seqPlayerIndex].freqScaleChannelFlags = 0;
        gActiveSeqs[seqPlayerIndex].volChannelFlags = 0;
        gActiveSeqs[seqPlayerIndex].isWaitingForFonts = false;
        gActiveSeqs[seqPlayerIndex].isSeqPlayerInit = false;

        for (scaleIndex = 0; scaleIndex < VOL_SCALE_INDEX_MAX; scaleIndex++) {
            gActiveSeqs[seqPlayerIndex].volScales[scaleIndex] = 0x7F;
        }

        gActiveSeqs[seqPlayerIndex].volFadeTimer = 1;
        gActiveSeqs[seqPlayerIndex].fadeVolUpdate = true;
    }
}

void Audio_ResetActiveSequencesAndVolume(void) {
    u8 seqPlayerIndex;
    u8 scaleIndex;

    for (seqPlayerIndex = 0; seqPlayerIndex < 5; seqPlayerIndex++) {
        gActiveSeqs[seqPlayerIndex].volCur = 1.0f;
        gActiveSeqs[seqPlayerIndex].volTimer = 0;
        gActiveSeqs[seqPlayerIndex].fadeVolUpdate = false;
        for (scaleIndex = 0; scaleIndex < VOL_SCALE_INDEX_MAX; scaleIndex++) {
            gActiveSeqs[seqPlayerIndex].volScales[scaleIndex] = 0x7F;
        }
    }
    Audio_ResetActiveSequences();
}
