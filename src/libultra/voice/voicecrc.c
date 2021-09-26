#include "global.h"

s32 __osVoiceContDataCrc(u8* arg0, u32 arg1) {
    u32 phi_a0;
    u32 phi_v0;
    u32 phi_v1 = 0;

    for (phi_v0 = arg1; phi_v0 > 0; phi_v0--) {
        for (phi_a0 = 0x80; phi_a0 > 0; phi_a0 >>= 1) {
            phi_v1 <<= 1;
            if (*arg0 & phi_a0) {
                if (phi_v1 & 0x100) {
                    phi_v1 ^= 0x84;
                } else {
                    phi_v1++;
                }
            } else if (phi_v1 & 0x100) {
                phi_v1 ^= 0x85;
            }
        }
        arg0++;
    }

    do {
        phi_v1 <<= 1;
        if (phi_v1 & 0x100) {
            phi_v1 ^= 0x85;
        }
        phi_v0++;
    } while (phi_v0 < 8);

    return phi_v1 & 0xFF;
}
