/*
 * File: z_en_river_sound.c
 * Overlay: ovl_En_River_Sound
 * Description: Various environment Sfx along a path (e.g., ocean waves, gulls, waterfall, lava, etc)
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
        if (BgCheck_EntityRaycastFloor5(&globalCtx->colCtx, &this->actor.floorPoly, &bgId, &this->actor, worldPos) != BGCHECK_Y_MIN) {
            this->soundFreqIndex = SurfaceType_GetConveyorSpeed(&globalCtx->colCtx, this->actor.floorPoly, bgId);
        } else {
            this->soundFreqIndex = 0;
        }

        if (this->soundFreqIndex == 0) {
            this->soundFreqIndex = this->actor.params - 0xFD;
        } else {
            this->soundFreqIndex--;
        }

        this->soundFreqIndex = CLAMP_MAX(this->soundFreqIndex, 2);
    }
}

void EnRiverSound_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static f32 freqScale[] = {
        0.7f, // 1 / sqrt(2)
        1.0f, // 1
        1.4f, // sqrt(2)
    };
    EnRiverSound* this = THIS;
    s16 params = this->actor.params;

    if (params < 0xFD) {
        Audio_PlayActorSound2(&this->actor, gAudioEnvironmentalSfx[params]);
    } else {
        Audio_PlaySfxForRiver(&this->actor.projectedPos, freqScale[this->soundFreqIndex]);
    }
}
