#ifndef Z_OBJ_WTURN_H
#define Z_OBJ_WTURN_H

#include "global.h"

struct ObjWturn;

typedef void (*ObjWturnActionFunc)(struct ObjWturn*, PlayState*);

typedef struct ObjWturn {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ObjWturnActionFunc actionFunc;
    /* 0x148 */ s16 camId;
    /* 0x14A */ s16 unk_14A;
} ObjWturn; // size = 0x14C

extern const ActorInit Obj_Wturn_InitVars;

#endif // Z_OBJ_WTURN_H
