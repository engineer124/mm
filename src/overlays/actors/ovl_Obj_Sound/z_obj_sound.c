/*
 * File: z_obj_sound.c
 * Overlay: ovl_Obj_Sound
 * Description: Plays certain sounds (e.g., swamp waterfall noise)
 */

#include "z_obj_sound.h"

#define FLAGS (ACTOR_FLAG_10 | ACTOR_FLAG_20)

#define THIS ((ObjSound*)thisx)

void ObjSound_Init(Actor* thisx, PlayState* play);
void ObjSound_Destroy(Actor* thisx, PlayState* play);
void ObjSound_Update(Actor* thisx, PlayState* play);
void ObjSound_Draw(Actor* thisx, PlayState* play);

/**
 * Sound Modes:
 *
 * Mode 0:
 *   - NA_SE_EV_FOUNTAIN
 *   - NA_SE_EV_COW_CRY_LV
 *   - NA_SE_EV_WATER_WALL_BIG
 *   - NA_SE_EV_WATER_WALL
 *   - NA_SE_EV_MAKE_TURRET
 *   - NA_SE_EV_WATER_BUBBLE
 *   - NA_SE_EV_DRAIN
 *   - NA_SE_EV_BOILED_WATER_S
 * Mode 1: ()
 *   - NA_BGM_SHOP
 *   - NA_BGM_MINI_GAME
 *   - NA_BGM_MILK_BAR
 *   - NA_BGM_MILK_BAR_DUPLICATE
 *   - NA_BGM_ASTRAL_OBSERVATORY
 * Mode 2:
 *   - Unused in scenes
 * Mode 3: (Fanfares)
 *   - NA_BGM_SWAMP_CRUISE
 */

const ActorInit Obj_Sound_InitVars = {
    ACTOR_OBJ_SOUND,
    ACTORCAT_ITEMACTION,
    FLAGS,
    GAMEPLAY_KEEP,
    sizeof(ObjSound),
    (ActorFunc)ObjSound_Init,
    (ActorFunc)ObjSound_Destroy,
    (ActorFunc)ObjSound_Update,
    (ActorFunc)NULL,
};

void ObjSound_Init(Actor* thisx, PlayState* play) {
    ObjSound* this = THIS;

    this->unk_144 = false;
    this->unk_146 = (this->actor.params >> 8) & 0xFF;
    this->unk_145 = (this->actor.params >> 7) & 1;
    this->actor.params &= 0x7F;
    if (this->unk_146 == 3) {
        this->actor.draw = ObjSound_Draw;
    }
}

void ObjSound_Destroy(Actor* thisx, PlayState* play) {
    ObjSound* this = THIS;

    if (this->unk_146 == 1) {
        Audio_PlayMainBgmAtPos(NULL, NA_BGM_GENERAL_SFX);
    }
}

void ObjSound_Update(Actor* thisx, PlayState* play) {
    ObjSound* this = THIS;

    if (this->unk_146 == 0) {
        if (this->unk_145) {
            func_800B9010(&this->actor, gAudioEnvironmentalSfx[this->actor.params]);
        } else {
            func_800B8FE8(&this->actor, gAudioEnvironmentalSfx[this->actor.params]);
        }
    } else if (this->unk_144) {
        if (this->unk_146 == 1) {
            Audio_PlayMainBgmAtPos(&this->actor.projectedPos, this->actor.params);
        } else if (this->unk_146 == 2) {
            Audio_PlaySfxAtFixedPos(&this->actor.projectedPos, gAudioEnvironmentalSfx[this->actor.params]);
        }
    } else {
        this->unk_144 = true;
    }
}

void ObjSound_Draw(Actor* thisx, PlayState* play) {
    ObjSound* this = THIS;

    if ((gSaveContext.eventInf[4] & 2) || (gSaveContext.eventInf[3] & 0x20)) {
        Audio_PlayFanfareAtPos(&this->actor.projectedPos, this->actor.params);
    }
}
