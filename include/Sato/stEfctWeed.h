#ifndef STEFCTWEED_H
#define STEFCTWEED_H

#include "Sato/stEmitter.h"

struct stEfctWeedBase : stEmitterCallBack
{
    stEfctWeedBase();
    ~stEfctWeedBase() {}
    virtual void init() {}
    virtual int getGeneratePos(JGeometry::TVec3f *, Mtx m); // overide
};

struct stEfctWeed : stEfctWeedBase
{
    stEfctWeed();        // 0x80254ca0
    virtual void calc(); // 0x80254d58, overide
};

// Unused struct it seems
struct stEfctWeedDrift : stEfctWeedBase
{
    stEfctWeedDrift();
    virtual void calc();
};

#endif // STEFCTWEED_H
