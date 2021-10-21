#ifndef Z_OBJ_SOUND_H
#define Z_OBJ_SOUND_H

#include "global.h"

struct ObjSound;

typedef struct ObjSound {
    /* 0x000 */ Actor actor;
    /* 0x144 */ u8 unk_144;
    /* 0x145 */ u8 unk_145;
    /* 0x146 */ u8 unk_146;
    /* 0x147 */ u8 unk_147;
} ObjSound; // size = 0x148

extern const ActorInit Obj_Sound_InitVars;

#endif // Z_OBJ_SOUND_H
