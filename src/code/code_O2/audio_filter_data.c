#include "global.h"






s16 sLowPassFilterData[] = {
    0x0000, 0x0000, 0x0000, 0x7FFF, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0F0E, 0x105C, 0x112E, 0x1175, 0x112E, 0x105C, 0x0F0E, 0x0D58, 
    0x0D57, 0x10DA, 0x1333, 0x1406, 0x1333, 0x10DA, 0x0D57, 0x092F, 
    0x0A4C, 0x1151, 0x1682, 0x186C, 0x1682, 0x1151, 0x0A4C, 0x0351, 
    0x0536, 0x1064, 0x19F6, 0x1DB9, 0x19F6, 0x1064, 0x0536, 0xFCDE, 
    0xFEF7, 0x0D5D, 0x1C7C, 0x22F0, 0x1C7C, 0x0D5D, 0xFEF7, 0xF8B9, 
    0xF9EA, 0x0811, 0x1BEA, 0x254A, 0x1BEA, 0x0811, 0xF9EA, 0xF96E, 
    0xF6CF, 0x02D6, 0x1D11, 0x2B14, 0x1D11, 0x02D6, 0xF6CF, 0xFD47, 
    0xF734, 0xFD4B, 0x1BD1, 0x2EBA, 0x1BD1, 0xFD4B, 0xF734, 0x029C, 
    0xFAA3, 0xF8E5, 0x189B, 0x300A, 0x189B, 0xF8E5, 0xFAA3, 0x05CC, 
    0xFF2B, 0xF54C, 0x16D3, 0x3570, 0x16D3, 0xF54C, 0xFF2B, 0x05D6, 
    0x03D4, 0xF3F7, 0x1313, 0x37CE, 0x1313, 0xF3F7, 0x03D4, 0x024E, 
    0x06E9, 0xF463, 0x0F1A, 0x3A85, 0x0F1A, 0xF463, 0x06E9, 0xFDC8,
    0x07E7, 0xF606, 0x0B5F, 0x400D, 0x0B5F, 0xF606, 0x07E7, 0xFA91, 
    0x06E6, 0xF882, 0x07E0, 0x4D58, 0x07E0, 0xF882, 0x06E6, 0xF9E4, 
    0x0349, 0xFCAB, 0x035F, 0x68CD, 0x035F, 0xFCAB, 0x0349, 0xFCCC,
};

s16 sHighPassFilterData[] = {
    0xFEDF, 0xFEDD, 0xFEDF, 0x7810, 0xFEDF, 0xFEDD, 0xFEDF, 0xFEDE, 
    0xFE30, 0xFE2D, 0xFE2D, 0x7342, 0xFE2D, 0xFE2D, 0xFE30, 0xFE31, 
    0xFD6A, 0xFD62, 0xFD60, 0x6DC5, 0xFD60, 0xFD62, 0xFD6A, 0xFD70, 
    0xFCB9, 0xFCA9, 0xFCA3, 0x68CE, 0xFCA3, 0xFCA9, 0xFCB9, 0xFCCA, 
    0xFC1C, 0xFC00, 0xFBF2, 0x6455, 0xFBF2, 0xFC00, 0xFC1C, 0xFC3D, 
    0xFB60, 0xFB2C, 0xFB0E, 0x5ED0, 0xFB0E, 0xFB2C, 0xFB60, 0xFBA2, 
    0xFAB3, 0xFA56, 0xFA1E, 0x5974, 0xFA1E, 0xFA56, 0xFAB3, 0xFB2A, 
    0xFA16, 0xF970, 0xF908, 0x53FA, 0xF908, 0xF970, 0xFA16, 0xFAE5, 
    0xF9B3, 0xF8AB, 0xF800, 0x4FA6, 0xF800, 0xF8AB, 0xF9B3, 0xFAEE, 
    0xF987, 0xF777, 0xF601, 0x49B5, 0xF601, 0xF777, 0xF987, 0xFBBB, 
    0xFA0C, 0xF6A5, 0xF3FA, 0x466E, 0xF3FA, 0xF6A5, 0xFA0C, 0xFD1D, 
    0xFB1B, 0xF638, 0xF1DB, 0x44EA, 0xF1DB, 0xF638, 0xFB1B, 0xFE91, 
    0xFDF3, 0xF6C1, 0xED84, 0x446D, 0xED84, 0xF6C1, 0xFDF3, 0x0000, 
    0xFFDE, 0xF91E, 0xE9B6, 0x445F, 0xE9B6, 0xF91E, 0xFFDE, 0xFEFE, 
    0xFCFC, 0xFFFD, 0xE4B7, 0x4358, 0xE4B7, 0xFFFD, 0xFCFC, 0xFFFD,
};

s16 D_801D3070[] = {
    0xFFD5, 0xFD34, 0xFB4B, 0x6E32, 0xFB4B, 0xFD34, 0xFFD5, 0x0275, 
    0x00F9, 0xFC69, 0xF8E5, 0x6391, 0xF8E5, 0xFC69, 0x00F9, 0x0515, 
    0x04F3, 0xFCDE, 0xF5A6, 0x50F2, 0xF5A6, 0xFCDE, 0x04F3, 0x0A2B, 
    0x09C1, 0xFF2E, 0xF384, 0x44D1, 0xF384, 0xFF2E, 0x09C1, 0x0D0B, 
    0x0D55, 0x02B2, 0xF2C7, 0x39A2, 0xF2C7, 0x02B2, 0x0D55, 0x0BDB, 
    0x0F13, 0x06C6, 0xF2A8, 0x31A0, 0xF2A8, 0x06C6, 0x0F13, 0x07FA, 
    0x0F12, 0x0B5B, 0xF2AA, 0x2CFC, 0xF2AA, 0x0B5B, 0x0F12, 0x037A, 
    0x0D2E, 0x1062, 0xF2EF, 0x2A6B, 0xF2EF, 0x1062, 0x0D2E, 0x0050, 
    0x098E, 0x1591, 0xF3C6, 0x28F3, 0xF3C6, 0x1591, 0x098E, 0x0056, 
    0x04FB, 0x1A68, 0xF597, 0x27F1, 0xF597, 0x1A68, 0x04FB, 0x0474, 
    0x00FC, 0x1E62, 0xF8D4, 0x2726, 0xF8D4, 0x1E62, 0x00FC, 0x0BC2, 
    0xFF9E, 0x20EB, 0xFE33, 0x264D, 0xFE33, 0x20EB, 0xFF9E, 0x137C, 
    0x02CF, 0x1CE0, 0x05F8, 0x2009, 0x05F8, 0x1CE0, 0x02CF, 0x14A6, 
    0x09C3, 0x14EF, 0x0E43, 0x1761, 0x0E43, 0x14EF, 0x09C3, 0x0EB1, 
    0x02C7, 0xFF33, 0xFBFE, 0x6BD8, 0xFBFE, 0xFF33, 0x02C7, 0x04F7, 
    0x0537, 0xFF94, 0xFA10, 0x6104, 0xFA10, 0xFF94, 0x0537, 0x07D3, 
    0x09EC, 0x01C7, 0xF7C8, 0x4D8D, 0xF7C8, 0x01C7, 0x09EC, 0x0A98, 
    0x0D4E, 0x0512, 0xF6D5, 0x3F8A, 0xF6D5, 0x0512, 0x0D4E, 0x095D, 
    0x0F14, 0x0911, 0xF678, 0x370E, 0xF678, 0x0911, 0x0F14, 0x0594, 
    0x0F12, 0x0DB7, 0xF679, 0x3258, 0xF679, 0x0DB7, 0x0F12, 0x0103, 
    0x0CB0, 0x1247, 0xF742, 0x2E53, 0xF742, 0x1247, 0x0CB0, 0xFDC1, 
    0x0919, 0x1785, 0xF877, 0x2D3B, 0xF877, 0x1785, 0x0919, 0xFD8E, 
    0x04C9, 0x1DBD, 0xFA8D, 0x2EBF, 0xFA8D, 0x1DBD, 0x04C9, 0x014C, 
    0x00AC, 0x2251, 0xFEA0, 0x2EFC, 0xFEA0, 0x2251, 0x00AC, 0x0847, 
    0xFF54, 0x2006, 0x0473, 0x2878, 0x0473, 0x2006, 0xFF54, 0x0D39, 
    0x01FF, 0x1B5E, 0x0B0E, 0x21C9, 0x0B0E, 0x1B5E, 0x01FF, 0x0D5D, 
    0x07CB, 0x14C1, 0x11F3, 0x1A91, 0x11F3, 0x14C1, 0x07CB, 0x086E, 
    0x04AC, 0x0139, 0xFCD1, 0x6951, 0xFCD1, 0x0139, 0x04AC, 0x0482, 
    0x0768, 0x029A, 0xFB79, 0x5D0D, 0xFB79, 0x029A, 0x0768, 0x05DC, 
    0x0B76, 0x0621, 0xF9F4, 0x4B99, 0xF9F4, 0x0621, 0x0B76, 0x051C, 
    0x0D87, 0x0A71, 0xF959, 0x4182, 0xF959, 0x0A71, 0x0D87, 0x013C, 
    0x0CC5, 0x0EAD, 0xF9B8, 0x38FB, 0xF9B8, 0x0EAD, 0x0CC5, 0xFC72, 
    0x0A19, 0x1319, 0xFAAC, 0x340D, 0xFAAC, 0x1319, 0x0A19, 0xF91D, 
    0x0634, 0x1885, 0xFC36, 0x3373, 0xFC36, 0x1885, 0x0634, 0xF87E, 
    0x0151, 0x1FB9, 0xFEF7, 0x3736, 0xFEF7, 0x1FB9, 0x0151, 0xFB5E, 
    0xFCE5, 0x2001, 0x0372, 0x3157, 0x0372, 0x2001, 0xFCE5, 0x0187, 
    0xFBDE, 0x1C38, 0x0854, 0x28FC, 0x0854, 0x1C38, 0xFBDE, 0x05A4, 
    0xFE09, 0x198C, 0x0EDB, 0x253C, 0x0EDB, 0x198C, 0xFE09, 0x0605, 
    0x038A, 0x14CE, 0x167F, 0x20A6, 0x167F, 0x14CE, 0x038A, 0x01A7, 
    0x0512, 0x030D, 0xFDAB, 0x698A, 0xFDAB, 0x030D, 0x0512, 0x0188, 
    0x077A, 0x0554, 0xFCBE, 0x5F0E, 0xFCBE, 0x0554, 0x077A, 0x00CD, 
    0x09AA, 0x09DF, 0xFC09, 0x4DCC, 0xFC09, 0x09DF, 0x09AA, 0xFCCF, 
    0x08E5, 0x0E41, 0xFC5A, 0x4262, 0xFC5A, 0x0E41, 0x08E5, 0xF7FE, 
    0x0609, 0x1301, 0xFD5F, 0x3D13, 0xFD5F, 0x1301, 0x0609, 0xF46C, 
    0x016E, 0x18DB, 0xFF4B, 0x3D4D, 0xFF4B, 0x18DB, 0x016E, 0xF34C, 
    0xFC07, 0x1A4E, 0x0263, 0x361E, 0x0263, 0x1A4E, 0xFC07, 0xF775, 
    0xF86F, 0x1964, 0x0616, 0x2EE0, 0x0616, 0x1964, 0xF86F, 0xFCFA, 
    0xF758, 0x1782, 0x0A9A, 0x29CA, 0x0A9A, 0x1782, 0xF758, 0x00AB, 
    0xF8FD, 0x14E0, 0x107D, 0x2688, 0x107D, 0x14E0, 0xF8FD, 0x00B6, 
    0xFD6D, 0x113B, 0x1854, 0x242C, 0x1854, 0x113B, 0xFD6D, 0xFC74, 
    0x03C8, 0x0472, 0xFE9A, 0x6A5C, 0xFE9A, 0x0472, 0x03C8, 0xFDA1, 
    0x04E3, 0x0736, 0xFE3B, 0x5F62, 0xFE3B, 0x0736, 0x04E3, 0xFB22, 
    0x0472, 0x0C68, 0xFE64, 0x50E5, 0xFE64, 0x0C68, 0x0472, 0xF5D7, 
    0x0139, 0x11C6, 0xFF89, 0x4ADF, 0xFF89, 0x11C6, 0x0139, 0xF1CF, 
    0xFC34, 0x1432, 0x01A3, 0x3FA2, 0x01A3, 0x1432, 0xFC34, 0xF2E9, 
    0xF7B5, 0x146E, 0x0442, 0x3570, 0x0442, 0x146E, 0xF7B5, 0xF769, 
    0xF49C, 0x13CA, 0x0786, 0x2EF3, 0x0786, 0x13CA, 0xF49C, 0xFC5F, 
    0xF37E, 0x123D, 0x0BB3, 0x2AC9, 0x0BB3, 0x123D, 0xF37E, 0xFFB0, 
    0xF4C6, 0x0F96, 0x1111, 0x27E6, 0x1111, 0x0F96, 0xF4C6, 0xFFAC, 
    0xF86F, 0x0BB9, 0x17DC, 0x2581, 0x17DC, 0x0BB9, 0xF86F, 0xFBD1, 
    0x0136, 0x0547, 0xFF9A, 0x6D2E, 0xFF9A, 0x0547, 0x0136, 0xFAF9, 
    0x00A9, 0x0876, 0xFFC9, 0x6527, 0xFFC9, 0x0876, 0x00A9, 0xF7D8, 
    0xFD47, 0x0D04, 0x00FC, 0x532C, 0x00FC, 0x0D04, 0xFD47, 0xF4A3, 
    0xF8CA, 0x0F21, 0x02E3, 0x4392, 0x02E3, 0x0F21, 0xF8CA, 0xF60B, 
    0xF4A8, 0x0FC4, 0x055B, 0x3942, 0x055B, 0x0FC4, 0xF4A8, 0xFA33, 
    0xF1B7, 0x0F48, 0x0887, 0x32C7, 0x0887, 0x0F48, 0xF1B7, 0xFEFB, 
    0xF11A, 0x0D39, 0x0C34, 0x2D26, 0x0C34, 0x0D39, 0xF11A, 0x0231, 
    0xF231, 0x0A77, 0x114C, 0x2A8A, 0x114C, 0x0A77, 0xF231, 0x024D, 
    0xF504, 0x06B6, 0x18A8, 0x2A1E, 0x18A8, 0x06B6, 0xF504, 0xFED5, 
    0xFE28, 0x0523, 0x00A4, 0x6C1D, 0x00A4, 0x0523, 0xFE28, 0xFB5F, 
    0xFC1F, 0x0799, 0x0161, 0x6036, 0x0161, 0x0799, 0xFC1F, 0xF9F1, 
    0xF77B, 0x0A92, 0x035C, 0x4DD4, 0x035C, 0x0A92, 0xF77B, 0xFABD, 
    0xF335, 0x0BA3, 0x05F1, 0x41FF, 0x05F1, 0x0BA3, 0xF335, 0xFEC1, 
    0xF0FE, 0x0AA5, 0x08C8, 0x37A5, 0x08C8, 0x0AA5, 0xF0FE, 0x0379, 
    0xF0A3, 0x088A, 0x0C45, 0x3123, 0x0C45, 0x088A, 0xF0A3, 0x0681, 
    0xF1B0, 0x0597, 0x114C, 0x2EC3, 0x114C, 0x0597, 0xF1B0, 0x06D3, 
    0xF433, 0x015A, 0x18FF, 0x2FB1, 0x18FF, 0x015A, 0xF433, 0x0400, 
    0xFBCD, 0x0432, 0x01A1, 0x6A64, 0x01A1, 0x0432, 0xFBCD, 0xFE75, 
    0xF8FA, 0x05F1, 0x02E5, 0x5F76, 0x02E5, 0x05F1, 0xF8FA, 0xFF32, 
    0xF497, 0x0738, 0x0592, 0x4C74, 0x0592, 0x0738, 0xF497, 0x0323, 
    0xF25D, 0x0671, 0x0879, 0x3F40, 0x0879, 0x0671, 0xF25D, 0x07A2, 
    0xF1F0, 0x0464, 0x0C22, 0x382B, 0x0C22, 0x0464, 0xF1F0, 0x0AA6, 
    0xF31C, 0x0116, 0x1165, 0x360A, 0x1165, 0x0116, 0xF31C, 0x0B33, 
    0xF716, 0xFC6C, 0x16AC, 0x31CC, 0x16AC, 0xFC6C, 0xF716, 0x07DC, 
    0xFAE6, 0x02AA, 0x0289, 0x690C, 0x0289, 0x02AA, 0xFAE6, 0x0257, 
    0xF861, 0x035A, 0x0431, 0x5CEB, 0x0431, 0x035A, 0xF861, 0x04BE, 
    0xF56F, 0x02F3, 0x0799, 0x4C31, 0x0799, 0x02F3, 0xF56F, 0x0992, 
    0xF4D7, 0x00CF, 0x0BB4, 0x43CA, 0x0BB4, 0x00CF, 0xF4D7, 0x0CD9, 
    0xF72B, 0xFD2E, 0x1005, 0x3C41, 0x1005, 0xFD2E, 0xF72B, 0x0C65, 
    0xFB74, 0xF89D, 0x14A7, 0x361D, 0x14A7, 0xF89D, 0xFB74, 0x08B3, 
    0xFBA1, 0x00CA, 0x0365, 0x6A00, 0x0365, 0x00CA, 0xFBA1, 0x04E1, 
    0xF9F7, 0x006C, 0x0598, 0x6023, 0x0598, 0x006C, 0xF9F7, 0x07C0, 
    0xF975, 0xFE28, 0x09E1, 0x5075, 0x09E1, 0xFE28, 0xF975, 0x0AFE, 
    0xFBF0, 0xFA6E, 0x0E4D, 0x45D5, 0x0E4D, 0xFA6E, 0xFBF0, 0x0A4A, 
    0x0069, 0xF5BE, 0x1307, 0x3E48, 0x1307, 0xF5BE, 0x0069, 0x064F, 
    0xFDD6, 0xFEBF, 0x042F, 0x6C27, 0x042F, 0xFEBF, 0xFDD6, 0x04A1, 
    0xFDCE, 0xFD40, 0x06CA, 0x6253, 0x06CA, 0xFD40, 0xFDCE, 0x0631, 
    0x0049, 0xF92B, 0x0BE5, 0x5444, 0x0BE5, 0xF92B, 0x0049, 0x05B2, 
    0x0480, 0xF570, 0x0F2C, 0x4243, 0x0F2C, 0xF570, 0x0480, 0x0140, 
    0x00DE, 0xFCDA, 0x04C5, 0x6CD6, 0x04C5, 0xFCDA, 0x00DE, 0x0194, 
    0x0278, 0xFA97, 0x0774, 0x6082, 0x0774, 0xFA97, 0x0278, 0x00D0, 
    0x0645, 0xF6DD, 0x0B18, 0x4807, 0x0B18, 0xF6DD, 0x0645, 0xFD0C, 
    0x035D, 0xFBC7, 0x04C8, 0x6500, 0x04C8, 0xFBC7, 0x035D, 0xFDC0, 
    0x058E, 0xF989, 0x070C, 0x557D, 0x070C, 0xF989, 0x058E, 0xFBA3, 
    0x01B2, 0xFE3B, 0x01D3, 0x73D4, 0x01D3, 0xFE3B, 0x01B2, 0xFE6B,
};

s16 D_801D3700[] = {
    0x02AF, 0x0FDA, 0x19C7, 0x1D78, 0x19C7, 0x0FDA, 0x02AF, 0xF61C, 
    0xFAE9, 0x0AD9, 0x18C1, 0x1E41, 0x18C1, 0x0AD9, 0xFAE9, 0xEFA7, 
    0xF4C7, 0x0560, 0x172B, 0x1ECE, 0x172B, 0x0560, 0xF4C7, 0xEE58, 
    0xEFF4, 0x0000, 0x1719, 0x2213, 0x1719, 0x0000, 0xEFF4, 0xF061, 
    0xEF77, 0xFACA, 0x147A, 0x2247, 0x147A, 0xFACA, 0xEF77, 0xF6BD, 
    0xF0D6, 0xF60D, 0x12C8, 0x24AD, 0x12C8, 0xF60D, 0xF0D6, 0xFC7C, 
    0xF400, 0xF1AC, 0x1147, 0x28C6, 0x1147, 0xF1AC, 0xF400, 0x0000, 
    0xF886, 0xEDFF, 0x0F6E, 0x2E2B, 0x0F6E, 0xEDFF, 0xF886, 0x0000, 
    0xFD5A, 0xEB95, 0x0CE5, 0x3489, 0x0CE5, 0xEB95, 0xFD5A, 0xFC79, 
    0x00F6, 0xEB7E, 0x093B, 0x3A0A, 0x093B, 0xEB7E, 0x00F6, 0xF773, 
    0x01FD, 0xED34, 0x04EE, 0x4046, 0x04EE, 0xED34, 0x01FD, 0xF3B9, 
    0x0000, 0xEECC, 0x0000, 0x4F8F, 0x0000, 0xEECC, 0x0000, 0xF1F9, 
    0xFB81, 0xF56D, 0xFA8F, 0x4DF2, 0xFA8F, 0xF56D, 0xFB81, 0xF6FA, 
    0xF91B, 0xF880, 0xF822, 0x4D58, 0xF822, 0xF880, 0xF91B, 0xF9E2, 
    0xF3CE, 0x0542, 0x1558, 0x1BD5, 0x1558, 0x0542, 0xF3CE, 0xE970, 
    0xEE2A, 0x0000, 0x145B, 0x1D41, 0x145B, 0x0000, 0xEE2A, 0xE9A5, 
    0xECB3, 0xFB0F, 0x117A, 0x1C51, 0x117A, 0xFB0F, 0xECB3, 0xEFC5, 
    0xECBC, 0xF681, 0x0FB5, 0x1D7D, 0x0FB5, 0xF681, 0xECBC, 0xF671, 
    0xEE61, 0xF1FD, 0x0E5E, 0x205E, 0x0E5E, 0xF1FD, 0xEE61, 0xFC61, 
    0xF1B4, 0xED99, 0x0CF2, 0x24B4, 0x0CF2, 0xED99, 0xF1B4, 0x0000, 
    0xF659, 0xE9CB, 0x0B04, 0x2A3A, 0x0B04, 0xE9CB, 0xF659, 0x0000, 
    0xFB43, 0xE733, 0x084B, 0x30AF, 0x084B, 0xE733, 0xFB43, 0xFC5F, 
    0xFEF0, 0xE64B, 0x04A5, 0x3812, 0x04A5, 0xE64B, 0xFEF0, 0xF6E9, 
    0x0000, 0xE731, 0x0000, 0x40DC, 0x0000, 0xE731, 0x0000, 0xF27C, 
    0xFE03, 0xED34, 0xFB12, 0x4046, 0xFB12, 0xED34, 0xFE03, 0xF3B9, 
    0xFA30, 0xF333, 0xF6A5, 0x4011, 0xF6A5, 0xF333, 0xFA30, 0xF803, 
    0xF81B, 0xF605, 0xF4A2, 0x400D, 0xF4A2, 0xF605, 0xF81B, 0xFA90, 
    0xEB5F, 0xFB40, 0x0FD6, 0x1929, 0x0FD6, 0xFB40, 0xEB5F, 0xEB9B, 
    0xEA89, 0xF6E6, 0x0DE0, 0x1962, 0x0DE0, 0xF6E6, 0xEA89, 0xF249, 
    0xEAFF, 0xF26E, 0x0C86, 0x1B4F, 0x0C86, 0xF26E, 0xEAFF, 0xF985, 
    0xED0B, 0xEDAB, 0x0B4D, 0x1ECF, 0x0B4D, 0xEDAB, 0xED0B, 0x0000, 
    0xF1C2, 0xEA2E, 0x0935, 0x21AF, 0x0935, 0xEA2E, 0xF1C2, 0x03C5, 
    0xF6BB, 0xE614, 0x071D, 0x2794, 0x071D, 0xE614, 0xF6BB, 0x03CB, 
    0xFBC2, 0xE154, 0x045B, 0x3172, 0x045B, 0xE154, 0xFBC2, 0x0000, 
    0x0000, 0xE00A, 0x0000, 0x39F5, 0x0000, 0xE00A, 0x0000, 0xF9E3, 
    0x0110, 0xE64B, 0xFB5B, 0x3812, 0xFB5B, 0xE64B, 0x0110, 0xF6EA, 
    0xFF0A, 0xEB7E, 0xF6C5, 0x3A0A, 0xF6C5, 0xEB7E, 0xFF0A, 0xF773, 
    0xFB31, 0xF18B, 0xF2BF, 0x3A53, 0xF2BF, 0xF18B, 0xFB31, 0xFB63, 
    0xF918, 0xF462, 0xF0E7, 0x3A85, 0xF0E7, 0xF462, 0xF918, 0xFDC7, 
    0xE938, 0xF2C1, 0x0B84, 0x18A1, 0x0B84, 0xF2C1, 0xE938, 0xF7BC, 
    0xEA2D, 0xEDD9, 0x0A51, 0x1B66, 0x0A51, 0xEDD9, 0xEA2D, 0x0000, 
    0xEEF3, 0xEAC6, 0x080F, 0x1C8A, 0x080F, 0xEAC6, 0xEEF3, 0x06C6, 
    0xF424, 0xE715, 0x05FE, 0x200D, 0x05FE, 0xE715, 0xF424, 0x0A64, 
    0xF9D8, 0xE25E, 0x0394, 0x26C7, 0x0394, 0xE25E, 0xF9D8, 0x0A7A, 
    0x0000, 0xDC77, 0x0000, 0x321F, 0x0000, 0xDC77, 0x0000, 0x06CC, 
    0x043E, 0xE154, 0xFBA5, 0x3172, 0xFBA5, 0xE154, 0x043E, 0x0000, 
    0x04BD, 0xE733, 0xF7B5, 0x30AF, 0xF7B5, 0xE733, 0x04BD, 0xFC5F, 
    0x02A6, 0xEB95, 0xF31B, 0x3489, 0xF31B, 0xEB95, 0x02A6, 0xFC79, 
    0xFE6A, 0xF068, 0xEE03, 0x39A7, 0xEE03, 0xF068, 0xFE6A, 0x0000, 
    0xFC2D, 0xF3F5, 0xECEE, 0x37CF, 0xECEE, 0xF3F5, 0xFC2D, 0x024D, 
    0xED74, 0xEB39, 0x076E, 0x19CF, 0x076E, 0xEB39, 0xED74, 0x08AA, 
    0xF2B6, 0xE7FC, 0x0553, 0x1BBD, 0x0553, 0xE7FC, 0xF2B6, 0x0EFD, 
    0xF8C3, 0xE3B2, 0x0313, 0x2043, 0x0313, 0xE3B2, 0xF8C3, 0x127D, 
    0x0000, 0xDDB7, 0x0000, 0x28BE, 0x0000, 0xDDB7, 0x0000, 0x12AE, 
    0x0628, 0xE25E, 0xFC6C, 0x26C7, 0xFC6C, 0xE25E, 0x0628, 0x0A7A, 
    0x0945, 0xE614, 0xF8E3, 0x2794, 0xF8E3, 0xE614, 0x0945, 0x03CB, 
    0x09A7, 0xE9CB, 0xF4FC, 0x2A3A, 0xF4FC, 0xE9CB, 0x09A7, 0x0000, 
    0x077A, 0xEDFF, 0xF092, 0x2E2B, 0xF092, 0xEDFF, 0x077A, 0x0000, 
    0x034E, 0xF2C7, 0xEBB5, 0x32EB, 0xEBB5, 0xF2C7, 0x034E, 0x036B, 
    0x00D6, 0xF54B, 0xE92F, 0x356F, 0xE92F, 0xF54B, 0x00D6, 0x05D4, 
    0xF83B, 0xE4A6, 0x02CC, 0x1CCD, 0x02CC, 0xE4A6, 0xF83B, 0x1758, 
    0x0000, 0xDF09, 0x0000, 0x232E, 0x0000, 0xDF09, 0x0000, 0x1AE2, 
    0x073D, 0xE3B2, 0xFCED, 0x2043, 0xFCED, 0xE3B2, 0x073D, 0x127D, 
    0x0BDC, 0xE715, 0xFA02, 0x200D, 0xFA02, 0xE715, 0x0BDC, 0x0A64, 
    0x0E3E, 0xEA2E, 0xF6CB, 0x21AF, 0xF6CB, 0xEA2E, 0x0E3E, 0x03C5, 
    0x0E4C, 0xED99, 0xF30E, 0x24B4, 0xF30E, 0xED99, 0x0E4C, 0x0000, 
    0x0C00, 0xF1AC, 0xEEB9, 0x28C6, 0xEEB9, 0xF1AC, 0x0C00, 0x0000, 
    0x07D3, 0xF665, 0xE9E8, 0x2D88, 0xE9E8, 0xF665, 0x07D3, 0x0365, 
    0x055E, 0xF8E4, 0xE766, 0x3009, 0xE766, 0xF8E4, 0x055E, 0x05CB, 
    0x07C5, 0xE4A6, 0xFD34, 0x1CCD, 0xFD34, 0xE4A6, 0x07C5, 0x1758, 
    0x0D4A, 0xE7FC, 0xFAAD, 0x1BBD, 0xFAAD, 0xE7FC, 0x0D4A, 0x0EFD, 
    0x110D, 0xEAC6, 0xF7F1, 0x1C8A, 0xF7F1, 0xEAC6, 0x110D, 0x06C6, 
    0x12F5, 0xEDAB, 0xF4B3, 0x1ECF, 0xF4B3, 0xEDAB, 0x12F5, 0x0000, 
    0x119F, 0xF1FD, 0xF1A2, 0x205E, 0xF1A2, 0xF1FD, 0x119F, 0xFC61, 
    0x0F2A, 0xF60D, 0xED38, 0x24AD, 0xED38, 0xF60D, 0x0F2A, 0xFC7C, 
    0x0B7A, 0xFA92, 0xE6E9, 0x2C00, 0xE6E9, 0xFA92, 0x0B7A, 0x0000, 
    0x08CD, 0xFD49, 0xE430, 0x2EB9, 0xE430, 0xFD49, 0x08CD, 0x029B, 
    0x128C, 0xEB39, 0xF892, 0x19CF, 0xF892, 0xEB39, 0x128C, 0x08AA, 
    0x15D3, 0xEDD9, 0xF5AF, 0x1B66, 0xF5AF, 0xEDD9, 0x15D3, 0x0000, 
    0x1501, 0xF26E, 0xF37A, 0x1B4F, 0xF37A, 0xF26E, 0x1501, 0xF985, 
    0x1344, 0xF681, 0xF04B, 0x1D7D, 0xF04B, 0xF681, 0x1344, 0xF671, 
    0x1089, 0xFACA, 0xEB86, 0x2247, 0xEB86, 0xFACA, 0x1089, 0xF6BD, 
    0x0C92, 0x0000, 0xE485, 0x2A2B, 0xE485, 0x0000, 0x0C92, 0xFA48, 
    0x0933, 0x02D5, 0xE2F0, 0x2B13, 0xE2F0, 0x02D5, 0x0933, 0xFD46, 
    0x16C8, 0xF2C1, 0xF47C, 0x18A1, 0xF47C, 0xF2C1, 0x16C8, 0xF7BC, 
    0x1577, 0xF6E6, 0xF220, 0x1962, 0xF220, 0xF6E6, 0x1577, 0xF249, 
    0x134D, 0xFB0F, 0xEE86, 0x1C51, 0xEE86, 0xFB0F, 0x134D, 0xEFC5, 
    0x100C, 0x0000, 0xE8E7, 0x2213, 0xE8E7, 0x0000, 0x100C, 0xF061, 
    0x094F, 0x057C, 0xE5BA, 0x2419, 0xE5BA, 0x057C, 0x094F, 0xF640, 
    0x0617, 0x080F, 0xE417, 0x2549, 0xE417, 0x080F, 0x0617, 0xF96C, 
    0x14A1, 0xFB40, 0xF02A, 0x1929, 0xF02A, 0xFB40, 0x14A1, 0xEB9B, 
    0x11D6, 0x0000, 0xEBA5, 0x1D41, 0xEBA5, 0x0000, 0x11D6, 0xE9A5, 
    0x0B39, 0x0560, 0xE8D5, 0x1ECE, 0xE8D5, 0x0560, 0x0B39, 0xEE58, 
    0x0459, 0x0AC8, 0xE54F, 0x217C, 0xE54F, 0x0AC8, 0x0459, 0xF525, 
    0x010B, 0x0D5C, 0xE385, 0x22EF, 0xE385, 0x0D5C, 0x010B, 0xF8B8, 
    0x0C32, 0x0542, 0xEAA8, 0x1BD5, 0xEAA8, 0x0542, 0x0C32, 0xE970, 
    0x0517, 0x0AD9, 0xE73F, 0x1E41, 0xE73F, 0x0AD9, 0x0517, 0xEFA7, 
    0xFDA3, 0x0F84, 0xE535, 0x1F3A, 0xE535, 0x0F84, 0xFDA3, 0xF897, 
    0xFACB, 0x1064, 0xE609, 0x1DBA, 0xE609, 0x1064, 0xFACB, 0xFCDD, 
    0xFD51, 0x0FDA, 0xE639, 0x1D78, 0xE639, 0x0FDA, 0xFD51, 0xF61C, 
    0xF735, 0x11A8, 0xE7C7, 0x1AA5, 0xE7C7, 0x11A8, 0xF735, 0x0000, 
    0xF5B4, 0x1151, 0xE97D, 0x186D, 0xE97D, 0x1151, 0xF5B4, 0x034F, 
    0xF379, 0x111B, 0xEBD5, 0x153F, 0xEBD5, 0x111B, 0xF379, 0x0722, 
    0xF2AA, 0x10DA, 0xECCD, 0x1407, 0xECCD, 0x10DA, 0xF2AA, 0x092E, 
    0xF0F2, 0x105C, 0xEED2, 0x1175, 0xEED2, 0x105C, 0xF0F2, 0x0D57,
};
