#include "global.h"

void AudioSource_Init(GlobalContext* globalCtx) {
    SoundSource* sources = &globalCtx->soundSources[0];
    s32 i;

    // clang-format off
    for (i = 0; i < ARRAY_COUNT(globalCtx->soundSources); i++) { sources[i].countdown = 0; }
    // clang-format on
}

void AudioSource_Update(GlobalContext* globalCtx) {
    SoundSource* source = &globalCtx->soundSources[0];
    s32 i;

    for (i = 0; i < ARRAY_COUNT(globalCtx->soundSources); i++) {
        if (source->countdown != 0) {
            if (DECR(source->countdown) == 0) {
                Audio_StopSfxByPos(&source->relativePos);
            } else {
                SkinMatrix_Vec3fMtxFMultXYZ(&globalCtx->projectionMatrix, &source->originPos, &source->relativePos);
                if (source->unk_01) {
                    Audio_PlaySfxByPosAndId(&source->relativePos, source->sfxId);
                }
            }
        }

        source++;
    }
}

void func_800F048C(GlobalContext* globalCtx, Vec3f* pos, u32 duration, u16 sfxId, u32 isBool) {
    s32 countdown;
    SoundSource* source;
    s32 smallestCountdown = 0xFFFF;
    SoundSource* backupSource = NULL;
    s32 i;

    source = &globalCtx->soundSources[0];
    for (i = 0; i < ARRAY_COUNT(globalCtx->soundSources); i++) {
        if (source->countdown == 0) {
            break;
        }

        countdown = source->countdown;
        if (countdown < smallestCountdown) {
            smallestCountdown = countdown;
            backupSource = source;
        }
        source++;
    }

    if (i >= ARRAY_COUNT(globalCtx->soundSources)) {
        source = backupSource;
        Audio_StopSfxByPos(&source->relativePos);
    }

    source->originPos = *pos;
    source->countdown = duration;
    source->unk_01 = isBool;
    source->sfxId = sfxId;

    SkinMatrix_Vec3fMtxFMultXYZ(&globalCtx->projectionMatrix, &source->originPos, &source->relativePos);
    Audio_PlaySfxByPosAndId(&source->relativePos, sfxId);
}

void AudioSource_CreateSoundSourceSoundSource(GlobalContext* globalCtx, Vec3f* pos, u32 duration, u16 sfxId) {
    func_800F048C(globalCtx, pos, duration, sfxId, false);
}

void func_800F0590(GlobalContext* globalCtx, Vec3f* pos, u32 duration, u16 sfxId) {
    func_800F048C(globalCtx, pos, duration, sfxId, true);
}
