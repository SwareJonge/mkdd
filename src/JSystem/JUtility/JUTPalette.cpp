#include <dolphin/os.h>
#include <JSystem/JUtility/JUTPalette.h>
#include <JSystem/JUtility/JUTDbg.h>

void JUTPalette::storeTLUT(_GXTlut name, ResTLUT *tlut)
{
    if (tlut == NULL) {
        OSErrorLine(35, "JUTTexture: TLUT is NULL\n");
    }
    mTlutName = name;
    mFormat = tlut->format;
    mTransparency = tlut->transparency;
    mNumColors = tlut->numColors;
    mColorTable = tlut + 8;
    GXInitTlutObj(&mTlutObj, (void *)mColorTable, (GXTlutFmt)mFormat, mNumColors);
}

void JUTPalette::storeTLUT(_GXTlut name, _GXTlutFmt format, JUTTransparency transparency,
                           u16 colorCount, void *table)
{
    mTlutName = name;
    mFormat = format;
    mTransparency = transparency;
    mNumColors = colorCount;
    mColorTable = (ResTLUT *)table;
    GXInitTlutObj(&mTlutObj, (void *)mColorTable, (GXTlutFmt)mFormat, mNumColors);
}

bool JUTPalette::load()
{
    bool check = mNumColors != 0;
    if (check) {
        GXLoadTlut(&mTlutObj, mTlutName);
    }

    return check;
}