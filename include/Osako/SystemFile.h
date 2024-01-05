#ifndef SYSTEMFILE_H
#define SYSTEMFILE_H

#include <dolphin/os/OSTime.h>

#include "Osako/SaveFile.h"
#include "Osako/SystemRecord.h"

// TODO: where do i put this
typedef union OSTimeDivider
{
    OSTime t;
    u32 a[2];
} OSTimeDivider;

class SystemFile : public SaveFile
{
public:
    SystemFile();

    void init();       // 0x80205584
    void initHeader(); // 0x802055dc
    void initData(u8); // 0x80205694
    bool isSavable();  // 0x80205b18
    void setCheckDataSub(u8, OSTimeDivider &, OSCalendarTime &);

    virtual void setPart(FilePart); // 08
    virtual void fetch();           // 0C
    virtual void store();           // 10
    virtual void setCheckData(s64); // 14
    virtual void checkData();       // 18
    virtual int getAccessWay();     // 1C
    virtual char *getFileName();    // 20
    virtual int getFileNo();        // 24
    virtual int getBannerFormat();  // 28
    virtual s32 getCommentOffset(); // 2C
    virtual u8 getIconNum();        // 30
    virtual int getIconOffset();    // 34
    virtual u8 getIconFormat(u8);   // 38
    virtual u8 getIconSpeed(u8);    // 3C
    virtual u8 getIconAnim();       // 40
    virtual void *getBuf();           // 44
    virtual u32 getFileSize();      // 48
    virtual s32 getLength();        // 4C
    virtual s32 getOffset();        // 50
    virtual ~SystemFile();          // 54

    class FileData
    {
    public:
        // Inline
        FileData() {}

        char mComment[32];
        char mTag[32];
        SystemRecord mSystemRecord;
        u8 mScrambleData[0x19dc]; // an array of randum numbers
        u16 mSeed;
        u8 _1ff2;
        u8 _1ff3;
        u32 _1ff4;
        u32 _1ff8;
        u32 mChecksum; // _1ffc
    };                 // size: 0x2000

    void checksumThing() { mChecksums[_6022] = mFileData[_6022].mChecksum; }
    void checksumThing2()
    {
        mChecksums[0] = mFileData[0].mChecksum;
        mChecksums[1] = mFileData[1].mChecksum;
    }

    static char *mspFileNameString;

    // vtable 0x0
    // aligned buffer?
    u8 mBanner[3072] ALIGN(32);
    u8 mBannerPallete[512];
    u8 mIcon[3072];
    u8 mIconPalette[512];
    u8 _1c20[0x2010 - 0x1c20];
    u16 mTick;
    u8 _2012;
    u8 _2013; // some sort of identifier?
    u32 _2014;
    u32 _2018;
    u32 mChecksum;
    FileData mFileData[2];
    u8 _6020;
    u8 _6021;
    u8 _6022;
    u8 _6023; // the active FileData section(s)?
    u8 _6024;
    u8 _6025[3];
    FilePart mPart;
    int _602c;
    u32 mChecksums[2];
    u8 _6038[0x6040 - 0x6038];
}; // Size: 0x6040

extern SystemFile gSystemFile;

#endif
