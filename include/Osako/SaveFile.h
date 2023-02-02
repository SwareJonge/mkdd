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

    class FilePart {

    };

    virtual void setPart(FilePart *);      // 08
    virtual void fetch();                  // 0C
    virtual void store();                  // 10
    virtual void setCheckData(long, long); // 14
    virtual void checkData();              // 18
    virtual int getAccessWay();            // 1C
    virtual char *getFileName();           // 20
    virtual int getFileNo();               // 24
    virtual int getBannerFormat();         // 28
    virtual long getCommentOffset();       // 2C
    virtual int getIconNum();              // 30
    virtual int getIconOffset();           // 34
    virtual int getIconFormat();           // 38
    virtual int getIconSpeed();            // 3C
    virtual int getIconAnim();             // 40
    virtual u8 *getBuf();                  // 44
    virtual long getFileSize();            // 48
    virtual long getLength();              // 4C
    virtual long getOffset();              // 50
};

#endif