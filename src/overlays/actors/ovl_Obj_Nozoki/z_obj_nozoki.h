#ifndef Z_OBJ_NOZOKI_H
#define Z_OBJ_NOZOKI_H

#include "global.h"

struct ObjNozoki;

typedef void (*ObjNozokiActionFunc)(struct ObjNozoki*, PlayState*);

#define OBJNOZOKI_GET_SWITCHFLAG1(thisx) ((thisx)->params & 0x7F)
#define OBJNOZOKI_GET_TYPE(thisx) (((thisx)->params >> 7) & 3)
#define OBJNOZOKI_GET_200(thisx) ((thisx)->params & 0x200)
#define OBJNOZOKI_GET_400(thisx) ((thisx)->params & 0x400)
#define OBJNOZOKI_GET_SWITCHFLAG2(thisx) (((thisx)->params >> 9) & 0x7F)

#define OBJNOZOKI_400 0x400

typedef enum {
    /* 0 */ SAKON_HIDEOUT_DOOR,
    /* 1 */ SAKON_HIDEOUT_MASK,
    /* 2 */ SAKON_HIDEOUT_BACK_SHUTTER,
    /* 3 */ SAKON_HIDEOUT_FRONT_SHUTTER,
    /* 4 */ SAKON_HIDEOUT_CURIOSITY
} ObjNozokiType;



typedef struct ObjNozoki {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ u8 type;
    /* 0x15D */ u8 sunMaskConveyorSpeed; // May be union
    /* 0x15E */ s8 timer;
    /* 0x15F */ s8 actorCsId;
    /* 0x160 */ ObjNozokiActionFunc actionFunc;
} ObjNozoki; // size = 0x164

#endif // Z_OBJ_NOZOKI_H
