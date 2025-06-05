#ifndef JAUDIO_JASKERNEL_H
#define JAUDIO_JASKERNEL_H

#include "JSystem/JAudio/System/JASHeapCtrl.h"
#include "types.h"

class JASHeap;
class JKRHeap;
class JKRSolidHeap;

namespace JASKernel
{
    void setupRootHeap(JKRSolidHeap *, u32);
    JKRHeap *getSystemHeap();
    JASMemChunkPool<1024, JASThreadingModel::ObjectLevelLockable> *getCommandHeap();
    void setupAramHeap(u32, u32);
    JASHeap *getAramHeap();

    extern u32 sAramBase;
    extern JKRHeap *sSystemHeap;
    extern JASMemChunkPool<1024, JASThreadingModel::ObjectLevelLockable> *sCommandHeap;
};

#endif
