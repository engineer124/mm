#include "global.h"
#include "message_data_static.h"
#include "overlays/kaleido_scope/ovl_kaleido_scope/z_kaleido_scope.h"

#if 0

//fontWidth
extern f32 D_801D0470[159];

//rupeesTextLocalization
u8 D_801D06F0[4][8] = { { "rupee(s)" }, //EN
                        { "rubin(e)" }, //DE
                        { "rubis"    }, //FR
                        { "rupia(s)" }  //SPA 
                        };

// rupeesTextLength
u8 D_801D0710[4] = {8,8,5,8};

//TextArea
char D_801D0714[11][16] = {
                                    "Great Bay Coast",
                                    "Zora Cape",
                                    "Snowhead",
                                    "Mountain Village",
                                    "Clock Town",
                                    "Milk Road",
                                    "Woodfall",
                                    "Southern Swamp",
                                    "Ikana Canyon",
                                    "Stone Tower",
                                    "Entrance"
                                };

//TextAreaLength
s16 D_801D07C4[11] = {15,9,8,16,10,9,8,14,12,11,8};

#endif

extern f32 D_801D0470[159];
extern u8 D_801D06F0[4][8];
extern u8 D_801D0710[4];
extern u8 D_801D0714[11][16];
extern s16 D_801D07C4[11];

void Message_FindMessageNES(PlayState* play, u16 textId) {
    MessageContext* msgCtx = &play->msgCtx;
    Font* font = &msgCtx->font;
    MessageTableEntry* msgEntry = msgCtx->messageEntryTableNes;
    const char* segment = msgEntry->segment;
    const char* foundSegment;
    const char* nextSegment;

    while (msgEntry->textId != 0xFFFF) {
        if (msgEntry->textId == textId) {
            foundSegment = msgEntry->segment;
            msgEntry++;
            nextSegment = msgEntry->segment;
            font->messageStart = foundSegment - segment;
            font->messageEnd = nextSegment - foundSegment;
            return;
        }
        msgEntry++;
    }

    msgEntry = msgCtx->messageEntryTableNes;
    foundSegment = msgEntry->segment;
    msgEntry++;
    nextSegment = msgEntry->segment;
    font->messageStart = foundSegment - segment;
    font->messageEnd = nextSegment - foundSegment;
}

void Message_LoadCharNES(PlayState* play, u8 codePointIndex, s32* offset, f32* arg3, s16 decodedBufPos) {
    MessageContext* msgCtx = &play->msgCtx;
    s32 temp1 = *offset;
    f32 temp2 = *arg3;

    Font_LoadCharNES(play, codePointIndex, temp1);
    msgCtx->decodedBuffer.schar[decodedBufPos] = codePointIndex;
    temp1 += FONT_CHAR_TEX_SIZE;
    temp2 += (16.0f * msgCtx->unk12098);
    *offset = temp1;
    *arg3 = temp2;
}

void Message_LoadPluralRupeesNES(PlayState* play, s16* decodedBufPos, s32* offset, f32* arg3) {
    MessageContext* msgCtx = &play->msgCtx;
    s16 p = *decodedBufPos;
    s32 o = *offset;
    f32 f = *arg3;

    msgCtx->decodedBuffer.schar[p] = 0x20;
    p++;
    Font_LoadCharNES(play, 'R', o);
    o += FONT_CHAR_TEX_SIZE;
    msgCtx->decodedBuffer.schar[p] = 'R';
    p++;
    Font_LoadCharNES(play, 'u', o);
    o += FONT_CHAR_TEX_SIZE;
    msgCtx->decodedBuffer.schar[p] = 'u';
    p++;
    Font_LoadCharNES(play, 'p', o);
    o += FONT_CHAR_TEX_SIZE;
    msgCtx->decodedBuffer.schar[p] = 'p';
    p++;
    Font_LoadCharNES(play, 'e', o);
    o += FONT_CHAR_TEX_SIZE;
    msgCtx->decodedBuffer.schar[p] = 'e';
    p++;
    Font_LoadCharNES(play, 'e', o);
    o += FONT_CHAR_TEX_SIZE;
    msgCtx->decodedBuffer.schar[p] = 'e';
    p++;
    Font_LoadCharNES(play, 's', o);
    o += FONT_CHAR_TEX_SIZE;
    msgCtx->decodedBuffer.schar[p] = 's';

    f += 16.0f * msgCtx->unk12098 * 6.0f;
    *decodedBufPos = p;
    *offset = o;
    *arg3 = f;
}

void Message_LoadLocalizedRupeesNES(PlayState* play, s16* decodedBufPos, s32* offset, f32* arg3) {
    MessageContext* msgCtx = &play->msgCtx;
    s16 p = *decodedBufPos;
    s32 o = *offset;
    f32 f = *arg3;
    u8 j;

    msgCtx->decodedBuffer.schar[p] = ' ';
    p++;

    for (j = 0; j < D_801D0710[gSaveContext.options.language - 1]; j++) {
        Font_LoadCharNES(play, D_801D06F0[gSaveContext.options.language - 1][j], o);
        msgCtx->decodedBuffer.schar[p] = D_801D06F0[gSaveContext.options.language - 1][j];
        o += FONT_CHAR_TEX_SIZE;
        p++;
    }

    p--;
    f += 16.0f * msgCtx->unk12098 * (D_801D0710[gSaveContext.options.language - 1] + 1);
    *decodedBufPos = p;
    *offset = o;
    *arg3 = f;
}

void Message_LoadRupeesNES(PlayState* play, s16* decodedBufPos, s32* offset, f32* arg3, s16 singular) {
    MessageContext* msgCtx = &play->msgCtx;
    s16 p = *decodedBufPos;
    s32 o = *offset;
    f32 f = *arg3;

    msgCtx->decodedBuffer.schar[p] = ' ';
    p++;
    Font_LoadCharNES(play, 'R', o);
    o += FONT_CHAR_TEX_SIZE;
    msgCtx->decodedBuffer.schar[p] = 'R';
    p++;
    Font_LoadCharNES(play, 'u', o);
    o += FONT_CHAR_TEX_SIZE;
    msgCtx->decodedBuffer.schar[p] = 'u';
    p++;
    Font_LoadCharNES(play, 'p', o);
    o += FONT_CHAR_TEX_SIZE;
    msgCtx->decodedBuffer.schar[p] = 'p';
    p++;
    Font_LoadCharNES(play, 'e', o);
    o += FONT_CHAR_TEX_SIZE;
    msgCtx->decodedBuffer.schar[p] = 'e';
    p++;
    Font_LoadCharNES(play, 'e', o);
    o += FONT_CHAR_TEX_SIZE;
    msgCtx->decodedBuffer.schar[p] = 'e';

    if (singular != 1) {
        p++;
        Font_LoadCharNES(play, 's', o);
        o += FONT_CHAR_TEX_SIZE;
        msgCtx->decodedBuffer.schar[p] = 's';
        f += 16.0f * msgCtx->unk12098 * 6.0f;
    } else {
        f += 16.0f * msgCtx->unk12098 * 5.0f;
    }

    *decodedBufPos = p;
    *offset = o;
    *arg3 = f;
}

void Message_LoadTimeNES(PlayState* play, u8 arg1, s32* offset, f32* arg3, s16* decodedBufPos) {
    MessageContext* msgCtx = &play->msgCtx;
    s16 p = *decodedBufPos;
    s32 o = *offset;
    f32 f = *arg3;
    u32 timeLeft;
    s16 digits[4];
    f32 timeLeftInMinutes;
    s16 i;

    if (arg1 == 0xCF) {
        // Calculates the time left before the moon crashes.
        // The day begins at CLOCK_TIME(6, 0) so it must be offset.
        timeLeft = (4 - CURRENT_DAY) * DAY_LENGTH - (u16)(((void)0, gSaveContext.save.time) - CLOCK_TIME(6, 0));
    } else {
        // Calculates the time left before a new day.
        // The day begins at CLOCK_TIME(6, 0) so it must be offset.
        timeLeft = DAY_LENGTH - (u16)(((void)0, gSaveContext.save.time) - CLOCK_TIME(6, 0));
    }

    timeLeftInMinutes = TIME_TO_MINUTES_F(timeLeft);

    digits[0] = 0;
    digits[1] = (timeLeftInMinutes / 60.0f);
    while (digits[1] >= 10) {
        digits[0]++;
        digits[1] -= 10;
    }

    digits[2] = 0;
    digits[3] = (s32)timeLeftInMinutes % 60;
    while (digits[3] >= 10) {
        digits[2]++;
        digits[3] -= 10;
    }

    for (i = 0; i < 4; i++) {
        Font_LoadCharNES(play, digits[i] + '0', o);
        o += FONT_CHAR_TEX_SIZE;
        msgCtx->decodedBuffer.schar[p] = digits[i] + '0';
        p++;
        if (i == 1) {
            Font_LoadCharNES(play, ':', o);
            o += FONT_CHAR_TEX_SIZE;
            msgCtx->decodedBuffer.schar[p] = ':';
            p++;
        }
    }

    f += 5.0f * (16.0f * msgCtx->unk12098);
    *decodedBufPos = p - 1;
    *offset = o;
    *arg3 = f;
}

void Message_LoadAreaTextNES(PlayState* play, s32* offset, f32* arg2, s16* decodedBufPos) {
    MessageContext* msgCtx = &play->msgCtx;
    s16 p = *decodedBufPos;
    s32 o = *offset;
    f32 f = *arg2;
    s16 i;
    u8 currentChar;
    s16 currentArea;
    s16 stringLimit;

    if ((func_8010A0A4(play) != 0) || (play->sceneNum == SCENE_SECOM)) {
        currentArea = 10;
    } else {
        currentArea = play->pauseCtx.unk_238[PAUSE_WORLD_MAP];
    }
    stringLimit = D_801D07C4[currentArea];

    for (i = 0; i < stringLimit; i++) {
        msgCtx->decodedBuffer.schar[p] = D_801D0714[currentArea][i];
        currentChar = msgCtx->decodedBuffer.schar[p];
        if (currentChar != ' ') {
            Font_LoadCharNES(play, D_801D0714[currentArea][i], o);
            o += FONT_CHAR_TEX_SIZE;
        }
        currentChar = msgCtx->decodedBuffer.schar[p];
        p++;
        f += (D_801D0470[currentChar - ' '] * msgCtx->unk12098);
    }

    p--;
    f += (stringLimit - 1) * (16.0f * msgCtx->unk12098);
    *decodedBufPos = p;
    *offset = o;
    *arg2 = f;
}

#pragma GLOBAL_ASM("asm/non_matchings/code/z_message_nes/func_80159438.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_message_nes/func_8015966C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_message_nes/func_8015B198.s")
