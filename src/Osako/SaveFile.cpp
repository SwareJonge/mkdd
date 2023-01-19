#include "JSystem/JUtility/JUTDbg.h"
#include "dolphin/OS.h"
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
        {
            bool flag = (c & 1) != 0;
            c = (c >> 1);
            if (flag)
                c = c ^ 0xEDB88320;
        }
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