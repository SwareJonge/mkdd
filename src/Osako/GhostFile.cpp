#include <stdio.h>
#include <string.h>
#include <dolphin/dvd.h>
#include "Kameda/ReadPrintMessage.h"
#include "Kameda/SceneFactory.h"
#include "Osako/GhostFile.h"
#include "Osako/ResMgr.h"
#include "Osako/SystemRecord.h"

GhostFile gGhostFile;

#define setChecksum(part) \
    part.mCheckData.mChecksum = getChecksum(part)

#define getChecksum(part) \
    getCRC((u8 *)&mHeader, (u8 *)&part.mCheckData.mChecksum)

GhostFile::GhostFile() { mGhostType = 0; }

GhostFile::~GhostFile() {}

void GhostFile::setup()
{
    mFileData.mPadRecord.setName(gSystemRecord.mDefaultName);
    GIRecord record = mFileData.mPadRecord;
    makeFileName(mFileName, &record);

    JKRArchive *ghostFileArc = SceneFactory::ptr()->getArchive(6);

    char buf[0x20];
    snprintf(buf, sizeof(buf), "bnr/bn_%s.bti", ResMgr::getCrsArcName(mFileData.mPadRecord.getCourseID()));

    ResTIMG *banner = (ResTIMG *)ghostFileArc->getResource(buf);
    memcpy(mHeader.mBanner, ((u8 *)banner + banner->mImageDataOffset), BANNER_SIZE);
    memmove(mHeader.mBannerPallete, ((u8 *)banner + banner->mPaletteOffset), PALETTE_SIZE);

    snprintf(buf, sizeof(buf), "icon/ic_%s.bti", ResMgr::getCrsArcName(mFileData.mPadRecord.getCourseID()));

    ResTIMG *icon = (ResTIMG *)ghostFileArc->getResource(buf);
    memcpy(mHeader.mIcon, ((u8 *)icon + icon->mImageDataOffset), ICON_SIZE);
    memcpy(mHeader.mIconPalette, ((u8 *)icon + icon->mPaletteOffset), PALETTE_SIZE);

    int min, sec, ms;
    mFileData.mPadRecord.getRaceTime().get(&min, &sec, &ms);

    void *iplBmg = ResMgr::getPtr(ResMgr::mcIpl);
    char *comment = ReadPrintMessage::getMessage(iplBmg, 2);
    strncpy(mFileData.mComment, comment, COMMENT_SIZE);
    char *tag = ReadPrintMessage::getMessage(iplBmg, 1);

    ReadPrintMessage::mNum[0] = min;
    ReadPrintMessage::mNum[1] = sec;
    ReadPrintMessage::mNum[2] = ms;

    J2DTextBox::TFontSize fontSize;
    fontSize.x = 0.0f;
    fontSize.y = 0.0f;
    ReadPrintMessage::tagCnv(tag, nullptr, fontSize, TAG_SIZE, mFileData.mTagData);
}

bool GhostFile::updateKartPadRecord(const KartPadRecord &rRec)
{
    switch (mGhostType)
    {
    case 2:
    case 3:
        if (mFileData.mPadRecord.isLittle(rRec))
            break;
    case 0:
    case 1:
        if (!rRec.isFrameValid())
            break;

        mFileData.mPadRecord = rRec;
        mGhostType = 3;
        return true;

        break;
    }
    return false;
}

bool GhostFile::available(ECourseID courseID)
{
    if (mGhostType == 2 && courseID == mFileData.mPadRecord.getCourseID())
        return true;

    mGhostType = 0;
    return false;
}

bool GhostFile::isValidGhostOnRace()
{
    bool validGhostType = (mGhostType == 2 || mGhostType == 3 || mGhostType == 4); // probably an inline
    if (validGhostType && mFileData.mPadRecord.getCourseID() == ResMgr::getCourseID())
        return true;

    return false;
}

void GhostFile::makeFileName(char *pFileName, const GIRecord *pRecord)
{
#line 174
    JUT_ASSERT(pFileName)
    JUT_ASSERT(pRecord)

    u8 codeSize = uuencode(pFileName, 0x20, (const u8 *)pRecord, sizeof(GIRecord));
    if (codeSize < 0x20)
    {
        pFileName[codeSize++] = 'a';
        pFileName[codeSize++] = 'a';
        pFileName[codeSize++] = '\0';
    }
}

bool GhostFile::analysisFileName(GIRecord *pRecord, const char *pFileName)
{
#line 195
    JUT_ASSERT(pFileName)
    JUT_ASSERT(pRecord)

    bool validSize = uudecode((u8 *)pRecord, sizeof(GIRecord), pFileName, 0x20) == sizeof(GIRecord);
    return (validSize) && pRecord->isValid();
}

void GhostFile::remakeFileName()
{
    size_t n = strlen(mFileName);
    if (mFileName[n - 1] != 'z')
        mFileName[n - 1] += 1;
    else if (mFileName[n - 2] != 'z')
        mFileName[n - 2] += 1;
    else
    {
        mFileName[n - 1] = 'a';
        mFileName[n - 2] = 'a';
    }
}

u8 GhostFile::uuencode(char *pFileName, u8 codeBufSize, const u8 *pRecord, u8 recordBufSize)
{
#line 237
    u8 codeSize = ((recordBufSize + 2) / 3) * 4 + 1; // is this some sort of modulo?
    JUT_ASSERT(codeBufSize >= codeSize)

    *pFileName = (recordBufSize & 0x3f) + 0x21;

    for (u8 offset = 1, i = 0; offset < codeSize && i < recordBufSize;)
    {
        UUData u;

        u.arr[0] = pRecord[i++];
        u.arr[1] = pRecord[i++];
        u.arr[2] = pRecord[i++];

        pFileName[offset++] = u.s.bit1 + 0x21l;
        pFileName[offset++] = u.s.bit2 + 0x21l;
        pFileName[offset++] = u.s.bit3 + 0x21l;
        pFileName[offset++] = u.s.bit4 + 0x21l;
    }
    return codeSize;
}

u8 GhostFile::uudecode(u8 *pRecord, u8 recordBufSize, const char *pFileName, u8 codeBufSize)
{
    u8 codeSize = *pFileName - 0x21;

    if (recordBufSize >= codeSize)
    {
        u8 offset = 1;
        for (u8 i = 0; offset < codeBufSize && i < codeSize; offset)
        {
            UUData u;

            u.s.bit1 = pFileName[offset++] - 0x21l;
            u.s.bit2 = pFileName[offset++] - 0x21l;
            u.s.bit3 = pFileName[offset++] - 0x21l;
            u.s.bit4 = pFileName[offset++] - 0x21l;

            pRecord[i++] = u.arr[0];
            pRecord[i++] = u.arr[1];
            pRecord[i++] = u.arr[2];
        }
    }

    return codeSize;
}

int GhostFile::getAccessWay() { return mAccessWay; }
char *GhostFile::getFileName() { return mFileName; }
int GhostFile::getFileNo() { return mFileNo; }
int GhostFile::getBannerFormat() { return CARD_STAT_BANNER_C8; }
s32 GhostFile::getCommentOffset() { return (s32)&mFileData - (s32)&mHeader; }
u8 GhostFile::getIconNum() { return 1; }
int GhostFile::getIconOffset() { return 0; }
u8 GhostFile::getIconAnim() { return CARD_STAT_ANIM_BOUNCE; }
u8 GhostFile::getIconFormat(u8) { return CARD_STAT_ICON_C8; }
u8 GhostFile::getIconSpeed(u8) { return CARD_STAT_SPEED_SLOW; }
void *GhostFile::getBuf() { return &mHeader; }
u32 GhostFile::getFileSize() { return sizeof(Header) + sizeof(FileData); }
s32 GhostFile::getLength() { return sizeof(Header) + sizeof(FileData); }
s32 GhostFile::getOffset() { return 0; }

void GhostFile::setCheckData(s64)
{
    mFileData.mCheckData.mGameVersion = DVDGetCurrentDiskID()->gameVersion;
    setChecksum(mFileData);
}

void GhostFile::checkData()
{
    if (mFileData.mCheckData.mChecksum == getChecksum(mFileData))
        mGhostType = 2;
    else
        mGhostType = 1;
}