#ifndef PAUSEMANAGER_H
#define PAUSEMANAGER_H

#include "JSystem/JKernel/JKRHeap.h"
#include "types.h"

class PauseManager {
public:
    PauseManager(JKRHeap *);    
    void setExecFlag(bool flag) {
        mExecFlag = flag;
    }
    void reset();

    void enablePause() {
        _14 = 0;
    }

    static PauseManager *getManager() {
        return mThis;
    }
    static PauseManager *mThis;

private:
    u8 _00[0x13];
    bool mExecFlag;
    u32 _14;
    u8 _18[0x3c - 0x18];
};

inline PauseManager *GETPauseManager() {
    return PauseManager::getManager();
}

#endif