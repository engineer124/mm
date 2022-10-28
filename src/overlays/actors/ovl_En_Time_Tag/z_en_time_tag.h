#ifndef Z_EN_TIME_TAG_H
#define Z_EN_TIME_TAG_H

#include "global.h"

struct EnTimeTag;

typedef void (*EnTimeTagActionFunc)(struct EnTimeTag*, PlayState*);

#define ENTIMETAG_GET_TYPE(thisx) (((thisx)->params & 0xE000) >> 0xD)
#define ENTIMETAG_GET_KICKOUT_TEXT_TYPE(thisx) (((thisx)->params & 0x1FE0) >> 0x5)
#define ENTIMETAG_GET_KICKOUT_EXIT_INDEX(thisx) ((thisx)->params & 0x1F)
#define ENTIMETAG_GET_SWITCHFLAG(thisx) ((thisx)->params & 0x7F)

typedef enum {
    /* 0 */ TIMETAG_KICKOUT_DOOR,
    /* 1 */ TIMETAG_SOARING_ENGRAVING,
    /* 2 */ TIMETAG_ROOFTOP_OATH,
    /* 3 */ TIMETAG_MIKAU_DIARY,
    /* 4 */ TIMETAG_KICKOUT_SPECIAL,
} TimeTagType;

typedef enum {
    /*  0 */ KICKOUT_TEXT_TYPE_0, // I am closing the shop
    /*  1 */ KICKOUT_TEXT_TYPE_1, // time to close shop
    /*  2 */ KICKOUT_TEXT_TYPE_2, // stock pot in locking the door
    /*  3 */ KICKOUT_TEXT_TYPE_3, // need some alone time
    /*  4 */ KICKOUT_TEXT_TYPE_4, // business is finished
    /*  5 */ KICKOUT_TEXT_TYPE_5, // the lady needs a break
    /*  6 */ KICKOUT_TEXT_TYPE_6, // place is about to close
    /*  7 */ KICKOUT_TEXT_TYPE_7, // they came
    /*  8 */ KICKOUT_TEXT_TYPE_8, // we have to close shop
    /*  9 */ KICKOUT_TEXT_TYPE_9, // I have to get ready for business
    /* 10 */ KICKOUT_TEXT_TYPE_10, // shop is opening, you need to go
    /* 11 */ KICKOUT_TEXT_TYPE_11, // shop is opening, we're in the way
    /* 12 */ KICKOUT_TEXT_TYPE_12, // it's time to close
    /* 13 */ KICKOUT_TEXT_TYPE_13, // quick, it's closing time
    /* 14 */ KICKOUT_TEXT_TYPE_14, // need to cut the moon, must leave
    /* 15 */ KICKOUT_TEXT_TYPE_15, // already time to close shop
    /* 16 */ KICKOUT_TEXT_TYPE_16, // no more for today
} EnTimeTagKickOutTextType;

typedef struct EnTimeTag {
    /* 0x000 */ Actor actor;
    /* 0x144 */ EnTimeTagActionFunc actionFunc;
} EnTimeTag; // size = 0x148

#endif // Z_EN_TIME_TAG_H
