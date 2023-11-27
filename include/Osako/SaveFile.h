#ifndef SAVEFILE_H
#define SAVEFILE_H

#include "types.h"

class SaveFile
{
public:
    static bool msCRCTableComputed;
    static u32 msaCRCTable[256];
    void makeCRCTable();
    u32 getCRC(u8 *pBegin, u8 *pEndNext);

    enum FilePart
    {

    };

    virtual void setPart(FilePart) = 0; // 08
    virtual void fetch() = 0;           // 0C
    virtual void store() = 0;           // 10
    virtual void setCheckData(s64) = 0; // 14
    virtual void checkData() = 0;       // 18
    virtual int getAccessWay() = 0;     // 1C
    virtual char *getFileName() = 0;    // 20
    virtual int getFileNo() = 0;        // 24
    virtual int getBannerFormat() = 0;  // 28
    virtual s32 getCommentOffset() = 0; // 2C
    virtual u8 getIconNum() = 0;        // 30
    virtual int getIconOffset() = 0;    // 34
    virtual u8 getIconFormat(u8) = 0;   // 38
    virtual u8 getIconSpeed(u8) = 0;    // 3C
    virtual u8 getIconAnim() = 0;       // 40
    virtual u8 *getBuf() = 0;           // 44
    virtual u32 getFileSize() = 0;      // 48
    virtual s32 getLength() = 0;        // 4C
    virtual s32 getOffset() = 0;        // 50
};

#endif