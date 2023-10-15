#include "global.h"
#include "gfx.h"
#include "overlays/actors/ovl_En_Door/z_en_door.h"
#include "interface/parameter_static/parameter_static.h"
#include "overlays/kaleido_scope/ovl_kaleido_scope/z_kaleido_scope.h"

void MapDisp_DestroyMapI(PlayState* play);
void MapDisp_InitMapI(PlayState* play);


/*
s32 D_801BEAD0[] = {0, 1, 2, 3};




*/

extern s32 D_801BEAD0[4]; // G_IM_SIZ
extern s32 D_801BEAE0[4]; // siz_LOAD_BLOCK
extern u32 D_801BEAF0[4]; // siz_INCR
extern s32 D_801BEB00[4]; // siz_SHIFT
extern u32 D_801BEB10[4]; // siz_BYTES
extern u32 D_801BEB20[4]; // siz_LINE_BYTES
extern TexturePtr D_801BEB38;

typedef struct {
    /* 0x00 */ MinimapList* minimapList;
    /* 0x04 */ s32 curRoom;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ TexturePtr lMapCurTex; // gameplay cur minimap room
    /* 0x14 */ s32 prevRoom;
    /* 0x18 */ TexturePtr lMapPrevTex;
    /* 0x1C */ s16 unk1C; //same as 0C
    /* 0x1E */ s16 unk1E; //same as 0E
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 swapAnimTimer;
    /* 0x28 */ void* texBuff0;
    /* 0x2C */ void* texBuff1;
    /* 0x30 */ s16 sceneMinX;
    /* 0x32 */ s16 sceneMinZ;
    /* 0x34 */ s16 sceneWidth;
    /* 0x36 */ s16 sceneHeight;
    /* 0x38 */ s16 sceneMidX;
    /* 0x3A */ s16 sceneMidZ;
    /* 0x3C */ s16* unk3C;
    /* 0x40 */ s16 unk40;
    /* 0x42 */ s16 unk42;
    /* 0x44 */ s16 unk44;
    /* 0x48 */ s16* unk48;
    /* 0x4C */ s16 unk4C;
    /* 0x50 */ s32 numChests;
    /* 0x54 */ MinimapChest* unk54;
    /* 0x58 */ s16 unk58;
    /* 0x5A */ s16 unk5A;
} MinimapContext; // size = 0x5C

extern MinimapContext sMinimapCtx;

extern s16 D_801BEBFA;         // sMinimapCtx.unk42
extern MinimapList D_801BEC14; // D_801F5130
extern s32 sSceneNumRooms;     // current scene's no. of rooms
extern s32 D_801BEC20;         // MinimapChest count
extern TransitionActorList D_801BEC24;
extern Color_RGBA8 D_801BEC2C[12]; // cat colors
extern struct_801BEC5C D_801BEC5C[5];
extern struct_801BEC70 D_801BEC70;
extern u16 D_801BEC84[0x10]; // palette 0
extern u16 D_801BECA4[0x10]; // palette 1
extern u16 D_801BECC4[0x10]; // palette 2
extern TexturePtr D_801BECE4[6];
extern TexturePtr D_801BED00[9];

typedef struct {
    /* 0x0 */ s16 sceneId;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ s32 unk8;
} struct_801BED24; // size = 0xC

extern struct_801BED24 D_801BED24[2];

extern MinimapEntry D_801F5130[0x50];
extern MinimapChest D_801F5270[32];
extern TransitionActorEntry D_801F53B0[48];
extern struct_801F56B0 D_801F56B0;

extern TexturePtr D_0C000000;

void MapDisp_GetMapITexture(void* dst, s32 mapId) {
    if (MapDisp_GetSizeOfMapITex(mapId) != 0) {
        CmpDma_LoadFile(SEGMENT_ROM_START(map_i_static), mapId, dst, MapDisp_GetSizeOfMapITex(mapId));
    }
}

void func_80102E90(PlayState* play, s16* arg1) {
    *arg1 = -1;
}

void func_80102EA4(PlayState* play, s16* arg1) {
}

void func_80102EB4(u32 param_1) {
    sMinimapCtx.unk20 |= param_1;
}

void func_80102ED0(u32 param_1) {
    sMinimapCtx.unk20 &= ~param_1;
}

s32 func_80102EF0(PlayState* play) {
    MinimapEntry* entry;
    s8 curRoom;

    if (Map_IsInBossArea(play) == true) {
        return true;
    }
    curRoom = play->roomCtx.curRoom.num;
    if (curRoom == -1) {
        return false;
    }
    entry = &sMinimapCtx.minimapList->entry[curRoom];
    if (entry->mapId == 0xFFFF) {
        return false;
    }
    if (MapData_GetMapIId(entry->mapId) == MAPDATA_MAP_I_MAX) {
        return false;
    }
    return true;
}

f32 func_80102F9C(f32 arg0) {
    s32 i;

    if ((sMinimapCtx.unk48[sMinimapCtx.unk40 - 1] - 80.0f) < arg0) {
        return sMinimapCtx.unk48[sMinimapCtx.unk40 - 1];
    }
    for (i = sMinimapCtx.unk40 - 2; i >= 0; i--) {
        if (((sMinimapCtx.unk48[i] - 80.0f) < arg0) && (arg0 < (sMinimapCtx.unk48[i + 1] + 80.0f))) {
            return sMinimapCtx.unk48[i];
        }
    }
    if (arg0 < (sMinimapCtx.unk48[0] + 80.0f)) {
        return sMinimapCtx.unk48[0];
    }
    return 0.0f;
}

void MapDisp_GetMapTexDim(MinimapEntry* minimapEntry, s32* width, s32* height) {
    MapData_GetMapTexDim(minimapEntry->mapId, width, height);
}

void MapDisp_GetMapScale(MinimapEntry* minimapEntry, s32* scale) {
    MapData_GetMapScale(minimapEntry->mapId, scale);
    if (*scale == 0) {
        *scale = 20;
    }
}

void MapDisp_GetMapPos(MinimapEntry* minimapEntry, s32* offsetX, s32* offsetY) {
    s32 width;
    s32 height;
    s32 temp;
    s32 temp2;

    if (minimapEntry->mapId == 0xFFFF) {
        *offsetX = 0;
        *offsetY = 0;
        return;
    }
    MapDisp_GetMapTexDim(minimapEntry, &width, &height);
    MapData_GetMapTexOffset(minimapEntry->mapId, offsetX, offsetY);
    if (minimapEntry->unk8 & 1) {
        temp = width / 2;
        *offsetX = (temp - *offsetX) + temp;
    }
    if (minimapEntry->unk8 & 2) {
        temp2 = height / 2;
        *offsetY = (temp2 - *offsetY) + temp2;
    }
}

void func_801031D0(PlayState* play, TexturePtr texture, s32 arg2, s32 arg3, s32 arg4, f32 arg5) {
    MinimapEntry* entry = &sMinimapCtx.minimapList->entry[arg4];
    s32 texWidth;
    s32 texHeight;
    s32 dtdy;
    s32 dsdx;
    s32 var_t0;
    s32 var_t1;
    s16 var_v1_4;
    s16 var_v1_5;
    Color_RGBA8 spAC_color;
    s32 spA8_drawType;

    if ((entry->mapId == 0xFFFF) || (texture == NULL)) {
        return;
    }

    MapDisp_GetMapTexDim(entry, &texWidth, &texHeight);

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL39_Overlay(play->state.gfxCtx);
    MapData_GetMapColor(MapData_GetMapColorIndex(entry->mapId), &spAC_color);
    gDPSetPrimColor(OVERLAY_DISP++, 0, 0, spAC_color.r, spAC_color.g, spAC_color.b,
                    (s32)(play->interfaceCtx.minimapAlpha * arg5 * spAC_color.a / 255.0f));
    MapData_GetDrawType(entry->mapId, &spA8_drawType);

    switch (spA8_drawType) {
        case MAPDATA_DRAW_1:
            gDPSetCombineMode(OVERLAY_DISP++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
            gDPLoadTextureBlock_4b(OVERLAY_DISP++, texture, G_IM_FMT_IA, texWidth, texHeight, 0,
                                   G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                   G_TX_NOLOD, G_TX_NOLOD);
            break;

        case MAPDATA_DRAW_3:
            gDPSetCombineLERP(OVERLAY_DISP++, 0, 0, 0, PRIMITIVE, 0, 0, 0, TEXEL0, 0, 0, 0, PRIMITIVE, 0, 0, 0, TEXEL0);
            gDPLoadTextureBlock_4b(OVERLAY_DISP++, texture, G_IM_FMT_I, texWidth, texHeight, 0,
                                   G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                   G_TX_NOLOD, G_TX_NOLOD);
            break;

        default:
        case MAPDATA_DRAW_0:
            gDPSetCombineMode(OVERLAY_DISP++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
            gDPLoadTextureBlock_4b(OVERLAY_DISP++, texture, G_IM_FMT_I, texWidth, texHeight, 0,
                                   G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                   G_TX_NOLOD, G_TX_NOLOD);
            break;
    }

    if (entry->unk8 & 1) {
        var_t1 = (texWidth - 1) << 5;
    } else {
        var_t1 = 0;
    }

    if (entry->unk8 & 2) {
        var_t0 = 0;
    } else {
        var_t0 = (texHeight - 1) << 5;
    }

    var_v1_4 = ((entry->unk8 & 1) ? -1 : 1) * (1 << 10);
    var_v1_5 = ((entry->unk8 & 2) ? 1 : -1) * (1 << 10);

    if (entry->unk8 & 1) {
        dsdx = var_v1_4 & 0xFFFF;
    } else {
        dsdx = var_v1_4;
    }
    if (entry->unk8 & 2) {
        dtdy = var_v1_5;
    } else {
        dtdy = var_v1_5 & 0xFFFF;
    }

    gSPTextureRectangle(OVERLAY_DISP++, arg2 << 2, arg3 << 2, (texWidth + arg2) << 2, (arg3 + texHeight) << 2,
                        G_TX_RENDERTILE, var_t1, var_t0, dsdx, dtdy);

    CLOSE_DISPS(play->state.gfxCtx);
}

// Tests if the map data should be rotated 180 degrees
// SCENE_35TAKI is the only scene with data flipped in this manner.
s32 MapDisp_IsDataRotated(PlayState* play) {
    if (play->sceneId == SCENE_35TAKI) {
        return true;
    }
    return false;
}

s32 func_80103A10(PlayState* play) {
    if ((gSaveContext.save.entrance == ENTRANCE(ROMANI_RANCH, 0)) && (Cutscene_GetSceneLayer(play) != 0)) {
        return false;
    }
    return true;
}

#define gDPLoadTextureBlock_TEST(pkt, timg, fmt, siz_LOAD_BLOCK, siz_INCR, siz_SHIFT, siz_BYTES, siz_LINE_BYTES, siz, \
                                 width, height, pal, cms, cmt, masks, maskt, shifts, shiftt)                          \
    _DW({                                                                                                             \
        gDPSetTextureImage(pkt, fmt, siz_LOAD_BLOCK, 1, timg);                                                        \
        gDPSetTile(pkt, fmt, siz_LOAD_BLOCK, 0, 0, G_TX_LOADTILE, 0, cmt, maskt, shiftt, cms, masks, shifts);         \
        gDPLoadSync(pkt);                                                                                             \
        gDPLoadBlock(pkt, G_TX_LOADTILE, 0, 0, (((width) * (height) + siz_INCR) >> siz_SHIFT) - 1,                    \
                     CALC_DXT(width, siz_BYTES));                                                                     \
        gDPPipeSync(pkt);                                                                                             \
        gDPSetTile(pkt, fmt, siz, (((width)*siz_LINE_BYTES) + 7) >> 3, 0, G_TX_RENDERTILE, pal, cmt, maskt, shiftt,   \
                   cms, masks, shifts);                                                                               \
        gDPSetTileSize(pkt, G_TX_RENDERTILE, 0, 0, ((width)-1) << G_TEXTURE_IMAGE_FRAC,                               \
                       ((height)-1) << G_TEXTURE_IMAGE_FRAC);                                                         \
    })

void func_80103A58(PlayState* play, Actor* actor) {
    MinimapEntry* spDC;
    s32 spD8;
    s32 spD4;
    s32 spD0;
    s32 spCC;
    s32 spC8;
    s32 spC4;
    f32 spC0;
    f32 unused1;
    f32 unused2;
    Player* player = GET_PLAYER(play);
    s32 scale;

    // inferred from `func_801045AC`
    unused1 = fabsf(player->actor.world.pos.y - actor->world.pos.y);
    unused2 = 1.0f - (1 / 350.0f) * unused1;

    if (unused2 < 0.0f) {
        unused2 = 0.0f;
    }

    spDC = &sMinimapCtx.minimapList->entry[sMinimapCtx.curRoom];
    if (spDC->mapId == 0xFFFF) {
        return;
    }

    MapDisp_GetMapPos(spDC, &spD0, &spCC);
    MapDisp_GetMapTexDim(spDC, &spC8, &spC4);

    scale = sMinimapCtx.minimapList->scale;
    if (sMinimapCtx.minimapList->scale == 0) {
        scale = 20;
    } else if (sMinimapCtx.minimapList->scale == -1) {
        s32 scaleTemp;

        MapDisp_GetMapScale(spDC, &scaleTemp);
        scale = scaleTemp;
    }

    spC0 = 1.0f / scale;
    if (!MapDisp_IsDataRotated(play)) {
        spD8 = (s32)((actor->world.pos.x - spDC->unk2) * spC0) + sMinimapCtx.unk8 + sMinimapCtx.unkC -
               sMinimapCtx.unk8 + spD0;
        spD4 = (s32)((actor->world.pos.z - spDC->unk6) * spC0) + sMinimapCtx.unkA + sMinimapCtx.unkE -
               sMinimapCtx.unkA + spCC;
    } else {
        spD8 = -(s32)((actor->world.pos.x - spDC->unk2) * spC0) + sMinimapCtx.unk8 + sMinimapCtx.unkC -
               sMinimapCtx.unk8 + spD0;
        spD4 = -(s32)((actor->world.pos.z - spDC->unk6) * spC0) + sMinimapCtx.unkA + sMinimapCtx.unkE -
               sMinimapCtx.unkA + spCC;
    }

    if ((spD8 > 0) && (spD8 < 0x3FF) && (spD4 > 0) && (spD4 < 0x3FF)) {
        OPEN_DISPS(play->state.gfxCtx);

        if ((actor->category == ACTORCAT_PLAYER) && (actor->flags & ACTOR_FLAG_80000000)) {
            s16 spA2;

            Gfx_SetupDL42_Overlay(play->state.gfxCtx);
            gSPMatrix(OVERLAY_DISP++, &gIdentityMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gDPSetCombineLERP(OVERLAY_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0,
                              PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);
            gDPSetEnvColor(OVERLAY_DISP++, 0, 0, 0, play->interfaceCtx.minimapAlpha);
            gDPSetCombineMode(OVERLAY_DISP++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
            gDPSetRenderMode(OVERLAY_DISP++, G_RM_AA_DEC_LINE, G_RM_NOOP2);

            Matrix_Translate(spD8 - 160.0f, 120.0f - spD4, 0.0f, MTXMODE_NEW);
            Matrix_RotateXFApply(-1.6f);
            spA2 = (s32)(0x7FFF - actor->focus.rot.y) / 1024;
            if (MapDisp_IsDataRotated(play)) {
                spA2 += 0x7FFF;
            }
            Matrix_RotateYF(spA2 / 10.0f, MTXMODE_APPLY);
            Matrix_Scale(0.4f, 0.4f, 0.4f, MTXMODE_APPLY);
            gSPMatrix(OVERLAY_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 200, 255, 0, play->interfaceCtx.minimapAlpha);
            gSPDisplayList(OVERLAY_DISP++, gameplay_keep_DL_01ED00);
        } else if ((actor->id == ACTOR_EN_BOX) && !Flags_GetTreasure(play, actor->params & 0x1F) &&
                   (func_80102F9C(player->actor.world.pos.y) == func_80102F9C(actor->world.pos.y))) {
            Gfx_SetupDL39_Overlay(play->state.gfxCtx);
            gDPPipeSync(OVERLAY_DISP++);
            gDPSetTextureLUT(OVERLAY_DISP++, G_TT_NONE);
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, play->interfaceCtx.minimapAlpha);
            gDPSetEnvColor(OVERLAY_DISP++, 0, 0, 0, play->interfaceCtx.minimapAlpha);
            gDPPipeSync(OVERLAY_DISP++);

            gDPLoadTextureBlock_TEST(OVERLAY_DISP++, gMapChestIconTex, G_IM_FMT_RGBA, D_801BEAE0[2], D_801BEAF0[2],
                                     D_801BEB00[2], D_801BEB10[2], D_801BEB20[2], D_801BEAD0[2], 8, 8, 0,
                                     G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                     G_TX_NOLOD, G_TX_NOLOD);

            gSPTextureRectangle(OVERLAY_DISP++, (spD8 - 4) << 2, (spD4 - 4) << 2, (spD8 + 4) << 2, (spD4 + 4) << 2,
                                G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);
        } else {
            Gfx_SetupDL39_Overlay(play->state.gfxCtx);
            gDPSetCombineMode(OVERLAY_DISP++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
            if (actor->flags & ACTOR_FLAG_80000000) {
                gDPSetPrimColor(OVERLAY_DISP++, 0, 0, D_801BEC2C[actor->category].r, D_801BEC2C[actor->category].g,
                                D_801BEC2C[actor->category].b, play->interfaceCtx.minimapAlpha);
                gSPTextureRectangle(OVERLAY_DISP++, (spD8 - 1) << 2, (spD4 - 1) << 2, (spD8 + 1) << 2, (spD4 + 1) << 2,
                                    G_TX_RENDERTILE, 0, 0, 0x0001, 0x0001);
            }
        }
        CLOSE_DISPS(play->state.gfxCtx);
    }
}

void func_8010439C(PlayState* play) {
    ActorContext* actorCtx;
    s32 i;

    if (play->roomCtx.curRoom.num != -1) {
        OPEN_DISPS(play->state.gfxCtx);

        gDPLoadTextureBlock_4b(OVERLAY_DISP++, &D_801BEB38, G_IM_FMT_I, 16, 16, 0, G_TX_NOMIRROR | G_TX_WRAP,
                               G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

        actorCtx = &play->actorCtx;
        for (i = 0; i < ACTORCAT_MAX; i++) {
            Actor* actor = actorCtx->actorLists[i].first;

            if (actor != NULL) {
                do {
                    if ((actor->update != NULL) && (actor->init == NULL) &&
                        Object_IsLoaded(&play->objectCtx, actor->objectSlot) &&
                        ((actor->id == ACTOR_EN_BOX) || (i == ACTORCAT_PLAYER) ||
                         (actor->flags & ACTOR_FLAG_80000000)) &&
                        ((sMinimapCtx.curRoom == actor->room) || (actor->room == -1))) {
                        func_80103A58(play, actor);
                    }
                    actor = actor->next;
                } while (actor != NULL);
            }
        }

        CLOSE_DISPS(play->state.gfxCtx);
    }
}

void func_801045AC(PlayState* play, Actor* actor) {
    MinimapEntry* sp7C;
    s32 sp78;
    s32 sp74;
    s32 sp70;
    s32 sp6C;
    s32 sp68;
    s32 sp64;
    Player* player = GET_PLAYER(play);
    f32 sp5C;
    f32 temp_fv1 = fabsf(player->actor.world.pos.y - actor->world.pos.y);
    s32 scale;
    f32 sp50 = 1.0f - (1 / 350.0f) * temp_fv1;

    if (sp50 < 0.0f) {
        sp50 = 0.0f;
    }

    sp7C = &sMinimapCtx.minimapList->entry[sMinimapCtx.curRoom];
    if (sp7C->mapId == 0xFFFF) {
        return;
    }

    MapDisp_GetMapPos(sp7C, &sp70, &sp6C);
    MapDisp_GetMapTexDim(sp7C, &sp68, &sp64);

    scale = sMinimapCtx.minimapList->scale;
    if (sMinimapCtx.minimapList->scale == 0) {
        scale = 20;
    } else if (sMinimapCtx.minimapList->scale == -1) {
        s32 sp4C;

        MapDisp_GetMapScale(sp7C, &sp4C);
        scale = sp4C;
    }
    sp5C = 1.0f / scale;
    if (!MapDisp_IsDataRotated(play)) {
        sp78 = (s32)((actor->world.pos.x - sp7C->unk2) * sp5C) + sMinimapCtx.unk8 + sMinimapCtx.unkC -
               sMinimapCtx.unk8 + sp70;
        sp74 = (s32)((actor->world.pos.z - sp7C->unk6) * sp5C) + sMinimapCtx.unkA + sMinimapCtx.unkE -
               sMinimapCtx.unkA + sp6C;
    } else {
        sp78 = -(s32)((actor->world.pos.x - sp7C->unk2) * sp5C) + sMinimapCtx.unk8 + sMinimapCtx.unkC -
               sMinimapCtx.unk8 + sp70;
        sp74 = -(s32)((actor->world.pos.z - sp7C->unk6) * sp5C) + sMinimapCtx.unkA + sMinimapCtx.unkE -
               sMinimapCtx.unkA + sp6C;
    }
    if ((sp78 > 0) && (sp78 < 0x3FF) && (sp74 > 0) && (sp74 < 0x3FF)) {
        OPEN_DISPS(play->state.gfxCtx);

        Gfx_SetupDL39_Overlay(play->state.gfxCtx);
        gDPSetCombineMode(OVERLAY_DISP++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);

        if ((actor->category == ACTORCAT_DOOR) && func_80103A10(play)) {
            s32 pad;

            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, D_801BEC2C[actor->category].r, D_801BEC2C[actor->category].g,
                            D_801BEC2C[actor->category].b,
                            (s32)((D_801BEC2C[actor->category].a * (1.0f - sMinimapCtx.swapAnimTimer * 0.05f) * sp50 *
                                   play->interfaceCtx.minimapAlpha) /
                                  255.0f));

            scale = sMinimapCtx.minimapList->scale;
            if (sMinimapCtx.minimapList->scale == 0) {
                scale = 20;
            } else if (sMinimapCtx.minimapList->scale == -1) {
                s32 sp34;

                MapDisp_GetMapScale(sp7C, &sp34);
                scale = sp34;
            }
            if (scale <= 50) {
                gSPTextureRectangle(OVERLAY_DISP++, (sp78 - 2) << 2, (sp74 - 2) << 2, (sp78 + 2) << 2, (sp74 + 2) << 2,
                                    G_TX_RENDERTILE, 0, 0, 0x0001, 0x0001);
            } else {
                gSPTextureRectangle(OVERLAY_DISP++, (sp78 - 1) << 2, (sp74 - 1) << 2, (sp78 + 1) << 2, (sp74 + 1) << 2,
                                    G_TX_RENDERTILE, 0, 0, 0x0001, 0x0001);
            }
        }

        CLOSE_DISPS(play->state.gfxCtx);
    }
}

void func_80104AE8(PlayState* play) {
    s32 i;
    Actor* actor;

    if (play->roomCtx.curRoom.num != -1) {
        OPEN_DISPS(play->state.gfxCtx);

        gDPLoadTextureBlock_4b(OVERLAY_DISP++, &D_801BEB38, G_IM_FMT_I, 16, 16, 0, G_TX_NOMIRROR | G_TX_WRAP,
                               G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

        actor = play->actorCtx.actorLists[ACTORCAT_DOOR].first;
        if (actor != NULL) {
            do {
                if ((actor->update != NULL) && (actor->init == NULL) &&
                    Object_IsLoaded(&play->objectCtx, actor->objectSlot) &&
                    ((sMinimapCtx.curRoom == actor->room) || (actor->room == -1))) {
                    func_801045AC(play, actor);
                }
                actor = actor->next;
            } while (actor != NULL);
        }

        CLOSE_DISPS(play->state.gfxCtx);
    }
}

void func_80104C80(PlayState* play) {
    ObjectContext* objectCtx = &play->objectCtx;
    s32 objectSlot = Object_GetSlot(objectCtx, GAMEPLAY_DANGEON_KEEP);

    if (objectSlot <= OBJECT_SLOT_NONE) {
        sMinimapCtx.unk20 |= 1;
        return;
    }

    do {
    } while (!Object_IsLoaded(objectCtx, objectSlot));

    //! FAKE: https://decomp.me/scratch/kvGBZ
    if (objectCtx) {}
}

void MapDisp_Init(PlayState* play) {
    s32 i;

    sMinimapCtx.minimapList = NULL;
    sMinimapCtx.curRoom = -1;
    sMinimapCtx.unk8 = 0xD2;
    sMinimapCtx.unkA = 0x8C;
    sMinimapCtx.unkC = 0xD2;
    sMinimapCtx.unkE = 0x8C;
    sMinimapCtx.lMapCurTex = NULL;
    sMinimapCtx.prevRoom = -1;
    sMinimapCtx.lMapPrevTex = NULL;
    sMinimapCtx.unk1C = 0;
    sMinimapCtx.unk1E = 0;
    sMinimapCtx.unk20 = 0;
    sMinimapCtx.swapAnimTimer = 0;

    if (!Map_IsInBossArea(play)) {
        sSceneNumRooms = play->numRooms;
    }
    sMinimapCtx.texBuff0 = THA_AllocTailAlign16(&play->state.tha, 0x4000);
    sMinimapCtx.texBuff1 = THA_AllocTailAlign16(&play->state.tha, 0x4000);
    func_80104C80(play);

    if (!Map_IsInBossArea(play)) {
        sMinimapCtx.sceneMinX = 0;
        sMinimapCtx.sceneMinZ = 0;
        sMinimapCtx.sceneWidth = 100;
        sMinimapCtx.sceneHeight = 100;
        sMinimapCtx.sceneMidX = TRUNCF_BINANG((f32)sMinimapCtx.sceneMinX + ((f32)sMinimapCtx.sceneWidth * 0.5f));
        sMinimapCtx.sceneMidZ = TRUNCF_BINANG((f32)sMinimapCtx.sceneMinZ + ((f32)sMinimapCtx.sceneHeight * 0.5f));
    }
    sMinimapCtx.unk3C = THA_AllocTailAlign16(&play->state.tha, sSceneNumRooms * sizeof(s16));

    for (i = 0; i < sSceneNumRooms; i++) {
        func_80102E90(play, &sMinimapCtx.unk3C[i]);
    }
    sMinimapCtx.unk48 = THA_AllocTailAlign16(&play->state.tha, 32 * sizeof(s16));

    for (i = 0; i < 32; i++) {
        sMinimapCtx.unk48[i] = -0x7FFF;
    }
    MapDisp_InitMapI(play);
    sMinimapCtx.unk58 = 0;
    sMinimapCtx.unk5A = 0;
    if (Map_IsInBossArea(play)) {
        func_8010549C(play, NULL);
        func_8010565C(play, 0, NULL);
    }
}

void func_80104F34(PlayState* play) {
    s32 i1;
    s32 i2;
    s32 i3;
    s32 i4;

    for (i1 = 0; i1 < 32; i1++) {
        sMinimapCtx.unk48[i1] = -0x7FFF;
    }
    for (i2 = 0; i2 < sSceneNumRooms; i2++) {
        MinimapEntry* mapEntry1 = &sMinimapCtx.minimapList->entry[i2];

        if (mapEntry1->mapId == 0xFFFF) {
            continue;
        }
        for (i1 = 0; i1 < 32; i1++) {
            if (sMinimapCtx.unk48[i1] == -0x7FFF) {
                sMinimapCtx.unk48[i1] = mapEntry1->unk4;
                break;
            } else if (fabsf((f32)sMinimapCtx.unk48[i1] - (f32)mapEntry1->unk4) < 5.0f) {
                break;
            }
        }
    }
    for (i2 = 0; i2 < sSceneNumRooms; i2++) {
        if (sMinimapCtx.unk48[i2] == -0x7FFF) {
            break;
        }
        for (i3 = i2 + 1; i3 < sSceneNumRooms; i3++) {
            if (sMinimapCtx.unk48[i3] == -0x7FFF) {
                break;
            }
            if (sMinimapCtx.unk48[i3] < sMinimapCtx.unk48[i2]) {
                s16 temp_t3 = sMinimapCtx.unk48[i2];

                sMinimapCtx.unk48[i2] = sMinimapCtx.unk48[i3];
                sMinimapCtx.unk48[i3] = temp_t3;
            }
        }
    }
    for (i2 = 0; i2 < sSceneNumRooms; i2++) {
        MinimapEntry* mapEntry2 = &sMinimapCtx.minimapList->entry[i2];

        sMinimapCtx.unk3C[i2] = -1;

        for (i4 = 0; i4 < sSceneNumRooms; i4++) {
            if (sMinimapCtx.unk48[i4] != -0x7FFF) {
                if (fabsf((f32)sMinimapCtx.unk48[i4] - (f32)mapEntry2->unk4) < 5.0f) {
                    sMinimapCtx.unk3C[i2] = i4;
                    break;
                }
            }
        }
    }
    sMinimapCtx.unk40 = 0;
    for (i2 = 0; i2 < sSceneNumRooms; i2++) {
        if (sMinimapCtx.unk48[i2] != -0x7FFF) {
            sMinimapCtx.unk40++;
        }
    }
    sMinimapCtx.unk44 = 0;
    for (i2 = 0; i2 < 5; i2++) {
        if (play->sceneId == D_801BEC5C[i2].unk0) {
            sMinimapCtx.unk44 = D_801BEC5C[i2].unk2;
        }
    }
}

s32 func_80105294(void) {
    struct_801BEC70 sp4 = D_801BEC70;

    if ((sMinimapCtx.minimapList == NULL) || (sMinimapCtx.unk58 < 0) || (sMinimapCtx.unk58 >= 5) ||
        (sSceneNumRooms == 0)) {
        return 0x7B;
    }
    return sp4.unk0[sMinimapCtx.unk58];
}

s16 func_80105318(void) {
    return sMinimapCtx.unk58;
}

// TransitionActor params test
s32 func_80105328(s32 params) {
    if (ENDOOR_PARAMS_GET_TYPE((u16)params) == ENDOOR_TYPE_5) {
        return true;
    }
    return false;
}

void func_8010534C(PlayState* play) {
    TransitionActorList* transitionActors = &D_801BEC24;
    s32 var_v0;
    s32 i;

    for (i = 0; i < transitionActors->count; i++) {
        if (func_80105328(D_801F53B0[i].params)) {
            if (ABS_ALT(D_801F53B0[i].id) != 0x18) {
                for (var_v0 = 0; var_v0 < sMinimapCtx.unk40; var_v0++) {
                    //! FAKE: needed for matching
                    s32 temp = (sMinimapCtx.unk48[var_v0] - 5);

                    if (((var_v0 == sMinimapCtx.unk40 - 1) &&
                         (D_801F53B0[i].pos.y >= (sMinimapCtx.unk48[var_v0] - 5))) ||
                        ((var_v0 != sMinimapCtx.unk40 - 1) &&
                         (D_801F53B0[i].pos.y >= (sMinimapCtx.unk48[var_v0] - 5)) &&
                         (D_801F53B0[i].pos.y < (sMinimapCtx.unk48[var_v0 + 1] - 5)))) {
                        sMinimapCtx.unk58 = var_v0;
                        return;
                    }
                }
            }
        }
    }
    sMinimapCtx.unk58 = 0;
}

void func_8010549C(PlayState* play, void* segmentAddress) {
    MinimapEntry* var_v1;
    MinimapList* temp_v0;
    s32 i;

    if (!Map_IsInBossArea(play)) {
        sSceneNumRooms = play->numRooms;
        temp_v0 = Lib_SegmentedToVirtual(segmentAddress);
        D_801BEC14 = *temp_v0;
        var_v1 = Lib_SegmentedToVirtual(temp_v0->entry);

        for (i = 0; i < sSceneNumRooms; i++) {
            D_801F5130[i] = *var_v1++;
        }

        D_801BEC14.entry = D_801F5130;
        if (play->colCtx.colHeader != NULL) {
            sMinimapCtx.sceneMinX = play->colCtx.colHeader->minBounds.x;
            sMinimapCtx.sceneMinZ = play->colCtx.colHeader->minBounds.z;
            sMinimapCtx.sceneWidth = play->colCtx.colHeader->maxBounds.x - play->colCtx.colHeader->minBounds.x;
            sMinimapCtx.sceneHeight = play->colCtx.colHeader->maxBounds.z - play->colCtx.colHeader->minBounds.z;
            sMinimapCtx.sceneMidX = sMinimapCtx.sceneMinX + (sMinimapCtx.sceneWidth * 0.5f);
            sMinimapCtx.sceneMidZ = sMinimapCtx.sceneMinZ + (sMinimapCtx.sceneHeight * 0.5f);
        }
    }
    sMinimapCtx.minimapList = &D_801BEC14;
    func_80104F34(play);
    func_8010534C(play);
}

void func_8010565C(PlayState* play, s32 num, void* segmentAddress) {
    MinimapChest* var_v1;
    s32 var_a3;

    if (!Map_IsInBossArea(play)) {
        var_v1 = Lib_SegmentedToVirtual(segmentAddress);
        for (var_a3 = 0; var_a3 < num; var_v1++, var_a3++) {
            D_801F5270[var_a3] = *var_v1;
        }
        D_801BEC20 = num;
    }
    sMinimapCtx.unk54 = D_801F5270;
    sMinimapCtx.numChests = D_801BEC20;
}

void func_80105818(PlayState* play, s32 num, TransitionActorEntry* transitionActorList) {
    s32 i;

    if (!Map_IsInBossArea(play)) {
        D_801BEC24.count = num;
        for (i = 0; i < num; i++) {
            D_801F53B0[i] = transitionActorList[i];
        }
        D_801BEC24.list = D_801F53B0;
    }
}

void MapDisp_Destroy(PlayState* play) {
    s32 i;

    sMinimapCtx.minimapList = NULL;
    sMinimapCtx.curRoom = -1;
    sMinimapCtx.unkC = 0xD2;
    sMinimapCtx.unkE = 0x8C;
    sMinimapCtx.lMapCurTex = NULL;
    sMinimapCtx.prevRoom = -1;
    sMinimapCtx.lMapPrevTex = NULL;
    sMinimapCtx.unk1C = 0;
    sMinimapCtx.unk1E = 0;
    sMinimapCtx.unk20 = 0;
    sMinimapCtx.swapAnimTimer = 0;
    sMinimapCtx.texBuff0 = NULL;
    sMinimapCtx.texBuff1 = NULL;

    for (i = 0; i < sSceneNumRooms; i++) {
        func_80102EA4(play, &sMinimapCtx.unk3C[i]);
    }

    sMinimapCtx.unk3C = NULL;
    sMinimapCtx.unk40 = 0;
    sMinimapCtx.unk42 = 0;
    sMinimapCtx.unk44 = 0;
    sMinimapCtx.unk4C = 0;
    sMinimapCtx.unk48 = NULL;
    sMinimapCtx.numChests = 0;
    sMinimapCtx.unk54 = NULL;
    MapDisp_DestroyMapI(play);
    sMinimapCtx.unk5A = 0;
}

void func_80105B34(PlayState* play) {
    s16 temp_a0;
    s16 temp_a0_2;
    s16 temp_v1;
    s16 temp_v1_2;

    if ((sMinimapCtx.minimapList != NULL) && (sSceneNumRooms != 0)) {
        D_801BEBFA = 8 - play->pauseCtx.unk_256;
        if (sMinimapCtx.prevRoom != -1) {
            if (sMinimapCtx.swapAnimTimer > 0) {
                temp_v1 = sMinimapCtx.unk8;
                temp_a0 = sMinimapCtx.unkC;
                if (temp_v1 != temp_a0) {
                    sMinimapCtx.unkC =
                        (s16)(s32)(((f32)(temp_v1 - temp_a0) / (f32)sMinimapCtx.swapAnimTimer) + (f32)temp_a0);
                }
                temp_v1_2 = sMinimapCtx.unkA;
                temp_a0_2 = sMinimapCtx.unkE;
                if (temp_v1_2 != temp_a0_2) {
                    sMinimapCtx.unkE =
                        (s16)(s32)(((f32)(temp_v1_2 - temp_a0_2) / (f32)sMinimapCtx.swapAnimTimer) + (f32)temp_a0_2);
                }
                sMinimapCtx.swapAnimTimer--;
            } else {
                sMinimapCtx.prevRoom = -1;
                sMinimapCtx.swapAnimTimer = 0;
                sMinimapCtx.unkC = sMinimapCtx.unk8;
                sMinimapCtx.unkE = sMinimapCtx.unkA;
            }
        } else {
            sMinimapCtx.swapAnimTimer = 0;
        }
    }
}

void MapDisp_SwapRooms(s16 nextRoom) {
    MinimapEntry* nextMinimapEntry;
    MinimapEntry* prevMinimapEntry;
    s32 sp54;
    s32 sp50;
    s32 sp4C;
    s32 sp48;
    s32 sp44;
    s32 sp40;

    if ((sMinimapCtx.minimapList != NULL) && (sSceneNumRooms != 0) && (nextRoom != -1)) {
        nextMinimapEntry = &sMinimapCtx.minimapList->entry[nextRoom];
        if ((nextMinimapEntry->mapId < 5) ||
            ((nextMinimapEntry->mapId >= 0x100) && (nextMinimapEntry->mapId < 0x162)) ||
            nextMinimapEntry->mapId == 0xFFFF) {

            sMinimapCtx.prevRoom = sMinimapCtx.curRoom;
            sMinimapCtx.curRoom = nextRoom;
            sMinimapCtx.swapAnimTimer = 20;

            sMinimapCtx.lMapPrevTex = sMinimapCtx.lMapCurTex;
            sp54 = sMinimapCtx.unk8;
            sp50 = sMinimapCtx.unkA;

            nextMinimapEntry = &sMinimapCtx.minimapList->entry[sMinimapCtx.curRoom];

            if (nextMinimapEntry->mapId == 0xFFFF) {
                sMinimapCtx.unk1E = 0;
                sMinimapCtx.unk8 = 0xD2;
                sMinimapCtx.unkA = 0x8C;
                sMinimapCtx.unkC = 0xD2;
                sMinimapCtx.unkE = 0x8C;
                sMinimapCtx.lMapCurTex = NULL;
                sMinimapCtx.unk1C = sMinimapCtx.unk1E;
                return;
            }
            MapDisp_GetMapPos(nextMinimapEntry, &sp44, &sp40);
            MapDisp_GetMapTexDim(nextMinimapEntry, &sp4C, &sp48);
            sMinimapCtx.unk8 = 0x127 - sp4C;
            sMinimapCtx.unkA = 0xDC - sp48;
            if (sMinimapCtx.prevRoom != -1) {
                prevMinimapEntry = &sMinimapCtx.minimapList->entry[sMinimapCtx.prevRoom];
                if (prevMinimapEntry->mapId == 0xFFFF) {
                    sMinimapCtx.unk1E = 0;
                    sMinimapCtx.lMapCurTex = NULL;
                    sMinimapCtx.unk1C = sMinimapCtx.unk1E;
                    sMinimapCtx.unkC = sMinimapCtx.unk8;
                    sMinimapCtx.unkE = sMinimapCtx.unkA;
                    return;
                } else {
                    s32 sp3C;
                    s32 sp38;
                    s32 scale;
                    s32 pad;

                    MapDisp_GetMapPos(prevMinimapEntry, &sp3C, &sp38);
                    scale = sMinimapCtx.minimapList->scale;
                    if (sMinimapCtx.minimapList->scale == 0) {
                        scale = 20;
                    } else if (sMinimapCtx.minimapList->scale == -1) {
                        s32 scaleTemp;

                        MapDisp_GetMapScale(nextMinimapEntry, &scaleTemp);
                        scale = scaleTemp;
                    }
                    sMinimapCtx.unk1C =
                        (s16)(s32)(((f32)sp44 +
                                    (((f32)prevMinimapEntry->unk2 - (f32)nextMinimapEntry->unk2) * (1.0f / scale))) -
                                   (f32)sp3C);
                    sMinimapCtx.unk1E =
                        (s16)(s32)(((f32)sp40 +
                                    (((f32)prevMinimapEntry->unk6 - (f32)nextMinimapEntry->unk6) * (1.0f / scale))) -
                                   (f32)sp38);
                    sMinimapCtx.unkC = sp54 - sMinimapCtx.unk1C;
                    sMinimapCtx.unkE = sp50 - sMinimapCtx.unk1E;
                }
            } else {
                sMinimapCtx.unk1C = sMinimapCtx.unk1E = 0;
                sMinimapCtx.unkC = sMinimapCtx.unk8;
                sMinimapCtx.unkE = sMinimapCtx.unkA;
            }
            sMinimapCtx.lMapCurTex = NULL;

            switch (MapData_MID_GetType(nextMinimapEntry->mapId)) {
                case MAPDATA_MID_GAMEPLAY_DANGEON_KEEP:
                    sMinimapCtx.lMapCurTex = MapData_GetMapTexGameplayDangeonKeep(nextMinimapEntry->mapId);
                    return;

                case MAPDATA_MID_MAP_GRAND_STATIC:
                    if (sMinimapCtx.lMapPrevTex == sMinimapCtx.texBuff0) {
                        sMinimapCtx.lMapCurTex = sMinimapCtx.texBuff1;
                    } else {
                        sMinimapCtx.lMapCurTex = sMinimapCtx.texBuff0;
                    }
                    if (MapData_GetSizeOfMapGrandTex(nextMinimapEntry->mapId) != 0) {
                        CmpDma_LoadFile(SEGMENT_ROM_START(map_grand_static), nextMinimapEntry->mapId - 0x100,
                                        sMinimapCtx.lMapCurTex, MapData_GetSizeOfMapGrandTex(nextMinimapEntry->mapId));
                    }
                    break;

                default:
                    break;
            }
        }
    }
}

void func_80105FE0(PlayState* play, s32 x, s32 z, s32 rot) {
    MinimapEntry* sp6C;
    s32 sp68;
    s32 sp64;
    s32 sp60;
    s32 sp5C;
    s32 sp58;
    s32 sp54;
    s32 scale;
    f32 sp4C;

    sp6C = &sMinimapCtx.minimapList->entry[sMinimapCtx.curRoom];
    if (sp6C->mapId == 0xFFFF) {
        return;
    }

    MapDisp_GetMapPos(sp6C, &sp60, &sp5C);
    MapDisp_GetMapTexDim(sp6C, &sp58, &sp54);
    scale = sMinimapCtx.minimapList->scale;

    if (sMinimapCtx.minimapList->scale == 0) {
        scale = 20;
    } else if (sMinimapCtx.minimapList->scale == -1) {
        s32 scaleTemp;

        MapDisp_GetMapScale(sp6C, &scaleTemp);
        scale = scaleTemp;
    }

    sp4C = 1.0f / scale;
    if (!MapDisp_IsDataRotated(play)) {
        sp68 = (s32)((x - (f32)sp6C->unk2) * sp4C) + sMinimapCtx.unk8 + (sMinimapCtx.unkC - sMinimapCtx.unk8) + sp60;
        sp64 = (s32)((z - (f32)sp6C->unk6) * sp4C) + sMinimapCtx.unkA + (sMinimapCtx.unkE - sMinimapCtx.unkA) + sp5C;
    } else {
        sp68 = -(s32)((x - (f32)sp6C->unk2) * sp4C) + sMinimapCtx.unk8 + (sMinimapCtx.unkC - sMinimapCtx.unk8) + sp60;
        sp64 = -(s32)((z - (f32)sp6C->unk6) * sp4C) + sMinimapCtx.unkA + (sMinimapCtx.unkE - sMinimapCtx.unkA) + sp5C;
    }

    if ((sp68 > 0) && (sp68 < 0x3FF) && (sp64 > 0) && (sp64 < 0x3FF)) {
        OPEN_DISPS(play->state.gfxCtx);

        Gfx_SetupDL42_Overlay(play->state.gfxCtx);
        gSPMatrix(OVERLAY_DISP++, &gIdentityMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gDPSetCombineLERP(OVERLAY_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0,
                          PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);
        gDPSetEnvColor(OVERLAY_DISP++, 0, 0, 0, 255);
        gDPSetCombineMode(OVERLAY_DISP++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
        gDPSetRenderMode(OVERLAY_DISP++, G_RM_AA_DEC_LINE, G_RM_NOOP2);
        Matrix_Translate(sp68 - 160.0f, 120.0f - sp64, 0.0f, MTXMODE_NEW);
        Matrix_RotateXFApply(-1.6f);
        if (MapDisp_IsDataRotated(play)) {
            rot += 0x7FFF;
        }
        Matrix_RotateYF(rot / 10.0f, MTXMODE_APPLY);
        Matrix_Scale(0.4f, 0.4f, 0.4f, MTXMODE_APPLY);
        gSPMatrix(OVERLAY_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gDPSetPrimColor(OVERLAY_DISP++, 0, 0xFF, 200, 0, 0, play->interfaceCtx.minimapAlpha);
        gSPDisplayList(OVERLAY_DISP++, gameplay_keep_DL_01ED00);

        CLOSE_DISPS(play->state.gfxCtx);
    }
}

s32 func_80106408(PlayState* play) {
    if ((gSaveContext.save.entrance == ENTRANCE(ROMANI_RANCH, 0)) && (Cutscene_GetSceneLayer(play) != 0)) {
        return true;
    }
    return false;
}

s32 func_80106450(PlayState* play) {
    if ((!func_8010A0A4(play) && Inventory_IsMapVisible(play->sceneId)) ||
        (func_8010A0A4(play) && CHECK_DUNGEON_ITEM(DUNGEON_MAP, gSaveContext.mapIndex))) {
        return true;
    }
    return false;
}

s32 func_801064CC(PlayState* play) {
    if (((play->csCtx.state != CS_STATE_IDLE) && !func_80106408(play)) || (sMinimapCtx.unk20 & 2) ||
        Map_IsInBossArea(play)) {
        return true;
    }
    return false;
}

s32 func_80106530(PlayState* play) {
    if ((func_801064CC(play) == true) || !func_80106450(play)) {
        return true;
    }
    return false;
}

s32 func_8010657C(s32 curRoom, s32 prevRoom) {
    MinimapEntry* entry;
    s16* temp1; // Can be removed, but adds readability

    if ((curRoom == -1) || (prevRoom == -1)) {
        return false;
    }

    entry = &sMinimapCtx.minimapList->entry[curRoom];
    temp1 = sMinimapCtx.unk3C;
    if ((temp1[curRoom] <= temp1[prevRoom]) && (temp1[prevRoom] <= (temp1[curRoom] + ((entry->unk8 >> 2) & 7)))) {
        return true;
    }

    entry = &sMinimapCtx.minimapList->entry[prevRoom];
    if ((temp1[prevRoom] <= temp1[curRoom]) && (temp1[curRoom] <= (temp1[prevRoom] + ((entry->unk8 >> 2) & 7)))) {
        return true;
    }

    return false;
}

void Minimap_DrawImpl(PlayState* play, s32 x, s32 z, s32 rot) {
    PauseContext* pauseCtx = &play->pauseCtx;

    if ((sMinimapCtx.minimapList != 0) && (pauseCtx->state <= PAUSE_STATE_OPENING_2) && !R_MINIMAP_DISABLED &&
        (play->interfaceCtx.minimapAlpha != 0)) {
        if (!func_801064CC(play) && (sSceneNumRooms != 0)) {
            if (func_80106450(play)) {
                func_801031D0(play, sMinimapCtx.lMapCurTex, sMinimapCtx.unkC, sMinimapCtx.unkE, sMinimapCtx.curRoom,
                              1.0f - (sMinimapCtx.swapAnimTimer * 0.05f));
                if ((sMinimapCtx.curRoom != sMinimapCtx.prevRoom) &&
                    func_8010657C(sMinimapCtx.curRoom, sMinimapCtx.prevRoom)) {
                    func_801031D0(play, sMinimapCtx.lMapPrevTex, sMinimapCtx.unkC + sMinimapCtx.unk1C,
                                  sMinimapCtx.unkE + sMinimapCtx.unk1E, sMinimapCtx.prevRoom,
                                  sMinimapCtx.swapAnimTimer * 0.05f);
                }
                func_80104AE8(play);
            }
            if ((!func_8010A0A4(play) || CHECK_DUNGEON_ITEM(DUNGEON_COMPASS, gSaveContext.mapIndex)) &&
                (func_8010A0A4(play) || Inventory_IsMapVisible(play->sceneId))) {
                if (play->interfaceCtx.minigameState == MINIGAME_STATE_NONE) {
                    func_80105FE0(play, x, z, rot);
                }
                func_8010439C(play);
            }
        }
    }
}

void MapDisp_ResetIMap(void) {
    s32 i;

    D_801F56B0.rooms = 0;
    for (i = 0; i < 32; i++) {
        D_801F56B0.unk4[i] = 0;
        D_801F56B0.unk84[i] = NULL;
        D_801F56B0.unk104[i] = NULL;
    }

    D_801F56B0.unk184 = 0;
    sMinimapCtx.unk20 &= ~1;
}

void MapDisp_InitMapI(PlayState* play) {
    MapDisp_ResetIMap();
}

void MapDisp_DestroyMapI(PlayState* play) {
    MapDisp_ResetIMap();
}

// alloc pause screen dungeon map
void* func_801068FC(PlayState* play, void* heap) {
    void* heapNext;
    s32 mapId;
    s32 mapCompactId;
    s32 dungeonMapRoomIter;
    s32 var_s2;
    s32 roomIter;
    MinimapEntry* entry;
    s32 var_s3;

    heapNext = heap;
    if ((sMinimapCtx.minimapList == NULL) || (sSceneNumRooms == 0)) {
        return heapNext;
    }
    D_801F56B0.rooms = 0;

    // loop for number of rooms
    for (roomIter = 0; roomIter < sSceneNumRooms; roomIter++) {
        var_s2 = false;
        entry = &sMinimapCtx.minimapList->entry[roomIter];
        if (entry->mapId == 0xFFFF) {
            continue;
        }
        mapCompactId = MapData_GetMapCompactId(entry->mapId);
        if (mapCompactId == -1) {
            continue;
        }
        for (dungeonMapRoomIter = 0; dungeonMapRoomIter < D_801F56B0.rooms; dungeonMapRoomIter++) {
            if (mapCompactId == D_801F56B0.unk4[dungeonMapRoomIter]) {
                var_s2 = true;
                break;
            }
        }
        if (var_s2 == false) {
            D_801F56B0.unk4[D_801F56B0.rooms] = mapCompactId;
            D_801F56B0.rooms++;
        }
    }

    D_801F56B0.unk84[0] = heap;
    for (dungeonMapRoomIter = 0; dungeonMapRoomIter < D_801F56B0.rooms; dungeonMapRoomIter++) {
        mapId = D_801F56B0.unk4[dungeonMapRoomIter];
        MapDisp_GetMapITexture(D_801F56B0.unk84[dungeonMapRoomIter], mapId);
        if (dungeonMapRoomIter + 1 < D_801F56B0.rooms) {
            D_801F56B0.unk84[dungeonMapRoomIter + 1] =
                ALIGN16((intptr_t)D_801F56B0.unk84[dungeonMapRoomIter] + MapData_CPID_GetSizeOfMapTex(mapId));
        } else {
            heapNext = (intptr_t)D_801F56B0.unk84[dungeonMapRoomIter] + MapData_CPID_GetSizeOfMapTex(mapId);
        }
    }
    for (roomIter = 0; roomIter < sSceneNumRooms; roomIter++) {
        entry = &sMinimapCtx.minimapList->entry[roomIter];
        if (entry->mapId == 0xFFFF) {
            D_801F56B0.unk104[roomIter] = NULL;
        } else {
            var_s3 = false;
            mapCompactId = MapData_GetMapCompactId(entry->mapId);
            for (dungeonMapRoomIter = 0; dungeonMapRoomIter < D_801F56B0.rooms; dungeonMapRoomIter++) {
                if (mapCompactId == D_801F56B0.unk4[dungeonMapRoomIter]) {
                    var_s3 = true;
                    break;
                }
            }
            if (!var_s3) {
                D_801F56B0.unk104[roomIter] = NULL;
            } else {
                void* dummy = D_801F56B0.unk84[dungeonMapRoomIter]; //! FAKE:

                D_801F56B0.unk104[roomIter] = D_801F56B0.unk84[dungeonMapRoomIter];
            }
        }
    }
    return heapNext;
}

s32 func_80106BEC(s32 arg0, f32 arg1) {
    if (arg0 == 0) {
        if ((sMinimapCtx.unk48[0] <= arg1) && ((sMinimapCtx.unk40 == 1) || (arg1 < sMinimapCtx.unk48[1]))) {
            return true;
        }
    } else if (arg0 >= (sMinimapCtx.unk40 - 1)) {
        if (sMinimapCtx.unk48[sMinimapCtx.unk40 - 1] <= arg1) {
            return true;
        }
    } else if ((sMinimapCtx.unk48[arg0] <= arg1) && (arg1 < sMinimapCtx.unk48[arg0 + 1])) {
        return true;
    }
    return false;
}

s32 func_80106D08(s32 sceneId) {
    switch (sceneId) {
        case SCENE_MITURIN_BS:
            return SCENE_MITURIN;

        case SCENE_HAKUGIN_BS:
            return SCENE_HAKUGIN;

        case SCENE_SEA_BS:
            return SCENE_SEA;

        case SCENE_INISIE_BS:
            return SCENE_INISIE_N;

        default:
            return sceneId;
    }
}

#define COLOR16(r, g, b, a)                                                          \
    (s16)(((s32)(((r)*31.0f) / 255.0f) << 11) | ((s32)(((g)*31.0f) / 255.0f) << 6) | \
          ((s32)(((b)*31.0f) / 255.0f) << 1) | (a))

void func_80106D5C(PlayState* play, s32 arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5, s32 arg6) {
    PauseContext* pauseCtx = &play->pauseCtx;
    s32 var_a1_2;
    s32 var_a1_4;
    s32 var_a1_5;
    s32 var_v1;
    s32 i;
    s32 temp1 = ((D_801F56B0.unk184 * -120.0f / 40.0f) + 200.0f) * 31.0f / 255.0f;
    s32 temp2 = ((D_801F56B0.unk184 * 115.0f / 40.0f) + 140.0f) * 31.0f / 255.0f;

    D_801BECC4[1] = (temp1 << 6) | (temp2 << 1) | 1;

    if (CHECK_DUNGEON_ITEM(DUNGEON_MAP, arg6)) {
        s32 requiredScopeTemp;

        D_801BEC84[0xF] = 0xAD5F;
        D_801BECA4[0xF] = 0xAD5F;
        D_801BECC4[0xF] = 0xAD5F;
    } else {
        D_801BECC4[0xF] = D_801BECA4[0xF] = D_801BEC84[0xF] = 0;
    }

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL39_Opa(play->state.gfxCtx);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, pauseCtx->alpha);
    gDPLoadTLUT_pal16(POLY_OPA_DISP++, 0, D_801BEC84);
    gDPLoadTLUT_pal16(POLY_OPA_DISP++, 1, D_801BECA4);
    gDPLoadTLUT_pal16(POLY_OPA_DISP++, 2, D_801BECC4);
    gDPSetTextureLUT(POLY_OPA_DISP++, G_TT_RGBA16);

    for (i = 0; i < sSceneNumRooms; i++) {
        s32 texWidth;
        s32 texHeight;
        s32 offsetX;
        s32 offsetY;
        MinimapEntry* entry;
        TexturePtr texture;
        s32 sp100;
        s32 spFC;
        s32 dsdx;
        s32 dtdy;
        s32 texPosX;
        s32 texPosY;
        s32 spE8;
        s32 two = 2;

        entry = &sMinimapCtx.minimapList->entry[i];
        if ((entry->mapId == 0xFFFF) || (entry->mapId >= 0x162)) {
            continue;
        }

        if ((sMinimapCtx.unk42 < sMinimapCtx.unk3C[i]) ||
            ((sMinimapCtx.unk3C[i] + ((entry->unk8 >> 2) & 7)) < sMinimapCtx.unk42)) {
            continue;
        }

        texture = D_801F56B0.unk104[i];
        if (texture == NULL) {
            continue;
        }

        spE8 = MapData_GetMapCompactId(entry->mapId);
        if (spE8 == -1) {
            continue;
        }

        MapData_CPID_GetTexDim(spE8, &texWidth, &texHeight);
        MapData_CPID_GetTexOffset(spE8, &offsetX, &offsetY);

        if (entry->unk8 & 1) {
            offsetX = (texWidth / 2 - offsetX) + texWidth / 2;
            sp100 = (texWidth - 1) << 5;
            dsdx = 0x3F << 10;
        } else {
            sp100 = 0;
            dsdx = 1 << 10;
        }

        if (entry->unk8 & 2) {
            s32 requiredScopeTemp;

            offsetY = (texHeight / 2 - offsetY) + texHeight / 2;
            spFC = (texHeight - 1) << 5;
            dtdy = 0x3F << 10;
        } else {
            spFC = 0;
            dtdy = 1 << 10;
        }

        texPosX = ((entry->unk2 - (f32)sMinimapCtx.sceneMidX) * arg5 - offsetX) + ((arg3 / two) + arg1);
        texPosY = ((entry->unk6 - (f32)sMinimapCtx.sceneMidZ) * arg5 - offsetY) + ((arg4 / two) + arg2);

        if (i == play->roomCtx.curRoom.num) {
            if (Map_IsInBossArea(play)) {
                gDPLoadTextureBlock_4b(POLY_OPA_DISP++, texture, G_IM_FMT_CI, texWidth, texHeight, 1,
                                       G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                       G_TX_NOLOD, G_TX_NOLOD);
            } else {
                gDPLoadTextureBlock_4b(POLY_OPA_DISP++, texture, G_IM_FMT_CI, texWidth, texHeight, 2,
                                       G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                       G_TX_NOLOD, G_TX_NOLOD);
            }
        } else if (((void)0, gSaveContext.save.saveInfo
                                 .permanentSceneFlags[Play_GetOriginalSceneId(func_80106D08(play->sceneId))])
                       .rooms &
                   (1 << i)) {
            gDPLoadTextureBlock_4b(POLY_OPA_DISP++, texture, G_IM_FMT_CI, texWidth, texHeight, 1,
                                   G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                   G_TX_NOLOD, G_TX_NOLOD);
        } else if (CHECK_DUNGEON_ITEM(DUNGEON_MAP, arg6)) {
            gDPLoadTextureBlock_4b(POLY_OPA_DISP++, texture, G_IM_FMT_CI, texWidth, texHeight, 0,
                                   G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                   G_TX_NOLOD, G_TX_NOLOD);
        } else {
            continue;
        }
        gSPTextureRectangle(POLY_OPA_DISP++, (texPosX << 2), (texPosY << 2), (texPosX + texWidth) << 2,
                            (texPosY + texHeight) << 2, 0, sp100, spFC, dsdx, dtdy);
        gDPPipeSync(POLY_OPA_DISP++);
    }

    gDPSetTextureLUT(POLY_OPA_DISP++, G_TT_NONE);

    CLOSE_DISPS(play->state.gfxCtx);
}

void func_80107B78(PlayState* play, s32 arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5) {
    s32 pad[23];
    MinimapChest* var_s0 = sMinimapCtx.unk54;
    s32 sp3C;
    MinimapEntry* sp38;
    s32 posX;
    s32 posZ;
    s32 i;
    s32 flags;
    s32 offsetX = 4;
    s32 offsetZ = 4;

    if (var_s0 == NULL) {
        return;
    }

    OPEN_DISPS(play->state.gfxCtx);

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetTextureLUT(POLY_OPA_DISP++, G_TT_NONE);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, play->pauseCtx.alpha);
    gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 255);
    gDPPipeSync(POLY_OPA_DISP++);

    gDPLoadTextureBlock_TEST(POLY_OPA_DISP++, gMapChestIconTex, G_IM_FMT_RGBA, D_801BEAE0[2], D_801BEAF0[2],
                             D_801BEB00[2], D_801BEB10[2], D_801BEB20[2], D_801BEAD0[2], 8, 8, 0,
                             G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                             G_TX_NOLOD);

    for (i = 0; i < sMinimapCtx.numChests; i++) {
        sp3C = var_s0[i].unk0;
        sp38 = &sMinimapCtx.minimapList->entry[sp3C];

        switch (play->sceneId) {
            case SCENE_MITURIN_BS:
                flags = ((void)0, gSaveContext.cycleSceneFlags[SCENE_MITURIN].chest) & (1 << var_s0[i].unk2);
                break;

            case SCENE_HAKUGIN_BS:
                flags = ((void)0, gSaveContext.cycleSceneFlags[SCENE_HAKUGIN].chest) & (1 << var_s0[i].unk2);
                break;

            case SCENE_SEA_BS:
                flags = ((void)0, gSaveContext.cycleSceneFlags[SCENE_SEA].chest) & (1 << var_s0[i].unk2);
                break;

            case SCENE_INISIE_BS:
                flags = ((void)0, gSaveContext.cycleSceneFlags[SCENE_INISIE_N].chest) & (1 << var_s0[i].unk2);
                break;

            default:
                flags = Flags_GetTreasure(play, var_s0[i].unk2);
                break;
        }

        if ((sMinimapCtx.unk42 < sMinimapCtx.unk3C[sp3C]) ||
            ((sMinimapCtx.unk3C[sp3C] + ((sp38->unk8 >> 2) & 7)) < sMinimapCtx.unk42) || (flags != 0)) {
            continue;
        }

        if (!func_80106BEC((s32)sMinimapCtx.unk42, (f32)var_s0[i].unk6)) {
            continue;
        }

        posX = (s32)((((var_s0[i].unk4 - (f32)sMinimapCtx.sceneMidX) * arg5) - offsetX) + ((arg3 / 2) + arg1));
        posZ = (s32)((((var_s0[i].unk8 - (f32)sMinimapCtx.sceneMidZ) * arg5) - offsetZ) + ((arg4 / 2) + arg2));

        gSPTextureRectangle(POLY_OPA_DISP++, posX << 2, posZ << 2, (posX + 8) << 2, (posZ + 8) << 2, G_TX_RENDERTILE, 0,
                            0, 1 << 10, 1 << 10);
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

void func_80108124(PlayState* play, s32 arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5, s32 arg6) {
    PauseContext* pauseCtx = &play->pauseCtx;
    TransitionActorList* transitionActors = &D_801BEC24;
    TransitionActorEntry* var_s1;
    s32 tempX;
    s32 tempY;
    s32 i;
    s8 roomA;
    s8 roomB;

    if (transitionActors->count != 0) {
        OPEN_DISPS(play->state.gfxCtx);

        gDPSetCombineMode(POLY_OPA_DISP++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        gDPLoadTextureBlock_4b(POLY_OPA_DISP++, &D_801BEB38, G_IM_FMT_I, 16, 16, 0, G_TX_NOMIRROR | G_TX_WRAP,
                               G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
        for (i = 0; i < transitionActors->count; i++) {
            if (func_80106BEC(sMinimapCtx.unk42, D_801F53B0[i].pos.y)) {
                if ((ABS_ALT(D_801F53B0[i].id) != ACTOR_EN_HOLL) && !func_80105328(D_801F53B0[i].params)) {
                    roomA = D_801F53B0[i].sides[0].room;
                    roomB = D_801F53B0[i].sides[1].room;
                    if (CHECK_DUNGEON_ITEM(DUNGEON_MAP, gSaveContext.mapIndex) || (roomA < 0) ||
                        (gSaveContext.save.saveInfo
                             .permanentSceneFlags[Play_GetOriginalSceneId(func_80106D08(play->sceneId))]
                             .rooms &
                         (1 << roomA)) ||
                        (roomB < 0) ||
                        (gSaveContext.save.saveInfo
                             .permanentSceneFlags[Play_GetOriginalSceneId(func_80106D08(play->sceneId))]
                             .rooms &
                         (1 << roomB))) {
                        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, pauseCtx->alpha);

                        tempX = ((f32)D_801F53B0[i].pos.x - sMinimapCtx.sceneMidX) * arg5 + ((arg3 / 2) + arg1);
                        tempY = ((f32)D_801F53B0[i].pos.z - sMinimapCtx.sceneMidZ) * arg5 + ((arg4 / 2) + arg2);
                        gSPTextureRectangle(POLY_OPA_DISP++, ((tempX - 1) << 2), ((tempY - 1) << 2), ((tempX + 1) << 2),
                                            ((tempY + 1) << 2), G_TX_RENDERTILE, 0, 0, 0x0400, 0x0400);
                    }
                }
            }
        }

        CLOSE_DISPS(play->state.gfxCtx);
    }
}

void func_80108558(PlayState* play, s32 arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5, s32 arg6) {
    s32 i;
    TransitionActorList* sp30 = &D_801BEC24;
    s32 offsetX = 4;
    s32 offsetZ = 4;
    s32 posX;
    s32 posZ;

    OPEN_DISPS(play->state.gfxCtx);

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetTextureLUT(POLY_OPA_DISP++, G_TT_NONE);
    gDPSetRenderMode(POLY_OPA_DISP++, G_RM_AA_DEC_LINE, G_RM_NOOP2);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, play->pauseCtx.alpha);
    gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 255);
    gDPPipeSync(POLY_OPA_DISP++);

    if (CHECK_DUNGEON_ITEM(DUNGEON_COMPASS, arg6)) {
        gDPLoadTextureBlock_TEST(POLY_OPA_DISP++, gMapBossIconTex, G_IM_FMT_IA, D_801BEAE0[1], D_801BEAF0[1],
                                 D_801BEB00[1], D_801BEB10[1], D_801BEB20[1], D_801BEAD0[1], 8, 8, 0,
                                 G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                 G_TX_NOLOD, G_TX_NOLOD);

        for (i = 0; i < sp30->count; i++) {
            if (!func_80105328(D_801F53B0[i].params)) {
                continue;
            }
            if (!func_80106BEC(sMinimapCtx.unk42, D_801F53B0[i].pos.y)) {
                continue;
            }
            if (ABS_ALT(D_801F53B0[i].id) == ACTOR_EN_HOLL) {
                continue;
            }

            posX = ((((f32)D_801F53B0[i].pos.x - sMinimapCtx.sceneMidX) * arg5) - offsetX) + ((arg3 / 2) + arg1);
            posZ = ((((f32)D_801F53B0[i].pos.z - sMinimapCtx.sceneMidZ) * arg5) - offsetZ) + ((arg4 / 2) + arg2);
            gSPTextureRectangle(POLY_OPA_DISP++, posX << 2, posZ << 2, (posX + 8) << 2, (posZ + 8) << 2,
                                G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);
        }
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

TexturePtr func_80108A10(s32 arg0) {
    if ((arg0 >= 0) && (arg0 < 8)) {
        return D_801BECE4[arg0];
    }
    if ((arg0 >= -8) && (arg0 < 0)) {
        return D_801BED00[-arg0];
    }
    return &D_0C000000;
}

s32 func_80108A64(PlayState* play) {
    PauseContext* pauseCtx = &play->pauseCtx;

    if (pauseCtx->pageIndex != PAUSE_MAP) {
        return true;
    }
    if ((pauseCtx->state == PAUSE_STATE_SAVEPROMPT) || IS_PAUSE_STATE_GAMEOVER) {
        return true;
    }
    if ((pauseCtx->state != PAUSE_STATE_MAIN) || (pauseCtx->mainState != PAUSE_MAIN_STATE_IDLE)) {
        return true;
    }
    if (pauseCtx->alpha == 0) {
        return true;
    }
    return false;
}

void func_80108AF8(PlayState* play) {
    PauseContext* pauseCtx = &play->pauseCtx;
    s32 temp_a0_2;
    s32 temp_a2_2;
    s16 a3;
    s16 a2;
    s32 pad;
    s32 var_s1;
    s32 spB4 = 0;

    if ((sMinimapCtx.minimapList != NULL) && (sSceneNumRooms != 0) && !func_80108A64(play)) {
        if (Map_IsInBossArea(play)) {
            switch (play->sceneId) {
                case SCENE_MITURIN_BS:
                    spB4 = 0;
                    break;

                case SCENE_HAKUGIN_BS:
                    spB4 = 1;
                    break;

                case SCENE_SEA_BS:
                    spB4 = 2;
                    break;

                case SCENE_INISIE_BS:
                    spB4 = 3;
                    break;

                default:
                    break;
            }
        } else {
            spB4 = gSaveContext.mapIndex;
        }
        OPEN_DISPS(play->state.gfxCtx);

        Gfx_SetupDL39_Opa(play->state.gfxCtx);
        gDPSetCombineMode(POLY_OPA_DISP++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 200, pauseCtx->alpha);

        for (var_s1 = 0; var_s1 < sMinimapCtx.unk40; var_s1++) {
            if ((gSaveContext.save.saveInfo.permanentSceneFlags[Play_GetOriginalSceneId(func_80106D08(play->sceneId))]
                     .unk_14 &
                 gBitFlags[4 - var_s1]) ||
                CHECK_DUNGEON_ITEM2(DUNGEON_MAP, spB4)) {
                gDPLoadTextureBlock(POLY_OPA_DISP++, func_80108A10(sMinimapCtx.unk44 + var_s1), G_IM_FMT_IA,
                                    G_IM_SIZ_8b, 24, 16, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP,
                                    G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
                gSPTextureRectangle(POLY_OPA_DISP++, 0x0144, (125 - var_s1 * 15) << 2, 0x01A4,
                                    ((125 - var_s1 * 15) + 16) << 2, G_TX_RENDERTILE, 0, 0, 0x0400, 0x0400);
            }
        }
        gDPPipeSync(POLY_OPA_DISP++);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 150, 150, 255, pauseCtx->alpha);
        gDPLoadTextureBlock(POLY_OPA_DISP++, func_80108A10((sMinimapCtx.unk44 - pauseCtx->unk_256) + 8), G_IM_FMT_IA,
                            G_IM_SIZ_8b, 24, 16, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK,
                            G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

        a3 = 80;
        a2 = 106;
        temp_a0_2 = (5 + (pauseCtx->unk_256 * 0xF));
        temp_a2_2 = temp_a0_2 + 0x10;
        if ((pauseCtx->cursorSpecialPos == 0) && (pauseCtx->cursorXIndex[1] == 0)) {
            a2++;
            a3--;
            temp_a2_2 += 4;
            temp_a0_2 -= 4;
            gSPTextureRectangle(POLY_OPA_DISP++, a3 << 2, (temp_a0_2) << 2, a2 << 2, (temp_a2_2) << 2, G_TX_RENDERTILE,
                                0, 0, 0x036E, 0x02AA);
        } else {
            gSPTextureRectangle(POLY_OPA_DISP++, (a3 + 1) << 2, temp_a0_2 << 2, (a2 - 1) << 2, temp_a2_2 << 2,
                                G_TX_RENDERTILE, 0, 0, 0x0400, 0x0400);
        }
        Gfx_SetupDL42_Opa(play->state.gfxCtx);

        CLOSE_DISPS(play->state.gfxCtx);
    }
}

s32 func_801090B0(s32 arg0) {
    if ((sMinimapCtx.minimapList == NULL) || (sSceneNumRooms == 0)) {
        return false;
    }
    if ((arg0 < 0) || (arg0 >= 6)) {
        return false;
    }
    if (sMinimapCtx.unk48[arg0] != -0x7FFF) {
        return true;
    }
    return false;
}

s32 func_80109124(s16 arg0) {
    s32 i;

    if ((sMinimapCtx.minimapList == NULL) || (sSceneNumRooms == 0)) {
        return -1;
    }
    if (sMinimapCtx.unk40 < 2) {
        return 0;
    }
    if ((sMinimapCtx.unk48[1] - 5) >= arg0) {
        return 0;
    }
    for (i = 1; i < sMinimapCtx.unk40; i++) {
        if (((sMinimapCtx.unk48[i] - 5) < arg0) && ((sMinimapCtx.unk48[i + 1] - 5) >= arg0)) {
            return i;
        }
    }
    return sMinimapCtx.unk40 - 1;
}

void func_801091F0(PlayState* play) {
    MinimapEntry* entry;
    f32 temp;
    s32 var_t2;
    s32 var_v0;
    s32 sp44 = 0;
    s32 sp40 = 0;
    s32 sp3C = 0;

    if (func_80108A64(play)) {
        return;
    }

    if (Map_IsInBossArea(play)) {
        switch (play->sceneId) {
            case SCENE_MITURIN_BS:
                sp44 = 0;
                break;

            case SCENE_HAKUGIN_BS:
                sp44 = 1;
                break;

            case SCENE_SEA_BS:
                sp44 = 2;
                break;

            case SCENE_INISIE_BS:
                sp44 = 3;
                break;

            default:
                break;
        }
    } else {
        sp44 = gSaveContext.mapIndex;
    }

    entry = sMinimapCtx.minimapList->entry;
    if ((entry->mapId == 0xFFFF) || (entry->mapId >= 0x162)) {
        return;
    }

    var_v0 = MapData_GetMapCompactId(entry->mapId);
    if (var_v0 == -1) {
        return;
    }

    var_t2 = MapData_CPID_GetMapScale(var_v0);
    if (var_t2 == 0) {
        var_t2 = 0x50;
    }

    for (var_v0 = 0; var_v0 < 2; var_v0++) {
        if (play->sceneId == D_801BED24[var_v0].sceneId) {
            sp40 = D_801BED24[var_v0].unk4;
            sp3C = D_801BED24[var_v0].unk8;
        }
    }

    temp = 1.0f / var_t2;

    func_80106D5C(play, sp40 + 0x90, sp3C + 0x55, 0x78, 0x64, temp, sp44);
    func_80108124(play, sp40 + 0x90, sp3C + 0x55, 0x78, 0x64, temp, sp44);
    func_80108558(play, sp40 + 0x90, sp3C + 0x55, 0x78, 0x64, temp, sp44);

    if (CHECK_DUNGEON_ITEM(DUNGEON_COMPASS, sp44)) {
        func_80107B78(play, sp40 + 0x90, sp3C + 0x55, 0x78, 0x64, temp);
    }
}

void func_80109428(PlayState* play) {
    sMinimapCtx.unk4C++;
    if (!(sMinimapCtx.unk20 & 1)) {
        D_801F56B0.unk184++;
        if (D_801F56B0.unk184 > 40) {
            sMinimapCtx.unk20 |= 1;
        }
    } else {
        D_801F56B0.unk184--;
        if (D_801F56B0.unk184 < 0) {
            sMinimapCtx.unk20 &= ~1;
        }
    }
}
