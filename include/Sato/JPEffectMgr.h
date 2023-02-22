#ifndef JPEFFECTMGR_H
#define JPEFFECTMGR_H

#include "JSystem/JKernel/JKRHeap.h"

class JPEffectMgr {
public:
    JPEffectMgr(JKRHeap *, bool);
    void calc();
    void calc_forMenu();

    void reset();

    static void createMgr(JKRHeap * heap, bool p2) {
        mThis = new JPEffectMgr(heap, p2);
    }

    static JPEffectMgr *getEffectMgr() {
        return mThis;
    }

    static JPEffectMgr * mThis;
};

inline void CreateJPAMgr(JKRHeap * heap, bool p2) { JPEffectMgr::createMgr(heap, p2); };

inline JPEffectMgr *GetJPAMgr() { return JPEffectMgr::getEffectMgr(); }

#endif