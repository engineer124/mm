#ifndef Z64MAP_H
#define Z64MAP_H
#include "ultra64.h"
#include "z64scene.h"

/*
Handles L-minimap and pause screen dungeon minimap system.

z_map_data.c stores MapSpriteInfo and helper functions to "simplify" accessing this data

Two separate Id systems are used to access data

mapId
0x000-0x004 fetches L-minimap data from gameplay_dangeon_keep
0x005-0x039 fetches dungeon minimap data from map_i_static
0x100-0x161 fetches L-minimap data from map_grand_static

mapCompactId
0x000-0x039 fetches dungeon minimap data from map_i_static
0x03A-0x09B fetches L-minimap data from map_grand_static
*/

#define FLOOR_INDEX_MAX 4

/* z_map_data */
#define MAPDATA_MAP_I_MAX 0x3A
#define MAPDATA_MAP_GRAND 0x100
#define MAPDATA_GET_MAP_GRAND_ID_FROM_COMPACT_ID(id) (id + (MAPDATA_MAP_GRAND - MAPDATA_MAP_I_MAX))
#define MAPDATA_GET_MAP_GRAND_ID_FROM_MAP_ID(mapId) (mapId - MAPDATA_MAP_GRAND)

#define MAPDATA_MID_GAMEPLAY_DANGEON_KEEP 0
#define MAPDATA_MID_MAP_GRAND_STATIC 1
#define MAPDATA_MID_MAP_I_STATIC 2

#define MAPDATA_CPID_MAP_GRAND_STATIC 0
#define MAPDATA_CPID_MAP_I_STATIC 1
#define MAPDATA_CPID_2 2

#define MAPDATA_DRAW_0 0
#define MAPDATA_DRAW_1 1
#define MAPDATA_DRAW_2 2
#define MAPDATA_DRAW_3 3

typedef struct {
    /* 0x0 */ u8 texWidth;
    /* 0x1 */ u8 texHeight;
    /* 0x2 */ s16 offsetX;
    /* 0x4 */ s16 offsetY;
    /* 0x6 */ u8 drawType;
    /* 0x7 */ u8 colorIndex;
    /* 0x8 */ s16 scale;
} MapSpriteInfo; // size = 0xA

typedef struct {
    /* 0x0 */ TexturePtr lmapTex; //minimap texture
    /* 0x4 */ u8 width;
    /* 0x5 */ u8 height;
    /* 0x6 */ u8 offsetX;
    /* 0x7 */ u8 offsetY;
    /* 0x8 */ u8 drawType;
    /* 0x9 */ u8 colorIndex;
    /* 0xA */ s16 scale;
} MapSpriteInfo2; // size = 0xC

/* z_map_disp */

typedef struct {
    /* 0x0 */ s16 unk0; //scene
    /* 0x2 */ s16 unk2;
} struct_801BEC5C; // size = 0x4

typedef struct {
    /* 0x00 */ s32 unk0[5];
} struct_801BEC70; // size = 0x14

typedef struct {
    /* 0x000 */ s32 rooms;
    /* 0x004 */ s32 unk4[32];
    /* 0x084 */ void* unk84[32];
    /* 0x104 */ void* unk104[32];
    /* 0x184 */ s32 unk184;
} struct_801F56B0; // size = 0x188

/* z_map_data */
TexturePtr MapData_GetMapTexGameplayDangeonKeep(s32);
s32 MapData_GetMapCompactId(s32);
s32 MapData_MID_GetType(s32);
s32 MapData_CPID_GetSizeOfMapTex(s32);
void MapData_GetDrawType(s32, s32*);
s32 MapData_GetMapColorIndex(s32);
s32 MapDisp_GetSizeOfMapITex(s32);
s32 MapData_GetMapIId(s32);
s32 MapData_GetSizeOfMapGrandTex(s32 mapId);
void MapData_GetMapTexDim(s32 mapId, s32* width, s32* height);
void MapData_GetMapTexOffset(s32 mapId, s32* offsetX, s32* offsetY);
void MapData_GetMapScale(s32 mapId, s32 *scale);
void MapData_CPID_GetTexDim(s32 mapCompactId, s32* width, s32* height);
void MapData_CPID_GetTexOffset(s32 mapCompactId, s32* offsetX, s32* offsetY);
s16 MapData_CPID_GetMapScale(s32 mapCompactId);

#endif
