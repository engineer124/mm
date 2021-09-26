#ifndef Z_EN_GAKUFU_H
#define Z_EN_GAKUFU_H

#include "global.h"

struct EnGakufu;

typedef void (*EnGakufuActionFunc)(struct EnGakufu*, GlobalContext*);

typedef struct EnGakufu {
    /* 0x000 */ Actor actor;
    /* 0x144 */ s32 unk_144;
    /* 0x148 */ s32 unk_148;
    /* 0x14C */ s32 unk_14C;
    /* 0x150 */ EnGakufuActionFunc actionFunc;
} EnGakufu; // size = 0x154

extern const ActorInit En_Gakufu_InitVars;

#endif // Z_EN_GAKUFU_H
