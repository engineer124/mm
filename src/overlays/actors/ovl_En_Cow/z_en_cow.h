#ifndef Z_EN_COW_H
#define Z_EN_COW_H

#include "global.h"

#define EN_COW_TYPE(thisx) ((thisx)->params & 0xF)

#define EN_COW_FLAG_IS_TAIL (1 << 0) // This is never set but it is cleared for tail types
#define EN_COW_FLAG_PLAYER_HAS_APPROACHED (1 << 1)
#define EN_COW_FLAG_WONT_GIVE_MILK (1 << 2)

typedef enum {
    /*  0 */ EN_COW_TYPE_DEFAULT,
    /*  1 */ EN_COW_TYPE_TAIL,
    /*  2 */ EN_COW_TYPE_ABDUCTED,
} EnCowType;

struct EnCow;

typedef void (*EnCowActionFunc)(struct EnCow*, GlobalContext*);

typedef enum {
    /* 0 */ COW_LIMB_NONE,
    /* 1 */ COW_LIMB_ROOT,
    /* 2 */ COW_LIMB_HEAD,
    /* 3 */ COW_LIMB_JAW,
    /* 4 */ COW_LIMB_NOSE,
    /* 5 */ COW_LIMB_NOSE_RING,
    /* 6 */ COW_LIMB_MAX,
} ObjectCowLimbs;

typedef struct EnCow {
    /* 0x0000 */ Actor actor;
    /* 0x0144 */ ColliderCylinder colliders[2];
    /* 0x01DC */ SkelAnime skelAnime;
    /* 0x0220 */ Vec3s jointTable[COW_LIMB_MAX];
    /* 0x0244 */ Vec3s morphTable[COW_LIMB_MAX];
    /* 0x0268 */ Vec3s headTilt;
    /* 0x026E */ u16 flags;
    /* 0x0270 */ u16 animationTimer;
    /* 0x0272 */ u16 animationCycle;
    /* 0x0274 */ EnCowActionFunc actionFunc;
} EnCow; // size = 0x278

extern const ActorInit En_Cow_InitVars;

#endif // Z_EN_COW_H
