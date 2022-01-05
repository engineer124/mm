#include "global.h"

extern DbCameraUnkStruct* D_801F6B50;

typedef struct {
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ s16 unk_02;
    /* 0x04 */ Vec3s unk_04;
    /* 0x0E */ s16 unk_0E;
} DbCameraSubStructArg3; // size = 0xC

typedef struct {
    /* 0x00 */ DbCameraSubStructArg3 unkSub[UNK_SIZE];
} DbCameraStructArg3; // size >= 0xC

typedef struct {
    /* 0x00 */ s16 unk_00;
    /* 0x02 */ s16 unk_02;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 unk_06;
} DbCameraSubStructArg4; // size = 0x8

typedef struct {
    /* 0x04 */ DbCameraSubStructArg4 unkSub[UNK_SIZE];
} DbCameraStructArg4; // size >= 0xA

typedef struct {
    /* 0x00 */ s16 unk_00;
    /* 0x02 */ s16 unk_02;
    /* 0x2A */ UNK_TYPE1 unk_04[0x8];
    /* 0x0C */ Vec3f unk_0C;
    /* 0x18 */ f32 unk_18;
    /* 0x1C */ f32 unk_1C;
    /* 0x2A */ UNK_TYPE1 unk_20[0x4];
    /* 0x24 */ s16 unk_24;
    /* 0x26 */ s16 unk_26;
    /* 0x28 */ s16 unk_28;
    /* 0x2A */ UNK_TYPE1 unk_2A[0x3];
    /* 0x2D */ u8 unk_2D;
} DbCameraStructArg5; // size >= 0x2E

// function declarations
s16 func_8016237C(Vec3f* pos, f32* arg1, s16* arg2, DbCameraStructArg3* arg3, DbCameraStructArg4* arg4, DbCameraStructArg5* arg5);
s16 func_8016253C(Vec3f* pos, f32* arg1, s16* arg2, DbCameraStructArg3* arg3, DbCameraStructArg4* arg4, DbCameraStructArg5* arg5);
s16 func_80162A50(Vec3f* pos, f32* arg1, s16* arg2, DbCameraStructArg3* arg3, DbCameraStructArg4* arg4, DbCameraStructArg5* arg5);
s16 func_801623E4(Vec3f* pos, f32* arg1, s16* arg2, DbCameraStructArg3* arg3, DbCameraStructArg4* arg4, DbCameraStructArg5* arg5);
s16 func_80161C20(Vec3f* pos, f32* arg1, s16* arg2, DbCameraStructArg3* arg3, DbCameraStructArg4* arg4, DbCameraStructArg5* arg5);
s16 func_80161E4C(Vec3f* pos, f32* arg1, s16* arg2, DbCameraStructArg3* arg3, DbCameraStructArg4* arg4, DbCameraStructArg5* arg5);
s16 func_801620CC(Vec3f* pos, f32* arg1, s16* arg2, DbCameraStructArg3* arg3, DbCameraStructArg4* arg4, DbCameraStructArg5* arg5);
s16 func_80163334(Vec3f* pos, f32* arg1, s16* arg2, DbCameraStructArg3* arg3, DbCameraStructArg4* arg4, DbCameraStructArg5* arg5);

// functions
s16 func_80161180(Vec3f* pos, f32* arg1, s16* arg2, DbCameraStructArg3* arg3, DbCameraStructArg4* arg4, DbCameraStructArg5* arg5) {
    return 0;
}

s32 func_8016119C(Camera* camera, DbCameraUnkStruct* arg1) {
    arg1->camera = camera;

    arg1->unk_08 = arg1->unk_0A = 0;
    arg1->unk_02 = 0;
    arg1->unk_00 = -1;
    arg1->unk_04 = 1;
    arg1->unk_06 = 0;
    
    D_801F6B50 = arg1;

    __osMemset(&arg1->unk_10, 0, 0x30);
    __osMemset(&arg1->unk_40, 0, 0x30);
    
    arg1->unk_10.unk_2D = arg1->unk_40.unk_2D = 7;

    return 1;
}

s16 (*func_8016122C(u8 arg0))(Vec3f*, f32*, s16*, DbCameraStructArg3*, DbCameraStructArg4*, DbCameraStructArg5*) {
    switch (arg0) {
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

s32 func_801612B8(DbCameraUnkStruct* arg0);
#pragma GLOBAL_ASM("asm/non_matchings/code/db_camera/func_801612B8.s")

#ifdef NON_EQUIVALENT
s32 func_80161998(u8* cmd, DbCameraUnkStruct* arg1) {
    s16 temp_a3;
    s16 temp_v1;
    u8* temp_a0;

    switch (arg1->unk_06) {
        case 999:
            return 0;
        case 2:
            return arg1->unk_08;
        case 1:
            if (arg1->unk_0C >= arg1->unk_0A) {
                arg1->unk_0A++;
                if (arg1->unk_0C >= arg1->unk_0A) {
                    if (func_801612B8(arg1) == 0) {
                        arg1->unk_06 = 3;
                    }
                }
            }
            break;
        case 3:
            break;
        default:
            if (arg1->unk_04 == 1) {
                temp_a0 = &cmd[arg1->unk_02];
                temp_v1 = temp_a0[0];
                arg1->unk_10.unk_2A = temp_v1;
                arg1->unk_40.unk_2A = temp_v1;
                arg1->unk_02 = arg1->unk_02 + 8;
                arg1->unk_0C = temp_a0[6];
                temp_a3 = arg1->unk_10.unk_2A * 0xC;
                arg1->unk_02 = arg1->unk_02 + temp_a3;
                arg1->unk_70 = &cmd[arg1->unk_02];
                arg1->unk_02 = arg1->unk_02 + temp_a3;
                arg1->unk_74 = &cmd[arg1->unk_02];
                arg1->unk_0A = 0;
                arg1->unk_78 = &cmd[arg1->unk_02];
                arg1->unk_02 = arg1->unk_02 + (arg1->unk_10.unk_2A * 8);
                arg1->unk_10.unk_1E[0xE] = 0;
                arg1->unk_40.unk_1E[0xE] = 0;
                arg1->unk_04 = 0;
                arg1->unk_00 += 1;
                arg1->unk_06 = 0;
                arg1->unk_40.unk_2D = 7;
                arg1->unk_10.unk_2D = 7;
                arg1->unk_08 = arg1->unk_0A;
            }


            arg1->unk_08++;
            if (arg1->unk_0C >= arg1->unk_0A) {
                arg1->unk_0A++;
                if (arg1->unk_0C >= arg1->unk_0A) {
                    if (func_801612B8(arg1) == 0) {
                        arg1->unk_06 = 3;
                    }
                }
            }
            break;
    }

    if (arg1->unk_0C < arg1->unk_08) {
        arg1->unk_04 = 1;
        if (cmd[arg1->unk_02] == -1) {
            arg1->unk_06 = 999;
            return 0;
        }
    }
    return arg1->unk_08;

}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/db_camera/func_80161998.s")
#endif

// Unused
s16 func_80161BAC(void) {
    u32 ret = D_801F6B50->unk_06 == 2;

    if (!ret) {
        ret = D_801F6B50->unk_06 == 1;
    }

    return ret;
}

void func_80161BE0(s16 arg0) {
    if (D_801F6B50->unk_06 == 0) {
        D_801F6B50->unk_06 = arg0;
    }
}

void func_80161C0C(void) {
    D_801F6B50->unk_06 = 0;
}

#ifdef NON_EQUIVALENT
s16 func_80161C20(Vec3f* arg0, f32* arg1, s16* arg2, DbCameraStructArg3* arg3, DbCameraStructArg4* arg4, DbCameraStructArg5* arg5) {
    f32 temp_f0;
    f32 new_var;
    s16 new_var2;
    s16 new_var3;

    if (arg5->unk_2D != 2) {
        arg5->unk_2D = 2;
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

    arg5->unk_24++;

    temp_f0 = ((f32)arg5->unk_24 / arg3->unkSub->unk_02) * (arg3->unkSub->unk_01 / 100.0f);

    if (arg0 != 0) {
        arg0->x = arg5->unk_0C.x + ((arg3->unkSub->unk_04.x - arg5->unk_0C.x) * temp_f0);
        arg0->y = arg5->unk_0C.y + ((arg3->unkSub->unk_04.y - arg5->unk_0C.y) * temp_f0);
        arg0->z = arg5->unk_0C.z + ((arg3->unkSub->unk_04.z - arg5->unk_0C.z) * temp_f0);
    }

    if (arg1 != 0) {
        *arg1 = arg5->unk_18 + ((arg4->unkSub->unk_04 - arg5->unk_18) * temp_f0);
    }

    if (arg2 != 0) {
        new_var = DEGF_TO_BINANG(arg4->unkSub->unk_02);
        new_var2 = arg5->unk_1C;
        new_var2 += (s16) ((new_var - arg5->unk_1C) * temp_f0);
        *arg2 = new_var2;
    }

    if (arg5->unk_24 >= arg3->unkSub->unk_02) {
        arg5->unk_2D = 7;
        return 1;
    }

    return 0;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/db_camera/func_80161C20.s")
#endif

#ifdef NON_EQUIVALENT
s16 func_80161E4C(Vec3f* arg0, f32* arg1, s16* arg2, DbCameraStructArg3* arg3, DbCameraStructArg4* arg4, DbCameraStructArg5* arg5) {
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
    
    temp_v1 = arg3->unkSub[0].unk_01 + 0x64;

    if (arg3->unkSub[0].unk_02 < 2) {
        phi_f2 = 1.0f;
    } else {
        phi_f2 = ((arg5->unk_24 * ((arg3->unkSub[0].unk_01 - 0x64) / (arg3->unkSub[0].unk_02 - 1))) + 100.0f) / ((temp_v1 * (arg3->unkSub[0].unk_02 / 2)) + ((temp_v1 / 2) * (arg3->unkSub[0].unk_02 & 1)));
    }
    arg5->unk_24 += 1;
    if (arg0 != 0) {
        arg0->x += (arg3->unkSub[0].unk_04.x - arg5->unk_0C.x) * phi_f2;
        arg0->y += (arg3->unkSub[0].unk_04.y - arg5->unk_0C.y) * phi_f2;
        arg0->z += (arg3->unkSub[0].unk_04.z - arg5->unk_0C.z) * phi_f2;
    }
    if (arg1 != 0) {
        *arg1 += (arg4->unkSub[0].unk_04 - arg5->unk_18) * phi_f2;
    }
    if (arg2 != 0) {
        *arg2 += (s16)(((s16)((arg4->unkSub[0].unk_02 * 182.04167f) + .5f) - arg5->unk_1C) * phi_f2);
    }

    if (arg5->unk_24 >= arg3->unkSub[0].unk_02) {
        arg5->unk_2D = 7;
        return 1;
    }
    return 0;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/db_camera/func_80161E4C.s")
#endif

#pragma GLOBAL_ASM("asm/non_matchings/code/db_camera/func_801620CC.s")

s16 func_8016237C(Vec3f* arg0, f32* arg1, s16* arg2, DbCameraStructArg3* arg3, DbCameraStructArg4* arg4, DbCameraStructArg5* arg5) {
    if (arg5->unk_2D != 0) {
        arg5->unk_2D = 0;
        arg5->unk_26 = 0;
        arg5->unk_24 = 0;
        arg5->unk_28 = 1;
    }

    arg5->unk_24++;
    if (arg5->unk_24 >= arg3->unkSub[0].unk_02) {
        arg5->unk_2D = 7;
        return true;
    }
    return false;
}

s16 func_801623E4(Vec3f* pos, f32* arg1, s16* arg2, DbCameraStructArg3* arg3, DbCameraStructArg4* arg4, DbCameraStructArg5* arg5) {
    s16 phi_v0;

    if (arg5->unk_2D != 1) {
        arg5->unk_2D = 1;
        arg5->unk_26 = 0;
        arg5->unk_24 = 0;
        arg5->unk_28 = 1;
        if (arg1 != 0) {
            *arg1 = arg4->unkSub[0].unk_04;
        }
        if (arg2 != 0) {
            *arg2 = DEGF_TO_BINANG(arg4->unkSub[0].unk_02);
        }
    }
    
    if (pos != NULL) {
        pos->x = arg3->unkSub[0].unk_04.x;
        pos->y = arg3->unkSub[0].unk_04.y;
        pos->z = arg3->unkSub[0].unk_04.z;
    }

    arg5->unk_24++;

    if (arg5->unk_24 >= arg3->unkSub[0].unk_02) {
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

s16 func_8016253C(Vec3f* pos, f32* arg1, s16* arg2, DbCameraStructArg3* arg3, DbCameraStructArg4* arg4, DbCameraStructArg5* arg5) {
    f32 new_var;
    f32 coeff[3];
    s32 sp3C[3];

    if (arg5->unk_2D != 5) {
        arg5->unk_2D = 5;
        arg5->unk_26 = 0;
        arg5->unk_28 = arg3->unkSub[0].unk_02;
        arg5->unk_24 = 0;
    }

    new_var = (f32)arg5->unk_24 / arg3->unkSub[arg5->unk_26 + 1].unk_02;

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
        pos->x = (coeff[0] * arg3[sp3C[0]].unkSub[0].unk_04.x) + (coeff[1] * arg3[sp3C[1]].unkSub[0].unk_04.x) + (coeff[2] * arg3[sp3C[2]].unkSub[0].unk_04.x);
        pos->y = (coeff[0] * arg3[sp3C[0]].unkSub[0].unk_04.y) + (coeff[1] * arg3[sp3C[1]].unkSub[0].unk_04.y) + (coeff[2] * arg3[sp3C[2]].unkSub[0].unk_04.y);
        pos->z = (coeff[0] * arg3[sp3C[0]].unkSub[0].unk_04.z) + (coeff[1] * arg3[sp3C[1]].unkSub[0].unk_04.z) + (coeff[2] * arg3[sp3C[2]].unkSub[0].unk_04.z);
    }

    if (arg1 != NULL) {
        *arg1 = (coeff[0] * arg4[sp3C[0]].unkSub[0].unk_04) + (coeff[1] * arg4[sp3C[1]].unkSub[0].unk_04) + (coeff[2] * arg4[sp3C[2]].unkSub[0].unk_04);
    }

    if (arg2 != NULL) {
        s16 sp34[3];
        s32 sp28[2];
        s32 temp;

        sp34[0] = DEGF_TO_BINANG(arg4[sp3C[0]].unkSub[0].unk_02);
        sp34[1] = DEGF_TO_BINANG(arg4[sp3C[1]].unkSub[0].unk_02);
        sp34[2] = DEGF_TO_BINANG(arg4[sp3C[2]].unkSub[0].unk_02);

        sp28[0] = (s16)(sp34[1] - sp34[0]);
        sp28[1] = sp28[0] + (s16)(sp34[2] - sp34[1]);

        temp = ((coeff[1] * sp28[0]) + (coeff[2] * sp28[1]));
        *arg2 = sp34[0] + temp;
    }

    arg5->unk_24++;

    if (arg5->unk_24 == arg3->unkSub[arg5->unk_26 + 1].unk_02) {
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

s16 func_80162A50(Vec3f* pos, f32* arg1, s16* arg2, DbCameraStructArg3* arg3, DbCameraStructArg4* arg4, DbCameraStructArg5* arg5) {
    f32 new_var;
    f32 coeff[4];
    s32 sp44[4];
    

    if (arg5->unk_2D != 4) {
        arg5->unk_2D = 4;
        arg5->unk_26 = 0;
        arg5->unk_28 = arg3->unkSub[0].unk_02;
        arg5->unk_24 = 0;
    }

    new_var = (f32)arg5->unk_24 / arg3->unkSub[arg5->unk_26 + 1].unk_02;

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
        pos->x = (coeff[0] * arg3->unkSub[sp44[0]].unk_04.x) + (coeff[1] * arg3->unkSub[sp44[1]].unk_04.x) + (coeff[2] * arg3->unkSub[sp44[2]].unk_04.x) + (coeff[3] * arg3->unkSub[sp44[3]].unk_04.x);
        pos->y = (coeff[0] * arg3->unkSub[sp44[0]].unk_04.y) + (coeff[1] * arg3->unkSub[sp44[1]].unk_04.y) + (coeff[2] * arg3->unkSub[sp44[2]].unk_04.y) + (coeff[3] * arg3->unkSub[sp44[3]].unk_04.y);
        pos->z = (coeff[0] * arg3->unkSub[sp44[0]].unk_04.z) + (coeff[1] * arg3->unkSub[sp44[1]].unk_04.z) + (coeff[2] * arg3->unkSub[sp44[2]].unk_04.z) + (coeff[3] * arg3->unkSub[sp44[3]].unk_04.z);
    }

    if (arg1 != NULL) {
        *arg1  = (coeff[0] * arg4->unkSub[sp44[0]].unk_04) + (coeff[1] * arg4->unkSub[sp44[1]].unk_04) + (coeff[2] * arg4->unkSub[sp44[2]].unk_04) + (coeff[3] * arg4->unkSub[sp44[3]].unk_04);
    }

    if (arg2 != NULL) {
        s16 sp3C[4];
        s32 sp2C[3];
        s32 temp;

        sp3C[0] = DEGF_TO_BINANG(arg4->unkSub[sp44[0]].unk_02);
        sp3C[1] = DEGF_TO_BINANG(arg4->unkSub[sp44[1]].unk_02);
        sp3C[2] = DEGF_TO_BINANG(arg4->unkSub[sp44[2]].unk_02);
        sp3C[3] = DEGF_TO_BINANG(arg4->unkSub[sp44[3]].unk_02);

        sp2C[0] = (s16)(sp3C[1] - sp3C[0]);
        sp2C[1] = sp2C[0] + (s16)(sp3C[2] - sp3C[1]);
        sp2C[2] = sp2C[1] + (s16)(sp3C[3] - sp3C[2]);

        temp = ((coeff[1] * sp2C[0]) + (coeff[2] * sp2C[1]) + (coeff[3] * sp2C[2]));
        *arg2 = sp3C[0] + temp;
    }

    arg5->unk_24++;

    if (arg5->unk_24 == arg3->unkSub[arg5->unk_26 + 1].unk_02) {
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
#pragma GLOBAL_ASM("asm/non_matchings/code/db_camera/func_80162FF8.s")

// Only used by unused func_80163334
#pragma GLOBAL_ASM("asm/non_matchings/code/db_camera/func_801631DC.s")

// Unused
#pragma GLOBAL_ASM("asm/non_matchings/code/db_camera/func_80163334.s")

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
