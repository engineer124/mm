/*
 * File: z_elf_msg.c
 * Overlay: ovl_Elf_Msg
 * Description: Readable Tatl call spot
 */

#include "z_elf_msg.h"
#include "overlays/actors/ovl_En_Elf/z_en_elf.h"

#define FLAGS (ACTOR_FLAG_10)

#define THIS ((ElfMsg*)thisx)

void ElfMsg_Init(Actor* thisx, PlayState* play);
void ElfMsg_Destroy(Actor* thisx, PlayState* play);
void ElfMsg_Update(Actor* thisx, PlayState* play);

void ElfMsg_Action(ElfMsg* this, PlayState* play);

ActorInit Elf_Msg_InitVars = {
    ACTOR_ELF_MSG,
    ACTORCAT_ITEMACTION,
    FLAGS,
    GAMEPLAY_KEEP,
    sizeof(ElfMsg),
    (ActorFunc)ElfMsg_Init,
    (ActorFunc)ElfMsg_Destroy,
    (ActorFunc)ElfMsg_Update,
    (ActorFunc)NULL,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1000, ICHAIN_STOP),
};

void ElfMsg_SetupAction(ElfMsg* this, ElfMsgActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

/**
 * Checks a scene flag - if flag is set, the actor is killed and function returns 1. Otherwise returns 0.
 * Can also set a switch flag from params while killing.
 */
s32 ElfMsg_KillCheck(ElfMsg* this, PlayState* play) {
    if ((this->actor.home.rot.y > 0) && (this->actor.home.rot.y <= 0x80) &&
        Flags_GetSwitch(play, this->actor.home.rot.y - 1)) {
        (void)"共倒れ"; // "Mutual destruction"
        if (ELFMSG_GET_SWITCH_FLAG(&this->actor) != 0x7F) {
            Flags_SetSwitch(play, ELFMSG_GET_SWITCH_FLAG(&this->actor));
        }
        Actor_Kill(&this->actor);
        return true;
    }

    if (this->actor.home.rot.y == 0x81) {
        if (Flags_GetClear(play, this->actor.room)) {
            if (ELFMSG_GET_SWITCH_FLAG(&this->actor) != 0x7F) {
                Flags_SetSwitch(play, ELFMSG_GET_SWITCH_FLAG(&this->actor));
            }
            Actor_Kill(&this->actor);
            return true;
        }
    }

    if (ELFMSG_GET_SWITCH_FLAG(&this->actor) == 0x7F) {
        return false;
    }

    if (Flags_GetSwitch(play, ELFMSG_GET_SWITCH_FLAG(&this->actor))) {
        (void)"共倒れ"; // "Mutual destruction"
        Actor_Kill(&this->actor);
        return true;
    }

    return false;
}

void ElfMsg_Init(Actor* thisx, PlayState* play) {
    ElfMsg* this = THIS;

    if (!ElfMsg_KillCheck(this, play)) {
        Actor_ProcessInitChain(&this->actor, sInitChain);
        if (ELFMSG_GET_XZ_RANGE(thisx) == 0) {
            thisx->scale.z = 0.4f;
            thisx->scale.x = 0.4f;
        } else {
            this->actor.scale.x = this->actor.scale.z = ELFMSG_GET_XZ_RANGE(thisx) * 0.04f;
        }
        if (this->actor.home.rot.z == 0) {
            this->actor.scale.y = 0.4f;
        } else {
            this->actor.scale.y = this->actor.home.rot.z * 0.04f;
        }
        ElfMsg_SetupAction(this, ElfMsg_Action);
        this->actor.shape.rot.x = this->actor.shape.rot.y = this->actor.shape.rot.z = 0;
    }
}

void ElfMsg_Destroy(Actor* thisx, PlayState* play) {
}

s32 ElfMsg_GetTextId(ElfMsg* this) {
    if (ELFMSG_GET_8000(&this->actor)) {
        return 0x200 + ELFMSG_GET_TEXT_ID_OFFSET(&this->actor);
    } else {
        return -0x200 - ELFMSG_GET_TEXT_ID_OFFSET(&this->actor);
    }
}

s32 ElfMsg_IsPlayerInRange(ElfMsg* this) {
    return (this->actor.xzDistToPlayer < (100.0f * this->actor.scale.x)) && (this->actor.playerHeightRel >= 0.0f) &&
           (this->actor.playerHeightRel < (100.0f * this->actor.scale.y));
}

void ElfMsg_Action(ElfMsg* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    EnElf* tatl = (EnElf*)player->tatlActor;

    if ((player->tatlActor != NULL) && ElfMsg_IsPlayerInRange(this)) {
        player->tatlTextId = ElfMsg_GetTextId(this);
        CutsceneManager_Queue(CS_ID_GLOBAL_TALK);
        tatl->tatlHintActor = &this->actor;
        if (this->actor.csId == CS_ID_NONE) {
            this->actor.csId = CS_ID_GLOBAL_TALK;
        }
        if ((player->tatlTextId < 0) && (this->actor.home.rot.x < 0)) {
            if (CutsceneManager_GetCurrentCsId() == CS_ID_GLOBAL_DOOR) {
                CutsceneManager_Stop(CS_ID_GLOBAL_DOOR);
                CutsceneManager_Queue(this->actor.csId);
            } else if (CutsceneManager_IsNext(this->actor.csId)) {
                this->actor.home.rot.x = 0;
                CutsceneManager_Start(this->actor.csId, &this->actor);
                func_800E0348(play->cameraPtrs[CAM_ID_MAIN]);
            } else {
                CutsceneManager_Queue(this->actor.csId);
            }
        }
    }
}

void ElfMsg_Update(Actor* thisx, PlayState* play) {
    ElfMsg* this = THIS;

    if (!ElfMsg_KillCheck(this, play)) {
        if (Actor_AcceptTalkRequest(&this->actor, &play->state)) {
            if (ELFMSG_GET_SWITCH_FLAG(thisx) != 0x7F) {
                Flags_SetSwitch(play, ELFMSG_GET_SWITCH_FLAG(thisx));
            }
            Actor_Kill(&this->actor);
            return;
        }

        if ((this->actor.home.rot.y >= 0) || (this->actor.home.rot.y < -0x80) ||
            (Flags_GetSwitch(play, -1 - this->actor.home.rot.y))) {
            this->actionFunc(this, play);
        }
    }
}
