#ifndef Z_ITEM_ETCETERA_H
#define Z_ITEM_ETCETERA_H

#include "global.h"

struct ItemEtcetera;

#define ITEMETCETERA_GET_FF(thisx) ((thisx)->params & 0xFF)
#define ITEMETCETERA_GET_TREASUREFLAG(thisx) (((thisx)->params >> 8) & 0x1F)

typedef void (*ItemEtceteraActionFunc)(struct ItemEtcetera*, PlayState*);

typedef enum {
    /* 0x00 */ ITEM_ETC_BOTTLE_1,
    /* 0x01 */ ITEM_ETC_BOTTLE_2,
    /* 0x02 */ ITEM_ETC_BOTTLE_3,
    /* 0x03 */ ITEM_ETC_BOTTLE_4,
    /* 0x04 */ ITEM_ETC_BOTTLE_5,
    /* 0x05 */ ITEM_ETC_BOTTLE_6,
    /* 0x06 */ ITEM_ETC_KEY_SMALL,
    /* 0x07 */ ITEM_ETC_ARROW_FIRE,
    /* 0x08 */ ITEM_ETC_RUPEE_GREEN_CHEST_GAME,
    /* 0x09 */ ITEM_ETC_RUPEE_BLUE_CHEST_GAME,
    /* 0x0A */ ITEM_ETC_RUPEE_RED_CHEST_GAME,
    /* 0x0B */ ITEM_ETC_RUPEE_PURPLE_CHEST_GAME,
    /* 0x0C */ ITEM_ETC_HEART_PIECE_CHEST_GAME,
    /* 0x0D */ ITEM_ETC_KEY_SMALL_CHEST_GAME
} ItemEtceteraType;

typedef struct ItemEtcetera {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ItemEtceteraActionFunc futureActionFunc;
    /* 0x148 */ s16 giDrawId;
    /* 0x14A */ s16 itemID;
    /* 0x14C */ u8 objIndex;
    /* 0x150 */ ActorFunc drawFunc;
    /* 0x154 */ ItemEtceteraActionFunc actionFunc;
} ItemEtcetera; // size = 0x158

extern const ActorInit Item_Etcetera_InitVars;

#endif // Z_ITEM_ETCETERA_H
