#ifndef FORCEEFFECTMGR_H
#define FORCEEFFECTMGR_H

#include <JSystem/JKernel/JKRDisposer.h>

#include "types.h"

class ForceEffectMgr : JKRDisposer
{
public:
    ForceEffectMgr();          // 0x8020d390
    virtual ~ForceEffectMgr(); // 0x8020d3dc
    void start();              // 0x8020d444
    void end();                // 0x8020d4b8

    static void create()
    {
        if (!mspForceEffectMgr)
            mspForceEffectMgr = new ForceEffectMgr();
    }

    static void destroy() { delete mspForceEffectMgr; }
    static ForceEffectMgr *ptr() { return mspForceEffectMgr; }

private:
    static ForceEffectMgr *mspForceEffectMgr; // 0x80416a58

    u8 _18;
    s16 _1a;
}; // 1c

#endif // FORCEEFFECTMGR_H