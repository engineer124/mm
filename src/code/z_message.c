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

extern s16 D_801C6A74;
extern s16 D_801C6A78;

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

void func_80147520(void) {
    sOcarinaButtonIndexBuf[0] = OCARINA_BTN_INVALID;
    sOcarinaButtonAlphaValues[0] = sOcarinaButtonAlphaValues[1] = sOcarinaButtonAlphaValues[2] =
        sOcarinaButtonAlphaValues[3] = sOcarinaButtonAlphaValues[4] = sOcarinaButtonAlphaValues[5] =
            sOcarinaButtonAlphaValues[6] = sOcarinaButtonAlphaValues[7] = sOcarinaButtonAlphaValues[8] = 0;
}

void Message_ResetOcarinaNoteState(PlayState* play) {
    MessageContext* msgCtx = &play->msgCtx;

    msgCtx->ocarinaButtonsPosY[0] = 189;
    msgCtx->ocarinaButtonsPosY[1] = 184;
    msgCtx->ocarinaButtonsPosY[2] = 179;
    msgCtx->ocarinaButtonsPosY[3] = 174;
    msgCtx->ocarinaButtonsPosY[4] = 169;

    func_80147520();

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

    if ((msgCtx->unk12020 == 0x10) || (msgCtx->unk12020 == 0x11)) {
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

    if (msgCtx->unk12020 == 0x10 || msgCtx->unk12020 == 0x11) {
        return CHECK_BTN_ALL(controller->press.button, BTN_A);
    } else {
        return CHECK_BTN_ALL(controller->press.button, BTN_A) || CHECK_BTN_ALL(controller->press.button, BTN_B) ||
               CHECK_BTN_ALL(controller->press.button, BTN_CUP);
    }
}

void Message_CloseTextbox(PlayState* play) {
    MessageContext* msgCtx = &play->msgCtx;

    if (play->msgCtx.msgLength != 0) {
        msgCtx->stateTimer = 2;
        msgCtx->msgMode = 0x43;
        msgCtx->unk12020 = 0;
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

    if (msgCtx->textIsCredits == 0) {
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
    s16 stepvar0;
    s16 stepvar1;
    s16 stepvar2;
    s16 stepvar3;
    s16 stepvar4;
    s16 stepvar5;

    gfx = *gfxp;

    if (msgCtx->textIsCredits == 0) {
        stepvar0 = ABS_ALT(D_801CFD28 - D_801CFD10[D_801CFD38][0]) / D_801CFD34;
        stepvar1 = ABS_ALT(D_801CFD2C - D_801CFD10[D_801CFD38][1]) / D_801CFD34;
        stepvar2 = ABS_ALT(D_801CFD30 - D_801CFD10[D_801CFD38][2]) / D_801CFD34;

        if (D_801CFD28 >= D_801CFD10[D_801CFD38][0]) {
            D_801CFD28 -= stepvar0;
        } else {
            D_801CFD28 += stepvar0;
        }

        if (D_801CFD2C >= D_801CFD10[D_801CFD38][1]) {
            D_801CFD2C -= stepvar1;
        } else {
            D_801CFD2C += stepvar1;
        }

        if (D_801CFD30 >= D_801CFD10[D_801CFD38][2]) {
            D_801CFD30 -= stepvar2;
        } else {
            D_801CFD30 += stepvar2;
        }

        stepvar3 = ABS_ALT(D_801CFD3C - D_801CFD1C[D_801CFD38][0]) / D_801CFD34;
        stepvar4 = ABS_ALT(D_801CFD40 - D_801CFD1C[D_801CFD38][1]) / D_801CFD34;
        stepvar5 = ABS_ALT(D_801CFD44 - D_801CFD1C[D_801CFD38][2]) / D_801CFD34;

        if (D_801CFD3C >= D_801CFD1C[D_801CFD38][0]) {
            D_801CFD3C -= stepvar3;
        } else {
            D_801CFD3C += stepvar3;
        }

        if (D_801CFD40 >= D_801CFD1C[D_801CFD38][1]) {
            D_801CFD40 -= stepvar4;
        } else {
            D_801CFD40 += stepvar4;
        }

        if (D_801CFD44 >= D_801CFD1C[D_801CFD38][2]) {
            D_801CFD44 -= stepvar5;
        } else {
            D_801CFD44 += stepvar5;
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
    s16 stepvar0;
    s16 stepvar1;
    s16 stepvar2;
    s16 stepvar3;
    s16 stepvar4;
    s16 stepvar5;

    gfx = *gfxp;

    if (msgCtx->textIsCredits == 0) {
        stepvar0 = ABS_ALT(D_801CFD60 - D_801CFD48[D_801CFD70][0]) / D_801CFD6C;
        stepvar1 = ABS_ALT(D_801CFD64 - D_801CFD48[D_801CFD70][1]) / D_801CFD6C;
        stepvar2 = ABS_ALT(D_801CFD68 - D_801CFD48[D_801CFD70][2]) / D_801CFD6C;

        if (D_801CFD60 >= D_801CFD48[D_801CFD70][0]) {
            D_801CFD60 -= stepvar0;
        } else {
            D_801CFD60 += stepvar0;
        }

        if (D_801CFD64 >= D_801CFD48[D_801CFD70][1]) {
            D_801CFD64 -= stepvar1;
        } else {
            D_801CFD64 += stepvar1;
        }

        if (D_801CFD68 >= D_801CFD48[D_801CFD70][2]) {
            D_801CFD68 -= stepvar2;
        } else {
            D_801CFD68 += stepvar2;
        }

        stepvar3 = ABS_ALT(D_801CFD74 - D_801CFD54[D_801CFD70][0]) / D_801CFD6C;
        stepvar4 = ABS_ALT(D_801CFD78 - D_801CFD54[D_801CFD70][1]) / D_801CFD6C;
        stepvar5 = ABS_ALT(D_801CFD7C - D_801CFD54[D_801CFD70][2]) / D_801CFD6C;

        if (D_801CFD74 >= D_801CFD54[D_801CFD70][0]) {
            D_801CFD74 -= stepvar3;
        } else {
            D_801CFD74 += stepvar3;
        }

        if (D_801CFD78 >= D_801CFD54[D_801CFD70][1]) {
            D_801CFD78 -= stepvar4;
        } else {
            D_801CFD78 += stepvar4;
        }

        if (D_801CFD7C >= D_801CFD54[D_801CFD70][2]) {
            D_801CFD7C -= stepvar5;
        } else {
            D_801CFD7C += stepvar5;
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
#pragma GLOBAL_ASM("asm/non_matchings/code/z_message/func_80148558.s")
#endif

// Matching, in-function data
#ifdef NON_MATCHING
void func_80148B98(PlayState* play, u8 arg1) {
    static s16 sHeld = 0; // D_801CFD80
    MessageContext* msgCtx = &play->msgCtx;
    Input* curInput = CONTROLLER1(&play->state);

    if ((curInput->rel.stick_y > 29) && sHeld == 0) {
        sHeld = 1;
        msgCtx->choiceIndex--;
        if (msgCtx->choiceIndex > 128) {
            msgCtx->choiceIndex = 0;
        } else {
            play_sound(NA_SE_SY_CURSOR);
        }
        return;
    } else if ((curInput->rel.stick_y < -29) && sHeld == 0) {
        sHeld = 1;
        msgCtx->choiceIndex++;
        if (msgCtx->choiceIndex > arg1) {
            msgCtx->choiceIndex = arg1;
        } else {
            play_sound(NA_SE_SY_CURSOR);
        }
        return;
    } else {
        if (ABS_ALT(curInput->rel.stick_y) < 30) {
            sHeld = 0;
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/z_message/func_80148B98.s")
#endif

void func_80148CBC(PlayState* play, UNK_PTR puParm2, u8 arg2) {
    MessageContext* msgCtx = &play->msgCtx;

    msgCtx->textPosX = 0x30;
    if (arg2 == 1) {
        msgCtx->textPosY = msgCtx->unk11FFE[1 + msgCtx->choiceIndex];
    } else {
        msgCtx->textPosY = msgCtx->unk11FFE[msgCtx->choiceIndex];
    }
    Message_DrawTextboxIcon(play, puParm2, msgCtx->textPosX, msgCtx->textPosY);
}

// Matching, in-function data
#ifdef NON_MATCHING
void func_80148D64(PlayState* play) {
    static s16 D_801CFD84 = 0;
    MessageContext* msgCtx = &play->msgCtx;

    if (play->msgCtx.unk120A6 < -0x1D) {
        msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2]--;
        if (msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2] < '0') {
            msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2] = '9';
        }
        Font_LoadCharNES(play, msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2],
                         msgCtx->unk120C4 + (msgCtx->unk120C2 << 7));
        play_sound(NA_SE_SY_RUPY_COUNT);
    } else if (msgCtx->unk120A6 >= 0x1E) {
        msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2]++;
        if (msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2] > '9') {
            msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2] = '0';
        }
        Font_LoadCharNES(play, msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2],
                         msgCtx->unk120C4 + (msgCtx->unk120C2 << 7));
        play_sound(NA_SE_SY_RUPY_COUNT);
    } else {
        if ((msgCtx->unk120A4 >= 0x1E) && (D_801CFD84 == 0)) {
            D_801CFD84 = 1;
            msgCtx->unk120C2++;
            if (msgCtx->unk120C2 >= 3) {
                msgCtx->unk120C2 = 2;
            } else {
                play_sound(NA_SE_SY_CURSOR);
            }
        } else if ((msgCtx->unk120A4 < -0x1D) && (D_801CFD84 == 0)) {
            D_801CFD84 = 1;
            msgCtx->unk120C2--;
            if (msgCtx->unk120C2 < 0) {
                msgCtx->unk120C2 = 0;
            } else {
                play_sound(NA_SE_SY_CURSOR);
            }
        } else {
            D_801CFD84 = 0;
        }
    }
    msgCtx->bankRupeesSelected = (msgCtx->decodedBuffer.schar[msgCtx->unk120C0] * 100) - ('0' * 100);
    msgCtx->bankRupeesSelected += (msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + 1] * 10) - ('0' * 10);
    msgCtx->bankRupeesSelected += (msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + 2]) - '0';
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/z_message/func_80148D64.s")
#endif

void func_80149048(PlayState* play) {
    MessageContext* msgCtx = &play->msgCtx;

    if (msgCtx->unk120A6 < -29) {
        msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2]--;
        if (msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2] < 0x30) {
            msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2] = 0x39;
        }
        Font_LoadCharNES(play, msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2],
                         msgCtx->unk120C4 + (msgCtx->unk120C2 << 7));
        play_sound(NA_SE_SY_RUPY_COUNT);
    } else if (msgCtx->unk120A6 > 29) {
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
    static s16 D_801CFD88 = 0;
    MessageContext* msgCtx = &play->msgCtx;

    if (msgCtx->unk120A6 < -29) {
        msgCtx->unk12054[msgCtx->unk120C2]--;
        if (msgCtx->unk12054[msgCtx->unk120C2] <= 0) {
            msgCtx->unk12054[msgCtx->unk120C2] = 5;
        }
        msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2] = msgCtx->unk12054[msgCtx->unk120C2] + 0x30;
        Font_LoadCharNES(play, msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2],
                         msgCtx->unk120C4 + (msgCtx->unk120C2 << 7));
        play_sound(NA_SE_SY_RUPY_COUNT);
        return;
    }
    if ((s32)msgCtx->unk120A6 > 29) {
        msgCtx->unk12054[msgCtx->unk120C2]++;
        if (msgCtx->unk12054[msgCtx->unk120C2] > 5) {
            msgCtx->unk12054[msgCtx->unk120C2] = 1;
        }
        msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2] = msgCtx->unk12054[msgCtx->unk120C2] + 0x30;
        Font_LoadCharNES(play, msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2],
                         msgCtx->unk120C4 + (msgCtx->unk120C2 << 7));
        play_sound(NA_SE_SY_RUPY_COUNT);
        return;
    }
    if (((s32)msgCtx->unk120A4 > 29) && (D_801CFD88 == 0)) {
        D_801CFD88 = 1;
        msgCtx->unk120C2 += 1;
        if ((s32)msgCtx->unk120C2 > 4) {
            msgCtx->unk120C2 = 4;
            return;
        }
        play_sound(NA_SE_SY_CURSOR);
        return;
    }
    if (((s32)msgCtx->unk120A4 < -29) && (D_801CFD88 == 0)) {
        D_801CFD88 = 1;
        msgCtx->unk120C2 += -1;
        if ((s32)msgCtx->unk120C2 < 0) {
            msgCtx->unk120C2 = 0;
            return;
        }
        play_sound(NA_SE_SY_CURSOR);
        return;
    }
    D_801CFD88 = 0;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/z_message/func_801491DC.s")
#endif

// Matching, in-function data
#ifdef NON_MATCHING
void func_80149454(PlayState* play) {
    static s16 D_801CFD8C = 0;
    MessageContext* msgCtx = &play->msgCtx;

    if (msgCtx->unk120A6 < -29) {
        msgCtx->unk12054[msgCtx->unk120C2]--;
        if (msgCtx->unk12054[msgCtx->unk120C2] < 0) {
            msgCtx->unk12054[msgCtx->unk120C2] = 9;
        }
        msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2] = msgCtx->unk12054[msgCtx->unk120C2] + 0x30;
        Font_LoadCharNES(play, msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2],
                         msgCtx->unk120C4 + (msgCtx->unk120C2 << 7));
        play_sound(NA_SE_SY_RUPY_COUNT);
        return;
    }
    if ((s32)msgCtx->unk120A6 > 29) {
        msgCtx->unk12054[msgCtx->unk120C2]++;
        if (msgCtx->unk12054[msgCtx->unk120C2] > 9) {
            msgCtx->unk12054[msgCtx->unk120C2] = 0;
        }
        msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2] = msgCtx->unk12054[msgCtx->unk120C2] + 0x30;
        Font_LoadCharNES(play, msgCtx->decodedBuffer.schar[msgCtx->unk120C0 + msgCtx->unk120C2],
                         msgCtx->unk120C4 + (msgCtx->unk120C2 << 7));
        play_sound(NA_SE_SY_RUPY_COUNT);
        return;
    }
    if (((s32)msgCtx->unk120A4 > 29) && (D_801CFD8C == 0)) {
        D_801CFD8C = 1;
        msgCtx->unk120C2 += 1;
        if ((s32)msgCtx->unk120C2 > 2) {
            msgCtx->unk120C2 = 2;
            return;
        }
        play_sound(NA_SE_SY_CURSOR);
        return;
    }
    if (((s32)msgCtx->unk120A4 < -29) && (D_801CFD8C == 0)) {
        D_801CFD8C = 1;
        msgCtx->unk120C2 += -1;
        if ((s32)msgCtx->unk120C2 < 0) {
            msgCtx->unk120C2 = 0;
            return;
        }
        play_sound(NA_SE_SY_CURSOR);
        return;
    }
    D_801CFD8C = 0;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/z_message/func_80149454.s")
#endif

// Matching, in-function data
#ifdef NON_MATCHING
void func_801496C8(PlayState* play) {
    static s16 D_801CFD90 = 0; // sHeld
    MessageContext* msgCtx = &play->msgCtx;

    if (play->msgCtx.unk120A6 < -0x1D) {
        msgCtx->unk12054[msgCtx->unk120C2]--;
        if (msgCtx->unk12054[msgCtx->unk120C2] < 0) {
            msgCtx->unk12054[msgCtx->unk120C2] = 3;
        }
        msgCtx->decodedBuffer.wchar[msgCtx->unk120C0 + msgCtx->unk120C2] = msgCtx->unk12054[msgCtx->unk120C2] + 0x824F;
        Font_LoadChar(play, msgCtx->decodedBuffer.wchar[msgCtx->unk120C0 + msgCtx->unk120C2],
                      msgCtx->unk120C4 + (msgCtx->unk120C2 << 7));
        play_sound(NA_SE_SY_RUPY_COUNT);
        return;
    }
    if (msgCtx->unk120A6 >= 0x1E) {
        msgCtx->unk12054[msgCtx->unk120C2]++;
        if (msgCtx->unk12054[msgCtx->unk120C2] >= 4) {
            msgCtx->unk12054[msgCtx->unk120C2] = 0;
        }
        msgCtx->decodedBuffer.wchar[msgCtx->unk120C0 + msgCtx->unk120C2] = msgCtx->unk12054[msgCtx->unk120C2] + 0x824F;
        Font_LoadChar(play, msgCtx->decodedBuffer.wchar[msgCtx->unk120C0 + msgCtx->unk120C2],
                      msgCtx->unk120C4 + (msgCtx->unk120C2 << 7));
        play_sound(NA_SE_SY_RUPY_COUNT);
        return;
    }
    if ((msgCtx->unk120A4 >= 0x1E) && (D_801CFD90 == 0)) {
        D_801CFD90 = 1;
        msgCtx->unk120C2++;
        if (msgCtx->unk120C2 >= 6) {
            msgCtx->unk120C2 = 5;
            return;
        }
        play_sound(NA_SE_SY_CURSOR);
        return;
    }
    if ((msgCtx->unk120A4 < -0x1D) && (D_801CFD90 == 0)) {
        D_801CFD90 = 1;
        msgCtx->unk120C2--;
        if (msgCtx->unk120C2 < 0) {
            msgCtx->unk120C2 = 0;
            return;
        }
        play_sound(NA_SE_SY_CURSOR);
        return;
    }
    D_801CFD90 = 0;
}
#else
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
        gDPSetPrimColor(gfx++, 0, 0, 0, 0, 0, msgCtx->unk1201E);
        gSPTextureRectangle(gfx++, (x + 1) << 2, (y + 1) << 2, ((x + sCharTexSize + 1)) << 2,
                            ((y + sCharTexSize + 1)) << 2, G_TX_RENDERTILE, 0, 0, sCharTexScale, sCharTexScale);
        gDPPipeSync(gfx++);
    }

    gDPSetPrimColor(gfx++, 0, 0, msgCtx->unk12018, msgCtx->unk1201A, msgCtx->unk1201C, msgCtx->unk1201E);
    gSPTextureRectangle(gfx++, (x) << 2, (y) << 2, ((x + sCharTexSize)) << 2, ((y + sCharTexSize)) << 2,
                        G_TX_RENDERTILE, 0, 0, sCharTexScale, sCharTexScale);
    *gfxp = gfx++;
}

// resizes textboxes when opening them
void Message_GrowTextbox(PlayState* play) {
    MessageContext* msgCtx = &play->msgCtx;

    if (!play->pauseCtx.bombersNotebookOpen) {
        sTextboxWidth = (s16)(s32)(D_801CFDA4[msgCtx->stateTimer] * 256.0f);
        sTextboxTexWidth = (s16)(s32)(1024.0f / D_801CFDA4[msgCtx->stateTimer]);
        sTextboxHeight = (s16)(s32)(D_801CFDC8[msgCtx->stateTimer] * 64.0f);
        sTextboxTexHeight = (s16)(s32)(1024.0f / D_801CFDC8[msgCtx->stateTimer]);
        msgCtx->textboxY = msgCtx->textboxYTarget + ((0x40 - sTextboxHeight) / 2);
        msgCtx->textboxColorAlphaCurrent += msgCtx->textboxColorAlphaTarget / 8;
        msgCtx->stateTimer++;
    } else {
        msgCtx->stateTimer = 8;
        sTextboxWidth = 0x200;
        sTextboxTexWidth = 0x200;
        sTextboxHeight = 0x59;
        sTextboxTexHeight = 0x2DB;
        msgCtx->textboxY = msgCtx->textboxYTarget + ((0x5A - sTextboxHeight) / 2);
    }

    if (msgCtx->stateTimer == 8) {
        // Reached the end
        msgCtx->textboxX = msgCtx->textboxXTarget;
        msgCtx->textboxY = msgCtx->textboxYTarget;
        msgCtx->msgMode = 3;
        msgCtx->textboxColorAlphaCurrent = msgCtx->textboxColorAlphaTarget;
    }

    if (!play->pauseCtx.bombersNotebookOpen) {
        msgCtx->textboxX = msgCtx->textboxXTarget + ((0x100 - sTextboxWidth) / 2);
        return;
    }

    msgCtx->textboxX = msgCtx->textboxXTarget + ((0x200 - sTextboxWidth) / 2);
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

void Message_DrawItemIcon(PlayState* play, Gfx** gfxP) {
    MessageContext* msgCtx;
    Gfx* gfx;
    s32 index;

    msgCtx = &play->msgCtx;
    gfx = *gfxP;
    msgCtx->unk12016 = msgCtx->unk12014;

    if (msgCtx->msgMode == 6) {
        play_sound(0);
    }

    gDPPipeSync(gfx++);
    gDPSetCombineMode(gfx++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    gDPSetPrimColor(gfx++, 0, 0, 255, 255, 255, msgCtx->unk1201E);

    if (msgCtx->itemId == ITEM_RECOVERY_HEART) {
        gDPPipeSync(gfx++);
        gDPSetPrimColor(gfx++, 0, 0, 255, 0, 0, msgCtx->unk1201E);
        gDPSetEnvColor(gfx++, 0, 80, 0, 255);
        gDPLoadTextureBlock(gfx++, gHeartFullTex, G_IM_FMT_IA, G_IM_SIZ_8b, 16, 16, 0, G_TX_NOMIRROR | G_TX_WRAP,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    } else if ((msgCtx->itemId >= ITEM_RUPEE_GREEN) && (msgCtx->itemId <= ITEM_RUPEE_HUGE)) {
        index = msgCtx->itemId - ITEM_RUPEE_GREEN;
        gDPPipeSync(gfx++);
        gDPSetPrimColor(gfx++, 0, 0, D_801CFDEC[index][0], D_801CFDEC[index][1], D_801CFDEC[index][2],
                        msgCtx->unk1201E);
        gDPSetEnvColor(gfx++, 0, 80, 0, 255);
        gDPLoadTextureBlock(gfx++, gRupeeCounterIconTex, G_IM_FMT_IA, G_IM_SIZ_8b, 16, 16, 0, G_TX_NOMIRROR | G_TX_WRAP,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    } else if (msgCtx->itemId == ITEM_STRAY_FAIRIES) {
        msgCtx->unk12016 = 0x18;
        gDPPipeSync(gfx++);
        gDPSetPrimColor(gfx++, 0, 0, D_801CFE5C[((void)0, gSaveContext.dungeonIndex)][0],
                        D_801CFE5C[((void)0, gSaveContext.dungeonIndex)][1],
                        D_801CFE5C[((void)0, gSaveContext.dungeonIndex)][2], msgCtx->unk1201E);
        gDPSetEnvColor(gfx++, D_801CFE68[((void)0, gSaveContext.dungeonIndex)][0],
                       D_801CFE68[((void)0, gSaveContext.dungeonIndex)][1],
                       D_801CFE68[((void)0, gSaveContext.dungeonIndex)][2], 0);
        gDPLoadTextureBlock_4b(gfx++, gStrayFairyGlowingCircleIconTex, G_IM_FMT_I, 32, 24, 0, G_TX_NOMIRROR | G_TX_WRAP,
                               G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
        gSPTextureRectangle(gfx++, msgCtx->unk12010 << 2, msgCtx->unk12012 << 2,
                            (msgCtx->unk12010 + msgCtx->unk12014) << 2, (msgCtx->unk12012 + msgCtx->unk12016) << 2,
                            G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);
        gDPSetPrimColor(gfx++, 0, 0, 255, 255, 255, msgCtx->unk1201E);
        gDPLoadTextureBlock(gfx++, D_801CFE4C[((void)0, gSaveContext.dungeonIndex)], G_IM_FMT_RGBA, G_IM_SIZ_32b, 32,
                            24, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
    } else if ((msgCtx->itemId >= ITEM_SONG_SONATA) && (msgCtx->itemId <= ITEM_SONG_SUN)) {
        index = msgCtx->itemId - ITEM_SONG_SONATA;
        gDPSetPrimColor(gfx++, 0, 0, D_801CFE04[index], D_801CFE1C[index], D_801CFE34[index], msgCtx->unk1201E);
        gDPLoadTextureBlock(gfx++, msgCtx->textboxSegment + 0x1000, G_IM_FMT_IA, G_IM_SIZ_8b, 16, 24, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
        msgCtx->unk12016 = 0x18;
    } else if (msgCtx->itemId == 0xCC) {
        gDPPipeSync(gfx++);
        gDPSetPrimColor(gfx++, 0, 0, 255, 255, 0, msgCtx->unk1201E);
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

    if (msgCtx->msgMode == 6) {
        play->msgCtx.ocarinaMode = 1;
        if (msgCtx->ocarinaAction == 0x33) {
            msgCtx->msgMode = 0x27;
            return;
        } else if (msgCtx->ocarinaAction == 0x34) {
            AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_DEFAULT);
            AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_DEFAULT);
            msgCtx->ocarinaStaff = AudioOcarina_GetPlaybackStaff();
            msgCtx->ocarinaStaff->pos = 0;
            D_801C6A74 = D_801C6A78 = 0;
            Message_ResetOcarinaNoteState(play);
            msgCtx->stateTimer = 3;
            msgCtx->msgMode = 0x29;
            AudioOcarina_SetPlaybackSong(OCARINA_SONG_SCARECROW_LONG + 1, 1);
            return;
        }
        if (msgCtx->ocarinaAction == 0x35) {
            msgCtx->msgMode = 0x2A;
            return;
        }
        if (msgCtx->ocarinaAction == 0x36) {
            AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_DEFAULT);
            AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_DEFAULT);
            msgCtx->ocarinaStaff = AudioOcarina_GetPlaybackStaff();
            msgCtx->ocarinaStaff->pos = 0;
            D_801C6A74 = D_801C6A78 = 0;
            Message_ResetOcarinaNoteState(play);
            msgCtx->stateTimer = 3;
            msgCtx->msgMode = 0x2E;
            AudioOcarina_SetPlaybackSong(OCARINA_SONG_SCARECROW_SPAWN + 1, 1);
            return;
        }
        if (msgCtx->ocarinaAction == 0x37) {
            msgCtx->msgMode = 0x2F;
            return;
        }
        if (msgCtx->ocarinaAction == 0x3A) {
            msgCtx->msgMode = 0x32;
            return;
        }
        if ((msgCtx->ocarinaAction >= 0x47) && (msgCtx->ocarinaAction < 0x52)) {
            msgCtx->msgMode = 0x35;
            return;
        }
        if (((msgCtx->ocarinaAction >= 2) && (msgCtx->ocarinaAction < 0x11)) || (msgCtx->ocarinaAction == 0x3B) ||
            (msgCtx->ocarinaAction == 0x3C) || (msgCtx->ocarinaAction == 0x3D) || (msgCtx->ocarinaAction == 0x3E) ||
            (msgCtx->ocarinaAction == 0x3F) || (msgCtx->ocarinaAction == 0x40)) {
            msgCtx->msgMode = 0xB;
            return;
        }
        if ((msgCtx->ocarinaAction == 0x41) || (msgCtx->ocarinaAction == 0x42) || (msgCtx->ocarinaAction == 0x43) ||
            (msgCtx->ocarinaAction == 0x44) || (msgCtx->ocarinaAction == 0x45) || (msgCtx->ocarinaAction == 0x46)) {
            msgCtx->msgMode = 0xC;
            return;
        }
        if ((msgCtx->ocarinaAction == 0) || (msgCtx->ocarinaAction == 1) || (msgCtx->ocarinaAction >= 0x28)) {
            msgCtx->msgMode = 0xA;
            return;
        }
        msgCtx->msgMode = 0xC;
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/code/z_message/Message_DrawText.s")

void Message_LoadItemIcon(PlayState* play, u16 itemId, s16 arg2) {
    MessageContext* msgCtx = &play->msgCtx;
    u16* new_var2 = &itemId;

    if (itemId == ITEM_RECOVERY_HEART) {
        msgCtx->unk12010 = (msgCtx->unk11FF8 - D_801CFF88[gSaveContext.options.language]);
        msgCtx->unk12012 = (arg2 + 0xA);
        msgCtx->unk12014 = 0x10;
    } else if ((itemId >= ITEM_RUPEE_GREEN) && (itemId <= ITEM_RUPEE_HUGE)) {
        msgCtx->unk12010 = (msgCtx->unk11FF8 - D_801CFF88[gSaveContext.options.language]);
        msgCtx->unk12012 = (arg2 + 0xA);
        msgCtx->unk12014 = 0x10;
    } else if (itemId == ITEM_STRAY_FAIRIES) {
        msgCtx->unk12010 = (msgCtx->unk11FF8 - D_801CFF70[gSaveContext.options.language]);
        msgCtx->unk12012 = (arg2 + 0xA);
        msgCtx->unk12014 = 0x20;
    } else if ((itemId >= ITEM_SONG_SONATA) && (itemId <= ITEM_SONG_SUN)) {
        msgCtx->unk12010 = (msgCtx->unk11FF8 - D_801CFF88[gSaveContext.options.language]);
        msgCtx->unk12012 = (arg2 + 0xA);
        msgCtx->unk12014 = 0x10;
        func_80178E3C(SEGMENT_ROM_START(icon_item_static_test), 0x61, msgCtx->textboxSegment + 0x1000, 0x180);
    } else if (itemId == ITEM_BOMBERS_NOTEBOOK) {
        msgCtx->unk12010 = (msgCtx->unk11FF8 - D_801CFF70[gSaveContext.options.language]);
        msgCtx->unk12012 = (arg2 + 6);
        msgCtx->unk12014 = 0x20;
        func_80178E3C(SEGMENT_ROM_START(icon_item_static_test), 0x61, msgCtx->textboxSegment + 0x1000, 0x1000);
    } else if (itemId <= ITEM_REMAINS_TWINMOLD) {
        msgCtx->unk12010 = (msgCtx->unk11FF8 - D_801CFF70[gSaveContext.options.language]);
        msgCtx->unk12012 = (arg2 + 6);
        msgCtx->unk12014 = 0x20;
        func_80178E3C(SEGMENT_ROM_START(icon_item_static_test), itemId, msgCtx->textboxSegment + 0x1000, 0x1000);
    } else if (itemId == 0xCC) {
        msgCtx->unk12010 = (msgCtx->unk11FF8 - D_801CFF70[gSaveContext.options.language]);
        msgCtx->unk12012 = (arg2 + 8);
        msgCtx->unk12014 = 0x20;
        func_80178E3C(SEGMENT_ROM_START(schedule_dma_static_test), 0x15, msgCtx->textboxSegment + 0x1000, 0x400);
    } else if (itemId >= 0xB8) {
        msgCtx->unk12010 = (msgCtx->unk11FF8 - D_801CFF70[gSaveContext.options.language]);
        msgCtx->unk12012 = (arg2 + 8);
        msgCtx->unk12014 = 0x20;
        func_80178E3C(SEGMENT_ROM_START(schedule_dma_static_test), (itemId - 0xB8), msgCtx->textboxSegment + 0x1000,
                      0x800);
    } else if (itemId >= 0x6E) {
        msgCtx->unk12010 = (msgCtx->unk11FF8 - D_801CFF7C[gSaveContext.options.language]);
        msgCtx->unk12012 = (arg2 + 0xA);
        msgCtx->unk12014 = 0x18;
        func_80178E3C(SEGMENT_ROM_START(icon_item_24_static_test), (itemId - 0x6E), msgCtx->textboxSegment + 0x1000,
                      0x900);
    }

    if (play->pauseCtx.bombersNotebookOpen) {
        msgCtx->unk12010 = ((msgCtx->unk12010 * 1.4f) + 2.0f);
        msgCtx->unk12014 = (msgCtx->unk12014 * 1.4f);
    }

    msgCtx->unk12022 = 1;
}

void Message_LoadChar(PlayState* play, u16 codePointIndex, s32* offset, f32* arg3, s16 decodedBufPos) {
    MessageContext* msgCtx = &play->msgCtx;
    s32 temp1 = *offset;
    f32 temp2 = *arg3;

    Font_LoadChar(play, codePointIndex, temp1);
    msgCtx->decodedBuffer.wchar[decodedBufPos] = codePointIndex;
    temp1 += FONT_CHAR_TEX_SIZE;
    temp2 += (16.0f * msgCtx->unk12098);
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
    t += 1;
    Font_LoadChar(play, 0x8373, k); // 0x8373 = ピ in JISX0213
    k += FONT_CHAR_TEX_SIZE;
    msgCtx->decodedBuffer.wchar[t] = 0x8373;
    t += 1;
    Font_LoadChar(play, 0x815C, k); // Ox815C = ― in JISX0213
    k += FONT_CHAR_TEX_SIZE;
    msgCtx->decodedBuffer.wchar[t] = 0x815C;

    f += 16.0f * msgCtx->unk12098 * 3.0f;
    *decodedBufPos = t;
    *offset = k;
    *arg3 = f;
}

void func_8014CDF0(OSTime time, s16* digits) {
    // offsetting to actual codepoints is done outside this function
    // every digits will be added 0x824F to get an actual S-JIS
    // printable character.
    OSTime temp_t2 = time;

    digits[0] = temp_t2 / 36000;
    temp_t2 -= digits[0] * 36000;

    digits[1] = temp_t2 / 6000;
    temp_t2 -= digits[1] * 6000;

    digits[2] = 0x135B; // 0x135B + 0x824F = 分 (minutes) in S-JIS

    digits[3] = temp_t2 / 1000;
    temp_t2 -= digits[3] * 1000;

    digits[4] = temp_t2 / 100;
    temp_t2 -= digits[4] * 100;

    digits[5] = 0x1313; // 0x1313 + 0x824F = 秒 (seconds) in S-JIS

    digits[6] = temp_t2 / 10;
    temp_t2 -= digits[6] * 10;

    digits[7] = temp_t2;
}

void Message_SetupLoadItemIcon(PlayState* play) {
    Font* font;
    MessageContext* msgCtx = &play->msgCtx;

    font = &msgCtx->font;
    if (msgCtx->msgBufPos == 0) {
        if (font->msgBuf.schar[msgCtx->msgBufPos + 2] != 0xFE) {
            msgCtx->unk11F18 = 0;
            if ((msgCtx->currentTextId == 0x176F) || (msgCtx->currentTextId == 0x1770) ||
                (msgCtx->currentTextId == 0x1771)) {
                msgCtx->itemId = ITEM_OCARINA;
                msgCtx->msgBufPos += 2;
            } else {
                msgCtx->msgBufPos += 2;
                if ((font->msgBuf.schar[msgCtx->msgBufPos] < 0xC8) || (font->msgBuf.schar[msgCtx->msgBufPos] >= 0xD8)) {
                    msgCtx->itemId = D_801CFF94[(u8)font->msgBuf.schar[msgCtx->msgBufPos]];
                } else {
                    msgCtx->itemId = 0xFE;
                }
            }
        } else {
            msgCtx->msgBufPos += 2;
            msgCtx->itemId = font->msgBuf.schar[msgCtx->msgBufPos];
        }
        msgCtx->unk11F14 = font->msgBuf.schar[++msgCtx->msgBufPos] << 8;
        msgCtx->unk11F14 |= font->msgBuf.schar[++msgCtx->msgBufPos];

        msgCtx->unk1206C = font->msgBuf.schar[++msgCtx->msgBufPos] << 8;
        msgCtx->unk1206C |= font->msgBuf.schar[++msgCtx->msgBufPos];

        msgCtx->unk12070 = font->msgBuf.schar[++msgCtx->msgBufPos] << 8;
        msgCtx->unk12070 |= font->msgBuf.schar[++msgCtx->msgBufPos];

        msgCtx->unk12074 = font->msgBuf.schar[++msgCtx->msgBufPos] << 8;
        msgCtx->unk12074 |= font->msgBuf.schar[++msgCtx->msgBufPos];

        msgCtx->msgBufPos++;
        if (msgCtx->itemId != 0xFE) {
            Message_LoadItemIcon(play, msgCtx->itemId, msgCtx->textboxY + 10);
        }
    }
}

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
    f += 7.0f * (16.0f * msgCtx->unk12098);
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
    sp3C += (stringLimit - 1) * (16.0f * msgCtx->unk12098);

    *decodedBufPos = temp_s1;
    *offset = temp_s2;
    *arg2 = sp3C;
}

void Message_Decode(PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/code/z_message/Message_Decode.s")

void func_80150A84(PlayState* play) {
    MessageContext* msgCtx = &play->msgCtx;
    u8 offset = msgCtx->textBoxType;
    u8 t;

    if (D_801CFC78[offset] != 0xE) {
        DmaMgr_SendRequest0(msgCtx->textboxSegment,
                            &((u8*)SEGMENT_ROM_START(message_static))[D_801CFC78[offset] * 0x1000], 0x1000);

        // TODO: I think there's an optimization here:
        // `(offset == 0)` instead of `(&D_801CFC78[offset] == &D_801CFC78[t = 0])`
        if (!play->pauseCtx.bombersNotebookOpen) {
            if ((&D_801CFC78[offset] == &D_801CFC78[t = 0]) || (&D_801CFC78[offset] == &D_801CFC78[t = 6]) ||
                (&D_801CFC78[offset] == &D_801CFC78[t = 10]) || (&D_801CFC78[offset] == &D_801CFC78[t = 11])) {
                msgCtx->textboxColorRed = 0;
                msgCtx->textboxColorGreen = 0;
                msgCtx->textboxColorBlue = 0;
            } else if (&D_801CFC78[offset] == &D_801CFC78[t = 1]) {
                msgCtx->textboxColorRed = 70;
                msgCtx->textboxColorGreen = 50;
                msgCtx->textboxColorBlue = 30;
            } else if ((&D_801CFC78[offset] == &D_801CFC78[t = 2]) || (&D_801CFC78[offset] == &D_801CFC78[t = 8])) {
                msgCtx->textboxColorRed = 0;
                msgCtx->textboxColorGreen = 10;
                msgCtx->textboxColorBlue = 50;
            } else if (&D_801CFC78[offset] == &D_801CFC78[t = 13]) {
                msgCtx->textboxColorRed = 255;
                msgCtx->textboxColorGreen = 255;
                msgCtx->textboxColorBlue = 195;
            } else {
                msgCtx->textboxColorRed = 255;
                msgCtx->textboxColorGreen = 0;
                msgCtx->textboxColorBlue = 0;
            }
            if (&D_801CFC78[offset] == &D_801CFC78[t = 1]) {
                msgCtx->textboxColorAlphaTarget = 230;
            } else if (&D_801CFC78[offset] == &D_801CFC78[t = 3]) {
                msgCtx->textboxColorAlphaTarget = 180;
            } else if (&D_801CFC78[offset] == &D_801CFC78[t = 13]) {
                msgCtx->textboxColorAlphaTarget = 220;
            } else {
                msgCtx->textboxColorAlphaTarget = 170;
            }
        } else {
            msgCtx->textboxColorRed = 255;
            msgCtx->textboxColorGreen = 255;
            msgCtx->textboxColorBlue = 255;
            msgCtx->textboxColorAlphaTarget = 255;
        }
        msgCtx->textboxColorAlphaCurrent = 0;
    }
}

void Message_OpenText(PlayState* play, u16 textId) {
    MessageContext* msgCtx = &play->msgCtx;
    Font* font = &msgCtx->font;
    Player* player = GET_PLAYER(play);
    f32 var_fv0;

    if (play->msgCtx.msgMode == 0) {
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
    } else if ((textId == 0x92) && (play->sceneId == 0x6A)) {
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
    msgCtx->textIsCredits = 0;
    var_fv0 = 1.0f;

    if (play->pauseCtx.bombersNotebookOpen) {
        if (gSaveContext.options.language == 0) {
            msgCtx->unk12098 = 1.4f;
            msgCtx->unk11FFC = 0x1E;
            msgCtx->unk11FF8 = 0x32;
            var_fv0 = 1.4;
        } else {
            msgCtx->unk12098 = 1.4f;
            msgCtx->unk11FFC = 0x16;
            msgCtx->unk11FF8 = 0x32;
            var_fv0 = 1.4;
        }
    } else if (textId >= 0x4E20) {
        msgCtx->textIsCredits = 1;
        msgCtx->unk12098 = 0.85f;
        msgCtx->unk11FFC = 6;
        msgCtx->unk11FF8 = 0x14;
    } else if (gSaveContext.options.language == 0) {
        msgCtx->unk12098 = 0.88f;
        msgCtx->unk11FFC = 0x12;
        msgCtx->unk11FF8 = 0x32;
    } else {
        msgCtx->unk12098 = 0.75f;
        msgCtx->unk11FFC = 0xC;
        msgCtx->unk11FF8 = 0x41;
    }

    sCharTexSize = msgCtx->unk12098 * 16.0f;
    sCharTexScale = 1024.0f / msgCtx->unk12098;
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
        MessageNES_FindMessage(play, textId);
        msgCtx->msgLength = font->messageEnd;
        DmaMgr_SendRequest0(&font->msgBuf, &_message_data_staticSegmentRomStart[font->messageStart], font->messageEnd);
    }

    msgCtx->unk12022 = 0;
    msgCtx->textUnskippable = 0;
    msgCtx->unk12020 = 0;
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
        msgCtx->unk1201E = 0xFF;
    } else {
        msgCtx->unk1201E = 0;
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

void func_801514B0(PlayState* play, u16 arg1, u8 arg2) {
    MessageContext* msgCtx = &play->msgCtx;
    Font* font = &msgCtx->font;
    Player* player = GET_PLAYER(play);
    f32 temp = 1024.0f;

    msgCtx->ocarinaAction = 0xFFFF;

    if (msgCtx->msgMode == 0) {
        gSaveContext.prevHudVisibility = gSaveContext.hudVisibility;
    }
    msgCtx->unk12092 = 0;
    D_801C6A70 = 0;
    msgCtx->unk12094 = 0;
    msgCtx->textIsCredits = 0;
    if (gSaveContext.options.language == 0) {
        msgCtx->unk12098 = 0.88f;
        msgCtx->unk11FFC = 0x12;
        msgCtx->unk11FF8 = 0x32;
    } else {
        msgCtx->unk12098 = 0.75f;
        msgCtx->unk11FFC = 0xC;
        msgCtx->unk11FF8 = 0x41;
    }
    sCharTexSize = (msgCtx->unk12098 * 16.0f);
    sCharTexScale = (temp / msgCtx->unk12098);
    D_801F6B08 = (temp / 1);
    if ((arg1 == 0x1709) && (player->transformation == 3)) {
        arg1 = 0x1705;
    }
    msgCtx->currentTextId = arg1;
    if (gSaveContext.options.language == 0) {
        Message_FindMessage(play, arg1);
        msgCtx->msgLength = font->messageEnd;
        DmaMgr_SendRequest0(&font->msgBuf, &SEGMENT_ROM_START(message_data_static)[font->messageStart],
                            font->messageEnd);
    } else {
        MessageNES_FindMessage(play, arg1);
        msgCtx->msgLength = font->messageEnd;
        DmaMgr_SendRequest0(&font->msgBuf, &SEGMENT_ROM_START(message_data_static)[font->messageStart],
                            font->messageEnd);
    }
    msgCtx->unk12022 = 0;
    msgCtx->textUnskippable = 0;
    msgCtx->unk12020 = 0;
    msgCtx->textDrawPos = 0;
    msgCtx->msgBufPos = 0;
    msgCtx->decodedTextLen = 0;
    msgCtx->unk11F08 = (u16)font->msgBuf.wchar[msgCtx->msgBufPos];
    msgCtx->unk11F18 = (s8)((s32)(msgCtx->unk11F08 & 0xF000) >> 0xC);
    msgCtx->textBoxType = 9;
    msgCtx->textBoxPos = arg2;
    msgCtx->unk11F0C = msgCtx->unk11F08 & 0xF;
    msgCtx->textUnskippable = 1;
    DmaMgr_SendRequest0(msgCtx->textboxSegment, &SEGMENT_ROM_START(message_static)[D_801CFC78[0] << 0xC], 0x1000);
    msgCtx->textboxColorRed = 0;
    msgCtx->textboxColorGreen = 0;
    msgCtx->textboxColorBlue = 0;
    msgCtx->textboxColorAlphaTarget = 0xE6;
    msgCtx->textboxColorAlphaCurrent = 0;
    msgCtx->unk1201E = 0xFF;
    msgCtx->talkActor = NULL;
    msgCtx->msgMode = 1;
    msgCtx->stateTimer = 0;
    msgCtx->unk12024 = 0;
    play->msgCtx.ocarinaMode = 0;
    msgCtx->unk120D2 = 0xFF;
    msgCtx->unk120CE = msgCtx->unk120D0 = msgCtx->unk120D2;
}

void Message_StartTextbox(PlayState* play, u16 textId, Actor* Actor) {
    MessageContext* msgCtx = &play->msgCtx;

    msgCtx->ocarinaAction = 0xFFFF;
    Message_OpenText(play, textId);
    msgCtx->talkActor = Actor;
    msgCtx->msgMode = 1;
    msgCtx->stateTimer = 0;
    msgCtx->unk12024 = 0;
    play->msgCtx.ocarinaMode = 0;
}

void Message_ContinueTextbox(PlayState* play, u16 textId) {
    MessageContext* msgCtx = &play->msgCtx;
    InterfaceContext* interfaceCtx = &play->interfaceCtx;

    msgCtx->msgLength = 0;
    Message_OpenText(play, textId);
    func_80150A84(play);
    msgCtx->msgMode = 5;
    msgCtx->stateTimer = 8;
    msgCtx->unk12024 = 0;

    if (interfaceCtx->unk_222 == 0) {
        if (textId != 0x1B93) {
            func_8011552C(play, 0x10);
        } else if (textId != 0xF8) {
            func_8011552C(play, 6);
        }
    }
    msgCtx->textboxColorAlphaCurrent = msgCtx->textboxColorAlphaTarget;

    if (play->pauseCtx.bombersNotebookOpen) {
        msgCtx->textboxXTarget = 0x22;
        msgCtx->textboxYTarget = 0x15E;
        Message_GrowTextbox(play);
        msgCtx->stateTimer = 1;
    }
}

void func_80151A68(PlayState* play, u16 textId) {
    MessageContext* msgCtx = &play->msgCtx;

    msgCtx->msgLength = 0;
    Message_OpenText(play, textId);
    func_80150A84(play);
    func_8015B198(play);
    msgCtx->msgMode = 0x45;
    msgCtx->unk12024 = 0;
    msgCtx->textboxColorAlphaCurrent = msgCtx->textboxColorAlphaTarget = msgCtx->unk1201E = 0;
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
    u8 temp = arg1;

    if (CHECK_QUEST_ITEM(QUEST_BOMBERS_NOTEBOOK)) {
        if ((gSaveContext.save.weekEventReg[D_801C6B28[arg1] >> 8] & (u8)D_801C6B28[arg1]) == 0) {
            msgCtx->unk120B2[msgCtx->unk120B1] = temp;
            msgCtx->unk120B1++;
        }
    } else if (arg1 >= 20) {
        if ((gSaveContext.save.weekEventReg[D_801C6B28[arg1] >> 8] & (u8)D_801C6B28[arg1]) == 0) {
            msgCtx->unk120B2[msgCtx->unk120B1] = temp;
            msgCtx->unk120B1++;
        }
    }
}

u32 func_80151C9C(PlayState* play) {
    MessageContext* msgCtx = &play->msgCtx;

    while (true) {
        if (msgCtx->unk120B1 == 0) {
            return false;
        }
        msgCtx->unk120B1--;

        if ((gSaveContext.save.weekEventReg[D_801C6B28[msgCtx->unk120B2[msgCtx->unk120B1]] >> 8] &
             (u8)D_801C6B28[msgCtx->unk120B2[msgCtx->unk120B1]]) == 0) {
            gSaveContext.save.weekEventReg[D_801C6B28[msgCtx->unk120B2[msgCtx->unk120B1]] >> 8] =
                ((void)0, gSaveContext.save.weekEventReg[D_801C6B28[msgCtx->unk120B2[msgCtx->unk120B1]] >> 8]) |
                (u8)D_801C6B28[msgCtx->unk120B2[msgCtx->unk120B1]];

            if ((D_801C6AB8[msgCtx->unk120B2[msgCtx->unk120B1]] != 0) && CHECK_QUEST_ITEM(QUEST_BOMBERS_NOTEBOOK)) {
                Message_ContinueTextbox(play, D_801C6AB8[msgCtx->unk120B2[msgCtx->unk120B1]]);
                play_sound(NA_SE_SY_SCHEDULE_WRITE);
                return true;
            }
        }
    }
}

extern s16 D_801C6A7C;
extern u16 D_801D023A[];
extern u16 D_801D028C[];
extern u16 sOcarinaSongFlagsMap[];

void Message_StartOcarinaImpl(PlayState* play, u16 ocarinaActionId) {
    MessageContext* msgCtx = &play->msgCtx;
    s16 j;
    s16 noStop;
    s32 k;
    u32 i;

    for (i = msgCtx->ocarinaAvailableSongs = 0; i <= (QUEST_SONG_SUN - QUEST_SONG_SONATA); i++) {
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

    if ((ocarinaActionId == 0x41) || (ocarinaActionId == 0x42)) {
        sOcarinaButtonAlphaValues[0] = sOcarinaButtonAlphaValues[1] = sOcarinaButtonAlphaValues[2] =
            sOcarinaButtonAlphaValues[3] = 255;
    } else {
        msgCtx->ocarinaStaff->pos = 0;
        D_801C6A74 = 0;
        D_801C6A78 = 0;
        Message_ResetOcarinaNoteState(play);
    }

    D_801C6A7C = 0xFF;
    msgCtx->lastPlayedSong = 0xFF;
    msgCtx->unk12048 = 0xFF;
    noStop = false;
    msgCtx->ocarinaAction = ocarinaActionId;

    if ((ocarinaActionId >= 0x47) && (ocarinaActionId <= 0x51)) {
        Message_OpenText(play, 0x1B59);
        func_80150A84(play);
    } else if ((ocarinaActionId == 0x3B) || (ocarinaActionId == 0x3C)) {
        noStop = true;
        Message_OpenText(play, D_801D028C[ocarinaActionId - 0x29]);
        func_80150A84(play);
    } else if ((ocarinaActionId >= 0x3D) && (ocarinaActionId <= 0x42)) {
        noStop = true;
        Message_OpenText(play, D_801D028C[ocarinaActionId - 0x29]);
        func_80150A84(play);
    } else if ((ocarinaActionId >= 0x43) && (ocarinaActionId <= 0x46)) {
        noStop = true;
        Message_OpenText(play, 0x1B59);
        func_80150A84(play);
    } else if ((ocarinaActionId == 1) || (ocarinaActionId >= 0x28)) {
        if ((ocarinaActionId >= 0x28) && (ocarinaActionId <= 0x2C)) {
            play_sound(0x4807);
        }
        if (ocarinaActionId == 0x36) {
            Message_OpenText(play, 0x1B5B);
            func_80150A84(play);
        } else {
            Message_OpenText(play, 0x1B5A);
            func_80150A84(play);
        }
    } else {
        noStop = true;
        if (ocarinaActionId >= 0x12) {
            Message_OpenText(play, 0x1B59);
            func_80150A84(play);
        } else {
            Message_OpenText(play, D_801D028C[ocarinaActionId]);
            func_80150A84(play);
        }
    }

    msgCtx->talkActor = NULL;
    msgCtx->unk12024 = 0;
    play->msgCtx.ocarinaMode = 0;
    msgCtx->textboxXTarget = 0x22;
    msgCtx->textboxYTarget = 0x8E;
    msgCtx->stateTimer = 8;
    Message_GrowTextbox(play);
    msgCtx->textboxX = 0x22;
    msgCtx->textboxY = 0x8E;
    msgCtx->unk1200C = 0x200;
    msgCtx->unk1200E = 0x200;
    msgCtx->unk12008 = 0x100;
    msgCtx->unk1200A = 0x40;
    msgCtx->msgMode = 5;
    msgCtx->textboxColorAlphaCurrent = msgCtx->textboxColorAlphaTarget;
    msgCtx->textboxColorAlphaCurrent = msgCtx->textboxColorAlphaTarget;

    if (!noStop) {
        func_80115844(play, 0x12);
        noStop = gSaveContext.hudVisibility;
        Interface_SetHudVisibility(HUD_VISIBILITY_B_ALT);
        gSaveContext.hudVisibility = noStop;
    }

    if ((ocarinaActionId == 1) || (ocarinaActionId == 0x38)) {
        msgCtx->msgMode = 0xA;
        msgCtx->textBoxType = 0xE;
    } else if (ocarinaActionId == 0x3A) {
        msgCtx->msgMode = 0x32;
    } else if (ocarinaActionId == 0x37) {
        msgCtx->msgMode = 0x2F;
        msgCtx->textBoxType = 2;
    } else if (ocarinaActionId == 0x34) {
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_DEFAULT);
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_DEFAULT);
        msgCtx->ocarinaStaff = AudioOcarina_GetPlaybackStaff();
        msgCtx->ocarinaStaff->pos = 0;
        D_801C6A78 = 0;
        D_801C6A74 = D_801C6A78;
        Message_ResetOcarinaNoteState(play);
        msgCtx->msgMode = 0x29;
        AudioOcarina_SetPlaybackSong(OCARINA_SONG_SCARECROW_LONG + 1, 1);
        gSaveContext.hudVisibility = 0;
        Interface_SetHudVisibility(HUD_VISIBILITY_NONE);
    }

    for (j = 0, k = 0; j < 48; j++, k += 0x80) {
        Font_LoadChar(play, 0x8140, k);
    };

    msgCtx->stateTimer = 3;
}

void Message_StartOcarina(PlayState* play, u16 ocarinaActionId) {
    play->msgCtx.blockSunsSong = false;
    Message_StartOcarinaImpl(play, ocarinaActionId);
}

void Message_StartOcarinaBlockSunsSong(PlayState* play, u16 ocarinaActionId) {
    play->msgCtx.blockSunsSong = true;
    Message_StartOcarinaImpl(play, ocarinaActionId);
}

/**
 * @return u8 A value of the TextState enum representing the current state of the on-screen message
 */
u8 Message_GetState(MessageContext* msgCtx) {
    if (msgCtx->msgLength == 0) {
        return TEXT_STATE_NONE;
    }

    if (msgCtx->msgMode == 0x42) {
        if (msgCtx->unk11F14 != 0xFFFF) {
            return TEXT_STATE_1;
        }

        if ((msgCtx->unk12020 == 0x10) || (msgCtx->unk12020 == 0x11)) {
            return TEXT_STATE_CHOICE;
        }
        if ((msgCtx->unk12020 == 0x40) || (msgCtx->unk12020 == 0x42) || (msgCtx->unk12020 == 0x30)) {
            return TEXT_STATE_5;
        }
        if (msgCtx->unk12020 == 0x41) {
            return TEXT_STATE_16;
        }
        if ((msgCtx->unk12020 >= 0x50) && (msgCtx->unk12020 < 0x58)) {
            return TEXT_STATE_3;
        }
        if ((msgCtx->unk12020 == 0x60) || (msgCtx->unk12020 == 0x61)) {
            return TEXT_STATE_14;
        }
        if (msgCtx->unk12020 == 0x62) {
            return TEXT_STATE_15;
        }
        if (msgCtx->unk12020 == 0x63) {
            return TEXT_STATE_17;
        }
        if (msgCtx->unk12020 == 0x12) {
            return TEXT_STATE_18;
        }
        return TEXT_STATE_DONE;
    }

    if (msgCtx->msgMode == 0x41) {
        return TEXT_STATE_10;
    }
    if (msgCtx->msgMode == 0x1B) {
        return TEXT_STATE_7;
    }
    if ((msgCtx->ocarinaMode == 3) || (msgCtx->msgMode == 0x37)) {
        return TEXT_STATE_8;
    }
    if (msgCtx->msgMode == 0x20) {
        return TEXT_STATE_9;
    }
    if ((msgCtx->msgMode == 0x21) || (msgCtx->msgMode == 0x3A)) {
        return TEXT_STATE_11;
    }
    if (msgCtx->msgMode == 0x3D) {
        return TEXT_STATE_12;
    }
    if (msgCtx->msgMode == 0x40) {
        return TEXT_STATE_13;
    }
    if ((msgCtx->msgMode == 0x43) && (msgCtx->stateTimer == 1) && (msgCtx->unk120B1 == 0)) {
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

    gDPSetPrimColor(gfx++, 0, 0, msgCtx->textboxColorRed, msgCtx->textboxColorGreen, msgCtx->textboxColorBlue,
                    msgCtx->textboxColorAlphaCurrent);

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
        gSPTextureRectangle(gfx++, (msgCtx->textboxX << 2), ((msgCtx->textboxY + 22) << 2),
                            ((msgCtx->textboxX + msgCtx->unk12008) << 2), ((msgCtx->textboxY + 0x36) << 2),
                            G_TX_RENDERTILE, 0, 0x0006, msgCtx->unk1200C << 1, 0x800);
    } else {
        gSPTextureRectangle(gfx++, (msgCtx->textboxX << 2), ((msgCtx->textboxY) << 2),
                            ((msgCtx->textboxX + sTextboxWidth) << 2), ((msgCtx->textboxY + sTextboxHeight) << 2),
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

    if (msgCtx->ocarinaAction == 2) {
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_MONKEY);
    } else if (msgCtx->ocarinaAction == 9) {
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_PIANO);
    } else if (msgCtx->ocarinaAction == 7) {
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_TATL);
    } else if (msgCtx->ocarinaAction == 0xA) {
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_FEMALE_VOICE);
    } else if (msgCtx->ocarinaAction == 0xC) {
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_DEFAULT);
    } else if (msgCtx->ocarinaAction == 3) {
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_BABY_SINGING);
    } else if (msgCtx->ocarinaAction == 0x10) {
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_ELDER_GORON_DRUMS);
    } else if (msgCtx->ocarinaAction == 6) {
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_TATL);
    } else if (msgCtx->ocarinaAction == 5) {
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_IKANA_KING);
    } else {
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_DEFAULT);
    }

    if ((msgCtx->ocarinaAction == 0x3B) || (msgCtx->ocarinaAction == 0x3C)) {
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_ELDER_GORON_DRUMS);
        AudioOcarina_SetPlaybackSong(OCARINA_SONG_GORON_LULLABY_INTRO + 1, 0x83);
        return;
    }
    if ((msgCtx->ocarinaAction == 0x3D) || (msgCtx->ocarinaAction == 0x3E)) {
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_BASS_GUITAR);
        AudioOcarina_SetPlaybackSong((msgCtx->ocarinaAction - 0x29), 0x84);
        return;
    }
    if ((msgCtx->ocarinaAction == 0x3F) || (msgCtx->ocarinaAction == 0x40)) {
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_ZORA_GUITAR);
        AudioOcarina_SetPlaybackSong((msgCtx->ocarinaAction - 0x2B), 0x44);
        return;
    }
    AudioOcarina_SetPlaybackSong((msgCtx->ocarinaAction - 1), 2);
}

void Message_SpawnSongEffect(PlayState* play) {
    MessageContext* msgCtx = &play->msgCtx;
    Player* player = GET_PLAYER(play);

    if (1) {}
    if ((msgCtx->songPlayed <= OCARINA_SONG_SCARECROW_SPAWN) &&
        (msgCtx->songPlayed != OCARINA_SONG_GORON_LULLABY_INTRO) &&
        ((msgCtx->ocarinaAction < 0x43) || (msgCtx->ocarinaAction >= 0x47))) {
        msgCtx->unk120B0 = true;
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

    if ((play->msgCtx.msgMode >= 0xD) && (msgCtx->msgMode < 0x41)) {
        if ((msgCtx->ocarinaAction != 1) && (msgCtx->ocarinaAction != 0x38)) {
            func_8012C680(&gfx);

            gDPSetCombineLERP(gfx++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0, PRIMITIVE,
                              ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);

            if ((msgCtx->msgMode == 0x1C) || (msgCtx->msgMode == 0x36)) {
                if ((msgCtx->ocarinaAction != 0x41) && (msgCtx->ocarinaAction != 0x42)) {
                    if (msgCtx->ocarinaAction >= 0x47) {
                        sOcarinaButtonStepG = msgCtx->ocarinaAction - 0x47;
                    } else if ((msgCtx->ocarinaAction == 0x41) || (msgCtx->ocarinaAction == 0x42)) {
                        sOcarinaButtonStepG = msgCtx->ocarinaAction - 0x2E;
                    } else if ((msgCtx->ocarinaAction >= 0x43) && (msgCtx->ocarinaAction < 0x47)) {
                        sOcarinaButtonStepG = msgCtx->ocarinaAction - 0x34;
                    } else {
                        sOcarinaButtonStepG = msgCtx->ocarinaAction - 0x12;
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

            if (msgCtx->msgMode != 0x27) {
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

void func_80153E7C(PlayState* play, void* arg1) {
    if ((gSaveContext.options.language == 0) && (play->msgCtx.textIsCredits == 0)) {
        Message_DrawText(play, arg1);
        return;
    }
    if (play->msgCtx.textIsCredits != 0) {
        func_8015E7EC(play, arg1);
        return;
    }
    func_8015966C(play, arg1, 0);
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
    gDPSetPrimColor(gfx++, 0, 0, 0, 0, 0, msgCtx->textboxColorAlphaCurrent);
    gDPSetEnvColor(gfx++, 140, 40, 160, 255);
    gDPLoadTextureBlock(gfx++, gSceneTitleCardGradientTex, G_IM_FMT_I, G_IM_SIZ_8b, 64, 1, 0, G_TX_NOMIRROR | G_TX_WRAP,
                        G_TX_NOMIRROR | G_TX_WRAP, 6, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    gSPTextureRectangle(gfx++, 0, (XREG(77) * 4), 0x0500, ((XREG(77) + XREG(76)) * 4), G_TX_RENDERTILE, 0, 0, 0x00CC,
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

extern s16 sTextboxXPositions[];
extern s16 sTextboxLowerYPositions[];
extern s16 sTextboxUpperYPositions[];
extern s16 sTextboxMidYPositions[];

#ifdef NON_EQUIVALENT
void Message_Update(PlayState* play) {
    static s16 D_801D045C[] = { 0x1B91, 0x1B90, 0x1B8F };
    static s16 D_801D0464[] = { 0x1B92, 0x1B8E };
    static u8 D_801D0468 = 0;
    MessageContext* msgCtx = &play->msgCtx;
    SramContext* sramCtx = &play->sramCtx;
    PauseContext* pauseCtx = &play->pauseCtx;
    InterfaceContext* interfaceCtx = &play->interfaceCtx;
    Input* input = CONTROLLER1(&play->state);

    s16 sp50;
    s16 sp4C;
    s16 sp4A;
    s32 sp44;
    s32 sp40;
    u16 sp3E;

    s16 averageY;
    s32 var_v1;
    u16 temp_t7;
    u16 temp_v1_2;

    play->msgCtx.unk120A4 = input->rel.stick_x;
    play->msgCtx.unk120A6 = input->rel.stick_y;

    averageY = 0;

    if (play->msgCtx.unk120A4 < -30) {
        if (msgCtx->unk120A8 == -1) {
            msgCtx->unk120AC--;
            if (msgCtx->unk120AC < 0) {
                msgCtx->unk120AC = 2;
            } else {
                msgCtx->unk120A4 = 0;
            }
        } else {
            msgCtx->unk120AC = 0xA;
            msgCtx->unk120A8 = -1;
        }
    } else if (msgCtx->unk120A4 > 30) {
        if (msgCtx->unk120A8 == 1) {
            msgCtx->unk120AC--;
            if (msgCtx->unk120AC < 0) {
                msgCtx->unk120AC = 2;
            } else {
                msgCtx->unk120A4 = 0;
            }
        } else {
            msgCtx->unk120AC = 0xA;
            msgCtx->unk120A8 = 1;
        }
    } else {
        msgCtx->unk120A8 = 0;
    }

    if (msgCtx->unk120A6 < -30) {
        if (msgCtx->unk120AA == -1) {
            msgCtx->unk120AE--;
            if (msgCtx->unk120AE < 0) {
                msgCtx->unk120AE = 2;
            } else {
                msgCtx->unk120A6 = 0;
            }
        } else {
            msgCtx->unk120AE = 0xA;
            msgCtx->unk120AA = -1;
        }
    } else if (msgCtx->unk120A6 > 30) {
        if (msgCtx->unk120AA == 1) {
            msgCtx->unk120AE--;
            if (msgCtx->unk120AE < 0) {
                msgCtx->unk120AE = 2;
            } else {
                msgCtx->unk120A6 = 0;
            }
        } else {
            msgCtx->unk120AE = 0xA;
            msgCtx->unk120AA = 1;
        }
    } else {
        msgCtx->unk120AA = 0;
    }

    if (msgCtx->msgLength == 0) {
        return;
    }

    switch (msgCtx->msgMode) {
        case 0x1: // MSGMODE_TEXT_START
            D_801C6A70++;
            var_v1 = 0;
            if ((D_801C6A70 >= 4) || ((msgCtx->talkActor == NULL) && (D_801C6A70 >= 2))) {
                var_v1 = 1;
            }
            if (var_v1) {
                if (msgCtx->talkActor != NULL) {
                    Actor_GetScreenPos(play, &GET_PLAYER(play)->actor, &sp50, &sp4C);
                    Actor_GetScreenPos(play, msgCtx->talkActor, &sp50, &sp4A);
                    if (sp4C >= sp4A) {
                        averageY = ((s32)(sp4C - sp4A) / 2) + sp4A;
                    } else {
                        averageY = ((s32)(sp4A - sp4C) / 2) + sp4C;
                    }
                } else {
                    msgCtx->textboxX = msgCtx->textboxXTarget;
                    msgCtx->textboxY = msgCtx->textboxYTarget;
                }

                var_v1 = msgCtx->textBoxType;

                if ((u32)msgCtx->textBoxPos == 0) {
                    if ((play->sceneId == 4) || (play->sceneId == 5) || (play->sceneId == 6)) {
                        if (averageY < 0x64) {
                            msgCtx->textboxYTarget = sTextboxLowerYPositions[var_v1];
                        } else {
                            msgCtx->textboxYTarget = sTextboxUpperYPositions[var_v1];
                        }
                    } else {
                        if (averageY < 0xA0) {
                            msgCtx->textboxYTarget = sTextboxLowerYPositions[var_v1];
                        } else {
                            msgCtx->textboxYTarget = sTextboxUpperYPositions[var_v1];
                        }
                    }
                } else if (msgCtx->textBoxPos == 1) {
                    msgCtx->textboxYTarget = sTextboxUpperYPositions[var_v1];
                } else if (msgCtx->textBoxPos == 2) {
                    msgCtx->textboxYTarget = sTextboxMidYPositions[var_v1];
                } else if (msgCtx->textBoxPos == 7) {
                    msgCtx->textboxYTarget = 0x9E;
                } else {
                    msgCtx->textboxYTarget = sTextboxLowerYPositions[var_v1];
                }

                msgCtx->textboxXTarget = sTextboxXPositions[var_v1];

                if ((gSaveContext.options.language == 0) && (msgCtx->textIsCredits == 0)) {
                    msgCtx->unk11FFE[0] = (s16)(msgCtx->textboxYTarget + 7);
                    msgCtx->unk11FFE[1] = (s16)(msgCtx->textboxYTarget + 0x19);
                    msgCtx->unk11FFE[2] = (s16)(msgCtx->textboxYTarget + 0x2B);
                } else {
                    msgCtx->unk11FFE[0] = (s16)(msgCtx->textboxYTarget + 0x14);
                    msgCtx->unk11FFE[1] = (s16)(msgCtx->textboxYTarget + 0x20);
                    msgCtx->unk11FFE[2] = (s16)(msgCtx->textboxYTarget + 0x2C);
                }

                if ((msgCtx->textBoxType == 4) || (msgCtx->textBoxType == 5)) {
                    msgCtx->msgMode = 3;
                    msgCtx->textboxX = msgCtx->textboxXTarget;
                    msgCtx->textboxY = msgCtx->textboxYTarget;
                    msgCtx->unk12008 = 0x100;
                    msgCtx->unk1200A = 0x40;
                    msgCtx->unk1200C = 0x200;
                    msgCtx->unk1200E = 0x200;
                    break;
                }

                Message_GrowTextbox(play);
                func_801A3FB4(0);
                msgCtx->stateTimer = 0;
                msgCtx->msgMode = 2;

                if (!pauseCtx->itemDescriptionOn) {
                    func_80150A84(play);
                }
            }
            break;

        case 0x2: // MSGMODE_TEXT_BOX_GROWING
            Message_GrowTextbox(play);
            break;

        case 0x3: // MSGMODE_TEXT_STARTING
            msgCtx->msgMode = 4;
            if (!pauseCtx->itemDescriptionOn) {
                if (msgCtx->currentTextId == 0xFF) {
                    func_8011552C(play, 0x12);
                } else if (msgCtx->currentTextId != 0xF8) {
                    func_8011552C(play, 0x10);
                }
            }
            break;

        case 0x4: // MSGMODE_TEXT_NEXT_MSG
            Message_Decode(play);
            if (msgCtx->textFade) {
                Interface_SetHudVisibility(1);
            }
            if (D_801D0468 != 0) {
                msgCtx->textDrawPos = msgCtx->decodedTextLen;
                D_801D0468 = 0;
            }
            break;

        case 0x5: // MSGMODE_TEXT_CONTINUING
            msgCtx->stateTimer--;
            if (msgCtx->stateTimer == 0) {
                Message_Decode(play);
            }
            break;

        case 0x6: // MSGMODE_TEXT_DISPLAYING
            if (msgCtx->textBoxType != 4) {
                if (CHECK_BTN_ALL(input->press.button, BTN_B) && !msgCtx->textUnskippable) {
                    msgCtx->unk12094 = 1;
                    msgCtx->textDrawPos = msgCtx->decodedTextLen;
                } else if (CHECK_BTN_ALL(input->press.button, BTN_A) && !msgCtx->textUnskippable) {

                    while (true) {
                        temp_v1_2 = msgCtx->decodedBuffer.wchar[msgCtx->textDrawPos];
                        if ((temp_v1_2 == 0x10) || (temp_v1_2 == 0x12) || (temp_v1_2 == 0x1B) || (temp_v1_2 == 0x1C) ||
                            (temp_v1_2 == 0x1D) || (temp_v1_2 == 0x19) || (temp_v1_2 == 0xE0) || (temp_v1_2 == 0xBF) ||
                            (temp_v1_2 == 0x15) || (temp_v1_2 == 0x1A)) {
                            break;
                        }
                        msgCtx->textDrawPos++;
                    }
                }
            } else if (CHECK_BTN_ALL(input->press.button, BTN_A) && (msgCtx->textUnskippable == 0)) {
                while (true) {
                    temp_v1_2 = msgCtx->decodedBuffer.wchar[msgCtx->textDrawPos];
                    if ((temp_v1_2 == 0x10) || (temp_v1_2 == 0x12) || (temp_v1_2 == 0x1B) || (temp_v1_2 == 0x1C) ||
                        (temp_v1_2 == 0x1D) || (temp_v1_2 == 0x19) || (temp_v1_2 == 0xE0) || (temp_v1_2 == 0xBF) ||
                        (temp_v1_2 == 0x15) || (temp_v1_2 == 0x1A)) {
                        break;
                    }
                    msgCtx->textDrawPos++;
                }
            }
            break;

        case 0x7: // MSGMODE_TEXT_AWAIT_INPUT
            if (Message_ShouldAdvance(play)) {
                msgCtx->msgMode = 6;
                msgCtx->textDrawPos++;
            }
            break;

        case 0x8: // MSGMODE_TEXT_DELAYED_BREAK
            msgCtx->stateTimer--;
            if (msgCtx->stateTimer == 0) {
                msgCtx->msgMode = 4;
            }
            break;

        case 0x41: // MSGMODE_TEXT_AWAIT_NEXT
            if (Message_ShouldAdvance(play)) {
                msgCtx->msgMode = 4;
                msgCtx->msgBufPos++;
            }
            break;

        case 0x42: // MSGMODE_TEXT_DONE
            if ((msgCtx->unk12020 == 0x50) || (msgCtx->unk12020 == 0x52)) {
                msgCtx->stateTimer--;
                if ((msgCtx->stateTimer == 0) || ((msgCtx->unk12020 == 0x52) && Message_ShouldAdvance(play))) {
                    if (msgCtx->unk11F14 != 0xFFFF) {
                        play_sound(0x4818);
                        Message_ContinueTextbox(play, msgCtx->unk11F14);
                    } else if (msgCtx->unk120B1 != 0) {
                        if (func_80151C9C(play) == 0) {
                            Message_CloseTextbox(play);
                        }
                    } else {
                        Message_CloseTextbox(play);
                    }
                }
            } else {
                switch (msgCtx->unk12020) {
                    case 0x55:
                        msgCtx->unk1201E = msgCtx->unk1201E + 0x14;
                        if (msgCtx->unk1201E >= 0xFF) {
                            msgCtx->unk1201E = 0xFF;
                            msgCtx->unk12020 = 0x56;
                        }
                        break;

                    case 0x56:
                        msgCtx->stateTimer--;
                        if (msgCtx->stateTimer == 0) {
                            msgCtx->unk12020 = 0x57;
                        }
                        break;

                    case 0x57:
                        msgCtx->unk1201E = msgCtx->unk1201E - 0x14;
                        if (msgCtx->unk1201E <= 0) {
                            msgCtx->unk1201E = 0;
                            if (msgCtx->unk11F14 != 0xFFFF) {
                                play_sound(0x4818);
                                Message_ContinueTextbox(play, msgCtx->unk11F14);
                                return;
                            }
                            if (msgCtx->unk120B1 != 0) {
                                if (func_80151C9C(play) == 0) {
                                    Message_CloseTextbox(play);
                                    return;
                                }
                            } else {
                                Message_CloseTextbox(play);
                                return;
                            }
                        }
                        break;

                    case 0x10:
                        func_80148B98(play, 1);
                        break;

                    case 0x11:
                        func_80148B98(play, 2);
                        break;

                    case 0x12:
                        func_80148B98(play, 1);
                        break;

                    // case 0x30:
                    //     return;
                    // case 0x40:
                    //     return;
                    // case 0x42:
                    //     return;
                    // case 0x41:
                    //     return;
                    default:
                        break;
                }

                if ((msgCtx->unk12020 == 0x10) && (play->msgCtx.ocarinaMode == 1)) {
                    if (Message_ShouldAdvance(play)) {
                        if (msgCtx->choiceIndex == 0) {
                            play->msgCtx.ocarinaMode = 2;
                        } else {
                            play->msgCtx.ocarinaMode = 4;
                        }
                        Message_CloseTextbox(play);
                    }
                } else if ((msgCtx->unk12020 == 0x10) && (play->msgCtx.ocarinaMode == 0x12)) {
                    if (Message_ShouldAdvance(play)) {
                        if (msgCtx->choiceIndex == 0) {
                            func_8019F208();
                            msgCtx->msgMode = 0x4A;
                            msgCtx->decodedTextLen -= 3;
                            msgCtx->unk120D6 = 0;
                            msgCtx->unk120D4 = 0;
                        } else {
                            func_8019F230();
                            play->msgCtx.ocarinaMode = 4;
                            Message_CloseTextbox(play);
                        }
                    }
                } else if ((msgCtx->unk12020 == 0x10) && (play->msgCtx.ocarinaMode == 0x13)) {
                    if (Message_ShouldAdvance(play)) {
                        if (msgCtx->choiceIndex == 0) {
                            func_8019F208();
                            if (gSaveContext.save.timeSpeedOffset == 0) {
                                play->msgCtx.ocarinaMode = 0x19;
                                gSaveContext.save.timeSpeedOffset = -2;
                            } else {
                                play->msgCtx.ocarinaMode = 0x18;
                                gSaveContext.save.timeSpeedOffset = 0;
                            }
                            Message_CloseTextbox(play);
                        } else {
                            func_8019F230();
                            play->msgCtx.ocarinaMode = 4;
                            Message_CloseTextbox(play);
                        }
                    }
                } else if ((msgCtx->unk12020 == 0x10) && (play->msgCtx.ocarinaMode == 0x15)) {
                    if (Message_ShouldAdvance(play)) {
                        if (msgCtx->choiceIndex == 0) {
                            func_8019F208();
                            if (gSaveContext.save.isNight != 0) {
                                gSaveContext.save.time = 0x4000;
                            } else {
                                gSaveContext.save.time = 0xC000;
                            }
                            play->msgCtx.ocarinaMode = 0x1A;
                            gSaveContext.timerStates[3] = 0;
                        } else {
                            func_8019F230();
                            play->msgCtx.ocarinaMode = 4;
                        }
                        Message_CloseTextbox(play);
                    }
                } else if ((msgCtx->unk12020 != 0x10) || (pauseCtx->state != 0x18)) {
                    if ((msgCtx->unk12020 == 0x10) && (play->msgCtx.ocarinaMode == 0x1B)) {
                        if (Message_ShouldAdvance(play)) {
                            if (msgCtx->choiceIndex == 0) {
                                func_8019F208();
                                play->msgCtx.ocarinaMode = 0x26;
                            } else {
                                func_8019F230();
                                play->msgCtx.ocarinaMode = 4;
                            }
                            Message_CloseTextbox(play);
                        }
                    } else if ((msgCtx->unk12020 == 0x60) || (msgCtx->unk12020 == 0x61) || (msgCtx->unk12020 == 0x10) ||
                               (msgCtx->unk12020 == 0x11) || (msgCtx->unk12020 == 0x50) || (msgCtx->unk12020 == 0x52) ||
                               (msgCtx->unk12020 == 0x55) || (msgCtx->unk12020 == 0x56) || (msgCtx->unk12020 == 0x57) ||
                               (msgCtx->unk12020 == 0x62)) {
                        if (msgCtx->unk12020 == 0x50) {}
                    } else if (pauseCtx->itemDescriptionOn) {
                        if ((input->rel.stick_x != 0) || (input->rel.stick_y != 0) ||
                            CHECK_BTN_ALL(input->press.button, BTN_A) || CHECK_BTN_ALL(input->press.button, BTN_B) ||
                            CHECK_BTN_ALL(input->press.button, BTN_START)) {
                            play_sound(0x4808);
                            Message_CloseTextbox(play);
                        }
                    } else if (play->msgCtx.ocarinaMode == 0x27) {
                        if (Message_ShouldAdvanceSilent(play) != 0) {
                            play_sound(0x4808);
                            Message_CloseTextbox(play);
                            play->msgCtx.ocarinaMode = 4;
                        }
                    } else if ((msgCtx->currentTextId != 0x2790) && Message_ShouldAdvanceSilent(play)) {
                        if (msgCtx->unk11F14 != 0xFFFF) {
                            play_sound(0x4818);
                            Message_ContinueTextbox(play, msgCtx->unk11F14);
                        } else if ((msgCtx->unk120B1 == 0) || (func_80151C9C(play) != 1)) {
                            if (msgCtx->currentTextId == 0x579) {
                                gSaveContext.hudVisibility = 0;
                            }
                            play_sound(0x4808);
                            Message_CloseTextbox(play);
                        }
                    }
                }
            }
            break;

        case 0x43: // MSGMODE_TEXT_CLOSING
            msgCtx->stateTimer--;
            if (msgCtx->stateTimer != 0) {
                break;
            }

            if (D_801C6A7C == 9) {
                if (interfaceCtx->restrictions.songOfSoaring == 0) {
                    if ((func_8010A0A4(play) != 0) || (play->sceneId == 0x4F)) {
                        Message_StartTextbox(play, 0x1B93U, NULL);
                        play->msgCtx.ocarinaMode = 0x1B;
                        D_801C6A7C = 0xFF;
                    } else if (msgCtx->unk120B0 == 0) {
                        if (gSaveContext.save.playerData.owlActivationFlags != 0) {
                            pauseCtx->unk_2C8 = pauseCtx->pageIndex;
                            pauseCtx->unk_2CA = pauseCtx->cursorPoint[4];
                            pauseCtx->pageIndex = 0;
                            pauseCtx->state = 0x13;
                            func_800F4A10(play);
                            pauseCtx->pageIndex = 1;
                            D_801C6A7C = 0xFF;
                            Message_CloseTextbox(play);
                            play->msgCtx.ocarinaMode = 4;
                            gSaveContext.prevHudVisibility = 0x15;
                            func_80115844(play, 0x12);
                            Game_SetFramerateDivisor(&play->state, 2);
                            if (ShrinkWindow_Letterbox_GetSizeTarget() != 0) {
                                ShrinkWindow_Letterbox_SetSizeTarget(0);
                            }
                            func_801A3AEC(1);
                            break;
                        }
                        D_801C6A7C = 0xFF;
                        Message_StartTextbox(play, 0xFBU, NULL);
                        play->msgCtx.ocarinaMode = 0x27;
                    } else {
                        msgCtx->stateTimer = 1;
                    }
                } else {
                    D_801C6A7C = 0xFF;
                    Message_StartTextbox(play, 0x1B95U, NULL);
                    play->msgCtx.ocarinaMode = 0x27;
                }
                break;
            }

            if ((msgCtx->currentTextId == 0xC) || (msgCtx->currentTextId == 0xD) || (msgCtx->currentTextId == 0xC5) ||
                (msgCtx->currentTextId == 0xC6) || (msgCtx->currentTextId == 0xC7) ||
                (msgCtx->currentTextId == 0x2165) || (msgCtx->currentTextId == 0x2166) ||
                (msgCtx->currentTextId == 0x2167) || (msgCtx->currentTextId == 0x2168)) {
                gSaveContext.healthAccumulator = 0x140; // Refill 20 hearts
            }

            if ((play->csCtx.state == 0) && (gSaveContext.save.cutscene < 0xFFF0) &&
                ((play->activeCamId == 0) || ((play->transitionTrigger == 0) && (play->transitionMode == 0))) &&
                (play->msgCtx.ocarinaMode == 4)) {
                if ((gSaveContext.prevHudVisibility == 0) || (gSaveContext.prevHudVisibility == 1) ||
                    (gSaveContext.prevHudVisibility == 2)) {
                    gSaveContext.prevHudVisibility = 0x32;
                }
                gSaveContext.hudVisibility = 0;
            }

            if ((msgCtx->currentTextId >= 0x1BB2) && (msgCtx->currentTextId < 0x1BB7) && (play->actorCtx.flags & 2)) {
                Message_StartTextbox(play, 0x5E6U, NULL);
            } else if (msgCtx->unk120B1 != 0) {
                if (func_80151C9C(play) == 0) {
                    msgCtx->stateTimer = 1;
                }
            } else {
                msgCtx->msgLength = 0;
                msgCtx->msgMode = 0;
                msgCtx->currentTextId = 0;
                msgCtx->stateTimer = 0;
                gGameInfo->data[0x55F] = 0;
                if (pauseCtx->itemDescriptionOn) {
                    func_8011552C(play, 0x15);
                    pauseCtx->itemDescriptionOn = false;
                }
                if (msgCtx->unk12020 == 0x30) {
                    msgCtx->unk12020 = 0;
                    play->msgCtx.ocarinaMode = 2;
                } else {
                    msgCtx->unk12020 = 0;
                }

                if (EQ_MAX_QUEST_HEART_PIECE_COUNT) {
                    RESET_HEART_PIECE_COUNT;
                    gSaveContext.save.playerData.healthCapacity += 0x10;
                    gSaveContext.save.playerData.health += 0x10;
                }

                if (msgCtx->ocarinaAction != 0x39) {
                    if (D_801C6A7C == 6) {
                        if (interfaceCtx->restrictions.unk_312 == 0) {
                            Message_StartTextbox(play, 0x1B8AU, NULL);
                            play->msgCtx.ocarinaMode = 0x12;
                        } else {
                            D_801C6A7C = 0xFF;
                            Message_StartTextbox(play, 0x1B95U, NULL);
                            play->msgCtx.ocarinaMode = 0x27;
                        }
                    } else if (D_801C6A7C == 0xC) {
                        if (interfaceCtx->restrictions.unk_314 == 0) {
                            if (gGameInfo->data[0xF] != 0) {
                                if (gSaveContext.save.timeSpeedOffset == 0) {
                                    Message_StartTextbox(play, 0x1B8CU, NULL);
                                } else {
                                    Message_StartTextbox(play, 0x1B8DU, NULL);
                                }
                                play->msgCtx.ocarinaMode = 0x13;
                            } else {
                                Message_StartTextbox(play, 0x1B8BU, NULL);
                                play->msgCtx.ocarinaMode = 4;
                            }
                        } else {
                            D_801C6A7C = 0xFF;
                            Message_StartTextbox(play, 0x1B95U, NULL);
                            play->msgCtx.ocarinaMode = 0x27;
                        }
                    } else if (D_801C6A7C == 0xD) {
                        if (interfaceCtx->restrictions.unk_313 == 0) {
                            if ((CURRENT_DAY != 3) || (gSaveContext.save.isNight == 0)) {
                                if (gSaveContext.save.isNight != 0) {
                                    Message_StartTextbox(play, D_801D0464[CURRENT_DAY - 1], NULL);
                                } else {
                                    Message_StartTextbox(play, D_801D045C[CURRENT_DAY - 1], NULL);
                                }
                                play->msgCtx.ocarinaMode = 0x15;
                            } else {
                                Message_StartTextbox(play, 0x1B94U, NULL);
                                play->msgCtx.ocarinaMode = 4;
                            }
                        } else {
                            D_801C6A7C = 0xFF;
                            Message_StartTextbox(play, 0x1B95U, NULL);
                            play->msgCtx.ocarinaMode = 0x27;
                        }
                    } else if ((msgCtx->ocarinaAction == 0x32) &&
                               ((play->msgCtx.ocarinaMode == 1) || (play->msgCtx.ocarinaMode == 3) ||
                                (play->msgCtx.ocarinaMode == 0xD) || (play->msgCtx.ocarinaMode == 0x2A))) {
                        play->msgCtx.ocarinaMode = 4;
                        if (msgCtx->lastPlayedSong == 9) {
                            play->msgCtx.ocarinaMode = 1;
                        }
                    }
                    D_801C6A7C = 0xFF;
                }
            }
            break;

        case 0xD:
            if (CHECK_BTN_ALL(input->press.button, BTN_B)) {
                AudioOcarina_SetInstrument(0);
                play->msgCtx.ocarinaMode = 4;
                Message_CloseTextbox(play);
            } else {
                msgCtx->unk12048 = 0xFF;
            }
            break;

        case 0x20:
            if ((msgCtx->ocarinaAction != 0x41) && (msgCtx->ocarinaAction != 0x42)) {
                if (Message_ShouldAdvance(play)) {
                    Message_StartOcarina(play, msgCtx->ocarinaAction);
                }
            }
            break;

        case 0x2B:
            if (CHECK_BTN_ALL(input->press.button, BTN_B)) {
                AudioOcarina_SetRecordingState(0);
                play_sound(0x4827);
                Message_CloseTextbox(play);
                msgCtx->msgMode = 0x2C;
            } else {
                msgCtx->unk12048 = 0xFF;
            }
            break;

        case 0x45:
            msgCtx->textboxColorAlphaCurrent += gGameInfo->data[0x589];
            if (msgCtx->textboxColorAlphaCurrent >= 0xFF) {
                msgCtx->textboxColorAlphaCurrent = 0xFF;
                msgCtx->msgMode = 0x46;
            }
            break;

        case 0x46:
            msgCtx->unk1201E += gGameInfo->data[0x589];
            if (msgCtx->unk1201E >= 0xFF) {
                msgCtx->unk1201E = 0xFF;
                msgCtx->msgMode = 0x47;
            }
            break;

        case 0x47:
            msgCtx->stateTimer--;
            if (msgCtx->stateTimer == 0) {
                msgCtx->msgMode = 0x48;
            }
            break;

        case 0x48:
            msgCtx->unk1201E -= gGameInfo->data[0x586];
            if (msgCtx->unk1201E <= 0) {
                msgCtx->unk1201E = 0;
                msgCtx->msgMode = 0x49;
            }
            break;

        case 0x49:
            msgCtx->textboxColorAlphaCurrent -= gGameInfo->data[0x586];
            if (msgCtx->textboxColorAlphaCurrent <= 0) {
                if ((msgCtx->currentTextId >= 0x1BB2) && (msgCtx->currentTextId < 0x1BB7) &&
                    (play->actorCtx.flags & 2)) {
                    Message_StartTextbox(play, 0x5E6U, NULL);
                    Interface_SetHudVisibility(2);
                } else {
                    if (msgCtx->currentTextId >= 0x100) {
                        // TODO: Need something in here
                    }
                    msgCtx->textboxColorAlphaCurrent = 0;
                    msgCtx->msgLength = 0;
                    msgCtx->msgMode = 0;
                    msgCtx->currentTextId = 0;
                    msgCtx->stateTimer = 0;
                }
            }
            break;

        case 0x4A:
            play->state.unk_A3 = 1;
            sp44 = gSaveContext.save.cutscene;
            sp3E = gSaveContext.save.time;
            sp40 = gSaveContext.save.day;

            Sram_SaveEndOfCycle(play);
            gSaveContext.timerStates[3] = 0;
            func_8014546C(sramCtx);

            gSaveContext.save.day = sp40;
            gSaveContext.save.time = sp3E;
            gSaveContext.save.cutscene = sp44;

            if (gSaveContext.fileNum != 0xFF) {
                func_80147008(sramCtx, D_801C67C8[gSaveContext.fileNum * 2], D_801C6818[gSaveContext.fileNum * 2]);
                func_80147020(sramCtx);
            }
            msgCtx->msgMode = 0x4B;
            break;

        case 0x4B:
            if (gSaveContext.fileNum != 0xFF) {
                play->state.unk_A3 = 1;
                if (play->sramCtx.status == 0) {
                    play->msgCtx.ocarinaMode = 0x16;
                    msgCtx->msgMode = 0x4C;
                }
            } else {
                play->msgCtx.ocarinaMode = 0x16;
                msgCtx->msgMode = 0x4C;
            }
            break;

        case 0x4D:
            play->state.unk_A3 = 1;
            gSaveContext.save.isOwlSave = 1;
            Play_SaveCycleSceneFlags(&play->state);
            func_8014546C(sramCtx);

            if (gSaveContext.fileNum != 0xFF) {
                func_80147138(sramCtx, D_801C6840[gSaveContext.fileNum * 2], D_801C6850[gSaveContext.fileNum * 2]);
                func_80147150(sramCtx);
            }
            msgCtx->msgMode = 0x4E;
            break;

        case 0x4E:
            if (gSaveContext.fileNum != 0xFF) {
                play->state.unk_A3 = 1;
                if (play->sramCtx.status == 0) {
                    play->msgCtx.ocarinaMode = 0x16;
                    msgCtx->msgMode = 0x4F;
                }
            } else {
                play->msgCtx.ocarinaMode = 0x16;
                msgCtx->msgMode = 0x4F;
            }

            if (msgCtx->msgMode == 0x4F) {
                gSaveContext.gameMode = 4;
                play->transitionTrigger = 0x14;
                play->transitionType = 2;
                play->nextEntrance = 0x1C00;
                gSaveContext.save.cutscene = 0;
                gSaveContext.sceneLayer = 0;
            }
            break;

        default:
            msgCtx->unk12048 = 0xFF;
            break;
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/z_message/Message_Update.s")
#endif

void Message_SetTables(PlayState* play) {
    play->msgCtx.messageEntryTableNes = D_801C6B98;
    play->msgCtx.messageTableStaff = D_801CFB08;
}

void Message_Init(PlayState* play) {
    Font* font;
    MessageContext* messageCtx = &play->msgCtx;

    Message_SetTables(play);

    play->msgCtx.ocarinaMode = 0;

    messageCtx->msgMode = 0;
    messageCtx->msgLength = 0;
    messageCtx->currentTextId = 0;
    messageCtx->unk12020 = 0;
    messageCtx->choiceIndex = 0;
    messageCtx->ocarinaAction = messageCtx->textUnskippable = 0;
    messageCtx->unk1201E = 0xFF;

    View_Init(&messageCtx->view, play->state.gfxCtx);

    messageCtx->textboxSegment = THA_AllocEndAlign16(&play->state.heap, 0x13C00);

    font = &play->msgCtx.font;
    Font_LoadOrderedFont(&play->msgCtx.font);
    font->unk_11D88 = 0;

    messageCtx->textIsCredits = messageCtx->unk12092 = 0;
    messageCtx->unk12094 = 0;
    messageCtx->textFade = false;
    messageCtx->ocarinaAvailableSongs = 0;
    messageCtx->textboxX = 0x34;
    messageCtx->textboxY = 0x24;
    messageCtx->unk120B0 = false;
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
