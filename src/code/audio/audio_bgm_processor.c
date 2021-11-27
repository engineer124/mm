#include "global.h"

void func_801A4DA4(void);

u8 sSeqCmdWritePos = 0;
u8 sSeqCmdReadPos = 0;
u8 isBgmStartDisabled = 0;
u8 sSoundModes[] = {
    AUDIO_MODE_STEREO, 
    AUDIO_MODE_HEADSET, 
    AUDIO_MODE_UNK, 
    AUDIO_MODE_MONO, 
    AUDIO_MODE_SURROUND,
};
u8 gAudioSpecId = 0;
u8 D_801DB4D8 = 0;
u32 D_801DB4DC = 0;

ReverbSettings gReverbSettings[46] = {
    /* 0x00 */ { 1, 0x30, 0x3000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 },
    /* 0x01 */ { 1, 0x20, 0x0800, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x0000, 0x0, 0x0 },
    /* 0x02 */ { 0 },
    /* 0x03 */ { 1, 0x30, 0x3000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 },
    /* 0x04 */ { 1, 0x30, 0x1800, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x0000, 0xB, 0xB },
    /* 0x05 */ { 0 },
    /* 0x06 */ { 1, 0x30, 0x3000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 },
    /* 0x07 */ { 1, 0x38, 0x2800, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x0000, 0x7, 0x7 },
    /* 0x08 */ { 0 },
    /* 0x09 */ { 1, 0x30, 0x3000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 },
    /* 0x0A */ { 1, 0x30, 0x6800, 0, 0, 0x7FFF, 0x1400, 0x1400, 0xFF, 0x3000, 0x6, 0x6 },
    /* 0x0B */ { 2, 0x50, 0x6000, 0, 0, 0x7FFF, 0xD000, 0x3000, 0xFF, 0x3000, 0x0, 0x0 },
    /* 0x0C */ { 1, 0x30, 0x3000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 },
    /* 0x0D */ { 1, 0x40, 0x5000, 0, 0, 0x7FFF, 0x1800, 0x1800, 0xFF, 0x3000, 0x7, 0x7 },
    /* 0x0E */ { 0 },
    /* 0x0F */ { 1, 0x30, 0x3000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 },
    /* 0x10 */ { 1, 0x40, 0x5C00, 0, 0, 0x7FFF, 0x2000, 0x2000, 0xFF, 0x3000, 0x4, 0x4 },
    /* 0x11 */ { 0 },
    /* 0x12 */ { 1, 0x30, 0x3000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 },
    /* 0x13 */ { 1, 0x30, 0x6000, 0, 0, 0x7FFF, 0x1000, 0x1000, 0xFF, 0x3000, 0xA, 0xA },
    /* 0x14 */ { 0 },
    /* 0x15 */ { 1, 0x30, 0x3000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 },
    /* 0x16 */ { 1, 0x30, 0x6800, 0, 0, 0x7FFF, 0x1400, 0x1400, 0xFF, 0x3000, 0x6, 0x6 },
    /* 0x17 */ { 0 },
    /* 0x18 */ { 1, 0x30, 0x3000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 },
    /* 0x19 */ { 1, 0x50, 0x5000, 0, 0, 0x7FFF, 0xD000, 0x3000, 0xFF, 0x3000, 0x0, 0x0 },
    /* 0x1A */ { 1, 0x30, 0x3000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 },
    /* 0x1B */ { 1, 0x20, 0x0000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x0000, 0x0, 0x0 },
    /* 0x1C */ { 0 },
    /* 0x1D */ { 1, 0x30, 0x3000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 },
    /* 0x1E */ { 1, 0x30, 0x1800, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x0000, 0xB, 0xB },
    /* 0x1F */ { 0 },
    /* 0x20 */ { 1, 0x30, 0x3000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 },
    /* 0x21 */ { 0 },
    /* 0x22 */ { 0 },
    /* 0x23 */ { 1, 0x30, 0x3000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 },
    /* 0x24 */ { 1, 0x40, 0x5000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 },
    /* 0x25 */ { 0 },
    /* 0x26 */ { 1, 0x30, 0x3000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 },
    /* 0x27 */ { 1, 0x30, 0x6800, 0, 0, 0x7FFF, 0x1400, 0x1400, 0xFF, 0x3000, 0x6, 0x6 },
    /* 0x28 */ { 2, 0x50, 0x6000, 0, 0, 0x7FFF, 0xD000, 0x3000, 0xFF, 0x3000, 0x0, 0x0 },
    /* 0x29 */ { 1, 0x30, 0x3000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 },
    /* 0x2A */ { 1, 0x30, 0x1800, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x0000, 0xB, 0xB },
    /* 0x2B */ { 1, 0x40, 0x5000, 0, 0, 0x7FFF, 0x1800, 0x1800, 0xFF, 0x3000, 0x7, 0x7 },
    /* 0x2C */ { 1, 0x30, 0x3000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 },
    /* 0x2D */ { 1, 0x50, 0x1800, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x0000, 0xB, 0xB },
};

ReverbSettings* D_801DB930[] = {
    &gReverbSettings[0], // 0x00
    &gReverbSettings[3], // 0x03
    &gReverbSettings[6], // 0x06
    &gReverbSettings[12], // 0x0C
    &gReverbSettings[15], // 0x0F
    &gReverbSettings[18], // 0x12
    &gReverbSettings[21], // 0x15
    &gReverbSettings[24], // 0x18
    &gReverbSettings[26], // 0x1A
    &gReverbSettings[9], // 0x09
};

AudioSpec gAudioSpecs[21] = {
    { 32000, 1, 24, 5, { 0 }, 2, { 0 }, &gReverbSettings[44], 0x500, 0x200, 0x7FFF, 0xAF0, 0x2D80, 0, 0x4100, 0x2D00, 0, 0, 0xDC800 },
    { 32000, 1, 24, 5, { 0 }, 2, { 0 }, &gReverbSettings[44], 0x500, 0x200, 0x7FFF, 0xAF0, 0x2D80, 0, 0x4100, 0x2D00, 0, 0, 0xDC800 },
    { 32000, 1, 24, 5, { 0 }, 2, { 0 }, &gReverbSettings[44], 0x500, 0x200, 0x7FFF, 0xAF0, 0x2D80, 0, 0x4100, 0x2D00, 0, 0, 0xDC800 },
    { 32000, 1, 24, 5, { 0 }, 2, { 0 }, &gReverbSettings[44], 0x500, 0x200, 0x7FFF, 0xAF0, 0x2D80, 0, 0x4100, 0x2D00, 0, 0, 0xDC800 },
    { 32000, 1, 24, 5, { 0 }, 2, { 0 }, &gReverbSettings[44], 0x500, 0x200, 0x7FFF, 0xAF0, 0x2D80, 0, 0x4100, 0x2D00, 0, 0, 0xDC800 },
    { 32000, 1, 24, 5, { 0 }, 2, { 0 }, &gReverbSettings[44], 0x500, 0x200, 0x7FFF, 0xAF0, 0x2D80, 0, 0x4100, 0x2D00, 0, 0, 0xDC800 },
    { 32000, 1, 24, 5, { 0 }, 2, { 0 }, &gReverbSettings[44], 0x500, 0x200, 0x7FFF, 0xAF0, 0x2D80, 0, 0x4100, 0x2D00, 0, 0, 0xDC800 },
    { 32000, 1, 24, 5, { 0 }, 2, { 0 }, &gReverbSettings[44], 0x500, 0x200, 0x7FFF, 0xAF0, 0x2D80, 0, 0x4100, 0x2D00, 0, 0, 0xDC800 },
    { 32000, 1, 24, 5, { 0 }, 2, { 0 }, &gReverbSettings[44], 0x500, 0x200, 0x7FFF, 0xAF0, 0x2D80, 0, 0x4100, 0x2D00, 0, 0, 0xDC800 },
    { 32000, 1, 24, 5, { 0 }, 2, { 0 }, &gReverbSettings[44], 0x500, 0x200, 0x7FFF, 0xAF0, 0x2D80, 0, 0x4100, 0x2D00, 0, 0, 0xDC800 },
    { 32000, 1, 28, 3, { 0 }, 2, { 0 }, &gReverbSettings[29], 0x500, 0x200, 0x7FFF, 0xAF0, 0x2D80, 0, 0x2800, 0x2D00, 0, 0, 0xDC800 },
    { 32000, 1, 28, 3, { 0 }, 2, { 0 }, &gReverbSettings[29], 0x500, 0x200, 0x7FFF, 0xAF0, 0x2D80, 0, 0x4100, 0x2D00, 0, 0, 0xDC800 },
    { 32000, 1, 28, 5, { 0 }, 2, { 0 }, &gReverbSettings[29], 0x500, 0x200, 0x7FFF, 0xAF0, 0x2D80, 0, 0x4100, 0x2D00, 0, 0, 0xCC800 },
    { 32000, 1, 24, 5, { 0 }, 3, { 0 }, &gReverbSettings[38], 0x500, 0x200, 0x7FFF, 0xAF0, 0x2D80, 0, 0x4100, 0x2D00, 0, 0, 0xDC800 },
    { 32000, 1, 24, 5, { 0 }, 3, { 0 }, &gReverbSettings[41], 0x500, 0x200, 0x7FFF, 0xAF0, 0x2D80, 0, 0x4100, 0x2D00, 0, 0, 0xDC800 },
    { 32000, 1, 24, 5, { 0 }, 2, { 0 }, &gReverbSettings[44], 0x500, 0x200, 0x7FFF, 0xAF0, 0x2D80, 0, 0x4000, 0x2D00, 0, 0, 0xDC800 },
    { 32000, 1, 22, 5, { 0 }, 2, { 0 }, &gReverbSettings[0], 0x500, 0x200, 0x7FFF, 0xAF0, 0x2D80, 0, 0x4100, 0x2D00, 0, 0, 0xDC800 },
    { 32000, 1, 22, 5, { 0 }, 2, { 0 }, &gReverbSettings[24], 0x500, 0x200, 0x7FFF, 0xAF0, 0x2D80, 0, 0x4100, 0x2D00, 0, 0, 0xDC800 },
    { 32000, 1, 16, 5, { 0 }, 2, { 0 }, &gReverbSettings[0], 0x500, 0x200, 0x7FFF, 0xAF0, 0x2D80, 0, 0x4100, 0x2D00, 0, 0, 0xDC800 },
    { 22050, 1, 24, 5, { 0 }, 2, { 0 }, &gReverbSettings[0], 0x500, 0x200, 0x7FFF, 0xAF0, 0x2D80, 0, 0x4100, 0x2D00, 0, 0, 0xDC800 },
    { 32000, 1, 24, 5, { 0 }, 2, { 0 }, &gReverbSettings[6], 0x500, 0x200, 0x7FFF, 0xAF0, 0x2D80, 0, 0x3600, 0x2600, 0, 0, 0xDC800 },
};

// OoT func_800F9280
void Audio_StartBgm(u8 playerIdx, u8 seqId, u8 seqArgs, u16 fadeTimer) {
    u8 channelIdx;
    u16 duration;
    s32 pad;

    if (isBgmStartDisabled == 0 || playerIdx == AUDIO_PLAYER_SFX) {
        seqArgs &= 0x7F;
        if (seqArgs == 0x7F) {
            duration = (fadeTimer >> 3) * 60 * gAudioContext.audioBufferParameters.updatesPerFrame;
            Audio_QueueCmdS32(0x85000000 | _SHIFTL(playerIdx, 16, 8) | _SHIFTL(seqId, 8, 8), duration);
        } else {
            Audio_QueueCmdS32(0x82000000 | _SHIFTL(playerIdx, 16, 8) | _SHIFTL(seqId, 8, 8),
                              (fadeTimer * (u16)gAudioContext.audioBufferParameters.updatesPerFrame) / 4);
        }

        gActiveBgms[playerIdx].seqId = seqId | (seqArgs << 8);
        gActiveBgms[playerIdx].prevSeqId = seqId | (seqArgs << 8);
        gActiveBgms[playerIdx].isSeqPlayerInit = true;

        if (gActiveBgms[playerIdx].volCur != 1.0f) {
            Audio_QueueCmdF32(0x41000000 | _SHIFTL(playerIdx, 16, 8), gActiveBgms[playerIdx].volCur);
        }

        gActiveBgms[playerIdx].tempoDuration = 0;
        gActiveBgms[playerIdx].tempoDefault = 0;
        gActiveBgms[playerIdx].tempoCmd = 0;

        for (channelIdx = 0; channelIdx < ARRAY_COUNT(gActiveBgms[playerIdx].channelData); channelIdx++) {
            gActiveBgms[playerIdx].channelData[channelIdx].volCur = 1.0f;
            gActiveBgms[playerIdx].channelData[channelIdx].volDuration = 0;
            gActiveBgms[playerIdx].channelData[channelIdx].freqScaleCur = 1.0f;
            gActiveBgms[playerIdx].channelData[channelIdx].freqScaleDuration = 0;
        }

        gActiveBgms[playerIdx].freqScaleChannelFlags = 0;
        gActiveBgms[playerIdx].volChannelFlags = 0;
    }
}

// OoT func_800F9474
void Audio_StopBgm(u8 playerIdx, u16 fadeTimer) {
    Audio_QueueCmdS32(0x83000000 | ((u8)playerIdx << 16),
                      (fadeTimer * (u16)gAudioContext.audioBufferParameters.updatesPerFrame) / 4);
    gActiveBgms[playerIdx].seqId = NA_BGM_DISABLED;
}

void Audio_ProcessSeqCmd(u32 cmd) {
    u8 subOp;
    s32 importance;
    u16 fadeTimer;
    u16 channelMask;
    u16 val;
    u8 oldSpec;
    u8 spec;
    u8 op;
    u8 setupOp;
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
            if (!gActiveBgms[playerIdx].isWaitingForFonts && (isBgmStartDisabled == 0)) {
                if (seqArgs < 0x80) {
                    Audio_StartBgm(playerIdx, seqId, seqArgs, fadeTimer);
                } else {
                    // This case is taken for the new 0x8000 flag introduced in MM

                    // Store the cmd to be called again once the fonts are loaded
                    // but changes the command so that next time, the (seqArgs < 0x80) case is taken
                    gActiveBgms[playerIdx].startSeqCmd = (cmd & ~0x8008000) + 0x8000000;
                    gActiveBgms[playerIdx].isWaitingForFonts = true;
                    gActiveBgms[playerIdx].fontId = *AudioCmd_GetFontsForSequence(seqId, &outNumFonts);
                    Audio_StopBgm(playerIdx, 1);

                    if (gActiveBgms[playerIdx].prevSeqId != NA_BGM_DISABLED) {
                        if (*AudioCmd_GetFontsForSequence(seqId, &outNumFonts) !=
                            *AudioCmd_GetFontsForSequence(gActiveBgms[playerIdx].prevSeqId & 0xFF, &outNumFonts)) {
                            // Discard Seq Fonts
                            Audio_QueueCmdS32(((seqId & 0xFF) << 8) | 0xF6000000, 0);
                        }
                    }

                    Audio_QueueCmdS8((((*AudioCmd_GetFontsForSequence(seqId, &outNumFonts) & 0xFF)) << 0x10) | 0xF5000000 | 0x1400 |
                                         (((playerIdx + 1) & 0xFF & 0xFF & 0xFF)),
                                     0);
                }
            }
            break;

        case 0x1:
            // disable seq player
            fadeTimer = (cmd & 0xFF0000) >> 13;
            Audio_StopBgm(playerIdx, fadeTimer);
            break;

        case 0x2:
            // queue sequence
            seqId = cmd & 0xFF;
            seqArgs = (cmd & 0xFF00) >> 8;
            fadeTimer = (cmd & 0xFF0000) >> 13;
            importance = seqArgs;

            // Checks if the requested bgm is first in the list of requested
            // If it is already queued and first in the list, then play the bgm immediately
            for (i = 0; i < sNumBgmRequests[playerIdx]; i++) {
                if (sBgmRequests[playerIdx][i].seqId == seqId) {
                    if (i == 0) {
                        Audio_StartBgm(playerIdx, seqId, seqArgs, fadeTimer);
                    }
                    return;
                }
            }

            // Searches the bgm requests for the first request that does not have a higher priority
            // than the current incoming request
            found = sNumBgmRequests[playerIdx];
            for (i = 0; i < sNumBgmRequests[playerIdx]; i++) {
                if (sBgmRequests[playerIdx][i].importance <= importance) {
                    found = i;
                    i = sNumBgmRequests[playerIdx]; // "break;"
                }
            }

            if (sNumBgmRequests[playerIdx] < 5) {
                sNumBgmRequests[playerIdx]++;
            }

            for (i = sNumBgmRequests[playerIdx] - 1; i != found; i--) {
                sBgmRequests[playerIdx][i].importance = sBgmRequests[playerIdx][i - 1].importance;
                sBgmRequests[playerIdx][i].seqId = sBgmRequests[playerIdx][i - 1].seqId;
            }
            sBgmRequests[playerIdx][found].importance = seqArgs;
            sBgmRequests[playerIdx][found].seqId = seqId;

            if (found == 0) {
                Audio_StartBgm(playerIdx, seqId, seqArgs, fadeTimer);
            }
            break;

        case 0x3:
            // unqueue/stop sequence
            fadeTimer = (cmd & 0xFF0000) >> 13;

            found = sNumBgmRequests[playerIdx];
            for (i = 0; i < sNumBgmRequests[playerIdx]; i++) {
                seqId = cmd & 0xFF;
                if (sBgmRequests[playerIdx][i].seqId == seqId) {
                    found = i;
                    i = sNumBgmRequests[playerIdx]; // "break;"
                }
            }

            if (found != sNumBgmRequests[playerIdx]) {
                for (i = found; i < sNumBgmRequests[playerIdx] - 1; i++) {
                    sBgmRequests[playerIdx][i].importance = sBgmRequests[playerIdx][i + 1].importance;
                    sBgmRequests[playerIdx][i].seqId = sBgmRequests[playerIdx][i + 1].seqId;
                }
                sNumBgmRequests[playerIdx]--;
            }

            if (found == 0) {
                // Stop current sequence
                Audio_StopBgm(playerIdx, fadeTimer);
                if (sNumBgmRequests[playerIdx] != 0) {
                    // Play the next sequence in the queue
                    Audio_StartBgm(playerIdx, sBgmRequests[playerIdx][0].seqId, sBgmRequests[playerIdx][0].importance, fadeTimer);
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
            gActiveBgms[playerIdx].volTarget = (f32)val / 127.0f;
            if (gActiveBgms[playerIdx].volCur != gActiveBgms[playerIdx].volTarget) {
                gActiveBgms[playerIdx].volVelocity =
                    (gActiveBgms[playerIdx].volCur - gActiveBgms[playerIdx].volTarget) / (f32)duration;
                gActiveBgms[playerIdx].volDuration = duration;
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
            for (i = 0; i < ARRAY_COUNT(gActiveBgms[playerIdx].channelData); i++) {
                gActiveBgms[playerIdx].channelData[i].freqScaleTarget = freqScaleTarget;
                gActiveBgms[playerIdx].channelData[i].freqScaleDuration = duration;
                gActiveBgms[playerIdx].channelData[i].freqScaleVelocity =
                    (gActiveBgms[playerIdx].channelData[i].freqScaleCur - freqScaleTarget) / (f32)duration;
            }
            gActiveBgms[playerIdx].freqScaleChannelFlags = 0xFFFF;
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
            gActiveBgms[playerIdx].channelData[channelIdx].freqScaleTarget = freqScaleTarget;
            gActiveBgms[playerIdx].channelData[channelIdx].freqScaleVelocity =
                (gActiveBgms[playerIdx].channelData[channelIdx].freqScaleCur - freqScaleTarget) / (f32)duration;
            gActiveBgms[playerIdx].channelData[channelIdx].freqScaleDuration = duration;
            gActiveBgms[playerIdx].freqScaleChannelFlags |= 1 << channelIdx;
            break;

        case 0x6:
            // transition vol scale
            duration = (cmd & 0xFF0000) >> 15;
            channelIdx = (cmd & 0xF00) >> 8;
            val = cmd & 0xFF;
            if (duration == 0) {
                duration++;
            }
            gActiveBgms[playerIdx].channelData[channelIdx].volTarget = (f32)val / 127.0f;
            if (gActiveBgms[playerIdx].channelData[channelIdx].volCur != gActiveBgms[playerIdx].channelData[channelIdx].volTarget) {
                gActiveBgms[playerIdx].channelData[channelIdx].volVelocity =
                    (gActiveBgms[playerIdx].channelData[channelIdx].volCur - gActiveBgms[playerIdx].channelData[channelIdx].volTarget) /
                    (f32)duration;
                gActiveBgms[playerIdx].channelData[channelIdx].volDuration = duration;
                gActiveBgms[playerIdx].volChannelFlags |= 1 << channelIdx;
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
            if ((gActiveBgms[playerIdx].channelPortMask & (1 << channelIdx)) == 0) {
                Audio_QueueCmdS8(0x06000000 | _SHIFTL(playerIdx, 16, 8) | _SHIFTL(channelIdx, 8, 8) | _SHIFTL(port, 0, 8),
                                 val);
            }
            break;

        case 0x9:
            // set channel mask for command 0x8
            gActiveBgms[playerIdx].channelPortMask = cmd & 0xFFFF;
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
            gActiveBgms[playerIdx].tempoCmd = cmd;
            break;

        case 0xC:
            // start sequence with setup commands
            setupOp = (cmd & 0xF00000) >> 20;
            if (setupOp != 0xF) {
                if (gActiveBgms[playerIdx].numSetupCmd < 7) {
                    found = gActiveBgms[playerIdx].numSetupCmd++;
                    if (found < 8) {
                        gActiveBgms[playerIdx].setupCmd[found] = cmd;
                        gActiveBgms[playerIdx].setupCmdTimer = 2;
                    }
                }
            } else {
                gActiveBgms[playerIdx].numSetupCmd = 0;
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
                    // set sequence disabled true in (isBgmStartDisabled & 1) bit
                    isBgmStartDisabled = (isBgmStartDisabled & 0xFE) | (u8)(val & 1);
                    break;
                case 2:
                    // set sequence disabled true in (isBgmStartDisabled & 2) bit
                    isBgmStartDisabled = (isBgmStartDisabled & 0xFD) | (u8)((val & 1) * 2);
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
    sAudioSeqCmds[sSeqCmdWritePos++] = cmd;
}

void Audio_ProcessSeqCmds(void) {
    while (sSeqCmdWritePos != sSeqCmdReadPos) {
        Audio_ProcessSeqCmd(sAudioSeqCmds[sSeqCmdReadPos++]);
    }
}

// OoT func_800FA0B4
u16 Audio_GetActiveBgm(u8 playerIdx) {
    if (gActiveBgms[playerIdx].isWaitingForFonts == true) {
        return gActiveBgms[playerIdx].startSeqCmd & 0xFF;
    }

    if (gActiveBgms[playerIdx].seqId != NA_BGM_DISABLED) {
        return gActiveBgms[playerIdx].seqId;
    }

    return NA_BGM_DISABLED;
}

// OoT func_800FA11C
s32 Audio_IsSeqCmdNotQueued(u32 cmdVal, u32 cmdMask) {
    u8 i;

    for (i = sSeqCmdReadPos; i != sSeqCmdWritePos; i++) {
        if ((sAudioSeqCmds[i] & cmdMask) == cmdVal) {
            return false;
        }
    }

    return true;
}

// OoT func_800FA174
// Unused
void Audio_ResetBgmRequests(u8 playerIdx) {
    sNumBgmRequests[playerIdx] = 0;
}

// OoT func_800FA18C
// Unused
void Audio_DisableSeqCmdSetupOp(u8 playerIdx, u8 setupOpDisabled) {
    u8 i;

    for (i = 0; i < gActiveBgms[playerIdx].numSetupCmd; i++) {
        u8 setupOp = (gActiveBgms[playerIdx].setupCmd[i] & 0xF00000) >> 20;

        if (setupOp == setupOpDisabled) {
            gActiveBgms[playerIdx].setupCmd[i] = 0xFF000000;
        }
    }
}

void Audio_SetVolScale(u8 playerIdx, u8 scaleIdx, u8 targetVol, u8 volFadeTimer) {
    f32 volScale;
    u8 i;

    gActiveBgms[playerIdx].volScales[scaleIdx] = targetVol & 0x7F;

    if (volFadeTimer != 0) {
        gActiveBgms[playerIdx].fadeVolUpdate = 1;
        gActiveBgms[playerIdx].volFadeTimer = volFadeTimer;
    } else {
        for (i = 0, volScale = 1.0f; i < 4; i++) {
            volScale *= gActiveBgms[playerIdx].volScales[i] / 127.0f;
        }

        Audio_SetVolScaleNow(playerIdx, volFadeTimer, volScale);
    }
}

/**
 * Apply various high-level commands to the active bgms
 */
void Audio_UpdateActiveBgms(void) {
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

    for (playerIdx = 0; playerIdx < ARRAY_COUNT(gActiveBgms); playerIdx++) {

        // The seqPlayer has finished initializing and is currently playing the active bgm
        if (gActiveBgms[playerIdx].isSeqPlayerInit && gAudioContext.seqPlayers[playerIdx].enabled) {
            gActiveBgms[playerIdx].isSeqPlayerInit = false;
        }

        // The seqPlayer is no longer playing the active bgm
        if ((Audio_GetActiveBgm(playerIdx) != NA_BGM_DISABLED) && !gAudioContext.seqPlayers[playerIdx].enabled &&
            (!gActiveBgms[playerIdx].isSeqPlayerInit)) {
            gActiveBgms[playerIdx].seqId = NA_BGM_DISABLED;
        }

        // Check if the requested bgm is waiting for fonts to load
        if (gActiveBgms[playerIdx].isWaitingForFonts) {
            switch ((s32)func_80193BA0(&dummy)) {
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                    // The fonts have been loaded successfully.
                    gActiveBgms[playerIdx].isWaitingForFonts = false;
                    // Queue the same command that was stored previously, but without the 0x8000 
                    Audio_ProcessSeqCmd(gActiveBgms[playerIdx].startSeqCmd);
                    break;
                case 0xFF:
                    // There was an error in loading the fonts
                    gActiveBgms[playerIdx].isWaitingForFonts = false;
                    break;
            }
        }

        if (gActiveBgms[playerIdx].fadeVolUpdate) {
            volume = 1.0f;
            for (j = 0; j < 4; j++) {
                volume *= (gActiveBgms[playerIdx].volScales[j] / 127.0f);
            }

            Audio_SeqCmd4((u8)(playerIdx + 8), gActiveBgms[playerIdx].volFadeTimer, (u8)(volume * 127.0f));
            gActiveBgms[playerIdx].fadeVolUpdate = 0;
        }

        if (gActiveBgms[playerIdx].volDuration != 0) {
            gActiveBgms[playerIdx].volDuration--;

            if (gActiveBgms[playerIdx].volDuration != 0) {
                gActiveBgms[playerIdx].volCur -= gActiveBgms[playerIdx].volVelocity;
            } else {
                gActiveBgms[playerIdx].volCur = gActiveBgms[playerIdx].volTarget;
            }

            Audio_QueueCmdF32(0x41000000 | _SHIFTL(playerIdx, 16, 8), gActiveBgms[playerIdx].volCur);
        }

        if (gActiveBgms[playerIdx].tempoCmd != 0) {
            tempoCmd = gActiveBgms[playerIdx].tempoCmd;
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
                        if (gActiveBgms[playerIdx].tempoDefault) {
                            tempoTarget = gActiveBgms[playerIdx].tempoDefault;
                        } else {
                            tempoTarget = tempoDefault;
                        }
                        break;
                }

                if (gActiveBgms[playerIdx].tempoDefault == 0) {
                    gActiveBgms[playerIdx].tempoDefault = tempoDefault;
                }

                gActiveBgms[playerIdx].tempoTarget = tempoTarget;
                gActiveBgms[playerIdx].tempoCur = gAudioContext.seqPlayers[playerIdx].tempo / 0x30;
                gActiveBgms[playerIdx].tempoVelocity = (gActiveBgms[playerIdx].tempoCur - gActiveBgms[playerIdx].tempoTarget) / tempoDuration;
                gActiveBgms[playerIdx].tempoDuration = tempoDuration;
                gActiveBgms[playerIdx].tempoCmd = 0;
            }
        }

        if (gActiveBgms[playerIdx].tempoDuration != 0) {
            gActiveBgms[playerIdx].tempoDuration--;
            if (gActiveBgms[playerIdx].tempoDuration != 0) {
                gActiveBgms[playerIdx].tempoCur -= gActiveBgms[playerIdx].tempoVelocity;
            } else {
                gActiveBgms[playerIdx].tempoCur = gActiveBgms[playerIdx].tempoTarget;
            }
            // set tempo
            Audio_QueueCmdS32(0x47000000 | _SHIFTL(playerIdx, 16, 8), gActiveBgms[playerIdx].tempoCur);
        }

        if (gActiveBgms[playerIdx].volChannelFlags != 0) {
            for (channelIdx = 0; channelIdx < 16; channelIdx++) {
                if (gActiveBgms[playerIdx].channelData[channelIdx].volDuration != 0) {
                    gActiveBgms[playerIdx].channelData[channelIdx].volDuration--;
                    if (gActiveBgms[playerIdx].channelData[channelIdx].volDuration != 0) {
                        gActiveBgms[playerIdx].channelData[channelIdx].volCur -= gActiveBgms[playerIdx].channelData[channelIdx].volVelocity;
                    } else {
                        gActiveBgms[playerIdx].channelData[channelIdx].volCur = gActiveBgms[playerIdx].channelData[channelIdx].volTarget;
                        gActiveBgms[playerIdx].volChannelFlags ^= (1 << channelIdx);
                    }
                    // CHAN_UPD_VOL_SCALE (playerIdx = seq, channelIdx = chan)
                    Audio_QueueCmdF32(0x01000000 | _SHIFTL(playerIdx, 16, 8) | _SHIFTL(channelIdx, 8, 8),
                                      gActiveBgms[playerIdx].channelData[channelIdx].volCur);
                }
            }
        }

        if (gActiveBgms[playerIdx].freqScaleChannelFlags != 0) {
            for (channelIdx = 0; channelIdx < 16; channelIdx++) {
                if (gActiveBgms[playerIdx].channelData[channelIdx].freqScaleDuration != 0) {
                    gActiveBgms[playerIdx].channelData[channelIdx].freqScaleDuration--;
                    if (gActiveBgms[playerIdx].channelData[channelIdx].freqScaleDuration != 0) {
                        gActiveBgms[playerIdx].channelData[channelIdx].freqScaleCur -= gActiveBgms[playerIdx].channelData[channelIdx].freqScaleVelocity;
                    } else {
                        gActiveBgms[playerIdx].channelData[channelIdx].freqScaleCur = gActiveBgms[playerIdx].channelData[channelIdx].freqScaleTarget;
                        gActiveBgms[playerIdx].freqScaleChannelFlags ^= (1 << channelIdx);
                    }
                    // CHAN_UPD_FREQ_SCALE
                    Audio_QueueCmdF32(0x04000000 | _SHIFTL(playerIdx, 16, 8) | _SHIFTL(channelIdx, 8, 8),
                                      gActiveBgms[playerIdx].channelData[channelIdx].freqScaleCur);
                }
            }
        }

        // Process setup commands
        if (gActiveBgms[playerIdx].numSetupCmd != 0) {
            // If there is any "Audio_SeqCmdF" queued, drop all setup commands
            if (!Audio_IsSeqCmdNotQueued(0xF0000000, 0xF0000000)) {
                gActiveBgms[playerIdx].numSetupCmd = 0;
                return;
            }

            // Only process setup commands once the timer reaches zero
            if (gActiveBgms[playerIdx].setupCmdTimer != 0) {
                gActiveBgms[playerIdx].setupCmdTimer--;
                continue;
            }

            // Do not process setup commands if the seqPlayer is already enabled
            if (gAudioContext.seqPlayers[playerIdx].enabled) {
                continue;
            }

            for (j = 0; j < gActiveBgms[playerIdx].numSetupCmd; j++) {
                setupOp = (gActiveBgms[playerIdx].setupCmd[j] & 0x00F00000) >> 20;
                setupPlayerIdx = (gActiveBgms[playerIdx].setupCmd[j] & 0x000F0000) >> 16;
                setupVal2 = (gActiveBgms[playerIdx].setupCmd[j] & 0xFF00) >> 8;
                setupVal1 = gActiveBgms[playerIdx].setupCmd[j] & 0xFF;

                switch (setupOp) {
                    case 0x0:
                        Audio_SetVolScale(setupPlayerIdx, 1, 0x7F, setupVal1);
                        break;
                    case 0x7:
                        if (sNumBgmRequests[playerIdx] == setupVal1) {
                            Audio_SetVolScale(setupPlayerIdx, 1, 0x7F, setupVal2);
                        }
                        break;
                    case 0x1:
                        Audio_SeqCmd3((u8)(playerIdx + 8), gActiveBgms[playerIdx].seqId);
                        break;
                    case 0x2:
                        Audio_StartSeq((u8)(setupPlayerIdx + 8), 1, gActiveBgms[setupPlayerIdx].seqId);
                        gActiveBgms[setupPlayerIdx].fadeVolUpdate = 1;
                        gActiveBgms[setupPlayerIdx].volScales[1] = 0x7F;
                        break;
                    case 0x3:
                        Audio_SeqCmdB30((u8)(setupPlayerIdx + 8), setupVal2, setupVal1);
                        break;
                    case 0x4:
                        Audio_SeqCmdB40((u8)(setupPlayerIdx + 8), setupVal1, 0);
                        break;
                    case 0x5:
                        seqId = gActiveBgms[playerIdx].setupCmd[j] & 0xFFFF;
                        Audio_StartSeq((u8)(setupPlayerIdx + 8), gActiveBgms[setupPlayerIdx].setupFadeTimer, seqId);
                        Audio_SetVolScale(setupPlayerIdx, 1, 0x7F, 0);
                        gActiveBgms[setupPlayerIdx].setupFadeTimer = 0;
                        break;
                    case 0x6:
                        gActiveBgms[playerIdx].setupFadeTimer = setupVal2;
                        break;
                    case 0x8:
                        Audio_SetVolScale(setupPlayerIdx, setupVal2, 0x7F, setupVal1);
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
                        channelMask = gActiveBgms[playerIdx].setupCmd[j] & 0xFFFF;
                        Audio_SeqCmdA((u8)(setupPlayerIdx + 8), channelMask);
                        break;
                    case 0xA:
                        Audio_SeqCmd5((u8)(setupPlayerIdx + 8), setupVal2, (setupVal1 * 10) & 0xFFFF);
                        break;
                }
            }

            gActiveBgms[playerIdx].numSetupCmd = 0;
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
                if (temp_v0 & 1) {
                    Audio_QueueCmdS32(((phi_s1_2 & 0xFF) << 8) | 0xE6040000, D_80200BCE);
                    Audio_ScheduleProcessCmds();
                }
                phi_s1_2++;
                temp_v0 = temp_v0 >> 1;
            }

            D_80200BCE -= D_80200BD0;
            ret = true;
        } else {
            for (; temp_v0 != 0;) {
                new_var = phi_s1_2;
                if (temp_v0 & 1) {
                    Audio_QueueCmdS32(((phi_s1_2 & 0xFF) << 8) | 0xE6000000,
                                      (s32)(D_801DB930[D_801DB4DC & 0xFF & 0xFF] + new_var));
                    Audio_ScheduleProcessCmds();
                }
                phi_s1_2++;
                temp_v0 = temp_v0 >> 1;
            }

            D_801DB4DC = 0;
            Audio_QueueCmdS8(0x46020000, gSfxChannelLayout);
            func_801A4D50();
        }
    }

    return ret;
}

// OoT func_800FADF8
void Audio_ResetBgms(void) {
    u8 playerIdx;
    u8 j;

    for (playerIdx = AUDIO_PLAYER_0; playerIdx < AUDIO_PLAYER_MAX; playerIdx++) {
        sNumBgmRequests[playerIdx] = 0;

        gActiveBgms[playerIdx].seqId = NA_BGM_DISABLED;
        gActiveBgms[playerIdx].prevSeqId = NA_BGM_DISABLED;
        gActiveBgms[playerIdx].tempoDuration = 0;
        gActiveBgms[playerIdx].tempoDefault = 0;
        gActiveBgms[playerIdx].tempoCmd = 0;
        gActiveBgms[playerIdx].channelPortMask = 0;
        gActiveBgms[playerIdx].numSetupCmd = 0;
        gActiveBgms[playerIdx].setupFadeTimer = 0;
        gActiveBgms[playerIdx].freqScaleChannelFlags = 0;
        gActiveBgms[playerIdx].volChannelFlags = 0;
        gActiveBgms[playerIdx].isWaitingForFonts = false;
        gActiveBgms[playerIdx].isSeqPlayerInit = false;

        for (j = 0; j < ARRAY_COUNT(gActiveBgms[playerIdx].volScales); j++) {
            gActiveBgms[playerIdx].volScales[j] = 0x7F;
        }

        gActiveBgms[playerIdx].volFadeTimer = 1;
        gActiveBgms[playerIdx].fadeVolUpdate = 1;
    }
}

// OoT func_800FAEB4
void Audio_ResetBgmVolume(void) {
    u8 playerIdx;
    u8 j;

    for (playerIdx = 0; playerIdx < AUDIO_PLAYER_MAX; playerIdx++) {
        gActiveBgms[playerIdx].volCur = 1.0f;
        gActiveBgms[playerIdx].volDuration = 0;
        gActiveBgms[playerIdx].fadeVolUpdate = 0;
        for (j = 0; j < ARRAY_COUNT(gActiveBgms[playerIdx].volScales); j++) {
            gActiveBgms[playerIdx].volScales[j] = 0x7F;
        }
    }
    Audio_ResetBgms();
}
