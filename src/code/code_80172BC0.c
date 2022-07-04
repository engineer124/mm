#include "global.h"

const u8 sSfxBankIds[] = {
    BANK_PLAYER, BANK_ITEM, BANK_ENV, BANK_ENEMY, BANK_SYSTEM, BANK_OCARINA, BANK_VOICE,
};

void AudioMgr_StopAllSfxExceptSystem(void) {
    u32 i;

    for (i = 0; i < ARRAY_COUNT(sSfxBankIds); i++) {
        if (sSfxBankIds[i] != BANK_SYSTEM) {
            //! FAKE
            i += (0, 0);
            Audio_StopSfxByBank(sSfxBankIds[i]);
        }
    }
}
