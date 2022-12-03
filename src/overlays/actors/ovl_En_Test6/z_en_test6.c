/*
 * File: z_en_test6.c
 * Overlay: ovl_En_Test6
 * Description: Song of Time effects (Return to DotFD, invert, skip forward)
 */

#include "z_en_test6.h"
#include "z64quake.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define FLAGS (ACTOR_FLAG_10 | ACTOR_FLAG_20 | ACTOR_FLAG_200000 | ACTOR_FLAG_OCARINA_NO_FREEZE)

#define THIS ((EnTest6*)thisx)

// NOTES
/**
 * Z2_CLOCKTOWERActorCutsceneList_00277C
 * playerActorCsIds[8] -> index 8
 * {
 *      800, // priority
 *      -1, // length
 *      -6, // csCamSceneDataId
 *      -1, // csIndex
 *      -1, // additionalCutscene
 *      0, // sound
 *      255, // unkB
 *      0, // hud visibility
 *      0, // returnCamBehavior?
 *      32 // letterboxSize
 * },
 *
 *
 *
 *
 *
 */

void EnTest6_Init(Actor* thisx, PlayState* play2);
void EnTest6_Destroy(Actor* thisx, PlayState* play2);
void EnTest6_Update(Actor* thisx, PlayState* play);
void EnTest6_Draw(Actor* thisx, PlayState* play);

struct SoTAmmoDrops;

typedef void (*SoTAmmoDropDrawFunc)(EnTest6*, PlayState*, struct SoTAmmoDrops*);

typedef struct SoTAmmoDrops {
    /* 0x00 */ s32 type;
    /* 0x04 */ f32 scale;
    /* 0x08 */ Vec3f pos;
    /* 0x14 */ SoTAmmoDropDrawFunc draw;
} SoTAmmoDrops; // size = 0x18

// CsState here are shared and must align with actor cues in cutscene data

// Double Sot Cs:
// Cues from `SPOT00CutsceneData_009710`

// Reset Sot Cs:
// Cues from `SPOT00CutsceneData_0091A0` (Reset cycle: keep current playerForm)
// Cues from `SPOT00CutsceneData_009450` (Reset cycle: return to human playerForm)

typedef enum {
    /* 0 */ SOT_ACTOR_CUE_NONE,
    /* 1 */ DOUBLE_SOT_ACTOR_CUE_1,   // frames 11-44, 82-98, 99-100
    /* 2 */ DOUBLE_SOT_ACTOR_CUE_0,   // frames 10-11
    /* 3 */ DOUBLE_SOT_ACTOR_CUE_2,   // frames 44-59
    /* 4 */ DOUBLE_SOT_ACTOR_CUE_3,   // frames 59-69
    /* 5 */ DOUBLE_SOT_ACTOR_CUE_4,   // frames 69-82
    /* 6 */ SOT_RESET_CS_ACTOR_CUE_0, // frames 0-5
    /* 7 */ SOT_RESET_CS_ACTOR_CUE_1, // frames 5-800
    /* 8 */ SOT_RESET_CS_ACTOR_CUE_2, // frames 800-1044
    /* 9 */ DOUBLE_SOT_ACTOR_CUE_5    // frames 98-99
} SharedSotCsState;

void EnTest6_SetupAction(EnTest6* this, EnTest6ActionFunc actionFunc);

void EnTest6_StartCutscene(EnTest6* this, PlayState* play);

void EnTest6_SetupInvertedSoTCutscene(EnTest6* this, PlayState* play);
void EnTest6_InvertedSoTCutscene(EnTest6* this, PlayState* play);

void EnTest6_SetupDoubleSoTCutscene(EnTest6* this, PlayState* play);
void EnTest6_DoubleSoTCutscene(EnTest6* this, PlayState* play);

void EnTest6_SharedSoTCutscene(EnTest6* this, PlayState* play);

void EnTest6_DrawAmmoDropDefault(EnTest6* this, PlayState* play, SoTAmmoDrops* ammoDrop);
void EnTest6_DrawAmmoDropRupee(EnTest6* this, PlayState* play, SoTAmmoDrops* ammoDrop);

SoTAmmoDrops sSoTAmmoDrops[12];

ActorInit En_Test6_InitVars = {
    ACTOR_EN_TEST6,
    ACTORCAT_ITEMACTION,
    FLAGS,
    GAMEPLAY_KEEP,
    sizeof(EnTest6),
    (ActorFunc)EnTest6_Init,
    (ActorFunc)EnTest6_Destroy,
    (ActorFunc)EnTest6_Update,
    (ActorFunc)EnTest6_Draw,
};

u8 sDoubleSotCsCamData[] = {
    // Header
    0x00, 0x0D, 0x01, 0xA8, 0x00, 0x00, 0x00, 0x64,

    // Camera At Data
    /* 0x0 */ 0x04, 0x64, 0x00, 0x0D, 0x00, 0x00, 0x00, 0x15, 0xFF, 0xED, 0x00, 0x00, // At
    /* 0x1 */ 0x04, 0x64, 0x00, 0x12, 0x00, 0x00, 0x00, 0x15, 0xFF, 0xED, 0x00, 0x00, // At
    /* 0x2 */ 0x04, 0x64, 0x00, 0x10, 0x00, 0x00, 0x00, 0x15, 0xFF, 0xED, 0x00, 0x00, // At
    /* 0x3 */ 0x04, 0x64, 0x00, 0x11, 0xFF, 0xE6, 0xFF, 0xFB, 0xFF, 0xE0, 0x00, 0x00, // At
    /* 0x4 */ 0x04, 0x64, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x12, 0xFF, 0xE0, 0x00, 0x00, // At
    /* 0x5 */ 0x04, 0x64, 0x00, 0x0E, 0x00, 0x01, 0x00, 0x16, 0xFF, 0xE5, 0x00, 0x00, // At
    /* 0x6 */ 0x04, 0x64, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFB, 0x00, 0x00, // At
    /* 0x7 */ 0x04, 0x64, 0x00, 0x07, 0x00, 0x10, 0x00, 0x1D, 0xFF, 0xB3, 0x00, 0x00, // At
    /* 0x8 */ 0x04, 0x64, 0x00, 0x03, 0x00, 0x01, 0x00, 0x13, 0x00, 0x6F, 0x00, 0x00, // At
    /* 0x9 */ 0x04, 0x64, 0x00, 0x03, 0xFF, 0xC5, 0x00, 0x15, 0x00, 0x5B, 0x00, 0x00, // At
    /* 0xA */ 0x04, 0x64, 0x00, 0x03, 0xFF, 0xED, 0x00, 0x3B, 0x00, 0x54, 0x00, 0x00, // At
    /* 0xB */ 0x04, 0x64, 0x00, 0x88, 0xFF, 0xED, 0x00, 0x3B, 0x00, 0x54, 0x00, 0x00, // At
    /* 0xC */ 0x04, 0x64, 0x00, 0x6C, 0xFF, 0xEF, 0x00, 0x39, 0x00, 0x52, 0x00, 0x00, // At

    // Camera Eye Data
    /* 0x0 */ 0x04, 0x64, 0x00, 0x0D, 0x00, 0x00, 0x00, 0x32, 0x02, 0xA9, 0x00, 0x00, // Eye
    /* 0x1 */ 0x04, 0x64, 0x00, 0x12, 0x00, 0x00, 0x00, 0x32, 0x02, 0xA9, 0x00, 0x00, // Eye
    /* 0x2 */ 0x04, 0x64, 0x00, 0x10, 0x00, 0x00, 0x00, 0x32, 0x02, 0xA9, 0x00, 0x00, // Eye
    /* 0x3 */ 0x04, 0x64, 0x00, 0x11, 0xFF, 0x98, 0x01, 0x77, 0x01, 0x59, 0x00, 0x00, // Eye
    /* 0x4 */ 0x04, 0x64, 0x00, 0x0F, 0x00, 0x00, 0xFF, 0xC2, 0x01, 0x21, 0x00, 0x00, // Eye
    /* 0x5 */ 0x04, 0x64, 0x00, 0x0E, 0xFF, 0xD1, 0x00, 0x7D, 0x00, 0xCD, 0x00, 0x00, // Eye
    /* 0x6 */ 0x04, 0x64, 0x00, 0x0C, 0xFF, 0xC6, 0xFF, 0xEF, 0x00, 0xC7, 0x00, 0x00, // Eye
    /* 0x7 */ 0x04, 0x64, 0x00, 0x07, 0x00, 0x10, 0x00, 0x35, 0x00, 0xD3, 0x00, 0x00, // Eye
    /* 0x8 */ 0x04, 0x64, 0x00, 0x03, 0xFF, 0xE1, 0x00, 0x3F, 0x02, 0x6F, 0x00, 0x00, // Eye
    /* 0x9 */ 0x04, 0x64, 0x00, 0x03, 0xFE, 0xAB, 0x01, 0xD0, 0x02, 0x1E, 0x00, 0x00, // Eye
    /* 0xA */ 0x04, 0x64, 0x00, 0x03, 0xFE, 0xAB, 0x01, 0xD0, 0x02, 0x1E, 0x00, 0x00, // Eye
    /* 0xB */ 0x04, 0x64, 0x00, 0x88, 0xFE, 0xAB, 0x01, 0xD0, 0x02, 0x1E, 0x00, 0x00, // Eye
    /* 0xC */ 0x04, 0x64, 0x00, 0x6C, 0xFE, 0xAD, 0x01, 0xCE, 0x02, 0x1C, 0x00, 0x00, // Eye

    // Camera Interpolation Data
    /* 0x0 */ 0x00, 0x0F, 0x00, 0x0A, 0x00, 0x46, 0x00, 0x00, // Interp
    /* 0x1 */ 0x00, 0x0F, 0x00, 0x0A, 0x00, 0x46, 0x00, 0x00, // Interp
    /* 0x2 */ 0x00, 0x0F, 0x00, 0x0A, 0x00, 0x46, 0x00, 0x00, // Interp
    /* 0x3 */ 0x00, 0x0A, 0x00, 0x02, 0x00, 0x3C, 0x00, 0x00, // Interp
    /* 0x4 */ 0x00, 0x0A, 0xFF, 0xEC, 0x00, 0x37, 0x00, 0x00, // Interp
    /* 0x5 */ 0x00, 0x10, 0x00, 0x00, 0x00, 0x2B, 0x00, 0x00, // Interp
    /* 0x6 */ 0x00, 0x0C, 0x00, 0x06, 0x00, 0x2F, 0x00, 0x00, // Interp
    /* 0x7 */ 0x00, 0x05, 0xFF, 0xFB, 0x00, 0x32, 0x00, 0x00, // Interp
    /* 0x8 */ 0x00, 0x02, 0xFF, 0xDC, 0x00, 0x6C, 0x00, 0x00, // Interp
    /* 0x9 */ 0x00, 0x02, 0xFF, 0xD8, 0x00, 0x78, 0x00, 0x00, // Interp
    /* 0xA */ 0x00, 0x02, 0xFF, 0xC4, 0x00, 0x78, 0x00, 0x00, // Interp
    /* 0xB */ 0x00, 0x02, 0xFF, 0xBA, 0x00, 0x82, 0x00, 0x00, // Interp
    /* 0xC */ 0x00, 0x02, 0xFF, 0xB0, 0x00, 0x8C, 0x00, 0x00, // Interp

    // Terminate
    0xFF, 0xFF, 0x00, 0x04
};

typedef enum {
    /* 0 */ SOT_AMMO_DROP_NONE,
    /* 1 */ SOT_AMMO_DROP_ARROWS,
    /* 2 */ SOT_AMMO_DROP_BOMB,
    /* 3 */ SOT_AMMO_DROP_DEKU_NUT,
    /* 4 */ SOT_AMMO_DROP_DEKU_STICK,
    /* 5 */ SOT_AMMO_DROP_RUPEE_GREEN,
    /* 6 */ SOT_AMMO_DROP_RUPEE_BLUE
} SoTAmmoDropType;

typedef enum {
    /*  0 */ SOT_DRAW_DOUBLE_SOT,
    /*  1 */ SOT_DRAW_RESET_CYCLE_SOT,
    /*  2 */ SOT_DRAW_INVERTED_SOT,
    /* 99 */ SOT_DRAW_TYPE_NONE = 99
} SoTDrawType;

TexturePtr sSoTAmmoDropTextures[] = {
    NULL,              // SOT_AMMO_DROP_NONE
    gDropArrows2Tex,   // SOT_AMMO_DROP_ARROWS
    gDropBombTex,      // SOT_AMMO_DROP_BOMB
    gDropDekuNutTex,   // SOT_AMMO_DROP_DEKU_NUT
    gDropDekuStickTex, // SOT_AMMO_DROP_DEKU_STICK
    gRupeeGreenTex,    // SOT_AMMO_DROP_RUPEE_GREEN
    gRupeeBlueTex,     // SOT_AMMO_DROP_RUPEE_BLUE
};

void EnTest6_SetupCutscene(EnTest6* this, PlayState* play) {
    s32 i;
    Player* player = GET_PLAYER(play);
    s32 ammoFlags;
    f32 yOffset;

    this->actor.home.pos = player->actor.world.pos;
    this->actor.home.rot = player->actor.shape.rot;

    switch (SOT_GET_OCARINA_MODE(&this->actor)) {
        case OCARINA_MODE_APPLY_INV_SOT_FAST:
        case OCARINA_MODE_APPLY_INV_SOT_SLOW:
            EnTest6_SetupInvertedSoTCutscene(this, play);
            ActorCutscene_SetIntentToPlay(play->playerActorCsIds[8]);
            break;

        case OCARINA_MODE_APPLY_DOUBLE_SOT:
            EnTest6_SetupDoubleSoTCutscene(this, play);
            ActorCutscene_SetIntentToPlay(play->playerActorCsIds[8]);
            break;

        default: // Setup "return to first day cutscene"
            ammoFlags = 0;
            yOffset = -900.0f;

            if (CHECK_EVENTINF(EVENTINF_ENDOFCYCLE_HAS_RUPEES)) {
                // Has rupee ammo
                for (i = 0; i < 6; i++) {
                    sSoTAmmoDrops[(s32)(Rand_ZeroOne() * ARRAY_COUNT(sSoTAmmoDrops))].type = SOT_AMMO_DROP_RUPEE_GREEN;
                }
                sSoTAmmoDrops[(s32)(Rand_ZeroOne() * ARRAY_COUNT(sSoTAmmoDrops))].type = SOT_AMMO_DROP_RUPEE_GREEN;
                sSoTAmmoDrops[(s32)(Rand_ZeroOne() * ARRAY_COUNT(sSoTAmmoDrops))].type = SOT_AMMO_DROP_RUPEE_BLUE;
                ammoFlags |= 1;
            }

            if (CHECK_EVENTINF(EVENTINF_ENDOFCYCLE_HAS_ARROW_AMMO)) {
                // Has arrow ammo
                sSoTAmmoDrops[(s32)(Rand_ZeroOne() * ARRAY_COUNT(sSoTAmmoDrops))].type = SOT_AMMO_DROP_ARROWS;
                sSoTAmmoDrops[(s32)(Rand_ZeroOne() * ARRAY_COUNT(sSoTAmmoDrops))].type = SOT_AMMO_DROP_ARROWS;
                if (!(ammoFlags & 1)) {
                    sSoTAmmoDrops[(s32)(Rand_ZeroOne() * ARRAY_COUNT(sSoTAmmoDrops))].type = SOT_AMMO_DROP_ARROWS;
                    sSoTAmmoDrops[(s32)(Rand_ZeroOne() * ARRAY_COUNT(sSoTAmmoDrops))].type = SOT_AMMO_DROP_ARROWS;
                }
                ammoFlags |= 0x10;
            }

            if (CHECK_EVENTINF(EVENTINF_ENDOFCYCLE_HAS_BOMB_AMMO)) {
                // Has bomb ammo
                sSoTAmmoDrops[(s32)(Rand_ZeroOne() * ARRAY_COUNT(sSoTAmmoDrops))].type = SOT_AMMO_DROP_BOMB;
                sSoTAmmoDrops[(s32)(Rand_ZeroOne() * ARRAY_COUNT(sSoTAmmoDrops))].type = SOT_AMMO_DROP_BOMB;
                if (!(ammoFlags & 1)) {
                    sSoTAmmoDrops[(s32)(Rand_ZeroOne() * ARRAY_COUNT(sSoTAmmoDrops))].type = SOT_AMMO_DROP_BOMB;
                    sSoTAmmoDrops[(s32)(Rand_ZeroOne() * ARRAY_COUNT(sSoTAmmoDrops))].type = SOT_AMMO_DROP_BOMB;
                }
                ammoFlags |= 2;
            }

            if (CHECK_EVENTINF(EVENTINF_ENDOFCYCLE_HAS_NUT_AMMO)) {
                // Has deku nut ammo
                sSoTAmmoDrops[(s32)(Rand_ZeroOne() * ARRAY_COUNT(sSoTAmmoDrops))].type = SOT_AMMO_DROP_DEKU_NUT;
                if (!(ammoFlags & (0x10 | 0x2 | 0x1))) {
                    sSoTAmmoDrops[(s32)(Rand_ZeroOne() * ARRAY_COUNT(sSoTAmmoDrops))].type = SOT_AMMO_DROP_DEKU_NUT;
                }
                ammoFlags |= 4;
            }

            if (CHECK_EVENTINF(EVENTINF_ENDOFCYCLE_HAS_STICK_AMMO)) {
                // Has deku stick ammo
                sSoTAmmoDrops[(s32)(Rand_ZeroOne() * ARRAY_COUNT(sSoTAmmoDrops))].type = SOT_AMMO_DROP_DEKU_STICK;
                if (!(ammoFlags & (0x10 | 0x2 | 0x1))) {
                    sSoTAmmoDrops[(s32)(Rand_ZeroOne() * ARRAY_COUNT(sSoTAmmoDrops))].type = SOT_AMMO_DROP_DEKU_STICK;
                }
            }

            for (i = 0; i < ARRAY_COUNT(sSoTAmmoDrops); i++) {
                sSoTAmmoDrops[i].pos.x = ((2.0f * Rand_ZeroOne()) - 1.0f) * 80.0f;
                sSoTAmmoDrops[i].pos.z = ((2.0f * Rand_ZeroOne()) - 1.0f) * 80.0f;
                sSoTAmmoDrops[i].pos.y = (((2.0f * Rand_ZeroOne()) - 1.0f) * 40.0f) + yOffset;
                sSoTAmmoDrops[i].scale = -10.0f;
                if (sSoTAmmoDrops[i].type <= SOT_AMMO_DROP_DEKU_STICK) {
                    sSoTAmmoDrops[i].draw = EnTest6_DrawAmmoDropDefault;
                } else {
                    sSoTAmmoDrops[i].draw = EnTest6_DrawAmmoDropRupee;
                }
                yOffset += 50.0f;
            }
            break;
    }
}

void EnTest6_EnableMotionBlur(s16 alpha) {
    func_8016566C(alpha);
}

void EnTest6_DisableMotionBlur(void) {
    func_80165690();
}

void EnTest6_EnableWhiteFillScreen(PlayState* play, f32 alphaRatio) {
    play->envCtx.fillScreen = true;
    play->envCtx.screenFillColor[0] = 250;
    play->envCtx.screenFillColor[1] = 250;
    play->envCtx.screenFillColor[2] = 250;
    play->envCtx.screenFillColor[3] = 255.0f * alphaRatio;
}

void EnTest6_DisableWhiteFillScreen(PlayState* play) {
    play->envCtx.fillScreen = false;
}

void EnTest6_DrawAmmoDropDefault(EnTest6* this, PlayState* play, SoTAmmoDrops* ammoDrop) {
    s32 pad;

    OPEN_DISPS(play->state.gfxCtx);

    if (ammoDrop->type != SOT_AMMO_DROP_NONE) {
        Matrix_Translate(ammoDrop->pos.x * ammoDrop->scale, ammoDrop->pos.y, ammoDrop->pos.z * ammoDrop->scale,
                         MTXMODE_NEW);
        Matrix_Scale(ammoDrop->scale * 0.02f, ammoDrop->scale * 0.02f, ammoDrop->scale * 0.02f, MTXMODE_APPLY);
        POLY_OPA_DISP = func_801660B8(play, POLY_OPA_DISP);
        POLY_OPA_DISP = func_8012C724(POLY_OPA_DISP);

        gSPSegment(POLY_OPA_DISP++, 0x08, Lib_SegmentedToVirtual(sSoTAmmoDropTextures[ammoDrop->type]));
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, gItemDropDL);
    }

    Matrix_Translate(ammoDrop->pos.x * ammoDrop->scale, ammoDrop->pos.y, ammoDrop->pos.z * ammoDrop->scale,
                     MTXMODE_NEW);
    Matrix_Scale(2.0f * ammoDrop->scale, 2.0f * ammoDrop->scale, 2.0f * ammoDrop->scale, MTXMODE_APPLY);
    Matrix_Mult(&play->billboardMtxF, MTXMODE_APPLY);
    Matrix_RotateZS(play->state.frames * 512, MTXMODE_APPLY);
    Matrix_Translate(0.0f, 0.0f, 2.0f, MTXMODE_APPLY);

    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    func_8012C2DC(play->state.gfxCtx);

    gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 210, 210, 230, 128);
    gDPSetEnvColor(POLY_XLU_DISP++, 255, 255, 255, 0);
    gSPClearGeometryMode(POLY_XLU_DISP++, G_FOG | G_LIGHTING);
    gSPDisplayList(POLY_XLU_DISP++, gEffSparklesDL);
    gSPSetGeometryMode(POLY_XLU_DISP++, G_FOG | G_LIGHTING);

    CLOSE_DISPS(play->state.gfxCtx);
}

void EnTest6_DrawAmmoDropRupee(EnTest6* this, PlayState* play, SoTAmmoDrops* ammoDrop) {
    s32 pad;
    Gfx* gfxHead = GRAPH_ALLOC(play->state.gfxCtx, 2 * sizeof(Gfx));
    Gfx* gfx = gfxHead;
    Hilite* hilite;
    Vec3f ammoDropPos;

    ammoDropPos.x = ammoDrop->pos.x * ammoDrop->scale;
    ammoDropPos.y = ammoDrop->pos.y;
    ammoDropPos.z = ammoDrop->pos.z * ammoDrop->scale;

    hilite = func_800BCBF4(&ammoDropPos, play);

    OPEN_DISPS(play->state.gfxCtx);

    if (gfxHead != NULL) {
        func_8012C28C(play->state.gfxCtx);

        //! FAKE: & 0xFFFF
        gDPSetTileSize(gfx++, 1, hilite->h.x1 & 0xFFFF, hilite->h.y1 & 0xFFFF, (hilite->h.x1 + 60) & 0xFFFF,
                       (hilite->h.y1 + 60) & 0xFFFF);
        gSPEndDisplayList(gfx++);

        gSPSegment(POLY_OPA_DISP++, 0x07, gfxHead);

        Matrix_Translate(ammoDropPos.x, ammoDropPos.y, ammoDropPos.z, MTXMODE_NEW);
        Matrix_Scale(ammoDrop->scale * 0.018f, ammoDrop->scale * 0.018f, ammoDrop->scale * 0.018f, MTXMODE_APPLY);
        Matrix_Mult(&play->billboardMtxF, MTXMODE_APPLY);

        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPSegment(POLY_OPA_DISP++, 0x08, Lib_SegmentedToVirtual(sSoTAmmoDropTextures[ammoDrop->type]));
        gSPDisplayList(POLY_OPA_DISP++, gRupeeDL);
    }

    Matrix_Translate(ammoDrop->pos.x * ammoDrop->scale, ammoDrop->pos.y, ammoDrop->pos.z * ammoDrop->scale,
                     MTXMODE_NEW);
    Matrix_Scale(ammoDrop->scale * 2.5f, ammoDrop->scale * 2.5f, ammoDrop->scale * 2.5f, MTXMODE_APPLY);
    Matrix_Mult(&play->billboardMtxF, MTXMODE_APPLY);
    Matrix_RotateZS(play->state.frames * 256, MTXMODE_APPLY);
    Matrix_Translate(0.0f, 0.0f, 4.0f, MTXMODE_APPLY);

    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    func_8012C2DC(play->state.gfxCtx);

    gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 220, 220, 230, 192);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 0);
    gSPClearGeometryMode(POLY_XLU_DISP++, G_FOG | G_LIGHTING);
    gSPDisplayList(POLY_XLU_DISP++, gEffSparklesDL);
    gSPSetGeometryMode(POLY_XLU_DISP++, G_FOG | G_LIGHTING);

    CLOSE_DISPS(play->state.gfxCtx);
}

void EnTest6_SetupAction(EnTest6* this, EnTest6ActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void EnTest6_Init(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EnTest6* this = THIS;
    s32 i;

    if (((SOT_GET_OCARINA_MODE(&this->actor) == OCARINA_MODE_APPLY_INV_SOT_FAST) ||
         (SOT_GET_OCARINA_MODE(&this->actor) == OCARINA_MODE_APPLY_INV_SOT_SLOW) ||
         (SOT_GET_OCARINA_MODE(&this->actor) == OCARINA_MODE_APPLY_DOUBLE_SOT)) &&
        (play->playerActorCsIds[8] == -1)) {
        Actor_Kill(&this->actor);
        return;
    }

    for (i = 0; i < ARRAY_COUNT(this->lights); i++) {
        Lights_PointNoGlowSetInfo(&this->lights[i].info, this->actor.world.pos.x, (s32)this->actor.world.pos.y - 20,
                                  this->actor.world.pos.z, 255, 255, 180, -1);
        this->lights[i].node = LightContext_InsertLight(play, &play->lightCtx, &this->lights[i].info);
    }

    this->screenFillAlpha = 0;
    this->csState = SOT_ACTOR_CUE_NONE;
    this->clockYaw = 0;
    this->drawType = SOT_DRAW_TYPE_NONE;

    EnTest6_SetupCutscene(this, play);
    EnTest6_SetupAction(this, EnTest6_StartCutscene);
}

void EnTest6_Destroy(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EnTest6* this = THIS;
    s32 i;

    play->envCtx.adjLightSettings.ambientColor[0] = 0;
    play->envCtx.adjLightSettings.ambientColor[1] = 0;
    play->envCtx.adjLightSettings.ambientColor[2] = 0;

    play->envCtx.adjLightSettings.diffuseColor1[0] = 0;
    play->envCtx.adjLightSettings.diffuseColor1[1] = 0;
    play->envCtx.adjLightSettings.diffuseColor1[2] = 0;

    play->envCtx.adjLightSettings.diffuseColor2[0] = 0;
    play->envCtx.adjLightSettings.diffuseColor2[1] = 0;
    play->envCtx.adjLightSettings.diffuseColor2[2] = 0;

    play->envCtx.adjLightSettings.fogColor[0] = 0;
    play->envCtx.adjLightSettings.fogColor[1] = 0;
    play->envCtx.adjLightSettings.fogColor[2] = 0;

    play->envCtx.adjLightSettings.fogNear = 0;
    play->envCtx.adjLightSettings.fogFar = 0;

    play->envCtx.fillScreen = false;

    for (i = 0; i < ARRAY_COUNT(this->lights); i++) {
        LightContext_RemoveLight(play, &play->lightCtx, this->lights[i].node);
    }
}

void EnTest6_StartCutscene(EnTest6* this, PlayState* play) {
    switch (SOT_GET_OCARINA_MODE(&this->actor)) {
        case OCARINA_MODE_APPLY_INV_SOT_FAST:
        case OCARINA_MODE_APPLY_INV_SOT_SLOW:
            if (!ActorCutscene_GetCanPlayNext(play->playerActorCsIds[8])) {
                ActorCutscene_SetIntentToPlay(play->playerActorCsIds[8]);
            } else {
                ActorCutscene_Start(play->playerActorCsIds[8], NULL);
                this->subCamId = ActorCutscene_GetCurrentSubCamId(play->playerActorCsIds[8]);
                EnTest6_SetupAction(this, EnTest6_InvertedSoTCutscene);
            }
            break;

        case OCARINA_MODE_APPLY_DOUBLE_SOT:
            if (!ActorCutscene_GetCanPlayNext(play->playerActorCsIds[8])) {
                ActorCutscene_SetIntentToPlay(play->playerActorCsIds[8]);
            } else {
                ActorCutscene_Start(play->playerActorCsIds[8], NULL);
                this->subCamId = ActorCutscene_GetCurrentSubCamId(play->playerActorCsIds[8]);
                EnTest6_SetupAction(this, EnTest6_DoubleSoTCutscene);
            }
            break;

        default:
            gSaveContext.save.daysElapsed = 0;
            gSaveContext.save.day = 0;
            gSaveContext.save.time = CLOCK_TIME(6, 0) - 1;
            EnTest6_SetupAction(this, EnTest6_SharedSoTCutscene);
            break;
    }
}

#define INV_SOT_STATE_INIT 90
#define INV_SOT_STATE_SETUP_CLOCKS 91
#define INV_SOT_STATE_CLOCKS 95
#define INV_SOT_STATE_END 99
#define INV_SOT_STATE_UNUSED 93

void EnTest6_SetupInvertedSoTCutscene(EnTest6* this, PlayState* play) {
    this->csState = INV_SOT_STATE_INIT;
    this->timer = 100;
    this->screenFillAlpha = 0;

    if (SOT_GET_OCARINA_MODE(&this->actor) == OCARINA_MODE_APPLY_INV_SOT_SLOW) {
        play_sound(NA_SE_SY_TIME_CONTROL_SLOW);
    } else if (SOT_GET_OCARINA_MODE(&this->actor) == OCARINA_MODE_APPLY_INV_SOT_FAST) {
        play_sound(NA_SE_SY_TIME_CONTROL_NORMAL);
    }
}

void EnTest6_StopInvertedSoTCutscene(EnTest6* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    player->actor.freezeTimer = 0;
    play->unk_18844 = false;
    ActorCutscene_Stop(play->playerActorCsIds[8]);
    func_800B7298(play, NULL, PLAYER_CSMODE_6);
    EnTest6_DisableMotionBlur();
    Distortion_ClearType(DISTORTION_TYPE_5);
    Actor_Kill(&this->actor);
}

Color_RGB8 sInvSoTFogColor = { 230, 230, 220 };
Color_RGB8 sInvSoTAmbientColor = { 120, 120, 100 };
Color_RGB8 sInvSoTDiffuseColor = { 0, 0, 0 };
s16 sInvSoTFogNear = 500;
s16 sInvSoTFogFar = 1500;

void EnTest6_InvertedSoTCutscene(EnTest6* this, PlayState* play) {
    Input* input = CONTROLLER1(&play->state);
    s16 clockYaw;
    Player* player = GET_PLAYER(play);
    Camera* mainCam;
    Vec3f subCamAt;
    Vec3f subCamEye;
    Vec3f sp54;
    s32 i;
    f32 sp4C;
    Camera* subCam;

    subCam = Play_GetCamera(play, this->subCamId);
    mainCam = Play_GetCamera(play, CAM_ID_MAIN);

    // Update cutscene effects
    switch (this->csState) {
        case INV_SOT_STATE_INIT:
            this->drawType = SOT_DRAW_INVERTED_SOT;
            this->invSotEnvLerp = 0.0f;
            this->speed = 0.1f;
            this->alpha = 0;
            this->clockYaw = 0;
            this->csState = INV_SOT_STATE_SETUP_CLOCKS;
            break;

        case INV_SOT_STATE_SETUP_CLOCKS:
            this->invSotEnvLerp += this->speed;
            Environment_LerpAmbientColor(play, &sInvSoTAmbientColor, this->invSotEnvLerp);
            Environment_LerpDiffuseColor(play, &sInvSoTDiffuseColor, this->invSotEnvLerp);
            Environment_LerpFogColor(play, &sInvSoTFogColor, this->invSotEnvLerp);
            Environment_LerpFog(play, sInvSoTFogNear, sInvSoTFogFar, this->invSotEnvLerp);

            if (this->timer == 90) {
                this->alpha = 0;
                if (SOT_GET_OCARINA_MODE(&this->actor) == OCARINA_MODE_APPLY_INV_SOT_FAST) {
                    this->clockAngle = 0x200;
                    this->clockDist = 0.0f;
                    sp4C = -100.0f;
                } else {
                    this->clockAngle = 0x570;
                    this->clockDist = 110.0f;
                    sp4C = 100.0f;
                }
                this->speed = 1.0f;

                for (i = 0; i < ARRAY_COUNT(this->clockPos); i++) {
                    this->clockPos[i].x = player->actor.world.pos.x;
                    this->clockPos[i].y = player->actor.world.pos.y;
                    this->clockPos[i].z = player->actor.world.pos.z;
                }

                this->particles = ZeldaArena_Malloc(sizeof(Vec3f) * 64);
                if (this->particles != NULL) {
                    for (i = 0; i < ARRAY_COUNT(this->particles[0]); i++) {
                        (*this->particles)[i].x = (((2.0f * Rand_ZeroOne()) - 1.0f) * 40.0f) + subCam->eye.x +
                                                  ((subCam->at.x - subCam->eye.x) * 0.2f);
                        (*this->particles)[i].y = (((2.0f * Rand_ZeroOne()) - 1.0f) * 120.0f) + subCam->eye.y +
                                                  ((subCam->at.y - subCam->eye.y) * 0.2f) + sp4C;
                        (*this->particles)[i].z = (((2.0f * Rand_ZeroOne()) - 1.0f) * 40.0f) + subCam->eye.z +
                                                  ((subCam->at.z - subCam->eye.z) * 0.2f);
                    }
                }

                EnTest6_EnableMotionBlur(120);
                Distortion_SetType(DISTORTION_TYPE_5);
                Distortion_SetCountdown(80);

                play->unk_18844 = true;
                this->csState = INV_SOT_STATE_CLOCKS;
            }
            break;

        case INV_SOT_STATE_CLOCKS:
            if (this->timer > 80) {
                this->alpha += 25;
            }

            if (this->timer < 20) {
                this->alpha -= 25;
            }

            Environment_LerpAmbientColor(play, &sInvSoTAmbientColor, this->invSotEnvLerp);
            Environment_LerpDiffuseColor(play, &sInvSoTDiffuseColor, this->invSotEnvLerp);
            Environment_LerpFogColor(play, &sInvSoTFogColor, this->invSotEnvLerp);
            Environment_LerpFog(play, sInvSoTFogNear + this->alpha, sInvSoTFogFar + this->alpha, this->invSotEnvLerp);

            this->clockYaw -= this->clockAngle;
            clockYaw = this->clockYaw;

            if (SOT_GET_OCARINA_MODE(&this->actor) == OCARINA_MODE_APPLY_INV_SOT_FAST) {
                this->clockAngle += 8;
                this->clockDist += this->speed;
            } else {
                this->clockAngle -= 8;
                this->clockDist -= this->speed;
            }

            for (i = 0; i < ARRAY_COUNT(this->clockPos); i++) {
                //! FAKE:
                if (player) {}
                clockYaw += 0x10000 / ARRAY_COUNT(this->clockPos);
                this->clockPos[i].x = player->actor.world.pos.x + (Math_SinS(clockYaw) * this->clockDist);
                this->clockPos[i].y = player->actor.world.pos.y;
                this->clockPos[i].z = player->actor.world.pos.z + (Math_CosS(clockYaw) * this->clockDist);
            }

            if (this->particles != NULL) {
                for (i = 0; i < ARRAY_COUNT(this->particles[0]); i++) {
                    // Wiggle in the x-direction
                    (*this->particles)[i].x += 2.0f * ((2.0f * Rand_ZeroOne()) - 1.0f);

                    // Fall or rise depending on slow-down or speed-up
                    if (SOT_GET_OCARINA_MODE(&this->actor) == OCARINA_MODE_APPLY_INV_SOT_FAST) {
                        // Rise up
                        (*this->particles)[i].y += 1.0f;
                    } else {
                        // Fall down
                        (*this->particles)[i].y -= 1.0f;
                    }
                    // Wiggle in the z-direction
                    (*this->particles)[i].z += 2.0f * ((2.0f * Rand_ZeroOne()) - 1.0f);
                }
            }

            if (this->timer == 10) {
                this->speed = 0.1f;
                EnTest6_DisableMotionBlur();
                Distortion_ClearType(DISTORTION_TYPE_5);
                play->unk_18844 = false;
                if (this->particles != NULL) {
                    ZeldaArena_Free(this->particles);
                }
                this->csState = INV_SOT_STATE_END;
            }
            break;

        case INV_SOT_STATE_END:
            this->invSotEnvLerp -= this->speed;
            Environment_LerpAmbientColor(play, &sInvSoTAmbientColor, this->invSotEnvLerp);
            Environment_LerpDiffuseColor(play, &sInvSoTDiffuseColor, this->invSotEnvLerp);
            Environment_LerpFogColor(play, &sInvSoTFogColor, this->invSotEnvLerp);
            Environment_LerpFog(play, sInvSoTFogNear, sInvSoTFogFar, this->invSotEnvLerp);
            break;
    }

    // Update Player Cs Animation
    if (this->screenFillAlpha != 0) {
        func_800B7298(play, NULL, PLAYER_CSMODE_7);
    } else {
        if (this->timer == 90) {
            // Look side-to-side but downwards, with chin down
            // gPlayerAnim_al_elf_tobidasi
            func_800B7298(play, NULL, PLAYER_CSMODE_66);
        }

        if (this->timer == 70) {
            // close eyes and sway body in circles
            // gPlayerAnim_alink_yurayura
            func_800B7298(play, NULL, PLAYER_CSMODE_82);
        }

        if (this->timer == 30) {
            // Look side-to-side but upwards, with chin up
            // gPlayerAnim_alink_kyoro
            func_800B7298(play, NULL, PLAYER_CSMODE_81);
        }

        if (this->timer == 5) {
            // Give a big nod of approval
            // gPlayerAnim_al_yes
            func_800B7298(play, NULL, PLAYER_CSMODE_74);
        }
    }

    // Update camera
    if (this->timer > 80) {
        subCam->fov += (90.0f - subCam->fov) / (this->timer - 80);
    } else if (this->timer > 60) {
        sp4C = 1.0f / (this->timer - 60);

        subCamAt.x = subCam->at.x + ((player->actor.world.pos.x - subCam->at.x) * sp4C);
        subCamAt.y = subCam->at.y + (((player->actor.focus.pos.y - subCam->at.y) - 20.0f) * sp4C);
        subCamAt.z = subCam->at.z + ((player->actor.world.pos.z - subCam->at.z) * sp4C);

        sp54.x = (Math_SinS(player->actor.world.rot.y) * 80.0f) + subCamAt.x;
        sp54.y = subCamAt.y + 20.0f;
        sp54.z = (Math_CosS(player->actor.world.rot.y) * 80.0f) + subCamAt.z;
        sp4C *= 0.75f;

        VEC3F_LERPIMPDST(&subCamEye, &subCam->eye, &sp54, sp4C);

        Play_SetCameraAtEye(play, this->subCamId, &subCamAt, &subCamEye);
    } else if ((this->timer < 11) && (this->timer > 0)) {
        subCam->fov += (mainCam->fov - subCam->fov) / this->timer;
    }

    // Update white screen
    if (this->screenFillAlpha != 0) {
        EnTest6_EnableWhiteFillScreen(play, this->screenFillAlpha * 0.05f);
        subCam->fov += (mainCam->fov - subCam->fov) * 0.05f;
        this->screenFillAlpha++;
        if (this->screenFillAlpha >= 20) {
            this->timer = 1;
        }
    } else if ((this->timer <= 60) && (this->timer > 40) &&
               (CHECK_BTN_ALL(input->press.button, BTN_A) || CHECK_BTN_ALL(input->press.button, BTN_B))) {
        this->screenFillAlpha = 1;

        if (SOT_GET_OCARINA_MODE(&this->actor) == OCARINA_MODE_APPLY_INV_SOT_SLOW) {
            AudioSfx_StopById(NA_SE_SY_TIME_CONTROL_SLOW);
        } else if (SOT_GET_OCARINA_MODE(&this->actor) == OCARINA_MODE_APPLY_INV_SOT_FAST) {
            AudioSfx_StopById(NA_SE_SY_TIME_CONTROL_NORMAL);
        }
    }

    if (DECR(this->timer) == 0) {
        EnTest6_StopInvertedSoTCutscene(this, play);
        play->msgCtx.ocarinaMode = 4;
    }
}

void EnTest6_SetupDoubleSoTCutscene(EnTest6* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    this->timer = 120;
    this->screenFillAlpha = 0;
    this->doubleSotEnvLerp = 0.0f;
    this->actor.home.pos = player->actor.world.pos;
    this->actor.home.rot = player->actor.shape.rot;
}

void EnTest6_StopDoubleSoTCutscene(EnTest6* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    player->actor.freezeTimer = 0;
    play->unk_18844 = false;
    ActorCutscene_Stop(play->playerActorCsIds[8]);
    func_800B7298(play, NULL, PLAYER_CSMODE_6);
    EnTest6_DisableMotionBlur();
    Distortion_ClearType(DISTORTION_TYPE_5);
    Actor_Kill(&this->actor);
}

static Vec3f sSubCamUp = { 0.0f, 1.0f, 0.0f };

Color_RGB8 sDoubleSoTFogColor = { 225, 230, 225 };
Color_RGB8 sDoubleSoTAmbientColor = { 120, 120, 100 };
Color_RGB8 sDoubleSoTDiffuseColor = { 0, 0, 0 };
s16 sDoubleSoTFogNear = 940;
s16 sDoubleSoTFogFar = 2000;

void EnTest6_DoubleSoTCutscene(EnTest6* this, PlayState* play) {
    Input* input = CONTROLLER1(&play->state);
    Player* player = GET_PLAYER(play);
    Camera* subCam;
    s32 pad;
    s16 subCamId;
    s16 pad2;

    if (this->timer > 115) {
        this->doubleSotEnvLerp += 0.2f;
        EnTest6_EnableWhiteFillScreen(play, this->doubleSotEnvLerp);
    } else if (this->timer > 90) {
        this->doubleSotEnvLerp -= 0.05f;
        EnTest6_EnableWhiteFillScreen(play, this->doubleSotEnvLerp);
    } else if (this->timer == 90) {
        this->doubleSotEnvLerp = 0.0f;
        EnTest6_DisableWhiteFillScreen(play);
    }

    if (this->timer == 1) {
        this->doubleSotEnvLerp = 0.0f;
        EnTest6_DisableWhiteFillScreen(play);
    } else if (this->timer < 17) {
        this->doubleSotEnvLerp -= 0.06666666f;
        EnTest6_EnableWhiteFillScreen(play, this->doubleSotEnvLerp);
    } else if (this->timer < 22) {
        this->doubleSotEnvLerp += 0.2f;
        EnTest6_EnableWhiteFillScreen(play, this->doubleSotEnvLerp);
    }

    if (this->timer == 115) {
        Environment_LerpAmbientColor(play, &sDoubleSoTAmbientColor, 1.0f);
        Environment_LerpDiffuseColor(play, &sDoubleSoTDiffuseColor, 1.0f);
        Environment_LerpFogColor(play, &sDoubleSoTFogColor, 1.0f);
        Environment_LerpFog(play, sDoubleSoTFogNear, sDoubleSoTFogFar, 1.0f);
        play->unk_18844 = true;
    }

    if (this->timer == 15) {
        Environment_LerpAmbientColor(play, &sDoubleSoTAmbientColor, 0.0f);
        Environment_LerpDiffuseColor(play, &sDoubleSoTDiffuseColor, 0.0f);
        Environment_LerpFogColor(play, &sDoubleSoTFogColor, 0.0f);
        Environment_LerpFog(play, sDoubleSoTFogNear, sDoubleSoTFogFar, 0.0f);
        play->unk_18844 = false;
    }

    if (this->screenFillAlpha >= 20) {
        Environment_LerpAmbientColor(play, &sDoubleSoTAmbientColor, this->doubleSotEnvLerp);
        Environment_LerpDiffuseColor(play, &sDoubleSoTDiffuseColor, this->doubleSotEnvLerp);
        Environment_LerpFogColor(play, &sDoubleSoTFogColor, this->doubleSotEnvLerp);
        Environment_LerpFog(play, sDoubleSoTFogNear, sDoubleSoTFogFar, this->doubleSotEnvLerp);
        play->unk_18844 = false;
    }

    func_800B8F98(&player->actor, NA_SE_PL_FLYING_AIR - SFX_FLAG);

    switch (this->timer) {
        case 119:
            EnTest6_EnableMotionBlur(50);
            break;

        case 115:
            EnTest6_EnableMotionBlur(20);
            Distortion_SetType(DISTORTION_TYPE_5);
            Distortion_SetCountdown(90);
            this->csState = DOUBLE_SOT_ACTOR_CUE_0;
            break;

        case 110:
            Audio_PlayFanfare(NA_BGM_SONG_OF_DOUBLE_TIME);
            break;

        case 38:
        case 114:
            this->csState = DOUBLE_SOT_ACTOR_CUE_1;
            break;

        case 76:
            this->csState = DOUBLE_SOT_ACTOR_CUE_2;
            break;

        case 61:
            EnTest6_EnableMotionBlur(150);
            this->csState = DOUBLE_SOT_ACTOR_CUE_3;
            break;

        case 51:
            EnTest6_EnableMotionBlur(180);
            this->csState = DOUBLE_SOT_ACTOR_CUE_4;
            break;

        case 14:
        case 15:
            EnTest6_EnableMotionBlur(50);
            Distortion_ClearType(DISTORTION_TYPE_5);
            this->csState = SOT_ACTOR_CUE_NONE;
            break;

        case 1:
            EnTest6_DisableMotionBlur();
            if (CHECK_EVENTINF(EVENTINF_52)) {
                this->csState = DOUBLE_SOT_ACTOR_CUE_5;
            }
            break;

        default:
            break;
    }

    EnTest6_SharedSoTCutscene(this, play);

    if (this->timer == 115) {
        subCamId = ActorCutscene_GetCurrentSubCamId(play->playerActorCsIds[8]);
        subCam = Play_GetCamera(play, subCamId);

        this->subCamAt = subCam->at;
        this->subCamEye = subCam->eye;
        this->subCamFov = subCam->fov;
        func_8016119C(subCam, &this->csCamInfo);
    }

    if ((this->timer <= 115) && (this->timer >= 16)) {
        func_80161998(sDoubleSotCsCamData, &this->csCamInfo);
    } else if (this->timer < 16) {
        subCamId = ActorCutscene_GetCurrentSubCamId(play->playerActorCsIds[8]);

        Play_SetCameraAtEyeUp(play, subCamId, &this->subCamAt, &this->subCamEye, &sSubCamUp);
        Play_SetCameraFov(play, subCamId, this->subCamFov);
        Play_SetCameraRoll(play, subCamId, 0);
    }

    switch (this->timer) {
        case 116:
            player->actor.freezeTimer = 2;
            player->actor.shape.rot.x = 0;
            player->actor.shape.rot.y = 0;
            player->actor.world.pos.x = 0.0f;
            player->actor.world.pos.y = 0.0f;
            player->actor.world.pos.z = 0.0f;
            player->actor.home.pos.x = 0.0f;
            player->actor.home.pos.y = 0.0f;
            player->actor.home.pos.z = 0.0f;
            break;

        case 98:
            func_800B7298(play, NULL, PLAYER_CSMODE_64);
            break;

        case 68:
            func_800B7298(play, NULL, PLAYER_CSMODE_65);
            break;

        case 52:
            func_800B7298(play, NULL, PLAYER_CSMODE_88);
            break;

        case 43:
            func_800B7298(play, NULL, PLAYER_CSMODE_114);
            break;

        case 38:
            func_800B7298(play, NULL, PLAYER_CSMODE_7);
            break;

        case 14:
            player->actor.freezeTimer = 5;
            player->actor.world.pos = player->actor.home.pos = this->actor.home.pos;
            player->actor.shape.rot = this->actor.home.rot;
            player->actor.focus.rot.y = player->actor.shape.rot.y;
            player->currentYaw = player->actor.shape.rot.y;
            player->unk_ABC = 0.0f;
            player->unk_AC0 = 0.0f;
            player->actor.shape.yOffset = 0.0f;
            break;

        default:
            break;
    }

    if ((this->screenFillAlpha > 0) && (this->screenFillAlpha < 20)) {
        EnTest6_EnableWhiteFillScreen(play, this->screenFillAlpha * 0.05f);
        this->screenFillAlpha++;
        if (this->screenFillAlpha >= 20) {
            this->timer = 15;
            this->doubleSotEnvLerp = 0.9333333f;
        }
    } else if ((this->timer < 96) && (this->timer > 50) &&
               (CHECK_BTN_ALL(input->press.button, BTN_A) || CHECK_BTN_ALL(input->press.button, BTN_B))) {
        this->screenFillAlpha = 1;
        this->timer = 39;
        Audio_QueueSeqCmd(0x111400FF);
    }

    if (DECR(this->timer) == 0) {
        EnTest6_StopDoubleSoTCutscene(this, play);
    }
}

void EnTest6_Update(Actor* thisx, PlayState* play) {
    EnTest6* this = THIS;

    this->actionFunc(this, play);
}

/**
 * Processes two different cutscenes:
 * return to "Dawn of the First Day" Cs, and Song of Double Time Cs
 */
void EnTest6_SharedSoTCutscene(EnTest6* this, PlayState* play) {
    s32 pad[2];
    Player* player = GET_PLAYER(play);
    f32 temp_f0;
    s32 i;
    s32 actionIndex;

    if (Cutscene_CheckActorAction(play, 0x1F9)) {
        actionIndex = Cutscene_GetActorActionIndex(play, 0x1F9);
        this->csState = play->csCtx.actorActions[actionIndex]->action;

        switch (this->csState) {
            case DOUBLE_SOT_ACTOR_CUE_1:
                break;

            case DOUBLE_SOT_ACTOR_CUE_0:
                this->drawType = SOT_DRAW_DOUBLE_SOT;
                this->clockYaw = 0;
                this->clockAngle = 0;
                player->actor.shape.shadowDraw = NULL;

                if (play->csCtx.actorActions[actionIndex]->startPos.x != 0) {
                    this->clockSpeed = (u32)play->csCtx.actorActions[actionIndex]->startPos.x;
                } else {
                    this->clockSpeed = 150.0f;
                }

                if (play->csCtx.actorActions[actionIndex]->startPos.y != 0) {
                    this->clockColorGray = play->csCtx.actorActions[actionIndex]->startPos.y;
                } else {
                    this->clockColorGray = 38;
                }

                if (play->csCtx.actorActions[actionIndex]->startPos.z != 0) {
                    this->clockDist = (u32)play->csCtx.actorActions[actionIndex]->startPos.z;
                } else {
                    this->clockDist = 480.0f;
                }
                break;

            case DOUBLE_SOT_ACTOR_CUE_2:
                if (play->csCtx.actorActions[actionIndex]->startPos.x != 0) {
                    this->clockSpeed += (u32)play->csCtx.actorActions[actionIndex]->startPos.x;
                }

                if (play->csCtx.actorActions[actionIndex]->startPos.y != 0) {
                    this->clockColorGray += (s16)play->csCtx.actorActions[actionIndex]->startPos.y;

                } else {
                    this->clockColorGray += 6;
                }

                if (play->csCtx.actorActions[actionIndex]->startPos.z != 0) {
                    this->clockDistSpeed = (u32)play->csCtx.actorActions[actionIndex]->startPos.z;
                } else {
                    this->clockDistSpeed = -32.0f;
                }
                this->clockDist += this->clockDistSpeed;
                break;

            case DOUBLE_SOT_ACTOR_CUE_3:
                if (play->csCtx.actorActions[actionIndex]->startPos.x != 0) {
                    this->clockSpeed += (u32)play->csCtx.actorActions[actionIndex]->startPos.x;
                }

                if (play->csCtx.actorActions[actionIndex]->startPos.y != 0) {
                    this->clockColorGray += (s16)play->csCtx.actorActions[actionIndex]->startPos.y;
                } else {
                    this->clockColorGray -= 4;
                }
                break;

            case DOUBLE_SOT_ACTOR_CUE_4:
                if (play->csCtx.actorActions[actionIndex]->startPos.x != 0) {
                    this->clockSpeed += (u32)play->csCtx.actorActions[actionIndex]->startPos.x;
                }

                if (play->csCtx.actorActions[actionIndex]->startPos.y != 0) {
                    this->clockColorGray += (s16)play->csCtx.actorActions[actionIndex]->startPos.y;
                } else {
                    this->clockColorGray -= 8;
                }

                if (play->csCtx.actorActions[actionIndex]->startPos.z != 0) {
                    this->clockDistSpeed += (u32)play->csCtx.actorActions[actionIndex]->startPos.z;
                } else {
                    this->clockDistSpeed += 20.0f;
                }

                this->clockDist += this->clockDistSpeed;
                if (this->clockDist > 3500.0f) {
                    this->clockDist = 3500.0f;
                    this->csState = SOT_ACTOR_CUE_NONE;
                }
                break;

            case SOT_RESET_CS_ACTOR_CUE_0:
                this->drawType = SOT_DRAW_RESET_CYCLE_SOT;
                this->clockYaw = 0;
                this->clockAngle = 0;
                player->actor.shape.shadowDraw = NULL;

                if (play->csCtx.actorActions[actionIndex]->startPos.x != 0) {
                    this->clockSpeed = (u32)play->csCtx.actorActions[actionIndex]->startPos.x;
                } else {
                    this->clockSpeed = 100.0f;
                }

                if (play->csCtx.actorActions[actionIndex]->startPos.y != 0) {
                    this->speed = (u32)play->csCtx.actorActions[actionIndex]->startPos.y;
                } else {
                    this->speed = 20.0f;
                }

                if (play->csCtx.actorActions[actionIndex]->startPos.z != 0) {
                    this->clockDist = (u32)play->csCtx.actorActions[actionIndex]->startPos.z;
                } else {
                    this->clockDist = 300.0f;
                }
                this->clockDistSpeed = 0.0f;
                break;

            case SOT_RESET_CS_ACTOR_CUE_1:
                if (play->csCtx.actorActions[actionIndex]->startPos.x != 0) {
                    this->clockDistSpeed = (u32)play->csCtx.actorActions[actionIndex]->startPos.x;
                } else {
                    this->clockDistSpeed = -5.0f;
                }
                this->clockSpeed += this->clockDistSpeed;
                break;

            case SOT_RESET_CS_ACTOR_CUE_2:
                if (play->csCtx.actorActions[actionIndex]->startPos.x != 0) {
                    this->clockDistSpeed += (u32)play->csCtx.actorActions[actionIndex]->startPos.x;
                } else {
                    this->clockDistSpeed += 2.0f;
                }

                this->clockSpeed += this->clockDistSpeed;
                if (this->clockSpeed > 10000.0f) {
                    this->clockSpeed = 10000.0f;
                    this->csState = SOT_ACTOR_CUE_NONE;
                }
                break;

            case SOT_ACTOR_CUE_NONE:
            default:
                this->drawType = SOT_DRAW_TYPE_NONE;
                return;

            case DOUBLE_SOT_ACTOR_CUE_5:
                Play_SetRespawnData(&play->state, 1, ((void)0, gSaveContext.save.entrance & 0xFFFF), player->unk_3CE,
                                    PLAYER_PARAMS(0xFF, PLAYER_INITMODE_B), &player->unk_3C0, player->unk_3CC);
                this->drawType = SOT_DRAW_TYPE_NONE;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->nextEntrance = gSaveContext.respawn[RESPAWN_MODE_RETURN].entrance;
                play->transitionType = TRANS_TYPE_02;
                if ((gSaveContext.save.time > CLOCK_TIME(18, 0)) || (gSaveContext.save.time < CLOCK_TIME(6, 0))) {
                    gSaveContext.respawnFlag = -99;
                    SET_EVENTINF(EVENTINF_27);
                } else {
                    gSaveContext.respawnFlag = 2;
                }
                play->msgCtx.ocarinaMode = 4;
                return;
        }
    } else {
        switch (this->csState) {
            case DOUBLE_SOT_ACTOR_CUE_0:
                this->drawType = SOT_DRAW_DOUBLE_SOT;
                this->clockYaw = 0;
                this->clockAngle = 0;
                player->actor.shape.shadowDraw = NULL;
                this->clockColorGray = 38;
                this->clockSpeed = 150.0f;
                this->clockDist = 480.0f;

            case DOUBLE_SOT_ACTOR_CUE_1:
                break;

            case DOUBLE_SOT_ACTOR_CUE_2:
                this->clockDistSpeed = -32.0f;
                this->clockColorGray += 6;
                this->clockDist += -32.0f;
                break;

            case DOUBLE_SOT_ACTOR_CUE_3:
                this->clockColorGray -= 4;
                break;

            case DOUBLE_SOT_ACTOR_CUE_4:
                this->clockColorGray -= 8;
                this->clockDistSpeed += 20.0f;
                this->clockDist += this->clockDistSpeed;
                if (this->clockDist > 3500.0f) {
                    this->clockDist = 3500.0f;
                    this->csState = SOT_ACTOR_CUE_NONE;
                }
                break;

            case SOT_RESET_CS_ACTOR_CUE_0:
                this->drawType = SOT_DRAW_RESET_CYCLE_SOT;
                this->clockYaw = 0;
                this->clockAngle = 0;
                player->actor.shape.shadowDraw = NULL;
                this->clockSpeed = 100.0f;
                this->speed = 20.0f;
                this->clockDist = 300.0f;
                this->clockDistSpeed = 0.0f;
                break;

            case SOT_RESET_CS_ACTOR_CUE_1:
                this->clockDistSpeed = -5.0f;
                this->clockSpeed += -5.0f;
                break;

            case SOT_RESET_CS_ACTOR_CUE_2:
                this->clockDistSpeed += 2.0f;
                this->clockSpeed += this->clockDistSpeed;
                if (this->clockSpeed > 10000.0f) {
                    this->clockSpeed = 10000.0f;
                    this->csState = SOT_ACTOR_CUE_NONE;
                }
                break;

            case SOT_ACTOR_CUE_NONE:
            default:
                this->drawType = SOT_DRAW_TYPE_NONE;
                return;

            case DOUBLE_SOT_ACTOR_CUE_5:
                if (gSaveContext.save.time > CLOCK_TIME(12, 0)) {
                    Play_SetRespawnData(&play->state, 1, ((void)0, gSaveContext.save.entrance & 0xFFFF),
                                        player->unk_3CE, PLAYER_PARAMS(0xFF, PLAYER_INITMODE_B), &player->unk_3C0,
                                        player->unk_3CC);
                    this->drawType = SOT_DRAW_TYPE_NONE;
                    play->transitionTrigger = TRANS_TRIGGER_START;
                    play->nextEntrance = gSaveContext.respawn[RESPAWN_MODE_RETURN].entrance;
                    play->transitionType = TRANS_TYPE_02;
                    gSaveContext.respawnFlag = 2;
                    play->msgCtx.ocarinaMode = 4;
                }
                return;
        }
    }

    if (this->drawType == SOT_DRAW_RESET_CYCLE_SOT) {
        for (i = 0; i < ARRAY_COUNT(sSoTAmmoDrops); i++) {
            sSoTAmmoDrops[i].pos.x += 2.0f * ((2.0f * Rand_ZeroOne()) - 1.0f);
            sSoTAmmoDrops[i].pos.z += 2.0f * ((2.0f * Rand_ZeroOne()) - 1.0f);
            sSoTAmmoDrops[i].pos.y += 3.0f;

            if (player->actor.world.pos.y < sSoTAmmoDrops[i].pos.y) {
                temp_f0 = sSoTAmmoDrops[i].pos.y - player->actor.world.pos.y;
                if (temp_f0 > 550.0f) {
                    temp_f0 = 1.0f;
                } else {
                    temp_f0 /= 550.0f;
                }
                sSoTAmmoDrops[i].scale = SQ(temp_f0);
            } else {
                sSoTAmmoDrops[i].scale = -10.0f;
            }
        }
    }
    this->clockYaw++;
}

/**
 * Draws 128 clocks and ammo drops
 */
void EnTest6_DrawResetCycleSotCs(EnTest6* this, PlayState* play) {
    s16 clockYaw1;
    s16 clockYaw2;
    s16 angle;
    s32 i;
    Vec3f clockPos;

    OPEN_DISPS(play->state.gfxCtx);

    this->gfx = POLY_OPA_DISP;
    clockPos.y = 0.0f;

    clockYaw1 = this->clockAngle;
    clockYaw2 = (s32)(Math_SinS(play->state.frames) * 12000.0f) + clockYaw1 + 0x4E20;
    angle = (play->state.frames & 0x3C) * 1024;
    angle *= (this->clockSpeed / 200.0f);
    this->clockAngle += (s16)this->clockSpeed;
    this->clockRotZ = (s16)((this->clockSpeed / 200.0f) * 256.0f);

    // Draw clocks
    for (i = 0; i < 64; i++) {
        clockYaw1 += 0x10000 / 16;
        clockPos.x = Math_CosS(clockYaw1) * this->clockDist;
        clockPos.z = Math_SinS(clockYaw1) * this->clockDist;
        Matrix_Translate(clockPos.x, clockPos.y, clockPos.z, MTXMODE_NEW);
        Matrix_RotateXS(0x4000, MTXMODE_APPLY);
        Matrix_Scale(0.8f, 0.8f, 0.8f, MTXMODE_APPLY);
        Matrix_RotateZS(angle, MTXMODE_APPLY);

        gSPMatrix(this->gfx++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gDPSetPrimColor(this->gfx++, 0, 0xFF, 28, 28, 28, 255);
        gDPSetEnvColor(this->gfx++, 255, 255, 255, 255);
        gDPSetRenderMode(this->gfx++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2);
        gSPDisplayList(this->gfx++, gSongOfTimeClockDL);

        clockYaw2 += 0x10000 / 16;
        clockPos.x = Math_CosS(clockYaw2) * this->clockDist;
        clockPos.z = Math_SinS(clockYaw2) * this->clockDist;
        Matrix_Translate(clockPos.x, clockPos.y, clockPos.z, MTXMODE_NEW);
        Matrix_RotateXS(0x4000, MTXMODE_APPLY);
        Matrix_Scale(0.8f, 0.8f, 0.8f, MTXMODE_APPLY);
        Matrix_RotateZS(-angle, MTXMODE_APPLY);

        gSPMatrix(this->gfx++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gDPSetPrimColor(this->gfx++, 0, 0xFF, 28, 28, 28, 255);
        gDPSetEnvColor(this->gfx++, 255, 255, 255, 255);
        gDPSetRenderMode(this->gfx++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2);
        gSPDisplayList(this->gfx++, gSongOfTimeClockDL);

        clockPos.y -= this->speed;
        angle += this->clockRotZ;
    }

    POLY_OPA_DISP = this->gfx;

    for (i = 0; i < ARRAY_COUNT(sSoTAmmoDrops); i++) {
        if (sSoTAmmoDrops[i].scale > 0.0f) {
            sSoTAmmoDrops[i].draw(this, play, &sSoTAmmoDrops[i]);
        }
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

/**
 * Draws 51 clocks
 */
void EnTest6_DrawDoubleSotCs(Actor* thisx, PlayState* play2) {
    EnTest6* this = THIS;
    PlayState* play = play2;
    Vec3f clockPos;
    s16 singleClockPitch;
    s16 clockRotZ;
    s32 i;
    Player* player = GET_PLAYER(play);
    s32 pad;

    //! FAKE:
    if (this) {}

    OPEN_DISPS(play->state.gfxCtx);

    this->gfx = POLY_OPA_DISP;
    this->clockAngle += (s16)this->clockSpeed;
    this->clockRotZ = this->clockAngle * 2;
    clockRotZ = (play->state.frames & 0x3C) * 1024;
    singleClockPitch = this->clockAngle + 0x4000;

    // All cases have the exact same code
    switch (player->transformation) {
        case PLAYER_FORM_DEKU:
            clockPos.x = player->actor.world.pos.y + 40.0f;
            break;

        case PLAYER_FORM_GORON:
            clockPos.x = player->actor.world.pos.y + 40.0f;
            break;

        case PLAYER_FORM_ZORA:
            clockPos.x = player->actor.world.pos.y + 40.0f;
            break;

        case PLAYER_FORM_FIERCE_DEITY:
            clockPos.x = player->actor.world.pos.y + 40.0f;
            break;

        default:
            clockPos.x = player->actor.world.pos.y + 40.0f;
            break;
    }

    // Draw clocks
    for (i = 0; i < 51; i++) {
        clockPos.y = Math_CosS(singleClockPitch) * this->clockDist;
        clockPos.z = Math_SinS(singleClockPitch) * this->clockDist;
        // Rotate
        Matrix_RotateZS(this->clockRotZ, MTXMODE_NEW);
        Matrix_Translate(clockPos.x, clockPos.y, clockPos.z, MTXMODE_APPLY);
        Matrix_Scale(0.85f, 0.85f, 0.85f, MTXMODE_APPLY);
        Matrix_RotateXS(singleClockPitch, MTXMODE_APPLY);
        Matrix_RotateZS(clockRotZ, MTXMODE_APPLY);

        gSPMatrix(this->gfx++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gDPSetPrimColor(this->gfx++, 0, 0xFF, this->clockColorGray, this->clockColorGray, this->clockColorGray, 255);
        gDPSetEnvColor(this->gfx++, 235, 238, 235, 255);
        gDPSetRenderMode(this->gfx++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2);
        gSPDisplayList(this->gfx++, gSongOfTimeClockDL);

        singleClockPitch += 0x10000 / 51;
    }

    Lights_PointSetPosition(&this->lights[0].info, player->actor.world.pos.x, player->actor.world.pos.y - 10.0f,
                            player->actor.world.pos.z);
    Lights_PointSetColorAndRadius(&this->lights[0].info, 100, 250, 100, 200);

    POLY_OPA_DISP = this->gfx;

    CLOSE_DISPS(play->state.gfxCtx);
}

/**
 * Draws 6 clocks and black particles
 */
void EnTest6_DrawInvertedSoTCs(EnTest6* this, PlayState* play2) {
    PlayState* play = play2;
    Player* player = GET_PLAYER(play);
    f32 flashScale;
    s32 i;

    OPEN_DISPS(play->state.gfxCtx);

    switch (this->csState) {
        case INV_SOT_STATE_SETUP_CLOCKS:
        case INV_SOT_STATE_UNUSED:
            Lights_PointSetPosition(&this->lights[0].info, player->actor.world.pos.x, player->actor.world.pos.y - 10.0f,
                                    player->actor.world.pos.z);
            Lights_PointSetColorAndRadius(&this->lights[0].info, 245, 245, 200, this->alpha);
            break;

        case INV_SOT_STATE_CLOCKS:
            this->gfx = POLY_XLU_DISP;

            // Draw clocks
            for (i = 0; i < ARRAY_COUNT(this->clockPos); i++) {
                Matrix_Translate(this->clockPos[i].x, this->clockPos[i].y, this->clockPos[i].z, MTXMODE_NEW);
                Matrix_Scale(0.3f, 0.3f, 0.3f, MTXMODE_APPLY);
                Matrix_RotateXS(-0x4000, MTXMODE_APPLY);
                Matrix_RotateZS(this->clockYaw, MTXMODE_APPLY);

                gSPMatrix(this->gfx++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gDPSetPrimColor(this->gfx++, 0, 0xFF, 28, 28, 28, 255);
                gDPSetEnvColor(this->gfx++, 245, 245, 200, this->alpha);
                gDPSetRenderMode(this->gfx++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_XLU_SURF2);
                gSPDisplayList(this->gfx++, gSongOfTimeClockDL);

                POLY_XLU_DISP = this->gfx;
            }

            // Draw black particles
            if (this->particles != NULL) {
                for (i = 0; i < ARRAY_COUNT(this->particles[0]); i++) {
                    flashScale = Rand_ZeroOne() * 0.0025f;
                    Matrix_Translate((*this->particles)[i].x, (*this->particles)[i].y, (*this->particles)[i].z,
                                     MTXMODE_NEW);
                    Matrix_Scale(flashScale, flashScale, flashScale, MTXMODE_APPLY);

                    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x80, 128, 128, 128, this->alpha >> 1);
                    gDPSetEnvColor(POLY_XLU_DISP++, 230, 230, 180, this->alpha);

                    func_8012C2DC(play->state.gfxCtx);
                    Matrix_Mult(&play->billboardMtxF, MTXMODE_APPLY);
                    Matrix_RotateZS(this->clockYaw + (i << 2), MTXMODE_APPLY);

                    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx),
                              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                    gSPDisplayList(POLY_XLU_DISP++, gEffFlash1DL);
                }
            }

            Lights_PointSetPosition(&this->lights[0].info, player->actor.world.pos.x, player->actor.world.pos.y - 10.0f,
                                    player->actor.world.pos.z);
            Lights_PointSetColorAndRadius(&this->lights[0].info, 250, 250, 100, this->alpha);
            break;

        case INV_SOT_STATE_INIT:
        case INV_SOT_STATE_END:
        default:
            break;
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

void EnTest6_Draw(Actor* thisx, PlayState* play) {
    EnTest6* this = THIS;

    if (this->csState != 0) {
        switch (this->drawType) {
            case SOT_DRAW_RESET_CYCLE_SOT:
                EnTest6_DrawResetCycleSotCs(this, play);
                break;

            case SOT_DRAW_DOUBLE_SOT:
                EnTest6_DrawDoubleSotCs(thisx, play);
                break;

            case SOT_DRAW_INVERTED_SOT:
                EnTest6_DrawInvertedSoTCs(this, play);
                break;

            default:
                break;
        }
    }
}
