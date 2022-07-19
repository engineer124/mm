/*
 * File: z_file_nameset_NES.c
 * Overlay: ovl_file_choose
 * Description:
 */

#include "z_file_choose.h"
#include "z64rumble.h"

extern UNK_TYPE D_01002800;
extern UNK_TYPE D_01007980;
extern UNK_TYPE D_0102A6B0;
extern UNK_TYPE D_0102B170;
extern UNK_TYPE D_010310F0;
extern UNK_TYPE D_010311F0;

// there are uses of D_0E000000.fillRect (appearing as D_0E0002E0) in this file
extern GfxMasterList D_0E000000;

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_file_choose/func_80804010.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_file_choose/func_808041A0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_file_choose/func_80804654.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_file_choose/func_808047D8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_file_choose/func_8080489C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_file_choose/func_80804DAC.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_file_choose/func_80804E74.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_file_choose/func_80804F98.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_file_choose/func_8080525C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_file_choose/func_808052B0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_file_choose/func_808054A4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_file_choose/func_808055D0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_file_choose/func_808058A4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_file_choose/func_80805918.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_file_choose/func_80805A58.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_file_choose/func_80805B30.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_file_choose/func_80805C1C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_file_choose/func_80806014.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_file_choose/func_80806148.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_file_choose/func_80806310.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_file_choose/func_808067E0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_file_choose/func_80806BC8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_file_choose/func_80806CA0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_file_choose/func_80806E84.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_file_choose/func_80806F30.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_file_choose/func_808071E4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_file_choose/func_80807390.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_file_choose/func_8080742C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_file_choose/func_808074B4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_file_choose/func_808077AC.s")
