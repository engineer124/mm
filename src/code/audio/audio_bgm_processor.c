#include "global.h"

void func_801A4DA4(void);

u8 sSeqCmdWrPos = 0;
u8 sSeqCmdRdPos = 0;
u8 D_801DB4C8 = 0;
u8 D_801DB4CC[] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x00, 0x00, 0x00,
};
u8 gAudioSpecId = 0;
u8 D_801DB4D8 = 0;
u32 D_801DB4DC = 0;

// /* 0x00 */ ReverbSettings gReverbSettings = { 1, 0x30, 0x3000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 };
// /* 0x01 */ ReverbSettings D_801DB4F8 = { 1, 0x20, 0x0800, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x0000, 0x0, 0x0 };
// /* 0x02 */ ReverbSettings D_801DB510 = { 0 };
// /* 0x03 */ ReverbSettings D_801DB528 = { 1, 0x30, 0x3000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 };
// /* 0x04 */ ReverbSettings D_801DB540 = { 1, 0x30, 0x1800, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x0000, 0xB, 0xB };
// /* 0x05 */ ReverbSettings D_801DB55C = { 0 };
// /* 0x06 */ ReverbSettings D_801DB570 = { 1, 0x30, 0x3000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 };
// /* 0x07 */ ReverbSettings D_801DB588 = { 1, 0x38, 0x2800, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x0000, 0x7, 0x7 };
// /* 0x08 */ ReverbSettings D_801DB5A0 = { 0 };
// /* 0x09 */ ReverbSettings D_801DB5B8 = { 1, 0x30, 0x3000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 };
// /* 0x0A */ ReverbSettings D_801DB5D0 = { 1, 0x30, 0x6800, 0, 0, 0x7FFF, 0x1400, 0x1400, 0xFF, 0x3000, 0x6, 0x6 };
// /* 0x0B */ ReverbSettings D_801DB5E8 = { 2, 0x50, 0x6000, 0, 0, 0x7FFF, 0xD000, 0x3000, 0xFF, 0x3000, 0x0, 0x0 };
// /* 0x0C */ ReverbSettings D_801DB600 = { 1, 0x30, 0x3000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 };
// /* 0x0D */ ReverbSettings D_801DB618 = { 1, 0x40, 0x5000, 0, 0, 0x7FFF, 0x1800, 0x1800, 0xFF, 0x3000, 0x7, 0x7 };
// /* 0x0E */ ReverbSettings D_801DB630 = { 0 };
// /* 0x0F */ ReverbSettings D_801DB648 = { 1, 0x30, 0x3000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 };
// /* 0x10 */ ReverbSettings D_801DB660 = { 1, 0x40, 0x5C00, 0, 0, 0x7FFF, 0x2000, 0x2000, 0xFF, 0x3000, 0x4, 0x4 };
// /* 0x11 */ ReverbSettings D_801DB678 = { 0 };
// /* 0x12 */ ReverbSettings D_801DB690 = { 1, 0x30, 0x3000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 };
// /* 0x13 */ ReverbSettings D_801DB6A8 = { 1, 0x30, 0x6000, 0, 0, 0x7FFF, 0x1000, 0x1000, 0xFF, 0x3000, 0xA, 0xA };
// /* 0x14 */ ReverbSettings D_801DB6C0 = { 0 };
// /* 0x15 */ ReverbSettings D_801DB6D8 = { 1, 0x30, 0x3000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 };
// /* 0x16 */ ReverbSettings D_801DB6F0 = { 1, 0x30, 0x6800, 0, 0, 0x7FFF, 0x1400, 0x1400, 0xFF, 0x3000, 0x6, 0x6 };
// /* 0x17 */ ReverbSettings D_801DB708 = { 0 };
// /* 0x18 */ ReverbSettings D_801DB720 = { 1, 0x30, 0x3000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 };
// /* 0x19 */ ReverbSettings D_801DB738 = { 1, 0x50, 0x5000, 0, 0, 0x7FFF, 0xD000, 0x3000, 0xFF, 0x3000, 0x0, 0x0 };
// /* 0x1A */ ReverbSettings D_801DB750 = { 1, 0x30, 0x3000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 };
// /* 0x1B */ ReverbSettings D_801DB768 = { 1, 0x20, 0x0000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x0000, 0x0, 0x0 };
// /* 0x1C */ ReverbSettings D_801DB780 = { 0 };
// /* 0x1D */ ReverbSettings D_801DB798 = { 1, 0x30, 0x3000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 };
// /* 0x1E */ ReverbSettings D_801DB7B0 = { 1, 0x30, 0x1800, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x0000, 0xB, 0xB };
// /* 0x1F */ ReverbSettings D_801DB7C8 = { 0 };
// /* 0x20 */ ReverbSettings D_801DB7E0 = { 1, 0x30, 0x3000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 };
// /* 0x21 */ ReverbSettings D_801DB7F8 = { 0 };
// /* 0x22 */ ReverbSettings D_801DB810 = { 0 };
// /* 0x23 */ ReverbSettings D_801DB828 = { 1, 0x30, 0x3000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 };
// /* 0x24 */ ReverbSettings D_801DB840 = { 1, 0x40, 0x5000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 };
// /* 0x25 */ ReverbSettings D_801DB858 = { 0 };
// /* 0x26 */ ReverbSettings D_801DB870 = { 1, 0x30, 0x3000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 };
// /* 0x27 */ ReverbSettings D_801DB888 = { 1, 0x30, 0x6800, 0, 0, 0x7FFF, 0x1400, 0x1400, 0xFF, 0x3000, 0x6, 0x6 };
// /* 0x28 */ ReverbSettings D_801DB8A0 = { 2, 0x50, 0x6000, 0, 0, 0x7FFF, 0xD000, 0x3000, 0xFF, 0x3000, 0x0, 0x0 };
// /* 0x29 */ ReverbSettings D_801DB8B8 = { 1, 0x30, 0x3000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 };
// /* 0x2A */ ReverbSettings D_801DB8D0 = { 1, 0x30, 0x1800, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x0000, 0xB, 0xB };
// /* 0x2B */ ReverbSettings D_801DB8E8 = { 1, 0x40, 0x5000, 0, 0, 0x7FFF, 0x1800, 0x1800, 0xFF, 0x3000, 0x7, 0x7 };
// /* 0x2C */ ReverbSettings D_801DB900 = { 1, 0x30, 0x3000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 };
// /* 0x2D */ ReverbSettings D_801DB918 = { 1, 0x50, 0x1800, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x0000, 0xB, 0xB };

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
void func_801A7B10(u8 playerIdx, u8 seqId, u8 seqArgs, u16 fadeTimer) {
    u8 i;
    u16 dur;
    s32 pad;

    if (D_801DB4C8 == 0 || playerIdx == AUDIO_PLAYER_SFX) {
        seqArgs &= 0x7F;
        if (seqArgs == 0x7F) {
            dur = (fadeTimer >> 3) * 60 * gAudioContext.audioBufferParameters.updatesPerFrame;
            Audio_QueueCmdS32(0x85000000 | _SHIFTL(playerIdx, 16, 8) | _SHIFTL(seqId, 8, 8), dur);
        } else {
            Audio_QueueCmdS32(0x82000000 | _SHIFTL(playerIdx, 16, 8) | _SHIFTL(seqId, 8, 8),
                              (fadeTimer * (u16)gAudioContext.audioBufferParameters.updatesPerFrame) / 4);
        }

        gActiveBgms[playerIdx].seqId1 = seqId | (seqArgs << 8);
        gActiveBgms[playerIdx].seqId2 = seqId | (seqArgs << 8);
        gActiveBgms[playerIdx].unk_21B = 1;

        if (gActiveBgms[playerIdx].volCur != 1.0f) {
            Audio_QueueCmdF32(0x41000000 | _SHIFTL(playerIdx, 16, 8), gActiveBgms[playerIdx].volCur);
        }

        gActiveBgms[playerIdx].unk_28 = 0;
        gActiveBgms[playerIdx].unk_18 = 0;
        gActiveBgms[playerIdx].unk_14 = 0;

        for (i = 0; i < ARRAY_COUNT(gActiveBgms[playerIdx].channelData); i++) {
            gActiveBgms[playerIdx].channelData[i].unk_00 = 1.0f;
            gActiveBgms[playerIdx].channelData[i].unk_0C = 0;
            gActiveBgms[playerIdx].channelData[i].unk_10 = 1.0f;
            gActiveBgms[playerIdx].channelData[i].unk_1C = 0;
        }

        gActiveBgms[playerIdx].unk_250 = 0;
        gActiveBgms[playerIdx].unk_252 = 0;
    }
}

// OoT func_800F9474
void func_801A7D04(u8 playerIdx, u16 arg1) {
    Audio_QueueCmdS32(0x83000000 | ((u8)playerIdx << 16),
                      (arg1 * (u16)gAudioContext.audioBufferParameters.updatesPerFrame) / 4);
    gActiveBgms[playerIdx].seqId1 = NA_BGM_DISABLED;
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
    u8 channelIdx;
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
            if (!gActiveBgms[playerIdx].unk_260 && (D_801DB4C8 == 0)) {
                if (seqArgs < 0x80) {
                    func_801A7B10(playerIdx, seqId, seqArgs, fadeTimer);
                } else {
                    gActiveBgms[playerIdx].unk_25C = (cmd & 0xF7FF7FFF) + 0x8000000;
                    gActiveBgms[playerIdx].unk_260 = true;
                    gActiveBgms[playerIdx].unk_261[0] = *func_80193C04(seqId, &sp4C);
                    func_801A7D04(playerIdx, 1);
                    if (gActiveBgms[playerIdx].seqId2 != NA_BGM_DISABLED) {
                        if (*func_80193C04(seqId, &sp4C) !=
                            *func_80193C04(gActiveBgms[playerIdx].seqId2 & 0xFF, &sp4C)) {
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
            gActiveBgms[playerIdx].volTarget = (f32)val / 127.0f;
            if (gActiveBgms[playerIdx].volCur != gActiveBgms[playerIdx].volTarget) {
                gActiveBgms[playerIdx].unk_08 =
                    (gActiveBgms[playerIdx].volCur - gActiveBgms[playerIdx].volTarget) / (f32)duration;
                gActiveBgms[playerIdx].unk_0C = duration;
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
            for (i = 0; i < ARRAY_COUNT(gActiveBgms[playerIdx].channelData); i++) {
                gActiveBgms[playerIdx].channelData[i].unk_14 = freqScale;
                gActiveBgms[playerIdx].channelData[i].unk_1C = duration;
                gActiveBgms[playerIdx].channelData[i].unk_18 =
                    (gActiveBgms[playerIdx].channelData[i].unk_10 - freqScale) / (f32)duration;
            }
            gActiveBgms[playerIdx].unk_250 = 0xFFFF;
            break;

        case 0xD:
            // transition freq scale
            duration = (cmd & 0xFF0000) >> 15;
            channelIdx = (cmd & 0xF000) >> 12;
            val = cmd & 0xFFF;
            if (duration == 0) {
                duration++;
            }
            freqScale = (f32)val / 1000.0f;
            gActiveBgms[playerIdx].channelData[channelIdx].unk_14 = freqScale;
            gActiveBgms[playerIdx].channelData[channelIdx].unk_18 =
                (gActiveBgms[playerIdx].channelData[channelIdx].unk_10 - freqScale) / (f32)duration;
            gActiveBgms[playerIdx].channelData[channelIdx].unk_1C = duration;
            gActiveBgms[playerIdx].unk_250 |= 1 << channelIdx;
            break;

        case 0x6:
            // transition vol scale
            duration = (cmd & 0xFF0000) >> 15;
            channelIdx = (cmd & 0xF00) >> 8;
            val = cmd & 0xFF;
            if (duration == 0) {
                duration++;
            }
            gActiveBgms[playerIdx].channelData[channelIdx].unk_04 = (f32)val / 127.0f;
            if (gActiveBgms[playerIdx].channelData[channelIdx].unk_00 != gActiveBgms[playerIdx].channelData[channelIdx].unk_04) {
                gActiveBgms[playerIdx].channelData[channelIdx].unk_08 =
                    (gActiveBgms[playerIdx].channelData[channelIdx].unk_00 - gActiveBgms[playerIdx].channelData[channelIdx].unk_04) /
                    (f32)duration;
                gActiveBgms[playerIdx].channelData[channelIdx].unk_0C = duration;
                gActiveBgms[playerIdx].unk_252 |= 1 << channelIdx;
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
            if ((gActiveBgms[playerIdx].unk_258 & (1 << channelIdx)) == 0) {
                Audio_QueueCmdS8(0x06000000 | _SHIFTL(playerIdx, 16, 8) | _SHIFTL(channelIdx, 8, 8) | _SHIFTL(port, 0, 8),
                                 val);
            }
            break;

        case 0x9:
            // set channel mask for command 0x8
            gActiveBgms[playerIdx].unk_258 = cmd & 0xFFFF;
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
            gActiveBgms[playerIdx].unk_14 = cmd;
            break;

        case 0xC:
            // start sequence with setup commands
            subOp = (cmd & 0xF00000) >> 20;
            if (subOp != 0xF) {
                if (gActiveBgms[playerIdx].unk_4D < 7) {
                    found = gActiveBgms[playerIdx].unk_4D++;
                    if (found < 8) {
                        gActiveBgms[playerIdx].unk_2C[found] = cmd;
                        gActiveBgms[playerIdx].unk_4C = 2;
                    }
                }
            } else {
                gActiveBgms[playerIdx].unk_4D = 0;
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
u16 Audio_GetActiveBgm(u8 playerIdx) {
    if (gActiveBgms[playerIdx].unk_260 == true) {
        return gActiveBgms[playerIdx].unk_25C & 0xFF;
    }

    if (gActiveBgms[playerIdx].seqId1 != NA_BGM_DISABLED) {
        return gActiveBgms[playerIdx].seqId1;
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
void func_801A8B14(u8 playerIdx) {
    D_801FFD34[playerIdx] = 0;
}

// OoT func_800FA18C
void func_801A8B2C(u8 playerIdx, u8 arg1) {
    u8 i;

    for (i = 0; i < gActiveBgms[playerIdx].unk_4D; i++) {
        u8 unkb = (gActiveBgms[playerIdx].unk_2C[i] & 0xF00000) >> 20;

        if (unkb == arg1) {
            gActiveBgms[playerIdx].unk_2C[i] = 0xFF000000;
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

void Audio_ProcessActiveBgms(void) {
    u32 temp_a1;
    u16 temp_lo;
    u16 temp_v1;
    u16 phi_a2;
    u8 temp_v0_4;
    u8 temp_a0;
    u8 playerIdx;
    u8 temp_s0_3;
    u8 temp_a3_3;
    s32 pad[2];
    u16 new_var;
    u32 dummy;
    f32 phi_f0;
    u8 phi_t0;
    u8 i;
    u8 j;
    u8 k;

    for (i = 0; i < ARRAY_COUNT(gActiveBgms); i++) {

        if ((gActiveBgms[i].unk_21B != 0) && gAudioContext.seqPlayers[i].enabled) {
            gActiveBgms[i].unk_21B = 0;
        }

        if ((Audio_GetActiveBgm(i) != NA_BGM_DISABLED) && !gAudioContext.seqPlayers[i].enabled &&
            (gActiveBgms[i].unk_21B == 0)) {
            gActiveBgms[i].seqId1 = NA_BGM_DISABLED;
        }

        if (gActiveBgms[i].unk_260) {
            switch ((s32)func_80193BA0(&dummy)) {
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                    gActiveBgms[i].unk_260 = false;
                    Audio_ProcessSeqCmd(gActiveBgms[i].unk_25C);
                    break;
                case 0xFF:
                    gActiveBgms[i].unk_260 = false;
                    break;
            }
        }

        if (gActiveBgms[i].fadeVolUpdate) {
            phi_f0 = 1.0f;
            for (j = 0; j < 4; j++) {
                phi_f0 *= (gActiveBgms[i].volScales[j] / 127.0f);
            }

            Audio_SeqCmd4((u8)(i + 8), gActiveBgms[i].volFadeTimer, (u8)(phi_f0 * 127.0f));
            gActiveBgms[i].fadeVolUpdate = 0;
        }

        if (gActiveBgms[i].unk_0C != 0) {
            gActiveBgms[i].unk_0C--;

            if (gActiveBgms[i].unk_0C != 0) {
                gActiveBgms[i].volCur = gActiveBgms[i].volCur - gActiveBgms[i].unk_08;
            } else {
                gActiveBgms[i].volCur = gActiveBgms[i].volTarget;
            }

            Audio_QueueCmdF32(0x41000000 | _SHIFTL(i, 16, 8), gActiveBgms[i].volCur);
        }

        if (gActiveBgms[i].unk_14 != 0) {
            temp_a1 = gActiveBgms[i].unk_14;
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
                        if (gActiveBgms[i].unk_18) {
                            phi_a2 = gActiveBgms[i].unk_18;
                        } else {
                            phi_a2 = temp_lo;
                        }
                        break;
                }

                if (gActiveBgms[i].unk_18 == 0) {
                    gActiveBgms[i].unk_18 = temp_lo;
                }

                gActiveBgms[i].unk_20 = phi_a2;
                gActiveBgms[i].unk_1C = gAudioContext.seqPlayers[i].tempo / 0x30;
                gActiveBgms[i].unk_24 = (gActiveBgms[i].unk_1C - gActiveBgms[i].unk_20) / phi_t0;
                gActiveBgms[i].unk_28 = phi_t0;
                gActiveBgms[i].unk_14 = 0;
            }
        }

        if (gActiveBgms[i].unk_28 != 0) {
            gActiveBgms[i].unk_28--;
            if (gActiveBgms[i].unk_28 != 0) {
                gActiveBgms[i].unk_1C = gActiveBgms[i].unk_1C - gActiveBgms[i].unk_24;
            } else {
                gActiveBgms[i].unk_1C = gActiveBgms[i].unk_20;
            }
            // set tempo
            Audio_QueueCmdS32(0x47000000 | _SHIFTL(i, 16, 8), gActiveBgms[i].unk_1C);
        }

        if (gActiveBgms[i].unk_252 != 0) {
            for (k = 0; k < 0x10; k++) {
                if (gActiveBgms[i].channelData[k].unk_0C != 0) {
                    gActiveBgms[i].channelData[k].unk_0C--;
                    if (gActiveBgms[i].channelData[k].unk_0C != 0) {
                        gActiveBgms[i].channelData[k].unk_00 -= gActiveBgms[i].channelData[k].unk_08;
                    } else {
                        gActiveBgms[i].channelData[k].unk_00 = gActiveBgms[i].channelData[k].unk_04;
                        gActiveBgms[i].unk_252 ^= (1 << k);
                    }
                    // CHAN_UPD_VOL_SCALE (i = seq, k = chan)
                    Audio_QueueCmdF32(0x01000000 | _SHIFTL(i, 16, 8) | _SHIFTL(k, 8, 8),
                                      gActiveBgms[i].channelData[k].unk_00);
                }
            }
        }

        if (gActiveBgms[i].unk_250 != 0) {
            for (k = 0; k < 0x10; k++) {
                if (gActiveBgms[i].channelData[k].unk_1C != 0) {
                    gActiveBgms[i].channelData[k].unk_1C--;
                    if (gActiveBgms[i].channelData[k].unk_1C != 0) {
                        gActiveBgms[i].channelData[k].unk_10 -= gActiveBgms[i].channelData[k].unk_18;
                    } else {
                        gActiveBgms[i].channelData[k].unk_10 = gActiveBgms[i].channelData[k].unk_14;
                        gActiveBgms[i].unk_250 ^= (1 << k);
                    }
                    // CHAN_UPD_FREQ_SCALE
                    Audio_QueueCmdF32(0x04000000 | _SHIFTL(i, 16, 8) | _SHIFTL(k, 8, 8),
                                      gActiveBgms[i].channelData[k].unk_10);
                }
            }
        }

        if (gActiveBgms[i].unk_4D != 0) {
            if (!func_801A8ABC(0xF0000000, 0xF0000000)) {
                gActiveBgms[i].unk_4D = 0;
                return;
            }

            if (gActiveBgms[i].unk_4C != 0) {
                gActiveBgms[i].unk_4C--;
                continue;
            }

            if (gAudioContext.seqPlayers[i].enabled) {
                continue;
            }

            for (j = 0; j < gActiveBgms[i].unk_4D; j++) {
                temp_a0 = (gActiveBgms[i].unk_2C[j] & 0x00F00000) >> 20;
                playerIdx = (gActiveBgms[i].unk_2C[j] & 0x000F0000) >> 16;
                temp_s0_3 = (gActiveBgms[i].unk_2C[j] & 0xFF00) >> 8;
                temp_a3_3 = gActiveBgms[i].unk_2C[j] & 0xFF;

                switch (temp_a0) {
                    case 0:
                        Audio_SetVolScale(playerIdx, 1, 0x7F, temp_a3_3);
                        break;
                    case 7:
                        if (D_801FFD34[i] == temp_a3_3) {
                            Audio_SetVolScale(playerIdx, 1, 0x7F, temp_s0_3);
                        }
                        break;
                    case 1:
                        Audio_SeqCmd3((u8)(i + 8), gActiveBgms[i].seqId1);
                        break;
                    case 2:
                        Audio_StartSeq((u8)(playerIdx + 8), 1, gActiveBgms[playerIdx].seqId1);
                        gActiveBgms[playerIdx].fadeVolUpdate = 1;
                        gActiveBgms[playerIdx].volScales[1] = 0x7F;
                        break;
                    case 3:
                        Audio_SeqCmdB30((u8)(playerIdx + 8), temp_s0_3, temp_a3_3);
                        break;
                    case 4:
                        Audio_SeqCmdB40((u8)(playerIdx + 8), temp_a3_3, 0);
                        break;
                    case 5:
                        temp_v1 = gActiveBgms[i].unk_2C[j] & 0xFFFF;
                        Audio_StartSeq((u8)(playerIdx + 8), gActiveBgms[playerIdx].unk_4E, temp_v1);
                        Audio_SetVolScale(playerIdx, 1, 0x7F, 0);
                        gActiveBgms[playerIdx].unk_4E = 0;
                        break;
                    case 6:
                        gActiveBgms[i].unk_4E = temp_s0_3;
                        break;
                    case 8:
                        Audio_SetVolScale(playerIdx, temp_s0_3, 0x7F, temp_a3_3);
                        break;
                    case 14:
                        if (temp_a3_3 & 1) {
                            Audio_QueueCmdS32(0xE3000000, SEQUENCE_TABLE);
                        }
                        if (temp_a3_3 & 2) {
                            Audio_QueueCmdS32(0xE3000000, FONT_TABLE);
                        }
                        if (temp_a3_3 & 4) {
                            Audio_QueueCmdS32(0xE3000000, SAMPLE_TABLE);
                        }
                        break;
                    case 9:
                        temp_v1 = gActiveBgms[i].unk_2C[j] & 0xFFFF;
                        Audio_SeqCmdA((u8)(playerIdx + 8), temp_v1);
                        break;
                    case 10:
                        Audio_SeqCmd5((u8)(playerIdx + 8), temp_s0_3, (temp_a3_3 * 10) & 0xFFFF);
                        break;
                }
            }

            gActiveBgms[i].unk_4D = 0;
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

// extern ReverbSettings* D_801DB930[10];
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
        D_801FFD34[playerIdx] = 0;
        gActiveBgms[playerIdx].seqId1 = NA_BGM_DISABLED;
        gActiveBgms[playerIdx].seqId2 = NA_BGM_DISABLED;
        gActiveBgms[playerIdx].unk_28 = 0;
        gActiveBgms[playerIdx].unk_18 = 0;
        gActiveBgms[playerIdx].unk_14 = 0;
        gActiveBgms[playerIdx].unk_258 = 0;
        gActiveBgms[playerIdx].unk_4D = 0;
        gActiveBgms[playerIdx].unk_4E = 0;
        gActiveBgms[playerIdx].unk_250 = 0;
        gActiveBgms[playerIdx].unk_252 = 0;
        gActiveBgms[playerIdx].unk_260 = false;
        gActiveBgms[playerIdx].unk_21B = 0;
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
        gActiveBgms[playerIdx].unk_0C = 0;
        gActiveBgms[playerIdx].fadeVolUpdate = 0;
        for (j = 0; j < ARRAY_COUNT(gActiveBgms[playerIdx].volScales); j++) {
            gActiveBgms[playerIdx].volScales[j] = 0x7F;
        }
    }
    Audio_ResetBgms();
}
