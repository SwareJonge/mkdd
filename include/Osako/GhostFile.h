#ifndef GHOSTFILE_H
#define GHOSTFILE_H

#include "kartEnums.h"

#include "Osako/GIRecord.h"
#include "Osako/PadRecorder.h"
#include "Osako/SaveFile.h"

class GhostFile : public SaveFile
{ // Autogenerated
public:
    // Global
    ~GhostFile();                                    // 0x8020e3ac
    void setup();                                    // 0x8020e3f4
    void updateKartPadRecord(const KartPadRecord &); // 0x8020e5c4
    void available(ECourseID);                       // 0x8020e6b0
    void isValidGhostOnRace();                       // 0x8020e6e8
    void makeFileName(char *, const GIRecord *);     // 0x8020e738
    void analysisFileName(GIRecord *, const char *); // 0x8020e828
    void remakeFileName();                           // 0x8020e918
    void uuencode(char *, u8, const u8 *, u8);       // 0x8020e990
    void uudecode(u8 *, u8, const char *, u8);       // 0x8020eae4
    virtual int getAccessWay();                      // 0x8020ebf0, overide
    virtual char *getFileName();                     // 0x8020ebfc, overide
    virtual int getFileNo();                         // 0x8020ec08, overide
    virtual int getBannerFormat();                   // 0x8020ec14, overide
    virtual s32 getCommentOffset();                  // 0x8020ec1c, overide
    virtual u8 getIconNum();                         // 0x8020ec2c, overide
    virtual int getIconOffset();                     // 0x8020ec34, overide
    virtual u8 getIconAnim();                        // 0x8020ec3c, overide
    virtual u8 getIconFormat(u8);                    // 0x8020ec44, overide
    virtual u8 getIconSpeed(u8);                     // 0x8020ec4c, overide
    virtual u8 *getBuf();                            // 0x8020ec54, overide
    virtual u32 getFileSize();                       // 0x8020ec5c, overide
    virtual s32 getLength();                         // 0x8020ec68, overide
    virtual s32 getOffset();                         // 0x8020ec74, overide
    virtual void setCheckData(s64);                  // 0x8020ec7c, overide
    virtual void checkData();                        // 0x8020eccc, overide
    // Inline/Unused
    // GhostFile();
    // Inline
    virtual void setPart(SaveFile::FilePart) {} // 0x8020ed28, overide
    virtual void fetch() {}                     // 0x8020ed2c, overide
    virtual void store() {}                     // 0x8020ed30, overide

    u8 pad[0x20 - 0x4];       // can i just use attribute align here? afaik this doesn't get written to
    u8 mBanner[3072];         // 0x20
    u8 mBannerPallete[512];   // C20
    u8 mIcon[1024];           // E20
    u8 mIconPalette[512];     // 1220
    char _1420[32];           //
    char mTagData[32];        // 1440
    KartPadRecord mPadRecord; // 1460
    u8 mGameVersion;          // A018
    u32 mChecksum;            // A01C
    char mFileName[32];       // A020
    int mFileNo;              // A040
    int mAccessWay;           // A044
    int mGhostType;           // A048
    u8 _a04c[0xa060 - 0xa04c];
}; // Size: 0xa060

extern GhostFile gGhostFile;

#endif // GHOSTFILE_H