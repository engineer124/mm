/*
 * File: z_en_test7.c
 * Overlay: ovl_En_Test7
 * Description: Soaring effects (wings, sphere, etc)
 */

#include "z_en_test7.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define FLAGS (ACTOR_FLAG_10 | ACTOR_FLAG_20 | ACTOR_FLAG_100000 | ACTOR_FLAG_200000 | ACTOR_FLAG_2000000)

#define THIS ((EnTest7*)thisx)

void EnTest7_Init(Actor* thisx, PlayState* play2);
void EnTest7_Destroy(Actor* thisx, PlayState* play);
void EnTest7_Update(Actor* thisx, PlayState* play);
void EnTest7_Draw(Actor* thisx, PlayState* play);

void EnTest7_StartWarpCS(EnTest7* this, PlayState* play);
void EnTest7_WarpCSPart1(EnTest7* this, PlayState* play);
void EnTest7_WarpCSPart2(EnTest7* this, PlayState* play);
void EnTest7_WarpCSPart3(EnTest7* this, PlayState* play);
void EnTest7_WarpCSPart4(EnTest7* this, PlayState* play);
void EnTest7_WarpCSPart5(EnTest7* this, PlayState* play);
void EnTest7_WarpCSPart6(EnTest7* this, PlayState* play);
void EnTest7_WarpCSWait(EnTest7* this, PlayState* play);
void EnTest7_WarpCSWarp(EnTest7* this, PlayState* play);
void EnTest7_PlayerAndCameraControl(EnTest7* this, PlayState* play);

void EnTest7_SetupArriveCs(EnTest7* this, PlayState* play);
void EnTest7_StartArriveCs(EnTest7* this, PlayState* play);
void EnTest7_StartArriveCsSkip(EnTest7* this, PlayState* play);
void EnTest7_ArriveCsPart1(EnTest7* this, PlayState* play);
void EnTest7_ArriveCsPart2(EnTest7* this, PlayState* play);
void EnTest7_ArriveCsPart3(EnTest7* this, PlayState* play);

ActorInit En_Test7_InitVars = {
    ACTOR_EN_TEST7,
    ACTORCAT_ITEMACTION,
    FLAGS,
    GAMEPLAY_KEEP,
    sizeof(EnTest7),
    (ActorFunc)EnTest7_Init,
    (ActorFunc)EnTest7_Destroy,
    (ActorFunc)EnTest7_Update,
    (ActorFunc)EnTest7_Draw,
};

void EnTest7_SetupPlayerCamFunc(EnTest7* this, EnTest7PlayerAndCameraControl playerCamFunc) {
    this->playerCamFunc = playerCamFunc;
}

void EnTest7_SetupAction(EnTest7* this, EnTest7ActionFunc1 actionFunc) {
    this->actionFunc = actionFunc;
}

void EnTest7_InitFeathers(OwlWarpFeather* feathers) {
    OwlWarpFeather* feather = feathers;
    s32 i;

    for (i = 0, feather = feathers; i < OWL_WARP_NUM_FEATHERS; i++, feather++) {
        s32 pad;

        feather->type = OWL_WARP_FEATHER_TYPE_DISABLED;
        feather->unk_04 = 0;
        feather->pos.x = 0.0f;
        feather->pos.y = 0.0f;
        feather->pos.z = 0.0f;
        feather->velocity.x = 0.0f;
        feather->velocity.y = 0.0f;
        feather->velocity.z = 0.0f;
        feather->accel.x = 0.0f;
        feather->accel.y = 0.0f;
        feather->accel.z = 0.0f;
        feather->scale = 0.00001f;
        feather->rot.x = 0;
        feather->rot.y = 0;
        feather->rot.z = 0;
        feather->rotVelocity.x = 0;
        feather->rotVelocity.y = 0;
        feather->rotVelocity.z = 0;
    }
}

void EnTest7_AddFeather(OwlWarpFeather* feather, Vec3f* pos, s32 isPosYRand) {
    s16 sp26 = Rand_ZeroOne() * 0xFFFF;
    f32 sp20;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f4;

    feather->pos = *pos;

    if (isPosYRand) {
        Math_CosS(sp26);
        feather->pos.x += 0;
        feather->pos.y += (Rand_ZeroOne() * 100.0f) - 20.0f;
        Math_SinS(sp26);
        feather->pos.z += 0;
    }

    sp20 = (Rand_ZeroOne() * 4.0f) + 2.0f;

    feather->velocity.x = Math_CosS(sp26) * sp20;
    feather->velocity.y = Rand_ZeroOne();
    feather->velocity.z = Math_SinS(sp26) * sp20;

    feather->accel.x = 0.0f;
    feather->accel.y = 0.0f;
    feather->accel.z = 0.0f;
    feather->scale = 0.25f;
    feather->rot.x = Rand_ZeroOne() * 0x10000;
    feather->rot.y = Rand_ZeroOne() * 0x10000;
    feather->rot.z = Rand_ZeroOne() * 0x10000;
    feather->unk_04 = 60;

    if (Rand_ZeroOne() < 0.9f) {
        feather->type = OWL_WARP_FEATHER_TYPE_1;
        feather->rot.x = Rand_ZeroOne() * 0x10000;
        feather->rot.y = Rand_ZeroOne() * 0x10000;
        feather->rot.z = Rand_ZeroOne() * 0x10000;
        feather->rotVelocity.x = 0;
        feather->rotVelocity.y = 0;
        feather->rotVelocity.z = 0;
    } else {
        feather->type = OWL_WARP_FEATHER_TYPE_2;
        feather->rot.x = 0;
        feather->rot.y = 0;
        feather->rot.z = Rand_ZeroOne() * 5000.0f;
        feather->rotVelocity.x = 0;
        feather->rotVelocity.y = (Rand_ZeroOne() * 8000.0f) + 2000.0f;
        if (Rand_ZeroOne() > 0.5f) {
            feather->rotVelocity.y = -feather->rotVelocity.y;
        }
        feather->rotVelocity.z = 0;
    }
}

void EnTest7_RequestFeather(OwlWarpFeather* feathers, Vec3f* pos, s32 isPosYRand) {
    static s32 sFeatherIndex = 0;
    s32 i;
    s32 featherAdded = false;
    OwlWarpFeather* feather;
    s32 featherIndex;

    for (i = 0, feather = feathers; i < OWL_WARP_NUM_FEATHERS; i++, feather++) {
        if (feather->type == OWL_WARP_FEATHER_TYPE_DISABLED) {
            EnTest7_AddFeather(feather, pos, isPosYRand);
            sFeatherIndex = i;
            featherAdded = true;
            break;
        }
    }

    if (!featherAdded) {
        featherIndex = sFeatherIndex + 1;
        if (featherIndex >= OWL_WARP_NUM_FEATHERS) {
            featherIndex = 0;
        }

        EnTest7_AddFeather(&feathers[featherIndex], pos, isPosYRand);
    }
}

Vec3f D_80AF3414 = { 0.0f, 1.0f, 0.0f };

Vec3f D_80AF3420 = { 0.0f, 0.0f, 1.0f };

void EnTest7_UpdateFeatherType1(PlayState* play, OwlWarpFeather* feather) {
    static MtxF D_80AF38B0;
    static Vec3f D_80AF38F0;
    static f32 D_80AF38FC;
    static Vec3f D_80AF3900;
    s32 sp2C;
    s32 sp28;
    s32 sp24;
    s16 temp_v0_2;
    s16 temp_v0_3;
    s32 temp_v0;
    f32 phi_f8;

    sp2C = feather->unk_04 % 41;
    sp24 = 0;
    sp28 = (feather->unk_04 + 7000) % 41;

    SkinMatrix_SetRotateRPY(&D_80AF38B0, feather->rot.x, feather->rot.y, feather->rot.z);
    SkinMatrix_Vec3fMtxFMultXYZ(&D_80AF38B0, &D_80AF3414, &D_80AF38F0);
    SkinMatrix_Vec3fMtxFMultXYZ(&D_80AF38B0, &D_80AF3420, &D_80AF3900);

    if (feather->rot.x < 0x3448) {
        feather->rot.x += 0x384;
    } else if (feather->rot.x >= 0x4BB9) {
        feather->rot.x -= 0x384;
    } else {
        feather->rot.x = (Math_SinS((sp2C * 65535.0f) / 41.0f) * 2000.0f) + 16384.0f;
    }

    if (feather->rot.y < -0xBB8) {
        feather->rot.y += 0x384;
    } else if (feather->rot.y > 0xBB8) {
        feather->rot.y -= 0x384;
    } else {
        sp24 = 1;
        feather->rot.y = Math_SinS((sp28 * 65535.0f) / 41.0f) * 2000.0f;
    }

    if (sp24 == 1) {
        if (D_80AF38F0.y < 0.0f) {
            feather->velocity.x += D_80AF38F0.x * 0.5f;
            feather->velocity.y += (D_80AF38F0.y * 0.5f) + 0.08f;
            feather->velocity.z += (D_80AF38F0.z * 0.5f);
        } else {
            feather->velocity.x += -D_80AF38F0.x * 0.5f;
            feather->velocity.y += (-D_80AF38F0.y * 0.5f) + 0.08f;
            feather->velocity.z += -D_80AF38F0.z * 0.5f;
        }
    } else if (D_80AF38F0.y < 0.0f) {
        feather->velocity.x += D_80AF38F0.x * 0.2f;
        feather->velocity.y += (D_80AF38F0.y * 0.2f) + 0.08f;
        feather->velocity.z += D_80AF38F0.z * 0.2f;
    } else {
        feather->velocity.x += -D_80AF38F0.x * 0.2f;
        feather->velocity.y += (-D_80AF38F0.y * 0.2f) + 0.08f;
        feather->velocity.z += (-D_80AF38F0.z * 0.2f);
    }

    feather->pos.x += feather->velocity.x;
    feather->pos.y += feather->velocity.y;
    feather->pos.z += feather->velocity.z;
}

void EnTest7_UpdateFeatherType2(PlayState* play, OwlWarpFeather* feather) {
    feather->rot.y += feather->rotVelocity.y;

    feather->accel.x = Rand_Centered();
    feather->accel.y = Rand_Centered() + -0.01f;
    feather->accel.z = Rand_Centered();

    feather->velocity.x += feather->accel.x;
    feather->velocity.y += feather->accel.y;
    feather->velocity.z += feather->accel.z;

    feather->pos.x += feather->velocity.x;
    feather->pos.y += feather->velocity.y;
    feather->pos.z += feather->velocity.z;
}

void EnTest7_UpdateFeathers(PlayState* play, OwlWarpFeather* arg1, EnTest7* this, s32 arg3, s32 arg4) {
    s32 pad[4];
    OwlWarpFeather* ptr;
    s16 phi_s1;
    s32 i;
    f32 temp_f28;
    Vec3f sp8C;
    f32 temp_f0;
    f32 temp_f22;
    f32 temp_f24;
    f32 temp_f26;
    f32 temp_f2;

    for (i = 0, ptr = arg1; i < (s32)(ARRAY_COUNT(this->feathers) * sizeof(this->feathers[0]));
         i += sizeof(this->feathers[0]), ptr++) {
        arg1 = ptr;

        if (arg1->type == OWL_WARP_FEATHER_TYPE_DISABLED) {
            continue;
        }

        if (arg1->type == OWL_WARP_FEATHER_TYPE_1) {
            EnTest7_UpdateFeatherType1(play, arg1);
        } else {
            EnTest7_UpdateFeatherType2(play, arg1);
        }

        if (arg3) {
            temp_f22 = arg1->pos.x - this->actor.world.pos.x;
            temp_f24 = arg1->pos.z - this->actor.world.pos.z;
            temp_f0 = SQ(temp_f22) + SQ(temp_f24);

            phi_s1 = -10000;
            if (temp_f0 > SQ(20.0f)) {
                phi_s1 /= ((temp_f0 - SQ(20.0f)) * 0.00125f) + 1.0f;
            }

            temp_f26 = (temp_f22 * Math_CosS(phi_s1)) - (Math_SinS(phi_s1) * temp_f24);
            temp_f28 = (temp_f22 * Math_SinS(phi_s1)) + (Math_CosS(phi_s1) * temp_f24);

            arg1->pos.x = this->actor.world.pos.x + temp_f26;
            arg1->pos.z = this->actor.world.pos.z + temp_f28;

            temp_f22 = arg1->velocity.x;
            temp_f24 = arg1->velocity.z;

            arg1->velocity.x = (temp_f22 * Math_CosS(phi_s1)) - (Math_SinS(phi_s1) * temp_f24);
            arg1->velocity.z = (temp_f22 * Math_SinS(phi_s1)) + (Math_CosS(phi_s1) * temp_f24);

            temp_f22 = arg1->accel.x;
            temp_f24 = arg1->accel.z;

            arg1->accel.x = (temp_f22 * Math_CosS(phi_s1)) - (Math_SinS(phi_s1) * temp_f24);
            arg1->accel.z = (temp_f22 * Math_SinS(phi_s1)) + (Math_CosS(phi_s1) * temp_f24);
        }

        if (arg4) {
            sp8C.x = arg1->pos.x - this->actor.world.pos.x;
            sp8C.y = arg1->pos.y - (this->actor.world.pos.y + 40.0f);
            sp8C.z = arg1->pos.z - this->actor.world.pos.z;

            temp_f2 = 1.0f - (0.5f / ((Math3D_Vec3fMagnitude(&sp8C) / 500.0f) + 1.0f));

            sp8C.x *= temp_f2;
            sp8C.y *= temp_f2;
            sp8C.z *= temp_f2;

            arg1->pos.x = this->actor.world.pos.x + sp8C.x;
            arg1->pos.y = this->actor.world.pos.y + sp8C.y + 40.0f;
            arg1->pos.z = this->actor.world.pos.z + sp8C.z;
        }
    }
}

void EnTest7_DrawFeathers(PlayState* play2, OwlWarpFeather* feathers) {
    s32 pad[3];
    PlayState* play = play2;
    Mtx* temp_v0;
    OwlWarpFeather* feather;
    s32 i;
    MtxF sp6C;

    OPEN_DISPS(play->state.gfxCtx);

    func_8012C1C0(play->state.gfxCtx);

    Matrix_Push();

    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x80, 255, 255, 255, 255);
    gDPSetEnvColor(POLY_OPA_DISP++, 255, 255, 255, 255);

    for (i = 0, feather = feathers; i < 0x1770; i += 0x3C, feather++) {
        if (feather->type == OWL_WARP_FEATHER_TYPE_DISABLED) {
            continue;
        }

        if ((feather->pos.x > 30000.0f) || (feather->pos.x < -30000.0f) || (feather->pos.y > 30000.0f) ||
            (feather->pos.y < -30000.0f) || (feather->pos.z > 30000.0f) || (feather->pos.z < -30000.0f)) {
            feather->type = OWL_WARP_FEATHER_TYPE_DISABLED;
            continue;
        }

        Matrix_Translate(feather->pos.x, feather->pos.y, feather->pos.z, MTXMODE_NEW);

        if (feather->type == OWL_WARP_FEATHER_TYPE_1) {
            Matrix_RotateZYX(feather->rot.x, feather->rot.y, feather->rot.z, MTXMODE_APPLY);
        } else {
            SkinMatrix_SetRotateYRP(&sp6C, feather->rot.x, feather->rot.y, feather->rot.z);
            Matrix_Mult(&sp6C, MTXMODE_APPLY);
        }

        Matrix_Scale(feather->scale, feather->scale, feather->scale, MTXMODE_APPLY);
        if (feather->type == OWL_WARP_FEATHER_TYPE_2) {
            Matrix_Translate(0.0f, 30.0f, 0.0f, MTXMODE_APPLY);
        }

        temp_v0 = Matrix_NewMtx(play->state.gfxCtx);
        if (temp_v0 != NULL) {
            gSPMatrix(POLY_OPA_DISP++, temp_v0, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_OPA_DISP++, gameplay_keep_DL_081628);
        }
    }

    if (feather) {}

    Matrix_Pop();

    CLOSE_DISPS(play->state.gfxCtx);
}

void EnTest7_InitSpheroid(OwlWarpSpheroid* spheroid) {
    spheroid->unk_00 = 0.0f;
    spheroid->unk_04 = 0.0f;
    spheroid->xzScale = 1.0f;
    spheroid->yScale = 1.0f;
    spheroid->yaw = 0;
}

void EnTest7_Init(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EnTest7* this = THIS;
    Player* player = GET_PLAYER(play);
    Player* player2 = GET_PLAYER(play);

    this->actor.world.rot.y = this->actor.shape.rot.y = player->actor.shape.rot.y;
    this->flags = 0;
    this->csState = 0;
    this->unk_1E8E = player->actor.shape.rot.y;
    this->playerScaleX = player->actor.scale.x;
    this->playerScaleZ = player->actor.scale.z;

    // Keyframe animations
    func_80183430(&this->skeletonInfo, &gameplay_keep_Blob_085640, &gameplay_keep_Blob_083534, this->unk_18FC,
                  this->unk_1BA8, NULL);
    func_801834A8(&this->skeletonInfo, &gameplay_keep_Blob_083534);

    EnTest7_InitFeathers(this->feathers);
    EnTest7_InitSpheroid(&this->spheroid);

    if (ENTEST7_GET(&this->actor) == ENTEST7_ARRIVE) {
        EnTest7_SetupAction(this, EnTest7_SetupArriveCs);
        EnTest7_SetupPlayerCamFunc(this, NULL);
    } else {
        EnTest7_SetupAction(this, EnTest7_StartWarpCS);
        EnTest7_SetupPlayerCamFunc(this, EnTest7_PlayerAndCameraControl);
        Audio_PlayBgm_StorePrevBgm(NA_BGM_SONG_OF_SOARING);
    }

    if (play->playerActorCsIds[8] == -1) {
        Actor_Kill(&this->actor);
        return;
    }

    ActorCutscene_SetIntentToPlay(play->playerActorCsIds[8]);
    player2->stateFlags1 |= PLAYER_STATE1_20;
    Lights_PointNoGlowSetInfo(&this->lightInfo, (Math_SinS(this->unk_1E8E) * 90.0f) + player->actor.world.pos.x,
                              player->actor.world.pos.y + 10.0f,
                              (Math_CosS(this->unk_1E8E) * 90.0f) + player->actor.world.pos.z, 255, 255, 255, 255);
    this->lightNode = LightContext_InsertLight(play, &play->lightCtx, &this->lightInfo);
}

void EnTest7_Destroy(Actor* thisx, PlayState* play) {
    EnTest7* this = THIS;

    ActorCutscene_Stop(play->playerActorCsIds[8]);
    LightContext_RemoveLight(play, &play->lightCtx, this->lightNode);
}

void EnTest7_StartWarpCS(EnTest7* this, PlayState* play) {
    if (!ActorCutscene_GetCanPlayNext(play->playerActorCsIds[8])) {
        ActorCutscene_SetIntentToPlay(play->playerActorCsIds[8]);
    } else {
        ActorCutscene_Start(play->playerActorCsIds[8], NULL);
        EnTest7_SetupAction(this, EnTest7_WarpCSPart1);
        play->unk_18844 = true;
    }
}

void EnTest7_WarpCSPart1(EnTest7* this, PlayState* play) {
    Color_RGB8 sp34 = { 64, 0, 0 };
    Color_RGB8 sp30 = { 220, 220, 255 };
    f32 sp2C = this->csState / 10.0f;

    Environment_LerpAmbientColor(play, &sp30, sp2C);
    Environment_LerpFogColor(play, &sp34, sp2C);
    Environment_LerpFog(play, 2000, 4000, sp2C);

    if (this->csState >= 10) {
        Camera* subCam = Play_GetCamera(play, ActorCutscene_GetCurrentSubCamId(play->playerActorCsIds[8]));

        this->subCamEye = subCam->eye;
        this->subCamAt = subCam->at;
        this->subCamFov = subCam->fov;

        EnTest7_SetupAction(this, EnTest7_WarpCSPart2);
        this->flags |= 0x20;
        Audio_PlaySfxAtPos(&this->actor.projectedPos, NA_SE_PL_WARP_WING_OPEN);
        func_8016566C(120); // enable motion blur
    }
}

void func_80AF1B68(EnTest7* this, PlayState* play) {
    this->flags |= 2;

    if (this->spheroid.unk_04 < 11.0f) {
        this->spheroid.unk_04 = this->spheroid.unk_04 + 0.3f;
        this->spheroid.xzScale = ((this->spheroid.unk_00 * -0.45f) / 11.0f) + 0.7f;
        this->spheroid.yScale = ((this->spheroid.unk_00 * -0.29999998f) / 11.0f) + 0.7f;
    }

    if (this->spheroid.unk_00 < 11.0f) {
        this->spheroid.unk_00 += 1.0f;
        if (this->spheroid.unk_00 > 6.0f) {
            Player* player = GET_PLAYER(play);

            this->flags &= ~1;
            player->actor.draw = NULL;
        }
    } else if (this->csState >= 87) {
        EnTest7_SetupAction(this, EnTest7_WarpCSPart4);
        this->flags &= -9;
        this->spheroid.yaw -= 0x2EE0;
    } else {
        this->spheroid.yaw -= 0x2EE0;
    }
}

void EnTest7_WarpCSPart2(EnTest7* this, PlayState* play) {
    Vec3f featherPos;

    if (func_80183DE0(&this->skeletonInfo)) {
        EnTest7_SetupAction(this, EnTest7_WarpCSPart3);
    }

    if (this->skeletonInfo.frameCtrl.unk_10 > 60.0f) {
        func_80AF1B68(this, play);
    }

    if ((this->skeletonInfo.frameCtrl.unk_10 > 20.0f) && !(this->flags & 0x40)) {
        this->flags |= 0x40;
        Audio_PlaySfxAtPos(&this->actor.projectedPos, NA_SE_PL_WARP_WING_CLOSE);
    }

    if (this->skeletonInfo.frameCtrl.unk_10 > 42.0f) {
        if (!(this->flags & 0x80)) {
            this->flags |= 0x80;
            Audio_PlaySfxAtPos(&this->actor.projectedPos, NA_SE_PL_WARP_WING_ROLL);
        }

        if (Rand_ZeroOne() < 0.3f) {
            Camera* subCam = Play_GetCamera(play, ActorCutscene_GetCurrentSubCamId(play->playerActorCsIds[8]));
            f32 rand = Rand_ZeroOne();

            featherPos.x = ((subCam->eye.x - this->actor.world.pos.x) * rand) + this->actor.world.pos.x;
            featherPos.y = ((subCam->eye.y - this->actor.world.pos.y) * rand) + this->actor.world.pos.y;
            featherPos.z = ((subCam->eye.z - this->actor.world.pos.z) * rand) + this->actor.world.pos.z;

            EnTest7_RequestFeather(this->feathers, &featherPos, true);
            this->flags |= 8;
        }
    } else {
        this->flags |= 1;
    }
}

void EnTest7_WarpCSPart3(EnTest7* this, PlayState* play) {
    Vec3f featherPos;
    Camera* subCam;
    f32 rand;

    func_80AF1B68(this, play);

    if (Rand_ZeroOne() < 0.3f) {
        subCam = Play_GetCamera(play, ActorCutscene_GetCurrentSubCamId(play->playerActorCsIds[8]));
        rand = Rand_ZeroOne();
        featherPos.x = ((subCam->eye.x - this->actor.world.pos.x) * rand) + this->actor.world.pos.x;
        featherPos.y = ((subCam->eye.y - this->actor.world.pos.y) * rand) + this->actor.world.pos.y;
        featherPos.z = ((subCam->eye.z - this->actor.world.pos.z) * rand) + this->actor.world.pos.z;
        EnTest7_RequestFeather(this->feathers, &featherPos, true);
    }

    Math_Vec3f_Copy(&featherPos, &this->actor.world.pos);
    EnTest7_RequestFeather(this->feathers, &featherPos, true);
    this->unk_18FC[1].y += 0x2EE0;
}

void EnTest7_WarpCSPart4(EnTest7* this, PlayState* play) {
    s32 pad;
    s32 temp = this->csState - 86;
    f32 temp_f0 = temp / 10.0f;
    Vec3f featherPos;

    this->flags |= 0x10;

    this->spheroid.xzScale = (-0.15f * temp_f0) + 0.25f;
    this->spheroid.yScale = (-0.3f * temp_f0) + 0.4f;
    this->spheroid.yaw -= 0x2EE0;

    this->flags |= 4;

    if (this->csState >= 96) {
        EnTest7_SetupAction(this, EnTest7_WarpCSPart5);
        this->flags &= ~0x10;
    }
    Math_Vec3f_Copy(&featherPos, &this->actor.world.pos);
    EnTest7_RequestFeather(this->feathers, &featherPos, true);
}

void EnTest7_WarpCSPart5(EnTest7* this, PlayState* play) {
    s32 temp = this->csState - 96;
    f32 four = 4;
    f32 sp1C = 1.0f - (temp / four);
    Camera* subCam;
    f32 temp_f2;
    f32 temp_f4;

    this->spheroid.xzScale = ((temp * -0.1f) / four) + 0.1f;
    this->spheroid.yScale = ((temp * 5.9f) / four) + 0.1f;
    this->spheroid.yaw -= 0x2EE0;

    this->actor.world.pos.y += 100.0f;

    subCam = Play_GetCamera(play, ActorCutscene_GetCurrentSubCamId(play->playerActorCsIds[8]));
    subCam->focalActor = NULL;

    subCam->eye.x = ((subCam->eye.x - this->subCamEye.x) * sp1C) + this->subCamEye.x;
    subCam->eye.y = ((subCam->eye.y - this->subCamEye.y) * sp1C) + this->subCamEye.y;
    subCam->eye.z = ((subCam->eye.z - this->subCamEye.z) * sp1C) + this->subCamEye.z;
    subCam->fov = ((subCam->fov - this->subCamFov) * sp1C) + this->subCamFov;

    if (this->csState >= 100) {
        R_PLAY_FILL_SCREEN_ON = true;
        R_PLAY_FILL_SCREEN_R = 255;
        R_PLAY_FILL_SCREEN_G = 255;
        R_PLAY_FILL_SCREEN_B = 255;
        R_PLAY_FILL_SCREEN_ALPHA = 255;
        play->unk_18844 = false;
        this->flags &= ~4;
        EnTest7_SetupAction(this, EnTest7_WarpCSPart6);
        func_80165690(); // disable motion blur
    }
}

void EnTest7_WarpCSPart6(EnTest7* this, PlayState* play) {
    s32 sp2C = this->csState - 100;
    f32 sp1C;
    Color_RGB8 sp24 = { 64, 0, 0 };
    Color_RGB8 sp20 = { 220, 220, 255 };

    if (R_PLAY_FILL_SCREEN_ON) {
        Audio_PlaySfxAtPos(&this->actor.projectedPos, NA_SE_PL_WARP_WING_VANISH);
        R_PLAY_FILL_SCREEN_ON = false;
        R_PLAY_FILL_SCREEN_R = 0;
        R_PLAY_FILL_SCREEN_G = 0;
        R_PLAY_FILL_SCREEN_B = 0;
        R_PLAY_FILL_SCREEN_ALPHA = 0;
    }

    sp1C = 1.0f - (sp2C / 10.0f);
    Environment_LerpAmbientColor(play, &sp20, sp1C);
    Environment_LerpFogColor(play, &sp24, sp1C);
    Environment_LerpFog(play, 2000, 4000, sp1C);

    if (this->csState >= 110) {
        EnTest7_SetupAction(this, EnTest7_WarpCSWait);
    }
}

void EnTest7_WarpCSWait(EnTest7* this, PlayState* play) {
    if (this->csState >= 130) {
        EnTest7_SetupAction(this, EnTest7_WarpCSWarp);
    }
}

u16 sOwlWarpEntrances[] = {
    ENTRANCE(GREAT_BAY_COAST, 11), ENTRANCE(ZORA_CAPE, 6),
    ENTRANCE(SNOWHEAD, 3),         ENTRANCE(MOUNTAIN_VILLAGE_WINTER, 8),
    ENTRANCE(SOUTH_CLOCK_TOWN, 9), ENTRANCE(MILK_ROAD, 4),
    ENTRANCE(WOODFALL, 4),         ENTRANCE(SOUTHERN_SWAMP_POISONED, 10),
    ENTRANCE(IKANA_CANYON, 4),     ENTRANCE(STONE_TOWER, 3),
};

void EnTest7_WarpCSWarp(EnTest7* this, PlayState* play) {
    Vec3f featherPos;

    if (this) {}

    Math_Vec3f_Copy(&featherPos, &this->actor.world.pos);

    if (Rand_ZeroOne() < 0.1f) {
        EnTest7_RequestFeather(this->feathers, &featherPos, true);
    }

    this->spheroid.yaw -= 0x2EE0;

    if (play->sceneId == SCENE_SECOM) {
        play->nextEntrance = ENTRANCE(IKANA_CANYON, 6);
    } else if (ENTEST7_GET(&this->actor) == OCARINA_MODE_WARP_TO_ENTRANCE) {
        func_80169F78(&play->state);
        gSaveContext.respawn[RESPAWN_MODE_TOP].playerParams =
            (gSaveContext.respawn[RESPAWN_MODE_TOP].playerParams & 0xFF) | 0x600;
        gSaveContext.respawnFlag = -6;
    } else {
        play->nextEntrance = sOwlWarpEntrances[ENTEST7_GET(&this->actor) - OCARINA_MODE_WARP_TO_GREAT_BAY_COAST];
        if ((play->nextEntrance == ENTRANCE(SOUTHERN_SWAMP_POISONED, 10)) && (gSaveContext.save.weekEventReg[20] & 2)) {
            play->nextEntrance = ENTRANCE(SOUTHERN_SWAMP_CLEARED, 10);
        } else if ((play->nextEntrance == ENTRANCE(MOUNTAIN_VILLAGE_WINTER, 8)) &&
                   (gSaveContext.save.weekEventReg[33] & 0x80)) {
            play->nextEntrance = ENTRANCE(MOUNTAIN_VILLAGE_SPRING, 8);
        }
    }

    play->transitionTrigger = TRANS_TRIGGER_START;
    play->transitionType = TRANS_TYPE_02;
    gSaveContext.seqId = (u8)NA_BGM_DISABLED;
    gSaveContext.ambienceId = AMBIENCE_ID_DISABLED;
}

void EnTest7_CameraControl1(EnTest7* this, PlayState* play, f32 lerp) {
    Vec3f sp3C;
    Vec3f* pos;
    Player* player = GET_PLAYER(play);
    Camera* subCam = Play_GetCamera(play, ActorCutscene_GetCurrentSubCamId(play->playerActorCsIds[8]));

    pos = &player->actor.world.pos;
    subCam->focalActor = NULL;

    sp3C.x = ((180.0f * Math_SinS(this->unk_1E8E)) * Math_CosS(0xFA0)) + pos->x;
    sp3C.y = (Math_SinS(0xFA0) * 180.0f) + pos->y;
    sp3C.z = ((180.0f * Math_CosS(this->unk_1E8E)) * Math_CosS(0xFA0)) + pos->z;

    subCam->eye.x = ((sp3C.x - subCam->eye.x) * lerp) + subCam->eye.x;
    subCam->eye.y = ((sp3C.y - subCam->eye.y) * lerp) + subCam->eye.y;
    subCam->eye.z = ((sp3C.z - subCam->eye.z) * lerp) + subCam->eye.z;

    subCam->fov = ((this->subCamFov - subCam->fov) * lerp) + subCam->fov;
    subCam->at.y += 1.4444444f;
}

void EnTest7_CameraControl2(EnTest7* this, PlayState* play, f32 lerp) {
    Vec3f* pos;
    Player* player = GET_PLAYER(play);
    Camera* subCam;
    Vec3f sp30;

    subCam = Play_GetCamera(play, ActorCutscene_GetCurrentSubCamId(play->playerActorCsIds[8]));
    subCam->focalActor = NULL;

    pos = &player->actor.world.pos;

    sp30.x = ((80.0f * Math_SinS(this->unk_1E8E)) * Math_CosS(0xBB8)) + pos->x;
    sp30.y = (Math_SinS(0xBB8) * 80.0f) + pos->y;
    sp30.z = ((80.0f * Math_CosS(this->unk_1E8E)) * Math_CosS(0xBB8)) + pos->z;

    subCam->eye.x = ((sp30.x - subCam->eye.x) * lerp) + subCam->eye.x;
    subCam->eye.y = ((sp30.y - subCam->eye.y) * lerp) + subCam->eye.y;
    subCam->eye.z = ((sp30.z - subCam->eye.z) * lerp) + subCam->eye.z;

    subCam->at.x = ((pos->x - subCam->at.x) * lerp) + subCam->at.x;
    subCam->at.y = (((pos->y + 40.0f) - subCam->at.y) * lerp) + subCam->at.y;
    subCam->at.z = ((pos->z - subCam->at.z) * lerp) + subCam->at.z;

    subCam->fov = ((this->subCamFov - subCam->fov) * lerp) + subCam->fov;
}

void EnTest7_PlayerControl(EnTest7* this, PlayState* play, f32 lerp) {
    Player* player = GET_PLAYER(play);

    // Spin Player
    player->actor.shape.rot.y += 0x2EE0;

    // Squish Player thin
    // player->actor.scale.x = LERPIMP(this->playerScaleX, 0.0f, lerp);
    // player->actor.scale.z = LERPIMP(this->playerScaleZ, 0.0f, lerp);
    player->actor.scale.x = ((0.0f - this->playerScaleX) * lerp) + this->playerScaleX;
    player->actor.scale.z = ((0.0f - this->playerScaleZ) * lerp) + this->playerScaleZ;
}

void EnTest7_PlayerAndCameraControl(EnTest7* this, PlayState* play) {
    f32 lerp;
    f32 sixteen = 16.0f;

    if ((this->csState >= 12) && (this->csState < 31)) {
        lerp = (this->csState - 12) / 18.0f;
        EnTest7_CameraControl1(this, play, lerp);
    } else if ((this->csState >= 79) && (this->csState < 96)) {
        lerp = (this->csState - 79) / sixteen;
        EnTest7_CameraControl2(this, play, lerp);
    }

    if ((this->csState >= 42) && (this->csState < 69)) {
        lerp = (this->csState - 42) / 26.0f;
        EnTest7_PlayerControl(this, play, lerp);
    }
}

void EnTest7_SetupArriveCs(EnTest7* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    this->unk_1E98 = player->actor.draw;
    player->actor.draw = NULL;
    player->stateFlags2 |= PLAYER_STATE2_20000000;
    this->flags |= 2;
    this->spheroid.unk_04 = 30.0f;
    if (play->roomCtx.curRoom.unk3 != 1) {
        EnTest7_SetupAction(this, EnTest7_StartArriveCs);
    } else {
        EnTest7_SetupAction(this, EnTest7_StartArriveCsSkip);
    }
}

s16 D_80AF3450[] = { 0, 0x31C7 };

f32 D_80AF3454 = 3500.0f;

void func_80AF29C0(EnTest7* this, PlayState* play) {
    s32 pad;
    Player* player = GET_PLAYER(play);
    Vec3f* pos = &player->actor.world.pos;
    Camera* subCam = Play_GetCamera(play, ActorCutscene_GetCurrentSubCamId(play->playerActorCsIds[8]));

    subCam->at.x = ((D_80AF3454 * Math_SinS(D_80AF3450[0]) * Math_CosS(D_80AF3450[1]))) + pos->x;
    subCam->at.y = (Math_SinS(D_80AF3450[1]) * D_80AF3454) + pos->y;
    subCam->at.z = ((D_80AF3454 * Math_CosS(D_80AF3450[0])) * Math_CosS(D_80AF3450[1])) + pos->z;

    this->actor.world.pos.x = subCam->at.x;
    this->actor.world.pos.y = subCam->at.y - 40.0f;
    this->actor.world.pos.z = subCam->at.z;
}

void EnTest7_StartArriveCs(EnTest7* this, PlayState* play) {
    Camera* subCam;

    if (!ActorCutscene_GetCanPlayNext(play->playerActorCsIds[8])) {
        ActorCutscene_SetIntentToPlay(play->playerActorCsIds[8]);
        return;
    }

    ActorCutscene_Start(play->playerActorCsIds[8], NULL);
    EnTest7_SetupAction(this, EnTest7_ArriveCsPart1);

    subCam = Play_GetCamera(play, ActorCutscene_GetCurrentSubCamId(play->playerActorCsIds[8]));
    this->subCamEye = subCam->eye;
    this->subCamAt = subCam->at;

    func_80AF29C0(this, play);
}

void func_80AF2BAC(EnTest7* this, PlayState* play, Vec3f* arg2, f32 arg3) {
    f32 x;
    f32 y;
    f32 z;
    Camera* subCam = Play_GetCamera(play, ActorCutscene_GetCurrentSubCamId(play->playerActorCsIds[8]));

    subCam->focalActor = NULL;
    x = ((subCam->at.x - arg2->x) * arg3) + arg2->x;
    y = ((subCam->at.y - arg2->y) * arg3) + arg2->y;
    z = ((subCam->at.z - arg2->z) * arg3) + arg2->z;

    subCam->at.x = x;
    subCam->at.y = y;
    subCam->at.z = z;
}

void EnTest7_ArriveCsPart1(EnTest7* this, PlayState* play) {
    f32 sp24 = (40 - this->csState) / 40.0f;
    Camera* subCam;

    this->spheroid.unk_00 = 11.0f;
    this->flags |= 4;
    this->spheroid.xzScale = 0.05f;
    this->spheroid.yScale = 0.05f;
    this->spheroid.yaw += 0x2EE0;

    this->actor.world.pos.x = ((this->actor.world.pos.x - this->actor.home.pos.x) * sp24) + this->actor.home.pos.x;
    this->actor.world.pos.y = ((this->actor.world.pos.y - this->actor.home.pos.y) * sp24) + this->actor.home.pos.y;
    this->actor.world.pos.z = ((this->actor.world.pos.z - this->actor.home.pos.z) * sp24) + this->actor.home.pos.z;

    subCam = Play_GetCamera(play, ActorCutscene_GetCurrentSubCamId(play->playerActorCsIds[8]));
    func_80AF2BAC(this, play, &this->actor.home.pos, sp24);

    subCam->at.x = this->actor.world.pos.x;
    subCam->at.y = this->actor.world.pos.y + 40.0f;
    subCam->at.z = this->actor.world.pos.z;

    func_800B9010(&this->actor, NA_SE_PL_WARP_WING_ROLL_2 - SFX_FLAG);
    if (this->csState >= 40) {
        this->flags &= ~4;
        EnTest7_SetupAction(this, EnTest7_ArriveCsPart2);
    }
}

void func_80AF2DB4(EnTest7* this, PlayState* play) {
    Camera* subCam;
    Player* player = GET_PLAYER(play);
    Vec3f* pos = &player->actor.world.pos;

    subCam = Play_GetCamera(play, ActorCutscene_GetCurrentSubCamId(play->playerActorCsIds[8]));

    subCam->eye.x = (Math_SinS(-player->actor.shape.rot.y) * 200.0f * -0.83907f) + pos->x;
    subCam->eye.y = pos->y + 108.8042f;
    subCam->eye.z = (Math_CosS(-player->actor.shape.rot.y) * 200.0f * -0.83907f) + pos->z;

    subCam->at.x = pos->x;
    subCam->at.y = pos->y + 40.0f;
    subCam->at.z = pos->z;
}

void EnTest7_StartArriveCsSkip(EnTest7* this, PlayState* play) {
    Camera* subCam;

    if (!ActorCutscene_GetCanPlayNext(play->playerActorCsIds[8])) {
        ActorCutscene_SetIntentToPlay(play->playerActorCsIds[8]);
    } else {
        ActorCutscene_Start(play->playerActorCsIds[8], NULL);
        EnTest7_SetupAction(this, EnTest7_ArriveCsPart2);

        subCam = Play_GetCamera(play, ActorCutscene_GetCurrentSubCamId(play->playerActorCsIds[8]));
        this->subCamEye = subCam->eye;
        this->subCamAt = subCam->at;
        this->csState = 40;

        func_80AF2DB4(this, play);
    }
}

void EnTest7_ArriveCsPart2(EnTest7* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Player* player2 = GET_PLAYER(play);
    Vec3f featherPos;

    func_800B9010(&this->actor, NA_SE_PL_WARP_WING_ROLL_2 - SFX_FLAG);

    featherPos = this->actor.world.pos;

    EnTest7_RequestFeather(this->feathers, &featherPos, true);
    EnTest7_RequestFeather(this->feathers, &featherPos, true);

    this->spheroid.unk_04 = 70 - this->csState;
    if (this->csState > 70) {
        EnTest7_SetupAction(this, EnTest7_ArriveCsPart3);
    }

    if (this->spheroid.unk_04 > 11.0f) {
        f32 temp = this->spheroid.unk_04 - 11.0f;

        this->spheroid.xzScale = ((-0.35f * temp) / 19.0f) + 0.4f;
        this->spheroid.yScale = ((-0.35f * temp) / 19.0f) + 0.4f;
        this->spheroid.yaw += 0x2EE0;
    } else {
        player2->actor.draw = this->unk_1E98;
        this->spheroid.unk_00 = this->spheroid.unk_04;
        this->spheroid.xzScale = ((this->spheroid.unk_04 * -0.29999998f) / 11.0f) + 0.7f;
        this->spheroid.yScale = ((this->spheroid.unk_04 * -0.29999998f) / 11.0f) + 0.7f;
        player->stateFlags2 &= ~PLAYER_STATE2_20000000;
    }
}

void EnTest7_ArriveCsPart3(EnTest7* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (this->csState > 90) {
        player->stateFlags1 &= ~PLAYER_STATE1_20;
        player->stateFlags1 &= ~PLAYER_STATE1_20000000;
        Actor_Kill(&this->actor);
    }
}

void EnTest7_Update(Actor* thisx, PlayState* play) {
    EnTest7* this = THIS;

    this->actionFunc(this, play);

    if (this->playerCamFunc != NULL) {
        this->playerCamFunc(this, play);
    }

    this->csState++;

    EnTest7_UpdateFeathers(play, this->feathers, this, (this->flags & 8) != 0, (this->flags & 0x10) != 0);
}

s32 func_80AF31D0(PlayState* play, SkeletonInfo* skeletonInfo, s32 limbIndex, Gfx** dList, u8* flags, Actor* thisx,
                  Vec3f* scale, Vec3s* rot, Vec3f* pos) {
    EnTest7* this = THIS;
    Vec3f featherPos;

    if ((*dList != NULL) && (Rand_ZeroOne() < 0.03f)) {
        Matrix_MultVec3f(&gZeroVec3f, &featherPos);
        EnTest7_RequestFeather(this->feathers, &featherPos, false);
    }
    return true;
}

void EnTest7_Draw(Actor* thisx, PlayState* play) {
    s32 pad[2];
    EnTest7* this = THIS;
    s32 sp40;

    // Draw wings
    if (this->flags & OWL_WARP_FLAGS_DRAW_WINGS) {
        Mtx* mtx = GRAPH_ALLOC(play->state.gfxCtx, ALIGN16(sizeof(Mtx) * this->skeletonInfo.unk_18->unk_1));

        if (mtx != NULL) {
            func_8018450C(play, &this->skeletonInfo, mtx, func_80AF31D0, NULL, &this->actor);
        } else {
            return;
        }
    }

    // Draw spheroid encasing that surrounds player after wings
    if (this->flags & OWL_WARP_FLAGS_DRAW_SPHEROID) {
        Matrix_Push();
        Matrix_Translate(0.0f, 4000.0f, 0.0f, MTXMODE_APPLY);
        Matrix_RotateZYX(0, this->spheroid.yaw, 0, MTXMODE_APPLY);
        Matrix_Scale(this->spheroid.xzScale * 100.0f, this->spheroid.yScale * 100.0f, this->spheroid.xzScale * 100.0f,
                     MTXMODE_APPLY);
        sp40 = this->spheroid.unk_00;
        AnimatedMat_DrawStep(play, Lib_SegmentedToVirtual(&gSoaringWarpCsSpheroidTexAnim), sp40);
        Gfx_DrawDListXlu(play, gSoaringWarpCsSpheroidDL);
        Matrix_Pop();
    }

    EnTest7_DrawFeathers(play, this->feathers);

    if (this->flags & OWL_WARP_FLAGS_DRAW_LENS_FLARE) {
        Environment_DrawLensFlare(play, &play->envCtx, &play->view, play->state.gfxCtx, this->actor.world.pos, 70.0f,
                                  5.0f, 0, 0);
    }
}
