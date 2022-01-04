#include "prevent_bss_reordering.h"
#include "global.h"

// This file is currently required to fix bss reordering in idle.c. It is not resolved by prevent_bss_reordering.h .
// Hopefully it will not be permanent.

u8 sIrqMgrStack[0x500];
StackEntry sIrqMgrStackInfo;
OSThread gMainThread;
u8 sMainStack[0x900];
StackEntry sMainStackInfo;
OSMesg sPiMgrCmdBuff[50];
OSMesgQueue gPiMgrCmdQ;
OSViMode gViConfigMode;
u8 D_8009B290;
