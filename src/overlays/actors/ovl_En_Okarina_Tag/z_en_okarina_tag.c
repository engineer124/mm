/*
 * File: z_en_okarina_tag.c
 * Overlay: ovl_En_Okarina_Tag
 * Description: Ocarina spots that responds to the ocarina and change switch flags depending on the song played
 */

#include "z_en_okarina_tag.h"

#define FLAGS (ACTOR_FLAG_10 | ACTOR_FLAG_2000000 | ACTOR_FLAG_CANT_LOCK_ON)

#define THIS ((EnOkarinaTag*)thisx)

void EnOkarinaTag_Init(Actor* thisx, PlayState* play);
void EnOkarinaTag_Destroy(Actor* thisx, PlayState* play);
void EnOkarinaTag_Update(Actor* thisx, PlayState* play);

void EnOkarinaTag_WaitForOcarina(EnOkarinaTag* this, PlayState* play);
void EnOkarinaTag_ListenToOcarina(EnOkarinaTag* this, PlayState* play);

ActorProfile En_Okarina_Tag_Profile = {
    /**/ ACTOR_EN_OKARINA_TAG,
    /**/ ACTORCAT_SWITCH,
    /**/ FLAGS,
    /**/ GAMEPLAY_KEEP,
    /**/ sizeof(EnOkarinaTag),
    /**/ EnOkarinaTag_Init,
    /**/ EnOkarinaTag_Destroy,
    /**/ EnOkarinaTag_Update,
    /**/ NULL,
};

void EnOkarinaTag_Destroy(Actor* thisx, PlayState* play) {
}

void EnOkarinaTag_Init(Actor* thisx, PlayState* play) {
    EnOkarinaTag* this = THIS;
    f32 xzRange = 0.0f;
    s32 yRange = 0;

    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;

    // Extract params
    this->type = OCARINASPOT_GET_TYPE(thisx);
    this->ocarinaSong = OCARINASPOT_GET_SONG(thisx);
    this->switchFlag = ENOKARINATAG_GET_SWITCH_FLAG(thisx);

    // Calculate interacting range
    if (this->actor.world.rot.z > 0) {
        xzRange = this->actor.world.rot.z;
        while (xzRange > 10.0f) {
            xzRange -= 10.0f;
            yRange++;
        }
    }

    this->xzRange = xzRange * 50.0f;
    this->yRange = yRange * 50.0f;

    if (this->switchFlag == ENOKARINATAG_SWITCH_FLAG_NONE) {
        this->switchFlag = SWITCH_FLAG_NONE;
    }

    if (this->ocarinaSong == OCARINASPOT_SONG_ALL) {
        this->ocarinaSong = OCARINASPOT_SONG_ALL_INTERNAL;
    }

    this->actor.targetMode = TARGET_MODE_1;
    this->actionFunc = EnOkarinaTag_WaitForOcarina;
}

void EnOkarinaTag_WaitForOcarina(EnOkarinaTag* this, PlayState* play) {
    f32 xzRange;
    f32 yRange;
    s16 yDiff;
    u16 ocarinaSong;

    if (this->switchFlag > SWITCH_FLAG_NONE) {
        if (this->type == OCARINASPOT_SET_SWITCH) {
            if (Flags_GetSwitch(play, this->switchFlag)) {
                // Switch is already set
                return;
            }
        }
        if (this->type == OCARINASPOT_UNSET_SWITCH) {
            if (!Flags_GetSwitch(play, this->switchFlag)) {
                // Switch is already unset
                return;
            }
        }
    }

    ocarinaSong = this->ocarinaSong;
    if (ocarinaSong == OCARINASPOT_SONG_SCARECROW_SPAWN) {
        ocarinaSong = OCARINASPOT_SONG_SCARECROW_SPAWN_INTERNAL;
        if (!gSaveContext.save.saveInfo.scarecrowSpawnSongSet) {
            return;
        }
    }

    if (this->ocarinaSong == OCARINASPOT_SONG_ALL_INTERNAL) {
        ocarinaSong = OCARINASPOT_SONG_ALL_INTERNAL_2;
    }

    if (Actor_AcceptOcarinaRequest(&this->actor, &play->state)) {
        // Start ocarina with the action to only allow the acceptable songs
        Message_StartOcarinaStaff(play, OCARINA_ACTION_CHECK_HEALING + ocarinaSong);
        this->actionFunc = EnOkarinaTag_ListenToOcarina;
        return;
    }

    yDiff = ABS_ALT((s16)(this->actor.yawTowardsPlayer - this->actor.world.rot.y));
    if (yDiff >= 0x4300) {
        this->debugDisplayType = 0;
        return;
    }
    this->debugDisplayType = 1;

    xzRange = this->xzRange;
    if (xzRange == 0.0f) {
        xzRange = 50000.0f;
    }

    yRange = this->yRange;
    if (yRange == 0.0f) {
        yRange = 50000.0f;
    }

    Actor_OfferOcarina(&this->actor, play, xzRange, yRange);
}

void EnOkarinaTag_ListenToOcarina(EnOkarinaTag* this, PlayState* play) {
    if (play->msgCtx.ocarinaMode == OCARINA_MODE_END) {
        this->actionFunc = EnOkarinaTag_WaitForOcarina;
        return;
    }

    if ((play->msgCtx.ocarinaMode == OCARINA_MODE_EVENT) ||
        ((this->ocarinaSong == OCARINASPOT_SONG_ALL_INTERNAL) &&
         ((play->msgCtx.ocarinaMode == OCARINA_MODE_PLAYED_TIME) ||
          (play->msgCtx.ocarinaMode == OCARINA_MODE_PLAYED_HEALING) ||
          (play->msgCtx.ocarinaMode == OCARINA_MODE_PLAYED_EPONAS) ||
          (play->msgCtx.ocarinaMode == OCARINA_MODE_PLAYED_SOARING) ||
          (play->msgCtx.ocarinaMode == OCARINA_MODE_PLAYED_SUNS) ||
          (play->msgCtx.ocarinaMode == OCARINA_MODE_PLAYED_STORMS) || (play->msgCtx.ocarinaMode == OCARINA_MODE_F)))) {
        // Correct song was played
        if (this->switchFlag > SWITCH_FLAG_NONE) {
            switch (this->type) {
                case OCARINASPOT_SET_SWITCH:
                    Flags_SetSwitch(play, this->switchFlag);
                    break;

                case OCARINASPOT_UNSET_SWITCH:
                    Flags_UnsetSwitch(play, this->switchFlag);
                    break;

                case OCARINASPOT_TOGGLE_SWITCH:
                    if (Flags_GetSwitch(play, this->switchFlag)) {
                        Flags_UnsetSwitch(play, this->switchFlag);
                    } else {
                        Flags_SetSwitch(play, this->switchFlag);
                    }
                    break;

                default:
                    break;
            }
        }
        play->msgCtx.ocarinaMode = OCARINA_MODE_END;
        Audio_PlaySfx(NA_SE_SY_CORRECT_CHIME);
        this->actionFunc = EnOkarinaTag_WaitForOcarina;
    }
}

void EnOkarinaTag_Update(Actor* thisx, PlayState* play) {
    EnOkarinaTag* this = THIS;

    this->actionFunc(this, play);
}
