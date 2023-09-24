#include "global.h"
#include "PR/viint.h"

OSViMode osViModePalLan1 = {
    OS_VI_PAL_LAN1, // type
    {
        // comRegs
        0x311E,               // ctrl
        WIDTH(320),           // width
        BURST(58, 30, 4, 69), // burst
        VSYNC(625),           // vSync
        HSYNC(3177, 23),      // hSync
        LEAP(3183, 3181),     // leap
        HSTART(128, 768),     // hStart
        SCALE(2, 0),          // xScale
        VCURRENT(0),          // vCurrent
    },
    { // fldRegs
      {
          // [0]
          ORIGIN(640),         // origin
          SCALE(1, 0),         // yScale
          START(95, 569),      // vStart
          BURST(107, 2, 9, 0), // vBurst
          VINTR(2),            // vIntr
      },
      {
          // [1]
          ORIGIN(640),         // origin
          SCALE(1, 0),         // yScale
          START(95, 569),      // vStart
          BURST(107, 2, 9, 0), // vBurst
          VINTR(2),            // vIntr
      } },
};
