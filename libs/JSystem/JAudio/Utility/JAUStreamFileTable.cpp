#include <dolphin/dvd.h>

#include "JSystem/JAudio/JAUStreamFileTable.h"

JAUStreamFileTable::JAUStreamFileTable() { mBuf = NULL; }

void JAUStreamFileTable::init(const void *pBuf)
{
    const BSFT *pBSFT = (const BSFT *)pBuf;
    if (pBSFT == NULL)
    {
        mBuf = NULL;
        return;
    }

    if (pBSFT->magic[0] == 'b' && pBSFT->magic[1] == 's' && pBSFT->magic[2] == 'f' && pBSFT->magic[3] == 't')
    {
        mBuf = pBSFT;
    }
}

u32 JAUStreamFileTable::getNumFiles() const { return mBuf->fileCount; }

const char *JAUStreamFileTable::getFilePath(int index) const
{
    if (mBuf == NULL)
        return NULL;
#line 52
    JUT_ASSERT(index >= 0);
    JUT_ASSERT(index < getNumFiles());

    //u32 fileOffset = mBuf->filenameOffsets[index];
    u32 fileOffset = *(u32*)(mBuf->filenameOffsets + index); // why
    return (const char *)mBuf + fileOffset;
}

const char *JAUStreamFileTable::getFilePath_fromEntry(s32 entry) const
{
    // UNUSED
    if (mBuf == NULL)
        return NULL;

    JUT_ASSERT(entry >= 0)
}

s32 JAUStreamDataMgr_StreamFileTable::getStreamFileEntry(JAISoundID soundID)
{
    const char *filePath = getFilePath(soundID.mId.mAdvancedId.mShortId);
    if (filePath == NULL)
        return -1;

    return DVDConvertPathToEntrynum(filePath);
}
