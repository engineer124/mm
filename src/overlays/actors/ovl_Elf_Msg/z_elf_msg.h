#ifndef Z_ELF_MSG_H
#define Z_ELF_MSG_H

#include "global.h"

#define ELFMSG_GET_TEXT_ID_OFFSET(thisx) ((thisx)->params & 0xFF)
#define ELFMSG_GET_SWITCH_FLAG(thisx) (((thisx)->params & 0x7F00) >> 8)
#define ELFMSG_GET_8000(thisx) ((thisx)->params & 0x8000)

#define ELFMSG_GET_XZ_RANGE(thisx) (ABS_ALT((thisx)->home.rot.x))

struct ElfMsg;

typedef void (*ElfMsgActionFunc)(struct ElfMsg*, PlayState*);

typedef struct ElfMsg {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ElfMsgActionFunc actionFunc;
} ElfMsg; // size = 0x148

#endif // Z_ELF_MSG_H
