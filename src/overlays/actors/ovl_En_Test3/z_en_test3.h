#ifndef Z_EN_TEST3_H
#define Z_EN_TEST3_H

#include "global.h"

struct EnTest3;

typedef void (*EnTest3ActionFunc)(struct EnTest3*, PlayState*);

#define KAFEI_GET_PARAM_1F(thisx) ((thisx)->params & 0x1F)
#define KAFEI_GET_PARAM_1E0(thisx) (((thisx)->params >> 5) & 0xF)

typedef struct {
    /* 0x0 */ u8 unk_0;
    /* 0x1 */ u8 unk_1;
    /* 0x2 */ u16 textId;
} EnTest3_struct_D78; // size = 0x4

typedef struct EnTest3 {
    /* 0x000 */ Player kafeiPlayer;
    /* 0xD78 */ EnTest3_struct_D78* unk_D78;
    /* 0xD7C */ Path* timePath;
    /* 0xD80 */ s32 timePathTimeSpeed;
    /* 0xD84 */ f32 unk_D84;
    /* 0xD88 */ s8 scheduleResult;
    /* 0xD89 */ u8 timePathIsSetup;
    /* 0xD8A */ s16 unk_D8A;
    /* 0xD8C */ u8 unk_D8C;
    /* 0xD8D */ s8 actorCsId;
    /* 0xD8E */ s16 subCamId;
    /* 0xD90 */ Player* linkPlayer;
    /* 0xD94 */ EnTest3ActionFunc actionFunc;
    /* 0xD98 */ Vec3f timePathTargetPos;
    /* 0xDA4 */ f32 timePathProgress;
    /* 0xDA8 */ s32 timePathTotalTime;
    /* 0xDAC */ s32 timePathWaypointTime;
    /* 0xDB0 */ s32 timePathWaypoint;
    /* 0xDB4 */ s32 timePathElapsedTime;
} EnTest3; // size = 0xDB8

#endif // Z_EN_TEST3_H
