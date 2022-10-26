#include "global.h"

s8 D_801C6A70 = 0;
s16 D_801C6A78 = 0;
s16 D_801C6A7C = 255;

s16 sOcarinaButtonStepR = 0;

s16 sOcarinaButtonStepG = 0;

s16 sOcarinaButtonStepB = 0;

s16 sOcarinaButtonFlashTimer = 12;

s16 sOcarinaButtonFlashColorIndex = 1;

s16 D_801C6A94 = 0;
u8 D_801C6A98[6][5] = {
    { BTN_ENABLED, BTN_ENABLED, BTN_DISABLED, BTN_ENABLED, BTN_ENABLED },
    { BTN_ENABLED, BTN_ENABLED, BTN_DISABLED, BTN_ENABLED, BTN_ENABLED },
    { BTN_ENABLED, BTN_DISABLED, BTN_DISABLED, BTN_ENABLED, BTN_ENABLED },
    { BTN_ENABLED, BTN_DISABLED, BTN_DISABLED, BTN_ENABLED, BTN_ENABLED },
    { BTN_ENABLED, BTN_ENABLED, BTN_DISABLED, BTN_ENABLED, BTN_ENABLED },
    { BTN_ENABLED, BTN_ENABLED, BTN_DISABLED, BTN_ENABLED, BTN_ENABLED },
};

u16 D_801C6AB8[] = {
    0x2147, 0x2134, 0x2135, 0x2136, 0x2137, 0x2138, 0x2139, 0x213A, 0x213B, 0x213C, 0x213D, 0x213E, 0x213F, 0x2140,
    0x2141, 0x2142, 0x2143, 0x2144, 0x2145, 0x2146, 0x2152, 0x2153, 0x2154, 0x0000, 0x2156, 0x2157, 0x2158, 0x2159,
    0x215A, 0x215B, 0x0000, 0x215D, 0x0000, 0x215F, 0x2160, 0x2161, 0x0000, 0x2163, 0x2164, 0x2165, 0x2166, 0x2167,
    0x2168, 0x2169, 0x216A, 0x216B, 0x216C, 0x216D, 0x216E, 0x216F, 0x2170, 0x2171, 0x2172, 0x2173, 0x2174, 0x0000,
};

u16 D_801C6B28[] = {
    0x4408, 0x4201, 0x4202, 0x4204, 0x4208, 0x4210, 0x4220, 0x4240, 0x4280, 0x4301, 0x4302, 0x4304, 0x4308, 0x4310,
    0x4320, 0x4340, 0x4380, 0x4401, 0x4402, 0x4404, 0x4410, 0x4420, 0x4440, 0x4480, 0x4501, 0x4502, 0x4504, 0x4508,
    0x4510, 0x4520, 0x4540, 0x4580, 0x4601, 0x4602, 0x4604, 0x4608, 0x4610, 0x4620, 0x4640, 0x4680, 0x4701, 0x4702,
    0x4704, 0x4708, 0x4710, 0x4720, 0x4740, 0x4780, 0x4801, 0x4802, 0x4804, 0x4808, 0x4810, 0x4820, 0x4840, 0x0000,
};

MessageTableEntry D_801C6B98[] = {
    { 0, 0, 0x08000000 },
    { 2, 0, 0x0800000C },
// TODO: Extract (~9000 lines)
};

MessageTableEntry D_801CFB08[] = {
    { 0x4E20, 0xB0, 0x07000000 },
    { 0x4E21, 0xB0, 0x0700003C },
// TODO: Extract (~150 lines)
};

s16 D_801CFC78[] = {
    0, 1, 3, 2, 14, 14, 0, 14, 0, 0, 0, 14, 14, 4, 14,
};

u8 sOcarinaButtonIndexBuf[12] = { 0 };

s16 sOcarinaButtonAlphaValues[9] = { 0 };

s16 gOcarinaSongItemMap[] = {
    OCARINA_SONG_SONATA,        OCARINA_SONG_GORON_LULLABY, OCARINA_SONG_NEW_WAVE,
    OCARINA_SONG_ELEGY,         OCARINA_SONG_OATH,          OCARINA_SONG_SARIAS,
    OCARINA_SONG_TIME,          OCARINA_SONG_HEALING,       OCARINA_SONG_EPONAS,
    OCARINA_SONG_SOARING,       OCARINA_SONG_STORMS,        OCARINA_SONG_SUNS,
    OCARINA_SONG_INVERTED_TIME, OCARINA_SONG_DOUBLE_TIME,   OCARINA_SONG_GORON_LULLABY_INTRO,
};

s16 sIconPrimColors[][3] = {
    { 0, 80, 200 },
    { 50, 130, 255 },
};
s16 sIconEnvColors[][3] = {
    { 0, 0, 0 },
    { 0, 130, 255 },
};

s16 sIconPrimR = 0;
s16 sIconPrimG = 80;
s16 sIconPrimB = 200;
s16 sIconFlashTimer = 12;
s16 sIconFlashColorIdx = 0;
s16 sIconEnvR = 0;
s16 sIconEnvG = 0;
s16 sIconEnvB = 0;

s16 D_801CFD10[][3] = {
    { 0, 80, 200 },
    { 50, 130, 255 },
};
s16 D_801CFD1C[][3] = {
    { 0, 0, 0 },
    { 0, 130, 255 },
};

s16 D_801CFD28 = 0;
s16 D_801CFD2C = 80;
s16 D_801CFD30 = 200;
s16 D_801CFD34 = 12;
s16 D_801CFD38 = 0;
s16 D_801CFD3C = 0;
s16 D_801CFD40 = 0;
s16 D_801CFD44 = 0;

s16 D_801CFD48[][3] = {
    { 0, 80, 200 },
    { 50, 130, 255 },
};
s16 D_801CFD54[][3] = {
    { 0, 0, 0 },
    { 0, 130, 255 },
};

s16 D_801CFD60 = 0;
s16 D_801CFD64 = 80;
s16 D_801CFD68 = 200;
s16 D_801CFD6C = 12;
s16 D_801CFD70 = 0;
s16 D_801CFD74 = 0;
s16 D_801CFD78 = 0;
s16 D_801CFD7C = 0;

s16 sHeld = 0; // func_80148B98
s16 D_801CFD84 = 0; // func_80148D64
s16 D_801CFD88 = 0; // func_801491DC
s16 D_801CFD8C = 0; // func_80149454
s16 D_801CFD90 = 0; // func_801496C8

s16 sTextboxWidth = 0x100;
s16 sTextboxHeight = 0x40;
s16 sTextboxTexWidth = 0x400;
s16 sTextboxTexHeight = 0x400;

f32 D_801CFDA4[] = {
    0.6f, 0.75f, 0.9f, 1.0f, 1.05f, 1.1f, 1.05f, 1.0f, 1.0f,
};

f32 D_801CFDC8[] = {
    0.6f, 0.75f, 0.9f, 1.0f, 1.05f, 1.1f, 1.05f, 1.0f, 1.0f,
};

s32 D_801CFDEC
/* 12A32C 801CFDEC */ .word 0x00FF0000
/* 12A330 801CFDF0 */ .word 0x00FFFFFF
/* 12A334 801CFDF4 */ .word 0xFFFF0000
/* 12A338 801CFDF8 */ .word 0xFF00FFFF
/* 12A33C 801CFDFC */ .word 0xFFFFFF64
/* 12A340 801CFE00 */ .word 0x00000000

s32 D_801CFE04
/* 12A344 801CFE04 */ .word 0x009600FF
/* 12A348 801CFE08 */ .word 0x006400FF
/* 12A34C 801CFE0C */ .word 0x00FF00FF
/* 12A350 801CFE10 */ .word 0x00FF00FF
/* 12A354 801CFE14 */ .word 0x00FF00FF
/* 12A358 801CFE18 */ .word 0x00FF00FF

s32 D_801CFE1C
/* 12A35C 801CFE1C */ .word 0x00FF0050
/* 12A360 801CFE20 */ .word 0x009600A0
/* 12A364 801CFE24 */ .word 0x006400F0
/* 12A368 801CFE28 */ .word 0x00FF00FF
/* 12A36C 801CFE2C */ .word 0x00FF00FF
/* 12A370 801CFE30 */ .word 0x00FF00FF

s32 D_801CFE34
/* 12A374 801CFE34 */ .word 0x00640028
/* 12A378 801CFE38 */ .word 0x00FF0000
/* 12A37C 801CFE3C */ .word 0x00FF0064
/* 12A380 801CFE40 */ .word 0x00FF00FF
/* 12A384 801CFE44 */ .word 0x00FF00FF
/* 12A388 801CFE48 */ .word 0x00FF00FF

s32 D_801CFE4C
/* 12A38C 801CFE4C */ .word 0x02008998
/* 12A390 801CFE50 */ .word 0x02009598
/* 12A394 801CFE54 */ .word 0x0200A198
/* 12A398 801CFE58 */ .word 0x0200AD98

s32 D_801CFE5C
/* 12A39C 801CFE5C */ .word 0xFF6EA05A
/* 12A3A0 801CFE60 */ .word 0xFF6478FF
/* 12A3A4 801CFE64 */ .word 0xFFF5F55A

s32 D_801CFE68
/* 12A3A8 801CFE68 */ .word 0xFFFFFFFF
/* 12A3AC 801CFE6C */ .word 0xFFFFFFFF
/* 12A3B0 801CFE70 */ .word 0xFFE1AA00

s32 D_801CFE74
/* 12A3B4 801CFE74 */ .word 0x00FF0078
/* 12A3B8 801CFE78 */ .word 0x00000046
/* 12A3BC 801CFE7C */ .word 0x00FF0050
/* 12A3C0 801CFE80 */ .word 0x0050006E
/* 12A3C4 801CFE84 */ .word 0x00FF00FF
/* 12A3C8 801CFE88 */ .word 0x00FF001E
/* 12A3CC 801CFE8C */ .word 0x005A00B4
/* 12A3D0 801CFE90 */ .word 0x00FF00D2
/* 12A3D4 801CFE94 */ .word 0x006400FF
/* 12A3D8 801CFE98 */ .word 0x00AA00AA
/* 12A3DC 801CFE9C */ .word 0x00AA00FF
/* 12A3E0 801CFEA0 */ .word 0x0082001E

s32 D_801CFEA4
/* 12A3E4 801CFEA4 */ .word 0x00FF003C
/* 12A3E8 801CFEA8 */ .word 0x003C0046
/* 12A3EC 801CFEAC */ .word 0x00FF0050
/* 12A3F0 801CFEB0 */ .word 0x0050005A
/* 12A3F4 801CFEB4 */ .word 0x00FF00FF
/* 12A3F8 801CFEB8 */ .word 0x00FF0032
/* 12A3FC 801CFEBC */ .word 0x00500096
/* 12A400 801CFEC0 */ .word 0x00FF00FF
/* 12A404 801CFEC4 */ .word 0x009600B4
/* 12A408 801CFEC8 */ .word 0x00AA00AA
/* 12A40C 801CFECC */ .word 0x00AA00FF
/* 12A410 801CFED0 */ .word 0x0082001E

s32 D_801CFED4
/* 12A414 801CFED4 */ .word 0x00FF003C
/* 12A418 801CFED8 */ .word 0x003C0046
/* 12A41C 801CFEDC */ .word 0x00FF0050
/* 12A420 801CFEE0 */ .word 0x0050005A
/* 12A424 801CFEE4 */ .word 0x00FF00FF
/* 12A428 801CFEE8 */ .word 0x00FF0032
/* 12A42C 801CFEEC */ .word 0x00500096
/* 12A430 801CFEF0 */ .word 0x00FF00FF
/* 12A434 801CFEF4 */ .word 0x009600B4
/* 12A438 801CFEF8 */ .word 0x00B400B4
/* 12A43C 801CFEFC */ .word 0x00C800FF
/* 12A440 801CFF00 */ .word 0x0082001E

s32 D_801CFF04
/* 12A444 801CFF04 */ .word 0x00C30000
/* 12A448 801CFF08 */ .word 0x00000046
/* 12A44C 801CFF0C */ .word 0x00FF0050
/* 12A450 801CFF10 */ .word 0x0050005A
/* 12A454 801CFF14 */ .word 0x00FF00FF
/* 12A458 801CFF18 */ .word 0x00FF0032
/* 12A45C 801CFF1C */ .word 0x00500096
/* 12A460 801CFF20 */ .word 0x00FF00FF
/* 12A464 801CFF24 */ .word 0x009600B4
/* 12A468 801CFF28 */ .word 0x00AA00AA
/* 12A46C 801CFF2C */ .word 0x00AA00FF
/* 12A470 801CFF30 */ .word 0x0082001E

s32 D_801CFF34
/* 12A474 801CFF34 */ .word 0x00FF003C
/* 12A478 801CFF38 */ .word 0x003C006E
/* 12A47C 801CFF3C */ .word 0x00AA00FF
/* 12A480 801CFF40 */ .word 0x0050005A
/* 12A484 801CFF44 */ .word 0x00FF00FF
/* 12A488 801CFF48 */ .word 0x00FF0032
/* 12A48C 801CFF4C */ .word 0x00500096
/* 12A490 801CFF50 */ .word 0x00FF00FF
/* 12A494 801CFF54 */ .word 0x009600B4
/* 12A498 801CFF58 */ .word 0x00AA00AA
/* 12A49C 801CFF5C */ .word 0x00AA00FF
/* 12A4A0 801CFF60 */ .word 0x0082001E

s32 D_801CFF64
/* 12A4A4 801CFF64 */ .word 0x02010306
/* 12A4A8 801CFF68 */ .word 0x06060303
/* 12A4AC 801CFF6C */ .word 0x03030106

s16 D_801CFF70[] = {
0x0004, 0x0012,
0x0012, 0x0012,
0x0012, 0x0000,
};

s16 D_801CFF7C[] = {
0x0000, 0x000E,
0x000E, 0x000E,
0x000E, 0x0000,
};

s16 D_801CFF88[] = {
0x0000, 0x0016,
0x0016, 0x0016,
0x0016, 0x0000,
};

s16 D_801CFF94[] = {
0x270F, 0x0084,
0x0085, 0x0086,
0x0087, 0x0088,
0x0089, 0x008A,
0x005A, 0x005B,
0x0083, 0x0083,
0x0070, 0x006F,
0x0079, 0x007A,
0x0083, 0x0077,
0x0083, 0x0083,
0x0006, 0x0006,
0x0006, 0x0006,
0x0006, 0x0008,
0x0007, 0x0056,
0x0057, 0x0058,
0x0001, 0x0001,
0x0001, 0x0001,
0x0053, 0x0054,
0x0055, 0x0002,
0x0003, 0x0004,
0x0009, 0x0009,
0x0009, 0x270F,
0x270F, 0x270F,
0x270F, 0x009B,
0x270F, 0x270F,
0x0051, 0x0052,
0x000C, 0x000A,
};

s32 D_801D0000
/* 12A540 801D0000 */ .byte 0x00

s32 D_801D0001
/* 12A541 801D0001 */ .byte 0x0D
/* 12A542 801D0002 */ .byte 0x00
/* 12A543 801D0003 */ .byte 0x4D
/* 12A544 801D0004 */ .byte 0x00
/* 12A545 801D0005 */ .byte 0x4E
/* 12A546 801D0006 */ .byte 0x00
/* 12A547 801D0007 */ .byte 0x4F
/* 12A548 801D0008 */ .byte 0x00
/* 12A549 801D0009 */ .byte 0x50
/* 12A54A 801D000A */ .byte 0x00
/* 12A54B 801D000B */ .byte 0x10
/* 12A54C 801D000C */ .byte 0x00
/* 12A54D 801D000D */ .byte 0x78
/* 12A54E 801D000E */ .byte 0x00
/* 12A54F 801D000F */ .byte 0x74
/* 12A550 801D0010 */ .byte 0x00
/* 12A551 801D0011 */ .byte 0x76
/* 12A552 801D0012 */ .byte 0x00
/* 12A553 801D0013 */ .byte 0x75
/* 12A554 801D0014 */ .byte 0x00
/* 12A555 801D0015 */ .byte 0x0C
/* 12A556 801D0016 */ .byte 0x00
/* 12A557 801D0017 */ .byte 0x0F
/* 12A558 801D0018 */ .byte 0x00
/* 12A559 801D0019 */ .byte 0x0E
/* 12A55A 801D001A */ .byte 0x00
/* 12A55B 801D001B */ .byte 0x0D
/* 12A55C 801D001C */ .byte 0x00
/* 12A55D 801D001D */ .byte 0x5C
/* 12A55E 801D001E */ .byte 0x27

s32 D_801D001F
/* 12A55F 801D001F */ .byte 0x0F
/* 12A560 801D0020 */ .byte 0x27
/* 12A561 801D0021 */ .byte 0x0F
/* 12A562 801D0022 */ .byte 0x27
/* 12A563 801D0023 */ .byte 0x0F
/* 12A564 801D0024 */ .byte 0x27
/* 12A565 801D0025 */ .byte 0x0F
/* 12A566 801D0026 */ .byte 0x27
/* 12A567 801D0027 */ .byte 0x0F
/* 12A568 801D0028 */ .byte 0x27
/* 12A569 801D0029 */ .byte 0x0F
/* 12A56A 801D002A */ .byte 0x27
/* 12A56B 801D002B */ .byte 0x0F
/* 12A56C 801D002C */ .byte 0x00
/* 12A56D 801D002D */ .byte 0x00
/* 12A56E 801D002E */ .byte 0x27
/* 12A56F 801D002F */ .byte 0x0F
/* 12A570 801D0030 */ .byte 0x27
/* 12A571 801D0031 */ .byte 0x0F
/* 12A572 801D0032 */ .byte 0x27
/* 12A573 801D0033 */ .byte 0x0F
/* 12A574 801D0034 */ .byte 0x00
/* 12A575 801D0035 */ .byte 0x6D
/* 12A576 801D0036 */ .byte 0x27
/* 12A577 801D0037 */ .byte 0x0F
/* 12A578 801D0038 */ .byte 0x00
/* 12A579 801D0039 */ .byte 0x6E
/* 12A57A 801D003A */ .byte 0x27
/* 12A57B 801D003B */ .byte 0x0F
/* 12A57C 801D003C */ .byte 0x27
/* 12A57D 801D003D */ .byte 0x0F
/* 12A57E 801D003E */ .byte 0x00
/* 12A57F 801D003F */ .byte 0x5D
/* 12A580 801D0040 */ .byte 0x00
/* 12A581 801D0041 */ .byte 0x5E
/* 12A582 801D0042 */ .byte 0x00
/* 12A583 801D0043 */ .byte 0x5F
/* 12A584 801D0044 */ .byte 0x00
/* 12A585 801D0045 */ .byte 0x60
/* 12A586 801D0046 */ .byte 0x00
/* 12A587 801D0047 */ .byte 0x13
/* 12A588 801D0048 */ .byte 0x00
/* 12A589 801D0049 */ .byte 0x12
/* 12A58A 801D004A */ .byte 0x00
/* 12A58B 801D004B */ .byte 0x13
/* 12A58C 801D004C */ .byte 0x00
/* 12A58D 801D004D */ .byte 0x14
/* 12A58E 801D004E */ .byte 0x00
/* 12A58F 801D004F */ .byte 0x15
/* 12A590 801D0050 */ .byte 0x00
/* 12A591 801D0051 */ .byte 0x16
/* 12A592 801D0052 */ .byte 0x00
/* 12A593 801D0053 */ .byte 0x17
/* 12A594 801D0054 */ .byte 0x00
/* 12A595 801D0055 */ .byte 0x18
/* 12A596 801D0056 */ .byte 0x00
/* 12A597 801D0057 */ .byte 0x19
/* 12A598 801D0058 */ .byte 0x00
/* 12A599 801D0059 */ .byte 0x1A
/* 12A59A 801D005A */ .byte 0x00
/* 12A59B 801D005B */ .byte 0x1B
/* 12A59C 801D005C */ .byte 0x00
/* 12A59D 801D005D */ .byte 0x1C
/* 12A59E 801D005E */ .byte 0x00
/* 12A59F 801D005F */ .byte 0x1D
/* 12A5A0 801D0060 */ .byte 0x00
/* 12A5A1 801D0061 */ .byte 0x1E
/* 12A5A2 801D0062 */ .byte 0x00
/* 12A5A3 801D0063 */ .byte 0x1F
/* 12A5A4 801D0064 */ .byte 0x00
/* 12A5A5 801D0065 */ .byte 0x20
/* 12A5A6 801D0066 */ .byte 0x00
/* 12A5A7 801D0067 */ .byte 0x21
/* 12A5A8 801D0068 */ .byte 0x00
/* 12A5A9 801D0069 */ .byte 0x22
/* 12A5AA 801D006A */ .byte 0x00
/* 12A5AB 801D006B */ .byte 0x23
/* 12A5AC 801D006C */ .byte 0x27
/* 12A5AD 801D006D */ .byte 0x0F
/* 12A5AE 801D006E */ .byte 0x27
/* 12A5AF 801D006F */ .byte 0x0F
/* 12A5B0 801D0070 */ .byte 0x00
/* 12A5B1 801D0071 */ .byte 0x24
/* 12A5B2 801D0072 */ .byte 0x00
/* 12A5B3 801D0073 */ .byte 0x25
/* 12A5B4 801D0074 */ .byte 0x00
/* 12A5B5 801D0075 */ .byte 0x26
/* 12A5B6 801D0076 */ .byte 0x27
/* 12A5B7 801D0077 */ .byte 0x0F
/* 12A5B8 801D0078 */ .byte 0x27
/* 12A5B9 801D0079 */ .byte 0x0F
/* 12A5BA 801D007A */ .byte 0x27
/* 12A5BB 801D007B */ .byte 0x0F
/* 12A5BC 801D007C */ .byte 0x27
/* 12A5BD 801D007D */ .byte 0x0F
/* 12A5BE 801D007E */ .byte 0x27
/* 12A5BF 801D007F */ .byte 0x0F
/* 12A5C0 801D0080 */ .byte 0x27
/* 12A5C1 801D0081 */ .byte 0x0F
/* 12A5C2 801D0082 */ .byte 0x27
/* 12A5C3 801D0083 */ .byte 0x0F
/* 12A5C4 801D0084 */ .byte 0x00
/* 12A5C5 801D0085 */ .byte 0x32
/* 12A5C6 801D0086 */ .byte 0x00
/* 12A5C7 801D0087 */ .byte 0x33
/* 12A5C8 801D0088 */ .byte 0x00
/* 12A5C9 801D0089 */ .byte 0x34
/* 12A5CA 801D008A */ .byte 0x00
/* 12A5CB 801D008B */ .byte 0x35
/* 12A5CC 801D008C */ .byte 0x00
/* 12A5CD 801D008D */ .byte 0x36
/* 12A5CE 801D008E */ .byte 0x00
/* 12A5CF 801D008F */ .byte 0x37
/* 12A5D0 801D0090 */ .byte 0x00
/* 12A5D1 801D0091 */ .byte 0x38
/* 12A5D2 801D0092 */ .byte 0x00
/* 12A5D3 801D0093 */ .byte 0x39
/* 12A5D4 801D0094 */ .byte 0x00
/* 12A5D5 801D0095 */ .byte 0x3A
/* 12A5D6 801D0096 */ .byte 0x00
/* 12A5D7 801D0097 */ .byte 0x3B
/* 12A5D8 801D0098 */ .byte 0x00
/* 12A5D9 801D0099 */ .byte 0x3C
/* 12A5DA 801D009A */ .byte 0x00
/* 12A5DB 801D009B */ .byte 0x3D
/* 12A5DC 801D009C */ .byte 0x00
/* 12A5DD 801D009D */ .byte 0x3E
/* 12A5DE 801D009E */ .byte 0x00
/* 12A5DF 801D009F */ .byte 0x3F
/* 12A5E0 801D00A0 */ .byte 0x00
/* 12A5E1 801D00A1 */ .byte 0x40
/* 12A5E2 801D00A2 */ .byte 0x00
/* 12A5E3 801D00A3 */ .byte 0x41
/* 12A5E4 801D00A4 */ .byte 0x00
/* 12A5E5 801D00A5 */ .byte 0x42
/* 12A5E6 801D00A6 */ .byte 0x00
/* 12A5E7 801D00A7 */ .byte 0x43
/* 12A5E8 801D00A8 */ .byte 0x00
/* 12A5E9 801D00A9 */ .byte 0x44
/* 12A5EA 801D00AA */ .byte 0x00
/* 12A5EB 801D00AB */ .byte 0x45
/* 12A5EC 801D00AC */ .byte 0x00
/* 12A5ED 801D00AD */ .byte 0x46
/* 12A5EE 801D00AE */ .byte 0x00
/* 12A5EF 801D00AF */ .byte 0x47
/* 12A5F0 801D00B0 */ .byte 0x00
/* 12A5F1 801D00B1 */ .byte 0x48
/* 12A5F2 801D00B2 */ .byte 0x00
/* 12A5F3 801D00B3 */ .byte 0x49
/* 12A5F4 801D00B4 */ .byte 0x27
/* 12A5F5 801D00B5 */ .byte 0x0F
/* 12A5F6 801D00B6 */ .byte 0x00
/* 12A5F7 801D00B7 */ .byte 0x25
/* 12A5F8 801D00B8 */ .byte 0x00
/* 12A5F9 801D00B9 */ .byte 0x18
/* 12A5FA 801D00BA */ .byte 0x00
/* 12A5FB 801D00BB */ .byte 0x22
/* 12A5FC 801D00BC */ .byte 0x00
/* 12A5FD 801D00BD */ .byte 0x26
/* 12A5FE 801D00BE */ .byte 0x00
/* 12A5FF 801D00BF */ .byte 0x24
/* 12A600 801D00C0 */ .byte 0x00
/* 12A601 801D00C1 */ .byte 0x28
/* 12A602 801D00C2 */ .byte 0x00
/* 12A603 801D00C3 */ .byte 0x29
/* 12A604 801D00C4 */ .byte 0x00
/* 12A605 801D00C5 */ .byte 0x2A
/* 12A606 801D00C6 */ .byte 0x00
/* 12A607 801D00C7 */ .byte 0x2B
/* 12A608 801D00C8 */ .byte 0x00
/* 12A609 801D00C9 */ .byte 0x2C
/* 12A60A 801D00CA */ .byte 0x27
/* 12A60B 801D00CB */ .byte 0x0F
/* 12A60C 801D00CC */ .byte 0x27
/* 12A60D 801D00CD */ .byte 0x0F
/* 12A60E 801D00CE */ .byte 0x27
/* 12A60F 801D00CF */ .byte 0x0F
/* 12A610 801D00D0 */ .byte 0x27
/* 12A611 801D00D1 */ .byte 0x0F
/* 12A612 801D00D2 */ .byte 0x27
/* 12A613 801D00D3 */ .byte 0x0F
/* 12A614 801D00D4 */ .byte 0x00
/* 12A615 801D00D5 */ .byte 0x2D
/* 12A616 801D00D6 */ .byte 0x00
/* 12A617 801D00D7 */ .byte 0x2E
/* 12A618 801D00D8 */ .byte 0x27
/* 12A619 801D00D9 */ .byte 0x0F
/* 12A61A 801D00DA */ .byte 0x27
/* 12A61B 801D00DB */ .byte 0x0F
/* 12A61C 801D00DC */ .byte 0x27
/* 12A61D 801D00DD */ .byte 0x0F
/* 12A61E 801D00DE */ .byte 0x27
/* 12A61F 801D00DF */ .byte 0x0F
/* 12A620 801D00E0 */ .byte 0x27
/* 12A621 801D00E1 */ .byte 0x0F
/* 12A622 801D00E2 */ .byte 0x27
/* 12A623 801D00E3 */ .byte 0x0F
/* 12A624 801D00E4 */ .byte 0x27
/* 12A625 801D00E5 */ .byte 0x0F
/* 12A626 801D00E6 */ .byte 0x27
/* 12A627 801D00E7 */ .byte 0x0F
/* 12A628 801D00E8 */ .byte 0x00
/* 12A629 801D00E9 */ .byte 0x2F
/* 12A62A 801D00EA */ .byte 0x00
/* 12A62B 801D00EB */ .byte 0x30
/* 12A62C 801D00EC */ .byte 0x27
/* 12A62D 801D00ED */ .byte 0x0F
/* 12A62E 801D00EE */ .byte 0x27
/* 12A62F 801D00EF */ .byte 0x0F
/* 12A630 801D00F0 */ .byte 0x27
/* 12A631 801D00F1 */ .byte 0x0F
/* 12A632 801D00F2 */ .byte 0x27
/* 12A633 801D00F3 */ .byte 0x0F
/* 12A634 801D00F4 */ .byte 0x27
/* 12A635 801D00F5 */ .byte 0x0F
/* 12A636 801D00F6 */ .byte 0x27
/* 12A637 801D00F7 */ .byte 0x0F
/* 12A638 801D00F8 */ .byte 0x27
/* 12A639 801D00F9 */ .byte 0x0F
/* 12A63A 801D00FA */ .byte 0x00
/* 12A63B 801D00FB */ .byte 0x31
/* 12A63C 801D00FC */ .byte 0x00
/* 12A63D 801D00FD */ .byte 0x31
/* 12A63E 801D00FE */ .byte 0x00
/* 12A63F 801D00FF */ .byte 0x31
/* 12A640 801D0100 */ .byte 0x00
/* 12A641 801D0101 */ .byte 0x31
/* 12A642 801D0102 */ .byte 0x00
/* 12A643 801D0103 */ .byte 0x31
/* 12A644 801D0104 */ .byte 0x00
/* 12A645 801D0105 */ .byte 0x31
/* 12A646 801D0106 */ .byte 0x00
/* 12A647 801D0107 */ .byte 0x31
/* 12A648 801D0108 */ .byte 0x27
/* 12A649 801D0109 */ .byte 0x0F
/* 12A64A 801D010A */ .byte 0x27
/* 12A64B 801D010B */ .byte 0x0F
/* 12A64C 801D010C */ .byte 0x27
/* 12A64D 801D010D */ .byte 0x0F
/* 12A64E 801D010E */ .byte 0x27
/* 12A64F 801D010F */ .byte 0x0F
/* 12A650 801D0110 */ .byte 0x27
/* 12A651 801D0111 */ .byte 0x0F
/* 12A652 801D0112 */ .byte 0x27
/* 12A653 801D0113 */ .byte 0x0F
/* 12A654 801D0114 */ .byte 0x27
/* 12A655 801D0115 */ .byte 0x0F
/* 12A656 801D0116 */ .byte 0x27
/* 12A657 801D0117 */ .byte 0x0F
/* 12A658 801D0118 */ .byte 0x27
/* 12A659 801D0119 */ .byte 0x0F
/* 12A65A 801D011A */ .byte 0x27
/* 12A65B 801D011B */ .byte 0x0F
/* 12A65C 801D011C */ .byte 0x27
/* 12A65D 801D011D */ .byte 0x0F
/* 12A65E 801D011E */ .byte 0x27
/* 12A65F 801D011F */ .byte 0x0F
/* 12A660 801D0120 */ .byte 0x27
/* 12A661 801D0121 */ .byte 0x0F
/* 12A662 801D0122 */ .byte 0x27
/* 12A663 801D0123 */ .byte 0x0F
/* 12A664 801D0124 */ .byte 0x00
/* 12A665 801D0125 */ .byte 0x61
/* 12A666 801D0126 */ .byte 0x00
/* 12A667 801D0127 */ .byte 0x61
/* 12A668 801D0128 */ .byte 0x00
/* 12A669 801D0129 */ .byte 0x61
/* 12A66A 801D012A */ .byte 0x00
/* 12A66B 801D012B */ .byte 0x62
/* 12A66C 801D012C */ .byte 0x00
/* 12A66D 801D012D */ .byte 0x63
/* 12A66E 801D012E */ .byte 0x00
/* 12A66F 801D012F */ .byte 0x64
/* 12A670 801D0130 */ .byte 0x00
/* 12A671 801D0131 */ .byte 0x65
/* 12A672 801D0132 */ .byte 0x00
/* 12A673 801D0133 */ .byte 0x66
/* 12A674 801D0134 */ .byte 0x00
/* 12A675 801D0135 */ .byte 0x67
/* 12A676 801D0136 */ .byte 0x00
/* 12A677 801D0137 */ .byte 0x68
/* 12A678 801D0138 */ .byte 0x00
/* 12A679 801D0139 */ .byte 0x69
/* 12A67A 801D013A */ .byte 0x00
/* 12A67B 801D013B */ .byte 0x6A
/* 12A67C 801D013C */ .byte 0x00
/* 12A67D 801D013D */ .byte 0x6B
/* 12A67E 801D013E */ .byte 0x00
/* 12A67F 801D013F */ .byte 0x6C
/* 12A680 801D0140 */ .byte 0x00
/* 12A681 801D0141 */ .byte 0x62
/* 12A682 801D0142 */ .byte 0x00
/* 12A683 801D0143 */ .byte 0x61
/* 12A684 801D0144 */ .byte 0x00
/* 12A685 801D0145 */ .byte 0x61
/* 12A686 801D0146 */ .byte 0x00
/* 12A687 801D0147 */ .byte 0x61
/* 12A688 801D0148 */ .byte 0x00
/* 12A689 801D0149 */ .byte 0x62
/* 12A68A 801D014A */ .byte 0x00
/* 12A68B 801D014B */ .byte 0x63
/* 12A68C 801D014C */ .byte 0x00
/* 12A68D 801D014D */ .byte 0xB8
/* 12A68E 801D014E */ .byte 0x00
/* 12A68F 801D014F */ .byte 0xB9
/* 12A690 801D0150 */ .byte 0x00
/* 12A691 801D0151 */ .byte 0xBA
/* 12A692 801D0152 */ .byte 0x00
/* 12A693 801D0153 */ .byte 0xBB
/* 12A694 801D0154 */ .byte 0x00
/* 12A695 801D0155 */ .byte 0xBC
/* 12A696 801D0156 */ .byte 0x00
/* 12A697 801D0157 */ .byte 0xBD
/* 12A698 801D0158 */ .byte 0x00
/* 12A699 801D0159 */ .byte 0xBE
/* 12A69A 801D015A */ .byte 0x00
/* 12A69B 801D015B */ .byte 0xBF
/* 12A69C 801D015C */ .byte 0x00
/* 12A69D 801D015D */ .byte 0xC0
/* 12A69E 801D015E */ .byte 0x00
/* 12A69F 801D015F */ .byte 0xC1
/* 12A6A0 801D0160 */ .byte 0x00
/* 12A6A1 801D0161 */ .byte 0xC2
/* 12A6A2 801D0162 */ .byte 0x00
/* 12A6A3 801D0163 */ .byte 0xC3
/* 12A6A4 801D0164 */ .byte 0x00
/* 12A6A5 801D0165 */ .byte 0xC4
/* 12A6A6 801D0166 */ .byte 0x00
/* 12A6A7 801D0167 */ .byte 0xC5
/* 12A6A8 801D0168 */ .byte 0x00
/* 12A6A9 801D0169 */ .byte 0xC6
/* 12A6AA 801D016A */ .byte 0x00
/* 12A6AB 801D016B */ .byte 0xC7
/* 12A6AC 801D016C */ .byte 0x00
/* 12A6AD 801D016D */ .byte 0xC8
/* 12A6AE 801D016E */ .byte 0x00
/* 12A6AF 801D016F */ .byte 0xC9
/* 12A6B0 801D0170 */ .byte 0x00
/* 12A6B1 801D0171 */ .byte 0xCA
/* 12A6B2 801D0172 */ .byte 0x00
/* 12A6B3 801D0173 */ .byte 0xCB
/* 12A6B4 801D0174 */ .byte 0x00
/* 12A6B5 801D0175 */ .byte 0xCC
/* 12A6B6 801D0176 */ .byte 0x27
/* 12A6B7 801D0177 */ .byte 0x0F
/* 12A6B8 801D0178 */ .byte 0x27
/* 12A6B9 801D0179 */ .byte 0x0F
/* 12A6BA 801D017A */ .byte 0x27
/* 12A6BB 801D017B */ .byte 0x0F
/* 12A6BC 801D017C */ .byte 0x27
/* 12A6BD 801D017D */ .byte 0x0F
/* 12A6BE 801D017E */ .byte 0x27
/* 12A6BF 801D017F */ .byte 0x0F
/* 12A6C0 801D0180 */ .byte 0x27
/* 12A6C1 801D0181 */ .byte 0x0F
/* 12A6C2 801D0182 */ .byte 0x27
/* 12A6C3 801D0183 */ .byte 0x0F
/* 12A6C4 801D0184 */ .byte 0x27
/* 12A6C5 801D0185 */ .byte 0x0F
/* 12A6C6 801D0186 */ .byte 0x27
/* 12A6C7 801D0187 */ .byte 0x0F

u16 D_801D0188[OWL_WARP_MAX][9] = { 
    { 0x834F, 0x838C, 0x815B, 0x8367, 0x8378, 0x8343, 0x82CC, 0x8A43, 0x8ADD }, 
    { 0x835D, 0x815B, 0x8389, 0x82CC, 0x82DD, 0x82B3, 0x82AB, 0x0000, 0x0000 }, 
    { 0x8358, 0x836D, 0x815B, 0x8377, 0x8362, 0x8368, 0x0000, 0x0000, 0x0000 }, 
    { 0x8E52, 0x97A2, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 }, 
    { 0x834E, 0x838D, 0x8362, 0x834E, 0x835E, 0x8345, 0x8393, 0x0000, 0x0000 }, 
    { 0x837E, 0x838B, 0x834E, 0x838D, 0x815B, 0x8368, 0x0000, 0x0000, 0x0000 }, 
    { 0x8345, 0x8362, 0x8368, 0x8374, 0x8348, 0x815B, 0x838B, 0x0000, 0x0000 }, 
    { 0x8FC0, 0x926E, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 }, 
    { 0x8343, 0x834A, 0x815B, 0x8369, 0x8C6B, 0x924A, 0x0000, 0x0000, 0x0000 }, 
    { 0x838D, 0x8362, 0x834E, 0x8372, 0x838B, 0x0000, 0x0000, 0x0000, 0x0000 }, 
    { 0x93FC, 0x82E8, 0x8CFB, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
};

s16 D_801D0250[OWL_WARP_MAX] = { 
    0x0009, 0x0007, 0x0006, 0x0002, 0x0007, 0x0006, 0x0007, 0x0002, 0x0006, 0x0005, 0x0003,
};

s32 D_801D0268
/* 12A7A8 801D0268 */ .word 0x82CD82E2
/* 12A7AC 801D026C */ .word 0x82A282D3
/* 12A7B0 801D0270 */ .word 0x82C282A4
/* 12A7B4 801D0274 */ .word 0x82A882BB
/* 12A7B8 801D0278 */ .word 0x82A20000

s32 D_801D027C
/* 12A7BC 801D027C */ .word 0x20012003
/* 12A7C0 801D0280 */ .word 0x20042002

s32 D_801D0284
/* 12A7C4 801D0284 */ .word 0x90D490C2
/* 12A7C8 801D0288 */ .word 0x89A997CE

u16 D_801D028C[] = {
0x1B5A, 0x1B5B,
0x1B5C, 0x1B5D,
0x1B5E, 0x1B5F,
0x1B60, 0x1B61,
0x1B62, 0x1B63,
0x1B64, 0x1B65,
0x1B66, 0x1B67,
0x1B68, 0x1B69,
0x1B6A, 0x0000,
0x0DC7, 0x0DC9,
0x120A, 0x120B,
0x122C, 0x122F,
0x120A, 0x120B,
};

u16 sOcarinaSongFlagsMap[] = {
0x0001, 0x0002,
0x0004, 0x0008,
0x0010, 0x0020,
0x0040, 0x0080,
0x0100, 0x0200,
0x0400, 0x0800,
};

s16 sOcarinaEffectActorIds[15] = {
    ACTOR_OCEFF_WIPE5, ACTOR_OCEFF_WIPE5, // Sonata of Awakening Effect, Sonata of Awakening Effect
    ACTOR_OCEFF_WIPE5, ACTOR_OCEFF_WIPE5, // Sonata of Awakening Effect, Sonata of Awakening Effect
    ACTOR_OCEFF_WIPE5, ACTOR_OCEFF_WIPE5, // Sonata of Awakening Effect, Sonata of Awakening Effect
    ACTOR_OCEFF_WIPE,  ACTOR_OCEFF_WIPE7, // Song of Time Effect, Song of Healing Effect
    ACTOR_OCEFF_WIPE2, ACTOR_OCEFF_WIPE6, // Epona's Song Effect, Song of Soaring Effect
    ACTOR_OCEFF_STORM, ACTOR_OCEFF_SPOT,  // Song of Storms Effect II [?], Sun's Song Effect
    ACTOR_OCEFF_WIPE, ACTOR_OCEFF_WIPE,   // Song of Time Effect, Song of Time Effect
    ACTOR_OCEFF_WIPE4                     // Scarecrow's Song Effect 
};
s32 sOcarinaEffectActorParams[15] = { 0,1,2,3,4,0,1,0,0,0,0,0,1,1,0, };

s16 sOcarinaButtonAPrimColors[][3] = {
    { 80, 150, 255 },
    { 100, 200, 255 },
};

s16 sOcarinaButtonAEnvColors[][3] = {
    { 10, 10, 10 },
    { 50, 50, 255 },
};
s16 sOcarinaButtonCPrimColors[][3] = {
    { 255, 255, 50 },
    { 255, 255, 180 },
};
s16 sOcarinaButtonCEnvColors[][3] = {
    { 10, 10, 10 },
    { 110, 110, 50 },
};

TexturePtr sOcarinaButtonTextures[] = {
    gOcarinaATex, gOcarinaCDownTex, gOcarinaCRightTex, gOcarinaCLeftTex, gOcarinaCUpTex,
};

s32 D_801D0378
/* 12A8B8 801D0378 */ .word 0x005B005C
/* 12A8BC 801D037C */ .word 0x005D005E
/* 12A8C0 801D0380 */ .word 0x005F0060
/* 12A8C4 801D0384 */ .word 0x00340048
/* 12A8C8 801D0388 */ .word 0x00320047
/* 12A8CC 801D038C */ .word 0x00350033
/* 12A8D0 801D0390 */ .word 0x0049004A
/* 12A8D4 801D0394 */ .word 0x00610061
/* 12A8D8 801D0398 */ .word 0x00610000

s32 D_801D039C
/* 12A8DC 801D039C */ .word 0x355C5D5E
/* 12A8E0 801D03A0 */ .word 0x35000000

s32 D_801D03A4
/* 12A8E4 801D03A4 */ .word 0x01070809

s32 D_801D03A8
/* 12A8E8 801D03A8 */ .word 0x003B003B
/* 12A8EC 801D03AC */ .word 0x003B003B
/* 12A8F0 801D03B0 */ .word 0x0022003B
/* 12A8F4 801D03B4 */ .word 0x003B003B
/* 12A8F8 801D03B8 */ .word 0x003B003B
/* 12A8FC 801D03BC */ .word 0x003B003B
/* 12A900 801D03C0 */ .word 0x003B003B
/* 12A904 801D03C4 */ .word 0x003B003B

s32 D_801D03C8
/* 12A908 801D03C8 */ .word 0x00220022
/* 12A90C 801D03CC */ .word 0x00220022
/* 12A910 801D03D0 */ .word 0x00220022
/* 12A914 801D03D4 */ .word 0x00220022
/* 12A918 801D03D8 */ .word 0x00220022
/* 12A91C 801D03DC */ .word 0x00220022
/* 12A920 801D03E0 */ .word 0x00220022
/* 12A924 801D03E4 */ .word 0x00220022

s32 D_801D03E8
/* 12A928 801D03E8 */ .word 0x008E008E
/* 12A92C 801D03EC */ .word 0x008E008E

s32 D_801D03F0
/* 12A930 801D03F0 */ .byte 0x00
/* 12A931 801D03F1 */ .byte 0xAE
/* 12A932 801D03F2 */ .byte 0x00
/* 12A933 801D03F3 */ .byte 0x8E
/* 12A934 801D03F4 */ .byte 0x00
/* 12A935 801D03F5 */ .byte 0x8E
/* 12A936 801D03F6 */ .byte 0x00
/* 12A937 801D03F7 */ .byte 0x8E
/* 12A938 801D03F8 */ .byte 0x00
/* 12A939 801D03F9 */ .byte 0x8E
/* 12A93A 801D03FA */ .byte 0x00
/* 12A93B 801D03FB */ .byte 0x82
/* 12A93C 801D03FC */ .byte 0x00
/* 12A93D 801D03FD */ .byte 0xAE
/* 12A93E 801D03FE */ .byte 0x00
/* 12A93F 801D03FF */ .byte 0x00
/* 12A940 801D0400 */ .byte 0x00

s32 D_801D0401
/* 12A941 801D0401 */ .byte 0x8E
/* 12A942 801D0402 */ .byte 0x00
/* 12A943 801D0403 */ .byte 0x8E
/* 12A944 801D0404 */ .byte 0x00
/* 12A945 801D0405 */ .byte 0x8E
/* 12A946 801D0406 */ .byte 0x00
/* 12A947 801D0407 */ .byte 0x8E

s32 D_801D0408
/* 12A948 801D0408 */ .word 0x00260026
/* 12A94C 801D040C */ .word 0x00260026
/* 12A950 801D0410 */ .word 0x00AE0026
/* 12A954 801D0414 */ .word 0x00260026
/* 12A958 801D0418 */ .word 0x0026003C
/* 12A95C 801D041C */ .word 0x00AE0000
/* 12A960 801D0420 */ .word 0x00260026
/* 12A964 801D0424 */ .word 0x00260026

s32 D_801D0428
/* 12A968 801D0428 */ .half 0x005A
/* 12A96A 801D042A */ .half 0x005A
/* 12A96C 801D042C */ .half 0x005A
/* 12A96E 801D042E */ .half 0x005A
/* 12A970 801D0430 */ .half 0x00AE
/* 12A972 801D0432 */ .half 0x005A
/* 12A974 801D0434 */ .half 0x005A
/* 12A976 801D0436 */ .half 0x005A
/* 12A978 801D0438 */ .half 0x005A
/* 12A97A 801D043A */ .half 0x005A
/* 12A97C 801D043C */ .half 0x00AE
/* 12A97E 801D043E */ .half 0x0000
/* 12A980 801D0440 */ .half 0x005A
/* 12A982 801D0442 */ .half 0x005A
/* 12A984 801D0444 */ .half 0x005A
/* 12A986 801D0446 */ .half 0x005A
/* 12A988 801D0448 */ .half 0x001C
/* 12A98A 801D044A */ .half 0x001D
/* 12A98C 801D044C */ .half 0x001E
/* 12A98E 801D044E */ .half 0x001F
/* 12A990 801D0450 */ .half 0x0020
/* 12A992 801D0452 */ .half 0x0021
/* 12A994 801D0454 */ .half 0x0022
/* 12A996 801D0456 */ .half 0x0023
/* 12A998 801D0458 */ .half 0x0024

s32 D_801D045A
/* 12A99A 801D045A */ .half 0x0025
/* 12A99C 801D045C */ .half 0x1B91
/* 12A99E 801D045E */ .half 0x1B90
/* 12A9A0 801D0460 */ .half 0x1B8F

s32 D_801D0462
/* 12A9A2 801D0462 */ .half 0x0000
/* 12A9A4 801D0464 */ .half 0x1B92
/* 12A9A6 801D0466 */ .half 0x1B8E

s32 D_801D0468
/* 12A9A8 801D0468 */ .word 0x00000000
/* 12A9AC 801D046C */ .word 0x00000000
