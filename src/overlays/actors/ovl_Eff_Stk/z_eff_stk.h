#ifndef Z_EFF_STK_H
#define Z_EFF_STK_H

#include "global.h"

struct EffStk;

typedef void (*EffStkActionFunc)(struct EffStk*, PlayState*);

typedef struct EffStk {
    /* 0x0000 */ Actor actor;
    /* 0x0144 */ char unk_144[0x8];
    /* 0x014C */ EffStkActionFunc actionFunc;
} EffStk; // size = 0x150

extern const ActorInit Eff_Stk_InitVars;

#endif // Z_EFF_STK_H
