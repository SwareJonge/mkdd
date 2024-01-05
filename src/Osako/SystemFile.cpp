#include <stdio.h>
#include <string.h>

#include <JSystem/JMath/JMath.h>

#include "Kameda/ReadPrintMessage.h"
#include "Osako/ResMgr.h"
#include "Osako/SystemFile.h"

// TODO: documentation, this really needs it

SystemFile gSystemFile;

char *SystemFile::mspFileNameString = "MarioKart Double Dash!!";

SystemFile::SystemFile()
{
    _6020 = 0;
    _6021 = 1;
    _6022 = 0;
    _6023 = 0;
    _602c = 2;
    _6024 = 0;
    mChecksums[0] = 0;
    mChecksums[1] = 0;
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

    switch (_602c)
    {
    case 0:
        initData(_6020);
        mFileData[_6020].mSystemRecord = gSystemRecord;
    case 1:
    case 2:
        initData(_6021);
        mFileData[_6021].mSystemRecord = gSystemRecord;
        _6020 = _6021;
        _6021 ^= 1;
        break;
    }
}

void SystemFile::store()
{
    gSystemRecord = mFileData[_6020].mSystemRecord;
    gSystemRecord.applyAudioSetting();
}

int SystemFile::getAccessWay() { return 1; }
char *SystemFile::getFileName() { return mspFileNameString; }
int SystemFile::getFileNo() { return 0; }
int SystemFile::getBannerFormat() { return CARD_STAT_BANNER_C8; }
s32 SystemFile::getCommentOffset() { return (s32)&mFileData[_6022] - (s32)&mBanner; }
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
    case 0:
        ret = &mBanner;
        break;
    case 1:
    case 2:
        ret = &mFileData[_6022];
        break;
    }
    return ret;
}

u32 SystemFile::getFileSize()
{
    return 0x6000;
}

s32 SystemFile::getLength()
{
    s32 ret = 0;
    switch (mPart)
    {
    case 0:
        ret = 0x6000;
        break;
    case 1:
        ret = 0x4000;
        break;
    case 2:
        ret = 0x2000;
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
    case mcData:
        ret = (s32)&mFileData[0] - (s32)&mBanner;
        break;
    case mcDataSub:
        ret = (s32)&mFileData[_6022] - (s32)&mBanner;
        break;
    }
    return ret;
}

void SystemFile::init()
{
    initHeader();
    initData(0);
    initData(1);

    _602c = 0;
#ifdef DEBUG // TODO: did this get added in later builds?(JP Release)
    _6024 = 0;
    mChecksums[0] = 0;
    mChecksums[1] = 0;
#endif
}

void SystemFile::initHeader()
{
    ResTIMG *banner = (ResTIMG *)ResMgr::getPtr(ResMgr::mcArcSystem, "IPL/bn_System.bti");
    memmove(mBanner, ((u8 *)banner + banner->mImageDataOffset), sizeof(mBanner));
    memmove(mBannerPallete, ((u8 *)banner + banner->mPaletteOffset), sizeof(mBannerPallete));

    ResTIMG *icon = (ResTIMG *)ResMgr::getPtr(ResMgr::mcArcSystem, "IPL/ic_System.bti");
    memmove(mIcon, ((u8 *)icon + icon->mImageDataOffset), sizeof(mIcon));
    memcpy(mIconPalette, ((u8 *)icon + icon->mPaletteOffset), sizeof(mIconPalette));

    mTick = OSGetTick();
    _2012 = 0;
    _2013 = 0xc3;
}

void SystemFile::initData(u8 dataNo)
{
#line 307
    JUT_MINMAX_ASSERT(0, dataNo, 2)

    void *iplBmg = ResMgr::getPtr(ResMgr::mcIpl);
    char *comment = ReadPrintMessage::getMessage(iplBmg, 2);
    strncpy(mFileData[dataNo].mComment, comment, sizeof(mFileData[dataNo].mComment));
    mFileData[dataNo].mTag[0] = '\0'; // This makes no sense
    mFileData[dataNo].mSeed = OSGetTick();
    mFileData[dataNo]._1ff2 = 0;
    mFileData[dataNo]._1ff3 = 0xc3;
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
        _2014 = divider.a[0];
        _2018 = divider.a[1];
        mChecksum = getCRC(mBanner, (u8 *)&mChecksum);
    case mcData:
        for (u8 i = 0; i < 2; i++)
            setCheckDataSub(i, divider, calendarTime);

        _6020 = 0;
        _6021 = 1;
        _6022 = 0;
        break;
    case mcDataSub:
        setCheckDataSub(_6020, divider, calendarTime);
        _6022 = _6020;
        break;
    }
}

void SystemFile::setCheckDataSub(u8 dataNo, OSTimeDivider &divider, OSCalendarTime &calendarTime)
{
    mFileData[dataNo]._1ff4 = divider.a[0];
    mFileData[dataNo]._1ff8 = divider.a[1];

    void *iplBmg = ResMgr::getPtr(ResMgr::mcIpl);
    char *tag = ReadPrintMessage::getMessage(iplBmg, 0);

    ReadPrintMessage::mNum[0] = calendarTime.year % 100;
    ReadPrintMessage::mNum[1] = calendarTime.mon + 1;
    ReadPrintMessage::mNum[2] = calendarTime.mday;

    J2DTextBox::TFontSize fontSize;
    fontSize.x = 0.0f;
    fontSize.y = 0.0f;
    ReadPrintMessage::tagCnv(tag, nullptr, fontSize, sizeof(mFileData[dataNo].mTag), mFileData[dataNo].mTag);

    char *comment = ReadPrintMessage::getMessage(iplBmg, 2);
    strncpy(mFileData[dataNo].mComment, comment, sizeof(mFileData[dataNo].mComment));

    JMath::TRandom_<JMath::TRandom_fast_> rndm(calendarTime.msec);

    for (int i = 0; i < sizeof(mFileData[dataNo].mScrambleData); i++)
    {
        mFileData[dataNo].mScrambleData[i] = rndm.get() >> 24;
    }

    mFileData[dataNo].mChecksum = getCRC((u8 *)&mFileData[dataNo], (u8 *)&mFileData[dataNo].mChecksum);
    mFileData[dataNo].mSystemRecord.crypt(mFileData[dataNo].mSeed);
}

void SystemFile::checkData()
{
    _6023 = 0;
    for (u8 i = 0; i < 2; i++)
    {
        mFileData[i].mSystemRecord.crypt(mFileData[i].mSeed);
        if (mFileData[i].mChecksum == getCRC((u8 *)&mFileData[i], (u8 *)&mFileData[i].mChecksum) && mFileData[i]._1ff3 == 0xc3)
            _6023 |= 1 << i;
    }

    if (mChecksum == getCRC(mBanner, (u8 *)&mChecksum) && _2013 == 0xc3)
        _6023 |= 4;
    else
        _6023 &= ~4;

    switch (_6023 & 3)
    {
    case 0:
        _602c = 0;
        break;
    case 1:
    case 2:
        _602c = 1;
        _6020 = (_6023 & 3) - 1;
        _6021 = 1 - _6020;
        break;
    case 3:
        _602c = 2;
        if (mFileData[0].mSystemRecord.mTimesFetched >= mFileData[1].mSystemRecord.mTimesFetched)
        {
            _6020 = 0;
            _6021 = 1;
        }
        else
        {
            _6020 = 1;
            _6021 = 0;
        }
        break;
    }

    _6022 = _6020;
}

bool SystemFile::isSavable()
{
    if (_602c == 0) // isInitialized?
        return true;

    for (int i = 0; i < 2; i++)
    {
        // Uhhh don't you want to check if both checksums are identical?
#ifdef DEBUG // I guess this got changed in later builds? check JP release to be sure
        if (_6023 & 1 << i && mChecksums[i] == mFileData[i].mChecksum)
            return true;
#else
        if (mChecksums[i] == 0 || _6023 & 1 << i && mChecksums[i] == mFileData[i].mChecksum)
            return true;
#endif
    }
    return false;
}
