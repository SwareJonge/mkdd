#ifndef STEFFECTMGR_H
#define STEFFECTMGR_H

#include "JSystem/JKernel/JKRSolidHeap.h"
#include "types.h"

struct stEffectMgr {
    void init();
    void createModel(JKRSolidHeap *, u32);
};

void CreateStEfctMgr();
inline stEffectMgr *GetStEfctMgr();

#endif