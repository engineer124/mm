#include "global.h"

extern CutsceneCamera* sCurCsCamera;

// function declarations
s16 func_8016237C(Vec3f* pos, f32* arg1, s16* arg2, CutsceneCameraCmd1Cmd2* arg3, CutsceneCameraCmd3* arg4,
                  SubCutsceneCamera* arg5);
s16 func_8016253C(Vec3f* pos, f32* arg1, s16* arg2, CutsceneCameraCmd1Cmd2* arg3, CutsceneCameraCmd3* arg4,
                  SubCutsceneCamera* arg5);
s16 func_80162A50(Vec3f* pos, f32* arg1, s16* arg2, CutsceneCameraCmd1Cmd2* arg3, CutsceneCameraCmd3* arg4,
                  SubCutsceneCamera* arg5);
s16 func_801623E4(Vec3f* pos, f32* arg1, s16* arg2, CutsceneCameraCmd1Cmd2* arg3, CutsceneCameraCmd3* arg4,
                  SubCutsceneCamera* arg5);
s16 func_80161C20(Vec3f* pos, f32* arg1, s16* arg2, CutsceneCameraCmd1Cmd2* arg3, CutsceneCameraCmd3* arg4,
                  SubCutsceneCamera* arg5);
s16 func_80161E4C(Vec3f* pos, f32* arg1, s16* arg2, CutsceneCameraCmd1Cmd2* arg3, CutsceneCameraCmd3* arg4,
                  SubCutsceneCamera* arg5);
s16 func_801620CC(Vec3f* pos, f32* arg1, s16* arg2, CutsceneCameraCmd1Cmd2* arg3, CutsceneCameraCmd3* arg4,
                  SubCutsceneCamera* arg5);
s16 func_80163334(Vec3f* pos, f32* arg1, s16* arg2, CutsceneCameraCmd1Cmd2* arg3, CutsceneCameraCmd3* arg4,
                  SubCutsceneCamera* arg5);
f32 func_80163660(Actor* actor);

// functions
s16 func_80161180(Vec3f* pos, f32* arg1, s16* arg2, CutsceneCameraCmd1Cmd2* arg3, CutsceneCameraCmd3* arg4,
                  SubCutsceneCamera* arg5) {
    return 0;
}

/**
 * Initializes Cutscene Camera Info
 */
s32 CutsceneCamera_Init(Camera* camera, CutsceneCamera* csCamera) {
    csCamera->camera = camera;

    csCamera->unk_08 = csCamera->unk_0A = 0;
    csCamera->cmdIndex = 0;
    csCamera->unk_00 = -1;
    csCamera->unk_04 = 1;
    csCamera->unk_06 = 0;

    sCurCsCamera = csCamera;

    __osMemset(&csCamera->eyeInterp, 0, 0x30);
    __osMemset(&csCamera->atInterp, 0, 0x30);

    csCamera->eyeInterp.unk_2D = csCamera->atInterp.unk_2D = 7;

    return 1;
}

s16 (*CutsceneCamera_Interpolate(u8 interpType))(Vec3f*, f32*, s16*, CutsceneCameraCmd1Cmd2*, CutsceneCameraCmd3*, SubCutsceneCamera*) {
    switch (interpType) {
        case 7:
        default:
            return func_80161180;
        case 0:
            return func_8016237C;
        case 5:
            return func_8016253C;
        case 4:
            return func_80162A50;
        case 1:
            return func_801623E4;
        case 2:
            return func_80161C20;
        case 3:
            return func_80161E4C;
        case 6:
            return func_801620CC;
    }
}

#ifdef NON_MATCHING
s32 CutsceneCamera_ProcessSubCommands(CutsceneCamera* csCamera) {
    s32 sp5C;
    f32* fov;  // sp58
    s16* roll; // sp54
    s16 (*sp50)(Vec3f*, f32*, s16*, CutsceneCameraCmd1Cmd2*, CutsceneCameraCmd3*, SubCutsceneCamera*);
    Player* player; // sp4C
    Actor* target;  // sp48
    s16 sp46;
    Camera* camera;
    s32 ret;

    sp5C = true;
    if (csCamera->unk_06 == 3) {
        return false;
    }

    camera = csCamera->camera;

    player = GET_PLAYER(camera->globalCtx);
    target = camera->target;

    if (csCamera->eyeCmd[csCamera->atInterp.unk_2C].subCmd1Cmd2[0].interpType <
        csCamera->atCmd[csCamera->eyeInterp.unk_2C].subCmd1Cmd2[0].interpType) {
        sp5C = false;
    }

    csCamera->eyeInterp.unk_00 = camera->eye;
    csCamera->atInterp.unk_00 = camera->at;

    if (sp5C) {
        fov = NULL;
    } else {
        fov = &csCamera->camera->fov;
    }

    if (sp5C) {
        roll = NULL;
    } else {
        roll = &csCamera->camera->roll;
    }

    sp50 = CutsceneCamera_Interpolate(csCamera->atCmd[csCamera->eyeInterp.unk_2C].subCmd1Cmd2[0].interpType);

    switch (csCamera->atCmd[csCamera->eyeInterp.unk_2C].subCmd1Cmd2[0].unk_0A) {
        case 2:
            OLib_DbCameraVec3fDiff(&player->actor.world, &csCamera->camera->at, &csCamera->camera->at, 2);
            break;
        case 3:
            OLib_DbCameraVec3fDiff(&player->actor.world, &csCamera->camera->at, &csCamera->camera->at, 1);
            break;
        case 1:
            OLib_DbCameraVec3fDiff(&player->actor.world, &csCamera->camera->at, &csCamera->camera->at, 1);
            break;
        case 4:
            OLib_DbCameraVec3fDiff(&target->world, &csCamera->camera->at, &csCamera->camera->at, 1);
            break;
        case 5:
            OLib_DbCameraVec3fDiff(&target->world, &csCamera->camera->at, &csCamera->camera->at, 2);
            break;
    }

    sp46 = sp50(&csCamera->camera->at, fov, roll, &csCamera->atCmd[csCamera->eyeInterp.unk_2C],
                &csCamera->cmd3[csCamera->eyeInterp.unk_2C], &csCamera->eyeInterp);

    switch (csCamera->atCmd[csCamera->eyeInterp.unk_2C].subCmd1Cmd2[0].unk_0A) {
        case 2:
            OLib_DbCameraVec3fSum(&player->actor.world, &csCamera->camera->at, &csCamera->camera->at, 2);
            break;
        case 3:
            OLib_DbCameraVec3fSum(&player->actor.world, &csCamera->camera->at, &csCamera->camera->at, 1);
            camera->at.y += func_80163660(&player->actor);
            break;
        case 1:
            OLib_DbCameraVec3fSum(&player->actor.world, &csCamera->camera->at, &csCamera->camera->at, 1);
            break;
        case 4:
            OLib_DbCameraVec3fSum(&target->world, &csCamera->camera->at, &csCamera->camera->at, 1);
            break;
        case 5:
            OLib_DbCameraVec3fSum(&target->world, &csCamera->camera->at, &csCamera->camera->at, 2);
            break;
    }

    csCamera->eyeInterp.unk_2C += sp46;

    if (sp5C) {
        fov = &csCamera->camera->fov;
    } else {
        fov = NULL;
    }

    if (sp5C) {
        roll = &csCamera->camera->roll;
    } else {
        roll = NULL;
    }

    sp50 = CutsceneCamera_Interpolate(csCamera->eyeCmd[csCamera->atInterp.unk_2C].subCmd1Cmd2[0].interpType);

    switch (csCamera->eyeCmd[csCamera->atInterp.unk_2C].subCmd1Cmd2[0].unk_0A) {
        case 2:
            OLib_DbCameraVec3fDiff(&player->actor.world, &csCamera->camera->eye, &csCamera->camera->eye, 2);
            break;
        case 3:
            OLib_DbCameraVec3fDiff(&player->actor.world, &csCamera->camera->eye, &csCamera->camera->eye, 1);
            break;
        case 1:
            OLib_DbCameraVec3fDiff(&player->actor.world, &csCamera->camera->eye, &csCamera->camera->eye, 1);
            break;
        case 4:
            OLib_DbCameraVec3fDiff(&target->world, &csCamera->camera->eye, &csCamera->camera->eye, 1);
            break;
        case 5:
            OLib_DbCameraVec3fDiff(&target->world, &csCamera->camera->eye, &csCamera->camera->eye, 2);
            break;
    }

    sp46 = sp50(&csCamera->camera->eye, fov, roll, &csCamera->eyeCmd[csCamera->atInterp.unk_2C],
                &csCamera->cmd3[csCamera->atInterp.unk_2C], &csCamera->atInterp);

    switch (csCamera->eyeCmd[csCamera->atInterp.unk_2C].subCmd1Cmd2[0].unk_0A) {
        case 2:
            OLib_DbCameraVec3fSum(&player->actor.world, &csCamera->camera->eye, &csCamera->camera->eye, 2);
            break;
        case 3:
            OLib_DbCameraVec3fSum(&player->actor.world, &csCamera->camera->eye, &csCamera->camera->eye, 1);
            camera->eye.y += func_80163660(&player->actor);
            break;
        case 1:
            OLib_DbCameraVec3fSum(&player->actor.world, &csCamera->camera->eye, &csCamera->camera->eye, 1);
            break;
        case 4:
            OLib_DbCameraVec3fSum(&target->world, &csCamera->camera->eye, &csCamera->camera->eye, 1);
            break;
        case 5:
            OLib_DbCameraVec3fSum(&target->world, &csCamera->camera->eye, &csCamera->camera->eye, 2);
            break;
    }

    csCamera->atInterp.unk_2C += sp46;

    if (csCamera->eyeInterp.unk_2C >= csCamera->eyeInterp.numEntries ||
        (csCamera->eyeInterp.unk_2C >= csCamera->atInterp.numEntries)) {
        ret = false;
    } else {
        ret = true;
    }

    return ret;
}
#else
s32 CutsceneCamera_ProcessSubCommands(CutsceneCamera* csCamera);
#pragma GLOBAL_ASM("asm/non_matchings/code/cutscene_camera/CutsceneCamera_ProcessSubCommands.s")
#endif

/**
 * Processes camera cutscene commands
 */
s32 CutsceneCamera_ProcessCommands(u8* cmd, CutsceneCamera* csCamera) {
    CutsceneCameraCmdHeader* cmdHeader;

    switch (csCamera->unk_06) {
        case 999:
            return 0;

        case 2:
            return csCamera->unk_08;

        case 1:
            if (csCamera->unk_0C >= csCamera->unk_0A) {
                csCamera->unk_0A++;
                if (csCamera->unk_0C >= csCamera->unk_0A) {
                    if (!CutsceneCamera_ProcessSubCommands(csCamera)) {
                        csCamera->unk_06 = 3;
                    }
                }
            }
            break;

        case 3:
            break;

        default:
            if (csCamera->unk_04 == 1) {
                // Header
                cmdHeader = (CutsceneCameraCmdHeader*)&cmd[csCamera->cmdIndex];
                csCamera->atInterp.numEntries = csCamera->eyeInterp.numEntries = cmdHeader->numEntries;
                csCamera->unk_0C = cmdHeader->unk_06;
                csCamera->cmdIndex += sizeof(CutsceneCameraCmdHeader);

                // First Command (at)
                csCamera->atCmd = (CutsceneCameraCmd1Cmd2*)&cmd[csCamera->cmdIndex];
                csCamera->cmdIndex += (s16)(csCamera->eyeInterp.numEntries * sizeof(CutsceneCameraCmd1Cmd2));

                // Second Command (eye)
                csCamera->eyeCmd = (CutsceneCameraCmd1Cmd2*)&cmd[csCamera->cmdIndex];
                csCamera->cmdIndex += (s16)(csCamera->eyeInterp.numEntries * sizeof(CutsceneCameraCmd1Cmd2));

                // Third Command
                csCamera->cmd3 = (CutsceneCameraCmd3*)&cmd[csCamera->cmdIndex];
                csCamera->cmdIndex += (s16)(csCamera->eyeInterp.numEntries * sizeof(CutsceneCameraSubCmd3));

                // Other Params
                csCamera->eyeInterp.unk_2C = 0;
                csCamera->atInterp.unk_2C = 0;

                csCamera->unk_04 = 0;
                csCamera->unk_00 = (csCamera->unk_00 & 0xFFFF) + 1;
                csCamera->unk_06 = 0;
                csCamera->unk_08 = csCamera->unk_0A = 0;
                csCamera->eyeInterp.unk_2D = csCamera->atInterp.unk_2D = 7;
            }

            csCamera->unk_08++;

            if (csCamera->unk_0C >= csCamera->unk_0A) {
                csCamera->unk_0A++;
                if (csCamera->unk_0C >= csCamera->unk_0A) {
                    if (!CutsceneCamera_ProcessSubCommands(csCamera)) {
                        csCamera->unk_06 = 3;
                    }
                }
            }
            break;
    }

    if (csCamera->unk_0C < csCamera->unk_08) {
        csCamera->unk_04 = 1;
        cmdHeader = (CutsceneCameraCmdHeader*)&cmd[csCamera->cmdIndex];
        if (cmdHeader->numEntries == -1) {
            csCamera->unk_06 = 999;
            return 0;
        }
    }

    return csCamera->unk_08;
}

// Unused
s16 func_80161BAC(void) {
    u32 ret = sCurCsCamera->unk_06 == 2;

    if (!ret) {
        ret = sCurCsCamera->unk_06 == 1;
    }

    return ret;
}

void func_80161BE0(s16 arg0) {
    if (sCurCsCamera->unk_06 == 0) {
        sCurCsCamera->unk_06 = arg0;
    }
}

void func_80161C0C(void) {
    sCurCsCamera->unk_06 = 0;
}

#ifdef NON_EQUIVALENT
s16 func_80161C20(Vec3f* pos, f32* arg1, s16* arg2, CutsceneCameraCmd1Cmd2* arg3, CutsceneCameraCmd3* arg4,
                  SubCutsceneCamera* arg5) {
    f32 temp_f0;

    if (arg5->unk_2D != 2) {
        arg5->unk_2D = 2;
        arg5->unk_26 = 0;
        arg5->unk_24 = 0;
        arg5->unk_28 = 1;
        if (pos != NULL) {
            arg5->unk_0C.x = pos->x;
            arg5->unk_0C.y = pos->y;
            arg5->unk_0C.z = pos->z;
        }

        if (arg1 != NULL) {
            arg5->unk_18 = *arg1;
        }

        if (arg2 != NULL) {
            arg5->unk_1C = *arg2;
        }
    }

    arg5->unk_24++;

    temp_f0 = ((f32)arg5->unk_24 / arg3->subCmd1Cmd2->unk_02) * (arg3->subCmd1Cmd2->unk_01 / 100.0f);

    if (pos != NULL) {
        pos->x = arg5->unk_0C.x + ((arg3->subCmd1Cmd2->unk_04.x - arg5->unk_0C.x) * temp_f0);
        pos->y = arg5->unk_0C.y + ((arg3->subCmd1Cmd2->unk_04.y - arg5->unk_0C.y) * temp_f0);
        pos->z = arg5->unk_0C.z + ((arg3->subCmd1Cmd2->unk_04.z - arg5->unk_0C.z) * temp_f0);
    }

    if (arg1 != NULL) {
        *arg1 = arg5->unk_18 + ((arg4->subCmd3->unk_04 - arg5->unk_18) * temp_f0);
    }

    if (arg2 != NULL) {
        f32 new_var;
        s16 new_var2;
        s16 new_var3;

        new_var = DEGF_TO_BINANG(arg4->subCmd3->unk_02);
        new_var2 = arg5->unk_1C;
        new_var2 += (s16)((new_var - arg5->unk_1C) * temp_f0);
        *arg2 = new_var2;
    }

    if (arg5->unk_24 >= arg3->subCmd1Cmd2->unk_02) {
        arg5->unk_2D = 7;
        return 1;
    }

    return 0;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/cutscene_camera/func_80161C20.s")
#endif

#ifdef NON_EQUIVALENT
s16 func_80161E4C(Vec3f* arg0, f32* arg1, s16* arg2, CutsceneCameraCmd1Cmd2* arg3, CutsceneCameraCmd3* arg4,
                  SubCutsceneCamera* arg5) {
    s16 temp_t0;
    s32 temp_v1;
    f32 phi_f2;

    if (arg5->unk_2D != 3) {
        arg5->unk_2D = 3;
        arg5->unk_26 = 0;
        arg5->unk_24 = 0;
        arg5->unk_28 = 1;
        if (arg0 != 0) {
            arg5->unk_0C.x = arg0->x;
            arg5->unk_0C.y = arg0->y;
            arg5->unk_0C.z = arg0->z;
        }
        if (arg1 != 0) {
            arg5->unk_18 = *arg1;
        }
        if (arg2 != 0) {
            arg5->unk_1C = *arg2;
        }
    }

    temp_v1 = arg3->subCmd1Cmd2[0].unk_01 + 0x64;

    if (arg3->subCmd1Cmd2[0].unk_02 < 2) {
        phi_f2 = 1.0f;
    } else {
        phi_f2 =
            ((arg5->unk_24 * ((arg3->subCmd1Cmd2[0].unk_01 - 0x64) / (arg3->subCmd1Cmd2[0].unk_02 - 1))) + 100.0f) /
            ((temp_v1 * (arg3->subCmd1Cmd2[0].unk_02 / 2)) + ((temp_v1 / 2) * (arg3->subCmd1Cmd2[0].unk_02 & 1)));
    }
    arg5->unk_24 += 1;
    if (arg0 != 0) {
        arg0->x += (arg3->subCmd1Cmd2[0].unk_04.x - arg5->unk_0C.x) * phi_f2;
        arg0->y += (arg3->subCmd1Cmd2[0].unk_04.y - arg5->unk_0C.y) * phi_f2;
        arg0->z += (arg3->subCmd1Cmd2[0].unk_04.z - arg5->unk_0C.z) * phi_f2;
    }
    if (arg1 != 0) {
        *arg1 += (arg4->subCmd3[0].unk_04 - arg5->unk_18) * phi_f2;
    }
    if (arg2 != 0) {
        *arg2 += (s16)(((s16)((arg4->subCmd3[0].unk_02 * 182.04167f) + .5f) - arg5->unk_1C) * phi_f2);
    }

    if (arg5->unk_24 >= arg3->subCmd1Cmd2[0].unk_02) {
        arg5->unk_2D = 7;
        return 1;
    }
    return 0;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/cutscene_camera/func_80161E4C.s")
#endif

#ifdef NON_EQUIVALENT
s16 func_801620CC(Vec3f* pos, f32* arg1, s16* arg2, CutsceneCameraCmd1Cmd2* arg3, CutsceneCameraCmd3* arg4,
                  SubCutsceneCamera* arg5) {
    f32 sp40;
    f32 sp3C;
    s32 pad0;
    s32 pad1;
    s32 pad2;

    if (arg5->unk_2D != 6) {
        arg5->unk_2D = 6;
        arg5->unk_26 = 0;
        arg5->unk_24 = 0;
        arg5->unk_28 = 1;
        if (pos != NULL) {
            arg5->unk_20 = OLib_Vec3fDist(&arg5->unk_00, pos) * func_80086760(*arg1 * 0.017453292f);
        }
        if (arg1 != NULL) {
            arg5->unk_18 = *arg1;
        }
        if (arg2 != NULL) {
            arg5->unk_1C = *arg2;
        }
    }

    if (arg3->subCmd1Cmd2->unk_02 < 2) {
        sp3C = 1.0f;
    } else {
        sp3C = ((arg5->unk_24 * ((arg3->subCmd1Cmd2->unk_01 - 0x64) / (arg3->subCmd1Cmd2->unk_02 - 1))) + 100.0f) /
               (((arg3->subCmd1Cmd2->unk_01 + 0x64) * (arg3->subCmd1Cmd2->unk_02 / 2)) +
                (((arg3->subCmd1Cmd2->unk_01 + 0x64) / 2) * (arg3->subCmd1Cmd2->unk_02 & 1)));
    }

    arg5->unk_24++;

    if (pos != NULL) {
        OLib_Vec3fDiffToVecSphGeo(&sp40, arg5, pos);
        sp40 = arg5->unk_20 / func_80086760(*arg1 * 0.017453292f);
        OLib_VecSphAddToVec3f(pos, arg5, &sp40);
    }

    if (arg1 != NULL) {
        *arg1 += (arg4->subCmd3[0].unk_04 - arg5->unk_18) * sp3C;
    }

    if (arg2 != NULL) {
        *arg2 += (s16)(((s16)((arg4->subCmd3[0].unk_02 * 182.04167f) + 0.5f) - arg5->unk_1C) * sp3C);
    }

    if (arg5->unk_24 >= arg3->subCmd1Cmd2[0].unk_02) {
        arg5->unk_2D = 7;
        return 1;
    }

    return 0;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/cutscene_camera/func_801620CC.s")
#endif

s16 func_8016237C(Vec3f* arg0, f32* arg1, s16* arg2, CutsceneCameraCmd1Cmd2* arg3, CutsceneCameraCmd3* arg4,
                  SubCutsceneCamera* arg5) {
    if (arg5->unk_2D != 0) {
        arg5->unk_2D = 0;
        arg5->unk_26 = 0;
        arg5->unk_24 = 0;
        arg5->unk_28 = 1;
    }

    arg5->unk_24++;
    if (arg5->unk_24 >= arg3->subCmd1Cmd2[0].unk_02) {
        arg5->unk_2D = 7;
        return true;
    }
    return false;
}

s16 func_801623E4(Vec3f* pos, f32* arg1, s16* arg2, CutsceneCameraCmd1Cmd2* arg3, CutsceneCameraCmd3* arg4,
                  SubCutsceneCamera* arg5) {
    s16 pad;

    if (arg5->unk_2D != 1) {
        arg5->unk_2D = 1;
        arg5->unk_26 = 0;
        arg5->unk_24 = 0;
        arg5->unk_28 = 1;
        if (arg1 != 0) {
            *arg1 = arg4->subCmd3[0].unk_04;
        }
        if (arg2 != 0) {
            *arg2 = DEGF_TO_BINANG(arg4->subCmd3[0].unk_02);
        }
    }

    if (pos != NULL) {
        pos->x = arg3->subCmd1Cmd2[0].unk_04.x;
        pos->y = arg3->subCmd1Cmd2[0].unk_04.y;
        pos->z = arg3->subCmd1Cmd2[0].unk_04.z;
    }

    arg5->unk_24++;

    if (arg5->unk_24 >= arg3->subCmd1Cmd2[0].unk_02) {
        arg5->unk_2D = 7;
        return true;
    }

    return false;
}

void func_801624EC(f32 u, f32* coeff) {
    f32 u1 = 1.0f - u;

    coeff[0] = u1 * u1 * 0.5f;
    coeff[1] = u * u1 + 0.5f;
    coeff[2] = u * u * 0.5f;
}

s16 func_8016253C(Vec3f* pos, f32* arg1, s16* arg2, CutsceneCameraCmd1Cmd2* arg3, CutsceneCameraCmd3* arg4,
                  SubCutsceneCamera* arg5) {
    f32 new_var;
    f32 coeff[3];
    s32 sp3C[3];

    if (arg5->unk_2D != 5) {
        arg5->unk_2D = 5;
        arg5->unk_26 = 0;
        arg5->unk_28 = arg3->subCmd1Cmd2[0].unk_02;
        arg5->unk_24 = 0;
    }

    new_var = (f32)arg5->unk_24 / arg3->subCmd1Cmd2[arg5->unk_26 + 1].unk_02;

    if (arg5->unk_26 < (arg5->unk_28 - 1)) {
        sp3C[0] = arg5->unk_26;
    } else {
        sp3C[0] = arg5->unk_28 - 1;
    }

    if ((arg5->unk_26 + 1) < (arg5->unk_28 - 1)) {
        sp3C[1] = arg5->unk_26 + 1;
    } else {
        sp3C[1] = arg5->unk_28 - 1;
    }

    if ((arg5->unk_26 + 2) < (arg5->unk_28 - 1)) {
        sp3C[2] = arg5->unk_26 + 2;
    } else {
        sp3C[2] = arg5->unk_28 - 1;
    }

    func_801624EC(new_var, coeff);

    if (pos != NULL) {
        pos->x = (coeff[0] * arg3[sp3C[0]].subCmd1Cmd2[0].unk_04.x) +
                 (coeff[1] * arg3[sp3C[1]].subCmd1Cmd2[0].unk_04.x) +
                 (coeff[2] * arg3[sp3C[2]].subCmd1Cmd2[0].unk_04.x);
        pos->y = (coeff[0] * arg3[sp3C[0]].subCmd1Cmd2[0].unk_04.y) +
                 (coeff[1] * arg3[sp3C[1]].subCmd1Cmd2[0].unk_04.y) +
                 (coeff[2] * arg3[sp3C[2]].subCmd1Cmd2[0].unk_04.y);
        pos->z = (coeff[0] * arg3[sp3C[0]].subCmd1Cmd2[0].unk_04.z) +
                 (coeff[1] * arg3[sp3C[1]].subCmd1Cmd2[0].unk_04.z) +
                 (coeff[2] * arg3[sp3C[2]].subCmd1Cmd2[0].unk_04.z);
    }

    if (arg1 != NULL) {
        *arg1 = (coeff[0] * arg4[sp3C[0]].subCmd3[0].unk_04) + (coeff[1] * arg4[sp3C[1]].subCmd3[0].unk_04) +
                (coeff[2] * arg4[sp3C[2]].subCmd3[0].unk_04);
    }

    if (arg2 != NULL) {
        s16 sp34[3];
        s32 sp28[2];
        s32 temp;

        sp34[0] = DEGF_TO_BINANG(arg4[sp3C[0]].subCmd3[0].unk_02);
        sp34[1] = DEGF_TO_BINANG(arg4[sp3C[1]].subCmd3[0].unk_02);
        sp34[2] = DEGF_TO_BINANG(arg4[sp3C[2]].subCmd3[0].unk_02);

        sp28[0] = (s16)(sp34[1] - sp34[0]);
        sp28[1] = sp28[0] + (s16)(sp34[2] - sp34[1]);

        temp = ((coeff[1] * sp28[0]) + (coeff[2] * sp28[1]));
        *arg2 = sp34[0] + temp;
    }

    arg5->unk_24++;

    if (arg5->unk_24 == arg3->subCmd1Cmd2[arg5->unk_26 + 1].unk_02) {
        arg5->unk_26++;
        arg5->unk_24 = 0;
        if (arg5->unk_26 >= (arg5->unk_28 - 2)) {
            arg5->unk_2D = 7;
            return arg5->unk_28;
        }
    }
    return 0;
}

/**
 * This code is very similar to the unused spline system in OoT's func_800BB0A0
 * in which that is based on the Super Mario 64 cutscene camera movement
 */
void func_801629BC(f32 u, f32* coeff) {
    coeff[0] = (1.0f - u) * (1.0f - u) * (1.0f - u) * (1.0f / 6.0f);
    coeff[1] = ((u * u * u * 0.5f) - u * u) + (2.0f / 3.0f);
    coeff[2] = (u * u * u * -0.5f) + (u * u * 0.5f) + (u * 0.5f) + (1.0f / 6.0f);
    coeff[3] = u * u * u * (1.0f / 6.0f);
}

s16 func_80162A50(Vec3f* pos, f32* arg1, s16* arg2, CutsceneCameraCmd1Cmd2* arg3, CutsceneCameraCmd3* arg4,
                  SubCutsceneCamera* arg5) {
    f32 new_var;
    f32 coeff[4];
    s32 sp44[4];

    if (arg5->unk_2D != 4) {
        arg5->unk_2D = 4;
        arg5->unk_26 = 0;
        arg5->unk_28 = arg3->subCmd1Cmd2[0].unk_02;
        arg5->unk_24 = 0;
    }

    new_var = (f32)arg5->unk_24 / arg3->subCmd1Cmd2[arg5->unk_26 + 1].unk_02;

    if (arg5->unk_26 < (arg5->unk_28 - 1)) {
        sp44[0] = arg5->unk_26;
    } else {
        sp44[0] = arg5->unk_28 - 1;
    }

    if ((arg5->unk_26 + 1) < (arg5->unk_28 - 1)) {
        sp44[1] = arg5->unk_26 + 1;
    } else {
        sp44[1] = arg5->unk_28 - 1;
    }

    if ((arg5->unk_26 + 2) < (arg5->unk_28 - 1)) {
        sp44[2] = arg5->unk_26 + 2;
    } else {
        sp44[2] = arg5->unk_28 - 1;
    }

    if ((arg5->unk_26 + 3) < (arg5->unk_28 - 1)) {
        sp44[3] = arg5->unk_26 + 3;
    } else {
        sp44[3] = arg5->unk_28 - 1;
    }

    func_801629BC(new_var, coeff);

    if (pos != NULL) {
        pos->x = (coeff[0] * arg3->subCmd1Cmd2[sp44[0]].unk_04.x) + (coeff[1] * arg3->subCmd1Cmd2[sp44[1]].unk_04.x) +
                 (coeff[2] * arg3->subCmd1Cmd2[sp44[2]].unk_04.x) + (coeff[3] * arg3->subCmd1Cmd2[sp44[3]].unk_04.x);
        pos->y = (coeff[0] * arg3->subCmd1Cmd2[sp44[0]].unk_04.y) + (coeff[1] * arg3->subCmd1Cmd2[sp44[1]].unk_04.y) +
                 (coeff[2] * arg3->subCmd1Cmd2[sp44[2]].unk_04.y) + (coeff[3] * arg3->subCmd1Cmd2[sp44[3]].unk_04.y);
        pos->z = (coeff[0] * arg3->subCmd1Cmd2[sp44[0]].unk_04.z) + (coeff[1] * arg3->subCmd1Cmd2[sp44[1]].unk_04.z) +
                 (coeff[2] * arg3->subCmd1Cmd2[sp44[2]].unk_04.z) + (coeff[3] * arg3->subCmd1Cmd2[sp44[3]].unk_04.z);
    }

    if (arg1 != NULL) {
        *arg1 = (coeff[0] * arg4->subCmd3[sp44[0]].unk_04) + (coeff[1] * arg4->subCmd3[sp44[1]].unk_04) +
                (coeff[2] * arg4->subCmd3[sp44[2]].unk_04) + (coeff[3] * arg4->subCmd3[sp44[3]].unk_04);
    }

    if (arg2 != NULL) {
        s16 sp3C[4];
        s32 sp2C[3];
        s32 temp;

        sp3C[0] = DEGF_TO_BINANG(arg4->subCmd3[sp44[0]].unk_02);
        sp3C[1] = DEGF_TO_BINANG(arg4->subCmd3[sp44[1]].unk_02);
        sp3C[2] = DEGF_TO_BINANG(arg4->subCmd3[sp44[2]].unk_02);
        sp3C[3] = DEGF_TO_BINANG(arg4->subCmd3[sp44[3]].unk_02);

        sp2C[0] = (s16)(sp3C[1] - sp3C[0]);
        sp2C[1] = sp2C[0] + (s16)(sp3C[2] - sp3C[1]);
        sp2C[2] = sp2C[1] + (s16)(sp3C[3] - sp3C[2]);

        temp = ((coeff[1] * sp2C[0]) + (coeff[2] * sp2C[1]) + (coeff[3] * sp2C[2]));
        *arg2 = sp3C[0] + temp;
    }

    arg5->unk_24++;

    if (arg5->unk_24 == arg3->subCmd1Cmd2[arg5->unk_26 + 1].unk_02) {
        arg5->unk_24 = 0;
        arg5->unk_26++;
        if (arg5->unk_26 >= (arg5->unk_28 - 3)) {
            arg5->unk_2D = 7;
            return arg5->unk_28;
        }
    }
    return 0;
}

// Only used by unused func_80162FF8
#pragma GLOBAL_ASM("asm/non_matchings/code/cutscene_camera/func_80162FF8.s")

// Only used by unused func_80163334
#pragma GLOBAL_ASM("asm/non_matchings/code/cutscene_camera/func_801631DC.s")

// Unused
#pragma GLOBAL_ASM("asm/non_matchings/code/cutscene_camera/func_80163334.s")

f32 func_80163660(Actor* actor) {
    if (actor->category != ACTORCAT_PLAYER) {
        return 38.0f - (actor->focus.pos.y - actor->world.pos.y);
    }

    switch (((Player*)actor)->transformation) {
        case PLAYER_FORM_DEKU:
            return -8.0f;
        case PLAYER_FORM_GORON:
            return 23.0f;
        case PLAYER_FORM_ZORA:
            return 27.0f;
        case PLAYER_FORM_FIERCE_DEITY:
            return 17.0f;
        default:
            return 0.0f;
    }
}
