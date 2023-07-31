#ifndef Z_EN_ELF_H
#define Z_EN_ELF_H

#include "global.h"
#include "objects/gameplay_keep/gameplay_keep.h"

struct EnElf;

typedef void (*EnElfActionFunc)(struct EnElf*, PlayState*);
typedef void (*EnElfUnkFunc)(struct EnElf*, PlayState*);

#define FAIRY_GET_TYPE(thisx) ((thisx)->params & 0xF)
#define FAIRY_GET_COLLECTIBLE_FLAG(thisx) (((thisx)->params & 0xFE00) >> 9)

#define FAIRY_PARAMS(type, boolParam, collectibleFlag) (((type) /* & 0xF */) | (((boolParam) & 0x1) << 8) | ((((collectibleFlag) & 0x7F) << 9) & 0xFE00))

typedef enum {
    /* 0x0 */ FAIRY_TYPE_TATL,
    /* 0x1 */ FAIRY_TYPE_BOTTLE_RELEASE_1,
    /* 0x2 */ FAIRY_TYPE_HEAL_TIMED,
    /* 0x3 */ FAIRY_TYPE_KOKIRI, // Remnant of OoT
    /* 0x4 */ FAIRY_TYPE_SPAWNER,
    /* 0x5 */ FAIRY_TYPE_REVIVE_DEATH,
    /* 0x6 */ FAIRY_TYPE_HEAL,
    /* 0x7 */ FAIRY_TYPE_HEAL_BIG,
    /* 0x8 */ FAIRY_TYPE_BOTTLE_RELEASE_0,
    /* 0x9 */ FAIRY_TYPE_9,
    /* 0xA */ FAIRY_TYPE_10
} FairyType;

typedef enum {
    /* 0x0 */ FAIRY_STATE_0,
    /* 0x1 */ FAIRY_STATE_1,
    /* 0x2 */ FAIRY_STATE_2,
    /* 0x3 */ FAIRY_STATE_3,
    /* 0x4 */ FAIRY_STATE_4,
    /* 0x5 */ FAIRY_STATE_5,
    /* 0x6 */ FAIRY_STATE_6,
    /* 0x7 */ FAIRY_STATE_7,
    /* 0x8 */ FAIRY_STATE_8,
    /* 0x9 */ FAIRY_STATE_9,
    /* 0xA */ FAIRY_STATE_10
} FairyState;

#define FAIRY_FLAG_0 (1 << 0)
#define FAIRY_FLAG_1 (1 << 1)
#define FAIRY_FLAG_TATL (1 << 2)
#define FAIRY_FLAG_NO_DRAW (1 << 3)

#define FAIRY_FLAG_MOVE_TO_PLAYER (1 << 4)
#define FAIRY_FLAG_5 (1 << 5)
#define FAIRY_FLAG_6 (1 << 6)
#define FAIRY_FLAG_7 (1 << 7)

#define FAIRY_FLAG_TIMED (1 << 8)
#define FAIRY_FLAG_BIG (1 << 9)
#define FAIRY_FLAG_10 (1 << 10)
#define FAIRY_FLAG_11 (1 << 11)

#define FAIRY_FLAG_12 (1 << 12)
#define FAIRY_FLAG_13 (1 << 13)
#define FAIRY_FLAG_14 (1 << 14)
#define FAIRY_FLAG_15 (1 << 15)


#define FAIRY_CS_FLAG_0 (1 << 0)
#define FAIRY_CS_FLAG_1 (1 << 1)
#define FAIRY_CS_FLAG_2 (1 << 2)
#define FAIRY_CS_FLAG_3 (1 << 3)

#define FAIRY_CS_FLAG_4 (1 << 4)
#define FAIRY_CS_FLAG_5 (1 << 5)

typedef struct EnElf {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ Vec3s jointTable[FAIRY_LIMB_MAX];
    /* 0x1B2 */ Vec3s morphTable[FAIRY_LIMB_MAX];
    /* 0x1DC */ Color_RGBAf innerColor;
    /* 0x1EC */ Color_RGBAf outerColor;
    /* 0x1FC */ LightInfo lightInfoGlow;
    /* 0x20C */ LightNode* lightNodeGlow;
    /* 0x210 */ LightInfo lightInfoNoGlow;
    /* 0x220 */ LightNode* lightNodeNoGlow;
    /* 0x224 */ Vec3f unkPos;
    /* 0x230 */ Actor* tatlHintActor;
    /* 0x234 */ Actor* unk_234;
    /* 0x238 */ f32 unk_238;
    /* 0x23C */ f32 unk_23C;
    /* 0x240 */ f32 unk_240;
    /* 0x244 */ s16 fairyState;
    /* 0x246 */ s16 unkPitch; // union?
    /* 0x248 */ s16 unkYaw;
    /* 0x24A */ s16 unkYawAngPitch;
    /* 0x24C */ s16 unkYawAngVel;
    /* 0x250 */ f32 unk_250;
    /* 0x254 */ f32 unk_254;
    /* 0x258 */ s16 yaw;
    /* 0x25A */ u16 timer;
    /* 0x25C */ s16 unkTimer;
    /* 0x25E */ s16 disappearTimer;
    /* 0x260 */ s16 collectibleFlag;
    /* 0x262 */ u16 fairyFlags;
    /* 0x264 */ u16 fairyCsFlags;
    /* 0x266 */ u16 hintTextId;
    /* 0x268 */ u8 unk_268;
    /* 0x269 */ u8 unk_269;
    /* 0x26C */ EnElfUnkFunc unkFunc;
    /* 0x270 */ EnElfActionFunc actionFunc;
} EnElf; // size = 0x274

#endif // Z_EN_ELF_H
