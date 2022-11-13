#include "global.h"
#include "z64shrink_window.h"
#include "z64view.h"
#include "message_data_static.h"
#include "interface/parameter_static/parameter_static.h"
#include "overlays/kaleido_scope/ovl_kaleido_scope/z_kaleido_scope.h"

#if 0

s16 sOcarinaEffectActorIds[15] = {
    ACTOR_OCEFF_WIPE5, ACTOR_OCEFF_WIPE5, // Sonata of Awakening Effect, Sonata of Awakening Effect
    ACTOR_OCEFF_WIPE5, ACTOR_OCEFF_WIPE5, // Sonata of Awakening Effect, Sonata of Awakening Effect
    ACTOR_OCEFF_WIPE5, ACTOR_OCEFF_WIPE5, // Sonata of Awakening Effect, Sonata of Awakening Effect
    ACTOR_OCEFF_WIPE,  ACTOR_OCEFF_WIPE7, // Song of Time Effect, Song of Healing Effect
    ACTOR_OCEFF_WIPE2, ACTOR_OCEFF_WIPE6, // Epona's Song Effect, Song of Soaring Effect
    ACTOR_OCEFF_STORM, ACTOR_OCEFF_SPOT,  // Song of Storms Effect II [?], Sun's Song Effect
    ACTOR_OCEFF_WIPE, ACTOR_OCEFF_WIPE,   // Song of Time Effect, Song of Time Effect
    ACTOR_OCEFF_WIPE4                     // Scarecrow's Song Effect 
};
s32 sOcarinaEffectActorParams[15] = { 0,1,2,3,4,0,1,0,0,0,0,0,1,1,0 };

#endif

extern s16 sTextboxHeight;
extern s16 sTextboxTexWidth;
extern s16 sTextboxTexHeight;
extern f32 D_801CFDA4[];
extern f32 D_801CFDC8[];
extern s16 sTextboxWidth;

extern s16 D_801CFF94[];

extern u8 D_801C6A70;
extern s32 D_801F6B08;

extern s16 D_801CFF70[];
extern s16 D_801CFF7C[];
extern s16 D_801CFF88[];

extern s16 sOcarinaButtonIndexBufPos;
extern s16 sOcarinaButtonIndexBufLen;

extern s16 D_801CFC78[15];

extern s16 D_801CFD10[][3];
extern s16 D_801CFD1C[][3];
extern s16 D_801CFD28;
extern s16 D_801CFD2C;
extern s16 D_801CFD30;
extern s16 D_801CFD34;
extern s16 D_801CFD38;
extern s16 D_801CFD3C;
extern s16 D_801CFD40;
extern s16 D_801CFD44;

extern s32 sCharTexSize;
extern s32 sCharTexScale;

extern u16 D_801C6AB8[];
extern u16 D_801C6B28[];
extern s16 sOcarinaEffectActorIds[];
extern s32 sOcarinaEffectActorParams[];
extern s16 sOcarinaButtonAPrimR;
extern s16 sOcarinaButtonAPrimB;
extern s16 sOcarinaButtonAPrimG;
extern s16 sOcarinaButtonAEnvR;
extern s16 sOcarinaButtonAEnvB;
extern s16 sOcarinaButtonAEnvG;
extern s16 sOcarinaButtonCPrimR;
extern s16 sOcarinaButtonCPrimB;
extern s16 sOcarinaButtonCPrimG;
extern s16 sOcarinaButtonCEnvR;
extern s16 sOcarinaButtonCEnvB;
extern s16 sOcarinaButtonCEnvG;
extern MessageTableEntry D_801C6B98[];
extern MessageTableEntry D_801CFB08[];

void Message_ResetOcarinaButtonAlphas(void) {
    sOcarinaButtonIndexBuf[0] = OCARINA_BTN_INVALID;
    sOcarinaButtonAlphaValues[0] = sOcarinaButtonAlphaValues[1] = sOcarinaButtonAlphaValues[2] =
        sOcarinaButtonAlphaValues[3] = sOcarinaButtonAlphaValues[4] = sOcarinaButtonAlphaValues[5] =
            sOcarinaButtonAlphaValues[6] = sOcarinaButtonAlphaValues[7] = sOcarinaButtonAlphaValues[8] = 0;
}

void Message_ResetOcarinaButtonState(PlayState* play) {
    MessageContext* msgCtx = &play->msgCtx;

    msgCtx->ocarinaButtonsPosY[0] = 189;
    msgCtx->ocarinaButtonsPosY[1] = 184;
    msgCtx->ocarinaButtonsPosY[2] = 179;
    msgCtx->ocarinaButtonsPosY[3] = 174;
    msgCtx->ocarinaButtonsPosY[4] = 169;

    Message_ResetOcarinaButtonAlphas();

    sOcarinaButtonAPrimR = 80;
    sOcarinaButtonAPrimG = 150;
    sOcarinaButtonAPrimB = 255;
    sOcarinaButtonAEnvR = 10;
    sOcarinaButtonAEnvG = 10;
    sOcarinaButtonAEnvB = 10;
    sOcarinaButtonCPrimR = 255;
    sOcarinaButtonCPrimG = 255;
    sOcarinaButtonCPrimB = 50;
    sOcarinaButtonCEnvR = 10;
    sOcarinaButtonCEnvG = 10;
    sOcarinaButtonCEnvB = 10;
}

s32 Message_ShouldAdvance(PlayState* play) {
    MessageContext* msgCtx = &play->msgCtx;
    Input* controller = CONTROLLER1(&play->state);

    if ((msgCtx->textboxEndType == 0x10) || (msgCtx->textboxEndType == 0x11)) {
        if (CHECK_BTN_ALL(controller->press.button, BTN_A)) {
            play_sound(NA_SE_SY_MESSAGE_PASS);
        }
        return CHECK_BTN_ALL(controller->press.button, BTN_A);
    } else {
        if (CHECK_BTN_ALL(controller->press.button, BTN_A) || CHECK_BTN_ALL(controller->press.button, BTN_B) ||
            CHECK_BTN_ALL(controller->press.button, BTN_CUP)) {
            play_sound(NA_SE_SY_MESSAGE_PASS);
        }
        return CHECK_BTN_ALL(controller->press.button, BTN_A) || CHECK_BTN_ALL(controller->press.button, BTN_B) ||
               CHECK_BTN_ALL(controller->press.button, BTN_CUP);
    }
}

s32 Message_ShouldAdvanceSilent(PlayState* play) {
    MessageContext* msgCtx = &play->msgCtx;
    Input* controller = CONTROLLER1(&play->state);

    if ((msgCtx->textboxEndType == 0x10) || (msgCtx->textboxEndType == 0x11)) {
        return CHECK_BTN_ALL(controller->press.button, BTN_A);
    } else {
        return CHECK_BTN_ALL(controller->press.button, BTN_A) || CHECK_BTN_ALL(controller->press.button, BTN_B) ||
               CHECK_BTN_ALL(controller->press.button, BTN_CUP);
    }
}

void func_801477B4(PlayState* play) {
    MessageContext* msgCtx = &play->msgCtx;

    if (play->msgCtx.msgLength != 0) {
        msgCtx->stateTimer = 2;
        msgCtx->msgMode = MSGMODE_TEXT_CLOSING;
        msgCtx->textboxEndType = 0;
        play_sound(NA_SE_PL_WALK_GROUND - SFX_FLAG);
    }
}

extern s16 sIconPrimColors[][3];
extern s16 sIconEnvColors[][3];

// Matching, in-function data
#ifdef NON_MATCHING
void Message_DrawTextboxIcon(PlayState* play, Gfx** gfxp, s16 x, s16 y) {
    static s16 sIconPrimR = 0;
    static s16 sIconPrimG = 80;
    static s16 sIconPrimB = 200;
    static s16 sIconFlashTimer = 12;
    static s16 sIconFlashColorIndex = 0;
    static s16 sIconEnvR = 0;
    static s16 sIconEnvG = 0;
    static s16 sIconEnvB = 0;
    Gfx* gfx;
    MessageContext* msgCtx = &play->msgCtx;
    Font* font = &msgCtx->font;
    s16 primR;
    s16 primG;
    s16 primB;
    s16 envR;
    s16 envG;
    s16 envB;
    u8* iconTexture = msgCtx->font.iconBuf;

    gfx = *gfxp;

    if (!msgCtx->textIsCredits) {
        primR = ABS_ALT(sIconPrimR - sIconPrimColors[sIconFlashColorIndex][0]) / sIconFlashTimer;
        primG = ABS_ALT(sIconPrimG - sIconPrimColors[sIconFlashColorIndex][1]) / sIconFlashTimer;
        primB = ABS_ALT(sIconPrimB - sIconPrimColors[sIconFlashColorIndex][2]) / sIconFlashTimer;

        if (sIconPrimR >= sIconPrimColors[sIconFlashColorIndex][0]) {
            sIconPrimR -= primR;
        } else {
            sIconPrimR += primR;
        }

        if (sIconPrimG >= sIconPrimColors[sIconFlashColorIndex][1]) {
            sIconPrimG -= primG;
        } else {
            sIconPrimG += primG;
        }

        if (sIconPrimB >= sIconPrimColors[sIconFlashColorIndex][2]) {
            sIconPrimB -= primB;
        } else {
            sIconPrimB += primB;
        }

        envR = ABS_ALT(sIconEnvR - sIconEnvColors[sIconFlashColorIndex][0]) / sIconFlashTimer;
        envG = ABS_ALT(sIconEnvG - sIconEnvColors[sIconFlashColorIndex][1]) / sIconFlashTimer;
        envB = ABS_ALT(sIconEnvB - sIconEnvColors[sIconFlashColorIndex][2]) / sIconFlashTimer;

        if (sIconEnvR >= sIconEnvColors[sIconFlashColorIndex][0]) {
            sIconEnvR -= envR;
        } else {
            sIconEnvR += envR;
        }

        if (sIconEnvG >= sIconEnvColors[sIconFlashColorIndex][1]) {
            sIconEnvG -= envG;
        } else {
            sIconEnvG += envG;
        }

        if (sIconEnvB >= sIconEnvColors[sIconFlashColorIndex][2]) {
            sIconEnvB -= envB;
        } else {
            sIconEnvB += envB;
        }

        sIconFlashTimer--;
        if (sIconFlashTimer == 0) {
            sIconPrimR = sIconPrimColors[sIconFlashColorIndex][0];
            sIconPrimG = sIconPrimColors[sIconFlashColorIndex][1];
            sIconPrimB = sIconPrimColors[sIconFlashColorIndex][2];
            sIconEnvR = sIconEnvColors[sIconFlashColorIndex][0];
            sIconEnvG = sIconEnvColors[sIconFlashColorIndex][1];
            sIconEnvB = sIconEnvColors[sIconFlashColorIndex][2];
            sIconFlashTimer = 12;
            sIconFlashColorIndex ^= 1;
        }

        gDPPipeSync(gfx++);
        gDPSetCombineLERP(gfx++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0, PRIMITIVE,
                          ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);
        gDPSetPrimColor(gfx++, 0, 0, sIconPrimR, sIconPrimG, sIconPrimB, 255);
        gDPSetEnvColor(gfx++, sIconEnvR, sIconEnvG, sIconEnvB, 255);

        if (!play->pauseCtx.bombersNotebookOpen) {
            gDPLoadTextureBlock_4b(gfx++, iconTexture, G_IM_FMT_I, 16, 16, 0, G_TX_NOMIRROR | G_TX_CLAMP,
                                   G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
            gSPTextureRectangle(gfx++, x << 2, y << 2, (x + sCharTexSize) << 2, (y + sCharTexSize) << 2,
                                G_TX_RENDERTILE, 0, 0, sCharTexScale, sCharTexScale);
        }

        msgCtx->stateTimer++;
        *gfxp = gfx;
    }
}
#else
void Message_DrawTextboxIcon(PlayState* play, Gfx** gfxp, s16 x, s16 y);
#pragma GLOBAL_ASM("asm/non_matchings/code/z_message/Message_DrawTextboxIcon.s")
#endif

// Matching, in-function data
#ifdef NON_MATCHING
void func_80147F18(PlayState* play, Gfx** gfxp, s16 arg2, s16 arg3) {
    static s16 D_801CFD28 = 0;
    static s16 D_801CFD2C = 80;
    static s16 D_801CFD30 = 200;
    static s16 D_801CFD34 = 12;
    static s16 D_801CFD38 = 0;
    static s16 D_801CFD3C = 0;
    static s16 D_801CFD40 = 0;
    static s16 D_801CFD44 = 0;
    Gfx* gfx;
    MessageContext* msgCtx = &play->msgCtx;
    s16 primR;
    s16 primG;
    s16 primB;
    s16 envR;
    s16 envG;
    s16 envB;

    gfx = *gfxp;

    if (!msgCtx->textIsCredits) {
        primR = ABS_ALT(D_801CFD28 - D_801CFD10[D_801CFD38][0]) / D_801CFD34;
        primG = ABS_ALT(D_801CFD2C - D_801CFD10[D_801CFD38][1]) / D_801CFD34;
        primB = ABS_ALT(D_801CFD30 - D_801CFD10[D_801CFD38][2]) / D_801CFD34;

        if (D_801CFD28 >= D_801CFD10[D_801CFD38][0]) {
            D_801CFD28 -= primR;
        } else {
            D_801CFD28 += primR;
        }

        if (D_801CFD2C >= D_801CFD10[D_801CFD38][1]) {
            D_801CFD2C -= primG;
        } else {
            D_801CFD2C += primG;
        }

        if (D_801CFD30 >= D_801CFD10[D_801CFD38][2]) {
            D_801CFD30 -= primB;
        } else {
            D_801CFD30 += primB;
        }

        envR = ABS_ALT(D_801CFD3C - D_801CFD1C[D_801CFD38][0]) / D_801CFD34;
        envG = ABS_ALT(D_801CFD40 - D_801CFD1C[D_801CFD38][1]) / D_801CFD34;
        envB = ABS_ALT(D_801CFD44 - D_801CFD1C[D_801CFD38][2]) / D_801CFD34;

        if (D_801CFD3C >= D_801CFD1C[D_801CFD38][0]) {
            D_801CFD3C -= envR;
        } else {
            D_801CFD3C += envR;
        }

        if (D_801CFD40 >= D_801CFD1C[D_801CFD38][1]) {
            D_801CFD40 -= envG;
        } else {
            D_801CFD40 += envG;
        }

        if (D_801CFD44 >= D_801CFD1C[D_801CFD38][2]) {
            D_801CFD44 -= envB;
        } else {
            D_801CFD44 += envB;
        }

        D_801CFD34--;
        if (D_801CFD34 == 0) {
            D_801CFD28 = D_801CFD10[D_801CFD38][0];
            D_801CFD2C = D_801CFD10[D_801CFD38][1];
            D_801CFD30 = D_801CFD10[D_801CFD38][2];
            D_801CFD3C = D_801CFD1C[D_801CFD38][0];
            D_801CFD40 = D_801CFD1C[D_801CFD38][1];
            D_801CFD44 = D_801CFD1C[D_801CFD38][2];
            D_801CFD34 = 12;
            D_801CFD38 ^= 1;
        }

        gDPPipeSync(gfx++);
        gDPSetRenderMode(gfx++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gDPSetCombineMode(gfx++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
        gDPSetPrimColor(gfx++, 0, 0, D_801CFD28, D_801CFD2C, D_801CFD30, 120);
        gDPFillRectangle(gfx++, (arg2 + 3), arg3, arg2 + 0x11, arg3 + 0xB);
        gDPFillRectangle(gfx++, arg2 + 6, arg3 - 2, arg2 + 0xE, arg3 + 0xD);
        gDPPipeSync(gfx++);

        msgCtx->stateTimer++;
        *gfxp = gfx++;
    }
}
#else
void func_80147F18(PlayState* play, Gfx** gfxp, s16 arg2, s16 arg3);
#pragma GLOBAL_ASM("asm/non_matchings/code/z_message/func_80147F18.s")
#endif

extern s16 D_801CFD48[][3];
extern s16 D_801CFD54[][3];

// Matching, in-function data
#ifdef NON_MATCHING
void func_80148558(PlayState* play, Gfx** gfxp, s16 arg2, s16 arg3) {
    static s16 D_801CFD60 = 0;
    static s16 D_801CFD64 = 80;
    static s16 D_801CFD68 = 200;
    static s16 D_801CFD6C = 12;
    static s16 D_801CFD70 = 0;
    static s16 D_801CFD74 = 0;
    static s16 D_801CFD78 = 0;
    static s16 D_801CFD7C = 0;
    Gfx* gfx;
    MessageContext* msgCtx = &play->msgCtx;
    s16 primR;
    s16 primG;
    s16 primB;
    s16 envR;
    s16 envG;
    s16 envB;

    gfx = *gfxp;

    if (!msgCtx->textIsCredits) {
        primR = ABS_ALT(D_801CFD60 - D_801CFD48[D_801CFD70][0]) / D_801CFD6C;
        primG = ABS_ALT(D_801CFD64 - D_801CFD48[D_801CFD70][1]) / D_801CFD6C;
        primB = ABS_ALT(D_801CFD68 - D_801CFD48[D_801CFD70][2]) / D_801CFD6C;

        if (D_801CFD60 >= D_801CFD48[D_801CFD70][0]) {
            D_801CFD60 -= primR;
        } else {
            D_801CFD60 += primR;
        }

        if (D_801CFD64 >= D_801CFD48[D_801CFD70][1]) {
            D_801CFD64 -= primG;
        } else {
            D_801CFD64 += primG;
        }

        if (D_801CFD68 >= D_801CFD48[D_801CFD70][2]) {
            D_801CFD68 -= primB;
        } else {
            D_801CFD68 += primB;
        }

        envR = ABS_ALT(D_801CFD74 - D_801CFD54[D_801CFD70][0]) / D_801CFD6C;
        envG = ABS_ALT(D_801CFD78 - D_801CFD54[D_801CFD70][1]) / D_801CFD6C;
        envB = ABS_ALT(D_801CFD7C - D_801CFD54[D_801CFD70][2]) / D_801CFD6C;

        if (D_801CFD74 >= D_801CFD54[D_801CFD70][0]) {
            D_801CFD74 -= envR;
        } else {
            D_801CFD74 += envR;
        }

        if (D_801CFD78 >= D_801CFD54[D_801CFD70][1]) {
            D_801CFD78 -= envG;
        } else {
            D_801CFD78 += envG;
        }

        if (D_801CFD7C >= D_801CFD54[D_801CFD70][2]) {
            D_801CFD7C -= envB;
        } else {
            D_801CFD7C += envB;
        }

        D_801CFD6C--;
        if (D_801CFD6C == 0) {
            D_801CFD60 = D_801CFD48[D_801CFD70][0];
            D_801CFD64 = D_801CFD48[D_801CFD70][1];
            D_801CFD68 = D_801CFD48[D_801CFD70][2];
            D_801CFD74 = D_801CFD54[D_801CFD70][0];
            D_801CFD78 = D_801CFD54[D_801CFD70][1];
            D_801CFD7C = D_801CFD54[D_801CFD70][2];
            D_801CFD6C = 12;
            D_801CFD70 ^= 1;
        }

        gDPPipeSync(gfx++);
        gDPSetRenderMode(gfx++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gDPSetCombineMode(gfx++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
        gDPSetPrimColor(gfx++, 0, 0, D_801CFD60, D_801CFD64, D_801CFD68, 120);
        gDPFillRectangle(gfx++, (arg2 + 3), arg3, arg2 + 0x1D, arg3 + 0xB);
        gDPFillRectangle(gfx++, arg2 + 6, arg3 - 2, arg2 + 0x1A, arg3 + 0xD);
        gDPPipeSync(gfx++);

        msgCtx->stateTimer++;
        *gfxp = gfx++;
    }
}
#else
void func_80148558(PlayState* play, Gfx** gfxp, s16 arg2, s16 arg3);
#pragma GLOBAL_ASM("asm/non_matchings/code/z_message/func_80148558.s")
#endif

// Matching, in-function data
#ifdef NON_MATCHING
void Message_HandleChoiceSelection(PlayState* play, u8 numChoices) {
    static s16 D_801CFD80 = false; // sAnalogStickHeld
    MessageContext* msgCtx = &play->msgCtx;
    Input* input = CONTROLLER1(&play->state);

    if ((input->rel.stick_y >= 30) && !D_801CFD80) {
        D_801CFD80 = true;
        msgCtx->choiceIndex--;
        if (msgCtx->choiceIndex > 128) {
            msgCtx->choiceIndex = 0;
        } else {
            play_sound(NA_SE_SY_CURSOR);
        }
    } else if ((input->rel.stick_y <= -30) && !D_801CFD80) {
        D_801CFD80 = true;
        msgCtx->choiceIndex++;
        if (msgCtx->choiceIndex > numChoices) {
            msgCtx->choiceIndex = numChoices;
        } else {
            play_sound(NA_SE_SY_CURSOR);
        }
    } else if (ABS_ALT(input->rel.stick_y) < 30) {
        D_801CFD80 = false;
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/z_message/Message_HandleChoiceSelection.s")
#endif

void func_80148CBC(PlayState* play, Gfx** gfxp, u8 arg2) {
    MessageContext* msgCtx = &play->msgCtx;

    msgCtx->textPosX = 0x30;
    if (arg2 == 1) {
        msgCtx->textPosY = msgCtx->unk11FFE[1 + msgCtx->choiceIndex];
    } else {
        msgCtx->textPosY = msgCtx->unk11FFE[msgCtx->choiceIndex];
    }
    Message_DrawTextboxIcon(play, gfxp, msgCtx->textPosX, msgCtx->textPosY);
}

// Matching, in-function data
#ifdef NON_MATCHING
void func_80148D64(PlayState* play) {
    static s16 D_801CFD84 = false; // sAnalogStickHeld
    MessageContext* msgCtx = &play->msgCtx;

    if (play->msgCtx.stickAdjY <= -30) {
        msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2]--;
        if (msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2] < '0') {
            msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2] = '9';
        }
        Font_LoadCharNES(play, msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2],
                         msgCtx->unk120C4 + (msgCtx->unk120C2 << 7));
        play_sound(NA_SE_SY_RUPY_COUNT);
    } else if (msgCtx->stickAdjY >= 30) {
        msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2]++;
        if (msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2] > '9') {
            msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2] = '0';
        }
        Font_LoadCharNES(play, msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2],
                         msgCtx->unk120C4 + (msgCtx->unk120C2 << 7));
        play_sound(NA_SE_SY_RUPY_COUNT);
    } else if ((msgCtx->stickAdjX >= 30) && !D_801CFD84) {
        D_801CFD84 = true;
        msgCtx->unk120C2++;
        if (msgCtx->unk120C2 > 2) {
            msgCtx->unk120C2 = 2;
        } else {
            play_sound(NA_SE_SY_CURSOR);
        }
    } else if ((msgCtx->stickAdjX <= -30) && !D_801CFD84) {
        D_801CFD84 = true;
        msgCtx->unk120C2--;
        if (msgCtx->unk120C2 < 0) {
            msgCtx->unk120C2 = 0;
        } else {
            play_sound(NA_SE_SY_CURSOR);
        }
    } else {
        D_801CFD84 = false;
    }

    msgCtx->bankRupeesSelected = (msgCtx->decodedBuffer.schar[msgCtx->unk120C0] * 100) - ('0' * 100);
    msgCtx->bankRupeesSelected += (msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + 1] * 10) - ('0' * 10);
    msgCtx->bankRupeesSelected += (msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + 2]) - '0';
}
#else
void func_80148D64(PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/code/z_message/func_80148D64.s")
#endif

void func_80149048(PlayState* play) {
    MessageContext* msgCtx = &play->msgCtx;

    if (msgCtx->stickAdjY <= -30) {
        msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2]--;
        if (msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2] < 0x30) {
            msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2] = 0x39;
        }
        Font_LoadCharNES(play, msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2],
                         msgCtx->unk120C4 + (msgCtx->unk120C2 << 7));
        play_sound(NA_SE_SY_RUPY_COUNT);
    } else if (msgCtx->stickAdjY >= 30) {
        msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2]++;
        if (msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2] >= 0x3A) {
            msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2] = 0x30;
        }
        Font_LoadCharNES(play, msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2],
                         msgCtx->unk120C4 + (msgCtx->unk120C2 << 7));
        play_sound(NA_SE_SY_RUPY_COUNT);
    }

    msgCtx->bankRupeesSelected = (msgCtx->decodedBuffer.schar[msgCtx->unk120C0] * 10) - 0x1E0U;
}

// Matching, in-function data
#ifdef NON_MATCHING
void func_801491DC(PlayState* play) {
    static s16 D_801CFD88 = false; // sAnalogStickHeld
    MessageContext* msgCtx = &play->msgCtx;

    if (msgCtx->stickAdjY <= -30) {
        msgCtx->unk12054[msgCtx->unk120C2]--;
        if (msgCtx->unk12054[msgCtx->unk120C2] <= 0) {
            msgCtx->unk12054[msgCtx->unk120C2] = 5;
        }
        msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2] = msgCtx->unk12054[msgCtx->unk120C2] + 0x30;
        Font_LoadCharNES(play, msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2],
                         msgCtx->unk120C4 + (msgCtx->unk120C2 << 7));
        play_sound(NA_SE_SY_RUPY_COUNT);
    } else if (msgCtx->stickAdjY >= 30) {
        msgCtx->unk12054[msgCtx->unk120C2]++;
        if (msgCtx->unk12054[msgCtx->unk120C2] > 5) {
            msgCtx->unk12054[msgCtx->unk120C2] = 1;
        }
        msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2] = msgCtx->unk12054[msgCtx->unk120C2] + 0x30;
        Font_LoadCharNES(play, msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2],
                         msgCtx->unk120C4 + (msgCtx->unk120C2 << 7));
        play_sound(NA_SE_SY_RUPY_COUNT);
    } else if ((msgCtx->stickAdjX >= 30) && !D_801CFD88) {
        D_801CFD88 = true;
        msgCtx->unk120C2++;
        if (msgCtx->unk120C2 > 4) {
            msgCtx->unk120C2 = 4;
        } else {
            play_sound(NA_SE_SY_CURSOR);
        }
    } else if ((msgCtx->stickAdjX <= -30) && !D_801CFD88) {
        D_801CFD88 = true;
        msgCtx->unk120C2--;
        if (msgCtx->unk120C2 < 0) {
            msgCtx->unk120C2 = 0;
        } else {
            play_sound(NA_SE_SY_CURSOR);
        }
    } else {
        D_801CFD88 = false;
    }
}
#else
void func_801491DC(PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/code/z_message/func_801491DC.s")
#endif

// Matching, in-function data
#ifdef NON_MATCHING
void func_80149454(PlayState* play) {
    static s16 D_801CFD8C = false; // sAnalogStickHeld
    MessageContext* msgCtx = &play->msgCtx;

    if (msgCtx->stickAdjY <= -30) {
        msgCtx->unk12054[msgCtx->unk120C2]--;
        if (msgCtx->unk12054[msgCtx->unk120C2] < 0) {
            msgCtx->unk12054[msgCtx->unk120C2] = 9;
        }
        msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2] = msgCtx->unk12054[msgCtx->unk120C2] + 0x30;
        Font_LoadCharNES(play, msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2],
                         msgCtx->unk120C4 + (msgCtx->unk120C2 << 7));
        play_sound(NA_SE_SY_RUPY_COUNT);
    } else if (msgCtx->stickAdjY >= 30) {
        msgCtx->unk12054[msgCtx->unk120C2]++;
        if (msgCtx->unk12054[msgCtx->unk120C2] > 9) {
            msgCtx->unk12054[msgCtx->unk120C2] = 0;
        }
        msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2] = msgCtx->unk12054[msgCtx->unk120C2] + 0x30;
        Font_LoadCharNES(play, msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2],
                         msgCtx->unk120C4 + (msgCtx->unk120C2 << 7));
        play_sound(NA_SE_SY_RUPY_COUNT);
    } else if ((msgCtx->stickAdjX >= 30) && !D_801CFD8C) {
        D_801CFD8C = true;
        msgCtx->unk120C2++;
        if (msgCtx->unk120C2 > 2) {
            msgCtx->unk120C2 = 2;
        } else {
            play_sound(NA_SE_SY_CURSOR);
        }
    } else if ((msgCtx->stickAdjX <= -30) && !D_801CFD8C) {
        D_801CFD8C = true;
        msgCtx->unk120C2--;
        if (msgCtx->unk120C2 < 0) {
            msgCtx->unk120C2 = 0;
        } else {
            play_sound(NA_SE_SY_CURSOR);
        }
    } else {
        D_801CFD8C = false;
    }
}
#else
void func_80149454(PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/code/z_message/func_80149454.s")
#endif

// Matching, in-function data
#ifdef NON_MATCHING
void func_801496C8(PlayState* play) {
    static s16 D_801CFD90 = false; // sAnalogStickHeld
    MessageContext* msgCtx = &play->msgCtx;

    if (play->msgCtx.stickAdjY <= -30) {
        msgCtx->unk12054[msgCtx->unk120C2]--;
        if (msgCtx->unk12054[msgCtx->unk120C2] < 0) {
            msgCtx->unk12054[msgCtx->unk120C2] = 3;
        }
        msgCtx->decodedBuffer.wchar[msgCtx->unk120C0 + msgCtx->unk120C2] = msgCtx->unk12054[msgCtx->unk120C2] + 0x824F;
        Font_LoadChar(play, msgCtx->decodedBuffer.wchar[msgCtx->unk120C0 + msgCtx->unk120C2],
                      msgCtx->unk120C4 + (msgCtx->unk120C2 << 7));
        play_sound(NA_SE_SY_RUPY_COUNT);
    } else if (msgCtx->stickAdjY >= 30) {
        msgCtx->unk12054[msgCtx->unk120C2]++;
        if (msgCtx->unk12054[msgCtx->unk120C2] >= 4) {
            msgCtx->unk12054[msgCtx->unk120C2] = 0;
        }
        msgCtx->decodedBuffer.wchar[msgCtx->unk120C0 + msgCtx->unk120C2] = msgCtx->unk12054[msgCtx->unk120C2] + 0x824F;
        Font_LoadChar(play, msgCtx->decodedBuffer.wchar[msgCtx->unk120C0 + msgCtx->unk120C2],
                      msgCtx->unk120C4 + (msgCtx->unk120C2 << 7));
        play_sound(NA_SE_SY_RUPY_COUNT);
    } else if ((msgCtx->stickAdjX >= 30) && !D_801CFD90) {
        D_801CFD90 = true;
        msgCtx->unk120C2++;
        if (msgCtx->unk120C2 > 5) {
            msgCtx->unk120C2 = 5;
        } else {
            play_sound(NA_SE_SY_CURSOR);
        }
    } else if ((msgCtx->stickAdjX <= -30) && !D_801CFD90) {
        D_801CFD90 = true;
        msgCtx->unk120C2--;
        if (msgCtx->unk120C2 < 0) {
            msgCtx->unk120C2 = 0;
        } else {
            play_sound(NA_SE_SY_CURSOR);
        }
    } else {
        D_801CFD90 = false;
    }
}
#else
void func_801496C8(PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/code/z_message/func_801496C8.s")
#endif

void Message_DrawTextChar(PlayState* play, TexturePtr arg1, Gfx** gfxp) {
    MessageContext* msgCtx = &play->msgCtx;
    Gfx* gfx = *gfxp;
    s16 x = msgCtx->textPosX;
    s16 y = msgCtx->textPosY;

    gDPPipeSync(gfx++);

    gDPLoadTextureBlock_4b(gfx++, arg1, G_IM_FMT_I, 16, 16, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP,
                           G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

    if ((msgCtx->textBoxType != 5) && (msgCtx->textBoxType != 0xD) && !play->pauseCtx.bombersNotebookOpen) {
        gDPSetPrimColor(gfx++, 0, 0, 0, 0, 0, msgCtx->textColorAlpha);
        gSPTextureRectangle(gfx++, (x + 1) << 2, (y + 1) << 2, ((x + sCharTexSize + 1)) << 2,
                            ((y + sCharTexSize + 1)) << 2, G_TX_RENDERTILE, 0, 0, sCharTexScale, sCharTexScale);
        gDPPipeSync(gfx++);
    }

    gDPSetPrimColor(gfx++, 0, 0, msgCtx->unk12018, msgCtx->unk1201A, msgCtx->unk1201C, msgCtx->textColorAlpha);
    gSPTextureRectangle(gfx++, (x) << 2, (y) << 2, ((x + sCharTexSize)) << 2, ((y + sCharTexSize)) << 2,
                        G_TX_RENDERTILE, 0, 0, sCharTexScale, sCharTexScale);
    *gfxp = gfx++;
}

// resizes textboxes when opening them
void func_80149C18(PlayState* play) {
    MessageContext* msgCtx = &play->msgCtx;

    if (!play->pauseCtx.bombersNotebookOpen) {
        sTextboxWidth = D_801CFDA4[msgCtx->stateTimer] * 256.0f;
        sTextboxTexWidth = 1024.0f / D_801CFDA4[msgCtx->stateTimer];
        sTextboxHeight = D_801CFDC8[msgCtx->stateTimer] * 64.0f;
        sTextboxTexHeight = 1024.0f / D_801CFDC8[msgCtx->stateTimer];
        msgCtx->unk1206A = msgCtx->unk12006 + ((0x40 - sTextboxHeight) / 2);
        msgCtx->unk1203C += msgCtx->unk1203A / 8;
        msgCtx->stateTimer++;
    } else {
        msgCtx->stateTimer = 8;
        sTextboxWidth = 0x200;
        sTextboxTexWidth = 0x200;
        sTextboxHeight = 0x59;
        sTextboxTexHeight = 0x2DB;
        msgCtx->unk1206A = msgCtx->unk12006 + ((0x5A - sTextboxHeight) / 2);
    }

    if (msgCtx->stateTimer == 8) {
        // Reached the end
        msgCtx->unk12068 = msgCtx->unk12004;
        msgCtx->unk1206A = msgCtx->unk12006;
        msgCtx->msgMode = MSGMODE_TEXT_STARTING;
        msgCtx->unk1203C = msgCtx->unk1203A;
    }

    if (!play->pauseCtx.bombersNotebookOpen) {
        msgCtx->unk12068 = msgCtx->unk12004 + ((0x100 - sTextboxWidth) / 2);
        return;
    }

    msgCtx->unk12068 = msgCtx->unk12004 + ((0x200 - sTextboxWidth) / 2);
}

void Message_FindMessage(PlayState* play, u16 textId) {
    MessageContext* msgCtx = &play->msgCtx;
    Font* font = &msgCtx->font;
    MessageTableEntry* msgEntry = msgCtx->messageEntryTable;
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

    msgEntry = msgCtx->messageEntryTable;
    foundSegment = msgEntry->segment;
    msgEntry++;
    nextSegment = msgEntry->segment;
    font->messageStart = foundSegment - segment;
    font->messageEnd = nextSegment - foundSegment;
}

extern u8 D_801CFDEC[][3];
extern s16 D_801CFE04[];
extern s16 D_801CFE1C[];
extern s16 D_801CFE34[];
extern TexturePtr D_801CFE4C[];
extern u8 D_801CFE5C[][3];
extern u8 D_801CFE68[][3];

void func_80149F74(PlayState* play, Gfx** gfxP) {
    MessageContext* msgCtx;
    Gfx* gfx;
    s32 index;

    msgCtx = &play->msgCtx;
    gfx = *gfxP;
    msgCtx->unk12016 = msgCtx->unk12014;

    if (msgCtx->msgMode == MSGMODE_TEXT_DISPLAYING) {
        play_sound(0);
    }

    gDPPipeSync(gfx++);
    gDPSetCombineMode(gfx++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    gDPSetPrimColor(gfx++, 0, 0, 255, 255, 255, msgCtx->textColorAlpha);

    if (msgCtx->itemId == ITEM_RECOVERY_HEART) {
        gDPPipeSync(gfx++);
        gDPSetPrimColor(gfx++, 0, 0, 255, 0, 0, msgCtx->textColorAlpha);
        gDPSetEnvColor(gfx++, 0, 80, 0, 255);
        gDPLoadTextureBlock(gfx++, gHeartFullTex, G_IM_FMT_IA, G_IM_SIZ_8b, 16, 16, 0, G_TX_NOMIRROR | G_TX_WRAP,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    } else if ((msgCtx->itemId >= ITEM_RUPEE_GREEN) && (msgCtx->itemId <= ITEM_RUPEE_HUGE)) {
        index = msgCtx->itemId - ITEM_RUPEE_GREEN;
        gDPPipeSync(gfx++);
        gDPSetPrimColor(gfx++, 0, 0, D_801CFDEC[index][0], D_801CFDEC[index][1], D_801CFDEC[index][2],
                        msgCtx->textColorAlpha);
        gDPSetEnvColor(gfx++, 0, 80, 0, 255);
        gDPLoadTextureBlock(gfx++, gRupeeCounterIconTex, G_IM_FMT_IA, G_IM_SIZ_8b, 16, 16, 0, G_TX_NOMIRROR | G_TX_WRAP,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    } else if (msgCtx->itemId == ITEM_STRAY_FAIRIES) {
        msgCtx->unk12016 = 0x18;
        gDPPipeSync(gfx++);
        gDPSetPrimColor(gfx++, 0, 0, D_801CFE5C[((void)0, gSaveContext.dungeonIndex)][0],
                        D_801CFE5C[((void)0, gSaveContext.dungeonIndex)][1],
                        D_801CFE5C[((void)0, gSaveContext.dungeonIndex)][2], msgCtx->textColorAlpha);
        gDPSetEnvColor(gfx++, D_801CFE68[((void)0, gSaveContext.dungeonIndex)][0],
                       D_801CFE68[((void)0, gSaveContext.dungeonIndex)][1],
                       D_801CFE68[((void)0, gSaveContext.dungeonIndex)][2], 0);
        gDPLoadTextureBlock_4b(gfx++, gStrayFairyGlowingCircleIconTex, G_IM_FMT_I, 32, 24, 0, G_TX_NOMIRROR | G_TX_WRAP,
                               G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
        gSPTextureRectangle(gfx++, msgCtx->unk12010 << 2, msgCtx->unk12012 << 2,
                            (msgCtx->unk12010 + msgCtx->unk12014) << 2, (msgCtx->unk12012 + msgCtx->unk12016) << 2,
                            G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);
        gDPSetPrimColor(gfx++, 0, 0, 255, 255, 255, msgCtx->textColorAlpha);
        gDPLoadTextureBlock(gfx++, D_801CFE4C[((void)0, gSaveContext.dungeonIndex)], G_IM_FMT_RGBA, G_IM_SIZ_32b, 32,
                            24, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
    } else if ((msgCtx->itemId >= ITEM_SONG_SONATA) && (msgCtx->itemId <= ITEM_SONG_SUN)) {
        index = msgCtx->itemId - ITEM_SONG_SONATA;
        gDPSetPrimColor(gfx++, 0, 0, D_801CFE04[index], D_801CFE1C[index], D_801CFE34[index], msgCtx->textColorAlpha);
        gDPLoadTextureBlock(gfx++, msgCtx->textboxSegment + 0x1000, G_IM_FMT_IA, G_IM_SIZ_8b, 16, 24, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
        msgCtx->unk12016 = 0x18;
    } else if (msgCtx->itemId == 0xCC) {
        gDPPipeSync(gfx++);
        gDPSetPrimColor(gfx++, 0, 0, 255, 255, 0, msgCtx->textColorAlpha);
        gDPSetEnvColor(gfx++, 0, 0, 0, 255);
        gDPLoadTextureBlock(gfx++, msgCtx->textboxSegment + 0x1000, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
    } else if (msgCtx->itemId >= 0xB8) {
        gDPLoadTextureBlock(gfx++, msgCtx->textboxSegment + 0x1000, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
    } else if (msgCtx->itemId >= ITEM_SKULL_TOKEN) {
        gDPLoadTextureBlock(gfx++, msgCtx->textboxSegment + 0x1000, G_IM_FMT_RGBA, G_IM_SIZ_32b, 24, 24, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
    } else {
        msgCtx->unk12016 = msgCtx->unk12014;
        gDPLoadTextureBlock(gfx++, msgCtx->textboxSegment + 0x1000, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
    }

    gSPTextureRectangle(gfx++, msgCtx->unk12010 << 2, msgCtx->unk12012 << 2, (msgCtx->unk12010 + msgCtx->unk12014) << 2,
                        (msgCtx->unk12012 + msgCtx->unk12016) << 2, G_TX_RENDERTILE, 0, 0, D_801F6B08, D_801F6B08);
    gDPPipeSync(gfx++);
    gDPSetCombineLERP(gfx++, 0, 0, 0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0);

    if (!play->pauseCtx.bombersNotebookOpen) {
        msgCtx->textPosX += 0x10;
    } else {
        msgCtx->textPosX += 0x32;
    }
    *gfxP = gfx;
}

void Message_HandleOcarina(PlayState* play) {
    MessageContext* msgCtx = &play->msgCtx;

    if (msgCtx->msgMode == MSGMODE_TEXT_DISPLAYING) {
        play->msgCtx.ocarinaMode = OCARINA_MODE_ACTIVE;
        if (msgCtx->ocarinaAction == OCARINA_ACTION_SCARECROW_LONG_RECORDING) {
            msgCtx->msgMode = MSGMODE_SCARECROW_LONG_RECORDING_START;
        } else if (msgCtx->ocarinaAction == OCARINA_ACTION_SCARECROW_LONG_DEMONSTRATION) {
            AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_DEFAULT);
            AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_DEFAULT);
            msgCtx->ocarinaStaff = AudioOcarina_GetPlaybackStaff();
            msgCtx->ocarinaStaff->pos = 0;
            sOcarinaButtonIndexBufPos = sOcarinaButtonIndexBufLen = 0;
            Message_ResetOcarinaButtonState(play);
            msgCtx->stateTimer = 3;
            msgCtx->msgMode = MSGMODE_SCARECROW_LONG_DEMONSTRATION;
            AudioOcarina_SetPlaybackSong(OCARINA_SONG_SCARECROW_LONG + 1, 1);
        } else if (msgCtx->ocarinaAction == OCARINA_ACTION_SCARECROW_SPAWN_RECORDING) {
            msgCtx->msgMode = MSGMODE_SCARECROW_SPAWN_RECORDING_START;
        } else if (msgCtx->ocarinaAction == OCARINA_ACTION_SCARECROW_SPAWN_DEMONSTRATION) {
            AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_DEFAULT);
            AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_DEFAULT);
            msgCtx->ocarinaStaff = AudioOcarina_GetPlaybackStaff();
            msgCtx->ocarinaStaff->pos = 0;
            sOcarinaButtonIndexBufPos = sOcarinaButtonIndexBufLen = 0;
            Message_ResetOcarinaButtonState(play);
            msgCtx->stateTimer = 3;
            msgCtx->msgMode = MSGMODE_SCARECROW_SPAWN_DEMONSTRATION;
            AudioOcarina_SetPlaybackSong(OCARINA_SONG_SCARECROW_SPAWN + 1, 1);
        } else if (msgCtx->ocarinaAction == OCARINA_ACTION_37) {
            msgCtx->msgMode = MSGMODE_2F;
        } else if (msgCtx->ocarinaAction == OCARINA_ACTION_3A) {
            msgCtx->msgMode = MSGMODE_32;
        } else if ((msgCtx->ocarinaAction >= OCARINA_ACTION_TIMED_PROMPT_SONATA) &&
                   (msgCtx->ocarinaAction <= OCARINA_ACTION_TIMED_PROMPT_STORMS)) {
            msgCtx->msgMode = MSGMODE_35;
        } else if (((msgCtx->ocarinaAction >= OCARINA_ACTION_DEMONSTRATE_SONATA) &&
                    (msgCtx->ocarinaAction <= OCARINA_ACTION_DEMONSTRATE_GORON_LULLABY_INTRO)) ||
                   (msgCtx->ocarinaAction == OCARINA_ACTION_3B) || (msgCtx->ocarinaAction == OCARINA_ACTION_3C) ||
                   (msgCtx->ocarinaAction == OCARINA_ACTION_DEMONSTRATE_EVAN_PART1_FIRST_HALF) ||
                   (msgCtx->ocarinaAction == OCARINA_ACTION_DEMONSTRATE_EVAN_PART2_FIRST_HALF) ||
                   (msgCtx->ocarinaAction == OCARINA_ACTION_DEMONSTRATE_EVAN_PART1_SECOND_HALF) ||
                   (msgCtx->ocarinaAction == OCARINA_ACTION_DEMONSTRATE_EVAN_PART2_SECOND_HALF)) {
            msgCtx->msgMode = MSGMODE_SONG_DEMONSTRATION_STARTING;
        } else if ((msgCtx->ocarinaAction == OCARINA_ACTION_PROMPT_EVAN_PART1_SECOND_HALF) ||
                   (msgCtx->ocarinaAction == OCARINA_ACTION_PROMPT_EVAN_PART2_SECOND_HALF) ||
                   (msgCtx->ocarinaAction == OCARINA_ACTION_PROMPT_WIND_FISH_HUMAN) ||
                   (msgCtx->ocarinaAction == OCARINA_ACTION_PROMPT_WIND_FISH_GORON) ||
                   (msgCtx->ocarinaAction == OCARINA_ACTION_PROMPT_WIND_FISH_ZORA) ||
                   (msgCtx->ocarinaAction == OCARINA_ACTION_PROMPT_WIND_FISH_DEKU)) {
            msgCtx->msgMode = MSGMODE_SONG_PROMPT_STARTING;
        } else if ((msgCtx->ocarinaAction == OCARINA_ACTION_0) || (msgCtx->ocarinaAction == OCARINA_ACTION_FREE_PLAY) ||
                   (msgCtx->ocarinaAction >= OCARINA_ACTION_CHECK_TIME)) {
            msgCtx->msgMode = MSGMODE_OCARINA_STARTING;
        } else {
            msgCtx->msgMode = MSGMODE_SONG_PROMPT_STARTING;
        }
    }
}

void func_8014ADBC(PlayState* play, Gfx** gfxP);
#pragma GLOBAL_ASM("asm/non_matchings/code/z_message/func_8014ADBC.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_message/func_8014C70C.s")

void Message_LoadChar(PlayState* play, u16 codePointIndex, s32* offset, f32* arg3, s16 decodedBufPos) {
    MessageContext* msgCtx = &play->msgCtx;
    s32 temp1 = *offset;
    f32 temp2 = *arg3;

    Font_LoadChar(play, codePointIndex, temp1);
    msgCtx->decodedBuffer.wchar[decodedBufPos] = codePointIndex;
    temp1 += FONT_CHAR_TEX_SIZE;
    temp2 += (16.0f * msgCtx->textCharScale);
    *offset = temp1;
    *arg3 = temp2;
}

// Message_LoadRupees JPN ?
void func_8014CCB4(PlayState* play, s16* decodedBufPos, s32* offset, f32* arg3) {
    MessageContext* msgCtx = &play->msgCtx;
    s16 t = *decodedBufPos;
    s32 k = *offset;
    f32 f = *arg3;

    Font_LoadChar(play, 0x838B, k); // 0x838B = ル in JISX0213
    k += FONT_CHAR_TEX_SIZE;
    msgCtx->decodedBuffer.wchar[t] = 0x838B;
    t++;
    Font_LoadChar(play, 0x8373, k); // 0x8373 = ピ in JISX0213
    k += FONT_CHAR_TEX_SIZE;
    msgCtx->decodedBuffer.wchar[t] = 0x8373;
    t++;
    Font_LoadChar(play, 0x815C, k); // Ox815C = ― in JISX0213
    k += FONT_CHAR_TEX_SIZE;
    msgCtx->decodedBuffer.wchar[t] = 0x815C;

    f += 16.0f * msgCtx->textCharScale * 3.0f;
    *decodedBufPos = t;
    *offset = k;
    *arg3 = f;
}

#pragma GLOBAL_ASM("asm/non_matchings/code/z_message/func_8014CDF0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_message/func_8014CFDC.s")

// Message_LoadTimeBeforeMoonCrash
void func_8014D304(PlayState* play, u16 arg1, s32* offset, f32* arg3, s16* decodedBufPos) {
    MessageContext* msgCtx = &play->msgCtx;
    s16 i;
    s16 p = *decodedBufPos;
    s32 o = *offset;
    f32 f = *arg3;
    u32 dayTime;
    s16 digits[4];
    s32 day;
    f32 timeInMinutes;

    if (arg1 == 0x20F) {
        day = gSaveContext.save.day;
        dayTime = 0x40000 - ((day % 5) << 16) - (u16)(-0x4000 + gSaveContext.save.time);
    } else {
        dayTime = 0x10000 - (u16)(-0x4000 + gSaveContext.save.time);
    }
    timeInMinutes = TIME_TO_MINUTES_F(dayTime);

    digits[0] = 0;
    digits[1] = (timeInMinutes / 60.0f);
    while (digits[1] >= 10) {
        digits[0]++;
        digits[1] -= 10;
    }

    digits[2] = 0;
    digits[3] = (s32)timeInMinutes % 60;
    while (digits[3] >= 10) {
        digits[2]++;
        digits[3] -= 10;
    }

    for (i = 0; i < 4; i++) {
        Font_LoadChar(play, digits[i] + 0x824F, o); // 0x824F = ０ in S-JIS
        o += FONT_CHAR_TEX_SIZE;
        msgCtx->decodedBuffer.wchar[p] = digits[i] + 0x824F;
        p++;
        if (i == 1) {
            // Hours (時間)
            Font_LoadChar(play, 0x8E9E, o);
            o += FONT_CHAR_TEX_SIZE;
            msgCtx->decodedBuffer.wchar[p] = 0x8E9E;
            p++;
            Font_LoadChar(play, 0x8AD4, o);
            o += FONT_CHAR_TEX_SIZE;
            msgCtx->decodedBuffer.wchar[p] = 0x8E9E;
            p++;
        } else if (i == 3) {
            // Minutes (分)
            Font_LoadChar(play, 0x95AA, o);
            o += FONT_CHAR_TEX_SIZE;
            msgCtx->decodedBuffer.wchar[p] = 0x95AA;
        }
    }
    f += 7.0f * (16.0f * msgCtx->textCharScale);
    *decodedBufPos = p;
    *offset = o;
    *arg3 = f;
}

// JPN Area names
extern u16 D_801D0188[OWL_WARP_MAX][9];
// Area Names lengths
extern s16 D_801D0250[OWL_WARP_MAX];

void func_8014D62C(PlayState* play, s32* offset, f32* arg2, s16* decodedBufPos) {
    MessageContext* msgCtx = &play->msgCtx;
    PauseContext* pauseCtx = &play->pauseCtx;
    s16 stringLimit;
    s16 temp_s1 = *decodedBufPos;
    s32 temp_s2 = *offset;
    f32 sp3C = *arg2;
    s16 owlWarpId;
    s16 phi_s0;

    if (func_8010A0A4(play) || (play->sceneId == SCENE_SECOM)) {
        owlWarpId = OWL_WARP_ENTRANCE;
    } else {
        owlWarpId = pauseCtx->cursorPoint[PAUSE_WORLD_MAP];
    }
    stringLimit = D_801D0250[owlWarpId];

    for (phi_s0 = 0; phi_s0 < stringLimit; phi_s0++, temp_s1++, temp_s2 += 0x80) {
        msgCtx->decodedBuffer.wchar[temp_s1] = D_801D0188[owlWarpId][phi_s0];
        Font_LoadChar(play, D_801D0188[owlWarpId][phi_s0], temp_s2);
    }

    temp_s1--;
    sp3C += (stringLimit - 1) * (16.0f * msgCtx->textCharScale);

    *decodedBufPos = temp_s1;
    *offset = temp_s2;
    *arg2 = sp3C;
}

void Message_Decode(PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/code/z_message/Message_Decode.s")

void func_80150A84(PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/code/z_message/func_80150A84.s")

void Message_OpenText(PlayState* play, u16 textId) {
    MessageContext* msgCtx = &play->msgCtx;
    Font* font = &msgCtx->font;
    Player* player = GET_PLAYER(play);
    f32 var_fv0;

    if (play->msgCtx.msgMode == MSGMODE_NONE) {
        gSaveContext.prevHudVisibility = gSaveContext.hudVisibility;
    }

    if (textId == 0xFF) {
        func_80115844(play, 0x12);
        play->msgCtx.unk_120BC = gSaveContext.hudVisibility;
        Interface_SetHudVisibility(HUD_VISIBILITY_A_B_C);
        gSaveContext.save.unk_06 = 0x14;
    } else if ((textId == 0x579) || (textId == 0x8D8)) {
        Interface_SetHudVisibility(HUD_VISIBILITY_A_HEARTS_MAGIC_WITH_OVERWRITE);
    } else if (((textId == 0x28) || (textId == 0x29) || (textId == 0x2A)) && (player->transformation == 3)) {
        //! FAKE:
        if (msgCtx) {}
        textId = 0xC9;
    } else if (textId == 0x11) {
        if (gSaveContext.save.inventory.strayFairies[((void)0, gSaveContext.dungeonIndex)] == 0xF) {
            textId = 0xF3;
        }
    } else if ((textId == 0x92) && (play->sceneId == SCENE_KOEPONARACE)) {
        textId = 0xCD;
    }

    if ((textId == 0xC) &&
        (((((void)0, gSaveContext.save.inventory.questItems) & 0xF0000000) >> QUEST_HEART_PIECE_COUNT) != 0)) {
        textId = ((((void)0, gSaveContext.save.inventory.questItems) & 0xF0000000) >> QUEST_HEART_PIECE_COUNT);
        textId += 0xC4;
    }

    msgCtx->currentTextId = textId;

    if ((msgCtx->currentTextId == 0xC) || (msgCtx->currentTextId == 0xD) || (msgCtx->currentTextId == 0xC5) ||
        (msgCtx->currentTextId == 0xC6) || (msgCtx->currentTextId == 0xC7)) {
        Interface_SetHudVisibility(HUD_VISIBILITY_A_HEARTS_MAGIC_WITH_OVERWRITE);
    }

    msgCtx->unk12092 = 0;
    D_801C6A70 = 0;
    msgCtx->unk12094 = 0;
    msgCtx->textIsCredits = false;
    var_fv0 = 1.0f;

    if (play->pauseCtx.bombersNotebookOpen) {
        if (gSaveContext.options.language == 0) {
            msgCtx->textCharScale = 1.4f;
            msgCtx->unk11FFC = 0x1E;
            msgCtx->unk11FF8 = 0x32;
            var_fv0 = 1.4;
        } else {
            msgCtx->textCharScale = 1.4f;
            msgCtx->unk11FFC = 0x16;
            msgCtx->unk11FF8 = 0x32;
            var_fv0 = 1.4;
        }
    } else if (textId >= 0x4E20) {
        msgCtx->textIsCredits = true;
        msgCtx->textCharScale = 0.85f;
        msgCtx->unk11FFC = 6;
        msgCtx->unk11FF8 = 0x14;
    } else if (gSaveContext.options.language == 0) {
        msgCtx->textCharScale = 0.88f;
        msgCtx->unk11FFC = 0x12;
        msgCtx->unk11FF8 = 0x32;
    } else {
        msgCtx->textCharScale = 0.75f;
        msgCtx->unk11FFC = 0xC;
        msgCtx->unk11FF8 = 0x41;
    }

    sCharTexSize = msgCtx->textCharScale * 16.0f;
    sCharTexScale = 1024.0f / msgCtx->textCharScale;
    D_801F6B08 = 1024.0f / var_fv0;

    if (msgCtx->textIsCredits) {
        Message_FindCreditsMessage(play, textId);
        msgCtx->msgLength = font->messageEnd;
        DmaMgr_SendRequest0(&font->msgBuf, &_staff_message_data_staticSegmentRomStart[font->messageStart],
                            font->messageEnd);
    } else if (gSaveContext.options.language == 0) {
        Message_FindMessage(play, textId);
        msgCtx->msgLength = font->messageEnd;
        DmaMgr_SendRequest0(&font->msgBuf, &_message_data_staticSegmentRomStart[font->messageStart], font->messageEnd);
    } else {
        Message_FindMessageNES(play, textId);
        msgCtx->msgLength = font->messageEnd;
        DmaMgr_SendRequest0(&font->msgBuf, &_message_data_staticSegmentRomStart[font->messageStart], font->messageEnd);
    }

    msgCtx->unk12022 = 0;
    msgCtx->textUnskippable = 0;
    msgCtx->textboxEndType = 0;
    msgCtx->textDrawPos = 0;
    msgCtx->msgBufPos = 0;
    msgCtx->decodedTextLen = 0;

    msgCtx->unk11F08 = font->msgBuf.schar[msgCtx->msgBufPos] << 8;
    msgCtx->unk11F08 |= font->msgBuf.schar[msgCtx->msgBufPos + 1];

    msgCtx->unk11F18 = (msgCtx->unk11F08 & 0xF000) >> 0xC;
    msgCtx->textBoxType = (msgCtx->unk11F08 & 0xF00) >> 8;
    msgCtx->textBoxPos = (msgCtx->unk11F08 & 0xF0) >> 4;
    msgCtx->unk11F0C = msgCtx->unk11F08 & 0xF;

    if ((msgCtx->unk11F0C == 1) || (msgCtx->unk11F0C == 3)) {
        msgCtx->textUnskippable = 1;
    }
    msgCtx->itemId = 0xFE;

    if ((msgCtx->textBoxType == 5) || (msgCtx->textBoxType == 0xD) || (play->pauseCtx.bombersNotebookOpen)) {
        msgCtx->unk120CE = msgCtx->unk120D0 = msgCtx->unk120D2 = 0;
    } else {
        msgCtx->unk120CE = msgCtx->unk120D0 = msgCtx->unk120D2 = 0xFF;
    }

    msgCtx->choiceIndex = 0;

    if (msgCtx->unk11F0C != 3) {
        msgCtx->textColorAlpha = 0xFF;
    } else {
        msgCtx->textColorAlpha = 0;
    }

    if (textId == 0x1467) {
        func_80151BB4(play, 2);
    }

    if (textId == 0x2955) {
        func_80151BB4(play, 1);
        func_80151BB4(play, 2);
        func_80151BB4(play, 0x32);
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/code/z_message/func_801514B0.s")

void Message_StartTextbox(PlayState* play, u16 textId, Actor* Actor) {
    MessageContext* msgCtx = &play->msgCtx;

    msgCtx->ocarinaAction = 0xFFFF;
    Message_OpenText(play, textId);
    msgCtx->talkActor = Actor;
    msgCtx->msgMode = MSGMODE_TEXT_START;
    msgCtx->stateTimer = 0;
    msgCtx->unk12024 = 0;
    play->msgCtx.ocarinaMode = OCARINA_MODE_NONE;
}

void func_80151938(PlayState* play, u16 textId) {
    MessageContext* msgCtx = &play->msgCtx;
    InterfaceContext* interfaceCtx = &play->interfaceCtx;

    msgCtx->msgLength = 0;
    Message_OpenText(play, textId);
    func_80150A84(play);
    msgCtx->msgMode = MSGMODE_TEXT_CONTINUING;
    msgCtx->stateTimer = 8;
    msgCtx->unk12024 = 0;

    if (interfaceCtx->unk_222 == 0) {
        if (textId != 0x1B93) {
            func_8011552C(play, 0x10);
        } else if (textId != 0xF8) {
            func_8011552C(play, 6);
        }
    }
    msgCtx->unk1203C = msgCtx->unk1203A;

    if (play->pauseCtx.bombersNotebookOpen) {
        msgCtx->unk12004 = 0x22;
        msgCtx->unk12006 = 0x15E;
        func_80149C18(play);
        msgCtx->stateTimer = 1;
    }
}

void Message_DisplaySceneTitleCard(PlayState* play, u16 textId) {
    MessageContext* msgCtx = &play->msgCtx;

    msgCtx->msgLength = 0;
    Message_OpenText(play, textId);
    func_80150A84(play);
    func_8015B198(play);
    msgCtx->msgMode = MSGMODE_SCENE_TITLE_CARD_FADE_IN_BACKGROUND;
    msgCtx->unk12024 = 0;
    msgCtx->unk1203C = msgCtx->unk1203A = msgCtx->textColorAlpha = 0;
    msgCtx->stateTimer = 30;

    // Day/Dawn/Night.. Messages
    if ((msgCtx->currentTextId >= 0x1BB2) && (msgCtx->currentTextId <= 0x1BB6)) {
        XREG(74) = 0x6A;
        XREG(75) = 0;
        XREG(77) = 0x58;
        XREG(76) = 0x44;
    } else {
        XREG(74) = 0x42;
        XREG(75) = 0x1E;
        XREG(77) = 0x3C;
        XREG(76) = 0x1C;
        msgCtx->unk11F1A[0] = msgCtx->unk11F1A[1] = msgCtx->unk11F1A[2] = 0;
        Interface_SetHudVisibility(HUD_VISIBILITY_NONE);
    }
}

void func_80151BB4(PlayState* play, u8 arg1) {
    MessageContext* msgCtx = &play->msgCtx;

    if (CHECK_QUEST_ITEM(QUEST_BOMBERS_NOTEBOOK)) {
        if (!CHECK_WEEKEVENTREG(D_801C6B28[arg1])) {
            msgCtx->unk120B2[msgCtx->unk120B1] = arg1;
            msgCtx->unk120B1++;
        }
    } else if (arg1 >= 20) {
        if (!CHECK_WEEKEVENTREG(D_801C6B28[arg1])) {
            msgCtx->unk120B2[msgCtx->unk120B1] = arg1;
            msgCtx->unk120B1++;
        }
    }
}

s32 func_80151C9C(PlayState* play) {
    MessageContext* msgCtx = &play->msgCtx;

    while (true) {
        if (msgCtx->unk120B1 == 0) {
            return false;
        }
        msgCtx->unk120B1--;

        if (!CHECK_WEEKEVENTREG(D_801C6B28[msgCtx->unk120B2[msgCtx->unk120B1]])) {
            SET_WEEKEVENTREG(D_801C6B28[msgCtx->unk120B2[msgCtx->unk120B1]]);

            if ((D_801C6AB8[msgCtx->unk120B2[msgCtx->unk120B1]] != 0) && CHECK_QUEST_ITEM(QUEST_BOMBERS_NOTEBOOK)) {
                func_80151938(play, D_801C6AB8[msgCtx->unk120B2[msgCtx->unk120B1]]);
                play_sound(NA_SE_SY_SCHEDULE_WRITE);
                return true;
            }
        }
    }
}

extern s16 sLastPlayedSong;
extern u16 D_801D023A[];
extern u16 D_801D028C[];
extern u16 sOcarinaSongFlagsMap[];

void Message_DisplayOcarinaStaffImpl(PlayState* play, u16 ocarinaAction) {
    MessageContext* msgCtx = &play->msgCtx;
    s16 j;
    s16 noStop;
    s32 k;
    u32 i;

    msgCtx->ocarinaAvailableSongs = 0;

    for (i = msgCtx->ocarinaAvailableSongs; i <= (QUEST_SONG_SUN - QUEST_SONG_SONATA); i++) {
        if (CHECK_QUEST_ITEM(QUEST_SONG_SONATA + i)) {
            msgCtx->ocarinaAvailableSongs = msgCtx->ocarinaAvailableSongs | sOcarinaSongFlagsMap[i];
        }
    }

    if (CHECK_QUEST_ITEM(QUEST_SONG_TIME)) {
        msgCtx->ocarinaAvailableSongs |= 0x3000;
    }
    if (CHECK_QUEST_ITEM(QUEST_SONG_LULLABY_INTRO)) {
        msgCtx->ocarinaAvailableSongs |= 0x4000;
    }
    if (CHECK_QUEST_ITEM(QUEST_SONG_LULLABY)) {
        msgCtx->ocarinaAvailableSongs &= 0xBFFF;
    }
    if (gSaveContext.save.scarecrowSpawnSongSet) {
        msgCtx->ocarinaAvailableSongs |= 0x400000;
    }
    if (gSaveContext.eventInf[3] & 2) {
        msgCtx->ocarinaAvailableSongs |= 0x800000;
    }

    msgCtx->ocarinaStaff = AudioOcarina_GetRecordingStaff();

    if ((ocarinaAction == OCARINA_ACTION_PROMPT_EVAN_PART1_SECOND_HALF) ||
        (ocarinaAction == OCARINA_ACTION_PROMPT_EVAN_PART2_SECOND_HALF)) {
        sOcarinaButtonAlphaValues[0] = sOcarinaButtonAlphaValues[1] = sOcarinaButtonAlphaValues[2] =
            sOcarinaButtonAlphaValues[3] = 255;
    } else {
        msgCtx->ocarinaStaff->pos = 0;
        sOcarinaButtonIndexBufPos = 0;
        sOcarinaButtonIndexBufLen = 0;
        Message_ResetOcarinaButtonState(play);
    }

    sLastPlayedSong = 0xFF;
    msgCtx->lastPlayedSong = 0xFF;
    msgCtx->lastOcarinaButtonIndex = 0xFF;
    noStop = false;
    msgCtx->ocarinaAction = ocarinaAction;

    if ((ocarinaAction >= OCARINA_ACTION_TIMED_PROMPT_SONATA) &&
        (ocarinaAction <= OCARINA_ACTION_TIMED_PROMPT_STORMS)) {
        Message_OpenText(play, 0x1B59);
        func_80150A84(play);
    } else if ((ocarinaAction == OCARINA_ACTION_3B) || (ocarinaAction == OCARINA_ACTION_3C)) {
        noStop = true;
        Message_OpenText(play, D_801D028C[ocarinaAction - 0x29]);
        func_80150A84(play);
    } else if ((ocarinaAction >= OCARINA_ACTION_DEMONSTRATE_EVAN_PART1_FIRST_HALF) &&
               (ocarinaAction <= OCARINA_ACTION_PROMPT_EVAN_PART2_SECOND_HALF)) {
        noStop = true;
        Message_OpenText(play, D_801D028C[ocarinaAction - 0x29]);
        func_80150A84(play);
    } else if ((ocarinaAction >= OCARINA_ACTION_PROMPT_WIND_FISH_HUMAN) &&
               (ocarinaAction <= OCARINA_ACTION_PROMPT_WIND_FISH_DEKU)) {
        noStop = true;
        Message_OpenText(play, 0x1B59);
        func_80150A84(play);
    } else if ((ocarinaAction == OCARINA_ACTION_FREE_PLAY) || (ocarinaAction >= OCARINA_ACTION_CHECK_TIME)) {
        if ((ocarinaAction >= OCARINA_ACTION_CHECK_TIME) && (ocarinaAction <= OCARINA_ACTION_CHECK_STORMS)) {
            play_sound(NA_SE_SY_TRE_BOX_APPEAR);
        }
        if (ocarinaAction == OCARINA_ACTION_SCARECROW_SPAWN_DEMONSTRATION) {
            Message_OpenText(play, 0x1B5B);
            func_80150A84(play);
        } else {
            Message_OpenText(play, 0x1B5A);
            func_80150A84(play);
        }
    } else {
        noStop = true;
        if (ocarinaAction >= OCARINA_ACTION_PROMPT_SONATA) {
            Message_OpenText(play, 0x1B59);
            func_80150A84(play);
        } else {
            Message_OpenText(play, D_801D028C[ocarinaAction]);
            func_80150A84(play);
        }
    }

    msgCtx->talkActor = NULL;
    msgCtx->unk12024 = 0;
    play->msgCtx.ocarinaMode = OCARINA_MODE_NONE;
    msgCtx->unk12004 = 0x22;
    msgCtx->unk12006 = 0x8E;
    msgCtx->stateTimer = 8;
    func_80149C18(play);
    msgCtx->unk12068 = 0x22;
    msgCtx->unk1206A = 0x8E;
    msgCtx->unk1200C = 0x200;
    msgCtx->unk1200E = 0x200;
    msgCtx->unk12008 = 0x100;
    msgCtx->unk1200A = 0x40;
    msgCtx->msgMode = MSGMODE_TEXT_CONTINUING;
    msgCtx->unk1203C = msgCtx->unk1203A;
    msgCtx->unk1203C = msgCtx->unk1203A;

    if (!noStop) {
        func_80115844(play, 0x12);
        noStop = gSaveContext.hudVisibility;
        Interface_SetHudVisibility(HUD_VISIBILITY_B_ALT);
        gSaveContext.hudVisibility = noStop;
    }

    if ((ocarinaAction == OCARINA_ACTION_FREE_PLAY) || (ocarinaAction == OCARINA_ACTION_CHECK_NOTIME)) {
        msgCtx->msgMode = MSGMODE_OCARINA_STARTING;
        msgCtx->textBoxType = 0xE;
    } else if (ocarinaAction == OCARINA_ACTION_3A) {
        msgCtx->msgMode = MSGMODE_32;
    } else if (ocarinaAction == OCARINA_ACTION_37) {
        msgCtx->msgMode = MSGMODE_2F;
        msgCtx->textBoxType = 2;
    } else if (ocarinaAction == OCARINA_ACTION_SCARECROW_LONG_DEMONSTRATION) {
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_DEFAULT);
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_DEFAULT);
        msgCtx->ocarinaStaff = AudioOcarina_GetPlaybackStaff();
        msgCtx->ocarinaStaff->pos = 0;
        sOcarinaButtonIndexBufLen = 0;
        sOcarinaButtonIndexBufPos = sOcarinaButtonIndexBufLen;
        Message_ResetOcarinaButtonState(play);
        msgCtx->msgMode = MSGMODE_SCARECROW_LONG_DEMONSTRATION;
        AudioOcarina_SetPlaybackSong(OCARINA_SONG_SCARECROW_LONG + 1, 1);
        gSaveContext.hudVisibility = 0;
        Interface_SetHudVisibility(HUD_VISIBILITY_NONE);
    }

    for (j = 0, k = 0; j < 48; j++, k += 0x80) {
        Font_LoadChar(play, 0x8140, k);
    };

    msgCtx->stateTimer = 3;
}

void Message_DisplayOcarinaStaff(PlayState* play, u16 ocarinaAction) {
    play->msgCtx.blockSunsSong = false;
    Message_DisplayOcarinaStaffImpl(play, ocarinaAction);
}

void Message_DisplayOcarinaStaffBlockSunsSong(PlayState* play, u16 ocarinaAction) {
    play->msgCtx.blockSunsSong = true;
    Message_DisplayOcarinaStaffImpl(play, ocarinaAction);
}

/**
 * @return u8 A value of the TextState enum representing the current state of the on-screen message
 */
u8 Message_GetState(MessageContext* msgCtx) {
    if (msgCtx->msgLength == 0) {
        return TEXT_STATE_NONE;
    }

    if (msgCtx->msgMode == MSGMODE_TEXT_DONE) {
        if (msgCtx->unk11F14 != 0xFFFF) {
            return TEXT_STATE_1;
        }

        if ((msgCtx->textboxEndType == TEXTBOX_ENDTYPE_10) || (msgCtx->textboxEndType == TEXTBOX_ENDTYPE_11)) {
            return TEXT_STATE_CHOICE;
        }
        if ((msgCtx->textboxEndType == TEXTBOX_ENDTYPE_40) || (msgCtx->textboxEndType == TEXTBOX_ENDTYPE_42) ||
            (msgCtx->textboxEndType == TEXTBOX_ENDTYPE_30)) {
            return TEXT_STATE_5;
        }
        if (msgCtx->textboxEndType == TEXTBOX_ENDTYPE_41) {
            return TEXT_STATE_16;
        }
        if ((msgCtx->textboxEndType >= TEXTBOX_ENDTYPE_50) && (msgCtx->textboxEndType <= TEXTBOX_ENDTYPE_57)) {
            return TEXT_STATE_3;
        }
        if ((msgCtx->textboxEndType == TEXTBOX_ENDTYPE_60) || (msgCtx->textboxEndType == TEXTBOX_ENDTYPE_61)) {
            return TEXT_STATE_14;
        }
        if (msgCtx->textboxEndType == TEXTBOX_ENDTYPE_62) {
            return TEXT_STATE_15;
        }
        if (msgCtx->textboxEndType == TEXTBOX_ENDTYPE_63) {
            return TEXT_STATE_17;
        }
        if (msgCtx->textboxEndType == TEXTBOX_ENDTYPE_12) {
            return TEXT_STATE_18;
        }
        return TEXT_STATE_DONE;
    }

    if (msgCtx->msgMode == MSGMODE_TEXT_AWAIT_NEXT) {
        return TEXT_STATE_10;
    }
    if (msgCtx->msgMode == MSGMODE_SONG_DEMONSTRATION_DONE) {
        return TEXT_STATE_7;
    }
    if ((msgCtx->ocarinaMode == OCARINA_MODE_EVENT) || (msgCtx->msgMode == MSGMODE_37)) {
        return TEXT_STATE_8;
    }
    if (msgCtx->msgMode == MSGMODE_OCARINA_AWAIT_INPUT) {
        return TEXT_STATE_9;
    }
    if ((msgCtx->msgMode == MSGMODE_21) || (msgCtx->msgMode == MSGMODE_3A)) {
        return TEXT_STATE_11;
    }
    if (msgCtx->msgMode == MSGMODE_3D) {
        return TEXT_STATE_12;
    }
    if (msgCtx->msgMode == MSGMODE_40) {
        return TEXT_STATE_13;
    }
    if ((msgCtx->msgMode == MSGMODE_TEXT_CLOSING) && (msgCtx->stateTimer == 1) && (msgCtx->unk120B1 == 0)) {
        return TEXT_STATE_CLOSING;
    }

    return TEXT_STATE_3;
}

void Message_DrawTextBox(PlayState* play, Gfx** gfxp) {
    MessageContext* msgCtx = &play->msgCtx;
    Gfx* gfx = *gfxp;

    gDPPipeSync(gfx++);

    if (((u32)msgCtx->textBoxType == 0) || (msgCtx->textBoxType == 2) || (msgCtx->textBoxType == 9) ||
        (msgCtx->textBoxType == 0xA)) {
        gDPSetRenderMode(gfx++, G_RM_CLD_SURF, G_RM_CLD_SURF2);
    } else if (msgCtx->textBoxType == 3) {
        gDPSetAlphaCompare(gfx++, G_AC_THRESHOLD);
        gDPSetRenderMode(gfx++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    }

    gDPSetPrimColor(gfx++, 0, 0, msgCtx->unk12034, msgCtx->unk12036, msgCtx->unk12038, msgCtx->unk1203C);

    if (((u32)msgCtx->textBoxType == 0) || (msgCtx->textBoxType == 2) || (msgCtx->textBoxType == 6) ||
        (msgCtx->textBoxType == 8) || (msgCtx->textBoxType == 9) || (msgCtx->textBoxType == 0xA)) {
        gDPLoadTextureBlock_4b(gfx++, msgCtx->textboxSegment, G_IM_FMT_I, 128, 64, 0, G_TX_MIRROR | G_TX_WRAP,
                               G_TX_NOMIRROR | G_TX_WRAP, 7, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    } else {
        gDPPipeSync(gfx++);

        if (msgCtx->textBoxType == 3) {
            gDPSetEnvColor(gfx++, 0, 0, 0, 255);
        } else if (msgCtx->textBoxType == 0xD) {
            gDPSetEnvColor(gfx++, 20, 0, 10, 255);
        } else {
            gDPSetEnvColor(gfx++, 50, 20, 0, 255);
        }
        gDPLoadTextureBlock_4b(gfx++, msgCtx->textboxSegment, G_IM_FMT_IA, 128, 64, 0, G_TX_MIRROR | G_TX_WRAP,
                               G_TX_MIRROR | G_TX_WRAP, 7, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    }

    if (msgCtx->textBoxType == 0xA) {
        gSPTextureRectangle(gfx++, (msgCtx->unk12068 << 2), ((msgCtx->unk1206A + 22) << 2),
                            ((msgCtx->unk12068 + msgCtx->unk12008) << 2), ((msgCtx->unk1206A + 0x36) << 2),
                            G_TX_RENDERTILE, 0, 0x0006, msgCtx->unk1200C << 1, 0x800);
    } else {
        gSPTextureRectangle(gfx++, (msgCtx->unk12068 << 2), ((msgCtx->unk1206A) << 2),
                            ((msgCtx->unk12068 + sTextboxWidth) << 2), ((msgCtx->unk1206A + sTextboxHeight) << 2),
                            G_TX_RENDERTILE, 0, 0, sTextboxTexWidth, sTextboxTexHeight);
    }

    // Draw treble clef
    if (msgCtx->textBoxType == 3) {
        gDPPipeSync(gfx++);
        gDPSetCombineLERP(gfx++, 1, 0, PRIMITIVE, 0, TEXEL0, 0, PRIMITIVE, 0, 1, 0, PRIMITIVE, 0, TEXEL0, 0, PRIMITIVE,
                          0);
        gDPSetPrimColor(gfx++, 0, 0, 255, 100, 0, 255);
        gDPLoadTextureBlock_4b(gfx++, gOcarinaTrebleClefTex, G_IM_FMT_I, 16, 32, 0, G_TX_MIRROR | G_TX_WRAP,
                               G_TX_MIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
        gSPTextureRectangle(gfx++, 0x0138, 0x0298, 0x0178, 0x0318, G_TX_RENDERTILE, 0, 0, 0x0400, 0x0400);
    }

    *gfxp = gfx++;
}

void Message_SetView(View* view) {
    SET_FULLSCREEN_VIEWPORT(view);
    View_ApplyOrthoToOverlay(view);
}

void func_80152CAC(PlayState* play) {
    MessageContext* msgCtx = &play->msgCtx;

    if (msgCtx->ocarinaAction == OCARINA_ACTION_DEMONSTRATE_SONATA) {
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_MONKEY);
    } else if (msgCtx->ocarinaAction == OCARINA_ACTION_DEMONSTRATE_HEALING) {
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_PIANO);
    } else if (msgCtx->ocarinaAction == OCARINA_ACTION_DEMONSTRATE_SARIAS) {
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_TATL);
    } else if (msgCtx->ocarinaAction == OCARINA_ACTION_DEMONSTRATE_EPONAS) {
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_FEMALE_VOICE);
    } else if (msgCtx->ocarinaAction == OCARINA_ACTION_DEMONSTRATE_STORMS) {
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_DEFAULT);
    } else if (msgCtx->ocarinaAction == OCARINA_ACTION_DEMONSTRATE_GORON_LULLABY) {
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_BABY_SINGING);
    } else if (msgCtx->ocarinaAction == OCARINA_ACTION_DEMONSTRATE_GORON_LULLABY_INTRO) {
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_ELDER_GORON_DRUMS);
    } else if (msgCtx->ocarinaAction == OCARINA_ACTION_DEMONSTRATE_OATH) {
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_TATL);
    } else if (msgCtx->ocarinaAction == OCARINA_ACTION_DEMONSTRATE_ELEGY) {
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_IKANA_KING);
    } else {
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_DEFAULT);
    }

    if ((msgCtx->ocarinaAction == OCARINA_ACTION_3B) || (msgCtx->ocarinaAction == OCARINA_ACTION_3C)) {
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_ELDER_GORON_DRUMS);
        AudioOcarina_SetPlaybackSong(OCARINA_SONG_GORON_LULLABY_INTRO + 1, 0x83);
        return;
    }
    if ((msgCtx->ocarinaAction == OCARINA_ACTION_DEMONSTRATE_EVAN_PART1_FIRST_HALF) ||
        (msgCtx->ocarinaAction == OCARINA_ACTION_DEMONSTRATE_EVAN_PART2_FIRST_HALF)) {
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_BASS_GUITAR);
        AudioOcarina_SetPlaybackSong(
            ((msgCtx->ocarinaAction - OCARINA_ACTION_DEMONSTRATE_EVAN_PART1_FIRST_HALF) + OCARINA_SONG_EVAN_PART1 + 1),
            0x84);
        return;
    }
    if ((msgCtx->ocarinaAction == OCARINA_ACTION_DEMONSTRATE_EVAN_PART1_SECOND_HALF) ||
        (msgCtx->ocarinaAction == OCARINA_ACTION_DEMONSTRATE_EVAN_PART2_SECOND_HALF)) {
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_ZORA_GUITAR);
        AudioOcarina_SetPlaybackSong(
            ((msgCtx->ocarinaAction - OCARINA_ACTION_DEMONSTRATE_EVAN_PART1_SECOND_HALF) + OCARINA_SONG_EVAN_PART1 + 1),
            0x44);
        return;
    }
    AudioOcarina_SetPlaybackSong((msgCtx->ocarinaAction - OCARINA_ACTION_FREE_PLAY), 2);
}

void Message_SpawnSongEffect(PlayState* play) {
    MessageContext* msgCtx = &play->msgCtx;
    Player* player = GET_PLAYER(play);

    if (1) {}
    if ((msgCtx->songPlayed <= OCARINA_SONG_SCARECROW_SPAWN) &&
        (msgCtx->songPlayed != OCARINA_SONG_GORON_LULLABY_INTRO) &&
        !((msgCtx->ocarinaAction >= OCARINA_ACTION_PROMPT_WIND_FISH_HUMAN) &&
          (msgCtx->ocarinaAction <= OCARINA_ACTION_PROMPT_WIND_FISH_DEKU))) {
        msgCtx->ocarinaSongEffectActive = true;
        if (msgCtx->songPlayed != OCARINA_SONG_SCARECROW_SPAWN) {
            Actor_Spawn(&play->actorCtx, play, sOcarinaEffectActorIds[msgCtx->songPlayed], player->actor.world.pos.x,
                        player->actor.world.pos.y, player->actor.world.pos.z, 0, 0, 0,
                        sOcarinaEffectActorParams[msgCtx->songPlayed]);
            return;
        }
        Actor_Spawn(&play->actorCtx, play, ACTOR_OCEFF_WIPE4, player->actor.world.pos.x, player->actor.world.pos.y,
                    player->actor.world.pos.z, 0, 0, 0, 0);
    }
}

extern s16 sOcarinaButtonStepR;
extern s16 sOcarinaButtonStepG;
extern s16 sOcarinaButtonStepB;
extern s16 sOcarinaButtonFlashTimer;
extern s16 sOcarinaButtonFlashColorIndex;
extern s16 sOcarinaButtonAPrimColors[][3];
extern s16 sOcarinaButtonAEnvColors[][3];
extern s16 sOcarinaButtonCPrimColors[][3];
extern s16 sOcarinaButtonCEnvColors[][3];

void Message_FlashOcarinaButtons(void) {
    sOcarinaButtonStepR = ABS_ALT(sOcarinaButtonAPrimR - sOcarinaButtonAPrimColors[sOcarinaButtonFlashColorIndex][0]) /
                          sOcarinaButtonFlashTimer;
    sOcarinaButtonStepG = ABS_ALT(sOcarinaButtonAPrimG - sOcarinaButtonAPrimColors[sOcarinaButtonFlashColorIndex][1]) /
                          sOcarinaButtonFlashTimer;
    sOcarinaButtonStepB = ABS_ALT(sOcarinaButtonAPrimB - sOcarinaButtonAPrimColors[sOcarinaButtonFlashColorIndex][2]) /
                          sOcarinaButtonFlashTimer;

    if (sOcarinaButtonAPrimR >= sOcarinaButtonAPrimColors[sOcarinaButtonFlashColorIndex][0]) {
        sOcarinaButtonAPrimR -= sOcarinaButtonStepR;
    } else {
        sOcarinaButtonAPrimR += sOcarinaButtonStepR;
    }
    if (sOcarinaButtonAPrimG >= sOcarinaButtonAPrimColors[sOcarinaButtonFlashColorIndex][1]) {
        sOcarinaButtonAPrimG -= sOcarinaButtonStepG;
    } else {
        sOcarinaButtonAPrimG += sOcarinaButtonStepG;
    }
    if (sOcarinaButtonAPrimB >= sOcarinaButtonAPrimColors[sOcarinaButtonFlashColorIndex][2]) {
        sOcarinaButtonAPrimB -= sOcarinaButtonStepB;
    } else {
        sOcarinaButtonAPrimB += sOcarinaButtonStepB;
    }

    sOcarinaButtonStepR = ABS_ALT(sOcarinaButtonAEnvR - sOcarinaButtonAEnvColors[sOcarinaButtonFlashColorIndex][0]) /
                          sOcarinaButtonFlashTimer;
    sOcarinaButtonStepG = ABS_ALT(sOcarinaButtonAEnvG - sOcarinaButtonAEnvColors[sOcarinaButtonFlashColorIndex][1]) /
                          sOcarinaButtonFlashTimer;
    sOcarinaButtonStepB = ABS_ALT(sOcarinaButtonAEnvB - sOcarinaButtonAEnvColors[sOcarinaButtonFlashColorIndex][2]) /
                          sOcarinaButtonFlashTimer;

    if (sOcarinaButtonCEnvR >= sOcarinaButtonAEnvColors[sOcarinaButtonFlashColorIndex][0]) {
        sOcarinaButtonAEnvR -= sOcarinaButtonStepR;
    } else {
        sOcarinaButtonAEnvR += sOcarinaButtonStepR;
    }
    if (sOcarinaButtonCEnvG >= sOcarinaButtonAEnvColors[sOcarinaButtonFlashColorIndex][1]) {
        sOcarinaButtonAEnvG -= sOcarinaButtonStepG;
    } else {
        sOcarinaButtonAEnvG += sOcarinaButtonStepG;
    }
    if (sOcarinaButtonCEnvB >= sOcarinaButtonAEnvColors[sOcarinaButtonFlashColorIndex][2]) {
        sOcarinaButtonAEnvB -= sOcarinaButtonStepB;
    } else {
        sOcarinaButtonAEnvB += sOcarinaButtonStepB;
    }

    sOcarinaButtonStepR = ABS_ALT(sOcarinaButtonCPrimR - sOcarinaButtonCPrimColors[sOcarinaButtonFlashColorIndex][0]) /
                          sOcarinaButtonFlashTimer;
    sOcarinaButtonStepG = ABS_ALT(sOcarinaButtonCPrimG - sOcarinaButtonCPrimColors[sOcarinaButtonFlashColorIndex][1]) /
                          sOcarinaButtonFlashTimer;
    sOcarinaButtonStepB = ABS_ALT(sOcarinaButtonCPrimB - sOcarinaButtonCPrimColors[sOcarinaButtonFlashColorIndex][2]) /
                          sOcarinaButtonFlashTimer;

    if (sOcarinaButtonCPrimR >= sOcarinaButtonCPrimColors[sOcarinaButtonFlashColorIndex][0]) {
        sOcarinaButtonCPrimR -= sOcarinaButtonStepR;
    } else {
        sOcarinaButtonCPrimR += sOcarinaButtonStepR;
    }
    if (sOcarinaButtonCPrimG >= sOcarinaButtonCPrimColors[sOcarinaButtonFlashColorIndex][1]) {
        sOcarinaButtonCPrimG -= sOcarinaButtonStepG;
    } else {
        sOcarinaButtonCPrimG += sOcarinaButtonStepG;
    }
    if (sOcarinaButtonCPrimB >= sOcarinaButtonCPrimColors[sOcarinaButtonFlashColorIndex][2]) {
        sOcarinaButtonCPrimB -= sOcarinaButtonStepB;
    } else {
        sOcarinaButtonCPrimB += sOcarinaButtonStepB;
    }

    sOcarinaButtonStepR = ABS_ALT(sOcarinaButtonCEnvR - sOcarinaButtonCEnvColors[sOcarinaButtonFlashColorIndex][0]) /
                          sOcarinaButtonFlashTimer;
    sOcarinaButtonStepG = ABS_ALT(sOcarinaButtonCEnvG - sOcarinaButtonCEnvColors[sOcarinaButtonFlashColorIndex][1]) /
                          sOcarinaButtonFlashTimer;
    sOcarinaButtonStepB = ABS_ALT(sOcarinaButtonCEnvB - sOcarinaButtonCEnvColors[sOcarinaButtonFlashColorIndex][2]) /
                          sOcarinaButtonFlashTimer;

    if (sOcarinaButtonCEnvR >= sOcarinaButtonCEnvColors[sOcarinaButtonFlashColorIndex][0]) {
        sOcarinaButtonCEnvR -= sOcarinaButtonStepR;
    } else {
        sOcarinaButtonCEnvR += sOcarinaButtonStepR;
    }
    if (sOcarinaButtonCEnvG >= sOcarinaButtonCEnvColors[sOcarinaButtonFlashColorIndex][1]) {
        sOcarinaButtonCEnvG -= sOcarinaButtonStepG;
    } else {
        sOcarinaButtonCEnvG += sOcarinaButtonStepG;
    }
    if (sOcarinaButtonCEnvB >= sOcarinaButtonCEnvColors[sOcarinaButtonFlashColorIndex][2]) {
        sOcarinaButtonCEnvB -= sOcarinaButtonStepB;
    } else {
        sOcarinaButtonCEnvB += sOcarinaButtonStepB;
    }

    sOcarinaButtonFlashTimer--;
    if (sOcarinaButtonFlashTimer == 0) {
        sOcarinaButtonAPrimR = sOcarinaButtonAPrimColors[sOcarinaButtonFlashColorIndex][0];
        sOcarinaButtonAPrimG = sOcarinaButtonAPrimColors[sOcarinaButtonFlashColorIndex][1];
        sOcarinaButtonAPrimB = sOcarinaButtonAPrimColors[sOcarinaButtonFlashColorIndex][2];
        sOcarinaButtonAEnvR = sOcarinaButtonAEnvColors[sOcarinaButtonFlashColorIndex][0];
        sOcarinaButtonAEnvG = sOcarinaButtonAEnvColors[sOcarinaButtonFlashColorIndex][1];
        sOcarinaButtonAEnvB = sOcarinaButtonAEnvColors[sOcarinaButtonFlashColorIndex][2];
        sOcarinaButtonCPrimR = sOcarinaButtonCPrimColors[sOcarinaButtonFlashColorIndex][0];
        sOcarinaButtonCPrimG = sOcarinaButtonCPrimColors[sOcarinaButtonFlashColorIndex][1];
        sOcarinaButtonCPrimB = sOcarinaButtonCPrimColors[sOcarinaButtonFlashColorIndex][2];
        sOcarinaButtonCEnvR = sOcarinaButtonCEnvColors[sOcarinaButtonFlashColorIndex][0];
        sOcarinaButtonCEnvG = sOcarinaButtonCEnvColors[sOcarinaButtonFlashColorIndex][1];
        sOcarinaButtonCEnvB = sOcarinaButtonCEnvColors[sOcarinaButtonFlashColorIndex][2];
        sOcarinaButtonFlashTimer = 3;
        sOcarinaButtonFlashColorIndex ^= 1;
    }
}

extern TexturePtr sOcarinaButtonTextures[];

void Message_DrawOcarinaButtons(PlayState* play, Gfx** gfxP) {
    MessageContext* msgCtx = &play->msgCtx;
    Gfx* gfx;
    u16 i;
    u16 notePosX;

    gfx = *gfxP;

    if ((play->msgCtx.msgMode >= MSGMODE_OCARINA_PLAYING) && (msgCtx->msgMode <= MSGMODE_40)) {
        if ((msgCtx->ocarinaAction != OCARINA_ACTION_FREE_PLAY) &&
            (msgCtx->ocarinaAction != OCARINA_ACTION_CHECK_NOTIME)) {
            func_8012C680(&gfx);

            gDPSetCombineLERP(gfx++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0, PRIMITIVE,
                              ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);

            if ((msgCtx->msgMode == MSGMODE_SONG_PROMPT) || (msgCtx->msgMode == MSGMODE_36)) {
                if ((msgCtx->ocarinaAction != OCARINA_ACTION_PROMPT_EVAN_PART1_SECOND_HALF) &&
                    (msgCtx->ocarinaAction != OCARINA_ACTION_PROMPT_EVAN_PART2_SECOND_HALF)) {
                    if (msgCtx->ocarinaAction >= OCARINA_ACTION_TIMED_PROMPT_SONATA) {
                        sOcarinaButtonStepG = msgCtx->ocarinaAction - OCARINA_ACTION_TIMED_PROMPT_SONATA;
                    } else if ((msgCtx->ocarinaAction == OCARINA_ACTION_PROMPT_EVAN_PART1_SECOND_HALF) ||
                               (msgCtx->ocarinaAction == OCARINA_ACTION_PROMPT_EVAN_PART2_SECOND_HALF)) {
                        // Can never pass as the conditional is opposite to one scope higher
                        sOcarinaButtonStepG = (msgCtx->ocarinaAction - OCARINA_ACTION_PROMPT_EVAN_PART1_SECOND_HALF) +
                                              OCARINA_SONG_EVAN_PART1;
                    } else if ((msgCtx->ocarinaAction >= OCARINA_ACTION_PROMPT_WIND_FISH_HUMAN) &&
                               (msgCtx->ocarinaAction <= OCARINA_ACTION_PROMPT_WIND_FISH_DEKU)) {
                        sOcarinaButtonStepG = msgCtx->ocarinaAction - OCARINA_ACTION_PROMPT_WIND_FISH_HUMAN +
                                              OCARINA_SONG_WIND_FISH_HUMAN;
                    } else {
                        sOcarinaButtonStepG = msgCtx->ocarinaAction - OCARINA_ACTION_PROMPT_SONATA;
                    }
                    sOcarinaButtonStepR = gOcarinaSongButtons[sOcarinaButtonStepG].numButtons;

                    for (notePosX = 98, i = 0; i < sOcarinaButtonStepR; notePosX += 18, i++) {
                        gDPPipeSync(gfx++);
                        gDPSetPrimColor(gfx++, 0, 0, 150, 150, 150, 150);
                        gDPSetEnvColor(gfx++, 10, 10, 10, 0);

                        gDPLoadTextureBlock(
                            gfx++, sOcarinaButtonTextures[gOcarinaSongButtons[sOcarinaButtonStepG].buttonIndex[i]],
                            G_IM_FMT_IA, G_IM_SIZ_8b, 16, 16, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP,
                            G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

                        gSPTextureRectangle(
                            gfx++, notePosX << 2,
                            msgCtx->ocarinaButtonsPosY[gOcarinaSongButtons[sOcarinaButtonStepG].buttonIndex[i]] << 2,
                            (notePosX + 16) << 2,
                            (msgCtx->ocarinaButtonsPosY[gOcarinaSongButtons[sOcarinaButtonStepG].buttonIndex[i]] + 16)
                                << 2,
                            G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);
                    }
                }
            }

            if (msgCtx->msgMode != MSGMODE_SCARECROW_LONG_RECORDING_START) {
                for (notePosX = 98, i = 0; i < 8; notePosX += 18, i++) {
                    if (sOcarinaButtonIndexBuf[i] == OCARINA_BTN_INVALID) {
                        break;
                    }

                    if (sOcarinaButtonAlphaValues[i] != 255) {
                        sOcarinaButtonAlphaValues[i] += 50;
                        if (sOcarinaButtonAlphaValues[i] >= 255) {
                            sOcarinaButtonAlphaValues[i] = 255;
                        }
                    }

                    gDPPipeSync(gfx++);
                    if (sOcarinaButtonIndexBuf[i] == OCARINA_BTN_A) {
                        gDPSetPrimColor(gfx++, 0, 0, sOcarinaButtonAPrimR, sOcarinaButtonAPrimG, sOcarinaButtonAPrimB,
                                        sOcarinaButtonAlphaValues[i]);
                        gDPSetEnvColor(gfx++, sOcarinaButtonAEnvR, sOcarinaButtonAEnvG, sOcarinaButtonAEnvB, 0);
                    } else {
                        gDPSetPrimColor(gfx++, 0, 0, sOcarinaButtonCPrimR, sOcarinaButtonCPrimG, sOcarinaButtonCPrimB,
                                        sOcarinaButtonAlphaValues[i]);
                        gDPSetEnvColor(gfx++, sOcarinaButtonCEnvR, sOcarinaButtonCEnvG, sOcarinaButtonCEnvB, 0);
                    }

                    gDPLoadTextureBlock(gfx++, sOcarinaButtonTextures[sOcarinaButtonIndexBuf[i]], G_IM_FMT_IA,
                                        G_IM_SIZ_8b, 16, 16, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP,
                                        G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

                    gSPTextureRectangle(
                        gfx++, notePosX << 2, msgCtx->ocarinaButtonsPosY[sOcarinaButtonIndexBuf[i]] << 2,
                        (notePosX + 16) << 2, (msgCtx->ocarinaButtonsPosY[sOcarinaButtonIndexBuf[i]] + 16) << 2,
                        G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);
                }
            }
        }
    }
    *gfxP = gfx;
}

void Message_DrawText(PlayState* play, Gfx** gfxP) {
    if ((gSaveContext.options.language == 0) && !play->msgCtx.textIsCredits) {
        func_8014ADBC(play, gfxP);
    } else if (play->msgCtx.textIsCredits) {
        func_8015E7EC(play, gfxP);
    } else {
        func_8015966C(play, gfxP, 0);
    }
}

void Message_DrawSceneTitleCard(PlayState* play, Gfx** gfxp) {
    MessageContext* msgCtx = &play->msgCtx;
    Gfx* gfx;

    gfx = *gfxp;
    func_8012C680(&gfx);

    gDPSetCombineLERP(gfx++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0, PRIMITIVE,
                      ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);
    gDPSetTextureFilter(gfx++, G_TF_BILERP);
    gDPSetAlphaDither(gfx++, G_AD_NOTPATTERN);
    gDPSetPrimColor(gfx++, 0, 0, 0, 0, 0, msgCtx->unk1203C);
    gDPSetEnvColor(gfx++, 140, 40, 160, 255);
    gDPLoadTextureBlock(gfx++, gSceneTitleCardGradientTex, G_IM_FMT_I, G_IM_SIZ_8b, 64, 1, 0, G_TX_NOMIRROR | G_TX_WRAP,
                        G_TX_NOMIRROR | G_TX_WRAP, 6, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    gSPTextureRectangle(gfx++, 0, XREG(77) << 2, 0x0500, (XREG(77) + XREG(76)) << 2, G_TX_RENDERTILE, 0, 0, 0x00CC,
                        0x0400);
    gDPPipeSync(gfx++);
    gDPSetCombineLERP(gfx++, 0, 0, 0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0);
    gDPSetEnvColor(gfx++, 0, 0, 0, 255);

    if ((msgCtx->currentTextId < 0x1BB2) || (msgCtx->currentTextId >= 0x1BB7)) {
        msgCtx->unk11FF8 = XREG(75);
    }
    msgCtx->unk11FFA = XREG(74);
    func_8015966C(play, &gfx, 0);
    *gfxp = gfx++;
}

void Message_DrawMain(PlayState* play, Gfx** gfxP);
#pragma GLOBAL_ASM("asm/non_matchings/code/z_message/Message_DrawMain.s")

void Message_Draw(PlayState* play) {
    Gfx* nextDisplayList;
    Gfx* polyOpa;
    GraphicsContext* gfxCtx = play->state.gfxCtx;

    OPEN_DISPS(gfxCtx);
    polyOpa = POLY_OPA_DISP;
    nextDisplayList = Graph_GfxPlusOne(polyOpa);
    gSPDisplayList(OVERLAY_DISP++, nextDisplayList);

    if ((play->msgCtx.currentTextId != 0x5E6) || !Play_InCsMode(play)) {
        Message_DrawMain(play, &nextDisplayList);
    }

    gSPEndDisplayList(nextDisplayList++);
    Graph_BranchDlist(polyOpa, nextDisplayList);
    POLY_OPA_DISP = nextDisplayList;
    CLOSE_DISPS(gfxCtx);
}

#pragma GLOBAL_ASM("asm/non_matchings/code/z_message/Message_Update.s")

void Message_SetTables(PlayState* play) {
    play->msgCtx.messageEntryTableNes = D_801C6B98;
    play->msgCtx.messageTableStaff = D_801CFB08;
}

void Message_Init(PlayState* play) {
    Font* font;
    MessageContext* messageCtx = &play->msgCtx;

    Message_SetTables(play);

    play->msgCtx.ocarinaMode = OCARINA_MODE_NONE;

    messageCtx->msgMode = 0;
    messageCtx->msgLength = 0;
    messageCtx->currentTextId = 0;
    messageCtx->textboxEndType = TEXTBOX_ENDTYPE_00;
    messageCtx->choiceIndex = 0;
    messageCtx->ocarinaAction = messageCtx->textUnskippable = 0;
    messageCtx->textColorAlpha = 0xFF;

    View_Init(&messageCtx->view, play->state.gfxCtx);

    messageCtx->textboxSegment = THA_AllocEndAlign16(&play->state.heap, 0x13C00);

    font = &play->msgCtx.font;
    Font_LoadOrderedFont(&play->msgCtx.font);
    font->unk_11D88 = 0;

    messageCtx->textIsCredits = messageCtx->unk12092 = false;
    messageCtx->unk12094 = 0;
    messageCtx->textFade = false;
    messageCtx->ocarinaAvailableSongs = 0;
    messageCtx->unk12068 = 0x34;
    messageCtx->unk1206A = 0x24;
    messageCtx->ocarinaSongEffectActive = false;
    messageCtx->unk120BE = 0;
    messageCtx->unk120C0 = 0;
    messageCtx->unk120C2 = 0;
    messageCtx->unk120C4 = 0;
    messageCtx->unk120C8 = 0;
    messageCtx->unk120CA = 0;
    messageCtx->unk120CC = 0;
    messageCtx->unk120CE = 0;
    messageCtx->unk120D0 = 0;
    messageCtx->unk120D2 = 0;
    messageCtx->unk120D4 = 0;
    messageCtx->unk120D6 = 0;
}
