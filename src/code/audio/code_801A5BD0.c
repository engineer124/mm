#include "global.h"

SoundRequest sSoundRequests[0x100];
u8 sSoundBankListEnd[7];
u8 sSoundBankFreeListStart[7];
u8 sSoundBankUnused[7];
ActiveSound gActiveSounds[7][3];
u8 sCurSfxPlayerChannelIdx;
u8 gSoundBankMuted[7];
UnusedBankLerp sUnusedBankLerp[7];

void Audio_SetSoundBanksMute(u16 muteMask) {
    u8 bankId;

    for (bankId = 0; bankId < 7; bankId++) {
        if (muteMask & 1) {
            gSoundBankMuted[bankId] = true;
        } else {
            gSoundBankMuted[bankId] = false;
        }
        muteMask = muteMask >> 1;
    }
}

void Audio_QueueSeqCmdMute(u8 channelIdx) {
    D_801DB4A0 |= (1 << channelIdx);
    Audio_SetVolScale(0, 2, 0x40, 0xF);
    Audio_SetVolScale(3, 2, 0x40, 0xF);
}

void Audio_ClearBGMMute(u8 channelIdx) {
    D_801DB4A0 &= ((1 << channelIdx) ^ 0xFFFF);
    if (D_801DB4A0 == 0) {
        Audio_SetVolScale(0, 2, 0x7F, 0xF);
        Audio_SetVolScale(3, 2, 0x7F, 0xF);
    }
}

// a4 is often the same as freqScale. (u8)(*a4 * 127.0f) is sent to script on IO port 2
void Audio_PlaySfxGeneral(u16 sfxId, Vec3f* pos, u8 token, f32* freqScale, f32* vol, s8* reverbAdd) {
    u8 i;
    SoundRequest* reqWrite;
    SoundRequest* reqRead;

    if (!gSoundBankMuted[SFX_BANK_SHIFT(sfxId)]) {
        reqWrite = &sSoundRequests[sSoundRequestWriteIndex];

        for (i = sSoundRequestReadIndex; sSoundRequestWriteIndex != i; i++) {
            reqRead = &sSoundRequests[i];
            if (reqRead->pos == pos && reqRead->sfxId == sfxId) {
                return;
            }
        }

        reqWrite->sfxId = sfxId;
        reqWrite->pos = pos;
        reqWrite->token = token;
        reqWrite->freqScale = freqScale;
        reqWrite->vol = vol;
        reqWrite->reverbAdd = reverbAdd;
        sSoundRequestWriteIndex++;
    }
}

void Audio_RemoveMatchingSoundRequests(u8 aspect, SoundBankEntry* cmp) {
    SoundRequest* req;
    s32 remove;
    u8 i = sSoundRequestReadIndex;

    for (; i != sSoundRequestWriteIndex; i++) {
        remove = false;
        req = &sSoundRequests[i];
        switch (aspect) {
            case 0:
                if (SFX_BANK_MASK(req->sfxId) == SFX_BANK_MASK(cmp->sfxId)) {
                    remove = true;
                }
                break;
            case 1:
                if (SFX_BANK_MASK(req->sfxId) == SFX_BANK_MASK(cmp->sfxId) && (&req->pos->x == cmp->posX)) {
                    remove = true;
                }
                break;
            case 2:
                if (&req->pos->x == cmp->posX) {
                    remove = true;
                }
                break;
            case 3:
                if (&req->pos->x == cmp->posX && req->sfxId == cmp->sfxId) {
                    remove = true;
                }
                break;
            case 4:
                if (req->token == cmp->token && req->sfxId == cmp->sfxId) {
                    remove = true;
                }
                break;
            case 5:
                if (req->sfxId == cmp->sfxId) {
                    remove = true;
                }
                break;
        }
        if (remove) {
            req->sfxId = 0;
        }
    }
}

void Audio_ProcessSoundRequest(void) {
    u16 sfxId;
    u8 count;
    u8 index;
    SoundRequest* req;
    SoundBankEntry* entry;
    SoundParams* soundParams;
    s32 bankId;
    u8 evictImportance;
    u8 evictIndex;

    req = &sSoundRequests[sSoundRequestReadIndex];
    evictIndex = 0x80;
    if (req->sfxId == 0) {
        return;
    }
    bankId = SFX_BANK(req->sfxId);
    count = 0;
    index = gSoundBanks[bankId][0].next;
    while (index != 0xFF && index != 0) {
        if (gSoundBanks[bankId][index].posX == &req->pos->x) {
            if ((gSoundParams[SFX_BANK_SHIFT(req->sfxId)][SFX_INDEX(req->sfxId)].params & 0x20) &&
                gSoundParams[SFX_BANK_SHIFT(req->sfxId)][SFX_INDEX(req->sfxId)].importance ==
                    gSoundBanks[bankId][index].sfxImportance) {
                return;
            }
            if (gSoundBanks[bankId][index].sfxId == req->sfxId) {
                count = gUsedChannelsPerBank[gSfxChannelLayout][bankId];
            } else {
                if (count == 0) {
                    evictIndex = index;
                    sfxId = gSoundBanks[bankId][index].sfxId & 0xFFFF;
                    evictImportance = gSoundParams[SFX_BANK_SHIFT(sfxId)][SFX_INDEX(sfxId)].importance;
                } else if (gSoundBanks[bankId][index].sfxImportance < evictImportance) {
                    evictIndex = index;
                    sfxId = gSoundBanks[bankId][index].sfxId & 0xFFFF;
                    evictImportance = gSoundParams[SFX_BANK_SHIFT(sfxId)][SFX_INDEX(sfxId)].importance;
                }
                count++;
                if (count == gUsedChannelsPerBank[gSfxChannelLayout][bankId]) {
                    if (gSoundParams[SFX_BANK_SHIFT(req->sfxId)][SFX_INDEX(req->sfxId)].importance >= evictImportance) {
                        index = evictIndex;
                    } else {
                        index = 0;
                    }
                }
            }
            if (count == gUsedChannelsPerBank[gSfxChannelLayout][bankId]) {
                soundParams = &gSoundParams[SFX_BANK_SHIFT(req->sfxId)][SFX_INDEX(req->sfxId)];
                if ((req->sfxId & 0xC00) || (soundParams->unk_01 & 1) || (index == evictIndex)) {
                    if ((gSoundBanks[bankId][index].sfxParams & 8) &&
                        gSoundBanks[bankId][index].state != SFX_STATE_QUEUED) {
                        Audio_ClearBGMMute(gSoundBanks[bankId][index].channelIdx);
                    }
                    gSoundBanks[bankId][index].token = req->token;
                    gSoundBanks[bankId][index].sfxId = req->sfxId;
                    gSoundBanks[bankId][index].state = SFX_STATE_QUEUED;
                    gSoundBanks[bankId][index].freshness = 2;
                    gSoundBanks[bankId][index].freqScale = req->freqScale;
                    gSoundBanks[bankId][index].vol = req->vol;
                    gSoundBanks[bankId][index].reverbAdd = req->reverbAdd;
                    gSoundBanks[bankId][index].sfxParams = soundParams->params;
                    gSoundBanks[bankId][index].sfxUnk01 = soundParams->unk_01;
                    gSoundBanks[bankId][index].sfxImportance = soundParams->importance;
                } else if (gSoundBanks[bankId][index].state == SFX_STATE_PLAYING_2) {
                    gSoundBanks[bankId][index].state = SFX_STATE_PLAYING_1;
                }
                index = 0;
            }
        }
        if (index != 0) {
            index = gSoundBanks[bankId][index].next;
        }
    }
    if (gSoundBanks[bankId][sSoundBankFreeListStart[bankId]].next != 0xFF && index != 0) {
        index = sSoundBankFreeListStart[bankId];
        entry = &gSoundBanks[bankId][index];
        entry->posX = &req->pos->x;
        entry->posY = &req->pos->y;
        entry->posZ = &req->pos->z;
        entry->token = req->token;
        entry->freqScale = req->freqScale;
        entry->vol = req->vol;
        entry->reverbAdd = req->reverbAdd;
        soundParams = &gSoundParams[SFX_BANK_SHIFT(req->sfxId)][SFX_INDEX(req->sfxId)];
        entry->sfxParams = soundParams->params;
        entry->sfxUnk01 = soundParams->unk_01;
        entry->sfxImportance = soundParams->importance;
        entry->sfxId = req->sfxId;
        entry->state = SFX_STATE_QUEUED;
        entry->freshness = 2;
        entry->prev = sSoundBankListEnd[bankId];
        gSoundBanks[bankId][sSoundBankListEnd[bankId]].next = sSoundBankFreeListStart[bankId];
        sSoundBankListEnd[bankId] = sSoundBankFreeListStart[bankId];
        sSoundBankFreeListStart[bankId] = gSoundBanks[bankId][sSoundBankFreeListStart[bankId]].next;
        gSoundBanks[bankId][sSoundBankFreeListStart[bankId]].prev = 0xFF;
        entry->next = 0xFF;
    }
}

// OoT func_800F7B54
void Audio_RemoveSoundBankEntry(u8 bankId, u8 entryIndex) {
    SoundBankEntry* entry = &gSoundBanks[bankId][entryIndex];
    u8 i;

    if (entry->sfxParams & 8) {
        Audio_ClearBGMMute(entry->channelIdx);
    }
    if (entryIndex == sSoundBankListEnd[bankId]) {
        sSoundBankListEnd[bankId] = entry->prev;
    } else {
        gSoundBanks[bankId][entry->next].prev = entry->prev;
    }
    gSoundBanks[bankId][entry->prev].next = entry->next;
    entry->next = sSoundBankFreeListStart[bankId];
    entry->prev = 0xFF;
    gSoundBanks[bankId][sSoundBankFreeListStart[bankId]].prev = entryIndex;
    sSoundBankFreeListStart[bankId] = entryIndex;
    entry->state = SFX_STATE_EMPTY;

    for (i = 0; i < gChannelsPerBank[gSfxChannelLayout][bankId]; i++) {
        if (gActiveSounds[bankId][i].entryIndex == entryIndex) {
            gActiveSounds[bankId][i].entryIndex = 0xFF;
            i = gChannelsPerBank[gSfxChannelLayout][bankId];
        }
    }
}

// OoT func_800F7CEC
void Audio_ChooseActiveSounds(u8 bankId) {
    u8 numChosenSounds;
    u8 numChannels;
    u8 entryIndex;
    u8 i;
    u8 j;
    u8 k;
    u8 sfxImportance;
    u8 needNewSound;
    u8 chosenEntryIndex;
    u16 temp3;
    s32 pad;
    SoundBankEntry* entry;
    ActiveSound chosenSounds[MAX_CHANNELS_PER_BANK];
    ActiveSound* activeSound;
    f32 tempf1;
    f32 tempf2;

    numChosenSounds = 0;
    for (i = 0; i < MAX_CHANNELS_PER_BANK; i++) {
        chosenSounds[i].priority = 0x7FFFFFFF;
        chosenSounds[i].entryIndex = 0xFF;
    }
    entryIndex = gSoundBanks[bankId][0].next;
    k = 0;
    while (entryIndex != 0xFF) {
        if ((SFX_STATE_QUEUED == gSoundBanks[bankId][entryIndex].state) &&
            (gSoundBanks[bankId][entryIndex].sfxId & 0xC00)) {
            gSoundBanks[bankId][entryIndex].freshness--;
        } else if (!(gSoundBanks[bankId][entryIndex].sfxId & 0xC00) &&
                   (gSoundBanks[bankId][entryIndex].state == SFX_STATE_PLAYING_2)) {
            Audio_QueueCmdS8((gSoundBanks[bankId][entryIndex].channelIdx << 8) | 0x6020000, 0);
            Audio_RemoveSoundBankEntry(bankId, entryIndex);
        }
        if (gSoundBanks[bankId][entryIndex].freshness == 0) {
            Audio_RemoveSoundBankEntry(bankId, entryIndex);
        } else if (gSoundBanks[bankId][entryIndex].state != SFX_STATE_EMPTY) {
            entry = &gSoundBanks[bankId][entryIndex];

            if (&gDefaultSfxPos.x == entry[0].posX) {
                entry->dist = 0.0f;
            } else {
                tempf1 = *entry->posY * 1;
                tempf2 = *entry->posX * 0.5f;
                entry->dist = (SQ(tempf2) + SQ(tempf1) + SQ(*entry->posZ)) / 10.0f;
            }
            sfxImportance = entry->sfxImportance;
            if (entry->sfxParams & 0x10) {
                entry->priority = SQ(0xFF - sfxImportance) * SQ(76);
            } else {
                if (entry->dist > 0x7FFFFFD0) {
                    entry->dist = 0x70000008;
                }
                entry->priority = (u32)entry->dist + (SQ(0xFF - sfxImportance) * SQ(76));
                if (*entry->posZ < 0.0f) {
                    entry->priority += (s32)(-*entry->posZ * 6.0f);
                }
            }
            if (entry->dist > SQ(1e5f)) {
                if (entry->state == SFX_STATE_PLAYING_1) {
                    Audio_QueueCmdS8((entry->channelIdx << 8) | 0x6020000, 0);
                    if (entry->sfxId & 0xC00) {
                        Audio_RemoveSoundBankEntry(bankId, entryIndex);
                        entryIndex = k;
                    }
                }
            } else {
                numChannels = gChannelsPerBank[gSfxChannelLayout][bankId];
                for (i = 0; i < numChannels; i++) {
                    if (chosenSounds[i].priority >= entry->priority) {
                        if (numChosenSounds < gChannelsPerBank[gSfxChannelLayout][bankId]) {
                            numChosenSounds++;
                        }
                        for (j = numChannels - 1; j > i; j--) {
                            chosenSounds[j].priority = chosenSounds[j - 1].priority;
                            chosenSounds[j].entryIndex = chosenSounds[j - 1].entryIndex;
                        }
                        chosenSounds[i].priority = entry->priority;
                        chosenSounds[i].entryIndex = entryIndex;
                        i = numChannels; // "break;"
                    }
                }
            }
            k = entryIndex;
        }
        entryIndex = gSoundBanks[bankId][k].next;
    }
    for (i = 0; i < numChosenSounds; i++) {
        entry = &gSoundBanks[bankId][chosenSounds[i].entryIndex];
        if (entry->state == SFX_STATE_QUEUED) {
            entry->state = SFX_STATE_READY;
        } else if (entry->state == SFX_STATE_PLAYING_1) {
            entry->state = SFX_STATE_PLAYING_REFRESH;
        }
    }

    // Pick something to play for all channels.
    numChannels = gChannelsPerBank[gSfxChannelLayout][bankId];
    for (i = 0; i < numChannels; i++) {
        needNewSound = false;
        activeSound = &gActiveSounds[bankId][i];

        if (activeSound->entryIndex == 0xFF) {
            needNewSound = true;
        } else {
            entry = &gSoundBanks[bankId][activeSound[0].entryIndex];
            if (entry->state == SFX_STATE_PLAYING_1) {
                if (entry->sfxId & 0xC00) {
                    Audio_RemoveSoundBankEntry(bankId, activeSound->entryIndex);
                } else {
                    entry->state = SFX_STATE_QUEUED;
                    entry->freshness = 0x80;
                }
                needNewSound = true;
            } else if (entry->state == SFX_STATE_EMPTY) {
                activeSound->entryIndex = 0xFF;
                needNewSound = true;
            } else {
                // Sound is already playing as it should, nothing to do.
                for (j = 0; j < numChannels; j++) {
                    if (activeSound->entryIndex == chosenSounds[j].entryIndex) {
                        chosenSounds[j].entryIndex = 0xFF;
                        j = numChannels;
                    }
                }
                numChosenSounds--;
            }
        }

        if (needNewSound == true) {
            for (j = 0; j < numChannels; j++) {
                chosenEntryIndex = chosenSounds[j].entryIndex;
                if ((chosenEntryIndex != 0xFF) &&
                    (gSoundBanks[bankId][chosenEntryIndex].state != SFX_STATE_PLAYING_REFRESH)) {
                    for (k = 0; k < numChannels; k++) {
                        if (chosenEntryIndex == gActiveSounds[bankId][k].entryIndex) {
                            needNewSound = false;
                            k = numChannels; // "break;"
                        }
                    }
                    if (needNewSound == true) {
                        activeSound->entryIndex = chosenEntryIndex;

                        chosenSounds[j].entryIndex = 0xFF;

                        j = numChannels + 1;
                        numChosenSounds--;
                    }
                }
            }
            if (j == numChannels) {
                // nothing found
                activeSound->entryIndex = 0xFF;
            }
        }
    }
}

// OoT func_800F8480
void Audio_PlayActiveSounds(u8 bankId) {
    u8 entryIndex;
    SequenceChannel* channel;
    SoundBankEntry* entry;
    u8 i;
    u8 new_var;

    for (i = 0; i < gChannelsPerBank[gSfxChannelLayout][bankId]; i++) {
        entryIndex = gActiveSounds[bankId][i].entryIndex;
        if (entryIndex != 0xFF) {
            entry = &gSoundBanks[bankId][entryIndex];
            channel = gAudioContext.seqPlayers[2].channels[sCurSfxPlayerChannelIdx];
            if (entry->state == SFX_STATE_READY) {
                entry->channelIdx = sCurSfxPlayerChannelIdx;

                if (entry->sfxParams & 8) {
                    Audio_QueueSeqCmdMute(sCurSfxPlayerChannelIdx);
                }

                if (entry->sfxParams & 0xC0) {
                    switch (entry->sfxParams & 0xC0) {
                        case 0x40:
                            entry->unk_2F = Audio_NextRandom() & 0xF;
                            break;
                        case 0x80:
                            entry->unk_2F = Audio_NextRandom() & 0x1F;
                            break;
                        case 0xC0:
                            entry->unk_2F = Audio_NextRandom() & 0x3F;
                            break;
                        default:
                            entry->unk_2F = 0;
                            break;
                    }
                }
                Audio_SetSfxProperties(bankId, entryIndex, sCurSfxPlayerChannelIdx);
                Audio_QueueCmdS8(0x06020000 | ((sCurSfxPlayerChannelIdx & 0xFF) << 8), 1);
                Audio_QueueCmdS8(0x06020000 | ((sCurSfxPlayerChannelIdx & 0xFF) << 8) | 4, entry->sfxId & 0xFF);

                if (D_801D6600[bankId]) {
                    new_var = ((u8)((entry->sfxId & 0x300) >> 7) + (u8)((entry->sfxId & 0xFF) >> 7));
                } else {
                    new_var = 0;
                }

                if ((entry->sfxParams & 0x100) && (entry->freshness == 0x80)) {
                    new_var += 0x80;
                }

                if (gIsLargeSoundBank[bankId]) {
                    Audio_QueueCmdS8(0x06020000 | ((sCurSfxPlayerChannelIdx & 0xFF) << 8) | 5, new_var);
                }

                if (entry->sfxId & 0xC00) {
                    entry->state = SFX_STATE_PLAYING_1;
                } else {
                    entry->state = SFX_STATE_PLAYING_2;
                }
            } else if ((u8)channel->soundScriptIO[1] == 0xFF) {
                Audio_RemoveSoundBankEntry(bankId, entryIndex);
            } else if (entry->state == SFX_STATE_PLAYING_REFRESH) {
                Audio_SetSfxProperties(bankId, entryIndex, sCurSfxPlayerChannelIdx);
                if (entry->sfxId & 0xC00) {
                    entry->state = SFX_STATE_PLAYING_1;
                } else {
                    entry->state = SFX_STATE_PLAYING_2;
                }
            }
        }
        sCurSfxPlayerChannelIdx++;
    }
}

// OoT func_800F87A0
void Audio_StopSfxByBank(u8 bankId) {
    SoundBankEntry* entry;
    s32 pad;
    SoundBankEntry cmp;
    u8 entryIndex = gSoundBanks[bankId][0].next;

    while (entryIndex != 0xFF) {
        entry = &gSoundBanks[bankId][entryIndex];
        if (entry->state >= SFX_STATE_PLAYING_REFRESH) {
            Audio_QueueCmdS8(0x06020000 | ((entry->channelIdx & 0xFF) << 8), 0);
        }
        if (entry->state != SFX_STATE_EMPTY) {
            Audio_RemoveSoundBankEntry(bankId, entryIndex);
        }
        entryIndex = gSoundBanks[bankId][0].next;
    }
    cmp.sfxId = bankId << 12;
    Audio_RemoveMatchingSoundRequests(0, &cmp);
}

// OoT func_800F8884
void func_801A7168(u8 bankId, Vec3f* pos) {
    SoundBankEntry* entry;
    u8 entryIndex = gSoundBanks[bankId][0].next;
    u8 prevEntryIndex = 0;

    while (entryIndex != 0xFF) {
        entry = &gSoundBanks[bankId][entryIndex];
        if (entry->posX == &pos->x) {
            if (entry->state >= SFX_STATE_PLAYING_REFRESH) {
                Audio_QueueCmdS8(0x06020000 | ((entry->channelIdx & 0xFF) << 8), 0);
            }
            if (entry->state != SFX_STATE_EMPTY) {
                Audio_RemoveSoundBankEntry(bankId, entryIndex);
            }
        } else {
            prevEntryIndex = entryIndex;
        }
        entryIndex = gSoundBanks[bankId][prevEntryIndex].next;
    }
}

// OoT func_800F89A0
void Audio_StopSfxByPosAndBank(u8 bankId, Vec3f* pos) {
    SoundBankEntry cmp;

    func_801A7168(bankId, pos);
    cmp.sfxId = bankId << 12;
    cmp.posX = &pos->x;
    Audio_RemoveMatchingSoundRequests(1, &cmp);
}

// OoT func_800F89E8
void Audio_StopSfxByPos(Vec3f* pos) {
    u8 i;
    SoundBankEntry cmp;

    for (i = 0; i < ARRAY_COUNT(gSoundBanks); i++) {
        func_801A7168(i, pos);
    }
    cmp.posX = &pos->x;
    Audio_RemoveMatchingSoundRequests(2, &cmp);
}

// OoT func_800F8A44
void Audio_StopSfxByPosAndId(Vec3f* pos, u16 sfxId) {
    SoundBankEntry* entry;
    u8 entryIndex = gSoundBanks[SFX_BANK(sfxId)][0].next;
    u8 prevEntryIndex = 0;
    SoundBankEntry cmp;

    while (entryIndex != 0xFF) {
        entry = &gSoundBanks[SFX_BANK(sfxId)][entryIndex];
        if (entry->posX == &pos->x && entry->sfxId == sfxId) {
            if (entry->state >= SFX_STATE_PLAYING_REFRESH) {
                Audio_QueueCmdS8(0x06020000 | ((entry->channelIdx & 0xFF) << 8), 0);
            }
            if (entry->state != SFX_STATE_EMPTY) {
                Audio_RemoveSoundBankEntry(SFX_BANK(sfxId), entryIndex);
            }
            entryIndex = 0xFF;
        } else {
            prevEntryIndex = entryIndex;
        }
        if (entryIndex != 0xFF) {
            entryIndex = gSoundBanks[SFX_BANK(sfxId)][prevEntryIndex].next;
        }
    }
    cmp.posX = &pos->x;
    cmp.sfxId = sfxId;
    Audio_RemoveMatchingSoundRequests(3, &cmp);
}

// OoT func_800F8BA0
void Audio_StopSfxByTokenAndId(u8 token, u16 sfxId) {
    SoundBankEntry* entry;
    u8 entryIndex = gSoundBanks[SFX_BANK(sfxId)][0].next;
    u8 prevEntryIndex = 0;
    SoundBankEntry cmp;

    while (entryIndex != 0xFF) {
        entry = &gSoundBanks[SFX_BANK(sfxId)][entryIndex];
        if (entry->token == token && entry->sfxId == sfxId) {
            if (entry->state >= SFX_STATE_PLAYING_REFRESH) {
                Audio_QueueCmdS8(0x06020000 | ((entry->channelIdx & 0xFF) << 8), 0);
            }
            if (entry->state != SFX_STATE_EMPTY) {
                Audio_RemoveSoundBankEntry(SFX_BANK(sfxId), entryIndex);
            }
        } else {
            prevEntryIndex = entryIndex;
        }
        if (entryIndex != 0xFF) {
            entryIndex = gSoundBanks[SFX_BANK(sfxId)][prevEntryIndex].next;
        }
    }
    cmp.token = token;
    cmp.sfxId = sfxId;
    Audio_RemoveMatchingSoundRequests(4, &cmp);
}

void Audio_StopSfxById(u32 sfxId) {
    SoundBankEntry* entry;
    u8 entryIndex = gSoundBanks[SFX_BANK(sfxId)][0].next;
    u8 prevEntryIndex = 0;
    SoundBankEntry cmp;

    while (entryIndex != 0xFF) {
        entry = &gSoundBanks[SFX_BANK(sfxId)][entryIndex];
        if (entry->sfxId == sfxId) {
            if (entry->state >= SFX_STATE_PLAYING_REFRESH) {
                Audio_QueueCmdS8(0x06020000 | ((entry->channelIdx & 0xFF) << 8), 0);
            }
            if (entry->state != SFX_STATE_EMPTY) {
                Audio_RemoveSoundBankEntry(SFX_BANK(sfxId), entryIndex);
            }
        } else {
            prevEntryIndex = entryIndex;
        }
        entryIndex = gSoundBanks[SFX_BANK(sfxId)][prevEntryIndex].next;
    }
    cmp.sfxId = sfxId;
    Audio_RemoveMatchingSoundRequests(5, &cmp);
}

void Audio_ProcessSoundRequests(void) {
    if (gAudioContext.seqPlayers[2].enabled) {
        while (sSoundRequestWriteIndex != sSoundRequestReadIndex) {
            Audio_ProcessSoundRequest();
            sSoundRequestReadIndex++;
        }
    }
}

// OoT func_800F8EA0
void Audio_SetUnusedBankLerp(u8 bankId, u8 target, u16 delay) {
    if (delay == 0) {
        delay++;
    }
    sUnusedBankLerp[bankId].target = target / 127.0f;
    sUnusedBankLerp[bankId].remainingFrames = delay;
    sUnusedBankLerp[bankId].step = ((sUnusedBankLerp[bankId].value - sUnusedBankLerp[bankId].target) / delay);
}

// OoT func_800F8F34
void Audio_StepUnusedBankLerp(u8 bankId) {
    if (sUnusedBankLerp[bankId].remainingFrames != 0) {
        sUnusedBankLerp[bankId].remainingFrames--;
        if (sUnusedBankLerp[bankId].remainingFrames != 0) {
            sUnusedBankLerp[bankId].value -= sUnusedBankLerp[bankId].step;
        } else {
            sUnusedBankLerp[bankId].value = sUnusedBankLerp[bankId].target;
        }
    }
}

// OoT func_800F8F88
void func_801A787C(void) {
    u8 bankId;

    if (gAudioContext.seqPlayers[2].enabled) {
        sCurSfxPlayerChannelIdx = 0;
        for (bankId = 0; bankId < ARRAY_COUNT(gSoundBanks); bankId++) {
            Audio_ChooseActiveSounds(bankId);
            Audio_PlayActiveSounds(bankId);
            Audio_StepUnusedBankLerp(bankId);
        }
    }
}

u8 Audio_IsSfxPlaying(u32 sfxId) {
    SoundBankEntry* entry;
    u8 entryIndex = gSoundBanks[SFX_BANK(sfxId)][0].next;

    while (entryIndex != 0xFF) {
        entry = &gSoundBanks[SFX_BANK(sfxId)][entryIndex];
        if (entry->sfxId == sfxId) {
            return true;
        }
        entryIndex = entry->next;
    }
    return false;
}

// OoT func_800F905C
void Audio_ResetSounds(void) {
    u8 bankId;
    u8 i;
    u8 entryIndex;

    sSoundRequestWriteIndex = 0;
    sSoundRequestReadIndex = 0;
    D_801DB4A0 = 0;
    for (bankId = 0; bankId < ARRAY_COUNT(gSoundBanks); bankId++) {
        sSoundBankListEnd[bankId] = 0;
        sSoundBankFreeListStart[bankId] = 1;
        sSoundBankUnused[bankId] = 0;
        gSoundBankMuted[bankId] = false;
        sUnusedBankLerp[bankId].value = 1.0f;
        sUnusedBankLerp[bankId].remainingFrames = 0;
    }
    for (bankId = 0; bankId < ARRAY_COUNT(gSoundBanks); bankId++) {
        for (i = 0; i < MAX_CHANNELS_PER_BANK; i++) {
            gActiveSounds[bankId][i].entryIndex = 0xFF;
        }
    }
    for (bankId = 0; bankId < ARRAY_COUNT(gSoundBanks); bankId++) {
        gSoundBanks[bankId][0].prev = 0xFF;
        gSoundBanks[bankId][0].next = 0xFF;
        for (i = 1; i < sBankSizes[bankId] - 1; i++) {
            gSoundBanks[bankId][i].prev = i - 1;
            gSoundBanks[bankId][i].next = i + 1;
        }
        gSoundBanks[bankId][i].prev = i - 1;
        gSoundBanks[bankId][i].next = 0xFF;
    }
}
