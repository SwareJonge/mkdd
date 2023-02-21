#ifndef JPEFFECTMGR_H
#define JPEFFECTMGR_H

#include "JSystem/JKernel/JKRHeap.h"

class JPEffectMgr {
public:
    JPEffectMgr(JKRHeap *, bool);
    void calc_forMenu();

    void reset();

    static JPEffectMgr * getEffectMgr() {
        return mThis;
    }

    static JPEffectMgr * mThis;
};

void CreateJPAMgr(JKRHeap *, bool);

inline JPEffectMgr *GetJPAMgr() { return JPEffectMgr::getEffectMgr(); }

#endif