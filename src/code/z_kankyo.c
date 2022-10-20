#include "global.h"

extern u8 sEnvIsTimeStopped;
extern f32 D_801BDB90;
extern u16 D_801BDB9C;
extern u8 D_801BDBA0;
extern u8 D_801BDBA4;
extern Gfx D_0E0002C8[];
extern u8 D_801F4F30;
extern u8 D_801F4F31;
extern u8 D_801F4F32;
extern u8 D_801F4F33;
extern u8 D_801F4F34;
extern Gfx* D_801F4F38;

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_GraphCallback.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_ZBufValToFixedPoint.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_Init.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_SmoothStepToU8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_LerpWeight.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_LerpWeightAccelDecel.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_UpdateSkybox.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_EnableUnderwaterLights.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_DisableUnderwaterLights.s")

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

s32 Environment_IsSceneUpsideDown(PlayState* play) {
    s32 ret = false;

    if ((play->sceneId == SCENE_F41) || (play->sceneId == SCENE_INISIE_R)) {
        ret = true;
    }
    return ret;
}

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800F6FF8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800F8554.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800F88C4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800F8970.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800F8A9C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_Update.s")

// Environment_DrawSunAndMoon? Maybe just "Majora's Moon"?
void func_800F8D84(PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800F8D84.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_DrawSunLensFlare.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_DrawLensFlare.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_RandCentered.s")

// Environment_DrawRainImpl?
#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FA3C4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_DrawRain.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_ChangeLightSetting.s")

void Environment_DrawSkyboxFilters(PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_DrawSkyboxFilters.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_DrawLightningFlash.s")

void Environment_UpdateLightningStrike(PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_UpdateLightningStrike.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_AddLightningBolts.s")

void Environment_DrawLightning(PlayState* play, s32 unused);
#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_DrawLightning.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_PlaySceneSequence.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_PlayTimeBasedSequence.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_DrawCustomLensFlare.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_InitGameOverLights.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_FadeInGameOverLights.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_FadeOutGameOverLights.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_UpdateRain.s")

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

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_DrawSandstorm.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_AdjustLights.s")

// Get ((to - from) * lerp)
void func_800FD538(Color_RGB8* from, Color_RGB8* to, f32 lerp, Vec3s* dst) {
    Color_RGB8 result;

    Color_RGB8_Lerp(from, to, lerp, &result);

    dst->x = result.r - from->r;
    dst->y = result.g - from->g;
    dst->z = result.b - from->b;
}

void func_800FD59C(PlayState* play, Color_RGB8* to, f32 lerp) {
    func_800FD538((Color_RGB8*)play->envCtx.unk_C4.ambientColor, to, lerp,
                  (Vec3s*)&play->envCtx.lightSettings.ambientColor);
}

void func_800FD5E0(PlayState* play, Color_RGB8* to, f32 lerp) {
    func_800FD538((Color_RGB8*)play->envCtx.unk_C4.diffuseColor1, to, lerp,
                  (Vec3s*)play->envCtx.lightSettings.diffuseColor1);
    func_800FD538((Color_RGB8*)play->envCtx.unk_C4.diffuseColor, to, lerp,
                  (Vec3s*)play->envCtx.lightSettings.diffuseColor2);
}

void func_800FD654(PlayState* play, Color_RGB8* to, f32 lerp) {
    func_800FD538((Color_RGB8*)play->envCtx.unk_C4.fogColor, to, lerp, (Vec3s*)play->envCtx.lightSettings.fogColor);
}

void func_800FD698(PlayState* play, s16 arg1, s16 arg2, f32 arg3) {
    play->envCtx.lightSettings.fogNear = (arg1 - (s16)play->envCtx.unk_C4.fogNear) * arg3;
    play->envCtx.lightSettings.fogFar = (arg2 - (s16)play->envCtx.unk_C4.fogFar) * arg3;
}

s32 Environment_GetBgsDayCount(void) {
    return gSaveContext.save.daysElapsed;
}

void Environment_ClearBgsDayCount(void) {
    gSaveContext.save.daysElapsed = 0;
}

s32 Environment_GetTotalDays(void) {
    return gSaveContext.save.day;
}

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_ForcePlaySequence.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_IsForcedSequenceDisabled.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_PlayStormNatureAmbience.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_StopStormNatureAmbience.s")

void Environment_WarpSongLeave(PlayState* play) {
    gWeatherMode = 0;
    gSaveContext.save.cutscene = 0;
    gSaveContext.respawnFlag = -3;
    play->nextEntrance = gSaveContext.respawn[RESPAWN_MODE_RETURN].entrance;
    play->transitionTrigger = TRANS_TRIGGER_START;
    play->transitionType = TRANS_TYPE_03;
    gSaveContext.nextTransitionType = TRANS_TYPE_03;
}

extern s32 sEnvSkyboxNumStars;
extern f32 D_801F4F28;

void func_800FD980(PlayState* play) {
    f32 phi_f0;

    if ((play->envCtx.unk_10 == 0) && (play->skyboxId == 1)) {
        if ((((void)0, gSaveContext.save.time) >= CLOCK_TIME(21, 0)) ||
            (((void)0, gSaveContext.save.time) < CLOCK_TIME(2, 0))) {
            phi_f0 = 1.0f;
        } else if (((void)0, gSaveContext.save.time) > CLOCK_TIME(19, 0)) {
            phi_f0 = 1.0f - ((CLOCK_TIME(21, 0) - ((void)0, gSaveContext.save.time)) * (1.0f / (CLOCK_TIME(2, 0) + 1)));
        } else if (((void)0, gSaveContext.save.time) < CLOCK_TIME(3, 0)) {
            phi_f0 = (CLOCK_TIME(3, 0) - ((void)0, gSaveContext.save.time)) * (1.0f / (CLOCK_TIME(1, 0) + 1));
        } else {
            phi_f0 = 0.0f;
        }

        phi_f0 = (play->envCtx.unk_17 == 0x18) ? 1.0f : phi_f0;

        D_801F4F28 = phi_f0;
        sEnvSkyboxNumStars = gSkyboxNumStars;
    } else {
        D_801F4F28 = 0.0f;
        sEnvSkyboxNumStars = 0;
    }

    if ((sEnvSkyboxNumStars != 0) && (D_801F4F28 != 0.0f)) {
        OPEN_DISPS(play->state.gfxCtx);

        D_801F4F38 = POLY_OPA_DISP;

        gSPNoOp(POLY_OPA_DISP++);

        CLOSE_DISPS(play->state.gfxCtx);
    } else {
        D_801F4F38 = NULL;
    }
}

void Environment_DrawSkyboxStar(Gfx** gfxp, f32 x, f32 y, s32 width, s32 height) {
    Gfx* gfx = *gfxp;
    u32 xl = x * 4.0f;
    u32 yl = y * 4.0f;
    u32 xd = width;
    u32 yd = height;

    gSPTextureRectangle(gfx++, xl, yl, xl + xd, yl + yd, 0, 0, 0, 0, 0);

    *gfxp = gfx;
}

extern Vec3s D_801DD880[];
extern u32 D_801DD8E0[8];
extern u32 D_801DD900[16];

// Environment_DrawSkyboxStars
#ifdef NON_EQUIVALENT
void Environment_DrawSkyboxStars(PlayState* play, Gfx** gfxP) {
    Vec3f pos;
    f32 imgY;
    f32 imgX;
    s32 negateY;
    Gfx* gfx;
    Gfx* gfxTemp;
    MtxF* viewProjectionMtxF;
    f32 invScale;
    f32 temp_f20;
    f32 temp_f22;
    f32 temp_f2;
    f32 scale;
    f32 temp_f4;
    s32 i;
    u32 randInt;
    s32 phi_v1; // spE8
    u32 imgWidth;
    f32* imgXPtr;
    f32* imgYPtr;
    s32 pad[4];

    gfx = *gfxP;
    viewProjectionMtxF = &play->viewProjectionMtxF;

    //! FAKE:
    if (1) {}
    if (1) {}

    negateY = Environment_IsSceneUpsideDown(play);

    Matrix_MtxToMtxF(play->view.viewingPtr, &play->billboardMtxF);
    Matrix_MtxToMtxF(&play->view.projection, viewProjectionMtxF);
    SkinMatrix_MtxFMtxFMult(viewProjectionMtxF, &play->billboardMtxF, viewProjectionMtxF);

    phi_v1 = 0;

    gDPPipeSync(gfx++);
    gDPSetEnvColor(gfx++, 255, 255, 255, 255 * D_801F4F28);
    gDPSetCombineLERP(gfx++, PRIMITIVE, 0, ENVIRONMENT, 0, PRIMITIVE, 0, ENVIRONMENT, 0, PRIMITIVE, 0, ENVIRONMENT, 0,
                      PRIMITIVE, 0, ENVIRONMENT, 0);
    gDPSetOtherMode(gfx++,
                    G_AD_DISABLE | G_CD_DISABLE | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE |
                        G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE,
                    G_AC_NONE | G_ZS_PRIM | G_RM_AA_XLU_LINE | G_RM_AA_XLU_LINE2);

    randInt = ((gSaveContext.save.playerData.playerName[7]) << 0x1C);
    randInt ^= ((gSaveContext.save.playerData.playerName[0]) << 0x18);
    randInt ^= ((gSaveContext.save.playerData.playerName[1]) << 0x14);
    randInt ^= ((gSaveContext.save.playerData.playerName[2]) << 0x10);
    randInt ^= (0, (gSaveContext.save.playerData.playerName[3]) << 0xC); //! FAKE:
    randInt ^= ((gSaveContext.save.playerData.playerName[4]) << 8);
    randInt ^= ((gSaveContext.save.playerData.playerName[5]) << 4);
    randInt ^= ((gSaveContext.save.playerData.playerName[6]) << 0);
    randInt ^= ((gSaveContext.save.playerData.playerName[7]) >> 4);

    // randInt = ((gSaveContext.save.playerData.playerName[7]) << 0x1C) ^
    //           ((gSaveContext.save.playerData.playerName[0]) << 0x18) ^
    //           ((gSaveContext.save.playerData.playerName[1]) << 0x14) ^
    //           ((gSaveContext.save.playerData.playerName[2]) << 0x10) ^
    //           ((gSaveContext.save.playerData.playerName[3]) << 0xC) ^
    //           ((gSaveContext.save.playerData.playerName[4]) << 8) ^
    //           ((gSaveContext.save.playerData.playerName[5]) << 4) ^
    //           ((gSaveContext.save.playerData.playerName[6]) << 0) ^
    //           ((gSaveContext.save.playerData.playerName[7]) >> 4);

    //! FAKE:
    if (&gSaveContext && &gSaveContext && &gSaveContext) {}

    for (i = 0; i < sEnvSkyboxNumStars; i++) {
        if (i < 16) {
            pos.x = play->view.eye.x + D_801DD880[i].x;
            pos.y = play->view.eye.y + D_801DD880[i].y;
            pos.z = play->view.eye.z + D_801DD880[i].z;
            imgWidth = 8;
        } else {
            // temp_f4 = Rand_ZeroOne_Variable(&randInt);
            randInt = (randInt * 1664525) + 1013904223;
            sRandFloat = (randInt >> 9) | 0x3F800000;
            temp_f4 = *((f32*)&sRandFloat) - 1; //! FAKE:

            // temp_f20 = Rand_ZeroOne_Variable(&randInt);
            randInt = (randInt * 1664525) + 1013904223;
            sRandFloat = (randInt >> 9) | 0x3F800000;
            temp_f20 = *((f32*)&sRandFloat) - 1.0f;

            // average
            temp_f20 = (temp_f20 + temp_f4) * 0.5f;

            // randInt = Rand_Next_Variable(&randInt);
            randInt = (randInt * 1664525) + 1013904223;

            // Set random position
            pos.y = play->view.eye.y + (SQ(temp_f20) * SQ(128.0f)) - 1000.0f;
            pos.x = play->view.eye.x + (Math_SinS(randInt) * (1.2f - temp_f20) * SQ(128.0f));
            pos.z = play->view.eye.z + (Math_CosS(randInt) * (1.2f - temp_f20) * SQ(128.0f));

            // temp_f2 = Rand_ZeroOne_Variable(&randInt);
            randInt = (randInt * 1664525) + 1013904223;
            sRandFloat = ((randInt >> 9) | 0x3F800000);
            temp_f2 = *((f32*)&sRandFloat) - 1.0f;

            // Set random width
            imgWidth = (u32)((SQ(temp_f2) * 8.0f) + 2.0f);
        }

        if (negateY) {
            pos.y = -pos.y;
        }

        if ((i < 15) || ((i == 15) && ((((void)0, gSaveContext.save.day) % 7) == 0))) {
            gDPSetColor(gfx++, G_SETPRIMCOLOR, D_801DD8E0[i & 7]);
        } else if (((i & 0x3F) == 0) || (i == 16)) {
            gDPSetColor(gfx++, G_SETPRIMCOLOR, D_801DD900[phi_v1 & 0xF]);
            phi_v1++;
        }

        //! FAKE:
        if ((&pos && &pos && &pos) != 0) {}
        imgXPtr = &imgX;
        imgYPtr = &imgY;

        if (imgWidth >= 2) {
            // w component
            scale = pos.x * play->viewProjectionMtxF.mf[0][3] + pos.y * play->viewProjectionMtxF.mf[1][3] +
                    pos.z * play->viewProjectionMtxF.mf[2][3] + play->viewProjectionMtxF.mf[3][3];
            if (scale >= 1.0f) {
                invScale = 1.0f / scale;
                // x component
                imgX = (pos.x * viewProjectionMtxF->mf[0][0] + pos.y * viewProjectionMtxF->mf[1][0] +
                        pos.z * viewProjectionMtxF->mf[2][0] + viewProjectionMtxF->mf[3][0]) *
                       invScale;
                // y component
                imgY = (((pos.x * viewProjectionMtxF->mf[0][1]) + (pos.y * viewProjectionMtxF->mf[1][1]) +
                         (pos.z * viewProjectionMtxF->mf[2][1])) +
                        viewProjectionMtxF->mf[3][1]) *
                       invScale;
            }

            if ((scale >= 1.0f) && (imgX > -1.0f) && (imgX < 1.0f) && (imgY > -1.0f) && (imgY < 1.0f)) {
                imgX = (imgX * (SCREEN_WIDTH / 2)) + (SCREEN_WIDTH / 2);
                imgY = (imgY * -(SCREEN_HEIGHT / 2)) + (SCREEN_HEIGHT / 2);

                gfxTemp = gfx;
                Environment_DrawSkyboxStar(&gfxTemp, imgX, imgY, imgWidth, 4);
                gfx = gfxTemp;
            }
        }
    }

    gDPPipeSync(gfx++);
    *gfxP = gfx;
}
#else
void Environment_DrawSkyboxStars(PlayState* play, Gfx** gfxP);
#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_DrawSkyboxStars.s")
#endif

void func_800FE390(PlayState* play) {
    func_800FD980(play);
    func_800F8D84(play);
    Environment_UpdateLightningStrike(play);
    Environment_DrawLightning(play, 0);
    Environment_DrawSkyboxFilters(play);
}

void func_800FE3E0(PlayState* play) {
    Gfx* sp2C;
    Gfx* sp28;

    if (D_801F4F38 != NULL) {
        OPEN_DISPS(play->state.gfxCtx);

        sp28 = POLY_OPA_DISP;
        sp2C = Graph_GfxPlusOne(sp28);

        gSPDisplayList(D_801F4F38, sp2C);

        Environment_DrawSkyboxStars(play, &sp2C);

        gSPEndDisplayList(sp2C++);

        Graph_BranchDlist(sp28, sp2C);

        POLY_OPA_DISP = sp2C;
        D_801F4F38 = NULL;

        CLOSE_DISPS(play->state.gfxCtx);
    }
}

void Environment_StopTime(void) {
    sEnvIsTimeStopped = true;
}

void Environment_StartTime(void) {
    sEnvIsTimeStopped = false;
}

u8 Environment_IsTimeStopped(void) {
    return sEnvIsTimeStopped;
}

u32 func_800FE4B8(PlayState* play) {
    u32 ret = play->envCtx.unk_E2;

    if ((play->sceneId == SCENE_OMOYA) && (play->roomCtx.curRoom.num == 0)) {
        // was the barn room blown off by aliens?
        ret = ((gSaveContext.save.day >= 2) && !(gSaveContext.save.weekEventReg[0x16] & 1)) ? true : false;
    }

    switch (play->sceneId) {
        case SCENE_13HUBUKINOMITI:
        case SCENE_11GORONNOSATO:
        case SCENE_10YUKIYAMANOMURA:
        case SCENE_14YUKIDAMANOMITI:
        case SCENE_12HAKUGINMAE:
        case SCENE_17SETUGEN:
        case SCENE_GORONRACE:
            // Winter/Snowing scenes
            if (gSaveContext.sceneLayer == 0) {
                ret = false;
            }
            break;

        case SCENE_10YUKIYAMANOMURA2:
            if (gSaveContext.sceneLayer == 1) {
                ret = false;
            }
            break;

        default:
            break;
    }

    return ret;
}

u8 Environment_IsFinalHours(PlayState* play) {
    u8 ret = false;

    if ((gSaveContext.save.day == 3) && (gSaveContext.save.time < CLOCK_TIME(6, 0))) {
        ret = true;
    }

    return ret;
}

u8 func_800FE5D0(PlayState* play) {
    u8 ret = false;

    if (Entrance_GetSceneId(((void)0, gSaveContext.save.entrance)) < 0) {
        ret = true;
    }

    return ret;
}

u32 func_800FE610(PlayState* play) {
    return 0;
}

u16 Environment_GetTimeSpeed(PlayState* play) {
    u16 timeSpeed = 0;

    if (R_TIME_SPEED != 0) {
        timeSpeed = R_TIME_SPEED + (u16)((void)0, gSaveContext.save.timeSpeedOffset);
    }

    return timeSpeed;
}

void func_800FE658(f32 arg0) {
    D_801BDB9C = arg0 * 45.511112f;
}

u8 func_800FE6F8(PlayState* play, s16 arg1, s16 arg2) {
    u16 temp_v0;
    u8 ret = false;

    if (R_TIME_SPEED != 0) {
        temp_v0 = ((void)0, gSaveContext.save.time) +
                  (R_TIME_SPEED + ((void)0, gSaveContext.save.timeSpeedOffset)) * arg1 * 20;
        if (arg2 < temp_v0) {
            ret = true;
        }
    }

    return ret;
}

u8 func_800FE778() {
    return D_801BDBA0;
}

u8 func_800FE788() {
    return D_801BDBA4;
}

f32 func_800FE798() {
    return D_801BDB90;
}

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FE7A8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FE9B4.s")

void func_800FEA50(PlayState* play) {
    s8 phi_v0;

    switch (gSaveContext.save.day) {
        default:
        case 0:
        case 1:
            phi_v0 = 0;
            break;
        case 2:
            phi_v0 = 3;
            break;
        case 3:
            phi_v0 = 4;
            break;
    }

    D_801F4F30 = phi_v0;
    D_801F4F33 = 1;
}

void func_800FEAB0(void) {
    D_801F4F33 = 0;
}

void func_800FEAC0(void) {
    if (D_801BDB9C != 0) {
        gSaveContext.save.time = ((void)0, gSaveContext.save.time) + D_801BDB9C;
        D_801BDB9C = 0;
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FEAF4.s")
