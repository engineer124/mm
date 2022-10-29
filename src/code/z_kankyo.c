#include "global.h"
#include "z64rumble.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "objects/gameplay_field_keep/gameplay_field_keep.h"

extern u8 sEnvIsTimeStopped;
extern f32 sSunEnvAlpha;
extern f32 sSunColor;
extern f32 sSunScale;
extern f32 sSunPrimAlpha;
extern f32 sSandstormLerpScale;
extern u16 sTimeJump;
extern u8 sSandstormColorIndex;
extern u8 sNextSandstormColorIndex;
extern Gfx D_0E0002C8[];
extern u8 D_801F4F30;
extern u8 D_801F4F31;
extern u8 D_801F4F32;
extern u8 D_801F4F33;
extern u8 sGameOverLightsIntensity;
extern LightInfo sNGameOverLightInfo;
extern LightInfo sSGameOverLightInfo;
extern LightNode* sNGameOverLightNode;
extern LightNode* sSGameOverLightNode;
extern Gfx* D_801F4F38;
extern s8 D_801BDBC0;
extern s8 D_801BDBC4;
extern s32 sEnvSkyboxNumStars;
extern f32 D_801F4F28;
extern TexturePtr sLightningTextures[];

extern s32 sSunScreenDepth;
extern s32 sSunDepthTestX;
extern s32 sSunDepthTestY;

u8 func_800FE5D0(PlayState* play);
void Environment_UpdateRain(PlayState* play);
void Environment_UpdateTimeBasedSequence(PlayState* play);
void Environment_UpdateTime(PlayState* play, EnvironmentContext* envCtx, PauseContext* pauseCtx, MessageContext* msgCtx,
                            GameOverContext* gameOverCtx);
void Environment_UpdateWeekEventRegs(PlayState* play);
void Environment_JumpForwardInTime(void);

void Environment_GraphCallback(GraphicsContext* gfxCtx, PlayState* play) {
    sSunScreenDepth = SysCfb_GetPixelDepth(sSunDepthTestX, sSunDepthTestY);
    Lights_GlowCheck(play);
}

typedef enum {
    /* 0x00 */ LIGHTNING_BOLT_START,
    /* 0x01 */ LIGHTNING_BOLT_WAIT,
    /* 0x02 */ LIGHTNING_BOLT_DRAW,
    /* 0xFF */ LIGHTNING_BOLT_INACTIVE = 0xFF
} LightningBoltState;

typedef struct {
    /* 0x00 */ u8 state;
    /* 0x04 */ Vec3f offset;
    /* 0x10 */ Vec3f pos;
    /* 0x1C */ s8 pitch;
    /* 0x1D */ s8 roll;
    /* 0x1E */ u8 textureIndex;
    /* 0x1F */ u8 delayTimer;
} LightningBolt; // size = 0x20

extern LightningBolt sLightningBolts[3];

typedef struct {
    /* 0x0 */ s32 mantissa;
    /* 0x4 */ s32 exponent;
} ZBufValConversionEntry; // size = 0x8

extern ZBufValConversionEntry sZBufValConversionTable[8];

#define ZBUFVAL_EXPONENT(v) (((v) >> 15) & 7)
#define ZBUFVAL_MANTISSA(v) (((v) >> 4) & 0x7FF)

/**
 * Convert an 18-bits Z buffer value to a fixed point 15.3 value
 *
 * zBufferVal is 18 bits:
 *   3: Exponent of z value
 *  11: Mantissa of z value
 *   4: dz value (unused)
 */
s32 Environment_ZBufValToFixedPoint(s32 zBufferVal) {
    // base[exp] + mantissa << shift[exp]
    s32 ret = (ZBUFVAL_MANTISSA(zBufferVal) << sZBufValConversionTable[ZBUFVAL_EXPONENT(zBufferVal)].mantissa) +
              sZBufValConversionTable[ZBUFVAL_EXPONENT(zBufferVal)].exponent;

    return ret;
}

extern s16 sLightningFlashAlpha;
extern u8 gSkyboxIsChanging;
extern u8 D_801F4E31;
extern u8 D_801BDBA8;

#ifdef NON_EQUIVALENT
void Environment_Init(PlayState* play2, EnvironmentContext* envCtx, s32 arg2) {
    PlayState* play = play2;
    f32 temp_ft4;
    u8 var_a0;
    u8 temp_t6;
    u8 temp_t7;
    u8 temp_t8;
    u8 temp_t9;
    u8 var_v0;
    s16 i;

    CREG(1) = 0;

    gSaveContext.sunsSongState = SUNSSONG_INACTIVE;

    gSaveContext.skyboxTime = ((void)0, gSaveContext.save.time);

    Environment_JumpForwardInTime();

    if ((((void)0, gSaveContext.save.time) >= CLOCK_TIME(18, 0)) ||
        (((void)0, gSaveContext.save.time) < CLOCK_TIME(6, 0))) {
        gSaveContext.save.isNight = 1;
    } else {
        gSaveContext.save.isNight = 0;
    }

    play->state.gfxCtx->callback = Environment_GraphCallback;
    play->state.gfxCtx->callbackParam = play;

    Lights_DirectionalSetInfo(&envCtx->dirLight1, 80, 80, 80, 80, 80, 80);
    LightContext_InsertLight(play, &play->lightCtx, &envCtx->dirLight1);

    Lights_DirectionalSetInfo(&envCtx->dirLight2, 80, 80, 80, 80, 80, 80);
    LightContext_InsertLight(play, &play->lightCtx, &envCtx->dirLight2);

    envCtx->unk_19 = 0;
    envCtx->unk_1A = 0;
    envCtx->changeLightEnabled = 0;
    envCtx->changeLightTimer = 0;
    envCtx->unk_44 = 0;
    envCtx->unk_10 = 99;
    envCtx->unk_11 = 99;

    envCtx->glareAlpha = 0.0f;
    envCtx->lensFlareAlphaScale = 0.0f;

    if ((play->sceneId == SCENE_00KEIKOKU) && (gSaveContext.sceneLayer == 8)) {
        gSaveContext.save.day = 1;
    }

    switch (gSaveContext.save.day) {
        default:
        case 0:
        case 1:
            envCtx->lightConfig = 0;
            envCtx->changeLightNextConfig = 0;
            break;
        case 2:
            envCtx->lightConfig = 3;
            envCtx->changeLightNextConfig = 3;
            break;
        case 3:
            envCtx->lightConfig = 4;
            envCtx->changeLightNextConfig = 4;
            break;
    }

    envCtx->unk_C1 = 0;
    envCtx->unk_C2 = 0;
    envCtx->unk_E0 = 0;
    envCtx->unk_E1 = 0;
    envCtx->unk_E2 = 0;
    envCtx->lightningState = 0;
    envCtx->timeSeqState = 0;
    envCtx->fillScreen = 0;
    envCtx->screenFillColor[0] = 0;
    envCtx->screenFillColor[1] = 0;
    envCtx->screenFillColor[2] = 0;
    envCtx->screenFillColor[3] = 0;
    envCtx->customSkyboxFilter = false;
    envCtx->skyboxFilterColor[0] = 0;
    envCtx->skyboxFilterColor[1] = 0;
    envCtx->skyboxFilterColor[2] = 0;
    envCtx->skyboxFilterColor[3] = 0;
    envCtx->sandstormState = 0;
    envCtx->sandstormPrimA = 0;
    envCtx->sandstormEnvA = 0;
    envCtx->lightBlend = 1.0f;

    gLightningStrike.state = 0;
    gLightningStrike.flashRed = 0;
    gLightningStrike.flashGreen = 0;
    gLightningStrike.flashBlue = 0;

    sLightningFlashAlpha = 0;

    D_801F4F30 = 0xFF;
    D_801F4F31 = 0;
    D_801F4E30 = 0;
    D_801F4F33 = 0;

    gSaveContext.cutsceneTransitionControl = 0;

    envCtx->adjLightSettings.ambientColor[0] = 0;
    envCtx->adjLightSettings.ambientColor[1] = 0;
    envCtx->adjLightSettings.ambientColor[2] = 0;
    envCtx->adjLightSettings.diffuseColor1[0] = 0;
    envCtx->adjLightSettings.diffuseColor1[1] = 0;
    envCtx->adjLightSettings.diffuseColor1[2] = 0;
    envCtx->adjLightSettings.diffuseColor2[0] = 0;
    envCtx->adjLightSettings.diffuseColor2[1] = 0;
    envCtx->adjLightSettings.diffuseColor2[2] = 0;
    envCtx->adjLightSettings.fogColor[0] = 0;
    envCtx->adjLightSettings.fogColor[1] = 0;
    envCtx->adjLightSettings.fogColor[2] = 0;
    envCtx->adjLightSettings.fogNear = 0;
    envCtx->adjLightSettings.fogFar = 0;

    envCtx->sunPos.x = -(Math_SinS(((void)0, gSaveContext.save.time) - CLOCK_TIME(12, 0)) * 120.0f) * 25.0f;
    envCtx->sunPos.y = (Math_CosS(((void)0, gSaveContext.save.time) - CLOCK_TIME(12, 0)) * 120.0f) * 25.0f;
    envCtx->sunPos.z = (Math_CosS(((void)0, gSaveContext.save.time) - CLOCK_TIME(12, 0)) * 20.0f) * 25.0f;

    envCtx->windDirection.x = 80;
    envCtx->windDirection.y = 80;
    envCtx->windDirection.z = 80;
    envCtx->windSpeed = 20.0f;

    envCtx->lightBlendEnabled = false;
    envCtx->lightSettingOverride = 0xFF;
    envCtx->lightBlendRateOverride = 0xFFFF;

    envCtx->sceneTimeSpeed = 0;
    R_TIME_SPEED = R_TIME_SPEED = 0;
    R_ENV_DISABLE_DBG = false;

    CREG(64) = 0;

    play->envCtx.precipitation[0] = 0;
    play->envCtx.precipitation[2] = 0;
    play->envCtx.precipitation[3] = 0;
    play->envCtx.precipitation[4] = 0;

    D_801F4E31 = envCtx->unk_17;

    var_a0 = 0;
    if (((void)0, gSaveContext.save.day) != 0) {
        var_a0 = (((void)0, gSaveContext.save.day) - 1);
    }

    temp_t8 = var_a0 + (D_801F4E31 * 3);
    var_v0 = temp_t8 & 0xFF;

    envCtx->unk_17 = temp_t8;
    envCtx->unk_18 = var_v0;

    // TODO: Solve `func_800FEAF4` first for the pattern here
    if (D_801F4E31 == 4) {
        var_v0 = 0xE & 0xFF;
        envCtx->unk_17 = 0xE;
        envCtx->unk_18 = var_v0;
    } else if (D_801F4E31 == 5) {
        var_v0 = 0x10U & 0xFF;
        envCtx->unk_17 = 0x10;
        envCtx->unk_18 = var_v0;
    } else if (D_801F4E31 == 6) {
        var_v0 = 0x11U & 0xFF;
        envCtx->unk_17 = 0x11;
        envCtx->unk_18 = var_v0;
    } else if (D_801F4E31 == 7) {
        temp_t6 = var_a0 + 0x12;
        var_v0 = temp_t6 & 0xFF;
        envCtx->unk_17 = temp_t6;
        envCtx->unk_18 = var_v0;
    } else if (D_801F4E31 == 8) {
        temp_t7 = var_a0 + 0x15;
        var_v0 = temp_t7 & 0xFF;
        envCtx->unk_17 = var_a0 + 0x15;
        envCtx->unk_18 = var_v0;
    } else if (D_801F4E31 == 9) {
        var_v0 = 0x18U & 0xFF;
        envCtx->unk_17 = 0x18;
        envCtx->unk_18 = var_v0;
    } else if (D_801F4E31 == 0xA) {
        temp_t9 = var_a0 + 0x19;
        var_v0 = temp_t9 & 0xFF;
        envCtx->unk_17 = temp_t9;
        envCtx->unk_18 = var_v0;
    }

    if (var_a0 >= 3) {
        envCtx->unk_17 = 0xD;
        envCtx->unk_18 = 0xD;
    }

    if (envCtx->unk_18 >= 0x1C) {
        envCtx->unk_17 = 0;
        envCtx->unk_18 = 0;
    }

    D_801F4E74 = 0.0f;

    if ((play->sceneId == SCENE_IKANA) &&
        ((((void)0, gSaveContext.sceneLayer) == 0) || (((void)0, gSaveContext.sceneLayer) == 1)) &&
        !(gSaveContext.save.weekEventReg[0x34] & 0x20)) {
        play->skyboxId = 3;
        envCtx->lightConfig = 5;
        envCtx->changeLightNextConfig = 5;
        D_801F4E74 = 1.0f;
    }

    if (gSaveContext.retainWeatherMode || (gSaveContext.respawnFlag != 0)) {
        if (gWeatherMode == 2) {
            if (!(gSaveContext.save.weekEventReg[0x34] & 0x20)) {
                play->skyboxId = 3;
                envCtx->lightConfig = 5;
                envCtx->changeLightNextConfig = 5;
                D_801F4E74 = 1.0f;
            } else {
                gWeatherMode = 0;
            }
        }

        play->envCtx.precipitation[2] = 0;
        play->envCtx.precipitation[3] = 0;

        if (gWeatherMode == 1) {
            if ((CURRENT_DAY == 2) && (((void)0, gSaveContext.save.time) >= CLOCK_TIME(7, 0)) &&
                (((void)0, gSaveContext.save.time) < CLOCK_TIME(17, 30))) {
                if (func_800FE4B8(play)) {
                    play->envCtx.precipitation[0] = 60;
                }
                play->envCtx.precipitation[1] = 60;
            } else {
                gWeatherMode = 0;
                Environment_StopStormNatureAmbience(play);
            }
        } else if (gWeatherMode == 3) {
            play->envCtx.precipitation[2] = 128;
            play->envCtx.precipitation[3] = 128;
            Environment_StopStormNatureAmbience(play);
        } else {
            Environment_StopStormNatureAmbience(play);
        }
    } else {
        gWeatherMode = 0;
        Environment_StopStormNatureAmbience(play);
    }

    gInterruptSongOfStorms = false;
    gLightConfigAfterUnderwater = 0;
    gSkyboxIsChanging = false;
    gSaveContext.retainWeatherMode = false;

    R_ENV_LIGHT1_DIR(0) = 80;
    R_ENV_LIGHT1_DIR(1) = 80;
    R_ENV_LIGHT1_DIR(2) = 80;

    R_ENV_LIGHT2_DIR(0) = -80;
    R_ENV_LIGHT2_DIR(1) = -80;
    R_ENV_LIGHT2_DIR(2) = -80;

    cREG(9) = 10;
    cREG(10) = 0;
    cREG(11) = 0;
    cREG(12) = 0;
    cREG(13) = 0;
    cREG(14) = 0;
    D_801F4DDC = 1; // should be u8, breaks bss

    for (i = 0; i < ARRAY_COUNT(sLightningBolts); i++) {
        sLightningBolts[i].state = LIGHTNING_BOLT_INACTIVE;
    }

    play->roomCtx.unk7A[0] = 0;
    play->roomCtx.unk7A[1] = 0;

    for (i = 0; i < ARRAY_COUNT(play->csCtx.actorActions); i++) {
        play->csCtx.actorActions[i] = NULL;
    }

    gCustomLensFlare1On = false;
    gCustomLensFlare2On = false;
    Rumble_StateReset();

    sEnvSkyboxNumStars = 0;
    gSkyboxNumStars = 0;
    D_801BDBA8 = 0;
    sEnvIsTimeStopped = 0;
    sSunPrimAlpha = 255.0f;

    Environment_UpdateWeekEventRegs(play);
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_Init.s")
#endif

u8 Environment_SmoothStepToU8(u8* pvalue, u8 target, u8 scale, u8 step, u8 minStep) {
    s16 stepSize = 0;
    s16 diff = target - *pvalue;

    if (target != *pvalue) {
        stepSize = diff / scale;
        if ((stepSize >= (s16)minStep) || ((s16)-minStep >= stepSize)) {
            if ((s16)step < stepSize) {
                stepSize = step;
            }
            if ((s16)-step > stepSize) {
                stepSize = -step;
            }
            *pvalue += (u8)stepSize;
        } else {
            if (stepSize < (s16)minStep) {
                stepSize = minStep;
                *pvalue += (u8)stepSize;
                if (target < *pvalue) {
                    *pvalue = target;
                }
            }
            if ((s16)-minStep < stepSize) {
                stepSize = -minStep;
                *pvalue += (u8)stepSize;
                if (*pvalue < target) {
                    *pvalue = target;
                }
            }
        }
    }
    return diff;
}

f32 Environment_LerpWeight(u16 max, u16 min, u16 val) {
    f32 diff = max - min;

    if (diff != 0.0f) {
        f32 ret = 1.0f - (max - val) / diff;

        if (!(ret >= 1.0f)) {
            return ret;
        }
    }

    return 1.0f;
}

f32 Environment_LerpWeightAccelDecel(u16 endFrame, u16 startFrame, u16 curFrame, u16 accelDuration, u16 decelDuration) {
    f32 endFrameF;
    f32 startFrameF;
    f32 curFrameF;
    f32 accelDurationF;
    f32 decelDurationF;
    f32 totalFrames;
    f32 temp;
    f32 framesElapsed;
    f32 ret;

    if (curFrame <= startFrame) {
        return 0.0f;
    }

    if (curFrame >= endFrame) {
        return 1.0f;
    }

    endFrameF = (s32)endFrame;
    startFrameF = (s32)startFrame;
    curFrameF = (s32)curFrame;
    totalFrames = endFrameF - startFrameF;
    framesElapsed = curFrameF - startFrameF;
    accelDurationF = (s32)accelDuration;
    decelDurationF = (s32)decelDuration;

    if ((startFrameF >= endFrameF) || (accelDurationF + decelDurationF > totalFrames)) {
        return 0.0f;
    }

    temp = 1.0f / ((totalFrames * 2.0f) - accelDurationF - decelDurationF);

    if (accelDurationF != 0.0f) {
        if (framesElapsed <= accelDurationF) {
            return temp * framesElapsed * framesElapsed / accelDurationF;
        }
        ret = temp * accelDurationF;
    } else {
        ret = 0.0f;
    }

    if (framesElapsed <= totalFrames - decelDurationF) {
        ret += 2.0f * temp * (framesElapsed - accelDurationF);
        return ret;
    }

    ret += 2.0f * temp * (totalFrames - accelDurationF - decelDurationF);

    if (decelDurationF != 0.0f) {
        ret += temp * decelDurationF;
        if (framesElapsed < totalFrames) {
            ret -= temp * (totalFrames - framesElapsed) * (totalFrames - framesElapsed) / decelDurationF;
        }
    }

    return ret;
}

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_UpdateSkybox.s")

void Environment_EnableUnderwaterLights(PlayState* play, s32 waterLightsIndex) {
    if (waterLightsIndex == 0x1F) {
        waterLightsIndex = 0;
    }

    if (play->envCtx.lightMode == LIGHT_MODE_TIME) {
        if (!D_801F4F31) {
            D_801F4F31 = true;
            gLightConfigAfterUnderwater = play->envCtx.changeLightNextConfig;
            if (play->envCtx.lightConfig != waterLightsIndex) {
                play->envCtx.lightConfig = waterLightsIndex;
                play->envCtx.changeLightNextConfig = waterLightsIndex;
            }
        } else if ((play->envCtx.lightConfig != waterLightsIndex) && (play->envCtx.changeLightTimer == 0)) {
            play->envCtx.changeLightEnabled = true;
            play->envCtx.changeLightNextConfig = waterLightsIndex;
            play->envCtx.changeDuration = 20;
            play->envCtx.changeLightTimer = play->envCtx.changeDuration;
        }
    } else if (play->envCtx.lightSettingOverride == 0xFF) {
        if (!D_801F4F31) {
            D_801F4F31 = true;
            gLightConfigAfterUnderwater = play->envCtx.unk_C1;
        }
        play->envCtx.lightBlendEnabled = false; // instantly switch to water lights
        play->envCtx.lightSettingOverride = waterLightsIndex;
    }
}

void Environment_DisableUnderwaterLights(PlayState* play) {
    if (play->envCtx.lightMode == LIGHT_MODE_TIME) {
        if (D_801F4F31) {
            D_801F4F31 = false;
            play->envCtx.lightConfig = gLightConfigAfterUnderwater;
            play->envCtx.changeLightNextConfig = gLightConfigAfterUnderwater;
        }
    } else if (D_801F4F31) {
        D_801F4F31 = false;
        play->envCtx.unk_C1 = gLightConfigAfterUnderwater;
        play->envCtx.lightBlendEnabled = false; // instantly switch to previous lights
        play->envCtx.lightSettingOverride = 0xFF;
        play->envCtx.lightBlend = 1.0f;
    }
}

void Environment_ClearRumbleRequests(void) {
    if ((gSaveContext.gameMode != 0) && (gSaveContext.gameMode != 3)) {
        Rumble_StateWipeRequests();
    }
}

void Environment_UpdateSkyboxRotY(PlayState* play) {
    if ((play->pauseCtx.state == 0) && (play->pauseCtx.debugEditor == 0) &&
        ((play->skyboxId == 1) || (play->skyboxId == 3))) {
        play->skyboxCtx.rotY -= R_TIME_SPEED * 1.0e-4f;
    }
}

// nextDayTime is used as both a time of day value and a timer to delay sfx when changing days.
// When Sun's Song is played, nextDayTime is set to 0x8001 or 0 for day and night respectively.
// These values will actually get used as a time of day value.
// After this, nextDayTime is assigned magic values of 0xFFFE or 0xFFFD for day and night respectively.
// From here, 0x10 is decremented from nextDayTime until it reaches either 0xFF0E or 0xFF0D, effectively
// delaying the chicken crow or dog howl sfx by 15 frames when loading the new area.
void Environment_UpdateNextDayTime(void) {
    if ((gSaveContext.nextDayTime >= 0xFF00) && (gSaveContext.nextDayTime != NEXT_TIME_NONE)) {
        gSaveContext.nextDayTime -= 0x10;
        if (gSaveContext.nextDayTime == (NEXT_TIME_DAY_SET - (15 * 0x10))) {
            play_sound(NA_SE_EV_CHICKEN_CRY_M);
            gSaveContext.nextDayTime = NEXT_TIME_NONE;
        } else if (gSaveContext.nextDayTime == (NEXT_TIME_NIGHT_SET - (15 * 0x10))) {
            func_8019F128(NA_SE_EV_DOG_CRY_EVENING);
            gSaveContext.nextDayTime = NEXT_TIME_NONE;
        }
    }
}

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

void func_800F6CEC(PlayState* play, u8 arg1, AdjLightSettings* arg2, EnvLightSettings* arg3) {
    s32 phi_t1;
    s32 temp_v1_2;
    s32 temp_v1 = (arg1 % 4);

    if ((((void)0, gSaveContext.save.day) >= 2) && (arg1 >= 4) && (arg1 < 8)) {
        temp_v1_2 = (((void)0, gSaveContext.save.day) * 4) + 4;
        for (phi_t1 = 0; phi_t1 != 3; phi_t1++) {
            arg2->ambientColor[phi_t1] =
                arg3[temp_v1_2 + temp_v1].ambientColor[phi_t1] - arg3[temp_v1].ambientColor[phi_t1];
            arg2->diffuseColor1[phi_t1] =
                arg3[temp_v1_2 + temp_v1].diffuseColor1[phi_t1] - arg3[temp_v1].diffuseColor1[phi_t1];
            arg2->diffuseColor2[phi_t1] = arg3[temp_v1_2 + temp_v1].diffuseColor[phi_t1] -
                                          arg3[temp_v1].diffuseColor[phi_t1]; // TODO rename to diffuseColor2
            arg2->fogColor[phi_t1] = arg3[temp_v1_2 + temp_v1].fogColor[phi_t1] - arg3[temp_v1].fogColor[phi_t1];
        }
        arg2->fogNear = arg3[temp_v1_2 + temp_v1].fogNear - arg3[temp_v1].fogNear;
    }

    if ((arg1 >= 4) && (arg1 < 8) && (gWeatherMode == 1)) {
        arg2->ambientColor[0] = -50;
        arg2->ambientColor[1] = -100;
        arg2->ambientColor[2] = -100;
        arg2->diffuseColor1[0] = -100;
        arg2->diffuseColor1[1] = -100;
        arg2->diffuseColor1[2] = -100;
        arg2->diffuseColor2[0] = -100;
        arg2->diffuseColor2[1] = -100;
        arg2->diffuseColor2[2] = -100;

        temp_v1 = arg1;
        arg2->fogColor[0] = -arg3[temp_v1].fogColor[0] + 30;
        arg2->fogColor[1] = -arg3[temp_v1].fogColor[1] + 30;
        arg2->fogColor[2] = -arg3[temp_v1].fogColor[2] + 45;
    }
}

u8 func_800F6EA4(f32 arg0, f32 arg1, f32 arg2) {
    arg0 = CLAMP(arg0, 0.0f, 255.0f);
    arg1 = CLAMP(arg1, 0.0f, 255.0f);

    return (((arg0 - arg1) * arg2) + arg1);
}

s32 Environment_IsSceneUpsideDown(PlayState* play) {
    s32 ret = false;

    if ((play->sceneId == SCENE_F41) || (play->sceneId == SCENE_INISIE_R)) {
        ret = true;
    }
    return ret;
}

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_UpdateLights.s")

void Environment_UpdateSun(PlayState* play) {
    f32 temp_f0;
    s16 temp_a0;
    u16 phi_v0;

    if (!play->envCtx.sunDisabled) {
        if (play->envCtx.precipitation[1] != 0) {
            Math_SmoothStepToF(&sSunPrimAlpha, 0.0f, 0.5f, 4.0f, 0.01f);
        } else {
            Math_SmoothStepToF(&sSunPrimAlpha, 255.0f, 0.5f, 4.0f, 0.01f);
        }

        if (Environment_IsSceneUpsideDown(play)) {
            phi_v0 = ((void)0, gSaveContext.save.time) + CLOCK_TIME(12, 0);
        } else {
            phi_v0 = ((void)0, gSaveContext.save.time);
        }

        temp_a0 = phi_v0 - CLOCK_TIME(12, 0);

        if (play->csCtx.state != 0) {
            // TODO document bug
            Math_SmoothStepToF(&play->envCtx.sunPos.x, -(Math_SinS(temp_a0) * 120.0f) * 25.0f, 1.0f, 0.8f, 0.8f);
            Math_SmoothStepToF(&play->envCtx.sunPos.y, Math_CosS(temp_a0) * 120.0f * 25.0f, 1.0f, 0.8f, 0.8f);
            //! @bug This should be z.
            Math_SmoothStepToF(&play->envCtx.sunPos.y, Math_CosS(temp_a0) * 20.0f * 25.0f, 1.0f, 0.8f, 0.8f);
        } else {
            play->envCtx.sunPos.x = -(Math_SinS(temp_a0) * 120.0f) * 25.0f;
            play->envCtx.sunPos.y = Math_CosS(temp_a0) * 120.0f * 25.0f;
            play->envCtx.sunPos.z = Math_CosS(temp_a0) * 20.0f * 25.0f;
        }

        temp_f0 = play->envCtx.sunPos.y / 25.0f;
        if (Environment_IsSceneUpsideDown(play)) {
            temp_f0 = -temp_f0;
        }

        sSunEnvAlpha = temp_f0 / 80.0f * 255.0f;
        if (sSunEnvAlpha < 0.0f) {
            sSunEnvAlpha = 0.0f;
        }
        if (sSunEnvAlpha > 255.0f) {
            sSunEnvAlpha = 255.0f;
        }
        sSunEnvAlpha = 255.0f - sSunEnvAlpha;

        sSunColor = temp_f0 / 80.0f;
        if (sSunColor < 0.0f) {
            sSunColor = 0.0f;
        }
        if (sSunColor > 1.0f) {
            sSunColor = 1.0f;
        }

        sSunScale = (2.0f * sSunColor) + 12.0f;
    }
}

void func_800F88C4(u16 arg0) {
    // TODO ((void)0)?
    if (((gSaveContext.save.weekEventReg[28] & 8) == 0) && ((gSaveContext.save.weekEventReg[28] & 0x10) == 0) &&
        ((gSaveContext.save.weekEventReg[arg0 >> 8] & (arg0 & 0xFF)) != 0)) {
        if (gSaveContext.save.weekEventReg[91] & 4) {
            gSaveContext.save.weekEventReg[28] |= 8;
            gSaveContext.save.weekEventReg[90] |= 8;
            gSaveContext.save.weekEventReg[51] |= 2;
        }

        if (gSaveContext.save.weekEventReg[91] & 8) {
            gSaveContext.save.weekEventReg[28] |= 0x10;
            gSaveContext.save.weekEventReg[90] |= 8;
            gSaveContext.save.weekEventReg[51] |= 2;
        }
    }
}

void func_800F8970(void) {
    if (((gSaveContext.save.weekEventReg[27] & 0x40) == 0) && ((gSaveContext.save.time - 0x3FFC) >= 0x2580)) {
        gSaveContext.save.weekEventReg[27] |= 0x40;
        func_800F88C4(0x1B02); // TODO macro?
    }

    if (((gSaveContext.save.weekEventReg[27] & 0x80) == 0) && ((gSaveContext.save.time - 0x3FFC) >= 0x2B30)) {
        gSaveContext.save.weekEventReg[27] |= 0x80;
        func_800F88C4(0x1B04);
    }

    if (((gSaveContext.save.weekEventReg[28] & 1) == 0) && ((gSaveContext.save.time - 0x3FFC) >= 0x30E0)) {
        gSaveContext.save.weekEventReg[28] |= 1;
        func_800F88C4(0x1B08);
    }

    if (((gSaveContext.save.weekEventReg[28] & 2) == 0) && ((gSaveContext.save.time - 0x3FFC) >= 0x3413)) {
        gSaveContext.save.weekEventReg[28] |= 2;
        func_800F88C4(0x1B10);
    }

    if (((gSaveContext.save.weekEventReg[28] & 4) == 0) && ((gSaveContext.save.time - 0x3FFC) >= 0x39C3)) {
        gSaveContext.save.weekEventReg[28] |= 4;
        func_800F88C4(0x1B20);
    }
}

#ifdef NON_MATCHING
void Environment_UpdateWeekEventRegs(PlayState* play) {
    u8 v1;
    u16 temp_a2_2;

    if ((u8)((void)0, ((gSaveContext.eventInf[7] & 0xE0) >> 5)) != (u8)((void)0, gSaveContext.save.day)) {
        v1 = ((void)0, (gSaveContext.eventInf[7] & 0x1F)) | (((void)0, gSaveContext.save.day) << 5);
        gSaveContext.eventInf[7] = v1;
        gSaveContext.save.weekEventReg[27] |= 0x40;
        gSaveContext.save.weekEventReg[27] |= 0x80;
        gSaveContext.save.weekEventReg[28] |= 1;
        gSaveContext.save.weekEventReg[28] |= 2;
        gSaveContext.save.weekEventReg[28] |= 4;

        func_800F88C4(0x1B02);
        func_800F88C4(0x1B04);
        func_800F88C4(0x1B08);
        func_800F88C4(0x1B10);
        func_800F88C4(0x1B20);

        gSaveContext.save.weekEventReg[27] &= (u8)~0x40;
        gSaveContext.save.weekEventReg[27] &= (u8)~0x80;
        gSaveContext.save.weekEventReg[28] &= (u8)~1;
        gSaveContext.save.weekEventReg[28] &= (u8)~2;
        gSaveContext.save.weekEventReg[28] &= (u8)~4;
    }
    if (((((void)0, gSaveContext.save.time) - 0x3FFC) < 0x1FFE) ||
        ((((void)0, gSaveContext.save.time) - 0x3FFC) >= 0x3FFD)) {
        gSaveContext.save.weekEventReg[90] &= (u8)~8;
    }

    func_800F8970();

    if (((gSaveContext.save.weekEventReg[75] & 0x10) == 0) && ((gSaveContext.save.weekEventReg[55] & 2) == 0)) {
        if (((void)0, gSaveContext.save.day) >= 2) {
            gSaveContext.save.weekEventReg[55] |= 2;
        } else if ((((void)0, gSaveContext.save.day) == 1) &&
                   ((((void)0, gSaveContext.save.time) - 0x3FFC) >= 0x6FF9)) {
            gSaveContext.save.weekEventReg[55] |= 2;
        }
    }

    if ((gSaveContext.save.weekEventReg[90] & 1) == 0) {
        temp_a2_2 = ((void)0, gSaveContext.save.time) - D_801F4E78;
        if (((gSaveContext.save.weekEventReg[89] & 0x40) != 0) &&
            (((((void)0, gSaveContext.save.time) - 0x3FFC) & 0xFFFF) >= 0xF556)) {
            gSaveContext.save.weekEventReg[90] |= 1;
        } else if (((gSaveContext.save.weekEventReg[89] & 8) != 0) && (temp_a2_2 >= 0x416)) {
            gSaveContext.save.weekEventReg[89] |= 0x40;
            D_801F4E78 = 0;
        } else if (((gSaveContext.save.weekEventReg[85] & 0x80) != 0) && (temp_a2_2 >= 0x1198)) {
            gSaveContext.save.weekEventReg[89] |= 8;
            D_801F4E78 = gSaveContext.save.time;
        } else if (((gSaveContext.save.weekEventReg[86] & 1) != 0) && (temp_a2_2 >= 0xC43)) {
            gSaveContext.save.weekEventReg[85] |= 0x80;
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_UpdateWeekEventRegs.s")
#endif

void Environment_Update(PlayState* play, EnvironmentContext* envCtx, LightContext* lightCtx, PauseContext* pauseCtx,
                        MessageContext* msgCtx, GameOverContext* gameOverCtx) {
    D_801BDBC0 = 0;
    D_801BDBC4 = 0;

    Environment_ClearRumbleRequests();

    if (pauseCtx->state == 0) {
        Environment_UpdateSkyboxRotY(play);
        Environment_UpdateRain(play);
        Environment_UpdateTimeBasedSequence(play);
        Environment_UpdateNextDayTime();
        Environment_UpdateTime(play, envCtx, pauseCtx, msgCtx, gameOverCtx);
        Environment_UpdateSun(play);
        Environment_UpdateLights(play, envCtx, lightCtx);
        Environment_UpdateWeekEventRegs(play);
    }
}

void Environment_DrawSun(PlayState* play) {
    if (!play->envCtx.sunDisabled) {
        OPEN_DISPS(play->state.gfxCtx);
        if ((play->envCtx.sunPos.y > -800.0f) || Environment_IsSceneUpsideDown(play)) {
            Matrix_Translate(play->view.eye.x + play->envCtx.sunPos.x, play->view.eye.y + play->envCtx.sunPos.y,
                             play->view.eye.z + play->envCtx.sunPos.z, MTXMODE_NEW);
            if (((void)0, gSaveContext.save.time) < 0x8000) {
                gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, (u8)((u8)(sSunColor * 52.0f) + 203),
                                (u8)((u8)(sSunColor * 19.0f) + 181), (u8)sSunPrimAlpha);
                gDPSetEnvColor(POLY_OPA_DISP++, (u8)(-(u8)(sSunColor * 5.0f) + 175),
                               (u8)((u8)(sSunColor * 135.0f) + 120), (u8)((u8)(sSunColor * 100.0f) + 100),
                               sSunEnvAlpha);
            } else {
                gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, (u8)((u8)(sSunColor * 75.0f) + 180),
                                (u8)((u8)(sSunColor * 100.0f) + 100), (u8)sSunPrimAlpha);
                gDPSetEnvColor(POLY_OPA_DISP++, 180, (u8)(sSunColor * 255.0f), (u8)(sSunColor * 200.0f), sSunEnvAlpha);
            }
            Matrix_Scale(sSunScale, sSunScale, sSunScale, 1);
            gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_LOAD);
            func_8012C148(play->state.gfxCtx);
            gSPDisplayList(POLY_OPA_DISP++, gSunDL);
        }
        CLOSE_DISPS(play->state.gfxCtx);
    }
}

void Environment_DrawSunLensFlare(PlayState* play, EnvironmentContext* envCtx, View* view, GraphicsContext* gfxCtx,
                                  Vec3f pos) {
    if ((play->envCtx.precipitation[1] == 0) && !(GET_ACTIVE_CAM(play)->stateFlags & CAM_STATE_UNDERWATER) &&
        (play->skyboxId == 1)) {
        f32 v0 = Math_CosS(((void)0, gSaveContext.save.time) - CLOCK_TIME(12, 0));
        Environment_DrawLensFlare(play, &play->envCtx, &play->view, play->state.gfxCtx, pos, 370.0f, v0 * 120.0f, 400,
                                  true);
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_DrawLensFlare.s")

f32 Environment_RandCentered(void) {
    return Rand_ZeroOne() - 0.5f;
}

void Environment_DrawRainImpl(PlayState* play, View* view, GraphicsContext* gfxCtx);
#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_DrawRainImpl.s")

void Environment_DrawRain(PlayState* play, View* view, GraphicsContext* gfxCtx) {
    if (!(GET_ACTIVE_CAM(play)->stateFlags & CAM_STATE_UNDERWATER) && (play->envCtx.precipitation[2] == 0)) {
        if (play->envCtx.precipitation[4] != 0) {
            if (play->envCtx.precipitation[2] == 0) {
                Environment_DrawRainImpl(play, view, gfxCtx);
            }
        } else if (!(GET_ACTIVE_CAM(play)->stateFlags & CAM_STATE_UNDERWATER)) {
            if (func_800FE4B8(play) && (play->envCtx.precipitation[2] == 0)) {
                Environment_DrawRainImpl(play, view, gfxCtx);
            }
        }
    }
}

void Environment_ChangeLightSetting(PlayState* play, u8 lightConfig) {
    if (play->envCtx.lightMode == LIGHT_MODE_TIME) {
        if ((lightConfig == 31) || D_801F4F33) {
            if (D_801F4F30 != 0xFF) {
                play->envCtx.changeLightEnabled = true;
                play->envCtx.changeLightNextConfig = D_801F4F30;
                play->envCtx.changeDuration = 20;
                play->envCtx.changeLightTimer = play->envCtx.changeDuration;
                D_801F4F30 = 0xFF;
            }
        } else if (play->envCtx.changeLightNextConfig != lightConfig) {
            D_801F4F30 = play->envCtx.lightConfig;
            play->envCtx.changeLightEnabled = true;
            play->envCtx.changeLightNextConfig = lightConfig;
            play->envCtx.changeDuration = 20;
            play->envCtx.changeLightTimer = play->envCtx.changeDuration;
        }
    } else if ((play->envCtx.unk_C1 != lightConfig) && (play->envCtx.lightBlend >= 1.0f) &&
               (play->envCtx.lightSettingOverride == 0xFF)) {
        if (lightConfig > 30) {
            lightConfig = 0;
        }

        play->envCtx.lightBlend = 0.0f;
        play->envCtx.unk_C2 = play->envCtx.unk_C1;
        play->envCtx.unk_C1 = lightConfig;
    }
}

void Environment_DrawSkyboxFilters(PlayState* play) {
    if ((((play->skyboxId != 0) && (play->lightCtx.fogNear < 980)) || (play->skyboxId >= 2)) &&
        ((play->skyboxId != 3) || (D_801F4E74 != 0.0f))) {
        f32 alpha;

        OPEN_DISPS(play->state.gfxCtx);

        func_8012C080(play->state.gfxCtx);

        alpha = (1000 - play->lightCtx.fogNear) * 0.02f;

        if (play->skyboxId == 2) {
            alpha = 1.0f;
        }

        if (alpha > 1.0f) {
            alpha = 1.0f;
        }

        if (play->skyboxId != 3) {
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, play->lightCtx.fogColor[0], play->lightCtx.fogColor[1],
                            play->lightCtx.fogColor[2], 255.0f * alpha);
        } else {
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, play->lightCtx.fogColor[0] + 16, play->lightCtx.fogColor[1] + 16,
                            play->lightCtx.fogColor[2] + 16, 255.0f * D_801F4E74);
        }
        gSPDisplayList(POLY_OPA_DISP++, D_0E0002C8);

        CLOSE_DISPS(play->state.gfxCtx);
    }

    if (play->envCtx.customSkyboxFilter) {
        OPEN_DISPS(play->state.gfxCtx);

        func_8012C080(play->state.gfxCtx);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, play->envCtx.skyboxFilterColor[0], play->envCtx.skyboxFilterColor[1],
                        play->envCtx.skyboxFilterColor[2], play->envCtx.skyboxFilterColor[3]);
        gSPDisplayList(POLY_OPA_DISP++, D_0E0002C8);

        CLOSE_DISPS(play->state.gfxCtx);
    }
}

void Environment_DrawLightningFlash(PlayState* play, u8 red, u8 green, u8 blue, u8 alpha) {
    OPEN_DISPS(play->state.gfxCtx);

    func_8012C080(play->state.gfxCtx);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, red, green, blue, alpha);
    gSPDisplayList(POLY_OPA_DISP++, D_0E0002C8);

    CLOSE_DISPS(play->state.gfxCtx);
}

void Environment_UpdateLightningStrike(PlayState* play) {
    if (play->envCtx.lightningState != LIGHTNING_OFF) {
        switch (gLightningStrike.state) {
            case LIGHTNING_STRIKE_WAIT:
                // every frame theres a 10% chance of the timer advancing 10 units
                if (Rand_ZeroOne() < 0.1f) {
                    gLightningStrike.delayTimer += 10.0f;
                }

                gLightningStrike.delayTimer += Rand_ZeroOne();

                if (gLightningStrike.delayTimer > 500.0f) {
                    gLightningStrike.flashRed = 200;
                    gLightningStrike.flashGreen = 200;
                    gLightningStrike.flashBlue = 255;
                    gLightningStrike.flashAlphaTarget = 200;

                    gLightningStrike.delayTimer = 0.0f;
                    Environment_AddLightningBolts(
                        play, (u8)((Rand_ZeroOne() * (ARRAY_COUNT(sLightningBolts) - 0.1f)) + 1.0f));
                    sLightningFlashAlpha = 0;
                    gLightningStrike.state++;
                }
                break;

            case LIGHTNING_STRIKE_START:
                gLightningStrike.flashRed = 200;
                gLightningStrike.flashGreen = 200;
                gLightningStrike.flashBlue = 255;

                play->envCtx.adjLightSettings.ambientColor[0] += 80;
                play->envCtx.adjLightSettings.ambientColor[1] += 80;
                play->envCtx.adjLightSettings.ambientColor[2] += 100;

                sLightningFlashAlpha += 100;

                if (sLightningFlashAlpha >= gLightningStrike.flashAlphaTarget) {
                    Audio_SetAmbienceChannelIO(AMBIENCE_CHANNEL_LIGHTNING, 0, 0);
                    gLightningStrike.state++;
                    gLightningStrike.flashAlphaTarget = 0;
                }
                break;

            case LIGHTNING_STRIKE_END:
                if (play->envCtx.adjLightSettings.ambientColor[0] > 0) {
                    play->envCtx.adjLightSettings.ambientColor[0] -= 10;
                    play->envCtx.adjLightSettings.ambientColor[1] -= 10;
                }

                if (play->envCtx.adjLightSettings.ambientColor[2] > 0) {
                    play->envCtx.adjLightSettings.ambientColor[2] -= 10;
                }

                sLightningFlashAlpha -= 10;

                if (sLightningFlashAlpha <= gLightningStrike.flashAlphaTarget) {
                    play->envCtx.adjLightSettings.ambientColor[0] = 0;
                    play->envCtx.adjLightSettings.ambientColor[1] = 0;
                    play->envCtx.adjLightSettings.ambientColor[2] = 0;

                    gLightningStrike.state = LIGHTNING_STRIKE_WAIT;

                    if (play->envCtx.lightningState == LIGHTNING_LAST) {
                        play->envCtx.lightningState = LIGHTNING_OFF;
                    }
                }
                break;
        }
    }

    if (gLightningStrike.state != LIGHTNING_STRIKE_WAIT) {
        Environment_DrawLightningFlash(play, gLightningStrike.flashRed, gLightningStrike.flashGreen,
                                       gLightningStrike.flashBlue, sLightningFlashAlpha);
    }
}

/**
 * Request the number of lightning bolts specified by `num`
 * Note: only 3 lightning bolts can be active at the same time.
 */
void Environment_AddLightningBolts(PlayState* play, u8 num) {
    s16 boltsAdded = 0;
    s16 i;

    for (i = 0; i < ARRAY_COUNT(sLightningBolts); i++) {
        if (sLightningBolts[i].state == LIGHTNING_BOLT_INACTIVE) {
            sLightningBolts[i].state = LIGHTNING_BOLT_START;
            boltsAdded++;

            if (boltsAdded >= num) {
                break;
            }
        }
    }
}

/**
 * Draw any active lightning bolt entries contained in `sLightningBolts`
 */
void Environment_DrawLightning(PlayState* play, s32 unused) {
    s16 i;
    f32 dx;
    f32 dz;
    f32 x;
    f32 z;
    s32 pad[2];

    OPEN_DISPS(play->state.gfxCtx);

    for (i = 0; i < ARRAY_COUNT(sLightningBolts); i++) {
        switch (sLightningBolts[i].state) {
            case LIGHTNING_BOLT_START:
                dx = play->view.at.x - play->view.eye.x;
                dz = play->view.at.z - play->view.eye.z;

                x = dx / sqrtf(SQ(dx) + SQ(dz));
                z = dz / sqrtf(SQ(dx) + SQ(dz));

                sLightningBolts[i].pos.x = play->view.eye.x + x * 9500.0f;
                sLightningBolts[i].pos.y = Rand_ZeroOne() * 1000.0f + 4000.0f;
                sLightningBolts[i].pos.z = play->view.eye.z + z * 9500.0f;

                sLightningBolts[i].offset.x = (Rand_ZeroOne() - 0.5f) * 5000.0f;
                sLightningBolts[i].offset.y = 0.0f;
                sLightningBolts[i].offset.z = (Rand_ZeroOne() - 0.5f) * 5000.0f;

                sLightningBolts[i].textureIndex = 0;
                sLightningBolts[i].pitch = (Rand_ZeroOne() - 0.5f) * 40.0f;
                sLightningBolts[i].roll = (Rand_ZeroOne() - 0.5f) * 40.0f;
                sLightningBolts[i].delayTimer = 3 * (i + 1);
                sLightningBolts[i].state++;
                break;
            case LIGHTNING_BOLT_WAIT:
                sLightningBolts[i].delayTimer--;

                if (sLightningBolts[i].delayTimer <= 0) {
                    sLightningBolts[i].state++;
                }
                break;
            case LIGHTNING_BOLT_DRAW:
                if (sLightningBolts[i].textureIndex < 7) {
                    sLightningBolts[i].textureIndex++;
                } else {
                    sLightningBolts[i].state = LIGHTNING_BOLT_INACTIVE;
                }
                break;
        }

        if (sLightningBolts[i].state == LIGHTNING_BOLT_DRAW) {
            Matrix_Translate(sLightningBolts[i].pos.x + sLightningBolts[i].offset.x,
                             sLightningBolts[i].pos.y + sLightningBolts[i].offset.y,
                             sLightningBolts[i].pos.z + sLightningBolts[i].offset.z, MTXMODE_NEW);
            Matrix_RotateXFApply(DEGF_TO_RADF(sLightningBolts[i].pitch));
            Matrix_RotateZF(DEGF_TO_RADF(sLightningBolts[i].roll), MTXMODE_APPLY);
            Matrix_Scale(22.0f, 100.0f, 22.0f, MTXMODE_APPLY);
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, 128);
            gDPSetEnvColor(POLY_XLU_DISP++, 0, 255, 255, 128);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPSegment(POLY_XLU_DISP++, 0x08,
                       Lib_SegmentedToVirtual(sLightningTextures[sLightningBolts[i].textureIndex]));
            func_8012C9BC(play->state.gfxCtx);
            gSPMatrix(POLY_XLU_DISP++, &D_01000000, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, gEffLightningDL);
        }
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

void Environment_PlaySceneSequence(PlayState* play) {
    u8 dayMinusOne = ((void)0, gSaveContext.save.day) - 1;

    if (dayMinusOne >= 3) {
        dayMinusOne = 0;
    }

    if (play->envCtx.timeSeqState != TIMESEQ_REQUEST) {
        play->envCtx.timeSeqState = TIMESEQ_DISABLED;
        if (((void)0, gSaveContext.forcedSeqId) != NA_BGM_GENERAL_SFX) {
            play->envCtx.timeSeqState = TIMESEQ_REQUEST;
            Audio_QueueSeqCmd(((void)0, gSaveContext.forcedSeqId) + 0x8000);
            gSaveContext.forcedSeqId = NA_BGM_GENERAL_SFX;
        } else if (!Environment_IsFinalHours(play) || func_800FE5D0(play) ||
                   (Audio_GetActiveSequence(SEQ_PLAYER_BGM_MAIN) != NA_BGM_FINAL_HOURS)) {
            if (play->sequenceCtx.seqId == NA_BGM_NO_MUSIC) {
                if (play->sequenceCtx.ambienceId == AMBIENCE_ID_13) {
                    return;
                }
                if (play->sequenceCtx.ambienceId != ((void)0, gSaveContext.ambienceId)) {
                    Audio_PlayAmbience(play->sequenceCtx.ambienceId);
                }
            } else if (play->sequenceCtx.ambienceId == AMBIENCE_ID_13) {
                if (play->sequenceCtx.seqId != ((void)0, gSaveContext.seqId)) {
                    Audio_PlaySceneSequence(play->sequenceCtx.seqId, dayMinusOne);
                }
            } else if ((((void)0, gSaveContext.save.time) >= CLOCK_TIME(6, 0)) &&
                       (((void)0, gSaveContext.save.time) <= CLOCK_TIME(17, 10))) {
                if (sSceneSeqState != SCENESEQ_DEFAULT) {
                    Audio_PlayMorningSceneSequence(play->sequenceCtx.seqId, dayMinusOne);
                } else if ((((void)0, gSaveContext.save.time) >= CLOCK_TIME(6, 1)) &&
                           (play->sequenceCtx.seqId != ((void)0, gSaveContext.seqId))) {
                    Audio_PlaySceneSequence(play->sequenceCtx.seqId, dayMinusOne);
                }
                play->envCtx.timeSeqState = TIMESEQ_FADE_DAY_BGM;
            } else {
                if (play->sequenceCtx.ambienceId != ((void)0, gSaveContext.ambienceId)) {
                    Audio_PlayAmbience(play->sequenceCtx.ambienceId);
                }
                if ((((void)0, gSaveContext.save.time) > CLOCK_TIME(17, 10)) &&
                    (((void)0, gSaveContext.save.time) < CLOCK_TIME(19, 0))) {
                    play->envCtx.timeSeqState = TIMESEQ_EARLY_NIGHT_CRITTERS;
                } else if ((((void)0, gSaveContext.save.time) > CLOCK_TIME(19, 0)) ||
                           (((void)0, gSaveContext.save.time) < CLOCK_TIME(5, 0))) {
                    play->envCtx.timeSeqState = TIMESEQ_NIGHT_CRITTERS;
                } else {
                    play->envCtx.timeSeqState = TIMESEQ_MORNING_CRITTERS;
                }
            }
        }
        func_801A3CD8(play->roomCtx.curRoom.echo);
        sSceneSeqState = SCENESEQ_DEFAULT;
    }
}

void Environment_UpdateTimeBasedSequence(PlayState* play) {
    s32 pad;

    //! FAKE:
    if (&gSaveContext.save) {}

    if ((play->csCtx.state == 0) && !(play->actorCtx.flags & ACTORCTX_FLAG_TELESCOPE_ON)) {
        switch (play->envCtx.timeSeqState) {
            case TIMESEQ_DAY_BGM:
                break;

            case TIMESEQ_FADE_DAY_BGM:
                if (((void)0, gSaveContext.save.time) > CLOCK_TIME(17, 10)) {
                    Audio_QueueSeqCmd(0x10F000FF);
                    play->envCtx.timeSeqState++;
                }
                break;

            case TIMESEQ_NIGHT_BEGIN_SFX:
                if (((void)0, gSaveContext.save.time) >= CLOCK_TIME(18, 0)) {
                    play->envCtx.timeSeqState++;
                }
                break;

            case TIMESEQ_EARLY_NIGHT_CRITTERS:
                if (play->envCtx.precipitation[1] < 9) {
                    Audio_PlayAmbience(play->sequenceCtx.ambienceId);
                    Audio_SetAmbienceChannelIO(1, 1, 1);
                }
                play->envCtx.timeSeqState++;
                break;

            case TIMESEQ_NIGHT_DELAY:
                if (((void)0, gSaveContext.save.time) >= CLOCK_TIME(19, 0)) {
                    play->envCtx.timeSeqState++;
                }
                break;

            case TIMESEQ_NIGHT_CRITTERS:
                Audio_SetAmbienceChannelIO(AMBIENCE_CHANNEL_CRITTER_0, 1, 0);
                Audio_SetAmbienceChannelIO(AMBIENCE_CHANNEL_CRITTER_1 << 4 | AMBIENCE_CHANNEL_CRITTER_3, 1, 1);
                play->envCtx.timeSeqState++;
                break;

            case TIMESEQ_DAY_BEGIN_SFX:
                if ((((void)0, gSaveContext.save.time) < CLOCK_TIME(19, 0)) &&
                    (((void)0, gSaveContext.save.time) >= CLOCK_TIME(5, 0))) {
                    play->envCtx.timeSeqState++;
                }
                break;

            case TIMESEQ_MORNING_CRITTERS:
                Audio_SetAmbienceChannelIO(AMBIENCE_CHANNEL_CRITTER_1 << 4 | AMBIENCE_CHANNEL_CRITTER_3, 1, 0);
                Audio_SetAmbienceChannelIO(AMBIENCE_CHANNEL_CRITTER_4 << 4 | AMBIENCE_CHANNEL_CRITTER_5, 1, 1);
                play->envCtx.timeSeqState++;
                break;

            case TIMESEQ_DAY_DELAY:
                break;

            default:
                break;
        }
    }

    if ((play->envCtx.timeSeqState != 0xFE) && (((void)0, gSaveContext.save.day) == 3) &&
        (((void)0, gSaveContext.save.time) < CLOCK_TIME(6, 0)) && !func_800FE5D0(play) &&
        (play->transitionTrigger == TRANS_TRIGGER_OFF) && (play->transitionMode == TRANS_MODE_OFF) &&
        (play->csCtx.state == 0) && ((play->sceneId != SCENE_00KEIKOKU) || (((void)0, gSaveContext.sceneLayer) != 1)) &&
        (ActorCutscene_GetCurrentIndex() == -1) &&
        (Audio_GetActiveSequence(SEQ_PLAYER_BGM_MAIN) != NA_BGM_FINAL_HOURS) &&
        (Audio_GetActiveSequence(SEQ_PLAYER_BGM_MAIN) != NA_BGM_SONG_OF_SOARING)) {
        Audio_QueueSeqCmd(0x140000FF);
        Audio_PlaySceneSequence(NA_BGM_FINAL_HOURS, 2);
    }
}

void Environment_DrawCustomLensFlare(PlayState* play) {
    Vec3f pos;

    if (gCustomLensFlare1On) {
        pos.x = gCustomLensFlare1Pos.x;
        pos.y = gCustomLensFlare1Pos.y;
        pos.z = gCustomLensFlare1Pos.z;

        Environment_DrawLensFlare(play, &play->envCtx, &play->view, play->state.gfxCtx, pos, D_801F4E44, D_801F4E48,
                                  D_801F4E4C, false);
    }

    if (gCustomLensFlare2On) {
        pos.x = gCustomLensFlare2Pos.x;
        pos.y = gCustomLensFlare2Pos.y;
        pos.z = gCustomLensFlare2Pos.z;

        Environment_DrawLensFlare(play, &play->envCtx, &play->view, play->state.gfxCtx, pos, D_801F4E5C, D_801F4E60,
                                  D_801F4E64, false);
    }
}

void Environment_InitGameOverLights(PlayState* play) {
    s32 pad;
    Player* player = GET_PLAYER(play);

    sGameOverLightsIntensity = 0;

    Lights_PointNoGlowSetInfo(&sNGameOverLightInfo, player->actor.world.pos.x - 10.0f,
                              player->actor.world.pos.y + 10.0f, player->actor.world.pos.z - 10.0f, 0, 0, 0, 255);
    sNGameOverLightNode = LightContext_InsertLight(play, &play->lightCtx, &sNGameOverLightInfo);

    Lights_PointNoGlowSetInfo(&sSGameOverLightInfo, player->actor.world.pos.x + 10.0f,
                              player->actor.world.pos.y + 10.0f, player->actor.world.pos.z + 10.0f, 0, 0, 0, 255);

    sSGameOverLightNode = LightContext_InsertLight(play, &play->lightCtx, &sSGameOverLightInfo);
}

void Environment_FadeInGameOverLights(PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 i;

    Lights_PointNoGlowSetInfo(&sNGameOverLightInfo, player->actor.world.pos.x - 10.0f,
                              player->actor.world.pos.y + 10.0f, player->actor.world.pos.z - 10.0f,
                              sGameOverLightsIntensity, sGameOverLightsIntensity, sGameOverLightsIntensity, 255);
    Lights_PointNoGlowSetInfo(&sSGameOverLightInfo, player->actor.world.pos.x + 10.0f,
                              player->actor.world.pos.y + 10.0f, player->actor.world.pos.z + 10.0f,
                              sGameOverLightsIntensity, sGameOverLightsIntensity, sGameOverLightsIntensity, 255);

    if (sGameOverLightsIntensity < 254) {
        sGameOverLightsIntensity += 2;
    }

    if (Play_CamIsNotFixed(&play->state)) {
        for (i = 0; i < 3; i++) {
            if (play->envCtx.adjLightSettings.ambientColor[i] > -255) {
                play->envCtx.adjLightSettings.ambientColor[i] -= 12;
                play->envCtx.adjLightSettings.diffuseColor1[i] -= 12;
            }
            play->envCtx.adjLightSettings.fogColor[i] = -255;
        }

        if (play->envCtx.lightSettings.fogFar + play->envCtx.adjLightSettings.fogFar > 900) {
            play->envCtx.adjLightSettings.fogFar -= 100;
        }

        if (play->envCtx.lightSettings.fogNear + play->envCtx.adjLightSettings.fogNear > 950) {
            play->envCtx.adjLightSettings.fogNear -= 10;
        }
    } else {
        play->envCtx.fillScreen = true;
        play->envCtx.screenFillColor[0] = 0;
        play->envCtx.screenFillColor[1] = 0;
        play->envCtx.screenFillColor[2] = 0;
        play->envCtx.screenFillColor[3] = sGameOverLightsIntensity;
    }
}

void Environment_FadeOutGameOverLights(PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 i;

    if (sGameOverLightsIntensity >= 3) {
        sGameOverLightsIntensity -= 3;
    } else {
        sGameOverLightsIntensity = 0;
    }

    if (sGameOverLightsIntensity == 1) {
        LightContext_RemoveLight(play, &play->lightCtx, sNGameOverLightNode);
        LightContext_RemoveLight(play, &play->lightCtx, sSGameOverLightNode);
    } else if (sGameOverLightsIntensity >= 2) {
        Lights_PointNoGlowSetInfo(&sNGameOverLightInfo, player->actor.world.pos.x - 10.0f,
                                  player->actor.world.pos.y + 10.0f, player->actor.world.pos.z - 10.0f,
                                  sGameOverLightsIntensity, sGameOverLightsIntensity, sGameOverLightsIntensity, 255);
        Lights_PointNoGlowSetInfo(&sSGameOverLightInfo, player->actor.world.pos.x + 10.0f,
                                  player->actor.world.pos.y + 10.0f, player->actor.world.pos.z + 10.0f,
                                  sGameOverLightsIntensity, sGameOverLightsIntensity, sGameOverLightsIntensity, 255);
    }

    if (Play_CamIsNotFixed(&play->state)) {
        for (i = 0; i < 3; i++) {
            Math_SmoothStepToS(&play->envCtx.adjLightSettings.ambientColor[i], 0, 5, 12, 1);
            Math_SmoothStepToS(&play->envCtx.adjLightSettings.diffuseColor1[i], 0, 5, 12, 1);
            play->envCtx.adjLightSettings.fogColor[i] = 0;
        }
        play->envCtx.adjLightSettings.fogFar = 0;
        play->envCtx.adjLightSettings.fogNear = 0;
    } else {
        play->envCtx.fillScreen = true;
        play->envCtx.screenFillColor[0] = 0;
        play->envCtx.screenFillColor[1] = 0;
        play->envCtx.screenFillColor[2] = 0;
        play->envCtx.screenFillColor[3] = sGameOverLightsIntensity;
        if (sGameOverLightsIntensity == 0) {
            play->envCtx.fillScreen = false;
        }
    }
}

void Environment_UpdateRain(PlayState* play) {
    u8 max = MAX(play->envCtx.precipitation[0], play->envCtx.precipitation[4]);

    if ((play->envCtx.precipitation[1] != max) && ((play->state.frames % 8) == 0)) {
        if (play->envCtx.precipitation[1] < max) {
            play->envCtx.precipitation[1] += 2;
        } else {
            play->envCtx.precipitation[1] -= 2;
        }
    }
}

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

extern Color_RGB8 sSandstormPrimColors[];
extern Color_RGB8 sSandstormEnvColors[];
extern u16 sSandstormScroll;

#ifdef NON_EQUIVALENT
void Environment_DrawSandstorm(PlayState* play, u8 sandstormState) {
    s32 primA1;
    s32 envA1;
    s32 primA = play->envCtx.sandstormPrimA;
    s32 envA = play->envCtx.sandstormEnvA;
    Color_RGBA8 primColor;
    Color_RGBA8 envColor;
    s32 index;
    f32 sp98;
    u16 sp96;
    u16 sp94;
    u16 sp92;

    switch (sandstormState) {
        case SANDSTORM_ACTIVE:
            envA1 = 128; // TODO: Bottom?
            primA1 = play->state.frames % 128;
            if (primA1 > 128) {
                primA1 = 255 - primA1;
            }
            primA1 += 73;
            break;

        case SANDSTORM_FILL:
            primA1 = 255;
            envA1 = (play->envCtx.sandstormPrimA >= 255) ? 255 : 128;
            break;

        case SANDSTORM_UNFILL:
        case SANDSTORM_UNK5:
            envA1 = 128;
            if (play->envCtx.sandstormEnvA > 128) {
                primA1 = 255;
            } else {
                primA1 = play->state.frames % 128;
                if (primA1 > 64) {
                    primA1 = 128 - primA1;
                }
                primA1 += 73;
            }
            if ((primA1 >= primA) && (primA1 != 255)) {
                play->envCtx.sandstormState++;
            }
            break;

        case SANDSTORM_DISSIPATE:
            envA1 = 0;
            primA1 = (play->envCtx.sandstormEnvA > 128) ? 255 : play->envCtx.sandstormEnvA >> 1;

            if (primA == 0) {
                play->envCtx.sandstormState = SANDSTORM_OFF;
            }
            break;

        case 6:
        case 8:
            envA1 = 1;
            primA1 = D_801F4E30;
            if (sandstormState == 8) {
                envA1 = 0xA;
            }
            break;

        case 7:
        case 9:
            envA1 = 1;
            if (play->envCtx.sandstormPrimA == 0) {
                play->envCtx.sandstormState = 0;
            }
            if (sandstormState == 9) {
                envA1 = 0xA;
            }
            break;

        case 10:
            envA1 = 0xFF;
            primA1 = D_801F4E30;
            if (play->envCtx.sandstormPrimA == 0) {
                play->envCtx.sandstormState = 0;
            }
            break;

        case 11:
            envA1 = 0x80;
            primA1 = play->state.frames & 0x7F;
            if (primA1 >= 0x41) {
                primA1 = 0x80 - primA1;
            }
            primA1 = primA1 + 0x49;
            break;

        case 12:
            if (play->envCtx.sandstormPrimA == 0) {
                play->envCtx.sandstormState = 0;
            }
            break;

        case 13:
            envA1 = 0xA;
            primA1 = D_801F4E30;
            break;
    }

    if (ABS_ALT(primA - primA1) < 9) {
        primA = primA1;
    } else if (primA1 < primA) {
        primA = primA - 9;
    } else {
        primA = primA + 9;
    }

    if (ABS_ALT(envA - envA1) < 9) {
        envA = envA1;
    } else if (envA1 < envA) {
        envA = envA - 9;
    } else {
        envA = envA + 9;
    }

    play->envCtx.sandstormPrimA = primA;
    play->envCtx.sandstormEnvA = envA;

    sp98 = (512.0f - (primA + envA)) * (3.0f / 128.0f);

    if (sp98 > 6.0f) {
        sp98 = 6.0f;
    }

    if (play->envCtx.sandstormPrimA != 0) {
        index = 0;
        if (sandstormState >= 0xB) {
            index = 0xC;
        }

        if ((play->envCtx.lightMode != LIGHT_MODE_TIME) ||
            (play->envCtx.lightSettingOverride != LIGHT_SETTING_OVERRIDE_NONE)) {
            primColor.r = sSandstormPrimColors[1 + index].r;
            primColor.g = sSandstormPrimColors[1 + index].g;
            primColor.b = sSandstormPrimColors[1 + index].b;
            envColor.r = sSandstormEnvColors[1 + index].r;
            envColor.g = sSandstormEnvColors[1 + index].g;
            envColor.b = sSandstormEnvColors[1 + index].b;
        } else if (sSandstormColorIndex == sNextSandstormColorIndex) {
            primColor.r = sSandstormPrimColors[sSandstormColorIndex + index].r;
            primColor.g = sSandstormPrimColors[sSandstormColorIndex + index].g;
            primColor.b = sSandstormPrimColors[sSandstormColorIndex + index].b;
            envColor.r = sSandstormEnvColors[sSandstormColorIndex + index].r;
            envColor.g = sSandstormEnvColors[sSandstormColorIndex + index].g;
            envColor.b = sSandstormEnvColors[sSandstormColorIndex + index].b;
        } else {
            primColor.r = (s32)F32_LERP(sSandstormPrimColors[sSandstormColorIndex + index].r,
                                        sSandstormPrimColors[sNextSandstormColorIndex + index].r, sSandstormLerpScale);
            primColor.g = (s32)F32_LERP(sSandstormPrimColors[sSandstormColorIndex + index].g,
                                        sSandstormPrimColors[sNextSandstormColorIndex + index].g, sSandstormLerpScale);
            primColor.b = (s32)F32_LERP(sSandstormPrimColors[sSandstormColorIndex + index].b,
                                        sSandstormPrimColors[sNextSandstormColorIndex + index].b, sSandstormLerpScale);
            envColor.r = (s32)F32_LERP(sSandstormEnvColors[sSandstormColorIndex + index].r,
                                       sSandstormEnvColors[sNextSandstormColorIndex + index].r, sSandstormLerpScale);
            envColor.g = (s32)F32_LERP(sSandstormEnvColors[sSandstormColorIndex + index].g,
                                       sSandstormEnvColors[sNextSandstormColorIndex + index].g, sSandstormLerpScale);
            envColor.b = (s32)F32_LERP(sSandstormEnvColors[sSandstormColorIndex + index].b,
                                       sSandstormEnvColors[sNextSandstormColorIndex + index].b, sSandstormLerpScale);
        }

        envColor.r = ((envColor.r * sp98) + ((6.0f - sp98) * primColor.r)) * (1.0f / 6.0f);
        envColor.g = ((envColor.g * sp98) + ((6.0f - sp98) * primColor.g)) * (1.0f / 6.0f);
        envColor.b = ((envColor.b * sp98) + ((6.0f - sp98) * primColor.b)) * (1.0f / 6.0f);

        sp96 = (u32)(sSandstormScroll * (11.0f / 6.0f));
        sp94 = (u32)(sSandstormScroll * (9.0f / 6.0f));
        sp92 = (u32)(sSandstormScroll * (6.0f / 6.0f));

        OPEN_DISPS(play->state.gfxCtx);

        POLY_XLU_DISP = func_8012C3A4(POLY_XLU_DISP);

        gDPSetAlphaDither(POLY_XLU_DISP++, G_AD_NOISE);
        gDPSetColorDither(POLY_XLU_DISP++, G_CD_NOISE);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x80, primColor.r, primColor.g, primColor.b, play->envCtx.sandstormPrimA);
        gDPSetEnvColor(POLY_XLU_DISP++, envColor.r, envColor.g, envColor.b, play->envCtx.sandstormEnvA);
        gSPSegment(POLY_XLU_DISP++, 0x08,
                   Gfx_TwoTexScroll(play->state.gfxCtx, G_TX_RENDERTILE, (u32)sp96 % 4096, 0, 512, 32, 1,
                                    (u32)sp94 % 4096, 4095 - ((u32)sp92 % 4096), 256, 64));
        gDPSetTextureLUT(POLY_XLU_DISP++, G_TT_NONE);
        gSPDisplayList(POLY_XLU_DISP++, gFieldSandstormDL);

        CLOSE_DISPS(play->state.gfxCtx);
    }

    sSandstormScroll += (s32)sp98;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/Environment_DrawSandstorm.s")
#endif

s32 Environment_AdjustLights(PlayState* play, f32 arg1, f32 arg2, f32 arg3, f32 arg4) {
    f32 temp;
    s32 i;
    Player* player = GET_PLAYER(play);

    if (play->roomCtx.curRoom.unk3 == 5) {
        return 0;
    }

    if (!Play_CamIsNotFixed(&play->state)) {
        return 0;
    }
    if (play->unk_18880 != 0) {
        return 0;
    }
    if ((player != NULL) && (player->stateFlags1 & PLAYER_STATE1_2)) {
        return 0;
    }

    arg1 = CLAMP_MIN(arg1, 0.0f);
    arg1 = CLAMP_MAX(arg1, 1.0f);

    temp = arg1 - arg3;

    if (arg1 < arg3) {
        temp = 0.0f;
    }

    play->envCtx.adjLightSettings.fogNear = (arg2 - play->envCtx.lightSettings.fogNear) * temp;

    if (arg1 == 0.0f) {
        for (i = 0; i < 3; i++) {
            play->envCtx.adjLightSettings.fogColor[i] = 0;
        }
    } else {
        temp = arg1 * 5.0f;
        temp = CLAMP_MAX(temp, 1.0f);

        for (i = 0; i < 3; i++) {
            play->envCtx.adjLightSettings.fogColor[i] = -(f32)play->envCtx.lightSettings.fogColor[i] * temp;
        }
    }

    if (arg4 <= 0.0f) {
        return 1;
    }

    arg1 *= arg4;

    for (i = 0; i < 3; i++) {
        play->envCtx.adjLightSettings.ambientColor[i] = -(f32)play->envCtx.lightSettings.ambientColor[i] * arg1;
        play->envCtx.adjLightSettings.diffuseColor1[i] = -(f32)play->envCtx.lightSettings.diffuseColor1[i] * arg1;
    }

    return 1;
}

// Get ((to - from) * lerp)
void Environment_LerpRGB8(Color_RGB8* from, Color_RGB8* to, f32 lerp, Vec3s* dst) {
    Color_RGB8 result;

    Color_RGB8_Lerp(from, to, lerp, &result);

    dst->x = result.r - from->r;
    dst->y = result.g - from->g;
    dst->z = result.b - from->b;
}

void Environment_LerpAmbientColor(PlayState* play, Color_RGB8* to, f32 lerp) {
    Environment_LerpRGB8((Color_RGB8*)play->envCtx.lightSettings.ambientColor, to, lerp,
                         (Vec3s*)&play->envCtx.adjLightSettings.ambientColor);
}

void Environment_LerpDiffuseColor(PlayState* play, Color_RGB8* to, f32 lerp) {
    Environment_LerpRGB8((Color_RGB8*)play->envCtx.lightSettings.diffuseColor1, to, lerp,
                         (Vec3s*)play->envCtx.adjLightSettings.diffuseColor1);
    Environment_LerpRGB8((Color_RGB8*)play->envCtx.lightSettings.diffuseColor, to, lerp,
                         (Vec3s*)play->envCtx.adjLightSettings.diffuseColor2);
}

void Environment_LerpFogColor(PlayState* play, Color_RGB8* to, f32 lerp) {
    Environment_LerpRGB8((Color_RGB8*)play->envCtx.lightSettings.fogColor, to, lerp,
                         (Vec3s*)play->envCtx.adjLightSettings.fogColor);
}

void Environment_LerpFog(PlayState* play, s16 fogNearTarget, s16 fogFarTarget, f32 lerp) {
    play->envCtx.adjLightSettings.fogNear = (fogNearTarget - (s16)play->envCtx.lightSettings.fogNear) * lerp;
    play->envCtx.adjLightSettings.fogFar = (fogFarTarget - (s16)play->envCtx.lightSettings.fogFar) * lerp;
}

// Repurposed from OoT to be more general
s32 Environment_GetBgsDayCount(void) {
    return gSaveContext.save.daysElapsed;
}

// Repurposed from OoT to be more general
void Environment_ClearBgsDayCount(void) {
    gSaveContext.save.daysElapsed = 0;
}

// Repurposed from OoT to be more general
s32 Environment_GetTotalDays(void) {
    return gSaveContext.save.day;
}

void Environment_ForcePlaySequence(u16 seqId) {
    gSaveContext.forcedSeqId = seqId;
}

s32 Environment_IsForcedSequenceDisabled(void) {
    s32 isDisabled = false;

    if (gSaveContext.forcedSeqId != NA_BGM_GENERAL_SFX) {
        isDisabled = true;
    }

    return isDisabled;
}

void Environment_PlayStormNatureAmbience(PlayState* play) {
    if (((play->sequenceCtx.seqId != NA_BGM_NO_MUSIC) && (play->sequenceCtx.ambienceId == AMBIENCE_ID_13)) ||
        (Audio_GetActiveSequence(SEQ_PLAYER_BGM_MAIN) == NA_BGM_FINAL_HOURS)) {
        Audio_PlayAmbience(AMBIENCE_ID_07);
    } else if ((play->sequenceCtx.seqId != NA_BGM_NO_MUSIC) && (play->sequenceCtx.ambienceId != AMBIENCE_ID_13)) {
        if ((((void)0, gSaveContext.save.time) >= CLOCK_TIME(6, 0)) &&
            (((void)0, gSaveContext.save.time) < CLOCK_TIME(18, 0))) {
            Audio_PlayAmbience(play->sequenceCtx.ambienceId);
        }
    }

    Audio_SetAmbienceChannelIO(AMBIENCE_CHANNEL_RAIN, 1, 1);
    Audio_SetAmbienceChannelIO(AMBIENCE_CHANNEL_LIGHTNING, 1, 1);
}

void Environment_StopStormNatureAmbience(PlayState* play) {
    if (((play->sequenceCtx.seqId != NA_BGM_NO_MUSIC) && (play->sequenceCtx.ambienceId == AMBIENCE_ID_13)) ||
        (Audio_GetActiveSequence(SEQ_PLAYER_BGM_MAIN) == NA_BGM_FINAL_HOURS)) {
        Audio_QueueSeqCmd(0x141400FF);
    } else if ((play->sequenceCtx.seqId != NA_BGM_NO_MUSIC) && (play->sequenceCtx.ambienceId != AMBIENCE_ID_13)) {
        if ((((void)0, gSaveContext.save.time) >= CLOCK_TIME(6, 0)) &&
            (((void)0, gSaveContext.save.time) < CLOCK_TIME(18, 0))) {
            Audio_QueueSeqCmd(0x141400FF);
        }
    }

    Audio_SetAmbienceChannelIO(AMBIENCE_CHANNEL_RAIN, 1, 0);
    Audio_SetAmbienceChannelIO(AMBIENCE_CHANNEL_LIGHTNING, 1, 0);
}

void Environment_WarpSongLeave(PlayState* play) {
    gWeatherMode = 0;
    gSaveContext.save.cutscene = 0;
    gSaveContext.respawnFlag = -3;
    play->nextEntrance = gSaveContext.respawn[RESPAWN_MODE_RETURN].entrance;
    play->transitionTrigger = TRANS_TRIGGER_START;
    play->transitionType = TRANS_TYPE_03;
    gSaveContext.nextTransitionType = TRANS_TYPE_03;
}

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

void Environment_Draw(PlayState* play) {
    func_800FD980(play);
    Environment_DrawSun(play);
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

void Environment_SetTimeJump(f32 minutes) {
    sTimeJump = CLOCK_TIME_F(0, minutes);
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

u8 func_800FE778(void) {
    return sSandstormColorIndex;
}

u8 func_800FE788(void) {
    return sNextSandstormColorIndex;
}

f32 func_800FE798(void) {
    return sSandstormLerpScale;
}

void func_800FE7A8(Color_RGBA8* arg0, Color_RGBA8* arg1) {
    f32 sp24;
    u32 pad;
    u32 index;
    u32 index2;

    sp24 = func_800FE798();
    index = func_800FE778();
    index2 = func_800FE788();
    if (sp24 <= 0.0f) {
        arg1->r = arg0[index].r;
        arg1->g = arg0[index].g;
        arg1->b = arg0[index].b;
        arg1->a = arg0[index].a;
    } else {
        arg1->r = (s32)F32_LERP(arg0[index].r, arg0[index2].r, sp24);
        arg1->g = (s32)F32_LERP(arg0[index].g, arg0[index2].g, sp24);
        arg1->b = (s32)F32_LERP(arg0[index].b, arg0[index2].b, sp24);
        arg1->a = (s32)F32_LERP(arg0[index].a, arg0[index2].a, sp24);
    }
}

u8 func_800FE9B4(PlayState* play) {
    u8 ret;

    ret = false;
    if ((play->envCtx.precipitation[4] == 0x3C) && (play->envCtx.precipitation[2] == 0)) {
        ret = true;
    }

    if ((play->envCtx.precipitation[0] == 0x3C) && !(GET_ACTIVE_CAM(play)->stateFlags & CAM_STATE_UNDERWATER)) {
        if (func_800FE4B8(play) && (play->envCtx.precipitation[2] == 0)) {
            ret = true;
        }
    }

    return ret;
}

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
    D_801F4F33 = true;
}

void func_800FEAB0(void) {
    D_801F4F33 = false;
}

void Environment_JumpForwardInTime(void) {
    if (sTimeJump != 0) {
        gSaveContext.save.time = ((void)0, gSaveContext.save.time) + sTimeJump;
        sTimeJump = 0;
    }
}

#ifdef NON_EQUIVALENT
void func_800FEAF4(EnvironmentContext* envCtx) {
    u8 temp_t8;
    u8 phi_v1;

    phi_v1 = 0;
    if (((void)0, gSaveContext.save.day) != 0) {
        phi_v1 = ((void)0, gSaveContext.save.day) - 1;
    }

    temp_t8 = phi_v1 + (D_801F4E31 * 3);
    envCtx->unk_17 = temp_t8;
    envCtx->unk_18 = temp_t8;

    if (D_801F4E31 == 4) {
        temp_t8 = 0xE;
        envCtx->unk_17 = temp_t8;
        envCtx->unk_18 = temp_t8;
    } else if (D_801F4E31 == 5) {
        temp_t8 = 0x10;
        envCtx->unk_17 = temp_t8;
        envCtx->unk_18 = temp_t8;
    } else if (D_801F4E31 == 6) {
        temp_t8 = 0x11;
        envCtx->unk_17 = temp_t8;
        envCtx->unk_18 = temp_t8;
    } else if (D_801F4E31 == 7) {
        temp_t8 = phi_v1 + 0x12;
        envCtx->unk_17 = temp_t8;
        envCtx->unk_18 = temp_t8;
    } else if (D_801F4E31 == 8) {
        temp_t8 = phi_v1 + 0x15;
        envCtx->unk_17 = temp_t8;
        envCtx->unk_18 = temp_t8;
    } else if (D_801F4E31 == 9) {
        temp_t8 = 0x18;
        envCtx->unk_17 = temp_t8;
        envCtx->unk_18 = temp_t8;
    } else if (D_801F4E31 == 0xA) {
        temp_t8 = phi_v1 + 0x19;
        envCtx->unk_17 = temp_t8;
        envCtx->unk_18 = temp_t8;
    }

    if (phi_v1 >= 3) {
        envCtx->unk_18 = envCtx->unk_17 = 0xD;
        temp_t8 = 0xD;
    }

    if (temp_t8 >= 0x1C) {
        envCtx->unk_17 = 0;
        envCtx->unk_18 = 0;
    }

    switch (((void)0, gSaveContext.save.day)) {
        default:
        case 0:
        case 1:
            envCtx->lightConfig = 0;
            envCtx->changeLightNextConfig = 0;
            break;
        case 2:
            envCtx->lightConfig = 3;
            envCtx->changeLightNextConfig = 3;
            break;
        case 3:
            envCtx->lightConfig = 4;
            envCtx->changeLightNextConfig = 4;
            break;
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/z_kankyo/func_800FEAF4.s")
#endif
