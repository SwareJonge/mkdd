#include <dolphin/os.h>
#include "JSystem/JUtility/JUTDbg.h"
#include "types.h"

#include "Osako/SaveFile.h"

u32 SaveFile::msaCRCTable[256]; // -common on
bool SaveFile::msCRCTableComputed;

void SaveFile::makeCRCTable()
{
    u32 c;

    for (u32 byte = 0; byte < 256; byte++)
    {
        c = byte;
        for (s32 bit = 0; bit < 8; bit++)
            c = (c & 1) ? (c >> 1) ^ 0xEDB88320 : c >> 1;
        msaCRCTable[byte] = c;
    }
    msCRCTableComputed = true;
}

u32 SaveFile::getCRC(u8 *pBegin, u8 *pEndNext)
{
    u32 crc = -1;

    JUT_ASSERT(59, pBegin);
    JUT_ASSERT(60, pEndNext);
    JUT_ASSERT(61, pBegin < pEndNext);

    if (!msCRCTableComputed)
        makeCRCTable();

    for (u8 *i = pBegin; i != pEndNext; i++)
        crc = msaCRCTable[(crc ^ *i) & 0xff] ^ (crc >> 8);

    return crc ^ 0xffffffff;
}
