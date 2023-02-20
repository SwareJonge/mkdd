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

    inline void enablePause();

    static PauseManager *getManager() {
        return mThis;
    }
    static PauseManager *mThis;

private:
    u8 _00[0x13];
    bool mExecFlag;

    u8 _14[0x3c - 0x14];
};

inline PauseManager *GETPauseManager() {
    return PauseManager::getManager();
}

#endif