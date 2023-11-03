#ifndef SYSTEMFILE_H
#define SYSTEMFILE_H
#include "Osako/SaveFile.h"
#include "Osako/systemRecord.h"

class SystemFile : public SaveFile
{
public:
    SystemFile();

    virtual void setPart(FilePart);  // 08
    virtual void fetch();            // 0C
    virtual void store();            // 10
    virtual void setCheckData(s64);  // 14
    virtual void checkData();        // 18
    virtual int getAccessWay();      // 1C
    virtual char *getFileName();     // 20
    virtual int getFileNo();         // 24
    virtual int getBannerFormat();   // 28
    virtual long getCommentOffset(); // 2C
    virtual u8 getIconNum();         // 30
    virtual int getIconOffset();     // 34
    virtual u8 getIconFormat(u8);    // 38
    virtual u8 getIconSpeed(u8);     // 3C
    virtual u8 getIconAnim();        // 40
    virtual u8 *getBuf();            // 44
    virtual long getFileSize();      // 48
    virtual long getLength();        // 4C
    virtual long getOffset();        // 50
    virtual ~SystemFile();           // 54

    class FileData
    {
    public:
        char mTitle[64];
        SystemRecord mSystemRecord;
        u8 _614[0x1ff0 - 0x614];
        u16 mSeed;
        u8 _1ff2[0x2000 - 0x1ff2];
    }; // size: 0x2000

    // vtable 0x0
    // aligned buffer?
    u8 _4[0x20 - 0x4];
    u8 _20[3072];
    u8 _c20[512];
    u8 _e20[3072];
    u8 _1a20[512];
    u8 _1c20[0x2010 - 0x1c20];
    u16 mTick;
    u8 _2012[0x201c - 0x2012];
    u32 mCheckSum;
    FileData mFileData[2];
    u8 _6020;
    u8 _6021;
    u8 _6022;
    u8 _6023;
    u8 _6024;
    u8 _6025[3];
    FilePart mPart;
    u8 _602c[0x6040 - 0x602c];
}; // Size: 0x6040

extern SystemFile gSystemFile;

#endif