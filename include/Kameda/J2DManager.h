#ifndef J2DMANAGER_H
#define J2DMANAGER_H

#include "JSystem/JKernel/JKRHeap.h"
#include "types.h"

class J2DManager {
public:
    J2DManager(JKRHeap *);   
    void calc();
    void calcParticle();
    void reset();
    void startLANNumAnm();

    void setStatus2Kart(int, int);

        void setDrawFlag(bool flag)
    {
        mDrawFlag = flag;
    }
    static J2DManager * getManager() {
        return mThis;
    }

    static J2DManager * mThis;

    u8 _00[0x8];
    bool mDrawFlag;
    u8 _0A[0x48 - 0x9];
}; // Size 0x48;

#endif