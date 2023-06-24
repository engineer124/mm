#ifndef Z_EN_ZORAEGG_H
#define Z_EN_ZORAEGG_H

#include "global.h"

struct EnZoraegg;

typedef void (*EnZoraeggActionFunc)(struct EnZoraegg*, PlayState*);

#define ZORAEGG_GET_1F(thisx) ((thisx)->params & 0x1F)
#define ZORAEGG_GET_FE00(thisx) (((thisx)->params & 0xFE00) >> 9)

#define ZORAEGG_PARAMS(param1F, paramFE00) (((param1F) & 0x1F) | (((paramFE00) & 0x7F) << 9))

typedef enum {
    /* 0x00 */ ZORAEGG_1F_00,
    /* 0x01 */ ZORAEGG_1F_01,
    /* 0x02 */ ZORAEGG_1F_02,
    /* 0x03 */ ZORAEGG_1F_03,
    /* 0x04 */ ZORAEGG_1F_04,
    /* 0x05 */ ZORAEGG_1F_05,
    /* 0x06 */ ZORAEGG_1F_06,
    /* 0x07 */ ZORAEGG_1F_07,
    /* 0x08 */ ZORAEGG_1F_08,
    /* 0x09 */ ZORAEGG_1F_09,
    /* 0x0A */ ZORAEGG_1F_0A,
    /* 0x0B */ ZORAEGG_1F_0B,
    /* 0x0C */ ZORAEGG_1F_0C,
    /* 0x0D */ ZORAEGG_1F_0D,
    /* 0x0E */ ZORAEGG_1F_0E,
    /* 0x0F */ ZORAEGG_1F_0F,
    /* 0x10 */ ZORAEGG_1F_10,
    /* 0x11 */ ZORAEGG_1F_11,
    /* 0x12 */ ZORAEGG_1F_12,
    /* 0x13 */ ZORAEGG_1F_13,
    /* 0x14 */ ZORAEGG_1F_14,
    /* 0x15 */ ZORAEGG_1F_15,
    /* 0x16 */ ZORAEGG_1F_16,
    /* 0x17 */ ZORAEGG_1F_17,
    /* 0x18 */ ZORAEGG_1F_18,
    /* 0x19 */ ZORAEGG_1F_19
} EnZoraeggParam;

typedef struct EnZoraegg {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ Vec3s jointTable[7];
    /* 0x1B2 */ Vec3s morphTable[7];
    /* 0x1DC */ Actor* unk_1DC;
    /* 0x1E0 */ f32 unk_1E0;
    /* 0x1E4 */ f32 unk_1E4;
    /* 0x1E8 */ s16 unk_1E8;
    /* 0x1EA */ u16 unk_1EA;
    /* 0x1EC */ u8 unk_1EC;
    /* 0x1ED */ u8 unk_1ED;
    /* 0x1EE */ u8 unk_1EE;
    /* 0x1EF */ u8 unk_1EF;
    /* 0x1F0 */ u16 cueType;
    /* 0x1F2 */ s16 unk_1F2;
    /* 0x1F4 */ s16 unk_1F4;
    /* 0x1F8 */ EnZoraeggActionFunc actionFunc;
} EnZoraegg; // size = 0x1FC

#endif // Z_EN_ZORAEGG_H
