#ifndef J2DMANAGER_H
#define J2DMANAGER_H

#include "JSystem/JKernel/JKRHeap.h"
#include "types.h"

class J2DManager {
public:
    J2DManager(JKRHeap *);
    void setDrawFlag(bool);
    void reset();
    void startLANNumAnm();

    static J2DManager * getManager() {
        return mThis;
    }

    static J2DManager * mThis;

    u8 _0[0x48];
};

#endif