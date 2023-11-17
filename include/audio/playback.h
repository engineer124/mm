#ifndef AUDIO_PLAYBACK_H
#define AUDIO_PLAYBACK_H

#include "audio/soundfont.h"
#include "PR/ultratypes.h"

struct Note;
struct SequenceLayer;

void AudioPlayback_NoteDisable(struct Note* note);
void AudioPlayback_ProcessNotes(void);
TunedSample* AudioPlayback_GetInstrumentTunedSample(Instrument* instrument, s32 semitone);
Instrument* AudioPlayback_GetInstrumentInner(s32 fontId, s32 instId);
Drum* AudioPlayback_GetDrum(s32 fontId, s32 drumId);
SoundEffect* AudioPlayback_GetSoundEffect(s32 fontId, s32 sfxId);
s32 AudioPlayback_SetFontInstrument(s32 instrumentType, s32 fontId, s32 index, void* value);
void AudioPlayback_SeqLayerNoteDecay(struct SequenceLayer* layer);
void AudioPlayback_SeqLayerNoteRelease(struct SequenceLayer* layer);
void AudioPlayback_InitSyntheticWave(struct Note* note, struct SequenceLayer* layer);
struct Note* AudioPlayback_AllocNote(struct SequenceLayer* layer);
void AudioPlayback_NoteInitAll(void);

#endif
