#ifndef Z64MESSAGE_H
#define Z64MESSAGE_H

#include "PR/ultratypes.h"
#include "z64view.h"
#include "unk.h"

struct Actor;
struct MessageTableEntry;
struct OcarinaStaff;

typedef enum TextState {
    /*  0 */ TEXT_STATE_NONE,
    /*  1 */ TEXT_STATE_1,
    /*  2 */ TEXT_STATE_CLOSING,
    /*  3 */ TEXT_STATE_3,
    /*  4 */ TEXT_STATE_CHOICE,
    /*  5 */ TEXT_STATE_5,
    /*  6 */ TEXT_STATE_DONE,
    /*  7 */ TEXT_STATE_7,
    /*  8 */ TEXT_STATE_8,
    /*  9 */ TEXT_STATE_9,
    /* 10 */ TEXT_STATE_10,
    /* 11 */ TEXT_STATE_11,
    /* 12 */ TEXT_STATE_12,
    /* 13 */ TEXT_STATE_13,
    /* 14 */ TEXT_STATE_14,
    /* 15 */ TEXT_STATE_15,
    /* 16 */ TEXT_STATE_16,
    /* 17 */ TEXT_STATE_17,
    /* 18 */ TEXT_STATE_18
} TextState;


#define FONT_CHAR_TEX_WIDTH 16
#define FONT_CHAR_TEX_HEIGHT 16
//! @TODO: Make this use `sizeof(AnyFontTextureSymbol)`
#define FONT_CHAR_TEX_SIZE ((16 * 16) / 2) // 16x16 I4 texture

// TODO: should Font be in its own header or is it fine to have it here?
// Font textures are loaded into here
typedef struct {
    /* 0x00000 */ union {
        u8 charBuf[2][FONT_CHAR_TEX_SIZE * 120];
        u64 force_structure_alignment_charTex;
    };
    /* 0x07800 */ union {
        u8 iconBuf[FONT_CHAR_TEX_SIZE];
        u64 force_structure_alignment_icon;
    };
    /* 0x07880 */ union {
        u8 fontBuf[FONT_CHAR_TEX_SIZE * 320];
        u64 force_structure_alignment_font;
    };
    /* 0x11880 */ union {
        char schar[1280]; // msgBuf
        u16 wchar[640];   // msgBufWide
        u64 force_structure_alignment_msg;
    } msgBuf;
    /* 0x11D80 */ u32 messageStart;
    /* 0x11D84 */ u32 messageEnd;
    /* 0x11D88 */ u8 unk_11D88; // current Char Buffer ?
} Font; // size = 0x11D90

typedef struct MessageContext {
    /* 0x00000 */ View view;
    /* 0x00168 */ Font font;
    /* 0x11EF8 */ u8* textboxSegment;
    /* 0x11EFC */ UNK_TYPE1 unk11EFC[0x4];
    /* 0x11F00 */ struct OcarinaStaff* ocarinaStaff;
    /* 0x11F04 */ u16 currentTextId;
    /* 0x11F06 */ UNK_TYPE1 unk11F06[0x2];
    /* 0x11F08 */ u16 unk11F08;
    /* 0x11F0A */ u8 textBoxType;
    /* 0x11F0B */ u8 textBoxPos;
    /* 0x11F0C */ u8 unk11F0C;
    /* 0x11F0B */ UNK_TYPE1 unk11F0D[0x3];
    /* 0x11F10 */ s32 msgLength;
    /* 0x11F14 */ u16 unk11F14;
    /* 0x11F16 */ u16 itemId;
    /* 0x11F18 */ s8 unk11F18;
    /* 0x11F19 */ UNK_TYPE1 unk11F19[0x1];
    /* 0x11F1A */ s16 unk11F1A[3];
    /* 0x11F20 */ UNK_TYPE1 unk11F20[0x2];
    /* 0x11F22 */ u8 msgMode; // TODO: MessageMode enum
    /* 0x11F23 */ UNK_TYPE1 unk11F23;
    /* 0x11F24 */ union {
        char schar[200];
        u16 wchar[100];
    } decodedBuffer;
    /* 0x11FEC */ u16 msgBufPos;
    /* 0x11FEE */ u16 textDrawPos;
    /* 0x11FF0 */ u16 decodedTextLen;
    /* 0x11FF2 */ u16 textUnskippable;
    /* 0x11FF4 */ s16 textPosX;
    /* 0x11FF6 */ s16 textPosY;
    /* 0x11FF8 */ s16 unk11FF8;
    /* 0x11FFA */ s16 unk11FFA;
    /* 0x11FFC */ s16 unk11FFC;
    /* 0x11FFE */ s16 unk11FFE[0x3];
    /* 0x12004 */ s16 textboxXTarget;
    /* 0x12006 */ s16 textboxYTarget;
    /* 0x12008 */ s16 unk12008;
    /* 0x1200A */ UNK_TYPE2 unk1200A;
    /* 0x1200C */ s16 unk1200C;
    /* 0x1200E */ s16 unk1200E;
    /* 0x12010 */ s16 unk12010;
    /* 0x12012 */ s16 unk12012;
    /* 0x12014 */ s16 unk12014;
    /* 0x12014 */ s16 unk12016;
    /* 0x12018 */ s16 unk12018; // messageR
    /* 0x1201A */ s16 unk1201A; // messageG
    /* 0x1201C */ s16 unk1201C; // messageB
    /* 0x1201E */ s16 unk1201E; // messageA
    /* 0x12020 */ u8 unk12020;  // probably textboxEndType
    /* 0x12021 */ u8 choiceIndex;
    /* 0x12022 */ u8 unk12022;
    /* 0x12023 */ u8 stateTimer;
    /* 0x12024 */ s16 unk12024;
    /* 0x12026 */ u16 unk12026;
    /* 0x12028 */ u16 songPlayed;
    /* 0x1202A */ u16 ocarinaMode;
    /* 0x1202C */ u16 ocarinaAction;
    /* 0x1202E */ u16 lastPlayedSong;
    /* 0x12030 */ s16 unk_12030;
    /* 0x12032 */ UNK_TYPE1 unk_12032[0x2];
    /* 0x12034 */ s16 textboxColorRed;
    /* 0x12036 */ s16 textboxColorGreen;
    /* 0x12038 */ s16 textboxColorBlue;
    /* 0x1203A */ s16 textboxColorAlphaTarget;
    /* 0x1203C */ s16 textboxColorAlphaCurrent;
    /* 0x1203E */ s16 unk1203E;
    /* 0x12040 */ struct Actor* talkActor;
    /* 0x12044 */ s16 unk12044;
    /* 0x12046 */ s16 blockSunsSong;
    /* 0x12048 */ u8 unk12048; // EnKakasi
    /* 0x12049 */ UNK_TYPE1 unk12049[0x1];
    /* 0x1204A */ s16 ocarinaButtonsPosY[0x5];
    /* 0x12054 */ s16 unk12054[6]; // First, second and third digits in lottery code guess
    /* 0x1205A */ UNK_TYPE1 unk12060[0x8];
    /* 0x12068 */ s16 textboxX;
    /* 0x1206A */ s16 textboxY;
    /* 0x1206C */ s32 unk1206C;
    /* 0x12070 */ s32 unk12070;
    /* 0x12074 */ s32 unk12074;
    /* 0x12078 */ s32 bankRupeesSelected;
    /* 0x1207C */ s32 bankRupees;
    /* 0x12080 */ struct MessageTableEntry* messageEntryTable;
    /* 0x12084 */ struct MessageTableEntry* messageEntryTableNes;
    /* 0x12088 */ UNK_TYPE4 unk12088;
    /* 0x1208C */ struct MessageTableEntry* messageTableStaff;
    /* 0x12090 */ s16 textIsCredits;
    /* 0x12092 */ s16 unk12092;
    /* 0x12094 */ s8 unk12094;
    /* 0x12095 */ UNK_TYPE1 unk12095[0x3];
    /* 0x12098 */ f32 unk12098; // Text_Scale?
    /* 0x1209C */ s16 textFade;
    /* 0x1209E */ UNK_TYPE1 unk1209E[0x2];
    /* 0x120A0 */ s32 ocarinaAvailableSongs;
    /* 0x120A4 */ s16 unk120A4;
    /* 0x120A6 */ s16 unk120A6;
    /* 0x120A8 */ s16 unk120A8;
    /* 0x120AA */ s16 unk120AA;
    /* 0x120AC */ s16 unk120AC;
    /* 0x120AE */ s16 unk120AE;
    /* 0x120B0 */ u8 unk120B0;
    /* 0x120B1 */ u8 unk120B1;
    /* 0x120B2 */ u8 unk120B2[0xA];
    /* 0x120BC */ u16 unk_120BC;
    /* 0x120BE */ s16 unk120BE;
    /* 0x120C0 */ s16 unk120C0;
    /* 0x120C2 */ s16 unk120C2;
    /* 0x120C4 */ s32 unk120C4;
    /* 0x120C8 */ s16 unk120C8;
    /* 0x120CA */ s16 unk120CA;
    /* 0x120CC */ s16 unk120CC;
    /* 0x120CE */ s16 unk120CE;
    /* 0x120D0 */ s16 unk120D0;
    /* 0x120D2 */ s16 unk120D2;
    /* 0x120D4 */ s16 unk120D4;
    /* 0x120D6 */ s16 unk120D6;
    /* 0x120D8 */ UNK_TYPE1 unk120D8[0x8];
} MessageContext; // size = 0x120E0

#endif
