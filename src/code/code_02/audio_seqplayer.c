#include "global.h"

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/func_801974D0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/func_80197538.s")

void Audio_SequenceChannelInit(SequenceChannel* seqChannel) {
    s32 i;

    if (seqChannel == &gAudioContext.sequenceChannelNone) {
        return;
    }

    seqChannel->enabled = false;
    seqChannel->finished = false;
    seqChannel->stopScript = false;
    seqChannel->stopSomething2 = false;
    seqChannel->hasInstrument = false;
    seqChannel->stereoHeadsetEffects = false;
    seqChannel->transposition = 0;
    seqChannel->largeNotes = false;
    seqChannel->bookOffset = 0;
    seqChannel->stereo.asByte = 0;
    seqChannel->changes.asByte = 0xFF;
    seqChannel->scriptState.depth = 0;
    seqChannel->newPan = 0x40;
    seqChannel->panChannelWeight = 0x80;
    seqChannel->unk_10 = 0xFF;
    seqChannel->velocityRandomVariance = 0;
    seqChannel->durationRandomVariance = 0;
    seqChannel->noteUnused = NULL;
    seqChannel->reverbIndex = 0;
    seqChannel->reverb = 0;
    seqChannel->unk_0C = 0;
    seqChannel->notePriority = 3;
    seqChannel->someOtherPriority = 1;
    seqChannel->delay = 0;
    seqChannel->adsr.envelope = gDefaultEnvelope;
    seqChannel->adsr.releaseRate = 0xF0;
    seqChannel->adsr.sustain = 0;
    seqChannel->vibratoRateTarget = 0x800;
    seqChannel->vibratoRateStart = 0x800;
    seqChannel->vibratoExtentTarget = 0;
    seqChannel->vibratoExtentStart = 0;
    seqChannel->vibratoRateChangeDelay = 0;
    seqChannel->vibratoExtentChangeDelay = 0;
    seqChannel->vibratoDelay = 0;
    seqChannel->filter = NULL;
    seqChannel->unk_20 = 0;
    seqChannel->unk_0F = 0;
    seqChannel->volume = 1.0f;
    seqChannel->volumeScale = 1.0f;
    seqChannel->freqScale = 1.0f;

    for (i = 0; i < 8; i++) {
        seqChannel->soundScriptIO[i] = -1;
    }

    seqChannel->unused = false;
    Audio_InitNoteLists(&seqChannel->notePool);
    seqChannel->unk_DC = 0;
    seqChannel->unk_E0 = 0;
    seqChannel->unk_D0 = 0;
}

#ifdef NON_EQUIVALENT
s32 Audio_SeqChannelSetLayer(SequenceChannel* seqChannel, s32 layerIdx) {
    SequenceChannelLayer* layer;

    if (seqChannel->layers[layerIdx] == NULL) {
        SequenceChannelLayer* layer;
        
        layer = Audio_AudioListPopBack(&gAudioContext.layerFreeList);
        seqChannel->layers[layerIdx] = layer;
        if (layer == NULL) {
            seqChannel->layers[layerIdx] = NULL;
            return -1;
        }
    } else {
        Audio_SeqChanLayerNoteDecay(seqChannel->layers[layerIdx]);
    }

    layer = seqChannel->layers[layerIdx];
    layer->seqChannel = seqChannel;
    layer->adsr = seqChannel->adsr;
    layer->adsr.releaseRate = 0;
    layer->enabled = true;
    layer->finished = false;
    layer->stopSomething = false;
    layer->continuousNotes = false;
    layer->bit3 = false;
    layer->ignoreDrumPan = false;
    layer->bit1 = false;
    layer->notePropertiesNeedInit = false;
    layer->stereo.asByte = 0;
    layer->portamento.mode = 0;
    layer->scriptState.depth = 0;
    layer->noteDuration = 0x80;
    // layer->unk_08 = 0x80;
    layer->pan = 0x40;
    layer->transposition = 0;
    layer->delay = 0;
    layer->duration = 0;
    layer->delay2 = 0;
    layer->note = NULL;
    layer->instrument = NULL;

    layer->freqScale = 1.0f;
    layer->unk_34 = 1.0f;
    layer->velocitySquare2 = 0.0f;
    
    layer->instOrWave = 0xFF;
    return 0;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/Audio_SeqChannelSetLayer.s")
#endif

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/func_801979D8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/func_80197A54.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/func_80197AA4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/func_80197B14.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/func_80197C0C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/func_80197C8C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/func_80197D24.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/Audio_SequencePlayerDisable.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/Audio_AudioListPushBack.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/Audio_AudioListPopBack.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/Audio_InitLayerFreelist.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/func_80197F28.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/func_80197F3C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/func_80197F74.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/func_80197FB4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/func_801980D0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/func_8019815C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/func_8019825C.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/func_80198640.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/func_80198CE0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/func_801990F4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/func_80199124.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/func_80199198.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/func_80199244.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/func_80199268.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/func_8019A0BC.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/Audio_ProcessSequences.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/func_8019AAF0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/func_8019AB40.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/func_8019AC10.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/Audio_InitSequencePlayer.s")

void Audio_InitSequencePlayers(void) {
    s32 i;

    Audio_InitLayerFreelist();
    for (i = 0; i < 80; i++) {
        gAudioContext.sequenceLayers[i].seqChannel = NULL;
        gAudioContext.sequenceLayers[i].enabled = false;
    }

    for (i = 0; i < 5; i++) {
        Audio_InitSequencePlayer(&gAudioContext.seqPlayers[i]);
    }
}
