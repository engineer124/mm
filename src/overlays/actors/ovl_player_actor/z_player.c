/*
 * File: z_player.c
 * Overlay: ovl_player_actor
 * Description: Player
 */

#include "global.h"
#include "z64rumble.h"

#include "overlays/actors/ovl_Arms_Hook/z_arms_hook.h"
#include "overlays/actors/ovl_Door_Spiral/z_door_spiral.h"
#include "overlays/actors/ovl_Door_Shutter/z_door_shutter.h"
#include "overlays/actors/ovl_En_Ishi/z_en_ishi.h"
#include "overlays/actors/ovl_En_Test3/z_en_test3.h"
#include "overlays/actors/ovl_En_Test7/z_en_test7.h"
#include "overlays/actors/ovl_En_Torch2/z_en_torch2.h"

#include "objects/gameplay_keep/gameplay_keep.h"

#include "objects/object_link_boy/object_link_boy.h"
#include "objects/object_link_goron/object_link_goron.h"
#include "objects/object_link_zora/object_link_zora.h"
#include "objects/object_link_nuts/object_link_nuts.h"
#include "objects/object_link_child/object_link_child.h"

#define THIS ((Player*)thisx)

extern UNK_TYPE D_06008860;
extern Gfx D_0600BDD8[];
extern AnimationHeader D_060178D0;

void Player_Init(Actor* thisx, PlayState* play);
void Player_Destroy(Actor* thisx, PlayState* play);
void Player_Update(Actor* thisx, PlayState* play);
void Player_Draw(Actor* thisx, PlayState* play);

extern LinkAnimationHeader D_0400CF98;
extern LinkAnimationHeader D_0400D0A8;
extern LinkAnimationHeader D_0400D0B0;
extern LinkAnimationHeader D_0400D0C8;
extern LinkAnimationHeader D_0400D0D0;
extern LinkAnimationHeader D_0400D100;
extern LinkAnimationHeader D_0400D218;
extern LinkAnimationHeader D_0400D220;
extern LinkAnimationHeader D_0400D228;
extern LinkAnimationHeader D_0400D3E8;
extern LinkAnimationHeader D_0400D3F8;
extern LinkAnimationHeader D_0400D420;
extern LinkAnimationHeader D_0400D488;
extern LinkAnimationHeader D_0400D490;
extern LinkAnimationHeader D_0400D4A8;
extern LinkAnimationHeader D_0400D4B0;
extern LinkAnimationHeader D_0400D4B8;
extern LinkAnimationHeader D_0400D4C0;
extern LinkAnimationHeader D_0400D4D8;
extern LinkAnimationHeader D_0400D4F0;
extern LinkAnimationHeader D_0400D4F8;
extern LinkAnimationHeader D_0400D500;
extern LinkAnimationHeader D_0400D5A8;
extern LinkAnimationHeader D_0400D5B0;
extern LinkAnimationHeader D_0400D638;
extern LinkAnimationHeader D_0400D660;
extern LinkAnimationHeader D_0400D698;
extern LinkAnimationHeader D_0400D8B0;
extern LinkAnimationHeader D_0400D9C8;
extern LinkAnimationHeader D_0400D9D0;
extern LinkAnimationHeader D_0400D9D8;
extern LinkAnimationHeader D_0400DA60;
extern LinkAnimationHeader D_0400DA68;
extern LinkAnimationHeader D_0400DA70;
extern LinkAnimationHeader D_0400DA88;
extern LinkAnimationHeader D_0400DAA0;
extern LinkAnimationHeader D_0400DAA8;
extern LinkAnimationHeader D_0400DAC0;
extern LinkAnimationHeader D_0400DAC8;
extern LinkAnimationHeader D_0400DAD0;
extern LinkAnimationHeader D_0400DAD8;
extern LinkAnimationHeader D_0400DAE0;
extern LinkAnimationHeader D_0400DAF0;
extern LinkAnimationHeader D_0400DB10;
extern LinkAnimationHeader D_0400DB18;
extern LinkAnimationHeader D_0400DB30;
extern LinkAnimationHeader D_0400DB68;
extern LinkAnimationHeader D_0400DBB0;
extern LinkAnimationHeader D_0400DBE8;
extern LinkAnimationHeader D_0400DC20;
extern LinkAnimationHeader D_0400DC28;
extern LinkAnimationHeader D_0400DC30;
extern LinkAnimationHeader D_0400DC48;
extern LinkAnimationHeader D_0400DC60;
extern LinkAnimationHeader D_0400DC78;
extern LinkAnimationHeader D_0400DC80;
extern LinkAnimationHeader D_0400DC88;
extern LinkAnimationHeader D_0400DCA8;
extern LinkAnimationHeader D_0400DCD0;
extern LinkAnimationHeader D_0400DCF8;
extern LinkAnimationHeader D_0400DD10;
extern LinkAnimationHeader D_0400DD30;
extern LinkAnimationHeader D_0400DD38;
extern LinkAnimationHeader D_0400DD40;
extern LinkAnimationHeader D_0400DD70;
extern LinkAnimationHeader D_0400DD78;
extern LinkAnimationHeader D_0400DD80;
extern LinkAnimationHeader D_0400DDF0;
extern LinkAnimationHeader D_0400DDF8;
extern LinkAnimationHeader D_0400DE00;
extern LinkAnimationHeader D_0400DE08;
extern LinkAnimationHeader D_0400DE10;
extern LinkAnimationHeader D_0400DE28;
extern LinkAnimationHeader D_0400DE30;
extern LinkAnimationHeader D_0400DE50;
extern LinkAnimationHeader D_0400DE60;
extern LinkAnimationHeader D_0400DEA0;
extern LinkAnimationHeader D_0400DEA8;
extern LinkAnimationHeader D_0400DF78;
extern LinkAnimationHeader D_0400DF90;
extern LinkAnimationHeader D_0400DF98;
extern LinkAnimationHeader D_0400DFA0;
extern LinkAnimationHeader D_0400DFA8;
extern LinkAnimationHeader D_0400DFB0;
extern LinkAnimationHeader D_0400DFB8;
extern LinkAnimationHeader D_0400DFC0;
extern LinkAnimationHeader D_0400DFC8;
extern LinkAnimationHeader D_0400DFD0;
extern LinkAnimationHeader D_0400DFD8;
extern LinkAnimationHeader D_0400DFE0;
extern LinkAnimationHeader D_0400DFE8;
extern LinkAnimationHeader D_0400DFF0;
extern LinkAnimationHeader D_0400DFF8;
extern LinkAnimationHeader D_0400E000;
extern LinkAnimationHeader D_0400E008;
extern LinkAnimationHeader D_0400E050;
extern LinkAnimationHeader D_0400E060;
extern LinkAnimationHeader D_0400E070;
extern LinkAnimationHeader D_0400E080;
extern LinkAnimationHeader D_0400E088;
extern LinkAnimationHeader D_0400E098;
extern LinkAnimationHeader D_0400E1F0;
extern LinkAnimationHeader D_0400E200;
extern LinkAnimationHeader D_0400E208;
extern LinkAnimationHeader D_0400E260;
extern LinkAnimationHeader D_0400E270;
extern LinkAnimationHeader D_0400E278;
extern LinkAnimationHeader D_0400E290;
extern LinkAnimationHeader D_0400E298;
extern LinkAnimationHeader D_0400E2A0;
extern LinkAnimationHeader D_0400E2B8;
extern LinkAnimationHeader D_0400E2C0;
extern LinkAnimationHeader D_0400E2C8;
extern LinkAnimationHeader D_0400E2D8;
extern LinkAnimationHeader D_0400E2E8;
extern LinkAnimationHeader D_0400E2F0;
extern LinkAnimationHeader D_0400E2F8;
extern LinkAnimationHeader D_0400E398;
extern LinkAnimationHeader D_0400E3A0;
extern LinkAnimationHeader D_0400E3A8;
extern LinkAnimationHeader D_0400E3B0;
extern LinkAnimationHeader D_0400E3B8;
extern LinkAnimationHeader D_0400E3C0;
extern LinkAnimationHeader D_0400E3D8;
extern LinkAnimationHeader D_0400E408;
extern LinkAnimationHeader D_0400E410;
extern LinkAnimationHeader D_0400E418;

// extern void (*D_8085D2CC[0x10])(PlayState*, Player*, s32);
extern void (*D_8085D2CC[0x10])();
extern PlayerAgeProperties D_8085BA38[PLAYER_FORM_MAX];
extern LinkAnimationHeader* D_8085D160[PLAYER_FORM_MAX];
extern UNK_TYPE D_8085D30C;
extern UNK_TYPE D_8085D330;
extern Vec3f D_8085D340;

s32 func_8085B1F0(PlayState* play, Player* player);
s32 func_8085B28C(PlayState* play, Player* player, s32 mode);
void func_8085B384(Player* player, PlayState* play);
s32 func_8085B3E0(PlayState* play, s32 damage);
void func_8085B460(PlayState* play, Actor* actor);
void func_8085B74C(PlayState* play);
void func_8085B820(PlayState* play, s16 arg1);
PlayerActionParam func_8085B854(PlayState* play, Player* player, ItemID itemId);
s32 func_8085B930(PlayState* play, LinkAnimationHeader* talkAnim, s32 animMode);
void func_8085B08C(Player* arg0, PlayState* play);
void func_80855818(Player* arg0, PlayState* play);

void func_80844EF8(Player* player, PlayState* play, Input* input);
s32 func_8085B134(PlayState* play);
void func_8085B170(PlayState* play, Player* player);

void func_8082F8BC(PlayState* play, Player* this, PlayerActionParam actionParam);

void func_80831990(PlayState* play, Player* this, ItemID item);

s32 func_80848BF4(Player* this, PlayState* play);

void func_80836988(Player* this, PlayState* play);
void func_8084C16C(Player* this, PlayState* play);

#define GET_PLAYER_ANIM(group, type) D_8085BE84[group * PLAYER_ANIMTYPE_MAX + type]

extern LinkAnimationHeader* D_8085BE84[PLAYER_ANIMGROUP_MAX * PLAYER_ANIMTYPE_MAX];

extern f32 D_8085D3FC[2];
extern Input* D_80862B44;

extern Vec3f D_80862AF0; /* type too large by 8 */
extern f32 D_80862AF4;
extern f32 D_80862AF8;
extern f32 D_80862AFC;
extern s16 D_80862B00;
extern s16 D_80862B02;
extern s32 D_80862B04;
extern s32 D_80862B08;
extern s32 D_80862B0C;
extern u32 D_80862B10;
extern s16 D_80862B14;
extern s16 D_80862B16;
extern f32 D_80862B18;
extern s32 D_80862B1C;
extern s32 D_80862B20;
extern s32 D_80862B24;
extern s16 D_80862B28;
extern s32 D_80862B2C;
extern Vec3f D_80862B30;
extern f32 D_80862B3C;
extern u32 D_80862B40;
extern Input* D_80862B44;
extern s32 D_80862B48;
extern s32 D_80862B4C;
extern EnvLightSettings D_80862B50;
extern s32 D_80862B6C;
extern f32 D_8085C3E4;

extern f32 D_8085D404[3];
extern f32 D_8085C3E8;
extern u16 D_8085C3EC[4];

extern u8 D_8085D174[];

extern f32 D_8085D35C;
extern f32 D_8085D360;
extern Vec3f D_8085D364;
extern Vec3f D_8085D370;
extern Color_RGBA8 D_8085D37C;
extern Color_RGBA8 D_8085D380;
extern UNK_TYPE D_8085D384;
extern f32 D_8085D3E0[5];
extern Color_RGBA8 D_8085D3F4;
extern Color_RGBA8 D_8085D3F8;
extern f32 D_8085D3FC[2];
extern f32 D_8085D404[3];
extern f32 D_8085D410[3];

void func_808508C8(Player* this, PlayState* play);
void func_80854430(Player* this, PlayState* play);
void func_8084D820(Player* this, PlayState* play);
void func_808561B0(Player* this, PlayState* play);
void func_80853D68(Player* this, PlayState* play);
void func_80857BE8(Player* this, PlayState* play);

void func_8084BC64(Player* this, PlayState* play);
void func_808548B8(Player* this, PlayState* play);
void func_8084FE7C(Player* this, PlayState* play);
void func_808505D0(Player* this, PlayState* play);

s32 func_8082DA90(PlayState* play) {
    return play->transitionTrigger != TRANS_TRIGGER_OFF || play->transitionMode != TRANS_MODE_OFF;
}

void func_8082DABC(Player* this) {
    this->linearVelocity = 0.0f;
    this->actor.speedXZ = 0.0f;
}

void func_8082DAD4(Player* this) {
    func_8082DABC(this);
    this->unk_AA5 = 0;
}

s32 func_8082DAFC(PlayState* play) {
    Player* player = GET_PLAYER(play);

    return CHECK_FLAG_ALL(player->actor.flags, 0x100);
}

void func_8082DB18(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    LinkAnimation_PlayOnce(play, &this->skelAnime, anim);
}

void func_8082DB3C(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    LinkAnimation_PlayLoop(play, &this->skelAnime, anim);
}

void func_8082DB60(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    LinkAnimation_PlayLoopSetSpeed(play, &this->skelAnime, anim, 2.0f / 3.0f);
}

void func_8082DB90(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    LinkAnimation_PlayOnceSetSpeed(play, &this->skelAnime, anim, 2.0f / 3.0f);
}

void func_8082DBC0(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    LinkAnimation_Change(play, &this->skelAnime, anim, -2.0f / 3.0f, Animation_GetLastFrame(anim), 0.0f, 2, 0.0f);
}

void func_8082DC28(Player* this) {
    this->skelAnime.jointTable[1].y = 0;
}

void func_8082DC38(Player* this) {
    this->stateFlags2 &= ~0x20000;
    this->meleeWeaponState = 0;
    this->meleeWeaponInfo[2].active = false;
    this->meleeWeaponInfo[1].active = false;
    this->meleeWeaponInfo[0].active = false;
}

void func_8082DC64(PlayState* play, Player* this) {
    if ((this->unk_3BC != -1) && (play->cameraPtrs[this->unk_3BC] != 0)) {
        this->unk_3BC = -1;
    }
    this->stateFlags2 &= ~0xC00;
}

void func_8082DCA0(PlayState* play, Player* this) {
    Actor* heldActor;

    heldActor = this->heldActor;
    if (heldActor != NULL) {
        if (!Player_IsHoldingHookshot(this)) {
            this->actor.child = NULL;
            this->heldActor = NULL;
            this->interactRangeActor = NULL;
            heldActor->parent = NULL;
            this->stateFlags1 &= ~0x800;
        }
    }

    if (Player_GetExplosiveHeld(this) >= 0) {
        func_8082F8BC(play, this, 0);
        this->heldItemId = 0xFE;
    }
}

void func_8082DD2C(PlayState* play, Player* arg1) {
    if ((arg1->stateFlags1 & 0x800) && (arg1->heldActor == NULL)) {
        if (arg1->interactRangeActor != NULL) {
            if (arg1->getItemId == 0) {
                arg1->stateFlags1 &= ~0x800;
                arg1->interactRangeActor = NULL;
            }
        } else {
            arg1->stateFlags1 &= ~0x800;
        }
    }

    func_8082DC38(arg1);
    arg1->unk_AA5 = 0;
    func_8082DC64(play, arg1);
    func_800E0238(Play_GetCamera(play, CAM_ID_MAIN));
    arg1->stateFlags1 &= ~0x306004;
    arg1->stateFlags2 &= ~0x90;
    arg1->unk_ADD = 0;
    arg1->unk_ADC = 0;
    arg1->actor.shape.rot.x = 0;
    arg1->actor.shape.rot.z = 0;
    arg1->unk_ABC = 0.0f;
    arg1->unk_AC0 = 0.0f;
}

s32 func_8082DE14(PlayState* play, Player* this) {
    if (this->itemActionParam > PLAYER_AP_LAST_USED) {
        func_80831990(play, this, ITEM_NONE);
        return true;
    }
    return false;
}

void func_8082DE50(PlayState* play, Player* this) {
    func_8082DD2C(play, this);
    func_8082DCA0(play, this);
}

#ifdef NON_MATCHING
s32 func_8082DE88(Player* arg0, s32 arg1, s32 arg2) {
    s16 temp_v0;

    temp_v0 = arg0->unk_D64 - D_80862B00;
    arg0->unk_AE8 += arg1 + (s32)(ABS_ALT(temp_v0) * fabsf(D_80862AFC) * 0.0000025415802f);
    if (D_80862B44->press.button & 0xC000) {
        arg0->unk_AE8 += 5;
    }
    return arg0->unk_AE8 >= arg2;
}
#else
s32 func_8082DE88(Player* this, s32 arg1, s32 arg2);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8082DE88.s")
#endif

void func_8082DF2C(PlayState* play) {
    if (play->actorCtx.freezeFlashTimer == 0) {
        play->actorCtx.freezeFlashTimer = 1;
    }
}

void Player_RequestRumble(PlayState* play, Player* this, u8 sourceIntensity, u8 decayTimer, u8 decayStep, s32 distSq) {
    if (this == GET_PLAYER(play)) {
        Rumble_Request(distSq, sourceIntensity, decayTimer, decayStep);
    }
}

void func_8082DF8C(Player* this, u16 arg1) {
    u16 var_v0;

    if (this->currentMask == PLAYER_MASK_GIANT) {
        func_8019F830(&this->actor.projectedPos, arg1);
        return;
    }

    if (this->actor.id == ACTOR_PLAYER) {
        if (this->currentMask == PLAYER_MASK_SCENTS) {
            var_v0 = 0xE0;
        } else {
            var_v0 = this->ageProperties->unk_92;
        }

        func_800B8E58(this, var_v0 + arg1);
    }
}

void func_8082E00C(Player* this) {
    s32 i;
    u16* var_s0;

    var_s0 = D_8085C3EC;
    for (i = 0; i < ARRAY_COUNT(D_8085C3EC); i++) {
        func_801A75E8(*var_s0 + this->ageProperties->unk_92);
        var_s0 += 1;
    }
}

u16 func_8082E078(Player* this, u16 arg1) {
    return arg1 + this->unk_B72;
}

void func_8082E094(Player* this, u16 arg1) {
    func_800B8E58(this, func_8082E078(this, arg1));
}

u16 func_8082E0CC(Player* arg0, u16 arg1) {
    return arg1 + arg0->unk_B72 + arg0->ageProperties->unk_94;
}

void func_8082E0F4(Player* arg0, u16 arg1) {
    func_800B8E58(arg0, func_8082E0CC(arg0, arg1));
}

void func_8082E12C(Player* this, f32 arg1) {
    s32 var_v1;

    if (this->currentMask == PLAYER_MASK_GIANT) {
        var_v1 = 0x8CE;
    } else {
        var_v1 = func_8082E0CC(this, 0x800);
    }
    func_8019F638(&this->actor.projectedPos, var_v1, arg1);
}

void func_8082E188(Player* this) {
    func_800B8E58(this, func_8082E0CC(this, 0x810));
}

void func_8082E1BC(Player* this) {
    func_800B8E58(this, func_8082E0CC(this, 0x820));
}

void func_8082E1F0(Player* this, u16 sfxId) {
    func_800B8E58(this, sfxId);
    this->stateFlags2 |= 8;
}

typedef struct struct_8082E224_arg1 {
    /* 0x0 */ u16 unk_0;
    /* 0x2 */ s16 unk_2; // negative marks the end
} struct_8082E224_arg1;  // size = 0x4

void func_8082E224(Player* this, struct_8082E224_arg1* arg1);
#if NON_MATCHING
// regalloc around } while ((temp_v0 >= 0) != 0);
void func_8082E224(Player* arg0, struct_8082E224_arg1* arg1) {
    s32 temp_v0;

    do {
        s32 var_s0 = ABS_ALT(arg1->unk_2);
        s32 temp_v0_3;

        temp_v0_3 = var_s0 & 0x7800;
        if (LinkAnimation_OnFrame(&arg0->skelAnime, fabsf((f32)(var_s0 & 0x7FF)))) {
            if (temp_v0_3 == 0x800) {
                func_800B8E58(arg0, arg1->unk_0);
            } else if (temp_v0_3 == 0x1000) {
                func_8082E094(arg0, arg1->unk_0);
            } else if (temp_v0_3 == 0x1800) {
                func_8082E0F4(arg0, arg1->unk_0);
            } else if (temp_v0_3 == 0x2000) {
                func_8082DF8C(arg0, arg1->unk_0);
            } else if (temp_v0_3 == 0x2800) {
                func_8082E1BC(arg0);
            } else if (temp_v0_3 == 0x3000) {
                func_8082E12C(arg0, 6.0f);
            } else if (temp_v0_3 == 0x3800) {
                func_8082E188(arg0);
            } else if (temp_v0_3 == 0x4000) {
                func_8082E12C(arg0, 0.0f);
            } else if (temp_v0_3 == 0x4800) {
                func_8019F638(&arg0->actor.projectedPos, arg0->ageProperties->unk_94 + 0x80A, 0.0f);
            } else if (temp_v0_3 == 0x5000) {
                func_800B8E58(arg0, arg1->unk_0 + arg0->ageProperties->unk_94);
            }
        }
        temp_v0 = arg1->unk_2;
        arg1 += 1;
    } while ((temp_v0 >= 0) != 0);
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8082E224.s")
#endif

void func_8082E438(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    LinkAnimation_Change(play, &this->skelAnime, anim, 3.0f / 3.0f, 0.0f, Animation_GetLastFrame(anim), 2, -6.0f);
}

void func_8082E4A4(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    LinkAnimation_Change(play, &this->skelAnime, anim, 2.0f / 3.0f, 0.0f, Animation_GetLastFrame(anim), 2, -6.0f);
}

void func_8082E514(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    LinkAnimation_Change(play, &this->skelAnime, anim, 3.0f / 3.0f, 0.0f, 0.0f, 0, -6.0f);
}

void func_8082E55C(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    LinkAnimation_Change(play, &this->skelAnime, anim, 2.0f / 3.0f, 0.0f, 0.0f, 0, -6.0f);
}

void func_8082E5A8(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    LinkAnimation_Change(play, &this->skelAnime, anim, 3.0f / 3.0f, 0.0f, 0.0f, 2, 0.0f);
}

void func_8082E5EC(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    LinkAnimation_Change(play, &this->skelAnime, anim, 2.0f / 3.0f, 0.0f, 0.0f, 2, 0.0f);
}

void func_8082E634(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    LinkAnimation_Change(play, &this->skelAnime, anim, 3.0f / 3.0f, 0.0f, 0.0f, 0, -16.0f);
}

s32 func_8082E67C(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    if (LinkAnimation_Update(play, &this->skelAnime)) {
        func_8082DB3C(play, this, anim);
        return true;
    }
    return false;
}

void func_8082E6D0(Player* this) {
    this->skelAnime.prevTransl = this->skelAnime.baseTransl;
    this->skelAnime.prevRot = this->actor.shape.rot.y;
}

void func_8082E6F8(Player* this) {
    func_8082E6D0(this);
    this->skelAnime.prevTransl.x *= this->ageProperties->unk_08;
    this->skelAnime.prevTransl.y *= this->ageProperties->unk_08;
    this->skelAnime.prevTransl.z *= this->ageProperties->unk_08;
}

void func_8082E784(Player* this) {
    this->skelAnime.jointTable[1].y = 0;
}

void func_8082E794(Player* this) {
    if (this->skelAnime.moveFlags != 0) {
        func_8082DC28(this);
        this->skelAnime.jointTable->x = this->skelAnime.baseTransl.x;
        this->skelAnime.jointTable->z = this->skelAnime.baseTransl.z;

        if (this->skelAnime.moveFlags & 8) {
            if (this->skelAnime.moveFlags & 2) {
                this->skelAnime.jointTable->y = this->skelAnime.prevTransl.y;
            }
        } else {
            this->skelAnime.jointTable->y = this->skelAnime.baseTransl.y;
        }
        func_8082E6D0(this);
        this->skelAnime.moveFlags = 0;
    }
}

void func_8082E820(Player* this, s32 arg1) {
    Vec3f pos;

    this->skelAnime.moveFlags = arg1;
    this->skelAnime.prevTransl = this->skelAnime.baseTransl;
    SkelAnime_UpdateTranslation(&this->skelAnime, &pos, this->actor.shape.rot.y);

    if (arg1 & 1) {
        pos.x *= this->ageProperties->unk_08;
        pos.z *= this->ageProperties->unk_08;
        this->actor.world.pos.x += pos.x * this->actor.scale.x;
        this->actor.world.pos.z += pos.z * this->actor.scale.z;
    }

    if (arg1 & 2) {
        if (!(arg1 & 4)) {
            pos.y *= this->ageProperties->unk_08;
        }
        this->actor.world.pos.y += pos.y * this->actor.scale.y;
    }

    func_8082DC28(this);
}

void func_8082E920(PlayState* play, Player* this, s32 arg2) {
    if (arg2 & 0x200) {
        func_8082E6F8(this);
    } else if ((arg2 & 0x100) || (this->skelAnime.moveFlags != 0)) {
        func_8082E6D0(this);
    } else {
        this->skelAnime.prevTransl = this->skelAnime.jointTable[0];
        this->skelAnime.prevRot = this->actor.shape.rot.y;
    }

    this->skelAnime.moveFlags = arg2;
    func_8082DABC(this);
    AnimationContext_DisableQueue(play);
}

void func_8082E9C8(PlayState* play, Player* this, LinkAnimationHeader* anim, s32 arg3, f32 playSpeed) {
    LinkAnimation_PlayOnceSetSpeed(play, &this->skelAnime, anim, playSpeed);
    func_8082E920(play, this, arg3);
}

void func_8082EA10(PlayState* play, Player* this, LinkAnimationHeader* anim, s32 arg3) {
    func_8082E9C8(play, this, anim, arg3, 3.0f / 3.0f);
}

void func_8082EA38(PlayState* play, Player* this, LinkAnimationHeader* anim, s32 arg3) {
    func_8082E9C8(play, this, anim, arg3, 2.0f / 3.0f);
}

void func_8082EA60(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    func_8082EA38(play, this, anim, 0x20C);
}

void func_8082EA80(PlayState* play, Player* this, LinkAnimationHeader* anim, s32 arg3, f32 playSpeed) {
    LinkAnimation_PlayLoopSetSpeed(play, &this->skelAnime, anim, playSpeed);
    func_8082E920(play, this, arg3);
}

void func_8082EAC8(PlayState* play, Player* this, LinkAnimationHeader* anim, s32 arg3) {
    func_8082EA80(play, this, anim, arg3, 3.0f / 3.0f);
}

void func_8082EAF0(PlayState* play, Player* this, LinkAnimationHeader* anim, s32 arg3) {
    func_8082EA80(play, this, anim, arg3, 2.0f / 3.0f);
}

void func_8082EB18(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    func_8082EAF0(play, this, anim, 0x1C);
}

void func_8082EB38(PlayState* play, Player* this) {
    s8 var_v0;
    s8 var_v1;

    this->unk_D60 = D_80862AFC;
    this->unk_D64 = D_80862B00;
    func_800FF3A0(&D_80862AFC, &D_80862B00, D_80862B44);

    if (D_80862AFC < 8.0f) {
        D_80862AFC = 0.0f;
    }

    D_80862B02 = Camera_GetInputDirYaw(play->cameraPtrs[play->activeCamId]) + D_80862B00;
    this->unk_ADE = (this->unk_ADE + 1) % ARRAY_COUNT(this->unk_ADF);
    if (D_80862AFC < 55.0f) {
        var_v0 = -1;
        var_v1 = -1;
    } else {
        var_v1 = ((u16)(D_80862B00 + 0x2000)) >> 9;
        var_v0 = ((u16)(BINANG_SUB(D_80862B02, this->actor.shape.rot.y) + 0x2000)) >> 0xE;
    }
    this->unk_ADF[this->unk_ADE] = var_v1;
    this->unk_AE3[this->unk_ADE] = var_v0;
}

void func_8082EC9C(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    LinkAnimation_PlayOnceSetSpeed(play, &this->skelAnime, anim, D_8085C3E4);
}

s32 func_8082ECCC(Player* this) {
    return this->stateFlags1 & PLAYER_STATE1_1000000;
}

typedef struct {
    /* 0x0 */ s8 unk_0;
    /* 0x1 */ s8 unk_1;
    /* 0x2 */ s8 unk_2;
    /* 0x3 */ s8 unk_3;
    /* 0x4 */ s8 unk_4;
    /* 0x5 */ s8 unk_5;
} struct_8085C3F4; // size = 0x6

extern struct_8085C3F4 D_8085C3F4[GI_MAX - 1];
#if 0
struct_8085C3F4 D_8085C3F4[0xB9] = {
    { 0x84, 0, 0xB0, -0x3C, 1, 0x3F },
    { 0x85, 1, 0xAF, 2, 1, 0x3F },
    { 0x86, 2, 0xAE, 3, 1, 0x3F },
    { 0x87, 2, 0xAE, 4, 1, 0x3F },
    { 0x88, 0x14, 0xAC, 5, 1, 0x3F },
    { 0x89, 0x14, 0xAB, 6, 1, 0x3F },
    { 0x8A, 0x13, 0xBD, 7, 1, 0x3F },
    { 0x5A, 0xA0, 0x21, 8, 0, 0xA8 },
    { 0x5B, 0xA0, 0x22, 9, 0, 0xA8 },
    { 0x83, 0x80, 9, 0xA, 0, 0x90 },
    { 0x83, 0xA0, 9, 0xB, 0, 0x90 },
    { 0x7B, 0xA0, 0x14, 0xC, 0, 0x96 },
    { 0x6F, 0xA0, 0x13, 0xD, 0, 0x96 },
    { 0x79, 0x6F, -0x1D, 0xE, 0, 0xA4 },
    { 0x7A, 0x6E, -0x1E, 0xF, 0, 0xA4 },
    { 0x83, 0x80, 9, 0x10, 0, 0x90 },
    { -1, 0, 0, 0x11, 0, 0 },
    { 0x83, 0x80, 9, 0x12, 0, 0x90 },
    { 0x83, 0x80, 9, 0x13, 0, 0x90 },
    { 6, 0x59, -0x1F, 0x14, 0, 0xA5 },
    { 0x8F, 0x59, -0x1F, 0x15, 0, 0xA5 },
    { 0x90, 0x59, -0x1F, 0x16, 0, 0xA5 },
    { 0x91, 0x59, -0x1F, 0x17, 0, 0xA5 },
    { 0x92, 0x59, -0x1F, 0x18, 0, 0xA5 },
    { 8, 0xD, -0x1B, 0x19, 0, 0x9F },
    { 0x98, 0xC0, -0x27, 0x1A, 0, 0xB0 },
    { 0x56, 0xA0, 0x18, 0x1B, 0, 0x98 },
    { 0x57, 0xA0, 0x19, 0x1C, 0, 0x98 },
    { 0x58, 0xA0, 0x1A, 0x1D, 0, 0x98 },
    { 0x93, 0x48, -0x24, 0x1E, 0, 0xAF },
    { 0x94, 0x49, -0x25, 0x1F, 0, 0xAF },
    { 0x95, 0x4A, -0x26, 0x20, 0, 0xAF },
    { 0x95, 0x4A, -0x26, 0x21, 0, 0xAF },
    { 1, 0xA0, 0x2F, 0x22, 0, 0xBF },
    { 0x54, 0xA0, 0x16, 0x23, 0, 0x97 },
    { 0x55, 0xA0, 0x17, 0x24, 0, 0x97 },
    { 2, 0xA0, 0x48, 0x25, 1, 0x21 },
    { 3, 0xA0, 0x49, 0x26, 1, 0x21 },
    { 4, 0xA0, 0x4A, 0x27, 1, 0x21 },
    { 9, 0xC, -0x12, 0x28, 0, 0x94 },
    { 0x8D, 0xC, -0x12, 0x29, 0, 0x94 },
    { 0x8E, 0xC, -0x12, 0x2A, 0, 0x94 },
    { 0x9D, 0xA0, -0x12, 0x2B, 0, 0x94 },
    { 0x9D, 0xA0, -0x12, 0x2C, 0, 0x94 },
    { 0x9E, 0xA0, -0x12, 0x2D, 0, 0x94 },
    { 0x97, 0xC0, -0x27, 0x2E, 0, 0xB0 },
    { 0x9B, 0xA0, -0x1B, 0x2F, 0, 0x9F },
    { 0x9B, 0xA0, -0x1B, 0x30, 0, 0x9F },
    { 0x9C, 0xA0, -0x1B, 0x31, 0, 0x9F },
    { 0x51, 0xA0, -0x28, 0x32, 0, 0xB3 },
    { 0x52, 0xA0, 0x34, 0x33, 0, -0x3D },
    { 0xC, 0x80, 0x5E, 0x34, 1, -0x36 },
    { 0xA, 0x80, -0x35, 0x35, 0, -0x3A },
    { 0x99, 0xC0, -0x27, 0x36, 0, 0xB0 },
    { 0x4D, 0xA0, 0x56, 0x37, 1, 0x48 },
    { 0x4E, 0xA0, 0x67, 0x38, 1, -7 },
    { 0x4F, 0xA0, 0x68, 0x39, 1, -6 },
    { 0x9A, 0xC0, -0x27, 0x3A, 0, 0xB0 },
    { 0x10, 0xA0, 0x69, 0x3B, 1, -5 },
    { 0x78, 0xA0, -2, 0x3C, 0, 0x86 },
    { 0x74, 0xA0, 0xA, 0x3D, 0, 0x92 },
    { 0x76, 0xA0, 0x1C, 0x3E, 0, 0xA0 },
    { 0x75, 0xA0, 0xB, 0x3F, 0, 0x91 },
    { -1, 0, 0, 0x40, 0, 0 },
    { 0xF, 0xA0, 0x29, 0x41, 0, 0xB4 },
    { 0xE, 0xA0, 0x30, 0x42, 0, 0xC0 },
    { 0xD, 0xA0, 0x75, 0x43, 2, 0x28 },
    { 0xD, 0, 0, 0x44, 0, 0 },
    { 0x83, 0x80, 9, 0x45, 0, 0x90 },
    { -1, 0, 0, 0x46, 0, 0 },
    { -1, 0, 0, 0x47, 0, 0 },
    { -1, 0, 0, 0x48, 0, 0 },
    { -1, 0, 0, 0x49, 0, 0 },
    { -1, 0, 0, 0x4A, 0, 0 },
    { -1, 0, 0, 0x4B, 0, 0 },
    { 0, 0xA0, 0x2B, 0x4C, 0, 0xB5 },
    { -1, 0, 0, 0x4D, 0, 0 },
    { -1, 0, 0, 0x4E, 0, 0 },
    { -1, 0, 0, 0x4F, 0, 0 },
    { 0x6D, 0x80, 0xC, 0x50, 2, 0x53 },
    { -1, 0xA0, 0x20, 0x51, 0, 0xA0 },
    { 0x6E, 0x80, 0xB5, 0x52, 1, 0x25 },
    { -1, 0, 0, 0x53, 0, 0 },
    { -1, 0, 0, 0x54, 0, 0 },
    { 0x5D, 0x80, 0x5D, 0x55, 0, 0 },
    { 0x5E, 0x80, 0x64, 0x56, 0, 0 },
    { 0x5F, 0x80, 0x65, 0x57, 0, 0 },
    { 0x60, 0x80, 0x66, 0x58, 0, 0 },
    { 0x11, 0x80, 0x58, 0x59, 1, 0x96 },
    { 0x12, 0x80, 1, 0x5A, 0, 0x9E },
    { 0x13, 0x80, 0x32, 0x5B, 0, -0x3F },
    { 0x14, 0x80, 0x31, 0x5C, 0, -0x3F },
    { 0x15, 0x80, 0x33, 0x5D, 0, -0x3F },
    { 0x16, 0x80, 0x3C, 0x5E, 2, 0x72 },
    { 0x16, 0x80, 1, 0x5F, 0, 0x9E },
    { 0x18, 0x80, 0x2C, 0x60, 0, 0xB6 },
    { 0x19, 0x80, 0x2C, 0x61, 0, 0xB6 },
    { 0x1A, 0x80, 0x36, 0x62, 0, -0x39 },
    { 0x1B, 0x80, 0x4C, 0x63, 1, 0x37 },
    { 0x1C, 0, 0, 0x64, 0, 0 },
    { 0x12, 0x80, 1, 0x65, 0, 0x9E },
    { 0x1E, 0x80, 0x53, 0x66, 1, 0x39 },
    { 0x1F, 0, 0, 0x67, 0, 0 },
    { 0x20, 0, 0, 0x68, 0, 0 },
    { 0x21, 0x80, 0x59, 0x69, 1, 0xAE },
    { 0x22, 0x80, 0x60, 0x6A, 1, -0x17 },
    { 0x23, 0x80, 0x70, 0x6B, 2, 0x1D },
    { -1, 0, 0, 0x6C, 0, 0 },
    { 0x12, 0x80, 1, 0x6D, 0, 0x9E },
    { 0x24, 0x80, 0x60, 0x6E, 1, -0x17 },
    { 0x25, 0x80, 0x74, 0x6F, 2, 0x27 },
    { 0x12, 0x80, 1, 0x70, 0, 0x9E },
    { -1, 0, 0, 0x71, 0, 0 },
    { -1, 0, 0, 0x72, 0, 0 },
    { -1, 0, 0, 0x73, 0, 0 },
    { -1, 0, 0, 0x74, 0, 0 },
    { -1, 0, 0, 0x75, 0, 0 },
    { -1, 0, 0, 0x76, 0, 0 },
    { -1, 0, 0, 0x77, 0, 0 },
    { 0x32, 0xA0, 0x5C, 0x78, 1, 0xBD },
    { 0x33, 0xA0, 0x45, 0x79, 1, 0x19 },
    { 0x34, 0xA0, 0x46, 0x7A, 1, 0x1A },
    { 0x35, 0xA0, 0x76, 0x7B, 2, 0x42 },
    { 0x44, 0xA0, 0x3E, 0x7C, 1, 2 },
    { 0x49, 0xA0, 0x73, 0x7D, 2, 0x26 },
    { 0x38, 0xA0, 0x11, 0x7E, 2, 0x65 },
    { 0x39, 0xA0, 0x3F, 0x7F, 1, 3 },
    { 0x3A, 0xA0, 0x2D, 0x80, 1, 0 },
    { 0x3B, 0xA0, 0x6A, 0x81, 2, 9 },
    { 0x3C, 0xA0, 0x71, 0x82, 2, 0x1F },
    { 0x3D, 0xA0, 0xF, 0x83, 2, 0x59 },
    { 0x3E, 0xA0, 0x72, 0x84, 2, 0x25 },
    { 0x3F, 0xA0, 4, 0x85, 2, 0x82 },
    { 0x40, 0xA0, 0x6B, 0x86, 2, 0xA },
    { 0x41, 0xA0, 0x6C, 0x87, 2, 0xB },
    { 0x42, 0xA0, 0x23, 0x88, 2, 0x66 },
    { 0x43, 0xA0, 3, 0x89, 2, 0x7D },
    { 0x36, 0xA0, 0x40, 0x8A, 1, 4 },
    { 0x45, 0xA0, 0xD, 0x8B, 2, 0x54 },
    { 0x46, 0xA0, 0x10, 0x8C, 2, 0x5A },
    { 0x47, 0xA0, 0x3B, 0x8D, 2, 0x6D },
    { 0x48, 0xA0, 0x3D, 0x8E, 2, 0x7E },
    { 0x37, 0xA0, 0xE, 0x8F, 2, 0x58 },
    { -1, 0, 0, 0x90, 0, 0 },
    { 0x9F, 0x80, 0x74, 0x91, 2, 0x27 },
    { 0xA0, 0x80, 0x2C, 0x92, 0, 0xB6 },
    { 0xA1, 0x80, 0x5F, 0x93, 1, -0x18 },
    { 0xA2, 0x80, 0x62, 0x94, 1, -0x11 },
    { 0xA3, 0x80, 0x63, 0x95, 1, -0x10 },
    { 0x28, 0x80, 0x5A, 0x96, 1, 0xB1 },
    { 0x29, 0x80, 0x5B, 0x97, 1, 0xB2 },
    { 0x2A, 0x80, 0x41, 0x98, 1, 0xB2 },
    { 0x2B, 0x80, 0x42, 0x99, 1, 0xB2 },
    { 0x2C, 0x80, 0x44, 0x9A, 1, 0xB2 },
    { 0x10, 0xA0, 0x69, 0x9B, 1, -5 },
    { 0x4D, 0xA0, 0x56, 0x9C, 1, 0x48 },
    { 0x4E, 0xA0, 0x67, 0x9D, 1, -7 },
    { 0x4F, 0xA0, 0x68, 0x9E, 1, -6 },
    { 0x51, 0xA0, -0x28, 0x9F, 0, 0xB3 },
    { 0x2D, 0x80, 0x6D, 0xA0, 2, 0xF },
    { 0x2E, 0x80, 0x37, 0xA1, 2, 0x45 },
    { -1, 0, 0, 0xA2, 0, 0 },
    { -1, 0, 0, 0xA3, 0, 0 },
    { -1, 0x80, 0x2D, 0xA4, 1, 0 },
    { -1, 0, 0, 0xA5, 0, 0 },
    { -1, 0, 0, 0xA6, 0, 0 },
    { -1, 0, 0, 0xA7, 0, 0 },
    { -1, 0, 0, 0xA8, 0, 0 },
    { 0x12, 0x80, 1, 0xA9, 0, 0x9E },
    { 0x2F, 0x80, 0x6E, 0xAA, 2, 0x10 },
    { 0x30, 0x80, 0x6F, 0xAB, 2, 0x15 },
    { -1, 0, 0, 0xAC, 0, 0 },
    { -1, 0, 0, 0xAD, 0, 0 },
    { -1, 0, 0, 0xAE, 0, 0 },
    { -1, 0, 0, 0xAF, 0, 0 },
    { -1, 0, 0, 0xB0, 0, 0 },
    { -1, 0, 0, 0xB1, 0, 0 },
    { -1, 0, 0, 0xB2, 0, 0 },
    { -1, 0x80, 0x3A, 0xB3, 2, 0x6B },
    { 0x31, 0xA0, 0x2E, 0xB4, 2, 0x4D },
    { 0x31, 0xA0, 0x2E, 0xB5, 2, 0x4D },
    { 0x31, 0xA0, 0x2E, 0xB6, 2, 0x4D },
    { 0x31, 0xA0, 0x2E, 0xB7, 2, 0x4D },
    { 0x31, 0xA0, 0x2E, 0xB8, 2, 0x4D },
    { 0x31, 0xA0, 0x2E, 0xB9, 2, 0x4D },
};
#endif

void func_8082ECE0(Player* this) {
    struct_8085C3F4* temp = &D_8085C3F4[this->getItemId - 1];

    this->unk_B2A = ABS_ALT(temp->unk_2);
}

LinkAnimationHeader* func_8082ED20(Player* this) {
    if ((this->transformation == PLAYER_FORM_ZORA) || (this->actor.id != ACTOR_PLAYER)) {
        return &gameplay_keep_Linkanim_00E410;
    }
    if (this->transformation == PLAYER_FORM_GORON) {
        return &gameplay_keep_Linkanim_00E260;
    }
    if (this->currentMask == PLAYER_MASK_SCENTS) {
        return &gameplay_keep_Linkanim_00D0B0;
    }
    return GET_PLAYER_ANIM(PLAYER_ANIMGROUP_0, this->modelAnimType);
}

#ifdef NON_EQUIVALENT
extern void* D_8085C84C[0x1E];
s32 func_8082ED94(Player* arg0) {
    s32 var_v1;
    s32* temp_v0;
    s32* temp_v0_2;
    s32* var_v0;
    void* temp_a1;
    void* temp_a1_2;
    s32 i;

    temp_a1 = arg0->skelAnime.animation;
    if ((&gameplay_keep_Linkanim_00DD58 != temp_a1) && (&gameplay_keep_Linkanim_00DD68 != temp_a1) &&
        (((func_8082ED20(arg0) != arg0->skelAnime.animation)) ||
         (&gameplay_keep_Linkanim_00D0B0 == arg0->skelAnime.animation))) {
#if 0
        temp_v0 = D_8085C84C + 4;
        if (temp_a1_2 == *D_8085C84C) {
            return 1;
        }
        var_v0 = temp_v0 + 4;
        var_v1 = 2;
        if (temp_a1_2 == *temp_v0) {
            return 2;
        }
loop_8:
        if (temp_a1_2 == var_v0->unk_0) {
            return var_v1 + 1;
        }
        temp_v0_2 = var_v0 + 4;
        if (temp_a1_2 == var_v0->unk_4) {
            return var_v1 + 2;
        }
        temp_v0_3 = temp_v0_2 + 4;
        if (temp_a1_2 == temp_v0_2->unk_4) {
            return var_v1 + 3;
        }
        if (temp_a1_2 == temp_v0_3->unk_4) {
            return var_v1 + 4;
        }
        var_v1 += 4;
        var_v0 = temp_v0_3 + 4 + 4;
        if (var_v1 == 0x1E) {
            return 0;
        }
        goto loop_8;
#endif
        for (i = 0; i < ARRAY_COUNT(D_8085C84C); i++) {
            if (arg0->skelAnime.animation == D_8085C84C[i]) {
                return i + 1;
            }
        }
    }
    return -1;
}
#else
s32 func_8082ED94(Player* this);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8082ED94.s")
#endif

extern u8 D_8085C96C[];
extern struct_8082E224_arg1* D_8085C93C[];

void func_8082EEA4(Player* this, s32 arg1) {
    u8 temp_v0 = D_8085C96C[arg1];

    if (temp_v0 != 0) {
        func_8082E224(this, D_8085C93C[temp_v0 - 1]);
    }
}

LinkAnimationHeader* func_8082EEE0(Player* this) {
    if (this->unk_B64 != 0) {
        return GET_PLAYER_ANIM(PLAYER_ANIMGROUP_3, this->modelAnimType);
    }
    return GET_PLAYER_ANIM(PLAYER_ANIMGROUP_2, this->modelAnimType);
}

s32 func_8082EF20(Player* this) {
    return func_8082ECCC(this) && this->unk_ACC != 0;
}

LinkAnimationHeader* func_8082EF54(Player* this) {
    if (func_8082EF20(this)) {
        return &gameplay_keep_Linkanim_00D490;
    }
    return GET_PLAYER_ANIM(PLAYER_ANIMGROUP_5, this->modelAnimType);
}

LinkAnimationHeader* func_8082EF9C(Player* this) {
    if (func_8082EF20(this)) {
        return &gameplay_keep_Linkanim_00D488;
    }
    return GET_PLAYER_ANIM(PLAYER_ANIMGROUP_4, this->modelAnimType);
}

LinkAnimationHeader* func_8082EFE4(Player* this) {
    if (func_800B7128(this)) {
        return &gameplay_keep_Linkanim_00D520;
    }
    return GET_PLAYER_ANIM(PLAYER_ANIMGROUP_22, this->modelAnimType);
}

typedef struct {
    /* 0x0 */ Color_RGB8 unk_0;
    /* 0x3 */ Color_RGB8 unk_3;
    /* 0x6 */ Color_RGB8 unk_6;
    /* 0xA */ s16 unk_A;
    /* 0xC */ s16 unk_C;
} struct_8082F02C_arg1; // size = 0xE

void func_8082F02C(PlayState* play, struct_8082F02C_arg1* arg1, f32 arg2) {
    func_800FD59C(play, &arg1->unk_0, arg2);
    func_800FD5E0(play, &arg1->unk_3, arg2);
    func_800FD654(play, &arg1->unk_6, arg2);
    func_800FD698(play, arg1->unk_A, arg1->unk_C, arg2);
}

void func_8082F09C(Player* this) {
    this->cylinder.base.colType = 5;
    this->cylinder.base.atFlags = 0;
    this->cylinder.base.acFlags = 0x11;
    this->cylinder.base.ocFlags1 = 0x39;
    this->cylinder.info.elemType = 1;
    this->cylinder.info.toucher.dmgFlags = 0;
    this->cylinder.info.bumper.dmgFlags = 0xF7CFFFFF;
    this->cylinder.info.toucherFlags = 0;
    this->cylinder.dim.radius = 0xC;
}

void func_8082F0E4(Player* arg0, u32 arg1, s32 arg2, s32 arg3) {
    arg0->cylinder.base.atFlags = 9;
    if (arg3 >= 0x1F) {
        arg0->cylinder.base.ocFlags1 = 0;
    } else {
        arg0->cylinder.base.ocFlags1 = 0x39;
    }

    arg0->cylinder.info.elemType = 2;
    arg0->cylinder.info.toucherFlags = 5;
    arg0->cylinder.dim.radius = arg3;
    arg0->cylinder.info.toucher.dmgFlags = arg1;
    arg0->cylinder.info.toucher.damage = arg2;

    if (arg1 & 0x400) {
        arg0->cylinder.base.acFlags = 0;
    } else {
        arg0->cylinder.base.colType = 0xA;
        arg0->cylinder.info.bumper.dmgFlags = 0xF7CFFFFF;

        if (arg1 & 0x80000) {
            arg0->cylinder.base.acFlags = 0;
        } else {
            arg0->cylinder.base.acFlags = 0x11;
        }
    }
}

void func_8082F164(Player* arg0, u16 button) {
    if ((arg0->transformation == PLAYER_FORM_ZORA) && CHECK_BTN_ALL(D_80862B44->cur.button, button)) {
        arg0->stateFlags1 |= PLAYER_STATE1_10;
    }
}

extern struct_8082F02C_arg1 D_8085C98C;

#ifdef NON_MATCHING
// stack
void func_8082F1AC(PlayState* play, Player* this) {
    f32 temp;
    s32 var_v0;
    s32 sp4C;
    s32 pad;

    sp4C = this->unk_B62;
    if ((gSaveContext.save.playerData.magic != 0) && (this->stateFlags1 & PLAYER_STATE1_10)) {
        if (gSaveContext.unk_3F28 == 0) {
            // Magic_Consume
            func_80115DB4(play, 0, 5);
        }

        temp = 16.0f;
        if (gSaveContext.save.playerData.magic >= 16) {
            var_v0 = 0xFF;
        } else {
            var_v0 = (gSaveContext.save.playerData.magic / temp) * 255.0f;
        }
        Math_StepToS(&this->unk_B62, var_v0, 50);
    } else if ((Math_StepToS(&this->unk_B62, 0, 50) != 0) && (gSaveContext.unk_3F28 != 0)) {
        // Magic_Reset
        func_80115D5C(&play->state);
    }

    if ((this->unk_B62 != 0) || (sp4C != 0)) {
        s16 sp46;
        s16 sp44;
        f32 sp40;
        f32 sp3C;
        f32 new_var;
        f32 sp34;

        sp46 = play->gameplayFrames * 7000;
        sp44 = play->gameplayFrames * 14000;
        func_8082F02C(play, &D_8085C98C, this->unk_B62 / 255.0f);

        sp34 = Math_SinS(sp44) * 40.0f;
        sp40 = Math_CosS(sp44) * 40.0f;
        sp3C = Math_SinS(sp46) * sp34;

        new_var = Math_CosS(sp46);
        Lights_PointNoGlowSetInfo(&this->lightInfo, this->actor.world.pos.x + sp40, this->actor.world.pos.y + sp3C,
                                  this->actor.world.pos.z + (new_var * sp34), 100, 200, 255, 600);

        func_800B8E58(this, NA_SE_PL_ZORA_SPARK_BARRIER - SFX_FLAG);
        func_800B648C(play, 1, 2, 100.0f, &this->actor.world.pos);
    }
}
#else
void func_8082F1AC(PlayState* play, Player* this);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8082F1AC.s")
#endif

void func_8082F43C(PlayState* play, Player* this, s32 (*arg2)(Player*, PlayState*)) {
    this->unk_AC4 = arg2;
    this->unk_ACE = 0;
    this->unk_AC8 = 0.0f;
    func_8082E00C(this);
}

// OoT: func_80833664
void func_8082F470(PlayState* play, Player* this, s32 arg2) {
    LinkAnimationHeader* current = this->skelAnime.animation;
    LinkAnimationHeader** iter = &D_8085BE84[this->modelAnimType];
    s32 i;

    this->stateFlags1 &= ~(PLAYER_STATE1_8 | PLAYER_STATE1_1000000);

    for (i = 0; i < PLAYER_ANIMGROUP_MAX; i++) {
        if (current == *iter) {
            break;
        }
        iter += PLAYER_ANIMTYPE_MAX;
    }

    func_8082F8BC(play, this, arg2);
    if (i < PLAYER_ANIMGROUP_MAX) {
        this->skelAnime.animation = GET_PLAYER_ANIM(i, this->modelAnimType);
    }
}

// sItemActionParams
extern s8 D_8085C99C[];

#if 0
s8 D_8085C99C[] = {
    PLAYER_AP_OCARINA, // ITEM_OCARINA,
    PLAYER_AP_BOW, // ITEM_BOW,
    PLAYER_AP_BOW_FIRE, // ITEM_ARROW_FIRE,
    PLAYER_AP_BOW_ICE, // ITEM_ARROW_ICE,
    PLAYER_AP_BOW_LIGHT, // ITEM_ARROW_LIGHT,
    PLAYER_AP_PICTO_BOX, // ITEM_OCARINA_FAIRY,
    PLAYER_AP_BOMB, // ITEM_BOMB,
    PLAYER_AP_BOMBCHU, // ITEM_BOMBCHU,
    PLAYER_AP_STICK, // ITEM_STICK,
    PLAYER_AP_NUT, // ITEM_NUT,
    PLAYER_AP_MAGIC_BEANS, // ITEM_MAGIC_BEANS,
    PLAYER_AP_PICTO_BOX, // ITEM_SLINGSHOT,
    PLAYER_AP_POWDER_KEG, // ITEM_POWDER_KEG,
    PLAYER_AP_PICTO_BOX, // ITEM_PICTO_BOX,
    PLAYER_AP_LENS, // ITEM_LENS,
    PLAYER_AP_HOOKSHOT, // ITEM_HOOKSHOT,
    PLAYER_AP_SWORD_GREAT_FAIRY, // ITEM_SWORD_GREAT_FAIRY,
    PLAYER_AP_PICTO_BOX, // ITEM_LONGSHOT, // OoT Leftover
    PLAYER_AP_BOTTLE, // ITEM_BOTTLE,
    PLAYER_AP_BOTTLE_POTION_RED, // ITEM_POTION_RED,
    PLAYER_AP_BOTTLE_POTION_GREEN, // ITEM_POTION_GREEN,
    PLAYER_AP_BOTTLE_POTION_BLUE, // ITEM_POTION_BLUE,
    PLAYER_AP_BOTTLE_FAIRY, // ITEM_FAIRY,
    PLAYER_AP_BOTTLE_DEKU_PRINCESS, // ITEM_DEKU_PRINCESS,
    PLAYER_AP_BOTTLE_MILK, // ITEM_MILK_BOTTLE,
    PLAYER_AP_BOTTLE_MILK_HALF, // ITEM_MILK_HALF,
    PLAYER_AP_BOTTLE_FISH, // ITEM_FISH,
    PLAYER_AP_BOTTLE_BUG, // ITEM_BUG,
    PLAYER_AP_BOTTLE_BUG, // ITEM_BLUE_FIRE, // !
    PLAYER_AP_BOTTLE_POE, // ITEM_POE,
    PLAYER_AP_BOTTLE_BIG_POE, // ITEM_BIG_POE,
    PLAYER_AP_BOTTLE_SPRING_WATER, // ITEM_SPRING_WATER,
    PLAYER_AP_BOTTLE_HOT_SPRING_WATER, // ITEM_HOT_SPRING_WATER,
    PLAYER_AP_BOTTLE_ZORA_EGG, // ITEM_ZORA_EGG,
    PLAYER_AP_BOTTLE_GOLD_DUST, // ITEM_GOLD_DUST,
    PLAYER_AP_BOTTLE_MUSHROOM, // ITEM_MUSHROOM,
    PLAYER_AP_BOTTLE_SEA_HORSE, // ITEM_SEA_HORSE,
    PLAYER_AP_BOTTLE_CHATEAU, // ITEM_CHATEAU,
    PLAYER_AP_BOTTLE_HYLIAN_LOACH, // ITEM_HYLIAN_LOACH,
    PLAYER_AP_BOTTLE_POE, // ITEM_OBABA_DRINK, // !
    PLAYER_AP_MOON_TEAR, // ITEM_MOON_TEAR,
    PLAYER_AP_DEED_LAND, // ITEM_DEED_LAND,
    PLAYER_AP_DEED_SWAMP, // ITEM_DEED_SWAMP,
    PLAYER_AP_DEED_MOUNTAIN, // ITEM_DEED_MOUNTAIN,
    PLAYER_AP_DEED_OCEAN, // ITEM_DEED_OCEAN,
    PLAYER_AP_ROOM_KEY, // ITEM_ROOM_KEY,
    PLAYER_AP_LETTER_MAMA, // ITEM_LETTER_MAMA,
    PLAYER_AP_LETTER_TO_KAFEI, // ITEM_LETTER_TO_KAFEI,
    PLAYER_AP_PENDANT_MEMORIES, // ITEM_PENDANT_MEMORIES,
    PLAYER_AP_38, // ITEM_TINGLE_MAP, // !
    PLAYER_AP_MASK_DEKU, // ITEM_MASK_DEKU,
    PLAYER_AP_MASK_GORON, // ITEM_MASK_GORON,
    PLAYER_AP_MASK_ZORA, // ITEM_MASK_ZORA,
    PLAYER_AP_MASK_FIERCE_DEITY, // ITEM_MASK_FIERCE_DEITY,
    PLAYER_AP_MASK_TRUTH, // ITEM_MASK_TRUTH,
    PLAYER_AP_MASK_KAFEIS_MASK, // ITEM_MASK_KAFEIS_MASK,
    PLAYER_AP_MASK_ALL_NIGHT, // ITEM_MASK_ALL_NIGHT,
    PLAYER_AP_MASK_BUNNY, // ITEM_MASK_BUNNY,
    PLAYER_AP_MASK_KEATON, // ITEM_MASK_KEATON,
    PLAYER_AP_MASK_GARO, // ITEM_MASK_GARO,
    PLAYER_AP_MASK_ROMANI, // ITEM_MASK_ROMANI,
    PLAYER_AP_MASK_CIRCUS_LEADER, // ITEM_MASK_CIRCUS_LEADER,
    PLAYER_AP_MASK_POSTMAN, // ITEM_MASK_POSTMAN,
    PLAYER_AP_MASK_COUPLE, // ITEM_MASK_COUPLE,
    PLAYER_AP_MASK_GREAT_FAIRY, // ITEM_MASK_GREAT_FAIRY,
    PLAYER_AP_MASK_GIBDO, // ITEM_MASK_GIBDO,
    PLAYER_AP_MASK_DON_GERO, // ITEM_MASK_DON_GERO,
    PLAYER_AP_MASK_KAMARO, // ITEM_MASK_KAMARO,
    PLAYER_AP_MASK_CAPTAIN, // ITEM_MASK_CAPTAIN,
    PLAYER_AP_MASK_STONE, // ITEM_MASK_STONE,
    PLAYER_AP_MASK_BREMEN, // ITEM_MASK_BREMEN,
    PLAYER_AP_MASK_BLAST, // ITEM_MASK_BLAST,
    PLAYER_AP_MASK_SCENTS, // ITEM_MASK_SCENTS,
    PLAYER_AP_MASK_GIANT, // ITEM_MASK_GIANT,
    PLAYER_AP_BOW_FIRE, // ITEM_BOW_ARROW_FIRE,
    PLAYER_AP_BOW_ICE, // ITEM_BOW_ARROW_ICE,
    PLAYER_AP_BOW_LIGHT, // ITEM_BOW_ARROW_LIGHT,
    PLAYER_AP_SWORD_KOKIRI, // ITEM_SWORD_KOKIRI,
    PLAYER_AP_SWORD_RAZOR, // ITEM_SWORD_RAZOR,
    PLAYER_AP_SWORD_GILDED, // ITEM_SWORD_GILDED,
    PLAYER_AP_SWORD_GREAT_FAIRY, // ITEM_SWORD_DEITY,
};
#endif

// Player_ItemToActionParam
PlayerActionParam func_8082F524(Player* this, ItemID item) {
    if (item >= ITEM_FD) {
        return PLAYER_AP_NONE;
    }
    if (item == ITEM_FC) {
        return PLAYER_AP_LAST_USED;
    }
    if (item == ITEM_FISHING_POLE) {
        return PLAYER_AP_FISHING_POLE;
    }
    if ((item == ITEM_SWORD_KOKIRI) && (this->transformation == PLAYER_FORM_ZORA)) {
        return PLAYER_AP_ZORA_FINS;
    }
    return D_8085C99C[item];
}

void func_8082F594(PlayState* play, Player* this) {
}

void func_8082F5A4(PlayState* play, Player* this) {
    this->unk_B28 = 0;
    this->unk_B08[1] = 1.0f;
}

void func_8082F5C0(PlayState* play, Player* this) {
    this->stateFlags1 |= 8;
    if (this->itemActionParam == 0x12) {
        this->unk_B28 = -2;
    } else {
        this->unk_B28 = -1;
    }
    this->unk_ACC = 0;
}

void func_8082F5FC(Player* this, Actor* actor) {
    this->heldActor = actor;
    this->interactRangeActor = actor;
    this->getItemId = 0;
    this->leftHandWorld.rot.y = actor->shape.rot.y - this->actor.shape.rot.y;
    this->stateFlags1 |= PLAYER_STATE1_800;
}

typedef struct {
    /* 0x0 */ u8 itemId;
    /* 0x2 */ s16 actorId;
} struct_8085CD24; // size = 0x4

extern struct_8085CD24 D_8085CD24[];
#if 0
struct_8085CD24 D_8085CD24[] = {
    { ITEM_BOMB, ACTOR_EN_BOM }, // PLAYER_AP_BOMB
    { ITEM_POWDER_KEG, ACTOR_EN_BOM }, // PLAYER_AP_POWDER_KEG
    { ITEM_BOMBCHU, ACTOR_EN_BOM_CHU }, // PLAYER_AP_BOMBCHU
};
#endif

void func_8082F62C(PlayState* play, Player* this) {
    s32 explosiveType;
    struct_8085CD24* explosiveInfo;
    Actor* explosiveActor;

    if (this->stateFlags1 & PLAYER_STATE1_800) {
        func_8082DE14(play, this);
        return;
    }

    explosiveType = Player_GetExplosiveHeld(this);
    explosiveInfo = &D_8085CD24[explosiveType];
    if ((explosiveType == 1) && (gSaveContext.powderKegTimer == 0)) {
        gSaveContext.powderKegTimer = 0xC8;
    }

    explosiveActor = Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, explosiveInfo->actorId,
                                        this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z,
                                        explosiveType == 1 ? 1 : 0, this->actor.shape.rot.y, 0, 0);
    if (explosiveActor != NULL) {
        if ((explosiveType == 0) && (play->unk_1887E != 0)) {
            play->unk_1887E--;
            if (play->unk_1887E == 0) {
                play->unk_1887E = -1;
            }
        } else if ((explosiveType == 2) && (play->unk_1887D != 0)) {
            play->unk_1887D--;
            if (play->unk_1887D == 0) {
                play->unk_1887D = -1;
            }
        } else {
            Inventory_ChangeAmmo(explosiveInfo->itemId, -1);
        }
        func_8082F5FC(this, explosiveActor);
    } else if (explosiveType == 1) {
        gSaveContext.powderKegTimer = 0;
    }
}

void func_8082F7F4(PlayState* play, Player* this) {
    ArmsHook* armsHook;

    this->stateFlags1 |= 8;
    this->unk_B28 = -3;
    this->unk_B48 = 0.0f;

    this->heldActor =
        Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACTOR_ARMS_HOOK, this->actor.world.pos.x,
                           this->actor.world.pos.y, this->actor.world.pos.z, 0, this->actor.shape.rot.y, 0, 0);
    if (this->heldActor == NULL) {
        func_80831990(play, this, 0xFFU);
        return;
    }

    armsHook = (ArmsHook*)this->heldActor;
    armsHook->actor.objBankIndex = this->actor.objBankIndex;
    armsHook->unk_208 = this->transformation;
}

void func_8082F8A0(PlayState* play, Player* this) {
    this->stateFlags1 |= 0x01000000;
}

extern void (*D_8085CB3C[PLAYER_AP_MAX])(PlayState* play, Player* this);

#if 0
void (*D_8085CB3C[0x53])(PlayState*, Player*) = {
    func_8082F594, // PLAYER_AP_NONE
    func_8082F594, // PLAYER_AP_LAST_USED
    func_8082F594, // PLAYER_AP_FISHING_POLE
    func_8082F594, // PLAYER_AP_SWORD_KOKIRI
    func_8082F594, // PLAYER_AP_SWORD_RAZOR
    func_8082F594, // PLAYER_AP_SWORD_GILDED
    func_8082F594, // PLAYER_AP_SWORD_GREAT_FAIRY
    func_8082F5A4, // PLAYER_AP_STICK
    func_8082F8A0, // PLAYER_AP_ZORA_FINS
    func_8082F5C0, // PLAYER_AP_BOW
    func_8082F5C0, // PLAYER_AP_BOW_FIRE
    func_8082F5C0, // PLAYER_AP_BOW_ICE
    func_8082F5C0, // PLAYER_AP_BOW_LIGHT
    func_8082F7F4, // PLAYER_AP_HOOKSHOT
    func_8082F62C, // PLAYER_AP_BOMB
    func_8082F62C, // PLAYER_AP_POWDER_KEG
    func_8082F62C, // PLAYER_AP_BOMBCHU
    func_8082F8A0, // PLAYER_AP_11
    func_8082F5C0, // PLAYER_AP_NUT
    func_8082F594, // PLAYER_AP_PICTO_BOX
    func_8082F594, // PLAYER_AP_OCARINA
    func_8082F594, // PLAYER_AP_BOTTLE
    func_8082F594, // PLAYER_AP_BOTTLE_FISH
    func_8082F594, // PLAYER_AP_BOTTLE_SPRING_WATER
    func_8082F594, // PLAYER_AP_BOTTLE_HOT_SPRING_WATER
    func_8082F594, // PLAYER_AP_BOTTLE_ZORA_EGG
    func_8082F594, // PLAYER_AP_BOTTLE_DEKU_PRINCESS
    func_8082F594, // PLAYER_AP_BOTTLE_GOLD_DUST
    func_8082F594, // PLAYER_AP_BOTTLE_1C
    func_8082F594, // PLAYER_AP_BOTTLE_SEA_HORSE
    func_8082F594, // PLAYER_AP_BOTTLE_MUSHROOM
    func_8082F594, // PLAYER_AP_BOTTLE_HYLIAN_LOACH
    func_8082F594, // PLAYER_AP_BOTTLE_BUG
    func_8082F594, // PLAYER_AP_BOTTLE_POE
    func_8082F594, // PLAYER_AP_BOTTLE_BIG_POE
    func_8082F594, // PLAYER_AP_BOTTLE_POTION_RED
    func_8082F594, // PLAYER_AP_BOTTLE_POTION_BLUE
    func_8082F594, // PLAYER_AP_BOTTLE_POTION_GREEN
    func_8082F594, // PLAYER_AP_BOTTLE_MILK
    func_8082F594, // PLAYER_AP_BOTTLE_MILK_HALF
    func_8082F594, // PLAYER_AP_BOTTLE_CHATEAU
    func_8082F594, // PLAYER_AP_BOTTLE_FAIRY
    func_8082F594, // PLAYER_AP_MOON_TEAR
    func_8082F594, // PLAYER_AP_DEED_LAND
    func_8082F594, // PLAYER_AP_ROOM_KEY
    func_8082F594, // PLAYER_AP_LETTER_TO_KAFEI
    func_8082F594, // PLAYER_AP_MAGIC_BEANS
    func_8082F594, // PLAYER_AP_DEED_SWAMP
    func_8082F594, // PLAYER_AP_DEED_MOUNTAIN
    func_8082F594, // PLAYER_AP_DEED_OCEAN
    func_8082F594, // PLAYER_AP_32
    func_8082F594, // PLAYER_AP_LETTER_MAMA
    func_8082F594, // PLAYER_AP_34
    func_8082F594, // PLAYER_AP_35
    func_8082F594, // PLAYER_AP_PENDANT_MEMORIES
    func_8082F594, // PLAYER_AP_37
    func_8082F594, // PLAYER_AP_38
    func_8082F594, // PLAYER_AP_39
    func_8082F594, // PLAYER_AP_MASK_TRUTH
    func_8082F594, // PLAYER_AP_MASK_KAFEIS_MASK
    func_8082F594, // PLAYER_AP_MASK_ALL_NIGHT
    func_8082F594, // PLAYER_AP_MASK_BUNNY
    func_8082F594, // PLAYER_AP_MASK_KEATON
    func_8082F594, // PLAYER_AP_MASK_GARO
    func_8082F594, // PLAYER_AP_MASK_ROMANI
    func_8082F594, // PLAYER_AP_MASK_CIRCUS_LEADER
    func_8082F594, // PLAYER_AP_MASK_POSTMAN
    func_8082F594, // PLAYER_AP_MASK_COUPLE
    func_8082F594, // PLAYER_AP_MASK_GREAT_FAIRY
    func_8082F594, // PLAYER_AP_MASK_GIBDO
    func_8082F594, // PLAYER_AP_MASK_DON_GERO
    func_8082F594, // PLAYER_AP_MASK_KAMARO
    func_8082F594, // PLAYER_AP_MASK_CAPTAIN
    func_8082F594, // PLAYER_AP_MASK_STONE
    func_8082F594, // PLAYER_AP_MASK_BREMEN
    func_8082F594, // PLAYER_AP_MASK_BLAST
    func_8082F594, // PLAYER_AP_MASK_SCENTS
    func_8082F594, // PLAYER_AP_MASK_GIANT
    func_8082F594, // PLAYER_AP_MASK_FIERCE_DEITY
    func_8082F594, // PLAYER_AP_MASK_GORON
    func_8082F594, // PLAYER_AP_MASK_ZORA
    func_8082F594, // PLAYER_AP_MASK_DEKU
    func_8082F594, // PLAYER_AP_LENS
};
#endif

#ifdef NON_MATCHING
void func_8082F8BC(PlayState* play, Player* this, PlayerActionParam actionParam) {
    this->itemActionParam = actionParam;
    this->heldItemActionParam = actionParam;
    this->stateFlags1 &= ~0x1000008;
    this->unk_B08[0] = 0.0f;
    this->unk_B08[1] = 0.0f;
    this->unk_B28 = 0;
    this->modelGroup = this->nextModelGroup;
    D_8085CB3C[actionParam](play, this);
    Player_SetModelGroup(this, this->modelGroup);
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8082F8BC.s")
#endif

void func_8082F938(PlayState* play, Player* this, UNK_TYPE arg2, UNK_TYPE arg3);
// has a loop unroll
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8082F938.s")

void func_8082FA5C(PlayState* play, Player* this, s32 meleeWeaponState) {
    u16 var_a2;
    u16 var_a1;

    if (this->meleeWeaponState == 0) {
        var_a2 = NA_SE_VO_LI_SWORD_N;
        if (this->transformation == PLAYER_FORM_GORON) {
            var_a1 = NA_SE_IT_GORON_PUNCH_SWING;
        } else {
            var_a1 = 0;
            if (this->meleeWeaponAnimation >= PLAYER_MWA_SPIN_ATTACK_1H) {
                var_a2 = NA_SE_VO_LI_SWORD_L;
            } else if (this->meleeWeaponAnimation == PLAYER_MWA_ZORA_PUNCH_KICK) {
                var_a1 = NA_SE_IT_GORON_PUNCH_SWING;
            } else {
                var_a1 = NA_SE_IT_SWORD_SWING_HARD;
                if (this->unk_ADD >= 3) {
                    var_a2 = NA_SE_VO_LI_SWORD_L;
                } else {
                    var_a1 = (this->itemActionParam == PLAYER_AP_SWORD_GREAT_FAIRY) ? NA_SE_IT_HAMMER_SWING
                                                                                    : NA_SE_IT_SWORD_SWING;
                }
            }
        }

        if (var_a1 != 0) {
            func_8082E1F0(this, var_a1);
        }

        if (!((this->meleeWeaponAnimation >= PLAYER_MWA_FLIPSLASH_START) &&
              (this->meleeWeaponAnimation <= PLAYER_MWA_ZORA_JUMPKICK_FINISH))) {
            func_8082DF8C(this, var_a2);
        }

        func_8082F938(play, this, 0, 4);
    }

    this->meleeWeaponState = meleeWeaponState;
}

s32 func_8082FB68(Player* this) {
    if ((this->unk_730 != NULL) && CHECK_FLAG_ALL(this->unk_730->flags, ACTOR_FLAG_1 | ACTOR_FLAG_4)) {
        this->stateFlags3 |= PLAYER_STATE3_80000000;
        return true;
    }

    if (this->stateFlags3 & PLAYER_STATE3_80000000) {
        this->stateFlags3 &= ~PLAYER_STATE3_80000000;
        if (this->linearVelocity == 0.0f) {
            this->currentYaw = this->actor.shape.rot.y;
        }
    }

    return false;
}

s32 func_8082FBE8(Player* this) {
    return func_80123420(this) || func_80123434(this);
}

s32 func_8082FC24(Player* this) {
    return func_8082FB68(this) || func_80123434(this);
}

void func_8082FC60(Player* this) {
    this->unk_B44 = 0.0f;
    this->unk_B40 = 0.0f;
}

s32 func_8082FC78(Player* this, ItemID item) {
    if ((item < ITEM_FD) && (func_8082F524(this, item) == this->heldItemActionParam)) {
        return true;
    }
    return false;
}

s32 func_8082FCC4(Player* this, ItemID item, PlayerActionParam actionParam) {
    if ((item < ITEM_FD) && (func_8082F524(this, item) == actionParam)) {
        return true;
    }
    return false;
}

s32 func_8082FD0C(Player* this, PlayerActionParam actionParam) {
    s32 btn;

    for (btn = EQUIP_SLOT_C_LEFT; btn <= EQUIP_SLOT_C_RIGHT; btn++) {
        if (func_8082FCC4(this, GET_CUR_FORM_BTN_ITEM(btn), actionParam)) {
            return btn;
        }
    }
    return -1;
}

extern u16 D_8085CFA8[4];
#if 0
u16 D_8085CFA8[] = { BTN_B, BTN_CLEFT, BTN_CDOWN, BTN_CRIGHT, };
#endif

s32 func_8082FDC4(void) {
    s32 i;

    for (i = 0; i < ARRAY_COUNT(D_8085CFA8); i++) {
        if (CHECK_BTN_ALL(D_80862B44->press.button, D_8085CFA8[i])) {
            break;
        }
    }

    return i;
}

void func_8082FE0C(Player* this, PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8082FE0C.s")

void func_808302CC(Player* this, PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_808302CC.s")

void func_808304BC(Player* this, PlayState* play) {
    if ((this->actor.id == ACTOR_PLAYER) && !(this->stateFlags3 & PLAYER_STATE3_40000000)) {
        if ((this->itemActionParam == this->heldItemActionParam) || (this->stateFlags1 & PLAYER_STATE1_400000)) {
            if ((gSaveContext.save.playerData.health != 0) && (play->csCtx.state == 0)) {
                if ((this->csMode == 0) && (play->unk_1887C == 0) && (play->activeCamId == 0)) {
                    if (!func_8082DA90(play) && (gSaveContext.unk_3DD0[4] != 5)) {
                        func_8082FE0C(this, play);
                    }
                }
            }
        }
    }

    if (this->stateFlags3 & PLAYER_STATE3_40000000) {
        func_808302CC(this, play);
    }
}

#ifdef NON_MATCHING
// missing instruction
s32 func_808305BC(PlayState* arg0, Player* arg1, ItemID* item, s32* arg3) {
    if (arg1->itemActionParam == PLAYER_AP_NUT) {
        *item = ITEM_NUT;
        *arg3 = (arg1->transformation == PLAYER_FORM_DEKU) ? 7 : 6;
    } else {
        *item = ITEM_BOW;
        *arg3 = (arg1->stateFlags1 & PLAYER_STATE1_800000) ? 1 : arg1->itemActionParam - 7;
    }

    if (arg1->transformation == PLAYER_FORM_DEKU) {
        if ((gSaveContext.save.playerData.magic >= 2) ||
            ((gSaveContext.save.weekEventReg[8] & 1) && (arg0->sceneNum == SCENE_BOWLING))) {
            return 1;
        }
        return 0;
    }
    if (arg1->stateFlags3 & 0x400) {
        return 1;
    }
    if (gSaveContext.minigameState == 1) {
        return arg0->interfaceCtx.hbaAmmo;
    }
    if (arg0->unk_1887C != 0) {
        return arg0->unk_1887C;
    }

    return AMMO(*item);
}
#else
s32 func_808305BC(PlayState* arg0, Player* arg1, ItemID* item, s32* arg3);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_808305BC.s")
#endif

// todo: fix
extern u16 D_8085CFAE[];

#if 0
s32 func_808306F8(Player* arg0, PlayState* arg1) {
    s32 sp50;
    s32 sp4C;
    s32 sp48;
    Actor* temp_v0_4;
    s16 temp_v0_2;
    s16 var_v1;
    s32 temp_v0_3;
    s32 var_t0;
    s8 temp_v0;

    temp_v0 = arg0->itemActionParam;
    if ((temp_v0 >= 0xA) && (temp_v0 < 0xD) && (gSaveContext.unk_3F28 != 0)) {
        play_sound(0x4806U);
        return 0;
    }
    func_8082F43C(arg1, arg0, func_80848BF4);
    temp_v0_2 = arg0->unk_B28;
    arg0->stateFlags3 |= 0x40;
    arg0->unk_ACC = 0xE;
    if (temp_v0_2 >= 0) {
        if (temp_v0_2 < 0) {
            var_v1 = -temp_v0_2;
        } else {
            var_v1 = temp_v0_2;
        }
        if (var_v1 != 2) {
            func_800B8E58(arg0, D_8085CFAE[var_v1]);
        }
        if ((Player_IsHoldingHookshot(arg0) == 0) && (func_808305BC(arg1, arg0, &sp50, &sp4C) > 0)) {
            temp_v0_3 = sp4C - 3;
            if (arg0->unk_B28 >= 0) {
                var_t0 = temp_v0_3;
                if ((temp_v0_3 >= 0) && (temp_v0_3 < 3)) {
                    if (gSaveContext.save.playerData.magic < (s32) *(&D_8085CFB8 + temp_v0_3)) {
                        var_t0 = -1;
                        sp4C = 2;
                    }
                } else if ((sp4C == 7) && (!(gSaveContext.save.weekEventReg[8] & 1) || (arg1->sceneNum != 0x11))) {
                    var_t0 = 3;
                } else {
                    var_t0 = -1;
                }
                sp48 = var_t0;
                temp_v0_4 = Actor_SpawnAsChild(&arg1->actorCtx, &arg0->actor, arg1, 0xF, arg0->actor.world.pos.x, arg0->actor.world.pos.y, arg0->actor.world.pos.z, (s16) 0, (s16) (s32) arg0->actor.shape.rot.y, (s16) 0, sp4C);
                arg0->heldActor = temp_v0_4;
                if ((temp_v0_4 != NULL) && (var_t0 >= 0)) {
                    func_80115DB4(arg1, (s16) *(&D_8085CFB8 + var_t0), 0);
                }
            }
        }
    }
    return 1;
}
#else
// bool
s32 func_808306F8(Player* this, PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_808306F8.s")
#endif

void func_808308DC(PlayState* play, Player* this);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_808308DC.s")

s32 func_80848780(Player* this, PlayState* play); /* static */
s32 func_808487B8(Player* this, PlayState* play); /* static */
s32 func_80848B6C(Player* this, PlayState* play); /* static */
s32 func_808490B4(Player* this, PlayState* play); /* static */
s32 func_808491B4(Player* this, PlayState* play); /* static */

s32 func_80848808(Player* this, PlayState* play); /* static */
s32 func_8084894C(Player* this, PlayState* play); /* static */
s32 func_80848A0C(Player* this, PlayState* play); /* static */
s32 func_80848AB0(Player* this, PlayState* play); /* static */
s32 func_80848E4C(Player* this, PlayState* play); /* static */
s32 func_80849054(Player* this, PlayState* play); /* static */
s32 func_8084923C(Player* this, PlayState* play); /* static */
s32 func_808492C4(Player* this, PlayState* play); /* static */
s32 func_8084933C(Player* this, PlayState* play); /* static */
s32 func_80849570(Player* this, PlayState* play); /* static */
s32 func_80849620(Player* this, PlayState* play); /* static */

// returns bool
extern s32 (*D_8085C9F0[PLAYER_AP_MAX])(Player*, PlayState*);

#if 0
s32 (*D_8085C9F0[0x53])(Player*, PlayState*) = {
    func_80848780, // PLAYER_AP_NONE
    func_80848780, // PLAYER_AP_LAST_USED
    func_80848780, // PLAYER_AP_FISHING_POLE
    func_808487B8, // PLAYER_AP_SWORD_KOKIRI
    func_808487B8, // PLAYER_AP_SWORD_RAZOR
    func_808487B8, // PLAYER_AP_SWORD_GILDED
    func_808487B8, // PLAYER_AP_SWORD_GREAT_FAIRY
    func_80848780, // PLAYER_AP_STICK
    func_80848780, // PLAYER_AP_ZORA_FINS
    func_80848B6C, // PLAYER_AP_BOW
    func_80848B6C, // PLAYER_AP_BOW_FIRE
    func_80848B6C, // PLAYER_AP_BOW_ICE
    func_80848B6C, // PLAYER_AP_BOW_LIGHT
    func_80848B6C, // PLAYER_AP_HOOKSHOT
    func_808490B4, // PLAYER_AP_BOMB
    func_808490B4, // PLAYER_AP_POWDER_KEG
    func_808490B4, // PLAYER_AP_BOMBCHU
    func_808491B4, // PLAYER_AP_11
    func_80848B6C, // PLAYER_AP_NUT
    func_80848780, // PLAYER_AP_PICTO_BOX
    func_80848780, // PLAYER_AP_OCARINA
    func_80848780, // PLAYER_AP_BOTTLE
    func_80848780, // PLAYER_AP_BOTTLE_FISH
    func_80848780, // PLAYER_AP_BOTTLE_SPRING_WATER
    func_80848780, // PLAYER_AP_BOTTLE_HOT_SPRING_WATER
    func_80848780, // PLAYER_AP_BOTTLE_ZORA_EGG
    func_80848780, // PLAYER_AP_BOTTLE_DEKU_PRINCESS
    func_80848780, // PLAYER_AP_BOTTLE_GOLD_DUST
    func_80848780, // PLAYER_AP_BOTTLE_1C
    func_80848780, // PLAYER_AP_BOTTLE_SEA_HORSE
    func_80848780, // PLAYER_AP_BOTTLE_MUSHROOM
    func_80848780, // PLAYER_AP_BOTTLE_HYLIAN_LOACH
    func_80848780, // PLAYER_AP_BOTTLE_BUG
    func_80848780, // PLAYER_AP_BOTTLE_POE
    func_80848780, // PLAYER_AP_BOTTLE_BIG_POE
    func_80848780, // PLAYER_AP_BOTTLE_POTION_RED
    func_80848780, // PLAYER_AP_BOTTLE_POTION_BLUE
    func_80848780, // PLAYER_AP_BOTTLE_POTION_GREEN
    func_80848780, // PLAYER_AP_BOTTLE_MILK
    func_80848780, // PLAYER_AP_BOTTLE_MILK_HALF
    func_80848780, // PLAYER_AP_BOTTLE_CHATEAU
    func_80848780, // PLAYER_AP_BOTTLE_FAIRY
    func_80848780, // PLAYER_AP_MOON_TEAR
    func_80848780, // PLAYER_AP_DEED_LAND
    func_80848780, // PLAYER_AP_ROOM_KEY
    func_80848780, // PLAYER_AP_LETTER_TO_KAFEI
    func_80848780, // PLAYER_AP_MAGIC_BEANS
    func_80848780, // PLAYER_AP_DEED_SWAMP
    func_80848780, // PLAYER_AP_DEED_MOUNTAIN
    func_80848780, // PLAYER_AP_DEED_OCEAN
    func_80848780, // PLAYER_AP_32
    func_80848780, // PLAYER_AP_LETTER_MAMA
    func_80848780, // PLAYER_AP_34
    func_80848780, // PLAYER_AP_35
    func_80848780, // PLAYER_AP_PENDANT_MEMORIES
    func_80848780, // PLAYER_AP_37
    func_80848780, // PLAYER_AP_38
    func_80848780, // PLAYER_AP_39
    func_80848780, // PLAYER_AP_MASK_TRUTH
    func_80848780, // PLAYER_AP_MASK_KAFEIS_MASK
    func_80848780, // PLAYER_AP_MASK_ALL_NIGHT
    func_80848780, // PLAYER_AP_MASK_BUNNY
    func_80848780, // PLAYER_AP_MASK_KEATON
    func_80848780, // PLAYER_AP_MASK_GARO
    func_80848780, // PLAYER_AP_MASK_ROMANI
    func_80848780, // PLAYER_AP_MASK_CIRCUS_LEADER
    func_80848780, // PLAYER_AP_MASK_POSTMAN
    func_80848780, // PLAYER_AP_MASK_COUPLE
    func_80848780, // PLAYER_AP_MASK_GREAT_FAIRY
    func_80848780, // PLAYER_AP_MASK_GIBDO
    func_80848780, // PLAYER_AP_MASK_DON_GERO
    func_80848780, // PLAYER_AP_MASK_KAMARO
    func_80848780, // PLAYER_AP_MASK_CAPTAIN
    func_80848780, // PLAYER_AP_MASK_STONE
    func_80848780, // PLAYER_AP_MASK_BREMEN
    func_80848780, // PLAYER_AP_MASK_BLAST
    func_80848780, // PLAYER_AP_MASK_SCENTS
    func_80848780, // PLAYER_AP_MASK_GIANT
    func_80848780, // PLAYER_AP_MASK_FIERCE_DEITY
    func_80848780, // PLAYER_AP_MASK_GORON
    func_80848780, // PLAYER_AP_MASK_ZORA
    func_80848780, // PLAYER_AP_MASK_DEKU
    func_80848780, // PLAYER_AP_LENS
};
#endif

void func_808309CC(PlayState* play, Player* this) {
    if (func_80848808 == this->unk_AC4) {
        func_808308DC(play, this);
    }

    func_8082F43C(play, this, D_8085C9F0[this->itemActionParam]);
    this->unk_ACC = 0;
    this->unk_AA4 = 0;
    func_8082DCA0(play, this);
    this->stateFlags3 &= ~PLAYER_STATE3_40000000;
}

extern LinkAnimationHeader* D_8085CFBC[2];
extern LinkAnimationHeader* D_8085CFC4[2];

LinkAnimationHeader* func_80830A58(PlayState* play, Player* this) {
    func_8082F43C(play, this, func_8084894C);
    func_8082DCA0(play, this);
    if (this->unk_B40 < 0.5f) {
        return D_8085CFBC[Player_IsHoldingTwoHandedWeapon(this)];
    }
    return D_8085CFC4[Player_IsHoldingTwoHandedWeapon(this)];
}

void func_80830AE8(Player* this) {
    s32 sfxId;

    if (this->transformation == PLAYER_FORM_GORON) {
        sfxId = NA_SE_PL_GORON_SQUAT;
    } else {
        s32 var_v1;

        if (this->transformation == PLAYER_FORM_DEKU) {
            var_v1 = NA_SE_PL_CHANGE_ARMS;
        } else {
            var_v1 = NA_SE_IT_SHIELD_SWING;
        }
        sfxId = var_v1;
    }

    func_800B8E58(this, sfxId);
}

void func_80830B38(Player* this) {
    s32 sfxId;

    if (this->transformation == PLAYER_FORM_GORON) {
        sfxId = NA_SE_PL_BALL_TO_GORON;
    } else {
        s32 var_v1;

        if (this->transformation == PLAYER_FORM_DEKU) {
            var_v1 = NA_SE_PL_TAKE_OUT_SHIELD;
        } else {
            var_v1 = NA_SE_IT_SHIELD_REMOVE;
        }
        sfxId = var_v1;
    }

    func_800B8E58(this, sfxId);
}

// bool
s32 func_80830B88(PlayState* play, Player* this) {
    if (CHECK_BTN_ALL(D_80862B44->cur.button, BTN_R)) {
        if (!(this->stateFlags1 & (PLAYER_STATE1_400000 | PLAYER_STATE1_800000 | PLAYER_STATE1_20000000))) {
            if (!(this->stateFlags1 & PLAYER_STATE1_8000000) ||
                ((this->currentBoots >= PLAYER_BOOTS_ZORA_UNDERWATER) && (this->actor.bgCheckFlags & 1))) {
                if ((play->unk_1887C == 0) && (this->itemActionParam == this->heldItemActionParam)) {
                    if ((this->transformation == PLAYER_FORM_FIERCE_DEITY) ||
                        (!Player_IsGoronOrDeku(this) &&
                         ((((this->transformation == PLAYER_FORM_ZORA)) &&
                           !(this->stateFlags1 & PLAYER_STATE1_2000000)) ||
                          ((this->transformation == PLAYER_FORM_HUMAN) &&
                           (this->currentShield != PLAYER_SHIELD_NONE))) &&
                         func_8082FBE8(this))) {
                        LinkAnimationHeader* anim = func_80830A58(play, this);
                        f32 lastFrame;

                        lastFrame = Animation_GetLastFrame(anim);
                        LinkAnimation_Change(play, &this->unk_284, anim, 3.0f / 3.0f, lastFrame, lastFrame, 2, 0.0f);
                        func_80830AE8(this);
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

void func_80830CE8(PlayState* play, Player* this) {
    func_8082F43C(play, this, func_80848AB0);
    if (this->heldItemActionParam < 0) {
        func_80123C58(this);
    }
    Animation_Reverse(&this->unk_284);
    func_80830B38(this);
}

void func_80830D40(PlayState* play, Player* this);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80830D40.s")

s32 func_80830DF0(Player* this, PlayState* play) {
    if (this->stateFlags3 & 0x40000000) {
        func_808302CC(this, play);
    } else {
        return false;
    }

    return true;
}

#ifdef NON_MATCHING
// swapped instructions
s32 func_80830E30(Player* this, PlayState* play) {
    LinkAnimationHeader* var_a2;

    if ((this->itemActionParam == 0x11) || (this->transformation == 2)) {
        func_8082F43C(play, this, func_8084923C);

        if (this->meleeWeaponAnimation == 0x1B) {
            var_a2 = &gameplay_keep_Linkanim_00E3A8;
        } else {
            LinkAnimationHeader* var_v0;

            if (this->meleeWeaponAnimation == 0x1C) {
                var_v0 = &gameplay_keep_Linkanim_00E3B0;
            } else {
                var_v0 = &gameplay_keep_Linkanim_00E3B8;
            }
            var_a2 = var_v0;
        }

        LinkAnimation_PlayOnce(play, &this->unk_284, var_a2);
        this->unk_ACC = 0xA;
    } else {
        if (!func_808306F8(this, play)) {
            return false;
        }

        if (Player_IsHoldingHookshot(this)) {
            var_a2 = &gameplay_keep_Linkanim_00D9D8;
        } else {
            LinkAnimationHeader* var_v0_2;

            if (this->transformation == 3) {
                var_v0_2 = &gameplay_keep_Linkanim_00E2F8;
            } else {
                var_v0_2 = &gameplay_keep_Linkanim_00D4F0;
            }
            var_a2 = var_v0_2;
        }

        LinkAnimation_PlayOnce(play, &this->unk_284, var_a2);
    }

    if (this->stateFlags1 & 0x800000) {
        func_8082DB3C(play, this, &gameplay_keep_Linkanim_00E050);
    } else if ((this->actor.bgCheckFlags & 1) && (this->transformation != 2)) {
        func_8082DB3C(play, this, func_8082ED20(this));
    }

    return true;
}
#else
s32 func_80830E30(Player* this, PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80830E30.s")
#endif

s32 func_80830F9C(PlayState* play) {
    return play->unk_1887C > 0 && CHECK_BTN_ALL(D_80862B44->press.button, BTN_B);
}

s32 func_80830FD4(PlayState* play) {
    return play->unk_1887C != 0 &&
           (play->unk_1887C < 0 ||
            CHECK_BTN_ANY(D_80862B44->cur.button, BTN_CRIGHT | BTN_CLEFT | BTN_CDOWN | BTN_CUP | BTN_B | BTN_A));
}

s32 func_80831010(Player* this, PlayState* play) {
    if ((this->unk_AA5 == 0) || (this->unk_AA5 == 3)) {
        if (func_8082FBE8(this) || (this->unk_730 != NULL) ||
            (func_800DF86C(Play_GetCamera(play, CAM_ID_MAIN), 0xD) == 0)) {
            return true;
        }
        this->unk_AA5 = 3;
    }
    return false;
}

s32 func_80831094(Player* this, PlayState* play) {
    if ((this->doorType == PLAYER_DOORTYPE_0) && !(this->stateFlags1 & PLAYER_STATE1_2000000)) {
        if ((D_80862B48 != 0) || func_80830F9C(play)) {
            if (func_80830E30(this, play)) {
                return func_80831010(this, play);
            }
        }
    }
    return false;
}

s32 func_80831124(PlayState* play, Player* this) {
    if (this->actor.child != NULL) {
        if (this->heldActor == NULL) {
            this->heldActor = this->actor.child;
            Player_RequestRumble(play, this, 255, 10, 250, SQ(0));
            func_800B8E58(this, NA_SE_IT_HOOKSHOT_RECEIVE);
        }
        return true;
    }
    return false;
}

s32 func_80831194(PlayState* play, Player* this) {
    if (this->heldActor != NULL) {
        if (!Player_IsHoldingHookshot(this)) {
            ItemID item;
            s32 sp30;

            func_808305BC(play, this, &item, &sp30);
            if ((this->transformation != PLAYER_FORM_DEKU) && !(this->stateFlags3 & PLAYER_STATE3_400)) {
                if (gSaveContext.minigameState == 1) {
                    if ((play->sceneNum != SCENE_SYATEKI_MIZU) && (play->sceneNum != SCENE_F01) &&
                        (play->sceneNum != SCENE_SYATEKI_MORI)) {
                        play->interfaceCtx.hbaAmmo -= 1;
                    }
                } else if (play->unk_1887C != 0) {
                    play->unk_1887C--;
                } else {
                    Inventory_ChangeAmmo(item, -1);
                }
            }

            if (play->unk_1887C == 1) {
                play->unk_1887C = -0xA;
            }

            Player_RequestRumble(play, this, 150, 10, 150, SQ(0));
        } else {
            Player_RequestRumble(play, this, 255, 20, 150, SQ(0));
            this->unk_B48 = 0.0f;
        }

        this->unk_D57 = (this->transformation == PLAYER_FORM_DEKU) ? 0x14 : 4;

        this->heldActor->parent = NULL;
        this->actor.child = NULL;
        this->heldActor = NULL;
        return 1;
    }

    return 0;
}

void func_8083133C(Player* this) {
    this->stateFlags1 |= PLAYER_STATE1_20000;

    if (!(this->skelAnime.moveFlags & 0x80) && (this->actor.bgCheckFlags & 0x200) && (D_80862B20 < 0x2000)) {
        this->currentYaw = this->actor.shape.rot.y = this->actor.wallYaw + 0x8000;
    }

    this->targetYaw = this->actor.shape.rot.y;
}

s32 func_808313A8(PlayState* play, Player* this, Actor* actor) {
    if (actor == NULL) {
        func_8082DE50(play, this);
        func_80836988(this, play);
        return true;
    }

    return false;
}

void func_808313F0(Player* this, PlayState* play) {
    if (!func_808313A8(play, this, this->heldActor)) {
        func_8082F43C(play, this, func_808490B4);
        LinkAnimation_PlayLoop(play, &this->unk_284, &gameplay_keep_Linkanim_00DB30);
    }
}

void func_80831454(Player* this) {
    if ((this->stateFlags3 & PLAYER_STATE3_20000000) || (this->stateFlags2 & PLAYER_STATE2_2000000)) {
        Audio_QueueSeqCmd(0x110000FF);
    }
}

#if 0
s32 func_80831494(PlayState* play, Player* this, void (*arg2)(Player*, PlayState*), s32 arg3) {
    if (arg2 == this->unk_748) {
        return 0;
    }

    play->actorCtx.unk5 &= ~4;

    if (this->actor.flags & ACTOR_FLAG_20000000) {
        AudioOcarina_SetInstrumentId(0);
        this->actor.flags &= ~ACTOR_FLAG_20000000;
    } else if ((func_80857BE8 == this->unk_748) || (func_808561B0 == this->unk_748)) {
        this->actor.shape.shadowDraw = ActorShadow_DrawFeet;
        this->unk_ABC = 0.0f;
        this->actor.shape.shadowScale = this->ageProperties->unk_04;
        if (func_80857BE8 == this->unk_748) {
            if (this->stateFlags3 & PLAYER_STATE3_80000) {
                func_80115D5C(&play->state);
            }
            func_8082DD2C(play, this);
            this->actor.shape.rot.x = 0;
            this->actor.shape.rot.z = 0;
            this->actor.bgCheckFlags &= ~0x0800;
        } else {
            Actor_SetScale(&this->actor, 0.01f);
        }
    } else if ((this->transformation == PLAYER_FORM_GORON) && (Player_GetMeleeWeaponHeld(this) != 0)) {
        func_80831990(play, this, ITEM_NONE);
    }

    func_800AEF44(Effect_GetByIndex(this->meleeWeaponEffectIndex[2]));
    this->unk_748 = arg2;

    if ((this->itemActionParam != this->heldItemActionParam) && (!(arg3 & 1) || !(this->stateFlags1 & PLAYER_STATE1_400000))) {
        func_80123C58(this);
    }

    if (!(arg3 & 1) && !(this->stateFlags1 & PLAYER_STATE1_800)) {
        func_808309CC(play, this);
        LinkAnimation_PlayLoop(play, &this->unk_284, func_8082ED20(this));
        this->stateFlags1 &= ~ PLAYER_STATE1_400000;
    }

    func_80831454(this);
    func_8082E794(this);

    this->stateFlags1 &= 0x4BFFFFBF;
    this->stateFlags2 &= 0xE577FFFF;
    this->stateFlags3 &= 0xDED05D75;

    this->unk_B08[7] = this->unk_B08[6] = this->unk_B08[5] = this->unk_B08[4] = this->unk_B08[3] = this->unk_B08[2] = 0.0f;

    this->unk_AE7 = 0;
    this->unk_AE8 = 0;
    this->unk_AA4 = 0;
    this->unk_B86 = 0;
    this->unk_B88 = 0;
    this->unk_B8A = 0;
    this->unk_B8C = 0;
    this->unk_B8E = 0;

    this->actor.shape.rot.z = 0;

    func_8082F09C(this);
    func_8082E00C(this);

    return 1;
}
#else
s32 func_80831494(PlayState* play, Player* this, void (*arg2)(Player*, PlayState* play), s32 arg3);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80831494.s")
#endif

void func_8083172C(PlayState* play, Player* this, void (*arg2)(Player*, PlayState*), s32 arg3) {
    s32 moveFlags = this->skelAnime.moveFlags;

    this->skelAnime.moveFlags = 0;
    func_80831494(play, this, arg2, arg3);
    this->skelAnime.moveFlags = moveFlags;
}

void func_80831760(PlayState* play, Player* this, void (*arg2)(Player*, PlayState*), s32 arg3) {
    if (this->heldItemActionParam >= 0) {
        s32 heldItemActionParam;

        heldItemActionParam = this->heldItemActionParam;
        this->heldItemActionParam = this->itemActionParam;
        func_80831494(play, this, arg2, arg3);
        this->heldItemActionParam = heldItemActionParam;

        Player_SetModels(this, Player_ActionToModelGroup(this, this->heldItemActionParam));
    }
}

void func_808317C4(Player* this) {
    if (Player_IsHoldingHookshot(this)) {
        if (this->heldActor != NULL) {
            Actor_MarkForDeath(this->heldActor);
            this->actor.child = NULL;
            this->heldActor = NULL;
        }
    }
}

s32 func_80831814(Player* this, PlayState* play, s32 arg2) {
    if (!(this->stateFlags1 & (PLAYER_STATE1_4 | PLAYER_STATE1_800 | PLAYER_STATE1_2000 | PLAYER_STATE1_4000))) {
        if (func_800DF86C(Play_GetCamera(play, CAM_ID_MAIN), 6) != 0) {
            if ((this->actor.bgCheckFlags & 1) ||
                (func_801242B4(this) && (this->actor.depthInWater < this->ageProperties->unk_2C))) {
                this->unk_AA5 = arg2;
                return true;
            }
        }
    }
    return false;
}

void func_808318C0(PlayState* play) {
    if (func_80115DB4(play, 0, 3) != 0) {
        if (play->actorCtx.unk3 != 0) {
            func_800B90F4(play);
        } else {
            play->actorCtx.unk3 = 1;
        }

        play_sound(play->actorCtx.unk3 != 0 ? NA_SE_SY_GLASSMODE_ON : NA_SE_SY_GLASSMODE_OFF);
    } else {
        play_sound(NA_SE_SY_ERROR);
    }
}

void func_80831944(PlayState* play, Player* this) {
    if (func_8012364C(play, this, func_8082FDC4()) == ITEM_LENS) {
        func_808318C0(play);
    }
}

#if 0
? func_8082E1F0(Player*, ?, s32, PlayState*);       /* extern */
? func_8082F470(PlayState*, Player*, s32);          /* extern */
s32 func_8082F524(Player*, s32);                    /* extern */
? func_808317C4(Player*, s32, PlayerModelIndices*); /* extern */
s32 func_80831814(Player*, PlayState*, ?, PlayState*); /* extern */
? func_808318C0(PlayState*);                        /* extern */
? func_8083A658(PlayState*, Player*, s32, PlayState*); /* extern */
extern ? D_8085CD00;
extern ? D_8085CD24;

void func_80831990(PlayState* play, Player* arg1, s32 arg2) {
    CollisionPoly* sp5C;
    s32 sp58;
    f32 sp54;
    s32 sp40;
    s32 sp3C;
    s32 temp_s1;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 var_a2;
    s32 var_v0;
    s32 var_v1;
    s32 var_v1_2;
    s8 temp_v1;
    u8 temp_v0_3;
    u8 temp_v0_4;
    u8 temp_v0_5;
    u8 temp_v1_2;

    temp_v0 = func_8082F524(arg1, arg2);
    temp_s1 = temp_v0;
    if (arg1->heldItemActionParam == arg1->itemActionParam) {
        if ((arg1->stateFlags1 & 0x400000) && (Player_ActionToMeleeWeapon(temp_v0) == 0) && (temp_s1 != PLAYER_AP_NONE)) {
            goto block_5;
        }
        goto block_8;
    }
block_5:
    if ((arg1->heldItemActionParam < 0) && ((Player_ActionToMeleeWeapon(temp_s1) != 0) || (temp_s1 == PLAYER_AP_NONE))) {
block_8:
        var_a2 = temp_s1 < 0x3A;
        if (temp_s1 != PLAYER_AP_NONE) {
            if ((arg1->stateFlags1 * 0x10) >= 0) {
                var_a2 ^= 1;
                goto block_18;
            }
            if (temp_s1 == PLAYER_AP_MASK_ZORA) {
                var_a2 ^= 1;
                goto block_18;
            }
            if (arg1->currentBoots >= 5) {
                var_a2 ^= 1;
                if (arg1->actor.bgCheckFlags & 1) {
                    goto block_17;
                }
            }
        } else {
block_17:
block_18:
            var_v1 = var_a2;
            if (var_a2 != 0) {
                var_v1 = temp_s1 < 0x52;
                if (var_v1 != 0) {
                    var_v1 = arg1->transformation != 4;
                    if (var_v1 == 0) {
                        var_v1 = (temp_s1 < 0x4D) ^ 1;
                    }
                }
            }
            if ((var_v1 != 0) || (((arg1->actor.flags & 0x100) == 0x100) && (temp_s1 != PLAYER_AP_NONE)) || (temp_s1 == PLAYER_AP_OCARINA) || ((temp_s1 >= 0x16) && (temp_s1 < 0x3A)) || ((temp_s1 == PLAYER_AP_PICTO_BOX) && (arg1->targetActor != NULL) && (arg1->exchangeItemId > 0))) {
                if (var_v1 != 0) {
                    var_v0 = temp_s1 - 0x4E;
                    if (temp_s1 < 0x4E) {
                        var_v0 = 4;
                    }
                    if (((arg1->currentMask != 0x14) && (temp_s1 == PLAYER_AP_MASK_GIANT) && ((gSaveContext.unk_3F28 != 0) || (gSaveContext.save.playerData.magic == 0))) || (((arg1->stateFlags1 * 0x10) >= 0) && (BgCheck_EntityCheckCeiling(&arg0->colCtx, &sp54, &arg1->actor.world.pos, D_8085BA38[var_v0].unk_00, &sp5C, &sp58, &arg1->actor) != 0))) {
                        play_sound(0x4806U);
                        return;
                    }
                    goto block_41;
                }
block_41:
                if ((temp_s1 == PLAYER_AP_MAGIC_BEANS) && (gSaveContext.save.inventory.ammo[gItemSlots[0xA]] == 0)) {
                    play_sound(0x4806U);
                    return;
                }
                arg1->heldItemActionParam = (s8) temp_s1;
                arg1->unk_AA5 = 5;
                return;
            }
            if (((temp_s1 == PLAYER_AP_STICK) && (gSaveContext.save.inventory.ammo[gItemSlots[8]] == 0)) || (((temp_v1 = arg0->unk_1887D, (temp_v1 != 0)) || (arg0->unk_1887E != 0)) && (arg0->actorCtx.actorLists[3].length >= 5)) || ((temp_v1 == 0) && (arg0->unk_1887E == 0) && (sp3C = var_a2, temp_v0_2 = Player_ActionToExplosive(arg1, temp_s1), (temp_v0_2 >= 0)) && ((gSaveContext.save.inventory.ammo[gItemSlots[*(&D_8085CD24 + (temp_v0_2 * 4))]] == 0) || (arg0->actorCtx.actorLists[3].length >= 3)))) {
                play_sound(0x4806U);
                return;
            }
            if (temp_s1 == PLAYER_AP_LENS) {
                func_808318C0(arg0);
                return;
            }
            if (temp_s1 == PLAYER_AP_PICTO_BOX) {
                if (func_80831814(arg1, arg0, 2, arg0) == 0) {
                    play_sound(0x4806U);
                }
            } else {
                temp_v0_3 = arg1->transformation;
                if ((temp_s1 == PLAYER_AP_NUT) && ((temp_v0_3 != 3) || (arg1->heldItemButton != 0))) {
                    if (gSaveContext.save.inventory.ammo[gItemSlots[9]] != 0) {
                        func_8083A658(arg0, arg1, var_a2, arg0);
                        return;
                    }
                    play_sound(0x4806U);
                    return;
                }
                if ((temp_v0_3 == 4) && (var_a2 != 0) && (temp_s1 < 0x4D)) {
                    temp_v0_4 = arg1->currentMask;
                    temp_v1_2 = temp_s1 - 0x39;
                    arg1->prevMask = temp_v0_4;
                    if (temp_v1_2 == temp_v0_4) {
                        arg1->currentMask = 0;
                        func_8082E1F0(arg1, 0x834, var_a2, arg0);
                    } else {
                        arg1->currentMask = temp_v1_2;
                        func_8082E1F0(arg1, 0x835, var_a2, arg0);
                    }
                    gSaveContext.save.equippedMask = arg1->currentMask;
                    return;
                }
                if ((temp_s1 != arg1->itemActionParam) || ((arg1->heldActor == NULL) && (Player_ActionToExplosive(arg1, temp_s1) >= 0))) {
                    temp_v0_5 = Player_ActionToModelGroup(arg1, temp_s1);
                    arg1->nextModelGroup = temp_v0_5;
                    var_v1_2 = arg1->transformation != 1;
                    if (var_v1_2 == 0) {
                        var_v1_2 = temp_s1 == PLAYER_AP_POWDER_KEG;
                    }
                    if ((var_v1_2 != 0) && (arg1->itemActionParam >= 0) && (arg2 != arg1->heldItemId) && (*(&D_8085CD00 + ((gPlayerModelTypes[arg1->modelGroup].modelAnimType * 6) + gPlayerModelTypes[temp_v0_5 & 0xFF].modelAnimType)) != 0)) {
                        arg1->heldItemId = (u8) arg2;
                        arg1->stateFlags3 |= 0x40000000;
                        return;
                    }
                    sp40 = var_v1_2;
                    func_808317C4(arg1, 5, gPlayerModelTypes);
                    func_8082DCA0(arg0, arg1);
                    func_8082F470(arg0, arg1, temp_s1);
                    if (var_v1_2 == 0) {
                        D_80862B48 = 1;
                        D_80862B4C = 1;
                    }
                } else {
                    D_80862B48 = 1;
                    D_80862B4C = 1;
                }
            }
        }
    }
}

#else
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80831990.s")
#endif

void func_80851BD4(Player* this, PlayState* play);
void func_8084BFDC(Player* this, PlayState* play);

void func_80831F34(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    s32 sp24;

    sp24 = func_801242B4(this);
    func_8082DE50(play, this);
    func_80831494(play, this, sp24 ? func_80851BD4 : func_8084BFDC, 0);
    func_8082DB18(play, this, anim);

    if (anim == &gameplay_keep_Linkanim_00D698) {
        this->skelAnime.endFrame = 84.0f;
    }

    this->stateFlags1 |= PLAYER_STATE1_80;
    func_8082DAD4(this);
    func_8082DF8C(this, NA_SE_VO_LI_DOWN);
    if (this == GET_PLAYER(play)) {
        this->unk_A86 = play->playerActorCsIds[6];
        func_801A0184();
        gSaveContext.powderKegTimer = 0;
        gSaveContext.unk_1014 = 0;
        gSaveContext.jinxTimer = 0;

        if (Inventory_ConsumeFairy(play) != 0) {
            play->gameOverCtx.state = 0x14;
            this->unk_AE7 = 1;
        } else {
            play->gameOverCtx.state = 1;
            func_801A41F8(0);
            func_801A3098(0x20);
            gSaveContext.seqIndex = 0xFF;
            gSaveContext.nightSeqIndex = 0xFF;
        }

        ShrinkWindow_SetLetterboxTarget(0x20);
    }
}

void func_8084D770(Player* this, PlayState* play);

s32 func_80832090(Player* this) {
    return (((func_8084D770 == this->unk_748) == 0) ||
            (((this->stateFlags3 & PLAYER_STATE3_40000000)) &&
             ((this->heldItemId == ITEM_FC) || (this->heldItemId == ITEM_NONE)))) &&
           (((func_80848808 == this->unk_AC4) == 0) || func_8082F524(this, this->heldItemId) == this->itemActionParam);
}

void func_8084AC84(Player* this, PlayState* play);
void func_8084AEEC(Player* this, PlayState* play);

s32 func_8083213C(Player* this) {
    return func_8084AC84 == this->unk_748 || func_8084AEEC == this->unk_748;
}

s32 func_8083216C(Player* this, PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8083216C.s")

s32 func_808323C0(Player* this, s16 csIndex) {
    if ((csIndex >= 0) && (ActorCutscene_GetCurrentIndex() != csIndex)) {
        if (!ActorCutscene_GetCanPlayNext(csIndex)) {
            ActorCutscene_SetIntentToPlay(csIndex);

            return false;
        }
        ActorCutscene_Start(csIndex, &this->actor);
    }
    this->unk_A86 = csIndex;

    return true;
}

s32 func_80832444(Player* this);
s32 func_8083249C(Player* this);
s32 func_808324EC(PlayState* play, Player* this, void (*arg2)(PlayState*, Player*), s32 arg3);
void func_80832578(Player* this, PlayState* play);

s32 func_80832444(Player* this) {
    if (this->unk_A86 >= 0) {
        if (ActorCutscene_GetCanPlayNext(this->unk_A86) == 0) {
            ActorCutscene_SetIntentToPlay(this->unk_A86);
            return false;
        }

        ActorCutscene_Start(this->unk_A86, &this->actor);
    }
    return true;
}

s32 func_8083249C(Player* this) {
    if ((this->unk_A86 >= 0) && (ActorCutscene_GetCurrentIndex() != this->unk_A86)) {
        return func_80832444(this);
    }
    return true;
}

s32 func_808324EC(PlayState* play, Player* this, void (*arg2)(PlayState*, Player*), s32 arg3) {
    this->unk_D58 = arg2;
    this->unk_A86 = arg3;
    func_80831494(play, this, func_8084D770, 0);
    func_8083249C(this);
    this->stateFlags2 |= PLAYER_STATE2_40;
    return func_8082DE14(play, this);
}

s32 func_80832558(PlayState* play, Player* this, void (*arg2)(PlayState*, Player*)) {
    return func_808324EC(play, this, arg2, -1);
}

#if 0
void func_80832578(Player* this, PlayState* play) {
    s16 sp26;

    sp26 = this->actor.shape.rot.y;
    if (!(this->stateFlags2 & 0x60)) {
        if ((this->unk_730 != NULL) && ((play->actorCtx.targetContext.unk4B != 0) || (this != GET_PLAYER(play))) && (this->unk_730->id != 0x233)) {
            Math_ScaledStepToS(&this->actor.shape.rot.y, Math_Vec3f_Yaw(&this->actor.world.pos, &this->unk_730->focus.pos), 0xFA0);
        } else if ((this->stateFlags1 & 0x20000) && !(this->stateFlags2 & 0x60)) {
            Math_ScaledStepToS(&this->actor.shape.rot.y, this->targetYaw, 0xFA0);
        }
    } else if (!(this->stateFlags2 & 0x40)) {
        Math_ScaledStepToS(&this->actor.shape.rot.y, this->currentYaw, 0x7D0);
    }
    this->unk_B4C = this->actor.shape.rot.y - sp26;
}
#else
void func_80832578(Player* this, PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80832578.s")
#endif

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80832660.s")

s16 func_80832754(Player* this, s32 arg1);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80832754.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80832888.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80832CAC.s")

s32 func_80832F24(Player* this) {
    return Math_StepToF(&this->linearVelocity, 0.0f, REG(43) / 100.0f);
}

s32 func_80832F78(Player* this, f32* arg1, s16* arg2, f32 arg3, PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80832F78.s")

s32 func_80833058(PlayState* play, Player* this, s8* arg2, UNK_TYPE arg3);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80833058.s")

s32 func_808331FC(PlayState* arg0, Player* arg1, SkelAnime* arg2, f32 arg3);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_808331FC.s")

void func_808332A0(PlayState* play, Player* this, s32 arg2, s32 arg3) {
    if (arg2 != 0) {
        this->unk_B08[0] = 0.0f;
    } else {
        this->unk_B08[0] = 0.5f;
    }

    this->stateFlags1 |= PLAYER_STATE1_1000;
    if ((this->actor.id == ACTOR_PLAYER) && ((arg3 != 0) || (this->transformation == PLAYER_FORM_HUMAN))) {
        s16 var_v1;
        Actor* actor;

        var_v1 = 0;
        if (arg3 != 0) {
            if (this->unk_730 != NULL) {
                var_v1 = Math_Vec3f_Pitch(this->bodyPartsPos, &this->unk_730->focus.pos);
            }
            if (gSaveContext.save.playerData.magic == 0) {
                return;
            }
        }

        actor = Actor_Spawn(&play->actorCtx, play, ACTOR_EN_M_THUNDER, this->bodyPartsPos[0].x, this->bodyPartsPos[0].y,
                            this->bodyPartsPos[0].z, var_v1, 0, 0, (this->itemActionParam - 3) | arg2);

        if ((actor != NULL) && (arg3 != 0)) {
            func_80115DB4(play, 1, 7);
            this->unk_D57 = 4;
        }
    }
}

s32 func_808333CC(Player* this);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_808333CC.s")

extern LinkAnimationHeader* D_8085CF50[2];
extern LinkAnimationHeader* D_8085CF58[2];
extern LinkAnimationHeader* D_8085CF60[2];
extern LinkAnimationHeader* D_8085CF68[2];
extern LinkAnimationHeader* D_8085CF70[2];
extern LinkAnimationHeader* D_8085CF78[2];

void func_8084CCEC(Player* this, PlayState* play);

void func_808334D4(PlayState* play, Player* this) {
    LinkAnimationHeader* anim;

    if ((this->meleeWeaponAnimation >= PLAYER_MWA_RIGHT_SLASH_1H) &&
        (this->meleeWeaponAnimation <= PLAYER_MWA_RIGHT_COMBO_2H)) {
        anim = D_8085CF58[Player_IsHoldingTwoHandedWeapon(this)];
    } else {
        anim = D_8085CF50[Player_IsHoldingTwoHandedWeapon(this)];
    }

    func_8082DC38(this);
    LinkAnimation_Change(play, &this->skelAnime, anim, 1.0f, 8.0f, Animation_GetLastFrame(anim), 2, -9.0f);
    func_808332A0(play, this, 0x200, 0);
}

void func_808335B0(PlayState* play, Player* this) {
    func_80831494(play, this, func_8084CCEC, 1);
    func_808334D4(play, this);
}

PlayerMeleeWeaponAnimation func_808335F4(Player* this);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_808335F4.s")

void func_80833728(Player* this, s32 arg1, s32 arg2, s32 arg3);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80833728.s")

void func_8083375C(Player* this, PlayerMeleeWeaponAnimation meleeWeaponAnimation);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8083375C.s")

void func_80833864(PlayState* play, Player* this, PlayerMeleeWeaponAnimation meleeWeaponAnimation);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80833864.s")

void func_80833998(Player* this, s32 invincibilityTimer) {
    if (this->invincibilityTimer >= 0) {
        this->invincibilityTimer = invincibilityTimer;
        this->unk_B5F = 0;
    }
}

void func_808339B4(Player* this, s32 invincibilityTimer) {
    if (invincibilityTimer < this->invincibilityTimer) {
        this->invincibilityTimer = invincibilityTimer;
    }
    this->unk_B5F = 0;
}

UNK_TYPE func_808339D4(PlayState* play, Player* this, UNK_TYPE arg2);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_808339D4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80833A64.s")

void func_80833AA0(Player* this, PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80833AA0.s")

void func_80833B18(PlayState* play, Player* this, s32 arg2, f32 arg3, f32 arg4, s16 arg5, s32 arg6);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80833B18.s")

s32 func_808340AC(s32 arg0) {
    s32 temp_v0 = arg0 - 2;

    if ((temp_v0 >= 0) && (temp_v0 <= 1)) {
        return temp_v0;
    }
    return -1;
}

s32 func_808340D4(s32 arg0) {
    return (arg0 == 4) || (arg0 == 7) || (arg0 == 12);
}

void func_8085421C(Player* this, PlayState* play);

void func_80834104(PlayState* play, Player* this) {
    func_80831494(play, this, func_8085421C, 0);
    this->stateFlags1 |= PLAYER_STATE1_20000000 | PLAYER_STATE1_80000000;
}

void func_80834140(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    if (!(this->stateFlags1 & PLAYER_STATE1_80)) {
        func_80834104(play, this);
        if (func_8082DA90(play)) {
            this->unk_AE8 = -30;
        }
        this->stateFlags1 |= PLAYER_STATE1_80;
        LinkAnimation_Change(play, &this->skelAnime, anim, 1.0f, 0.0f, 84.0f, 2, -6.0f);
        this->unk_AE7 = 1;
        this->linearVelocity = 0.0f;
    }
}

s32 func_808341F4(PlayState* play, Player* this);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_808341F4.s")

s32 func_808344C0(PlayState* play, Player* this) {
    s32 i = 0;

    while (i < ARRAY_COUNT(this->flameTimers)) {
        this->flameTimers[i] = Rand_S16Offset(0, 200);
        i++;
    }

    this->isBurning = true;
    return func_808341F4(play, this);
}

s32 func_80834534(PlayState* play, Player* this) {
    func_8082DF8C(this, NA_SE_VO_LI_FALL_L);
    return func_808344C0(play, this);
}

s32 func_8083456C(PlayState* play, Player* this) {
    if (this->actor.colChkInfo.acHitEffect == 1) {
        return func_80834534(play, this);
    }
    return 0;
}

void func_808345A8(Player* this) {
    if ((this->invincibilityTimer > 0) && (this->invincibilityTimer < 20)) {
        this->invincibilityTimer = 20;
    }
}

void func_808345C8(void) {
    if (INV_CONTENT(ITEM_MASK_DEKU) == ITEM_MASK_DEKU) {
        gSaveContext.save.playerForm = PLAYER_FORM_HUMAN;
        gSaveContext.save.equippedMask = PLAYER_MASK_NONE;
    }
}

s32 func_80834600(Player* this, PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80834600.s")

void func_80834CD0(Player* this, f32 arg1, u16 sfxId) {
    this->actor.velocity.y = arg1 * D_8085C3E4;
    this->actor.bgCheckFlags &= ~1;

    if (sfxId != 0) {
        func_8082E188(this);
        func_8082DF8C(this, sfxId);
    }

    this->stateFlags1 |= PLAYER_STATE1_40000;
    this->unk_B68 = this->actor.world.pos.y;
}

void func_80834D50(PlayState* play, Player* this, LinkAnimationHeader* anim, f32 arg3, u16 sfxId) {
    func_80831494(play, this, func_8084C16C, 1);
    if (anim != NULL) {
        func_8082DB90(play, this, anim);
    }
    func_80834CD0(this, arg3, sfxId);
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80834DB8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80834DFC.s")

void func_80835324(PlayState* play, Player* this, f32 arg2, s16 arg3) {
    func_80831494(play, this, func_8084D820, 0);
    func_8082DD2C(play, this);
    this->unk_A86 = -1;
    this->unk_AE7 = 1;
    this->unk_AE8 = 1;
    this->unk_3A0.x = Math_SinS(arg3) * arg2 + this->actor.world.pos.x;
    this->unk_3A0.z = Math_CosS(arg3) * arg2 + this->actor.world.pos.z;
    func_8082DB18(play, this, func_8082ED20(this));
}

void func_808353DC(PlayState* arg0, Player* arg1) {
    func_80831494(arg0, arg1, func_808508C8, 0);
    func_8082E634(arg0, arg1, &gameplay_keep_Linkanim_00E000);
}

s32 func_80835428(PlayState* play, Player* this) {
    if (!func_8082DA90(play) && (this->stateFlags1 & PLAYER_STATE1_80000000)) {
        func_80834104(play, this);
        func_8082DB3C(play, this, &gameplay_keep_Linkanim_00DD30);
        func_8082DF8C(this, NA_SE_VO_LI_FALL_S);
        func_8019F128(NA_SE_OC_SECRET_WARP_IN);
        return true;
    }
    return false;
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_808354A4.s")

void func_808355D8(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    func_80833AA0(this, play);
    this->unk_AE8 = -2;
    func_8082E5EC(play, this, anim);
    func_8082E1F0(this, NA_SE_IT_DEKUNUTS_FLOWER_CLOSE);
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8083562C.s")

void func_80835BC8(Player* this, Vec3f* translation, Vec3f* src, Vec3f* dst) {
    Lib_Vec3f_TranslateAndRotateY(translation, this->actor.shape.rot.y, src, dst);
}

void func_80835BF8(Vec3f* arg0, s16 arg1, f32 arg2, Vec3f* arg3) {
    arg3->x = Math_SinS(arg1) * arg2 + arg0->x;
    arg3->z = Math_CosS(arg1) * arg2 + arg0->z;
}

void func_80835C64(PlayState* play, Player* this, Vec3f* translation, Vec3f* arg3, s32 elfParams) {
    Vec3f pos;

    func_80835BC8(this, translation, arg3, &pos);
    Actor_Spawn(&play->actorCtx, play, ACTOR_EN_ELF, pos.x, pos.y, pos.z, 0, 0, 0, elfParams);
}

f32 func_80835CD8(PlayState* play, Player* this, Vec3f* arg2, Vec3f* pos, CollisionPoly** outPoly, s32* outBgId) {
    func_80835BC8(this, &this->actor.world.pos, arg2, pos);
    return BgCheck_EntityRaycastFloor5(&play->colCtx, outPoly, outBgId, &this->actor, pos);
}

f32 func_80835D2C(PlayState* play, Player* this, Vec3f* arg2, Vec3f* pos) {
    CollisionPoly* poly;
    s32 bgId;

    return func_80835CD8(play, this, arg2, pos, &poly, &bgId);
}

s32 func_80835D58(PlayState* play, Player* this, Vec3f* arg2, CollisionPoly** outPoly, s32* bgId, Vec3f* posResult) {
    Vec3f posA;
    Vec3f posB;

    posA.x = this->actor.world.pos.x;
    posA.y = this->actor.world.pos.y + arg2->y;
    posA.z = this->actor.world.pos.z;
    func_80835BC8(this, &this->actor.world.pos, arg2, &posB);

    return BgCheck_EntityLineTest2(&play->colCtx, &posA, &posB, posResult, outPoly, 1, 0, 0, 1, bgId, &this->actor);
}

extern Vec3f D_8085D100;

s32 func_80835DF8(PlayState* play, Player* this, CollisionPoly** outPoly, s32* outBgId) {
    Vec3f pos;
    f32 yIntersect = func_80835CD8(play, this, &D_8085D100, &pos, outPoly, outBgId);

    if ((*outBgId == BGCHECK_SCENE) && (fabsf(this->actor.world.pos.y - yIntersect) < 10.0f)) {
        func_800FAAB4(play, SurfaceType_GetLightSettingIndex(&play->colCtx, *outPoly, *outBgId));
        return true;
    }
    return false;
}

extern Vec3f D_8085D10C;
extern f32 D_8085D114;
#if 0
void func_80835EAC(PlayState* play, Player* this, DoorSpiral* door) {
    this->currentYaw = door->actor.home.rot.y + 0x8000;
    this->actor.shape.rot.y = this->currentYaw;

    if (this->linearVelocity <= 0.0f) {
        this->linearVelocity = 0.1f;
    }

    func_80835324(play, this, 50.0f, this->actor.shape.rot.y);
    this->unk_AE7 = 0;
    this->stateFlags1 |= 0x20000000;
    this->unk_397 = this->doorType;
    func_80835BF8(&door->actor.world.pos, door->actor.shape.rot.y, -140.0f, &this->unk_3A0);

    if (this->doorDirection != 0) {
        D_8085D10C.x = -400.0f;
    } else {
        D_8085D10C.x = 400.0f;
    }
    D_8085D114 = 200.0f;

    func_80835BC8(this, &this->unk_3A0, &D_8085D10C, &this->unk_3AC);
    door->shouldClimb = true;
    func_8082DAD4(this);

    if (this->doorTimer != 0) {
        this->unk_AE8 = 0;
        func_8082E438(play, this, func_8082ED20(this));
        this->skelAnime.endFrame = 0.0f;
    } else {
        this->linearVelocity = 0.1f;
    }

    Camera_ChangeSetting(Play_GetCamera(play, 0), 0x50);
    this->unk_3BA = play->doorCtx.transitionActorList[(s32) door->actor.params >> 0xA].sides[0].bgCamDataId;
    func_800B90F4(play);
    this->unk_B72 = 2;
}
#else
// doorType == PLAYER_DOORTYPE_4
void func_80835EAC(PlayState* play, Player* this, DoorSpiral* door);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80835EAC.s")
#endif

// doorType == PLAYER_DOORTYPE_2
void func_8083604C(PlayState* play, Player* this, Actor* door) {
    s32 pad;
    Vec3f sp38;

    this->currentYaw = door->home.rot.y;
    if (this->doorDirection > 0) {
        this->currentYaw -= 0x8000;
    }
    this->actor.shape.rot.y = this->currentYaw;
    if (this->linearVelocity <= 0.0f) {
        this->linearVelocity = 0.1f;
    }

    func_80835324(play, this, 50.0f, this->actor.shape.rot.y);
    this->unk_AE7 = 0;
    this->unk_397 = this->doorType;
    this->stateFlags1 |= PLAYER_STATE1_20000000;
    Actor_OffsetOfPointInActorCoords(door, &sp38, &this->actor.world.pos);

    func_80835BF8(&this->actor.world.pos, door->shape.rot.y, (42.0f - fabsf(sp38.z)) * this->doorDirection,
                  &this->actor.world.pos);
    func_80835BF8(&this->actor.world.pos, door->shape.rot.y, this->doorDirection * 20.0f, &this->unk_3A0);
    func_80835BF8(&this->actor.world.pos, door->shape.rot.y, this->doorDirection * -120.0f, &this->unk_3AC);

    ((DoorShutter*)door)->unk_15C = 1;
    func_8082DAD4(this);

    if (this->doorTimer != 0) {
        this->unk_AE8 = 0;
        func_8082E438(play, this, func_8082ED20(this));
        this->skelAnime.endFrame = 0.0f;
    } else {
        this->linearVelocity = 0.1f;
    }

    if (door->category == ACTORCAT_DOOR) {
        this->unk_3BA = (s16)play->doorCtx.transitionActorList[DOORSHUTTER_GET_FC00(door)]
                            .sides[this->doorDirection > 0 ? 0 : 1]
                            .bgCamDataId;
        func_800B90F4(play);
    }
}

// doorType not PLAYER_DOORTYPE_2 neither PLAYER_DOORTYPE_4
void func_80836258(PlayState* play, Player* this, Actor* door);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80836258.s")

s32 func_80839518(Player* this, PlayState* play);
s32 func_808365DC(Player* this, PlayState* play);
s32 func_8083D23C(Player* this, PlayState* play);
s32 func_8083CCB4(Player* this, PlayState* play);
s32 func_808391D8(Player* this, PlayState* play);
s32 func_8083DFC4(Player* this, PlayState* play);
s32 func_8083A114(Player* this, PlayState* play);
s32 func_80848570(Player* this, PlayState* play);
s32 func_8083A580(Player* this, PlayState* play);
s32 func_8083D78C(Player* this, PlayState* play);
s32 func_80839B18(Player* this, PlayState* play);
s32 func_8083A274(Player* this, PlayState* play);
s32 func_80834DFC(Player* this, PlayState* play);
s32 func_80838A90(Player* this, PlayState* play);
s32 func_8083A0CC(Player* this, PlayState* play);

extern s32 (*D_8085D054[0xF])(Player* this, PlayState* play);
#if 0
s32 (*D_8085D054[])(Player*, PlayState*) = {
    func_80839518,
    func_808365DC,
    func_8083D23C,
    func_8083CCB4,
    func_808391D8,
    func_8083DFC4,
    func_8083A114,
    func_80848570,
    func_8083A580,
    func_8083D78C,
    func_80839B18,
    func_8083A274,
    func_80834DFC,
    func_80838A90,
    func_8083A0CC,
};
#endif

void func_8085437C(Player* this, PlayState* play);

#if 0
// door stuff
s32 func_808365DC(Player* this, PlayState* play) {
    Actor* temp_s0;
    Actor* var_v0_3;
    s32 var_v0_2;
    s8 temp_a2;
    s8 var_v0;
    u8 var_v1;

    if (gSaveContext.save.playerData.health != 0) {
        var_v0 = this->doorType;
        if (var_v0 != 0) {
            if (this->actor.category == 2) {
                if (var_v0 < 0) {
                    if (ActorCutscene_GetCanPlayNext(0x7C) == 0) {
                        var_v0 = this->doorType;
                        goto block_6;
                    }
                    goto block_8;
                }
            block_6:
                if ((var_v0 > 0) && (ActorCutscene_GetCanPlayNext(0x7D) != 0)) {
            block_8:
                    if (!(this->stateFlags1 & 0x800) && ((CHECK_BTN_ALL(D_80862B44->press.button, BTN_A)) || (this->unk_748 == func_8085437C) || (this->doorType == 4) || (this->doorType == 5))) {
                        goto block_13;
                    }
                }
                goto block_39;
            }

        block_13:
            temp_s0 = this->doorActor;
            if (this->doorType < 0) {
                func_8085B460(play, temp_s0);
                if (temp_s0->textId == 0x1821) {
                    temp_s0->flags |= 0x100;
                }
                return 1;
            }
            gSaveContext.respawn[0].data = 0;

            if (this->doorType == 4) {
                func_80835EAC(play, this, (DoorSpiral* ) temp_s0);
            } else if (this->doorType == 2) {
                func_8083604C(play, this, temp_s0);
            } else {
                func_80836258(play, this, temp_s0);
            }
            var_v1 = this->actor.category;
            if (var_v1 == 2) {
                this->unk_A86 = 0x7D;
                ActorCutscene_Start((s16) this->unk_A86, &this->actor);
                var_v1 = this->actor.category;
            }
            if (var_v1 == 2) {
                if ((this->doorType < 3) && (temp_s0->category == 0xA) && ((this->doorType != 1) || ((((s16) temp_s0->params >> 7) & 7) != 7))) {
                    if (this->doorDirection > 0) {
                        var_v0_2 = 0;
                    } else {
                        var_v0_2 = 1;
                    }
                    temp_a2 = play->doorCtx.transitionActorList[(s32) (temp_s0->params & 0xFFFF) >> 0xA].sides[var_v0_2].room;
                    if ((temp_a2 >= 0) && (temp_a2 != play->roomCtx.currRoom.num)) {
                        Room_StartRoomTransition(play, &play->roomCtx, (s32) temp_a2);
                    }
                }
            }
            var_v0_3 = temp_s0->child;
            temp_s0->room = play->roomCtx.currRoom.num;
            if ((var_v0_3 != NULL) || (var_v0_3 = temp_s0->parent, (var_v0_3 != NULL))) {
                var_v0_3->room = play->roomCtx.currRoom.num;
            }
            return 1;
        }
    }
block_39:
    return 0;
}
#else
s32 func_808365DC(Player* this, PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_808365DC.s")
#endif

void func_80849A9C(Player* this, PlayState* play);
void func_80849DD0(Player* this, PlayState* play);

void func_80836888(Player* this, PlayState* play) {
    LinkAnimationHeader* anim;

    func_80831494(play, this, func_80849A9C, 1);

    if (this->unk_B40 < 0.5f) {
        anim = func_8082EF54(this);
        this->unk_B40 = 0.0f;
    } else {
        anim = func_8082EF9C(this);
        this->unk_B40 = 1.0f;
    }

    this->unk_B44 = this->unk_B40;
    func_8082DB3C(play, this, anim);
    this->currentYaw = this->actor.shape.rot.y;
}

void func_8083692C(Player* this, PlayState* play) {
    func_80831494(play, this, func_80849DD0, 1);
    func_8082E438(play, this, func_8082ED20(this));
    this->currentYaw = this->actor.shape.rot.y;
}

void func_80836988(Player* this, PlayState* play) {
    if (func_80123420(this)) {
        func_80836888(this, play);
    } else if (func_80123434(this)) {
        func_8083692C(this, play);
    } else {
        func_8085B384(this, play);
    }
}

void func_80849FE0(Player* this, PlayState* play);

void func_808369F4(Player* this, PlayState* play) {
    void (*var_a2)(Player*, PlayState*);

    if (func_80123420(this)) {
        var_a2 = func_80849A9C;
    } else if (func_80123434(this)) {
        var_a2 = func_80849DD0;
    } else {
        var_a2 = func_80849FE0;
    }
    func_80831494(play, this, var_a2, 1);
}

void func_80836A5C(Player* this, PlayState* play) {
    func_808369F4(this, play);
    if (func_80123420(this)) {
        this->unk_AE8 = 1;
    }
}

void func_80836A98(Player* this, LinkAnimationHeader* anim, PlayState* play) {
    func_80836A5C(this, play);
    func_8082EC9C(play, this, anim);
}
/*
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80836AD8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80836B3C.s")
*/
void func_8084C6EC(Player* this, PlayState* play);

void func_80836AD8(PlayState* play, Player* this) {
    func_80831494(play, this, func_80857BE8, 0);
    this->unk_B28 = 0;
    this->unk_B88 = 0;
    this->unk_AF0[0].x = 0.0f;
    this->unk_AF0[0].y = 0.0f;
    this->unk_AF0[0].z = 0.0f;
    this->unk_B08[0] = 0.0f;
    this->unk_B08[1] = 0.0f;
    func_800B8E58(this, NA_SE_PL_GORON_TO_BALL);
}

void func_80836B3C(PlayState* play, Player* this, f32 arg2) {
    this->currentYaw = this->actor.shape.rot.y;
    this->actor.world.rot.y = this->actor.shape.rot.y;

    if (this->transformation == PLAYER_FORM_GORON) {
        func_80836AD8(play, this);
        LinkAnimation_Change(play, &this->skelAnime, GET_PLAYER_ANIM(PLAYER_ANIMGROUP_15, this->modelAnimType),
                             1.5f * D_8085C3E4, 0.0f, 6.0f, 2, 0.0f);
    } else {
        LinkAnimationHeader* anim;

        anim = GET_PLAYER_ANIM(PLAYER_ANIMGROUP_15, this->modelAnimType);
        func_80831494(play, this, func_8084C6EC, 0);
        LinkAnimation_Change(play, &this->skelAnime, anim, 1.25f * D_8085C3E4, arg2, Animation_GetLastFrame(anim),
                             (u8)2, 0.0f);
    }
}

#ifdef NON_MATCHING
// matches but requires in-function static data
void func_80836C70(PlayState* play, Player* this, s32 bodyPartIndex) {
    static Vec3f D_8085D130 = { 0, 0, 0 };
    s32 i;

    for (i = 0; i < 4; i++) {
        Vec3f velocity;

        velocity.x = randPlusMinusPoint5Scaled(4.0f);
        velocity.y = Rand_ZeroFloat(2.0f);
        velocity.z = randPlusMinusPoint5Scaled(4.0f);
        D_8085D130.y = -0.2f;
        EffectSsHahen_Spawn(play, &this->bodyPartsPos[bodyPartIndex], &velocity, &D_8085D130, 0, 10, OBJECT_LINK_NUTS,
                            16, object_link_nuts_DL_008860);
    }
}
#else
extern Vec3f D_8085D130;
void func_80836C70(PlayState* play, Player* this, s32 bodyPartIndex);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80836C70.s")
#endif

void func_80836D8C(Player* this) {
    this->actor.focus.rot.x = 0;
    this->actor.focus.rot.z = 0;
    this->unk_AAC.x = 0;
    this->unk_AAC.y = 0;
    this->unk_AAC.z = 0;
    this->unk_AB2.x = 0;
    this->unk_AB2.y = 0;
    this->unk_AB2.z = 0;
    this->actor.shape.rot.y = this->actor.focus.rot.y;
    this->currentYaw = this->actor.focus.rot.y;
}

s32 func_80836DC0(PlayState* play, Player* this) {
    if ((MREG(48) != 0) || func_800C9DDC(&play->colCtx, this->actor.floorPoly, this->actor.floorBgId)) {
        func_80831494(play, this, func_808561B0, 0);
        this->stateFlags1 &= ~(PLAYER_STATE1_20000 | PLAYER_STATE1_40000000);
        func_8082E438(play, this, &gameplay_keep_Linkanim_00E270);
        func_8082DABC(this);
        func_80836D8C(this);
        this->actor.shape.shadowDraw = ActorShadow_DrawCircle;
        this->unk_B48 = -2000.0f;
        this->actor.shape.shadowScale = 13.0f;
        func_8082E1F0(this, NA_SE_PL_DEKUNUTS_IN_GRD);
        return true;
    }
    return false;
}

void func_80836EA0(PlayState* play, s16 quakeSpeed, s16 verticalMag, s16 quakeCountdown) {
    s16 quake = Quake_Add(Play_GetCamera(play, CAM_ID_MAIN), 3);

    Quake_SetSpeed(quake, quakeSpeed);
    Quake_SetQuakeValues(quake, verticalMag, 0, 0, 0);
    Quake_SetCountdown(quake, quakeCountdown);
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80836F10.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_808370D4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80837134.s")

void func_808373A4(PlayState* play, Player* this) {
    func_8082E438(play, this, &gameplay_keep_Linkanim_00E270);
    this->unk_B08[2] = 20000.0f;
    this->unk_B08[3] = 196608.0f;
    func_800B8E58(this, NA_SE_PL_DEKUNUTS_ATTACK);
}

s32 func_808373F8(PlayState* play, Player* this, u16 sfxId);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_808373F8.s")

s32 func_80837730(PlayState* play, Player* this, f32 arg2, s32 arg3);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80837730.s")

s32 func_8083784C(Player* this);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8083784C.s")

void func_808378FC(PlayState* play, Player* this);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_808378FC.s")

s32 func_8083798C(Player* this) {
    return (this->interactRangeActor != NULL) && (this->heldActor == NULL) &&
           (this->transformation != PLAYER_FORM_DEKU);
}

void func_8084E334(Player* this, PlayState* play);
void func_8084E4E4(Player* this, PlayState* play);
void func_8084E25C(Player* this, PlayState* play);

void func_808379C0(PlayState* play, Player* this) {
    if (func_8083798C(this)) {
        Actor* interactRangeActor = this->interactRangeActor;
        LinkAnimationHeader* anim;

        if ((interactRangeActor->id == ACTOR_EN_ISHI) && (ENISHI_GET_1(interactRangeActor) != 0)) {
            func_80831494(play, this, func_8084E334, 0);
            anim = &gameplay_keep_Linkanim_00DF90;
        } else if (((interactRangeActor->id == ACTOR_EN_BOMBF) || (interactRangeActor->id == ACTOR_EN_KUSA) ||
                    (interactRangeActor->id == ACTOR_EN_KUSA2) || (interactRangeActor->id == ACTOR_OBJ_GRASS_CARRY)) &&
                   (Player_GetStrength() <= PLAYER_STRENGTH_DEKU)) {
            func_80831494(play, this, func_8084E4E4, 0);
            anim = &gameplay_keep_Linkanim_00DD70;

            this->actor.world.pos.x =
                (Math_SinS(interactRangeActor->yawTowardsPlayer) * 20.0f) + interactRangeActor->world.pos.x;
            this->actor.world.pos.z =
                (Math_CosS(interactRangeActor->yawTowardsPlayer) * 20.0f) + interactRangeActor->world.pos.z;

            this->currentYaw = this->actor.shape.rot.y = interactRangeActor->yawTowardsPlayer + 0x8000;
        } else {
            func_80831494(play, this, func_8084E25C, 0);
            anim = GET_PLAYER_ANIM(PLAYER_ANIMGROUP_12, this->modelAnimType);
        }

        func_8082DB18(play, this, anim);
    } else {
        func_80836988(this, play);
        this->stateFlags1 &= ~PLAYER_STATE1_800;
    }
}

void func_8084E980(Player* this, PlayState* play);
void func_8084ED9C(Player* this, PlayState* play);

void func_80837B60(PlayState* play, Player* this) {
    func_8083172C(play, this, func_8084E980, 0);

    this->exchangeItemId = 0;
    this->stateFlags1 |= (PLAYER_STATE1_40 | PLAYER_STATE1_20000000);
    if (this->actor.textId != 0) {
        Message_StartTextbox(play, this->actor.textId, this->targetActor);
    }
    this->unk_730 = this->targetActor;
}

void func_80837BD0(PlayState* play, Player* this) {
    func_8083172C(play, this, func_8084FE7C, 0);
}

void func_80837BF8(PlayState* play, Player* this) {
    func_80831494(play, this, func_8084ED9C, 0);
}

void func_8084F4E8(Player* this, PlayState* play);
void func_80852C04(Player* this, PlayState* play);

void func_80837C20(PlayState* play, Player* this) {
    s32 sp1C = this->unk_AE8;
    s32 sp18 = this->unk_AE7;

    func_8083172C(play, this, func_8084F4E8, 0);
    this->actor.velocity.y = 0.0f;
    this->unk_AE8 = sp1C;
    this->unk_AE7 = sp18;
}

void func_80837C78(PlayState* play, Player* this) {
    func_8083172C(play, this, func_80852C04, 0);
    this->stateFlags1 |= (PLAYER_STATE1_400 | PLAYER_STATE1_20000000);

    if (this->getItemId == GI_HEART_CONTAINER) {
        this->unk_AE8 = 0x14;
    } else if (this->getItemId >= GI_NONE) {
        this->unk_AE8 = 1;
    } else {
        this->getItemId = -this->getItemId;
    }
}

void func_8084F1B8(Player* this, PlayState* play);

void func_80837CEC(PlayState* play, Player* this, CollisionPoly* arg2, f32 arg3, LinkAnimationHeader* anim) {
    f32 sp24 = arg2->normal.x * SHT_MINV;
    f32 sp20 = arg2->normal.z * SHT_MINV;

    func_80831494(play, this, func_8084F1B8, 0);
    func_8082DE50(play, this);
    func_8082DB18(play, this, anim);

    this->actor.world.pos.x -= (arg3 + 1.0f) * sp24;
    this->actor.world.pos.z -= (arg3 + 1.0f) * sp20;
    this->actor.shape.rot.y = Math_FAtan2F(sp20, sp24);

    this->currentYaw = this->actor.shape.rot.y;
    func_8082DAD4(this);
    this->actor.velocity.y = 0.0f;
    func_8082E6D0(this);
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80837DEC.s")

void func_8084F3DC(Player* this, PlayState* play);

void func_808381A0(Player* this, LinkAnimationHeader* anim, PlayState* play) {
    func_80831494(play, this, func_8084F3DC, 0);
    LinkAnimation_PlayOnceSetSpeed(play, &this->skelAnime, anim, 1.3f);
}

extern Vec3f D_8085D148;

s32 func_808381F8(PlayState* play, Player* this) {
    CollisionPoly* poly;
    s32 bgId;
    Vec3f pos;
    f32 yIntersect;

    func_80835BC8(this, &this->actor.prevPos, &D_8085D148, &pos);

    yIntersect = BgCheck_EntityRaycastFloor5(&play->colCtx, &poly, &bgId, &this->actor, &pos);

    return fabsf(yIntersect - this->actor.world.pos.y) < 10.0f;
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8083827C.s")

void func_8083868C(PlayState* play, Player* this) {
    s32 camMode;
    Camera* camera;

    if (this->unk_AA5 == 3) {
        if (func_800B7118(this)) {
            if (this->transformation == PLAYER_FORM_HUMAN) {
                camMode = CAM_MODE_SLINGSHOT;
            } else if (this->transformation == PLAYER_FORM_DEKU) {
                camMode = CAM_MODE_DEKUSHOOT;
            } else {
                camMode = CAM_MODE_BOWARROW;
            }
        } else {
            camMode = CAM_MODE_ZORAFIN;
        }
    } else {
        camMode = CAM_MODE_FIRSTPERSON;
    }

    camera = (this->actor.id == ACTOR_PLAYER) ? Play_GetCamera(play, CAM_ID_MAIN)
                                              : Play_GetCamera(play, ((EnTest3*)this)->unk_D8E);

    Camera_ChangeMode(camera, camMode);
}

void func_80838760(Player* this) {
    if (this->unk_A86 >= 0) {
        ActorCutscene_Stop(this->unk_A86);
        this->unk_A86 = -1;
    }
}

s32 func_808387A0(PlayState* play, Player* this) {
    if (this->unk_AA5 == 4) {
        func_80838760(this);
        this->actor.flags &= ~ACTOR_FLAG_100;
        func_80831494(play, this, func_8085B08C, 0);
        if (this->unk_3BA != 0) {
            this->stateFlags1 |= PLAYER_STATE1_20000000;
        }
        func_8082DC38(this);

        return true;
    }
    return false;
}

void func_80838830(Player* this, s16 objectId) {
    s32 pad[2];

    if (objectId != OBJECT_UNSET_0) {
        this->giObjectLoading = true;
        osCreateMesgQueue(&this->giObjectLoadQueue, &this->giObjectLoadMsg, 1);
        DmaMgr_SendRequestImpl(&this->giObjectDmaRequest, this->giObjectSegment, gObjectTable[objectId].vromStart,
                               gObjectTable[objectId].vromEnd - gObjectTable[objectId].vromStart, 0,
                               &this->giObjectLoadQueue, NULL);
    }
}

void func_808553F4(Player* this, PlayState* play);
void func_80855AF4(Player* this, PlayState* play);
void func_80855B9C(Player* this, PlayState* play);

void func_808388B8(PlayState* play, Player* this, PlayerTransformation playerForm) {
    func_8082DE50(play, this);
    func_80831760(play, this, func_808553F4, 0);
    func_8082E4A4(play, this, D_8085D160[this->transformation]);
    gSaveContext.save.playerForm = playerForm;
    this->stateFlags1 |= PLAYER_STATE1_2;

    D_80862B50 = play->envCtx.lightSettings;
    this->actor.velocity.y = 0.0f;
    func_800B90F4(play);
}

void func_808389BC(PlayState* play, Player* this) {
    func_80831760(play, this, func_80855AF4, 0);
    func_8082E4A4(play, this, &gameplay_keep_Linkanim_00D0C8);
    this->stateFlags1 |= (PLAYER_STATE1_100 | PLAYER_STATE1_20000000);
    func_8082DAD4(this);
}

void func_80838A20(PlayState* play, Player* this) {
    func_80831760(play, this, func_80855B9C, 0);
    func_8082DB90(play, this, &gameplay_keep_Linkanim_00D0A8);
    this->currentMask = PLAYER_MASK_NONE;
    this->stateFlags1 |= (PLAYER_STATE1_100 | PLAYER_STATE1_20000000);
    func_8082DAD4(this);
    func_80115D5C(&play->state);
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80838A90.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_808391D8.s")

s32 func_80839518(Player* this, PlayState* play) {
    if (this->unk_AA5 != 0) {
        func_80838A90(this, play);
        return true;
    } else if ((this->unk_730 != NULL) && (CHECK_FLAG_ALL(this->unk_730->flags, ACTOR_FLAG_1 | ACTOR_FLAG_40000) ||
                                           (this->unk_730->hintId != 0xFF))) {
        this->stateFlags2 |= PLAYER_STATE2_200000;
    } else if ((this->tatlTextId == 0) && !func_80123420(this) && CHECK_BTN_ALL(D_80862B44->press.button, BTN_CUP) &&
               !func_80831814(this, play, 1)) {
        play_sound(NA_SE_SY_ERROR);
    }
    return false;
}

void func_8084CB58(Player*, PlayState* play);

// Jumpslash/Jumpkick start
void func_808395F0(PlayState* play, Player* this, PlayerMeleeWeaponAnimation meleeWeaponAnimation, f32 linearVelocity,
                   f32 yVelocity) {
    if (this->transformation == PLAYER_FORM_ZORA) {
        linearVelocity *= 1.1f;
        meleeWeaponAnimation = PLAYER_MWA_ZORA_JUMPKICK_START;
        yVelocity *= 0.9f;
    }

    func_80833864(play, this, meleeWeaponAnimation);
    func_80831494(play, this, func_8084CB58, 0);
    this->stateFlags3 |= PLAYER_STATE3_2;
    this->linearVelocity = linearVelocity;
    this->currentYaw = this->actor.shape.rot.y;
    this->actor.velocity.y = yVelocity;
    this->actor.bgCheckFlags &= ~1;
    func_8082E188(this);
    func_8082DF8C(this, NA_SE_VO_LI_SWORD_L);
}

s32 func_808396B8(PlayState* play, Player* this) {
    if (!(this->stateFlags1 & PLAYER_STATE1_400000) &&
        (((this->actor.id != ACTOR_PLAYER) && CHECK_BTN_ALL(D_80862B44->press.button, BTN_B)) ||
         ((Player_GetMeleeWeaponHeld(this) != 0) &&
          ((this->transformation != PLAYER_FORM_GORON) || (this->actor.bgCheckFlags & 1)) &&
          ((this->transformation != PLAYER_FORM_ZORA) || !(this->stateFlags1 & PLAYER_STATE1_2000000)) &&
          (D_80862B48 != 0)))) {
        return true;
    }
    return false;
}

s32 func_80839770(Player* this, PlayState* play) {
    if (func_808396B8(play, this)) {
        if ((this->transformation != PLAYER_FORM_GORON) && (D_80862B08 != 7)) {
            func_808395F0(play, this,
                          (this->transformation == PLAYER_FORM_ZORA) ? PLAYER_MWA_ZORA_JUMPKICK_START
                                                                     : PLAYER_MWA_JUMPSLASH_START,
                          3.0f, 4.5f);
            return true;
        }
    }
    return false;
}

s32 func_80839800(Player* this, PlayState* play) {
    if ((this->unk_AE3[this->unk_ADE] == 0) && (D_80862B08 != 7)) {
        func_80836B3C(play, this, 0.0f);
        return true;
    }
    return false;
}

typedef struct struct_8085C2A4 {
    /* 0x0 */ LinkAnimationHeader* unk_0;
    /* 0x4 */ UNK_TYPE1 unk_4[4];
    /* 0x8 */ UNK_TYPE1 unk_8[4];
} struct_8085C2A4; // size = 0xC

extern struct_8085C2A4 D_8085C2A4[];

#ifdef NON_MATCHING
void func_80839860(Player* this, PlayState* play, s32 arg2) {
    s32 pad;
    f32 var_fv0;

    if (!(arg2 & 1)) {
        var_fv0 = 5.8f;
    } else {
        var_fv0 = 3.5f;
    }

    if (this->currentBoots == PLAYER_BOOTS_GIANT) {
        var_fv0 *= 0.5f;
    }

    if (arg2) {}

    func_80834D50(play, this, D_8085C2A4[arg2].unk_0, var_fv0, NA_SE_VO_LI_SWORD_N);

    this->unk_AE8 = 1;
    this->unk_AE7 = arg2;
    this->currentYaw = this->actor.shape.rot.y + (arg2 << 0xE);
    if (!(arg2 & 1)) {
        this->linearVelocity = 6.0f;
    } else {
        this->linearVelocity = 8.5f;
    }
    this->stateFlags2 |= PLAYER_STATE2_80000;
    func_800B8E58(this, ((arg2 << 0xE) == 0x8000) ? NA_SE_PL_ROLL : NA_SE_PL_SKIP);
}
#else
void func_80839860(Player* this, PlayState* play, s32 arg2);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80839860.s")
#endif

void func_80839978(PlayState* play, Player* this) {
    if (this->actor.bgCheckFlags & 1) {
        this->heldItemActionParam = PLAYER_AP_OCARINA;
        func_80831760(play, this, func_8084AC84, 0);
        func_8082DB60(play, this, &gameplay_keep_Linkanim_00D228);
        func_8082E920(play, this, (4 | 0x200));
        this->stateFlags3 |= PLAYER_STATE3_20000000;
        this->unk_B48 = this->linearVelocity;
        func_801A3098(NA_BGM_BREMEN_MARCH);
    }
}

void func_80839A10(PlayState* play, Player* this) {
    if (this->actor.bgCheckFlags & 1) {
        this->heldItemActionParam = PLAYER_AP_NONE;
        func_80831760(play, this, func_8084AEEC, 0);
        func_8082DB60(play, this, &gameplay_keep_Linkanim_00CF98);
        this->stateFlags2 |= PLAYER_STATE3_2000000;
        func_801A3098(NA_BGM_KAMARO_DANCE);
    }
}

void func_808573A4(Player* this, PlayState* play);

#ifdef NON_MATCHING
s32 func_80839A84(PlayState* play, Player* this) {
    if (this->transformation == PLAYER_FORM_DEKU) {
        if (func_80836DC0(play, this)) {
            return true;
        }

        func_80831494(play, this, func_808573A4, 0);
        this->unk_ADC = 4;
        this->stateFlags1 &= ~(PLAYER_STATE1_20000 | PLAYER_STATE1_40000000);
        func_808373A4(play, this);
        return true;
    }

    return false;
}
#else
s32 func_80839A84(PlayState* play, Player* this);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80839A84.s")
#endif

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80839B18.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80839CD8.s")

void func_80839CD8(Player* this, PlayState* play);

void func_80839E3C(Player* this, PlayState* play) {
    func_808369F4(this, play);
    func_80839CD8(this, play);
}

void func_80839E74(Player* this, PlayState* play) {
    func_80831494(play, this, func_80849FE0, 1);
    func_8082DB18(play, this, func_8082ED20(this));
    this->currentYaw = this->actor.shape.rot.y;
}

void func_80852B28(Player* this, PlayState* play);

void func_80839ED0(Player* this, PlayState* play) {
    if (!(this->stateFlags3 & PLAYER_STATE3_80) && (func_80852B28 != this->unk_748) && !func_8083213C(this)) {
        func_80836D8C(this);
        if (!(this->stateFlags1 & PLAYER_STATE1_40)) {
            if (func_801242B4(this)) {
                func_808353DC(play, this);
            } else {
                func_80836988(this, play);
            }
        }
        if (this->unk_AA5 < 5) {
            this->unk_AA5 = 0;
        }
    }
    this->stateFlags1 &= ~(PLAYER_STATE1_4 | PLAYER_STATE1_2000 | PLAYER_STATE1_4000 | PLAYER_STATE1_100000);
}

s32 func_80839F98(PlayState* arg0, Player* arg1) {
    if (!(arg1->stateFlags1 & PLAYER_STATE1_8000000)) {
        if (arg1->linearVelocity != 0.0f) {
            func_80836B3C(arg0, arg1, 0.0f);
            return true;
        }
        func_80836AD8(arg0, arg1);
        LinkAnimation_Change(arg0, &arg1->skelAnime, &gameplay_keep_Linkanim_00E208, 2.0f / 3.0f, 0.0f, 7.0f, 2, 0.0f);
        return true;
    }
    return false;
}

// Toggles swimming/walking underwater as Zora
void func_8083A04C(Player* this) {
    if (this->currentBoots == PLAYER_BOOTS_ZORA_UNDERWATER) {
        if (CHECK_BTN_ALL(D_80862B44->press.button, BTN_A)) {
            this->currentBoots = PLAYER_BOOTS_ZORA_LAND;
        }
        if (func_808508C8 == this->unk_748) {
            this->unk_AE8 = 20;
        }
    } else {
        if (CHECK_BTN_ALL(D_80862B44->press.button, BTN_B)) {
            this->currentBoots = PLAYER_BOOTS_ZORA_UNDERWATER;
        }
    }
}

s32 func_8083A0CC(Player* this, PlayState* play) {
    if ((D_80862B04 == 0) && (this->transformation == PLAYER_FORM_ZORA)) {
        func_8083A04C(this);
    }
    return false;
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8083A114.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8083A274.s")

s32 func_8083A4A4(Player* this, f32* arg1, s16* arg2, f32 arg3) {
    s16 yaw = this->currentYaw - *arg2;

    if (ABS_ALT(yaw) > 0x6000) {
        if (Math_StepToF(&this->linearVelocity, 0.0f, arg3) != 0) {
            *arg1 = 0.0f;
            *arg2 = this->currentYaw;
        } else {
            return true;
        }
    }
    return false;
}

void func_8083A548(Player* this) {
    if ((this->unk_ADC > 0) && !CHECK_BTN_ALL(D_80862B44->cur.button, BTN_B)) {
        this->unk_ADC = -this->unk_ADC;
    }
}

s32 func_8083A580(Player* this, PlayState* play) {
    if (CHECK_BTN_ALL(D_80862B44->cur.button, BTN_B)) {
        if (!(this->stateFlags1 & PLAYER_STATE1_400000) && (Player_GetMeleeWeaponHeld(this) != 0)) {
            if ((this->unk_ADC > 0) && (((this->transformation == PLAYER_FORM_ZORA)) ||
                                        ((this->unk_ADC == 1) && (this->itemActionParam != PLAYER_AP_STICK)))) {
                if (this->transformation == PLAYER_FORM_ZORA) {
                    func_80830E30(this, play);
                } else {
                    func_808335B0(play, this);
                }
                return true;
            }
        }
    } else {
        func_8083A548(this);
    }
    return false;
}

// unused
s32 func_8083A658(PlayState* play, Player* this) {
    if (this->actor.bgCheckFlags & 1) {
        func_80831494(play, this, func_80852B28, 0);
        func_8082DB18(play, this, &gameplay_keep_Linkanim_00DD38);
        this->unk_AA5 = 0;
        return true;
    }
    return false;
}

void func_808534C0(Player*, PlayState* play);

struct _struct_D_8085D200_0xC {
    /* 0x0 */ LinkAnimationHeader* unk_0; /* inferred */
    /* 0x4 */ LinkAnimationHeader* unk_4; /* inferred */
    /* 0x8 */ u8 unk_8;                   /* inferred */
    /* 0x9 */ u8 unk_9;                   /* inferred */
    /* 0xA */ char pad_A[2];              /* maybe part of unk_9[3]? */
};                                        /* size = 0xC */

extern struct _struct_D_8085D200_0xC D_8085D200[];

s32 func_8083A6C0(PlayState* play, Player* this) {
    if (D_80862B48 != 0) {
        if (Player_GetBottleHeld(this) >= 0) {
            func_80831494(play, this, func_808534C0, 0);
            if (this->actor.depthInWater > 12.0f) {
                this->unk_AE8 = 1;
            }
            func_8082DB90(play, this, D_8085D200[this->unk_AE8].unk_0);
            func_800B8E58(this, NA_SE_IT_SWORD_SWING);
            func_8082DF8C(this, NA_SE_VO_LI_AUTO_JUMP);
            return true;
        }
        return false;
    }
    return false;
}

void func_8084AF9C(Player* this, PlayState* play);
void func_8084B0EC(Player* this, PlayState* play);

void func_8083A794(Player* this, PlayState* play) {
    if ((func_8084AF9C != this->unk_748) && (func_8084B0EC != this->unk_748)) {
        this->unk_B70 = 0;
        this->unk_B34 = 0.0f;
        this->unk_B38 = 0.0f;
        func_8082E514(play, this, GET_PLAYER_ANIM(PLAYER_ANIMGROUP_2, this->modelAnimType));
    }

    func_80831494(play, this, func_8082FBE8(this) ? func_8084B0EC : func_8084AF9C, 1);
}

void func_8083A844(Player* this, PlayState* play, s16 currentYaw) {
    this->currentYaw = currentYaw;
    this->actor.shape.rot.y = this->currentYaw;
    func_8083A794(this, play);
}

void func_80850B18(Player*, PlayState* play);

s32 func_8083A878(PlayState* play, Player* this, f32 arg2) {
    WaterBox* waterBox;
    f32 ySurface = this->actor.world.pos.y;

    if (WaterBox_GetSurface1(play, &play->colCtx, this->actor.world.pos.x, this->actor.world.pos.z, &ySurface,
                             &waterBox)) {
        ySurface -= this->actor.world.pos.y;
        if (this->ageProperties->unk_24 <= ySurface) {
            func_80831494(play, this, func_80850B18, 0);
            func_8082E634(play, this, &gameplay_keep_Linkanim_00DFD0);
            this->stateFlags1 |= (PLAYER_STATE1_8000000 | PLAYER_STATE1_20000000);
            this->unk_AE8 = 20;
            this->linearVelocity = 2.0f;
            func_80123140(play, this);
            return false;
        }
    }
    func_80835324(play, this, arg2, this->actor.shape.rot.y);
    this->stateFlags1 |= PLAYER_STATE1_20000000;
    return true;
}

void func_8083A98C(Actor* thisx, PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8083A98C.s")

void func_8083AD04(PlayState* play, Player* this) {
    this->actor.update = func_8083A98C;
    this->actor.draw = NULL;
    if (play->sceneNum == SCENE_00KEIKOKU) {
        this->actor.focus.rot.x = 0xBD8;
        this->actor.focus.rot.y = -0x4D74;
        this->unk_AE8 = 20;
    } else if (play->sceneNum == SCENE_AYASHIISHOP) {
        this->actor.focus.rot.x = 0x9A6;
        this->actor.focus.rot.y = 0x2102;
        this->unk_AE8 = 2;
    } else {
        this->actor.focus.rot.x = 0x9A6;
        this->actor.focus.rot.y = 0x2102;
        this->unk_AE8 = 20;
    }
    play->actorCtx.unk5 |= 2;
}

void func_8083AD8C(PlayState* play, Player* this) {
    func_8085B384(this, play);
}

void func_808412A0(PlayState* play, Player* this);
void func_80841408(PlayState* play, Player* this);
void func_808412BC(PlayState* play, Player* this);
void func_808414E0(PlayState* play, Player* this);
void func_80841528(PlayState* play, Player* this);
void func_808415E4(PlayState* play, Player* this);
void func_80841624(PlayState* play, Player* this);
void func_808415A0(PlayState* play, Player* this);
void func_80841744(PlayState* play, Player* this);
void func_80841744(PlayState* play, Player* this);
void func_8083ADF0(PlayState* play, Player* this);
void func_8083AD8C(PlayState* play, Player* this);
void func_8083AD04(PlayState* play, Player* this);
void func_8083ADB8(PlayState* play, Player* this);
void func_8083ADF0(PlayState* play, Player* this);
void func_8083AE38(PlayState* play, Player* this);

void func_8083ADB8(PlayState* play, Player* this) {
    if (func_8083A878(play, this, 180.0f)) {
        this->unk_AE8 = -20;
    }
}

void func_8083ADF0(PlayState* play, Player* this) {
    this->linearVelocity = 2.0f;
    gSaveContext.entranceSpeed = 2.0f;

    if (func_8083A878(play, this, 120.0f)) {
        this->unk_AE8 = -15;
    }
}

void func_8083AE38(PlayState* play, Player* this) {
    if (gSaveContext.entranceSpeed < 0.1f) {
        gSaveContext.entranceSpeed = 0.1f;
    }

    this->linearVelocity = gSaveContext.entranceSpeed;
    if (func_8083A878(play, this, 800.0f)) {
        this->unk_AE8 = -80.0f / this->linearVelocity;
        if (this->unk_AE8 < -20) {
            this->unk_AE8 = -20;
        }
    }
}

void func_8084A5C0(Player* this, PlayState* play);
void func_8084A26C(Player* this, PlayState* play);
void func_8084B288(Player* this, PlayState* play);
void func_8084A8E8(Player* this, PlayState* play);
void func_8084B3B8(Player* this, PlayState* play);
void func_8084AB4C(Player* this, PlayState* play);

void func_8083AECC(Player* this, s16 currentYaw, PlayState* play) {
    func_80831494(play, this, func_8084A5C0, 1);
    LinkAnimation_CopyJointToMorph(play, &this->skelAnime);
    this->unk_B38 = 0.0f;
    this->unk_B34 = 0.0f;
    this->currentYaw = currentYaw;
}

void func_8083AF30(Player* this, PlayState* play) {
    func_80831494(play, this, func_8084A26C, 1);
    func_8082E514(play, this, GET_PLAYER_ANIM(PLAYER_ANIMGROUP_1, this->modelAnimType));
}

void func_8083AF8C(Player* this, s16 currentYaw, PlayState* play) {
    func_80831494(play, this, func_8084B288, 1);
    LinkAnimation_Change(play, &this->skelAnime, &gameplay_keep_Linkanim_00D3F8, 1.0f, 0.0f,
                         Animation_GetLastFrame(&gameplay_keep_Linkanim_00D3F8), (u8)2, -6.0f);
    this->linearVelocity = 8.0f;
    this->currentYaw = currentYaw;
}

void func_8083B030(Player* this, PlayState* play) {
    func_80831494(play, this, func_8084A8E8, 1);
    func_8082E514(play, this, GET_PLAYER_ANIM(PLAYER_ANIMGROUP_24, this->modelAnimType));
    this->unk_B38 = 0.0f;
}

void func_8083B090(Player* this, PlayState* play) {
    func_80831494(play, this, func_8084B3B8, 1);
    LinkAnimation_PlayOnceSetSpeed(play, &this->skelAnime, &gameplay_keep_Linkanim_00D3E8, 2.0f);
}

void func_8083B0E4(PlayState* play, Player* this, s16 currentYaw) {
    this->currentYaw = currentYaw;
    func_80831494(play, this, func_8084AB4C, 1);
    this->unk_B4E = 1200;
    this->unk_B4E *= D_8085C3E4;
    LinkAnimation_Change(play, &this->skelAnime, GET_PLAYER_ANIM(PLAYER_ANIMGROUP_25, this->modelAnimType), 1.0f, 0.0f,
                         0.0f, 0, -6.0f);
}

void func_8083B1A0(Player* this, PlayState* play) {
    LinkAnimationHeader* anim;

    func_80831494(play, this, func_80849FE0, 1);
    if (this->unk_B40 < 0.5f) {
        anim = GET_PLAYER_ANIM(PLAYER_ANIMGROUP_27, this->modelAnimType);
    } else {
        anim = GET_PLAYER_ANIM(PLAYER_ANIMGROUP_26, this->modelAnimType);
    }
    func_8082DB18(play, this, anim);
    this->currentYaw = this->actor.shape.rot.y;
}

void func_8083B23C(Player* this, PlayState* play) {
    func_80831494(play, this, func_80849A9C, 1);
    func_8082E438(play, this, GET_PLAYER_ANIM(PLAYER_ANIMGROUP_6, this->modelAnimType));
    this->unk_AE8 = 1;
}

void func_8083B29C(Player* this, PlayState* play) {
    if (this->linearVelocity != 0.0f) {
        func_8083A794(this, play);
        return;
    }
    func_8083B1A0(this, play);
}

void func_8083B2E4(Player* this, PlayState* play) {
    if (this->linearVelocity != 0.0f) {
        func_8083A794(this, play);
        return;
    } else {
        func_80836988(this, play);
    }
}

void func_8083B32C(PlayState* play, Player* this, f32 arg2) {
    this->stateFlags1 |= PLAYER_STATE1_40000;
    this->stateFlags1 &= ~PLAYER_STATE1_8000000;
    func_8082DC64(play, this);

    if (func_80837730(play, this, arg2, 500)) {
        func_800B8E58(this, NA_SE_EV_JUMP_OUT_WATER);
    }
    func_80123140(play, this);
}

// boolean
s32 func_8083B3B4(PlayState* play, Player* this, Input* input);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8083B3B4.s")

void func_808513EC(Player*, PlayState* play);

void func_8083B73C(PlayState* play, Player* this, s16 currentYaw) {
    func_80831494(play, this, func_808513EC, 0);
    func_8082E634(play, this, &gameplay_keep_Linkanim_00DFD0);
    this->actor.shape.rot.y = currentYaw;
    this->currentYaw = currentYaw;
}

void func_8083B798(PlayState* play, Player* this) {
    if (this->transformation == PLAYER_FORM_ZORA) {
        func_80831494(play, this, func_808513EC, 0);
        LinkAnimation_Change(play, &this->skelAnime, &gameplay_keep_Linkanim_00DFD0, 1.0f,
                             Animation_GetLastFrame(&gameplay_keep_Linkanim_00DFD0), 0.0f, 0, 0.0f);
        this->unk_B48 = 2.0f;
    } else {
        func_8082DB3C(play, this, &gameplay_keep_Linkanim_00DFD0);
        this->unk_AE8 = 1;
    }

    this->unk_AAA = 0x3E80;
}

void func_80850D68(Player* this, PlayState* play);

void func_8083B850(PlayState* play, Player* this) {
    this->currentBoots = PLAYER_BOOTS_ZORA_LAND;
    this->prevBoots = PLAYER_BOOTS_ZORA_LAND;
    func_80831494(play, this, func_80850D68, 0);
    this->unk_B48 = sqrtf(SQ(this->linearVelocity) + SQ(this->actor.velocity.y));
    func_8082F938(play, this, 1, 8);
    this->currentBoots = PLAYER_BOOTS_ZORA_LAND;
    this->prevBoots = PLAYER_BOOTS_ZORA_LAND;
}

void func_8083B8D0(PlayState* play, Player* this) {
    if (func_80837730(play, this, this->actor.velocity.y, 500)) {
        func_800B8E58(this, NA_SE_EV_DIVE_INTO_WATER);
        if (this->unk_B6A > 0x320) {
            func_8082DF8C(this, NA_SE_VO_LI_CLIMB_END);
        }
    }
}

void func_80857BE8(Player* this, PlayState* play);
void func_8084CA24(Player* this, PlayState* play);
void func_8084C94C(Player* this, PlayState* play);

void func_808516B4(Player* this, PlayState* play);

void func_8083B930(PlayState* play, Player* this) {
    LinkAnimationHeader* var_a2;

    if ((this->currentBoots < PLAYER_BOOTS_ZORA_UNDERWATER) || !(this->actor.bgCheckFlags & 1) ||
        (func_80857BE8 == this->unk_748)) {
        func_8082DE50(play, this);

        if (func_8084CA24 == this->unk_748) {
            func_8083B850(play, this);
            this->stateFlags3 |= PLAYER_STATE3_8000;
        } else if ((this->transformation == PLAYER_FORM_ZORA) && (func_8084C94C == this->unk_748)) {
            func_8083B850(play, this);
            this->stateFlags3 |= PLAYER_STATE3_8000;
            func_8082DB60(play, this, &gameplay_keep_Linkanim_00E3D8);
        } else if ((this->currentBoots < PLAYER_BOOTS_ZORA_UNDERWATER) && (this->stateFlags2 & PLAYER_STATE2_400)) {
            this->stateFlags2 &= ~PLAYER_STATE2_400;
            func_8083B3B4(play, this, NULL);
            this->unk_AE7 = 1;
        } else if (func_8084C94C == this->unk_748) {
            func_80831494(play, this, func_808516B4, 0);
            func_8083B798(play, this);
        } else {
            func_80831494(play, this, func_808508C8, 1);
            func_8082E438(play, this,
                          (this->actor.bgCheckFlags & 1) ? &gameplay_keep_Linkanim_00E008
                                                         : &gameplay_keep_Linkanim_00DFC0);
        }
    }
    if (!(this->stateFlags1 & PLAYER_STATE1_8000000) || (this->actor.depthInWater < this->ageProperties->unk_2C)) {
        func_8083B8D0(play, this);
    }

    this->stateFlags1 |= PLAYER_STATE1_8000000;
    this->stateFlags2 |= PLAYER_STATE2_400;
    this->stateFlags1 &= ~(PLAYER_STATE1_40000 | PLAYER_STATE1_80000);

    this->unk_AEC = 0.0f;
    func_80123140(play, this);
}

void func_808497A0(Player* this, PlayState* play);
void func_8084D4EC(Player* this, PlayState* play);

void func_8084E724(Player* this, PlayState* play);
void func_80851B58(Player* this, PlayState* play);
void func_80851588(Player* this, PlayState* play);
void func_808519FC(Player* this, PlayState* play);

void func_8083BB4C(PlayState* play, Player* this) {
    f32 sp1C = this->actor.depthInWater - this->ageProperties->unk_2C;

    if (sp1C < 0.0f) {
        this->unk_AD8 = 0;
        if ((this->transformation == PLAYER_FORM_ZORA) && (this->actor.bgCheckFlags & 1)) {
            this->currentBoots = 4;
        }
        func_801A3E38(0);
    } else {
        func_801A3E38(0x20);
        if ((this->transformation == PLAYER_FORM_ZORA) || (sp1C < 10.0f)) {
            this->unk_AD8 = 0;
        } else if (this->unk_AD8 < 300) {
            this->unk_AD8++;
        }
    }

    if ((this->actor.parent == NULL) && (func_8084D4EC != this->unk_748) && (func_8084F3DC != this->unk_748) &&
        ((func_8084CA24 != this->unk_748) || (this->actor.velocity.y < -2.0f))) {
        if (this->ageProperties->unk_2C < this->actor.depthInWater) {
            if (this->transformation == PLAYER_FORM_GORON) {
                func_80834140(play, this, &gameplay_keep_Linkanim_00DFE8);
                func_808345C8();
                func_8083B8D0(play, this);
            } else if (this->transformation == PLAYER_FORM_DEKU) {
                if (this->unk_B67 != 0) {
                    func_808373F8(play, this, NA_SE_VO_LI_AUTO_JUMP);
                } else {
                    if ((play->sceneNum == SCENE_20SICHITAI) && (this->unk_3CF == 0)) {
                        if (gSaveContext.eventInf[5] & 1) {
                            play->nextEntranceIndex = 0xA820;
                        } else {
                            play->nextEntranceIndex = 0xA810;
                        }
                        play->transitionTrigger = TRANS_TRIGGER_START;
                        play->transitionType = 4;
                        this->stateFlags1 |= PLAYER_STATE1_200;
                        play_sound(NA_SE_SY_DEKUNUTS_JUMP_FAILED);
                    } else if ((this->unk_3CF == 0) &&
                               ((play->sceneNum == SCENE_30GYOSON) || (play->sceneNum == SCENE_31MISAKI) ||
                                (play->sceneNum == SCENE_TORIDE))) {
                        func_80169EFC(&play->state);
                        func_808345C8();
                    } else {
                        func_80831494(play, this, func_808497A0, 0);
                        this->stateFlags1 |= PLAYER_STATE1_20000000;
                    }
                    func_8083B8D0(play, this);
                }
            } else if (!(this->stateFlags1 & PLAYER_STATE1_8000000) ||
                       (((this->currentBoots < PLAYER_BOOTS_ZORA_UNDERWATER) || !(this->actor.bgCheckFlags & 1)) &&
                        (func_8084E724 != this->unk_748) && (func_80851B58 != this->unk_748) &&
                        (func_80851BD4 != this->unk_748) && (func_808508C8 != this->unk_748) &&
                        (func_808513EC != this->unk_748) && (func_80851588 != this->unk_748) &&
                        (func_808516B4 != this->unk_748) && (func_808519FC != this->unk_748) &&
                        (func_80850B18 != this->unk_748) && (func_80850D68 != this->unk_748))) {
                func_8083B930(play, this);
            }
        } else if ((this->stateFlags1 & PLAYER_STATE1_8000000) &&
                   (this->actor.depthInWater < this->ageProperties->unk_24) &&
                   (((func_80850D68 != this->unk_748) && !(this->stateFlags3 & PLAYER_STATE3_8000)) ||
                    (this->actor.bgCheckFlags & 1))) {
            if (this->skelAnime.moveFlags == 0) {
                this = this;
                func_8083B0E4(play, this, this->actor.shape.rot.y);
            }
            func_8083B32C(play, this, this->actor.velocity.y);
        }
    }
}

void func_8083BF54(PlayState* play, Player* this);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8083BF54.s")

#ifdef NON_MATCHING
s16 func_8083C62C(Player* this, s32 arg1) {
    Vec3f sp30;
    s16 sp2E;
    s16 sp2C;
    Vec3f* sp28 = &this->unk_730->focus.pos;

    sp30.x = this->actor.world.pos.x;
    sp30.y = this->bodyPartsPos[7].y + 3.0f;
    sp30.z = this->actor.world.pos.z;

    sp2E = Math_Vec3f_Pitch(&sp30, sp28);
    sp2C = Math_Vec3f_Yaw(&sp30, sp28);
    Math_SmoothStepToS(&this->actor.focus.rot.y, sp2C, 4, 0x2710, 0);
    Math_SmoothStepToS(&this->actor.focus.rot.x, sp2E, 4, 0x2710, 0);
    this->unk_AA6 |= 2;

    return func_80832754(this, arg1);
}
#else
s16 func_8083C62C(Player* this, s32 arg1);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8083C62C.s")
#endif

extern Vec3f D_8085D218;

void func_8083C6E8(Player* this, PlayState* play) {
    if (this->unk_730 != NULL) {
        if (func_800B7128(this) || func_8082EF20(this)) {
            func_8083C62C(this, 1);
        } else {
            func_8083C62C(this, 0);
        }
        return;
    }

    if (D_80862B08 == 0xB) {
        Math_SmoothStepToS(&this->actor.focus.rot.x, -20000, 10, 4000, 800);
    } else {
        s16 sp46 = 0;
        f32 yIntersect;
        Vec3f pos;
        s16 temp_v0;

        yIntersect = func_80835D2C(play, this, &D_8085D218, &pos);
        if (yIntersect > BGCHECK_Y_MIN) {
            temp_v0 = Math_FAtan2F(40.0f, this->actor.world.pos.y - yIntersect);
            sp46 = CLAMP(temp_v0, -4000, 4000);
        }
        this->actor.focus.rot.y = this->actor.shape.rot.y;
        Math_SmoothStepToS(&this->actor.focus.rot.x, sp46, 14, 4000, 30);
    }
    func_80832754(this, func_800B7128(this) || func_8082EF20(this));
}

#ifdef NON_MATCHING
void func_8083C85C(Player* this) {
    Math_ScaledStepToS(&this->unk_AB2.x, D_80862B3C * -500.0f, 900);
    this->unk_AA6 |= 0x48;
    this->unk_AAC.x = (-(f32)this->unk_AB2.x * 0.5f);
}
#else
void func_8083C85C(Player* this);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8083C85C.s")
#endif

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8083C8E8.s")

void func_8083CB04(Player* this, f32 arg1, s16 arg2, f32 arg3, f32 arg4, s16 arg5) {
    Math_AsymStepToF(&this->linearVelocity, arg1, arg3, arg4);
    Math_ScaledStepToS(&this->currentYaw, arg2, arg5);
}

void func_8083CB58(Player* this, f32 arg1, s16 arg2) {
    func_8083CB04(this, arg1, arg2, REG(19) / 100.0f, 1.5f, REG(27));
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8083CBC4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8083CCB4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8083CF68.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8083D168.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8083D23C.s")

void func_8084E65C(Player* this, PlayState* play);

void func_8083D6DC(Player* this, PlayState* play) {
    func_80831494(play, this, func_8084E65C, 1);
    func_8082DB18(play, this, GET_PLAYER_ANIM(PLAYER_ANIMGROUP_28, this->modelAnimType));
}

s32 func_8083D738(Player* this, Actor* heldActor) {
    if ((heldActor != NULL) && !(heldActor->flags & ACTOR_FLAG_800000) &&
        ((this->linearVelocity < 1.1f) || (heldActor->id == ACTOR_EN_BOM_CHU))) {
        return false;
    }
    return true;
}

void func_8084E58C(Player*, PlayState* play);

s32 func_8083D78C(Player* this, PlayState* play) {
    if (this->stateFlags1 & PLAYER_STATE1_800) {
        if ((this->heldActor != NULL) &&
            CHECK_BTN_ANY(D_80862B44->press.button, BTN_CRIGHT | BTN_CLEFT | BTN_CDOWN | BTN_B | BTN_A)) {
            if (!func_808313A8(play, this, this->heldActor)) {
                if (!func_8083D738(this, this->heldActor)) {
                    func_80831494(play, this, func_8084E58C, 1);
                    func_8082DB18(play, this, GET_PLAYER_ANIM(PLAYER_ANIMGROUP_29, this->modelAnimType));
                    return true;
                }
                func_8083D6DC(this, play);
            }

            return true;
        }
    }
    return false;
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8083D860.s")

void func_8084FC0C(Player* this, PlayState* play);

void func_8083DCC4(Player* this, LinkAnimationHeader* anim, PlayState* play) {
    func_8083172C(play, this, func_8084FC0C, 0);
    LinkAnimation_PlayOnceSetSpeed(play, &this->skelAnime, anim, 1.3333334f);
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8083DD1C.s")

void func_8083DEE4(PlayState* play, Player* this);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8083DEE4.s")

void func_8083DF38(Player* this, LinkAnimationHeader* anim, PlayState* play) {
    if (!func_80832558(play, this, func_80837BF8)) {
        func_80831494(play, this, func_8084ED9C, 0);
    }

    func_8082DB18(play, this, anim);
    func_8082DAD4(this);

    this->actor.shape.rot.y = this->currentYaw = this->actor.wallYaw + 0x8000;
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8083DFC4.s")

s32 func_8083E14C(PlayState* play, Player* this) {
    if ((this->actor.bgCheckFlags & 0x200) &&
        ((this->stateFlags2 & PLAYER_STATE2_10) || CHECK_BTN_ALL(D_80862B44->cur.button, BTN_A))) {
        DynaPolyActor* var_v1 = NULL;

        if (this->actor.wallBgId != 0x32) {
            var_v1 = DynaPoly_GetActor(&play->colCtx, this->actor.wallBgId);
        }

        if (&var_v1->actor == this->rightHandActor) {
            if (this->stateFlags2 & PLAYER_STATE2_10) {
                return true;
            }
            return false;
        }
    }

    func_808369F4(this, play);
    func_8082DB18(play, this, &gameplay_keep_Linkanim_00DE08);
    this->stateFlags2 &= ~PLAYER_STATE2_10;
    return true;
}

void func_8084EE50(Player* this, PlayState* play);
void func_8084EF9C(Player* this, PlayState* play);

void func_8083E234(Player* this, PlayState* play) {
    func_80831494(play, this, func_8084EE50, 0);
    func_8082DB18(play, this, &gameplay_keep_Linkanim_00DDF8);
    this->stateFlags2 |= PLAYER_STATE2_10;
}

void func_8083E28C(Player* this, PlayState* play) {
    func_80831494(play, this, func_8084EF9C, 0);
    func_8082DB18(play, this, GET_PLAYER_ANIM(PLAYER_ANIMGROUP_34, this->modelAnimType));
    this->stateFlags2 |= PLAYER_STATE2_10;
}

void func_8083E2F4(Player* this, PlayState* play) {
    this->stateFlags1 &= ~(PLAYER_STATE1_200000 | PLAYER_STATE1_8000000);
    func_80833AA0(this, play);

    if (this->transformation == PLAYER_FORM_DEKU) {
        this->linearVelocity = -1.7f;
    } else {
        this->linearVelocity = -0.4f;
    }
}

s32 func_8083E354(Player* this, PlayState* play) {
    if (!CHECK_BTN_ALL(D_80862B44->press.button, BTN_A) && (this->actor.bgCheckFlags & 0x200)) {
        if ((D_80862B0C & 8) || (D_80862B0C & 2) ||
            func_800C9AE4(&play->colCtx, this->actor.wallPoly, this->actor.wallBgId)) {
            return false;
        }
    }

    func_8083E2F4(this, play);
    func_8082DF8C(this, NA_SE_VO_LI_AUTO_JUMP);
    return true;
}

s32 func_8083E404(Player* this, f32 arg1, s16 arg2) {
    f32 sp1C = BINANG_SUB(arg2, this->actor.shape.rot.y);
    f32 temp_fv1;

    if (this->unk_730 != NULL) {
        func_8083C62C(this, func_800B7128(this) || func_8082EF20(this));
    }

    temp_fv1 = fabsf(sp1C) / 0x8000;
    if (((SQ(temp_fv1) * 50.0f) + 6.0f) < arg1) {
        return 1;
    }

    if ((((1.0f - temp_fv1) * 10.0f) + 6.8f) < arg1) {
        return -1;
    }
    return 0;
}

s32 func_8083E514(Player* this, f32* arg2, s16* arg3, PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8083E514.s")

s32 func_8083E758(Player* this, f32* arg1, s16* arg2) {
    f32 temp_fv0;
    u16 temp_v0;
    s16 var_v1;

    var_v1 = *arg2 - this->actor.shape.rot.y;
    temp_v0 = ABS_ALT(var_v1);
    temp_fv0 = Math_CosS(temp_v0);
    *arg1 *= temp_fv0;

    // Can't be (*arg1 != 0.0f)
    if (*arg1 != 0) {
        if (temp_fv0 > 0.0f) {
            return 1;
        }
        return -1;
    }
    return 0;
}

s32 func_8083E7F8(Player* this, f32* arg1, s16* arg2, PlayState* play) {
    func_8083C6E8(this, play);

    if ((*arg1 != 0.0f) || (ABS_ALT(this->unk_B4C) > 0x190)) {
        s16 temp_a0 = *arg2 - (u16)Camera_GetInputDirYaw(play->cameraPtrs[play->activeCamId]);
        u16 temp;

        temp = (ABS_ALT(temp_a0) - 0x2000);
        if ((temp < 0x4000) || (this->unk_B4C != 0)) {
            return -1;
        }
        return 1;
    }

    return 0;
}

void func_8083E8E0(Player* this, f32 arg1, s16 arg2) {
    s16 temp = arg2 - this->actor.shape.rot.y;

    if (arg1 > 0.0f) {
        if (temp < 0) {
            this->unk_B44 = 0.0f;
        } else {
            this->unk_B44 = 1.0f;
        }
    }

    Math_StepToF(&this->unk_B40, this->unk_B44, 0.3f);
}

void func_8083E958(PlayState* play, Player* this) {
    LinkAnimation_BlendToJoint(play, &this->skelAnime, func_8082EF54(this), this->unk_B38, func_8082EF9C(this),
                               this->unk_B38, this->unk_B40, (void*)this->unk_88A);
}

s32 func_8083E9C4(f32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    f32 temp_fv0;

    if ((arg3 == 0.0f) && (arg1 > 0.0f)) {
        arg3 = arg2;
    }
    temp_fv0 = (arg0 + arg1) - arg3;
    if (((temp_fv0 * arg1) >= 0.0f) && (((temp_fv0 - arg1) * arg1) < 0.0f)) {
        return true;
    }
    return false;
}

#ifdef NON_EQUIVALENT
// not sure about equivalency. there are a few extra instructions
void func_8083EA44(Player* this, f32 arg1) {
    s32 sp24;
    f32 new_var;

    new_var = SREG(30) * 0.5f;

    arg1 *= new_var;
    arg1 = CLAMP(arg1, -7.25f, 7.25f);
    sp24 = func_8083E9C4(this->unk_B38, arg1, 29.0f, 10.0f);

    if (sp24 || func_8083E9C4(this->unk_B38, arg1, 29.0f, 24.0f)) {
        func_8082E12C(this, this->linearVelocity);
        if (this->linearVelocity > 4.0f) {
            this->stateFlags2 |= 8;
        }
        if (sp24) {
            this->actor.shape.unk_17 = 1;
        } else {
            this->actor.shape.unk_17 = 2;
        }
    }

    this->unk_B38 += arg1;
    if (this->unk_B38 < 0.0f) {
        this->unk_B38 += 29.0f;
    } else if (this->unk_B38 >= 29.0f) {
        this->unk_B38 -= 29.0f;
    }
}
#else
void func_8083EA44(Player* this, f32 arg1);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8083EA44.s")
#endif

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8083EBD0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8083EE60.s")

void func_8084A794(Player* this, PlayState* play);
void func_8084A884(Player* this, PlayState* play);

void func_8083F144(Player* this, PlayState* play) {
    func_80831494(play, this, func_8084A794, 1);
    func_8082E438(play, this, &gameplay_keep_Linkanim_00DAC0);
}

s32 func_8083F190(Player* this, f32* arg1, s16* arg2, PlayState* play) {
    if (this->linearVelocity > 6.0f) {
        func_8083F144(this, play);
        return true;
    }

    if (*arg1 != 0.0f) {
        if (func_80832F24(this)) {
            *arg1 = 0.0f;
            *arg2 = this->currentYaw;
        } else {
            return true;
        }
    }
    return false;
}

void func_8083F230(Player* this, PlayState* play) {
    func_80831494(play, this, func_8084A884, 1);
    func_8082DB18(play, this, &gameplay_keep_Linkanim_00DAC8);
}

void func_8083F27C(PlayState* play, Player* this) {
    f32 temp_fv0;
    LinkAnimationHeader* sp38;
    LinkAnimationHeader* sp34;

    sp38 = GET_PLAYER_ANIM(PLAYER_ANIMGROUP_23, this->modelAnimType);
    sp34 = GET_PLAYER_ANIM(PLAYER_ANIMGROUP_24, this->modelAnimType);

    this->skelAnime.animation = sp38;

    func_8083EA44(this, (REG(30) / 1000.0f) + ((REG(32) / 1000.0f) * this->linearVelocity));

    temp_fv0 = this->unk_B38 * (16.0f / 29.0f);
    LinkAnimation_BlendToJoint(play, &this->skelAnime, sp34, temp_fv0, sp38, temp_fv0, this->unk_B40,
                               (void*)this->unk_88A);
}

void func_8083F358(Player* this, s32 arg1, PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8083F358.s")

void func_8083F57C(Player* this, PlayState* play) {
    f32 temp_fv0;
    f32 var_fs0;

    if (this->unk_B34 < 1.0f) {
        f32 temp_fs0;

        temp_fs0 = SREG(30) * 0.5f;
        func_8083EA44(this, REG(35) / 1000.0f);
        LinkAnimation_LoadToJoint(play, &this->skelAnime, GET_PLAYER_ANIM(PLAYER_ANIMGROUP_1, this->modelAnimType),
                                  this->unk_B38);

        // required
        this->unk_B34 += 1 * temp_fs0;
        if (this->unk_B34 >= 1.0f) {
            this->unk_B34 = 1.0f;
        }
        var_fs0 = this->unk_B34;
    } else {
        temp_fv0 = (this->linearVelocity - (REG(48) / 100.0f));
        if (temp_fv0 < 0.0f) {
            var_fs0 = 1.0f;
            func_8083EA44(this, (REG(35) / 1000.0f) + ((REG(36) / 1000.0f) * this->linearVelocity));
            func_8083F358(this, 0, play);
        } else {
            var_fs0 = (REG(37) / 1000.0f) * temp_fv0;
            if (var_fs0 < 1.0f) {
                func_8083EA44(this, (REG(35) / 1000.0f) + ((REG(36) / 1000.0f) * this->linearVelocity));
            } else {
                var_fs0 = 1.0f;
                func_8083EA44(this, (REG(39) / 100.0f) + ((REG(38) / 1000.0f) * temp_fv0));
            }
            func_8083F358(this, 1, play);
            LinkAnimation_LoadToJoint(play, &this->skelAnime, func_8082EEE0(this), this->unk_B38 * (20.0f / 29.0f));
        }
    }

    if (var_fs0 < 1.0f) {
        LinkAnimation_InterpJointMorph(play, &this->skelAnime, 1.0f - var_fs0);
    }
}

void func_8083F828(Vec3f* arg0, Vec3f* arg1, f32 arg2, f32 arg3, f32 arg4) {
    arg1->x = randPlusMinusPoint5Scaled(arg3) + arg0->x;
    arg1->y = randPlusMinusPoint5Scaled(arg4) + (arg0->y + arg2);
    arg1->z = randPlusMinusPoint5Scaled(arg3) + arg0->z;
}

s32 func_8083F8A8(PlayState* play, Player* this, f32 arg2, s32 arg3, f32 arg4, s32 arg5, s32 arg6, s32 arg7);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8083F8A8.s")

extern Color_RGBA8 D_8085D26C;
extern Vec3f D_8085D270;

#ifdef NON_MATCHING
s32 func_8083FBC4(PlayState* arg0, Player* arg1) {
    Vec3f* var_s0;
    s32 var_s1;
    s32 i;

    if ((arg1->unk_B72 == 0) || (arg1->unk_B72 == 1)) {
        var_s0 = &arg1->actor.shape.feetPos[0];
        i = 0;
        do {
            func_800B1210(arg0, var_s0, &gZeroVec3f, &gZeroVec3f, 0x32, 0x1E);
            i += 1;
            var_s0 += 1;
        } while (i != 2);
        return 1;
    }

    if (arg1->unk_B72 == 0xE) {
        var_s0 = &arg1->actor.shape.feetPos[0];
        i = 0;
        do {
            EffectSsDust_Spawn(arg0, 0, var_s0, &gZeroVec3f, &D_8085D270, &D_8085D26C, &D_8085D26C, 0x64, 0x28, 0x11,
                               0);
            i += 1;
            var_s0 += 1;
        } while (i != 2);
        return 1;
    }

    return 0;
}
#else
s32 func_8083FBC4(PlayState* arg0, Player* arg1);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8083FBC4.s")
#endif

s32 func_8083FCF0(PlayState* play, Player* this, f32 arg2, f32 arg3, f32 arg4) {
    if (arg4 < this->skelAnime.curFrame) {
        func_8082DC38(this);
    } else if (arg2 <= this->skelAnime.curFrame) {
        this->stateFlags3 |= PLAYER_STATE3_2000000;
        func_8082FA5C(play, this, (arg3 <= this->skelAnime.curFrame) ? 1 : -1);
        return true;
    }
    return false;
}

s32 func_8083FD80(Player* this, PlayState* arg1) {
    if (!Player_IsGoronOrDeku(this) && (Player_GetMeleeWeaponHeld(this) != 0) &&
        (this->transformation != PLAYER_FORM_ZORA) && (D_80862B48 != 0)) {
        func_8083375C(this, PLAYER_MWA_STAB_1H);
        func_8082DB18(arg1, this, &gameplay_keep_Linkanim_00DBE8);
        this->unk_AE7 = 1;
        this->meleeWeaponAnimation = PLAYER_MWA_STAB_1H;
        this->currentYaw = this->actor.shape.rot.y + this->unk_AB2.y;
        this->unk_ADD = 0;
        return true;
    }
    return false;
}

s32 func_8083FE38(Player* this, PlayState* play) {
    return func_80838A90(this, play) || func_808391D8(this, play) || func_8083D23C(this, play);
}

void func_8083FE90(PlayState* play, Player* this, u16 sfxId) {
    func_80836EA0(play, 0x6C77, 7, 20);
    Player_RequestRumble(play, this, 255, 20, 150, SQ(0));
    func_800B8E58(this, sfxId);
}

void func_8083FEF4(PlayState* play, Player* this) {
    Inventory_ChangeAmmo(ITEM_STICK, -1);
    func_80831990(play, this, ITEM_NONE);
}

s32 func_8083FF30(PlayState* play, Player* this) {
    if ((this->itemActionParam == PLAYER_AP_STICK) && (this->unk_B08[1] > 0.5f)) {
        if (AMMO(ITEM_STICK) != 0) {
            EffectSsStick_Spawn(play, &this->bodyPartsPos[0xF], BINANG_ADD(this->actor.shape.rot.y, 0x8000));
            this->unk_B08[1] = 0.5f;
            func_8083FEF4(play, this);
            func_800B8E58(this, NA_SE_IT_WOODSTICK_BROKEN);
        }
        return true;
    }

    return false;
}

// handles razor sword health and breaking
s32 func_8083FFEC(PlayState* play, Player* this) {
    if (this->itemActionParam == PLAYER_AP_SWORD_RAZOR) {
        if (gSaveContext.save.playerData.swordHealth > 0) {
            gSaveContext.save.playerData.swordHealth--;
            if (gSaveContext.save.playerData.swordHealth <= 0) {
                Item_Give(play, ITEM_SWORD_KOKIRI);
                func_80831990(play, this, ITEM_SWORD_KOKIRI);
                func_800B8E58(this, NA_SE_IT_MAJIN_SWORD_BROKEN);
                if (Message_GetState(&play->msgCtx) == TEXT_STATE_NONE) {
                    Message_StartTextbox(play, 0xF9, NULL);
                }
            }
        }
        return true;
    }
    return false;
}

s32 func_80840094(PlayState* play, Player* this) {
    func_8083FF30(play, this);
    return func_8083FFEC(play, this);
}

void func_8084B5C0(Player* this, PlayState* play);
void func_80854C70(Player* this, PlayState* play);

extern LinkAnimationHeader* D_8085D294[];

void func_808400CC(PlayState* play, Player* this) {
    if (func_8084B5C0 != this->unk_748) {
        func_8082DD2C(play, this);
        if ((this->transformation != PLAYER_FORM_HUMAN) && (this->transformation != PLAYER_FORM_FIERCE_DEITY)) {
            u8 moveFlags = this->skelAnime.moveFlags;
            s32 pad;

            this->skelAnime.moveFlags = 0;
            func_80831494(play, this, func_80854C70, 0);
            this->skelAnime.moveFlags = moveFlags;
        } else {
            s32 var_v1;
            s32 pad;

            func_80831494(play, this, func_80854C70, 0);
            if (func_80123420(this)) {
                var_v1 = 2;
            } else {
                var_v1 = 0;
            }
            func_8082DB90(play, this, D_8085D294[Player_IsHoldingTwoHandedWeapon(this) + var_v1]);
        }
    }

    Player_RequestRumble(play, this, 180, 20, 100, SQ(0));
    this->linearVelocity = -18.0f;
    func_80840094(play, this);
}

s32 func_808401F4(PlayState* play, Player* this);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_808401F4.s")

void func_80840770(PlayState* play, Player* this);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80840770.s")

void func_80840980(Player* this, u16 arg1) {
    func_8082DF8C(this, arg1);
}

void func_808409A8(PlayState* play, Player* this, f32 speedXZ, f32 yVelocity) {
    Actor* heldActor = this->heldActor;

    if (!func_808313A8(play, this, heldActor)) {
        heldActor->world.rot.y = this->actor.shape.rot.y;
        heldActor->speedXZ = speedXZ;
        heldActor->velocity.y = yVelocity;
        func_808309CC(play, this);
        func_800B8E58(this, NA_SE_PL_THROW);
        func_8082DF8C(this, NA_SE_VO_LI_SWORD_N);
    }
}

s32 func_80840A30(PlayState* play, Player* this, f32* arg2, f32 arg3);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80840A30.s")

extern u8 D_8085CF80[]; // PlayerMeleeWeaponAnimation
extern u8 D_8085CF84[]; // PlayerMeleeWeaponAnimation
#if 0
u8 D_8085CF80[] = {
    PLAYER_MWA_SPIN_ATTACK_1H,
    PLAYER_MWA_SPIN_ATTACK_2H,
};
u8 D_8085CF84[] = {
    PLAYER_MWA_BIG_SPIN_1H,
    PLAYER_MWA_BIG_SPIN_2H,
};
#endif

s32 func_80840CD4(Player* this, PlayState* arg1) {
    if (func_808387A0(arg1, this)) {
        this->stateFlags2 |= PLAYER_STATE2_20000;
    } else if (!CHECK_BTN_ALL(D_80862B44->cur.button, BTN_B)) {
        PlayerMeleeWeaponAnimation meleeWeaponAnimation;

        if ((this->unk_B08[0] >= 0.85f) || func_808333CC(this)) {
            meleeWeaponAnimation = D_8085CF84[Player_IsHoldingTwoHandedWeapon(this)];
        } else {
            meleeWeaponAnimation = D_8085CF80[Player_IsHoldingTwoHandedWeapon(this)];
        }
        func_80833864(arg1, this, meleeWeaponAnimation);
        func_808339B4(this, -8);
        this->stateFlags2 |= PLAYER_STATE2_20000;
        if (this->unk_AE3[this->unk_ADE] == 0) {
            this->stateFlags2 |= PLAYER_STATE2_40000000;
        }
    } else {
        return false;
    }

    return true;
}

void func_8084CE84(Player* this, PlayState* play);
void func_8084D18C(Player* this, PlayState* play);

void func_80840DEC(Player* this, PlayState* play) {
    func_80831494(play, this, func_8084CE84, 1);
}

void func_80840E24(Player* this, PlayState* play) {
    func_80831494(play, this, func_8084D18C, 1);
}

void func_80840E5C(Player* this, PlayState* play) {
    func_808369F4(this, play);
    func_8082DC38(this);
    func_8082E438(play, this, D_8085CF68[Player_IsHoldingTwoHandedWeapon(this)]);
    this->currentYaw = this->actor.shape.rot.y;
}

void func_80840EC0(Player* this, PlayState* play) {
    func_80831494(play, this, func_8084CCEC, 1);
    func_8082DB3C(play, this, D_8085CF60[Player_IsHoldingTwoHandedWeapon(this)]);
    this->unk_AE8 = 1;
    this->unk_B38 = 0.0f;
}

void func_80840F34(Player* this) {
    Math_StepToF(&this->unk_B08[0], (gSaveContext.save.weekEventReg[23] & 2) ? 1.0f : 0.5f, 0.02f);
}

s32 func_80840F90(PlayState* play, Player* this, UNK_PTR arg2, f32 arg3, UNK_TYPE arg4, UNK_TYPE arg5);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80840F90.s")

s32 func_808411D4(PlayState* play, Player* this, f32* arg2, s32 arg3) {
    f32 xDiff = this->unk_3A0.x - this->actor.world.pos.x;
    f32 yDiff = this->unk_3A0.z - this->actor.world.pos.z;
    s32 sp2C;
    s32 pad2;
    s16 var_v1;

    sp2C = sqrtf(SQ(xDiff) + SQ(yDiff));
    var_v1 = Math_Vec3f_Yaw(&this->actor.world.pos, &this->unk_3A0);
    if (sp2C < arg3) {
        *arg2 = 0.0f;
        var_v1 = this->actor.shape.rot.y;
    }
    if (func_80840F90(play, this, NULL, *arg2, var_v1, 2) != 0) {
        return 0;
    }
    return sp2C;
}

void func_808412A0(PlayState* play, Player* this) {
    this->actor.update = func_801229EC;
    this->actor.draw = NULL;
}

void func_80854118(Player*, PlayState* play);

void func_808412BC(PlayState* play, Player* this) {
    func_80831494(play, this, func_80854118, 0);
    this->stateFlags1 |= 0x20000000;
    LinkAnimation_Change(play, &this->skelAnime, &gameplay_keep_Linkanim_00DF78, 0.6666667f, 0.0f, 24.0f, (u8)2, 0.0f);
    this->actor.world.pos.y += 800.0f;
}

extern u8 D_8085D2B0[];
#if 0
u8 D_8085D2B0[] = {
    ITEM_SWORD_RAZOR, ITEM_SWORD_KOKIRI, ITEM_OCARINA, ITEM_OCARINA,
};
#endif

void func_80841358(PlayState* arg0, Player* this, s32 arg2) {
    ItemID item;
    PlayerActionParam actionParam;

    //! @bug OoB read if player is human
    item = D_8085D2B0[this->transformation];
    actionParam = D_8085C99C[item];
    func_808317C4(this);
    func_8082DCA0(arg0, this);
    this->heldItemId = item;
    this->nextModelGroup = Player_ActionToModelGroup(this, actionParam);
    func_8082F8BC(arg0, this, actionParam);
    func_808309CC(arg0, this);
    if (arg2) {
        func_800B8E58(this, NA_SE_IT_SWORD_PICKOUT);
    }
}

void func_80852FD4(Player* this, PlayState* play);
void func_8085437C(Player* this, PlayState* play);
void func_8085439C(Player* this, PlayState* play);

extern Vec3f D_8085D2B4;

void func_80841408(PlayState* play, Player* this) {
    func_80831494(play, this, func_80852FD4, 0);
    this->stateFlags1 |= PLAYER_STATE1_20000000;
    Math_Vec3f_Copy(&this->actor.world.pos, &D_8085D2B4);
    this->currentYaw = this->actor.shape.rot.y = -0x8000;
    LinkAnimation_Change(play, &this->skelAnime, this->ageProperties->unk_A8, 2.0f / 3.0f, 0.0f, 0.0f, 2, 0.0f);
    func_8082E920(play, this, (1 | 2 | 4 | 8 | 0x80 | 0x200));
    if (this->transformation == PLAYER_FORM_FIERCE_DEITY) {
        func_80841358(play, this, false);
    }
    this->unk_AE8 = 20;
}

void func_808414E0(PlayState* play, Player* this) {
    func_80831494(play, this, func_8085437C, 0);
    func_8082E920(play, this, (1 | 2 | 8 | 0x10 | 0x80));
}

void func_80834DB8(Player* this, LinkAnimationHeader* anim, f32 arg2, PlayState* play);

void func_80841528(PlayState* play, Player* this) {
    func_80834DB8(this, &gameplay_keep_Linkanim_00DCD8, 12.0f, play);
    func_80831494(play, this, func_8085439C, 0);
    this->stateFlags1 |= PLAYER_STATE1_20000000;
    this->unk_B68 = this->actor.world.pos.y;
}

void func_808540A0(Player*, PlayState* play);
void func_808496AC(Player*, PlayState* play);
void func_80855C28(Player*, PlayState* play);
void func_808540A0(Player*, PlayState* play);

extern LinkAnimationHeader* D_8085D17C[PLAYER_FORM_MAX];
extern LinkAnimationHeader* D_8085D190[PLAYER_FORM_MAX];

void func_808415A0(PlayState* play, Player* this) {
    func_80833B18(play, this, 1, 2.0f, 2.0f, this->actor.shape.rot.y + 0x8000, 0);
}

void func_808415E4(PlayState* play, Player* this) {
    func_80831494(play, this, func_808540A0, 0);
    this->actor.draw = NULL;
    this->stateFlags1 |= PLAYER_STATE1_20000000;
}

void func_80841624(PlayState* play, Player* this) {
    if (gSaveContext.save.isOwlSave) {
        func_80831494(play, this, func_808496AC, 0);
        func_8082E514(play, this, GET_PLAYER_ANIM(PLAYER_ANIMGROUP_43, this->modelAnimType));
        this->stateFlags1 |= PLAYER_STATE1_20000000;
        this->unk_AE8 = 0x28;
        gSaveContext.save.isOwlSave = false;
    } else {
        func_80831494(play, this, func_80849FE0, 0);
        func_8082E514(play, this, GET_PLAYER_ANIM(PLAYER_ANIMGROUP_43, this->modelAnimType));
        this->stateFlags1 |= PLAYER_STATE1_20000000;
        this->stateFlags2 |= PLAYER_STATE2_20000000;
        Actor_Spawn(&play->actorCtx, play, ACTOR_EN_TEST7, this->actor.world.pos.x, this->actor.world.pos.y,
                    this->actor.world.pos.z, 0, 0, 0, ENTEST7_MINUS1);
    }
}

void func_80841744(PlayState* play, Player* this) {
    func_80831494(play, this, func_80855C28, 0);
    if (((s32)(this->actor.params & 0xF00) >> 8) == 8) {
        func_8082DBC0(play, this, D_8085D17C[this->transformation]);
        this->heldItemActionParam = PLAYER_AP_OCARINA;
        Player_SetModels(this, Player_ActionToModelGroup(this, this->heldItemActionParam));
    } else {
        func_8082DB60(play, this, GET_PLAYER_ANIM(PLAYER_ANIMGROUP_43, this->modelAnimType));
    }
    this->stateFlags1 |= PLAYER_STATE1_20000000;
    this->unk_ABC = -10000.0f;
    this->unk_AE8 = 0x2710;
    this->unk_B08[7] = 8.0f;
}

extern InitChainEntry D_8085D2C0[];

extern Vec3s D_8085D2C4;
extern Vec3s D_8085D2C4;

extern ColliderCylinderInit D_8085C2EC;
extern ColliderCylinderInit D_8085C318;
extern ColliderQuadInit D_8085C344;
extern ColliderQuadInit D_8085C394;
extern FlexSkeletonHeader D_060177B8;

void Player_InitCommon(Player* this, PlayState* play, FlexSkeletonHeader* skelHeader) {
    Actor_ProcessInitChain(&this->actor, D_8085D2C0);
    this->currentYaw = this->actor.world.rot.y;

    if ((((this->actor.params & 0xF00) >> 8) != 0xC) &&
        ((gSaveContext.respawnFlag != 2) || (gSaveContext.respawn[1].playerParams != 0xCFF))) {
        func_808309CC(play, this);
        SkelAnime_InitLink(play, &this->skelAnime, skelHeader, GET_PLAYER_ANIM(PLAYER_ANIMGROUP_0, this->modelAnimType),
                           9, (void*)this->unk_74C, (void*)this->unk_7EB, 0x16);
        this->skelAnime.baseTransl = D_8085D2C4;

        SkelAnime_InitLink(play, &this->unk_284, skelHeader, func_8082ED20(this), 9, (void*)this->unk_929,
                           (void*)this->unk_9C8, 0x16);
        this->unk_284.baseTransl = D_8085D2C4;

        if (this->transformation == 1) {
            SkelAnime_InitFlex(play, &this->unk_2C8, &D_060177B8, &D_060178D0, this->jointTable, this->morphTable, 5);
        }

        ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawFeet, this->ageProperties->unk_04);
    }

    this->unk_3BC = -1;
    Collider_InitAndSetCylinder(play, &this->cylinder, &this->actor, &D_8085C2EC);
    Collider_InitAndSetCylinder(play, &this->shieldCylinder, &this->actor, &D_8085C318);
    Collider_InitAndSetQuad(play, &this->meleeWeaponQuads[0], &this->actor, &D_8085C344);
    Collider_InitAndSetQuad(play, &this->meleeWeaponQuads[1], &this->actor, &D_8085C344);
    Collider_InitAndSetQuad(play, &this->shieldQuad, &this->actor, &D_8085C394);
}

void func_80841A50(PlayState* play, Player* this) {
    if ((play->roomCtx.currRoom.num >= 0) && (play->roomCtx.prevRoom.num < 0)) {
        Math_Vec3f_Copy(&this->unk_3C0, &this->actor.world.pos);
        this->unk_3CC = this->actor.shape.rot.y;
        this->unk_3CE = play->roomCtx.currRoom.num;
        this->unk_3CF = 1;
    }
}

#if 0
void Player_Init(Actor* thisx, PlayState* play) {
    s32 sp60;
    s8* sp44;
    PosRot* sp40;
    PosRot* temp_a0_2;
    RespawnData* temp_t8;
    RespawnData* temp_t9_2;
    SaveContext* var_v1;
    s16 temp_v0;
    s16 temp_v0_6;
    s16 temp_v1_3;
    s32 temp_v0_7;
    s32 temp_v1;
    s32 var_a1;
    s32 var_a2;
    s32 var_v0_2;
    s32 var_v0_3;
    s8 temp_v0_2;
    s8* temp_a0;
    s8* temp_v1_2;
    u32 temp_t7;
    u32 temp_t9;
    u8 temp_t1;
    u8 temp_t6;
    u8 temp_v0_4;
    u8 temp_v0_5;
    u8 var_v0;
    void* temp_v0_3;
    Player* this = (Player* ) thisx;

    play->playerInit = Player_InitCommon;
    play->playerUpdate = func_80844EF8;
    play->unk_18770 = func_8085B170;
    play->startPlayerFishing = func_8085B134;
    play->grabPlayer = func_8085B1F0;
    play->startPlayerCutscene = func_8085B28C;
    play->func_18780 = func_8085B384;
    play->damagePlayer = func_8085B3E0;
    play->talkWithPlayer = func_8085B460;
    play->unk_1878C = func_8085B74C;
    play->unk_18790 = func_8085B820;
    play->unk_18794 = func_8085B854;
    play->setPlayerTalkAnim = func_8085B930;

    gActorOverlayTable[0].initInfo->objectId = 1;
    temp_v0 = this->actor.shape.rot.x;
    this->actor.room = -1;
    this->unk_A86 = -1;
    temp_t1 = temp_v0 - 1;
    if (temp_v0 != 0) {
        this->transformation = temp_t1;
        temp_v0_2 = Object_GetIndex(&play->objectCtx, gPlayerFormObjectIndices[temp_t1 & 0xFF]);
        this->actor.objBankIndex = temp_v0_2;
        if (temp_v0_2 < 0) {
            Actor_MarkForDeath(&this->actor);
            return;
        }
        Actor_SetObjectDependency(play, &this->actor);
    } else {
        temp_t6 = gSaveContext.save.playerForm;
        temp_v1 = temp_t6 & 0xFF;
        this->transformation = temp_t6;
        if (temp_v1 == 4) {
            var_v0 = gSaveContext.save.equippedMask;
            if (var_v0 == 0x14) {
                gSaveContext.save.equippedMask = 0;
                var_v0 = 0;
            }
            this->currentMask = var_v0;
        } else {
            this->currentMask = temp_v1 + 0x15;
            gSaveContext.save.equippedMask = 0;
        }
        Inventory_UpdateDeitySwordEquip(play);
        this->unk_B28 = 0;
        this->unk_B90 = 0;
        this->unk_B92 = 0;
        this->unk_B94 = 0;
        this->unk_B96 = 0;
        this->stateFlags1 &= 0xFCFFEFF7;
        this->stateFlags2 &= 0xBEFDFFFF;
        this->stateFlags3 &= 0xFC424437;
        this->unk_B08[0] = 0.0f;
        this->unk_B08[1] = 0.0f;
    }
    if (this->transformation == 2) {
        if ((this->stateFlags1 * 0x10) < 0) {
            this->unk_B08[2] = 1.0f;
        } else {
            this->unk_B08[2] = 0.0f;
        }
    }
    this->actor.flags &= 0xFBFDFFFF;
    if (this->transformation != 3) {
        this->actor.flags |= 0x04000000;
        if (this->transformation == 1) {
            this->actor.flags |= 0x20000;
        }
    }
    this->ageProperties = &D_8085BA38[this->transformation];
    this->heldItemActionParam = 0;
    this->itemActionParam = 0;
    this->heldItemId = 0xFF;
    // todo: declaration
    func_80831990(play, &this->actor, 0xFFU);
    Player_SetEquipmentData(play, this);
    this->prevBoots = this->currentBoots;
    Player_InitCommon(&this->actor, play, gPlayerSkeletons[this->transformation]);
    if (this->actor.shape.rot.z != 0) {
        this->actor.shape.rot.z = 0;
        // todo: declaration
        func_8082F938((Player* ) play, this, 0, 4);
#if 0
        temp_v0_3 = Effect_GetByIndex(this->meleeWeaponEffectIndex[2]);
        if (this->transformation == 1) {
            temp_v0_3->unk_606 = (unaligned s32) D_8085D338;
        } else {
            temp_v0_3->unk_606 = (unaligned s32) D_8085D33C;
        }
#endif
        if ((this->csMode == 9) || (this->csMode == 0x5D)) {
            // todo: declaration
            func_80831494(play, this, func_8085B08C, 0);
            this->stateFlags1 |= 0x20000000;
            return;
        }
        // todo: declaration
        func_80831494(play, this, func_80855818, 0);
        this->actor.shape.rot.y = this->currentYaw;
        if (this->prevMask != 0) {
            // todo: declaration
            func_8082DB90(play, this, &D_0400D0A8);
        } else {
            if (this->transformation == 4) {
                LinkAnimation_Change(play, &this->skelAnime, D_8085D160[this->transformation], -0.6666667f, 9.0f, 0.0f, (u8) 2, 0.0f);
            } else {
                // todo: declaration
                func_8082DB60(play, this, &D_0400D0D0);
            }
        }
        this->stateFlags1 |= 0x30000000;
        this->stateFlags3 |= 0x20000;
        this->unk_B08[7] = 3.0f;
        return;
    }
    this->prevMask = this->currentMask;
    Effect_Add(play, this->meleeWeaponEffectIndex, 2, 0U, (u8) 0, &D_8085D30C);
    Effect_Add(play, &this->meleeWeaponEffectIndex[1], 2, 0U, (u8) 0, &D_8085D30C);
    // todo: declaration
    func_8082F938((Player* ) play, this, 0, 4);
#if 0
    if (this->transformation == 1) {
        *(&D_8085D330 + 4) = (unaligned s32) D_8085D338;
    } else {
        *(&D_8085D330 + 4) = (unaligned s32) D_8085D33C;
    }
#endif
    Effect_Add(play, &this->meleeWeaponEffectIndex[2], 4, 0U, (u8) 0, &D_8085D330);
    if (this->actor.shape.rot.x != 0) {
        this->actor.shape.rot.x = 0;
        this->csMode = 0x44;
        // todo: declaration
        func_80831494(play, this, func_8085B08C, 0);
        this->stateFlags1 |= 0x20000000;
        return;
    }
    play->unk_1887C = 0;
    play->unk_1887D = 0;
    play->unk_1887E = 0;
    this->giObjectSegment = ZeldaArena_Malloc(0x2000U);
    this->maskObjectSegment = ZeldaArena_Malloc(0x3800U);
    temp_a0 = &this->unk_404[0x24];
    Lights_PointNoGlowSetInfo((LightInfo* ) temp_a0, (s16) (s32) this->actor.world.pos.x, (s16) (s32) this->actor.world.pos.y, (s16) (s32) this->actor.world.pos.z, (u8) 0xFF, (u8) 0x80, (u8) 0, (s16) -1);
#if 0
    this->unk_504 = LightContext_InsertLight(play, &play->lightCtx, (LightInfo* ) temp_a0);
#endif
    Play_AssignPlayerActorCsIdsFromScene(&play->state, (s32) this->actor.cutscene);
    var_a2 = gSaveContext.respawnFlag;
    if (var_a2 != 0) {
        if (var_a2 == -3) {
            this->actor.params = gSaveContext.respawn[3].playerParams;
        } else {
            if ((var_a2 == 1) || (var_a2 == -1)) {
                this->unk_D6A = -2;
            }
            if (var_a2 != -7) {
                temp_a0_2 = &this->actor.world;
                if ((var_a2 == -8) || (var_a2 == -5) || (var_a2 == -4)) {
                    var_a2 = 1;
                }
                if ((var_a2 < 0) && (var_a2 != -1) && (var_a2 != -6)) {
                    var_v1 = &gSaveContext;
                } else {
                    var_v0_2 = var_a2 - 1;
                    if (var_a2 < 0) {
                        var_v0_2 = 2;
                    }
                    temp_v1_2 = &gSaveContext + (var_v0_2 << 5);
                    sp40 = temp_a0_2;
                    Math_Vec3f_Copy(&temp_a0_2->pos, &gSaveContext.respawn[var_v0_2].pos);
                    Math_Vec3f_Copy(&this->actor.home.pos, &sp40->pos);
                    Math_Vec3f_Copy(&this->actor.prevPos, &sp40->pos);
                    Math_Vec3f_Copy(&this->actor.focus.pos, &sp40->pos);
                    var_v1 = (SaveContext* ) temp_v1_2;
                    this->unk_B68 = (s16) (s32) this->actor.world.pos.y;
                    temp_v0_6 = var_v1->respawn[0].yaw;
                    this->actor.shape.rot.y = temp_v0_6;
                    this->currentYaw = temp_v0_6;
                    this->actor.params = var_v1->respawn[0].playerParams;
                }
                play->actorCtx.flags.switches[2] = var_v1->respawn[0].tempSwitchFlags;
                play->actorCtx.flags.collectible[1] = var_v1->respawn[0].unk_18;
                play->actorCtx.flags.collectible[2] = var_v1->respawn[0].tempCollectFlags;
            }
        }
    }
    if ((var_a2 == 4) || (var_a1 = 0, (gSaveContext.respawnFlag == -4))) {
        var_a1 = 1;
    }
    if (func_801226E0(play, var_a1) == 0) {
        gSaveContext.respawn[0].playerParams = (this->actor.params & 0xFF) | 0xD00;
    }
    gSaveContext.respawn[0].data = 1;
    if (var_a2 == 0) {
        temp_t9_2 = gSaveContext.respawn;
        temp_t8 = &gSaveContext.respawn[2];
        temp_t8->pos.x = temp_t9_2->pos.x;
        temp_t8->pos.y = temp_t9_2->pos.y;
        temp_t8->pos.z = temp_t9_2->pos.z;
        temp_t8->yaw = (s32) temp_t9_2->yaw;
        temp_t8->entranceIndex = (s32) temp_t9_2->entranceIndex;
        temp_t8->tempSwitchFlags = temp_t9_2->tempSwitchFlags;
        temp_t8->unk_18 = temp_t9_2->unk_18;
        temp_t8->tempCollectFlags = temp_t9_2->tempCollectFlags;
    }
    gSaveContext.respawn[2].playerParams = (gSaveContext.respawn[2].playerParams & 0xFF) | 0xD00;
    var_v0_3 = (s32) (this->actor.params & 0xF00) >> 8;
    if (((var_v0_3 == 5) || (var_v0_3 == 6)) && (gSaveContext.save.cutscene >= 0xFFF0)) {
        var_v0_3 = 0xD;
    }
    D_8085D2CC[var_v0_3](play, this, var_a2);
    if ((this->actor.draw != NULL) && (gSaveContext.save.hasTatl != 0)) {
        if ((((gSaveContext.gameMode == 0)) || (gSaveContext.gameMode == 3)) && (play->sceneNum != 8)) {
            // todo: declaration
            this->tatlActor = func_80835C64(play, &this->actor, &this->actor.world.pos, &D_8085D340, 0);
            temp_v1_3 = (s16) gSaveContext.dogParams;
            if (temp_v1_3 != 0) {
                gSaveContext.dogParams = temp_v1_3 | 0x8000;
            }
            if (gSaveContext.powderKegTimer != 0) {
                this->nextModelGroup = Player_ActionToModelGroup(this, PLAYER_AP_POWDER_KEG);
                this->heldItemId = 0xC;
                func_8082F8BC(play, this, PLAYER_AP_POWDER_KEG);
                // todo: declaration
                func_808313F0(this, play);
            } else if (gSaveContext.unk_1014 != 0) {
                // todo: declaration
                func_8082F5FC(this, Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, 0xB9, this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z, (s16) 0, (s16) (s32) this->actor.shape.rot.y, (s16) 0, 0x8000));
            // todo: declaration
                func_808313F0(this, play);
            }
        }
    }
    Minimap_SavePlayerRoomInitInfo(play);
    // todo: declaration
    func_80841A50(play, this);
    this->unk_3CF = 0;
    MREG(64) = 0;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/Player_Init.s")
#endif

void func_80842510(s16* arg0) {
    s16 temp_ft0;

    temp_ft0 = (ABS_ALT(*arg0) * 100.0f) / 1000.0f;
    temp_ft0 = CLAMP(temp_ft0, 0x190, 0xFA0);

    Math_ScaledStepToS(arg0, 0, temp_ft0);
}

void func_808425B4(Player* this) {
    if (!(this->unk_AA6 & 2)) {
        s16 sp26 = this->actor.focus.rot.y - this->actor.shape.rot.y;

        func_80842510(&sp26);
        this->actor.focus.rot.y = this->actor.shape.rot.y + sp26;
    }
    if (!(this->unk_AA6 & 1)) {
        func_80842510(&this->actor.focus.rot.x);
    }
    if (!(this->unk_AA6 & 8)) {
        func_80842510(&this->unk_AAC.x);
    }
    if (!(this->unk_AA6 & 0x40)) {
        func_80842510(&this->unk_AB2.x);
    }
    if (!(this->unk_AA6 & 4)) {
        func_80842510(&this->actor.focus.rot.z);
    }
    if (!(this->unk_AA6 & 0x10)) {
        func_80842510(&this->unk_AAC.y);
    }
    if (!(this->unk_AA6 & 0x20)) {
        func_80842510(&this->unk_AAC.z);
    }
    if (!(this->unk_AA6 & 0x80)) {
        if (this->unk_AA8 != 0) {
            func_80842510(&this->unk_AA8);
        } else {
            func_80842510(&this->unk_AB2.y);
        }
    }
    if (!(this->unk_AA6 & 0x100)) {
        func_80842510(&this->unk_AB2.z);
    }

    this->unk_AA6 = 0;
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_808426F0.s")

s32 func_808430E0(Player* this) {
    if ((this->transformation == PLAYER_FORM_DEKU) && (this->actor.bgCheckFlags & 1) && (func_8083784C(this) != 0)) {
        this->actor.bgCheckFlags &= ~1;
    }
    if (this->actor.bgCheckFlags & 1) {
        return false;
    }

    if (!(this->stateFlags1 & PLAYER_STATE1_8000000)) {
        D_80862B08 = 0;
    }
    this->unk_B6C = 0;
    this->unk_B6E = 0;
    D_80862B28 = 0;
    return true;
}

void func_80843178(PlayState* play, Player* this);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80843178.s")

void Player_UpdateCamAndSeqModes(PlayState* play, Player* this) {
    u8 seqMode;
    s32 pad[2];
    Camera* camera;
    s32 camMode;

    if (this == GET_PLAYER(play)) {
        seqMode = SEQ_MODE_DEFAULT;
        if (this->stateFlags1 & PLAYER_STATE1_100000) {
            seqMode = SEQ_MODE_STILL;
        } else if (this->csMode != 0) {
            Camera_ChangeMode(Play_GetCamera(play, CAM_ID_MAIN), CAM_MODE_NORMAL);
        } else {
            camera = (this->actor.id == ACTOR_PLAYER) ? Play_GetCamera(play, CAM_ID_MAIN)
                                                      : Play_GetCamera(play, ((EnTest3*)this)->unk_D8E);
            if ((this->actor.parent != NULL) && (this->stateFlags3 & PLAYER_STATE3_80)) {
                camMode = CAM_MODE_HOOKSHOT;
                Camera_SetViewParam(camera, CAM_VIEW_TARGET, this->actor.parent);
            } else if (func_8084BC64 == this->unk_748) {
                camMode = CAM_MODE_STILL;
            } else if (this->stateFlags3 & PLAYER_STATE3_8000) {
                if (this->stateFlags1 & PLAYER_STATE1_8000000) {
                    camMode = CAM_MODE_GORONDASH;
                } else {
                    camMode = CAM_MODE_FREEFALL;
                }
            } else if (this->stateFlags3 & PLAYER_STATE3_80000) {
                if (this->actor.bgCheckFlags & 1) {
                    camMode = CAM_MODE_GORONDASH;
                } else {
                    camMode = CAM_MODE_GORONJUMP;
                }
            } else if (this->stateFlags2 & PLAYER_STATE2_100) {
                camMode = CAM_MODE_PUSHPULL;
            } else if (this->unk_730 != NULL) {
                if ((this->actor.flags & ACTOR_FLAG_100) == ACTOR_FLAG_100) {
                    camMode = CAM_MODE_TALK;
                } else if (this->stateFlags1 & PLAYER_STATE1_10000) {
                    if (this->stateFlags1 & PLAYER_STATE1_2000000) {
                        camMode = CAM_MODE_FOLLOWBOOMERANG;
                    } else {
                        camMode = CAM_MODE_FOLLOWTARGET;
                    }
                } else {
                    camMode = CAM_MODE_BATTLE;
                }
                Camera_SetViewParam(camera, CAM_VIEW_TARGET, this->unk_730);
            } else if (this->stateFlags1 & PLAYER_STATE1_1000) {
                camMode = CAM_MODE_CHARGE;
            } else if (this->stateFlags3 & PLAYER_STATE3_100) {
                camMode = CAM_MODE_DEKUHIDE;
            } else if (this->stateFlags1 & PLAYER_STATE1_2000000) {
                camMode = CAM_MODE_FOLLOWBOOMERANG;
                Camera_SetViewParam(camera, CAM_VIEW_TARGET, this->boomerangActor);
            } else if (this->stateFlags1 & (PLAYER_STATE1_4 | PLAYER_STATE1_2000 | PLAYER_STATE1_4000)) {
                if (func_80123434(this)) {
                    camMode = CAM_MODE_HANGZ;
                } else {
                    camMode = CAM_MODE_HANG;
                }
            } else if ((this->stateFlags3 & PLAYER_STATE3_2000) && (this->actor.velocity.y < 0.0f)) {
                if (this->stateFlags1 & (PLAYER_STATE1_20000 | PLAYER_STATE1_40000000)) {
                    camMode = CAM_MODE_DEKUFLYZ;
                } else {
                    camMode = CAM_MODE_DEKUFLY;
                }
            } else if (this->stateFlags1 & (PLAYER_STATE1_20000 | PLAYER_STATE1_40000000)) {
                if (func_800B7128(this) || func_8082EF20(this)) {
                    camMode = CAM_MODE_BOWARROWZ;
                } else if (this->stateFlags1 & PLAYER_STATE1_200000) {
                    camMode = CAM_MODE_CLIMBZ;
                } else {
                    camMode = CAM_MODE_TARGET;
                }
            } else if ((this->stateFlags1 & PLAYER_STATE1_400000) && (this->transformation != 0)) {
                camMode = CAM_MODE_STILL;
            } else if (this->stateFlags1 & PLAYER_STATE1_40000) {
                camMode = CAM_MODE_JUMP;
            } else if (this->stateFlags1 & PLAYER_STATE1_200000) {
                camMode = CAM_MODE_CLIMB;
            } else if (this->stateFlags1 & PLAYER_STATE1_80000) {
                camMode = CAM_MODE_FREEFALL;
            } else if (((func_808548B8 == this->unk_748) &&
                        (this->meleeWeaponAnimation >= PLAYER_MWA_FORWARD_SLASH_1H) &&
                        (this->meleeWeaponAnimation <= PLAYER_MWA_ZORA_PUNCH_KICK)) ||
                       (this->stateFlags3 & PLAYER_STATE3_8) ||
                       ((func_8084FE7C == this->unk_748) && (this->unk_AE8 == 0)) || (func_808505D0 == this->unk_748)) {
                camMode = CAM_MODE_STILL;
            } else {
                camMode = CAM_MODE_NORMAL;
                if ((this->linearVelocity == 0.0f) &&
                    (!(this->stateFlags1 & PLAYER_STATE1_800000) || (this->rideActor->speedXZ == 0.0f))) {
                    seqMode = SEQ_MODE_STILL;
                }
            }

            Camera_ChangeMode(camera, camMode);
        }

        if (play->actorCtx.targetContext.bgmEnemy != NULL) {
            seqMode = SEQ_MODE_ENEMY;
            Audio_SetBgmEnemyVolume(sqrtf(play->actorCtx.targetContext.bgmEnemy->xyzDistToPlayerSq));
        }

        Audio_SetSequenceMode(seqMode);
    }
}

void func_808442D8(PlayState* play, Player* this) {
    f32 var_fa0;
    f32 temp_fv1;

    if (this->unk_B08[1] == 0.0f) {
        func_80831990(play, this, ITEM_NONE);
        return;
    }

    var_fa0 = 1.0f;
    if (DECR(this->unk_B28) == 0) {
        Inventory_ChangeAmmo(ITEM_STICK, -1);
        this->unk_B28 = 1;
        this->unk_B08[1] = 0.0f;
        var_fa0 = 0.0f;
    } else if (this->unk_B28 >= 0xC9) {
        var_fa0 = (0xD2 - this->unk_B28) / 10.0f;
    } else if (this->unk_B28 < 0x14) {
        var_fa0 = this->unk_B28 / 20.0f;
        this->unk_B08[1] = var_fa0;
    }

    if (var_fa0 > 0.0f) {
        func_800B0EB0(play, &this->meleeWeaponInfo[0].tip, &D_8085D364, &D_8085D370, &D_8085D37C, &D_8085D380,
                      (var_fa0 * 200.0f), 0, 8);
        if (((play->roomCtx.currRoom.enablePosLights != 0)) || (gGameInfo->data[0x23D] != 0)) {
            temp_fv1 = (Rand_ZeroOne() * 30.0f) + 225.0f;
            Lights_PointSetColorAndRadius(&this->lightInfo, temp_fv1, temp_fv1 * 0.7f, 0, var_fa0 * 300.0f);
        }
    }
}

void func_808445C4(PlayState* play, Player* this);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_808445C4.s")

void func_808446F4(PlayState* play, Player* this) {
    f32 var_fv0;

    var_fv0 = 200000.0f - (this->unk_AA0 * 5.0f);
    if (var_fv0 < 0.0f) {
        var_fv0 = 0.0f;
    }

    this->unk_A9C += var_fv0;
    if (this->unk_A9C > 4000000.0f) {
        this->unk_A9C = 0.0f;
        Player_RequestRumble(play, this, 120, 20, 10, SQ(0));
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80844784.s")

void func_80844D80(PlayState* play, Player* this) {
    Vec3f pos;
    Vec3f spA0;
    Vec3f velocity;
    Vec3f accel;
    Vec3f sp7C;
    s32 i;

    Math_Vec3f_Diff(&this->meleeWeaponInfo[0].tip, &this->meleeWeaponInfo[0].base, &sp7C);
    Math_Vec3f_SumScaled(&this->meleeWeaponInfo[0].base, &sp7C, 0.3f, &spA0);

    for (i = 0; i < 2; i++) {
        Math_Vec3f_SumScaled(&this->meleeWeaponInfo[0].base, &sp7C, Rand_ZeroOne(), &pos);
        Math_Vec3f_AddRand(&pos, 15.0f, &pos);
        Math_Vec3f_DistXYZAndStoreNormDiff(&spA0, &pos, 1.7f, &velocity);
        Math_Vec3f_ScaleAndStore(&velocity, 0.01f, &accel);
        EffectSsKirakira_SpawnDispersed(play, &pos, &velocity, &accel, &D_8085D3F4, &D_8085D3F8,
                                        Rand_S16Offset(-20, -120), 15);
    }
}

void func_8085AACC(PlayState* play, Player* this, s32 arg2);

#if 0
// Player_UpdateCommon
void func_80844EF8(Player* player, PlayState* play, Input* input) {
    s32 sp58;
    f32 sp50;
    f32 sp44;
    PosRot* sp38;
    CollisionCheckContext* sp34;
    ColliderCylinder* sp2C;                         /* compiler-managed */
    ColliderCylinder* sp28;
    PlayerFunc748 temp_v0_15;
    PlayerFunc748 temp_v0_21;
    PlayerFunc748 temp_v0_22;
    Actor* temp_v0_13;
    Actor* temp_v0_16;
    ColliderCylinder* temp_a1_3;
    ColliderCylinder* temp_a1_4;
    ColliderCylinder* temp_a1_5;
    CollisionCheckContext* temp_a1_6;
    CsCmdActorAction* temp_v0_14;
    LinkAnimationHeader* var_a2_2;
    LinkAnimationHeader* var_v0_2;
    PosRot* temp_a1;
    f32 temp_fa0;
    f32 temp_fa0_2;
    f32 temp_fv0;
    f32 temp_fv1;
    f32 var_fv0;
    f32 var_fv0_2;
    f32 var_fv0_3;
    f32 var_fv0_4;
    f32 var_fv0_5;
    f32 var_fv1;
    f32 var_fv1_2;
    s16 temp_v0_19;
    s16 temp_v0_9;
    s32 temp_v0_17;
    s32 var_a2;
    s32 var_v0;
    s32 var_v1;
    s8 temp_v0;
    s8 temp_v0_10;
    s8 temp_v0_11;
    s8 temp_v0_4;
    s8 temp_v0_8;
    s8 temp_v1_2;
    u16 temp_v0_6;
    u16 temp_v0_7;
    u16 temp_v1_7;
    u32 temp_a0;
    u32 temp_t7;
    u32 temp_v1_3;
    u32 temp_v1_4;
    u8 temp_a1_2;
    u8 temp_v0_12;
    u8 temp_v0_18;
    u8 temp_v0_20;
    u8 temp_v0_2;
    u8 temp_v0_3;
    u8 temp_v0_5;
    u8 temp_v1;
    u8 temp_v1_5;
    u8 temp_v1_6;

    D_80862B44 = input;
    temp_v0 = player->unk_D6A;
    if (temp_v0 < 0) {
        player->unk_D6A = temp_v0 + 1;
        if (player->unk_D6A == 0) {
            player->unk_D6A = 1;
            play_sound(0x5806U);
        }
    }
    temp_a1 = &player->actor.home;
    sp38 = temp_a1;
    Math_Vec3f_Copy(&player->actor.prevPos, &temp_a1->pos);
    sp44 = fabsf(player->linearVelocity);
    temp_fv1 = (fabsf(Math_SinS(player->unk_B6C) * 800.0f) + 100.0f) * sp44;

    if (temp_fv1 < 300.0f) {
        var_fv0 = 300.0f;
    } else {
        var_fv0 = temp_fv1;
    }

    Math_StepToF(&player->unk_AC0, 0.0f, var_fv0);
    temp_v1 = player->unk_D57;
    if (temp_v1 != 0) {
        player->unk_D57 = temp_v1 - 1;
    }
    temp_v0_2 = player->unk_B5E;
    if (temp_v0_2 != 0) {
        player->unk_B5E = temp_v0_2 - 1;
    }
    temp_v0_3 = player->unk_D6B;
    if (temp_v0_3 != 0) {
        player->unk_D6B = temp_v0_3 - 1;
    }
    temp_v0_4 = player->invincibilityTimer;
    if (temp_v0_4 < 0) {
        player->invincibilityTimer = temp_v0_4 + 1;
    } else if (temp_v0_4 > 0) {
        player->invincibilityTimer = temp_v0_4 - 1;
    }
    temp_v0_5 = player->unk_B64;
    if (temp_v0_5 != 0) {
        player->unk_B64 = temp_v0_5 - 1;
    }
    temp_v0_6 = player->unk_B60;
    if (temp_v0_6 != 0) {
        player->unk_B60 = temp_v0_6 - 1;
    }
    temp_v0_7 = gSaveContext.jinxTimer;
    if (temp_v0_7 != 0) {
        gSaveContext.jinxTimer = temp_v0_7 - 1;
    }
    func_80122C20(play, &player->unk_3D0);
    if ((player->transformation == 0) && (func_8082FBE8(player) != 0)) {
        func_80844D80(play, player);
    }
    if (player->transformation == 2) {
        var_v0 = 0;
        if (player->stateFlags1 & 0x800000) {
            var_v0 = 1;
        }
        Math_StepToF(&player->unk_B08[2], (f32) var_v0, D_8085D3FC[var_v0]);
    }
    func_80832888(player, play);
    if (play->roomCtx.currRoom.enablePosLights != 0) {
        Lights_PointSetColorAndRadius((LightInfo* ) player->unk_508, 0xFFU, 0xFFU, 0xFFU, (s16) 0x3C);
    } else {
        player->unk_514 = -1;
    }
    temp_v0_8 = player->itemActionParam;
    if ((temp_v0_8 == 7) && (player->unk_B28 != 0)) {
        func_808442D8(play, &player->actor);
    } else if (temp_v0_8 == 2) {
        temp_v0_9 = player->unk_B28;
        if (temp_v0_9 < 0) {
            player->unk_B28 = temp_v0_9 + 1;
        }
    }
    if (player->unk_B65 != 0) {
        func_808445C4(play, &player->actor);
    }
    if (player->isBurning != 0) {
        func_808341F4(play, player);
    }
    if (player->stateFlags2 & 0x8000) {
        if (!(player->actor.bgCheckFlags & 1)) {
            func_8082DABC(player);
            Actor_MoveWithGravity(&player->actor);
        }
        func_80843178(play, player);
        sp28 = &player->cylinder;
        sp2C = &player->shieldCylinder;
    } else {
        temp_v0_10 = player->prevBoots;
        temp_v1_2 = player->currentBoots;
        sp2C = player->unk_508;
        if (temp_v0_10 != temp_v1_2) {
            if (temp_v1_2 == 5) {
                if (player->stateFlags1 & 0x800000) {
                    func_8082DC64(play, player);
                    if (player->ageProperties->unk_2C < player->actor.depthInWater) {
                        player->stateFlags2 |= 0x400;
                    }
                }
            } else if ((player->stateFlags1 & 0x800000) && ((temp_v0_10 == 5) || (player->actor.bgCheckFlags & 1))) {
                func_8083B930(play, player);
                player->stateFlags2 &= ~0x400;
                if (player->unk_748 == func_808508C8) {
                    player->unk_AE8 = 0x14;
                }
            }
            player->prevBoots = player->currentBoots;
        }
        if ((player->actor.parent == NULL) && (player->stateFlags1 & 0x800000)) {
            player->actor.parent = player->rideActor;
            func_80837BD0(play, player);
            player->unk_AE8 = -1;
            func_8082DB18(play, player, &D_0400E088);
            func_8082E920(play, player, 0x9B);
        }
        temp_v0_11 = player->unk_ADC;

        if (temp_v0_11 == 0) {
            player->unk_ADD = 0;
        } else if (temp_v0_11 < 0) {
            player->unk_ADC = temp_v0_11 + 1;
        } else {
            player->unk_ADC = temp_v0_11 - 1;
        }

        if (!(player->stateFlags3 & 0x2000)) {
            Math_ScaledStepToS(&player->unk_AAA, 0, 0x190);
        }
        temp_v0_12 = player->transformation;

        if (((s32) temp_v0_12 > 0) && ((s32) temp_v0_12 < 4)) {
            func_800BBB74(&player->eyeTexId, 0x14, 0x50, 3);
        } else {
            func_800BBAC0(&player->eyeTexId, 0x14, 0x50, 6);
        }

        if (play->gameplayFrames & 0x20) {
            var_a2 = 0;
        } else {
            var_a2 = 3;
        }
        player->actor.shape.face = var_a2 + player->eyeTexId;
        if (player->currentMask == 4) {
            func_80124420(player);
        }
        if (func_800B7118(player) != 0) {
            func_808484F0(player);
        }
        if ((play->unk_18844 == 0) && !(player->skelAnime.moveFlags & 0x80)) {
            if (!(player->stateFlags1 & 2) && (player->actor.parent == NULL)) {
                func_80844784(play, player);
            }
            func_80843178(play, player);
        } else {
            D_80862B08 = 0;
            player->unk_D5E = 0;
            if (player->stateFlags1 & 0x800000) {
                temp_v0_13 = player->rideActor;
                player->actor.floorPoly = temp_v0_13->floorPoly;
                player->actor.floorBgId = temp_v0_13->floorBgId;
            }
            D_80862B10 = 0;
            player->unk_B80 = 0.0f;
        }
        func_8083562C(play, &player->actor, player->actor.floorPoly, (s32) player->actor.floorBgId);
        temp_v1_3 = D_80862B10;
        temp_t7 = temp_v1_3 - 1;
        if (temp_v1_3 != 0) {
            D_80862B10 = temp_t7;
            if (D_80862B14 == 0) {
                var_fv1 = D_8085D404[temp_t7];
                if (!(player->stateFlags1 & 0x800000)) {
                    var_fv1 *= 0.25f;
                }
            } else {
                var_fv1 = D_8085D410[D_80862B10];
            }
            sp50 = var_fv1;
            Math_StepToF(&player->unk_B80, var_fv1, var_fv1 * 0.1f);

            if (player->stateFlags1 & 0x800000) {
                var_fv0_2 = 400.0f;
            } else {
                var_fv0_2 = 800.0f;
            }
            Math_ScaledStepToS(&player->unk_B84, D_80862B16, (s16) (s32) (var_fv0_2 * var_fv1));
        } else if (player->unk_B80 != 0.0f) {
            if (player->stateFlags1 & 0x800000) {
                var_fv0_3 = 0.5f;
            } else {
                var_fv0_3 = 2.0f;
            }
            Math_StepToF(&player->unk_B80, 0.0f, var_fv0_3);
        }

        if (!(player->stateFlags1 & 0x20000080) && !(player->stateFlags3 & 0x80) && (player->unk_748 != func_80854430)) {
            func_8083BB4C(play, player);
            if (Play_InCsMode(play) == 0) {
                if ((player->actor.id == 0) && (!(player->stateFlags1 & 0x80000000)) && (gSaveContext.save.playerData.health == 0) && (func_808323C0(&player->actor, play->playerActorCsIds[6]) != 0)) {
                    if (player->stateFlags3 & 0x01000000) {
                        func_808355D8(play, player, &D_0400E2D8);
                    } else {
                        temp_v1_4 = player->stateFlags1;
                        if (temp_v1_4 & 0x206004) {
                            func_8082DD2C(play, player, &D_0400E2D8);
                            func_80833AA0(player, play);
                        } else if ((player->actor.bgCheckFlags & 1) || ((temp_v1_4 & 80000000))) {
                            if (func_801242B4(player) != 0) {
                                var_a2_2 = &D_0400DFE8;
                            } else {
                                var_v0_2 = &D_0400D698;
                                if (player->unk_B65 != 0) {
                                    var_v0_2 = &D_0400DC28;
                                }
                                var_a2_2 = var_v0_2;
                            }
                            func_80831F34(play, player, var_a2_2);
                        }
                    }
                } else {
                    if ((player->actor.parent == NULL) && ((func_8082DA90(play) != 0) || (player->unk_D6B != 0) || (func_80834600(player, play) == 0))) {
                        func_8083827C(player, play);
                    } else {
                        player->unk_B68 = (s16) (s32) player->actor.world.pos.y;
                    }
                    func_808446F4(play, player);
                }
            }
        } else if (!(player->actor.bgCheckFlags & 1) && (player->unk_748 == func_8084D820) && (player->unk_397 == 4)) {
            player->actor.world.pos.y = player->actor.prevPos.y;
        }
        temp_a1_2 = play->csCtx.state;
        if (temp_a1_2 != 0) {
            temp_v1_5 = player->csMode;
            if ((temp_v1_5 != 5) && !(player->stateFlags1 & 0x800000)) {
                temp_a0 = player->stateFlags2;
                if (!(temp_a0 & 0x80) && (player->actor.id == 0)) {
                    temp_v0_14 = play->csCtx.playerAction;
                    if ((temp_v0_14 != NULL) && (*(&D_8085D384 + temp_v0_14->action) != 0)) {
                        func_800B7298(play, NULL, 5U);
                        func_8082DABC(player);
                    } else if ((temp_v1_5 == 0) && !(temp_a0 & 0x08000400) && (temp_a1_2 != 3)) {
                        func_800B7298(play, NULL, 0x14U);
                        func_8082DABC(player);
                    }
                }
            }
        }
        temp_v1_6 = player->csMode;
        if (temp_v1_6 != 0) {
            temp_v0_15 = player->unk_748;
            if ((temp_v1_6 != 6) || !(player->stateFlags1 & 0x04206004)) {
                if (temp_v0_15 != func_8085B08C) {
                    player->unk_AA5 = 4;
                    if (temp_v1_6 == 5) {
                        func_808387A0(play, player);
                        func_8082DAD4(player);
                    }
                }
            } else if (temp_v0_15 != func_8085B08C) {
                func_8085AACC(play, player, 0, play);
            }
        } else {
            player->unk_395 = 0;
        }
        func_8083BF54(play, &player->actor);
        Lights_PointSetPosition((LightInfo* ) sp2C, (s16) (s32) player->actor.world.pos.x, (s16) (s32) (player->actor.world.pos.y + 40.0f), (s16) (s32) player->actor.world.pos.z);
        temp_v0_16 = player->unk_730;
        if (((temp_v0_16 == NULL) || (player->targetActor == temp_v0_16) || (temp_v0_16->hintId == 0xFF)) && (player->tatlTextId == 0)) {
            player->stateFlags2 &= 0xFFDFFFFD;
        }
        player->stateFlags1 &= 0xFFBFEFEF;
        player->stateFlags2 &= 0xFBBEAC92;
        player->stateFlags3 &= 0xE1EFE6AF;
        func_808425B4(player);
        func_8082EB38(play, player);
        if (player->stateFlags1 & 0x800000) {
            D_8085C3E4 = 0.5f;
        } else {
            D_8085C3E4 = 1.0f;
        }
        D_8085C3E8 = 1.0f / D_8085C3E4;
        D_80862B4C = 0;
        D_80862B48 = 0;
        temp_v0_17 = Play_InCsMode(play);
        var_v1 = temp_v0_17;
        D_80862B2C = (s32) player->currentMask;
        if (!(player->stateFlags3 & 4)) {
            sp58 = temp_v0_17;
            player->unk_748(player, play);
            var_v1 = sp58;
        }
        if (var_v1 == 0) {
            func_808426F0(play, player);
        }
        Player_UpdateCamAndSeqModes(play, player);
        temp_v0_18 = player->skelAnime.moveFlags;
        if (temp_v0_18 & 8) {
            if (temp_v0_18 & 4) {
                var_fv0_4 = 1.0f;
            } else {
                var_fv0_4 = player->ageProperties->unk_08;
            }
            AnimationContext_SetMoveActor(play, &player->actor, &player->skelAnime, var_fv0_4);
        }
        func_80832578(player, play);
        if (player->actor.flags & 0x100) {
            player->targetActorDistance = 0.0f;
        } else {
            player->targetActor = NULL;
            player->exchangeItemId = 0;
            player->targetActorDistance = 3.4028235e38f;
        }
        if (!(player->actor.flags & 0x30000000) && (player->unk_AA5 != 5)) {
            player->unk_A90 = NULL;
            player->unk_A94 = 3.4028235e38f;
        }
        if (!(player->stateFlags1 & 0x800)) {
            player->interactRangeActor = NULL;
            player->getItemDirection = 0x6000;
        }
        if (player->actor.parent == NULL) {
            player->rideActor = NULL;
        }
        player->tatlTextId = 0;
        player->unk_B2B = -1;
        player->unk_AA0 = 3.4028235e38f;
        player->doorType = PLAYER_DOORTYPE_0;
        player->unk_B75 = 0;
        player->unk_A78 = NULL;
        Math_StepToF(&player->windSpeed, 0.0f, 0.5f);
        if ((player->unk_B62 != 0) || ((gSaveContext.unk_3F28 == 0) && (gSaveContext.save.playerData.magic != 0) && (player->stateFlags1 & 0x10))) {
            func_8082F1AC(&play->state, player);
        }
        temp_fv0 = player->actor.world.pos.y - player->actor.prevPos.y;
        var_fv1_2 = ((player->bodyPartsPos[3].y + player->bodyPartsPos[6].y) * 0.5f) + temp_fv0;
        var_fv0_5 = temp_fv0 + (player->bodyPartsPos[7].y + 10.0f);
        if (player->cylinder.info.toucher.dmgFlags == 0x80000) {
            player->cylinder.dim.height = 0x50;
            var_fv1_2 = ((var_fv0_5 + var_fv1_2) * 0.5f) - 40.0f;
        } else {
            player->cylinder.dim.height = (s16) (s32) (var_fv0_5 - var_fv1_2);
            temp_v0_19 = player->cylinder.dim.height;
            if (temp_v0_19 < 0) {
                temp_fa0 = var_fv0_5;
                var_fv0_5 = var_fv1_2;
                var_fv1_2 = temp_fa0;
                player->cylinder.dim.height = -temp_v0_19;
            }
        }
        temp_fa0_2 = player->actor.world.pos.y;
        player->cylinder.dim.yShift = (s16) (s32) (var_fv1_2 - temp_fa0_2);
        if (player->unk_B62 != 0) {
            player->shieldCylinder.base.acFlags = 0;
            player->shieldCylinder.info.toucher.dmgFlags = 0x80000;
            player->shieldCylinder.info.toucherFlags = 1;
            player->shieldCylinder.info.bumperFlags = 0;
            player->shieldCylinder.dim.height = 0x50;
            player->shieldCylinder.dim.radius = 0x32;
            temp_a1_3 = &player->shieldCylinder;
            player->shieldCylinder.dim.yShift = (s16) (s32) ((((var_fv0_5 + var_fv1_2) * 0.5f) - 40.0f) - temp_fa0_2);
            sp2C = temp_a1_3;
            Collider_UpdateCylinder(&player->actor, temp_a1_3);
            CollisionCheck_SetAT(play, &play->colChkCtx, &sp2C->base);
        } else if (player->stateFlags1 & 0x400000) {
            temp_v0_20 = player->transformation;
            if ((temp_v0_20 == 1) || (temp_v0_20 == 3)) {
                player->shieldCylinder.base.acFlags = 0x15;
                player->shieldCylinder.info.toucher.dmgFlags = 0x100000;
                player->shieldCylinder.info.toucherFlags = 0;
                player->shieldCylinder.info.bumperFlags = 1;
                if (temp_v0_20 == 1) {
                    player->shieldCylinder.dim.height = 0x23;
                } else {
                    player->shieldCylinder.dim.height = 0x1E;
                }
                temp_a1_4 = &player->shieldCylinder;
                if (player->transformation == 1) {
                    player->shieldCylinder.dim.radius = 0x1E;
                } else {
                    player->shieldCylinder.dim.radius = 0x14;
                }
                player->shieldCylinder.dim.yShift = 0;
                sp2C = temp_a1_4;
                Collider_UpdateCylinder(&player->actor, temp_a1_4);
                CollisionCheck_SetAC(play, &play->colChkCtx, &sp2C->base);
                player->cylinder.dim.yShift = 0;
                player->cylinder.dim.height = player->shieldCylinder.dim.height;
            } else {
                player->cylinder.dim.height = (s16) (s32) ((f32) player->cylinder.dim.height * 0.8f);
            }
        }
        temp_a1_5 = &player->cylinder;
        sp2C = &player->shieldCylinder;
        sp28 = temp_a1_5;
        Collider_UpdateCylinder(&player->actor, temp_a1_5);
        if (!(player->stateFlags2 & 0x4000)) {
            if (!(player->stateFlags1 & 0x806084) && !(player->stateFlags3 & 0x10000000)) {
                temp_v0_21 = player->unk_748;
                if ((temp_v0_21 != func_808561B0) && (temp_v0_21 != func_80853D68) && (player->actor.draw != NULL)) {
                    if ((player->actor.id != 0) && (player->csMode == 0x6E)) {
                        player->cylinder.dim.radius = 8;
                    }
                    CollisionCheck_SetOC(play, &play->colChkCtx, &sp28->base);
                }
            }
            if (!(player->stateFlags1 & 0x04000080) && (player->invincibilityTimer <= 0)) {
                temp_v0_22 = player->unk_748;
                if ((temp_v0_22 != func_808561B0) && ((temp_v0_22 != func_80857BE8) || (player->unk_AE7 != 1))) {
                    temp_a1_6 = &play->colChkCtx;
                    if (player->cylinder.base.atFlags != 0) {
                        sp34 = temp_a1_6;
                        CollisionCheck_SetAT(play, temp_a1_6, &sp28->base);
                    }
                    CollisionCheck_SetAC(play, temp_a1_6, &sp28->base);
                }
            }
        }
        AnimationContext_SetNextQueue(play);
    }
    func_801229FC(player);
    Math_Vec3f_Copy(&sp38->pos, &player->actor.world.pos);

    if ((player->stateFlags1 & 0x30000080) || (player != GET_PLAYER(play))) {
        player->actor.colChkInfo.mass = 0xFF;
    } else {
        player->actor.colChkInfo.mass = *(D_8085D174 + player->transformation);
    }

    player->stateFlags3 &= ~0x404;
    Collider_ResetCylinderAC(play, &sp28->base);
    Collider_ResetCylinderAC(play, &sp2C->base);
    Collider_ResetCylinderAT(play, &sp2C->base);
    Collider_ResetQuadAT(play, &player->meleeWeaponQuads[0].base);
    Collider_ResetQuadAT(play, &player->meleeWeaponQuads[1].base);
    Collider_ResetQuadAC(play, &player->shieldQuad.base);
    temp_v1_7 = player->actor.bgCheckFlags;
    if (!(temp_v1_7 & 1) || (temp_v1_7 & 2) || (player->actor.floorBgId != 0x32)) {
        player->unk_AC0 = 0.0f;
    }
    player->actor.shape.yOffset = player->unk_ABC + player->unk_AC0;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80844EF8.s")
#endif

extern Vec3f D_8085D41C;

#ifdef NON_MATCHING
// in-function static bss
void Player_Update(Actor* thisx, PlayState* play) {
    static Vec3f D_80862AF0; // sDogSpawnPos
    Player* this = (Player*)thisx;
    s32 dogParams;
    s32 pad;
    Input input;
    s32 pad2;

    this->stateFlags3 &= ~0x10;
    if (gSaveContext.dogParams < 0) {
        if (Object_GetIndex(&play->objectCtx, 0x132) < 0) {
            gSaveContext.dogParams = 0;
        } else {
            Actor* dog;

            gSaveContext.dogParams &= (u16)~0x8000;
            func_80835BC8(this, &this->actor.world.pos, &D_8085D41C, &D_80862AF0);

            dogParams = gSaveContext.dogParams;

            dog = Actor_Spawn(&play->actorCtx, play, 0xE2, D_80862AF0.x, D_80862AF0.y, D_80862AF0.z, 0,
                              this->actor.shape.rot.y, 0, dogParams | 0x8000);
            if (dog != NULL) {
                dog->room = -1;
            }
        }
    }

    if ((this->interactRangeActor != NULL) && (this->interactRangeActor->update == NULL)) {
        this->interactRangeActor = NULL;
    }

    if ((this->heldActor != NULL) && (this->heldActor->update == NULL)) {
        func_8082DCA0(play, this);
    }

    if ((play->actorCtx.unk268 != 0) && (this == GET_PLAYER(play))) {
        input = play->actorCtx.unk_26C;
    } else if ((this->csMode == 5) || (this->stateFlags1 & 0x20000020) || (this != GET_PLAYER(play)) ||
               (func_8082DA90(play) != 0) || (gSaveContext.save.playerData.health == 0)) {
        bzero(&input, sizeof(Input));
        this->unk_B68 = this->actor.world.pos.y;
    } else {
        input = play->state.input[0];
        if (this->unk_B5E != 0) {
            input.cur.button &= ~0xC008;
            input.press.button &= ~0xC008;
        }
    }

    func_80844EF8(this, play, &input);
    play->actorCtx.unk268 = 0;
    bzero(&play->actorCtx.unk_26C, sizeof(Input));

    MREG(52) = this->actor.world.pos.x;
    MREG(53) = this->actor.world.pos.y;
    MREG(54) = this->actor.world.pos.z;
    MREG(55) = this->actor.world.rot.y;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/Player_Update.s")
#endif

void Player_DrawGameplay(PlayState* play, Player* this, s32 lod, Gfx* cullDList,
                         OverrideLimbDrawFlex overrideLimbDraw) {
    OPEN_DISPS(play->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x0C, cullDList);
    gSPSegment(POLY_XLU_DISP++, 0x0C, cullDList);

    Player_DrawImpl(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount, lod,
                    this->transformation, 0, this->actor.shape.face, overrideLimbDraw, func_80128BD0, &this->actor);

    CLOSE_DISPS(play->state.gfxCtx);
}

void func_80846460(Player* this) {
    Vec3f* pos;
    Vec3f* bodyPartsPos;
    s32 i;

    this->actor.focus.pos.x = this->actor.world.pos.x;
    this->actor.focus.pos.z = this->actor.world.pos.z;
    this->actor.focus.pos.y = this->actor.world.pos.y + 24.0f;

    pos = &this->actor.world.pos;
    bodyPartsPos = this->bodyPartsPos;
    for (i = 0; i < ARRAY_COUNT(this->bodyPartsPos); i++) {
        Math_Vec3f_Copy(bodyPartsPos, pos);
        bodyPartsPos++;
    }

    this->bodyPartsPos[7].y = this->actor.world.pos.y + 24.0f;
    this->bodyPartsPos[0].y = this->actor.world.pos.y + 60.0f;
    Math_Vec3f_Copy(this->actor.shape.feetPos, pos);
    Math_Vec3f_Copy(&this->actor.shape.feetPos[1], pos);
}

extern struct_80124618 D_8085D510[2];
extern struct_80124618 D_8085D520[2];
extern struct_80124618 D_8085D530[2];
extern struct_80124618 D_8085D540[2];
extern struct_80124618* D_8085D550[3];
extern struct_80124618* D_8085D55C[3];
extern struct_80124618* D_8085D568[3];
extern Gfx* D_8085D574[3];

extern Color_RGB8 D_8085D580;
extern Color_RGB8 D_8085D584;
extern Gfx D_0600C540[];

extern UNK_TYPE D_06013138;

extern UNK_TYPE D_0400E318;
extern UNK_TYPE D_0400E330;
extern UNK_TYPE D_0400E348;
extern UNK_TYPE D_0400E430;
extern Gfx D_04050D10[];
extern Gfx D_040528B0[];
extern Gfx D_060127B0[];
extern Gfx D_060134D0[];
extern UNK_TYPE D_06014684;

#if 0
void Player_Draw(Actor* thisx, PlayState* play) {
    Player* this = (Player* ) thisx;
    s32 spEC;
    struct_80124618** spE8;
    struct_80124618** spE4;
    f32 spE0;
    Gfx** spDC;
    s32 spD8;
    Gfx* spCC;
    //GraphicsContext* spC4;
    Color_RGB8 spBC;
    f32 spB8;
    f32 spB4;
    u8 sp9B;
    s32 (*sp84)(PlayState*, s32, Gfx**, Vec3f*, Vec3s*, Actor*);
    s32 sp80;
    Vec3f sp74;
    s16 sp70;
    f32 sp64;
    Vec3f* sp48;
    //GraphicsContext* temp_s0;
    //GraphicsContext* var_a0;
    f32 temp_fa0;
    f32 temp_fa1;
    f32 temp_ft2;
    f32 temp_fv0;
    f32 var_fa1;
    f32 var_fv1;
    s16 temp_s0_2;
    s16 temp_v0_10;
    s16 temp_v0_7;
    s8 temp_v1;
    u32 temp_v0_15;
    u32 temp_v1_2;

    spEC = 0;
    Math_Vec3f_Copy(&this->unk_D6C, this->bodyPartsPos);
    if (this->stateFlags3 & 0x40100) {
        spE8 = D_8085D550;
        // temp_s0 = play->state.gfxCtx;

        OPEN_DISPS(play->state.gfxCtx);

        Matrix_Push();
        func_8012C268(play);
        spEC = 1;
        if (this->stateFlags3 & 0x40000) {
            Matrix_SetTranslateRotateYXZ(this->unk_AF0[0].x, this->unk_AF0[0].y, this->unk_AF0[0].z, &gZeroVec3s);
            Matrix_Scale(this->actor.scale.x, this->actor.scale.y, this->actor.scale.z, MTXMODE_APPLY);
            spE8 = D_8085D568;
            spE0 = 0.0f;
        } else {
            Matrix_Translate(0.0f, -this->unk_ABC, 0.0f, MTXMODE_APPLY);
            temp_ft2 = (f32) (this->unk_AE8 - 6);
            spE0 = temp_ft2;
            if (temp_ft2 < 0.0f) {
                spE8 = D_8085D55C;
                spE0 = (f32) this->unk_B86;
            }
        }
        spDC = D_8085D574;
        sp48 = &this->unk_AF0[1];
        spD8 = 0;
        spE4 = spE8;
        do {
            Matrix_Push();
            func_80124618(*spE4, spE0, sp48);
            Matrix_Scale(this->unk_AF0[1].x, this->unk_AF0[1].y, this->unk_AF0[1].z, MTXMODE_APPLY);

#if 0
            temp_v0_2 = temp_s0->polyOpa.p;
            temp_s0->polyOpa.p = temp_v0_2 + 8;
            temp_v0_2->words.w0 = 0xDA380003;
            spCC = temp_v0_2;
            spCC->words.w1 = Matrix_NewMtx(play->state.gfxCtx);
#endif
            gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

#if 0
            temp_v0_3 = temp_s0->polyOpa.p;
            temp_s0->polyOpa.p = temp_v0_3 + 8;
            temp_v0_3->words.w0 = 0xDE000000;
            temp_v0_3->words.w1 = *spDC;
#endif
            gSPDisplayList(POLY_OPA_DISP++, *spDC);

            Matrix_Pop();
            spD8 = spD8 + 1;
            spDC += 4;
            spE4 += 4;
        } while (spD8 != 3);
        Matrix_Pop();

        CLOSE_DISPS(play->state.gfxCtx);
    }

    if ((this->stateFlags2 & 0x20000000) && (this->unk_ABC > -3900.0f)) {
        OPEN_DISPS(play->state.gfxCtx);

        //spC4 = play->state.gfxCtx;
        if (spEC == 0) {
            func_8012C268(play);
        }

        func_8012C2DC(play->state.gfxCtx);
        func_800B8050(&this->actor, play, 0);
        func_800B8118(&this->actor, play, 0);
        func_80122868(play, this);
        if (this->stateFlags3 & 0x1000) {
            temp_fv0 = this->unk_ABC;
            spB8 = temp_fv0 + 1.0f;
            spB4 = 1.0f - (temp_fv0 * 0.5f);
            func_80846460(this);
            Matrix_Translate(this->actor.world.pos.x, this->actor.world.pos.y + (1200.0f * this->actor.scale.y * spB8), this->actor.world.pos.z, MTXMODE_NEW);
            if (this->unk_B86 != 0) {
                Matrix_RotateYS(this->unk_B28, MTXMODE_APPLY);
                Matrix_RotateXS(this->unk_B86, MTXMODE_APPLY);
                Matrix_RotateYS((s16) (this->unk_B28 * -1), MTXMODE_APPLY);
            }
            Matrix_RotateYS(this->actor.shape.rot.y, MTXMODE_APPLY);
            Matrix_RotateZS(this->actor.shape.rot.z, MTXMODE_APPLY);
            if (spB8 < spB4) {
                var_fv1 = spB4;
            } else {
                var_fv1 = spB8;
            }
            Matrix_Scale(this->actor.scale.x * spB4 * 1.15f, this->actor.scale.y * spB8 * 1.15f, var_fv1 * this->actor.scale.z * 1.15f, MTXMODE_APPLY);
            Matrix_RotateXS(this->actor.shape.rot.x, MTXMODE_APPLY);
            Scene_SetRenderModeXlu(play, 0, 1U);
            Lib_LerpRGB(&D_8085D580, &D_8085D584, this->unk_B08[2], &spBC);

#if 0
            temp_v0_4 = spC4->polyOpa.p;
            spC4->polyOpa.p = temp_v0_4 + 8;
            temp_v0_4->words.w0 = 0xFB000000;
            temp_v0_4->words.w1 = ((u8) spBC.r << 0x18) | (spBC.g << 0x10) | (spBC.b << 8) | 0xFF;
#endif
            gDPSetEnvColor(POLY_OPA_DISP++, (u8) spBC.r, spBC.g, spBC.b, 255);

#if 0
            temp_v0_5 = spC4->polyOpa.p;
            spC4->polyOpa.p = temp_v0_5 + 8;
            temp_v0_5->words.w0 = 0xDA380003;
            temp_v0_5->words.w1 = Matrix_NewMtx(play->state.gfxCtx);
#endif
            gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

#if 0
            temp_v0_6 = spC4->polyOpa.p;
            spC4->polyOpa.p = temp_v0_6 + 8;
            temp_v0_6->words.w0 = 0xDE000000;
            temp_v0_6->words.w1 = (u32) &D_0600BDD8;
#endif
            gSPDisplayList(POLY_OPA_DISP++, D_0600BDD8);


            temp_v0_7 = this->unk_B88;

            if (temp_v0_7 != 0) {
                if (temp_v0_7 < 3) {
                    func_80124618(D_8085D540, (f32) temp_v0_7, this->unk_AF0);
                    Matrix_Scale(this->unk_AF0[0].x, this->unk_AF0[0].y, this->unk_AF0[0].z, MTXMODE_APPLY);
#if 0
                    temp_v0_8 = spC4->polyOpa.p;
                    spC4->polyOpa.p = temp_v0_8 + 8;
                    temp_v0_8->words.w0 = 0xDA380003;
                    temp_v0_8->words.w1 = Matrix_NewMtx(play->state.gfxCtx);
#endif
                    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                }
#if 0
                temp_v0_9 = spC4->polyOpa.p;
                spC4->polyOpa.p = temp_v0_9 + 8;
                temp_v0_9->words.w0 = 0xDE000000;
                temp_v0_9->words.w1 = (u32) &D_0600C540;
#endif
                gSPDisplayList(POLY_OPA_DISP++, D_0600C540);
            }

            func_80122BA4(play, &this->unk_3D0, 1, 0xFF);
            func_80122BA4(play, &this->unk_3D0, 2, 0xFF);
            temp_v0_10 = this->unk_B88;

            if (temp_v0_10 < 3) {
                temp_v1 = this->unk_AE7;
                if (temp_v1 >= 5) {
                    temp_fa1 = (f32) (temp_v1 - 4) * 0.02f;

                    if (temp_v0_10 != 0) {
                        sp9B = (temp_v0_10 * -0x55) + 0xFF;
                    } else {
                        sp9B = (200.0f * temp_fa1);
                    }

                    if (temp_v0_10 != 0) {
                        var_fa1 = 0.65f;
                    } else {
                        var_fa1 = temp_fa1 * 1.0f;
                    }

                    Matrix_Scale(1.0f, var_fa1, var_fa1, MTXMODE_APPLY);

#if 0
                    temp_v0_11 = spC4->polyXlu.p;
                    spC4->polyXlu.p = temp_v0_11 + 8;
                    temp_v0_11->words.w0 = 0xDA380003;
                    temp_v0_11->words.w1 = Matrix_NewMtx(play->state.gfxCtx);
#endif
                    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

                    AnimatedMat_DrawXlu(play, Lib_SegmentedToVirtual(&D_06013138));

#if 0
                    temp_v0_12 = spC4->polyXlu.p;
                    spC4->polyXlu.p = temp_v0_12 + 8;
                    temp_v0_12->words.w0 = 0xFB000000;
                    temp_v0_12->words.w1 = sp9B | 0x9B000000;
#endif
                    gDPSetEnvColor(POLY_XLU_DISP++, 155, 0, 0, sp9B);

#if 0
                    temp_v0_13 = spC4->polyXlu.p;
                    spC4->polyXlu.p = temp_v0_13 + 8;
                    temp_v0_13->words.w1 = (u32) &D_060127B0;
                    temp_v0_13->words.w0 = 0xDE000000;
#endif
                    gSPDisplayList(POLY_XLU_DISP++, D_060127B0);

                    AnimatedMat_DrawXlu(play, Lib_SegmentedToVirtual(&D_06014684));

#if 0
                    temp_v0_14 = spC4->polyXlu.p;
                    spC4->polyXlu.p = temp_v0_14 + 8;
                    temp_v0_14->words.w1 = (u32) &D_060134D0;
                    temp_v0_14->words.w0 = 0xDE000000;
#endif
                    gSPDisplayList(POLY_XLU_DISP++, D_060134D0);
                }
            }
        } else if ((this->transformation == 1) && (this->stateFlags1 & 0x400000)) {
            func_80846460(this);
            SkelAnime_DrawFlexOpa(play, this->unk_2C8.skeleton, this->unk_2C8.jointTable, this->unk_2C8.dListCount, NULL, NULL, NULL);
        } else {
            sp84 = func_80125D4C;

            if ((this->csMode != 0) || (this->actor.projectedPos.z < 320.0f)) {
                sp80 = 0;
            } else {
                sp80 = 1;
            }

            if (this->stateFlags1 & 0x100000) {
                SkinMatrix_Vec3fMtxFMultXYZ(&play->viewProjectionMtxF, &this->actor.focus.pos, &sp74);
                if (sp74.z < -4.0f) {
                    sp84 = func_801262C8;
                }
            }

            if (this->stateFlags2 & 0x04000000) {
                temp_v0_15 = play->gameplayFrames;
                temp_s0_2 = temp_v0_15 * 0x258;
                sp70 = temp_v0_15 * 0x3E8;
                Matrix_Push();
                this->actor.scale.y = -this->actor.scale.y;
                Matrix_SetTranslateRotateYXZ(this->actor.world.pos.x, this->actor.world.pos.y + (2.0f * this->actor.depthInWater) + (this->unk_ABC * this->actor.scale.y), this->actor.world.pos.z, &this->actor.shape.rot);
                Matrix_Scale(this->actor.scale.x, this->actor.scale.y, this->actor.scale.z, MTXMODE_APPLY);
                Matrix_RotateXS(temp_s0_2, MTXMODE_APPLY);
                Matrix_RotateYS(sp70, MTXMODE_APPLY);
                Matrix_Scale(1.1f, 0.95f, 1.05f, MTXMODE_APPLY);
                Matrix_RotateYS(-sp70, MTXMODE_APPLY);
                Matrix_RotateXS(-temp_s0_2, MTXMODE_APPLY);
                Player_DrawGameplay(play, &this->actor, sp80, gCullFrontDList, sp84);
                this->actor.scale.y = -this->actor.scale.y;
                Matrix_Pop();
            }

#if 0
            temp_v0_16 = spC4->polyOpa.p;
            spC4->polyOpa.p = temp_v0_16 + 8;
            temp_v0_16->words.w1 = 0;
            temp_v0_16->words.w0 = 0xD9FFF9FF;
            temp_v0_17 = spC4->polyXlu.p;
            spC4->polyXlu.p = temp_v0_17 + 8;
            temp_v0_17->words.w1 = 0;
            temp_v0_17->words.w0 = 0xD9FFF9FF;
#endif
            gSPClearGeometryMode(POLY_OPA_DISP++, G_CULL_BOTH);
            gSPClearGeometryMode(POLY_XLU_DISP++, G_CULL_BOTH);

            if ((this->transformation == 2) && (this->unk_B62 != 0) && !(this->stateFlags3 & 0x8000)) {
                Matrix_Push();
                Matrix_RotateXS(-0x4000, MTXMODE_APPLY);
                Matrix_Translate(0.0f, 0.0f, -1800.0f, MTXMODE_APPLY);
                Player_DrawZoraShield(play, this);
                Matrix_Pop();
            }

            Player_DrawGameplay(play, &this->actor, sp80, gCullBackDList, sp84);
        }

        func_801229A0(play, this);

        if (this->stateFlags2 & 0x4000) {
            temp_fa0 = this->unk_B48;
#if 0
            temp_v0_18 = spC4->polyXlu.p;
            spC4->polyXlu.p = temp_v0_18 + 8;
            temp_v0_18->words.w0 = 0xDB060020;
            temp_v1_2 = play->gameplayFrames;
            temp_v0_18->words.w1 = Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0U, -(s32) temp_v1_2 & 0x7F, 0x20, 0x20, 1, 0U, ((s32) temp_v1_2 * -2) & 0x7F, 0x20, 0x20);
#endif
            temp_v1_2 = play->gameplayFrames;
            gSPSegment(POLY_XLU_DISP++, 0x08, Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0U, -(s32) temp_v1_2 & 0x7F, 0x20, 0x20, 1, 0U, ((s32) temp_v1_2 * -2) & 0x7F, 0x20, 0x20));

            Matrix_Scale(temp_fa0, temp_fa0, temp_fa0, MTXMODE_APPLY);

#if 0
            temp_v0_19 = spC4->polyXlu.p;
            spC4->polyXlu.p = temp_v0_19 + 8;
            temp_v0_19->words.w0 = 0xDA380003;
            temp_v0_19->words.w1 = Matrix_NewMtx(play->state.gfxCtx);
#endif
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

#if 0
            temp_v0_20 = spC4->polyXlu.p;
            spC4->polyXlu.p = temp_v0_20 + 8;
            temp_v0_20->words.w1 = 0x003264FF;
            temp_v0_20->words.w0 = 0xFB000000;
#endif
            gDPSetEnvColor(POLY_XLU_DISP++, 0, 0x32, 0x64, 255);

#if 0
            temp_v0_21 = spC4->polyXlu.p;
            spC4->polyXlu.p = temp_v0_21 + 8;
            temp_v0_21->words.w1 = (u32) &D_04050D10;
            temp_v0_21->words.w0 = 0xDE000000;
#endif
            gSPDisplayList(POLY_XLU_DISP++, D_04050D10);
        }

        if (this->unk_B2A > 0) {
            Player_DrawGetItem(play, this);
        }

        func_80122D44(play, &this->unk_3D0);

        CLOSE_DISPS(play->state.gfxCtx);
    }

    play->actorCtx.unk5 &= 0xFFF7;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/Player_Draw.s")
#endif

void Player_Destroy(Actor* thisx, PlayState* play) {
    Player* this = (Player*)thisx;

    Effect_Destroy(play, this->meleeWeaponEffectIndex[0]);
    Effect_Destroy(play, this->meleeWeaponEffectIndex[1]);
    Effect_Destroy(play, this->meleeWeaponEffectIndex[2]);
    LightContext_RemoveLight(play, &play->lightCtx, this->lightNode);
    Collider_DestroyCylinder(play, &this->cylinder);
    Collider_DestroyCylinder(play, &this->shieldCylinder);
    Collider_DestroyQuad(play, &this->meleeWeaponQuads[0]);
    Collider_DestroyQuad(play, &this->meleeWeaponQuads[1]);
    Collider_DestroyQuad(play, &this->shieldQuad);
    ZeldaArena_Free(this->giObjectSegment);
    ZeldaArena_Free(this->maskObjectSegment);
    func_80115D5C(&play->state);
    func_80831454(this);
}

s32 func_80847190(PlayState* play, Player* this, s32 arg2);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80847190.s")

void func_8084748C(Player* this, f32* arg1, f32 arg2, s16 arg3);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8084748C.s")

void func_808475B4(Player* this);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_808475B4.s")

void func_808477D0(PlayState* play, Player* this, Input* input, f32 arg3) {
    f32 var_fv0;

    if ((input != NULL) && CHECK_BTN_ANY(input->press.button, BTN_B | BTN_A)) {
        var_fv0 = 1.0f;
    } else {
        var_fv0 = 0.5f;
    }

    var_fv0 = var_fv0 * arg3;
    var_fv0 = CLAMP(var_fv0, 1.0f, 2.5f);
    this->skelAnime.playSpeed = var_fv0;

    LinkAnimation_Update(play, &this->skelAnime);
}

void func_80854614(Player* this, PlayState* play);

s32 func_80847880(PlayState* play, Player* this) {
    if (play->unk_1887C != 0) {
        if (play->sceneNum == SCENE_20SICHITAI) {
            func_80831494(play, this, func_80854430, 0);
            play->unk_1887C = 0;
            this->csMode = 0;
            return true;
        }

        func_8082DE50(play, this);
        func_80831494(play, this, func_80854614, 0);
        if (!func_800B7118(this) || Player_IsHoldingHookshot(this)) {
            func_80831990(play, this, ITEM_BOW);
        }
        func_8082DB18(play, this, func_8082ED20(this));
        this->csMode = 0;
        this->stateFlags1 |= PLAYER_STATE1_100000;
        func_8082DABC(this);
        func_80836D8C(this);

        return true;
    }
    return false;
}

s32 func_80847994(PlayState* play, Player* this) {
    if (this->stateFlags3 & PLAYER_STATE3_20) {
        this->stateFlags3 &= ~PLAYER_STATE3_20;
        this->heldItemActionParam = PLAYER_AP_OCARINA;
        this->unk_AA5 = 5;
        func_80838A90(this, play);
        return true;
    }
    return false;
}

void func_808479F4(PlayState* play, Player* this, f32 arg2) {
    if (this->actor.wallBgId != 0x32) {
        DynaPolyActor* actor = DynaPoly_GetActor(&play->colCtx, this->actor.wallBgId);

        if (actor != NULL) {
            func_800B72F8(actor, arg2, this->actor.world.rot.y);
        }
    }
}

void func_80847A50(Player* this) {
    func_800B8E58(this,
                  ((this->unk_AE7 != 0) ? NA_SE_PL_WALK_METAL1 : NA_SE_PL_WALK_LADDER) + this->ageProperties->unk_94);
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80847A94.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80847BF0.s")

#if 0
void func_80847E2C(Player* arg0, f32 arg1, f32 arg2) {
    f32 var_fa0;

    if ((arg0->unk_B48 != 0.0f) && (arg2 <= arg0->skelAnime.curFrame)) {
        if (arg1 < fabsf(arg0->unk_B48)) {
            if (arg0->unk_B48 >= 0.0f) {
                var_fa0 = 1 * arg1;
            } else {
                var_fa0 = -1 * arg1;
            }
        } else {
            var_fa0 = arg0->unk_B48;
        }
        arg0->unk_B48 -= var_fa0;
        arg0->actor.world.pos.y += var_fa0;
    }
}
#else
void func_80847E2C(Player* this, f32 arg1, f32 arg2);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80847E2C.s")
#endif

s32 func_80847ED4(Player* this) {
    return this->interactRangeActor != NULL && this->interactRangeActor->id == ACTOR_EN_ZOG &&
           CHECK_BTN_ALL(D_80862B44->cur.button, BTN_A);
}

extern Vec3f D_8085D5D0;

void func_80847F1C(Player* this) {
    s32 pad;
    f32 yPos;
    s16 yaw;
    Actor* interactRangeActor = this->interactRangeActor;

    if (func_80847ED4(this)) {
        yPos = this->actor.world.pos.y;
        yaw = this->currentYaw - interactRangeActor->shape.rot.y;
        Lib_Vec3f_TranslateAndRotateY(&interactRangeActor->world.pos, interactRangeActor->shape.rot.y, &D_8085D5D0,
                                      &this->actor.world.pos);
        this->actor.world.pos.y = yPos;
        this->actor.shape.rot.y = interactRangeActor->shape.rot.y;

        interactRangeActor->speedXZ = Math_CosS(ABS_ALT(yaw)) * this->linearVelocity * 0.5f;
        if (interactRangeActor->speedXZ < 0.0f) {
            interactRangeActor->speedXZ = 0.0f;
        }
        func_8083133C(this);
    }
}

void func_80847FF8(Player* this, f32* arg1, f32 arg2, UNK_TYPE arg3);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80847FF8.s")

void func_80848048(PlayState* play, Player* this) {
    func_80831494(play, this, func_80851588, 0);
    func_8082E634(play, this, &gameplay_keep_Linkanim_00DFD0);
}

s32 func_80848094(PlayState* play, Player* this, f32* arg2, s16* arg3) {
    LinkAnimationHeader* anim;
    s16 temp_v0 = this->currentYaw - *arg3;
    s32 temp_v0_2;

    if (ABS_ALT(temp_v0) > 0x6000) {
        anim = &gameplay_keep_Linkanim_00E000;
        if (Math_StepToF(&this->linearVelocity, 0.0f, 1.0f)) {
            this->currentYaw = *arg3;
        } else {
            *arg2 = 0.0f;
            *arg3 = this->currentYaw;
        }
    } else {
        temp_v0_2 = func_8083E514(this, arg2, arg3, play);
        if (temp_v0_2 > 0) {
            anim = &gameplay_keep_Linkanim_00DFD0;
        } else if (temp_v0_2 < 0) {
            anim = &gameplay_keep_Linkanim_00DFB8;
        } else {
            s16 diff = BINANG_SUB(this->actor.shape.rot.y, *arg3);

            if (diff > 0) {
                anim = &gameplay_keep_Linkanim_00DFB0;
            } else {
                anim = &gameplay_keep_Linkanim_00DFA8;
            }
        }
    }

    if (anim != this->skelAnime.animation) {
        func_8082E634(play, this, anim);
        return true;
    }
    return false;
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_808481CC.s")

void func_80848250(PlayState* play, Player* this) {
    this->unk_B2A = 0;
    this->stateFlags1 &= ~(PLAYER_STATE1_400 | PLAYER_STATE1_800);
    this->getItemId = GI_NONE;
    func_800E0238(Play_GetCamera(play, CAM_ID_MAIN));
}

void func_80848294(PlayState* play, Player* this) {
    func_80848250(play, this);
    func_8082DC28(this);
    func_80839E74(this, play);
    this->currentYaw = this->actor.shape.rot.y;
}

s32 func_808482E0(PlayState* play, Player* this);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_808482E0.s")

extern struct_8082E224_arg1 D_8085D5DC;
extern struct_8082E224_arg1 D_8085D5E0;

void func_808484CC(Player* arg0) {
    func_8082E224(arg0, &D_8085D5E0);
}

void func_808484F0(Player* arg0) {
    arg0->unk_B08[0] += arg0->unk_B08[1];
    arg0->unk_B08[1] -= arg0->unk_B08[0] * 5.0f;
    arg0->unk_B08[1] *= 0.3f;
    if (fabsf(arg0->unk_B08[1]) < 0.00001f) {
        arg0->unk_B08[1] = 0.0f;
        if (fabsf(arg0->unk_B08[0]) < 0.00001f) {
            arg0->unk_B08[0] = 0.0f;
        }
    }
}

s32 func_80848570(Player* this, PlayState* play) {
    if (!func_8083A6C0(play, this)) {
        if (func_808396B8(play, this)) {
            PlayerMeleeWeaponAnimation meleeWeaponAnimation = func_808335F4(this);

            func_80833864(play, this, meleeWeaponAnimation);
            if ((meleeWeaponAnimation >= PLAYER_MWA_SPIN_ATTACK_1H) ||
                ((this->transformation == PLAYER_FORM_FIERCE_DEITY) && func_8082FBE8(this))) {
                this->stateFlags2 |= PLAYER_STATE2_20000;
                func_808332A0(play, this, 0, meleeWeaponAnimation < PLAYER_MWA_SPIN_ATTACK_1H);
            }
        } else {
            return false;
        }
    }
    return true;
}

// elegy of emptiness (?)
void func_80848640(PlayState* play, Player* this) {
    EnTorch2* torch2;
    Actor* effChange;

    torch2 = (EnTorch2*)play->actorCtx.unk_254[this->transformation];
    if (torch2 != NULL) {
        Math_Vec3f_Copy(&torch2->actor.home.pos, &this->actor.world.pos);
        torch2->actor.home.rot.y = this->actor.shape.rot.y;
        torch2->state = 0;
        torch2->framesUntilNextState = 20;
    } else {
        torch2 = (EnTorch2*)Actor_Spawn(&play->actorCtx, play, ACTOR_EN_TORCH2, this->actor.world.pos.x,
                                        this->actor.world.pos.y, this->actor.world.pos.z, 0, this->actor.shape.rot.y, 0,
                                        this->transformation);
    }

    if (torch2 != NULL) {
        play->actorCtx.unk_254[this->transformation] = (Actor*)torch2;
        Play_SetupRespawnPoint(&play->state, this->transformation + 3, 0xBFF);
    }

    effChange = Actor_Spawn(&play->actorCtx, play, ACTOR_EFF_CHANGE, this->actor.world.pos.x, this->actor.world.pos.y,
                            this->actor.world.pos.z, 0, this->actor.shape.rot.y, 0,
                            (GET_PLAYER_FORM << 3) | this->transformation);
    if (effChange != NULL) {
        //! @bug
        func_800B8E58((Player*)effChange, NA_SE_PL_TRANSFORM);
    }
}

s32 func_80848780(Player* this, PlayState* play) {
    if (func_80830B88(play, this)) {
        return true;
    }
    return false;
}

s32 func_808487B8(Player* this, PlayState* play) {
    if (func_80830B88(play, this) || func_80830DF0(this, play)) {
        return true;
    }
    return false;
}

s32 func_80848808(Player* this, PlayState* play) {
    if (LinkAnimation_Update(play, &this->unk_284) ||
        ((func_8082F524(this, this->heldItemId) == this->itemActionParam) &&
         (D_80862B48 = D_80862B48 || ((this->modelAnimType != PLAYER_ANIMTYPE_3) &&
                                      (this->itemActionParam != PLAYER_AP_STICK) && (play->unk_1887C == 0))))) {
        func_8082F43C(play, this, D_8085C9F0[this->itemActionParam]);
        this->unk_ACC = 0;
        this->unk_AA4 = 0;
        D_80862B4C = D_80862B48;
        return this->unk_AC4(this, play);
    }

    if (func_8082ED94(this) != 0) {
        func_80830D40(play, this);
        func_8082DB18(play, this, func_8082ED20(this));
        this->unk_AA4 = 0;
    } else {
        func_80830D40(play, this);
    }
    return true;
}

s32 func_8084894C(Player* this, PlayState* play) {
    LinkAnimation_Update(play, &this->unk_284);
    if (!CHECK_BTN_ALL(D_80862B44->cur.button, BTN_R)) {
        func_80830CE8(play, this);
    } else {
        this->stateFlags1 |= PLAYER_STATE1_400000;
        Player_SetModelsForHoldingShield(this);
        if ((this->transformation == PLAYER_FORM_ZORA) && CHECK_BTN_ALL(D_80862B44->cur.button, BTN_B)) {
            func_8082F164(this, BTN_R | BTN_B);
        }
    }
    return true;
}

s32 func_80848A0C(Player* this, PlayState* play) {
    if (LinkAnimation_Update(play, &this->unk_284)) {
        LinkAnimationHeader* anim;
        f32 lastFrame;

        anim = func_80830A58(play, this);
        lastFrame = Animation_GetLastFrame(anim);
        LinkAnimation_Change(play, &this->unk_284, anim, 1.0f, lastFrame, lastFrame, 2, 0.0f);
    }

    this->stateFlags1 |= PLAYER_STATE1_400000;
    Player_SetModelsForHoldingShield(this);
    return true;
}

s32 func_80848AB0(Player* this, PlayState* play) {
    // possibly a case of if ((D_80862B48 = D_80862B4C) || LinkAnimation_Update(play, &this->unk_284))
    D_80862B48 = D_80862B4C;
    if ((D_80862B48 != 0) || LinkAnimation_Update(play, &this->unk_284)) {
        func_8082F43C(play, this, D_8085C9F0[this->itemActionParam]);
        LinkAnimation_PlayLoop(play, &this->unk_284, D_8085BE84[this->modelAnimType]);
        this->unk_AA4 = 0;
        this->unk_AC4(this, play);
        return false;
    }
    return true;
}

s32 func_80848B6C(Player* this, PlayState* play) {
    if (this->unk_B28 >= 0) {
        this->unk_B28 = -this->unk_B28;
    }

    if (!Player_IsHoldingHookshot(this) || func_80831124(play, this)) {
        if (!func_80830B88(play, this) && !func_80831094(this, play)) {
            return false;
        }
    }
    return true;
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80848BF4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80848E4C.s")

s32 func_80849054(Player* this, PlayState* play) {
    if (!(this->actor.bgCheckFlags & 1) || LinkAnimation_Update(play, &this->unk_284)) {
        func_8082F43C(play, this, func_80848B6C);
    }
    return true;
}

s32 func_808490B4(Player* this, PlayState* play) {
    Actor* heldActor = this->heldActor;

    if (heldActor == NULL) {
        func_808309CC(play, this);
    }

    if (func_80830B88(play, this)) {
        return true;
    }

    if (this->stateFlags1 & PLAYER_STATE1_800) {
        if (LinkAnimation_Update(play, &this->unk_284)) {
            LinkAnimation_PlayLoop(play, &this->unk_284, &gameplay_keep_Linkanim_00DB30);
        }

        //! @bug: Not checking if heldActor is NULL
        if ((heldActor->id == ACTOR_EN_NIW) && (this->actor.velocity.y <= 0.0f)) {
            this->actor.terminalVelocity = -2.0f;
            this->actor.gravity = -0.5f;
            this->unk_B68 = this->actor.world.pos.y;
        }
        return true;
    }
    return func_80848780(this, play);
}

s32 func_808491B4(Player* this, PlayState* play) {
    if (func_80830B88(play, this)) {
        return true;
    }

    if (this->stateFlags1 & PLAYER_STATE1_2000000) {
        func_8082F43C(play, this, func_80849570);
    } else if (func_80831094(this, play)) {
        return true;
    }

    return false;
}

s32 func_8084923C(Player* this, PlayState* play) {
    if (LinkAnimation_Update(play, &this->unk_284)) {
        func_8082F43C(play, this, func_808492C4);
        LinkAnimation_PlayLoop(play, &this->unk_284, &gameplay_keep_Linkanim_00E3C0);
    }
    if (this->unk_284.animation == &gameplay_keep_Linkanim_00E3C0) {
        func_80831010(this, play);
    }
    return true;
}

s32 func_808492C4(Player* this, PlayState* play) {
    LinkAnimation_Update(play, &this->unk_284);
    func_80831010(this, play);
    if (D_80862B4C == 0) {
        func_8082F43C(play, this, func_8084933C);
        LinkAnimation_PlayOnce(play, &this->unk_284, &gameplay_keep_Linkanim_00E398);
    }
    return true;
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8084933C.s")

s32 func_80849570(Player* this, PlayState* play) {
    if (func_80830B88(play, this)) {
        return true;
    }

    if (this->stateFlags3 & PLAYER_STATE3_800000) {
        func_8082F43C(play, this, func_80849620);
        LinkAnimation_PlayOnce(play, &this->unk_284, &gameplay_keep_Linkanim_00E3A0);
        this->stateFlags3 &= ~PLAYER_STATE3_800000;
        func_800B8E58(this, NA_SE_PL_CATCH_BOOMERANG);
        func_8082DF8C(this, NA_SE_VO_LI_SWORD_N);
        return true;
    }

    return false;
}

s32 func_80849620(Player* this, PlayState* play) {
    if (!func_808491B4(this, play) && LinkAnimation_Update(play, &this->unk_284)) {
        if (this->stateFlags1 & PLAYER_STATE1_2000000) {
            func_8082F43C(play, this, func_80849570);
            this->unk_ACC = 0;
        } else {
            func_8082F43C(play, this, func_80848780);
        }
    }
    return true;
}

void func_808496AC(Player* this, PlayState* play) {
    LinkAnimation_Update(play, &this->skelAnime);
    func_808323C0(this, play->playerActorCsIds[2]);

    if (DECR(this->unk_AE8) == 0) {
        if (Message_GetState(&play->msgCtx) == TEXT_STATE_NONE) {
            func_80838760(this);
            func_80831494(play, this, func_80849FE0, 0);
            this->stateFlags1 &= ~PLAYER_STATE1_20000000;
        }
    } else if (this->unk_AE8 == 30) {
        if (Message_GetState(&play->msgCtx) != TEXT_STATE_NONE) {
            this->unk_AE8 += 1;
        } else {
            Message_StartTextbox(play, 0xC03, NULL);
        }
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_808497A0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80849A9C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80849DD0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80849FE0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8084A26C.s")

extern s8 D_8085D004[];

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8084A5C0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8084A794.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8084A884.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8084A8E8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8084AB4C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8084AC84.s")

extern s8 D_8085D01C[];

void func_8084AEEC(Player* this, PlayState* play) {
    this->stateFlags2 |= PLAYER_STATE2_20;
    LinkAnimation_Update(play, &this->skelAnime);
    func_80832F24(this);
    if (!func_80847880(play, this)) {
        if ((func_80833058(play, this, D_8085D01C, 0) == 0) || (func_8084AEEC == this->unk_748)) {
            if (!CHECK_BTN_ALL(D_80862B44->cur.button, BTN_B)) {
                func_80839E74(this, play);
            }
        }
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8084AF9C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8084B0EC.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8084B288.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8084B3B8.s")

void func_8084B4A8(Player* this, PlayState* play) {
    if (this->skelAnime.animation == &gameplay_keep_Linkanim_00DB10) {
        if (LinkAnimation_Update(play, &this->skelAnime)) {
            func_8082E794(this);
            func_8082E438(play, this, GET_PLAYER_ANIM(PLAYER_ANIMGROUP_31, this->modelAnimType));
        }
    } else {
        func_8082E67C(play, this, GET_PLAYER_ANIM(PLAYER_ANIMGROUP_32, this->modelAnimType));
    }

    if (DECR(this->unk_AE8) == 0) {
        if (func_80838A90(this, play) == 0) {
            func_80836A98(this, GET_PLAYER_ANIM(PLAYER_ANIMGROUP_33, this->modelAnimType), play);
        }
        this->actor.flags &= ~ACTOR_FLAG_100;
        func_800E0238(Play_GetCamera(play, CAM_ID_MAIN));
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8084B5C0.s")

void func_8084BAA4(Player* this, PlayState* play) {
    func_80832F24(this);
    if (this->unk_AE7 == 0) {
        D_80862B04 = func_8083216C(this, play);
        if ((func_8084894C == this->unk_AC4) || (func_808331FC(play, this, &this->unk_284, 4.0f) > 0)) {
            func_80831494(play, this, func_80849A9C, 1);
        }
    } else {
        s32 temp_v0;

        this->stateFlags1 |= PLAYER_STATE1_400000;
        temp_v0 = func_808331FC(play, this, &this->skelAnime, 4.0f);
        if ((temp_v0 != 0) && ((temp_v0 > 0) || LinkAnimation_Update(play, &this->skelAnime))) {
            LinkAnimationHeader* anim;
            f32 lastFrame;

            func_80831494(play, this, func_8084B5C0, 1);
            Player_SetModelsForHoldingShield(this);
            anim = GET_PLAYER_ANIM(PLAYER_ANIMGROUP_19, this->modelAnimType);
            lastFrame = Animation_GetLastFrame(anim);
            LinkAnimation_Change(play, &this->skelAnime, anim, 1.0f, lastFrame, lastFrame, 2, 0.0f);
        }
    }
}

void func_8084BBF0(Player* this, PlayState* play) {
    s32 temp_v0;

    func_80832F24(this);
    temp_v0 = func_808331FC(play, this, &this->skelAnime, 16.0f);
    if (temp_v0 != 0) {
        if (LinkAnimation_Update(play, &this->skelAnime) || (temp_v0 > 0)) {
            func_80836988(this, play);
        }
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8084BC64.s")

void func_8084BF28(Player* this, PlayState* play);

void func_8084BE40(Player* this, PlayState* play) {
    this->stateFlags2 |= (PLAYER_STATE2_20 | PLAYER_STATE2_40);
    func_808345A8(this);
    func_80832F24(this);
    if (LinkAnimation_Update(play, &this->skelAnime) && (this->linearVelocity == 0.0f)) {
        if (this->stateFlags1 & PLAYER_STATE1_20000000) {
            this->unk_AE8 += 1;
        } else {
            func_80831494(play, this, func_8084BF28, 0);
            this->stateFlags1 |= PLAYER_STATE1_4000000;
        }

        func_8082DB90(play, this,
                      (this->currentYaw != this->actor.shape.rot.y) ? &gameplay_keep_Linkanim_00DC88
                                                                    : &gameplay_keep_Linkanim_00DAE0);
        this->currentYaw = this->actor.shape.rot.y;
    }
}

extern struct_8082E224_arg1 D_8085D604[2];

void func_8084BF28(Player* this, PlayState* play) {
    this->stateFlags2 |= PLAYER_STATE2_20;

    func_808345A8(this);
    if (this->stateFlags1 & PLAYER_STATE1_20000000) {
        LinkAnimation_Update(play, &this->skelAnime);
    } else {
        s32 temp_v0 = func_808331FC(play, this, &this->skelAnime, 16.0f);

        if (temp_v0 != 0) {
            if (LinkAnimation_Update(play, &this->skelAnime) || (temp_v0 > 0)) {
                func_80836988(this, play);
            }
        }
    }

    func_8082E224(this, D_8085D604);
}

extern struct_8082E224_arg1 D_8085D60C[];

void func_8084BFDC(Player* this, PlayState* play) {
    if ((this->transformation != PLAYER_FORM_GORON) && (this->actor.depthInWater <= 0.0f)) {
        if ((play->roomCtx.currRoom.unk2 == 3) || (D_80862B08 == 9) ||
            ((func_808340AC(D_80862B08) >= 0) &&
             !SurfaceType_IsWallDamage(&play->colCtx, this->actor.floorPoly, this->actor.floorBgId))) {
            func_808344C0(play, this);
        }
    }

    func_80832F24(this);
    if (LinkAnimation_Update(play, &this->skelAnime)) {
        if (this == GET_PLAYER(play)) {
            func_80840770(play, this);
        }
    } else if (this->skelAnime.animation == &gameplay_keep_Linkanim_00D698) {
        func_8082E224(this, D_8085D60C);
    } else if ((this->skelAnime.animation == &gameplay_keep_Linkanim_00DC28) &&
               LinkAnimation_OnFrame(&this->skelAnime, 88.0f)) {
        func_8082E094(this, 0x850);
    }
}

s32 func_8084C124(PlayState* play, Player* this) {
    if (func_80837730(play, this, 3.0f, 500)) {
        func_800B8E58(this, NA_SE_EV_DIVE_INTO_WATER);
        return true;
    }
    return false;
}

void func_8084C16C(Player* this, PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8084C16C.s")

void func_8084C6EC(Player* this, PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8084C6EC.s")

void func_8084C94C(Player* this, PlayState* play) {
    this->stateFlags2 |= PLAYER_STATE2_20;

    if (LinkAnimation_Update(play, &this->skelAnime)) {
        func_8082DB3C(play, this, &gameplay_keep_Linkanim_00DE60);
    }

    Math_StepToF(&this->linearVelocity, 0.0f, 0.05f);
    if (this->actor.bgCheckFlags & 1) {
        if (this->unk_B6A >= 0x190) {
            this->actor.colChkInfo.damage = 0x10;
            func_80833B18(play, this, 1, 4.0f, 5.0f, this->actor.shape.rot.y, 0x14);
        } else {
            func_80836B3C(play, this, 4.0f);
        }
    }
}

void func_8084CA24(Player* this, PlayState* play) {
    this->stateFlags2 |= PLAYER_STATE2_20;

    if (LinkAnimation_Update(play, &this->skelAnime)) {
        func_8082DB60(play, this, &gameplay_keep_Linkanim_00E3D8);
    }

    Math_SmoothStepToS(&this->unk_B88, 0, 6, 0x7D0, 0x190);
    if (!func_80840A30(play, this, &this->linearVelocity, 0.0f)) {
        if (this->actor.bgCheckFlags & 1) {
            if (this->unk_AAA > 0x36B0) {
                this->actor.colChkInfo.damage = 0x10;
                func_80833B18(play, this, 1, 4.0f, 5.0f, this->actor.shape.rot.y, 0x14);
            } else {
                func_80836B3C(play, this, 4.0f);
            }
        } else {
            this->actor.gravity = -1.0f;
            this->unk_AAA = Math_FAtan2F(this->actor.speedXZ, -this->actor.velocity.y);
            func_8082F164(this, BTN_R);
        }
    }
}

typedef struct struct_8085CD30 {
    /* 0x00 */ LinkAnimationHeader* unk_0; /* inferred */
    /* 0x04 */ LinkAnimationHeader* unk_4; /* inferred */
    /* 0x08 */ LinkAnimationHeader* unk_8; /* inferred */
    /* 0x0C */ u8 unk_C;                   /* inferred */
    /* 0x0D */ u8 unk_D;                   /* inferred */
    /* 0x0E */ char pad_E[2];              /* maybe part of unk_D[3]? */
} struct_8085CD30;                         /* size = 0x10 */

extern struct_8085CD30 D_8085CD30[0x10];

s32 func_8083CBC4(Player* this, f32 arg1, s16 arg2, f32 arg3, f32 arg4, f32 arg5, s16 arg6);
s32 func_80836F10(PlayState* play, Player* this);

void func_8084CB58(Player* this, PlayState* play) {
    struct_8085CD30* sp3C;
    f32 sp38;
    s16 sp36;

    sp3C = &D_8085CD30[this->meleeWeaponAnimation];
    this->stateFlags2 |= 0x20;
    if (this->transformation == PLAYER_FORM_ZORA) {
        this->actor.gravity = -0.8f;
    } else {
        this->actor.gravity = -1.2f;
    }

    LinkAnimation_Update(play, &this->skelAnime);

    if (!func_808401F4(play, this)) {
        func_8083FCF0(play, this, 6.0f, sp3C->unk_C, sp3C->unk_D);
        if (!(this->actor.bgCheckFlags & 1)) {
            func_80832F78(this, &sp38, &sp36, 0.0f, play);
            func_8083CBC4(this, sp38, this->currentYaw, 1.0f, 0.05f, 0.1f, 200);
        } else if (func_80836F10(play, this) >= 0) {
            this->meleeWeaponAnimation += 3;
            func_80833864(play, this, this->meleeWeaponAnimation);
            this->unk_ADD = 3;
            this->meleeWeaponState = 0;
            func_8082E1BC(this);
        }
    }
}

void func_8084CCEC(Player* arg0, PlayState* arg1) {
    f32 sp34;
    s16 sp32;
    s32 temp_v0;

    arg0->stateFlags1 |= 0x1000;
    if (LinkAnimation_Update(arg1, &arg0->skelAnime) != 0) {
        func_8082E794(arg0);
        func_8083133C(arg0);
        arg0->stateFlags1 &= 0xFFFDFFFF;
        func_8082DB3C(arg1, arg0, D_8085CF60[Player_IsHoldingTwoHandedWeapon(arg0)]);
        arg0->unk_AE8 = -1;
    }
    func_80832F24(arg0);
    if ((func_8083FE38(arg0, arg1) == 0) && (arg0->unk_AE8 != 0)) {
        func_80840F34(arg0);
        if (arg0->unk_AE8 < 0) {
            if (arg0->unk_B08[0] >= 0.1f) {
                arg0->unk_ADD = 0;
                arg0->unk_AE8 = 1;
                return;
            }
            if (~(D_80862B44->cur.button | ~0x4000) != 0) {
                func_80840E5C(arg0, arg1);
            }
        } else if (func_80840CD4(arg0, arg1) == 0) {
            func_80832F78(arg0, &sp34, &sp32, 0.0f, arg1);
            temp_v0 = func_8083E7F8(arg0, &sp34, &sp32, arg1);
            if (temp_v0 > 0) {
                func_80840DEC(arg0, arg1);
                return;
            }
            if (temp_v0 < 0) {
                func_80840E24(arg0, arg1);
            }
        }
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8084CE84.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8084D18C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8084D4EC.s")

void func_8084D770(Player* this, PlayState* play) {
    s32 temp_v0;

    this->stateFlags2 |= (PLAYER_STATE2_20 | PLAYER_STATE2_40);
    if (this->unk_D58 == func_80837BF8) {
        this->stateFlags2 |= PLAYER_STATE2_1;
    }

    LinkAnimation_Update(play, &this->skelAnime);
    func_8083249C(this);

    temp_v0 = func_8083216C(this, play);
    if (((this->stateFlags1 & PLAYER_STATE1_800) && (this->heldActor != NULL) && (this->getItemId == GI_NONE)) ||
        (temp_v0 == 0)) {
        this->unk_D58(play, this);
    }
}

void func_8084D820(Player* this, PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8084D820.s")

// door stuff
void func_8084E034(Player* this, PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8084E034.s")

// grab/hold an actor (?)
void func_8084E25C(Player* this, PlayState* play) {
    func_80832F24(this);

    if (LinkAnimation_Update(play, &this->skelAnime)) {
        func_80836988(this, play);
        func_808313F0(this, play);
    } else if (LinkAnimation_OnFrame(&this->skelAnime, 4.0f)) {
        Actor* interactRangeActor;

        interactRangeActor = this->interactRangeActor;
        if (!func_808313A8(play, this, interactRangeActor)) {
            this->actor.child = interactRangeActor;
            this->heldActor = interactRangeActor;
            interactRangeActor->parent = &this->actor;
            interactRangeActor->bgCheckFlags &= ~0xFF;
            this->leftHandWorld.rot.y = interactRangeActor->shape.rot.y - this->actor.shape.rot.y;
        }
    } else {
        Math_ScaledStepToS(&this->leftHandWorld.rot.y, 0, 0xFA0);
    }
}

void func_8084E434(Player* this, PlayState* play);

// grab/hold an actor (?) What's the difference with func_8084E25C ?
void func_8084E334(Player* this, PlayState* play) {
    func_80832F24(this);

    if (LinkAnimation_Update(play, &this->skelAnime)) {
        func_8082DB3C(play, this, &D_0400DFA0);
        this->unk_AE8 = 1;
    } else if (this->unk_AE8 == 0) {
        if (LinkAnimation_OnFrame(&this->skelAnime, 27.0f)) {
            Actor* interactRangeActor = this->interactRangeActor;

            this->heldActor = interactRangeActor;
            this->actor.child = interactRangeActor;
            interactRangeActor->parent = &this->actor;
        } else if (LinkAnimation_OnFrame(&this->skelAnime, 25.0f)) {
            func_8082DF8C(this, NA_SE_VO_LI_SWORD_L);
        }
    } else if (CHECK_BTN_ANY(D_80862B44->press.button, BTN_CRIGHT | BTN_CLEFT | BTN_CDOWN | BTN_B | BTN_A)) {
        func_80831494(play, this, func_8084E434, 1);
        func_8082DB18(play, this, &D_0400DF98);
    }
}

// throw held actor (?)
void func_8084E434(Player* this, PlayState* play) {
    if (LinkAnimation_Update(play, &this->skelAnime)) {
        func_80836988(this, play);
    } else if (LinkAnimation_OnFrame(&this->skelAnime, 6.0f)) {
        Actor* heldActor = this->heldActor;

        heldActor->world.rot.y = this->actor.shape.rot.y;
        heldActor->speedXZ = 10.0f;
        heldActor->velocity.y = 20.0f;
        func_808309CC(play, this);
        func_800B8E58(this, NA_SE_PL_THROW);
        func_8082DF8C(this, NA_SE_VO_LI_SWORD_N);
    }
}

void func_8084E4E4(Player* this, PlayState* play) {
    if (LinkAnimation_Update(play, &this->skelAnime)) {
        func_8082DB3C(play, this, &D_0400DD80);
        this->unk_AE8 = 0xF;
    } else if (this->unk_AE8 != 0) {
        this->unk_AE8--;
        if (this->unk_AE8 == 0) {
            func_80836A98(this, &D_0400DD78, play);
            this->stateFlags1 &= ~PLAYER_STATE1_800;
            func_8082DF8C(this, NA_SE_VO_LI_DAMAGE_S);
        }
    }
}

void func_8084E58C(Player* this, PlayState* play) {
    func_80832F24(this);

    if (LinkAnimation_Update(play, &this->skelAnime)) {
        func_80836988(this, play);
    } else if (LinkAnimation_OnFrame(&this->skelAnime, 4.0f)) {
        Actor* heldActor = this->heldActor;

        if (!func_808313A8(play, this, heldActor)) {
            heldActor->velocity.y = 0.0f;
            heldActor->speedXZ = 0.0f;
            func_808309CC(play, this);
            if (heldActor->id == ACTOR_EN_BOM_CHU) {
                func_80831814(this, play, 0);
            }
        }
    }
}

void func_8084E65C(Player* this, PlayState* play) {
    f32 sp34;
    s16 sp32;

    func_80832F24(this);

    if (LinkAnimation_Update(play, &this->skelAnime) ||
        ((this->skelAnime.curFrame >= 8.0f) && func_80832F78(this, &sp34, &sp32, 0.018f, play))) {
        func_80836988(this, play);
    } else if (LinkAnimation_OnFrame(&this->skelAnime, 3.0f)) {
        func_808409A8(play, this, this->linearVelocity + 8.0f, 12.0f);
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8084E724.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8084E980.s")

void func_8084ED9C(Player* arg0, PlayState* arg1) {
    f32 sp34;
    s16 sp32;
    s32 temp_v0;

    arg0->stateFlags2 |= 0x141;
    func_8083DEE4(arg1, arg0);
    if (LinkAnimation_Update(arg1, &arg0->skelAnime) && !func_8083E14C(arg1, arg0)) {
        func_80832F78(arg0, &sp34, &sp32, 0.0f, arg1);
        temp_v0 = func_8083E758(arg0, &sp34, &sp32);
        if (temp_v0 > 0) {
            func_8083E234(arg0, arg1);
        } else if (temp_v0 < 0) {
            func_8083E28C(arg0, arg1);
        }
    }
}

extern struct_8082E224_arg1 D_8085D650[];
extern struct_8082E224_arg1 D_8085D658[];

void func_8084EE50(Player* arg0, PlayState* arg1) {
    arg0->stateFlags2 |= (PLAYER_STATE1_1 | PLAYER_STATE1_40 | PLAYER_STATE1_100);

    if (func_8082E67C(arg1, arg0, &D_0400DE10)) {
        arg0->unk_AE8 = 1;
    } else if ((arg0->unk_AE8 == 0) && LinkAnimation_OnFrame(&arg0->skelAnime, 11.0f)) {
        func_8082DF8C(arg0, NA_SE_VO_LI_PUSH);
    }

    func_8082E224(arg0, D_8085D650);
    func_8083DEE4(arg1, arg0);

    if (!func_8083E14C(arg1, arg0)) {
        f32 sp34;
        s16 sp32;
        s32 temp_v0;

        func_80832F78(arg0, &sp34, &sp32, 0.0f, arg1);
        temp_v0 = func_8083E758(arg0, &sp34, &sp32);
        if (temp_v0 < 0) {
            func_8083E28C(arg0, arg1);
        } else if (temp_v0 == 0) {
            func_8083DF38(arg0, &D_0400DDF0, arg1);
        } else {
            arg0->stateFlags2 |= PLAYER_STATE2_10;
        }
    }

    if (arg0->stateFlags2 & PLAYER_STATE2_10) {
        func_808479F4(arg1, arg0, 2.0f);
        arg0->linearVelocity = 2.0f;
    }
}

extern Vec3f D_8085D660;

void func_8084EF9C(Player* arg0, PlayState* arg1) {
    LinkAnimationHeader* anim = GET_PLAYER_ANIM(PLAYER_ANIMGROUP_35, arg0->modelAnimType);

    arg0->stateFlags2 |= (PLAYER_STATE1_1 | PLAYER_STATE1_40 | PLAYER_STATE1_100);

    if (func_8082E67C(arg1, arg0, anim)) {
        arg0->unk_AE8 = 1;
    } else if (arg0->unk_AE8 == 0) {
        if (LinkAnimation_OnFrame(&arg0->skelAnime, 11.0f)) {
            func_8082DF8C(arg0, NA_SE_VO_LI_PUSH);
        }
        if (0) {}
    } else {
        func_8082E224(arg0, D_8085D658);
    }

    func_8083DEE4(arg1, arg0);
    if (!func_8083E14C(arg1, arg0)) {
        f32 sp78;
        s16 sp76;
        s32 temp_v0;

        func_80832F78(arg0, &sp78, &sp76, 0.0f, arg1);

        temp_v0 = func_8083E758(arg0, &sp78, &sp76);
        if (temp_v0 > 0) {
            func_8083E234(arg0, arg1);
        } else if (temp_v0 == 0) {
            func_8083DF38(arg0, GET_PLAYER_ANIM(PLAYER_ANIMGROUP_36, arg0->modelAnimType), arg1);
        } else {
            arg0->stateFlags2 |= PLAYER_STATE2_10;
        }
    }

    if (arg0->stateFlags2 & PLAYER_STATE2_10) {
        Vec3f sp64;
        f32 temp;
        CollisionPoly* poly;
        s32 bgId;
        Vec3f sp4C;
        Vec3f sp40;

        temp = func_80835D2C(arg1, arg0, &D_8085D660, &sp64) - arg0->actor.world.pos.y;
        if (fabsf(temp) < 26.800001f) {
            sp64.y -= 7.0f;
            sp4C.x = arg0->actor.world.pos.x;
            sp4C.z = arg0->actor.world.pos.z;
            sp4C.y = sp64.y;
            if (!BgCheck_EntityLineTest2(&arg1->colCtx, &sp4C, &sp64, &sp40, &poly, 1, 0, 0, 1, &bgId, &arg0->actor)) {
                func_808479F4(arg1, arg0, -2.0f);
                return;
            }
        }
        arg0->stateFlags2 &= ~PLAYER_STATE2_10;
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8084F1B8.s")

void func_8084F3DC(Player* arg0, PlayState* arg1) {
    arg0->stateFlags2 |= PLAYER_STATE2_40;

    if (LinkAnimation_Update(arg1, &arg0->skelAnime)) {
        arg0->currentYaw = arg0->skelAnime.jointTable[1].y + arg0->actor.shape.rot.y;
        func_8082E820(arg0, 1);
        arg0->actor.shape.rot.y = arg0->currentYaw;
        func_80839E74(arg0, arg1);
        arg0->stateFlags1 &= ~(PLAYER_STATE1_4 | PLAYER_STATE1_2000 | PLAYER_STATE1_4000);
    } else if (LinkAnimation_OnFrame(&arg0->skelAnime, arg0->skelAnime.endFrame - 6.0f)) {
        func_8082E1BC(arg0);
    } else if (LinkAnimation_OnFrame(&arg0->skelAnime, arg0->skelAnime.endFrame - 34.0f)) {
        func_800B8E58(arg0, NA_SE_PL_CLIMB_CLIFF);
        func_8082DF8C(arg0, NA_SE_VO_LI_CLIMB_END);
        func_8084C124(arg1, arg0);
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8084F4E8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8084FC0C.s")

void func_8084FD7C(PlayState* play, Player* this, Actor* actor) {
    s16 var_a3;

    if (this->unk_B86[0] != 0) {
        this->unk_B86[0]--;
        return;
    }

    this->unk_AB2.y = func_80847190(play, this, 1) - this->actor.shape.rot.y;

    var_a3 = ABS_ALT(this->unk_AB2.y) - 0x4000;
    if (var_a3 > 0) {
        var_a3 = CLAMP_MAX(var_a3, 0x15E);
        actor->shape.rot.y += var_a3 * ((this->unk_AB2.y >= 0) ? 1 : -1);
        actor->world.rot.y = actor->shape.rot.y;
    }

    this->unk_AB2.y += 0x2710;
    this->unk_AA8 = -0x1388;
}

s32 func_8084FE48(Player* this) {
    return this->unk_730 == NULL && !func_8082FC24(this);
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8084FE7C.s")

extern struct_8082E224_arg1 D_8085D708[];

void func_808505D0(Player* this, PlayState* play) {
    this->stateFlags2 |= 0x40;
    func_80847E2C(this, 1.0f, 10.0f);

    if (LinkAnimation_Update(play, &this->skelAnime) != 0) {
        Actor* rideActor = this->rideActor;

        Camera_ChangeSetting(Play_GetCamera(play, 0), 1);
        func_80839E74(this, play);

        this->stateFlags1 &= ~PLAYER_STATE1_800000;
        this->actor.parent = NULL;
        D_801BDA9C = 0;
        if (CHECK_QUEST_ITEM(QUEST_SONG_EPONA) || (gGameInfo->data[0x2A1] != 0)) {
            gSaveContext.save.horseData.scene = play->sceneNum;
            gSaveContext.save.horseData.pos.x = rideActor->world.pos.x;
            gSaveContext.save.horseData.pos.y = rideActor->world.pos.y;
            gSaveContext.save.horseData.pos.z = rideActor->world.pos.z;
            gSaveContext.save.horseData.yaw = rideActor->shape.rot.y;
        }
    } else {
        if (this->mountSide < 0) {
            D_8085D708[0].unk_2 = 0x2828;
        } else {
            D_8085D708[0].unk_2 = 0x281D;
        }

        func_8082E224(this, D_8085D708);
    }
}

s32 func_80850734(PlayState* play, Player* this) {
    if ((this->transformation == PLAYER_STRENGTH_ZORA) && (this->windSpeed == 0.0f) &&
        (this->currentBoots < PLAYER_BOOTS_ZORA_UNDERWATER) && CHECK_BTN_ALL(D_80862B44->cur.button, BTN_A)) {
        func_8083B850(play, this);
        this->stateFlags2 |= PLAYER_STATE2_400;
        LinkAnimation_Change(play, &this->skelAnime, &D_0400E418, 2.0f / 3.0f, 4.0f,
                             Animation_GetLastFrame(&D_0400E418), 2, -6.0f);
        this->unk_AE8 = 5;
        this->unk_B86[0] = 0;
        this->unk_B48 = this->linearVelocity;
        this->actor.velocity.y = 0.0f;
        func_800B8E58(this, NA_SE_PL_ZORA_SWIM_DASH);
        return true;
    }
    return false;
}

s32 func_80850854(PlayState* play, Player* this) {
    if ((this->transformation == PLAYER_FORM_DEKU) && (this->unk_B67 != 0) &&
        (gSaveContext.save.playerData.health != 0) && (D_80862AFC != 0.0f)) {
        func_808373F8(play, this, 0);
        return true;
    }
    return false;
}

void func_808508C8(Player* this, PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_808508C8.s")

void func_80850B18(Player* this, PlayState* play) {
    if (!func_80838A90(this, play)) {
        this->stateFlags2 |= PLAYER_STATE2_20;
        func_808477D0(play, this, NULL, this->linearVelocity);
        func_808475B4(this);

        if (DECR(this->unk_AE8) == 0) {
            func_808353DC(play, this);
        }
    }
}

void func_80850BA8(Player* this) {
    this->linearVelocity = Math_CosS(this->unk_AAA) * this->unk_B48;
    this->actor.velocity.y = -Math_SinS(this->unk_AAA) * this->unk_B48;
}

void func_80850BF8(Player* this, f32 arg1) {
    f32 temp_fv0;
    s16 temp_ft0;

    Math_AsymStepToF(&this->unk_B48, arg1, 1.0f, (fabsf(this->unk_B48) * 0.01f) + 0.4f);
    temp_fv0 = Math_CosS(D_80862B44->rel.stick_x * 0x10E);

    temp_ft0 = (((D_80862B44->rel.stick_x >= 0) ? 1 : -1) * (1.0f - temp_fv0) * -1100.0f);
    temp_ft0 = CLAMP(temp_ft0, -0x1F40, 0x1F40);

    this->currentYaw += temp_ft0;
}

void func_80850D20(PlayState* play, Player* this);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80850D20.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80850D68.s")

extern s8 D_8085D048[];

void func_808513EC(Player* this, PlayState* play) {
    f32 sp34;
    s16 sp32;
    s16 sp30;
    s16 var_v0;

    this->stateFlags2 |= PLAYER_STATE2_20;
    func_808475B4(this);
    func_8082F164(this, BTN_R);
    if ((func_80833058(play, this, D_8085D048, 1) == 0) && !func_8083B3B4(play, this, D_80862B44) &&
        !func_80850854(play, this)) {
        func_808477D0(play, this, D_80862B44, this->linearVelocity);
        if (func_8082DA90(play)) {
            sp34 = this->linearVelocity;
            sp32 = this->actor.shape.rot.y;
        } else {
            func_80832F78(this, &sp34, &sp32, 0.0f, play);
        }
        sp30 = this->actor.shape.rot.y - sp32;
        if (!func_80850734(play, this)) {
            if (func_8082FC24(this) || func_80847ED4(this)) {
                func_80848048(play, this);
            } else {
                if ((sp34 == 0.0f) || (ABS_ALT(sp30) > 0x6000) ||
                    (this->currentBoots >= PLAYER_BOOTS_ZORA_UNDERWATER)) {
                    func_808353DC(play, this);
                }
            }
            func_80847FF8(this, &this->linearVelocity, sp34, sp32);
        }
    }
}

void func_80851588(Player* this, PlayState* play) {
    f32 sp2C;
    s16 sp2A;

    func_808477D0(play, this, D_80862B44, this->linearVelocity);
    func_808475B4(this);
    func_8082F164(this, BTN_R);

    if ((func_80833058(play, this, D_8085D048, 1) == 0) && !func_8083B3B4(play, this, D_80862B44)) {
        func_80832F78(this, &sp2C, &sp2A, 0.0f, play);

        if (sp2C == 0.0f) {
            func_808353DC(play, this);
        } else if (!func_8082FC24(this) && !func_80847ED4(this)) {
            func_8083B73C(play, this, sp2A);
        } else {
            func_80848094(play, this, &sp2C, &sp2A);
        }

        func_80847FF8(this, &this->linearVelocity, sp2C, sp2A);
    }
}

void func_808516B4(Player* this, PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_808516B4.s")

void func_808519FC(Player* this, PlayState* play) {
    this->stateFlags2 |= PLAYER_STATE2_20;

    func_8082F164(this, BTN_R);
    if (((this->stateFlags1 & PLAYER_STATE1_400) || (this->skelAnime.curFrame <= 1.0f) || !func_80850734(play, this)) &&
        LinkAnimation_Update(play, &this->skelAnime)) {
        if (!(this->stateFlags1 & PLAYER_STATE1_400) || func_808482E0(play, this)) {
            func_80848250(play, this);
            func_808353DC(play, this);
            func_8082DC64(play, this);
        }
    } else {
        if ((this->stateFlags1 & PLAYER_STATE1_400) && LinkAnimation_OnFrame(&this->skelAnime, 10.0f)) {
            func_8082ECE0(this);
            func_8082DC64(play, this);
        } else if (LinkAnimation_OnFrame(&this->skelAnime, 5.0f)) {
            func_8082DF8C(this, NA_SE_VO_LI_BREATH_DRINK);
        }
    }

    func_808475B4(this);
    func_8084748C(this, &this->linearVelocity, 0.0f, this->actor.shape.rot.y);
}

void func_80851B58(Player* this, PlayState* play) {
    func_808475B4(this);
    Math_StepToF(&this->linearVelocity, 0.0f, 0.4f);
    if (LinkAnimation_Update(play, &this->skelAnime) && (this->linearVelocity < 10.0f)) {
        func_808353DC(play, this);
    }
}

void func_80851BD4(Player* this, PlayState* play) {
    func_808475B4(this);
    if (LinkAnimation_Update(play, &this->skelAnime) && (this == GET_PLAYER(play))) {
        func_80840770(play, this);
    }
    func_8084748C(this, &this->linearVelocity, 0.0f, this->actor.shape.rot.y);
}

s32 func_80851C40(PlayState* play, Player* this);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80851C40.s")

void func_80851D30(PlayState* play, Player* this);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80851D30.s")

void func_80851EAC(Player* this) {
    this->unk_B86[0] = -1;
    this->unk_B88 = -1;
    this->unk_B08[2] = 0.0f;
}

typedef struct struct_8085D714 {
    /* 0x0 */ u8 unk_0;
    /* 0x4 */ LinkAnimationHeader* unk_4;
} struct_8085D714; // size = 0x8

extern struct_8085D714 D_8085D714[];

#if NON_EQUIVALENT
void func_80851EC8(PlayState* play, Player* this) {
    u8 temp_v1;

    temp_v1 = play->msgCtx.unk12048;
    this->unk_B86[D_8085D714[temp_v1].unk_0] = temp_v1;
    this->unk_B08[temp_v1 + 2] = 3.0f;
}
#else
void func_80851EC8(PlayState* play, Player* this);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80851EC8.s")
#endif

void func_80851F18(PlayState* play, Player* this);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80851F18.s")

void func_808521E0(PlayState* play, Player* this) {
    if (func_80851C40(play, this) != 0) {
        if (this->skelAnime.animation != &D_0400E1F0) {
            func_8082DB60(play, this, &D_0400E1F0);
        }

        func_80124618(D_801C0490, this->skelAnime.curFrame, &this->unk_AF0[1]);
    } else if (play->msgCtx.ocarinaMode == 1) {
        if (play->msgCtx.unk12048 != 0xFF) {
            func_80851EC8(play, this);
        }

        func_80851F18(play, this);
    }
}

void func_80852290(PlayState* play, Player* this);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80852290.s")

void func_8085255C(PlayState* play, Player* this) {
    if (this->transformation == PLAYER_FORM_DEKU) {
        func_80851D30(play, this);
    } else if (this->transformation == PLAYER_FORM_GORON) {
        func_808521E0(play, this);
    } else if (this->transformation == PLAYER_FORM_ZORA) {
        func_80852290(play, this);
    }
}

void func_808525C4(PlayState* arg0, Player* arg1) {
    if (arg1->unk_AE8++ >= 3) {
        if ((arg1->transformation == PLAYER_FORM_ZORA) || (arg1->transformation == PLAYER_FORM_DEKU)) {
            func_8082E5A8(arg0, arg1, D_8085D190[arg1->transformation]);
        } else if (arg1->transformation == PLAYER_FORM_GORON) {
            func_80851EAC(arg1);
            func_8082DB60(arg0, arg1, &D_0400E200);
        } else {
            func_8082DB60(arg0, arg1, D_8085D190[arg1->transformation]);
        }

        arg1->unk_B48 = 1.0f;
    }
}

void func_8085269C(Player* this, PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8085269C.s")

void func_80852B28(Player* this, PlayState* arg1) {
    func_80832F24(this);

    if (LinkAnimation_Update(arg1, &this->skelAnime)) {
        func_80836A98(this, &D_0400DD40, arg1);
    } else if (LinkAnimation_OnFrame(&this->skelAnime, 3.0f)) {
        if (Actor_Spawn(&arg1->actorCtx, arg1, 0xF, this->bodyPartsPos[0xF].x, this->bodyPartsPos[0xF].y,
                        this->bodyPartsPos[0xF].z, 0xFA0, this->actor.shape.rot.y, 0, 8) != NULL) {
            Inventory_ChangeAmmo(SLOT_NUT, -1);
            this->unk_D57 = 4;
        }

        func_8082DF8C(this, NA_SE_VO_LI_SWORD_N);
    }
}

void func_80852C04(Player* this, PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80852C04.s")

extern struct_8082E224_arg1 D_8085D75C[];

void func_80852FD4(Player* this, PlayState* play) {
    if (LinkAnimation_Update(play, &this->skelAnime)) {
        if (this->unk_AE7 == 0) {
            if (DECR(this->unk_AE8) == 0) {
                this->unk_AE7 = 1;
                this->skelAnime.endFrame = this->skelAnime.animLength - 1.0f;
            }
        } else {
            func_80839E74(this, play);
        }
    } else if ((this->transformation == PLAYER_FORM_FIERCE_DEITY) && LinkAnimation_OnFrame(&this->skelAnime, 158.0f)) {
        func_8082DF8C(this, NA_SE_VO_LI_SWORD_N);
    } else if (this->transformation != PLAYER_FORM_FIERCE_DEITY) {
        func_8082E224(this, D_8085D75C);
    } else {
        func_808484CC(this);
    }
}

extern Vec3f D_8085D764;
extern Vec3f D_8085D770;
extern Vec3f D_8085D77C;
extern Color_RGBA8 D_8085D788;
extern Color_RGBA8 D_8085D78C;

void func_808530E0(PlayState* play, Player* this) {
    Vec3f pos;
    Vec3f velocity;
    Vec3f accel;

    func_80835BC8(this, &this->actor.world.pos, &D_8085D764, &pos);
    func_80835BC8(this, &gZeroVec3f, &D_8085D770, &velocity);
    func_80835BC8(this, &gZeroVec3f, &D_8085D77C, &accel);
    func_800B0EB0(play, &pos, &velocity, &accel, &D_8085D788, &D_8085D78C, 40, 10, 10);
}

void func_80853194(Player* this, PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80853194.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_808534C0.s")

extern Vec3f D_8085D7EC;

void func_80853754(Player* arg0, PlayState* arg1) {
    func_808323C0(arg0, arg1->playerActorCsIds[2]);

    if (LinkAnimation_Update(arg1, &arg0->skelAnime)) {
        func_80838760(arg0);
        func_80839E74(arg0, arg1);
        return;
    } else if (LinkAnimation_OnFrame(&arg0->skelAnime, 37.0f)) {
        s32 sp2C = 8;

        func_800B8E58(arg0, NA_SE_EV_BOTTLE_CAP_OPEN);
        func_8082DF8C(arg0, NA_SE_VO_LI_AUTO_JUMP);
        if (arg0->heldItemActionParam == PLAYER_AP_BOTTLE_FAIRY) {
            func_80123D50(arg1, arg0, ITEM_BOTTLE, PLAYER_AP_BOTTLE);
            func_800B8E58(arg0, NA_SE_EV_FIATY_HEAL - SFX_FLAG);
            sp2C = 1;
        }

        func_80835C64(arg1, arg0, &arg0->leftHandWorld.pos, &D_8085D7EC, sp2C);
    }
}

typedef struct struct_8085D80C {
    /* 0x0 */ s16 actorId;
    /* 0x2 */ s16 params;
} struct_8085D80C; // size = 0x4

extern s8 D_8085D804[];
extern f32 D_8085D800;
extern Vec3f D_8085D7F8;
extern struct_8085D80C D_8085D80C[];
#if 0
struct_8085D80C D_8085D80C[] = {
    { ACTOR_EN_FISH, 0 }, // PLAYER_AP_BOTTLE_FISH
    { ACTOR_OBJ_AQUA, 0 }, // PLAYER_AP_BOTTLE_SPRING_WATER
    { ACTOR_OBJ_AQUA, 1 }, // PLAYER_AP_BOTTLE_HOT_SPRING_WATER
    { ACTOR_EN_ZORAEGG, 0x11 }, // PLAYER_AP_BOTTLE_ZORA_EGG
    { ACTOR_EN_DNP, 1 }, // PLAYER_AP_BOTTLE_DEKU_PRINCESS
    { ACTOR_EN_MUSHI2, 0 }, // PLAYER_AP_BOTTLE_GOLD_DUST
    { ACTOR_EN_MUSHI2, 0 }, // PLAYER_AP_BOTTLE_1C
    { ACTOR_EN_OT, 0x8000 }, // PLAYER_AP_BOTTLE_SEAHORSE
    { ACTOR_EN_MUSHI2, 0 }, // PLAYER_AP_BOTTLE_MUSHROOM
    { ACTOR_EN_MUSHI2, 0 }, // PLAYER_AP_BOTTLE_HYLIAN_LOACH
    { ACTOR_EN_MUSHI2, 0 }, // PLAYER_AP_BOTTLE_BUG
};
#endif
extern struct_8082E224_arg1 D_8085D838[];

void func_80853850(Player* arg0, PlayState* arg1) {
    CollisionPoly* sp6C;
    s32 sp68;
    Vec3f sp5C;
    f32 temp_fa0;
    f32 temp_fv0;
    f32 temp_fv1;
    struct_8085D80C* sp4C;

    D_8085D800 = D_8085D804[arg0->transformation];
    if (func_80835D58(arg1, arg0, &D_8085D7F8, &sp6C, &sp68, &sp5C)) {
        temp_fv1 = arg0->actor.world.pos.x - sp5C.x;
        temp_fa0 = arg0->actor.world.pos.z - sp5C.z;
        temp_fv0 = sqrtf(SQ(temp_fv1) + SQ(temp_fa0));

        if (temp_fv0 != 0.0f) {
            temp_fv0 = 3.0f / temp_fv0;

            arg0->actor.world.pos.x = arg0->actor.world.pos.x + (temp_fv1 * temp_fv0);
            arg0->actor.world.pos.z = arg0->actor.world.pos.z + (temp_fa0 * temp_fv0);
        }
    }

    func_80832F24(arg0);
    func_8083249C(arg0);

    if (LinkAnimation_Update(arg1, &arg0->skelAnime)) {
        func_80838760(arg0);
        if (func_80838A90(arg0, arg1) == 0) {
            func_80839E74(arg0, arg1);
        }
    } else if (LinkAnimation_OnFrame(&arg0->skelAnime, 76.0f)) {
        sp4C = &D_8085D80C[arg0->heldItemActionParam - PLAYER_AP_BOTTLE - 1];

        Actor_Spawn(&arg1->actorCtx, arg1, sp4C->actorId,
                    (Math_SinS(arg0->actor.shape.rot.y) * 5.0f) + arg0->leftHandWorld.pos.x, arg0->leftHandWorld.pos.y,
                    (Math_CosS(arg0->actor.shape.rot.y) * 5.0f) + arg0->leftHandWorld.pos.z, 0x4000,
                    arg0->actor.shape.rot.y, 0, sp4C->params);
        func_80123D50(arg1, arg0, ITEM_BOTTLE, PLAYER_AP_BOTTLE);
    } else {
        func_8082E224(arg0, D_8085D838);
    }
}

void func_80853A5C(Player *this, PlayState *play);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80853A5C.s")

void func_80853CC0(Player* arg0, PlayState* arg1) {
    arg0->stateFlags2 |= (PLAYER_STATE2_20 | PLAYER_STATE2_40);

    if (LinkAnimation_Update(arg1, &arg0->skelAnime)) {
        func_8082DB3C(arg1, arg0, &D_0400DE30);
    }

    if (arg1->sceneNum != SCENE_SEA_BS) {
        func_8082F164(arg0, BTN_R);
    }

    if (func_8082DE88(arg0, 0, 0x64)) {
        func_80836988(arg0, arg1);
        arg0->stateFlags2 &= ~PLAYER_STATE2_80;
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80853D68.s")

void func_80859CE0(PlayState* play, Player* this, s32 arg2);

void func_80854010(Player* this, PlayState* play) {
    if ((DECR(this->unk_AE8) == 0) && (func_808387A0(play, this) != 0)) {
        func_80859CE0(play, this, 0);
        func_80831494(play, this, func_8085B08C, 0);
        func_8085B08C(this, play);
    }
}

void func_808540A0(Player* this, PlayState* play) {
    func_80831494(play, this, func_80854010, 0);
    this->unk_AE8 = 0x28;
    Actor_Spawn(&play->actorCtx, play, 0x49, 0.0f, 0.0f, 0.0f, 0, 0, 0, 0x10);
}

void func_8085AB58(Player* this, CsCmdActorAction* playerAction);

void func_80854118(Player* arg0, PlayState* arg1) {
    if (D_80862B18 < 150.0f) {
        if (LinkAnimation_Update(arg1, &arg0->skelAnime) != 0) {
            if (arg0->unk_AE8 == 0) {
                if (arg0->actor.bgCheckFlags & 1) {
                    arg0->skelAnime.endFrame = arg0->skelAnime.animLength - 1.0f;
                    func_8082E1BC(arg0);
                    arg0->unk_AE8 = 1;
                }
            } else {
                func_8085B384(arg0, arg1);
            }
        }
        Math_SmoothStepToF(&arg0->actor.velocity.y, 2.0f, 0.3f, 8.0f, 0.5f);
    }

    if (arg1->csCtx.state != 0) {
        if (arg1->csCtx.playerAction != NULL) {
            s32 pad;
            f32 sp28;

            sp28 = arg0->actor.world.pos.y;
            func_8085AB58(arg0, arg1->csCtx.playerAction);
            arg0->actor.world.pos.y = sp28;
        }
    }
}

void func_8085421C(Player* this, PlayState* play) {
    if (this->skelAnime.animation == NULL) {
        this->stateFlags2 |= PLAYER_STATE2_4000;
    } else {
        LinkAnimation_Update(play, &this->skelAnime);
        if ((this->skelAnime.animation == &D_0400D698) && LinkAnimation_OnFrame(&this->skelAnime, 60.0f)) {
            func_8082E094(this, 0x850);
            func_8082DF8C(this, NA_SE_VO_LI_DAMAGE_S);
        }
    }

    if ((this->unk_AE8++ >= 9) && !func_8082DA90(play)) {
        if (this->unk_AE7 != 0) {
            if (this->unk_AE7 < 0) {
                func_80169FDC(&play->state);
            } else {
                func_80169EFC(&play->state);
            }
            if (!SurfaceType_IsWallDamage(&play->colCtx, this->actor.floorPoly, this->actor.floorBgId)) {
                gSaveContext.respawnFlag = -5;
            }

            play->transitionType = 4;
            play_sound(NA_SE_OC_ABYSS);
        } else {
            play->transitionType = 2;
            gSaveContext.nextTransitionType = 2;
            gSaveContext.seqIndex = 0xFF;
            gSaveContext.nightSeqIndex = 0xFF;
        }

        play->transitionTrigger = TRANS_TRIGGER_START;
    }
}

void func_8085437C(Player* this, PlayState* play) {
    func_808365DC(this, play);
}

void func_8085439C(Player* this, PlayState* play) {
    this->actor.gravity = -1.0f;
    LinkAnimation_Update(play, &this->skelAnime);

    if (this->actor.velocity.y < 0.0f) {
        func_80833AA0(this, play);
    } else if (this->actor.velocity.y < 6.0f) {
        Math_StepToF(&this->linearVelocity, 3.0f, 0.5f);
    }
}

void func_80854430(Player* this, PlayState* play) {
    if (play->unk_1887C < 0) {
        play->unk_1887C = 0;
        func_80839ED0(this, play);
    } else if (this->unk_AE7 == 0) {
        if ((play->sceneNum != SCENE_20SICHITAI) && CHECK_BTN_ALL(D_80862B44->press.button, BTN_B)) {
            play->unk_1887C = 0xA;
            func_80847880(play, this);
            func_80831494(play, this, func_80854430, 1);
            this->unk_AE7 = 1;
        } else {
            play->unk_1887C = 0;
            func_80847190(play, this, 0);

            if (play->actorCtx.unk5 & 4) {
                this->stateFlags1 |= PLAYER_STATE1_100000;
                func_8083868C(play, this);
            } else {
                this->stateFlags1 &= ~PLAYER_STATE1_100000;
                if ((play->sceneNum == SCENE_20SICHITAI) &&
                    (func_8012364C(play, this, func_8082FDC4()) == ITEM_PICTO_BOX)) {
                    s32 requiredScopeTemp;

                    play->actorCtx.unk5 |= 4;
                }
            }
        }
    } else if (CHECK_BTN_ANY(D_80862B44->press.button, BTN_CRIGHT | BTN_CLEFT | BTN_CDOWN | BTN_CUP | BTN_R | BTN_A)) {
        play->unk_1887C = -1;
        func_80854614(this, play);
        func_80831494(play, this, func_80854430, 0);
        this->unk_AE7 = 0;
    } else {
        play->unk_1887C = 0xA;
        func_80854614(this, play);
    }
}

void func_80854614(Player* this, PlayState* play) {
    this->unk_AA5 = 3;
    func_8083868C(play, this);
    LinkAnimation_Update(play, &this->skelAnime);
    func_8083216C(this, play);
    this->unk_AB2.y = func_80847190(play, this, 1) - this->actor.shape.rot.y;
    this->unk_AA6 |= 0x80;

    if (play->unk_1887C < 0) {
        play->unk_1887C++;
        if (play->unk_1887C == 0) {
            func_80839ED0(this, play);
        }
    }
}

void func_808546D0(Player* this, PlayState* play) {
    if (this->unk_AE7 >= 0) {
        if (this->unk_AE7 < 6) {
            this->unk_AE7++;
        } else {
            this->unk_B48 = (this->unk_AE7 >> 1) * 22.0f;
            if (func_8082DE88(this, 1, 0x64)) {
                this->unk_AE7 = -1;
                EffectSsIcePiece_SpawnBurst(play, &this->actor.world.pos, this->actor.scale.x);
                func_800B8E58(this, NA_SE_PL_ICE_BROKEN);
            }

            if (this->transformation == PLAYER_FORM_ZORA) {
                func_80834104(play, this);
                this->skelAnime.animation = NULL;
                this->unk_AE8 = -0x28;
                this->unk_AE7 = 1;
                this->linearVelocity = 0.0f;
            } else if (play->gameplayFrames % 4 == 0) {
                func_8085B3E0(play, -1);
            }
        }

        this->stateFlags2 |= PLAYER_STATE2_4000;
    } else if (LinkAnimation_Update(play, &this->skelAnime)) {
        func_80836988(this, play);
        func_808339B4(this, 0x14);
    }
}

void func_80854800(Player* this, PlayState* play) {
    LinkAnimation_Update(play, &this->skelAnime);
    func_808345A8(this);

    if (((this->unk_AE8 % 25) != 0) || func_808339D4(play, this, -1)) {
        if (DECR(this->unk_AE8) == 0) {
            func_80836988(this, play);
        }
    }
    this->unk_B65 = 0x28;
    func_800B8F98(&this->actor, this->ageProperties->unk_92 + (NA_SE_VO_LI_TAKEN_AWAY - SFX_FLAG));
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_808548B8.s")

void func_80854C70(Player* this, PlayState* play) {
    LinkAnimation_Update(play, &this->skelAnime);
    func_80832F24(this);
    if (this->skelAnime.curFrame >= 6.0f) {
        func_80836988(this, play);
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80854CD0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80854EFC.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_808550D0.s")

void func_80855218(PlayState* play, Player* this, s32 arg2);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80855218.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_808553F4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80855818.s")

void func_80855A7C(Player* this, PlayState* play) {
    if (this->unk_AE8++ >= 0x5B) {
        play->msgCtx.ocarinaMode = 4;
        func_8085B384(this, play);
        return;
    }
    if (this->unk_AE8 == 0xA) {
        func_80848640(play, this);
    }
}

// giant mask
void func_80855AF4(Player* this, PlayState* play) {
    this->stateFlags2 |= PLAYER_STATE2_40;

    func_80855218(play, this, 0);
    this->unk_AE7++;

    if (!(this->stateFlags1 & PLAYER_STATE1_100)) {
        this->prevMask = this->currentMask;
        gSaveContext.save.equippedMask = this->currentMask = PLAYER_MASK_GIANT;
        func_80115DB4(play, 0, 6);
        this->currentBoots = PLAYER_BOOTS_GIANT;
        this->prevBoots = PLAYER_BOOTS_GIANT;
        func_80123140(play, this);
        func_8085B384(this, play);
    }
}

void func_80855B9C(Player* this, PlayState* play) {
    this->stateFlags2 |= PLAYER_STATE2_40;

    LinkAnimation_Update(play, &this->skelAnime);

    if (!(this->stateFlags1 & PLAYER_STATE1_100)) {
        this->prevMask = this->currentMask;

        gSaveContext.save.equippedMask = this->currentMask = PLAYER_MASK_NONE;

        this->currentBoots = PLAYER_BOOTS_HYLIAN;
        this->prevBoots = PLAYER_BOOTS_HYLIAN;
        func_80123140(play, this);
        func_8085B384(this, play);
    }
}

void func_80855C28(Player* this, PlayState* play) {
    s16 sp3E;
    s32 pad;
    LinkAnimationHeader* sp34;
    s32 var_a0;

    func_808323C0(this, play->playerActorCsIds[4]);
    sp3E = BINANG_SUB(this->actor.shape.rot.y, this->actor.world.rot.y);

    var_a0 = 0;
    if ((this->actor.floorHeight - this->actor.world.pos.y) < 60.0f) {
        Math_StepToF(&this->unk_B08[7], 200.0f, 150.0f);
        var_a0 = Math_StepToS(&this->unk_AE8, 0xFA0, 0x15E);
    }

    this->actor.shape.rot.y += this->unk_AE8;
    this->skelAnime.jointTable->x = 0;
    this->skelAnime.jointTable->z = 0;
    this->unk_ABC += this->unk_B08[7];

    if (this->unk_ABC >= 0.0f) {
        this->unk_ABC = 0.0f;
        if ((var_a0 != 0) && (sp3E < 0)) {
            if (BINANG_SUB(this->actor.shape.rot.y, this->actor.world.rot.y) >= 0) {
                this->actor.shape.rot.y = this->actor.world.rot.y;
                func_80838760(this);
                if (((s32)(this->actor.params & 0xF00) >> 8) == 8) {
                    sp34 = D_8085D17C[this->transformation];
                    func_80836A5C(this, play);
                    LinkAnimation_Change(play, &this->skelAnime, sp34, -2.0f / 3.0f, Animation_GetLastFrame(sp34), 0.0f,
                                         2, -6.0f);
                } else {
                    func_80839E74(this, play);
                }
            }
        }
    } else if (this->unk_AE7 == 0) {
        func_800B8E58(this, NA_SE_PL_WARP_PLATE_OUT);
        this->unk_AE7 = 1;
    }
}

void func_80855E08(Player* this, PlayState* play) {
    this->stateFlags2 |= PLAYER_STATE2_20;

    if (LinkAnimation_Update(play, &this->skelAnime)) {
        func_8082DB3C(play, this, &D_0400D9D0);
    }

    Math_Vec3f_Sum(&this->actor.world.pos, &this->actor.velocity, &this->actor.world.pos);
    if (func_80831124(play, this) != 0) {
        f32 var_fv0;

        Math_Vec3f_Copy(&this->actor.prevPos, &this->actor.world.pos);
        func_80843178(play, this);

        var_fv0 = this->actor.world.pos.y - this->actor.floorHeight;
        var_fv0 = CLAMP_MAX(var_fv0, 20.0f);

        this->actor.world.pos.y = this->actor.world.pos.y - var_fv0;
        this->actor.shape.rot.x = 0;
        this->linearVelocity = 1.0f;
        this->actor.velocity.y = 0.0f;
        this->actor.world.rot.x = this->actor.shape.rot.x;
        func_80833AA0(this, play);
        this->stateFlags2 &= ~PLAYER_STATE2_400;
        this->actor.bgCheckFlags |= 1;
        this->stateFlags3 |= PLAYER_STATE3_10000;
    } else if ((this->skelAnime.animation != &D_0400D9C8) || (this->skelAnime.curFrame >= 4.0f)) {
        this->actor.gravity = 0.0f;
        Math_ScaledStepToS(&this->actor.shape.rot.x, this->actor.world.rot.x, 0x800);
        Player_RequestRumble(play, this, 100, 2, 100, SQ(0));
    }
}

void func_80855F9C(PlayState* arg0, Player* this) {
    f32 sp24;
    s16 sp22;

    this->stateFlags2 |= 0x20;
    func_80832F78(this, &sp24, &sp22, 0.018f, arg0);
    Math_ScaledStepToS(&this->currentYaw, sp22, 0x258);
}

void func_80856000(PlayState* play, Player* this) {
    CollisionPoly* sp3C;
    s32 sp38;
    Vec3f sp2C;
    f32 sp28;

    sp2C.x = this->actor.world.pos.x;
    sp2C.y = this->actor.world.pos.y - 20.0f;
    sp2C.z = this->actor.world.pos.z;
    BgCheck_EntityCheckCeiling(&play->colCtx, &sp28, &sp2C, 30.0f, &sp3C, &sp38, &this->actor);
}

void func_80856074(PlayState* play, Player* this) {
    if (func_8083F8A8(play, this, 12.0f, 4, 0.0f, 10, 50, 1)) {
        EffectSsHahen_SpawnBurst(play, &this->actor.world.pos, 3.0f, 0, 4, 8, 2, -1, 10, NULL);
    }
}

extern f32 D_8085D91C;
extern f32 D_8085D928;
extern Vec3f D_8085D918;
extern Vec3f D_8085D924;
extern Color_RGBA8 D_8085D930;
extern Color_RGBA8 D_8085D934;

void func_80856110(PlayState* play, Player* this, f32 arg2, f32 arg3, f32 arg4, s16 scale, s16 scaleStep, s16 life) {
    Vec3f pos;

    pos.x = this->actor.world.pos.x;
    pos.y = this->actor.world.pos.y + arg2;
    pos.z = this->actor.world.pos.z;

    D_8085D91C = arg3;
    D_8085D928 = arg4;

    func_800B0EB0(play, &pos, &D_8085D918, &D_8085D924, &D_8085D930, &D_8085D934, scale, scaleStep, life);
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_808561B0.s")

extern Color_RGBA8 D_8085D938;
extern Color_RGBA8 D_8085D93C;

extern Vec3f D_8085D940;
extern f32 D_8085D948;
extern Vec3f D_8085D94C;
extern f32 D_8085D954;

void func_808566C0(PlayState* play, Player* this, s32 arg2, f32 arg3, f32 arg4, f32 arg5, s32 life) {
    Color_RGBA8 primColor = D_8085D938;
    Color_RGBA8 envColor = D_8085D93C;
    Vec3f pos;
    s32 scale;
    f32 sp34;
    Vec3f* temp_v0;

    if (Rand_ZeroOne() < 0.5f) {
        sp34 = -1.0f;
    } else {
        sp34 = 1.0f;
    }

    D_8085D940.x = (Rand_ZeroFloat(arg4) + arg3) * sp34;
    D_8085D94C.x = arg5 * sp34;
    if (Rand_ZeroOne() < 0.5f) {
        sp34 = -1.0f;
    } else {
        sp34 = 1.0f;
    }

    temp_v0 = &this->bodyPartsPos[arg2];
    D_8085D948 = (Rand_ZeroFloat(arg4) + arg3) * sp34;
    D_8085D954 = arg5 * sp34;
    pos.x = temp_v0->x;
    pos.y = Rand_ZeroFloat(15.0f) + temp_v0->y;
    pos.z = temp_v0->z;
    if (Rand_ZeroOne() < 0.5f) {
        scale = 0x7D0;
    } else {
        scale = -0x96;
    }

    EffectSsKirakira_SpawnDispersed(play, &pos, &D_8085D940, &D_8085D94C, &primColor, &envColor, scale, life);
}

void func_8085687C(Player* this) {
}

s32 func_80856888(f32* arg0, f32 arg1, f32 arg2) {
    if (arg2 != 0.0f) {
        if (arg1 < *arg0) {
            arg2 = -arg2;
        }
        *arg0 += arg2;
        if (((*arg0 - arg1) * arg2) >= 0.0f) {
            *arg0 = arg1;
            return 1;
        }
    } else if (arg1 == *arg0) {
        return 1;
    }
    return 0;
}

void func_80856918(Player* this, PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80856918.s")

#ifdef NON_EQUIVALENT
// mostly regalloc, dunno about equivalency
void func_808573A4(Player* arg0, PlayState* arg1) {
    arg0->stateFlags2 |= 0x60;

    LinkAnimation_Update(arg1, &arg0->skelAnime);
    func_8082F0E4(arg0, 0x8000U, 1, 0x1E);
    if (func_80838A90(arg0, arg1) == 0) {
        s16 sp46;
        f32 sp40;
        s16 sp3E;
        s32 temp;

        sp46 = arg0->currentYaw;
        func_80832F78(arg0, &sp40, &sp3E, 0.018f, arg1);
        sp40 *= 1.0f - (0.9f * ((11100.0f - arg0->unk_B08[2]) / 11100.0f));
        if (func_8083A4A4(arg0, &sp40, &sp3E, (f32) gGameInfo->data[0x2B] / 100.0f) == 0) {
            func_8083CB58(arg0, sp40, sp3E);
        }

        arg0->unk_B08[2] += -800.0f;
        temp = (arg0->currentYaw - sp46);
        arg0->actor.shape.rot.y += (s32) arg0->unk_B08[2] + temp;

        if (Math_StepToF(&arg0->unk_B08[3], 0.0f, arg0->unk_B08[2]) != 0) {
            arg0->actor.shape.rot.y = arg0->currentYaw;
            func_8083B2E4(arg0, arg1);
        } else if (&D_0400E270 == arg0->skelAnime.animation) {
            arg0->stateFlags3 |= 0x100000;
            if (arg0->unk_B08[3] < 0.0f) {
                func_8082E438(arg1, arg0, func_8082ED20(arg0));
            }
        }

        func_808566C0(arg1, arg0, 0, 1.0f, 0.5f, 0.0f, 0x20);
        if (arg0->unk_B08[2] > 9500.0f) {
            func_8083F8A8(&arg1->state, &arg0->actor, 2.0f, 1, 2.5f, 0xA, 0x12, 1);
        }

        func_800AE930(&arg1->colCtx, Effect_GetByIndex(arg0->meleeWeaponEffectIndex[2]), &arg0->actor.world.pos, 2.0f, arg0->currentYaw, arg0->actor.floorPoly, arg0->actor.floorBgId);
        func_800B8F98(&arg0->actor, func_8082E078(arg0, 0xD0));
    }
}
#else
void func_808573A4(Player* this, PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_808573A4.s")
#endif

void func_80857640(Player* this, f32 arg1, s32 arg2) {
    func_80834CD0(this, arg1, NA_SE_VO_LI_SWORD_N);
    func_800B8E58(this, NA_SE_PL_GORON_BALLJUMP);
    func_8082DABC(this);
    if (this->unk_AE8 < arg2) {
        this->unk_AE8 = arg2;
    }
    this->unk_AE7 = 1;
    this->unk_B48 = 1.0f;
}

#ifdef NON_EQUIVALENT
// dunno if equivalent
void func_808576BC(PlayState* arg0, Player* arg1) {
    s32 sp34;
    f32 sp30;
    s32 var_v0;

    sp30 = Math_CosS(arg1->currentYaw);
    var_v0 = (s32) (((Math_SinS(arg1->currentYaw) * arg1->actor.velocity.x) + (arg1->actor.velocity.z * sp30)) * 800.0f) - arg1->unk_AE8;

    var_v0 = ABS_ALT(var_v0);
    if (var_v0 >= 0x7D1) {
        if (var_v0 >= 0x1771) {
            sp34 = var_v0;
            func_800B8F98(arg1, NA_SE_PL_GORON_SLIP - SFX_FLAG);
        }

        if (func_8083F8A8(arg0, arg1, 12.0f, -1 - (var_v0 >> 0xC), (var_v0 >> 0xA) + 1.0f, (var_v0 >> 7) + 0xA0, 0x14, 1) != 0) {
            func_800B8E58(arg1, (arg1->unk_B72 == 0xE) ? NA_SE_PL_ROLL_SNOW_DUST - SFX_FLAG : NA_SE_PL_ROLL_DUST - SFX_FLAG);
        }
    }
}
#else
void func_808576BC(PlayState* play, Player* this);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_808576BC.s")
#endif

void func_808577E0(Player* this);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_808577E0.s")

s32 func_80857950(PlayState* arg0, Player* arg1) {
    if (((arg1->unk_B88 == 0) && !CHECK_BTN_ALL(D_80862B44->cur.button, BTN_A) ) || ((arg1->unk_AE7 == 3) && (arg1->actor.velocity.y < 0.0f))) {
        func_80831494(arg0, arg1, func_80849FE0, 1);
        Math_Vec3f_Copy(&arg1->actor.world.pos, &arg1->actor.prevPos);
        LinkAnimation_Change(arg0, &arg1->skelAnime, &D_0400E208, -2.0f / 3.0f, 7.0f, 0.0f, 2, 0.0f);
        func_800B8E58(arg1, NA_SE_PL_BALL_TO_GORON);
        return true;
    }

    return false;
}

s32 func_80857A44(PlayState* play, Player* this) {
    if (LinkAnimation_Update(play, &this->skelAnime)) {
        func_8082E794(this);

        this->actor.shape.shadowDraw = ActorShadow_DrawCircle;
        this->actor.bgCheckFlags |= 0x800;
        this->unk_AE7 = 4;
        this->actor.shape.shadowScale = 30.0f;
        this->unk_AE8 = this->linearVelocity * 500.0f;
        this->unk_B08[0] = this->linearVelocity;
        this->unk_B08[1] = 0.0f;
        this->actor.home.rot.y = this->currentYaw;

        return true;
    }

    return false;
}

void func_80857AEC(PlayState* play, Player* this) {
    if (this->actor.bgCheckFlags & 2) {
        this->unk_B08[1] += this->unk_B08[0] * 0.05f;

        if (this->unk_B88 == 0) {
            if (this->unk_AE7 == 1) {
                this->unk_AE7 = 2;
                func_8083FE90(play, this, NA_SE_PL_GORON_PUNCH);
                play->actorCtx.unk2 = 4;
                EffectSsBlast_SpawnWhiteShockwave(play, &this->actor.world.pos, &gZeroVec3f, &gZeroVec3f);
                this->unk_AE8 = 0;
                this->unk_B08[0] = 0.0f;
                Actor_Spawn(&play->actorCtx, play, ACTOR_EN_TEST, this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z, 0, 0, 0, 0);
            } else {
                this->unk_AE7 = 4;
            }
        }

        func_8082E1BC(this);
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80857BE8.s")

extern void (*D_8085D990[0x14])(PlayState* play, Player* this, LinkAnimationHeader* anim);

void func_80858DB4(PlayState* play, Player* this, LinkAnimationHeader* anim);
void func_80858DDC(PlayState* play, Player* this, LinkAnimationHeader* anim);
void func_80858E40(PlayState* play, Player* this, LinkAnimationHeader* anim);
void func_80858E60(PlayState* play, Player* this, LinkAnimationHeader* anim);
void func_80858E80(PlayState* play, Player* this, LinkAnimationHeader* anim);
void func_80858EA0(PlayState* play, Player* this, LinkAnimationHeader* anim);
void func_80858EFC(PlayState* play, Player* this, LinkAnimationHeader* anim);
void func_80858F1C(PlayState* play, Player* this, LinkAnimationHeader* anim);
void func_80858F3C(PlayState* play, Player* this, LinkAnimationHeader* anim);
void func_80858F5C(PlayState* play, Player* this, LinkAnimationHeader* anim);
void func_80858FBC(PlayState* play, Player* this, LinkAnimationHeader* anim);
void func_80859028(PlayState* play, Player* this, LinkAnimationHeader* anim);
void func_808591BC(PlayState* play, Player* this, LinkAnimationHeader* anim);
void func_80858DFC(PlayState* play, Player* this, LinkAnimationHeader* anim);
void func_80858F7C(PlayState* play, Player* this, LinkAnimationHeader* anim);
void func_80858F9C(PlayState* play, Player* this, LinkAnimationHeader* anim);
void func_80859168(PlayState* play, Player* this, LinkAnimationHeader* anim);
void func_80859210(PlayState* play, Player* this, LinkAnimationHeader* anim);
void func_80858EC0(PlayState* play, Player* this, LinkAnimationHeader* anim);

void func_80858C84(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    func_8082E784(this);
    func_8082E438(play, this, anim);
    func_8082DABC(this);
}

void func_80858CC8(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    func_8082E784(this);
    LinkAnimation_Change(play, &this->skelAnime, anim, 2.0f / 3.0f, 0.0f, Animation_GetLastFrame(anim), 2, -8.0f);
    func_8082DABC(this);
}

void func_80858D48(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    func_8082E784(this);
    LinkAnimation_Change(play, &this->skelAnime, anim, 2.0f / 3.0f, 0.0f, 0.0f, 0, -8.0f);
    func_8082DABC(this);
}

void func_80858DB4(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    func_8082DABC(this);
}

void func_80858DDC(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    func_80858C84(play, this, anim);
}

void func_80858DFC(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    func_8082E784(this);
    func_8082E5A8(play, this, anim);
    func_8082DABC(this);
}

void func_80858E40(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    func_80858CC8(play, this, anim);
}

void func_80858E60(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    func_80858D48(play, this, anim);
}

void func_80858E80(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    func_8082EA60(play, this, anim);
}

void func_80858EA0(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    func_8082EA10(play, this, anim, 0x9C);
}

void func_80858EC0(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    func_8082DBC0(play, this, anim);
    func_8082E920(play, this, 0x9C);
}

void func_80858EFC(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    func_8082EB18(play, this, anim);
}

void func_80858F1C(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    func_8082EAC8(play, this, anim, 0x9C);
}

void func_80858F3C(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    func_8082DB18(play, this, anim);
}

void func_80858F5C(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    func_8082DB3C(play, this, anim);
}

void func_80858F7C(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    func_8082DB90(play, this, anim);
}

void func_80858F9C(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    func_8082DB60(play, this, anim);
}

void func_80858FBC(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    LinkAnimation_Update(play, &this->skelAnime);
}

void func_80858FE8(Player* this) {
    if (this->skelAnime.animation == &gameplay_keep_Linkanim_00E120) {
        func_8082E094(this, 0xD0U);
        func_800B8E58(this, NA_SE_VO_LK_DRAGGED_DAMAGE - SFX_FLAG);
    }
}

extern struct_8082E224_arg1 D_8085DA08;
extern struct_8082E224_arg1 D_8085DA14;
extern struct_8082E224_arg1 D_8085DA38;
extern struct_8082E224_arg1 D_8085DA48;
extern struct_8082E224_arg1 D_8085DA7C;
extern struct_8082E224_arg1 D_8085DA84;
extern struct_8082E224_arg1 D_8085DA88;
extern struct_8082E224_arg1 D_8085DA8C;
extern struct_8082E224_arg1 D_8085DA90;

void func_80859028(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    void* temp_v0;

    if (LinkAnimation_Update(play, &this->skelAnime) != 0) {
        func_80858D48(play, this, anim);
        this->unk_AE8 = 1;
    }
    if (this->skelAnime.animation == &gameplay_keep_Linkanim_00E150) {
        func_8082E224(this, &D_8085DA08);
        return;
    }
    if (this->skelAnime.animation == &gameplay_keep_Linkanim_00E118) {
        func_8082E224(this, &D_8085DA14);
        return;
    }
    if (this->skelAnime.animation == &gameplay_keep_Linkanim_00E430) {
        func_8082E224(this, &D_8085DA38);
        return;
    }
    if (this->skelAnime.animation == &gameplay_keep_Linkanim_00D030) {
        func_8082E224(this, &D_8085DA7C);
        return;
    }
    if (this->skelAnime.animation == &gameplay_keep_Linkanim_00CF48) {
        func_8082E224(this, &D_8085DA84);
        return;
    }
    if (this->skelAnime.animation == &gameplay_keep_Linkanim_00D108) {
        func_8082E224(this, &D_8085DA90);
        return;
    }
    func_80858FE8(this);
}

void func_80859168(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    if (LinkAnimation_Update(play, &this->skelAnime) != 0) {
        func_8082E794(this);
        func_8082DB60(play, this, anim);
    }
}

void func_808591BC(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    if (LinkAnimation_Update(play, &this->skelAnime) != 0) {
        func_8082EB18(play, this, anim);
        this->unk_AE8 = 1;
    }
}

void func_80859210(PlayState* play, Player* this, LinkAnimationHeader* anim) {
    LinkAnimation_Update(play, &this->skelAnime);
    // TODO: check wtf is going on here
    func_8082E224(this, (struct_8082E224_arg1* ) anim);
}

void func_80859248(Player* this) {
    if ((this->unk_398 == NULL) || (this->unk_398->update == NULL)) {
        this->unk_398 = NULL;
    }
    this->unk_730 = this->unk_398;
    if (this->unk_398 != NULL) {
        this->actor.shape.rot.y = func_8083C62C(this, 0);
    }
}

void func_8085929C(PlayState* play, Player* this, UNK_TYPE arg2) {
    this->stateFlags1 |= PLAYER_STATE1_8000000;
    this->stateFlags2 |= PLAYER_STATE2_400;
    this->stateFlags1 &= ~(PLAYER_STATE1_40000 | PLAYER_STATE1_80000);
    func_8082DB3C(play, this, &gameplay_keep_Linkanim_00DFD0);
    this->linearVelocity = 0.0f;
}


void func_80859300(PlayState* play, Player* this, UNK_TYPE arg2) {
    this->actor.gravity = 0.0f;

    if (this->unk_AE7 == 0) {
        if ((this->transformation == PLAYER_FORM_DEKU) || func_8083B3B4(play, this, NULL)) {
            this->unk_AE7 = 1;
        } else {
            func_808477D0(play, this, NULL, fabsf(this->actor.velocity.y));
            Math_ScaledStepToS(&this->unk_AAA, -0x2710, 0x320);
            func_8084748C(this, &this->actor.velocity.y, 4.0f, this->currentYaw);
        }
    } else {
        if (LinkAnimation_Update(play, &this->skelAnime)) {
            if (this->unk_AE7 == 1) {
                func_8082E634(play, this, &D_0400E000);
            } else {
                func_8082DB3C(play, this, &D_0400E000);
            }
        }
        func_808475B4(this);
        func_8084748C(this, &this->linearVelocity, 0.0f, this->actor.shape.rot.y);
    }
}

void func_80859414(PlayState* play, Player* this, UNK_TYPE arg2) {
    func_80859248(this);

    if (func_801242B4(this)) {
        func_80859300(play, this, 0);
    } else {
        LinkAnimation_Update(play, &this->skelAnime);
        if (func_801240DC(this) || (this->stateFlags1 & PLAYER_STATE1_800)) {
            func_8083216C(this, play);
        } else if ((this->interactRangeActor != NULL) && (this->interactRangeActor->textId == 0xFFFF)) {
            func_8083D23C(this, play);
        }
    }
}

void func_808594D0(PlayState* play, Player* this, UNK_TYPE arg2) {
    if (this->stateFlags1 & PLAYER_STATE1_8000000) {
        func_8085929C(play, this, 0);
    } else {
        LinkAnimationHeader* anim = GET_PLAYER_ANIM(PLAYER_ANIMGROUP_43, this->modelAnimType);

        if ((this->unk_396 == 6) || (this->unk_396 == 0x2E)) {
            func_8082DB18(play, this, anim);
        } else {
            func_8082E784(this);
            LinkAnimation_Change(play, &this->skelAnime, anim, 2.0f / 3.0f, 0.0f, Animation_GetLastFrame(anim), 0, -4.0f);
        }
        func_8082DABC(this);
    }
}

void func_808595B8(PlayState* play, Player* this, UNK_TYPE arg2) {
    if (func_80847880(play, this)) {
        return;
    }

    if ((this->csMode == 0x14) && (play->csCtx.state == 0)) {
        func_800B7298(play, NULL, 6);
    } else if (this->stateFlags1 & PLAYER_STATE1_8000000) {
        func_80859300(play, this, 0);
        this->actor.velocity.y = 0.0f;
    } else {
        LinkAnimation_Update(play, &this->skelAnime);
        if (func_801240DC(this) || (this->stateFlags1 & PLAYER_STATE1_800)) {
            func_8083216C(this, play);
        }
    }
}

void func_8085968C(PlayState* play, Player* this, UNK_TYPE arg2) {
    if (this->actor.id == ACTOR_EN_TEST3) {
        func_80838830(this, OBJECT_GI_MSSA);
        this->stateFlags1 |= PLAYER_STATE1_400;
    }

    func_8082DB90(play, this, (this->transformation == PLAYER_FORM_DEKU) ? &D_0400E2B8 : &D_0400D5A8);
}

void func_80859708(PlayState* play, Player* this, UNK_TYPE arg2) {
    LinkAnimation_Update(play, &this->skelAnime);
    if ((this->actor.id == ACTOR_EN_TEST3) && Animation_OnFrame(&this->skelAnime, 20.0f)) {
        this->unk_B2A = 0x3A;
        func_80151BB4(play, 0x1B);
        func_801A3098(0x37);
    }
}

// unused
void func_8085978C(PlayState* play, Player* this, UNK_TYPE arg2) {
    f32 temp_fv0;
    s16 temp_v0;

    this->stateFlags1 &= 0xFDFFFFFF;
    temp_v0 = Math_Vec3f_Yaw(&this->actor.world.pos, &this->unk_3A0);
    temp_fv0 = this->linearVelocity;
    this->actor.world.rot.y = temp_v0;
    this->actor.shape.rot.y = temp_v0;
    this->currentYaw = temp_v0;
    if (temp_fv0 <= 0.0f) {
        this->linearVelocity = 0.1f;
    } else if (temp_fv0 > 2.5f) {
        this->linearVelocity = 2.5f;
    }
    if ((this->transformation != 4) && (play->roomCtx.currRoom.unk3 == 5)) {
        gGameInfo->data[0x220] = 0x2D;
        gGameInfo->data[0x221] = 0xFF;
        gGameInfo->data[0x222] = 0xFF;
        gGameInfo->data[0x223] = 0xFF;
        gGameInfo->data[0x224] = 0;
        play_sound(0x4834U);
    }
}

// unused
void func_80859890(PlayState* play, Player* this, UNK_TYPE arg2) {
    f32 sp24;
    s16 temp_v1 = gGameInfo->data[0x220];

    if (temp_v1 > 0) {
        gGameInfo->data[0x224] += temp_v1;
        if (gGameInfo->data[0x224] >= 0x100) {
            gGameInfo->data[0x220] = -0x40;
            gGameInfo->data[0x224] = 0xFF;
            gSaveContext.save.playerForm = 4;
            this->actor.update = func_8012301C;
            this->actor.draw = NULL;
            this->unk_AE7 = 0;
        }
    } else if (temp_v1 < 0) {
        gGameInfo->data[0x224] += temp_v1;
        if (gGameInfo->data[0x224] < 0) {
            gGameInfo->data[0x220] = 0;
            gGameInfo->data[0x224] = 0;
        }
    } else {
        sp24 = 2.5f;
        func_808411D4(play, this, &sp24, 0xA);
        this->unk_AE8++;
        if (this->unk_AE8 >= 0x15) {
            this->csMode = 0xA;
        }
    }
}

// unused
void func_80859990(PlayState* play, Player* this, UNK_TYPE arg2) {
    this->linearVelocity = 2.5f;
    func_80835BF8(&this->actor.world.pos, this->actor.shape.rot.y, 180.0f, &this->unk_3A0);
}

void func_808599DC(PlayState* play, Player* this, UNK_TYPE arg2) {
    f32 sp1C;

    sp1C = 2.5f;
    func_808411D4(play, this, &sp1C, 0xA);
}

void func_80859A10(PlayState* play, Player* this, UNK_TYPE arg2) {
    func_8083B23C(this, play);
}

void func_80859A44(PlayState* play, Player* this, UNK_TYPE arg2) {
    func_80859248(this);
    if (this->unk_AE8 != 0) {
        if (LinkAnimation_Update(play, &this->skelAnime)) {
            func_8082DB3C(play, this, func_8082EF54(this));
            this->unk_AE8 = 0;
        }
        func_8082FC60(this);
    } else {
        func_8083E958(play, this);
    }
}

void func_80859AD0(PlayState* play, Player* this, UNK_PTR arg2) {
    func_80840F90(play, this, arg2, 0.0f, 0, 0);
}

void func_80859AF8(PlayState* play, Player* this, UNK_PTR arg2) {
    this->actor.shape.face = 0xF;
    func_80840F90(play, this, arg2, 0.0f, 0, 0);
}

void func_80859B28(PlayState* play, Player* this, UNK_PTR arg2) {
    func_80840F90(play, this, arg2, 0.0f, 0, 1);
}

void func_80859B54(PlayState* play, Player* this, UNK_TYPE arg2) {
    func_80858CC8(play, this, &gameplay_keep_Linkanim_00DDB0);
    this->heldItemActionParam = PLAYER_AP_OCARINA;
    Player_SetModels(this, Player_ActionToModelGroup(this, this->heldItemActionParam));
}

/*
void func_80859BA8(PlayState* arg0, Player* arg1, void* arg2) {
    f32 sp4;
    f32 temp_fa0;
    f32 temp_ft1;
    f32 temp_fv0;
    f32 temp_fv1;
    u16 temp_v0;

    temp_fv0 = (f32) arg2->unk_C;
    temp_v0 = arg2->unk_2;
    temp_fv1 = (f32) arg2->unk_10;
    temp_fa0 = (f32) arg2->unk_14;
    temp_ft1 = (f32) (arg0->csCtx.frames - temp_v0) / (f32) (arg2->unk_4 - temp_v0);
    sp4 = temp_ft1;
    arg1->actor.world.pos.x = (((f32) arg2->unk_18 - temp_fv0) * temp_ft1) + temp_fv0;
    arg1->actor.world.pos.y = (((f32) arg2->unk_1C - temp_fv1) * sp4) + temp_fv1;
    arg1->actor.world.pos.z = (((f32) arg2->unk_20 - temp_fa0) * sp4) + temp_fa0;
}
*/
void func_80859BA8(PlayState* play, Player* this, UNK_PTR arg2);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_80859BA8.s")

void func_80859C60(PlayState* play, Player* this, UNK_PTR arg2) {
    if (arg2 != NULL) {
        func_80859BA8(play, this, arg2);
    }

    LinkAnimation_Update(play, &this->skelAnime);
}

void func_80859CA0(PlayState* play, Player* this, s32 arg2) {
    func_8082E514(play, this, GET_PLAYER_ANIM(PLAYER_ANIMGROUP_43, this->modelAnimType));
    func_8082DABC(this);
}

void func_80859CE0(PlayState* play, Player* this, s32 arg2) {
    this->actor.draw = Player_Draw;
}

void func_80859CFC(PlayState* play, Player* this, UNK_TYPE arg2) {
    func_80841358(play, this, 0);
    func_8082DB90(play, this, &D_0400D660);
}

void func_80859D44(PlayState* play, Player *this, UNK_TYPE arg2) {
    LinkAnimation_Update(play, &this->skelAnime);
}


typedef struct struct_8085E368 {
    /* 0x0 */ s16 unk_0;                            /* inferred */
    /* 0x2 */ s16 unk_2;                            /* inferred */
    /* 0x4 */ s16 unk_4;                            /* inferred */
    /* 0x6 */ s16 unk_6;                            /* inferred */
    /* 0x8 */ s16 unk_8;                            /* inferred */
    /* 0xA */ s16 unk_A;                            /* inferred */
} struct_8085E368; // size = 0xC


extern LinkAnimationHeader* D_8085E354[PLAYER_FORM_MAX];
extern struct_8085E368 D_8085E368[];

extern Color_RGBA8 D_8085E3A4;
extern Color_RGBA8 D_8085E3A8;

void func_80859D70(PlayState* play, Player* this, UNK_TYPE arg2) {
    struct_8085E368* temp_s0;
    Vec3f pos;
    Vec3f sp34;

    func_808591BC(play, this, D_8085E354[this->transformation]);

    if (this->rightHandType != 0xFF) {
        this->rightHandType = 0xFF;
    } else {
        temp_s0 = &D_8085E368[this->transformation];
        sp34.x = randPlusMinusPoint5Scaled(temp_s0->unk_6) + temp_s0->unk_0;
        sp34.y = randPlusMinusPoint5Scaled(temp_s0->unk_8) + temp_s0->unk_2;
        sp34.z = randPlusMinusPoint5Scaled(temp_s0->unk_A) + temp_s0->unk_4;
        SkinMatrix_Vec3fMtxFMultXYZ(&this->shieldMf, &sp34, &pos);
        EffectSsKirakira_SpawnDispersed(play, &pos, &gZeroVec3f, &gZeroVec3f, &D_8085E3A4, &D_8085E3A8, 600, -10);
    }
}

void func_80859EBC(PlayState* play, Player* this, s32 arg2) {
    if (LinkAnimation_Update(play, &this->skelAnime)) {
        func_8085AACC(play, this, arg2);
    } else if (this->unk_AE8 == 0) {
        Item_Give(play, ITEM_SWORD_RAZOR);
        func_80841358(play, this, 0);
    } else {
        func_808484CC(this);
    }
}

void func_80859F4C(PlayState* play, Player* this, UNK_TYPE arg2);
void func_80859FCC(PlayState* play, Player* this, UNK_TYPE arg2);
void func_80859FF4(PlayState* play, Player* this, UNK_TYPE arg2);
void func_8085A120(PlayState* play, Player* this, UNK_TYPE arg2);
void func_8085A144(PlayState* play, Player* this, UNK_TYPE arg2);
void func_8085A19C(PlayState* play, Player* this, UNK_TYPE arg2);
void func_8085A1D4(PlayState* play, Player* this, UNK_TYPE arg2);
void func_8085A24C(PlayState* play, Player* this, UNK_TYPE arg2);
void func_8085A2AC(PlayState* play, Player* this, UNK_TYPE arg2);
void func_8085A330(PlayState* play, Player* this, UNK_TYPE arg2);

void func_80859F4C(PlayState* play, Player* this, UNK_TYPE arg2) {
    if (LinkAnimation_Update(play, &this->skelAnime) != 0) {
        func_8083FCF0(play, this, 0.0f, 99.0f, this->skelAnime.endFrame - 8.0f);
    }
    if (this->itemActionParam != 5) {
        func_80841358(play, this, 1);
    }
}

void func_80859FCC(PlayState* play, Player* this, UNK_TYPE arg2) {
    if (this->transformation != 3) {
        gSaveContext.save.playerForm = 3;
    }
}

void func_80859FF4(PlayState* play, Player* this, UNK_TYPE arg2) {
    LinkAnimation_Update(play, &this->skelAnime);
    if (gSaveContext.save.playerForm != this->transformation) {
        this->actor.update = func_8012301C;
        this->actor.draw = NULL;
    }
}

void func_8085A04C(PlayState* play, Player* this, UNK_PTR arg2);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8085A04C.s")

void func_8085A120(PlayState* play, Player* this, UNK_TYPE arg2) {
    if (this->transformation != 0) {
        gSaveContext.save.playerForm = 0;
    }
}

void func_8085A144(PlayState* play, Player* this, UNK_TYPE arg2) {
    LinkAnimation_Update(play, &this->skelAnime);
    if (gSaveContext.save.playerForm != this->transformation) {
        this->actor.update = func_8012301C;
        this->actor.draw = NULL;
    }
}

void func_8085A19C(PlayState* play, Player* this, UNK_TYPE arg2) {
    func_8082DB18(play, this, &gameplay_keep_Linkanim_00D2D0);
    this->unk_AAA = -0x8000;
}

void func_8085A1D4(PlayState* play, Player* this, UNK_TYPE arg2) {
    LinkAnimation_Update(play, &this->skelAnime);
    this->actor.gravity = 0.0f;
    Math_StepToF(&this->actor.velocity.y, -this->actor.terminalVelocity, -((f32) gGameInfo->data[0x44] / 100.0f));
}

void func_8085A24C(PlayState* play, Player* this, UNK_TYPE arg2) {
    func_8082DB90(play, this, D_8085D17C[this->transformation]);
    this->heldItemActionParam = 0x14;
    Player_SetModels(this, Player_ActionToModelGroup(this, (enum PlayerActionParam) this->heldItemActionParam));
}

void func_8085A2AC(PlayState* play, Player* this, UNK_TYPE arg2) {
    if ((LinkAnimation_Update(play, &this->skelAnime) != 0) && (this->skelAnime.animation == D_8085D17C[this->transformation])) {
        func_808525C4(play, this);
        return;
    }
    if (this->unk_AE8 != 0) {
        func_8085255C(play, this);
    }
}

void func_8085A330(PlayState* play, Player* this, UNK_TYPE arg2) {
    func_8082DBC0(play, this, D_8085D17C[this->transformation]);
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8085A364.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8085A40C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8085A4A4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8085A530.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8085A5DC.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8085A66C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8085A6C0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8085A710.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8085A768.s")

void func_8085A7C0(PlayState* play, Player* this, UNK_TYPE arg2) {
    if (LinkAnimation_Update(play, &this->skelAnime)) {
        if (this->unk_AE8 == 0) {
            if ((Message_GetState(&play->msgCtx) == TEXT_STATE_CLOSING) || (Message_GetState(&play->msgCtx) == TEXT_STATE_NONE)) {
                this->unk_B2A = 0;
                this->unk_AE8 = -1;
            } else {
                this->unk_B2A = 0x6F;
            }
        } else if (this->unk_AE8 < 0) {
            if (Actor_HasParent(&this->actor, play)) {
                this->actor.parent = NULL;
                this->unk_AE8 = 1;
            } else {
                Actor_PickUp(&this->actor, play, GI_PENDANT_OF_MEMORIES, 9999.9f, 9999.9f);
            }
        }
    } else if (LinkAnimation_OnFrame(&this->skelAnime, 4.0f)) {
        gSaveContext.save.weekEventReg[50] |= 0x80;
    }
}

void func_8085A8C4(PlayState* play, Player* this, UNK_TYPE arg2) {
    if ((this->transformation != PLAYER_FORM_HUMAN) && (play->roomCtx.currRoom.unk3 == 5)) {
        gGameInfo->data[0x220] = 0x2D;
        gGameInfo->data[0x221] = 0xFF;
        gGameInfo->data[0x222] = 0xFF;
        gGameInfo->data[0x223] = 0xFF;
        gGameInfo->data[0x224] = 0;
        play_sound(NA_SE_SY_WHITE_OUT_T);
    }
}

void func_8085A940(PlayState* play, Player* this, UNK_TYPE arg2) {
    s16 temp_v1 = gGameInfo->data[0x220];

    if (temp_v1 > 0) {
        gGameInfo->data[0x224] += temp_v1;
        if (gGameInfo->data[0x224] >= 0x100) {
            gGameInfo->data[0x220] = -0x40;
            gGameInfo->data[0x224] = 0xFF;
            gSaveContext.save.playerForm = 4;
            this->actor.update = func_8012301C;
            this->actor.draw = NULL;
            this->unk_AE7 = 0;
        }
    } else if (temp_v1 < 0) {
        gGameInfo->data[0x224] += temp_v1;
        if (gGameInfo->data[0x224] < 0) {
            gGameInfo->data[0x220] = 0;
            gGameInfo->data[0x224] = 0;
        }
    } else {
        LinkAnimation_Update(play, &this->skelAnime);
    }
}

void func_8085AA10(PlayState* play, Player* this, UNK_TYPE arg2) {
    if (LinkAnimation_Update(play, &this->skelAnime) && (ActorCutscene_GetCurrentIndex() == 0x7D)) {
        ActorCutscene_Stop(0x7D);
    }
}

void func_8085AA60(PlayState* play, Player* this, UNK_TYPE arg2) {
    func_80848640(play, this);
}

void func_8085AA84(PlayState* play, Player* this, UNK_TYPE arg2) {
    if (LinkAnimation_Update(play, &this->skelAnime)) {
        this->stateFlags2 |= PLAYER_STATE2_1000000;
    }
}

void func_8085AACC(PlayState* play, Player* this, UNK_TYPE arg2) {
    if (func_801242B4(this)) {
        func_808353DC(play, this);
        func_8082DC64(play, this);
    } else {
        func_80839ED0(this, play);
        if (!func_808391D8(this, play)) {
            func_8083D23C(this, play);
        }
    }

    this->csMode = 0;
    this->unk_AA5 = 0;
}

void func_8085AB58(Player* this, CsCmdActorAction* playerAction) {
    this->actor.world.pos.x = playerAction->startPos.x;
    this->actor.world.pos.y = playerAction->startPos.y;
    this->actor.world.pos.z = playerAction->startPos.z;

    this->currentYaw = this->actor.shape.rot.y = playerAction->rot.y;
}

void func_8085ABA8(Player* this, CsCmdActorAction* playerAction) {
    f32 xDiff = playerAction->startPos.x - (s32) this->actor.world.pos.x;
    f32 yDiff = playerAction->startPos.y - (s32) this->actor.world.pos.y;
    f32 zDiff = playerAction->startPos.z - (s32) this->actor.world.pos.z;
    f32 dist;
    s16 temp_v0;

    temp_v0 = playerAction->rot.y - this->actor.shape.rot.y;
    dist = sqrtf(SQ(xDiff) + SQ(yDiff) + SQ(zDiff));
    if (this->linearVelocity == 0.0f) {
        if ((dist > 50.0f) || (ABS_ALT(temp_v0) > 0x4000)) {
            func_8085AB58(this, playerAction);
        }
    }

    this->skelAnime.moveFlags = 0;
    func_8082E784(this);
}

typedef struct struct_8085DA94 {
    /* 0x0 */ UNK_TYPE1 unk_0[0x8];
} struct_8085DA94; // size = 0x8

extern struct_8085DA94 D_8085DA94[0x8C];
extern struct_8085DA94 D_8085DEF4[0x8C];

void func_8085AC9C(PlayState* play, Player* this, UNK_PTR arg2, struct_8085DA94* arg3);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8085AC9C.s")

void func_8085AD5C(PlayState* play, Player* this, s32 arg2) {
    if ((arg2 != 1) && (arg2 != 7) && (arg2 != 0x14) && (arg2 != 6)) {
        func_8082DCA0(play, this);
    }
}

void func_8085ADA0(PlayState* play, Player* this, UNK_TYPE arg2);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8085ADA0.s")


void func_8085B08C(Player* this, PlayState* play) {
    if (this->csMode != this->unk_395) {
        D_80862B6C = this->skelAnime.moveFlags;
        func_8082E794(this);

        this->unk_395 = this->csMode;
        func_8085AD5C(play, this, this->csMode);
        func_8085AC9C(play, this, NULL, &D_8085DA94[this->csMode]);
    }

    func_8085AC9C(play, this, NULL, &D_8085DEF4[this->csMode]);
}

s32 func_8085B134(PlayState* play) {
    Player* player = GET_PLAYER(play);

    func_8082DE50(play, player);
    func_80831990(play, player, ITEM_FISHING_POLE);
    return 1;
}

void func_8085B170(PlayState* play, Player* player) {
    func_8082DE50(play, player);
    func_80831494(play, player, func_80853CC0, 0);
    func_8082DB18(play, player, &D_0400DE28);
    player->stateFlags2 |= PLAYER_STATE2_80;
    func_8082DAD4(player);
    func_8082DF8C(player, NA_SE_VO_LI_HELD);
}

s32 func_8085B1F0(PlayState* play, Player* player) {
    if (!Player_InBlockingCsMode(play, player) && (player->invincibilityTimer >= 0) && !func_801240DC(player)) {
        if (!(player->stateFlags1 & (PLAYER_STATE1_80 | PLAYER_STATE1_2000 | PLAYER_STATE1_4000 | PLAYER_STATE1_100000 | PLAYER_STATE1_200000 | PLAYER_STATE1_800000))) {
            if (!(player->stateFlags2 & PLAYER_STATE2_80) && !(player->stateFlags3 & PLAYER_STATE3_80)) {
                func_8085B170(play, player);
                return true;
            }
        }
    }

    return false;
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8085B28C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8085B384.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8085B3E0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_player_actor/func_8085B460.s")

void func_8085B74C(PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 temp_fv1;
    f32 linearVelocity;

    linearVelocity = player->linearVelocity;
    if (linearVelocity < 0.0f) {
        linearVelocity = -linearVelocity;
        player->actor.world.rot.y += 0x8000;
    }

    temp_fv1 = gGameInfo->data[0x2D] / 100.0f;

    if (temp_fv1 < linearVelocity) {
        gSaveContext.entranceSpeed = temp_fv1;
    } else {
        gSaveContext.entranceSpeed = linearVelocity;
    }

    func_80835324(play, player, 400.0f, (D_80862B10 != 0) ? D_80862B16 : player->actor.world.rot.y);
    player->stateFlags1 |= (PLAYER_STATE1_1 | PLAYER_STATE1_20000000);
}

void func_8085B820(PlayState* play, s16 arg1) {
    Player* player = GET_PLAYER(play);

    player->actor.focus.rot.y = arg1;
    func_80836D8C(player);
}

PlayerActionParam func_8085B854(PlayState* play, Player* player, ItemID itemId) {
    PlayerActionParam actionParam = func_8082F524(player, itemId);

    if ((actionParam >= PLAYER_AP_MASK_TRUTH) && (actionParam <= PLAYER_AP_MASK_DEKU) && (actionParam == (player->currentMask + PLAYER_AP_MASK_TRUTH - 1))) {
        actionParam = PLAYER_AP_NONE;
    }

    if ((actionParam <= PLAYER_AP_NONE) || (actionParam >= PLAYER_AP_MAX)) {
        return -1;
    }

    player->heldItemActionParam = PLAYER_AP_NONE;
    player->unk_748 = NULL;
    func_80831760(play, player, func_80853A5C, 0);
    player->unk_A86 = 0x7C;
    player->heldItemActionParam = actionParam;
    func_8082DB18(play, player, &gameplay_keep_Linkanim_00DCA8);
    player->stateFlags1 |= (PLAYER_STATE1_40 | PLAYER_STATE1_20000000);
    player->unk_B2A = 0;
    player->exchangeItemId = actionParam;

    return actionParam;
}

s32 func_8085B930(PlayState* play, LinkAnimationHeader* talkAnim, s32 animMode) {
    Player* player = GET_PLAYER(play);

    if (!(player->actor.flags & ACTOR_FLAG_100)) {
        return false;
    }

    LinkAnimation_Change(play, &player->skelAnime, (talkAnim == NULL) ? func_8082ED20(player) : talkAnim, 2.0f / 3.0f, 0.0f, Animation_GetLastFrame(talkAnim), animMode, -6.0f);
    return true;
}
