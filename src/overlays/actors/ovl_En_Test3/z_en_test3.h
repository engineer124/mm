#ifndef Z_EN_TEST3_H
#define Z_EN_TEST3_H

#include "global.h"

struct EnTest3;

typedef void (*EnTest3ActionFunc)(struct EnTest3*, PlayState*);

typedef struct {
    /* 0 */ u8 unk_0;
    /* 1 */ u8 unk_1;
    /* 2 */ u16 unk_2;
} struct_D78; // size = 0x4

typedef struct EnTest3 {
    /* 0x000 */ Player player;
    /* 0xD78 */ struct_D78* unk_D78;
    /* 0xD7C */ Path* unk_D7C;
    /* 0xD80 */ s32 unk_D80;
    /* 0xD84 */ f32 unk_D84;
    /* 0xD88 */ s8 unk_D88;
    /* 0xD89 */ u8 unk_D89;
    /* 0xD8A */ s16 unk_D8A;
    /* 0xD8C */ u8 unk_D8C;
    /* 0xD8D */ s8 unk_D8D;
    /* 0xD8E */ s16 unk_D8E;
    /* 0xD90 */ Player* unk_D90;
    /* 0xD94 */ EnTest3ActionFunc unk_D94;
    /* 0xD98 */ Vec3f unk_D98;
    /* 0xDA4 */ f32 unk_DA4;
    /* 0xDA8 */ s32 unk_DA8;
    /* 0xDAC */ s32 unk_DAC;
    /* 0xDB0 */ s32 unk_DB0;
    /* 0xDB4 */ s32 unk_DB4;
} EnTest3; // size = 0xDB8

extern const ActorInit En_Test3_InitVars;

#endif // Z_EN_TEST3_H
