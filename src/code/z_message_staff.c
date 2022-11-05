#include "global.h"
#include "message_data_static.h"

void Message_FindCreditsMessage(PlayState* play, u16 textId) {
    MessageContext* msgCtx = &play->msgCtx;
    Font* font = &msgCtx->font;
    MessageTableEntry* msgEntry = msgCtx->messageTableStaff;
    const char* segment = msgEntry->segment;
    const char* foundSegment;
    const char* nextSegment;

    while (msgEntry->textId != 0xFFFF) {
        if (msgEntry->textId == textId) {
            foundSegment = msgEntry->segment;
            font->charBuf[font->unk_11D88][0] = msgEntry->typePos;
            msgEntry++;
            nextSegment = msgEntry->segment;
            font->messageStart = foundSegment - segment;
            font->messageEnd = nextSegment - foundSegment;
            break;
        }
        msgEntry++;
    }
}

extern f32 sFontWidths[];

#ifdef NON_MATCHING
void Message_DrawTextCredits(PlayState* play, Gfx** gfxP) {
    MessageContext* msgCtx = &play->msgCtx;
    u16 lookAheadCharacter;
    u16 j;
    u16 i;
    u16 sfxHi;
    char character;
    u16 charTexIndex;
    Font* font = &msgCtx->font;
    Gfx* gfx = *gfxP;

    msgCtx->textPosX = msgCtx->unk11FF8;

    if (!msgCtx->textIsCredits) {
        msgCtx->textPosY = msgCtx->unk11FFA;
    } else {
        msgCtx->textPosY = 48;
    }

    if (msgCtx->textBoxType == 5) {
        msgCtx->textColorR = msgCtx->textColorG = msgCtx->textColorB = 0;
    } else {
        msgCtx->textColorR = msgCtx->textColorG = msgCtx->textColorB = 255;
    }

    charTexIndex = 0;

    for (i = 0; i < msgCtx->textDrawPos; i++) {
        character = msgCtx->decodedBuffer.schar[i];

        switch (character) {
            case 0x1: // MESSAGE_NEWLINE
                msgCtx->textPosX = msgCtx->unk11FF8;
                if ((msgCtx->choiceNum == 1) || (msgCtx->choiceNum == 3)) {
                    msgCtx->textPosX += 32;
                }
                if ((msgCtx->choiceNum == 2) && (msgCtx->textPosY != msgCtx->unk11FFA)) {
                    msgCtx->textPosX += 32;
                }
                msgCtx->textPosY += msgCtx->unk11FFC;
                break;

            case 0x5: // Remnant of MESSAGE_COLOR
                break;

            case ' ':
                msgCtx->textPosX += 6;
                break;

            case 0x4: // MESSAGE_BOX_BREAK
                if (msgCtx->msgMode == MSGMODE_TEXT_DISPLAYING) {
                    if (!msgCtx->textboxSkipped) {
                        play_sound(0);
                        msgCtx->msgMode = MSGMODE_TEXT_AWAIT_NEXT;
                        Font_LoadMessageBoxEndIcon(font, 0);
                    } else {
                        msgCtx->msgMode = MSGMODE_TEXT_NEXT_MSG;
                        msgCtx->textUnskippable = false;
                        msgCtx->msgBufPos++;
                    }
                }
                *gfxP = gfx;
                return;

            case 0x6: // MESSAGE_SHIFT
                msgCtx->textPosX += msgCtx->decodedBuffer.schar[++i];
                break;

            case 0x7: // MESSAGE_TEXTID
                msgCtx->textboxEndType = TEXTBOX_ENDTYPE_20;
                if (msgCtx->msgMode == MSGMODE_TEXT_DISPLAYING) {
                    play_sound(0);
                    msgCtx->msgMode = MSGMODE_TEXT_DONE;
                    Font_LoadMessageBoxEndIcon(font, 0);
                }
                *gfxP = gfx;
                return;

            case 0x8: // MESSAGE_QUICKTEXT_ENABLE
                if (((i + 1) == msgCtx->textDrawPos) &&
                    ((msgCtx->msgMode == MSGMODE_TEXT_DISPLAYING) ||
                     ((msgCtx->msgMode >= MSGMODE_OCARINA_STARTING) && (msgCtx->msgMode <= MSGMODE_26)))) {
                    j = i;
                    while (true) {
                        lookAheadCharacter = msgCtx->decodedBuffer.schar[j];
                        if (lookAheadCharacter == 6) {
                            j += 2;
                            continue;
                        }
                        if ((lookAheadCharacter != 9) && (lookAheadCharacter != 0xA) && (lookAheadCharacter != 0xB) &&
                            (lookAheadCharacter != 0xC) && (lookAheadCharacter != 0xD) && (lookAheadCharacter != 4) &&
                            (lookAheadCharacter != 2)) {
                            j++;
                            continue;
                        }
                        break;
                    }
                    i = j - 1;
                    msgCtx->textDrawPos = i + 1;
                }
                // fallthrough
            case 0x9: // MESSAGE_QUICKTEXT_DISABLE
                break;

            case 0xD: // MESSAGE_AWAIT_BUTTON_PRESS
                if ((i + 1) == msgCtx->textDrawPos) {
                    if (msgCtx->msgMode == MSGMODE_TEXT_DISPLAYING) {
                        msgCtx->msgMode = MSGMODE_TEXT_AWAIT_INPUT;
                        Font_LoadMessageBoxEndIcon(font, 0);
                    }
                    *gfxP = gfx;
                    return;
                }
                break;

            case 0xC: // MESSAGE_BOX_BREAK_DELAYED
                if (msgCtx->msgMode == MSGMODE_TEXT_DISPLAYING) {
                    msgCtx->stateTimer = msgCtx->decodedBuffer.schar[++i];
                    msgCtx->msgMode = MSGMODE_TEXT_DELAYED_BREAK;
                }
                *gfxP = gfx;
                return;

            case 0x11: // MESSAGE_FADE2
                if (msgCtx->msgMode == MSGMODE_TEXT_DISPLAYING) {
                    msgCtx->msgMode = MSGMODE_TEXT_DONE;
                    msgCtx->textboxEndType = TEXTBOX_ENDTYPE_50;
                    msgCtx->stateTimer = msgCtx->decodedBuffer.schar[++i] << 8;
                    msgCtx->stateTimer |= msgCtx->decodedBuffer.schar[++i];
                }
                *gfxP = gfx;
                return;

            case 0x12: // MESSAGE_SFX
                if ((msgCtx->msgMode == MSGMODE_TEXT_DISPLAYING) && !msgCtx->messageHasSetSfx) {
                    msgCtx->messageHasSetSfx = true;
                    sfxHi = msgCtx->decodedBuffer.schar[i + 1];
                    sfxHi = sfxHi << 8;
                    play_sound(sfxHi | msgCtx->decodedBuffer.schar[i + 2]);
                }
                i += 2;
                break;

            case 0x13: // remnant of MESSAGE_ITEM_ICON
                break;

            case 0x15: // MESSAGE_BACKGROUND
                if (msgCtx->msgMode == MSGMODE_TEXT_DISPLAYING) {
                    play_sound(0);
                }
                gDPPipeSync(gfx++);
                gDPSetCombineMode(gfx++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
                gDPSetPrimColor(gfx++, 0, 0, 0, 0, 0, msgCtx->textColorAlpha);
                gDPLoadTextureBlock_4b(gfx++, msgCtx->textboxSegment + 0x1000, 4, 96, 48, 0, 0 | 0, 0 | 0, 0, 0, 0, 0);
                gSPTextureRectangle(gfx++, (msgCtx->textPosX + 1) << 2, (msgCtx->unk12012 + 1) << 2,
                                    (msgCtx->textPosX + 0x61) << 2, (msgCtx->unk12012 + 0x31) << 2, 0, 0, 0, 0x0400,
                                    0x0400);
                gDPLoadTextureBlock_4b(gfx++, msgCtx->textboxSegment + 0x1900, 4, 96, 48, 0, 0 | 0, 0 | 0, 0, 0, 0, 0);
                gSPTextureRectangle(gfx++, (msgCtx->textPosX + 0x61) << 2, (msgCtx->unk12012 + 1) << 2,
                                    (msgCtx->textPosX + 0xC2) << 2, (msgCtx->unk12012 + 0x31) << 2, 0, 0, 0, 0x0400,
                                    0x0400);
                gDPPipeSync(gfx++);
                gDPSetPrimColor(gfx++, 0, 0, 255, 60, 0, msgCtx->textColorAlpha);
                gDPLoadTextureBlock_4b(gfx++, msgCtx->textboxSegment + 0x1000, 4, 96, 48, 0, 0 | 0, 0 | 0, 0, 0, 0, 0);
                gSPTextureRectangle(gfx++, msgCtx->textPosX << 2, msgCtx->unk12012 << 2, (msgCtx->textPosX + 0x60) << 2,
                                    (msgCtx->unk12012 + 0x30) << 2, 0, 0, 0, 0x0400, 0x0400);
                gDPLoadTextureBlock_4b(gfx++, msgCtx->textboxSegment + 0x1900, 4, 96, 48, 0, 0 | 0, 0 | 0, 0, 0, 0, 0);
                gSPTextureRectangle(gfx++, (msgCtx->textPosX + 0x60) << 2, msgCtx->unk12012 << 2,
                                    (msgCtx->textPosX + 0xC0) << 2, (msgCtx->unk12012 + 0x30) << 2, 0, 0, 0, 0x0400,
                                    0x0400);
                gDPPipeSync(gfx++);
                gDPSetCombineLERP(gfx++, 0, 0, 0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, PRIMITIVE, TEXEL0, 0,
                                  PRIMITIVE, 0);
                msgCtx->textPosX += 32;
                break;

            case 0x14: // MESSAGE_TEXT_SPEED
                msgCtx->textDelay = msgCtx->decodedBuffer.schar[++i];
                break;

            case 0x1A: // MESSAGE_UNSKIPPABLE
                msgCtx->textUnskippable = true;
                break;

            case 0x1B: // MESSAGE_TWO_CHOICE
                msgCtx->textboxEndType = TEXTBOX_ENDTYPE_10;
                if (msgCtx->msgMode == MSGMODE_TEXT_DISPLAYING) {
                    msgCtx->choiceTextId = msgCtx->currentTextId;
                    msgCtx->stateTimer = 4;
                    msgCtx->choiceIndex = 0;
                    Font_LoadMessageBoxEndIcon(font, 2);
                }
                break;

            case 0x1C: // MESSAGE_THREE_CHOICE
                msgCtx->textboxEndType = TEXTBOX_ENDTYPE_11;
                if (msgCtx->msgMode == MSGMODE_TEXT_DISPLAYING) {
                    msgCtx->choiceTextId = msgCtx->currentTextId;
                    msgCtx->stateTimer = 4;
                    msgCtx->choiceIndex = 0;
                    Font_LoadMessageBoxEndIcon(font, 2);
                }
                break;

            case 0x2: // MESSAGE_END
                if (msgCtx->msgMode == MSGMODE_TEXT_DISPLAYING) {
                    msgCtx->msgMode = MSGMODE_TEXT_DONE;
                    if (msgCtx->textboxEndType == TEXTBOX_ENDTYPE_00) {
                        play_sound(NA_SE_SY_MESSAGE_END);
                        Font_LoadMessageBoxEndIcon(font, 1);
                        if (play->csCtx.state == 0) {
                            func_8011552C(play, 3);
                        }
                    }
                }
                *gfxP = gfx;
                return;

            case 0x10: // MESSAGE_OCARINA
                if ((i + 1) == msgCtx->textDrawPos) {
                    Message_HandleOcarina(play);
                    *gfxP = gfx;
                    return;
                }
                break;

            case 0xE: // MESSAGE_FADE
                if (msgCtx->msgMode == MSGMODE_TEXT_DISPLAYING) {
                    msgCtx->msgMode = MSGMODE_TEXT_DONE;
                    msgCtx->textboxEndType = TEXTBOX_ENDTYPE_50;
                    msgCtx->stateTimer = msgCtx->decodedBuffer.schar[++i];
                    Font_LoadMessageBoxEndIcon(font, 1);
                    if (play->csCtx.state == 0) {
                        func_8011552C(play, 3);
                    }
                }
                *gfxP = gfx;
                return;

            case 0xA: // MESSAGE_PERSISTENT
                if (msgCtx->msgMode == MSGMODE_TEXT_DISPLAYING) {
                    play_sound(0);
                    msgCtx->msgMode = MSGMODE_TEXT_DONE;
                    msgCtx->textboxEndType = TEXTBOX_ENDTYPE_30;
                }
                *gfxP = gfx;
                return;

            case 0xB: // MESSAGE_EVENT
                if (msgCtx->msgMode == MSGMODE_TEXT_DISPLAYING) {
                    msgCtx->msgMode = MSGMODE_TEXT_DONE;
                    msgCtx->textboxEndType = TEXTBOX_ENDTYPE_40;
                    Font_LoadMessageBoxEndIcon(font, 0);
                    play_sound(NA_SE_SY_MESSAGE_END);
                }
                *gfxP = gfx;
                return;

            default:
                if ((msgCtx->msgMode == MSGMODE_TEXT_DISPLAYING) && ((i + 1) == msgCtx->textDrawPos) &&
                    (msgCtx->textDelayTimer == msgCtx->textDelay)) {
                    play_sound(0);
                }
                Message_DrawTextChar(play, &font->charBuf[font->unk_11D88][charTexIndex], &gfx);
                charTexIndex += 0x80;

                msgCtx->textPosX += (s32)(sFontWidths[character - ' '] * msgCtx->textCharScale);
                break;
        }
    }

    if (msgCtx->textDelayTimer == 0) {
        msgCtx->textDrawPos = i + 1;
        msgCtx->textDelayTimer = msgCtx->textDelay;
    } else {
        msgCtx->textDelayTimer--;
    }

    *gfxP = gfx;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/z_message_staff/Message_DrawTextCredits.s")
#endif

#pragma GLOBAL_ASM("asm/non_matchings/code/z_message_staff/Message_DecodeCredits.s")
