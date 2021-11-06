/*
 * File: z_en_river_sound.c
 * Overlay: ovl_En_River_Sound
 * Description:
 */

#include "z_en_river_sound.h"

#define FLAGS 0x00000030

#define THIS ((EnRiverSound*)thisx)

void EnRiverSound_Init(Actor* thisx, GlobalContext* globalCtx);
void EnRiverSound_Update(Actor* thisx, GlobalContext* globalCtx);
void EnRiverSound_Draw(Actor* thisx, GlobalContext* globalCtx);

const ActorInit En_River_Sound_InitVars = {
    ACTOR_EN_RIVER_SOUND,
    ACTORCAT_ITEMACTION,
    FLAGS,
    GAMEPLAY_KEEP,
    sizeof(EnRiverSound),
    (ActorFunc)EnRiverSound_Init,
    (ActorFunc)Actor_Noop,
    (ActorFunc)EnRiverSound_Update,
    (ActorFunc)EnRiverSound_Draw,
};

void EnRiverSound_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnRiverSound* this = THIS;
    s32 params = this->actor.params;
    Path* path;
    s32 pathIndex;
    s32 pad[2];

    this->playSound = 0;
    pathIndex = (params >> 8) & 0xFF;
    this->actor.params = params & 0xFF;
    if (pathIndex == 0xFF) {
        Actor_MarkForDeath(&this->actor);
        return;
    }

    path = &globalCtx->setupPathList[pathIndex];
    this->pathPoints = Lib_SegmentedToVirtual(path->points);
    this->pathCount = path->count;
}

void EnRiverSound_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnRiverSound* this = THIS;
    Vec3f* worldPos = &this->actor.world.pos;
    Vec3f eye;
    s32 bgId;

    Math_Vec3f_Copy(&eye, &globalCtx->view.eye);

    if (this->actor.params < 0xFD) {
        func_800BCCDC(this->pathPoints, this->pathCount, &eye, worldPos, 1);
    } else {
        func_800BCCDC(this->pathPoints, this->pathCount, &eye, worldPos, 0);
        if (func_800C411C(&globalCtx->colCtx, &this->actor.floorPoly, &bgId, this, worldPos) != BGCHECK_Y_MIN) {
            this->soundPitchIndex = func_800C9E18(&globalCtx->colCtx, this->actor.floorPoly, bgId);
        } else {
            this->soundPitchIndex = 0;
        }

        if (this->soundPitchIndex == 0) {
            this->soundPitchIndex = this->actor.params - 0xFD;
        } else {
            this->soundPitchIndex--;
        }

        this->soundPitchIndex = CLAMP_MAX(this->soundPitchIndex, 2);
    }
}

/*
u16 D_801E0BD0[22] = {
    NA_SE_EV_RIVER_STREAM - SFX_FLAG,
    NA_SE_EV_WAVE - SFX_FLAG,
    NA_SE_EV_WATER_WALL_BIG - SFX_FLAG,
    NA_SE_EV_WATER_WALL - SFX_FLAG,
    NA_SE_EV_MAGMA_LEVEL - SFX_FLAG,
    NA_SE_EV_MAGMA_LEVEL_M - SFX_FLAG,
    NA_SE_EV_MAGMA_LEVEL_L - SFX_FLAG,
    NA_SE_EV_TORCH - SFX_FLAG,
    NA_SE_EV_FOUNTAIN - SFX_FLAG,
    NA_SE_EV_DRAIN - SFX_FLAG,
    NA_SE_EV_CROWD - SFX_FLAG,
    NA_SE_EV_WATER_CONVECTION - SFX_FLAG,
    NA_SE_EV_GORON_CHEER - SFX_FLAG,
    NA_SE_EV_WATER_WALL_BIG_SILENT - SFX_FLAG,
    NA_SE_EV_WATER_BUBBLE - SFX_FLAG,
    NA_SE_EV_COW_CRY_LV - SFX_FLAG,
    NA_SE_EV_MAKE_TURRET - SFX_FLAG,
    NA_SE_EV_BOILED_WATER_S - SFX_FLAG,
    NA_SE_EV_BOILED_WATER_L - SFX_FLAG,
    NA_SE_EV_WAVE_S - SFX_FLAG,
    NA_SE_EV_WAVE_S - SFX_FLAG,
    NA_SE_EV_WAVE_S - SFX_FLAG,
};
*/

void EnRiverSound_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static f32 sFreqScale[] = {
        0.7f, // 1 / sqrt(2)
        1.0f, // 1
        1.4f, // sqrt(2)
    };
    EnRiverSound* this = THIS;
    s16 params = this->actor.params;

    if (params < 0xFD) {
        Audio_PlayActorSound2(&this->actor, D_801E0BD0[params]);
    } else {
        Audio_PlaySoundRiver(&this->actor.projectedPos, sFreqScale[this->soundPitchIndex]);
    }
}
