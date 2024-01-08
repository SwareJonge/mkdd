#include <stdio.h>
#include <string.h>

#include <JSystem/JMath/JMath.h>

#include "Kameda/ReadPrintMessage.h"
#include "Osako/ResMgr.h"
#include "Osako/SystemFile.h"

// TODO: documentation, this really needs it

// The checks for PAL are here because the PAL version is ever so slightly different, i assume this also applies to the demo versions

#define getChecksum(part) \
    getCRC((u8 *)&part, (u8 *)&part.mCheckData.mChecksum)

#define setChecksum(part) \
    part.mCheckData.mChecksum = getChecksum(part)

#define isCheckDataValid(part) \
    part.mCheckData.mChecksum == getChecksum(part) && part.mCheckData._03 == 0xc3

SystemFile gSystemFile;

char *SystemFile::mspFileNameString = "MarioKart Double Dash!!";

SystemFile::SystemFile()
{
    mCurrentIndex = 0;
    mBackupIndex = 1;
    mActiveSave = 0;
    mValidSections = 0;
    mSectionCount = 2;
    _6024 = 0;
    mDataChecksums[0] = 0;
    mDataChecksums[1] = 0;
}

SystemFile::~SystemFile() {}

void SystemFile::setPart(SaveFile::FilePart part)
{
    mPart = part;
}

void SystemFile::fetch()
{
    gSystemRecord.mTimesFetched++;

    if (mPart == mcHeader)
        initHeader();

    switch (mSectionCount)
    {
    case 0:
        initData(mCurrentIndex);
        mFileData[mCurrentIndex].mSystemRecord = gSystemRecord;
    case 1:
    case 2:
        initData(mBackupIndex);
        mFileData[mBackupIndex].mSystemRecord = gSystemRecord;
        mCurrentIndex = mBackupIndex;
        mBackupIndex ^= 1;
        break;
    }
}

void SystemFile::store()
{
    gSystemRecord = mFileData[mCurrentIndex].mSystemRecord;
    gSystemRecord.applyAudioSetting();
}

int SystemFile::getAccessWay() { return 1; }
char *SystemFile::getFileName() { return mspFileNameString; }
int SystemFile::getFileNo() { return 0; }
int SystemFile::getBannerFormat() { return CARD_STAT_BANNER_C8; }
s32 SystemFile::getCommentOffset() { return (s32)&mFileData[mActiveSave] - (s32)&mHeader; }
u8 SystemFile::getIconNum() { return 3; }
int SystemFile::getIconOffset() { return 0; }
u8 SystemFile::getIconAnim() { return CARD_STAT_ANIM_LOOP; }
u8 SystemFile::getIconFormat(u8) { return CARD_STAT_ICON_C8; }
u8 SystemFile::getIconSpeed(u8) { return CARD_STAT_SPEED_MIDDLE; }

void *SystemFile::getBuf()
{
    void *ret = nullptr;
    switch (mPart)
    {
    case mcHeader:
        ret = &mHeader;
        break;
    case mcData1:
    case mcData2:
        ret = &mFileData[mActiveSave];
        break;
    }
    return ret;
}

u32 SystemFile::getFileSize()
{
    return sizeof(Header) + sizeof(FileData) * 2;
}

s32 SystemFile::getLength()
{
    // TODO: i'm not entirely sure if this approach is correct
    s32 ret = 0;
    switch (mPart)
    {
    case mcHeader:
        ret = sizeof(Header) + sizeof(FileData) * 2; // 0x6000
        break;
    case mcData1:
        ret = sizeof(FileData) * 2; // 0x4000
        break;
    case mcData2:
        ret = sizeof(FileData); // 0x2000
        break;
    }
    return ret;
}

s32 SystemFile::getOffset()
{
    s32 ret = 0;
    switch (mPart)
    {
    case mcHeader:
        break;
    case mcData1:
        ret = (s32)&mFileData[0] - (s32)&mHeader;
        break;
    case mcData2:
        ret = (s32)&mFileData[mActiveSave] - (s32)&mHeader;
        break;
    }
    return ret;
}

void SystemFile::init()
{
    initHeader();
    initData(0);
    initData(1);

    mSectionCount = 0;
#ifndef VIDEO_PAL
    _6024 = 0;
    mDataChecksums[0] = 0;
    mDataChecksums[1] = 0;
#endif
}

void SystemFile::initHeader()
{
    ResTIMG *banner = (ResTIMG *)ResMgr::getPtr(ResMgr::mcArcSystem, "IPL/bn_System.bti");
    memmove(mHeader.mBanner, ((u8 *)banner + banner->mImageDataOffset), BANNER_SIZE);
    memmove(mHeader.mBannerPallete, ((u8 *)banner + banner->mPaletteOffset), PALETTE_SIZE);

    ResTIMG *icon = (ResTIMG *)ResMgr::getPtr(ResMgr::mcArcSystem, "IPL/ic_System.bti");
    memmove(mHeader.mIcon, ((u8 *)icon + icon->mImageDataOffset), BANNER_SIZE);
    memcpy(mHeader.mIconPalette, ((u8 *)icon + icon->mPaletteOffset), PALETTE_SIZE);

    mHeader.mCheckData.init();
}

void SystemFile::initData(u8 dataNo)
{
#line 307
    JUT_MINMAX_ASSERT(0, dataNo, 2)

    void *iplBmg = ResMgr::getPtr(ResMgr::mcIpl);
    char *comment = ReadPrintMessage::getMessage(iplBmg, 2);
    strncpy(mFileData[dataNo].mComment, comment, COMMENT_SIZE);
    mFileData[dataNo].mTag[0] = '\0'; // This makes no sense
    mFileData[dataNo].mCheckData.init();
}

void SystemFile::setCheckData(OSTime time)
{
    OSCalendarTime calendarTime;
    OSTimeDivider divider;
    divider.t = time;
    OSTicksToCalendarTime(time, &calendarTime);

    switch (mPart)
    {
    case mcHeader:
        mHeader.mCheckData._04 = divider.a[0];
        mHeader.mCheckData._08 = divider.a[1];
        setChecksum(mHeader);
    case mcData1:
        for (u8 i = 0; i < 2; i++)
            setCheckDataSub(i, divider, calendarTime);

        mCurrentIndex = 0;
        mBackupIndex = 1;
        mActiveSave = 0;
        break;
    case mcData2:
        setCheckDataSub(mCurrentIndex, divider, calendarTime);
        mActiveSave = mCurrentIndex;
        break;
    }
}

void SystemFile::setCheckDataSub(u8 dataNo, OSTimeDivider &divider, OSCalendarTime &calendarTime)
{
    mFileData[dataNo].mCheckData._04 = divider.a[0];
    mFileData[dataNo].mCheckData._08 = divider.a[1];

    void *iplBmg = ResMgr::getPtr(ResMgr::mcIpl);
    char *tag = ReadPrintMessage::getMessage(iplBmg, 0);

    ReadPrintMessage::mNum[0] = calendarTime.year % 100;
    ReadPrintMessage::mNum[1] = calendarTime.mon + 1;
    ReadPrintMessage::mNum[2] = calendarTime.mday;

    J2DTextBox::TFontSize fontSize;
    fontSize.x = 0.0f;
    fontSize.y = 0.0f;
    ReadPrintMessage::tagCnv(tag, nullptr, fontSize, TAG_SIZE, mFileData[dataNo].mTag);

    char *comment = ReadPrintMessage::getMessage(iplBmg, 2);
    strncpy(mFileData[dataNo].mComment, comment, COMMENT_SIZE);

    JMath::TRandom_<JMath::TRandom_fast_> rndm(calendarTime.msec);

    for (int i = 0; i < sizeof(mFileData[dataNo].mScrambleData); i++)
    {
        mFileData[dataNo].mScrambleData[i] = rndm.get() >> 24;
    }

    setChecksum(mFileData[dataNo]);
    mFileData[dataNo].mSystemRecord.crypt(mFileData[dataNo].mCheckData.mKey);
}

void SystemFile::checkData()
{
    mValidSections = 0;
    for (u8 i = 0; i < 2; i++)
    {
        mFileData[i].mSystemRecord.crypt(mFileData[i].mCheckData.mKey);
        if (isCheckDataValid(mFileData[i]))
            mValidSections |= 1 << i;
    }

    if (isCheckDataValid(mHeader))
        mValidSections |= 4;
    else
        mValidSections &= ~4;

    switch (mValidSections & 3)
    {
    case 0: // no sections are valid
        mSectionCount = 0;
        break;
    case 1: // section 1 is valid
    case 2: // section 2 is valid
        mSectionCount = 1;
        mCurrentIndex = (mValidSections & 3) - 1;
        mBackupIndex = 1 - mCurrentIndex;
        break;
    case 3: // both sections are valid
        mSectionCount = 2;

        // determine which save is older
        if (mFileData[0].mSystemRecord.mTimesFetched >= mFileData[1].mSystemRecord.mTimesFetched)
        {
            mCurrentIndex = 0;
            mBackupIndex = 1;
        }
        else
        {
            mCurrentIndex = 1;
            mBackupIndex = 0;
        }
        break;
    }

    mActiveSave = mCurrentIndex;
}

bool SystemFile::isSavable()
{
    if (mSectionCount == 0)
        return true;

    for (int i = 0; i < 2; i++)
    {
        // Uhhh don't you want to check if both checksums are identical?
#ifndef VIDEO_PAL
        if ((mValidSections & 1 << i) && (mDataChecksums[i] == mFileData[i].mCheckData.mChecksum))
            return true;
#else
        if ((mDataChecksums[i] == 0) || (mValidSections & 1 << i) && (mDataChecksums[i] == mFileData[i].mCheckData.mChecksum))
            return true;
#endif
    }
    return false;
}
