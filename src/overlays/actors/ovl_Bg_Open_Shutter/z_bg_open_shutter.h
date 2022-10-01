#ifndef Z_BG_OPEN_SHUTTER_H
#define Z_BG_OPEN_SHUTTER_H

#include "global.h"

struct BgOpenShutter;

typedef void (*BgOpenShutterActionFunc)(struct BgOpenShutter*, PlayState*);

typedef struct BgOpenShutter {
    /* 0x000 */ DoorSlidingActor door;
    /* 0x160 */ BgOpenShutterActionFunc actionFunc;
    /* 0x164 */ s32 unk_164;
} BgOpenShutter; // size = 0x168

extern const ActorInit Bg_Open_Shutter_InitVars;

#endif // Z_BG_OPEN_SHUTTER_H
