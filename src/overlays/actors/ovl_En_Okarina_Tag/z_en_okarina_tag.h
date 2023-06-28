#ifndef Z_EN_OKARINA_TAG_H
#define Z_EN_OKARINA_TAG_H

#include "global.h"

struct EnOkarinaTag;

typedef void (*EnOkarinaTagActionFunc)(struct EnOkarinaTag*, PlayState*);

#define ENOKARINATAG_GET_SWITCHFLAGS(thisx) (((thisx)->params & 0x7F))
#define OCARINASPOT_GET_SONG(thisx) (((thisx)->params >> 0x7) & 0xF)
#define OCARINASPOT_GET_TYPE(thisx) (((thisx)->params >> 0xB) & 0x1F)


typedef enum {
    /* 0 */ OCARINASPOT_SET_SWITCH,
    /* 1 */ OCARINASPOT_UNSET_SWITCH,
    /* 2 */ OCARINASPOT_TOGGLE_SWITCH
} OcarinaSpotType;

typedef enum {
    /* -1 */ OCARINASPOT_SONG_ALL_INTERNAL = -1,
    /*  0 */ OCARINASPOT_SONG_ALL_INTERNAL_2,
    /*  1 */ OCARINASPOT_SONG_1,
    /*  2 */ OCARINASPOT_SONG_2,
    /*  3 */ OCARINASPOT_SONG_3,
    /*  4 */ OCARINASPOT_SONG_4,
    /*  5 */ OCARINASPOT_SONG_5,
    /*  6 */ OCARINASPOT_SONG_SCARECROW_SPAWN,
    /*  7 */ OCARINASPOT_SONG_7,
    /*  8 */ OCARINASPOT_SONG_8,
    /*  9 */ OCARINASPOT_SONG_9,
    /*  A */ OCARINASPOT_SONG_SCARECROW_SPAWN_INTERNAL,
    /*  B */ OCARINASPOT_SONG_B,
    /*  C */ OCARINASPOT_SONG_C,
    /*  D */ OCARINASPOT_SONG_D,
    /*  E */ OCARINASPOT_SONG_E,
    /*  F */ OCARINASPOT_SONG_ALL
} OcarinaSpotSong;

typedef struct EnOkarinaTag {
    /* 0x000 */ Actor actor;
    /* 0x144 */ EnOkarinaTagActionFunc actionFunc;
    /* 0x148 */ s16 type;
    /* 0x14A */ s16 ocarinaSong;
    /* 0x14C */ s32 switchFlags;
    /* 0x150 */ s8 debugDisplayType; // debug version only
    /* 0x154 */ f32 xzRange;
    /* 0x158 */ f32 yRange;
} EnOkarinaTag; // size = 0x15C

#endif // Z_EN_OKARINA_TAG_H
