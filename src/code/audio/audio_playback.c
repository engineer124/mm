#include "global.h"

void AudioPlayback_InitNoteSub(Note* note, NoteSubEu* sub, NoteSubAttributes* attrs) {
    f32 volRight, volLeft;
    s32 smallPanIndex;
    u64 pad;
    u8 strongLeft;
    u8 strongRight;
    f32 vel;
    u8 pan;
    u8 reverbVol;
    StereoData sp24;
    s32 stereoHeadsetEffects = note->playbackState.stereoHeadsetEffects;

    vel = attrs->velocity;
    pan = attrs->pan;
    reverbVol = attrs->reverbVol;
    sp24 = attrs->stereo.s;

    sub->bitField0 = note->noteSubEu.bitField0;
    sub->bitField1 = note->noteSubEu.bitField1; 
    sub->sound.samples = note->noteSubEu.sound.samples;
    sub->unk_06 = note->noteSubEu.unk_06;

    AudioPlayback_NoteSetResamplingRate(sub, attrs->frequency);

    pan &= 0x7F;

    sub->bitField0.stereoStrongRight = false;
    sub->bitField0.stereoStrongLeft = false;
    sub->bitField0.stereoHeadsetEffects = sp24.stereoHeadsetEffects;
    sub->bitField0.usesHeadsetPanEffects = sp24.usesHeadsetPanEffects;
    if (stereoHeadsetEffects && gAudioContext.soundMode == 1) {
        smallPanIndex = pan >> 1;
        if (smallPanIndex > 0x3f) {
            smallPanIndex = 0x3f;
        }

        sub->headsetPanLeft = gHeadsetPanQuantization[smallPanIndex];
        sub->headsetPanRight = gHeadsetPanQuantization[0x3f - smallPanIndex];
        sub->bitField1.usesHeadsetPanEffects2 = true;

        volLeft = gHeadsetPanVolume[pan];
        volRight = gHeadsetPanVolume[0x7f - pan];
    } else if (stereoHeadsetEffects && gAudioContext.soundMode == 0) {
        strongLeft = strongRight = 0;
        sub->headsetPanRight = 0;
        sub->headsetPanLeft = 0;
        sub->bitField1.usesHeadsetPanEffects2 = false;

        volLeft = gStereoPanVolume[pan];
        volRight = gStereoPanVolume[0x7f - pan];
        if (pan < 0x20) {
            strongLeft = 1;
        } else if (pan > 0x60) {
            strongRight = 1;
        }

        sub->bitField0.stereoStrongRight = strongRight;
        sub->bitField0.stereoStrongLeft = strongLeft;

        switch (sp24.bit2) {
            case 0:
                break;
            case 1:
                sub->bitField0.stereoStrongRight = sp24.strongRight;
                sub->bitField0.stereoStrongLeft = sp24.strongLeft;
                break;
            case 2:
                sub->bitField0.stereoStrongRight = sp24.strongRight | strongRight;
                sub->bitField0.stereoStrongLeft = sp24.strongLeft | strongLeft;
                break;
            case 3:
                sub->bitField0.stereoStrongRight = sp24.strongRight ^ strongRight;
                sub->bitField0.stereoStrongLeft = sp24.strongLeft ^ strongLeft;
                break;
        }

    } else if (gAudioContext.soundMode == 3) {
        sub->bitField0.stereoHeadsetEffects = false;
        sub->bitField0.usesHeadsetPanEffects = false;
        volLeft = 0.707f;
        volRight = 0.707f;
    } else {
        sub->bitField0.stereoStrongRight = sp24.strongRight;
        sub->bitField0.stereoStrongLeft = sp24.strongLeft;
        volLeft = gDefaultPanVolume[pan];
        volRight = gDefaultPanVolume[0x7f - pan];
    }

    vel = 0.0f > vel ? 0.0f : vel;
    vel = 1.0f < vel ? 1.0f : vel;

    sub->targetVolLeft = (s32)((vel * volLeft) * 4095.999f);
    sub->targetVolRight = (s32)((vel * volRight) * 4095.999f);

    sub->unk_2 = attrs->unk_1;
    sub->filter = attrs->filter;
    sub->unk_07 = attrs->unk_14;
    sub->unk_0E = attrs->unk_16;
    sub->reverbVol = reverbVol;
    sub->unk_19 = attrs->unk_3;

}

void AudioPlayback_NoteSetResamplingRate(NoteSubEu* noteSubEu, f32 resamplingRateInput) {
    f32 resamplingRate = 0.0f;

    if (resamplingRateInput < 2.0f) {
        noteSubEu->bitField1.hasTwoParts = false;

        if (1.99998f < resamplingRateInput) {
            resamplingRate = 1.99998f;
        } else {
            resamplingRate = resamplingRateInput;
        }

    } else {
        noteSubEu->bitField1.hasTwoParts = true;
        if (3.99996f < resamplingRateInput) {
            resamplingRate = 1.99998f;
        } else {
            resamplingRate = resamplingRateInput * 0.5f;
        }
    }
    noteSubEu->resamplingRateFixedPoint = (s32)(resamplingRate * 32768.0f);
}

void AudioPlayback_NoteInit(Note* note) {
    if (note->playbackState.parentLayer->adsr.releaseRate == 0) {
        AudioEffects_AdsrInit(&note->playbackState.adsr, note->playbackState.parentLayer->seqChannel->adsr.envelope,
                       &note->playbackState.adsrVolScale);
    } else {
        AudioEffects_AdsrInit(&note->playbackState.adsr, note->playbackState.parentLayer->adsr.envelope,
                       &note->playbackState.adsrVolScale);
    }

    note->playbackState.unk_04 = 0;
    note->playbackState.adsr.action.s.state = ADSR_STATE_INITIAL;
    note->noteSubEu = gDefaultNoteSub;
}

void AudioPlayback_NoteDisable(Note* note) {
    if (note->noteSubEu.bitField0.needsInit == true) {
        note->noteSubEu.bitField0.needsInit = false;
    }
    note->playbackState.priority = 0;
    note->noteSubEu.bitField0.enabled = false; 
    note->playbackState.unk_04 = 0;
    note->noteSubEu.bitField0.finished = false;
    note->playbackState.parentLayer = NO_LAYER;
    note->playbackState.prevParentLayer = NO_LAYER;
    note->playbackState.adsr.action.s.state = ADSR_STATE_DISABLED;
    note->playbackState.adsr.current = 0;
}

void AudioPlayback_ProcessNotes(void) {
    s32 pad;
    s32 new_var;
    NoteAttributes* attrs;
    NoteSubEu* noteSubEu2;
    NoteSubEu* noteSubEu;
    Note* note;
    NotePlaybackState* playbackState;
    NoteSubAttributes subAttrs;
    u8 bookOffset;
    f32 scale;
    s32 i;

    for (i = 0; i < gAudioContext.numNotes; i++) {
        note = &gAudioContext.notes[i];
        noteSubEu2 = &gAudioContext.noteSubsEu[gAudioContext.noteSubEuOffset + i];
        playbackState = &note->playbackState;
        if (playbackState->parentLayer != NO_LAYER) {
            if ((u32)playbackState->parentLayer < 0x7FFFFFFFU) {
                continue;
            }

            if (note != playbackState->parentLayer->note && playbackState->unk_04 == 0) {
                playbackState->adsr.action.s.release = true;
                playbackState->adsr.fadeOutVel = gAudioContext.audioBufferParameters.updatesPerFrameInv;
                playbackState->priority = 1;
                playbackState->unk_04 = 2;
                goto out;
            } else if (!playbackState->parentLayer->enabled && playbackState->unk_04 == 0 &&
                       playbackState->priority >= 1) {
                // do nothing
            } else if (playbackState->parentLayer->seqChannel->seqPlayer == NULL) {
                AudioSeq_SequenceChannelDisable(playbackState->parentLayer->seqChannel);
                playbackState->priority = 1;
                playbackState->unk_04 = 1;
                continue;
            } else if (playbackState->parentLayer->seqChannel->seqPlayer->muted &&
                       (playbackState->parentLayer->seqChannel->muteBehavior & 0x40)) {
                // do nothing
            } else {
                goto out;
            }

            AudioPlayback_SeqChanLayerNoteRelease(playbackState->parentLayer);
            AudioPlayback_AudioListRemove(&note->listItem);
            AudioPlayback_AudioListPushFront(&note->listItem.pool->decaying, &note->listItem);
            playbackState->priority = 1;
            playbackState->unk_04 = 2;
        } else if (playbackState->unk_04 == 0 && playbackState->priority >= 1) {
            continue;
        }

    out:
        if (playbackState->priority != 0) {
            if (1) {}
            noteSubEu = &note->noteSubEu;
            if (playbackState->unk_04 >= 1 || noteSubEu->bitField0.finished) {
                if (playbackState->adsr.action.s.state == ADSR_STATE_DISABLED || noteSubEu->bitField0.finished) {
                    if (playbackState->wantedParentLayer != NO_LAYER) {
                        AudioPlayback_NoteDisable(note);
                        if (playbackState->wantedParentLayer->seqChannel != NULL) {
                            AudioPlayback_NoteInitForLayer(note, playbackState->wantedParentLayer);
                            AudioEffects_NoteVibratoInit(note);
                            AudioEffects_NotePortamentoInit(note);
                            AudioPlayback_AudioListRemove(&note->listItem);
                            AudioSeq_AudioListPushBack(&note->listItem.pool->active, &note->listItem);
                            playbackState->wantedParentLayer = NO_LAYER;
                            // don't skip
                        } else {
                            AudioPlayback_NoteDisable(note);
                            AudioPlayback_AudioListRemove(&note->listItem);
                            AudioSeq_AudioListPushBack(&note->listItem.pool->disabled, &note->listItem);
                            playbackState->wantedParentLayer = NO_LAYER;
                            goto skip;
                        }
                    } else {
                        if (playbackState->parentLayer != NO_LAYER) {
                            playbackState->parentLayer->bit1 = true;
                        }
                        AudioPlayback_NoteDisable(note);
                        AudioPlayback_AudioListRemove(&note->listItem);
                        AudioSeq_AudioListPushBack(&note->listItem.pool->disabled, &note->listItem);
                        continue;
                    }
                }
            } else if (playbackState->adsr.action.s.state == ADSR_STATE_DISABLED) {
                if (playbackState->parentLayer != NO_LAYER) {
                    playbackState->parentLayer->bit1 = true;
                }
                AudioPlayback_NoteDisable(note);
                AudioPlayback_AudioListRemove(&note->listItem);
                AudioSeq_AudioListPushBack(&note->listItem.pool->disabled, &note->listItem);
                continue;
            }

            scale = AudioEffects_AdsrUpdate(&playbackState->adsr);
            AudioEffects_NoteVibratoUpdate(note);
            new_var = playbackState->unk_04;
            attrs = &playbackState->attributes;
            if (new_var == 1 || new_var == 2) {
                subAttrs.frequency = attrs->freqScale;
                subAttrs.velocity = attrs->velocity;
                subAttrs.pan = attrs->pan;
                subAttrs.reverbVol = attrs->reverb;
                subAttrs.stereo = attrs->stereo;
                subAttrs.unk_1 = attrs->unk_1;
                subAttrs.filter = attrs->filter;
                subAttrs.unk_14 = attrs->unk_4;
                subAttrs.unk_16 = attrs->unk_6;
                subAttrs.unk_3 = attrs->unk_3;
                bookOffset = noteSubEu->bitField1.bookOffset;
            } else {
                SequenceChannelLayer* layer = playbackState->parentLayer;
                SequenceChannel* channel = playbackState->parentLayer->seqChannel;

                subAttrs.frequency = layer->noteFreqScale;
                subAttrs.velocity = layer->noteVelocity;
                subAttrs.pan = layer->notePan;

                if (layer->unk_08 == 0x80) {
                    subAttrs.unk_3 = channel->unk_10;
                } else {
                    subAttrs.unk_3 = layer->unk_08;
                }

                if (layer->stereo.s.bit2 == 0) {
                    subAttrs.stereo = channel->stereo;
                } else {
                    subAttrs.stereo = layer->stereo;
                }
                    
                if (layer->unk_0A.s.bit_2 == 1) { 
                    subAttrs.reverbVol = channel->reverb;
                } else {
                    subAttrs.reverbVol = layer->unk_09;
                }

                if (layer->unk_0A.s.bit_9 == 1) {
                    subAttrs.unk_1 = channel->unk_0C;
                } else {
                    subAttrs.unk_1 = 0;
                    if (1) {}
                }

                subAttrs.filter = channel->filter;
                subAttrs.unk_14 = channel->unk_0F;
                subAttrs.unk_16 = channel->unk_20;
                bookOffset = channel->bookOffset & 0x7;

                if (channel->seqPlayer->muted && (channel->muteBehavior & 8)) {
                    subAttrs.frequency = 0.0f;
                    subAttrs.velocity = 0.0f;
                }
            }

            subAttrs.frequency *= playbackState->vibratoFreqScale * playbackState->portamentoFreqScale;
            subAttrs.frequency *= gAudioContext.audioBufferParameters.resampleRate;
            subAttrs.velocity *= scale;
            AudioPlayback_InitNoteSub(note, noteSubEu2, &subAttrs);
            noteSubEu->bitField1.bookOffset = bookOffset;
        skip:;
        }
    }
}

AudioBankSound* AudioPlayback_InstrumentGetAudioBankSound(Instrument* instrument, s32 semitone) {
    AudioBankSound* sound;

    if (semitone < instrument->normalRangeLo) {
        sound = &instrument->lowNotesSound;
    } else if (semitone <= instrument->normalRangeHi) {
        sound = &instrument->normalNotesSound;
    } else {
        sound = &instrument->highNotesSound;
    }
    return sound;
}

Instrument* AudioPlayback_GetInstrumentInner(s32 bankId, s32 instId) {
    Instrument* inst;

    if (bankId == 0xFF) {
        return NULL;
    }

    if (!AudioLoad_IsBankLoadComplete(bankId)) {
        gAudioContext.audioErrorFlags = bankId + 0x10000000;
        return NULL;
    }

    if (instId >= gAudioContext.ctlEntries[bankId].numInstruments) {
        gAudioContext.audioErrorFlags = ((bankId << 8) + instId) + 0x3000000;
        return NULL;
    }

    inst = gAudioContext.ctlEntries[bankId].instruments[instId];
    if (inst == NULL) {
        gAudioContext.audioErrorFlags = ((bankId << 8) + instId) + 0x1000000;
        return inst;
    }

    return inst;
}

Drum* AudioPlayback_GetDrum(s32 bankId, s32 drumId) {
    Drum* drum;

    if (bankId == 0xFF) {
        return NULL;
    }

    if (!AudioLoad_IsBankLoadComplete(bankId)) {
        gAudioContext.audioErrorFlags = bankId + 0x10000000;
        return NULL;
    }

    if (drumId >= gAudioContext.ctlEntries[bankId].numDrums) {
        gAudioContext.audioErrorFlags = ((bankId << 8) + drumId) + 0x4000000;
        return NULL;
    }
    if ((u32)gAudioContext.ctlEntries[bankId].drums < 0x80000000U) {
        return NULL;
    }
    drum = gAudioContext.ctlEntries[bankId].drums[drumId];

    if (drum == NULL) {
        gAudioContext.audioErrorFlags = ((bankId << 8) + drumId) + 0x5000000;
    }

    return drum;
}

AudioBankSound* AudioPlayback_GetSfx(s32 bankId, s32 sfxId) {
    AudioBankSound* sfx;

    if (bankId == 0xFF) {
        return NULL;
    }

    if (!AudioLoad_IsBankLoadComplete(bankId)) {
        gAudioContext.audioErrorFlags = bankId + 0x10000000;
        return NULL;
    }

    if (sfxId >= gAudioContext.ctlEntries[bankId].numSfx) {
        gAudioContext.audioErrorFlags = ((bankId << 8) + sfxId) + 0x4000000;
        return NULL;
    }

    if ((u32)gAudioContext.ctlEntries[bankId].soundEffects < 0x80000000U) {
        return NULL;
    }

    sfx = &gAudioContext.ctlEntries[bankId].soundEffects[sfxId];

    if (sfx == NULL) {
        gAudioContext.audioErrorFlags = ((bankId << 8) + sfxId) + 0x5000000;
    }

    if (sfx->sample == NULL) {
        return NULL;
    }

    return sfx;
}

// OoT func_800E7744
s32 func_801957B4(s32 instrument, s32 bankId, s32 instId, void* arg3) {
    if (bankId == 0xFF) {
        return -1;
    }

    if (!AudioLoad_IsBankLoadComplete(bankId)) {
        return -2;
    }

    switch (instrument) {
        case 0:
            if (instId >= gAudioContext.ctlEntries[bankId].numDrums) {
                return -3;
            }
            gAudioContext.ctlEntries[bankId].drums[instId] = arg3;
            break;

        case 1:
            if (instId >= gAudioContext.ctlEntries[bankId].numSfx) {
                return -3;
            }
            gAudioContext.ctlEntries[bankId].soundEffects[instId] = *(AudioBankSound*)arg3;
            break;

        default:
            if (instId >= gAudioContext.ctlEntries[bankId].numInstruments) {
                return -3;
            }
            gAudioContext.ctlEntries[bankId].instruments[instId] = arg3;
            break;
    }

    return 0;
}

void AudioPlayback_SeqChanLayerDecayRelease(SequenceChannelLayer* seqLayer, s32 target) {
    Note* note;
    NoteAttributes* attrs;
    SequenceChannel* chan;
    s32 i;

    if (seqLayer == NO_LAYER) {
        return;
    }

    seqLayer->bit3 = false;

    if (seqLayer->note == NULL) {
        return;
    }

    note = seqLayer->note;
    attrs = &note->playbackState.attributes;

    if (note->playbackState.wantedParentLayer == seqLayer) {
        note->playbackState.wantedParentLayer = NO_LAYER;
    }

    if (note->playbackState.parentLayer != seqLayer) {
        if (note->playbackState.parentLayer == NO_LAYER && note->playbackState.wantedParentLayer == NO_LAYER &&
            note->playbackState.prevParentLayer == seqLayer && target != ADSR_STATE_DECAY) {
            note->playbackState.adsr.fadeOutVel = gAudioContext.audioBufferParameters.updatesPerFrameInv;
            note->playbackState.adsr.action.s.release = true;
        }
        return;
    }

    if (note->playbackState.adsr.action.s.state != ADSR_STATE_DECAY) {
        attrs->freqScale = seqLayer->noteFreqScale;
        attrs->velocity = seqLayer->noteVelocity;
        attrs->pan = seqLayer->notePan;

        if (seqLayer->seqChannel != NULL) {
            chan = seqLayer->seqChannel;

            if (seqLayer->unk_0A.s.bit_2 == 1) { 
                attrs->reverb = chan->reverb;
            } else {
                attrs->reverb = seqLayer->unk_09;
            }

            if (seqLayer->unk_08 == 0x80) { 
                attrs->unk_3 = chan->unk_10;
            } else {
                attrs->unk_3 = seqLayer->unk_08; 
            }

            if (seqLayer->unk_0A.s.bit_9 == 1) {
                attrs->unk_1 = chan->unk_0C;
            } else {
                attrs->unk_1 = 0;
            }

            attrs->filter = chan->filter;

            if (attrs->filter != NULL) {
                for (i = 0; i < 8; i++) {
                    attrs->filterBuf[i] = attrs->filter[i];
                }
                attrs->filter = attrs->filterBuf;
            }

            attrs->unk_6 = chan->unk_20;
            attrs->unk_4 = chan->unk_0F;
            if (chan->seqPlayer->muted && (chan->muteBehavior & 8)) {
                note->noteSubEu.bitField0.finished = true;
            }

            if (seqLayer->stereo.asByte == 0) {
                attrs->stereo = chan->stereo;
            } else {
                attrs->stereo = seqLayer->stereo;
            }
            note->playbackState.priority = chan->someOtherPriority;
        } else {
            attrs->stereo = seqLayer->stereo;
            note->playbackState.priority = 1;
        }

        note->playbackState.prevParentLayer = note->playbackState.parentLayer;
        note->playbackState.parentLayer = NO_LAYER;
        if (target == ADSR_STATE_RELEASE) {
            note->playbackState.adsr.fadeOutVel = gAudioContext.audioBufferParameters.updatesPerFrameInv;
            note->playbackState.adsr.action.s.release = true;
            note->playbackState.unk_04 = 2;
        } else {
            note->playbackState.unk_04 = 1;
            note->playbackState.adsr.action.s.decay = true;
            if (seqLayer->adsr.releaseRate == 0) {
                note->playbackState.adsr.fadeOutVel = gAudioContext.unk_3520[seqLayer->seqChannel->adsr.releaseRate];
            } else {
                note->playbackState.adsr.fadeOutVel = gAudioContext.unk_3520[seqLayer->adsr.releaseRate];
            }
            note->playbackState.adsr.sustain =
                ((f32)(s32)(seqLayer->seqChannel->adsr.sustain) * note->playbackState.adsr.current) / 256.0f;
        }
    }

    if (target == ADSR_STATE_DECAY) {
        AudioPlayback_AudioListRemove(&note->listItem);
        AudioPlayback_AudioListPushFront(&note->listItem.pool->decaying, &note->listItem);
    }
}

void AudioPlayback_SeqChanLayerNoteDecay(SequenceChannelLayer* seqLayer) {
    AudioPlayback_SeqChanLayerDecayRelease(seqLayer, ADSR_STATE_DECAY);
}

void AudioPlayback_SeqChanLayerNoteRelease(SequenceChannelLayer* seqLayer) {
    AudioPlayback_SeqChanLayerDecayRelease(seqLayer, ADSR_STATE_RELEASE);
}

s32 AudioPlayback_BuildSyntheticWave(Note* note, SequenceChannelLayer* seqLayer, s32 waveId) {
    f32 freqScale;
    f32 ratio;
    u8 sampleCountIndex;

    if (waveId < 128) {
        waveId = 128;
    }

    freqScale = seqLayer->freqScale;
    if (seqLayer->portamento.mode != 0 && 0.0f < seqLayer->portamento.extent) {
        freqScale *= (seqLayer->portamento.extent + 1.0f);
    }
    if (freqScale < 0.99999f) {
        sampleCountIndex = 0;
        ratio = 1.0465f;
    } else if (freqScale < 1.99999f) {
        sampleCountIndex = 1;
        ratio = 0.52325f;
    } else if (freqScale < 3.99999f) {
        sampleCountIndex = 2;
        ratio = 0.26263f;
    } else {
        sampleCountIndex = 3;
        ratio = 0.13081f;
    }
    seqLayer->freqScale *= ratio;
    note->playbackState.waveId = waveId;
    note->playbackState.sampleCountIndex = sampleCountIndex;

    note->noteSubEu.sound.samples = &gWaveSamples[waveId - 128][sampleCountIndex * 64];

    return sampleCountIndex;
}

void AudioPlayback_InitSyntheticWave(Note* note, SequenceChannelLayer* seqLayer) {
    s32 sampleCountIndex;
    s32 waveSampleCountIndex;
    s32 waveId = seqLayer->instOrWave;

    if (waveId == 0xff) {
        waveId = seqLayer->seqChannel->instOrWave;
    }

    sampleCountIndex = note->playbackState.sampleCountIndex;
    waveSampleCountIndex = AudioPlayback_BuildSyntheticWave(note, seqLayer, waveId);

    if (waveSampleCountIndex != sampleCountIndex) {
        note->noteSubEu.unk_06 = waveSampleCountIndex * 4 + sampleCountIndex;
    }
}

void AudioPlayback_InitNoteList(AudioListItem* list) {
    list->prev = list;
    list->next = list;
    list->u.count = 0;
}

void AudioPlayback_InitNoteLists(NotePool* pool) {
    AudioPlayback_InitNoteList(&pool->disabled);
    AudioPlayback_InitNoteList(&pool->decaying);
    AudioPlayback_InitNoteList(&pool->releasing);
    AudioPlayback_InitNoteList(&pool->active);
    pool->disabled.pool = pool;
    pool->decaying.pool = pool;
    pool->releasing.pool = pool;
    pool->active.pool = pool;
}

void AudioPlayback_InitNoteFreeList(void) {
    s32 i;

    AudioPlayback_InitNoteLists(&gAudioContext.noteFreeLists);
    for (i = 0; i < gAudioContext.numNotes; i++) {
        gAudioContext.notes[i].listItem.u.value = &gAudioContext.notes[i];
        gAudioContext.notes[i].listItem.prev = NULL;
        AudioSeq_AudioListPushBack(&gAudioContext.noteFreeLists.disabled, &gAudioContext.notes[i].listItem);
    }
}

void AudioPlayback_NotePoolClear(NotePool* pool) {
    s32 i;
    AudioListItem* source;
    AudioListItem* cur;
    AudioListItem* dest;

    for (i = 0; i < 4; i++) {
        switch (i) {
            case 0:
                source = &pool->disabled;
                dest = &gAudioContext.noteFreeLists.disabled;
                break;

            case 1:
                source = &pool->decaying;
                dest = &gAudioContext.noteFreeLists.decaying;
                break;

            case 2:
                source = &pool->releasing;
                dest = &gAudioContext.noteFreeLists.releasing;
                break;

            case 3:
                source = &pool->active;
                dest = &gAudioContext.noteFreeLists.active;
                break;
        }

        for (;;) {
            cur = source->next;
            if (cur == source || cur == NULL) {
                break;
            }
            AudioPlayback_AudioListRemove(cur);
            AudioSeq_AudioListPushBack(dest, cur);
        }
    }
}

void AudioPlayback_NotePoolFill(NotePool* pool, s32 count) {
    s32 i;
    s32 j;
    Note* note;
    AudioListItem* source;
    AudioListItem* dest;

    AudioPlayback_NotePoolClear(pool);

    for (i = 0, j = 0; j < count; i++) {
        if (i == 4) {
            return;
        }

        switch (i) {
            case 0:
                source = &gAudioContext.noteFreeLists.disabled;
                dest = &pool->disabled;
                break;

            case 1:
                source = &gAudioContext.noteFreeLists.decaying;
                dest = &pool->decaying;
                break;

            case 2:
                source = &gAudioContext.noteFreeLists.releasing;
                dest = &pool->releasing;
                break;

            case 3:
                source = &gAudioContext.noteFreeLists.active;
                dest = &pool->active;
                break;
        }

        while (j < count) {
            note = AudioSeq_AudioListPopBack(source);
            if (note == NULL) {
                break;
            }
            AudioSeq_AudioListPushBack(dest, &note->listItem);
            j++;
        }
    }
}

void AudioPlayback_AudioListPushFront(AudioListItem* list, AudioListItem* item) {
    // add 'item' to the front of the list given by 'list', if it's not in any list
    if (item->prev == NULL) {
        item->prev = list;
        item->next = list->next;
        list->next->prev = item;
        list->next = item;
        list->u.count++;
        item->pool = list->pool;
    }
}

void AudioPlayback_AudioListRemove(AudioListItem* item) {
    // remove 'item' from the list it's in, if any
    if (item->prev != NULL) {
        item->prev->next = item->next;
        item->next->prev = item->prev;
        item->prev = NULL;
    }
}

Note* AudioPlayback_PopNodeWithValueLessEqual(AudioListItem* list, s32 limit) {
    AudioListItem* cur = list->next;
    AudioListItem* best;

    if (cur == list) {
        return NULL;
    }

    for (best = cur; cur != list; cur = cur->next) {
        if (((Note*)best->u.value)->playbackState.priority >= ((Note*)cur->u.value)->playbackState.priority) {
            best = cur;
        }
    }

    if (best == NULL) {
        return NULL;
    }

    if (limit <= ((Note*)best->u.value)->playbackState.priority) {
        return NULL;
    }

    return best->u.value;
}
void AudioPlayback_NoteInitForLayer(Note* note, SequenceChannelLayer* seqLayer) {
    s32 pad[3];
    s16 instId;
    SequenceChannel* channel = seqLayer->seqChannel;
    NoteSubStruct* noteSubStruct = &((Note2*)note)->noteSubStruct;
    NoteSubEu* sub = &note->noteSubEu;

    noteSubStruct->playbackState.prevParentLayer = NO_LAYER;
    noteSubStruct->playbackState.parentLayer = seqLayer;
    noteSubStruct->playbackState.priority = channel->notePriority;
    seqLayer->notePropertiesNeedInit = true;
    seqLayer->bit3 = true;
    seqLayer->note = note;
    channel->noteUnused = note;
    channel->layerUnused = seqLayer;
    seqLayer->noteVelocity = 0.0f;
    AudioPlayback_NoteInit(note);
    instId = seqLayer->instOrWave;

    if (instId == 0xff) {
        instId = channel->instOrWave;
    }
    sub->sound.audioBankSound = seqLayer->sound;

    if (instId >= 0x80 && instId < 0xC0) {
        sub->bitField1.isSyntheticWave = true;
    } else {
        sub->bitField1.isSyntheticWave = false;
    }

    if (sub->bitField1.isSyntheticWave) {
        AudioPlayback_BuildSyntheticWave(note, seqLayer, instId);
    } else if (channel->unk_DC == 1) {
        noteSubStruct->unk_BC = sub->sound.audioBankSound->sample->loop->start;
    } else {
        noteSubStruct->unk_BC = channel->unk_DC;
        if (noteSubStruct->unk_BC >= sub->sound.audioBankSound->sample->loop->end) {
            noteSubStruct->unk_BC = 0;
        }
    }


    noteSubStruct->playbackState.bankId = channel->bankId;
    noteSubStruct->playbackState.stereoHeadsetEffects = channel->stereoHeadsetEffects;
    sub->bitField1.reverbIndex = channel->reverbIndex & 3;
}

// OoT func_800E82C0
void func_801963E8(Note* note, SequenceChannelLayer* seqLayer) {
    AudioPlayback_SeqChanLayerNoteRelease(note->playbackState.parentLayer);
    note->playbackState.wantedParentLayer = seqLayer;
}

void AudioPlayback_NoteReleaseAndTakeOwnership(Note* note, SequenceChannelLayer* seqLayer) {
    note->playbackState.wantedParentLayer = seqLayer;
    note->playbackState.priority = seqLayer->seqChannel->notePriority;

    note->playbackState.adsr.fadeOutVel = gAudioContext.audioBufferParameters.updatesPerFrameInv;
    note->playbackState.adsr.action.s.release = true;
}

Note* AudioPlayback_AllocNoteFromDisabled(NotePool* pool, SequenceChannelLayer* seqLayer) {
    Note* note = AudioSeq_AudioListPopBack(&pool->disabled);
    if (note != NULL) {
        AudioPlayback_NoteInitForLayer(note, seqLayer);
        AudioPlayback_AudioListPushFront(&pool->active, &note->listItem);
    }
    return note;
}

Note* AudioPlayback_AllocNoteFromDecaying(NotePool* pool, SequenceChannelLayer* seqLayer) {
    Note* note = AudioPlayback_PopNodeWithValueLessEqual(&pool->decaying, seqLayer->seqChannel->notePriority);

    if (note != NULL) {
        AudioPlayback_NoteReleaseAndTakeOwnership(note, seqLayer); 
        AudioPlayback_AudioListRemove(&note->listItem);
        AudioSeq_AudioListPushBack(&pool->releasing, &note->listItem);
    }
    return note;
}

Note* AudioPlayback_AllocNoteFromActive(NotePool* pool, SequenceChannelLayer* seqLayer) {
    Note* rNote;
    Note* aNote;
    s32 rPriority;
    s32 aPriority;

    rPriority = aPriority = 0x10;
    rNote = AudioPlayback_PopNodeWithValueLessEqual(&pool->releasing, seqLayer->seqChannel->notePriority);

    if (rNote != NULL) {
        rPriority = rNote->playbackState.priority;
    }

    aNote = AudioPlayback_PopNodeWithValueLessEqual(&pool->active, seqLayer->seqChannel->notePriority);

    if (aNote != NULL) {
        aPriority = aNote->playbackState.priority;
    }

    if (rNote == NULL && aNote == NULL) {
        return NULL;
    }

    if (aPriority < rPriority) {
        AudioPlayback_AudioListRemove(&aNote->listItem);
        func_801963E8(aNote, seqLayer);
        AudioSeq_AudioListPushBack(&pool->releasing, &aNote->listItem);
        aNote->playbackState.priority = seqLayer->seqChannel->notePriority;
        return aNote;
    }
    rNote->playbackState.wantedParentLayer = seqLayer;
    rNote->playbackState.priority = seqLayer->seqChannel->notePriority;
    return rNote;
}

Note* AudioPlayback_AllocNote(SequenceChannelLayer* seqLayer) {
    Note* ret;
    u32 policy = seqLayer->seqChannel->noteAllocPolicy;

    if (policy & 1) {
        ret = seqLayer->note;
        if (ret != NULL && ret->playbackState.prevParentLayer == seqLayer &&
            ret->playbackState.wantedParentLayer == NO_LAYER) {
            AudioPlayback_NoteReleaseAndTakeOwnership(ret, seqLayer);
            AudioPlayback_AudioListRemove(&ret->listItem);
            AudioSeq_AudioListPushBack(&ret->listItem.pool->releasing, &ret->listItem);
            return ret;
        }
    }

    if (policy & 2) {
        if (!(ret = AudioPlayback_AllocNoteFromDisabled(&seqLayer->seqChannel->notePool, seqLayer)) &&
            !(ret = AudioPlayback_AllocNoteFromDecaying(&seqLayer->seqChannel->notePool, seqLayer)) &&
            !(ret = AudioPlayback_AllocNoteFromActive(&seqLayer->seqChannel->notePool, seqLayer))) {
            goto null_return;
        }
        return ret;
    }

    if (policy & 4) {
        if (!(ret = AudioPlayback_AllocNoteFromDisabled(&seqLayer->seqChannel->notePool, seqLayer)) &&
            !(ret = AudioPlayback_AllocNoteFromDisabled(&seqLayer->seqChannel->seqPlayer->notePool, seqLayer)) &&
            !(ret = AudioPlayback_AllocNoteFromDecaying(&seqLayer->seqChannel->notePool, seqLayer)) &&
            !(ret = AudioPlayback_AllocNoteFromDecaying(&seqLayer->seqChannel->seqPlayer->notePool, seqLayer)) &&
            !(ret = AudioPlayback_AllocNoteFromActive(&seqLayer->seqChannel->notePool, seqLayer)) &&
            !(ret = AudioPlayback_AllocNoteFromActive(&seqLayer->seqChannel->seqPlayer->notePool, seqLayer))) {
            goto null_return;
        }
        return ret;
    }

    if (policy & 8) {
        if (!(ret = AudioPlayback_AllocNoteFromDisabled(&gAudioContext.noteFreeLists, seqLayer)) &&
            !(ret = AudioPlayback_AllocNoteFromDecaying(&gAudioContext.noteFreeLists, seqLayer)) &&
            !(ret = AudioPlayback_AllocNoteFromActive(&gAudioContext.noteFreeLists, seqLayer))) {
            goto null_return;
        }
        return ret;
    }

    if (!(ret = AudioPlayback_AllocNoteFromDisabled(&seqLayer->seqChannel->notePool, seqLayer)) &&
        !(ret = AudioPlayback_AllocNoteFromDisabled(&seqLayer->seqChannel->seqPlayer->notePool, seqLayer)) &&
        !(ret = AudioPlayback_AllocNoteFromDisabled(&gAudioContext.noteFreeLists, seqLayer)) &&
        !(ret = AudioPlayback_AllocNoteFromDecaying(&seqLayer->seqChannel->notePool, seqLayer)) &&
        !(ret = AudioPlayback_AllocNoteFromDecaying(&seqLayer->seqChannel->seqPlayer->notePool, seqLayer)) &&
        !(ret = AudioPlayback_AllocNoteFromDecaying(&gAudioContext.noteFreeLists, seqLayer)) &&
        !(ret = AudioPlayback_AllocNoteFromActive(&seqLayer->seqChannel->notePool, seqLayer)) &&
        !(ret = AudioPlayback_AllocNoteFromActive(&seqLayer->seqChannel->seqPlayer->notePool, seqLayer)) &&
        !(ret = AudioPlayback_AllocNoteFromActive(&gAudioContext.noteFreeLists, seqLayer))) {
        goto null_return;
    }
    return ret;

null_return:
    seqLayer->bit3 = true;
    return NULL;
}

void AudioPlayback_NoteInitAll(void) {
    Note* note;
    s32 i;

    for (i = 0; i < gAudioContext.numNotes; i++) {
        note = &gAudioContext.notes[i];
        note->noteSubEu = gZeroNoteSub;
        note->playbackState.priority = 0;
        note->playbackState.unk_04 = 0;
        note->playbackState.parentLayer = NO_LAYER;
        note->playbackState.wantedParentLayer = NO_LAYER;
        note->playbackState.prevParentLayer = NO_LAYER;
        note->playbackState.waveId = 0;
        note->playbackState.attributes.velocity = 0.0f;
        note->playbackState.adsrVolScale = 0;

        note->playbackState.adsr.action.asByte = 0; 
        
        note->vibratoState.active = 0;
        note->portamento.cur = 0;
        note->portamento.speed = 0;
        note->playbackState.stereoHeadsetEffects = false;
        note->unk_BC = 0;

        note->synthesisState.synthesisBuffers = AudioHeap_AllocDmaMemory(&gAudioContext.notesAndBuffersPool, 0x2E0);
        note->playbackState.attributes.filterBuf = AudioHeap_AllocDmaMemory(&gAudioContext.notesAndBuffersPool, 0x10);
    }
}
