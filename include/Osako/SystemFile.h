#ifndef SYSTEMFILE_H
#define SYSTEMFILE_H
#include "Osako/SaveFile.h"

class SystemFile : public SaveFile {
    SystemFile();

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
    virtual ~SystemFile();                 // 54

    // vtable 0x0
    // aligned buffer?
    u8 _4[0x20 - 0x4];
    u8 _20[3072];
    u8 _c20[512];
    u8 _e20[3072];
    u8 _1a20[512];
};

extern SystemFile gSystemFile;

#endif