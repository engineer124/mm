#include "global.h"

s16 sLowPassFilterData[] = {
    0x0000, 0x0000, 0x0000, 0x7FFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0F0E, 0x105C, 0x112E, 0x1175, 0x112E,
    0x105C, 0x0F0E, 0x0D58, 0x0D57, 0x10DA, 0x1333, 0x1406, 0x1333, 0x10DA, 0x0D57, 0x092F, 0x0A4C, 0x1151,
    0x1682, 0x186C, 0x1682, 0x1151, 0x0A4C, 0x0351, 0x0536, 0x1064, 0x19F6, 0x1DB9, 0x19F6, 0x1064, 0x0536,
    0xFCDE, 0xFEF7, 0x0D5D, 0x1C7C, 0x22F0, 0x1C7C, 0x0D5D, 0xFEF7, 0xF8B9, 0xF9EA, 0x0811, 0x1BEA, 0x254A,
    0x1BEA, 0x0811, 0xF9EA, 0xF96E, 0xF6CF, 0x02D6, 0x1D11, 0x2B14, 0x1D11, 0x02D6, 0xF6CF, 0xFD47, 0xF734,
    0xFD4B, 0x1BD1, 0x2EBA, 0x1BD1, 0xFD4B, 0xF734, 0x029C, 0xFAA3, 0xF8E5, 0x189B, 0x300A, 0x189B, 0xF8E5,
    0xFAA3, 0x05CC, 0xFF2B, 0xF54C, 0x16D3, 0x3570, 0x16D3, 0xF54C, 0xFF2B, 0x05D6, 0x03D4, 0xF3F7, 0x1313,
    0x37CE, 0x1313, 0xF3F7, 0x03D4, 0x024E, 0x06E9, 0xF463, 0x0F1A, 0x3A85, 0x0F1A, 0xF463, 0x06E9, 0xFDC8,
    0x07E7, 0xF606, 0x0B5F, 0x400D, 0x0B5F, 0xF606, 0x07E7, 0xFA91, 0x06E6, 0xF882, 0x07E0, 0x4D58, 0x07E0,
    0xF882, 0x06E6, 0xF9E4, 0x0349, 0xFCAB, 0x035F, 0x68CD, 0x035F, 0xFCAB, 0x0349, 0xFCCC,
};

s16 sHighPassFilterData[] = {
    0xFEDF, 0xFEDD, 0xFEDF, 0x7810, 0xFEDF, 0xFEDD, 0xFEDF, 0xFEDE, 0xFE30, 0xFE2D, 0xFE2D, 0x7342, 0xFE2D, 0xFE2D,
    0xFE30, 0xFE31, 0xFD6A, 0xFD62, 0xFD60, 0x6DC5, 0xFD60, 0xFD62, 0xFD6A, 0xFD70, 0xFCB9, 0xFCA9, 0xFCA3, 0x68CE,
    0xFCA3, 0xFCA9, 0xFCB9, 0xFCCA, 0xFC1C, 0xFC00, 0xFBF2, 0x6455, 0xFBF2, 0xFC00, 0xFC1C, 0xFC3D, 0xFB60, 0xFB2C,
    0xFB0E, 0x5ED0, 0xFB0E, 0xFB2C, 0xFB60, 0xFBA2, 0xFAB3, 0xFA56, 0xFA1E, 0x5974, 0xFA1E, 0xFA56, 0xFAB3, 0xFB2A,
    0xFA16, 0xF970, 0xF908, 0x53FA, 0xF908, 0xF970, 0xFA16, 0xFAE5, 0xF9B3, 0xF8AB, 0xF800, 0x4FA6, 0xF800, 0xF8AB,
    0xF9B3, 0xFAEE, 0xF987, 0xF777, 0xF601, 0x49B5, 0xF601, 0xF777, 0xF987, 0xFBBB, 0xFA0C, 0xF6A5, 0xF3FA, 0x466E,
    0xF3FA, 0xF6A5, 0xFA0C, 0xFD1D, 0xFB1B, 0xF638, 0xF1DB, 0x44EA, 0xF1DB, 0xF638, 0xFB1B, 0xFE91, 0xFDF3, 0xF6C1,
    0xED84, 0x446D, 0xED84, 0xF6C1, 0xFDF3, 0x0000, 0xFFDE, 0xF91E, 0xE9B6, 0x445F, 0xE9B6, 0xF91E, 0xFFDE, 0xFEFE,
    0xFCFC, 0xFFFD, 0xE4B7, 0x4358, 0xE4B7, 0xFFFD, 0xFCFC, 0xFFFD,
};

s16 D_801D3070[] = {
    0xFFD5, 0xFD34, 0xFB4B, 0x6E32, 0xFB4B, 0xFD34, 0xFFD5, 0x0275, 0x00F9, 0xFC69, 0xF8E5, 0x6391, 0xF8E5, 0xFC69,
    0x00F9, 0x0515, 0x04F3, 0xFCDE, 0xF5A6, 0x50F2, 0xF5A6, 0xFCDE, 0x04F3, 0x0A2B, 0x09C1, 0xFF2E, 0xF384, 0x44D1,
    0xF384, 0xFF2E, 0x09C1, 0x0D0B, 0x0D55, 0x02B2, 0xF2C7, 0x39A2, 0xF2C7, 0x02B2, 0x0D55, 0x0BDB, 0x0F13, 0x06C6,
    0xF2A8, 0x31A0, 0xF2A8, 0x06C6, 0x0F13, 0x07FA, 0x0F12, 0x0B5B, 0xF2AA, 0x2CFC, 0xF2AA, 0x0B5B, 0x0F12, 0x037A,
    0x0D2E, 0x1062, 0xF2EF, 0x2A6B, 0xF2EF, 0x1062, 0x0D2E, 0x0050, 0x098E, 0x1591, 0xF3C6, 0x28F3, 0xF3C6, 0x1591,
    0x098E, 0x0056, 0x04FB, 0x1A68, 0xF597, 0x27F1, 0xF597, 0x1A68, 0x04FB, 0x0474, 0x00FC, 0x1E62, 0xF8D4, 0x2726,
    0xF8D4, 0x1E62, 0x00FC, 0x0BC2, 0xFF9E, 0x20EB, 0xFE33, 0x264D, 0xFE33, 0x20EB, 0xFF9E, 0x137C, 0x02CF, 0x1CE0,
    0x05F8, 0x2009, 0x05F8, 0x1CE0, 0x02CF, 0x14A6, 0x09C3, 0x14EF, 0x0E43, 0x1761, 0x0E43, 0x14EF, 0x09C3, 0x0EB1,
    0x02C7, 0xFF33, 0xFBFE, 0x6BD8, 0xFBFE, 0xFF33, 0x02C7, 0x04F7, 0x0537, 0xFF94, 0xFA10, 0x6104, 0xFA10, 0xFF94,
    0x0537, 0x07D3, 0x09EC, 0x01C7, 0xF7C8, 0x4D8D, 0xF7C8, 0x01C7, 0x09EC, 0x0A98, 0x0D4E, 0x0512, 0xF6D5, 0x3F8A,
    0xF6D5, 0x0512, 0x0D4E, 0x095D, 0x0F14, 0x0911, 0xF678, 0x370E, 0xF678, 0x0911, 0x0F14, 0x0594, 0x0F12, 0x0DB7,
    0xF679, 0x3258, 0xF679, 0x0DB7, 0x0F12, 0x0103, 0x0CB0, 0x1247, 0xF742, 0x2E53, 0xF742, 0x1247, 0x0CB0, 0xFDC1,
    0x0919, 0x1785, 0xF877, 0x2D3B, 0xF877, 0x1785, 0x0919, 0xFD8E, 0x04C9, 0x1DBD, 0xFA8D, 0x2EBF, 0xFA8D, 0x1DBD,
    0x04C9, 0x014C, 0x00AC, 0x2251, 0xFEA0, 0x2EFC, 0xFEA0, 0x2251, 0x00AC, 0x0847, 0xFF54, 0x2006, 0x0473, 0x2878,
    0x0473, 0x2006, 0xFF54, 0x0D39, 0x01FF, 0x1B5E, 0x0B0E, 0x21C9, 0x0B0E, 0x1B5E, 0x01FF, 0x0D5D, 0x07CB, 0x14C1,
    0x11F3, 0x1A91, 0x11F3, 0x14C1, 0x07CB, 0x086E, 0x04AC, 0x0139, 0xFCD1, 0x6951, 0xFCD1, 0x0139, 0x04AC, 0x0482,
    0x0768, 0x029A, 0xFB79, 0x5D0D, 0xFB79, 0x029A, 0x0768, 0x05DC, 0x0B76, 0x0621, 0xF9F4, 0x4B99, 0xF9F4, 0x0621,
    0x0B76, 0x051C, 0x0D87, 0x0A71, 0xF959, 0x4182, 0xF959, 0x0A71, 0x0D87, 0x013C, 0x0CC5, 0x0EAD, 0xF9B8, 0x38FB,
    0xF9B8, 0x0EAD, 0x0CC5, 0xFC72, 0x0A19, 0x1319, 0xFAAC, 0x340D, 0xFAAC, 0x1319, 0x0A19, 0xF91D, 0x0634, 0x1885,
    0xFC36, 0x3373, 0xFC36, 0x1885, 0x0634, 0xF87E, 0x0151, 0x1FB9, 0xFEF7, 0x3736, 0xFEF7, 0x1FB9, 0x0151, 0xFB5E,
    0xFCE5, 0x2001, 0x0372, 0x3157, 0x0372, 0x2001, 0xFCE5, 0x0187, 0xFBDE, 0x1C38, 0x0854, 0x28FC, 0x0854, 0x1C38,
    0xFBDE, 0x05A4, 0xFE09, 0x198C, 0x0EDB, 0x253C, 0x0EDB, 0x198C, 0xFE09, 0x0605, 0x038A, 0x14CE, 0x167F, 0x20A6,
    0x167F, 0x14CE, 0x038A, 0x01A7, 0x0512, 0x030D, 0xFDAB, 0x698A, 0xFDAB, 0x030D, 0x0512, 0x0188, 0x077A, 0x0554,
    0xFCBE, 0x5F0E, 0xFCBE, 0x0554, 0x077A, 0x00CD, 0x09AA, 0x09DF, 0xFC09, 0x4DCC, 0xFC09, 0x09DF, 0x09AA, 0xFCCF,
    0x08E5, 0x0E41, 0xFC5A, 0x4262, 0xFC5A, 0x0E41, 0x08E5, 0xF7FE, 0x0609, 0x1301, 0xFD5F, 0x3D13, 0xFD5F, 0x1301,
    0x0609, 0xF46C, 0x016E, 0x18DB, 0xFF4B, 0x3D4D, 0xFF4B, 0x18DB, 0x016E, 0xF34C, 0xFC07, 0x1A4E, 0x0263, 0x361E,
    0x0263, 0x1A4E, 0xFC07, 0xF775, 0xF86F, 0x1964, 0x0616, 0x2EE0, 0x0616, 0x1964, 0xF86F, 0xFCFA, 0xF758, 0x1782,
    0x0A9A, 0x29CA, 0x0A9A, 0x1782, 0xF758, 0x00AB, 0xF8FD, 0x14E0, 0x107D, 0x2688, 0x107D, 0x14E0, 0xF8FD, 0x00B6,
    0xFD6D, 0x113B, 0x1854, 0x242C, 0x1854, 0x113B, 0xFD6D, 0xFC74, 0x03C8, 0x0472, 0xFE9A, 0x6A5C, 0xFE9A, 0x0472,
    0x03C8, 0xFDA1, 0x04E3, 0x0736, 0xFE3B, 0x5F62, 0xFE3B, 0x0736, 0x04E3, 0xFB22, 0x0472, 0x0C68, 0xFE64, 0x50E5,
    0xFE64, 0x0C68, 0x0472, 0xF5D7, 0x0139, 0x11C6, 0xFF89, 0x4ADF, 0xFF89, 0x11C6, 0x0139, 0xF1CF, 0xFC34, 0x1432,
    0x01A3, 0x3FA2, 0x01A3, 0x1432, 0xFC34, 0xF2E9, 0xF7B5, 0x146E, 0x0442, 0x3570, 0x0442, 0x146E, 0xF7B5, 0xF769,
    0xF49C, 0x13CA, 0x0786, 0x2EF3, 0x0786, 0x13CA, 0xF49C, 0xFC5F, 0xF37E, 0x123D, 0x0BB3, 0x2AC9, 0x0BB3, 0x123D,
    0xF37E, 0xFFB0, 0xF4C6, 0x0F96, 0x1111, 0x27E6, 0x1111, 0x0F96, 0xF4C6, 0xFFAC, 0xF86F, 0x0BB9, 0x17DC, 0x2581,
    0x17DC, 0x0BB9, 0xF86F, 0xFBD1, 0x0136, 0x0547, 0xFF9A, 0x6D2E, 0xFF9A, 0x0547, 0x0136, 0xFAF9, 0x00A9, 0x0876,
    0xFFC9, 0x6527, 0xFFC9, 0x0876, 0x00A9, 0xF7D8, 0xFD47, 0x0D04, 0x00FC, 0x532C, 0x00FC, 0x0D04, 0xFD47, 0xF4A3,
    0xF8CA, 0x0F21, 0x02E3, 0x4392, 0x02E3, 0x0F21, 0xF8CA, 0xF60B, 0xF4A8, 0x0FC4, 0x055B, 0x3942, 0x055B, 0x0FC4,
    0xF4A8, 0xFA33, 0xF1B7, 0x0F48, 0x0887, 0x32C7, 0x0887, 0x0F48, 0xF1B7, 0xFEFB, 0xF11A, 0x0D39, 0x0C34, 0x2D26,
    0x0C34, 0x0D39, 0xF11A, 0x0231, 0xF231, 0x0A77, 0x114C, 0x2A8A, 0x114C, 0x0A77, 0xF231, 0x024D, 0xF504, 0x06B6,
    0x18A8, 0x2A1E, 0x18A8, 0x06B6, 0xF504, 0xFED5, 0xFE28, 0x0523, 0x00A4, 0x6C1D, 0x00A4, 0x0523, 0xFE28, 0xFB5F,
    0xFC1F, 0x0799, 0x0161, 0x6036, 0x0161, 0x0799, 0xFC1F, 0xF9F1, 0xF77B, 0x0A92, 0x035C, 0x4DD4, 0x035C, 0x0A92,
    0xF77B, 0xFABD, 0xF335, 0x0BA3, 0x05F1, 0x41FF, 0x05F1, 0x0BA3, 0xF335, 0xFEC1, 0xF0FE, 0x0AA5, 0x08C8, 0x37A5,
    0x08C8, 0x0AA5, 0xF0FE, 0x0379, 0xF0A3, 0x088A, 0x0C45, 0x3123, 0x0C45, 0x088A, 0xF0A3, 0x0681, 0xF1B0, 0x0597,
    0x114C, 0x2EC3, 0x114C, 0x0597, 0xF1B0, 0x06D3, 0xF433, 0x015A, 0x18FF, 0x2FB1, 0x18FF, 0x015A, 0xF433, 0x0400,
    0xFBCD, 0x0432, 0x01A1, 0x6A64, 0x01A1, 0x0432, 0xFBCD, 0xFE75, 0xF8FA, 0x05F1, 0x02E5, 0x5F76, 0x02E5, 0x05F1,
    0xF8FA, 0xFF32, 0xF497, 0x0738, 0x0592, 0x4C74, 0x0592, 0x0738, 0xF497, 0x0323, 0xF25D, 0x0671, 0x0879, 0x3F40,
    0x0879, 0x0671, 0xF25D, 0x07A2, 0xF1F0, 0x0464, 0x0C22, 0x382B, 0x0C22, 0x0464, 0xF1F0, 0x0AA6, 0xF31C, 0x0116,
    0x1165, 0x360A, 0x1165, 0x0116, 0xF31C, 0x0B33, 0xF716, 0xFC6C, 0x16AC, 0x31CC, 0x16AC, 0xFC6C, 0xF716, 0x07DC,
    0xFAE6, 0x02AA, 0x0289, 0x690C, 0x0289, 0x02AA, 0xFAE6, 0x0257, 0xF861, 0x035A, 0x0431, 0x5CEB, 0x0431, 0x035A,
    0xF861, 0x04BE, 0xF56F, 0x02F3, 0x0799, 0x4C31, 0x0799, 0x02F3, 0xF56F, 0x0992, 0xF4D7, 0x00CF, 0x0BB4, 0x43CA,
    0x0BB4, 0x00CF, 0xF4D7, 0x0CD9, 0xF72B, 0xFD2E, 0x1005, 0x3C41, 0x1005, 0xFD2E, 0xF72B, 0x0C65, 0xFB74, 0xF89D,
    0x14A7, 0x361D, 0x14A7, 0xF89D, 0xFB74, 0x08B3, 0xFBA1, 0x00CA, 0x0365, 0x6A00, 0x0365, 0x00CA, 0xFBA1, 0x04E1,
    0xF9F7, 0x006C, 0x0598, 0x6023, 0x0598, 0x006C, 0xF9F7, 0x07C0, 0xF975, 0xFE28, 0x09E1, 0x5075, 0x09E1, 0xFE28,
    0xF975, 0x0AFE, 0xFBF0, 0xFA6E, 0x0E4D, 0x45D5, 0x0E4D, 0xFA6E, 0xFBF0, 0x0A4A, 0x0069, 0xF5BE, 0x1307, 0x3E48,
    0x1307, 0xF5BE, 0x0069, 0x064F, 0xFDD6, 0xFEBF, 0x042F, 0x6C27, 0x042F, 0xFEBF, 0xFDD6, 0x04A1, 0xFDCE, 0xFD40,
    0x06CA, 0x6253, 0x06CA, 0xFD40, 0xFDCE, 0x0631, 0x0049, 0xF92B, 0x0BE5, 0x5444, 0x0BE5, 0xF92B, 0x0049, 0x05B2,
    0x0480, 0xF570, 0x0F2C, 0x4243, 0x0F2C, 0xF570, 0x0480, 0x0140, 0x00DE, 0xFCDA, 0x04C5, 0x6CD6, 0x04C5, 0xFCDA,
    0x00DE, 0x0194, 0x0278, 0xFA97, 0x0774, 0x6082, 0x0774, 0xFA97, 0x0278, 0x00D0, 0x0645, 0xF6DD, 0x0B18, 0x4807,
    0x0B18, 0xF6DD, 0x0645, 0xFD0C, 0x035D, 0xFBC7, 0x04C8, 0x6500, 0x04C8, 0xFBC7, 0x035D, 0xFDC0, 0x058E, 0xF989,
    0x070C, 0x557D, 0x070C, 0xF989, 0x058E, 0xFBA3, 0x01B2, 0xFE3B, 0x01D3, 0x73D4, 0x01D3, 0xFE3B, 0x01B2, 0xFE6B,
};

s16 D_801D3700[] = {
    0x02AF, 0x0FDA, 0x19C7, 0x1D78, 0x19C7, 0x0FDA, 0x02AF, 0xF61C, 0xFAE9, 0x0AD9, 0x18C1, 0x1E41, 0x18C1, 0x0AD9,
    0xFAE9, 0xEFA7, 0xF4C7, 0x0560, 0x172B, 0x1ECE, 0x172B, 0x0560, 0xF4C7, 0xEE58, 0xEFF4, 0x0000, 0x1719, 0x2213,
    0x1719, 0x0000, 0xEFF4, 0xF061, 0xEF77, 0xFACA, 0x147A, 0x2247, 0x147A, 0xFACA, 0xEF77, 0xF6BD, 0xF0D6, 0xF60D,
    0x12C8, 0x24AD, 0x12C8, 0xF60D, 0xF0D6, 0xFC7C, 0xF400, 0xF1AC, 0x1147, 0x28C6, 0x1147, 0xF1AC, 0xF400, 0x0000,
    0xF886, 0xEDFF, 0x0F6E, 0x2E2B, 0x0F6E, 0xEDFF, 0xF886, 0x0000, 0xFD5A, 0xEB95, 0x0CE5, 0x3489, 0x0CE5, 0xEB95,
    0xFD5A, 0xFC79, 0x00F6, 0xEB7E, 0x093B, 0x3A0A, 0x093B, 0xEB7E, 0x00F6, 0xF773, 0x01FD, 0xED34, 0x04EE, 0x4046,
    0x04EE, 0xED34, 0x01FD, 0xF3B9, 0x0000, 0xEECC, 0x0000, 0x4F8F, 0x0000, 0xEECC, 0x0000, 0xF1F9, 0xFB81, 0xF56D,
    0xFA8F, 0x4DF2, 0xFA8F, 0xF56D, 0xFB81, 0xF6FA, 0xF91B, 0xF880, 0xF822, 0x4D58, 0xF822, 0xF880, 0xF91B, 0xF9E2,
    0xF3CE, 0x0542, 0x1558, 0x1BD5, 0x1558, 0x0542, 0xF3CE, 0xE970, 0xEE2A, 0x0000, 0x145B, 0x1D41, 0x145B, 0x0000,
    0xEE2A, 0xE9A5, 0xECB3, 0xFB0F, 0x117A, 0x1C51, 0x117A, 0xFB0F, 0xECB3, 0xEFC5, 0xECBC, 0xF681, 0x0FB5, 0x1D7D,
    0x0FB5, 0xF681, 0xECBC, 0xF671, 0xEE61, 0xF1FD, 0x0E5E, 0x205E, 0x0E5E, 0xF1FD, 0xEE61, 0xFC61, 0xF1B4, 0xED99,
    0x0CF2, 0x24B4, 0x0CF2, 0xED99, 0xF1B4, 0x0000, 0xF659, 0xE9CB, 0x0B04, 0x2A3A, 0x0B04, 0xE9CB, 0xF659, 0x0000,
    0xFB43, 0xE733, 0x084B, 0x30AF, 0x084B, 0xE733, 0xFB43, 0xFC5F, 0xFEF0, 0xE64B, 0x04A5, 0x3812, 0x04A5, 0xE64B,
    0xFEF0, 0xF6E9, 0x0000, 0xE731, 0x0000, 0x40DC, 0x0000, 0xE731, 0x0000, 0xF27C, 0xFE03, 0xED34, 0xFB12, 0x4046,
    0xFB12, 0xED34, 0xFE03, 0xF3B9, 0xFA30, 0xF333, 0xF6A5, 0x4011, 0xF6A5, 0xF333, 0xFA30, 0xF803, 0xF81B, 0xF605,
    0xF4A2, 0x400D, 0xF4A2, 0xF605, 0xF81B, 0xFA90, 0xEB5F, 0xFB40, 0x0FD6, 0x1929, 0x0FD6, 0xFB40, 0xEB5F, 0xEB9B,
    0xEA89, 0xF6E6, 0x0DE0, 0x1962, 0x0DE0, 0xF6E6, 0xEA89, 0xF249, 0xEAFF, 0xF26E, 0x0C86, 0x1B4F, 0x0C86, 0xF26E,
    0xEAFF, 0xF985, 0xED0B, 0xEDAB, 0x0B4D, 0x1ECF, 0x0B4D, 0xEDAB, 0xED0B, 0x0000, 0xF1C2, 0xEA2E, 0x0935, 0x21AF,
    0x0935, 0xEA2E, 0xF1C2, 0x03C5, 0xF6BB, 0xE614, 0x071D, 0x2794, 0x071D, 0xE614, 0xF6BB, 0x03CB, 0xFBC2, 0xE154,
    0x045B, 0x3172, 0x045B, 0xE154, 0xFBC2, 0x0000, 0x0000, 0xE00A, 0x0000, 0x39F5, 0x0000, 0xE00A, 0x0000, 0xF9E3,
    0x0110, 0xE64B, 0xFB5B, 0x3812, 0xFB5B, 0xE64B, 0x0110, 0xF6EA, 0xFF0A, 0xEB7E, 0xF6C5, 0x3A0A, 0xF6C5, 0xEB7E,
    0xFF0A, 0xF773, 0xFB31, 0xF18B, 0xF2BF, 0x3A53, 0xF2BF, 0xF18B, 0xFB31, 0xFB63, 0xF918, 0xF462, 0xF0E7, 0x3A85,
    0xF0E7, 0xF462, 0xF918, 0xFDC7, 0xE938, 0xF2C1, 0x0B84, 0x18A1, 0x0B84, 0xF2C1, 0xE938, 0xF7BC, 0xEA2D, 0xEDD9,
    0x0A51, 0x1B66, 0x0A51, 0xEDD9, 0xEA2D, 0x0000, 0xEEF3, 0xEAC6, 0x080F, 0x1C8A, 0x080F, 0xEAC6, 0xEEF3, 0x06C6,
    0xF424, 0xE715, 0x05FE, 0x200D, 0x05FE, 0xE715, 0xF424, 0x0A64, 0xF9D8, 0xE25E, 0x0394, 0x26C7, 0x0394, 0xE25E,
    0xF9D8, 0x0A7A, 0x0000, 0xDC77, 0x0000, 0x321F, 0x0000, 0xDC77, 0x0000, 0x06CC, 0x043E, 0xE154, 0xFBA5, 0x3172,
    0xFBA5, 0xE154, 0x043E, 0x0000, 0x04BD, 0xE733, 0xF7B5, 0x30AF, 0xF7B5, 0xE733, 0x04BD, 0xFC5F, 0x02A6, 0xEB95,
    0xF31B, 0x3489, 0xF31B, 0xEB95, 0x02A6, 0xFC79, 0xFE6A, 0xF068, 0xEE03, 0x39A7, 0xEE03, 0xF068, 0xFE6A, 0x0000,
    0xFC2D, 0xF3F5, 0xECEE, 0x37CF, 0xECEE, 0xF3F5, 0xFC2D, 0x024D, 0xED74, 0xEB39, 0x076E, 0x19CF, 0x076E, 0xEB39,
    0xED74, 0x08AA, 0xF2B6, 0xE7FC, 0x0553, 0x1BBD, 0x0553, 0xE7FC, 0xF2B6, 0x0EFD, 0xF8C3, 0xE3B2, 0x0313, 0x2043,
    0x0313, 0xE3B2, 0xF8C3, 0x127D, 0x0000, 0xDDB7, 0x0000, 0x28BE, 0x0000, 0xDDB7, 0x0000, 0x12AE, 0x0628, 0xE25E,
    0xFC6C, 0x26C7, 0xFC6C, 0xE25E, 0x0628, 0x0A7A, 0x0945, 0xE614, 0xF8E3, 0x2794, 0xF8E3, 0xE614, 0x0945, 0x03CB,
    0x09A7, 0xE9CB, 0xF4FC, 0x2A3A, 0xF4FC, 0xE9CB, 0x09A7, 0x0000, 0x077A, 0xEDFF, 0xF092, 0x2E2B, 0xF092, 0xEDFF,
    0x077A, 0x0000, 0x034E, 0xF2C7, 0xEBB5, 0x32EB, 0xEBB5, 0xF2C7, 0x034E, 0x036B, 0x00D6, 0xF54B, 0xE92F, 0x356F,
    0xE92F, 0xF54B, 0x00D6, 0x05D4, 0xF83B, 0xE4A6, 0x02CC, 0x1CCD, 0x02CC, 0xE4A6, 0xF83B, 0x1758, 0x0000, 0xDF09,
    0x0000, 0x232E, 0x0000, 0xDF09, 0x0000, 0x1AE2, 0x073D, 0xE3B2, 0xFCED, 0x2043, 0xFCED, 0xE3B2, 0x073D, 0x127D,
    0x0BDC, 0xE715, 0xFA02, 0x200D, 0xFA02, 0xE715, 0x0BDC, 0x0A64, 0x0E3E, 0xEA2E, 0xF6CB, 0x21AF, 0xF6CB, 0xEA2E,
    0x0E3E, 0x03C5, 0x0E4C, 0xED99, 0xF30E, 0x24B4, 0xF30E, 0xED99, 0x0E4C, 0x0000, 0x0C00, 0xF1AC, 0xEEB9, 0x28C6,
    0xEEB9, 0xF1AC, 0x0C00, 0x0000, 0x07D3, 0xF665, 0xE9E8, 0x2D88, 0xE9E8, 0xF665, 0x07D3, 0x0365, 0x055E, 0xF8E4,
    0xE766, 0x3009, 0xE766, 0xF8E4, 0x055E, 0x05CB, 0x07C5, 0xE4A6, 0xFD34, 0x1CCD, 0xFD34, 0xE4A6, 0x07C5, 0x1758,
    0x0D4A, 0xE7FC, 0xFAAD, 0x1BBD, 0xFAAD, 0xE7FC, 0x0D4A, 0x0EFD, 0x110D, 0xEAC6, 0xF7F1, 0x1C8A, 0xF7F1, 0xEAC6,
    0x110D, 0x06C6, 0x12F5, 0xEDAB, 0xF4B3, 0x1ECF, 0xF4B3, 0xEDAB, 0x12F5, 0x0000, 0x119F, 0xF1FD, 0xF1A2, 0x205E,
    0xF1A2, 0xF1FD, 0x119F, 0xFC61, 0x0F2A, 0xF60D, 0xED38, 0x24AD, 0xED38, 0xF60D, 0x0F2A, 0xFC7C, 0x0B7A, 0xFA92,
    0xE6E9, 0x2C00, 0xE6E9, 0xFA92, 0x0B7A, 0x0000, 0x08CD, 0xFD49, 0xE430, 0x2EB9, 0xE430, 0xFD49, 0x08CD, 0x029B,
    0x128C, 0xEB39, 0xF892, 0x19CF, 0xF892, 0xEB39, 0x128C, 0x08AA, 0x15D3, 0xEDD9, 0xF5AF, 0x1B66, 0xF5AF, 0xEDD9,
    0x15D3, 0x0000, 0x1501, 0xF26E, 0xF37A, 0x1B4F, 0xF37A, 0xF26E, 0x1501, 0xF985, 0x1344, 0xF681, 0xF04B, 0x1D7D,
    0xF04B, 0xF681, 0x1344, 0xF671, 0x1089, 0xFACA, 0xEB86, 0x2247, 0xEB86, 0xFACA, 0x1089, 0xF6BD, 0x0C92, 0x0000,
    0xE485, 0x2A2B, 0xE485, 0x0000, 0x0C92, 0xFA48, 0x0933, 0x02D5, 0xE2F0, 0x2B13, 0xE2F0, 0x02D5, 0x0933, 0xFD46,
    0x16C8, 0xF2C1, 0xF47C, 0x18A1, 0xF47C, 0xF2C1, 0x16C8, 0xF7BC, 0x1577, 0xF6E6, 0xF220, 0x1962, 0xF220, 0xF6E6,
    0x1577, 0xF249, 0x134D, 0xFB0F, 0xEE86, 0x1C51, 0xEE86, 0xFB0F, 0x134D, 0xEFC5, 0x100C, 0x0000, 0xE8E7, 0x2213,
    0xE8E7, 0x0000, 0x100C, 0xF061, 0x094F, 0x057C, 0xE5BA, 0x2419, 0xE5BA, 0x057C, 0x094F, 0xF640, 0x0617, 0x080F,
    0xE417, 0x2549, 0xE417, 0x080F, 0x0617, 0xF96C, 0x14A1, 0xFB40, 0xF02A, 0x1929, 0xF02A, 0xFB40, 0x14A1, 0xEB9B,
    0x11D6, 0x0000, 0xEBA5, 0x1D41, 0xEBA5, 0x0000, 0x11D6, 0xE9A5, 0x0B39, 0x0560, 0xE8D5, 0x1ECE, 0xE8D5, 0x0560,
    0x0B39, 0xEE58, 0x0459, 0x0AC8, 0xE54F, 0x217C, 0xE54F, 0x0AC8, 0x0459, 0xF525, 0x010B, 0x0D5C, 0xE385, 0x22EF,
    0xE385, 0x0D5C, 0x010B, 0xF8B8, 0x0C32, 0x0542, 0xEAA8, 0x1BD5, 0xEAA8, 0x0542, 0x0C32, 0xE970, 0x0517, 0x0AD9,
    0xE73F, 0x1E41, 0xE73F, 0x0AD9, 0x0517, 0xEFA7, 0xFDA3, 0x0F84, 0xE535, 0x1F3A, 0xE535, 0x0F84, 0xFDA3, 0xF897,
    0xFACB, 0x1064, 0xE609, 0x1DBA, 0xE609, 0x1064, 0xFACB, 0xFCDD, 0xFD51, 0x0FDA, 0xE639, 0x1D78, 0xE639, 0x0FDA,
    0xFD51, 0xF61C, 0xF735, 0x11A8, 0xE7C7, 0x1AA5, 0xE7C7, 0x11A8, 0xF735, 0x0000, 0xF5B4, 0x1151, 0xE97D, 0x186D,
    0xE97D, 0x1151, 0xF5B4, 0x034F, 0xF379, 0x111B, 0xEBD5, 0x153F, 0xEBD5, 0x111B, 0xF379, 0x0722, 0xF2AA, 0x10DA,
    0xECCD, 0x1407, 0xECCD, 0x10DA, 0xF2AA, 0x092E, 0xF0F2, 0x105C, 0xEED2, 0x1175, 0xEED2, 0x105C, 0xF0F2, 0x0D57,
};

// OoT func_800DDE20
f32 func_8018B0F0(f32 arg0) {
    return 256.0f * gAudioContext.audioBufferParameters.unkUpdatesPerFrameScaled / arg0;
}

// OoT func_800DDE3C
void func_8018B10C(void) {
    s32 i;

    gAudioContext.unk_3520[255] = func_8018B0F0(0.25f);
    gAudioContext.unk_3520[254] = func_8018B0F0(0.33f);
    gAudioContext.unk_3520[253] = func_8018B0F0(0.5f);
    gAudioContext.unk_3520[252] = func_8018B0F0(0.66f);
    gAudioContext.unk_3520[251] = func_8018B0F0(0.75f);

    for (i = 128; i < 251; i++) {
        gAudioContext.unk_3520[i] = func_8018B0F0(251 - i);
    }

    for (i = 16; i < 128; i++) {
        gAudioContext.unk_3520[i] = func_8018B0F0(4 * (143 - i));
    }

    for (i = 1; i < 16; i++) {
        gAudioContext.unk_3520[i] = func_8018B0F0(60 * (23 - i));
    }

    gAudioContext.unk_3520[0] = 0.0f;
}

void AudioHeap_ResetLoadStatus(void) {
    s32 i;

    for (i = 0; i < 0x30; i++) {
        if (gAudioContext.bankLoadStatus[i] != 5) {
            gAudioContext.bankLoadStatus[i] = 0;
        }
    }

    for (i = 0; i < 0x30; i++) {
        if (gAudioContext.sampleBankLoadStatus[i] != 5) {
            gAudioContext.sampleBankLoadStatus[i] = 0;
        }
    }

    for (i = 0; i < 0x80; i++) {
        if (gAudioContext.seqLoadStatus[i] != 5) {
            gAudioContext.seqLoadStatus[i] = 0;
        }
    }
}

void AudioHeap_DiscardBank(s32 bankId) {
    s32 i;

    for (i = 0; i < gAudioContext.numNotes; i++) {
        Note* note = &gAudioContext.notes[i];

        if (note->playbackState.bankId == bankId) {
            if (note->playbackState.unk_04 == 0 && note->playbackState.priority != 0) {
                note->playbackState.parentLayer->enabled = false;
                note->playbackState.parentLayer->finished = true;
            }
            Audio_NoteDisable(note);
            Audio_AudioListRemove(&note->listItem);
            Audio_AudioListPushBack(&gAudioContext.noteFreeLists.disabled, &note->listItem);
        }
    }
}

// OoT func_800DE12C
void func_8018B3FC(s32 bankId) {
    s32 i;

    for (i = 0; i < gAudioContext.numNotes; i++) {
        Note* note = &gAudioContext.notes[i];
        NotePlaybackState* state = &note->playbackState;

        if (state->bankId == bankId) {
            if (state->priority != 0) {
                state->priority = 1;
                state->adsr.fadeOutVel = gAudioContext.audioBufferParameters.updatesPerFrameInv;
                state->adsr.action.s.release = true;
            }
        }
    }
}

void AudioHeap_DiscardSequence(s32 seqId) {
    s32 i;

    for (i = 0; i < gAudioContext.audioBufferParameters.numSequencePlayers; i++) {
        if (gAudioContext.seqPlayers[i].enabled && gAudioContext.seqPlayers[i].seqId == seqId) {
            Audio_SequencePlayerDisable(&gAudioContext.seqPlayers[i]);
        }
    }
}

// OoT func_800DE238
void* AudioHeap_WritebackDCache(void* mem, u32 size) {
    Audio_osWritebackDCache(mem, size);
    if (mem) {}
    return (void*)((u32)mem + 0x20000000);
}

// OoT func_800DE258
void* AudioHeap_AllocZeroedMaybeExternal(AudioAllocPool* pool, u32 size) {
    void* ret = NULL;

    if (gAudioContext.externalPool.start != 0) {
        ret = AudioHeap_AllocZeroed(&gAudioContext.externalPool, size);
    }
    if (ret == NULL) {
        ret = AudioHeap_AllocZeroed(pool, size);
    }
    return ret;
}

// OoT func_800DE2B0
void* AudioHeap_AllocMaybeExternal(AudioAllocPool* pool, u32 size) {
    void* ret = NULL;

    if (gAudioContext.externalPool.start != NULL) {
        ret = AudioHeap_Alloc(&gAudioContext.externalPool, size);
    }
    if (ret == NULL) {
        ret = AudioHeap_Alloc(pool, size);
    }
    return ret;
}

void* AudioHeap_AllocDmaMemory(AudioAllocPool* pool, u32 size) {
    void* ret;

    ret = AudioHeap_Alloc(pool, size);
    if (ret != NULL) {
        ret = AudioHeap_WritebackDCache(ret, size);
    }
    return ret;
}

void* AudioHeap_AllocDmaMemoryZeroed(AudioAllocPool* pool, u32 size) {
    void* ret;

    ret = AudioHeap_AllocZeroed(pool, size);
    if (ret != NULL) {
        ret = AudioHeap_WritebackDCache(ret, size);
    }
    return ret;
}

void* AudioHeap_AllocZeroed(AudioAllocPool* pool, u32 size) {
    u8* ret = AudioHeap_Alloc(pool, size);
    u8* ptr;

    if (ret != NULL) {
        for (ptr = ret; ptr < pool->cur; ptr++) {
            *ptr = 0; 
        }
    }

    return ret;
}

// New MM Function
void* func_8018B69C(AudioAllocPool* pool, u32 size) {
    u8* sp1C = pool->cur;
    void* ret = AudioHeap_Alloc(pool, size);

    if (ret != NULL) {
        pool->cur = sp1C;
        pool->count--;
    }
    return ret;
}

void* AudioHeap_Alloc(AudioAllocPool* pool, u32 size) {
    u32 aligned = ALIGN16(size);
    u8* ret = pool->cur;

    if (pool->start + pool->size >= pool->cur + aligned) {
        pool->cur += aligned;
    } else {
        return NULL;
    }
    pool->count++;
    return ret;
}

void AudioHeap_AllocPoolInit(AudioAllocPool* pool, void* mem, u32 size) {
    pool->cur = pool->start = (u8*)ALIGN16((u32)mem);
    pool->size = size - ((u32)mem & 0xF);
    pool->count = 0;
}

void AudioHeap_PersistentCacheClear(AudioPersistentCache* persistent) {
    persistent->pool.count = 0;
    persistent->numEntries = 0;
    persistent->pool.cur = persistent->pool.start;
}

void AudioHeap_TemporaryCacheClear(AudioTemporaryCache* temporary) {
    temporary->pool.count = 0;
    temporary->pool.cur = temporary->pool.start;
    temporary->nextSide = 0;
    temporary->entries[0].ptr = temporary->pool.start;
    temporary->entries[1].ptr = temporary->pool.start + temporary->pool.size;
    temporary->entries[0].id = -1;
    temporary->entries[1].id = -1;
}

// OoT func_800DE4A0
void AudioHeap_ResetPool(AudioAllocPool* pool) {
    pool->count = 0;
    pool->cur = pool->start;
}

// OoT func_800DE4B0
void AudioHeap_PopCache(s32 tableType) {
    AudioCache* loadedPool;
    AudioAllocPool* persistentPool;
    AudioPersistentCache* persistent;
    void* entryPtr;
    u8* table;

    switch (tableType) {
        case SEQUENCE_TABLE:
            loadedPool = &gAudioContext.seqCache;
            table = gAudioContext.seqLoadStatus;
            break;
        case BANK_TABLE:
            loadedPool = &gAudioContext.bankCache;
            table = gAudioContext.bankLoadStatus;
            break;
        case SAMPLE_TABLE:
            loadedPool = &gAudioContext.sampleBankCache;
            table = gAudioContext.sampleBankLoadStatus;
            break;
    }

    persistent = &loadedPool->persistent;
    persistentPool = &persistent->pool;

    if (persistent->numEntries == 0) {
        return;
    }

    entryPtr = persistent->entries[persistent->numEntries - 1].ptr;
    persistentPool->cur = entryPtr;
    persistentPool->count--;

    if (tableType == SAMPLE_TABLE) {
        AudioHeap_DiscardSampleBank(persistent->entries[persistent->numEntries - 1].id);
    }
    if (tableType == BANK_TABLE) {
        AudioHeap_DiscardBank(persistent->entries[persistent->numEntries - 1].id);
    }
    table[persistent->entries[persistent->numEntries - 1].id] = 0;
    persistent->numEntries--;
}

void AudioHeap_InitMainPools(s32 initPoolSize) {
    AudioHeap_AllocPoolInit(&gAudioContext.audioInitPool, gAudioContext.audioHeap, initPoolSize);
    AudioHeap_AllocPoolInit(&gAudioContext.audioSessionPool, gAudioContext.audioHeap + initPoolSize,
                             gAudioContext.audioHeapSize - initPoolSize);
    gAudioContext.externalPool.start = NULL;
}

void AudioHeap_SessionPoolsInit(AudioPoolSplit4* split) {
    gAudioContext.audioSessionPool.cur = gAudioContext.audioSessionPool.start;
    AudioHeap_AllocPoolInit(&gAudioContext.notesAndBuffersPool,
                             AudioHeap_Alloc(&gAudioContext.audioSessionPool, split->wantSeq), split->wantSeq);
    AudioHeap_AllocPoolInit(&gAudioContext.cachePool,
                             AudioHeap_Alloc(&gAudioContext.audioSessionPool, split->wantCustom), split->wantCustom);
}

void AudioHeap_CachePoolInit(AudioPoolSplit2* split) {
    gAudioContext.cachePool.cur = gAudioContext.cachePool.start;
    AudioHeap_AllocPoolInit(&gAudioContext.persistentCommonPool,
                             AudioHeap_Alloc(&gAudioContext.cachePool, split->wantPersistent), split->wantPersistent);
    AudioHeap_AllocPoolInit(&gAudioContext.temporaryCommonPool,
                             AudioHeap_Alloc(&gAudioContext.cachePool, split->wantTemporary), split->wantTemporary);
}

void AudioHeap_PersistentCachesInit(AudioPoolSplit3* split) {
    gAudioContext.persistentCommonPool.cur = gAudioContext.persistentCommonPool.start;
    AudioHeap_AllocPoolInit(&gAudioContext.seqCache.persistent.pool,
                             AudioHeap_Alloc(&gAudioContext.persistentCommonPool, split->wantSeq), split->wantSeq);
    AudioHeap_AllocPoolInit(&gAudioContext.bankCache.persistent.pool,
                             AudioHeap_Alloc(&gAudioContext.persistentCommonPool, split->wantBank), split->wantBank);
    AudioHeap_AllocPoolInit(&gAudioContext.sampleBankCache.persistent.pool,
                             AudioHeap_Alloc(&gAudioContext.persistentCommonPool, split->wantSample), split->wantSample);
    AudioHeap_PersistentCacheClear(&gAudioContext.seqCache.persistent);
    AudioHeap_PersistentCacheClear(&gAudioContext.bankCache.persistent);
    AudioHeap_PersistentCacheClear(&gAudioContext.sampleBankCache.persistent);
}

void AudioHeap_TemporaryCachesInit(AudioPoolSplit3* split) {
    gAudioContext.temporaryCommonPool.cur = gAudioContext.temporaryCommonPool.start;
    AudioHeap_AllocPoolInit(&gAudioContext.seqCache.temporary.pool,
                             AudioHeap_Alloc(&gAudioContext.temporaryCommonPool, split->wantSeq), split->wantSeq);
    AudioHeap_AllocPoolInit(&gAudioContext.bankCache.temporary.pool,
                             AudioHeap_Alloc(&gAudioContext.temporaryCommonPool, split->wantBank), split->wantBank);
    AudioHeap_AllocPoolInit(&gAudioContext.sampleBankCache.temporary.pool,
                             AudioHeap_Alloc(&gAudioContext.temporaryCommonPool, split->wantSample), split->wantSample);
    AudioHeap_TemporaryCacheClear(&gAudioContext.seqCache.temporary);
    AudioHeap_TemporaryCacheClear(&gAudioContext.bankCache.temporary);
    AudioHeap_TemporaryCacheClear(&gAudioContext.sampleBankCache.temporary);
}

void* AudioHeap_AllocCached(s32 tableType, s32 size, s32 where, s32 id) {
    AudioCache* loadedPool;
    AudioTemporaryCache* tp;
    AudioAllocPool* pool;
    void* mem;
    void* ret;
    u8 firstVal;
    u8 secondVal;
    s32 i;
    u8* table;
    s32 side;

    switch (tableType) {
        case SEQUENCE_TABLE:
            loadedPool = &gAudioContext.seqCache;
            table = gAudioContext.seqLoadStatus;
            break;
        case BANK_TABLE:
            loadedPool = &gAudioContext.bankCache;
            table = gAudioContext.bankLoadStatus;
            break;
        case SAMPLE_TABLE:
            loadedPool = &gAudioContext.sampleBankCache;
            table = gAudioContext.sampleBankLoadStatus;
            break;
    }

    if (where == 0) {
        tp = &loadedPool->temporary;
        pool = &tp->pool;

        if (pool->size < size) {
            return NULL;
        }

        firstVal = (tp->entries[0].id == -1) ? 0 : table[tp->entries[0].id];
        secondVal = (tp->entries[1].id == -1) ? 0 : table[tp->entries[1].id];

        if (tableType == BANK_TABLE) {
            if (firstVal == 4) {
                for (i = 0; i < gAudioContext.numNotes; i++) {
                    if (gAudioContext.notes[i].playbackState.bankId == tp->entries[0].id &&
                        gAudioContext.notes[i].noteSubEu.bitField0.s.enabled != 0) {
                        break;
                    }
                }

                if (i == gAudioContext.numNotes) {
                    AudioLoad_SetBankLoadStatus(tp->entries[0].id, 3);
                    firstVal = 3;
                }
            }

            if (secondVal == 4) {
                for (i = 0; i < gAudioContext.numNotes; i++) {
                    if (gAudioContext.notes[i].playbackState.bankId == tp->entries[1].id &&
                        gAudioContext.notes[i].noteSubEu.bitField0.s.enabled != 0) {
                        break;
                    }
                }

                if (i == gAudioContext.numNotes) {
                    AudioLoad_SetBankLoadStatus(tp->entries[1].id, 3);
                    secondVal = 3;
                }
            }
        }

        if (firstVal == 0) {
            tp->nextSide = 0;
        } else if (secondVal == 0) {
            tp->nextSide = 1;
        } else if (firstVal == 3 && secondVal == 3) {
            // Use the opposite side from last time.
        } else if (firstVal == 3) {
            tp->nextSide = 0;
        } else if (secondVal == 3) {
            tp->nextSide = 1;
        } else {
            // Check if there is a side which isn't in active use, if so, evict that one.
            if (tableType == SEQUENCE_TABLE) {
                if (firstVal == 2) {
                    for (i = 0; i < gAudioContext.audioBufferParameters.numSequencePlayers; i++) {
                        if (gAudioContext.seqPlayers[i].enabled != 0 &&
                            gAudioContext.seqPlayers[i].seqId == tp->entries[0].id) {
                            break;
                        }
                    }

                    if (i == gAudioContext.audioBufferParameters.numSequencePlayers) {
                        tp->nextSide = 0;
                        goto done;
                    }
                }

                if (secondVal == 2) {
                    for (i = 0; i < gAudioContext.audioBufferParameters.numSequencePlayers; i++) {
                        if (gAudioContext.seqPlayers[i].enabled != 0 &&
                            gAudioContext.seqPlayers[i].seqId == tp->entries[1].id) {
                            break;
                        }
                    }

                    if (i == gAudioContext.audioBufferParameters.numSequencePlayers) {
                        tp->nextSide = 1;
                        goto done;
                    }
                }
            } else if (tableType == BANK_TABLE) {
                if (firstVal == 2) {
                    for (i = 0; i < gAudioContext.numNotes; i++) {
                        if (gAudioContext.notes[i].playbackState.bankId == tp->entries[0].id &&
                            gAudioContext.notes[i].noteSubEu.bitField0.s.enabled != 0) {
                            break;
                        }
                    }
                    if (i == gAudioContext.numNotes) {
                        tp->nextSide = 0;
                        goto done;
                    }
                }

                if (secondVal == 2) {
                    for (i = 0; i < gAudioContext.numNotes; i++) {
                        if (gAudioContext.notes[i].playbackState.bankId == tp->entries[1].id &&
                            gAudioContext.notes[i].noteSubEu.bitField0.s.enabled != 0) {
                            break;
                        }
                    }
                    if (i == gAudioContext.numNotes) {
                        tp->nextSide = 1;
                        goto done;
                    }
                }
            }

            // No such luck. Evict the side that wasn't chosen last time, except
            // if it is being loaded into.
            if (tp->nextSide == 0) {
                if (firstVal == 1) {
                    if (secondVal == 1) {
                        goto fail;
                    }
                    tp->nextSide = 1;
                }
            } else {
                if (secondVal == 1) {
                    if (firstVal == 1) {
                        goto fail;
                    }
                    tp->nextSide = 0;
                }
            }

            if (0) {
            fail:
                // Both sides are being loaded into.
                return NULL;
            }
        }
    done:

        side = tp->nextSide;

        if (tp->entries[side].id != -1) {
            if (tableType == SAMPLE_TABLE) {
                AudioHeap_DiscardSampleBank(tp->entries[side].id);
            }
            table[tp->entries[side].id] = 0;
            if (tableType == BANK_TABLE) {
                AudioHeap_DiscardBank(tp->entries[side].id);
            }
        }

        switch (side) {
            case 0:
                tp->entries[0].ptr = pool->start;
                tp->entries[0].id = id;
                tp->entries[0].size = size;
                pool->cur = pool->start + size;

                if (tp->entries[1].id != -1 && tp->entries[1].ptr < pool->cur) {
                    if (tableType == SAMPLE_TABLE) {
                        AudioHeap_DiscardSampleBank(tp->entries[1].id);
                    }

                    table[tp->entries[1].id] = 0;
                    switch (tableType) {
                        case SEQUENCE_TABLE:
                            AudioHeap_DiscardSequence((s32)tp->entries[1].id);
                            break;
                        case BANK_TABLE:
                            AudioHeap_DiscardBank((s32)tp->entries[1].id);
                            break;
                    }

                    tp->entries[1].id = -1;
                    tp->entries[1].ptr = pool->start + pool->size;
                }

                ret = tp->entries[0].ptr;
                break;

            case 1:
                tp->entries[1].ptr = (u8*)((u32)(pool->start + pool->size - size) & ~0xF);
                tp->entries[1].id = id;
                tp->entries[1].size = size;
                if (tp->entries[0].id != -1 && tp->entries[1].ptr < pool->cur) {
                    if (tableType == SAMPLE_TABLE) {
                        AudioHeap_DiscardSampleBank(tp->entries[0].id);
                    }

                    table[tp->entries[0].id] = 0;
                    switch (tableType) {
                        case SEQUENCE_TABLE:
                            AudioHeap_DiscardSequence(tp->entries[0].id);
                            break;
                        case BANK_TABLE:
                            AudioHeap_DiscardBank(tp->entries[0].id);
                            break;
                    }

                    tp->entries[0].id = -1;
                    pool->cur = pool->start;
                }
                ret = tp->entries[1].ptr;
                break;

            default:
                return NULL;
        }

        tp->nextSide ^= 1;
        return ret;
    }

    mem = AudioHeap_Alloc(&loadedPool->persistent.pool, size);
    loadedPool->persistent.entries[loadedPool->persistent.numEntries].ptr = mem;

    if (mem == NULL) {
        switch (where) {
            case 2:
                return AudioHeap_AllocCached(tableType, size, 0, id);

            case 0:
            case 1:
                return NULL;
        }
    }

    loadedPool->persistent.entries[loadedPool->persistent.numEntries].id = id;
    loadedPool->persistent.entries[loadedPool->persistent.numEntries].size = size;
    return loadedPool->persistent.entries[loadedPool->persistent.numEntries++].ptr;
}

// OoT func_800DF074
void* AudioHeap_SearchCaches(s32 tableType, s32 where, s32 bankId) {
    // Search order is based on 'where':
    // 0: permanent, temporary, persistent
    // 1: permanent, persistent
    // 2: permanent, persistent, temporary
    // 3: permanent
    void* ret;

    ret = AudioHeap_SearchPermanentCache(tableType, bankId);
    if (ret != NULL) {
        return ret;
    }
    if (where == 3) {
        return NULL;
    }
    return AudioHeap_SearchRegularCaches(tableType, where, bankId);
}

// OoT func_800DF0CC
void* AudioHeap_SearchRegularCaches(s32 tableType, s32 where, s32 bankId) {
    u32 i;
    AudioCache* loadedPool;
    AudioTemporaryCache* temporary;
    AudioPersistentCache* persistent;

    switch (tableType) {
        case SEQUENCE_TABLE:
            loadedPool = &gAudioContext.seqCache;
            break;
        case BANK_TABLE:
            loadedPool = &gAudioContext.bankCache;
            break;
        case SAMPLE_TABLE:
            loadedPool = &gAudioContext.sampleBankCache;
            break;
    }

    temporary = &loadedPool->temporary;
    if (where == 0) {
        if (temporary->entries[0].id == bankId) {
            temporary->nextSide = 1;
            return temporary->entries[0].ptr;
        } else if (temporary->entries[1].id == bankId) {
            temporary->nextSide = 0;
            return temporary->entries[1].ptr;
        } else {
            return NULL;
        }
    }

    persistent = &loadedPool->persistent;
    for (i = 0; i < persistent->numEntries; i++) {
        if (persistent->entries[i].id == bankId) {
            return persistent->entries[i].ptr;
        }
    }

    if (where == 2) {
        return AudioHeap_SearchCaches(tableType, 0, bankId);
    }
    return NULL;
}

// OoT func_800DF1D8
void func_8018C4E4(f32 arg0, f32 arg1, u16* arg2) {
    s32 i;
    f32 tmp[16];

    tmp[0] = (f32)(arg1 * 262159.0f);
    tmp[8] = (f32)(arg0 * 262159.0f);
    tmp[1] = (f32)((arg1 * arg0) * 262159.0f);
    tmp[9] = (f32)(((arg0 * arg0) + arg1) * 262159.0f);

    for (i = 2; i < 8; i++) {
        //! @bug value was probably meant to be stored to tmp[i] and tmp[8 + i]
        arg2[i] = arg1 * tmp[i - 2] + arg0 * tmp[i - 1];
        arg2[8 + i] = arg1 * tmp[6 + i] + arg0 * tmp[7 + i];
    }

    for (i = 0; i < 16; i++) {
        arg2[i] = tmp[i];
    }
}

// OoT func_800DF5AC
void AudioHeap_ClearFilter(s16* filter) {
    s32 i;

    for (i = 0; i < 8; i++) {
        filter[i] = 0;
    }
}
// OoT func_800DF5DC
void AudioHeap_LoadLowPassFilter(s16* filter, s32 cutoff) {
    s32 i;
    s16* ptr = &sLowPassFilterData[8 * cutoff];

    for (i = 0; i < 8; i++) {
        filter[i] = ptr[i];
    }
}
// OoT func_800DF630
void AudioHeap_LoadHighPassFilter(s16* filter, s32 cutoff) {
    s32 i;
    s16* ptr = &sHighPassFilterData[8 * (cutoff - 1)];

    for (i = 0; i < 8; i++) {
        filter[i] = ptr[i];
    }
}

// OoT func_800DF688
#ifdef NON_EQUIVALENT
void AudioHeap_LoadFilter(s16* filter, s32 lowPassCutoff, s32 highPassCutoff) {
    s32 i;

    if (lowPassCutoff == 0 && highPassCutoff == 0) {
        // Identity filter
        AudioHeap_LoadLowPassFilter(filter, 0);
    } else if (highPassCutoff == 0) {
        AudioHeap_LoadLowPassFilter(filter, lowPassCutoff);
    } else if (lowPassCutoff == 0) {
        AudioHeap_LoadHighPassFilter(filter, highPassCutoff);
    } else {
        s16* ptr1;
        s16* ptr2;

        if (lowPassCutoff < highPassCutoff) {
            ptr1 = &D_801D3070[8 * highPassCutoff];
            ptr2 = &D_801D3070[8 * (lowPassCutoff - 1)];
            for (i = 0; i < 8; i++) {
                filter[i] = (ptr1[i] + ptr2[i]) / 2;
            }
        } else if (highPassCutoff < lowPassCutoff) {
            ptr1 = &D_801D3700[8 * lowPassCutoff];
            ptr2 = &D_801D3700[8 * (highPassCutoff - 1)];
            for (i = 0; i < 8; i++) {
                filter[i] = (ptr1[i] + ptr2[i]) / 2;
            }
        }

    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/AudioHeap_LoadFilter.s")
#endif

// OoT func_800DF7BC
void AudioHeap_UpdateReverb(SynthesisReverb* reverb) {
}

// OoT func_800DF7C4
void AudioHeap_UpdateReverbs(void) {
    s32 count;
    s32 i;
    s32 j;

    if (gAudioContext.audioBufferParameters.specUnk4 == 2) {
        count = 2;
    } else {
        count = 1;
    }

    for (i = 0; i < gAudioContext.numSynthesisReverbs; i++) {
        for (j = 0; j < count; j++) {
            AudioHeap_UpdateReverb(&gAudioContext.synthesisReverbs[i]);
        }
    }
}

// OoT func_800DF888
void func_8018CC3C(void) {
    s32 ind;
    s32 i;

    ind = gAudioContext.curAIBufIdx;
    gAudioContext.aiBufLengths[ind] = gAudioContext.audioBufferParameters.minAiBufferLength;

    for (i = 0; i < 0x580; i++) {
        gAudioContext.aiBuffers[ind][i] = 0;
    }
}

s32 AudioHeap_ResetStep(void) {
    s32 i;
    s32 j;
    s32 sp24;

    if (gAudioContext.audioBufferParameters.specUnk4 == 2) {
        sp24 = 2;
    } else {
        sp24 = 1;
    }

    switch (gAudioContext.resetStatus) {
        case 5:
            for (i = 0; i < gAudioContext.audioBufferParameters.numSequencePlayers; i++) {
                Audio_SequencePlayerDisableAsFinished(&gAudioContext.seqPlayers[i]);
            }
            gAudioContext.audioResetFadeOutFramesLeft = 2 / sp24;
            gAudioContext.resetStatus--;
            break;

        case 4:
            if (gAudioContext.audioResetFadeOutFramesLeft != 0) {
                gAudioContext.audioResetFadeOutFramesLeft--;
                AudioHeap_UpdateReverbs();
            } else {
                for (i = 0; i < gAudioContext.numNotes; i++) {
                    if (gAudioContext.notes[i].noteSubEu.bitField0.s.enabled &&
                        gAudioContext.notes[i].playbackState.adsr.action.s.state != ADSR_STATE_DISABLED) {
                        gAudioContext.notes[i].playbackState.adsr.fadeOutVel =
                            gAudioContext.audioBufferParameters.updatesPerFrameInv;
                        gAudioContext.notes[i].playbackState.adsr.action.s.release = true;
                    }
                }
                gAudioContext.audioResetFadeOutFramesLeft = 8 / sp24;
                gAudioContext.resetStatus--;
            }
            break;

        case 3:
            if (gAudioContext.audioResetFadeOutFramesLeft != 0) {
                gAudioContext.audioResetFadeOutFramesLeft--;
                AudioHeap_UpdateReverbs();
            } else {
                gAudioContext.audioResetFadeOutFramesLeft = 2 / sp24;
                gAudioContext.resetStatus--;
            }
            break;

        case 2:
            func_8018CC3C();
            if (gAudioContext.audioResetFadeOutFramesLeft != 0) {
                gAudioContext.audioResetFadeOutFramesLeft--;
            } else {
                gAudioContext.resetStatus--;
                AudioHeap_DiscardSampleCaches();
                AudioHeap_DiscardSampleBanks();
            }
            break;

        case 1:
            AudioHeap_Init();
            gAudioContext.resetStatus = 0;
            for (i = 0; i < 3; i++) {
                gAudioContext.aiBufLengths[i] = gAudioContext.audioBufferParameters.maxAiBufferLength;
                for (j = 0; j < 0x580; j++) {
                    gAudioContext.aiBuffers[i][j] = 0;
                }
            }
            break;
    }

    if (gAudioContext.resetStatus < 3) {
        return 0;
    }

    return 1;
}

#ifdef NON_EQUIVALENT
void AudioHeap_Init(void) {
    s32 pad1[4];
    s16* mem;
    s32 persistentMem;
    s32 temporaryMem;
    s32 totalMem;
    s32 wantMisc;
    u32 intMask;
    s32 i;
    s32 j;
    s32 pad2;
    AudioSpec* spec;

    spec = &gAudioSpecs[gAudioContext.audioResetSpecIdToLoad];
    gAudioContext.sampleDmaCount = 0;
    gAudioContext.audioBufferParameters.frequency = spec->frequency;
    gAudioContext.audioBufferParameters.aiFrequency = osAiSetFrequency(gAudioContext.audioBufferParameters.frequency);
    gAudioContext.audioBufferParameters.samplesPerFrameTarget =
        ((gAudioContext.audioBufferParameters.frequency / gAudioContext.refreshRate) + 0xF) & 0xFFF0;
    gAudioContext.audioBufferParameters.minAiBufferLength =
        gAudioContext.audioBufferParameters.samplesPerFrameTarget - 0x10;
    gAudioContext.audioBufferParameters.maxAiBufferLength =
        gAudioContext.audioBufferParameters.samplesPerFrameTarget + 0x10;
    gAudioContext.audioBufferParameters.updatesPerFrame =
        ((gAudioContext.audioBufferParameters.samplesPerFrameTarget + 0x10) / 0xD0) + 1;
    gAudioContext.audioBufferParameters.samplesPerUpdate = (gAudioContext.audioBufferParameters.samplesPerFrameTarget /
                                                            gAudioContext.audioBufferParameters.updatesPerFrame) &
                                                           ~7;
    gAudioContext.audioBufferParameters.samplesPerUpdateMax = gAudioContext.audioBufferParameters.samplesPerUpdate + 8;
    gAudioContext.audioBufferParameters.samplesPerUpdateMin = gAudioContext.audioBufferParameters.samplesPerUpdate - 8;
    gAudioContext.audioBufferParameters.resampleRate = 32000.0f / (s32)gAudioContext.audioBufferParameters.frequency;
    gAudioContext.audioBufferParameters.unkUpdatesPerFrameScaled =
        (1.0f / 256.0f) / gAudioContext.audioBufferParameters.updatesPerFrame;
    gAudioContext.audioBufferParameters.unk_24 = gAudioContext.audioBufferParameters.updatesPerFrame * 0.25f;
    gAudioContext.audioBufferParameters.updatesPerFrameInv = 1.0f / gAudioContext.audioBufferParameters.updatesPerFrame;
    gAudioContext.sampleDmaBufSize1 = spec->unk_10;
    gAudioContext.sampleDmaBufSize2 = spec->unk_12;

    gAudioContext.numNotes = spec->numNotes;
    gAudioContext.audioBufferParameters.numSequencePlayers = spec->numSequencePlayers;
    if (gAudioContext.audioBufferParameters.numSequencePlayers > 4) {
        gAudioContext.audioBufferParameters.numSequencePlayers = 4;
    }
    gAudioContext.unk_2 = spec->unk_14;
    gAudioContext.tempoInternalToExternal = (u32)(gAudioContext.audioBufferParameters.updatesPerFrame * 2880000.0f /
                                                  gTatumsPerBeat / gAudioContext.unk_2960);

    gAudioContext.unk_2870 = gAudioContext.refreshRate;
    gAudioContext.unk_2870 *= gAudioContext.audioBufferParameters.updatesPerFrame;
    gAudioContext.unk_2870 /= gAudioContext.audioBufferParameters.aiFrequency;
    gAudioContext.unk_2870 /= gAudioContext.tempoInternalToExternal;

    gAudioContext.audioBufferParameters.specUnk4 = spec->unk_04;
    gAudioContext.audioBufferParameters.samplesPerFrameTarget *= gAudioContext.audioBufferParameters.specUnk4;
    gAudioContext.audioBufferParameters.maxAiBufferLength *= gAudioContext.audioBufferParameters.specUnk4;
    gAudioContext.audioBufferParameters.minAiBufferLength *= gAudioContext.audioBufferParameters.specUnk4;
    gAudioContext.audioBufferParameters.updatesPerFrame *= gAudioContext.audioBufferParameters.specUnk4;

    if (gAudioContext.audioBufferParameters.specUnk4 >= 2) {
        gAudioContext.audioBufferParameters.maxAiBufferLength -= 0x10;
    }

    gAudioContext.maxAudioCmds =
        gAudioContext.numNotes * 0x10 * gAudioContext.audioBufferParameters.updatesPerFrame +
        spec->numReverbs * 0x18 + 0x140;

    persistentMem = spec->persistentSeqMem + spec->persistentBankMem + spec->persistentSampleMem + 0x10;
    temporaryMem = spec->temporarySeqMem + spec->temporaryBankMem + spec->temporarySampleMem + 0x10;
    totalMem = persistentMem + temporaryMem;
    wantMisc = gAudioContext.audioSessionPool.size - totalMem - 0x100;

    if (gAudioContext.externalPool.start != NULL) {
        gAudioContext.externalPool.cur = gAudioContext.externalPool.start;
    }

    gAudioContext.sessionPoolSplit.wantSeq = wantMisc;
    gAudioContext.sessionPoolSplit.wantCustom = totalMem;
    AudioHeap_SessionPoolsInit(&gAudioContext.sessionPoolSplit);
    gAudioContext.cachePoolSplit.wantPersistent = persistentMem;
    gAudioContext.cachePoolSplit.wantTemporary = temporaryMem;
    AudioHeap_CachePoolInit(&gAudioContext.cachePoolSplit);
    gAudioContext.persistentCommonPoolSplit.wantSeq = spec->persistentSeqMem;
    gAudioContext.persistentCommonPoolSplit.wantBank = spec->persistentBankMem;
    gAudioContext.persistentCommonPoolSplit.wantSample = spec->persistentSampleMem;
    AudioHeap_PersistentCachesInit(&gAudioContext.persistentCommonPoolSplit);
    gAudioContext.temporaryCommonPoolSplit.wantSeq = spec->temporarySeqMem;
    gAudioContext.temporaryCommonPoolSplit.wantBank = spec->temporaryBankMem;
    gAudioContext.temporaryCommonPoolSplit.wantSample = spec->temporarySampleMem;
    AudioHeap_TemporaryCachesInit(&gAudioContext.temporaryCommonPoolSplit);

    AudioHeap_ResetLoadStatus();
    gAudioContext.notes =
        AudioHeap_AllocZeroed(&gAudioContext.notesAndBuffersPool, gAudioContext.numNotes * sizeof(Note));
    Audio_NoteInitAll();
    Audio_InitNoteFreeList();
    gAudioContext.noteSubsEu = AudioHeap_AllocZeroed(&gAudioContext.notesAndBuffersPool,
                                                 gAudioContext.audioBufferParameters.updatesPerFrame *
                                                     gAudioContext.numNotes * sizeof(NoteSubEu));

    for (i = 0; i != 2; i++) {
        gAudioContext.abiCmdBufs[i] =
            AudioHeap_AllocDmaMemoryZeroed(&gAudioContext.notesAndBuffersPool, gAudioContext.maxAudioCmds * sizeof(u64));
    }

    gAudioContext.unk_3520 = AudioHeap_Alloc(&gAudioContext.notesAndBuffersPool, 0x100 * sizeof(f32));
    func_8018B10C();
    for (i = 0; i < 4; i++) {
        gAudioContext.synthesisReverbs[i].useReverb = 0;
    }

    // This code below likely related to func_8018E8C8
    /*
    gAudioContext.numSynthesisReverbs = spec->numReverbs;
    for (i = 0; i < gAudioContext.numSynthesisReverbs; i++) {
        ReverbSettings* settings = &spec->reverbSettings[i];
        SynthesisReverb* reverb = &gAudioContext.synthesisReverbs[i];
        reverb->downsampleRate = settings->downsampleRate;
        reverb->windowSize = settings->windowSize * 64;
        reverb->windowSize /= reverb->downsampleRate;
        reverb->unk_0C = settings->unk_4;
        reverb->unk_0A = settings->unk_A;
        reverb->unk_14 = settings->unk_6 * 64;
        reverb->unk_16 = settings->unk_8;
        reverb->unk_18 = 0;
        reverb->leakRtl = settings->leakRtl;
        reverb->leakLtr = settings->leakLtr;
        reverb->unk_05 = settings->unk_10;
        reverb->unk_08 = settings->unk_12;
        reverb->useReverb = 8;
        reverb->leftRingBuf = AudioHeap_AllocZeroedMaybeExternal(&gAudioContext.notesAndBuffersPool, reverb->windowSize * sizeof(s16));
        reverb->rightRingBuf = AudioHeap_AllocZeroedMaybeExternal(&gAudioContext.notesAndBuffersPool, reverb->windowSize * sizeof(s16));
        reverb->nextRingBufPos = 0;
        reverb->unk_20 = 0;
        reverb->curFrame = 0;
        reverb->bufSizePerChan = reverb->windowSize;
        reverb->framesToIgnore = 2;
        reverb->resampleFlags = 1;
        reverb->sound.sample = &reverb->sample;
        reverb->sample.loop = &reverb->loop;
        reverb->sound.tuning = 1.0f;
        reverb->sample.codec = 4;
        reverb->sample.medium = MEDIUM_RAM;
        reverb->sample.size = reverb->windowSize * 2;
        reverb->sample.sampleAddr = (u8*)reverb->leftRingBuf;
        reverb->loop.start = 0;
        reverb->loop.count = 1;
        reverb->loop.end = reverb->windowSize;

        if (reverb->downsampleRate != 1) {
            reverb->unk_0E = 0x8000 / reverb->downsampleRate;
            reverb->unk_30 = AudioHeap_AllocZeroed(&gAudioContext.notesAndBuffersPool, 0x20);
            reverb->unk_34 = AudioHeap_AllocZeroed(&gAudioContext.notesAndBuffersPool, 0x20);
            reverb->unk_38 = AudioHeap_AllocZeroed(&gAudioContext.notesAndBuffersPool, 0x20);
            reverb->unk_3C = AudioHeap_AllocZeroed(&gAudioContext.notesAndBuffersPool, 0x20);
            for (j = 0; j < gAudioContext.audioBufferParameters.updatesPerFrame; j++) {
                mem = AudioHeap_AllocZeroedMaybeExternal(&gAudioContext.notesAndBuffersPool, 0x340);
                reverb->items[0][j].toDownsampleLeft = mem;
                reverb->items[0][j].toDownsampleRight = mem + 0x1A0 / sizeof(s16);
                mem = AudioHeap_AllocZeroedMaybeExternal(&gAudioContext.notesAndBuffersPool, 0x340);
                reverb->items[1][j].toDownsampleLeft = mem;
                reverb->items[1][j].toDownsampleRight = mem + 0x1A0 / sizeof(s16);
            }
        }

        if (settings->lowPassFilterCutoffLeft != 0) {
            reverb->filterLeftState = AudioHeap_AllocDmaMemoryZeroed(&gAudioContext.notesAndBuffersPool, 0x40);
            reverb->filterLeft = AudioHeap_AllocDmaMemory(&gAudioContext.notesAndBuffersPool, 8 * sizeof(s16));
            AudioHeap_LoadLowPassFilter(reverb->filterLeft, settings->lowPassFilterCutoffLeft);
        } else {
            reverb->filterLeft = NULL;
        }

        if (settings->lowPassFilterCutoffRight != 0) {
            reverb->filterRightState = AudioHeap_AllocDmaMemoryZeroed(&gAudioContext.notesAndBuffersPool, 0x40);
            reverb->filterRight = AudioHeap_AllocDmaMemory(&gAudioContext.notesAndBuffersPool, 8 * sizeof(s16));
            AudioHeap_LoadLowPassFilter(reverb->filterRight, settings->lowPassFilterCutoffRight);
        } else {
            reverb->filterRight = NULL;
        }
    }
    */

    Audio_InitSequencePlayers();
    for (j = 0; j < gAudioContext.audioBufferParameters.numSequencePlayers; j++) {
        func_8019AC10(j);
        Audio_ResetSequencePlayer(&gAudioContext.seqPlayers[j]);
    }

    AudioHeap_InitSampleCaches(spec->persistentSampleCacheMem, spec->temporarySampleCacheMem);
    AudioLoad_InitSampleDmaBuffers(gAudioContext.numNotes);
    gAudioContext.preloadSampleStackTop = 0;
    AudioLoad_InitSlowLoads();
    AudioLoad_InitScriptLoads();
    AudioLoad_InitAsyncLoads();
    gAudioContext.unk_4 = 0x1000;
    AudioLoad_LoadPermanentSamples();
    intMask = osSetIntMask(1);
    osWritebackDCacheAll();
    osSetIntMask(intMask);
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/AudioHeap_Init.s")
#endif

// OoT func_800E04E8
void* AudioHeap_SearchPermanentCache(s32 tableType, s32 id) {
    s32 i;

    for (i = 0; i < gAudioContext.permanentPool.count; i++) {
        if (gAudioContext.permanentCache[i].tableType == tableType && gAudioContext.permanentCache[i].id == id) {
            return gAudioContext.permanentCache[i].ptr;
        }
    }
    return NULL;
}

// OoT func_800E0540
void* AudioHeap_AllocPermanent(s32 tableType, s32 id, u32 size) {
    void* ret;
    s32 index;

    index = gAudioContext.permanentPool.count;

    ret = AudioHeap_Alloc(&gAudioContext.permanentPool, size);
    gAudioContext.permanentCache[index].ptr = ret;
    if (ret == NULL) {
        return NULL;
    }
    gAudioContext.permanentCache[index].tableType = tableType;
    gAudioContext.permanentCache[index].id = id;
    gAudioContext.permanentCache[index].size = size;
    //! @bug UB: missing return. "ret" is in v0 at this point, but doing an
    // explicit return uses an additional register.
    // return ret;
}

// OoT func_800E05C4
void* AudioHeap_AllocSampleCache(u32 size, s32 bankId, void* sampleAddr, s8 medium, s32 persistent) {
    SampleCacheEntry* entry;

    if (persistent == false) {
        entry = AudioHeap_AllocTemporarySampleCacheEntry(size);
    } else {
        entry = AudioHeap_AllocPersistentSampleCacheEntry(size);
    }
    if (entry != NULL) {
        //! @bug Should use sampleBankId, not bankId
        entry->sampleBankId = bankId;
        entry->sampleAddr = sampleAddr;
        entry->origMedium = medium;
        return entry->allocatedAddr;
    }
    return NULL;
}

void AudioHeap_InitSampleCaches(u32 persistentSize, u32 temporarySize) { 
    void* mem;

    mem = AudioHeap_AllocMaybeExternal(&gAudioContext.notesAndBuffersPool, persistentSize);
    if (mem == NULL) {
        gAudioContext.persistentSampleCache.pool.size = 0;
    } else {
        AudioHeap_AllocPoolInit(&gAudioContext.persistentSampleCache.pool, mem, persistentSize);
    }
    mem = AudioHeap_AllocMaybeExternal(&gAudioContext.notesAndBuffersPool, temporarySize);
    if (mem == NULL) {
        gAudioContext.temporarySampleCache.pool.size = 0;
    } else {
        AudioHeap_AllocPoolInit(&gAudioContext.temporarySampleCache.pool, mem, temporarySize);
    }
    gAudioContext.persistentSampleCache.size = 0;
    gAudioContext.temporarySampleCache.size = 0;
}

// OoT func_800E06CC
SampleCacheEntry* AudioHeap_AllocTemporarySampleCacheEntry(u32 size) {
    s32 pad2[2];
    void* mem;
    s32 pad3[2];
    u8* allocAfter;
    u8* allocBefore;
    s32 pad1;
    s32 index;
    s32 i;
    SampleCacheEntry* ret;
    AudioPreloadReq* preload;
    AudioSampleCache* pool;
    u8* start;
    u8* end;

    pool = &gAudioContext.temporarySampleCache;
    allocBefore = pool->pool.cur;
    mem = AudioHeap_Alloc(&pool->pool, size);
    if (mem == NULL) {
        // Reset the pool and try again. We still keep pointers to within the
        // pool, so we have to be careful to discard existing overlapping
        // allocations further down.
        u8* old = pool->pool.cur;

        pool->pool.cur = pool->pool.start;
        mem = AudioHeap_Alloc(&pool->pool, size);
        if (mem == NULL) {
            pool->pool.cur = old;
            return NULL;
        }
        allocBefore = pool->pool.start;
    }

    allocAfter = pool->pool.cur;

    index = -1;
    for (i = 0; i < gAudioContext.preloadSampleStackTop; i++) {
        preload = &gAudioContext.preloadSampleStack[i];
        if (preload->isFree == false) {
            start = preload->ramAddr;
            end = preload->ramAddr + preload->sample->size - 1;

            if (end < allocBefore && start < allocBefore) {
                continue;
            }
            if (end >= allocAfter && start >= allocAfter) {
                continue;
            }

            // Overlap
            preload->isFree = true;
        }
    }

    for (i = 0; i < pool->size; i++) {
        if (pool->entries[i].inUse == 0) {
            continue;
        }

        start = pool->entries[i].allocatedAddr;
        end = start + pool->entries[i].size - 1;

        if (end < allocBefore && start < allocBefore) {
            continue;
        }
        if (end >= allocAfter && start >= allocAfter) {
            continue;
        }

        // Overlap, discard existing entry.
        AudioHeap_DiscardSampleCacheEntry(&pool->entries[i]);
        pool->entries[i].inUse = 0;
        if (index == -1) {
            index = i;
        }
    }

    if (index == -1) {
        for (i = 0; i < pool->size; i++) {
            if (pool->entries[i].inUse == 0) {
                break;
            }
        }

        index = i;
        if (index == pool->size) {
            if (pool->size == 0x80) {
                return NULL;
            }
            pool->size++;
        }
    }

    ret = &pool->entries[index];
    ret->inUse = 1;
    ret->allocatedAddr = mem;
    ret->size = size;
    return ret;
}

// OoT func_800E0964
void AudioHeap_UnapplySampleCacheForBank(SampleCacheEntry* entry, s32 bankId) {
    Drum* drum;
    Instrument* inst;
    AudioBankSound* sfx;
    s32 instId;
    s32 drumId;
    s32 sfxId;

    for (instId = 0; instId < gAudioContext.ctlEntries[bankId].numInstruments; instId++) {
        inst = Audio_GetInstrumentInner(bankId, instId);
        if (inst != NULL) {
            if (inst->normalRangeLo != 0) {
                AudioHeap_UnapplySampleCache(entry, inst->lowNotesSound.sample);
            }
            if (inst->normalRangeHi != 0x7F) {
                AudioHeap_UnapplySampleCache(entry, inst->highNotesSound.sample);
            }
            AudioHeap_UnapplySampleCache(entry, inst->normalNotesSound.sample);
        }
    }

    for (drumId = 0; drumId < gAudioContext.ctlEntries[bankId].numDrums; drumId++) {
        drum = Audio_GetDrum(bankId, drumId);
        if (drum != NULL) {
            AudioHeap_UnapplySampleCache(entry, drum->sound.sample);
        }
    }

    for (sfxId = 0; sfxId < gAudioContext.ctlEntries[bankId].numSfx; sfxId++) {
        sfx = Audio_GetSfx(bankId, sfxId);
        if (sfx != NULL) {
            AudioHeap_UnapplySampleCache(entry, sfx->sample);
        }
    }
}

void AudioHeap_DiscardSampleCacheEntry(SampleCacheEntry* entry) {
    s32 numBanks;
    s32 sampleBankId1;
    s32 sampleBankId2;
    s32 bankId;

    numBanks = gAudioContext.audioBankTable->header.entryCnt;
    for (bankId = 0; bankId < numBanks; bankId++) {
        sampleBankId1 = gAudioContext.ctlEntries[bankId].sampleBankId1;
        sampleBankId2 = gAudioContext.ctlEntries[bankId].sampleBankId2;
        if (((sampleBankId1 != 0xFF) && (entry->sampleBankId == sampleBankId1)) || ((sampleBankId2 != 0xFF) && (entry->sampleBankId == sampleBankId2)) ||
            entry->sampleBankId == 0 || entry->sampleBankId == 0xFE) {
            if (AudioHeap_SearchCaches(BANK_TABLE, 2, bankId) != NULL) {
                if (1) {}
                if (AudioLoad_IsBankLoadComplete(bankId) != 0) {
                    AudioHeap_UnapplySampleCacheForBank(entry, bankId);
                }
            }
        }
    }
}

// OoT func_800E0BB4
void AudioHeap_UnapplySampleCache(SampleCacheEntry* entry, AudioBankSample* sample) {
    if (sample != NULL) {
        if (sample->sampleAddr == entry->allocatedAddr) {
            sample->sampleAddr = entry->sampleAddr;
            sample->medium = entry->origMedium;
        }
    }
}

// OoT func_800E0BF8
SampleCacheEntry* AudioHeap_AllocPersistentSampleCacheEntry(u32 size) {
    AudioSampleCache* pool;
    SampleCacheEntry* entry;
    void* mem;

    pool = &gAudioContext.persistentSampleCache;
    mem = AudioHeap_Alloc(&pool->pool, size);
    if (mem == NULL) {
        return NULL;
    }

    if (pool->size == 0x80) {
        return NULL;
    }

    entry = &pool->entries[pool->size];
    entry->inUse = true;
    entry->allocatedAddr = mem;
    entry->size = size;
    pool->size++;
    return entry;
}

// OoT func_800E0C80
void AudioHeap_DiscardSampleCacheForBank(SampleCacheEntry* entry, s32 sampleBankId1, s32 sampleBankId2, s32 bankId) {
    if ((entry->sampleBankId == sampleBankId1) || (entry->sampleBankId == sampleBankId2) || (entry->sampleBankId == 0)) {
        AudioHeap_UnapplySampleCacheForBank(entry, bankId);
    }
}

// OoT func_800E0CBC
void AudioHeap_DiscardSampleCaches(void) {
    s32 numBanks;
    s32 sampleBankId1;
    s32 sampleBankId2;
    s32 bankId;
    s32 j;

    numBanks = gAudioContext.audioBankTable->header.entryCnt;
    for (bankId = 0; bankId < numBanks; bankId++) {
        sampleBankId1 = gAudioContext.ctlEntries[bankId].sampleBankId1;
        sampleBankId2 = gAudioContext.ctlEntries[bankId].sampleBankId2;
        if ((sampleBankId1 == 0xFF) && (sampleBankId2 == 0xFF)) {
            continue;
        }
        if (AudioHeap_SearchCaches(BANK_TABLE, 3, bankId) == NULL || !AudioLoad_IsBankLoadComplete(bankId)) {
            continue;
        }

        for (j = 0; j < gAudioContext.persistentSampleCache.size; j++) {
            AudioHeap_DiscardSampleCacheForBank(&gAudioContext.persistentSampleCache.entries[j], sampleBankId1, sampleBankId2, bankId);
        }
        for (j = 0; j < gAudioContext.temporarySampleCache.size; j++) {
            AudioHeap_DiscardSampleCacheForBank(&gAudioContext.temporarySampleCache.entries[j], sampleBankId1, sampleBankId2, bankId);
        }
    }
}


typedef struct {
    u8* oldAddr;
    u8* newAddr;
    u32 size;
    u8 newMedium;
} StorageChange;

// OoT func_800E0E0C
void AudioHeap_ChangeStorage(StorageChange* change, AudioBankSample* sample) {
    if (sample != NULL && ((sample->medium == change->newMedium) || (D_801FD120 != 1)) && ((sample->medium == 0) || (D_801FD120 != 0))) {
        u8* start = change->oldAddr;
        u8* end = change->oldAddr + change->size;
        u8* pad = sample->sampleAddr;
        u8* sampleAddr = sample->sampleAddr;

        if (start <= sampleAddr && sampleAddr < end) {
            sample->sampleAddr = sampleAddr - start + change->newAddr;
            if (D_801FD120 == 0) {
                sample->medium = change->newMedium;
            } else {
                sample->medium = 0;
            }
        }
    }
}

// OoT func_800E0E6C
void AudioHeap_DiscardSampleBank(s32 sampleBankId) {
    D_801FD120 = 0;
    AudioHeap_ApplySampleBankCacheInternal(0, sampleBankId);
}

// OoT func_800E0E90
void AudioHeap_ApplySampleBankCache(s32 sampleBankId) {
    D_801FD120 = 1;
    AudioHeap_ApplySampleBankCacheInternal(1, sampleBankId);
}

// OoT func_800E0EB4
void AudioHeap_ApplySampleBankCacheInternal(s32 apply, s32 sampleBankId) {
    SampleBankTable* sampleBankTable;
    SampleBankTableEntry* entry;
    s32 numBanks;
    s32 instId;
    s32 drumId;
    s32 sfxId;
    StorageChange change;
    s32 sampleBankId1;
    s32 sampleBankId2;
    s32 bankId;
    Drum* drum;
    Instrument* inst;
    AudioBankSound* sfx;
    u8** fakematch;
    s32 pad[4];

    sampleBankTable = gAudioContext.sampleBankTable;
    numBanks = gAudioContext.audioBankTable->header.entryCnt;
    change.oldAddr = AudioHeap_SearchCaches(SAMPLE_TABLE, 2, sampleBankId);
    if (change.oldAddr == NULL) {
        return;
    }

    entry = &sampleBankTable->entries[sampleBankId];
    change.size = entry->size;
    change.newMedium = entry->medium;
    change.newAddr = entry->romAddr;

    fakematch = &change.oldAddr;
    if ((apply != false) && (apply == true)) {
        u8* temp = change.newAddr;

        change.newAddr = *fakematch; // = change.oldAddr
        change.oldAddr = temp;
    }

    for (bankId = 0; bankId < numBanks; bankId++) {
        sampleBankId1 = gAudioContext.ctlEntries[bankId].sampleBankId1;
        sampleBankId2 = gAudioContext.ctlEntries[bankId].sampleBankId2;
        if ((sampleBankId1 != 0xFF) || (sampleBankId2 != 0xFF)) {
            if (!AudioLoad_IsBankLoadComplete(bankId) || AudioHeap_SearchCaches(BANK_TABLE, 2, bankId) == NULL) {
                continue;
            }

            if (sampleBankId1 == sampleBankId) {
            } else if (sampleBankId2 == sampleBankId) {
            } else {
                continue;
            }

            for (instId = 0; instId < gAudioContext.ctlEntries[bankId].numInstruments; instId++) {
                inst = Audio_GetInstrumentInner(bankId, instId);
                if (inst != NULL) {
                    if (inst->normalRangeLo != 0) {
                        AudioHeap_ChangeStorage(&change, inst->lowNotesSound.sample);
                    }
                    if (inst->normalRangeHi != 0x7F) {
                        AudioHeap_ChangeStorage(&change, inst->highNotesSound.sample);
                    }
                    AudioHeap_ChangeStorage(&change, inst->normalNotesSound.sample);
                }
            }

            for (drumId = 0; drumId < gAudioContext.ctlEntries[bankId].numDrums; drumId++) {
                drum = Audio_GetDrum(bankId, drumId);
                if (drum != NULL) {
                    AudioHeap_ChangeStorage(&change, drum->sound.sample);
                }
            }

            for (sfxId = 0; sfxId < gAudioContext.ctlEntries[bankId].numSfx; sfxId++) {
                sfx = Audio_GetSfx(bankId, sfxId);
                if (sfx != NULL) {
                    AudioHeap_ChangeStorage(&change, sfx->sample);
                }
            }
        }
    }
}

// OoT func_800E1148
void AudioHeap_DiscardSampleBanks(void) {
    AudioCache* pool;
    AudioPersistentCache* persistent;
    AudioTemporaryCache* temporary;
    u32 i;

    pool = &gAudioContext.sampleBankCache;
    temporary = &pool->temporary;

    if (temporary->entries[0].id != -1) {
        AudioHeap_DiscardSampleBank(temporary->entries[0].id);
    }

    if (temporary->entries[1].id != -1) {
        AudioHeap_DiscardSampleBank(temporary->entries[1].id);
    }

    persistent = &pool->persistent;
    for (i = 0; i < persistent->numEntries; i++) {
        AudioHeap_DiscardSampleBank(persistent->entries[i].id);
    }
}

// New MM Function?
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018E344.s")
// ? func_8018E8C8(s32, s32, ?); // extern

// void func_8018E344(s32 id, u32 arg1, s32 arg2, s32 arg3) {
//     void* sp1C;
//     s16* temp_a0_2;
//     s16* temp_a0_4;
//     s32 temp_a0;
//     s32 temp_lo;
//     s32 temp_t1;
//     u8 temp_v0;
//     void* temp_a0_3;
//     void* temp_a0_5;
//     void* temp_v0_2;
//     void* temp_v0_3;
//     void* temp_v0_4;
//     void* temp_v1;
//     SynthesisReverb* reverb;
//     s32 phi_a0;
//     s16* phi_a0_2;
//     void* phi_v1;
//     s16* phi_a0_3;
//     void* phi_v1_2;

//     switch (arg1) {
//     case 0:
//         func_8018E8C8(id, arg2, 0);
//         return;
//     case 1:
//         if (arg2 < 4) {
//             arg2 = 4;
//         }
//         temp_a0 = arg2 << 6;
//         phi_a0 = temp_a0;
//         if (temp_a0 < 0x100) {
//             phi_a0 = 0x100;
//         }
//         temp_t1 = (id * 0x2D0) + 0x18;
//         temp_v1 = temp_t1 + &gAudioContext;
//         temp_v0 = temp_v1->unk4;
//         temp_lo = phi_a0 / temp_v0;
//         if (arg3 == 0) {
//             if (temp_v1->unk1E >= (arg2 / temp_v0)) {
//                 if ((temp_v1->unk20 >= temp_lo) || (temp_v1->unk24 >= temp_lo)) {
//                     temp_v1->unk20 = 0;
//                     temp_v1->unk24 = 0;
//                 }
//                 goto block_12;
//             }
//             // Duplicate return node #41. Try simplifying control flow for better match
//             return;
//         }
// block_12:
//         gAudioContext.unk_0006[temp_t1] = temp_lo;
//         if (((temp_v1->unk4 != 1) || (temp_v1->unk18 != 0)) && (gAudioContext.unk_0006[temp_t1].unk8 = 0x8000 / temp_v1->unk4, (temp_v1->unk30 == 0)) && (sp1C = temp_v1, temp_v1->unk30 = AudioHeap_AllocZeroed(&gAudioContext.notesAndBuffersPool, 0x20), temp_v1->unk34 = AudioHeap_AllocZeroed(&gAudioContext.notesAndBuffersPool, 0x20), temp_v1->unk38 = AudioHeap_AllocZeroed(&gAudioContext.notesAndBuffersPool, 0x20), temp_v0_2 = AudioHeap_AllocZeroed(&gAudioContext.notesAndBuffersPool, 0x20), temp_v1->unk3C = temp_v0_2, (temp_v0_2 == 0))) {
//             temp_v1->unk4 = 1;
//             return;
//         }
//     default:
//         return;
//     case 2:
//         gAudioContext.synthesisReverbs[id].unk_0C = arg2;
//         return;
//     case 3:
//         gAudioContext.synthesisReverbs[id].unk_16 = arg2;
//         return;
//     case 4:
//         gAudioContext.synthesisReverbs[id].unk_0A = arg2;
//         return;
//     case 5:
//         gAudioContext.synthesisReverbs[id].leakRtl = arg2;
//         return;
//     case 6:
//         gAudioContext.synthesisReverbs[id].leakLtr = arg2;
//         return;
//     case 7:
//         if (arg2 != 0) {
//             reverb = &gAudioContext.synthesisReverbs[id];
//             if ((arg3 != 0) || (temp_a0_2 = reverb->unk278, phi_a0_2 = temp_a0_2, phi_v1 = reverb, (temp_a0_2 == 0))) {
//                 reverb =  &gAudioContext.synthesisReverbs[id];
//                 sp1C = reverb;
//                 reverb->unk280 = AudioHeap_AllocDmaMemoryZeroed(&gAudioContext.notesAndBuffersPool, 0x40);
//                 temp_v0_3 = AudioHeap_AllocDmaMemory(&gAudioContext.notesAndBuffersPool, 0x10);
//                 temp_a0_3 = temp_v0_3;
//                 reverb->unk278 = temp_v0_3;
//                 phi_a0_2 = temp_a0_3;
//                 phi_v1 = reverb;
//             }
//             phi_v1->unk270 = phi_a0_2;
//             if (phi_a0_2 != 0) {
//                 AudioHeap_LoadLowPassFilter(phi_a0_2, arg2);
//                 return;
//             }
//             // Duplicate return node #41. Try simplifying control flow for better match
//             return;
//         }
//         reverb = &gAudioContext.synthesisReverbs[id];
//         reverb->unk270 = 0;
//         if (arg3 != 0) {
//             reverb->unk278 = 0;
//             return;
//         }
//         // Duplicate return node #41. Try simplifying control flow for better match
//         return;
//     case 8:
//         if (arg2 != 0) {
//             if ((arg3 != 0) || (reverb = &gAudioContext.synthesisReverbs[id], temp_a0_4 = reverb->unk27C, phi_a0_3 = temp_a0_4, phi_v1_2 = reverb, (temp_a0_4 == 0))) {
//                 reverb = &gAudioContext.synthesisReverbs[id];
//                 sp1C = reverb;
//                 reverb->unk284 = AudioHeap_AllocDmaMemoryZeroed(&gAudioContext.notesAndBuffersPool, 0x40);
//                 temp_v0_4 = AudioHeap_AllocDmaMemory(&gAudioContext.notesAndBuffersPool, 0x10);
//                 temp_a0_5 = temp_v0_4;
//                 reverb->unk27C = temp_v0_4;
//                 phi_a0_3 = temp_a0_5;
//                 phi_v1_2 = reverb;
//             }
//             phi_v1_2->unk274 = phi_a0_3;
//             if (phi_a0_3 != 0) {
//                 AudioHeap_LoadLowPassFilter(phi_a0_3, arg2);
//                 return;
//             }
//             // Duplicate return node #41. Try simplifying control flow for better match
//             return;
//         }
//         reverb = &gAudioContext.synthesisReverbs[id];
//         reverb->unk274 = 0;
//         if (arg3 != 0) {
//             reverb->unk27C = 0;
//             return;
//         }
//         // Duplicate return node #41. Try simplifying control flow for better match
//         return;
//     case 9:
//         reverb = &gAudioContext.synthesisReverbs[id];
//         reverb->unk19 = arg2;
//         if (arg2 == 0) {
//             reverb->unk18 = 0;
//             return;
//         }
//         reverb->unk18 = 1;
//         // Duplicate return node #41. Try simplifying control flow for better match
//         return;
//     }
// }



// New MM Function?
#pragma GLOBAL_ASM("asm/non_matchings/code/audio_heap/func_8018E8C8.s")
// ? func_8018E344(s32, ?, u16, s32); // extern

// void func_8018E8C8(s32 id, ReverbSettings* settings, s32 arg2) {
//     s32 temp_v1;
//     u16 temp_v0;
//     u8 temp_t6;
//     void* temp_s0;
//     void* temp_s0_2;
//     void* phi_s0;

//     if (arg2 != 0) {
//         temp_s0 = (id * 0x2D0) + 0x18 + &gAudioContext;
//         temp_s0->unk1E = arg1->unk2 / arg1->unk0;
//         temp_s0->unk30 = 0;
//         phi_s0 = temp_s0;
//     } else if (temp_s0_2->unk1E < (arg1->unk2 / arg1->unk0)) {
//          return
//     }
//         phi_s0->unk4 = arg1->unk0;
//         phi_s0->unk18 = 0;
//         phi_s0->unk19 = 0;
//         phi_s0->unk1A = 0;
//         phi_s0->unk1C = 0;
//         func_8018E344(id, 1, arg1->unk2, arg2);
//         phi_s0->unkC = arg1->unk4;
//         phi_s0->unkA = arg1->unkA;
//         phi_s0->unk14 = arg1->unk6 << 6;
//         phi_s0->unk16 = arg1->unk8;
//         phi_s0->unk10 = arg1->unkC;
//         phi_s0->unk12 = arg1->unkE;
//         phi_s0->unk5 = arg1->unk10;
//         phi_s0->unk1 = 8;
//         phi_s0->unk8 = arg1->unk12;
//         if (arg2 != 0) {
//             phi_s0->unk28 = AudioHeap_AllocZeroedMaybeExternal(&gAudioContext.notesAndBuffersPool, phi_s0->unk6 * 2);
//             phi_s0->unk2C = AudioHeap_AllocZeroedMaybeExternal(&gAudioContext.notesAndBuffersPool, phi_s0->unk6 * 2);
//             phi_s0->unk0 = 1;
//             phi_s0->unk20 = 0;
//             phi_s0->unk24 = 0;
//             phi_s0->unk3 = 0;
//             phi_s0->unk2 = 2;
//         }
//         temp_v0 = phi_s0->unk6;
//         temp_t6 = (phi_s0->unk290 & 0xFF8F) | 0x40;
//         phi_s0->unk290 = temp_t6;
//         phi_s0->unk290 = temp_t6 & 0xF3;
//         temp_v1 = phi_s0->unk290;
//         phi_s0->unk288 = phi_s0 + 0x290;
//         phi_s0->unk298 = phi_s0 + 0x2A0;
//         phi_s0->unk290 = (((((temp_v0 * 2) & 0xFFFFFF) ^ temp_v1) << 8) >> 8) ^ temp_v1;
//         phi_s0->unk2A0 = 0;
//         phi_s0->unk2A8 = 1;
//         phi_s0->unk2A4 = temp_v0;
//         phi_s0->unk28C = 1.0f;
//         phi_s0->unk294 = phi_s0->unk28;
//         func_8018E344(id, 7, arg1->unk14, arg2);
//         func_8018E344(id, 8, arg1->unk16, arg2);
//     }
// }


//     gAudioContext.numSynthesisReverbs = spec->numReverbs;
//     for (i = 0; i < gAudioContext.numSynthesisReverbs; i++) {
//         ReverbSettings* settings = &spec->reverbSettings[i];
//         SynthesisReverb* reverb = &gAudioContext.synthesisReverbs[i];
//         reverb->downsampleRate = settings->downsampleRate;
//         reverb->windowSize = settings->windowSize * 64;
//         reverb->windowSize /= reverb->downsampleRate;
//         reverb->unk_0C = settings->unk_4;
//         reverb->unk_0A = settings->unk_A;
//         reverb->unk_14 = settings->unk_6 * 64;
//         reverb->unk_16 = settings->unk_8;
//         reverb->unk_18 = 0;
//         reverb->leakRtl = settings->leakRtl;
//         reverb->leakLtr = settings->leakLtr;
//         reverb->unk_05 = settings->unk_10;
//         reverb->unk_08 = settings->unk_12;
//         reverb->useReverb = 8;
//         reverb->leftRingBuf = AudioHeap_AllocZeroedMaybeExternal(&gAudioContext.notesAndBuffersPool, reverb->windowSize * sizeof(s16));
//         reverb->rightRingBuf = AudioHeap_AllocZeroedMaybeExternal(&gAudioContext.notesAndBuffersPool, reverb->windowSize * sizeof(s16));
//         reverb->nextRingBufPos = 0;
//         reverb->unk_20 = 0;
//         reverb->curFrame = 0;
//         reverb->bufSizePerChan = reverb->windowSize;
//         reverb->framesToIgnore = 2;
//         reverb->resampleFlags = 1;
//         reverb->sound.sample = &reverb->sample;
//         reverb->sample.loop = &reverb->loop;
//         reverb->sound.tuning = 1.0f;
//         reverb->sample.codec = 4;
//         reverb->sample.medium = 0;
//         reverb->sample.size = reverb->windowSize * 2;
//         reverb->sample.sampleAddr = (u8*)reverb->leftRingBuf;
//         reverb->loop.start = 0;
//         reverb->loop.count = 1;
//         reverb->loop.end = reverb->windowSize;

//         if (reverb->downsampleRate != 1) {
//             reverb->unk_0E = 0x8000 / reverb->downsampleRate;
//             reverb->unk_30 = AudioHeap_AllocZeroed(&gAudioContext.notesAndBuffersPool, 0x20);
//             reverb->unk_34 = AudioHeap_AllocZeroed(&gAudioContext.notesAndBuffersPool, 0x20);
//             reverb->unk_38 = AudioHeap_AllocZeroed(&gAudioContext.notesAndBuffersPool, 0x20);
//             reverb->unk_3C = AudioHeap_AllocZeroed(&gAudioContext.notesAndBuffersPool, 0x20);
//             for (j = 0; j < gAudioContext.audioBufferParameters.updatesPerFrame; j++) {
//                 mem = AudioHeap_AllocZeroedMaybeExternal(&gAudioContext.notesAndBuffersPool, 0x340);
//                 reverb->items[0][j].toDownsampleLeft = mem;
//                 reverb->items[0][j].toDownsampleRight = mem + 0x1A0 / sizeof(s16);
//                 mem = AudioHeap_AllocZeroedMaybeExternal(&gAudioContext.notesAndBuffersPool, 0x340);
//                 reverb->items[1][j].toDownsampleLeft = mem;
//                 reverb->items[1][j].toDownsampleRight = mem + 0x1A0 / sizeof(s16);
//             }
//         }

//         if (settings->unk_14 != 0) {
//             reverb->filterLeftState = AudioHeap_AllocDmaMemoryZeroed(&gAudioContext.notesAndBuffersPool, 0x40);
//             reverb->filterLeft = AudioHeap_AllocDmaMemory(&gAudioContext.notesAndBuffersPool, 8 * sizeof(s16));
//             func_800DF5DC(reverb->filterLeft, settings->unk_14);
//         } else {
//             reverb->filterLeft = NULL;
//         }

//         if (settings->unk_16 != 0) {
//             reverb->filterRightState = AudioHeap_AllocDmaMemoryZeroed(&gAudioContext.notesAndBuffersPool, 0x40);
//             reverb->filterRight = AudioHeap_AllocDmaMemory(&gAudioContext.notesAndBuffersPool, 8 * sizeof(s16));
//             func_800DF5DC(reverb->filterRight, settings->unk_16);
//         } else {
//             reverb->filterRight = NULL;
//         }
//     }

//     Audio_InitSequencePlayers();
