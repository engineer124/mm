#include "global.h"
#include "stack.h"
#include "system_malloc.h"

// extern UNK_TYPE1 D_801FBE10;
// extern UNK_TYPE1 D_801FBE28;
// extern UNK_TYPE1 D_801FBE2C;
// extern UNK_TYPE4 D_801FBE30;
extern STACK(sSysFlashromStack, 0x1000);
extern StackEntry sys_flashromStackEntry;
extern OSThread sys_flashromOSThread;
extern s80185D40 D_801FD008;
extern OSMesg D_801FD034;

#pragma GLOBAL_ASM("asm/non_matchings/code/sys_flashrom/func_801857C0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/sys_flashrom/func_801857D0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/sys_flashrom/func_80185864.s")

// Initialize flash
s32 func_80185908(void);
#pragma GLOBAL_ASM("asm/non_matchings/code/sys_flashrom/func_80185908.s")

// Reads `numPages` starting at `curPage` and places the result at `dst`
s32 func_80185968(void* dst, u32 curPage, s32 numPages);
#pragma GLOBAL_ASM("asm/non_matchings/code/sys_flashrom/func_80185968.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/sys_flashrom/func_801859F0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/sys_flashrom/func_80185A2C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/sys_flashrom/func_80185B1C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/sys_flashrom/func_80185BE4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/sys_flashrom/func_80185C24.s")

void SysFlashrom_ThreadEntry(s80185D40* arg0);
#pragma GLOBAL_ASM("asm/non_matchings/code/sys_flashrom/SysFlashrom_ThreadEntry.s")

// Start thread with write commmand
void func_80185DDC(void* dram, s32 curPage, s32 numPages);
#pragma GLOBAL_ASM("asm/non_matchings/code/sys_flashrom/func_80185DDC.s")

// Check if the message queue is full, which suggests a task is currently running?
s32 func_80185EC4(void);
#pragma GLOBAL_ASM("asm/non_matchings/code/sys_flashrom/func_80185EC4.s")

// block until thread is done and destroy it
s32 func_80185F04(void);
#pragma GLOBAL_ASM("asm/non_matchings/code/sys_flashrom/func_80185F04.s")

// synchronous flashrom task, start thread - block - destroy thread
s32 func_80185F64(void* dram, s32 curPage, s32 numPages);
#pragma GLOBAL_ASM("asm/non_matchings/code/sys_flashrom/func_80185F64.s")
