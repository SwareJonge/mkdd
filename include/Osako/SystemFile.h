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

    virtual void setPart(FilePart); // 08, 0x802050c8
    virtual void fetch();           // 0C, 0x802050d0
    virtual void store();           // 10, 0x8020532c
    virtual void setCheckData(s64); // 14, 0x80205760
    virtual void checkData();       // 18, 0x80205998
    virtual int getAccessWay();     // 1C, 0x80205440
    virtual char *getFileName();    // 20, 0x80205448
    virtual int getFileNo();        // 24, 0x80205450
    virtual int getBannerFormat();  // 28, 0x80205458
    virtual s32 getCommentOffset(); // 2C, 0x80205460
    virtual u8 getIconNum();        // 30, 0x8020547c
    virtual int getIconOffset();    // 34, 0x80205484
    virtual u8 getIconFormat(u8);   // 38, 0x80205494
    virtual u8 getIconSpeed(u8);    // 3C, 0x8020549c
    virtual u8 getIconAnim();       // 40, 0x8020548c
    virtual void *getBuf();         // 44, 0x802054a4
    virtual u32 getFileSize();      // 48, 0x802054e4
    virtual s32 getLength();        // 4C, 0x802054ec
    virtual s32 getOffset();        // 50, 0x80205530
    virtual ~SystemFile();          // 54, 0x80205080

    // Fabricated
    struct CheckData
    {
        void init()
        {
            mKey = OSGetTick();
            _02 = 0;
            _03 = 0xc3;
        }

        u16 mKey;      // the key/mask to decrypt systemRecord data, unused for the header
        u8 _02;        // unused, maybe this was game version as well(which would also always be 0?)
        u8 _03;        // some sort of identifier?
        u32 _04, _08;  // OSTimeDivider values
        u32 mChecksum; // CRC32 of the block
    };                 // Size: 0x10

    class FileData
    {
    public:
        // Inline
        FileData() {}

        char mComment[COMMENT_SIZE];
        char mTag[TAG_SIZE];
        SystemRecord mSystemRecord;
        u8 mScrambleData[0x19dc]; // an array of random numbers
        CheckData mCheckData;
    }; // size: 0x2000

    void setActiveChecksum() { mDataChecksums[mActiveSave] = mFileData[mActiveSave].mCheckData.mChecksum; }

    void setDataChecksums()
    {
        mDataChecksums[0] = mFileData[0].mCheckData.mChecksum;
        mDataChecksums[1] = mFileData[1].mCheckData.mChecksum;
    }

    static char *mspFileNameString;

    // vtable 0x0
    struct Header
    {
        u8 mBanner[BANNER_SIZE];
        u8 mBannerPallete[PALETTE_SIZE];
        u8 mIcon[BANNER_SIZE]; // TODO: why is the icon size bigger here compared to ghostfile?
        u8 mIconPalette[PALETTE_SIZE];
        u8 _1c20[0x3F0]; // unused, padding to get the size up to 0x2000?
        CheckData mCheckData;
    } mHeader ALIGN(32);

    FileData mFileData[2];

    u8 mCurrentIndex;      // this should be the same as active save? i don't really understand the difference
    u8 mBackupIndex;       // the index of the backup data, opposite of active save
    u8 mActiveSave;        // the index of the current active save,
    u8 mValidSections;     // 1 = mFileData[0] valid, 2 = mFileData[1] valid, 4 = Header valid
    u8 _6024;              // unused
    FilePart mPart;        // the requested part
    int mSectionCount;     // might be another FilePart
    u32 mDataChecksums[2]; // CRC32 of FileData
};                         // Size: 0x6040

extern SystemFile gSystemFile;

#endif
