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

    enum FilePart {

    };

    virtual void setPart(FilePart) = 0;        // 08
    virtual void fetch() = 0;                  // 0C
    virtual void store() = 0;                  // 10
    virtual void setCheckData(long, long) = 0; // 14
    virtual void checkData() = 0;              // 18
    virtual int getAccessWay() = 0;            // 1C
    virtual char *getFileName() = 0;           // 20
    virtual int getFileNo() = 0;               // 24
    virtual int getBannerFormat() = 0;         // 28
    virtual long getCommentOffset() = 0;       // 2C
    virtual int getIconNum() = 0;              // 30
    virtual int getIconOffset() = 0;           // 34
    virtual int getIconFormat() = 0;           // 38
    virtual int getIconSpeed() = 0;            // 3C
    virtual int getIconAnim() = 0;             // 40
    virtual u8 *getBuf() = 0;                  // 44
    virtual long getFileSize() = 0;            // 48
    virtual long getLength() = 0;              // 4C
    virtual long getOffset() = 0;              // 50
};

#endif