#include "z_en_gakufu.h"

#define FLAGS 0x02000010

#define THIS ((EnGakufu*)thisx)

void EnGakufu_Init(Actor* thisx, GlobalContext* globalCtx);
void EnGakufu_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnGakufu_Update(Actor* thisx, GlobalContext* globalCtx);
void EnGakufu_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80AFC960(EnGakufu* this);
s32 func_80AFCC24(EnGakufu* this, GlobalContext* globalCtx);
void func_80AFCB94(EnGakufu* this, GlobalContext* globalCtx);
void func_80AFCBD4(EnGakufu* this, GlobalContext* globalCtx);
void func_80AFCC14(EnGakufu* this, GlobalContext* globalCtx);
void func_80AFCC58(EnGakufu* this, GlobalContext* globalCtx);
void func_80AFCD44(EnGakufu* this, GlobalContext* globalCtx);
void func_80AFCDC8(EnGakufu* this, GlobalContext* globalCtx);

const ActorInit En_Gakufu_InitVars = {
    ACTOR_EN_GAKUFU,
    ACTORCAT_ITEMACTION,
    FLAGS,
    GAMEPLAY_KEEP,
    sizeof(EnGakufu),
    (ActorFunc)EnGakufu_Init,
    (ActorFunc)EnGakufu_Destroy,
    (ActorFunc)EnGakufu_Update,
    (ActorFunc)EnGakufu_Draw,
};

Vec3f D_80AFD1D0 = {
    -710.0f,
    -123.0f,
    -3528.0f,
};

u8 D_80AFD1DC[] = {
    3, 12, 6, 12,
    9, 12, 0, 12,
    3, 12, 6, 12,
    3, 12, 6, 12,
    3, 12, 6, 12,
    9, 12, 6, 12,
};

u8 D_80AFD1F4[] = {
    ITEM00_RUPEE_RED, ITEM00_RUPEE_RED, ITEM00_RUPEE_RED, // Set 1
    ITEM00_RUPEE_RED, ITEM00_RUPEE_GREEN, ITEM00_RUPEE_GREEN, // Set 2
    ITEM00_RUPEE_BLUE, ITEM00_RUPEE_BLUE, ITEM00_RUPEE_BLUE, // Set 3
    ITEM00_RUPEE_RED, ITEM00_RUPEE_BLUE, ITEM00_RUPEE_BLUE, // Set 4
    ITEM00_RUPEE_GREEN, ITEM00_RUPEE_GREEN, ITEM00_RUPEE_GREEN, // Set 5
};

f32 D_80AFD204[] = {
    -4.0f, -2.0f, 0.0f, 1.0f, 3.0f,
};

s32 D_80AFD218[] = {
    0x020024A0, 0x020025A0, 0x020026A0, 0x020027A0, 0x020028A0, 0x00000000,
};

s32 D_80AFD230[] = {
    0x00140014, 0x00000000, 0x02000000, 0x545400FF, 0xFFEC0014, 0x00000000, 0x00000000, 0x545400FF,
    0xFFECFFEC, 0x00000000, 0x00000200, 0x545400FF, 0x0014FFEC, 0x00000000, 0x02000200, 0x545400FF,
};

s32 D_80AFD270[] = {
    0xE7000000, 0x00000000, 0xD7000002, 0xFFFFFFFF, 0xFC11FFFF, 0xFFFFF238, 0xEF182CA0, 0x0C184A50,
    0xD9000000, 0x00200005, 0x01004008, D_80AFD230, 0x06000204, 0x00000406, 0xDF000000, 0x00000000,
};

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Gakufu/func_80AFC960.s")

void EnGakufu_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnGakufu* this = THIS;

    this->unk_144 = 0x17;
    func_80AFC960(this);
    Actor_SetScale(&this->actor, 1.0f);
    if ((this->actor.params & 0xF) == 1) {
        this->actor.draw = NULL;
        this->actionFunc = func_80AFCBD4;
        return;
    }
    this->actor.flags &= 0xFDFFFFFF;
    if (func_80AFCC24(this, globalCtx) != 0) {
        gSaveContext.eventInf[3] |= 2;
    } else {
        gSaveContext.eventInf[3] &= (u8)~2;
    }
    this->actionFunc = func_80AFCDC8;
    gSaveContext.eventInf[3] &= (u8)~4;
}

void EnGakufu_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnGakufu* this = THIS;

    if ((this->actor.params & 0xF) != 1) {
        gSaveContext.eventInf[3] &= (u8)~2;
    }
}

void func_80AFCB94(EnGakufu* this, GlobalContext* globalCtx) {
    if (this->actor.home.rot.x > 0) {
        this->actor.draw = EnGakufu_Draw;
        this->actor.home.rot.x--;
    } else {
        this->actor.draw = NULL;
        this->actor.home.rot.x = 0;
        this->actionFunc = func_80AFCBD4;
    }
}

void func_80AFCBD4(EnGakufu* this, GlobalContext* globalCtx) {
    if (this->actor.home.rot.x > 0) {
        func_80AFC960(this);
        this->actionFunc = func_80AFCB94;
    }
}

void func_80AFCC14(EnGakufu* this, GlobalContext* globalCtx) {
}

s32 func_80AFCC24(EnGakufu* this, GlobalContext* globalCtx) {
    if (this->actor.xzDistToPlayer < 600.0f) {
        return true;
    } else {
        return false;
    }
}

#ifdef NON_MATCHING
void func_80AFCC58(EnGakufu* this, GlobalContext* globalCtx) {
    f32 phi_f6;
    f32 new_var;
    s32 i;

    play_sound(0x4802);
    phi_f6 = gSaveContext.time * 0.00036621094f;
    
    // 0.00036621094f = 0x39C00000
    for (i = 0; i < 3; i++) {
        Item_DropCollectible(globalCtx, &D_80AFD1D0, D_80AFD1F4[i + D_80AFD1DC[(s32)(phi_f6)]]);
    }

    this->actionFunc = func_80AFCC14;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Gakufu/func_80AFCC58.s")
#endif

void func_80AFCD44(EnGakufu* this, GlobalContext* globalCtx) {
    if (this->actor.cutscene == -1) {
        func_80AFCC58(this, globalCtx);
    } else if (ActorCutscene_GetCanPlayNext(this->actor.cutscene) != 0) {
        ActorCutscene_StartAndSetUnkLinkFields(this->actor.cutscene, &this->actor);
        func_80AFCC58(this, globalCtx);
    } else {
        ActorCutscene_SetIntentToPlay(this->actor.cutscene);
    }

}

void func_80AFCDC8(EnGakufu* this, GlobalContext* globalCtx) {
    if (gSaveContext.eventInf[3] & 2) {
        if (gSaveContext.eventInf[3] & 4) {
            gSaveContext.eventInf[3] &= (u8)~2;
            gSaveContext.eventInf[3] &= (u8)~4;

            this->actionFunc = func_80AFCD44;
            func_80AFCD44(this, globalCtx);
            this->actor.draw = NULL;
        } else if (!func_80AFCC24(this, globalCtx)) {
            gSaveContext.eventInf[3] &= (u8)~2;
        }
    } else if (func_80AFCC24(this, globalCtx)) {
        gSaveContext.eventInf[3] |= 2;
    }
}

void EnGakufu_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnGakufu* this = THIS;

    this->actionFunc(this, globalCtx);
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Gakufu/EnGakufu_Draw.s")
