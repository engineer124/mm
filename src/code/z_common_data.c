#include "global.h"

SaveContext gSaveContext;

void SaveContext_Init(void) {
    bzero(&gSaveContext, sizeof(gSaveContext));
    gSaveContext.playerForm = 0;
    gSaveContext.seqId = (u8)NA_BGM_DISABLED;
    gSaveContext.natureAmbienceId = 0xFF;
    gSaveContext.unk_3F46 = NA_BGM_GENERAL_SFX;
    gSaveContext.nextCutsceneIndex = 0xFFEF;
    gSaveContext.cutsceneTrigger = 0;
    gSaveContext.unk_3F4D = 0;
    gSaveContext.nextDayTime = 0xFFFF;
    gSaveContext.environmentTime = 0;
    gSaveContext.dogIsLost = true;
    gSaveContext.nextTransition = 0xFF;
    gSaveContext.unk_3F26 = 50;
    gSaveContext.language = 1;
    gSaveContext.audioSetting = FS_AUDIO_STEREO;
    gSaveContext.zTargetSetting = 0;
}
