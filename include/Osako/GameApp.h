#ifndef GAMEAPP_H
#define GAMEAPP_H

#include "types.h"
#include "JSystem/JKernel/JKRHeap.h"

class GameApp
{
public:
    GameApp(u32 heapSize, const char *appName, void *heapMem);
    virtual ~GameApp();
    virtual void draw() {};
    virtual void calc() {};
    virtual void reset() {};

    JKRHeap *getHeap() { return mHeap; }

protected:
    JKRHeap *mHeap; // 4
    void *mHeapMem; // 8
};

#endif