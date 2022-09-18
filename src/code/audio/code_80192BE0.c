#include "global.h"

AudioTask* AudioThread_UpdateImpl(void);
void AudioThread_SetFadeOutTimer(s32 seqPlayerIndex, s32 fadeTimer);
void AudioThread_SetFadeInTimer(s32 seqPlayerIndex, s32 fadeTimer);
void AudioThread_ProcessCmds(u32 msg);
void AudioThread_ProcessSeqPlayerCmd(SequencePlayer* seqPlayer, AudioCmd* cmd);
void AudioThread_ProcessChannelCmd(SequenceChannel* channel, AudioCmd* cmd);
s32 func_8019440C(s32 seqPlayerIndex, s32 arg1, s32 arg2, s32* arg3, s32* arg4);
s32 AudioThread_CountAndReleaseNotes(s32 arg0);

AudioTask* AudioThread_Update(void) {
    return AudioThread_UpdateImpl();
}

AudioTask* AudioThread_UpdateImpl(void) {
    static AudioTask* sWaitingAudioTask = NULL;
    u32 samplesRemainingInAi;
    s32 numAbiCmds;
    s32 pad;
    s32 j;
    s32 sp5C;
    s16* curAiBuffer;
    OSTask_t* task;
    s32 index;
    u32 sp4C;
    s32 sp48;
    s32 i;

    gAudioContext.totalTaskCount++;
    if ((gAudioContext.totalTaskCount % gAudioContext.audioBufferParameters.specUnk4) != 0) {
        if (gCustomUpdateFunction != NULL) {
            gCustomUpdateFunction();
        }

        if (((gAudioContext.totalTaskCount % gAudioContext.audioBufferParameters.specUnk4) + 1) ==
            gAudioContext.audioBufferParameters.specUnk4) {
            return sWaitingAudioTask;
        } else {
            return NULL;
        }
    }

    osSendMesg(gAudioContext.taskStartQueueP, gAudioContext.totalTaskCount, OS_MESG_NOBLOCK);
    gAudioContext.rspTaskIndex ^= 1;
    gAudioContext.curAiBufferIndex++;
    gAudioContext.curAiBufferIndex %= 3;
    index = (gAudioContext.curAiBufferIndex + 1) % 3;
    samplesRemainingInAi = osAiGetLength() / 4;

    if (gAudioContext.resetTimer < 16) {
        if (gAudioContext.aiBufNumSamples[index] != 0) {
            osAiSetNextBuffer(gAudioContext.aiBuffers[index],
                              2 * gAudioContext.aiBufNumSamples[index] * (s32)SAMPLE_SIZE);
            if (gAudioContext.aiBuffers[index]) {}
            if (gAudioContext.aiBufNumSamples[index]) {}
        }
    }

    if (gCustomUpdateFunction != NULL) {
        gCustomUpdateFunction();
    }

    sp5C = gAudioContext.curAudioFrameDmaCount;
    for (i = 0; i < gAudioContext.curAudioFrameDmaCount; i++) {
        if (osRecvMesg(&gAudioContext.curAudioFrameDmaQueue, NULL, OS_MESG_NOBLOCK) == 0) {
            sp5C--;
        }
    }

    if (sp5C != 0) {
        for (i = 0; i < sp5C; i++) {
            osRecvMesg(&gAudioContext.curAudioFrameDmaQueue, NULL, OS_MESG_BLOCK);
        }
    }

    sp48 = gAudioContext.curAudioFrameDmaQueue.validCount;
    if (sp48 != 0) {
        for (i = 0; i < sp48; i++) {
            osRecvMesg(&gAudioContext.curAudioFrameDmaQueue, NULL, OS_MESG_NOBLOCK);
        }
    }

    gAudioContext.curAudioFrameDmaCount = 0;
    AudioLoad_DecreaseSampleDmaTtls();
    AudioLoad_ProcessLoads(gAudioContext.resetStatus);
    AudioLoad_ProcessScriptLoads();

    if (gAudioContext.resetStatus != 0) {
        if (AudioHeap_ResetStep() == 0) {
            if (gAudioContext.resetStatus == 0) {
                osSendMesg(gAudioContext.audioResetQueueP, gAudioContext.specId, OS_MESG_NOBLOCK);
            }

            sWaitingAudioTask = NULL;
            return NULL;
        }
    }

    if (gAudioContext.resetTimer > 16) {
        return NULL;
    }
    if (gAudioContext.resetTimer != 0) {
        gAudioContext.resetTimer++;
    }

    gAudioContext.curTask = &gAudioContext.rspTask[gAudioContext.rspTaskIndex];
    gAudioContext.curAbiCmdBuf = gAudioContext.abiCmdBufs[gAudioContext.rspTaskIndex];

    index = gAudioContext.curAiBufferIndex;
    curAiBuffer = gAudioContext.aiBuffers[index];

    gAudioContext.aiBufNumSamples[index] = (s16)(
        (((gAudioContext.audioBufferParameters.samplesPerFrameTarget - samplesRemainingInAi) + 0x80) & ~0xF) + 0x10);
    if (gAudioContext.aiBufNumSamples[index] < gAudioContext.audioBufferParameters.minAiBufNumSamples) {
        gAudioContext.aiBufNumSamples[index] = gAudioContext.audioBufferParameters.minAiBufNumSamples;
    }

    if (gAudioContext.aiBufNumSamples[index] > gAudioContext.audioBufferParameters.maxAiBufNumSamples) {
        gAudioContext.aiBufNumSamples[index] = gAudioContext.audioBufferParameters.maxAiBufNumSamples;
    }

    j = 0;
    if (gAudioContext.resetStatus == 0) {
        // msg = 0000RREE R = read pos, E = End Pos
        while (osRecvMesg(gAudioContext.cmdProcQueueP, (OSMesg*)&sp4C, OS_MESG_NOBLOCK) != -1) {
            //! FAKE:
            if (1) {}
            AudioThread_ProcessCmds(sp4C);
            j++;
        }
        if ((j == 0) && gAudioContext.cmdQueueFinished) {
            AudioThread_ScheduleProcessCmds();
        }
    }

    if (gAudioSPDataPtr == (u64*)gAudioContext.curAbiCmdBuf) {
        return -1;
    }

    gAudioContext.curAbiCmdBuf =
        AudioSynth_Update(gAudioContext.curAbiCmdBuf, &numAbiCmds, curAiBuffer, gAudioContext.aiBufNumSamples[index]);

    // Update audioRandom to the next random number
    gAudioContext.audioRandom = (gAudioContext.audioRandom + gAudioContext.totalTaskCount) * osGetCount();
    gAudioContext.audioRandom =
        gAudioContext.audioRandom + gAudioContext.aiBuffers[index][gAudioContext.totalTaskCount & 0xFF];

    // gWaveSamples[8] interprets compiled assembly code as s16 samples as a way to generate sound with noise.
    // Start with the address of AudioThread_Update, and offset it by a random number between 0 - 0xFFF0
    // Use the resulting address as the starting address to interpret an array of samples i.e. `s16 samples[]`
    gWaveSamples[8] = (s16*)(((u8*)AudioThread_Update) + (gAudioContext.audioRandom & 0xFFF0));

    index = gAudioContext.rspTaskIndex;
    gAudioContext.curTask->taskQueue = NULL;
    gAudioContext.curTask->unk_44 = NULL;

    task = &gAudioContext.curTask->task.t;
    task->type = M_AUDTASK;
    task->flags = 0;
    task->ucodeBoot = aspMainTextStart;
    task->ucodeBootSize = 0x1000;
    task->ucodeDataSize = ((gspF3DZEX2_NoN_PosLight_fifoTextStart - aspMainDataStart) * sizeof(u64)) - 1;
    task->ucode = aspMainTextStart;
    task->ucodeData = aspMainDataStart;
    task->ucodeSize = 0x1000;
    task->dramStack = (u64*)D_801D6200;
    task->dramStackSize = 0;
    task->outputBuff = NULL;
    task->outputBuffSize = NULL;
    if (1) {}
    task->dataPtr = (u64*)gAudioContext.abiCmdBufs[index];
    task->dataSize = numAbiCmds * sizeof(Acmd);
    task->yieldDataPtr = NULL;
    task->yieldDataSize = 0;

    if (gAudioContext.unk_29BC < numAbiCmds) {
        gAudioContext.unk_29BC = numAbiCmds;
    }

    if (gAudioContext.audioBufferParameters.specUnk4 == 1) {
        return gAudioContext.curTask;
    }

    sWaitingAudioTask = gAudioContext.curTask;
    return NULL;
}

void AudioThread_ProcessGlobalCmd(AudioCmd* cmd) {
    s32 i;
    s32 pad;

    switch (cmd->op) {
        case AUDIOCMD_OP_GLOBAL_SYNC_LOAD_SEQ_PARTS:
            AudioLoad_SyncLoadSeqParts(cmd->arg1, cmd->arg2, cmd->asInt, &gAudioContext.externalLoadQueue);
            break;

        case AUDIOCMD_OP_GLOBAL_SYNC_INIT_SEQPLAYER:
            AudioLoad_SyncInitSeqPlayer(cmd->arg0, cmd->arg1, cmd->arg2);
            AudioThread_SetFadeInTimer(cmd->arg0, cmd->asInt);
            break;

        case AUDIOCMD_OP_GLOBAL_SYNC_INIT_SEQPLAYER_SKIP_TICKS:
            AudioLoad_SyncInitSeqPlayerSkipTicks(cmd->arg0, cmd->arg1, cmd->asInt);
            AudioThread_SetFadeInTimer(cmd->arg0, 500);
            AudioSeq_SkipForwardSequence(&gAudioContext.seqPlayers[cmd->arg0]);
            break;

        case AUDIOCMD_OP_GLOBAL_DISABLE_SEQPLAYER:
            if (gAudioContext.seqPlayers[cmd->arg0].enabled) {
                if (cmd->asInt == 0) {
                    AudioSeq_SequencePlayerDisableAsFinished(&gAudioContext.seqPlayers[cmd->arg0]);
                } else {
                    AudioThread_SetFadeOutTimer(cmd->arg0, cmd->asInt);
                }
            }
            break;

        case AUDIOCMD_OP_GLOBAL_SET_SOUND_MODE:
            gAudioContext.soundMode = cmd->asInt;
            break;

        case AUDIOCMD_OP_GLOBAL_MUTE:
            if (cmd->arg0 == SEQ_ALL_SEQPLAYERS) {
                for (i = 0; i < gAudioContext.audioBufferParameters.numSequencePlayers; i++) {
                    gAudioContext.seqPlayers[i].muted = true;
                    gAudioContext.seqPlayers[i].recalculateVolume = true;
                }
            } else {
                gAudioContext.seqPlayers[cmd->arg0].muted = true;
                gAudioContext.seqPlayers[cmd->arg0].recalculateVolume = true;
            }
            break;

        case AUDIOCMD_OP_GLOBAL_UNMUTE:
            if (cmd->asInt == 1) {
                for (i = 0; i < gAudioContext.numNotes; i++) {
                    Note* note = &gAudioContext.notes[i];
                    NoteSampleState* noteSampleState = &note->noteSampleState;

                    if (noteSampleState->bitField0.enabled && (note->playbackState.status == PLAYBACK_STATUS_0) &&
                        (note->playbackState.parentLayer->channel->muteFlags & MUTE_FLAGS_STOP_SAMPLES)) {
                        noteSampleState->bitField0.finished = true;
                    }
                }
            }

            if (cmd->arg0 == SEQ_ALL_SEQPLAYERS) {
                for (i = 0; i < gAudioContext.audioBufferParameters.numSequencePlayers; i++) {
                    gAudioContext.seqPlayers[i].muted = false;
                    gAudioContext.seqPlayers[i].recalculateVolume = true;
                }
            } else {
                gAudioContext.seqPlayers[cmd->arg0].muted = false;
                gAudioContext.seqPlayers[cmd->arg0].recalculateVolume = true;
            }

            break;

        case AUDIOCMD_OP_GLOBAL_SYNC_LOAD_INSTRUMENT:
            AudioLoad_SyncLoadInstrument(cmd->arg0, cmd->arg1, cmd->arg2);
            break;

        case AUDIOCMD_OP_GLOBAL_ASYNC_LOAD_SAMPLE_BANK:
            AudioLoad_AsyncLoadSampleBank(cmd->arg0, cmd->arg1, cmd->arg2, &gAudioContext.externalLoadQueue);
            break;

        case AUDIOCMD_OP_GLOBAL_ASYNC_LOAD_FONT:
            AudioLoad_AsyncLoadFont(cmd->arg0, cmd->arg1, cmd->arg2, &gAudioContext.externalLoadQueue);
            break;

        case AUDIOCMD_OP_GLOBAL_ASYNC_LOAD_SEQ:
            AudioLoad_AsyncLoadSeq(cmd->arg0, cmd->arg1, cmd->arg2, &gAudioContext.externalLoadQueue);
            break;

        case AUDIOCMD_OP_GLOBAL_DISCARD_SEQ_FONTS:
            AudioLoad_DiscardSeqFonts(cmd->arg1);
            break;

        case AUDIOCMD_OP_GLOBAL_SET_ACTIVE_CHANNEL_FLAGS:
            gAudioContext.activeChannelsFlags[cmd->arg0] = cmd->asUShort;
            break;

        case AUDIOCMD_OP_GLOBAL_RESET_AUDIO_HEAP:
            gAudioContext.resetStatus = 5;
            gAudioContext.specId = cmd->asUInt;
            break;

        case AUDIOCMD_OP_GLOBAL_SET_CUSTOM_UPDATE_FUNCTION:
            gCustomUpdateFunction = (void (*)(void))cmd->asUInt;
            break;

        case AUDIOCMD_OP_GLOBAL_SET_CUSTOM_FUNCTION:
            if (cmd->arg2 == AUDIO_CUSTOM_FUNCTION_REVERB) {
                gCustomReverbFunction = (s32(*)(Sample*, s32, s8, s32))cmd->asUInt;
            } else if (cmd->arg2 == AUDIO_CUSTOM_FUNCTION_SYNTH) {
                gCustomSynthFunction = (Acmd * (*)(Acmd*, s32, s32)) cmd->asUInt;
            } else {
                gAudioContext.customSeqFunctions[cmd->arg2] = (u32(*)(s8, SequenceChannel*))cmd->asUInt;
            }
            break;

        case AUDIOCMD_OP_GLOBAL_SET_DRUM_FONT:
        case AUDIOCMD_OP_GLOBAL_SET_SFX_FONT:
        case AUDIOCMD_OP_GLOBAL_SET_INSTRUMENT_FONT:
            if (AudioPlayback_SetFontInstrument(cmd->op - AUDIOCMD_OP_GLOBAL_SET_DRUM_FONT, cmd->arg1, cmd->arg2,
                                                cmd->asInt)) {}
            break;

        case AUDIOCMD_OP_GLOBAL_DISABLE_ALL_SEQPLAYERS: {
            u32 flags = cmd->asUInt;

            if (flags == 1) {
                for (i = 0; i < gAudioContext.audioBufferParameters.numSequencePlayers; i++) {
                    if (gAudioContext.seqPlayers[i].enabled) {
                        AudioSeq_SequencePlayerDisableAsFinished(&gAudioContext.seqPlayers[i]);
                    }
                }
            }
            AudioThread_CountAndReleaseNotes(flags);
        } break;

        case AUDIOCMD_OP_GLOBAL_POP_PERSISTENT_CACHE:
            AudioHeap_PopPersistentCache(cmd->asInt);
            break;

        case AUDIOCMD_OP_GLOBAL_E5:
            func_8018FA60(cmd->arg0, cmd->arg1, cmd->arg2, cmd->asInt);
            break;

        case AUDIOCMD_OP_GLOBAL_SET_REVERB_DATA:
            AudioHeap_SetReverbData(cmd->arg1, cmd->arg0, cmd->asInt, 0);
            break;

        default:
            break;
    }
}

void AudioThread_SetFadeOutTimer(s32 seqPlayerIndex, s32 fadeTimer) {
    SequencePlayer* seqPlayer = &gAudioContext.seqPlayers[seqPlayerIndex];

    if (fadeTimer == 0) {
        fadeTimer = 1;
    }

    seqPlayer->fadeVelocity = -(seqPlayer->fadeVolume / fadeTimer);
    seqPlayer->state = SEQPLAYER_STATE_2;
    seqPlayer->fadeTimer = fadeTimer;
}

void AudioThread_SetFadeInTimer(s32 seqPlayerIndex, s32 fadeTimer) {
    SequencePlayer* seqPlayer;

    if (fadeTimer != 0) {
        seqPlayer = &gAudioContext.seqPlayers[seqPlayerIndex];
        seqPlayer->state = SEQPLAYER_STATE_1;
        seqPlayer->storedFadeTimer = fadeTimer;
        seqPlayer->fadeTimer = fadeTimer;
        seqPlayer->fadeVolume = 0.0f;
        seqPlayer->fadeVelocity = 0.0f;
    }
}

void AudioThread_InitMesgQueuesInternal(void) {
    gAudioContext.cmdWritePos = 0;
    gAudioContext.cmdReadPos = 0;
    gAudioContext.cmdQueueFinished = false;
    gAudioContext.taskStartQueueP = &gAudioContext.taskStartQueue;
    gAudioContext.cmdProcQueueP = &gAudioContext.cmdProcQueue;
    gAudioContext.audioResetQueueP = &gAudioContext.audioResetQueue;

    osCreateMesgQueue(gAudioContext.taskStartQueueP, gAudioContext.taskStartMsgs,
                      ARRAY_COUNT(gAudioContext.taskStartMsgs));
    osCreateMesgQueue(gAudioContext.cmdProcQueueP, gAudioContext.cmdProcMsgs, ARRAY_COUNT(gAudioContext.cmdProcMsgs));
    osCreateMesgQueue(gAudioContext.audioResetQueueP, gAudioContext.audioResetMesgs,
                      ARRAY_COUNT(gAudioContext.audioResetMesgs));
}

void AudioThread_QueueCmd(u32 opArgs, void** data) {
    AudioCmd* cmd = &gAudioContext.cmdBuf[gAudioContext.cmdWritePos & 0xFF];

    cmd->opArgs = opArgs;
    cmd->data = *data;

    gAudioContext.cmdWritePos++;

    if (gAudioContext.cmdWritePos == gAudioContext.cmdReadPos) {
        gAudioContext.cmdWritePos--;
    }
}

void AudioThread_QueueCmdF32(u32 opArgs, f32 data) {
    AudioThread_QueueCmd(opArgs, (void**)&data);
}

void AudioThread_QueueCmdS32(u32 opArgs, s32 data) {
    AudioThread_QueueCmd(opArgs, (void**)&data);
}

void AudioThread_QueueCmdS8(u32 opArgs, s8 data) {
    u32 uData = data << 0x18;

    AudioThread_QueueCmd(opArgs, (void**)&uData);
}

void AudioThread_QueueCmdU16(u32 opArgs, u16 data) {
    u32 uData = data << 0x10;

    AudioThread_QueueCmd(opArgs, (void**)&uData);
}

s32 AudioThread_ScheduleProcessCmds(void) {
    static s32 D_801D5FF4 = 0;
    s32 ret;

    if (D_801D5FF4 < (u8)((gAudioContext.cmdWritePos - gAudioContext.cmdReadPos) + 0x100)) {
        D_801D5FF4 = (u8)((gAudioContext.cmdWritePos - gAudioContext.cmdReadPos) + 0x100);
    }

    ret = osSendMesg(gAudioContext.cmdProcQueueP,
                     (void*)(((gAudioContext.cmdReadPos & 0xFF) << 8) | (gAudioContext.cmdWritePos & 0xFF)),
                     OS_MESG_NOBLOCK);
    if (ret != -1) {
        gAudioContext.cmdReadPos = gAudioContext.cmdWritePos;
        ret = 0;
    } else {
        return -1;
    }

    return ret;
}

void AudioThread_ResetCmdQueue(void) {
    gAudioContext.cmdQueueFinished = false;
    gAudioContext.cmdReadPos = gAudioContext.cmdWritePos;
}

void AudioThread_ProcessCmd(AudioCmd* cmd) {
    SequencePlayer* seqPlayer;
    u16 activeChannelsFlags;
    s32 i;

    if ((cmd->op & 0xF0) >= 0xE0) {
        AudioThread_ProcessGlobalCmd(cmd);
        return;
    }

    if (cmd->arg0 < gAudioContext.audioBufferParameters.numSequencePlayers) {
        seqPlayer = &gAudioContext.seqPlayers[cmd->arg0];
        if (cmd->op & 0x80) {
            AudioThread_ProcessGlobalCmd(cmd);
            return;
        }
        if (cmd->op & 0x40) {
            AudioThread_ProcessSeqPlayerCmd(seqPlayer, cmd);
            return;
        }

        if (cmd->arg1 < SEQ_NUM_CHANNELS) {
            AudioThread_ProcessChannelCmd(seqPlayer->channels[cmd->arg1], cmd);
            return;
        }
        if (cmd->arg1 == SEQ_ALL_CHANNELS) {
            activeChannelsFlags = gAudioContext.activeChannelsFlags[cmd->arg0];
            for (i = 0; i < SEQ_NUM_CHANNELS; i++) {
                if (activeChannelsFlags & 1) {
                    AudioThread_ProcessChannelCmd(seqPlayer->channels[i], cmd);
                }
                activeChannelsFlags = activeChannelsFlags >> 1;
            }
        }
    }
}

void AudioThread_ProcessCmds(u32 msg) {
    static u8 curCmdRdPos = 0;
    AudioCmd* cmd;
    u8 endPos;

    if (!gAudioContext.cmdQueueFinished) {
        curCmdRdPos = msg >> 8;
    }

    while (true) {
        endPos = msg & 0xFF;
        if (curCmdRdPos == endPos) {
            gAudioContext.cmdQueueFinished = false;
            break;
        }

        cmd = &gAudioContext.cmdBuf[curCmdRdPos++ & 0xFF];
        if (cmd->op == AUDIOCMD_OP_GLOBAL_STOP_AUDIOCMDS) {
            gAudioContext.cmdQueueFinished = true;
            break;
        }

        AudioThread_ProcessCmd(cmd);
        cmd->op = 0;
    }
}

u32 func_80193BA0(u32* out) {
    u32 msg;

    if (osRecvMesg(&gAudioContext.externalLoadQueue, (OSMesg*)&msg, OS_MESG_NOBLOCK) == -1) {
        *out = 0;
        return 0;
    }

    *out = msg & 0xFFFFFF;
    return msg >> 0x18;
}

u8* AudioThread_GetFontsForSequence(s32 seqId, u32* outNumFonts) {
    return AudioLoad_GetFontsForSequence(seqId, outNumFonts);
}

// unused
void AudioThread_GetSampleBankIdsOfFont(s32 fontId, u32* sampleBankId1, u32* sampleBankId2) {
    *sampleBankId1 = gAudioContext.soundFontList[fontId].sampleBankId1;
    *sampleBankId2 = gAudioContext.soundFontList[fontId].sampleBankId2;
}

s32 func_80193C5C(void) {
    s32 pad;
    s32 specId;

    if (osRecvMesg(gAudioContext.audioResetQueueP, (OSMesg*)&specId, OS_MESG_NOBLOCK) == -1) {
        return 0;
    } else if (gAudioContext.specId != specId) {
        return -1;
    } else {
        return 1;
    }
}

void AudioThread_WaitForAudioResetQueueP(void) {
    // macro?
    // clang-format off
    s32 chk = -1; s32 sp28; do {} while (osRecvMesg(gAudioContext.audioResetQueueP, (OSMesg*)&sp28, OS_MESG_NOBLOCK) != chk);
    // clang-format on
}

s32 AudioThread_ResetAudioHeap(s32 specId) {
    s32 resetStatus;
    OSMesg msg;
    s32 pad;

    AudioThread_WaitForAudioResetQueueP();
    resetStatus = gAudioContext.resetStatus;
    if (resetStatus != 0) {
        AudioThread_ResetCmdQueue();
        if (gAudioContext.specId == specId) {
            return -2;
        } else if (resetStatus > 2) {
            gAudioContext.specId = specId;
            return -3;
        } else {
            osRecvMesg(gAudioContext.audioResetQueueP, &msg, OS_MESG_BLOCK);
        }
    }

    AudioThread_WaitForAudioResetQueueP();
    AUDIOCMD_GLOBAL_RESET_AUDIO_HEAP(specId);

    return AudioThread_ScheduleProcessCmds();
}

void AudioThread_PreNMIInternal(void) {
    gAudioContext.resetTimer = 1;
    if (gAudioContextInitalized) {
        AudioThread_ResetAudioHeap(0);
        gAudioContext.resetStatus = 0;
    }
}

// Unused
s8 AudioThread_GetChannelIO(s32 seqPlayerIndex, s32 channelIndex, s32 ioPort) {
    SequencePlayer* seqPlayer = &gAudioContext.seqPlayers[seqPlayerIndex];
    SequenceChannel* channel;

    if (seqPlayer->enabled) {
        channel = seqPlayer->channels[channelIndex];
        return channel->soundScriptIO[ioPort];
    } else {
        return SEQ_IO_VAL_NONE;
    }
}

// Unused
s8 AudioThread_GetSeqPlayerIO(s32 seqPlayerIndex, s32 ioPort) {
    return gAudioContext.seqPlayers[seqPlayerIndex].soundScriptIO[ioPort];
}

// Unused
void AudioThread_InitExternalPool(void* addr, size_t size) {
    AudioHeap_InitPool(&gAudioContext.externalPool, addr, size);
}

// Unused
void AudioThread_ResetExternalPool(void) {
    gAudioContext.externalPool.startAddr = NULL;
}

void AudioThread_ProcessSeqPlayerCmd(SequencePlayer* seqPlayer, AudioCmd* cmd) {
    f32 fadeVolume;

    switch (cmd->op) {
        case AUDIOCMD_OP_SEQPLAYER_FADE_VOLUME_SCALE:
            if (seqPlayer->fadeVolumeScale != cmd->asFloat) {
                seqPlayer->fadeVolumeScale = cmd->asFloat;
                seqPlayer->recalculateVolume = true;
            }
            break;

        case AUDIOCMD_OP_SEQPLAYER_SET_TEMPO:
            seqPlayer->tempo = cmd->asInt * TATUMS_PER_BEAT;
            break;

        case AUDIOCMD_OP_SEQPLAYER_SET_SCALED_UNK_0C:
            seqPlayer->unk_0C = cmd->asInt * TATUMS_PER_BEAT;
            break;

        case AUDIOCMD_OP_SEQPLAYER_SET_UNK_0C:
            seqPlayer->unk_0C = cmd->asInt;
            break;

        case AUDIOCMD_OP_SEQPLAYER_SET_TRANSPOSITION:
            seqPlayer->transposition = cmd->asSbyte;
            break;

        case AUDIOCMD_OP_SEQPLAYER_SET_IO:
            seqPlayer->soundScriptIO[cmd->arg2] = cmd->asSbyte;
            break;

        case AUDIOCMD_OP_SEQPLAYER_SET_FADE_VOLUME:
            fadeVolume = (s32)cmd->arg1 / 127.0f;
            goto fadeBlock;

        case AUDIOCMD_OP_SEQPLAYER_SCALE_FADE_VOLUME:
            fadeVolume = ((s32)cmd->arg1 / 100.0f) * seqPlayer->fadeVolume;
        fadeBlock:
            if (seqPlayer->state != SEQPLAYER_STATE_2) {
                seqPlayer->volume = seqPlayer->fadeVolume;
                if (cmd->asInt == 0) {
                    seqPlayer->fadeVolume = fadeVolume;
                } else {
                    s32 fadeTimer = cmd->asInt;

                    seqPlayer->state = SEQPLAYER_STATE_0;
                    seqPlayer->fadeTimer = fadeTimer;
                    seqPlayer->fadeVelocity = (fadeVolume - seqPlayer->fadeVolume) / fadeTimer;
                }
            }
            break;

        case AUDIOCMD_OP_SEQPLAYER_SET_FADE_TIMER:
            if (seqPlayer->state != SEQPLAYER_STATE_2) {
                if (cmd->asInt == 0) {
                    seqPlayer->fadeVolume = seqPlayer->volume;
                } else {
                    s32 fadeTimer = cmd->asInt;

                    seqPlayer->state = SEQPLAYER_STATE_0;
                    seqPlayer->fadeTimer = fadeTimer;
                    seqPlayer->fadeVelocity = (seqPlayer->volume - seqPlayer->fadeVolume) / fadeTimer;
                }
            }
            break;

        case AUDIOCMD_OP_SEQPLAYER_SET_BEND:
            seqPlayer->bend = cmd->asFloat;
            if (seqPlayer->bend == 1.0f) {
                seqPlayer->applyBend = false;
            } else {
                seqPlayer->applyBend = true;
            }
            break;

        default:
            break;
    }
}

void AudioThread_ProcessChannelCmd(SequenceChannel* channel, AudioCmd* cmd) {
    switch (cmd->op) {
        u8 filterCutoff;

        case AUDIOCMD_OP_CHANNEL_VOL_SCALE:
            if (channel->volumeScale != cmd->asFloat) {
                channel->volumeScale = cmd->asFloat;
                channel->changes.s.volume = true;
            }
            break;

        case AUDIOCMD_OP_CHANNEL_VOL:
            if (channel->volume != cmd->asFloat) {
                channel->volume = cmd->asFloat;
                channel->changes.s.volume = true;
            }
            break;

        case AUDIOCMD_OP_CHANNEL_PAN_SIGNED:
            if (channel->newPan != cmd->asSbyte) {
                channel->newPan = cmd->asSbyte;
                channel->changes.s.pan = true;
            }
            break;

        case AUDIOCMD_OP_CHANNEL_PAN_UNSIGNED:
            if (channel->newPan != cmd->asSbyte) {
                channel->panChannelWeight = cmd->asSbyte;
                channel->changes.s.pan = true;
            }
            break;

        case AUDIOCMD_OP_CHANNEL_FREQ_SCALE:
            if (channel->freqScale != cmd->asFloat) {
                channel->freqScale = cmd->asFloat;
                channel->changes.s.freqScale = true;
            }
            break;

        case AUDIOCMD_OP_CHANNEL_REVERB_VOLUME:
            if (channel->targetReverbVol != cmd->asSbyte) {
                channel->targetReverbVol = cmd->asSbyte;
            }
            break;

        case AUDIOCMD_OP_CHANNEL_REVERB_INDEX:
            if (channel->reverbIndex != cmd->asSbyte) {
                channel->reverbIndex = cmd->asSbyte;
            }
            break;

        case AUDIOCMD_OP_CHANNEL_SURROUND_EFFECT_INDEX:
            channel->surroundEffectIndex = cmd->asSbyte;
            break;

        case AUDIOCMD_OP_CHANNEL_IO:
            if (cmd->arg2 < ARRAY_COUNT(channel->soundScriptIO)) {
                channel->soundScriptIO[cmd->arg2] = cmd->asSbyte;
            }
            break;

        case AUDIOCMD_OP_CHANNEL_MUTE:
            channel->muted = cmd->asSbyte;
            break;

        case AUDIOCMD_OP_CHANNEL_MUTE_FLAGS:
            channel->muteFlags = cmd->asSbyte;
            break;

        case AUDIOCMD_OP_CHANNEL_VIBRATO_X8:
            channel->vibrato.vibratoExtentTarget = cmd->asUbyte * 8;
            channel->vibrato.vibratoExtentChangeDelay = 1;
            break;

        case AUDIOCMD_OP_CHANNEL_VIBRATO_X32:
            channel->vibrato.vibratoRateTarget = cmd->asUbyte * 32;
            channel->vibrato.vibratoRateChangeDelay = 1;
            break;

        case AUDIOCMD_OP_CHANNEL_COMB_FILTER_SIZE:
            channel->combFilterSize = cmd->asUbyte;
            break;

        case AUDIOCMD_OP_CHANNEL_COMB_FILTER_GAIN:
            channel->combFilterGain = cmd->asUShort;
            break;

        case AUDIOCMD_OP_CHANNEL_STEREO:
            channel->stereoData.asByte = cmd->asUbyte;
            break;

        case AUDIOCMD_OP_CHANNEL_SET_START_POS:
            channel->startSamplePos = cmd->asInt;
            break;

        case AUDIOCMD_OP_CHANNEL_SFX_STATE:
            channel->sfxState = cmd->asUInt;
            break;

        case AUDIOCMD_OP_CHANNEL_FILTER:
            filterCutoff = cmd->arg2;
            if (cmd->asUInt != 0) {
                channel->filter = cmd->asUInt;
            }
            if (channel->filter != NULL) {
                AudioHeap_LoadFilter(channel->filter, filterCutoff >> 4, filterCutoff & 0xF);
            }
            break;

        case AUDIOCMD_OP_CHANNEL_GAIN:
            channel->gain = cmd->asUbyte;
            break;

        default:
            break;
    }
}

// Unused
void func_801942BC(s32 arg0, s32 arg1, s32 arg2) {
    AUDIOCMD_GLOBAL_NOOP_1(arg0, arg1, arg2, 1);
}

// Unused
void func_80194304(void) {
    AUDIOCMD_GLOBAL_NOOP_1(0, 0, 0, 0);
}

// Unused
void func_80194328(u32 arg0, s32 arg1) {
    AUDIOCMD_GLOBAL_NOOP_2(0, 0, arg1, arg0);
}

// Unused
void AudioThread_WaitForAudioTask(void) {
    osRecvMesg(gAudioContext.taskStartQueueP, NULL, OS_MESG_NOBLOCK);
    osRecvMesg(gAudioContext.taskStartQueueP, NULL, OS_MESG_BLOCK);
}

// Unused
s32 func_8019439C(s32 seqPlayerIndex, s32 channelIndex, s32 layerIndex) {
    s32 pad;
    s32 loopEnd;
    s32 samplePosInt;

    if (!func_8019440C(seqPlayerIndex, channelIndex, layerIndex, &loopEnd, &samplePosInt)) {
        return 0;
    }
    return samplePosInt;
}

// Unused
s32 func_801943D0(s32 seqPlayerIndex, s32 channelIndex, s32 layerIndex) {
    s32 pad;
    s32 loopEnd;
    s32 samplePosInt;

    if (!func_8019440C(seqPlayerIndex, channelIndex, layerIndex, &loopEnd, &samplePosInt)) {
        return 0;
    }
    return loopEnd - samplePosInt;
}

// Only used in unused functions
s32 func_8019440C(s32 seqPlayerIndex, s32 channelIndex, s32 layerIndex, s32* loopEnd, s32* samplePosInt) {
    SequencePlayer* seqPlayer = &gAudioContext.seqPlayers[seqPlayerIndex];
    SequenceLayer* layer;
    Note* note;
    TunedSample* tunedSample;

    if (seqPlayer->enabled && seqPlayer->channels[channelIndex]->enabled) {
        layer = seqPlayer->channels[channelIndex]->layers[layerIndex];
        if (layer == NULL) {
            return false;
        }

        if (layer->enabled) {
            if (layer->note == NULL) {
                return false;
            }

            if (!layer->bit3) {
                return false;
            }

            note = layer->note;
            if (layer == note->playbackState.parentLayer) {

                if (note->noteSampleState.bitField1.isSyntheticWave == true) {
                    return false;
                }

                tunedSample = note->noteSampleState.tunedSample;
                if (tunedSample == NULL) {
                    return false;
                }
                *loopEnd = tunedSample->sample->loop->loopEnd;
                *samplePosInt = note->synthesisState.samplePosInt;
                return true;
            }
            return false;
        }
    }
    return false;
}

s32 AudioThread_GetEnabledNotesCount(void) {
    return AudioThread_CountAndReleaseNotes(0);
}
// Unused
s32 AudioThread_GetEnabledSampledNotesCount(void) {
    return AudioThread_CountAndReleaseNotes(2);
}

/**
 * @brief
 *
 * @param flags 0: count notes. 1: release all notes. 2: count sample notes 3: release sample notes
 * @return s32
 */
s32 AudioThread_CountAndReleaseNotes(s32 flags) {
    s32 ret;
    NotePlaybackState* playbackState;
    NoteSampleState* noteSampleState;
    s32 i;
    Note* note;
    TunedSample* tunedSample;

    ret = 0;
    for (i = 0; i < gAudioContext.numNotes; i++) {
        note = &gAudioContext.notes[i];
        playbackState = &note->playbackState;
        if (note->noteSampleState.bitField0.enabled) {
            noteSampleState = &note->noteSampleState;
            if (playbackState->adsr.action.s.state != ADSR_STATE_DISABLED) {
                if (flags >= 2) {
                    tunedSample = noteSampleState->tunedSample;
                    if ((tunedSample == NULL) || noteSampleState->bitField1.isSyntheticWave) {
                        continue;
                    }
                    if (tunedSample->sample->medium == MEDIUM_RAM) {
                        continue;
                    }
                }

                ret++;
                if ((flags & 1) == 1) {
                    playbackState->adsr.fadeOutVel = gAudioContext.audioBufferParameters.updatesPerFrameInv;
                    playbackState->adsr.action.s.release = true;
                }
            }
        }
    }
    return ret;
}

u32 AudioThread_NextRandom(void) {
    static u32 audRand = 0x12345678;
    static u32 D_801D6000 = 0x11111111;
    u32 count = osGetCount();

    audRand = ((gAudioContext.totalTaskCount + audRand + count) * (gAudioContext.audioRandom + 0x1234567));
    audRand = (audRand & 1) + (audRand * 2) + D_801D6000;
    D_801D6000 = count;

    return audRand;
}

void AudioThread_InitMesgQueues(void) {
    AudioThread_InitMesgQueuesInternal();
}
