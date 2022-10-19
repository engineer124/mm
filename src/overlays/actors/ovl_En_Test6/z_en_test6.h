#ifndef Z_EN_TEST6_H
#define Z_EN_TEST6_H

#include "global.h"

struct EnTest6;

typedef void (*EnTest6ActionFunc)(struct EnTest6*, PlayState*);

#define SOT_GET_OCARINA_MODE(thisx) ((thisx)->params)

typedef struct {
    /* 0x00 */ LightNode* node;
    /* 0x04 */ LightInfo info;
} EnTest6Light; // size = 0x14

typedef struct EnTest6 {
    /* 0x000 */ Actor actor;
    /* 0x144 */ EnTest6ActionFunc actionFunc;
    /* 0x148 */ Gfx* gfx;
    /* 0x14C */ f32 unk_14C;
    /* 0x150 */ f32 clockDist; // Radius from player
    /* 0x154 */ f32 unk_154;
    /* 0x158 */ f32 clockDistSpeed; // Radial Speed
    /* 0x15C */ f32 unk_15C;
    /* 0x160 */ f32 unk_160;
    /* 0x164 */ EnTest6Light lights[2];
    /* 0x18C */ DbCameraUnkStruct unk_18C;
    /* 0x20C */ Vec3f clockPos[6];
    /* 0x254 */ Vec3f (*unk_254)[64];
    /* 0x258 */ Vec3f subCamAt;
    /* 0x264 */ Vec3f subCamEye;
    /* 0x270 */ f32 subCamFov;
    /* 0x274 */ s16 csState;
    /* 0x276 */ s16 drawType;
    /* 0x278 */ s16 clockYaw;
    /* 0x27A */ s16 unk_27A;
    /* 0x27C */ s16 clockYawSpeed;
    /* 0x27E */ s16 unk_27E;
    /* 0x280 */ s16 unk_280;
    /* 0x282 */ s16 unk_282;
    /* 0x284 */ s16 subCamId;
    /* 0x286 */ s16 unk_286;
} EnTest6; // size = 0x288

#endif // Z_EN_TEST6_H
