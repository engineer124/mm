#ifndef AUDIOCMD_H
#define AUDIOCMD_H

/**
 * Audio Thread Commands to safely transfer information/requests/data
 * from the graph thread to the audio thread
 */

typedef enum {
    /* 0x01 */ AUDIOCMD_OP_CHANNEL_VOL_SCALE = 0x1,
    /* 0x02 */ AUDIOCMD_OP_CHANNEL_VOL,
    /* 0x03 */ AUDIOCMD_OP_CHANNEL_PAN_SIGNED,
    /* 0x04 */ AUDIOCMD_OP_CHANNEL_FREQ_SCALE,
    /* 0x05 */ AUDIOCMD_OP_CHANNEL_REVERB_VOLUME,
    /* 0x06 */ AUDIOCMD_OP_CHANNEL_IO,
    /* 0x07 */ AUDIOCMD_OP_CHANNEL_PAN_UNSIGNED,
    /* 0x08 */ AUDIOCMD_OP_CHANNEL_MUTE,
    /* 0x09 */ AUDIOCMD_OP_CHANNEL_MUTE_FLAGS,
    /* 0x0A */ AUDIOCMD_OP_CHANNEL_VIBRATO_SMALL,
    /* 0x0B */ AUDIOCMD_OP_CHANNEL_VIBRATO_LARGE,
    /* 0x0C */ AUDIOCMD_OP_CHANNEL_COMB_FILTER_SIZE,
    /* 0x0D */ AUDIOCMD_OP_CHANNEL_COMB_FILTER_GAIN,
    /* 0x0E */ AUDIOCMD_OP_CHANNEL_STEREO,
    /* 0x0F */ AUDIOCMD_OP_CHANNEL_SET_START_POS,
    /* 0x10 */ AUDIOCMD_OP_CHANNEL_SFX_STATE,
    /* 0x11 */ AUDIOCMD_OP_CHANNEL_REVERB_INDEX,
    /* 0x12 */ AUDIOCMD_OP_CHANNEL_SURROUND_EFFECT_INDEX,
    /* 0x13 */ AUDIOCMD_OP_CHANNEL_FILTER,
    /* 0x14 */ AUDIOCMD_OP_CHANNEL_GAIN,
    /* 0x41 */ AUDIOCMD_OP_SEQPLAYER_FADE_VOLUME_SCALE = 0x41,
    /* 0x46 */ AUDIOCMD_OP_SEQPLAYER_SET_IO = 0x46,
    /* 0x47 */ AUDIOCMD_OP_SEQPLAYER_SET_TEMPO,
    /* 0x48 */ AUDIOCMD_OP_SEQPLAYER_SET_TRANSPOSITION,
    /* 0x49 */ AUDIOCMD_OP_SEQPLAYER_SET_SCALED_UNK_0C,
    /* 0x4A */ AUDIOCMD_OP_SEQPLAYER_SET_FADE_VOLUME,
    /* 0x4B */ AUDIOCMD_OP_SEQPLAYER_SCALE_FADE_VOLUME,
    /* 0x4C */ AUDIOCMD_OP_SEQPLAYER_SET_FADE_TIMER,
    /* 0x4D */ AUDIOCMD_OP_SEQPLAYER_SET_BEND,
    /* 0x4E */ AUDIOCMD_OP_SEQPLAYER_SET_UNK_0C,
    /* 0x81 */ AUDIOCMD_OP_GLOBAL_SYNC_LOAD_SEQ_PARTS = 0x81,
    /* 0x82 */ AUDIOCMD_OP_GLOBAL_SYNC_INIT_SEQPLAYER,
    /* 0x83 */ AUDIOCMD_OP_GLOBAL_DISABLE_SEQPLAYER,
    /* 0x85 */ AUDIOCMD_OP_GLOBAL_SYNC_INIT_SEQPLAYER_SKIP_TICKS = 0x85,
    /* 0x90 */ AUDIOCMD_OP_GLOBAL_SET_ACTIVE_CHANNEL_FLAGS = 0x90,
    /* 0xE0 */ AUDIOCMD_OP_GLOBAL_SET_DRUM_FONT = 0xE0,
    /* 0xE1 */ AUDIOCMD_OP_GLOBAL_SET_SFX_FONT,
    /* 0xE2 */ AUDIOCMD_OP_GLOBAL_SET_INSTRUMENT_FONT,
    /* 0xE3 */ AUDIOCMD_OP_GLOBAL_POP_PERSISTENT_CACHE,
    /* 0xE4 */ AUDIOCMD_OP_GLOBAL_SET_CUSTOM_FUNCTION,
    /* 0xE5 */ AUDIOCMD_OP_GLOBAL_E5,
    /* 0xE6 */ AUDIOCMD_OP_GLOBAL_SET_REVERB_DATA,
    /* 0xF0 */ AUDIOCMD_OP_GLOBAL_SET_SOUND_MODE = 0xF0,
    /* 0xF1 */ AUDIOCMD_OP_GLOBAL_MUTE,
    /* 0xF2 */ AUDIOCMD_OP_GLOBAL_UNMUTE,
    /* 0xF3 */ AUDIOCMD_OP_GLOBAL_SYNC_LOAD_INSTRUMENT,
    /* 0xF4 */ AUDIOCMD_OP_GLOBAL_ASYNC_LOAD_SAMPLE_BANK,
    /* 0xF5 */ AUDIOCMD_OP_GLOBAL_ASYNC_LOAD_FONT,
    /* 0xF6 */ AUDIOCMD_OP_GLOBAL_DISCARD_SEQ_FONTS,
    /* 0xF8 */ AUDIOCMD_OP_GLOBAL_STOP_AUDIOCMDS = 0xF8,
    /* 0xF9 */ AUDIOCMD_OP_GLOBAL_RESET_AUDIO_HEAP,
    /* 0xFA */ AUDIOCMD_OP_GLOBAL_NOOP_1, // used but no code exists for it
    /* 0xFB */ AUDIOCMD_OP_GLOBAL_SET_CUSTOM_UPDATE_FUNCTION,
    /* 0xFC */ AUDIOCMD_OP_GLOBAL_ASYNC_LOAD_SEQ,
    /* 0xFD */ AUDIOCMD_OP_GLOBAL_NOOP_2, // used but no code exists for it
    /* 0xFE */ AUDIOCMD_OP_GLOBAL_DISABLE_ALL_SEQPLAYERS
} AudioThreadCmdId;

#define MK_CMD(b0, b1, b2, b3) (_SHIFTL(b0, 24, 8) | _SHIFTL(b1, 16, 8) | _SHIFTL(b2, 8, 8) | _SHIFTL(b3, 0, 8))



// ==== Audio Thread Channel Commands ====

/**
 * Set the volumeScale on a given channel
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param channelIndex the index of the seqPlayer to modify
 * @param volume the target volume for the sequence. Ranged from 0-0xFF, with 0x7F mapping to 1.0f
 */
#define AUDIOCMD_CHANNEL_VOL_SCALE(seqPlayerIndex, channelIndex, volumeScale) \
    AudioThread_QueueCmdF32(MK_CMD(AUDIOCMD_OP_CHANNEL_VOL_SCALE, seqPlayerIndex, channelIndex, 0), volumeScale)

/**
 * Set the volume on a given channel
 */

/**
 * Set channel volume.
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param channelIndex the index of the seqPlayer to modify
 * @param volume the target volume for the sequence. Ranged from 0-0xFF, with 0x7F mapping to 1.0f
 */
#define AUDIOCMD_CHANNEL_VOL(seqPlayerIndex, channelIndex, volume) \
    AudioThread_QueueCmdF32(MK_CMD(AUDIOCMD_OP_CHANNEL_VOL, seqPlayerIndex, channelIndex, 0), volume)

/**
 * Set pan signed.
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param channelIndex the index of the seqPlayer to modify
 * @param panSigned
 */
#define AUDIOCMD_CHANNEL_PAN_SIGNED(seqPlayerIndex, channelIndex, panSigned) \
    AudioThread_QueueCmdS8(MK_CMD(AUDIOCMD_OP_CHANNEL_PAN_SIGNED, seqPlayerIndex, channelIndex, 0), panSigned)

/**
 * Set frequency scale.
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param channelIndex the index of the seqPlayer to modify
 * @param freqScale
 */
#define AUDIOCMD_CHANNEL_FREQ_SCALE(seqPlayerIndex, channelIndex, freqScale) \
    AudioThread_QueueCmdF32(MK_CMD(AUDIOCMD_OP_CHANNEL_FREQ_SCALE, seqPlayerIndex, channelIndex, 0), freqScale)

/**
 * Set reverb volume.
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param channelIndex the index of the seqPlayer to modify
 * @param reverbVolume
 */
#define AUDIOCMD_CHANNEL_REVERB_VOLUME(seqPlayerIndex, channelIndex, reverbVolume) \
    AudioThread_QueueCmdS8(MK_CMD(AUDIOCMD_OP_CHANNEL_REVERB_VOLUME, seqPlayerIndex, channelIndex, 0), reverbVolume)

/**
 * Set channel io.
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param channelIndex the index of the seqPlayer to modify
 * @param ioPort
 * @param ioData
 */
#define AUDIOCMD_CHANNEL_IO(seqPlayerIndex, channelIndex, ioPort, ioData) \
    AudioThread_QueueCmdS8(MK_CMD(AUDIOCMD_OP_CHANNEL_IO, (seqPlayerIndex), (channelIndex), (ioPort)), (ioData))

/**
 * Set pan unsigned.
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param channelIndex the index of the seqPlayer to modify
 * @param panUnsigned
 */
#define AUDIOCMD_CHANNEL_PAN_UNSIGNED(seqPlayerIndex, channelIndex, panUnsigned) \
    AudioThread_QueueCmdS8(MK_CMD(AUDIOCMD_OP_CHANNEL_PAN_UNSIGNED, seqPlayerIndex, channelIndex, 0), panUnsigned)

/**
 * Mute a specified channel.
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param channelIndex the index of the seqPlayer to modify
 * @param muted
 */
#define AUDIOCMD_CHANNEL_MUTE(seqPlayerIndex, channelIndex, muted) \
    AudioThread_QueueCmdS8(MK_CMD(AUDIOCMD_OP_CHANNEL_MUTE, seqPlayerIndex, channelIndex, 0), muted)

/**
 * Set the muteFlags for a specified channel.
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param channelIndex the index of the seqPlayer to modify
 * @param muteFlags
 */
#define AUDIOCMD_CHANNEL_MUTE_FLAGS(seqPlayerIndex, channelIndex, muteFlags) \
    AudioThread_QueueCmdS8(MK_CMD(AUDIOCMD_OP_CHANNEL_MUTE_FLAGS, seqPlayerIndex, channelIndex, 0), muteFlags)

/**
 * Apply a small amount of vibrato (x8).
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param channelIndex the index of the seqPlayer to modify
 * @param vibratoExtentTarget
 */
#define AUDIOCMD_CHANNEL_VIBRATO_SMALL(seqPlayerIndex, channelIndex, vibratoExtentTarget)              \
    AudioThread_QueueCmdS8(MK_CMD(AUDIOCMD_OP_CHANNEL_VIBRATO_SMALL, seqPlayerIndex, channelIndex, 0), \
                           vibratoExtentTarget)

/**
 * Apply a large amount of vibrato (x32).
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param channelIndex the index of the seqPlayer to modify
 * @param vibratoExtentTarget
 */
#define AUDIOCMD_CHANNEL_VIBRATO_LARGE(seqPlayerIndex, channelIndex, vibratoExtentTarget)              \
    AudioThread_QueueCmdS8(MK_CMD(AUDIOCMD_OP_CHANNEL_VIBRATO_LARGE, seqPlayerIndex, channelIndex, 0), \
                           vibratoExtentTarget)

/**
 * Set the comb filter size.
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param channelIndex the index of the seqPlayer to modify
 * @param combFilterSize
 */
#define AUDIOCMD_CHANNEL_COMB_FILTER_SIZE(seqPlayerIndex, channelIndex, combFilterSize)                   \
    AudioThread_QueueCmdS8(MK_CMD(AUDIOCMD_OP_CHANNEL_COMB_FILTER_SIZE, seqPlayerIndex, channelIndex, 0), \
                           combFilterSize)

/**
 * Set the comb filter gain.
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param channelIndex the index of the seqPlayer to modify
 * @param combFilterGain
 */
#define AUDIOCMD_CHANNEL_COMB_FILTER_GAIN(seqPlayerIndex, channelIndex, combFilterGain)                    \
    AudioThread_QueueCmdU16(MK_CMD(AUDIOCMD_OP_CHANNEL_COMB_FILTER_GAIN, seqPlayerIndex, channelIndex, 0), \
                            combFilterGain)

/**
 * Set the stereo bits.
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param channelIndex the index of the seqPlayer to modify
 * @param stereoData
 */
#define AUDIOCMD_CHANNEL_STEREO(seqPlayerIndex, channelIndex, stereoData) \
    AudioThread_QueueCmdS8(MK_CMD(AUDIOCMD_OP_CHANNEL_STEREO, seqPlayerIndex, channelIndex, 0), stereoData)

/**
 * Set the start position of a sample.
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param channelIndex the index of the seqPlayer to modify
 * @param startSamplePos
 */
#define AUDIOCMD_CHANNEL_SET_START_POS(seqPlayerIndex, channelIndex, startSamplePos) \
    AudioThread_QueueCmdS32(MK_CMD(AUDIOCMD_OP_CHANNEL_SET_START_POS, seqPlayerIndex, channelIndex, 0), startSamplePos)

/**
 * Set the address of the sfxState to read from.
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param channelIndex the index of the seqPlayer to modify
 * @param sfxState
 */
#define AUDIOCMD_CHANNEL_SFX_STATE(seqPlayerIndex, channelIndex, sfxState) \
    AudioThread_QueueCmdS32(MK_CMD(AUDIOCMD_OP_CHANNEL_SFX_STATE, seqPlayerIndex, channelIndex, 0), sfxState)

/**
 * Set the reverb index.
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param channelIndex the index of the seqPlayer to modify
 * @param reverbIndex
 */
#define AUDIOCMD_CHANNEL_REVERB_INDEX(seqPlayerIndex, channelIndex, reverbIndex) \
    AudioThread_QueueCmdS8(MK_CMD(AUDIOCMD_OP_CHANNEL_REVERB_INDEX, seqPlayerIndex, channelIndex, 0), reverbIndex)

/**
 * Set the surround effect index.
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param channelIndex the index of the seqPlayer to modify
 * @param surroundEffectIndex
 */
#define AUDIOCMD_CHANNEL_SURROUND_EFFECT_INDEX(seqPlayerIndex, channelIndex, surroundEffectIndex)              \
    AudioThread_QueueCmdS8(MK_CMD(AUDIOCMD_OP_CHANNEL_SURROUND_EFFECT_INDEX, seqPlayerIndex, channelIndex, 0), \
                           surroundEffectIndex)

/**
 * Set a filter.
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param channelIndex the index of the seqPlayer to modify
 * @param filterCutoff
 * @param filter
 */
#define AUDIOCMD_CHANNEL_FILTER(seqPlayerIndex, channelIndex, filterCutoff, filter) \
    AudioThread_QueueCmdS32(MK_CMD(AUDIOCMD_OP_CHANNEL_FILTER, seqPlayerIndex, channelIndex, filterCutoff), filter)

/**
 * Set the gain.
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param channelIndex the index of the seqPlayer to modify
 * @param gain
 */
#define AUDIOCMD_CHANNEL_GAIN(seqPlayerIndex, channelIndex, gain) \
    AudioThread_QueueCmdS8(MK_CMD(AUDIOCMD_OP_CHANNEL_GAIN, seqPlayerIndex, channelIndex, 0), gain)



// ==== Audio Thread SeqPlayer Commands ====

/**
 * Set the fade volume scale.
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param fadeVolumeScale
 */
#define AUDIOCMD_SEQPLAYER_FADE_VOLUME_SCALE(seqPlayerIndex, fadeVolumeScale) \
    AudioThread_QueueCmdF32(MK_CMD(AUDIOCMD_OP_SEQPLAYER_FADE_VOLUME_SCALE, seqPlayerIndex, 0, 0), fadeVolumeScale)

/**
 * Set the sequence player global io
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param ioPort
 * @param ioData
 */
#define AUDIOCMD_SEQPLAYER_SET_IO(seqPlayerIndex, ioPort, ioData) \
    AudioThread_QueueCmdS8(MK_CMD(AUDIOCMD_OP_SEQPLAYER_SET_IO, seqPlayerIndex, 0, ioPort), ioData)

/**
 * Set the tempo.
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param tempo
 */
#define AUDIOCMD_SEQPLAYER_SET_TEMPO(seqPlayerIndex, tempo) \
    AudioThread_QueueCmdS32(MK_CMD(AUDIOCMD_OP_SEQPLAYER_SET_TEMPO, seqPlayerIndex, 0, 0), tempo)

/**
 * Set the transposition.
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param transposition
 */
#define AUDIOCMD_SEQPLAYER_SET_TRANSPOSITION(seqPlayerIndex, transposition) \
    AudioThread_QueueCmdS8(MK_CMD(AUDIOCMD_OP_SEQPLAYER_SET_TRANSPOSITION, seqPlayerIndex, 0, 0), transposition)

/**
 * Set unk_0C and scale (tempo-related).
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param unk_0C
 */
#define AUDIOCMD_SEQPLAYER_SET_SCALED_UNK_0C(seqPlayerIndex, unk_0C) \
    AudioThread_QueueCmdS32(MK_CMD(AUDIOCMD_OP_SEQPLAYER_SET_SCALED_UNK_0C, seqPlayerIndex, 0, 0), unk_0C)

/**
 * Set unk_0C (tempo-related).
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param unk_0C
 */
#define AUDIOCMD_SEQPLAYER_SET_UNK_0C(seqPlayerIndex, unk_0C) \
    AudioThread_QueueCmdS32(MK_CMD(AUDIOCMD_OP_SEQPLAYER_SET_UNK_0C, seqPlayerIndex, 0, 0), unk_0C)

/**
 * Set the fade volume.
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param fadeVolume
 * @param fadeTimer
 */
#define AUDIOCMD_SEQPLAYER_SET_FADE_VOLUME(seqPlayerIndex, fadeVolume, fadeTimer) \
    AudioThread_QueueCmdS32(MK_CMD(AUDIOCMD_OP_SEQPLAYER_SET_FADE_VOLUME, seqPlayerIndex, fadeVolume, 0), fadeTimer)

/**
 * Set the fade volume and scale.
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param fadeVolume
 * @param fadeTimer
 */
#define AUDIOCMD_SEQPLAYER_SCALE_FADE_VOLUME(seqPlayerIndex, fadeVolume, fadeTimer) \
    AudioThread_QueueCmdS32(MK_CMD(AUDIOCMD_OP_SEQPLAYER_SCALE_FADE_VOLUME, seqPlayerIndex, fadeVolume, 0), fadeTimer)

/**
 * Set the fade timer.
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param fadeTimer
 */
#define AUDIOCMD_SEQPLAYER_SET_FADE_TIMER(seqPlayerIndex, fadeTimer) \
    AudioThread_QueueCmdS32(MK_CMD(AUDIOCMD_OP_SEQPLAYER_SET_FADE_TIMER, seqPlayerIndex, 0, 0), fadeTimer)

/**
 * Set the bend.
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param bend
 */
#define AUDIOCMD_SEQPLAYER_SET_BEND(seqPlayerIndex, bend) \
    AudioThread_QueueCmdF32(MK_CMD(AUDIOCMD_OP_SEQPLAYER_SET_BEND, seqPlayerIndex, 0, 0), bend)



// ==== Audio Thread Global Commands ====

/**
 * Synchronously load a sequence in parts.
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param seqId
 * @param arg2
 * @param data
 */
#define AUDIOCMD_GLOBAL_SYNC_LOAD_SEQ_PARTS(seqPlayerIndex, seqId, arg2, data) \
    AudioThread_QueueCmdS32(MK_CMD(AUDIOCMD_OP_GLOBAL_SYNC_LOAD_SEQ_PARTS, seqPlayerIndex, seqId, arg2), data)

/**
 * Synchronously initialize a sequence player
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param seqId
 * @param arg2
 * @param fadeTimer
 */
#define AUDIOCMD_GLOBAL_SYNC_INIT_SEQPLAYER(seqPlayerIndex, seqId, arg2, fadeTimer) \
    AudioThread_QueueCmdS32(MK_CMD(AUDIOCMD_OP_GLOBAL_SYNC_INIT_SEQPLAYER, seqPlayerIndex, seqId, arg2), fadeTimer)

/**
 * Disable a sequence player.
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param fadeTimer
 */
#define AUDIOCMD_GLOBAL_DISABLE_SEQPLAYER(seqPlayerIndex, fadeTimer) \
    AudioThread_QueueCmdS32(MK_CMD(AUDIOCMD_OP_GLOBAL_DISABLE_SEQPLAYER, seqPlayerIndex, 0, 0), fadeTimer)

/**
 * Synchronously initialize a sequence player and skip ticks.
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param seqId
 * @param skipTicks
 */
#define AUDIOCMD_GLOBAL_SYNC_INIT_SEQPLAYER_SKIP_TICKS(seqPlayerIndex, seqId, skipTicks)                         \
    AudioThread_QueueCmdS32(MK_CMD(AUDIOCMD_OP_GLOBAL_SYNC_INIT_SEQPLAYER_SKIP_TICKS, seqPlayerIndex, seqId, 0), \
                            skipTicks)

/**
 * Set active channel flags.
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param activeChannelsFlags
 */
#define AUDIOCMD_GLOBAL_SET_ACTIVE_CHANNEL_FLAGS(seqPlayerIndex, activeChannelsFlags)                  \
    AudioThread_QueueCmdU16(MK_CMD(AUDIOCMD_OP_GLOBAL_SET_ACTIVE_CHANNEL_FLAGS, seqPlayerIndex, 0, 0), \
                            activeChannelsFlags)

/**
 * Pop the persistent cache of the specified table.
 *
 * @param tableType
 */
#define AUDIOCMD_GLOBAL_POP_PERSISTENT_CACHE(tableType) \
    AudioThread_QueueCmdS32(MK_CMD(AUDIOCMD_OP_GLOBAL_POP_PERSISTENT_CACHE, 0, 0, 0), tableType)

/**
 * Set a custom function.
 *
 * @param functionType
 * @param functionPtr
 */
#define AUDIOCMD_GLOBAL_SET_CUSTOM_FUNCTION(functionType, functionPtr) \
    AudioThread_QueueCmdS32(MK_CMD(AUDIOCMD_OP_GLOBAL_SET_CUSTOM_FUNCTION, 0, 0, functionType), functionPtr)

/**
 * Do something related to the unloaded-type audio data in the heap.
 *
 * @param tableType
 * @param id
 * @param type
 * @param data
 */
#define AUDIOCMD_GLOBAL_E5(tableType, id, type, data) \
    AudioThread_QueueCmdS32(MK_CMD(AUDIOCMD_OP_GLOBAL_E5, tableType, id, type), data)

/**
 * Set reverb data.
 *
 * @param reverbIndex
 * @param dataType
 * @param data
 */
#define AUDIOCMD_GLOBAL_SET_REVERB_DATA(reverbIndex, dataType, data) \
    AudioThread_QueueCmdS32(MK_CMD(AUDIOCMD_OP_GLOBAL_SET_REVERB_DATA, dataType, reverbIndex, 0), data)

/**
 * Set sound mode.
 *
 * @param soundMode
 */
#define AUDIOCMD_GLOBAL_SET_SOUND_MODE(soundMode) \
    AudioThread_QueueCmdS32(MK_CMD(AUDIOCMD_OP_GLOBAL_SET_SOUND_MODE, 0, 0, 0), soundMode)

/**
 * Mute a sequence player.
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 */
#define AUDIOCMD_GLOBAL_MUTE(seqPlayerIndex) \
    AudioThread_QueueCmdS32(MK_CMD(AUDIOCMD_OP_GLOBAL_MUTE, seqPlayerIndex, 0, 0), 0)

/**
 * Unmute a sequence player.
 *
 * @param seqPlayerIndex the index of the seqPlayer to modify
 * @param restart
 */
#define AUDIOCMD_GLOBAL_UNMUTE(seqPlayerIndex, restart) \
    AudioThread_QueueCmdS32(MK_CMD(AUDIOCMD_OP_GLOBAL_UNMUTE, seqPlayerIndex, 0, 0), restart)

/**
 * Synchronously load an instrument.
 *
 * @param fontId
 * @param instId
 * @param drumId
 */
#define AUDIOCMD_GLOBAL_SYNC_LOAD_INSTRUMENT(fontId, instId, drumId) \
    AudioThread_QueueCmdS8(MK_CMD(AUDIOCMD_OP_GLOBAL_SYNC_LOAD_INSTRUMENT, fontId, instId, drumId), 0)

/**
 * Asynchronously load a sample bank.
 *
 * @param sampleBankId
 * @param arg1
 * @param retData
 */
#define AUDIOCMD_GLOBAL_ASYNC_LOAD_SAMPLE_BANK(sampleBankId, arg1, retData) \
    AudioThread_QueueCmdS8(MK_CMD(AUDIOCMD_OP_GLOBAL_ASYNC_LOAD_SAMPLE_BANK, sampleBankId, arg1, retData), 0)

/**
 * Asynchronously load a font.
 *
 * @param fontId
 * @param arg1
 * @param retData
 */
#define AUDIOCMD_GLOBAL_ASYNC_LOAD_FONT(fontId, arg1, retData) \
    AudioThread_QueueCmdS8(MK_CMD(AUDIOCMD_OP_GLOBAL_ASYNC_LOAD_FONT, fontId, arg1, retData), 0)

/**
 * Discard sequence fonts.
 *
 * @param seqId
 */
#define AUDIOCMD_GLOBAL_DISCARD_SEQ_FONTS(seqId) \
    AudioThread_QueueCmdS32(MK_CMD(AUDIOCMD_OP_GLOBAL_DISCARD_SEQ_FONTS, 0, seqId, 0), 0)

/**
 * Stop processing all audio thread commands.
 */
#define AUDIOCMD_GLOBAL_STOP_AUDIOCMDS() AudioThread_QueueCmdS32(MK_CMD(AUDIOCMD_OP_GLOBAL_STOP_AUDIOCMDS, 0, 0, 0), 0)

/**
 * Reset Audio Heap.
 *
 * @param specId
 */
#define AUDIOCMD_GLOBAL_RESET_AUDIO_HEAP(specId) \
    AudioThread_QueueCmdS32(MK_CMD(AUDIOCMD_OP_GLOBAL_RESET_AUDIO_HEAP, 0, 0, 0), specId)

/**
 * No Operation. No code is programmed for this operation.
 *
 * @param arg0
 * @param arg1
 * @param arg2
 * @param data
 */
#define AUDIOCMD_GLOBAL_NOOP_1(arg0, arg1, arg2, data) \
    AudioThread_QueueCmdS32(MK_CMD(AUDIOCMD_OP_GLOBAL_NOOP_1, arg0, arg1, arg2), data)

/**
 * Set a custom function that runs every audio thread update.
 *
 * @param functionPtr
 */
#define AUDIOCMD_GLOBAL_SET_CUSTOM_UPDATE_FUNCTION(functionPtr) \
    AudioThread_QueueCmdS32(MK_CMD(AUDIOCMD_OP_GLOBAL_SET_CUSTOM_UPDATE_FUNCTION, 0, 0, 0), functionPtr)

/**
 * Asynchronously load a sequence.
 *
 * @param seqId
 * @param arg1
 * @param retData
 */
#define AUDIOCMD_GLOBAL_ASYNC_LOAD_SEQ(seqId, arg1, retData) \
    AudioThread_QueueCmdS8(MK_CMD(AUDIOCMD_OP_GLOBAL_ASYNC_LOAD_SEQ, seqId, arg1, retData), 0)

/**
 * No Operation. No code is programmed for this operation.
 *
 * @param arg0
 * @param arg1
 * @param arg2
 * @param data
 */
#define AUDIOCMD_GLOBAL_NOOP_2(arg0, arg1, arg2, data) \
    AudioThread_QueueCmdS32(MK_CMD(AUDIOCMD_OP_GLOBAL_NOOP_2, arg0, arg1, arg2), data)

/**
 * Disable all sequence players.
 *
 * @param flags
 */
#define AUDIOCMD_GLOBAL_DISABLE_ALL_SEQPLAYERS(flags) \
    AudioThread_QueueCmdS32(MK_CMD(AUDIOCMD_OP_GLOBAL_DISABLE_ALL_SEQPLAYERS, 0, 0, 0), flags)

#endif
