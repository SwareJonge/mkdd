#ifndef _JSYSTEM_JKR_JKRFILE_H
#define _JSYSTEM_JKR_JKRFILE_H

#include "JSystem/JKernel/JKRDisposer.h"

class JKRFile : JKRDisposer{
public:
    virtual ~JKRFile();

    virtual bool open(const char *fileName)                         = 0;
    virtual void close()                                            = 0;
    virtual s32 writeData(const void *addr, s32 length, s32 offset) = 0;
    virtual s32 readData(void *addr, s32 length, s32 offset)        = 0;
    virtual s32 getFileSize() const                                 = 0;
};

#endif