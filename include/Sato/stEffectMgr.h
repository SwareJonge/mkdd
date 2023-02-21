#ifndef STEFFECTMGR_H
#define STEFFECTMGR_H

#include "JSystem/JKernel/JKRSolidHeap.h"
#include "types.h"

struct stEffectMgr {
    stEffectMgr();
    void init();
    void createModel(JKRSolidHeap *, u32);
    
    void reset();

    static void createMgr() { mMgr = new stEffectMgr(); }
    static stEffectMgr *getMgr() { return mMgr; }
    static stEffectMgr * mMgr;

    u8 _00[0x1398];
};

inline void CreateStEfctMgr(){ stEffectMgr::createMgr(); }
inline stEffectMgr *GetStEfctMgr(){return stEffectMgr::getMgr(); }

#endif