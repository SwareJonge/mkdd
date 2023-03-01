#ifndef JPEFFECTMGR_H
#define JPEFFECTMGR_H

#include "JSystem/JKernel/JKRHeap.h"

class JPEffectMgr {
public:
    JPEffectMgr(JKRHeap *, bool, u8);
    void calc();
    void calc_forMenu();

    void reset();

    static void createMgr(JKRHeap * heap, bool p2) {
        mThis = new JPEffectMgr(heap, p2, 1);
    }

    static JPEffectMgr *getEffectMgr() {
        return mThis;
    }

    static JPEffectMgr * mThis;
private:
    u8 _00[0x38];
};

inline void CreateJPAMgr(JKRHeap * heap, bool p2) { JPEffectMgr::createMgr(heap, p2); };

inline JPEffectMgr *GetJPAMgr() { return JPEffectMgr::getEffectMgr(); }

#endif