#include "global.h"

extern u8 sEnvIsTimeStopped;
extern Gfx D_0E0002C8[];

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800F5090.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800F50D4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Kankyo_Init.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800F5954.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_LerpWeight.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800F5B10.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800F5CD0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800F6834.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800F694C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800F6A04.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800F6A40.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800F6AB8.s")

void Environment_UpdateTime(PlayState* play, EnvironmentContext* envCtx, PauseContext* pauseCtx, MessageContext* msgCtx,
                            GameOverContext* gameOverCtx) {
    u16 time;

    if (!sEnvIsTimeStopped && (pauseCtx->state == 0) && (gameOverCtx->state == GAMEOVER_INACTIVE)) {
        if ((msgCtx->msgMode == 0) || (msgCtx->currentTextId == 0xF7) || (msgCtx->currentTextId == 0x20D2) ||
            (msgCtx->currentTextId == 0x140C) ||
            ((msgCtx->currentTextId >= 0x100) && (msgCtx->currentTextId <= 0x200)) ||
            (((void)0, gSaveContext.gameMode) == 3)) {
            if (!FrameAdvance_IsEnabled(&play->state) &&
                ((play->transitionMode == TRANS_MODE_OFF) || (((void)0, gSaveContext.gameMode) != 0)) &&
                (play->transitionTrigger == TRANS_TRIGGER_OFF)) {
                if ((ActorCutscene_GetCurrentIndex() == -1) && !Play_InCsMode(play)) {
                    gSaveContext.save.time = ((void)0, gSaveContext.save.time) + (u16)R_TIME_SPEED;
                    if (R_TIME_SPEED != 0) {
                        gSaveContext.save.time =
                            ((void)0, gSaveContext.save.time) + (u16)((void)0, gSaveContext.save.timeSpeedOffset);
                    }
                }
            }
        }
    }
    if ((((void)0, gSaveContext.skyboxTime) >= CLOCK_TIME(6, 0)) ||
        (((void)0, gSaveContext.save.time) < CLOCK_TIME(6, 0)) ||
        (((void)0, gSaveContext.save.time) >= (CLOCK_TIME(6, 0) + 0x10))) {
        gSaveContext.skyboxTime = gSaveContext.save.time;
    }

    time = ((void)0, gSaveContext.save.time);

    if ((time >= CLOCK_TIME(18, 0)) || (time < CLOCK_TIME(6, 0))) {
        gSaveContext.save.isNight = 1;
    } else {
        gSaveContext.save.isNight = 0;
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800F6CEC.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800F6EA4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Kankyo_IsSceneUpsideDown.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800F6FF8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800F8554.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800F88C4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800F8970.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800F8A9C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800F8CD4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800F8D84.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800F9728.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800F9824.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FA39C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FA3C4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FA9FC.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FAAB4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FAC20.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FAF74.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FB010.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_AddLightningBolts.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FB388.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FB758.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FB9B4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FBCBC.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Kankyo_InitGameOverLights.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Kankyo_FadeInGameOverLights.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Kankyo_FadeOutGameOverLights.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FC3DC.s")

void Environment_FillScreen(GraphicsContext* gfxCtx, u8 red, u8 green, u8 blue, u8 alpha, u8 drawFlags) {
    if (alpha != 0) {
        OPEN_DISPS(gfxCtx);

        if (drawFlags & FILL_SCREEN_OPA) {
            POLY_OPA_DISP = func_8012BFC4(POLY_OPA_DISP);
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, red, green, blue, alpha);
            gDPSetAlphaDither(POLY_OPA_DISP++, G_AD_DISABLE);
            gDPSetColorDither(POLY_OPA_DISP++, G_CD_DISABLE);
            gSPDisplayList(POLY_OPA_DISP++, D_0E0002C8);
        }

        if (drawFlags & FILL_SCREEN_XLU) {
            POLY_XLU_DISP = func_8012BFC4(POLY_XLU_DISP);
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, red, green, blue, alpha);

            if ((u32)alpha == 255) {
                gDPSetRenderMode(POLY_XLU_DISP++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
            }

            gDPSetAlphaDither(POLY_XLU_DISP++, G_AD_DISABLE);
            gDPSetColorDither(POLY_XLU_DISP++, G_CD_DISABLE);
            gSPDisplayList(POLY_XLU_DISP++, D_0E0002C8);
        }

        CLOSE_DISPS(gfxCtx);
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FC64C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FD2B4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FD538.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FD59C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FD5E0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FD654.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FD698.s")

s32 Environment_GetBgsDayCount(void) {
    return gSaveContext.save.daysElapsed;
}

void Environment_ClearBgsDayCount(void) {
    gSaveContext.save.daysElapsed = 0;
}

s32 Environment_GetTotalDays(void) {
    return gSaveContext.save.day;
}

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FD750.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FD768.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FD78C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FD858.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FD928.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FD980.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FDAF8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FDC94.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FE390.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FE3E0.s")

void Environment_StopTime(void) {
    sEnvIsTimeStopped = true;
}

void Environment_StartTime(void) {
    sEnvIsTimeStopped = false;
}

u8 Environment_IsTimeStopped(void) {
    return sEnvIsTimeStopped;
}

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FE4B8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FE590.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FE5D0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FE610.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FE620.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FE658.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FE6F8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FE778.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FE788.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FE798.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FE7A8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FE9B4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FEA50.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FEAB0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FEAC0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FEAF4.s")
