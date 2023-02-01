#ifndef GAMEAPP_H
#define GAMEAPP_H

#include "types.h"
#include "JSystem/JKernel/JKRHeap.h"

class GameApp
{
public:
    GameApp(u32, const char *, void *);
    virtual ~GameApp();
    virtual void draw() { return; };
    virtual void calc() { return; };
    virtual void reset() { return; };

    JKRHeap * getHeap() { 
        return mHeap;
    }

protected:
    JKRHeap *mHeap;
    void *mSequenceHeap; // this is null for next to every App, except for MovieApp
};

#endif