#!/usr/bin/env python3
import argparse
from cgitb import text
import dataclasses
import os
import struct
import sys
import typing

GETITEMENTRY_FORMAT = ">BBbBH"
GETITEMENTRY_LENGTH = struct.calcsize(GETITEMENTRY_FORMAT)

# MM US: 0x2E964

itemId_names = {
    0x00: "ITEM_OCARINA_OF_TIME",
    0x01: "ITEM_BOW",
    0x02: "ITEM_ARROW_FIRE",
    0x03: "ITEM_ARROW_ICE",
    0x04: "ITEM_ARROW_LIGHT",
    0x05: "ITEM_OCARINA_FAIRY",
    0x06: "ITEM_BOMB",
    0x07: "ITEM_BOMBCHU",
    0x08: "ITEM_DEKU_STICK",
    0x09: "ITEM_DEKU_NUT",
    0x0A: "ITEM_MAGIC_BEANS",
    0x0B: "ITEM_SLINGSHOT",
    0x0C: "ITEM_POWDER_KEG",
    0x0D: "ITEM_PICTOGRAPH_BOX",
    0x0E: "ITEM_LENS_OF_TRUTH",
    0x0F: "ITEM_HOOKSHOT",
    0x10: "ITEM_SWORD_GREAT_FAIRY",
    0x11: "ITEM_LONGSHOT",
    0x12: "ITEM_BOTTLE",
    0x13: "ITEM_POTION_RED",
    0x14: "ITEM_POTION_GREEN",
    0x15: "ITEM_POTION_BLUE",
    0x16: "ITEM_FAIRY",
    0x17: "ITEM_DEKU_PRINCESS",
    0x18: "ITEM_MILK_BOTTLE",
    0x19: "ITEM_MILK_HALF",
    0x1A: "ITEM_FISH",
    0x1B: "ITEM_BUG",
    0x1C: "ITEM_BLUE_FIRE",
    0x1D: "ITEM_POE",
    0x1E: "ITEM_BIG_POE",
    0x1F: "ITEM_SPRING_WATER",
    0x20: "ITEM_HOT_SPRING_WATER",
    0x21: "ITEM_ZORA_EGG",
    0x22: "ITEM_GOLD_DUST",
    0x23: "ITEM_MUSHROOM",
    0x24: "ITEM_SEAHORSE",
    0x25: "ITEM_CHATEAU",
    0x26: "ITEM_HYLIAN_LOACH",
    0x27: "ITEM_OBABA_DRINK",
    0x28: "ITEM_MOONS_TEAR",
    0x29: "ITEM_DEED_LAND",
    0x2A: "ITEM_DEED_SWAMP",
    0x2B: "ITEM_DEED_MOUNTAIN",
    0x2C: "ITEM_DEED_OCEAN",
    0x2D: "ITEM_ROOM_KEY",
    0x2E: "ITEM_LETTER_MAMA",
    0x2F: "ITEM_LETTER_TO_KAFEI",
    0x30: "ITEM_PENDANT_OF_MEMORIES",
    0x31: "ITEM_TINGLE_MAP",
    0x32: "ITEM_MASK_DEKU",
    0x33: "ITEM_MASK_GORON",
    0x34: "ITEM_MASK_ZORA",
    0x35: "ITEM_MASK_FIERCE_DEITY",
    0x36: "ITEM_MASK_TRUTH",
    0x37: "ITEM_MASK_KAFEIS_MASK",
    0x38: "ITEM_MASK_ALL_NIGHT",
    0x39: "ITEM_MASK_BUNNY",
    0x3A: "ITEM_MASK_KEATON",
    0x3B: "ITEM_MASK_GARO",
    0x3C: "ITEM_MASK_ROMANI",
    0x3D: "ITEM_MASK_CIRCUS_LEADER",
    0x3E: "ITEM_MASK_POSTMAN",
    0x3F: "ITEM_MASK_COUPLE",
    0x40: "ITEM_MASK_GREAT_FAIRY",
    0x41: "ITEM_MASK_GIBDO",
    0x42: "ITEM_MASK_DON_GERO",
    0x43: "ITEM_MASK_KAMARO",
    0x44: "ITEM_MASK_CAPTAIN",
    0x45: "ITEM_MASK_STONE",
    0x46: "ITEM_MASK_BREMEN",
    0x47: "ITEM_MASK_BLAST",
    0x48: "ITEM_MASK_SCENTS",
    0x49: "ITEM_MASK_GIANT",
    0x4A: "ITEM_BOW_FIRE",
    0x4B: "ITEM_BOW_ICE",
    0x4C: "ITEM_BOW_LIGHT",
    0x4D: "ITEM_SWORD_KOKIRI",
    0x4E: "ITEM_SWORD_RAZOR",
    0x4F: "ITEM_SWORD_GILDED",
    0x50: "ITEM_SWORD_DEITY",
    0x51: "ITEM_SHIELD_HERO",
    0x52: "ITEM_SHIELD_MIRROR",
    0x53: "ITEM_QUIVER_30",
    0x54: "ITEM_QUIVER_40",
    0x55: "ITEM_QUIVER_50",
    0x56: "ITEM_BOMB_BAG_20",
    0x57: "ITEM_BOMB_BAG_30",
    0x58: "ITEM_BOMB_BAG_40",
    0x59: "ITEM_WALLET_DEFAULT",
    0x5A: "ITEM_WALLET_ADULT",
    0x5B: "ITEM_WALLET_GIANT",
    0x5C: "ITEM_FISHING_POLE",
    0x5D: "ITEM_REMAINS_ODOLWA",
    0x5E: "ITEM_REMAINS_GOHT",
    0x5F: "ITEM_REMAINS_GYORG",
    0x60: "ITEM_REMAINS_TWINMOLD",
    0x61: "ITEM_SONG_SONATA",
    0x62: "ITEM_SONG_LULLABY",
    0x63: "ITEM_SONG_NOVA",
    0x64: "ITEM_SONG_ELEGY",
    0x65: "ITEM_SONG_OATH",
    0x66: "ITEM_SONG_SARIA",
    0x67: "ITEM_SONG_TIME",
    0x68: "ITEM_SONG_HEALING",
    0x69: "ITEM_SONG_EPONA",
    0x6A: "ITEM_SONG_SOARING",
    0x6B: "ITEM_SONG_STORMS",
    0x6C: "ITEM_SONG_SUN",
    0x6D: "ITEM_BOMBERS_NOTEBOOK",
    0x6E: "ITEM_SKULL_TOKEN",
    0x6F: "ITEM_HEART_CONTAINER",
    0x70: "ITEM_HEART_PIECE",
    0x73: "ITEM_SONG_LULLABY_INTRO" ,
    0x74: "ITEM_KEY_BOSS",
    0x75: "ITEM_COMPASS",
    0x76: "ITEM_DUNGEON_MAP",
    0x77: "ITEM_STRAY_FAIRIES",
    0x78: "ITEM_KEY_SMALL",
    0x79: "ITEM_MAGIC_JAR_SMALL",
    0x7A: "ITEM_MAGIC_JAR_BIG",
    0x7B: "ITEM_HEART_PIECE_2",
    0x7C: "ITEM_INVALID_1",
    0x7D: "ITEM_INVALID_2",
    0x7E: "ITEM_INVALID_3",
    0x7F: "ITEM_INVALID_4",
    0x80: "ITEM_INVALID_5",
    0x81: "ITEM_INVALID_6",
    0x82: "ITEM_INVALID_7",
    0x83: "ITEM_RECOVERY_HEART",
    0x84: "ITEM_RUPEE_GREEN",
    0x85: "ITEM_RUPEE_BLUE",
    0x86: "ITEM_RUPEE_10",
    0x87: "ITEM_RUPEE_RED",
    0x88: "ITEM_RUPEE_PURPLE",
    0x89: "ITEM_RUPEE_SILVER",
    0x8A: "ITEM_RUPEE_HUGE",
    0x8B: "ITEM_DEKU_STICKS_5",
    0x8C: "ITEM_DEKU_STICKS_10",
    0x8D: "ITEM_DEKU_NUTS_5",
    0x8E: "ITEM_DEKU_NUTS_10",
    0x8F: "ITEM_BOMBS_5",
    0x90: "ITEM_BOMBS_10",
    0x91: "ITEM_BOMBS_20",
    0x92: "ITEM_BOMBS_30",
    0x93: "ITEM_ARROWS_10",
    0x94: "ITEM_ARROWS_30",
    0x95: "ITEM_ARROWS_40",
    0x96: "ITEM_ARROWS_50",
    0x97: "ITEM_BOMBCHUS_20",
    0x98: "ITEM_BOMBCHUS_10",
    0x99: "ITEM_BOMBCHUS_1",
    0x9A: "ITEM_BOMBCHUS_5",
    0x9B: "ITEM_DEKU_STICK_UPGRADE_20",
    0x9C: "ITEM_DEKU_STICK_UPGRADE_30",
    0x9D: "ITEM_DEKU_NUT_UPGRADE_30",
    0x9E: "ITEM_DEKU_NUT_UPGRADE_40",
    0x9F: "ITEM_CHATEAU_2",
    0xA0: "ITEM_MILK",
    0xA1: "ITEM_GOLD_DUST_2",
    0xA2: "ITEM_HYLIAN_LOACH_2",
    0xA3: "ITEM_SEAHORSE_CAUGHT",
    0xF0: "ITEM_F0" ,
    0xF1: "ITEM_F1",
    0xF2: "ITEM_F2",
    0xFC: "ITEM_FC" ,
    0xFD: "ITEM_FD",
    0xFE: "ITEM_FE",
    0xFF: "ITEM_NONE",
}

getItemId_names = {
    0x00: "GI_NONE",
    0x01: "GI_RUPEE_GREEN",
    0x02: "GI_RUPEE_BLUE",
    0x03: "GI_RUPEE_10",
    0x04: "GI_RUPEE_RED",
    0x05: "GI_RUPEE_PURPLE",
    0x06: "GI_RUPEE_SILVER",
    0x07: "GI_RUPEE_HUGE",
    0x08: "GI_WALLET_ADULT",
    0x09: "GI_WALLET_GIANT",
    0x0A: "GI_RECOVERY_HEART",
    0x0C: "GI_HEART_PIECE",
    0x0D: "GI_HEART_CONTAINER",
    0x0E: "GI_MAGIC_JAR_SMALL",
    0x0F: "GI_MAGIC_JAR_BIG",
    0x11: "GI_STRAY_FAIRY",
    0x14: "GI_BOMBS_1",
    0x15: "GI_BOMBS_5",
    0x16: "GI_BOMBS_10",
    0x17: "GI_BOMBS_20",
    0x18: "GI_BOMBS_30",
    0x19: "GI_DEKU_STICKS_1",
    0x1A: "GI_BOMBCHUS_10",
    0x1B: "GI_BOMB_BAG_20",
    0x1C: "GI_BOMB_BAG_30",
    0x1D: "GI_BOMB_BAG_40",
    0x1E: "GI_ARROWS_10",
    0x1F: "GI_ARROWS_30",
    0x20: "GI_ARROWS_40",
    0x21: "GI_ARROWS_50",
    0x22: "GI_QUIVER_30",
    0x23: "GI_QUIVER_40",
    0x24: "GI_QUIVER_50",
    0x25: "GI_ARROW_FIRE",
    0x26: "GI_ARROW_ICE",
    0x27: "GI_ARROW_LIGHT",
    0x28: "GI_DEKU_NUTS_1",
    0x29: "GI_DEKU_NUTS_5",
    0x2A: "GI_DEKU_NUTS_10",
    0x2E: "GI_BOMBCHUS_20",
    0x32: "GI_SHIELD_HERO",
    0x33: "GI_SHIELD_MIRROR",
    0x34: "GI_POWDER_KEG",
    0x35: "GI_MAGIC_BEANS",
    0x36: "GI_BOMBCHUS_1",
    0x37: "GI_SWORD_KOKIRI",
    0x38: "GI_SWORD_RAZOR",
    0x39: "GI_SWORD_GILDED",
    0x3A: "GI_BOMBCHUS_5",
    0x3B: "GI_SWORD_GREAT_FAIRY",
    0x3C: "GI_KEY_SMALL",
    0x3D: "GI_KEY_BOSS",
    0x3E: "GI_MAP",
    0x3F: "GI_COMPASS",
    0x40: "GI_40",
    0x41: "GI_HOOKSHOT",
    0x42: "GI_LENS_OF_TRUTH",
    0x43: "GI_PICTOGRAPH_BOX",
    0x4C: "GI_OCARINA_OF_TIME",
    0x50: "GI_BOMBERS_NOTEBOOK",
    0x52: "GI_SKULL_TOKEN",
    0x55: "GI_REMAINS_ODOLWA",
    0x56: "GI_REMAINS_GOHT",
    0x57: "GI_REMAINS_GYORG",
    0x58: "GI_REMAINS_TWINMOLD",
    0x59: "GI_POTION_RED_BOTTLE",
    0x5A: "GI_BOTTLE",
    0x5B: "GI_POTION_RED",
    0x5C: "GI_POTION_GREEN",
    0x5D: "GI_POTION_BLUE",
    0x5E: "GI_FAIRY",
    0x5F: "GI_DEKU_PRINCESS",
    0x60: "GI_MILK_BOTTLE",
    0x61: "GI_MILK_HALF",
    0x62: "GI_FISH",
    0x63: "GI_BUG",
    0x64: "GI_BLUE_FIRE",
    0x65: "GI_POE",
    0x66: "GI_BIG_POE",
    0x67: "GI_SPRING_WATER",
    0x68: "GI_HOT_SPRING_WATER",
    0x69: "GI_ZORA_EGG",
    0x6A: "GI_GOLD_DUST",
    0x6B: "GI_MUSHROOM",
    0x6C: "GI_6C",
    0x6D: "GI_6D",
    0x6E: "GI_SEAHORSE",
    0x6F: "GI_CHATEAU_BOTTLE",
    0x70: "GI_70",
    0x75: "GI_75",
    0x76: "GI_ICE_TRAP",
    0x78: "GI_MASK_DEKU",
    0x79: "GI_MASK_GORON",
    0x7A: "GI_MASK_ZORA",
    0x7B: "GI_MASK_FIERCE_DEITY",
    0x7C: "GI_MASK_CAPTAIN",
    0x7D: "GI_MASK_GIANT",
    0x7E: "GI_MASK_ALL_NIGHT",
    0x7F: "GI_MASK_BUNNY",
    0x80: "GI_MASK_KEATON",
    0x81: "GI_MASK_GARO",
    0x82: "GI_MASK_ROMANI",
    0x83: "GI_MASK_CIRCUS_LEADER",
    0x84: "GI_MASK_POSTMAN",
    0x85: "GI_MASK_COUPLE",
    0x86: "GI_MASK_GREAT_FAIRY",
    0x87: "GI_MASK_GIBDO",
    0x88: "GI_MASK_DON_GERO",
    0x89: "GI_MASK_KAMARO",
    0x8A: "GI_MASK_TRUTH",
    0x8B: "GI_MASK_STONE",
    0x8C: "GI_MASK_BREMEN",
    0x8D: "GI_MASK_BLAST",
    0x8E: "GI_MASK_SCENTS",
    0x8F: "GI_MASK_KAFEIS_MASK",
    0x91: "GI_CHATEAU",
    0x92: "GI_MILK",
    0x93: "GI_GOLD_DUST_2",
    0x94: "GI_HYLIAN_LOACH",
    0x95: "GI_SEAHORSE_CAUGHT",
    0x96: "GI_MOONS_TEAR",
    0x97: "GI_DEED_LAND",
    0x98: "GI_DEED_SWAMP",
    0x99: "GI_DEED_MOUNTAIN",
    0x9A: "GI_DEED_OCEAN",
    0x9B: "GI_SWORD_GREAT_FAIRY_STOLEN",
    0x9C: "GI_SWORD_KOKIRI_STOLEN",
    0x9D: "GI_SWORD_RAZOR_STOLEN",
    0x9E: "GI_SWORD_GILDED_STOLEN",
    0x9F: "GI_SHIELD_HERO_STOLEN",
    0xA0: "GI_ROOM_KEY",
    0xA1: "GI_LETTER_TO_MAMA",
    0xA2: "GI_A2",
    0xA3: "GI_A3",
    0xA4: "GI_A4",
    0xA5: "GI_A5",
    0xA6: "GI_A6",
    0xA7: "GI_A7",
    0xA8: "GI_A8",
    0xA9: "GI_BOTTLE_STOLEN",
    0xAA: "GI_LETTER_TO_KAFEI",
    0xAB: "GI_PENDANT_OF_MEMORIES",
    0xB4: "GI_TINGLE_MAP_CLOCK_TOWN",
    0xB5: "GI_TINGLE_MAP_WOODFALL",
    0xB6: "GI_TINGLE_MAP_SNOWHEAD",
    0xB7: "GI_TINGLE_MAP_ROMANI_RANCH",
    0xB8: "GI_TINGLE_MAP_GREAT_BAY",
    0xB9: "GI_TINGLE_MAP_STONE_TOWER",
    0xBA: "GI_MAX" # I pity the fool
}

getItemDrawId_names = {
    0x00: "GID_BOTTLE",
    0x01: "GID_KEY_SMALL",
    0x02: "GID_MASK_KAMARO",
    0x03: "GID_MASK_COUPLE",
    0x04: "GID_04",
    0x05: "GID_05",
    0x06: "GID_06",
    0x07: "GID_07",
    0x08: "GID_RECOVERY_HEART",
    0x09: "GID_KEY_BOSS",
    0x0A: "GID_COMPASS",
    0x0B: "GID_BOMBERS_NOTEBOOK",
    0x0C: "GID_MASK_STONE",
    0x0D: "GID_MASK_KAFEIS_MASK",
    0x0E: "GID_MASK_CIRCUS_LEADER",
    0x0F: "GID_MASK_BREMEN",
    0x10: "GID_MASK_ALL_NIGHT",
    0x11: "GID_DEKU_NUTS",
    0x12: "GID_HEART_CONTAINER",
    0x13: "GID_HEART_PIECE",
    0x14: "GID_QUIVER_30",
    0x15: "GID_QUIVER_40",
    0x16: "GID_QUIVER_50",
    0x17: "GID_BOMB_BAG_20",
    0x18: "GID_BOMB_BAG_30",
    0x19: "GID_BOMB_BAG_40",
    0x1A: "GID_DEKU_STICK",
    0x1B: "GID_DUNGEON_MAP",
    0x1C: "GID_MAGIC_JAR_SMALL",
    0x1D: "GID_MAGIC_JAR_BIG",
    0x1E: "GID_BOMB",
    0x1F: "GID_STONE_OF_AGONY",
    0x20: "GID_WALLET_ADULT",
    0x21: "GID_WALLET_GIANT",
    0x22: "GID_MASK_DON_GERO",
    0x23: "GID_ARROWS_SMALL",
    0x24: "GID_ARROWS_MEDIUM",
    0x25: "GID_ARROWS_LARGE",
    0x26: "GID_BOMBCHU",
    0x27: "GID_SHIELD_HERO",
    0x28: "GID_HOOKSHOT",
    0x29: "GID_29",
    0x2A: "GID_OCARINA",
    0x2B: "GID_MILK",
    0x2C: "GID_MASK_KEATON",
    0x2D: "GID_TINGLE_MAP",
    0x2E: "GID_BOW",
    0x2F: "GID_LENS",
    0x30: "GID_POTION_GREEN",
    0x31: "GID_POTION_RED",
    0x32: "GID_POTION_BLUE",
    0x33: "GID_SHIELD_MIRROR",
    0x34: "GID_MAGIC_BEANS",
    0x35: "GID_FISH",
    0x36: "GID_LETTER_MAMA",
    0x37: "GID_37",
    0x38: "GID_SWORD_BGS",
    0x39: "GID_MASK_SUN",
    0x3A: "GID_MASK_BLAST",
    0x3B: "GID_FAIRY",
    0x3C: "GID_MASK_SCENTS",
    0x3D: "GID_MASK_CAPTAIN",
    0x3E: "GID_MASK_BUNNY",
    0x3F: "GID_MASK_TRUTH",
    0x40: "GID_DEED_SWAMP",
    0x41: "GID_DEED_MOUNTAIN",
    0x42: "GID_RUPEE_HUGE",
    0x43: "GID_DEED_OCEAN",
    0x44: "GID_MASK_GORON",
    0x45: "GID_MASK_ZORA",
    0x46: "GID_46",
    0x47: "GID_ARROW_FIRE",
    0x48: "GID_ARROW_ICE",
    0x49: "GID_ARROW_LIGHT",
    0x4A: "GID_SKULL_TOKEN",
    0x4B: "GID_BUG",
    0x4C: "GID_4C",
    0x4D: "GID_POE",
    0x4E: "GID_FAIRY_2",
    0x4F: "GID_RUPEE_GREEN",
    0x50: "GID_RUPEE_BLUE",
    0x51: "GID_RUPEE_RED",
    0x52: "GID_BIG_POE",
    0x53: "GID_RUPEE_PURPLE",
    0x54: "GID_RUPEE_SILVER",
    0x55: "GID_SWORD_KOKIRI",
    0x56: "GID_SKULL_TOKEN_2",
    0x57: "GID_57",
    0x58: "GID_ZORA_EGG",
    0x59: "GID_MOONS_TEAR",
    0x5A: "GID_DEED_LAND",
    0x5B: "GID_MASK_DEKU",
    0x5C: "GID_REMAINS_ODOLWA",
    0x5D: "GID_POWDER_KEG",
    0x5E: "GID_GOLD_DUST",
    0x5F: "GID_SEAHORSE",
    0x60: "GID_60",
    0x61: "GID_HYLIAN_LOACH",
    0x62: "GID_SEAHORSE_CAUGHT",
    0x63: "GID_REMAINS_GOHT",
    0x64: "GID_REMAINS_GYORG",
    0x65: "GID_REMAINS_TWINMOLD",
    0x66: "GID_SWORD_RAZOR",
    0x67: "GID_SWORD_GILDED",
    0x68: "GID_SWORD_GREAT_FAIRY",
    0x69: "GID_MASK_GARO",
    0x6A: "GID_MASK_GREAT_FAIRY",
    0x6B: "GID_MASK_GIBDO",
    0x6C: "GID_ROOM_KEY",
    0x6D: "GID_LETTER_TO_KAFEI",
    0x6E: "GID_PENDANT_OF_MEMORIES",
    0x6F: "GID_MUSHROOM",
    0x70: "GID_MASK_ROMANI",
    0x71: "GID_MASK_POSTMAN",
    0x72: "GID_MASK_GIANT",
    0x73: "GID_CHATEAU",
    0x74: "GID_PICTOGRAPH_BOX",
    0x75: "GID_MASK_FIERCE_DEITY",
    0x76: "GID_MAX",
    -1: "GID_NONE",
}

objectId_names = {
    0x000: "OBJECT_UNSET_0",
    0x001: "GAMEPLAY_KEEP",
    0x002: "GAMEPLAY_FIELD_KEEP",
    0x003: "GAMEPLAY_DANGEON_KEEP",
    0x004: "OBJECT_NB",
    0x005: "OBJECT_OKUTA",
    0x006: "OBJECT_CROW",
    0x007: "OBJECT_AH",
    0x008: "OBJECT_DY_OBJ",
    0x009: "OBJECT_WALLMASTER",
    0x00A: "OBJECT_DODONGO",
    0x00B: "OBJECT_FIREFLY",
    0x00C: "OBJECT_BOX",
    0x00D: "OBJECT_AL",
    0x00E: "OBJECT_BUBBLE",
    0x00F: "OBJECT_NIW",
    0x010: "OBJECT_LINK_BOY",
    0x011: "OBJECT_LINK_CHILD",
    0x012: "OBJECT_TITE",
    0x013: "OBJECT_TAB",
    0x014: "OBJECT_PH",
    0x015: "OBJECT_AND",
    0x016: "OBJECT_MSMO",
    0x017: "OBJECT_DINOFOS",
    0x018: "OBJECT_DRS",
    0x019: "OBJECT_ZL1",
    0x01A: "OBJECT_AN4",
    0x01B: "OBJECT_UNSET_1B",
    0x01C: "OBJECT_TEST3",
    0x01D: "OBJECT_FAMOS",
    0x01E: "OBJECT_UNSET_1E",
    0x01F: "OBJECT_UNSET_1F",
    0x020: "OBJECT_ST",
    0x021: "OBJECT_UNSET_21",
    0x022: "OBJECT_THIEFBIRD",
    0x023: "OBJECT_UNSET_23",
    0x024: "OBJECT_UNSET_24",
    0x025: "OBJECT_UNSET_25",
    0x026: "OBJECT_UNSET_26",
    0x027: "OBJECT_UNSET_27",
    0x028: "OBJECT_UNSET_28",
    0x029: "OBJECT_UNSET_29",
    0x02A: "OBJECT_BOMBF",
    0x02B: "OBJECT_UNSET_2B",
    0x02C: "OBJECT_UNSET_2C",
    0x02D: "OBJECT_UNSET_2D",
    0x02E: "OBJECT_UNSET_2E",
    0x02F: "OBJECT_UNSET_2F",
    0x030: "OBJECT_AM",
    0x031: "OBJECT_DEKUBABA",
    0x032: "OBJECT_UNSET_32",
    0x033: "OBJECT_UNSET_33",
    0x034: "OBJECT_UNSET_34",
    0x035: "OBJECT_UNSET_35",
    0x036: "OBJECT_UNSET_36",
    0x037: "OBJECT_UNSET_37",
    0x038: "OBJECT_UNSET_38",
    0x039: "OBJECT_UNSET_39",
    0x03A: "OBJECT_UNSET_3A",
    0x03B: "OBJECT_UNSET_3B",
    0x03C: "OBJECT_UNSET_3C",
    0x03D: "OBJECT_UNSET_3D",
    0x03E: "OBJECT_WARP1",
    0x03F: "OBJECT_B_HEART",
    0x040: "OBJECT_DEKUNUTS",
    0x041: "OBJECT_UNSET_41",
    0x042: "OBJECT_UNSET_42",
    0x043: "OBJECT_UNSET_43",
    0x044: "OBJECT_UNSET_44",
    0x045: "OBJECT_UNSET_45",
    0x046: "OBJECT_UNSET_46",
    0x047: "OBJECT_UNSET_47",
    0x048: "OBJECT_UNSET_48",
    0x049: "OBJECT_UNSET_49",
    0x04A: "OBJECT_UNSET_4A",
    0x04B: "OBJECT_UNSET_4B",
    0x04C: "OBJECT_UNSET_4C",
    0x04D: "OBJECT_UNSET_4D",
    0x04E: "OBJECT_UNSET_4E",
    0x04F: "OBJECT_UNSET_4F",
    0x050: "OBJECT_UNSET_50",
    0x051: "OBJECT_BB",
    0x052: "OBJECT_DEATH",
    0x053: "OBJECT_UNSET_53",
    0x054: "OBJECT_UNSET_54",
    0x055: "OBJECT_UNSET_55",
    0x056: "OBJECT_UNSET_56",
    0x057: "OBJECT_UNSET_57",
    0x058: "OBJECT_UNSET_58",
    0x059: "OBJECT_UNSET_59",
    0x05A: "OBJECT_UNSET_5A",
    0x05B: "OBJECT_UNSET_5B",
    0x05C: "OBJECT_F40_OBJ",
    0x05D: "OBJECT_PO_COMPOSER",
    0x05E: "OBJECT_UNSET_5E",
    0x05F: "OBJECT_HATA",
    0x060: "OBJECT_UNSET_60",
    0x061: "OBJECT_WOOD02",
    0x062: "OBJECT_UNSET_62",
    0x063: "OBJECT_UNSET_63",
    0x064: "OBJECT_TRAP",
    0x065: "OBJECT_UNSET_65",
    0x066: "OBJECT_UNSET_66",
    0x067: "OBJECT_UNSET_67",
    0x068: "OBJECT_UNSET_68",
    0x069: "OBJECT_UNSET_69",
    0x06A: "OBJECT_VM",
    0x06B: "OBJECT_UNSET_6B",
    0x06C: "OBJECT_UNSET_6C",
    0x06D: "OBJECT_UNSET_6D",
    0x06E: "OBJECT_UNSET_6E",
    0x06F: "OBJECT_UNSET_6F",
    0x070: "OBJECT_EFC_STAR_FIELD",
    0x071: "OBJECT_UNSET_71",
    0x072: "OBJECT_UNSET_72",
    0x073: "OBJECT_UNSET_73",
    0x074: "OBJECT_UNSET_74",
    0x075: "OBJECT_RD",
    0x076: "OBJECT_YUKIMURA_OBJ",
    0x077: "OBJECT_HEAVY_OBJECT",
    0x078: "OBJECT_UNSET_78",
    0x079: "OBJECT_UNSET_79",
    0x07A: "OBJECT_UNSET_7A",
    0x07B: "OBJECT_UNSET_7B",
    0x07C: "OBJECT_UNSET_7C",
    0x07D: "OBJECT_HORSE_LINK_CHILD",
    0x07E: "OBJECT_UNSET_7E",
    0x07F: "OBJECT_UNSET_7F",
    0x080: "OBJECT_SYOKUDAI",
    0x081: "OBJECT_UNSET_81",
    0x082: "OBJECT_UNSET_82",
    0x083: "OBJECT_UNSET_83",
    0x084: "OBJECT_EFC_TW",
    0x085: "OBJECT_UNSET_85",
    0x086: "OBJECT_GI_KEY",
    0x087: "OBJECT_MIR_RAY",
    0x088: "OBJECT_CTOWER_ROT",
    0x089: "OBJECT_UNSET_89",
    0x08A: "OBJECT_BDOOR",
    0x08B: "OBJECT_UNSET_8B",
    0x08C: "OBJECT_UNSET_8C",
    0x08D: "OBJECT_UNSET_8D",
    0x08E: "OBJECT_SB",
    0x08F: "OBJECT_GI_MELODY",
    0x090: "OBJECT_GI_HEART",
    0x091: "OBJECT_GI_COMPASS",
    0x092: "OBJECT_GI_BOSSKEY",
    0x093: "OBJECT_UNSET_93",
    0x094: "OBJECT_GI_NUTS",
    0x095: "OBJECT_UNSET_95",
    0x096: "OBJECT_GI_HEARTS",
    0x097: "OBJECT_GI_ARROWCASE",
    0x098: "OBJECT_GI_BOMBPOUCH",
    0x099: "OBJECT_IN",
    0x09A: "OBJECT_UNSET_9A",
    0x09B: "OBJECT_UNSET_9B",
    0x09C: "OBJECT_UNSET_9C",
    0x09D: "OBJECT_OS_ANIME",
    0x09E: "OBJECT_GI_BOTTLE",
    0x09F: "OBJECT_GI_STICK",
    0x0A0: "OBJECT_GI_MAP",
    0x0A1: "OBJECT_OF1D_MAP",
    0x0A2: "OBJECT_RU2",
    0x0A3: "OBJECT_UNSET_A3",
    0x0A4: "OBJECT_GI_MAGICPOT",
    0x0A5: "OBJECT_GI_BOMB_1",
    0x0A6: "OBJECT_UNSET_A6",
    0x0A7: "OBJECT_MA2",
    0x0A8: "OBJECT_GI_PURSE",
    0x0A9: "OBJECT_UNSET_A9",
    0x0AA: "OBJECT_UNSET_AA",
    0x0AB: "OBJECT_RR",
    0x0AC: "OBJECT_UNSET_AC",
    0x0AD: "OBJECT_UNSET_AD",
    0x0AE: "OBJECT_UNSET_AE",
    0x0AF: "OBJECT_GI_ARROW",
    0x0B0: "OBJECT_GI_BOMB_2",
    0x0B1: "OBJECT_UNSET_B1",
    0x0B2: "OBJECT_UNSET_B2",
    0x0B3: "OBJECT_GI_SHIELD_2",
    0x0B4: "OBJECT_GI_HOOKSHOT",
    0x0B5: "OBJECT_GI_OCARINA",
    0x0B6: "OBJECT_GI_MILK",
    0x0B7: "OBJECT_MA1",
    0x0B8: "OBJECT_UNSET_B8",
    0x0B9: "OBJECT_UNSET_B9",
    0x0BA: "OBJECT_UNSET_BA",
    0x0BB: "OBJECT_NY",
    0x0BC: "OBJECT_FR",
    0x0BD: "OBJECT_UNSET_BD",
    0x0BE: "OBJECT_UNSET_BE",
    0x0BF: "OBJECT_GI_BOW",
    0x0C0: "OBJECT_GI_GLASSES",
    0x0C1: "OBJECT_GI_LIQUID",
    0x0C2: "OBJECT_ANI",
    0x0C3: "OBJECT_GI_SHIELD_3",
    0x0C4: "OBJECT_UNSET_C4",
    0x0C5: "OBJECT_UNSET_C5",
    0x0C6: "OBJECT_GI_BEAN",
    0x0C7: "OBJECT_GI_FISH",
    0x0C8: "OBJECT_UNSET_C8",
    0x0C9: "OBJECT_UNSET_C9",
    0x0CA: "OBJECT_UNSET_CA",
    0x0CB: "OBJECT_GI_LONGSWORD",
    0x0CC: "OBJECT_UNSET_CC",
    0x0CD: "OBJECT_UNSET_CD",
    0x0CE: "OBJECT_UNSET_CE",
    0x0CF: "OBJECT_UNSET_CF",
    0x0D0: "OBJECT_ZO",
    0x0D1: "OBJECT_UNSET_D1",
    0x0D2: "OBJECT_UMAJUMP",
    0x0D3: "OBJECT_UNSET_D3",
    0x0D4: "OBJECT_UNSET_D4",
    0x0D5: "OBJECT_MASTERGOLON",
    0x0D6: "OBJECT_MASTERZOORA",
    0x0D7: "OBJECT_AOB",
    0x0D8: "OBJECT_IK",
    0x0D9: "OBJECT_AHG",
    0x0DA: "OBJECT_CNE",
    0x0DB: "OBJECT_UNSET_DB",
    0x0DC: "OBJECT_UNSET_DC",
    0x0DD: "OBJECT_AN3",
    0x0DE: "OBJECT_BJI",
    0x0DF: "OBJECT_BBA",
    0x0E0: "OBJECT_AN2",
    0x0E1: "OBJECT_UNSET_E1",
    0x0E2: "OBJECT_AN1",
    0x0E3: "OBJECT_BOJ",
    0x0E4: "OBJECT_FZ",
    0x0E5: "OBJECT_BOB",
    0x0E6: "OBJECT_GE1",
    0x0E7: "OBJECT_YABUSAME_POINT",
    0x0E8: "OBJECT_UNSET_E8",
    0x0E9: "OBJECT_UNSET_E9",
    0x0EA: "OBJECT_UNSET_EA",
    0x0EB: "OBJECT_UNSET_EB",
    0x0EC: "OBJECT_D_HSBLOCK",
    0x0ED: "OBJECT_D_LIFT",
    0x0EE: "OBJECT_MAMENOKI",
    0x0EF: "OBJECT_GOROIWA",
    0x0F0: "OBJECT_TORYO",
    0x0F1: "OBJECT_DAIKU",
    0x0F2: "OBJECT_NWC",
    0x0F3: "OBJECT_GM",
    0x0F4: "OBJECT_MS",
    0x0F5: "OBJECT_HS",
    0x0F6: "OBJECT_UNSET_F6",
    0x0F7: "OBJECT_LIGHTSWITCH",
    0x0F8: "OBJECT_KUSA",
    0x0F9: "OBJECT_TSUBO",
    0x0FA: "OBJECT_UNSET_FA",
    0x0FB: "OBJECT_UNSET_FB",
    0x0FC: "OBJECT_KANBAN",
    0x0FD: "OBJECT_OWL",
    0x0FE: "OBJECT_MK",
    0x0FF: "OBJECT_FU",
    0x100: "OBJECT_GI_KI_TAN_MASK",
    0x101: "OBJECT_UNSET_101",
    0x102: "OBJECT_GI_MASK18",
    0x103: "OBJECT_GI_RABIT_MASK",
    0x104: "OBJECT_GI_TRUTH_MASK",
    0x105: "OBJECT_UNSET_105",
    0x106: "OBJECT_STREAM",
    0x107: "OBJECT_MM",
    0x108: "OBJECT_UNSET_108",
    0x109: "OBJECT_UNSET_109",
    0x10A: "OBJECT_UNSET_10A",
    0x10B: "OBJECT_UNSET_10B",
    0x10C: "OBJECT_UNSET_10C",
    0x10D: "OBJECT_UNSET_10D",
    0x10E: "OBJECT_UNSET_10E",
    0x10F: "OBJECT_JS",
    0x110: "OBJECT_CS",
    0x111: "OBJECT_UNSET_111",
    0x112: "OBJECT_UNSET_112",
    0x113: "OBJECT_GI_SOLDOUT",
    0x114: "OBJECT_UNSET_114",
    0x115: "OBJECT_MAG",
    0x116: "OBJECT_UNSET_116",
    0x117: "OBJECT_UNSET_117",
    0x118: "OBJECT_UNSET_118",
    0x119: "OBJECT_GI_GOLONMASK",
    0x11A: "OBJECT_GI_ZORAMASK",
    0x11B: "OBJECT_UNSET_11B",
    0x11C: "OBJECT_UNSET_11C",
    0x11D: "OBJECT_KA",
    0x11E: "OBJECT_UNSET_11E",
    0x11F: "OBJECT_ZG",
    0x120: "OBJECT_UNSET_120",
    0x121: "OBJECT_GI_M_ARROW",
    0x122: "OBJECT_DS2",
    0x123: "OBJECT_UNSET_123",
    0x124: "OBJECT_FISH",
    0x125: "OBJECT_GI_SUTARU",
    0x126: "OBJECT_UNSET_126",
    0x127: "OBJECT_SSH",
    0x128: "OBJECT_BIGSLIME",
    0x129: "OBJECT_BG",
    0x12A: "OBJECT_BOMBIWA",
    0x12B: "OBJECT_HINTNUTS",
    0x12C: "OBJECT_RS",
    0x12D: "OBJECT_UNSET_12D",
    0x12E: "OBJECT_GLA",
    0x12F: "OBJECT_UNSET_12F",
    0x130: "OBJECT_GELDB",
    0x131: "OBJECT_UNSET_131",
    0x132: "OBJECT_DOG",
    0x133: "OBJECT_KIBAKO2",
    0x134: "OBJECT_DNS",
    0x135: "OBJECT_DNK",
    0x136: "OBJECT_UNSET_136",
    0x137: "OBJECT_GI_INSECT",
    0x138: "OBJECT_UNSET_138",
    0x139: "OBJECT_GI_GHOST",
    0x13A: "OBJECT_GI_SOUL",
    0x13B: "OBJECT_UNSET_13B",
    0x13C: "OBJECT_UNSET_13C",
    0x13D: "OBJECT_UNSET_13D",
    0x13E: "OBJECT_UNSET_13E",
    0x13F: "OBJECT_GI_RUPY",
    0x140: "OBJECT_MU",
    0x141: "OBJECT_WF",
    0x142: "OBJECT_SKB",
    0x143: "OBJECT_GS",
    0x144: "OBJECT_PS",
    0x145: "OBJECT_OMOYA_OBJ",
    0x146: "OBJECT_COW",
    0x147: "OBJECT_UNSET_147",
    0x148: "OBJECT_GI_SWORD_1",
    0x149: "OBJECT_UNSET_149",
    0x14A: "OBJECT_UNSET_14A",
    0x14B: "OBJECT_ZL4",
    0x14C: "OBJECT_LINK_GORON",
    0x14D: "OBJECT_LINK_ZORA",
    0x14E: "OBJECT_GRASSHOPPER",
    0x14F: "OBJECT_BOYO",
    0x150: "OBJECT_UNSET_150",
    0x151: "OBJECT_UNSET_151",
    0x152: "OBJECT_UNSET_152",
    0x153: "OBJECT_FWALL",
    0x154: "OBJECT_LINK_NUTS",
    0x155: "OBJECT_JSO",
    0x156: "OBJECT_KNIGHT",
    0x157: "OBJECT_ICICLE",
    0x158: "OBJECT_SPDWEB",
    0x159: "OBJECT_UNSET_159",
    0x15A: "OBJECT_BOSS01",
    0x15B: "OBJECT_BOSS02",
    0x15C: "OBJECT_BOSS03",
    0x15D: "OBJECT_BOSS04",
    0x15E: "OBJECT_BOSS05",
    0x15F: "OBJECT_BOSS06",
    0x160: "OBJECT_BOSS07",
    0x161: "OBJECT_RAF",
    0x162: "OBJECT_FUNEN",
    0x163: "OBJECT_RAILLIFT",
    0x164: "OBJECT_NUMA_OBJ",
    0x165: "OBJECT_FLOWERPOT",
    0x166: "OBJECT_SPINYROLL",
    0x167: "OBJECT_ICE_BLOCK",
    0x168: "OBJECT_UNSET_168",
    0x169: "OBJECT_KEIKOKU_DEMO",
    0x16A: "OBJECT_SLIME",
    0x16B: "OBJECT_PR",
    0x16C: "OBJECT_F52_OBJ",
    0x16D: "OBJECT_F53_OBJ",
    0x16E: "OBJECT_UNSET_16E",
    0x16F: "OBJECT_KIBAKO",
    0x170: "OBJECT_SEK",
    0x171: "OBJECT_GMO",
    0x172: "OBJECT_BAT",
    0x173: "OBJECT_SEKIHIL",
    0x174: "OBJECT_SEKIHIG",
    0x175: "OBJECT_SEKIHIN",
    0x176: "OBJECT_SEKIHIZ",
    0x177: "OBJECT_UNSET_177",
    0x178: "OBJECT_WIZ",
    0x179: "OBJECT_LADDER",
    0x17A: "OBJECT_MKK",
    0x17B: "OBJECT_UNSET_17B",
    0x17C: "OBJECT_UNSET_17C",
    0x17D: "OBJECT_UNSET_17D",
    0x17E: "OBJECT_KEIKOKU_OBJ",
    0x17F: "OBJECT_SICHITAI_OBJ",
    0x180: "OBJECT_DEKUCITY_ANA_OBJ",
    0x181: "OBJECT_RAT",
    0x182: "OBJECT_WATER_EFFECT",
    0x183: "OBJECT_UNSET_183",
    0x184: "OBJECT_DBLUE_OBJECT",
    0x185: "OBJECT_BAL",
    0x186: "OBJECT_WARP_UZU",
    0x187: "OBJECT_DRIFTICE",
    0x188: "OBJECT_FALL",
    0x189: "OBJECT_HANAREYAMA_OBJ",
    0x18A: "OBJECT_CRACE_OBJECT",
    0x18B: "OBJECT_DNQ",
    0x18C: "OBJECT_OBJ_TOKEIDAI",
    0x18D: "OBJECT_EG",
    0x18E: "OBJECT_TRU",
    0x18F: "OBJECT_TRT",
    0x190: "OBJECT_HAKUGIN_OBJ",
    0x191: "OBJECT_HORSE_GAME_CHECK",
    0x192: "OBJECT_STK",
    0x193: "OBJECT_UNSET_193",
    0x194: "OBJECT_UNSET_194",
    0x195: "OBJECT_MNK",
    0x196: "OBJECT_GI_BOTTLE_RED",
    0x197: "OBJECT_TOKEI_TOBIRA",
    0x198: "OBJECT_AZ",
    0x199: "OBJECT_TWIG",
    0x19A: "OBJECT_DEKUCITY_OBJ",
    0x19B: "OBJECT_PO_FUSEN",
    0x19C: "OBJECT_RACETSUBO",
    0x19D: "OBJECT_HA",
    0x19E: "OBJECT_BIGOKUTA",
    0x19F: "OBJECT_OPEN_OBJ",
    0x1A0: "OBJECT_FU_KAITEN",
    0x1A1: "OBJECT_FU_MATO",
    0x1A2: "OBJECT_MTORIDE",
    0x1A3: "OBJECT_OSN",
    0x1A4: "OBJECT_TOKEI_STEP",
    0x1A5: "OBJECT_LOTUS",
    0x1A6: "OBJECT_TL",
    0x1A7: "OBJECT_DKJAIL_OBJ",
    0x1A8: "OBJECT_VISIBLOCK",
    0x1A9: "OBJECT_TSN",
    0x1AA: "OBJECT_DS2N",
    0x1AB: "OBJECT_FSN",
    0x1AC: "OBJECT_SHN",
    0x1AD: "OBJECT_BIGICICLE",
    0x1AE: "OBJECT_GI_BOTTLE_15",
    0x1AF: "OBJECT_TK",
    0x1B0: "OBJECT_MARKET_OBJ",
    0x1B1: "OBJECT_GI_RESERVE00",
    0x1B2: "OBJECT_GI_RESERVE01",
    0x1B3: "OBJECT_LIGHTBLOCK",
    0x1B4: "OBJECT_TAKARAYA_OBJECTS",
    0x1B5: "OBJECT_WDHAND",
    0x1B6: "OBJECT_SDN",
    0x1B7: "OBJECT_SNOWWD",
    0x1B8: "OBJECT_GIANT",
    0x1B9: "OBJECT_COMB",
    0x1BA: "OBJECT_HANA",
    0x1BB: "OBJECT_BOSS_HAKUGIN",
    0x1BC: "OBJECT_MEGANEANA_OBJ",
    0x1BD: "OBJECT_GI_NUTSMASK",
    0x1BE: "OBJECT_STK2",
    0x1BF: "OBJECT_SPOT11_OBJ",
    0x1C0: "OBJECT_DANPEI_OBJECT",
    0x1C1: "OBJECT_DHOUSE",
    0x1C2: "OBJECT_HAKAISI",
    0x1C3: "OBJECT_PO",
    0x1C4: "OBJECT_SNOWMAN",
    0x1C5: "OBJECT_PO_SISTERS",
    0x1C6: "OBJECT_PP",
    0x1C7: "OBJECT_GORONSWITCH",
    0x1C8: "OBJECT_DELF",
    0x1C9: "OBJECT_BOTIHASIRA",
    0x1CA: "OBJECT_GI_BIGBOMB",
    0x1CB: "OBJECT_PST",
    0x1CC: "OBJECT_BSMASK",
    0x1CD: "OBJECT_SPIDERTENT",
    0x1CE: "OBJECT_ZORAEGG",
    0x1CF: "OBJECT_KBT",
    0x1D0: "OBJECT_GG",
    0x1D1: "OBJECT_MARUTA",
    0x1D2: "OBJECT_GHAKA",
    0x1D3: "OBJECT_OYU",
    0x1D4: "OBJECT_DNP",
    0x1D5: "OBJECT_DAI",
    0x1D6: "OBJECT_KGY",
    0x1D7: "OBJECT_FB",
    0x1D8: "OBJECT_TAISOU",
    0x1D9: "OBJECT_MASK_BU_SAN",
    0x1DA: "OBJECT_MASK_KI_TAN",
    0x1DB: "OBJECT_MASK_RABIT",
    0x1DC: "OBJECT_MASK_SKJ",
    0x1DD: "OBJECT_MASK_BAKURETU",
    0x1DE: "OBJECT_MASK_TRUTH",
    0x1DF: "OBJECT_GK",
    0x1E0: "OBJECT_HAKA_OBJ",
    0x1E1: "OBJECT_MASK_GORON",
    0x1E2: "OBJECT_MASK_ZORA",
    0x1E3: "OBJECT_MASK_NUTS",
    0x1E4: "OBJECT_MASK_BOY",
    0x1E5: "OBJECT_DNT",
    0x1E6: "OBJECT_YUKIYAMA",
    0x1E7: "OBJECT_ICEFLOE",
    0x1E8: "OBJECT_GI_GOLD_DUST",
    0x1E9: "OBJECT_GI_BOTTLE_16",
    0x1EA: "OBJECT_GI_BOTTLE_22",
    0x1EB: "OBJECT_BEE",
    0x1EC: "OBJECT_OT",
    0x1ED: "OBJECT_UTUBO",
    0x1EE: "OBJECT_DORA",
    0x1EF: "OBJECT_GI_LOACH",
    0x1F0: "OBJECT_GI_SEAHORSE",
    0x1F1: "OBJECT_BIGPO",
    0x1F2: "OBJECT_HARIKO",
    0x1F3: "OBJECT_DNO",
    0x1F4: "OBJECT_SINKAI_KABE",
    0x1F5: "OBJECT_KIN2_OBJ",
    0x1F6: "OBJECT_ISHI",
    0x1F7: "OBJECT_HAKUGIN_DEMO",
    0x1F8: "OBJECT_JG",
    0x1F9: "OBJECT_GI_SWORD_2",
    0x1FA: "OBJECT_GI_SWORD_3",
    0x1FB: "OBJECT_GI_SWORD_4",
    0x1FC: "OBJECT_UM",
    0x1FD: "OBJECT_MASK_GIBUDO",
    0x1FE: "OBJECT_MASK_JSON",
    0x1FF: "OBJECT_MASK_KERFAY",
    0x200: "OBJECT_MASK_BIGELF",
    0x201: "OBJECT_RB",
    0x202: "OBJECT_MBAR_OBJ",
    0x203: "OBJECT_IKANA_OBJ",
    0x204: "OBJECT_KZ",
    0x205: "OBJECT_TOKEI_TURRET",
    0x206: "OBJECT_ZOG",
    0x207: "OBJECT_ROTLIFT",
    0x208: "OBJECT_POSTHOUSE_OBJ",
    0x209: "OBJECT_GI_MASK09",
    0x20A: "OBJECT_GI_MASK14",
    0x20B: "OBJECT_GI_MASK15",
    0x20C: "OBJECT_INIBS_OBJECT",
    0x20D: "OBJECT_TREE",
    0x20E: "OBJECT_KAIZOKU_OBJ",
    0x20F: "OBJECT_GI_RESERVE_B_00",
    0x210: "OBJECT_GI_RESERVE_C_00",
    0x211: "OBJECT_ZOB",
    0x212: "OBJECT_MILKBAR",
    0x213: "OBJECT_DMASK",
    0x214: "OBJECT_MASK_KYOJIN",
    0x215: "OBJECT_GI_RESERVE_C_01",
    0x216: "OBJECT_ZOD",
    0x217: "OBJECT_KUMO30",
    0x218: "OBJECT_OBJ_YASI",
    0x219: "OBJECT_MASK_ROMERNY",
    0x21A: "OBJECT_TANRON1",
    0x21B: "OBJECT_TANRON2",
    0x21C: "OBJECT_TANRON3",
    0x21D: "OBJECT_GI_MAGICMUSHROOM",
    0x21E: "OBJECT_OBJ_CHAN",
    0x21F: "OBJECT_GI_MASK10",
    0x220: "OBJECT_ZOS",
    0x221: "OBJECT_MASK_POSTHAT",
    0x222: "OBJECT_F40_SWITCH",
    0x223: "OBJECT_LODMOON",
    0x224: "OBJECT_TRO",
    0x225: "OBJECT_GI_MASK12",
    0x226: "OBJECT_GI_MASK23",
    0x227: "OBJECT_GI_BOTTLE_21",
    0x228: "OBJECT_GI_CAMERA",
    0x229: "OBJECT_KAMEJIMA",
    0x22A: "OBJECT_HARFGIBUD",
    0x22B: "OBJECT_ZOV",
    0x22C: "OBJECT_HGDOOR",
    0x22D: "OBJECT_UNSET_22D",
    0x22E: "OBJECT_UNSET_22E",
    0x22F: "OBJECT_UNSET_22F",
    0x230: "OBJECT_DOR01",
    0x231: "OBJECT_DOR02",
    0x232: "OBJECT_DOR03",
    0x233: "OBJECT_DOR04",
    0x234: "OBJECT_LAST_OBJ",
    0x235: "OBJECT_REDEAD_OBJ",
    0x236: "OBJECT_IKNINSIDE_OBJ",
    0x237: "OBJECT_IKNV_OBJ",
    0x238: "OBJECT_PAMERA",
    0x239: "OBJECT_HSSTUMP",
    0x23A: "OBJECT_ZM",
    0x23B: "OBJECT_BIG_FWALL",
    0x23C: "OBJECT_SECOM_OBJ",
    0x23D: "OBJECT_HUNSUI",
    0x23E: "OBJECT_UCH",
    0x23F: "OBJECT_TANRON4",
    0x240: "OBJECT_TANRON5",
    0x241: "OBJECT_DT",
    0x242: "OBJECT_GI_MASK03",
    0x243: "OBJECT_CHA",
    0x244: "OBJECT_OBJ_DINNER",
    0x245: "OBJECT_GI_RESERVE_B_01",
    0x246: "OBJECT_LASTDAY",
    0x247: "OBJECT_BAI",
    0x248: "OBJECT_IN2",
    0x249: "OBJECT_IKN_DEMO",
    0x24A: "OBJECT_YB",
    0x24B: "OBJECT_RZ",
    0x24C: "OBJECT_MASK_ZACHO",
    0x24D: "OBJECT_GI_FIELDMAP",
    0x24E: "OBJECT_MASK_STONE",
    0x24F: "OBJECT_BJT",
    0x250: "OBJECT_TARU",
    0x251: "OBJECT_MOONSTON",
    0x252: "OBJECT_MASK_BREE",
    0x253: "OBJECT_GI_SCHEDULE",
    0x254: "OBJECT_GI_STONEMASK",
    0x255: "OBJECT_ZORABAND",
    0x256: "OBJECT_KEPN_KOYA",
    0x257: "OBJECT_OBJ_USIYANE",
    0x258: "OBJECT_GI_MASK05",
    0x259: "OBJECT_GI_MASK11",
    0x25A: "OBJECT_GI_MASK20",
    0x25B: "OBJECT_NNH",
    0x25C: "OBJECT_MASK_GERO",
    0x25D: "OBJECT_MASK_YOFUKASI",
    0x25E: "OBJECT_MASK_MEOTO",
    0x25F: "OBJECT_MASK_DANCER",
    0x260: "OBJECT_KZSAKU",
    0x261: "OBJECT_OBJ_MILK_BIN",
    0x262: "OBJECT_RANDOM_OBJ",
    0x263: "OBJECT_KUJIYA",
    0x264: "OBJECT_KITAN",
    0x265: "OBJECT_GI_MASK06",
    0x266: "OBJECT_GI_MASK16",
    0x267: "OBJECT_ASTR_OBJ",
    0x268: "OBJECT_BSB",
    0x269: "OBJECT_FALL2",
    0x26A: "OBJECT_STH",
    0x26B: "OBJECT_GI_MSSA",
    0x26C: "OBJECT_SMTOWER",
    0x26D: "OBJECT_GI_MASK21",
    0x26E: "OBJECT_YADO_OBJ",
    0x26F: "OBJECT_SYOTEN",
    0x270: "OBJECT_MOONEND",
    0x271: "OBJECT_OB",
    0x272: "OBJECT_GI_BOTTLE_04",
    0x273: "OBJECT_OBJ_DANPEILIFT",
    0x274: "OBJECT_WDOR01",
    0x275: "OBJECT_WDOR02",
    0x276: "OBJECT_WDOR03",
    0x277: "OBJECT_STK3",
    0x278: "OBJECT_KINSTA1_OBJ",
    0x279: "OBJECT_KINSTA2_OBJ",
    0x27A: "OBJECT_BH",
    0x27B: "OBJECT_WDOR04",
    0x27C: "OBJECT_WDOR05",
    0x27D: "OBJECT_GI_MASK17",
    0x27E: "OBJECT_GI_MASK22",
    0x27F: "OBJECT_LBFSHOT",
    0x280: "OBJECT_FUSEN",
    0x281: "OBJECT_ENDING_OBJ",
    0x282: "OBJECT_GI_MASK13",
    0x283: "OBJECT_ID_MAX",
}

@dataclasses.dataclass
class GetItemEntry:
    getItemId: int
    itemId: int
    field: int
    gi: int
    textId: int
    objectId: int

    def __str__(self) -> str:
        itemId_name = itemId_names.get(self.itemId, f"NOT FOUND: 0x{self.itemId:X}")
        gid = abs(self.gi) - 1
        getitemDrawId_name = getItemDrawId_names.get(gid, f"NOT FOUND: 0x{abs(self.gi) - 1:X}")

        if self.gi == 0:
            chest_anim = 0
        else:
            chest_anim = "CHEST_ANIM_SHORT" if self.gi < 0 else "CHEST_ANIM_LONG"

        objectId_name = objectId_names.get(self.objectId, f"NOT FOUND: 0x{abs(self.objectId):X}")

        ret = f"/* 0x{self.getItemId:02X} */ GET_ITEM({itemId_name}, {objectId_name}, {getitemDrawId_name}, 0x{self.textId:X},  0x{self.field:X}, {chest_anim})"

        return ret

def main() -> None:
    description = "Extract and print the GetItem table"
    epilog = ""

    parser = argparse.ArgumentParser(description=description, epilog=epilog, formatter_class=argparse.RawTextHelpFormatter)
    parser.add_argument("file", help="binary file to read")
    parser.add_argument("offset", help="offset at which to start reading")
    parser.add_argument("count", help="number of entries to extract")
    parser.add_argument("-g", help="look further back for glitched items from Get Item Manipulation", action="store_true")

    args = parser.parse_args()

    start = int(args.offset, 0)
    count = int(args.count, 0)
    glitched = args.g
    pos_start = 0

    with open(args.file, "rb") as f:
        if glitched:
            start -= GETITEMENTRY_LENGTH * count
            pos_start -= count
            count *= 2

        f.seek(start, os.SEEK_SET)
        data = f.read(GETITEMENTRY_LENGTH * count)
        unpacked_data = struct.iter_unpack(GETITEMENTRY_FORMAT, data)
        get_item_entries: typing.List[GetItemEntry] = []

        for i,entry in enumerate(unpacked_data):
            get_item_entries.append(GetItemEntry(pos_start + i+1, *entry))

        for entry in get_item_entries:
            print(f"{entry},")



if __name__ == "__main__":
    main()
