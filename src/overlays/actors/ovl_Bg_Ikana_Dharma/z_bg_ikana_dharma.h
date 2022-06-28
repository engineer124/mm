#ifndef Z_BG_IKANA_DHARMA_H
#define Z_BG_IKANA_DHARMA_H

#include "global.h"

#define BGIKANADHARMA_GET_SWITCHFLAG(thisx) (((thisx)->params >> 8) & 0x7F)
#define BGIKANADHARMA_IS_CHILD(thisx) (((thisx)->params >> 5) & 1)
#define BGIKANADHARMA_NUM_SEGMENTS(thisx) ((thisx)->params & 0xF)
#define BGIKANADHARMA_PARAMS(switchFlag, isChild, numSegments) (((switchFlag & 0x7F) << 8) | ((isChild & 1) << 5) | (numSegments & 0xF))

struct BgIkanaDharma;

typedef void (*BgIkanaDharmaActionFunc)(struct BgIkanaDharma*, PlayState*);

typedef struct BgIkanaDharma {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x015C */ ColliderCylinder collider;
    /* 0x01A8 */ BgIkanaDharmaActionFunc actionFunc;
    /* 0x01AC */ s8 cutsceneFramesRemaining;
} BgIkanaDharma; // size = 0x1B0

extern const ActorInit Bg_Ikana_Dharma_InitVars;

#endif // Z_BG_IKANA_DHARMA_H
