#include "global.h"

// SoundFont comments from deathbasketslair
// TODO: Verify
AudioTable gSoundFontTable = {
    41, 0, 0, 0,
    { 0x00000, 0x81C0, 2, 0, 0x0FF, 0x7A10, 0x1C5 }, // Ocarina songs
    { 0x081C0, 0x36D0, 2, 0, 0x0FF, 0x7E01, 0x058 }, // Actor Sounds
    { 0x0B890, 0xCE0,  2, 0, 0x0FF, 0x1500, 0x000 }, // Nature Sounds
    { 0x0C570, 0x15D0, 2, 2, 0x1FF, 0x1040, 0x000 }, // Termina Field
    { 0x0DB40, 0xB60,  2, 2, 0x1FF, 0x0D40, 0x000 }, // Lost Woods
    { 0x0E6A0, 0xBE0,  2, 2, 0x1FF, 0x0A40, 0x000 }, // Mini Game
    { 0x0F280, 0xFC0,  2, 2, 0x1FF, 0x0D40, 0x000 }, // Fairy Fountain
    { 0x10240, 0x6F0,  2, 2, 0x1FF, 0x1000, 0x000 }, // Romani Ranch
    { 0x10930, 0x560,  2, 2, 0x1FF, 0x0F00, 0x000 }, // Horse Race
    { 0x10E90, 0xCC0,  2, 1, 0x1FF, 0x0D40, 0x000 }, // Bremen Mask March
    { 0x11B50, 0xAA0,  2, 2, 0x1FF, 0x0A40, 0x000 }, // Game House
    { 0x125F0, 0xA60,  2, 2, 0x1FF, 0x0A40, 0x000 }, // Zora's Domain
    { 0x13050, 0xBF0,  2, 2, 0x1FF, 0x0B40, 0x000 }, // Shop
    { 0x13C40, 0x4C0,  2, 2, 0x1FF, 0x0500, 0x000 }, // Ghost Shop
    { 0x14100, 0xC00,  2, 2, 0x1FF, 0x0C40, 0x000 }, // Kotake And Koume
    { 0x14D00, 0xDE0,  2, 1, 0x1FF, 0x0F40, 0x000 }, // Game Over/Fanfares
    { 0x15AE0, 0x660,  2, 1, 0x1FF, 0x0F00, 0x000 }, // Owl
    { 0x16140, 0x14D0, 2, 2, 0x1FF, 0x1040, 0x000 }, // Majora's Mask
    { 0x17610, 0xC50,  2, 1, 0x1FF, 0x0D00, 0x000 }, // The Four Giants
    { 0x18260, 0x1150, 2, 1, 0x1FF, 0x0F40, 0x000 }, // Zora Music
    { 0x193B0, 0x520,  2, 2, 0x1FF, 0x0600, 0x000 }, // Woodfall Dungeon
    { 0x198D0, 0x770,  2, 2, 0x1FF, 0x0D00, 0x000 }, // Snowhead
    { 0x1A040, 0x500,  2, 2, 0x1FF, 0x0800, 0x000 }, // UFO Attack
    { 0x1A540, 0x940,  2, 2, 0x1FF, 0x0E00, 0x000 }, // Clock Tower
    { 0x1AE80, 0x840,  2, 2, 0x1FF, 0x0B40, 0x000 }, // Sword Game
    { 0x1B6C0, 0x1440, 2, 2, 0x1FF, 0x1040, 0x000 }, // Clock Town
    { 0x1CB00, 0x300,  2, 2, 0x1FF, 0x0400, 0x000 }, // Ikana Canyon
    { 0x1CE00, 0xAE0,  2, 2, 0x1FF, 0x0A40, 0x000 }, // Song of Healing angry mask seller
    { 0x1D8E0, 0x6F0,  2, 2, 0x1FF, 0x0C00, 0x000 }, // Southern Swamp
    { 0x1DFD0, 0x5B0,  2, 2, 0x1FF, 0x0B00, 0x000 }, // Great Bay
    { 0x1E580, 0x810,  2, 2, 0x1FF, 0x0900, 0x000 }, // Ikana Canyon
    { 0x1ED90, 0x520,  2, 2, 0x1FF, 0x0800, 0x000 }, // Kamaro's Dance  
    { 0x1F2B0, 0xFF0,  2, 2, 0x1FF, 0x1040, 0x000 }, // Milk Bar
    { 0x202A0, 0x15E0, 2, 2, 0x1FF, 0x1040, 0x000 }, // Ikana Castle
    { 0x21880, 0xD0,   2, 1, 0x1FF, 0x0100, 0x000 }, // Kamaro's Song (flute)
    { 0x21950, 0x14B0, 2, 2, 0x1FF, 0x1040, 0x000 }, // Introduction
    { 0x22E00, 0x1410, 2, 2, 0x1FF, 0x0F40, 0x000 }, // Credits
    { 0x24210, 0x1540, 2, 2, 0x1FF, 0x0F40, 0x000 }, // Credits 2
    { 0x25750, 0x390,  2, 2, 0x2FF, 0x0500, 0x000 }, // Goron City
    { 0x25AE0, 0x520,  2, 1, 0x1FF, 0x0800, 0x000 }, // Garo's Song
    { 0x26000, 0x3F0,  2, 2, 0x0FF, 0x0500, 0x000 }, // 
};

u8 gSequenceFontTable = {
    // Indices for second half of table, interpreted as u16. 128 indices for 128 sequences
    0x100, 0x103, 0x105, 0x107, 0x109, 0x10B, 0x10D, 0x10F, 0x111, 0x113, 0x115, 0x117, 0x119, 0x11B, 0x11D, 0x11F,
    0x121, 0x123, 0x125, 0x127, 0x129, 0x12B, 0x12D, 0x12F, 0x131, 0x133, 0x135, 0x137, 0x139, 0x13B, 0x13D, 0x13F,
    0x141, 0x143, 0x145, 0x147, 0x149, 0x14B, 0x14D, 0x14F, 0x151, 0x153, 0x155, 0x157, 0x159, 0x15B, 0x15D, 0x15F,
    0x161, 0x163, 0x165, 0x167, 0x169, 0x16B, 0x16D, 0x16F, 0x171, 0x173, 0x175, 0x177, 0x179, 0x17B, 0x17D, 0x17F,
    0x181, 0x183, 0x185, 0x187, 0x189, 0x18B, 0x18D, 0x18F, 0x191, 0x193, 0x195, 0x197, 0x199, 0x19B, 0x19D, 0x19F,
    0x1A1, 0x1A3, 0x1A5, 0x1A7, 0x1A9, 0x1AB, 0x1AD, 0x1AF, 0x1B1, 0x1B3, 0x1B5, 0x1B7, 0x1B9, 0x1BB, 0x1BD, 0x1BF,
    0x1C1, 0x1C3, 0x1C5, 0x1C7, 0x1C9, 0x1CB, 0x1CD, 0x1CF, 0x1D1, 0x1D3, 0x1D5, 0x1D7, 0x1D9, 0x1DB, 0x1DD, 0x1DF,
    0x1E1, 0x1E3, 0x1E5, 0x1E7, 0x1E9, 0x1EB, 0x1ED, 0x1EF, 0x1F1, 0x1F3, 0x1F5, 0x1F7, 0x1F9, 0x1FB, 0x1FD, 0x1FF,
    
    // numFontsPerSequence, FontId(s)
    /* 0x100 */ { 2, 0x01, 0x00 }, // NA_BGM_GENERAL_SFX
    /* 0x103 */ { 1, 0x02 }, // NA_BGM_NATURE_AMBIENCE
    /* 0x105 */ { 1, 0x03 }, // NA_BGM_TERMINA_FIELD
    /* 0x107 */ { 1, 0x03 }, // NA_BGM_CHASE
    /* 0x109 */ { 1, 0x11 }, // NA_BGM_MAJORAS_THEME
    /* 0x10B */ { 1, 0x17 }, // NA_BGM_CLOCK_TOWER
    /* 0x10D */ { 1, 0x19 }, // NA_BGM_STONE_TOWER_TEMPLE
    /* 0x10F */ { 1, 0x19 }, // NA_BGM_INV_STONE_TOWER_TEMPLE
    { 1, 0x03 }, { 1, 0x03 }, { 1, 0x1B }, { 1, 0x17 }, { 1, 0x1C }, { 1, 0x16 }, { 1, 0x05 }, { 1, 0x03 }, 
    { 1, 0x1D }, { 1, 0x1E }, { 1, 0x19 }, { 1, 0x15 }, { 1, 0x03 }, { 1, 0x19 }, { 1, 0x19 }, { 1, 0x19 }, 
    { 1, 0x06 }, { 1, 0x03 }, { 1, 0x03 }, { 1, 0x03 }, { 1, 0x14 }, { 1, 0x03 }, { 1, 0x11 }, { 1, 0x03 }, 
    { 1, 0x0F }, { 1, 0x03 }, { 1, 0x0F }, { 1, 0x19 }, { 1, 0x0F }, { 1, 0x03 }, { 1, 0x26 }, { 1, 0x05 }, 
    { 1, 0x06 }, { 1, 0x06 }, { 1, 0x1F }, { 1, 0x03 }, { 1, 0x0D }, { 1, 0x12 }, { 1, 0x05 }, { 1, 0x07 }, 
    { 1, 0x26 }, { 1, 0x03 }, { 1, 0x00 }, { 1, 0x00 }, { 1, 0x00 }, { 1, 0x00 }, { 1, 0x0B }, { 1, 0x0F }, 
    { 1, 0x03 }, { 1, 0x0F }, { 1, 0x17 }, { 1, 0x1A }, { 1, 0x20 }, { 1, 0x03 }, { 1, 0x04 }, { 1, 0x26 }, 
    { 1, 0x08 }, { 1, 0x08 }, { 1, 0x08 }, { 1, 0x0E }, { 1, 0x0C }, { 1, 0x10 }, { 1, 0x0A }, { 1, 0x00 }, 
    { 1, 0x00 }, { 1, 0x00 }, { 1, 0x00 }, { 1, 0x12 }, { 1, 0x12 }, { 1, 0x13 }, { 1, 0x12 }, { 1, 0x12 }, 
    { 1, 0x18 }, { 1, 0x00 }, { 1, 0x06 }, { 1, 0x09 }, { 1, 0x12 }, { 1, 0x09 }, { 1, 0x20 }, { 1, 0x17 }, 
    { 1, 0x13 }, { 1, 0x13 }, { 1, 0x00 }, { 1, 0x00 }, { 1, 0x00 }, { 1, 0x00 }, { 1, 0x00 }, { 1, 0x00 }, 
    { 1, 0x17 }, { 1, 0x00 }, { 1, 0x13 }, { 1, 0x13 }, { 1, 0x13 }, { 1, 0x15 }, { 1, 0x16 }, { 1, 0x13 }, 
    { 1, 0x13 }, { 1, 0x11 }, { 1, 0x11 }, { 1, 0x11 }, { 1, 0x13 }, { 1, 0x13 }, { 1, 0x13 }, { 1, 0x21 }, 
    { 1, 0x21 }, { 1, 0x22 }, { 1, 0x08 }, { 1, 0x27 }, { 1, 0x24 }, { 1, 0x03 }, { 1, 0x23 }, { 1, 0x0F }, 
    { 1, 0x0F }, { 1, 0x0F }, { 1, 0x00 }, { 1, 0x11 }, { 1, 0x21 }, { 1, 0x21 }, { 1, 0x21 }, { 1, 0x25 },

    // pad?
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

AudioTable gSequenceTable = {
    128, 0, 0, 0,
    { 0x00000, 0xC6A0, 2, 0, 0, 0, 0 }, // NA_BGM_GENERAL_SFX
    { 0x0C6A0, 0xFC0,  2, 0, 0, 0, 0 }, // NA_BGM_NATURE_AMBIENCE
    { 0x0D660, 0x1B00, 2, 2, 0, 0, 0 }, // NA_BGM_TERMINA_FIELD
    { 0x0F160, 0x6A0,  2, 2, 0, 0, 0 }, // NA_BGM_CHASE
    { 0x0F800, 0xA80,  2, 2, 0, 0, 0 }, // NA_BGM_MAJORAS_THEME
    { 0x10280, 0xB10,  2, 2, 0, 0, 0 }, // NA_BGM_CLOCK_TOWER
    { 0x10D90, 0xC30,  2, 2, 0, 0, 0 }, // NA_BGM_STONE_TOWER_TEMPLE
    { 0x119C0, 0x12C0, 2, 2, 0, 0, 0 }, // NA_BGM_INV_STONE_TOWER_TEMPLE
    { 0x12C80, 0x170,  2, 2, 0, 0, 0 }, // NA_BGM_FAILURE_0
    { 0x12DF0, 0x1A0,  2, 2, 0, 0, 0 }, // NA_BGM_FAILURE_1
    { 0x12F90, 0x890,  2, 2, 0, 0, 0 }, // NA_BGM_HAPPY_MASK_SALESMAN
    { 0x13820, 0x910,  2, 2, 0, 0, 0 }, // NA_BGM_SONG_OF_HEALING
    { 0x14130, 0xE00,  2, 2, 0, 0, 0 }, // NA_BGM_SWAMP_REGION
    { 0x14F30, 0x670,  2, 2, 0, 0, 0 }, // NA_BGM_ALIEN_INVASION
    { 0x155A0, 0xC00,  2, 2, 0, 0, 0 }, // NA_BGM_SWAMP_CRUISE
    { 0x161A0, 0x1C0,  2, 2, 0, 0, 0 }, // NA_BGM_SHARPS_CURSE
    { 0x16360, 0xCF0,  2, 2, 0, 0, 0 }, // NA_BGM_GREAT_BAY_REGION
    { 0x17050, 0xD40,  2, 2, 0, 0, 0 }, // NA_BGM_IKANA_REGION
    { 0x17D90, 0xB90,  2, 2, 0, 0, 0 }, // NA_BGM_DEKU_KING
    { 0x18920, 0xC40,  2, 2, 0, 0, 0 }, // NA_BGM_MOUNTAIN_REGION
    { 0x19560, 0xD40,  2, 2, 0, 0, 0 }, // NA_BGM_PIRATES_FORTRESS
    { 0x1A2A0, 0xE90,  2, 2, 0, 0, 0 }, // NA_BGM_CLOCK_TOWN_DAY_1
    { 0x1B130, 0x8C0,  2, 2, 0, 0, 0 }, // NA_BGM_CLOCK_TOWN_DAY_2
    { 0x1B9F0, 0x8A0,  2, 2, 0, 0, 0 }, // NA_BGM_CLOCK_TOWN_DAY_3
    { 0x1C290, 0x4D0,  2, 2, 0, 0, 0 }, // NA_BGM_FILE_SELECT
    { 0x1C760, 0x390,  2, 1, 0, 0, 0 }, // NA_BGM_CLEAR_EVENT
    { 0x1CAF0, 0x1670, 2, 2, 0, 0, 0 }, // NA_BGM_ENEMY
    { 0x1E160, 0x1E30, 2, 2, 0, 0, 0 }, // NA_BGM_BOSS
    { 0x1FF90, 0xBB0,  2, 2, 0, 0, 0 }, // NA_BGM_WOODFALL_TEMPLE
    { 0x20B40, 0x610,  2, 2, 0, 0, 0 }, // NA_BGM_MARKET
    { 0x21150, 0xC60,  2, 2, 0, 0, 0 }, // NA_BGM_OPENING
    { 0x21DB0, 0x570,  2, 2, 0, 0, 0 }, // NA_BGM_INSIDE_A_HOUSE
    { 0x22320, 0x1B0,  2, 1, 0, 0, 0 }, // NA_BGM_GAME_OVER
    { 0x224D0, 0x630,  2, 2, 0, 0, 0 }, // NA_BGM_CLEAR_BOSS
    { 0x22B00, 0x150,  2, 1, 0, 0, 0 }, // NA_BGM_GET_ITEM
    { NA_BGM_CLOCK_TOWN_DAY_2, 0x0, 2, 2, 0, 0, 0 }, // NA_BGM_GATE_OPEN
    { 0x22C50, 0x230,  2, 1, 0, 0, 0 }, // NA_BGM_GET_HEART
    { 0x22E80, 0x8A0,  2, 2, 0, 0, 0 }, // NA_BGM_MINI_GAME_2
    { 0x23720, 0xAB0,  2, 2, 0, 0, 0 }, // NA_BGM_GORON_RACE
    { 0x241D0, 0x9D0,  2, 2, 0, 0, 0 }, // NA_BGM_MUSIC_BOX_HOUSE
    { NA_BGM_FILE_SELECT, 0x0, 2, 2, 0, 0, 0 }, // NA_BGM_FAIRY_FOUNTAIN
    { 0x24BA0, 0xA00,  2, 2, 0, 0, 0 }, // NA_BGM_ZELDAS_LULLABY
    { 0x255A0, 0x500,  2, 2, 0, 0, 0 }, // NA_BGM_ROSA_SISTERS
    { 0x25AA0, 0x450,  2, 1, 0, 0, 0 }, // NA_BGM_OPEN_CHEST
    { 0x25EF0, 0x360,  2, 2, 0, 0, 0 }, // NA_BGM_MARINE_RESEARCH_LAB
    { 0x26250, 0x660,  2, 2, 0, 0, 0 }, // NA_BGM_GIANTS_THEME
    { 0x268B0, 0x1020, 2, 2, 0, 0, 0 }, // NA_BGM_GURU_GURU
    { 0x278D0, 0x2590, 2, 2, 0, 0, 0 }, // NA_BGM_ROMANI_RANCH
    { 0x29E60, 0xA20,  2, 2, 0, 0, 0 }, // NA_BGM_GORON_VILLAGE
    { 0x2A880, 0x750,  2, 2, 0, 0, 0 }, // NA_BGM_MAYORS_OFFICE
    { 0x2AFD0, 0x180,  2, 1, 0, 0, 0 }, // NA_BGM_OCA_EPONA
    { 0x2B150, 0xB0,   2, 1, 0, 0, 0 }, // NA_BGM_OCA_SUNS
    { 0x2B200, 0x170,  2, 1, 0, 0, 0 }, // NA_BGM_OCA_TIME
    { 0x2B370, 0x110,  2, 1, 0, 0, 0 }, // NA_BGM_OCA_STORM
    { 0x2B480, 0x11A0, 2, 2, 0, 0, 0 }, // NA_BGM_ZORA_HALL
    { 0x2C620, 0x290,  2, 2, 0, 0, 0 }, // NA_BGM_GET_NEW_MASK
    { 0x2C8B0, 0x1B30, 2, 2, 0, 0, 0 }, // NA_BGM_MINI_BOSS
    { 0x2E3E0, 0x1C0,  2, 1, 0, 0, 0 }, // NA_BGM_GET_SMALL_ITEM
    { 0x2E5A0, 0x7A0,  2, 2, 0, 0, 0 }, // NA_BGM_ASTRAL_OBSERVATORY
    { 0x2ED40, 0x140,  2, 2, 0, 0, 0 }, // NA_BGM_CAVERN
    { 0x2EE80, 0x1290, 2, 2, 0, 0, 0 }, // NA_BGM_MILK_BAR
    { 0x30110, 0x210,  2, 1, 0, 0, 0 }, // NA_BGM_ZELDA_APPEAR
    { 0x30320, 0x950,  2, 2, 0, 0, 0 }, // NA_BGM_SARIAS_SONG
    { 0x30C70, 0x250,  2, 2, 0, 0, 0 }, // NA_BGM_GORON_GOAL
    { 0x30EC0, 0x1A80, 2, 2, 0, 0, 0 }, // NA_BGM_HORSE
    { 0x32940, 0x660,  2, 2, 0, 0, 0 }, // NA_BGM_HORSE_GOAL
    { 0x32FA0, 0x1AD0, 2, 2, 0, 0, 0 }, // NA_BGM_INGO
    { 0x34A70, 0xAC0,  2, 2, 0, 0, 0 }, // NA_BGM_KOTAKE_POTION_SHOP
    { 0x35530, 0xEC0,  2, 2, 0, 0, 0 }, // NA_BGM_SHOP
    { 0x363F0, 0x720,  2, 1, 0, 0, 0 }, // NA_BGM_OWL
    { 0x36B10, 0xE10,  2, 2, 0, 0, 0 }, // NA_BGM_MINI_GAME
    { 0x37920, 0x130,  2, 1, 0, 0, 0 }, // NA_BGM_OCA_SOARING
    { 0x37A50, 0xE0,   2, 1, 0, 0, 0 }, // NA_BGM_OCA_HEALING
    { 0x37B30, 0x150,  2, 1, 0, 0, 0 }, // NA_BGM_INVERTED_SONG_OF_TIME
    { 0x37C80, 0x170,  2, 1, 0, 0, 0 }, // NA_BGM_SONG_OF_DOUBLE_TIME
    { 0x37DF0, 0x780,  2, 1, 0, 0, 0 }, // NA_BGM_SONATA_OF_AWAKENING
    { 0x38570, 0x960,  2, 1, 0, 0, 0 }, // NA_BGM_GORON_LULLABY
    { 0x38ED0, 0x900,  2, 1, 0, 0, 0 }, // NA_BGM_NEW_WAVE_BOSSA_NOVA
    { 0x397D0, 0x620,  2, 1, 0, 0, 0 }, // NA_BGM_ELEGY_OF_EMPTINESS
    { 0x39DF0, 0x410,  2, 1, 0, 0, 0 }, // NA_BGM_OATH_TO_ORDER
    { 0x3A200, 0x2C0,  2, 2, 0, 0, 0 }, // NA_BGM_SWORD_TRAINING_HALL
    { 0x3A4C0, 0xD0,   2, 1, 0, 0, 0 }, // NA_BGM_GORON_LULLABY_INTRO
    { 0x3A590, 0x3E0,  2, 1, 0, 0, 0 }, // NA_BGM_OCA_FAIRY
    { 0x3A970, 0x320,  2, 1, 0, 0, 0 }, // NA_BGM_BREMEN_MARCH
    { 0x3AC90, 0x570,  2, 1, 0, 0, 0 }, // NA_BGM_BALLAD_OF_THE_WIND_FISH
    { 0x3B200, 0x480,  2, 1, 0, 0, 0 }, // NA_BGM_SONG_OF_SOARING
    { NA_BGM_MILK_BAR, 0x0, 2, 2, 0, 0, 0 }, // NA_BGM_MILK_BAR_DUPLICATE
    { 0x3B680, 0xCD0,  2, 2, 0, 0, 0 }, // NA_BGM_FINAL_HOURS
    { 0x3C350, 0x160,  2, 1, 0, 0, 0 }, // NA_BGM_MIKAU_RIFF
    { 0x3C4B0, 0x80,   2, 1, 0, 0, 0 }, // NA_BGM_MIKAU_FINALE
    { 0x3C530, 0x90,   2, 2, 0, 0, 0 }, // NA_BGM_FROG_SONG
    { 0x3C5C0, 0x110,  2, 1, 0, 0, 0 }, // NA_BGM_OCA_SONATA
    { 0x3C6D0, 0x110,  2, 1, 0, 0, 0 }, // NA_BGM_OCA_LULLABY
    { 0x3C7E0, 0x120,  2, 1, 0, 0, 0 }, // NA_BGM_OCA_NEW_WAVE
    { 0x3C900, 0xD0,   2, 1, 0, 0, 0 }, // NA_BGM_OCA_ELEGY
    { 0x3C9D0, 0xD0,   2, 1, 0, 0, 0 }, // NA_BGM_OCA_OATH
    { NA_BGM_FINAL_HOURS, 0x0, 2, 2, 0, 0, 0 }, // NA_BGM_MAJORAS_LAIR
    { NA_BGM_GORON_LULLABY_INTRO, 0x0, 2, 1, 0, 0, 0 }, // NA_BGM_OCA_LULLABY_INTRO
    { 0x3CAA0, 0x140,  2, 1, 0, 0, 0 }, // NA_BGM_OCA_GUITAR_BASS_SESSION
    { 0x3CBE0, 0x290,  2, 1, 0, 0, 0 }, // NA_BGM_PIANO_SESSION
    { 0x3CE70, 0x6B0,  2, 1, 0, 0, 0 }, // NA_BGM_INDIGO_GO_SESSION
    { 0x3D520, 0xF20,  2, 2, 0, 0, 0 }, // NA_BGM_SNOWHEAD_TEMPLE
    { 0x3E440, 0x1020, 2, 2, 0, 0, 0 }, // NA_BGM_GREAT_BAY_TEMPLE
    { 0x3F460, 0x10,   2, 1, 0, 0, 0 }, // NA_BGM_NEW_WAVE_SAXOPHONE
    { 0x3F470, 0x10,   2, 1, 0, 0, 0 }, // NA_BGM_NEW_WAVE_VOCAL
    { 0x3F480, 0x1D80, 2, 2, 0, 0, 0 }, // NA_BGM_MAJORAS_WRATH
    { 0x41200, 0xFE0,  2, 2, 0, 0, 0 }, // NA_BGM_MAJORAS_INCARNATION
    { 0x421E0, 0x1120, 2, 2, 0, 0, 0 }, // NA_BGM_MAJORAS_MASK
    { 0x43300, 0x120,  2, 1, 0, 0, 0 }, // NA_BGM_BASS_PLAY
    { 0x43420, 0x180,  2, 1, 0, 0, 0 }, // NA_BGM_DRUMS_PLAY
    { 0x435A0, 0x240,  2, 1, 0, 0, 0 }, // NA_BGM_PIANO_PLAY
    { 0x437E0, 0xE80,  2, 2, 0, 0, 0 }, // NA_BGM_IKANA_CASTLE
    { 0x44660, 0xDC0,  2, 2, 0, 0, 0 }, // NA_BGM_GATHERING_GIANTS
    { 0x45420, 0x270,  2, 1, 0, 0, 0 }, // NA_BGM_KAMARO_DANCE
    { 0x45690, 0x1060, 2, 2, 0, 0, 0 }, // NA_BGM_CREMIA_CARRIAGE
    { 0x466F0, 0x420,  2, 1, 0, 0, 0 }, // NA_BGM_KEATON_QUIZ
    { 0x46B10, 0x2360, 2, 2, 0, 0, 0 }, // NA_BGM_END_CREDITS_1
    { 0x48E70, 0xC50,  2, 2, 0, 0, 0 }, // NA_BGM_OPENING_LOOP
    { 0x49AC0, 0x1B40, 2, 2, 0, 0, 0 }, // NA_BGM_TITLE_THEME
    { 0x4B600, 0x2C0,  2, 2, 0, 0, 0 }, // NA_BGM_DUNGEON_APPEAR
    { 0x4B8C0, 0x350,  2, 2, 0, 0, 0 }, // NA_BGM_WOODFALL_CLEAR
    { 0x4BC10, 0x460,  2, 2, 0, 0, 0 }, // NA_BGM_SNOWHEAD_CLEAR
    { 0x4C070, 0x10,   2, 2, 0, 0, 0 }, //
    { 0x4C080, 0xD70,  2, 2, 0, 0, 0 }, // NA_BGM_INTO_THE_MOON
    { 0x4CDF0, 0x760,  2, 2, 0, 0, 0 }, // NA_BGM_GOODBYE_GIANT
    { 0x4D550, 0x610,  2, 2, 0, 0, 0 }, // NA_BGM_TATL_AND_TAEL
    { 0x4DB60, 0x6C0,  2, 2, 0, 0, 0 }, // NA_BGM_MOONS_DESTRUCTION
    { 0x4E220, 0x3260, 2, 2, 0, 0, 0 }, // NA_BGM_END_CREDITS_2
};

AudioTable gSampleBankTable = {
    3, 0, 0, 0,
    { 0x0,      0x538CC0, 2, 4, 0, 0, 0 },
    { 0x0,      0x0,      2, 4, 0, 0, 0 },
    { 0x538CC0, 0xFAB0,   2, 4, 0, 0, 0 },
};
