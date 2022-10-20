#ifndef Z_EN_TEST4_H
#define Z_EN_TEST4_H

#include "global.h"

struct EnTest4;

typedef void (*EnTest4ActionFunc)(struct EnTest4*, PlayState*);

#define THREE_DAY_CLOCK_GET_BELL_VOLUME_INDEX(thisx) (((thisx)->params >> 5) & 0xF)
#define THREE_DAY_CLOCK_GET_SKYBOX_STAR_COUNT(thisx) (((thisx)->params >> 10))


typedef enum {
    /* 0 */ NIGHT_INDEX,
    /* 1 */ DAY_INDEX
} DayNightIndex;

typedef struct EnTest4 {
    /* 0x000 */ Actor actor;
    /* 0x144 */ s8 dayNightIndex; // 0 on night, 1 on day
    /* 0x145 */ u8 transitionCsTimer;
    /* 0x146 */ u16 unk_146;
    /* 0x148 */ u16 nextBellTime; // Next time the bell will sound
    /* 0x14A */ u16 lastBellTime; // Last time the bell sounded
    /* 0x14C */ u8 state;
    /* 0x150 */ EnTest4ActionFunc actionFunc;
} EnTest4; // size = 0x154

typedef enum {
    /* 0 */ TEST4_STATE_CLEAR,
    /* 1 */ TEST4_STATE_RAIN
} EnTest4State;

#endif // Z_EN_TEST4_H
