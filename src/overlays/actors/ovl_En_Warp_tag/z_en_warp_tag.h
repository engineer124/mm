#ifndef Z_EN_WARP_TAG_H
#define Z_EN_WARP_TAG_H

#include "global.h"

struct EnWarptag;

typedef void (*EnWarptagActionFunc)(struct EnWarptag*, PlayState*);

typedef enum {
    /* 0x0 */ WARPTAG_CHECK_TIME,
    /* 0x1 */ WARPTAG_CHECK_HEALING,
    /* 0x2 */ WARPTAG_CHECK_EPONAS,
    /* 0x3 */ WARPTAG_CHECK_SOARING,
    /* 0x4 */ WARPTAG_CHECK_SUNS,
    /* 0x5 */ WARPTAG_CHECK_STORMS,
    /* 0xF */ WARPTAG_CHECK_MAX = 0xF
} WarptagOcarinaCheck;

#define WARPTAG_TYPE_NOT_OCARINA (WARPTAG_CHECK_MAX << 6)
// No real define. Indirectly set by `WARPTAG_TYPE_NOT_OCARINA`
// #define WARPTAG_TYPE_OCARINA
#define WARPTAG_TYPE_WARP_PAD 0
#define WARPTAG_TYPE_GROTTO 1

// Only two known Variants:
//  Goron Trial (MOON):    0x03C1
//  Deku Playground:       0x83C0

#define WARPTAG_GET_OCARINA_CHECK_MASK(thisx) ((thisx)->params & 0x3C0)
#define WARPTAG_GET_OCARINA_CHECK(thisx) (((thisx)->params >> 6) & 0xF)

// Grotto Type
#define WARPTAG_GET_EXIT_INDEX(thisx) ((thisx)->params & 0x3F)
// Spawn Type (Warp Pad, Ocarina)
#define WARPTAG_GET_SPAWN_INDEX(thisx) ((thisx)->params & 0x3F)

#define WARPTAG_GET_TYPE(thisx) ((thisx)->params < 0) // 0x8000 flag

typedef struct EnWarptag {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ s8 dangeonKeepObjIndex;
    /* 0x15E */ union {
                    s16 playerSpinYaw; // spins player, motion blur alpha tied to spin
                    s16 grottoExitDelay; // 10 frame delay before player can leave the grotto
                };
    /* 0x160 */ EnWarptagActionFunc actionFunc;
} EnWarptag; // size = 0x164

#endif // Z_EN_WARP_TAG_H
