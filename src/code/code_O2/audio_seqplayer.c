#include "global.h"

void Audio_SequencePlayerDisable(SequencePlayer* seqPlayer);
void Audio_AudioListPushBack(AudioListItem* list, AudioListItem* item);
void* Audio_AudioListPopBack(AudioListItem* list);


u8 Audio_M64ReadU8(M64ScriptState* state);

s16 Audio_M64ReadS16(M64ScriptState* state);

u16 Audio_M64ReadCompressedU16(M64ScriptState* state);

u8 Audio_GetInstrument(SequenceChannel* seqChannel, u8 instId, Instrument** instOut, AdsrSettings* adsr);

u16 Audio_GetScriptControlFlowArgument(M64ScriptState* state, u8 arg1) {
    u8 temp_v0 = D_801D6100[arg1];
    u8 loBits = temp_v0 & 3;
    u16 ret = 0;

    if (loBits == 1) {
        if ((temp_v0 & 0x80) == 0) {
            ret = Audio_M64ReadU8(state);
        } else {
            ret = Audio_M64ReadS16(state);
        }
    }
    return ret;
}

#ifdef NON_MATCHING
s32 Audio_HandleScriptFlowControl(SequencePlayer* seqPlayer, M64ScriptState* state, s32 cmd, s32 arg) {
    switch (cmd) {
        case 0xFF:
            if (state->depth == 0) {
                return -1;
            }
            state->pc = state->stack[--state->depth];
            break;

        case 0xFD:
            return Audio_M64ReadCompressedU16(state);

        case 0xFE:
            return 1;

        case 0xFC:
            state->stack[(void)0, state->depth++] = state->pc;
            state->pc = seqPlayer->seqData + (u16)arg;
            break;

        case 0xF8:
            state->remLoopIters[(void)0, state->depth] = arg;
            state->stack[state->depth++] = state->pc;
            break;

        case 0xF7:
            state->remLoopIters[state->depth - 1]--;
            if (state->remLoopIters[state->depth - 1] != 0) {
                state->pc = state->stack[state->depth - 1];
            } else {
                state->depth--;
            }
            break;

        case 0xF6:
            state->depth--;
            break;

        case 0xF5:
        case 0xF9:
        case 0xFA:
        case 0xFB:
            if (cmd == 0xFA && state->value != 0) {
                break;
            }
            if (cmd == 0xF9 && state->value >= 0) {
                break;
            }
            if (cmd == 0xF5 && state->value < 0) {
                break;
            }
            state->pc = seqPlayer->seqData + (u16)arg;
            break;

        case 0xF2:
        case 0xF3:
        case 0xF4:
            if (cmd == 0xF3 && state->value != 0) {
                break;
            }
            if (cmd == 0xF2 && state->value >= 0) {
                break;
            }
            state->pc += (s8)(arg & 0xFF);
            break;
    }

    return 0;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/Audio_HandleScriptFlowControl.s")
#endif

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

void Audio_SeqChannelLayerDisable(SequenceChannelLayer* layer) {
    if (layer != NULL) {
        if (layer->seqChannel != &gAudioContext.sequenceChannelNone && layer->seqChannel->seqPlayer->finished == 1) {
            Audio_SeqChanLayerNoteRelease(layer);
        } else {
            Audio_SeqChanLayerNoteDecay(layer);
        }
        layer->enabled = false;
        layer->finished = true;
    }
}

void Audio_SeqChannelLayerFree(SequenceChannel* seqChannel, s32 layerIdx) {
    SequenceChannelLayer* layer = seqChannel->layers[layerIdx];

    if (layer != NULL) {
        Audio_AudioListPushBack(&gAudioContext.layerFreeList, &layer->listItem);
        Audio_SeqChannelLayerDisable(layer);
        seqChannel->layers[layerIdx] = NULL;
    }
}

void Audio_SequenceChannelDisable(SequenceChannel* seqChannel) {
    s32 i;

    seqChannel->finished = true;

    for (i = 0; i < 4; i++) {
        Audio_SeqChannelLayerFree(seqChannel, i);
    }

    Audio_NotePoolClear(&seqChannel->notePool);
    seqChannel->enabled = false;
}

void Audio_SequencePlayerInitChannels(SequencePlayer* seqPlayer, u16 channelBits) {
    SequenceChannel* seqChannel;
    s32 i;

    for (i = 0; i < 0x10; i++) {
        if (channelBits & 1) {
            seqChannel = seqPlayer->channels[i];
            seqChannel->bankId = seqPlayer->defaultBank;
            seqChannel->muteBehavior = seqPlayer->muteBehavior;
            seqChannel->noteAllocPolicy = seqPlayer->noteAllocPolicy;
        }
        channelBits = channelBits >> 1;
    }
}

void Audio_SequencePlayerDisableChannels(SequencePlayer* seqPlayer, u16 channelBitsUnused) {
    SequenceChannel* seqChannel;
    s32 i;

    for (i = 0; i < 0x10; i++) {
        seqChannel = seqPlayer->channels[i];
        if (IS_SEQUENCE_CHANNEL_VALID(seqChannel) == 1) {
            Audio_SequenceChannelDisable(seqChannel);
        }
    }
}

void Audio_SequenceChannelEnable(SequencePlayer* seqPlayer, u8 channelIdx, void* script) {
    SequenceChannel* seqChannel = seqPlayer->channels[channelIdx];
    s32 i;

    seqChannel->enabled = true;
    seqChannel->finished = false;
    seqChannel->scriptState.depth = 0;
    seqChannel->scriptState.pc = script;
    seqChannel->delay = 0;
    for (i = 0; i < 4; i++) {
        if (seqChannel->layers[i] != NULL) {
            Audio_SeqChannelLayerFree(seqChannel, i);
        }
    }
}

void Audio_SequencePlayerDisableAsFinished(SequencePlayer* seqPlayer) {
    seqPlayer->finished = true;
    Audio_SequencePlayerDisable(seqPlayer);
}

void Audio_SequencePlayerDisable(SequencePlayer* seqPlayer) {
    Audio_SequencePlayerDisableChannels(seqPlayer, 0xFFFF);
    Audio_NotePoolClear(&seqPlayer->notePool);
    if (!seqPlayer->enabled) {
        return;
    }

    seqPlayer->enabled = false;
    seqPlayer->finished = true;

    if (Audio_IsSeqLoadComplete(seqPlayer->seqId)) {
        Audio_SetSeqLoadStatus(seqPlayer->seqId, 3);
    }
    if (Audio_IsBankLoadComplete(seqPlayer->defaultBank)) {
        Audio_SetBankLoadStatus(seqPlayer->defaultBank, 4);
    }

    if (seqPlayer->defaultBank == gAudioContext.bankLoadedPool.temporary.entries[0].id) {
        gAudioContext.bankLoadedPool.temporary.nextSide = 1;
    } else if (seqPlayer->defaultBank == gAudioContext.bankLoadedPool.temporary.entries[1].id) {
        gAudioContext.bankLoadedPool.temporary.nextSide = 0;
    }
}

void Audio_AudioListPushBack(AudioListItem* list, AudioListItem* item) {
    if (item->prev == NULL) {
        list->prev->next = item;
        item->prev = list->prev;
        item->next = list;
        list->prev = item;
        list->u.count++;
        item->pool = list->pool;
    }
}

void* Audio_AudioListPopBack(AudioListItem* list) {
    AudioListItem* item = list->prev;

    if (item == list) {
        return NULL;
    }

    item->prev->next = list;
    list->prev = item->prev;
    item->prev = NULL;
    list->u.count--;
    return item->u.value;
}

void Audio_InitLayerFreelist(void) {
    s32 i;

    gAudioContext.layerFreeList.prev = &gAudioContext.layerFreeList;
    gAudioContext.layerFreeList.next = &gAudioContext.layerFreeList;
    gAudioContext.layerFreeList.u.count = 0;
    gAudioContext.layerFreeList.pool = NULL;

    for (i = 0; i < ARRAY_COUNT(gAudioContext.sequenceLayers); i++) {
        gAudioContext.sequenceLayers[i].listItem.u.value = &gAudioContext.sequenceLayers[i];
        gAudioContext.sequenceLayers[i].listItem.prev = NULL;
        Audio_AudioListPushBack(&gAudioContext.layerFreeList, &gAudioContext.sequenceLayers[i].listItem);
    }
}

u8 Audio_M64ReadU8(M64ScriptState* state) {
    return *(state->pc++);
}

s16 Audio_M64ReadS16(M64ScriptState* state) {
    s16 ret = *(state->pc++) << 8;
    ret = *(state->pc++) | ret;
    return ret;
}

u16 Audio_M64ReadCompressedU16(M64ScriptState* state) {
    u16 ret = *(state->pc++);
    if (ret & 0x80) {
        ret = (ret << 8) & 0x7F00;
        ret = *(state->pc++) | ret;
    }
    return ret;
}

#ifdef NON_EQUIVALENT
void func_80197FB4(SequenceChannelLayer* layer) {
    s32 val;

    if (layer->enabled == 0) {
        return;
    }

    if (layer->delay > 1) {
        layer->delay--;
        if (!layer->stopSomething && layer->delay <= layer->duration) {
            Audio_SeqChanLayerNoteDecay(layer);
            layer->stopSomething = true;
        }
        return;
    }

    func_801980D0(layer);
    val = func_8019825C(layer);
    if (val == -1) {
        return;
    }

    val = func_80198CE0(layer, val);
    if (val != -1) {
        val = func_80198640(layer, val);
    }
    if (val != -1) {
        func_8019815C(layer, val);
    }

    if (layer->stopSomething == 1) {
        if ((layer->note != NULL) || layer->continuousNotes) {
            Audio_SeqChanLayerNoteDecay(layer);
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/func_80197FB4.s")
#endif

// OoT func_800E9ED8
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/func_801980D0.s")

// OoT func_800E9F64
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/func_8019815C.s")

// OoT func_800EA0C0
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/func_8019825C.s")

// OoT func_800EA440
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/func_80198640.s")

// OoT func_800EAAE0
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/func_80198CE0.s")

// OoT func_800EAEF4
void func_801990F4(SequenceChannel* seqChannel, u8 arg1) {
    if ((arg1 & 0xF) != 0) {
        seqChannel->notePriority = arg1 & 0xF;
    }
    arg1 = arg1 >> 4;
    if (arg1 != 0) {
        seqChannel->someOtherPriority = arg1;
    }
}

u8 Audio_GetInstrument(SequenceChannel* seqChannel, u8 instId, Instrument** instOut, AdsrSettings* adsr) {
    Instrument* inst = Audio_GetInstrumentInner(seqChannel->bankId, instId);

    if (inst == NULL) {
        *instOut = NULL;
        return 0;
    }
    adsr->envelope = inst->envelope;
    adsr->releaseRate = inst->releaseRate;
    *instOut = inst;
    instId += 2;
    return instId;
}

void Audio_SetInstrument(SequenceChannel* seqChannel, u8 instId) {
    if (instId >= 0x80) {
        seqChannel->instOrWave = instId;
        seqChannel->instrument = NULL;
    } else if (instId == 0x7F) {
        seqChannel->instOrWave = 0;
        seqChannel->instrument = (Instrument*)1;
    } else if (instId == 0x7E) {
        seqChannel->instOrWave = 1;
        seqChannel->instrument = (Instrument*)2;
    } else if ((seqChannel->instOrWave =
                    Audio_GetInstrument(seqChannel, instId, &seqChannel->instrument, &seqChannel->adsr)) == 0) {
        seqChannel->hasInstrument = false;
        return;
    }
    seqChannel->hasInstrument = true;
}

void Audio_SequenceChannelSetVolume(SequenceChannel* seqChannel, u8 volume) {
    seqChannel->volume = (f32)(s32)volume / 127.0f;
}

#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/Audio_SequenceChannelProcessScript.s")

#ifdef NON_EQUIVALENT
void Audio_SequencePlayerProcessSequence(SequencePlayer* seqPlayer) {
    u8 command;
    u8 commandLow;
    M64ScriptState* seqScript = &seqPlayer->scriptState;
    s16 tempS;
    u16 temp;
    s32 i;
    s32 value;
    u8* data;
    u8* data2;
    u8* data3;
    s32 pad3;
    s32 dummy;

    if (!seqPlayer->enabled) {
        return;
    }

    if (!Audio_IsSeqLoadComplete(seqPlayer->seqId) || !Audio_IsBankLoadComplete(seqPlayer->defaultBank)) {
        Audio_SequencePlayerDisable(seqPlayer);
        return;
    }

    Audio_SetSeqLoadStatus(seqPlayer->seqId, 2);
    Audio_SetBankLoadStatus(seqPlayer->defaultBank, 2);

    if (seqPlayer->muted && (seqPlayer->muteBehavior & 0x80)) {
        return;
    }

    seqPlayer->unk_E0++;
    seqPlayer->tempoAcc += seqPlayer->tempo;
    seqPlayer->tempoAcc += (s16)seqPlayer->unk_0C;

    if (seqPlayer->tempoAcc < gAudioContext.tempoInternalToExternal) {
        return;
    }

    seqPlayer->tempoAcc -= (u16)gAudioContext.tempoInternalToExternal;

    if (seqPlayer->unk_0b2 == true) {
        return;
    }

    if (seqPlayer->delay > 1) {
        seqPlayer->delay--;
    } else {
        seqPlayer->recalculateVolume = true;

        while (true) {
            command = Audio_M64ReadU8(seqScript);

            // 0xF2 and above are "flow control" commands, including termination.
            if (command >= 0xF2) {
                s32 scriptHandled =
                    Audio_HandleScriptFlowControl(seqPlayer, seqScript, command,
                                                  Audio_GetScriptControlFlowArgument(&seqPlayer->scriptState, command));

                if (scriptHandled != 0) {
                    if (scriptHandled == -1) {
                        Audio_SequencePlayerDisable(seqPlayer);
                    } else {
                        seqPlayer->delay = (u16)scriptHandled;
                    }
                    break;
                }
            } else if (command >= 0xC0) {
                switch (command) {
                    case 0xF1:
                        Audio_NotePoolClear(&seqPlayer->notePool);
                        command = Audio_M64ReadU8(seqScript);
                        Audio_NotePoolFill(&seqPlayer->notePool, command);
                        // Fake-match: the asm has two breaks in a row here,
                        // which the compiler normally optimizes out.
                        dummy = -1;
                        if (dummy < 0) {
                            dummy = 0;
                        }
                        if (dummy > 1) {
                            dummy = 1;
                        }
                        if (dummy) {}
                        break;
                    case 0xF0:
                        Audio_NotePoolClear(&seqPlayer->notePool);
                        break;
                    case 0xDF:
                        seqPlayer->transposition = 0;
                        // Note: intentional fallthrough, also executes below command
                    case 0xDE:
                        seqPlayer->transposition += (s8)Audio_M64ReadU8(seqScript);
                        break;
                    case 0xDD:
                        seqPlayer->tempo = Audio_M64ReadU8(seqScript) * 48;
                        if (seqPlayer->tempo > gAudioContext.tempoInternalToExternal) {
                            seqPlayer->tempo = (u16)gAudioContext.tempoInternalToExternal;
                        }
                        if ((s16)seqPlayer->tempo <= 0) {
                            seqPlayer->tempo = 1;
                        }
                        break;
                    case 0xDC:
                        seqPlayer->unk_0C = (s8)Audio_M64ReadU8(seqScript) * 48;
                        break;
                    case 0xDA:
                        command = Audio_M64ReadU8(seqScript);
                        temp = Audio_M64ReadS16(seqScript);
                        switch (command) {
                            case 0:
                            case 1:
                                if (seqPlayer->state != 2) {
                                    seqPlayer->fadeTimerUnkEu = temp;
                                    seqPlayer->state = command;
                                }
                                break;
                            case 2:
                                seqPlayer->fadeTimer = temp;
                                seqPlayer->state = command;
                                seqPlayer->fadeVelocity = (0 - seqPlayer->fadeVolume) / (s32)seqPlayer->fadeTimer;
                                break;
                        }
                        break;
                    case 0xDB:
                        value = Audio_M64ReadU8(seqScript);
                        switch (seqPlayer->state) {
                            case 1:
                                seqPlayer->state = 0;
                                seqPlayer->fadeVolume = 0.0f;
                                // NOTE: Intentional fallthrough
                            case 0:
                                seqPlayer->fadeTimer = seqPlayer->fadeTimerUnkEu;
                                if (seqPlayer->fadeTimerUnkEu != 0) {
                                    seqPlayer->fadeVelocity =
                                        ((value / 127.0f) - seqPlayer->fadeVolume) / (s32)(seqPlayer->fadeTimer);
                                } else {
                                    seqPlayer->fadeVolume = (s32)value / 127.0f;
                                }
                                break;
                            case 2:
                                break;
                        }
                        break;
                    case 0xD9:
                        seqPlayer->fadeVolumeScale = (s8)Audio_M64ReadU8(seqScript) / 127.0f;
                        break;
                    case 0xD7:
                        temp = Audio_M64ReadS16(seqScript);
                        Audio_SequencePlayerInitChannels(seqPlayer, temp);
                        break;
                    case 0xD6:
                        Audio_M64ReadS16(seqScript);
                        break;
                    case 0xD5:
                        seqPlayer->muteVolumeScale = (s8)Audio_M64ReadU8(seqScript) / 127.0f;
                        break;
                    case 0xD4:
                        seqPlayer->muted = true;
                        break;
                    case 0xD3:
                        seqPlayer->muteBehavior = Audio_M64ReadU8(seqScript);
                        break;
                    case 0xD1:
                    case 0xD2:
                        temp = Audio_M64ReadS16(seqScript);
                        data3 = &seqPlayer->seqData[temp];
                        if (command == 0xD2) {
                            seqPlayer->shortNoteVelocityTable = data3;
                        } else {
                            seqPlayer->shortNoteDurationTable = data3;
                        }
                        break;
                    case 0xD0:
                        seqPlayer->noteAllocPolicy = Audio_M64ReadU8(seqScript);
                        break;
                    case 0xCE:
                        command = Audio_M64ReadU8(seqScript);
                        if (command == 0) {
                            seqScript->value = (gAudioContext.audioRandom >> 2) & 0xFF;
                        } else {
                            seqScript->value = (gAudioContext.audioRandom >> 2) % command;
                        }
                        break;
                    case 0xCD: {
                        temp = Audio_M64ReadS16(seqScript);

                        if ((seqScript->value != -1) && (seqScript->depth != 3)) {
                            data = seqPlayer->seqData + (u32)(temp + (seqScript->value << 1));
                            seqScript->stack[seqScript->depth] = seqScript->pc;
                            seqScript->depth++;

                            temp = (data[0] << 8) + data[1];
                            seqScript->pc = &seqPlayer->seqData[temp];
                        }
                        break;
                    }
                    case 0xCC:
                        seqScript->value = Audio_M64ReadU8(seqScript);
                        break;
                    case 0xC9:
                        seqScript->value &= Audio_M64ReadU8(seqScript);
                        break;
                    case 0xC8:
                        seqScript->value -= Audio_M64ReadU8(seqScript);
                        break;
                    case 0xC7:
                        command = Audio_M64ReadU8(seqScript);
                        temp = Audio_M64ReadS16(seqScript);
                        data2 = &seqPlayer->seqData[temp];
                        *data2 = (u8)seqScript->value + command;
                        break;
                    case 0xC6:
                        seqPlayer->unk_0b2 = true;
                        return;
                    case 0xC5:
                        seqPlayer->unk_E0 = (u16)Audio_M64ReadS16(seqScript);
                        break;
                    case 0xEF:
                        Audio_M64ReadS16(seqScript);
                        Audio_M64ReadU8(seqScript);
                        break;
                    case 0xC4:
                        command = Audio_M64ReadU8(seqScript);
                        if (command == 0xFF) {
                            command = seqPlayer->playerIndex;
                        }
                        commandLow = Audio_M64ReadU8(seqScript);
                        func_800E20D4(command, commandLow, 0);
                        if (command == (u8)seqPlayer->playerIndex) {
                            return;
                        }
                        break;
                }
            } else {
                commandLow = command & 0x0F;

                switch (command & 0xF0) {
                    case 0:
                        seqScript->value = seqPlayer->channels[commandLow]->enabled ^ 1;
                        break;
                    case 0x50:
                        seqScript->value -= seqPlayer->unk_158[commandLow];
                        break;
                    case 0x70:
                        seqPlayer->unk_158[commandLow] = seqScript->value;
                        break;
                    case 0x80:
                        seqScript->value = seqPlayer->unk_158[commandLow];
                        if (commandLow < 2) {
                            seqPlayer->unk_158[commandLow] = -1;
                        }
                        break;
                    case 0x40:
                        Audio_SequenceChannelDisable(seqPlayer->channels[commandLow]);
                        break;
                    case 0x90:
                        temp = Audio_M64ReadS16(seqScript);
                        Audio_SequenceChannelEnable(seqPlayer, commandLow, (void*)&seqPlayer->seqData[temp]);
                        break;
                    case 0xA0:
                        tempS = Audio_M64ReadS16(seqScript);
                        Audio_SequenceChannelEnable(seqPlayer, commandLow, (void*)&seqScript->pc[tempS]);
                        break;
                    case 0xB0:
                        command = Audio_M64ReadU8(seqScript);
                        temp = Audio_M64ReadS16(seqScript);
                        data2 = &seqPlayer->seqData[temp];
                        Audio_SyncLoadSeq(command, data2, &seqPlayer->unk_158[commandLow]);
                        break;
                    case 0x60: {
                        command = Audio_M64ReadU8(seqScript);
                        value = command;
                        temp = Audio_M64ReadU8(seqScript);

                        func_800E4EEC(value, temp, &seqPlayer->unk_158[commandLow]);
                        break;
                    }
                }
            }
        }
    }

    for (i = 0; i < ARRAY_COUNT(seqPlayer->channels); i++) {
        if (seqPlayer->channels[i]->enabled) {
            Audio_SequenceChannelProcessScript(seqPlayer->channels[i]);
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_seqplayer/Audio_SequencePlayerProcessSequence.s")
#endif

void Audio_ProcessSequences(s32 arg0) {
    SequencePlayer* seqPlayer;
    u32 i;

    gAudioContext.noteSubEuOffset =
        (gAudioContext.audioBufferParameters.updatesPerFrame - arg0 - 1) * gAudioContext.maxSimultaneousNotes;
    for (i = 0; i < (u32)gAudioContext.audioBufferParameters.numSequencePlayers; i++) {
        seqPlayer = &gAudioContext.seqPlayers[i];
        if (seqPlayer->enabled == 1) {
            Audio_SequencePlayerProcessSequence(seqPlayer);
            Audio_SequencePlayerProcessSound(seqPlayer);
        }
    }
    Audio_ProcessNotes();
}

void Audio_ProcessSequence(SequencePlayer* seqPlayer) {
    while (seqPlayer->unk_DC > 0) {
        Audio_SequencePlayerProcessSequence(seqPlayer);
        Audio_SequencePlayerProcessSound(seqPlayer);
        seqPlayer->unk_DC--;
    }
}

void Audio_ResetSequencePlayer(SequencePlayer* seqPlayer) {
    s32 i;

    Audio_SequencePlayerDisable(seqPlayer);
    seqPlayer->unk_0b2 = false;
    seqPlayer->delay = 0;
    seqPlayer->state = 1;
    seqPlayer->fadeTimer = 0;
    seqPlayer->fadeTimerUnkEu = 0;
    seqPlayer->tempoAcc = 0;
    seqPlayer->tempo = 120 * TATUMS_PER_BEAT; // 120 BPM
    seqPlayer->unk_0C = 0;
    seqPlayer->transposition = 0;
    seqPlayer->noteAllocPolicy = 0;
    seqPlayer->shortNoteVelocityTable = gDefaultShortNoteVelocityTable;
    seqPlayer->shortNoteDurationTable = gDefaultShortNoteDurationTable;
    seqPlayer->unk_E0 = 0;
    seqPlayer->unk_16 = 0;
    seqPlayer->fadeVolume = 1.0f;
    seqPlayer->fadeVelocity = 0.0f;
    seqPlayer->volume = 0.0f;
    seqPlayer->muteVolumeScale = 0.5f;

    for (i = 0; i < 0x10; i++) {
        Audio_SequenceChannelInit(seqPlayer->channels[i]);
    }
}

// OoT func_800EC734
void func_8019AC10(s32 seqPlayerIdx) {
    SequenceChannel* seqChannel;
    SequencePlayer* seqPlayer = &gAudioContext.seqPlayers[seqPlayerIdx];
    s32 i, j;

    for (i = 0; i < 16; i++) {
        seqPlayer->channels[i] = Audio_AllocZeroed(&gAudioContext.notesAndBuffersPool, sizeof(SequenceChannel));
        if (seqPlayer->channels[i] == NULL) {
            seqPlayer->channels[i] = &gAudioContext.sequenceChannelNone;
        } else {
            seqChannel = seqPlayer->channels[i];
            seqChannel->seqPlayer = seqPlayer;
            seqChannel->enabled = false;
            seqChannel->unk_11 = i;
            for (j = 0; j < 4; j++) {
                seqChannel->layers[j] = NULL;
            }
        }
        Audio_SequenceChannelInit(seqPlayer->channels[i]);
    }
}

void Audio_InitSequencePlayer(SequencePlayer* seqPlayer) {
    s32 i, j;

    for (i = 0; i < 0x10; i++) {
        seqPlayer->channels[i] = &gAudioContext.sequenceChannelNone;
    }

    seqPlayer->enabled = false;
    seqPlayer->muted = false;
    seqPlayer->bankDmaInProgress = false;
    seqPlayer->seqDmaInProgress = false;
    seqPlayer->unk_0b1 = false;

    for (j = 0; j < 8; j++) {
        seqPlayer->unk_158[j] = -1;
    }
    seqPlayer->muteBehavior = 0x40 | 0x20;
    seqPlayer->fadeVolumeScale = 1.0f;
    seqPlayer->unk_34 = 1.0f;
    Audio_InitNoteLists(&seqPlayer->notePool);
    Audio_ResetSequencePlayer(seqPlayer);
}

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
