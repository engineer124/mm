#ifndef VTX_H
#define VTX_H

// Vtx Coordinates
#define SET_VTX_X_POS(vtx, index, posX)        vtx[index + 0].v.ob[0] = vtx[index + 2].v.ob[0] = posX
#define SET_VTX_X_WIDTH(vtx, index, width)     vtx[index + 1].v.ob[0] = vtx[index + 3].v.ob[0] = vtx[index + 0].v.ob[0] + width
#define SET_VTX_X(vtx, index, posX, width)     SET_VTX_X_POS(vtx, index, posX); \
                                               SET_VTX_X_WIDTH(vtx, index, width)
#define SET_VTX_X_ALT(vtx, index, posX, width) SET_VTX_X_POS(vtx, index, posX); \
                                               vtx[index + 1].v.ob[0] = vtx[index + 3].v.ob[0] = posX + width

#define SET_VTX_Y_POS(vtx, index, posY)        vtx[index + 0].v.ob[1] = vtx[index + 1].v.ob[1] = posY
#define SET_VTX_Y_HEIGHT(vtx, index, height)   vtx[index + 2].v.ob[1] = vtx[index + 3].v.ob[1] = vtx[index + 0].v.ob[1] - height
#define SET_VTX_Y(vtx, index, posY, height)    SET_VTX_Y_POS(vtx, index, posY); \
                                               SET_VTX_Y_HEIGHT(vtx, index, height)
#define SET_VTX_Y_ALT(vtx, index, posY, height)SET_VTX_Y_POS(vtx, index, posY); \
                                               vtx[index + 2].v.ob[1] = vtx[index + 3].v.ob[1] = posY - height

#define SET_VTX_Z(vtx, index, posZ)           (vtx[index + 0].v.ob[2] = vtx[index + 1].v.ob[2] = \
                                               vtx[index + 2].v.ob[2] = vtx[index + 3].v.ob[2] = posZ)
#define SET_VTX_FLAG(vtx, index)              (vtx[index + 0].v.flag  = vtx[index + 1].v.flag = \
                                               vtx[index + 2].v.flag  = vtx[index + 3].v.flag = 0)
// Vtx Texture Coordinates
// Right Texture Coordinates
#define SET_VTX_TC_1(vtx, index, value)       (vtx[index + 1].v.tc[0] = vtx[index + 3].v.tc[0] = value << 5)
// Bottom Texture Coordinates
#define SET_VTX_TC_2(vtx, index, value)       (vtx[index + 2].v.tc[1] = vtx[index + 3].v.tc[1] = value << 5)
// Left and Top Texture Coordinates
#define SET_VTX_TC_FULL_1(vtx, index, value)  (vtx[index + 0].v.tc[0] = vtx[index + 0].v.tc[1] = \
                                               vtx[index + 1].v.tc[1] = vtx[index + 2].v.tc[0] = value << 5)
// Right and Top Texture Coordinates
#define SET_VTX_TC_FULL_2(vtx, index, value)  (vtx[index + 1].v.tc[0] = vtx[index + 2].v.tc[1] = \
                                               vtx[index + 3].v.tc[0] = vtx[index + 3].v.tc[1] = value << 5)
#define SET_VTX_TC_FULL_2_ALT(vtx, index, val)(vtx[index + 1].v.tc[0] = vtx[index + 3].v.tc[0] = \
                                               vtx[index + 2].v.tc[1] = vtx[index + 3].v.tc[1] = val << 5)

// Vtx Alphas
#define SET_VTX_ALPHA(vtx, index, alpha)      (vtx[index + 0].v.cn[0] = vtx[index + 1].v.cn[0] = \
                                               vtx[index + 2].v.cn[0] = vtx[index + 3].v.cn[0] = \
                                               vtx[index + 0].v.cn[1] = vtx[index + 1].v.cn[1] = \
                                               vtx[index + 2].v.cn[1] = vtx[index + 3].v.cn[1] = \
                                               vtx[index + 0].v.cn[2] = vtx[index + 1].v.cn[2] = \
                                               vtx[index + 2].v.cn[2] = vtx[index + 3].v.cn[2] = \
                                               vtx[index + 0].v.cn[3] = vtx[index + 1].v.cn[3] = \
                                               vtx[index + 2].v.cn[3] = vtx[index + 3].v.cn[3] = alpha)
#define SET_VTX_ALPHA_ALT(vtx, index, alpha)  (vtx[index + 0].v.cn[0] = vtx[index + 2].v.cn[0] = \
                                               vtx[index + 0].v.cn[1] = vtx[index + 2].v.cn[1] = \
                                               vtx[index + 0].v.cn[2] = vtx[index + 2].v.cn[2] = \
                                               vtx[index + 1].v.cn[0] = vtx[index + 3].v.cn[0] = \
                                               vtx[index + 1].v.cn[1] = vtx[index + 3].v.cn[1] = \
                                               vtx[index + 1].v.cn[2] = vtx[index + 3].v.cn[2] = \
                                               vtx[index + 0].v.cn[3] = vtx[index + 2].v.cn[3] = \
                                               vtx[index + 1].v.cn[3] = vtx[index + 3].v.cn[3] = alpha)
#define SET_VTX_COLOR(vtx, index, color)      (vtx[index + 0].v.cn[0] = vtx[index + 1].v.cn[0] = \
                                               vtx[index + 2].v.cn[0] = vtx[index + 3].v.cn[0] = \
                                               vtx[index + 0].v.cn[1] = vtx[index + 1].v.cn[1] = \
                                               vtx[index + 2].v.cn[1] = vtx[index + 3].v.cn[1] = \
                                               vtx[index + 0].v.cn[2] = vtx[index + 1].v.cn[2] = \
                                               vtx[index + 2].v.cn[2] = vtx[index + 3].v.cn[2] = color)
#define SET_VTX_COLOR_ALT(vtx, index, color)  (vtx[index + 0].v.cn[0] = vtx[index + 2].v.cn[0] = \
                                               vtx[index + 0].v.cn[1] = vtx[index + 2].v.cn[1] = \
                                               vtx[index + 0].v.cn[2] = vtx[index + 2].v.cn[2] = \
                                               vtx[index + 1].v.cn[0] = vtx[index + 3].v.cn[0] = \
                                               vtx[index + 1].v.cn[1] = vtx[index + 3].v.cn[1] = \
                                               vtx[index + 1].v.cn[2] = vtx[index + 3].v.cn[2] = color)
#define SET_VTX_ALPHA_3(vtx, index, alpha)    (vtx[index + 0].v.cn[3] = vtx[index + 1].v.cn[3] = \
                                               vtx[index + 2].v.cn[3] = vtx[index + 3].v.cn[3] = alpha)
#define SET_VTX_ALPHA_3_ALT(vtx, index, alpha)(vtx[index + 0].v.cn[3] = vtx[index + 2].v.cn[3] = \
                                               vtx[index + 1].v.cn[3] = vtx[index + 3].v.cn[3] = alpha)
                                               #endif
