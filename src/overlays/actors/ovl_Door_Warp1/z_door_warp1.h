#ifndef Z_DOOR_WARP1_H
#define Z_DOOR_WARP1_H

#include "global.h"
#include "overlays/actors/ovl_Dm_Hina/z_dm_hina.h"

struct DoorWarp1;

typedef void (*DoorWarp1ActionFunc)(struct DoorWarp1*, PlayState*);

#define WARP_GET_TYPE(thisx) ((thisx)->params & 0xFF)
#define WARP_GET_TYPE00_1(thisx) (((thisx)->params >> 8) & 0xFF)
#define WARP_GET_TYPE00_2(thisx) ((thisx)->params & 0xFF00)
#define WARP_GET_TYPE00_3(thisx) ((thisx)->params >> 8)

typedef enum {
    /* 0 */ WARP_BLUE_MOON,
    /* 1 */ WARP_BLUE_BOSS,
    /* 2 */ WARP_ENTRANCE_WOODFALL_TEMPLE,
    /* 3 */ WARP_ENTRANCE_SNOWHEAD_TEMPLE,
    /* 4 */ WARP_ENTRANCE_GREAT_BAY_TEMPLE,
    /* 5 */ WARP_ENTRANCE_STONE_TOWER_TEMPLE,
    /* 6 */ WARP_ENTRANCE_ACTIVE,
} WarpType;

typedef struct DoorWarp1 {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ SkelAnime skelAnime;
    /* 0x1A0 */ DmHina* bossRemains;
    /* 0x1A4 */ f32 unk_1A4;
    /* 0x1A8 */ f32 unk_1A8;
    /* 0x1AC */ union {
                    f32 beamScaleY; // Entrance Type
                    f32 blueWarpScroll; // Blue Warp Type
                };
    /* 0x1B0 */ f32 blueWarpPortalAlpha2;
    /* 0x1B4 */ f32 blueWarpPortalAlpha1;
    /* 0x1B8 */ f32 unk_1B8;
    /* 0x1BC */ f32 unk_1BC;
    /* 0x1C0 */ f32 unk_1C0;
    /* 0x1C4 */ s16 scale;
    /* 0x1C6 */ s16 unk_1C6;
    /* 0x1C8 */ s16 unk_1C8;
    /* 0x1CA */ s16 unk_1CA;
    /* 0x1CC */ s16 unk_1CC;
    /* 0x1CE */ s16 unk_1CE;
    /* 0x1D0 */ u16 unk_1D0;
    /* 0x1D2 */ s8 unk_1D2;
    /* 0x1D3 */ u8 isBgActor;
    /* 0x1D4 */ u8 activationBeamOn;
    /* 0x1D8 */ DoorWarp1ActionFunc actionFunc;
    /* 0x1DC */ LightNode* upperLight;
    /* 0x1E0 */ LightInfo upperLightInfo;
    /* 0x1F0 */ LightNode* lowerLight;
    /* 0x1F4 */ LightInfo lowerLightInfo;
    /* 0x202 */ u8 unk_202;
    /* 0x203 */ u8 unk_203;
    /* 0x204 */ f32 unk_204;
    /* 0x208 */ u8 unk_208;
} DoorWarp1; // size = 0x20C

#endif // Z_DOOR_WARP1_H
