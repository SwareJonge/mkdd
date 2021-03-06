#include "types.h"
#include "Osako/SaveFile.h"

#include "JSystem/JUtility/JUTAssert.h"
#include "Dolphin/OS.h"

u32 SaveFile::msaCRCTable[256];
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
    if (!pBegin)
    {
        JUTAssertion::showAssert_f(JUTAssertion::getSDevice(), __FILE__, 59, "%s", "pBegin");
        OSPanic(__FILE__, 59, "Halt");
    }
    if (!pEndNext)
    {
        JUTAssertion::showAssert_f(JUTAssertion::getSDevice(), __FILE__, 60, "%s", "pEndNext");
        OSPanic(__FILE__, 60, "Halt");
    }
    if (pBegin >= pEndNext)
    {
        JUTAssertion::showAssert_f(JUTAssertion::getSDevice(), __FILE__, 61, "%s", "pBegin < pEndNext");
        OSPanic(__FILE__, 61, "Halt");
    }
    if (!msCRCTableComputed)
    {
        makeCRCTable();
    }

    for (u8 *i = pBegin; i != pEndNext; i++)
    {
        crc = msaCRCTable[(crc ^ *i) & 0xff] ^ (crc >> 8);
    }

    return crc ^ 0xffffffff;
}