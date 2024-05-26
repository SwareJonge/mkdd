#ifndef JAUDIO_JAUSTREAMFILETABLE_H
#define JAUDIO_JAUSTREAMFILETABLE_H

#include "JSystem/JAudio/Interface/JAIStreamDataMgr.h"

struct BSFT
{
    u8 magic[4];            // 0
    u32 fileCount;          // 4
    u32 filenameOffsets[1]; // 8, dynamic length
};

class JAUStreamFileTable
{
public:
    // Global
    JAUStreamFileTable();                     // 0x800a98b4
    void init(const void *);                  // 0x800a98c0
    u32 getNumFiles() const;                  // 0x800a990c
    const char *getFilePath(int index) const; // 0x800a9918
    // Inline/Unused
    const char *getFilePath_fromEntry(long entry) const;

    bool isValid() { return mBuf; }

private:
    const BSFT *mBuf;
};

class JAUStreamDataMgr_StreamFileTable : public JAIStreamDataMgr, public JAUStreamFileTable
{
public:
    JAUStreamDataMgr_StreamFileTable() {}

    virtual ~JAUStreamDataMgr_StreamFileTable() {}
    virtual s32 getStreamFileEntry(JAISoundID soundID);
};

#endif