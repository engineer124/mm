#ifndef Z_EN_BBA_01_H
#define Z_EN_BBA_01_H

#include "z_en_hy_code.h"

struct EnBba01;

typedef void (*EnBba01ActionFunc)(struct EnBba01*, PlayState*);

#define ENBBA01_GET_PATH(thisx) (((thisx)->params & 0x7E00) >> 9)

typedef struct EnBba01 {
    /* 0x000 */ EnHy enHy;
    /* 0x3EC */ UNK_TYPE1 unk_3EC[0x230];
} EnBba01; // size = 0x61C

#endif // Z_EN_BBA_01_H
