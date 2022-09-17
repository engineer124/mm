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
    /* 0x06 */ AUDIOCMD_OP_CHANNEL_SCRIPT_IO,
    /* 0x07 */ AUDIOCMD_OP_CHANNEL_PAN_UNSIGNED,
    /* 0x08 */ AUDIOCMD_OP_CHANNEL_STOP_SOMETHING2,
    /* 0x09 */ AUDIOCMD_OP_CHANNEL_MUTE_FLAGS,
    /* 0x0A */ AUDIOCMD_OP_CHANNEL_VIBRATO_X8,    
    /* 0x0B */ AUDIOCMD_OP_CHANNEL_VIBRATO_X32,   
    /* 0x0C */ AUDIOCMD_OP_CHANNEL_COMB_FILTER_SIZE,
    /* 0x0D */ AUDIOCMD_OP_CHANNEL_COMB_FILTER_GAIN,
    /* 0x0E */ AUDIOCMD_OP_CHANNEL_STEREO,
    /* 0x0F */ AUDIOCMD_OP_CHANNEL_UNK_F,
    /* 0x10 */ AUDIOCMD_OP_CHANNEL_SFX_STATE,
    /* 0x11 */ AUDIOCMD_OP_CHANNEL_REVERB_INDEX,
    /* 0x12 */ AUDIOCMD_OP_CHANNEL_SURROUND_EFFECT_INDEX,
    /* 0x13 */ AUDIOCMD_OP_CHANNEL_FILTER,
    /* 0x14 */ AUDIOCMD_OP_CHANNEL_GAIN,
    /* 0x41 */ AUDIOCMD_OP_SEQPLAYER_41 = 0x41,
    /* 0x46 */ AUDIOCMD_OP_SEQPLAYER_46 = 0x46,
    /* 0x47 */ AUDIOCMD_OP_SEQPLAYER_47,
    /* 0x48 */ AUDIOCMD_OP_SEQPLAYER_48,
    /* 0x49 */ AUDIOCMD_OP_SEQPLAYER_49,
    /* 0x4A */ AUDIOCMD_OP_SEQPLAYER_4A,
    /* 0x4B */ AUDIOCMD_OP_SEQPLAYER_4B,
    /* 0x4C */ AUDIOCMD_OP_SEQPLAYER_4C,
    /* 0x4D */ AUDIOCMD_OP_SEQPLAYER_4D,
    /* 0x4E */ AUDIOCMD_OP_SEQPLAYER_4E,
    /* 0x81 */ AUDIOCMD_OP_GLOBAL_81 = 0x81,
    /* 0x82 */ AUDIOCMD_OP_GLOBAL_82,
    /* 0x83 */ AUDIOCMD_OP_GLOBAL_83, // Disable Sequence
    /* 0x85 */ AUDIOCMD_OP_GLOBAL_85 = 0x85,
    /* 0x90 */ AUDIOCMD_OP_GLOBAL_90 = 0x90,
    /* 0xE0 */ AUDIOCMD_OP_GLOBAL_E0 = 0xE0,
    /* 0xE1 */ AUDIOCMD_OP_GLOBAL_E1,
    /* 0xE2 */ AUDIOCMD_OP_GLOBAL_E2,
    /* 0xE3 */ AUDIOCMD_OP_GLOBAL_E3,
    /* 0xE4 */ AUDIOCMD_OP_GLOBAL_E4,
    /* 0xE5 */ AUDIOCMD_OP_GLOBAL_E5,
    /* 0xE6 */ AUDIOCMD_OP_GLOBAL_E6,
    /* 0xF0 */ AUDIOCMD_OP_GLOBAL_F0 = 0xF0,
    /* 0xF1 */ AUDIOCMD_OP_GLOBAL_F1,
    /* 0xF2 */ AUDIOCMD_OP_GLOBAL_F2,
    /* 0xF3 */ AUDIOCMD_OP_GLOBAL_F3,
    /* 0xF4 */ AUDIOCMD_OP_GLOBAL_F4,
    /* 0xF5 */ AUDIOCMD_OP_GLOBAL_F5,
    /* 0xF6 */ AUDIOCMD_OP_GLOBAL_F6,
    /* 0xF8 */ AUDIOCMD_OP_GLOBAL_F8 = 0xF8,
    /* 0xF9 */ AUDIOCMD_OP_GLOBAL_F9,
    /* 0xFA */ AUDIOCMD_OP_GLOBAL_FA,
    /* 0xFB */ AUDIOCMD_OP_GLOBAL_FB,
    /* 0xFC */ AUDIOCMD_OP_GLOBAL_FC,
    /* 0xFD */ AUDIOCMD_OP_GLOBAL_FD,
    /* 0xFE */ AUDIOCMD_OP_GLOBAL_FE
} AudioThreadCmdId;

#define MK_CMD(b0, b1, b2, b3) (_SHIFTL(b0, 24, 8) | _SHIFTL(b1, 16, 8) | _SHIFTL(b2, 8, 8) | _SHIFTL(b3, 0, 8))

// ==== Audio Thread Channel Commands ====

/**
 * Set the volumeScale on a given channel
 */
#define AUDIOCMD_CHANNEL_VOL_SCALE(seqPlayerIndex, channelIndex, volumeScale) \
    AudioThread_QueueCmdF32(MK_CMD(AUDIOCMD_OP_CHANNEL_VOL_SCALE, seqPlayerIndex, channelIndex, 0), volumeScale)

/**
 * Set the volume on a given channel
 */
#define AUDIOCMD_CHANNEL_VOL(seqPlayerIndex, channelIndex, volume) \
    AudioThread_QueueCmdF32(MK_CMD(AUDIOCMD_OP_CHANNEL_VOL, seqPlayerIndex, channelIndex, 0), volume)

#define AUDIOCMD_CHANNEL_PAN_SIGNED(seqPlayerIndex, channelIndex, panSigned) \
    AudioThread_QueueCmdS8(MK_CMD(AUDIOCMD_OP_CHANNEL_PAN_SIGNED, seqPlayerIndex, channelIndex, 0), panSigned)

#define AUDIOCMD_CHANNEL_FREQ_SCALE(seqPlayerIndex, channelIndex, freqScale) \
    AudioThread_QueueCmdF32(MK_CMD(AUDIOCMD_OP_CHANNEL_FREQ_SCALE, seqPlayerIndex, channelIndex, 0), freqScale)

#define AUDIOCMD_CHANNEL_REVERB_VOLUME(seqPlayerIndex, channelIndex, reverbVolume) \
    AudioThread_QueueCmdS8(MK_CMD(AUDIOCMD_OP_CHANNEL_REVERB_VOLUME, seqPlayerIndex, channelIndex, 0), reverbVolume)

#define AUDIOCMD_CHANNEL_SCRIPT_IO(seqPlayerIndex, channelIndex, ioPort, ioData)                          \
    AudioThread_QueueCmdS8(MK_CMD(AUDIOCMD_OP_CHANNEL_SCRIPT_IO, (seqPlayerIndex), (channelIndex), (ioPort)), \
                           (ioData))

#define AUDIOCMD_CHANNEL_PAN_UNSIGNED(seqPlayerIndex, channelIndex, panUnsigned) \
    AudioThread_QueueCmdS8(MK_CMD(AUDIOCMD_OP_CHANNEL_PAN_UNSIGNED, seqPlayerIndex, channelIndex, 0), panUnsigned)

#define AUDIOCMD_CHANNEL_STOP_SOMETHING2(seqPlayerIndex, channelIndex, stopSomething2) \
    AudioThread_QueueCmdS8(MK_CMD(AUDIOCMD_OP_CHANNEL_STOP_SOMETHING2, seqPlayerIndex, channelIndex, 0), stopSomething2)

#define AUDIOCMD_CHANNEL_MUTE_FLAGS(seqPlayerIndex, channelIndex, muteFlags) \
    AudioThread_QueueCmdS8(MK_CMD(AUDIOCMD_OP_CHANNEL_MUTE_FLAGS, seqPlayerIndex, channelIndex, 0), muteFlags)

#define AUDIOCMD_CHANNEL_VIBRATO_X8(seqPlayerIndex, channelIndex, vibratoExtentTarget) \
    AudioThread_QueueCmdS8(MK_CMD(AUDIOCMD_OP_CHANNEL_VIBRATO_X8, seqPlayerIndex, channelIndex, 0), vibratoExtentTarget)

#define AUDIOCMD_CHANNEL_VIBRATO_X32(seqPlayerIndex, channelIndex, vibratoExtentTarget) \
    AudioThread_QueueCmdS8(MK_CMD(AUDIOCMD_OP_CHANNEL_VIBRATO_X32, seqPlayerIndex, channelIndex, 0), vibratoExtentTarget)

#define AUDIOCMD_CHANNEL_COMB_FILTER_SIZE(seqPlayerIndex, channelIndex, combFilterSize) \
    AudioThread_QueueCmdS8(MK_CMD(AUDIOCMD_OP_CHANNEL_COMB_FILTER_SIZE, seqPlayerIndex, channelIndex, 0), combFilterSize)

#define AUDIOCMD_CHANNEL_COMB_FILTER_GAIN(seqPlayerIndex, channelIndex, combFilterGain) \
    AudioThread_QueueCmdU16(MK_CMD(AUDIOCMD_OP_CHANNEL_COMB_FILTER_GAIN, seqPlayerIndex, channelIndex, 0), combFilterGain)

#define AUDIOCMD_CHANNEL_STEREO(seqPlayerIndex, channelIndex, stereoData) \
    AudioThread_QueueCmdS8(MK_CMD(AUDIOCMD_OP_CHANNEL_STEREO, seqPlayerIndex, channelIndex, 0), stereoData)

#define AUDIOCMD_CHANNEL_UNK_F(seqPlayerIndex, channelIndex, unk_DC) \
    AudioThread_QueueCmdS32(MK_CMD(AUDIOCMD_OP_CHANNEL_UNK_F, seqPlayerIndex, channelIndex, 0), unk_DC)

#define AUDIOCMD_CHANNEL_SFX_STATE(seqPlayerIndex, channelIndex, sfxState) \
    AudioThread_QueueCmdS32(MK_CMD(AUDIOCMD_OP_CHANNEL_SFX_STATE, seqPlayerIndex, channelIndex, 0), sfxState)

#define AUDIOCMD_CHANNEL_REVERB_INDEX(seqPlayerIndex, channelIndex, reverbIndex) \
    AudioThread_QueueCmdS8(MK_CMD(AUDIOCMD_OP_CHANNEL_REVERB_INDEX, seqPlayerIndex, channelIndex, 0), reverbIndex)

#define AUDIOCMD_CHANNEL_SURROUND_EFFECT_INDEX(seqPlayerIndex, channelIndex, surroundEffectIndex) \
    AudioThread_QueueCmdS8(MK_CMD(AUDIOCMD_OP_CHANNEL_SURROUND_EFFECT_INDEX, seqPlayerIndex, channelIndex, 0), surroundEffectIndex)

#define AUDIOCMD_CHANNEL_FILTER(seqPlayerIndex, channelIndex, filterCutoff, filter) \
    AudioThread_QueueCmdS32(MK_CMD(AUDIOCMD_OP_CHANNEL_FILTER, seqPlayerIndex, channelIndex, filterCutoff), filter)

#define AUDIOCMD_CHANNEL_GAIN(seqPlayerIndex, channelIndex, gain) \
    AudioThread_QueueCmdS8(MK_CMD(AUDIOCMD_OP_CHANNEL_GAIN, seqPlayerIndex, channelIndex, 0), gain)

// ==== Audio Thread SeqPlayer Commands ====


// ==== Audio Thread Global Commands ====



#endif
