#include "global.h"

#define SAMPLES_TO_OVERPRODUCE 0x10
#define EXTRA_BUFFERED_AI_SAMPLES_TARGET 0x80

#define ACMD_SND_MDE ((u32)0xF0000000)
#define ACMD_MUTE ((u32)0xF1000000)

typedef enum {
    /* 0x0 */ CHAN_UPD_UNK_0,
    /* 0x1 */ CHAN_UPD_VOL_SCALE,
    /* 0x2 */ CHAN_UPD_VOL,
    /* 0x3 */ CHAN_UPD_PAN_SIGNED,
    /* 0x4 */ CHAN_UPD_FREQ_SCALE,
    /* 0x5 */ CHAN_UPD_REVERB,
    /* 0x6 */ CHAN_UPD_SCRIPT_IO,
    /* 0x7 */ CHAN_UPD_PAN_UNSIGNED,
    /* 0x8 */ CHAN_UPD_STOP_SOMETHING2,
    /* 0x9 */ CHAN_UPD_MUTE_BEHAVE,
    /* 0xA */ CHAN_UPD_VIBE_X8,
    /* 0xB */ CHAN_UPD_VIBE_X32,
    /* 0xC */ CHAN_UPD_UNK_0F,
    /* 0xD */ CHAN_UPD_UNK_20,
    /* 0xE */ CHAN_UPD_STEREO
} ChannelUpdateType;

void Audio_ProcessPlayerCmd(SequencePlayer* seqPlayer, AudioCmd* arg1);
void Audio_ProcessChannelCmd(SequenceChannel* channel, AudioCmd* cmd);
s32 func_80194568(s32 arg0);
s32 func_8019440C(s32 playerIdx, s32 arg1, s32 arg2, s32* arg3, s32* arg4);
void Audio_SetFadeOutTimer(s32 playerIdx, s32 fadeTimer);
void Audio_SetFadeInTimer(s32 playerIdx, s32 fadeTimer);

// OoT func_800E4FE0 (AudioMgr_Retrace)
AudioTask* func_80192BE0(void) {
    return func_80192C00();
}

// OoT func_800E5000
AudioTask* func_80192C00(void) {
    static AudioTask* sWaitingAudioTask = NULL;
    u32 samplesRemainingInAi;
    s32 numAbiCmds;
    s32 pad;
    s32 j;
    s32 sp5C;
    s16* currAiBuffer;
    OSTask_t* task;
    s32 index;
    u32 sp4C;
    s32 sp48;
    s32 i;

    gAudioContext.totalTaskCount++;
    if (gAudioContext.totalTaskCount % (gAudioContext.audioBufferParameters.specUnk4) != 0) {
        if (D_80208E68 != NULL) {
            D_80208E68();
        }

        if ((gAudioContext.totalTaskCount % gAudioContext.audioBufferParameters.specUnk4) + 1 ==
            gAudioContext.audioBufferParameters.specUnk4) {
            return sWaitingAudioTask;
        } else {
            return NULL;
        }
    }

    osSendMesg(gAudioContext.taskStartQueueP, gAudioContext.totalTaskCount, OS_MESG_NOBLOCK);
    gAudioContext.rspTaskIndex ^= 1;
    gAudioContext.curAiBuffferIndex++;
    gAudioContext.curAiBuffferIndex %= 3;
    index = (gAudioContext.curAiBuffferIndex - 2 + 3) % 3;
    samplesRemainingInAi = osAiGetLength() / 4;

    if (gAudioContext.resetTimer < 16) {
        if (gAudioContext.aiBufLengths[index] != 0) {
            osAiSetNextBuffer(gAudioContext.aiBuffers[index], gAudioContext.aiBufLengths[index] * 4);
            if (gAudioContext.aiBuffers[index]) {}
            if (gAudioContext.aiBufLengths[index]) {}
        }
    }

    if (D_80208E68 != NULL) {
        D_80208E68();
    }

    sp5C = gAudioContext.curAudioFrameDmaCount;
    for (i = 0; i < gAudioContext.curAudioFrameDmaCount; i++) {
        if (osRecvMesg(&gAudioContext.currAudioFrameDmaQueue, NULL, OS_MESG_NOBLOCK) == 0) {
            sp5C--;
        }
    }

    if (sp5C != 0) {
        for (i = 0; i < sp5C; i++) {
            osRecvMesg(&gAudioContext.currAudioFrameDmaQueue, NULL, OS_MESG_BLOCK);
        }
    }

    sp48 = gAudioContext.currAudioFrameDmaQueue.validCount;
    if (sp48 != 0) {
        for (i = 0; i < sp48; i++) {
            osRecvMesg(&gAudioContext.currAudioFrameDmaQueue, NULL, OS_MESG_NOBLOCK);
        }
    }

    gAudioContext.curAudioFrameDmaCount = 0;
    AudioLoad_DecreaseSampleDmaTtls();
    AudioLoad_ProcessLoads(gAudioContext.resetStatus);
    AudioLoad_ProcessScriptLoads();

    if (gAudioContext.resetStatus != 0) {
        if (AudioHeap_ResetStep() == 0) {
            if (gAudioContext.resetStatus == 0) {
                osSendMesg(gAudioContext.audioResetQueueP, gAudioContext.audioResetSpecIdToLoad, OS_MESG_NOBLOCK);
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

    index = gAudioContext.curAiBuffferIndex;
    currAiBuffer = gAudioContext.aiBuffers[index];

    gAudioContext.aiBufLengths[index] =
        (s16)((((gAudioContext.audioBufferParameters.samplesPerFrameTarget - samplesRemainingInAi) +
                EXTRA_BUFFERED_AI_SAMPLES_TARGET) &
               ~0xF) +
              SAMPLES_TO_OVERPRODUCE);
    if (gAudioContext.aiBufLengths[index] < gAudioContext.audioBufferParameters.minAiBufferLength) {
        gAudioContext.aiBufLengths[index] = gAudioContext.audioBufferParameters.minAiBufferLength;
    }

    if (gAudioContext.aiBufLengths[index] > gAudioContext.audioBufferParameters.maxAiBufferLength) {
        gAudioContext.aiBufLengths[index] = gAudioContext.audioBufferParameters.maxAiBufferLength;
    }

    j = 0;
    if (gAudioContext.resetStatus == 0) {
        // msg = 0000RREE R = read pos, E = End Pos
        while (osRecvMesg(gAudioContext.cmdProcQueueP, (OSMesg*)&sp4C, OS_MESG_NOBLOCK) != -1) {
            if (1) {}
            if (1) {}
            if (1) {}
            Audio_ProcessCmds(sp4C);
            j++;
        }
        if ((j == 0) && (gAudioContext.cmdQueueFinished)) {
            Audio_ScheduleProcessCmds();
        }
    }

    if (gAudioSPDataPtr == (u64*)gAudioContext.curAbiCmdBuf) {
        return -1;
    }

    gAudioContext.curAbiCmdBuf =
        AudioSynth_Update(gAudioContext.curAbiCmdBuf, &numAbiCmds, currAiBuffer, gAudioContext.aiBufLengths[index]);
    gAudioContext.audioRandom = (gAudioContext.audioRandom + gAudioContext.totalTaskCount) * osGetCount();
    gAudioContext.audioRandom =
        gAudioContext.aiBuffers[index][gAudioContext.totalTaskCount & 0xFF] + gAudioContext.audioRandom;
    gWaveSamples[8] = (s16*)(((u8*)func_80192BE0) + (gAudioContext.audioRandom & 0xFFF0));

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
    } else {
        sWaitingAudioTask = gAudioContext.curTask;
        return NULL;
    }
}

void Audio_ProcessGlobalCmd(AudioCmd* cmd) {
    s32 i;
    s32 pad;

    switch (cmd->op) {
        case 0x81:
            AudioLoad_SyncLoadSeqParts(cmd->arg1, cmd->arg2, cmd->data, &gAudioContext.externalLoadQueue);
            break;
        case 0x82:
            AudioLoad_SyncInitSeqPlayer(cmd->arg0, cmd->arg1, cmd->arg2);
            Audio_SetFadeInTimer(cmd->arg0, cmd->data);
            break;
        case 0x85:
            AudioLoad_SyncInitSeqPlayerSkipTicks(cmd->arg0, cmd->arg1, cmd->data);
            Audio_SetFadeInTimer(cmd->arg0, 500);
            AudioSeq_SkipForwardSequence(&gAudioContext.seqPlayers[cmd->arg0]);
            break;
        case 0x83:
            if (gAudioContext.seqPlayers[cmd->arg0].enabled) {
                if (cmd->asInt == 0) {
                    AudioSeq_SequencePlayerDisableAsFinished(&gAudioContext.seqPlayers[cmd->arg0]);
                } else {
                    Audio_SetFadeOutTimer(cmd->arg0, cmd->asInt);
                }
            }
            break;
        case 0xF0:
            gAudioContext.soundMode = cmd->asUInt;
            break;
        case 0xF1:
            if (cmd->arg0 == 0xFF) {
                for (i = 0; i < gAudioContext.audioBufferParameters.numSequencePlayers; i++) {
                    gAudioContext.seqPlayers[i].muted = true;
                    gAudioContext.seqPlayers[i].recalculateVolume = true;
                }
            } else {
                gAudioContext.seqPlayers[cmd->arg0].muted = true;
                gAudioContext.seqPlayers[cmd->arg0].recalculateVolume = true;
            }
            break;
        case 0xF2:
            if (cmd->asUInt == 1) {
                for (i = 0; i < gAudioContext.numNotes; i++) {
                    Note* note = &gAudioContext.notes[i];
                    NoteSubEu* noteSubEu = &note->noteSubEu;

                    if (noteSubEu->bitField0.enabled && note->playbackState.unk_04 == 0) {
                        if (note->playbackState.parentLayer->channel->muteBehavior & 8) {
                            noteSubEu->bitField0.finished = true;
                        }
                    }
                }
            }

            if (cmd->arg0 == 0xFF) {
                for (i = 0; i < gAudioContext.audioBufferParameters.numSequencePlayers; i++) {
                    gAudioContext.seqPlayers[i].muted = false;
                    gAudioContext.seqPlayers[i].recalculateVolume = true;
                }
            } else {
                gAudioContext.seqPlayers[cmd->arg0].muted = false;
                gAudioContext.seqPlayers[cmd->arg0].recalculateVolume = true;
            }

            break;
        case 0xF3:
            AudioLoad_SyncLoadInstrument(cmd->arg0, cmd->arg1, cmd->arg2);
            break;
        case 0xF4:
            AudioLoad_AsyncLoadSampleBank(cmd->arg0, cmd->arg1, cmd->arg2, &gAudioContext.externalLoadQueue);
            break;
        case 0xF5:
            AudioLoad_AsyncLoadFont(cmd->arg0, cmd->arg1, cmd->arg2, &gAudioContext.externalLoadQueue);
            break;
        case 0xFC:
            AudioLoad_AsyncLoadSeq(cmd->arg0, cmd->arg1, cmd->arg2, &gAudioContext.externalLoadQueue);
            break;
        case 0xF6:
            AudioLoad_DiscardSeqFonts(cmd->arg1);
            break;
        case 0x90:
            gAudioContext.activeChannelsFlags[cmd->arg0] = cmd->asUShort;
            break;
        case 0xF9:
            gAudioContext.resetStatus = 5;
            gAudioContext.audioResetSpecIdToLoad = cmd->asUInt;
            break;
        case 0xFB:
            D_80208E68 = (void (*)(void))cmd->asUInt;
            break;
        case 0xE4:
            if (cmd->arg2 == 0xFF) {
                D_80208E70 = (s32(*)(SoundFontSample*, s32, s8, s32))cmd->asUInt;
            } else if (cmd->arg2 == 0xFE) {
                D_80208E74 = (Acmd * (*)(Acmd*, s32, s32)) cmd->asUInt;
            } else {
                gAudioContext.unk_29A8[cmd->arg2] = (u32(*)(s8, SequenceChannel*))cmd->asUInt;
            }
            break;
        case 0xE0: // Drum
        case 0xE1: // Sfx
        case 0xE2: // Instrument
            if (AudioPlayback_SetFontInstrument(cmd->op - 0xE0, cmd->arg1, cmd->arg2, cmd->data)) {}
            break;
        case 0xFE: {
            u32 temp_t7 = cmd->asUInt;
            if (temp_t7 == 1) {
                for (i = 0; i < gAudioContext.audioBufferParameters.numSequencePlayers; i++) {
                    if (gAudioContext.seqPlayers[i].enabled) {
                        AudioSeq_SequencePlayerDisableAsFinished(&gAudioContext.seqPlayers[i]);
                    }
                }
            }
            func_80194568(temp_t7);
        } break;
        case 0xE3:
            AudioHeap_PopCache(cmd->asInt);
            break;
        case 0xE5:
            func_8018FA60(cmd->arg0, cmd->arg1, cmd->arg2, cmd->data);
            break;
        case 0xE6:
            AudioHeap_SetReverbData(cmd->arg1, cmd->arg0, cmd->data, 0);
            break;
        default:
            break;
    }
}

// OoT func_800E5958  (SetFadeOutTimer)
void Audio_SetFadeOutTimer(s32 playerIdx, s32 fadeTimer) {
    SequencePlayer* seqPlayer = &gAudioContext.seqPlayers[playerIdx];

    if (fadeTimer == 0) {
        fadeTimer = 1;
    }

    seqPlayer->fadeVelocity = -(seqPlayer->fadeVolume / fadeTimer);
    seqPlayer->state = 2;
    seqPlayer->fadeTimer = fadeTimer;
}

// OoT func_800E59AC (SetFadeInTimer)
void Audio_SetFadeInTimer(s32 playerIdx, s32 fadeTimer) {
    SequencePlayer* seqPlayer;

    if (fadeTimer != 0) {
        seqPlayer = &gAudioContext.seqPlayers[playerIdx];
        seqPlayer->state = 1;
        seqPlayer->fadeTimerUnkEu = fadeTimer;
        seqPlayer->fadeTimer = fadeTimer;
        seqPlayer->fadeVolume = 0.0f;
        seqPlayer->fadeVelocity = 0.0f;
    }
}

void Audio_InitMesgQueuesInternal(void) {
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

void Audio_QueueCmd(u32 opArgs, void** data) {
    AudioCmd* cmd = &gAudioContext.cmdBuf[gAudioContext.cmdWritePos & 0xFF];

    cmd->opArgs = opArgs;
    cmd->data = *data;

    gAudioContext.cmdWritePos++;

    if (gAudioContext.cmdWritePos == gAudioContext.cmdReadPos) {
        gAudioContext.cmdWritePos--;
    }
}

void Audio_QueueCmdF32(u32 opArgs, f32 data) {
    Audio_QueueCmd(opArgs, (void**)&data);
}

void Audio_QueueCmdS32(u32 opArgs, s32 data) {
    Audio_QueueCmd(opArgs, (void**)&data);
}

void Audio_QueueCmdS8(u32 opArgs, s8 data) {
    u32 uData = data << 0x18;

    Audio_QueueCmd(opArgs, (void**)&uData);
}

void Audio_QueueCmdU16(u32 opArgs, u16 data) {
    u32 uData = data << 0x10;

    Audio_QueueCmd(opArgs, (void**)&uData);
}

s32 Audio_ScheduleProcessCmds(void) {
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

void Audio_ResetCmdQueue(void) {
    gAudioContext.cmdQueueFinished = false;
    gAudioContext.cmdReadPos = gAudioContext.cmdWritePos;
}

void Audio_ProcessCmd(AudioCmd* cmd) {
    SequencePlayer* seqPlayer;
    u16 phi_v0;
    s32 i;

    if ((cmd->op & 0xF0) >= 0xE0) {
        Audio_ProcessGlobalCmd(cmd);
        return;
    }

    if (cmd->arg0 < gAudioContext.audioBufferParameters.numSequencePlayers) {
        seqPlayer = &gAudioContext.seqPlayers[cmd->arg0];
        if (cmd->op & 0x80) {
            Audio_ProcessGlobalCmd(cmd);
            return;
        }
        if (cmd->op & 0x40) {
            Audio_ProcessPlayerCmd(seqPlayer, cmd);
            return;
        }

        if (cmd->arg1 < 0x10) {
            Audio_ProcessChannelCmd(seqPlayer->channels[cmd->arg1], cmd);
            return;
        }
        if (cmd->arg1 == 0xFF) {
            phi_v0 = gAudioContext.activeChannelsFlags[cmd->arg0];
            for (i = 0; i < ARRAY_COUNT(seqPlayer->channels); i++) {
                if (phi_v0 & 1) {
                    Audio_ProcessChannelCmd(seqPlayer->channels[i], cmd);
                }
                phi_v0 = phi_v0 >> 1;
            }
        }
    }
}

void Audio_ProcessCmds(u32 msg) {
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
            return;
        }

        cmd = &gAudioContext.cmdBuf[curCmdRdPos++ & 0xFF];
        if (cmd->op == 0xF8) {
            gAudioContext.cmdQueueFinished = true;
            return;
        }

        Audio_ProcessCmd(cmd);
        cmd->op = 0;
    }
}

// OoT func_800E5E20
u32 func_80193BA0(u32* out) {
    u32 msg;

    if (osRecvMesg(&gAudioContext.externalLoadQueue, (OSMesg*)&msg, OS_MESG_NOBLOCK) == -1) {
        *out = 0;
        return 0;
    }

    *out = msg & 0xFFFFFF;
    return msg >> 0x18;
}

// OoT func_800E5E84
u8* AudioCmd_GetFontsForSequence(s32 seqId, u32* outNumFonts) {
    return AudioLoad_GetFontsForSequence(seqId, outNumFonts);
}

// OoT func_800E5EA4
void func_80193C24(s32 fontId, u32* sampleBankId1, u32* sampleBankId2) {
    *sampleBankId1 = gAudioContext.soundFonts[fontId].sampleBankId1;
    *sampleBankId2 = gAudioContext.soundFonts[fontId].sampleBankId2;
}

// OoT func_800E5EDC
s32 func_80193C5C(void) {
    s32 pad;
    s32 sp18;

    if (osRecvMesg(gAudioContext.audioResetQueueP, (OSMesg*)&sp18, OS_MESG_NOBLOCK) == -1) {
        return 0;
    } else if (gAudioContext.audioResetSpecIdToLoad != sp18) {
        return -1;
    } else {
        return 1;
    }
}

// OoT func_800E5F34
void func_80193CB4(void) {
    // macro?
    // clang-format off
    s32 chk = -1; s32 sp28; do {} while (osRecvMesg(gAudioContext.audioResetQueueP, (OSMesg*)&sp28, OS_MESG_NOBLOCK) != chk);
    // clang-format on
}

// OoT func_800E5F88
s32 func_80193D08(s32 resetPreloadID) {
    s32 resetStatus;
    OSMesg msg;
    s32 pad;

    func_80193CB4();
    resetStatus = gAudioContext.resetStatus;
    if (resetStatus != 0) {
        Audio_ResetCmdQueue();
        if (gAudioContext.audioResetSpecIdToLoad == resetPreloadID) {
            return -2;
        } else if (resetStatus > 2) {
            gAudioContext.audioResetSpecIdToLoad = resetPreloadID;
            return -3;
        } else {
            osRecvMesg(gAudioContext.audioResetQueueP, &msg, OS_MESG_BLOCK);
        }
    }

    func_80193CB4();
    Audio_QueueCmdS32(0xF9000000, resetPreloadID);

    return Audio_ScheduleProcessCmds();
}

void Audio_PreNMIInternal(void) {
    gAudioContext.resetTimer = 1;
    if (gAudioContextInitalized) {
        func_80193D08(0);
        gAudioContext.resetStatus = 0;
    }
}

s8 Audio_GetChannelIO(s32 playerIdx, s32 channelIdx, s32 port) {
    SequencePlayer* seqPlayer = &gAudioContext.seqPlayers[playerIdx];
    SequenceChannel* channel;

    if (seqPlayer->enabled) {
        channel = seqPlayer->channels[channelIdx];
        return channel->soundScriptIO[port];
    } else {
        return -1;
    }
}

s8 Audio_GetPlayerIO(s32 playerIdx, s32 port) {
    return gAudioContext.seqPlayers[playerIdx].soundScriptIO[port];
}

void Audio_InitExternalHeap(void* addr, size_t size) {
    AudioHeap_AllocPoolInit(&gAudioContext.externalPool, addr, size);
}

void Audio_DestroyExternalHeap(void) {
    gAudioContext.externalPool.startAddr = NULL;
}

// OoT func_800E6128
void Audio_ProcessPlayerCmd(SequencePlayer* seqPlayer, AudioCmd* cmd) {
    f32 fadeVolume;
    switch (cmd->op) {
        case 0x41:
            if (seqPlayer->fadeVolumeScale != cmd->asFloat) {
                seqPlayer->fadeVolumeScale = cmd->asFloat;
                seqPlayer->recalculateVolume = true;
            }
            return;
        case 0x47:
            seqPlayer->tempo = cmd->asInt * 0x30;
            return;
        case 0x49:
            seqPlayer->unk_0C = cmd->asInt * 0x30;
            return;
        case 0x4E:
            seqPlayer->unk_0C = cmd->asInt;
            return;
        case 0x48:
            seqPlayer->transposition = cmd->asSbyte;
            return;
        case 0x46:
            seqPlayer->soundScriptIO[cmd->arg2] = cmd->asSbyte;
            return;
        case 0x4A:
            fadeVolume = (s32)cmd->arg1 / 127.0f;
            goto block_11;
        case 0x4B:
            fadeVolume = ((s32)cmd->arg1 / 100.0f) * seqPlayer->fadeVolume;
        block_11:
            if (seqPlayer->state != 2) {
                seqPlayer->volume = seqPlayer->fadeVolume;
                if (cmd->asInt == 0) {
                    seqPlayer->fadeVolume = fadeVolume;
                } else {
                    s32 tmp = cmd->asInt;

                    seqPlayer->state = 0;
                    seqPlayer->fadeTimer = tmp;
                    seqPlayer->fadeVelocity = (fadeVolume - seqPlayer->fadeVolume) / tmp;
                }
            }
            return;
        case 0x4C:
            if (seqPlayer->state != 2) {
                if (cmd->asInt == 0) {
                    seqPlayer->fadeVolume = seqPlayer->volume;
                } else {
                    s32 tmp = cmd->asInt;

                    seqPlayer->state = 0;
                    seqPlayer->fadeTimer = tmp;
                    seqPlayer->fadeVelocity = (seqPlayer->volume - seqPlayer->fadeVolume) / tmp;
                }
            }
            return;
        case 0x4D:
            seqPlayer->bend = cmd->asFloat;
            if (seqPlayer->bend == 1.0f) {
                seqPlayer->applyBend = false;
            } else {
                seqPlayer->applyBend = true;
            }
    }
}

// OoT func_800E6300
void Audio_ProcessChannelCmd(SequenceChannel* channel, AudioCmd* cmd) {
    switch (cmd->op) {
        u8 new_var;

        case CHAN_UPD_VOL_SCALE:
            if (channel->volumeScale != cmd->asFloat) {
                channel->volumeScale = cmd->asFloat;
                channel->changes.s.volume = true;
            }
            return;
        case CHAN_UPD_VOL:
            if (channel->volume != cmd->asFloat) {
                channel->volume = cmd->asFloat;
                channel->changes.s.volume = true;
            }
            return;
        case CHAN_UPD_PAN_SIGNED:
            if (channel->newPan != cmd->asSbyte) {
                channel->newPan = cmd->asSbyte;
                channel->changes.s.pan = true;
            }
            return;
        case CHAN_UPD_PAN_UNSIGNED:
            if (channel->newPan != cmd->asSbyte) {
                channel->panChannelWeight = cmd->asSbyte;
                channel->changes.s.pan = true;
            }
            return;
        case CHAN_UPD_FREQ_SCALE:
            if (channel->freqScale != cmd->asFloat) {
                channel->freqScale = cmd->asFloat;
                channel->changes.s.freqScale = true;
            }
            return;
        case CHAN_UPD_REVERB:
            if (channel->reverb != cmd->asSbyte) {
                channel->reverb = cmd->asSbyte;
            }
            return;
        case 0x11:
            if (channel->reverbIndex != cmd->asSbyte) {
                channel->reverbIndex = cmd->asSbyte;
            }
            return;
        case 0x12:
            channel->unk_10 = cmd->asSbyte;
            return;
        case CHAN_UPD_SCRIPT_IO:
            if (cmd->arg2 < ARRAY_COUNT(channel->soundScriptIO)) {
                channel->soundScriptIO[cmd->arg2] = cmd->asSbyte;
            }
            return;
        case CHAN_UPD_STOP_SOMETHING2:
            channel->stopSomething2 = cmd->asSbyte;
            return;
        case CHAN_UPD_MUTE_BEHAVE:
            channel->muteBehavior = cmd->asSbyte;
            return;
        case CHAN_UPD_VIBE_X8:
            channel->vibrato.vibratoExtentTarget = cmd->asUbyte * 8;
            channel->vibrato.vibratoExtentChangeDelay = 1;
            return;
        case CHAN_UPD_VIBE_X32:
            channel->vibrato.vibratoRateTarget = cmd->asUbyte * 32;
            channel->vibrato.vibratoRateChangeDelay = 1;
            return;
        case CHAN_UPD_UNK_0F:
            channel->unk_0F = cmd->asUbyte;
            return;
        case CHAN_UPD_UNK_20:
            channel->unk_20 = cmd->asUShort;
            return;
        case CHAN_UPD_STEREO:
            channel->stereo.asByte = cmd->asUbyte;
            return;
        case 0xF:
            channel->unk_DC = cmd->asInt;
            return;
        case 0x10:
            channel->sfxState = cmd->asUInt;
            return;
        case 0x13:
            new_var = cmd->arg2;
            if (cmd->asUInt != 0) {
                channel->filter = cmd->asUInt;
            }
            if (channel->filter != NULL) {
                AudioHeap_LoadFilter(channel->filter, new_var >> 4, new_var & 0xF);
            }
            return;
        case 0x14:
            channel->unk_0C = cmd->asUbyte;
            return;
    }
}

// OoT func_800E64B0
void func_801942BC(s32 arg0, s32 arg1, s32 arg2) {
    Audio_QueueCmdS32(((arg0 & 0xFF) << 0x10) | 0xFA000000 | ((arg1 & 0xFF) << 8) | (arg2 & 0xFF), 1);
}

// OoT func_800E64F8
void func_80194304(void) {
    Audio_QueueCmdS32(0xFA000000, 0);
}

// OoT func_800E651C
void func_80194328(u32 arg0, s32 arg1) {
    Audio_QueueCmdS32((arg1 & 0xFF) | 0xFD000000, arg0);
}

void Audio_WaitForAudioTask(void) {
    osRecvMesg(gAudioContext.taskStartQueueP, NULL, OS_MESG_NOBLOCK);
    osRecvMesg(gAudioContext.taskStartQueueP, NULL, OS_MESG_BLOCK);
}

// New to MM
s32 func_8019439C(s32 playerIdx, s32 arg1, s32 arg2) {
    s32 pad;
    s32 sp28;
    s32 sp24;

    if (!func_8019440C(playerIdx, arg1, arg2, &sp28, &sp24)) {
        return 0;
    }
    return sp24;
}

// New to MM
s32 func_801943D0(s32 playerIdx, s32 arg1, s32 arg2) {
    s32 pad;
    s32 sp28;
    s32 sp24;

    if (!func_8019440C(playerIdx, arg1, arg2, &sp28, &sp24)) {
        return 0;
    }
    return sp28 - sp24;
}

// OoT func_800E6590
s32 func_8019440C(s32 playerIdx, s32 arg1, s32 arg2, s32* arg3, s32* arg4) {
    SequencePlayer* seqPlayer;
    SequenceLayer* layer;
    Note* note;
    SoundFontSound* sound;
    s32 loopEnd;
    s32 samplePos;

    seqPlayer = &gAudioContext.seqPlayers[playerIdx];
    if (seqPlayer->enabled && seqPlayer->channels[arg1]->enabled) {
        layer = seqPlayer->channels[arg1]->layers[arg2];
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

                if (note->noteSubEu.bitField1.isSyntheticWave == true) {
                    return false;
                }

                sound = note->noteSubEu.sound.soundFontSound;
                if (sound == NULL) {
                    return false;
                }
                *arg3 = sound->sample->loop->end;
                *arg4 = note->synthesisState.samplePosInt;
                return true;
            }
            return false;
        }
    }
    return false;
}

// OoT func_800E6680
s32 func_80194528(void) {
    return func_80194568(0);
}

// OoT func_800E66A0
void func_80194548(void) {
    func_80194568(2);
}

// OoT func_800E66C0
s32 func_80194568(s32 arg0) {
    s32 ret;
    NotePlaybackState* playbackState;
    NoteSubEu* noteSubEu;
    s32 i;
    Note* note;
    SoundFontSound* sound;

    ret = 0;
    for (i = 0; i < gAudioContext.numNotes; i++) {
        note = &gAudioContext.notes[i];
        playbackState = &note->playbackState;
        if (note->noteSubEu.bitField0.enabled) {
            noteSubEu = &note->noteSubEu;
            if (playbackState->adsr.action.s.state != 0) {
                if (arg0 >= 2) {
                    sound = noteSubEu->sound.soundFontSound;
                    if (sound == NULL || noteSubEu->bitField1.isSyntheticWave) {
                        continue;
                    }
                    if (sound->sample->medium == MEDIUM_RAM) {
                        continue;
                    }
                }

                ret++;
                if ((arg0 & 1) == 1) {
                    playbackState->adsr.fadeOutVel = gAudioContext.audioBufferParameters.updatesPerFrameInv;
                    playbackState->adsr.action.s.release = true;
                }
            }
        }
    }
    return ret;
}

u32 Audio_NextRandom(void) {
    static u32 audRand = 0x12345678;
    static u32 D_801D6000 = 0x11111111;
    u32 count = osGetCount();

    audRand = ((gAudioContext.totalTaskCount + audRand + count) * (gAudioContext.audioRandom + 0x1234567));
    audRand = (audRand & 1) + (audRand * 2) + D_801D6000;
    D_801D6000 = count;

    return audRand;
}

void Audio_InitMesgQueues(void) {
    Audio_InitMesgQueuesInternal();
}
