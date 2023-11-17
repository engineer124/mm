/*
 * File: z_elf_msg6.c
 * Overlay: ovl_Elf_Msg6
 * Description: Tatl Hint (another proximity-activated C-up hint?)
 */

#include "z_elf_msg6.h"
#include "overlays/actors/ovl_En_Elf/z_en_elf.h"

#define FLAGS (ACTOR_FLAG_10)

#define THIS ((ElfMsg6*)thisx)

void ElfMsg6_Init(Actor* thisx, PlayState* play);
void ElfMsg6_Destroy(Actor* thisx, PlayState* play);
void ElfMsg6_Update(Actor* thisx, PlayState* play);

void func_80BA1CF8(ElfMsg6* this, PlayState* play);
void ElfMsg6_NextRegionHint(ElfMsg6* this, PlayState* play);
void func_80BA1F80(ElfMsg6* this, PlayState* play);
void ElfMsg6_DoNothing(ElfMsg6* this, PlayState* play);
void func_80BA2048(ElfMsg6* this, PlayState* play);
void func_80BA215C(ElfMsg6* this, PlayState* play);
void func_80BA21C4(ElfMsg6* this, PlayState* play);

ActorInit Elf_Msg6_InitVars = {
    /**/ ACTOR_ELF_MSG6,
    /**/ ACTORCAT_ITEMACTION,
    /**/ FLAGS,
    /**/ GAMEPLAY_KEEP,
    /**/ sizeof(ElfMsg6),
    /**/ ElfMsg6_Init,
    /**/ ElfMsg6_Destroy,
    /**/ ElfMsg6_Update,
    /**/ NULL,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1000, ICHAIN_STOP),
};

s32 func_80BA15A0(void) {
    if (CHECK_QUEST_ITEM(QUEST_REMAINS_ODOLWA) && !CHECK_WEEKEVENTREG(WEEKEVENTREG_TATL_HINT_COMPLETED_ODOLWA)) {
        return true;
    }

    if (CHECK_QUEST_ITEM(QUEST_REMAINS_GOHT) && !CHECK_WEEKEVENTREG(WEEKEVENTREG_TATL_HINT_COMPLETED_GOHT)) {
        return true;
    }

    if (CHECK_QUEST_ITEM(QUEST_REMAINS_GYORG) && !CHECK_WEEKEVENTREG(WEEKEVENTREG_TATL_HINT_COMPLETED_GYORG)) {
        return true;
    }

    if (CHECK_QUEST_ITEM(QUEST_REMAINS_TWINMOLD) && !CHECK_WEEKEVENTREG(WEEKEVENTREG_TATL_HINT_COMPLETED_TWINMOLD)) {
        return true;
    }

    return false;
}

void func_80BA165C(void) {
    if (CHECK_QUEST_ITEM(QUEST_REMAINS_ODOLWA)) {
        SET_WEEKEVENTREG(WEEKEVENTREG_TATL_HINT_COMPLETED_ODOLWA);
    }

    if (CHECK_QUEST_ITEM(QUEST_REMAINS_GOHT)) {
        SET_WEEKEVENTREG(WEEKEVENTREG_TATL_HINT_COMPLETED_GOHT);
    }

    if (CHECK_QUEST_ITEM(QUEST_REMAINS_GYORG)) {
        SET_WEEKEVENTREG(WEEKEVENTREG_TATL_HINT_COMPLETED_GYORG);
    }

    if (CHECK_QUEST_ITEM(QUEST_REMAINS_TWINMOLD)) {
        SET_WEEKEVENTREG(WEEKEVENTREG_TATL_HINT_COMPLETED_TWINMOLD);
    }
}

s32 func_80BA16F4(ElfMsg6* this, PlayState* play) {
    if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_TATL_HINT_CLOCK_TOWN_SOUTH_GATE) &&
        (INV_CONTENT(ITEM_MASK_DEKU) == ITEM_MASK_DEKU)) {
        this->actor.textId = 0x216;
        return false;
    }

    if (!func_80BA15A0()) {
        return true;
    }

    if (!CHECK_QUEST_ITEM(QUEST_REMAINS_ODOLWA)) {
        this->actor.textId = 0x256;
        return false;
    }

    if (!CHECK_QUEST_ITEM(QUEST_REMAINS_GOHT)) {
        if (CHECK_WEEKEVENTREG(WEEKEVENTREG_TATL_HINT_CLOCK_TOWN_NORTH_GATE)) {
            this->actor.textId = 0x257;
        } else {
            this->actor.textId = 0x231;
        }
        return false;
    }

    if (!CHECK_QUEST_ITEM(QUEST_REMAINS_GYORG)) {
        if (CHECK_WEEKEVENTREG(WEEKEVENTREG_TATL_HINT_CLOCK_TOWN_WEST_GATE)) {
            this->actor.textId = 0x258;
        } else {
            this->actor.textId = 0x232;
        }
        return false;
    }

    if (!CHECK_QUEST_ITEM(QUEST_REMAINS_TWINMOLD)) {
        if (CHECK_WEEKEVENTREG(WEEKEVENTREG_TATL_HINT_CLOCK_TOWN_EAST_GATE)) {
            this->actor.textId = 0x259;
        } else {
            this->actor.textId = 0x233;
        }
        return false;
    }

    return true;
}

void ElfMsg6_Init(Actor* thisx, PlayState* play) {
    ElfMsg6* this = THIS;

    Actor_ProcessInitChain(&this->actor, sInitChain);

    if (ABS_ALT(this->actor.home.rot.x) == 0) {
        this->actor.scale.z = 0.4f;
        this->actor.scale.x = 0.4f;
    } else {
        this->actor.scale.x = this->actor.scale.z = ABS_ALT(this->actor.home.rot.x) * 0.04f;
    }

    if (this->actor.home.rot.z == 0) {
        this->actor.scale.y = 0.4f;
    } else {
        this->actor.scale.y = this->actor.home.rot.z * 0.04f;
    }

    this->actor.shape.rot.x = this->actor.shape.rot.y = this->actor.shape.rot.z = 0;

    switch (ELFMSG6_GET_TYPE(&this->actor)) {
        case 0:
            this->actionFunc = ElfMsg6_NextRegionHint;
            if (func_80BA16F4(this, play)) {
                Actor_Kill(&this->actor);
                return;
            }
            break;

        case 1:
            this->actionFunc = func_80BA1F80;
            if ((this->actor.csId == CS_ID_NONE) || ((ELFMSG6_SWITCH_FLAG(&this->actor) != 0x7F) &&
                                                     Flags_GetSwitch(play, ELFMSG6_SWITCH_FLAG(&this->actor)))) {
                Actor_Kill(&this->actor);
                return;
            }

            switch (ELFMSG6_GET_SUBTYPE(&this->actor)) {
                case 0:
                    if (gSaveContext.save.saveInfo.inventory.items[ITEM_HOOKSHOT] != ITEM_HOOKSHOT) {
                        Actor_Kill(&this->actor);
                        return;
                    }
                    break;

                case 1:
                    if (CHECK_WEEKEVENTREG(WEEKEVENTREG_83_02)) {
                        Actor_Kill(&this->actor);
                        return;
                    }
                    break;
            }
            break;

        case 2:
            if (INV_CONTENT(ITEM_OCARINA_OF_TIME) == ITEM_OCARINA_OF_TIME) {
                Actor_Kill(&this->actor);
                return;
            }

            if (CHECK_WEEKEVENTREG(WEEKEVENTREG_CLOCK_TOWER_OPENED)) {
                if (CHECK_WEEKEVENTREG(WEEKEVENTREG_TATL_HINT_CLOCK_TOWER_OPENED)) {
                    Actor_Kill(&this->actor);
                    return;
                }
                this->actor.textId = 0x25B;
            } else {
                if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_74_20) ||
                    CHECK_WEEKEVENTREG(WEEKEVENTREG_TATL_HINT_CLOCK_TOWER_MIDNIGHT)) {
                    Actor_Kill(&this->actor);
                    return;
                }
                this->actor.textId = 0x224;
            }
            this->actionFunc = func_80BA1CF8;
            break;

        case 3:
            if (((ELFMSG6_SWITCH_FLAG(&this->actor) != 0x7F) &&
                 Flags_GetSwitch(play, ELFMSG6_SWITCH_FLAG(&this->actor))) ||
                CHECK_WEEKEVENTREG(WEEKEVENTREG_88_10) || CHECK_WEEKEVENTREG(WEEKEVENTREG_91_01) ||
                (INV_CONTENT(ITEM_MASK_ZORA) == ITEM_MASK_ZORA)) {
                Actor_Kill(&this->actor);
                return;
            }
            this->actionFunc = func_80BA2048;
            this->actor.textId = 0x24D;
            break;

        case 4:
            this->actionFunc = func_80BA215C;
            this->actor.textId = 0x255;
            break;

        case 5:
            this->actionFunc = func_80BA21C4;
            this->actor.textId = 0x25E;
            break;

        case 6:
            this->actionFunc = func_80BA21C4;
            this->actor.textId = 0x25F;
            break;

        default:
            this->actionFunc = ElfMsg6_DoNothing;
            break;
    }
}

void ElfMsg6_Destroy(Actor* thisx, PlayState* play) {
}

bool func_80BA1C00(ElfMsg6* this) {
    return (this->actor.xzDistToPlayer < (100.0f * this->actor.scale.x)) &&
           ((this->actor.playerHeightRel >= 0.0f) && (this->actor.playerHeightRel < (100.0f * this->actor.scale.y)));
}

void func_80BA1C88(ElfMsg6* this, PlayState* play, s16 arg2) {
    Player* player = GET_PLAYER(play);
    EnElf* tatl = (EnElf*)player->tatlActor;

    if (player->tatlActor != NULL) {
        player->tatlTextId = arg2;
        CutsceneManager_Queue(CS_ID_GLOBAL_TALK);
        tatl->tatlHintActor = &this->actor;
        if (this->actor.csId == CS_ID_NONE) {
            this->actor.csId = CS_ID_GLOBAL_TALK;
        }
    }
}

void func_80BA1CF8(ElfMsg6* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    EnElf* tatl = (EnElf*)player->tatlActor;

    if (player->tatlActor == NULL) {
        return;
    }

    if (Actor_AcceptTalkRequest(&this->actor, &play->state)) {
        switch (this->actor.textId) {
            case 0x224:
                SET_WEEKEVENTREG(WEEKEVENTREG_TATL_HINT_CLOCK_TOWER_MIDNIGHT);
                break;

            case 0x25B:
                SET_WEEKEVENTREG(WEEKEVENTREG_TATL_HINT_CLOCK_TOWER_OPENED);
                break;
        }
        Actor_Kill(&this->actor);
        return;
    }

    if ((this->actor.textId == 0x224) && CHECK_WEEKEVENTREG(WEEKEVENTREG_CLOCK_TOWER_OPENED)) {
        this->actor.textId = 0x25B;
    } else if (func_80BA1C00(this) && (player->actor.speed > 1.0f)) {
        player->tatlTextId = -this->actor.textId;
        CutsceneManager_Queue(CS_ID_GLOBAL_TALK);
        tatl->tatlHintActor = &this->actor;
        if (this->actor.csId == CS_ID_NONE) {
            this->actor.csId = CS_ID_GLOBAL_TALK;
        }
    }
}

void ElfMsg6_NextRegionHint(ElfMsg6* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    EnElf* tatl = (EnElf*)player->tatlActor;

    if (player->tatlActor == NULL) {
        return;
    }

    if (Actor_AcceptTalkRequest(&this->actor, &play->state)) {
        switch (this->actor.textId) {
            case 0x216:
                SET_WEEKEVENTREG(WEEKEVENTREG_TATL_HINT_CLOCK_TOWN_SOUTH_GATE);
                break;

            case 0x231:
                SET_WEEKEVENTREG(WEEKEVENTREG_TATL_HINT_CLOCK_TOWN_NORTH_GATE);
                break;

            case 0x232:
                SET_WEEKEVENTREG(WEEKEVENTREG_TATL_HINT_CLOCK_TOWN_WEST_GATE);
                break;

            case 0x233:
                SET_WEEKEVENTREG(WEEKEVENTREG_TATL_HINT_CLOCK_TOWN_EAST_GATE);
                break;
        }
        func_80BA165C();
        Actor_Kill(&this->actor);
        return;
    }

    if (func_80BA1C00(this) && (player->actor.speed > 1.0f)) {
        player->tatlTextId = -this->actor.textId;
        CutsceneManager_Queue(CS_ID_GLOBAL_TALK);
        tatl->tatlHintActor = &this->actor;
        if (this->actor.csId == CS_ID_NONE) {
            this->actor.csId = CS_ID_GLOBAL_TALK;
        }
    }
}

void func_80BA1F80(ElfMsg6* this, PlayState* play) {
    if ((ELFMSG6_GET_SUBTYPE(&this->actor) == 1) && CHECK_WEEKEVENTREG(WEEKEVENTREG_83_02)) {
        Actor_Kill(&this->actor);
        return;
    }

    if (func_80BA1C00(this)) {
        if (CutsceneManager_IsNext(this->actor.csId)) {
            CutsceneManager_StartWithPlayerCs(this->actor.csId, NULL);
            Flags_SetSwitch(play, ELFMSG6_SWITCH_FLAG(&this->actor));
            Actor_Kill(&this->actor);
            return;
        }

        CutsceneManager_Queue(this->actor.csId);
    }
}

void ElfMsg6_DoNothing(ElfMsg6* this, PlayState* play) {
}

void func_80BA2048(ElfMsg6* this, PlayState* play) {
    if (Actor_AcceptTalkRequest(&this->actor, &play->state)) {
        EnElf* fairy = (EnElf*)GET_PLAYER(play)->tatlActor;

        fairy->fairyCsFlags |= FAIRY_CS_FLAG_5;
        if (ELFMSG6_SWITCH_FLAG(&this->actor) != 0x7F) {
            Flags_SetSwitch(play, ELFMSG6_SWITCH_FLAG(&this->actor));
        }
        Actor_Kill(&this->actor);
        return;
    }

    if (((ELFMSG6_SWITCH_FLAG(&this->actor) != 0x7F) && Flags_GetSwitch(play, ELFMSG6_SWITCH_FLAG(&this->actor))) ||
        CHECK_WEEKEVENTREG(WEEKEVENTREG_88_10) || CHECK_WEEKEVENTREG(WEEKEVENTREG_91_01) ||
        (INV_CONTENT(ITEM_MASK_ZORA) == ITEM_MASK_ZORA)) {
        Actor_Kill(&this->actor);
        return;
    }

    if (func_80BA1C00(this)) {
        func_80BA1C88(this, play, 0x24D);
    }
}

void func_80BA215C(ElfMsg6* this, PlayState* play) {
    if (Actor_AcceptTalkRequest(&this->actor, &play->state)) {
        Actor_Kill(&this->actor);
        return;
    }

    if (func_80BA1C00(this)) {
        func_80BA1C88(this, play, -this->actor.textId);
    }
}

void func_80BA21C4(ElfMsg6* this, PlayState* play) {
    if (Actor_AcceptTalkRequest(&this->actor, &play->state)) {
        EnElf* fairy = (EnElf*)GET_PLAYER(play)->tatlActor;

        fairy->fairyCsFlags |= FAIRY_CS_FLAG_5;
        if (ELFMSG6_SWITCH_FLAG(&this->actor) != 0x7F) {
            Flags_SetSwitch(play, ELFMSG6_SWITCH_FLAG(&this->actor));
        }
        Actor_Kill(&this->actor);
        return;
    }

    if (((ELFMSG6_SWITCH_FLAG(&this->actor) != 0x7F) && Flags_GetSwitch(play, ELFMSG6_SWITCH_FLAG(&this->actor))) ||
        CHECK_QUEST_ITEM(QUEST_SONG_EPONA)) {
        Actor_Kill(&this->actor);
        return;
    }

    if (func_80BA1C00(this)) {
        func_80BA1C88(this, play, this->actor.textId);
    }
}

void ElfMsg6_Update(Actor* thisx, PlayState* play) {
    ElfMsg6* this = THIS;

    this->actionFunc(this, play);
}
