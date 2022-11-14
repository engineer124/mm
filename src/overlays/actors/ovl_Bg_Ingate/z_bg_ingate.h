#ifndef Z_BG_INGATE_H
#define Z_BG_INGATE_H

#include "global.h"

struct BgIngate;

typedef void (*BgIngateActionFunc)(struct BgIngate*, PlayState*);

#define BGINGATE_GET_FF(thisx) ((thisx)->params & 0xFF)

#define BGINGATE_FLAGS_TIME_PATH_IS_SETUP (1 << 0)
#define BGINGATE_FLAGS_TIME_PATH_HAS_REACHED_END (1 << 1)
#define BGINGATE_FLAGS_4 (1 << 2)
#define BGINGATE_FLAGS_8 (1 << 3)
#define BGINGATE_FLAGS_10 (1 << 4)

typedef struct BgIngate {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ BgIngateActionFunc actionFunc;
    /* 0x160 */ u16 flags;
    /* 0x164 */ Path* timePath;
    /* 0x168 */ s16 timePathTimeSpeed;
    /* 0x16A */ s16 timer;
    /* 0x16C */ s16 unk16C;
    /* 0x16E */ s16 actorCsId;
    /* 0x170 */ Vec3f timePathTargetPos;
    /* 0x17C */ f32 timePathProgress;
    /* 0x180 */ s32 timePathTotalTime;
    /* 0x184 */ s32 timePathWaypointTime;
    /* 0x188 */ s32 timePathWaypoint;
    /* 0x18C */ s32 timePathElapsedTime;
} BgIngate; // size = 0x190

#endif // Z_BG_INGATE_H
